#pragma once

#include "MathUtils/Vector2.h"
#include "MathUtils/Vector3.h"

namespace Potato {

class Camera;

namespace Gameplay {

class BattleController;
class Squad;

/**
 * 螢幕→世界座標的 RTS 點選層
 *
 * 把滑鼠像素座標經 inverse view-projection 轉成世界射線，
 * 對戰場做兩種查詢：
 *   - PickSquad：射線對小隊包圍球取最近命中（選取單位）
 *   - IntersectGround：射線與地面 y=0 求交（下達移動/攻進指令的落點）
 *
 * 小隊的 2D 格子座標經 cellSize 映到世界 XZ 平面（y=0），
 * 與 BattleSceneSync 的 SetLocalPosition(p.x*cell, 0, p.y*cell) 一致。
 */
struct PickRay {
    Vector3 origin;
    Vector3 direction;  // 已正規化
};

class BattlePicker {
public:
    // 像素座標 → 世界射線。視口取自 camera.GetViewport()。
    // perspective 與 ortho 都支援。
    static PickRay ScreenToWorldRay(const Camera& camera,
                                    float screenX, float screenY);

    // 射線與水平面 y=planeY 求交；命中回傳 true 並輸出世界交點
    static bool IntersectGround(const PickRay& ray, float planeY,
                                Vector3& hit);

    // 世界座標 → 戰場格子座標（除以 cellSize）
    static Vector2 WorldToCell(const Vector3& world, float cellSize);

    // 射線對指定隊伍所有小隊的包圍球取最近命中。
    // 小隊世界位置 = (pos.x*cellSize, 0, pos.y*cellSize)，
    // 半徑 = 0.6 * cellSize * radiusScale（與 BattleSceneSync 一致）。
    // 未命中回傳 nullptr。
    static Squad* PickSquad(BattleController& battle, int team,
                            const PickRay& ray, float cellSize,
                            float radiusScale = 1.0f);
};

} // namespace Gameplay
} // namespace Potato
