#pragma once

// 治理事件（C-2/Epic A）：民心/秩序的戰場來源定義。
// 事件本身由 BattleController 偵測/接收並記入 recorder 事件流
// （record-is-truth）；SageCommand 消費同表作用戰場民心/秩序，
// Campaign::Governance 在章節邊界折進戰役帳。
//
// 分層：本檔只放 enum + delta 表，Gameplay 內共享；
// Campaign 可含入（Gameplay ← Campaign 方向不變）。

namespace Potato {
namespace Gameplay {

enum class GovernanceEvent {
    VillageOccupied,   // 村莊佔領：我軍隊進駐 village 互動點
    SurrenderAccepted, // 受降：敵軍潰逃/歸附，任其生還
    ConvoyProtected,   // 護輜：我軍隊進駐 convoy/supply 互動點
    Atrocity,          // 暴行：屠殺已潰逃/投降之敵
};

// 單一事件的民心/秩序 delta（戰場層直接值；戰役層再乘折半率）
struct GovernanceDelta {
    float support; // 民心 ±
    float order;   // 秩序 ±
};

inline GovernanceDelta DeltaOf(GovernanceEvent ev) {
    switch (ev) {
    case GovernanceEvent::VillageOccupied:
        return {5.0f, 2.0f};  // 秋毫無犯入村
    case GovernanceEvent::SurrenderAccepted:
        return {8.0f, 3.0f};  // 受降仁政
    case GovernanceEvent::ConvoyProtected:
        return {2.0f, 6.0f};  // 護輜安民
    case GovernanceEvent::Atrocity:
        return {-10.0f, -8.0f}; // 屠戮降者
    }
    return {0.0f, 0.0f};
}

inline const char* GovernanceEventName(GovernanceEvent ev) {
    switch (ev) {
    case GovernanceEvent::VillageOccupied:
        return "村莊佔領";
    case GovernanceEvent::SurrenderAccepted:
        return "受降";
    case GovernanceEvent::ConvoyProtected:
        return "護輜";
    case GovernanceEvent::Atrocity:
        return "暴行";
    }
    return "未知";
}

} // namespace Gameplay
} // namespace Potato
