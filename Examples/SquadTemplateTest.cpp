// SquadTemplateTest - G-6 小隊編制模板無頭測試
// 覆蓋：schema 驗證/壞 JSON 拒絕、stats 子欄位預設、stamina 套用與缺省、
//       unit_class 無效降級、Instantiate 屬性全套對驗、模板庫
//       LoadDir/Find/SortedByCost、預算跳過（不降人數）與 budget=0 無上限

#include "Gameplay/SquadTemplate.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/Squad.h"

#include <cmath>
#include <cstdio>
#include <fstream>

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

        // 解析失敗 → 物件重置為預設（不殘留上次內容）
        SquadTemplate t5;
        Check(t5.LoadFromString(tplJson), "重置測試：先載合法模板");
        Check(!t5.LoadFromString("{bad"), "重置測試：再載壞 JSON 拒絕");
        Check(t5.id.empty() && t5.members == 10 && t5.cost == 0 &&
                  std::fabs(t5.speed - 2.0f) < 1e-6f &&
                  t5.unitClass == UnitClass::Infantry &&
                  !t5.hasStamina && t5.warnings.empty(),
              "失敗後欄位重置為預設");

        // 空 id + 空 name：可載入（記警告）但 Instantiate 拒絕匿名隊
        SquadTemplate tAnon;
        Check(tAnon.LoadFromString(
                  R"({"schema":"potato.squad_template/1"})"),
              "空 id/name 模板仍解析");
        Check(!tAnon.warnings.empty(), "空 id/name 記入 warnings");
        {
            BattleController ab(5, 5, 1.0f);
            Check(tAnon.Instantiate(ab, "", 0, Vector2(1, 1)) == nullptr,
                  "匿名模板 Instantiate 回 nullptr");
        }
    }

    // ---- [3b] 數值消毒：越界夾回 + warnings ----
    printf("\n[3b] 數值消毒\n");
    {
        SquadTemplate s1;
        Check(s1.LoadFromString(R"({
                  "schema":"potato.squad_template/1","id":"s1",
                  "members":-5,"cost":-20,
                  "stats":{"speed":-1.0,"engage_range":-2.0,
                           "damage_per_member":-0.5}
              })"),
              "負值欄位仍解析");
        Check(s1.members == 1, "members<=0 夾到 1", (float)s1.members, 1.0f);
        Check(s1.cost == 0, "cost<0 夾到 0", (float)s1.cost, 0.0f);
        Check(s1.speed == 0.0f && s1.engageRange == 0.0f &&
                  s1.damagePerMember == 0.0f,
              "stats 負值夾到 0");
        Check(s1.warnings.size() >= 5, "每個越界都記警告",
              (float)s1.warnings.size(), 5.0f);

        SquadTemplate s2;
        Check(s2.LoadFromString(R"({
                  "schema":"potato.squad_template/1","id":"s2",
                  "members":1e999,"cost":1e999,
                  "stats":{"stamina":{"threshold":5.0,"penalty_mul":-1.0}}
              })"),
              "非有限/越界仍解析");
        Check(s2.members == 10, "members 非有限 → 預設 10",
              (float)s2.members, 10.0f);
        Check(s2.cost == 0, "cost 非有限 → 預設 0", (float)s2.cost, 0.0f);
        Check(s2.hasStamina &&
                  std::fabs(s2.staminaThreshold - 1.0f) < 1e-6f &&
                  s2.staminaPenaltyMul == 0.0f,
              "threshold 夾回 [0,1]、penalty_mul 夾到 0");
        Check(!s2.warnings.empty(), "非有限/越界記警告");

        SquadTemplate s3;
        Check(s3.LoadFromString(R"({
                  "schema":"potato.squad_template/1","id":"s3",
                  "unit_class":42,"stats":{"stamina":"oops"}
              })"),
              "型別錯誤仍解析");
        Check(s3.unitClass == UnitClass::Infantry &&
                  !s3.hasStamina,
              "unit_class 數字→Infantry、stamina 非物件忽略");
        Check(s3.warnings.size() >= 2, "型別錯誤各記一條警告",
              (float)s3.warnings.size(), 2.0f);
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

        // LoadDir 守衛與累積+除重語義
        Check(lib.LoadDir("") == 0, "空目錄路徑回 0");
        const size_t before = lib.Size();
        const size_t again = lib.LoadDir("assets/squads");
        Check(again == n, "重載同目錄仍解析全部檔案",
              (float)again, (float)n);
        Check(lib.Size() == before,
              "同 id 除重 keep latest（Size 不變）",
              (float)lib.Size(), (float)before);

        // C-3 目錄整併：舊 assets/templates 已併入 assets/squads，
        // 遷移檔（infantry_company/cavalry_platoon）仍在且數值保留
        SquadTemplateLibrary lib2;
        Check(lib2.LoadDir("assets/squads") >= 5,
              "squads 整併後 ≥5 模板");
        const SquadTemplate* cp = lib2.Find("cavalry_platoon");
        Check(cp && cp->unitClass == UnitClass::Cavalry &&
                  cp->members == 12 && cp->cost == 160 &&
                  std::fabs(cp->speed - 3.5f) < 1e-6f &&
                  std::fabs(cp->damagePerMember - 0.06f) < 1e-6f,
              "cavalry_platoon 整併後數值保留");
        Check(lib2.Find("infantry_company") != nullptr,
              "infantry_company 整併後仍在");

        // Add / Clear
        SquadTemplateLibrary lib3;
        SquadTemplate manual;
        manual.LoadFromString(R"({"schema":"potato.squad_template/1",
            "id":"manual","cost":7})");
        lib3.Add(manual);
        Check(lib3.Size() == 1 && lib3.Find("manual") != nullptr,
              "Add 手工模板");
        lib3.Clear();
        Check(lib3.Size() == 0 && lib3.Find("manual") == nullptr,
              "Clear 清空");

        // LoadFromFile：直接路徑（cwd 暫存檔）+ ../ 逐層容錯（assets）
        {
            std::ofstream tmp("squad_template_direct_test.json");
            tmp << R"({"schema":"potato.squad_template/1","id":"tmp_direct",
                       "members":9})";
        }
        SquadTemplate direct;
        Check(direct.LoadFromFile("squad_template_direct_test.json") &&
                  direct.id == "tmp_direct" && direct.members == 9,
              "LoadFromFile 直接路徑");
        SquadTemplate nested;
        Check(nested.LoadFromFile("assets/squads/infantry_line.json") &&
                  nested.id == "infantry_line",
              "LoadFromFile ../ 逐層容錯");
        Check(!direct.LoadFromFile("no_such_template_file.json"),
              "LoadFromFile 壞路徑拒絕");
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

        // budget<0 同樣視為無上限（與 budget=0 一致的哨兵語義）
        auto rNeg = BudgetedBuild(lib, -100,
                                  {"infantry_line", "cavalry_shock",
                                   "archer_skirmish"});
        Check(rNeg.built.size() == 3 && rNeg.skipped.empty(),
              "budget<0 全部建成");
        Check(rNeg.spent == 380, "budget<0 spent=380",
              (float)rNeg.spent, 380.0f);
    }

    // ---- [6] CreateSquadFromTemplate：逐隊扣帳 API（哨兵不同）----
    printf("\n[6] CreateSquadFromTemplate\n");
    {
        BattleController battle(10, 10, 1.0f);
        SquadTemplate cav;
        Check(cav.LoadFromFile("assets/squads/cavalry_shock.json"),
              "載入 cavalry_shock 供扣帳測試");

        int budget = 200;
        Squad* ok = battle.CreateSquadFromTemplate(cav, 0, Vector2(1, 1),
                                                   budget);
        Check(ok != nullptr, "足額建隊");
        Check(budget == 40, "扣帳 200-160=40", (float)budget, 40.0f);
        Check(ok && std::fabs(ok->GetSpeed() - 3.5f) < 1e-6f,
              "模板 stats 經 CreateSquadFromTemplate 套用");

        Squad* no = battle.CreateSquadFromTemplate(cav, 0, Vector2(2, 2),
                                                   budget);
        Check(no == nullptr, "超支拒絕（40 < 160）");
        Check(budget == 40, "拒絕不扣帳", (float)budget, 40.0f);

        int infinite = -1;
        Squad* free1 = battle.CreateSquadFromTemplate(cav, 0,
                                                      Vector2(3, 3),
                                                      infinite);
        Check(free1 != nullptr && infinite == -1,
              "budget<0 無限不扣帳");

        // budget=0 在扣帳 API 是「真沒錢」：免費模板可建、要錢的不行
        int zero = 0;
        SquadTemplate freeTpl;
        freeTpl.LoadFromString(R"({"schema":"potato.squad_template/1",
            "id":"free","cost":0})");
        Check(battle.CreateSquadFromTemplate(freeTpl, 0, Vector2(4, 4),
                                             zero) != nullptr,
              "零成本模板 0 預算可建");
        Check(battle.CreateSquadFromTemplate(cav, 0, Vector2(5, 5),
                                             zero) == nullptr,
              "budget=0 對收費模板拒絕（哨兵差異）");
    }

    printf("\n=== 結果: %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
