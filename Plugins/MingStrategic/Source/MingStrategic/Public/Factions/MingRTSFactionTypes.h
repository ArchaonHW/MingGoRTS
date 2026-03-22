#pragma once

#include "CoreMinimal.h"
#include "MingStrategic.generated.h"

/**
 * 12勢力類型 - 民國史詩時期主要勢力
 */
UENUM(BlueprintType)
enum class EFactionType: uint8 {
    // 核心勢力 (3個)
    BeiyangGovernment      UMETA(DisplayName = "北洋政府"),      // 北京，中央正統
    NationalistGovernment  UMETA(DisplayName = "國民政府"),      // 南京/廣州，民族主義
    ChineseCommunistParty  UMETA(DisplayName = "中國共產黨"),    // 延安/江西，人民戰爭

    // 北方軍閥 (4個)
    FengtianClique       UMETA(DisplayName = "奉系軍閥"),       // 瀋陽，東北工業
    ZhiliClique          UMETA(DisplayName = "直系軍閥"),       // 保定/洛陽，中原霸主
    AnhuiClique          UMETA(DisplayName = "皖系軍閥"),       // 天津/合肥，政治操作
    JinClique            UMETA(DisplayName = "晉系軍閥"),       // 太原，山西模範

    // 南方軍閥 (3個)
    GuangxiClique        UMETA(DisplayName = "桂系軍閥"),       // 桂林/南寧，廣西民兵
    YunnanClique         UMETA(DisplayName = "滇系軍閥"),       // 昆明，護國傳統
    SichuanClique        UMETA(DisplayName = "川系軍閥"),       // 成都/重慶，防區制

    // 邊疆勢力 (2個)
    MaFamily             UMETA(DisplayName = "馬家軍"),         // 蘭州/西寧，回族騎兵
    XinjiangFaction      UMETA(DisplayName = "新疆勢力"),       // 迪化，邊疆要塞

    // 特殊
    Invalid              UMETA(DisplayName = "無效"),
    Count                UMETA(DisplayName = "勢力數量")  // 12
};

/**
 * 勢力難度等級
 */
UENUM(BlueprintType)
enum class EFactionDifficulty: uint8 {
    Easy          UMETA(DisplayName = "容易 ★★☆☆☆"),
    Normal        UMETA(DisplayName = "普通 ★★★☆☆"),
    Hard          UMETA(DisplayName = "困難 ★★★★☆"),
    VeryHard      UMETA(DisplayName = "極難 ★★★★★"),
    Custom        UMETA(DisplayName = "自定義")
};

/**
 * 勢力特色機制類型
 */
UENUM(BlueprintType)
enum class EFactionSpecialMechanic: uint8 {
    // 政治機制
    CentralLegitimacy    UMETA(DisplayName = "中央正統"),      // 外交優勢
    Nationalism          UMETA(DisplayName = "民族主義"),      // 現代化
    PeoplesWar           UMETA(DisplayName = "人民戰爭"),      // 游擊戰
    
    // 軍事機制
    NortheastIndustry    UMETA(DisplayName = "東北工業"),      // 騎兵優勢
    CentralHegemony      UMETA(DisplayName = "中原霸主"),      // 兵力龐大
    PoliticalManeuvering UMETA(DisplayName = "政治操作"),      // 日本援助
    ShanxiModel          UMETA(DisplayName = "山西模範"),      // 防禦專精
    GuangxiMilitia       UMETA(DisplayName = "廣西民兵"),      // 山地戰
    DefenseOfNation      UMETA(DisplayName = "護國傳統"),      // 邊疆擴張
    DefenseZoneSystem    UMETA(DisplayName = "防區制"),        // 內部統一
    ReligiousUnity       UMETA(DisplayName = "宗教團結"),      // 回族騎兵
    FrontierFortress     UMETA(DisplayName = "邊疆要塞"),      // 民族複雜
    
    // 經濟機制
    IndustrialBase       UMETA(DisplayName = "工業基礎"),
    TradeNetwork         UMETA(DisplayName = "貿易網絡"),
    ResourceRich         UMETA(DisplayName = "資源豐富"),
    
    // 文化機制
    CulturalHeritage     UMETA(DisplayName = "文化傳承"),
    RegionalIdentity     UMETA(DisplayName = "地域認同"),
    
    Count
};

/**
 * 勢力關係類型
 */
UENUM(BlueprintType)
enum class EFactionRelationType: uint8 {
    Ally           UMETA(DisplayName = "同盟"),
    Friendly       UMETA(DisplayName = "友好"),
    Neutral        UMETA(DisplayName = "中立"),
    Hostile        UMETA(DisplayName = "敵對"),
    War            UMETA(DisplayName = "戰爭"),
    Vassal         UMETA(DisplayName = "附庸"),
    Suzerain       UMETA(DisplayName = "宗主")
};

/**
 * 勢力狀態
 */
UENUM(BlueprintType)
enum class EFactionState: uint8 {
    Active         UMETA(DisplayName = "活躍"),
    Defeated       UMETA(DisplayName = "被擊敗"),
    Absorbed       UMETA(DisplayName = "被吸收"),
    Allied         UMETA(DisplayName = "結盟"),
    Isolated       UMETA(DisplayName = "孤立"),
    Expanding      UMETA(DisplayName = "擴張中"),
    Defensive      UMETA(DisplayName = "防禦中")
};

/**
 * 起始地區
 */
UENUM(BlueprintType)
enum class EStartingRegion: uint8 {
    // 華北
    Beijing        UMETA(DisplayName = "北京"),
    Baoding        UMETA(DisplayName = "保定"),
    Tianjin        UMETA(DisplayName = "天津"),
    Taiyuan        UMETA(DisplayName = "太原"),
    
    // 華東/華中
    Nanjing        UMETA(DisplayName = "南京"),
    Guangzhou      UMETA(DisplayName = "廣州"),
    Luoyang        UMETA(DisplayName = "洛陽"),
    Hefei          UMETA(DisplayName = "合肥"),
    
    // 東北
    Shenyang       UMETA(DisplayName = "瀋陽"),
    
    // 華南
    Guilin         UMETA(DisplayName = "桂林"),
    Nanning        UMETA(DisplayName = "南寧"),
    
    // 西南
    Kunming        UMETA(DisplayName = "昆明"),
    Chengdu        UMETA(DisplayName = "成都"),
    Chongqing      UMETA(DisplayName = "重慶"),
    
    // 西北
    Lanzhou        UMETA(DisplayName = "蘭州"),
    Xining         UMETA(DisplayName = "西寧"),
    Dihua          UMETA(DisplayName = "迪化"),
    
    // 特殊
    Yanan          UMETA(DisplayName = "延安"),
    Jiangxi        UMETA(DisplayName = "江西"),
    
    Count
};

/**
 * 專屬單位類型
 */
UENUM(BlueprintType)
enum class EUniqueUnitType: uint8 {
    // 北洋政府
    BeiyangEliteInfantry    UMETA(DisplayName = "北洋精銳步兵"),
    ZhongnanhaiGuard        UMETA(DisplayName = "中南海衛隊"),
    
    // 國民政府
    WhampoaCadet            UMETA(DisplayName = "黃埔學員"),
    NationalistGuard        UMETA(DisplayName = "國民衛隊"),
    
    // 中國共產黨
    RedArmyVeteran          UMETA(DisplayName = "紅軍老兵"),
    PeoplesGuerrilla        UMETA(DisplayName = "人民游擊隊"),
    
    // 奉系
    NortheastCavalry        UMETA(DisplayName = "東北騎兵"),
    MukdenElite             UMETA(DisplayName = "奉天精銳"),
    
    // 直系
    ZhiliRegularArmy        UMETA(DisplayName = "直隸正規軍"),
    CentralPlainWarrior     UMETA(DisplayName = "中原勇士"),
    
    // 皖系
    AnhuiMilitia            UMETA(DisplayName = "皖系民兵"),
    PoliticalGuard          UMETA(DisplayName = "政治衛隊"),
    
    // 晉系
    ShanxiDefender          UMETA(DisplayName = "山西衛士"),
    TaiyuanElite            UMETA(DisplayName = "太原精銳"),
    
    // 桂系
    GuangxiWolf             UMETA(DisplayName = "廣西狼兵"),
    MountainFighter         UMETA(DisplayName = "山地戰士"),
    
    // 滇系
    YunnanIronBull          UMETA(DisplayName = "雲南鐵牛"),
    FrontierDefender        UMETA(DisplayName = "邊疆衛士"),
    
    // 川系
    SichuanWarlord          UMETA(DisplayName = "四川軍閥"),
    BaFighter               UMETA(DisplayName = "巴國戰士"),
    
    // 馬家軍
    HuiCavalry              UMETA(DisplayName = "回民騎兵"),
    SilkRoadWarrior         UMETA(DisplayName = "絲路勇士"),
    
    // 新疆
    XinjiangGuard          UMETA(DisplayName = "新疆衛隊"),
    FrontierVeteran          UMETA(DisplayName = "邊疆老兵"),
    
    Count
};

/**
 * 勢力AI策略類型
 */
UENUM(BlueprintType)
enum class EFactionAIStrategy: uint8 {
    AggressiveExpansion    UMETA(DisplayName = "侵略擴張"),
    DefensiveConsolidation UMETA(DisplayName = "防禦鞏固"),
    EconomicDevelopment  UMETA(DisplayName = "經濟發展"),
    DiplomaticManipulation UMETA(DisplayName = "外交操控"),
    MilitarySuperiority    UMETA(DisplayName = "軍事優勢"),
    CulturalInfluence      UMETA(DisplayName = "文化影響"),
    BalancedApproach       UMETA(DisplayName = "平衡策略"),
    AdaptiveStrategy       UMETA(DisplayName = "適應性策略")
};
