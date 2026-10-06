#pragma once

#include "Core/CoreTypes.h"

#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

class BattleController;
class BattleRecorder;
class BattleResources;
class Roster;

/**
 * 戰後流轉（PostBattle）——G-7 M&B 式結算
 *
 * 戰鬥結束後把「發生過什麼」轉成可持續的 meta 資產：
 *   傷亡分類 —— 每隊 lost 拆成 傷兵（可恢復）/ 陣亡（永久），
 *               傷兵比例可配置（M&B 的 wounded vs killed）
 *   戰利品   —— 殲滅敵隊得點數，入帳 BattleResources.loot；
 *               陣亡敵方隊長的遺物列入拾獲清單
 *   回放     —— 結算事件以 AddRecord 補記進 BattleRecorder，
 *               回放時可見戰後一拍
 *
 * 用法：
 *   PostBattle pb;
 *   pb.SetWoundedRatio(0.6f);
 *   auto report = pb.Settle(battle, roster, 0); // 0 = 勝方隊號
 *   pb.ApplyLoot(res, 0, report);
 *   pb.RecordTo(report, recorder, battle.GetElapsed());
 */
struct SquadCasualty {
    std::string squadName;
    int team = 0;
    int lost = 0;      // maxMembers - members
    int wounded = 0;   // 可恢復（傷兵）
    int dead = 0;      // 永久損失（陣亡）
    bool eliminated = false;
};

struct PostBattleReport {
    int winnerTeam = -1;               // -1 = Draw/Ongoing
    std::vector<SquadCasualty> casualties;
    std::vector<std::string> relics;   // 拾獲的敵方遺物
    int lootPoints = 0;                // 戰利品點數（依殲滅數計）

    // 某隊的傷亡合計（下一場整補的依據）
    int TotalLost(int team) const;
    int TotalWounded(int team) const;
    int TotalDead(int team) const;
};

class PostBattle {
public:
    // woundedRatio：傷亡中列為傷兵的比例（0~1，預設 0.6）
    void SetWoundedRatio(float r);
    float GetWoundedRatio() const { return woundedRatio; }

    // 每殲滅一支敵隊的戰利品點數（預設 2）
    void SetLootPerEliminated(int n) { lootPerEliminated = n > 0 ? n : 0; }
    int GetLootPerEliminated() const { return lootPerEliminated; }

    // 從戰鬥終態產生報告。battle 需已結束（outcome != Ongoing）；
    // winnerTeam 傳入勝方隊號（Draw 傳 -1 → 無戰利品）。
    PostBattleReport Settle(const BattleController& battle,
                            const Roster& roster,
                            int winnerTeam) const;

    // 戰利品入帳（重複呼叫會重複入帳——呼叫方負責只結算一次）
    void ApplyLoot(BattleResources& res, int team,
                   const PostBattleReport& report) const;

    // 把結算結果補記進回放（戰報最後一拍）
    void RecordTo(const PostBattleReport& report,
                  BattleRecorder& recorder, float t) const;

private:
    float woundedRatio = 0.6f;
    int lootPerEliminated = 2;
};

} // namespace Gameplay
} // namespace Potato
