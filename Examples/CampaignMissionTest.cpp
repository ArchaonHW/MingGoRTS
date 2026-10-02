#include "Gameplay/BattleController.h"
#include <iostream>
#include <limits>
using namespace Potato;
using namespace Potato::Gameplay;
static int failures = 0;
static void Check(bool value, const char *label) {
    std::cout << (value ? "PASS " : "FAIL ") << label << '\n';
    if (!value)
        ++failures;
}
int main() {
    BattleController rescue(24, 16, 1);
    auto *guard = rescue.CreateSquad("後衛", 0, {2, 2}, 20);
    auto *enemy = rescue.CreateSquad("追兵", 1, {20, 14}, 20);
    BattleController foreign(24, 16, 1);
    auto *other = foreign.CreateSquad("外部", 0, {2, 2}, 10);
    Check(!rescue.SetProtectionObjective(other, 2), "foreign squad rejected");
    Check(!rescue.SetProtectionObjective(enemy, 2), "enemy protection rejected");
    Check(!rescue.SetProtectionObjective(guard, std::numeric_limits<float>::quiet_NaN()),
          "invalid timer rejected");
    Check(rescue.SetProtectionObjective(guard, 2), "protection configured");
    rescue.BeginExecution();
    Check(!rescue.SetProtectionObjective(guard, 3), "objective cannot change during battle");
    rescue.Update(1);
    Check(rescue.GetOutcome() == BattleOutcome::Ongoing, "before rescue timer remains ongoing");
    rescue.Update(1);
    Check(rescue.GetOutcome() == BattleOutcome::Victory && !enemy->IsEliminated(),
          "timer victory without enemy annihilation");
    Check(!rescue.Withdraw(), "terminal victory cannot become defeat");
    BattleController lost(24, 16, 1);
    guard = lost.CreateSquad("後衛", 0, {2, 2}, 20);
    lost.CreateSquad("主軍", 0, {3, 2}, 20);
    lost.CreateSquad("追兵", 1, {20, 14}, 20);
    lost.SetProtectionObjective(guard, 2);
    lost.BeginExecution();
    guard->AdjustMorale(-2);
    lost.Update(2);
    Check(lost.GetOutcome() == BattleOutcome::Defeat, "protected rout takes priority over timer");
    BattleController killed(24, 16, 1);
    guard = killed.CreateSquad("後衛", 0, {2, 2}, 20);
    killed.CreateSquad("主軍", 0, {3, 2}, 20);
    killed.CreateSquad("追兵", 1, {20, 14}, 20);
    killed.SetProtectionObjective(guard, 2);
    killed.BeginExecution();
    guard->ApplyCasualties(20);
    killed.Update(2);
    Check(killed.GetOutcome() == BattleOutcome::Defeat,
          "protected death loses even with survivors");
    BattleController retreat(24, 16, 1);
    retreat.CreateSquad("主軍", 0, {2, 2}, 20);
    retreat.CreateSquad("敵軍", 1, {20, 14}, 20);
    Check(!retreat.Withdraw(), "deployment cannot settle withdrawal");
    retreat.BeginExecution();
    Check(retreat.Withdraw() && retreat.GetPhase() == BattlePhase::Resolution,
          "withdrawal settles once");
    Check(!retreat.Withdraw(), "duplicate withdrawal rejected");
    return failures ? 1 : 0;
}
