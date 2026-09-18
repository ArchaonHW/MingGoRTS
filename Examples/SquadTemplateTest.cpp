// SquadTemplateTest - G-6 編制模板無頭測試
// 驗收：模板解析（缺省/未知兵種/陣列/包裝物件）、Instantiate 欄位對驗、
//       預算拒絕不降規、真檔 assets/templates/ 載入
#include "Gameplay/SquadTemplate.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/Squad.h"
#include <cstdio>
#include <cmath>

using namespace Potato::Gameplay;
using Potato::Vector2;

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* name) {
    if (ok) { g_pass++; printf("  [PASS] %s\n", name); }
    else    { g_fail++; printf("  [FAIL] %s\n", name); }
}

int main() {
    printf("=== Squad Template Tests (G-6) ===\n\n");

    // [1] 完整模板解析
    printf("[1] 模板解析\n");
    const char* tplJson = R"({
        "schema": "potato.squad_template/1",
        "id": "t1", "name": "測試連",
        "members": 20, "unit_class": "cavalry",
        "damage_per_member": 0.06, "speed": 3.5,
        "engage_range": 2.0, "morale": 0.8, "cost": 150
    })";
    SquadTemplate t1;
    Check(t1.LoadFromString(tplJson), "完整模板解析");
    Check(t1.id == "t1" && t1.name == "測試連", "id/name");
    Check(t1.members == 20, "members");
    Check(t1.unitClass == UnitClass::Cavalry, "unit_class cavalry");
    Check(std::fabs(t1.damagePerMember - 0.06f) < 1e-6f, "damage_per_member");
    Check(std::fabs(t1.speed - 3.5f) < 1e-6f, "speed");
    Check(std::fabs(t1.engageRange - 2.0f) < 1e-6f, "engage_range");
    Check(std::fabs(t1.morale - 0.8f) < 1e-6f, "morale");
    Check(t1.cost == 150, "cost");

    // [2] 缺省值與防護
    printf("\n[2] 缺省值\n");
    SquadTemplate t2;
    Check(t2.LoadFromString(R"({"id":"t2"})"), "極簡模板解析");
    Check(t2.members == 10, "缺省 members = 10");
    Check(t2.unitClass == UnitClass::Infantry, "缺省 unit_class Infantry");
    Check(t2.cost == 0, "缺省 cost = 0");
    SquadTemplate t3;
    Check(t3.LoadFromString(R"({"id":"t3","unit_class":"laser_cav"})"),
          "未知兵種仍解析");
    Check(t3.unitClass == UnitClass::Infantry, "未知兵種 → Infantry");
    SquadTemplate bad;
    Check(!bad.LoadFromString("[1,2,3]"), "非物件根節點拒絕");
    Check(!bad.LoadFromString("{bad json"), "壞 JSON 拒絕");

    // [3] LoadAll：陣列 / 包裝物件 / 單物件
    printf("\n[3] LoadAll 多模板\n");
    auto arr = SquadTemplate::LoadAll(
        R"([{"id":"a","members":5},{"id":"b","unit_class":"archer"}])");
    Check(arr.size() == 2, "陣列載入 2 筆");
    Check(arr.size() == 2 && arr[1].unitClass == UnitClass::Archer,
          "陣列第 2 筆 archer");
    auto wrapped = SquadTemplate::LoadAll(
        R"({"templates":[{"id":"x"},{"id":"y"},{"id":"z"}]})");
    Check(wrapped.size() == 3, "templates 包裝載入 3 筆");
    auto single = SquadTemplate::LoadAll(tplJson);
    Check(single.size() == 1, "單物件 → 1 筆");

    // [4] Instantiate 欄位對驗
    printf("\n[4] Instantiate\n");
    BattleController battle(10, 10, 1.0f);
    Squad* s = t1.Instantiate(battle, 0, Vector2(3, 3), "騎兵一隊");
    Check(s != nullptr, "建隊成功");
    Check(s && s->GetName() == "騎兵一隊", "nameOverride 生效");
    Check(s && s->GetMembers() == 20, "members 套用");
    Check(s && s->GetUnitClass() == UnitClass::Cavalry, "兵種套用");
    Check(s && std::fabs(s->GetSpeed() - 3.5f) < 1e-6f, "speed 套用");
    Check(s && std::fabs(s->GetEngageRange() - 2.0f) < 1e-6f,
          "engageRange 套用");
    Check(s && std::fabs(s->GetMorale() - 0.8f) < 1e-6f, "morale 套用");
    Check(s && std::fabs(s->GetAttackDPS() - 0.06f * 20) < 1e-4f,
          "damagePerMember 套用（DPS=1.2）");
    Squad* s2 = t1.Instantiate(battle, 1, Vector2(5, 5));
    Check(s2 && s2->GetName() == "測試連", "空 override 用模板名");

    // [5] 預算配兵：足額扣帳 / 超支拒絕不降規
    printf("\n[5] 預算規則\n");
    BattleController b2(10, 10, 1.0f);
    int budget = 200;
    Squad* ok = b2.CreateSquadFromTemplate(t1, 0, Vector2(1, 1), budget);
    Check(ok != nullptr, "足額建隊");
    Check(budget == 50, "扣帳 200-150=50");
    Squad* no = b2.CreateSquadFromTemplate(t1, 0, Vector2(2, 2), budget);
    Check(no == nullptr, "超支拒絕（50 < 150）");
    Check(budget == 50, "拒絕不扣帳");
    int infinite = -1;
    Squad* free1 = b2.CreateSquadFromTemplate(t1, 0, Vector2(3, 3), infinite);
    Check(free1 != nullptr && infinite == -1, "budget<0 無限不扣帳");
    SquadTemplate freeTpl;
    freeTpl.LoadFromString(R"({"id":"free","cost":0})");
    int zero = 0;
    Squad* free2 = b2.CreateSquadFromTemplate(freeTpl, 0, Vector2(4, 4), zero);
    Check(free2 != nullptr, "零成本模板 0 預算可建");

    // [6] 真檔資產載入
    printf("\n[6] 資產檔\n");
    SquadTemplate inf, cav;
    Check(inf.LoadFromFile("assets/templates/infantry_company.json"),
          "infantry_company.json 載入");
    Check(cav.LoadFromFile("assets/templates/cavalry_platoon.json"),
          "cavalry_platoon.json 載入");
    Check(inf.unitClass == UnitClass::Infantry && inf.cost == 100,
          "步兵連欄位");
    Check(cav.unitClass == UnitClass::Cavalry && cav.members == 12,
          "騎兵排欄位");

    printf("\n=== %d passed, %d failed ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
