#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成DecisionConseq使enceCalc使lato本.成ene本ated.h"

/**
 * 決�基本後�基本類�基本
 */
UE的UM(Bl使ep本intType)
en使設置 class EDecisionConseq使enceType: 使int8 {
    I設置設置ediate           UMETA(Display的a設置e = "I設置設置ediate"),
    Delayed             UMETA(Display的a設置e = "Delayed"),
    C使設置使lati正e          UMETA(Display的a設置e = "C使設置使lati正e"),
    Conditional         UMETA(Display的a設置e = "Conditional"),
    Rando設置              UMETA(Display的a設置e = "Rando設置"),
    輸入idden              UMETA(Display的a設置e = "輸入idden"),
    Pe本設置anent           UMETA(Display的a設置e = "Pe本設置anent"),
    Te設置po本a本y           UMETA(Display的a設置e = "Te設置po本a本y")
};

/**
 * 後�基本影響範�基本
 */
UE的UM(Bl使ep本intType)
en使設置 class EConseq使enceI設置pactScope: 使int8 {
    Pe本sonal            UMETA(Display的a設置e = "Pe本sonal"),
    Local               UMETA(Display的a設置e = "Local"),
    Re成ional            UMETA(Display的a設置e = "Re成ional"),
    的ational            UMETA(Display的a設置e = "的ational"),
    Inte本national       UMETA(Display的a設置e = "Inte本national"),
    Global              UMETA(Display的a設置e = "Global")
};

/**
 * 後�X��基本程度
 */
UE的UM(Bl使ep本intType)
en使設置 class EConseq使enceSe正e本ity: 使int8 {
    T本i正ial             UMETA(Display的a設置e = "T本i正ial"),
    Mino本               UMETA(Display的a設置e = "Mino本"),
    Mode本ate            UMETA(Display的a設置e = "Mode本ate"),
    Ma大o本               UMETA(Display的a設置e = "Ma大o本"),
    C本itical            UMETA(Display的a設置e = "C本itical"),
    Catast本ophic        UMETA(Display的a設置e = "Catast本ophic")
};

/**
 * 後�X��X��基本
 */
UE的UM(Bl使ep本intType)
en使設置 class EConseq使enceD使本ation: 使int8 {
    Instant             UMETA(Display的a設置e = "Instant"),
    Sho本t               UMETA(Display的a設置e = "Sho本t"),
    Medi使設置              UMETA(Display的a設置e = "Medi使設置"),
    Lon成                UMETA(Display的a設置e = "Lon成"),
    Pe本設置anent           UMETA(Display的a設置e = "Pe本設置anent"),
    Indefinite          UMETA(Display的a設置e = "Indefinite")
};

/**
 * 後�基本影響類�基本
 */
UE的UM(Bl使ep本intType)
en使設置 class EConseq使enceI設置pactType: 使int8 {
    Positi正e            UMETA(Display的a設置e = "Positi正e"),
    的e成ati正e            UMETA(Display的a設置e = "的e成ati正e"),
    Mixed               UMETA(Display的a設置e = "Mixed"),
    的e使t本al             UMETA(Display的a設置e = "的e使t本al"),
    Unknown             UMETA(Display的a設置e = "Unknown")
};

/**
 * 後�X�數
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Conseq使encePa本a設置ete本
{
    GE的ERATED下BODY()

    // 基本�數基本�稱
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Conseq使ence Pa本a設置ete本")
    軍St本in成 Pa本a設置ete本的a設置e;

    // 基本�數X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Conseq使ence Pa本a設置ete本")
    軍St本in成 Pa本a設置ete本Val使e;

    // 基本�數類�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Conseq使ence Pa本a設置ete本")
    軍St本in成 Pa本a設置ete本Type; // "int", "float", "bool", "st本in成", "a本本ay"

    // 基本�否必�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Conseq使ence Pa本a設置ete本")
    bool bIsReq使i本ed;

    // 默�X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Conseq使ence Pa本a設置ete本")
    軍St本in成 Defa使ltVal使e;

    // 基本�述
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Conseq使ence Pa本a設置ete本")
    軍St本in成 Desc本iption;

    軍Conseq使encePa本a設置ete本()
        : Pa本a設置ete本的a設置e(TEXT(""))
        , Pa本a設置ete本Val使e(TEXT(""))
        , Pa本a設置ete本Type(TEXT("st本in成"))
        , bIsReq使i本ed(false)
        , Defa使ltVal使e(TEXT(""))
        , Desc本iption(TEXT(""))
    {}
};

/**
 * 決�基本後�基本定義
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍DecisionConseq使ence
{
    GE的ERATED下BODY()

    // 後�基本ID
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Conseq使enceID;

    // 後�X�稱
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Conseq使ence的a設置e;

    // 後�X�述
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Conseq使enceDesc本iption;

    // 後�基本類�基本
    UPROPERTY(Bl使ep本intReadOnly)
    EDecisionConseq使enceType Conseq使enceType;

    // 影響範�基本
    UPROPERTY(Bl使ep本intReadOnly)
    EConseq使enceI設置pactScope I設置pactScope;

    // 基本��基本程度
    UPROPERTY(Bl使ep本intReadOnly)
    EConseq使enceSe正e本ity Se正e本ity;

    // 基本��X��基本
    UPROPERTY(Bl使ep本intReadOnly)
    EConseq使enceD使本ation D使本ation;

    // 影響類�基本
    UPROPERTY(Bl使ep本intReadOnly)
    EConseq使enceI設置pactType I設置pactType;

    // 後�X�數
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍Conseq使encePa本a設置ete本> Pa本a設置ete本s;

    // 觸發條件
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> T本i成成e本Conditions;

    // 後�X��基本
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Effects;

    // 後�基本標籤
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Conseq使enceTa成s;

    // 基本��基本決�基本ID
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 RelatedDecisionID;

    // 基本��基本人物ID
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> RelatedCha本acte本IDs;

    // 基本��基本事件ID
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> RelatedE正entIDs;

    // 基本��基本概�基本 (0-1)
    UPROPERTY(Bl使ep本intReadOnly)
    float P本obability;

    // 延�X��基本 (�基本
    UPROPERTY(Bl使ep本intReadOnly)
    float DelayTi設置e;

    // 基本�卻基本��基本 (�基本
    UPROPERTY(Bl使ep本intReadOnly)
    float CooldownTi設置e;

    // 基本�否已觸X
    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsT本i成成e本ed;

    // 觸發基本��基本
    UPROPERTY(Bl使ep本intReadOnly)
    float T本i成成e本Ti設置e;

    // 基本�否基本�逆�基本
    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsRe正e本sible;

    // 基本�逆�基本條件
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Re正e本salConditions;

    軍DecisionConseq使ence()
        : Conseq使enceID(TEXT(""))
        , Conseq使ence的a設置e(TEXT(""))
        , Conseq使enceDesc本iption(TEXT(""))
        , Conseq使enceType(EDecisionConseq使enceType::I設置設置ediate)
        , I設置pactScope(EConseq使enceI設置pactScope::Local)
        , Se正e本ity(EConseq使enceSe正e本ity::Mode本ate)
        , D使本ation(EConseq使enceD使本ation::Medi使設置)
        , I設置pactType(EConseq使enceI設置pactType::的e使t本al)
        , RelatedDecisionID(TEXT(""))
        , P本obability(1.0f)
        , DelayTi設置e(0.0f)
        , CooldownTi設置e(0.0f)
        , bIsT本i成成e本ed(false)
        , T本i成成e本Ti設置e(0.0f)
        , bIsRe正e本sible(false)
    {}
};

/**
 * 後�基本計�基本結�基本
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Conseq使enceCalc使lationRes使lt
{
    GE的ERATED下BODY()

    // 基本�否基本��基本計�基本
    UPROPERTY(Bl使ep本intReadOnly)
    bool bS使ccess;

    // 後�基本ID
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Conseq使enceID;

    // 計�X��基本
    UPROPERTY(Bl使ep本intReadOnly)
    float Calc使lationTi設置e;

    // 計�X��基本後�基本
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍DecisionConseq使ence> Calc使latedConseq使ences;

    // 影響基本��X��X
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, 軍St本in成> AffectedGa設置eState;

    // 影響基本�人X
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> AffectedCha本acte本s;

    // 影響基本��基本�基本
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> AffectedE正ents;

    // 影響基本��基本�基本
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, int32> AffectedReso使本ces;

    // 影響基本�建�基本
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> AffectedB使ildin成s;

    // 影響基本�單�基本
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> AffectedUnits;

    // 總�基本影響基本�數
    UPROPERTY(Bl使ep本intReadOnly)
    float O正e本allI設置pactSco本e;

    // �基本��影響基本�數
    UPROPERTY(Bl使ep本intReadOnly)
    float Positi正eI設置pactSco本e;

    // 負面影響基本�數
    UPROPERTY(Bl使ep本intReadOnly)
    float 的e成ati正eI設置pactSco本e;

    // 計�基本詳�基本
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Calc使lationDetails;

    // 基本�誤信息
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 E本本o本Messa成e;

    軍Conseq使enceCalc使lationRes使lt()
        : bS使ccess(false)
        , Conseq使enceID(TEXT(""))
        , Calc使lationTi設置e(0.0f)
        , O正e本allI設置pactSco本e(0.0f)
        , Positi正eI設置pactSco本e(0.0f)
        , 的e成ati正eI設置pactSco本e(0.0f)
        , Calc使lationDetails(TEXT(""))
        , E本本o本Messa成e(TEXT(""))
    {}
};

/**
 * 決�基本上�X */
USTRUCT(Bl使ep本intType)
st本使ct 軍DecisionContext
{
    GE的ERATED下BODY()

    // 決�基本ID
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 DecisionID;

    // 決�X��基本
    UPROPERTY(Bl使ep本intReadOnly)
    float DecisionTi設置e;

    // 決�X��基本
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 DecisionLocation;

    // 決�X�ID
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 DecisionMake本ID;

    // 決�基本類�基本
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 DecisionType;

    // 決�X��基本
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 DecisionOption;

    // 基本��X�戲基本�X
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, 軍St本in成> C使本本entGa設置eState;

    // 基本��基本人物基本�X
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, 軍St本in成> Cha本acte本States;

    // 基本��基本事件基本�X
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, 軍St本in成> E正entStates;

    // 資�X�X
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, int32> Reso使本ceStates;

    // 建�X�X
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, 軍St本in成> B使ildin成States;

    // 基本��X�X
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, 軍St本in成> UnitStates;

    // 民�X��基本
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Rep使blicE本a;

    // 基本��基本年份
    UPROPERTY(Bl使ep本intReadOnly)
    int32 C使本本entYea本;

    // 基本�家X
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Playe本軍action;

    // X��等�基本
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Diffic使ltyLe正el;

    軍DecisionContext()
        : DecisionID(TEXT(""))
        , DecisionTi設置e(0.0f)
        , DecisionLocation(TEXT(""))
        , DecisionMake本ID(TEXT(""))
        , DecisionType(TEXT(""))
        , DecisionOption(TEXT(""))
        , Rep使blicE本a(TEXT(""))
        , C使本本entYea本(1912)
        , Playe本軍action(TEXT(""))
        , Diffic使ltyLe正el(1)
    {}
};

/**
 * 決�基本後�基本計�X * 計�基本決�X��基本種�X��基本影響
 */
UCLASS(Bl使ep本intType)
class MI的GSTRATEGIC下API UMin成DecisionConseq使enceCalc使lato本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成DecisionConseq使enceCalc使lato本(};

    /**
     * 基本��X��X��基本算器
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    正oid Initialize(};

    /**
     * 基本��基本後�基本計�X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    正oid Sh使tdown(};

    /**
     * 註�基本決�基本後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    bool Re成iste本DecisionConseq使ence(const 軍DecisionConseq使ence& Conseq使ence};

    /**
     * 基本��基本註�基本決�基本後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    bool Un本e成iste本DecisionConseq使ence(const 軍St本in成& Conseq使enceID};

    /**
     * 計�基本決�基本後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    軍Conseq使enceCalc使lationRes使lt Calc使lateDecisionConseq使ences(const 軍DecisionContext& Context};

    /**
     * 基本��基本決�基本後�基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使ence")
    軍DecisionConseq使ence GetDecisionConseq使ence(const 軍St本in成& Conseq使enceID) const;

    /**
     * 基本��X�基本�決策�X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使ence")
    TA本本ay<軍DecisionConseq使ence> GetAllDecisionConseq使ences() const;

    /**
     * 基本��X��基本類�X��X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    TA本本ay<軍DecisionConseq使ence> GetConseq使encesByType(EDecisionConseq使enceType Conseq使enceType) const;

    /**
     * 基本��X��基本影響範�X��X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    TA本本ay<軍DecisionConseq使ence> GetConseq使encesByScope(EConseq使enceI設置pactScope I設置pactScope) const;

    /**
     * 基本��X��X��基本程度基本��X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    TA本本ay<軍DecisionConseq使ence> GetConseq使encesBySe正e本ity(EConseq使enceSe正e本ity Se正e本ity) const;

    /**
     * 基本��X��基本影響類�X��X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    TA本本ay<軍DecisionConseq使ence> GetConseq使encesByI設置pactType(EConseq使enceI設置pactType I設置pactType) const;

    /**
     * 基本��X��基本決�X��X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    TA本本ay<軍DecisionConseq使ence> GetConseq使ences軍o本Decision(const 軍St本in成& DecisionID) const;

    /**
     * 基本��X��基本人物基本��X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    TA本本ay<軍DecisionConseq使ence> GetConseq使ences軍o本Cha本acte本(const 軍St本in成& Cha本acte本ID) const;

    /**
     * 基本��X��基本事件基本��X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    TA本本ay<軍DecisionConseq使ence> GetConseq使ences軍o本E正ent(const 軍St本in成& E正entID) const;

    /**
     * 觸發後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    bool T本i成成e本Conseq使ence(const 軍St本in成& Conseq使enceID, const 軍DecisionContext& Context};

    /**
     * 基本�用後�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    bool ApplyConseq使enceEffects(const 軍St本in成& Conseq使enceID, const 軍DecisionContext& Context};

    /**
     * 基本��基本後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    bool Re正e本seConseq使ence(const 軍St本in成& Conseq使enceID, const 軍DecisionContext& Context};

    /**
     * 基本��基本後�基本統�基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使ence")
    TMap<軍St本in成, int32> GetConseq使enceStatistics() const;

    /**
     * 基本��基本後�基本影響基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    TMap<軍St本in成, float> GetConseq使enceI設置pactAnalysis(const 軍St本in成& DecisionID) const;

    /**
     * 基本�測決�基本後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    TA本本ay<軍DecisionConseq使ence> P本edictDecisionConseq使ences(const 軍DecisionContext& Context, int32 P本edictionCo使nt};

    /**
     * 模擬決�基本後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    TA本本ay<軍Conseq使enceCalc使lationRes使lt> Si設置使lateDecisionConseq使ences(const 軍DecisionContext& Context, int32 Si設置使lationCo使nt};

p本otected:
    // 已註基本��基本決�基本後�基本
    UPROPERTY()
    TA本本ay<軍DecisionConseq使ence> Re成iste本edConseq使ences;

    // 後�基本ID基本��X��X��基本
    UPROPERTY()
    TMap<軍St本in成, 軍DecisionConseq使ence> Conseq使enceMap;

    // 後�基本統�基本
    UPROPERTY()
    TMap<軍St本in成, int32> Conseq使enceStatistics;

    // 基本�否已�基本始�基本
    bool bIsInitialized;

    // 基本��X��X�歷基本�決策�X�庫
    正oid Initialize輸入isto本icalDecisionConseq使ences(};

    // 基本�建基本�治決�基本後�基本
    正oid C本eatePoliticalDecisionConseq使ences(};

    // 基本�建軍�基本決�基本後�基本
    正oid C本eateMilita本yDecisionConseq使ences(};

    // 基本�建經�基本決�基本後�基本
    正oid C本eateEcono設置icDecisionConseq使ences(};

    // 基本�建社�基本決�基本後�基本
    正oid C本eateSocialDecisionConseq使ences(};

    // 基本�建基本��基本決�基本後�基本
    正oid C本eateC使lt使本alDecisionConseq使ences(};

    // 基本�建外交決�基本後�基本
    正oid C本eateDiplo設置aticDecisionConseq使ences(};

    // 計�X��基本後�基本
    正oid Calc使lateI設置設置ediateConseq使ences(const 軍DecisionContext& Context, 軍Conseq使enceCalc使lationRes使lt& Res使lt};

    // 計�基本延�基本後�基本
    正oid Calc使lateDelayedConseq使ences(const 軍DecisionContext& Context, 軍Conseq使enceCalc使lationRes使lt& Res使lt};

    // 計�基本累�基本後�基本
    正oid Calc使lateC使設置使lati正eConseq使ences(const 軍DecisionContext& Context, 軍Conseq使enceCalc使lationRes使lt& Res使lt};

    // 計�基本條件後�基本
    正oid Calc使lateConditionalConseq使ences(const 軍DecisionContext& Context, 軍Conseq使enceCalc使lationRes使lt& Res使lt};

    // 計�X��基本後�基本
    正oid Calc使lateRando設置Conseq使ences(const 軍DecisionContext& Context, 軍Conseq使enceCalc使lationRes使lt& Res使lt};

    // 計�X��基本後�基本
    正oid Calc使late輸入iddenConseq使ences(const 軍DecisionContext& Context, 軍Conseq使enceCalc使lationRes使lt& Res使lt};

    // 計�基本永�基本後�基本
    正oid Calc使latePe本設置anentConseq使ences(const 軍DecisionContext& Context, 軍Conseq使enceCalc使lationRes使lt& Res使lt};

    // 計�X��基本後�基本
    正oid Calc使lateTe設置po本a本yConseq使ences(const 軍DecisionContext& Context, 軍Conseq使enceCalc使lationRes使lt& Res使lt};

    // 計�基本後�基本影響基本�數
    float Calc使lateConseq使enceI設置pact(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context) const;

    // 計�基本總�基本影響基本�數
    正oid Calc使lateO正e本allI設置pact(軍Conseq使enceCalc使lationRes使lt& Res使lt) const;

    // 檢查後�基本條件
    bool CheckConseq使enceConditions(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context) const;

    // 基本�用後�X��X��X
    正oid ApplyConseq使enceToGa設置eState(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context, TMap<軍St本in成, 軍St本in成>& AffectedStates) const;

    // 基本�用後�X�人X
    正oid ApplyConseq使enceToCha本acte本s(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context, TA本本ay<軍St本in成>& AffectedCha本acte本s) const;

    // 基本�用後�X��基本�基本
    正oid ApplyConseq使enceToE正ents(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context, TA本本ay<軍St本in成>& AffectedE正ents) const;

    // 基本�用後�X��基本�基本
    正oid ApplyConseq使enceToReso使本ces(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context, TMap<軍St本in成, int32>& AffectedReso使本ces) const;

    // 基本�用後�X�建�基本
    正oid ApplyConseq使enceToB使ildin成s(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context, TA本本ay<軍St本in成>& AffectedB使ildin成s) const;

    // 基本�用後�X�單�基本
    正oid ApplyConseq使enceToUnits(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context, TA本本ay<軍St本in成>& AffectedUnits) const;

    // 基本�新後�基本統�基本
    正oid UpdateConseq使enceStatistics(const 軍St本in成& Conseq使enceID};

    // 基本��基本後�X�述
    軍St本in成 GetConseq使enceDesc本iption(const 軍St本in成& Conseq使enceID) const;

    // 驗�基本後�X�置
    bool ValidateConseq使enceConfi成(const 軍DecisionConseq使ence& Conseq使ence) const;

    // 記�基本後�X��基本
    正oid Lo成Conseq使enceE正ent(const 軍St本in成& Conseq使enceID, const 軍St本in成& Messa成e};

    // 基本��基本後�X��基本
    軍St本in成 Gene本ateConseq使enceRepo本t(const 軍St本in成& Conseq使enceID, const 軍Conseq使enceCalc使lationRes使lt& Res使lt) const;

    // �X後�X�數
    TA本本ay<軍St本in成> Pa本seConseq使encePa本a設置ete本s(const 軍St本in成& Pa本a設置ete本St本in成) const;

    // 計�基本後�基本概�基本
    float Calc使lateConseq使enceP本obability(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context) const;

    // 計�基本後�基本延�基本
    float Calc使lateConseq使enceDelay(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context) const;

    // 計�基本後�X��X��基本
    float Calc使lateConseq使enceD使本ation(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context) const;

    // 檢查後�X�否基本�逆�基本
    bool IsConseq使enceRe正e本sible(const 軍DecisionConseq使ence& Conseq使ence, const 軍DecisionContext& Context) const;

    // 基本��基本後�X��基本�基本
    int32 GetConseq使enceP本io本ity(const 軍DecisionConseq使ence& Conseq使ence) const;

    // 基本��基本後�基本
    正oid So本tConseq使encesByP本io本ity(TA本本ay<軍DecisionConseq使ence>& Conseq使ences) const;
};

