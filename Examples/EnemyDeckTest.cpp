// enemy_deck 無頭測試——EnemyDeckLibrary 載入/攤平/套用 +
// ChapterDef.enemy_deck 端對端（章節欄位 → 牌組 → 敵將手牌）。
// 執行檔以 0 表全數通過；任一 Check 失敗印 [FAIL] 並回 1。

#include "Gameplay/EnemyDeck.h"
#include "Gameplay/EnemyGeneral.h"
#include "Gameplay/Squad.h"

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;
using namespace Potato::Gameplay;

static int g_fail = 0;
static void Check(bool cond, const char* msg) {
    if (cond) {
        std::printf("[PASS] %s\n", msg);
    } else {
        std::printf("[FAIL] %s\n", msg);
        ++g_fail;
    }
}

static std::string FindAssets() {
    // ctest 工作目錄是 build*/bin——上兩層回 repo 根
    for (const char* p : {"assets", "../../assets"}) {
        std::error_code ec;
        if (fs::is_directory(p, ec)) return p;
    }
    return "assets";
}

int main() {
    const std::string assets = FindAssets();
    const std::string doctrineDir = assets + "/doctrine";
    const std::string deckDir = assets + "/decks";

    // [1] 卡池與牌組庫載入
    DoctrineLibrary lib;
    Check(lib.LoadDir(doctrineDir) > 0, "doctrine pool loaded");
    EnemyDeckLibrary decks;
    const int nDeck = decks.LoadDir(deckDir);
    Check(nDeck >= 1, "enemy deck loaded");
    Check(decks.Find("duanqiao_garrison") != nullptr,
          "duanqiao_garrison found");
    Check(decks.Find("nonexistent") == nullptr, "unknown deck absent");

    // [2] 攤平套用：hold_line(3)+skirmish_scout+rally_guard 進敵將手牌
    {
        EnemyGeneral g = EnemyGeneral::MakeGlock();
        Squad probe("probe", 1, {0.0f, 0.0f}, 5);
        const int before =
            (int)g.BuildDoctrineFor(probe, 0, 1).Rules().size();
        const EnemyDeck* d = decks.Find("duanqiao_garrison");
        const int added = decks.ApplyTo(g, d, lib);
        Check(added > 0, "deck rules merged");
        const int after =
            (int)g.BuildDoctrineFor(probe, 0, 1).Rules().size();
        Check(after == before + added, "doctrine grew by deck rules");

        // 具名去重：同一 deck 再套不加規則
        Check(decks.ApplyTo(g, d, lib) == 0, "reapply deduped");
        Check((int)g.BuildDoctrineFor(probe, 0, 1).Rules().size() == after,
              "doctrine unchanged after reapply");
    }

    // [3] 未知卡 id → 警告且略過，不炸整束
    {
        EnemyDeckLibrary bad;
        const char* tmp = "enemy_deck_test_tmp.json";
        {
            std::ofstream f(tmp);
            f << R"({"schema":"potato.enemy_deck/1","id":"x",
                     "cards":["hold_line","no_such_card"]})";
        }
        Check(bad.LoadFile(tmp) == 1, "deck with bad ref loads");
        EnemyGeneral g = EnemyGeneral::MakeGlock();
        const int added = bad.ApplyTo(g, bad.Find("x"), lib);
        Check(added == 3, "known card applied despite bad sibling");
        bool warned = false;
        for (const auto& w : bad.Warnings())
            warned = warned || w.find("no_such_card") != std::string::npos;
        Check(warned, "unknown card id warned");
        std::error_code ec;
        fs::remove(tmp, ec);
    }

    // [4] 壞檔降級：壞 JSON/錯 schema/缺 id 各記警告不中止
    {
        EnemyDeckLibrary bad;
        const char* tmp = "enemy_deck_bad_tmp.json";
        {
            std::ofstream f(tmp);
            f << "{not json";
        }
        Check(bad.LoadFile(tmp) == 0, "broken json rejected");
        {
            std::ofstream f(tmp);
            f << R"({"schema":"potato.other/1","id":"y"})";
        }
        Check(bad.LoadFile(tmp) == 0, "wrong schema rejected");
        {
            std::ofstream f(tmp);
            f << R"({"schema":"potato.enemy_deck/1"})";
        }
        Check(bad.LoadFile(tmp) == 0, "missing id rejected");
        Check(bad.Warnings().size() >= 3, "all degrades warned");
        std::error_code ec;
        fs::remove(tmp, ec);
    }

    // [5] 同 id 後載覆蓋（疊層語義）
    {
        EnemyDeckLibrary dup;
        const char* tmp = "enemy_deck_dup_tmp.json";
        {
            std::ofstream f(tmp);
            f << R"({"schema":"potato.enemy_deck/1","id":"d",
                     "cards":["hold_line"]})";
        }
        dup.LoadFile(tmp);
        {
            std::ofstream f(tmp);
            f << R"({"schema":"potato.enemy_deck/1","id":"d",
                     "name":"新版","cards":["focus_weakest"]})";
        }
        dup.LoadFile(tmp);
        const EnemyDeck* d = dup.Find("d");
        Check(d && d->name == "新版" && d->cardIds.size() == 1,
              "duplicate id overridden");
        std::error_code ec;
        fs::remove(tmp, ec);
    }

    // [6] 章節欄位端對端：duanqiao.json 的 enemy_deck 能在庫中解析
    {
        // 只驗資產契約——ChapterLibrary 屬 Campaign 層，
        // Gameplay 測試不連結（依賴方向守衛）
        std::ifstream in(assets + "/campaign/duanqiao.json");
        std::string chapter;
        if (in) {
            std::ostringstream ss;
            ss << in.rdbuf();
            chapter = ss.str();
        }
        Check(chapter.find("\"enemy_deck\": \"duanqiao_garrison\"")
                  != std::string::npos ||
                  chapter.find("\"enemy_deck\":\"duanqiao_garrison\"")
                      != std::string::npos,
              "duanqiao chapter wires enemy_deck");
    }

    if (g_fail == 0) std::printf("ALL PASS\n");
    return g_fail == 0 ? 0 : 1;
}
