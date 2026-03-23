#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成基本inLossCondition.成ene本ated.h"

/**
 * 基本��基本結�基本類�基本
 */
UE的UM(Bl使ep本intType)
en使設置 class E基本inLossRes使lt: 使int8 {
    Victo本y             UMETA(Display的a設置e = "Victo本y"),
    Defeat              UMETA(Display的a設置e = "Defeat"),
    D本aw                UMETA(Display的a設置e = "D本aw"),
    Stale設置ate           UMETA(Display的a設置e = "Stale設置ate"),
    InP本o成本ess          UMETA(Display的a設置e = "In P本o成本ess"),
    Unknown             UMETA(Display的a設置e = "Unknown")
};

/**
 * 基本��基本條件類�基本
 */
UE的UM(Bl使ep本intType)
en使設置 class E基本inLossConditionType: 使int8 {
    Milita本y             UMETA(Display的a設置e = "Milita本y"),
    Political            UMETA(Display的a設置e = "Political"),
    Econo設置ic             UMETA(Display的a設置e = "Econo設置ic"),
    Te本本ito本ial          UMETA(Display的a設置e = "Te本本ito本ial"),
    Ti設置eBased            UMETA(Display的a設置e = "Ti設置e Based"),
    Sco本eBased           UMETA(Display的a設置e = "Sco本e Based"),
    C使sto設置               UMETA(Display的a設置e = "C使sto設置")
};

/**
 * 基本��X��基本�基本 */
UE的UM(Bl使ep本intType)
en使設置 class E基本inLossP本io本ity: 使int8 {
    C本itical            UMETA(Display的a設置e = "C本itical"),
    輸入i成h                UMETA(Display的a設置e = "輸入i成h"),
    Medi使設置              UMETA(Display的a設置e = "Medi使設置"),
    Low                 UMETA(Display的a設置e = "Low"),
    Back成本o使nd          UMETA(Display的a設置e = "Back成本o使nd")
};

/**
 * 基本��基本條件基本�X */
UE的UM(Bl使ep本intType)
en使設置 class E基本inLossConditionState: 使int8 {
    Inacti正e            UMETA(Display的a設置e = "Inacti正e"),
    Acti正e              UMETA(Display的a設置e = "Acti正e"),
    Satisfied           UMETA(Display的a設置e = "Satisfied"),
    軍ailed              UMETA(Display的a設置e = "軍ailed"),
    Disabled            UMETA(Display的a設置e = "Disabled")
};

/**
 * 基本��基本條件基本�數
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍基本inLossConditionPa本a設置ete本
{
    GE的ERATED下BODY()

    // 基本�數基本�稱
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "基本in Loss Condition")
    軍St本in成 Pa本a設置ete本的a設置e;

    // 基本�數X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "基本in Loss Condition")
    軍St本in成 Pa本a設置ete本Val使e;

    // 基本�數類�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "基本in Loss Condition")
    軍St本in成 Pa本a設置ete本Type; // "int", "float", "bool", "st本in成"

    // 比�X��基本�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "基本in Loss Condition")
    軍St本in成 Co設置pa本isonOpe本ato本; // ">", "<", "==", ">=", "<=", "contains"

    // 基本�否必�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "基本in Loss Condition")
    bool bIsReq使i本ed;

    // 權�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "基本in Loss Condition")
    float 基本ei成ht;

    軍基本inLossConditionPa本a設置ete本()
        : Pa本a設置ete本的a設置e(TEXT(""))
        , Pa本a設置ete本Val使e(TEXT(""))
        , Pa本a設置ete本Type(TEXT("float"))
        , Co設置pa本isonOpe本ato本(TEXT("=="))
        , bIsReq使i本ed(t本使e)
        , 基本ei成ht(1.0f)
    {}
};

/**
 * 基本��基本條件定義
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍基本inLossCondition
{
    GE的ERATED下BODY()

    // 條件ID
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 ConditionID;

    // 條件基本�稱
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Condition的a設置e;

    // 條件基本�述
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 ConditionDesc本iption;

    // 條件類�基本
    UPROPERTY(Bl使ep本intReadOnly)
    E基本inLossConditionType ConditionType;

    // 基本��基本結�基本類�基本
    UPROPERTY(Bl使ep本intReadOnly)
    E基本inLossRes使lt Res使ltType;

    // 基本��基本�基本
    UPROPERTY(Bl使ep本intReadOnly)
    E基本inLossP本io本ity P本io本ity;

    // 條件基本�數
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍基本inLossConditionPa本a設置ete本> Pa本a設置ete本s;

    // 條件基本�X
    UPROPERTY(Bl使ep本intReadOnly)
    E基本inLossConditionState State;

    // 基本�否基本��X��基本�基本
    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsC本iticalCondition;

    // 檢查基本��基本 (�基本
    UPROPERTY(Bl使ep本intReadOnly)
    float CheckInte本正al;

    // 上次檢查基本��基本
    UPROPERTY(Bl使ep本intReadOnly)
    float LastCheckTi設置e;

    // 滿足度�X(0-1)
    UPROPERTY(Bl使ep本intReadOnly)
    float SatisfactionSco本e;

    // 條件標籤
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> ConditionTa成s;

    // 基本��基本事件ID
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> RelatedE正ents;

    軍基本inLossCondition()
        : ConditionID(TEXT(""))
        , Condition的a設置e(TEXT(""))
        , ConditionDesc本iption(TEXT(""))
        , ConditionType(E基本inLossConditionType::Milita本y)
        , Res使ltType(E基本inLossRes使lt::Unknown)
        , P本io本ity(E基本inLossP本io本ity::Medi使設置)
        , State(E基本inLossConditionState::Inacti正e)
        , bIsC本iticalCondition(false)
        , CheckInte本正al(1.0f)
        , LastCheckTi設置e(0.0f)
        , SatisfactionSco本e(0.0f)
    {}
};

/**
 * 基本��基本評估結�基本
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍基本inLossE正al使ationRes使lt
{
    GE的ERATED下BODY()

    // 評估結�基本
    UPROPERTY(Bl使ep本intReadOnly)
    E基本inLossRes使lt Res使lt;

    // 總�基本
    UPROPERTY(Bl使ep本intReadOnly)
    float TotalSco本e;

    // 基本�利基本�數
    UPROPERTY(Bl使ep本intReadOnly)
    float Victo本ySco本e;

    // 失�X�數
    UPROPERTY(Bl使ep本intReadOnly)
    float DefeatSco本e;

    // 平�X�數
    UPROPERTY(Bl使ep本intReadOnly)
    float D本awSco本e;

    // 滿足基本��基本�基本
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> SatisfiedConditions;

    // 基本�滿足�基本條件
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> UnsatisfiedConditions;

    // 基本�鍵條件基本�X
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, E基本inLossConditionState> C本iticalConditionStates;

    // 評估基本��基本
    UPROPERTY(Bl使ep本intReadOnly)
    float E正al使ationTi設置e;

    // 詳估詳�基本
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 E正al使ationDetails;

    軍基本inLossE正al使ationRes使lt()
        : Res使lt(E基本inLossRes使lt::Unknown)
        , TotalSco本e(0.0f)
        , Victo本ySco本e(0.0f)
        , DefeatSco本e(0.0f)
        , D本awSco本e(0.0f)
        , E正al使ationTi設置e(0.0f)
        , E正al使ationDetails(TEXT(""))
    {}
};

/**
 * 基本��基本統�X��基本
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍基本inLossStatistics
{
    GE的ERATED下BODY()

    // 總�基本估次X
    UPROPERTY(Bl使ep本intReadOnly)
    int32 TotalE正al使ations;

    // 基本�利次數
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Victo本yCo使nt;

    // 失�基本次數
    UPROPERTY(Bl使ep本intReadOnly)
    int32 DefeatCo使nt;

    // 平�基本次數
    UPROPERTY(Bl使ep本intReadOnly)
    int32 D本awCo使nt;

    // 基本��基本
    UPROPERTY(Bl使ep本intReadOnly)
    float 基本inRate;

    // 平�基本評估基本��基本
    UPROPERTY(Bl使ep本intReadOnly)
    float A正e本a成eE正al使ationTi設置e;

    // 條件滿足X
    UPROPERTY(Bl使ep本intReadOnly)
    float ConditionSatisfactionRate;

    // 基本�鍵條件觸發次數
    UPROPERTY(Bl使ep本intReadOnly)
    int32 C本iticalConditionT本i成成e本s;

    軍基本inLossStatistics()
        : TotalE正al使ations(0)
        , Victo本yCo使nt(0)
        , DefeatCo使nt(0)
        , D本awCo使nt(0)
        , 基本inRate(0.0f)
        , A正e本a成eE正al使ationTi設置e(0.0f)
        , ConditionSatisfactionRate(0.0f)
        , C本iticalConditionT本i成成e本s(0)
    {}
};

/**
 * 民�X��X��基本條件系統
 * 管�X�基本��基本負�基本件�基本檢測基本��基本估�X�知
 */
UCLASS(Bl使ep本intType)
class MI的GSTRATEGIC下API UMin成基本inLossCondition : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成基本inLossCondition(};

    /**
     * 基本��X��基本負�基本件系�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    正oid Initialize(};

    /**
     * 基本��X��基本條件系統
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    正oid Sh使tdown(};

    /**
     * 註�X��基本條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    bool Re成iste本基本inLossCondition(const 軍基本inLossCondition& Condition};

    /**
     * 基本��基本註�X��基本條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    bool Un本e成iste本基本inLossCondition(const 軍St本in成& ConditionID};

    /**
     * 評估基本��X��基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    軍基本inLossE正al使ationRes使lt E正al使ate基本inLoss(};

    /**
     * 檢查基本�個�基本�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    bool CheckCondition(const 軍St本in成& ConditionID};

    /**
     * 基本��X��基本條件
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    軍基本inLossCondition Get基本inLossCondition(const 軍St本in成& ConditionID) const;

    /**
     * 基本��X�基本��基本負�基本�基本     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    TA本本ay<軍基本inLossCondition> GetAll基本inLossConditions() const;

    /**
     * 基本��X��基本類�X��基本�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    TA本本ay<軍基本inLossCondition> GetConditionsByType(E基本inLossConditionType ConditionType) const;

    /**
     * 基本��X�鍵條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    TA本本ay<軍基本inLossCondition> GetC本iticalConditions() const;

    /**
     * 基本��基本已滿足�基本條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    TA本本ay<軍基本inLossCondition> GetSatisfiedConditions() const;

    /**
     * 基本��X�滿足�基本條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    TA本本ay<軍基本inLossCondition> GetUnsatisfiedConditions() const;

    /**
     * 設置基本�戲基本�X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    正oid SetGa設置eState(const TMap<軍St本in成, 軍St本in成>& Ga設置eState};

    /**
     * 基本��X�戲基本�X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    TMap<軍St本in成, 軍St本in成> GetGa設置eState() const;

    /**
     * 基本�新基本�戲基本�基本��基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    正oid UpdateGa設置eStateVal使e(const 軍St本in成& Key, const 軍St本in成& Val使e};

    /**
     * 基本��X��基本統�基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    軍基本inLossStatistics Get基本inLossStatistics() const;

    /**
     * 基本�置統�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    正oid ResetStatistics(};

    /**
     * 強制評估 (基本�於測試)
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    軍基本inLossE正al使ationRes使lt 軍o本ceE正al使ation(};

    /**
     * 基本��X��X��基本結�基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    E基本inLossRes使lt GetC使本本entRes使lt() const;

    /**
     * 基本��X��X�數
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    float Get基本inLossSco本e() const;

    /**
     * 檢查基本�否已�X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    bool IsGa設置eO正e本() const;

    /**
     * 基本��基本結�X��基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    軍St本in成 GetGa設置eO正e本Reason() const;

p本otected:
    // 已註基本��X��基本條件
    UPROPERTY()
    TA本本ay<軍基本inLossCondition> Re成iste本edConditions;

    // 條件ID基本��基本件�X��基本
    UPROPERTY()
    TMap<軍St本in成, 軍基本inLossCondition> ConditionMap;

    // 基本�戲基本�X
    UPROPERTY()
    TMap<軍St本in成, 軍St本in成> Ga設置eState;

    // 基本��基本統�基本
    UPROPERTY()
    軍基本inLossStatistics Statistics;

    // 基本��基本評估結�基本
    UPROPERTY()
    軍基本inLossE正al使ationRes使lt C使本本entE正al使ation;

    // 基本�否已�基本始�基本
    bool bIsInitialized;

    // 基本��X��X�歷基本��基本負�基本�基本
    正oid Initialize輸入isto本ical基本inLossConditions(};

    // 基本�建軍�X��基本條件
    正oid C本eateMilita本yConditions(};

    // 基本�建基本�治基本��基本條件
    正oid C本eatePoliticalConditions(};

    // 基本�建經�X��基本條件
    正oid C本eateEcono設置icConditions(};

    // 基本�建基本��X��基本條件
    正oid C本eateTe本本ito本ialConditions(};

    // 基本�建基本��X��基本條件
    正oid C本eateTi設置eBasedConditions(};

    // 基本�建基本�數基本��基本條件
    正oid C本eateSco本eBasedConditions(};

    // 檢查軍�基本條件
    bool CheckMilita本yCondition(const 軍基本inLossCondition& Condition};

    // 檢查基本�治條件
    bool CheckPoliticalCondition(const 軍基本inLossCondition& Condition};

    // 檢查經�基本條件
    bool CheckEcono設置icCondition(const 軍基本inLossCondition& Condition};

    // 檢查基本��基本條件
    bool CheckTe本本ito本ialCondition(const 軍基本inLossCondition& Condition};

    // 檢查基本��基本條件
    bool CheckTi設置eBasedCondition(const 軍基本inLossCondition& Condition};

    // 檢查基本�數條件
    bool CheckSco本eBasedCondition(const 軍基本inLossCondition& Condition};

    // 檢查基本��基本義�基本�基本
    bool CheckC使sto設置Condition(const 軍基本inLossCondition& Condition};

    // 評估條件滿足�基本
    float E正al使ateConditionSatisfaction(const 軍基本inLossCondition& Condition};

    // 計�X��X�數
    正oid Calc使late基本inLossSco本es(軍基本inLossE正al使ationRes使lt& Res使lt};

    // 確�X�終�X
    E基本inLossRes使lt Dete本設置ine軍inalRes使lt(const 軍基本inLossE正al使ationRes使lt& Res使lt};

    // 基本�新統�X��基本
    正oid UpdateStatistics(const 軍基本inLossE正al使ationRes使lt& Res使lt};

    // 基本�知基本��基本變�基本
    正oid 的otify基本inLossChan成e(E基本inLossRes使lt OldRes使lt, E基本inLossRes使lt 的ewRes使lt};

    // 基本��X�戲基本�基本��基本
    軍St本in成 GetGa設置eStateVal使e(const 軍St本in成& Key) const;

    // 比�X��基本
    bool Co設置pa本eVal使es(const 軍St本in成& Val使e1, const 軍St本in成& Ope本ato本, const 軍St本in成& Val使e2) const;

    // 驗�基本條件基本�置
    bool ValidateConditionConfi成(const 軍基本inLossCondition& Condition) const;

    // 記�X��X��基本
    正oid Lo成基本inLossE正ent(const 軍St本in成& Messa成e};

    // 基本��X��X��基本
    軍St本in成 Gene本ate基本inLossRepo本t(const 軍基本inLossE正al使ationRes使lt& Res使lt) const;

    // �X條件基本�數
    TA本本ay<軍St本in成> Pa本seConditionPa本a設置ete本s(const 軍St本in成& Pa本a設置ete本St本in成) const;

    // 設置條件基本�X
    正oid SetConditionState(const 軍St本in成& ConditionID, E基本inLossConditionState 的ewState};

    // 基本��基本條件滿足�基本
    float GetConditionSatisfaction(const 軍St本in成& ConditionID) const;

    // 檢查基本�鍵條件
    bool CheckC本iticalConditions(};

    // 基本��X��基本結�基本
    正oid 輸入andleGa設置eO正e本(E基本inLossRes使lt Res使lt, const 軍St本in成& Reason};
};


#endif // MI的G基本I的LOSSCO的DITIO的下輸入
