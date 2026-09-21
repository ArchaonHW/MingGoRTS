// MythIncursionTest - D-4 神話入侵事件無頭測試
// 驗收：達閾觸發、稀有性 one-shot、鬼軍/假訊兩種生成、
//       安撫/忽視/逾時三結局、具名三欄、ChapterDef 旗標解析
#include "Campaign/ChapterLibrary.h"
#include "Campaign/MythLayer.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/MythIncursion.h"
#include "Gameplay/QuantumFog.h"
#include "Gameplay/Squad.h"

#include <cstdio>
#include <string>
#include <vector>

using namespace Potato::Campaign;
using namespace Potato::Gameplay;
using Potato::Vector2;

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* name) {
    if (ok) { g_pass++; printf("  [PASS] %s\n", name); }
    else    { g_fail++; printf("  [FAIL] %s\n", name); }
}

static int SquadCount(const BattleController& b) {
    int n = 0;
    for (const auto& s : b.GetSquads()) (void)s, ++n;
    return n;
}

int main() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    printf("=== MythIncursion Tests (D-4) ===\n\n");

    const Vector2 spawn{5.0f, 5.0f};

    // ---- [1] 達閾觸發 + 具名三欄 + 稀有性 ----
    printf("[1] 觸發與稀有性\n");
    {
        BattleController battle(20, 20, 1.0f);
        MythIncursion inc;
        std::vector<IncursionEvent> incs;
        std::vector<OutcomeEvent> outs;
        std::vector<std::string> strs;
        inc.SetIncursionCallback(
            [&](const IncursionEvent& e) { incs.push_back(e); });
        inc.SetOutcomeCallback(
            [&](const OutcomeEvent& e) { outs.push_back(e); });
        inc.SetEventCallback(
            [&](const std::string& s) { strs.push_back(s); });

        // 未 Arm：達閾也不觸發
        inc.Update(0.016f, battle, 3, "斷橋", "橋姬");
        Check(!inc.GetIncursion().fired, "未 Arm 不觸發");

        inc.Arm(2, IncursionKind::GhostLegion, spawn);
        // 未達閾
        inc.Update(0.016f, battle, 1, "斷橋", "橋姬");
        Check(!inc.GetIncursion().fired, "未達閾不觸發");
        // 達閾觸發
        inc.Update(0.016f, battle, 2, "斷橋", "橋姬");
        Check(inc.GetIncursion().fired, "達閾觸發");
        Check(inc.GetIncursion().pending, "進入待解決");
        Check(incs.size() == 1, "IncursionEvent 一筆");
        Check(strs.size() == 1, "入帳字串一筆");
        if (incs.size() == 1) {
            const IncursionEvent& e = incs.at(0);
            Check(e.shrine == "斷橋" && e.spirit == "橋姬" &&
                      !e.when.empty() && !e.detail.empty(),
                  "具名三欄齊全");
            Check(e.kind == IncursionKind::GhostLegion,
                  "kind=鬼軍夜行");
        }
        Check(SquadCount(battle) == 1, "鬼隊已生成");
        if (SquadCount(battle) == 1) {
            Check(battle.GetSquads().at(0)->GetTeam() == 2,
                  "鬼軍 team=2 第三方");
        }
        // 稀有性：再達閾不重複
        inc.Update(0.016f, battle, 3, "斷橋", "橋姬");
        Check(incs.size() == 1, "one-shot 不重複觸發");
        // 解決：安撫
        Check(inc.Resolve(true), "安撫解決");
        Check(outs.size() == 1 && outs.at(0).pacified,
              "pacified 結局一筆");
        Check(inc.GetIncursion().resolved, "已結算");
        Check(battle.GetSquads().at(0)->IsEliminated(),
              "安撫後鬼隊消散");
        Check(!inc.Resolve(true), "重複 Resolve 拒絕");
        Check(strs.size() == 2, "結算入帳字串第二筆");
        Check(strs.at(1).find("安撫") != std::string::npos,
              "結算字串含安撫");
    }

    // ---- [2] 逾時=忽視 ----
    printf("[2] 逾時忽視\n");
    {
        BattleController battle(20, 20, 1.0f);
        MythIncursion inc;
        inc.Arm(1, IncursionKind::GhostLegion, spawn);
        OutcomeEvent out{};
        int outN = 0;
        inc.SetOutcomeCallback(
            [&](const OutcomeEvent& e) { out = e; ++outN; });
        inc.Update(0.016f, battle, 1, "斷橋", "橋姬");
        Check(inc.GetIncursion().pending, "已觸發待解決");
        // 累積超過窗口
        for (int i = 0; i < 2000; ++i) {
            inc.Update(0.016f, battle, 1, "斷橋", "橋姬");
        }
        Check(outN == 1 && !out.pacified, "逾時自動 ignored");
        Check(inc.GetIncursion().resolved, "逾時已結算");
        Check(battle.GetSquads().at(0)->GetMembers() > 0,
              "忽視鬼隊留存");
        Check(!inc.Resolve(true), "結算後 Resolve 拒絕");
        Check(out.spirit == "橋姬", "結局回傳原始靈名");
    }

    // ---- [3] 狐仙假訊 ----
    printf("[3] 狐仙假訊\n");
    {
        BattleController battle(20, 20, 1.0f);
        BattleResources res;
        res.Setup(battle, 0, /*intel=*/3, /*cp=*/0);
        QuantumFog fog;
        fog.BindResources(&res);
        battle.BindFog(&fog);
        MythIncursion inc;
        inc.Arm(2, IncursionKind::FoxRumor, spawn);
        inc.Update(0.016f, battle, 2, "斷橋", "狐仙");
        Check(inc.GetIncursion().fired, "假訊觸發");
        Check(battle.GetSquads().at(0)->GetName().find("幻影") !=
                  std::string::npos,
              "幻影隊命名");
        const int eid =
            battle.GetFogEntityId(battle.GetSquads().at(0).get());
        Check(eid >= 0, "幻影隊已綁 fog entity");
        // 假雲可被玩家觀測（扣 team 0 情報，不扣敵方）
        Check(fog.Observe(eid, spawn), "假雲可觀測（觀測方=0）");
        // 忽視：幻影消散——情報層造假不留實體卡勝利判定
        Check(inc.Resolve(false), "主動忽視");
        Check(inc.GetIncursion().resolved &&
                  !inc.GetIncursion().pacified,
              "ignored 結算");
        Check(battle.GetSquads().at(0)->IsEliminated(),
              "忽視後幻影消散");
    }

    // ---- [4] MythLayer::SpiritOf ----
    printf("[4] SpiritOf\n");
    {
        MythLayer myth;
        myth.BindSpirit("斷橋", "橋姬");
        Check(myth.SpiritOf("斷橋") == "橋姬", "綁定可查");
        Check(myth.SpiritOf("他處").empty(), "未綁回空");
    }

    // ---- [5] ChapterDef incursion 旗標 ----
    printf("[5] 章節旗標\n");
    {
        const char* good = R"({
            "schema":"potato.campaign_chapter/1","id":"c1",
            "name":"章一","arc":1,"chapter":1,"map":"m.json",
            "incursion":{"seepage":2,"kind":"ghost_legion"}})";
        ChapterDef d;
        Check(d.LoadFromString(good), "合法 incursion 載入");
        Check(d.incursion.enabled && d.incursion.seepage == 2 &&
                  d.incursion.kind == "ghost_legion",
              "旗標解析正確");

        const char* missing = R"({
            "schema":"potato.campaign_chapter/1","id":"c2",
            "name":"章二","arc":1,"chapter":2,"map":"m.json"})";
        ChapterDef d2;
        Check(d2.LoadFromString(missing), "缺欄章節載入");
        Check(!d2.incursion.enabled, "缺欄=關閉");

        const char* bad = R"({
            "schema":"potato.campaign_chapter/1","id":"c3",
            "name":"章三","arc":1,"chapter":3,"map":"m.json",
            "incursion":{"seepage":9,"kind":"bogus"}})";
        ChapterDef d3;
        Check(d3.LoadFromString(bad), "壞欄章節仍載入");
        Check(!d3.incursion.enabled && !d3.warnings.empty(),
              "壞欄=關閉+警告");

        const char* frac = R"({
            "schema":"potato.campaign_chapter/1","id":"c4",
            "name":"章四","arc":1,"chapter":4,"map":"m.json",
            "incursion":{"seepage":2.5,"kind":"ghost_legion"}})";
        ChapterDef d4;
        Check(d4.LoadFromString(frac), "非整數章節仍載入");
        Check(!d4.incursion.enabled, "seepage=2.5 拒絕（須整數）");
    }

    // ---- [6] 邊界組 ----
    printf("[6] 邊界組\n");
    {
        // Arm(0) = Quiet 章關閉
        BattleController battle(20, 20, 1.0f);
        MythIncursion inc;
        inc.Arm(0, IncursionKind::GhostLegion, spawn);
        inc.Update(0.016f, battle, 3, "斷橋", "橋姬");
        Check(!inc.GetIncursion().fired, "Arm(0) 關閉不觸發");
        Check(!inc.Resolve(true), "從未觸發 Resolve 拒絕");

        // 越界門檻關閉
        inc.Arm(9, IncursionKind::GhostLegion, spawn);
        inc.Update(0.016f, battle, 3, "斷橋", "橋姬");
        Check(!inc.GetIncursion().fired, "門檻越界視為關閉");

        // FoxRumor 無 fog 降級：仍生成小隊、可結算
        MythIncursion inc2;
        inc2.Arm(1, IncursionKind::FoxRumor, spawn);
        inc2.Update(0.016f, battle, 1, "斷橋", "狐仙");
        Check(inc2.GetIncursion().fired, "無 fog 假訊降級觸發");
        Check(inc2.Resolve(true), "降級假訊可解決");

        // 重 Arm 消散舊鬼隊
        MythIncursion inc3;
        inc3.Arm(1, IncursionKind::GhostLegion, spawn);
        inc3.Update(0.016f, battle, 1, "斷橋", "橋姬");
        const Squad* oldGhost = nullptr;
        for (const auto& s : battle.GetSquads()) {
            if (s->GetTeam() == 2 && !s->IsEliminated()) oldGhost = s.get();
        }
        inc3.Arm(1, IncursionKind::GhostLegion, spawn);
        Check(oldGhost && oldGhost->IsEliminated(),
              "重 Arm 消散舊鬼隊");

        // ParseIncursionKind
        IncursionKind k;
        Check(ParseIncursionKind("ghost_legion", k) &&
                  k == IncursionKind::GhostLegion,
              "kind 解析 ghost_legion");
        Check(ParseIncursionKind("fox_rumor", k) &&
                  k == IncursionKind::FoxRumor,
              "kind 解析 fox_rumor");
        Check(!ParseIncursionKind("bogus", k), "未知 kind 拒絕");
    }

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
