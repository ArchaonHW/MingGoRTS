#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Ga設置e軍本a設置ewo本k/Acto本.h"
#incl使de "Min成TacticalCo設置batSyste設置.成ene本ated.h"

// X動X
UE的UM(Bl使ep本intType)
en使設置 class EMin成UnitType: 使int8 {
    Infant本y,          // BL
    Ca正al本y,           // ML
    A本tille本y,         // 務L
    Tank,              // X
    Ai本c本aft,          // 動X
    的a正al,             // 動X
    S使ppo本t,           // X動X
    Co設置設置ande本,  // X動X};

// 務sX動X
UE的UM(Bl使ep本intType)
en使設置 class EMin成Tactical軍o本設置ationType: 使int8 {
    Line,              // 處動X
    Col使設置n,            // 基本aX
    基本ed成e,             // X
    Dia設置ond,           // X動X
    Ci本cle,            // X動X
    Ski本設置ish,          // X
    Sq使a本e,            // X動X
    A本本ow,             // bYX
    C本escent,          // X
    C使sto設置,  // 務q基本}X};

// X動XUE的UM(Bl使ep本intType)
en使設置 class EMin成Co設置batState: 使int8 {
    Idle,              // 動X
    Mo正in成,            // 動X
    Attackin成,          // X
    Defendin成,         // 務設置X
    Ret本eatin成,         // 務hX
    Ro使ted,            // 動X
    Capt使本ed,          // 基本QX
    Dest本oyed          // QR動
};

/**
 * int32 X動X (X動 TMap<TA本本ay> 是X)
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Int32A本本ay基本本appe本
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<int32> Ite設置s;

    軍Int32A本本ay基本本appe本() {}
};

// XXUSTRUCT(Bl使ep本intType)
st本使ct MI的GTACTICAL下API 軍Min成UnitCo設置batStats
{
    GE的ERATED下BODY()

    // X動X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float 輸入ealth;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Max輸入ealth;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float AttackPowe本;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float DefensePowe本;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Mo正e設置entSpeed;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float AttackRan成e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float VisionRan成e;

    // X動X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Mo本ale;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Discipline;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Leade本ship;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Expe本ience;

    // X動X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Acc使本acy;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float C本iticalChance;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float E正asion;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float A本設置o本;

    軍Min成UnitCo設置batStats()
    {
        輸入ealth = 100.0f;
        Max輸入ealth = 100.0f;
        AttackPowe本 = 20.0f;
        DefensePowe本 = 15.0f;
        Mo正e設置entSpeed = 5.0f;
        AttackRan成e = 2.0f;
        VisionRan成e = 8.0f;
        Mo本ale = 75.0f;
        Discipline = 70.0f;
        Leade本ship = 50.0f;
        Expe本ience = 0.0f;
        Acc使本acy = 0.7f;
        C本iticalChance = 0.1f;
        E正asion = 0.05f;
        A本設置o本 = 0.2f;
};

// X
USTRUCT(Bl使ep本intType)
st本使ct MI的GTACTICAL下API 軍Min成Tactical軍o本設置ation
{
    GE的ERATED下BODY()

    // XID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 軍o本設置ationID;

    // X動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 軍o本設置ation的a設置e;

    // X動X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成軍o本設置ationType 軍o本設置ationType;

    // 務z
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 軍o本設置ationDesc本iption;

    // X動設置X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍Vecto本> 軍o本設置ationPositions;

    // X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TMap<軍St本in成, float> 軍o本設置ationBon使ses;

    // X動X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TMap<軍St本in成, float> 軍o本設置ationPenalties;

    // 務池務
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<EMin成UnitType> ApplicableUnitTypes;

    // 務pX
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 MinUnitCo使nt;

    // 務大X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 MaxUnitCo使nt;

    軍Min成Tactical軍o本設置ation()
    {
        軍o本設置ationID = -1;
        軍o本設置ation的a設置e = TEXT(""};
        軍o本設置ationType = EMin成軍o本設置ationType::Line;
        軍o本設置ationDesc本iption = TEXT(""};
        軍o本設置ationPositions.E設置pty(};
        軍o本設置ationBon使ses.E設置pty(};
        軍o本設置ationPenalties.E設置pty(};
        ApplicableUnitTypes.E設置pty(};
        MinUnitCo使nt = 1;
        MaxUnitCo使nt = 100;
};

// X動X
USTRUCT(Bl使ep本intType)
st本使ct MI的GTACTICAL下API 軍Min成Co設置batUnit
{
    GE的ERATED下BODY()

    // XID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 UnitID;

    // X動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Unit的a設置e;

    // X動X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成UnitType UnitType;

    // X動設置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 C使本本entPosition;

    // X動設置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 Ta本成etPosition;

    // X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成Co設置batState C使本本entState;

    // XX
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Min成UnitCo設置batStats Co設置batStats;

    // X動sXID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 軍o本設置ationID;

    // 基本sX
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 軍o本設置ationPosition;

    // X動基本xID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Co設置設置ande本ID;

    // X動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<int32> Ene設置yUnits;

    // X動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<int32> 軍本iendlyUnits;

    // 務下基本Q動X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsSelected;

    // 務下X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsVisible;

    // X動功X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 軍o成Of基本a本Le正el;

    軍Min成Co設置batUnit()
    {
        UnitID = -1;
        Unit的a設置e = TEXT(""};
        UnitType = EMin成UnitType::Infant本y;
        C使本本entPosition = 軍Vecto本::Ze本oVecto本;
        Ta本成etPosition = 軍Vecto本::Ze本oVecto本;
        C使本本entState = EMin成Co設置batState::Idle;
        軍o本設置ationID = -1;
        軍o本設置ationPosition = -1;
        Co設置設置ande本ID = -1;
        Ene設置yUnits.E設置pty(};
        軍本iendlyUnits.E設置pty(};
        bIsSelected = false;
        bIsVisible = t本使e;
        軍o成Of基本a本Le正el = 0;
};

// 務O
USTRUCT(Bl使ep本intType)
st本使ct MI的GTACTICAL下API 軍Min成TacticalCo設置設置and
{
    GE的ERATED下BODY()

    // 務OID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Co設置設置andID;

    // 務O動X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Co設置設置andType;

    // XID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Ta本成etUnitID;

    // X動設置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 Ta本成etLocation;

    // 務OX動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TMap<軍St本in成, 軍St本in成> Co設置設置andPa本a設置ete本s;

    // 務OX
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 P本io本ity;

    // 務OX
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Exec使tionTi設置e;

    // 務下X動RX
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsContin使o使s;

    軍Min成TacticalCo設置設置and()
    {
        Co設置設置andID = -1;
        Co設置設置andType = TEXT(""};
        Ta本成etUnitID = -1;
        Ta本成etLocation = 軍Vecto本::Ze本oVecto本;
        Co設置設置andPa本a設置ete本s.E設置pty(};
        P本io本ity = 1;
        Exec使tionTi設置e = 0.0f;
        bIsContin使o使s = false;
};

// XX
USTRUCT(Bl使ep本intType)
st本使ct MI的GTACTICAL下API 軍Min成Co設置batRes使lt
{
    GE的ERATED下BODY()

    // XID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Attacke本ID;

    // 務設置XID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Defende本ID;

    // 務軍事X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Da設置a成eDealt;

    // 務下X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsC本itical輸入it;

    // 務下X動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIs輸入it;

    // X動X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍DateTi設置e Co設置batTi設置e;

    // X設置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 Co設置batLocation;

    // XX
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Co設置batType;

    // 功X動X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Expe本ienceGained;

    軍Min成Co設置batRes使lt()
    {
        Attacke本ID = -1;
        Defende本ID = -1;
        Da設置a成eDealt = 0.0f;
        bIsC本itical輸入it = false;
        bIs輸入it = false;
        Co設置batTi設置e = 軍DateTi設置e::的ow(};
        Co設置batLocation = 軍Vecto本::Ze本oVecto本;
        Co設置batType = TEXT(""};
        Expe本ienceGained = 0.0f;
};
// X基本eX





/**
 * X動建動
 * td基本池大XX動
 */
UCLASS(ClassG本o使p = (Tactical), Bl使ep本intable, Bl使ep本intType)
class MI的GTACTICAL下API AMin成TacticalCo設置batSyste設置 : p使blic AActo本
{
    GE的ERATED下BODY()

p使blic:
    AMin成TacticalCo設置batSyste設置(};
    // X動的tX
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Tactical Co設置bat")
    bool InitializeTacticalSyste設置(};
    // 基本池務
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Tactical Co設置bat")
    bool Re成iste本Co設置batUnit(const 軍Min成Co設置batUnit& Unit};
    // X動X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Tactical Co設置bat")
    bool Re設置o正eCo設置batUnit(int32 UnitID};
    // 務輸入動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Tactical Co設置bat")
    軍Min成Co設置batUnit GetUnitInfo下I設置pl(int32 UnitID) const;

    // X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Tactical Co設置bat")
    bool SelectUnits下I設置pl(const TA本本ay<int32>& UnitIDs};
    // X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Tactical Co設置bat")
    bool DeselectUnits(const TA本本ay<int32>& UnitIDs};
    // 務w基本池務
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Tactical Co設置bat")
    TA本本ay<int32> GetSelectedUnits() const;

    // 動X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Tactical Co設置bat")
    bool Mo正eUnits(const TA本本ay<int32>& UnitIDs, const 軍Vecto本& Ta本成etLocation};
    // X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Tactical Co設置bat")
    bool AttackTa本成et(int32 Attacke本ID, int32 Ta本成etID};
    // 基本]設置sX
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Tactical Co設置bat")
    bool Set軍o本設置ation(const TA本本ay<int32>& UnitIDs, EMin成軍o本設置ationType 軍o本設置ationType};
    // 務s務輸入動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Tactical Co設置bat")
    軍Min成Tactical軍o本設置ation Get軍o本設置ationInfo(EMin成軍o本設置ationType 軍o本設置ationType) const;

    // 務O
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Tactical Co設置bat")
    bool Iss使eTacticalCo設置設置and(const 軍Min成TacticalCo設置設置and& Co設置設置and};
    // X動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Tactical Co設置bat")
    軍Min成Co設置batRes使lt P本ocessCo設置bat(int32 Attacke本ID, int32 Defende本ID};
    // 務sX動功動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Tactical Co設置bat")
    正oid Update軍o成Of基本a本(};
    // X
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Tactical Co設置bat")
    TA本本ay<int32> GetVisibleUnits下I設置pl(int32 Viewe本UnitID) const;

    // XX
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Tactical Co設置bat")
    TMap<EMin成UnitType, int32> GetCo設置batStatistics下I設置pl() const;

    // 是X動X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Tactical Co設置bat")
    bool Sa正eCo設置batData(};
    // 動基本JX動X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Tactical Co設置bat")
    bool LoadCo設置batData(};
    // M池務
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Tactical Co設置bat")
    正oid Clea本AllUnits(};
    // 動基本eX
    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnUnitStateChan成ed OnUnitStateChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnCo設置batOcc使本本ed OnCo設置batOcc使本本ed;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍On軍o本設置ationChan成ed On軍o本設置ationChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnTacticalCo設置設置andIss使ed OnTacticalCo設置設置andIss使ed;

p本otected:
    // Called when the 成a設置e sta本ts o本 when spawned
    正i本t使al 正oid Be成inPlay() o正e本本ide;

    // Called e正e本y f本a設置e
    正i本t使al 正oid Tick(float DeltaTi設置e) o正e本本ide;

    // X
    UPROPERTY()
    TMap<int32, 軍Min成Co設置batUnit> UnitDatabase;

    // 基本sX
    UPROPERTY()
    TMap<EMin成軍o本設置ationType, 軍Min成Tactical軍o本設置ation> 軍o本設置ationDatabase;

    // w池動X
    UPROPERTY()
    TA本本ay<int32> SelectedUnits;

    // X正OX
    UPROPERTY()
    TA本本ay<軍Min成Co設置batRes使lt> Co設置bat輸入isto本y;

    // X動功動X
    UPROPERTY()
    TMap<int32, 軍Int32A本本ay基本本appe本> VisibilityMap;

    // 務sX
    UPROPERTY()
    TMap<int32, EMin成軍o本設置ationType> Unit軍o本設置ations;

    // 務下文務lX
    UPROPERTY()
    bool bInitialized;

p本i正ate:
    // 動基本J務]基本sX
    正oid LoadDefa使lt軍o本設置ations(};
    // 基本p務池軍事
    float Calc使lateCo設置batDa設置a成e(const 軍Min成Co設置batUnit& Attacke本, const 軍Min成Co設置batUnit& Defende本};
    // 基本pXX
    float Calc使late輸入itChance(const 軍Min成Co設置batUnit& Attacke本, const 軍Min成Co設置batUnit& Defende本};
    // 基本pX動X
    float Calc使lateC本iticalChance(const 軍Min成Co設置batUnit& Attacke本};
    // 務sX
    正oid UpdateUnitState(int32 UnitID, EMin成Co設置batState 的ewState};
    // X動X
    正oid P本ocessUnitMo正e設置ent(float DeltaTi設置e};
    // 務池基本
    正oid P本ocessCo設置batLo成ic(float DeltaTi設置e};
    // 務s務成X
    正oid UpdateUnitExpe本ience(int32 UnitID, float Expe本ienceA設置o使nt};
    // 動dX動X
    bool IsUnitVisible(int32 Viewe本ID, int32 Ta本成etID) const;

    // 務sX動設置
    TA本本ay<軍Vecto本> Gene本ate軍o本設置ationPositions(EMin成軍o本設置ationType 軍o本設置ationType, const 軍Vecto本& Cente本, int32 UnitCo使nt) const;

    // 動X
    bool ValidateUnitData(const 軍Min成Co設置batUnit& Unit) const;
};

