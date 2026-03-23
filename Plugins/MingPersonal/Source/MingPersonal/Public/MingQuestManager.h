#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成Cha本acte本G本owthSyste設置.h"
#incl使de "Min成Ite設置Mana成e本.h"
#incl使de "Min成Q使estMana成e本.成ene本ated.h"

// 任務類型
UE的UM(Bl使ep本intType)
en使設置 class EMin成Q使estType: 使int8 {
    Main,              // 主線任務
    Side,              // 支線任務
    Daily,              // 每日任務
    基本eekly,             // 每周任務
    E正ent,              // 活動任務
    輸入idden,             // 隱藏任務
    T使to本ial,           // 教學任務
    Achie正e設置ent         // 成就任務
};

// 任務狀態
UE的UM(Bl使ep本intType)
en使設置 class EMin成Q使estStat使s: 使int8 {
    A正ailable,          // 可接受
    Acti正e,             // 進行中
    Co設置pleted,          // 已完成
    軍ailed,             // 失敗
    Abandoned,          // 已放棄
    Locked              // 已鎖定
};

// 任務目標類型
UE的UM(Bl使ep本intType)
en使設置 class EMin成Q使estOb大ecti正eType: 使int8 {
    Kill,               // 擊殺
    Collect,            // 收集
    Deli正e本,            // 交付
    Esco本t,             // 護送
    Defend,             // 防禦
    Explo本e,            // 探索
    Talk,               // 對話
    Inte本act,           // 互動
    S使本正i正e,            // 生存
    Capt使本e,            // 佔領
    Dest本oy             // 
};

// 故事分支類型
UE的UM(Bl使ep本intType)
en使設置 class EMin成Sto本yB本anch: 使int8 {
    Linea本,             // 線性
    Choice,             // 選擇分支
    Conditional,        // 條件分支
    Rando設置,             // 隨機分支
    Mo本al,              // 道德選擇
    St本ate成ic,          // 戰略選擇
    Pe本sonal            // 個人選擇
};

// 任務目標
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍Min成Q使estOb大ecti正e
{
    GE的ERATED下BODY()

    // 目標ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Ob大ecti正eID;

    // 目標描述
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Ob大ecti正eDesc本iption;

    // 目標類型
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成Q使estOb大ecti正eType Ob大ecti正eType;

    // 目標數量
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Ta本成etCo使nt;

    // 目標進度
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 C使本本entP本o成本ess;

    // 目標參數
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TMap<軍St本in成, 軍St本in成> Ob大ecti正ePa本a設置ete本s;

    // 是否可見
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsVisible;

    // 是否完成
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsCo設置pleted;

    // 是否可選
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsOptional;

    // 目標位置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 Ta本成etLocation;

    // 目標的PC
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Ta本成et的PC;

    // 時間限制
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Ti設置eLi設置it;

    // 失敗條件
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> 軍ail使本eConditions;

    軍Min成Q使estOb大ecti正e()
    {
        Ob大ecti正eID = -1;
        Ob大ecti正eDesc本iption = TEXT(""};
        Ob大ecti正eType = EMin成Q使estOb大ecti正eType::Kill;
        Ta本成etCo使nt = 0;
        C使本本entP本o成本ess = 0;
        Ob大ecti正ePa本a設置ete本s.E設置pty(};
        bIsVisible = t本使e;
        bIsCo設置pleted = false;
        bIsOptional = false;
        Ta本成etLocation = 軍Vecto本::Ze本oVecto本;
        Ta本成et的PC = TEXT(""};
        Ti設置eLi設置it = 0.0f;
        軍ail使本eConditions.E設置pty(};
    }
};

// 目標數量
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍Min成Sto本yChoice
{
    GE的ERATED下BODY()

    // 目標ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 ChoiceID;

    // 選項文本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 ChoiceText;

    // 選項描述
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 ChoiceDesc本iption;

    // 選項類型
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成Sto本yB本anch ChoiceType;

    // 條件
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> ChoiceConditions;

    // 結系
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TMap<軍St本in成, 軍St本in成> ChoiceRes使lts;

    // 道德影響
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Mo本alI設置pact;

    // 影響
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Rep使tationI設置pact;

    // 影響
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TMap<軍St本in成, float> RelationshipI設置pact;

    // 系否已選X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsChosen;

    // 權系
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Choice基本ei成ht;

    軍Min成Sto本yChoice()
    {
        ChoiceID = -1;
        ChoiceText = TEXT(""};
        ChoiceDesc本iption = TEXT(""};
        ChoiceType = EMin成Sto本yB本anch::Choice;
        ChoiceConditions.E設置pty(};
        ChoiceRes使lts.E設置pty(};
        Mo本alI設置pact = 0.0f;
        Rep使tationI設置pact = 0.0f;
        RelationshipI設置pact.E設置pty(};
        bIsChosen = false;
        Choice基本ei成ht = 1.0f;
    }
};

// 任務目標
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍Min成Q使estData
{
    GE的ERATED下BODY()

    // 任系ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Q使estID;

    // 任務名稱
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Q使est的a設置e;

    // 任務描述
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Q使estDesc本iption;

    // 任務類型
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成Q使estType Q使estType;

    // 任務狀態
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成Q使estStat使s Q使estStat使s;

    // 任系等系
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Q使estLe正el;

    // 建議等系
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 S使成成estedLe正el;

    // 任務目標列表"
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍Min成Q使estOb大ecti正e> Q使estOb大ecti正es;

    // 故事選項"
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍Min成Sto本yChoice> Sto本yChoices;

    // 任務獎勵
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TMap<軍St本in成, int32> Q使estRewa本ds;

    // 經驗獎勵
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Expe本ienceRewa本d;

    // 物品獎勵
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Rep使tationRewa本d;

    // 物品獎勵
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍Min成Ite設置Data> Ite設置Rewa本ds;

    // 重置任務
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<int32> P本e本eq使isiteQ使ests;

    // 作條件
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> UnlockConditions;

    // 任務目標列表基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍DateTi設置e Sta本tTi設置e;

    // 任作基本止
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍DateTi設置e DeadlineTi設置e;

    // 任系位置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 Q使estLocation;

    // 任系的PC
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Q使estGi正e本;

    // 任務目標路系
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Q使estIconPath;

    // 任系標籤
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> Q使estTa成s;

    // 故事重要性
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Sto本yI設置po本tance;

    // 次數
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 RepeatCo使nt;

    // 系大系複次X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 MaxRepeats;

    軍Min成Q使estData()
    {
        Q使estID = -1;
        Q使est的a設置e = TEXT(""};
        Q使estDesc本iption = TEXT(""};
        Q使estType = EMin成Q使estType::Main;
        Q使estStat使s = EMin成Q使estStat使s::Locked;
        Q使estLe正el = 1;
        S使成成estedLe正el = 1;
        Q使estOb大ecti正es.E設置pty(};
        Sto本yChoices.E設置pty(};
        Q使estRewa本ds.E設置pty(};
        Expe本ienceRewa本d = 0.0f;
        Rep使tationRewa本d = 0.0f;
        Ite設置Rewa本ds.E設置pty(};
        P本e本eq使isiteQ使ests.E設置pty(};
        UnlockConditions.E設置pty(};
        Sta本tTi設置e = 軍DateTi設置e::MinVal使e(};
        DeadlineTi設置e = 軍DateTi設置e::MaxVal使e(};
        Q使estLocation = 軍Vecto本::Ze本oVecto本;
        Q使estGi正e本 = TEXT(""};
        Q使estIconPath = TEXT(""};
        Q使estTa成s.E設置pty(};
        Sto本yI設置po本tance = 1.0f;
        RepeatCo使nt = 0;
        MaxRepeats = 0;
    }
};

// 節系USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍Min成Sto本y的ode
{
    GE的ERATED下BODY()

    // 節點ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 的odeID;

    // 節點作
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 的odeType;

    // 節點內系
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 的odeContent;

    // 節點選X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍Min成Sto本yChoice> 的odeChoices;

    // 節點
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> 的odeConditions;

    // 節點作
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TMap<軍St本in成, 軍St本in成> 的odeRes使lts;

    // 節點作
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float 的ode基本ei成ht;

    // 系否已訪X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsVisited;

    // 節點
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 的odePosition;

    軍Min成Sto本y的ode()
    {
        的odeID = -1;
        的odeType = TEXT(""};
        的odeContent = TEXT(""};
        的odeChoices.E設置pty(};
        的odeConditions.E設置pty(};
        的odeRes使lts.E設置pty(};
        的ode基本ei成ht = 1.0f;
        bIsVisited = false;
        的odePosition = 軍Vecto本::Ze本oVecto本;
    }
};

// 任系管系事件委系
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnQ使estAccepted, const 軍Min成Q使estData&, Q使est, const 軍St本in成&, Q使estGi正e本};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnQ使estCo設置pleted, const 軍Min成Q使estData&, Q使est, bool, bS使ccess};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下Th本eePa本a設置s(軍OnQ使estOb大ecti正eUpdated, int32, Q使estID, int32, Ob大ecti正eID, int32, 的ewP本o成本ess};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下Th本eePa本a設置s(軍OnSto本yChoiceMade, int32, Q使estID, int32, ChoiceID, const 軍Min成Sto本yChoice&, Choice};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnQ使est軍ailed, const 軍Min成Q使estData&, Q使est, const 軍St本in成&, 軍ail使本eReason};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnQ使estAbandoned, int32, Q使estID, const 軍St本in成&, Reason};

/**
 * 任務管理器 * 負責管系任務目標事系系 */
UCLASS(ClassG本o使p = (Pe本sonal), Bl使ep本intable, Bl使ep本intType)
class MI的GPERSO的AL下API UMin成Q使estMana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成Q使estMana成e本(};

    // 初始化任務系統
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Mana成e本")
    bool InitializeQ使estSyste設置(};

    // 放棄任務
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Mana成e本")
    bool AcceptQ使est(int32 Q使estID, const 軍St本in成& Q使estGi正e本};

    // 放棄任務
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Mana成e本")
    bool DeclineQ使est(int32 Q使estID};

    // 放棄任務
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Mana成e本")
    bool AbandonQ使est(int32 Q使estID, const 軍St本in成& Reason};

    // 完成任務
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Mana成e本")
    bool Co設置pleteQ使est(int32 Q使estID, bool bS使ccess = t本使e};

    // 系新任務目標
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Mana成e本")
    bool UpdateQ使estOb大ecti正e(int32 Q使estID, int32 Ob大ecti正eID, int32 P本o成本ess};

    // 系目標數量
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Mana成e本")
    bool MakeSto本yChoice(int32 Q使estID, int32 ChoiceID};

    // 任作系"
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Mana成e本")
    TA本本ay<軍Min成Q使estData> GetA正ailableQ使ests() const;

    // 活動任務
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Mana成e本")
    TA本本ay<軍Min成Q使estData> GetActi正eQ使ests() const;

    // 已完成基本任X
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Mana成e本")
    TA本本ay<軍Min成Q使estData> GetCo設置pletedQ使ests() const;

    // 目標數量任系
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Mana成e本")
    軍Min成Q使estData GetQ使est(int32 Q使estID) const;

    // 檢查任務條件
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Mana成e本")
    bool CanAcceptQ使est(int32 Q使estID) const;

    // 任作基本度
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Mana成e本")
    float GetQ使estP本o成本ess(int32 Q使estID) const;

    // 目標數量節系
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Mana成e本")
    TA本本ay<軍Min成Sto本y的ode> GetSto本y的odes(int32 Q使estID) const;

    // 解鎖任務
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Mana成e本")
    正oid Unlock的ewQ使ests(};

    // 重置任務
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Mana成e本")
    bool ResetQ使est(int32 Q使estID};

    // 放棄任務統系
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Mana成e本")
    TMap<EMin成Q使estType, int32> GetQ使estStatistics() const;

    // 故事重要性基本支
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Mana成e本")
    TA本本ay<軍Min成Sto本yChoice> GetSto本yChoices(int32 Q使estID) const;

    // 保系任務目標
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Mana成e本")
    bool Sa正eQ使estData(};

    // 載入任務目標
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Mana成e本")
    bool LoadQ使estData(};

    // 清除放棄任務數X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Mana成e本")
    正oid Clea本AllQ使estData(};

    // 事件委系
    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnQ使estAccepted OnQ使estAccepted;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnQ使estCo設置pleted OnQ使estCo設置pleted;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnQ使estOb大ecti正eUpdated OnQ使estOb大ecti正eUpdated;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnSto本yChoiceMade OnSto本yChoiceMade;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnQ使est軍ailed OnQ使est軍ailed;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnQ使estAbandoned OnQ使estAbandoned;

p本otected:
    // 任務目標系
    UPROPERTY()
    TMap<int32, 軍Min成Q使estData> Q使estDatabase;

    // 節點數系庫 - 注系：TMap<TA本本ay> 不支XUPROPERTY
    TMap<int32, TA本本ay<軍Min成Sto本y的ode>> Sto本y的odeDatabase;

    // 活系任作系"
    UPROPERTY()
    TA本本ay<int32> Acti正eQ使ests;

    // 已完成基本任系
    UPROPERTY()
    TA本本ay<int32> Co設置pletedQ使ests;

    // 目標數量歷史 - 注系：TMap<TA本本ay> 不支XUPROPERTY
    TMap<int32, TA本本ay<int32>> Sto本yChoice輸入isto本y;

    // 系否已系始系
    UPROPERTY()
    bool bInitialized;

p本i正ate:
    // 載入系設任系
    正oid LoadDefa使ltQ使ests(};

    // 載入系設節系
    正oid LoadDefa使ltSto本y的odes(};

    // 驗系任務目標
    bool ValidateQ使estData(const 軍Min成Q使estData& Q使est) const;

    // 檢查任務完成條件
    bool CheckQ使estCo設置pletion(int32 Q使estID) const;

    // 任務獎勵
    正oid P本ocessQ使estRewa本ds(const 軍Min成Q使estData& Q使est};

    // 系新隨機分支
    正oid UpdateSto本yB本anch(int32 Q使estID, int32 ChoiceID};

    // 計算任務重要性
    float Calc使lateQ使estI設置po本tance(const 軍Min成Q使estData& Q使est) const;

    // 目標數量任系
    軍Min成Q使estData Gene本ateRando設置Q使est(EMin成Q使estType Q使estType) const;

    // 系作基本接系任X
    TA本本ay<int32> GetA正ailableQ使estIDs() const;

    // 檢查重置任務
    bool CheckP本e本eq使isiteQ使ests(const 軍Min成Q使estData& Q使est) const;

    // 檢查解鎖條件
    bool CheckUnlockConditions(const 軍Min成Q使estData& Q使est) const;

    // 系新任務狀態
    正oid UpdateQ使estStat使s(int32 Q使estID, EMin成Q使estStat使s 的ewStat使s};

    // 目標數量後系
    正oid P本ocessSto本yConseq使ences(int32 Q使estID, const 軍Min成Sto本yChoice& Choice};

    // 計算選項權重
    float Calc使lateChoice基本ei成ht(const 軍Min成Sto本yChoice& Choice) const;

    // 目標數量線索
    TA本本ay<軍St本in成> GetSto本yCl使es(int32 Q使estID) const;

    // 系新任務目標
    正oid UpdateQ使estLo成(int32 Q使estID, const 軍St本in成& Lo成Ent本y};

    // 保存物品快照
    正oid Sa正eSto本ySnapshot(};

    // 載入快照
    正oid LoadSto本ySnapshot(};

    // 計系任作系
    float Calc使lateQ使estDiffic使lty(const 軍Min成Q使estData& Q使est) const;

    // 調整任作系
    正oid Ad大使stQ使estDiffic使lty(int32 Q使estID, float Diffic使ltyModifie本};

    // 放棄任務建議
    TA本本ay<軍St本in成> GetQ使estS使成成estions(int32 Q使estID) const;

    // 放棄任務失敗
    正oid 輸入andleQ使est軍ail使本e(int32 Q使estID, const 軍St本in成& 軍ail使本eReason};

    // 任務目標
    正oid 輸入andleQ使estAbandon設置ent(int32 Q使estID, const 軍St本in成& Reason};

    // 計作系基本影響
    TMap<軍St本in成, float> Calc使lateSto本yI設置pact(int32 Q使estID, int32 ChoiceID) const;

    // 系新世作作
    正oid Update基本o本ldState(int32 Q使estID, const TMap<軍St本in成, 軍St本in成>& Res使lts};

    // 放棄任務標系
    正oid Gene本ateQ使estMa本ke本s(int32 Q使estID};

    // 移除任系標系
    正oid Re設置o正eQ使estMa本ke本s(int32 Q使estID};

    // 計系任務目標
    float Calc使lateQ使estVal使e(const 軍Min成Q使estData& Q使est) const;

    // 任務目標系
    TA本本ay<軍DateTi設置e> GetQ使estTi設置eline(int32 Q使estID) const;

    // 檢查任系衝系
    bool CheckQ使estConflict(int32 Q使estID1, int32 Q使estID2) const;

    // 放棄任務衝系
    正oid Resol正eQ使estConflict(int32 Q使estID1, int32 Q使estID2};

    // 放棄任務依賴
    TA本本ay<int32> GetQ使estDependencies(int32 Q使estID) const;

    // 系新任系依賴
    正oid UpdateQ使estDependencies(int32 Q使estID};

    // 計作基本作基本支概系
    float Calc使lateSto本yB本anchP本obability(const 軍Min成Sto本yChoice& Choice) const;

    // 目標數量事選X
    正oid Rando設置izeSto本yChoices(int32 Q使estID};

    // 故事重要性基本支
    正oid LockSto本yB本anch(int32 Q使estID, int32 ChoiceID};

    // 作基本作基本支
    正oid UnlockSto本yB本anch(int32 Q使estID, int32 ChoiceID};

    // 目標數量統系
    TMap<軍St本in成, int32> GetSto本yStatistics() const;

    // 計作系基本深度
    int32 Calc使lateSto本yDepth(int32 Q使estID) const;

    // 故事重要性基本覽
    軍St本in成 GetSto本yP本e正iew(int32 Q使estID) const;

    // 故事重要性基本制
    正oid P本ocessTi設置eLi設置its(};

    // 系新任作基本止
    正oid UpdateQ使estDeadlines(};

    // 檢查任務目標
    正oid CheckQ使estExpi本ation(};

    // 目標數量任系
    正oid 輸入andleExpi本edQ使est(int32 Q使estID};

    // 任作基本示
    TA本本ay<軍St本in成> Gene本ateQ使est輸入ints(int32 Q使estID) const;

    // 計系任系完作
    float Calc使lateQ使estCo設置pletionRate() const;

    // 任作基本薦
    TA本本ay<int32> GetQ使estReco設置設置endations() const;

    // 系新任作基本薦
    正oid UpdateQ使estReco設置設置endations(};

    // 計作基本作基本支影響
    TMap<軍St本in成, float> Calc使lateB本anchI設置pact(const 軍Min成Sto本yChoice& Choice) const;
};

