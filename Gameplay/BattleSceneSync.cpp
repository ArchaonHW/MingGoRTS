#include "BattleSceneSync.h"
#include "BattleController.h"
#include "Squad.h"

#include "Scene/SceneNode.h"
#include "Rendering/RenderableComponent.h"

#include <algorithm>
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
    selected = nullptr; // 綁定已清,選取指標不得留給下一場 battle
    scene = nullptr;
}

} // namespace Gameplay
} // namespace Potato
