// GeneralGuardTest - G-8 將軍親臨無頭測試
// 驗收：衛隊旗標、陣亡即敗（潰逃不算）、將軍激勵/帶隊突擊走 CP、
//       技能前置條件（非衛隊/CP 不足/已出局拒絕）
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
    printf("=== General Guard Tests (G-8) ===\n\n");

    // [1] 旗標
    printf("[1] 衛隊旗標\n");
    Squad g("衛隊", 0, Vector2(0, 0), 10);
    Check(!g.IsGeneralGuard(), "預設非衛隊");
    g.SetGeneralGuard(true);
    Check(g.IsGeneralGuard(), "SetGeneralGuard 生效");

    // [2] 陣亡即敗：衛隊全滅 → Defeat，即使其餘我軍存活
    printf("\n[2] 陣亡即敗\n");
    {
        BattleController b(10, 10, 1.0f);
        Squad* guard = b.CreateSquad("將軍衛隊", 0, Vector2(2, 2), 10);
        guard->SetGeneralGuard(true);
        b.CreateSquad("步兵隊", 0, Vector2(3, 2), 10); // 仍存活
        b.CreateSquad("敵軍", 1, Vector2(8, 8), 10);
        b.BeginExecution();
        guard->ApplyCasualties(10); // 全滅
        b.Update(0.1f);
        Check(b.GetOutcome() == BattleOutcome::Defeat,
              "衛隊全滅 → Defeat（步兵仍存活）");
    }
    // 無衛隊時一般判定不受影響
    {
        BattleController b(10, 10, 1.0f);
        b.CreateSquad("甲", 0, Vector2(2, 2), 10);
        b.CreateSquad("乙", 1, Vector2(8, 8), 10);
        b.BeginExecution();
        b.Update(0.1f);
        Check(b.GetOutcome() == BattleOutcome::Ongoing,
              "無衛隊陣亡 → 戰鬥持續");
    }
    // 潰逃不算陣亡
    {
        BattleController b(10, 10, 1.0f);
        Squad* guard = b.CreateSquad("將軍衛隊", 0, Vector2(2, 2), 10);
        guard->SetGeneralGuard(true);
        b.CreateSquad("步兵隊", 0, Vector2(3, 2), 10); // 非衛隊仍戰
        b.CreateSquad("敵軍", 1, Vector2(8, 8), 10);
        b.BeginExecution();
        guard->AdjustMorale(-1.0f); // 士氣歸零 → 潰逃
        Check(guard->IsRouting() && !guard->IsEliminated(),
              "衛隊潰逃但未消滅");
        b.Update(0.1f);
        Check(b.GetOutcome() == BattleOutcome::Ongoing,
              "衛隊潰逃不觸發落敗（步兵仍戰）");
    }

    // [3] 將軍激勵
    printf("\n[3] 將軍激勵\n");
    {
        BattleController b(20, 20, 1.0f);
        Squad* guard = b.CreateSquad("衛隊", 0, Vector2(5, 5), 10);
        guard->SetGeneralGuard(true);
        Squad* near = b.CreateSquad("近隊", 0, Vector2(8, 5), 10);
        Squad* far = b.CreateSquad("遠隊", 0, Vector2(15, 15), 10);
        b.CreateSquad("敵軍", 1, Vector2(19, 19), 10);
        near->AdjustMorale(-0.5f);
        far->AdjustMorale(-0.5f);
        b.SetCommandPoints(0, 1);
        b.BeginExecution();
        Check(b.GeneralRally(guard), "CP=1 激勵成功");
        Check(b.GetCommandPoints(0) == 0, "扣 1 CP");
        Check(std::fabs(near->GetMorale() - 0.8f) < 1e-5f,
              "半徑內士氣 +0.30");
        Check(std::fabs(far->GetMorale() - 0.5f) < 1e-5f,
              "半徑外不受影響");
        Check(!b.GeneralRally(guard), "CP=0 拒絕");
        Check(!b.GeneralRally(near), "非衛隊拒絕");
    }

    // [4] 帶隊突擊
    printf("\n[4] 帶隊突擊\n");
    {
        BattleController b(20, 20, 1.0f);
        Squad* guard = b.CreateSquad("衛隊", 0, Vector2(5, 5), 10);
        guard->SetGeneralGuard(true);
        b.CreateSquad("敵軍", 1, Vector2(19, 19), 10); // 讓戰鬥持續運轉
        b.SetCommandPoints(0, 2);
        b.BeginExecution();
        float baseSpeed = guard->GetEffectiveSpeed();
        float baseDps = guard->GetAttackDPS();
        Check(b.GeneralCharge(guard), "突擊成功");
        Check(b.GetCommandPoints(0) == 1, "扣 1 CP");
        Check(guard->IsCharging(), "IsCharging");
        Check(std::fabs(guard->GetEffectiveSpeed() - baseSpeed * 1.5f)
                  < 1e-5f, "速度 ×1.5");
        Check(std::fabs(guard->GetAttackDPS() - baseDps * 1.5f) < 1e-5f,
              "火力 ×1.5");
        for (int i = 0; i < 70; ++i) { // 7s > 6s 持續
            b.Update(0.1f);
        }
        Check(!guard->IsCharging(), "到期歸零");
        Check(std::fabs(guard->GetEffectiveSpeed() - baseSpeed) < 1e-5f,
              "速度回復");
        Check(b.GeneralCharge(guard), "CP 剩 1 可再突擊");
        Check(b.GetCommandPoints(0) == 0, "二次突擊扣光 CP");
        Check(!b.GeneralCharge(guard), "CP=0 拒絕");
    }

    // [5] 前置條件：已消滅/潰逃衛隊不可發動
    printf("\n[5] 前置條件\n");
    {
        BattleController b(10, 10, 1.0f);
        Squad* guard = b.CreateSquad("衛隊", 0, Vector2(2, 2), 10);
        guard->SetGeneralGuard(true);
        b.SetCommandPoints(0, 5);
        b.BeginExecution();
        guard->AdjustMorale(-1.0f); // 潰逃
        Check(!b.GeneralRally(guard), "潰逃衛隊不可激勵");
        Check(!b.GeneralCharge(guard), "潰逃衛隊不可突擊");
        Check(b.GetCommandPoints(0) == 5, "拒絕不扣 CP");
    }

    printf("\n=== %d passed, %d failed ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
