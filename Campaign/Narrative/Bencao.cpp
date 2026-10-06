#include "Campaign/Narrative/Bencao.h"

#include "Campaign/Chapters/ChapterLibrary.h"
#include "Campaign/Ledger/Ledger.h"
#include "Campaign/Myth/MythActions.h"
#include "Gameplay/Json/Json.h"
#include "Gameplay/Json/JsonValue.h"

#include <algorithm>
#include <cctype>
#include <set>
#include <system_error>
#include <utility>

namespace Potato::Campaign {

using Gameplay::JsonValue;
using Gameplay::Result;

namespace {

// The closed vocabularies a trigger param can actually resolve
// against — a manifest naming anything else loads into a dead
// unlock, which is worse than a loud rejection (review D2).
bool IsTerrainFlag(std::string_view s) {
    static constexpr std::string_view kFlags[] = {
        "water", "river", "road", "forest",
        "highland", "chokepoint", "open",
    };
    for (std::string_view f : kFlags) {
        if (s == f) return true;
    }
    return false;
}

// Myth-state ids: the catalog's player verbs (MythActionDefs) plus
// the two non-purchased entries — "invasion" (the god's own move,
// folded by LogMythEvents) and "infiltrated" (MythState presence).
bool IsMythStateId(std::string_view s) {
    if (s == "infiltrated" || s == "invasion") return true;
    for (const MythActionDef& d : MythActionDefs()) {
        if (s == d.id) return true;
    }
    return false;
}

bool CategoryFromString(std::string_view s, BencaoCategory& out) {
    struct Row { std::string_view id; BencaoCategory cat; };
    static constexpr Row kRows[] = {
        {"shancao", BencaoCategory::Shancao},
        {"xicao", BencaoCategory::Xicao},
        {"ducao", BencaoCategory::Ducao},
        {"manshui", BencaoCategory::Manshui},
        {"gucai", BencaoCategory::Gucai},
        {"jinshi", BencaoCategory::Jinshi},
        {"chongshou", BencaoCategory::Chongshou},
        {"renbu", BencaoCategory::Renbu},
    };
    for (const Row& r : kRows) {
        if (s == r.id) {
            out = r.cat;
            return true;
        }
    }
    return false;
}

struct KindRow { std::string_view id; UnlockKind kind; };
constexpr KindRow kKindRows[] = {
    {"terrain", UnlockKind::Terrain},
    {"ledger_tag", UnlockKind::LedgerTag},
    {"governance", UnlockKind::Governance},
    {"myth_state", UnlockKind::MythState},
    {"corruption", UnlockKind::Corruption},
    {"chapter_close", UnlockKind::ChapterClose},
};

bool UnlockKindFromString(std::string_view s, UnlockKind& out) {
    return UnlockKindFromName(s, out);
}

// Required string field: present, non-empty, within bound.
bool ReadField(const JsonValue& doc, std::string_view key,
               std::size_t maxLen, std::string& out) {
    const std::string* s = doc.FindString(key);
    if (!s || s->empty() || s->size() > maxLen) return false;
    out = *s;
    return true;
}

// Optional free text: absent or string within bound.
bool ReadText(const JsonValue& doc, std::string_view key,
              std::size_t maxLen, std::string& out) {
    if (!doc.Has(key)) return true;
    if (!doc[key].IsString() || doc[key].AsString().size() > maxLen) {
        return false;
    }
    out = doc[key].AsString();
    return true;
}

const char* ValidateUnlock(const JsonValue& u, BencaoEntry& out) {
    if (!u.IsObject()) return "unlock is not an object";
    const std::string* kind = u.FindString("kind");
    if (!kind || !UnlockKindFromString(*kind, out.unlockKind)) {
        return "unknown unlock kind";
    }
    switch (out.unlockKind) {
    case UnlockKind::Terrain:
        if (!ReadField(u, "terrain",
                       BencaoLibrary::MAX_UNLOCK_PARAM_LEN,
                       out.unlockParam)) {
            return "unlock.terrain missing/out of range";
        }
        // Closed vocab: the only ids TerrainFlagsOf can emit.
        if (!IsTerrainFlag(out.unlockParam)) {
            return "unlock.terrain is not a TERRAIN_* flag id";
        }
        break;
    case UnlockKind::LedgerTag:
        // A tag longer than MAX_TAG_LEN can never be posted.
        if (!ReadField(u, "tag", Ledger::MAX_TAG_LEN,
                       out.unlockParam)) {
            return "unlock.tag missing/out of range";
        }
        break;
    case UnlockKind::Governance:
        // The event id must fit a `resolution:<event>` seal tag —
        // longer ids resolve through the bare-tag path only.
        if (!ReadField(u, "event",
                       Ledger::MAX_TAG_LEN - Ledger::TAG_RESOLUTION.size(),
                       out.unlockParam)) {
            return "unlock.event missing/out of range";
        }
        break;
    case UnlockKind::MythState:
        if (!ReadField(u, "state",
                       BencaoLibrary::MAX_UNLOCK_PARAM_LEN,
                       out.unlockParam)) {
            return "unlock.state missing/out of range";
        }
        if (!IsMythStateId(out.unlockParam)) {
            return "unlock.state is not a myth action/state id";
        }
        break;
    case UnlockKind::Corruption:
        // at_least 0 degenerates to "any non-null ledger" — the
        // trigger must name a real threshold.
        if (!u["at_least"].IsInt() ||
            u["at_least"].AsInt() < 1 ||
            u["at_least"].AsInt() > BencaoLibrary::MAX_UNLOCK_INT) {
            return "unlock.at_least out of range";
        }
        out.unlockInt = u["at_least"].AsInt();
        break;
    case UnlockKind::ChapterClose:
        // -1 = every chapter close; otherwise a real chapter index.
        if (!u["chapter"].IsInt() ||
            u["chapter"].AsInt() < -1 ||
            u["chapter"].AsInt() >= static_cast<std::int64_t>(
                ChapterLibrary::MAX_CHAPTERS)) {
            return "unlock.chapter out of range";
        }
        out.unlockInt = u["chapter"].AsInt();
        break;
    }
    return nullptr;
}

// One file -> BencaoEntry or a reason string. Pure validation:
// unknown members are ignored (forward-compat) except the reserved
// annotation keys — 批註 is runtime-bound, files cannot author it.
const char* ValidateBencao(const JsonValue& doc, BencaoEntry& out) {
    if (!doc.IsObject()) return "root is not an object";
    // The reserved annotation keys — English spellings AND the CJK
    // name an author would naturally reach for. 批註 is
    // runtime-bound: files that try to author it are rejected.
    if (doc.Has("annotation") || doc.Has("annotations") ||
        doc.Has("marginalia") || doc.Has("批註") ||
        doc.Has("批注")) {
        return "annotation fields are runtime-bound";
    }
    if (!ReadField(doc, "id", BencaoLibrary::MAX_ID_LEN, out.id) ||
        !ReadField(doc, "name", BencaoLibrary::MAX_NAME_LEN,
                   out.name) ||
        !ReadField(doc, "nature", BencaoLibrary::MAX_TEXT_LEN,
                   out.nature) ||
        !ReadField(doc, "indications", BencaoLibrary::MAX_TEXT_LEN,
                   out.indications) ||
        !ReadField(doc, "source", BencaoLibrary::MAX_SOURCE_LEN,
                   out.source)) {
        return "missing/mistyped field";
    }
    const std::string* cat = doc.FindString("category");
    if (!cat || !CategoryFromString(*cat, out.category)) {
        return "unknown category";
    }
    if (doc.Has("aliases")) {
        if (!doc["aliases"].IsArray() ||
            doc["aliases"].Size() > BencaoLibrary::MAX_ALIASES) {
            return "aliases out of range";
        }
        for (const JsonValue& a : doc["aliases"].Items()) {
            if (!a.IsString() || a.AsString().empty() ||
                a.AsString().size() > BencaoLibrary::MAX_NAME_LEN) {
                return "alias out of range";
            }
            out.aliases.push_back(a.AsString());
        }
    }
    if (!ReadText(doc, "origin", BencaoLibrary::MAX_TEXT_LEN,
                  out.origin)) {
        return "origin out of range";
    }
    if (!doc.Has("unlock")) return "missing/mistyped field";
    if (const char* why = ValidateUnlock(doc["unlock"], out)) {
        return why;
    }
    if (doc.Has("lang")) {
        if (!doc["lang"].IsObject()) return "lang is not an object";
        // zh-tw is inherent to the entry text; the block may add
        // other languages but must not assert zh-tw false. en rides
        // ignored (OQ-B3).
        const JsonValue& zh = doc["lang"]["zh-tw"];
        if (zh.IsBool() && !zh.AsBool()) {
            return "lang.zh-tw must not be false";
        }
    }
    return nullptr;
}

} // namespace

const char* UnlockKindName(UnlockKind k) {
    switch (k) {
    case UnlockKind::Terrain:      return "terrain";
    case UnlockKind::LedgerTag:    return "ledger_tag";
    case UnlockKind::Governance:   return "governance";
    case UnlockKind::MythState:    return "myth_state";
    case UnlockKind::Corruption:   return "corruption";
    case UnlockKind::ChapterClose: return "chapter_close";
    }
    return "chapter_close";
}

bool UnlockKindFromName(std::string_view s, UnlockKind& out) {
    for (const KindRow& r : kKindRows) {
        if (s == r.id) {
            out = r.kind;
            return true;
        }
    }
    return false;
}

BencaoLoadResult BencaoLibrary::Load(const std::filesystem::path& dir,
                                     BencaoLibrary& out) {
    BencaoLoadResult res;

    std::error_code ec;
    std::vector<std::filesystem::path> files;
    for (std::filesystem::directory_iterator it(
             dir, std::filesystem::directory_options::none, ec);
         !ec && it != std::filesystem::directory_iterator();
         it.increment(ec)) {
        // Per-entry stat uses its OWN error channel — sharing `ec`
        // would let a stat failure be cleared by the next
        // increment() and silently drop the file.
        std::error_code ec2;
        const bool isFile = it->is_regular_file(ec2);
        if (ec2) {
            res.rejected.push_back(
                {it->path(), "io", "stat failed: " + ec2.message()});
            continue;
        }
        // Case-folded extension: X.JSON/X.Json are still content.
        std::string ext = it->path().extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(),
                       [](unsigned char c) {
                           return static_cast<char>(std::tolower(c));
                       });
        if (isFile && ext == ".json") {
            files.push_back(it->path());
        }
    }
    if (ec) {
        res.error = "io";
        res.reason = "bencao dir unreadable: " + ec.message();
        return res;
    }
    // Deterministic registration order: filename sort (category
    // order is applied at commit below).
    std::sort(files.begin(), files.end());

    std::vector<BencaoEntry> entries;
    std::set<std::string> ids;
    for (const std::filesystem::path& f : files) {
        const std::u8string u8 = f.u8string();
        Result<JsonValue> doc = Gameplay::Json::Load(
            std::string_view(
                reinterpret_cast<const char*>(u8.data()),
                u8.size()),
            SCHEMA);
        if (!doc.ok()) {
            res.rejected.push_back({f, doc.error, doc.reason});
            continue;
        }
        BencaoEntry e;
        if (const char* why = ValidateBencao(doc.value, e)) {
            res.rejected.push_back({f, "field", why});
            continue;
        }
        if (ids.count(e.id)) {
            res.rejected.push_back(
                {f, "duplicate", "bencao id already registered"});
            continue;
        }
        ids.insert(e.id);
        entries.push_back(std::move(e));
    }
    if (entries.size() > MAX_ENTRIES) {
        // Overflow can't be a per-file rejection (already committed);
        // fail wholesale — a library beyond bound is a content bug.
        res.error = "overflow";
        res.reason = "bencao entries exceed MAX_ENTRIES";
        return res;
    }

    // Canonical order: category ordinal, then id.
    std::sort(entries.begin(), entries.end(),
              [](const BencaoEntry& a, const BencaoEntry& b) {
                  if (a.category != b.category) {
                      return static_cast<int>(a.category) <
                             static_cast<int>(b.category);
                  }
                  return a.id < b.id;
              });
    out.entries_ = std::move(entries);
    res.ok = true;
    return res;
}

const BencaoEntry* BencaoLibrary::Find(std::string_view id) const {
    for (const BencaoEntry& e : entries_) {
        if (e.id == id) return &e;
    }
    return nullptr;
}

} // namespace Potato::Campaign
