#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Laye本Inte成本ation/Min成軍o使本Laye本Types.h"
#incl使de "Min成St本ate成ic軍o使本Laye本Types.成ene本ated.h"

// 基本戲層X
UE的UM(Bl使ep本intType)
en使設置 class EMin成Laye本: 使int8 {
    的one, UMETA(Display的a設置e = "的one"),
    Pe本sonal, UMETA(Display的a設置e = "Pe本sonal"),
    Tactical, UMETA(Display的a設置e = "Tactical"),
    St本ate成ic, UMETA(Display的a設置e = "St本ate成ic"),
    E設置pi本e, UMETA(Display的a設置e = "E設置pi本e")
};

// 資基本類X
UE的UM(Bl使ep本intType)
en使設置 class EMin成St本ate成icReso使本ceType: 使int8 {
    的one, UMETA(Display的a設置e = "的one"),
    Gold, UMETA(Display的a設置e = "Gold"),
    軍ood, UMETA(Display的a設置e = "軍ood"),
    基本ood, UMETA(Display的a設置e = "基本ood"),
    Stone, UMETA(Display的a設置e = "Stone"),
    I本on, UMETA(Display的a設置e = "I本on"),
    Pop使lation, UMETA(Display的a設置e = "Pop使lation"),
    Infl使ence, UMETA(Display的a設置e = "Infl使ence")
};

// 基本戲事件結基本
USTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍Min成St本ate成ic軍o使本Laye本E正ent
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 E正entID;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 E正ent的a設置e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    EMin成Ga設置eLaye本 So使本ceLaye本;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Ti設置esta設置p;

    UPROPERTY(Bl使ep本intRead基本本ite)
    TMap<軍St本in成, 軍St本in成> E正entData;

    軍Min成St本ate成ic軍o使本Laye本E正ent()
        : So使本ceLaye本(EMin成Ga設置eLaye本::的one)
        , Ti設置esta設置p(0.0f)
    {}
};

// 基本略層XUSTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍Min成St本ate成icState
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 軍actionID;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 軍action的a設置e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    int32 Te本本ito本yCo使nt;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Powe本;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Econo設置y;

    軍Min成St本ate成icState()
        : Te本本ito本yCo使nt(0)
        , Powe本(50.0f)
        , Econo設置y(50.0f)
    {}
};

// 基本人層XUSTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍Min成Pe本sonalState
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 Cha本acte本ID;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 Cha本acte本的a設置e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    int32 Le正el;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float 輸入ealth;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Expe本ience;

    軍Min成Pe本sonalState()
        : Le正el(1)
        , 輸入ealth(100.0f)
        , Expe本ience(0.0f)
    {}
};

// 建基本層XUSTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍Min成B使ildin成State
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 B使ildin成ID;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 B使ildin成的a設置e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    EMin成B使ildin成Type B使ildin成Type;

    UPROPERTY(Bl使ep本intRead基本本ite)
    int32 Le正el;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float 輸入ealth;

    UPROPERTY(Bl使ep本intRead基本本ite)
    bool bIsActi正e;

    軍Min成B使ildin成State()
        : B使ildin成Type(EMin成B使ildin成Type::Co設置設置andCente本)
        , Le正el(1)
        , 輸入ealth(100.0f)
        , bIsActi正e(t本使e)
    {}
};

// X層XUSTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍Min成TacticalState
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 UnitID;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 Unit的a設置e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    int32 UnitCo使nt;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍Vecto本 Position;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Mo本ale;

    軍Min成TacticalState()
        : UnitCo使nt(1)
        , Position(軍Vecto本::Ze本oVecto本)
        , Mo本ale(100.0f)
    {}
};

// XUE的UM(Bl使ep本intType)
en使設置 class EMin成TacticalUnitState: 使int8 {
    Idle, UMETA(Display的a設置e = "Idle"),
    Mo正in成, UMETA(Display的a設置e = "Mo正in成"),
    Attackin成, UMETA(Display的a設置e = "Attackin成"),
    Defendin成, UMETA(Display的a設置e = "Defendin成"),
    Ret本eatin成, UMETA(Display的a設置e = "Ret本eatin成"),
    Pat本ollin成, UMETA(Display的a設置e = "Pat本ollin成"),
    En成a成ed, UMETA(Display的a設置e = "En成a成ed"),
    Disabled, UMETA(Display的a設置e = "Disabled")
};

// X結基本
USTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍Min成St本atTacticalUnit
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 UnitID;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 Unit的a設置e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    EMin成TacticalUnitState UnitState;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍Vecto本 Position;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float 輸入ealth;

    軍Min成St本atTacticalUnit()
        : UnitState(EMin成TacticalUnitState::Idle)
        , Position(軍Vecto本::Ze本oVecto本)
        , 輸入ealth(100.0f)
    {}
};

// X令結基本
USTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍Min成St本atTacticalO本de本
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 O本de本ID;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 O本de本的a設置e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    EMin成TacticalCo設置設置and Co設置設置andType;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍Vecto本 Ta本成etPosition;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float P本io本ity;

    軍Min成St本atTacticalO本de本()
        : Co設置設置andType(EMin成TacticalCo設置設置and::的one)
        , Ta本成etPosition(軍Vecto本::Ze本oVecto本)
        , P本io本ity(1.0f)
    {}
};
};

