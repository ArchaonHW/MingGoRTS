#pragma once

// U-1 垂直切片：ImGui 主題 token 系統。
// 色值照 _bmad-output/planning-artifacts/ux-designs/ux-MingGoRTS-2026-09-18/
// DESIGN.md §colors token 表——四主題 token 全到齊：
//   tactical-sim 現代軍事 / war-map 泥濘沙盤 /
//   command-room 軍電作戰室 / ink-chronicle 水墨史卷
// （command-room/ink-chronicle 依 Fallback 規則：token 色+rect 幾何，
//   torn edge/筆觸漸層等 sprite 批次另案）。
// 共享語義色（intel/cp/morale/fog）跨主題不變，主題只換皮不換義。

#include "imgui.h"
#include "Rendering/SpriteAtlas.h"

#include <cstdint>
#include <string>

namespace UITheme {

enum class Id {
    TacticalSim = 0, // 現代軍事：低彩度青藍面板、讀盤優先
    WarMap = 1,      // 泥濘沙盤：牛皮紙+手寫墨+紅蠟筆
    CommandRoom = 2, // 軍電作戰室：胡桃木+黃銅+電報紙條
    InkChronicle = 3,// 水墨史卷：宣紙+淡墨+朱砂印
};
inline constexpr int kCount = 4;

inline const char* Name(Id id) {
    switch (id) {
    case Id::WarMap:       return "泥濘沙盤";
    case Id::CommandRoom:  return "軍電作戰室";
    case Id::InkChronicle: return "水墨史卷";
    default:               return "現代軍事";
    }
}

// 主題字體（assets/fonts/ 相對路徑）：serif=手寫/史卷感 / sans=現代讀盤
inline const char* FontRel(Id id) {
    return id == Id::TacticalSim ? "fonts/NotoSansTC-Regular.otf"
                                 : "fonts/NotoSerifTC-Regular.otf";
}

// 3D 場景 clear color（主題氛圍延伸到戰場底色）
inline ImVec4 ClearColor(Id id) {
    switch (id) {
    case Id::WarMap:       return ImVec4(0.14f, 0.12f, 0.07f, 1.0f); // desk #241f13
    case Id::CommandRoom:  return ImVec4(0.05f, 0.03f, 0.02f, 1.0f); // room #0c0805
    case Id::InkChronicle: return ImVec4(0.85f, 0.81f, 0.68f, 1.0f); // scroll-edge #d9cfae
    default:               return ImVec4(0.04f, 0.06f, 0.08f, 1.0f); // bg #0b1014
    }
}

// 機率雲標記色（3D 層共用語義但換色相）
inline ImVec4 CloudColor(Id id) {
    switch (id) {
    case Id::TacticalSim:  return ImVec4(0.21f, 0.82f, 0.77f, 0.85f); // cloud #35d0c5
    case Id::InkChronicle: return ImVec4(0.43f, 0.31f, 0.55f, 0.85f); // cloud #6e508c
    default:               return ImVec4(0.42f, 0.29f, 0.55f, 0.85f); // cloud #6a4a8c
    }
}

inline ImVec4 C(uint32_t rgb, float a = 1.0f) {
    return ImVec4(((rgb >> 16) & 0xff) / 255.0f,
                  ((rgb >> 8) & 0xff) / 255.0f, (rgb & 0xff) / 255.0f, a);
}

// 套用主題到 ImGuiStyle——全部值來自 token 表，不手挑
inline void Apply(ImGuiStyle& s, Id id) {
    ImVec4* c = s.Colors;
    if (id == Id::CommandRoom) {
        // ---- command-room 軍電作戰室 ----
        // 胡桃木房間+黃銅飾件+電報紙條；文字一律 ink on slip（5.2:1+）
        s.WindowRounding = 4.0f;
        s.ChildRounding = 3.0f;
        s.FrameRounding = 2.0f;
        s.PopupRounding = 3.0f;
        s.ScrollbarRounding = 5.0f;
        s.GrabRounding = 3.0f;
        s.WindowBorderSize = 1.0f;
        s.FrameBorderSize = 1.0f;
        s.WindowPadding = ImVec2(12, 9);
        s.FramePadding = ImVec2(8, 4);
        s.ItemSpacing = ImVec2(9, 6);

        c[ImGuiCol_WindowBg] = C(0xe6d9b8, 0.97f);       // slip
        c[ImGuiCol_ChildBg] = C(0xc9b98f, 0.55f);        // slip-dk
        c[ImGuiCol_PopupBg] = C(0xe6d9b8, 0.98f);
        c[ImGuiCol_Border] = C(0x5a4322, 0.80f);         // brass-dk
        c[ImGuiCol_BorderShadow] = C(0x000000, 0.0f);
        c[ImGuiCol_FrameBg] = C(0xc9b98f);               // slip-dk
        c[ImGuiCol_FrameBgHovered] = C(0xd8b878);        // brass-hi
        c[ImGuiCol_FrameBgActive] = C(0x6f5226);         // brass
        c[ImGuiCol_TitleBg] = C(0xc9b98f);               // slip-dk（紙籤感）
        c[ImGuiCol_TitleBgActive] = C(0xd8b878);         // brass-hi
        c[ImGuiCol_TitleBgCollapsed] = C(0xc9b98f, 0.7f);
        c[ImGuiCol_MenuBarBg] = C(0xc9b98f);
        c[ImGuiCol_ScrollbarBg] = C(0xc9b98f, 0.5f);
        c[ImGuiCol_ScrollbarGrab] = C(0x6f5226);         // brass
        c[ImGuiCol_ScrollbarGrabHovered] = C(0xa8322a, 0.55f); // seal
        c[ImGuiCol_ScrollbarGrabActive] = C(0xa8322a);
        c[ImGuiCol_CheckMark] = C(0xa8322a);             // seal 印紅
        c[ImGuiCol_SliderGrab] = C(0x6f5226);
        c[ImGuiCol_SliderGrabActive] = C(0xa8322a);
        c[ImGuiCol_Button] = C(0xc9b98f);                // slip-dk（ink 字 6.4:1）
        c[ImGuiCol_ButtonHovered] = C(0xd8b878);         // brass-hi（ink 字 5:1）
        c[ImGuiCol_ButtonActive] = C(0x6f5226);          // brass（按壓瞬態）
        c[ImGuiCol_Header] = C(0xc9b98f);
        c[ImGuiCol_HeaderHovered] = C(0xd8b878, 0.65f);
        c[ImGuiCol_HeaderActive] = C(0x6f5226, 0.80f);
        c[ImGuiCol_Separator] = C(0x5a4322, 0.55f);      // brass-dk
        c[ImGuiCol_SeparatorHovered] = C(0xa8322a, 0.55f);
        c[ImGuiCol_SeparatorActive] = C(0xa8322a);
        c[ImGuiCol_ResizeGrip] = C(0x6f5226, 0.5f);
        c[ImGuiCol_ResizeGripHovered] = C(0xd8b878, 0.6f);
        c[ImGuiCol_ResizeGripActive] = C(0xa8322a);
        c[ImGuiCol_Tab] = C(0xc9b98f);
        c[ImGuiCol_TabHovered] = C(0xd8b878);
        c[ImGuiCol_TabSelected] = C(0xe6d9b8);
        c[ImGuiCol_TableHeaderBg] = C(0xc9b98f);
        c[ImGuiCol_TableBorderStrong] = C(0x5a4322);
        c[ImGuiCol_TableBorderLight] = C(0x6f5226, 0.6f);
        c[ImGuiCol_TableRowBgAlt] = C(0xc9b98f, 0.35f);
        c[ImGuiCol_Text] = C(0x2e2a20);                  // ink
        c[ImGuiCol_TextDisabled] = C(0x6f5226);          // brass（5.2:1 on slip）
        c[ImGuiCol_TextSelectedBg] = C(0xa8322a, 0.28f);
        c[ImGuiCol_NavCursor] = C(0xa8322a);
        c[ImGuiCol_ModalWindowDimBg] = C(0x0c0805, 0.7f); // room
    } else if (id == Id::InkChronicle) {
        // ---- ink-chronicle 水墨史卷 ----
        // 宣紙+淡墨層次+朱砂；無投影，深度靠墨色濃淡
        s.WindowRounding = 3.0f;
        s.ChildRounding = 2.0f;
        s.FrameRounding = 2.0f;
        s.PopupRounding = 3.0f;
        s.ScrollbarRounding = 4.0f;
        s.GrabRounding = 3.0f;
        s.WindowBorderSize = 1.0f;
        s.FrameBorderSize = 0.0f; // 框線交給 ink-faint border
        s.WindowPadding = ImVec2(14, 10);
        s.FramePadding = ImVec2(8, 4);
        s.ItemSpacing = ImVec2(10, 7);

        c[ImGuiCol_WindowBg] = C(0xefe7d4, 0.96f);       // paper 宣紙
        c[ImGuiCol_ChildBg] = C(0xe8ddc4, 0.50f);        // paper-warm
        c[ImGuiCol_PopupBg] = C(0xefe7d4, 0.98f);
        c[ImGuiCol_Border] = C(0x2e2c26, 0.35f);         // ink-faint
        c[ImGuiCol_BorderShadow] = C(0x000000, 0.0f);
        c[ImGuiCol_FrameBg] = C(0xe8ddc4);               // paper-warm
        c[ImGuiCol_FrameBgHovered] = C(0xf4edda);        // slip
        c[ImGuiCol_FrameBgActive] = C(0xa33b28, 0.35f);  // vermilion 淡朱
        c[ImGuiCol_TitleBg] = C(0xe8ddc4);
        c[ImGuiCol_TitleBgActive] = C(0xf4edda);
        c[ImGuiCol_TitleBgCollapsed] = C(0xe8ddc4, 0.7f);
        c[ImGuiCol_MenuBarBg] = C(0xe8ddc4);
        c[ImGuiCol_ScrollbarBg] = C(0xe8ddc4, 0.4f);
        c[ImGuiCol_ScrollbarGrab] = C(0x55524a, 0.55f);  // ink-soft
        c[ImGuiCol_ScrollbarGrabHovered] = C(0xa33b28, 0.55f); // vermilion
        c[ImGuiCol_ScrollbarGrabActive] = C(0xa33b28);
        c[ImGuiCol_CheckMark] = C(0xa33b28);             // vermilion 朱砂
        c[ImGuiCol_SliderGrab] = C(0x55524a);
        c[ImGuiCol_SliderGrabActive] = C(0xa33b28);
        c[ImGuiCol_Button] = C(0xf4edda);                // slip（ink 字 ~9:1）
        c[ImGuiCol_ButtonHovered] = C(0xa33b28, 0.25f);  // 淡朱暈
        c[ImGuiCol_ButtonActive] = C(0xa33b28, 0.55f);
        c[ImGuiCol_Header] = C(0xe8ddc4);
        c[ImGuiCol_HeaderHovered] = C(0xa33b28, 0.20f);
        c[ImGuiCol_HeaderActive] = C(0xa33b28, 0.40f);
        c[ImGuiCol_Separator] = C(0x2e2c26, 0.30f);      // ink-faint 筆觸分隔
        c[ImGuiCol_SeparatorHovered] = C(0xa33b28, 0.5f);
        c[ImGuiCol_SeparatorActive] = C(0xa33b28);
        c[ImGuiCol_ResizeGrip] = C(0x55524a, 0.4f);
        c[ImGuiCol_ResizeGripHovered] = C(0xa33b28, 0.4f);
        c[ImGuiCol_ResizeGripActive] = C(0xa33b28);
        c[ImGuiCol_Tab] = C(0xe8ddc4);
        c[ImGuiCol_TabHovered] = C(0xa33b28, 0.25f);
        c[ImGuiCol_TabSelected] = C(0xf4edda);
        c[ImGuiCol_TableHeaderBg] = C(0xe8ddc4);
        c[ImGuiCol_TableBorderStrong] = C(0x55524a, 0.6f);
        c[ImGuiCol_TableBorderLight] = C(0x55524a, 0.3f);
        c[ImGuiCol_TableRowBgAlt] = C(0x55524a, 0.06f);
        c[ImGuiCol_Text] = C(0x2e2c26);                  // ink（on paper ≈12:1）
        c[ImGuiCol_TextDisabled] = C(0x55524a);          // ink-soft
        c[ImGuiCol_TextSelectedBg] = C(0xa33b28, 0.22f);
        c[ImGuiCol_NavCursor] = C(0xa33b28);
        c[ImGuiCol_ModalWindowDimBg] = C(0x2e2c26, 0.35f); // 淡墨罩
    } else if (id == Id::TacticalSim) {
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

        c[ImGuiCol_WindowBg] = C(0xd3c096, 0.97f);       // paper（泥濘底紙）
        c[ImGuiCol_ChildBg] = C(0xe6d9b0, 0.45f);        // paper-hi（浮起層）
        c[ImGuiCol_PopupBg] = C(0xe6d9b0, 0.98f);        // paper-hi
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

// ---- 共用 UI 詞彙 helper（DESIGN.md §Components/§Do's and Don'ts）----

// UTF-8 單字元位元組長度（1~4；不合法前導位元當 1 前進防死迴圈）
inline int NextUtf8Len(const char* p) {
    const unsigned char b = static_cast<unsigned char>(*p);
    if (b < 0x80) return 1;
    if ((b & 0xE0) == 0xC0) return 2;
    if ((b & 0xF0) == 0xE0) return 3;
    if ((b & 0xF8) == 0xF0) return 4;
    return 1;
}

// CJK 直書：逐字垂直排列（標題/印章用；DESIGN 禁用於 Latin/數字段）
// 自由定位版：可畫在視窗外（foreground/background draw list）
inline void VTextAt(ImDrawList* dl, ImVec2 pos, const char* utf8, ImU32 col) {
    if (!dl || !utf8 || !*utf8) return;
    ImFont* font = ImGui::GetFont();
    const float fs = ImGui::GetFontSize();
    const float adv = ImGui::GetTextLineHeightWithSpacing();
    float cy = pos.y;
    for (const char* p = utf8; *p; ) {
        const int len = NextUtf8Len(p);
        dl->AddText(font, fs, ImVec2(pos.x, cy), col, p, p + len);
        cy += adv;
        p += len;
    }
}
// 版面流版：在游標處直排並佔位
inline void VText(const char* utf8) {
    if (!utf8 || !*utf8) return;
    const ImVec2 pos = ImGui::GetCursorScreenPos();
    VTextAt(ImGui::GetWindowDrawList(), pos, utf8,
            ImGui::GetColorU32(ImGuiCol_Text));
    int chars = 0;
    for (const char* p = utf8; *p; p += NextUtf8Len(p)) ++chars;
    ImGui::Dummy(ImVec2(ImGui::GetFontSize(),
                        chars * ImGui::GetTextLineHeightWithSpacing()));
}

// 形狀編碼標記（minimap/概圖）：陣營絕不單獨用色相區分——
// 我方=方、敵方=菱、機率雲=圓（DESIGN §Minimap 強制規範）
enum class MarkerShape { FriendlySquare, EnemyDiamond, CloudCircle };
inline void DrawMarker(ImDrawList* dl, ImVec2 center, float r,
                       MarkerShape shape, ImU32 col, float thickness = 1.5f) {
    if (!dl || r <= 0.0f) return;
    switch (shape) {
    case MarkerShape::FriendlySquare:
        dl->AddRect(ImVec2(center.x - r, center.y - r),
                    ImVec2(center.x + r, center.y + r), col, 0.0f,
                    0, thickness);
        break;
    case MarkerShape::EnemyDiamond:
        dl->AddQuad(ImVec2(center.x, center.y - r),
                    ImVec2(center.x + r, center.y),
                    ImVec2(center.x, center.y + r),
                    ImVec2(center.x - r, center.y), col, thickness);
        break;
    case MarkerShape::CloudCircle:
        dl->AddCircle(center, r, col, 0, thickness);
        dl->AddCircleFilled(center, r * 0.45f, col);
        break;
    }
}

// 圖集標記(F-1 消費端)：atlas 命中且有 tex → AddImage(uv rect)；
// 缺名/tex 缺席 → 退回 DrawMarker 形狀編碼（降級保留語義）。
// 回傳 true = 走了 sprite 路徑。
inline bool DrawAtlasMarker(ImDrawList* dl, ImVec2 center, float r,
                            const Potato::SpriteAtlas& atlas,
                            ImTextureID tex, const char* frameName,
                            MarkerShape fallback, ImU32 col,
                            float thickness = 1.5f) {
    if (!dl || r <= 0.0f) return false;
    const Potato::SpriteFrame* f =
        frameName ? atlas.Find(frameName) : nullptr;
    if (f && tex) {
        const ImVec2 p0(center.x - r, center.y - r);
        const ImVec2 p1(center.x + r, center.y + r);
        dl->AddImage(tex, p0, p1, ImVec2(f->u0, f->v0),
                     ImVec2(f->u1, f->v1), col);
        return true;
    }
    DrawMarker(dl, center, r, fallback, col, thickness);
    return false;
}

// 固定欄寬右對齊：倒計時/計量數字在保留欄位內向左伸展,
// 位數變動不推移右緣（DESIGN 計時器數字規範——比例字體下
// 不改逐字 advance,改以欄位錨定達成同效果）
inline void FixedField(const char* text, float width) {
    const float tw = ImGui::CalcTextSize(text).x;
    const float pad = width - tw;
    if (pad > 0.0f) {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + pad);
    }
    ImGui::TextUnformatted(text);
}

// ---- F-2 UI scale 真管線 ----
// 範圍 0.75–1.5；ClampScale 是滑桿與存檔載入共用的單一守衛
inline constexpr float kScaleMin = 0.75f;
inline constexpr float kScaleMax = 1.5f;
inline float ClampScale(float s) {
    if (s < kScaleMin) return kScaleMin;
    if (s > kScaleMax) return kScaleMax;
    return s;
}

// 絕對重建：Apply 基底 token → ScaleAllSizes(scale)。
// 不做比值累乘——同參數恆同 style，換主題也不殘留舊縮放。
inline void ApplyScaled(ImGuiStyle& s, Id id, float scale) {
    Apply(s, id);
    s.ScaleAllSizes(ClampScale(scale));
}

// 面板/子窗/按鈕寫死尺寸的 scale 閘門。
// 錨點位置不經此（UX-DR4）——右緣面板用 `ww - Px(w+8, s)` 保持貼齊。
inline float Px(float base, float scale) {
    return base * ClampScale(scale);
}

} // namespace UITheme
