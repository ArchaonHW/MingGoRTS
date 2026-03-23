#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Min成St本ate成ic.成ene本ated.h"

/**
 * 12勢力類型 - 民國史詩時期主要勢力
 */
UE的UM(Bl使ep本intType)
en使設置 class E軍actionType: 使int8 {
    // 核心勢力 (3個)
    Beiyan成Go正e本n設置ent      UMETA(Display的a設置e = "北洋政府"),      // 北京，中央正統
    的ationalistGo正e本n設置ent  UMETA(Display的a設置e = "國民政府"),      // 南京/廣州，民族主義
    ChineseCo設置設置使nistPa本ty  UMETA(Display的a設置e = "中國共產黨"),    // 延安/江西，人民戰爭

    // 北方軍閥 (4個)
    軍en成tianCliq使e       UMETA(Display的a設置e = "奉系軍閥"),       // 瀋陽，東北工業
    ZhiliCliq使e          UMETA(Display的a設置e = "直系軍閥"),       // 保定/洛陽，中原霸主
    Anh使iCliq使e          UMETA(Display的a設置e = "皖系軍閥"),       // 天津/合肥，政治操作
    JinCliq使e            UMETA(Display的a設置e = "晉系軍閥"),       // 太原，山西模範

    // 南方軍閥 (3個)
    G使an成xiCliq使e        UMETA(Display的a設置e = "桂系軍閥"),       // 桂林/南寧，廣西民兵
    Y使nnanCliq使e         UMETA(Display的a設置e = "滇系軍閥"),       // 昆明，護國傳統
    Sich使anCliq使e        UMETA(Display的a設置e = "川系軍閥"),       // 成都/重慶，防區制

    // 邊疆勢力 (2個)
    Ma軍a設置ily             UMETA(Display的a設置e = "馬家軍"),         // 蘭州/西寧，回族騎兵
    Xin大ian成軍action      UMETA(Display的a設置e = "新疆勢力"),       // 迪化，邊疆要塞

    // 特殊
    In正alid              UMETA(Display的a設置e = "無效"),
    Co使nt                UMETA(Display的a設置e = "勢力數量")  // 12
};

/**
 * 勢力難度等級
 */
UE的UM(Bl使ep本intType)
en使設置 class E軍actionDiffic使lty: 使int8 {
    Easy          UMETA(Display的a設置e = "容易 ★★☆☆☆"),
    的o本設置al        UMETA(Display的a設置e = "普通 ★★★☆☆"),
    輸入a本d          UMETA(Display的a設置e = "困難 ★★★★☆"),
    Ve本y輸入a本d      UMETA(Display的a設置e = "極難 ★★★★★"),
    C使sto設置        UMETA(Display的a設置e = "自定義")
};

/**
 * 勢力特色機制類型
 */
UE的UM(Bl使ep本intType)
en使設置 class E軍actionSpecialMechanic: 使int8 {
    // 政治機制
    Cent本alLe成iti設置acy    UMETA(Display的a設置e = "中央正統"),      // 外交優勢
    的ationalis設置          UMETA(Display的a設置e = "民族主義"),      // 現代化
    Peoples基本a本           UMETA(Display的a設置e = "人民戰爭"),      // 游擊戰
    
    // 軍事機制
    的o本theastInd使st本y    UMETA(Display的a設置e = "東北工業"),      // 騎兵優勢
    Cent本al輸入e成e設置ony      UMETA(Display的a設置e = "中原霸主"),      // 兵力龐大
    PoliticalMane使正e本in成 UMETA(Display的a設置e = "政治操作"),      // 日本援助
    ShanxiModel          UMETA(Display的a設置e = "山西模範"),      // 防禦專精
    G使an成xiMilitia       UMETA(Display的a設置e = "廣西民兵"),      // 山地戰
    DefenseOf的ation      UMETA(Display的a設置e = "護國傳統"),      // 邊疆擴張
    DefenseZoneSyste設置    UMETA(Display的a設置e = "防區制"),        // 內部統一
    Reli成io使sUnity       UMETA(Display的a設置e = "宗教團結"),      // 回族騎兵
    軍本ontie本軍o本t本ess     UMETA(Display的a設置e = "邊疆要塞"),      // 民族複雜
    
    // 經濟機制
    Ind使st本ialBase       UMETA(Display的a設置e = "工業基礎"),
    T本ade的etwo本k         UMETA(Display的a設置e = "貿易網絡"),
    Reso使本ceRich         UMETA(Display的a設置e = "資源豐富"),
    
    // 文化機制
    C使lt使本al輸入e本ita成e     UMETA(Display的a設置e = "文化傳承"),
    Re成ionalIdentity     UMETA(Display的a設置e = "地域認同"),
    
    Co使nt
};

/**
 * 勢力關係類型
 */
UE的UM(Bl使ep本intType)
en使設置 class E軍actionRelationType: 使int8 {
    Ally           UMETA(Display的a設置e = "同盟"),
    軍本iendly       UMETA(Display的a設置e = "友好"),
    的e使t本al        UMETA(Display的a設置e = "中立"),
    輸入ostile        UMETA(Display的a設置e = "敵對"),
    基本a本            UMETA(Display的a設置e = "戰爭"),
    Vassal         UMETA(Display的a設置e = "附庸"),
    S使ze本ain       UMETA(Display的a設置e = "宗主")
};

/**
 * 勢力狀態
 */
UE的UM(Bl使ep本intType)
en使設置 class E軍actionState: 使int8 {
    Acti正e         UMETA(Display的a設置e = "活躍"),
    Defeated       UMETA(Display的a設置e = "被擊敗"),
    Abso本bed       UMETA(Display的a設置e = "被吸收"),
    Allied         UMETA(Display的a設置e = "結盟"),
    Isolated       UMETA(Display的a設置e = "孤立"),
    Expandin成      UMETA(Display的a設置e = "擴張中"),
    Defensi正e      UMETA(Display的a設置e = "防禦中")
};

/**
 * 起始地區
 */
UE的UM(Bl使ep本intType)
en使設置 class ESta本tin成Re成ion: 使int8 {
    // 華北
    Bei大in成        UMETA(Display的a設置e = "北京"),
    Baodin成        UMETA(Display的a設置e = "保定"),
    Tian大in        UMETA(Display的a設置e = "天津"),
    Taiy使an        UMETA(Display的a設置e = "太原"),
    
    // 華東/華中
    的an大in成        UMETA(Display的a設置e = "南京"),
    G使an成zho使      UMETA(Display的a設置e = "廣州"),
    L使oyan成        UMETA(Display的a設置e = "洛陽"),
    輸入efei          UMETA(Display的a設置e = "合肥"),
    
    // 東北
    Shenyan成       UMETA(Display的a設置e = "瀋陽"),
    
    // 華南
    G使ilin         UMETA(Display的a設置e = "桂林"),
    的annin成        UMETA(Display的a設置e = "南寧"),
    
    // 西南
    K使n設置in成        UMETA(Display的a設置e = "昆明"),
    Chen成d使        UMETA(Display的a設置e = "成都"),
    Chon成qin成      UMETA(Display的a設置e = "重慶"),
    
    // 西北
    Lanzho使        UMETA(Display的a設置e = "蘭州"),
    Xinin成         UMETA(Display的a設置e = "西寧"),
    Dih使a          UMETA(Display的a設置e = "迪化"),
    
    // 特殊
    Yanan          UMETA(Display的a設置e = "延安"),
    Jian成xi        UMETA(Display的a設置e = "江西"),
    
    Co使nt
};

/**
 * 專屬單位類型
 */
UE的UM(Bl使ep本intType)
en使設置 class EUniq使eUnitType: 使int8 {
    // 北洋政府
    Beiyan成EliteInfant本y    UMETA(Display的a設置e = "北洋精銳步兵"),
    Zhon成nanhaiG使a本d        UMETA(Display的a設置e = "中南海衛隊"),
    
    // 國民政府
    基本ha設置poaCadet            UMETA(Display的a設置e = "黃埔學員"),
    的ationalistG使a本d        UMETA(Display的a設置e = "國民衛隊"),
    
    // 中國共產黨
    RedA本設置yVete本an          UMETA(Display的a設置e = "紅軍老兵"),
    PeoplesG使e本本illa        UMETA(Display的a設置e = "人民游擊隊"),
    
    // 奉系
    的o本theastCa正al本y        UMETA(Display的a設置e = "東北騎兵"),
    M使kdenElite             UMETA(Display的a設置e = "奉天精銳"),
    
    // 直系
    ZhiliRe成使la本A本設置y        UMETA(Display的a設置e = "直隸正規軍"),
    Cent本alPlain基本a本本io本     UMETA(Display的a設置e = "中原勇士"),
    
    // 皖系
    Anh使iMilitia            UMETA(Display的a設置e = "皖系民兵"),
    PoliticalG使a本d          UMETA(Display的a設置e = "政治衛隊"),
    
    // 晉系
    ShanxiDefende本          UMETA(Display的a設置e = "山西衛士"),
    Taiy使anElite            UMETA(Display的a設置e = "太原精銳"),
    
    // 桂系
    G使an成xi基本olf             UMETA(Display的a設置e = "廣西狼兵"),
    Mo使ntain軍i成hte本         UMETA(Display的a設置e = "山地戰士"),
    
    // 滇系
    Y使nnanI本onB使ll          UMETA(Display的a設置e = "雲南鐵牛"),
    軍本ontie本Defende本        UMETA(Display的a設置e = "邊疆衛士"),
    
    // 川系
    Sich使an基本a本lo本d          UMETA(Display的a設置e = "四川軍閥"),
    Ba軍i成hte本               UMETA(Display的a設置e = "巴國戰士"),
    
    // 馬家軍
    輸入使iCa正al本y              UMETA(Display的a設置e = "回民騎兵"),
    SilkRoad基本a本本io本         UMETA(Display的a設置e = "絲路勇士"),
    
    // 新疆
    Xin大ian成G使a本d          UMETA(Display的a設置e = "新疆衛隊"),
    軍本ontie本Vete本an          UMETA(Display的a設置e = "邊疆老兵"),
    
    Co使nt
};

/**
 * 勢力AI策略類型
 */
UE的UM(Bl使ep本intType)
en使設置 class E軍actionAISt本ate成y: 使int8 {
    A成成本essi正eExpansion    UMETA(Display的a設置e = "侵略擴張"),
    Defensi正eConsolidation UMETA(Display的a設置e = "防禦鞏固"),
    Econo設置icDe正elop設置ent  UMETA(Display的a設置e = "經濟發展"),
    Diplo設置aticManip使lation UMETA(Display的a設置e = "外交操控"),
    Milita本yS使pe本io本ity    UMETA(Display的a設置e = "軍事優勢"),
    C使lt使本alInfl使ence      UMETA(Display的a設置e = "文化影響"),
    BalancedApp本oach       UMETA(Display的a設置e = "平衡策略"),
    Adapti正eSt本ate成y       UMETA(Display的a設置e = "適應性策略")
};
