#pragma once

#include "Core/CoreTypes.h"
#include "MathUtils/Vector2.h"
#include "Doctrine.h"

#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

class BattleController;
class Squad;

/**
 * 智能規劃器（BattlePlanner）——回合層的 AI 參謀
 *
 * 對應設計「回合層負責思考」：玩家部署完小隊後，
 * BattlePlanner 分析敵我兵力比、地形可達性、小隊強度排序，
 * 自動產出全隊 doctrine 腳本與 CP 建議。
 *
 * 每個 SquadPlan 附 rationale（為何這樣規劃），
 * 讓玩家看懂 AI 的判斷——可解釋性是回合層的核心體驗。
 */
class BattlePlanner {
public:
    // 戰略姿態：由敵我總兵力比決定
    enum class Posture {
        Aggressive, // 兵力優勢 >= 1.3x：主攻壓上
        Balanced,   // 0.7x ~ 1.3x：攻防並重
        Defensive   // 兵力劣勢 <= 0.7x：保守、早退
    };

    // 小隊角色：由隊內強度排序決定
    enum class SquadRole {
        Assault,  // 最強梯次：主攻
        Support,  // 中間梯次：支援接戰友軍
        Reserve   // 最弱梯次：預備隊，原地待命
    };

    struct SquadPlan {
        const Squad* squad = nullptr;
        SquadRole role = SquadRole::Assault;
        DoctrineSet doctrine;
        std::string rationale; // 給玩家看的判斷理由
    };

    struct Plan {
        int team = 0;
        Posture posture = Posture::Balanced;
        int suggestedCP = 0;
        std::vector<SquadPlan> squadPlans;
        std::string summary;   // 全隊規劃摘要
    };

    // 分析戰場並生成規劃（在 BeginExecution 之前呼叫）
    Plan GeneratePlan(const BattleController& battle, int team) const;

    // 把規劃寫回 controller（AssignDoctrine + SetCommandPoints）
    void ApplyPlan(BattleController& battle, const Plan& plan) const;

    static const char* PostureName(Posture p);
    static const char* RoleName(SquadRole r);

private:
    SquadRole ClassifyRole(size_t rank, size_t squadCount) const;
    DoctrineSet BuildDoctrine(SquadRole role, Posture posture) const;
};

} // namespace Gameplay
} // namespace Potato
