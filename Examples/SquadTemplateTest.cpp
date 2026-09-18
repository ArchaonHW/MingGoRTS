// SquadTemplateTest - G-6 小隊編制模板無頭測試
// 覆蓋：schema 驗證/壞 JSON 拒絕、stats 子欄位預設、stamina 套用與缺省、
//       unit_class 無效降級、Instantiate 屬性全套對驗、模板庫
//       LoadDir/Find/SortedByCost、預算跳過（不降人數）與 budget=0 無上限

#include "Gameplay/SquadTemplate.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/Squad.h"

#include <cmath>
#include <cstdio>

using namespace Potato;
using namespace Potato::Gameplay;

static int g_pass = 0;
static int g_fail = 0;

static void Check(bool ok, const char* name, float value = 0.0f,
                  float expected = 0.0f) {
    if (ok) {
        g_pass++;
        printf("  [PASS] %s\n", name);
    } else {
        g_fail++;
        printf("  [FAIL] %s  (got %.4f, expected %.4f)\n", name, value,
               expected);
    }
}

int main() {
    printf("=== Squad Template Tests (G-6) ===\n\n");

    // ---- [1] 快樂路徑：完整模板解析 + Instantiate 屬性全套對驗 ----
    printf("[1] 完整模板解析與建隊\n");
    const char* tplJson = R"({
        "schema": "potato.squad_template/1",
        "id": "test_cav",
        "name": "測試騎兵隊",
        "unit_class": "cavalry",
        "members": 20,
        "cost": 150,
        "stats": {
            "speed": 3.5,
            "engage_range": 2.0,
            "damage_per_member": 0.06,
            "stamina": {
                "drain_move": 0.5,
                "drain_combat": 0.08,
                "regen": 0.04,
                "threshold": 0.9,
                "penalty_mul": 0.25
            }
        }
    })";
    SquadTemplate t1;
    Check(t1.LoadFromString(tplJson), "完整模板解析");
    Check(t1.id == "test_cav" && t1.name == "測試騎兵隊", "id/name");
    Check(t1.members == 20, "members", (float)t1.members, 20.0f);
    Check(t1.unitClass == UnitClass::Cavalry, "unit_class cavalry");
    Check(t1.cost == 150, "cost", (float)t1.cost, 150.0f);
    Check(std::fabs(t1.speed - 3.5f) < 1e-6f, "stats.speed");
    Check(std::fabs(t1.engageRange - 2.0f) < 1e-6f, "stats.engage_range");
    Check(std::fabs(t1.damagePerMember - 0.06f) < 1e-6f,
          "stats.damage_per_member");
    Check(t1.hasStamina, "stamina 子物件已讀");
    Check(t1.warnings.empty(), "合法模板無警告");

    {
        BattleController battle(10, 10, 1.0f);
        Squad* s = t1.Instantiate(battle, "騎兵一隊", 0, Vector2(3, 3));
        Check(s != nullptr, "Instantiate 建隊成功");
        Check(s && s->GetName() == "騎兵一隊", "名稱套用");
        Check(s && s->GetMembers() == 20, "members 套用");
        Check(s && s->GetUnitClass() == UnitClass::Cavalry, "兵種套用");
        Check(s && std::fabs(s->GetSpeed() - 3.5f) < 1e-6f, "speed 套用");
        Check(s && std::fabs(s->GetEngageRange() - 2.0f) < 1e-6f,
              "engageRange 套用");
        Check(s && std::fabs(s->GetAttackDPS() - 0.06f * 20) < 1e-4f,
              "damagePerMember 套用（DPS=1.2）",
              s ? s->GetAttackDPS() : 0.0f, 1.2f);
        // stamina 套用：threshold 0.9 → stamina 0.5 即疲憊；
        // penalty_mul 0.25 → 有效速度 = 3.5 * 0.25
        if (s) {
            s->SetStamina(0.5f);
            Check(s->IsExhausted(), "stamina threshold 套用（0.5<0.9 疲憊）");
            Check(std::fabs(s->GetEffectiveSpeed() - 3.5f * 0.25f) < 1e-4f,
                  "stamina penalty_mul 套用",
                  s->GetEffectiveSpeed(), 3.5f * 0.25f);
            // drain_move 0.5/s：往遠處走 1 秒 → stamina ≈ 0.5-0.5=0.0
            s->SetStamina(0.5f);
            s->IssueOrder(SquadOrder::MoveTo, Vector2(9, 9));
            s->Update(1.0f, nullptr);
            Check(std::fabs(s->GetStamina() - 0.0f) < 1e-3f,
                  "stamina drain_move 套用", s->GetStamina(), 0.0f);
        }
        // 空名 → 退回模板 name
        Squad* s2 = t1.Instantiate(battle, "", 1, Vector2(5, 5));
        Check(s2 && s2->GetName() == "測試騎兵隊", "空名退回模板 name");
    }

    // ---- [2] 缺欄位預設 + unit_class 降級 + stamina 整段可選 ----
    printf("\n[2] 缺省值與降級路徑\n");
    {
        SquadTemplate t2;
        Check(t2.LoadFromString(
                  R"({"schema":"potato.squad_template/1","id":"t2"})"),
              "極簡模板（僅 schema+id）解析");
        Check(t2.members == 10, "缺省 members=10", (float)t2.members, 10.0f);
        Check(t2.unitClass == UnitClass::Infantry, "缺省 unit_class Infantry");
        Check(t2.cost == 0, "缺省 cost=0");
        Check(std::fabs(t2.speed - 2.0f) < 1e-6f &&
                  std::fabs(t2.engageRange - 1.5f) < 1e-6f &&
                  std::fabs(t2.damagePerMember - 0.05f) < 1e-6f,
              "缺 stats 子欄位 → Squad 預設值");
        Check(!t2.hasStamina, "無 stamina 段 → hasStamina=false");
        Check(t2.name == "t2", "缺 name 退回 id");

        // 無 stamina 段的隊保持 Squad 預設疲勞參數
        BattleController b(10, 10, 1.0f);
        Squad* ds = t2.Instantiate(b, "預設隊", 0, Vector2(2, 2));
        if (ds) {
            ds->SetStamina(0.5f);
            Check(!ds->IsExhausted(),
                  "無 stamina 段 → 預設閾值 0.3（0.5 不疲憊）");
            Check(std::fabs(ds->GetEffectiveSpeed() - ds->GetSpeed()) < 1e-6f,
                  "無 stamina 段 → 預設不減速");
        } else {
            Check(false, "預設模板建隊");
        }

        // stamina 部分欄位：寫了的套用，缺的用預設
        SquadTemplate t3;
        Check(t3.LoadFromString(R"({
                  "schema":"potato.squad_template/1","id":"t3",
                  "stats":{"stamina":{"threshold":0.8,"penalty_mul":0.5}}
              })"),
              "stamina 部分欄位仍解析");
        Check(t3.hasStamina &&
                  std::fabs(t3.staminaThreshold - 0.8f) < 1e-6f &&
                  std::fabs(t3.staminaDrainMove - 0.03f) < 1e-6f,
              "stamina 缺欄用預設（threshold 0.8 / drain_move 0.03）");
    }

    // ---- [3] 壞檔拒絕：壞 JSON / 非物件 / schema 錯 / 無效兵種降級 ----
    printf("\n[3] 壞檔拒絕與兵種降級\n");
    {
        SquadTemplate bad;
        Check(!bad.LoadFromString("{bad json"), "壞 JSON 拒絕");
        Check(!bad.LoadFromString("[1,2,3]"), "非物件根節點拒絕");
        Check(!bad.LoadFromString(R"({"id":"x"})"), "缺 schema 拒絕");
        Check(!bad.LoadFromString(
                  R"({"schema":"potato.character_card/1","id":"x"})"),
              "錯 schema 拒絕");

        SquadTemplate t4;
        Check(t4.LoadFromString(R"({
                  "schema":"potato.squad_template/1","id":"t4",
                  "unit_class":"laser_cav"
              })"),
              "未知兵種仍解析");
        Check(t4.unitClass == UnitClass::Infantry,
              "未知兵種降級 Infantry");
        Check(!t4.warnings.empty(), "降級記入 warnings");
    }

    // ---- [4] 模板庫：真檔 LoadDir / Find / SortedByCost ----
    printf("\n[4] SquadTemplateLibrary\n");
    {
        SquadTemplateLibrary lib;
        const size_t n = lib.LoadDir("assets/squads");
        Check(n >= 3, "assets/squads 載入 ≥3 模板", (float)n, 3.0f);
        Check(lib.Find("infantry_line") != nullptr, "Find infantry_line");
        Check(lib.Find("archer_skirmish") != nullptr, "Find archer_skirmish");
        Check(lib.Find("cavalry_shock") != nullptr, "Find cavalry_shock");
        Check(lib.Find("no_such_id") == nullptr, "Find 查無回 nullptr");

        const SquadTemplate* inf = lib.Find("infantry_line");
        const SquadTemplate* arc = lib.Find("archer_skirmish");
        const SquadTemplate* cav = lib.Find("cavalry_shock");
        Check(inf && inf->unitClass == UnitClass::Infantry &&
                  inf->members == 30,
              "infantry_line 欄位");
        Check(arc && arc->unitClass == UnitClass::Archer &&
                  std::fabs(arc->engageRange - 4.0f) < 1e-6f,
              "archer_skirmish 欄位");
        Check(cav && cav->unitClass == UnitClass::Cavalry &&
                  std::fabs(cav->speed - 3.5f) < 1e-6f,
              "cavalry_shock 欄位");

        auto sorted = lib.SortedByCost();
        Check(sorted.size() == lib.Size(), "SortedByCost 數量一致");
        bool ascending = true;
        for (size_t i = 1; i < sorted.size(); ++i) {
            if (sorted[i]->cost < sorted[i - 1]->cost) ascending = false;
        }
        Check(ascending, "SortedByCost 升冪");
    }

    // ---- [5] 預算配兵：依序檢查、超支跳過不降人數、budget=0 無上限 ----
    printf("\n[5] BudgetedBuild\n");
    {
        SquadTemplateLibrary lib;
        Check(lib.LoadDir("assets/squads") >= 3, "預算用模板庫載入");
        // cost: infantry_line 100 / archer_skirmish 120 / cavalry_shock 160

        // budget 220：inf 100 建（剩 120）→ cav 160 跳過 → arc 120 建
        auto r = BudgetedBuild(lib, 220,
                               {"infantry_line", "cavalry_shock",
                                "archer_skirmish"});
        Check(r.built.size() == 2, "預算內建 2 隊",
              (float)r.built.size(), 2.0f);
        Check(r.skipped.size() == 1 &&
                  r.skipped[0] == "cavalry_shock",
              "超支項跳過並記錄");
        Check(r.spent == 220, "spent=總成本 220", (float)r.spent, 220.0f);
        Check(r.built.size() == 2 && r.built[0]->id == "infantry_line" &&
                  r.built[1]->id == "archer_skirmish",
              "built 保留 wishlist 順序");

        // 跳過的隊真的沒建：built 套進 battle 只有 2 支
        {
            BattleController battle(10, 10, 1.0f);
            int built = 0;
            for (const SquadTemplate* tp : r.built) {
                if (tp->Instantiate(battle, "", 0, Vector2(1, 1))) ++built;
            }
            Check(built == 2 &&
                      battle.GetSquads().size() == 2,
                  "built 模板全建成", (float)built, 2.0f);
        }

        // budget=0 無上限：全建、spent=總成本
        auto r0 = BudgetedBuild(lib, 0,
                                {"infantry_line", "cavalry_shock",
                                 "archer_skirmish"});
        Check(r0.built.size() == 3 && r0.skipped.empty(),
              "budget=0 全部建成");
        Check(r0.spent == 380, "budget=0 spent=380",
              (float)r0.spent, 380.0f);

        // 查無 id 也記 skipped；預算只夠免費模板時貴隊全跳
        auto rBad = BudgetedBuild(lib, 50,
                                  {"ghost_template", "infantry_line"});
        Check(rBad.built.empty() && rBad.skipped.size() == 2,
              "查無 id + 超支皆記 skipped");
    }

    printf("\n=== 結果: %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
