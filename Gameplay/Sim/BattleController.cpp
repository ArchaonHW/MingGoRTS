#include "Gameplay/Sim/BattleController.h"

#include "Gameplay/Map/BattleMap.h"
#include "Gameplay/Squad/Squad.h"

#include <utility>

namespace Potato::Gameplay {

namespace {

// Numeric fold — deterministic mix of a 64-bit value into an accumulator.
std::uint64_t Fold(std::uint64_t acc, std::uint64_t v) {
    acc ^= v + 0x9E3779B97F4A7C15ull + (acc << 6) + (acc >> 2);
    return acc;
}

// Normalize width-dependent sentinels (NO_REGION is ~size_t{0}) so the
// checksum is identical on 32- and 64-bit targets.
std::uint64_t RegionKey(std::size_t r) {
    return r == Squad::NO_REGION ? ~std::uint64_t{0}
                                 : static_cast<std::uint64_t>(r);
}

} // namespace

BattleController::BattleController(std::uint64_t seed, const BattleMap& map,
                                   const DoctrineLibrary& cards)
    : map_(map), cards_(cards), sim_(seed) {}

BattleController::~BattleController() = default;
BattleController::BattleController(const BattleController&) = default;

void BattleController::EmitBeatChanged(int tick, BattleBeat target) {
    events_.push_back({SimEvent::Kind::BeatChanged, tick, -1, -1,
                       static_cast<int>(target), {}});
}

bool BattleController::RequestBeat(BattleBeat target) {
    if (!CanTransition(beat_, target)) return false;
    // BeatChanged.tick = index of the tick the transition followed:
    // P->E fires before any tick (0); the auto-close path inside Tick()
    // stamps the producing tick via EmitBeatChanged directly.
    beat_ = target;
    EmitBeatChanged(static_cast<int>(sim_.TickCount()), target);
    if (target == BattleBeat::Aftermath) CloseBattle(true);
    return true;
}

bool BattleController::DeploySquad(const SquadTemplate& t,
                                   std::size_t regionIndex, int side) {
    if (beat_ != BattleBeat::Planning) return false;
    if (regionIndex >= map_.RegionCount()) return false; // incl. NO_REGION
    if (side != 0 && side != 1) return false;            // invisible faction
    squads_.push_back(Squad::Instantiate(t, regionIndex, side));
    sheets_.push_back(SquadSheet{}); // empty sheet = no doctrine
    routTimers_.push_back(0);
    return true;
}

bool BattleController::SetSheet(std::size_t squadIndex, SquadSheet sheet) {
    if (beat_ != BattleBeat::Planning) return false;
    if (squadIndex >= squads_.size()) return false;
    sheets_[squadIndex] = std::move(sheet);
    return true;
}

bool BattleController::SetCpPool(int cp) {
    if (beat_ != BattleBeat::Planning) return false;
    cpPool_ = cp < 0 ? 0 : cp;
    return true;
}

bool BattleController::Tick() {
    if (beat_ != BattleBeat::Execution) return false;

    const int tick = static_cast<int>(sim_.TickCount());
    // 1) doctrine eval on tick-start state (EvalTick is const on squads_)
    EvalOutcome out = Doctrine::EvalTick(map_, squads_, sheets_, cards_,
                                         sim_.Rng(), cpPool_, tick);
    // 2) commit pending deltas post-eval
    Doctrine::ApplyDeltas(squads_, out.deltas);
    // 3) squads advance edge traversal; Routing squads age off-field
    for (std::size_t i = 0; i < squads_.size(); ++i) {
        Squad& s = squads_[i];
        s.TickMove();
        if (s.state == SquadState::Routing) {
            if (++routTimers_[i] >= ROUT_TICKS) {
                s.ApplyEvent(SquadEvent::RetreatComplete); // -> Routed
            }
        } else {
            routTimers_[i] = 0;
        }
    }
    // 4) append this tick's events to the battle log
    for (const SimEvent& e : out.events) events_.push_back(e);
    // 5) sim housekeeping: tick counter + stream draw + stream checksum
    sim_.Tick();
    // 6) a wiped side closes the battle — same tick, after all commits;
    //    BeatChanged is stamped with the producing tick's index so every
    //    event of the closing tick shares one tick value.
    if (CountEffective(0) == 0 || CountEffective(1) == 0) {
        beat_ = BattleBeat::Aftermath;
        EmitBeatChanged(tick, BattleBeat::Aftermath);
        CloseBattle(false);
    }
    return true;
}

int BattleController::CountEffective(int side) const {
    int n = 0;
    for (const Squad& s : squads_) {
        if (s.side == side && s.IsEffective()) ++n;
    }
    return n;
}

void BattleController::CloseBattle(bool forced) {
    outcome_.forced = forced;
    outcome_.elapsedTicks = sim_.TickCount();
    for (int side = 0; side < 2; ++side) {
        for (const Squad& s : squads_) {
            if (s.side != side) continue;
            switch (s.state) {
                case SquadState::Routed:    ++outcome_.routed[side]; break;
                case SquadState::Destroyed: ++outcome_.destroyed[side]; break;
                default:                    ++outcome_.effective[side]; break;
            }
        }
    }
    const int eff0 = outcome_.effective[0], eff1 = outcome_.effective[1];
    outcome_.winnerSide = (eff0 > 0 && eff1 == 0) ? 0
                        : (eff1 > 0 && eff0 == 0) ? 1 : -1;
}

std::uint64_t BattleController::Checksum() const {
    std::uint64_t h = sim_.Checksum();
    h = Fold(h, sim_.TickCount());
    h = Fold(h, static_cast<std::uint64_t>(beat_));
    h = Fold(h, static_cast<std::uint64_t>(cpPool_));
    h = Fold(h, static_cast<std::uint64_t>(squads_.size()));
    for (std::size_t i = 0; i < squads_.size(); ++i) {
        const Squad& s = squads_[i];
        h = Fold(h, static_cast<std::uint64_t>(s.maxHp));
        h = Fold(h, static_cast<std::uint64_t>(s.hp));
        h = Fold(h, static_cast<std::uint64_t>(s.attack));
        h = Fold(h, static_cast<std::uint64_t>(s.speedMilli));
        h = Fold(h, static_cast<std::uint64_t>(s.cohesion));
        h = Fold(h, static_cast<std::uint64_t>(s.side));
        h = Fold(h, static_cast<std::uint64_t>(s.state));
        h = Fold(h, RegionKey(s.regionIndex));
        h = Fold(h, RegionKey(s.edgeTarget));
        h = Fold(h, static_cast<std::uint64_t>(s.edgeProgress));
        h = Fold(h, static_cast<std::uint64_t>(routTimers_[i]));
        for (const CardSlot& slot : sheets_[i].slots) {
            h = Fold(h, static_cast<std::uint64_t>(slot.cardIndex));
            h = Fold(h, static_cast<std::uint64_t>(slot.cooldownRemaining));
        }
    }
    return h;
}

} // namespace Potato::Gameplay
