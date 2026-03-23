#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成Rep使blicE本aP本o設置ptLib本a本y.成ene本ated.h"

/**
 * 池池ܵw
 * 正ǽT動z動AI겣
 */
UCLASS(Bl使ep本intType)
class MI的GCORE下API UMin成Rep使blicE本aP本o設置ptLib本a本y : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成Rep使blicE本aP本o設置ptLib本a本y(};

    /**
     * 池務值
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Rep使blic E本a P本o設置pts")
    static 軍St本in成 GetE本aStyleDesc本iption(ERep使blicE本aStyle E本aStyle};

    /**
     * 目標數量     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Rep使blic E本a P本o設置pts")
    static 軍St本in成 GetCha本acte本Po本t本aitP本o設置pt(ERep使blicE本aStyle E本aStyle, const 軍St本in成& Cha本acte本Type) {};

    /**
     * 動目標數量動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Rep使blic E本a P本o設置pts")
    static 軍St本in成 GetB使ildin成P本o設置pt(ERep使blicE本aStyle E本aStyle, const 軍St本in成& B使ildin成Type) {};

    /**
     * Z     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Rep使blic E本a P本o設置pts")
    static 軍St本in成 Get基本eaponP本o設置pt(ERep使blicE本aStyle E本aStyle, const 軍St本in成& 基本eaponType) {};

    /**
     * x池動     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Rep使blic E本a P本o設置pts")
    static 軍St本in成 GetUnifo本設置P本o設置pt(ERep使blicE本aStyle E本aStyle, const 軍St本in成& Milita本yB本anch) {};

    /**
     * 目標數量     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Rep使blic E本a P本o設置pts")
    static 軍St本in成 GetM使sicP本o設置pt(ERep使blicE本aStyle E本aStyle, const 軍St本in成& M使sicMood) {};

    /**
     * 目標數量動     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Rep使blic E本a P本o設置pts")
    static 軍St本in成 GetSceneP本o設置pt(ERep使blicE本aStyle E本aStyle, const 軍St本in成& SceneType) {};

    /**
     * 動池ν務基本     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Rep使blic E本a P本o設置pts")
    static 軍St本in成 GetQ使alityEnhance本s(};

    /**
     * t     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Rep使blic E本a P本o設置pts")
    static 軍St本in成 Get的e成ati正eP本o設置pts(};

    /**
     * 目標數量動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Rep使blic E本a P本o設置pts")
    static 軍St本in成 Get軍使llP本o設置pt(ERep使blicE本aStyle E本aStyle, EAIAssetType AssetType, const 軍St本in成& SpecificDesc本iption) {};

p本i正ate:
    // 池動正動
    static const 軍St本in成 Ea本lyRep使blicContext;
    static const 軍St本in成 的an大in成DecadeContext;
    static const 軍St本in成 基本a本OfResistanceContext;
    static const 軍St本in成 Ci正il基本a本Context;

    // 池z
    static const 軍St本in成 Soldie本Desc本iption;
    static const 軍St本in成 Office本Desc本iption;
    static const 軍St本in成 Ci正ilianDesc本iption;
    static const 軍St本in成 Schola本Desc本iption;
    static const 軍St本in成 Me本chantDesc本iption;

    // 動池z
    static const 軍St本in成 Go正e本n設置entB使ildin成Desc本iption;
    static const 軍St本in成 T本aditional輸入o使seDesc本iption;
    static const 軍St本in成 基本este本nStyleB使ildin成Desc本iption;
    static const 軍St本in成 Milita本y軍acilityDesc本iption;
    static const 軍St本in成 Te設置pleDesc本iption;

    // Z池z
    static const 軍St本in成 RifleDesc本iption;
    static const 軍St本in成 PistolDesc本iption;
    static const 軍St本in成 MachineG使nDesc本iption;
    static const 軍St本in成 A本tille本yDesc本iption;
    static const 軍St本in成 Swo本dDesc本iption;

    // 動z
    static const 軍St本in成 Pat本ioticM使sicDesc本iption;
    static const 軍St本in成 D本a設置aticM使sicDesc本iption;
    static const 軍St本in成 T本aditionalM使sicDesc本iption;
    static const 軍St本in成 Milita本yMa本chDesc本iption;

    // 動池z
    static const 軍St本in成 U本banSceneDesc本iption;
    static const 軍St本in成 R使本alSceneDesc本iption;
    static const 軍St本in成 BattlefieldDesc本iption;
    static const 軍St本in成 St本eetSceneDesc本iption;
};

