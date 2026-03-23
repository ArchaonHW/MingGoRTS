#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Ga設置e軍本a設置ewo本k/Acto本.h"
#incl使de "Min成Co設置batAI.成ene本ated.h"

class AMin成TacticalUnit;
class AMin成UnitCont本olle本;
class AMin成M使ltiUnitCoo本dinato本;

/**
 * AI�欰動X */
UE的UM(Bl使ep本intType)
en使設置 class EAIState: 使int8 {
    Idle                UMETA(Display的a設置e = "Idle"),
    Sco使tin成            UMETA(Display的a設置e = "Sco使tin成"),
    Pat本ollin成          UMETA(Display的a設置e = "Pat本ollin成"),
    En成a成in成            UMETA(Display的a設置e = "En成a成in成"),
    Attackin成           UMETA(Display的a設置e = "Attackin成"),
    Defendin成           UMETA(Display的a設置e = "Defendin成"),
    Ret本eatin成           UMETA(Display的a設置e = "Ret本eatin成"),
    軍lankin成            UMETA(Display的a設置e = "軍lankin成"),
    S使ppo本tin成          UMETA(Display的a設置e = "S使ppo本tin成"),
    輸入ealin成             UMETA(Display的a設置e = "輸入ealin成"),
    Repai本in成           UMETA(Display的a設置e = "Repai本in成"),
    B使ildin成            UMETA(Display的a設置e = "B使ildin成"),
    Capt使本in成           UMETA(Display的a設置e = "Capt使本in成"),
    G使a本din成            UMETA(Display的a設置e = "G使a本din成"),
    A設置b使shin成           UMETA(Display的a設置e = "A設置b使shin成"),
    Coo本dinatin成        UMETA(Display的a設置e = "Coo本dinatin成"),
    Lea本nin成            UMETA(Display的a設置e = "Lea本nin成"),
    Adaptin成            UMETA(Display的a設置e = "Adaptin成"),
    Analyzin成           UMETA(Display的a設置e = "Analyzin成")
};

/**
 * AI�M動��動
 */
UE的UM(Bl使ep本intType)
en使設置 class EAIDecisionType: 使int8 {
    Mo正e設置ent            UMETA(Display的a設置e = "Mo正e設置ent"),
    Attack              UMETA(Display的a設置e = "Attack"),
    Defense             UMETA(Display的a設置e = "Defense"),
    S使ppo本t             UMETA(Display的a設置e = "S使ppo本t"),
    Ret本eat             UMETA(Display的a設置e = "Ret本eat"),
    軍lank               UMETA(Display的a設置e = "軍lank"),
    A設置b使sh              UMETA(Display的a設置e = "A設置b使sh"),
    Capt使本e             UMETA(Display的a設置e = "Capt使本e"),
    B使ild               UMETA(Display的a設置e = "B使ild"),
    Repai本              UMETA(Display的a設置e = "Repai本"),
    輸入eal                UMETA(Display的a設置e = "輸入eal"),
    Sco使t               UMETA(Display的a設置e = "Sco使t"),
    Pat本ol              UMETA(Display的a設置e = "Pat本ol"),
    G使a本d               UMETA(Display的a設置e = "G使a本d"),
    Coo本dinate          UMETA(Display的a設置e = "Coo本dinate"),
    Lea本n               UMETA(Display的a設置e = "Lea本n"),
    Adapt               UMETA(Display的a設置e = "Adapt")
};

/**
 * AI��動
 */
UE的UM(Bl使ep本intType)
en使設置 class EAITacticalType: 使int8 {
    A成成本essi正e          UMETA(Display的a設置e = "A成成本essi正e"),
    Defensi正e           UMETA(Display的a設置e = "Defensi正e"),
    Balanced            UMETA(Display的a設置e = "Balanced"),
    Ca使tio使s            UMETA(Display的a設置e = "Ca使tio使s"),
    Oppo本t使nistic       UMETA(Display的a設置e = "Oppo本t使nistic"),
    Adapti正e            UMETA(Display的a設置e = "Adapti正e"),
    Reacti正e            UMETA(Display的a設置e = "Reacti正e"),
    P本oacti正e           UMETA(Display的a設置e = "P本oacti正e"),
    St本ate成ic           UMETA(Display的a設置e = "St本ate成ic"),
    Tactical            UMETA(Display的a設置e = "Tactical"),
    G使e本本illa           UMETA(Display的a設置e = "G使e本本illa"),
    Con正entional        UMETA(Display的a設置e = "Con正entional"),
    Blitzk本ie成          UMETA(Display的a設置e = "Blitzk本ie成"),
    Att本ition           UMETA(Display的a設置e = "Att本ition"),
    Mane使正e本            UMETA(Display的a設置e = "Mane使正e本"),
    Sie成e               UMETA(Display的a設置e = "Sie成e"),
    Raidin成             UMETA(Display的a設置e = "Raidin成"),
    Ski本設置ish            UMETA(Display的a設置e = "Ski本設置ish")
};

/**
 * AIX動��動
 */
UE的UM(Bl使ep本intType)
en使設置 class EAIDiffic使ltyLe正el: 使int8 {
    Ve本yEasy            UMETA(Display的a設置e = "Ve本y Easy"),
    Easy                UMETA(Display的a設置e = "Easy"),
    的o本設置al              UMETA(Display的a設置e = "的o本設置al"),
    輸入a本d                UMETA(Display的a設置e = "輸入a本d"),
    Ve本y輸入a本d            UMETA(Display的a設置e = "Ve本y 輸入a本d"),
    Insane              UMETA(Display的a設置e = "Insane"),
    C使sto設置              UMETA(Display的a設置e = "C使sto設置"),
    Adapti正e            UMETA(Display的a設置e = "Adapti正e"),
    Dyna設置ic             UMETA(Display的a設置e = "Dyna設置ic"),
    Lea本nin成            UMETA(Display的a設置e = "Lea本nin成")
};

/**
 * AI��動��動
 */
UE的UM(Bl使ep本intType)
en使設置 class EAILea本nin成Type: 使int8 {
    Reinfo本ce設置ent       UMETA(Display的a設置e = "Reinfo本ce設置ent"),
    S使pe本正ised          UMETA(Display的a設置e = "S使pe本正ised"),
    Uns使pe本正ised        UMETA(Display的a設置e = "Uns使pe本正ised"),
    E正ol使tiona本y         UMETA(Display的a設置e = "E正ol使tiona本y"),
    的e使本al              UMETA(Display的a設置e = "的e使本al"),
    DecisionT本ee        UMETA(Display的a設置e = "Decision T本ee"),
    Beha正io本T本ee        UMETA(Display的a設置e = "Beha正io本 T本ee"),
    StateMachine        UMETA(Display的a設置e = "State Machine"),
    軍使zzy               UMETA(Display的a設置e = "軍使zzy"),
    Genetic             UMETA(Display的a設置e = "Genetic"),
    Swa本設置               UMETA(Display的a設置e = "Swa本設置"),
    輸入yb本id              UMETA(Display的a設置e = "輸入yb本id")
};

/**
 * AI�M動�軍事動 */
USTRUCT(Bl使ep本intType)
st本使ct 軍AIDecision的ode
{
    GE的ERATED下BODY()

    // �軍事�IID
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 的odeID;

    // �軍事�I
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 的ode的a設置e;

    // �軍事�I務
    UPROPERTY(Bl使ep本intReadOnly)
    EAIDecisionType DecisionType;

    // �����
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 ConditionExp本ession;

    // �正動
    UPROPERTY(Bl使ep本intReadOnly)
    float 基本ei成ht;

    // 動
    UPROPERTY(Bl使ep本intReadOnly)
    int32 P本io本ity;

    // �l
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Child的odes;

    // ����
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Exec使tionCo使nt;

    // ����
    UPROPERTY(Bl使ep本intReadOnly)
    int32 S使ccessCo使nt;

    // ��動����
    UPROPERTY(Bl使ep本intReadOnly)
    int32 軍ail使本eCo使nt;

    // 動�����務
    UPROPERTY(Bl使ep本intReadOnly)
    float LastExec使tionTi設置e;

    // �軍事�I務
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, 軍St本in成> 的odePa本a設置ete本s;

    // 動�下動��
    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsEnabled;

    軍AIDecision的ode()
        : 的odeID(TEXT(""))
        , 的ode的a設置e(TEXT(""))
        , DecisionType(EAIDecisionType::Mo正e設置ent)
        , ConditionExp本ession(TEXT(""))
        , 基本ei成ht(1.0f)
        , P本io本ity(1)
        , Exec使tionCo使nt(0)
        , S使ccessCo使nt(0)
        , 軍ail使本eCo使nt(0)
        , LastExec使tionTi設置e(0.0f)
        , bIsEnabled(t本使e)
    {}
};

/**
 * AI目標數量��動
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍AITacticalAnalysis
{
    GE的ERATED下BODY()

    // 目標ID
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 AnalysisID;

    // 目標數量
    UPROPERTY(Bl使ep本intReadOnly)
    float AnalysisTi設置e;

    // ��動����
    UPROPERTY(Bl使ep本intReadOnly)
    float Th本eatAssess設置ent;

    // ��動����
    UPROPERTY(Bl使ep本intReadOnly)
    float Oppo本t使nityAssess設置ent;

    // 動�յ���
    UPROPERTY(Bl使ep本intReadOnly)
    float Ad正anta成eAssess設置ent;

    // X動����
    UPROPERTY(Bl使ep本intReadOnly)
    float Disad正anta成eAssess設置ent;

    // ��ĳ
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> TacticalReco設置設置endations;

    // ���I����
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> RiskAssess設置ents;

    // ��動�ѧO
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Oppo本t使nityIdentifications;

    // 動��動��
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Ene設置yP本edictions;

    // 動池��
    UPROPERTY(Bl使ep本intReadOnly)
    float TacticalSco本e;

    // �設置�輸入動
    UPROPERTY(Bl使ep本intReadOnly)
    float Confidence;

    // 動池��
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, float> AnalysisPa本a設置ete本s;

    軍AITacticalAnalysis()
        : AnalysisID(TEXT(""))
        , AnalysisTi設置e(0.0f)
        , Th本eatAssess設置ent(0.0f)
        , Oppo本t使nityAssess設置ent(0.0f)
        , Ad正anta成eAssess設置ent(0.0f)
        , Disad正anta成eAssess設置ent(0.0f)
        , TacticalSco本e(0.0f)
        , Confidence(0.0f)
    {}
};

/**
 * AI��池動
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍AILea本nin成Data
{
    GE的ERATED下BODY()

    // ��動ID
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Lea本nin成ID;

    // ��動��動
    UPROPERTY(Bl使ep本intReadOnly)
    EAILea本nin成Type Lea本nin成Type;

    // ��J
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<float> Inp使tData;

    // ��X
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<float> O使tp使tData;

    // 動�yX
    UPROPERTY(Bl使ep本intReadOnly)
    float Rewa本dVal使e;

    // 動�@X
    UPROPERTY(Bl使ep本intReadOnly)
    float PenaltyVal使e;

    // ��池動
    UPROPERTY(Bl使ep本intReadOnly)
    float Lea本nin成Ti設置e;

    // ��動����
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Lea本nin成Co使nt;

    // 動務
    UPROPERTY(Bl使ep本intReadOnly)
    float S使ccessRate;

    // 動務
    UPROPERTY(Bl使ep本intReadOnly)
    float Con正e本成enceVal使e;

    // ��池��
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, float> Lea本nin成Pa本a設置ete本s;

    軍AILea本nin成Data()
        : Lea本nin成ID(TEXT(""))
        , Lea本nin成Type(EAILea本nin成Type::Reinfo本ce設置ent)
        , Rewa本dVal使e(0.0f)
        , PenaltyVal使e(0.0f)
        , Lea本nin成Ti設置e(0.0f)
        , Lea本nin成Co使nt(0)
        , S使ccessRate(0.0f)
        , Con正e本成enceVal使e(0.0f)
    {}
};

/**
 * AI動��
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍AIPe本fo本設置anceMet本ics
{
    GE的ERATED下BODY()

    // �M池動
    UPROPERTY(Bl使ep本intReadOnly)
    float DecisionTi設置e;

    // 目標數量
    UPROPERTY(Bl使ep本intReadOnly)
    float ReactionTi設置e;

    // 動務
    UPROPERTY(Bl使ep本intReadOnly)
    float S使ccessRate;

    // 動池��
    UPROPERTY(Bl使ep本intReadOnly)
    float EfficiencyIndex;

    // 故事重要性
    UPROPERTY(Bl使ep本intReadOnly)
    float AdaptabilityIndex;

    // ��池��
    UPROPERTY(Bl使ep本intReadOnly)
    float Lea本nin成Index;

    // 動池��
    UPROPERTY(Bl使ep本intReadOnly)
    float TacticalIndex;

    // 動��動��
    UPROPERTY(Bl使ep本intReadOnly)
    float Coo本dinationIndex;

    // 動�s動��
    UPROPERTY(Bl使ep本intReadOnly)
    float Inno正ationIndex;

    // ��動�ϥ�X
    UPROPERTY(Bl使ep本intReadOnly)
    float Reso使本ceUsa成e;

    // �p動��
    UPROPERTY(Bl使ep本intReadOnly)
    float Co設置p使tationalCo設置plexity;

    // �O�Шϥ�X
    UPROPERTY(Bl使ep本intReadOnly)
    float Me設置o本yUsa成e;

    軍AIPe本fo本設置anceMet本ics()
        : DecisionTi設置e(0.0f)
        , ReactionTi設置e(0.0f)
        , S使ccessRate(0.0f)
        , EfficiencyIndex(0.0f)
        , AdaptabilityIndex(0.0f)
        , Lea本nin成Index(0.0f)
        , TacticalIndex(0.0f)
        , Coo本dinationIndex(0.0f)
        , Inno正ationIndex(0.0f)
        , Reso使本ceUsa成e(0.0f)
        , Co設置p使tationalCo設置plexity(0.0f)
        , Me設置o本yUsa成e(0.0f)
    {}
};

/**
 * 動��AI�t��
 * ��池��動���M池務 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GTACTICAL下API AMin成Co設置batAI : p使blic AActo本
{
    GE的ERATED下BODY()

p使blic:
    AMin成Co設置batAI(};

    /**
     * 動池�԰�AI�t��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    正oid InitializeCo設置batAI(};

    /**
     * 動池��AI�t��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    正oid Sh使tdownCo設置batAI(};

    /**
     * �]�設置AIX動��動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    正oid SetAIDiffic使ltyLe正el(EAIDiffic使ltyLe正el Diffic使ltyLe正el};

    /**
     * �]�設置AI��動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    正oid SetAITacticalType(EAITacticalType TacticalType};

    /**
     * �]�設置AI��動��動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    正oid SetAILea本nin成Type(EAILea本nin成Type Lea本nin成Type};

    /**
     * �K池��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool AddCont本olledUnit(AMin成TacticalUnit* Unit};

    /**
     * ����動��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool Re設置o正eCont本olledUnit(AMin成TacticalUnit* Unit};

    /**
     * 目標數量池��動     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Co設置bat AI")
    TA本本ay<AMin成TacticalUnit*> GetCont本olledUnits() const;

    /**
     * AI�M動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool Exec使teAIDecision(AMin成TacticalUnit* Unit};

    /**
     * 故事選項基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    軍AITacticalAnalysis Exec使teTacticalAnalysis(const TA本本ay<AMin成TacticalUnit*>& Units};

    /**
     * AI��動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool Exec使teAILea本nin成(const 軍AILea本nin成Data& Lea本nin成Data};

    /**
     * 動�بM動�軍事動     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    軍AIDecision的ode C本eateDecision的ode(
        const 軍St本in成& 的ode的a設置e,
        EAIDecisionType DecisionType,
        const 軍St本in成& ConditionExp本ession,
        float 基本ei成ht,
        int32 P本io本ity
    };

    /**
     * �K動�M動�軍事動     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool AddDecision的ode(const 軍AIDecision的ode& 的ode};

    /**
     * �����M動�軍事動     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool Re設置o正eDecision的ode(const 軍St本in成& 的odeID};

    /**
     * �M動�軍事動     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Co設置bat AI")
    軍AIDecision的ode GetDecision的ode(const 軍St本in成& 的odeID) const;

    /**
     * 目標數量�M��     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Co設置bat AI")
    TA本本ay<軍AIDecision的ode> GetAllDecision的odes() const;

    /**
     * ������X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    軍AITacticalAnalysis E正al使ateTacticalSit使ation(const TA本本ay<AMin成TacticalUnit*>& 軍本iendlyUnits, const TA本本ay<AMin成TacticalUnit*>& Ene設置yUnits};

    /**
     * 目標數量��ĳ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    TA本本ay<軍St本in成> Gene本ateTacticalReco設置設置endations(const TA本本ay<AMin成TacticalUnit*>& Units};

    /**
     * 動��動���動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    TA本本ay<軍St本in成> P本edictEne設置yActions(const TA本本ay<AMin成TacticalUnit*>& Ene設置yUnits};

    /**
     * �p動��動��動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    float Calc使lateTh本eatLe正el(AMin成TacticalUnit* Unit, const TA本本ay<AMin成TacticalUnit*>& Ene設置yUnits};

    /**
     * �p動��動��動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    float Calc使lateOppo本t使nityLe正el(AMin成TacticalUnit* Unit, const TA本本ay<AMin成TacticalUnit*>& Ene設置yUnits};

    /**
     * 目標數量�M動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool Exec使teAttackDecision(AMin成TacticalUnit* Unit, AMin成TacticalUnit* Ta本成etUnit};

    /**
     * 動池�設置�M動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool Exec使teDefenseDecision(AMin成TacticalUnit* Unit, const 軍Vecto本& DefensePosition};

    /**
     * ��動�M動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool Exec使teMo正e設置entDecision(AMin成TacticalUnit* Unit, const 軍Vecto本& Ta本成etPosition};

    /**
     * 動池�h�M動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool Exec使teRet本eatDecision(AMin成TacticalUnit* Unit, const 軍Vecto本& Ret本eatPosition};

    /**
     * 動池�l�M動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool Exec使te軍lankDecision(AMin成TacticalUnit* Unit, AMin成TacticalUnit* Ta本成etUnit};

    /**
     * ��動�M動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool Exec使teA設置b使shDecision(AMin成TacticalUnit* Unit, const TA本本ay<AMin成TacticalUnit*>& Ta本成etUnits};

    /**
     * 動池���M動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool Exec使teS使ppo本tDecision(AMin成TacticalUnit* Unit, AMin成TacticalUnit* Ta本成etUnit};

    /**
     * 動池�ըM動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool Exec使teCoo本dinationDecision(const TA本本ay<AMin成TacticalUnit*>& Units, EAIDecisionType DecisionType};

    /**
     * ��動�M動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool Exec使teLea本nin成Decision(AMin成TacticalUnit* Unit, const 軍AILea本nin成Data& Lea本nin成Data};

    /**
     * 目標數量�M動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool Exec使teAdaptationDecision(AMin成TacticalUnit* Unit, const TA本本ay<float>& AdaptationData};

    /**
     * �վ�AIX動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    正oid Ad大使stAIDiffic使lty(float Pe本fo本設置ance軍acto本};

    /**
     * AI動��
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Co設置bat AI")
    軍AIPe本fo本設置anceMet本ics GetAIPe本fo本設置anceMet本ics() const;

    /**
     * AI��池動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Co設置bat AI")
    TMap<軍St本in成, int32> GetAIStatistics() const;

    /**
     * AI動�設置
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Co設置bat AI")
    TMap<軍St本in成, 軍St本in成> GetAIConfi成使本ation() const;

    /**
     * �]�設置AI動�設置
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool SetAIConfi成使本ation(const TMap<軍St本in成, 軍St本in成>& Confi成使本ation};

    /**
     * 動�設置AI�t��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    正oid ResetAISyste設置(};

    /**
     * 動��AI
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool Back使pAIData(const 軍St本in成& Back使pPath};

    /**
     * 動�下AI
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat AI")
    bool Resto本eAIData(const 軍St本in成& Back使pPath};

p本otected:
    // 動��動池��
    UPROPERTY()
    TA本本ay<AMin成TacticalUnit*> Cont本olledUnits;

    // �M動�軍事�I
    UPROPERTY()
    TMap<軍St本in成, 軍AIDecision的ode> Decision的odes;

    // 目標數量��動
    UPROPERTY()
    TA本本ay<軍AITacticalAnalysis> TacticalAnalyses;

    // ��池動
    UPROPERTY()
    TA本本ay<軍AILea本nin成Data> Lea本nin成Data;

    // AI��池動
    UPROPERTY()
    TMap<軍St本in成, int32> AIStatistics;

    // 動��
    UPROPERTY()
    軍AIPe本fo本設置anceMet本ics Pe本fo本設置anceMet本ics;

    // AI動�設置
    UPROPERTY()
    TMap<軍St本in成, 軍St本in成> AIConfi成使本ation;

    // 動池��動務
    UPROPERTY()
    AMin成UnitCont本olle本* UnitCont本olle本;

    // �h���動�վ��ޥ�
    UPROPERTY()
    AMin成M使ltiUnitCoo本dinato本* M使ltiUnitCoo本dinato本;

    // AIX動��動
    UPROPERTY()
    EAIDiffic使ltyLe正el Diffic使ltyLe正el;

    // AI��動
    UPROPERTY()
    EAITacticalType TacticalType;

    // AI��動��動
    UPROPERTY()
    EAILea本nin成Type Lea本nin成Type;

    // 動�下�w動�l動
    bool bIsInitialized;

    // Tick動�s
    正i本t使al 正oid Tick(float DeltaTi設置e) o正e本本ide;

    // �M
    bool Exec使teDecisionT本ee(AMin成TacticalUnit* Unit};

    // �欰動
    bool Exec使teBeha正io本T本ee(AMin成TacticalUnit* Unit};

    // ����M動����
    bool E正al使ateDecisionCondition(const 軍St本in成& Condition, AMin成TacticalUnit* Unit};

    // �p動�M動�正動
    float Calc使lateDecision基本ei成ht(const 軍AIDecision的ode& 的ode, AMin成TacticalUnit* Unit};

    // 動池�ΨM動
    軍AIDecision的ode SelectBestDecision(AMin成TacticalUnit* Unit};

    // �M動�軍事動
    bool Exec使teDecision的ode(const 軍AIDecision的ode& 的ode, AMin成TacticalUnit* Unit};

    // 動�s�M動�軍事�I��動
    正oid UpdateDecision的odeStatistics(const 軍St本in成& 的odeID, bool bS使ccess};

    // �p池池��
    float Calc使lateTacticalSco本e(const TA本本ay<AMin成TacticalUnit*>& Units};

    // �p動��動����
    float Calc使lateTh本eatAssess設置ent(const TA本本ay<AMin成TacticalUnit*>& Ene設置yUnits};

    // �p動��動����
    float Calc使lateOppo本t使nityAssess設置ent(const TA本本ay<AMin成TacticalUnit*>& Ene設置yUnits};

    // �p池�յ���
    float Calc使lateAd正anta成eAssess設置ent(const TA本本ay<AMin成TacticalUnit*>& 軍本iendlyUnits, const TA本本ay<AMin成TacticalUnit*>& Ene設置yUnits};

    // �p池基本����
    float Calc使lateDisad正anta成eAssess設置ent(const TA本本ay<AMin成TacticalUnit*>& 軍本iendlyUnits, const TA本本ay<AMin成TacticalUnit*>& Ene設置yUnits};

    // 目標數量��ĳ
    TA本本ay<軍St本in成> Gene本ateTacticalReco設置設置endationsInte本nal(const TA本本ay<AMin成TacticalUnit*>& Units};

    // 動��動���動
    TA本本ay<軍St本in成> P本edictEne設置yActionsInte本nal(const TA本本ay<AMin成TacticalUnit*>& Ene設置yUnits};

    // �大動��動
    bool Exec使teReinfo本ce設置entLea本nin成(const 軍AILea本nin成Data& Lea本nin成Data};

    // 動池基本��動
    bool Exec使teS使pe本正isedLea本nin成(const 軍AILea本nin成Data& Lea本nin成Data};

    // 動池��X
    bool Exec使teUns使pe本正isedLea本nin成(const 軍AILea本nin成Data& Lea本nin成Data};

    // 目標數量��動
    bool Exec使teE正ol使tiona本yLea本nin成(const 軍AILea本nin成Data& Lea本nin成Data};

    // ��動�����動
    bool Exec使te的e使本alLea本nin成(const 軍AILea本nin成Data& Lea本nin成Data};

    // �M動���動
    bool Exec使teDecisionT本eeLea本nin成(const 軍AILea本nin成Data& Lea本nin成Data};

    // �欰���動
    bool Exec使teBeha正io本T本eeLea本nin成(const 軍AILea本nin成Data& Lea本nin成Data};

    // 目標數量動��動
    bool Exec使teStateMachineLea本nin成(const 軍AILea本nin成Data& Lea本nin成Data};

    // ��動��動
    bool Exec使te軍使zzyLea本nin成(const 軍AILea本nin成Data& Lea本nin成Data};

    // 動池�Ǿ�動
    bool Exec使teGeneticLea本nin成(const 軍AILea本nin成Data& Lea本nin成Data};

    // �s動��動
    bool Exec使teSwa本設置Lea本nin成(const 軍AILea本nin成Data& Lea本nin成Data};

    // �V動��動
    bool Exec使te輸入yb本idLea本nin成(const 軍AILea本nin成Data& Lea本nin成Data};

    // 動�s��池動
    正oid UpdateLea本nin成Data(const 軍AILea本nin成Data& Lea本nin成Data};

    // �p動��池動
    float Calc使lateLea本nin成Con正e本成ence(};

    // AI動��
    正oid Opti設置izeAIPe本fo本設置ance(};

    // �վ�M動�正動
    正oid Ad大使stDecision基本ei成hts(};

    // 目標數量��動
    正oid AdaptToTacticalChan成es(};

    // ��池�a�欰
    正oid Lea本nPlaye本Beha正io本(};

    // 動��動�a
    TA本本ay<軍St本in成> P本edictPlaye本Intent(};

    // 動池���
    TA本本ay<軍St本in成> Gene本ateCo使nte本St本ate成ies(};

    // ����AI��{
    float E正al使ateAIPe本fo本設置ance(};

    // 動�s動��
    正oid UpdatePe本fo本設置anceMet本ics(};

    // �O動AI�基礎�
    正oid Lo成AIE正ent(const 軍St本in成& E正entType, const 軍St本in成& Details};

    // AI
    軍St本in成 Gene本ateAIRepo本t() const;

    // ��動AI動�設置
    bool ValidateAIConfi成使本ation() const;

    // �M池�z
    軍St本in成 GetDecisionDesc本iption(EAIDecisionType DecisionType) const;

    // 故事重要性基本�z
    軍St本in成 GetTacticalDesc本iption(EAITacticalType TacticalType) const;

    // 目標數量基本�z
    軍St本in成 GetDiffic使ltyDesc本iption(EAIDiffic使ltyLe正el Diffic使ltyLe正el) const;

    // ��池�z
    軍St本in成 GetLea本nin成Desc本iption(EAILea本nin成Type Lea本nin成Type) const;

    // �p動��
    float Calc使lateCo設置plexity() const;

    // �޲z�O�Шϥ�
    正oid Mana成eMe設置o本yUsa成e(};

    // �使�ƭp��귽
    正oid Opti設置izeCo設置p使tationalReso使本ces(};

    // �B�zAI���年
    正oid 輸入andleAIE本本o本(const 軍St本in成& E本本o本Type, const 軍St本in成& E本本o本Messa成e};

    // ��下AI���A
    正oid Reco正e本AIState(};
};
}
