#pragma once

// 天命兌換器（D-3/Epic D）：神明 favor → 民心/秩序的可見門檻兌換。
//
// 貨幣是 MythLayer::favor（逐神 0–100，已持久化於 myth_layer 段）；
// 兌換一律寫 Campaign::Governance（帳本真相層）——戰中 SageCommand
// 同名軸不碰。
//
// 灰顯可見：Options() 永遠列出全部價目，不可兌換的項附
// affordable=false + reason（照 NoBattleResolver::CheckOptions
// 慣例）——門檻可見是設計本體，不是隱藏。戰中 UI 另須參考
// battleUsesLeft（affordable 只管 favor/綁定，不折戰中額度——
// 額度是「這一場」的語義，章節邊界不受限）。
//
// 兌換守衛：Convert 要求 spirit 已登錄 favor 表（HasSpirit）——
// 未登錄名字不得以 kNeutralFavor 起算鑄幣；Options 則仍以
// 中立 50 顯示價目（顯示≠可成交）。
//
// 回呼再入注意：onEvent 內呼叫 ResetBattleUses 會重置本場額度
// （呼叫端自傷，favor 有界故可封頂）——比照 MythLayer 慣例不堵。
//
// 戰中限次：每場戰 kMaxBattleUses 次（執行期計數，不入存檔）；
// 章節邊界 Convert(inBattle=false) 不限次，只受 favor 餘額。
//
// 入帳：Convert 成功發 FateEvent 型別化回呼（先變異後派出，回呼
// 可安全再入）。呼叫端決定去向——戰中接
// recorder.AddRecord(battle.GetElapsed(), msg)，章節邊界接
// LedgerChain::Append(EntrySource::Myth, ...)。

#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace Potato {
namespace Campaign {

class MythLayer;
class Governance;

// 兌換標的：天命換哪條治理軸
enum class FateTarget {
    PopularSupport, // 民心
    CivilOrder,     // 秩序
};

const char* FateTargetName(FateTarget t);

// 價目項（Options 回傳；affordable=false 時 reason 給原因）
struct FateOption {
    FateTarget target;
    float cost;
    float gain;
    bool affordable;      // favor 足額且層已綁定
    bool battleUsesLeft;  // 戰中額度尚有剩（UI 判斷戰中灰顯用）
    std::string reason;   // affordable=false 的中文原因
};

// 兌換記錄（回呼型別；呼叫端寫 Recorder/LedgerChain）
// gain 是實際生效量（夾取後 after−before），非名目價目——
// record-is-truth：入帳數字即帳本實際位移。
struct FateEvent {
    std::string spirit;
    FateTarget target;
    float cost;
    float gain;   // 實際生效量（軸滿格時可能 < 價目 gain）
    bool inBattle;
};

class FateExchange {
public:
    // 綁定兩層（呼叫端擁有生命週期；未綁定時 Convert 拒絕）。
    // 重綁順帶重置戰中額度——比照 ShrineField::Bind 場域生命週期。
    void Bind(MythLayer& myths, Governance& gov);

    // 灰顯價目：永遠列出全部標的，不足項 affordable=false+reason
    std::vector<FateOption> Options(const std::string& spirit) const;

    // 兌換：favor 足額 +（戰中時）額度未盡 → 扣 favor、加治理軸、
    // 發 FateEvent。失敗回 false 且零副作用。
    bool Convert(const std::string& spirit, FateTarget target,
                 bool inBattle);

    // 戰中額度（新戰場景由呼叫端重置）
    void ResetBattleUses() { battleUses = 0; }
    int BattleUses() const { return battleUses; }

    void SetEventCallback(std::function<void(const FateEvent&)> cb) {
        onEvent = std::move(cb);
    }

    // 價目（寫死可調參）
    static constexpr float kFateCost = 20.0f;
    static constexpr float kFateGain = 5.0f;
    static constexpr int kMaxBattleUses = 2;

private:
    MythLayer* myths = nullptr;
    Governance* gov = nullptr;
    int battleUses = 0;
    std::function<void(const FateEvent&)> onEvent;
};

} // namespace Campaign
} // namespace Potato
