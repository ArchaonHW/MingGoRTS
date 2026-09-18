#pragma once

#include "Core/CoreTypes.h"
#include "MathUtils/Vector2.h"
#include "Doctrine.h"

#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

class BattleController;
class BattleResources;

/**
 * 作戰計畫箭頭（G-5，HOI4 battle plan → doctrine）
 *
 * 回合層 UI 在地圖上畫的進攻軸線：from → to 一段主攻方向，
 * 綁定到具名小隊（squadName 空 = 未指派，Apply 時當通用箭頭
 * 兜底套到沒有專屬箭頭的小隊）。
 */
struct PlanArrow {
    std::string squadName;
    Vector2 from;
    Vector2 to;
    int priority = 30; // 主軸推進規則的 priority（越小越優先）
};

/**
 * 作戰計畫：一組箭頭 + 集結點 + 計畫加成
 *
 * 轉譯規則（ArrowToDoctrine）每支箭頭產出三條 doctrine：
 *   prio 10  HealthBelow(0.30) → RetreatToRally（保命優先，15s 冷卻）
 *   prio 20  ObjectiveReached  → AttackNearest（到位後就地接敵）
 *   prio N   Always            → AdvanceToObjective（主軸，N=arrow.priority）
 *
 * 計畫加成（HOI4 planning bonus）：
 *   attackMul —— Apply 時乘進指派小隊的 damagePerMember（執行層生效）
 *   bonusIntel / bonusCP —— 經 BattleResources 入帳（CP 同步 controller）
 */
class BattlePlan {
public:
    void AddArrow(const std::string& squadName, const Vector2& from,
                  const Vector2& to, int priority = 30);
    size_t ArrowCount() const { return arrows.size(); }
    const PlanArrow& Arrow(size_t i) const { return arrows[i]; }

    void SetRallyPoint(const Vector2& p);
    bool HasRallyPoint() const { return hasRally; }
    const Vector2& RallyPoint() const { return rallyPoint; }

    void SetPlanBonus(float attackMul, int bonusIntel = 0,
                      int bonusCP = 0);
    float AttackMultiplier() const { return attackMul; }
    int BonusIntel() const { return bonusIntel; }
    int BonusCP() const { return bonusCP; }

    // 單一箭頭 → DoctrineSet（純函式，產出可用 ToJson 序列化驗證）
    DoctrineSet ArrowToDoctrine(const PlanArrow& arrow) const;

    // 對 team 套用計畫：
    //  - 有專屬箭頭的小隊 → 該箭頭轉譯 + SetSquadObjective(arrow.to)
    //  - 無專屬箭頭的小隊 → 第一支未指派箭頭（squadName 空）兜底
    //  - 計畫加成：attackMul 乘 damagePerMember、intel/CP 入帳
    // 回傳實際指派 doctrine 的小隊數。
    int Apply(BattleController& battle, BattleResources* res,
              int team) const;

    // potato.battle_plan/1 序列化（戰前計畫可存檔/回放）
    std::string ToJson() const;
    bool FromJson(const std::string& json);

private:
    std::vector<PlanArrow> arrows;
    Vector2 rallyPoint{0.0f, 0.0f};
    bool hasRally = false;
    float attackMul = 1.0f;
    int bonusIntel = 0;
    int bonusCP = 0;
};

} // namespace Gameplay
} // namespace Potato
