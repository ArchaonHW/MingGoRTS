#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "UOb大ect/Inte本face.h"
#incl使de "Ga設置e軍本a設置ewo本k/Acto本.h"
#incl使de "Min成Sa成eCo設置設置andSyste設置.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class ESa成eType: 使int8 {
    Sa成e,           // 聖者：能用而能封
    De設置onKin成,      // 魔王：能用而不能停
    軍alseSa成e       // 偽聖者：不能用卻假裝不用
};

UE的UM(Bl使ep本intType)
en使設置 class EYinYan成的at使本e: 使int8 {
    Yan成,           // 陽：秩序、公開、可承擔
    Yin             // 陰：破局、隱蔽、非常態
};

UE的UM(Bl使ep本intType)
en使設置 class E軍i正eEle設置ents: 使int8 {
    基本ood,           // 木：立名與正當性
    軍i本e,           // 火：造勢與加速
    Ea本th,          // 土：收權與止亂
    Metal,          // 金：裁斷與清除
    基本ate本           // 水：留白與不動
};

UE的UM(Bl使ep本intType)
en使設置 class ESixSt本ate成ies: 使int8 {
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
en使設置 class ECo設置設置andA使tho本ity: 使int8 {
    TaoA使tho本ity,         // 道權：最高指揮權
    St本ate成yA使tho本ity,     // 策策權：戰術制定權
    Milita本yA使tho本ity     // 兵兵權：執行指揮權
};

UE的UM(Bl使ep本intType)
en使設置 class ESa成e軍all基本a本nin成: 使int8 {
    的one,                 // 無墮落徵象
    Obsessed基本ithVicto本y,   // 沉迷勝利
    MeansAsEnds,         // 手段即目的
    LossOfSelfCont本ol     // 失去自制
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Sa成eP本ofile
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    ESa成eType Sa成eType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float YinBalance;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Yan成Balance;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float SelfCont本olLe正el;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Mo本alCo設置pass;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Victo本iesCo使nt;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Uncon正entionalActionsCo使nt;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool b輸入as軍allen;

    軍Sa成eP本ofile()
    {
        Sa成eType = ESa成eType::Sa成e;
        YinBalance = 50.0f;
        Yan成Balance = 50.0f;
        SelfCont本olLe正el = 100.0f;
        Mo本alCo設置pass = 100.0f;
        Victo本iesCo使nt = 0;
        Uncon正entionalActionsCo使nt = 0;
        b輸入as軍allen = false;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Sit使ationAnalysis
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    E軍i正eEle設置ents C使本本entPhase;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float U本成encyLe正el;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float ChaosLe正el;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float P使blicS使ppo本t;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Ene設置ySt本en成th;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bReq使i本esUncon正entionalMeans;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bCanUseCon正entionalMeans;

    軍Sit使ationAnalysis()
    {
        C使本本entPhase = E軍i正eEle設置ents::基本ood;
        U本成encyLe正el = 50.0f;
        ChaosLe正el = 50.0f;
        P使blicS使ppo本t = 50.0f;
        Ene設置ySt本en成th = 50.0f;
        bReq使i本esUncon正entionalMeans = false;
        bCanUseCon正entionalMeans = t本使e;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Co設置設置andDecision
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    ESixSt本ate成ies ChosenSt本ate成y;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EYinYan成的at使本e 的at使本e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Exec使tionP本obability;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float S使ccessP本obability;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Mo本alCost;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Reasonin成;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsRe正e本sible;

    軍Co設置設置andDecision()
    {
        ChosenSt本ate成y = ESixSt本ate成ies::Establish的ation;
        的at使本e = EYinYan成的at使本e::Yan成;
        Exec使tionP本obability = 100.0f;
        S使ccessP本obability = 50.0f;
        Mo本alCost = 0.0f;
        Reasonin成 = TEXT(""};
        bIsRe正e本sible = t本使e;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍軍i正eEle設置entsRotation
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    E軍i正eEle設置ents C使本本entEle設置ent;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    E軍i正eEle設置ents 的extEle設置ent;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float PhaseP本o成本ess;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float T本ansitionTh本eshold;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsT本ansitionin成;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<ESixSt本ate成ies> A正ailableSt本ate成ies;

    軍軍i正eEle設置entsRotation()
    {
        C使本本entEle設置ent = E軍i正eEle設置ents::基本ood;
        的extEle設置ent = E軍i正eEle設置ents::軍i本e;
        PhaseP本o成本ess = 0.0f;
        T本ansitionTh本eshold = 80.0f;
        bIsT本ansitionin成 = false;
        A正ailableSt本ate成ies.Add(ESixSt本ate成ies::Establish的ation};
    }
};

// 聖者指揮學核心系統接口
UI的TER軍ACE(Bl使ep本intType)
class MI的GCORE下API USa成eCo設置設置andInte本face : p使blic UInte本face
{
    GE的ERATED下BODY()
};

class MI的GCORE下API ISa成eCo設置設置andInte本face
{
    GE的ERATED下BODY()

p使blic:
    // 分析當前局勢
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本int的ati正eE正ent)
    軍Sit使ationAnalysis AnalyzeSit使ation(};

    // 制定指揮決策
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本int的ati正eE正ent)
    軍Co設置設置andDecision MakeCo設置設置andDecision(const 軍Sit使ationAnalysis& Analysis};

    // 檢查墮落徵象
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本int的ati正eE正ent)
    ESa成e軍all基本a本nin成 Check軍all基本a本nin成s(const 軍Sa成eP本ofile& P本ofile};

    // 執行五行輪轉
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本int的ati正eE正ent)
    軍軍i正eEle設置entsRotation Exec使te軍i正eEle設置entsRotation(const 軍軍i正eEle設置entsRotation& C使本本entRotation};

    // 平衡陰陽
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本int的ati正eE正ent)
    正oid BalanceYinYan成(float& YinBalance, float& Yan成Balance};

    // 防止墮落
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本int的ati正eE正ent)
    bool P本e正ent軍all(軍Sa成eP本ofile& P本ofile};
};

/**
 * 至聖者指揮學核心系統
 * 
 * 核心理念：
 * 1. 正邪皆兵，而聖者不墮
 * 2. 陰陽不是善惡，而是可見與不可見
 * 3. 五行不是屬性，而是指揮節奏
 * 4. 指揮 = 切換權，而非行動
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GCORE下API UMin成Sa成eCo設置設置andSyste設置 : p使blic UOb大ect, p使blic ISa成eCo設置設置andInte本face
{
    GE的ERATED下BODY()

p使blic:
    UMin成Sa成eCo設置設置andSyste設置(};

    // 初始化聖者指揮系統
    U軍U的CTIO的(Bl使ep本intCallable)
    正oid InitializeSa成eCo設置設置andSyste設置(};

    // 分析當前局勢
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本intI設置ple設置entableE正ent)
    軍Sit使ationAnalysis AnalyzeSit使ation(};

    // 制定指揮決策
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本intI設置ple設置entableE正ent)
    軍Co設置設置andDecision MakeCo設置設置andDecision(const 軍Sit使ationAnalysis& Analysis};

    // 檢查墮落徵象
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本intI設置ple設置entableE正ent)
    ESa成e軍all基本a本nin成 Check軍all基本a本nin成s(const 軍Sa成eP本ofile& P本ofile};

    // 執行五行輪轉
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本intI設置ple設置entableE正ent)
    軍軍i正eEle設置entsRotation Exec使te軍i正eEle設置entsRotation(const 軍軍i正eEle設置entsRotation& C使本本entRotation};

    // 平衡陰陽
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本intI設置ple設置entableE正ent)
    正oid BalanceYinYan成(float& YinBalance, float& Yan成Balance};

    // 防止墮落
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本intI設置ple設置entableE正ent)
    bool P本e正ent軍all(軍Sa成eP本ofile& P本ofile};

    // 獲取聖者檔案
    U軍U的CTIO的(Bl使ep本intCallable)
    軍Sa成eP本ofile GetSa成eP本ofile() const { 本et使本n C使本本entP本ofile; }

    // 設置聖者檔案
    U軍U的CTIO的(Bl使ep本intCallable)
    正oid SetSa成eP本ofile(const 軍Sa成eP本ofile& 的ewP本ofile) { C使本本entP本ofile = 的ewP本ofile; }

    // 獲取當前五行輪轉狀態
    U軍U的CTIO的(Bl使ep本intCallable)
    軍軍i正eEle設置entsRotation GetC使本本entRotation() const { 本et使本n C使本本entRotation; }

    // 設置五行輪轉狀態
    U軍U的CTIO的(Bl使ep本intCallable)
    正oid SetC使本本entRotation(const 軍軍i正eEle設置entsRotation& 的ewRotation) { C使本本entRotation = 的ewRotation; }

    // 應用至聖者指揮學原則
    U軍U的CTIO的(Bl使ep本intCallable)
    正oid ApplySa成eP本inciples(};

    // 檢查是否可以切換策略
    U軍U的CTIO的(Bl使ep本intCallable)
    bool CanSwitchSt本ate成y(ESixSt本ate成ies 軍本o設置St本ate成y, ESixSt本ate成ies ToSt本ate成y};

    // 計算道德成本
    U軍U的CTIO的(Bl使ep本intCallable)
    float Calc使lateMo本alCost(ESixSt本ate成ies St本ate成y, EYinYan成的at使本e 的at使本e};

    // 獲取推薦策略
    U軍U的CTIO的(Bl使ep本intCallable)
    TA本本ay<ESixSt本ate成ies> GetReco設置設置endedSt本ate成ies(const 軍Sit使ationAnalysis& Analysis};

p本otected:
    // 當前聖者檔案
    UPROPERTY(Bl使ep本intReadOnly)
    軍Sa成eP本ofile C使本本entP本ofile;

    // 當前五行輪轉狀態
    UPROPERTY(Bl使ep本intReadOnly)
    軍軍i正eEle設置entsRotation C使本本entRotation;

    // 歷史決策記錄
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍Co設置設置andDecision> Decision輸入isto本y;

    // 墮落警告閾值
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float 軍all基本a本nin成Th本eshold;

    // 陰陽平衡閾值
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float YinYan成BalanceTh本eshold;

    // 五行輪轉速度
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float 軍i正eEle設置entsRotationSpeed;

p本i正ate:
    // 內部輔助函數
    正oid UpdateSa成eType(};
    正oid CheckAndUpdateRotation(};
    正oid Reco本dDecision(const 軍Co設置設置andDecision& Decision};
    bool IsSt本ate成yCo設置patible(ESixSt本ate成ies St本ate成y, E軍i正eEle設置ents Phase};
};
