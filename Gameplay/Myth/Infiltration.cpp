#include "Gameplay/Myth/Infiltration.h"

#include "Gameplay/Doctrine/Doctrine.h" // SimEvent
#include "Gameplay/Map/BattleMap.h"

namespace Potato::Gameplay {

static_assert(static_cast<int>(InfiltrationLevel::Invaded) + 1 ==
                  static_cast<int>(kInfiltrationLevelCount),
              "InfiltrationLevel grew — update the transition table");
static_assert(static_cast<int>(MythEventKind::Pacification) + 1 ==
                  static_cast<int>(kMythEventKindCount),
              "MythEventKind grew — update the transition table");

InfiltrationLevel Transition(InfiltrationLevel level,
                             MythEventKind event) {
    // [level][event] — see the header for the spec table.
    static constexpr std::uint8_t TABLE[kInfiltrationLevelCount]
                                       [kMythEventKindCount] = {
        {1, 0}, // None
        {2, 0}, // Whispered
        {3, 1}, // Haunted
        {3, 2}, // Invaded
    };
    const auto l = static_cast<std::size_t>(level);
    const auto e = static_cast<std::size_t>(event);
    if (l >= kInfiltrationLevelCount || e >= kMythEventKindCount) {
        return level; // unreachable through the wire gates
    }
    return static_cast<InfiltrationLevel>(TABLE[l][e]);
}

const char* InfiltrationLevelName(InfiltrationLevel level) {
    switch (level) {
    case InfiltrationLevel::None:      return "none";
    case InfiltrationLevel::Whispered: return "whispered";
    case InfiltrationLevel::Haunted:   return "haunted";
    case InfiltrationLevel::Invaded:   return "invaded";
    }
    return "unknown";
}

const char* MythEventKindName(MythEventKind kind) {
    switch (kind) {
    case MythEventKind::Incursion:    return "incursion";
    case MythEventKind::Pacification: return "pacification";
    }
    return "unknown";
}

bool InfiltrationLevelFromInt(std::int64_t v, InfiltrationLevel& out) {
    if (v < 0 ||
        v >= static_cast<std::int64_t>(kInfiltrationLevelCount)) {
        return false;
    }
    out = static_cast<InfiltrationLevel>(v);
    return true;
}

bool MythEventKindFromInt(std::int64_t v, MythEventKind& out) {
    if (v < 0 || v >= static_cast<std::int64_t>(kMythEventKindCount)) {
        return false;
    }
    out = static_cast<MythEventKind>(v);
    return true;
}

void MythField::Init(const BattleMap& map) {
    levels_.assign(map.RegionCount(), 0);
}

bool MythField::Seed(std::size_t region, InfiltrationLevel level) {
    if (region >= levels_.size() ||
        static_cast<std::size_t>(level) >= kInfiltrationLevelCount) {
        return false;
    }
    // Journal contract: an accepted call must change observable
    // state — a seed to the current level is rejected, never
    // recorded, so no-op ops can't ride a valid record.
    if (levels_[region] == static_cast<std::uint8_t>(level)) {
        return false;
    }
    levels_[region] = static_cast<std::uint8_t>(level);
    return true;
}

std::optional<SimEvent> MythField::Apply(std::size_t region,
                                         MythEventKind kind,
                                         int tick) {
    if (region >= levels_.size() ||
        static_cast<std::size_t>(kind) >= kMythEventKindCount) {
        return std::nullopt;
    }
    const InfiltrationLevel next =
        Transition(static_cast<InfiltrationLevel>(levels_[region]),
                   kind);
    if (next == static_cast<InfiltrationLevel>(levels_[region])) {
        return std::nullopt;
    }
    levels_[region] = static_cast<std::uint8_t>(next);
    // param = region, aux = new level (0..3 fits the 0..6 aux wire
    // gate), squadIndex/side = -1: the myth layer is side-less —
    // spirits answer to no banner.
    return SimEvent{SimEvent::Kind::InfiltrationChanged, tick, -1, -1,
                    static_cast<int>(region),
                    static_cast<int>(next), -1, {}, {}};
}

InfiltrationLevel MythField::LevelAt(std::size_t region) const {
    if (region >= levels_.size()) return InfiltrationLevel::None;
    return static_cast<InfiltrationLevel>(levels_[region]);
}

} // namespace Potato::Gameplay
