#pragma once

#include "Core/CoreTypes.h"

#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

class BattleController;
class Squad;

/**
 * 名冊（Roster）——T-8 具名單位與遺物
 *
 * 「隊長級以上才入冊」：每支小隊部署時登記一名具名隊長
 * （可帶一件遺物 relic）。小隊被殲滅 → 隊長陣亡，
 * 寫入名冊並可存成 JSON——戰後「哀悼/重建」一拍的資料來源。
 *
 * 用法：
 *   Roster roster;
 *   roster.Enroll(alpha, "老周", "captain", "斷刃");
 *   ... 每 tick roster.Update(battle) ...
 *   roster.SaveToFile("roster.json");
 */
struct RosterEntry {
    std::string name;      // 隊長姓名
    std::string rank;      // captain / veteran ...
    std::string squadName;
    int team = 0;
    bool alive = true;
    float deathTime = -1.0f;  // 陣亡的遊戲時間；-1 = 仍在世
    std::string relic;        // 遺物（掉落物，供戰後拾取/紀念）
    std::string art;          // 立繪 PNG 路徑（cards/art/...），UI 顯示用
};

class Roster {
public:
    // 部署階段登記隊長；rank 僅接受 "captain" 以上語義（自由字串）
    void Enroll(const Squad* squad, const std::string& name,
                const std::string& rank = "captain",
                const std::string& relic = "",
                const std::string& art = "");

    // 每 tick 呼叫：偵測小隊殲滅 → 記陣亡（重複呼叫安全）
    void Update(const BattleController& battle);

    const std::vector<RosterEntry>& GetEntries() const { return entries; }
    size_t DeadCount() const;
    size_t AliveCount() const { return entries.size() - DeadCount(); }

    // 存/讀 JSON（potato.roster/1）
    bool SaveToFile(const std::string& path) const;
    bool LoadFromFile(const std::string& path);

private:
    std::vector<RosterEntry> entries;
    std::vector<const Squad*> watched;   // 與 entries 平行
};

} // namespace Gameplay
} // namespace Potato
