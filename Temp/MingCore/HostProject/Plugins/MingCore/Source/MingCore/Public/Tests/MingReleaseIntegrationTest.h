#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingReleaseIntegrationTest.generated.h"

UENUM(BlueprintType)
enum class EReleaseTestCategory : uint8
{
    Epic1_RTSCombat,        // Epic 1.1: RTS 戰鬥系統
    Epic2_Economic,         // Epic 2.1: 經濟系統
    Epic3_AICampaign,       // Epic 3.1: AI 和戰役系統
    Epic4_UIAudio,          // Epic 4.1: UI 和音頻系統
    Epic5_Network,          // Epic 5.1: 網絡和多人遊戲系統
    Epic6_SaveLoad,         // Epic 6.1: 保存和載入系統
    Epic7_Localization,     // Epic 7.1: 本地化和國際化系統
    Epic8_Performance,      // Epic 8.1: 性能優化和調試系統
    Epic9_AIContentGen,     // Epic 9: AI 內容生成系統
    Epic10_FourLayer,       // Epic 5: 四層策略整合系統
    SystemIntegration,      // 全系統整合
    FinalValidation         // 最終驗證
};

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
 * Release 版本最終集成測試套件
 * 測試所有 Epic 系統的完整性和整合性
 */
UCLASS()
class MINGPERSONAL_API UMingReleaseIntegrationTest : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UMingReleaseIntegrationTest();

    // ========== 初始化 ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test")
    void InitializeReleaseTestSuite();

    // ========== Epic 1: RTS 戰鬥系統測試 ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 1")
    FReleaseTestResult TestEpic1_RTSUnitSelection();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 1")
    FReleaseTestResult TestEpic1_RTSPathfinding();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 1")
    FReleaseTestResult TestEpic1_RTSCombatSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 1")
    FReleaseTestResult TestEpic1_RTSAIController();

    // ========== Epic 2: 經濟系統測試 ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 2")
    FReleaseTestResult TestEpic2_EconomicSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 2")
    FReleaseTestResult TestEpic2_BuildingSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 2")
    FReleaseTestResult TestEpic2_ResourceManager();

    // ========== Epic 3: AI 和戰役系統測試 ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 3")
    FReleaseTestResult TestEpic3_CampaignSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 3")
    FReleaseTestResult TestEpic3_AIEnhancedSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 3")
    FReleaseTestResult TestEpic3_RelationshipSystem();

    // ========== Epic 4: UI 和音頻系統測試 ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 4")
    FReleaseTestResult TestEpic4_UIEnhancedSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 4")
    FReleaseTestResult TestEpic4_AudioEnhancedSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 4")
    FReleaseTestResult TestEpic4_RepublicEraAudio();

    // ========== Epic 5: 網絡和多人遊戲系統測試 ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 5")
    FReleaseTestResult TestEpic5_NetworkSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 5")
    FReleaseTestResult TestEpic5_MultiplayerSession();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 5")
    FReleaseTestResult TestEpic5_DataSynchronization();

    // ========== Epic 6: 保存和載入系統測試 ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 6")
    FReleaseTestResult TestEpic6_SaveLoadSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 6")
    FReleaseTestResult TestEpic6_DataFormats();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 6")
    FReleaseTestResult TestEpic6_CloudSave();

    // ========== Epic 7: 本地化和國際化系統測試 ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 7")
    FReleaseTestResult TestEpic7_LocalizationSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 7")
    FReleaseTestResult TestEpic7_CulturalAdaptation();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 7")
    FReleaseTestResult TestEpic7_MultiLanguageSupport();

    // ========== Epic 8: 性能優化和調試系統測試 ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 8")
    FReleaseTestResult TestEpic8_PerformanceSystem();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 8")
    FReleaseTestResult TestEpic8_DebuggingTools();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 8")
    FReleaseTestResult TestEpic8_OptimizationModes();

    // ========== Epic 9: AI 內容生成系統測試 ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 9")
    FReleaseTestResult TestEpic9_AIContentGeneration();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 9")
    FReleaseTestResult TestEpic9_BatchGeneration();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 9")
    FReleaseTestResult TestEpic9_SceneAutoGeneration();

    // ========== Epic 10: 四層策略整合系統測試 ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 10")
    FReleaseTestResult TestEpic10_FourLayerIntegration();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 10")
    FReleaseTestResult TestEpic10_LayerSwitching();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Epic 10")
    FReleaseTestResult TestEpic10_DataSynchronization();

    // ========== 全系統整合測試 ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Integration")
    FReleaseTestResult TestFullSystemIntegration();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Integration")
    FReleaseTestResult TestCrossSystemCommunication();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Integration")
    FReleaseTestResult TestEndToEndGameplay();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Integration")
    FReleaseTestResult TestMemoryAndResourceManagement();

    // ========== 最終驗證測試 ==========
    UFUNCTION(BlueprintCallable, Category = "Release Test | Validation")
    FReleaseTestResult TestReleaseReadiness();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Validation")
    FReleaseTestResult TestPlatformCompatibility();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Validation")
    FReleaseTestResult TestPerformanceBenchmarks();

    UFUNCTION(BlueprintCallable, Category = "Release Test | Validation")
    FReleaseTestResult TestSecurityAndStability();

    // ========== 批量測試執行 ==========
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

    // ========== 結果管理 ==========
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

    // 事件
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

    // 輔助函數
    void RecordResult(const FReleaseTestResult& Result);
    FReleaseTestResult CreateSuccessResult(const FString& TestName, EReleaseTestCategory Category, int32 EpicNum, const FString& Message);
    FReleaseTestResult CreateFailureResult(const FString& TestName, EReleaseTestCategory Category, int32 EpicNum, const FString& Message);
    FReleaseTestResult CreateWarningResult(const FString& TestName, EReleaseTestCategory Category, int32 EpicNum, const FString& Message);
    FReleaseTestResult CreateCriticalResult(const FString& TestName, EReleaseTestCategory Category, int32 EpicNum, const FString& Message);

private:
    void LogTestStart(const FString& TestName, int32 EpicNumber);
    void LogTestEnd(const FString& TestName, EReleaseTestResult Result);
};
