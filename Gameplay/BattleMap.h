#pragma once

#include "Core/CoreTypes.h"
#include "MathUtils/Vector2.h"
#include "FlowField.h"

#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

/**
 * 戰鬥地圖（BattleMap）——T-4 斷橋地圖 JSON
 *
 * 從 JSON 載入戰場定義：grid 尺寸、區域（zone）、渡口（ford）、
 * 部署區（deployZone）、圖釘點（pin）、互動物件（interactable）、
 * 地形阻擋與成本。ApplyToField 把地形灌進 FlowField。
 *
 * JSON schema: potato.battle_map/1
 *   grid:        {width, height, cellSize}
 *   terrain:     [{rect:[x,y,w,h], blocked, cost, note, type?}]
 *                // type: "highland" | "forest" | "mud"，缺省 = plain
 *   zones:       [{name, rect:[x,y,w,h]}]
 *   fords:       [{name, rect:[x,y,w,h]}]          // 可通行的水域缺口
 *   deployZones: {"0": {rect:[...]}, "1": {...}}    // 各隊部署範圍
 *   pins:        [{name, pos:[x,y], note}]
 *   interactables: [{type, pos:[x,y], radius, note}]
 */
struct MapRect {
    int x = 0, y = 0, w = 0, h = 0;
    bool Contains(const Vector2& worldPos, float cellSize) const;
};

struct MapZone {
    std::string name;
    MapRect rect;
};

struct MapPin {
    std::string name;
    Vector2 pos;
    std::string note;
};

struct MapInteractable {
    std::string type;    // oil_slick / rockfall / supply_cache ...
    Vector2 pos;
    float radius = 1.0f;
    std::string note;
};

class BattleMap {
public:
    bool LoadFromFile(const std::string& path);
    bool LoadFromString(const std::string& json);

    const std::string& GetName() const { return name; }
    int GetGridWidth() const { return gridW; }
    int GetGridHeight() const { return gridH; }
    float GetCellSize() const { return cellSize; }

    // 把地形阻擋/成本寫進 flow field（部署前呼叫一次）
    void ApplyToField(FlowField& field) const;

    // 部署合法性：pos 是否在該隊部署區內
    bool IsInDeployZone(int team, const Vector2& pos) const;

    const std::vector<MapZone>& GetZones() const { return zones; }
    const std::vector<MapZone>& GetFords() const { return fords; }
    const std::vector<MapPin>& GetPins() const { return pins; }
    const std::vector<MapInteractable>& GetInteractables() const {
        return interactables;
    }
    const MapPin* FindPin(const std::string& pinName) const;

    // pos 落在哪個命名區域（zone 或 ford），沒有回 nullptr
    const MapZone* ZoneAt(const Vector2& pos) const;

private:
    std::string name;
    int gridW = 0, gridH = 0;
    float cellSize = 1.0f;

    struct TerrainPatch {
        MapRect rect;
        bool blocked = false;
        float cost = 1.0f;
        TerrainType type = TerrainType::Plain;
        std::string note;
    };
    std::vector<TerrainPatch> terrain;
    std::vector<MapZone> zones;
    std::vector<MapZone> fords;
    MapRect deployZones[2];
    bool hasDeploy[2] = {false, false};
    std::vector<MapPin> pins;
    std::vector<MapInteractable> interactables;
};

} // namespace Gameplay
} // namespace Potato
