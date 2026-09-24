#include "NarrativePack.h"
#include "Serialization/JsonParser.h"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

namespace Potato {
namespace Gameplay {

namespace {

// 全部出現的 {key} 代換為 value（字面取代，無轉義）
void ReplaceAll(std::string& s, const std::string& key,
                const std::string& value) {
    size_t pos = 0;
    while ((pos = s.find(key, pos)) != std::string::npos) {
        s.replace(pos, key.size(), value);
        pos += value.size();
    }
}

} // namespace

void NarrativePack::Clear() {
    entries.clear();
    warnings.clear();
    lookupMisses = 0;
}

int NarrativePack::LoadFile(const std::string& path) {
    std::ifstream f(path);
    if (!f) {
        warnings.push_back("讀檔失敗：" + path);
        return 0;
    }
    std::ostringstream ss;
    ss << f.rdbuf();

    JsonValue root;
    if (!JsonValue::ParseOk(ss.str(), root) || !root.IsObject() ||
        root["schema"].AsString() != "potato.narrative_pack/1") {
        warnings.push_back("非 narrative_pack/1 或解析失敗：" + path);
        return 0;
    }

    int n = 0;
    for (const auto& e : root["entries"].AsArray()) {
        const std::string id = e["id"].AsString();
        if (!e.IsObject() || id.empty()) {
            warnings.push_back(path + "：條目缺 id，跳過");
            continue;
        }
        NarrativeEntry ne;
        ne.id = id;
        // locale 內容：locales 物件逐鍵，或頂層 "zh-TW"/"en" 平欄位
        const JsonValue& loc = e["locales"];
        if (loc.IsObject()) {
            for (const auto& [k, v] : loc.objectValue)
                ne.locales[k] = v.AsString();
        } else {
            for (const char* k : {"zh-TW", "en"}) {
                const std::string v = e[k].AsString();
                if (!v.empty()) ne.locales[k] = v;
            }
        }
        if (ne.locales.empty()) {
            warnings.push_back(path + "：「" + id + "」無內容，跳過");
            continue;
        }
        ne.reg = e["register"].AsString();
        for (const auto& tg : e["tags"].AsArray())
            ne.tags.push_back(tg.AsString());
        entries[id] = std::move(ne); // 同 id 後載覆蓋
        ++n;
    }
    return n;
}

int NarrativePack::LoadDir(const std::string& dir) {
    int n = 0;
    std::error_code ec;
    if (!fs::is_directory(dir, ec)) {
        warnings.push_back("目錄不存在：" + dir);
        return 0;
    }
    for (const auto& e : fs::recursive_directory_iterator(dir, ec)) {
        if (e.is_regular_file() && e.path().extension() == ".json")
            n += LoadFile(e.path().string());
    }
    return n;
}

const std::string* NarrativePack::Find(const std::string& id,
                                       const std::string& locale) const {
    auto it = entries.find(id);
    if (it == entries.end()) { ++lookupMisses; return nullptr; }
    const auto& locs = it->second.locales;
    for (const std::string& k : {locale, std::string("zh-TW"),
                                 std::string("en")}) {
        auto li = locs.find(k);
        if (li != locs.end() && !li->second.empty())
            return &li->second;
    }
    if (!locs.empty()) return &locs.begin()->second;
    ++lookupMisses;
    return nullptr;
}

std::string NarrativePack::Frag(
    const std::string& id, const std::string& fallback,
    const std::map<std::string, std::string>& pairs,
    const std::string& locale) const {
    const std::string* s = Find(id, locale);
    std::string out = s ? *s : fallback;
    for (const auto& [k, v] : pairs) ReplaceAll(out, k, v);
    return out;
}

const NarrativeEntry* NarrativePack::Entry(const std::string& id) const {
    auto it = entries.find(id);
    return it == entries.end() ? nullptr : &it->second;
}

} // namespace Gameplay
} // namespace Potato
