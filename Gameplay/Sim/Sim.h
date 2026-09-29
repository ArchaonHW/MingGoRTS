#pragma once

#include <cstdint>

namespace Potato::Gameplay {

// Fixed-tick simulation rate. All sim time derives from integer ticks.
constexpr std::int32_t TICK_RATE_HZ = 20;

// Minimal deterministic simulation kernel shell.
// Owns the single seeded PRNG stream — no global rand(), no floats.
// BattleState arrays and doctrine evaluation arrive in later stories;
// this skeleton proves the compile/link/tick contract and the
// determinism convention (same seed -> same checksum).
class Sim {
public:
    explicit Sim(std::uint64_t seed);

    // Advances the simulation exactly one tick.
    void Tick();

    std::uint64_t TickCount() const { return tickCount_; }

    // Deterministic fingerprint of sim state. Identical seeds produce
    // identical checksums across compilers and platforms.
    std::uint64_t Checksum() const { return checksum_; }

private:
    // SplitMix64 — one stream, draws in canonical order only.
    // Counter-based: every seed maps to a distinct stream, seed 0 included
    // (no degenerate state, no remap branch, no seed aliasing).
    std::uint64_t NextRandom();

    std::uint64_t tickCount_ = 0;
    std::uint64_t checksum_ = 0;
    std::uint64_t rngState_ = 0;
};

} // namespace Potato::Gameplay
