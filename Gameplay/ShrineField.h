#pragma once

// 神社場域追蹤器（D-2/Epic D：神話第二張地圖）。
// GovernanceField 的姊妹類——BattleController 不認識地圖，本類由
// 「有地圖知識的呼叫端」每拍驅動，把 shrine 互動物轉譯成可選擇的
// 神話互動：
//
// - 我軍活隊進駐 shrine 圈 → 節點轉 Occupied（待互動），發「發現
//   神社」事件一筆
// - ApplyChoice 收安撫/挑釁/獻祭 → 節點結算一次（record-is-truth，
//   拜過的神社不再受理），發 favor delta 回呼 + 選擇事件字串
// - favor 歸 Campaign 層 MythLayer——本類只發回呼，接線由呼叫端
//   做（myth.AdjustFavor(spirit, delta)），依賴方向不倒流
// - 選擇入帳由呼叫端接 recorder.AddRecord(battle.GetElapsed(), msg)
//   ——回放 hash 鏈自動覆蓋，BattleController 不用改
// - GetShrines() 唯讀供渲染層：視覺態 + 供品態字串（環境敘事）

#include "BattleMap.h"
#include "MathUtils/Vector2.h"

#include <cstddef>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace Potato {
namespace Gameplay {

class BattleController;

// 神社互動選擇
enum class ShrineChoice {
    Appease, // 安撫
    Provoke, // 挑釁
    Offer,   // 獻祭
};

const char* ShrineChoiceName(ShrineChoice c);

// 節點視覺態（渲染層唯讀消費）
enum class ShrineVisual {
    Idle,     // 未接觸
    Occupied, // 已進駐待互動
    Appeased, // 已安撫
    Provoked, // 已挑釁
    Offered,  // 已獻祭
};

class ShrineField {
public:
    struct ShrineState {
        size_t mapIndex = 0;    // 對應 MapInteractable 索引（渲染對帳用）
        Vector2 pos;
        float radius = 1.0f;
        std::string spirit;     // 空 = 「境靈」泛稱
        std::string offering;   // 供品態（內容側給定）
        ShrineVisual visual = ShrineVisual::Idle;
    };

    // 綁定地圖互動物（部署完成後呼叫一次）；只收 type=="shrine"。
    // 注意：重綁會清掉已結算狀態——favor 是持久資源，
    // 重綁重拜同一節點會二次入帳，熱重載場景勿複用本物件。
    void Bind(const std::vector<MapInteractable>& interactables);

    // 每拍驅動：我軍活隊首次進駐 shrine 圈 → Occupied + 發現事件
    // （事件在掃描完後統一派出——回呼再入層不懸空迭代器）
    void Update(const BattleController& battle);

    // 以下索引一律是原圖 MapInteractable 索引（mapIndex），
    // 與 GovernanceField::IsBurned 同慣例——渲染/UI 直接對帳。

    // 玩家對 Occupied 節點下選擇；非待互動態或壞索引回 false
    // 不發回呼；非法 ShrineChoice 值同樣拒絕
    bool ApplyChoice(size_t mapIndex, ShrineChoice c);

    bool IsPending(size_t mapIndex) const; // Occupied 且未結算
    const ShrineState* GetShrine(size_t mapIndex) const;
    const std::vector<ShrineState>& GetShrines() const {
        return shrines;
    }

    // 事件字串出口（呼叫端接 recorder.AddRecord / MythLog）
    void SetEventCallback(
        std::function<void(const std::string&)> cb) {
        onEvent = std::move(cb);
    }
    // favor delta 出口（呼叫端接 MythLayer::AdjustFavor）
    void SetFavorCallback(
        std::function<void(const std::string& spirit, float delta)> cb) {
        onFavor = std::move(cb);
    }

    // favor delta 常數（Provoke 取負值）
    static constexpr float kAppeaseFavor = 10.0f;
    static constexpr float kProvokeFavor = 20.0f;
    static constexpr float kOfferFavor = 25.0f;

private:
    ShrineState* Find(size_t mapIndex);
    const ShrineState* Find(size_t mapIndex) const;

    std::vector<ShrineState> shrines;
    std::function<void(const std::string&)> onEvent;
    std::function<void(const std::string&, float)> onFavor;
};

} // namespace Gameplay
} // namespace Potato
