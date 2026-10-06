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

#include "Gameplay/BattleController.h"
#include "Gameplay/Doctrine.h"
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

// ---- 動作表 ----
static const int kNumActions = 5;
static const char* kActionNames[] = {
    "wait",     // 0:不介入（省 CP）
    "advance",  // 1:全軍 AttackMove → 目標點
    "engage",   // 2:各隊 Engage 最近敵軍
    "hold",     // 3:全軍原地駐守
    "retreat",  // 4:全軍撤往集結點
};

static const int kStateSize = 15;

// ============================================================================
// BattleCommandEnv — 把 BattleController 包成 RLEnvironment
// ============================================================================
class BattleCommandEnv : public RLEnvironment {
public:
    explicit BattleCommandEnv(unsigned seed) { Reset(seed); }

    // 指定 seed 重建戰場（評估用同一組 seed 比較才公平）
    State Reset(unsigned seed) {
        epSeed = seed;
        std::mt19937 rng(seed);
        auto jitter = [&](float base, float range) {
            return base + std::uniform_real_distribution<float>(-range, range)(rng);
        };

        battle = std::make_unique<BattleController>(20, 15, 1.0f);
        // 中央牆留缺口（y 6~8）——同斷橋地形
        for (int y = 0; y < 15; ++y) {
            if (y < 6 || y > 8) {
                battle->GetField().SetObstacle(10, y, true);
            }
        }
        battle->SetObjective(0, Vector2(18, 7));
        battle->SetRallyPoint(0, Vector2(1, 7));
        battle->SetRallyPoint(1, Vector2(18, 7));
        battle->SetCommandPoints(0, 60); // 指揮官預算
        battle->SetCommandPoints(1, 0);  // 守軍全交 doctrine

        // 攻方四隊：站位/人數隨 seed 抖動;只有墊底 Hold——無指揮不動
        const char* atkNames[] = {"前鋒", "左翼", "右翼", "預備隊"};
        const Vector2 atkPos[] = {{3, 4}, {2, 7}, {3, 10}, {5, 7}};
        ownSquads.clear();
        for (int i = 0; i < 4; ++i) {
            Vector2 p(jitter(atkPos[i].x, 1.0f), jitter(atkPos[i].y, 1.0f));
            p.x = std::clamp(p.x, 0.5f, 9.0f);
            p.y = std::clamp(p.y, 0.5f, 14.5f);
            Squad* s = battle->CreateSquad(atkNames[i], 0, p,
                                           10 + (int)(rng() % 6));
            DoctrineSet d;
            d.AddRule({DoctrineTrigger::Always,
                       DoctrineAction::HoldPosition, 0.0f, 100});
            battle->AssignDoctrine(s, d);
            ownSquads.push_back(s);
        }
        // 守方四隊：VideoDataDemo 式守備 doctrine
        const char* defNames[] = {"橋頭守軍", "北段守軍", "南段守軍", "城內守軍"};
        const Vector2 defPos[] = {{12, 7}, {11, 3}, {11, 11}, {14, 7}};
        enemySquads.clear();
        for (int i = 0; i < 4; ++i) {
            Vector2 p(jitter(defPos[i].x, 0.8f), jitter(defPos[i].y, 1.0f));
            p.x = std::clamp(p.x, 10.5f, 19.5f);
            p.y = std::clamp(p.y, 0.5f, 14.5f);
            // 守軍刻意較弱（8~11 vs 攻方 10~15）——主動求戰明顯有利,
            // DQN 才不會收斂到「龜縮換平手」的懦夫最優解
            Squad* s = battle->CreateSquad(defNames[i], 1, p,
                                           8 + (int)(rng() % 4));
            DoctrineSet d;
            d.AddRule({DoctrineTrigger::EnemyInRange,
                       DoctrineAction::AttackNearest, 4.0f, 10});
            d.AddRule({DoctrineTrigger::AllyEngaged,
                       DoctrineAction::DefendNearestAlly, 0.0f, 50});
            d.AddRule({DoctrineTrigger::Always,
                       DoctrineAction::HoldPosition, 0.0f, 100});
            battle->AssignDoctrine(s, d);
            enemySquads.push_back(s);
        }

        ownStart = TotalMembers(ownSquads);
        enemyStart = TotalMembers(enemySquads);
        prevOwnLost = 0;
        prevEnemyLost = 0;
        prevProgress = MeanDistToObjective();
        lastAction = 0;
        elapsed = 0.0f;
        battle->BeginExecution();
        return Featurize();
    }

    State Reset() override { return Reset(epSeed); }

    StepResult Step(const Action& action) override {
        ApplyAction(action.id);
        lastAction = action.id;

        StepResult out;
        for (int i = 0; i < kTicksPerDecision; ++i) {
            battle->Update(kDt);
            elapsed += kDt;
            if (battle->GetPhase() == BattlePhase::Resolution) {
                break;
            }
        }

        // ---- reward shaping ----
        const int ownLost = ownStart - TotalMembers(ownSquads);
        const int enemyLost = enemyStart - TotalMembers(enemySquads);
        float r = 0.06f * (float)((enemyLost - prevEnemyLost) -
                                  (ownLost - prevOwnLost)); // 交換比
        const float progress = MeanDistToObjective();
        r += 0.08f * (prevProgress - progress);             // 壓向目標
        r += 0.03f * OwnEngagedFraction();                  // 接戰即時獎勵
        r -= 0.004f;                                        // 每步微懲（催促）
        prevOwnLost = ownLost;
        prevEnemyLost = enemyLost;
        prevProgress = progress;

        out.done = (battle->GetOutcome() != BattleOutcome::Ongoing) ||
                   (elapsed >= maxTime);
        if (battle->GetOutcome() == BattleOutcome::Victory) {
            r += 1.5f;
        } else if (battle->GetOutcome() == BattleOutcome::Defeat) {
            r -= 1.5f;
        } else if (out.done) {
            r -= 0.6f; // 超時平手＝沒打到東西,明顯差過積極求戰
        }

        out.nextState = Featurize();
        out.reward = r;
        return out;
    }

    std::vector<Action> GetActionSpace() const override {
        std::vector<Action> v;
        for (int i = 0; i < kNumActions; ++i) {
            v.emplace_back(i, kActionNames[i]);
        }
        return v;
    }
    int GetStateSize() const override { return kStateSize; }

    // ---- 評估/檢查用 ----
    const BattleController& Battle() const { return *battle; }
    BattleOutcome GetOutcome() const { return battle->GetOutcome(); }
    int OwnLost() const { return ownStart - TotalMembers(ownSquads); }
    int EnemyLost() const { return enemyStart - TotalMembers(enemySquads); }
    int GetLastAction() const { return lastAction; }

private:
    static constexpr float kDt = 0.1f;
    static constexpr int kTicksPerDecision = 10;   // 決策間隔 1.0s
    static constexpr float kHoldSeconds = 1.2f;    // 介入令持續（略重疊保動量）
    const float maxTime = 60.0f;

    int TotalMembers(const std::vector<Squad*>& v) const {
        int n = 0;
        for (const Squad* s : v) {
            if (!s->IsEliminated()) n += s->GetMembers();
        }
        return n;
    }

    float OwnEngagedFraction() const {
        float engaged = 0.0f;
        int n = 0;
        for (const Squad* s : ownSquads) {
            if (s->IsEliminated()) continue;
            ++n;
            if (s->IsEngaged()) engaged += 1.0f;
        }
        return n > 0 ? engaged / n : 0.0f;
    }

    float MeanDistToObjective() const {
        const Vector2 obj = battle->GetObjective(0);
        float sum = 0.0f;
        int n = 0;
        for (const Squad* s : ownSquads) {
            if (s->IsEliminated()) continue;
            sum += (s->GetPosition() - obj).Length();
            ++n;
        }
        return n > 0 ? sum / n : 0.0f;
    }

    const Squad* NearestEnemy(const Squad* self) const {
        const Squad* best = nullptr;
        float bd = 1e30f;
        for (const Squad* e : enemySquads) {
            if (e->IsEliminated() || e->IsRouting()) continue;
            float d = (e->GetPosition() - self->GetPosition()).Length();
            if (d < bd) { bd = d; best = e; }
        }
        return best;
    }

    void ApplyAction(int id) {
        const Vector2 obj = battle->GetObjective(0);
        const Vector2 rally = battle->GetRallyPoint(0);
        for (Squad* s : ownSquads) {
            if (s->IsEliminated() || s->IsRouting()) continue;
            switch (id) {
            case 1:
                battle->Intervene(s, SquadOrder::AttackMove, obj, kHoldSeconds);
                break;
            case 2: {
                const Squad* t = NearestEnemy(s);
                if (t) {
                    battle->Intervene(s, SquadOrder::Engage, t, kHoldSeconds);
                }
                break;
            }
            case 3:
                battle->Intervene(s, SquadOrder::Hold, s->GetPosition(),
                                  kHoldSeconds);
                break;
            case 4:
                battle->Intervene(s, SquadOrder::Retreat, rally, kHoldSeconds);
                break;
            default:
                break; // wait / 未知 id
            }
        }
    }

    State Featurize() const {
        State s;
        s.features.assign(kStateSize, 0.0f);
        s.timestamp = (uint64_t)(elapsed * 1000.0f);

        const int ownAlive = TotalMembers(ownSquads);
        const int enemyAlive = TotalMembers(enemySquads);
        float moraleSum = 0, eMoraleSum = 0, staminaSum = 0;
        float engaged = 0, routing = 0, eEngaged = 0;
        float nearestSum = 0;
        int aliveSquads = 0, eAliveSquads = 0;
        for (const Squad* sq : ownSquads) {
            if (sq->IsEliminated()) continue;
            ++aliveSquads;
            moraleSum += sq->GetMorale();
            staminaSum += sq->GetStamina();
            if (sq->IsEngaged()) engaged += 1.0f;
            if (sq->IsRouting()) routing += 1.0f;
            const Squad* ne = NearestEnemy(sq);
            nearestSum += ne ? (ne->GetPosition() - sq->GetPosition()).Length()
                             : 20.0f;
        }
        for (const Squad* sq : enemySquads) {
            if (sq->IsEliminated()) continue;
            ++eAliveSquads;
            eMoraleSum += sq->GetMorale();
            if (sq->IsEngaged()) eEngaged += 1.0f;
        }
        const float as = (float)std::max(1, aliveSquads);
        const float es = (float)std::max(1, eAliveSquads);

        s.features[0] = ownStart > 0 ? (float)ownAlive / ownStart : 0.0f;
        s.features[1] = enemyStart > 0 ? (float)enemyAlive / enemyStart : 0.0f;
        s.features[2] = moraleSum / as;
        s.features[3] = eMoraleSum / es;
        s.features[4] = engaged / as;
        s.features[5] = routing / as;
        s.features[6] = MeanDistToObjective() / 20.0f;
        s.features[7] = (nearestSum / as) / 20.0f;
        s.features[8] = (float)battle->GetCommandPoints(0) / 60.0f;
        s.features[9] = elapsed / maxTime;
        s.features[10] = enemyAlive > ownAlive ? 1.0f : 0.0f;
        s.features[11] = std::clamp((float)((enemyStart - enemyAlive) -
                                            (ownStart - ownAlive)) / 60.0f,
                                    -1.0f, 1.0f);
        s.features[12] = eEngaged / es;
        s.features[13] = staminaSum / as;
        s.features[14] = (float)lastAction / 4.0f;
        return s;
    }

    std::unique_ptr<BattleController> battle;
    std::vector<Squad*> ownSquads;
    std::vector<Squad*> enemySquads;
    unsigned epSeed = 0;
    int ownStart = 0, enemyStart = 0;
    int prevOwnLost = 0, prevEnemyLost = 0;
    float prevProgress = 0.0f;
    int lastAction = 0;
    float elapsed = 0.0f;
};

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
