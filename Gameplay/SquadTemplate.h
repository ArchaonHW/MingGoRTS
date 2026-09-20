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
 * - schema 字串不符 / JSON 壞 → LoadFromFile/LoadFromString 回 false；
 *   失敗時物件先被重置為預設值，不產生半成品
 * - 缺 stats 子欄位 → 用 Squad 建構預設值（不 fail）
 * - stamina 整段可選；有寫則五欄逐讀，缺一欄用 Squad 預設；
 *   stamina 存在但不是物件 → 忽略並記 warnings
 * - unit_class 無效字串或非字串非空值 → Infantry + 記入 warnings
 *   （比照 EnemyGeneral 的 UnitClassFromString 慣例）
 * - 越界數值（members/cost/速度/傷害/疲勞參數）→ 夾到合法範圍 +
 *   記 warnings；warnings 是給呼叫端呈現的機器可讀降級記錄
 * - morale 刻意不進 schema v1：模板只管編制硬參數，士氣是戰場
 *   動態狀態，歸 SageCommand/doctrine 層管
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
    // CreateSquad 一致）。回 nullptr 的情況：CreateSquad 失敗，或
    // 三者皆空（不建無名隊）。
    Squad* Instantiate(BattleController& battle, const std::string& name,
                       int team, const Vector2& pos) const;

    // 只把 stats 套到既有 squad（兵種/速度/接戰距離/單兵傷害/疲勞參數），
    // 不動 members/name——RefitCamp::Deploy 重建招募單位時重用，
    // 與 Instantiate 共用同一套 stamping 邏輯
    void ApplyStats(Squad* squad) const;
};

/**
 * 模板庫：掃描目錄載入多模板，依 id 查找 / 依 cost 排序查詢
 */
class SquadTemplateLibrary {
public:
    // 掃描 dir 下所有 *.json 逐檔載入；壞檔/schema 不符跳過不中止。
    // 目錄本身也吃逐層 ../ 容錯；dir 空字串 → 回 0。
    // 【累積語義】不清空既有內容——呼叫端可連續 LoadDir 多個目錄
    // （DuanqiaoPlayable 就是載 squads 再載 templates）。同 id 撞車
    // 時後載入的覆蓋先載入的（keep latest）。回傳本次成功解析的檔案數。
    size_t LoadDir(const std::string& dir);

    // 直接加一筆（測試/程式化模板用）；同 id 不除重，Find 取先載入者
    void Add(const SquadTemplate& tpl) { templates.push_back(tpl); }
    void Clear() { templates.clear(); }
    size_t Size() const { return templates.size(); }

    // 【生命週期注意】All/Find/SortedByCost 回傳的參照/指標指向
    // templates 內部元素——Add/LoadDir/Clear 可能觸發 vector
    // 重新配置使舊指標懸空，取用後勿跨變動沿用。
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
    std::vector<std::string> skipped;      // 被跳過的 template id
    // C-3：skipped[i] 的原因，與 skipped 逐項對齊——
    // "unknown_id"（查無模板）/ "over_budget"（超支）
    std::vector<std::string> skipReasons;
    int spent = 0;                         // built 的總成本
};

// 依序檢查 orderedWishlist 每個 template id：cost ≤ 剩餘預算才入
// built 並扣帳；單項超支 → 記入 skipped 後繼續評估後續項；
// 查無的 id 同樣記 skipped（無法定價 = 不能建）。
//
// 【budget 哨兵】budget <= 0 = 無上限：全部入 built（spec 的
// budget=0 保回歸路徑，負值同樣放行）。注意與
// BattleController::CreateSquadFromTemplate 的差異是有意的：
// 那是逐隊扣帳 API，budget 是活钱包——budget<0 才無限、budget=0
// 是「真沒錢」只建得起免費模板；此處是願望清單篩選 API，
// 非正預算一律視為「不追蹤預算」。
BudgetedBuildResult BudgetedBuild(
    const SquadTemplateLibrary& library, int budget,
    const std::vector<std::string>& orderedWishlist);

} // namespace Gameplay
} // namespace Potato
