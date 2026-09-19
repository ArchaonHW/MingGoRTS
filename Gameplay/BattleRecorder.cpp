#include "BattleRecorder.h"
#include "BattleController.h"
#include "Gameplay/Ledger.h"
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

uint64_t BattleRecorder::RootHash() const {
    uint64_t h = LedgerChain::kGenesisHash;
    char tbuf[32];
    for (const Record& r : records) {
        std::snprintf(tbuf, sizeof(tbuf), "%.6g",
                      static_cast<double>(r.t));
        h = LedgerHash(h, std::string(tbuf) + "|" + r.event);
    }
    return h;
}

bool BattleRecorder::SaveToFile(const std::string& path) const {
    std::ofstream f(path);
    if (!f) {
        return false;
    }
    char hbuf[24];
    std::snprintf(hbuf, sizeof(hbuf), "%016llx",
                  static_cast<unsigned long long>(RootHash()));
    f << "{\n  \"schema\": \"potato.battle_replay/1\",\n"
      << "  \"rootHash\": \"" << hbuf << "\",\n"
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
    const std::string storedHash = root["rootHash"].AsString();
    records.clear();
    for (const auto& e : root["events"].AsArray()) {
        records.push_back({e["t"].AsFloat(), e["event"].AsString()});
    }
    if (storedHash.empty()) {
        // 舊版無雜湊檔：降級載入，由呼叫端決定是否警告
        loadedLegacy = true;
        return true;
    }
    char hbuf[24];
    std::snprintf(hbuf, sizeof(hbuf), "%016llx",
                  static_cast<unsigned long long>(RootHash()));
    if (storedHash != hbuf) {
        // 雜湊不符 = 檔案遭篡改——拒絕且不留已載入的髒資料
        records.clear();
        return false;
    }
    loadedLegacy = false;
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
