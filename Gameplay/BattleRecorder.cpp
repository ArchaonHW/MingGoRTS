#include "BattleRecorder.h"
#include "BattleController.h"
#include "Serialization/JsonParser.h"

#include <chrono>
#include <fstream>
#include <sstream>
#include <thread>

namespace Potato {
namespace Gameplay {

void BattleRecorder::Attach(BattleController& battle) {
    auto downstream = battle.GetEventCallback();
    BattleController* bc = &battle;
    battle.SetEventCallback([this, bc, downstream](const std::string& msg) {
        records.push_back({bc->GetElapsed(), msg});
        if (downstream) {
            downstream(msg);
        }
    });
}

static std::string EscapeJson(const std::string& s) {
    std::string out;
    for (char c : s) {
        switch (c) {
        case '"':  out += "\\\""; break;
        case '\\': out += "\\\\"; break;
        case '\n': out += "\\n"; break;
        case '\t': out += "\\t"; break;
        default:
            if (static_cast<unsigned char>(c) < 0x20) {
                char buf[8];
                std::snprintf(buf, sizeof(buf), "\\u%04x", c);
                out += buf;
            } else {
                out += c;
            }
        }
    }
    return out;
}

bool BattleRecorder::SaveToFile(const std::string& path) const {
    std::ofstream f(path);
    if (!f) {
        return false;
    }
    f << "{\n  \"schema\": \"potato.battle_replay/1\",\n"
      << "  \"events\": [\n";
    for (size_t i = 0; i < records.size(); ++i) {
        f << "    {\"t\": " << records[i].t
          << ", \"event\": \"" << EscapeJson(records[i].event) << "\"}"
          << (i + 1 < records.size() ? ",\n" : "\n");
    }
    f << "  ]\n}\n";
    return f.good();
}

bool BattleRecorder::LoadFromFile(const std::string& path) {
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
    records.clear();
    for (const auto& e : root["events"].AsArray()) {
        records.push_back({e["t"].AsFloat(), e["event"].AsString()});
    }
    return true;
}

void BattleRecorder::Replay(
    const std::function<void(float, const std::string&)>& cb,
    float timeScale) const {
    float lastT = 0.0f;
    for (const auto& r : records) {
        if (timeScale > 0.0f && r.t > lastT) {
            float wait = (r.t - lastT) / timeScale;
            std::this_thread::sleep_for(
                std::chrono::milliseconds(static_cast<int>(wait * 1000)));
        }
        lastT = r.t;
        cb(r.t, r.event);
    }
}

} // namespace Gameplay
} // namespace Potato
