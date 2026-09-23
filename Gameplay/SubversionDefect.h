#pragma once

// E-3 顛覆與內應——敵軍小隊開場倒戈的機械兌現。
// NoBattleResolver（Campaign 層）判定顛覆成功後只產出結果；
// 本函式把「內應」變成戰鬥層事實——確定性、無亂數、可無頭測試。
//
// 呼叫端必須在 Deployment 階段呼叫（BeginExecution 前）——
// 倒戈隊要補編 doctrine，AssignDoctrine 僅部署階段可用。
//
// 倒戈選取：士氣升冪 → 兵員升冪 → 名稱升冪（最動搖者先被
// 策反，排序全確定性、與迭代序/指標無關）。
// 全數倒戈合法——首個 CheckOutcome tick 即 Victory，
// 「不戰而勝」是湧現結果而非捷徑。

namespace Potato {
namespace Gameplay {

class BattleController;

// 把至多 maxCount 支 enemyTeam 存活小隊翻成 playerTeam。
// 回傳實際倒戈數（0 = 無隊可倒/無額度）。幂等責任在呼叫端。
int ApplyDefection(BattleController& battle,
                   int enemyTeam, int playerTeam, int maxCount);

} // namespace Gameplay
} // namespace Potato
