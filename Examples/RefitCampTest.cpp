// RefitCampTest - G-9 整補循環無頭測試
// 驗收：傷兵入池恢復、陣亡移除、招募扣帳、戰力跨場保存、存檔 roundtrip
#include "Gameplay/RefitCamp.h"
#include "Gameplay/PostBattle.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/Roster.h"
#include "Gameplay/SquadTemplate.h"
#include <cstdio>
#include <cmath>

using namespace Potato::Gameplay;
using Potato::Vector2;

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* name) {
    if (ok) { g_pass++; printf("  [PASS] %s\n", name); }
    else    { g_fail++; printf("  [FAIL] %s\n", name); }
}

static VeteranUnit MakeUnit(const char* name, int members, int max) {
    VeteranUnit u;
    u.squadName = name;
    u.members = members;
    u.maxMembers = max;
    u.unitClass = UnitClass::Infantry;
    u.captainName = "隊長";
    return u;
}

int main() {
    printf("=== Refit Camp Tests (G-9) ===\n\n");

    // [1] 帳務與建軍
    printf("[1] 帳務\n");
    RefitCamp camp;
    camp.DepositLoot(10);
    camp.DepositLoot(-5); // 負值忽略
    Check(camp.GetLoot() == 10, "DepositLoot 入帳");
    camp.EnrollUnit(MakeUnit("前鋒", 30, 30));
    camp.EnrollUnit(MakeUnit("中軍", 25, 30));
    Check(camp.GetUnits().size() == 2, "EnrollUnit 建軍");
    Check(camp.FindUnit("前鋒") != nullptr, "FindUnit 命中");
    Check(camp.FindUnit("不存在") == nullptr, "FindUnit 查無");

    // [2] 戰後吸收：打一場真仗 → Settle → Absorb
    printf("\n[2] 戰後吸收\n");
    PostBattleReport report;
    Roster roster;
    {
        BattleController battle(10, 10, 1.0f);
        Squad* a = battle.CreateSquad("前鋒", 0, Vector2(2, 2), 30);
        battle.CreateSquad("敵軍", 1, Vector2(8, 8), 10);
        roster.Enroll(a, "老周", "captain", "斷刃");
        battle.BeginExecution();
        a->ApplyCasualties(10); // 前鋒損 10（6 傷兵 4 陣亡）
        battle.Update(0.1f);
        PostBattle pb;
        pb.SetWoundedRatio(0.6f);
        report = pb.Settle(battle, roster, 0);
    }
    Check(report.casualties.size() >= 1, "報告有傷亡記錄");
    const SquadCasualty* fc = nullptr;
    for (const auto& c : report.casualties) {
        if (c.squadName == "前鋒") fc = &c;
    }
    Check(fc && fc->lost == 10, "前鋒 lost=10");
    Check(fc && fc->wounded == 6, "前鋒 wounded=6（60%）");
    camp.Absorb(report, roster, 0);
    const VeteranUnit* v = camp.FindUnit("前鋒");
    Check(v && v->members == 20, "Absorb 兵力記回（30-10=20）");
    Check(v && v->wounded == 6, "Absorb 傷兵入池");
    Check(camp.FindUnit("中軍") != nullptr, "未參戰單位不受影響");

    // [3] 傷兵恢復：1 loot = 1 傷兵，不超編制
    printf("\n[3] 傷兵恢復\n");
    Check(camp.HealWounded(4) == 4, "醫治 4 名（花 4 點）");
    Check(camp.GetLoot() == 6, "戰利品 10-4=6");
    Check(camp.FindUnit("前鋒")->members == 24, "兵力 20+4=24");
    Check(camp.FindUnit("前鋒")->wounded == 2, "傷兵剩 2");
    // 補滿編制後 wounded 不再消耗（中軍滿編無傷兵）
    Check(camp.HealWounded(10) == 2, "剩 2 傷兵全醫");
    Check(camp.GetLoot() == 4, "戰利品 6-2=4");
    Check(camp.FindUnit("前鋒")->members == 26, "兵力 24+2=26");

    // [4] 招募：吃模板、扣帳、滿編入伍
    // 模板帶刻意偏離預設的 stats，供 [7] 驗 Deploy 回補
    printf("\n[4] 招募\n");
    SquadTemplateLibrary lib;
    SquadTemplate tpl;
    tpl.LoadFromString(R"({"schema":"potato.squad_template/1",
        "id":"rifle","name":"步兵連","unit_class":"infantry",
        "members":20,"cost":3,
        "stats":{"speed":3.5,"engage_range":2.0,
                 "damage_per_member":0.06,
                 "stamina":{"threshold":0.9,"penalty_mul":0.25}}})");
    lib.Add(tpl);
    Check(camp.Recruit(lib, "rifle"), "招募成功（4>=3）");
    Check(camp.GetLoot() == 1, "戰利品 4-3=1");
    Check(camp.GetUnits().size() == 3, "新單位入伍");
    const VeteranUnit* nu = camp.FindUnit("步兵連");
    Check(nu && nu->members == 20 && nu->maxMembers == 20, "滿編入伍");
    Check(nu && nu->templateId == "rifle", "templateId 記錄");
    Check(!camp.Recruit(lib, "rifle"), "戰利品不足拒絕（1<3）");
    Check(!camp.Recruit(lib, "ghost_unit"), "查無模板拒絕");
    Check(camp.GetLoot() == 1, "拒絕不扣帳");

    // [5] 遺物配備
    printf("\n[5] 遺物配備\n");
    RefitCamp camp2;
    camp2.EnrollUnit(MakeUnit("衛隊", 10, 10));
    PostBattleReport r2;
    r2.relics.push_back("敵將軍刀");
    camp2.Absorb(r2, roster, 0);
    Check(camp2.Inventory().size() == 1, "拾獲遺物入庫");
    Check(camp2.AssignRelic("衛隊", "敵將軍刀"), "配備成功");
    Check(camp2.Inventory().empty(), "庫存清空");
    Check(camp2.FindUnit("衛隊")->relics.size() == 1, "單位持有遺物");
    Check(!camp2.AssignRelic("衛隊", "不存在的"), "庫存沒有拒絕");

    // [6] 被殲滅單位移除
    printf("\n[6] 殲滅移除\n");
    RefitCamp camp3;
    camp3.EnrollUnit(MakeUnit("敢死隊", 10, 10));
    PostBattleReport r3;
    SquadCasualty c3;
    c3.squadName = "敢死隊"; c3.team = 0;
    c3.lost = 10; c3.dead = 10; c3.eliminated = true;
    r3.casualties.push_back(c3);
    camp3.Absorb(r3, roster, 0);
    Check(camp3.FindUnit("敢死隊") == nullptr, "殲滅單位移除");

    // 殘編老兵再損：lost 從部署兵力起算,不能從編制上限回推
    RefitCamp camp4;
    camp4.EnrollUnit(MakeUnit("殘連", 20, 30));
    PostBattleReport r4;
    SquadCasualty c4;
    c4.squadName = "殘連"; c4.team = 0;
    c4.lost = 5; c4.wounded = 3; c4.dead = 2; // 部署 20 人再損 5
    r4.casualties.push_back(c4);
    camp4.Absorb(r4, roster, 0);
    Check(camp4.FindUnit("殘連")->members == 15, "殘編再損 20-5=15");
    Check(camp4.FindUnit("殘連")->wounded == 3, "殘編傷兵入池");

    // [7] Deploy 跨場重建 + 存檔 roundtrip
    printf("\n[7] Deploy 與存檔\n");
    BattleController b2(10, 10, 1.0f);
    std::vector<Vector2> pos = {Vector2(1, 1), Vector2(2, 1)};
    auto squads = camp.Deploy(b2, 0, pos, &lib);
    Check(squads.size() == 3, "Deploy 建 3 隊（positions 不足遞補）");
    Check(squads[0]->GetMembers() == 26, "前鋒兵力跨場保存");
    Check(squads[0]->GetPosition().x == 1.0f, "位置依序");
    // 招募單位（步兵連，templateId=rifle）回補模板 stats；
    // 初始單位（前鋒，無 templateId）維持 Squad 預設
    Check(std::fabs(squads[0]->GetSpeed() - 2.0f) < 1e-6f,
          "無 templateId 單位維持預設速度");
    Check(squads.size() == 3 &&
              std::fabs(squads[2]->GetSpeed() - 3.5f) < 1e-6f,
          "招募單位回補模板 speed=3.5");
    Check(squads.size() == 3 &&
              std::fabs(squads[2]->GetEngageRange() - 2.0f) < 1e-6f,
          "招募單位回補 engage_range=2.0");
    Check(squads.size() == 3 &&
              std::fabs(squads[2]->GetAttackDPS() - 0.06f * 20) < 1e-4f,
          "招募單位回補 damage_per_member=0.06");
    if (squads.size() == 3) {
        squads[2]->SetStamina(0.5f);
        Check(squads[2]->IsExhausted() &&
                  std::fabs(squads[2]->GetEffectiveSpeed() -
                            3.5f * 0.25f) < 1e-4f,
              "招募單位回補 stamina 參數（threshold 0.9/penalty 0.25）");
    }
    // 不給 library 的舊呼叫法仍可編譯（預設 nullptr，stats 不補）
    {
        BattleController b3(10, 10, 1.0f);
        auto s3 = camp.Deploy(b3, 0, pos);
        Check(s3.size() == 3 &&
                  std::fabs(s3[2]->GetSpeed() - 2.0f) < 1e-6f,
              "無 library 時招募單位只給預設 stats");
    }
    // 存檔 roundtrip
    Check(camp.SaveToFile("refit_camp_test.json"), "存檔");
    RefitCamp loaded;
    Check(loaded.LoadFromFile("refit_camp_test.json"), "讀檔");
    Check(loaded.GetLoot() == camp.GetLoot(), "loot 保存");
    Check(loaded.GetUnits().size() == camp.GetUnits().size(), "單位數保存");
    const VeteranUnit* lv = loaded.FindUnit("前鋒");
    Check(lv && lv->members == 26 && lv->maxMembers == 30,
          "前鋒狀態完整");
    Check(!loaded.LoadFromFile("nonexistent_camp.json"), "壞路徑拒絕");

    printf("\n=== %d passed, %d failed ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
