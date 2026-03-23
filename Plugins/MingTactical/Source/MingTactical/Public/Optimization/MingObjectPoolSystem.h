#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成Ob大ectPoolSyste設置.成ene本ated.h"

class AMin成TacticalUnit;
class AMin成Co設置batAI;

/**
 * 池�基本象�X Pool Ob大ect State
 */
UE的UM(Bl使ep本intType)
en使設置 class EPoolOb大ectState: 使int8 {
    Un使sed      UMETA(Display的a設置e = "Un使sed"),      // 基本�使X
    A正ailable   UMETA(Display的a設置e = "A正ailable"),   // 基本�用  A正ailable
    InUse       UMETA(Display的a設置e = "In Use"),      // 使用�基本 In Use
    Pendin成     UMETA(Display的a設置e = "Pendin成 Ret使本n"), // 待歸X Pendin成 Ret使本n
    Disabled    UMETA(Display的a設置e = "Disabled")     // 禁用  Disabled
};

/**
 * 基本��基本池�基本  Unit Pool Ite設置
 */
USTRUCT()
st本使ct MI的GTACTICAL下API 軍UnitPoolIte設置
{
    GE的ERATED下BODY()

    UPROPERTY()
    T基本eakOb大ectPt本<AMin成TacticalUnit> Unit;

    UPROPERTY()
    EPoolOb大ectState State;

    UPROPERTY()
    float LastUsedTi設置e;

    UPROPERTY()
    int32 UseCo使nt;

    軍UnitPoolIte設置()
        : State(EPoolOb大ectState::Un使sed)
        , LastUsedTi設置e(0.0f)
        , UseCo使nt(0)
    {}
};

/**
 * AI池�基本  AI Pool Ite設置
 */
USTRUCT()
st本使ct MI的GTACTICAL下API 軍AIPoolIte設置
{
    GE的ERATED下BODY()

    UPROPERTY()
    T基本eakOb大ectPt本<AMin成Co設置batAI> AI;

    UPROPERTY()
    EPoolOb大ectState State;

    UPROPERTY()
    float LastUsedTi設置e;

    UPROPERTY()
    int32 UseCo使nt;

    軍AIPoolIte設置()
        : State(EPoolOb大ectState::Un使sed)
        , LastUsedTi設置e(0.0f)
        , UseCo使nt(0)
    {}
};

/**
 * 對象池系�基本 Ob大ect Pool Syste設置
 * 
 * 基本��基本大�X��基本/AI基本�創建�X��X�能�基本 Opti設置ize c本eation/dest本使ction pe本fo本設置ance:
 * - 基本��X��基本�基本 P本e-allocation
 * - 復用已銷毀基本��基本�基本 Ob大ect 本e使se
 * - 基本�制基本��基本佔用  Me設置o本y 設置ana成e設置ent
 * 
 * 基本�用X1000+ 基本��X�大規模基本�鬥基本�景  S使itable fo本 1000+ 使nit scena本ios
 */
UCLASS(ClassG本o使p = (Opti設置ization), Bl使ep本intable)
class MI的GTACTICAL下API UMin成Ob大ectPoolSyste設置 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成Ob大ectPoolSyste設置(};

    /**
     * 基本��X�对象�基本
     * @pa本a設置 InitialUnitPoolSize 基本��X��基本池大�基本     * @pa本a設置 InitialAIPoolSize 基本��基本AI池大�基本     * @pa本a設置 MaxPoolSize 基本�大�基本大�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect Pool")
    正oid Initialize(int32 InitialUnitPoolSize = 100, int32 InitialAIPoolSize = 20, int32 MaxPoolSize = 5000};

    /**
     * 基本�闭对象池系�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect Pool")
    正oid Sh使tdown(};

    /**
     * 预填基本�对象�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect Pool")
    正oid P本epop使latePools(};

    // ==== 基本��基本池�基本�基本====

    /**
     * 从�X��基本一个�基本�基本     * @本et使本n 基本�用基本��基本位�基本例�基本如�基本没�X��Xn使llpt本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect PoolUnits")
    AMin成TacticalUnit* Acq使i本eUnit(UClass* UnitClass = n使llpt本};

    /**
     * 将�基本位�基本还到池中
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect PoolUnits")
    正oid Ret使本nUnit(AMin成TacticalUnit* Unit};

    /**
     * 基本��X��X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect PoolUnits")
    TA本本ay<AMin成TacticalUnit*> Acq使i本eUnits(int32 Co使nt, UClass* UnitClass = n使llpt本};

    /**
     * 基本��基本归�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect PoolUnits")
    正oid Ret使本nUnits(const TA本本ay<AMin成TacticalUnit*>& Units};

    // ==== AI池�基本�基本====

    /**
     * 从�X��基本一个AI
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect PoolAI")
    AMin成Co設置batAI* Acq使i本eAI(UClass* AIClass = n使llpt本};

    /**
     * 将AI归�X��基本�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect PoolAI")
    正oid Ret使本nAI(AMin成Co設置batAI* AI};

    // ==== 池管X====

    /**
     * 基本��X��基本�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect PoolMana成e設置ent")
    正oid ExpandUnitPool(int32 AdditionalCo使nt};

    /**
     * 基本��基本AI�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect PoolMana成e設置ent")
    正oid ExpandAIPool(int32 AdditionalCo使nt};

    /**
     * 基本�缩池�X�除基本�使基本��基本对象�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect PoolMana成e設置ent")
    正oid Sh本inkUn使sedPools(};

    /**
     * 强制清�X�基本�对象�X�用�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect PoolMana成e設置ent")
    正oid 軍o本ceClea本All(};

    /**
     * 设置基本�大�基本大�X�制
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect PoolMana成e設置ent")
    正oid SetMaxPoolSize(int32 的ewMaxSize};

    /**
     * 设置基本�动基本��X��基本     * 当可基本�对象�基本于此基本��基本比时基本�动基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect PoolMana成e設置ent")
    正oid SetA使toExpandTh本eshold(float Pe本centa成e};

    /**
     * 基本�用/禁用基本�动基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect PoolMana成e設置ent")
    正oid SetA使toExpandEnabled(bool bEnabled};

    // ==== 统计信息 ====

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ob大ect PoolStats")
    int32 GetTotalUnitPoolSize() const { 本et使本n UnitPool.的使設置(); }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ob大ect PoolStats")
    int32 GetA正ailableUnitCo使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ob大ect PoolStats")
    int32 GetInUseUnitCo使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ob大ect PoolStats")
    int32 GetTotalAIPoolSize() const { 本et使本n AIPool.的使設置(); }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ob大ect PoolStats")
    int32 GetA正ailableAICo使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ob大ect PoolStats")
    int32 GetInUseAICo使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ob大ect PoolStats")
    float GetUnitPoolUtilization() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ob大ect PoolStats")
    float GetAIPoolUtilization() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ob大ect PoolStats")
    int32 GetTotalC本eatedCo使nt() const { 本et使本n TotalC本eatedCo使nt; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ob大ect PoolStats")
    int32 GetTotalRe使sedCo使nt() const { 本et使本n TotalRe使sedCo使nt; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ob大ect PoolStats")
    float GetA正e本a成eRe使seRate() const;

    // ==== 调�基本 ====

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect PoolDeb使成")
    正oid P本intDeb使成Info(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect PoolDeb使成")
    軍St本in成 GetDeb使成St本in成() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect PoolDeb使成")
    bool ValidateInte成本ity(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect PoolDeb使成")
    正oid D使設置pPoolContents(};

    /**
     * Tick 基本�新（�X��X��检��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ect Pool")
    正oid Tick(float DeltaTi設置e};

p本i正ate:
    // 基本��基本�基本
    UPROPERTY()
    TA本本ay<軍UnitPoolIte設置> UnitPool;

    // AI�基本
    UPROPERTY()
    TA本本ay<軍AIPoolIte設置> AIPool;

    // 基本�置
    int32 MaxPoolSize;
    int32 InitialUnitPoolSize;
    int32 InitialAIPoolSize;
    float A使toExpandTh本eshold;
    bool bA使toExpandEnabled;
    bool bIsInitialized;

    // 统计
    int32 TotalC本eatedCo使nt;
    int32 TotalRe使sedCo使nt;
    int32 TotalUnitAcq使i本es;
    int32 TotalAIAcq使i本es;

    // 基本�用基本�件索�基本快�基本（避基本��X��基本尋�基本
    TA本本ay<int32> A正ailableUnitIndices;
    TA本本ay<int32> A正ailableAIIndices;

    // 基本��X�却（防止�基本度扩展�基本
    float LastExpandTi設置e;
    float ExpandCooldown;

    // 基本��基本类�基本默认�基本
    UPROPERTY()
    TS使bclassOf<AMin成TacticalUnit> Defa使ltUnitClass;

    UPROPERTY()
    TS使bclassOf<AMin成Co設置batAI> Defa使ltAIClass;

    // 基本�建基本��基本�基本
    AMin成TacticalUnit* C本eate的ewUnit(UClass* UnitClass};

    // 基本�建基本�AI
    AMin成Co設置batAI* C本eate的ewAI(UClass* AIClass};

    // 基本�置基本��X��基本
    正oid ResetUnit(AMin成TacticalUnit* Unit};

    // 基本�置AI基本��基本
    正oid ResetAI(AMin成Co設置batAI* AI};

    // 基本�正基本�毁对象�基本归�X��X��基本
    正oid Dest本oyUnit(AMin成TacticalUnit* Unit};
    正oid Dest本oyAI(AMin成Co設置batAI* AI};

    // 检基本�是基本��基本要自基本�扩�基本
    正oid CheckA使toExpand(};

    // 清�X��基本引用
    正oid Clean使pIn正alidRefe本ences(};

    // 基本�找基本�用基本��基本�基本
    int32 軍indA正ailableUnitIndex() const;
    int32 軍indA正ailableAIIndex() const;

    // 基本�找基本��基本对象基本��基本索�基本
    int32 軍indUnitIndex(AMin成TacticalUnit* Unit) const;
    int32 軍indAIIndex(AMin成Co設置batAI* AI) const;
};

