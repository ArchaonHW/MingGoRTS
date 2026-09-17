#include "Roster.h"
#include "BattleController.h"
#include "Squad.h"
#include "Serialization/JsonParser.h"

#include <fstream>
#include <sstream>

namespace Potato {
namespace Gameplay {

void Roster::Enroll(const Squad* squad, const std::string& name,
                    const std::string& rank, const std::string& relic,
                    const std::string& art) {
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

void Roster::Update(const BattleController& battle) {
    for (size_t i = 0; i < watched.size(); ++i) {
        if (entries[i].alive && watched[i]->IsEliminated()) {
            entries[i].alive = false;
            entries[i].deathTime = battle.GetElapsed();
        }
    }
}

size_t Roster::DeadCount() const {
    size_t n = 0;
    for (const auto& e : entries) {
        if (!e.alive) {
            ++n;
        }
    }
    return n;
}

static std::string Esc(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else out += c;
    }
    return out;
}

bool Roster::SaveToFile(const std::string& path) const {
    std::ofstream f(path);
    if (!f) {
        return false;
    }
    f << "{\n  \"schema\": \"potato.roster/1\",\n  \"entries\": [\n";
    for (size_t i = 0; i < entries.size(); ++i) {
        const RosterEntry& e = entries[i];
        f << "    {\"name\": \"" << Esc(e.name)
          << "\", \"rank\": \"" << Esc(e.rank)
          << "\", \"squad\": \"" << Esc(e.squadName)
          << "\", \"team\": " << e.team
          << ", \"alive\": " << (e.alive ? "true" : "false")
          << ", \"deathTime\": " << e.deathTime
          << ", \"relic\": \"" << Esc(e.relic) << "\""
          << ", \"art\": \"" << Esc(e.art) << "\"}"
          << (i + 1 < entries.size() ? ",\n" : "\n");
    }
    f << "  ]\n}\n";
    return f.good();
}

bool Roster::LoadFromFile(const std::string& path) {
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
    entries.clear();
    watched.clear();
    for (const auto& j : root["entries"].AsArray()) {
        RosterEntry e;
        e.name = j["name"].AsString();
        e.rank = j["rank"].AsString();
        e.squadName = j["squad"].AsString();
        e.team = j["team"].AsInt();
        e.alive = j["alive"].AsBool(true);
        e.deathTime = j["deathTime"].AsFloat(-1.0f);
        e.relic = j["relic"].AsString();
        e.art = j["art"].AsString();
        entries.push_back(e);
        watched.push_back(nullptr); // 讀回的名冊不再追蹤即時物件
    }
    return true;
}

} // namespace Gameplay
} // namespace Potato
