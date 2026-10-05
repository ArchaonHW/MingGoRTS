#pragma once

#include "Campaign/Ledger/Ledger.h"
#include "Gameplay/Doctrine/Doctrine.h" // SimEvent
#include "Gameplay/Result.h"

#include <cstddef>
#include <span>

namespace Potato::Campaign {

// Battle deeds -> ledger postings (Story 4.2, the A-2 auto-detection
// seam). The recorded SimEvent stream is the truth: BookDeeds folds
// the player's deeds into double-entry postings — atrocity kinds
// (VillageBurned, SquadExecuted) carry the "atrocity" tag that the
// 墮落 ratchet folds (4.3 — computed, never written).
//
// Scope: only deeds committed by `playerSide` post to this ledger —
// it is the player's domain chronicle; enemy deeds are the enemy's
// ledger problem (and survive in the battle record regardless).
// Event kinds that aren't deeds (CardFired, BeatChanged, ...) are
// skipped silently; deed events stamped -1 or for the other side
// post nothing.
//
// Leg pricing is the architecture's example economics — asymmetric
// costs are the design: burning banks 物資 against 民心, execution
// banks 軍威 dread against 民心 revulsion. Magnitudes are initial
// balance targets, not final tuning.
Gameplay::Result<std::size_t>
BookDeeds(Ledger& ledger, int playerSide,
          std::span<const Gameplay::SimEvent> events);

} // namespace Potato::Campaign
