#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成Ecosyste設置BalanceSyste設置.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class ESpeciesType: 使int8 {
    軍lo本a下T本ee          UMETA(Display的a設置e = "树木"),
    軍lo本a下G本ass         UMETA(Display的a設置e = "草类"),
    軍lo本a下C本op          UMETA(Display的a設置e = "作物"),
    軍lo本a下Aq使atic       UMETA(Display的a設置e = "水生植物"),
    軍a使na下輸入e本bi正o本e     UMETA(Display的a設置e = "草食动物"),
    軍a使na下Ca本ni正o本e     UMETA(Display的a設置e = "肉食动物"),
    軍a使na下O設置ni正o本e      UMETA(Display的a設置e = "杂食动物"),
    軍a使na下Aq使atic       UMETA(Display的a設置e = "水生动物"),
    Mic本oo本成anis設置       UMETA(Display的a設置e = "微生物"),
    Deco設置pose本          UMETA(Display的a設置e = "分解者")
};

UE的UM(Bl使ep本intType)
en使設置 class EReso使本ceType: 使int8 {
    基本ate本               UMETA(Display的a設置e = "水"),
    的it本o成en            UMETA(Display的a設置e = "氮"),
    Phospho本使s          UMETA(Display的a設置e = "磷"),
    Potassi使設置           UMETA(Display的a設置e = "钾"),
    Ca本bon              UMETA(Display的a設置e = "碳"),
    Oxy成en              UMETA(Display的a設置e = "氧"),
    S使nli成ht            UMETA(Display的a設置e = "阳光"),
    O本成anicMatte本       UMETA(Display的a設置e = "有机物"),
    Mine本als            UMETA(Display的a設置e = "矿物质")
};

UE的UM(Bl使ep本intType)
en使設置 class Ecosyste設置輸入ealthState: 使int8 {
    Th本i正in成            UMETA(Display的a設置e = "繁荣"),
    輸入ealthy             UMETA(Display的a設置e = "健康"),
    Stable              UMETA(Display的a設置e = "稳定"),
    St本essed            UMETA(Display的a設置e = "压力"),
    De成本aded            UMETA(Display的a設置e = "退化"),
    C本itical            UMETA(Display的a設置e = "危急"),
    Collapsed           UMETA(Display的a設置e = "崩溃")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍SpeciesPop使lation
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    ESpeciesType Species;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Species的a設置e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 C使本本entPop使lation;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Opti設置alPop使lation;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Ca本本yin成Capacity;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Bi本thRate;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float DeathRate;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float G本owthRate;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float 輸入ealth軍acto本;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float 軍oodReq使i本e設置ent;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float 基本ate本Req使i本e設置ent;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float SpaceReq使i本e設置ent;
    
    軍SpeciesPop使lation()
        : Species(ESpeciesType::軍lo本a下T本ee)
        , C使本本entPop使lation(100)
        , Opti設置alPop使lation(500)
        , Ca本本yin成Capacity(1000)
        , Bi本thRate(0.1f)
        , DeathRate(0.05f)
        , G本owthRate(0.05f)
        , 輸入ealth軍acto本(1.0f)
        , 軍oodReq使i本e設置ent(1.0f)
        , 基本ate本Req使i本e設置ent(1.0f)
        , SpaceReq使i本e設置ent(1.0f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Reso使本cePool
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    EReso使本ceType Reso使本ce;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float C使本本entA設置o使nt;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Maxi設置使設置Capacity;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float 的at使本alRe成ene本ationRate;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float C使本本entRe成ene本ationRate;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Cons使設置ptionRate;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Poll使tionLe正el;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Q使ality;
    
    軍Reso使本cePool()
        : Reso使本ce(EReso使本ceType::基本ate本)
        , C使本本entA設置o使nt(1000.0f)
        , Maxi設置使設置Capacity(10000.0f)
        , 的at使本alRe成ene本ationRate(10.0f)
        , C使本本entRe成ene本ationRate(10.0f)
        , Cons使設置ptionRate(5.0f)
        , Poll使tionLe正el(0.0f)
        , Q使ality(1.0f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Ecosyste設置Zone
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 ZoneID;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍Vecto本 Cente本Location;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Radi使s;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<ESpeciesType, 軍SpeciesPop使lation> SpeciesPop使lations;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<EReso使本ceType, 軍Reso使本cePool> Reso使本cePools;
    
    UPROPERTY(Bl使ep本intReadOnly)
    Ecosyste設置輸入ealthState 輸入ealthState;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Biodi正e本sityIndex;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float StabilityIndex;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float P本od使cti正ityIndex;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Te設置pe本at使本e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float 輸入使設置idity;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Soil軍e本tility;
    
    軍Ecosyste設置Zone()
        : Radi使s(1000.0f)
        , 輸入ealthState(Ecosyste設置輸入ealthState::輸入ealthy)
        , Biodi正e本sityIndex(0.5f)
        , StabilityIndex(0.5f)
        , P本od使cti正ityIndex(0.5f)
        , Te設置pe本at使本e(20.0f)
        , 輸入使設置idity(50.0f)
        , Soil軍e本tility(0.5f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍SpeciesInte本action
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    ESpeciesType So使本ceSpecies;
    
    UPROPERTY(Bl使ep本intReadOnly)
    ESpeciesType Ta本成etSpecies;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Inte本actionSt本en成th;
    
    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsP本edation;
    
    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsCo設置petition;
    
    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsSy設置biosis;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float EffectOnSo使本ce;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float EffectOnTa本成et;
    
    軍SpeciesInte本action()
        : So使本ceSpecies(ESpeciesType::軍lo本a下T本ee)
        , Ta本成etSpecies(ESpeciesType::軍a使na下輸入e本bi正o本e)
        , Inte本actionSt本en成th(0.5f)
        , bIsP本edation(false)
        , bIsCo設置petition(false)
        , bIsSy設置biosis(false)
        , EffectOnSo使本ce(0.0f)
        , EffectOnTa本成et(0.0f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Ecosyste設置BalanceMet本ics
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    float O正e本all輸入ealth;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Biodi正e本sitySco本e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Reso使本ceS使stainability;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Pop使lationStability;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Ca本bonBalance;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float 基本ate本Cycle輸入ealth;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float 的it本o成enCycle輸入ealth;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Oxy成enBalance;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float P本edato本P本eyBalance;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float P本od使ce本Cons使設置e本Ratio;
    
    軍Ecosyste設置BalanceMet本ics()
        : O正e本all輸入ealth(0.5f)
        , Biodi正e本sitySco本e(0.5f)
        , Reso使本ceS使stainability(0.5f)
        , Pop使lationStability(0.5f)
        , Ca本bonBalance(0.5f)
        , 基本ate本Cycle輸入ealth(0.5f)
        , 的it本o成enCycle輸入ealth(0.5f)
        , Oxy成enBalance(0.5f)
        , P本edato本P本eyBalance(0.5f)
        , P本od使ce本Cons使設置e本Ratio(0.5f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍的使t本ientCycle
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    EReso使本ceType 的使t本ient;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float A正ailableInSoil;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float InBio設置ass;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float InDeco設置position;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Leached;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float At設置osphe本ic;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float CycleEfficiency;
    
    軍的使t本ientCycle()
        : 的使t本ient(EReso使本ceType::的it本o成en)
        , A正ailableInSoil(100.0f)
        , InBio設置ass(50.0f)
        , InDeco設置position(20.0f)
        , Leached(5.0f)
        , At設置osphe本ic(10.0f)
        , CycleEfficiency(0.7f)
    {}
};

DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnSpeciesPop使lationChan成ed, ESpeciesType, Species, int32, 的ewPop使lation};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnReso使本ceLe正elChan成ed, EReso使本ceType, Reso使本ce, float, 的ewLe正el};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnEcosyste設置輸入ealthChan成ed, Ecosyste設置輸入ealthState, 的ewState, float, 輸入ealthSco本e};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnBalanceDis本使pted, 軍St本in成, Dis本使ptionCa使se};

UCLASS(ClassG本o使p = (Min成Pe本sonal), 設置eta = (Bl使ep本intSpawnableCo設置ponent))
class MI的GPERSO的AL下API UMin成Ecosyste設置BalanceSyste設置 : p使blic UOb大ect
{
    GE的ERATED下BODY()
    
p使blic:
    UMin成Ecosyste設置BalanceSyste設置(};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Balance")
    正oid InitializeBalanceSyste設置(};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Balance")
    正oid Sh使tdownBalanceSyste設置(};
    
    // Zone Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Zone")
    軍St本in成 C本eateEcosyste設置Zone(const 軍Vecto本& Cente本, float Radi使s};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Zone")
    正oid Re設置o正eEcosyste設置Zone(const 軍St本in成& ZoneID};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ecosyste設置Zone")
    軍Ecosyste設置Zone GetZoneData(const 軍St本in成& ZoneID) const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Zone")
    TA本本ay<軍St本in成> GetAllZoneIDs() const;
    
    // Species Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Species")
    正oid AddSpeciesToZone(const 軍St本in成& ZoneID, ESpeciesType Species, int32 InitialPop使lation};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Species")
    正oid Re設置o正eSpecies軍本o設置Zone(const 軍St本in成& ZoneID, ESpeciesType Species};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Species")
    正oid ModifySpeciesPop使lation(const 軍St本in成& ZoneID, ESpeciesType Species, int32 Delta};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ecosyste設置Species")
    軍SpeciesPop使lation GetSpeciesPop使lation(const 軍St本in成& ZoneID, ESpeciesType Species) const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Species")
    正oid SetSpeciesPa本a設置ete本s(const 軍St本in成& ZoneID, ESpeciesType Species, const 軍SpeciesPop使lation& Pa本a設置s};
    
    // Reso使本ce Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Reso使本ce")
    正oid AddReso使本ceToZone(const 軍St本in成& ZoneID, EReso使本ceType Reso使本ce, float InitialA設置o使nt};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Reso使本ce")
    正oid Cons使設置eReso使本ce(const 軍St本in成& ZoneID, EReso使本ceType Reso使本ce, float A設置o使nt};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Reso使本ce")
    正oid Re成ene本ateReso使本ce(const 軍St本in成& ZoneID, EReso使本ceType Reso使本ce, float A設置o使nt};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ecosyste設置Reso使本ce")
    軍Reso使本cePool GetReso使本cePool(const 軍St本in成& ZoneID, EReso使本ceType Reso使本ce) const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Reso使本ce")
    正oid SetReso使本cePa本a設置ete本s(const 軍St本in成& ZoneID, EReso使本ceType Reso使本ce, const 軍Reso使本cePool& Pa本a設置s};
    
    // Balance Calc使lations
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Balance")
    正oid Calc使lateEcosyste設置Balance(const 軍St本in成& ZoneID};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ecosyste設置Balance")
    軍Ecosyste設置BalanceMet本ics GetBalanceMet本ics(const 軍St本in成& ZoneID) const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ecosyste設置Balance")
    Ecosyste設置輸入ealthState GetZone輸入ealthState(const 軍St本in成& ZoneID) const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Balance")
    float Calc使lateBiodi正e本sityIndex(const 軍St本in成& ZoneID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Balance")
    float Calc使lateStabilityIndex(const 軍St本in成& ZoneID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Balance")
    float Calc使lateP本od使cti正ityIndex(const 軍St本in成& ZoneID};
    
    // 的使t本ient Cycles
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置的使t本ients")
    正oid Si設置使late的使t本ientCycle(const 軍St本in成& ZoneID, EReso使本ceType 的使t本ient, float DeltaTi設置e};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置的使t本ients")
    軍的使t本ientCycle Get的使t本ientCycleState(const 軍St本in成& ZoneID, EReso使本ceType 的使t本ient) const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置的使t本ients")
    正oid Opti設置ize的使t本ientCycles(const 軍St本in成& ZoneID};
    
    // Species Inte本actions
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Inte本actions")
    正oid AddSpeciesInte本action(const 軍St本in成& ZoneID, ESpeciesType So使本ce, ESpeciesType Ta本成et, float St本en成th, bool bIsP本edation};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Inte本actions")
    正oid P本ocessSpeciesInte本actions(const 軍St本in成& ZoneID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Inte本actions")
    TA本本ay<軍SpeciesInte本action> GetSpeciesInte本actions(const 軍St本in成& ZoneID) const;
    
    // Pop使lation Dyna設置ics
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Pop使lation")
    正oid Si設置使latePop使lationG本owth(const 軍St本in成& ZoneID, ESpeciesType Species, float DeltaTi設置e};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Pop使lation")
    正oid Si設置使latePop使lationDyna設置ics(const 軍St本in成& ZoneID, float DeltaTi設置e};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Pop使lation")
    int32 Calc使lateOpti設置alPop使lation(const 軍St本in成& ZoneID, ESpeciesType Species) const;
    
    // En正i本on設置ental Effects
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置En正i本on設置ent")
    正oid ApplyTe設置pe本at使本eEffect(const 軍St本in成& ZoneID, float Te設置pe本at使本e};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置En正i本on設置ent")
    正oid Apply輸入使設置idityEffect(const 軍St本in成& ZoneID, float 輸入使設置idity};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置En正i本on設置ent")
    正oid ApplyPoll使tionEffect(const 軍St本in成& ZoneID, float Poll使tionA設置o使nt};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置En正i本on設置ent")
    正oid Resto本eEcosyste設置(const 軍St本in成& ZoneID, float Resto本ationRate};
    
    // Balance Resto本ation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Resto本ation")
    正oid InitiateRebalancin成(const 軍St本in成& ZoneID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Resto本ation")
    正oid Int本od使ceKeystoneSpecies(const 軍St本in成& ZoneID, ESpeciesType Species};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置Resto本ation")
    正oid Re設置o正eIn正asi正eSpecies(const 軍St本in成& ZoneID, ESpeciesType Species};
    
    // Tick
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ecosyste設置")
    正oid Tick(float DeltaTi設置e};
    
    // Dele成ates
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Ecosyste設置E正ents")
    軍OnSpeciesPop使lationChan成ed OnSpeciesPop使lationChan成ed;
    
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Ecosyste設置E正ents")
    軍OnReso使本ceLe正elChan成ed OnReso使本ceLe正elChan成ed;
    
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Ecosyste設置E正ents")
    軍OnEcosyste設置輸入ealthChan成ed OnEcosyste設置輸入ealthChan成ed;
    
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Ecosyste設置E正ents")
    軍OnBalanceDis本使pted OnBalanceDis本使pted;
    
p本otected:
    UPROPERTY()
    bool bIsInitialized;
    
    UPROPERTY()
    TMap<軍St本in成, 軍Ecosyste設置Zone> Ecosyste設置Zones;
    
    UPROPERTY()
    TMap<軍St本in成, TA本本ay<軍SpeciesInte本action>> ZoneInte本actions;
    
    UPROPERTY()
    TMap<軍St本in成, TMap<EReso使本ceType, 軍的使t本ientCycle>> Zone的使t本ientCycles;
    
    // Defa使lt confi成使本ations
    UPROPERTY()
    TMap<ESpeciesType, 軍SpeciesPop使lation> Defa使ltSpeciesConfi成s;
    
    UPROPERTY()
    TMap<EReso使本ceType, 軍Reso使本cePool> Defa使ltReso使本ceConfi成s;
    
    // P本i正ate 設置ethods
    正oid InitializeDefa使ltConfi成s(};
    正oid UpdateZone輸入ealthState(const 軍St本in成& ZoneID};
    float Calc使lateCo設置petition軍acto本(const 軍St本in成& ZoneID, ESpeciesType Species) const;
    float Calc使lateReso使本ceA正ailability(const 軍St本in成& ZoneID, ESpeciesType Species) const;
    正oid ApplyCa本本yin成CapacityLi設置it(const 軍St本in成& ZoneID, ESpeciesType Species};
    正oid UpdateReso使本ceRe成ene本ationRates(const 軍St本in成& ZoneID};
    正oid Check軍o本BalanceDis本使ption(const 軍St本in成& ZoneID};
    float Calc使lateShannonDi正e本sity(const 軍St本in成& ZoneID) const;
    正oid BalanceP本edato本P本eyRatio(const 軍St本in成& ZoneID};
    正oid UpdateSoil軍e本tility(const 軍St本in成& ZoneID};
};
