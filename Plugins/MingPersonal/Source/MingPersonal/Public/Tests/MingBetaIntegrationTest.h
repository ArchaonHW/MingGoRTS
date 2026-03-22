#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MingBetaIntegrationTest.generated.h"

UENUM(BlueprintType)
enum class EBetaTestCategory : uint8
{
    AudioSystem,        // Story 3.5
    UISystem,           // Story 3.6
    SaveLoadSystem,     // Story 3.7
    Integration,        // 系統整合
    Performance,        // 性能測試
    Compatibility       // 兼容性測試
};

UENUM(BlueprintType)
enum class EBetaTestResult : uint8
{
    NotRun,
    Passed,
    Failed,
    Warning,
    Skipped
};

USTRUCT(BlueprintType)
struct FBetaTestResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TestName;

    UPROPERTY(BlueprintReadOnly)
    EBetaTestCategory Category;

    UPROPERTY(BlueprintReadOnly)
    EBetaTestResult Result;

    UPROPERTY(BlueprintReadOnly)
    FString Message;

    UPROPERTY(BlueprintReadOnly)
    float ExecutionTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime ExecutionTimeStamp;

    FBetaTestResult()
    {
        Result = EBetaTestResult::NotRun;
        ExecutionTime = 0.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBetaTestCompleted, const FString&, TestName, const FBetaTestResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBetaTestSuiteCompleted, const TArray<FBetaTestResult>&, AllResults);

/**
 * Beta版本整合測試套件
 * 測試 Story 3.5-3.7 的所有功能
 */
UCLASS()
class MINGPERSONAL_API UMingBetaIntegrationTest : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UMingBetaIntegrationTest();

    // 初始化測試套件
    UFUNCTION(BlueprintCallable, Category = "Beta Test")
    void InitializeTestSuite();

    // ========== Story 3.5 音頻系統測試 ==========

    UFUNCTION(BlueprintCallable, Category = "Beta Test | Audio")
    FBetaTestResult TestAudioSystemInitialization();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | Audio")
    FBetaTestResult TestMusicPlayback();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | Audio")
    FBetaTestResult TestSFXPlayback();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | Audio")
    FBetaTestResult TestRegionalAudio();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | Audio")
    FBetaTestResult TestAudioRelationshipIntegration();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | Audio")
    FBetaTestResult TestMetaSoundsSystem();

    // ========== Story 3.6 UI系統測試 ==========

    UFUNCTION(BlueprintCallable, Category = "Beta Test | UI")
    FBetaTestResult TestUISystemInitialization();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | UI")
    FBetaTestResult TestRelationshipPanel();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | UI")
    FBetaTestResult TestReputationPanel();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | UI")
    FBetaTestResult TestDialogueSystem();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | UI")
    FBetaTestResult TestQuestPanel();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | UI")
    FBetaTestResult TestMainDashboard();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | UI")
    FBetaTestResult TestAccessibilityFeatures();

    // ========== Story 3.7 保存載入系統測試 ==========

    UFUNCTION(BlueprintCallable, Category = "Beta Test | SaveLoad")
    FBetaTestResult TestSaveSystemInitialization();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | SaveLoad")
    FBetaTestResult TestBasicSave();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | SaveLoad")
    FBetaTestResult TestBasicLoad();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | SaveLoad")
    FBetaTestResult TestAutoSave();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | SaveLoad")
    FBetaTestResult TestMultipleFormats();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | SaveLoad")
    FBetaTestResult TestDataIntegrity();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | SaveLoad")
    FBetaTestResult TestBackupAndRestore();

    // ========== 整合測試 ==========

    UFUNCTION(BlueprintCallable, Category = "Beta Test | Integration")
    FBetaTestResult TestAudioUIIntegration();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | Integration")
    FBetaTestResult TestUISaveLoadIntegration();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | Integration")
    FBetaTestResult TestRelationshipAudioUIFlow();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | Integration")
    FBetaTestResult TestFullGameCycle();

    // ========== 性能測試 ==========

    UFUNCTION(BlueprintCallable, Category = "Beta Test | Performance")
    FBetaTestResult TestAudioPerformance();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | Performance")
    FBetaTestResult TestUIPerformance();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | Performance")
    FBetaTestResult TestSaveLoadPerformance();

    UFUNCTION(BlueprintCallable, Category = "Beta Test | Performance")
    FBetaTestResult TestMemoryUsage();

    // ========== 批量測試 ==========

    UFUNCTION(BlueprintCallable, Category = "Beta Test")
    void RunAllAudioTests();

    UFUNCTION(BlueprintCallable, Category = "Beta Test")
    void RunAllUITests();

    UFUNCTION(BlueprintCallable, Category = "Beta Test")
    void RunAllSaveLoadTests();

    UFUNCTION(BlueprintCallable, Category = "Beta Test")
    void RunAllIntegrationTests();

    UFUNCTION(BlueprintCallable, Category = "Beta Test")
    void RunAllPerformanceTests();

    UFUNCTION(BlueprintCallable, Category = "Beta Test")
    void RunFullBetaTestSuite();

    // ========== 結果管理 ==========

    UFUNCTION(BlueprintPure, Category = "Beta Test")
    TArray<FBetaTestResult> GetAllResults() const { return TestResults; }

    UFUNCTION(BlueprintPure, Category = "Beta Test")
    TArray<FBetaTestResult> GetResultsByCategory(EBetaTestCategory Category) const;

    UFUNCTION(BlueprintPure, Category = "Beta Test")
    int32 GetPassedCount() const;

    UFUNCTION(BlueprintPure, Category = "Beta Test")
    int32 GetFailedCount() const;

    UFUNCTION(BlueprintPure, Category = "Beta Test")
    float GetSuccessRate() const;

    UFUNCTION(BlueprintCallable, Category = "Beta Test")
    void ExportTestReport(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Beta Test")
    void ClearResults();

    // 事件
    UPROPERTY(BlueprintAssignable)
    FOnBetaTestCompleted OnTestCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnBetaTestSuiteCompleted OnSuiteCompleted;

protected:
    UPROPERTY()
    TArray<FBetaTestResult> TestResults;

    UPROPERTY()
    bool bIsRunningTests;

    // 測試輔助函數
    void RecordResult(const FBetaTestResult& Result);
    FBetaTestResult CreateSuccessResult(const FString& TestName, EBetaTestCategory Category, const FString& Message);
    FBetaTestResult CreateFailureResult(const FString& TestName, EBetaTestCategory Category, const FString& Message);
    FBetaTestResult CreateWarningResult(const FString& TestName, EBetaTestCategory Category, const FString& Message);
    
    // 系統引用驗證
    bool ValidateAudioSystem();
    bool ValidateUISystem();
    bool ValidateSaveSystem();
    
    // 測試數據準備
    void PrepareTestData();
    void CleanupTestData();

private:
    void InitializeSystems();
};
