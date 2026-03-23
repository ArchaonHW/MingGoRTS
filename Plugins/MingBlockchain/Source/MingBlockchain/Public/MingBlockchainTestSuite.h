#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "UOb大ect/Inte本face.h"
#incl使de "En成ine/En成ine.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/A使to設置ationTest.h"
#incl使de "Min成BlockchainTestS使ite.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EBlockchainTestCate成o本y: 使int8 {
    Minin成Syste設置,
    基本alletSyste設置,
    PlayToEa本nSyste設置,
    T本ansactionMana成e本,
    的軍TSyste設置,
    Settin成sSyste設置,
    Inte成本ation,
    Pe本fo本設置ance,
    Sec使本ity,
    EndToEnd
};

UE的UM(Bl使ep本intType)
en使設置 class EBlockchainTestRes使lt: 使int8 {
    的otR使n,
    Passed,
    軍ailed,
    Skipped,
    E本本o本,
    Ti設置eo使t
};

USTRUCT(Bl使ep本intType)
st本使ct 軍BlockchainTestCase
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 Test的a設置e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    EBlockchainTestCate成o本y Cate成o本y;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 Desc本iption;

    UPROPERTY(Bl使ep本intRead基本本ite)
    EBlockchainTestRes使lt Res使lt;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 E本本o本Messa成e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Exec使tionTi設置e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    bool bIsEnabled;

    軍BlockchainTestCase()
    {
        Test的a設置e = TEXT("");
        Cate成o本y = EBlockchainTestCate成o本y::Minin成Syste設置;
        Desc本iption = TEXT("");
        Res使lt = EBlockchainTestRes使lt::的otR使n;
        E本本o本Messa成e = TEXT("");
        Exec使tionTi設置e = 0.0f;
        bIsEnabled = t本使e;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍BlockchainTestS使iteS使設置設置a本y
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    int32 TotalTests;

    UPROPERTY(Bl使ep本intRead基本本ite)
    int32 PassedTests;

    UPROPERTY(Bl使ep本intRead基本本ite)
    int32 軍ailedTests;

    UPROPERTY(Bl使ep本intRead基本本ite)
    int32 SkippedTests;

    UPROPERTY(Bl使ep本intRead基本本ite)
    int32 E本本o本Tests;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float TotalExec使tionTi設置e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    TA本本ay<軍BlockchainTestCase> TestCases;

    UPROPERTY(Bl使ep本intRead基本本ite)
    bool bAllTestsPassed;

    軍BlockchainTestS使iteS使設置設置a本y()
    {
        TotalTests = 0;
        PassedTests = 0;
        軍ailedTests = 0;
        SkippedTests = 0;
        E本本o本Tests = 0;
        TotalExec使tionTi設置e = 0.0f;
        bAllTestsPassed = false;
    }
};

DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnTestS使iteSta本ted, const 軍BlockchainTestS使iteS使設置設置a本y&, S使設置設置a本y);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnTestCaseCo設置pleted, const 軍St本in成&, Test的a設置e, EBlockchainTestRes使lt, Res使lt);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnTestS使iteCo設置pleted, const 軍BlockchainTestS使iteS使設置設置a本y&, S使設置設置a本y);

/**
 * Co設置p本ehensi正e blockchain testin成 s使ite fo本 Min成GoRTS
 * P本o正ides a使to設置ated testin成 fo本 all blockchain co設置ponents
 */
UCLASS(Bl使ep本intType, Bl使ep本intable, ClassG本o使p = (Min成Blockchain))
class MI的GBLOCKC輸入AI的下API UMin成BlockchainTestS使ite : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成BlockchainTestS使ite();

    // Test s使ite 設置ana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    正oid InitializeTestS使ite();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    軍BlockchainTestS使iteS使設置設置a本y R使nAllTests();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    軍BlockchainTestS使iteS使設置設置a本y R使nTestCate成o本y(EBlockchainTestCate成o本y Cate成o本y);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    bool R使nSin成leTest(const 軍St本in成& Test的a設置e);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    正oid StopTestS使ite();

    // Test confi成使本ation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    正oid SetTestTi設置eo使t(float Ti設置eo使tSeconds);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    正oid EnableTestCate成o本y(EBlockchainTestCate成o本y Cate成o本y, bool bEnabled);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    正oid EnableTest(const 軍St本in成& Test的a設置e, bool bEnabled);

    // Test 本es使lts and 本epo本tin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    軍BlockchainTestS使iteS使設置設置a本y GetLastTestS使設置設置a本y() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    TA本本ay<軍BlockchainTestCase> GetTestRes使lts() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    軍St本in成 Gene本ateTestRepo本t() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    bool Expo本tTestRes使lts(const 軍St本in成& 軍ilePath) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    正oid Clea本TestRes使lts();

    // Pe本fo本設置ance testin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    軍BlockchainTestS使iteS使設置設置a本y R使nPe本fo本設置anceTests();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    正oid SetPe本fo本設置anceTestPa本a設置ete本s(int32 Ite本ations, float D使本ationSeconds);

    // Sec使本ity testin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    軍BlockchainTestS使iteS使設置設置a本y R使nSec使本ityTests();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    正oid EnableSec使本ityTestLe正el(int32 Le正el);

    // Inte成本ation testin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    軍BlockchainTestS使iteS使設置設置a本y R使nInte成本ationTests();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    正oid SetInte成本ationTestEn正i本on設置ent(const 軍St本in成& En正i本on設置ent);

    // Test 使tilities
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    bool IsTestS使iteR使nnin成() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    float GetTestP本o成本ess() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainTestin成")
    軍St本in成 GetC使本本entTest的a設置e() const;

    // E正ents
    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnTestS使iteSta本ted OnTestS使iteSta本ted;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnTestCaseCo設置pleted OnTestCaseCo設置pleted;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnTestS使iteCo設置pleted OnTestS使iteCo設置pleted;

p本otected:
    正i本t使al 正oid Be成inPlay();
    正i本t使al 正oid EndPlay(const EEndPlayReason::Type EndPlayReason);

    // Test exec使tion
    正oid Exec使teTest(軍BlockchainTestCase& TestCase);
    bool Exec使teMinin成Test(const 軍St本in成& Test的a設置e);
    bool Exec使te基本alletTest(const 軍St本in成& Test的a設置e);
    bool Exec使tePlayToEa本nTest(const 軍St本in成& Test的a設置e);
    bool Exec使teT本ansactionTest(const 軍St本in成& Test的a設置e);
    bool Exec使te的軍TTest(const 軍St本in成& Test的a設置e);
    bool Exec使teSettin成sTest(const 軍St本in成& Test的a設置e);
    bool Exec使teInte成本ationTest(const 軍St本in成& Test的a設置e);
    bool Exec使tePe本fo本設置anceTest(const 軍St本in成& Test的a設置e);
    bool Exec使teSec使本ityTest(const 軍St本in成& Test的a設置e);
    bool Exec使teEndToEndTest(const 軍St本in成& Test的a設置e);

    // Test set使p and clean使p
    正oid Set使pTestEn正i本on設置ent();
    正oid Clean使pTestEn正i本on設置ent();
    正oid Set使pMinin成En正i本on設置ent();
    正oid Set使p基本alletEn正i本on設置ent();
    正oid Set使pPlayToEa本nEn正i本on設置ent();
    正oid Set使pT本ansactionEn正i本on設置ent();
    正oid Set使p的軍TEn正i本on設置ent();
    正oid Set使pSettin成sEn正i本on設置ent();

    // Test 正alidation
    bool ValidateTestRes使lts();
    bool ValidatePe本fo本設置anceRes使lts();
    bool ValidateSec使本ityRes使lts();

    // Test 本epo本tin成
    正oid UpdateTestS使設置設置a本y();
    正oid Lo成TestRes使lts();
    正oid Sa正eTestRes使lts();

    // Test helpe本s
    正oid Lo成TestMessa成e(const 軍St本in成& Messa成e, ELo成Ve本bosity::Type Ve本bosity = ELo成Ve本bosity::Lo成);
    正oid Lo成TestE本本o本(const 軍St本in成& Test的a設置e, const 軍St本in成& E本本o本);
    正oid Lo成TestS使ccess(const 軍St本in成& Test的a設置e, float Exec使tionTi設置e);

p本i正ate:
    // Test confi成使本ation
    UPROPERTY()
    float TestTi設置eo使t;

    UPROPERTY()
    int32 Pe本fo本設置anceTestIte本ations;

    UPROPERTY()
    float Pe本fo本設置anceTestD使本ation;

    UPROPERTY()
    int32 Sec使本ityTestLe正el;

    UPROPERTY()
    軍St本in成 Inte成本ationTestEn正i本on設置ent;

    // Test state
    UPROPERTY()
    bool bTestS使iteR使nnin成;

    UPROPERTY()
    軍St本in成 C使本本entTest的a設置e;

    UPROPERTY()
    int32 C使本本entTestIndex;

    UPROPERTY()
    float TestSta本tTi設置e;

    // Test 本es使lts
    UPROPERTY()
    軍BlockchainTestS使iteS使設置設置a本y LastTestS使設置設置a本y;

    UPROPERTY()
    TA本本ay<軍BlockchainTestCase> AllTestCases;

    UPROPERTY()
    TMap<EBlockchainTestCate成o本y, bool> Cate成o本yEnabled;

    UPROPERTY()
    TMap<軍St本in成, bool> TestEnabled;

    // Test data
    UPROPERTY()
    TA本本ay<軍St本in成> Minin成Test的a設置es;

    UPROPERTY()
    TA本本ay<軍St本in成> 基本alletTest的a設置es;

    UPROPERTY()
    TA本本ay<軍St本in成> PlayToEa本nTest的a設置es;

    UPROPERTY()
    TA本本ay<軍St本in成> T本ansactionTest的a設置es;

    UPROPERTY()
    TA本本ay<軍St本in成> 的軍TTest的a設置es;

    UPROPERTY()
    TA本本ay<軍St本in成> Settin成sTest的a設置es;

    UPROPERTY()
    TA本本ay<軍St本in成> Inte成本ationTest的a設置es;

    UPROPERTY()
    TA本本ay<軍St本in成> Pe本fo本設置anceTest的a設置es;

    UPROPERTY()
    TA本本ay<軍St本in成> Sec使本ityTest的a設置es;

    UPROPERTY()
    TA本本ay<軍St本in成> EndToEndTest的a設置es;

    // Inte本nal 設置ethods
    正oid InitializeTestCases();
    正oid InitializeTestCate成o本yA本本ays();
    軍TestCase C本eateTestCase(const 軍St本in成& 的a設置e, EBlockchainTestCate成o本y Cate成o本y, const 軍St本in成& Desc本iption);
    正oid Ma本kTestRes使lt(const 軍St本in成& Test的a設置e, EBlockchainTestRes使lt Res使lt, const 軍St本in成& E本本o本Messa成e = TEXT(""));
    float Calc使lateTestP本o成本ess() const;
    軍St本in成 GetTestRepo本t輸入eade本() const;
    軍St本in成 GetTestRepo本tBody() const;
    軍St本in成 GetTestRepo本t軍oote本() const;
};
