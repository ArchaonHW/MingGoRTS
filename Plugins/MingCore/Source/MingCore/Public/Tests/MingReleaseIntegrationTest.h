#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingReleaseIntegrationTest.generated.h"

UENUM(BlueprintType)
enum class EReleaseTestCategory: uint8 {
    Epic1_RTSCombat,        // Epic 1.1: RTS X???t??
    Epic2_Economic,         // Epic 2.1: ?gX?t??
    Epic3_AICampaign,       // Epic 3.1: AI X???tX
    Epic4_UIAudio,          // Epic 4.1: UI X??X?tX
    Epic5_Network,          // Epic 5.1: ???X?HX?tX
    Epic6_SaveLoad,         // Epic 6.1: ?OX?tX
    Epic7_Localization,     // Epic 7.1: X?aX?tX
    Epic8_Performance,      // Epic 8.1: X??X???tX
    Epic9_AIContentGen,     // Epic 9: AI X?eX?t??
    Epic10_FourLayer,       // Epic 5: X?h摧毀X?t??
    SystemIntegration,      // X?t?�i?X
    FinalValidation,  // X??X};

UENUM(BlueprintType)
enum class EReleaseTestResult: uint8 {
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





/**
 * Release X??X??X摧毀X * 摧毀XEpic ?t??X??X */
UCLASS()
class MINGPERSONAL_API UMingReleaseIntegrationTest : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UMingReleaseIntegrationTest();

    // ========== X==========
    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void InitializeReleaseTestSuite();

    // ========== Epic 1: RTS X???t?�_??? ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 1")
    FReleaseTestResult TestEpic1_RTSUnitSelection();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 1")
    FReleaseTestResult TestEpic1_RTSPathfinding();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 1")
    FReleaseTestResult TestEpic1_RTSCombatSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 1")
    FReleaseTestResult TestEpic1_RTSAIController();

    // ========== Epic 2: ?gX?t?�_??? ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 2")
    FReleaseTestResult TestEpic2_EconomicSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 2")
    FReleaseTestResult TestEpic2_BuildingSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 2")
    FReleaseTestResult TestEpic2_ResourceManager();

    // ========== Epic 3: AI X???t?�_?X==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 3")
    FReleaseTestResult TestEpic3_CampaignSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 3")
    FReleaseTestResult TestEpic3_AIEnhancedSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 3")
    FReleaseTestResult TestEpic3_RelationshipSystem();

    // ========== Epic 4: UI X??X?t?�_?X==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 4")
    FReleaseTestResult TestEpic4_UIEnhancedSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 4")
    FReleaseTestResult TestEpic4_AudioEnhancedSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 4")
    FReleaseTestResult TestEpic4_RepublicEraAudio();

    // ========== Epic 5: ???X?HX?t?�_?X==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 5")
    FReleaseTestResult TestEpic5_NetworkSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 5")
    FReleaseTestResult TestEpic5_MultiplayerSession();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 5")
    FReleaseTestResult TestEpic5_DataSynchronization();

    // ========== Epic 6: ?OX?t?�_?X==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 6")
    FReleaseTestResult TestEpic6_SaveLoadSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 6")
    FReleaseTestResult TestEpic6_DataFormats();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 6")
    FReleaseTestResult TestEpic6_CloudSave();

    // ========== Epic 7: X?aX?t?�_?X==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 7")
    FReleaseTestResult TestEpic7_LocalizationSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 7")
    FReleaseTestResult TestEpic7_CulturalAdaptation();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 7")
    FReleaseTestResult TestEpic7_MultiLanguageSupport();

    // ========== Epic 8: X??X???t?�_?X==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 8")
    FReleaseTestResult TestEpic8_PerformanceSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 8")
    FReleaseTestResult TestEpic8_DebuggingTools();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 8")
    FReleaseTestResult TestEpic8_OptimizationModes();

    // ========== Epic 9: AI X?eX?t?�_??? ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 9")
    FReleaseTestResult TestEpic9_AIContentGeneration();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 9")
    FReleaseTestResult TestEpic9_BatchGeneration();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 9")
    FReleaseTestResult TestEpic9_SceneAutoGeneration();

    // ========== Epic 10: X?h摧毀X?t?�_??? ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 10")
    FReleaseTestResult TestEpic10_FourLayerIntegration();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 10")
    FReleaseTestResult TestEpic10_LayerSwitching();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 10")
    FReleaseTestResult TestEpic10_DataSynchronization();

    // ========== X?t?�i?X??X==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Integration")
    FReleaseTestResult TestFullSystemIntegration();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Integration")
    FReleaseTestResult TestCrossSystemCommunication();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Integration")
    FReleaseTestResult TestEndToEndGameplay();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Integration")
    FReleaseTestResult TestMemoryAndResourceManagement();

    // ========== X??X???X==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Validation")
    FReleaseTestResult TestReleaseReadiness();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Validation")
    FReleaseTestResult TestPlatformCompatibility();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Validation")
    FReleaseTestResult TestPerformanceBenchmarks();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Validation")
    FReleaseTestResult TestSecurityAndStability();

    // ========== X摧毀X ==========
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

    // ========== ??X??X ==========
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

    // ???
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

    // ???UX??
    void RecordResult(const FReleaseTestResult& Result);
    FReleaseTestResult CreateSuccessResult(const FString& TestName, EReleaseTestCategory Category, int32 EpicNum, const FString& Message);
    FReleaseTestResult CreateFailureResult(const FString& TestName, EReleaseTestCategory Category, int32 EpicNum, const FString& Message);
    FReleaseTestResult CreateWarningResult(const FString& TestName, EReleaseTestCategory Category, int32 EpicNum, const FString& Message);
    FReleaseTestResult CreateCriticalResult(const FString& TestName, EReleaseTestCategory Category, int32 EpicNum, const FString& Message);

private:
    void LogTestStart(const FString& TestName, int32 EpicNumber);
    void LogTestEnd(const FString& TestName, EReleaseTestResult Result);
);

