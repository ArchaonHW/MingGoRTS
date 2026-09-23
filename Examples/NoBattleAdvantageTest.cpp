// NoBattleAdvantageTest — E-2 首波優勢兌現（headless）
// 覆蓋：敵資源增量（CP/intel）、我軍士氣減益與夾取下限、
// 零小隊邊界、敵隊小隊不受減益、members=0 跳過。

#include "Gameplay/BattleController.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/NoBattleAdvantage.h"
#include "Gameplay/Squad.h"

#include <cmath>
#include <cstdio>

using namespace Potato::Gameplay;
using Potato::Vector2;

static bool Near(float a, float b) { return std::fabs(a - b) < 0.001f; }

static int failures = 0;
static void Check(bool cond, const char* name) {
    if (cond) {
        printf("  [PASS] %s\n", name);
    } else {
        printf("  [FAIL] %s\n", name);
        ++failures;
    }
}

int main() {
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    // ---- [1] 標準套用：敵 +2CP/+2intel、我軍士氣 -0.10 ----
    {
        BattleController battle(16, 16, 1.0f);
        BattleResources res;
        res.Setup(battle, 0, 8, 5);
        res.Setup(battle, 1, 0, 3);
        Squad* mine = battle.CreateSquad("前鋒", 0, Vector2(4, 4), 30);
        Squad* foe  = battle.CreateSquad("敵衛", 1, Vector2(12, 12), 30);
        const float mBefore = mine->GetMorale();
        const float fBefore = foe->GetMorale();

        ApplyFirstWaveAdvantage(battle, res, 1, 0);

        Check(res.GetCP(1) == 5, "敵 CP +2");
        Check(res.GetIntel(1) == 2, "敵 intel +2");
        Check(res.GetCP(0) == 5 && res.GetIntel(0) == 8,
              "我軍資源不變");
        Check(Near(mine->GetMorale(), mBefore - 0.10f), "我軍士氣 -0.10");
        Check(foe->GetMorale() == fBefore, "敵隊士氣不減");
    }

    // ---- [2] 士氣夾取下限：0.05 → 0.10（不歸零不潰逃）----
    {
        BattleController battle(16, 16, 1.0f);
        BattleResources res;
        res.Setup(battle, 0, 0, 0);
        res.Setup(battle, 1, 0, 0);
        Squad* s = battle.CreateSquad("弱旅", 0, Vector2(4, 4), 30);
        // 先壓到 0.05
        s->AdjustMorale(0.05f - s->GetMorale());
        Check(Near(s->GetMorale(), 0.05f), "前置士氣 0.05");

        ApplyFirstWaveAdvantage(battle, res, 1, 0);

        Check(Near(s->GetMorale(), 0.10f), "士氣夾到 0.10");
        Check(!s->IsRouting(), "不觸發潰逃");
    }

    // ---- [3] 零我軍小隊：敵資源照加、不崩潰 ----
    {
        BattleController battle(16, 16, 1.0f);
        BattleResources res;
        res.Setup(battle, 0, 0, 0);
        res.Setup(battle, 1, 1, 1);
        battle.CreateSquad("敵隊", 1, Vector2(8, 8), 20);

        ApplyFirstWaveAdvantage(battle, res, 1, 0);

        Check(res.GetCP(1) == 3 && res.GetIntel(1) == 3,
              "零我軍小隊：敵資源照加");
    }

    // ---- [4] 全滅小隊跳過（members<=0 的 AdjustMorale 早退）----
    {
        BattleController battle(16, 16, 1.0f);
        BattleResources res;
        res.Setup(battle, 0, 0, 0);
        res.Setup(battle, 1, 0, 0);
        Squad* dead = battle.CreateSquad("殘部", 0, Vector2(4, 4), 0);
        const float mBefore = dead->GetMorale();

        ApplyFirstWaveAdvantage(battle, res, 1, 0);

        Check(dead->GetMorale() == mBefore, "全滅小隊士氣不動");
    }

    printf("=== %s ===\n",
           failures == 0 ? "ALL PASS" : "FAILURES");
    return failures == 0 ? 0 : 1;
}
