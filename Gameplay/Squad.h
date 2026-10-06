#pragma once

#include "Core/CoreTypes.h"
#include "MathUtils/Vector2.h"

#include <algorithm>
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
 * 兵種（G-2 克制三角）：騎>弓、弓>步、步>騎。
 * 克制方輸出 ×1.5，被克方 ×0.7（ResolveCombat 套用）。
 */
enum class UnitClass {
    Infantry,   // 步
    Archer,     // 弓
    Cavalry     // 騎
};

// 顯示/序列化用
const char* UnitClassName(UnitClass cls);
UnitClass UnitClassFromString(const std::string& s, bool* ok = nullptr);

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
    // 疲勞修正後的實際移速（疲憊時打折）
    float GetEffectiveSpeed() const;
    float GetStamina() const { return stamina; }        // 0.0 ~ 1.0
    bool IsExhausted() const { return stamina < exhaustedThreshold; }
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
    float GetDamagePerMember() const { return damagePerMember; }

    // G-5 計畫加成記錄：已乘進 damagePerMember 的作戰計畫倍率。
    // 重複 Apply 時先除回舊倍率再乘新倍率 → 同一份計畫不會疊乘。
    void SetPlanAttackMul(float m) { planAttackMul = (m > 0.0f) ? m : 1.0f; }
    float GetPlanAttackMul() const { return planAttackMul; }

    // 兵種（G-2）：預設步兵；克制矩陣見 UnitClass 註解
    UnitClass GetUnitClass() const { return unitClass; }
    void SetUnitClass(UnitClass c) { unitClass = c; }

    // 將軍衛隊（G-8）：旗標本身不改變戰鬥行為；
    // 全滅 → BattleController 直接判該隊落敗（潰逃不算）
    void SetGeneralGuard(bool v) { generalGuard = v; }
    bool IsGeneralGuard() const { return generalGuard; }

    // 帶隊突擊（G-8）：chargeTimer > 0 時速度/火力 ×1.5，自然衰減
    void StartCharge(float seconds);
    bool IsCharging() const { return chargeTimer > 0.0f; }

    // 疲勞參數（每秒速率 / 閾值 / 疲憊移速倍率）
    void SetStaminaParams(float drainMove, float drainCombat,
                          float regen, float threshold, float penaltyMul);
    void SetStamina(float s) { stamina = std::clamp(s, 0.0f, 1.0f); }

    void IssueOrder(SquadOrder newOrder, const Vector2& target);
    void IssueOrder(SquadOrder newOrder, const Squad* target);

    // 造成傷亡：members 減少並依傷亡比例扣 morale
    void ApplyCasualties(int count);
    // 指揮層（SageCommand 政策效果等）直接調整士氣；不造成傷亡。
    // 士氣歸零同樣觸發潰逃（routing），已潰逃者不會因士氣回升而復歸。
    void AdjustMorale(float delta);
    // 士氣回復（每秒），僅未接戰時
    void RecoverMorale(float dt);

    // 由 BattleController 每 tick 呼叫
    void Update(float dt, const FlowField* field);

    // 戰鬥狀態由 controller 設定
    void SetEngaged(bool value) { engaged = value; }
    void SetUnderAttack(bool value) { underAttack = value; }
    bool IsUnderAttack() const { return underAttack; }

private:
    // 回傳本 tick 是否實際位移
    bool MoveToward(const Vector2& dest, float dt, const FlowField* field);

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
    float planAttackMul = 1.0f; // G-5：已套用的計畫加成倍率
    UnitClass unitClass = UnitClass::Infantry; // G-2 兵種

    // 將軍衛隊（G-8）與帶隊突擊計時
    bool generalGuard = false;
    float chargeTimer = 0.0f;

    // 疲勞（G-3）：移動/交戰消耗，駐守回復；低於閾值移速打折
    float stamina;
    float staminaDrainMove;
    float staminaDrainCombat;
    float staminaRegen;
    float exhaustedThreshold;
    float exhaustedSpeedMul;

    SquadOrder order;
    Vector2 orderTarget;
    const Squad* engageTarget;
};

} // namespace Gameplay
} // namespace Potato
