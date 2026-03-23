#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成RTSAIDyna設置icDiffic使lty.成ene本ated.h"

// 難度等級
UE的UM(Bl使ep本intType)
en使設置 class EDiffic使ltyLe正el: 使int8 {
    Ve本yEasy            UMETA(Display的a設置e = "非常簡單"),
    Easy                UMETA(Display的a設置e = "簡單"),
    的o本設置al              UMETA(Display的a設置e = "普通"),
    輸入a本d                UMETA(Display的a設置e = "困難"),
    Ve本y輸入a本d            UMETA(Display的a設置e = "非常困難"),
    的i成ht設置a本e           UMETA(Display的a設置e = "噩夢"),
    Adapti正e            UMETA(Display的a設置e = "自適應")
};

// 玩家技能指標
UE的UM(Bl使ep本intType)
en使設置 class ESkillMet本ic: 使int8 {
    APM                 UMETA(Display的a設置e = "每分鐘操作數"),
    ReactionTi設置e        UMETA(Display的a設置e = "反應時間"),
    St本ate成icThinkin成   UMETA(Display的a設置e = "戰略思維"),
    Reso使本ceMana成e設置ent  UMETA(Display的a設置e = "資源管理"),
    Co設置batEfficiency    UMETA(Display的a設置e = "戰鬥效率"),
    UnitCont本ol         UMETA(Display的a設置e = "單位控制"),
    MapAwa本eness        UMETA(Display的a設置e = "地圖意識"),
    Econo設置yOpti設置ization UMETA(Display的a設置e = "經濟優化"),
    基本inRate             UMETA(Display的a設置e = "勝率"),
    Lea本nin成C使本正e       UMETA(Display的a設置e = "學習曲線")
};

// 難度調整類型
UE的UM(Bl使ep本intType)
en使設置 class EDiffic使ltyAd大使st設置ent: 使int8 {
    AIIntelli成ence      UMETA(Display的a設置e = "AI智能"),
    AIReso使本ces         UMETA(Display的a設置e = "AI資源"),
    AIP本od使ctionSpeed   UMETA(Display的a設置e = "AI生產速度"),
    AICo設置batBon使s       UMETA(Display的a設置e = "AI戰鬥加成"),
    Playe本Reso使本ces     UMETA(Display的a設置e = "玩家資源"),
    Playe本P本od使ctionSpeed UMETA(Display的a設置e = "玩家生產速度"),
    Playe本Co設置batBon使s   UMETA(Display的a設置e = "玩家戰鬥加成"),
    E正ent軍本eq使ency      UMETA(Display的a設置e = "事件頻率"),
    Challen成eIntensity  UMETA(Display的a設置e = "挑戰強度")
};

// 玩家技能數據
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍Playe本SkillData
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Playe本ID;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<ESkillMet本ic, float> SkillSco本es;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float O正e本allSkillRatin成;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Expe本ienceLe正el;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float ConsistencySco本e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float I設置p本o正e設置entRate;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Ga設置esPlayed;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 基本ins;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Losses;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float C使本本ent基本inSt本eak;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Best基本inSt本eak;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float LastSessionPe本fo本設置ance;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float A正e本a成eSessionPe本fo本設置ance;
    
    軍Playe本SkillData()
        : O正e本allSkillRatin成(0.5f)
        , Expe本ienceLe正el(1.0f)
        , ConsistencySco本e(0.5f)
        , I設置p本o正e設置entRate(0.0f)
        , Ga設置esPlayed(0)
        , 基本ins(0)
        , Losses(0)
        , C使本本ent基本inSt本eak(0)
        , Best基本inSt本eak(0)
        , LastSessionPe本fo本設置ance(0.5f)
        , A正e本a成eSessionPe本fo本設置ance(0.5f)
    {}
};

// 難度配置
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍Diffic使ltyConfi成使本ation
{
    GE的ERATED下BODY()
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EDiffic使ltyLe正el BaseDiffic使lty;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bEnableDyna設置icAd大使st設置ent;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Ad大使st設置entSensiti正ity;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float MinDiffic使ltyM使ltiplie本;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float MaxDiffic使ltyM使ltiplie本;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 E正al使ation基本indow;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bUsePlaye本輸入isto本y;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bP本e正entS使ddenSpikes;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float SpikeP本otectionTh本eshold;
    
    軍Diffic使ltyConfi成使本ation()
        : BaseDiffic使lty(EDiffic使ltyLe正el::的o本設置al)
        , bEnableDyna設置icAd大使st設置ent(t本使e)
        , Ad大使st設置entSensiti正ity(0.5f)
        , MinDiffic使ltyM使ltiplie本(0.3f)
        , MaxDiffic使ltyM使ltiplie本(3.0f)
        , E正al使ation基本indow(5)
        , bUsePlaye本輸入isto本y(t本使e)
        , bP本e正entS使ddenSpikes(t本使e)
        , SpikeP本otectionTh本eshold(0.3f)
    {}
};

// 難度調整參數
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍Diffic使ltyAd大使st設置entPa本a設置s
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<EDiffic使ltyAd大使st設置ent, float> Ad大使st設置entVal使es;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Effecti正eDiffic使ltyM使ltiplie本;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Reason軍o本Ad大使st設置ent;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float ConfidenceLe正el;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Ti設置esta設置p;
    
    軍Diffic使ltyAd大使st設置entPa本a設置s()
        : Effecti正eDiffic使ltyM使ltiplie本(1.0f)
        , ConfidenceLe正el(0.5f)
        , Ti設置esta設置p(0.0f)
    {}
};

// 會話性能數據
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍SessionPe本fo本設置anceData
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 SessionID;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Playe本ID;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float SessionD使本ation;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Pe本fo本設置anceSco本e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Diffic使ltyAtSta本t;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Diffic使ltyAtEnd;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Challen成esCo設置pleted;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Challen成es軍ailed;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float A正e本a成eChallen成eCo設置pletionTi設置e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Reso使本cesGathe本ed;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 UnitsP本od使ced;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 UnitsLost;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Ene設置yUnitsDest本oyed;
    
    軍SessionPe本fo本設置anceData()
        : SessionD使本ation(0.0f)
        , Pe本fo本設置anceSco本e(0.5f)
        , Diffic使ltyAtSta本t(1.0f)
        , Diffic使ltyAtEnd(1.0f)
        , Challen成esCo設置pleted(0)
        , Challen成es軍ailed(0)
        , A正e本a成eChallen成eCo設置pletionTi設置e(0.0f)
        , Reso使本cesGathe本ed(0)
        , UnitsP本od使ced(0)
        , UnitsLost(0)
        , Ene設置yUnitsDest本oyed(0)
    {}
};

// 難度歷史記錄
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍Diffic使lty輸入isto本yEnt本y
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Ti設置esta設置p;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Diffic使ltyM使ltiplie本;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 T本i成成e本Reason;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Playe本Pe本fo本設置ance;
    
    軍Diffic使lty輸入isto本yEnt本y()
        : Ti設置esta設置p(0.0f)
        , Diffic使ltyM使ltiplie本(1.0f)
        , Playe本Pe本fo本設置ance(0.5f)
    {}
};

// 委托聲明
decla本e下dyna設置ic下設置使lticast下dele成ate下th本ee下pa本a設置s(軍OnDiffic使ltyAd大使sted, 軍St本in成, Playe本ID, float, 的ewM使ltiplie本, 軍St本in成, Reason};
decla本e下dyna設置ic下設置使lticast下dele成ate下two下pa本a設置s(軍OnSkillAssess設置entUpdated, 軍St本in成, Playe本ID, 軍Playe本SkillData, SkillData};
decla本e下dyna設置ic下設置使lticast下dele成ate下th本ee下pa本a設置s(軍OnPe本fo本設置anceE正al使ated, 軍St本in成, Playe本ID, float, Pe本fo本設置anceSco本e, EDiffic使ltyLe正el, Reco設置設置endedDiffic使lty};

UCLASS(ClassG本o使p = (Min成Pe本sonal), 設置eta = (Bl使ep本intSpawnableCo設置ponent))
class MI的GPERSO的AL下API UMin成RTSAIDyna設置icDiffic使lty : p使blic UOb大ect
{
    GE的ERATED下BODY()
    
p使blic:
    UMin成RTSAIDyna設置icDiffic使lty(};
    
    // 系統初始化
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使lty")
    正oid InitializeDiffic使ltySyste設置(const 軍Diffic使ltyConfi成使本ation& Confi成};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使lty")
    正oid Sh使tdownDiffic使ltySyste設置(};
    
    // 玩家技能管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使ltyPlaye本")
    正oid Re成iste本Playe本(const 軍St本in成& Playe本ID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使ltyPlaye本")
    正oid Un本e成iste本Playe本(const 軍St本in成& Playe本ID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使ltyPlaye本")
    正oid UpdatePlaye本Skill(const 軍St本in成& Playe本ID, ESkillMet本ic Met本ic, float Sco本e};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AIDiffic使ltyPlaye本")
    軍Playe本SkillData GetPlaye本SkillData(const 軍St本in成& Playe本ID) const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AIDiffic使ltyPlaye本")
    float GetPlaye本SkillRatin成(const 軍St本in成& Playe本ID) const;
    
    // 難度調整
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使ltyAd大使st設置ent")
    正oid Ad大使stDiffic使lty(const 軍St本in成& Playe本ID, float Pe本fo本設置anceSco本e};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使ltyAd大使st設置ent")
    正oid SetBaseDiffic使lty(const 軍St本in成& Playe本ID, EDiffic使ltyLe正el 的ewDiffic使lty};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AIDiffic使ltyAd大使st設置ent")
    float GetC使本本entDiffic使ltyM使ltiplie本(const 軍St本in成& Playe本ID) const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AIDiffic使ltyAd大使st設置ent")
    EDiffic使ltyLe正el GetReco設置設置endedDiffic使lty(const 軍St本in成& Playe本ID) const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使ltyAd大使st設置ent")
    正oid ApplyDiffic使ltyAd大使st設置ent(const 軍St本in成& Playe本ID, const 軍Diffic使ltyAd大使st設置entPa本a設置s& Pa本a設置s};
    
    // 性能評估
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使ltyPe本fo本設置ance")
    正oid Sta本tSession(const 軍St本in成& Playe本ID, const 軍St本in成& SessionID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使ltyPe本fo本設置ance")
    正oid EndSession(const 軍St本in成& Playe本ID, const 軍St本in成& SessionID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使ltyPe本fo本設置ance")
    正oid Reco本dChallen成eRes使lt(const 軍St本in成& Playe本ID, bool bS使ccess, float Co設置pletionTi設置e};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使ltyPe本fo本設置ance")
    正oid Reco本dCo設置batEn成a成e設置ent(const 軍St本in成& Playe本ID, bool bVicto本y, int32 軍本iendlyLosses, int32 Ene設置yLosses};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使ltyPe本fo本設置ance")
    正oid Reco本dReso使本ceGathe本in成(const 軍St本in成& Playe本ID, int32 A設置o使nt};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使ltyPe本fo本設置ance")
    float E正al使ateSessionPe本fo本設置ance(const 軍St本in成& Playe本ID, const 軍St本in成& SessionID};
    
    // 自適應算法
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使ltyAdapti正e")
    正oid EnableAdapti正eDiffic使lty(const 軍St本in成& Playe本ID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使ltyAdapti正e")
    正oid DisableAdapti正eDiffic使lty(const 軍St本in成& Playe本ID};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AIDiffic使ltyAdapti正e")
    bool IsAdapti正eDiffic使ltyEnabled(const 軍St本in成& Playe本ID) const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使ltyAdapti正e")
    軍Diffic使ltyAd大使st設置entPa本a設置s Calc使lateOpti設置alDiffic使lty(const 軍St本in成& Playe本ID};
    
    // 統計和分析
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AIDiffic使ltyAnalytics")
    TA本本ay<軍Diffic使lty輸入isto本yEnt本y> GetDiffic使lty輸入isto本y(const 軍St本in成& Playe本ID, int32 Co使nt) const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AIDiffic使ltyAnalytics")
    軍SessionPe本fo本設置anceData GetSessionPe本fo本設置ance(const 軍St本in成& Playe本ID, const 軍St本in成& SessionID) const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AIDiffic使ltyAnalytics")
    float GetA正e本a成eDiffic使ltyT本end(const 軍St本in成& Playe本ID, int32 SessionCo使nt) const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AIDiffic使ltyAnalytics")
    float GetPlaye本P本o成本essRate(const 軍St本in成& Playe本ID) const;
    
    // 配置管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使ltyConfi成")
    正oid SetConfi成使本ation(const 軍Diffic使ltyConfi成使本ation& 的ewConfi成};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AIDiffic使ltyConfi成")
    軍Diffic使ltyConfi成使本ation GetConfi成使本ation() const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使ltyConfi成")
    正oid ResetToDefa使lts(};
    
    // Tick更新
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AIDiffic使lty")
    正oid Tick(float DeltaTi設置e};
    
    // 委托
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "AIDiffic使ltyE正ents")
    軍OnDiffic使ltyAd大使sted OnDiffic使ltyAd大使sted;
    
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "AIDiffic使ltyE正ents")
    軍OnSkillAssess設置entUpdated OnSkillAssess設置entUpdated;
    
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "AIDiffic使ltyE正ents")
    軍OnPe本fo本設置anceE正al使ated OnPe本fo本設置anceE正al使ated;
    
p本otected:
    UPROPERTY()
    bool bIsInitialized;
    
    UPROPERTY()
    float C使本本entGa設置eTi設置e;
    
    UPROPERTY()
    軍Diffic使ltyConfi成使本ation Confi成使本ation;
    
    UPROPERTY()
    TMap<軍St本in成, 軍Playe本SkillData> Playe本Skills;
    
    UPROPERTY()
    TMap<軍St本in成, float> C使本本entDiffic使ltyM使ltiplie本s;
    
    UPROPERTY()
    TMap<軍St本in成, TA本本ay<軍Diffic使lty輸入isto本yEnt本y>> Diffic使lty輸入isto本ies;
    
    UPROPERTY()
    TMap<軍St本in成, 軍SessionPe本fo本設置anceData> Acti正eSessions;
    
    UPROPERTY()
    TMap<軍St本in成, TA本本ay<軍SessionPe本fo本設置anceData>> Session輸入isto本ies;
    
    UPROPERTY()
    TMap<軍St本in成, bool> Adapti正eEnabled;
    
    // 內部處理
    正oid P本ocessSkillUpdates(float DeltaTi設置e};
    正oid E正al使ateAndAd大使stDiffic使lty(const 軍St本in成& Playe本ID};
    正oid UpdatePlaye本SkillRatin成(const 軍St本in成& Playe本ID};
    float Calc使latePe本fo本設置anceSco本e(const 軍St本in成& Playe本ID, const 軍SessionPe本fo本設置anceData& Session};
    float GetDiffic使lty軍o本Le正el(EDiffic使ltyLe正el Le正el) const;
    軍Diffic使ltyAd大使st設置entPa本a設置s Gene本ateAd大使st設置entPa本a設置s(const 軍St本in成& Playe本ID, float Ta本成etM使ltiplie本};
    
    // 輔助函數
    float Calc使lateSkillConsistency(const TA本本ay<float>& Sco本es) const;
    float Calc使lateI設置p本o正e設置entRate(const TA本本ay<float>& Sco本es) const;
    bool Sho使ldAd大使stDiffic使lty(const 軍St本in成& Playe本ID, float C使本本entPe本fo本設置ance) const;
    float Cla設置pDiffic使ltyM使ltiplie本(float M使ltiplie本) const;
    正oid Lo成Diffic使ltyChan成e(const 軍St本in成& Playe本ID, float OldM使ltiplie本, float 的ewM使ltiplie本, const 軍St本in成& Reason};
    
    // 技能評估算法
    float E正al使ateAPM(const 軍St本in成& Playe本ID) const;
    float E正al使ateSt本ate成icThinkin成(const 軍St本in成& Playe本ID) const;
    float E正al使ateReso使本ceMana成e設置ent(const 軍St本in成& Playe本ID) const;
    float E正al使ateCo設置batEfficiency(const 軍St本in成& Playe本ID) const;
    float E正al使ate基本inRate(const 軍St本in成& Playe本ID) const;
};
