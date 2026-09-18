// BattleRenderDemo - 斷橋攻防戰即時渲染 → MP4
// 管線: BattleController(doctrine 執行) → BattleSceneSync → SceneGraph
//       → CollectVisibleNodes(視錐剔除) → SceneRenderer → OpenGL → ffmpeg
// 用法: BattleRenderDemo.exe <輸出mp4> [秒數] [fps]

#include "Rendering/OpenGLRenderer.h"
#include "Rendering/SceneRenderer.h"
#include "Rendering/RenderableComponent.h"
#include "Rendering/Camera.h"
#include "Scene/SceneNode.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/BattleSceneSync.h"
#include "Gameplay/Doctrine.h"
#include "MathUtils/Matrix4.h"
#include "Media/VideoEncoder.h"

#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <string>

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

// ---- 單位立方體 mesh ----
static Mesh MakeBox(float w, float h, float d) {
    Mesh m;
    float x = w * 0.5f, y = h * 0.5f, z = d * 0.5f;
    Vector3 n[6] = {{0,0,1},{0,0,-1},{1,0,0},{-1,0,0},{0,1,0},{0,-1,0}};
    // 每面 4 頂點(順時針從外看)
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
            vx.texCoord = Vector2(0,0);
            verts.push_back(vx);
        }
        idx.insert(idx.end(), {base, base+1, base+2, base, base+2, base+3});
    }
    m.SetVertices(verts);
    m.SetIndices(idx);
    return m;
}

// 掛一個靜態地形節點(地形/牆/橋)
static void AddStaticBox(SharedPtr<SceneNode> parent, SharedPtr<Mesh> mesh,
                         const Vector3& pos, const Vector3& color,
                         const char* name, float boundR) {
    auto node = MakeShared<SceneNode>(name);
    node->SetLocalPosition(pos);
    auto rc = MakeShared<RenderableComponent>();
    rc->mesh = mesh;
    rc->color = color;
    rc->boundingRadius = boundR;
    node->SetRenderable(rc);
    parent->AddChild(node);
}

int main(int argc, char** argv) {
    std::string outPath = (argc > 1) ? argv[1] : "battle_demo.mp4";
    float duration = (argc > 2) ? (float)atof(argv[2]) : 15.0f;
    int   fps      = (argc > 3) ? atoi(argv[3]) : 30;
    const int W = 1280, H = 720;
    const int totalFrames = (int)(duration * fps);
    const float CELL = 2.0f;                 // grid → world
    const float FIELD_W = 20 * CELL;         // 40
    const float FIELD_H = 15 * CELL;         // 30

    std::cout << "=== Battle Render Demo ===" << std::endl;
    std::cout << "Output: " << outPath << " | " << duration << "s @ " << fps << "fps" << std::endl;

    OpenGLRenderer renderer;
    RendererConfig rcfg;
    rcfg.window.width = W; rcfg.window.height = H;
    rcfg.window.title = "MingGoRTS - Battle";
    rcfg.window.vsync = false;
    renderer.SetConfig(rcfg);
    if (!renderer.Initialize()) { std::cerr << "Renderer init failed\n"; return 1; }
    renderer.SetViewport(0, 0, W, H);

    auto unitShader = MakeShared<Shader>();
    if (!unitShader->LoadFromSource(UNIT_VERT, UNIT_FRAG)) {
        std::cerr << "Shader compile failed\n"; return 1;
    }
    unitShader->Bind();
    unitShader->SetUniformVec3("uLightDir", Vector3(-0.4f, 1.0f, -0.35f));

    auto boxMesh = MakeShared<Mesh>(MakeBox(1.5f, 1.6f, 1.5f));       // 小隊
    auto groundMesh = MakeShared<Mesh>(MakeBox(FIELD_W, 0.3f, FIELD_H)); // 地面
    auto wallMesh = MakeShared<Mesh>(MakeBox(CELL, 2.4f, CELL));        // 城牆塊
    auto bridgeMesh = MakeShared<Mesh>(MakeBox(CELL, 0.35f, CELL * 3)); // 橋面

    // ---- 戰場:中央城牆(x=10 格)留橋缺口(y=6~8 格)----
    SceneGraph scene;
    auto root = MakeShared<SceneNode>("root");
    scene.SetRootNode(root);

    AddStaticBox(root, groundMesh, Vector3(FIELD_W/2, -0.2f, FIELD_H/2),
                 Vector3(0.16f, 0.22f, 0.15f), "ground", 40.0f);
    for (int gy = 0; gy < 15; ++gy) {
        if (gy >= 6 && gy <= 8) continue; // 橋缺口
        AddStaticBox(root, wallMesh, Vector3(10 * CELL, 1.2f, gy * CELL + CELL * 0.5f),
                     Vector3(0.45f, 0.42f, 0.40f), "wall", 2.0f);
    }
    AddStaticBox(root, bridgeMesh, Vector3(10 * CELL, 0.05f, 7 * CELL),
                 Vector3(0.42f, 0.32f, 0.20f), "bridge", 3.0f);

    // ---- 戰鬥編成:藍軍(攻方,team0) vs 紅軍(守方,team1) ----
    BattleController battle(20, 15, CELL);
    battle.SetObjective(0, Vector2(17, 7));        // 攻方目標:牆右側
    battle.SetRallyPoint(0, Vector2(1, 7));
    battle.SetRallyPoint(1, Vector2(18, 7));
    battle.SetCommandPoints(0, 3);
    battle.SetCommandPoints(1, 3);

    // 攻方 4 隊從左翼推進
    const char* atkNames[] = {"前鋒", "左翼", "右翼", "預備隊"};
    Vector2 atkPos[] = {{3,4},{2,7},{3,10},{5,7}};
    for (int i = 0; i < 4; ++i) {
        Squad* s = battle.CreateSquad(atkNames[i], 0, atkPos[i], 12);
        DoctrineSet d;
        d.AddRule({DoctrineTrigger::EnemyInRange, DoctrineAction::AttackNearest, 3.5f, 10});
        d.AddRule({DoctrineTrigger::Always, DoctrineAction::AdvanceToObjective, 0.0f, 100});
        battle.AssignDoctrine(s, d);
    }
    // 守方 3 隊堵橋 + 1 隊右翼策應
    const char* defNames[] = {"橋頭守軍", "北段守軍", "南段守軍", "城內守軍"};
    Vector2 defPos[] = {{12,7},{11,3},{11,11},{14,7}};
    for (int i = 0; i < 4; ++i) {
        Squad* s = battle.CreateSquad(defNames[i], 1, defPos[i], 12);
        DoctrineSet d;
        d.AddRule({DoctrineTrigger::EnemyInRange, DoctrineAction::AttackNearest, 4.0f, 10});
        d.AddRule({DoctrineTrigger::AllyEngaged, DoctrineAction::DefendNearestAlly, 0.0f, 50});
        d.AddRule({DoctrineTrigger::Always, DoctrineAction::HoldPosition, 0.0f, 100});
        battle.AssignDoctrine(s, d);
    }

    battle.SetEventCallback([](const std::string& e){ std::cout << "  [戰報] " << e << std::endl; });

    // ---- Gameplay → 場景 ----
    BattleSceneSync sync;
    sync.Attach(battle, scene, CELL);
    sync.SetUnitMesh(boxMesh);
    sync.Sync(battle);

    battle.BeginExecution();
    battle.SetTimeScale(1.0f);

    // ---- 相機:高位斜俯視戰場 ----
    Camera cam;
    cam.SetPosition(Vector3(FIELD_W / 2, 42.0f, FIELD_H + 14.0f));
    cam.SetTarget(Vector3(FIELD_W / 2, 0.0f, FIELD_H / 2 - 3.0f));
    cam.SetPerspective(50.0f * 3.14159265f / 180.0f, (float)W / H, 0.1f, 300.0f);

    SceneRenderer sceneRenderer;
    sceneRenderer.SetDefaultShader(unitShader);

    // ---- ffmpeg(Media/VideoEncoder,高品質檔 crf16/slow)----
    Media::VideoEncoder enc;
    if (!enc.Open(outPath, W, H, fps, Media::PixelFormat::RGB,
                  "ffmpeg_battle.log", Media::EncodeQuality::High())) {
        std::cerr << "ffmpeg pipe failed\n";
        return 1;
    }

    std::vector<unsigned char> frameBuf(W * H * 3);
    std::cout << "Rendering " << totalFrames << " frames..." << std::endl;

    for (int f = 0; f < totalFrames; ++f) {
        renderer.PollEvents();
        battle.Update(1.0f / fps);
        sync.Sync(battle);

        renderer.SetClearColor(Vector3(0.07f, 0.09f, 0.13f));
        renderer.Clear();
        renderer.EnableDepthTest(true);
        renderer.EnableCulling(false); // 手排頂點繞序不保證 CCW,先關背面剔除

        RenderStats stats = sceneRenderer.Render(scene, cam);
        if (f == 0) {
            std::cout << "frame0: visible=" << stats.visibleNodes
                      << " renderable=" << stats.renderableNodes
                      << " draws=" << stats.drawCalls << std::endl;
        }

        // 擷取必須在 SwapBuffers 前:交換後 back buffer 內容未定義
        glReadPixels(0, 0, W, H, GL_RGB, GL_UNSIGNED_BYTE, frameBuf.data());
        if (!enc.WriteFrame(frameBuf.data(), frameBuf.size())) {
            std::cerr << "encoder pipe died at frame " << f << "\n";
            break;
        }
        renderer.SwapBuffers();

        if (battle.GetOutcome() != BattleOutcome::Ongoing) {
            std::cout << "戰鬥結束於 frame " << f << std::endl;
            // 結束後再播 1.5 秒定格
            for (int g = 0; g < fps * 3 / 2 && f + g < totalFrames; ++g) {
                renderer.SetClearColor(Vector3(0.07f, 0.09f, 0.13f));
                renderer.Clear();
                sceneRenderer.Render(scene, cam);
                glReadPixels(0, 0, W, H, GL_RGB, GL_UNSIGNED_BYTE, frameBuf.data());
                if (!enc.WriteFrame(frameBuf.data(), frameBuf.size())) {
                    break;
                }
                renderer.SwapBuffers();
            }
            break;
        }
        if (f % 60 == 0) std::cout << "  frame " << f << "/" << totalFrames << std::endl;
    }

    if (enc.Close() != 0) {
        std::cerr << "ffmpeg exited non-zero — output may be truncated\n";
        renderer.Shutdown();
        return 1;
    }
    renderer.Shutdown();
    std::cout << "Done: " << outPath << std::endl;
    return 0;
}
