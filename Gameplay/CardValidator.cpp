#include "CardValidator.h"
#include "Serialization/JsonParser.h"

#include <filesystem>
#include <fstream>
#include <set>
#include <sstream>

namespace fs = std::filesystem;

namespace Potato {
namespace Gameplay {

namespace {

const char* kTriggers[] = {
    "Always", "HealthBelow", "MoraleBelow", "EnemyInRange",
    "UnderAttack", "Outnumbered", "AllyEngaged", "ObjectiveReached",
    nullptr,
};
const char* kActions[] = {
    "AttackNearest", "AttackWeakest", "AdvanceToObjective",
    "HoldPosition", "RetreatToRally", "DefendNearestAlly", "Scout",
    nullptr,
};
const char* kRarities[] = {
    "common", "uncommon", "rare", "epic", "legendary", nullptr,
};
const char* kUnitClasses[] = {
    "infantry", "archer", "cavalry", nullptr,
};

bool InSet(const char* const* table, const std::string& s) {
    for (int i = 0; table[i]; ++i)
        if (s == table[i]) return true;
    return false;
}

void Err(CardReport& r, std::string field, std::string msg) {
    r.issues.push_back({true, std::move(field), std::move(msg)});
}
void Warn(CardReport& r, std::string field, std::string msg) {
    r.issues.push_back({false, std::move(field), std::move(msg)});
}

// 單張 doctrine 卡（signature 或 cards[] 元素）的 trigger/action 驗證
void CheckRule(const JsonValue& j, const std::string& base,
               CardReport& r) {
    if (!j.IsObject()) {
        Err(r, base, "規則不是物件");
        return;
    }
    const std::string trig = j["trigger"].AsString();
    const std::string act = j["action"].AsString();
    if (trig.empty())
        Err(r, base + ".trigger", "缺 trigger");
    else if (!InSet(kTriggers, trig))
        Err(r, base + ".trigger",
            "未知 trigger「" + trig + "」（載入會降級 Always）");
    if (act.empty())
        Err(r, base + ".action", "缺 action");
    else if (!InSet(kActions, act))
        Err(r, base + ".action",
            "未知 action「" + act + "」（載入會降級 HoldPosition）");
    if (j["threshold"].AsFloat(0.0f) < 0.0f)
        Warn(r, base + ".threshold", "threshold 為負");
    if (j["cooldown"].AsFloat(0.0f) < 0.0f)
        Warn(r, base + ".cooldown", "cooldown 為負");
}

} // namespace

const char* const* CardValidator::TriggerNames() { return kTriggers; }
const char* const* CardValidator::ActionNames() { return kActions; }
const char* const* CardValidator::RarityNames() { return kRarities; }
const char* const* CardValidator::UnitClassNames() { return kUnitClasses; }

bool CardReport::Ok() const {
    for (const auto& i : issues)
        if (i.error) return false;
    return true;
}

CardReport CardValidator::ValidateString(const std::string& json,
                                         const std::string& pathLabel,
                                         const std::string& assetsRoot) {
    CardReport r;
    r.path = pathLabel;

    JsonValue root;
    if (!JsonValue::ParseOk(json, root) || !root.IsObject()) {
        Err(r, "(root)", "JSON 解析失敗或非物件");
        return r;
    }

    if (root["schema"].AsString() != "potato.character_card/1")
        Err(r, "schema", "須為 potato.character_card/1");

    r.cardId = root["id"].AsString();
    if (r.cardId.empty()) Err(r, "id", "缺卡 id");
    if (root["name"].AsString().empty()) Err(r, "name", "缺名稱");

    // 人格三軸：缺→warn（載入預設 50），越界→warn（載入 clamp）
    const JsonValue& p = root["personality"];
    if (!p.IsObject()) {
        Warn(r, "personality", "缺人格塊（三軸全取預設 50）");
    } else {
        for (const char* axis : {"aggression", "discipline", "cunning"}) {
            const JsonValue& a = p[axis];
            if (a.IsNull())
                Warn(r, std::string("personality.") + axis,
                     "缺軸（預設 50）");
            else {
                float v = a.AsFloat(-1.0f);
                if (v < 0.0f || v > 100.0f)
                    Warn(r, std::string("personality.") + axis,
                         "越界（載入時 clamp 到 0..100）");
            }
        }
    }

    // signatureDoctrine：敵將的「招式」——缺了降級警告不擋
    const JsonValue& sig = root["signatureDoctrine"];
    if (!sig.IsObject())
        Warn(r, "signatureDoctrine", "缺簽名卡（敵將無招式）");
    else
        CheckRule(sig, "signatureDoctrine", r);

    // 附加卡槽
    int idx = 0;
    for (const auto& j : root["cards"].AsArray())
        CheckRule(j, "cards[" + std::to_string(idx++) + "]", r);

    // unit_class：未知值會被載入器靜默忽略——視為 error
    const std::string uc = root["unit_class"].AsString();
    if (!uc.empty() && !InSet(kUnitClasses, uc))
        Err(r, "unit_class", "未知兵種「" + uc + "」");

    const std::string rarity = root["rarity"].AsString();
    if (!rarity.empty() && !InSet(kRarities, rarity))
        Warn(r, "rarity", "未知稀有度「" + rarity + "」");

    // 立繪檔案存在性（assetsRoot 非空才查）
    const std::string art = root["art"].AsString();
    if (!art.empty() && !assetsRoot.empty() &&
        !fs::exists(fs::path(assetsRoot) / art))
        Warn(r, "art", "立繪檔不存在：" + art);

    return r;
}

CardReport CardValidator::ValidateFile(const std::string& path,
                                       const std::string& assetsRoot) {
    std::ifstream f(path);
    if (!f) {
        CardReport r;
        r.path = path;
        Err(r, "(file)", "讀檔失敗");
        return r;
    }
    std::ostringstream ss;
    ss << f.rdbuf();
    return ValidateString(ss.str(), path, assetsRoot);
}

std::vector<CardReport> CardValidator::ValidateDir(
    const std::string& dir, const std::string& assetsRoot) {
    std::vector<CardReport> out;
    std::error_code ec;
    if (!fs::is_directory(dir, ec)) return out;

    for (const auto& e : fs::recursive_directory_iterator(dir, ec)) {
        if (!e.is_regular_file() || e.path().extension() != ".json")
            continue;
        out.push_back(ValidateFile(e.path().string(), assetsRoot));
    }

    // 跨卡：id 重複（同名 id 會讓卡池指向不明）
    std::set<std::string> seen;
    for (auto& r : out) {
        if (r.cardId.empty()) continue;
        if (!seen.insert(r.cardId).second)
            Err(r, "id", "卡 id 重複：" + r.cardId);
    }
    return out;
}

} // namespace Gameplay
} // namespace Potato
