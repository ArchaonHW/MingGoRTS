#include "BattlePicker.h"

#include "BattleController.h"
#include "Squad.h"
#include "Rendering/Camera.h"
#include "Physics/CollisionDetection.h"

#include <cmath>

namespace Potato {
namespace Gameplay {

PickRay BattlePicker::ScreenToWorldRay(const Camera& camera,
                                       float screenX, float screenY) {
    int vx = 0, vy = 0, vw = 1, vh = 1;
    camera.GetViewport(vx, vy, vw, vh);

    // 像素 → NDC（y 翻轉：螢幕原點在左上，NDC 原點在中心）
    const float ndcX = ((screenX - vx) / static_cast<float>(vw)) * 2.0f - 1.0f;
    const float ndcY = 1.0f - ((screenY - vy) / static_cast<float>(vh)) * 2.0f;

    const Matrix4 invVP = camera.GetViewProjectionMatrix().Inverse();

    // 近平面點（z=-1）與遠平面點（z=+1）反投影
    auto unproject = [&](float ndcZ) {
        Vector4 p = invVP * Vector4(ndcX, ndcY, ndcZ, 1.0f);
        if (std::fabs(p.w) > 1e-8f) {
            p = p * (1.0f / p.w);
        }
        return Vector3(p.x, p.y, p.z);
    };

    PickRay ray;
    const Vector3 nearPt = unproject(-1.0f);
    const Vector3 farPt = unproject(1.0f);
    ray.origin = nearPt;
    ray.direction = (farPt - nearPt).Normalized();
    return ray;
}

bool BattlePicker::IntersectGround(const PickRay& ray, float planeY,
                                   Vector3& hit) {
    if (std::fabs(ray.direction.y) < 1e-8f) {
        return false;  // 射線與地面平行
    }
    const float t = (planeY - ray.origin.y) / ray.direction.y;
    if (t < 0.0f) {
        return false;  // 交點在射線反方向
    }
    hit = ray.origin + ray.direction * t;
    return true;
}

Vector2 BattlePicker::WorldToCell(const Vector3& world, float cellSize) {
    if (cellSize <= 0.0f) return Vector2(0.0f, 0.0f);
    return Vector2(world.x / cellSize, world.z / cellSize);
}

Squad* BattlePicker::PickSquad(BattleController& battle, int team,
                               const PickRay& ray, float cellSize,
                               float radiusScale) {
    Squad* closest = nullptr;
    float closestT = 1e30f;
    const float radius = 0.6f * cellSize * radiusScale;

    for (const auto& squadPtr : battle.GetSquads()) {
        Squad* squad = squadPtr.get();
        if (!squad || squad->GetTeam() != team || squad->GetMembers() <= 0 ||
            squad->IsRouting()) {
            continue;
        }
        const Vector2& p = squad->GetPosition();
        const Vector3 center(p.x * cellSize, 0.5f * cellSize, p.y * cellSize);

        float t = 0.0f;
        Vector3 n;
        if (RayVsSphere(ray.origin, ray.direction, center, radius,
                        closestT, t, n)) {
            closest = squad;
            closestT = t;
        }
    }
    return closest;
}

} // namespace Gameplay
} // namespace Potato
