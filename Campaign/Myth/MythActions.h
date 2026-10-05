#pragma once

#include "Campaign/Ledger/Ledger.h"
#include "Campaign/Myth/MythLog.h"
#include "Gameplay/Myth/Infiltration.h" // MythActionKind
#include "Gameplay/Result.h"

#include <cstdint>
#include <span>

namespace Potato::Campaign {

// The myth-action catalog (Story 5.4): every 天命-spending verb the
// player can author. The sim-side effect lives in Gameplay
// (MythField / Squad via the IssueMyth* verbs); THIS table prices
// it — 天命 debit through SpendMandate and a credit sink that banks
// the deed's echo into the ledger's other accounts.
//
// Sink choices are the GDD's "mythic actions amplify the 民心 axis":
// pacifying converts into governance capital directly, possession
// honors the host's line (武功), a ghost army banks dread (軍威).
// Costs track the GDD tempo table (~15 early / ~25 mid bands).
struct MythActionDef {
    Gameplay::MythActionKind kind;
    const char* id;          // canonical ASCII wire id
    const char* name;        // chronicle name — MythLog stores this
    std::int64_t cost;       // 天命 debit
    Account sink;            // credit leg — where the echo lands
    std::int64_t sinkAmount;
    bool needsSquad;         // target is a squad (possession), not
                             // a region (pacify / ghost army)
};

// The catalog — array order is the enum order.
std::span<const MythActionDef> MythActionDefs();
const MythActionDef* FindMythAction(Gameplay::MythActionKind kind);

// The purchase seam: validate the kind, pay 天命 through
// SpendMandate (insufficient funds reject BEFORE anything posts),
// and enter the act into the MythLog BY NAME. The host then issues
// the battle verb — pairing the spend to a sim effect is the
// caller's contract (an unpaid act in a record is an audit finding;
// a paid act never issued is just a donation).
//
// Returns the ledger seq of the spend entry — the durable anchor
// tying the miracle to its price.
Gameplay::Result<std::uint64_t>
PerformMythAction(Ledger& ledger, MythLog& log,
                  Gameplay::MythActionKind kind, int side,
                  int region, int squad);

} // namespace Potato::Campaign
