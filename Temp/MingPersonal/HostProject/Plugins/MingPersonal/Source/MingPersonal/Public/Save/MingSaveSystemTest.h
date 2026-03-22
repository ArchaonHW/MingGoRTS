#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSaveSystemTest.generated.h"

// Forward declarations
class UMingPersonalManager;
class UMingSaveGameManager;
class UMingRelationshipManager;

/**
 * 保存系統測試套件
 * 驗證保存和載入系統的核心功能
 */
UCLASS()
class MINGPERSONAL_API UMingSaveSystemTest : public UObject
{
    GENERATED_BODY()

public:
    UMingSaveSystemTest();

    // 運行所有測試
    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool RunAllTests();

    // 測試結果
    UFUNCTION(BlueprintPure, Category = "Save System Test")
    FString GetTestResults() const { return TestResults; }

    UFUNCTION(BlueprintPure, Category = "Save System Test")
    int32 GetPassedTests() const { return PassedTests; }

    UFUNCTION(BlueprintPure, Category = "Save System Test")
    int32 GetFailedTests() const { return FailedTests; }

    UFUNCTION(BlueprintPure, Category = "Save System Test")
    float GetTestPassRate() const;

    // 個別測試
    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestSaveGameCreation();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestSaveAndLoad();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestMultipleSaveSlots();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestQuickSave();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestDeleteSave();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestSaveSlotInfo();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestAutoSaveSettings();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestRelationshipDataPersistence();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestReputationDataPersistence();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestExportImport();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestSaveValidation();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestVersionCompatibility();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestCompressionEncryption();

private:
    // 測試結果日誌
    UPROPERTY()
    FString TestResults;

    UPROPERTY()
    int32 PassedTests;

    UPROPERTY()
    int32 FailedTests;

    UPROPERTY()
    TArray<FString> ErrorMessages;

    // 測試幫助函數
    void LogTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT(""));
    void ResetTestCounts();

    // 清理測試存檔
    void CleanupTestSaves();

    // 創建測試用的個人管理器
    UMingPersonalManager* CreateTestPersonalManager();

    // 創建測試用的保存管理器
    UMingSaveGameManager* CreateTestSaveManager();
};
