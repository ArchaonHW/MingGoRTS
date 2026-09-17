// QuantumTest - Quantum 模組 + QuantumFog 的 headless 驗證
//
// 覆蓋：
//   QubitRegister — 歸一化、H 疊加、CNOT 糾纏、測量塌縮
//   Qudit       — Born rule 分佈、Diffuse 退相干、CollapseTo
//   QuantumFog  — 情報扣點觀測、時效回疊加、機率雲查詢

#include "Quantum/QubitRegister.h"
#include "Quantum/Qudit.h"
#include "Gameplay/QuantumFog.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/BattleController.h"

#include <algorithm>
#include <cmath>
#include <cstdio>

using namespace Potato;
using namespace Potato::Quantum;
using namespace Potato::Gameplay;

static int g_failures = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", msg); ++g_failures; } \
    else { printf("  ok: %s\n", msg); } \
} while (0)

static void TestQubitRegister() {
    printf("-- QubitRegister --\n");

    QubitRegister reg(2, 42);
    CHECK(reg.IsNormalized(), "初始 |00> 歸一化");
    CHECK(reg.BasisProbability(0) == 1.0, "|00> 機率 = 1");

    reg.H(0);
    CHECK(std::abs(reg.Probability(0) - 0.5) < 1e-9, "H(0) 後 P(q0=1)=0.5");

    // H(q0) + CNOT → 貝爾態 (|00>+|11>)/√2
    reg.CNOT(0, 1);
    CHECK(std::abs(reg.BasisProbability(0) - 0.5) < 1e-9 &&
          std::abs(reg.BasisProbability(3) - 0.5) < 1e-9 &&
          reg.BasisProbability(1) < 1e-9, "貝爾態 |00>+|11>");

    // 糾纏：測 q0 後 q1 必然相同
    bool m0 = reg.Measure(0);
    CHECK(std::abs(reg.Probability(1) - (m0 ? 1.0 : 0.0)) < 1e-9,
          "測量塌縮：q1 跟隨 q0 結果");
    CHECK(reg.IsNormalized(), "塌縮後仍歸一化");

    // MeasureAll 分佈（大量取樣驗 Born rule）
    int ones = 0;
    const int trials = 2000;
    for (int i = 0; i < trials; ++i) {
        QubitRegister r(1, 0);
        r.H(0);
        ones += static_cast<int>(r.MeasureAll());
    }
    CHECK(std::abs(ones / (double)trials - 0.5) < 0.05,
          "H+Measure 分佈 ~50/50");

    printf("  態: %s\n", reg.ToString().c_str());
}

static void TestQudit() {
    printf("-- Qudit --\n");

    Qudit q(4, 7);
    CHECK(std::abs(q.Probability(0) - 0.25) < 1e-9, "均勻疊加 P=0.25");

    q.SetProbabilities({0.1, 0.2, 0.3, 0.4});
    CHECK(std::abs(q.Probability(3) - 0.4) < 1e-9, "SetProbabilities 生效");

    int m = q.Measure();
    CHECK(m >= 0 && m < 4, "Measure 回傳合法 index");
    CHECK(std::abs(q.Probability(m) - 1.0) < 1e-9, "塌縮後 P(m)=1");

    // 退相干：塌縮態 Diffuse(1.0) → 完全均勻
    q.Diffuse(1.0);
    CHECK(std::abs(q.Probability(0) - 0.25) < 1e-9, "Diffuse(1) → 均勻");

    // 塌縮後 Diffuse 一半 → 0.625
    Qudit q2(4, 7);
    q2.CollapseTo(0);
    q2.Diffuse(0.5);
    CHECK(std::abs(q2.Probability(0) - 0.625) < 1e-9, "Diffuse(0.5) 插值正確");
}

static void TestQuantumFog() {
    printf("-- QuantumFog --\n");

    BattleController battle(10, 10, 1.0f);
    BattleResources res;
    res.Setup(battle, /*team=*/0, /*intel=*/2, /*cp=*/0);

    QuantumFog fog(5.0f, 1);  // 時效 5s、觀測花 1 情報
    fog.BindResources(&res);

    std::vector<Vector2> cells = {
        Vector2(8, 2), Vector2(8, 5), Vector2(7, 8)
    };
    int id = fog.AddEntity("敵將親衛", 0, cells, {0.5, 0.3, 0.2});
    CHECK(id >= 0, "AddEntity 成功");
    CHECK(!fog.IsRevealed(id), "初始未揭露");

    auto cloud = fog.GetCloud(id);
    CHECK(cloud.size() == 3, "機率雲 3 候選");
    CHECK(std::abs(cloud[0].second - 0.5) < 1e-6, "先驗機率 0.5 正確");

    // 觀測：扣 1 情報、塌縮到真值最近候選
    Vector2 truePos(8.1f, 5.1f);
    CHECK(fog.Observe(id, truePos), "觀測成功（情報足夠）");
    CHECK(res.GetIntel(0) == 1, "情報扣 1 點");
    CHECK(fog.IsRevealed(id), "已揭露");
    Vector2 rp = fog.GetRevealedPos(id);
    CHECK(std::abs(rp.x - 8.1f) < 0.01f, "揭露真實位置");

    // 已揭露重複觀測不扣點
    CHECK(fog.Observe(id, truePos), "重複觀測回傳 true");
    CHECK(res.GetIntel(0) == 1, "重複觀測不扣點");

    // 情報時效：5s 後回疊加態
    fog.Update(6.0f);
    CHECK(!fog.IsRevealed(id), "時效過期回疊加態");

    // 情報耗盡 → 觀測失敗、態不塌縮
    CHECK(res.GetIntel(0) == 1, "剩 1 情報");
    CHECK(fog.Observe(id, truePos), "最後一點觀測成功");
    fog.Update(6.0f);  // 再過期
    CHECK(!fog.Observe(id, truePos), "情報耗盡觀測失敗");
    CHECK(!fog.IsRevealed(id), "失敗觀測不揭露");
}

// 雲中最大候選機率(含被 GetCloud 篩掉的——此處直接查 fog 內部不方便,
// 用 GetCloud 即可,本測試機率皆遠高於 1e-4 閾值)
static double CloudMaxProb(const std::vector<std::pair<Vector2, double>>& c) {
    double m = 0.0;
    for (const auto& e : c) m = std::max(m, e.second);
    return m;
}
static double CloudProbSum(const std::vector<std::pair<Vector2, double>>& c) {
    double s = 0.0;
    for (const auto& e : c) s += e.second;
    return s;
}

static void TestQuantumFogProbe() {
    printf("-- QuantumFog Probe --\n");

    BattleController battle(10, 10, 1.0f);
    BattleResources res;
    res.Setup(battle, /*team=*/0, /*intel=*/4, /*cp=*/0);

    QuantumFog fog(5.0f, /*observe=*/2, /*probe=*/1);
    fog.BindResources(&res);

    int id = fog.AddEntity("敵將親衛", 0,
                           {Vector2(8, 2), Vector2(8, 5), Vector2(7, 8)},
                           {0.34, 0.33, 0.33});
    Vector2 truePos(8.1f, 5.1f); // 最近候選 = (8,5)

    // 無效輸入不扣點
    CHECK(!fog.Probe(-1, truePos, 0.4f), "無效 id 探測失敗");
    CHECK(!fog.Probe(id, truePos, 0.0f), "strength<=0 探測失敗");
    CHECK(!fog.Probe(id, truePos, 0.0f / 0.0f), "NaN strength 探測失敗");
    CHECK(res.GetIntel(0) == 4, "失敗探測不扣點");

    // 探測:扣 1 情報 < 觀測 2;機率向真值收縮但不塌縮
    const double p0 = CloudMaxProb(fog.GetCloud(id));
    CHECK(fog.Probe(id, truePos, 0.4f), "探測成功");
    CHECK(res.GetIntel(0) == 3, "探測扣 1 點(觀測價 2)");
    auto c1 = fog.GetCloud(id);
    const double p1 = CloudMaxProb(c1);
    CHECK(p1 > p0, "探測後最大機率上升");
    CHECK(c1.size() > 1, "探測不完全塌縮(>1 非零候選)");
    CHECK(!fog.IsRevealed(id), "探測不揭露");
    CHECK(std::abs(CloudProbSum(c1) - 1.0) < 1e-6, "機率維持歸一化");

    // 重複探測:單調收斂向塌縮逼近
    CHECK(fog.Probe(id, truePos, 0.4f), "第二次探測成功");
    const double p2 = CloudMaxProb(fog.GetCloud(id));
    CHECK(p2 > p1, "第二次探測繼續收縮");
    CHECK(res.GetIntel(0) == 2, "再扣 1 點");

    // strength>1 clamp:等同 strength=1,仍可收縮不爆掉
    CHECK(fog.Probe(id, truePos, 5.0f), "strength>1 仍成功(clamp)");
    CHECK(res.GetIntel(0) == 1, "clamp 探測照扣 1 點");

    // 補滿情報連續探測 → maxProb 趨近 1(逼近塌縮但不觸發揭露)
    res.Setup(battle, 0, /*intel=*/8, /*cp=*/0);
    double prev = p2;
    for (int i = 0; i < 8; ++i) {
        CHECK(fog.Probe(id, truePos, 0.4f), "連續探測成功");
        const double cur = CloudMaxProb(fog.GetCloud(id));
        CHECK(cur >= prev, "探測機率單調不減");
        prev = cur;
    }
    CHECK(prev > 0.9, "連續探測逼近塌縮");
    CHECK(!fog.IsRevealed(id), "逼近塌縮仍不揭露");

    // 情報不足 → 失敗、雲不變(逐候選比較,不只比 size)
    res.Setup(battle, 0, /*intel=*/0, /*cp=*/0);
    const auto before = fog.GetCloud(id);
    CHECK(!fog.Probe(id, truePos, 0.4f), "情報耗盡探測失敗");
    const auto after = fog.GetCloud(id);
    bool sameCloud = after.size() == before.size();
    for (size_t i = 0; sameCloud && i < before.size(); ++i) {
        sameCloud = std::abs(after[i].second - before[i].second) < 1e-9;
    }
    CHECK(sameCloud, "失敗探測雲不變");

    // 已揭露實體:探測回 true 不扣點
    res.Setup(battle, 0, /*intel=*/3, /*cp=*/0);
    CHECK(fog.Observe(id, truePos), "觀測揭露");
    CHECK(fog.Probe(id, truePos, 0.4f), "已揭露探測回 true");
    CHECK(res.GetIntel(0) == 1, "已揭露探測不扣點");
}

int main() {
    printf("=== QuantumTest ===\n");
    TestQubitRegister();
    TestQudit();
    TestQuantumFog();
    TestQuantumFogProbe();

    if (g_failures == 0) {
        printf("ALL CHECKS PASSED\n");
        return 0;
    }
    printf("%d CHECKS FAILED\n", g_failures);
    return 1;
}
