#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成Pe本fo本設置anceTest.成ene本ated.h"

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Pe本fo本設置anceTestMet本ics
{
    GE的ERATED下BODY()

    // 軍PS Met本ics
    UPROPERTY(Bl使ep本intReadOnly)
    float A正e本a成e軍PS;

    UPROPERTY(Bl使ep本intReadOnly)
    float Min軍PS;

    UPROPERTY(Bl使ep本intReadOnly)
    float Max軍PS;

    // Me設置o本y Met本ics
    UPROPERTY(Bl使ep本intReadOnly)
    float UsedMe設置o本yMB;

    UPROPERTY(Bl使ep本intReadOnly)
    float PeakMe設置o本yMB;

    // Unit Met本ics
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Acti正eUnitCo使nt;

    UPROPERTY(Bl使ep本intReadOnly)
    float A正e本a成eUnitUpdateTi設置e;

    // E正ent Syste設置 Met本ics
    UPROPERTY(Bl使ep本intReadOnly)
    int32 E正entsPe本Second;

    UPROPERTY(Bl使ep本intReadOnly)
    float A正e本a成eE正entLatency;

    // 的a正i成ation Met本ics
    UPROPERTY(Bl使ep本intReadOnly)
    float Pathfindin成Ti設置e;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 Acti正ePathReq使ests;

    // AI Met本ics
    UPROPERTY(Bl使ep本intReadOnly)
    float A正e本a成eAIUpdateTi設置e;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 AIQ使e本iesPe本Second;

    軍Min成Pe本fo本設置anceMet本ics()
        : A正e本a成e軍PS(0.0f)
        , Min軍PS(MAX下軍LT)
        , Max軍PS(0.0f)
        , UsedMe設置o本yMB(0.0f)
        , PeakMe設置o本yMB(0.0f)
        , Acti正eUnitCo使nt(0)
        , A正e本a成eUnitUpdateTi設置e(0.0f)
        , E正entsPe本Second(0)
        , A正e本a成eE正entLatency(0.0f)
        , Pathfindin成Ti設置e(0.0f)
        , Acti正ePathReq使ests(0)
        , A正e本a成eAIUpdateTi設置e(0.0f)
        , AIQ使e本iesPe本Second(0)
    {}
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成Pe本fo本設置anceTestType: 使int8 {
    UnitSpawnin成 UMETA(Display的a設置e = "Unit Spawnin成"),
    Mo正e設置entSt本ess UMETA(Display的a設置e = "Mo正e設置ent St本ess"),
    Co設置batSt本ess UMETA(Display的a設置e = "Co設置bat St本ess"),
    SelectionSt本ess UMETA(Display的a設置e = "Selection St本ess"),
    軍o本設置ationSt本ess UMETA(Display的a設置e = "軍o本設置ation St本ess"),
    軍使llSi設置使lation UMETA(Display的a設置e = "軍使ll Si設置使lation")
};

/**
 * Pe本fo本設置ance testin成 syste設置 fo本 Min成GoRTS
 * Validates 60fps ta本成et with 1000+ 使nits
 */
UCLASS(ClassG本o使p = (Testin成), Bl使ep本intable)
class MI的GTACTICAL下API UMin成Pe本fo本設置anceTest : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成Pe本fo本設置anceTest(};

    // Test Confi成使本ation
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Test Confi成")
    int32 Ta本成etUnitCo使nt;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Test Confi成")
    float Ta本成et軍PS;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Test Confi成")
    float TestD使本ation;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Test Confi成")
    EMin成Pe本fo本設置anceTestType TestType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Test Confi成")
    bool bA使toSta本tOnBe成inPlay;

    // Test Cont本ol
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Test Cont本ol")
    正oid Sta本tTest(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Test Cont本ol")
    正oid StopTest(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Test Cont本ol")
    正oid Pa使seTest(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Test Cont本ol")
    正oid Res使設置eTest(};

    // Test Actions
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Test Actions")
    正oid SpawnTestUnits(int32 Co使nt};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Test Actions")
    正oid Dest本oyAllTestUnits(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Test Actions")
    正oid St本essTestSelection(int32 SelectionCo使nt};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Test Actions")
    正oid St本essTestMo正e設置ent(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Test Actions")
    正oid St本essTestCo設置bat(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Test Actions")
    正oid Test軍o本設置ationSyste設置(int32 UnitCo使nt, EMin成軍o本設置ationType 軍o本設置ationType};

    // Res使lts
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Res使lts")
    const 軍Min成Pe本fo本設置anceMet本ics& GetC使本本entMet本ics() const { 本et使本n C使本本entMet本ics; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Res使lts")
    bool IsTestR使nnin成() const { 本et使本n bIsR使nnin成; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Res使lts")
    float GetTestP本o成本ess() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Res使lts")
    正oid Expo本tRes使ltsTo軍ile(const 軍St本in成& 軍ilePath};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Res使lts")
    軍St本in成 Gene本ateRepo本t() const;

    // Validation
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Validation")
    bool DidPass軍PSReq使i本e設置ent() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Validation")
    bool DidPassMe設置o本yReq使i本e設置ent() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Validation")
    bool DidPassAllReq使i本e設置ents() const;

    // Deb使成 Vis使alization
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Deb使成")
    正oid EnableDeb使成Vis使alization(bool bEnable};

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Deb使成")
    bool bShowDeb使成Info;

p本otected:
    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "State")
    bool bIsR使nnin成;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "State")
    bool bIsPa使sed;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "State")
    float TestSta本tTi設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "State")
    float TestElapsedTi設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "State")
    軍Min成Pe本fo本設置anceMet本ics C使本本entMet本ics;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "State")
    TA本本ay<軍Min成Pe本fo本設置anceMet本ics> Met本ics輸入isto本y;

    // 注基本：TA本本ay<T基本eakOb大ectPt本> 不被 Bl使ep本int 動
    TA本本ay<T基本eakOb大ectPt本<class AMin成TacticalUnit>> TestUnits;

    // Pe本fo本設置ance t本ackin成
    正oid UpdateMet本ics(float DeltaTi設置e};
    正oid T本ack軍PS(float DeltaTi設置e};
    正oid T本ackMe設置o本y(};
    正oid T本ackUnitPe本fo本設置ance(};
    正oid T本ackE正entSyste設置(};
    正oid T本ackAIPe本fo本設置ance(};

    // Test scena本ios
    正oid R使nUnitSpawnin成Test(float DeltaTi設置e};
    正oid R使nMo正e設置entSt本essTest(float DeltaTi設置e};
    正oid R使nCo設置batSt本essTest(float DeltaTi設置e};
    正oid R使nSelectionSt本essTest(float DeltaTi設置e};
    正oid R使n軍o本設置ationSt本essTest(float DeltaTi設置e};
    正oid R使n軍使llSi設置使lationTest(float DeltaTi設置e};

    // 輸入elpe本s
    軍Vecto本 GetRando設置SpawnLocation() const;
    軍Vecto本 GetRando設置Destination() const;
    正oid SelectRando設置Units(int32 Co使nt};
    正oid Iss使eRando設置Co設置設置ands(};

    UPROPERTY()
    T基本eakOb大ectPt本<class U基本o本ld> 基本o本ldContext;

    // Ti設置in成
    float LastMet本icsUpdateTi設置e;
    float Met本icsUpdateInte本正al;

    // 軍本a設置e t本ackin成
    int32 軍本a設置eCo使nt;
    float Total軍本a設置eTi設置e;
    float Min軍本a設置eTi設置e;
    float Max軍本a設置eTi設置e;

p使blic:
    // Static 使tility f使nctions
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utilities", 設置eta = (Static軍使nction = "t本使e"))
    static float GetC使本本ent軍PS(};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utilities", 設置eta = (Static軍使nction = "t本使e"))
    static float GetUsedMe設置o本yMB(};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utilities", 設置eta = (Static軍使nction = "t本使e"))
    static int32 GetOb大ectCo使nt(U基本o本ld* 基本o本ld, TS使bclassOf<AActo本> Acto本Class};
};

