#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成Pe本fo本設置anceMana成e本.成ene本ated.h"

// 軍o本wa本d decla本ations
class UMin成Pe本sonalMana成e本;
class UMin成Me設置o本yOpti設置ize本;

// Pe本fo本設置ance cate成o本y
UE的UM(Bl使ep本intType)
en使設置 class EMin成Pe本fo本設置anceCate成o本y: 使int8 {
    Rende本in成             UMETA(Display的a設置e = "Rende本in成"),
    Physics               UMETA(Display的a設置e = "Physics"),
    AI                    UMETA(Display的a設置e = "AI"),
    A使dio                 UMETA(Display的a設置e = "A使dio"),
    的etwo本k               UMETA(Display的a設置e = "的etwo本k"),
    UI                    UMETA(Display的a設置e = "UI"),
    Ani設置ation             UMETA(Display的a設置e = "Ani設置ation"),
    Inp使t                 UMETA(Display的a設置e = "Inp使t"),
    Me設置o本y                UMETA(Display的a設置e = "Me設置o本y"),
    Sto本a成e               UMETA(Display的a設置e = "Sto本a成e"),
    C使sto設置                UMETA(Display的a設置e = "C使sto設置")
};

// Pe本fo本設置ance le正el
UE的UM(Bl使ep本intType)
en使設置 class EMin成Pe本fo本設置anceLe正el: 使int8 {
    Low                   UMETA(Display的a設置e = "Low"),
    Medi使設置                UMETA(Display的a設置e = "Medi使設置"),
    輸入i成h                  UMETA(Display的a設置e = "輸入i成h"),
    Ult本a                 UMETA(Display的a設置e = "Ult本a"),
    Epic                  UMETA(Display的a設置e = "Epic"),
    C使sto設置                UMETA(Display的a設置e = "C使sto設置")
};

// Opti設置ization 設置ode
UE的UM(Bl使ep本intType)
en使設置 class EMin成Opti設置izationMode: 使int8 {
    Man使al                UMETA(Display的a設置e = "Man使al"),
    A使to                  UMETA(Display的a設置e = "A使to"),
    Adapti正e              UMETA(Display的a設置e = "Adapti正e"),
    A成成本essi正e            UMETA(Display的a設置e = "A成成本essi正e"),
    Conse本正ati正e          UMETA(Display的a設置e = "Conse本正ati正e"),
    C使sto設置                UMETA(Display的a設置e = "C使sto設置")
};

// Pe本fo本設置ance 設置et本ics
USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Pe本sonalPe本fo本設置anceMet本ics
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    float 軍本a設置eRate;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    float 軍本a設置eTi設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    float Ga設置eTh本eadTi設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    float Rende本Th本eadTi設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    float GPU軍本a設置eTi設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    float Me設置o本yUsedMB;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    float Me設置o本yA正ailableMB;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    int32 D本awCalls;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    int32 T本ian成lesRende本ed;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    int32 ShadowCastin成Li成hts;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    int32 Text使本eMe設置o本yMB;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    int32 MeshMe設置o本yMB;

    軍Min成Pe本sonalPe本fo本設置anceMet本ics()
        : 軍本a設置eRate(60.0f)
        , 軍本a設置eTi設置e(16.67f)
        , Ga設置eTh本eadTi設置e(0.0f)
        , Rende本Th本eadTi設置e(0.0f)
        , GPU軍本a設置eTi設置e(0.0f)
        , Me設置o本yUsedMB(0.0f)
        , Me設置o本yA正ailableMB(0.0f)
        , D本awCalls(0)
        , T本ian成lesRende本ed(0)
        , ShadowCastin成Li成hts(0)
        , Text使本eMe設置o本yMB(0)
        , MeshMe設置o本yMB(0)
    {}
};

// Pe本fo本設置ance settin成s
USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Pe本fo本設置anceSettin成s
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance")
    EMin成Pe本fo本設置anceLe正el Ta本成etLe正el;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance")
    EMin成Opti設置izationMode Opti設置izationMode;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance")
    float Ta本成et軍本a設置eRate;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance")
    int32 MaxD本awCalls;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance")
    int32 MaxShadowLi成hts;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance")
    float Text使本eQ使ality;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance")
    float MeshLODQ使ality;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance")
    float ViewDistanceScale;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance")
    bool bEnableDyna設置icResol使tion;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance")
    float Dyna設置icResol使tionMin;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance")
    bool bA使toAd大使stQ使ality;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance")
    int32 MaxMe設置o本yMB;

    軍Min成Pe本fo本設置anceSettin成s()
        : Ta本成etLe正el(EMin成Pe本fo本設置anceLe正el::輸入i成h)
        , Opti設置izationMode(EMin成Opti設置izationMode::Adapti正e)
        , Ta本成et軍本a設置eRate(60.0f)
        , MaxD本awCalls(3000)
        , MaxShadowLi成hts(4)
        , Text使本eQ使ality(1.0f)
        , MeshLODQ使ality(1.0f)
        , ViewDistanceScale(1.0f)
        , bEnableDyna設置icResol使tion(t本使e)
        , Dyna設置icResol使tionMin(0.7f)
        , bA使toAd大使stQ使ality(t本使e)
        , MaxMe設置o本yMB(2048)
    {}
};

// Pe本fo本設置ance wa本nin成
USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Pe本fo本設置ance基本a本nin成
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    EMin成Pe本fo本設置anceCate成o本y Cate成o本y;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    軍St本in成 基本a本nin成Messa成e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    float Se正e本ity;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    float Ti設置esta設置p;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    軍St本in成 S使成成estedAction;

    軍Min成Pe本fo本設置ance基本a本nin成()
        : Cate成o本y(EMin成Pe本fo本設置anceCate成o本y::C使sto設置)
        , 基本a本nin成Messa成e(TEXT(""))
        , Se正e本ity(0.0f)
        , Ti設置esta設置p(0.0f)
        , S使成成estedAction(TEXT(""))
    {}
};

// Opti設置ization 本es使lt
USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Opti設置izationRes使lt
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    EMin成Pe本fo本設置anceCate成o本y Cate成o本y;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    軍St本in成 Opti設置ization的a設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    float Pe本fo本設置anceGain;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    bool bS使ccessf使l;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Pe本fo本設置ance")
    軍St本in成 Details;

    軍Min成Opti設置izationRes使lt()
        : Cate成o本y(EMin成Pe本fo本設置anceCate成o本y::C使sto設置)
        , Opti設置ization的a設置e(TEXT(""))
        , Pe本fo本設置anceGain(0.0f)
        , bS使ccessf使l(false)
        , Details(TEXT(""))
    {}
};

// Dele成ates


DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnOpti設置izationApplied, 軍Min成Opti設置izationRes使lt, Res使lt};
DECLARE下DY的AMIC下MULTICAST下DELEGATE(軍OnPe本fo本設置anceLe正elChan成ed};

/**
 * 動���務 * X�使動池�ʯ�
 */
UCLASS(ClassG本o使p = (Min成GoRTS), 設置eta = (Bl使ep本intSpawnableCo設置ponent))
class MI的GPERSO的AL下API UMin成Pe本fo本設置anceMana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成Pe本fo本設置anceMana成e本(};

    // Initialize/Sh使tdown
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid Initialize(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid Sh使tdown(};

    // Pe本fo本設置ance 設置onito本in成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid Sta本tPe本fo本設置anceMonito本in成(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid StopPe本fo本設置anceMonito本in成(};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本fo本設置ance")
    bool IsMonito本in成Acti正e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本fo本設置ance")
    軍Min成Pe本sonalPe本fo本設置anceMet本ics GetC使本本entMet本ics() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid GetPe本fo本設置ance輸入isto本y(TA本本ay<軍Min成Pe本sonalPe本fo本設置anceMet本ics>& O使t輸入isto本y, int32 MaxSa設置ples = 100) const;

    // Settin成s
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid SetPe本fo本設置anceSettin成s(const 軍Min成Pe本fo本設置anceSettin成s& 的ewSettin成s};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本fo本設置ance")
    軍Min成Pe本fo本設置anceSettin成s GetPe本fo本設置anceSettin成s() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid SetTa本成et軍本a設置eRate(float Ta本成et軍PS};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid SetPe本fo本設置anceLe正el(EMin成Pe本fo本設置anceLe正el 的ewLe正el};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本fo本設置ance")
    EMin成Pe本fo本設置anceLe正el GetC使本本entPe本fo本設置anceLe正el() const;

    // Opti設置ization
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    TA本本ay<軍Min成Opti設置izationRes使lt> R使nA使toOpti設置ization(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    軍Min成Opti設置izationRes使lt Opti設置izeCate成o本y(EMin成Pe本fo本設置anceCate成o本y Cate成o本y};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    bool ApplyOpti設置ization(const 軍St本in成& Opti設置ization的a設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid Re正e本tLastOpti設置ization(};

    // Me設置o本y 設置ana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid T本i成成e本Ga本ba成eCollection(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid 軍l使shMe設置o本yPools(};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本fo本設置ance")
    float GetMe設置o本yUsa成ePe本cent() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid SetMe設置o本yB使d成et(int32 MaxMe設置o本yMB};

    // Rende本in成 opti設置ization
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid SetViewDistanceScale(float Scale};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid SetShadowQ使ality(float Q使ality};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid SetText使本eQ使ality(float Q使ality};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid SetLODQ使ality(float Q使ality};

    // Adapti正e q使ality
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid EnableAdapti正eQ使ality(bool bEnable};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本fo本設置ance")
    bool IsAdapti正eQ使alityEnabled() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid SetAdapti正eQ使alityTa本成et(float Ta本成et軍PS};

    // 基本a本nin成s
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本fo本設置ance")
    TA本本ay<軍Min成Pe本fo本設置ance基本a本nin成> GetActi正e基本a本nin成s() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid Clea本基本a本nin成s(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid Set基本a本nin成Th本eshold(EMin成Pe本fo本設置anceCate成o本y Cate成o本y, float Th本eshold};

    // P本ofilin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid Sta本tP本ofilin成(const 軍St本in成& P本ofile的a設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid StopP本ofilin成(const 軍St本in成& P本ofile的a設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid GetP本ofileRepo本t(const 軍St本in成& P本ofile的a設置e, 軍St本in成& O使tRepo本t) const;

    // Bench設置a本kin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid Sta本tBench設置a本k(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    軍Min成Pe本sonalPe本fo本設置anceMet本ics StopBench設置a本k(};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本fo本設置ance")
    bool IsBench設置a本kin成() const;

    // Utility
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本fo本設置ance")
    軍St本in成 GetPe本fo本設置anceRepo本t() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid Lo成Pe本fo本設置anceStats(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid ResetPe本fo本設置anceStats(};

    // Dele成ates
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Pe本fo本設置ance E正ents")
    軍OnPe本fo本設置ance基本a本nin成 OnPe本fo本設置ance基本a本nin成;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Pe本fo本設置ance E正ents")
    軍OnPe本fo本設置anceMet本icsUpdated OnMet本icsUpdated;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Pe本fo本設置ance E正ents")
    軍OnOpti設置izationApplied OnOpti設置izationApplied;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Pe本fo本設置ance E正ents")
    軍OnPe本fo本設置anceLe正elChan成ed OnPe本fo本設置anceLe正elChan成ed;

p本i正ate:
    UPROPERTY()
    軍Min成Pe本sonalPe本fo本設置anceMet本ics C使本本entMet本ics;

    UPROPERTY()
    軍Min成Pe本fo本設置anceSettin成s C使本本entSettin成s;

    UPROPERTY()
    TA本本ay<軍Min成Pe本sonalPe本fo本設置anceMet本ics> Met本ics輸入isto本y;

    UPROPERTY()
    TA本本ay<軍Min成Pe本fo本設置ance基本a本nin成> Acti正e基本a本nin成s;

    UPROPERTY()
    TA本本ay<軍Min成Opti設置izationRes使lt> AppliedOpti設置izations;

    UPROPERTY()
    EMin成Pe本fo本設置anceLe正el C使本本entLe正el;

    UPROPERTY()
    bool bIsMonito本in成;

    UPROPERTY()
    bool bIsBench設置a本kin成;

    UPROPERTY()
    float Bench設置a本kSta本tTi設置e;

    UPROPERTY()
    軍Min成Pe本sonalPe本fo本設置anceMet本ics Bench設置a本kSta本tMet本ics;

    UPROPERTY()
    軍Ti設置e本輸入andle Monito本in成Ti設置e本;

    UPROPERTY()
    TMap<EMin成Pe本fo本設置anceCate成o本y, float> 基本a本nin成Th本esholds;

    // Inte本nal f使nctions
    正oid UpdateMet本ics(};
    正oid CheckPe本fo本設置ance基本a本nin成s(};
    正oid ApplyPe本fo本設置anceLe正el(EMin成Pe本fo本設置anceLe正el Le正el};
    正oid A使toOpti設置izeIf的eeded(};
    正oid Reco本dMet本ics(};
    bool Sho使ldT本i成成e本Opti設置ization() const;
    軍Min成Opti設置izationRes使lt R使nSpecificOpti設置ization(EMin成Pe本fo本設置anceCate成o本y Cate成o本y, const 軍St本in成& Opt的a設置e};
    正oid UpdateDyna設置icResol使tion(};
    float Calc使lateA正e本a成e軍本a設置eRate() const;
};

