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
    UMingGoRTSBuildingUpgradeTest(};

    // å»ºç??‡ç?ç³»çµ±å¯¦ä?
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade Test")
    UMingGoRTSBuildingUpgradeSystem* UpgradeSystem;

    // ?å??–æ¸¬è©?    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    void InitializeTest(};

    // æ¸¬è©¦å»ºç??‡ç?ç³»çµ±?å???    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestBuildingUpgradeSystemInitialization(};

    // æ¸¬è©¦?‡ç?è·¯å??µå»º
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePathCreation(};

    // æ¸¬è©¦å»ºç??‡ç?æµç?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestBuildingUpgradeFlow(};

    // æ¸¬è©¦?‡ç??«å??¢å¾©
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePauseResume(};

    // æ¸¬è©¦?‡ç??–æ?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeCancellation(};

    // æ¸¬è©¦ç«‹å³?‡ç?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestInstantUpgrade(};

    // æ¸¬è©¦?‡ç??ªå?ç´?    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePriority(};

    // æ¸¬è©¦?‡ç?è·¯å?è§??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePathUnlocking(};

    // æ¸¬è©¦?‡ç??ˆæ??‰ç”¨
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeEffectsApplication(};

    // æ¸¬è©¦?‡ç?æ¢ä»¶æª¢æŸ¥
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeRequirementsCheck(};

    // æ¸¬è©¦?‡ç??æœ¬è¨ˆç?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeCostCalculation(};

    // æ¸¬è©¦?‡ç??‚é?è¨ˆç?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeTimeCalculation(};

    // æ¸¬è©¦æ°‘å??‚æ??¹è‰²?‡ç?è·¯å?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestRepublicanEraUpgradePaths(};

    // æ¸¬è©¦?‡ç?çµ±è?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeStatistics(};

    // æ¸¬è©¦?‡ç??ç½®
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradeConfiguration(};

    // æ¸¬è©¦?‡ç?ä¿å?è¼‰å…¥
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestSaveAndLoad(};

    // æ¸¬è©¦ä¸¦è??‡ç??åˆ¶
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestConcurrentUpgradeLimit(};

    // æ¸¬è©¦?‡ç??è¦½?Ÿèƒ½
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool TestUpgradePreview(};

    // ?‹è??€?‰æ¸¬è©?    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    bool RunAllTests(};

    // ?²å?æ¸¬è©¦çµæ?
    UFUNCTION(BlueprintPure, Category = "Building Upgrade Test")
    FString GetTestResults() const;

    // æ¸…ç?æ¸¬è©¦
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Test")
    void CleanupTest(};

private:
    // æ¸¬è©¦çµæ?
    UPROPERTY()
    TArray<FString> TestResults;

    // æ¸¬è©¦?šé??¸é?
    int32 PassedTests;

    // æ¸¬è©¦å¤±æ??¸é?
    int32 FailedTests;

    // æ·»å?æ¸¬è©¦çµæ?
    void AddTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT("")};

    // ?µå»ºæ¸¬è©¦å»ºç?
    FString CreateTestBuilding(const FString& BuildingName, EBuildingType BuildingType};

    // ?µå»ºæ¸¬è©¦?‡ç?è·¯å?
    FBuildingUpgradePath CreateTestUpgradePath(const FString& PathName, EBuildingType BuildingType};

    // é©—è??‡ç?è·¯å??¸æ?
    bool VerifyUpgradePathData(const FBuildingUpgradePath& UpgradePath, const FString& ExpectedName, EBuildingType ExpectedType};

    // é©—è??‡ç??²åº¦
    bool VerifyUpgradeProgress(const FString& BuildingID, EBuildingUpgradeStatus ExpectedStatus};

    // ?²å??¨æ??‡ç?è·¯å?ID
    FString GetRandomUpgradePathID(EBuildingType BuildingType) const;

    // ?ç½®æ¸¬è©¦è¨ˆæ•¸??    void ResetTestCounters(};

    // ?µå»ºæ¸¬è©¦?ç½®
    FBuildingUpgradeSystemConfig CreateTestConfig(};

    // é©—è??ç½®?‰ç”¨
    bool VerifyConfigApplication(const FBuildingUpgradeSystemConfig& ExpectedConfig};

    // ç­‰å??‡ç?å®Œæ?
    bool WaitForUpgradeCompletion(const FString& BuildingID, float TimeoutSeconds = 10.0f};

    // æ¨¡æ“¬?‚é?æµé€?    void SimulateTimeProgress(float DeltaTime};

    // é©—è??‡ç??ˆæ?
    bool VerifyUpgradeEffects(const FString& BuildingID, const TArray<FBuildingUpgradeEffect>& ExpectedEffects};

    // ?µå»ºæ¸¬è©¦?‡ç??ˆæ?
    TArray<FBuildingUpgradeEffect> CreateTestEffects(};

    // ?µå»ºæ¸¬è©¦?‡ç??€æ±?    TArray<FBuildingUpgradeRequirement> CreateTestRequirements(};
};

