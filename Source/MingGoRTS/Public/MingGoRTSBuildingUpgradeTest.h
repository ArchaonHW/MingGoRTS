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

    // 建?X???系統實??
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade Test")
    UMingGoRTSBuildingUpgradeSystem* UpgradeSystem;

    // ???X?測??    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    void InitializeTest();

    // 測試建?X???系統???X    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestBuildingUpgradeSystemInitialization();

    // 測試????路?X?建
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePathCreation();

    // 測試建?X???流??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestBuildingUpgradeFlow();

    // 測試???X??X?復
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePauseResume();

    // 測試???X???
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeCancellation();

    // 測試立即????
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestInstantUpgrade();

    // 測試???X?????    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePriority();

    // 測試????路???X
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePathUnlocking();

    // 測試???X??X?用
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeEffectsApplication();

    // 測試????條件檢查
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeRequirementsCheck();

    // 測試???X?本計??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeCostCalculation();

    // 測試???X???計??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeTimeCalculation();

    // 測試民?X??X?色????路??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestRepublicanEraUpgradePaths();

    // 測試????統??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeStatistics();

    // 測試???X?置
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeConfiguration();

    // 測試????保??載入
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestSaveAndLoad();

    // 測試並?X??X?制
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestConcurrentUpgradeLimit();

    // 測試???X?覽??能
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePreview();

    // ???X???測??    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool RunAllTests();

    // ????測試結??
    UFUNCTION(BlueprintPure, Category = "Building Upgrade Test")
    FString GetTestResults() const;

    // 清??測試
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    void CleanupTest();

private:
    // 測試結??
    UPROPERTY()
    TArray<FString> TestResults;

    // 測試???X???
    int32 PassedTests;

    // 測試失?X???
    int32 FailedTests;

    // 添??測試結??
    void AddTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT("")};

    // ??建測試建??
    FString CreateTestBuilding(const FString& BuildingName, EBuildingType BuildingType};

    // ??建測試????路??
    FBuildingUpgradePath CreateTestUpgradePath(const FString& PathName, EBuildingType BuildingType};

    // 驗?X???路?X???
    bool VerifyUpgradePathData(const FBuildingUpgradePath& UpgradePath, const FString& ExpectedName, EBuildingType ExpectedType};

    // 驗?X??X?度
    bool VerifyUpgradeProgress(const FString& BuildingID, EBuildingUpgradeStatus ExpectedStatus};

    // ???X??X???路??ID
    FString GetRandomUpgradePathID(EBuildingType BuildingType) const;

    // ??置測試計數X    void ResetTestCounters();

    // ??建測試??置
    FBuildingUpgradeSystemConfig CreateTestConfig();

    // 驗?X?置??用
    bool VerifyConfigApplication(const FBuildingUpgradeSystemConfig& ExpectedConfig};

    // 等?X???完??
    bool WaitForUpgradeCompletion(const FString& BuildingID, float TimeoutSeconds = 10.0f};

    // 模擬????流??    void SimulateTimeProgress(float DeltaTime};

    // 驗?X??X???
    bool VerifyUpgradeEffects(const FString& BuildingID, const TArray<FBuildingUpgradeEffect>& ExpectedEffects};

    // ??建測試???X???
    TArray<FBuildingUpgradeEffect> CreateTestEffects();

    // ??建測試???X???    TArray<FBuildingUpgradeRequirement> CreateTestRequirements();
};

