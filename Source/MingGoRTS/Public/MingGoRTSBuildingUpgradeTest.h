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

    // ��?X???�t�ι�??
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade Test")
    UMingGoRTSBuildingUpgradeSystem* UpgradeSystem;

    // ???X?��??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    void InitializeTest();

    // ���ի�?X???�t��???X
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestBuildingUpgradeSystemInitialization();

    // ����????��?X?��
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePathCreation();

    // ���ի�?X???�y??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestBuildingUpgradeFlow();

    // ����???X??X?�_
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePauseResume();

    // ����???X???
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeCancellation();

    // ���եߧY????
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestInstantUpgrade();

    // ����???X?????
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePriority();

    // ����????��???X
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePathUnlocking();

    // ����???X??X?��
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeEffectsApplication();

    // ����????�����ˬd
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeRequirementsCheck();

    // ����???X?���p??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeCostCalculation();

    // ����???X???�p??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeTimeCalculation();

    // ���ե�?X??X?��????��??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestRepublicanEraUpgradePaths();

    // ����????��??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeStatistics();

    // ����???X?�m
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeConfiguration();

    // ����????�O??���J
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestSaveAndLoad();

    // ���ը�?X??X?��
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestConcurrentUpgradeLimit();

    // ����???X?��??��
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePreview();

    // ???X???��??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool RunAllTests();

    // ????���յ�??
    UFUNCTION(BlueprintPure, Category = "Building Upgrade Test")
    FString GetTestResults() const;

    // �M??����
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    void CleanupTest();

private:
    // ���յ�??
    UPROPERTY()
    TArray<FString> TestResults;

    // ����???X???
    int32 PassedTests;

    // ���ե�?X???
    int32 FailedTests;

    // �K??���յ�??
    void AddTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT("")};

    // ??�ش��ի�??
    FString CreateTestBuilding(const FString& BuildingName, EBuildingType BuildingType);

    // ??�ش���????��??
    FBuildingUpgradePath CreateTestUpgradePath(const FString& PathName, EBuildingType BuildingType);

    // ��?X???��?X???
    bool VerifyUpgradePathData(const FBuildingUpgradePath& UpgradePath, const FString& ExpectedName, EBuildingType ExpectedType);

    // ��?X??X?��
    bool VerifyUpgradeProgress(const FString& BuildingID, EBuildingUpgradeStatus ExpectedStatus);

    // ???X??X???��??ID
    FString GetRandomUpgradePathID(EBuildingType BuildingType) const;

    // ??�m���խp��X
    void ResetTestCounters();

    // ??�ش���??�m
    FBuildingUpgradeSystemConfig CreateTestConfig();

    // ��?X?�m??��
    bool VerifyConfigApplication(const FBuildingUpgradeSystemConfig& ExpectedConfig);

    // ��?X???��??
    bool WaitForUpgradeCompletion(const FString& BuildingID, float TimeoutSeconds = 10.0f);

    // ����????�y??
    void SimulateTimeProgress(float DeltaTime);

    // ��?X??X???
    bool VerifyUpgradeEffects(const FString& BuildingID, const TArray<FBuildingUpgradeEffect>& ExpectedEffects);

    // ??�ش���???X???
    TArray<FBuildingUpgradeEffect> CreateTestEffects();

    // ??�ش���???X???
    TArray<FBuildingUpgradeRequirement> CreateTestRequirements();
};

