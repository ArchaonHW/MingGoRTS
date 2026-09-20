#include "SquadTemplate.h"
#include "BattleController.h"
#include "Serialization/JsonParser.h"

#include <algorithm>
#include <cctype>
#include <cmath>
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

// 非負浮點欄位：缺欄 → 預設；非有限 → 預設+警告；負值 → 夾 0+警告
float ReadNonNeg(const JsonValue& v, float def, const char* field,
                 std::vector<std::string>& warnings) {
    const double raw = v.AsNumber(def);
    if (!std::isfinite(raw)) {
        warnings.push_back(std::string(field) + " 非有限值，用預設");
        return def;
    }
    if (raw < 0.0) {
        warnings.push_back(std::string(field) + " 為負，夾到 0");
        return 0.0f;
    }
    return static_cast<float>(raw);
}
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
    // 先重置：失敗路徑留下的是乾淨預設物件，不殘留上次載入的值
    *this = SquadTemplate();

    JsonValue root;
    if (!JsonValue::ParseOk(json, root) || !root.IsObject()) {
        return false;
    }
    // schema 驗證：不符/缺欄一律拒絕，不產生半成品
    if (root["schema"].AsString() != kSchema) {
        return false;
    }

    id = root["id"].AsString();
    name = root["name"].AsString(id); // 缺名用 id 頂替
    if (id.empty() && name.empty()) {
        // 可載入但無名可建——Instantiate 會拒絕（patch：無匿名隊）
        warnings.push_back("模板缺 id 與 name，Instantiate 將回 nullptr");
    }

    // members：double 讀取，非有限 → 預設；越界 → 夾進 [1,100000]
    {
        const double raw = root["members"].AsNumber(10.0);
        if (!std::isfinite(raw)) {
            warnings.push_back("members 非有限值，用預設 10");
            members = 10;
        } else {
            if (raw <= 0.0) {
                warnings.push_back("members<=0，夾到 1");
            } else if (raw > 100000.0) {
                warnings.push_back("members 超過 100000，夾到上限");
            }
            members = static_cast<int>(std::clamp(raw, 1.0, 100000.0));
        }
    }

    // cost：負值 → 0+警告；非有限 → 0+警告；上限防 int 溢位
    {
        const double raw = root["cost"].AsNumber(0.0);
        if (!std::isfinite(raw)) {
            warnings.push_back("cost 非有限值，用預設 0");
            cost = 0;
        } else if (raw < 0.0) {
            warnings.push_back("cost 為負，夾到 0");
            cost = 0;
        } else {
            cost = (raw > 2147483647.0) ? 2147483647
                                        : static_cast<int>(raw);
        }
    }

    // unit_class：無效字串 → Infantry + 警告（EnemyGeneral 慣例）；
    // 缺欄走預設不算警告；非字串非空值（數字/bool/陣列）→ 警告
    {
        const JsonValue& uc = root["unit_class"];
        if (uc.IsString()) {
            const std::string ucs = uc.AsString();
            bool ok = false;
            unitClass = UnitClassFromString(ucs, &ok);
            if (!ok && !ucs.empty()) {
                warnings.push_back("未知 unit_class \"" + ucs +
                                   "\"，降為 infantry");
            }
        } else if (!uc.IsNull()) {
            warnings.push_back("unit_class 非字串，忽略（維持 infantry）");
        }
    }

    // stats 子物件：缺子欄位 → Squad 預設值，不 fail；負值/非有限 → 夾+警告
    const JsonValue& stats = root["stats"];
    speed = ReadNonNeg(stats["speed"], 2.0f, "stats.speed", warnings);
    engageRange = ReadNonNeg(stats["engage_range"], 1.5f,
                             "stats.engage_range", warnings);
    damagePerMember = ReadNonNeg(stats["damage_per_member"], 0.05f,
                                 "stats.damage_per_member", warnings);

    // stamina 整段可選；有寫則五欄逐讀，缺一欄用 Squad 預設；
    // 存在但不是物件 → 忽略 + 警告
    const JsonValue& st = stats["stamina"];
    if (st.IsObject()) {
        hasStamina = true;
        staminaDrainMove = ReadNonNeg(st["drain_move"], kDefaultDrainMove,
                                      "stats.stamina.drain_move", warnings);
        staminaDrainCombat =
            ReadNonNeg(st["drain_combat"], kDefaultDrainCombat,
                       "stats.stamina.drain_combat", warnings);
        staminaRegen = ReadNonNeg(st["regen"], kDefaultRegen,
                                  "stats.stamina.regen", warnings);
        staminaPenaltyMul =
            ReadNonNeg(st["penalty_mul"], kDefaultPenaltyMul,
                       "stats.stamina.penalty_mul", warnings);
        const double th = st["threshold"].AsNumber(kDefaultThreshold);
        if (!std::isfinite(th)) {
            warnings.push_back("stats.stamina.threshold 非有限值，用預設");
            staminaThreshold = kDefaultThreshold;
        } else {
            if (th < 0.0 || th > 1.0) {
                warnings.push_back(
                    "stats.stamina.threshold 越界 [0,1]，夾回");
            }
            staminaThreshold =
                static_cast<float>(std::clamp(th, 0.0, 1.0));
        }
    } else if (!st.IsNull()) {
        warnings.push_back("stats.stamina 非物件，整段忽略");
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
    if (finalName.empty()) {
        return nullptr; // 不建匿名隊（id/name/override 全空）
    }
    Squad* s = battle.CreateSquad(finalName, team, pos, members);
    if (!s) {
        return nullptr;
    }
    ApplyStats(s);
    return s;
}

void SquadTemplate::ApplyStats(Squad* squad) const {
    if (!squad) {
        return;
    }
    // 逐 setter 套屬性——不改 CreateSquad 簽名，引擎面零侵入
    squad->SetUnitClass(unitClass);
    squad->SetSpeed(speed);
    squad->SetEngageRange(engageRange);
    squad->SetDamagePerMember(damagePerMember);
    if (hasStamina) {
        squad->SetStaminaParams(staminaDrainMove, staminaDrainCombat,
                                staminaRegen, staminaThreshold,
                                staminaPenaltyMul);
    }
}

size_t SquadTemplateLibrary::LoadDir(const std::string& dir) {
    if (dir.empty()) {
        return 0;
    }
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
        if (!tpl.LoadFromString(ss.str())) {
            continue;
        }
        // 同 id 撞車：後載覆蓋先載（keep latest）
        auto it = std::find_if(templates.begin(), templates.end(),
                               [&](const SquadTemplate& t) {
                                   return t.id == tpl.id;
                               });
        if (it != templates.end()) {
            *it = tpl;
        } else {
            templates.push_back(tpl);
        }
        ++loaded;
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
    // 非正預算 = 無上限（spec 的 budget=0 保回歸路徑 + 負值放行）
    const bool unlimited = (budget <= 0);
    int remaining = budget;
    for (const std::string& wishId : orderedWishlist) {
        const SquadTemplate* tpl = library.Find(wishId);
        if (!tpl) {
            r.skipped.push_back(wishId); // 查無模板：無法定價 → 跳過
            r.skipReasons.push_back("unknown_id");
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
            r.skipReasons.push_back("over_budget");
        }
    }
    return r;
}

} // namespace Gameplay
} // namespace Potato
