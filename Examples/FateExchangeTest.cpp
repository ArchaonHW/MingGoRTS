// FateExchangeTest - D-3 天命貨幣與轉換無頭測試
// 驗收：兌換扣 favor 加治理軸、favor 不足灰顯可見不隱藏、
//       戰中限次/重置、章節邊界不限次、零副作用拒絕、
//       FateEvent 回呼入帳接線
#include "Campaign/FateExchange.h"
#include "Campaign/Governance.h"
#include "Campaign/MythLayer.h"

#include <cstdio>
#include <functional>
#include <string>
#include <vector>

using namespace Potato::Campaign;

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* name) {
    if (ok) { g_pass++; printf("  [PASS] %s\n", name); }
    else    { g_fail++; printf("  [FAIL] %s\n", name); }
}

int main() {
    printf("=== FateExchange Tests (D-3) ===\n\n");

    const std::string spirit = "境靈";

    // ---- [1] 兌換成功：扣 favor、加軸、發事件 ----
    printf("[1] 兌換成功\n");
    {
        MythLayer myth;
        Governance gov;
        FateExchange ex;
        ex.Bind(myth, gov);
        myth.AdjustFavor(spirit, 30.0f); // 50+30=80
        std::vector<FateEvent> events;
        ex.SetEventCallback(
            [&events](const FateEvent& e) { events.push_back(e); });

        Check(ex.Convert(spirit, FateTarget::PopularSupport, false),
              "兌換民心成功");
        Check(myth.Favor(spirit) ==
                  80.0f - FateExchange::kFateCost,
              "favor 扣 kFateCost");
        Check(gov.PopularSupport() == 50.0f + FateExchange::kFateGain,
              "民心 +kFateGain");
        Check(gov.CivilOrder() == 50.0f, "秩序不受影響");
        Check(events.size() == 1, "FateEvent 一筆");
        if (events.size() == 1) {
            Check(events.at(0).spirit == spirit &&
                      events.at(0).target ==
                          FateTarget::PopularSupport &&
                      events.at(0).cost == FateExchange::kFateCost &&
                      events.at(0).gain == FateExchange::kFateGain &&
                      !events.at(0).inBattle,
                  "事件欄位齊全");
        }
    }

    // ---- [2] favor 不足：拒絕 + 灰顯可見 ----
    printf("[2] favor 不足\n");
    {
        MythLayer myth;
        Governance gov;
        FateExchange ex;
        ex.Bind(myth, gov);
        myth.AdjustFavor(spirit, -40.0f); // 50-40=10 < 20
        bool cb = false;
        ex.SetEventCallback([&cb](const FateEvent&) { cb = true; });

        Check(!ex.Convert(spirit, FateTarget::CivilOrder, false),
              "不足 Convert 拒絕");
        Check(myth.Favor(spirit) == 10.0f, "favor 不變");
        Check(gov.CivilOrder() == 50.0f, "秩序不變");
        Check(!cb, "拒絕不發回呼");

        auto opts = ex.Options(spirit);
        Check(opts.size() == 2, "兩項價目全列（可見不隱藏）");
        Check(!opts.at(0).affordable && !opts.at(1).affordable,
              "兩項 affordable=false");
        Check(!opts.at(0).reason.empty(), "附原因字串");
        Check(opts.at(0).cost == FateExchange::kFateCost &&
                  opts.at(0).gain == FateExchange::kFateGain,
              "價目常數正確");
    }

    // ---- [3] 戰中限次 + 重置 ----
    printf("[3] 戰中限次\n");
    {
        MythLayer myth;
        Governance gov;
        FateExchange ex;
        ex.Bind(myth, gov);
        myth.AdjustFavor(spirit, 50.0f); // 100

        for (int i = 0; i < FateExchange::kMaxBattleUses; ++i) {
            Check(ex.Convert(spirit, FateTarget::CivilOrder, true),
                  "戰中額度內成功");
        }
        Check(!ex.Convert(spirit, FateTarget::CivilOrder, true),
              "額度盡拒絕");
        Check(ex.BattleUses() == FateExchange::kMaxBattleUses,
              "計數到位");
        Check(!ex.Options(spirit).at(0).battleUsesLeft,
              "Options 報額度盡");
        // 章節邊界不受戰中額度限制
        Check(ex.Convert(spirit, FateTarget::PopularSupport, false),
              "章節邊界不限次");
        ex.ResetBattleUses();
        Check(ex.Convert(spirit, FateTarget::CivilOrder, true),
              "重置後戰中可再兌");
    }

    // ---- [4] 邊界與防禦 ----
    printf("[4] 邊界防禦\n");
    {
        MythLayer myth;
        Governance gov;
        FateExchange ex; // 未 Bind
        Check(!ex.Convert(spirit, FateTarget::PopularSupport, false),
              "未綁定拒絕");
        Check(!ex.Options(spirit).at(0).affordable,
              "未綁定灰顯");

        ex.Bind(myth, gov);
        Check(!ex.Convert("", FateTarget::PopularSupport, false),
              "空 spirit 拒絕");
        Check(!ex.Options("").at(0).affordable, "空 spirit 灰顯");

        // 非法 enum：零副作用
        myth.AdjustFavor(spirit, 30.0f); // 80
        Check(!ex.Convert(spirit, (FateTarget)99, false),
              "非法 target 拒絕");
        Check(myth.Favor(spirit) == 80.0f, "非法 target 不扣 favor");
        Check(gov.PopularSupport() == 50.0f, "非法 target 軸不變");

        // 幻影錢包防線：未登錄 spirit 不得以中立 50 起算扣費
        Check(ex.Options("新神").at(0).affordable,
              "未登錄 spirit 價目仍顯示（中立 50）");
        Check(!ex.Convert("新神", FateTarget::CivilOrder, false),
              "未登錄 spirit Convert 拒絕");
        Check(!myth.HasSpirit("新神"), "拒絕不鑄錢包");
        Check(gov.CivilOrder() == 50.0f, "幻影兌換軸不變");
    }

    // ---- [5] 軸夾取 ----
    printf("[5] 夾取\n");
    {
        MythLayer myth;
        Governance gov;
        FateExchange ex;
        ex.Bind(myth, gov);
        myth.AdjustFavor(spirit, 50.0f);
        gov.AdjustPopularSupport(48.0f); // 98
        Check(ex.Convert(spirit, FateTarget::PopularSupport, false),
              "高位兌換");
        Check(gov.PopularSupport() == 100.0f, "民心夾到 100");
        // 低端夾取
        gov.AdjustCivilOrder(-100.0f);
        Check(gov.CivilOrder() == 0.0f, "秩序夾到 0");
        Check(gov.Depravity() == 0.0f, "墮落不被調整觸碰");
    }

    // ---- [6] 滿格夾取時 gain 報實際生效量 ----
    printf("[6] 事件 gain 如實\n");
    {
        MythLayer myth;
        Governance gov;
        FateExchange ex;
        ex.Bind(myth, gov);
        myth.AdjustFavor(spirit, 30.0f); // 80
        gov.AdjustPopularSupport(48.0f); // 98
        FateEvent ev{};
        ex.SetEventCallback([&ev](const FateEvent& e) { ev = e; });
        ex.Convert(spirit, FateTarget::PopularSupport, false);
        Check(ev.gain == 2.0f,
              "gain=實際生效 2 而非名目 5（record-is-truth）");

        // 滿軸拒絕：實得為零不白扣費
        Check(!ex.Convert(spirit, FateTarget::PopularSupport, false),
              "滿軸 Convert 拒絕");
        Check(myth.Favor(spirit) == 60.0f, "滿軸不扣 favor");
        Check(!ex.Options(spirit).at(0).affordable,
              "滿軸民心項灰顯");
        Check(ex.Options(spirit).at(1).affordable,
              "秩序項仍正常可兌");
    }

    // ---- [7] favor==cost 邊界 + 多神隔離 ----
    printf("[7] 邊界值與隔離\n");
    {
        MythLayer myth;
        Governance gov;
        FateExchange ex;
        ex.Bind(myth, gov);
        myth.AdjustFavor(spirit, -30.0f); // 50-30=20 == kFateCost
        Check(ex.Options(spirit).at(0).affordable,
              "favor==cost 判定足額");
        Check(ex.Convert(spirit, FateTarget::CivilOrder, false),
              "favor==cost 可兌");
        Check(myth.Favor(spirit) == 0.0f, "扣到 0");
        Check(!ex.Convert(spirit, FateTarget::CivilOrder, false),
              "扣到 0 後拒絕");

        // 多神隔離：A 扣費不動 B
        myth.AdjustFavor("乙神", 10.0f); // 60
        Check(myth.Favor("乙神") == 60.0f, "乙神 favor 獨立");
        Check(ex.Convert("乙神", FateTarget::CivilOrder, false),
              "乙神可兌");
        Check(myth.Favor("乙神") == 40.0f, "乙神扣費");
    }

    // ---- [8] 回呼再入安全 ----
    printf("[8] 回呼再入\n");
    {
        MythLayer myth;
        Governance gov;
        FateExchange ex;
        ex.Bind(myth, gov);
        myth.AdjustFavor(spirit, 50.0f); // 100
        int depth = 0;
        std::function<void(const FateEvent&)> cb;
        cb = [&](const FateEvent&) {
            if (++depth == 1) {
                // 回呼內再入 Convert——已扣費已計次，再入走新一次
                ex.Convert(spirit, FateTarget::CivilOrder, true);
            }
        };
        ex.SetEventCallback(cb);
        Check(ex.Convert(spirit, FateTarget::PopularSupport, true),
              "外層兌換成功");
        Check(depth == 2, "再入事件亦派出");
        Check(ex.BattleUses() == 2, "再入各計一次額度");
        Check(myth.Favor(spirit) ==
                  100.0f - 2 * FateExchange::kFateCost,
              "兩次都扣費");
    }

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
