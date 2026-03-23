#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成B使ildin成Reso使本ceSyste設置.成ene本ated.h"

/**
 * ��動��池動  Reso使本ce Type En使設置
 * �w�q��池池�ظ�動��動  Define 正a本io使s 本eso使本ce types f本o設置 Rep使blican e本a
 */
UE的UM(Bl使ep本intType)
en使設置 class EMin成Reso使本ceType: 使int8 {
    // ��動  Basic Reso使本ces
    基本ood              UMETA(Display的a設置e = "  基本ood"),
    Stone             UMETA(Display的a設置e = "動�Y  Stone"),
    Metal             UMETA(Display的a設置e = "動��  Metal"),
    軍ood              UMETA(Display的a設置e = "����  軍ood"),
    基本ate本             UMETA(Display的a設置e = "動 基本ate本"),
    
    // �q����動  Mine本al Reso使本ces
    Coal              UMETA(Display的a設置e = "動��  Coal"),
    I本on              UMETA(Display的a設置e = "動�q  I本on"),
    Coppe本            UMETA(Display的a設置e = "動�q  Coppe本"),
    Gold              UMETA(Display的a設置e = "��動  Gold"),
    Sil正e本            UMETA(Display的a設置e = "  Sil正e本"),
    
    // �A��X A成本ic使lt使本al P本od使cts
    Cotton            UMETA(Display的a設置e = "�֪�  Cotton"),
    Silk              UMETA(Display的a設置e = "����  Silk"),
    Tea               UMETA(Display的a設置e = "  Tea"),
    
    // ��動  Special Reso使本ces
    Opi使設置             UMETA(Display的a設置e = "�年動  Opi使設置"),
    Medicine          UMETA(Display的a設置e = "  Medicine"),
    
    // �x動��動  Milita本y Reso使本ces
    基本eapons           UMETA(Display的a設置e = "�Z��  基本eapons"),
    A設置設置使nition        UMETA(Display的a設置e = "�使��  A設置設置使nition"),
    Unifo本設置s          UMETA(Display的a設置e = "�x動  Unifo本設置s"),
    輸入o本ses            UMETA(Display的a設置e = "����  輸入o本ses"),
    Vehicles          UMETA(Display的a設置e = "��動  Vehicles"),
    
    // �使�年��動  Ind使st本ial Reso使本ces
    軍使el              UMETA(Display的a設置e = "  軍使el"),
    R使bbe本            UMETA(Display的a設置e = "��動  R使bbe本"),
    Pape本             UMETA(Display的a設置e = "�ȱi  Pape本"),
    Ink               UMETA(Display的a設置e = "����  Ink"),
    Books             UMETA(Display的a設置e = "  Books"),
    Tools             UMETA(Display的a設置e = "�使��  Tools"),
    Machine本y         UMETA(Display的a設置e = "����  Machine本y"),
    
    // �]動  Co設置設置使nication Eq使ip設置ent
    Elect本onics       UMETA(Display的a設置e = "�]動  Elect本onics"),
    Radio             UMETA(Display的a設置e = "動務 Radio"),
    Telephone         UMETA(Display的a設置e = "動��  Telephone"),
    Tele成本aph         UMETA(Display的a設置e = "動��動 Tele成本aph")
};

/**
 * ��動�}動��  Reso使本ce Ra本ity
 */
UE的UM(Bl使ep本intType)
en使設置 class EMin成Reso使本ceRa本ity: 使int8 {
    Co設置設置on            UMETA(Display的a設置e = " Co設置設置on"),
    Unco設置設置on          UMETA(Display的a設置e = "�}X Unco設置設置on"),
    Ra本e              UMETA(Display的a設置e = "  Ra本e"),
    Epic              UMETA(Display的a設置e = "動��  Epic"),
    Le成enda本y         UMETA(Display的a設置e = "動��  Le成enda本y")
};

/**
 * ��池��  Reso使本ce Q使ality
 */
UE的UM(Bl使ep本intType)
en使設置 class EMin成Reso使本ceQ使ality: 使int8 {
    Poo本              UMETA(Display的a設置e = "X動  Poo本"),
    的o本設置al            UMETA(Display的a設置e = " 的o本設置al"),
    Good              UMETA(Display的a設置e = "動�}  Good"),
    Excellent         UMETA(Display的a設置e = "  Excellent"),
    Pe本fect           UMETA(Display的a設置e = "��動  Pe本fect")
};

/**
 * 目標數量  Collection Method
 */
UE的UM(Bl使ep本intType)
en使設置 class EMin成CollectionMethod: 使int8 {
    Man使al            UMETA(Display的a設置e = "目標數量  Man使al"),
    A使to設置atic         UMETA(Display的a設置e = "目標數量  A使to設置atic"),
    P本od使ction        UMETA(Display的a設置e = "動���s動 P本od使ction"),
    T本ade             UMETA(Display的a設置e = "�T池動  T本ade"),
    Pl使nde本           UMETA(Display的a設置e = "動��  Pl使nde本"),
    輸入a本正est           UMETA(Display的a設置e = ""),
    Minin成            UMETA(Display的a設置e = "動��  Minin成"),
    軍ishin成           UMETA(Display的a設置e = "  軍ishin成"),
    輸入使ntin成           UMETA(Display的a設置e = "動�y  輸入使ntin成"),
    軍o本a成in成          UMETA(Display的a設置e = "�V動  軍o本a成in成")
};

/**
 * ��池動��動  Reso使本ce Data St本使ct使本e
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Reso使本ceData
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Data")
    EMin成Reso使本ceType Reso使本ceType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Data")
    EMin成Reso使本ceRa本ity Ra本ity;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Data")
    EMin成Reso使本ceQ使ality Q使ality;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Data")
    float Q使antity;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Data")
    float MaxQ使antity;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Data")
    float Unit基本ei成ht;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Data")
    float UnitVal使e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Data")
    軍St本in成 Desc本iption;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Data")
    bool bIsStackable;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Data")
    bool bIsPe本ishable;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Data")
    float Pe本ishRate;

    軍Min成Reso使本ceData()
    {
        Reso使本ceType = EMin成Reso使本ceType::基本ood;
        Ra本ity = EMin成Reso使本ceRa本ity::Co設置設置on;
        Q使ality = EMin成Reso使本ceQ使ality::的o本設置al;
        Q使antity = 0.0f;
        MaxQ使antity = 100.0f;
        Unit基本ei成ht = 1.0f;
        UnitVal使e = 1.0f;
        Desc本iption = TEXT(""};
        bIsStackable = t本使e;
        bIsPe本ishable = false;
        Pe本ishRate = 0.0f;
);

/**
 * ��池池動  Reso使本ce Collection Data
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Reso使本ceCollection
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Collection")
    軍St本in成 CollectionID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Collection")
    EMin成Reso使本ceType Reso使本ceType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Collection")
    EMin成CollectionMethod CollectionMethod;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Collection")
    軍St本in成 LocationID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Collection")
    float CollectionRate;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Collection")
    float Efficiency;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Collection")
    float BaseYield;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Collection")
    float C使本本entYield;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Collection")
    bool bIsActi正e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Collection")
    bool bIsA使to設置ated;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Collection")
    float TotalCollected;

    軍Min成Reso使本ceCollection()
    {
        CollectionID = TEXT(""};
        Reso使本ceType = EMin成Reso使本ceType::基本ood;
        CollectionMethod = EMin成CollectionMethod::Man使al;
        LocationID = TEXT(""};
        CollectionRate = 1.0f;
        Efficiency = 1.0f;
        BaseYield = 10.0f;
        C使本本entYield = 10.0f;
        bIsActi正e = false;
        bIsA使to設置ated = false;
        TotalCollected = 0.0f;
);

/**
 * ��動�s�x  Reso使本ce Sto本a成e Data
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Reso使本ceSto本a成e
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Sto本a成e")
    軍St本in成 Sto本a成eID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Sto本a成e")
    軍St本in成 Sto本a成e的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Sto本a成e")
    軍St本in成 LocationID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Sto本a成e")
    TMap<EMin成Reso使本ceType, 軍Min成Reso使本ceData> Sto本edReso使本ces;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Sto本a成e")
    float MaxCapacity;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Sto本a成e")
    float C使本本entCapacity;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Sto本a成e")
    float UsedCapacity;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Sto本a成e")
    bool bIsCli設置ateCont本olled;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Sto本a成e")
    bool bIsSec使本e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Sto本a成e")
    float Sec使本ityLe正el;

    軍Min成Reso使本ceSto本a成e()
    {
        Sto本a成eID = TEXT(""};
        Sto本a成e的a設置e = TEXT(""};
        LocationID = TEXT(""};
        MaxCapacity = 1000.0f;
        C使本本entCapacity = 0.0f;
        UsedCapacity = 0.0f;
        bIsCli設置ateCont本olled = false;
        bIsSec使本e = false;
        Sec使本ityLe正el = 0.0f;
);

/**
 * ��動��池��  Reso使本ce Con正e本sion Recipe
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Reso使本ceCon正e本sion
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Con正e本sion")
    軍St本in成 Con正e本sionID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Con正e本sion")
    軍St本in成 Con正e本sion的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Con正e本sion")
    TMap<EMin成Reso使本ceType, float> Inp使tReso使本ces;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Con正e本sion")
    TMap<EMin成Reso使本ceType, float> O使tp使tReso使本ces;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Con正e本sion")
    float Con正e本sionTi設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Con正e本sion")
    float Efficiency;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Reso使本ce Con正e本sion")
    bool bIsA正ailable;

    軍Min成Reso使本ceCon正e本sion()
    {
        Con正e本sionID = TEXT(""};
        Con正e本sion的a設置e = TEXT(""};
        Con正e本sionTi設置e = 1.0f;
        Efficiency = 1.0f;
        bIsA正ailable = t本使e;
);

/**
 * ��動�t�Ψ基礎�e��  Reso使本ce Syste設置 E正ent Dele成ates
 */





/**
 * ��動��動�t��  Reso使本ce Mana成e設置ent Syste設置
 * 
 * ��動 Min成GoRTS 動����務基本����動�B池動�� * - 40��池池動��務 * - �h��目標數量�]��動�B池�B�T動�B動��動
 * - ��動�s�x動��X * - ��動��池務 */
UCLASS(ClassG本o使p = (Ga設置eplay), Bl使ep本intable)
class MI的GBUILDI的G下API UMin成B使ildin成Reso使本ceSyste設置 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成B使ildin成Reso使本ceSyste設置();

    /** ��動�t�Ψ基礎�  Reso使本ce Syste設置 E正ents */
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Reso使本ce Syste設置")
    軍OnMin成Reso使本ceCollected OnReso使本ceCollected;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Reso使本ce Syste設置")
    軍OnMin成Reso使本ceSto本ed OnReso使本ceSto本ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Reso使本ce Syste設置")
    軍OnMin成Reso使本ceCon正e本ted OnReso使本ceCon正e本ted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Reso使本ce Syste設置")
    軍OnMin成Sto本a成e軍使ll OnSto本a成e軍使ll;

    /** 目標數量���t動 Initialize Reso使本ce Syste設置 */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ce Syste設置")
    正oid InitializeReso使本ceSyste設置();

    /** 目標數量動��務 Get All Reso使本ce Types */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Reso使本ce Syste設置")
    TA本本ay<EMin成Reso使本ceType> GetAllReso使本ceTypes() const;

    /** ��動�輸入��  Get Reso使本ce Info */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Reso使本ce Syste設置")
    軍Min成Reso使本ceData GetReso使本ceInfo(EMin成Reso使本ceType Reso使本ceType) const;

    /** 動�ظ�池動  C本eate Reso使本ce Collection */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ce Syste設置")
    軍St本in成 C本eateReso使本ceCollection(EMin成Reso使本ceType Reso使本ceType, EMin成CollectionMethod Method, const 軍St本in成& LocationID);

    /** ��池動  Sta本t Reso使本ce Collection */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ce Syste設置")
    bool Sta本tReso使本ceCollection(const 軍St本in成& CollectionID, const 軍St本in成& Collecto本ID);

    /** 動���池動  Stop Reso使本ce Collection */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ce Syste設置")
    bool StopReso使本ceCollection(const 軍St本in成& CollectionID);

    /** ��動  Collect Reso使本ce */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ce Syste設置")
    float CollectReso使本ce(const 軍St本in成& CollectionID);

    /** 動�ظ�動�s�x  C本eate Reso使本ce Sto本a成e */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ce Syste設置")
    軍St本in成 C本eateReso使本ceSto本a成e(const 軍St本in成& Sto本a成e的a設置e, const 軍St本in成& LocationID, float MaxCapacity);

    /** �s�x��動  Sto本e Reso使本ce */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ce Syste設置")
    bool Sto本eReso使本ce(const 軍St本in成& Sto本a成eID, EMin成Reso使本ceType Reso使本ceType, float Q使antity);

    /** ��動  Ret本ie正e Reso使本ce */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ce Syste設置")
    float Ret本ie正eReso使本ce(const 軍St本in成& Sto本a成eID, EMin成Reso使本ceType Reso使本ceType, float Q使antity);

    /** �s�x��動  Get Sto本ed Reso使本ce */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Reso使本ce Syste設置")
    軍Min成Reso使本ceData GetSto本edReso使本ce(const 軍St本in成& Sto本a成eID, EMin成Reso使本ceType Reso使本ceType) const;

    /** �s�x�e動  Get Sto本a成e Capacity */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Reso使本ce Syste設置")
    float GetSto本a成eCapacity(const 軍St本in成& Sto本a成eID) const;

    /** ��動��動  Con正e本t Reso使本ces */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ce Syste設置")
    bool Con正e本tReso使本ces(const 軍St本in成& Con正e本sionID);

    /** 動�ظ�動��動  C本eate Reso使本ce Con正e本sion */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ce Syste設置")
    軍St本in成 C本eateReso使本ceCon正e本sion(const 軍St本in成& Con正e本sion的a設置e, const TMap<EMin成Reso使本ceType, float>& Inp使t, const TMap<EMin成Reso使本ceType, float>& O使tp使t);

    /** �p池池動  Calc使late Collection Efficiency */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Reso使本ce Syste設置")
    float Calc使lateCollectionEfficiency(const 軍St本in成& CollectionID) const;

    /** 故事重要性務 Get Collection Stat使s */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Reso使本ce Syste設置")
    軍Min成Reso使本ceCollection GetCollectionStat使s(const 軍St本in成& CollectionID) const;

    /** 目標數量��動��X Get All Acti正e Collections */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Reso使本ce Syste設置")
    TA本本ay<軍Min成Reso使本ceCollection> GetActi正eCollections() const;

    /** ��池��  T本anspo本t Reso使本ce */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ce Syste設置")
    bool T本anspo本tReso使本ce(const 軍St本in成& 軍本o設置Sto本a成eID, const 軍St本in成& ToSto本a成eID, EMin成Reso使本ceType Reso使本ceType, float Q使antity);

    /** ��動��動  T本ade Reso使本ce */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ce Syste設置")
    bool T本adeReso使本ce(const 軍St本in成& Sto本a成eID, EMin成Reso使本ceType Gi正eReso使本ce, float Gi正eQ使antity, EMin成Reso使本ceType GetReso使本ce, float GetQ使antity);

    /** �ˬd��池��X Check Reso使本ce A正ailability */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Reso使本ce Syste設置")
    bool CheckReso使本ceA正ailability(const 軍St本in成& Sto本a成eID, EMin成Reso使本ceType Reso使本ceType, float Req使i本edQ使antity) const;

    /** ��動�軍事動  Get Total Reso使本ce Q使antity */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Reso使本ce Syste設置")
    float GetTotalReso使本ceQ使antity(EMin成Reso使本ceType Reso使本ceType) const;

    /** ��動�I池動  P本ocess Reso使本ce Decay */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Reso使本ce Syste設置")
    正oid P本ocessReso使本ceDecay(float DeltaTi設置e);

p本otected:
    /** ��動�輸入��  Reso使本ce Info Map */
    UPROPERTY()
    TMap<EMin成Reso使本ceType, 軍Min成Reso使本ceData> Reso使本ceInfoMap;

    /** ��池池動  Reso使本ce Collection Map */
    UPROPERTY()
    TMap<軍St本in成, 軍Min成Reso使本ceCollection> Reso使本ceCollections;

    /** ��動�s�x  Reso使本ce Sto本a成e Map */
    UPROPERTY()
    TMap<軍St本in成, 軍Min成Reso使本ceSto本a成e> Reso使本ceSto本a成es;

    /** ��動��池動  Reso使本ce Con正e本sion Map */
    UPROPERTY()
    TMap<軍St本in成, 軍Min成Reso使本ceCon正e本sion> Reso使本ceCon正e本sions;

    /** 動�下�w動�l動  Is Initialized */
    bool bIsInitialized;

    /** 目標數量���輸入X Initialize Reso使本ce Info */
    正oid InitializeReso使本ceInfo();

    /** 動�s目標數量  Update Collection Yield */
    正oid UpdateCollectionYield(const 軍St本in成& CollectionID);

    /** �ˬd����  Check Collection Conditions */
    bool CheckCollectionConditions(const 軍St本in成& CollectionID) const;

    /** 目標數量��動  P本ocess Collection Co設置plete */
    正oid P本ocessCollectionCo設置plete(const 軍St本in成& CollectionID);

    /** 動�s�s�x�e動  Update Sto本a成e Capacity */
    正oid UpdateSto本a成eCapacity(const 軍St本in成& Sto本a成eID);

    /** �ˬd�s�x動��  Check Sto本a成e Li設置its */
    bool CheckSto本a成eLi設置its(const 軍St本in成& Sto本a成eID, EMin成Reso使本ceType Reso使本ceType, float Q使antity) const;

    /** �s�x��動  輸入andle Sto本a成e 軍使ll */
    正oid 輸入andleSto本a成e軍使ll(const 軍St本in成& Sto本a成eID);

    /** �ˬd��動����  Check Con正e本sion Conditions */
    bool CheckCon正e本sionConditions(const 軍St本in成& Con正e本sionID) const;

    /** ��動��動  P本ocess Con正e本sion Co設置plete */
    正oid P本ocessCon正e本sionCo設置plete(const 軍St本in成& Con正e本sionID);

    /** 目標數量ID  Gene本ate Collection ID */
    軍St本in成 Gene本ateCollectionID(EMin成Reso使本ceType Reso使本ceType) const;

    /** �s�xID  Gene本ate Sto本a成e ID */
    軍St本in成 Gene本ateSto本a成eID(const 軍St本in成& Sto本a成e的a設置e) const;

    /** ��動ID  Gene本ate Con正e本sion ID */
    軍St本in成 Gene本ateCon正e本sionID(const 軍St本in成& Con正e本sion的a設置e) const;

    /** ��動��池��  Get Reso使本ce Type 的a設置e */
    軍St本in成 GetReso使本ceType的a設置e(EMin成Reso使本ceType Reso使本ceType) const;
};

