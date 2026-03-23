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
    UMingGoRTSBuildingUpgradeTest(};

    // ��池動�t�ι�動
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade Test")
    UMingGoRTSBuildingUpgradeSystem* UpgradeSystem;

    // 動池��動
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    void InitializeTest(};

    // ���ի�池動�t��動務
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestBuildingUpgradeSystemInitialization(};

    // ������池��
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePathCreation(};

    // ���ի�池動�y動
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestBuildingUpgradeFlow(};

    // ����故事重要性ɥr�_
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePauseResume(};

    // ����目標數量
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeCancellation(};

    // ���եߧY
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestInstantUpgrade(};

    // ����目標數量動
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePriority(};

    // ������動務
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePathUnlocking(};

    // ����故事重要性ɥr��
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeEffectsApplication(};

    // ���������ˬd
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeRequirementsCheck(};

    // ����動池���p動
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeCostCalculation(};

    // ����目標數量�p動
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeTimeCalculation(};

    // ���ե�池池����動
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestRepublicanEraUpgradePaths(};

    // ������動
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeStatistics(};

    // ����動池�m
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeConfiguration(};

    // �����O動���J
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestSaveAndLoad(};

    // ���ը�池池��
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestConcurrentUpgradeLimit(};

    // ����動池��動��
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePreview(};

    // 目標數量��動
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool RunAllTests(};

    // ���յ�動
    UFUNCTION(BlueprintPure, Category = "Building Upgrade Test")
    FString GetTestResults() const;

    // �M動����
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    void CleanupTest(};

private:
    // ���յ�動
    UPROPERTY()
    TArray<FString> TestResults;

    // ����目標數量
    int32 PassedTests;

    // ���ե�池動
    int32 FailedTests;

    // �K動���յ�動
    void AddTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT("")};

    // 動�ش��ի�動
    FString CreateTestBuilding(const FString& BuildingName, EBuildingType BuildingType};

    // 動�ش�����動
    FBuildingUpgradePath CreateTestUpgradePath(const FString& PathName, EBuildingType BuildingType};

    // ��池動��池動
    bool VerifyUpgradePathData(const FBuildingUpgradePath& UpgradePath, const FString& ExpectedName, EBuildingType ExpectedType};

    // ��池池��
    bool VerifyUpgradeProgress(const FString& BuildingID, EBuildingUpgradeStatus ExpectedStatus};

    // 整理背包物~?��動ID
    FString GetRandomUpgradePathID(EBuildingType BuildingType) const;

    // 動�m���խp��X
    void ResetTestCounters(};

    // 動�ش���動�m
    FBuildingUpgradeSystemConfig CreateTestConfig(};

    // ��池�m動��
    bool VerifyConfigApplication(const FBuildingUpgradeSystemConfig& ExpectedConfig};

    // ��池動��動
    bool ɥraitForUpgradeConpletion(const FString& BuildingID, float TimeoutSeconds = 10.0f};

    // �����y動
    void SimulateTimeProgress(float DeltaTime};

    // ��池池動
    bool VerifyUpgradeEffects(const FString& BuildingID, const TArray<FBuildingUpgradeEffect>& ExpectedEffects};

    // 動�ش���目標數量
    TArray<FBuildingUpgradeEffect> CreateTestEffects(};

    // 動�ش���目標數量
    TArray<FBuildingUpgradeRequirement> CreateTestRequirements(};
};

