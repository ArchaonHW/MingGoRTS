#pragma once

// E-2 談判與嚇阻決算——無戰判定失敗/中計後的「敵獲首波優勢」兌現。
// NoBattleResolver（Campaign 層）只產出 enemyAdvantage 旗標；
// 本函式是它在戰鬥層的機械兌現——確定性、無亂數、可無頭測試。
// 呼叫端在 BeginExecution 前套用一次（部署階段修正才影響開局）。

namespace Potato {
namespace Gameplay {

class BattleController;
class BattleResources;

// 首波優勢兌現（提案配方）：
//   敵隊 +2 CP +2 intel——先機 = 更多介入籌碼與資訊
//   我軍每小隊士氣 −0.10，夾到 ≥0.10——敗談後軍心浮動但不即潰
// 零我軍小隊合法（敵資源照加）；幂等由呼叫端保證（只套一次）。
void ApplyFirstWaveAdvantage(BattleController& battle,
                             BattleResources& res,
                             int enemyTeam, int playerTeam);

} // namespace Gameplay
} // namespace Potato
