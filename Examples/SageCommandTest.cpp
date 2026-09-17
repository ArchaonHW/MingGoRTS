// SageCommandTest - 至聖者指揮學 × 治平者管理學 headless 驗證
// 覆蓋 spec I/O matrix 全行：
//   正策生效 / 逆策累墮 / 土階段封邪 / 墮落徵象(60/80/100) /
//   至聖者無戰 / 治平者無勝 / 五行輪轉(金水減半) / 兵權執行(Deployment→Execution)

#include "Gameplay/SageCommand.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/BattlePlanner.h"
#include "Gameplay/Squad.h"

#include <cstdio>
#include <cmath>
#include <string>
#include <vector>

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
        printf("  [FAIL] %s  (got %.4f, expected %.4f)\n", name, value, expected);
    }
}

// 標準戰場：我方兩隊 vs 敵方兩隊（各 20 人）
static void BuildBattle(BattleController& b) {
    b.SetObjective(0, Vector2(18.0f, 7.0f));
    b.SetObjective(1, Vector2(1.0f, 7.0f));
    b.SetRallyPoint(0, Vector2(1.0f, 7.0f));
    b.SetRallyPoint(1, Vector2(18.0f, 7.0f));
    b.CreateSquad("我方甲隊", 0, Vector2(2.0f, 4.0f), 30);
    b.CreateSquad("我方乙隊", 0, Vector2(2.0f, 10.0f), 30);
    b.CreateSquad("敵方甲隊", 1, Vector2(16.0f, 4.0f), 20);
    b.CreateSquad("敵方乙隊", 1, Vector2(16.0f, 10.0f), 20);
}

static Squad* FirstSquadOf(BattleController& b, int team) {
    for (const auto& s : b.GetSquads()) {
        if (s->GetTeam() == team) {
            return s.get();
        }
    }
    return nullptr;
}

int main() {
    printf("=== Sage Command Tests ===\n\n");

    // ---------------------------------------------------------------
    printf("[1] 正策生效：任一階段可用、上升治理值、不累墮；非法階段拒絕\n");
    {
        BattleController battle(20, 15, 1.0f);
        BuildBattle(battle);
        SageCommand sage;

        Squad* ally = FirstSquadOf(battle, 0);
        ally->AdjustMorale(-0.5f); // 士氣壓到 0.5 以觀察回升

        Check(sage.ApplyPolicy(Policy::EstablishPeople, battle),
              "立人回傳 true");
        Check(ally->GetMorale() > 0.6f, "我軍士氣上升",
              ally->GetMorale(), 0.65f);
        Check(sage.GetCorruption() == 0.0f, "正策不累墮落",
              sage.GetCorruption(), 0.0f);

        Check(sage.ApplyPolicy(Policy::EstablishState, battle),
              "立國回傳 true");
        Check(sage.GetPopularSupport() >= 60.0f, "民心 50→60",
              sage.GetPopularSupport(), 60.0f);

        Check(sage.ApplyPolicy(Policy::EstablishOrder, battle),
              "立制回傳 true");
        Check(sage.GetCivilOrder() >= 60.0f, "秩序 50→60",
              sage.GetCivilOrder(), 60.0f);

        // 非法階段：把戰鬥推到 Resolution 後政策一律拒絕
        battle.BeginExecution();
        for (const auto& s : battle.GetSquads()) {
            if (s->GetTeam() == 1) {
                s->ApplyCasualties(999);
            }
        }
        battle.Update(0.1f);
        Check(battle.GetPhase() == BattlePhase::Resolution,
              "戰鬥進入 Resolution");
        Check(!sage.ApplyPolicy(Policy::EstablishOrder, battle),
              "Resolution 階段政策回傳 false");
    }

    // ---------------------------------------------------------------
    printf("\n[2] 逆策生效+累墮：木火折扣、敵方受效果、墮落滿後拒絕\n");
    {
        BattleController battle(20, 15, 1.0f);
        BuildBattle(battle);
        SageCommand sage;

        Squad* enemy = FirstSquadOf(battle, 1);
        Check(sage.ApplyPolicy(Policy::SlanderEnemy, battle),
              "讒敵回傳 true");
        Check(enemy->GetMorale() < 1.0f, "敵隊士氣下降",
              enemy->GetMorale(), 0.85f);
        // 木火期折扣：基礎 8 × 0.5 = 4
        Check(std::fabs(sage.GetCorruption() - 4.0f) < 0.01f,
              "木火期墮落折扣 +4", sage.GetCorruption(), 4.0f);

        // 連續施逆策（欺天 25×0.5=12.5）直到墮落滿；滿後必須拒絕
        int applied = 0;
        while (sage.ApplyPolicy(Policy::DeceiveHeaven, battle)) {
            ++applied;
            if (applied > 20) {
                break;
            }
        }
        Check(sage.GetCorruption() >= 100.0f, "墮落達 100",
              sage.GetCorruption(), 100.0f);
        Check(sage.GetOutcome() == SageOutcome::Fallen,
              "墮落滿 → outcome=Fallen");
        Check(!sage.ApplyPolicy(Policy::Terrorize, battle),
              "墮落滿後逆策回傳 false");
    }

    // ---------------------------------------------------------------
    printf("\n[3] 土階段封邪：逆策鎖定、墮落衰減、秩序回升；非土拒絕\n");
    {
        BattleController battle(20, 15, 1.0f);
        BuildBattle(battle);
        SageCommand sage;

        Check(!sage.SealHeresy(), "木火期封邪回傳 false");
        sage.AdvancePhase(); // 木火 → 土
        Check(sage.GetWuXingPhase() == WuXingPhase::Earth,
              "進入土階段");
        Check(sage.SealHeresy(), "土階段封邪回傳 true");
        Check(sage.IsHereticSealed(), "逆策已鎖定");

        sage.AddCorruption(50.0f); // 低於 60，不觸發徵象
        float c0 = sage.GetCorruption();
        float o0 = sage.GetCivilOrder();
        for (int i = 0; i < 20; ++i) {
            sage.Tick(0.1f, battle); // 2 秒
        }
        Check(sage.GetCorruption() < c0, "封邪中墮落衰減",
              sage.GetCorruption(), c0);
        Check(sage.GetCivilOrder() > o0, "封邪中秩序回升",
              sage.GetCivilOrder(), o0);
        Check(!sage.ApplyPolicy(Policy::SlanderEnemy, battle),
              "封印中逆策回傳 false");
        Check(sage.ApplyPolicy(Policy::EstablishOrder, battle),
              "封印中正策仍可用");

        // 離開土階段 → 封印解除
        sage.AdvancePhase();
        Check(!sage.IsHereticSealed(), "離開土階段封印解除");
    }

    // ---------------------------------------------------------------
    printf("\n[4] 墮落徵象：60/80/100 門檻各觸發一次，100 → 失格\n");
    {
        BattleController battle(20, 15, 1.0f);
        BuildBattle(battle);
        SageCommand sage;

        int signEvents = 0;
        sage.SetEventCallback([&signEvents](const std::string& msg) {
            if (msg.find("徵象") != std::string::npos) {
                ++signEvents;
            }
        });

        sage.AddCorruption(60.0f);
        Check(sage.GetSignLevel() == 1, "墮落 60 → 徵象一",
              static_cast<float>(sage.GetSignLevel()), 1.0f);

        // 封邪衰減到 60 以下，再衝上去——徵象一不得重複觸發
        sage.AdvancePhase(); // → 土
        sage.SealHeresy();
        for (int i = 0; i < 100; ++i) {
            sage.Tick(0.1f, battle); // 衰減至 0
        }
        Check(sage.GetCorruption() < 60.0f, "衰減至門檻以下",
              sage.GetCorruption(), 0.0f);

        sage.AddCorruption(75.0f); // 再越 60 與 80 之間
        Check(sage.GetSignLevel() == 1, "徵象一不重複觸發",
              static_cast<float>(sage.GetSignLevel()), 1.0f);

        sage.AddCorruption(10.0f); // 85 → 徵象二
        Check(sage.GetSignLevel() == 2, "墮落 80 → 徵象二",
              static_cast<float>(sage.GetSignLevel()), 2.0f);

        sage.AddCorruption(50.0f); // 越 100 → 徵象三 + 失格
        Check(sage.GetSignLevel() == 3, "墮落 100 → 徵象三",
              static_cast<float>(sage.GetSignLevel()), 3.0f);
        Check(sage.GetOutcome() == SageOutcome::Fallen,
              "墮落 100 → outcome=Fallen");
        Check(signEvents == 3, "三徵象各只觸發一次",
              static_cast<float>(signEvents), 3.0f);
        Check(!sage.ApplyPolicy(Policy::EstablishOrder, battle),
              "失格後所有政策拒絕");
        Check(sage.ResolveOutcome(battle) == SageOutcome::Fallen,
              "失格優先於戰場勝負");
    }

    // ---------------------------------------------------------------
    printf("\n[5] 五行輪轉：木火→土→金水→木火；金水期策效減半\n");
    {
        BattleController battle(20, 15, 1.0f);
        BuildBattle(battle);
        SageCommand sage;

        Check(sage.GetWuXingPhase() == WuXingPhase::WoodFire, "初始木火");
        sage.AdvancePhase();
        Check(sage.GetWuXingPhase() == WuXingPhase::Earth, "木火→土");
        sage.AdvancePhase();
        Check(sage.GetWuXingPhase() == WuXingPhase::MetalWater, "土→金水");
        sage.AdvancePhase();
        Check(sage.GetWuXingPhase() == WuXingPhase::WoodFire, "金水→木火");

        // 金水期效果減半：立制 +10 → +5
        SageCommand sageMW;
        sageMW.AdvancePhase();
        sageMW.AdvancePhase(); // 木火→土→金水
        sageMW.ApplyPolicy(Policy::EstablishOrder, battle);
        Check(std::fabs(sageMW.GetCivilOrder() - 55.0f) < 0.01f,
              "金水期立制 +5（減半）", sageMW.GetCivilOrder(), 55.0f);

        SageCommand sageWF; // 木火期對照
        sageWF.ApplyPolicy(Policy::EstablishOrder, battle);
        Check(std::fabs(sageWF.GetCivilOrder() - 60.0f) < 0.01f,
              "木火期立制 +10（全額）", sageWF.GetCivilOrder(), 60.0f);
    }

    // ---------------------------------------------------------------
    printf("\n[6] 兵權執行：Deployment 產 doctrine → BeginExecution → 即時層\n");
    {
        BattleController battle(20, 15, 1.0f);
        BuildBattle(battle);
        SageCommand sage;

        BattlePlanner planner;
        for (int team = 0; team <= 1; ++team) {
            BattlePlanner::Plan plan = planner.GeneratePlan(battle, team);
            planner.ApplyPlan(battle, plan);
        }
        Check(battle.GetCommandPoints(0) > 0, "規劃器配發 CP",
              static_cast<float>(battle.GetCommandPoints(0)), 1.0f);

        Check(battle.BeginExecution(), "Deployment → Execution");
        Check(battle.GetPhase() == BattlePhase::Execution, "進入即時層");
        Check(!battle.BeginExecution(), "非 Deployment 呼叫回傳 false");

        // 指揮 = 切換權：即時層中策權仍可切換
        Check(sage.ApplyPolicy(Policy::RallyTroops, battle),
              "即時層中策權可切換（整軍）");

        // 兵權：CP 介入可用
        Squad* ally = FirstSquadOf(battle, 0);
        Check(battle.Intervene(ally, SquadOrder::Hold,
                               ally->GetPosition(), 2.0f),
              "CP 介入成功");
    }

    // ---------------------------------------------------------------
    printf("\n[7] 無勝而勝：潰逃/歸附 > 殲滅 且 民心 ≥60 → SubdueWithoutWar\n");
    {
        BattleController battle(20, 15, 1.0f);
        BuildBattle(battle);
        SageCommand sage;

        sage.ApplyPolicy(Policy::EstablishState, battle); // 民心 60
        Check(sage.GetPopularSupport() >= 60.0f, "民心達標");

        battle.BeginExecution();

        // 兩敵隊各失 4 人後士氣歸零 → 潰逃（歸附 32 > 殲滅 8）
        for (const auto& s : battle.GetSquads()) {
            if (s->GetTeam() == 1) {
                s->ApplyCasualties(4);
                s->AdjustMorale(-1.0f);
            }
        }
        battle.Update(0.1f);
        Check(battle.GetOutcome() == BattleOutcome::Victory,
              "敵軍全潰 → 戰場勝利");
        sage.Tick(0.1f, battle);
        Check(sage.GetSubduedCount() > sage.GetAnnihilatedCount(),
              "歸附數 > 殲滅數",
              static_cast<float>(sage.GetSubduedCount()),
              static_cast<float>(sage.GetAnnihilatedCount()));
        Check(sage.ResolveOutcome(battle) == SageOutcome::SubdueWithoutWar,
              "結算 = 無勝而勝");
        Check(sage.GetOutcome() == SageOutcome::SubdueWithoutWar,
              "Tick 同步勝利型態");
    }

    // ---------------------------------------------------------------
    printf("\n[8] 對照：全殲取勝 → 普通 Victory（非無勝而勝）\n");
    {
        BattleController battle(20, 15, 1.0f);
        BuildBattle(battle);
        SageCommand sage;

        sage.ApplyPolicy(Policy::EstablishState, battle); // 民心 60
        battle.BeginExecution();
        for (const auto& s : battle.GetSquads()) {
            if (s->GetTeam() == 1) {
                s->ApplyCasualties(999); // 全殲，不留潰逃
            }
        }
        battle.Update(0.1f);
        Check(battle.GetOutcome() == BattleOutcome::Victory,
              "全殲 → 戰場勝利");
        Check(sage.ResolveOutcome(battle) == SageOutcome::Victory,
              "全殲取勝 = 普通 Victory");
    }

    // ---------------------------------------------------------------
    printf("\n[9] 治平者無勝：戰敗/平手 但 民心≥70 且 秩序≥70 → GovernedPeace\n");
    {
        BattleController battle(20, 15, 1.0f);
        BuildBattle(battle);
        SageCommand sage;

        // 治理達標：民心 50→70（立國×2）、秩序 50→70（立制×2）
        sage.ApplyPolicy(Policy::EstablishState, battle);
        sage.ApplyPolicy(Policy::EstablishState, battle);
        sage.ApplyPolicy(Policy::EstablishOrder, battle);
        sage.ApplyPolicy(Policy::EstablishOrder, battle);
        Check(sage.GetPopularSupport() >= 70.0f, "民心 ≥70");
        Check(sage.GetCivilOrder() >= 70.0f, "秩序 ≥70");

        // 我軍全滅 → 戰場敗北
        battle.BeginExecution();
        for (const auto& s : battle.GetSquads()) {
            if (s->GetTeam() == 0) {
                s->ApplyCasualties(999);
            }
        }
        battle.Update(0.1f);
        Check(battle.GetOutcome() == BattleOutcome::Defeat,
              "我軍全滅 → 戰場敗北");
        Check(sage.ResolveOutcome(battle) == SageOutcome::GovernedPeace,
              "兵敗而治成 = 治平者無勝");

        // 對照：同樣戰敗但治理未達標 → 真敗北
        BattleController battle2(20, 15, 1.0f);
        BuildBattle(battle2);
        SageCommand sage2;
        sage2.ApplyPolicy(Policy::EstablishState, battle2); // 民心 60、秩序 50
        battle2.BeginExecution();
        for (const auto& s : battle2.GetSquads()) {
            if (s->GetTeam() == 0) {
                s->ApplyCasualties(999);
            }
        }
        battle2.Update(0.1f);
        Check(battle2.GetOutcome() == BattleOutcome::Defeat,
              "對照組同樣戰場敗北");
        Check(sage2.ResolveOutcome(battle2) == SageOutcome::Defeat,
              "秩序 50 < 70 → 普通敗北");

        // 墮落滿值時連 GovernedPeace 也救不回（失格優先）
        SageCommand sage3;
        sage3.AddCorruption(100.0f);
        Check(sage3.ResolveOutcome(battle) == SageOutcome::Fallen,
              "失格優先於治平無勝");
    }

    printf("\n=== 結果: %d PASS, %d FAIL ===\n", g_pass, g_fail);
    printf(g_fail == 0 ? "ALL CHECKS PASSED\n" : "CHECKS FAILED\n");
    return g_fail == 0 ? 0 : 1;
}
