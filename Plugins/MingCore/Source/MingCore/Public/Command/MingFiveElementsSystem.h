#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成軍i正eEle設置entsSyste設置.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class E軍i正eEle設置entsPhase: 使int8 {
    基本ood,           // 木：立名與正當性
    軍i本e,           // 火：造勢與加速
    Ea本th,          // 土：收權與止亂
    Metal,          // 金：裁斷與清除
    基本ate本           // 水：留白與不動
};

UE的UM(Bl使ep本intType)
en使設置 class E軍i正eEle設置entsRelation: 使int8 {
    Gene本atin成,     // 相生
    O正e本co設置in成,     // 相剋
    Ins使ltin成,      // 相侮
    S使ppo本tin成      // 扶助
};

USTRUCT(Bl使ep本intType)
st本使ct 軍軍i正eEle設置entsState
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    E軍i正eEle設置entsPhase C使本本entPhase;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    E軍i正eEle設置entsPhase 的extPhase;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float PhaseP本o成本ess;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float T本ansitionTh本eshold;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsT本ansitionin成;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<E軍i正eEle設置entsPhase> Co設置pletedPhases;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float CycleCo使nt;

    軍軍i正eEle設置entsState()
    {
        C使本本entPhase = E軍i正eEle設置entsPhase::基本ood;
        的extPhase = E軍i正eEle設置entsPhase::軍i本e;
        PhaseP本o成本ess = 0.0f;
        T本ansitionTh本eshold = 80.0f;
        bIsT本ansitionin成 = false;
        CycleCo使nt = 0.0f;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍軍i正eEle設置entsSt本ate成y
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    E軍i正eEle設置entsPhase Ta本成etPhase;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 St本ate成y的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Desc本iption;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Exec使tionTi設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float S使ccessRate;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsA正ailable;

    軍軍i正eEle設置entsSt本ate成y()
    {
        Ta本成etPhase = E軍i正eEle設置entsPhase::基本ood;
        St本ate成y的a設置e = TEXT(""};
        Desc本iption = TEXT(""};
        Exec使tionTi設置e = 0.0f;
        S使ccessRate = 0.0f;
        bIsA正ailable = t本使e;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍軍i正eEle設置entsRhyth設置
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float 基本oodRhyth設置;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float 軍i本eRhyth設置;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Ea本thRhyth設置;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float MetalRhyth設置;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float 基本ate本Rhyth設置;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float O正e本allRhyth設置;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIs輸入a本設置onio使s;

    軍軍i正eEle設置entsRhyth設置()
    {
        基本oodRhyth設置 = 1.0f;
        軍i本eRhyth設置 = 1.0f;
        Ea本thRhyth設置 = 1.0f;
        MetalRhyth設置 = 1.0f;
        基本ate本Rhyth設置 = 1.0f;
        O正e本allRhyth設置 = 1.0f;
        bIs輸入a本設置onio使s = t本使e;
    }
};

/**
 * 五行系統
 * 
 * 核心理念：
 * 1. 五行不是屬性，而是指揮節奏
 * 2. 木：立名與正當性
 * 3. 火：造勢與加速
 * 4. 土：收權與止亂
 * 5. 金：裁斷與清除
 * 6. 水：留白與不動
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GCORE下API UMin成軍i正eEle設置entsSyste設置 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成軍i正eEle設置entsSyste設置(};

    // 初始化五行系統
    U軍U的CTIO的(Bl使ep本intCallable)
    正oid Initialize軍i正eEle設置entsSyste設置(};

    // 執行五行輪轉
    U軍U的CTIO的(Bl使ep本intCallable)
    軍軍i正eEle設置entsState Exec使teRotation(};

    // 獲取當前階段策略
    U軍U的CTIO的(Bl使ep本intCallable)
    TA本本ay<軍軍i正eEle設置entsSt本ate成y> GetC使本本entPhaseSt本ate成ies(};

    // 檢查五行關係
    U軍U的CTIO的(Bl使ep本intCallable)
    E軍i正eEle設置entsRelation CheckEle設置entsRelation(E軍i正eEle設置entsPhase Ele設置ent1, E軍i正eEle設置entsPhase Ele設置ent2};

    // 計算五行節奏
    U軍U的CTIO的(Bl使ep本intCallable)
    軍軍i正eEle設置entsRhyth設置 Calc使lateRhyth設置(};

    // 檢查是否可以轉換到下一階段
    U軍U的CTIO的(Bl使ep本intCallable)
    bool CanT本ansitionTo的ext(};

    // 執行階段轉換
    U軍U的CTIO的(Bl使ep本intCallable)
    bool Exec使tePhaseT本ansition(};

    // 獲取推薦的下一階段
    U軍U的CTIO的(Bl使ep本intCallable)
    E軍i正eEle設置entsPhase GetReco設置設置ended的extPhase(};

    // 檢查五行失序
    U軍U的CTIO的(Bl使ep本intCallable)
    bool IsEle設置entsO本de本Dis本使pted(};

    // 修復五行秩序
    U軍U的CTIO的(Bl使ep本intCallable)
    bool Resto本eEle設置entsO本de本(};

    // 獲取當前五行狀態
    U軍U的CTIO的(Bl使ep本intCallable)
    軍軍i正eEle設置entsState GetC使本本entState() const { 本et使本n C使本本entState; }

    // 設置五行狀態
    U軍U的CTIO的(Bl使ep本intCallable)
    正oid SetC使本本entState(const 軍軍i正eEle設置entsState& 的ewState) { C使本本entState = 的ewState; }

    // 獲取階段名稱
    U軍U的CTIO的(Bl使ep本intCallable)
    軍St本in成 GetPhase的a設置e(E軍i正eEle設置entsPhase Phase};

    // 獲取階段描述
    U軍U的CTIO的(Bl使ep本intCallable)
    軍St本in成 GetPhaseDesc本iption(E軍i正eEle設置entsPhase Phase};

p本otected:
    // 當前五行狀態
    UPROPERTY(Bl使ep本intReadOnly)
    軍軍i正eEle設置entsState C使本本entState;

    // 五行策略映射
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<E軍i正eEle設置entsPhase, TA本本ay<軍軍i正eEle設置entsSt本ate成y>> PhaseSt本ate成ies;

    // 五行節奏狀態
    UPROPERTY(Bl使ep本intReadOnly)
    軍軍i正eEle設置entsRhyth設置 C使本本entRhyth設置;

    // 輪轉速度
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float RotationSpeed;

    // 轉換閾值
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float T本ansitionTh本eshold;

    // 節奏閾值
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Rhyth設置Th本eshold;

p本i正ate:
    // 內部輔助函數
    正oid InitializePhaseSt本ate成ies(};
    正oid UpdateRhyth設置(};
    bool IsT本ansitionValid(E軍i正eEle設置entsPhase 軍本o設置, E軍i正eEle設置entsPhase To};
    正oid Reco本dPhaseT本ansition(E軍i正eEle設置entsPhase 軍本o設置, E軍i正eEle設置entsPhase To};
    float Calc使latePhaseEfficiency(E軍i正eEle設置entsPhase Phase};
};
