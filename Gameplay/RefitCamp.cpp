#include "RefitCamp.h"
#include "BattleController.h"
#include "PostBattle.h"
#include "Roster.h"
#include "SquadTemplate.h"
#include "Serialization/JsonParser.h"

#include <fstream>
#include <sstream>

namespace Potato {
namespace Gameplay {

const VeteranUnit* RefitCamp::FindUnit(const std::string& squadName) const {
    for (const auto& u : units) {
        if (u.squadName == squadName) {
            return &u;
        }
    }
    return nullptr;
}

void RefitCamp::Absorb(const PostBattleReport& report,
                       const Roster& roster, int team) {
    (void)roster; // 隊長生死由 roster 記錄；此處只管編制
    for (const auto& c : report.casualties) {
        if (c.team != team) {
            continue;
        }
        for (size_t i = 0; i < units.size(); ++i) {
            if (units[i].squadName != c.squadName) {
                continue;
            }
            if (c.eliminated) {
                // 全滅 → 移出常備軍（傷兵也救不回來）
                units.erase(units.begin() + i);
            } else {
                // lost 從部署兵力起算（squad.maxMembers=部署值），
                // 只能從營記兵力扣——殘編老兵不能用編制上限回推
                units[i].members -= c.lost;
                if (units[i].members < 0) {
                    units[i].members = 0;
                }
                units[i].wounded += c.wounded;
            }
            break;
        }
    }
    for (const auto& r : report.relics) {
        inventory.push_back(r);
    }
}

int RefitCamp::HealWounded(int maxSpend) {
    int budget = maxSpend < loot ? maxSpend : loot;
    int healed = 0;
    for (auto& u : units) {
        while (u.wounded > 0 && budget > 0 &&
               u.members < u.maxMembers) {
            --u.wounded;
            ++u.members;
            --budget;
            ++healed;
        }
    }
    loot -= healed;
    return healed;
}

bool RefitCamp::Recruit(const SquadTemplateLibrary& library,
                        const std::string& templateId) {
    const SquadTemplate* tpl = library.Find(templateId);
    if (!tpl || tpl->cost > loot) {
        return false;
    }
    loot -= tpl->cost;
    VeteranUnit u;
    u.squadName = tpl->name;
    u.templateId = tpl->id;
    u.unitClass = tpl->unitClass;
    u.members = tpl->members;
    u.wounded = 0;
    u.maxMembers = tpl->members;
    units.push_back(u);
    return true;
}

bool RefitCamp::AssignRelic(const std::string& squadName,
                            const std::string& relic) {
    for (size_t i = 0; i < inventory.size(); ++i) {
        if (inventory[i] == relic) {
            for (auto& u : units) {
                if (u.squadName == squadName) {
                    u.relics.push_back(relic);
                    inventory.erase(inventory.begin() + i);
                    return true;
                }
            }
            return false; // 庫存有但查無單位
        }
    }
    return false;
}

std::vector<Squad*> RefitCamp::Deploy(
    BattleController& battle, int team,
    const std::vector<Vector2>& positions,
    const SquadTemplateLibrary* library) {
    std::vector<Squad*> out;
    for (size_t i = 0; i < units.size(); ++i) {
        Vector2 pos(0.0f, 0.0f);
        if (!positions.empty()) {
            size_t pi = i < positions.size() ? i : positions.size() - 1;
            pos = positions[pi];
            if (i >= positions.size()) {
                pos.y += static_cast<float>(i - positions.size() + 1);
            }
        }
        Squad* s = battle.CreateSquad(units[i].squadName, team, pos,
                                      units[i].members);
        if (s) {
            // 招募單位回補模板 stats（G-6）：speed/engage_range/
            // damage/stamina 全由模板 stamped；查無模板就只給預設
            if (library && !units[i].templateId.empty()) {
                if (const SquadTemplate* tpl =
                        library->Find(units[i].templateId)) {
                    tpl->ApplyStats(s);
                }
            }
            // unitClass 以單位自身記錄為準（壓在模板值之後）
            s->SetUnitClass(units[i].unitClass);
            out.push_back(s);
        }
    }
    return out;
}

static std::string JsonEscape(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 2);
    for (char c : s) {
        if (c == '"' || c == '\\') {
            out += '\\';
        }
        out += c;
    }
    return out;
}

bool RefitCamp::SaveToFile(const std::string& path) const {
    std::ostringstream ss;
    ss << "{\"schema\":\"potato.refit_camp/1\",\"loot\":" << loot
       << ",\"inventory\":[";
    for (size_t i = 0; i < inventory.size(); ++i) {
        ss << "\"" << JsonEscape(inventory[i]) << "\""
           << (i + 1 < inventory.size() ? "," : "");
    }
    ss << "],\"units\":[";
    for (size_t i = 0; i < units.size(); ++i) {
        const auto& u = units[i];
        ss << "{\"squad_name\":\"" << JsonEscape(u.squadName)
           << "\",\"template_id\":\"" << JsonEscape(u.templateId)
           << "\",\"unit_class\":\"" << UnitClassName(u.unitClass)
           << "\",\"members\":" << u.members
           << ",\"wounded\":" << u.wounded
           << ",\"max_members\":" << u.maxMembers
           << ",\"captain\":\"" << JsonEscape(u.captainName)
           << "\",\"relics\":[";
        for (size_t j = 0; j < u.relics.size(); ++j) {
            ss << "\"" << JsonEscape(u.relics[j]) << "\""
               << (j + 1 < u.relics.size() ? "," : "");
        }
        ss << "]}" << (i + 1 < units.size() ? "," : "");
    }
    ss << "]}";
    std::ofstream f(path);
    if (!f) {
        return false;
    }
    f << ss.str();
    return true;
}

JsonValue RefitCamp::ToJson() const {
    JsonValue o;
    o.type = JsonValue::Type::Object;
    o.objectValue["schema"] = JsonValue::String("potato.refit_camp/1");
    o.objectValue["loot"] = JsonValue::Number(loot);
    JsonValue inv;
    inv.type = JsonValue::Type::Array;
    for (const auto& r : inventory) {
        inv.arrayValue.push_back(JsonValue::String(r));
    }
    o.objectValue["inventory"] = inv;
    JsonValue us;
    us.type = JsonValue::Type::Array;
    for (const auto& u : units) {
        JsonValue ju;
        ju.type = JsonValue::Type::Object;
        ju.objectValue["squad_name"] = JsonValue::String(u.squadName);
        ju.objectValue["template_id"] = JsonValue::String(u.templateId);
        ju.objectValue["unit_class"] =
            JsonValue::String(UnitClassName(u.unitClass));
        ju.objectValue["members"] = JsonValue::Number(u.members);
        ju.objectValue["wounded"] = JsonValue::Number(u.wounded);
        ju.objectValue["max_members"] = JsonValue::Number(u.maxMembers);
        ju.objectValue["captain"] = JsonValue::String(u.captainName);
        JsonValue rs;
        rs.type = JsonValue::Type::Array;
        for (const auto& r : u.relics) {
            rs.arrayValue.push_back(JsonValue::String(r));
        }
        ju.objectValue["relics"] = rs;
        us.arrayValue.push_back(ju);
    }
    o.objectValue["units"] = us;
    return o;
}

bool RefitCamp::FromJson(const JsonValue& root) {
    if (root["schema"].AsString() != "potato.refit_camp/1") {
        return false;
    }
    loot = root["loot"].AsInt(0);
    inventory.clear();
    for (const auto& r : root["inventory"].AsArray()) {
        inventory.push_back(r.AsString());
    }
    units.clear();
    for (const auto& u : root["units"].AsArray()) {
        VeteranUnit vu;
        vu.squadName = u["squad_name"].AsString();
        vu.templateId = u["template_id"].AsString();
        vu.unitClass = UnitClassFromString(u["unit_class"].AsString());
        vu.members = u["members"].AsInt(0);
        vu.wounded = u["wounded"].AsInt(0);
        vu.maxMembers = u["max_members"].AsInt(0);
        vu.captainName = u["captain"].AsString();
        for (const auto& r : u["relics"].AsArray()) {
            vu.relics.push_back(r.AsString());
        }
        units.push_back(vu);
    }
    return true;
}

bool RefitCamp::LoadFromFile(const std::string& path) {
    std::ifstream f(path);
    if (!f) {
        return false;
    }
    std::ostringstream buf;
    buf << f.rdbuf();
    JsonValue root;
    if (!JsonValue::ParseOk(buf.str(), root)) {
        return false;
    }
    return FromJson(root);
}

} // namespace Gameplay
} // namespace Potato
