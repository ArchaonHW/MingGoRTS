#include "Campaign/World/WorldState.h"

#include "Gameplay/Json/JsonValue.h"

#include <algorithm>
#include <cstring>
#include <utility>

namespace Potato::Campaign {
namespace {

using Gameplay::JsonValue;
using Gameplay::Result;

bool ControlFromName(std::string_view s, WorldControl& out) {
    if (s == "neutral") {
        out = WorldControl::Neutral;
        return true;
    }
    if (s == "player") {
        out = WorldControl::Player;
        return true;
    }
    if (s == "rival") {
        out = WorldControl::Rival;
        return true;
    }
    return false;
}

const char* ControlName(WorldControl c) {
    switch (c) {
    case WorldControl::Neutral:
        return "neutral";
    case WorldControl::Player:
        return "player";
    case WorldControl::Rival:
        return "rival";
    }
    return "neutral";
}

const char* KindName(WorldEventKind k) {
    switch (k) {
    case WorldEventKind::SetControl:
        return "control";
    case WorldEventKind::Resolve:
        return "resolve";
    }
    return "control";
}

bool KindFromName(std::string_view s, WorldEventKind& out) {
    if (s == "control") {
        out = WorldEventKind::SetControl;
        return true;
    }
    if (s == "resolve") {
        out = WorldEventKind::Resolve;
        return true;
    }
    return false;
}

bool ValidId(std::string_view s) {
    return !s.empty() && s.size() <= WorldState::MAX_ID_LEN;
}

// Bitcast a u64 counter to/from an int64 wire field (MintOutbox
// ledger-seal precedent) — bit-exact across platforms.
std::int64_t ToWire(std::uint64_t v) {
    std::int64_t s;
    std::memcpy(&s, &v, sizeof(s));
    return s;
}
std::uint64_t FromWire(std::int64_t s) {
    std::uint64_t v;
    std::memcpy(&v, &s, sizeof(v));
    return v;
}

// Canonical order: (day, seq, insertion-stable).
bool Before(const WorldEvent& a, const WorldEvent& b) {
    if (a.day != b.day) {
        return a.day < b.day;
    }
    return a.seq < b.seq;
}

} // namespace

Result<WorldState> WorldState::Init(const WorldMap& map,
                                    std::uint64_t seed) {
    if (map.NodeCount() == 0) {
        return Gameplay::Fail<WorldState>("world",
                                          "cannot Init on empty map");
    }
    WorldState s;
    s.worldId_ = map.Id();
    s.day_ = 0;
    s.warband_ = map.NodeAt(map.StartIndex()).id;
    for (std::size_t i = 0; i < map.NodeCount(); ++i) {
        const WorldNode& n = map.NodeAt(i);
        if (n.control != WorldControl::Neutral) {
            s.control_[n.id] = n.control;
        }
    }
    s.rng_ = Gameplay::Prng(seed);
    return Gameplay::Ok(std::move(s));
}

WorldControl WorldState::ControlAt(std::string_view node) const {
    const auto it = control_.find(std::string(node));
    return it == control_.end() ? WorldControl::Neutral : it->second;
}

Result<bool> WorldState::Enqueue(const WorldMap& map, WorldEvent ev) {
    if (!ValidId(ev.node)) {
        return Gameplay::Fail<bool>("world", "event node id bad");
    }
    if (map.FindNode(ev.node) == nullptr) {
        return Gameplay::Fail<bool>("world",
                                    "event targets unknown node");
    }
    if (ev.day < day_ || ev.day > MAX_DAY) {
        return Gameplay::Fail<bool>("world",
                                    "event day out of range");
    }
    if (queue_.size() >= MAX_EVENTS) {
        return Gameplay::Fail<bool>("world", "event queue full");
    }
    queue_.push_back(std::move(ev));
    return Gameplay::Ok(true);
}

Result<int> WorldState::ResolveBeats(const WorldMap& map,
                                     std::int64_t days) {
    if (days < 0 || day_ + days > MAX_DAY) {
        return Gameplay::Fail<int>("world", "day advance out of range");
    }
    day_ += days;

    // Split due events out, THEN stable-sort the extracted batch —
    // sorting index lists breaks the compaction below (the index
    // order no longer matches queue positions).
    std::vector<WorldEvent> batch;
    std::vector<WorldEvent> keep;
    for (WorldEvent& ev : queue_) {
        if (ev.day <= day_) {
            batch.push_back(std::move(ev));
        } else {
            keep.push_back(std::move(ev));
        }
    }
    std::stable_sort(batch.begin(), batch.end(), Before);
    queue_ = std::move(keep);

    int applied = 0;
    for (const WorldEvent& ev : batch) {
        if (map.FindNode(ev.node) == nullptr) {
            continue; // node edited out of content — skip
        }
        switch (ev.kind) {
        case WorldEventKind::SetControl:
            if (ev.control == WorldControl::Neutral) {
                control_.erase(ev.node);
            } else {
                control_[ev.node] = ev.control;
            }
            break;
        case WorldEventKind::Resolve:
            if (resolved_.size() < MAX_RESOLVED ||
                resolved_.count(ev.node)) {
                resolved_.insert(ev.node);
            }
            break;
        }
        ++applied;
    }
    return Gameplay::Ok(applied);
}

bool WorldState::SetWarband(std::string_view node,
                            const WorldMap& map) {
    if (map.FindNode(node) == nullptr) {
        return false;
    }
    warband_ = std::string(node);
    return true;
}

Result<JsonValue> WorldState::ToJson() const {
    JsonValue::Object root;
    root["schema"] = JsonValue::String(std::string(SCHEMA));
    root["world"] = JsonValue::String(worldId_);
    root["day"] = JsonValue::Int(day_);
    root["warband"] = JsonValue::String(warband_);
    if (!control_.empty()) {
        JsonValue::Object ctrl;
        for (const auto& [id, c] : control_) {
            ctrl[id] = JsonValue::String(ControlName(c));
        }
        root["control"] = JsonValue::MakeObject(std::move(ctrl));
    }
    if (!resolved_.empty()) {
        JsonValue::Array rs;
        for (const std::string& id : resolved_) {
            rs.push_back(JsonValue::String(id));
        }
        root["resolved"] = JsonValue::MakeArray(std::move(rs));
    }
    root["rng"] = JsonValue::Int(ToWire(rng_.State()));
    if (!queue_.empty()) {
        std::vector<WorldEvent> sorted = queue_;
        std::stable_sort(sorted.begin(), sorted.end(), Before);
        JsonValue::Array q;
        for (const WorldEvent& ev : sorted) {
            JsonValue::Object je;
            je["day"] = JsonValue::Int(ev.day);
            je["seq"] = JsonValue::Int(
                static_cast<std::int64_t>(ev.seq));
            je["kind"] = JsonValue::String(KindName(ev.kind));
            je["node"] = JsonValue::String(ev.node);
            if (ev.kind == WorldEventKind::SetControl) {
                je["control"] =
                    JsonValue::String(ControlName(ev.control));
            }
            q.push_back(JsonValue::MakeObject(std::move(je)));
        }
        root["queue"] = JsonValue::MakeArray(std::move(q));
    }
    return Gameplay::Ok(JsonValue::MakeObject(std::move(root)));
}

Result<WorldState> WorldState::FromJson(const JsonValue& doc) {
    if (!doc.IsObject()) {
        return Gameplay::Fail<WorldState>("world",
                                          "state is not an object");
    }
    {
        const std::string* s = doc.FindString("schema");
        if (s == nullptr || *s != SCHEMA) {
            return Gameplay::Fail<WorldState>(
                "schema", "expected potato.worldstate/1");
        }
    }
    // Two-pass: validate everything into locals, commit at the end.
    WorldState s;
    {
        const std::string* w = doc.FindString("world");
        if (w == nullptr || !ValidId(*w)) {
            return Gameplay::Fail<WorldState>("world",
                                              "missing/bad 'world'");
        }
        s.worldId_ = *w;
    }
    {
        const JsonValue& d = doc["day"];
        if (!d.IsInt() || d.AsInt() < 0 || d.AsInt() > MAX_DAY) {
            return Gameplay::Fail<WorldState>("world", "bad 'day'");
        }
        s.day_ = d.AsInt();
    }
    {
        const std::string* wb = doc.FindString("warband");
        if (wb == nullptr || !ValidId(*wb)) {
            return Gameplay::Fail<WorldState>("world",
                                              "missing/bad 'warband'");
        }
        s.warband_ = *wb;
    }
    if (doc.Has("control")) {
        const JsonValue& c = doc["control"];
        if (!c.IsObject()) {
            return Gameplay::Fail<WorldState>("world",
                                              "'control' not object");
        }
        for (const auto& [k, v] : c.Members()) {
            if (!ValidId(k)) {
                return Gameplay::Fail<WorldState>(
                    "world", "control key out of range");
            }
            WorldControl wc;
            if (!v.IsString() || !ControlFromName(v.AsString(), wc)) {
                return Gameplay::Fail<WorldState>(
                    "world", "control value unknown");
            }
            if (wc != WorldControl::Neutral) {
                s.control_[k] = wc;
            }
        }
    }
    if (doc.Has("resolved")) {
        const JsonValue& r = doc["resolved"];
        if (!r.IsArray() || r.Size() > MAX_RESOLVED) {
            return Gameplay::Fail<WorldState>("world",
                                              "'resolved' bad/over");
        }
        for (const JsonValue& item : r.Items()) {
            if (!item.IsString() || !ValidId(item.AsString())) {
                return Gameplay::Fail<WorldState>(
                    "world", "resolved entry bad");
            }
            s.resolved_.insert(item.AsString());
        }
    }
    if (doc.Has("rng")) {
        const JsonValue& r = doc["rng"];
        if (!r.IsInt()) {
            return Gameplay::Fail<WorldState>("world",
                                              "'rng' not int");
        }
        s.rng_ = Gameplay::Prng(FromWire(r.AsInt()));
    }
    if (doc.Has("queue")) {
        const JsonValue& q = doc["queue"];
        if (!q.IsArray() || q.Size() > MAX_EVENTS) {
            return Gameplay::Fail<WorldState>("world",
                                              "'queue' bad/over");
        }
        for (const JsonValue& je : q.Items()) {
            if (!je.IsObject()) {
                return Gameplay::Fail<WorldState>(
                    "world", "queue entry not object");
            }
            WorldEvent ev;
            const JsonValue& d = je["day"];
            const JsonValue& sq = je["seq"];
            if (!d.IsInt() || d.AsInt() < 0 || d.AsInt() > MAX_DAY ||
                !sq.IsInt() || sq.AsInt() < 0) {
                return Gameplay::Fail<WorldState>(
                    "world", "queue day/seq bad");
            }
            ev.day = d.AsInt();
            ev.seq = static_cast<std::uint64_t>(sq.AsInt());
            const std::string* k = je.FindString("kind");
            if (k == nullptr || !KindFromName(*k, ev.kind)) {
                return Gameplay::Fail<WorldState>(
                    "world", "queue kind unknown");
            }
            const std::string* n = je.FindString("node");
            if (n == nullptr || !ValidId(*n)) {
                return Gameplay::Fail<WorldState>(
                    "world", "queue node bad");
            }
            ev.node = *n;
            if (ev.kind == WorldEventKind::SetControl) {
                const std::string* c = je.FindString("control");
                if (c == nullptr ||
                    !ControlFromName(*c, ev.control)) {
                    return Gameplay::Fail<WorldState>(
                        "world", "queue control bad");
                }
            }
            s.queue_.push_back(std::move(ev));
        }
    }
    return Gameplay::Ok(std::move(s));
}

} // namespace Potato::Campaign
