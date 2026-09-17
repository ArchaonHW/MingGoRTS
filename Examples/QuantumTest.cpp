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
#include "Gameplay/EnemyGeneral.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <limits>

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
    CHECK(!fog.Probe(id, truePos,
                     std::numeric_limits<float>::quiet_NaN()),
          "NaN strength 探測失敗");
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

static void TestQuantumFogEntangle() {
    printf("-- QuantumFog Entangle --\n");

    BattleController battle(10, 10, 1.0f);
    BattleResources res;
    res.Setup(battle, /*team=*/0, /*intel=*/6, /*cp=*/0);

    QuantumFog fog(5.0f, /*observe=*/2, /*probe=*/1);
    fog.BindResources(&res);

    // A/B 雲同構(B = A + (10,0)):相對偏移一致 → idx 對 idx 相關
    const int a = fog.AddEntity("佯攻隊", 0,
        {Vector2(0, 0), Vector2(0, 4), Vector2(4, 0)});
    const int b = fog.AddEntity("主力隊", 0,
        {Vector2(10, 0), Vector2(10, 4), Vector2(14, 0)});

    CHECK(!fog.Entangle(a, a), "自我糾纏拒絕");
    CHECK(!fog.Entangle(a, 99), "無效 id 糾纏拒絕");
    CHECK(fog.Entangle(a, b), "糾纏建立");
    CHECK(!fog.Entangle(a, b), "重複糾纏拒絕");
    CHECK(fog.EntangledPartner(a) == b, "A 對象是 B");
    CHECK(fog.EntangledPartner(b) == a, "對稱:B 對象是 A");
    CHECK(fog.EntangledPartner(99) == -1, "無效查詢 -1");

    // 觀測 A(真值近候選1) → B 集中到同向候選 (10,4),仍不揭露
    CHECK(fog.Observe(a, Vector2(0.2f, 3.9f)), "觀測 A");
    CHECK(res.GetIntel(0) == 4, "糾纏傳遞不另扣點");
    CHECK(!fog.IsRevealed(b), "B 未揭露");
    double maxP = 0.0;
    Vector2 maxPos;
    for (const auto& c : fog.GetCloud(b)) {
        if (c.second > maxP) { maxP = c.second; maxPos = c.first; }
    }
    CHECK(maxP > 0.6, "B 機率集中(blend 0.7)");
    CHECK(std::abs(maxPos.x - 10.0f) < 0.01f &&
          std::abs(maxPos.y - 4.0f) < 0.01f, "集中到同向候選");

    // 對稱:A 過期回雲後觀測 B → A 集中到同向候選 (0,0)
    fog.Update(6.0f);
    CHECK(!fog.IsRevealed(a), "A 情報過期回雲");
    CHECK(fog.Observe(b, Vector2(10.1f, 0.1f)), "觀測 B");
    maxP = 0.0;
    for (const auto& c : fog.GetCloud(a)) {
        if (c.second > maxP) { maxP = c.second; maxPos = c.first; }
    }
    CHECK(std::abs(maxPos.x) < 0.01f && std::abs(maxPos.y) < 0.01f,
          "對稱:A 集中到同向候選");
    CHECK(!fog.IsRevealed(a), "A 仍未揭露");

    // 已消去的相關候選不復活(硬證據優先於糾纏推測)
    const int c2 = fog.AddEntity("斥候甲", 0,
        {Vector2(0, 6), Vector2(0, 9), Vector2(4, 6)});
    const int d = fog.AddEntity("斥候乙", 0,
        {Vector2(10, 6), Vector2(10, 9), Vector2(14, 6)});
    CHECK(fog.Entangle(c2, d), "第二對糾纏建立");
    CHECK(fog.EliminateCandidate(d, 1), "消去 D 候選1");
    res.Setup(battle, 0, /*intel=*/4, /*cp=*/0);
    CHECK(fog.Observe(c2, Vector2(0.1f, 8.9f)), "觀測 C(近候選1)");
    bool revived = false;
    for (const auto& e : fog.GetCloud(d)) {
        if (std::abs(e.first.x - 10.0f) < 0.01f &&
            std::abs(e.first.y - 9.0f) < 0.01f) revived = true;
    }
    CHECK(!revived, "消去的相關候選不復活");

    // 已揭露對象不受糾纏影響
    CHECK(fog.Reveal(d, Vector2(14.0f, 6.0f)), "接觸揭露 D");
    const Vector2 dp = fog.GetRevealedPos(d);
    fog.Reveal(c2, Vector2(0.0f, 6.0f)); // 對象 d 已揭露,Propagate 跳過
    CHECK(std::abs(fog.GetRevealedPos(d).x - dp.x) < 0.01f,
          "已揭露對象不被糾纏改寫");

    // 接觸揭露也傳遞糾纏,但只在塌縮轉換一次——每 tick Reveal 不複利
    const int e3 = fog.AddEntity("伏兵甲", 0,
        {Vector2(6, 0), Vector2(6, 4), Vector2(9, 0)});
    const int f = fog.AddEntity("伏兵乙", 0,
        {Vector2(16, 0), Vector2(16, 4), Vector2(19, 0)});
    CHECK(fog.Entangle(e3, f), "第三對糾纏建立");
    CHECK(fog.Reveal(e3, Vector2(6.1f, 3.9f)), "接觸揭露 E");
    maxP = 0.0;
    for (const auto& c : fog.GetCloud(f)) {
        if (c.second > maxP) { maxP = c.second; maxPos = c.first; }
    }
    CHECK(maxP > 0.6, "接觸揭露也傳遞糾纏");
    CHECK(std::abs(maxPos.x - 16.0f) < 0.01f &&
          std::abs(maxPos.y - 4.0f) < 0.01f, "集中到同向候選");
    // 模擬持續接觸:重複 Reveal → 對方雲不複利收縮
    fog.Reveal(e3, Vector2(6.1f, 3.9f));
    fog.Reveal(e3, Vector2(6.1f, 3.9f));
    double maxP2 = 0.0;
    for (const auto& c : fog.GetCloud(f)) maxP2 = std::max(maxP2, c.second);
    CHECK(std::abs(maxP2 - maxP) < 1e-9, "重複接觸不複利收縮");

    // Probe 不觸發糾纏(弱測量不構成 joint 測量)
    const int g = fog.AddEntity("遊騎甲", 0,
        {Vector2(0, 12), Vector2(0, 15), Vector2(4, 12)});
    const int h = fog.AddEntity("遊騎乙", 0,
        {Vector2(10, 12), Vector2(10, 15), Vector2(14, 12)});
    CHECK(fog.Entangle(g, h), "第四對糾纏建立");
    res.Setup(battle, 0, /*intel=*/4, /*cp=*/0);
    const auto hBefore = fog.GetCloud(h);
    CHECK(fog.Probe(g, Vector2(0.1f, 14.9f), 0.5f), "探測 G");
    const auto hAfter = fog.GetCloud(h);
    bool hSame = hAfter.size() == hBefore.size();
    for (size_t i = 0; hSame && i < hBefore.size(); ++i) {
        hSame = std::abs(hAfter[i].second - hBefore[i].second) < 1e-9;
    }
    CHECK(hSame, "Probe 不觸發糾纏");
}

static void TestQuantumFogPersonality() {
    printf("-- QuantumFog Personality Priors --\n");

    EnemyGeneral aggro, cautious;
    CHECK(aggro.LoadFromString(
              R"({"name":"猛將","personality":{"aggression":95,"discipline":50,"cunning":10}})"),
          "侵略卡載入");
    CHECK(cautious.LoadFromString(
              R"({"name":"老狐","personality":{"aggression":10,"discipline":90,"cunning":50}})"),
          "謹慎卡載入");

    const Vector2 center(5, 5);
    const Vector2 enemyDir(0, 1); // 敵方在北
    const float radius = 3.0f;
    const Vector2 biasA = aggro.FogBiasPoint(center, enemyDir, radius);
    const Vector2 biasC = cautious.FogBiasPoint(center, enemyDir, radius);
    CHECK(biasA.y > center.y, "侵略型偏置前推");
    CHECK(biasC.y < center.y, "謹慎型偏置後縮");
    CHECK(aggro.FogPriorScale() > cautious.FogPriorScale(),
          "紀律型先驗更集中");

    // 同幾何不同人格 → 先驗分佈可測差異
    // (兩個 fog 實例各自首個 AddEntityCloud 用相同 seed → 候選佈局一致)
    QuantumFog fogA(5.0f, 1, 1), fogC(5.0f, 1, 1);
    const int ea = fogA.AddEntityCloud("甲", 0, center, radius, 8, 0.8f,
                                     &biasA, aggro.FogPriorScale());
    const int ec = fogC.AddEntityCloud("乙", 0, center, radius, 8, 0.8f,
                                     &biasC, cautious.FogPriorScale());
    CHECK(ea >= 0 && ec >= 0, "雙方雲建立");
    // 同 seed 前提顯式斷言:兩實例首次 AddEntityCloud 候選佈局應一致
    const auto& candsA = fogA.GetEntity(ea)->candidates;
    const auto& candsC = fogC.GetEntity(ec)->candidates;
    bool sameLayout = candsA.size() == candsC.size();
    for (size_t i = 0; sameLayout && i < candsA.size(); ++i) {
        sameLayout = std::abs(candsA[i].x - candsC[i].x) < 1e-4f &&
                     std::abs(candsA[i].y - candsC[i].y) < 1e-4f;
    }
    CHECK(sameLayout, "同 seed 候選佈局一致(比較有效)");
    auto frontMass = [center](const QuantumFog& f, int id) {
        double s = 0.0;
        for (const auto& c : f.GetCloud(id)) {
            if (c.first.y > center.y) s += c.second;
        }
        return s;
    };
    CHECK(frontMass(fogA, ea) > frontMass(fogC, ec),
          "侵略型前線先驗質量更高");

    // 狡詐軸:enemyDir=(0,1) 時 perp=(-1,0) → 高狡詐偏置偏 -x 側翼
    EnemyGeneral sly;
    CHECK(sly.LoadFromString(
              R"({"name":"詐將","personality":{"aggression":50,"discipline":50,"cunning":90}})"),
          "狡詐卡載入");
    const Vector2 biasS = sly.FogBiasPoint(center, enemyDir, radius);
    CHECK(biasS.x < center.x, "狡詐型偏置側翼(-x)");
    CHECK(std::abs(biasS.y - center.y) < 1e-4f, "狡詐型不前後偏移");

    // 無卡(三軸預設 50) → 中立:bias=center、scale=1.0
    EnemyGeneral blank;
    const Vector2 biasB = blank.FogBiasPoint(center, enemyDir, radius);
    CHECK(std::abs(biasB.x - center.x) < 1e-4f &&
              std::abs(biasB.y - center.y) < 1e-4f,
          "無卡退回中立偏置");
    CHECK(std::abs(blank.FogPriorScale() - 1.0f) < 1e-4f,
          "無卡退回中立集中度");
    // 零向量退化安全
    const Vector2 zeroBias =
        aggro.FogBiasPoint(center, Vector2(0, 0), radius);
    CHECK(std::abs(zeroBias.x - center.x) < 1e-4f &&
              std::abs(zeroBias.y - center.y) < 1e-4f,
          "零方向退回 center");
}

// ---- Q-5：可注入隨機源後端 ----
void TestRandomSourceInjection() {
    printf("-- RandomSource injection --\n");
    using Quantum::IRandomSource;
    using Quantum::SeededRandomSource;
    using Quantum::EntropyRandomSource;

    // 同 seed 逐位一致（回放可重現）
    {
        SeededRandomSource a(42), b(42), c(43);
        bool same = true, diff = false;
        for (int i = 0; i < 32; ++i) {
            if (a.NextU64() != b.NextU64()) same = false;
        }
        for (int i = 0; i < 32; ++i) {
            if (c.NextU64() != SeededRandomSource(42).NextU64())
                diff = true; // 序列從頭一致才對——這裡應 diff
        }
        CHECK(same, "SeededRandomSource 同 seed 逐位一致");
        CHECK(diff, "不同 seed 產生不同序列");
    }

    // Entropy 後端可建構且產出 [0,1)
    {
        EntropyRandomSource e;
        bool inRange = true;
        for (int i = 0; i < 16; ++i) {
            const double v = e.NextDouble();
            if (!(v >= 0.0 && v < 1.0)) inRange = false;
        }
        CHECK(inRange, "EntropyRandomSource.NextDouble ∈ [0,1)");
    }

    // 劇本源：固定回傳 → Measure 結果可預測
    struct FixedSource : IRandomSource {
        uint64_t v;
        explicit FixedSource(uint64_t x) : v(x) {}
        uint64_t NextU64() override { return v; }
    };
    {
        // roll=0 → 命中第一個基態
        Qudit q0(4, std::unique_ptr<IRandomSource>(
                        new FixedSource(0)));
        CHECK(q0.Measure() == 0, "注入源 roll=0 → 基態 0");
        // roll≈1 → 命中末基態
        Qudit q1(4, std::unique_ptr<IRandomSource>(
                        new FixedSource(~uint64_t(0))));
        CHECK(q1.Measure() == 3, "注入源 roll≈1 → 基態 3");
    }
    {
        // QuantumBitSource 注入源接管擲骰
        // （Born 慣例 roll < P(1) → 1，roll=0 → bit 1）
        Quantum::QuantumBitSource qs(
            std::unique_ptr<IRandomSource>(new FixedSource(0)));
        CHECK(qs.NextBit(), "注入源 roll=0 → bit 1");
        CHECK(qs.NextDouble() == 0.0, "注入源接管 NextDouble");
        // 傳統 seed 路徑不回歸
        Quantum::QuantumBitSource qa(7), qb(7);
        bool seqSame = true;
        for (int i = 0; i < 32; ++i)
            if (qa.NextBit() != qb.NextBit()) seqSame = false;
        CHECK(seqSame, "seed 路徑同 seed 一致");
    }
    {
        // QubitRegister 注入源（roll < P(1)=0.5 → 測得 1）
        QubitRegister reg(2, std::unique_ptr<IRandomSource>(
                               new FixedSource(0)));
        reg.H(0);
        CHECK(reg.Measure(0), "暫存器注入源 roll=0 → 測得 1");
    }
}

int main() {
    printf("=== QuantumTest ===\n");
    TestQubitRegister();
    TestQudit();
    TestQuantumFog();
    TestQuantumFogProbe();
    TestQuantumFogEntangle();
    TestQuantumFogPersonality();
    TestRandomSourceInjection();

    if (g_failures == 0) {
        printf("ALL CHECKS PASSED\n");
        return 0;
    }
    printf("%d CHECKS FAILED\n", g_failures);
    return 1;
}
