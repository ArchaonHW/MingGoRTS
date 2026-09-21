// BattleTrainer - 戰鬥指揮官 RL 訓練 CLI（產出權重檔供遊戲內使用）
//
//   BattleTrainer [--episodes N] [--eval N] [--seed S] [--out FILE]
//                 [--weights-seed S] [--agent-seed S]
//
//   --episodes    訓練回合數（預設 200）
//   --eval        評估場數（預設 30）
//   --seed        訓練 episode seed 基底（預設 1000，每場 +131）
//   --out         權重輸出（預設 trained_battle_agent.json，
//                 DuanqiaoPlayable 的 AI 指揮官讀同名檔）
//   --weights-seed  DQN 權重初始化 seed（預設 42）
//   --agent-seed    DQN 探索 RNG seed（預設 1234）
//
// 管線與 BattleTrainerDemo 相同，只是把超參數開成命令列：
// BattleCommandEnv → DQNAgent → 訓練 → 固定 seed 評估 → Serialize。
// 全部 seeded，同參數完全可重現。回傳非零 = 訓練後沒贏過基線。

#include "Gameplay/BattleCommandEnv.h"
#include "Gameplay/BattleController.h"
#include "AI/ReinforcementLearning.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

using namespace Potato;
using namespace Potato::Gameplay;
using namespace Potato::AI;

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

int main(int argc, char** argv) {
    int episodes = 200, evalEpisodes = 30;
    unsigned seedBase = 1000, weightsSeed = 42, agentSeed = 1234;
    std::string outPath = "trained_battle_agent.json";
    for (int i = 1; i < argc; ++i) {
        auto next = [&](int& dst) {
            if (i + 1 < argc) dst = std::atoi(argv[++i]);
        };
        auto nextU = [&](unsigned& dst) {
            if (i + 1 < argc) dst = (unsigned)std::strtoul(argv[++i], nullptr, 10);
        };
        if (std::strcmp(argv[i], "--episodes") == 0) next(episodes);
        else if (std::strcmp(argv[i], "--eval") == 0) next(evalEpisodes);
        else if (std::strcmp(argv[i], "--seed") == 0) nextU(seedBase);
        else if (std::strcmp(argv[i], "--weights-seed") == 0) nextU(weightsSeed);
        else if (std::strcmp(argv[i], "--agent-seed") == 0) nextU(agentSeed);
        else if (std::strcmp(argv[i], "--out") == 0 && i + 1 < argc)
            outPath = argv[++i];
        else if (std::strcmp(argv[i], "--help") == 0) {
            printf("用法見檔頭註解。\n");
            return 0;
        }
    }
    if (episodes <= 0 || evalEpisodes <= 0) {
        fprintf(stderr, "episodes/eval 必須 > 0\n");
        return 2;
    }

    printf("=== Battle Trainer ===\n");
    printf("episodes=%d eval=%d seedBase=%u out=%s\n\n", episodes,
           evalEpisodes, seedBase, outPath.c_str());

    // ---- 基線：指揮官永不行動 ----
    const unsigned evalSeedBase = 5000;
    float baselineMean = 0.0f;
    {
        BattleCommandEnv env(0);
        for (int i = 0; i < evalEpisodes; ++i) {
            baselineMean += RunEvalEpisode(env, evalSeedBase + i,
                                           [](const State&) { return 0; });
        }
        baselineMean /= evalEpisodes;
        printf("baseline mean score = %.3f（不介入=原地站崗）\n\n",
               baselineMean);
    }

    // ---- DQN 訓練 ----
    DQNAgent agent(kCommandStateSize, kCommandActionCount,
                   /*lr*/ 0.003f, /*gamma*/ 0.95f, /*epsilon*/ 1.0f,
                   /*buffer*/ 6000, /*batch*/ 32);
    agent.SeedWeights(weightsSeed);
    agent.SetSeed(agentSeed);

    std::vector<float> epRewards;
    int trainSteps = 0;
    {
        BattleCommandEnv env(0);
        for (int ep = 0; ep < episodes; ++ep) {
            State s = env.Reset(seedBase + (unsigned)ep * 131u);
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
                if (trainSteps % 200 == 0) agent.UpdateTargetNetwork();
                if (r.done) break;
            }
            epRewards.push_back(total);
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

    // ---- 訓練後評估 ----
    float trainedMean = 0.0f;
    int wins = 0, actionHist[kCommandActionCount] = {0};
    {
        BattleCommandEnv env(0);
        for (int i = 0; i < evalEpisodes; ++i) {
            State s = env.Reset(evalSeedBase + i);
            float sc = 0.0f;
            for (int step = 0; step < 60; ++step) {
                Action a = agent.SelectAction(s, /*explore*/ false);
                if (a.id >= 0 && a.id < kCommandActionCount)
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
        trainedMean /= evalEpisodes;
    }
    printf("\ntrained mean score = %+.3f  wins=%d/%d\n", trainedMean, wins,
           evalEpisodes);
    printf("action 分佈:");
    for (int i = 0; i < kCommandActionCount; ++i) {
        printf(" %s=%d", CommandActionName(i), actionHist[i]);
    }
    printf("\n");

    // ---- 輸出權重 ----
    std::ofstream out(outPath);
    if (!out) {
        fprintf(stderr, "無法寫入 %s\n", outPath.c_str());
        return 2;
    }
    out << agent.Serialize();
    out.close();
    printf("權重已寫出: %s\n", outPath.c_str());

    if (trainedMean <= baselineMean + 0.05f) {
        printf("警告: 訓練後分數未顯著贏過基線（可能 episodes 太少）\n");
        return 1;
    }
    return 0;
}
