#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSBuildingUpgradeSystem.h"
#include "MingGoRTSBuildingUpgradeTest.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSBuildingUpgradeTest : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSBuildingUpgradeTest();

    // ��?X???�t�ι�??
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade Test")
    UMingGoRTSBuildingUpgradeSystem* UpgradeSystem;

    // ???X?��??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    void InitializeTest();

    // ���ի�?X???�t��???X
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestBuildingUpgradeSystemInitialization();

    // ����摧毀��?X?��
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePathCreation();

    // ���ի�?X???�y??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestBuildingUpgradeFlow();

    // ����故事重要性?�_
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePauseResume();

    // ����目標數量
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeCancellation();

    // ���եߧY摧毀
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestInstantUpgrade();

    // ����目標數量??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePriority();

    // ����摧毀��???X
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePathUnlocking();

    // ����故事重要性?��
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeEffectsApplication();

    // ����摧毀�����ˬd
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeRequirementsCheck();

    // ����???X?���p??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeCostCalculation();

    // ����目標數量�p??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeTimeCalculation();

    // ���ե�?X??X?��摧毀��??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestRepublicanEraUpgradePaths();

    // ����摧毀��??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeStatistics();

    // ����???X?�m
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeConfiguration();

    // ����摧毀�O??���J
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestSaveAndLoad();

    // ���ը�?X??X?��
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestConcurrentUpgradeLimit();

    // ����???X?��??��
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePreview();

    // 目標數量��??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool RunAllTests();

    // 摧毀���յ�??
    UFUNCTION(BlueprintPure, Category = "Building Upgrade Test")
    FString GetTestResults() const;

    // �M??����
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    void CleanupTest();

private:
    // ���յ�??
    UPROPERTY()
    TArray<FString> TestResults;

    // ����目標數量
    int32 PassedTests;

    // ���ե�?X???
    int32 FailedTests;

    // �K??���յ�??
    void AddTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT("")};

    // ??�ش��ի�??
    FString CreateTestBuilding(const FString& BuildingName, EBuildingType BuildingType);

    // ??�ش���摧毀��??
    FBuildingUpgradePath CreateTestUpgradePath(const FString& PathName, EBuildingType BuildingType);

    // ��?X???��?X???
    bool VerifyUpgradePathData(const FBuildingUpgradePath& UpgradePath, const FString& ExpectedName, EBuildingType ExpectedType);

    // ��?X??X?��
    bool VerifyUpgradeProgress(const FString& BuildingID, EBuildingUpgradeStatus ExpectedStatus);

    // 整理背包物品��??ID
    FString GetRandomUpgradePathID(EBuildingType BuildingType) const;

    // ??�m���խp��X
    void ResetTestCounters();

    // ??�ش���??�m
    FBuildingUpgradeSystemConfig CreateTestConfig();

    // ��?X?�m??��
    bool VerifyConfigApplication(const FBuildingUpgradeSystemConfig& ExpectedConfig);

    // ��?X???��??
    bool WaitForUpgradeCompletion(const FString& BuildingID, float TimeoutSeconds = 10.0f);

    // ����摧毀�y??
    void SimulateTimeProgress(float DeltaTime);

    // ��?X??X???
    bool VerifyUpgradeEffects(const FString& BuildingID, const TArray<FBuildingUpgradeEffect>& ExpectedEffects);

    // ??�ش���目標數量
    TArray<FBuildingUpgradeEffect> CreateTestEffects();

    // ??�ش���目標數量
    TArray<FBuildingUpgradeRequirement> CreateTestRequirements();
};

