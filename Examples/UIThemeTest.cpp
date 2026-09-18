// UIThemeTest - 主題 token 品質門檻（無頭）
//
// 把 DESIGN.md 的硬規範變成自動檢查：
//   - 主要文字對比 ≥4.5:1（WCAG 衍生，各主題 Text on 有效 WindowBg）
//   - 次級文字仍須可讀（gloss 層 ≥3:1——規格只禁止它承載關鍵資訊）
//   - hover/啟用態必須與靜止態可分辨（可及性 affordance）
//   - 選取游標色不等於文字色
//   - 四主題 WindowBg 兩兩可辨（主題真的不同，不只是改名）
//   - UTF-8 步進器與主題列舉覆蓋

#include "UITheme.h"
#include "imgui.h"

#include <cmath>
#include <cstdio>
#include <cstring>

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* name) {
    if (ok) { ++g_pass; printf("  [PASS] %s\n", name); }
    else    { ++g_fail; printf("  [FAIL] %s\n", name); }
}

// WCAG 相對亮度（sRGB → linear）
static float Luminance(const ImVec4& c) {
    auto lin = [](float v) {
        return v <= 0.03928f ? v / 12.92f
                             : std::pow((v + 0.055f) / 1.055f, 2.4f);
    };
    return 0.2126f * lin(c.x) + 0.7152f * lin(c.y) + 0.0722f * lin(c.z);
}
static float Contrast(const ImVec4& a, const ImVec4& b) {
    const float la = Luminance(a), lb = Luminance(b);
    return (la > lb ? (la + 0.05f) / (lb + 0.05f)
                    : (lb + 0.05f) / (la + 0.05f));
}
// WindowBg 有 alpha → 合成在該主題 ClearColor 上才是實際底色
static ImVec4 EffectiveBg(const ImVec4& bg, const ImVec4& clear) {
    return ImVec4(bg.x * bg.w + clear.x * (1.0f - bg.w),
                  bg.y * bg.w + clear.y * (1.0f - bg.w),
                  bg.z * bg.w + clear.z * (1.0f - bg.w), 1.0f);
}
static float ColorDist(const ImVec4& a, const ImVec4& b) {
    const float dx = a.x - b.x, dy = a.y - b.y, dz = a.z - b.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

int main() {
    printf("=== UITheme Token Gate ===\n\n");

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(1920, 1080);

    const UITheme::Id ids[4] = {
        UITheme::Id::TacticalSim, UITheme::Id::WarMap,
        UITheme::Id::CommandRoom, UITheme::Id::InkChronicle};

    ImVec4 winBgs[4];
    for (int i = 0; i < UITheme::kCount; ++i) {
        const UITheme::Id id = ids[i];
        printf("[%s]\n", UITheme::Name(id));

        ImGuiStyle style;
        UITheme::Apply(style, id);
        const ImVec4* c = style.Colors;
        const ImVec4 effBg =
            EffectiveBg(c[ImGuiCol_WindowBg], UITheme::ClearColor(id));
        winBgs[i] = effBg;

        char label[96];
        const float tContrast = Contrast(c[ImGuiCol_Text], effBg);
        std::snprintf(label, sizeof(label),
                      "Text/WindowBg 對比 %.1f:1 ≥4.5", tContrast);
        Check(tContrast >= 4.5f, label);

        const float dContrast = Contrast(c[ImGuiCol_TextDisabled], effBg);
        std::snprintf(label, sizeof(label),
                      "TextDisabled 對比 %.1f:1 ≥3.0", dContrast);
        Check(dContrast >= 3.0f, label);

        Check(ColorDist(c[ImGuiCol_Button], c[ImGuiCol_ButtonHovered])
                  > 0.02f,
              "Button hover 可分辨");
        Check(ColorDist(c[ImGuiCol_Header], c[ImGuiCol_HeaderHovered])
                  > 0.02f,
              "Header hover 可分辨");
        Check(ColorDist(c[ImGuiCol_FrameBg], c[ImGuiCol_FrameBgHovered])
                  > 0.02f,
              "Frame hover 可分辨");
        Check(ColorDist(c[ImGuiCol_NavCursor], c[ImGuiCol_Text]) > 0.02f,
              "選取游標色 ≠ 文字色");
        Check(Contrast(c[ImGuiCol_CheckMark], c[ImGuiCol_FrameBg]) >= 1.5f,
              "CheckMark 在 FrameBg 上可見");

        Check(UITheme::FontRel(id) != nullptr, "FontRel 有值");
        Check(UITheme::CloudColor(id).w > 0.0f, "CloudColor 有效");
    }

    // 字體映射：tactical-sim=sans、其餘=serif（DESIGN §Typography）
    Check(std::strstr(UITheme::FontRel(UITheme::Id::TacticalSim), "Sans"),
          "tactical-sim 用 sans");
    Check(std::strstr(UITheme::FontRel(UITheme::Id::WarMap), "Serif") &&
              std::strstr(UITheme::FontRel(UITheme::Id::CommandRoom),
                          "Serif") &&
              std::strstr(UITheme::FontRel(UITheme::Id::InkChronicle),
                          "Serif"),
          "紙本三主題用 serif");

    // 四主題底色兩兩可辨
    bool distinct = true;
    for (int i = 0; i < 4 && distinct; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            if (ColorDist(winBgs[i], winBgs[j]) < 0.05f) distinct = false;
        }
    }
    Check(distinct, "四主題 WindowBg 兩兩可辨");

    // UTF-8 步進器
    Check(UITheme::NextUtf8Len("A") == 1, "ASCII=1");
    Check(UITheme::NextUtf8Len("中") == 3, "CJK=3");
    Check(UITheme::NextUtf8Len("\xF0\x9F\x8E\xAE") == 4, "4-byte=4");
    Check(UITheme::NextUtf8Len("\x80") == 1, "非法前導=1（防死迴圈）");

    ImGui::DestroyContext();
    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
