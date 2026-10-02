#include "RefitCamp.h"
#include "BattleController.h"
#include "PostBattle.h"
#include "Roster.h"
#include "Serialization/JsonParser.h"
#include "Serialization/JsonValidation.h"
#include "Serialization/JsonWriter.h"
#include "SquadTemplate.h"
#include <set>

#include <fstream>
#include <sstream>

namespace Potato {
namespace Gameplay {

const VeteranUnit *RefitCamp::FindUnit(const std::string &squadName) const {
    for (const auto &u : units) {
        if (u.squadName == squadName) {
            return &u;
        }
    }
    return nullptr;
}

void RefitCamp::Absorb(const PostBattleReport &report, const Roster &roster, int team) {
    (void)roster; // 隊長生死由 roster 記錄；此處只管編制
    std::vector<VeteranUnit> recovering;
    for (const auto &c : report.casualties) {
        if (c.team != team) {
            continue;
        }
        for (size_t i = 0; i < units.size(); ++i) {
            if (units[i].squadName != c.squadName) {
                continue;
            }
            if (c.eliminated) {
                // 先前在營的傷兵沒有出戰，轉交新軍官；不復活陣亡隊長。
                if (units[i].wounded > 0) {
                    // 複製兵種、模板及紀念遺物，再換新接收軍官；只剩在營傷兵，
                    // 沒有免費恢復這次全滅的部署兵力，也沒有復活原隊長。
                    VeteranUnit medical = units[i];
                    medical.members = 0;
                    medical.squadName = "傷員接收 · " + units[i].squadName;
                    medical.captainName = "接收軍官 · " + units[i].captainName;
                    medical.wounded = medical.maxMembers = units[i].wounded;
                    recovering.push_back(std::move(medical));
                }
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
    for (const auto &medical : recovering)
        units.push_back(medical);
    for (const auto &r : report.relics) {
        inventory.push_back(r);
    }
}

int RefitCamp::HealWounded(int maxSpend) {
    int budget = maxSpend < loot ? maxSpend : loot;
    int healed = 0;
    for (auto &u : units) {
        while (u.wounded > 0 && budget > 0 && u.members < u.maxMembers) {
            --u.wounded;
            ++u.members;
            --budget;
            ++healed;
        }
    }
    loot -= healed;
    return healed;
}

bool RefitCamp::Recruit(const SquadTemplateLibrary &library, const std::string &templateId) {
    const SquadTemplate *tpl = library.Find(templateId);
    if (!tpl || tpl->cost > loot || recruitSerial >= 900000) {
        return false;
    }
    loot -= tpl->cost;
    VeteranUnit u;
    u.squadName = tpl->name;
    // 序號包含過去已陣亡的招募隊；只找目前 units 是否撞名不足以維持身分。
    ++recruitSerial;
    // 序號持久化，已陣亡的補充隊伍身分永不重用。
    if (recruitSerial > 1)
        u.squadName = tpl->name + " · " + std::to_string(recruitSerial);
    while (FindUnit(u.squadName)) {
        ++recruitSerial;
        u.squadName = tpl->name + " · " + std::to_string(recruitSerial);
    }
    u.captainName = "補充軍官 · " + std::to_string(recruitSerial);
    u.templateId = tpl->id;
    u.unitClass = tpl->unitClass;
    u.members = tpl->members;
    u.wounded = 0;
    u.maxMembers = tpl->members;
    units.push_back(u);
    return true;
}

bool RefitCamp::AssignRelic(const std::string &squadName, const std::string &relic) {
    for (size_t i = 0; i < inventory.size(); ++i) {
        if (inventory[i] == relic) {
            for (auto &u : units) {
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

std::vector<Squad *> RefitCamp::Deploy(BattleController &battle, int team,
                                       const std::vector<Vector2> &positions,
                                       const SquadTemplateLibrary *library) {
    std::vector<Squad *> out;
    for (size_t i = 0; i < units.size(); ++i) {
        // 傷員接收隊可合法只有 wounded，醫治前不能生成零兵力戰場小隊。
        if (units[i].members <= 0)
            continue;
        Vector2 pos(0.0f, 0.0f);
        if (!positions.empty()) {
            size_t pi = i < positions.size() ? i : positions.size() - 1;
            pos = positions[pi];
            if (i >= positions.size()) {
                pos.y += static_cast<float>(i - positions.size() + 1);
            }
        }
        Squad *s = battle.CreateSquad(units[i].squadName, team, pos, units[i].members);
        if (s) {
            // 招募單位回補模板 stats（G-6）：speed/engage_range/
            // damage/stamina 全由模板 stamped；查無模板就只給預設
            if (library && !units[i].templateId.empty()) {
                if (const SquadTemplate *tpl = library->Find(units[i].templateId)) {
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

static std::string JsonEscape(const std::string &s) {
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

bool RefitCamp::SaveToFile(const std::string &path) const {
    std::ofstream f(path);
    if (!f)
        return false;
    f << JsonSerialization::WriteJson(ToJson());
    return f.good();
}

JsonValue RefitCamp::ToJson() const {
    JsonValue o;
    o.type = JsonValue::Type::Object;
    o.objectValue["schema"] = JsonValue::String("potato.refit_camp/1");
    o.objectValue["loot"] = JsonValue::Number(loot);
    o.objectValue["recruit_serial"] = JsonValue::Number(recruitSerial);
    JsonValue inv;
    inv.type = JsonValue::Type::Array;
    for (const auto &r : inventory) {
        inv.arrayValue.push_back(JsonValue::String(r));
    }
    o.objectValue["inventory"] = inv;
    JsonValue us;
    us.type = JsonValue::Type::Array;
    for (const auto &u : units) {
        JsonValue ju;
        ju.type = JsonValue::Type::Object;
        ju.objectValue["squad_name"] = JsonValue::String(u.squadName);
        ju.objectValue["template_id"] = JsonValue::String(u.templateId);
        ju.objectValue["unit_class"] = JsonValue::String(UnitClassName(u.unitClass));
        ju.objectValue["members"] = JsonValue::Number(u.members);
        ju.objectValue["wounded"] = JsonValue::Number(u.wounded);
        ju.objectValue["max_members"] = JsonValue::Number(u.maxMembers);
        ju.objectValue["captain"] = JsonValue::String(u.captainName);
        JsonValue rs;
        rs.type = JsonValue::Type::Array;
        for (const auto &r : u.relics) {
            rs.arrayValue.push_back(JsonValue::String(r));
        }
        ju.objectValue["relics"] = rs;
        us.arrayValue.push_back(ju);
    }
    o.objectValue["units"] = us;
    return o;
}

bool RefitCamp::FromJson(const JsonValue &root) {
    if (!root.IsObject() || root["schema"].AsString() != "potato.refit_camp/1" ||
        !JsonValidation::Integer(root["loot"]) || !JsonValidation::Strings(root["inventory"]) ||
        !root["units"].IsArray())
        return false;
    if (!root["recruit_serial"].IsNull() &&
        !JsonValidation::Integer(root["recruit_serial"], 0, 900000))
        return false;
    int nextSerial = root["recruit_serial"].AsInt();
    std::vector<VeteranUnit> next;
    std::set<std::string> names;
    for (const auto &u : root["units"].AsArray()) {
        for (auto key : {"squad_name", "template_id", "unit_class", "captain"})
            if (!u[key].IsString())
                return false;
        if (u["squad_name"].stringValue.empty() ||
            !names.insert(u["squad_name"].stringValue).second ||
            !JsonValidation::Integer(u["members"], 0, 10000) ||
            !JsonValidation::Integer(u["wounded"], 0, 10000) ||
            !JsonValidation::Integer(u["max_members"], 1, 10000) ||
            u["members"].AsInt() + u["wounded"].AsInt() > u["max_members"].AsInt() ||
            !JsonValidation::Strings(u["relics"]))
            return false;
        const auto cls = u["unit_class"].stringValue;
        if (cls != "infantry" && cls != "cavalry" && cls != "archer")
            return false;
        VeteranUnit v;
        v.squadName = u["squad_name"].stringValue;
        v.templateId = u["template_id"].stringValue;
        v.unitClass = UnitClassFromString(cls);
        v.members = u["members"].AsInt();
        v.wounded = u["wounded"].AsInt();
        v.maxMembers = u["max_members"].AsInt();
        v.captainName = u["captain"].stringValue;
        for (const auto &r : u["relics"].AsArray())
            v.relics.push_back(r.stringValue);
        next.push_back(v);
    }
    recruitSerial = nextSerial;
    loot = root["loot"].AsInt();
    units = std::move(next);
    inventory.clear();
    for (const auto &r : root["inventory"].AsArray())
        inventory.push_back(r.stringValue);
    return true;
}

bool RefitCamp::LoadFromFile(const std::string &path) {
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
