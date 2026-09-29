#pragma once

#include "Gameplay/Json/JsonValue.h"
#include "Gameplay/Result.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace Potato::Gameplay {

// Named-squad model — the actors of a battle. Runtime instances live on a
// region/node graph (store region INDICES, not ids — the controller owns
// the BattleMap and validates adjacency). Integer-only, no exceptions.

enum class UnitType : std::uint8_t {
    Infantry,
    Cavalry,
    Artillery,
    Engineers,
    Scouts,
    Militia,
};

enum class Speed : std::uint8_t {
    Slow,
    Medium,
    Fast,
    VeryFast,
};

enum class SquadState : std::uint8_t {
    Holding,
    Moving,
    Routing,
    Routed,     // off the field — preserved for refit
    Destroyed,
};

enum class SquadEvent : std::uint8_t {
    MoveOrder,
    Arrived,
    CohesionBreak,   // cohesion fell below ROUT_THRESHOLD
    HpZero,
    RetreatComplete, // controller-driven; future stories issue it
};

constexpr int ROUT_THRESHOLD = 20;      // cohesion < 20% -> rout (GDD)
constexpr int TICKS_PER_EDGE_BASE = 40; // 2 s at 20 Hz, medium speed

// Milli-multiplier per Speed (medium = 1.0).
constexpr int SpeedMilli(Speed s) {
    switch (s) {
        case Speed::Slow:     return 600;
        case Speed::Medium:   return 1000;
        case Speed::Fast:     return 1500;
        case Speed::VeryFast: return 2000;
    }
    return 1000;
}

// Ticks to traverse one edge at a given speed. Integer division —
// deterministic; slow 66 / medium 40 / fast 26 / very_fast 20.
constexpr int TicksForEdge(int speedMilli) {
    return TICKS_PER_EDGE_BASE * 1000 / speedMilli;
}

// Explicit transition table — the table IS the spec (architecture pattern).
// Terminal states accept no events.
constexpr bool CanTransition(SquadState from, SquadEvent ev) {
    switch (ev) {
        case SquadEvent::MoveOrder:       return from == SquadState::Holding;
        case SquadEvent::Arrived:         return from == SquadState::Moving;
        case SquadEvent::CohesionBreak:   return from == SquadState::Holding ||
                                                    from == SquadState::Moving;
        case SquadEvent::RetreatComplete: return from == SquadState::Routing;
        case SquadEvent::HpZero:
            return from == SquadState::Holding || from == SquadState::Moving ||
                   from == SquadState::Routing;
    }
    return false;
}

constexpr SquadState TargetOf(SquadEvent ev) {
    switch (ev) {
        case SquadEvent::MoveOrder:       return SquadState::Moving;
        case SquadEvent::Arrived:         return SquadState::Holding;
        case SquadEvent::CohesionBreak:   return SquadState::Routing;
        case SquadEvent::HpZero:          return SquadState::Destroyed;
        case SquadEvent::RetreatComplete: return SquadState::Routed;
    }
    return SquadState::Holding;
}

struct Squad {
    std::string id;        // battle-scoped named identity
    std::string name;      // display, may carry UTF-8 CJK
    UnitType unit = UnitType::Infantry;
    int maxHp = 1;
    int hp = 1;
    int attack = 0;
    int speedMilli = 1000; // see SpeedMilli
    int cohesion = 100;    // 0–100 percent
    int cost = 0;          // 物資 (template field; refit uses it later)

    std::size_t regionIndex = ~std::size_t{0}; // BattleMap::NO_REGION-shaped
    std::size_t edgeTarget = ~std::size_t{0};
    int edgeProgress = 0; // ticks elapsed on current edge

    SquadState state = SquadState::Holding;

    bool IsEffective() const {
        return state != SquadState::Routed && state != SquadState::Destroyed;
    }

    // Issue a move order toward an adjacent region. Only valid from
    // Holding (table-gated); adjacency validation is the caller's job.
    bool IssueMove(std::size_t target);
    // Advance edge traversal one tick; arrives when progress reaches
    // TicksForEdge(speedMilli). No-op unless Moving.
    void TickMove();
    // Combat interface: apply losses, then evaluate the state table.
    // HpZero takes precedence over CohesionBreak. A rout cancels any
    // in-flight move.
    void ApplyHit(int hpLoss, int cohesionLoss);
    void RestoreCohesion(int amount);
    void Heal(int amount);

    // Instantiate a runtime squad from a loaded template.
    static Squad Instantiate(const struct SquadTemplate& t,
                             std::size_t regionIndex);
};

struct SquadTemplate {
    std::string id;
    std::string name;
    UnitType unit = UnitType::Infantry;
    int hp = 1;
    int attack = 0;
    Speed speed = Speed::Medium;
    int cohesion = 100;
    int cost = 0;
};

// potato.squad/1 — a template library file.
class SquadTemplateLibrary {
public:
    static Result<SquadTemplateLibrary> Load(std::string_view path);
    static Result<SquadTemplateLibrary> FromJson(const JsonValue& root);

    std::size_t Count() const { return templates_.size(); }
    const SquadTemplate& At(std::size_t index) const { return templates_[index]; }
    const SquadTemplate* Find(std::string_view id) const;

private:
    std::vector<SquadTemplate> templates_;
};

} // namespace Potato::Gameplay
