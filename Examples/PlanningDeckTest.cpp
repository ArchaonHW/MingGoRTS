// PlanningDeckTest - T-9 回合層牌組模型的 headless 驗證
//
// 覆蓋：Init 回讀已指派 doctrine、卡槽上限、增刪改/排序交換、
// Validate 警告、AI 參謀模板載入（rationale）、Commit 寫回

#include "Gameplay/PlanningDeck.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/BattlePlanner.h"
#include "Gameplay/Squad.h"

#include <cstdio>
#include <string>

using namespace Potato;
using namespace Potato::Gameplay;

static int g_failures = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", msg); ++g_failures; } \
    else { printf("  ok: %s\n", msg); } \
} while (0)

static bool HasWarningContaining(const std::vector<std::string>& ws,
                                 const char* sub) {
    for (const auto& w : ws) {
        if (w.find(sub) != std::string::npos) return true;
    }
    return false;
}

int main() {
    printf("=== PlanningDeckTest ===\n");

    BattleController battle(20, 15, 1.0f);
    Squad* alpha = battle.CreateSquad("Alpha", 0, Vector2(5.0f, 5.0f), 30);
    Squad* bravo = battle.CreateSquad("Bravo", 0, Vector2(8.0f, 5.0f), 20);
    battle.CreateSquad("Enemy", 1, Vector2(10.0f, 12.0f), 25);
    battle.SetObjective(0, Vector2(10.0f, 13.0f));
    battle.SetRallyPoint(0, Vector2(5.0f, 2.0f));

    // [1] Init：只收我方小隊；未指派 doctrine → 空卡組
    PlanningDeck deck;
    deck.Init(battle, 0);
    CHECK(deck.SquadCount() == 2, "Init 收 2 支我方小隊");
    CHECK(deck.Deck(0).rules.empty() && deck.Deck(1).rules.empty(),
          "未指派 doctrine → 空卡組");

    // [2] 已指派 doctrine 回讀
    {
        DoctrineSet ds;
        ds.AddRule({DoctrineTrigger::HealthBelow,
                    DoctrineAction::RetreatToRally, 0.3f, 5});
        ds.AddRule({DoctrineTrigger::Always,
                    DoctrineAction::AttackNearest, 0.0f, 90});
        battle.AssignDoctrine(alpha, ds);
        PlanningDeck d2;
        d2.Init(battle, 0);
        const int ai = d2.FindDeck(alpha);
        CHECK(ai >= 0, "FindDeck 找到 Alpha");
        CHECK(d2.Deck(ai).rules.size() == 2, "回讀 2 條規則");
        CHECK(d2.Deck(ai).rules[0].trigger == DoctrineTrigger::HealthBelow,
              "回讀依 priority 排序");
    }

    // [3] AddRule/RemoveRule/SetRule + 槽位上限
    PlanningDeck edit;
    edit.Init(battle, 0);
    const int di = edit.FindDeck(bravo);
    CHECK(di >= 0, "FindDeck 找到 Bravo");
    edit.SetSlotCap(3);
    CHECK(edit.AddRule(di, {DoctrineTrigger::Always,
                            DoctrineAction::HoldPosition, 0.0f, 90}),
          "AddRule 成功");
    CHECK(edit.AddRule(di, {DoctrineTrigger::HealthBelow,
                            DoctrineAction::RetreatToRally, 0.3f, 5}),
          "第二條成功");
    CHECK(edit.AddRule(di, {DoctrineTrigger::EnemyInRange,
                            DoctrineAction::AttackNearest, 3.0f, 10}),
          "第三條成功");
    CHECK(!edit.AddRule(di, {DoctrineTrigger::Always,
                             DoctrineAction::HoldPosition, 0.0f, 99}),
          "滿槽 AddRule 失敗");
    CHECK(edit.Deck(di).rules[0].priority == 5, "規則依 priority 升序");

    CHECK(edit.SetRule(di, 0, {DoctrineTrigger::MoraleBelow,
                               DoctrineAction::HoldPosition, 0.5f, 80}),
          "SetRule 成功");
    // 原 [5,10,90] → 改首條為 prio 80 → 重排為 [10,80,90]
    CHECK(edit.Deck(di).rules[1].priority == 80,
          "SetRule 後依 priority 重排");

    // SwapRules 交換 priority
    CHECK(edit.SwapRules(di, 0, 1), "SwapRules 成功");
    // swap 後規則重排；首條應仍是 priority 最小者
    CHECK(edit.Deck(di).rules[0].priority <=
              edit.Deck(di).rules[1].priority,
          "SwapRules 後維持升序");
    CHECK(!edit.SwapRules(di, 0, 9), "越界 SwapRules 失敗");

    CHECK(edit.RemoveRule(di, 0), "RemoveRule 成功");
    CHECK(edit.Deck(di).rules.size() == 2, "刪除後剩 2 條");
    CHECK(!edit.RemoveRule(di, 5), "越界 RemoveRule 失敗");
    CHECK(!edit.RemoveRule(9, 0), "越界 deck RemoveRule 失敗");

    // [4] Validate：把 Alpha 的 Always 墊底刪掉 → 應產生警告
    CHECK(edit.RemoveRule(edit.FindDeck(alpha), 1),
          "刪除 Alpha 的 Always 墊底");
    auto warns = edit.Validate();
    CHECK(HasWarningContaining(warns, "Always"), "偵測到無 Always 墊底警告");
    CHECK(!HasWarningContaining(warns, "Enemy"),
          "敵隊不進 Validate");

    // [5] Commit 寫回
    edit.Commit(battle);
    const DoctrineSet* committed = battle.GetDoctrine(bravo);
    CHECK(committed && committed->Count() == 2, "Commit 寫回 2 條規則");
    CHECK(committed->Rules()[0].priority <=
              committed->Rules()[1].priority,
          "Commit 後維持排序");

    // [6] AI 參謀模板：一鍵載入 + rationale
    PlanningDeck tpl;
    tpl.Init(battle, 0);
    BattlePlanner planner;
    tpl.LoadPlannerTemplate(planner, battle, 0);
    bool allNonEmpty = true;
    bool anyRationale = false;
    for (int i = 0; i < tpl.SquadCount(); ++i) {
        if (tpl.Deck(i).rules.empty()) allNonEmpty = false;
        if (!tpl.Rationale(tpl.Deck(i).squad).empty()) anyRationale = true;
    }
    CHECK(allNonEmpty, "模板為每隊產生 doctrine");
    CHECK(anyRationale, "模板保留 rationale");
    CHECK(tpl.SuggestedCP() >= 0, "模板帶 CP 建議");
    CHECK(tpl.Validate().empty(), "模板產出無警告（含 Always 墊底）");

    // 模板 Commit 後能開戰
    tpl.Commit(battle);
    CHECK(battle.BeginExecution(), "模板 Commit 後可 BeginExecution");

    if (g_failures == 0) {
        printf("PASS\n");
        return 0;
    }
    printf("FAILED: %d\n", g_failures);
    return 1;
}
