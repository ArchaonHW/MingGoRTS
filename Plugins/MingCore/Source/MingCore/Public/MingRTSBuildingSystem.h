#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成RTSB使ildin成Syste設置.成ene本ated.h"

class AActo本;
class UMin成RTSReso使本ceMana成e本;
class UMin成RTSUnitMana成e本;

UE的UM(Bl使ep本intType)
en使設置 class ERTSB使ildin成Type: 使int8 {
    Residential,     // 住宅
    Co設置設置e本cial,      // 商業
    Ind使st本ial,      // 工業
    Milita本y,        // 軍事
    A成本ic使lt使本al,   // 農業
    Ed使cational,    // 教育
    Medical,         // 醫療
    Sto本a成e,         // 倉儲
    Powe本,           // 電力
    Defense,         // 防禦
    Reso使本ce,        // 資源
    Resea本ch,        // 研究
    Ente本tain設置ent,   // 娛樂
    Go正e本n設置ent,      // 政府
    Reli成io使s,       // 宗教
    C使sto設置          // 自定義
};

UE的UM(Bl使ep本intType)
en使設置 class ERTSB使ildin成State: 使int8 {
    Plannin成,        // 規劃中
    Unde本Const本使ction, // 建造中
    Ope本ational,     // 運作中
    Da設置a成ed,         // 受損
    Repai本in成,       // 維修中
    Up成本adin成,       // 升級中
    De設置olishin成,     // 拆除中
    Abandoned,       // 廢棄
    Dest本oyed        // 已
};

UE的UM(Bl使ep本intType)
en使設置 class ERTSB使ildin成Size: 使int8 {
    S設置all,           // 小型
    Medi使設置,          // 中型
    La本成e,           // 大型
    Ext本aLa本成e      // 超大型
};

USTRUCT(Bl使ep本intType)
st本使ct 軍RTSB使ildin成Data
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    軍St本in成 B使ildin成ID;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    軍St本in成 B使ildin成的a設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    ERTSB使ildin成Type B使ildin成Type;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    ERTSB使ildin成State B使ildin成State;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    ERTSB使ildin成Size B使ildin成Size;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    軍Vecto本 Location;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    軍Rotato本 Rotation;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    float 輸入ealth;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    float Max輸入ealth;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    float Const本使ctionP本o成本ess;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    float P本od使ctionRate;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    float MaintenanceCost;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    int32 基本o本ke本Capacity;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    int32 C使本本ent基本o本ke本s;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    TA本本ay<ERTSReso使本ceType> Req使i本edReso使本ces;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    TA本本ay<float> Reso使本ceCons使設置ption;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    TA本本ay<ERTSReso使本ceType> P本od使cedReso使本ces;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    TA本本ay<float> Reso使本ceP本od使ction;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    float Infl使enceRadi使s;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    float Rende本Distance;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    int32 C使本本entLOD;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    bool bIsVisible;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    bool bReq使i本esUpdate;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Data")
    軍DateTi設置e LastUpdateTi設置e;

    軍RTSB使ildin成Data()
    {
        B使ildin成ID = TEXT(""};
        B使ildin成的a設置e = TEXT(""};
        B使ildin成Type = ERTSB使ildin成Type::Residential;
        B使ildin成Size = ERTSB使ildin成Size::S設置all;
        Location = 軍Vecto本::Ze本oVecto本;
        Rotation = 軍Rotato本::Ze本oRotato本;
        B使ildin成State = ERTSB使ildin成State::Plannin成;
        輸入ealth = 100.0f;
        Max輸入ealth = 100.0f;
        Const本使ctionP本o成本ess = 0.0f;
        P本od使ctionRate = 1.0f;
        MaintenanceCost = 1.0f;
        基本o本ke本Capacity = 5;
        C使本本ent基本o本ke本s = 0;
        Infl使enceRadi使s = 300.0f;
        Rende本Distance = 1000.0f;
        C使本本entLOD = 0;
        bIsVisible = t本使e;
        bReq使i本esUpdate = t本使e;
        bIsPowe本ed = false;
        bIsConnected = false;
        LastUpdateTi設置e = 軍DateTi設置e::的ow(};
    }
};

// 政府基本數據基本]防禦
USTRUCT(Bl使ep本intType)
st本使ct 軍B使ildin成Pe本fo本設置anceSettin成s
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance Settin成s")
    bool bEnableLOD = t本使e;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance Settin成s")
    bool bEnableC使llin成 = t本使e;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance Settin成s")
    bool bEnableBatchin成 = t本使e;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance Settin成s")
    float Update軍本eq使ency = 60.0f;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance Settin成s")
    int32 MaxVisibleB使ildin成s = 100;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance Settin成s")
    float C使llin成Distance = 2000.0f;

    軍B使ildin成Pe本fo本設置anceSettin成s()
    {
        bEnableLOD = t本使e;
        bEnableC使llin成 = t本使e;
        bEnableBatchin成 = t本使e;
        Update軍本eq使ency = 60.0f;
        MaxVisibleB使ildin成s = 100;
        C使llin成Distance = 2000.0f;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍RTSB使ildin成Te設置plate
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Te設置plate")
    軍St本in成 Te設置plateID;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Te設置plate")
    軍St本in成 Te設置plate的a設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Te設置plate")
    ERTSB使ildin成Type B使ildin成Type;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Te設置plate")
    ERTSB使ildin成Size B使ildin成Size;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Te設置plate")
    float Const本使ctionCost;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Te設置plate")
    float Const本使ctionTi設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Te設置plate")
    TA本本ay<ERTSReso使本ceType> Req使i本edReso使本ces;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Te設置plate")
    TA本本ay<float> Reso使本ceCosts;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Te設置plate")
    軍St本in成 Bl使ep本intPath;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Te設置plate")
    軍St本in成 Desc本iption;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Te設置plate")
    bool bIsA正ailable;

    軍RTSB使ildin成Te設置plate()
    {
        Te設置plateID = TEXT(""};
        Te設置plate的a設置e = TEXT(""};
        B使ildin成Type = ERTSB使ildin成Type::Residential;
        B使ildin成Size = ERTSB使ildin成Size::S設置all;
        Const本使ctionCost = 100.0f;
        Const本使ctionTi設置e = 60.0f;
        Bl使ep本intPath = TEXT(""};
        Desc本iption = TEXT(""};
        bIsA正ailable = t本使e;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍RTSB使ildin成Up成本ade
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Up成本ade")
    軍St本in成 Up成本adeID;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Up成本ade")
    軍St本in成 Up成本ade的a設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Up成本ade")
    軍St本in成 Desc本iption;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Up成本ade")
    float Up成本adeCost;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Up成本ade")
    float Up成本adeTi設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Up成本ade")
    TA本本ay<ERTSReso使本ceType> Req使i本edReso使本ces;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Up成本ade")
    TA本本ay<float> Reso使本ceCosts;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Up成本ade")
    TA本本ay<軍St本in成> Effects;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "B使ildin成 Up成本ade")
    bool bIsA正ailable;

    軍RTSB使ildin成Up成本ade()
    {
        Up成本adeID = TEXT(""};
        Up成本ade的a設置e = TEXT(""};
        Desc本iption = TEXT(""};
        Up成本adeCost = 50.0f;
        Up成本adeTi設置e = 30.0f;
        bIsA正ailable = t本使e;
    }
};

/**
 * RTS基本數據建進
 * 基本數據宗數據進
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GCORE下API UMin成RTSB使ildin成Syste設置 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成RTSB使ildin成Syste設置(};

    
    正oid InitializeB使ildin成Syste設置(UMin成RTSReso使本ceMana成e本* InReso使本ceMana成e本, UMin成RTSUnitMana成e本* InUnitMana成e本};

    // 建築基本數據
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    軍St本in成 C本eateB使ildin成(const 軍RTSB使ildin成Te設置plate& Te設置plate, const 軍Vecto本& Location, const 軍Rotato本& Rotation) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    bool Dest本oyB使ildin成(const 軍St本in成& B使ildin成ID) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    軍RTSB使ildin成Data GetB使ildin成(const 軍St本in成& B使ildin成ID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    TA本本ay<軍RTSB使ildin成Data> GetAllB使ildin成s() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    TA本本ay<軍RTSB使ildin成Data> GetB使ildin成sByType(ERTSB使ildin成Type B使ildin成Type) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    TA本本ay<軍RTSB使ildin成Data> GetB使ildin成sByState(ERTSB使ildin成State B使ildin成State) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    TA本本ay<軍RTSB使ildin成Data> GetB使ildin成sInA本ea(const 軍Vecto本& Cente本, float Radi使s) const;

    
    正oid SetB使ildin成State(const 軍St本in成& B使ildin成ID, ERTSB使ildin成State 的ewState};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    ERTSB使ildin成State GetB使ildin成State(const 軍St本in成& B使ildin成ID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    bool IsB使ildin成Ope本ational(const 軍St本in成& B使ildin成ID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid Sta本tConst本使ction(const 軍St本in成& B使ildin成ID) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid StopConst本使ction(const 軍St本in成& B使ildin成ID) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid Co設置pleteConst本使ction(const 軍St本in成& B使ildin成ID) {};

    // 建築
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    bool Up成本adeB使ildin成(const 軍St本in成& B使ildin成ID, const 軍RTSB使ildin成Up成本ade& Up成本ade) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    bool CanUp成本adeB使ildin成(const 軍St本in成& B使ildin成ID, const 軍RTSB使ildin成Up成本ade& Up成本ade) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    TA本本ay<軍RTSB使ildin成Up成本ade> GetA正ailableUp成本ades(const 軍St本in成& B使ildin成ID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid AddB使ildin成Up成本ade(const 軍St本in成& B使ildin成ID, const 軍RTSB使ildin成Up成本ade& Up成本ade) {};

    // 建築進基本@
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid Repai本B使ildin成(const 軍St本in成& B使ildin成ID, float Repai本A設置o使nt};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid Da設置a成eB使ildin成(const 軍St本in成& B使ildin成ID, float Da設置a成eA設置o使nt};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    float GetB使ildin成輸入ealth(const 軍St本in成& B使ildin成ID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    float GetB使ildin成輸入ealthPe本centa成e(const 軍St本in成& B使ildin成ID) const;

    // 建築進
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid UpdateB使ildin成P本od使ction(float DeltaTi設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    float GetB使ildin成P本od使ctionRate(const 軍St本in成& B使ildin成ID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid SetB使ildin成P本od使ctionRate(const 軍St本in成& B使ildin成ID, float Rate};

    // 建築t數據基本數據
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid EnhanceB使ildin成Syste設置(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid Opti設置izeB使ildin成Layo使t(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid I設置p本o正eConst本使ctionEfficiency(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid I設置ple設置entS設置a本tB使ildin成Mana成e設置ent(};

    // 分配工人
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    bool Assi成n基本o本ke本s(const 軍St本in成& B使ildin成ID, int32 基本o本ke本Co使nt};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid Re設置o正e基本o本ke本s(const 軍St本in成& B使ildin成ID, int32 基本o本ke本Co使nt};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    int32 GetA正ailable基本o本ke本Capacity(const 軍St本in成& B使ildin成ID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    bool 輸入asEno使成h基本o本ke本s(const 軍St本in成& B使ildin成ID) const;

    // 建築進O
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid AddB使ildin成Te設置plate(const 軍RTSB使ildin成Te設置plate& Te設置plate) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid Re設置o正eB使ildin成Te設置plate(const 軍St本in成& Te設置plateID) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    軍RTSB使ildin成Te設置plate GetB使ildin成Te設置plate(const 軍St本in成& Te設置plateID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    TA本本ay<軍RTSB使ildin成Te設置plate> GetAllB使ildin成Te設置plates() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    TA本本ay<軍RTSB使ildin成Te設置plate> GetB使ildin成Te設置platesByType(ERTSB使ildin成Type B使ildin成Type) const;

    // 建築防禦
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    bool CanPlaceB使ildin成(const 軍RTSB使ildin成Te設置plate& Te設置plate, const 軍Vecto本& Location) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    bool IsLocationOcc使pied(const 軍Vecto本& Location, float Radi使s) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    軍Vecto本 軍indBestLocation(const 軍RTSB使ildin成Te設置plate& Te設置plate, const 軍Vecto本& P本efe本本edLocation) const;

    // 建築
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid ConnectB使ildin成(const 軍St本in成& B使ildin成ID, const 軍St本in成& Ta本成etB使ildin成ID) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid DisconnectB使ildin成(const 軍St本in成& B使ildin成ID, const 軍St本in成& Ta本成etB使ildin成ID) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    bool IsB使ildin成Connected(const 軍St本in成& B使ildin成ID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    TA本本ay<軍St本in成> GetConnectedB使ildin成s(const 軍St本in成& B使ildin成ID) const;

    // 建築基本數據
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "RTS B使ildin成 Syste設置")
    int32 GetTotalB使ildin成Co使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "RTS B使ildin成 Syste設置")
    int32 GetB使ildin成Co使ntByType(ERTSB使ildin成Type B使ildin成Type) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "RTS B使ildin成 Syste設置")
    int32 GetOpe本ationalB使ildin成Co使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "RTS B使ildin成 Syste設置")
    float GetTotalB使ildin成Val使e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "RTS B使ildin成 Syste設置")
    float GetTotalMaintenanceCost() const;

    // 建築進基本
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid OnB使ildin成Attacked(const 軍St本in成& B使ildin成ID, float Da設置a成e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid OnB使ildin成Capt使本ed(const 軍St本in成& B使ildin成ID, const 軍St本in成& 的ewOwne本) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid OnReso使本ceDepleted(const 軍St本in成& B使ildin成ID) {};

    // 拆除中
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "RTS B使ildin成 E正ents")
    軍OnB使ildin成Const本使cted OnB使ildin成Const本使cted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "RTS B使ildin成 E正ents")
    軍OnB使ildin成Dest本oyed OnB使ildin成Dest本oyed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "RTS B使ildin成 E正ents")
    軍OnB使ildin成StateChan成ed OnB使ildin成StateChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "RTS B使ildin成 E正ents")
    軍OnB使ildin成Up成本aded OnB使ildin成Up成本aded;

p本otected:
    // t�V進
    UPROPERTY()
    TOb大ectPt本<UMin成RTSReso使本ceMana成e本> Reso使本ceMana成e本;

    UPROPERTY()
    TOb大ectPt本<UMin成RTSUnitMana成e本> UnitMana成e本;

    // 建築
    UPROPERTY()
    TMap<軍St本in成, 軍RTSB使ildin成Data> B使ildin成s;

    UPROPERTY()
    TMap<軍St本in成, 軍RTSB使ildin成Te設置plate> B使ildin成Te設置plates;

    
    TMap<軍St本in成, TA本本ay<軍RTSB使ildin成Up成本ade>> B使ildin成Up成本ades;

    
    TMap<軍St本in成, TA本本ay<軍St本in成>> B使ildin成Connections;

    // 設置
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成 Settin成s")
    float Const本使ctionSpeedM使ltiplie本 = 1.0f;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成 Settin成s")
    float MaintenanceM使ltiplie本 = 1.0f;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成 Settin成s")
    float Repai本SpeedM使ltiplie本 = 1.0f;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成 Settin成s")
    bool bA使toRepai本 = t本使e;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "B使ildin成 Settin成s")
    bool bA使toUp成本ade = false;

    // 建築進
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Enhance設置ent Settin成s")
    float EfficiencyBon使s = 1.2f;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Enhance設置ent Settin成s")
    float Layo使tOpti設置ization軍acto本 = 1.1f;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Enhance設置ent Settin成s")
    bool bEnableS設置a本tMana成e設置ent = t本使e;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Enhance設置ent Settin成s")
    bool bEnableA使toLayo使t = t本使e;

    
    bool bIsInitialized;

    // 政府基本數據進
    正oid InitializeDefa使ltTe設置plates(};
    正oid P本ocessB使ildin成Const本使ction(軍RTSB使ildin成Data& B使ildin成, float DeltaTi設置e};
    正oid P本ocessB使ildin成P本od使ction(軍RTSB使ildin成Data& B使ildin成, float DeltaTi設置e};
    正oid P本ocessB使ildin成Maintenance(軍RTSB使ildin成Data& B使ildin成, float DeltaTi設置e};
    正oid UpdateB使ildin成Connections(const 軍St本in成& B使ildin成ID) {};
    軍St本in成 Gene本ateB使ildin成ID(ERTSB使ildin成Type B使ildin成Type) const;
    軍St本in成 GetB使ildin成的a設置e(ERTSB使ildin成Type B使ildin成Type) const;
    float GetB使ildin成SizeM使ltiplie本(ERTSB使ildin成Size B使ildin成Size) const;
    bool ValidateB使ildin成Place設置ent(const 軍RTSB使ildin成Te設置plate& Te設置plate, const 軍Vecto本& Location) const;
    正oid ApplyB使ildin成Effects(const 軍St本in成& B使ildin成ID, const 軍RTSB使ildin成Up成本ade& Up成本ade) {};
    正oid CheckB使ildin成Inte成本ity(軍RTSB使ildin成Data& B使ildin成};

    // 建築基本數據進
    正oid Opti設置izeB使ildin成Efficiency(};
    正oid Calc使lateOpti設置alLayo使t(};
    正oid I設置p本o正eConst本使ctionSpeed(};
    正oid I設置ple設置entS設置a本tMaintenance(};
    float Calc使lateB使ildin成Efficiency(const 軍St本in成& B使ildin成ID) const;
    正oid A使toO本成anizeB使ildin成s(};
    正oid Opti設置izeReso使本ce軍low(};

// 政府基本數據
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid Opti設置izeLa本成eScaleB使ildin成Mana成e設置ent(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid Opti設置izeMe設置o本yUsa成e(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid Opti設置izeRende本in成Pe本fo本設置ance(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid Opti設置ize的etwo本kSynch本onization(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid EnablePe本fo本設置anceMode(bool bEnable};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "RTS B使ildin成 Syste設置")
    float GetB使ildin成Syste設置Pe本fo本設置ance() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "RTS B使ildin成 Syste設置")
    int32 GetActi正eB使ildin成Co使nt() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid UpdateB使ildin成LOD(const 軍St本in成& B使ildin成ID, int32 LODLe正el};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS B使ildin成 Syste設置")
    正oid BatchUpdateB使ildin成s(const TA本本ay<軍St本in成>& B使ildin成IDs};

p本i正ate:
    // 拆除中UX進
    正oid 的otifyB使ildin成Const本使cted(const 軍St本in成& B使ildin成ID, ERTSB使ildin成Type B使ildin成Type};
    正oid 的otifyB使ildin成Dest本oyed(const 軍St本in成& B使ildin成ID, ERTSB使ildin成Type B使ildin成Type};
    正oid 的otifyB使ildin成StateChan成ed(const 軍St本in成& B使ildin成ID, ERTSB使ildin成State 的ewState};
    正oid 的otifyB使ildin成Up成本aded(const 軍St本in成& B使ildin成ID, const 軍St本in成& Up成本adeID) {};

    // 政府基本數據基本數據進
    正oid Opti設置izeB使ildin成DataSt本使ct使本es(};
    正oid I設置ple設置entB使ildin成Poolin成(};
    正oid Opti設置izeB使ildin成Updates(};
    正oid Red使ceMe設置o本y軍ootp本int(};
    正oid Opti設置izeRende本in成Calls(};
    正oid I設置ple設置ent的etwo本kDeltaCo設置p本ession(};
};

