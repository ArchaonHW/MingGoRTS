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
#include "Gameplay/BattlePlanner.h"
#include "Gameplay/PlanningDeck.h"
#include "Gameplay/EnemyGeneral.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/QuantumFog.h"
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
        font = io.Fonts->AddFontFromFileTTF(kFont, 18.0f, nullptr,
                                            io.Fonts->GetGlyphRangesChineseFull());
    }
    if (!font) io.Fonts->AddFontDefault(); // 字型缺檔/損毀也要保證 atlas 非空

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

    // ---- 地圖 ----
    BattleMap map;
    if (!map.LoadFromFile("assets/maps/duanqiao.json")) {
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
    battle.SetEventCallback([&](const std::string& e) {
        eventLog.push_back(e);
        if (eventLog.size() > 8) eventLog.pop_front();
        std::printf("  [戰報] %s\n", e.c_str());
    });

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
    battle.CreateSquad("前鋒", 0, Vector2(8.0f, 3.0f),  30);
    battle.CreateSquad("中軍", 0, Vector2(12.0f, 2.5f), 30);
    battle.CreateSquad("左翼", 0, Vector2(16.0f, 3.0f), 25);
    Squad* rearGuard = battle.CreateSquad("後衛", 0, Vector2(12.0f, 4.5f), 15);
    Squad* e0 = battle.CreateSquad("格洛克親衛", 1, Vector2(12.0f, 12.0f), 35);
    Squad* e1 = battle.CreateSquad("蠻兵隊",     1, Vector2(9.0f, 12.5f),  25);
    Squad* e2 = battle.CreateSquad("掠奪隊",     1, Vector2(15.0f, 12.5f), 25);
    Squad* e3 = battle.CreateSquad("守橋隊",     1, Vector2(12.0f, 10.5f), 20);
    if (southCamp)  battle.SetObjective(0, southCamp->pos);
    if (northRally) battle.SetObjective(1, northRally->pos);
    if (northRally) battle.SetRallyPoint(0, northRally->pos);
    if (southCamp)  battle.SetRallyPoint(1, southCamp->pos);

    EnemyGeneral glock = EnemyGeneral::MakeGlock();
    glock.ApplyTo(battle, 1);
    BattlePlanner planner;
    // 情報/CP 走 BattleResources(fog 觀測扣點要它)
    BattleResources res;
    res.Setup(battle, 0, /*intel=*/8, /*cp=*/5);
    res.Setup(battle, 1, /*intel=*/0, /*cp=*/3);
    BattleResources::ApplyMoraleRule(battle);

    // ---- Q-1/Q-3 敵情霧:敵軍以疊加態存在,觀測或接觸才塌縮 ----
    // 觀測 2 情報全額買斷;探測 1 情報讓雲向真值收縮(弱觀測)
    QuantumFog fog(/*intelDuration=*/25.0f, /*observe=*/2, /*probe=*/1);
    fog.BindResources(&res);
    battle.BindFog(&fog);
    for (Squad* es : {e0, e1, e2, e3}) {
        // 雲心朝敵軍進攻方向偏移 1.5 格:不洩漏真實位置,
        // 靠偵查/觀測才能確定敵人在哪
        Vector2 center = es->GetPosition();
        if (northRally) {
            Vector2 dir = northRally->pos - center;
            const float len = dir.Length();
            if (len > 1e-4f) center = center + dir * (1.5f / len);
        }
        const int id = fog.AddEntityCloud(
            es->GetName(), /*觀測方=*/0, center,
            /*radius=*/3.5f, /*count=*/6, /*minSpacing=*/1.2f,
            southCamp ? &southCamp->pos : nullptr);
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
    bool paused = false;
    float speedScale = 1.0f;
    bool prevL = false, prevR = false, prevSpace = false, prevEsc = false;
    double prevTime = glfwGetTime();

    std::printf("斷橋可玩 demo — 左鍵選取,右鍵下令(CP),Space 暫停\n");

    while (!renderer.ShouldClose()) {
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

        ImGui::SetNextWindowPos(ImVec2(8, 8), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_Always);
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
        ImGui::Text("CP: %d   情報: %d   倍速: %.1fx   時間: %.0fs",
                    battle.GetCommandPoints(0), res.GetIntel(0),
                    paused ? 0.0f : speedScale, battle.GetElapsed());
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
        if (planningPhase)
            ImGui::TextDisabled("Alt+左鍵=移目標點 | Alt+右鍵=移集結點");
        ImGui::End();

        // ---- T-9 回合層:作戰計畫視窗(三欄:小隊/卡槽/編輯器)----
        if (planningPhase) {
            ImGui::SetNextWindowPos(ImVec2(ww * 0.5f - 330, 40),
                                    ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(660, 420), ImGuiCond_FirstUseEver);
            ImGui::Begin("作戰計畫 — 戰前軍議", nullptr,
                         ImGuiWindowFlags_NoCollapse);

            if (curDeck >= deck.SquadCount()) curDeck = 0;
            auto& cd = deck.Deck(curDeck);
            if (curRule >= (int)cd.rules.size()) curRule = -1;

            // 左欄:小隊清單
            ImGui::BeginChild("squads", ImVec2(150, 300), true);
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
            ImGui::BeginChild("slots", ImVec2(230, 300), true);
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
            ImGui::BeginChild("editor", ImVec2(0, 300), true);
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
            if (ImGui::Button("開 戰", ImVec2(120, 32))) {
                deck.Commit(battle);
                battle.BeginExecution();
                planningPhase = false;
                eventLog.push_back("作戰計畫已下達——開戰");
            }
            ImGui::SameLine();
            ImGui::TextDisabled("寫好劇本再開戰;CP 留給救火");
            ImGui::End();
        }

        // ---- T-10 全軍狀態列:每隊兵力/士氣條 + CP 介入按鈕 ----
        if (!planningPhase) {
            ImGui::SetNextWindowPos(ImVec2((float)ww - 272.0f, 8.0f),
                                    ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(264, 0), ImGuiCond_Always);
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
                ImGui::EndDisabled();
            }
            ImGui::End();
        }

        // 事件流(ImGui 座標是 window 空間;視窗最小化時 wh=0,clamp 防負值)
        ImGui::SetNextWindowPos(ImVec2(8, std::max(0.0f, (float)wh - 190.0f)),
                                ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(430, 182), ImGuiCond_Always);
        ImGui::Begin("戰況", nullptr,
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        for (const auto& e : eventLog) ImGui::TextUnformatted(e.c_str());
        ImGui::End();

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
            ImGui::SetNextWindowPos(ImVec2(ww * 0.5f - 120, wh * 0.35f),
                                    ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(240, 0), ImGuiCond_Always);
            ImGui::Begin("##outcome", nullptr,
                         ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs |
                         ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SetWindowFontScale(2.2f);
            ImGui::TextColored(col, "%s", txt);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        renderer.SwapBuffers();
    }

    battle.BindFog(nullptr); // fog 是 local,先於 battle 解構——解綁防懸空
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    renderer.Shutdown();
    return 0;
}
