#include "Campaign/Myth/MythLog.h"

namespace Potato::Campaign {

Gameplay::Result<std::uint64_t> MythLog::Record(std::string_view action,
                                                std::string_view name,
                                                int side, int region,
                                                int squad) {
    if (entries_.size() >= MAX_ENTRIES) {
        return Gameplay::Fail<std::uint64_t>("mythlog",
                                             "log at capacity");
    }
    if (action.empty() || action.size() > MAX_NAME_LEN ||
        name.empty() || name.size() > MAX_NAME_LEN) {
        return Gameplay::Fail<std::uint64_t>("mythlog",
                                             "bad action/name");
    }
    MythLogEntry e;
    e.seq = entries_.size();
    e.action = std::string(action);
    e.name = std::string(name);
    e.side = side;
    e.region = region;
    e.squad = squad;
    entries_.push_back(std::move(e));
    return Gameplay::Ok(entries_.back().seq);
}

Gameplay::Result<Gameplay::JsonValue> MythLog::ToJson() const {
    using Gameplay::JsonValue;
    JsonValue::Array arr;
    for (const MythLogEntry& e : entries_) {
        JsonValue::Object o;
        o["seq"] = JsonValue::Int(static_cast<std::int64_t>(e.seq));
        o["action"] = JsonValue::String(e.action);
        o["name"] = JsonValue::String(e.name);
        o["side"] = JsonValue::Int(e.side);
        o["region"] = JsonValue::Int(e.region);
        o["squad"] = JsonValue::Int(e.squad);
        arr.push_back(JsonValue::MakeObject(std::move(o)));
    }
    JsonValue::Object root;
    root["schema"] = JsonValue::String(std::string(SCHEMA));
    root["entries"] = JsonValue::MakeArray(std::move(arr));
    return Gameplay::Ok(JsonValue::MakeObject(std::move(root)));
}

Gameplay::Result<MythLog> MythLog::FromJson(
    const Gameplay::JsonValue& doc) {
    using Gameplay::Fail;
    const std::string* s = doc.FindString("schema");
    if (!s || *s != SCHEMA) {
        return Fail<MythLog>("schema", "expected potato.mythlog/1");
    }
    const auto& arr = doc["entries"];
    if (!arr.IsArray()) {
        return Fail<MythLog>("schema", "entries: not an array");
    }
    if (arr.Size() > MAX_ENTRIES) {
        return Fail<MythLog>("mythlog", "too many entries");
    }
    MythLog out;
    for (const Gameplay::JsonValue& j : arr.Items()) {
        if (!j.IsObject() || !j["seq"].IsInt() ||
            !j["action"].IsString() || !j["name"].IsString() ||
            !j["side"].IsInt() || !j["region"].IsInt() ||
            !j["squad"].IsInt()) {
            return Fail<MythLog>("schema", "malformed entry");
        }
        const std::int64_t seq = j["seq"].AsInt();
        if (seq != static_cast<std::int64_t>(out.entries_.size())) {
            return Fail<MythLog>("schema", "seq not contiguous");
        }
        const auto side = j["side"].AsInt();
        const auto region = j["region"].AsInt();
        const auto squad = j["squad"].AsInt();
        if (side < -1 || side > 1 || region < -1 || squad < -1) {
            return Fail<MythLog>("schema",
                                 "entry field out of range");
        }
        auto r = out.Record(j["action"].AsString(),
                            j["name"].AsString(),
                            static_cast<int>(side),
                            static_cast<int>(region),
                            static_cast<int>(squad));
        if (!r.ok()) return Fail<MythLog>("schema", r.reason);
    }
    return Gameplay::Ok(std::move(out));
}

} // namespace Potato::Campaign
