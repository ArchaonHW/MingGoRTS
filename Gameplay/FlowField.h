#pragma once

#include "Core/CoreTypes.h"
#include "MathUtils/Vector2.h"

#include <vector>

namespace Potato {
namespace Gameplay {

/**
 * Flow Field（流向場）尋路
 *
 * 以小隊（squad）為最小單位的群體尋路：從目標格做反向 Dijkstra
 * 產生 integration cost，再對每格求「往最低 cost 鄰居」的方向。
 * 所有小隊共用同一張場，查詢 O(1)，適合 squad-level 移動。
 */
class FlowField {
public:
    FlowField(int width, int height, float cellSize);

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    float GetCellSize() const { return cellSize; }

    // 障礙與地形成本（cost >= 1.0，越大越難走）
    void SetObstacle(int x, int y, bool blocked);
    void SetCost(int x, int y, float cost);
    bool IsBlocked(int x, int y) const;

    // 從目標世界座標重算整張場；回傳 false 表示目標不可達/不合法
    bool Compute(const Vector2& goalWorld);

    // 目前場的目標（Compute 設定）；HasGoal 為 false 時場無效
    const Vector2& GetGoal() const { return goal; }
    bool HasGoal() const { return hasGoal; }

    // 查詢世界座標處該往哪走；不在場內或無路徑時回傳零向量
    Vector2 GetDirection(const Vector2& worldPos) const;

    // 到達目標的剩餘 cost（粗略路徑長度）；無路徑回傳 -1
    float GetIntegrationCost(const Vector2& worldPos) const;

    bool WorldToCell(const Vector2& worldPos, int& outX, int& outY) const;
    Vector2 CellToWorld(int x, int y) const;

private:
    int width;
    int height;
    float cellSize;

    Vector2 goal;
    bool hasGoal = false;

    std::vector<uint8> blocked;
    std::vector<float> terrainCost;
    std::vector<float> integration;
    std::vector<Vector2> direction;

    int Index(int x, int y) const { return y * width + x; }
    bool InBounds(int x, int y) const {
        return x >= 0 && y >= 0 && x < width && y < height;
    }
};

} // namespace Gameplay
} // namespace Potato
