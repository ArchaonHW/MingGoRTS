#include "BattleSceneSync.h"

#include "MathUtils/CurlNoise.h"
#include "MathUtils/GustField.h"
#include "QuantumFog.h"
#include "BattleController.h"
#include "Squad.h"

#include "Scene/SceneNode.h"
#include "Rendering/RenderableComponent.h"

#include <algorithm>
#include <cmath>
#include <string>
#include <unordered_set>

namespace Potato {
namespace Gameplay {

Vector3 BattleSceneSync::TeamColor(int team) {
    switch (team) {
        case 0:  return Vector3(0.30f, 0.50f, 0.90f); // 玩家:藍
        case 1:  return Vector3(0.90f, 0.30f, 0.25f); // 敵方:紅
        case 2:  return Vector3(0.35f, 0.80f, 0.40f); // 友軍:綠
        default: return Vector3(0.60f, 0.60f, 0.62f); // 中立:灰
    }
}

void BattleSceneSync::Attach(BattleController& battle, SceneGraph& sceneGraph,
                             float cell) {
    Detach();
    scene = &sceneGraph;
    cellSize = cell;

    parentNode = MakeShared<SceneNode>("__battle_units");
    if (scene->GetRootNode()) {
        scene->GetRootNode()->AddChild(parentNode);
    } else {
        scene->SetRootNode(parentNode);
    }

    for (auto& squad : battle.GetSquads()) {
        CreateBinding(squad.get());
    }
}

void BattleSceneSync::CreateBinding(Squad* squad) {
    auto node = MakeShared<SceneNode>(squad->GetName());
    auto rc = MakeShared<RenderableComponent>();
    rc->mesh = unitMesh;
    rc->color = TeamColor(squad->GetTeam());
    rc->boundingRadius = 0.6f * cellSize;
    node->SetRenderable(rc);
    parentNode->AddChild(node);
    bindings.push_back({squad, node, rc});
    EnsureOverlay(bindings.back());
}

void BattleSceneSync::EnsureOverlay(Binding& b) {
    // 選取環:mesh 為空就拆掉既有節點,否則建立或更新
    if (ringMesh) {
        if (!b.ringNode) {
            b.ringNode = MakeShared<SceneNode>("__sel_ring");
            // y=0.22:高過渡口橋板頂(≈0.17)與水面(≈0.075),不被地形遮擋
            b.ringNode->SetLocalPosition(Vector3(0.0f, 0.22f, 0.0f));
            b.ringNode->SetActive(false);
            b.node->AddChild(b.ringNode);
        }
        if (!b.ringNode->GetRenderable()) {
            b.ringNode->SetRenderable(MakeShared<RenderableComponent>());
        }
        RenderableComponent* rc = b.ringNode->GetRenderable();
        rc->mesh = ringMesh;
        rc->color = Vector3(1.0f, 0.9f, 0.25f); // 選取環:亮黃
        rc->boundingRadius = 0.9f * cellSize;
    } else if (b.ringNode) {
        b.node->RemoveChild(b.ringNode.get());
        b.ringNode.reset();
    }

    // 血條:bg+fill 成對;任一 mesh 為空就整組拆
    if (barBgMesh && barFillMesh) {
        if (!b.barBgNode) {
            b.barBgNode = MakeShared<SceneNode>("__hp_bg");
            b.node->AddChild(b.barBgNode);
            b.barFillNode = MakeShared<SceneNode>("__hp_fill");
            b.node->AddChild(b.barFillNode);
            b.barFillRc = MakeShared<RenderableComponent>();
            b.barFillRc->color = Vector3(0.2f, 0.9f, 0.2f);
            b.barFillNode->SetRenderable(b.barFillRc);
        }
        // bg 版面在這裡重放(fill 由 Sync 依血量重寫),barY/barWidth 改動可套用
        b.barBgNode->SetLocalPosition(Vector3(0.0f, barY, 0.0f));
        b.barBgNode->SetLocalScale(Vector3(barWidth, 1.0f, 1.0f));
        if (!b.barBgNode->GetRenderable()) {
            b.barBgNode->SetRenderable(MakeShared<RenderableComponent>());
        }
        RenderableComponent* bgRc = b.barBgNode->GetRenderable();
        bgRc->mesh = barBgMesh;
        bgRc->color = Vector3(0.10f, 0.10f, 0.12f);
        bgRc->boundingRadius = barWidth * 0.7f;

        b.barFillRc->mesh = barFillMesh;
        b.barFillRc->boundingRadius = barWidth * 0.7f;

        // overlay 超出單位包圍球(bar 在 barY 高處)——放大剔除半徑防早退
        b.renderable->boundingRadius =
            std::max(0.6f * cellSize, barY + barWidth * 0.5f);
    } else {
        if (b.barBgNode) {
            b.node->RemoveChild(b.barBgNode.get());
            b.barBgNode.reset();
        }
        if (b.barFillNode) {
            b.node->RemoveChild(b.barFillNode.get());
            b.barFillNode.reset();
            b.barFillRc.reset();
        }
        b.renderable->boundingRadius = 0.6f * cellSize;
    }
}

void BattleSceneSync::Sync(BattleController& battle) {
    if (!parentNode) return;

    // 補掛 Attach 之後才建立的 squad
    {
        std::unordered_set<Squad*> bound;
        for (auto& b : bindings) bound.insert(b.squad);
        for (auto& squad : battle.GetSquads()) {
            if (bound.find(squad.get()) == bound.end()) {
                CreateBinding(squad.get());
            }
        }
    }

    for (auto& b : bindings) {
        Squad* squad = b.squad;

        // Q-1 敵情霧:未揭露的 bound squad 藏真身(overlay 為子節點一併隱藏)
        if (fog) {
            const int fid = battle.GetFogEntityId(squad);
            if (fid >= 0 && !fog->IsRevealed(fid)) {
                b.node->SetActive(false);
                continue;
            }
            b.node->SetActive(true); // 揭露狀態恢復顯示(全滅由下方再隱)
        }

        if (squad->IsEliminated()) {
            b.node->SetActive(false);
            if (b.ringNode) b.ringNode->SetActive(false);
            if (b.barBgNode) b.barBgNode->SetActive(false);
            if (b.barFillNode) b.barFillNode->SetActive(false);
            continue;
        }

        const Vector2& p = squad->GetPosition();
        b.node->SetLocalPosition(Vector3(p.x * cellSize, 0.0f, p.y * cellSize));

        // 士氣映射亮度:潰逃半暗,滿士氣全亮
        float lum = squad->IsRouting() ? 0.35f
                                       : 0.55f + 0.45f * squad->GetMorale();
        Vector3 base = TeamColor(squad->GetTeam());
        b.renderable->color = base * lum;

        // overlay:選取環只在選中時顯示;血條在潰逃時隱藏
        if (b.ringNode) {
            b.ringNode->SetActive(squad == selected && !squad->IsRouting());
        }
        if (b.barBgNode && b.barFillNode) {
            bool showBar = !squad->IsRouting();
            b.barBgNode->SetActive(showBar);
            b.barFillNode->SetActive(showBar);
            if (showBar) {
                float pct = squad->GetHealthPct();
                b.barFillNode->SetLocalScale(Vector3(barWidth * pct, 1.0f, 1.0f));
                // 直立血條:fill 與 bg 同高,向前(z+)浮出避免 z-fighting
                b.barFillNode->SetLocalPosition(
                    Vector3(-barWidth * (1.0f - pct) * 0.5f, barY, 0.04f));
                b.barFillRc->color = Vector3(1.0f - pct, pct, 0.15f);
            }
        }
    }

    // Q-1:機率雲標記——每個未揭露 entity 一組候選標記,scale∝機率
    if (fog) {
        for (size_t id = 0; id < fog->EntityCount(); ++id) {
            const int eid = static_cast<int>(id);
            const Squad* owner = battle.GetFogSquad(eid);
            // 無主雲(entity 沒綁 squad)無法被觀測/接觸,永不顯示
            const bool hide = fog->IsRevealed(eid) || !owner ||
                              owner->IsEliminated();
            auto it = fogNodes.find(eid);
            if (hide) {
                if (it != fogNodes.end()) it->second->SetActive(false);
                continue;
            }
            SceneNode* node = EnsureFogNode(eid);
            if (!node) continue;
            node->SetActive(true);
            const auto cloud = fog->GetCloud(eid);
            const auto& children = node->GetChildren();
            for (size_t i = 0; i < children.size(); ++i) {
                SceneNode* c = children[i].get();
                if (i >= cloud.size()) {
                    c->SetActive(false); // 低機率候選被 GetCloud 篩掉,標記也藏
                    continue;
                }
                c->SetActive(true);
                Vector3 mpos(cloud[i].first.x * cellSize, 0.5f * cellSize,
                             cloud[i].first.y * cellSize);
                if (fogDrift && fogDriftStrength > 0.0f) {
                    // P-1：場在 (pos,t) 連續 → 標記平滑漂移不跳動
                    const Vector3 dv =
                        fogDrift->Sample(mpos, battle.GetElapsed());
                    // P-4：間歇陣風調製（E[g]=1，重尾突發）
                    const float g = fogGust
                        ? fogGust->Intensity(mpos, battle.GetElapsed())
                        : 1.0f;
                    mpos.x += dv.x * fogDriftStrength * g;
                    mpos.z += dv.z * fogDriftStrength * g;
                }
                // D-5 滲透修飾（唯讀消費）：陰影錯位靜態偏移 +
                // 定域正弦微移（相位依候選索引，無亂數可重現）
                if (seepageShadow > 0.0f) {
                    mpos.x += seepageShadow * cellSize * 0.5f;
                }
                if (seepageJitter > 0.0f) {
                    const float ph = static_cast<float>(i) * 1.7f;
                    mpos.y += seepageJitter * cellSize *
                              std::sin(battle.GetElapsed() * 3.0f + ph);
                }
                c->SetLocalPosition(mpos);
                const float s = 0.4f + static_cast<float>(cloud[i].second) * 2.0f;
                c->SetLocalScale(Vector3(s, s, s));
                if (auto* rc = c->GetRenderable()) {
                    // 滲透霧色偏移（等級1 局部異常）——零偏移=原色
                    rc->color = Vector3(0.55f, 0.45f, 0.85f) + seepageTint;
                }
            }
        }
    }
}

void BattleSceneSync::SetUnitMesh(SharedPtr<Mesh> mesh) {
    unitMesh = std::move(mesh);
    for (auto& b : bindings) {
        b.renderable->mesh = unitMesh;
    }
}

void BattleSceneSync::SetOverlayMeshes(SharedPtr<Mesh> ring,
                                       SharedPtr<Mesh> barBg,
                                       SharedPtr<Mesh> barFill,
                                       float barHeight, float width) {
    ringMesh = std::move(ring);
    barBgMesh = std::move(barBg);
    barFillMesh = std::move(barFill);
    barY = barHeight;
    barWidth = width;
    for (auto& b : bindings) {
        EnsureOverlay(b);
    }
}

void BattleSceneSync::SetFog(const QuantumFog* f) {
    fog = f;
    // 換綁/解綁後舊標記的 entityId 對不上新 fog——先把節點
    // 從場景摘除(否則殘留永遠顯示的舊雲),再清空映射表
    for (auto& kv : fogNodes) {
        if (kv.second) {
            kv.second->SetActive(false);
            if (parentNode) {
                parentNode->RemoveChild(kv.second.get());
            }
        }
    }
    fogNodes.clear();
}

void BattleSceneSync::SetFogDrift(const Quasi::TurbulenceField* field,
                                  float strength) {
    fogDrift = field;
    fogDriftStrength = strength;
}

void BattleSceneSync::SetFogDriftGust(const Quasi::GustField* gust) {
    fogGust = gust;
}

void BattleSceneSync::SetSeepageFX(const Vector3& tint, float jitterAmp,
                                   float shadowOffset) {
    seepageTint = tint;
    seepageJitter = jitterAmp;
    seepageShadow = shadowOffset;
}

void BattleSceneSync::SetFogMarkerMesh(SharedPtr<Mesh> mesh) {
    fogMarkerMesh = std::move(mesh);
    for (auto& kv : fogNodes) {
        for (auto& c : kv.second->GetChildren()) {
            if (auto* rc = c->GetRenderable()) rc->mesh = fogMarkerMesh;
        }
    }
}

SceneNode* BattleSceneSync::EnsureFogNode(int entityId) {
    auto it = fogNodes.find(entityId);
    if (it != fogNodes.end()) return it->second.get();
    if (!fog || !parentNode) return nullptr;
    const UncertainEntity* e = fog->GetEntity(entityId);
    if (!e) return nullptr;

    auto node = MakeShared<SceneNode>("__fog_e" + std::to_string(entityId));
    for (size_t i = 0; i < e->candidates.size(); ++i) {
        auto c = MakeShared<SceneNode>("__fog_c" + std::to_string(i));
        auto rc = MakeShared<RenderableComponent>();
        rc->mesh = fogMarkerMesh;
        rc->color = Vector3(0.55f, 0.45f, 0.85f); // 敵情雲:紫灰
        rc->boundingRadius = cellSize * 1.5f;
        c->SetRenderable(rc);
        node->AddChild(c);
    }
    parentNode->AddChild(node);
    fogNodes[entityId] = node;
    return node.get();
}

void BattleSceneSync::SetSelectedSquad(const Squad* squad) {
    selected = squad;
}

SceneNode* BattleSceneSync::GetNodeFor(const Squad* squad) const {
    for (auto& b : bindings) {
        if (b.squad == squad) return b.node.get();
    }
    return nullptr;
}

void BattleSceneSync::Detach() {
    if (parentNode && parentNode->GetParent()) {
        parentNode->GetParent()->RemoveChild(parentNode.get());
    }
    parentNode.reset();
    bindings.clear();
    fogNodes.clear();   // 標記節點隨 parentNode 子樹一起釋放
    fog = nullptr;      // 外層持有的 fog 不隨綁定續命
    fogMarkerMesh.reset();
    selected = nullptr; // 綁定已清,選取指標不得留給下一場 battle
    scene = nullptr;
}

} // namespace Gameplay
} // namespace Potato
