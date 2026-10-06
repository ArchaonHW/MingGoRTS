// PostBattleTest - G-7 戰後流轉無頭測試
// 驗收：傷亡分類（傷兵/陣亡）、戰利品依殲滅數入帳、恢復率可配置、回放記錄戰後事件
#include "Gameplay/PostBattle.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/BattleRecorder.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/Roster.h"
#include "Gameplay/Squad.h"
#include <cstdio>
#include <cstring>

using namespace Potato::Gameplay;
using Potato::Vector2;

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* name) {
    if (ok) { g_pass++; printf("  [PASS] %s\n", name); }
    else    { g_fail++; printf("  [FAIL] %s\n", name); }
}

int main() {
    printf("=== PostBattle Tests (G-7) ===\n\n");

    BattleController b(10, 10, 1.0f);
    Squad* alpha = b.CreateSquad("Alpha", 0, Vector2(2, 5), 30);
    Squad* bravo = b.CreateSquad("Bravo", 0, Vector2(2, 7), 20);
    Squad* foe1  = b.CreateSquad("敵甲", 1, Vector2(8, 5), 25);
    Squad* foe2  = b.CreateSquad("敵乙", 1, Vector2(8, 7), 15);

    Roster roster;
    roster.Enroll(alpha, "老周", "captain", "斷刃");
    roster.Enroll(foe1,  "格洛克", "captain", "敵將印");
    roster.Enroll(foe2,  "副官", "lieutenant", "");

    // 模擬戰損：Alpha 損 10、foe1/foe2 全滅
    alpha->ApplyCasualties(10);
    foe1->ApplyCasualties(25);
    foe2->ApplyCasualties(15);
    roster.Update(b);

    // [1] 傷亡分類
    PostBattle pb;
    pb.SetWoundedRatio(0.6f);
    auto rep = pb.Settle(b, roster, /*winnerTeam=*/0);
    Check(rep.winnerTeam == 0, "winnerTeam 記錄");
    Check(rep.casualties.size() == 3, "傷亡報告涵蓋受損三隊（Bravo 無損不入列）");
    Check(rep.TotalLost(0) == 10, "Alpha lost = 10");
    Check(rep.TotalWounded(0) == 6, "Alpha wounded = 10*0.6 = 6");
    Check(rep.TotalDead(0) == 4, "Alpha dead = 4");
    Check(rep.TotalLost(1) == 40, "敵方全損 25+15 = 40");

    // [2] 戰利品：殲滅 2 敵隊 × 預設 2 點 = 4
    Check(rep.lootPoints == 4, "戰利品 = 殲滅數×2 = 4");
    Check(rep.relics.size() == 1 && rep.relics[0] == "敵將印",
          "拾獲敵將遺物（無遺物者不列）");

    // [3] 入帳
    BattleResources res;
    res.Setup(b, 0, /*intel=*/5, /*cp=*/3);
    pb.ApplyLoot(res, 0, rep);
    Check(res.GetLoot(0) == 4, "戰利品入帳");
    Check(res.GetIntel(0) == 5, "情報不受戰利品影響");
    Check(res.SpendLoot(0, 3) && res.GetLoot(0) == 1, "戰利品可消費");
    Check(!res.SpendLoot(0, 5), "戰利品不足拒絕消費");

    // [4] 回放記錄戰後事件
    BattleRecorder rec;
    pb.RecordTo(rep, rec, 99.0f);
    Check(rec.Count() >= 5, "回放含結算+遺物+入帳事件");
    bool hasLoot = false;
    for (const auto& r : rec.GetRecords())
        if (r.event.find("戰利品入帳") != std::string::npos) hasLoot = true;
    Check(hasLoot, "回放含戰利品入帳行");

    // [5] 恢復率可配置
    PostBattle pb2;
    pb2.SetWoundedRatio(0.9f);
    auto rep2 = pb2.Settle(b, roster, 0);
    Check(rep2.TotalWounded(0) == 9, "恢復率 0.9 → wounded 9");
    pb2.SetWoundedRatio(2.0f); // 越界 clamp 到 1.0
    Check(pb2.GetWoundedRatio() == 1.0f, "恢復率 clamp [0,1]");

    // [6] Draw（-1）無戰利品
    auto rep3 = pb.Settle(b, roster, -1);
    Check(rep3.lootPoints == 0 && rep3.relics.empty(), "Draw 無戰利品/遺物");

    // [7] 敗方不入帳
    BattleResources res2;
    pb.ApplyLoot(res2, 1, rep);
    Check(res2.GetLoot(1) == 0, "敗方拿不到戰利品");

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail ? 1 : 0;
}
