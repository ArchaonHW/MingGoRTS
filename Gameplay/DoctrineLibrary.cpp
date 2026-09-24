#include "DoctrineLibrary.h"
#include "Serialization/JsonParser.h"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

namespace Potato {
namespace Gameplay {

namespace {

const char* const kAxes[] = {"combat", "nobattle", "myth", nullptr};

// E/D epic 掛鉤點註冊表：無戰軸（NoBattleResolver 路徑加權）
// 與神話軸（MythLayer 安撫/天命連動）——新掛鉤在此註冊
const char* const kHooks[] = {
    "nobattle.negotiation", // 談判籌碼
    "nobattle.deterrence",  // 嚇阻加成
    "nobattle.subversion",  // 顛覆內應
    "nobattle.surrender",   // 受降加成
    "myth.soothing",        // 安撫（神怒壓力緩解）
    "myth.fate",            // 天命連動（fate 可用性）
    nullptr,
};

bool InTable(const char* const* table, const std::string& s) {
    for (const char* const* t = table; *t; ++t)
        if (s == *t) return true;
    return false;
}

} // namespace

const char* const* DoctrineLibrary::AxisNames() { return kAxes; }
const char* const* DoctrineLibrary::HookKeys() { return kHooks; }
bool DoctrineLibrary::IsAxis(const std::string& a) {
    return InTable(kAxes, a);
}
bool DoctrineLibrary::IsHookKey(const std::string& k) {
    return InTable(kHooks, k);
}

void DoctrineCard::AppendRules(DoctrineSet& set) const {
    for (const DoctrineRule& r : rules) set.AddRule(r);
}

void DoctrineLibrary::Clear() {
    cards.clear();
    byId.clear();
    warnings.clear();
}

int DoctrineLibrary::LoadDir(const std::string& dir) {
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

int DoctrineLibrary::LoadFile(const std::string& path) {
    std::ifstream f(path);
    if (!f) {
        warnings.push_back("讀檔失敗：" + path);
        return 0;
    }
    std::ostringstream ss;
    ss << f.rdbuf();
    JsonValue root;
    if (!JsonValue::ParseOk(ss.str(), root) || !root.IsObject() ||
        root["schema"].AsString() != "potato.doctrine/1") {
        warnings.push_back("非 doctrine/1 或解析失敗：" + path);
        return 0;
    }

    DoctrineCard card;
    card.id = root["id"].AsString();
    if (card.id.empty()) {
        warnings.push_back(path + "：缺 id，整卡跳過");
        return 0;
    }
    const std::string tag = path + "：「" + card.id + "」";
    if (byId.count(card.id)) {
        warnings.push_back(tag + " id 重複，後載覆蓋");
    }
    card.name = root["name"].AsString();
    if (card.name.empty()) {
        warnings.push_back(tag + " 缺 name");
    }
    card.flavor = root["flavor"].AsString();
    card.axis = root["axis"].AsString("combat");
    if (!IsAxis(card.axis)) {
        warnings.push_back(tag + " 未知 axis「" + card.axis +
                           "」，降為 combat");
        card.axis = "combat";
    }
    card.cost = root["cost"].AsInt(0);
    if (card.cost < 0) {
        warnings.push_back(tag + " cost<0 鉗回 0");
        card.cost = 0;
    }

    // rules：trigger/action 走既有名表；未知名整卡拒收
    // （與 doctrine_set FromJson 的「整組作廢」同規——靜默降級
    // 比拒收更難查）
    for (const JsonValue& r : root["rules"].AsArray()) {
        DoctrineRule rule;
        if (!TriggerFromName(r["trigger"].AsString(), rule.trigger) ||
            !ActionFromName(r["action"].AsString(), rule.action)) {
            warnings.push_back(tag + " 未知 trigger/action，整卡跳過");
            return 0;
        }
        rule.threshold = r["threshold"].AsFloat(0.0f);
        rule.priority = r["priority"].AsInt(100);
        rule.cooldown = r["cooldown"].AsFloat(0.0f);
        if (!std::isfinite(rule.threshold) ||
            !std::isfinite(rule.cooldown)) {
            warnings.push_back(tag + " threshold/cooldown 非有限，整卡跳過");
            return 0;
        }
        if (rule.threshold < 0.0f || rule.cooldown < 0.0f) {
            warnings.push_back(tag + " threshold/cooldown<0 鉗回 0");
            rule.threshold = (std::max)(0.0f, rule.threshold);
            rule.cooldown = (std::max)(0.0f, rule.cooldown);
        }
        card.rules.push_back(rule);
    }

    for (const JsonValue& m : root["modifiers"].AsArray()) {
        DoctrineModifier mod;
        mod.key = m["key"].AsString();
        mod.value = m["value"].AsFloat(0.0f);
        if (mod.key.empty() || !std::isfinite(mod.value)) {
            warnings.push_back(tag + " modifier 缺 key/值非有限，略過");
            continue;
        }
        if (!IsHookKey(mod.key)) {
            // 未知掛鉤：保留但警告——舊檔遇到新掛鉤不該死
            warnings.push_back(tag + " 未知 hook key「" + mod.key + "」");
        }
        card.modifiers.push_back(mod);
    }

    if (card.rules.empty() && card.modifiers.empty()) {
        warnings.push_back(tag + " 空卡（無 rule 無 modifier），跳過");
        return 0;
    }

    if (byId.count(card.id)) {
        cards[byId[card.id]] = std::move(card);
    } else {
        byId[card.id] = cards.size();
        cards.push_back(std::move(card));
    }
    return 1;
}

const DoctrineCard* DoctrineLibrary::Find(const std::string& id) const {
    auto it = byId.find(id);
    return it == byId.end() ? nullptr : &cards[it->second];
}

std::vector<const DoctrineCard*>
DoctrineLibrary::Axis(const std::string& axis) const {
    std::vector<const DoctrineCard*> out;
    for (const DoctrineCard& c : cards)
        if (c.axis == axis) out.push_back(&c);
    return out;
}

float DoctrineLibrary::TotalModifier(const std::string& key) const {
    float total = 0.0f;
    for (const DoctrineCard& c : cards)
        for (const DoctrineModifier& m : c.modifiers)
            if (m.key == key) total += m.value;
    return total;
}

float DoctrineLibrary::TotalModifier(const std::set<std::string>& ids,
                                     const std::string& key) const {
    float total = 0.0f;
    for (const std::string& id : ids) {
        const DoctrineCard* c = Find(id);
        if (!c) continue;
        for (const DoctrineModifier& m : c->modifiers)
            if (m.key == key) total += m.value;
    }
    return total;
}

} // namespace Gameplay
} // namespace Potato
