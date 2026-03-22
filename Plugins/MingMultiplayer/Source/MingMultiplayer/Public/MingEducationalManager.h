#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingEducationalTypes.h"
#include "MingEducationalManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEducationalModeToggled, bool, bEnabled, EMingEducationalMode, Mode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLessonStarted, const FMingEducationalLesson&, Lesson);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLessonCompleted, const FString&, LessonID, float, Score);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHistoricalEventTriggered, const FMingHistoricalEvent&, Event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuizCompleted, const FString&, QuizID, int32, CorrectAnswers);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAchievementUnlocked, const FMingEducationalAchievement&, Achievement);

UCLASS(ClassGroup = (Educational, Learning))
class MINGMULTIPLAYER_API UMingEducationalManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UMingEducationalManager();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override { return true; }
    virtual TStatId GetStatId() const override;

    // Educational Mode Management
    UFUNCTION(BlueprintCallable, Category = "Educational")
    bool EnableEducationalMode(EMingEducationalMode Mode);

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void DisableEducationalMode();

    UFUNCTION(BlueprintPure, Category = "Educational")
    bool IsEducationalModeEnabled() const;

    UFUNCTION(BlueprintPure, Category = "Educational")
    EMingEducationalMode GetCurrentEducationalMode() const;

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void SetEducationalDifficulty(EMingEducationalDifficulty Difficulty);

    UFUNCTION(BlueprintPure, Category = "Educational")
    EMingEducationalDifficulty GetEducationalDifficulty() const;

    // Historical Content Management
    UFUNCTION(BlueprintCallable, Category = "Educational")
    bool LoadHistoricalContent(const FString& ContentID);

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void UnloadHistoricalContent(const FString& ContentID);

    UFUNCTION(BlueprintPure, Category = "Educational")
    TArray<FMingHistoricalContent> GetLoadedHistoricalContent() const;

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void ShowHistoricalInfo(const FString& TopicID);

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void HideHistoricalInfo();

    UFUNCTION(BlueprintPure, Category = "Educational")
    bool IsHistoricalInfoVisible() const;

    // Lesson Management
    UFUNCTION(BlueprintCallable, Category = "Educational")
    bool StartLesson(const FString& LessonID);

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void PauseLesson();

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void ResumeLesson();

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void StopLesson();

    UFUNCTION(BlueprintPure, Category = "Educational")
    bool IsLessonActive() const;

    UFUNCTION(BlueprintPure, Category = "Educational")
    FMingEducationalLesson GetCurrentLesson() const;

    UFUNCTION(BlueprintCallable, Category = "Educational")
    bool SkipToNextLesson();

    UFUNCTION(BlueprintCallable, Category = "Educational")
    bool GoToPreviousLesson();

    UFUNCTION(BlueprintPure, Category = "Educational")
    TArray<FMingEducationalLesson> GetAvailableLessons() const;

    UFUNCTION(BlueprintPure, Category = "Educational")
    TArray<FMingEducationalLesson> GetCompletedLessons() const;

    // Quiz System
    UFUNCTION(BlueprintCallable, Category = "Educational")
    bool StartQuiz(const FString& QuizID);

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void SubmitQuizAnswer(int32 QuestionIndex, const FString& Answer);

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void CompleteQuiz();

    UFUNCTION(BlueprintPure, Category = "Educational")
    bool IsQuizActive() const;

    UFUNCTION(BlueprintPure, Category = "Educational")
    FMingEducationalQuiz GetCurrentQuiz() const;

    UFUNCTION(BlueprintPure, Category = "Educational")
    int32 GetCurrentQuizScore() const;

    UFUNCTION(BlueprintPure, Category = "Educational")
    float GetCurrentQuizProgress() const;

    // Historical Events
    UFUNCTION(BlueprintCallable, Category = "Educational")
    void RegisterHistoricalEvent(const FMingHistoricalEvent& Event);

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void TriggerHistoricalEvent(const FString& EventID);

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void ScheduleHistoricalEvent(const FMingHistoricalEvent& Event, float Delay);

    UFUNCTION(BlueprintPure, Category = "Educational")
    TArray<FMingHistoricalEvent> GetAvailableHistoricalEvents() const;

    UFUNCTION(BlueprintPure, Category = "Educational")
    TArray<FMingHistoricalEvent> GetTriggeredHistoricalEvents() const;

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void ShowHistoricalTimeline();

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void HideHistoricalTimeline();

    // Interactive Learning
    UFUNCTION(BlueprintCallable, Category = "Educational")
    bool StartInteractiveLearning(const FString& LearningModuleID);

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void ProcessLearningInteraction(const FMingLearningInteraction& Interaction);

    UFUNCTION(BlueprintPure, Category = "Educational")
    bool IsInteractiveLearningActive() const;

    UFUNCTION(BlueprintPure, Category = "Educational")
    FMingInteractiveLearningModule GetCurrentLearningModule() const;

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void ShowLearningHints();

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void HideLearningHints();

    // Progress Tracking
    UFUNCTION(BlueprintPure, Category = "Educational")
    FMingEducationalProgress GetEducationalProgress() const;

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void ResetEducationalProgress();

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void SaveEducationalProgress();

    UFUNCTION(BlueprintCallable, Category = "Educational")
    bool LoadEducationalProgress();

    UFUNCTION(BlueprintPure, Category = "Educational")
    float GetOverallLearningScore() const;

    UFUNCTION(BlueprintPure, Category = "Educational")
    int32 GetTotalLessonsCompleted() const;

    UFUNCTION(BlueprintPure, Category = "Educational")
    int32 GetTotalQuizzesCompleted() const;

    // Achievement System
    UFUNCTION(BlueprintCallable, Category = "Educational")
    void UnlockAchievement(const FString& AchievementID);

    UFUNCTION(BlueprintPure, Category = "Educational")
    TArray<FMingEducationalAchievement> GetUnlockedAchievements() const;

    UFUNCTION(BlueprintPure, Category = "Educational")
    TArray<FMingEducationalAchievement> GetAvailableAchievements() const;

    UFUNCTION(BlueprintPure, Category = "Educational")
    bool IsAchievementUnlocked(const FString& AchievementID) const;

    // Content Adaptation
    UFUNCTION(BlueprintCallable, Category = "Educational")
    void SetAdaptiveLearningEnabled(bool bEnabled);

    UFUNCTION(BlueprintPure, Category = "Educational")
    bool IsAdaptiveLearningEnabled() const;

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void SetPlayerLearningProfile(const FMingPlayerLearningProfile& Profile);

    UFUNCTION(BlueprintPure, Category = "Educational")
    FMingPlayerLearningProfile GetPlayerLearningProfile() const;

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void UpdateAdaptiveDifficulty();

    UFUNCTION(BlueprintCallable, Category = "Educational")
    FMingEducationalContent GetAdaptedContent(const FString& ContentID);

    // Multiplayer Education
    UFUNCTION(BlueprintCallable, Category = "Educational")
    bool EnableCooperativeLearning();

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void StartCooperativeLesson(const FString& LessonID, const TArray<int32>& PlayerIDs);

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void ShareLearningProgress();

    UFUNCTION(BlueprintCallable, Category = "Educational")
    void ReceiveLearningProgress(const FMingEducationalProgress& Progress);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnEducationalModeToggled OnEducationalModeToggled;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLessonStarted OnLessonStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLessonCompleted OnLessonCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnHistoricalEventTriggered OnHistoricalEventTriggered;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnQuizCompleted OnQuizCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnAchievementUnlocked OnAchievementUnlocked;

protected:
    UPROPERTY()
    bool bEducationalModeEnabled;

    UPROPERTY()
    EMingEducationalMode CurrentEducationalMode;

    UPROPERTY()
    EMingEducationalDifficulty EducationalDifficulty;

    UPROPERTY()
    TArray<FMingHistoricalContent> LoadedHistoricalContent;

    UPROPERTY()
    TArray<FMingEducationalLesson> AvailableLessons;

    UPROPERTY()
    TArray<FMingEducationalLesson> CompletedLessons;

    UPROPERTY()
    FMingEducationalLesson CurrentLesson;

    UPROPERTY()
    FMingEducationalQuiz CurrentQuiz;

    UPROPERTY()
    TArray<FMingHistoricalEvent> AvailableHistoricalEvents;

    UPROPERTY()
    TArray<FMingHistoricalEvent> TriggeredHistoricalEvents;

    UPROPERTY()
    TArray<FMingHistoricalEvent> ScheduledHistoricalEvents;

    UPROPERTY()
    FMingInteractiveLearningModule CurrentLearningModule;

    UPROPERTY()
    FMingEducationalProgress EducationalProgress;

    UPROPERTY()
    TArray<FMingEducationalAchievement> UnlockedAchievements;

    UPROPERTY()
    TArray<FMingEducationalAchievement> AvailableAchievements;

    UPROPERTY()
    FMingPlayerLearningProfile PlayerLearningProfile;

    UPROPERTY()
    bool bAdaptiveLearningEnabled;

    UPROPERTY()
    bool bCooperativeLearningEnabled;

    UPROPERTY()
    bool bHistoricalInfoVisible;

    UPROPERTY()
    bool bLessonActive;

    UPROPERTY()
    bool bQuizActive;

    UPROPERTY()
    bool bInteractiveLearningActive;

    UPROPERTY()
    bool bHistoricalTimelineVisible;

    UPROPERTY()
    float LastUpdateTime;

    UPROPERTY()
    float UpdateInterval;

    // Internal Functions
    void UpdateEducationalSystem(float DeltaTime);
    void UpdateCurrentLesson(float DeltaTime);
    void UpdateHistoricalEvents(float DeltaTime);
    void UpdateInteractiveLearning(float DeltaTime);
    void UpdateAdaptiveLearning(float DeltaTime);
    void ProcessScheduledEvents(float DeltaTime);
    void ValidateEducationalState();

    // Helper Functions
    void InitializeDefaultContent();
    void LoadEducationalData();
    void SaveEducationalData();
    void CalculateLearningProgress();
    void TriggerAchievementCheck();
    void AdaptContentDifficulty();
    void GeneratePersonalizedContent();
    void UpdateLearningAnalytics();
    void SyncCooperativeProgress();

private:
    bool ValidateLesson(const FMingEducationalLesson& Lesson) const;
    bool ValidateQuiz(const FMingEducationalQuiz& Quiz) const;
    bool ValidateHistoricalEvent(const FMingHistoricalEvent& Event) const;
    FString GenerateLessonID() const;
    FString GenerateQuizID() const;
    FString GenerateAchievementID() const;
    void CleanupEducationalResources();
    void ResetEducationalSystem();
};
