#include "BattleMap.h"
#include "FlowField.h"
#include "Serialization/JsonParser.h"

#include <fstream>
#include <sstream>

namespace Potato {
namespace Gameplay {

bool MapRect::Contains(const Vector2& worldPos, float cell) const {
    float cx = worldPos.x / cell;
    float cy = worldPos.y / cell;
    return cx >= x && cx < x + w && cy >= y && cy < y + h;
}

static MapRect ParseRect(const JsonValue& v) {
    MapRect r;
    const auto& a = v.AsArray();
    if (a.size() >= 4) {
        r.x = a[0].AsInt();
        r.y = a[1].AsInt();
        r.w = a[2].AsInt();
        r.h = a[3].AsInt();
    }
    return r;
}

static Vector2 ParsePos(const JsonValue& v) {
    const auto& a = v.AsArray();
    if (a.size() >= 2) {
        return Vector2(a[0].AsFloat(), a[1].AsFloat());
    }
    return Vector2(0.0f, 0.0f);
}

bool BattleMap::LoadFromFile(const std::string& path) {
    // 容錯：逐層往上找（CTest 工作目錄可能深至 build/bin/Release）
    for (const char* prefix : {"", "../", "../../", "../../../"}) {
        std::ifstream f(std::string(prefix) + path);
        if (f) {
            std::ostringstream ss;
            ss << f.rdbuf();
            return LoadFromString(ss.str());
        }
    }
    return false;
}

bool BattleMap::LoadFromString(const std::string& json) {
    JsonValue root;
    if (!JsonValue::ParseOk(json, root) || !root.IsObject()) {
        return false;
    }

    name = root["name"].AsString("unnamed");
    const JsonValue& g = root["grid"];
    gridW = g["width"].AsInt();
    gridH = g["height"].AsInt();
    cellSize = g["cellSize"].AsFloat(1.0f);
    if (gridW <= 0 || gridH <= 0 || cellSize <= 0.0f) {
        return false;
    }

    for (const auto& t : root["terrain"].AsArray()) {
        TerrainPatch p;
        p.rect = ParseRect(t["rect"]);
        p.blocked = t["blocked"].AsBool(false);
        p.cost = t["cost"].AsFloat(1.0f);
        p.note = t["note"].AsString();
        terrain.push_back(p);
    }
    for (const auto& z : root["zones"].AsArray()) {
        zones.push_back({z["name"].AsString(), ParseRect(z["rect"])});
    }
    for (const auto& z : root["fords"].AsArray()) {
        fords.push_back({z["name"].AsString(), ParseRect(z["rect"])});
    }
    const JsonValue& dz = root["deployZones"];
    for (int t = 0; t <= 1; ++t) {
        const JsonValue& d = dz[std::to_string(t)];
        if (d.IsObject()) {
            deployZones[t] = ParseRect(d["rect"]);
            hasDeploy[t] = true;
        }
    }
    for (const auto& p : root["pins"].AsArray()) {
        pins.push_back({p["name"].AsString(), ParsePos(p["pos"]),
                        p["note"].AsString()});
    }
    for (const auto& o : root["interactables"].AsArray()) {
        MapInteractable it;
        it.type = o["type"].AsString();
        it.pos = ParsePos(o["pos"]);
        it.radius = o["radius"].AsFloat(1.0f);
        it.note = o["note"].AsString();
        interactables.push_back(it);
    }
    return true;
}

void BattleMap::ApplyToField(FlowField& field) const {
    for (const auto& p : terrain) {
        for (int y = p.rect.y; y < p.rect.y + p.rect.h; ++y) {
            for (int x = p.rect.x; x < p.rect.x + p.rect.w; ++x) {
                if (p.blocked) {
                    field.SetObstacle(x, y, true);
                } else {
                    field.SetCost(x, y, p.cost);
                }
            }
        }
    }
    // 渡口：把 blocked 水域中屬於 ford 的格子解除阻擋
    for (const auto& f : fords) {
        for (int y = f.rect.y; y < f.rect.y + f.rect.h; ++y) {
            for (int x = f.rect.x; x < f.rect.x + f.rect.w; ++x) {
                field.SetObstacle(x, y, false);
            }
        }
    }
}

bool BattleMap::IsInDeployZone(int team, const Vector2& pos) const {
    if (team < 0 || team > 1 || !hasDeploy[team]) {
        return true; // 未定義部署區 = 不限制
    }
    return deployZones[team].Contains(pos, cellSize);
}

const MapPin* BattleMap::FindPin(const std::string& pinName) const {
    for (const auto& p : pins) {
        if (p.name == pinName) {
            return &p;
        }
    }
    return nullptr;
}

const MapZone* BattleMap::ZoneAt(const Vector2& pos) const {
    for (const auto& z : zones) {
        if (z.rect.Contains(pos, cellSize)) {
            return &z;
        }
    }
    for (const auto& f : fords) {
        if (f.rect.Contains(pos, cellSize)) {
            return &f;
        }
    }
    return nullptr;
}

} // namespace Gameplay
} // namespace Potato
