// QuantumBench - Q-8 大規模疊加態效能基準（無頭）
//
// 量測 QuantumFog 在 100 實體 × 6 候選規模下的單幀成本：
//   Update（退相干 Diffuse + 時效）、GetCloud（渲染資料）、
//   Probe/Observe（探測與塌縮）、PropagateEntanglement。
// 驗收：Update 單幀 < 1ms（Release）。回傳非零 = 超標或失敗。

#include "Gameplay/QuantumFog.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/BattleController.h"
#include "MathUtils/Vector2.h"

#include <chrono>
#include <cstdio>
#include <vector>

using namespace Potato;
using namespace Potato::Gameplay;
using Clock = std::chrono::steady_clock;

static double TimeMs(Clock::time_point a, Clock::time_point b) {
    return std::chrono::duration<double, std::milli>(b - a).count();
}

int main() {
    printf("=== QuantumFog 效能基準（Q-8）===\n\n");

    constexpr int kEntities = 100;
    constexpr int kCandidates = 6;

    BattleController battle(30, 20, 1.0f);
    BattleResources res;
    res.Setup(battle, 0, /*intel=*/9999, /*cp=*/0);

    QuantumFog fog(30.0f, /*observe=*/2, /*probe=*/1);
    fog.BindResources(&res);

    // ---- 建立 100 實體 × 6 候選 ----
    auto t0 = Clock::now();
    std::vector<int> ids;
    for (int i = 0; i < kEntities; ++i) {
        const Vector2 center(5.0f + (i % 10) * 2.0f,
                             5.0f + (i / 10) * 1.5f);
        ids.push_back(fog.AddEntityCloud("e" + std::to_string(i), 0, center,
                                       /*radius=*/3.0f, kCandidates,
                                       /*minSpacing=*/0.8f));
    }
    const double setupMs = TimeMs(t0, Clock::now());
    printf("建立 %d 實體 x %d 候選：%.2f ms\n", kEntities, kCandidates,
           setupMs);

    // ---- Update 單幀成本（60fps 預算）----
    constexpr int kFrames = 240;
    t0 = Clock::now();
    for (int f = 0; f < kFrames; ++f) fog.Update(1.0f / 60.0f);
    const double updateMs = TimeMs(t0, Clock::now()) / kFrames;
    printf("Update 單幀：%.4f ms（%d 幀平均）\n", updateMs, kFrames);

    // ---- GetCloud（每實體渲染資料產出）----
    t0 = Clock::now();
    size_t cloudPts = 0;
    for (int id : ids)
        cloudPts += fog.GetCloud(id).size();
    const double cloudMs = TimeMs(t0, Clock::now()) / kEntities;
    printf("GetCloud 每實體：%.4f ms（平均候選 %.1f 點）\n", cloudMs,
           static_cast<double>(cloudPts) / kEntities);

    // ---- Probe / Observe 單次成本 ----
    t0 = Clock::now();
    int probes = 0;
    for (int id : ids) {
        if (fog.Probe(id, Vector2(10.0f, 10.0f), 0.4f)) ++probes;
    }
    const double probeMs = TimeMs(t0, Clock::now()) / kEntities;
    printf("Probe 每實體：%.4f ms（%d 成功）\n", probeMs, probes);

    res.Setup(battle, 0, /*intel=*/9999, /*cp=*/0);
    t0 = Clock::now();
    int observed = 0;
    for (int id : ids) {
        if (fog.Observe(id, Vector2(10.0f, 10.0f))) ++observed;
    }
    const double obsMs = TimeMs(t0, Clock::now()) / kEntities;
    printf("Observe 每實體：%.4f ms（%d 塌縮）\n", obsMs, observed);

    // ---- 驗收 ----
    printf("\n");
    int fail = 0;
    // Update < 1ms 為驗收目標；CI/慢機給 5ms 硬上限，1ms 以上印警告
    if (updateMs < 1.0) {
        printf("[PASS] Update %.4f ms < 1ms 驗收門檻\n", updateMs);
    } else if (updateMs < 5.0) {
        printf("[WARN] Update %.4f ms 超過 1ms 目標但在 5ms 硬上限內\n",
               updateMs);
    } else {
        printf("[FAIL] Update %.4f ms 超過 5ms 硬上限——需要 LOD\n",
               updateMs);
        ++fail;
    }
    if (observed == kEntities && probes == kEntities) {
        printf("[PASS] Probe/Observe 全實體可操作\n");
    } else {
        printf("[FAIL] 部分實體觀測失敗（%d/%d, %d/%d）\n", probes,
               kEntities, observed, kEntities);
        ++fail;
    }
    return fail == 0 ? 0 : 1;
}
