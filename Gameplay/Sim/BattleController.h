#pragma once

#include "Gameplay/Command/Intervention.h"
#include "Gameplay/Doctrine/Doctrine.h"
#include "Gameplay/Result.h"
#include "Gameplay/Sim/Sim.h"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace Potato::Gameplay {

class BattleMap;
struct Squad;
struct SquadTemplate;

// Three-beat battle loop — the state table IS the spec (arch pattern:
// enum class + explicit transition table, unit-testable).
enum class BattleBeat : std::uint8_t { Planning, Execution, Aftermath };

constexpr bool CanTransition(BattleBeat a, BattleBeat b) {
    return (a == BattleBeat::Planning && b == BattleBeat::Execution) ||
           (a == BattleBeat::Execution && b == BattleBeat::Aftermath);
}

// Emitted once when Execution -> Aftermath closes the battle.
// `forced` distinguishes a manual RequestBeat(Aftermath) concede
// (both sides intact, winnerSide == -1) from a mutual wipe draw.
struct BattleOutcome {
    int winnerSide = -1;            // -1 = draw or forced end, see `forced`
    bool forced = false;            // true if closed by RequestBeat, not a wipe
    std::uint64_t elapsedTicks = 0; // ticks actually executed
    int effective[2] = {0, 0};      // includes Routing (still on-field)
    int routed[2] = {0, 0};
    int destroyed[2] = {0, 0};
};

// Ticks a Routing squad needs to leave the field (2 s at 20 Hz).
constexpr int ROUT_TICKS = 2 * TICK_RATE_HZ;

// Owns one battle: sim kernel (seed, tick counter, checksum, the PRNG
// stream), live squads, their doctrine sheets, and the append-only
// SimEvent log (record-is-truth — the recorder consumes exactly this).
// Beats: Planning accepts deploy/sheet input and never ticks;
// Execution ticks deterministically; Aftermath is terminal.
// `map` and `cards` must outlive the controller (borrowed refs).
class BattleController {
public:
    BattleController(std::uint64_t seed, const BattleMap& map,
                     const DoctrineLibrary& cards);
    ~BattleController();
    // Copy-constructible (tests snapshot whole controllers); copies share
    // the borrowed map_/cards_ refs. Assignment deleted — refs can't reseat.
    BattleController(const BattleController&);
    BattleController& operator=(const BattleController&) = delete;

    BattleBeat Beat() const { return beat_; }

    // Table-gated transition; illegal moves (e.g. Planning->Aftermath)
    // rejected with no side effects. Emits BeatChanged into the log.
    bool RequestBeat(BattleBeat target);

    // --- Planning input (rejected outside Planning) ---
    bool DeploySquad(const SquadTemplate& t, std::size_t regionIndex,
                     int side);
    bool SetSheet(std::size_t squadIndex, SquadSheet sheet);
    // Test/harness hook — the live economy (regen, cap) runs itself.
    bool SetCpPool(int side, int cp);

    // --- CP interventions (Execution only) ---
    // Deduct-at-issue; the command applies at the start of the next tick
    // (before doctrine eval). Rejections carry a reason and never deduct.
    Result<bool> IssueRedirect(int side, int squadIndex,
                               std::size_t region);   // 1 CP
    Result<bool> IssueOverride(int side, int squadIndex,
                               int slotIndex);        // 2 CP
    Result<bool> IssueRetreat(int side, int squadIndex); // 3 CP

    // --- Execution tick ---
    // One deterministic tick: doctrine eval (snapshot semantics) ->
    // apply pending deltas -> squads advance edge traversal -> append
    // events -> sim housekeeping draw -> side-wipe check auto-closes.
    // No-op returning false outside Execution.
    bool Tick();

    // --- Read-only surface ---
    const Sim& GetSim() const { return sim_; }
    const std::vector<Squad>& Squads() const { return squads_; }
    const std::vector<SimEvent>& Events() const { return events_; }
    const BattleOutcome& Outcome() const { return outcome_; }
    int CpPool(int side) const {
        assert(side == 0 || side == 1);
        return cpPool_[side == 1 ? 1 : 0];
    }

    // End-to-end determinism fingerprint: sim stream checksum folded
    // with per-squad numeric state, beat, CP pools, and the pending
    // command queue (committed-but-unapplied state). Strings are
    // excluded (Squad is non-POD — hash field values, never object
    // memory).
    std::uint64_t Checksum() const;

private:
    int CountEffective(int side) const;
    void EmitBeatChanged(int tick, BattleBeat target);
    void CloseBattle(bool forced); // compute outcome + enter Aftermath
    void ApplyInterventions(int tick);

    const BattleMap& map_;
    const DoctrineLibrary& cards_;
    Sim sim_;
    BattleBeat beat_ = BattleBeat::Planning;
    std::vector<Squad> squads_;
    std::vector<SquadSheet> sheets_;     // index-aligned with squads_
    std::vector<int> routTimers_;        // index-aligned; Routing squads age out
    std::vector<SimEvent> events_;       // append-only battle log
    std::vector<Intervention> pendingCommands_; // applied at next tick start
    BattleOutcome outcome_;
    std::array<int, 2> cpPool_ = {CP_START, CP_START};
    int cpRegen_ = 0;
};

} // namespace Potato::Gameplay
