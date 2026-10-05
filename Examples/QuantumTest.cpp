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

int main() {
    printf("=== QuantumTest ===\n");
    TestQubitRegister();
    TestQudit();
    TestQuantumFog();

    if (g_failures == 0) {
        printf("ALL CHECKS PASSED\n");
        return 0;
    }
    printf("%d CHECKS FAILED\n", g_failures);
    return 1;
}
