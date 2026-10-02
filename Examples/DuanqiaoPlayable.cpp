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
#include "Rendering/ImageCodec.h"
#include "Scene/SceneNode.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/BattleSceneSync.h"
#include "Gameplay/BattlePicker.h"
#include "Gameplay/BattleMap.h"
#include "Gameplay/BattlePlanner.h"
#include "Gameplay/PlanningDeck.h"
#include "Gameplay/BattlePlan.h"
#include "Gameplay/EnemyGeneral.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/BattleRecorder.h"
#include "Gameplay/Roster.h"
#include "Gameplay/PostBattle.h"
#include "Campaign/CampaignState.h"
#include "Campaign/CampaignFlow.h"
#include "Gameplay/HistorianReport.h"
#include "Gameplay/GeneralDossier.h"
#include "Gameplay/RefitCamp.h"
#include "Gameplay/SquadTemplate.h"
#include "Gameplay/QuantumFog.h"
#include "MathUtils/CurlNoise.h"
#include "MathUtils/Matrix4.h"
#include "DemoAssets.h"
#include "UITheme.h"
#include "CharacterArt.h"

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

// 機率區域只在 demo 的透明 pass 繪製；高度高於橋板與水面。
static constexpr float FOG_PATCH_Y = 0.28f;
static const char* FOG_VERT = R"(
#version 330 core
layout(location=0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec2 vPatch;
out float vConfidence;
void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);
    worldPos.y = 0.28;
    vPatch = aPos.xz / 0.45;
    // BattleSceneSync 的既有縮放 s = 0.4 + 2p；不查詢敵軍真實位置。
    vConfidence = clamp((length(model[0].xyz) - 0.4) / 2.0, 0.0, 1.0);
    gl_Position = projection * view * worldPos;
}
)";
static const char* FOG_FRAG = R"(
#version 330 core
in vec2 vPatch;
in float vConfidence;
uniform vec3 uColor;
out vec4 FragColor;
void main() {
    float radius = length(vPatch);
    float softness = 1.0 - smoothstep(0.15, 1.0, radius);
    float alpha = softness * mix(0.20, 0.65, vConfidence);
    if (alpha < 0.003) discard;
    FragColor = vec4(uColor, alpha);
}
)";

// ---- 簡易 lambert shader(uniform 名對齊 SceneRenderer::SubmitRenderList)----
static const char* UNIT_VERT = R"(
#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 vNormal;
out vec3 vWorldPos;
void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    vWorldPos = worldPos.xyz;
    gl_Position = projection * view * worldPos;
}
)";

static const char* UNIT_FRAG = R"(
#version 330 core
in vec3 vNormal;
in vec3 vWorldPos;
uniform vec3 uColor;
uniform vec3 uLightDir;
uniform vec3 uCameraPos;
uniform int uSurfaceProfile;
uniform float uVisualTime;
uniform vec2 uRiverRange;
out vec4 FragColor;

float hash21(vec2 p) {
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x * p.y);
}

float valueNoise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    f = f * f * (3.0 - 2.0 * f);
    float a = hash21(i);
    float b = hash21(i + vec2(1.0, 0.0));
    float c = hash21(i + vec2(0.0, 1.0));
    float d = hash21(i + vec2(1.0, 1.0));
    return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

void main() {
    vec3 n = normalize(vNormal);
    float diff = max(dot(n, normalize(uLightDir)), 0.0);
    float light = 0.30 + 0.75 * diff;
    vec3 col = uColor * light;

    if (uSurfaceProfile == 1) { // Terrain: restrained soil/grass variation.
        float broad = valueNoise(vWorldPos.xz * 0.22);
        float fine = valueNoise(vWorldPos.xz * 1.7);
        float fineFootprint = max(fwidth(vWorldPos.x * 1.7), fwidth(vWorldPos.z * 1.7));
        float fineWeight = 1.0 - smoothstep(0.25, 0.9, fineFootprint);
        // 大面積草地色階與岸邊泥土，避免整塊地面只剩均勻綠色。
        vec3 grassTint = mix(vec3(0.75, 0.86, 0.68), vec3(1.28, 1.19, 1.02), broad);
        col *= grassTint * (1.0 + (fine - 0.5) * 0.16 * fineWeight);
        float bankDistance = min(abs(vWorldPos.z - uRiverRange.x), abs(vWorldPos.z - uRiverRange.y));
        float bank = (1.0 - smoothstep(0.08, 0.90, bankDistance + (fine - 0.5) * 0.25))
                   * smoothstep(0.6, 0.95, n.y);
        col = mix(col, vec3(0.25, 0.23, 0.16) * light, bank * 0.72);
        float roughness = mix(0.82, 0.96, fine);
        vec3 halfDir = normalize(normalize(uLightDir) + normalize(uCameraPos - vWorldPos));
        float specular = pow(max(dot(n, halfDir), 0.0), mix(28.0, 8.0, roughness));
        col += vec3(0.20, 0.24, 0.17) * specular * mix(0.05, 0.015, roughness);
    } else if (uSurfaceProfile == 2) { // Water: cool depth and subtle world-space ripples.
        // 沿河流動的細長波峰；不再相乘成大塊圓形光斑。
        float phaseA = vWorldPos.z * 8.0 + sin(vWorldPos.x * 0.55 - uVisualTime * 0.45) * 0.7;
        float phaseB = vWorldPos.z * 13.0 + sin(vWorldPos.x * 1.1 - uVisualTime * 0.3) * 0.7
                     + vWorldPos.x * 0.3 - uVisualTime * 0.9;
        float waveFootprint = max(fwidth(phaseA), fwidth(phaseB));
        float waveWeight = 1.0 - smoothstep(0.35, 1.1, waveFootprint);
        float ripple = 0.5 + 0.5 * sin(phaseA) * waveWeight;
        float topFace = smoothstep(0.45, 0.92, n.y);
        float bankDistance = min(abs(vWorldPos.z - uRiverRange.x), abs(vWorldPos.z - uRiverRange.y));
        float shallow = 1.0 - smoothstep(0.10, 0.80, bankDistance);
        col = mix(uColor * vec3(0.64, 0.86, 0.92), vec3(0.18, 0.36, 0.36), shallow * 0.5) * light;
        col *= 0.96 + 0.06 * ripple;
        float crest = smoothstep(0.86, 0.99, sin(phaseB)) * waveWeight * topFace;
        float crestSegments = 0.5 + 0.5 * sin(vWorldPos.x * 2.1 + sin(vWorldPos.z * 2.0) - uVisualTime * 0.4);
        crest *= smoothstep(0.25, 0.85, crestSegments);
        col += vec3(0.020, 0.044, 0.055) * crest;
        vec3 halfDir = normalize(normalize(uLightDir) + normalize(uCameraPos - vWorldPos));
        float glint = pow(max(dot(n, halfDir), 0.0), 36.0) * topFace;
        col += vec3(0.025, 0.055, 0.065) * glint;
    } else if (uSurfaceProfile == 3) { // Timber: subtle seams and long grain.
        float topFace = smoothstep(0.45, 0.92, n.y);
        vec2 plankCell = vWorldPos.xz * vec2(1.0, 2.8);
        vec2 plankFrac = fract(plankCell);
        vec2 plankEdge = min(plankFrac, 1.0 - plankFrac);
        float seamWidth = 0.025 + max(fwidth(plankCell.y), 0.001);
        float seam = 1.0 - smoothstep(0.025, seamWidth, plankEdge.y);
        float endCell = vWorldPos.x / 2.0 + floor(plankCell.y) * 0.5;
        float endEdge = min(fract(endCell), 1.0 - fract(endCell));
        float endSeam = 1.0 - smoothstep(0.008, 0.008 + max(fwidth(endCell), 0.008), endEdge);
        seam = max(seam, endSeam);
        float grainPhase = vWorldPos.x * 15.0 + valueNoise(vWorldPos.xz * 1.4) * 4.0;
        float grainWeight = 1.0 - smoothstep(0.35, 1.1, fwidth(grainPhase));
        float grain = 0.5 + 0.5 * sin(grainPhase) * grainWeight;
        float boardTone = hash21(vec2(floor(endCell), floor(plankCell.y)));
        col *= 0.93 + boardTone * 0.14 + (grain - 0.5) * 0.10 - seam * topFace * 0.25;
    } else if (uSurfaceProfile == 4) { // Stone: cool mineral variation.
        float mineral = valueNoise(vWorldPos.xz * 2.4 + vWorldPos.y * 0.7);
        col *= 0.94 + mineral * 0.12;
    }

    FragColor = vec4(col, 1.0);
}
)";

// ---- 程序化 mesh ----

// 士兵零件共用單一 mesh；texCoord.x 僅在專用 shader 表示材質類型。
static const char* SOLDIER_VERT = R"(
#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoord;
uniform mat4 model, view, projection;
out vec3 vNormal;
out vec3 vLocalPos;
flat out int vPart;
void main() {
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    vLocalPos = aPos;
    vPart = int(aTexCoord.x + 0.5);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";
static const char* SOLDIER_FRAG = R"(
#version 330 core
in vec3 vNormal;
in vec3 vLocalPos;
flat in int vPart;
uniform vec3 uColor;
out vec4 FragColor;
void main() {
    float moraleLight = clamp(max(max(uColor.r, uColor.g), uColor.b) / 0.9, 0.0, 1.0);
    vec3 col = vec3(0.40, 0.42, 0.32);
    if (vPart == 1) col = vec3(0.69, 0.49, 0.34);
    if (vPart == 2) col = vec3(0.075, 0.079, 0.071);
    if (vPart == 3) col = vec3(0.29, 0.19, 0.105);
    if (vPart == 4) col = vec3(0.32, 0.35, 0.28);
    if (vPart == 5) col = vec3(0.29, 0.29, 0.21);
    if (vPart == 6) col = uColor / max(max(max(uColor.r, uColor.g), uColor.b), 0.001) * 0.78;
    if (vPart == 7) col = vec3(0.13, 0.15, 0.15);
    if (vPart == 8) col = vec3(0.56, 0.53, 0.46);
    if (vPart == 9) col = vec3(0.36, 0.19, 0.14);
    if (vPart == 0 || vPart == 4 || vPart == 5) {
        float cloth = sin(vLocalPos.y * 31.0 + vLocalPos.x * 9.0) * sin(vLocalPos.z * 23.0);
        col *= 0.98 + cloth * 0.025;
    }
    vec3 n = normalize(vNormal);
    vec3 lightDir = normalize(vec3(-0.4, 1.0, 0.35));
    float light = 0.40 + 0.60 * max(dot(n, lightDir), 0.0);
    float sheen = pow(max(dot(n, normalize(lightDir + vec3(0.0, 0.55, 1.0))), 0.0), 24.0);
    float spec = vPart == 7 ? 0.055 : (vPart == 2 ? 0.014 : 0.004);
    FragColor = vec4((col * light + vec3(sheen * spec)) * moraleLight, 1.0);
}
)";

static Mesh MakeSoldier(float cell) {
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    vertices.reserve(3400);
    indices.reserve(12000);
    auto vertex = [&](Vector3 p, Vector3 n, int part) {
        Vertex v{};
        v.position = p * cell;
        v.normal = n.Normalized();
        v.texCoord = Vector2(static_cast<float>(part), 0.0f);
        vertices.push_back(v);
    };
    // 橢球梯度提供平滑法線；極點跳過退化三角形。
    auto oval = [&](Vector3 center, Vector3 radius, int part, int seg = 12, int rings = 7) {
        const uint32 base = static_cast<uint32>(vertices.size());
        for (int r = 0; r <= rings; ++r) {
            const float latitude = -1.5707963f + 3.1415927f * r / rings;
            const float c = std::cos(latitude), s = std::sin(latitude);
            for (int k = 0; k <= seg; ++k) {
                const float angle = 6.2831853f * k / seg;
                const Vector3 unit(c * std::cos(angle), s, c * std::sin(angle));
                vertex(center + unit * radius, {unit.x/radius.x, unit.y/radius.y, unit.z/radius.z}, part);
            }
        }
        for (int r = 0; r < rings; ++r) {
            for (int k = 0; k < seg; ++k) {
                const uint32 a = base + r * (seg + 1) + k, b = a + 1;
                const uint32 c = a + seg + 1, d = c + 1;
                if (r > 0) indices.insert(indices.end(), {a,c,b});
                if (r < rings - 1) indices.insert(indices.end(), {b,c,d});
            }
        }
    };
    // 圓錐台沿兩端連線延伸，法線含錐度；關節以重疊橢球柔化輪廓。
    auto tube = [&](Vector3 from, Vector3 to, float r0, float r1, int part, int seg = 10) {
        const Vector3 delta = to - from;
        const float length = delta.Length();
        const Vector3 axis = delta / length;
        const Vector3 reference = std::abs(axis.y) < 0.9f ? Vector3(0,1,0) : Vector3(1,0,0);
        const Vector3 u = axis.Cross(reference).Normalized(), v = axis.Cross(u);
        const uint32 base = static_cast<uint32>(vertices.size());
        for (int row = 0; row < 2; ++row) {
            for (int k = 0; k <= seg; ++k) {
                const float angle = 6.2831853f * k / seg;
                const Vector3 radial = u * std::cos(angle) + v * std::sin(angle);
                vertex((row == 0 ? from : to) + radial * (row == 0 ? r0 : r1),
                       radial - axis * ((r1-r0)/length), part);
            }
        }
        for (int k = 0; k < seg; ++k) {
            const uint32 a = base + k, b = a + 1, c = a + seg + 1, d = c + 1;
            indices.insert(indices.end(), {a,b,c,b,d,c});
        }
        // 端面用各自法線，避免槍口、衣領的法線被側面插值。
        for (int row = 0; row < 2; ++row) {
            const Vector3 center = row == 0 ? from : to;
            const Vector3 normal = row == 0 ? -axis : axis;
            const uint32 cap = static_cast<uint32>(vertices.size());
            vertex(center, normal, part);
            for (int k = 0; k <= seg; ++k) {
                const float angle = 6.2831853f * k / seg;
                vertex(center + (u*std::cos(angle)+v*std::sin(angle))*(row == 0 ? r0 : r1), normal, part);
            }
            for (int k = 0; k < seg; ++k) {
                if (row == 0) indices.insert(indices.end(), {cap,cap+k+2,cap+k+1});
                else indices.insert(indices.end(), {cap,cap+k+1,cap+k+2});
            }
        }
    };
    auto box = [&](Vector3 center, Vector3 size, int part) {
        const Vector3 h = size * 0.5f;
        const Vector3 corners[8] = {
            {-h.x,-h.y,-h.z}, {h.x,-h.y,-h.z}, {h.x,h.y,-h.z}, {-h.x,h.y,-h.z},
            {-h.x,-h.y,h.z}, {h.x,-h.y,h.z}, {h.x,h.y,h.z}, {-h.x,h.y,h.z}};
        const int faces[6][4] = {{4,5,6,7},{1,0,3,2},{5,1,2,6},{0,4,7,3},{7,6,2,3},{0,1,5,4}};
        const Vector3 normals[6] = {{0,0,1},{0,0,-1},{1,0,0},{-1,0,0},{0,1,0},{0,-1,0}};
        for (int f = 0; f < 6; ++f) {
            const uint32 base = static_cast<uint32>(vertices.size());
            for (int k = 0; k < 4; ++k) {
                Vertex v{};
                v.position = (center + corners[faces[f][k]]) * cell;
                v.normal = normals[f];
                v.texCoord = Vector2(static_cast<float>(part), 0.0f);
                vertices.push_back(v);
            }
            indices.insert(indices.end(), {base,base+1,base+2,base,base+2,base+3});
        }
    };
    // +Z 為正面；成人頭身比例、腳底 y=0，所有零件合併成共用 mesh。
    for (float side : {-1.0f, 1.0f}) {
        const Vector3 hip(side*0.105f,0.80f,0), knee(side*0.12f,0.47f,0.025f);
        const Vector3 ankle(side*0.135f,0.14f,-0.015f);
        tube(hip,knee,0.098f,0.073f,4);
        oval(knee,{0.077f,0.081f,0.080f},4);
        tube(knee,ankle,0.074f,0.053f,4);
        oval({side*0.135f,0.075f,0.055f},{0.076f,0.075f,0.145f},2);
        tube(ankle,{side*0.135f,0.25f,-0.005f},0.059f,0.064f,2);
    }
    oval({0,0.805f,0},{0.205f,0.13f,0.125f},4);
    // 多圈橢圓制服：腰身窄、胸肩寬，避免方塊或球形軀幹。
    const float torsoY[] = {0.79f,0.88f,1.04f,1.18f,1.235f};
    const float torsoX[] = {0.174f,0.166f,0.205f,0.231f,0.165f};
    const float torsoZ[] = {0.118f,0.113f,0.137f,0.124f,0.098f};
    const uint32 torsoBase = static_cast<uint32>(vertices.size());
    constexpr int torsoSeg = 16;
    for (int r = 0; r < 5; ++r) {
        const int lo = r == 0 ? 0 : r-1, hi = r == 4 ? 4 : r+1;
        const float sx = (torsoX[hi]-torsoX[lo])/(torsoY[hi]-torsoY[lo]);
        const float sz = (torsoZ[hi]-torsoZ[lo])/(torsoY[hi]-torsoY[lo]);
        for (int k = 0; k <= torsoSeg; ++k) {
            const float a = 6.2831853f*k/torsoSeg, c = std::cos(a), s = std::sin(a);
            vertex({torsoX[r]*c,torsoY[r],torsoZ[r]*s},
                   {c/torsoX[r],-sx*c*c/torsoX[r]-sz*s*s/torsoZ[r],s/torsoZ[r]},0);
        }
    }
    for (int r = 0; r < 4; ++r) for (int k = 0; k < torsoSeg; ++k) {
        const uint32 a = torsoBase+r*(torsoSeg+1)+k, b = a+1, c = a+torsoSeg+1, d = c+1;
        indices.insert(indices.end(),{a,c,b,b,c,d});
    }
    oval({0,1.224f,0},{0.165f,0.028f,0.098f},0,12,6);
    tube({0,1.21f,0},{0,1.345f,0},0.060f,0.063f,1);
    oval({0,1.442f,0.012f},{0.103f,0.127f,0.096f},1,16,10);
    oval({0,1.425f,0.111f},{0.026f,0.039f,0.028f},1,8,6);
    for (float side : {-1.0f,1.0f}) {
        oval({side*0.042f,1.467f,0.100f},{0.019f,0.009f,0.008f},8,8,6);
        oval({side*0.042f,1.467f,0.107f},{0.005f,0.007f,0.003f},2,8,6);
        oval({side*0.042f,1.485f,0.096f},{0.024f,0.004f,0.005f},3,8,6);
    }
    oval({0,1.393f,0.102f},{0.023f,0.004f,0.006f},9,8,6);
    for (float side : {-1.0f,1.0f}) oval({side*0.103f,1.443f,0.002f},{0.020f,0.035f,0.022f},1,8,6);
    oval({0,1.567f,0},{0.121f,0.078f,0.107f},4);
    oval({0,1.532f,0.106f},{0.125f,0.015f,0.093f},4);
    oval({0,1.555f,0.104f},{0.027f,0.024f,0.008f},6,8,6);
    // 雙臂於胸前彎曲，左右手實際落在槍托握把與前護木。
    const Vector3 shoulders[] = {{-0.231f,1.163f,0},{0.231f,1.163f,0}};
    const Vector3 elbows[] = {{-0.273f,0.963f,0.144f},{0.278f,0.992f,0.137f}};
    const Vector3 hands[] = {{-0.166f,1.067f,0.265f},{0.193f,1.121f,0.267f}};
    for (int i = 0; i < 2; ++i) {
        oval(shoulders[i],{0.078f,0.092f,0.085f},0);
        tube(shoulders[i],elbows[i],0.077f,0.061f,0);
        oval(elbows[i],{0.064f,0.064f,0.067f},0);
        tube(elbows[i],hands[i],0.061f,0.044f,0);
        oval(hands[i],{0.054f,0.046f,0.054f},1);
        oval({shoulders[i].x*1.15f,1.17f,0.014f},{0.016f,0.041f,0.060f},6,8,6);
    }
    // 圓角背包、織帶、皮帶與小型彈藥袋；箱形僅用於裝備。
    oval({0,1.013f,-0.205f},{0.168f,0.204f,0.089f},5);
    for (float side : {-1.0f,1.0f}) {
        tube({side*0.12f,1.22f,0.094f},{side*0.104f,0.868f,0.120f},0.017f,0.017f,5,6);
        box({side*0.126f,0.87f,0.15f},{0.078f,0.107f,0.055f},5);
        box({side*0.118f,0.929f,0.151f},{0.086f,0.019f,0.063f},5);
    }
    oval({0,0.847f,0},{0.19f,0.032f,0.126f},3);
    box({0,0.847f,0.13f},{0.042f,0.040f,0.014f},7);
    box({0.073f,1.146f,0.130f},{0.061f,0.038f,0.012f},6);
    // 槍身斜橫於胸前，低多邊形圓管與木製槍托均具真實軸向。
    const Vector3 stock(-0.325f,1.03f,0.264f), receiver(-0.092f,1.078f,0.264f);
    const Vector3 foreEnd(0.272f,1.137f,0.264f), muzzle(0.48f,1.171f,0.264f);
    tube(stock,receiver,0.045f,0.031f,3,10);
    oval(stock,{0.040f,0.058f,0.032f},3,8,6);
    tube(receiver,foreEnd,0.027f,0.022f,3,10);
    tube(receiver,muzzle,0.016f,0.011f,7,10);
    tube({-0.104f,1.062f,0.267f},{-0.145f,1.010f,0.267f},0.025f,0.023f,3,8);
    box({-0.016f,1.039f,0.266f},{0.052f,0.068f,0.035f},7);
    box({0.391f,1.173f,0.264f},{0.017f,0.037f,0.016f},7);
    Mesh mesh;
    mesh.SetVertices(vertices);
    mesh.SetIndices(indices);
    return mesh;
}

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

// 平面直徑 0.9，既有候選機率縮放仍由 BattleSceneSync 提供。
static Mesh MakeFogPatch() {
    Mesh m;
    std::vector<Vertex> verts(4);
    const Vector3 positions[] = {
        Vector3(-0.45f, 0, -0.45f), Vector3(0.45f, 0, -0.45f),
        Vector3(0.45f, 0, 0.45f), Vector3(-0.45f, 0, 0.45f)
    };
    for (size_t i = 0; i < verts.size(); ++i) {
        verts[i].position = positions[i];
        verts[i].normal = Vector3(0, 1, 0);
        verts[i].texCoord = Vector2(0, 0);
    }
    m.SetVertices(verts);
    m.SetIndices({0, 2, 1, 0, 3, 2});
    return m;
}

// 膠囊體:車削(profile = 下半球 + 圓柱 + 上半球)
static Mesh MakeCapsule(float radius, float height, int seg = 12, int rings = 6,
                        bool centered = false) {
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
                                 profile[r].y + (centered ? 0.0f : height * 0.5f),
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
                                       const char* name, float boundR,
                                       SurfaceProfile surfaceProfile = SurfaceProfile::Neutral) {
    auto node = MakeShared<SceneNode>(name);
    node->SetLocalPosition(pos);
    auto rc = MakeShared<RenderableComponent>();
    rc->mesh = mesh;
    rc->color = color;
    rc->surfaceProfile = surfaceProfile;
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
    if (!BattlePicker::IntersectGround(ray, FOG_PATCH_Y, g)) return -1;
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
enum class ShellScreen { Title, Story, Aftermath, Complete, Battle, Quit };
static bool CaptureFrame(const char* path, int width, int height);

#include "CampaignShell.h"

// 直接擷取本程式的 framebuffer；僅供 --visual-check 驗證使用。
static bool CaptureFrame(const char* path, int width, int height) {
    if (width <= 0 || height <= 0) return false;
    std::vector<uint8> pixels(static_cast<size_t>(width) * height * 4);
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    const size_t rowBytes = static_cast<size_t>(width) * 4;
    for (int y = 0; y < height / 2; ++y) {
        auto top = pixels.begin() + static_cast<size_t>(y) * rowBytes;
        auto bottom = pixels.begin() + static_cast<size_t>(height - 1 - y) * rowBytes;
        std::swap_ranges(top, top + rowBytes, bottom);
    }
    std::string error;
    if (!ImageCodec::WritePNGFile(path, width, height, pixels.data(), &error)) {
        std::fprintf(stderr, "[visual-check] capture failed: %s\n", error.c_str());
        return false;
    }
    return true;
}

int main(int argc, char** argv) {
    bool visualCheck = false;
    bool campaignCheck = false, combatCheck = false, naturalCheck = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--visual-check") visualCheck = true;
        if (std::string(argv[i]) == "--campaign-natural-check") { campaignCheck = true; naturalCheck = true; }
        if (std::string(argv[i]) == "--campaign-check") campaignCheck = true;
        if (std::string(argv[i]) == "--campaign-combat-check") { campaignCheck = true; combatCheck = true; }
    }
    bool visualCheckPassed = !visualCheck;
    const int W = 1440, H = 810;

    OpenGLRenderer renderer;
    RendererConfig rcfg;
    rcfg.window.width = W;
    rcfg.window.height = H;
    rcfg.window.title = "MingGoRTS - 七章戰役史卷";
    rcfg.window.vsync = true;
    renderer.SetConfig(rcfg);
    if (!renderer.Initialize()) {
        std::fprintf(stderr, "[SKIP] no display / renderer init failed\n");
        return visualCheck ? 1 : 0;
    }
    GLFWwindow* window = static_cast<GLFWwindow*>(renderer.GetWindowHandle());
    if (visualCheck) {
        GLint samples = 0;
        glGetIntegerv(GL_SAMPLES, &samples);
        std::printf("[visual-check] MSAA=%d GPU=%s\n", samples,
                    reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        std::fflush(stdout);
    }
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
    io.FontDefault = fontSans ? fontSans : io.Fonts->Fonts[0];

    // U-1 殼層狀態:主題/UI 縮放在標題頁設定頁修改
    UITheme::Id theme = UITheme::Id::TacticalSim;

    float uiScale = 1.0f;
    UITheme::Apply(ImGui::GetStyle(), theme);

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
    auto soldierShader = MakeShared<Shader>();
    if (!soldierShader->LoadFromSource(SOLDIER_VERT, SOLDIER_FRAG)) {
        std::fprintf(stderr, "Soldier shader compile failed\n");
        shutdownAll();
        return 1;
    }
    auto fogShader = MakeShared<Shader>();
    if (!fogShader->LoadFromSource(FOG_VERT, FOG_FRAG)) {
        std::fprintf(stderr, "Fog shader compile failed\n");
        shutdownAll();
        return 1;
    }

    // U-1 殼層外迴圈:Title ↔ Battle;整場戰鬥在內層 scope,
    // 出 scope 所有 stack 物件析構 = 乾淨重置(回主選單可再戰)
    // G-9 整補營:常備軍+戰利品帳持有在殼層外,跨場持續
    // E-7 戰役框架:整補營改由 CampaignState 持有（章節邊界聚合存檔）
    // N-2 敵將檔案:聽聞/驗證狀態也跨場——驗過的將不重聽傳聞
    Campaign::CampaignState campaign;
    RefitCamp& camp = campaign.Camp();
    GeneralDossier dossier;
    SquadTemplateLibrary campLibrary;
    campLibrary.LoadDir(DemoAssets::Resolve("squads"));
    campLibrary.LoadDir(DemoAssets::Resolve("templates"));
    auto portraits = MakeShared<Texture>();
    std::string portraitError;
    if (!CharacterArt::LoadAtlas(*portraits, DemoAssets::Resolve("portraits/commanders-v1.png"), portraitError)) {
        std::fprintf(stderr, "Character portrait atlas could not be loaded: %s\n", portraitError.c_str());
        if (visualCheck) { shutdownAll(); return 1; }
        portraits.reset();
    }
    Campaign::ChapterLibrary chapters;
    std::string campaignError;
    if (!chapters.LoadFromFile(DemoAssets::Resolve("campaign/chapters.json"), campaignError)) {
        std::fprintf(stderr, "%s\n", campaignError.c_str()); shutdownAll(); return 1;
    }
    Campaign::CampaignFlow flow(chapters);
    const bool verification = visualCheck || campaignCheck;
    auto saveDir = std::filesystem::path(DemoAssets::Root()).parent_path() / (verification ? "verification-saves" : "saves");
    std::error_code saveError;
    std::filesystem::create_directories(saveDir, saveError);
    if (saveError) { std::fprintf(stderr, "Cannot create save directory\n"); shutdownAll(); return 1; }
    const std::string savePath = (saveDir / (combatCheck ? "campaign-combat.json" : "campaign.json")).string();
    if (verification) {
        if (!CommitCampaign(campaign, flow, savePath, campaignError,
                [&](auto& s, auto& e) { return flow.StartNew(s, e); })) { shutdownAll(); return 2; }
        if (visualCheck && !CommitCampaign(campaign, flow, savePath, campaignError,
                [&](auto& s, auto& e) { return flow.Choose(s, "mercy", e); })) { shutdownAll(); return 2; }
    } else if (std::filesystem::exists(savePath)) {
        Campaign::CampaignState loaded;
        if (loaded.LoadFromFile(savePath) && flow.Validate(loaded, campaignError)) campaign = std::move(loaded);
        else campaignError = "現有存檔無法驗證，未覆寫。可先備份檔案，再建立新戰役。";
    }
    if (visualCheck) CampaignFrame(window, renderer, ShellScreen::Title, campaign, chapters, flow,
        campLibrary, savePath, campaignError, portraits.get(), fontSans, fontSerif, theme, uiScale, "presentation-title.png");
    ShellScreen screen = visualCheck ? ShellScreen::Battle : campaignCheck ? ShellScreen::Story : ShellScreen::Title;
    int shellFrames = 0;
    const double campaignStarted = glfwGetTime();
    while (screen != ShellScreen::Quit && !renderer.ShouldClose()) {
        if (campaignCheck && glfwGetTime() - campaignStarted > (naturalCheck ? 600 : 180)) { campaignError = "Campaign verification timeout"; break; }
        if (screen != ShellScreen::Battle) {
            const auto previous = screen;
            if (campaignCheck && campaign.chapter.chapter == 4) glfwSetWindowSize(window, 960, 640);
            if (campaignCheck && campaign.chapter.chapter == 7) glfwSetWindowSize(window, 1440, 810);
            std::string screenshot;
            if (campaignCheck && shellFrames == 2) screenshot = "campaign-" + std::to_string(campaign.chapter.chapter) + "-" +
                (screen == ShellScreen::Story ? "briefing" : screen == ShellScreen::Aftermath ? "aftermath" : "complete") + (combatCheck ? "-combat" : "") + ".png";
            screen = CampaignFrame(window, renderer, screen, campaign, chapters, flow, campLibrary,
                savePath, campaignError, portraits.get(), fontSans, fontSerif, theme, uiScale, screenshot.empty() ? nullptr : screenshot.c_str());
            ++shellFrames;
            if (campaignCheck && shellFrames >= 5) {
                if (screen == ShellScreen::Story) {
                    const auto* c = flow.Current(campaign);
                    if (naturalCheck && !CommitCampaign(campaign, flow, savePath, campaignError, [](auto& s, auto&) { s.Camp().HealWounded(s.Camp().GetLoot()); return true; })) break;
                    if (!flow.SelectedChoice(campaign)) {
                        std::string choice = c->choices.front().id;
                        if (c->number == 1) choice = combatCheck ? "pursue" : "mercy";
                        if (c->number == 2) choice = combatCheck ? "pursue" : "rescue";
                        if (c->number == 3) choice = combatCheck ? "attack" : "negotiate";
                        if (!CommitCampaign(campaign, flow, savePath, campaignError,
                            [&](auto& s, auto& e) { return flow.Choose(s, choice, e); })) break;
                        shellFrames = 0;
                    } else if (flow.SelectedChoice(campaign)->id == "negotiate") {
                        if (!CommitCampaign(campaign, flow, savePath, campaignError,
                            [&](auto& s, auto& e) { return flow.CompletePeace(s, e); })) break;
                        screen = ShellScreen::Aftermath;
                    } else screen = ShellScreen::Battle;
                } else if (screen == ShellScreen::Aftermath) {
                    Campaign::CampaignState restored;
                    if (!restored.LoadFromFile(savePath) || !flow.Validate(restored, campaignError)) break;
                    campaign = std::move(restored);
                    if (!CommitCampaign(campaign, flow, savePath, campaignError,
                        [&](auto& s, auto& e) { return flow.Advance(s, e); })) break;
                    screen = ResumeCampaign(campaign);
                    std::printf("[campaign-check] reload/advance: chapter=%d completed=%zu dead=%d\n", campaign.chapter.chapter, campaign.progress.completed.size(), campaign.progress.cumulativeDead);
                } else if (screen == ShellScreen::Complete) {
                    std::printf("[campaign-check] PASS: seven chapters; route=%s (%s battle outcomes)\n", combatCheck ? "combat" : "peace", naturalCheck ? "natural" : "scripted");
                    break;
                }
            }
            if (screen != previous) shellFrames = 0;
            continue;
        }
        const auto* chapter = flow.Current(campaign);
        const auto* chapterChoice = flow.SelectedChoice(campaign);
        if (!chapter || !chapterChoice || campaign.progress.stage != Campaign::CampaignStage::Briefing) { screen = ResumeCampaign(campaign); continue; }
        bool settlementAttempted = false, settlementSaved = false;
        auto settleBattle = [&](const BattleController& b, const Roster& r) {
            settlementAttempted = true;
            settlementSaved = CommitCampaign(campaign, flow, savePath, campaignError,
                [&](auto& s, auto& e) { return flow.CompleteBattle(s, b, r, e); });
        };
        bool backToTitle = false;
        { // ---- 戰鬥場次 scope 開始(內部維持原縮排以保 diff 最小) ----
        // 戰後結算狀態:每場重建——宣告成 static 會跨場殘留上一場的報告
        std::string chronicler;
        float endedAt = -1.0f;
        float replayCursor = 0.0f;

    // ---- 地圖 ----
    BattleMap map;
    if (!map.LoadFromFile(DemoAssets::Resolve(chapter->mapPath.substr(7)).c_str())) {
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
    float riverMin = FH + 1.0f, riverMax = -1.0f;
    for (int y = 0; y < GH; ++y) {
        for (int x = 0; x < GW; ++x) {
            if (!battle.GetField().IsBlocked(x, y)) continue;
            riverMin = std::min(riverMin, y * CELL);
            riverMax = std::max(riverMax, (y + 1) * CELL);
        }
    }
    unitShader->Bind();
    unitShader->SetUniformVec2("uRiverRange", riverMax >= riverMin
        ? Vector2(riverMin, riverMax) : Vector2(-10000.0f, -10000.0f));
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
    auto pinMesh    = MakeShared<Mesh>(MakeBox(0.25f, 1.4f, 0.25f));
    auto propMesh   = MakeShared<Mesh>(MakeBox(0.7f, 0.7f, 0.7f));
    auto capMesh    = MakeShared<Mesh>(MakeSoldier(CELL));
    auto ringMesh   = MakeShared<Mesh>(MakeRing(0.55f * CELL, 0.75f * CELL));
    // 血條直立(XY 面朝相機):平躺版在 63° 俯角下只剩 ~2px 不可讀
    auto barBgMesh  = MakeShared<Mesh>(MakeBox(1.0f, 0.16f, 0.03f));
    auto barFillMesh = MakeShared<Mesh>(MakeBox(0.96f, 0.11f, 0.04f));
    // 候選位置與機率縮放照舊，平面徑向透明度表達可能所在區域。
    auto cloudMesh  = MakeShared<Mesh>(MakeFogPatch());

    auto groundNode = AddStaticBox(root, groundMesh,
                                   Vector3(FW / 2, -0.15f, FH / 2),
                                   Vector3(chapter->tint[0], chapter->tint[1], chapter->tint[2]), "ground", FW,
                                   SurfaceProfile::Terrain);
    groundNode->SetLocalScale(Vector3(FW, 1, FH));

    // 河面:IsBlocked 的格子鋪水磚(渡口自動留空)
    for (int y = 0; y < GH; ++y) {
        for (int x = 0; x < GW; ++x) {
            if (battle.GetField().IsBlocked(x, y)) {
                AddStaticBox(root, waterMesh,
                             Vector3((x + 0.5f) * CELL, -0.05f, (y + 0.5f) * CELL),
                             Vector3(0.15f, 0.30f, 0.45f), "water", CELL,
                             SurfaceProfile::Water);
            }
        }
    }
    // 渡口鋪橋板
    for (const auto& ford : map.GetFords()) {
        const float width = ford.rect.w * CELL, length = ford.rect.h * CELL;
        const float cx = (ford.rect.x + ford.rect.w * 0.5f) * CELL;
        const float cz = (ford.rect.y + ford.rect.h * 0.5f) * CELL;
        auto deckMesh = MakeShared<Mesh>(MakeBox(width, 0.3f, length));
        const float radius = std::sqrt(width * width + length * length);
        AddStaticBox(root, deckMesh, Vector3(cx, 0.02f, cz),
                     Vector3(0.42f, 0.32f, 0.20f), "ford", radius,
                     SurfaceProfile::Timber);
        auto beamMesh = MakeShared<Mesh>(MakeBox(0.10f, 0.12f, length));
        for (float side : {-1.0f, 1.0f}) {
            AddStaticBox(root, beamMesh, Vector3(cx + side * (width * 0.5f - 0.05f), 0.19f, cz),
                         Vector3(0.30f, 0.22f, 0.13f), "ford_beam", radius,
                         SurfaceProfile::Timber);
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
                     c, "prop", CELL,
                     it.type == "oil_slick" ? SurfaceProfile::Neutral
                                            : SurfaceProfile::Stone);
    }

    // ---- 編成(同 DuanqiaoDemo):我 4 隊北岸 / 敵格洛克 4 隊南岸 ----
    const MapPin* northRally = map.FindPin("北岸集結點");
    const MapPin* southCamp  = map.FindPin("南岸敵營");
    Squad* rearGuard = nullptr;
    Squad* generalGuard = nullptr;
    // Original identities and current manpower come from the persistent camp.
    auto deployPositions = chapter->friendlyDeployment;
    // Extra recruits use unblocked cells within the friendly deployment zone.
    for (int y = 1; deployPositions.size() < camp.GetUnits().size() && y < GH; ++y)
        for (int x = 2; deployPositions.size() < camp.GetUnits().size() && x < GW - 2; x += 2) {
            Vector2 p{float(x), float(y)};
            if (map.IsInDeployZone(0, p) && !battle.GetField().IsBlocked(x, y) &&
                std::none_of(deployPositions.begin(), deployPositions.end(), [&](const auto& v) { return (v - p).Length() < 1; })) deployPositions.push_back(p);
        }
    auto allies = camp.Deploy(battle, 0, deployPositions, &campLibrary);
    for (auto* squad : allies) {
        if (squad->GetName() == "後衛") rearGuard = squad;
        if (squad->GetName() == "將軍衛隊") { squad->SetGeneralGuard(true); generalGuard = squad; }
        const auto* veteran = camp.FindUnit(squad->GetName());
        roster.Enroll(squad, veteran->captainName, "captain", veteran->relics.empty() ? "" : veteran->relics.front());
    }
    if (!rearGuard && !allies.empty()) rearGuard = allies.back();
    if (chapterChoice->holdSeconds > 0 && !battle.SetProtectionObjective(rearGuard, chapterChoice->holdSeconds)) {
        std::fprintf(stderr, "Cannot configure protection mission\n"); shutdownAll(); return 2;
    }
    std::vector<Squad*> enemies;
    for (const auto& deployment : chapter->enemies) {
        auto* squad = battle.CreateSquad(deployment.name, 1, deployment.position, deployment.members);
        squad->SetSpeed(chapter->enemySpeed);
        squad->SetEngageRange(chapter->enemyRange);
        squad->SetDamagePerMember(chapter->enemyDamage);
        roster.Enroll(squad, enemies.empty() ? chapter->enemyGeneralName : chapter->title + " · " + deployment.name, "captain");
        enemies.push_back(squad);
    }
    Squad* e0 = enemies[0]; Squad* e1 = enemies[1]; Squad* e2 = enemies[2];
    if (southCamp)  battle.SetObjective(0, southCamp->pos);
    if (northRally) battle.SetObjective(1, northRally->pos);
    if (northRally) battle.SetRallyPoint(0, northRally->pos);
    if (southCamp)  battle.SetRallyPoint(1, southCamp->pos);

    JsonValue card; card.type = JsonValue::Type::Object;
    card.objectValue["schema"] = JsonValue::String("potato.character_card/1");
    card.objectValue["name"] = JsonValue::String(chapter->enemyGeneralName);
    card.objectValue["epithet"] = JsonValue::String(chapter->title);
    JsonValue personality; personality.type = JsonValue::Type::Object;
    personality.objectValue["aggression"] = JsonValue::Number(chapter->enemyPersonality == "侵略" ? 90 : 40);
    personality.objectValue["discipline"] = JsonValue::Number(chapter->enemyPersonality == "審慎" ? 80 : 40);
    personality.objectValue["cunning"] = JsonValue::Number(chapter->enemyPersonality == "狡詐" ? 85 : 20);
    card.objectValue["personality"] = personality;
    EnemyGeneral glock;
    if (!glock.LoadFromString(JsonSerialization::WriteJson(card))) { shutdownAll(); return 1; }
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
    if (e2) e2->SetUnitClass(UnitClass::Cavalry);

    // ---- Q-1/Q-3 敵情霧:敵軍以疊加態存在,觀測或接觸才塌縮 ----
    // 觀測 2 情報全額買斷;探測 1 情報讓雲向真值收縮(弱觀測)
    QuantumFog fog(/*intelDuration=*/25.0f, /*observe=*/2, /*probe=*/1);
    fog.BindResources(&res);
    battle.BindFog(&fog);
    for (Squad* es : enemies) {
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
                               chapterChoice->holdSeconds > 0 ? DoctrineAction::HoldPosition : DoctrineAction::Scout, 0.0f, 90});
        }
    }

    // ---- Gameplay → 場景 ----
    BattleSceneSync sync;
    sync.Attach(battle, scene, CELL);
    sync.SetUnitMesh(capMesh);
    sync.SetMovementFacing(true);
    sync.SetOverlayMeshes(ringMesh, barBgMesh, barFillMesh, 2.0f * CELL, 1.2f);
    sync.SetFog(&fog);
    sync.SetFogMarkerMesh(cloudMesh);
    // P-1 湍流漂移:機率雲標記疊加無散度微擾,不確定性「活」起來;
    // strength=0 可關閉(預設行為不變)
    Quasi::TurbulenceField fogTurb(/*octaves=*/6, /*seed=*/42,
                                  /*baseFreq=*/0.15f, /*baseAmp=*/1.0f);
    sync.SetFogDrift(&fogTurb, 0.4f * CELL);
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
    Vector3 desiredCamTarget = camTarget;
    float desiredCamHeight = camHeight;

    SceneRenderer sceneRenderer;
    sceneRenderer.SetDefaultShader(unitShader);

    Squad* selected = nullptr;
    auto currentPortraitSquad = [&]() -> const Squad* {
        if (selected && selected->GetTeam() == 0 && !selected->IsEliminated()) return selected;
        if (generalGuard && !generalGuard->IsEliminated()) return generalGuard;
        for (const auto& squad : battle.GetSquads()) {
            if (squad->GetTeam() == 0 && !squad->IsEliminated()) return squad.get();
        }
        return nullptr;
    };
    bool planningPhase = true;  // T-9:開戰前停在回合層編牌
    bool inspectCharacter = false;
    bool previousInspectKey = false;
    int curDeck = 0, curRule = -1; // 牌組 UI 選中態
    Squad* arrowDragSquad = nullptr; // G-5:拖曳中的箭頭起點小隊
    Vector2 arrowDragPos;            // G-5:拖曳目前落點(cell)
    bool paused = false;
    float speedScale = 1.0f;
    bool prevL = false, prevR = false, prevSpace = false, prevEsc = false;
    double prevTime = glfwGetTime();
    int lastFbWidth = W, lastFbHeight = H;
    int visualFrame = 0;
    const double visualStarted = prevTime;
    std::vector<double> frameTimes;
    bool executionCaptured = false;
    bool portraitCaptured = false;
    bool planningCaptured = false;
    bool inspectionCaptured = false;
    auto beginBattle = [&]() {
        if (!CommitCampaign(campaign, flow, savePath, campaignError, [](auto&, auto&) { return true; })) return;
        deck.Commit(battle);
        plan.SetRallyPoint(battle.GetRallyPoint(0));
        const int arrows = plan.Apply(battle, &res, 0);
        battle.BeginExecution();
        planningPhase = false;
        eventLog.push_back(arrows > 0
            ? "作戰計畫已下達——開戰(" + std::to_string(arrows) + " 支箭頭生效)"
            : "作戰計畫已下達——開戰");
    };

    std::printf("斷橋可玩 demo — 左鍵選取,右鍵下令(CP),Space 暫停\n");

    while (!renderer.ShouldClose() && !backToTitle) {
        renderer.PollEvents();
        double now = glfwGetTime();
        if (visualCheck && now - visualStarted >= 90.0) {
            std::printf("[visual-check] timeout after 90 seconds\n");
            std::fflush(stdout);
            backToTitle = true;
            break;
        }
        const double frameSeconds = std::max(0.0, now - prevTime);
        float dt = (float)std::min(frameSeconds, 0.1);
        prevTime = now;
        ++visualFrame;
        if (visualCheck && visualFrame == 80) inspectCharacter = true;
        if ((visualCheck || campaignCheck) && visualFrame > 30) frameTimes.push_back(frameSeconds * 1000.0);

        // framebuffer 用於 GL viewport;window size 用於 ImGui/游標(HiDPI 下兩者不同)
        int dw = W, dh = H, ww = W, wh = H;
        glfwGetFramebufferSize(window, &dw, &dh);
        glfwGetWindowSize(window, &ww, &wh);
        if (dw <= 0 || dh <= 0) {
            glfwWaitEventsTimeout(0.05);
            prevTime = glfwGetTime();
            continue;
        }
        const float sx = (ww > 0) ? (float)dw / ww : 1.0f;
        const float sy = (wh > 0) ? (float)dh / wh : 1.0f;
        if (dw > 0 && dh > 0) {
            renderer.SetViewport(0, 0, dw, dh);
            if (dw != lastFbWidth || dh != lastFbHeight) {
                cam.SetViewport(0, 0, dw, dh);
                cam.SetPerspective(50.0f * 3.14159265f / 180.0f,
                                   (float)dw / dh, 0.1f, 300.0f);
                lastFbWidth = dw;
                lastFbHeight = dh;
            }
        }

        // ---- 鍵盤 ----
        bool space = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
        if (space && !prevSpace) paused = !paused;
        prevSpace = space;
        bool esc = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (esc && !prevEsc) { selected = nullptr; sync.SetSelectedSquad(nullptr); }
        prevEsc = esc;
        const bool inspectKey = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
        if (inspectKey && !previousInspectKey && !io.WantTextInput) inspectCharacter = !inspectCharacter;
        previousInspectKey = inspectKey;
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) speedScale = 0.5f;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) speedScale = 1.0f;
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) speedScale = 2.0f;

        float pan = 12.0f * dt;
        float panX = 0.0f, panZ = 0.0f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) panZ -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) panZ += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) panX -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) panX += 1.0f;
        const float panLength = std::sqrt(panX * panX + panZ * panZ);
        if (panLength > 0.0f) {
            desiredCamTarget.x += pan * panX / panLength;
            desiredCamTarget.z += pan * panZ / panLength;
        }
        desiredCamTarget.x = std::clamp(desiredCamTarget.x, 0.0f, FW);
        desiredCamTarget.z = std::clamp(desiredCamTarget.z, 0.0f, FH);
        // 滾輪在 ImGui 視窗上時不縮放相機
        if (!io.WantCaptureMouse) {
            desiredCamHeight = std::clamp(desiredCamHeight - (float)g_scroll * 2.0f, 4.0f, 45.0f);
        }
        g_scroll = 0.0;
        // 指數平滑以秒為單位，30/60/120 FPS 下有一致的鏡頭反應。
        const float cameraBlend = 1.0f - std::exp(-14.0f * dt);
        camTarget = camTarget + (desiredCamTarget - camTarget) * cameraBlend;
        camHeight += (desiredCamHeight - camHeight) * cameraBlend;
        camBack = camHeight * 0.5f; // 保持俯角，滾輪可真正拉近人物細節。
        cam.SetPosition(Vector3(camTarget.x, camHeight, camTarget.z + camBack));
        cam.SetTarget(camTarget);
        if (inspectCharacter) {
            const Squad* focus = currentPortraitSquad();
            if (focus && !focus->IsEliminated()) {
                const Vector2 p = focus->GetPosition();
                cam.SetPosition(Vector3(p.x * CELL + 1.7f * CELL, 1.65f * CELL,
                                        p.y * CELL + 3.0f * CELL));
                cam.SetTarget(Vector3(p.x * CELL, 0.85f * CELL, p.y * CELL));
            } else inspectCharacter = false;
        }

        battle.SetTimeScale(paused ? 0.0f : speedScale);
        if (visualCheck && planningPhase && visualFrame >= 60) beginBattle();

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

        if (campaignCheck && planningPhase && visualFrame >= 10) beginBattle();
        // Explicit verification fixture: exercise rendering, live casualties and settlement;
        // headless mission tests verify the actual protection win/loss rules.
        if (campaignCheck && !naturalCheck && visualFrame == 80 && battle.GetPhase() == BattlePhase::Execution) {
            if (!allies.empty()) allies.front()->ApplyCasualties(2);
            for (auto* enemy : enemies) enemy->ApplyCasualties(enemy->GetMembers());
        }
        battle.Update(dt);
        roster.Update(battle); // T-8:殲滅偵測→記陣亡
        if (battle.GetOutcome() != BattleOutcome::Ongoing && !settlementAttempted) settleBattle(battle, roster);
        if (campaignCheck && settlementSaved && visualFrame >= 90) backToTitle = true;
        sync.Sync(battle);

        // ---- 渲染 ----
        { // U-1:clear color 跟主題走
            const ImVec4 cc = UITheme::ClearColor(theme);
            renderer.SetClearColor(Vector3(cc.x, cc.y, cc.z));
        }
        renderer.Clear();
        renderer.EnableDepthTest(true);
        renderer.EnableCulling(false); // 手排頂點繞序不保證 CCW
        unitShader->Bind();
        unitShader->SetUniformFloat("uVisualTime", static_cast<float>(now));
        auto renderItems = sceneRenderer.CollectRenderList(scene, cam);
        std::vector<RenderItem> opaqueItems, fogItems, contactShadows;
        opaqueItems.reserve(renderItems.size());
        for (auto& item : renderItems) {
            if (inspectCharacter && item.mesh == pinMesh) continue; // 近景略過遮住人物的戰術圖釘。
            if (item.node && item.node->GetName().rfind("__fog_c", 0) == 0) {
                item.shader = fogShader;
                fogItems.push_back(item);
            } else {
                if (item.mesh == capMesh) {
                    item.shader = soldierShader;
                    RenderItem shadow = item;
                    shadow.mesh = cloudMesh;
                    shadow.shader = fogShader;
                    shadow.color = Vector3(0.018f, 0.021f, 0.016f);
                    contactShadows.push_back(shadow);
                }
                opaqueItems.push_back(item);
            }
        }
        sceneRenderer.SubmitRenderList(opaqueItems, cam);
        if (visualCheck && !portraitCaptured && visualFrame >= 45) {
            // 真實場景的獨立近景驗證；不改正常操作的相機或遊戲規則。
            for (const auto& squad : battle.GetSquads()) {
                if (squad->GetTeam() != 0 || squad->IsEliminated()) continue;
                const Vector2 p = squad->GetPosition();
                Camera portrait = cam;
                portrait.SetPosition(Vector3(p.x * CELL + 1.5f * CELL,
                    1.65f * CELL, p.y * CELL + 2.6f * CELL));
                portrait.SetTarget(Vector3(p.x * CELL, 0.85f * CELL, p.y * CELL));
                auto closeItems = sceneRenderer.CollectRenderList(scene, portrait);
                closeItems.erase(std::remove_if(closeItems.begin(), closeItems.end(),
                    [](const RenderItem& item) {
                        return item.node && item.node->GetName().rfind("__fog_c", 0) == 0;
                    }), closeItems.end());
                for (auto& item : closeItems) {
                    if (item.mesh == capMesh) item.shader = soldierShader;
                }
                renderer.Clear();
                sceneRenderer.SubmitRenderList(closeItems, portrait);
                portraitCaptured = CaptureFrame("soldier-closeup.png", dw, dh);
                if (!portraitCaptured) backToTitle = true;
                renderer.Clear();
                sceneRenderer.SubmitRenderList(opaqueItems, cam);
                prevTime = glfwGetTime(); // 不把額外近景繪製/PNG 計入正常幀時間。
                break;
            }
        }
        if (!fogItems.empty() || !contactShadows.empty()) {
            // 所有區域共用紫色與同一高度；交疊 alpha 不依候選提交順序。
            const GLboolean hadBlend = glIsEnabled(GL_BLEND);
            const GLboolean hadDepthTest = glIsEnabled(GL_DEPTH_TEST);
            GLboolean hadDepthWrite;
            GLint blendSrcRGB, blendDstRGB, blendSrcAlpha, blendDstAlpha;
            GLint blendEquationRGB, blendEquationAlpha;
            glGetBooleanv(GL_DEPTH_WRITEMASK, &hadDepthWrite);
            glGetIntegerv(GL_BLEND_SRC_RGB, &blendSrcRGB);
            glGetIntegerv(GL_BLEND_DST_RGB, &blendDstRGB);
            glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrcAlpha);
            glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDstAlpha);
            glGetIntegerv(GL_BLEND_EQUATION_RGB, &blendEquationRGB);
            glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &blendEquationAlpha);
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            sceneRenderer.SubmitRenderList(contactShadows, cam);
            sceneRenderer.SubmitRenderList(fogItems, cam);
            glBlendFuncSeparate(blendSrcRGB, blendDstRGB, blendSrcAlpha, blendDstAlpha);
            glBlendEquationSeparate(blendEquationRGB, blendEquationAlpha);
            if (!hadBlend) glDisable(GL_BLEND);
            glDepthMask(hadDepthWrite);
            if (!hadDepthTest) glDisable(GL_DEPTH_TEST);
        }

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
        ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_Always);
        ImGui::Begin("戰役指揮", nullptr,
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("階段: %s%s",
                    battle.GetPhase() == BattlePhase::Execution ? "執行" :
                    battle.GetPhase() == BattlePhase::Deployment ? "部署" : "結算",
                    paused ? "(暫停)" : "");
        ImGui::Text("第 %d 章 · %s", chapter->number, chapter->title.c_str());
        ImGui::TextWrapped("決策：%s", chapterChoice->label.c_str());
        if (chapterChoice->holdSeconds > 0) ImGui::TextWrapped("保護 %s：%.0f / %.0f 秒", rearGuard->GetName().c_str(), battle.GetElapsed(), chapterChoice->holdSeconds);
        if (!campaignError.empty()) ImGui::TextWrapped("%s", campaignError.c_str());
        if (battle.GetPhase() == BattlePhase::Execution && ImGui::Button("撤退 · 記為失利")) ImGui::OpenPopup("確認撤退");
        if (ImGui::BeginPopupModal("確認撤退", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::TextUnformatted("本章記為失利，已發生的傷亡會延續。");
            if (ImGui::Button("確認撤退")) { battle.Withdraw(); ImGui::CloseCurrentPopup(); }
            ImGui::SameLine(); if (ImGui::Button("繼續作戰")) ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
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
        if (ImGui::CollapsingHeader("操作與敵情說明")) {
        ImGui::TextColored(ImVec4(0.25f, 0.45f, 0.95f, 1), "藍色：我軍小隊");
        ImGui::TextColored(ImVec4(0.95f, 0.30f, 0.25f, 1), "紅色：已揭露敵軍");
        ImGui::TextColored(ImVec4(0.70f, 0.55f, 1.0f, 1), "紫色：敵軍可能所在區域");
        ImGui::TextWrapped("紫色區域不是敵軍或建築數量；越濃代表該候選位置機率越高。");
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
        ImGui::TextDisabled("左鍵選取我軍 | 點紫色區域探測(1情報)");
        ImGui::TextDisabled("Shift+點紫色區域觀測(2情報)，揭露敵軍");
        ImGui::TextDisabled("右鍵下令 | Space 暫停 | 1/2/3 倍速");
        ImGui::TextDisabled("WASD 平移 | 滾輪縮放 | Esc 取消選取");
        if (planningPhase) {
            ImGui::TextDisabled("Alt+左鍵=移目標點 | Alt+右鍵=移集結點");
            ImGui::TextDisabled("Ctrl+左鍵自小隊拖出=畫進攻箭頭");
        }
        }
        const float commandPanelBottom = ImGui::GetWindowPos().y + ImGui::GetWindowSize().y + 8.0f;
        ImGui::End();

        // 肖像是角色資訊卡；兵力、士氣與命令皆來自目前小隊。
        const Squad* portraitSquad = currentPortraitSquad();
        const float portraitRoom = static_cast<float>(wh) - 202.0f - commandPanelBottom;
        if (portraitSquad && portraitRoom >= 160.0f) {
            const float cardHeight = std::min(portraitRoom, 430.0f);
            ImGui::SetNextWindowPos(ImVec2(8, commandPanelBottom), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(300, cardHeight), ImGuiCond_Always);
            ImGui::Begin("軍官與小隊", nullptr, ImGuiWindowFlags_NoCollapse);
            ImGui::TextColored(ImVec4(0.83f, 0.71f, 0.43f, 1), "%s", portraitSquad->GetName().c_str());
            const float portraitSize = std::min(276.0f, std::max(60.0f, cardHeight - 145.0f));
            CharacterArt::Portrait(portraits.get(), portraitSquad == generalGuard ? 0 : 1,
                                   ImVec2(portraitSize, portraitSize));
            ImGui::Text("兵力 %d / %d    士氣 %.0f%%", portraitSquad->GetMembers(),
                        portraitSquad->GetMaxMembers(), portraitSquad->GetMorale() * 100.0f);
            ImGui::TextDisabled("當前命令 · %s", OrderName(portraitSquad->GetOrder()));
            if (ImGui::Button(inspectCharacter ? "返回戰場視角 (C)" : "近看戰場人物 (C)", ImVec2(-1, 0))) {
                inspectCharacter = !inspectCharacter;
            }
            ImGui::End();
        }

        // ---- N-2 敵將檔案:判詞是聽聞態,親衛雲揭露才回真值 ----
        {
            const int guardEid = battle.GetFogEntityId(e0);
            if (guardEid >= 0 && fog.IsRevealed(guardEid)) {
                dossier.Verify(glock); // 冪等：寫真值+標 verified
            }
            if (const HearsayEntry* he = dossier.Find(glock.GetName())) {
                ImGui::SetNextWindowPos(ImVec2(ww - 308.0f, 8),
                                        ImGuiCond_Always);
                ImGui::SetNextWindowSize(ImVec2(300, 214), ImGuiCond_Always);
                ImGui::Begin("敵將檔案", nullptr,
                             ImGuiWindowFlags_NoCollapse |
                                 ImGuiWindowFlags_AlwaysAutoResize);
                ImGui::Text("%s %s", glock.GetName().c_str(),
                            glock.GetEpithet().c_str());
                CharacterArt::Portrait(portraits.get(), 2, ImVec2(94, 94));
                ImGui::SameLine();
                ImGui::BeginGroup();
                ImGui::TextColored(ImVec4(0.78f, 0.36f, 0.28f, 1), "敵方指揮官");
                ImGui::TextDisabled(he->verified ? "情報已確認" : "情報待查證");
                ImGui::EndGroup();
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
        if (planningPhase && !inspectCharacter) {
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
            if (ImGui::Button("開 戰", ImVec2(120, 32))) {
                beginBattle();
            }
            ImGui::SameLine();
            ImGui::TextDisabled("寫好劇本再開戰;CP 留給救火");
            ImGui::End();
        }

        // ---- T-10 全軍狀態列:每隊兵力/士氣條 + CP 介入按鈕 ----
        if (!planningPhase) {
            ImGui::SetNextWindowPos(ImVec2((float)ww - 308.0f, 230.0f),
                                    ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(300, std::max(120.0f, static_cast<float>(wh) - 488.0f)), ImGuiCond_Always);
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
        ImGui::SetNextWindowPos(ImVec2(8, std::max(0.0f, (float)wh - 190.0f)),
                                ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(430, 182), ImGuiCond_Always);
        ImGui::Begin("戰況", nullptr,
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        for (const auto& e : eventLog) ImGui::TextUnformatted(e.c_str());
        ImGui::End();

        // ---- 小地圖（右下錨點;形狀編碼:■我 ◆敵 ●雲,DESIGN 強制——
        //      陣營不靠色相區分,色弱/主題切換下語義不變）----
        {
            const float ms = 170.0f * uiScale;
            ImGui::SetNextWindowPos(
                ImVec2((float)ww - ms - 18.0f,
                       std::max(0.0f, (float)wh - ms - 60.0f)),
                ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(ms + 16.0f, ms + 62.0f),
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
                        : IM_COL32(64, 115, 242, 255), // 我軍藍
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
                        IM_COL32(242, 77, 64, 255), 2.0f);
                } else {
                    for (const auto& cand : fog.GetCloud((int)id)) {
                        const int alpha = static_cast<int>(80.0 +
                            160.0 * std::clamp(cand.second, 0.0, 1.0));
                        UITheme::DrawMarker(
                            mdl, toMap(cand.first), mr * 0.7f,
                            UITheme::MarkerShape::CloudCircle,
                            IM_COL32(179, 140, 255, alpha));
                    }
                }
            }
            // 圖例(形狀+文字雙編碼)
            ImGui::TextDisabled("■我軍  ◆敵軍  ●疑似敵情");
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
            ImGui::SetNextWindowPos(ImVec2(ww * 0.5f - 120, wh * 0.35f),
                                    ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(240, 0), ImGuiCond_Always);
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
                hin.battleName = chapter->title;
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

            ImGui::SetNextWindowPos(ImVec2(ww * 0.5f - 330, wh * 0.42f),
                                    ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(660, 0), ImGuiCond_Always);
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
            ImGui::BeginChild("replaylist", ImVec2(0, 130), true);
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
            if (!settlementSaved) {
                ImGui::TextWrapped("%s", campaignError.c_str());
                if (ImGui::Button("重試保存結算")) settleBattle(battle, roster);
            }
            ImGui::BeginDisabled(!settlementSaved);
            if (ImGui::Button("查看戰後行軍帳", ImVec2(200, 36))) backToTitle = true;
            ImGui::EndDisabled();
            ImGui::End();
        }

        ImGui::PopFont();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (visualCheck) {
            if (!inspectionCaptured && visualFrame >= 100) {
                inspectionCaptured = CaptureFrame("visual-inspection.png", dw, dh);
                inspectCharacter = false;
                if (!inspectionCaptured) backToTitle = true;
                prevTime = glfwGetTime();
            }
            if (!planningCaptured && planningPhase && visualFrame >= 45) {
                planningCaptured = CaptureFrame("visual-planning.png", dw, dh);
                if (!planningCaptured) backToTitle = true;
                prevTime = glfwGetTime();
            }
            if (visualFrame >= 120 && !executionCaptured) {
                executionCaptured = CaptureFrame("visual-execution.png", dw, dh);
                if (!executionCaptured) backToTitle = true;
                prevTime = glfwGetTime(); // PNG 寫檔時間不計入幀時間。
            }
            const bool finished = battle.GetOutcome() != BattleOutcome::Ongoing;
            if (finished || backToTitle || now - visualStarted >= 90.0) {
                visualCheckPassed = finished && executionCaptured && portraitCaptured && planningCaptured && inspectionCaptured;
                if (finished) visualCheckPassed = CaptureFrame("visual-result.png", dw, dh) && visualCheckPassed;
                double sum = 0.0;
                for (double ms : frameTimes) sum += ms;
                std::sort(frameTimes.begin(), frameTimes.end());
                const double mean = frameTimes.empty() ? 0.0 : sum / frameTimes.size();
                const double p95 = frameTimes.empty() ? 0.0 : frameTimes[static_cast<size_t>((frameTimes.size() - 1) * 0.95)];
                std::printf("[visual-check] frames=%zu mean=%.2fms p95=%.2fms FPS=%.1f outcome=%d\n",
                    frameTimes.size(), mean, p95, mean > 0.0 ? 1000.0 / mean : 0.0, static_cast<int>(battle.GetOutcome()));
                std::fflush(stdout);
                backToTitle = true;
            }
        }
        renderer.SwapBuffers();
    }

    if (battle.GetOutcome() != BattleOutcome::Ongoing && !settlementAttempted) {
        roster.Update(battle); settleBattle(battle, roster);
    }
    if (campaignCheck && !frameTimes.empty()) {
        double sum=0; for(double ms:frameTimes) sum+=ms;
        std::sort(frameTimes.begin(),frameTimes.end());
        std::printf("[campaign-performance] chapter=%d frames=%zu mean=%.2fms p95=%.2fms FPS=%.1f\n",
            chapter->number,frameTimes.size(),sum/frameTimes.size(),frameTimes[size_t((frameTimes.size()-1)*.95)],1000.0/(sum/frameTimes.size()));
    }
    battle.BindFog(nullptr); // fog 是 local,先於 battle 解構——解綁防懸空

        } // ---- 戰鬥場次 scope 結束:battle/fog/scene 全數析構 ----
        screen = (renderer.ShouldClose() || visualCheck) ? ShellScreen::Quit : settlementSaved ? ShellScreen::Aftermath : ShellScreen::Story;
        shellFrames = 0;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    renderer.Shutdown();
    if (campaignCheck && (campaign.progress.stage != Campaign::CampaignStage::Complete || !campaignError.empty())) return 2;
    return visualCheckPassed ? 0 : 2;
}
