#pragma once

// 神話滲透層（D-1/Epic D）：區域滲透等級狀態機 + 神明好感表。
//
// 滲透四階 Quiet→Anomalies→Seep→Manifest：治理/屠殺事件經 Feed
// 累積區域壓力，跨閾值升階並錄轉換（record-is-truth）；
// 章內等級只升不降（土地不會在戰中遺忘），章節邊界由
// DeriveFrom 以治理快照重推導——跨章同樣只升不降。
// 渲染層唯讀消費 Level()/Levels()（架構邊界：唯讀出口）。
//
// 神明態度：BindSpirit 把區域綁守護靈，favor < kAngryFavor 的
// 怒神區域暴行類事件壓力加成——D-3 天命兌換的 favor 欄位在此預留。
//
// 分層：本類住 Campaign/（可含入 Gameplay 型別）；
// Gameplay 永不回流依賴 Campaign。

#include "Gameplay/GovernanceEvent.h"
#include "Gameplay/MythLog.h"

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace Potato {

class JsonValue;

namespace Campaign {

// 滲透等級（序數即階數——比較/夾取直接用整數語義）
enum class Seepage {
    Quiet = 0,     // 風平浪靜
    Anomalies = 1, // 異象：局部反常（霧色偏移/陰影錯位）
    Seep = 2,      // 滲透：音景先於視覺、物件微移
    Manifest = 3,  // 降臨：全主題切換
};

const char* SeepageName(Seepage s);

// 跨階記錄：哪個區域、從哪階到哪階、第幾章——record-is-truth
struct SeepageTransition {
    std::string region;
    Seepage from = Seepage::Quiet;
    Seepage to = Seepage::Quiet;
    int chapter = 1;
};

class MythLayer {
public:
    // ---- 事件驅動 ----
    // 治理事件入區域：暴行/焚村累壓力，仁政類減壓；
    // 壓力可波動、等級 ratchet 只升不降。未知區域自動建檔 Quiet。
    void Feed(const std::string& region, Gameplay::GovernanceEvent ev);

    // ---- 神明態度 ----
    void BindSpirit(const std::string& region, const std::string& spirit);
    void AdjustFavor(const std::string& spirit, float delta);
    float Favor(const std::string& spirit) const; // 未登錄 = kNeutralFavor

    // ---- 渲染層唯讀出口 ----
    Seepage Level(const std::string& region) const;
    std::unordered_map<std::string, Seepage> Levels() const; // 快照
    float Pressure(const std::string& region) const;
    const std::vector<SeepageTransition>& Transitions() const {
        return transitions;
    }

    // ---- 章節邊界重推導 ----
    // 治理快照換算壓力注入所有已知區域再 ratchet；
    // 墮落/動亂推高滲透，清廉治理不讓等級回落。
    void DeriveFrom(float depravity, float civilOrder, int unrestLevel,
                    int chapter);

    // 轉換敘事出口：每次跨階發一筆 MythEvent（祠=區域、靈=守護靈）。
    // 事件在層內變異完成後才派出——回呼可安全再入層。
    void SetEventCallback(
        std::function<void(const Gameplay::MythEvent&)> cb) {
        onEvent = std::move(cb);
    }
    const std::function<void(const Gameplay::MythEvent&)>&
    EventCallback() const { return onEvent; }

    // ---- 持久化（CampaignState myth_layer 段，JsonValue 段式）----
    JsonValue ToJson() const;
    bool FromJson(const JsonValue& j);

    // 壓力閾值（寫死可調參）
    static constexpr float kAnomalies = 10.0f;
    static constexpr float kSeep = 30.0f;
    static constexpr float kManifest = 60.0f;
    // 怒神：favor 低於此值，該區域暴行壓力 ×kAngerBoost
    static constexpr float kAngryFavor = 25.0f;
    static constexpr float kAngerBoost = 1.5f;
    static constexpr float kNeutralFavor = 50.0f;
    // DeriveFrom 治理快照→壓力換算率
    static constexpr float kDepravityRate = 0.15f;
    static constexpr float kUnrestRate = 5.0f;
    static constexpr float kOrderWarn = 30.0f;   // 秩序低於此注入壓力
    static constexpr float kOrderRate = 0.4f;

private:
    struct Region {
        float pressure = 0.0f;
        Seepage level = Seepage::Quiet;
        std::string spirit; // 守護靈（空 = 未綁）
    };

    void Ratchet(const std::string& region, Region& r,
                 std::vector<Gameplay::MythEvent>& out);
    void Dispatch(const std::vector<Gameplay::MythEvent>& events);

    std::unordered_map<std::string, Region> regions;
    std::unordered_map<std::string, float> favor;
    std::vector<SeepageTransition> transitions;
    int chapter = 1;
    std::function<void(const Gameplay::MythEvent&)> onEvent;
};

} // namespace Campaign
} // namespace Potato
