#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成RTSEcosyste設置Si設置使lation.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EEcosyste設置Zone: 使int8 {
    的one UMETA(Display的a設置e = "無"),
    軍o本est UMETA(Display的a設置e = "森林"),
    G本assland UMETA(Display的a設置e = "草原"),
    Dese本t UMETA(Display的a設置e = "沙漠"),
    基本etland UMETA(Display的a設置e = "濕地"),
    Mo使ntain UMETA(Display的a設置e = "山地"),
    Ri正e本 UMETA(Display的a設置e = "河流"),
    Lake UMETA(Display的a設置e = "湖泊"),
    Ocean UMETA(Display的a設置e = "海洋"),
    T使nd本a UMETA(Display的a設置e = "凍原"),
    Rainfo本est UMETA(Display的a設置e = "雨林"),
    Sa正annah UMETA(Display的a設置e = "稀樹草原"),
    Co使nt UMETA(Display的a設置e = "生態區數量")
};

UE的UM(Bl使ep本intType)
en使設置 class ESeasonType: 使int8 {
    Sp本in成 UMETA(Display的a設置e = "春季"),
    S使設置設置e本 UMETA(Display的a設置e = "夏季"),
    A使t使設置n UMETA(Display的a設置e = "秋季"),
    基本inte本 UMETA(Display的a設置e = "冬季"),
    Co使nt UMETA(Display的a設置e = "季節數量")
};

UE的UM(Bl使ep本intType)
en使設置 class E基本eathe本Type: 使int8 {
    Clea本 UMETA(Display的a設置e = "晴朗"),
    Clo使dy UMETA(Display的a設置e = "多雲"),
    Rain UMETA(Display的a設置e = "下雨"),
    Snow UMETA(Display的a設置e = "下雪"),
    Sto本設置 UMETA(Display的a設置e = "暴風雨"),
    軍o成 UMETA(Display的a設置e = "霧"),
    D本o使成ht UMETA(Display的a設置e = "乾旱"),
    軍lood UMETA(Display的a設置e = "洪水"),
    Co使nt UMETA(Display的a設置e = "天氣數量")
};

UE的UM(Bl使ep本intType)
en使設置 class EReso使本ceType: 使int8 {
    基本ate本 UMETA(Display的a設置e = "水資源"),
    軍ood UMETA(Display的a設置e = "食物"),
    基本ood UMETA(Display的a設置e = "木材"),
    Stone UMETA(Display的a設置e = "石材"),
    Metal UMETA(Display的a設置e = "金屬"),
    軍e本tility UMETA(Display的a設置e = "肥沃度"),
    Biodi正e本sity UMETA(Display的a設置e = "生物多樣性"),
    Co使nt UMETA(Display的a設置e = "資源數量")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Ecosyste設置Reso使本ce
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    EReso使本ceType Reso使本ceType;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float C使本本entA設置o使nt;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float MaxCapacity;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Re成ene本ationRate;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Cons使設置ptionRate;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Q使ality;

    軍Ecosyste設置Reso使本ce()
        : Reso使本ceType(EReso使本ceType::基本ate本)
        , C使本本entA設置o使nt(100.0f)
        , MaxCapacity(100.0f)
        , Re成ene本ationRate(1.0f)
        , Cons使設置ptionRate(0.5f)
        , Q使ality(1.0f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Cli設置ateCondition
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    ESeasonType C使本本entSeason;

    UPROPERTY(Bl使ep本intRead基本本ite)
    E基本eathe本Type C使本本ent基本eathe本;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Te設置pe本at使本e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float 輸入使設置idity;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float P本ecipitation;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float 基本indSpeed;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Dayli成ht輸入o使本s;

    軍Cli設置ateCondition()
        : C使本本entSeason(ESeasonType::Sp本in成)
        , C使本本ent基本eathe本(E基本eathe本Type::Clea本)
        , Te設置pe本at使本e(20.0f)
        , 輸入使設置idity(50.0f)
        , P本ecipitation(0.0f)
        , 基本indSpeed(5.0f)
        , Dayli成ht輸入o使本s(12.0f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Ecosyste設置ZoneData
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    EEcosyste設置Zone ZoneType;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍Vecto本 ZoneCente本;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float ZoneRadi使s;

    UPROPERTY(Bl使ep本intRead基本本ite)
    TMap<EReso使本ceType, 軍Ecosyste設置Reso使本ce> Reso使本ces;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍Cli設置ateCondition Cli設置ate;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Ca本本yin成Capacity;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float C使本本entPop使lation;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float 輸入ealthIndex;

    UPROPERTY(Bl使ep本intRead基本本ite)
    bool bIsPoll使ted;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Poll使tionLe正el;

    軍Ecosyste設置ZoneData()
        : ZoneType(EEcosyste設置Zone::G本assland)
        , ZoneRadi使s(1000.0f)
        , Ca本本yin成Capacity(1000.0f)
        , C使本本entPop使lation(500.0f)
        , 輸入ealthIndex(1.0f)
        , bIsPoll使ted(false)
        , Poll使tionLe正el(0.0f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍SpeciesPop使lation
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 Species的a設置e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    int32 Pop使lationCo使nt;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float G本owthRate;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float DeathRate;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Mi成本ationRate;

    UPROPERTY(Bl使ep本intRead基本本ite)
    TA本本ay<EReso使本ceType> Req使i本edReso使本ces;

    UPROPERTY(Bl使ep本intRead基本本ite)
    EEcosyste設置Zone P本efe本本edZone;

    軍SpeciesPop使lation()
        : Pop使lationCo使nt(100)
        , G本owthRate(0.05f)
        , DeathRate(0.02f)
        , Mi成本ationRate(0.01f)
        , P本efe本本edZone(EEcosyste設置Zone::軍o本est)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Ecosyste設置E正ent
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 E正ent的a設置e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 E正entDesc本iption;

    UPROPERTY(Bl使ep本intRead基本本ite)
    EEcosyste設置Zone AffectedZone;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float I設置pactSe正e本ity;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float D使本ation;

    UPROPERTY(Bl使ep本intRead基本本ite)
    TMap<EReso使本ceType, float> Reso使本ceI設置pacts;

    軍Ecosyste設置E正ent()
        : I設置pactSe正e本ity(0.5f)
        , D使本ation(1.0f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Ecosyste設置Statistics
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    int32 TotalZones;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 輸入ealthyZones;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 Poll使tedZones;

    UPROPERTY(Bl使ep本intReadOnly)
    float A正e本a成e輸入ealthIndex;

    UPROPERTY(Bl使ep本intReadOnly)
    float TotalBiodi正e本sity;

    UPROPERTY(Bl使ep本intReadOnly)
    float A正e本a成eTe設置pe本at使本e;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 Acti正eSpecies;

    UPROPERTY(Bl使ep本intReadOnly)
    float Ecosyste設置Stability;

    軍Ecosyste設置Statistics()
        : TotalZones(0)
        , 輸入ealthyZones(0)
        , Poll使tedZones(0)
        , A正e本a成e輸入ealthIndex(0.0f)
        , TotalBiodi正e本sity(0)
        , A正e本a成eTe設置pe本at使本e(0.0f)
        , Acti正eSpecies(0)
        , Ecosyste設置Stability(0.0f)
    {}
};

/**
 * 生態環境模擬系統
 */
UCLASS(ClassG本o使p = (Min成St本ate成ic), Bl使ep本intType, Bl使ep本intable)
class MI的GSTRATEGIC下API UMin成RTSEcosyste設置Si設置使lation : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成RTSEcosyste設置Si設置使lation(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Syste設置")
    正oid InitializeEcosyste設置(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Syste設置")
    正oid Sh使tdownEcosyste設置(};

    // 生態區管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Zones")
    int32 C本eateEcosyste設置Zone(EEcosyste設置Zone ZoneType, 軍Vecto本 Cente本, float Radi使s};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Zones")
    正oid Re設置o正eEcosyste設置Zone(int32 ZoneId};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Zones")
    正oid UpdateZoneReso使本ces(int32 ZoneId, EReso使本ceType Reso使本ceType, float A設置o使nt};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSEcosyste設置Zones")
    軍Ecosyste設置ZoneData GetZoneData(int32 ZoneId) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSEcosyste設置Zones")
    TA本本ay<int32> GetAllZones() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSEcosyste設置Zones")
    TA本本ay<int32> GetZonesByType(EEcosyste設置Zone ZoneType) const;

    // 氣候系統
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Cli設置ate")
    正oid SetSeason(ESeasonType 的ewSeason};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Cli設置ate")
    正oid Set基本eathe本(int32 ZoneId, E基本eathe本Type 基本eathe本};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Cli設置ate")
    正oid UpdateCli設置ate(int32 ZoneId, float Te設置pe本at使本eDelta, float 輸入使設置idityDelta};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSEcosyste設置Cli設置ate")
    軍Cli設置ateCondition GetZoneCli設置ate(int32 ZoneId) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Cli設置ate")
    正oid Si設置使late基本eathe本Chan成e(};

    // 資源管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Reso使本ces")
    正oid AddReso使本ce(int32 ZoneId, EReso使本ceType Reso使本ceType, float A設置o使nt};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Reso使本ces")
    正oid Cons使設置eReso使本ce(int32 ZoneId, EReso使本ceType Reso使本ceType, float A設置o使nt};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Reso使本ces")
    正oid Re成ene本ateReso使本ces(float DeltaTi設置e};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSEcosyste設置Reso使本ces")
    float GetReso使本ceA正ailability(int32 ZoneId, EReso使本ceType Reso使本ceType) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSEcosyste設置Reso使本ces")
    float Calc使lateReso使本ceBalance(int32 ZoneId) const;

    // 物種管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Species")
    正oid AddSpecies(int32 ZoneId, const 軍SpeciesPop使lation& Species};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Species")
    正oid UpdateSpeciesPop使lation(int32 ZoneId, const 軍St本in成& Species的a設置e, int32 的ewCo使nt};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Species")
    正oid Si設置使latePop使lationG本owth(float DeltaTi設置e};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSEcosyste設置Species")
    TA本本ay<軍SpeciesPop使lation> GetZoneSpecies(int32 ZoneId) const;

    // 環境影響
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置I設置pact")
    正oid ApplyPoll使tion(int32 ZoneId, float Poll使tionA設置o使nt};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置I設置pact")
    正oid CleanUpPoll使tion(int32 ZoneId, float Clean使pA設置o使nt};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置I設置pact")
    正oid T本i成成e本Ecosyste設置E正ent(const 軍Ecosyste設置E正ent& E正ent};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置I設置pact")
    正oid Apply輸入使設置anActi正ityI設置pact(int32 ZoneId, float Acti正ityIntensity, float D使本ation};

    // 生態平衡計算
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSEcosyste設置Balance")
    float Calc使lateZone輸入ealth(int32 ZoneId) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSEcosyste設置Balance")
    float Calc使lateBiodi正e本sityIndex(int32 ZoneId) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSEcosyste設置Balance")
    float Calc使lateCa本本yin成Capacity(int32 ZoneId) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSEcosyste設置Balance")
    float P本edictEcosyste設置CollapseRisk(int32 ZoneId) const;

    // 模擬更新
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Si設置使lation")
    正oid Si設置使lateEcosyste設置Tick(float DeltaTi設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Si設置使lation")
    正oid Ad正anceSeason(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Si設置使lation")
    正oid P本ocessReso使本ce軍low(};

    // 統計與報告
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSEcosyste設置Analytics")
    軍Ecosyste設置Statistics GetEcosyste設置Statistics() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSEcosyste設置Analytics")
    正oid Gene本ateEcosyste設置Repo本t(const 軍St本in成& Repo本tPath};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSEcosyste設置Analytics")
    TA本本ay<int32> 軍indDe成本adedZones() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSEcosyste設置Analytics")
    TA本本ay<int32> 軍indReso使本ceRichZones() const;

    // 事件
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Min成RTSEcosyste設置E正ents")
    軍OnZoneDe成本aded OnZoneDe成本aded;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Min成RTSEcosyste設置E正ents")
    軍OnReso使本ceDepleted OnReso使本ceDepleted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Min成RTSEcosyste設置E正ents")
    軍OnSeasonChan成ed OnSeasonChan成ed;

p本i正ate:
    UPROPERTY()
    TMap<int32, 軍Ecosyste設置ZoneData> Ecosyste設置Zones;

    UPROPERTY()
    TMap<int32, TA本本ay<軍SpeciesPop使lation>> ZoneSpecies;

    UPROPERTY()
    ESeasonType C使本本entSeason;

    UPROPERTY()
    int32 的extZoneId;

    UPROPERTY()
    float Si設置使lationTi設置e;

    // 內部計算函數
    float Calc使lateReso使本ceRe成ene本ation(const 軍Ecosyste設置Reso使本ce& Reso使本ce, EEcosyste設置Zone ZoneType, const 軍Cli設置ateCondition& Cli設置ate) const;
    float Calc使latePoll使tionI設置pact(float Poll使tionLe正el, EEcosyste設置Zone ZoneType) const;
    正oid Sp本eadPoll使tion(int32 So使本ceZoneId, float A設置o使nt};
    正oid Mi成本ateSpeciesBetweenZones(float DeltaTi設置e};
    正oid UpdateCli設置ateEffects(float DeltaTi設置e};
    float GetSeasonalTe設置pe本at使本eM使ltiplie本(ESeasonType Season) const;
    float Get基本eathe本Reso使本ceM使ltiplie本(E基本eathe本Type 基本eathe本, EReso使本ceType Reso使本ce) const;
};

// 事件委託聲明
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnZoneDe成本aded, int32, ZoneId, float, 輸入ealthIndex};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnReso使本ceDepleted, int32, ZoneId, EReso使本ceType, Reso使本ceType};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnSeasonChan成ed, ESeasonType, 的ewSeason};
};
