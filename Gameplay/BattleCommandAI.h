#pragma once

// 戰鬥指揮官 AI 契約層（訓練/推論共用）。
//
// BattleCommandEnv（訓練環境，Gameplay/BattleCommandEnv.h）與
// BattleCommanderAI（遊戲內消費端，下方）共用同一組觀測/動作定義——
// 特徵順序與動作表就是權重檔的介面，集中在這裡單一來源防漂移。
//
// 動作空間（5）：
//   0 wait     不介入（省 CP）
//   1 advance  全軍 AttackMove → 目標點
//   2 engage   各隊 Engage 最近敵軍
//   3 hold     全軍原地駐守
//   4 retreat  全軍撤往集結點
// 非 wait 動作對每支存活小隊走 Intervene（各扣 1 CP，語義同遊戲內）。

#include "Gameplay/BattleController.h"
#include "Gameplay/Squad.h"
#include "AI/ReinforcementLearning.h"

#include <memory>
#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

inline constexpr int kCommandStateSize = 15;
inline constexpr int kCommandActionCount = 5;

const char* CommandActionName(int id);

// 決策節奏（與訓練環境同值——換節奏權重即失效）
inline constexpr float kCommandTickDt = 0.1f;       // 戰鬥 tick
inline constexpr int kCommandTicksPerDecision = 10; // 決策間隔 1.0s
inline constexpr float kCommandHoldSeconds = 1.2f;  // 介入令持續

// 跨步狀態：env 與 commander 各自追蹤，餵給特徵化
struct CommandSnapshot {
    int ownStart = 0;      // 開局我方總兵力
    int enemyStart = 0;    // 開局敵方總兵力
    float elapsed = 0.0f;  // 戰鬥已歷時（秒）
    int lastAction = 0;    // 上次動作 id
};

// 15 維特徵化——順序即訓練契約，勿更動（trained_battle_agent.json
// 的權重對此排列敏感）。timestamp 由 snap.elapsed 推得。
Potato::AI::State FeaturizeCommandState(
    const BattleController& battle,
    const std::vector<Squad*>& own,
    const std::vector<Squad*>& enemy,
    const CommandSnapshot& snap, float maxTime);

// 動作 id → 全軍姿態介入（0/未知 id 無操作；Intervene 內部扣 CP）
void ApplyCommandAction(BattleController& battle, int actionId,
                        const std::vector<Squad*>& own,
                        const std::vector<Squad*>& enemy,
                        float holdSeconds);

// ---- 遊戲內消費端：載入訓練權重、即時戰場代打 ----
// 用法：LoadFromFile(trained_battle_agent.json) → 每場開戰 Reset →
// 每幀 Update。權重不存在時 IsLoaded()=false，呼叫端降級即可。
class BattleCommanderAI {
public:
    BattleCommanderAI();

    bool LoadFromFile(const std::string& path);
    bool IsLoaded() const { return loaded; }

    // 記住基線兵力與雙方名單；team = 我方（demo 中 0=攻方玩家側）
    void Reset(const BattleController& battle, int team,
               float maxTimeSec = 60.0f);

    // 每幀呼叫；內部累積 dt，滿 1.0s 決策一次（與訓練節奏一致）。
    // 非執行階段/未載入權重 → 直接略過。
    void Update(BattleController& battle, float dt);

    int GetLastAction() const { return snap.lastAction; }
    const char* LastActionName() const {
        return CommandActionName(snap.lastAction);
    }

private:
    Potato::AI::DQNAgent agent;
    bool loaded = false;
    CommandSnapshot snap;
    std::vector<Squad*> own, enemy;
    float maxTime = 60.0f;
    float decisionAcc = 0.0f;
    int team = 0;
};

} // namespace Gameplay
} // namespace Potato
