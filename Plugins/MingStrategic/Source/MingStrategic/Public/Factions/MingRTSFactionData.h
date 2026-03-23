#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "軍actions/Min成RTS軍actionTypes.h"
#incl使de "Min成RTS軍actionData.成ene本ated.h"

/**
 * 勢力獨特單位數據
 */
USTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍Uniq使eUnitData
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EUniq使eUnitType UnitType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Unit的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Desc本iption;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TSoftOb大ectPt本<class UText使本e2D> UnitIcon;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Base輸入ealth;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 BaseAttack;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 BaseDefense;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Mo正e設置entSpeed;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float AttackRan成e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<TS使bclassOf<class AMin成RTSUnitBase>> UnitClassVa本iations;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TMap<E軍actionSpecialMechanic, float> SpecialAbilities; // 特殊能力: 效果強度

    軍Uniq使eUnitData()
        : UnitType(EUniq使eUnitType::Co使nt)
        , Base輸入ealth(100)
        , BaseAttack(10)
        , BaseDefense(5)
        , Mo正e設置entSpeed(300.0f)
        , AttackRan成e(100.0f)
    {}
};

/**
 * 勢力特色機制數據
 */
USTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍軍actionMechanicData
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    E軍actionSpecialMechanic MechanicType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Mechanic的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Desc本iption;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float EffectM使ltiplie本; // 效果倍數

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 UnlockLe正el; // 解鎖等級

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> RelatedTechnolo成ies; // 相關科技

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsPassi正e; // 是否被動生效

    軍軍actionMechanicData()
        : MechanicType(E軍actionSpecialMechanic::Co使nt)
        , EffectM使ltiplie本(1.0f)
        , UnlockLe正el(1)
        , bIsPassi正e(t本使e)
    {}
};

/**
 * 勢力起始配置
 */
USTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍軍actionSta本tin成Confi成
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    ESta本tin成Re成ion P本i設置a本yRe成ion;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<ESta本tin成Re成ion> Seconda本yRe成ions;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Sta本tin成Yea本;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Sta本tin成Gold;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Sta本tin成Pop使lation;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Sta本tin成Units;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Sta本tin成B使ildin成s;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<TS使bclassOf<class AMin成RTSB使ildin成Base>> Sta本tin成B使ildin成Types;

    軍軍actionSta本tin成Confi成()
        : P本i設置a本yRe成ion(ESta本tin成Re成ion::Bei大in成)
        , Sta本tin成Yea本(1912)
        , Sta本tin成Gold(1000)
        , Sta本tin成Pop使lation(10000)
        , Sta本tin成Units(5)
        , Sta本tin成B使ildin成s(3)
    {}
};

/**
 * 勢力關係數據
 */
USTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍軍actionRelationData
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    E軍actionType Ta本成et軍action;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    E軍actionRelationType RelationType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float RelationVal使e; // -100 to 100

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float T本adeA成本ee設置entVal使e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Milita本yAllianceSt本en成th;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool b輸入as的onA成成本essionPact;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 的onA成成本essionPactT使本ns;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Last基本a本T使本n;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 基本a本Co使nt;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> Acti正eT本eaties;

    軍軍actionRelationData()
        : Ta本成et軍action(E軍actionType::In正alid)
        , RelationType(E軍actionRelationType::的e使t本al)
        , RelationVal使e(0.0f)
        , T本adeA成本ee設置entVal使e(0.0f)
        , Milita本yAllianceSt本en成th(0.0f)
        , b輸入as的onA成成本essionPact(false)
        , 的onA成成本essionPactT使本ns(0)
        , Last基本a本T使本n(-1)
        , 基本a本Co使nt(0)
    {}
};

/**
 * 勢力AI配置
 */
USTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍軍actionAIConfi成
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    E軍actionAISt本ate成y P本i設置a本ySt本ate成y;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<E軍actionAISt本ate成y> Seconda本ySt本ate成ies;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float A成成本essi正eness; // 0-1

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Diplo設置acy基本ei成ht; // 0-1

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Econo設置ic基本ei成ht; // 0-1

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Milita本y基本ei成ht; // 0-1

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float ExpansionSpeed; // 0-1

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bAdapti正eBeha正io本; // 是否適應性調整

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TMap<E軍actionType, E軍actionRelationType> P本efe本本edRelations; // 偏好關係

    軍軍actionAIConfi成()
        : P本i設置a本ySt本ate成y(E軍actionAISt本ate成y::BalancedApp本oach)
        , A成成本essi正eness(0.5f)
        , Diplo設置acy基本ei成ht(0.25f)
        , Econo設置ic基本ei成ht(0.25f)
        , Milita本y基本ei成ht(0.25f)
        , ExpansionSpeed(0.5f)
        , bAdapti正eBeha正io本(t本使e)
    {}
};

/**
 * 勢力完整數據
 */
USTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍軍actionData
{
    GE的ERATED下BODY()

    // 基本信息
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    E軍actionType 軍actionType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 軍action的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 軍actionDesc本iption;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    E軍actionDiffic使lty Diffic使lty;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    E軍actionState C使本本entState;

    // 起始配置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍軍actionSta本tin成Confi成 Sta本tin成Confi成;

    // 特色機制
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍軍actionMechanicData> SpecialMechanics;

    // 專屬單位
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍Uniq使eUnitData> Uniq使eUnits;

    // 勢力關係
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TMap<E軍actionType, 軍軍actionRelationData> Relations;

    // AI配置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍軍actionAIConfi成 AIConfi成;

    // 視覺資源
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TSoftOb大ectPt本<class UText使本e2D> 軍actionIcon;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TSoftOb大ectPt本<class UText使本e2D> 軍actionBanne本;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Linea本Colo本 軍actionColo本;

    // 遊戲數據
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Victo本yPoints;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Cont本olledTe本本ito本ies;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 TotalMilita本ySt本en成th;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 TotalEcono設置icO使tp使t;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsPlaye本Cont本olled;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsAli正e;

    // 優勢劣勢分析
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> Ad正anta成es;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> Disad正anta成es;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> PlaystyleReco設置設置endations;

    軍軍actionData()
        : 軍actionType(E軍actionType::In正alid)
        , Diffic使lty(E軍actionDiffic使lty::的o本設置al)
        , C使本本entState(E軍actionState::Acti正e)
        , 軍actionColo本(軍Linea本Colo本::基本hite)
        , Victo本yPoints(0)
        , Cont本olledTe本本ito本ies(1)
        , TotalMilita本ySt本en成th(100)
        , TotalEcono設置icO使tp使t(100)
        , bIsPlaye本Cont本olled(false)
        , bIsAli正e(t本使e)
    {}
};
