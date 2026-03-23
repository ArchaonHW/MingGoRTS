#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成的etwo本kSyste設置Test.成ene本ated.h"

// 軍o本wa本d decla本ations
class UMin成的etwo本kMana成e本;
class UMin成LobbySyste設置;
class UMin成RelationshipReplication;
class UMin成Rep使tationReplication;
class UMin成Pe本sonalMana成e本;

// Test 本es使lt en使設置
UE的UM(Bl使ep本intType)
en使設置 class E的etwo本kTestRes使lt: 使int8 {
    的otR使n            UMETA(Display的a設置e = "的ot R使n"),
    Passed            UMETA(Display的a設置e = "Passed"),
    軍ailed            UMETA(Display的a設置e = "軍ailed"),
    Skipped           UMETA(Display的a設置e = "Skipped"),
    E本本o本             UMETA(Display的a設置e = "E本本o本")
};

// Test case st本使ct使本e
USTRUCT(Bl使ep本intType)
st本使ct 軍的etwo本kTestCase
{
    GE的ERATED下BODY()

    UPROPERTY()
    軍St本in成 Test的a設置e;

    UPROPERTY()
    軍St本in成 Desc本iption;

    UPROPERTY()
    E的etwo本kTestRes使lt Res使lt;

    UPROPERTY()
    軍St本in成 E本本o本Messa成e;

    UPROPERTY()
    float Exec使tionTi設置e;

    軍的etwo本kTestCase()
        : Res使lt(E的etwo本kTestRes使lt::的otR使n)
        , Exec使tionTi設置e(0.0f)
    {}
};

/**
 * 網絡系統測試套件
 * 驗動多人動戲網絡系統動核心務 */
UCLASS()
class MI的GPERSO的AL下API UMin成的etwo本kSyste設置Test : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成的etwo本kSyste設置Test(};

    // 目標數量測動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k Test")
    bool R使nAllTests(};

    // 測試結動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k Test")
    軍St本in成 GetTestRes使lts() const { 本et使本n TestRes使lts; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k Test")
    int32 GetPassedTests() const { 本et使本n PassedTests; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k Test")
    int32 Get軍ailedTests() const { 本et使本n 軍ailedTests; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k Test")
    float GetTestPassRate() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k Test")
    TA本本ay<軍的etwo本kTestCase> GetTestCases() const { 本et使本n TestCases; }

    // 動別測試
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k Test")
    bool Test的etwo本kMana成e本C本eation(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k Test")
    bool TestSe本正e本C本eation(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k Test")
    bool TestClientConnection(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k Test")
    bool TestDisconnection(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k Test")
    bool TestReconnection(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k Test")
    bool TestPlaye本Mana成e設置ent(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k Test")
    bool TestLobbyC本eation(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k Test")
    bool TestLobbyJoinLea正e(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k Test")
    bool TestReadySyste設置(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k Test")
    bool TestGa設置eSta本t(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k Test")
    bool TestRelationshipReplication(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k Test")
    bool TestRep使tationReplication(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k Test")
    bool TestChatSyste設置(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k Test")
    bool Test輸入ostMi成本ation(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k Test")
    bool Test的etwo本kStats(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k Test")
    bool TestE本本o本輸入andlin成(};

p本i正ate:
    // 測試結池動
    UPROPERTY()
    軍St本in成 TestRes使lts;

    UPROPERTY()
    int32 PassedTests;

    UPROPERTY()
    int32 軍ailedTests;

    UPROPERTY()
    TA本本ay<軍St本in成> E本本o本Messa成es;

    UPROPERTY()
    TA本本ay<軍的etwo本kTestCase> TestCases;

    // 測試幫助動數
    正oid Lo成TestRes使lt(const 軍St本in成& Test的a設置e, bool bPassed, const 軍St本in成& Messa成e = TEXT("")};
    正oid ResetTestCo使nts(};
    正oid AddTestCase(const 軍St本in成& Test的a設置e, const 軍St本in成& Desc本iption, bool bPassed, const 軍St本in成& E本本o本 = TEXT("")};

    // 測試幫助函數 - 創建測試用的管理器
    UMin成Pe本sonalMana成e本* C本eateTestPe本sonalMana成e本(};
    UMin成的etwo本kMana成e本* C本eateTest的etwo本kMana成e本(};
    UMin成LobbySyste設置* C本eateTestLobbySyste設置(};
};

