#include "FlowField.h"

#include <queue>
#include <limits>
#include <cmath>

namespace Potato {
namespace Gameplay {

FlowField::FlowField(int w, int h, float cell)
    : width(w)
    , height(h)
    , cellSize(cell)
    , blocked(static_cast<size_t>(w) * h, 0)
    , terrainCost(static_cast<size_t>(w) * h, 1.0f)
    , terrainType(static_cast<size_t>(w) * h,
                  static_cast<uint8>(TerrainType::Plain))
    , integration(static_cast<size_t>(w) * h,
                  std::numeric_limits<float>::infinity())
    , direction(static_cast<size_t>(w) * h, Vector2(0.0f, 0.0f)) {
}

void FlowField::SetObstacle(int x, int y, bool isBlocked) {
    if (InBounds(x, y)) {
        blocked[Index(x, y)] = isBlocked ? 1 : 0;
    }
}

void FlowField::SetCost(int x, int y, float cost) {
    if (InBounds(x, y) && cost >= 1.0f) {
        terrainCost[Index(x, y)] = cost;
    }
}

bool FlowField::IsBlocked(int x, int y) const {
    return InBounds(x, y) && blocked[Index(x, y)] != 0;
}

void FlowField::SetTerrain(int x, int y, TerrainType type) {
    if (InBounds(x, y)) {
        terrainType[Index(x, y)] = static_cast<uint8>(type);
    }
}

TerrainType FlowField::GetTerrain(int x, int y) const {
    if (!InBounds(x, y)) {
        return TerrainType::Plain;
    }
    return static_cast<TerrainType>(terrainType[Index(x, y)]);
}

TerrainType FlowField::TerrainAt(const Vector2& worldPos) const {
    int x = 0, y = 0;
    if (!WorldToCell(worldPos, x, y)) {
        return TerrainType::Plain;
    }
    return GetTerrain(x, y);
}

bool FlowField::WorldToCell(const Vector2& worldPos, int& outX, int& outY) const {
    outX = static_cast<int>(worldPos.x / cellSize);
    outY = static_cast<int>(worldPos.y / cellSize);
    return InBounds(outX, outY);
}

Vector2 FlowField::CellToWorld(int x, int y) const {
    return Vector2((x + 0.5f) * cellSize, (y + 0.5f) * cellSize);
}

bool FlowField::Compute(const Vector2& goalWorld) {
    int gx = 0, gy = 0;
    if (!WorldToCell(goalWorld, gx, gy) || IsBlocked(gx, gy)) {
        hasGoal = false;
        return false;
    }
    goal = goalWorld;
    hasGoal = true;

    const float inf = std::numeric_limits<float>::infinity();
    std::fill(integration.begin(), integration.end(), inf);
    std::fill(direction.begin(), direction.end(), Vector2(0.0f, 0.0f));

    // 反向 Dijkstra：從目標格往外擴散累積 cost
    using Node = std::pair<float, int>; // (cost, cellIndex)
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open;
    integration[Index(gx, gy)] = 0.0f;
    open.push({0.0f, Index(gx, gy)});

    static const int dirs[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    while (!open.empty()) {
        Node top = open.top();
        open.pop();
        float cost = top.first;
        int idx = top.second;
        if (cost > integration[idx]) {
            continue;
        }

        int cx = idx % width;
        int cy = idx / width;
        for (const auto& d : dirs) {
            int nx = cx + d[0];
            int ny = cy + d[1];
            if (!InBounds(nx, ny) || blocked[Index(nx, ny)]) {
                continue;
            }
            // 斜向不可穿越被夾住的角落
            if (d[0] != 0 && d[1] != 0 &&
                (blocked[Index(cx + d[0], cy)] || blocked[Index(cx, cy + d[1])])) {
                continue;
            }
            float step = (d[0] != 0 && d[1] != 0) ? 1.41421356f : 1.0f;
            float newCost = cost + step * terrainCost[Index(nx, ny)];
            int nIdx = Index(nx, ny);
            if (newCost < integration[nIdx]) {
                integration[nIdx] = newCost;
                open.push({newCost, nIdx});
            }
        }
    }

    // 每格方向 = 往 integration 最低的鄰居
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = Index(x, y);
            if (blocked[idx] || integration[idx] == inf) {
                continue;
            }
            float best = integration[idx];
            Vector2 bestDir(0.0f, 0.0f);
            for (const auto& d : dirs) {
                int nx = x + d[0];
                int ny = y + d[1];
                if (!InBounds(nx, ny)) {
                    continue;
                }
                int nIdx = Index(nx, ny);
                if (integration[nIdx] < best) {
                    best = integration[nIdx];
                    bestDir = Vector2(static_cast<float>(d[0]),
                                      static_cast<float>(d[1]));
                }
            }
            direction[idx] = bestDir.Normalize();
        }
    }

    return true;
}

Vector2 FlowField::GetDirection(const Vector2& worldPos) const {
    int x = 0, y = 0;
    if (!WorldToCell(worldPos, x, y)) {
        return Vector2(0.0f, 0.0f);
    }
    return direction[Index(x, y)];
}

float FlowField::GetIntegrationCost(const Vector2& worldPos) const {
    int x = 0, y = 0;
    if (!WorldToCell(worldPos, x, y)) {
        return -1.0f;
    }
    float c = integration[Index(x, y)];
    return (c == std::numeric_limits<float>::infinity()) ? -1.0f : c;
}

} // namespace Gameplay
} // namespace Potato
