// BattleSandboxTest — G-4 沙盤模擬器無頭驗證。
// 覆蓋：同 seed 可重現、攻性牌組勝被動牌組、trigger 統計、
// 報表 schema/欄位、牌組載入含壞檔拒絕。
#include "Examples/BattleSandboxCore.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace Potato;
using namespace Potato::Gameplay;

static int g_pass = 0, g_fail = 0;
static void Check(bool cond, const char* name) {
    if (cond) { ++g_pass; printf("  [PASS] %s\n", name); }
    else      { ++g_fail; printf("  [FAIL] %s\n", name); }
}

// 攻性牌組：見敵（30 格內恆真）即攻最近，否則推進目標
static DoctrineSet AggressiveDeck() {
    DoctrineSet d;
    d.AddRule({DoctrineTrigger::EnemyInRange,
               DoctrineAction::AttackNearest, 30.0f, 10});
    d.AddRule({DoctrineTrigger::Always,
               DoctrineAction::AdvanceToObjective, 0.0f, 100});
    return d;
}
// 被動牌組：只站崗
static DoctrineSet PassiveDeck() {
    DoctrineSet d;
    d.AddRule({DoctrineTrigger::Always, DoctrineAction::HoldPosition,
               0.0f, 100});
    return d;
}

static SandboxConfig Cfg(unsigned seed, int matches) {
    SandboxConfig c;
    c.deckA.name = "aggro";
    c.deckA.doctrine = AggressiveDeck();
    c.deckB.name = "passive";
    c.deckB.doctrine = PassiveDeck();
    c.matches = matches;
    c.seed = seed;
    c.maxSeconds = 60.0f;
    return c;
}

int main() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    printf("=== BattleSandbox Tests (G-4) ===\n\n");

    // ---- [1] 同 seed 完全可重現（CI 契約）----
    printf("[1] determinism\n");
    {
        const SandboxReport r1 = RunSandbox(Cfg(7, 4));
        const SandboxReport r2 = RunSandbox(Cfg(7, 4));
        Check(r1.results.size() == 4 && r2.results.size() == 4,
              "場次數正確");
        bool identical = true;
        for (size_t i = 0; i < r1.results.size(); ++i) {
            if (r1.results[i].winnerTeam != r2.results[i].winnerTeam ||
                r1.results[i].duration != r2.results[i].duration) {
                identical = false;
            }
        }
        Check(identical, "同 seed 逐場 winner/duration 逐位一致");
        Check(r1.winsA == r2.winsA && r1.winsB == r2.winsB &&
                  r1.draws == r2.draws,
              "累計勝場一致");
        Check(r1.trigA == r2.trigA && r1.trigB == r2.trigB,
              "trigger 統計一致");
        Check(SandboxReportToJson(r1, "a", "b") ==
                  SandboxReportToJson(r2, "a", "b"),
              "報表 JSON 逐位一致");
    }

    // ---- [2] 攻性牌組勝被動牌組（平衡訊號 sanity）----
    printf("[2] balance signal\n");
    {
        const SandboxReport rep = RunSandbox(Cfg(42, 6));
        Check(rep.winsA > rep.winsB,
              "主動進攻牌組勝場 > 純站崗牌組");
        Check(rep.winsA + rep.winsB + rep.draws == rep.matches,
              "勝負和加總 = 場次");
        Check(rep.totalDuration > 0.0f, "均時長統計非零");
        Check(rep.trigA.count("EnemyInRange") > 0 &&
                  rep.trigA.at("EnemyInRange") > 0,
              "攻方 EnemyInRange 觸發有記錄");
        Check(rep.trigB.count("Always") > 0, "守方 Always 觸發有記錄");
    }

    // ---- [3] 牌組載入 ----
    printf("[3] deck loading\n");
    {
        SandboxDeck d;
        Check(SandboxLoadDeck("no_such_deck.json", d) == false,
              "缺檔拒絕");
        {
            std::ofstream bad("sandbox_bad_deck.json",
                              std::ios::binary | std::ios::trunc);
            bad << "{\"schema\":\"potato.doctrine_set/1\",\"rules\":"
                   "[{\"trigger\":\"nonsense\"}]}";
        }
        Check(SandboxLoadDeck("sandbox_bad_deck.json", d) == false,
              "壞卡拒絕（unknown trigger）");
        {
            std::ofstream good("sandbox_good_deck.json",
                               std::ios::binary | std::ios::trunc);
            good << AggressiveDeck().ToJson();
        }
        Check(SandboxLoadDeck("sandbox_good_deck.json", d) == true,
              "合法 doctrine_set 載入");
        Check(d.name == "sandbox_good_deck", "牌組名取檔名去副檔名");
        Check(d.doctrine.Count() == 2, "規則數保留");
        std::remove("sandbox_bad_deck.json");
        std::remove("sandbox_good_deck.json");
    }

    // ---- [4] 報表 schema 與欄位 ----
    printf("[4] report\n");
    {
        const SandboxReport rep = RunSandbox(Cfg(3, 2));
        const std::string j = SandboxReportToJson(rep, "deckA", "deckB");
        JsonValue root;
        Check(JsonValue::ParseOk(j, root), "報表為合法 JSON");
        Check(root["schema"].AsString() == "potato.sandbox_report/1",
              "schema 標記");
        Check(root["matches"].AsInt() == 2, "matches 欄位");
        Check(root["deckA"]["wins"].AsInt() >= 0 &&
                  root["deckB"]["wins"].AsInt() >= 0,
              "雙邊勝場欄位");
        Check(root["deckA"]["triggerHits"].IsObject(),
              "triggerHits 欄位存在");
    }

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
