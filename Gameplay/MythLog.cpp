#include "MythLog.h"

#include "Serialization/JsonParser.h"

#include <cstdio>

namespace Potato {
namespace Gameplay {

void MythLog::Record(const std::string& shrine,
                     const std::string& spirit, const std::string& when,
                     const std::string& detail) {
    MythEvent ev{shrine, spirit, when, detail};
    events.push_back(ev);
    if (seepage) {
        seepage(ev); // UX 滲透觸發
    }
}

std::vector<std::string> MythLog::TestimonyLines() const {
    std::vector<std::string> lines;
    for (const auto& e : events) {
        lines.push_back(e.when + "·" + e.shrine + "之" + e.spirit +
                        "——" + e.detail);
    }
    return lines;
}

std::string MythLog::ToJson() const {
    std::string out =
        "{\"schema\":\"potato.myth_log/1\",\"events\":[";
    for (size_t i = 0; i < events.size(); ++i) {
        const auto& e = events[i];
        char buf[512];
        std::snprintf(buf, sizeof(buf),
                      "{\"shrine\":\"%s\",\"spirit\":\"%s\","
                      "\"when\":\"%s\",\"detail\":\"%s\"}%s",
                      e.shrine.c_str(), e.spirit.c_str(),
                      e.when.c_str(), e.detail.c_str(),
                      i + 1 < events.size() ? "," : "");
        out += buf;
    }
    out += "]}";
    return out;
}

bool MythLog::FromJson(const std::string& json) {
    JsonValue root;
    if (!JsonValue::ParseOk(json, root) ||
        root["schema"].AsString() != "potato.myth_log/1") {
        return false;
    }
    events.clear();
    for (const JsonValue& e : root["events"].AsArray()) {
        MythEvent ev;
        ev.shrine = e["shrine"].AsString();
        ev.spirit = e["spirit"].AsString();
        ev.when = e["when"].AsString();
        ev.detail = e["detail"].AsString();
        events.push_back(ev);
    }
    return true;
}

} // namespace Gameplay
} // namespace Potato
