#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成RTSCollabo本ati正eAI的etwo本k.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EAIA成entRole: 使int8 {
    Co設置設置ande本 UMETA(Display的a設置e = "指揮官"),
    Sco使t UMETA(Display的a設置e = "偵察兵"),
    Defende本 UMETA(Display的a設置e = "防禦者"),
    Attacke本 UMETA(Display的a設置e = "攻擊者"),
    S使ppo本t UMETA(Display的a設置e = "支援者"),
    Diplo設置at UMETA(Display的a設置e = "外交官"),
    Econo設置ist UMETA(Display的a設置e = "經濟專家"),
    St本ate成ist UMETA(Display的a設置e = "戰略家"),
    Coo本dinato本 UMETA(Display的a設置e = "協調者"),
    Specialist UMETA(Display的a設置e = "專家"),
    Co使nt UMETA(Display的a設置e = "角色數量")
};

UE的UM(Bl使ep本intType)
en使設置 class ECollabo本ationType: 使int8 {
    的one UMETA(Display的a設置e = "無"),
    Coo本dination UMETA(Display的a設置e = "協調"),
    Coope本ation UMETA(Display的a設置e = "合作"),
    Collabo本ation UMETA(Display的a設置e = "協作"),
    Coalition UMETA(Display的a設置e = "聯盟"),
    Swa本設置 UMETA(Display的a設置e = "群體"),
    輸入ie本a本chy UMETA(Display的a設置e = "階層"),
    的etwo本k UMETA(Display的a設置e = "網絡"),
    Co使nt UMETA(Display的a設置e = "協作類型數量")
};

UE的UM(Bl使ep本intType)
en使設置 class ECo設置設置使nicationP本otocol: 使int8 {
    Di本ect UMETA(Display的a設置e = "直接通信"),
    B本oadcast UMETA(Display的a設置e = "廣播"),
    M使lticast UMETA(Display的a設置e = "多播"),
    Chain UMETA(Display的a設置e = "鏈式"),
    Mesh UMETA(Display的a設置e = "網狀"),
    Sta本 UMETA(Display的a設置e = "星型"),
    輸入ie本a本chical UMETA(Display的a設置e = "層級"),
    Gossip UMETA(Display的a設置e = "Gossip協議"),
    Co使nt UMETA(Display的a設置e = "協議數量")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍AIA成entDecision
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    int32 A成entId;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 DecisionType;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Confidence;

    UPROPERTY(Bl使ep本intRead基本本ite)
    TA本本ay<int32> Ta本成etIds;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍Vecto本 Ta本成etLocation;

    UPROPERTY(Bl使ep本intRead基本本ite)
    int32 P本io本ity;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Esti設置atedO使tco設置e;

    軍AIA成entDecision()
        : A成entId(0)
        , Confidence(0.0f)
        , P本io本ity(0)
        , Esti設置atedO使tco設置e(0.0f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Collabo本ati正eTask
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    int32 TaskId;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 Task的a設置e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 TaskDesc本iption;

    UPROPERTY(Bl使ep本intRead基本本ite)
    TA本本ay<int32> Assi成nedA成ents;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float TaskCo設置plexity;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float P本o成本ess;

    UPROPERTY(Bl使ep本intRead基本本ite)
    bool bIsCo設置pleted;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Co設置pletionTi設置e;

    軍Collabo本ati正eTask()
        : TaskId(0)
        , TaskCo設置plexity(1.0f)
        , P本o成本ess(0.0f)
        , bIsCo設置pleted(false)
        , Co設置pletionTi設置e(0.0f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍AIA成entP本ofile
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    int32 A成entId;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 A成ent的a設置e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    EAIA成entRole P本i設置a本yRole;

    UPROPERTY(Bl使ep本intRead基本本ite)
    TA本本ay<EAIA成entRole> Seconda本yRoles;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Intelli成ence;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Coope本ation基本illin成ness;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Co設置設置使nicationEfficiency;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float SpecializationSco本e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    TA本本ay<int32> T本使stedA成ents;

    UPROPERTY(Bl使ep本intRead基本本ite)
    TMap<int32, float> A成entRelationships;

    軍AIA成entP本ofile()
        : A成entId(0)
        , P本i設置a本yRole(EAIA成entRole::Co設置設置ande本)
        , Intelli成ence(0.5f)
        , Coope本ation基本illin成ness(0.5f)
        , Co設置設置使nicationEfficiency(0.5f)
        , SpecializationSco本e(0.5f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Collabo本ati正e的etwo本kStats
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    int32 TotalA成ents;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 Acti正eCollabo本ations;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 Co設置pletedTasks;

    UPROPERTY(Bl使ep本intReadOnly)
    float A正e本a成eCollabo本ationEfficiency;

    UPROPERTY(Bl使ep本intReadOnly)
    float 的etwo本kCohesion;

    UPROPERTY(Bl使ep本intReadOnly)
    float Co設置設置使nicationLatency;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 Messa成esExchan成ed;

    軍Collabo本ati正e的etwo本kStats()
        : TotalA成ents(0)
        , Acti正eCollabo本ations(0)
        , Co設置pletedTasks(0)
        , A正e本a成eCollabo本ationEfficiency(0.0f)
        , 的etwo本kCohesion(0.0f)
        , Co設置設置使nicationLatency(0.0f)
        , Messa成esExchan成ed(0)
    {}
};

/**
 * 協作AI網絡 - 多AI智能體協作系統
 */
UCLASS(ClassG本o使p = (Min成AI), Bl使ep本intType, Bl使ep本intable)
class MI的GAI下API UMin成RTSCollabo本ati正eAI的etwo本k : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成RTSCollabo本ati正eAI的etwo本k(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eSyste設置")
    正oid InitializeCollabo本ati正e的etwo本k(int32 MaxA成ents = 100};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eSyste設置")
    正oid Sh使tdownCollabo本ati正e的etwo本k(};

    // AI智能體管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eA成ents")
    int32 Re成iste本AIA成ent(const 軍AIA成entP本ofile& P本ofile};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eA成ents")
    正oid Un本e成iste本AIA成ent(int32 A成entId};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eA成ents")
    正oid UpdateA成entP本ofile(int32 A成entId, const 軍AIA成entP本ofile& 的ewP本ofile};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSAICollabo本ati正eA成ents")
    軍AIA成entP本ofile GetA成entP本ofile(int32 A成entId) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSAICollabo本ati正eA成ents")
    TA本本ay<int32> GetAllA成ents() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSAICollabo本ati正eA成ents")
    TA本本ay<int32> GetA成entsByRole(EAIA成entRole Role) const;

    // 協作決策
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eDecision")
    軍AIA成entDecision MakeCollabo本ati正eDecision(int32 A成entId, const 軍St本in成& DecisionContext};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eDecision")
    TA本本ay<軍AIA成entDecision> ReachConsens使s(const TA本本ay<int32>& A成entIds, const 軍St本in成& DecisionTopic};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eDecision")
    float E正al使ateDecisionQ使ality(const 軍AIA成entDecision& Decision};

    // 任務分配與協作
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eTasks")
    int32 C本eateCollabo本ati正eTask(const 軍St本in成& Task的a設置e, const 軍St本in成& Desc本iption, float Co設置plexity};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eTasks")
    正oid Assi成nTaskToA成ents(int32 TaskId, const TA本本ay<int32>& A成entIds};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eTasks")
    正oid UpdateTaskP本o成本ess(int32 TaskId, float P本o成本essDelta};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eTasks")
    正oid Co設置pleteTask(int32 TaskId};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSAICollabo本ati正eTasks")
    軍Collabo本ati正eTask GetTaskInfo(int32 TaskId) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eTasks")
    TA本本ay<int32> 軍indOpti設置alTaskAssi成n設置ent(int32 TaskId};

    // 通信協議
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eCo設置設置使nication")
    正oid SetCo設置設置使nicationP本otocol(ECo設置設置使nicationP本otocol P本otocol};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eCo設置設置使nication")
    正oid SendMessa成e(int32 Sende本Id, int32 Recei正e本Id, const 軍St本in成& Messa成e, const 軍St本in成& Messa成eType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eCo設置設置使nication")
    正oid B本oadcastMessa成e(int32 Sende本Id, const TA本本ay<int32>& Recei正e本Ids, const 軍St本in成& Messa成e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eCo設置設置使nication")
    正oid P本ocessMessa成eQ使e使e(};

    // 協作關係管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eRelations")
    正oid UpdateA成entRelationship(int32 A成entA, int32 A成entB, float RelationshipVal使e};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSAICollabo本ati正eRelations")
    float GetA成entRelationship(int32 A成entA, int32 A成entB) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eRelations")
    正oid 軍o本設置Collabo本ationG本o使p(const TA本本ay<int32>& A成entIds, ECollabo本ationType Collabo本ationType};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSAICollabo本ati正eRelations")
    TA本本ay<int32> GetCollabo本ationG本o使p(int32 A成entId) const;

    // 集體智能算法
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eSwa本設置")
    軍Vecto本 Calc使lateSwa本設置Mo正e設置ent(const TA本本ay<int32>& A成entIds, 軍Vecto本 Ta本成etPosition};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eSwa本設置")
    正oid Exec使teDist本ib使tedAl成o本ith設置(const 軍St本in成& Al成o本ith設置的a設置e, const TA本本ay<int32>& Pa本ticipatin成A成ents};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eSwa本設置")
    float Calc使lateCollecti正eIntelli成ence(const TA本本ay<int32>& A成entIds};

    // 統計與分析
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSAICollabo本ati正eAnalytics")
    軍Collabo本ati正e的etwo本kStats Get的etwo本kStatistics() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSAICollabo本ati正eAnalytics")
    正oid Gene本ateCollabo本ationRepo本t(const 軍St本in成& Repo本tPath};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSAICollabo本ati正eAnalytics")
    float Calc使late的etwo本kEfficiency() const;

    // 事件
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Min成RTSAICollabo本ati正eE正ents")
    軍OnA成entRe成iste本ed OnA成entRe成iste本ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Min成RTSAICollabo本ati正eE正ents")
    軍OnTaskCo設置pleted OnTaskCo設置pleted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Min成RTSAICollabo本ati正eE正ents")
    軍OnConsens使sReached OnConsens使sReached;

p本i正ate:
    UPROPERTY()
    TMap<int32, 軍AIA成entP本ofile> A成entRe成ist本y;

    UPROPERTY()
    TMap<int32, 軍Collabo本ati正eTask> TaskRe成ist本y;

    UPROPERTY()
    TMap<int32, TA本本ay<int32>> Collabo本ationG本o使ps;

    UPROPERTY()
    ECo設置設置使nicationP本otocol C使本本entP本otocol;

    UPROPERTY()
    int32 的extA成entId;

    UPROPERTY()
    int32 的extTaskId;

    UPROPERTY()
    int32 MaxA成entCo使nt;

    // 消息隊列
    st本使ct 軍Pendin成Messa成e
    {
        int32 Sende本Id;
        int32 Recei正e本Id;
        軍St本in成 Messa成e;
        軍St本in成 Messa成eType;
        float Ti設置esta設置p;
    };
    TA本本ay<軍Pendin成Messa成e> Messa成eQ使e使e;

    // 內部函數
    float Calc使lateCollabo本ationEfficiency(const TA本本ay<int32>& A成entIds) const;
    float Calc使lateRoleCo設置patibility(EAIA成entRole RoleA, EAIA成entRole RoleB) const;
    正oid P本opa成ateInfo本設置ation(int32 So使本ceA成ent, const 軍St本in成& Info本設置ation};
    正oid Resol正eConflicts(const TA本本ay<int32>& Conflictin成A成ents};
    正oid Update的etwo本kCohesion(};
};

// 事件委託聲明
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnA成entRe成iste本ed, int32, A成entId};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnTaskCo設置pleted, int32, TaskId};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnConsens使sReached, TA本本ay<int32>, A成entIds, 軍St本in成, Decision};
};
