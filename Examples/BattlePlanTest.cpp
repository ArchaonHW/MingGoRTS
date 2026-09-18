// BattlePlanTest - G-5 作戰計畫箭頭 → doctrine 無頭測試
// 驗收：畫箭頭產出合法 DoctrineSet（ToJson/FromJson 序列化可驗證）、
//       計畫加成（攻擊乘算/情報/CP）在執行層生效
#include "Gameplay/BattlePlan.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/Squad.h"

#include <cmath>
#include <cstdio>
#include <cstring>

using namespace Potato;
using namespace Potato::Gameplay;

static int g_pass = 0;
static int g_fail = 0;

static void Check(bool ok, const char* name) {
    if (ok) {
        g_pass++;
        printf("  [PASS] %s\n", name);
    } else {
        g_fail++;
        printf("  [FAIL] %s\n", name);
    }
}

int main() {
    printf("=== Battle Plan Tests (G-5) ===\n\n");

    // ---- [1] 箭頭 → DoctrineSet 轉譯 ----
    printf("[1] 箭頭轉譯\n");
    BattlePlan plan;
    plan.AddArrow("前鋒營", Vector2(2, 7), Vector2(8, 2), /*prio=*/30);
    Check(plan.ArrowCount() == 1, "加入一支箭頭");
    Check(plan.Arrow(0).squadName == "前鋒營", "箭頭綁定小隊名");

    DoctrineSet doc = plan.ArrowToDoctrine(plan.Arrow(0));
    Check(doc.Count() == 3, "轉譯出 3 條規則");
    const auto& rules = doc.Rules();
    Check(rules[0].trigger == DoctrineTrigger::HealthBelow &&
              rules[0].action == DoctrineAction::RetreatToRally &&
              rules[0].priority == 10,
          "保命規則 prio 10");
    Check(rules[1].trigger == DoctrineTrigger::ObjectiveReached &&
              rules[1].action == DoctrineAction::AttackNearest,
          "到位接敵 prio 20");
    Check(rules[2].trigger == DoctrineTrigger::Always &&
              rules[2].action == DoctrineAction::AdvanceToObjective &&
              rules[2].priority == 30,
          "主軸推進用箭頭 priority");

    // ---- [2] DoctrineSet 序列化驗證 ----
    printf("\n[2] DoctrineSet 序列化\n");
    std::string djson = doc.ToJson();
    Check(djson.find("potato.doctrine_set/1") != std::string::npos,
          "doctrine JSON schema");
    Check(djson.find("AdvanceToObjective") != std::string::npos,
          "JSON 含主軸動作");
    DoctrineSet doc2;
    Check(doc2.FromJson(djson), "doctrine 回讀");
    Check(doc2.Count() == doc.Count(), "回讀規則數一致");
    Check(doc2.Rules()[0].trigger == rules[0].trigger &&
              doc2.Rules()[2].action == rules[2].action,
          "回讀內容一致");
    Check(!doc2.FromJson("{\"schema\":\"potato.other/1\"}"),
          "錯 schema 拒絕");
    Check(!doc2.FromJson("{bad"), "壞 JSON 拒絕");

    // ---- [3] BattlePlan 序列化 ----
    printf("\n[3] 計畫序列化\n");
    plan.SetRallyPoint(Vector2(2, 8));
    plan.SetPlanBonus(1.15f, /*intel=*/2, /*cp=*/1);
    plan.AddArrow("", Vector2(1, 7), Vector2(7, 3)); // 通用兜底箭頭
    std::string pjson = plan.ToJson();
    Check(pjson.find("potato.battle_plan/1") != std::string::npos,
          "plan JSON schema");
    BattlePlan plan2;
    Check(plan2.FromJson(pjson), "plan 回讀");
    Check(plan2.ArrowCount() == 2, "回讀 2 支箭頭");
    Check(plan2.HasRallyPoint() &&
              std::fabs(plan2.RallyPoint().x - 2.0f) < 1e-3f,
          "集結點回讀");
    Check(std::fabs(plan2.AttackMultiplier() - 1.15f) < 1e-4f &&
              plan2.BonusIntel() == 2 && plan2.BonusCP() == 1,
          "加成回讀");
    Check(!plan2.FromJson("{bad"), "壞 plan JSON 拒絕");

    // ---- [4] Apply：專屬箭頭 + 通用兜底 + 計畫加成 ----
    printf("\n[4] Apply\n");
    BattleController battle(12, 10, 1.0f);
    Squad* vanguard = battle.CreateSquad("前鋒營", 0, Vector2(2, 7), 20);
    Squad* reserve = battle.CreateSquad("預備隊", 0, Vector2(1, 8), 15);
    Squad* enemy = battle.CreateSquad("敵軍", 1, Vector2(8, 2), 25);
    BattleResources res;
    res.Setup(battle, 0, /*intel=*/5, /*cp=*/2);

    const float baseDpm = vanguard->GetDamagePerMember();
    int n = plan.Apply(battle, &res, /*team=*/0);
    Check(n == 2, "兩支我軍都獲指派");
    Check(battle.GetDoctrine(vanguard) != nullptr, "前鋒營有 doctrine");
    Check(battle.GetDoctrine(reserve) != nullptr, "預備隊兜底有 doctrine");
    Check(battle.GetDoctrine(vanguard)->Rules()[2].priority == 30,
          "前鋒營主軸 priority 30");
    Check(std::fabs(vanguard->GetDamagePerMember() - baseDpm * 1.15f) <
              1e-4f,
          "計畫加成攻擊 ×1.15");
    Check(res.GetIntel(0) == 7, "情報 +2 入帳");
    Check(res.GetCP(0) == 3, "CP +1 入帳");
    Check(battle.GetCommandPoints(0) == 3, "CP 同步進 controller");
    Check(std::fabs(battle.GetRallyPoint(0).x - 2.0f) < 1e-3f,
          "集結點寫入");

    // 敵隊不受我方計畫影響
    Check(battle.GetDoctrine(enemy) == nullptr, "敵軍無我方 doctrine");

    // ---- [5] 執行層：沿箭頭推進 ----
    printf("\n[5] 執行層生效\n");
    Check(battle.BeginExecution(), "開戰");
    const Vector2 startPos = vanguard->GetPosition();
    for (int i = 0; i < 120; ++i) {
        battle.Update(0.1f);
    }
    const float distStart = (plan.Arrow(0).to - startPos).Length();
    const float distNow =
        (plan.Arrow(0).to - vanguard->GetPosition()).Length();
    Check(distNow < distStart, "前鋒營朝箭頭目標推進");
    Check(vanguard->GetOrder() == SquadOrder::AttackMove ||
              vanguard->GetOrder() == SquadOrder::Engage,
          "前鋒營執行主軸命令");

    // ---- [6] 無箭頭小隊保持原 doctrine ----
    printf("\n[6] 無對應箭頭\n");
    BattleController b2(10, 10, 1.0f);
    Squad* loner = b2.CreateSquad("孤軍", 0, Vector2(2, 5), 10);
    DoctrineSet keep;
    keep.AddRule(DoctrineRule(DoctrineTrigger::Always,
                              DoctrineAction::HoldPosition, 0.0f, 50));
    b2.AssignDoctrine(loner, keep);
    BattlePlan empty;
    empty.AddArrow("別隊", Vector2(0, 0), Vector2(9, 9));
    Check(empty.Apply(b2, nullptr, 0) == 0, "無通用箭頭 → 不指派");
    Check(b2.GetDoctrine(loner)->Rules()[0].action ==
              DoctrineAction::HoldPosition,
          "原 doctrine 不動");

    // ---- [7] 加成邊界 ----
    printf("\n[7] 加成邊界\n");
    BattlePlan clamped;
    clamped.SetPlanBonus(-1.0f, -5, -3);
    Check(std::fabs(clamped.AttackMultiplier() - 1.0f) < 1e-6f,
          "負倍率 → 1.0");
    Check(clamped.BonusIntel() == 0 && clamped.BonusCP() == 0,
          "負加成 → 0");

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
