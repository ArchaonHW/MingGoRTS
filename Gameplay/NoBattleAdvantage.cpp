#include "Gameplay/NoBattleAdvantage.h"

#include "Gameplay/BattleController.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/Squad.h"

#include <algorithm>

namespace Potato {
namespace Gameplay {

void ApplyFirstWaveAdvantage(BattleController& battle,
                             BattleResources& res,
                             int enemyTeam, int playerTeam) {
    // 先機 = 介入籌碼與情報都向敵傾斜
    res.AddCP(battle, enemyTeam, 2);
    res.AddIntel(enemyTeam, 2);
    // 敗談後軍心浮動——減益夾到 ≥0.10，不製造即潰
    // （AdjustMorale 內部夾 [0,1] 且歸零即 routing，故精算 delta）
    for (const auto& s : battle.GetSquads()) {
        if (s && s->GetTeam() == playerTeam) {
            const float target = std::max(0.10f, s->GetMorale() - 0.10f);
            s->AdjustMorale(target - s->GetMorale());
        }
    }
}

} // namespace Gameplay
} // namespace Potato
