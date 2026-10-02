#include "Campaign/CampaignState.h"
#include "Serialization/JsonValidation.h"
#include <set>
namespace Potato::Campaign {
using namespace JsonValidation;
JsonValue CampaignProgress::ToJson() const {
    JsonValue j;
    j.type = JsonValue::Type::Object;
    j.objectValue["schema"] = JsonValue::String("potato.campaign_progress/1");
    j.objectValue["initialized"] = JsonValue::Bool(initialized);
    j.objectValue["stage"] = JsonValue::String(stage == CampaignStage::Briefing    ? "briefing"
                                               : stage == CampaignStage::Aftermath ? "aftermath"
                                                                                   : "complete");
    JsonValue a;
    a.type = JsonValue::Type::Array;
    for (const auto &id : completed)
        a.arrayValue.push_back(JsonValue::String(id));
    j.objectValue["completed"] = a;
    for (auto key : {"choices", "outcomes"}) {
        JsonValue o;
        o.type = JsonValue::Type::Object;
        for (const auto &[id, v] : std::string(key) == "choices" ? choices : outcomes)
            o.objectValue[id] = JsonValue::String(v);
        j.objectValue[key] = o;
    }
    j.objectValue["cumulativeDead"] = JsonValue::Number(cumulativeDead);
    j.objectValue["lastPeaceful"] = JsonValue::Bool(lastPeaceful);
    JsonValue r;
    r.type = JsonValue::Type::Object;
    r.objectValue["winnerTeam"] = JsonValue::Number(lastReport.winnerTeam);
    r.objectValue["lootPoints"] = JsonValue::Number(lastReport.lootPoints);
    a.arrayValue.clear();
    for (const auto &c : lastReport.casualties) {
        JsonValue x;
        x.type = JsonValue::Type::Object;
        x.objectValue["squadName"] = JsonValue::String(c.squadName);
        x.objectValue["team"] = JsonValue::Number(c.team);
        x.objectValue["lost"] = JsonValue::Number(c.lost);
        x.objectValue["wounded"] = JsonValue::Number(c.wounded);
        x.objectValue["dead"] = JsonValue::Number(c.dead);
        x.objectValue["eliminated"] = JsonValue::Bool(c.eliminated);
        a.arrayValue.push_back(x);
    }
    r.objectValue["casualties"] = a;
    a.arrayValue.clear();
    for (const auto &v : lastReport.relics)
        a.arrayValue.push_back(JsonValue::String(v));
    r.objectValue["relics"] = a;
    j.objectValue["lastReport"] = r;
    return j;
}
bool CampaignProgress::FromJson(const JsonValue &j) {
    if (!j.IsObject() || j["schema"].AsString() != "potato.campaign_progress/1" ||
        !j["initialized"].IsBool() || !j["initialized"].boolValue || !j["stage"].IsString() ||
        !Strings(j["completed"]) || !j["choices"].IsObject() || !j["outcomes"].IsObject() ||
        !Integer(j["cumulativeDead"]) || !j["lastPeaceful"].IsBool())
        return false;
    CampaignProgress p;
    p.initialized = true;
    const auto st = j["stage"].stringValue;
    if (st == "briefing")
        p.stage = CampaignStage::Briefing;
    else if (st == "aftermath")
        p.stage = CampaignStage::Aftermath;
    else if (st == "complete")
        p.stage = CampaignStage::Complete;
    else
        return false;
    std::set<std::string> ids;
    for (const auto &v : j["completed"].arrayValue) {
        if (v.stringValue.empty() || !ids.insert(v.stringValue).second)
            return false;
        p.completed.push_back(v.stringValue);
    }
    for (auto key : {"choices", "outcomes"})
        for (const auto &[id, v] : j[key].objectValue) {
            if (id.empty() || !v.IsString() || v.stringValue.empty())
                return false;
            (std::string(key) == "choices" ? p.choices : p.outcomes)[id] = v.stringValue;
        }
    p.cumulativeDead = j["cumulativeDead"].AsInt();
    p.lastPeaceful = j["lastPeaceful"].boolValue;
    const auto &r = j["lastReport"];
    if (!r.IsObject() || !Integer(r["winnerTeam"], -1, 1) || !Integer(r["lootPoints"]) ||
        !r["casualties"].IsArray() || !Strings(r["relics"]))
        return false;
    p.lastReport.winnerTeam = r["winnerTeam"].AsInt();
    p.lastReport.lootPoints = r["lootPoints"].AsInt();
    for (const auto &x : r["casualties"].arrayValue) {
        if (!x["squadName"].IsString() || x["squadName"].stringValue.empty() ||
            !Integer(x["team"], 0, 1) || !Integer(x["lost"]) || !Integer(x["wounded"]) ||
            !Integer(x["dead"]) || x["lost"].AsInt() != x["wounded"].AsInt() + x["dead"].AsInt() ||
            !x["eliminated"].IsBool())
            return false;
        Gameplay::SquadCasualty c;
        c.squadName = x["squadName"].stringValue;
        c.team = x["team"].AsInt();
        c.lost = x["lost"].AsInt();
        c.wounded = x["wounded"].AsInt();
        c.dead = x["dead"].AsInt();
        c.eliminated = x["eliminated"].boolValue;
        p.lastReport.casualties.push_back(c);
    }
    for (const auto &v : r["relics"].arrayValue)
        p.lastReport.relics.push_back(v.stringValue);
    *this = std::move(p);
    return true;
}
} // namespace Potato::Campaign
