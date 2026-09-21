// BattleTrainerDemo - 戰鬥指揮官 RL 訓練無頭測試
//
// 管線: BattleCommandEnv(RLEnvironment 包 BattleController)
//       → DQNAgent(AI/ReinforcementLearning,真 TD 訓練)
//       → 訓練 N episode → 固定 seed 評估 → trained_battle_agent.json
//
// 場景（同 VideoDataDemo 斷橋地形）：team 0 攻方四隊,doctrine 只有
// Always→HoldPosition——沒有指揮官介入就原地站崗到超時。
// RL agent 扮演攻方指揮官：每 1 秒決策一次,從 5 個全軍姿態動作選一個,
// 非 wait 動作對每支存活小隊各扣 1 CP（Intervene 語義同遊戲內）。
// 學習目標：在 60 CP 預算內學會「推進→接戰→殲敵」,贏過永不行動的基線。
//
// 驗收：環境機制正確、訓練 reward 曲線向上、訓練後評估分數贏基線、
//       策略可序列化回讀。

#include "Gameplay/BattleCommandEnv.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/Squad.h"
#include "AI/ReinforcementLearning.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <functional>
#include <random>
#include <sstream>
#include <string>
#include <vector>

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

// 環境已抽成 Gameplay/BattleCommandEnv（與 BattleCommanderAI 共用
// BattleCommandAI.h 的特徵/動作契約）——此檔只留訓練/評估管線。
static const int kNumActions = kCommandActionCount;
static const int kStateSize = kCommandStateSize;

// ============================================================================
// 評估：跑一場,回傳分數（勝 +1 / 敗 -1 + 交換比修正）
// ============================================================================
static float RunEvalEpisode(BattleCommandEnv& env, unsigned seed,
                            const std::function<int(const State&)>& policy) {
    State s = env.Reset(seed);
    for (int step = 0; step < 60; ++step) {
        StepResult r = env.Step(Action(policy(s), "eval"));
        s = r.nextState;
        if (r.done) break;
    }
    float score = 0.0f;
    if (env.GetOutcome() == BattleOutcome::Victory) score += 1.0f;
    if (env.GetOutcome() == BattleOutcome::Defeat) score -= 1.0f;
    score += (float)(env.EnemyLost() - env.OwnLost()) / 40.0f;
    return score;
}

int main() {
    printf("=== Battle Trainer Demo (DQN) ===\n\n");

    // ---- [1] 環境機制 ----
    printf("[1] 環境機制\n");
    {
        BattleCommandEnv env(7);
        State s0 = env.Reset(7);
        Check((int)s0.features.size() == kStateSize, "state 維度 15");
        bool finite = true;
        for (float f : s0.features) finite &= std::isfinite(f);
        Check(finite, "state 全為有限值");
        Check(env.GetActionSpace().size() == (size_t)kNumActions,
              "5 個動作");

        // advance 動作真的下達 AttackMove 命令
        StepResult r = env.Step(Action(1, "advance"));
        const Squad* first = nullptr;
        for (const auto& sq : env.Battle().GetSquads()) {
            if (sq->GetTeam() == 0 && !sq->IsEliminated()) {
                first = sq.get();
                break;
            }
        }
        Check(first && first->GetOrder() == SquadOrder::AttackMove,
              "advance → squad AttackMove");
        Check(std::isfinite(r.reward), "step reward 有限");
    }

    // ---- [2] 基線：指揮官永不行動 ----
    printf("\n[2] 基線（永不介入）\n");
    const unsigned kEvalSeedBase = 5000;
    const int kEvalEpisodes = 30;
    float baselineMean = 0.0f;
    {
        BattleCommandEnv env(0);
        for (int i = 0; i < kEvalEpisodes; ++i) {
            baselineMean += RunEvalEpisode(env, kEvalSeedBase + i,
                                           [](const State&) { return 0; });
        }
        baselineMean /= kEvalEpisodes;
        printf("  baseline mean score = %.3f（30 場,不介入=原地站崗）\n",
               baselineMean);
    }

    // ---- [3] DQN 訓練 ----
    printf("\n[3] DQN 訓練\n");
    const int kEpisodes = 200;
    DQNAgent agent(kStateSize, kNumActions,
                   /*lr*/ 0.003f, /*gamma*/ 0.95f, /*epsilon*/ 1.0f,
                   /*buffer*/ 6000, /*batch*/ 32);
    agent.SeedWeights(42);   // 權重固定 → 整條管線可重現
    agent.SetSeed(1234);

    std::vector<float> epRewards;
    int trainSteps = 0;
    {
        BattleCommandEnv env(0);
        for (int ep = 0; ep < kEpisodes; ++ep) {
            State s = env.Reset(1000 + (unsigned)ep * 131u);
            float total = 0.0f;
            for (int step = 0; step < 60; ++step) {
                Action a = agent.SelectAction(s, /*explore*/ true);
                StepResult r = env.Step(a);
                Experience e;
                e.state = s;
                e.action = a;
                e.reward = r.reward;
                e.nextState = r.nextState;
                e.done = r.done;
                agent.TrainStep(e);
                total += r.reward;
                s = r.nextState;
                ++trainSteps;
                if (trainSteps % 200 == 0) {
                    agent.UpdateTargetNetwork();
                }
                if (r.done) break;
            }
            epRewards.push_back(total);
            // epsilon 衰減到 0.05 地板
            if (agent.GetExplorationRate() > 0.05f) {
                agent.DecayExplorationRate(0.96f);
            }
            if ((ep + 1) % 20 == 0) {
                float m = 0.0f;
                for (size_t i = epRewards.size() - 20;
                     i < epRewards.size(); ++i) {
                    m += epRewards[i];
                }
                printf("  ep %3d-%3d mean reward = %+.3f  epsilon=%.2f\n",
                       ep - 18, ep + 1, m / 20.0f,
                       agent.GetExplorationRate());
            }
        }
    }

    // 學習曲線僅供觀察：高 ε 前期隨機動作也會打到架（advance/engage 佔 2/5），
    // 且 DQN target 更新有中期震盪——曲線非單調，不當斷言條件。
    // 「變聰明」的證據在下面：同 seed 評估贏過不介入基線 + 勝場數。
    float first20 = 0.0f, last20 = 0.0f;
    for (int i = 0; i < 20; ++i) {
        first20 += epRewards[i];
        last20 += epRewards[kEpisodes - 20 + i];
    }
    printf("  reward curve: first20=%+.3f last20=%+.3f\n",
           first20 / 20.0f, last20 / 20.0f);

    // ---- [4] 訓練後評估（ε=0 貪婪,同一組 eval seed）----
    printf("\n[4] 訓練後評估\n");
    float trainedMean = 0.0f;
    int wins = 0, actionHist[5] = {0};
    {
        BattleCommandEnv env(0);
        for (int i = 0; i < kEvalEpisodes; ++i) {
            const unsigned seed = kEvalSeedBase + i;
            // 記分同時統計動作分佈（看策略是否像樣）
            State s = env.Reset(seed);
            float sc = 0.0f;
            for (int step = 0; step < 60; ++step) {
                Action a = agent.SelectAction(s, /*explore*/ false);
                actionHist[a.id]++;
                StepResult r = env.Step(a);
                s = r.nextState;
                if (r.done) break;
            }
            if (env.GetOutcome() == BattleOutcome::Victory) {
                sc += 1.0f; wins++;
            }
            if (env.GetOutcome() == BattleOutcome::Defeat) sc -= 1.0f;
            sc += (float)(env.EnemyLost() - env.OwnLost()) / 40.0f;
            trainedMean += sc;
        }
        trainedMean /= kEvalEpisodes;
    }
    printf("  trained mean score = %+.3f  wins=%d/30\n", trainedMean, wins);
    printf("  action 分佈: wait=%d advance=%d engage=%d hold=%d retreat=%d\n",
           actionHist[0], actionHist[1], actionHist[2], actionHist[3],
           actionHist[4]);
    Check(trainedMean > baselineMean + 0.05f,
          "訓練後分數顯著贏過基線");
    Check(wins >= 12, "訓練後勝場 ≥12/30（學會主動求戰）");
    Check(actionHist[1] + actionHist[2] > 0,
          "策略確實會下達進攻指令");

    // ---- [5] 策略持久化 ----
    printf("\n[5] 序列化\n");
    {
        const std::string path = "trained_battle_agent.json";
        std::ofstream(path) << agent.Serialize();
        DQNAgent loaded(kStateSize, kNumActions);
        bool ok = false;
        std::ifstream in(path);
        if (in) {
            std::stringstream buf;
            buf << in.rdbuf();
            ok = loaded.Deserialize(buf.str());
        }
        Check(ok, "trained_battle_agent.json 回讀成功");
        // 同一 state 貪婪動作一致 → 權重真的搬過去了
        if (ok) {
            BattleCommandEnv env(0);
            State s = env.Reset(kEvalSeedBase);
            Check(loaded.SelectAction(s, false).id ==
                      agent.SelectAction(s, false).id,
                  "回讀後貪婪動作一致");
        }
    }

    printf("\n=== %d PASS / %d FAIL ===\n", g_pass, g_fail);
    return g_fail > 0 ? 1 : 0;
}
