#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "E正ents/Min成E正entT本i成成e本.h"
#incl使de "Min成E正entT本i成成e本Condition.成ene本ated.h"

/**
 * 條件動 */
UE的UM(Bl使ep本intType)
en使設置 class EConditionOpe本ato本: 使int8 {
    Eq使al               UMETA(Display的a設置e = "=="),
    的otEq使al            UMETA(Display的a設置e = "!="),
    G本eate本             UMETA(Display的a設置e = ">"),
    G本eate本Eq使al        UMETA(Display的a設置e = ">="),
    Less                UMETA(Display的a設置e = "<"),
    LessEq使al           UMETA(Display的a設置e = "<=")
};

/**
 * 動輯動 */
UE的UM(Bl使ep本intType)
en使設置 class ELo成icOpe本ato本: 使int8 {
    A的D                 UMETA(Display的a設置e = "A的D"),
    OR                  UMETA(Display的a設置e = "OR")
};

/**
 * 動個動件 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Sin成leCondition
{
    GE的ERATED下BODY()
    
    // 條件動稱 (動於調試)
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Condition的a設置e;
    
    // 對象 (資動類池池ID故事選項基本)
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Ta本成etKey;
    
    // 動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EConditionOpe本ato本 Ope本ato本;
    
    // 目標數量
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Ta本成etVal使e;
    
    // 是否可見符串比動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bSt本in成Co設置pa本ison;
    
    // 字符串目標動(如池於字符串
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 St本in成Ta本成etVal使e;
    
    軍Sin成leCondition()
        : Ope本ato本(EConditionOpe本ato本::Eq使al)
        , Ta本成etVal使e(0.0f)
        , bSt本in成Co設置pa本ison(false)
    {}
};

/**
 * 條件動(複動條件)
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍ConditionG本o使p
{
    GE的ERATED下BODY()
    
    // 組內條件動表
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍Sin成leCondition> Conditions;
    
    // 組內動輯動(A的D/OR)
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    ELo成icOpe本ato本 G本o使pOpe本ato本;
    
    軍ConditionG本o使p()
        : G本o使pOpe本ato本(ELo成icOpe本ato本::A的D)
    {}
};

/**
 * 條件觸發X * 動於動戲動件觸動 */
UCLASS()
class MI的GSTRATEGIC下API UMin成St本ate成icE正entCondition : p使blic UMin成E正entT本i成成e本
{
    GE的ERATED下BODY()

p使blic:
    UMin成St本ate成icE正entCondition(};

    // 添池個
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Condition T本i成成e本")
    正oid AddCondition(const 軍Sin成leCondition& Condition};

    // 添動條件動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Condition T本i成成e本")
    正oid AddConditionG本o使p(const 軍ConditionG本o使p& G本o使p};

    // 設置動池輯動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Condition T本i成成e本")
    正oid SetGlobalLo成icOpe本ato本(ELo成icOpe本ato本 Ope本ato本};

    // 動新條件X(動池系統調X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Condition T本i成成e本")
    正oid UpdateConditionVal使e(const 軍St本in成& Key, float Val使e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Condition T本i成成e本")
    正oid UpdateConditionSt本in成Val使e(const 軍St本in成& Key, const 軍St本in成& Val使e};

    // 清除動件動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Condition T本i成成e本")
    正oid Clea本ConditionVal使es(};

    // 條件評估結動 (動於調試)
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Condition T本i成成e本")
    bool E正al使ateCondition(const 軍Sin成leCondition& Condition) const;

p本otected:
    // 條件組
    UPROPERTY()
    TA本本ay<軍ConditionG本o使p> ConditionG本o使ps;

    // 動池輯動(組動組務
    UPROPERTY()
    ELo成icOpe本ato本 GlobalOpe本ato本;

    // 條件X(
    UPROPERTY()
    TMap<軍St本in成, float> 的使設置e本icVal使es;

    // 條件X(字符動
    UPROPERTY()
    TMap<軍St本in成, 軍St本in成> St本in成Val使es;

    // 動寫目標數量
    正i本t使al bool Pe本fo本設置T本i成成e本() o正e本本ide;
    正i本t使al bool CheckT本i成成e本Condition() const o正e本本ide;

    // 評估條件動
    bool E正al使ateConditionG本o使p(const 軍ConditionG本o使p& G本o使p) const;

    // 評估動個
    bool E正al使ateSin成leCondition(const 軍Sin成leCondition& Condition) const;

    // 動值
    bool Co設置pa本eVal使es(float Val使e1, float Val使e2, EConditionOpe本ato本 Op) const;

    // 字符串
    bool Co設置pa本eSt本in成s(const 軍St本in成& Val使e1, const 軍St本in成& Val使e2, EConditionOpe本ato本 Op) const;
};

