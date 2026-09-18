#pragma once

// U-1 垂直切片：ImGui 主題 token 系統。
// 色值照 _bmad-output/planning-artifacts/ux-designs/ux-MingGoRTS-2026-09-18/
// DESIGN.md §colors token 表——tactical-sim 現代軍事 / war-map 泥濘沙盤
// （先行兩主題；command-room / ink-chronicle 待 sprite 資產批次）。
// 共享語義色（intel/cp/morale/fog）跨主題不變，主題只換皮不換義。

#include "imgui.h"

#include <cstdint>
#include <string>

namespace UITheme {

enum class Id {
    TacticalSim = 0, // 現代軍事：低彩度青藍面板、讀盤優先
    WarMap = 1,      // 泥濘沙盤：牛皮紙+手寫墨+紅蠟筆
};

inline const char* Name(Id id) {
    return id == Id::WarMap ? "泥濘沙盤" : "現代軍事";
}

// 主題字體（assets/fonts/ 相對路徑）：serif=沙盤手寫感 / sans=現代讀盤
inline const char* FontRel(Id id) {
    return id == Id::WarMap ? "fonts/NotoSerifTC-Regular.otf"
                            : "fonts/NotoSansTC-Regular.otf";
}

// 3D 場景 clear color（主題氛圍延伸到戰場底色）
inline ImVec4 ClearColor(Id id) {
    return id == Id::WarMap ? ImVec4(0.14f, 0.12f, 0.07f, 1.0f)  // desk #241f13
                            : ImVec4(0.04f, 0.06f, 0.08f, 1.0f); // bg #0b1014
}

// 機率雲標記色（3D 層共用語義但換色相）
inline ImVec4 CloudColor(Id id) {
    return id == Id::WarMap ? ImVec4(0.42f, 0.29f, 0.55f, 0.85f) // cloud #6a4a8c
                            : ImVec4(0.21f, 0.82f, 0.77f, 0.85f); // cloud #35d0c5
}

inline ImVec4 C(uint32_t rgb, float a = 1.0f) {
    return ImVec4(((rgb >> 16) & 0xff) / 255.0f,
                  ((rgb >> 8) & 0xff) / 255.0f, (rgb & 0xff) / 255.0f, a);
}

// 套用主題到 ImGuiStyle——全部值來自 token 表，不手挑
inline void Apply(ImGuiStyle& s, Id id) {
    ImVec4* c = s.Colors;
    if (id == Id::TacticalSim) {
        // ---- tactical-sim 現代軍事 ----
        s.WindowRounding = 2.0f;
        s.ChildRounding = 2.0f;
        s.FrameRounding = 2.0f;
        s.PopupRounding = 2.0f;
        s.ScrollbarRounding = 2.0f;
        s.GrabRounding = 2.0f;
        s.WindowBorderSize = 1.0f;
        s.FrameBorderSize = 1.0f;
        s.WindowPadding = ImVec2(8, 6);
        s.FramePadding = ImVec2(6, 3);
        s.ItemSpacing = ImVec2(8, 4);

        c[ImGuiCol_WindowBg] = C(0x0d141a, 0.94f);       // panel
        c[ImGuiCol_ChildBg] = C(0x10181e, 0.90f);        // terrain
        c[ImGuiCol_PopupBg] = C(0x0d141a, 0.97f);
        c[ImGuiCol_Border] = C(0x24333d);                // hairline
        c[ImGuiCol_BorderShadow] = C(0x000000, 0.0f);
        c[ImGuiCol_FrameBg] = C(0x10181e);               // terrain
        c[ImGuiCol_FrameBgHovered] = C(0x2e4450);        // hairline2
        c[ImGuiCol_FrameBgActive] = C(0x24333d);
        c[ImGuiCol_TitleBg] = C(0x0b1014);               // bg
        c[ImGuiCol_TitleBgActive] = C(0x10181e);
        c[ImGuiCol_TitleBgCollapsed] = C(0x0b1014, 0.7f);
        c[ImGuiCol_MenuBarBg] = C(0x0d141a);
        c[ImGuiCol_ScrollbarBg] = C(0x0b1014, 0.6f);
        c[ImGuiCol_ScrollbarGrab] = C(0x2e4450);
        c[ImGuiCol_ScrollbarGrabHovered] = C(0x2fd4c4, 0.55f); // teal-dim
        c[ImGuiCol_ScrollbarGrabActive] = C(0x2fd4c4);   // teal
        c[ImGuiCol_CheckMark] = C(0x2fd4c4);             // teal
        c[ImGuiCol_SliderGrab] = C(0x2fd4c4, 0.55f);
        c[ImGuiCol_SliderGrabActive] = C(0x2fd4c4);
        c[ImGuiCol_Button] = C(0x2e4450);                // hairline2
        c[ImGuiCol_ButtonHovered] = C(0x2fd4c4, 0.35f);  // teal tint
        c[ImGuiCol_ButtonActive] = C(0x2fd4c4, 0.60f);
        c[ImGuiCol_Header] = C(0x24333d);
        c[ImGuiCol_HeaderHovered] = C(0x2fd4c4, 0.35f);
        c[ImGuiCol_HeaderActive] = C(0x2fd4c4, 0.55f);
        c[ImGuiCol_Separator] = C(0x24333d);
        c[ImGuiCol_SeparatorHovered] = C(0x2fd4c4, 0.55f);
        c[ImGuiCol_SeparatorActive] = C(0x2fd4c4);
        c[ImGuiCol_ResizeGrip] = C(0x2e4450, 0.4f);
        c[ImGuiCol_ResizeGripHovered] = C(0x2fd4c4, 0.5f);
        c[ImGuiCol_ResizeGripActive] = C(0x2fd4c4);
        c[ImGuiCol_Tab] = C(0x10181e);
        c[ImGuiCol_TabHovered] = C(0x2fd4c4, 0.4f);
        c[ImGuiCol_TabSelected] = C(0x24333d);
        c[ImGuiCol_TableHeaderBg] = C(0x10181e);
        c[ImGuiCol_TableBorderStrong] = C(0x2e4450);
        c[ImGuiCol_TableBorderLight] = C(0x24333d);
        c[ImGuiCol_TableRowBgAlt] = C(0x10181e, 0.4f);
        c[ImGuiCol_Text] = C(0xd9e6ea);                  // text
        c[ImGuiCol_TextDisabled] = C(0x7a919c);          // faint
        c[ImGuiCol_TextSelectedBg] = C(0x2fd4c4, 0.3f);
        c[ImGuiCol_NavCursor] = C(0x2fd4c4);
        c[ImGuiCol_ModalWindowDimBg] = C(0x05080b, 0.7f); // page
    } else {
        // ---- war-map 泥濘沙盤 ----
        s.WindowRounding = 4.0f;
        s.ChildRounding = 3.0f;
        s.FrameRounding = 3.0f;
        s.PopupRounding = 3.0f;
        s.ScrollbarRounding = 6.0f;
        s.GrabRounding = 3.0f;
        s.WindowBorderSize = 1.0f;
        s.FrameBorderSize = 1.0f;
        s.WindowPadding = ImVec2(10, 8);
        s.FramePadding = ImVec2(8, 4);
        s.ItemSpacing = ImVec2(8, 6);

        c[ImGuiCol_WindowBg] = C(0xe6d9b0, 0.97f);       // paper-hi
        c[ImGuiCol_ChildBg] = C(0xd3c096, 0.60f);        // paper
        c[ImGuiCol_PopupBg] = C(0xe6d9b0, 0.98f);
        c[ImGuiCol_Border] = C(0x37301f, 0.45f);         // ink
        c[ImGuiCol_BorderShadow] = C(0x000000, 0.0f);
        c[ImGuiCol_FrameBg] = C(0xd3c096);               // paper
        c[ImGuiCol_FrameBgHovered] = C(0xb3a172);        // paper-dk
        c[ImGuiCol_FrameBgActive] = C(0x8a7a58);         // mud
        c[ImGuiCol_TitleBg] = C(0xb3a172);               // paper-dk
        c[ImGuiCol_TitleBgActive] = C(0x8a7a58);         // mud
        c[ImGuiCol_TitleBgCollapsed] = C(0xb3a172, 0.7f);
        c[ImGuiCol_MenuBarBg] = C(0xd3c096);
        c[ImGuiCol_ScrollbarBg] = C(0xd3c096, 0.5f);
        c[ImGuiCol_ScrollbarGrab] = C(0x8a7a58);
        c[ImGuiCol_ScrollbarGrabHovered] = C(0x8f2d20, 0.55f); // grease
        c[ImGuiCol_ScrollbarGrabActive] = C(0x8f2d20);
        c[ImGuiCol_CheckMark] = C(0x8f2d20);             // grease
        c[ImGuiCol_SliderGrab] = C(0x8a7a58);
        c[ImGuiCol_SliderGrabActive] = C(0x8f2d20);
        c[ImGuiCol_Button] = C(0xb3a172);                // paper-dk
        c[ImGuiCol_ButtonHovered] = C(0x8f2d20, 0.45f);  // grease tint
        c[ImGuiCol_ButtonActive] = C(0x8f2d20, 0.70f);
        c[ImGuiCol_Header] = C(0xb3a172);
        c[ImGuiCol_HeaderHovered] = C(0x8f2d20, 0.40f);
        c[ImGuiCol_HeaderActive] = C(0x8f2d20, 0.60f);
        c[ImGuiCol_Separator] = C(0x4a4234, 0.5f);       // pencil
        c[ImGuiCol_SeparatorHovered] = C(0x8f2d20, 0.55f);
        c[ImGuiCol_SeparatorActive] = C(0x8f2d20);
        c[ImGuiCol_ResizeGrip] = C(0x8a7a58, 0.5f);
        c[ImGuiCol_ResizeGripHovered] = C(0x8f2d20, 0.5f);
        c[ImGuiCol_ResizeGripActive] = C(0x8f2d20);
        c[ImGuiCol_Tab] = C(0xd3c096);
        c[ImGuiCol_TabHovered] = C(0x8f2d20, 0.45f);
        c[ImGuiCol_TabSelected] = C(0xb3a172);
        c[ImGuiCol_TableHeaderBg] = C(0xd3c096);
        c[ImGuiCol_TableBorderStrong] = C(0x8a7a58);
        c[ImGuiCol_TableBorderLight] = C(0xb3a172);
        c[ImGuiCol_TableRowBgAlt] = C(0x8a7a58, 0.15f);
        c[ImGuiCol_Text] = C(0x37301f);                  // ink
        c[ImGuiCol_TextDisabled] = C(0x4a4234);          // pencil
        c[ImGuiCol_TextSelectedBg] = C(0x8f2d20, 0.28f);
        c[ImGuiCol_NavCursor] = C(0x8f2d20);
        c[ImGuiCol_ModalWindowDimBg] = C(0x241f13, 0.7f); // desk
    }
}

// 載入主題字體：先載 ASCII 預設範圍，再 MergeMode 補中文全集；
// 回傳 nullptr 表示檔案缺失/損毀（呼叫方退回內建字體）。
inline ImFont* LoadFont(ImGuiIO& io, const std::string& path, float size) {
    ImFont* font = io.Fonts->AddFontFromFileTTF(path.c_str(), size);
    if (!font) return nullptr;
    ImFontConfig cfg;
    cfg.MergeMode = true;
    io.Fonts->AddFontFromFileTTF(path.c_str(), size, &cfg,
                                 io.Fonts->GetGlyphRangesChineseFull());
    return font;
}

} // namespace UITheme
