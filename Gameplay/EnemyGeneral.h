#pragma once

#include "Core/CoreTypes.h"
#include "Doctrine.h"
#include "Squad.h"
#include "MathUtils/Vector2.h"

#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

class BattleController;
class Squad;

/**
 * 敵將（EnemyGeneral）——T-5 敵方指揮官的人格化腳本
 *
 * 讀取角色卡 JSON（potato.character_card/1，見 assets/cards/）：
 *   personality:  {aggression, discipline, cunning}   // 0~100 三軸
 *   signatureDoctrine: {name, trigger, threshold, action}
 *   stats:        {command, moraleAura, ...}
 *
 * 人格三軸調變生成的 doctrine：
 *   aggression → 接敵距離、撤退血線（高侵略=晚退、主動追擊）
 *   discipline → 規則冷卻、預設動作（高紀律=守位等敵，低紀律=見敵就衝）
 *   cunning    → 是否帶 AttackWeakest / DefendNearestAlly 等戰術卡
 *
 * 簽名卡（signatureDoctrine）永遠插在最高優先，是敵將的「招式」。
 */
class EnemyGeneral {
public:
    EnemyGeneral() = default;

    // 從角色卡 JSON 載入；失敗回 false
    bool LoadFromFile(const std::string& path);
    bool LoadFromString(const std::string& json);

    // 內建敵將：格洛克（斷橋原型的守將）侵略90/紀律40/狡詐10
    static EnemyGeneral MakeGlock();

    const std::string& GetName() const { return name; }
    const std::string& GetEpithet() const { return epithet; }
    float GetAggression() const { return aggression; }
    float GetDiscipline() const { return discipline; }
    float GetCunning() const { return cunning; }

    // 立繪/卡面資產路徑（卡片 "art" 欄位，相對 assets/ 根目錄）；
    // UI 顯示立繪時以此路徑載入 PNG。卡片未指定時為空字串。
    const std::string& GetArtPath() const { return artPath; }
    const std::string& GetFaction() const { return faction; }
    const std::string& GetRarity() const { return rarity; }
    const std::string& GetCardId() const { return cardId; }

    // G-2 兵種：卡片 "unit_class"（infantry/archer/cavalry）——
    // 該將全軍的兵種標記；未標記時 HasUnitClass=false 不覆寫。
    bool HasUnitClass() const { return hasUnitClass; }
    UnitClass GetUnitClass() const { return unitClass; }
    void SetUnitClass(UnitClass c) { unitClass = c; hasUnitClass = true; }

    // 為某支小隊生成 doctrine（人格 × 簽名卡 × 小隊角色）
    DoctrineSet BuildDoctrineFor(const Squad& squad, int squadRank,
                                 int squadCount) const;

    // 敵情霧先驗偏置（Q-4）：侵略高 → 偏置點推向 enemyDir；
    // 狡詐高 → 偏向 enemyDir 垂直的側翼。enemyDir 零向量時回傳
    // center（退化安全）。回傳值餵給 QuantumFog::AddEntityCloud
    // 的 biasPoint。
    Vector2 FogBiasPoint(Vector2 center, Vector2 enemyDir,
                         float radius) const;
    // 紀律 → 先驗集中度：高紀律守位 → 雲更集中（<1）；
    // 預設人格（50）回 1.0。
    float FogPriorScale() const;

    // 對整隊敵軍套用：強者帶簽名卡主攻、弱者依人格守/伏
    void ApplyTo(BattleController& battle, int team) const;

    // doctrine 卡 action 字串 → 列舉（供測試/工具直接驗證映射）
    static DoctrineAction ActionFromString(const std::string& s);

private:
    static DoctrineTrigger TriggerFromString(const std::string& s);

    std::string name = "未知敵將";
    std::string epithet;
    std::string artPath;   // 立繪 PNG（cards/art/...）
    std::string faction;
    std::string rarity;
    std::string cardId;
    float aggression = 50.0f;
    float discipline = 50.0f;
    float cunning = 50.0f;
    UnitClass unitClass = UnitClass::Infantry; // G-2
    bool hasUnitClass = false;

    // 手寫卡：signature + 附加卡（從 JSON 或 MakeGlock 填入）
    struct Card {
        std::string name;
        DoctrineRule rule;
    };
    std::vector<Card> cards;
};

} // namespace Gameplay
} // namespace Potato
