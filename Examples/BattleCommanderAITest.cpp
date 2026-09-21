// BattleCommanderAITest - RL 指揮官契約層無頭測試
//
// 驗證（不跑長訓練——那是 BattleTrainerDemo/BattleTrainer 的活）：
//   [1] 契約：動作名表、env 狀態維度/動作空間、Reset 同 seed 可重現
//   [2] 動作套用：advance → AttackMove、hold、CP 消耗、wait 無副作用
//   [3] BattleCommanderAI：缺檔降級、序列化權重載入、Update 決策節奏
//   [4] 訓練-推論一致性：同一權重 env 評估與 commander 首動作一致

#include "Gameplay/BattleCommandAI.h"
#include "Gameplay/BattleCommandEnv.h"
#include "AI/ReinforcementLearning.h"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

using namespace Potato;
using namespace Potato::Gameplay;
using namespace Potato::AI;

static int g_pass = 0;
static int g_fail = 0;

static void Check(bool ok, const char* name) {
    if (ok) {
        g_pass++;
        printf("  [PASS] %s\n", name);
    } else {
        g_fail++;
        printf("  [FAIL] %s\n", name);
    }
}

static bool FeaturesEqual(const State& a, const State& b) {
    if (a.features.size() != b.features.size()) return false;
    for (size_t i = 0; i < a.features.size(); ++i) {
        if (a.features[i] != b.features[i]) return false;
    }
    return true;
}

int main() {
    printf("=== Battle Commander AI Test ===\n\n");

    // ---- [1] 契約 ----
    printf("[1] 動作/狀態契約\n");
    Check(std::string(CommandActionName(0)) == "wait", "動作0=wait");
    Check(std::string(CommandActionName(1)) == "advance", "動作1=advance");
    Check(std::string(CommandActionName(2)) == "engage", "動作2=engage");
    Check(std::string(CommandActionName(3)) == "hold", "動作3=hold");
    Check(std::string(CommandActionName(4)) == "retreat", "動作4=retreat");
    Check(std::string(CommandActionName(5)) == "unknown", "越界=unknown");
    Check(std::string(CommandActionName(-1)) == "unknown", "負值=unknown");
    {
        BattleCommandEnv envA(77), envB(77);
        State a = envA.Reset(77), b = envB.Reset(77);
        Check((int)a.features.size() == kCommandStateSize, "state 15 維");
        Check(FeaturesEqual(a, b), "同 seed Reset 特徵逐位一致");
        bool finite = true;
        for (float f : a.features) finite &= std::isfinite(f);
        Check(finite, "特徵全為有限值");
        Check(envA.GetActionSpace().size() == (size_t)kCommandActionCount,
              "動作空間 5 個");
        Check(envA.GetStateSize() == kCommandStateSize,
              "GetStateSize=15");
    }

    // ---- [2] 動作套用 ----
    printf("\n[2] 動作套用（ApplyCommandAction）\n");
    {
        BattleCommandEnv env(11);
        env.Reset(11);
        const int cpBefore = env.Battle().GetCommandPoints(0);
        env.Step(Action(1, "advance"));
        bool attackMove = false;
        for (const auto& sq : env.Battle().GetSquads()) {
            if (sq->GetTeam() == 0 && !sq->IsEliminated() &&
                sq->GetOrder() == SquadOrder::AttackMove) {
                attackMove = true;
            }
        }
        Check(attackMove, "advance → 攻方 AttackMove");
        Check(env.Battle().GetCommandPoints(0) < cpBefore,
              "介入消耗 CP");
        Check(env.GetLastAction() == 1, "lastAction 記錄");

        BattleCommandEnv env2(22);
        env2.Reset(22);
        const int cp2 = env2.Battle().GetCommandPoints(0);
        env2.Step(Action(0, "wait"));
        Check(env2.Battle().GetCommandPoints(0) == cp2,
              "wait 不扣 CP");
        env2.Step(Action(99, "bogus"));
        Check(env2.Battle().GetCommandPoints(0) == cp2,
              "未知動作無副作用");
    }

    // ---- [3] BattleCommanderAI ----
    printf("\n[3] BattleCommanderAI 載入/節奏\n");
    {
        BattleCommanderAI cmd;
        Check(!cmd.IsLoaded(), "未載入 → IsLoaded=false");
        Check(!cmd.LoadFromFile("__no_such_weights__.json"),
              "缺檔 LoadFromFile=false");
        Check(!cmd.IsLoaded(), "載入失敗後仍 false");

        // 塞一個確定性權重（未訓練也行——驗管線不驗品質）
        DQNAgent src(kCommandStateSize, kCommandActionCount);
        src.SeedWeights(42);
        src.SetSeed(7);
        const std::string path = "__test_commander_weights__.json";
        std::ofstream(path) << src.Serialize();
        Check(cmd.LoadFromFile(path), "權重檔載入成功");
        Check(cmd.IsLoaded(), "IsLoaded=true");

        BattleCommandEnv env(5);
        env.Reset(5);
        cmd.Reset(const_cast<BattleController&>(env.Battle()), 0);
        // 不足 1.0s 不決策
        cmd.Update(const_cast<BattleController&>(env.Battle()), 0.5f);
        Check(cmd.GetLastAction() == 0, "未滿 1s 不決策");
        // 滿一個決策間隔 → lastAction 落在合法範圍
        cmd.Update(const_cast<BattleController&>(env.Battle()), 0.6f);
        Check(cmd.GetLastAction() >= 0 &&
                  cmd.GetLastAction() < kCommandActionCount,
              "決策後 lastAction 合法");
        std::remove(path.c_str());
    }

    // ---- [4] 訓練-推論一致性：同權重 env policy 與 commander 同動作 ----
    printf("\n[4] 契約一致性（env policy vs commander 首動作）\n");
    {
        DQNAgent src(kCommandStateSize, kCommandActionCount);
        src.SeedWeights(99);
        src.SetSeed(3);
        const std::string path = "__test_commander_weights2__.json";
        std::ofstream(path) << src.Serialize();

        // env 路徑：Reset 後直接問 agent
        BattleCommandEnv env(5000);
        State s = env.Reset(5000);
        const int envAction = src.SelectAction(s, false).id;

        // commander 路徑：另一個同 seed env 的 battle,Reset 後 Update 1.0s+
        BattleCommanderAI cmd;
        cmd.LoadFromFile(path);
        BattleCommandEnv env2(5000);
        env2.Reset(5000);
        cmd.Reset(const_cast<BattleController&>(env2.Battle()), 0);
        cmd.Update(const_cast<BattleController&>(env2.Battle()), 1.0f);
        Check(cmd.GetLastAction() == envAction,
              "commander 首動作 == env 貪婪動作");
        std::remove(path.c_str());
    }

    printf("\n=== %d PASS / %d FAIL ===\n", g_pass, g_fail);
    return g_fail > 0 ? 1 : 0;
}
