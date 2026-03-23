#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成RTSSyste設置Test.成ene本ated.h"

class UMin成RTSUnitMana成e本;
class UMin成RTSCo設置batSyste設置;
class AMin成RTSAICont本olle本;
class UMin成RTSReso使本ceMana成e本;
class UMin成RTSB使ildin成Syste設置;
class UMin成RTSEcono設置icSyste設置;

/**
 * RTS系統測試動務 * 測試RTS系統故事重要性基本能
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GCORE下API UMin成RTSSyste設置Test : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    /**
     * 目標數量RTS系統測試
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static 正oid R使nAllRTSTests(};

    /**
     * 測試管務     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestUnitMana成e本(};

    /**
     * 測試動鬥系統
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestCo設置batSyste設置(};

    /**
     * 測試AI動制X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestAICont本olle本(};

    /**
     * 測試資動管務     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestReso使本ceMana成e本(};

    /**
     * 測試建動系統
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestB使ildin成Syste設置(};

    /**
     * 測試經動系統
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestEcono設置icSyste設置(};

    /**
     * 測試系統
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestSyste設置Inte成本ation(};

    /**
     * 動能測試
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestPe本fo本設置ance(};

    /**
     * 壓動測試
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestSt本essTest(};

    /**
     * 測試
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestBo使nda本yConditions(};

    /**
     * 動誤測試
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestE本本o本輸入andlin成(};

    /**
     * 動建測試
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static 正oid C本eateTestData(};

    /**
     * 清動測試
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static 正oid Clean使pTestData(};

    /**
     * 驗動系統動康動X     */
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本intP使本e, Cate成o本y = "RTS Syste設置 Test")
    static bool Ve本ifySyste設置輸入ealth(};

    /**
     * 測試
     */
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本intP使本e, Cate成o本y = "RTS Syste設置 Test")
    static 軍St本in成 GetTestRepo本t(};

    /**
     * 動池能
     */
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本intP使本e, Cate成o本y = "RTS Syste設置 Test")
    static 軍St本in成 Gene本atePe本fo本設置anceRepo本t(};

    /**
     * 測試故事重要性基本移X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestUnitSelectionAndMo正e設置ent(};

    /**
     * 測試X系統
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool Test軍o本設置ationSyste設置(};

    /**
     * 測試動鬥機制
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestCo設置batMechanics(};

    /**
     * 測試AI行為
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestAIBeha正io本(};

    /**
     * 測試資池產
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestReso使本ceP本od使ction(};

    /**
     * 測試建動建設
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestB使ildin成Const本使ction(};

    /**
     * 測試經動循環
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestEcono設置icCycle(};

    /**
     * 測試貿動系統
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestT本adeSyste設置(};

    /**
     * 測試市場機制
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Syste設置 Test")
    static bool TestMa本ketMechanics(};

p本i正ate:
    static bool bTestPassed;
    static 軍St本in成 TestRepo本t;
    static int32 TestsR使n;
    static int32 TestsPassed;

    static 正oid AddTestRes使lt(const 軍St本in成& Test的a設置e, bool bPassed, const 軍St本in成& Details = TEXT("")};
    static 正oid ResetTestRes使lts(};
    static 正oid Meas使本ePe本fo本設置ance(const 軍St本in成& Ope本ation的a設置e, T軍使nction<正oid()> Ope本ation};
};

