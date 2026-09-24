#pragma once

// PlayerDeck —— G-2 牌庫掠奪：deck = 科技樹。
//
// 牌庫成長來自征服而非商店：敵將被擊敗/收服（CampaignLedger
// 記下非 Unknown 處置）時，其 signatureDoctrineId 指向的
// doctrine 卡解鎖進玩家牌庫。
//
// 語義：
// - 已擁有的卡再掠得 → 強化層 refine+1（modifier 乘
//   (1 + 0.25·refine)，戰鬥規則不放大——平衡考量）
// - refine 滿 3 後再掠得 → 轉化為 spoils 戰利品點（資源回收）
// - 無戰收服（Negotiated/Intimidated/Defected/Subdued）同樣
//   給卡——不戰不懲罰收集；disp==Unknown 不掠奪
// - 每次掠奪記 LootEvent 入帳（牌庫自帶流水帳）
// - potato.player_deck/1 序列化，由 CampaignState 聚合存檔

#include "Gameplay/CampaignLedger.h"
#include "Gameplay/Doctrine.h"
#include "Gameplay/DoctrineLibrary.h"
#include "Serialization/JsonParser.h"

#include <set>
#include <string>
#include <vector>

namespace Potato {
namespace Campaign {

class PlayerDeck {
public:
    enum class LootOutcome {
        NotEligible,  // disp==Unknown——敵將未決不掠奪
        NoSignature,  // 敵將未指定 signatureDoctrineId
        UnknownCard,  // id 不在卡池（內容錯誤，記警告不上帳）
        Added,        // 新卡入庫
        Refined,      // 重複 → 強化層+1
        Converted,    // refine 滿 → 轉化 spoils
    };

    struct LootEvent {
        std::string generalId;
        std::string cardId;
        int chapter = 0;
        Gameplay::GeneralDisposition disposition =
            Gameplay::GeneralDisposition::Unknown;
        LootOutcome outcome = LootOutcome::NotEligible;
    };

    static constexpr int kRefineCap = 3;
    static constexpr float kRefineBonus = 0.25f;

    // 掠奪敵將 signature 卡；lib 用於驗證卡 id 存在
    LootOutcome LootSignature(const std::string& generalId,
                              const std::string& cardId,
                              Gameplay::GeneralDisposition disp,
                              int chapter,
                              const Gameplay::DoctrineLibrary& lib);

    bool Owns(const std::string& id) const;
    int RefineLevel(const std::string& id) const;
    size_t Size() const { return owned.size(); }
    int Spoils() const { return spoils; }

    // 擁有卡 id 集（餵 DoctrineLibrary::TotalModifier 子集版）
    std::set<std::string> Ids() const;
    // 擁有卡 modifier 加總，含 refine 加成（×(1+0.25·refine)）
    float TotalModifier(const Gameplay::DoctrineLibrary& lib,
                        const std::string& key) const;
    // 擁有卡的戰鬥規則全部注入（牌庫→DoctrineSet 上場面）
    void AppendOwnedRules(const Gameplay::DoctrineLibrary& lib,
                          Gameplay::DoctrineSet& set) const;

    const std::vector<LootEvent>& Journal() const { return journal; }

    // potato.player_deck/1
    JsonValue ToJson() const;
    bool FromJson(const JsonValue& j);

private:
    struct Owned {
        std::string id;
        int refine = 0;
    };
    std::vector<Owned> owned;
    std::vector<LootEvent> journal;
    int spoils = 0;
};

const char* LootOutcomeName(PlayerDeck::LootOutcome o);

} // namespace Campaign
} // namespace Potato
