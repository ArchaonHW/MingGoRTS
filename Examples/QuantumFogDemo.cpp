// QuantumFogDemo - 量子態敵情霧可視化
//
// 敵軍在畫面上不是一個點，而是一團「機率雲」——
// 半透明的紅色疊影散佈在候選位置上，透明度 ∝ 該態機率。
// 花情報點觀測 → 雲塌縮成實心標記（顯示真實位置）；
// 情報時效過後雲重新散開（退相干）。
//
// 右側面板：情報點餘額、逐隊觀測按鈕、時效倒數、上帝視角開關。

#include "Gameplay/QuantumFog.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/BattleController.h"

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <algorithm>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

using namespace Potato;
using namespace Potato::Gameplay;

namespace {

struct DemoState {
    std::unique_ptr<BattleController> battle;
    std::unique_ptr<BattleResources> resources;
    std::unique_ptr<QuantumFog> fog;
    std::vector<Vector2> truePositions;  // 敵軍真實位置（雲背後的真相）
    std::vector<std::string> names;
    bool godView = false;
    std::vector<std::string> log;

    void Log(const std::string& m) {
        log.push_back(m);
        if (log.size() > 50) log.erase(log.begin());
    }

    void Reset() {
        battle = std::make_unique<BattleController>(20, 15, 1.0f);
        resources = std::make_unique<BattleResources>();
        resources->Setup(*battle, /*team=*/0, /*intel=*/10, /*cp=*/3);
        fog = std::make_unique<QuantumFog>(/*intelDuration=*/12.0f, /*cost=*/2);
        fog->BindResources(resources.get());
        log.clear();

        // 兩支敵軍：各有 4 個候選出現點，先驗機率依「人格」傾斜
        // （侵略型敵將的親衛隊較可能靠前線）
        fog->AddEntity("格洛克親衛", 0,
            {Vector2(16, 3), Vector2(15, 7), Vector2(16, 11), Vector2(12, 7)},
            {0.45, 0.30, 0.15, 0.10});
        fog->AddEntity("格洛克伏兵", 0,
            {Vector2(10, 2), Vector2(11, 12), Vector2(14, 5), Vector2(14, 10)},
            {0.20, 0.20, 0.35, 0.25});
        names = {"格洛克親衛", "格洛克伏兵"};
        truePositions = {Vector2(15, 7), Vector2(14, 10)};
        Log("戰場初始化：敵軍以疊加態存在，花情報點觀測塌縮");
    }
};

void RenderControlPanel(DemoState& s, float height) {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(260, height), ImGuiCond_Always);
    ImGui::Begin("量子觀測台", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse);

    ImGui::Text("情報點 Intel: %d", s.resources->GetIntel(0));
    ImGui::TextDisabled("每次觀測花 2 點，時效 12 秒");
    ImGui::Separator();

    for (size_t i = 0; i < s.names.size(); ++i) {
        const int id = static_cast<int>(i);
        const auto* e = s.fog->GetEntity(id);
        if (!e) continue;

        ImGui::PushID(id);
        ImGui::Text("%s", s.names[i].c_str());
        if (e->revealed) {
            ImGui::TextColored(ImVec4(0.4f, 1, 0.6f, 1),
                "已塌縮 (%.0f, %.0f)｜剩 %.1fs",
                e->revealedPos.x, e->revealedPos.y, e->revealTimer);
        } else {
            char btn[64];
            snprintf(btn, sizeof(btn), "觀測 Observe (2 情報)");
            if (ImGui::Button(btn)) {
                if (s.fog->Observe(id, s.truePositions[i])) {
                    s.Log(s.names[i] + " 塌縮至 (" +
                          std::to_string((int)s.truePositions[i].x) + "," +
                          std::to_string((int)s.truePositions[i].y) + ")");
                } else {
                    s.Log("情報不足，觀測失敗");
                }
            }
            auto cloud = s.fog->GetCloud(id);
            for (const auto& c : cloud) {
                ImGui::TextDisabled("  (%.0f,%.0f) %.0f%%",
                    c.first.x, c.first.y, c.second * 100.0);
            }
        }
        ImGui::Separator();
        ImGui::PopID();
    }

    ImGui::Checkbox("上帝視角（顯示真實位置）", &s.godView);
    if (ImGui::Button("重置戰場")) s.Reset();
    ImGui::Separator();
    ImGui::Text("事件記錄");
    for (const auto& m : s.log) ImGui::TextWrapped("%s", m.c_str());
    ImGui::End();
}

void RenderBattlefield(DemoState& s, float left, float width, float height) {
    ImGui::SetNextWindowPos(ImVec2(left, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::Begin("戰場（量子疊加態）", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse);

    const FlowField& field = s.battle->GetField();
    ImVec2 origin = ImGui::GetCursorScreenPos();
    ImVec2 avail = ImGui::GetContentRegionAvail();
    float cell = std::min(avail.x / field.GetWidth(), avail.y / field.GetHeight());
    ImDrawList* dl = ImGui::GetWindowDrawList();

    // 格線
    for (int x = 0; x <= field.GetWidth(); ++x) {
        dl->AddLine(ImVec2(origin.x + x * cell, origin.y),
                    ImVec2(origin.x + x * cell, origin.y + field.GetHeight() * cell),
                    IM_COL32(60, 60, 70, 255));
    }
    for (int y = 0; y <= field.GetHeight(); ++y) {
        dl->AddLine(ImVec2(origin.x, origin.y + y * cell),
                    ImVec2(origin.x + field.GetWidth() * cell, origin.y + y * cell),
                    IM_COL32(60, 60, 70, 255));
    }

    auto cellCenter = [&](const Vector2& c) {
        return ImVec2(origin.x + (c.x + 0.5f) * cell,
                      origin.y + (c.y + 0.5f) * cell);
    };

    // 敵軍機率雲：透明度與半徑隨機率縮放（疊加態的視覺化）
    for (size_t i = 0; i < s.names.size(); ++i) {
        const int id = static_cast<int>(i);
        const auto* e = s.fog->GetEntity(id);
        if (!e) continue;

        if (e->revealed) {
            // 塌縮態：實心標記 + 時效光環
            ImVec2 c = cellCenter(e->revealedPos);
            dl->AddCircleFilled(c, cell * 0.42f, IM_COL32(235, 80, 70, 255));
            float t = e->revealTimer / 12.0f;
            dl->AddCircle(c, cell * 0.55f,
                          IM_COL32(235, 80, 70, (int)(160 * t)), 0, 3.0f);
        } else {
            for (const auto& [pos, prob] : s.fog->GetCloud(id)) {
                ImVec2 c = cellCenter(pos);
                const int alpha = (int)(40 + 160 * prob);
                const float r = cell * (0.15f + 0.35f * (float)prob);
                dl->AddCircleFilled(c, r, IM_COL32(235, 80, 70, alpha));
                dl->AddCircle(c, r, IM_COL32(255, 120, 100, alpha));
            }
        }

        if (s.godView) {
            ImVec2 c = cellCenter(s.truePositions[i]);
            dl->AddCircle(c, cell * 0.3f, IM_COL32(120, 255, 120, 255), 0, 2.0f);
        }
    }

    ImGui::End();
}

void glfwErrorCallback(int error, const char* desc) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, desc);
}

} // namespace

int main() {
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window =
        glfwCreateWindow(1100, 700, "量子敵情霧 Quantum Fog", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImFont* font = io.Fonts->AddFontFromFileTTF(
        "C:/Windows/Fonts/msjh.ttc", 18.0f, nullptr,
        io.Fonts->GetGlyphRangesChineseFull());
    if (!font) io.Fonts->AddFontDefault();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    DemoState s;
    s.Reset();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        s.fog->Update(io.DeltaTime);
        // 真實位置緩慢漂移（觀測只能揭露出觀測當下的位置）
        for (auto& p : s.truePositions) {
            p.x = std::max(1.0f, std::min(18.0f, p.x + 0.15f * io.DeltaTime));
        }

        RenderControlPanel(s, io.DisplaySize.y);
        RenderBattlefield(s, 260.0f, io.DisplaySize.x - 260.0f, io.DisplaySize.y);

        ImGui::Render();
        int dw, dh;
        glfwGetFramebufferSize(window, &dw, &dh);
        glViewport(0, 0, dw, dh);
        glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
