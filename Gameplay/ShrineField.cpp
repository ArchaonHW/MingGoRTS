#include "ShrineField.h"

#include "BattleController.h"
#include "Squad.h"

namespace Potato {
namespace Gameplay {

const char* ShrineChoiceName(ShrineChoice c) {
    switch (c) {
    case ShrineChoice::Appease: return "安撫";
    case ShrineChoice::Provoke: return "挑釁";
    case ShrineChoice::Offer:   return "獻祭";
    }
    return "未知";
}

void ShrineField::Bind(
    const std::vector<MapInteractable>& interactables) {
    shrines.clear();
    for (size_t i = 0; i < interactables.size(); ++i) {
        const auto& it = interactables[i];
        if (it.type != "shrine") {
            continue;
        }
        ShrineState s;
        s.mapIndex = i;
        s.pos = it.pos;
        s.radius = it.radius;
        s.spirit = it.spirit;
        s.offering = it.offering;
        shrines.push_back(s);
    }
}

ShrineField::ShrineState* ShrineField::Find(size_t mapIndex) {
    for (auto& s : shrines) {
        if (s.mapIndex == mapIndex) {
            return &s;
        }
    }
    return nullptr;
}

const ShrineField::ShrineState* ShrineField::Find(
    size_t mapIndex) const {
    for (const auto& s : shrines) {
        if (s.mapIndex == mapIndex) {
            return &s;
        }
    }
    return nullptr;
}

const ShrineField::ShrineState* ShrineField::GetShrine(
    size_t mapIndex) const {
    return Find(mapIndex);
}

void ShrineField::Update(const BattleController& battle) {
    // 發現事件先收集、掃描完再派出——回呼若在迭代中再入
    // （如呼叫 Bind）不會懸空 shrines 迭代器
    std::vector<std::string> pending;
    for (auto& s : shrines) {
        if (s.visual != ShrineVisual::Idle) {
            continue; // 已發現/已結算不重發
        }
        for (const auto& sq : battle.GetSquads()) {
            if (sq->GetTeam() != 0 || sq->IsEliminated() ||
                sq->IsRouting()) {
                continue;
            }
            const Vector2 d = sq->GetPosition() - s.pos;
            if (d.LengthSquared() <= s.radius * s.radius) {
                s.visual = ShrineVisual::Occupied;
                pending.push_back("發現神社[#" +
                    std::to_string(s.mapIndex) + "]:" +
                    (s.spirit.empty() ? "境靈" : s.spirit));
                break;
            }
        }
    }
    if (onEvent) {
        for (const auto& m : pending) {
            onEvent(m);
        }
    }
}

bool ShrineField::IsPending(size_t mapIndex) const {
    const ShrineState* s = Find(mapIndex);
    return s && s->visual == ShrineVisual::Occupied;
}

bool ShrineField::ApplyChoice(size_t mapIndex, ShrineChoice c) {
    ShrineState* s = Find(mapIndex);
    if (!s || s->visual != ShrineVisual::Occupied) {
        return false; // 未進駐或已結算：拒絕，不發任何回呼
    }
    ShrineVisual next;
    float delta;
    switch (c) {
    case ShrineChoice::Appease:
        next = ShrineVisual::Appeased;
        delta = kAppeaseFavor;
        break;
    case ShrineChoice::Provoke:
        next = ShrineVisual::Provoked;
        delta = -kProvokeFavor;
        break;
    case ShrineChoice::Offer:
        next = ShrineVisual::Offered;
        delta = kOfferFavor;
        break;
    default:
        return false; // 非法列舉值：不結算、不發回呼
    }
    s->visual = next; // 先結算再發回呼——再入 ApplyChoice 會被拒
    const std::string spirit =
        s->spirit.empty() ? "境靈" : s->spirit;
    if (onEvent) {
        onEvent("神社[#" + std::to_string(s->mapIndex) + "]:" +
                spirit + " " + ShrineChoiceName(c));
    }
    if (onFavor) {
        onFavor(spirit, delta);
    }
    return true;
}

} // namespace Gameplay
} // namespace Potato
