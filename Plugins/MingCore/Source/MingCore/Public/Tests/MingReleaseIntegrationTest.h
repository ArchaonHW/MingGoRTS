#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingReleaseIntegrationTest.generated.h"

UENUM(BlueprintType)
enum class EReleaseTestCategory : uint8 {
    Epic1_RTSCombat,        // Epic 1.1: RTS Combat System
    Epic2_Economic,         // Epic 2.1: Economic System
    Epic3_AICampaign,       // Epic 3.1: AI Campaign System
    Epic4_UIAudio,          // Epic 4.1: UI Audio System
    Epic5_Network,          // Epic 5.1: Network System
    Epic6_SaveLoad,         // Epic 6.1: Save Load System
    Epic7_Localization,     // Epic 7.1: Localization System
    Epic8_Performance,      // Epic 8.1: Performance System
    Epic9_AIContentGen,     // Epic 9: AI Content Generation
    Epic10_FourLayer,       // Epic 5: Four Layer System
    SystemIntegration,      // System Integration
    FinalValidation,  // Final Validation
};

UENUM(BlueprintType)
enum class EReleaseTestResult : uint8 {
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
    FDateTime ExecutionTimestamp;

    FReleaseTestResult()
    {
        Result = EReleaseTestResult::NotRun;
        ExecutionTime = 0.0f;
        EpicNumber = 0;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReleaseTestCompleted, const FReleaseTestResult&, Result};
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReleaseTestSuiteCompleted};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEpicTestCompleted, int32, EpicNumber};

/**
 * Release Integration Test
 * Comprehensive Epic testing suite
UCLASS()
class MINGPERSONAL_API UMingReleaseIntegrationTest : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UMingReleaseIntegrationTest(};

    // ========== Initialization ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void InitializeReleaseTestSuite(};

    // ========== Epic 1: RTS Combat System ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 1")
    FReleaseTestResult TestEpic1_RTSUnitSelection(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 1")
    FReleaseTestResult TestEpic1_RTSPathfinding(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 1")
    FReleaseTestResult TestEpic1_RTSCombatSystem(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 1")
    FReleaseTestResult TestEpic1_RTSAIController(};

    // ========== Epic 2: Economic System ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 2")
    FReleaseTestResult TestEpic2_EconomicSystem(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 2")
    FReleaseTestResult TestEpic2_BuildingSystem(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 2")
    FReleaseTestResult TestEpic2_ResourceManager(};

    // ========== Epic 3: AI Campaign System ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 3")
    FReleaseTestResult TestEpic3_CampaignSystem(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 3")
    FReleaseTestResult TestEpic3_AIEnhancedSystem(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 3")
    FReleaseTestResult TestEpic3_RelationshipSystem(};

    // ========== Epic 4: UI Audio System ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 4")
    FReleaseTestResult TestEpic4_UIEnhancedSystem(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 4")
    FReleaseTestResult TestEpic4_AudioEnhancedSystem(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 4")
    FReleaseTestResult TestEpic4_RepublicEraAudio(};

    // ========== Epic 5: Network System ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 5")
    FReleaseTestResult TestEpic5_NetworkSystem(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 5")
    FReleaseTestResult TestEpic5_MultiplayerSession(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 5")
    FReleaseTestResult TestEpic5_DataSynchronization(};

    // ========== Epic 6: Save Load System ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 6")
    FReleaseTestResult TestEpic6_SaveLoadSystem(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 6")
    FReleaseTestResult TestEpic6_DataFormats(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 6")
    FReleaseTestResult TestEpic6_CloudSave(};

    // ========== Epic 7: Localization System ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 7")
    FReleaseTestResult TestEpic7_LocalizationSystem(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 7")
    FReleaseTestResult TestEpic7_CulturalAdaptation(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 7")
    FReleaseTestResult TestEpic7_MultiLanguageSupport(};

    // ========== Epic 8: Performance System ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 8")
    FReleaseTestResult TestEpic8_PerformanceSystem(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 8")
    FReleaseTestResult TestEpic8_DebuggingTools(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 8")
    FReleaseTestResult TestEpic8_OptimizationModes(};

    // ========== Epic 9: AI Content Generation ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 9")
    FReleaseTestResult TestEpic9_AIContentGeneration(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 9")
    FReleaseTestResult TestEpic9_BatchGeneration(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 9")
    FReleaseTestResult TestEpic9_SceneAutoGeneration(};

    // ========== Epic 10: Four Layer System ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 10")
    FReleaseTestResult TestEpic10_FourLayerIntegration(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 10")
    FReleaseTestResult TestEpic10_LayerSwitching(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Epic 10")
    FReleaseTestResult TestEpic10_DataSynchronization(};

    // ========== System Integration ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test - Integration")
    FReleaseTestResult TestFullSystemIntegration(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Integration")
    FReleaseTestResult TestCrossSystemCommunication(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Integration")
    FReleaseTestResult TestEndToEndGameplay(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Integration")
    FReleaseTestResult TestMemoryAndResourceManagement(};

    // ========== Final Validation ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test - Validation")
    FReleaseTestResult TestReleaseReadiness(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Validation")
    FReleaseTestResult TestPlatformCompatibility(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Validation")
    FReleaseTestResult TestPerformanceBenchmarks(};

    UFUNCTION(BlueprintCallable, Category = "Release Test - Validation")
    FReleaseTestResult TestSecurityAndStability(};

    // ========== Test Execution ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic1Tests(};

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic2Tests(};

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic3Tests(};

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic4Tests(};

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic5Tests(};

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic6Tests(};

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic7Tests(};

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic8Tests(};

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic9Tests(};

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunEpic10Tests(};

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunAllEpicTests(};

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunIntegrationTests(};

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunValidationTests(};

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void RunFullReleaseTestSuite(};

    // ========== Results ==========
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
    void GenerateReleaseReport(const FString& FilePath};

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    FString GetReleaseSummary() const;

    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void ClearResults(};

    // Events
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

    // Helper Functions
    void RecordResult(const FReleaseTestResult& Result};
    FReleaseTestResult CreateSuccessResult(const FString& TestName, EReleaseTestCategory Category, int32 EpicNumber, const FString& Message};
    FReleaseTestResult CreateFailureResult(const FString& TestName, EReleaseTestCategory Category, int32 EpicNumber, const FString& Message};
    FReleaseTestResult CreateWarningResult(const FString& TestName, EReleaseTestCategory Category, int32 EpicNumber, const FString& Message};
    FReleaseTestResult CreateCriticalResult(const FString& TestName, EReleaseTestCategory Category, int32 EpicNumber, const FString& Message};

private:
    void LogTestStart(const FString& TestName, int32 EpicNumber};
    void LogTestEnd(const FString& TestName, EReleaseTestResult Result};
};
