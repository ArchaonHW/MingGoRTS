#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Laye本Inte成本ation/Min成軍o使本Laye本Types.h"
#incl使de "Min成B使ildin成Laye本Cont本olle本.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EMin成Laye本B使ildin成Type: 使int8 {
    輸入eadq使a本te本s UMETA(Display的a設置e = "輸入eadq使a本te本s"),
    Ba本本acks UMETA(Display的a設置e = "Ba本本acks"),
    軍acto本y UMETA(Display的a設置e = "軍acto本y"),
    A本設置o本y UMETA(Display的a設置e = "A本設置o本y"),
    基本a本eho使se UMETA(Display的a設置e = "基本a本eho使se"),
    Resea本chLab UMETA(Display的a設置e = "Resea本ch Lab"),
    MedicalStation UMETA(Display的a設置e = "Medical Station"),
    T本ainin成G本o使nd UMETA(Display的a設置e = "T本ainin成 G本o使nd"),
    DefenseTowe本 UMETA(Display的a設置e = "Defense Towe本"),
    基本all UMETA(Display的a設置e = "基本all"),
    Gene本ato本 UMETA(Display的a設置e = "Gene本ato本"),
    軍a本設置 UMETA(Display的a設置e = "軍a本設置"),
    Mine UMETA(Display的a設置e = "Mine"),
    基本o本kshop UMETA(Display的a設置e = "基本o本kshop")
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成B使ildStat使s: 使int8 {
    Planned UMETA(Display的a設置e = "Planned"),
    Unde本Const本使ction UMETA(Display的a設置e = "Unde本 Const本使ction"),
    Acti正e UMETA(Display的a設置e = "Acti正e"),
    Da設置a成ed UMETA(Display的a設置e = "Da設置a成ed"),
    Dest本oyed UMETA(Display的a設置e = "Dest本oyed"),
    Up成本adin成 UMETA(Display的a設置e = "Up成本adin成")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成B使ildin成Instance
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    軍St本in成 B使ildin成ID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    軍St本in成 B使ildin成的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    EMin成B使ildin成Type B使ildin成Type = EMin成B使ildin成Type::Ba本本acks;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    EMin成B使ildStat使s Stat使s = EMin成B使ildStat使s::Planned;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    軍Vecto本 Location;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    軍Rotato本 Rotation;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    int32 Le正el = 1;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    int32 MaxLe正el = 5;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    float 輸入ealth = 100.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    float Max輸入ealth = 100.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    float Const本使ctionP本o成本ess = 0.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    float B使ildTi設置e = 60.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    TMap<軍St本in成, int32> Reso使本ceCosts;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    int32 基本o本ke本Co使nt = 0;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    int32 Max基本o本ke本s = 10;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    TA本本ay<軍St本in成> P本od使cin成Units;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    TA本本ay<軍St本in成> Resea本chin成Techs;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    bool bIsSelected = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    軍St本in成 軍actionID;

    軍Min成B使ildin成Instance()
        : B使ildin成Type(EMin成B使ildin成Type::Ba本本acks)
        , Stat使s(EMin成B使ildStat使s::Planned)
        , Le正el(1)
        , MaxLe正el(5)
        , 輸入ealth(100.0f)
        , Max輸入ealth(100.0f)
        , Const本使ctionP本o成本ess(0.0f)
        , B使ildTi設置e(60.0f)
        , 基本o本ke本Co使nt(0)
        , Max基本o本ke本s(10)
        , bIsSelected(false)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成BaseLayo使t
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Base")
    軍St本in成 BaseID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Base")
    軍St本in成 Base的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Base")
    軍St本in成 軍actionID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Base")
    軍Vecto本 Cente本Location;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Base")
    float Radi使s = 1000.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Base")
    TA本本ay<軍St本in成> B使ildin成IDs;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Base")
    int32 Total基本o本ke本s = 0;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Base")
    int32 Max基本o本ke本s = 100;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Base")
    bool bIsUnde本Attack = false;

    軍Min成BaseLayo使t()
        : Radi使s(1000.0f)
        , Total基本o本ke本s(0)
        , Max基本o本ke本s(100)
        , bIsUnde本Attack(false)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成P本od使ctionQ使e使e
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "P本od使ction")
    軍St本in成 B使ildin成ID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "P本od使ction")
    TA本本ay<軍St本in成> Q使e使eIte設置s;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "P本od使ction")
    float C使本本entP本o成本ess = 0.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "P本od使ction")
    float P本od使ctionTi設置e = 30.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "P本od使ction")
    bool bIsPa使sed = false;

    軍Min成P本od使ctionQ使e使e()
        : C使本本entP本o成本ess(0.0f)
        , P本od使ctionTi設置e(30.0f)
        , bIsPa使sed(false)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Reso使本ce軍low
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce 軍low")
    TMap<軍St本in成, int32> Inco設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce 軍low")
    TMap<軍St本in成, int32> Expenses;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce 軍low")
    TMap<軍St本in成, int32> 的et軍low;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce 軍low")
    float Efficiency = 1.0f;
};

DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnB使ildin成Const本使cted, const 軍Min成B使ildin成Instance&, B使ildin成);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnB使ildin成Up成本aded, const 軍Min成B使ildin成Instance&, B使ildin成);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnB使ildin成Dest本oyed, const 軍St本in成&, B使ildin成ID);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnP本od使ctionCo設置pleted, const 軍St本in成&, UnitType);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnBaseAttacked, const 軍St本in成&, BaseID);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnB使ildin成Selected, const 軍St本in成&, B使ildin成ID, bool, bIsSelected);

/**
 * B使ildin成 Laye本 Cont本olle本
 * Mana成es base const本使ction, b使ildin成 設置ana成e設置ent, and 本eso使本ce p本od使ction
 */
UCLASS(ClassG本o使p = (B使ildin成), Bl使ep本intable)
class MI的GBUILDI的G下API UMin成B使ildin成Laye本Cont本olle本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成B使ildin成Laye本Cont本olle本();

    // Initialization
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Laye本")
    正oid InitializeB使ildin成Laye本();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Laye本")
    正oid Sh使tdownB使ildin成Laye本();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Laye本")
    正oid Acti正ateB使ildin成Laye本();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Laye本")
    正oid Deacti正ateB使ildin成Laye本();

    // Base Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Base Mana成e設置ent")
    正oid LoadBase(const 軍St本in成& BaseID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Base Mana成e設置ent")
    正oid C本eate的ewBase(const 軍St本in成& Base的a設置e, 軍Vecto本 Location, const 軍St本in成& 軍actionID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Base Mana成e設置ent")
    正oid Clea本Base();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Base Mana成e設置ent")
    bool GetC使本本entBase(軍Min成BaseLayo使t& O使tBase) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Base Mana成e設置ent")
    正oid SetBaseUnde本Attack(bool bUnde本Attack);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Base Mana成e設置ent")
    正oid DefendBase();

    // B使ildin成 Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Mana成e設置ent")
    軍St本in成 PlaceB使ildin成Bl使ep本int(EMin成B使ildin成Type Type, 軍Vecto本 Location, 軍Rotato本 Rotation);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Mana成e設置ent")
    正oid Sta本tConst本使ction(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Mana成e設置ent")
    正oid CancelConst本使ction(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Mana成e設置ent")
    正oid Co設置pleteConst本使ction(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Mana成e設置ent")
    正oid Up成本adeB使ildin成(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Mana成e設置ent")
    正oid De設置olishB使ildin成(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Mana成e設置ent")
    正oid Repai本B使ildin成(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Mana成e設置ent")
    bool GetB使ildin成(const 軍St本in成& B使ildin成ID, 軍Min成B使ildin成Instance& O使tB使ildin成) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Mana成e設置ent")
    TA本本ay<軍Min成B使ildin成Instance> GetAllB使ildin成s() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Mana成e設置ent")
    TA本本ay<軍Min成B使ildin成Instance> GetB使ildin成sByType(EMin成B使ildin成Type Type) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Mana成e設置ent")
    TA本本ay<軍Min成B使ildin成Instance> GetActi正eB使ildin成s() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Mana成e設置ent")
    TA本本ay<軍Min成B使ildin成Instance> GetUnde本Const本使ctionB使ildin成s() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Mana成e設置ent")
    正oid SelectB使ildin成(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Mana成e設置ent")
    正oid DeselectB使ildin成(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成 Mana成e設置ent")
    正oid Clea本B使ildin成Selection();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成 Mana成e設置ent")
    TA本本ay<軍St本in成> GetSelectedB使ildin成s() const { 本et使本n SelectedB使ildin成IDs; }

    // 基本o本ke本 Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本o本ke本s")
    正oid Assi成n基本o本ke本s(const 軍St本in成& B使ildin成ID, int32 基本o本ke本Co使nt);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本o本ke本s")
    正oid Re設置o正e基本o本ke本s(const 軍St本in成& B使ildin成ID, int32 基本o本ke本Co使nt);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本o本ke本s")
    正oid SetB使ildin成P本io本ity(const 軍St本in成& B使ildin成ID, int32 P本io本ity);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本o本ke本s")
    int32 GetA正ailable基本o本ke本s() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本o本ke本s")
    int32 GetTotal基本o本ke本s() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本o本ke本s")
    正oid 輸入i本e基本o本ke本s(int32 Co使nt);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本o本ke本s")
    正oid Reassi成n基本o本ke本s(const 軍St本in成& 軍本o設置B使ildin成ID, const 軍St本in成& ToB使ildin成ID, int32 Co使nt);

    // P本od使ction Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "P本od使ction")
    正oid Q使e使eUnitP本od使ction(const 軍St本in成& B使ildin成ID, const 軍St本in成& UnitType);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "P本od使ction")
    正oid CancelUnitP本od使ction(const 軍St本in成& B使ildin成ID, int32 Q使e使eIndex);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "P本od使ction")
    正oid Pa使seP本od使ction(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "P本od使ction")
    正oid Res使設置eP本od使ction(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "P本od使ction")
    正oid R使shP本od使ction(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "P本od使ction")
    軍Min成P本od使ctionQ使e使e GetP本od使ctionQ使e使e(const 軍St本in成& B使ildin成ID) const;

    // Reso使本ce Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ces")
    軍Min成Reso使本ce軍low Calc使lateReso使本ce軍low() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ces")
    int32 GetReso使本ceP本od使ction(const 軍St本in成& Reso使本ceType) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ces")
    int32 GetReso使本ceCons使設置ption(const 軍St本in成& Reso使本ceType) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ces")
    float GetPowe本P本od使ction() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ces")
    float GetPowe本Cons使設置ption() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Reso使本ces")
    bool 輸入asPowe本S使本pl使s() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ces")
    正oid Opti設置izeReso使本ceDist本ib使tion();

    // G本id & Place設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G本id")
    軍Vecto本 SnapToG本id(軍Vecto本 Location, float G本idSize = 100.0f);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G本id")
    bool IsValidB使ildLocation(軍Vecto本 Location, float Radi使s) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G本id")
    正oid SetB使ildRadi使s(float Radi使s);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G本id")
    正oid ShowB使ildableA本eas();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G本id")
    正oid 輸入ideB使ildableA本eas();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G本id")
    軍Vecto本 軍ind的ea本estValidLocation(軍Vecto本 Desi本edLocation, EMin成B使ildin成Type Type) const;

    // Ca設置e本a & View
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ca設置e本a")
    正oid 軍oc使sOnB使ildin成(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ca設置e本a")
    正oid 軍oc使sOnBaseCente本();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ca設置e本a")
    正oid SetO正e本正iewCa設置e本a();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ca設置e本a")
    正oid Set軍ollowCa設置e本a(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ca設置e本a")
    正oid Zoo設置Ca設置e本a(float A設置o使nt);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ca設置e本a")
    正oid PanCa設置e本a(軍Vecto本 Di本ection);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ca設置e本a")
    正oid RotateCa設置e本a(float An成le);

    // Laye本 Inte成本ation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Laye本 Inte成本ation")
    正oid Req使estSt本ate成icS使ppo本t(const 軍St本in成& S使ppo本tType);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Laye本 Inte成本ation")
    正oid SendReso使本cesToSt本ate成ic(TMap<軍St本in成, int32> Reso使本ces);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Laye本 Inte成本ation")
    正oid Recei正eUnit軍本o設置Tactical(const 軍St本in成& UnitID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Laye本 Inte成本ation")
    正oid Ret使本nToSt本ate成ic();

    // Utility 軍使nctions
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    int32 GetB使ildin成Li設置it() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    int32 GetC使本本entB使ildin成Co使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    float GetBaseDefenseRatin成() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    float GetBaseEfficiency() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Utility")
    正oid To成成leB使ildin成Visibility(EMin成B使ildin成Type Type, bool bVisible);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Utility")
    正oid To成成leG本idDisplay(bool bShow);

    // E正ents
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnB使ildin成Const本使cted OnB使ildin成Const本使cted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnB使ildin成Up成本aded OnB使ildin成Up成本aded;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnB使ildin成Dest本oyed OnB使ildin成Dest本oyed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnP本od使ctionCo設置pleted OnP本od使ctionCo設置pleted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnBaseAttacked OnBaseAttacked;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnB使ildin成Selected OnB使ildin成Selected;

p本otected:
    UPROPERTY()
    TMap<軍St本in成, 軍Min成B使ildin成Instance> B使ildin成s;

    UPROPERTY()
    軍Min成BaseLayo使t C使本本entBase;

    UPROPERTY()
    TMap<軍St本in成, 軍Min成P本od使ctionQ使e使e> P本od使ctionQ使e使es;

    UPROPERTY()
    TA本本ay<軍St本in成> SelectedB使ildin成IDs;

    UPROPERTY()
    bool bIsActi正e = false;

    UPROPERTY()
    float B使ildRadi使s = 2000.0f;

    UPROPERTY()
    bool bShowG本id = t本使e;

    UPROPERTY()
    int32 的extB使ildin成ID = 1;

    // Inte本nal f使nctions
    正oid InitializeDefa使ltB使ildin成s();
    正oid P本ocessConst本使ctionTick(float DeltaTi設置e);
    正oid P本ocessP本od使ctionTick(float DeltaTi設置e);
    正oid UpdateReso使本ceP本od使ction();
    正oid CheckB使ildin成輸入ealth();
    
    正oid OnB使ildin成Co設置pleted(const 軍St本in成& B使ildin成ID);
    正oid OnUnitP本od使ction軍inished(const 軍St本in成& B使ildin成ID, const 軍St本in成& UnitType);
    正oid OnB使ildin成Da設置a成ed(const 軍St本in成& B使ildin成ID, float Da設置a成eA設置o使nt);
    正oid OnBaseDefenseT本i成成e本ed();
    
    bool CanAffo本dB使ildin成(const 軍Min成B使ildin成Instance& B使ildin成) const;
    正oid Ded使ctB使ildin成Cost(const 軍Min成B使ildin成Instance& B使ildin成);
    正oid Ref使ndB使ildin成Cost(const 軍Min成B使ildin成Instance& B使ildin成);
    
    軍St本in成 Gene本ateB使ildin成ID();
    int32 Calc使lateMax基本o本ke本s(EMin成B使ildin成Type Type) const;
    float Calc使lateB使ildTi設置e(EMin成B使ildin成Type Type, int32 Le正el) const;
};

