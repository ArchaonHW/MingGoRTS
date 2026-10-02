#include "Roster.h"
#include "BattleController.h"
#include "Serialization/JsonParser.h"
#include "Serialization/JsonValidation.h"
#include "Squad.h"

#include <fstream>
#include <sstream>

namespace Potato {
namespace Gameplay {

void Roster::Enroll(const Squad *squad, const std::string &name, const std::string &rank,
                    const std::string &relic, const std::string &art) {
    if (!squad) {
        return;
    }
    RosterEntry e;
    e.name = name;
    e.rank = rank;
    e.squadName = squad->GetName();
    e.team = squad->GetTeam();
    e.relic = relic;
    e.art = art;
    entries.push_back(e);
    watched.push_back(squad);
}

void Roster::Update(const BattleController &battle) {
    for (size_t i = 0; i < watched.size(); ++i) {
        if (entries[i].alive && watched[i] && watched[i]->IsEliminated()) {
            entries[i].alive = false;
            entries[i].deathTime = battle.GetElapsed();
        }
    }
}

size_t Roster::DeadCount() const {
    size_t n = 0;
    for (const auto &e : entries) {
        if (!e.alive) {
            ++n;
        }
    }
    return n;
}

static std::string Esc(const std::string &s) {
    std::string out;
    for (char c : s) {
        if (c == '"')
            out += "\\\"";
        else if (c == '\\')
            out += "\\\\";
        else
            out += c;
    }
    return out;
}

bool Roster::SaveToFile(const std::string &path) const {
    std::ofstream f(path);
    if (!f) {
        return false;
    }
    f << "{\n  \"schema\": \"potato.roster/1\",\n  \"entries\": [\n";
    for (size_t i = 0; i < entries.size(); ++i) {
        const RosterEntry &e = entries[i];
        f << "    {\"name\": \"" << Esc(e.name) << "\", \"rank\": \"" << Esc(e.rank)
          << "\", \"squad\": \"" << Esc(e.squadName) << "\", \"team\": " << e.team
          << ", \"alive\": " << (e.alive ? "true" : "false") << ", \"deathTime\": " << e.deathTime
          << ", \"relic\": \"" << Esc(e.relic) << "\""
          << ", \"art\": \"" << Esc(e.art) << "\"}" << (i + 1 < entries.size() ? ",\n" : "\n");
    }
    f << "  ]\n}\n";
    return f.good();
}

JsonValue Roster::ToJson() const {
    JsonValue o;
    o.type = JsonValue::Type::Object;
    o.objectValue["schema"] = JsonValue::String("potato.roster/1");
    JsonValue arr;
    arr.type = JsonValue::Type::Array;
    for (const auto &e : entries) {
        JsonValue je;
        je.type = JsonValue::Type::Object;
        je.objectValue["name"] = JsonValue::String(e.name);
        je.objectValue["rank"] = JsonValue::String(e.rank);
        je.objectValue["squad"] = JsonValue::String(e.squadName);
        je.objectValue["team"] = JsonValue::Number(e.team);
        je.objectValue["alive"] = JsonValue::Bool(e.alive);
        je.objectValue["deathTime"] = JsonValue::Number(e.deathTime);
        je.objectValue["relic"] = JsonValue::String(e.relic);
        je.objectValue["art"] = JsonValue::String(e.art);
        arr.arrayValue.push_back(je);
    }
    o.objectValue["entries"] = arr;
    return o;
}

// 永久名冊按既有身分合併，陣亡是不可逆狀態；合併後一律移除監看指標。
// 戰鬥中只 Update 本場名冊，不能讓跨章歷史依賴上一場已銷毀的 Squad。
void Roster::MergeHistory(const Roster &source) {
    for (const auto &e : source.entries) {
        bool found = false;
        for (auto &old : entries)
            if (old.team == e.team && old.name == e.name) {
                if (old.alive)
                    old = e;
                found = true;
                break;
            }
        if (!found)
            entries.push_back(e);
    }
    watched.assign(entries.size(), nullptr);
}
bool Roster::FromJson(const JsonValue &root) {
    if (!root.IsObject() || root["schema"].AsString() != "potato.roster/1" ||
        !root["entries"].IsArray())
        return false;
    std::vector<RosterEntry> candidate;
    for (const auto &j : root["entries"].AsArray()) {
        for (auto key : {"name", "rank", "squad", "relic", "art"})
            if (!j[key].IsString())
                return false;
        if (!JsonValidation::Integer(j["team"], 0, 1) || !j["alive"].IsBool() ||
            !JsonValidation::Number(j["deathTime"], -1, 100000000))
            return false;
        RosterEntry e;
        e.name = j["name"].AsString();
        e.rank = j["rank"].AsString();
        e.squadName = j["squad"].AsString();
        e.team = j["team"].AsInt();
        e.alive = j["alive"].AsBool();
        e.deathTime = j["deathTime"].AsFloat();
        e.relic = j["relic"].AsString();
        e.art = j["art"].AsString();
        if (e.name.empty() || e.squadName.empty() || (!e.alive && e.deathTime < 0))
            return false;
        candidate.push_back(e);
    }
    entries = std::move(candidate);
    watched.assign(entries.size(), nullptr);
    return true;
}

bool Roster::LoadFromFile(const std::string &path) {
    std::ifstream f(path);
    if (!f) {
        return false;
    }
    std::ostringstream ss;
    ss << f.rdbuf();
    JsonValue root;
    if (!JsonValue::ParseOk(ss.str(), root)) {
        return false;
    }
    return FromJson(root);
}

} // namespace Gameplay
} // namespace Potato
