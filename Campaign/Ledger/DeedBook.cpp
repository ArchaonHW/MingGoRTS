#include "Campaign/Ledger/DeedBook.h"

#include <string>
#include <utility>

namespace Potato::Campaign {

namespace {

using Gameplay::SimEvent;

// Tag vocabulary — the fold keys Epic 4's accumulators read.
// TAG_ATROCITY (Ledger.h) marks the unforgivable; "raid" marks
// commerce raiding (war, not atrocity); region/victim tags keep
// the fold placeable.
std::string RegionTag(int region) {
    return "region:" + std::to_string(region);
}

// deed -> posting legs; returns false for non-deed kinds.
bool DeedPosting(const SimEvent& e, Posting& p) {
    switch (e.kind) {
        case SimEvent::Kind::VillageOccupied:
            p = {{Account::PopularSupport, 10}, {Account::Materiel, 5},
                 "occupied village", {RegionTag(e.param)}};
            return true;
        case SimEvent::Kind::VillageBurned:
            p = {{Account::Materiel, 40}, {Account::PopularSupport, 15},
                 "burned village",
                 {std::string(Ledger::TAG_ATROCITY), RegionTag(e.param)}};
            return true;
        case SimEvent::Kind::ConvoyArrived:
            p = {{Account::Materiel, 30}, {Account::PopularSupport, 5},
                 "convoy arrived", {RegionTag(e.param)}};
            return true;
        case SimEvent::Kind::ConvoyRaided:
            p = {{Account::Materiel, 25}, {Account::PopularSupport, 5},
                 "raided convoy", {"raid", RegionTag(e.param)}};
            return true;
        case SimEvent::Kind::SquadExecuted:
            // "victim:" names the murdered squad — every other kind's
            // squadIndex is the perpetrator; conflating them would
            // attribute the atrocity to the wrong index.
            p = {{Account::ArmyPrestige, 5},
                 {Account::PopularSupport, 10},
                 "refused rout-surrender",
                 {std::string(Ledger::TAG_ATROCITY),
                  "victim:" + std::to_string(e.squadIndex),
                  RegionTag(e.param)}};
            return true;
        default:
            return false; // CardFired/BeatChanged/... aren't deeds
    }
}

} // namespace

Gameplay::Result<std::size_t>
BookDeeds(Ledger& ledger, int playerSide,
          std::span<const Gameplay::SimEvent> events) {
    if (playerSide != 0 && playerSide != 1) {
        return Gameplay::Fail<std::size_t>("deeds",
                                           "side must be 0 or 1");
    }
    std::size_t posted = 0;
    for (const SimEvent& e : events) {
        // Only the player's deeds book into the player's ledger —
        // an enemy arsonist gains nothing from our 物資 account.
        if (e.side != playerSide) continue;
        Posting p;
        if (!DeedPosting(e, p)) continue;
        auto r = ledger.Post(std::move(p));
        // A rejected deed aborts the fold. Entries already posted
        // STAY posted — the chain is append-only history, not a
        // transaction — so the failure reason names the count and
        // a retry must resume, not re-fold (no idempotency key).
        if (!r.ok()) {
            return Gameplay::Fail<std::size_t>(
                "deeds", "posting rejected after " +
                             std::to_string(posted) +
                             " deeds: " + r.reason);
        }
        ++posted;
    }
    return Gameplay::Ok(posted);
}

} // namespace Potato::Campaign
