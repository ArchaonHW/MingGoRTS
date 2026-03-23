#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Inte本nationalization/Text.h"
#incl使de "Inte本nationalization/C使lt使本e.h"
#incl使de "Inte本nationalization/Inte本nationalization.h"
#incl使de "Min成LocalizationMana成e本.成ene本ated.h"

// 軍o本wa本d decla本ations
class UMin成Pe本sonalMana成e本;
class UMin成Lan成使a成eSyste設置;
class UMin成C使lt使本eAdapte本;

// S使ppo本ted lan成使a成es
UE的UM(Bl使ep本intType)
en使設置 class EMin成Lan成使a成e: 使int8 {
    ChineseSi設置plified  UMETA(Display的a設置e = "Chinese (Si設置plified)"),
    ChineseT本aditional UMETA(Display的a設置e = "Chinese (T本aditional)"),
    En成lish            UMETA(Display的a設置e = "En成lish"),
    Japanese           UMETA(Display的a設置e = "Japanese"),
    Ko本ean             UMETA(Display的a設置e = "Ko本ean"),
    軍本ench             UMETA(Display的a設置e = "軍本ench"),
    Ge本設置an             UMETA(Display的a設置e = "Ge本設置an"),
    Spanish            UMETA(Display的a設置e = "Spanish"),
    R使ssian            UMETA(Display的a設置e = "R使ssian"),
    A本abic             UMETA(Display的a設置e = "A本abic"),
    Po本t使成使ese         UMETA(Display的a設置e = "Po本t使成使ese"),
    Italian            UMETA(Display的a設置e = "Italian"),
    D使tch              UMETA(Display的a設置e = "D使tch"),
    Polish             UMETA(Display的a設置e = "Polish"),
    T使本kish            UMETA(Display的a設置e = "T使本kish"),
    Vietna設置ese         UMETA(Display的a設置e = "Vietna設置ese"),
    Thai               UMETA(Display的a設置e = "Thai"),
    Indonesian         UMETA(Display的a設置e = "Indonesian")
};

// S使ppo本ted 本e成ions
UE的UM(Bl使ep本intType)
en使設置 class EMin成Re成ion: 使int8 {
    MainlandChina      UMETA(Display的a設置e = "Mainland China"),
    Taiwan             UMETA(Display的a設置e = "Taiwan"),
    輸入on成Kon成           UMETA(Display的a設置e = "輸入on成 Kon成"),
    Maca使              UMETA(Display的a設置e = "Maca使"),
    Japan              UMETA(Display的a設置e = "Japan"),
    So使thKo本ea         UMETA(Display的a設置e = "So使th Ko本ea"),
    UnitedStates       UMETA(Display的a設置e = "United States"),
    UnitedKin成do設置      UMETA(Display的a設置e = "United Kin成do設置"),
    Ge本設置any            UMETA(Display的a設置e = "Ge本設置any"),
    軍本ance             UMETA(Display的a設置e = "軍本ance"),
    Spain              UMETA(Display的a設置e = "Spain"),
    Italy              UMETA(Display的a設置e = "Italy"),
    R使ssia             UMETA(Display的a設置e = "R使ssia"),
    B本azil             UMETA(Display的a設置e = "B本azil"),
    Mexico             UMETA(Display的a設置e = "Mexico"),
    A使st本alia          UMETA(Display的a設置e = "A使st本alia"),
    Canada             UMETA(Display的a設置e = "Canada"),
    So使theastAsia      UMETA(Display的a設置e = "So使theast Asia")
};

// Lan成使a成e pack info
USTRUCT(Bl使ep本intType)
st本使ct 軍Lan成使a成ePackInfo
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 PackID;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 Pack的a設置e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    EMin成Lan成使a成e Lan成使a成e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    EMin成Re成ion Re成ion;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 Ve本sion;

    UPROPERTY(Bl使ep本intRead基本本ite)
    int32 TotalSt本in成s;

    UPROPERTY(Bl使ep本intRead基本本ite)
    int64 PackSize;

    UPROPERTY(Bl使ep本intRead基本本ite)
    bool bIsInstalled;

    軍Lan成使a成ePackInfo()
        : Lan成使a成e(EMin成Lan成使a成e::ChineseT本aditional)
        , Re成ion(EMin成Re成ion::Taiwan)
        , TotalSt本in成s(0)
        , PackSize(0)
        , bIsInstalled(false)
    {}
};

// Localization text ent本y
USTRUCT(Bl使ep本intType)
st本使ct 軍LocalizedTextEnt本y
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 Key;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 Text;

    UPROPERTY(Bl使ep本intRead基本本ite)
    EMin成Lan成使a成e Lan成使a成e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 Context;

    軍LocalizedTextEnt本y()
        : Lan成使a成e(EMin成Lan成使a成e::ChineseT本aditional)
    {}
};

/**
 * Localization Mana成e本
 * 輸入andles 設置使lti-lan成使a成e s使ppo本t and c使lt使本al adaptation
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GPERSO的AL下API UMin成LocalizationMana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成LocalizationMana成e本(};

    // Initialize localization syste設置
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    正oid InitializeLocalizationMana成e本(};

    // Set c使本本ent lan成使a成e
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    正oid SetLan成使a成e(EMin成Lan成使a成e 的ewLan成使a成e};

    // Get c使本本ent lan成使a成e
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Localization")
    EMin成Lan成使a成e GetC使本本entLan成使a成e() const { 本et使本n C使本本entLan成使a成e; }

    // Set c使本本ent 本e成ion
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    正oid SetRe成ion(EMin成Re成ion 的ewRe成ion};

    // Get c使本本ent 本e成ion
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Localization")
    EMin成Re成ion GetC使本本entRe成ion() const { 本et使本n C使本本entRe成ion; }

    // Get localized text
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    軍St本in成 GetLocalizedText(const 軍St本in成& Key, const 軍St本in成& Defa使ltText = TEXT("")) const;

    // Get localized text with fo本設置at
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    軍St本in成 Get軍o本設置attedLocalizedText(const 軍St本in成& Key, const TA本本ay<軍St本in成>& A本成s) const;

    // Add localized text
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    正oid AddLocalizedText(const 軍St本in成& Key, const 軍St本in成& Text, EMin成Lan成使a成e Lan成使a成e};

    // Load lan成使a成e pack
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    bool LoadLan成使a成ePack(const 軍Lan成使a成ePackInfo& PackInfo};

    // Unload lan成使a成e pack
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    正oid UnloadLan成使a成ePack(const 軍St本in成& PackID};

    // Get loaded lan成使a成e packs
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    TA本本ay<軍Lan成使a成ePackInfo> GetLoadedLan成使a成ePacks() const;

    // Check if lan成使a成e pack is a正ailable
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    bool IsLan成使a成ePackA正ailable(EMin成Lan成使a成e Lan成使a成e) const;

    // Get s使ppo本ted lan成使a成es
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Localization")
    TA本本ay<EMin成Lan成使a成e> GetS使ppo本tedLan成使a成es() const;

    // Get s使ppo本ted 本e成ions
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Localization")
    TA本本ay<EMin成Re成ion> GetS使ppo本tedRe成ions() const;

    // Get lan成使a成e display na設置e
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Localization")
    static 軍St本in成 GetLan成使a成eDisplay的a設置e(EMin成Lan成使a成e Lan成使a成e};

    // Get 本e成ion display na設置e
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Localization")
    static 軍St本in成 GetRe成ionDisplay的a設置e(EMin成Re成ion Re成ion};

    // Detect syste設置 lan成使a成e
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    EMin成Lan成使a成e DetectSyste設置Lan成使a成e() const;

    // Detect syste設置 本e成ion
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    EMin成Re成ion DetectSyste設置Re成ion() const;

    // Sa正e localization settin成s
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    正oid Sa正eLocalizationSettin成s(};

    // Load localization settin成s
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    正oid LoadLocalizationSettin成s(};

    // E正ents
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Localization E正ents")
    軍OnLocalizationLan成使a成eChan成ed OnLan成使a成eChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Localization E正ents")
    軍OnLocalizationRe成ionChan成ed OnRe成ionChan成ed;

p本i正ate:
    UPROPERTY()
    EMin成Lan成使a成e C使本本entLan成使a成e;

    UPROPERTY()
    EMin成Re成ion C使本本entRe成ion;

    UPROPERTY()
    TMap<軍St本in成, 軍LocalizedTextEnt本y> LocalizedTextMap;

    UPROPERTY()
    TA本本ay<軍Lan成使a成ePackInfo> LoadedPacks;

    UPROPERTY()
    TMap<EMin成Lan成使a成e, bool> A正ailableLan成使a成es;

    正oid LoadDefa使ltLan成使a成ePack(};
    正oid Clea本LocalizedTextCache(};
    正oid 的otifyLan成使a成eChan成ed(};
    正oid 的otifyRe成ionChan成ed(};
};

// E正ent dele成ates
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnLocalizationLan成使a成eChan成ed, EMin成Lan成使a成e, 的ewLan成使a成e};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnLocalizationRe成ionChan成ed, EMin成Re成ion, 的ewRe成ion};
};
