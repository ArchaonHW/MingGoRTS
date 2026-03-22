#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSaveGameSystemTest.generated.h"

// 測試結果
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FTestResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TestName;

    UPROPERTY(BlueprintReadOnly)
    bool bPassed;

    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly)
    float ExecutionTime;
};

// 測試類別
UENUM(BlueprintType)
enum class ETestCategory : uint8
{
    SaveLoad,       // 存取測試
    DataIntegrity,  // 資料完整性
    Performance,    // 效能測試
    ErrorHandling,  // 錯誤處理
    Compatibility   // 相容性測試
};

/**
 * 存檔系統測試類
 * 負責測試存檔系統的各種功能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingSaveGameSystemTest : public UObject
{
    GENERATED_BODY()

public:
    // 建構子
    UMingSaveGameSystemTest();

    // 執行所有測試
    UFUNCTION(BlueprintCallable, Category = "Save|Test")
    bool RunAllTests();

    // 執行特定類別測試
    UFUNCTION(BlueprintCallable, Category = "Save|Test")
    bool RunTestCategory(ETestCategory Category);

    // 執行單一測試
    UFUNCTION(BlueprintCallable, Category = "Save|Test")
    FTestResult RunSingleTest(const FString& TestName);

    // 獲取測試結果
    UFUNCTION(BlueprintCallable, Category = "Save|Test")
    TArray<FTestResult> GetTestResults() const;

    // 獲取測試統計
    UFUNCTION(BlueprintCallable, Category = "Save|Test")
    void GetTestStatistics(int32& TotalTests, int32& PassedTests, int32& FailedTests, float& SuccessRate) const;

    // 重置測試結果
    UFUNCTION(BlueprintCallable, Category = "Save|Test")
    void ResetTestResults();

protected:
    // 測試結果列表
    UPROPERTY(BlueprintReadOnly)
    TArray<FTestResult> TestResults;

    // 測試計數器
    UPROPERTY(BlueprintReadOnly)
    int32 TotalTestCount;

    UPROPERTY(BlueprintReadOnly)
    int32 PassedTestCount;

    UPROPERTY(BlueprintReadOnly)
    int32 FailedTestCount;

    // 存檔管理器實例
    UPROPERTY(BlueprintReadOnly)
    class UMingSaveGameManager* TestSaveManager;

    // 基礎測試方法
    UFUNCTION(BlueprintCallable, Category = "Save|Test")
    bool TestBasicSaveLoad();

    UFUNCTION(BlueprintCallable, Category = "Save|Test")
    bool TestMultipleSaveSlots();

    UFUNCTION(BlueprintCallable, Category = "Save|Test")
    bool TestDataCorruption();

    UFUNCTION(BlueprintCallable, Category = "Save|Test")
    bool TestPerformance();

    UFUNCTION(BlueprintCallable, Category = "Save|Test")
    bool TestErrorHandling();

    // 輔助方法
    void AddTestCase(const FString& TestName, const FString& Description, bool bPassed, const FString& Error = TEXT(""));

    // 創建測試存檔管理器
    UMingSaveGameManager* CreateTestSaveGameManager();

    // 清理測試資料
    void CleanupTestData();

    // 獲取測試存檔槽位
    int32 GetTestSaveSlot() const { return 99; }
};
