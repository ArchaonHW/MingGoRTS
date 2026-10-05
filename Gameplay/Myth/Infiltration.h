#pragma once

#include "Gameplay/Result.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace Potato::Gameplay {

class BattleMap;
struct SimEvent;

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

// Myth-layer transition inputs. Additional kinds (shrine capture,
// invasion resolution) append — ordinals are wire-stable.
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

// Battle-scope per-region store (mirror of GovernanceField's
// contract: borrowed map ref, Init, deterministic, no PRNG). Levels
// are sim state — they fold into the controller checksum, so
// recorded battles reproduce infiltration bit-exact.
class MythField {
public:
    // Sizes the level table to the map's regions. Borrowed ref —
    // the map must outlive the field.
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

    InfiltrationLevel LevelAt(std::size_t region) const;
    std::size_t RegionCount() const { return levels_.size(); }
    const std::vector<std::uint8_t>& Levels() const { return levels_; }

private:
    std::vector<std::uint8_t> levels_; // region-indexed, 0..3
};

} // namespace Potato::Gameplay
