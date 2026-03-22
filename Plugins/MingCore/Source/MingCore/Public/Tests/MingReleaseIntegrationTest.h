#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingReleaseIntegrationTest.generated.h"

UENUM(BlueprintType)
enum class EReleaseTestCategory : uint8
{
    Epic1_RTSCombat,        // Epic 1.1: RTS ?∞È¨•Á≥ªÁµ±
    Epic2_Economic,         // Epic 2.1: Á∂ìÊ?Á≥ªÁµ±
    Epic3_AICampaign,       // Epic 3.1: AI ?åÊà∞ÂΩπÁ≥ªÁµ?    Epic4_UIAudio,          // Epic 4.1: UI ?åÈü≥?ªÁ≥ªÁµ?    Epic5_Network,          // Epic 5.1: Á∂≤Áµ°?åÂ?‰∫∫È??≤Á≥ªÁµ?    Epic6_SaveLoad,         // Epic 6.1: ‰øùÂ??åË??•Á≥ªÁµ?    Epic7_Localization,     // Epic 7.1: ?¨Âú∞?ñÂ??ãÈ??ñÁ≥ªÁµ?    Epic8_Performance,      // Epic 8.1: ?ßËÉΩ?™Â??åË™øË©¶Á≥ªÁµ?    Epic9_AIContentGen,     // Epic 9: AI ?ßÂÆπ?üÊ?Á≥ªÁµ±
    Epic10_FourLayer,       // Epic 5: ?õÂ±§Á≠ñÁï•?¥Â?Á≥ªÁµ±
    SystemIntegration,      // ?®Á≥ªÁµ±Êï¥??    FinalValidation         // ?ÄÁµÇÈ?Ë≠?};

UENUM(BlueprintType)
enum class EReleaseTestResult : uint8
{
    NotRun,
    Passed,
    Failed,
    Warning,
    Critical,
    Skipped
};

USTRUCT(BlueprintType)
struct FReleaseTestResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TestName;

    UPROPERTY(BlueprintReadOnly)
    EReleaseTestCategory Category;

    UPROPERTY(BlueprintReadOnly)
    EReleaseTestResult Result;

    UPROPERTY(BlueprintReadOnly)
    FString Message;

    UPROPERTY(BlueprintReadOnly)
    float ExecutionTime;

    UPROPERTY(BlueprintReadOnly)
    int32 EpicNumber;

    UPROPERTY(BlueprintReadOnly)
    FDateTime ExecutionTimeStamp;

    FReleaseTestResult()
    {
        Result = EReleaseTestResult::NotRun;
        ExecutionTime = 0.0f;
        EpicNumber = 0;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReleaseTestCompleted, const FString&, TestName, const FReleaseTestResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReleaseTestSuiteCompleted, const TArray<FReleaseTestResult>&, AllResults);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEpicTestCompleted, int32, EpicNumber);

/**
 * Release ?àÊú¨?ÄÁµÇÈ??êÊ∏¨Ë©¶Â?‰ª? * Ê∏¨Ë©¶?Ä??Epic Á≥ªÁµ±?ÑÂ??¥ÊÄßÂ??¥Â??? */
UCLASS()
class MINGPERSONAL_API UMingReleaseIntegrationTest : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UMingReleaseIntegrationTest();

    // ========== ?ùÂ???==========
    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void InitializeReleaseTestSuite();

    // ========== Epic 1: RTS ?∞È¨•Á≥ªÁµ±Ê∏¨Ë©¶ ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 1")
    FReleaseTestResult TestEpic1_RTSUnitSelection();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 1")
    FReleaseTestResult TestEpic1_RTSPathfinding();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 1")
    FReleaseTestResult TestEpic1_RTSCombatSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 1")
    FReleaseTestResult TestEpic1_RTSAIController();

    // ========== Epic 2: Á∂ìÊ?Á≥ªÁµ±Ê∏¨Ë©¶ ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 2")
    FReleaseTestResult TestEpic2_EconomicSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 2")
    FReleaseTestResult TestEpic2_BuildingSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 2")
    FReleaseTestResult TestEpic2_ResourceManager();

    // ========== Epic 3: AI ?åÊà∞ÂΩπÁ≥ªÁµ±Ê∏¨Ë©?==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 3")
    FReleaseTestResult TestEpic3_CampaignSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 3")
    FReleaseTestResult TestEpic3_AIEnhancedSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 3")
    FReleaseTestResult TestEpic3_RelationshipSystem();

    // ========== Epic 4: UI ?åÈü≥?ªÁ≥ªÁµ±Ê∏¨Ë©?==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 4")
    FReleaseTestResult TestEpic4_UIEnhancedSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 4")
    FReleaseTestResult TestEpic4_AudioEnhancedSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 4")
    FReleaseTestResult TestEpic4_RepublicEraAudio();

    // ========== Epic 5: Á∂≤Áµ°?åÂ?‰∫∫È??≤Á≥ªÁµ±Ê∏¨Ë©?==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 5")
    FReleaseTestResult TestEpic5_NetworkSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 5")
    FReleaseTestResult TestEpic5_MultiplayerSession();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 5")
    FReleaseTestResult TestEpic5_DataSynchronization();

    // ========== Epic 6: ‰øùÂ??åË??•Á≥ªÁµ±Ê∏¨Ë©?==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 6")
    FReleaseTestResult TestEpic6_SaveLoadSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 6")
    FReleaseTestResult TestEpic6_DataFormats();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 6")
    FReleaseTestResult TestEpic6_CloudSave();

    // ========== Epic 7: ?¨Âú∞?ñÂ??ãÈ??ñÁ≥ªÁµ±Ê∏¨Ë©?==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 7")
    FReleaseTestResult TestEpic7_LocalizationSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 7")
    FReleaseTestResult TestEpic7_CulturalAdaptation();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 7")
    FReleaseTestResult TestEpic7_MultiLanguageSupport();

    // ========== Epic 8: ?ßËÉΩ?™Â??åË™øË©¶Á≥ªÁµ±Ê∏¨Ë©?==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 8")
    FReleaseTestResult TestEpic8_PerformanceSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 8")
    FReleaseTestResult TestEpic8_DebuggingTools();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 8")
    FReleaseTestResult TestEpic8_OptimizationModes();

    // ========== Epic 9: AI ?ßÂÆπ?üÊ?Á≥ªÁµ±Ê∏¨Ë©¶ ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 9")
    FReleaseTestResult TestEpic9_AIContentGeneration();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 9")
    FReleaseTestResult TestEpic9_BatchGeneration();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 9")
    FReleaseTestResult TestEpic9_SceneAutoGeneration();

    // ========== Epic 10: ?õÂ±§Á≠ñÁï•?¥Â?Á≥ªÁµ±Ê∏¨Ë©¶ ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 10")
    FReleaseTestResult TestEpic10_FourLayerIntegration();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 10")
    FReleaseTestResult TestEpic10_LayerSwitching();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 10")
    FReleaseTestResult TestEpic10_DataSynchronization();

    // ========== ?®Á≥ªÁµ±Êï¥?àÊ∏¨Ë©?==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Integration")
    FReleaseTestResult TestFullSystemIntegration();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Integration")
    FReleaseTestResult TestCrossSystemCommunication();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Integration")
    FReleaseTestResult TestEndToEndGameplay();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Integration")
    FReleaseTestResult TestMemoryAndResourceManagement();

    // ========== ?ÄÁµÇÈ?Ë≠âÊ∏¨Ë©?==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Validation")
    FReleaseTestResult TestReleaseReadiness();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Validation")
    FReleaseTestResult TestPlatformCompatibility();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Validation")
    FReleaseTestResult TestPerformanceBenchmarks();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Validation")
    FReleaseTestResult TestSecurityAndStability();

    // ========== ?πÈ?Ê∏¨Ë©¶?∑Ë? ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic1Tests();

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic2Tests();

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic3Tests();

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic4Tests();

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic5Tests();

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic6Tests();

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic7Tests();

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic8Tests();

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic9Tests();

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic10Tests();

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunAllEpicTests();

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunIntegrationTests();

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunValidationTests();

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunFullReleaseTestSuite();

    // ========== ÁµêÊ?ÁÆ°Á? ==========
    UFUNCTION(BlueprintPure, Category = "Release Test")
    TArray<FReleaseTestResult> GetAllResults() const { return TestResults; }

    UFUNCTION(BlueprintPure, Category = "Release Test")
    TArray<FReleaseTestResult> GetResultsByCategory(EReleaseTestCategory Category) const;

    UFUNCTION(BlueprintPure, Category = "Release Test")
    TArray<FReleaseTestResult> GetResultsByEpic(int32 EpicNumber) const;

    UFUNCTION(BlueprintPure, Category = "Release Test")
    int32 GetPassedCount() const;

    UFUNCTION(BlueprintPure, Category = "Release Test")
    int32 GetFailedCount() const;

    UFUNCTION(BlueprintPure, Category = "Release Test")
    int32 GetCriticalCount() const;

    UFUNCTION(BlueprintPure, Category = "Release Test")
    float GetSuccessRate() const;

    UFUNCTION(BlueprintPure, Category = "Release Test")
    bool IsReleaseReady() const;

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void GenerateReleaseReport(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    FString GetReleaseSummary() const;

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void ClearResults();

    // ‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable)
    FOnReleaseTestCompleted OnTestCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnReleaseTestSuiteCompleted OnSuiteCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnEpicTestCompleted OnEpicCompleted;

protected:
    UPROPERTY()
    TArray<FReleaseTestResult> TestResults;

    UPROPERTY()
    bool bIsRunningTests;

    UPROPERTY()
    int32 CurrentEpicNumber;

    // ËºîÂä©?ΩÊï∏
    void RecordResult(const FReleaseTestResult& Result);
    FReleaseTestResult CreateSuccessResult(const FString& TestName, EReleaseTestCategory Category, int32 EpicNum, const FString& Message);
    FReleaseTestResult CreateFailureResult(const FString& TestName, EReleaseTestCategory Category, int32 EpicNum, const FString& Message);
    FReleaseTestResult CreateWarningResult(const FString& TestName, EReleaseTestCategory Category, int32 EpicNum, const FString& Message);
    FReleaseTestResult CreateCriticalResult(const FString& TestName, EReleaseTestCategory Category, int32 EpicNum, const FString& Message);

private:
    void LogTestStart(const FString& TestName, int32 EpicNumber);
    void LogTestEnd(const FString& TestName, EReleaseTestResult Result);
};
