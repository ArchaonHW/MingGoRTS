#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成Sa正eGa設置eSyste設置Test.成ene本ated.h"

// 測試結果
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍TestRes使lt
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Test的a設置e;

    UPROPERTY(Bl使ep本intReadOnly)
    bool bPassed;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 E本本o本Messa成e;

    UPROPERTY(Bl使ep本intReadOnly)
    float Exec使tionTi設置e;
};

// 測試類別
UE的UM(Bl使ep本intType)
en使設置 class ETestCate成o本y: 使int8 {
    Sa正eLoad,       // 存取測試
    DataInte成本ity,  // 資料完整性
    Pe本fo本設置ance,    // 效能測試
    E本本o本輸入andlin成,  // 錯誤處理
    Co設置patibility   // 相容性測試
};

/**
 * 存檔系統測試類
 * 負責測試存檔系統的各種功能
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GPERSO的AL下API UMin成Sa正eGa設置eSyste設置Test : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    // 建構子
    UMin成Sa正eGa設置eSyste設置Test(};

    // 執行所有測試
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正eTest")
    bool R使nAllTests(};

    // 執行特定類別測試
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正eTest")
    bool R使nTestCate成o本y(ETestCate成o本y Cate成o本y};

    // 執行單一測試
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正eTest")
    軍TestRes使lt R使nSin成leTest(const 軍St本in成& Test的a設置e};

    // 獲取測試結果
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正eTest")
    TA本本ay<軍TestRes使lt> GetTestRes使lts() const;

    // 獲取測試統計
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正eTest")
    正oid GetTestStatistics(int32& TotalTests, int32& PassedTests, int32& 軍ailedTests, float& S使ccessRate) const;

    // 重置測試結果
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正eTest")
    正oid ResetTestRes使lts(};

p本otected:
    // 測試結果列表
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍TestRes使lt> TestRes使lts;

    // 測試計數器
    UPROPERTY(Bl使ep本intReadOnly)
    int32 TotalTestCo使nt;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 PassedTestCo使nt;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 軍ailedTestCo使nt;

    // 存檔管理器實例
    UPROPERTY(Bl使ep本intReadOnly)
    class UMin成Sa正eGa設置eMana成e本* TestSa正eMana成e本;

    // 基礎測試方法
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正eTest")
    bool TestBasicSa正eLoad(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正eTest")
    bool TestM使ltipleSa正eSlots(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正eTest")
    bool TestDataCo本本使ption(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正eTest")
    bool TestPe本fo本設置ance(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正eTest")
    bool TestE本本o本輸入andlin成(};

    // 輔助方法
    正oid AddTestCase(const 軍St本in成& Test的a設置e, const 軍St本in成& Desc本iption, bool bPassed, const 軍St本in成& E本本o本 = TEXT("")};

    // 創建測試存檔管理器
    UMin成Sa正eGa設置eMana成e本* C本eateTestSa正eGa設置eMana成e本(};

    // 清理測試資料
    正oid Clean使pTestData(};

    // 獲取測試存檔槽位
    int32 GetTestSa正eSlot() const { 本et使本n 99; }
};
