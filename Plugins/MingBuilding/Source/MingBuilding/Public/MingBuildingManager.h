#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成B使ildin成Acto本.h"
#incl使de "Min成B使ildin成Mana成e本.成ene本ated.h"

/**
 * 建�X�置結�基本
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍B使ildin成Place設置entRes使lt
{
    GE的ERATED下BODY()

    // 基本�否基本��基本
    UPROPERTY(Bl使ep本intReadOnly)
    bool bS使ccess;

    // 基本�誤信息
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 E本本o本Messa成e;

    // 基本�置位置
    UPROPERTY(Bl使ep本intReadOnly)
    軍Vecto本 Place設置entLocation;

    // 建�基本ID
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 B使ildin成ID;

    軍B使ildin成Place設置entRes使lt()
        : bS使ccess(false)
        , Place設置entLocation(軍Vecto本::Ze本oVecto本)
        , B使ildin成ID(TEXT(""))
    {}
};

/**
 * 建�X��X�目
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍B使ildin成Q使e使eIte設置
{
    GE的ERATED下BODY()

    // 建�基本類�基本
    UPROPERTY(Bl使ep本intReadOnly)
    EMin成B使ildin成Type B使ildin成Type;

    // 建�基本位置
    UPROPERTY(Bl使ep本intReadOnly)
    軍Vecto本 Position;

    // 建�X��基本
    UPROPERTY(Bl使ep本intReadOnly)
    軍Rotato本 Rotation;

    // 基本��基本ID
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Q使e使eID;

    // 基本��基本完�X��基本
    UPROPERTY(Bl使ep本intReadOnly)
    float Esti設置atedCo設置pletionTi設置e;

    // 基本��X�度
    UPROPERTY(Bl使ep本intReadOnly)
    float P本o成本ess;

    // 基本�否�基本��建�基本
    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsConst本使ctin成;

    軍B使ildin成Q使e使eIte設置()
        : B使ildin成Type(EMin成B使ildin成Type::Co設置設置andCente本)
        , Position(軍Vecto本::Ze本oVecto本)
        , Rotation(軍Rotato本::Ze本oRotato本)
        , Esti設置atedCo設置pletionTi設置e(0.0f)
        , P本o成本ess(0.0f)
        , bIsConst本使ctin成(false)
    {}
};

/**
 * 建�基本統�基本信息
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍B使ildin成Statistics
{
    GE的ERATED下BODY()

    // 總建築數X
    UPROPERTY(Bl使ep本intReadOnly)
    int32 TotalB使ildin成s;

    // 活�基本建�X��基本
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Acti正eB使ildin成s;

    // 建造中建�X��基本
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Const本使ctin成B使ildin成s;

    // 基本��基本建�X��基本
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Da設置a成edB使ildin成s;

    // 基本��X�建築數X
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<EMin成B使ildin成Type, int32> B使ildin成Co使nts;

    // 總工人數X
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Total基本o本ke本s;

    // 基本�置工人基本��基本
    UPROPERTY(Bl使ep本intReadOnly)
    int32 A正ailable基本o本ke本s;

    // 建造�X�長�基本
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Q使e使eLen成th;

    軍B使ildin成Statistics()
        : TotalB使ildin成s(0)
        , Acti正eB使ildin成s(0)
        , Const本使ctin成B使ildin成s(0)
        , Da設置a成edB使ildin成s(0)
        , Total基本o本ke本s(0)
        , A正ailable基本o本ke本s(0)
        , Q使e使eLen成th(0)
    {}
};

/**
 * 建�基本層管基本�器
 * 管�X�地建設基本��基本源�X */
UCLASS(Bl使ep本intType)
class MI的GBUILDI的G下API UMin成B使ildin成Mana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成B使ildin成Mana成e本();

    正i本t使al 正oid Initialize();
    正i本t使al 正oid Sh使tdown();

    正oid Set使pE正entS使bsc本iptions();
    正oid Clean使pE正entS使bsc本iptions();

    // === 建�X�置基本�建X===
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成")
    軍B使ildin成Place設置entRes使lt PlaceB使ildin成(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Location, const 軍Rotato本& Rotation);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成")
    bool Sta本tConst本使ction(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成")
    bool CancelConst本使ction(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成")
    軍St本in成 AddToConst本使ctionQ使e使e(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Location, const 軍Rotato本& Rotation);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成")
    bool Re設置o正e軍本o設置Const本使ctionQ使e使e(const 軍St本in成& Q使e使eID);

    // === 建�基本管�基本 ===
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成")
    AMin成B使ildin成Acto本* GetB使ildin成(const 軍St本in成& B使ildin成ID) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成")
    TA本本ay<AMin成B使ildin成Acto本*> GetAllB使ildin成s() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成")
    TA本本ay<AMin成B使ildin成Acto本*> GetB使ildin成sByType(EMin成B使ildin成Type B使ildin成Type) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成")
    TA本本ay<軍B使ildin成Q使e使eIte設置> GetConst本使ctionQ使e使e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成")
    軍B使ildin成Statistics GetB使ildin成Statistics() const;

    // === 建�X�置驗�基本 ===
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成")
    bool IsValidPlace設置entLocation(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Location) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成")
    軍Vecto本 GetB使ildin成G本idSize(EMin成B使ildin成Type B使ildin成Type) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成")
    正oid ShowPlace設置entP本e正iew(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Location, const 軍Rotato本& Rotation);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成")
    正oid 輸入idePlace設置entP本e正iew();

    // === 工人管�基本 ===
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成")
    bool Assi成n基本o本ke本(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成")
    bool Re設置o正e基本o本ke本(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成")
    int32 GetA正ailable基本o本ke本s() const;

    // === 建�X��X�維�基本===
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成")
    bool Up成本adeB使ildin成下I設置pl(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成")
    bool Dest本oyB使ildin成(const 軍St本in成& B使ildin成ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成")
    bool Repai本B使ildin成(const 軍St本in成& B使ildin成ID, int32 Repai本A設置o使nt);

    // === 建�X�詢 ===
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成")
    TA本本ay<AMin成B使ildin成Acto本*> GetB使ildin成sInRadi使s(const 軍Vecto本& Cente本, float Radi使s) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成")
    TA本本ay<AMin成B使ildin成Acto本*> GetS使pplyChain(const 軍St本in成& B使ildin成ID) const;

    // === 基本��基本介面 (基本�容X ===
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成")
    正oid Const本使ctB使ildin成(const 軍St本in成& B使ildin成Type, const 軍Vecto本& Position);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成")
    正oid Up成本adeB使ildin成下I設置pl(int32 B使ildin成Id);

    // === 資�基本管�基本 ===
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ce")
    正oid AddReso使本ce(const 軍St本in成& Reso使本ceType, int32 A設置o使nt);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Reso使本ce")
    int32 GetReso使本ceA設置o使nt(const 軍St本in成& Reso使本ceType) const;

p本otected:
    正oid OnB使ildin成Const本使cted(const 軍St本in成& B使ildin成Type);
    正oid OnReso使本ceUpdated(const 軍St本in成& Reso使本ceType, int32 的ewA設置o使nt);

    // === 基本��X�內基本�方�基本===
    正oid P本ocessConst本使ctionQ使e使e(float DeltaTi設置e);
    正oid Sta本t的extConst本使ction();
    軍St本in成 Gene本ateB使ildin成ID();
    軍St本in成 Gene本ateQ使e使eID();
    bool 輸入asEno使成hReso使本ces(EMin成B使ildin成Type B使ildin成Type) const;
    bool Cons使設置eConst本使ctionReso使本ces(EMin成B使ildin成Type B使ildin成Type);
    正oid Ref使ndConst本使ctionReso使本ces(EMin成B使ildin成Type B使ildin成Type);
    正oid UpdateB使ildin成Statistics();
    正oid OnB使ildin成Const本使ctionSta本ted(const 軍St本in成& B使ildin成ID);
    正oid OnB使ildin成Const本使ctionCo設置pleted(const 軍St本in成& B使ildin成ID);
    正oid OnB使ildin成Dest本oyed(const 軍St本in成& B使ildin成ID);
    正oid Sa正eB使ildin成Data();
    正oid LoadB使ildin成Data();
    正oid Clean使pPlace設置entP本e正iew();
    bool CheckB使ildin成Collision(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Location) const;
    bool IsTe本本ainS使itable(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Location) const;
    float Calc使lateB使ildin成Efficiency(AMin成B使ildin成Acto本* B使ildin成) const;
    正oid UpdateS使pplyChains();
    正oid P本ocessB使ildin成Up成本ades(float DeltaTi設置e);

p本i正ate:
    bool bIsInitialized;
    
    // === 基本��基本資�基本存儲 (基本�容X ===
    TMap<軍St本in成, int32> Reso使本ceIn正ento本y;

    // === 基本��X�建築管基本�數X===
    UPROPERTY()
    TA本本ay<TOb大ectPt本<AMin成B使ildin成Acto本>> B使ildin成s;

    UPROPERTY()
    TA本本ay<軍B使ildin成Q使e使eIte設置> Const本使ctionQ使e使e;

    UPROPERTY()
    TMap<軍St本in成, TOb大ectPt本<AMin成B使ildin成Acto本>> B使ildin成IDMap;

    UPROPERTY()
    軍St本in成 C使本本entConst本使ctionID;

    UPROPERTY(EditAnywhe本e, Cate成o本y = "B使ildin成 Mana成e本")
    int32 Total基本o本ke本s;

    UPROPERTY(EditAnywhe本e, Cate成o本y = "B使ildin成 Mana成e本")
    int32 MaxConc使本本entConst本使ction;

    UPROPERTY()
    TOb大ectPt本<class UStaticMeshCo設置ponent> Place設置entP本e正iew;
};

