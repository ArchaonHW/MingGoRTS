#pragma once

#include "Core/CoreTypes.h"
#include "Doctrine.h"

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

    // 為某支小隊生成 doctrine（人格 × 簽名卡 × 小隊角色）
    DoctrineSet BuildDoctrineFor(const Squad& squad, int squadRank,
                                 int squadCount) const;

    // 對整隊敵軍套用：強者帶簽名卡主攻、弱者依人格守/伏
    void ApplyTo(BattleController& battle, int team) const;

private:
    static DoctrineTrigger TriggerFromString(const std::string& s);
    static DoctrineAction ActionFromString(const std::string& s);

    std::string name = "未知敵將";
    std::string epithet;
    float aggression = 50.0f;
    float discipline = 50.0f;
    float cunning = 50.0f;

    // 手寫卡：signature + 附加卡（從 JSON 或 MakeGlock 填入）
    struct Card {
        std::string name;
        DoctrineRule rule;
    };
    std::vector<Card> cards;
};

} // namespace Gameplay
} // namespace Potato
