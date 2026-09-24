#pragma once

// 神話入侵（D-4/Epic D）：mid-battle 插入的意義改變者事件。
//
// 觸發：呼叫端 Arm(滲透門檻序數, kind, spawnPos) 後每拍 Update 餵
// 區域滲透等級快照（Gameplay 不讀 Campaign——依賴方向不倒流）。
// 達閾→一次性觸發（稀有性守衛：每場最多一次）。
//
// 兩種入侵：
//   鬼軍夜行 GhostLegion——CreateSquad 中場生成 team=2 第三方
//     小隊（無 doctrine 的 Hold 幽影，威懾性存在）
//   狐仙假訊 FoxRumor——小額鬼隊 + fog entity 候選格指向虛構
//     方位（無主雲不渲染，必須綁真 squad；無 fog 時降級為純現身）
//
// 解決窗口 kResolveWindow 秒內呼叫端 Resolve：
//   pacified → 鬼隊消散（ApplyCasualties 全員，無 RemoveSquad API）
//   ignored/逾時 → 鬼隊留存 + OutcomeEvent(ignored)
// 逾時由 Update 的 dt 累積結算——全確定性。
//
// 三路回呼出口（先變異後派出，回呼可安全再入）：
//   IncursionEvent → 呼叫端接 MythLog::Record（具名三欄）+
//     SetTimeScale(0) 暫停提示（UX 層的事）
//   OutcomeEvent   → pacified 呼叫端 gov.Adjust* 加分；
//     ignored 呼叫端 MythLayer 推滲透（注壓量呼叫端決定）
//   事件字串       → 呼叫端 recorder.AddRecord(GetElapsed(), msg)

#include "MathUtils/Vector2.h"

#include <functional>
#include <string>
#include <utility>

namespace Potato {
namespace Gameplay {

class BattleController;
class Squad;

enum class IncursionKind {
    GhostLegion, // 鬼軍夜行：實體小隊中場現身
    FoxRumor,    // 狐仙假訊：幻影小隊+假情報雲
};

const char* IncursionKindName(IncursionKind k);

// ChapterDef.incursion.kind 字串 → enum（接線共用字面值，
// 避免呼叫端各抄一份）。不識別回 false。
bool ParseIncursionKind(const std::string& s, IncursionKind& out);

// 觸發事件（呼叫端接 MythLog::Record——shrine/spirit/when/detail
// 即具名四欄）
struct IncursionEvent {
    IncursionKind kind;
    std::string shrine;  // 祠=區域名
    std::string spirit;  // 靈=守護靈名
    std::string when;    // 戰役時間標記（T+秒）
    std::string detail;  // 記事
};

// 結局事件（呼叫端決定治理加分/滲透注壓量）
struct OutcomeEvent {
    bool pacified;       // true=安撫成功；false=忽視
    std::string region;
    std::string spirit;
};

// 渲染層唯讀狀態
struct IncursionState {
    bool fired = false;      // 已觸發（稀有性 latch）
    bool pending = false;    // 待解決（窗口內）
    bool resolved = false;   // 已結算
    bool pacified = false;   // 結算結果
    IncursionKind kind = IncursionKind::GhostLegion;
    std::string region;
    std::string spirit;
    float windowLeft = 0.0f; // 剩餘解決秒數
};

class MythIncursion {
public:
    // 佈防：seepageThreshold 為滲透序數（1-3,與 MythLayer::Seepage
    // 序數對齊但此處只吃 int——不引 Campaign 型別）；越界視為關閉。
    // spawnPos 為鬼軍現身點。重 Arm 重置全部狀態並消散舊鬼隊
    // （新戰場景）。生命週期：一次 Arm 對應一場 battle——
    // battle 必須活得比解決窗口久。
    void Arm(int seepageThreshold, IncursionKind kind,
             Vector2 spawnPos);

    // 解除佈防：清空 ghost/fog/ghostEid 指標與全部狀態，
    // 不觸碰它們指向的對象。battle/fog 生命期結束前呼叫——
    // 之後 Arm/Update/Resolve 皆安全（無 UAF 窗口）。
    void Disarm();

    // 每拍驅動：餵區域滲透快照。達閾→觸發生成+發事件；
    // pending 時累積窗口，逾時自動 ignored 結算。
    // 【呼叫端契約】只在 Execution 期間驅動；dt 餵真實秒數——
    // 暫停展示提示時若繼續呼叫，窗口照樣流逝（要凍結窗口就
    // 暫停中別呼叫 Update）。勿在 battle 的 Emit/Update 回呼
    // 內呼叫——CreateSquad push_back 會讓 battle 迭代懸空。
    void Update(float dt, BattleController& battle, int seepageLevel,
                const std::string& region, const std::string& spirit);

    // 解決：pending 中才有效。pacified→鬼隊消散+pacified 結局；
    // false→ignored 結局（鬼隊留存）。回 false=無 pending。
    bool Resolve(bool pacified);

    const IncursionState& GetIncursion() const { return state; }

    void SetIncursionCallback(
        std::function<void(const IncursionEvent&)> cb) {
        onIncursion = std::move(cb);
    }
    void SetOutcomeCallback(
        std::function<void(const OutcomeEvent&)> cb) {
        onOutcome = std::move(cb);
    }
    void SetEventCallback(
        std::function<void(const std::string&)> cb) {
        onEvent = std::move(cb);
    }

    // 可調參
    static constexpr float kResolveWindow = 15.0f;
    static constexpr int kGhostTeam = 2;       // 第三方
    static constexpr int kGhostMembers = 12;   // 象徵性戰力
    static constexpr float kRumorOffset = 8.0f; // 假訊雲偏移格數

private:
    void Trigger(BattleController& battle,
                 const std::string& region, const std::string& spirit);
    void Settle(bool pacified);

    bool armed = false;
    int threshold = 0;
    IncursionKind kind = IncursionKind::GhostLegion;
    Vector2 spawnPos{};

    IncursionState state;
    Squad* ghost = nullptr;   // 生成的鬼隊（battle 持有所有權）
    class QuantumFog* fog = nullptr; // 觸發時的 fog（假訊收隊用）
    int ghostEid = -1;               // 假訊 fog entity id

    std::function<void(const IncursionEvent&)> onIncursion;
    std::function<void(const OutcomeEvent&)> onOutcome;
    std::function<void(const std::string&)> onEvent;
};

} // namespace Gameplay
} // namespace Potato
