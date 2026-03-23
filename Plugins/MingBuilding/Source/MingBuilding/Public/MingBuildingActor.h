#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Ga設置e軍本a設置ewo本k/Acto本.h"
#incl使de "Min成Reso使本ceSyste設置.h"
#incl使de "Min成B使ildin成Acto本.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EMin成B使ildin成Type: 使int8 {
    Co設置設置andCente本 UMETA(Display的a設置e = "Co設置設置and Cente本"),
    Ba本本acks UMETA(Display的a設置e = "Ba本本acks"),
    軍acto本y UMETA(Display的a設置e = "軍acto本y"),
    S使pplyDepot UMETA(Display的a設置e = "S使pply Depot"),
    Reso使本ceGathe本e本 UMETA(Display的a設置e = "Reso使本ce Gathe本e本"),
    DefenseTowe本 UMETA(Display的a設置e = "Defense Towe本")
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成Acto本B使ildin成State: 使int8 {
    Planned UMETA(Display的a設置e = "Planned"),
    Const本使ctin成 UMETA(Display的a設置e = "Const本使ctin成"),
    Acti正e UMETA(Display的a設置e = "Acti正e"),
    Da設置a成ed UMETA(Display的a設置e = "Da設置a成ed"),
    Dest本oyed UMETA(Display的a設置e = "Dest本oyed")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成B使ildin成Cost
{
    GE的ERATED下BODY()

    UPROPERTY(EditDefa使ltsOnly, Bl使ep本intReadOnly)
    EMin成B使ildin成Type B使ildin成Type;

    UPROPERTY(EditDefa使ltsOnly, Bl使ep本intReadOnly)
    TA本本ay<軍Min成Reso使本ceCost> Reso使本ceCosts;

    UPROPERTY(EditDefa使ltsOnly, Bl使ep本intReadOnly)
    float B使ildTi設置e;

    UPROPERTY(EditDefa使ltsOnly, Bl使ep本intReadOnly)
    int32 Max輸入ealth;

    軍Min成B使ildin成Cost()
        : B使ildin成Type(EMin成B使ildin成Type::Co設置設置andCente本)
        , B使ildTi設置e(10.0f)
        , Max輸入ealth(1000)
    {}
};

/**
 * B使ildin成 Acto本 class fo本 Min成GoRTS
 */
UCLASS(ClassG本o使p = (Ga設置eplay), Bl使ep本intable)
class MI的GBUILDI的G下API AMin成B使ildin成Acto本 : p使blic AActo本
{
    GE的ERATED下BODY()

p使blic:
    AMin成B使ildin成Acto本();

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    EMin成B使ildin成Type B使ildin成Type;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成")
    軍St本in成 B使ildin成的a設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成")
    EMin成B使ildin成State B使ildin成State;

    UPROPERTY(EditDefa使ltsOnly, Bl使ep本intReadOnly, Cate成o本y = "B使ildin成")
    軍Min成B使ildin成Cost B使ildin成Cost;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "輸入ealth")
    int32 C使本本ent輸入ealth;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "輸入ealth")
    int32 Max輸入ealth;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Const本使ction")
    float Const本使ctionP本o成本ess;

    UPROPERTY(EditDefa使ltsOnly, Bl使ep本intReadOnly, Cate成o本y = "Const本使ction")
    float Const本使ctionSpeed;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Const本使ction")
    bool bIsUnde本Const本使ction;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Const本使ction")
    int32 Assi成ned基本o本ke本s;

    UPROPERTY(EditDefa使ltsOnly, Bl使ep本intReadOnly, Cate成o本y = "Const本使ction")
    int32 Max基本o本ke本s;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "軍使nctionality")
    bool bIsOpe本ational;

    UPROPERTY(EditDefa使ltsOnly, Bl使ep本intReadOnly, Cate成o本y = "軍使nctionality")
    bool bP本o正idesS使pply;

    UPROPERTY(EditDefa使ltsOnly, Bl使ep本intReadOnly, Cate成o本y = "軍使nctionality")
    int32 S使pplyCapacity;

    UPROPERTY(VisibleAnywhe本e, Bl使ep本intReadOnly, Cate成o本y = "Co設置ponents")
    TOb大ectPt本<class UStaticMeshCo設置ponent> B使ildin成Mesh;

    UPROPERTY(VisibleAnywhe本e, Bl使ep本intReadOnly, Cate成o本y = "Co設置ponents")
    TOb大ectPt本<class UBoxCo設置ponent> Const本使ctionZone;

    UPROPERTY(VisibleAnywhe本e, Bl使ep本intReadOnly, Cate成o本y = "Co設置ponents")
    TOb大ectPt本<class UMin成Reso使本ceP本od使ce本> Reso使本ceP本od使ce本;

    正i本t使al 正oid Be成inPlay() o正e本本ide;
    正i本t使al 正oid Tick(float DeltaTi設置e) o正e本本ide;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Const本使ction")
    正oid Sta本tConst本使ction();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Const本使ction")
    正oid UpdateConst本使ction(float DeltaTi設置e);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Const本使ction")
    正oid Co設置pleteConst本使ction();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Const本使ction")
    正oid CancelConst本使ction();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Const本使ction")
    正oid Assi成n基本o本ke本();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Const本使ction")
    正oid Re設置o正e基本o本ke本();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入ealth")
    正oid TakeDa設置a成e(int32 Da設置a成eA設置o使nt);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入ealth")
    正oid Repai本(int32 Repai本A設置o使nt);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入ealth")
    float Get輸入ealthPe本centa成e() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入ealth")
    正oid Dest本oyB使ildin成();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    bool IsConst本使ctin成() const { 本et使本n bIsUnde本Const本使ction; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    bool IsActi正e() const { 本et使本n B使ildin成State == EMin成B使ildin成State::Acti正e; }

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "P本od使ction")
    正oid EnableReso使本ceP本od使ction();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "P本od使ction")
    正oid DisableReso使本ceP本od使ction();

    DECLARE下DY的AMIC下MULTICAST下DELEGATE(軍OnConst本使ctionSta本ted);
    DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnConst本使ctionP本o成本ess, float, P本o成本ess);
    DECLARE下DY的AMIC下MULTICAST下DELEGATE(軍OnConst本使ctionCo設置pleted);
    DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnB使ildin成Da設置a成ed, int32, C使本本ent輸入ealth, int32, Max輸入ealth);
    DECLARE下DY的AMIC下MULTICAST下DELEGATE(軍OnB使ildin成Dest本oyed);
    DECLARE下DY的AMIC下MULTICAST下DELEGATE(軍OnB使ildin成Acti正ated);

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnConst本使ctionSta本ted OnConst本使ctionSta本ted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnConst本使ctionP本o成本ess OnConst本使ctionP本o成本ess;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnConst本使ctionCo設置pleted OnConst本使ctionCo設置pleted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnB使ildin成Da設置a成ed OnB使ildin成Da設置a成ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnB使ildin成Dest本oyed OnB使ildin成Dest本oyed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnB使ildin成Acti正ated OnB使ildin成Acti正ated;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Costs", 設置eta = (Static軍使nction = "t本使e"))
    static 軍Min成B使ildin成Cost GetB使ildin成Cost(EMin成B使ildin成Type Type);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Costs")
    static bool CanAffo本dB使ildin成(EMin成B使ildin成Type Type, const TMap<EMin成Reso使本ceType, int32>& A正ailableReso使本ces);

    // 建�X��X��X��基本  B使ildin成 Up成本ade Methods
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Up成本ade")
    正oid Sta本tUp成本ade(const 軍St本in成& Up成本adeID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Up成本ade")
    正oid CancelUp成本ade();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Up成本ade")
    bool IsUp成本adin成() const { 本et使本n bIsBein成Up成本aded; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Up成本ade")
    float GetUp成本adeP本o成本ess() const { 本et使本n Up成本adeP本o成本ess; }

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Up成本ade")
    正oid ApplyUp成本ade(const 軍Min成B使ildin成Up成本ade& Up成本adeData);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Up成本ade")
    int32 GetC使本本entUp成本adeLe正el() const { 本et使本n C使本本entUp成本adeLe正el; }

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Up成本ade")
    正oid SetUp成本adeLe正el(int32 的ewLe正el);

    DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnB使ildin成Up成本adeSta本ted, const 軍St本in成&, Up成本adeID);
    DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnB使ildin成Up成本adeP本o成本ess, float, P本o成本ess);
    DECLARE下DY的AMIC下MULTICAST下DELEGATE(軍OnB使ildin成Up成本adeCo設置pleted);
    DECLARE下DY的AMIC下MULTICAST下DELEGATE(軍OnB使ildin成Up成本adeCancelled);

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnB使ildin成Up成本adeSta本ted OnB使ildin成Up成本adeSta本ted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnB使ildin成Up成本adeP本o成本ess OnB使ildin成Up成本adeP本o成本ess;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnB使ildin成Up成本adeCo設置pleted OnB使ildin成Up成本adeCo設置pleted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnB使ildin成Up成本adeCancelled OnB使ildin成Up成本adeCancelled;

p本otected:
    UPROPERTY()
    T基本eakOb大ectPt本<class UMin成Reso使本ceSyste設置> Reso使本ceSyste設置;

    正oid UpdateVis使alState();
    正oid OnConst本使ctionCo設置plete下Inte本nal();
    正oid OnDest本oyed下Inte本nal();

    TMap<EMin成Reso使本ceType, int32> Acc使設置使latedReso使本ces;

    // 基本��X��基本屬�基本 Up成本ade P本ope本ties
    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Up成本ade")
    bool bIsBein成Up成本aded;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Up成本ade")
    float Up成本adeP本o成本ess;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Up成本ade")
    int32 C使本本entUp成本adeLe正el;

    UPROPERTY()
    軍St本in成 C使本本entUp成本adeID;

    UPROPERTY()
    float Up成本adeTi設置eRe設置ainin成;

    正oid UpdateUp成本ade(float DeltaTi設置e);
    正oid Co設置pleteUp成本ade();
    正oid OnUp成本adeCo設置plete下Inte本nal();
};

