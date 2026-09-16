#pragma once

#include "Core/CoreTypes.h"
#include "MathUtils/Vector2.h"

#include <string>

namespace Potato {
namespace Gameplay {

class FlowField;

/**
 * 小隊指令（玩家直接介入或 doctrine 動作產生的底層命令）
 */
enum class SquadOrder {
    Hold,           // 原地駐守
    MoveTo,         // 沿 flow field 往目標移動
    AttackMove,     // 往目標移動，途中接戰
    Retreat,        // 往集結點撤退
    Engage          // 追擊指定敵隊
};

/**
 * 小隊（Squad）——本設計的最小可控單位
 *
 * 成員數即戰力：傷亡扣減 members 與 morale；
 * morale 歸零進入 routing（潰逃），不再受控直到脫離戰鬥。
 */
class Squad {
public:
    Squad(const std::string& name, int team, const Vector2& pos, int members);

    const std::string& GetName() const { return name; }
    int GetTeam() const { return team; }
    const Vector2& GetPosition() const { return position; }
    int GetMembers() const { return members; }
    int GetMaxMembers() const { return maxMembers; }
    float GetMorale() const { return morale; }          // 0.0 ~ 1.0
    float GetHealthPct() const { return static_cast<float>(members) / maxMembers; }
    float GetSpeed() const { return speed; }
    float GetEngageRange() const { return engageRange; }
    float GetAttackDPS() const;

    SquadOrder GetOrder() const { return order; }
    const Vector2& GetOrderTarget() const { return orderTarget; }
    const Squad* GetEngageTarget() const { return engageTarget; }

    bool IsEliminated() const { return members <= 0; }
    bool IsRouting() const { return routing; }
    bool IsEngaged() const { return engaged; }

    void SetSpeed(float s) { speed = s; }
    void SetEngageRange(float r) { engageRange = r; }
    void SetDamagePerMember(float d) { damagePerMember = d; }

    void IssueOrder(SquadOrder newOrder, const Vector2& target);
    void IssueOrder(SquadOrder newOrder, const Squad* target);

    // 造成傷亡：members 減少並依傷亡比例扣 morale
    void ApplyCasualties(int count);
    // 士氣回復（每秒），僅未接戰時
    void RecoverMorale(float dt);

    // 由 BattleController 每 tick 呼叫
    void Update(float dt, const FlowField* field);

    // 戰鬥狀態由 controller 設定
    void SetEngaged(bool value) { engaged = value; }
    void SetUnderAttack(bool value) { underAttack = value; }
    bool IsUnderAttack() const { return underAttack; }

private:
    void MoveToward(const Vector2& dest, float dt, const FlowField* field);

    std::string name;
    int team;
    Vector2 position;

    int members;
    int maxMembers;
    float morale;
    bool routing;
    bool engaged;
    bool underAttack;

    float speed;          // 世界單位/秒
    float engageRange;    // 接戰距離（世界單位）
    float damagePerMember;// 每名成員每秒傷害

    SquadOrder order;
    Vector2 orderTarget;
    const Squad* engageTarget;
};

} // namespace Gameplay
} // namespace Potato
