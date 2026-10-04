#pragma once

#include "Campaign/Ledger/Ledger.h"
#include "Gameplay/Json/JsonValue.h"
#include "Gameplay/Result.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace Potato::Campaign {

// Chapter progression spine (stories 3.3/3.4 grow the model).
// `unlocked`/`resolved` are parallel vectors sized to the chapter
// space; `current` indexes into them — empty vectors require
// current==0, and current==size() is the campaign-complete
// sentinel.
struct ChapterProgress {
    std::int64_t current = 0;           // active chapter index
    std::vector<bool> unlocked;         // per-chapter unlock flags
    std::vector<bool> resolved;         // per-chapter resolved flags
};

// Named roster squad spine (story 3.5 grows the model; name
// uniqueness and richer invariants are its business logic).
struct RosterEntry {
    std::string name;
    std::int64_t veterancy = 0;   // battles survived
    std::int64_t casualties = 0;  // cumulative dead
    bool dead = false;            // wiped/disbanded — stays dead
};

// The campaign facade: ONE versioned document covering every
// cross-chapter store. Save/load is a single operation — a bad
// field rejects the whole document without mutating caller state.
//
// The ledger is embedded as its own `potato.ledger/3` sub-document:
// its seal/chain verification rides inside Ledger::FromJson, so a
// campaign file carrying a broken chain fails at load. Chapter and
// roster are minimal spines this story persists; 3.3–3.5 grow them
// via schema bumps.
class CampaignState {
public:
    static constexpr std::string_view SCHEMA = "potato.campaign/1";
    static constexpr std::size_t MAX_ROSTER = 256;
    static constexpr std::size_t MAX_CHAPTERS = 64;
    // Bytes, not codepoints — ~21 CJK chars. Name uniqueness and
    // content rules are 3.5's; this is a wire bound.
    static constexpr std::size_t MAX_NAME_LEN = 64;
    // veterancy/casualties bound — mirrors the file-is-untrusted
    // policy: a load must not admit counters no write path could
    // produce.
    static constexpr std::int64_t MAX_ROSTER_COUNT = 1000000;

    Ledger& GetLedger() { return ledger_; }
    const Ledger& GetLedger() const { return ledger_; }
    ChapterProgress& GetChapter() { return chapter_; }
    const ChapterProgress& GetChapter() const { return chapter_; }
    std::vector<RosterEntry>& GetRoster() { return roster_; }
    const std::vector<RosterEntry>& GetRoster() const {
        return roster_;
    }

    Gameplay::Result<Gameplay::JsonValue> ToJson() const;
    static Gameplay::Result<CampaignState> FromJson(
        const Gameplay::JsonValue& doc);

private:
    Ledger ledger_;
    ChapterProgress chapter_;
    std::vector<RosterEntry> roster_;
};

} // namespace Potato::Campaign
