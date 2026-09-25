// PeaceHazardTest — PPAH 隨機首達時框架驗證（headless）
// 覆蓋：softplus hazard 單調性、competing-risks 歸因機率
// （Eq.32 a/(a+b)）、發散累積 a.s. 命中、時間單位不變性、
// restart 性質（存檔還原後序列一致）、schema tag、absorbing。

#include "Campaign/PeaceHazard.h"
#include "Serialization/JsonParser.h"

#include <cmath>
#include <cstdio>

using namespace Potato::Campaign;
using Potato::JsonValue;

static int failures = 0;
static void Check(bool cond, const char* name) {
    if (cond) {
        printf("  [PASS] %s\n", name);
    } else {
        printf("  [FAIL] %s\n", name);
        ++failures;
    }
}

int main() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    printf("=== PeaceHazard Tests (PPAH) ===\n");

    // ---- [1] softplus 性質 ----
    printf("\n[1] softplus\n");
    {
        Check(std::fabs(PeaceHazard::Softplus(0.0f) -
                        std::log(2.0f)) < 1e-6f,
              "softplus(0)=ln2");
        Check(std::fabs(PeaceHazard::Softplus(30.0f) - 30.0f) < 1e-4f,
              "大輸入線性（不溢出）");
        Check(PeaceHazard::Softplus(-30.0f) > 0.0f,
              "大負輸入仍為正");
        Check(PeaceHazard::Softplus(1.0f) >
                  PeaceHazard::Softplus(0.5f),
              "單調遞增");
    }

    // ---- [2] hazard 對特徵的符號方向 ----
    printf("\n[2] hazard 係數方向\n");
    {
        PeaceHazard h;
        PeaceHazard::Features good{0.8f, 0.8f, 0.0f, 0.0f};
        PeaceHazard::Features bad{0.2f, 0.2f, 0.8f, 0.8f};
        Check(h.LambdaPeace(good) > h.LambdaPeace(bad),
              "民心/秩序高 → λ_P 升");
        Check(h.LambdaCollapse(bad) > h.LambdaCollapse(good),
              "墮落/滲透高 → λ_C 升");
        Check(h.LambdaPeace(bad) > 0.0f &&
                  h.LambdaCollapse(bad) > 0.0f,
              "softplus 保證 hazard 恆正（發散累積前提）");
    }

    // ---- [3] competing risks：歸因機率收斂 λ_P/λ（Eq.32）----
    printf("\n[3] competing risks 歸因\n");
    {
        // 劣勢局：λ_P≈softplus(-3.5)≈0.0294，λ_C≈softplus(-2.55)≈0.0751
        const PeaceHazard::Features f{0.2f, 0.2f, 0.6f, 0.5f};
        PeaceHazard probe;
        const double lp = probe.LambdaPeace(f);
        const double lc = probe.LambdaCollapse(f);
        const double analytic = lp / (lp + lc);

        const int N = 2000;
        int peaceWins = 0;
        for (int i = 0; i < N; ++i) {
            PeaceHazard h;
            h.Seed(1000 + i);
            for (int step = 0;
                 step < 10000 &&
                 h.GetOutcome() == PeaceHazard::Outcome::Unresolved;
                 ++step) {
                h.Advance(f);
            }
            if (h.GetOutcome() == PeaceHazard::Outcome::StablePeace) {
                ++peaceWins;
            }
        }
        const double empirical = (double)peaceWins / N;
        printf("    analytic=%.4f empirical=%.4f\n", analytic,
               empirical);
        Check(std::fabs(empirical - analytic) < 0.05,
              "Pr(和平先於崩壞) ≈ λ_P/(λ_P+λ_C)");
    }

    // ---- [4] 發散累積 hazard → a.s. 命中 ----
    printf("\n[4] a.s. 命中\n");
    {
        PeaceHazard h;
        h.Seed(7);
        const PeaceHazard::Features f{0.5f, 0.5f, 0.0f, 0.0f};
        for (int i = 0; i < 5000 &&
                        h.GetOutcome() ==
                            PeaceHazard::Outcome::Unresolved;
             ++i) {
            h.Advance(f);
        }
        Check(h.GetOutcome() != PeaceHazard::Outcome::Unresolved,
              "λ>0 恆成立 → 有限步內必命中");
        // λ_P≈0.20、λ_C≈0.005 的基準局，和平應主導
        Check(h.GetOutcome() == PeaceHazard::Outcome::StablePeace,
              "基準局 → StablePeace");
    }

    // ---- [5] absorbing 契約 + dt<=0 no-op ----
    printf("\n[5] absorbing\n");
    {
        PeaceHazard h;
        h.Seed(3);
        const PeaceHazard::Features f{0.5f, 0.5f, 0.0f, 0.0f};
        h.Advance(0.0, f);
        Check(h.CumulativePeaceHazard() == 0.0,
              "dt=0 不推進累積 hazard");
        while (h.GetOutcome() == PeaceHazard::Outcome::Unresolved) {
            h.Advance(f);
        }
        const double cpAfter = h.CumulativePeaceHazard();
        const uint64_t dAfter = h.DrawCount();
        h.Advance(f);
        Check(h.CumulativePeaceHazard() == cpAfter &&
                  h.DrawCount() == dAfter,
              "命中後 Advance 為 no-op");
    }

    // ---- [6] 時間單位不變性（Thm 4.3）----
    printf("\n[6] 單位不變\n");
    {
        const PeaceHazard::Features f{0.4f, 0.6f, 0.1f, 0.2f};
        PeaceHazard a, b;
        a.Seed(1);
        b.Seed(1);
        a.Advance(1.0, f);              // 一步 Δt=1
        b.Advance(0.5, f);              // 兩步 Δt=0.5
        b.Advance(0.5, f);
        Check(std::fabs(a.CumulativePeaceHazard() -
                        b.CumulativePeaceHazard()) < 1e-12,
              "Σλ·Δt 與切分粒度無關");
        Check(std::fabs(a.CumulativeCollapseHazard() -
                        b.CumulativeCollapseHazard()) < 1e-12,
              "collapse 累積同不變");
    }

    // ---- [7] restart：存檔還原後序列一致 ----
    printf("\n[7] restart/存檔\n");
    {
        const PeaceHazard::Features f{0.5f, 0.5f, 0.0f, 0.0f};
        PeaceHazard a;
        a.Seed(42);
        for (int i = 0; i < 3; ++i) a.Advance(f);

        PeaceHazard b;
        Check(b.FromJson(a.ToJson()), "FromJson 接受自產檔");
        Check(b.CumulativePeaceHazard() == a.CumulativePeaceHazard() &&
                  b.CumulativeCollapseHazard() ==
                      a.CumulativeCollapseHazard(),
              "累積 hazard 還原");
        Check(b.DrawCount() == a.DrawCount(), "draws 還原");
        Check(b.GetOutcome() == a.GetOutcome(), "outcome 還原");

        // 兩實例同餵後續步 → 完全一致的後續軌跡
        for (int i = 0; i < 5; ++i) {
            a.Advance(f);
            b.Advance(f);
        }
        Check(a.GetOutcome() == b.GetOutcome() &&
                  a.CumulativePeaceHazard() ==
                      b.CumulativePeaceHazard() &&
                  a.DrawCount() == b.DrawCount(),
              "還原後續軌跡逐位一致（restart）");
    }

    // ---- [8] schema tag 軟驗證 ----
    printf("\n[8] schema tag\n");
    {
        PeaceHazard h;
        const JsonValue j = h.ToJson();
        Check(j["schema"].AsString() == "potato.peace_hazard/1",
              "寫出 potato.peace_hazard/1");
        JsonValue noTag = j;
        noTag.objectValue.erase("schema");
        PeaceHazard r;
        Check(r.FromJson(noTag), "缺 tag 舊檔容忍");
        JsonValue badTag = j;
        badTag.objectValue["schema"] =
            JsonValue::String("potato.other/9");
        Check(!r.FromJson(badTag), "錯 tag 拒絕");
    }

    // ---- [9] Survival 衰減 + FromGovernance 映射 ----
    printf("\n[9] survival/映射\n");
    {
        Governance gov;
        gov.AdjustPopularSupport(-50.0f); // 50→0
        gov.AdjustCivilOrder(50.0f);      // 50→100
        const auto f =
            PeaceHazard::FromGovernance(gov, 2.0f); // 超界 clamp
        Check(std::fabs(f.popularSupport - 0.0f) < 1e-6f &&
                  std::fabs(f.civilOrder - 1.0f) < 1e-6f &&
                  std::fabs(f.seepagePressure - 1.0f) < 1e-6f,
              "FromGovernance 0..1 正規化+clamp");

        PeaceHazard h;
        const PeaceHazard::Features mid{0.5f, 0.5f, 0.0f, 0.0f};
        h.Advance(mid);
        const double expect =
            std::exp(-(h.CumulativePeaceHazard() +
                       h.CumulativeCollapseHazard()));
        Check(std::fabs(h.Survival() - expect) < 1e-12,
              "S(t)=exp(−Λ_P−Λ_C)");
        Check(h.StepPeaceChance(mid) > 0.0 &&
                  h.StepPeaceChance(mid) < 1.0 &&
                  h.StepCollapseChance(mid) > 0.0,
              "步條件機率界於 (0,1)");
    }

    printf("\n=== 結果: %s ===\n", failures == 0 ? "全部 PASS" : "有 FAIL");
    return failures == 0 ? 0 : 1;
}
