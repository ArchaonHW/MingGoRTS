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
    events_.push_back({SimEvent::Kind::BeatChanged, tick, -1, -1, -1,
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

bool BattleController::SetCpPool(int side, int cp) {
    if (beat_ != BattleBeat::Planning) return false;
    if (side != 0 && side != 1) return false;
    cpPool_[side] = cp < 0 ? 0 : (cp > CP_CAP ? CP_CAP : cp);
    return true;
}

// --- CP interventions ------------------------------------------------

namespace {

Result<bool> CheckCommand(const BattleBeat beat, const int side,
                          const std::vector<Squad>& squads,
                          const std::vector<Intervention>& pending,
                          const int squadIndex, const int kindCost,
                          const int cpPool) {
    if (beat != BattleBeat::Execution) {
        return Fail<bool>("command", "interventions only during Execution");
    }
    if (side != 0 && side != 1) {
        return Fail<bool>("command", "side must be 0 or 1");
    }
    if (cpPool < kindCost) {
        return Fail<bool>("command", "insufficient CP");
    }
    if (squadIndex < 0 ||
        static_cast<std::size_t>(squadIndex) >= squads.size()) {
        return Fail<bool>("command", "invalid squad index");
    }
    const Squad& sq = squads[static_cast<std::size_t>(squadIndex)];
    if (sq.side != side) {
        return Fail<bool>("command", "cannot command the other side");
    }
    if (!sq.IsEffective()) {
        return Fail<bool>("command", "squad is off-field");
    }
    // Routing is "effective" (still on-field) but accepts no commands —
    // otherwise the CP is spent on a guaranteed apply-time no-op.
    if (sq.state == SquadState::Routing) {
        return Fail<bool>("command", "squad is routing off-field");
    }
    // One pending command per squad — a second would be a paid no-op
    // at apply (e.g. IssueMove is Holding-gated, forceNext is a bool).
    for (const Intervention& c : pending) {
        if (c.squadIndex == squadIndex) {
            return Fail<bool>("command",
                              "squad already has a pending command");
        }
    }
    return Ok(true);
}

} // namespace

Result<bool> BattleController::IssueRedirect(int side, int squadIndex,
                                             std::size_t region) {
    auto chk = CheckCommand(beat_, side, squads_, pendingCommands_,
                            squadIndex, CostOf(InterventionKind::Redirect),
                            cpPool_[side == 1 ? 1 : 0]);
    if (!chk.ok()) return chk;
    const Squad& sq = squads_[static_cast<std::size_t>(squadIndex)];
    if (sq.state != SquadState::Holding) {
        return Fail<bool>("command", "squad is not Holding");
    }
    if (region >= map_.RegionCount()) {
        return Fail<bool>("command", "target region out of bounds");
    }
    if (sq.regionIndex >= map_.RegionCount()) {
        return Fail<bool>("command", "squad region out of bounds");
    }
    bool adjacent = false;
    for (std::size_t n : map_.Neighbors(sq.regionIndex)) {
        if (n == region) { adjacent = true; break; }
    }
    if (!adjacent) {
        return Fail<bool>("command", "target not adjacent to squad");
    }
    cpPool_[side] -= CostOf(InterventionKind::Redirect);
    const int tick = static_cast<int>(sim_.TickCount());
    pendingCommands_.push_back({side, InterventionKind::Redirect,
                                squadIndex, static_cast<int>(region), tick});
    events_.push_back({SimEvent::Kind::Intervention, tick, squadIndex, -1,
                       static_cast<int>(region),
                       static_cast<int>(InterventionKind::Redirect), {}});
    return Ok(true);
}

Result<bool> BattleController::IssueOverride(int side, int squadIndex,
                                             int slotIndex) {
    auto chk = CheckCommand(beat_, side, squads_, pendingCommands_,
                            squadIndex, CostOf(InterventionKind::Override),
                            cpPool_[side == 1 ? 1 : 0]);
    if (!chk.ok()) return chk;
    if (slotIndex < 0 || static_cast<std::size_t>(slotIndex) >=
            sheets_[static_cast<std::size_t>(squadIndex)].slots.size()) {
        return Fail<bool>("command", "invalid slot index");
    }
    cpPool_[side] -= CostOf(InterventionKind::Override);
    const int tick = static_cast<int>(sim_.TickCount());
    pendingCommands_.push_back({side, InterventionKind::Override,
                                squadIndex, slotIndex, tick});
    events_.push_back({SimEvent::Kind::Intervention, tick, squadIndex, -1,
                       slotIndex,
                       static_cast<int>(InterventionKind::Override), {}});
    return Ok(true);
}

Result<bool> BattleController::IssueRetreat(int side, int squadIndex) {
    auto chk = CheckCommand(beat_, side, squads_, pendingCommands_,
                            squadIndex, CostOf(InterventionKind::Retreat),
                            cpPool_[side == 1 ? 1 : 0]);
    if (!chk.ok()) return chk;
    cpPool_[side] -= CostOf(InterventionKind::Retreat);
    const int tick = static_cast<int>(sim_.TickCount());
    pendingCommands_.push_back({side, InterventionKind::Retreat,
                                squadIndex, -1, tick});
    events_.push_back({SimEvent::Kind::Intervention, tick, squadIndex, -1,
                       -1,
                       static_cast<int>(InterventionKind::Retreat), {}});
    return Ok(true);
}

void BattleController::ApplyInterventions(int tick) {
    (void)tick; // applied at tick start; issueTick already recorded
    for (const Intervention& cmd : pendingCommands_) {
        if (cmd.squadIndex < 0 ||
            static_cast<std::size_t>(cmd.squadIndex) >= squads_.size())
            continue;
        Squad& sq = squads_[static_cast<std::size_t>(cmd.squadIndex)];
        // Routing accepts no commands either — a squad that broke
        // between issue and apply skips silently (CP already spent).
        if (!sq.IsEffective() || sq.state == SquadState::Routing) continue;
        switch (cmd.kind) {
            case InterventionKind::Redirect: {
                // Re-validate adjacency at apply — issue-time approval
                // stays valid only while the squad hasn't moved (the
                // pending-command guard allows one per squad, but keep
                // the gate for future command kinds that relocate).
                bool adjacent = false;
                const std::size_t target =
                    static_cast<std::size_t>(cmd.target);
                if (sq.regionIndex < map_.RegionCount() &&
                    target < map_.RegionCount()) {
                    for (std::size_t n : map_.Neighbors(sq.regionIndex)) {
                        if (n == target) { adjacent = true; break; }
                    }
                }
                if (adjacent) sq.IssueMove(target);
                break;
            }
            case InterventionKind::Override:
                if (cmd.target >= 0 && static_cast<std::size_t>(cmd.target) <
                        sheets_[static_cast<std::size_t>(cmd.squadIndex)]
                            .slots.size()) {
                    sheets_[static_cast<std::size_t>(cmd.squadIndex)]
                        .slots[static_cast<std::size_t>(cmd.target)]
                        .forceNext = true;
                }
                break;
            case InterventionKind::Retreat:
                // -> Routing; ApplyEvent clears in-flight edge state.
                sq.ApplyEvent(SquadEvent::CohesionBreak);
                break;
        }
    }
    pendingCommands_.clear();
}

bool BattleController::Tick() {
    if (beat_ != BattleBeat::Execution) return false;

    const int tick = static_cast<int>(sim_.TickCount());
    // 0) queued interventions apply at tick start (part of snapshot state)
    ApplyInterventions(tick);
    // CP regen: +1 per 60 s to each side, capped
    if (++cpRegen_ >= CP_REGEN_TICKS) {
        cpRegen_ = 0;
        for (int side = 0; side < 2; ++side) {
            if (cpPool_[side] < CP_CAP) ++cpPool_[side];
        }
    }
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
    pendingCommands_.clear(); // after Aftermath nothing ever applies
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
    h = Fold(h, static_cast<std::uint64_t>(cpPool_[0]));
    h = Fold(h, static_cast<std::uint64_t>(cpPool_[1]));
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
            h = Fold(h, static_cast<std::uint64_t>(slot.forceNext));
        }
    }
    // Committed-but-unapplied commands are sim state too — two
    // controllers with different queued orders must fingerprint apart.
    h = Fold(h, static_cast<std::uint64_t>(pendingCommands_.size()));
    for (const Intervention& c : pendingCommands_) {
        h = Fold(h, static_cast<std::uint64_t>(
                        static_cast<std::uint32_t>(c.side)));
        h = Fold(h, static_cast<std::uint64_t>(c.kind));
        h = Fold(h, static_cast<std::uint64_t>(
                        static_cast<std::uint32_t>(c.squadIndex)));
        h = Fold(h, static_cast<std::uint64_t>(
                        static_cast<std::uint32_t>(c.target)));
        h = Fold(h, static_cast<std::uint64_t>(
                        static_cast<std::uint32_t>(c.issueTick)));
    }
    return h;
}

} // namespace Potato::Gameplay
