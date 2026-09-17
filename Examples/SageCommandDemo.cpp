// SageCommandDemo - 至聖者指揮畫面（ImGui 五區佈局）
//
//   上：治平儀表（民心 / 秩序 / 勝利型態）
//   左：道權 Tao（墮落監察儀表 + 三徵象，唯讀）
//   中：戰場（ImDrawList 2D：地形/目標/小隊即時位置）
//   右：策權 Ce（正逆六策切換 + 五行輪轉 + 土階段封邪）
//   下：兵權 Bing（小隊列表 + CP 介入 + 時間控制 + 事件記錄）
//
// 畫面流：Deployment（參謀規劃 → 開始執行）→ Execution（策權即時切換）
//         → Resolution（結算勝利型態：勝利 / 無勝而勝 / 失格）
// 所有遊戲邏輯都在 Gameplay/（SageCommand + BattleController），
// 本檔只負責呈現與把按鈕接到切換介面——「指揮 = 切換權，非行動」。

#include "Gameplay/SageCommand.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/BattlePlanner.h"
#include "Gameplay/Squad.h"
#include "Gameplay/FlowField.h"

#define GLFW_INCLUDE_NONE
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

// ---------------------------------------------------------------------------
// 應用狀態：戰鬥 + 指揮核心 + 介面狀態
// ---------------------------------------------------------------------------
struct DemoApp {
    std::unique_ptr<BattleController> battle;
    SageCommand sage;
    BattlePlanner planner;
    bool planned = false;
    std::vector<std::string> log;
    char status[128] = "部署階段：先請參謀規劃，再開始執行";

    void Log(const std::string& msg) {
        log.push_back(msg);
        if (log.size() > 200) {
            log.erase(log.begin());
        }
    }

    // 初始部署直接複用 AutoPlannerDemo 場景：
    // 20x15 斷牆地形（x=10 牆，y 6~8 缺口）、3v2 小隊、互攻目標
    void Reset() {
        battle = std::make_unique<BattleController>(20, 15, 1.0f);
        sage = SageCommand();
        planned = false;
        log.clear();
        snprintf(status, sizeof(status), "%s",
                 "部署階段：先請參謀規劃，再開始執行");

        for (int y = 0; y < 15; ++y) {
            if (y < 6 || y > 8) {
                battle->GetField().SetObstacle(10, y, true);
            }
        }
        battle->SetObjective(0, Vector2(18.0f, 7.0f));
        battle->SetObjective(1, Vector2(1.0f, 7.0f));
        battle->SetRallyPoint(0, Vector2(1.0f, 7.0f));
        battle->SetRallyPoint(1, Vector2(18.0f, 7.0f));

        battle->CreateSquad("Alpha",   0, Vector2(2.0f, 4.0f),  30);
        battle->CreateSquad("Bravo",   0, Vector2(2.0f, 7.0f),  30);
        battle->CreateSquad("Charlie", 0, Vector2(2.0f, 10.0f), 20);
        battle->CreateSquad("Shield",  1, Vector2(16.0f, 7.0f), 35);
        battle->CreateSquad("Spear",   1, Vector2(17.0f, 5.0f), 25);

        battle->SetEventCallback([this](const std::string& m) { Log(m); });
        sage.SetEventCallback([this](const std::string& m) { Log(m); });
    }
};

const char* OrderName(SquadOrder o) {
    switch (o) {
    case SquadOrder::Hold:       return "駐守 Hold";
    case SquadOrder::MoveTo:     return "移動 MoveTo";
    case SquadOrder::AttackMove: return "攻進 AttackMove";
    case SquadOrder::Retreat:    return "撤退 Retreat";
    case SquadOrder::Engage:     return "追擊 Engage";
    }
    return "?";
}

ImU32 TeamColor(int team, bool routing) {
    if (routing) {
        return IM_COL32(150, 150, 150, 255); // 潰逃灰
    }
    return team == 0 ? IM_COL32(80, 140, 255, 255)   // 我軍藍
                     : IM_COL32(235, 80, 70, 255);   // 敵軍紅
}

// ---------------------------------------------------------------------------
// 上區：治平儀表
// ---------------------------------------------------------------------------
void RenderGoverningPanel(DemoApp& app, float width) {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, 96), ImGuiCond_Always);
    ImGui::Begin("治平儀表 Governing", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse);

    ImGui::Text("民心 Support");
    ImGui::SameLine(110);
    ImGui::ProgressBar(app.sage.GetPopularSupport() / 100.0f, ImVec2(180, 0));
    ImGui::SameLine();
    ImGui::Text("秩序 Order");
    ImGui::SameLine(400);
    ImGui::ProgressBar(app.sage.GetCivilOrder() / 100.0f, ImVec2(180, 0));
    ImGui::SameLine();
    ImGui::Text("五行 %s", SageCommand::PhaseName(app.sage.GetWuXingPhase()));

    SageOutcome o = app.sage.GetOutcome();
    ImVec4 oc = o == SageOutcome::SubdueWithoutWar ? ImVec4(0.4f, 1.0f, 0.6f, 1)
              : o == SageOutcome::GovernedPeace   ? ImVec4(1.0f, 0.85f, 0.3f, 1)
              : o == SageOutcome::Fallen          ? ImVec4(1.0f, 0.3f, 0.3f, 1)
              : o == SageOutcome::Victory         ? ImVec4(0.6f, 0.9f, 1.0f, 1)
                                                  : ImVec4(1, 1, 1, 1);
    ImGui::TextColored(oc, "勝利型態：%s", SageCommand::OutcomeName(o));
    ImGui::SameLine();
    ImGui::TextDisabled("｜敵軍 歸附 %d / 殲滅 %d｜無戰:歸附>殲滅 民心≥60｜無勝:敗/平 民心≥70 秩序≥70",
                        app.sage.GetSubduedCount(),
                        app.sage.GetAnnihilatedCount());
    ImGui::End();
}

// ---------------------------------------------------------------------------
// 左區：道權（唯讀監察）
// ---------------------------------------------------------------------------
void RenderTaoPanel(DemoApp& app, float top, float height) {
    ImGui::SetNextWindowPos(ImVec2(0, top), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(250, height), ImGuiCond_Always);
    ImGui::Begin("道權 Tao（唯讀監察）", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse);

    ImGui::Text("墮落值 Corruption");
    float c = app.sage.GetCorruption() / 100.0f;
    ImVec4 bar = c >= 1.0f ? ImVec4(0.9f, 0.1f, 0.1f, 1)
               : c >= 0.8f ? ImVec4(0.9f, 0.4f, 0.1f, 1)
               : c >= 0.6f ? ImVec4(0.9f, 0.8f, 0.1f, 1)
                           : ImVec4(0.5f, 0.3f, 0.7f, 1);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, bar);
    ImGui::ProgressBar(c, ImVec2(-1, 22));
    ImGui::PopStyleColor();

    ImGui::Separator();
    ImGui::Text("墮落徵象 Signs");
    for (int lv = 1; lv <= 3; ++lv) {
        bool on = app.sage.GetSignLevel() >= lv;
        ImGui::TextColored(on ? ImVec4(1, 0.5f, 0.3f, 1)
                              : ImVec4(0.5f, 0.5f, 0.5f, 1),
                           "%s %s", on ? "●" : "○",
                           SageCommand::SignName(lv));
    }

    ImGui::Separator();
    ImGui::Text("封邪：%s", app.sage.IsHereticSealed() ? "封印中" : "未封印");
    if (app.sage.GetOutcome() == SageOutcome::Fallen) {
        ImGui::TextColored(ImVec4(1, 0.2f, 0.2f, 1),
                           "至聖者失格——勝負無效");
    }
    ImGui::TextDisabled("道權只監察，不下命令。");
    ImGui::End();
}

// ---------------------------------------------------------------------------
// 中區：戰場（ImDrawList 2D）
// ---------------------------------------------------------------------------
void RenderBattlefield(DemoApp& app, float left, float top,
                       float width, float height) {
    ImGui::SetNextWindowPos(ImVec2(left, top), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::Begin("戰場 Battlefield", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse);

    BattleController& battle = *app.battle;
    const FlowField& field = battle.GetField();

    ImVec2 origin = ImGui::GetCursorScreenPos();
    ImVec2 avail = ImGui::GetContentRegionAvail();
    float cell = std::min(avail.x / field.GetWidth(),
                          avail.y / field.GetHeight());
    float mapW = cell * field.GetWidth();
    float mapH = cell * field.GetHeight();
    ImDrawList* dl = ImGui::GetWindowDrawList();

    // 底圖與障礙
    dl->AddRectFilled(origin, ImVec2(origin.x + mapW, origin.y + mapH),
                      IM_COL32(28, 30, 34, 255));
    for (int y = 0; y < field.GetHeight(); ++y) {
        for (int x = 0; x < field.GetWidth(); ++x) {
            ImVec2 p0(origin.x + x * cell, origin.y + y * cell);
            ImVec2 p1(p0.x + cell, p0.y + cell);
            if (field.IsBlocked(x, y)) {
                dl->AddRectFilled(p0, p1, IM_COL32(90, 90, 100, 255));
            }
            dl->AddRect(p0, p1, IM_COL32(50, 52, 58, 120));
        }
    }

    // 目標點（菱形）與集結點（方框）
    auto toScreen = [&](const Vector2& w) {
        return ImVec2(origin.x + w.x * cell, origin.y + w.y * cell);
    };
    for (int team = 0; team <= 1; ++team) {
        if (battle.HasObjective(team)) {
            ImVec2 p = toScreen(battle.GetObjective(team));
            float r = cell * 0.4f;
            ImU32 col = team == 0 ? IM_COL32(120, 190, 255, 255)
                                  : IM_COL32(255, 140, 130, 255);
            dl->AddQuadFilled(ImVec2(p.x, p.y - r), ImVec2(p.x + r, p.y),
                              ImVec2(p.x, p.y + r), ImVec2(p.x - r, p.y), col);
        }
        if (battle.HasRallyPoint(team)) {
            ImVec2 p = toScreen(battle.GetRallyPoint(team));
            float r = cell * 0.3f;
            dl->AddRect(ImVec2(p.x - r, p.y - r), ImVec2(p.x + r, p.y + r),
                        IM_COL32(200, 200, 200, 200));
        }
    }

    // 小隊：圓點（大小隨成員）+ 士氣色環 + 名稱
    for (const auto& s : battle.GetSquads()) {
        if (s->IsEliminated()) {
            continue;
        }
        ImVec2 p = toScreen(s->GetPosition());
        float r = cell * (0.35f + 0.30f * s->GetHealthPct());
        ImU32 col = TeamColor(s->GetTeam(), s->IsRouting());
        dl->AddCircleFilled(p, r, col, 20);
        // 士氣環：外圈弧長 = morale
        dl->PathClear();
        dl->PathArcTo(p, r + 3.0f, -1.5708f,
                      -1.5708f + 6.2832f * s->GetMorale(), 20);
        dl->PathStroke(IM_COL32(255, 220, 80, 255), 0, 2.0f);
        dl->AddText(ImVec2(p.x + r + 4, p.y - 7),
                    IM_COL32(230, 230, 230, 255), s->GetName().c_str());
        if (s->IsRouting()) {
            dl->AddText(ImVec2(p.x - r, p.y + r + 2),
                        IM_COL32(255, 200, 60, 255), "ROUT");
        }
    }

    // 結算覆層
    if (battle.GetPhase() == BattlePhase::Resolution ||
        app.sage.GetOutcome() == SageOutcome::Fallen) {
        ImVec2 center(origin.x + mapW * 0.5f, origin.y + mapH * 0.5f);
        dl->AddRectFilled(ImVec2(center.x - 200, center.y - 40),
                          ImVec2(center.x + 200, center.y + 40),
                          IM_COL32(0, 0, 0, 200));
        dl->AddText(ImVec2(center.x - 160, center.y - 10),
                    IM_COL32(255, 235, 140, 255),
                    SageCommand::OutcomeName(app.sage.GetOutcome()));
    }

    ImGui::Dummy(ImVec2(mapW, mapH));
    ImGui::End();
}

// ---------------------------------------------------------------------------
// 右區：策權（唯一的切換介面）
// ---------------------------------------------------------------------------
void RenderCePanel(DemoApp& app, float x, float top, float width,
                   float height) {
    ImGui::SetNextWindowPos(ImVec2(x, top), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::Begin("策權 Ce（切換）", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse);

    BattleController& battle = *app.battle;
    bool resolved = battle.GetPhase() == BattlePhase::Resolution ||
                    app.sage.GetOutcome() != SageOutcome::Ongoing;

    ImGui::Text("五行 %s", SageCommand::PhaseName(app.sage.GetWuXingPhase()));
    if (ImGui::Button("五行輪轉 Advance Phase", ImVec2(-1, 0))) {
        app.sage.AdvancePhase();
    }
    if (app.sage.GetWuXingPhase() == WuXingPhase::MetalWater) {
        ImGui::TextDisabled("金水期：所有策效果減半");
    }
    if (app.sage.GetWuXingPhase() == WuXingPhase::WoodFire) {
        ImGui::TextDisabled("木火期：逆策墮落折扣");
    }

    ImGui::BeginDisabled(app.sage.GetWuXingPhase() != WuXingPhase::Earth ||
                         app.sage.IsHereticSealed());
    if (ImGui::Button("封邪 Seal Heresy（土階段限定）", ImVec2(-1, 0))) {
        app.sage.SealHeresy();
    }
    ImGui::EndDisabled();

    ImGui::Separator();
    ImGui::Text("正六策 Upright");
    for (int i = 0; i <= static_cast<int>(Policy::RallyTroops); ++i) {
        Policy p = static_cast<Policy>(i);
        ImGui::BeginDisabled(resolved);
        if (ImGui::Button(SageCommand::PolicyName(p), ImVec2(-1, 0))) {
            app.sage.ApplyPolicy(p, battle);
        }
        ImGui::EndDisabled();
    }

    ImGui::Separator();
    ImGui::Text("逆六策 Heretic（累墮落）");
    bool hereticBlocked = app.sage.IsHereticSealed() ||
                          app.sage.GetCorruption() >= 100.0f;
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.45f, 0.15f, 0.2f, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4(0.6f, 0.2f, 0.28f, 1));
    for (int i = static_cast<int>(Policy::SlanderEnemy);
         i <= static_cast<int>(Policy::DeceiveHeaven); ++i) {
        Policy p = static_cast<Policy>(i);
        ImGui::BeginDisabled(resolved || hereticBlocked);
        if (ImGui::Button(SageCommand::PolicyName(p), ImVec2(-1, 0))) {
            app.sage.ApplyPolicy(p, battle);
        }
        ImGui::EndDisabled();
    }
    ImGui::PopStyleColor(2);
    if (hereticBlocked) {
        ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1),
                           app.sage.IsHereticSealed() ? "逆策已封印"
                                                      : "墮落已滿");
    }
    ImGui::End();
}

// ---------------------------------------------------------------------------
// 下區：兵權（小隊 / CP 介入 / 時間控制 / 記錄）
// ---------------------------------------------------------------------------
void RenderBingPanel(DemoApp& app, float top, float width, float height) {
    ImGui::SetNextWindowPos(ImVec2(0, top), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::Begin("兵權 Bing（執行層）", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse);

    BattleController& battle = *app.battle;
    BattlePhase phase = battle.GetPhase();

    // 左半：流程控制 + 小隊
    ImGui::BeginChild("bing_left", ImVec2(width * 0.55f, 0), true);
    ImGui::Text("階段：%s",
                phase == BattlePhase::Deployment ? "Deployment 部署"
              : phase == BattlePhase::Execution  ? "Execution 即時"
                                                 : "Resolution 戰後");
    ImGui::SameLine();
    ImGui::TextDisabled("%s", app.status);

    if (phase == BattlePhase::Deployment) {
        if (ImGui::Button("參謀規劃 Generate Plan")) {
            for (int team = 0; team <= 1; ++team) {
                BattlePlanner::Plan plan =
                    app.planner.GeneratePlan(battle, team);
                app.Log(plan.summary);
                for (const auto& sp : plan.squadPlans) {
                    app.Log("  " + sp.squad->GetName() + ": " + sp.rationale);
                }
                app.planner.ApplyPlan(battle, plan);
            }
            app.planned = true;
            snprintf(app.status, sizeof(app.status), "%s",
                     "規劃完成——可按「開始執行」");
        }
        ImGui::SameLine();
        ImGui::BeginDisabled(!app.planned);
        if (ImGui::Button("開始執行 Begin Execution")) {
            if (battle.BeginExecution()) {
                snprintf(app.status, sizeof(app.status), "%s",
                         "即時層：策權可隨時切換，CP 可介入");
            }
        }
        ImGui::EndDisabled();
    } else {
        // 時間控制
        if (ImGui::Button("暫停")) battle.SetTimeScale(0.0f);
        ImGui::SameLine();
        if (ImGui::Button("×1")) battle.SetTimeScale(1.0f);
        ImGui::SameLine();
        if (ImGui::Button("×4")) battle.SetTimeScale(4.0f);
        ImGui::SameLine();
        ImGui::Text("timeScale=%.2f  CP=%d", battle.GetTimeScale(),
                    battle.GetCommandPoints(0));
    }
    ImGui::SameLine();
    if (ImGui::Button("重新部署 Reset")) {
        app.Reset();
        // 本幀到此為止（舊 battle 已銷毀），收尾 ImGui 結構後直接回
        ImGui::EndChild();
        ImGui::End();
        return;
    }

    ImGui::Separator();
    // 小隊表：兵權介入只對我方（team 0）
    for (const auto& s : battle.GetSquads()) {
        if (s->IsEliminated()) {
            ImGui::TextDisabled("%s [殲滅]", s->GetName().c_str());
            continue;
        }
        ImGui::Text("%s T%d %d/%d人 %s%s", s->GetName().c_str(),
                    s->GetTeam(), s->GetMembers(), s->GetMaxMembers(),
                    OrderName(s->GetOrder()),
                    s->IsRouting() ? " [潰逃]" : "");
        ImGui::SameLine(240);
        ImGui::PushID(s->GetName().c_str());
        ImGui::ProgressBar(s->GetMorale(), ImVec2(120, 0));
        if (s->GetTeam() == 0 && phase == BattlePhase::Execution &&
            !s->IsRouting()) {
            ImGui::SameLine();
            if (ImGui::SmallButton("攻進")) {
                battle.Intervene(s.get(), SquadOrder::AttackMove,
                                 battle.GetObjective(0), 5.0f);
            }
            ImGui::SameLine();
            if (ImGui::SmallButton("駐守")) {
                battle.Intervene(s.get(), SquadOrder::Hold,
                                 s->GetPosition(), 5.0f);
            }
            ImGui::SameLine();
            if (ImGui::SmallButton("撤退")) {
                battle.Intervene(s.get(), SquadOrder::Retreat,
                                 battle.GetRallyPoint(0), 5.0f);
            }
        }
        ImGui::PopID();
    }
    ImGui::EndChild();

    // 右半：事件記錄
    ImGui::SameLine();
    ImGui::BeginChild("bing_log", ImVec2(0, 0), true);
    ImGui::Text("事件記錄 Event Log");
    ImGui::Separator();
    for (auto it = app.log.rbegin(); it != app.log.rend(); ++it) {
        ImGui::TextUnformatted(it->c_str());
    }
    ImGui::EndChild();

    ImGui::End();
}

void glfwErrorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

} // namespace

int main() {
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return 1;
    }

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window =
        glfwCreateWindow(1440, 900, "至聖者指揮畫面 Sage Command", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GL loader (glad)\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();

    // 繁體中文字型：找得到微軟正黑體就載入，找不到退回預設字型
    // （UI 字串皆中英並列，缺字型不阻塞功能）
    ImFont* font = io.Fonts->AddFontFromFileTTF(
        "C:/Windows/Fonts/msjh.ttc", 18.0f, nullptr,
        io.Fonts->GetGlyphRangesChineseFull());
    if (font == nullptr) {
        io.Fonts->AddFontDefault();
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    DemoApp app;
    app.Reset();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 邏輯推進：即時層用真實 dt（內部乘 timeScale）；
        // SageCommand.Tick 吃遊戲時間 dt
        float dt = io.DeltaTime;
        if (app.battle->GetPhase() == BattlePhase::Execution) {
            app.battle->Update(dt);
        }
        app.sage.Tick(dt * app.battle->GetTimeScale(), *app.battle);

        float w = io.DisplaySize.x;
        float h = io.DisplaySize.y;
        float top = 96.0f;
        float bottomH = 210.0f;
        float leftW = 250.0f;
        float rightW = 290.0f;
        float midH = h - top - bottomH;

        RenderGoverningPanel(app, w);
        RenderTaoPanel(app, top, midH);
        RenderBattlefield(app, leftW, top, w - leftW - rightW, midH);
        RenderCePanel(app, w - rightW, top, rightW, midH);
        RenderBingPanel(app, h - bottomH, w, bottomH);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
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
