#pragma once

#include "Core/CoreTypes.h"

#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

class EnemyGeneral;

/**
 * 敵將檔案（N-2）——判詞是聽聞態，不是真相態。
 *
 * 「據報」是敘事層的核心修辭：檔案先記傳聞（系統性低估的固定
 * 偏差），偵查/觀測驗證後才回真值；高狡詐敵將能在傳聞裡放假
 * 判詞——謊在文字不在數，驗證時揭「前判有詐」。
 *
 * 寫死規則：
 * - 聽聞估值 = 真值 × 0.8（系統性低估，玩家可學習修正）
 * - 判詞由顯性軸產生（≥70 為顯性，皆中平寫中庸）
 * - cunning ≥ 70 → 放假判詞：verdict 改寫最弱軸的反向描述
 */
struct HearsayEntry {
    std::string generalName;
    std::string verdict;   // 判詞一句（傳聞體）
    float estAggression = 0.0f;
    float estDiscipline = 0.0f;
    float estCunning = 0.0f;
    bool verified = false; // 觀測驗證後為真相
    bool planted = false;  // 狡詐者放假判詞（驗證後才揭曉）
};

class GeneralDossier {
public:
    // 聽聞偏差寫死：估 = 真 × kHearsayBias
    static constexpr float kHearsayBias = 0.8f;
    // 狡詐門檻：≥ 此值放假判詞
    static constexpr float kCunningPlantThreshold = 70.0f;
    // 顯性軸門檻：≥ 此值寫該軸判詞
    static constexpr float kDominantThreshold = 70.0f;

    // 建傳聞檔案；同名覆蓋（重新聽聞）
    const HearsayEntry& Hear(const EnemyGeneral& g);

    // 觀測驗證：三軸回真值、verified=true；planted 檔案
    // verdict 附加「（前判有詐）」。查無檔案回 false。
    bool Verify(const EnemyGeneral& g);

    const HearsayEntry* Find(const std::string& generalName) const;
    const std::vector<HearsayEntry>& Entries() const { return entries; }

private:
    std::vector<HearsayEntry> entries;
};

} // namespace Gameplay
} // namespace Potato
