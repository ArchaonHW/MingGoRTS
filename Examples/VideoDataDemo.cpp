// VideoDataDemo - 時序訓練資料產線：戰鬥 episode → 影格序列 + 逐幀標註
//
// 管線: BattleController(doctrine 執行) → BattleSceneSync → SceneRenderer →
//       Media/HeadlessCapture(隱藏窗口+FBO,top-down RGBA) → PNG 影格 +
//       frames.jsonl 逐幀標註 + Media/VideoEncoder(ffmpeg 可用時 ep.mp4)
//
// 與 BattleRenderDemo(單一展示影片) 的差異：本工具產的是資料集——
//   - 多 episode、seeded 編成抖動(人數/站位/編制)提供樣本多樣性
//   - 逐幀標註: 小隊螢幕 bbox、兵力、士氣、命令、潰逃/殲滅旗標、
//     最近事件——可供偵測/追蹤/行為辨識/戰果預測訓練
//   - dataset.json manifest + label schema
//
// 用法:
//   VideoDataDemo --out output/video_ds --episodes 2 --seconds 10
//                 --fps 12 --size 640x360 --seed 42 [--mp4]
// 無顯示環境:隱藏窗口失敗時回 0(工具性質,不進 POTATO_TESTS)

#include "Rendering/OpenGLRenderer.h"
#include "Rendering/SceneRenderer.h"
#include "Rendering/RenderableComponent.h"
#include "Rendering/Camera.h"
#include "Rendering/ImageCodec.h"
#include "Scene/SceneNode.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/BattleSceneSync.h"
#include "Gameplay/Doctrine.h"
#include "MathUtils/Matrix4.h"
#include "Media/HeadlessCapture.h"
#include "Media/VideoEncoder.h"
#include "Media/DatasetManifest.h"

#include <glad/glad.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <random>
#include <string>
#include <vector>

using namespace Potato;
using namespace Potato::Gameplay;

namespace fs = std::filesystem;

// ---- lambert shader(同 BattleRenderDemo 手排 mesh 對齊)----
static const char* UNIT_VERT = R"(
#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 vNormal;
out vec3 vWorld;
void main() {
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    vec4 wp = model * vec4(aPos, 1.0);
    vWorld = wp.xyz;
    gl_Position = projection * view * wp;
}
)";

static const char* UNIT_FRAG = R"(
#version 330 core
in vec3 vNormal;
in vec3 vWorld;
uniform vec3 uColor;
uniform vec3 uLightDir;
uniform vec3 uCamPos;
out vec4 FragColor;
void main() {
    vec3 n = normalize(vNormal);
    vec3 l = normalize(uLightDir);
    float diff = max(dot(n, l), 0.0);
    // 半球環境光:天空冷色/地表暖色,比平底 ambient 有立體感
    vec3 amb = mix(vec3(0.12,0.10,0.08), vec3(0.22,0.26,0.34),
                   n.y * 0.5 + 0.5);
    vec3 col = uColor * (amb + 0.85 * diff);
    // Blinn 高光:單位頂面受光給一點金屬感
    vec3 v = normalize(uCamPos - vWorld);
    vec3 h = normalize(l + v);
    col += vec3(0.25) * pow(max(dot(n, h), 0.0), 48.0) * diff;
    // 戰術格線:只畫在低矮表面(地面/橋面),格子=CELL 2m
    if (vWorld.y < 0.15) {
        vec2 g = abs(fract(vWorld.xz * 0.5) - 0.5);
        float line = smoothstep(0.46, 0.5, max(g.x, g.y));
        col = mix(col, col * 0.5, line * 0.7);
    }
    FragColor = vec4(col, 1.0);
}
)";

static Mesh MakeBox(float w, float h, float d) {
    Mesh m;
    float x = w * 0.5f, y = h * 0.5f, z = d * 0.5f;
    Vector3 n[6] = {{0,0,1},{0,0,-1},{1,0,0},{-1,0,0},{0,1,0},{0,-1,0}};
    Vector3 quad[6][4] = {
        {{-x,-y,z},{x,-y,z},{x,y,z},{-x,y,z}},
        {{x,-y,-z},{-x,-y,-z},{-x,y,-z},{x,y,-z}},
        {{x,-y,z},{x,-y,-z},{x,y,-z},{x,y,z}},
        {{-x,-y,-z},{x,-y,-z},{x,-y,z},{x,y,-z}},
        {{-x,y,z},{x,y,z},{x,y,-z},{x,y,-z}},
        {{-x,-y,z},{-x,-y,z},{-x,y,-z},{-x,y,z}},
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

// ---- CLI ----
struct Args {
    std::string out = "output/video_ds";
    int episodes = 2;
    float seconds = 10.0f;
    int fps = 12;
    int w = 640, h = 360;
    unsigned seed = 42;
    bool mp4 = true; // ffmpeg 不在 PATH 時自動退化為純 PNG 影格
    int ssaa = 1;    // 超採樣倍率(2 = 4x 像素再盒式降採,消鋸齒)
    int crf = 18;    // x264 品質
    std::string preset = "slow"; // x264 preset(白名單,非法退回 medium)
};
static bool ParseInt(const char* s, int& out) {
    if (!s || !*s) return false;
    char* end = nullptr;
    long v = std::strtol(s, &end, 10);
    if (end == s || *end) return false;
    out = (int)v;
    return true;
}
static bool ParseArgs(int argc, char** argv, Args& a) {
    for (int i = 1; i < argc; ++i) {
        const char* k = argv[i];
        auto need = [&](const char* n) -> const char* {
            if (i + 1 >= argc) {
                printf("參數 %s 缺值\n", n);
                return nullptr;
            }
            return argv[++i];
        };
        if (!std::strcmp(k, "--out")) {
            const char* v = need(k); if (!v) return false; a.out = v;
        } else if (!std::strcmp(k, "--episodes")) {
            const char* v = need(k); if (!v || !ParseInt(v, a.episodes)) return false;
        } else if (!std::strcmp(k, "--seconds")) {
            const char* v = need(k); if (!v) return false; a.seconds = (float)std::atof(v);
        } else if (!std::strcmp(k, "--fps")) {
            const char* v = need(k); if (!v || !ParseInt(v, a.fps)) return false;
        } else if (!std::strcmp(k, "--size")) {
            const char* v = need(k);
            if (!v) return false;
            const char* x = std::strchr(v, 'x');
            if (!x) return false;
            const std::string ws(v, x - v);
            if (!ParseInt(ws.c_str(), a.w) || !ParseInt(x + 1, a.h)) {
                return false;
            }
        } else if (!std::strcmp(k, "--seed")) {
            const char* v = need(k); if (!v) return false; a.seed = (unsigned)std::strtoul(v, nullptr, 10);
        } else if (!std::strcmp(k, "--no-mp4")) {
            a.mp4 = false;
        } else if (!std::strcmp(k, "--ssaa")) {
            const char* v = need(k);
            if (!v || !ParseInt(v, a.ssaa) || a.ssaa < 1 || a.ssaa > 4) {
                return false;
            }
        } else if (!std::strcmp(k, "--crf")) {
            const char* v = need(k);
            if (!v || !ParseInt(v, a.crf)) return false;
        } else if (!std::strcmp(k, "--preset")) {
            const char* v = need(k); if (!v) return false; a.preset = v;
        } else {
            printf("未知參數 %s\n", k);
            return false;
        }
    }
    return a.episodes > 0 && a.seconds > 0 && a.fps > 0 && a.w > 0 && a.h > 0;
}

// 世界座標 → 螢幕像素(背後/越界回 false)
static bool WorldToScreen(const Camera& cam, const Vector3& w,
                          int W, int H, float& sx, float& sy) {
    const Vector4 clip = cam.GetViewProjectionMatrix() *
                         Vector4(w.x, w.y, w.z, 1.0f);
    if (clip.w < 1e-5f) return false;
    sx = (clip.x / clip.w * 0.5f + 0.5f) * (float)W;
    sy = (0.5f - clip.y / clip.w * 0.5f) * (float)H;
    return true;
}

static const char* OrderName(SquadOrder o) {
    switch (o) {
    case SquadOrder::Hold:       return "hold";
    case SquadOrder::AttackMove: return "attack_move";
    case SquadOrder::Engage:     return "engage";
    case SquadOrder::Retreat:    return "retreat";
    default:                     return "none";
    }
}
// top-down 影格原地翻成 bottom-up——VideoEncoder 以 glReadPixels 語意
// (bottom-up + ffmpeg vflip) 為輸入;HeadlessCapture 出檔是 top-down。
static void FlipRows(std::vector<uint8_t>& img, int w, int h) {
    const size_t rb = (size_t)w * 4;
    std::vector<uint8_t> tmp(img.size());
    for (int y = 0; y < h; ++y) {
        std::memcpy(tmp.data() + (size_t)y * rb,
                    img.data() + (size_t)(h - 1 - y) * rb, rb);
    }
    img.swap(tmp);
}

// f×f 盒式降採(SSAA 後端);要求 sw/sh 可被 f 整除
static void Downsample(const std::vector<uint8_t>& src, int sw, int sh,
                       int f, std::vector<uint8_t>& dst) {
    const int dw = sw / f, dh = sh / f;
    dst.assign((size_t)dw * dh * 4, 0);
    for (int y = 0; y < dh; ++y) {
        for (int x = 0; x < dw; ++x) {
            for (int c = 0; c < 4; ++c) {
                int sum = 0;
                for (int j = 0; j < f; ++j) {
                    for (int i = 0; i < f; ++i) {
                        sum += src[((size_t)(y * f + j) * sw +
                                    (size_t)(x * f + i)) * 4 + c];
                    }
                }
                dst[((size_t)y * dw + (size_t)x) * 4 + c] =
                    (uint8_t)(sum / (f * f));
            }
        }
    }
}

static const char* OutcomeName(BattleOutcome o) {
    switch (o) {
    case BattleOutcome::Victory: return "victory";
    case BattleOutcome::Defeat:  return "defeat";
    case BattleOutcome::Draw:    return "draw";
    default:                     return "ongoing";
    }
}

int main(int argc, char** argv) {
    Args args;
    if (!ParseArgs(argc, argv, args)) {
        printf("用法: VideoDataDemo --out DIR [--episodes N] [--seconds S] "
               "[--fps F] [--size WxH] [--seed S] [--no-mp4] "
               "[--ssaa N] [--crf N] [--preset slow]\n");
        return 1;
    }
    const int W = args.w, H = args.h;
    const float CELL = 2.0f;
    const int GW = 20, GH = 15;
    const float FIELD_W = GW * CELL, FIELD_H = GH * CELL;
    const int totalFrames = (int)(args.seconds * args.fps);

    // ---- 離屏擷取(Media/HeadlessCapture:隱藏窗口+FBO+top-down 語意)----
    // SSAA:擷取解析度 = 輸出 × ssaa,Grab 後盒式降採回 W×H
    Media::HeadlessCapture cap;
    const int CW = W * args.ssaa, CH = H * args.ssaa;
    if (!cap.Begin(CW, CH)) {
        printf("無 GL 環境——隱藏窗口建立失敗,skip\n");
        return 0; // 工具性質:skip 不當失敗
    }
    glEnable(GL_DEPTH_TEST);

    auto unitShader = MakeShared<Shader>();
    if (!unitShader->LoadFromSource(UNIT_VERT, UNIT_FRAG)) {
        printf("shader 編譯失敗\n");
        return 1;
    }
    unitShader->Bind();
    unitShader->SetUniformVec3("uLightDir", Vector3(-0.4f, 1.0f, -0.35f));

    auto boxMesh   = MakeShared<Mesh>(MakeBox(1.5f, 1.6f, 1.5f));
    auto groundMesh= MakeShared<Mesh>(MakeBox(FIELD_W, 0.3f, FIELD_H));
    auto wallMesh  = MakeShared<Mesh>(MakeBox(CELL, 2.4f, CELL));
    auto bridgeMesh= MakeShared<Mesh>(MakeBox(CELL, 0.35f, CELL * 3));

    Camera cam;
    cam.SetPosition(Vector3(FIELD_W / 2, 42.0f, FIELD_H + 14.0f));
    cam.SetTarget(Vector3(FIELD_W / 2, 0.0f, FIELD_H / 2 - 3.0f));
    cam.SetPerspective(50.0f * 3.14159265f / 180.0f, (float)W / H,
                       0.1f, 300.0f);
    unitShader->SetUniformVec3("uCamPos", cam.GetPosition());

    std::error_code ec;
    fs::create_directories(args.out, ec);
    std::vector<std::string> epNames;
    std::vector<uint8_t> encBuf; // encoder 用 bottom-up 暫存(見 FlipRows)

    for (int ep = 0; ep < args.episodes; ++ep) {
        const unsigned epSeed = args.seed + (unsigned)ep * 7919u;
        std::mt19937 rng(epSeed);
        auto jitter = [&](float base, float range) {
            return base + (std::uniform_real_distribution<float>(
                               -range, range))(rng);
        };

        char epName[32];
        std::snprintf(epName, sizeof(epName), "ep_%04d", ep);
        const fs::path epDir = fs::path(args.out) / epName;
        fs::create_directories(epDir, ec);

        // ---- 場景(固定地形 + seeded 編成抖動)----
        SceneGraph scene;
        auto root = MakeShared<SceneNode>("root");
        scene.SetRootNode(root);
        AddStaticBox(root, groundMesh,
                     Vector3(FIELD_W / 2, -0.2f, FIELD_H / 2),
                     Vector3(0.16f, 0.22f, 0.15f), "ground", 40.0f);
        for (int gy = 0; gy < GH; ++gy) {
            if (gy >= 6 && gy <= 8) continue;
            AddStaticBox(root, wallMesh,
                         Vector3(10 * CELL, 1.2f, gy * CELL + CELL * 0.5f),
                         Vector3(0.45f, 0.42f, 0.40f), "wall", 2.0f);
        }
        AddStaticBox(root, bridgeMesh, Vector3(10 * CELL, 0.05f, 7 * CELL),
                     Vector3(0.42f, 0.32f, 0.20f), "bridge", 3.0f);

        BattleController battle(GW, GH, CELL);
        battle.SetObjective(0, Vector2(17, 7));
        battle.SetRallyPoint(0, Vector2(1, 7));
        battle.SetRallyPoint(1, Vector2(18, 7));
        battle.SetCommandPoints(0, 3);
        battle.SetCommandPoints(1, 3);

        // 攻方 4 隊:站位/人數隨 seed 抖動
        const char* atkNames[] = {"前鋒", "左翼", "右翼", "預備隊"};
        Vector2 atkPos[] = {{3,4},{2,7},{3,10},{5,7}};
        for (int i = 0; i < 4; ++i) {
            Vector2 p(jitter(atkPos[i].x, 1.5f), jitter(atkPos[i].y, 1.5f));
            p.x = std::max(0.5f, std::min(p.x, (float)GW - 0.5f));
            p.y = std::max(0.5f, std::min(p.y, (float)GH - 0.5f));
            Squad* s = battle.CreateSquad(atkNames[i], 0, p,
                                          10 + (int)(rng() % 6));
            DoctrineSet d;
            d.AddRule({DoctrineTrigger::EnemyInRange,
                       DoctrineAction::AttackNearest, 3.5f, 10});
            d.AddRule({DoctrineTrigger::Always,
                       DoctrineAction::AdvanceToObjective, 0.0f, 100});
            battle.AssignDoctrine(s, d);
        }
        const char* defNames[] = {"橋頭守軍", "北段守軍", "南段守軍", "城內守軍"};
        Vector2 defPos[] = {{12,7},{11,3},{11,11},{14,7}};
        for (int i = 0; i < 4; ++i) {
            Vector2 p(jitter(defPos[i].x, 1.0f), jitter(defPos[i].y, 1.5f));
            p.x = std::max(0.5f, std::min(p.x, (float)GW - 0.5f));
            p.y = std::max(0.5f, std::min(p.y, (float)GH - 0.5f));
            Squad* s = battle.CreateSquad(defNames[i], 1, p,
                                          10 + (int)(rng() % 6));
            DoctrineSet d;
            d.AddRule({DoctrineTrigger::EnemyInRange,
                       DoctrineAction::AttackNearest, 4.0f, 10});
            d.AddRule({DoctrineTrigger::AllyEngaged,
                       DoctrineAction::DefendNearestAlly, 0.0f, 50});
            d.AddRule({DoctrineTrigger::Always,
                       DoctrineAction::HoldPosition, 0.0f, 100});
            battle.AssignDoctrine(s, d);
        }

        std::string lastEvent;
        battle.SetEventCallback([&](const std::string& e) {
            lastEvent = e;
        });

        BattleSceneSync sync;
        sync.Attach(battle, scene, CELL);
        sync.SetUnitMesh(boxMesh);
        sync.Sync(battle);
        battle.BeginExecution();

        SceneRenderer sceneRenderer;
        sceneRenderer.SetDefaultShader(unitShader);

        // ---- 編碼器(Media/VideoEncoder;ffmpeg 缺席 → 純 PNG 影格)----
        Media::VideoEncoder enc;
        const fs::path mp4Path = epDir / (std::string(epName) + ".mp4");
        Media::EncodeQuality eq;
        eq.crf = args.crf;
        eq.preset = args.preset.c_str();
        const bool encoding =
            args.mp4 && enc.Open(mp4Path.string(), W, H, args.fps,
                                 Media::PixelFormat::RGBA,
                                 (epDir / "ffmpeg.log").string(), eq);
        if (args.mp4 && !encoding) {
            printf("  [ep%d] ffmpeg 不可用,僅產 PNG 影格\n", ep);
        }

        std::ofstream jsonl(epDir / "frames.jsonl");
        int framesWritten = 0;
        bool done = false;
        for (int f = 0; f < totalFrames && !done; ++f) {
            battle.Update(1.0f / args.fps);
            sync.Sync(battle);

            // Grab:綁 FBO+設 viewport+讀回+翻成 top-down(與 bbox 座標同向)
            const std::vector<uint8_t> hi = cap.Grab([&] {
                glClearColor(0.07f, 0.09f, 0.13f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                sceneRenderer.Render(scene, cam);
                return true;
            });
            if (hi.empty()) continue; // 該幀放棄:不寫影格也不寫標註
            std::vector<uint8_t> frame;
            if (args.ssaa > 1) {
                Downsample(hi, CW, CH, args.ssaa, frame);
            } else {
                frame = hi;
            }
            if (encoding) {
                encBuf = frame; // VideoEncoder 走 bottom-up(vflip),翻回
                FlipRows(encBuf, W, H);
                enc.WriteFrame(encBuf.data(), encBuf.size());
            }
            char imgName[32];
            std::snprintf(imgName, sizeof(imgName), "img_%05d.png", f);
            ImageCodec::WritePNGFile((epDir / imgName).string(), W, H,
                                     frame.data());

            // ---- 逐幀標註 ----
            jsonl << "{\"f\":" << f
                  << ",\"t\":" << (float)f / args.fps
                  << ",\"event\":";
            if (lastEvent.empty()) {
                jsonl << "null";
            } else {
                jsonl << "\"";
                for (char ch : lastEvent) {
                    if (ch == '"' || ch == '\\') jsonl << '\\';
                    jsonl << ch;
                }
                jsonl << "\"";
            }
            jsonl << ",\"squads\":[";
            bool first = true;
            for (const auto& sq : battle.GetSquads()) {
                const Vector2& gp = sq->GetPosition();
                const Vector3 world(gp.x * CELL, 0.8f * CELL,
                                    gp.y * CELL);
                float cx, cy;
                const bool onScreen =
                    WorldToScreen(cam, world, W, H, cx, cy);
                float bx0 = 0, by0 = 0, bx1 = 0, by1 = 0;
                bool clipped = false;
                if (onScreen) {
                    // 螢幕半徑:投影世界右向量偏移估像素尺度
                    float ox, oy;
                    WorldToScreen(cam,
                                  world + Vector3(0.8f * CELL, 0, 0),
                                  W, H, ox, oy);
                    const float hr = std::abs(ox - cx);
                    float ux, uy;
                    WorldToScreen(cam,
                                  world + Vector3(0, 1.0f * CELL, 0),
                                  W, H, ux, uy);
                    const float vr = std::abs(uy - cy);
                    bx0 = cx - hr; by0 = cy - vr;
                    bx1 = cx + hr; by1 = cy + vr;
                    clipped = bx0 < 0 || by0 < 0 || bx1 > W || by1 > H;
                    bx0 = std::max(0.0f, bx0);
                    by0 = std::max(0.0f, by0);
                    bx1 = std::min((float)W, bx1);
                    by1 = std::min((float)H, by1);
                }
                if (!first) jsonl << ",";
                first = false;
                char escName[64];
                int en = 0;
                for (char ch : sq->GetName()) {
                    if (ch == '"' || ch == '\\') escName[en++] = '\\';
                    if (en < (int)sizeof(escName) - 2) escName[en++] = ch;
                }
                escName[en] = 0;
                jsonl << "{\"name\":\"" << escName << "\""
                      << ",\"team\":" << sq->GetTeam()
                      << ",\"members\":" << sq->GetMembers()
                      << ",\"morale\":" << sq->GetMorale()
                      << ",\"order\":\"" << OrderName(sq->GetOrder()) << "\""
                      << ",\"routing\":" << (sq->IsRouting() ? 1 : 0)
                      << ",\"eliminated\":"
                      << (sq->IsEliminated() ? 1 : 0)
                      << ",\"visible\":" << (onScreen ? 1 : 0)
                      << ",\"clipped\":" << (clipped ? 1 : 0)
                      << ",\"bbox\":[" << bx0 << "," << by0 << ","
                      << bx1 << "," << by1 << "]"
                      << ",\"world\":[" << gp.x << "," << gp.y << "]}";
            }
            jsonl << "]}\n";
            ++framesWritten;
            lastEvent.clear(); // 事件只標在發生當幀,不殘留到後續影格

            if (battle.GetOutcome() != BattleOutcome::Ongoing) {
                // 結束後補 12 幀定格(事件收尾供時序模型學習)
                const int tail = std::min(12, totalFrames - f - 1);
                for (int g = 1; g <= tail; ++g) {
                    const std::vector<uint8_t> hi = cap.Grab([&] {
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                        sceneRenderer.Render(scene, cam);
                        return true;
                    });
                    if (hi.empty()) continue;
                    std::vector<uint8_t> tailPx;
                    if (args.ssaa > 1) {
                        Downsample(hi, CW, CH, args.ssaa, tailPx);
                    } else {
                        tailPx = hi;
                    }
                    if (encoding) {
                        encBuf = tailPx;
                        FlipRows(encBuf, W, H);
                        enc.WriteFrame(encBuf.data(), encBuf.size());
                    }
                    std::snprintf(imgName, sizeof(imgName),
                                  "img_%05d.png", f + g);
                    ImageCodec::WritePNGFile((epDir / imgName).string(),
                                             W, H, tailPx.data());
                    jsonl << "{\"f\":" << (f + g)
                          << ",\"t\":"
                          << (float)(f + g) / args.fps
                          << ",\"event\":null,\"squads\":[]}\n";
                    ++framesWritten;
                }
                done = true;
            }
        }
        if (enc.IsOpen()) enc.Close();
        jsonl.close();

        // episode meta
        std::ofstream meta(epDir / "meta.json");
        meta << "{\"episode\":" << ep << ",\"seed\":" << epSeed
             << ",\"outcome\":\"" << OutcomeName(battle.GetOutcome())
             << "\",\"frames\":" << framesWritten
             << ",\"fps\":" << args.fps << ",\"w\":" << W
             << ",\"h\":" << H
             << ",\"ssaa\":" << args.ssaa
             << ",\"crf\":" << args.crf
             << ",\"preset\":\"" << args.preset << "\""
             << ",\"mp4\":" << (encoding ? "true" : "false") << "}\n";

        epNames.push_back(epName);
        printf("[ep%d] seed=%u outcome=%s frames=%d\n", ep, epSeed,
               OutcomeName(battle.GetOutcome()), framesWritten);
    }

    // ---- dataset manifest(Media/DatasetManifest 統一 schema)----
    Media::DatasetManifest manifest;
    manifest.kind = "video"; // schema: potato.video_dataset/1
    manifest.seed = (int)args.seed;
    manifest.episodes = epNames;
    manifest.extrasJson =
        "{\"note\":\"逐幀標註:bbox=[x0,y0,x1,y1]像素,已 clamp 至畫面;"
        "clipped=部份出框;world=格座標;visible=在視錐內\"}";
    manifest.SaveToFile((fs::path(args.out) / "dataset.json").string());

    printf("Done: %d episodes → %s\n", (int)epNames.size(),
           args.out.c_str());
    return 0;
}
