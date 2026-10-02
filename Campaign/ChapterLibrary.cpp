#include "Campaign/ChapterLibrary.h"
#include "Gameplay/BattleMap.h"
#include "Gameplay/SquadTemplate.h"
#include "Serialization/JsonValidation.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <set>
#include <sstream>
namespace Potato::Campaign {
using namespace JsonValidation;
const ChapterDefinition *ChapterLibrary::Find(const std::string &id) const {
    for (const auto &c : chapters)
        if (c.id == id)
            return &c;
    return nullptr;
}
bool ChapterLibrary::LoadFromFile(const std::string &path, std::string &error) {
    std::ifstream f(path);
    std::ostringstream ss;
    ss << f.rdbuf();
    JsonValue root;
    auto fail = [&]() {
        error = "章節資產內容無效：" + path;
        return false;
    };
    if (!f || !JsonValue::ParseOk(ss.str(), root) ||
        root["schema"].AsString() != "potato.chapters/1" || !root["chapters"].IsArray() ||
        root["chapters"].Size() < 3)
        return fail();
    // 先建立整包候選，途中任何章節出錯都不替換 chapters，避免只載入半卷。
    std::vector<ChapterDefinition> candidate;
    std::set<std::string> ids;
    for (const auto &j : root["chapters"].arrayValue) {
        ChapterDefinition c;
        const char *keys[] = {
            "id",         "title",       "opening", "objectiveText",    "victoryText",
            "defeatText", "closingHook", "mapPath", "enemyGeneralName", "enemyPersonality"};
        for (auto key : keys)
            if (!j[key].IsString() || j[key].stringValue.empty())
                return fail();
        c.id = j["id"].stringValue;
        c.title = j["title"].stringValue;
        c.opening = j["opening"].stringValue;
        c.objectiveText = j["objectiveText"].stringValue;
        c.victoryText = j["victoryText"].stringValue;
        c.defeatText = j["defeatText"].stringValue;
        c.closingHook = j["closingHook"].stringValue;
        c.mapPath = j["mapPath"].stringValue;
        c.enemyGeneralName = j["enemyGeneralName"].stringValue;
        c.enemyPersonality = j["enemyPersonality"].stringValue;
        if (!ids.insert(c.id).second || !Integer(j["number"], 1, 100) ||
            j["number"].AsInt() != static_cast<int>(candidate.size() + 1) ||
            !Integer(j["arc"], 0, 3))
            return fail();
        c.number = j["number"].AsInt();
        c.arc = j["arc"].AsInt();
        if (c.enemyPersonality != "侵略" && c.enemyPersonality != "審慎" &&
            c.enemyPersonality != "狡詐")
            return fail();
        if (!Number(j["enemySpeed"], 0.1, 10) || !Number(j["enemyRange"], 0.1, 10) ||
            !Number(j["enemyDamage"], 0.001, 1))
            return fail();
        c.enemySpeed = j["enemySpeed"].AsFloat();
        c.enemyRange = j["enemyRange"].AsFloat();
        c.enemyDamage = j["enemyDamage"].AsFloat();
        if (!j["tint"].IsArray() || j["tint"].Size() != 3)
            return fail();
        for (int i = 0; i < 3; ++i) {
            if (!Number(j["tint"][i], 0, 1))
                return fail();
            c.tint[i] = j["tint"][i].AsFloat();
        }
        if (c.mapPath.rfind("assets/maps/", 0) != 0 || c.mapPath.find("..") != std::string::npos)
            return fail();
        // path 已限制在資產地圖目錄；相對章節檔找 assets，從 system32 啟動也可用。
        const auto mapFile =
            std::filesystem::path(path).parent_path().parent_path() / c.mapPath.substr(7);
        Gameplay::BattleMap map;
        if (!map.LoadFromFile(mapFile.string()))
            return fail();
        // 與實際戰場使用同一套地形灌入邏輯，檢查出生點不是阻擋格。
        Gameplay::FlowField field(map.GetGridWidth(), map.GetGridHeight(), map.GetCellSize());
        map.ApplyToField(field);
        auto pos = [&](const JsonValue &v, Vector2 &p) {
            if (!v.IsArray() || v.Size() != 2 ||
                !Number(v[0], 0, map.GetGridWidth() * map.GetCellSize() - 0.01) ||
                !Number(v[1], 0, map.GetGridHeight() * map.GetCellSize() - 0.01))
                return false;
            p = {v[0].AsFloat(), v[1].AsFloat()};
            return !field.IsBlocked(int(p.x / map.GetCellSize()), int(p.y / map.GetCellSize()));
        };
        if (!j["friendlyDeployment"].IsArray() || j["friendlyDeployment"].Size() < 5 ||
            !j["enemies"].IsArray() || j["enemies"].Size() != 4 || !j["choices"].IsArray() ||
            j["choices"].Size() < 2)
            return fail();
        for (const auto &v : j["friendlyDeployment"].arrayValue) {
            Vector2 p;
            if (!pos(v, p))
                return fail();
            c.friendlyDeployment.push_back(p);
        }
        std::set<std::string> names;
        for (const auto &e : j["enemies"].arrayValue) {
            EnemyDeployment d;
            if (!e["name"].IsString() || e["name"].stringValue.empty() ||
                !names.insert(e["name"].stringValue).second || !Integer(e["members"], 1, 200) ||
                !pos(e["position"], d.position))
                return fail();
            d.name = e["name"].stringValue;
            d.members = e["members"].AsInt();
            c.enemies.push_back(d);
        }
        std::set<std::string> choices;
        for (const auto &v : j["choices"].arrayValue) {
            ChapterChoice ch;
            if (!v["id"].IsString() || v["id"].stringValue.empty() ||
                !choices.insert(v["id"].stringValue).second || !v["label"].IsString() ||
                !v["description"].IsString() || !Integer(v["cost"], 0, 100) ||
                !Integer(v["supply"], 0, 100) || !Integer(v["reward"], 0, 100) ||
                !Number(v["holdSeconds"], 0, 300))
                return fail();
            ch.id = v["id"].stringValue;
            ch.label = v["label"].stringValue;
            ch.description = v["description"].stringValue;
            ch.cost = v["cost"].AsInt();
            ch.supply = v["supply"].AsInt();
            ch.reward = v["reward"].AsInt();
            ch.holdSeconds = v["holdSeconds"].AsFloat();
            c.choices.push_back(ch);
        }
        candidate.push_back(c);
    }
    // 模板 ID 同樣只在啟動時載入；續玩時拒絕不存在的模板，避免退回預設能力。
    Gameplay::SquadTemplateLibrary templates;
    const auto assets = std::filesystem::path(path).parent_path().parent_path();
    templates.LoadDir((assets / "squads").string());
    templates.LoadDir((assets / "templates").string());
    templateIds.clear();
    for (const auto *entry : templates.SortedByCost())
        templateIds.push_back(entry->id);
    chapters = std::move(candidate);
    error.clear();
    return true;
}
bool ChapterLibrary::IsKnownTemplate(const std::string &id) const {
    return id.empty() || std::find(templateIds.begin(), templateIds.end(), id) != templateIds.end();
}
} // namespace Potato::Campaign
