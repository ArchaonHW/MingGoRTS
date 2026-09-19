// QuantumPlanTest - quantum-plan-effects：計畫加成依情報確定度縮放
// I/O 矩陣：CONCENTRATED_TIP / REVEALED_AT_TIP / EMPTY_TERRAIN / NO_FOG /
//           RESOLVE_ELSEWHERE / PROBE_CONCENTRATES / IDEMPOTENT / NO_BONUS / REAPPLY
#include "Gameplay/BattlePlan.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/QuantumFog.h"
#include "Gameplay/Squad.h"

#include <cmath>
#include <cstdio>

using namespace Potato;
using namespace Potato::Gameplay;

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* name) {
    if (ok) { ++g_pass; printf("  [PASS] %s\n", name); }
    else    { ++g_fail; printf("  [FAIL] %s\n", name); }
}

int main() {
    printf("=== Quantum Plan Effects Tests ===\n\n");

    // 共用 fixture：20x15 戰場，我軍前鋒在 (2,7)，敵軍真身在 (15,7)；
    // 雲候選 (15,7)=0.8、(17,9)=0.2；箭頭尖端 (15,7) 半徑 2 內質量 0.8
    // → effectiveMul = 1 + (1.3-1)*0.8 = 1.24
    printf("[1] CONCENTRATED_TIP：尖端壓雲 0.8 → ×1.24\n");
    {
        BattleController battle(20, 15, 1.0f);
        BattleResources res;
        res.Setup(battle, 0, /*intel=*/5, /*cp=*/0);
        QuantumFog fog(30.0f, 1);
        fog.BindResources(&res);
        battle.BindFog(&fog);

        Squad* vg = battle.CreateSquad("前鋒", 0, Vector2(2, 7), 20);
        Squad* enemy = battle.CreateSquad("敵軍", 1, Vector2(15, 7), 20);
        const int eid = fog.AddEntity("敵軍", /*觀測方=*/0,
            {Vector2(15, 7), Vector2(17, 9)}, {0.8, 0.2});
        battle.BindFogSquad(enemy, eid);

        BattlePlan plan;
        plan.SetPlanBonus(1.3f, 0, 0);
        plan.AddArrow("前鋒", Vector2(2, 7), Vector2(15, 7));
        battle.BindPlan(&plan);

        const float baseDpm = vg->GetDamagePerMember();
        Check(plan.Apply(battle, &res, 0) == 1, "Apply 指派 1 隊");
        Check(std::fabs(vg->GetPlanAttackMul() - 1.24f) < 1e-4f,
              "planAttackMul ≈ 1.24");
        Check(std::fabs(vg->GetDamagePerMember() - baseDpm * 1.24f) < 1e-4f,
              "damagePerMember 乘上 1.24");

        // [2] REVEALED_AT_TIP：真身在尖端 → certainty 1 → 滿額 1.3
        printf("[2] REVEALED_AT_TIP：坐實尖端 → ×1.30\n");
        Check(fog.Reveal(eid, Vector2(15, 7)), "揭露於尖端");
        plan.UpdateUncertainty(battle);
        Check(std::fabs(vg->GetPlanAttackMul() - 1.30f) < 1e-4f,
              "揭露後回升滿額");
        Check(std::fabs(vg->GetDamagePerMember() - baseDpm * 1.30f) < 1e-4f,
              "damagePerMember 跟進 1.30");

        // [3] RESOLVE_ELSEWHERE：真身在遠方 → 賭輸掉到 1.0
        printf("[3] RESOLVE_ELSEWHERE：真身離尖 → ×1.00\n");
        Check(fog.Reveal(eid, Vector2(1, 1)), "改揭露於遠方");
        plan.UpdateUncertainty(battle);
        Check(std::fabs(vg->GetPlanAttackMul() - 1.0f) < 1e-4f,
              "落空後加成歸 1");
        Check(std::fabs(vg->GetDamagePerMember() - baseDpm) < 1e-4f,
              "damagePerMember 回到基準");

        // [4] IDEMPOTENT：fog 不變連續重評 → 逐位元不動
        printf("[4] IDEMPOTENT：重評冪等\n");
        const float dpm1 = vg->GetDamagePerMember();
        plan.UpdateUncertainty(battle);
        plan.UpdateUncertainty(battle);
        Check(vg->GetDamagePerMember() == dpm1,
              "連續 UpdateUncertainty damagePerMember 逐位元不變");
    }

    // [5] EMPTY_TERRAIN：尖端遠離所有雲 → 滿額（不構成懷疑）
    printf("[5] EMPTY_TERRAIN：尖端無雲 → 滿額\n");
    {
        BattleController battle(20, 15, 1.0f);
        QuantumFog fog(30.0f, 1);
        battle.BindFog(&fog);
        Squad* vg = battle.CreateSquad("前鋒", 0, Vector2(2, 7), 20);
        fog.AddEntity("敵軍", 0, {Vector2(15, 7), Vector2(17, 9)},
                      {0.8, 0.2});

        BattlePlan plan;
        plan.SetPlanBonus(1.3f, 0, 0);
        plan.AddArrow("前鋒", Vector2(2, 7), Vector2(5, 5)); // 尖端無雲
        plan.Apply(battle, nullptr, 0);
        Check(std::fabs(vg->GetPlanAttackMul() - 1.3f) < 1e-4f,
              "無雲尖端仍滿額 1.3");
    }

    // [6] NO_FOG：完全不綁 fog → 與舊行為一致
    printf("[6] NO_FOG：無霧 → 滿額\n");
    {
        BattleController battle(20, 15, 1.0f);
        Squad* vg = battle.CreateSquad("前鋒", 0, Vector2(2, 7), 20);
        BattlePlan plan;
        plan.SetPlanBonus(1.3f, 0, 0);
        plan.AddArrow("前鋒", Vector2(2, 7), Vector2(15, 7));
        plan.Apply(battle, nullptr, 0);
        Check(std::fabs(vg->GetPlanAttackMul() - 1.3f) < 1e-4f,
              "無 fog 滿額 1.3");
        plan.UpdateUncertainty(battle); // 不崩潰、不改值
        Check(std::fabs(vg->GetPlanAttackMul() - 1.3f) < 1e-4f,
              "無 fog UpdateUncertainty 安全無效");
    }

    // [7] PROBE_CONCENTRATES：探測收縮雲 → 加成回升
    printf("[7] PROBE_CONCENTRATES：探測集中 → 加成升\n");
    {
        BattleController battle(20, 15, 1.0f);
        BattleResources res;
        res.Setup(battle, 0, /*intel=*/5, /*cp=*/0);
        QuantumFog fog(30.0f, 1, /*probe=*/1);
        fog.BindResources(&res);
        battle.BindFog(&fog);

        Squad* vg = battle.CreateSquad("前鋒", 0, Vector2(2, 7), 20);
        Squad* enemy = battle.CreateSquad("敵軍", 1, Vector2(15, 7), 20);
        // 尖端候選先驗只有 0.3 → eff = 1 + 0.3*0.3 = 1.09
        const int eid = fog.AddEntity("敵軍", 0,
            {Vector2(15, 7), Vector2(17, 9)}, {0.3, 0.7});
        battle.BindFogSquad(enemy, eid);

        BattlePlan plan;
        plan.SetPlanBonus(1.3f, 0, 0);
        plan.AddArrow("前鋒", Vector2(2, 7), Vector2(15, 7));
        battle.BindPlan(&plan);
        plan.Apply(battle, &res, 0);
        Check(std::fabs(vg->GetPlanAttackMul() - 1.09f) < 1e-4f,
              "低質量尖端 → ×1.09");

        Check(fog.Probe(eid, Vector2(15, 7), /*strength=*/1.0f),
              "探測向尖端收縮");
        plan.UpdateUncertainty(battle);
        Check(vg->GetPlanAttackMul() > 1.09f + 1e-4f,
              "雲集中後加成上升");
    }

    // [8] NO_BONUS：attackMul 1.0 → 不論 certainty 恆 1.0
    printf("[8] NO_BONUS：無加成 → 恆 1.0\n");
    {
        BattleController battle(20, 15, 1.0f);
        QuantumFog fog(30.0f, 1);
        battle.BindFog(&fog);
        Squad* vg = battle.CreateSquad("前鋒", 0, Vector2(2, 7), 20);
        const int eid = fog.AddEntity("敵軍", 0,
            {Vector2(15, 7), Vector2(17, 9)}, {1.0, 0.0});
        Check(eid >= 0, "NO_BONUS 雲確實註冊（單候選會回 -1）");

        BattlePlan plan;
        plan.AddArrow("前鋒", Vector2(2, 7), Vector2(15, 7)); // 無 SetPlanBonus
        battle.BindPlan(&plan);
        plan.Apply(battle, nullptr, 0);
        plan.UpdateUncertainty(battle);
        Check(std::fabs(vg->GetPlanAttackMul() - 1.0f) < 1e-6f,
              "attackMul 1 → planAttackMul 1");
    }

    // [9] REAPPLY：中途重跑 Apply → 重綁定不疊乘
    printf("[9] REAPPLY：重跑 Apply 不疊乘\n");
    {
        BattleController battle(20, 15, 1.0f);
        QuantumFog fog(30.0f, 1);
        battle.BindFog(&fog);
        Squad* vg = battle.CreateSquad("前鋒", 0, Vector2(2, 7), 20);
        fog.AddEntity("敵軍", 0, {Vector2(15, 7), Vector2(17, 9)},
                      {0.8, 0.2});

        BattlePlan plan;
        plan.SetPlanBonus(1.3f, 0, 0);
        plan.AddArrow("前鋒", Vector2(2, 7), Vector2(15, 7));
        const float baseDpm = vg->GetDamagePerMember();
        plan.Apply(battle, nullptr, 0);
        Check(std::fabs(vg->GetDamagePerMember() - baseDpm * 1.24f) < 1e-4f,
              "首次 Apply ×1.24");
        plan.Apply(battle, nullptr, 0);
        plan.Apply(battle, nullptr, 0);
        Check(std::fabs(vg->GetDamagePerMember() - baseDpm * 1.24f) < 1e-4f,
              "重複 Apply 不疊乘");
    }

    // [10] 控制器自動路徑：BindPlan 後 execution tick 自動重評
    printf("[10] BindPlan：tick 自動重評\n");
    {
        BattleController battle(20, 15, 1.0f);
        BattleResources res;
        res.Setup(battle, 0, /*intel=*/5, /*cp=*/0);
        QuantumFog fog(30.0f, 1);
        fog.BindResources(&res);
        battle.BindFog(&fog);

        Squad* vg = battle.CreateSquad("前鋒", 0, Vector2(2, 7), 20);
        Squad* enemy = battle.CreateSquad("敵軍", 1, Vector2(15, 7), 20);
        const int eid = fog.AddEntity("敵軍", 0,
            {Vector2(15, 7), Vector2(17, 9)}, {0.8, 0.2});
        battle.BindFogSquad(enemy, eid);

        BattlePlan plan;
        plan.SetPlanBonus(1.3f, 0, 0);
        plan.AddArrow("前鋒", Vector2(2, 7), Vector2(15, 7));
        battle.BindPlan(&plan);
        plan.Apply(battle, &res, 0);
        Check(std::fabs(vg->GetPlanAttackMul() - 1.24f) < 1e-4f,
              "Apply 後 ×1.24");

        // 直接揭露於遠方再跑 tick——不經手動 UpdateUncertainty
        fog.Reveal(eid, Vector2(1, 1));
        battle.BeginExecution();
        battle.Update(0.1f);
        Check(std::fabs(vg->GetPlanAttackMul() - 1.0f) < 1e-4f,
              "tick 自動重評 → 落空歸 1");
    }

    // [11] UNCLAIMED_REVEAL：雲從未覆蓋尖端的揭露不拖垮加成
    printf("[11] UNCLAIMED_REVEAL：無關揭露不懲罰\n");
    {
        BattleController battle(20, 15, 1.0f);
        QuantumFog fog(30.0f, 1);
        battle.BindFog(&fog);
        Squad* vg = battle.CreateSquad("前鋒", 0, Vector2(2, 7), 20);
        // 雲 A 覆蓋尖端（質量 0.6，另一候選在 R 外）；雲 B 與尖端無關
        const int eidA = fog.AddEntity("敵軍A", 0,
            {Vector2(5, 5), Vector2(10, 10)}, {0.6, 0.4});
        const int eidB = fog.AddEntity("敵軍B", 0,
            {Vector2(15, 7), Vector2(17, 9)}, {0.8, 0.2});

        BattlePlan plan;
        plan.SetPlanBonus(1.3f, 0, 0);
        plan.AddArrow("前鋒", Vector2(2, 7), Vector2(5, 5));
        const float baseDpm = vg->GetDamagePerMember();
        plan.Apply(battle, nullptr, 0);
        Check(std::fabs(vg->GetPlanAttackMul() - 1.18f) < 1e-4f,
              "雲 A 質量 0.6 → ×1.18");

        // 揭露無關的雲 B——尖端加成不動（舊碼會誤掉到 1.0）
        fog.Reveal(eidB, Vector2(15, 7));
        plan.UpdateUncertainty(battle);
        Check(std::fabs(vg->GetPlanAttackMul() - 1.18f) < 1e-4f,
              "無關揭露後仍 ×1.18");
        Check(std::fabs(vg->GetDamagePerMember() - baseDpm * 1.18f) < 1e-4f,
              "無關揭露後 damagePerMember 不變");

        // 雲 A 的雲確實覆蓋過尖端——揭露他處才算賭輸歸 1
        fog.Reveal(eidA, Vector2(1, 1));
        plan.UpdateUncertainty(battle);
        Check(std::fabs(vg->GetPlanAttackMul() - 1.0f) < 1e-4f,
              "覆蓋過尖端的雲落空 → 歸 1");
    }

    // [12] ARROW_MUTATION：Apply 後改動箭頭 → 綁定失效且倍率回滾
    printf("[12] ARROW_MUTATION：改箭頭回滾加成\n");
    {
        BattleController battle(20, 15, 1.0f);
        QuantumFog fog(30.0f, 1);
        battle.BindFog(&fog);
        Squad* vg = battle.CreateSquad("前鋒", 0, Vector2(2, 7), 20);
        fog.AddEntity("敵軍", 0, {Vector2(15, 7), Vector2(17, 9)},
                      {0.8, 0.2});

        BattlePlan plan;
        plan.SetPlanBonus(1.3f, 0, 0);
        plan.AddArrow("前鋒", Vector2(2, 7), Vector2(15, 7));
        const float baseDpm = vg->GetDamagePerMember();
        plan.Apply(battle, nullptr, 0);
        Check(std::fabs(vg->GetPlanAttackMul() - 1.24f) < 1e-4f,
              "Apply 後 ×1.24");

        Check(plan.RemoveArrowFor("前鋒"), "移除箭頭");
        Check(std::fabs(vg->GetPlanAttackMul() - 1.0f) < 1e-4f,
              "移除後 planAttackMul 回滾 1.0");
        Check(std::fabs(vg->GetDamagePerMember() - baseDpm) < 1e-4f,
              "移除後 damagePerMember 回基準");
        plan.UpdateUncertainty(battle); // 綁定已清——安全無效
        Check(std::fabs(vg->GetDamagePerMember() - baseDpm) < 1e-4f,
              "UpdateUncertainty 不再改值");
    }

    // [13] DEBUFF_PLAN：attackMul<1 減益計畫也吃確定度縮放
    printf("[13] DEBUFF_PLAN：減益計畫對稱縮放\n");
    {
        BattleController battle(20, 15, 1.0f);
        QuantumFog fog(30.0f, 1);
        battle.BindFog(&fog);
        Squad* vg = battle.CreateSquad("前鋒", 0, Vector2(2, 7), 20);
        const int eid = fog.AddEntity("敵軍", 0,
            {Vector2(15, 7), Vector2(17, 9)}, {0.8, 0.2});

        BattlePlan plan;
        plan.SetPlanBonus(0.8f, 0, 0); // 減益計畫
        plan.AddArrow("前鋒", Vector2(2, 7), Vector2(15, 7));
        plan.Apply(battle, nullptr, 0);
        // effMul = 1 + (0.8-1)*0.8 = 0.84
        Check(std::fabs(vg->GetPlanAttackMul() - 0.84f) < 1e-4f,
              "減益依 0.8 質量縮放 → ×0.84");
        fog.Reveal(eid, Vector2(15, 7));
        plan.UpdateUncertainty(battle);
        Check(std::fabs(vg->GetPlanAttackMul() - 0.8f) < 1e-4f,
              "坐實後減益到位 ×0.80");
    }

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
