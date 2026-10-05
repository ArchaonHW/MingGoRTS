#pragma once

#include "Gameplay/Result.h"
#include "Gameplay/Sim/Sim.h" // TICK_RATE_HZ

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace Potato::Gameplay {

class BattleMap;
struct SimEvent;
struct Squad;

// Myth-layer infiltration (Epic 5, Story 5.1): a per-region state
// machine 0-3 on the SAME map the battle uses — the historical layer
// is what clerks record; the myth layer is what people believe.
//
// The transition table IS the spec (architecture §state transitions:
// enum class + explicit unit-testable table). Drivers — myth actions
// (Story 5.4) and invasion events (Story 5.5) — always pass through
// it; there is no setter that bypasses the table during play.
//
// InfiltrationLevel — how far the spirit world has pushed in:
//   0 None      — 平靖: the land is quiet.
//   1 Whispered — 耳語: people leave offerings; rumors start.
//   2 Haunted   — 作祟: the land stirs; shrines answer back.
//   3 Invaded   — 傾巢: the myth layer overruns the region — Story
//                 5.5's invasion events trigger off this state.
enum class InfiltrationLevel : std::uint8_t {
    None = 0,
    Whispered = 1,
    Haunted = 2,
    Invaded = 3,
};

// Myth-layer transition inputs (planning-phase verbs; shrine
// capture is NOT a MythEventKind — it rides the per-tick Tick scan,
// and Story 5.4's myth actions get their own journal mechanism).
// Ordinals are wire-stable.
enum class MythEventKind : std::uint8_t {
    Incursion = 0,    // the myth layer pushes in (myth actions, omens)
    Pacification = 1, // ritual appeasement — backs off one step
};

// Enum-count pins: serialisation and the table below are asserted
// against these — adding a member without updating Myth code breaks
// the build, not a save.
constexpr std::size_t kInfiltrationLevelCount = 4;
constexpr std::size_t kMythEventKindCount = 2;

// THE TABLE (spec):
//                  Incursion   Pacification
//   0 None            1            0
//   1 Whispered       2            0
//   2 Haunted         3            1
//   3 Invaded         3            2
// Saturating at both ends — an Incursion at 3 or Pacification at 0
// is a legal no-op apply, not an error.
InfiltrationLevel Transition(InfiltrationLevel level,
                             MythEventKind event);

// Human-readable names (chronicle/debug — never parse these back).
const char* InfiltrationLevelName(InfiltrationLevel level);
const char* MythEventKindName(MythEventKind kind);

// Wire gates — the only way persistence/wire ints become enums.
bool InfiltrationLevelFromInt(std::int64_t v, InfiltrationLevel& out);
bool MythEventKindFromInt(std::int64_t v, MythEventKind& out);

// --- Shrines (Story 5.2) ---
// A shrine is a capturable myth asset on a MYTH_SHRINE region — its
// deity holds jurisdiction there (narrative: local earth gods own
// specific ground). Occupation flips allegiance the same way villages
// latch: exclusive presence held for SHRINE_DEDICATION_TICKS.
//
// GodStance is the deity's disposition toward each side — the
// minimal model Story 5.2 needs (capture pleases the capturer's
// claim and offends the other's). Story 5.7 grows this into
// per-region conduct-modulated disposition driving myth-action cost.
enum class GodStance : std::int8_t {
    Wrathful = -1, // 神怒
    Neutral = 0,   // 不聞
    Favorable = 1, // 神悅
};

const char* GodStanceName(GodStance stance);

// Exclusive-presence ticks before a shrine's allegiance latches —
// same tempo as village occupation (4 s at 20 Hz): dedicating sacred
// ground takes as long as holding a village.
constexpr int SHRINE_DEDICATION_TICKS = 4 * TICK_RATE_HZ;

struct ShrineTrack {
    std::size_t region = 0;
    int claimant = -1;  // this tick's exclusive-presence side (0/1)
    int dwell = 0;      // consecutive ticks for claimant (saturating)
    int owner = -1;     // latched allegiance (-1 = unconsecrated)
    // Deity disposition toward each side — capture sets +1/-1 and
    // the god REMEMBERS: stance persists even after the latch is
    // released (unconsecrated ground still reads its last offense —
    // deliberate; Story 5.7 modulates costs from this).
    std::array<std::int8_t, 2> stance = {0, 0};
};

// Battle-scope per-region store (mirror of GovernanceField's
// contract: Init'd from the map, deterministic, no PRNG). Levels
// AND shrine tracks are sim state — they fold into the controller
// checksum, so recorded battles reproduce the myth layer bit-exact.
class MythField {
public:
    // Sizes the level table to the map's regions and indexes its
    // MYTH_SHRINE regions into shrine tracks. The map is consumed
    // at Init only — no borrowed pointer is kept (levels_ bounds
    // serve all checks).
    void Init(const BattleMap& map);

    // Seed a persisted level carried in from campaign state
    // (Planning-phase input, journaled as the "mythseed" op). The
    // seed IS a level set — it bypasses the table deliberately:
    // carry-in is an initial condition, not a transition. Emits no
    // event (nothing changed mid-battle; the chronicle already
    // knows this state). Rejects OOB regions, out-of-domain levels,
    // and writes that would change nothing — journaled ops must have
    // an observable effect (a forged no-op op must never replay).
    bool Seed(std::size_t region, InfiltrationLevel level);

    // Apply a myth-layer event through the transition table.
    // Returns an InfiltrationChanged SimEvent only when the level
    // actually changes (the event name is literal — a saturating
    // no-op or invalid input produces no record).
    std::optional<SimEvent> Apply(std::size_t region,
                                  MythEventKind kind, int tick);

    // End-of-tick myth-layer scan (Story 5.2): shrine
    // dwell/latch/capture on post-move truth — the same
    // exclusive-presence discipline villages use, including
    // departure-region semantics (a squad mid-march still claims
    // its departure node until the edge completes). Burning a
    // village on the same region does NOT release the shrine —
    // ash holds no village, but the god's jurisdiction is ground,
    // not buildings. Events come back in map order.
    std::vector<SimEvent> Tick(const std::vector<Squad>& squads,
                               int tick);

    InfiltrationLevel LevelAt(std::size_t region) const;
    std::size_t RegionCount() const { return levels_.size(); }
    const std::vector<std::uint8_t>& Levels() const { return levels_; }

    // Shrine surface — map order; `ShrineAt` is nullptr for
    // non-shrine regions. `StanceAt` is Neutral for missing entries.
    // This is the co-render contract: shrine state keyed by the
    // same region geometry terrain draws on.
    const std::vector<ShrineTrack>& Shrines() const {
        return shrines_;
    }
    const ShrineTrack* ShrineAt(std::size_t region) const;
    GodStance StanceAt(std::size_t region, int side) const;

private:
    std::vector<std::uint8_t> levels_;   // region-indexed, 0..3
    std::vector<ShrineTrack> shrines_;   // MYTH_SHRINE regions, map order
};

} // namespace Potato::Gameplay
