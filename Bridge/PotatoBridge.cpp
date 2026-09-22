// PotatoBridge — C ABI 橋：給 C#（P/Invoke）消費的無頭戰鬥 API。
//
// 設計：
// - 不透明 handle 持有 BattleController + 可選內建 QuantumFog；
//   squadId = CreateSquad 順序索引（小隊不移除 → 索引穩定）。
// - 快照走二進制 little-endian（每幀一次 P/Invoke，不做 JSON）。
// - 部署期 API 只建場景；BeginExecution 後進即時層。
//
// 快照格式（全 little-endian）：
//   u32 'PBS1' | u32 version=1
//   f32 elapsed | i32 phase | i32 outcome | f32 timeScale
//   i32 cp0 | i32 cp1 | i32 intel0 | i32 intel1 | i32 gridW | i32 gridH
//   i32 obstacleCount → 每筆 u8 x, u8 y
//   i32 squadCount → 每筆：
//     i32 id | i32 team | f32 x,y | i32 members | i32 maxMembers
//     f32 morale | i32 order | f32 orderTx,orderTy
//     i32 engaged | i32 routing | i32 eliminated | i32 underAttack
//     i32 fogEntityId(-1) | i32 unitClass | i32 generalGuard
//   i32 fogCount → 每實體：
//     i32 entityId | i32 revealed | i32 ownerSquadId(-1)
//     f32 revealedX,revealedY | i32 candCount → 每筆 f32 x,y,prob

#include "Gameplay/BattleController.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/QuantumFog.h"

#include <cstring>
#include <memory>
#include <vector>

#if defined(_WIN32)
#define PB_API extern "C" __declspec(dllexport)
#else
#define PB_API extern "C" __attribute__((visibility("default")))
#endif

namespace {

using namespace Potato::Gameplay;
using Potato::Vector2;

struct PB_Battle {
    std::unique_ptr<BattleController> battle;
    std::unique_ptr<QuantumFog> fog;      // PB_EnableFog 後內建持有
    std::unique_ptr<BattleResources> res; // fog 觀測的情報點池
    std::vector<Squad*> squadIndex;       // squadId → Squad*
};

PB_Battle* As(void* h) { return static_cast<PB_Battle*>(h); }

Squad* SquadAt(PB_Battle* b, int squadId) {
    if (!b || squadId < 0 ||
        squadId >= static_cast<int>(b->squadIndex.size())) return nullptr;
    return b->squadIndex[static_cast<size_t>(squadId)];
}

// ---- little-endian writer ----
struct Writer {
    unsigned char* p;
    unsigned char* end;
    bool ok = true;
    void U8(unsigned v) {
        if (p + 1 > end) { ok = false; return; }
        *p++ = static_cast<unsigned char>(v);
    }
    void U32(uint32_t v) {
        if (p + 4 > end) { ok = false; return; }
        std::memcpy(p, &v, 4); p += 4;
    }
    void I32(int32_t v) { U32(static_cast<uint32_t>(v)); }
    void F32(float v) {
        if (p + 4 > end) { ok = false; return; }
        std::memcpy(p, &v, 4); p += 4;
    }
};

} // namespace

PB_API void* PB_Create(int gridW, int gridH, float cellSize) {
    if (gridW <= 0 || gridH <= 0 || cellSize <= 0.0f) return nullptr;
    auto* b = new PB_Battle();
    b->battle = std::make_unique<BattleController>(gridW, gridH, cellSize);
    return b;
}

PB_API void PB_Destroy(void* h) { delete As(h); }

// ---- 部署 ----
PB_API void PB_SetObstacle(void* h, int x, int y, int blocked) {
    if (auto* b = As(h)) b->battle->GetField().SetObstacle(x, y, blocked != 0);
}
PB_API void PB_SetTerrain(void* h, int x, int y, int terrain) {
    if (auto* b = As(h)) {
        b->battle->GetField().SetTerrain(
            x, y, static_cast<TerrainType>(terrain));
    }
}
PB_API void PB_SetObjective(void* h, int team, float x, float y) {
    if (auto* b = As(h)) b->battle->SetObjective(team, Vector2(x, y));
}
PB_API void PB_SetRallyPoint(void* h, int team, float x, float y) {
    if (auto* b = As(h)) b->battle->SetRallyPoint(team, Vector2(x, y));
}
PB_API void PB_SetCommandPoints(void* h, int team, int points) {
    if (auto* b = As(h)) b->battle->SetCommandPoints(team, points);
}
PB_API void PB_SetRoutShock(void* h, float radius, float moraleHit) {
    if (auto* b = As(h)) b->battle->SetRoutShock(radius, moraleHit);
}
PB_API void PB_SetCombatWidth(void* h, int width) {
    if (auto* b = As(h)) b->battle->SetCombatWidth(width);
}
PB_API void PB_SetJamming(void* h, float radius) {
    if (auto* b = As(h)) b->battle->SetJamming(radius);
}

PB_API int PB_AddSquad(void* h, const char* name, int team,
                       float x, float y, int members) {
    auto* b = As(h);
    if (!b || !name) return -1;
    Squad* s = b->battle->CreateSquad(name, team, Vector2(x, y), members);
    if (!s) return -1;
    b->squadIndex.push_back(s);
    return static_cast<int>(b->squadIndex.size()) - 1;
}

PB_API void PB_SetUnitClass(void* h, int squadId, int cls) {
    if (Squad* s = SquadAt(As(h), squadId)) {
        s->SetUnitClass(static_cast<UnitClass>(cls));
    }
}
PB_API void PB_SetGeneralGuard(void* h, int squadId, int v) {
    if (Squad* s = SquadAt(As(h), squadId)) s->SetGeneralGuard(v != 0);
}
PB_API void PB_SetSpeed(void* h, int squadId, float speed) {
    if (Squad* s = SquadAt(As(h), squadId)) s->SetSpeed(speed);
}

// doctrine JSON（potato.doctrine_set/1）；失敗回 0
PB_API int PB_SetDoctrineJson(void* h, int squadId, const char* json) {
    auto* b = As(h);
    Squad* s = SquadAt(b, squadId);
    if (!s || !json) return 0;
    DoctrineSet d;
    if (!d.FromJson(json)) return 0;
    b->battle->AssignDoctrine(s, d);
    return 1;
}

PB_API int PB_BeginExecution(void* h) {
    auto* b = As(h);
    return (b && b->battle->BeginExecution()) ? 1 : 0;
}

// ---- 敵情霧（可選）----
PB_API void PB_EnableFog(void* h) {
    auto* b = As(h);
    if (!b || b->fog) return;
    if (!b->res) b->res = std::make_unique<BattleResources>();
    b->fog = std::make_unique<QuantumFog>();
    b->fog->BindResources(b->res.get()); // Observe/Probe 扣情報點
    b->battle->BindFog(b->fog.get());
}

// 情報/CP 資源池（Observe/Probe 花情報；CP 由 controller 自管，
// 此處 Setup 會同步進 controller）
PB_API void PB_SetupResources(void* h, int team, int intel, int cp) {
    auto* b = As(h);
    if (!b) return;
    if (!b->res) b->res = std::make_unique<BattleResources>();
    b->res->Setup(*b->battle, team, intel, cp);
}
PB_API void PB_AddIntel(void* h, int team, int amount) {
    auto* b = As(h);
    if (b && b->res) b->res->AddIntel(team, amount);
}

PB_API int PB_FogAddCloud(void* h, const char* name, int team,
                          float centerX, float centerY, float radius,
                          int count, float minSpacing) {
    auto* b = As(h);
    if (!b || !b->fog || !name) return -1;
    return b->fog->AddEntityCloud(name, team,
                                  Vector2(centerX, centerY), radius,
                                  count, minSpacing);
}

PB_API int PB_FogBindSquad(void* h, int squadId, int entityId) {
    auto* b = As(h);
    Squad* s = SquadAt(b, squadId);
    if (!s || entityId < 0) return 0;
    b->battle->BindFogSquad(s, entityId);
    return 1;
}

PB_API int PB_FogObserve(void* h, int entityId, float x, float y) {
    auto* b = As(h);
    return (b && b->fog &&
            b->fog->Observe(entityId, Vector2(x, y))) ? 1 : 0;
}
PB_API int PB_FogProbe(void* h, int entityId, float x, float y,
                       float strength) {
    auto* b = As(h);
    return (b && b->fog &&
            b->fog->Probe(entityId, Vector2(x, y), strength)) ? 1 : 0;
}
PB_API int PB_FogReveal(void* h, int entityId, float x, float y) {
    auto* b = As(h);
    return (b && b->fog &&
            b->fog->Reveal(entityId, Vector2(x, y))) ? 1 : 0;
}
PB_API void PB_SetFogRevealRange(void* h, float cells) {
    if (auto* b = As(h)) b->battle->SetFogRevealRange(cells);
}

// ---- 執行 ----
PB_API void PB_Step(void* h, float realDt) {
    if (auto* b = As(h)) b->battle->Update(realDt);
}
PB_API void PB_SetTimeScale(void* h, float scale) {
    if (auto* b = As(h)) b->battle->SetTimeScale(scale);
}
PB_API float PB_GetTimeScale(void* h) {
    auto* b = As(h);
    return b ? b->battle->GetTimeScale() : 0.0f;
}
PB_API int PB_InterveneMove(void* h, int squadId, int order,
                            float x, float y, float holdSeconds) {
    auto* b = As(h);
    Squad* s = SquadAt(b, squadId);
    if (!s) return 0;
    return b->battle->Intervene(s, static_cast<SquadOrder>(order),
                                Vector2(x, y), holdSeconds) ? 1 : 0;
}
PB_API int PB_InterveneEngage(void* h, int squadId, int targetSquadId,
                              float holdSeconds) {
    auto* b = As(h);
    Squad* s = SquadAt(b, squadId);
    Squad* t = SquadAt(b, targetSquadId);
    if (!s || !t) return 0;
    return b->battle->Intervene(s, SquadOrder::Engage, t, holdSeconds)
               ? 1
               : 0;
}
PB_API int PB_GeneralRally(void* h, int squadId) {
    auto* b = As(h);
    Squad* s = SquadAt(b, squadId);
    return (s && b->battle->GeneralRally(s)) ? 1 : 0;
}
PB_API int PB_GeneralCharge(void* h, int squadId) {
    auto* b = As(h);
    Squad* s = SquadAt(b, squadId);
    return (s && b->battle->GeneralCharge(s)) ? 1 : 0;
}

// ---- 快照 ----
PB_API int PB_SnapshotSize(void* h) {
    auto* b = As(h);
    if (!b) return 0;
    const auto& squads = b->battle->GetSquads();
    int size = 8 + 40 + 4; // magic+ver, header, obstacleCount
    size += b->battle->GetField().GetWidth() *
            b->battle->GetField().GetHeight() * 2;
    size += 4 + static_cast<int>(squads.size()) * 68;
    if (b->fog) {
        size += 4;
        for (size_t i = 0; i < b->fog->EntityCount(); ++i) {
            const UncertainEntity* e =
                b->fog->GetEntity(static_cast<int>(i));
            if (!e) continue;
            size += 24 + static_cast<int>(e->candidates.size()) * 12;
        }
    } else {
        size += 4;
    }
    return size;
}

PB_API int PB_Snapshot(void* h, unsigned char* dst, int capacity) {
    auto* b = As(h);
    if (!b || !dst || capacity <= 0) return 0;
    Writer w{dst, dst + capacity};

    w.U32(0x31534250u); // 'PBS1'
    w.U32(1);           // version
    w.F32(b->battle->GetElapsed());
    w.I32(static_cast<int>(b->battle->GetPhase()));
    w.I32(static_cast<int>(b->battle->GetOutcome()));
    w.F32(b->battle->GetTimeScale());
    w.I32(b->battle->GetCommandPoints(0));
    w.I32(b->battle->GetCommandPoints(1));
    w.I32(b->res ? b->res->GetIntel(0) : 0);
    w.I32(b->res ? b->res->GetIntel(1) : 0);
    const FlowField& field = b->battle->GetField();
    w.I32(field.GetWidth());
    w.I32(field.GetHeight());

    const int gw = field.GetWidth(), gh = field.GetHeight();
    w.I32(gw * gh); // 全格輸出（0/1）——一次性讀不用協調變更集
    for (int y = 0; y < gh; ++y) {
        for (int x = 0; x < gw; ++x) {
            w.U8(field.IsBlocked(x, y) ? 1 : 0);
            w.U8(static_cast<unsigned>(field.GetTerrain(x, y)));
        }
    }

    const auto& squads = b->battle->GetSquads();
    w.I32(static_cast<int>(squads.size()));
    for (size_t i = 0; i < squads.size(); ++i) {
        const Squad* s = squads[i].get();
        const Vector2& p = s->GetPosition();
        const Vector2& ot = s->GetOrderTarget();
        w.I32(static_cast<int>(i));
        w.I32(s->GetTeam());
        w.F32(p.x); w.F32(p.y);
        w.I32(s->GetMembers());
        w.I32(s->GetMaxMembers());
        w.F32(s->GetMorale());
        w.I32(static_cast<int>(s->GetOrder()));
        w.F32(ot.x); w.F32(ot.y);
        w.I32(s->IsEngaged() ? 1 : 0);
        w.I32(s->IsRouting() ? 1 : 0);
        w.I32(s->IsEliminated() ? 1 : 0);
        w.I32(s->IsUnderAttack() ? 1 : 0);
        w.I32(b->battle->GetFogEntityId(s));
        w.I32(static_cast<int>(s->GetUnitClass()));
        w.I32(s->IsGeneralGuard() ? 1 : 0);
    }

    const int fogCount =
        b->fog ? static_cast<int>(b->fog->EntityCount()) : 0;
    w.I32(fogCount);
    for (int i = 0; i < fogCount; ++i) {
        const bool revealed = b->fog->IsRevealed(i);
        const Vector2 rp = revealed ? b->fog->GetRevealedPos(i)
                                    : Vector2(0.0f, 0.0f);
        const Squad* owner = b->battle->GetFogSquad(i);
        int ownerIdx = -1;
        if (owner) {
            for (size_t j = 0; j < squads.size(); ++j) {
                if (squads[j].get() == owner) {
                    ownerIdx = static_cast<int>(j);
                    break;
                }
            }
        }
        const auto cloud = b->fog->GetCloud(i);
        w.I32(i);
        w.I32(revealed ? 1 : 0);
        w.I32(ownerIdx);
        w.F32(rp.x); w.F32(rp.y);
        w.I32(static_cast<int>(cloud.size()));
        for (const auto& [pos, prob] : cloud) {
            w.F32(pos.x); w.F32(pos.y);
            w.F32(static_cast<float>(prob));
        }
    }
    if (!w.ok) return 0;
    return static_cast<int>(w.p - dst);
}
