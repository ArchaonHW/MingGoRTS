// QuasiRandomTest - MathUtils/QuasiRandom 的 headless 驗證
//
// 覆蓋：
//   VanDerCorput/Halton — 範圍與覆蓋性
//   R2/R2InDisk         — 差異度優於 PRNG、範圍、決定性
//   PoissonDisk         — 最小間距、圓內約束、種子決定性
//   QuantumFog::AddEntityCloud — 藍噪候選格接入疊加態霧

#include "MathUtils/QuasiRandom.h"
#include "Gameplay/QuantumFog.h"

#include <cmath>
#include <cstdio>
#include <random>
#include <vector>

using namespace Potato;
using namespace Potato::Quasi;
using namespace Potato::Gameplay;

static int g_failures = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", msg); ++g_failures; } \
    else { printf("  ok: %s\n", msg); } \
} while (0)

static void TestVanDerCorput() {
    printf("-- VanDerCorput --\n");

    // 前 4 項應為 1/2, 1/4, 3/4, 1/8（base 2, n=1..4）
    const double e0 = VanDerCorput(1); // 0.5
    const double e1 = VanDerCorput(2); // 0.25
    const double e2 = VanDerCorput(3); // 0.75
    CHECK(std::abs(e0 - 0.5) < 1e-12, "VDC(1) = 1/2");
    CHECK(std::abs(e1 - 0.25) < 1e-12, "VDC(2) = 1/4");
    CHECK(std::abs(e2 - 0.75) < 1e-12, "VDC(3) = 3/4");

    for (uint64_t i = 1; i < 1024; ++i) {
        const double v = VanDerCorput(i);
        if (v <= 0.0 || v >= 1.0) {
            CHECK(false, "VDC 值域 (0,1)");
            return;
        }
    }
    CHECK(true, "VDC 值域 (0,1) 維持 1024 項");
}

static void TestR2() {
    printf("-- R2 golden-ratio sequence --\n");

    // 塑性常數健全性：ψ₂³ = ψ₂ + 1
    const double psi2 = PlasticConstant(2);
    CHECK(std::abs(psi2 * psi2 * psi2 - psi2 - 1.0) < 1e-10,
          "塑性常數滿足 ψ^3 = ψ + 1");

    // 值域
    std::vector<Vector2> pts;
    for (uint64_t i = 0; i < 512; ++i) pts.push_back(R2(i));
    for (const Vector2& p : pts) {
        if (p.x < 0.0f || p.x >= 1.0f || p.y < 0.0f || p.y >= 1.0f) {
            CHECK(false, "R2 值域 [0,1)^2");
            return;
        }
    }
    CHECK(true, "R2 值域 [0,1)^2 維持 512 項");

    // 核心宣稱：R2 差異度應顯著優於 PRNG（這是採用它的理由）
    std::vector<Vector2> rnd;
    std::mt19937_64 rng(42);
    std::uniform_real_distribution<float> u01(0.0f, 1.0f);
    for (int i = 0; i < 512; ++i) rnd.push_back(Vector2(u01(rng), u01(rng)));

    const double dR2 = Discrepancy2D(pts);
    const double dRand = Discrepancy2D(rnd);
    printf("    discrepancy: R2=%.6f  PRNG=%.6f\n", dR2, dRand);
    CHECK(dR2 < dRand * 0.7, "R2 差異度顯著低於 PRNG（≥30% 改善）");

    // skip 參數打亂序列起點（文件註明的 jitter 用法）
    const Vector2 a = R2(0), b = R2(0, 12345);
    CHECK(a.x != b.x || a.y != b.y, "R2 skip 改變序列起點");
}

static void TestR2InDisk() {
    printf("-- R2InDisk --\n");

    const float radius = 10.0f;
    for (uint64_t i = 0; i < 256; ++i) {
        const Vector2 p = R2InDisk(i, radius);
        if (p.Length() > radius + 1e-4f) {
            CHECK(false, "R2InDisk 點在圓外");
            return;
        }
    }
    CHECK(true, "R2InDisk 256 點皆在圓內");

    // 決定性：同參數同輸出
    const Vector2 p1 = R2InDisk(7, radius), p2 = R2InDisk(7, radius);
    CHECK(p1.x == p2.x && p1.y == p2.y, "R2InDisk 決定性");
}

static void TestPoissonDisk() {
    printf("-- PoissonDisk --\n");

    const Vector2 center(50.0f, 50.0f);
    const float radius = 20.0f, minDist = 5.0f;
    const auto pts = PoissonDisk(center, radius, 40, minDist, 7);

    CHECK(pts.size() >= 20, "PoissonDisk 產生足量點（≥20/40，允許飽和）");

    // 全部在圓內
    for (const Vector2& p : pts) {
        const Vector2 d = p - center;
        if (d.Length() > radius + 1e-4f) {
            CHECK(false, "PoissonDisk 點在圓外");
            return;
        }
    }
    CHECK(true, "PoissonDisk 所有點在圓內");

    // 最小間距（藍噪核心不變量）
    bool ok = true;
    for (size_t i = 0; i < pts.size() && ok; ++i)
        for (size_t j = i + 1; j < pts.size(); ++j) {
            const Vector2 d = pts[i] - pts[j];
            if (d.Length() < minDist - 1e-3f) { ok = false; break; }
        }
    CHECK(ok, "PoissonDisk 最小間距不變量");

    // 種子決定性（回放需求）
    const auto pts2 = PoissonDisk(center, radius, 40, minDist, 7);
    CHECK(pts.size() == pts2.size(), "PoissonDisk 同 seed 點數一致");
    bool same = pts.size() == pts2.size();
    for (size_t i = 0; same && i < pts.size(); ++i)
        same = (pts[i].x == pts2[i].x && pts[i].y == pts2[i].y);
    CHECK(same, "PoissonDisk 同 seed 逐點一致");

    // 邊界條件
    CHECK(PoissonDisk(center, radius, 0, minDist).empty(),
          "PoissonDisk count=0 回傳空");
    CHECK(PoissonDisk(center, 0.0f, 10, minDist).empty(),
          "PoissonDisk radius=0 回傳空");
}

static void TestAddEntityCloud() {
    printf("-- QuantumFog::AddEntityCloud --\n");

    QuantumFog fog(30.0f, 1);
    const int id = fog.AddEntityCloud(
        "藍噪偵查區", 1, Vector2(30.0f, 30.0f), 12.0f, 6, 4.0f);
    CHECK(id >= 0, "AddEntityCloud 註冊成功");

    const UncertainEntity* e = fog.GetEntity(id);
    CHECK(e != nullptr, "實體可查");
    CHECK(e->candidates.size() >= 2, "候選數 ≥ 2");

    // 先驗歸一化
    double sum = 0.0;
    for (double p : e->priors) sum += p;
    CHECK(std::abs(sum - 1.0) < 1e-9, "先驗機率歸一化");

    // 候選格最小間距（Poisson 路徑生效時成立）
    bool spaced = true;
    for (size_t i = 0; i < e->candidates.size() && spaced; ++i)
        for (size_t j = i + 1; j < e->candidates.size(); ++j) {
            const Vector2 d = e->candidates[i] - e->candidates[j];
            if (d.Length() < 4.0f - 1e-3f) { spaced = false; break; }
        }
    CHECK(spaced, "候選格藍噪間距（≥minSpacing）");

    // 完整生命周期：觀測 → 塌縮 → 雲收斂
    const Vector2 truePos(28.0f, 31.0f);
    CHECK(fog.Observe(id, truePos), "Observe 塌縮");
    const auto cloud = fog.GetCloud(id);
    CHECK(cloud.size() == 1 &&
          std::abs(cloud[0].second - 1.0) < 1e-9,
          "塌縮後雲為單點 1.0");

    // 邊界條件
    CHECK(fog.AddEntityCloud("bad", 1, Vector2(0, 0), 0.0f, 6, 4.0f) < 0,
          "radius=0 拒絕");
    CHECK(fog.AddEntityCloud("bad2", 1, Vector2(0, 0), 5.0f, 1, 4.0f) < 0,
          "count<2 拒絕");
}

int main() {
    printf("=== QuasiRandomTest ===\n");
    TestVanDerCorput();
    TestR2();
    TestR2InDisk();
    TestPoissonDisk();
    TestAddEntityCloud();

    if (g_failures == 0) {
        printf("=== ALL CHECKS PASSED ===\n");
        return 0;
    }
    printf("=== %d FAILURES ===\n", g_failures);
    return 1;
}
