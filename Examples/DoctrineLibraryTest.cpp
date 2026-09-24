// DoctrineLibraryTest —— G-1 doctrine 卡池無頭測試：
// 載入/驗證/axis 分布/E-D 掛鉤覆蓋/壞卡跳過/規則轉譯/子集加總。

#include "Gameplay/DoctrineLibrary.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <set>
#include <string>

namespace fs = std::filesystem;
using namespace Potato;
using namespace Potato::Gameplay;

static int failures = 0;
static int checks = 0;

static void Check(bool cond, const char* name) {
    ++checks;
    if (!cond) {
        ++failures;
        std::printf("FAIL: %s\n", name);
    }
}

int main() {
    // ---- 正式卡池載入 ----
    std::string dir = "assets/doctrine";
    if (!fs::is_directory(dir)) dir = "../../assets/doctrine";
    DoctrineLibrary lib;
    const int n = lib.LoadDir(dir);
    Check(n >= 10, "正式卡池卡數");
    Check(lib.Size() == static_cast<size_t>(n), "Size==載入數");
    Check(lib.Warnings().empty(), "正式卡池零警告");

    // ---- axis 分布與 E/D 掛鉤覆蓋 ----
    Check(lib.Axis("combat").size() >= 4, "combat 軸卡數");
    Check(lib.Axis("nobattle").size() >= 3, "nobattle 軸卡數");
    Check(lib.Axis("myth").size() >= 2, "myth 軸卡數");
    // AC：無戰軸（受降/談判籌碼）與神話軸（安撫/天命）掛鉤皆有卡覆蓋
    Check(lib.TotalModifier("nobattle.negotiation") > 0.0f,
          "掛鉤 nobattle.negotiation");
    Check(lib.TotalModifier("nobattle.surrender") > 0.0f,
          "掛鉤 nobattle.surrender");
    Check(lib.TotalModifier("nobattle.deterrence") > 0.0f,
          "掛鉤 nobattle.deterrence");
    Check(lib.TotalModifier("nobattle.subversion") > 0.0f,
          "掛鉤 nobattle.subversion");
    Check(lib.TotalModifier("myth.soothing") > 0.0f,
          "掛鉤 myth.soothing");
    Check(lib.TotalModifier("myth.fate") > 0.0f, "掛鉤 myth.fate");

    // 卡上所有 modifier key 都應在註冊表內（自洽，防野 key）
    bool allKnown = true;
    for (const DoctrineCard& c : lib.Cards())
        for (const DoctrineModifier& m : c.modifiers)
            allKnown = allKnown && DoctrineLibrary::IsHookKey(m.key);
    Check(allKnown, "正式卡池無未知 hook key");

    // ---- 規則轉譯進 DoctrineSet ----
    {
        const DoctrineCard* v = lib.Find("vanguard_push");
        Check(v != nullptr && v->rules.size() == 2, "卡規則數");
        DoctrineSet set;
        v->AppendRules(set);
        Check(set.Count() == 2, "AppendRules 注入");
        // priority 升序：EnemyInRange(10) 應在 Always(90) 前
        Check(set.Rules()[0].trigger == DoctrineTrigger::EnemyInRange,
              "priority 排序保持");
    }

    // ---- 子集加總（G-2 牌庫消費面）----
    {
        const float neg = lib.TotalModifier({"envoy_deference",
                                             "quarter_accept"},
                                            "nobattle.negotiation");
        Check(neg > 0.19f && neg < 0.21f, "子集 modifier 加總");
        Check(lib.TotalModifier({"envoy_deference"},
                                "nobattle.deterrence") == 0.0f,
              "子集缺 key 為零");
        Check(lib.TotalModifier({"no_such_card"},
                                "nobattle.negotiation") == 0.0f,
              "未知 id 不炸");
    }

    // ---- 壞卡跳過 + warn，不影響好卡 ----
    {
        const fs::path tmp = "doctrine_lib_test_tmp";
        fs::create_directories(tmp);
        {
            std::ofstream f(tmp / "good.json");
            f << R"({"schema":"potato.doctrine/1","id":"ok_card",
"name":"好卡","axis":"combat",
"rules":[{"trigger":"Always","action":"HoldPosition"}]})";
        }
        {
            std::ofstream f(tmp / "badtrigger.json");
            f << R"({"schema":"potato.doctrine/1","id":"bad_trig",
"rules":[{"trigger":"Nonsense","action":"HoldPosition"}]})";
        }
        {
            std::ofstream f(tmp / "empty.json");
            f << R"({"schema":"potato.doctrine/1","id":"empty_card"})";
        }
        {
            std::ofstream f(tmp / "badschema.json");
            f << R"({"schema":"potato.doctrine_set/1","rules":[]})";
        }
        {
            std::ofstream f(tmp / "weird.json");
            f << R"({"schema":"potato.doctrine/1","id":"weird",
"axis":"chaos","cost":-3,
"modifiers":[{"key":"future.hook","value":1.0},
             {"key":"","value":2.0}],
"rules":[{"trigger":"Always","action":"HoldPosition",
"cooldown":-1}]})";
        }
        {
            std::ofstream f(tmp / "zz_dup.json");
            f << R"({"schema":"potato.doctrine/1","id":"ok_card",
"name":"覆蓋者","axis":"myth",
"modifiers":[{"key":"myth.fate","value":0.5}]})";
        }

        DoctrineLibrary t;
        const int m = t.LoadDir(tmp.string());
        Check(m == 3, "好卡+降級卡+覆蓋卡入池、壞卡全跳");
        Check(t.Warnings().size() >= 6, "各類壞況皆記警告");
        Check(t.Find("bad_trig") == nullptr, "未知 trigger 拒收");
        Check(t.Find("empty_card") == nullptr, "空卡拒收");
        const DoctrineCard* w = t.Find("weird");
        Check(w != nullptr && w->axis == "combat" && w->cost == 0,
              "壞 axis/cost 降級保留");
        Check(w && w->modifiers.size() == 1,
              "未知 hook key 保留、空 key 丟棄");
        Check(w && w->rules[0].cooldown == 0.0f,
              "負 cooldown 鉗回");
        // 覆蓋語義走明確呼叫序（目錄迭代序不定）
        Check(t.LoadFile((tmp / "zz_dup.json").string()) == 1,
              "後載檔回傳入池");
        const DoctrineCard* d = t.Find("ok_card");
        Check(d != nullptr && d->axis == "myth",
              "同 id 後載覆蓋");

        std::error_code ec;
        fs::remove_all(tmp, ec);
    }

    // ---- 目錄不存在 ----
    {
        DoctrineLibrary t;
        Check(t.LoadDir("no_such_dir_xyz") == 0 && !t.Warnings().empty(),
              "缺目錄記警告");
    }

    std::printf("%d checks, %d failures\n", checks, failures);
    return failures == 0 ? 0 : 1;
}
