#pragma once

#include "Core/CoreTypes.h"

#include <functional>
#include <string>
#include <unordered_set>

namespace Potato {
namespace Gameplay {

class BattleController;
class Squad;

/**
 * 三權模型：道權（監察）/ 策權（切換）/ 兵權（執行）
 *
 * 對應《至聖者指揮學》的權力分立——「指揮 = 切換權，非行動」：
 * 道權唯讀監察墮落，策權是唯一的切換鈕，兵權是 doctrine 執行層。
 */
enum class Authority {
    Tao,    // 道權：墮落儀表 + 徵象，唯讀，不能下命令
    Ce,     // 策權：正逆六策 + 五行節奏，唯一的切換介面
    Bing    // 兵權：doctrine 指派 + CP 介入（既有 BattleController 機制）
};

/**
 * 五行節奏：木火（生髮，逆策墮落折扣）→ 土（封邪窗口）→ 金水（裁斷與不作為，策效減半）→ 木火
 */
enum class WuXingPhase {
    WoodFire,   // 木火：逆策墮落成本半價
    Earth,      // 土：可 SealHeresy() 封邪
    MetalWater  // 金水：所有策效果減半（係數 0.5）
};

/**
 * 政策：正六策（立人/立制/立國/正名/養民/整軍）+ 逆六策
 */
enum class Policy {
    // ---- 正六策 ----
    EstablishPeople,    // 立人：我軍全隊士氣上升
    EstablishOrder,     // 立制：秩序上升
    EstablishState,     // 立國：民心上升
    RectifyNames,       // 正名：秩序與民心並進
    NourishPeople,      // 養民：民心上升
    RallyTroops,        // 整軍：我軍士氣 + 秩序
    // ---- 逆六策 ----
    SlanderEnemy,       // 讒敵：最近敵隊士氣下降
    ConfuseEnemy,       // 惑敵：敵隊命令干擾（強制撤退覆寫）
    BribeEnemy,         // 餌敵：敵最強隊士氣大降
    Terrorize,          // 威嚇：敵全軍士氣下降
    Sabotage,           // 破壞：敵全軍士氣下降 + 撤退干擾
    DeceiveHeaven       // 欺天：敵全軍士氣大降，墮落成本最高
};

/**
 * 至聖者結局
 */
enum class SageOutcome {
    Ongoing,
    Victory,            // 普通勝利（以殲滅取勝）
    Defeat,             // 敗北（含平手——未勝即敗）
    SubdueWithoutWar,   // 至聖者無戰：不戰而屈人之兵（潰逃/歸附 > 殲滅 且 民心 ≥ 60）
    GovernedPeace,      // 治平者無勝：戰敗而治成（敗/平手 且 民心 ≥ 70 且 秩序 ≥ 70）
    Fallen              // 失格：墮落達頂，勝負無效
};

/**
 * SageCommand —— 至聖者指揮學 × 治平者管理學的 headless 邏輯核心
 *
 * 整合在 BattleController 之上：
 *  - 策權 ApplyPolicy() 直接作用於 Squad 士氣 / 民心 / 秩序，
 *    逆策的命令干擾走 BattleController::Intervene 同型機制；
 *  - 道權全部是唯讀查詢（墮落值 / 徵象 / 結局），不能下命令；
 *  - 兵權沿用 BattlePlanner + BeginExecution + Intervene。
 *
 * 墮落徵象三門檻（各只觸發一次）：
 *  60 = 以非常為常 / 80 = 以手段為正義 / 100 = 以勝負代是非 → 失格 Fallen
 *
 * 核心價值（對應《至聖者指揮學》/《治平者管理學》）：
 *  至聖者無戰 → SubdueWithoutWar：勝利但殲滅不是主因（孫子：不戰而屈人之兵）
 *  治平者無勝 → GovernedPeace：戰場未勝但治理已成（政戰合一：民心秩序高時
 *               戰敗不算真敗——仁者無敵、夫唯不爭）
 *  以勝負代是非 → Fallen：墮落滿值時勝負無效，連 GovernedPeace 也不救
 */
class SageCommand {
public:
    using EventCallback = std::function<void(const std::string&)>;

    explicit SageCommand(int playerTeam = 0, int enemyTeam = 1);

    // ---- 策權：唯一的切換介面 ----
    // 正策任一戰鬥階段可用（Resolution 除外）；逆策在封印中或墮落 ≥100 時拒絕。
    // 回傳 false = 非法階段 / 逆策被封印 / 墮落滿 / 已失格。
    bool ApplyPolicy(Policy policy, BattleController& battle);

    // 土階段封邪：鎖定逆策、墮落隨 Tick 衰減、秩序回升。非土階段回傳 false。
    bool SealHeresy();

    // 五行輪轉：木火 → 土 → 金水 → 木火；離開土階段時封印自動解除
    void AdvancePhase();

    // 每幀/每 tick 呼叫：墮落衰減、秩序回升、歸附統計、結局更新
    void Tick(float dt, const BattleController& battle);

    // 戰後結算：依 BattleController 結局 + 潰逃/殲滅統計 + 民心判定勝利型態
    SageOutcome ResolveOutcome(const BattleController& battle);

    // 劇本/測試用：直接累加墮落（與逆策走同一套徵象與失格判定）
    void AddCorruption(float amount);

    // ---- 道權：唯讀監察 ----
    float GetCorruption() const { return corruption; }       // 0 ~ 100
    int GetSignLevel() const { return signLevel; }           // 已觸發徵象數 0~3
    bool IsHereticSealed() const { return hereticSealed; }
    WuXingPhase GetWuXingPhase() const { return wuxing; }
    float GetPopularSupport() const { return popularSupport; } // 民心 0 ~ 100
    float GetCivilOrder() const { return civilOrder; }         // 秩序 0 ~ 100
    SageOutcome GetOutcome() const { return outcome; }
    int GetSubduedCount() const { return subdued; }         // 敵軍潰逃/歸附數
    int GetAnnihilatedCount() const { return annihilated; } // 敵軍被殲滅數

    void SetEventCallback(EventCallback cb) { onEvent = std::move(cb); }

    // ---- 顯示名稱（中英並列：ImGui 缺 CJK 字型也能辨識）----
    static const char* PolicyName(Policy p);
    static const char* PhaseName(WuXingPhase p);
    static const char* OutcomeName(SageOutcome o);
    static const char* SignName(int level);       // level 1~3
    static const char* AuthorityName(Authority a);
    static bool IsHereticPolicy(Policy p);
    static bool IsUprightPolicy(Policy p);

private:
    float EffectFactor() const;     // 金水期所有策效果 ×0.5
    float CorruptionFactor() const; // 木火期逆策墮落 ×0.5
    void CheckSigns();              // 門檻 → 徵象 → 失格
    void UpdateBattleStats(const BattleController& battle);
    void Emit(const std::string& msg);

    Squad* FindNearestEnemy(BattleController& battle) const;
    Squad* FindStrongestEnemy(BattleController& battle) const;
    int ApplyEnemyMorale(BattleController& battle, float delta, bool all);
    void TryDisruptEnemy(BattleController& battle, int maxSquads, float holdSeconds);

    int playerTeam;
    int enemyTeam;

    float corruption;       // 墮落值 0~100
    float popularSupport;   // 民心
    float civilOrder;       // 秩序
    int signLevel;          // 已觸發徵象 0~3（每徵象只觸發一次）
    bool hereticSealed;     // 土階段封邪中
    WuXingPhase wuxing;
    SageOutcome outcome;

    int subdued;            // 最近一次統計：潰逃/歸附敵兵數
    int annihilated;        // 最近一次統計：被殲滅敵兵數
    std::unordered_set<const Squad*> routedCounted; // 已計入歸附的潰逃隊

    EventCallback onEvent;

    static constexpr float CORRUPTION_MAX = 100.0f;
    static constexpr float SEAL_CORRUPTION_DECAY = 6.0f;  // 封邪中每秒墮落衰減
    static constexpr float SEAL_ORDER_RECOVER = 4.0f;     // 封邪中每秒秩序回升
    static constexpr float SUBDUE_SUPPORT_MIN = 60.0f;    // 無勝而勝民心門檻
    static constexpr float GOVERN_SUPPORT_MIN = 70.0f;    // 治平無勝民心門檻
    static constexpr float GOVERN_ORDER_MIN = 70.0f;      // 治平無勝秩序門檻
};

} // namespace Gameplay
} // namespace Potato
