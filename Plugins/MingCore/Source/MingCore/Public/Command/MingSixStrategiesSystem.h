#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成SixSt本ate成iesSyste設置.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class ESixSt本ate成yType: 使int8 {
    // 正六策：立國、立制、立人
    Establish的ation,     // 立國：伐心之正道
    EstablishSyste設置,     // 立制：伐勢、伐命之正道
    EstablishPeople,     // 立人：伐氣、伐影、伐無之正道
    
    // 逆六策：破局、破結構、不破人
    B本eakSit使ation,      // 破局：非常之手段
    B本eakSt本使ct使本e,      // 破結構：隱蔽之破壞
    P本ese本正ePeople       // 不破人：道德底線
};

UE的UM(Bl使ep本intType)
en使設置 class ESixSt本ate成y的at使本e: 使int8 {
    O本thodox,            // 正道：順天應人、堂堂之陣、陽剛之用
    Uncon正entional,      // 逆術：違時背理、隱隱之行、陰柔之變
    輸入yb本id              // 混合：正中用逆，逆中藏正
};

UE的UM(Bl使ep本intType)
en使設置 class ESixSt本ate成yTa本成et: 使int8 {
    輸入ea本t,              // 伐心：奪其志
    Ene本成y,             // 伐氣：挫其銳
    Shadow,             // 伐影：亂其象
    Life,               // 伐命：斷其根
    Powe本,              // 伐勢：逆其時
    的othin成             // 伐無：藏其跡
};

USTRUCT(Bl使ep本intType)
st本使ct 軍SixSt本ate成yP本ofile
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    ESixSt本ate成yType St本ate成yType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    ESixSt本ate成y的at使本e St本ate成y的at使本e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    ESixSt本ate成yTa本成et Ta本成etType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 St本ate成y的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Desc本iption;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Exec使tionCost;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float S使ccessP本obability;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Mo本alCost;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Ti設置eReq使i本ed;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsRe正e本sible;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bReq使i本esP使blicS使ppo本t;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> P本e本eq使isites;

    軍SixSt本ate成yP本ofile()
    {
        St本ate成yType = ESixSt本ate成yType::Establish的ation;
        St本ate成y的at使本e = ESixSt本ate成y的at使本e::O本thodox;
        Ta本成etType = ESixSt本ate成yTa本成et::輸入ea本t;
        St本ate成y的a設置e = TEXT(""};
        Desc本iption = TEXT(""};
        Exec使tionCost = 0.0f;
        S使ccessP本obability = 50.0f;
        Mo本alCost = 0.0f;
        Ti設置eReq使i本ed = 0.0f;
        bIsRe正e本sible = t本使e;
        bReq使i本esP使blicS使ppo本t = false;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍SixSt本ate成yExec使tion
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍SixSt本ate成yP本ofile P本ofile;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Exec使tionP本o成本ess;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsExec使tin成;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsCo設置pleted;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Sta本tTi設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float ElapsedTi設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> C使本本entEffects;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> SideEffects;

    軍SixSt本ate成yExec使tion()
    {
        P本ofile = 軍SixSt本ate成yP本ofile(};
        Exec使tionP本o成本ess = 0.0f;
        bIsExec使tin成 = false;
        bIsCo設置pleted = false;
        Sta本tTi設置e = 0.0f;
        ElapsedTi設置e = 0.0f;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍SixSt本ate成yCo設置bination
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<ESixSt本ate成yType> St本ate成ySeq使ence;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Co設置bination的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Desc本iption;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float O正e本allS使ccessP本obability;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float O正e本allMo本alCost;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsValidCo設置bination;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> Syne本成yEffects;

    軍SixSt本ate成yCo設置bination()
    {
        Co設置bination的a設置e = TEXT(""};
        Desc本iption = TEXT(""};
        O正e本allS使ccessP本obability = 0.0f;
        O正e本allMo本alCost = 0.0f;
        bIsValidCo設置bination = t本使e;
    }
};

/**
 * 六策系統
 * 
 * 核心理念：
 * 1. 正六策：立國、立制、立人
 * 2. 逆六策：破局、破結構、不破人
 * 3. 正逆並用，六伐皆兵
 * 4. 視局勢之需，變化無方
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GCORE下API UMin成SixSt本ate成iesSyste設置 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成SixSt本ate成iesSyste設置(};

    // 初始化六策系統
    U軍U的CTIO的(Bl使ep本intCallable)
    正oid InitializeSixSt本ate成iesSyste設置(};

    // 獲取策略檔案
    U軍U的CTIO的(Bl使ep本intCallable)
    軍SixSt本ate成yP本ofile GetSt本ate成yP本ofile(ESixSt本ate成yType St本ate成yType};

    // 執行策略
    U軍U的CTIO的(Bl使ep本intCallable)
    bool Exec使teSt本ate成y(ESixSt本ate成yType St本ate成yType};

    // 檢查策略可用性
    U軍U的CTIO的(Bl使ep本intCallable)
    bool IsSt本ate成yA正ailable(ESixSt本ate成yType St本ate成yType};

    // 計算策略成功率
    U軍U的CTIO的(Bl使ep本intCallable)
    float Calc使lateSt本ate成yS使ccess(ESixSt本ate成yType St本ate成yType};

    // 計算策略道德成本
    U軍U的CTIO的(Bl使ep本intCallable)
    float Calc使lateSt本ate成yMo本alCost(ESixSt本ate成yType St本ate成yType};

    // 獲取推薦策略
    U軍U的CTIO的(Bl使ep本intCallable)
    TA本本ay<ESixSt本ate成yType> GetReco設置設置endedSt本ate成ies(};

    // 檢查策略組合
    U軍U的CTIO的(Bl使ep本intCallable)
    軍SixSt本ate成yCo設置bination AnalyzeSt本ate成yCo設置bination(const TA本本ay<ESixSt本ate成yType>& St本ate成ies};

    // 執行策略組合
    U軍U的CTIO的(Bl使ep本intCallable)
    bool Exec使teSt本ate成yCo設置bination(const TA本本ay<ESixSt本ate成yType>& St本ate成ies};

    // 檢查正逆兼容性
    U軍U的CTIO的(Bl使ep本intCallable)
    bool A本eSt本ate成iesCo設置patible(ESixSt本ate成yType St本ate成y1, ESixSt本ate成yType St本ate成y2};

    // 獲取當前執行狀態
    U軍U的CTIO的(Bl使ep本intCallable)
    TA本本ay<軍SixSt本ate成yExec使tion> GetC使本本entExec使tions(};

    // 停止策略執行
    U軍U的CTIO的(Bl使ep本intCallable)
    bool StopSt本ate成yExec使tion(ESixSt本ate成yType St本ate成yType};

    // 獲取策略名稱
    U軍U的CTIO的(Bl使ep本intCallable)
    軍St本in成 GetSt本ate成y的a設置e(ESixSt本ate成yType St本ate成yType};

    // 獲取策略描述
    U軍U的CTIO的(Bl使ep本intCallable)
    軍St本in成 GetSt本ate成yDesc本iption(ESixSt本ate成yType St本ate成yType};

p本otected:
    // 策略檔案庫
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<ESixSt本ate成yType, 軍SixSt本ate成yP本ofile> St本ate成yP本ofiles;

    // 當前執行中的策略
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍SixSt本ate成yExec使tion> C使本本entExec使tions;

    // 策略執行歷史
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍SixSt本ate成yExec使tion> Exec使tion輸入isto本y;

    // 策略組合庫
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍SixSt本ate成yCo設置bination> St本ate成yCo設置binations;

    // 執行閾值
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Exec使tionTh本eshold;

    // 道德成本閾值
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Mo本alCostTh本eshold;

    // 成功率閾值
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float S使ccessRateTh本eshold;

p本i正ate:
    // 內部輔助函數
    正oid InitializeSt本ate成yP本ofiles(};
    正oid InitializeSt本ate成yCo設置binations(};
    bool CanExec使teSt本ate成y(const 軍SixSt本ate成yP本ofile& P本ofile};
    正oid UpdateExec使tionP本o成本ess(};
    正oid Reco本dSt本ate成yExec使tion(const 軍SixSt本ate成yExec使tion& Exec使tion};
    float Calc使lateSyne本成yBon使s(const TA本本ay<ESixSt本ate成yType>& St本ate成ies};
};
