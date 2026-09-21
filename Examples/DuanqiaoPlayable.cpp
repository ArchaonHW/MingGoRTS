// DuanqiaoPlayable - 斷橋可玩 demo(E4-3 + A-4)
//
// 管線: BattleMap(duanqiao.json) → BattleController(doctrine 執行)
//       → BattleSceneSync(小隊節點+選取環+血條) → SceneRenderer → OpenGL
// 操作: 左鍵選取我軍小隊 / 右鍵地面=AttackMove、右鍵敵軍=Engage(皆走 CP 介入)
//       左鍵點敵情機率雲=探測(1情報,雲收縮) / Shift+左鍵點雲=觀測(2情報,塌縮)
//       Space 暫停 / 1,2,3 倍速
//       WASD 平移視角 / 滾輪縮放 / Esc 取消選取
// 無顯示環境: 印 [SKIP] 並以 0 結束(不進 POTATO_TESTS)

#include "Rendering/OpenGLRenderer.h"
#include "Rendering/SceneRenderer.h"
#include "Rendering/RenderableComponent.h"
#include "Rendering/Camera.h"
#include "Scene/SceneNode.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/BattleSceneSync.h"
#include "Gameplay/BattlePicker.h"
#include "Gameplay/BattleMap.h"
#include "Gameplay/GovernanceField.h"
#include "Gameplay/BattlePlanner.h"
#include "Gameplay/PlanningDeck.h"
#include "Gameplay/BattlePlan.h"
#include "Gameplay/EnemyGeneral.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/BattleRecorder.h"
#include "Gameplay/Roster.h"
#include "Gameplay/PostBattle.h"
#include "Campaign/CampaignState.h"
#include "Campaign/ChapterLibrary.h"
#include "Gameplay/HistorianReport.h"
#include "Gameplay/GeneralDossier.h"
#include "Gameplay/RefitCamp.h"
#include "Gameplay/SquadTemplate.h"
#include "Gameplay/QuantumFog.h"
#include "MathUtils/CurlNoise.h"
#include "MathUtils/GustField.h"
#include "MathUtils/Matrix4.h"
#include "DemoAssets.h"
#include "UITheme.h"
#include "UISettings.h"

#include <glad/glad.h>
#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE // 防止 GLFW 引入系統 gl.h 與 glad 衝突
#endif
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <deque>
#include <filesystem>
#include <string>
#include <vector>

using namespace Potato;
using namespace Potato::Gameplay;

// ---- 簡易 lambert shader(uniform 名對齊 SceneRenderer::SubmitRenderList)----
static const char* UNIT_VERT = R"(
#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 vNormal;
void main() {
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

static const char* UNIT_FRAG = R"(
#version 330 core
in vec3 vNormal;
uniform vec3 uColor;
uniform vec3 uLightDir;
out vec4 FragColor;
void main() {
    vec3 n = normalize(vNormal);
    float diff = max(dot(n, normalize(uLightDir)), 0.0);
    vec3 col = uColor * (0.30 + 0.75 * diff);
    FragColor = vec4(col, 1.0);
}
)";

// ---- 程序化 mesh ----

static Mesh MakeBox(float w, float h, float d) {
    Mesh m;
    float x = w * 0.5f, y = h * 0.5f, z = d * 0.5f;
    Vector3 n[6] = {{0,0,1},{0,0,-1},{1,0,0},{-1,0,0},{0,1,0},{0,-1,0}};
    Vector3 quad[6][4] = {
        {{-x,-y,z},{x,-y,z},{x,y,z},{-x,y,z}},
        {{x,-y,-z},{-x,-y,-z},{-x,y,-z},{x,y,-z}},
        {{x,-y,z},{x,-y,-z},{x,y,-z},{x,y,z}},
        {{-x,-y,-z},{-x,-y,z},{-x,y,z},{-x,y,-z}},
        {{-x,y,z},{x,y,z},{x,y,-z},{-x,y,-z}},
        {{-x,-y,-z},{x,-y,-z},{x,-y,z},{-x,-y,z}},
    };
    std::vector<Vertex> verts;
    std::vector<uint32> idx;
    for (int f = 0; f < 6; f++) {
        uint32 base = (uint32)verts.size();
        for (int v = 0; v < 4; v++) {
            Vertex vx;
            vx.position = quad[f][v];
            vx.normal = n[f];
            vx.texCoord = Vector2(0, 0);
            verts.push_back(vx);
        }
        idx.insert(idx.end(), {base, base+1, base+2, base, base+2, base+3});
    }
    m.SetVertices(verts);
    m.SetIndices(idx);
    return m;
}

// 膠囊體:車削(profile = 下半球 + 圓柱 + 上半球)
static Mesh MakeCapsule(float radius, float height, int seg = 12, int rings = 6) {
    Mesh m;
    float cylHalf = height * 0.5f - radius; // 圓柱半高
    if (cylHalf < 0.0f) cylHalf = 0.0f;
    // profile: (r, y, ny, nr) 從底部頂點到頂部頂點
    std::vector<Vector3> profile;   // x=r, y=y
    std::vector<Vector3> pNormal;   // profile 法線(y 分量與徑向分量)
    for (int i = 0; i <= rings; ++i) { // 下半球 -90°..0°
        float t = -1.5707963f + 1.5707963f * i / rings;
        profile.push_back(Vector3(radius * std::cos(t), -cylHalf + radius * std::sin(t), 0));
        pNormal.push_back(Vector3(std::cos(t), std::sin(t), 0));
    }
    for (int i = 0; i <= rings; ++i) { // 上半球 0°..90°
        float t = 1.5707963f * i / rings;
        profile.push_back(Vector3(radius * std::cos(t), cylHalf + radius * std::sin(t), 0));
        pNormal.push_back(Vector3(std::cos(t), std::sin(t), 0));
    }
    int rows = (int)profile.size();
    std::vector<Vertex> verts;
    std::vector<uint32> idx;
    for (int r = 0; r < rows; ++r) {
        for (int s = 0; s <= seg; ++s) {
            float a = 6.2831853f * s / seg;
            float ca = std::cos(a), sa = std::sin(a);
            Vertex v;
            // 膠囊底部落在 y=0,直立在地面(squad 節點在 y=0)
            v.position = Vector3(profile[r].x * ca,
                                 profile[r].y + height * 0.5f,
                                 profile[r].x * sa);
            v.normal = Vector3(pNormal[r].x * ca, pNormal[r].y, pNormal[r].x * sa);
            v.texCoord = Vector2((float)s / seg, (float)r / (rows - 1));
            verts.push_back(v);
        }
    }
    for (int r = 0; r + 1 < rows; ++r) {
        for (int s = 0; s < seg; ++s) {
            uint32 a = r * (seg + 1) + s, b = a + 1;
            uint32 c = a + seg + 1, d = c + 1;
            idx.insert(idx.end(), {a, c, b, b, c, d});
        }
    }
    m.SetVertices(verts);
    m.SetIndices(idx);
    return m;
}

// 平放選取環:XZ 平面同心圓環
static Mesh MakeRing(float innerR, float outerR, int seg = 24) {
    Mesh m;
    std::vector<Vertex> verts;
    std::vector<uint32> idx;
    for (int s = 0; s <= seg; ++s) {
        float a = 6.2831853f * s / seg;
        float ca = std::cos(a), sa = std::sin(a);
        Vertex vi, vo;
        vi.position = Vector3(innerR * ca, 0, innerR * sa);
        vo.position = Vector3(outerR * ca, 0, outerR * sa);
        vi.normal = vo.normal = Vector3(0, 1, 0);
        vi.texCoord = vo.texCoord = Vector2(0, 0);
        verts.push_back(vi);
        verts.push_back(vo);
    }
    for (int s = 0; s < seg; ++s) {
        uint32 a = s * 2, b = a + 1, c = a + 2, d = a + 3;
        idx.insert(idx.end(), {a, c, b, b, c, d});
    }
    m.SetVertices(verts);
    m.SetIndices(idx);
    return m;
}

static SharedPtr<SceneNode> AddStaticBox(SharedPtr<SceneNode> parent,
                                       SharedPtr<Mesh> mesh,
                                       const Vector3& pos,
                                       const Vector3& color,
                                       const char* name, float boundR) {
    auto node = MakeShared<SceneNode>(name);
    node->SetLocalPosition(pos);
    auto rc = MakeShared<RenderableComponent>();
    rc->mesh = mesh;
    rc->color = color;
    rc->boundingRadius = boundR;
    node->SetRenderable(rc);
    parent->AddChild(node);
    return node;
}

// ---- HUD ----

static double g_scroll = 0.0;
static void ScrollCallback(GLFWwindow*, double, double yoff) { g_scroll += yoff; }

// 點選機率雲:ray 與雲高平面求交,取最近候選格的 entity(-1 = 沒點到)
static int PickFogCloud(const QuantumFog& fog, const BattleController& battle,
                        const PickRay& ray, float cell) {
    Vector3 g;
    if (!BattlePicker::IntersectGround(ray, 0.5f * cell, g)) return -1;
    int best = -1;
    float bestDist = 0.9f * cell;
    for (size_t id = 0; id < fog.EntityCount(); ++id) {
        const int eid = static_cast<int>(id);
        if (fog.IsRevealed(eid)) continue;
        const Squad* owner = battle.GetFogSquad(eid);
        if (!owner || owner->IsEliminated()) continue; // 無主雲不可觀測
        for (const auto& c : fog.GetCloud(eid)) {
            const float dx = c.first.x * cell - g.x;
            const float dz = c.first.y * cell - g.z;
            const float d = std::sqrt(dx * dx + dz * dz);
            if (d < bestDist) { bestDist = d; best = eid; }
        }
    }
    return best;
}

static const char* OrderName(SquadOrder o) {
    switch (o) {
    case SquadOrder::Hold:       return "駐守";
    case SquadOrder::MoveTo:     return "移動";
    case SquadOrder::AttackMove: return "攻進";
    case SquadOrder::Retreat:    return "撤退";
    case SquadOrder::Engage:     return "追擊";
    }
    return "?";
}

// G-5：世界座標 → window 像素（計畫箭頭線條疊加用；背後點回 false）
static bool WorldToScreen(const Camera& cam, const Vector3& w,
                          float ww, float wh, ImVec2& out) {
    const Vector4 clip = cam.GetViewProjectionMatrix() *
                         Vector4(w.x, w.y, w.z, 1.0f);
    if (clip.w < 1e-5f) return false;
    out.x = (clip.x / clip.w * 0.5f + 0.5f) * ww;
    out.y = (0.5f - clip.y / clip.w * 0.5f) * wh;
    return true;
}

// ---- U-1 遊戲殼：Title → Battle → (回 Title | 離開) ----
enum class ShellScreen { Title, Battle, Quit };

// 標題頁單幀：置中視窗 + 開戰/說明/設定/離開 + B.5 章節地圖殼。
// theme/uiScale 由設定頁就地修改；applied* 追蹤已套用的值。
// pendingChapter：UI 唯讀消費 CampaignState——玩家選的章節只
// 寫到 out 參數，由主迴圈（部署層）套用，UI 不直接改戰役狀態。
static ShellScreen TitleFrame(GLFWwindow* window, OpenGLRenderer& renderer,
                              UITheme::Id& theme, float& uiScale,
                              ImFont* fontSans, ImFont* fontSerif,
                              UITheme::Id& appliedTheme, float& appliedScale,
                              RefitCamp& camp, const SquadTemplateLibrary& campLibrary,
                              const Campaign::CampaignState& campaign,
                              const Campaign::ChapterLibrary& chapters,
                              const Campaign::ChapterDef*& pendingChapter) {
    renderer.PollEvents();

    int dw = 0, dh = 0, ww = 0, wh = 0;
    glfwGetFramebufferSize(window, &dw, &dh);
    glfwGetWindowSize(window, &ww, &wh);
    if (dw > 0 && dh > 0) renderer.SetViewport(0, 0, dw, dh);
    const ImVec4 cc = UITheme::ClearColor(theme);
    renderer.SetClearColor(Vector3(cc.x, cc.y, cc.z));
    renderer.Clear(); // 不清屏會顯示未初始化後備緩衝(背景亂碼)

    ImGuiIO& io = ImGui::GetIO();
    static float themeFade = 0.0f;
    if (appliedTheme != theme || uiScale != appliedScale) {
        // F-2 絕對重建：基底 token × scale——不累乘漂移，
        // 換主題時也不再打回未縮放的尺寸（U-1 粗版 bug）
        UITheme::ApplyScaled(ImGui::GetStyle(), theme, uiScale);
        if (appliedTheme != theme) {
            themeFade = 1.0f; // 換主題 → 全屏 scrim 淡出過場(DESIGN 主題切換規範)
        }
        appliedTheme = theme;
        appliedScale = uiScale;
    }
    io.FontGlobalScale = uiScale;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // 紙本三主題走 serif、戰術面板走 sans（DESIGN §Typography）
    ImGui::PushFont(theme != UITheme::Id::TacticalSim ? fontSerif : fontSans,
                    18.0f);

    ShellScreen next = ShellScreen::Title;
    static bool showHelp = false, showSettings = false;

    ImGui::SetNextWindowPos(ImVec2(ww * 0.5f, wh * 0.46f), ImGuiCond_Always,
                            ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(UITheme::Px(430, uiScale), 0),
                             ImGuiCond_Always);
    ImGui::Begin("##title", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoBackground);

    ImGui::PushFont(fontSerif, 34.0f);
    const char* title = "斷 橋 攻 防 戰";
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() -
                          ImGui::CalcTextSize(title).x) * 0.5f);
    ImGui::TextUnformatted(title);
    ImGui::PopFont();
    ImGui::TextDisabled("寫下教令,然後看它自己打。");
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("開 戰", ImVec2(-1, UITheme::Px(34, uiScale)))) {
        next = ShellScreen::Battle;
    }

    // ---- G-9 整補營:跨場常備軍 + 戰利品帳(首戰後出現) ----
    if (!camp.GetUnits().empty()) {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextDisabled("整補營 · 戰利品 %d", camp.GetLoot());
        int totalWounded = 0;
        for (const auto& u : camp.GetUnits()) {
            totalWounded += u.wounded;
            if (u.wounded > 0) {
                ImGui::Text("  %s 兵力 %d/%d 傷 %d", u.squadName.c_str(),
                            u.members, u.maxMembers, u.wounded);
            } else {
                ImGui::Text("  %s 兵力 %d/%d", u.squadName.c_str(),
                            u.members, u.maxMembers);
            }
        }
        if (totalWounded > 0) {
            if (ImGui::Button("醫治傷兵(1點=1人)", ImVec2(-1, 0))) {
                camp.HealWounded(camp.GetLoot());
            }
        }
        const auto sorted = campLibrary.SortedByCost();
        if (!sorted.empty()) {
            const SquadTemplate* cheapest = sorted.front();
            char rlabel[96];
            std::snprintf(rlabel, sizeof(rlabel), "招募 %s(%d 戰利品)",
                          cheapest->name.c_str(), cheapest->cost);
            const bool canAfford = camp.GetLoot() >= cheapest->cost;
            if (!canAfford) ImGui::BeginDisabled();
            if (ImGui::Button(rlabel, ImVec2(-1, 0))) {
                camp.Recruit(campLibrary, cheapest->id);
            }
            if (!canAfford) ImGui::EndDisabled();
        }
    }

    // ---- B.5 章節地圖殼：唯讀顯示戰役進度 + 弧內自由選章 ----
    if (chapters.Size() > 0) {
        static const char* kArcNames[] = {"軍閥", "北伐", "抗戰", "內戰"};
        const int arc = campaign.chapter.arc;
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextDisabled("戰役 · 第%d弧 %s", arc + 1,
                            (arc >= 0 && arc < 4) ? kArcNames[arc] : "?");
        for (const Campaign::ChapterDef* def : chapters.Sorted()) {
            if (def->arc != arc) {
                continue;
            }
            const bool taken =
                std::find(campaign.chapter.frontsTaken.begin(),
                          campaign.chapter.frontsTaken.end(),
                          def->id) != campaign.chapter.frontsTaken.end();
            const bool current = campaign.chapter.chapterId == def->id;
            const bool sel = pendingChapter == def;
            char label[160];
            std::snprintf(label, sizeof(label), "%s %d-%d %s",
                          taken ? "◆" : (current ? "▶" : "·"),
                          def->arc + 1, def->chapter,
                          def->name.c_str());
            if (ImGui::Selectable(label, sel)) {
                pendingChapter = def; // 寫 out 參數，不動 campaign
            }
            if (ImGui::IsItemHovered() && !def->map.empty()) {
                ImGui::SetTooltip("地圖 %s", def->map.c_str());
            }
        }
    }

    if (ImGui::Button("操作說明", ImVec2(-1, 0))) showHelp = !showHelp;
    if (showHelp) {
        ImGui::PushTextWrapPos();
        ImGui::TextUnformatted(
            "規劃:編排教條卡(觸發→動作),Alt+拖移圖釘,"
            "Ctrl+自小隊拖出進攻箭頭,開戰。\n"
            "執行:左鍵選隊,右鍵下令耗 CP;點雲探測(1情報),"
            "Shift+點雲觀測(2情報);接觸 3 格內免費揭露。\n"
            "Space 暫停,WASD 平移,滾輪升降,Esc 取消選取。");
        ImGui::PopTextWrapPos();
    }
    static bool settingsDirty = false;
    if (ImGui::Button("設 定", ImVec2(-1, 0))) showSettings = !showSettings;
    if (showSettings) {
        int t = (int)theme;
        if (ImGui::Combo("主題", &t,
                         "現代軍事\0泥濘沙盤\0軍電作戰室\0水墨史卷\0")) {
            theme = (UITheme::Id)t;
            settingsDirty = true;
        }
        if (ImGui::SliderFloat("介面縮放", &uiScale,
                               UITheme::kScaleMin, UITheme::kScaleMax,
                               "%.2fx")) {
            settingsDirty = true;
        }
    }
    // F-2 持久化：拖曳中不落盤，放開控制項才原子寫（收起頁面也照存）
    if (settingsDirty && !ImGui::IsAnyItemActive()) {
        static const std::string kSettingsPath =
            (DemoAssets::ExeDir() / "saves" / "settings.json")
                .generic_string();
        UISettings::Save(kSettingsPath,
                         UISettings::Data{(int)theme, uiScale});
        settingsDirty = false;
    }
    if (ImGui::Button("離 開", ImVec2(-1, 0))) next = ShellScreen::Quit;

    ImGui::End();
    ImGui::PopFont();

    // CJK 直書標題（印章式，標題窗右側;DESIGN:直書僅限標題/印章）
    ImGui::PushFont(fontSerif ? fontSerif : ImGui::GetFont(), 30.0f);
    UITheme::VTextAt(ImGui::GetForegroundDrawList(),
                     ImVec2(ww * 0.5f + UITheme::Px(245, uiScale),
                            wh * 0.28f),
                     "斷橋攻防戰",
                     ImGui::GetColorU32(ImGuiCol_Text));
    ImGui::PopFont();

    // 主題切換過場：新主題 clear color 全屏 scrim 淡出(~0.45s)。
    // 無 RTT/FBO 下的合法 crossfade——遮的是 3D 背景,UI 本身已
    // 即時換色(視覺上等同淡入)
    if (themeFade > 0.0f) {
        themeFade = std::max(0.0f, themeFade - io.DeltaTime / 0.45f);
        const ImVec4 fc = UITheme::ClearColor(theme);
        ImGui::GetForegroundDrawList()->AddRectFilled(
            ImVec2(0, 0), ImVec2((float)ww, (float)wh),
            ImGui::GetColorU32(ImVec4(fc.x, fc.y, fc.z, themeFade)));
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    renderer.SwapBuffers();
    return next;
}

int main() {
    const int W = 1440, H = 810;

    OpenGLRenderer renderer;
    RendererConfig rcfg;
    rcfg.window.width = W;
    rcfg.window.height = H;
    rcfg.window.title = "MingGoRTS - 斷橋攻防戰";
    rcfg.window.vsync = true;
    renderer.SetConfig(rcfg);
    if (!renderer.Initialize()) {
        std::fprintf(stderr, "[SKIP] no display / renderer init failed\n");
        return 0;
    }
    GLFWwindow* window = static_cast<GLFWwindow*>(renderer.GetWindowHandle());
    glfwSetScrollCallback(window, ScrollCallback);

    // ImGui + U-1 字體自包含:assets/fonts/ 的 OFL Noto;缺檔退回內建字
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGuiIO& io = ImGui::GetIO();
    ImFont* fontSans =
        UITheme::LoadFont(io, DemoAssets::Resolve("fonts/NotoSansTC-Regular.otf"), 18.0f);
    ImFont* fontSerif =
        UITheme::LoadFont(io, DemoAssets::Resolve("fonts/NotoSerifTC-Regular.otf"), 18.0f);
    if (!fontSans && !fontSerif) {
        io.Fonts->AddFontDefault(); // 字體缺檔也要保證 atlas 非空
    }
    // F-5/UX-DR3：chronicler 字體回退鏈 serif→sans→內建字。
    // PushFont(nullptr) 雖會退回目前字體，此處明確化讓回退語意固定。
    ImFont* fontFallback = fontSans ? fontSans : fontSerif;
    if (!fontFallback) fontFallback = io.Fonts->Fonts[0];
    if (!fontSans) fontSans = fontFallback;
    if (!fontSerif) fontSerif = fontFallback;
    io.FontDefault = fontSans;

    // U-1 殼層狀態:主題/UI 縮放在標題頁設定頁修改
    // F-2：potato.settings/1 持久化——缺檔=預設值不報錯
    const std::string kSettingsPath =
        (DemoAssets::ExeDir() / "saves" / "settings.json")
            .generic_string();
    UITheme::Id theme = UITheme::Id::TacticalSim;
    float uiScale = 1.0f;
    {
        UISettings::Data st;
        if (UISettings::Load(kSettingsPath, st)) {
            if (st.theme >= 0 && st.theme < UITheme::kCount) {
                theme = (UITheme::Id)st.theme;
            }
            uiScale = st.uiScale;
        }
    }
    UITheme::Id appliedTheme = theme;
    float appliedScale = uiScale;
    UITheme::ApplyScaled(ImGui::GetStyle(), theme, uiScale);

    // ImGui/renderer 已初始化後的失敗路徑都要走這個清理
    auto shutdownAll = [&]() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        renderer.Shutdown();
    };

    auto unitShader = MakeShared<Shader>();
    if (!unitShader->LoadFromSource(UNIT_VERT, UNIT_FRAG)) {
        std::fprintf(stderr, "Shader compile failed\n");
        shutdownAll();
        return 1;
    }
    unitShader->Bind();
    unitShader->SetUniformVec3("uLightDir", Vector3(-0.4f, 1.0f, -0.35f));

    // U-1 殼層外迴圈:Title ↔ Battle;整場戰鬥在內層 scope,
    // 出 scope 所有 stack 物件析構 = 乾淨重置(回主選單可再戰)
    // G-9 整補營:常備軍+戰利品帳持有在殼層外,跨場持續
    // E-7 戰役框架:整補營改由 CampaignState 持有（章節邊界聚合存檔）
    // N-2 敵將檔案:聽聞/驗證狀態也跨場——驗過的將不重聽傳聞
    Campaign::CampaignState campaign;
    RefitCamp& camp = campaign.Camp();
    GeneralDossier dossier;
    SquadTemplateLibrary campLibrary;
    // C-3 目錄整併：舊 templates/ 已併入 squads/，單一目錄載入
    campLibrary.LoadDir(DemoAssets::Resolve("squads"));

    // ---- C-1 戰役存檔接線：啟動讀檔 + 章節定義庫 ----
    // 存檔放 exe 旁 saves/（與啟動 cwd 無關）；無檔=新戰役不報錯
    const std::string kCampaignSave =
        (DemoAssets::ExeDir() / "saves" / "campaign.json")
            .generic_string();
    Campaign::ChapterLibrary chapters;
    chapters.LoadDir(DemoAssets::Resolve("campaign"));
    if (!campaign.LoadFromFile(kCampaignSave)) {
        // 新戰役：依 (arc,chapter) 序 seed 首章
        const auto sorted = chapters.Sorted();
        if (!sorted.empty()) {
            campaign.AdvanceChapter(sorted.front()->arc,
                                    sorted.front()->chapter,
                                    sorted.front()->id);
        }
    }
    ShellScreen screen = ShellScreen::Title;
    // B.5 章節選擇暫存：TitleFrame 唯讀 campaign，選章寫到這裡，
    // 進部署時由主迴圈（非 UI 層）套用到 campaign.chapter。
    const Campaign::ChapterDef* pendingChapter = nullptr;
    while (screen != ShellScreen::Quit && !renderer.ShouldClose()) {
        if (screen == ShellScreen::Title) {
            screen = TitleFrame(window, renderer, theme, uiScale,
                                fontSans, fontSerif, appliedTheme,
                                appliedScale, camp, campLibrary,
                                campaign, chapters, pendingChapter);
            continue;
        }
        if (pendingChapter != nullptr) {
            // B.5 選章 → 讀定義 → 部署流程讀到的是選中的章節
            campaign.chapter.arc = pendingChapter->arc;
            campaign.chapter.chapter = pendingChapter->chapter;
            campaign.chapter.chapterId = pendingChapter->id;
            pendingChapter = nullptr;
        }
        bool backToTitle = false;
        { // ---- 戰鬥場次 scope 開始(內部維持原縮排以保 diff 最小) ----
        // 戰後結算狀態:每場重建——宣告成 static 會跨場殘留上一場的報告
        std::string chronicler;
        float endedAt = -1.0f;
        float replayCursor = 0.0f;
        GovernanceField govField; // A-1/A-3/A-4：治理互動追蹤（每場重建）

    // ---- 地圖 ----
    BattleMap map;
    if (!map.LoadFromFile(DemoAssets::Resolve("maps/duanqiao.json").c_str())) {
        std::fprintf(stderr, "cannot load assets/maps/duanqiao.json\n");
        shutdownAll();
        return 1;
    }
    const float CELL = map.GetCellSize();
    const int GW = map.GetGridWidth(), GH = map.GetGridHeight();
    const float FW = GW * CELL, FH = GH * CELL;

    // 事件流(HUD + console);在 battle 之前宣告,避免解構順序留下懸空 callback
    std::deque<std::string> eventLog;

    BattleController battle(GW, GH, CELL);
    map.ApplyToField(battle.GetField());
    govField.Bind(map.GetInteractables(), map.GetConvoys()); // A-1/A-3/A-4
    battle.SetEventCallback([&](const std::string& e) {
        eventLog.push_back(e);
        if (eventLog.size() > 8) eventLog.pop_front();
        std::printf("  [戰報] %s\n", e.c_str());
    });

    // T-11 戰後素材:T-7 錄製器包在事件回調外層(錄下全部戰報)
    BattleRecorder recorder;
    recorder.Attach(battle);
    Roster roster;

    // ---- 場景:地形 ----
    SceneGraph scene;
    auto root = MakeShared<SceneNode>("root");
    scene.SetRootNode(root);

    auto groundMesh = MakeShared<Mesh>(MakeBox(1.0f, 0.3f, 1.0f));
    auto waterMesh  = MakeShared<Mesh>(MakeBox(CELL, 0.25f, CELL));
    auto plankMesh  = MakeShared<Mesh>(MakeBox(CELL, 0.3f, CELL));
    auto pinMesh    = MakeShared<Mesh>(MakeBox(0.25f, 1.4f, 0.25f));
    auto propMesh   = MakeShared<Mesh>(MakeBox(0.7f, 0.7f, 0.7f));
    auto capMesh    = MakeShared<Mesh>(MakeCapsule(0.35f * CELL, 1.6f * CELL));
    auto ringMesh   = MakeShared<Mesh>(MakeRing(0.55f * CELL, 0.75f * CELL));
    // 血條直立(XY 面朝相機):平躺版在 63° 俯角下只剩 ~2px 不可讀
    auto barBgMesh  = MakeShared<Mesh>(MakeBox(1.0f, 0.16f, 0.03f));
    auto barFillMesh = MakeShared<Mesh>(MakeBox(0.96f, 0.11f, 0.04f));
    auto cloudMesh  = MakeShared<Mesh>(MakeBox(0.7f, 0.7f, 0.7f)); // 機率雲標記

    auto groundNode = AddStaticBox(root, groundMesh,
                                   Vector3(FW / 2, -0.15f, FH / 2),
                                   Vector3(0.16f, 0.22f, 0.15f), "ground", FW);
    groundNode->SetLocalScale(Vector3(FW, 1, FH));

    // 河面:IsBlocked 的格子鋪水磚(渡口自動留空)
    for (int y = 0; y < GH; ++y) {
        for (int x = 0; x < GW; ++x) {
            if (battle.GetField().IsBlocked(x, y)) {
                AddStaticBox(root, waterMesh,
                             Vector3((x + 0.5f) * CELL, -0.05f, (y + 0.5f) * CELL),
                             Vector3(0.15f, 0.30f, 0.45f), "water", CELL);
            }
        }
    }
    // 渡口鋪橋板
    for (const auto& ford : map.GetFords()) {
        for (int y = 0; y < ford.rect.h; ++y) {
            for (int x = 0; x < ford.rect.w; ++x) {
                AddStaticBox(root, plankMesh,
                             Vector3((ford.rect.x + x + 0.5f) * CELL, 0.02f,
                                     (ford.rect.y + y + 0.5f) * CELL),
                             Vector3(0.42f, 0.32f, 0.20f), "ford", CELL);
            }
        }
    }
    // 圖釘標記
    for (const auto& pin : map.GetPins()) {
        AddStaticBox(root, pinMesh,
                     Vector3(pin.pos.x * CELL, 0.7f, pin.pos.y * CELL),
                     Vector3(0.85f, 0.75f, 0.30f), "pin", CELL);
    }
    // 互動物
    for (const auto& it : map.GetInteractables()) {
        Vector3 c = (it.type == "oil_slick") ? Vector3(0.10f, 0.10f, 0.10f)
                                             : Vector3(0.45f, 0.42f, 0.40f);
        AddStaticBox(root, propMesh,
                     Vector3(it.pos.x * CELL, 0.35f, it.pos.y * CELL),
                     c, "prop", CELL);
    }

    // ---- 編成(同 DuanqiaoDemo):我 4 隊北岸 / 敵格洛克 4 隊南岸 ----
    const MapPin* northRally = map.FindPin("北岸集結點");
    const MapPin* southCamp  = map.FindPin("南岸敵營");
    Squad* rearGuard = nullptr;
    Squad* generalGuard = nullptr;
    if (camp.GetUnits().empty()) {
        // 首戰：預設編制入伍，進整補營當常備軍
        battle.CreateSquad("前鋒", 0, Vector2(8.0f, 3.0f),  30);
        battle.CreateSquad("中軍", 0, Vector2(12.0f, 2.5f), 30);
        battle.CreateSquad("左翼", 0, Vector2(16.0f, 3.0f), 25);
        rearGuard = battle.CreateSquad("後衛", 0, Vector2(12.0f, 4.5f), 15);
        // G-8 將軍親臨：衛隊全滅即敗（潰逃不算），技能走 CP
        generalGuard = battle.CreateSquad("將軍衛隊", 0, Vector2(12.0f, 1.0f), 10);
        generalGuard->SetGeneralGuard(true);
        for (const auto& sq : battle.GetSquads()) {
            if (sq->GetTeam() != 0) continue;
            VeteranUnit vu;
            vu.squadName = sq->GetName();
            vu.unitClass = sq->GetUnitClass();
            vu.members = sq->GetMembers();
            vu.maxMembers = sq->GetMaxMembers();
            camp.EnrollUnit(vu);
        }
    } else {
        // G-9：整補營跨場重建——兵力/傷兵狀態從上一場延續
        const std::vector<Vector2> deployPos = {
            Vector2(8.0f, 3.0f), Vector2(12.0f, 2.5f), Vector2(16.0f, 3.0f),
            Vector2(12.0f, 4.5f), Vector2(12.0f, 1.0f),
        };
        // 傳 campLibrary 讓招募單位回補模板 stats（速度/火力/疲勞）
        for (Squad* s : camp.Deploy(battle, 0, deployPos, &campLibrary)) {
            if (s->GetName() == "後衛") rearGuard = s;
            if (s->GetName() == "將軍衛隊") {
                s->SetGeneralGuard(true);
                generalGuard = s;
            }
        }
    }
    Squad* e0 = battle.CreateSquad("格洛克親衛", 1, Vector2(12.0f, 12.0f), 35);
    Squad* e1 = battle.CreateSquad("蠻兵隊",     1, Vector2(9.0f, 12.5f),  25);
    Squad* e2 = battle.CreateSquad("掠奪隊",     1, Vector2(15.0f, 12.5f), 25);
    Squad* e3 = battle.CreateSquad("守橋隊",     1, Vector2(12.0f, 10.5f), 20);
    if (southCamp)  battle.SetObjective(0, southCamp->pos);
    if (northRally) battle.SetObjective(1, northRally->pos);
    if (northRally) battle.SetRallyPoint(0, northRally->pos);
    if (southCamp)  battle.SetRallyPoint(1, southCamp->pos);

    // T-8 名冊:兩軍具名隊長入冊(陣亡=戰後哀悼/戰果清單素材)
    {
        const char* capNames0[] = {"周鐵槍", "陳守拙", "林燕翼", "石敢當"};
        const char* relics0[] = {"斷刃", "舊旗", "竹哨", "平安符"};
        const char* capNames1[] = {"格洛克", "血手布魯", "獨眼斯卡", "老槐"};
        int i0 = 0, i1 = 0;
        for (const auto& sq : battle.GetSquads()) {
            if (sq->GetTeam() == 0 && i0 < 4) {
                roster.Enroll(sq.get(), capNames0[i0], "captain",
                              relics0[i0]);
                ++i0;
            } else if (sq->GetTeam() == 1 && i1 < 4) {
                roster.Enroll(sq.get(), capNames1[i1], "captain");
                ++i1;
            }
        }
    }

    EnemyGeneral glock = EnemyGeneral::MakeGlock();
    glock.ApplyTo(battle, 1);
    // N-2:開局只聽聞不真相——已驗證的檔案跨場保留
    if (!dossier.Find(glock.GetName())) {
        dossier.Hear(glock);
    }
    BattlePlanner planner;
    // 情報/CP 走 BattleResources(fog 觀測扣點要它)
    BattleResources res;
    res.Setup(battle, 0, /*intel=*/8, /*cp=*/5);
    res.Setup(battle, 1, /*intel=*/0, /*cp=*/3);
    BattleResources::ApplyMoraleRule(battle);
    // G-1 士氣連鎖：潰逃隊對 4 格內友軍造成 15% 士氣衝擊（可連鎖）
    battle.SetRoutShock(/*radius=*/4.0f, /*moraleHit=*/0.15f);
    // G-2：掠奪隊標騎兵（快攻克制弓），守橋隊預設步兵；戰線寬 2
    battle.SetCombatWidth(2);
    // P-3 擁擠阻塞：渡口寬 2 格是天然 chokepoint——多隊同擠渡口時
    // 密度過臨界值移速急降（jamming 行為級近似），孤隊不受影響
    battle.SetJamming(/*radius=*/2.0f);
    if (e2) e2->SetUnitClass(UnitClass::Cavalry);

    // ---- Q-1/Q-3 敵情霧:敵軍以疊加態存在,觀測或接觸才塌縮 ----
    // 觀測 2 情報全額買斷;探測 1 情報讓雲向真值收縮(弱觀測)
    QuantumFog fog(/*intelDuration=*/25.0f, /*observe=*/2, /*probe=*/1);
    fog.BindResources(&res);
    battle.BindFog(&fog);
    for (Squad* es : {e0, e1, e2, e3}) {
        // 雲心朝敵軍進攻方向偏移 1.5 格:不洩漏真實位置,
        // 靠偵查/觀測才能確定敵人在哪
        Vector2 center = es->GetPosition();
        Vector2 advanceDir(0.0f, 0.0f);
        if (northRally) {
            Vector2 dir = northRally->pos - center;
            const float len = dir.Length();
            if (len > 1e-4f) {
                advanceDir = dir * (1.0f / len);
                center = center + dir * (1.5f / len);
            }
        }
        // Q-4 人格先驗:格洛克侵略 90 → 偏置點推向敵方前線
        const Vector2 bias = glock.FogBiasPoint(center, advanceDir, 3.5f);
        const int id = fog.AddEntityCloud(
            es->GetName(), /*觀測方=*/0, center,
            /*radius=*/3.5f, /*count=*/6, /*minSpacing=*/1.2f,
            &bias, glock.FogPriorScale());
        if (id >= 0) battle.BindFogSquad(es, id);
    }
    // Q-2 糾纏:e0/e1 同向機動——觀測其一,另一朵雲向同向候選收縮
    if (!fog.Entangle(battle.GetFogEntityId(e0),
                      battle.GetFogEntityId(e1))) {
        printf("[fog] 糾纏建立失敗(e0/e1)\n");
    }

    // ---- T-9 回合層牌組:Init 收隊 → AI 參謀模板起手 → 玩家可編輯 ----
    PlanningDeck deck;
    deck.Init(battle, 0);
    deck.LoadPlannerTemplate(planner, battle, 0);

    // ---- G-5 作戰計畫箭頭:Ctrl+左鍵自小隊拖出主攻軸線,開戰套用 ----
    BattlePlan plan;
    plan.SetPlanBonus(/*attackMul=*/1.10f, /*intel=*/2, /*cp=*/1);
    // quantum-plan:加成依箭頭尖端的情報確定度即時縮放——
    // 雲未解析時加成打折,偵查/觀測坐實後回到滿額
    battle.BindPlan(&plan);
    // 後衛改當偵查兵:低血撤退 > 遇敵應戰 > 無事就往最近的雲走
    {
        const int di = deck.FindDeck(rearGuard);
        if (di >= 0) {
            auto& d = deck.Deck(di);
            d.rules.clear();
            d.rules.push_back({DoctrineTrigger::HealthBelow,
                               DoctrineAction::RetreatToRally, 0.3f, 1});
            d.rules.push_back({DoctrineTrigger::EnemyInRange,
                               DoctrineAction::AttackNearest, 3.0f, 10});
            d.rules.push_back({DoctrineTrigger::Always,
                               DoctrineAction::Scout, 0.0f, 90});
        }
    }

    // ---- Gameplay → 場景 ----
    BattleSceneSync sync;
    sync.Attach(battle, scene, CELL);
    sync.SetUnitMesh(capMesh);
    sync.SetOverlayMeshes(ringMesh, barBgMesh, barFillMesh, 2.0f * CELL, 1.2f);
    sync.SetFog(&fog);
    sync.SetFogMarkerMesh(cloudMesh);
    // P-1 湍流漂移:機率雲標記疊加無散度微擾,不確定性「活」起來;
    // strength=0 可關閉(預設行為不變)
    Quasi::TurbulenceField fogTurb(/*octaves=*/6, /*seed=*/42,
                                  /*baseFreq=*/0.15f, /*baseAmp=*/1.0f);
    sync.SetFogDrift(&fogTurb, 0.4f * CELL);
    // P-4 間歇陣風:漂移量乘對數正態強度場,雲忽快忽慢（預設關閉時 g≡1）
    Quasi::GustField fogGust(/*octaves=*/5, /*seed=*/42,
                             /*baseFreq=*/0.08f, /*sigma=*/0.7f);
    sync.SetFogDriftGust(&fogGust);
    sync.Sync(battle);

    // T-9 圖釘:objective(紅)/rally(藍)——Planning 中 Alt+點地移動,
    // 開戰後仍顯示作為戰場錨點
    auto objPinNode = AddStaticBox(root, pinMesh, Vector3(0, 0.9f, 0),
                                   Vector3(0.9f, 0.25f, 0.25f), "objpin", CELL);
    auto rallyPinNode = AddStaticBox(root, pinMesh, Vector3(0, 0.9f, 0),
                                     Vector3(0.25f, 0.45f, 0.95f), "rallypin",
                                     CELL);

    // 開戰前停留 Planning phase;battle 保持 Deployment 不 tick

    // ---- 相機:RTS 高位俯視,WASD 平移、滾輪升降 ----
    Camera cam;
    cam.SetViewport(0, 0, W, H);
    cam.SetPerspective(50.0f * 3.14159265f / 180.0f, (float)W / H, 0.1f, 300.0f);
    Vector3 camTarget(FW / 2, 0.0f, FH / 2 - 2.0f);
    float camHeight = 26.0f, camBack = 13.0f;

    SceneRenderer sceneRenderer;
    sceneRenderer.SetDefaultShader(unitShader);

    Squad* selected = nullptr;
    bool planningPhase = true;  // T-9:開戰前停在回合層編牌
    int curDeck = 0, curRule = -1; // 牌組 UI 選中態
    Squad* arrowDragSquad = nullptr; // G-5:拖曳中的箭頭起點小隊
    Vector2 arrowDragPos;            // G-5:拖曳目前落點(cell)
    bool paused = false;
    float speedScale = 1.0f;
    bool prevL = false, prevR = false, prevSpace = false, prevEsc = false;
    double prevTime = glfwGetTime();

    std::printf("斷橋可玩 demo — 左鍵選取,右鍵下令(CP),Space 暫停\n");

    while (!renderer.ShouldClose() && !backToTitle) {
        renderer.PollEvents();
        double now = glfwGetTime();
        float dt = (float)std::min(now - prevTime, 0.1);
        prevTime = now;

        // framebuffer 用於 GL viewport;window size 用於 ImGui/游標(HiDPI 下兩者不同)
        int dw = W, dh = H, ww = W, wh = H;
        glfwGetFramebufferSize(window, &dw, &dh);
        glfwGetWindowSize(window, &ww, &wh);
        const float sx = (ww > 0) ? (float)dw / ww : 1.0f;
        const float sy = (wh > 0) ? (float)dh / wh : 1.0f;
        if (dw > 0 && dh > 0) {
            renderer.SetViewport(0, 0, dw, dh);
            cam.SetViewport(0, 0, dw, dh);
            cam.SetPerspective(50.0f * 3.14159265f / 180.0f,
                               (float)dw / dh, 0.1f, 300.0f);
        }

        // ---- 鍵盤 ----
        bool space = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
        if (space && !prevSpace) paused = !paused;
        prevSpace = space;
        bool esc = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (esc && !prevEsc) { selected = nullptr; sync.SetSelectedSquad(nullptr); }
        prevEsc = esc;
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) speedScale = 0.5f;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) speedScale = 1.0f;
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) speedScale = 2.0f;

        float pan = 12.0f * dt;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camTarget.z -= pan;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camTarget.z += pan;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camTarget.x -= pan;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camTarget.x += pan;
        camTarget.x = std::max(0.0f, std::min(camTarget.x, FW));
        camTarget.z = std::max(0.0f, std::min(camTarget.z, FH));
        // 滾輪在 ImGui 視窗上時不縮放相機
        if (!io.WantCaptureMouse) {
            camHeight = std::max(10.0f,
                         std::min(camHeight - (float)g_scroll * 2.0f, 45.0f));
        }
        g_scroll = 0.0;
        cam.SetPosition(Vector3(camTarget.x, camHeight, camTarget.z + camBack));
        cam.SetTarget(camTarget);

        battle.SetTimeScale(paused ? 0.0f : speedScale);

        // ---- 圖釘位置同步(Planning 中可拖移,執行中固定顯示)----
        {
            Vector2 op = battle.GetObjective(0);
            objPinNode->SetLocalPosition(
                Vector3(op.x * CELL, 0.9f, op.y * CELL));
            Vector2 rp = battle.GetRallyPoint(0);
            rallyPinNode->SetLocalPosition(
                Vector3(rp.x * CELL, 0.9f, rp.y * CELL));
        }

        // ---- 滑鼠點選/下令(只在執行中且未分勝負)----
        bool live = battle.GetPhase() == BattlePhase::Execution &&
                    battle.GetOutcome() == BattleOutcome::Ongoing;
        // 游標座標是 window 空間,乘 HiDPI 比例轉進 framebuffer 空間
        double mx, my;
        glfwGetCursorPos(window, &mx, &my);
        float fmx = (float)mx * sx, fmy = (float)my * sy;
        bool cursorIn = fmx >= 0.0f && fmy >= 0.0f &&
                        fmx < (float)dw && fmy < (float)dh;
        bool lmb = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        bool rmb = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
        bool lClick = lmb && !prevL, rClick = rmb && !prevR;
        prevL = lmb; prevR = rmb;

        // ---- Planning:Alt+左鍵=移 objective 圖釘 / Alt+右鍵=移 rally ----
        if (planningPhase && cursorIn && !io.WantCaptureMouse &&
            (lClick || rClick)) {
            const bool alt =
                glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS;
            if (alt) {
                PickRay ray = BattlePicker::ScreenToWorldRay(cam, fmx, fmy);
                Vector3 g;
                if (BattlePicker::IntersectGround(ray, 0.0f, g)) {
                    Vector2 cell = BattlePicker::WorldToCell(g, CELL);
                    cell.x = std::max(0.0f, std::min(cell.x, (float)GW - 0.5f));
                    cell.y = std::max(0.0f, std::min(cell.y, (float)GH - 0.5f));
                    if (lClick) battle.SetObjective(0, cell);
                    else        battle.SetRallyPoint(0, cell);
                    eventLog.push_back(lClick ? "目標點已移動"
                                              : "集結點已移動");
                    if (eventLog.size() > 8) eventLog.pop_front();
                }
            }
        }

        // ---- G-5 Planning:Ctrl+左鍵自小隊拖出進攻箭頭 ----
        if (planningPhase && cursorIn && !io.WantCaptureMouse) {
            PickRay ray = BattlePicker::ScreenToWorldRay(cam, fmx, fmy);
            if (lClick && (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) ==
                               GLFW_PRESS ||
                           glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) ==
                               GLFW_PRESS)) {
                arrowDragSquad = BattlePicker::PickSquad(battle, 0, ray, CELL);
            }
            if (arrowDragSquad) {
                Vector3 g;
                if (BattlePicker::IntersectGround(ray, 0.0f, g)) {
                    arrowDragPos = BattlePicker::WorldToCell(g, CELL);
                }
                if (!lmb) { // 放開 → 落點入計畫(同隊重畫覆蓋舊箭頭)
                    Vector2 to(
                        std::max(0.0f,
                                 std::min(arrowDragPos.x, (float)GW - 0.5f)),
                        std::max(0.0f,
                                 std::min(arrowDragPos.y, (float)GH - 0.5f)));
                    plan.RemoveArrowFor(arrowDragSquad->GetName());
                    plan.AddArrow(arrowDragSquad->GetName(),
                                  arrowDragSquad->GetPosition(), to);
                    eventLog.push_back("計畫箭頭:" +
                                       arrowDragSquad->GetName());
                    if (eventLog.size() > 8) eventLog.pop_front();
                    arrowDragSquad = nullptr;
                }
            }
        }

        if (live && cursorIn && !io.WantCaptureMouse && (lClick || rClick)) {
            PickRay ray = BattlePicker::ScreenToWorldRay(cam, fmx, fmy);
            // 左右鍵獨立處理:同幀雙擊不會把右鍵指令吞掉
            if (lClick) {
                Squad* hit = BattlePicker::PickSquad(battle, 0, ray, CELL);
                if (hit) {
                    selected = hit;
                    sync.SetSelectedSquad(hit);
                } else {
                    // 點到敵情雲:LMB=探測(1情報,雲收縮) Shift+LMB=觀測(2情報,塌縮)
                    const int eid = PickFogCloud(fog, battle, ray, CELL);
                    if (eid >= 0) {
                        Squad* ts = battle.GetFogSquad(eid);
                        const bool full =
                            glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) ==
                                GLFW_PRESS ||
                            glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) ==
                                GLFW_PRESS;
                        bool ok = false;
                        std::string msg;
                        if (full) {
                            ok = ts && fog.Observe(eid, ts->GetPosition());
                            msg = ok ? "觀測塌縮:" + ts->GetName()
                                     : "觀測失敗(情報不足)";
                            const int pid = fog.EntangledPartner(eid);
                            if (ok && pid >= 0 && !fog.IsRevealed(pid)) {
                                if (Squad* ps = battle.GetFogSquad(pid))
                                    msg += " 糾纏→" + ps->GetName() +
                                           "雲收縮";
                            }
                        } else {
                            ok = ts && fog.Probe(eid, ts->GetPosition(), 0.4f);
                            if (ok) {
                                double mp = 0.0;
                                for (const auto& c : fog.GetCloud(eid))
                                    mp = std::max(mp, c.second);
                                msg = "探測收縮:" + ts->GetName() +
                                      "(最高" +
                                      std::to_string(int(mp * 100)) +
                                      "%)";
                            } else {
                                msg = "探測失敗(情報不足)";
                            }
                        }
                        eventLog.push_back(msg);
                        if (eventLog.size() > 8) eventLog.pop_front();
                    } else {
                        selected = nullptr;
                        sync.SetSelectedSquad(nullptr);
                    }
                }
            }
            if (rClick && selected) {
                Squad* enemy = BattlePicker::PickSquad(battle, 1, ray, CELL);
                // 未揭露的敵軍不可被 Engage(看不見的打不到)
                if (enemy) {
                    const int fid = battle.GetFogEntityId(enemy);
                    if (fid >= 0 && !fog.IsRevealed(fid)) enemy = nullptr;
                }
                bool ok = false;
                if (enemy) {
                    ok = battle.Intervene(selected, SquadOrder::Engage, enemy);
                } else {
                    Vector3 g;
                    if (BattlePicker::IntersectGround(ray, 0.0f, g)) {
                        Vector2 cell = BattlePicker::WorldToCell(g, CELL);
                        cell.x = std::max(0.0f, std::min(cell.x, (float)GW - 0.5f));
                        cell.y = std::max(0.0f, std::min(cell.y, (float)GH - 0.5f));
                        ok = battle.Intervene(selected, SquadOrder::AttackMove, cell);
                    }
                }
                if (!ok) {
                    eventLog.push_back("下令失敗(CP 不足或目標無效)");
                    if (eventLog.size() > 8) eventLog.pop_front();
                }
            }
        }

        // 選中的小隊不在了(潰逃/全滅)就解除選取
        if (selected && (selected->IsEliminated() || selected->IsRouting())) {
            selected = nullptr;
            sync.SetSelectedSquad(nullptr);
        }

        battle.Update(dt);
        roster.Update(battle); // T-8:殲滅偵測→記陣亡
        sync.Sync(battle);

        // A-1/A-3/A-4 治理源：佔領/焚村/護輜由 GovernanceField
        // 追蹤（地圖知識在此層），事件經 battle 入帳
        govField.Update(dt, battle);

        // ---- 渲染 ----
        { // U-1:clear color 跟主題走
            const ImVec4 cc = UITheme::ClearColor(theme);
            renderer.SetClearColor(Vector3(cc.x, cc.y, cc.z));
        }
        renderer.Clear();
        renderer.EnableDepthTest(true);
        renderer.EnableCulling(false); // 手排頂點繞序不保證 CCW
        sceneRenderer.Render(scene, cam);

        // ---- HUD ----
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::PushFont(theme != UITheme::Id::TacticalSim ? fontSerif
                                                        : fontSans,
                        18.0f);

        // ---- G-5 計畫箭頭疊加線:規劃中亮金,開戰後淡化作軸線錨點 ----
        if (plan.ArrowCount() > 0 || arrowDragSquad) {
            ImDrawList* dl = ImGui::GetBackgroundDrawList();
            const ImU32 col = planningPhase ? IM_COL32(255, 180, 60, 230)
                                            : IM_COL32(255, 180, 60, 90);
            auto drawArrow = [&](const Vector2& a, const Vector2& b) {
                ImVec2 pa, pb;
                if (!WorldToScreen(
                        cam, Vector3(a.x * CELL, 0.6f, a.y * CELL),
                        (float)ww, (float)wh, pa) ||
                    !WorldToScreen(
                        cam, Vector3(b.x * CELL, 0.6f, b.y * CELL),
                        (float)ww, (float)wh, pb)) {
                    return;
                }
                dl->AddLine(pa, pb, col, 3.0f);
                ImVec2 d(pb.x - pa.x, pb.y - pa.y);
                const float len = std::sqrt(d.x * d.x + d.y * d.y);
                if (len > 12.0f) { // 箭頭頭
                    d.x /= len;
                    d.y /= len;
                    const ImVec2 n(-d.y, d.x);
                    dl->AddTriangleFilled(
                        pb,
                        ImVec2(pb.x - d.x * 14.0f + n.x * 6.0f,
                               pb.y - d.y * 14.0f + n.y * 6.0f),
                        ImVec2(pb.x - d.x * 14.0f - n.x * 6.0f,
                               pb.y - d.y * 14.0f - n.y * 6.0f),
                        col);
                }
            };
            for (size_t i = 0; i < plan.ArrowCount(); ++i) {
                drawArrow(plan.Arrow(i).from, plan.Arrow(i).to);
            }
            if (arrowDragSquad) {
                drawArrow(arrowDragSquad->GetPosition(), arrowDragPos);
            }
        }

        ImGui::SetNextWindowPos(ImVec2(8, 8), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(UITheme::Px(300, uiScale), 0),
                                 ImGuiCond_Always);
        ImGui::Begin("斷橋指揮", nullptr,
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("階段: %s%s",
                    battle.GetPhase() == BattlePhase::Execution ? "執行" :
                    battle.GetPhase() == BattlePhase::Deployment ? "部署" : "結算",
                    paused ? "(暫停)" : "");
        int hiddenFoes = 0;
        for (size_t id = 0; id < fog.EntityCount(); ++id) {
            const Squad* owner = battle.GetFogSquad((int)id);
            if (owner && owner->IsEliminated()) continue; // 全滅不算「未揭露」
            if (!fog.IsRevealed((int)id)) ++hiddenFoes;
        }
        ImGui::Text("CP: %d   情報: %d   倍速: %.1fx",
                    battle.GetCommandPoints(0), res.GetIntel(0),
                    paused ? 0.0f : speedScale);
        { // 戰鬥時鐘:固定欄寬右對齊,數字變動不推移版面(DESIGN 計時器規範)
            char tbuf[32];
            std::snprintf(tbuf, sizeof(tbuf), "時間 %.0fs",
                          battle.GetElapsed());
            ImGui::SameLine(0.0f, 16.0f);
            UITheme::FixedField(tbuf, 88.0f * uiScale);
        }
        ImGui::Text("未揭露敵軍: %d / %d", hiddenFoes, (int)fog.EntityCount());
        ImGui::Separator();
        if (selected) {
            ImGui::Text("選取: %s", selected->GetName().c_str());
            ImGui::Text("兵力 %d/%d  士氣 %.0f%%  命令 %s",
                        selected->GetMembers(), selected->GetMaxMembers(),
                        selected->GetMorale() * 100.0f,
                        OrderName(selected->GetOrder()));
        } else {
            ImGui::TextDisabled("未選取小隊(左鍵點選)");
        }
        ImGui::Separator();
        ImGui::TextDisabled("左鍵選取 | 點雲探測(1情報) | Shift+點雲觀測(2情報)");
        ImGui::TextDisabled("右鍵下令 | Space 暫停 | 1/2/3 倍速");
        ImGui::TextDisabled("WASD 平移 | 滾輪縮放 | Esc 取消選取");
        if (planningPhase) {
            ImGui::TextDisabled("Alt+左鍵=移目標點 | Alt+右鍵=移集結點");
            ImGui::TextDisabled("Ctrl+左鍵自小隊拖出=畫進攻箭頭");
        }
        ImGui::End();

        // ---- N-2 敵將檔案:判詞是聽聞態,親衛雲揭露才回真值 ----
        {
            const int guardEid = battle.GetFogEntityId(e0);
            if (guardEid >= 0 && fog.IsRevealed(guardEid)) {
                dossier.Verify(glock); // 冪等：寫真值+標 verified
            }
            if (const HearsayEntry* he = dossier.Find(glock.GetName())) {
                ImGui::SetNextWindowPos(
                    ImVec2(ww - UITheme::Px(308, uiScale), 8),
                    ImGuiCond_Always);
                ImGui::SetNextWindowSize(
                    ImVec2(UITheme::Px(300, uiScale), 0),
                    ImGuiCond_Always);
                ImGui::Begin("敵將檔案", nullptr,
                             ImGuiWindowFlags_NoCollapse |
                                 ImGuiWindowFlags_AlwaysAutoResize);
                ImGui::Text("%s %s", glock.GetName().c_str(),
                            glock.GetEpithet().c_str());
                ImGui::TextWrapped("判詞:%s", he->verdict.c_str());
                ImGui::Separator();
                if (he->verified) {
                    ImGui::TextDisabled("實情(已觀測驗證)");
                    ImGui::Text("侵略 %.0f  紀律 %.0f  狡詐 %.0f",
                                he->estAggression, he->estDiscipline,
                                he->estCunning);
                } else {
                    ImGui::TextDisabled("傳聞(未驗證,或有所低估)");
                    ImGui::Text("侵略 ~%.0f  紀律 ~%.0f  狡詐 ~%.0f",
                                he->estAggression, he->estDiscipline,
                                he->estCunning);
                    ImGui::TextDisabled("觀測親衛之雲以驗其實");
                }
                ImGui::End();
            }
        }

        // ---- T-9 回合層:作戰計畫視窗(三欄:小隊/卡槽/編輯器)----
        if (planningPhase) {
            ImGui::SetNextWindowPos(
                ImVec2(ww * 0.5f - UITheme::Px(330, uiScale), 40),
                ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(
                ImVec2(UITheme::Px(660, uiScale),
                       UITheme::Px(420, uiScale)),
                ImGuiCond_FirstUseEver);
            ImGui::Begin("作戰計畫 — 戰前軍議", nullptr,
                         ImGuiWindowFlags_NoCollapse);

            if (curDeck >= deck.SquadCount()) curDeck = 0;
            auto& cd = deck.Deck(curDeck);
            if (curRule >= (int)cd.rules.size()) curRule = -1;

            // 左欄:小隊清單
            ImGui::BeginChild("squads",
                              ImVec2(UITheme::Px(150, uiScale),
                                     UITheme::Px(300, uiScale)),
                              true);
            for (int i = 0; i < deck.SquadCount(); ++i) {
                const auto& d = deck.Deck(i);
                char lbl[64];
                std::snprintf(lbl, sizeof(lbl), "%s (%d/%d)",
                              d.squad ? d.squad->GetName().c_str() : "?",
                              (int)d.rules.size(), deck.SlotCap());
                if (ImGui::Selectable(lbl, i == curDeck)) {
                    curDeck = i;
                    curRule = -1;
                }
            }
            ImGui::EndChild();
            ImGui::SameLine();

            // 中欄:卡槽列
            ImGui::BeginChild("slots",
                              ImVec2(UITheme::Px(230, uiScale),
                                     UITheme::Px(300, uiScale)),
                              true);
            for (int i = 0; i < (int)cd.rules.size(); ++i) {
                const auto& r = cd.rules[i];
                char lbl[96];
                std::snprintf(lbl, sizeof(lbl), "[%d] %s(%.1f) → %s",
                              r.priority, TriggerName(r.trigger),
                              r.threshold, ActionName(r.action));
                ImGui::PushID(i);
                if (ImGui::Selectable(lbl, i == curRule)) curRule = i;
                ImGui::PopID();
            }
            if (ImGui::Button("＋ 新增卡") &&
                (int)cd.rules.size() < deck.SlotCap()) {
                deck.AddRule(curDeck, {DoctrineTrigger::Always,
                                       DoctrineAction::HoldPosition,
                                       0.0f, 90});
                curRule = (int)cd.rules.size() - 1;
            }
            ImGui::SameLine();
            if (ImGui::Button("－ 刪除") && curRule >= 0) {
                deck.RemoveRule(curDeck, curRule);
                --curRule;
            }
            ImGui::SameLine();
            if (ImGui::Button("↑") && curRule > 0) {
                deck.SwapRules(curDeck, curRule, curRule - 1);
                --curRule;
            }
            ImGui::SameLine();
            if (ImGui::Button("↓") && curRule >= 0 &&
                curRule < (int)cd.rules.size() - 1) {
                deck.SwapRules(curDeck, curRule, curRule + 1);
                ++curRule;
            }
            ImGui::EndChild();
            ImGui::SameLine();

            // 右欄:卡編輯器 + AI 理由
            ImGui::BeginChild("editor",
                              ImVec2(0, UITheme::Px(300, uiScale)),
                              true);
            if (curRule >= 0 && curRule < (int)cd.rules.size()) {
                DoctrineRule r = cd.rules[curRule];
                bool changed = false;

                static const DoctrineTrigger kTriggers[] = {
                    DoctrineTrigger::Always, DoctrineTrigger::HealthBelow,
                    DoctrineTrigger::MoraleBelow, DoctrineTrigger::EnemyInRange,
                    DoctrineTrigger::UnderAttack, DoctrineTrigger::Outnumbered,
                    DoctrineTrigger::AllyEngaged,
                    DoctrineTrigger::ObjectiveReached};
                static const DoctrineAction kActions[] = {
                    DoctrineAction::AttackNearest,
                    DoctrineAction::AttackWeakest,
                    DoctrineAction::AdvanceToObjective,
                    DoctrineAction::HoldPosition,
                    DoctrineAction::RetreatToRally,
                    DoctrineAction::DefendNearestAlly, DoctrineAction::Scout};

                int ti = 0, ai = 0;
                for (int i = 0; i < 8; ++i)
                    if (kTriggers[i] == r.trigger) ti = i;
                for (int i = 0; i < 7; ++i)
                    if (kActions[i] == r.action) ai = i;

                if (ImGui::BeginCombo("觸發條件", TriggerName(r.trigger))) {
                    for (int i = 0; i < 8; ++i) {
                        if (ImGui::Selectable(TriggerName(kTriggers[i]),
                                              i == ti)) {
                            r.trigger = kTriggers[i];
                            changed = true;
                        }
                    }
                    ImGui::EndCombo();
                }
                if (ImGui::BeginCombo("動作", ActionName(r.action))) {
                    for (int i = 0; i < 7; ++i) {
                        if (ImGui::Selectable(ActionName(kActions[i]),
                                              i == ai)) {
                            r.action = kActions[i];
                            changed = true;
                        }
                    }
                    ImGui::EndCombo();
                }
                changed |= ImGui::DragFloat("參數(血/士氣/距離)",
                                            &r.threshold, 0.05f, 0.0f, 20.0f);
                changed |= ImGui::InputInt("優先級", &r.priority);
                changed |= ImGui::DragFloat("冷卻秒", &r.cooldown, 0.5f,
                                            0.0f, 60.0f);
                if (changed) {
                    deck.SetRule(curDeck, curRule, r);
                    // priority 變動會重排——找回同一條規則的位置
                    for (int i = 0; i < (int)cd.rules.size(); ++i) {
                        const auto& x = cd.rules[i];
                        if (x.trigger == r.trigger && x.action == r.action &&
                            x.priority == r.priority &&
                            x.threshold == r.threshold) {
                            curRule = i;
                            break;
                        }
                    }
                }
            } else {
                ImGui::TextDisabled("選一張卡編輯");
            }
            ImGui::Separator();
            const std::string& ra = deck.Rationale(cd.squad);
            if (!ra.empty()) {
                ImGui::TextWrapped("AI 參謀: %s", ra.c_str());
            }
            ImGui::EndChild();

            // 底部:模板/警告/開戰
            ImGui::Separator();
            if (ImGui::Button("AI 參謀規劃")) {
                deck.LoadPlannerTemplate(planner, battle, 0);
                curRule = -1;
            }
            if (!deck.Summary().empty()) {
                ImGui::SameLine();
                ImGui::TextDisabled("%s", deck.Summary().c_str());
            }
            for (const auto& w : deck.Validate()) {
                ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f), "%s",
                                   w.c_str());
            }
            // G-5 計畫箭頭清單(有箭頭的小隊開戰後改走箭頭軸線)
            if (plan.ArrowCount() > 0) {
                ImGui::Separator();
                ImGui::TextDisabled(
                    "計畫箭頭 %d(加成 ×%.2f/情報+%d/CP+%d)",
                    (int)plan.ArrowCount(), plan.AttackMultiplier(),
                    plan.BonusIntel(), plan.BonusCP());
                for (size_t i = 0; i < plan.ArrowCount(); ++i) {
                    const PlanArrow& a = plan.Arrow(i);
                    ImGui::BulletText(
                        "%s → (%.0f,%.0f)",
                        a.squadName.empty() ? "全軍" : a.squadName.c_str(),
                        a.to.x, a.to.y);
                }
                if (ImGui::Button("清除箭頭")) plan.ClearArrows();
            }
            if (ImGui::Button("開 戰",
                              ImVec2(UITheme::Px(120, uiScale),
                                     UITheme::Px(32, uiScale)))) {
                deck.Commit(battle);
                // G-5:箭頭計畫在卡組之後套用——有箭頭的小隊
                // doctrine/目標點以箭頭軸線為準,加成經 res 入帳
                plan.SetRallyPoint(battle.GetRallyPoint(0));
                const int arrows = plan.Apply(battle, &res, 0);
                battle.BeginExecution();
                planningPhase = false;
                eventLog.push_back(
                    arrows > 0
                        ? "作戰計畫已下達——開戰(" +
                              std::to_string(arrows) + " 支箭頭生效)"
                        : "作戰計畫已下達——開戰");
            }
            ImGui::SameLine();
            ImGui::TextDisabled("寫好劇本再開戰;CP 留給救火");
            ImGui::End();
        }

        // ---- T-10 全軍狀態列:每隊兵力/士氣條 + CP 介入按鈕 ----
        if (!planningPhase) {
            ImGui::SetNextWindowPos(
                ImVec2((float)ww - UITheme::Px(272, uiScale), 8.0f),
                ImGuiCond_Always);
            ImGui::SetNextWindowSize(
                ImVec2(UITheme::Px(264, uiScale), 0),
                ImGuiCond_Always);
            ImGui::Begin("全軍", nullptr,
                         ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_AlwaysAutoResize);
            for (const auto& sq : battle.GetSquads()) {
                if (sq->GetTeam() != 0) continue;
                const bool sel = (sq.get() == selected);
                if (ImGui::Selectable(sq->GetName().c_str(), sel)) {
                    selected = sq.get();
                    sync.SetSelectedSquad(selected);
                }
                // 兵力條(綠→紅)
                const float hp = sq->GetHealthPct();
                ImGui::PushStyleColor(ImGuiCol_PlotHistogram,
                    ImVec4(1.0f - hp, hp * 0.85f, 0.15f, 1.0f));
                char hpLbl[48];
                std::snprintf(hpLbl, sizeof(hpLbl), "兵力 %d/%d",
                              sq->GetMembers(), sq->GetMaxMembers());
                ImGui::ProgressBar(hp, ImVec2(-1, 0), hpLbl);
                ImGui::PopStyleColor();
                // 士氣條(藍系;潰逃標紅)
                const float mo = sq->GetMorale();
                ImGui::PushStyleColor(ImGuiCol_PlotHistogram,
                    sq->IsRouting()
                        ? ImVec4(0.9f, 0.2f, 0.2f, 1.0f)
                        : ImVec4(0.25f, 0.45f + mo * 0.3f, 0.95f, 1.0f));
                char moLbl[48];
                std::snprintf(moLbl, sizeof(moLbl),
                              sq->IsRouting() ? "士氣 %.0f%% 潰逃中"
                                              : "士氣 %.0f%%",
                              mo * 100.0f);
                ImGui::ProgressBar(mo, ImVec2(-1, 0), moLbl);
                ImGui::PopStyleColor();
                ImGui::TextDisabled("命令: %s", OrderName(sq->GetOrder()));
                // quantum-plan:計畫加成依情報確定度即時浮動——
                // 雲未解析時低於滿額,坐實後升回 attackMul
                if (sq->GetPlanAttackMul() > 1.001f) {
                    ImGui::TextDisabled("計畫加成 ×%.2f",
                                        sq->GetPlanAttackMul());
                }
                ImGui::Separator();
            }

            // CP 介入按鈕列(對齊右鍵語義;CP 不足自動 disable)
            if (selected && !selected->IsEliminated()) {
                const int cp = battle.GetCommandPoints(0);
                ImGui::Text("CP 介入(%d)", cp);
                ImGui::BeginDisabled(cp <= 0);
                if (ImGui::Button("攻進目標")) {
                    if (battle.Intervene(selected, SquadOrder::AttackMove,
                                         battle.GetObjective(0))) {
                        eventLog.push_back("CP: " + selected->GetName() +
                                           " 攻進目標");
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("撤回集結")) {
                    if (battle.Intervene(selected, SquadOrder::Retreat,
                                         battle.GetRallyPoint(0))) {
                        eventLog.push_back("CP: " + selected->GetName() +
                                           " 撤退");
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("原地駐守")) {
                    if (battle.Intervene(selected, SquadOrder::Hold,
                                         selected->GetPosition())) {
                        eventLog.push_back("CP: " + selected->GetName() +
                                           " 駐守");
                    }
                }
                // G-8 將軍技能(僅衛隊顯示;同樣走 CP)
                if (selected->IsGeneralGuard()) {
                    ImGui::SameLine();
                    if (ImGui::Button("將軍激勵")) {
                        if (battle.GeneralRally(selected)) {
                            eventLog.push_back("將軍激勵:全軍振奮");
                        }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("帶隊突擊")) {
                        if (battle.GeneralCharge(selected)) {
                            eventLog.push_back("帶隊突擊:衛隊衝鋒!");
                        }
                    }
                }
                ImGui::EndDisabled();
            }
            ImGui::End();
        }

        // 事件流(ImGui 座標是 window 空間;視窗最小化時 wh=0,clamp 防負值)
        ImGui::SetNextWindowPos(
            ImVec2(8, std::max(0.0f,
                               (float)wh - UITheme::Px(190, uiScale))),
            ImGuiCond_Always);
        ImGui::SetNextWindowSize(
            ImVec2(UITheme::Px(430, uiScale),
                   UITheme::Px(182, uiScale)),
            ImGuiCond_Always);
        ImGui::Begin("戰況", nullptr,
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        for (const auto& e : eventLog) ImGui::TextUnformatted(e.c_str());
        ImGui::End();

        // ---- 小地圖（右下錨點;形狀編碼:■我 ◆敵 ●雲,DESIGN 強制——
        //      陣營不靠色相區分,色弱/主題切換下語義不變）----
        {
            const float ms = UITheme::Px(170, uiScale);
            ImGui::SetNextWindowPos(
                ImVec2((float)ww - ms - UITheme::Px(18, uiScale),
                       std::max(0.0f, (float)wh - ms -
                                          UITheme::Px(60, uiScale))),
                ImGuiCond_Always);
            ImGui::SetNextWindowSize(
                ImVec2(ms + UITheme::Px(16, uiScale),
                       ms + UITheme::Px(62, uiScale)),
                ImGuiCond_Always);
            ImGui::Begin("小地圖", nullptr,
                         ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoMove);
            ImDrawList* mdl = ImGui::GetWindowDrawList();
            const ImVec2 mp = ImGui::GetCursorScreenPos();
            const ImVec2 me(mp.x + ms, mp.y + ms);
            ImGui::Dummy(ImVec2(ms, ms));
            mdl->AddRectFilled(mp, me,
                               ImGui::GetColorU32(ImGuiCol_ChildBg));
            mdl->AddRect(mp, me, ImGui::GetColorU32(ImGuiCol_Border));
            auto toMap = [&](const Vector2& cell) {
                return ImVec2(mp.x + (cell.x / (float)GW) * ms,
                              mp.y + (cell.y / (float)GH) * ms);
            };
            const float mr = 3.4f * uiScale;
            // 集結點(情報金小圈)+ 計畫箭頭(細線)
            if (battle.HasRallyPoint(0)) {
                mdl->AddCircle(toMap(battle.GetRallyPoint(0)),
                               mr * 1.4f, IM_COL32(216, 168, 60, 220),
                               0, 1.5f);
            }
            for (size_t i = 0; i < plan.ArrowCount(); ++i) {
                mdl->AddLine(toMap(plan.Arrow(i).from),
                             toMap(plan.Arrow(i).to),
                             IM_COL32(216, 168, 60,
                                      planningPhase ? 200 : 90),
                             1.2f);
            }
            // 我方=方框(選取中描金邊)
            for (const auto& sq : battle.GetSquads()) {
                if (sq->GetTeam() != 0 || sq->IsEliminated()) continue;
                UITheme::DrawMarker(
                    mdl, toMap(sq->GetPosition()), mr,
                    UITheme::MarkerShape::FriendlySquare,
                    sq.get() == selected
                        ? IM_COL32(212, 162, 60, 255)   // myth gold
                        : IM_COL32(156, 176, 108, 255), // friendly
                    2.0f);
            }
            // 敵軍:揭露=菱形,未揭露=候選格雲圓(逐候選,機率雲本體)
            for (size_t id = 0; id < fog.EntityCount(); ++id) {
                const Squad* owner = battle.GetFogSquad((int)id);
                if (!owner || owner->IsEliminated()) continue;
                if (fog.IsRevealed((int)id)) {
                    UITheme::DrawMarker(
                        mdl, toMap(owner->GetPosition()), mr,
                        UITheme::MarkerShape::EnemyDiamond,
                        IM_COL32(176, 74, 50, 255), 2.0f);
                } else {
                    const ImU32 ccol =
                        ImGui::GetColorU32(UITheme::CloudColor(theme));
                    for (const auto& cand : fog.GetCloud((int)id)) {
                        UITheme::DrawMarker(
                            mdl, toMap(cand.first), mr * 0.7f,
                            UITheme::MarkerShape::CloudCircle, ccol);
                    }
                }
            }
            // 圖例(形狀+文字雙編碼)
            ImGui::TextDisabled("■我軍  ◆敵軍  ●敵情雲");
            ImGui::End();
        }

        // 戰果 banner
        if (battle.GetOutcome() != BattleOutcome::Ongoing) {
            const char* txt = battle.GetOutcome() == BattleOutcome::Victory ? "勝 利"
                            : battle.GetOutcome() == BattleOutcome::Defeat  ? "敗 北"
                                                                          : "平 手";
            ImVec4 col = battle.GetOutcome() == BattleOutcome::Victory
                             ? ImVec4(0.4f, 1.0f, 0.4f, 1.0f)
                         : battle.GetOutcome() == BattleOutcome::Defeat
                             ? ImVec4(1.0f, 0.4f, 0.4f, 1.0f)
                             : ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
            ImGui::SetNextWindowPos(
                ImVec2(ww * 0.5f - UITheme::Px(120, uiScale),
                       wh * 0.35f),
                ImGuiCond_Always);
            ImGui::SetNextWindowSize(
                ImVec2(UITheme::Px(240, uiScale), 0),
                ImGuiCond_Always);
            ImGui::Begin("##outcome", nullptr,
                         ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs |
                         ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SetWindowFontScale(2.2f);
            ImGui::TextColored(col, "%s", txt);
            ImGui::End();

            // ---- T-11 戰後層:史官筆戰報 + 名冊 + 回放時間軸 ----
            if (endedAt < 0.0f) {
                endedAt = (float)now;
                // N-1:史官體戰報改由片段組裝器產出(含省略計數)
                HistorianInput hin;
                hin.battleName = "斷橋之役";
                hin.outcome = battle.GetOutcome();
                hin.elapsedSec = battle.GetElapsed();
                hin.recorder = &recorder;
                hin.roster = &roster;
                chronicler = ComposeHistorianReport(hin).text;
            }
            // 逐字敲出(30 字/秒);退回 UTF-8 字元邊界,避免切出亂碼
            size_t shown =
                std::min(chronicler.size(),
                         (size_t)((now - endedAt) * 30.0));
            while (shown < chronicler.size() && shown > 0 &&
                   (chronicler[shown] & 0xC0) == 0x80) {
                --shown;
            }
            const float maxT = recorder.Count() > 0
                                   ? recorder.GetRecords().back().t : 0.0f;

            ImGui::SetNextWindowPos(
                ImVec2(ww * 0.5f - UITheme::Px(330, uiScale),
                       wh * 0.42f),
                ImGuiCond_Always);
            ImGui::SetNextWindowSize(
                ImVec2(UITheme::Px(660, uiScale), 0),
                ImGuiCond_Always);
            ImGui::Begin("戰後結算", nullptr,
                         ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::TextWrapped("%s", chronicler.substr(0, shown).c_str());
            ImGui::Separator();

            // 名冊:兩欄(我軍 | 敵軍)
            ImGui::Columns(2, "roster", true);
            ImGui::TextDisabled("我軍名冊");
            for (const auto& e : roster.GetEntries()) {
                if (e.team != 0) continue;
                if (e.alive) {
                    ImGui::TextColored(ImVec4(0.6f, 1.0f, 0.6f, 1.0f),
                                       "%s(%s)生還", e.name.c_str(),
                                       e.squadName.c_str());
                } else {
                    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f),
                                       "%s(%s)陣亡 %.0fs",
                                       e.name.c_str(), e.squadName.c_str(),
                                       e.deathTime);
                }
                if (!e.relic.empty())
                    ImGui::TextDisabled("  遺物:%s", e.relic.c_str());
            }
            ImGui::NextColumn();
            ImGui::TextDisabled("敵軍名冊");
            for (const auto& e : roster.GetEntries()) {
                if (e.team != 1) continue;
                ImGui::TextColored(
                    e.alive ? ImVec4(0.9f, 0.8f, 0.4f, 1.0f)
                            : ImVec4(0.7f, 0.7f, 0.7f, 1.0f),
                    "%s(%s)%s", e.name.c_str(), e.squadName.c_str(),
                    e.alive ? "在逃" : "授首");
            }
            ImGui::Columns(1);
            ImGui::Separator();

            // 回放時間軸:拖曳跳到事件點
            ImGui::Text("回放時間軸(%zu 則)", recorder.Count());
            ImGui::SliderFloat("##timeline", &replayCursor, 0.0f, maxT,
                               "%.1fs");
            ImGui::BeginChild("replaylist",
                              ImVec2(0, UITheme::Px(130, uiScale)),
                              true);
            int lastIdx = -1;
            for (int i = 0; i < (int)recorder.Count(); ++i) {
                if (recorder.GetRecords()[i].t <= replayCursor) lastIdx = i;
            }
            const int from = std::max(0, lastIdx - 9);
            for (int i = from; i <= lastIdx; ++i) {
                const auto& rec = recorder.GetRecords()[i];
                ImGui::TextDisabled("[%5.1fs] %s", rec.t,
                                    rec.event.c_str());
            }
            if (lastIdx >= 0) ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();
            ImGui::Separator();
            if (ImGui::Button("返回主選單",
                              ImVec2(UITheme::Px(150, uiScale),
                                     UITheme::Px(30, uiScale)))) {
                backToTitle = true;
            }
            ImGui::End();
        }

        ImGui::PopFont();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        renderer.SwapBuffers();
    }

    // G-9 戰後收口:戰鬥有結果才結算——傷亡/戰利品/遺物轉存整補營
    if (battle.GetOutcome() != BattleOutcome::Ongoing) {
        const bool iWon = battle.GetOutcome() == BattleOutcome::Victory;
        const int winnerTeam = iWon ? 0
            : battle.GetOutcome() == BattleOutcome::Defeat ? 1 : -1;
        PostBattle postBattle;
        PostBattleReport report = postBattle.Settle(battle, roster, winnerTeam);
        // 戰利品與遺物是勝者的拾獲;敗北/平手我軍不入帳
        if (!iWon) {
            report.lootPoints = 0;
            report.relics.clear();
        }
        camp.DepositLoot(report.lootPoints);
        camp.Absorb(report, roster, 0);

        // C-1 章節邊界存檔：勝利且章節定義有 next 才跳章，
        // 任一結果都寫出 campaign 檔（戰果/傷亡跨場持續）
        if (iWon) {
            if (const Campaign::ChapterDef* cur =
                    chapters.Find(campaign.chapter.chapterId)) {
                // B.5 已收戰線：勝場記入 frontsTaken（去重），
                // 供章節地圖殼顯示 ◆ 標記
                if (std::find(campaign.chapter.frontsTaken.begin(),
                              campaign.chapter.frontsTaken.end(),
                              cur->id) ==
                    campaign.chapter.frontsTaken.end()) {
                    campaign.chapter.frontsTaken.push_back(cur->id);
                }
                if (!cur->next.empty()) {
                    if (const Campaign::ChapterDef* nxt =
                            chapters.Find(cur->next)) {
                        campaign.AdvanceChapter(nxt->arc, nxt->chapter,
                                                nxt->id);
                    }
                } else {
                    // 無 next：同弧章節序 +1（chapterId 維持，
                    // 內容側未定義次章時不換章）
                    campaign.AdvanceChapter(
                        campaign.chapter.arc,
                        campaign.chapter.chapter + 1,
                        campaign.chapter.chapterId);
                }
            }
        }
        // C-2 治理折帳:本場治理事件計數折進戰役帳,動亂事件入史官筆
        campaign.Gov().Accumulate(battle.GetGovernanceEvents());
        for (const auto& msg : campaign.Gov().PollUnrestEvents()) {
            chronicler += msg + "\n";
        }
        {
            std::error_code ec;
            std::filesystem::create_directories(
                std::filesystem::path(kCampaignSave).parent_path(), ec);
        }
        campaign.SaveToFile(kCampaignSave);
    }

    battle.BindFog(nullptr); // fog 是 local,先於 battle 解構——解綁防懸空
    battle.BindPlan(nullptr); // plan 同為 local——一併解綁

        } // ---- 戰鬥場次 scope 結束:battle/fog/scene 全數析構 ----
        screen = renderer.ShouldClose() ? ShellScreen::Quit
                                        : ShellScreen::Title;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    renderer.Shutdown();
    return 0;
}
