#pragma once

#include "Campaign/Narrative/Bencao.h"

#include "Gameplay/Json/JsonValue.h"
#include "Gameplay/Map/BattleMap.h"
#include "Gameplay/Result.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace Potato::Campaign {

class Ledger;
class MythLog;
class MythState;
struct RosterEntry;

// The codex's persistent state (Story 10.2) — which pages the book
// has grown, and which new pages are still waiting for the 補鈔
// delivery pass (Story 10.3). Sibling doc `potato.bencao_state/1`,
// same pattern as potato.myth/1 / potato.rivals/1: schema revisions
// stay independent of potato.campaign.
//
// Wire shape:
//   {"schema":"potato.bencao_state/1",
//    "unlocked":["sanqi",...],   // insertion order = unlock order
//    "pending":["fuzi",...]}     // 補鈔 queue, FIFO
//
// Canonical form: `unlocked` and `pending` are disjoint — a page is
// either awaiting 補鈔 (pending) or already written into the book
// (unlocked). `unlocked` IS the citable set (Story 10.5): a page
// the book doesn't hold yet can't be quoted.
class BencaoCodex {
public:
    static constexpr std::string_view SCHEMA = "potato.bencao_state/1";
    // A codex can never hold more pages than a library can host.
    static constexpr std::size_t MAX_ENTRIES = BencaoLibrary::MAX_ENTRIES;

    // Pages written into the book — delivered 補鈔. `false` for
    // pages still pending (known to the Scribe, not yet on paper).
    bool IsUnlocked(std::string_view id) const;
    // Insertion order = delivery order; the store keeps history,
    // 10.4's renderer re-sorts for display.
    const std::vector<std::string>& Unlocked() const { return unlocked_; }
    // FIFO queue of pages awaiting 補鈔 delivery.
    const std::vector<std::string>& Pending() const { return pending_; }

    // The 10.3 delivery seam: the 補鈔 move itself. Pops up to `max`
    // pending ids in queue order, writes them into the book
    // (unlocked), and returns them. `max == 0` delivers nothing.
    std::vector<std::string> TakePending(std::size_t max);

    Gameplay::Result<Gameplay::JsonValue> ToJson() const;
    // Rejects: bad schema, non-string ids, over-capacity lists.
    // Duplicates dedupe (first occurrence wins) — saves written by
    // older code may overlap unlocked/pending; unlocked wins.
    static Gameplay::Result<BencaoCodex> FromJson(
        const Gameplay::JsonValue& doc);

private:
    friend Gameplay::Result<std::vector<std::string>>
    ResolveBencaoUnlocks(const BencaoLibrary& lib,
                         const struct CodexSignals& signals,
                         BencaoCodex& codex);

    std::vector<std::string> unlocked_;
    std::vector<std::string> pending_;
};

// Read-only signal bundle the caller assembles at chapter settle
// (Story 10.2). Every pointer is a view — the engine writes nothing
// outside the codex. Null means "that store isn't wired here", and
// every trigger kind depending on it simply evaluates false;
// chapter_close works with no stores at all.
struct CodexSignals {
    const Ledger* ledger = nullptr;     // ledger_tag, governance, corruption
    const MythLog* mythLog = nullptr;   // myth_state (action membership)
    const MythState* myth = nullptr;    // myth_state "infiltrated"
    // Exposed for the deferred veterancy-keyed unlocks (續斷/骨碎補
    // wait on RosterEntry::scars); the six kinds don't read it yet.
    const std::vector<RosterEntry>* roster = nullptr;
    std::string_view chapterId;         // chapter that just settled
    std::int64_t chapterIndex = -1;     // for chapter_close; -1 = none
    // Lowercase TERRAIN_* flag ids present on the chapter map —
    // assemble via TerrainFlagsOf; the engine never loads maps.
    std::vector<std::string> terrains;
};

// The unlock engine. Walks the library in canonical order
// (category, then id — Entries() order IS the eval order), tests
// each entry's trigger against `signals`, and enqueues every new
// match in `codex`'s pending queue. Idempotent: a page already
// unlocked or pending is never claimed twice.
//
// Trigger semantics per UnlockKind:
//   terrain       — signals.terrains contains unlockParam
//   ledger_tag    — some LedgerEntry.tags carries unlockParam
//   governance    — some entry carries tag "resolution:<param>" or
//                   bare "<param>" (resolution seals + deed tags)
//   myth_state    — "infiltrated" → myth->HasChapter(chapterId);
//                   else any MythLog entry.action == unlockParam
//   corruption    — FoldGovernance(*ledger).corruption >= unlockInt
//   chapter_close — unlockInt == -1 (every settle) or == chapterIndex
//
// Returns the newly unlocked ids in canonical order.
Gameplay::Result<std::vector<std::string>>
ResolveBencaoUnlocks(const BencaoLibrary& lib,
                     const CodexSignals& signals,
                     BencaoCodex& codex);

// Distinct TERRAIN_* flag names present on the map, lowercase ids
// ("water","river","road","forest","highland","chokepoint","open"),
// in flag order — deterministic regardless of region declaration
// order. The caller loads `ChapterDef.map` itself; this unit owns
// no file I/O.
std::vector<std::string>
TerrainFlagsOf(const Gameplay::BattleMap& map);

} // namespace Potato::Campaign
