#include "Gameplay/SubversionDefect.h"

#include "Gameplay/BattleController.h"
#include "Gameplay/Doctrine.h"
#include "Gameplay/Squad.h"

#include <algorithm>
#include <vector>

namespace Potato {
namespace Gameplay {

int ApplyDefection(BattleController& battle,
                   int enemyTeam, int playerTeam, int maxCount) {
    if (maxCount <= 0) {
        return 0;
    }
    // 最動搖者先被策反：士氣升冪→兵員升冪→名稱升冪（全確定性）
    std::vector<Squad*> candidates;
    for (const auto& s : battle.GetSquads()) {
        if (s && s->GetTeam() == enemyTeam && !s->IsEliminated() &&
            !s->IsRouting()) {
            candidates.push_back(s.get());
        }
    }
    std::sort(candidates.begin(), candidates.end(),
              [](const Squad* a, const Squad* b) {
                  if (a->GetMorale() != b->GetMorale())
                      return a->GetMorale() < b->GetMorale();
                  if (a->GetMembers() != b->GetMembers())
                      return a->GetMembers() < b->GetMembers();
                  return a->GetName() < b->GetName();
              });

    // 倒戈隊兜底 doctrine：主動攻擊新敵方——不保留敵方腳本
    // （EnemyGeneral::ApplyTo 給的敵令對新陣營無意義），
    // 也不留無令待機（無 doctrine 的隊 Evaluate 不到不動）。
    DoctrineSet fallback;
    fallback.AddRule(DoctrineRule(DoctrineTrigger::Always,
                                  DoctrineAction::AttackNearest,
                                  0.0f, 100));

    int flipped = 0;
    for (Squad* s : candidates) {
        if (flipped >= maxCount) break;
        s->Defect(playerTeam);
        battle.AssignDoctrine(s, fallback); // Deployment 階段才生效
        ++flipped;
    }
    return flipped;
}

} // namespace Gameplay
} // namespace Potato
