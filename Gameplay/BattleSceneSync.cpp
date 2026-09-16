#include "BattleSceneSync.h"
#include "BattleController.h"
#include "Squad.h"

#include "Scene/SceneNode.h"
#include "Rendering/RenderableComponent.h"

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
            continue;
        }

        const Vector2& p = squad->GetPosition();
        b.node->SetLocalPosition(Vector3(p.x * cellSize, 0.0f, p.y * cellSize));

        // 士氣映射亮度:潰逃半暗,滿士氣全亮
        float lum = squad->IsRouting() ? 0.35f
                                       : 0.55f + 0.45f * squad->GetMorale();
        Vector3 base = TeamColor(squad->GetTeam());
        b.renderable->color = base * lum;
    }
}

void BattleSceneSync::SetUnitMesh(SharedPtr<Mesh> mesh) {
    unitMesh = std::move(mesh);
    for (auto& b : bindings) {
        b.renderable->mesh = unitMesh;
    }
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
    scene = nullptr;
}

} // namespace Gameplay
} // namespace Potato
