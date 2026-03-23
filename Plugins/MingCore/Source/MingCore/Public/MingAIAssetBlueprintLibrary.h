#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Kis設置et/Bl使ep本int軍使nctionLib本a本y.h"
#incl使de "Min成AIAssetBl使ep本intLib本a本y.成ene本ated.h"

class UMin成AIAssetGene本ato本;
class UMin成AIAssetMana成e本;

/**
 * AI�겣動池��動 * 故事重要性基本��AI�겣動池��動務 */
UCLASS()
class MI的GCORE下API UMin成AIAssetBl使ep本intLib本a本y : p使blic UBl使ep本int軍使nctionLib本a本y
{
    GE的ERATED下BODY()

p使blic:
    /**
     * AI�겣動務     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI Asset")
    static UMin成AIAssetGene本ato本* GetAIAssetGene本ato本(};

    /**
     * AI�겣��務     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI Asset")
    static UMin成AIAssetMana成e本* GetAIAssetMana成e本(};

    /**
     * 動池AI�겣�t��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Asset")
    static 正oid InitializeAIAssetSyste設置(};

    /**
     * ����
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI AssetCha本acte本s")
    static 軍St本in成 Gene本ateCha本acte本Po本t本ait(
        const 軍St本in成& Cha本acte本Desc本iption, 
        ERep使blicE本aStyle E本aStyle = ERep使blicE本aStyle::的an大in成Decade,
        int32 I設置a成e基本idth = 512,
        int32 I設置a成e輸入ei成ht = 512
    };

    /**
     * ��務     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI AssetB使ildin成s")
    static 軍St本in成 Gene本ateB使ildin成(
        const 軍St本in成& B使ildin成Desc本iption,
        ERep使blicE本aStyle E本aStyle = ERep使blicE本aStyle::的an大in成Decade,
        int32 I設置a成e基本idth = 512,
        int32 I設置a成e輸入ei成ht = 512
    };

    /**
     * �Z��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Asset基本eapons")
    static 軍St本in成 Gene本ate基本eapon(
        const 軍St本in成& 基本eaponDesc本iption,
        ERep使blicE本aStyle E本aStyle = ERep使blicE本aStyle::的an大in成Decade,
        int32 I設置a成e基本idth = 512,
        int32 I設置a成e輸入ei成ht = 512
    };

    /**
     * �x動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI AssetUnifo本設置s")
    static 軍St本in成 Gene本ateUnifo本設置(
        const 軍St本in成& Unifo本設置Desc本iption,
        ERep使blicE本aStyle E本aStyle = ERep使blicE本aStyle::的an大in成Decade,
        int32 I設置a成e基本idth = 512,
        int32 I設置a成e輸入ei成ht = 512
    };

    /**
     * 動池��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI AssetM使sic")
    static 軍St本in成 Gene本ateBack成本o使ndM使sic(
        const 軍St本in成& M使sicDesc本iption,
        ERep使blicE本aStyle E本aStyle = ERep使blicE本aStyle::的an大in成Decade,
        float D使本ation = 30.0f,
        const 軍St本in成& M使sicStyle = TEXT("Classical")
    };

    /**
     * 動池��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI AssetScenes")
    static 軍St本in成 Gene本ateScene(
        const 軍St本in成& SceneDesc本iption,
        ERep使blicE本aStyle E本aStyle = ERep使blicE本aStyle::的an大in成Decade,
        int32 I設置a成e基本idth = 1024,
        int32 I設置a成e輸入ei成ht = 512
    };

    /**
     * ��池池��動     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI AssetP本o設置pts")
    static 軍St本in成 GetRep使blicE本aP本o設置pt(ERep使blicE本aStyle E本aStyle, EAIAssetType AssetType};

    /**
     * ����動池��動     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI AssetP本o設置pts")
    static 軍St本in成 GetCha本acte本Po本t本aitP本o設置pt(ERep使blicE本aStyle E本aStyle, const 軍St本in成& Cha本acte本Type) {};

    /**
     * ��池動��動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI AssetP本o設置pts")
    static 軍St本in成 GetB使ildin成P本o設置pt(ERep使blicE本aStyle E本aStyle, const 軍St本in成& B使ildin成Type) {};

    /**
     * �Z��動��動     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI AssetP本o設置pts")
    static 軍St本in成 Get基本eaponP本o設置pt(ERep使blicE本aStyle E本aStyle, const 軍St本in成& 基本eaponType) {};

    /**
     * 故事重要性基本��動     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI AssetP本o設置pts")
    static 軍St本in成 GetM使sicP本o設置pt(ERep使blicE本aStyle E本aStyle, const 軍St本in成& M使sicMood) {};

    /**
     * 動���겣
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI AssetMana成e設置ent")
    static TA本本ay<軍St本in成> Sea本chAssets(
        const 軍St本in成& Sea本chKeywo本d = TEXT(""),
        EAssetCate成o本y Cate成o本y = EAssetCate成o本y::Cha本acte本s,
        ERep使blicE本aStyle E本aStyle = ERep使blicE本aStyle::的an大in成Decade,
        float MinRatin成 = 0.0f,
        bool b軍a正o本itesOnly = false
    };

    /**
     * �겣�輸入��
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI AssetMana成e設置ent")
    static 軍St本in成 GetAssetInfo(const 軍St本in成& AssetID) {};

    /**
     * ��動�겣動��X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI AssetMana成e設置ent")
    static bool SetAsset軍a正o本ite(const 軍St本in成& AssetID, bool b軍a正o本ite};

    /**
     * ��動�겣
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI AssetMana成e設置ent")
    static bool RateAsset(const 軍St本in成& AssetID, float Ratin成};

    /**
     * �基本X�겣
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI AssetMana成e設置ent")
    static bool Expo本tAsset(const 軍St本in成& AssetID, const 軍St本in成& Expo本tPath) {};

    /**
     * �겣��動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI AssetMana成e設置ent")
    static TMap<EAssetCate成o本y, int32> GetAssetStatistics(};

    /**
     * 動池�˸겣
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI AssetMana成e設置ent")
    static TA本本ay<軍St本in成> GetReco設置設置endedAssets(EAssetCate成o本y Cate成o本y, int32 Co使nt = 10};

    /**
     * �s�x�ϥ�
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI AssetMana成e設置ent")
    static float GetSto本a成eUsa成eMB(};

    /**
     * �M池動�겣
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI AssetMana成e設置ent")
    static int32 Clean使pIn正alidAssets(};

    /**
     * 目標數量����動務     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI AssetBatch Gene本ation")
    static TA本本ay<軍St本in成> Gene本ateCha本acte本Po本t本aitSet(
        const TA本本ay<軍St本in成>& Cha本acte本Desc本iptions,
        ERep使blicE本aStyle E本aStyle = ERep使blicE本aStyle::的an大in成Decade
    };

    /**
     * 目標數量��池動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI AssetBatch Gene本ation")
    static TA本本ay<軍St本in成> Gene本ateB使ildin成Set(
        const TA本本ay<軍St本in成>& B使ildin成Desc本iptions,
        ERep使blicE本aStyle E本aStyle = ERep使blicE本aStyle::的an大in成Decade
    };

    /**
     * 故事選項X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI AssetBatch Gene本ation")
    static TA本本ay<軍St本in成> Gene本ateM使sicSet(
        const TA本本ay<軍St本in成>& M使sicDesc本iptions,
        ERep使blicE本aStyle E本aStyle = ERep使blicE本aStyle::的an大in成Decade,
        float D使本ation = 30.0f
    };

    /**
     * �]�設置Stable Diff使sion��動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI AssetConfi成使本ation")
    static 正oid SetStableDiff使sionEndpoint(const 軍St本in成& Endpoint) {};

    /**
     * �]�設置AIVA��動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI AssetConfi成使本ation")
    static 正oid SetAIVAEndpoint(const 軍St本in成& Endpoint) {};

    /**
     * �t��動X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI AssetSyste設置")
    static bool IsAIAssetSyste設置Initialized(};

    /**
     * 故事選項X務     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI AssetSyste設置")
    static int32 GetGene本ationQ使e使eCo使nt(};

    /**
     * 目標數量池��X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI AssetSyste設置")
    static 正oid CancelAllGene本ations(};
};

