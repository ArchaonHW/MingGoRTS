#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingUserExperienceTest.generated.h"

/**
 * 用戶體驗測試套件
 * 測試真實用戶場景和可用性
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingUserExperienceTest : public UObject
{
    GENERATED_BODY()

public:
    UMingUserExperienceTest();

    // ========== 新手用戶體驗測試 ==========
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | New User")
    void StartNewUserExperienceTest();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | New User")
    void SimulateNewUserJourney();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | New User")
    void TestTutorialEffectiveness();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | New User")
    void TestOnboardingFlow();
    
    UFUNCTION(BlueprintPure, Category = "UX Test | New User")
    float GetNewUserCompletionRate() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | New User")
    float GetAverageTutorialTime() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | New User")
    int32 GetHelpRequestCount() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | New User")
    float GetNewUserSatisfaction() const;

    // ========== 資深用戶體驗測試 ==========
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | Expert User")
    void StartExpertUserTest();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | Expert User")
    void SimulateExpertGameplay();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | Expert User")
    void TestAdvancedControls();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | Expert User")
    void TestStrategicDepth();
    
    UFUNCTION(BlueprintPure, Category = "UX Test | Expert User")
    float GetExpertEfficiency() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | Expert User")
    int32 GetAdvancedActionsCount() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | Expert User")
    float GetStrategicDepth() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | Expert User")
    float GetExpertSatisfaction() const;

    // ========== 界面可用性測試 ==========
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | UI")
    void StartUIUsabilityTest();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | UI")
    void TestInterfaceAccessibility();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | UI")
    void TestNavigationEfficiency();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | UI")
    void TestVisualClarity();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | UI")
    void TestResponsiveDesign();
    
    UFUNCTION(BlueprintPure, Category = "UX Test | UI")
    float GetClickAccuracy() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | UI")
    float GetNavigationEfficiency() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | UI")
    int32 GetErrorClickCount() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | UI")
    float GetUIResponsiveness() const;

    // ========== 交互體驗測試 ==========
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | Interaction")
    void StartInteractionTest();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | Interaction")
    void TestControlResponsiveness();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | Interaction")
    void TestFeedbackSystems();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | Interaction")
    void TestInputLatency();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | Interaction")
    void TestGestureRecognition();
    
    UFUNCTION(BlueprintPure, Category = "UX Test | Interaction")
    float GetAverageInputLatency() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | Interaction")
    float GetControlPrecision() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | Interaction")
    int32 GetInputErrors() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | Interaction")
    float GetInteractionSatisfaction() const;

    // ========== 學習曲線測試 ==========
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | Learning")
    void StartLearningCurveTest();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | Learning")
    void TestSkillProgression();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | Learning")
    void TestKnowledgeRetention();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | Learning")
    void TestDifficultyProgression();
    
    UFUNCTION(BlueprintPure, Category = "UX Test | Learning")
    float GetLearningRate() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | Learning")
    float GetSkillMasteryTime() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | Learning")
    float GetKnowledgeRetentionRate() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | Learning")
    float GetDifficultyBalance() const;

    // ========== 情感體驗測試 ==========
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | Emotional")
    void StartEmotionalExperienceTest();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | Emotional")
    void TestEngagementLevels();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | Emotional")
    void TestFrustrationPoints();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test | Emotional")
    void TestSatisfactionMoments();
    
    UFUNCTION(BlueprintPure, Category = "UX Test | Emotional")
    float GetEngagementScore() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | Emotional")
    int32 GetFrustrationCount() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | Emotional")
    float GetSatisfactionScore() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test | Emotional")
    float GetEmotionalBalance() const;

    // ========== 綜合用戶體驗測試 ==========
    
    UFUNCTION(BlueprintCallable, Category = "UX Test")
    void RunAllUserExperienceTests();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test")
    void RunQuickUXTest();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test")
    void RunComprehensiveUXTest();
    
    UFUNCTION(BlueprintPure, Category = "UX Test")
    bool AreAllUXTestsPassed() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test")
    int32 GetPassedTestCount() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test")
    int32 GetFailedTestCount() const;
    
    UFUNCTION(BlueprintCallable, Category = "UX Test")
    FString GenerateUXTestReport();
    
    UFUNCTION(BlueprintCallable, Category = "UX Test")
    void ExportUXTestData(const FString& FilePath);
    
    UFUNCTION(BlueprintPure, Category = "UX Test")
    float GetOverallUXScore() const;
    
    UFUNCTION(BlueprintPure, Category = "UX Test")
    FString GetUXRecommendations() const;

protected:
    virtual void Tick(float DeltaTime) override;

private:
    // 新手用戶數據
    UPROPERTY()
    float NewUserCompletionRate;
    
    UPROPERTY()
    float AverageTutorialTime;
    
    UPROPERTY()
    int32 HelpRequestCount;
    
    UPROPERTY()
    float NewUserSatisfaction;
    
    // 資深用戶數據
    UPROPERTY()
    float ExpertEfficiency;
    
    UPROPERTY()
    int32 AdvancedActionsCount;
    
    UPROPERTY()
    float StrategicDepth;
    
    UPROPERTY()
    float ExpertSatisfaction;
    
    // UI可用性數據
    UPROPERTY()
    float ClickAccuracy;
    
    UPROPERTY()
    float NavigationEfficiency;
    
    UPROPERTY()
    int32 ErrorClickCount;
    
    UPROPERTY()
    float UIResponsiveness;
    
    // 交互體驗數據
    UPROPERTY()
    float AverageInputLatency;
    
    UPROPERTY()
    float ControlPrecision;
    
    UPROPERTY()
    int32 InputErrors;
    
    UPROPERTY()
    float InteractionSatisfaction;
    
    // 學習曲線數據
    UPROPERTY()
    float LearningRate;
    
    UPROPERTY()
    float SkillMasteryTime;
    
    UPROPERTY()
    float KnowledgeRetentionRate;
    
    UPROPERTY()
    float DifficultyBalance;
    
    // 情感體驗數據
    UPROPERTY()
    float EngagementScore;
    
    UPROPERTY()
    int32 FrustrationCount;
    
    UPROPERTY()
    float SatisfactionScore;
    
    UPROPERTY()
    float EmotionalBalance;
    
    // 測試狀態
    UPROPERTY()
    bool bNewUserTestRunning;
    
    UPROPERTY()
    bool bExpertTestRunning;
    
    UPROPERTY()
    bool bUITestRunning;
    
    UPROPERTY()
    bool bInteractionTestRunning;
    
    UPROPERTY()
    bool bLearningTestRunning;
    
    UPROPERTY()
    bool bEmotionalTestRunning;
    
    // 測試結果
    UPROPERTY()
    int32 PassedTests;
    
    UPROPERTY()
    int32 FailedTests;
    
    UPROPERTY()
    TArray<FString> TestResults;

    // 內部函數
    void UpdateNewUserMetrics(float DeltaTime);
    void UpdateExpertUserMetrics(float DeltaTime);
    void UpdateUIMetrics(float DeltaTime);
    void UpdateInteractionMetrics(float DeltaTime);
    void UpdateLearningMetrics(float DeltaTime);
    void UpdateEmotionalMetrics(float DeltaTime);
    
    void GuideThroughTutorial();
    void MonitorLearningProgress();
    void TrackUserMistakes();
    void AnalyzeUserBehavior();
    
    void TestAdvancedStrategies();
    void ValidateExpertControls();
    void MonitorPerformanceMetrics();
    void AnalyzeStrategicDecisions();
    
    void TestButtonAccessibility();
    void ValidateMenuNavigation();
    void CheckColorContrast();
    void VerifyTextReadability();
    
    void MeasureInputLatency();
    void TestControlAccuracy();
    void ValidateFeedbackSystems();
    void CheckGestureRecognition();
    
    void AddTestResult(const FString& TestName, bool bPassed, const FString& Details = TEXT(""));
    void ResetTestResults();
    void CalculateUXStatistics();
    void GenerateRecommendations();
    
    // 用戶體驗分析
    void AnalyzeUserJourney();
    void IdentifyPainPoints();
    void ValidateFlowEfficiency();
    void AssessAccessibility();
};
