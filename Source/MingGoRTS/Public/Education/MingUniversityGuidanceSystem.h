// Copyright (c) 2026 MingGoRTS. All rights reserved.
// University Guidance System - C3-1
// Provides comprehensive guidance and tutorial systems for university/educational use

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingUniversityGuidanceSystem.generated.h"

UENUM(BlueprintType)
enum class EGuidanceCategory : uint8
{
    Tutorial = 0, UMETA(DisplayName = "Tutorial"),
    HistoricalContext, UMETA(DisplayName = "Historical Context"),
    StrategyGuide, UMETA(DisplayName = "Strategy Guide"),
    CulturalEducation, UMETA(DisplayName = "Cultural Education"),
    GameMechanics, UMETA(DisplayName = "Game Mechanics"),
    CharacterDevelopment, UMETA(DisplayName = "Character Development"),
    MissionObjective, UMETA(DisplayName = "Mission Objective"),
    Assessment, UMETA(DisplayName = "Assessment"),
    StudyPlan, UMETA(DisplayName = "Study Plan"),
    InteractiveLearning UMETA(DisplayName = "Interactive Learning")
};

UENUM(BlueprintType)
enum class EGuidanceLevel : uint8
{
    Beginner = 0, UMETA(DisplayName = "Beginner"),
    Elementary, UMETA(DisplayName = "Elementary"),
    Intermediate, UMETA(DisplayName = "Intermediate"),
    Advanced, UMETA(DisplayName = "Advanced"),
    Expert UMETA(DisplayName = "Expert")
};

UENUM(BlueprintType)
enum class EGuidanceType : uint8
{
    Popup = 0, UMETA(DisplayName = "Popup"),
    Tooltip, UMETA(DisplayName = "Tooltip"),
    Video, UMETA(DisplayName = "Video"),
    InteractiveDemo, UMETA(DisplayName = "Interactive Demo"),
    Quiz, UMETA(DisplayName = "Quiz"),
    Highlight, UMETA(DisplayName = "Highlight"),
    AudioNarration, UMETA(DisplayName = "Audio Narration"),
    StepByStep, UMETA(DisplayName = "Step by Step"),
    Encyclopedia, UMETA(DisplayName = "Encyclopedia"),
    ReferenceCard UMETA(DisplayName = "Reference Card")
};

UENUM(BlueprintType)
enum class EProgressStatus : uint8
{
    Locked = 0, UMETA(DisplayName = "Locked"),
    Available, UMETA(DisplayName = "Available"),
    InProgress, UMETA(DisplayName = "In Progress"),
    Completed, UMETA(DisplayName = "Completed"),
    ReviewRequired UMETA(DisplayName = "Review Required")
};

USTRUCT(BlueprintType)
struct FGuidanceContent
{
    GENERATED_BODY()

    UPROPERTY()
    FString ContentID;

    UPROPERTY()
    FString Title;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    EGuidanceCategory Category;

    UPROPERTY()
    EGuidanceLevel Level;

    UPROPERTY()
    EGuidanceType Type;

    UPROPERTY()
    TArray<FString> Prerequisites;

    UPROPERTY()
    float EstimatedDurationMinutes;

    UPROPERTY()
    int32 ExperiencePoints;

    UPROPERTY()
    TMap<FString, FString> ContentData;

    UPROPERTY()
    FString MediaURL;

    UPROPERTY()
    bool bIsInteractive;

    UPROPERTY()
    int32 OrderIndex;

    FGuidanceContent()
        : Category(EGuidanceCategory::Tutorial)
        , Level(EGuidanceLevel::Beginner)
        , Type(EGuidanceType::Popup)
        , EstimatedDurationMinutes(5.0f)
        , ExperiencePoints(10)
        , bIsInteractive(false)
        , OrderIndex(0)
    {}
};

USTRUCT(BlueprintType)
struct FGuidanceProgress
{
    GENERATED_BODY()

    UPROPERTY()
    FString ContentID;

    UPROPERTY()
    EProgressStatus Status;

    UPROPERTY()
    float CompletionPercentage;

    UPROPERTY()
    uint32 StartTimestamp;

    UPROPERTY()
    uint32 CompletionTimestamp;

    UPROPERTY()
    int32 Attempts;

    UPROPERTY()
    float Score;

    UPROPERTY()
    TArray<FString> Notes;

    UPROPERTY()
    bool bBookmarked;

    FGuidanceProgress()
        : Status(EProgressStatus::Locked)
        , CompletionPercentage(0.0f)
        , StartTimestamp(0)
        , CompletionTimestamp(0)
        , Attempts(0)
        , Score(0.0f)
        , bBookmarked(false)
    {}
};

USTRUCT(BlueprintType)
struct FLearningPath
{
    GENERATED_BODY()

    UPROPERTY()
    FString PathID;

    UPROPERTY()
    FString Name;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    TArray<FString> ContentSequence;

    UPROPERTY()
    EGuidanceCategory Category;

    UPROPERTY()
    EGuidanceLevel TargetLevel;

    UPROPERTY()
    float EstimatedTotalHours;

    UPROPERTY()
    int32 TotalExperiencePoints;

    UPROPERTY()
    FString CertificateName;

    UPROPERTY()
    bool bIsLinear;

    FLearningPath()
        : Category(EGuidanceCategory::Tutorial)
        , TargetLevel(EGuidanceLevel::Beginner)
        , EstimatedTotalHours(1.0f)
        , TotalExperiencePoints(100)
        , bIsLinear(true)
    {}
};

USTRUCT(BlueprintType)
struct FStudentProfile
{
    GENERATED_BODY()

    UPROPERTY()
    FString StudentID;

    UPROPERTY()
    FString StudentName;

    UPROPERTY()
    FString ClassGroup;

    UPROPERTY()
    EGuidanceLevel CurrentLevel;

    UPROPERTY()
    int32 TotalExperiencePoints;

    UPROPERTY()
    int32 CompletedContentCount;

    UPROPERTY()
    float TotalStudyHours;

    UPROPERTY()
    TArray<FString> CompletedPaths;

    UPROPERTY()
    TArray<FString> Achievements;

    UPROPERTY()
    TMap<EGuidanceCategory, float> CategoryProgress;

    UPROPERTY()
    uint32 LastActivityTimestamp;

    FStudentProfile()
        : CurrentLevel(EGuidanceLevel::Beginner)
        , TotalExperiencePoints(0)
        , CompletedContentCount(0)
        , TotalStudyHours(0.0f)
        , LastActivityTimestamp(0)
    {}
};

USTRUCT(BlueprintType)
struct FQuizQuestion
{
    GENERATED_BODY()

    UPROPERTY()
    FString QuestionID;

    UPROPERTY()
    FString Question;

    UPROPERTY()
    TArray<FString> Options;

    UPROPERTY()
    int32 CorrectAnswerIndex;

    UPROPERTY()
    FString Explanation;

    UPROPERTY()
    EGuidanceCategory Category;

    UPROPERTY()
    int32 Difficulty;

    UPROPERTY()
    int32 Points;

    FQuizQuestion()
        : CorrectAnswerIndex(0)
        , Category(EGuidanceCategory::Tutorial)
        , Difficulty(1)
        , Points(10)
    {}
};

USTRUCT(BlueprintType)
struct FQuizResult
{
    GENERATED_BODY()

    UPROPERTY()
    FString QuizID;

    UPROPERTY()
    int32 TotalQuestions;

    UPROPERTY()
    int32 CorrectAnswers;

    UPROPERTY()
    float Score;

    UPROPERTY()
    int32 PointsEarned;

    UPROPERTY()
    uint32 CompletionTimestamp;

    UPROPERTY()
    TArray<FString> MissedQuestions;

    FQuizResult()
        : TotalQuestions(0)
        , CorrectAnswers(0)
        , Score(0.0f)
        , PointsEarned(0)
        , CompletionTimestamp(0)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGuidanceContentStarted, FString, ContentID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGuidanceContentCompleted, FString, ContentID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGuidanceProgressUpdated, FString, ContentID, float, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLearningPathCompleted, FString, PathID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuizCompleted, FQuizResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAchievementUnlocked, FString, AchievementID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUp, EGuidanceLevel, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStudyReminder);

/**
 * University Guidance System
 * Provides comprehensive guidance and tutorial systems for university/educational use
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingUniversityGuidanceSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingUniversityGuidanceSystem();

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void InitializeGuidanceSystem();

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void ShutdownGuidanceSystem();

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void RegisterStudent(const FString& StudentID, const FString& StudentName, const FString& ClassGroup);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void UnregisterStudent(const FString& StudentID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    FStudentProfile GetStudentProfile(const FString& StudentID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void UpdateStudentProfile(const FString& StudentID, const FStudentProfile& Profile);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void AddGuidanceContent(const FGuidanceContent& Content);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void RemoveGuidanceContent(const FString& ContentID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    FGuidanceContent GetGuidanceContent(const FString& ContentID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    TArray<FGuidanceContent> GetAllGuidanceContent();

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    TArray<FGuidanceContent> GetContentByCategory(EGuidanceCategory Category);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    TArray<FGuidanceContent> GetContentByLevel(EGuidanceLevel Level);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void StartGuidanceContent(const FString& StudentID, const FString& ContentID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void CompleteGuidanceContent(const FString& StudentID, const FString& ContentID, float Score);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void UpdateContentProgress(const FString& StudentID, const FString& ContentID, float ProgressPercentage);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    FGuidanceProgress GetContentProgress(const FString& StudentID, const FString& ContentID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    TArray<FGuidanceProgress> GetAllProgressForStudent(const FString& StudentID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void AddLearningPath(const FLearningPath& Path);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void RemoveLearningPath(const FString& PathID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    FLearningPath GetLearningPath(const FString& PathID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    TArray<FLearningPath> GetAllLearningPaths();

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void EnrollStudentInPath(const FString& StudentID, const FString& PathID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void CompleteLearningPath(const FString& StudentID, const FString& PathID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    float GetPathProgressForStudent(const FString& StudentID, const FString& PathID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    FGuidanceContent GetNextRecommendedContent(const FString& StudentID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void AddQuizQuestion(const FQuizQuestion& Question);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void RemoveQuizQuestion(const FString& QuestionID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    TArray<FQuizQuestion> GetQuizForCategory(EGuidanceCategory Category, int32 QuestionCount);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    FQuizResult SubmitQuizAnswers(const FString& StudentID, const FString& QuizID, const TArray<int32>& Answers);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void AwardExperiencePoints(const FString& StudentID, int32 Points);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void AwardAchievement(const FString& StudentID, const FString& AchievementID, const FString& AchievementName);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    bool CheckAndPerformLevelUp(const FString& StudentID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void ShowGuidancePopup(const FString& StudentID, const FString& ContentID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void ShowTooltip(const FString& StudentID, const FString& TargetElementID, const FString& Message);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void HighlightElement(const FString& StudentID, const FString& ElementID, float Duration);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void StartInteractiveDemo(const FString& StudentID, const FString& DemoID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void EnableStudyReminders(bool bEnable, float ReminderIntervalHours);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void BookmarkContent(const FString& StudentID, const FString& ContentID, bool bBookmark);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    TArray<FGuidanceContent> GetBookmarkedContent(const FString& StudentID);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void SearchContent(const FString& SearchQuery, TArray<FGuidanceContent>& OutResults);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void GenerateStudyPlan(const FString& StudentID, int32 TargetHoursPerWeek);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void ExportProgressReport(const FString& StudentID, const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void ImportProgressReport(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void PauseGuidance();

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void ResumeGuidance();

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void SkipCurrentContent();

    UFUNCTION(BlueprintCallable, Category = "University Guidance")
    void SetGuidanceEnabled(bool bEnabled);

    UFUNCTION(BlueprintPure, Category = "University Guidance")
    bool IsGuidanceEnabled() const;

    UFUNCTION(BlueprintPure, Category = "University Guidance")
    int32 GetTotalContentCount() const;

    UFUNCTION(BlueprintPure, Category = "University Guidance")
    int32 GetCompletedContentCount(const FString& StudentID);

    UFUNCTION(BlueprintPure, Category = "University Guidance")
    float GetOverallProgressForStudent(const FString& StudentID);

    UPROPERTY(BlueprintAssignable, Category = "Guidance Events")
    FOnGuidanceContentStarted OnContentStarted;

    UPROPERTY(BlueprintAssignable, Category = "Guidance Events")
    FOnGuidanceContentCompleted OnContentCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Guidance Events")
    FOnGuidanceProgressUpdated OnProgressUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Guidance Events")
    FOnLearningPathCompleted OnPathCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Guidance Events")
    FOnQuizCompleted OnQuizCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Guidance Events")
    FOnAchievementUnlocked OnAchievementUnlocked;

    UPROPERTY(BlueprintAssignable, Category = "Guidance Events")
    FOnLevelUp OnLevelUp;

    UPROPERTY(BlueprintAssignable, Category = "Guidance Events")
    FOnStudyReminder OnStudyReminder;

protected:
    UPROPERTY()
    TMap<FString, FGuidanceContent> GuidanceContents;

    UPROPERTY()
    TMap<FString, FLearningPath> LearningPaths;

    UPROPERTY()
    TMap<FString, FStudentProfile> StudentProfiles;

    UPROPERTY()
    TMap<FString, TMap<FString, FGuidanceProgress>> StudentProgress;

    UPROPERTY()
    TMap<FString, FQuizQuestion> QuizQuestions;

    UPROPERTY()
    TArray<FString> ActiveStudents;

    UPROPERTY()
    bool bGuidanceEnabled;

    UPROPERTY()
    bool bPaused;

    UPROPERTY()
    FString CurrentContentID;

    UPROPERTY()
    FTimerHandle ReminderTimer;

    void InitializeDefaultContent();
    void InitializeDefaultLearningPaths();
    void InitializeDefaultQuizzes();
    void SendReminderNotification();
    void UnlockPrerequisiteContent(const FString& StudentID, const FString& CompletedContentID);
    void CheckPathCompletion(const FString& StudentID, const FString& ContentID);
    void EvaluateAchievements(const FString& StudentID);
    bool IsContentUnlocked(const FString& StudentID, const FString& ContentID);
    TArray<FGuidanceContent> GetAvailableContent(const FString& StudentID);
    EGuidanceLevel CalculateLevelFromXP(int32 TotalXP);
    void SaveProgress(const FString& StudentID);
    void LoadProgress(const FString& StudentID);
    void LogGuidance(const FString& Message);
    static UMingUniversityGuidanceSystem* Get(UObject* WorldContextObject);
};
