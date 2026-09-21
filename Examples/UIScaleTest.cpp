// UIScaleTest — F-2 UI scale 真管線驗證（headless）
// 覆蓋：ApplyScaled 絕對重建（無累乘漂移）、ClampScale 邊界、
// Px 尺寸閘門、UISettings potato.settings/1 roundtrip/鉗制/缺檔/壞檔。

#include "UITheme.h"
#include "UISettings.h"
#include "imgui.h"

#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* name) {
    if (ok) { ++g_pass; printf("  [PASS] %s\n", name); }
    else    { ++g_fail; printf("[FAIL] %s\n", name); }
}

static bool Nearly(float a, float b) {
    return std::fabs(a - b) < 0.0001f;
}

int main() {
    printf("=== UI Scale Pipeline Tests (F-2) ===\n");
    ImGui::CreateContext(); // ImGuiStyle 方法不需 frame，但建 context 保險

    // ---- [1] 絕對重建：同參數恆同 style ----
    printf("\n[1] ApplyScaled 確定性\n");
    {
        ImGuiStyle a, b;
        UITheme::ApplyScaled(a, UITheme::Id::WarMap, 1.5f);
        UITheme::ApplyScaled(b, UITheme::Id::WarMap, 1.5f);
        Check(Nearly(a.WindowPadding.x, b.WindowPadding.x) &&
                  Nearly(a.ItemSpacing.y, b.ItemSpacing.y) &&
                  Nearly(a.FramePadding.x, b.FramePadding.x),
              "同參數兩次重建逐位一致");
    }

    // ---- [2] 基底×scale：1.5x 的 WarMap WindowPadding = 10*1.5 ----
    printf("\n[2] 基底×scale 絕對值\n");
    {
        ImGuiStyle base, scaled;
        UITheme::Apply(base, UITheme::Id::WarMap);
        UITheme::ApplyScaled(scaled, UITheme::Id::WarMap, 1.5f);
        Check(Nearly(scaled.WindowPadding.x, base.WindowPadding.x * 1.5f),
              "WindowPadding = 基底×1.5");
        Check(Nearly(scaled.ItemSpacing.y, base.ItemSpacing.y * 1.5f),
              "ItemSpacing = 基底×1.5");
        // 換主題後再縮放：不留舊縮放（U-1 粗版 bug 對照）
        UITheme::ApplyScaled(scaled, UITheme::Id::TacticalSim, 1.0f);
        UITheme::Apply(base, UITheme::Id::TacticalSim);
        Check(Nearly(scaled.WindowPadding.x, base.WindowPadding.x),
              "換主題+scale=1 → 完全回基底");
    }

    // ---- [3] ClampScale 邊界 ----
    printf("\n[3] ClampScale\n");
    Check(Nearly(UITheme::ClampScale(0.5f), 0.75f), "0.5 → 0.75");
    Check(Nearly(UITheme::ClampScale(2.0f), 1.5f), "2.0 → 1.5");
    Check(Nearly(UITheme::ClampScale(1.0f), 1.0f), "1.0 不動");
    Check(Nearly(UITheme::Px(300.f, 1.5f), 450.f), "Px(300,1.5)=450");
    Check(Nearly(UITheme::Px(300.f, 9.9f), 450.f), "Px 越界也鉗");

    // ---- [4] UISettings roundtrip ----
    printf("\n[4] UISettings 持久化\n");
    {
        const std::string path = "uisettings_test_tmp.json";
        UISettings::Data d{2, 1.25f};
        Check(UISettings::Save(path, d), "Save 成功");
        UISettings::Data out;
        Check(UISettings::Load(path, out), "Load 成功");
        Check(out.theme == 2 && Nearly(out.uiScale, 1.25f),
              "theme/scale 逐值相符");
        std::error_code ec;
        std::filesystem::remove(path, ec);
    }

    // ---- [5] 越界值鉗制 + 缺檔/壞檔 ----
    printf("\n[5] 邊界與降級\n");
    {
        const std::string path = "uisettings_test_tmp.json";
        {
            std::ofstream f(path, std::ios::trunc);
            f << "{\"schema\":\"potato.settings/1\",\"theme\":3,"
                 "\"uiScale\":9.9}";
        }
        UISettings::Data out;
        Check(UISettings::Load(path, out) &&
                  Nearly(out.uiScale, 1.5f) && out.theme == 3,
              "檔案值越界 → 鉗回 1.5");

        { // 壞檔：schema 不符
            std::ofstream f(path, std::ios::trunc);
            f << "{\"schema\":\"potato.other/9\"}";
        }
        out.theme = 7;
        out.uiScale = 0.9f;
        Check(!UISettings::Load(path, out) && out.theme == 7 &&
                  Nearly(out.uiScale, 0.9f),
              "壞檔拒絕且現況不動");

        std::error_code ec;
        std::filesystem::remove(path, ec);
        out.uiScale = 0.8f;
        Check(!UISettings::Load(path, out) &&
                  Nearly(out.uiScale, 0.8f),
              "缺檔 false 且預設不動");
    }

    ImGui::DestroyContext();
    printf("\n=== 結果: %s ===\n", g_fail == 0 ? "全部 PASS" : "有 FAIL");
    return g_fail == 0 ? 0 : 1;
}
