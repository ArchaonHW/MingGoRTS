#pragma once

#include "Gameplay/Result.h"
#include "Gameplay/Sim/Sim.h"

#include <cstddef>
#include <cstdint>

namespace Potato::Gameplay {

// CP economy (GDD FR3): start 3, +1 per 60 s, cap 5.
constexpr int CP_START = 3;
constexpr int CP_CAP = 5;
constexpr int CP_REGEN_TICKS = 60 * TICK_RATE_HZ;

// Mid-Execution player/AI commands. Costs: redirect 1, override 2,
// retreat 3. Queued at issue, applied at next tick start (well inside
// the ~3 s resolution budget).
enum class InterventionKind : std::uint8_t { Redirect = 0, Override, Retreat };

constexpr int CostOf(InterventionKind k) {
    switch (k) { // no default: new kinds must pick a cost
        case InterventionKind::Redirect: return 1;
        case InterventionKind::Override: return 2;
        case InterventionKind::Retreat:  return 3;
    }
    return 0; // unreachable — all enumerators handled
}

// target: region index (Redirect) or slot index (Override); unused for
// Retreat. issueTick is recorded for replay/ordering metadata — the
// apply step doesn't re-read it (queue order is canonical).
struct Intervention {
    int side = 0;
    InterventionKind kind = InterventionKind::Redirect;
    int squadIndex = -1;
    int target = -1;
    int issueTick = 0;
};

} // namespace Potato::Gameplay
