#pragma once

// F-3：HUD 密度三檔——資訊密度的單一來源門檻矩陣。
//
// 面板層級裁撤，不是逐行裁撤：各 HUDFeature 對應一個面板或一組
// 同質資訊行，HUDShown(feature, density) 回應該密度下是否顯示。
// 恆在件（階段/CP/時鐘、全軍控制面、結算視窗）沒有枚舉值——
// 它們不經閘門，「關鍵情報三檔恆在」由不列舉保證。

namespace HUDDensityUI {

enum class Density : int { Minimal = 0, Standard = 1, Verbose = 2 };

enum class Feature : int {
    EventLog = 0,   // 戰況卷軸
    Minimap,        // 小地圖
    EnemyDossier,   // 敵將檔案
    SquadDetail,    // 斷橋指揮內選取小隊明細
    HelpHints,      // 操作提示行
    FogIntel,       // 未揭露敵軍計數/霧情報（verbose 專屬）
    ExecStats,      // 執行率/帳本計數列（verbose 專屬）
    Count
};

// 門檻：Standard 開前五項，FogIntel/ExecStats 僅 Verbose
inline bool HUDShown(Feature f, Density d) {
    const int need = (f == Feature::FogIntel || f == Feature::ExecStats)
                         ? 2
                         : 1;
    return static_cast<int>(d) >= need;
}

// int → Density，越界鉗回（檔案值/外部輸入共用守衛）
inline Density DensityFromInt(int v) {
    if (v < 0) return Density::Minimal;
    if (v > 2) return Density::Verbose;
    return static_cast<Density>(v);
}

inline const char* DensityName(Density d) {
    switch (d) {
    case Density::Minimal:  return "精簡";
    case Density::Standard: return "標準";
    case Density::Verbose:  return "詳盡";
    }
    return "標準";
}

} // namespace HUDDensityUI
