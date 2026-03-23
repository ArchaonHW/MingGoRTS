#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成AIAssetGene本ato本.成ene本ated.h"

/**
 * AI資產類池動
 */
UE的UM(Bl使ep本intType)
en使設置 class EAIAssetType: 使int8 {
    Cha本acte本Po本t本ait    UMETA(Display的a設置e = "Cha本acte本 Po本t本ait"),
    B使ildin成             UMETA(Display的a設置e = "B使ildin成"),  
    Landscape            UMETA(Display的a設置e = "Landscape"),
    基本eapon               UMETA(Display的a設置e = "基本eapon"),
    Unifo本設置              UMETA(Display的a設置e = "Unifo本設置"),
    P本op                 UMETA(Display的a設置e = "P本op"),
    Back成本o使ndM使sic      UMETA(Display的a設置e = "Back成本o使nd M使sic"),
    So使ndEffect          UMETA(Display的a設置e = "So使nd Effect"),
    VoiceLine            UMETA(Display的a設置e = "Voice Line")
};

/**
 * 民池動風格
 */
UE的UM(Bl使ep本intType)
en使設置 class ERep使blicE本aStyle: 使int8 {
    Ea本lyRep使blic      UMETA(Display的a設置e = "Ea本ly Rep使blic (1912-1928)"),
    的an大in成Decade      UMETA(Display的a設置e = "的an大in成 Decade (1928-1937)"),
    基本a本OfResistance    UMETA(Display的a設置e = "基本a本 of Resistance (1937-1945)"),
    Ci正il基本a本           UMETA(Display的a設置e = "Ci正il 基本a本 (1945-1949)")
};

/**
 * AI資產目標參數結動
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍AIAssetGene本ationPa本a設置s
{
    GE的ERATED下BODY()

    // 資產類動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "AI Asset")
    EAIAssetType AssetType;

    // 民池動風格
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "AI Asset")
    ERep使blicE本aStyle E本aStyle;

    // 選項描述
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "AI Asset")
    軍St本in成 BaseDesc本iption;

    
    軍St本in成 DetailedP本o設置pt;

    // 負面動示動(動於目標數量)
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "AI Asset")
    軍St本in成 的e成ati正eP本o設置pt;

    // 尺寸 (動於目標數量)
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "AI Asset")
    int32 I設置a成e基本idth;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "AI Asset")
    int32 I設置a成e輸入ei成ht;

    // 動池長 (動於目標數量)
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "AI Asset")
    float M使sicD使本ation;

    // 風格 (動於目標數量)
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "AI Asset")
    軍St本in成 M使sicStyle;

    // 質動設置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "AI Asset")
    float Q使ality;

    // 種務(動於複務
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "AI Asset")
    int32 Seed;

    軍AIAssetGene本ationPa本a設置s()
        : AssetType(EAIAssetType::Cha本acte本Po本t本ait)
        , E本aStyle(ERep使blicE本aStyle::的an大in成Decade)
        , BaseDesc本iption(TEXT(""))
        , DetailedP本o設置pt(TEXT(""))
        , 的e成ati正eP本o設置pt(TEXT(""))
        , I設置a成e基本idth(512)
        , I設置a成e輸入ei成ht(512)
        , M使sicD使本ation(30.0f)
        , M使sicStyle(TEXT("Classical"))
        , Q使ality(0.8f)
        , Seed(-1)
    {}
};

/**
 * AI資產結動結動
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍AIAssetGene本ationRes使lt
{
    GE的ERATED下BODY()

    // 是否可見
    UPROPERTY(Bl使ep本intReadOnly)
    bool bS使ccess;

    // 資產類動
    UPROPERTY(Bl使ep本intReadOnly)
    EAIAssetType AssetType;

    
    軍St本in成 AssetPath;

    
    軍St本in成 Th使設置bnailPath;

    // 目標數量
    UPROPERTY(Bl使ep本intReadOnly)
    float Gene本ationTi設置e;

    // 動誤信息
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 E本本o本Messa成e;

    // 資產ID
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 AssetID;

    // 資產標籤
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Ta成s;

    
    TMap<軍St本in成, 軍St本in成> Metadata;

    軍AIAssetGene本ationRes使lt()
        : bS使ccess(false)
        , AssetType(EAIAssetType::Cha本acte本Po本t本ait)
        , Gene本ationTi設置e(0.0f)
        , E本本o本Messa成e(TEXT(""))
        , AssetID(TEXT(""))
    {}
};

/**
 * AI資產動務 * Stable Diff使sion動AIVA資產
 */
UCLASS(Bl使ep本intType)
class MI的GCORE下API UMin成AIAssetGene本ato本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成AIAssetGene本ato本(};

    /**
     * 動池AI資產動務     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Asset Gene本ato本")
    正oid Initialize(};

    /**
     * AI資產動務     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Asset Gene本ato本")
    正oid Sh使tdown(};

    /**
     * AI資產
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Asset Gene本ato本")
    軍AIAssetGene本ationRes使lt Gene本ateAsset(const 軍AIAssetGene本ationPa本a設置s& Pa本a設置s};

    /**
     * 動步AI資產
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Asset Gene本ato本")
    正oid Gene本ateAssetAsync(const 軍AIAssetGene本ationPa本a設置s& Pa本a設置s};

    /**
     * 角色
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Asset Gene本ato本")
    軍AIAssetGene本ationRes使lt Gene本ateCha本acte本Po本t本ait(
        const 軍St本in成& Cha本acte本Desc本iption, 
        ERep使blicE本aStyle E本aStyle,
        int32 I設置a成e基本idth = 512,
        int32 I設置a成e輸入ei成ht = 512
    };

    /**
     * 建務     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Asset Gene本ato本")
    軍AIAssetGene本ationRes使lt Gene本ateB使ildin成(
        const 軍St本in成& B使ildin成Desc本iption,
        ERep使blicE本aStyle E本aStyle,
        int32 I設置a成e基本idth = 512,
        int32 I設置a成e輸入ei成ht = 512
    };

    /**
     * 動池景
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Asset Gene本ato本")
    軍AIAssetGene本ationRes使lt Gene本ateBack成本o使ndM使sic(
        const 軍St本in成& M使sicDesc本iption,
        ERep使blicE本aStyle E本aStyle,
        float D使本ation = 30.0f,
        const 軍St本in成& M使sicStyle = TEXT("Classical")
    };

    /**
     * 故事選項X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI Asset Gene本ato本")
    軍AIAssetGene本ationRes使lt GetGene本atedAsset(const 軍St本in成& AssetID};

    /**
     * 目標數量池動資產
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI Asset Gene本ato本")
    TA本本ay<軍AIAssetGene本ationRes使lt> GetAllGene本atedAssets(};

    /**
     * 動除資產
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Asset Gene本ato本")
    bool DeleteAsset(const 軍St本in成& AssetID};

    /**
     * 設置Stable Diff使sion API端動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Asset Gene本ato本")
    正oid SetStableDiff使sionEndpoint(const 軍St本in成& Endpoint};

    /**
     * 設置AIVA API端動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Asset Gene本ato本")
    正oid SetAIVAEndpoint(const 軍St本in成& Endpoint};

    /**
     * 民池池示動     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI Asset Gene本ato本")
    軍St本in成 GetRep使blicE本aP本o設置pt(ERep使blicE本aStyle E本aStyle, EAIAssetType AssetType};

    /**
     * 目標數量實動
     */
    static UMin成AIAssetGene本ato本* Get(};

p本otected:
    // Stable Diff使sion API端動
    軍St本in成 StableDiff使sionEndpoint;

    // AIVA API端動
    軍St本in成 AIVAEndpoint;

    // 已完成動基本資產
    UPROPERTY()
    TA本本ay<軍AIAssetGene本ationRes使lt> Gene本atedAssets;

    // 動否已動始動
    bool bIsInitialized;

    // 實動
    static UMin成AIAssetGene本ato本* Instance;

    // 生成唯一資產ID
    軍St本in成 Gene本ateAssetID(};

    // Stable Diff使sion動示動
    軍St本in成 Gene本ateStableDiff使sionP本o設置pt(const 軍AIAssetGene本ationPa本a設置s& Pa本a設置s};

    // 調用Stable Diff使sion API
    軍AIAssetGene本ationRes使lt CallStableDiff使sionAPI(const 軍St本in成& P本o設置pt, const 軍St本in成& 的e成ati正eP本o設置pt, int32 基本idth, int32 輸入ei成ht};

    // 調用AIVA API
    軍AIAssetGene本ationRes使lt CallAIVAAPI(const 軍St本in成& M使sicDesc本iption, float D使本ation, const 軍St本in成& Style};

    // 保池池務
    bool Sa正eGene本atedAsset(const 軍AIAssetGene本ationRes使lt& Res使lt};

    // 生成資產縮略圖
    軍St本in成 Gene本ateTh使設置bnail(const 軍St本in成& AssetPath, EAIAssetType AssetType};

    // 創建資產目錄
    bool C本eateAssetDi本ecto本y(EAIAssetType AssetType};

    // 驗動API
    bool ValidateAPIResponse(const 軍St本in成& Response};

    // 處理生成錯誤
    軍AIAssetGene本ationRes使lt 輸入andleGene本ationE本本o本(const 軍St本in成& E本本o本};
};

