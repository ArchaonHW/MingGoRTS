// RivalDeckTest - N-3 對手軍師反制牌組無頭測試
// 驗收：trigger 使用分佈跨場記錄/讀取、反制牌組產出合法 DoctrineSet
//       （ToJson 可驗證）、戰報判詞行輸出
#include "Gameplay/RivalDeck.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/Squad.h"

#include <cstdio>
#include <string>

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
    printf("=== Rival Deck Tests (N-3) ===\n\n");

    // ---- [1] 使用分佈記錄 ----
    printf("[1] trigger 分佈\n");
    RivalDeck rival;
    rival.RecordUsage(DoctrineTrigger::Always, 40);
    rival.RecordUsage(DoctrineTrigger::HealthBelow, 8);
    rival.RecordUsage(DoctrineTrigger::Always, 10);
    Check(rival.UsageCount(DoctrineTrigger::Always) == 50,
          "Always 累計 50");
    Check(rival.TotalUsage() == 58, "總計 58");
    DoctrineTrigger habit;
    Check(rival.MostUsedTrigger(habit) &&
              habit == DoctrineTrigger::Always,
          "最慣用 = Always");

    // ---- [2] 跨場累積（模擬連續兩場） ----
    printf("\n[2] 跨場累積\n");
    {
        BattleController b(12, 10, 1.0f);
        Squad* s = b.CreateSquad("我軍", 0, Vector2(2, 7), 20);
        b.CreateSquad("敵軍", 1, Vector2(8, 2), 20);
        // 玩家慣用「敵近就打」
        DoctrineSet playerDoc;
        playerDoc.AddRule(DoctrineRule(DoctrineTrigger::EnemyInRange,
                                       DoctrineAction::AttackNearest,
                                       6.0f, 10));
        playerDoc.AddRule(DoctrineRule(DoctrineTrigger::Always,
                                       DoctrineAction::HoldPosition,
                                       0.0f, 90));
        b.AssignDoctrine(s, playerDoc);
        b.SetObjective(0, Vector2(8, 2));
        b.BeginExecution();
        for (int i = 0; i < 100; ++i) {
            b.Update(0.1f);
        }
        rival.RecordBattle(b, /*playerTeam=*/0);
        Check(rival.TotalUsage() > 0, "戰鬥後有 trigger 統計");
    }
    {
        // 第二場：同樣習慣繼續累積
        BattleController b(12, 10, 1.0f);
        Squad* s = b.CreateSquad("我軍", 0, Vector2(2, 7), 20);
        b.CreateSquad("敵軍", 1, Vector2(8, 2), 20);
        DoctrineSet playerDoc;
        playerDoc.AddRule(DoctrineRule(DoctrineTrigger::EnemyInRange,
                                       DoctrineAction::AttackNearest,
                                       6.0f, 10));
        // 墊底規則保底——敵距超出 6 格時仍逐 tick 命中
        playerDoc.AddRule(DoctrineRule(DoctrineTrigger::Always,
                                       DoctrineAction::HoldPosition,
                                       0.0f, 90));
        b.AssignDoctrine(s, playerDoc);
        b.SetObjective(0, Vector2(8, 2));
        b.BeginExecution();
        for (int i = 0; i < 100; ++i) {
            b.Update(0.1f);
        }
        int before = rival.TotalUsage();
        rival.RecordBattle(b, 0);
        Check(rival.TotalUsage() > before, "第二場繼續累積");
    }

    // ---- [3] 反制牌組 ----
    printf("\n[3] 反制牌組\n");
    DoctrineSet counter = rival.BuildCounterDeck();
    Check(counter.Count() > 0, "反制牌組非空");
    std::string djson = counter.ToJson();
    Check(djson.find("potato.doctrine_set/1") != std::string::npos,
          "反制牌組可序列化驗證");
    DoctrineSet verify;
    Check(verify.FromJson(djson), "反制牌組回讀合法");
    Check(verify.Count() == counter.Count(), "回讀規則數一致");

    // 不同慣用 → 不同牌組
    RivalDeck rival2;
    rival2.RecordUsage(DoctrineTrigger::MoraleBelow, 30);
    DoctrineSet counter2 = rival2.BuildCounterDeck();
    Check(counter2.ToJson() != djson, "慣用不同 → 牌組不同");

    // ---- [4] 戰報判詞行 ----
    printf("\n[4] 判詞行\n");
    std::string line = rival.WarningLine();
    Check(line.find("針對我軍慣用") != std::string::npos,
          "判詞行含反制語句");
    RivalDeck empty;
    Check(empty.WarningLine().find("無足判斷") != std::string::npos,
          "無資料判詞行");

    // ---- [5] 序列化持久化 ----
    printf("\n[5] 序列化\n");
    std::string rjson = rival.ToJson();
    Check(rjson.find("potato.rival_deck/1") != std::string::npos,
          "rival JSON schema");
    RivalDeck rival3;
    Check(rival3.FromJson(rjson), "回讀");
    Check(rival3.TotalUsage() == rival.TotalUsage(), "分佈一致");
    Check(rival3.WarningLine() == rival.WarningLine(), "判詞一致");
    Check(!rival3.FromJson("{bad"), "壞 JSON 拒絕");

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
