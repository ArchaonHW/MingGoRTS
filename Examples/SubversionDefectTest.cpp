// SubversionDefectTest — E-3 顛覆內應（headless）
// 覆蓋：倒戈翻面/選取排序/數量上限/doctrine 兜底/全倒戈自動勝利
// /零敵隊邊界/情報消費與 Gather 排除。

#include "Campaign/NoBattleResolver.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/EnemyGeneral.h"
#include "Gameplay/GeneralDossier.h"
#include "Gameplay/Ledger.h"
#include "Gameplay/SubversionDefect.h"
#include "Gameplay/Squad.h"

#include <cstdio>

using namespace Potato::Campaign;
using namespace Potato::Gameplay;
using Potato::Vector2;

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

    // ---- [1] 部分倒戈：士氣最弱者先被策反 ----
    printf("\n[1] 選取排序\n");
    {
        BattleController battle(16, 16, 1.0f);
        Squad* mine = battle.CreateSquad("我軍", 0, Vector2(4, 4), 30);
        Squad* e1 = battle.CreateSquad("頑抗隊", 1, Vector2(12, 12), 30);
        Squad* e2 = battle.CreateSquad("動搖隊", 1, Vector2(9, 12), 30);
        Squad* e3 = battle.CreateSquad("猶豫隊", 1, Vector2(15, 12), 30);
        // 士氣：e1=0.9、e2=0.3、e3=0.5 → 倒戈序 e2→e3→e1
        e1->AdjustMorale(-0.10f);
        e2->AdjustMorale(-0.70f);
        e3->AdjustMorale(-0.50f);

        const int flipped = ApplyDefection(battle, 1, 0, 2);
        Check(flipped == 2, "倒戈數 = min(2,敵存活3)");
        Check(e2->GetTeam() == 0 && e3->GetTeam() == 0,
              "士氣最弱兩隊倒戈");
        Check(e1->GetTeam() == 1, "士氣最高者仍敵對");
        Check(mine->GetTeam() == 0, "我軍不受影響");
        Check(e2->GetOrder() == SquadOrder::Hold &&
                  e3->GetOrder() == SquadOrder::Hold,
              "倒戈隊先按兵");
        Check(battle.GetDoctrine(e2) != nullptr &&
                  battle.GetDoctrine(e3) != nullptr,
              "倒戈隊補編兜底 doctrine");
    }

    // ---- [2] 全數倒戈：首 tick 自動 Victory（不戰而勝湧現）----
    printf("\n[2] 全倒戈\n");
    {
        BattleController battle(16, 16, 1.0f);
        battle.CreateSquad("我軍", 0, Vector2(4, 4), 30);
        battle.CreateSquad("敵甲", 1, Vector2(12, 12), 30);
        battle.CreateSquad("敵乙", 1, Vector2(9, 12), 25);

        const int flipped = ApplyDefection(battle, 1, 0, 5);
        Check(flipped == 2, "倒戈數上限=敵存活數");
        battle.BeginExecution();
        battle.Update(0.1f); // 觸發 CheckOutcome
        Check(battle.GetOutcome() == BattleOutcome::Victory,
              "全倒戈首 tick Victory");
    }

    // ---- [3] 邊界：零敵隊 / count=0 / 潰逃隊不倒 ----
    printf("\n[3] 邊界\n");
    {
        BattleController battle(16, 16, 1.0f);
        battle.CreateSquad("我軍", 0, Vector2(4, 4), 30);
        Check(ApplyDefection(battle, 1, 0, 3) == 0, "零敵隊回 0");

        Squad* e = battle.CreateSquad("敵隊", 1, Vector2(8, 8), 20);
        Check(ApplyDefection(battle, 1, 0, 0) == 0 && e->GetTeam() == 1,
              "count=0 不倒戈");

        // 潰逃隊不算倒戈對象（已失去組織）
        Squad* rout = battle.CreateSquad("潰隊", 1, Vector2(8, 9), 20);
        rout->AdjustMorale(-1.0f); // 歸零→潰逃
        Check(rout->IsRouting(), "前置：潰逃");
        Check(ApplyDefection(battle, 1, 0, 5) == 1 &&
                  e->GetTeam() == 0 && rout->GetTeam() == 1,
              "潰逃隊不倒戈、存活隊照倒");
    }

    // ---- [4] 情報消費：verified 判詞兌換後不再計入 ----
    printf("\n[4] 情報消費\n");
    {
        LedgerChain chain;
        GeneralDossier dossier;
        EnemyGeneral g = EnemyGeneral::MakeGlock();
        dossier.Hear(g);
        dossier.Verify(g);
        Check(NoBattleResolver::Gather(chain, dossier, g.GetName())
                      .verifiedIntel == 1,
              "verified 判詞計入");

        Check(dossier.ConsumeVerified(g.GetName()) == 1,
              "消費回傳條數");
        Check(dossier.ConsumeVerified(g.GetName()) == 0,
              "重複消費冪等回 0");
        Check(NoBattleResolver::Gather(chain, dossier, g.GetName())
                      .verifiedIntel == 0,
              "消耗後 Gather 不計入");
        Check(dossier.Find(g.GetName()) != nullptr &&
                  dossier.Find(g.GetName())->verified,
              "檔案仍在且仍是真相（消耗≠刪除）");
    }

    // ---- [5] 未驗證傳聞不可消費 ----
    printf("\n[5] 不可消費\n");
    {
        GeneralDossier dossier;
        EnemyGeneral g = EnemyGeneral::MakeGlock();
        dossier.Hear(g); // 未驗證
        Check(dossier.ConsumeVerified(g.GetName()) == 0,
              "未驗證傳聞不消耗");
        Check(dossier.ConsumeVerified("查無此人") == 0,
              "無檔案回 0");
    }

    printf("=== %s ===\n",
           failures == 0 ? "ALL PASS" : "FAILURES");
    return failures == 0 ? 0 : 1;
}
