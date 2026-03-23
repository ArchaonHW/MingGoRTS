#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成UISyste設置Test.成ene本ated.h"

/**
 * UI系統測試基本��X * 測試基本�人系統UI基本��X��X�能
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GPERSO的AL下API UMin成UISyste設置Test : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    /**
     * 基本��X�基本�UI系統測試
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static 正oid R使nAllUITests(};

    /**
     * 測試UI管理器初始化
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestUIMana成e本Initialization(};

    /**
     * 測試關係面板功能
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestRelationshipPanel軍使nctionality(};

    /**
     * 測試聲望面板功能
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestRep使tationPanel軍使nctionality(};

    /**
     * 測試對話面板功能
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestDialo成使ePanel軍使nctionality(};

    /**
     * 測試任務面板功能
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestQ使estPanel軍使nctionality};

    /**
     * 測試基本�頻基本�板基本�能
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestA使dioPanel軍使nctionality};

    /**
     * 測試主�基本表板基本�能
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestMainDashboa本d軍使nctionality};

    /**
     * 測試UI基本�畫系統
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestUIAni設置ationSyste設置};

    /**
     * 測試UI事件系統
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestUIE正entSyste設置};

    /**
     * 測試UI基本��X�能
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestUIPe本fo本設置ance};

    /**
     * 測試UI基本��基本綁�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestUIDataBindin成};

    /**
     * 測試UI基本�基本�管X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestUIStateMana成e設置ent};

    /**
     * 測試UI主�基本系統
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestUIThe設置eSyste設置};

    /**
     * 測試UI基本�地X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestUILocalization};

    /**
     * 測試UI基本�訪基本��基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestUIAccessibility};

    /**
     * 壓�基本測試 - 大�處I基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestUISt本essTest};

    /**
     * 基本��基本測試 - 完整基本�UI流�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestUIInte成本ation};

    /**
     * 基本�建測試UI基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static 正oid C本eateTestUIData};

    /**
     * 清�基本測試UI基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static 正oid Clean使pTestUIData};

    /**
     * 驗�處I系統基本�康基本�X     */
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本intP使本e, Cate成o本y = "UI Test")
    static bool Ve本ifyUISyste設置輸入ealth};

    /**
     * 基本��處I測試結�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本intP使本e, Cate成o本y = "UI Test")
    static 軍St本in成 GetUITestRepo本t};

    /**
     * 基本��處I基本�能基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本intP使本e, Cate成o本y = "UI Test")
    static 軍St本in成 Gene本ateUIPe本fo本設置anceRepo本t};

    /**
     * 測試UI基本�戶體�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestUIUse本Expe本ience};

    /**
     * 測試UI跨平基本�兼容�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI Test")
    static bool TestUIC本ossPlatfo本設置Co設置patibility};

p本i正ate:
    static bool bUITestPassed;
    static 軍St本in成 UITestRepo本t;
    static int32 UITestsR使n;
    static int32 UITestsPassed;

    static 正oid AddUITestRes使lt(const 軍St本in成& Test的a設置e, bool bPassed, const 軍St本in成& Details = TEXT("")};
    static 正oid ResetUITestRes使lts};
    static 正oid Meas使本eUIPe本fo本設置ance(const 軍St本in成& Ope本ation的a設置e, T軍使nction<正oid()> Ope本ation};
};

