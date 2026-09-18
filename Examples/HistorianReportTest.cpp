// HistorianReportTest - N-1 模組化史官戰報無頭測試
// 覆蓋：空記錄報告成立、混合事件分類計數、ELIMINATED 隊別歸屬、
//       結局句分支、省略計數恆等式、nullptr 防護、名冊殉國/得全/斬敵句

#include "Gameplay/HistorianReport.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/Squad.h"

#include <cstdio>
#include <string>

using namespace Potato;
using namespace Potato::Gameplay;

static int g_pass = 0;
static int g_fail = 0;

static void Check(bool ok, const char* name, int value = 0, int expected = 0) {
    if (ok) {
        g_pass++;
        printf("  [PASS] %s\n", name);
    } else {
        g_fail++;
        printf("  [FAIL] %s  (got %d, expected %d)\n", name, value, expected);
    }
}

static bool Contains(const std::string& hay, const char* needle) {
    return hay.find(needle) != std::string::npos;
}

// 手灌記錄的事件流（不走真實模擬）
static BattleRecorder MakeRecorder() {
    BattleRecorder rec;
    rec.AddRecord(0.0f, "前鋒 deployed (team 0)");          // 省略：部署
    rec.AddRecord(0.1f, "前鋒 doctrine assigned (2 rules)");// 省略：部署
    rec.AddRecord(1.0f, "=== Execution phase begins ===");  // 省略：階段
    rec.AddRecord(5.0f, "CP intervention on 前鋒");          // 介入
    rec.AddRecord(8.0f, "掠奪隊 被我軍目擊（接觸偵查）");     // 揭露
    rec.AddRecord(12.0f, "左翼 潰逃！恐慌向友軍擴散");        // 潰逃
    rec.AddRecord(12.5f, "中軍 目睹友軍潰逃，士氣動搖");      // 潰逃（擴散句含「潰逃」）
    rec.AddRecord(20.0f, "蠻兵隊 ELIMINATED");               // 殲滅（敵）
    rec.AddRecord(25.0f, "後衛 ELIMINATED");                 // 殲滅（我）
    rec.AddRecord(30.0f, "某隊 took 12 casualties");         // 省略：傷害
    rec.AddRecord(31.0f, "幽靈隊 ELIMINATED");               // 殲滅（roster 查無→中性）
    return rec;
}

int main() {
    printf("=== Historian Report Tests (N-1) ===\n\n");

    // 名冊需要真 Squad——建 battle 收編，再殲兩隊標記生死
    BattleController battle(10, 10, 1.0f);
    Roster roster;
    Squad* vanguard = battle.CreateSquad("前鋒", 0, Vector2(1, 1), 30);
    Squad* rearGuard = battle.CreateSquad("後衛", 0, Vector2(2, 1), 15);
    Squad* enemyA = battle.CreateSquad("蠻兵隊", 1, Vector2(8, 8), 25);
    Squad* enemyB = battle.CreateSquad("掠奪隊", 1, Vector2(9, 8), 25);
    roster.Enroll(vanguard, "陳鐵", "captain", "斷刃");
    roster.Enroll(rearGuard, "老周");
    roster.Enroll(enemyA, "格洛克");
    roster.Enroll(enemyB, "蠻力");
    rearGuard->ApplyCasualties(999); // 我軍殉國一例
    enemyA->ApplyCasualties(999);    // 斬敵一例
    roster.Update(battle);

    // [1] 空記錄：報告成立、省略 0
    printf("[1] 空記錄報告\n");
    {
        HistorianInput in;
        in.outcome = BattleOutcome::Victory;
        in.elapsedSec = 30.0f;
        HistorianReport r = ComposeHistorianReport(in);
        Check(!r.text.empty(), "空記錄仍產出報告");
        Check(r.omittedCount == 0, "省略計數為 0", r.omittedCount, 0);
        Check(r.narratedCount == 0, "書寫數為 0");
        Check(Contains(r.text, "本報告省略 0 項"), "省略欄位在場");
        Check(Contains(r.text, "我軍克敵"), "勝利結局句");
        Check(Contains(r.text, "斷橋之役"), "戰名入題");
    }

    // [2] 混合事件流分類計數
    printf("\n[2] 事件分類計數\n");
    {
        BattleRecorder rec = MakeRecorder();
        HistorianInput in;
        in.outcome = BattleOutcome::Victory;
        in.recorder = &rec;
        in.roster = &roster;
        HistorianReport r = ComposeHistorianReport(in);
        Check(r.interventionCount == 1, "介入計數", r.interventionCount, 1);
        Check(r.revealCount == 1, "揭露計數", r.revealCount, 1);
        Check(r.routCount == 2, "潰逃計數（含擴散句）", r.routCount, 2);
        Check(r.enemyLosses == 1, "斬敵計數", r.enemyLosses, 1);
        Check(r.playerLosses == 1, "我軍覆計數", r.playerLosses, 1);
        Check(r.neutralLosses == 1, "中性殲滅", r.neutralLosses, 1);
        // narrated = 1+1+2+3 = 7；total = 11 → omitted = 4
        Check(r.narratedCount == 7, "書寫總數", r.narratedCount, 7);
        Check(r.omittedCount == 4, "省略 = 總數 - 書寫", r.omittedCount, 4);
        Check(Contains(r.text, "本報告省略 4 項"), "省略數入文");
        Check(Contains(r.text, "強令介入 1 次"), "介入句");
        Check(Contains(r.text, "潰逃 2 起"), "潰逃句");
        Check(Contains(r.text, "斬敵 1 隊"), "斬敵句");
        Check(Contains(r.text, "我軍覆 1 隊"), "我軍覆句");
    }

    // [3] 結局句分支
    printf("\n[3] 結局句\n");
    {
        HistorianInput in;
        in.outcome = BattleOutcome::Defeat;
        Check(Contains(ComposeHistorianReport(in).text, "我軍失利"),
              "敗北句");
        in.outcome = BattleOutcome::Draw;
        Check(Contains(ComposeHistorianReport(in).text, "鳴金收兵"),
              "平手句");
        in.outcome = BattleOutcome::Ongoing;
        Check(Contains(ComposeHistorianReport(in).text, "戰事未決"),
              "未決句");
    }

    // [4] nullptr 防護 + 名冊句
    printf("\n[4] nullptr 與名冊\n");
    {
        HistorianInput in;
        in.roster = nullptr; // 無名冊
        HistorianReport r = ComposeHistorianReport(in);
        Check(!Contains(r.text, "殉國") && !Contains(r.text, "得全"),
              "無名冊無名冊句");

        in.roster = &roster;
        r = ComposeHistorianReport(in);
        Check(Contains(r.text, "「老周」隊長殉國"), "殉國句");
        Check(Contains(r.text, "「陳鐵」得全"), "得全句");
        Check(Contains(r.text, "斬敵「格洛克」"), "斬敵句");
        Check(Contains(r.text, "「蠻力」得全") == false, "敵軍生者不入得全");
    }

    printf("\n=== %d PASS / %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
