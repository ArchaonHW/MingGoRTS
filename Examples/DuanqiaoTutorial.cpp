// DuanqiaoTutorial - Q-9 量子偵查教學情境
// 三段引導:觀測(LMB) → 探測(RMB 或 P+LMB) → 情報時效回雲;R 重置重看
// 吃 DuanqiaoPlayable 既有元件(BattleController/QuantumFog/
// BattleSceneSync),12x8 最小戰場 + 3 敵雲 + 1 偵查隊,全離線無外部資產。
// 無顯示環境印 [SKIP] 並以 0 結束(不進 POTATO_TESTS)。

#include "Rendering/OpenGLRenderer.h"
#include "Rendering/SceneRenderer.h"
#include "Rendering/RenderableComponent.h"
#include "Rendering/Camera.h"
#include "Scene/SceneNode.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/BattlePicker.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/BattleSceneSync.h"
#include "Gameplay/QuantumFog.h"
#include "Gameplay/TutorialScript.h"
#include "MathUtils/Matrix4.h"

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

// ---- 程序化 mesh(與 DuanqiaoPlayable 相同的已驗證繞序)----
static Mesh MakeBox(float w, float h, float d) {
    Mesh m;
    float x = w * 0.5f, y = h * 0.5f, z = d * 0.5f;
    Vector3 n[6] = {{0,0,1},{0,0,-1},{1,0,0},{-1,0,0},{0,1,0},{0,-1,0}};
    Vector3 quad[6][4] = {
        {{-x,-y,z},{x,-y,z},{x,y,z},{-x,y,z}},
        {{x,-y,-z},{-x,-y,-z},{-x,y,-z},{x,y,-z}},
        {{-x,-y,-z},{-x,-y,z},{-x,y,z},{-x,y,-z}},
        {{-x,y,z},{x,y,z},{x,y,-z},{-x,y,-z}},
        {{-x,-y,-z},{x,-y,-z},{x,-y,z},{-x,y,-z}},
        {{-x,-y,z},{x,y,z},{x,y,-z},{-x,y,-z}},
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
        idx.insert(idx.end(),
                   {base, base + 1, base + 2, base, base + 2, base + 3});
    }
    m.SetVertices(verts);
    m.SetIndices(idx);
    return m;
}

// 平放環:XZ 平面同心圓環(高亮當前教學目標雲用)
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
static void ScrollCallback(GLFWwindow*, double, double yoff) {
    g_scroll += yoff;
}

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

int main() {
    const int W = 1280, H = 720;

    OpenGLRenderer renderer;
    RendererConfig rcfg;
    rcfg.window.width = W;
    rcfg.window.height = H;
    rcfg.window.title = "MingGoRTS - 量子偵查教學";
    rcfg.window.vsync = true;
    renderer.SetConfig(rcfg);
    if (!renderer.Initialize()) {
        std::fprintf(stderr, "[SKIP] no display / renderer init failed\n");
        return 0;
    }
    GLFWwindow* window = static_cast<GLFWwindow*>(renderer.GetWindowHandle());
    glfwSetScrollCallback(window, ScrollCallback);

    // ImGui(中文字型:微軟正黑體;缺檔退回內建字,中文會顯示 ?)
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGuiIO& io = ImGui::GetIO();
    const char* kFont = "C:/Windows/Fonts/msjh.ttc";
    ImFont* font = nullptr;
    if (std::filesystem::exists(kFont)) {
        font = io.Fonts->AddFontFromFileTTF(
            kFont, 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    }
    if (!font) io.Fonts->AddFontDefault(); // 字型缺檔也要保證 atlas 非空

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

    // ---- 最小戰場 12x8,無地形(不依賴外部檔案)----
    const float CELL = 2.0f;
    const int GW = 12, GH = 8;
    const float FW = GW * CELL, FH = GH * CELL;

    // 事件流(HUD + console);在 battle 之前宣告,避免解構順序留下懸空 callback
    std::deque<std::string> eventLog;
    auto logEvent = [&](const std::string& e) {
        eventLog.push_back(e);
        if (eventLog.size() > 6) eventLog.pop_front();
        std::printf("  [戰報] %s\n", e.c_str());
    };

    BattleController battle(GW, GH, CELL);
    battle.SetEventCallback(logEvent);
    BattleResources res;

    // 玩家偵查隊(固定駐守;教學只教觀測循環,不教移動)
    battle.CreateSquad("偵查隊", 0, Vector2(2, 4), 20);

    // 三朵敵情雲:目標 A/B + 一朵自由探索用
    Squad* eA = battle.CreateSquad("假想敵·甲", 1, Vector2(8, 2), 15);
    Squad* eB = battle.CreateSquad("假想敵·乙", 1, Vector2(9, 5), 15);
    Squad* eC = battle.CreateSquad("假想敵·丙", 1, Vector2(7, 6), 15);

    // ---- 教學狀態機 + fog ----
    // fog 用固定位址的 stack 物件 + move-assign 重建,BindFog/SetFog 的
    // 指標在 R 重置後仍然有效,不用重新綁指標
    constexpr int kFogEntities = 3;
    constexpr int kObserveCost = 2, kProbeCost = 1; // 對齊下方 fog 建構參數
    QuantumFog fog;
    TutorialScript script;
    BattleSceneSync sync; // 提前宣告,讓 setupFog 能 SetFog 清舊標記節點
    Squad* enemySquads[kFogEntities] = {eA, eB, eC};
    int fogIds[kFogEntities] = {-1, -1, -1};

    auto setupFog = [&]() {
        // 時效縮短到 8s:教學不需要等 25s
        fog = QuantumFog(/*duration=*/8.0f, kObserveCost, kProbeCost);
        fog.BindResources(&res);
        fogIds[0] = fog.AddEntity(eA->GetName(), 0,
            {Vector2(7, 2), Vector2(8, 2), Vector2(9, 2), Vector2(8, 3)},
            {0.3, 0.4, 0.2, 0.1});
        fogIds[1] = fog.AddEntity(eB->GetName(), 0,
            {Vector2(8, 5), Vector2(9, 5), Vector2(10, 5), Vector2(9, 4)},
            {0.25, 0.35, 0.25, 0.15});
        fogIds[2] = fog.AddEntity(eC->GetName(), 0,
            {Vector2(6, 6), Vector2(7, 6), Vector2(8, 6)},
            {0.4, 0.4, 0.2});
        for (int i = 0; i < kFogEntities; ++i) {
            if (fogIds[i] < 0) {
                // 少一朵雲教學就走不完——至少要留診斷
                logEvent("敵情雲建立失敗(entity " + std::to_string(i) +
                         ")");
                continue;
            }
            battle.BindFogSquad(enemySquads[i], fogIds[i]);
        }
        battle.BindFog(&fog); // 接線 fog 事件 → 戰報流
        sync.SetFog(&fog);    // 重置時拆掉舊雲標記節點再重建
        // 重置時補滿情報,否則重看會卡在沒情報可用。
        // 12 點:正解路線最多花 5(觀測2+探測1+過期補觀測2),其餘留給自由探索
        res.Setup(battle, 0, /*intel=*/12, /*cp=*/0);
    };
    setupFog();
    script = TutorialScript(fogIds[0], fogIds[1]);

    battle.BeginExecution();
    battle.SetTimeScale(1.0f);

    // ---- 場景 ----
    SceneGraph scene;
    auto root = MakeShared<SceneNode>("root");
    scene.SetRootNode(root);

    auto groundMesh = MakeShared<Mesh>(MakeBox(1.0f, 0.3f, 1.0f));
    auto capMesh    = MakeShared<Mesh>(MakeBox(0.4f * CELL, 1.0f * CELL,
                                              0.4f * CELL));
    auto ringMesh   = MakeShared<Mesh>(MakeRing(0.55f * CELL, 0.75f * CELL));
    auto barBgMesh  = MakeShared<Mesh>(MakeBox(1.0f, 0.16f, 0.03f));
    auto barFillMesh = MakeShared<Mesh>(MakeBox(0.96f, 0.11f, 0.04f));
    auto cloudMesh  = MakeShared<Mesh>(MakeBox(0.7f, 0.7f, 0.7f));
    auto hlMesh     = MakeShared<Mesh>(MakeRing(0.9f * CELL, 1.05f * CELL));

    AddStaticBox(root, groundMesh, Vector3(FW / 2, -0.15f, FH / 2),
                 Vector3(0.18f, 0.24f, 0.17f), "ground", FW)
        ->SetLocalScale(Vector3(FW, 1, FH));

    // 目標高亮環(跟著當前步驟的雲 modal 候選走)
    // 包圍半徑須 ≥ 環外緣 1.05*CELL,否則在視錐邊緣會被誤剔除
    auto hlNode = AddStaticBox(root, hlMesh, Vector3(0, 0.3f, 0),
                               Vector3(1.0f, 0.85f, 0.2f), "highlight",
                               1.1f * CELL);

    sync.Attach(battle, scene, CELL);
    sync.SetUnitMesh(capMesh);
    sync.SetOverlayMeshes(ringMesh, barBgMesh, barFillMesh,
                          1.4f * CELL, 1.2f);
    sync.SetFog(&fog);
    sync.SetFogMarkerMesh(cloudMesh);
    sync.Sync(battle);

    // ---- 相機:高位俯視,WASD 平移、滾輪升降 ----
    Camera cam;
    cam.SetViewport(0, 0, W, H);
    cam.SetPerspective(50.0f * 3.14159265f / 180.0f, (float)W / H,
                       0.1f, 300.0f);
    Vector3 camTarget(FW / 2, 0.0f, FH / 2);
    float camHeight = 22.0f, camBack = 11.0f;

    SceneRenderer sceneRenderer;
    sceneRenderer.SetDefaultShader(unitShader);

    // 時效偵測:記錄上一幀各 entity 揭露態,曾揭露→未揭露 = 過期事件
    bool prevRevealed[kFogEntities] = {};
    std::string nudge;
    float nudgeTimer = 0.0f;

    bool prevL = false, prevR = false, prevRkey = false;
    double prevTime = glfwGetTime();

    std::printf("量子偵查教學 — LMB 觀測,RMB/P+LMB 探測,R 重置\n");

    while (!renderer.ShouldClose()) {
        renderer.PollEvents();
        const double now = glfwGetTime();
        const float dt = (float)std::min(now - prevTime, 0.1);
        prevTime = now;

        // framebuffer 用於 GL viewport;window size 用於 ImGui/游標(HiDPI)
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

        // ---- 鍵盤:R 重置教學 ----
        const bool rKey = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
        if (rKey && !prevRkey) {
            setupFog();
            script = TutorialScript(fogIds[0], fogIds[1]);
            for (int i = 0; i < kFogEntities; ++i) prevRevealed[i] = false;
            nudge.clear();
            nudgeTimer = 0.0f;
            logEvent("教學重置——從觀測重來");
        }
        prevRkey = rKey;

        const float pan = 10.0f * dt;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camTarget.z -= pan;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camTarget.z += pan;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camTarget.x -= pan;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camTarget.x += pan;
        camTarget.x = std::max(0.0f, std::min(camTarget.x, FW));
        camTarget.z = std::max(0.0f, std::min(camTarget.z, FH));
        // 滾輪在 ImGui 視窗上時不縮放相機
        if (!io.WantCaptureMouse) {
            camHeight = std::max(10.0f,
                std::min(camHeight - (float)g_scroll * 2.0f, 40.0f));
        }
        g_scroll = 0.0;
        cam.SetPosition(Vector3(camTarget.x, camHeight,
                                camTarget.z + camBack));
        cam.SetTarget(camTarget);

        battle.Update(dt);

        // 時效偵測(完成條件由 fog 狀態輪詢判定)
        for (int i = 0; i < kFogEntities; ++i) {
            const bool rv = fog.IsRevealed(fogIds[i]);
            if (prevRevealed[i] && !rv) {
                if (script.Advance(TutorialEvent::Expired, fogIds[i])) {
                    logEvent("情報過期——真身退回機率雲");
                }
            }
            prevRevealed[i] = rv;
        }

        // ---- 滑鼠:LMB 觀測 / RMB 或 P+LMB 探測 ----
        double mx, my;
        glfwGetCursorPos(window, &mx, &my);
        const float fmx = (float)mx * sx, fmy = (float)my * sy;
        const bool cursorIn = fmx >= 0.0f && fmy >= 0.0f &&
                              fmx < (float)dw && fmy < (float)dh;
        const bool lmb =
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) ==
                GLFW_PRESS;
        const bool rmb =
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) ==
                GLFW_PRESS;
        const bool lClick = lmb && !prevL, rClick = rmb && !prevR;
        prevL = lmb; prevR = rmb;

        if (cursorIn && !io.WantCaptureMouse && (lClick || rClick)) {
            const PickRay ray =
                BattlePicker::ScreenToWorldRay(cam, fmx, fmy);
            const int eid = PickFogCloud(fog, battle, ray, CELL);
            bool showNudge = false;
            if (eid >= 0) {
                Squad* ts = battle.GetFogSquad(eid);
                // LMB=觀測;RMB 或按住 P 的 LMB=探測(spec:Probe 鍵+點雲)
                const bool probeKey =
                    glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
                const bool wantProbe = rClick || probeKey;
                bool ok = false;
                TutorialEvent ev = TutorialEvent::None;
                if (wantProbe) {
                    ok = ts && fog.Probe(eid, ts->GetPosition(), 0.4f);
                    ev = TutorialEvent::Probed;
                    if (ok) {
                        double mp = 0.0;
                        for (const auto& c : fog.GetCloud(eid))
                            mp = std::max(mp, c.second);
                        logEvent("探測收縮:" + ts->GetName() + "(最高" +
                                 std::to_string(int(mp * 100)) + "%)");
                    } else {
                        // 情報不足/目標無效都有可能,不硬編原因
                        logEvent("探測失敗");
                    }
                } else {
                    ok = ts && fog.Observe(eid, ts->GetPosition());
                    ev = TutorialEvent::Observed;
                    logEvent(ok ? "觀測塌縮:" + ts->GetName()
                                : "觀測失敗");
                }
                // 失敗的操作沒有事件可餵;成功但非當前步驟也提示
                // (自由探索不禁用,但要讓玩家知道教學期待什麼)
                showNudge = !ok || !script.Advance(ev, eid);
            } else {
                // 沒點到任何雲也給提示,別讓玩家對空地乾點
                showNudge = true;
            }
            if (showNudge) {
                nudge = script.Nudge();
                nudgeTimer = 3.0f;
            }
        }
        if (nudgeTimer > 0.0f) nudgeTimer -= dt;

        // ---- 高亮環跟隨當前目標雲的最高機率候選 ----
        {
            const int tgt = script.TargetEntity();
            bool hl = false;
            if (tgt >= 0 && !fog.IsRevealed(tgt)) {
                const auto cloud = fog.GetCloud(tgt);
                if (!cloud.empty()) {
                    auto best = cloud.begin();
                    for (auto it = cloud.begin(); it != cloud.end(); ++it)
                        if (it->second > best->second) best = it;
                    hlNode->SetLocalPosition(Vector3(
                        best->first.x * CELL, 0.3f,
                        best->first.y * CELL));
                    hl = true;
                }
            }
            hlNode->SetActive(hl);
        }

        sync.Sync(battle);

        // ---- 渲染 ----
        renderer.SetClearColor(Vector3(0.07f, 0.09f, 0.13f));
        renderer.Clear();
        renderer.EnableDepthTest(true);
        renderer.EnableCulling(false); // 手排頂點繞序不保證 CCW
        sceneRenderer.Render(scene, cam);

        // ---- HUD ----
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 教學面板(左上):步驟指示 + 進度 + 提示
        ImGui::SetNextWindowPos(ImVec2(8, 8), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(330, 0), ImGuiCond_Always);
        ImGui::Begin("量子偵查教學", nullptr,
                     ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_AlwaysAutoResize);
        // 進度條:完成的步數 / 3
        char progLbl[32];
        std::snprintf(progLbl, sizeof(progLbl), "進度 %d/%d",
                      std::min(script.StepIndex(),
                               TutorialScript::StepCount()),
                      TutorialScript::StepCount());
        ImGui::ProgressBar(
            (float)std::min(script.StepIndex(),
                            TutorialScript::StepCount()) /
                (float)TutorialScript::StepCount(),
            ImVec2(-1, 0), progLbl);

        // 步驟所需情報:觀測 2 / 探測 1;時效步若已有揭露中的雲,
        // 等它自然過期不需再花情報
        bool anyRevealed = false;
        for (int i = 0; i < kFogEntities; ++i)
            anyRevealed = anyRevealed || fog.IsRevealed(fogIds[i]);
        int neededIntel = 0;
        switch (script.Step()) {
        case TutorialStep::Observe: neededIntel = kObserveCost; break;
        case TutorialStep::Probe:   neededIntel = kProbeCost; break;
        case TutorialStep::Expire:
            neededIntel = anyRevealed ? 0 : kObserveCost;
            break;
        default: break;
        }
        const bool intelLocked = res.GetIntel(0) < neededIntel;

        if (intelLocked) {
            // 情報不夠付當前步驟 → 軟鎖警告取代一般提示
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 18.0f);
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.3f, 1.0f),
                               "情報不足——按 R 重置");
            ImGui::PopTextWrapPos();
        } else {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 18.0f);
            ImGui::TextUnformatted(script.Hint());
            ImGui::PopTextWrapPos();
        }
        if (nudgeTimer > 0.0f && !nudge.empty()) {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 18.0f);
            ImGui::TextColored(ImVec4(1, 0.8f, 0.3f, 1), "%s",
                               nudge.c_str());
            ImGui::PopTextWrapPos();
        }
        // 步驟目標已被觀測成真身(如步驟 2 誤用觀測):真身不可再點、
        // 高亮也會消失——提示等它回雲或直接重置
        {
            const int tgt = script.TargetEntity();
            if (tgt >= 0 && fog.IsRevealed(tgt)) {
                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 18.0f);
                ImGui::TextColored(ImVec4(1, 0.8f, 0.3f, 1),
                                   "目標已被觀測——等待回雲或按 R");
                ImGui::PopTextWrapPos();
            }
        }
        // 時效步驟的保險提示:進步驟 3 時若目標早已過期回雲,
        // 玩家等不到 expire 事件——提示再觀測一次
        if (script.Step() == TutorialStep::Expire && !anyRevealed &&
            !intelLocked) {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 18.0f);
            ImGui::TextColored(ImVec4(1, 0.8f, 0.3f, 1),
                               "目前沒有揭露中的敵軍——再左鍵觀測一朵雲,"
                               "等它情報過期");
            ImGui::PopTextWrapPos();
        }
        ImGui::Separator();
        ImGui::Text("情報點: %d   時間: %.0fs", res.GetIntel(0),
                    battle.GetElapsed());
        ImGui::TextDisabled("LMB 點雲=觀測(2情報) | RMB/P+點雲=探測(1情報)");
        ImGui::TextDisabled("R 重置 | WASD 平移 | 滾輪縮放");
        ImGui::End();

        // 戰況流(左下)
        ImGui::SetNextWindowPos(ImVec2(8, std::max(0.0f, (float)wh - 160.0f)),
                                ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(400, 152), ImGuiCond_Always);
        ImGui::Begin("戰況", nullptr,
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        for (const auto& e : eventLog) ImGui::TextUnformatted(e.c_str());
        ImGui::End();

        // 完成總結面板
        if (script.IsDone()) {
            ImGui::SetNextWindowPos(ImVec2(ww * 0.5f - 210.0f, wh * 0.3f),
                                    ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(420, 0), ImGuiCond_Always);
            ImGui::Begin("教學總結", nullptr,
                         ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f),
                               "三段引導完成!");
            ImGui::Separator();
            ImGui::BulletText("觀測:花全額情報點,雲塌縮見真身位置");
            ImGui::BulletText("探測:花少量情報,雲向真值收縮但不塌縮");
            ImGui::BulletText("時效:情報會過期,真身退回機率雲");
            ImGui::Separator();
            ImGui::TextWrapped("偵查隊還在,雲也還在——自由探索,"
                               "或按 R 重新演練。");
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        renderer.SwapBuffers();
    }

    battle.BindFog(nullptr); // fog 是 local,先於 battle 解構——解綁防懸空
    shutdownAll();
    return 0;
}
