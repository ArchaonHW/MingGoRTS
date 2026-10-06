#pragma once

#include "Campaign/World/WorldMap.h"
#include "Gameplay/Result.h"
#include "Gameplay/Sim/Sim.h" // Prng — the dedicated world stream

#include <cstddef>
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace Potato::Gameplay {
class JsonValue;
}

namespace Potato::Campaign {

// World runtime state (Story 12.2, D-ARCH-10): the living side of
// the open campaign map — day counter, warband anchor, per-node
// control, resolved-POI set, and a pending event queue resolved at
// day-scale beats.
//
// NOT a second tick sim: resolution is event-granular, ordered,
// integer-only, and draws from a dedicated SplitMix64 stream that
// is NOT the battle's PRNG. Zero file I/O, no exceptions.
//
// Persisted as sibling doc `potato.worldstate/1` (MythState
// potato.myth/1 precedent) — embedding into the campaign save
// envelope is Story 12.8's seam.
//
// Wire shape:
//   {"schema":"potato.worldstate/1","world":"republic_fall",
//    "day":4,"warband":"kaifeng",
//    "control":{"kaifeng":"player","luoyang":"rival"},
//    "resolved":["longmen_shrine"],"rng":12345,
//    "queue":[{"day":6,"seq":0,"kind":"control",
//              "node":"luoyang","control":"neutral"}]}
//
// Canonical form: only non-Neutral control is stored (silence is
// ground state); `resolved` and `queue` are sorted; `rng` is the
// raw SplitMix64 counter so a reloaded campaign draws the same
// sequence.

enum class WorldEventKind : std::uint8_t {
    SetControl, // node changes faction control
    Resolve,    // mark a POI/encounter id resolved
};

struct WorldEvent {
    std::int64_t day = 0;         // beat at which it applies
    std::uint64_t seq = 0;        // emission tie-break
    WorldEventKind kind = WorldEventKind::SetControl;
    std::string node;             // world node id (the target)
    WorldControl control = WorldControl::Neutral; // SetControl arg
};

class WorldState {
public:
    static constexpr std::string_view SCHEMA = "potato.worldstate/1";
    static constexpr std::size_t MAX_EVENTS = 1024;   // queued
    static constexpr std::size_t MAX_RESOLVED = 4096;
    static constexpr std::size_t MAX_ID_LEN = 64;
    static constexpr std::int64_t MAX_DAY = 36500;    // ~100 yrs

    // Fresh campaign state bound to a world doc: day 0, warband at
    // map.StartIndex(), control seeded from node `control` fields,
    // world PRNG at `seed`.
    static Gameplay::Result<WorldState> Init(const WorldMap& map,
                                             std::uint64_t seed);

    std::int64_t Day() const { return day_; }
    const std::string& WorldId() const { return worldId_; }
    // Warband anchor node id; empty only in a default state.
    const std::string& WarbandAt() const { return warband_; }
    // Neutral on miss (silence = unremarkable ground state).
    WorldControl ControlAt(std::string_view node) const;
    bool IsResolved(std::string_view node) const {
        return resolved_.count(std::string(node)) != 0;
    }
    // Canonical order: (day, seq, insertion-stable).
    const std::vector<WorldEvent>& Pending() const { return queue_; }

    // Queue an event: node must exist in `map`, day >= Day(), queue
    // under MAX_EVENTS. Mutation only on success.
    Gameplay::Result<bool> Enqueue(const WorldMap& map,
                                   WorldEvent ev);

    // Advance `days` beats (>= 0), draining every queued event with
    // day <= new day in canonical (day, seq, insertion) order.
    // Integer math; no I/O; no exceptions. An event whose node has
    // since vanished from the map is skipped but counts as applied
    // (content may shrink between versions — the beat marches on).
    Gameplay::Result<int> ResolveBeats(const WorldMap& map,
                                       std::int64_t days);

    // The world's dedicated PRNG stream. Draw order IS the
    // canonical order — draw only from inside beat resolution and
    // other canonical paths, never from presentation.
    std::uint64_t Draw() { return rng_.Next(); }

    // Relocate the warband (12.3's movement mechanics call this
    // after charging march cost). false if `node` isn't in `map`.
    bool SetWarband(std::string_view node, const WorldMap& map);

    Gameplay::Result<Gameplay::JsonValue> ToJson() const;
    static Gameplay::Result<WorldState> FromJson(
        const Gameplay::JsonValue& doc);

private:
    std::string worldId_;
    std::int64_t day_ = 0;
    std::string warband_;
    std::map<std::string, WorldControl> control_; // non-Neutral only
    std::set<std::string> resolved_;
    std::vector<WorldEvent> queue_;
    Gameplay::Prng rng_{0};
};

} // namespace Potato::Campaign
