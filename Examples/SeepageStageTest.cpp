// D-5 滲透視效消費端無頭測試：等級→SeepageVisual 映射、
// scrim 過渡狀態機、motion-reduction 降檔、越界 clamp、
// UISettings motionReduction 欄位相容。
#include "Gameplay/SeepageStage.h"
#include "Examples/UISettings.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace Potato;
using namespace Potato::Gameplay;

static int g_pass = 0, g_fail = 0;
static void Check(bool cond, const char* name) {
    if (cond) { ++g_pass; printf("  [PASS] %s\n", name); }
    else      { ++g_fail; printf("  [FAIL] %s\n", name); }
}

int main() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    printf("=== SeepageStage Tests (D-5) ===\n\n");

    // ---- [1] 等級映射 ----
    printf("[1] 等級映射\n");
    {
        SeepageStage st;
        st.Update(0.016f, 0);
        const SeepageVisual& v0 = st.GetVisual();
        Check(v0.fogTint.x == 0.0f && v0.fogTint.y == 0.0f &&
                  v0.fogTint.z == 0.0f,
              "L0 全零指令包");
        Check(v0.audioLevel == 0 && !v0.themeOverride &&
                  v0.jitterAmp == 0.0f && v0.shadowOffset == 0.0f,
              "L0 無偏移/音景/覆寫");

        st.Update(0.016f, 1);
        const SeepageVisual& v1 = st.GetVisual();
        Check(v1.fogTint.x != 0.0f || v1.fogTint.z != 0.0f,
              "L1 霧色偏移非零");
        Check(v1.shadowOffset > 0.0f, "L1 陰影錯位非零");
        Check(v1.audioLevel == 1, "L1 音景 bed");
        Check(v1.jitterAmp == 0.0f && !v1.themeOverride,
              "L1 無微移/無覆寫");

        st.Update(0.016f, 2);
        const SeepageVisual& v2 = st.GetVisual();
        Check(v2.jitterAmp > 0.0f, "L2 物件微移");
        Check(v2.audioLevel == 2, "L2 音景 bed+layer");
        Check(!v2.themeOverride, "L2 無主題覆寫");

        st.Update(0.016f, 3);
        const SeepageVisual& v3 = st.GetVisual();
        Check(v3.themeOverride, "L3 全主題切換旗標");
        Check(v3.audioLevel == 2, "L3 音景沿用（無專屬音態）");
    }

    // ---- [2] 過渡狀態機 ----
    printf("[2] scrim 過渡\n");
    {
        SeepageStage st;
        st.Update(0.016f, 2);
        Check(st.TransitionActive(), "升級觸發過渡");
        Check(st.ScrimAlpha() >= 0.0f, "scrim 非負");
        // 同等級重餵不重播：等 alpha 衰完再餵同級不得重觸發
        float peak = 0.0f;
        for (int i = 0; i < 200; ++i) {
            st.Update(0.05f, 2);
            peak = std::max(peak, st.ScrimAlpha());
        }
        Check(!st.TransitionActive(), "過渡結束");
        Check(peak > 0.3f, "峰值曾達高峰");
        Check(st.ScrimAlpha() == 0.0f, "結束後 alpha 歸零");
        st.Update(0.05f, 2);
        Check(!st.TransitionActive(), "同等級重餵不重播");
        // 再升級 → 重新觸發
        st.Update(0.016f, 3);
        Check(st.TransitionActive(), "再升級重新觸發");
        Check(st.GetVisual().themeOverride, "L3 覆寫生效");
        // 等級回落（跨章重置）→ 覆寫清除
        st.Update(0.016f, 0);
        Check(!st.GetVisual().themeOverride, "回落清除覆寫");
    }

    // ---- [3] motion-reduction ----
    printf("[3] motion-reduction\n");
    {
        SeepageStage st;
        st.SetMotionReduction(true);
        st.Update(0.016f, 3);
        // 短檔：attack+hold+release = 0.05+0+0.12 = 0.17s
        float t = 0.0f;
        while (st.TransitionActive() && t < 5.0f) {
            st.Update(0.02f, 3);
            t += 0.02f;
        }
        Check(t < 0.30f, "降檔過渡 0.3s 內結束");

        SeepageStage st2;
        st2.Update(0.016f, 3);
        float t2 = 0.0f;
        while (st2.TransitionActive() && t2 < 5.0f) {
            st2.Update(0.02f, 3);
            t2 += 0.02f;
        }
        Check(t2 > 0.8f, "常規過渡較長（漸變演出）");
        Check(t2 > t, "motion-reduction 明顯更短");
    }

    // ---- [4] 邊界組 ----
    printf("[4] 邊界組\n");
    {
        SeepageStage st;
        st.Update(0.016f, -5);
        Check(st.Level() == 0, "負等級 clamp 0");
        st.Update(0.016f, 99);
        Check(st.Level() == 3, "越界 clamp 3");
        Check(st.GetVisual().themeOverride, "clamp 後 L3 覆寫");
        // dt<=0/NaN 不推進 scrim
        SeepageStage st2;
        st2.Update(0.016f, 2);
        const float a0 = st2.ScrimAlpha();
        st2.Update(0.0f, 2);
        st2.Update(-1.0f, 2);
        Check(st2.TransitionActive(), "dt<=0 不推進 scrim");
        Check(st2.ScrimAlpha() == a0, "alpha 不變");
    }

    // ---- [5] UISettings motionReduction 相容 ----
    printf("[5] UISettings 相容\n");
    {
        const char* path = "seepage_settings_test.json";
        std::remove(path);
        // 舊檔（無 motionReduction 欄）→ 預設 0
        {
            std::ofstream f(path, std::ios::trunc);
            f << "{\"schema\":\"potato.settings/1\",\"theme\":1,"
                 "\"uiScale\":1.1,\"hudDensity\":2}";
        }
        UISettings::Data d;
        Check(UISettings::Load(path, d), "舊檔可載入");
        Check(d.motionReduction == 0, "缺欄預設 0");
        Check(d.hudDensity == 2 && d.theme == 1, "既有欄位不受影響");
        // 新欄 roundtrip
        d.motionReduction = 1;
        Check(UISettings::Save(path, d), "新檔寫出");
        UISettings::Data d2;
        Check(UISettings::Load(path, d2) && d2.motionReduction == 1,
              "motionReduction roundtrip");
        std::remove(path);
    }

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
