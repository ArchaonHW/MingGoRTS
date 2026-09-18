#include "SquadTemplate.h"
#include "BattleController.h"
#include "Serialization/JsonParser.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <system_error>

namespace Potato {
namespace Gameplay {

namespace {
// stats.stamina 缺欄時的 Squad 預設值（與 Squad.cpp 建構同步）
constexpr float kDefaultDrainMove = 0.03f;
constexpr float kDefaultDrainCombat = 0.05f;
constexpr float kDefaultRegen = 0.06f;
constexpr float kDefaultThreshold = 0.3f;
constexpr float kDefaultPenaltyMul = 0.6f;
} // namespace

bool SquadTemplate::LoadFromFile(const std::string& path) {
    // 容錯：逐層往上找（CTest 工作目錄可能深至 build/bin/Release）
    for (const char* prefix : {"", "../", "../../", "../../../"}) {
        std::ifstream f(std::string(prefix) + path);
        if (f) {
            std::ostringstream ss;
            ss << f.rdbuf();
            return LoadFromString(ss.str());
        }
    }
    return false;
}

bool SquadTemplate::LoadFromString(const std::string& json) {
    JsonValue root;
    if (!JsonValue::ParseOk(json, root) || !root.IsObject()) {
        return false;
    }
    // schema 驗證：不符/缺欄一律拒絕，不產生半成品
    if (root["schema"].AsString() != kSchema) {
        return false;
    }

    warnings.clear();
    id = root["id"].AsString();
    name = root["name"].AsString(id); // 缺名用 id 頂替
    members = root["members"].AsInt(10);
    if (members <= 0) {
        members = 1; // 人數是模板的一部分；非正數無意義，保底 1
    }
    cost = root["cost"].AsInt(0);
    if (cost < 0) {
        cost = 0;
    }

    // unit_class：無效字串 → Infantry + 警告（EnemyGeneral 慣例）；
    // 缺欄走預設不算警告
    {
        const std::string ucs = root["unit_class"].AsString();
        bool ok = false;
        unitClass = UnitClassFromString(ucs, &ok);
        if (!ok && !ucs.empty()) {
            warnings.push_back("未知 unit_class \"" + ucs +
                               "\"，降為 infantry");
        }
    }

    // stats 子物件：缺子欄位 → Squad 預設值，不 fail
    const JsonValue& stats = root["stats"];
    speed = stats["speed"].AsFloat(2.0f);
    engageRange = stats["engage_range"].AsFloat(1.5f);
    damagePerMember = stats["damage_per_member"].AsFloat(0.05f);

    // stamina 整段可選；有寫則五欄逐讀，缺一欄用 Squad 預設
    const JsonValue& st = stats["stamina"];
    hasStamina = st.IsObject();
    if (hasStamina) {
        staminaDrainMove = st["drain_move"].AsFloat(kDefaultDrainMove);
        staminaDrainCombat = st["drain_combat"].AsFloat(kDefaultDrainCombat);
        staminaRegen = st["regen"].AsFloat(kDefaultRegen);
        staminaThreshold = st["threshold"].AsFloat(kDefaultThreshold);
        staminaPenaltyMul = st["penalty_mul"].AsFloat(kDefaultPenaltyMul);
    }
    return true;
}

Squad* SquadTemplate::Instantiate(BattleController& battle,
                                  const std::string& squadName,
                                  int team, const Vector2& pos) const {
    std::string finalName = squadName;
    if (finalName.empty()) {
        finalName = name;
    }
    if (finalName.empty()) {
        finalName = id;
    }
    Squad* s = battle.CreateSquad(finalName, team, pos, members);
    if (!s) {
        return nullptr;
    }
    // 逐 setter 套屬性——不改 CreateSquad 簽名，引擎面零侵入
    s->SetUnitClass(unitClass);
    s->SetSpeed(speed);
    s->SetEngageRange(engageRange);
    s->SetDamagePerMember(damagePerMember);
    if (hasStamina) {
        s->SetStaminaParams(staminaDrainMove, staminaDrainCombat,
                            staminaRegen, staminaThreshold,
                            staminaPenaltyMul);
    }
    return s;
}

size_t SquadTemplateLibrary::LoadDir(const std::string& dir) {
    namespace fs = std::filesystem;

    // 目錄本身也吃逐層 ../ 容錯（與 LoadFromFile 同一工作目錄假設）
    fs::path root;
    for (const char* prefix : {"", "../", "../../", "../../../"}) {
        std::error_code ec;
        fs::path cand(std::string(prefix) + dir);
        if (fs::is_directory(cand, ec)) {
            root = cand;
            break;
        }
    }
    if (root.empty()) {
        return 0;
    }

    std::vector<fs::path> files;
    {
        std::error_code ec;
        for (const auto& e : fs::directory_iterator(root, ec)) {
            if (!e.is_regular_file(ec)) {
                continue;
            }
            std::string ext = e.path().extension().string();
            for (auto& c : ext) {
                c = static_cast<char>(
                    std::tolower(static_cast<unsigned char>(c)));
            }
            if (ext == ".json") {
                files.push_back(e.path());
            }
        }
    }
    std::sort(files.begin(), files.end()); // 載入順序穩定

    size_t loaded = 0;
    for (const auto& p : files) {
        std::ifstream f(p);
        if (!f) {
            continue;
        }
        std::ostringstream ss;
        ss << f.rdbuf();
        SquadTemplate tpl;
        // 壞檔/schema 不符：跳過不中止（單檔錯誤不拖垮整庫）
        if (tpl.LoadFromString(ss.str())) {
            templates.push_back(tpl);
            ++loaded;
        }
    }
    return loaded;
}

const SquadTemplate* SquadTemplateLibrary::Find(
    const std::string& id) const {
    for (const auto& t : templates) {
        if (t.id == id) {
            return &t;
        }
    }
    return nullptr;
}

std::vector<const SquadTemplate*> SquadTemplateLibrary::SortedByCost()
    const {
    std::vector<const SquadTemplate*> out;
    out.reserve(templates.size());
    for (const auto& t : templates) {
        out.push_back(&t);
    }
    std::sort(out.begin(), out.end(),
              [](const SquadTemplate* a, const SquadTemplate* b) {
                  if (a->cost != b->cost) {
                      return a->cost < b->cost;
                  }
                  return a->id < b->id;
              });
    return out;
}

BudgetedBuildResult BudgetedBuild(
    const SquadTemplateLibrary& library, int budget,
    const std::vector<std::string>& orderedWishlist) {
    BudgetedBuildResult r;
    const bool unlimited = (budget == 0); // 0 = 無上限（保回歸）
    int remaining = budget;
    for (const std::string& wishId : orderedWishlist) {
        const SquadTemplate* tpl = library.Find(wishId);
        if (!tpl) {
            r.skipped.push_back(wishId); // 查無模板：無法定價 → 跳過
            continue;
        }
        if (unlimited || tpl->cost <= remaining) {
            r.built.push_back(tpl);
            r.spent += tpl->cost;
            if (!unlimited) {
                remaining -= tpl->cost;
            }
        } else {
            // 超支跳過（非降人數）：人數是模板的一部分，模板是原子單位
            r.skipped.push_back(wishId);
        }
    }
    return r;
}

} // namespace Gameplay
} // namespace Potato
