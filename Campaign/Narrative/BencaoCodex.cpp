#include "Campaign/Narrative/BencaoCodex.h"

#include "Campaign/Governance/Accumulators.h"
#include "Campaign/Ledger/Ledger.h"
#include "Campaign/Myth/MythLog.h"
#include "Campaign/Myth/MythState.h"
#include "Campaign/Roster/Roster.h"

#include <algorithm>

namespace Potato::Campaign {
namespace {

using Gameplay::JsonValue;
using Gameplay::Result;

// Byte-exact tag membership — the ledger's own fold-key discipline.
bool HasTag(const LedgerEntry& e, std::string_view tag) {
    for (const std::string& t : e.tags) {
        if (t == tag) {
            return true;
        }
    }
    return false;
}

bool Triggered(const BencaoEntry& entry, const CodexSignals& sig) {
    switch (entry.unlockKind) {
    case UnlockKind::Terrain:
        return std::find(sig.terrains.begin(), sig.terrains.end(),
                         entry.unlockParam) != sig.terrains.end();
    case UnlockKind::LedgerTag:
        if (!sig.ledger) {
            return false;
        }
        for (const LedgerEntry& e : sig.ledger->Entries()) {
            if (HasTag(e, entry.unlockParam)) {
                return true;
            }
        }
        return false;
    case UnlockKind::Governance: {
        if (!sig.ledger) {
            return false;
        }
        // Resolution seals book `resolution:<kind>`; governance-flavored
        // deed tags may also match the event id bare.
        const std::string seal = "resolution:" + entry.unlockParam;
        for (const LedgerEntry& e : sig.ledger->Entries()) {
            if (HasTag(e, seal) || HasTag(e, entry.unlockParam)) {
                return true;
            }
        }
        return false;
    }
    case UnlockKind::MythState:
        if (entry.unlockParam == "infiltrated") {
            return sig.myth && !sig.chapterId.empty() &&
                   sig.myth->HasChapter(sig.chapterId);
        }
        if (!sig.mythLog) {
            return false;
        }
        for (const MythLogEntry& e : sig.mythLog->Entries()) {
            if (e.action == entry.unlockParam) {
                return true;
            }
        }
        return false;
    case UnlockKind::Corruption:
        return sig.ledger &&
               FoldGovernance(*sig.ledger).corruption >= entry.unlockInt;
    case UnlockKind::ChapterClose:
        return entry.unlockInt < 0 || entry.unlockInt == sig.chapterIndex;
    }
    return false;
}

// Deduped append: first occurrence wins, over-capacity reports a
// reason string (persisted docs are untrusted input).
const char* ReadIds(const JsonValue& arr, std::vector<std::string>& out) {
    for (const JsonValue& v : arr.Items()) {
        if (!v.IsString()) {
            return "non-string id";
        }
        if (out.size() >= BencaoCodex::MAX_ENTRIES) {
            return "overflow";
        }
        const std::string& id = v.AsString();
        if (std::find(out.begin(), out.end(), id) == out.end()) {
            out.push_back(id);
        }
    }
    return nullptr;
}

} // namespace

bool BencaoCodex::IsUnlocked(std::string_view id) const {
    return std::find(unlocked_.begin(), unlocked_.end(), id) !=
           unlocked_.end();
}

std::vector<std::string> BencaoCodex::TakePending(std::size_t max) {
    const std::size_t n = std::min(max, pending_.size());
    std::vector<std::string> out(pending_.begin(), pending_.begin() + n);
    pending_.erase(pending_.begin(), pending_.begin() + n);
    // Delivered = transcribed into the book: the drained pages move
    // from the queue to the unlocked set.
    unlocked_.insert(unlocked_.end(), out.begin(), out.end());
    return out;
}

Result<JsonValue> BencaoCodex::ToJson() const {
    JsonValue::Array unlocked;
    unlocked.reserve(unlocked_.size());
    for (const std::string& id : unlocked_) {
        unlocked.push_back(JsonValue::String(id));
    }
    JsonValue::Array pending;
    pending.reserve(pending_.size());
    for (const std::string& id : pending_) {
        pending.push_back(JsonValue::String(id));
    }
    JsonValue::Object root;
    root["schema"] = JsonValue::String(std::string(SCHEMA));
    root["unlocked"] = JsonValue::MakeArray(std::move(unlocked));
    root["pending"] = JsonValue::MakeArray(std::move(pending));
    return Gameplay::Ok(JsonValue::MakeObject(std::move(root)));
}

Result<BencaoCodex> BencaoCodex::FromJson(const JsonValue& doc) {
    if (!doc.IsObject()) {
        return Gameplay::Fail<BencaoCodex>("schema",
                                           "root is not an object");
    }
    const std::string* schema = doc.FindString("schema");
    if (!schema || *schema != SCHEMA) {
        return Gameplay::Fail<BencaoCodex>("schema",
                                           "expected potato.bencao_state/1");
    }
    if (!doc.Has("unlocked") || !doc["unlocked"].IsArray() ||
        !doc.Has("pending") || !doc["pending"].IsArray()) {
        return Gameplay::Fail<BencaoCodex>("schema",
                                           "unlocked/pending: not arrays");
    }
    BencaoCodex codex;
    if (const char* err = ReadIds(doc["unlocked"], codex.unlocked_)) {
        return Gameplay::Fail<BencaoCodex>("bencao_state", err);
    }
    // Pending ids already written into the book are stale — drop them;
    // the canonical form keeps the two lists disjoint.
    std::vector<std::string> pend;
    if (const char* err = ReadIds(doc["pending"], pend)) {
        return Gameplay::Fail<BencaoCodex>("bencao_state", err);
    }
    for (const std::string& id : pend) {
        if (!codex.IsUnlocked(id)) {
            codex.pending_.push_back(id);
        }
    }
    return Gameplay::Ok(std::move(codex));
}

Result<std::vector<std::string>>
ResolveBencaoUnlocks(const BencaoLibrary& lib, const CodexSignals& signals,
                     BencaoCodex& codex) {
    std::vector<std::string> fresh;
    for (const BencaoEntry& e : lib.Entries()) {
        if (codex.IsUnlocked(e.id)) {
            continue;
        }
        // A pending page is already claimed by the queue — don't
        // double-enqueue it.
        if (std::find(codex.pending_.begin(), codex.pending_.end(),
                      e.id) != codex.pending_.end()) {
            continue;
        }
        if (!Triggered(e, signals)) {
            continue;
        }
        codex.pending_.push_back(e.id);
        fresh.push_back(e.id);
    }
    return Gameplay::Ok(std::move(fresh));
}

std::vector<std::string>
TerrainFlagsOf(const Gameplay::BattleMap& map) {
    std::uint32_t seen = 0;
    for (std::size_t i = 0; i < map.RegionCount(); ++i) {
        seen |= map.RegionAt(i).terrain;
    }
    std::vector<std::string> out;
    // Fixed flag order — output is deterministic no matter which
    // regions carried which flags.
    static const std::pair<Gameplay::Terrain, std::string_view>
        kNames[] = {
            {Gameplay::TERRAIN_WATER, "water"},
            {Gameplay::TERRAIN_RIVER, "river"},
            {Gameplay::TERRAIN_ROAD, "road"},
            {Gameplay::TERRAIN_FOREST, "forest"},
            {Gameplay::TERRAIN_HIGHLAND, "highland"},
            {Gameplay::TERRAIN_CHOKEPOINT, "chokepoint"},
            {Gameplay::TERRAIN_OPEN, "open"},
        };
    for (const auto& [flag, name] : kNames) {
        if (seen & flag) {
            out.emplace_back(name);
        }
    }
    return out;
}

} // namespace Potato::Campaign
