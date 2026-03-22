#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSBuildingUpgradeSystem.h"
#include "MingGoRTSBuildingUpgradeTest.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSBuildingUpgradeTest : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSBuildingUpgradeTest();

    // 建築升級系統實例
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade Test")
    UMingGoRTSBuildingUpgradeSystem* UpgradeSystem;

    // 初始化測試
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    void InitializeTest();

    // 測試建築升級系統初始化
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestBuildingUpgradeSystemInitialization();

    // 測試升級路徑創建
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePathCreation();

    // 測試建築升級流程
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestBuildingUpgradeFlow();

    // 測試升級暫停恢復
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePauseResume();

    // 測試升級取消
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeCancellation();

    // 測試立即升級
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestInstantUpgrade();

    // 測試升級優先級
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePriority();

    // 測試升級路徑解鎖
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePathUnlocking();

    // 測試升級效果應用
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeEffectsApplication();

    // 測試升級條件檢查
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeRequirementsCheck();

    // 測試升級成本計算
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeCostCalculation();

    // 測試升級時間計算
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeTimeCalculation();

    // 測試民國時期特色升級路徑
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestRepublicanEraUpgradePaths();

    // 測試升級統計
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeStatistics();

    // 測試升級配置
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeConfiguration();

    // 測試升級保存載入
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestSaveAndLoad();

    // 測試並行升級限制
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestConcurrentUpgradeLimit();

    // 測試升級預覽功能
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePreview();

    // 運行所有測試
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool RunAllTests();

    // 獲取測試結果
    UFUNCTION(BlueprintPure, Category = "Building Upgrade Test")
    FString GetTestResults() const;

    // 清理測試
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    void CleanupTest();

private:
    // 測試結果
    UPROPERTY()
    TArray<FString> TestResults;

    // 測試通過數量
    int32 PassedTests;

    // 測試失敗數量
    int32 FailedTests;

    // 添加測試結果
    void AddTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT(""));

    // 創建測試建築
    FString CreateTestBuilding(const FString& BuildingName, EBuildingType BuildingType);

    // 創建測試升級路徑
    FBuildingUpgradePath CreateTestUpgradePath(const FString& PathName, EBuildingType BuildingType);

    // 驗證升級路徑數據
    bool VerifyUpgradePathData(const FBuildingUpgradePath& UpgradePath, const FString& ExpectedName, EBuildingType ExpectedType);

    // 驗證升級進度
    bool VerifyUpgradeProgress(const FString& BuildingID, EBuildingUpgradeStatus ExpectedStatus);

    // 獲取隨機升級路徑ID
    FString GetRandomUpgradePathID(EBuildingType BuildingType) const;

    // 重置測試計數器
    void ResetTestCounters();

    // 創建測試配置
    FBuildingUpgradeSystemConfig CreateTestConfig();

    // 驗證配置應用
    bool VerifyConfigApplication(const FBuildingUpgradeSystemConfig& ExpectedConfig);

    // 等待升級完成
    bool WaitForUpgradeCompletion(const FString& BuildingID, float TimeoutSeconds = 10.0f);

    // 模擬時間流逝
    void SimulateTimeProgress(float DeltaTime);

    // 驗證升級效果
    bool VerifyUpgradeEffects(const FString& BuildingID, const TArray<FBuildingUpgradeEffect>& ExpectedEffects);

    // 創建測試升級效果
    TArray<FBuildingUpgradeEffect> CreateTestEffects();

    // 創建測試升級需求
    TArray<FBuildingUpgradeRequirement> CreateTestRequirements();
};
