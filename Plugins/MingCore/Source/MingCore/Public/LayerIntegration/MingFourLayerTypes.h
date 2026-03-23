#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成軍o使本Laye本Types.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EMin成Ga設置eLaye本: 使int8 {
    的one UMETA(Display的a設置e = "的one"),
    St本ate成ic UMETA(Display的a設置e = "St本ate成ic Laye本"),
    Tactical UMETA(Display的a設置e = "Tactical Laye本"),
    Pe本sonal UMETA(Display的a設置e = "Pe本sonal Laye本"),
    B使ildin成 UMETA(Display的a設置e = "B使ildin成 Laye本")
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成Laye本T本ansitionType: 使int8 {
    Instant UMETA(Display的a設置e = "Instant"),
    軍ade UMETA(Display的a設置e = "軍ade"),
    Slide UMETA(Display的a設置e = "Slide"),
    Zoo設置 UMETA(Display的a設置e = "Zoo設置"),
    Cine設置atic UMETA(Display的a設置e = "Cine設置atic")
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成St本ate成icAction: 使int8 {
    的one UMETA(Display的a設置e = "的one"),
    Decla本e基本a本 UMETA(Display的a設置e = "Decla本e 基本a本"),
    軍o本設置Alliance UMETA(Display的a設置e = "軍o本設置 Alliance"),
    T本adeA成本ee設置ent UMETA(Display的a設置e = "T本ade A成本ee設置ent"),
    Mo正eT本oops UMETA(Display的a設置e = "Mo正e T本oops"),
    Diplo設置aticMission UMETA(Display的a設置e = "Diplo設置atic Mission"),
    Espiona成e UMETA(Display的a設置e = "Espiona成e"),
    Reso使本ceT本adin成 UMETA(Display的a設置e = "Reso使本ce T本adin成")
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成TacticalCo設置設置and: 使int8 {
    的one UMETA(Display的a設置e = "的one"),
    Attack UMETA(Display的a設置e = "Attack"),
    Defend UMETA(Display的a設置e = "Defend"),
    Ret本eat UMETA(Display的a設置e = "Ret本eat"),
    軍lank UMETA(Display的a設置e = "軍lank"),
    輸入oldPosition UMETA(Display的a設置e = "輸入old Position"),
    Cha本成e UMETA(Display的a設置e = "Cha本成e"),
    A設置b使sh UMETA(Display的a設置e = "A設置b使sh"),
    S使ppo本t UMETA(Display的a設置e = "S使ppo本t"),
    Recon UMETA(Display的a設置e = "Reconnaissance")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Laye本State
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Laye本 State")
    EMin成Ga設置eLaye本 C使本本entLaye本 = EMin成Ga設置eLaye本::的one;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Laye本 State")
    EMin成Ga設置eLaye本 P本e正io使sLaye本 = EMin成Ga設置eLaye本::的one;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Laye本 State")
    float T本ansitionP本o成本ess = 0.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Laye本 State")
    bool bIsT本ansitionin成 = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Laye本 State")
    EMin成Laye本T本ansitionType T本ansitionType = EMin成Laye本T本ansitionType::軍ade;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Laye本 State")
    float T本ansitionD使本ation = 1.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Laye本 State")
    軍St本in成 Laye本Context;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Laye本 State")
    TMap<軍St本in成, 軍St本in成> Laye本Data;

    軍Min成Laye本State()
        : C使本本entLaye本(EMin成Ga設置eLaye本::的one)
        , P本e正io使sLaye本(EMin成Ga設置eLaye本::的one)
        , T本ansitionP本o成本ess(0.0f)
        , bIsT本ansitionin成(false)
        , T本ansitionType(EMin成Laye本T本ansitionType::軍ade)
        , T本ansitionD使本ation(1.0f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Ga設置eDecision
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    軍St本in成 DecisionID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    軍St本in成 Decision的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    int32 P本io本ity = 0;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    float Confidence = 1.0f;

    軍Min成Ga設置eDecision()
        : P本io本ity(0)
        , Confidence(1.0f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Laye本Conflict
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Conflict")
    EMin成Ga設置eLaye本 So使本ceLaye本 = EMin成Ga設置eLaye本::的one;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Conflict")
    EMin成Ga設置eLaye本 Ta本成etLaye本 = EMin成Ga設置eLaye本::的one;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Conflict")
    軍St本in成 ConflictType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Conflict")
    float Se正e本ity = 0.5f;

    軍Min成Laye本Conflict()
        : So使本ceLaye本(EMin成Ga設置eLaye本::的one)
        , Ta本成etLaye本(EMin成Ga設置eLaye本::的one)
        , Se正e本ity(0.5f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成St本ate成icDecision
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "St本ate成ic")
    軍St本in成 DecisionID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "St本ate成ic")
    軍St本in成 Decision的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "St本ate成ic")
    EMin成St本ate成icAction ActionType = EMin成St本ate成icAction::的one;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "St本ate成ic")
    軍St本in成 Ta本成et軍action;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "St本ate成ic")
    TA本本ay<軍St本in成> In正ol正edRe成ions;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "St本ate成ic")
    int32 Reso使本ceCost = 0;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "St本ate成ic")
    int32 T本oopReq使i本e設置ent = 0;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "St本ate成ic")
    float ExpectedO使tco設置e = 0.5f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "St本ate成ic")
    TA本本ay<軍St本in成> P本e本eq使isites;

    軍Min成St本ate成icDecision()
        : ActionType(EMin成St本ate成icAction::的one)
        , Reso使本ceCost(0)
        , T本oopReq使i本e設置ent(0)
        , ExpectedO使tco設置e(0.5f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成TacticalO本de本
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Tactical")
    軍St本in成 O本de本ID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Tactical")
    EMin成TacticalCo設置設置and Co設置設置andType = EMin成TacticalCo設置設置and::的one;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Tactical")
    TA本本ay<int32> UnitIDs;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Tactical")
    軍Vecto本 Ta本成etLocation;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Tactical")
    int32 Ta本成etUnitID = -1;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Tactical")
    float P本io本ity = 1.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Tactical")
    bool bA使toExec使te = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Tactical")
    float 軍o本設置ation基本idth = 100.0f;

    軍Min成TacticalO本de本()
        : Co設置設置andType(EMin成TacticalCo設置設置and::的one)
        , Ta本成etUnitID(-1)
        , P本io本ity(1.0f)
        , bA使toExec使te(false)
        , 軍o本設置ation基本idth(100.0f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Pe本sonalAction
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Pe本sonal")
    軍St本in成 ActionID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Pe本sonal")
    軍St本in成 Action的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Pe本sonal")
    軍St本in成 Cha本acte本ID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Pe本sonal")
    軍St本in成 Ta本成etCha本acte本ID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Pe本sonal")
    軍St本in成 Dialo成使eID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Pe本sonal")
    bool bIsCo設置bat = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Pe本sonal")
    bool bIsDiplo設置atic = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Pe本sonal")
    TA本本ay<軍St本in成> RelatedDecisions;

    軍Min成Pe本sonalAction()
        : bIsCo設置bat(false)
        , bIsDiplo設置atic(false)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成B使ildin成Co設置設置and
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    軍St本in成 Co設置設置andID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    軍St本in成 B使ildin成Type;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    軍Vecto本 B使ildLocation;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    int32 B使ildin成Le正el = 1;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    bool bIsUp成本ade = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    bool bIsDe設置olish = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    TA本本ay<軍St本in成> Reso使本ceReq使i本e設置ents;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    float B使ildTi設置e = 60.0f;

    軍Min成B使ildin成Co設置設置and()
        : B使ildin成Le正el(1)
        , bIsUp成本ade(false)
        , bIsDe設置olish(false)
        , B使ildTi設置e(60.0f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Laye本SyncData
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Sync")
    TMap<軍St本in成, int32> St本ate成icReso使本ces;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Sync")
    TMap<軍St本in成, int32> TacticalUnits;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Sync")
    軍St本in成 Pe本sonalCha本acte本State;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Sync")
    TMap<軍St本in成, 軍St本in成> B使ildin成States;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Sync")
    float GlobalTi設置e = 0.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Sync")
    軍St本in成 C使本本entMissionID;

    軍Min成Laye本SyncData()
        : GlobalTi設置e(0.0f)
    {}
};

