#include "Gameplay/EnemyDeck.h"

#include "Gameplay/EnemyGeneral.h"
#include "Serialization/JsonParser.h"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

namespace Potato {
namespace Gameplay {

int EnemyDeckLibrary::LoadFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        warnings.push_back("無法讀取牌組檔：" + path);
        return 0;
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    JsonValue root;
    if (!JsonValue::ParseOk(ss.str(), root) || !root.IsObject()) {
        warnings.push_back("牌組 JSON 解析失敗：" + path);
        return 0;
    }
    if (root["schema"].AsString() != "potato.enemy_deck/1") {
        warnings.push_back("schema 非 potato.enemy_deck/1：" + path);
        return 0;
    }
    EnemyDeck d;
    d.id = root["id"].AsString();
    if (d.id.empty()) {
        warnings.push_back("牌組缺 id：" + path);
        return 0;
    }
    d.name = root["name"].AsString();
    d.description = root["description"].AsString();
    if (root["cards"].IsArray()) {
        for (const auto& c : root["cards"].AsArray()) {
            if (c.IsString() && !c.AsString().empty())
                d.cardIds.push_back(c.AsString());
            else
                warnings.push_back(d.id + "：cards 含非字串項，略過");
        }
    }
    if (d.cardIds.empty())
        warnings.push_back("牌組無有效卡：" + path);

    // 同 id 後載覆蓋（與 DoctrineLibrary 同款疊層語義）
    for (auto& e : decks) {
        if (e.id == d.id) {
            e = std::move(d);
            return 1;
        }
    }
    decks.push_back(std::move(d));
    return 1;
}

int EnemyDeckLibrary::LoadDir(const std::string& dir) {
    std::error_code ec;
    if (!fs::is_directory(dir, ec)) {
        warnings.push_back("牌組目錄不存在：" + dir);
        return 0;
    }
    int n = 0;
    for (const auto& e : fs::recursive_directory_iterator(dir, ec)) {
        if (e.is_regular_file(ec) && e.path().extension() == ".json")
            n += LoadFile(e.path().string());
    }
    return n;
}

const EnemyDeck* EnemyDeckLibrary::Find(const std::string& id) const {
    for (const auto& d : decks)
        if (d.id == id) return &d;
    return nullptr;
}

int EnemyDeckLibrary::ApplyTo(EnemyGeneral& general,
                              const EnemyDeck* deck,
                              const DoctrineLibrary& lib) {
    if (!deck) return 0;
    int added = 0;
    for (const std::string& cid : deck->cardIds) {
        const DoctrineCard* card = lib.Find(cid);
        if (!card) {
            warnings.push_back(deck->id + "：未知 doctrine 卡 " + cid);
            continue;
        }
        if (general.HasCard(cid)) continue; // 具名去重，不重複併入
        for (const DoctrineRule& r : card->rules) {
            general.AddCard(cid, r);
            ++added;
        }
    }
    return added;
}

} // namespace Gameplay
} // namespace Potato
