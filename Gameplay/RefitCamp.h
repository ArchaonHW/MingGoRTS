#pragma once

#include "Core/CoreTypes.h"
#include "MathUtils/Vector2.h"
#include "Serialization/JsonParser.h"
#include "Squad.h"

#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

class BattleController;
class Roster;
class SquadTemplateLibrary;
struct PostBattleReport;

/**
 * 整補營（RefitCamp）——G-9 戰鬥間 meta 層
 *
 * 跨場持續的「營地狀態」：G-7 結算的傷兵/戰利品/遺物在這裡
 * 變成可操作資產——傷兵恢復、模板招募（G-6）、遺物配備。
 * 營地自持戰利品帳（DepositLoot 從單場 BattleResources 轉存），
 * 本體可 SaveToFile 跨 session 保存。
 *
 * JSON schema: potato.refit_camp/1
 *
 * 用法：
 *   RefitCamp camp;
 *   camp.EnrollUnit(...);                    // 首戰前建軍
 *   ... 戰鬥 ...  PostBattle::Settle ...
 *   camp.DepositLoot(res.GetLoot(0));        // 戰利品轉存
 *   camp.Absorb(report, roster, 0);          // 傷亡入池、遺物入庫
 *   camp.HealWounded(camp.GetLoot());        // 醫治
 *   camp.Recruit(library, "infantry_line");  // 招募
 *   camp.Deploy(battle2, 0, positions, &library); // 下一場重建（套模板 stats）
 */
struct VeteranUnit {
    std::string squadName;              // 沿用原隊名（敘事連續性）
    std::string templateId;             // 招募來源（"" = 初始單位）
    UnitClass unitClass = UnitClass::Infantry;
    int members = 0;                    // 現有兵力（傷兵不算）
    int wounded = 0;                    // 傷兵池（可恢復）
    int maxMembers = 0;                 // 編制上限
    std::string captainName;            // 具名隊長（接 Roster）
    std::vector<std::string> relics;    // 配備的遺物
};

class RefitCamp {
public:
    // ---- 帳務 ----
    void DepositLoot(int points) { if (points > 0) loot += points; }
    int GetLoot() const { return loot; }

    // ---- 建軍 ----
    // 初始單位登記（首戰前）；wounded 由戰後 Absorb 寫入
    void EnrollUnit(const VeteranUnit& unit) { units.push_back(unit); }
    const std::vector<VeteranUnit>& GetUnits() const { return units; }
    const VeteranUnit* FindUnit(const std::string& squadName) const;

    // ---- 戰後吸收（G-7 對接）----
    // report 中屬於 team 的隊：存活→members/wounded 記回、
    // 被殲滅→移出常備軍（隊長已入冊）；report.relics → 庫存
    void Absorb(const PostBattleReport& report, const Roster& roster,
                int team);

    // ---- 整補 ----
    // 傷兵恢復：每 1 戰利品回 1 名（不超 maxMembers）；
    // maxSpend 為花費上限，回傳實際恢復人數
    int HealWounded(int maxSpend);
    // 招募：花 template.cost 建新滿編單位；查無模板/戰利品不足回 false
    bool Recruit(const SquadTemplateLibrary& library,
                 const std::string& templateId);
    // 遺物配備：從庫存移到單位身上；庫存沒有/查無單位回 false
    bool AssignRelic(const std::string& squadName,
                     const std::string& relic);
    const std::vector<std::string>& Inventory() const { return inventory; }

    // ---- 出戰 ----
    // 依序 CreateSquad 重建各單位（members 保留現兵力）；
    // positions 不足時多餘單位排在最後一點往 y+ 方向排開。
    // library 非空時：招募單位（templateId 非空且查得到）套用模板
    // stats（speed/engage_range/damage/stamina）；查無或 nullptr
    // 則維持 Squad 預設。unitClass 一律以單位自身記錄為準。
    std::vector<Squad*> Deploy(BattleController& battle, int team,
                               const std::vector<Vector2>& positions,
                               const SquadTemplateLibrary* library = nullptr);

    // ---- 存檔 ----
    // ToJson/FromJson 產生/消化與檔案同構的 JsonValue——
    // CampaignState 聚合存檔時以此嵌入子文件
    JsonValue ToJson() const;
    bool FromJson(const JsonValue& j);
    bool SaveToFile(const std::string& path) const;
    bool LoadFromFile(const std::string& path);

private:
    int loot = 0;
    std::vector<VeteranUnit> units;
    std::vector<std::string> inventory; // 未配備的遺物
};

} // namespace Gameplay
} // namespace Potato
