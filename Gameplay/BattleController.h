#pragma once

#include "Core/CoreTypes.h"
#include "MathUtils/Vector2.h"
#include "FlowField.h"
#include "Squad.h"
#include "Doctrine.h"

#include <functional>
#include <memory>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

namespace Potato {
namespace Gameplay {

class QuantumFog;

/**
 * 戰鬥階段：對應「回合層 → 即時層 → 戰後」的三拍循環
 */
enum class BattlePhase {
    Deployment,     // 回合層：編成、寫 doctrine、配置
    Execution,      // 即時層：doctrine 自動執行，可用 CP 介入
    Resolution      // 戰後：勝負已定
};

enum class BattleOutcome {
    Ongoing,
    Victory,        // team 0（玩家方）全殲或擊潰敵軍
    Defeat,
    Draw
};

/**
 * 戰鬥控制器
 *
 * 即時執行層的核心：持有戰場 grid/flow field、所有小隊、
 * 每隊的 doctrine 集合與目標點，逐 tick 評估 doctrine →
 * 下達命令 → 移動 → 結算戰鬥。不依賴 EventBus，
 * 事件用回調輸出（上層可自行接到 EventBus）。
 */
class BattleController {
public:
    using EventCallback = std::function<void(const std::string&)>;

    BattleController(int gridWidth, int gridHeight, float cellSize);

    // ---- 部署階段 ----
    Squad* CreateSquad(const std::string& name, int team,
                       const Vector2& pos, int members);
    void AssignDoctrine(Squad* squad, const DoctrineSet& doctrine);
    // 讀回已指派的 doctrine（PlanningDeck 回讀用）；未指派回 nullptr
    const DoctrineSet* GetDoctrine(const Squad* squad) const {
        auto it = doctrines.find(const_cast<Squad*>(squad));
        return it != doctrines.end() ? &it->second : nullptr;
    }
    void SetObjective(int team, const Vector2& pos);
    void SetRallyPoint(int team, const Vector2& pos);
    void SetCommandPoints(int team, int points);

    FlowField& GetField() { return field; }
    const FlowField& GetField() const { return field; }

    // 查詢部署設定（BattlePlanner 等規劃器用）
    bool HasObjective(int team) const {
        return objectives.find(team) != objectives.end();
    }
    Vector2 GetObjective(int team) const {
        auto it = objectives.find(team);
        return it != objectives.end() ? it->second : Vector2(0.0f, 0.0f);
    }
    bool HasRallyPoint(int team) const {
        return rallyPoints.find(team) != rallyPoints.end();
    }
    Vector2 GetRallyPoint(int team) const {
        auto it = rallyPoints.find(team);
        return it != rallyPoints.end() ? it->second : Vector2(0.0f, 0.0f);
    }
    // 某隊現存成員總數（含潰逃中）
    int TotalMembers(int team) const;

    // 目前遊戲時間（秒）與事件回調存取（BattleRecorder 附加用）
    float GetElapsed() const { return elapsed; }
    EventCallback GetEventCallback() const { return onEvent; }

    // T-6 士氣執行率：小隊士氣 < threshold 時 doctrine 動作只有 rate 機率生效
    // （rate 1.0 = 關閉此規則）
    void SetMoraleExecution(float threshold, float rate);

    // 部署完畢 → 進入即時執行（不再能改 doctrine）
    bool BeginExecution();

    // ---- 即時階段 ----
    // timeScale：0 = 暫停（指令階段）、0.25 = 子彈時間、1 = 正常
    void SetTimeScale(float scale);
    float GetTimeScale() const { return timeScale; }

    // CP 介入：覆寫該隊命令 holdSeconds 秒（遊戲時間）。回傳 false = CP 不足/階段不對
    bool Intervene(Squad* squad, SquadOrder order, const Vector2& target,
                   float holdSeconds = 5.0f);
    // 指定敵隊版本：Engage 等需要 squad 目標的命令用；target 全滅回 false
    bool Intervene(Squad* squad, SquadOrder order, const Squad* target,
                   float holdSeconds = 5.0f);

    // ---- 量子敵情霧（Q-1，可選）----
    // 綁定後：Update 會推 fog（情報時效/退相干），我軍小隊進入
    // fogRevealRange 格內的未揭露敵軍自動免費揭露（接觸偵查）。
    void BindFog(QuantumFog* fog);
    // 把一支小隊與 fog entity 關聯（entity 的疊加雲代表這支小隊）
    void BindFogSquad(Squad* squad, int entityId);
    QuantumFog* GetFog() const { return fog; }
    int GetFogEntityId(const Squad* squad) const;
    Squad* GetFogSquad(int entityId) const;
    void SetFogRevealRange(float cells) {
        if (cells > 0.0f) fogRevealRange = cells; // NaN/非正數忽略
    }

    // 每幀呼叫：realDt 為真實秒數，內部乘 timeScale
    void Update(float realDt);

    // ---- 查詢 ----
    BattlePhase GetPhase() const { return phase; }
    BattleOutcome GetOutcome() const { return outcome; }
    int GetCommandPoints(int team) const;
    const std::vector<UniquePtr<Squad>>& GetSquads() const { return squads; }

    void SetEventCallback(EventCallback cb) { onEvent = std::move(cb); }

private:
    void EvaluateDoctrines();
    void ResolveCombat(float dt);
    void UpdateContexts();
    void CheckOutcome();
    void Emit(const std::string& msg);

    SquadContext BuildContext(const Squad& squad) const;
    FlowField* GetTeamField(int team);
    Squad* FindNearestEnemy(const Squad& squad, float maxDist) const;
    Squad* FindWeakestEnemy(const Squad& squad, float maxDist) const;
    Squad* FindNearestEngagedAlly(const Squad& squad) const;
    // Scout 目標：最近未揭露敵情雲的最高機率候選格；無則回 false
    bool FindScoutTarget(const Squad& squad, Vector2& out) const;
    // squad 綁定了未揭露的敵情實體 → 對 doctrine/Engage 層不可見
    bool IsHiddenByFog(const Squad& squad) const;

    FlowField field;                                  // 地形/障礙定義
    std::unordered_map<int, UniquePtr<FlowField>> teamFields; // 各隊目標場
    std::vector<UniquePtr<Squad>> squads;
    std::unordered_map<Squad*, DoctrineSet> doctrines;
    std::unordered_map<int, Vector2> objectives;
    std::unordered_map<int, Vector2> rallyPoints;
    std::unordered_map<int, int> commandPoints;
    std::unordered_map<Squad*, float> interventionUntil; // 剩餘覆寫秒數
    std::unordered_map<Squad*, float> damageBuffer;      // 小數傷害累積
    std::unordered_map<Squad*, SquadContext> contexts;
    QuantumFog* fog = nullptr;                     // Q-1 敵情霧（外層持有）
    std::unordered_map<Squad*, int> fogEntities;   // squad → fog entityId
    float fogRevealRange = 3.0f;                   // 接觸偵查距離（格）

    BattlePhase phase;
    BattleOutcome outcome;
    float timeScale;
    float doctrineTimer;      // doctrine 評估節流
    float elapsed;
    float moraleExecThreshold = 0.0f; // 0 = 關閉
    float moraleExecRate = 1.0f;
    mutable std::mt19937 execRng{std::random_device{}()};

    static constexpr float DOCTRINE_INTERVAL = 0.25f; // 每 0.25s 遊戲時間評估一次
    static constexpr float OBJECTIVE_RADIUS = 1.0f;
    static constexpr float ENGAGE_RADIUS_FACTOR = 1.0f;

    EventCallback onEvent;
};

} // namespace Gameplay
} // namespace Potato
