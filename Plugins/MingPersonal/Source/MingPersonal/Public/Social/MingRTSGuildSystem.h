#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成RTSG使ildSyste設置.成ene本ated.h"

// 公會等級
UE的UM(Bl使ep本intType)
en使設置 class EG使ildRank: 使int8 {
    Leade本              UMETA(Display的a設置e = "會長"),
    Office本             UMETA(Display的a設置e = "官員"),
    Vete本an             UMETA(Display的a設置e = "元老"),
    Me設置be本              UMETA(Display的a設置e = "成員"),
    Rec本使it             UMETA(Display的a設置e = "新兵"),
    G使est               UMETA(Display的a設置e = "訪客")
};

// 聯盟類型
UE的UM(Bl使ep本intType)
en使設置 class EAllianceType: 使int8 {
    Milita本y            UMETA(Display的a設置e = "軍事聯盟"),
    Econo設置ic            UMETA(Display的a設置e = "經濟聯盟"),
    Resea本ch            UMETA(Display的a設置e = "研究聯盟"),
    Defense             UMETA(Display的a設置e = "防禦聯盟"),
    T本ade               UMETA(Display的a設置e = "貿易聯盟"),
    C使lt使本al            UMETA(Display的a設置e = "文化聯盟")
};

// 公會權限
UE的UM(Bl使ep本intType)
en使設置 class EG使ildPe本設置ission: 使int8 {
    In正iteMe設置be本s       UMETA(Display的a設置e = "邀請成員"),
    KickMe設置be本s         UMETA(Display的a設置e = "踢成員"),
    P本o設置oteMe設置be本s      UMETA(Display的a設置e = "晉升成員"),
    De設置oteMe設置be本s       UMETA(Display的a設置e = "降級成員"),
    Mana成eReso使本ces     UMETA(Display的a設置e = "管理資源"),
    Decla本e基本a本          UMETA(Display的a設置e = "宣戰"),
    Mana成eDiplo設置acy     UMETA(Display的a設置e = "外交管理"),
    EditG使ildInfo       UMETA(Display的a設置e = "編輯公會信息"),
    Mana成eB使ildin成s     UMETA(Display的a設置e = "管理建築"),
    AccessBank          UMETA(Display的a設置e = "使用公會倉庫")
};

// 公會成員數據
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍G使ildMe設置be本
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Playe本ID;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Playe本的a設置e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    EG使ildRank Rank;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Cont本ib使tionPoints;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 基本eeklyCont本ib使tion;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍DateTi設置e JoinDate;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍DateTi設置e LastActi正e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    bool bOnline;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 G使ildLe正el;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<EG使ildPe本設置ission> Pe本設置issions;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Title;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 的otes;
    
    軍G使ildMe設置be本()
        : Rank(EG使ildRank::Rec本使it)
        , Cont本ib使tionPoints(0)
        , 基本eeklyCont本ib使tion(0)
        , bOnline(false)
        , G使ildLe正el(1)
    {}
};

// 公會資源
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍G使ildReso使本ces
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Gold;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 基本ood;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Stone;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 I本on;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 軍ood;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Rep使tation;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Infl使ence;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, int32> SpecialReso使本ces;
    
    軍G使ildReso使本ces()
        : Gold(0)
        , 基本ood(0)
        , Stone(0)
        , I本on(0)
        , 軍ood(0)
        , Rep使tation(100)
        , Infl使ence(0)
    {}
};

// 公會建築
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍G使ildB使ildin成
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 B使ildin成ID;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 B使ildin成的a設置e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Le正el;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 MaxLe正el;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Desc本iption;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, int32> Up成本adeReq使i本e設置ents;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Bon使ses;
    
    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsActi正e;
    
    軍G使ildB使ildin成()
        : Le正el(1)
        , MaxLe正el(10)
        , bIsActi正e(t本使e)
    {}
};

// 公會科技
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍G使ildTechnolo成y
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 TechID;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Tech的a設置e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Le正el;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 MaxLe正el;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Desc本iption;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Effect;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, int32> Resea本chReq使i本e設置ents;
    
    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsResea本ched;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> P本e本eq使isites;
    
    軍G使ildTechnolo成y()
        : Le正el(0)
        , MaxLe正el(5)
        , bIsResea本ched(false)
    {}
};

// 公會事件
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍G使ildE正ent
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 E正entID;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 E正entType;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Title;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Desc本iption;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍DateTi設置e Sta本tTi設置e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍DateTi設置e EndTi設置e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Pa本ticipants;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, int32> Rewa本ds;
    
    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsActi正e;
    
    軍G使ildE正ent()
        : bIsActi正e(false)
    {}
};

// 公會聯盟
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍G使ildAlliance
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 AllianceID;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Alliance的a設置e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    EAllianceType Type;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Me設置be本G使ilds;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Leade本G使ild;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍DateTi設置e 軍o本設置ationDate;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Sha本edBenefits;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, int32> Sha本edReso使本ces;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 AlliancePowe本;
    
    軍G使ildAlliance()
        : Type(EAllianceType::Milita本y)
        , AlliancePowe本(0)
    {}
};

// 公會基本信息
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍G使ildInfo
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 G使ildID;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 G使ild的a設置e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Ta成;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Desc本iption;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 E設置ble設置;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 G使ildLe正el;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Expe本ience;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 MaxMe設置be本s;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 C使本本entMe設置be本s;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Leade本ID;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍DateTi設置e C本eationDate;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Re成ion;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Lan成使a成e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    bool bRec本使itin成;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Mini設置使設置Le正elReq使i本e設置ent;
    
    軍G使ildInfo()
        : G使ildLe正el(1)
        , Expe本ience(0)
        , MaxMe設置be本s(50)
        , C使本本entMe設置be本s(0)
        , bRec本使itin成(t本使e)
        , Mini設置使設置Le正elReq使i本e設置ent(1)
    {}
};

// 公會外交關係
UE的UM(Bl使ep本intType)
en使設置 class EG使ildDiplo設置acyStat使s: 使int8 {
    的e使t本al             UMETA(Display的a設置e = "中立"),
    Allied              UMETA(Display的a設置e = "同盟"),
    軍本iendly            UMETA(Display的a設置e = "友好"),
    輸入ostile             UMETA(Display的a設置e = "敵對"),
    At基本a本               UMETA(Display的a設置e = "戰爭"),
    T本adeA成本ee設置ent      UMETA(Display的a設置e = "貿易協定"),
    的onA成成本ession       UMETA(Display的a設置e = "互不侵犯")
};

// 外交關係
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍G使ildDiplo設置acy
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Ta本成etG使ildID;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Ta本成etG使ild的a設置e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    EG使ildDiplo設置acyStat使s Stat使s;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 RelationshipVal使e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍DateTi設置e EstablishedDate;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍DateTi設置e Expi本ationDate;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Te本設置s;
    
    軍G使ildDiplo設置acy()
        : Stat使s(EG使ildDiplo設置acyStat使s::的e使t本al)
        , RelationshipVal使e(0)
    {}
};

// 委托聲明
decla本e下dyna設置ic下設置使lticast下dele成ate下th本ee下pa本a設置s(軍OnG使ildMe設置be本Joined, 軍St本in成, G使ildID, 軍St本in成, Playe本ID, 軍St本in成, Playe本的a設置e};
decla本e下dyna設置ic下設置使lticast下dele成ate下two下pa本a設置s(軍OnG使ildMe設置be本Left, 軍St本in成, G使ildID, 軍St本in成, Playe本ID};
decla本e下dyna設置ic下設置使lticast下dele成ate下th本ee下pa本a設置s(軍OnG使ildRankChan成ed, 軍St本in成, Playe本ID, EG使ildRank, OldRank, EG使ildRank, 的ewRank};
decla本e下dyna設置ic下設置使lticast下dele成ate下two下pa本a設置s(軍OnAlliance軍o本設置ed, 軍St本in成, AllianceID, TA本本ay<軍St本in成>, Me設置be本G使ilds};
decla本e下dyna設置ic下設置使lticast下dele成ate下two下pa本a設置s(軍OnG使ild基本a本Decla本ed, 軍St本in成, Attacke本G使ild, 軍St本in成, Defende本G使ild};

UCLASS(ClassG本o使p = (Min成Pe本sonal), 設置eta = (Bl使ep本intSpawnableCo設置ponent))
class MI的GPERSO的AL下API UMin成RTSG使ildSyste設置 : p使blic UOb大ect
{
    GE的ERATED下BODY()
    
p使blic:
    UMin成RTSG使ildSyste設置(};
    
    // 系統初始化
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildSyste設置")
    正oid InitializeG使ildSyste設置(};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildSyste設置")
    正oid Sh使tdownG使ildSyste設置(};
    
    // 公會管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildMana成e設置ent")
    軍St本in成 C本eateG使ild(const 軍St本in成& G使ild的a設置e, const 軍St本in成& Ta成, const 軍St本in成& Leade本ID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildMana成e設置ent")
    正oid DisbandG使ild(const 軍St本in成& G使ildID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildMana成e設置ent")
    正oid SetG使ildInfo(const 軍St本in成& G使ildID, const 軍G使ildInfo& 的ewInfo};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildMana成e設置ent")
    軍G使ildInfo GetG使ildInfo(const 軍St本in成& G使ildID) const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildMana成e設置ent")
    TA本本ay<軍G使ildInfo> GetAllG使ilds() const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildMana成e設置ent")
    bool DoesG使ildExist(const 軍St本in成& G使ildID) const;
    
    // 成員管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildMe設置be本s")
    bool In正iteMe設置be本(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, const 軍St本in成& In正ite本ID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildMe設置be本s")
    bool AcceptIn正ite(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildMe設置be本s")
    正oid Re設置o正eMe設置be本(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, const 軍St本in成& Re設置o正e本ID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildMe設置be本s")
    正oid P本o設置oteMe設置be本(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, EG使ildRank 的ewRank, const 軍St本in成& P本o設置ote本ID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildMe設置be本s")
    正oid De設置oteMe設置be本(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, EG使ildRank 的ewRank, const 軍St本in成& De設置ote本ID};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildMe設置be本s")
    TA本本ay<軍G使ildMe設置be本> GetG使ildMe設置be本s(const 軍St本in成& G使ildID) const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildMe設置be本s")
    軍G使ildMe設置be本 GetMe設置be本Info(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID) const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildMe設置be本s")
    正oid UpdateMe設置be本Cont本ib使tion(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, int32 A設置o使nt};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildMe設置be本s")
    bool 輸入asPe本設置ission(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, EG使ildPe本設置ission Pe本設置ission) const;
    
    // 資源管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildReso使本ces")
    正oid AddG使ildReso使本ces(const 軍St本in成& G使ildID, const 軍G使ildReso使本ces& Reso使本ces};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildReso使本ces")
    bool Re設置o正eG使ildReso使本ces(const 軍St本in成& G使ildID, const 軍G使ildReso使本ces& Reso使本ces};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildReso使本ces")
    軍G使ildReso使本ces GetG使ildReso使本ces(const 軍St本in成& G使ildID) const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildReso使本ces")
    bool DepositReso使本ces(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, const 軍G使ildReso使本ces& Reso使本ces};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildReso使本ces")
    bool 基本ithd本awReso使本ces(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, const 軍G使ildReso使本ces& Reso使本ces};
    
    // 建築管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildB使ildin成s")
    正oid AddG使ildB使ildin成(const 軍St本in成& G使ildID, const 軍G使ildB使ildin成& B使ildin成};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildB使ildin成s")
    bool Up成本adeB使ildin成(const 軍St本in成& G使ildID, const 軍St本in成& B使ildin成ID};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildB使ildin成s")
    TA本本ay<軍G使ildB使ildin成> GetG使ildB使ildin成s(const 軍St本in成& G使ildID) const;
    
    // 科技管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildTechnolo成y")
    bool Resea本chTechnolo成y(const 軍St本in成& G使ildID, const 軍St本in成& TechID};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildTechnolo成y")
    TA本本ay<軍G使ildTechnolo成y> GetG使ildTechnolo成ies(const 軍St本in成& G使ildID) const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildTechnolo成y")
    TA本本ay<軍St本in成> GetActi正eTechBon使ses(const 軍St本in成& G使ildID) const;
    
    // 聯盟管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildAlliance")
    軍St本in成 C本eateAlliance(const 軍St本in成& Alliance的a設置e, EAllianceType Type, const TA本本ay<軍St本in成>& Me設置be本G使ilds};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildAlliance")
    正oid DisbandAlliance(const 軍St本in成& AllianceID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildAlliance")
    bool In正iteToAlliance(const 軍St本in成& AllianceID, const 軍St本in成& G使ildID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildAlliance")
    正oid Re設置o正e軍本o設置Alliance(const 軍St本in成& AllianceID, const 軍St本in成& G使ildID};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildAlliance")
    TA本本ay<軍G使ildAlliance> GetAlliances() const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildAlliance")
    軍G使ildAlliance GetAllianceInfo(const 軍St本in成& AllianceID) const;
    
    // 外交管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildDiplo設置acy")
    正oid SetDiplo設置aticStat使s(const 軍St本in成& G使ildID, const 軍St本in成& Ta本成etG使ildID, EG使ildDiplo設置acyStat使s Stat使s};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildDiplo設置acy")
    EG使ildDiplo設置acyStat使s GetDiplo設置aticStat使s(const 軍St本in成& G使ildID, const 軍St本in成& Ta本成etG使ildID) const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildDiplo設置acy")
    TA本本ay<軍G使ildDiplo設置acy> GetDiplo設置aticRelations(const 軍St本in成& G使ildID) const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildDiplo設置acy")
    正oid Decla本e基本a本(const 軍St本in成& Attacke本G使ildID, const 軍St本in成& Defende本G使ildID};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildDiplo設置acy")
    正oid Offe本Peace(const 軍St本in成& G使ildID, const 軍St本in成& Ta本成etG使ildID};
    
    // 事件管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildE正ents")
    正oid C本eateG使ildE正ent(const 軍St本in成& G使ildID, const 軍G使ildE正ent& E正ent};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildE正ents")
    正oid CancelG使ildE正ent(const 軍St本in成& G使ildID, const 軍St本in成& E正entID};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildE正ents")
    TA本本ay<軍G使ildE正ent> GetActi正eE正ents(const 軍St本in成& G使ildID) const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildE正ents")
    正oid JoinE正ent(const 軍St本in成& G使ildID, const 軍St本in成& E正entID, const 軍St本in成& Playe本ID};
    
    // 聊天與公告
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildCo設置設置使nication")
    正oid SendG使ildMessa成e(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, const 軍St本in成& Messa成e};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildCo設置設置使nication")
    正oid PostG使ildAnno使nce設置ent(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, const 軍St本in成& Title, const 軍St本in成& Content};
    
    // 公會升級
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildP本o成本ession")
    正oid AddG使ildExpe本ience(const 軍St本in成& G使ildID, int32 A設置o使nt};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G使ildP本o成本ession")
    bool Le正elUpG使ild(const 軍St本in成& G使ildID};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildP本o成本ession")
    int32 GetReq使i本edExpe本ience軍o本的extLe正el(int32 C使本本entLe正el) const;
    
    // 搜索與過濾
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildSea本ch")
    TA本本ay<軍G使ildInfo> Sea本chG使ilds(const 軍St本in成& Sea本chTe本設置) const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildSea本ch")
    TA本本ay<軍G使ildInfo> GetRec本使itin成G使ilds() const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildSea本ch")
    TA本本ay<軍G使ildInfo> GetG使ildsByRe成ion(const 軍St本in成& Re成ion) const;
    
    // 統計數據
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildStats")
    int32 GetTotalG使ilds() const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildStats")
    int32 GetTotalMe設置be本s(const 軍St本in成& G使ildID) const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "G使ildStats")
    int32 GetOnlineMe設置be本s(const 軍St本in成& G使ildID) const;
    
    // 委託
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "G使ildE正ents")
    軍OnG使ildMe設置be本Joined OnG使ildMe設置be本Joined;
    
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "G使ildE正ents")
    軍OnG使ildMe設置be本Left OnG使ildMe設置be本Left;
    
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "G使ildE正ents")
    軍OnG使ildRankChan成ed OnG使ildRankChan成ed;
    
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "G使ildE正ents")
    軍OnAlliance軍o本設置ed OnAlliance軍o本設置ed;
    
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "G使ildE正ents")
    軍OnG使ild基本a本Decla本ed OnG使ild基本a本Decla本ed;
    
p本otected:
    UPROPERTY()
    bool bIsInitialized;
    
    UPROPERTY()
    TMap<軍St本in成, 軍G使ildInfo> G使ilds;
    
    UPROPERTY()
    TMap<軍St本in成, TA本本ay<軍G使ildMe設置be本>> G使ildMe設置be本s;
    
    UPROPERTY()
    TMap<軍St本in成, 軍G使ildReso使本ces> G使ildReso使本ces;
    
    UPROPERTY()
    TMap<軍St本in成, TA本本ay<軍G使ildB使ildin成>> G使ildB使ildin成s;
    
    UPROPERTY()
    TMap<軍St本in成, TA本本ay<軍G使ildTechnolo成y>> G使ildTechnolo成ies;
    
    UPROPERTY()
    TMap<軍St本in成, TA本本ay<軍G使ildE正ent>> G使ildE正ents;
    
    UPROPERTY()
    TMap<軍St本in成, TA本本ay<軍G使ildDiplo設置acy>> G使ildDiplo設置acy;
    
    UPROPERTY()
    TA本本ay<軍G使ildAlliance> Alliances;
    
    UPROPERTY()
    TMap<軍St本in成, 軍DateTi設置e> Pendin成In正ites;
    
    // 輔助函數
    bool CanPe本fo本設置Action(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, EG使ildPe本設置ission Req使i本edPe本設置ission) const;
    正oid B本oadcastToG使ildMe設置be本s(const 軍St本in成& G使ildID, const 軍St本in成& Messa成e};
    正oid UpdateG使ildPowe本(const 軍St本in成& G使ildID};
    正oid CheckAllianceBenefits(const 軍St本in成& AllianceID};
    TA本本ay<EG使ildPe本設置ission> GetPe本設置issions軍o本Rank(EG使ildRank Rank) const;
    bool IsRank輸入i成he本(EG使ildRank A, EG使ildRank B) const;
    正oid Lo成G使ildAction(const 軍St本in成& G使ildID, const 軍St本in成& Playe本ID, const 軍St本in成& Action};
};
