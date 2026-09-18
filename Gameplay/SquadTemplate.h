#pragma once

#include "Core/CoreTypes.h"
#include "MathUtils/Vector2.h"
#include "Squad.h"

#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

class BattleController;

/**
 * 小隊編制模板（G-6，HOI4 division designer 的 squad 粒度版）：
 * squad 的組成（人數/兵種/屬性/成本）定義成 JSON 模板檔，
 * 編成階段用模板建隊而非逐隊手寫參數；預算限制下配兵走
 * BudgetedBuild 的寫死降規則（超支跳過，不降人數）。
 *
 * JSON schema: potato.squad_template/1
 * {
 *   "schema": "potato.squad_template/1",
 *   "id": "infantry_line",
 *   "name": "步兵線列",
 *   "unit_class": "infantry",        // infantry / archer / cavalry
 *   "members": 30,
 *   "cost": 100,
 *   "stats": {
 *     "speed": 2.0,
 *     "engage_range": 1.5,
 *     "damage_per_member": 0.05,
 *     "stamina": {                    // 子物件整段可選
 *       "drain_move": 0.03,
 *       "drain_combat": 0.05,
 *       "regen": 0.06,
 *       "threshold": 0.3,
 *       "penalty_mul": 0.6
 *     }
 *   }
 * }
 *
 * 錯誤處理（寫死）：
 * - schema 字串不符 / JSON 壞 → LoadFromFile/LoadFromString 回 false，
 *   不產生半成品
 * - 缺 stats 子欄位 → 用 Squad 建構預設值（不 fail）
 * - stamina 整段可選；有寫則五欄逐讀，缺一欄用 Squad 預設
 * - unit_class 無效字串 → Infantry + 記入 warnings
 *   （比照 EnemyGeneral 的 UnitClassFromString 慣例）
 */
struct SquadTemplate {
    static constexpr const char* kSchema = "potato.squad_template/1";

    std::string id;
    std::string name;
    int members = 10;
    UnitClass unitClass = UnitClass::Infantry;
    int cost = 0;

    // stats.* 欄位；預設值對齊 Squad.cpp 建構
    float speed = 2.0f;
    float engageRange = 1.5f;
    float damagePerMember = 0.05f;

    // stamina 子物件：hasStamina=false 時 Instantiate 不呼叫
    // SetStaminaParams（保留 Squad 預設）
    bool hasStamina = false;
    float staminaDrainMove = 0.03f;
    float staminaDrainCombat = 0.05f;
    float staminaRegen = 0.06f;
    float staminaThreshold = 0.3f;
    float staminaPenaltyMul = 0.6f;

    // 載入警告（降級路徑記錄，如未知 unit_class；不影響載入成功與否）
    std::vector<std::string> warnings;

    // 逐層 ../ 容錯（CTest 工作目錄可能深至 build/bin/Release），
    // 比照 BattleMap::LoadFromFile
    bool LoadFromFile(const std::string& path);
    bool LoadFromString(const std::string& json);

    // 建隊：CreateSquad(name, team, pos, members) 後逐 setter 套屬性。
    // name 空字串 → 模板 name（再空 → id）。同名 squad 不擋（與
    // CreateSquad 一致）。失敗回 nullptr（CreateSquad 失敗時）。
    Squad* Instantiate(BattleController& battle, const std::string& name,
                       int team, const Vector2& pos) const;
};

/**
 * 模板庫：掃描目錄載入多模板，依 id 查找 / 依 cost 排序查詢
 */
class SquadTemplateLibrary {
public:
    // 掃描 dir 下所有 *.json 逐檔載入；壞檔/schema 不符跳過不中止。
    // 目錄本身也吃逐層 ../ 容錯。回傳成功載入的模板數。
    size_t LoadDir(const std::string& dir);

    // 直接加一筆（測試/程式化模板用）；同 id 不除重，Find 取先載入者
    void Add(const SquadTemplate& tpl) { templates.push_back(tpl); }
    void Clear() { templates.clear(); }
    size_t Size() const { return templates.size(); }
    const std::vector<SquadTemplate>& All() const { return templates; }

    const SquadTemplate* Find(const std::string& id) const;
    // cost 升冪；同 cost 依 id 排序讓結果穩定
    std::vector<const SquadTemplate*> SortedByCost() const;

private:
    std::vector<SquadTemplate> templates;
};

/**
 * 預算配兵結果（G-6 寫死規則：超支跳過，不降人數）
 */
struct BudgetedBuildResult {
    // 通過預算檢查的模板（順序同 wishlist）；指標指向 library 內部，
    // 生命週期隨 library，勿在 library 變動後沿用
    std::vector<const SquadTemplate*> built;
    std::vector<std::string> skipped;  // 被跳過的 template id
    int spent = 0;                     // built 的總成本
};

// 依序檢查 orderedWishlist 每個 template id：cost ≤ 剩餘預算才入
// built 並扣帳；單項超支 → 記入 skipped 後繼續評估後續項；
// 查無的 id 同樣記 skipped（無法定價 = 不能建）。
// budget=0 視為無上限：全部入 built（保回歸路徑）。
BudgetedBuildResult BudgetedBuild(
    const SquadTemplateLibrary& library, int budget,
    const std::vector<std::string>& orderedWishlist);

} // namespace Gameplay
} // namespace Potato
