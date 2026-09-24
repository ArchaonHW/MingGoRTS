#pragma once

#include "Gameplay/BattleController.h"
#include "Gameplay/BattleRecorder.h"
#include "Gameplay/Roster.h"

#include <cstdint>
#include <string>

namespace Potato {
namespace Gameplay {

class LedgerChain;
class NarrativePack;

/**
 * 史官戰報組裝器（HistorianReport）——N-1 帳本文法核心。
 *
 * 「帳本是失真地圖」：BattleRecorder 記下全部事件（完整帳），
 * 本組裝器只書寫其中值得入史的事件（節選帳），其餘記入省略計數。
 * 每份報告文末必帶「本報告省略 N 項」——回放即審計工具，
 * 記錄與書寫是同一個機制。
 *
 * 用法：
 *   HistorianInput in;
 *   in.battleName = "斷橋之役";
 *   in.outcome = battle.GetOutcome();
 *   in.elapsedSec = battle.GetElapsed();
 *   in.recorder = &recorder;
 *   in.roster = &roster;
 *   HistorianReport r = ComposeHistorianReport(in);
 *   // r.text 逐字敲出；r.omittedCount 供 UI/審計
 */

struct HistorianInput {
    std::string battleName = "斷橋之役";
    BattleOutcome outcome = BattleOutcome::Ongoing;
    float elapsedSec = 0.0f;
    const BattleRecorder* recorder = nullptr; // 無記錄 → omitted=0
    const Roster* roster = nullptr;           // 無名冊 → 略過名冊句
    int playerTeam = 0;
    int enemyTeam = 1;
    // N-3 帳本外洩：敵軍針對的我軍慣用 trigger 名（如 "EnemyInRange"）；
    // 非空 → 名冊句後、省略計數前插入判詞行「彼之陣法…」
    std::string counteredHabit;
    // L-3 查帳：非空且有帳 → 判詞行後插入試算段——
    // 平衡產「借貸相符」、斷鏈/不健全產「墨跡未乾」存疑句
    const LedgerChain* ledger = nullptr;
    // L-8 確信：>0 時查帳段末附 seeded 抽驗結論（相符/存疑/拒絕）。
    // seed==0 → 以帳簿 RootHash 自引種；<=0 → 維持 L-6 前句式
    uint64_t assuranceSeed = 0;
    int assuranceSample = 3;
    // E-4 和平語域：非空 → 無戰章節組裝（不經戰鬥狀態機的章節
    // 同樣被書寫不留白）。呼叫端傳中文路徑名（談判/嚇阻/顛覆）
    // ——Gameplay 層不回依賴 Campaign 的 NoBattlePath enum。
    // 失敗/中計不走此徑（仍進戰鬥語域）。
    std::string peacePathZh;
    // 敵將名（去向句用）；空 → 「敵將」泛稱
    std::string peaceGeneral;
    // C-2 敘事包：非空 → 碎片從 pack 取（zh-TW），
    // 查無 id 回退內建字串；pack 的 LookupMisses 計未命中數
    const class NarrativePack* pack = nullptr;
};

struct HistorianReport {
    std::string text;          // 組裝全文（史官體）
    int narratedCount = 0;     // 被書寫入報的事件數
    int omittedCount = 0;      // 記錄了但未書寫的事件數
    int routCount = 0;         // 潰逃起數
    int playerLosses = 0;      // 我軍殲滅隊數
    int enemyLosses = 0;       // 敵軍殲滅隊數
    int neutralLosses = 0;     // 隊別不明的殲滅（roster 查無）
    int revealCount = 0;       // 情報揭露次數
    int interventionCount = 0; // CP 強令次數
};

HistorianReport ComposeHistorianReport(const HistorianInput& in);

} // namespace Gameplay
} // namespace Potato
