// Copyright (c) 2026 MingGoRTS. All rights reserved.
// University Guidance System Implementation - C3-1

#include "Education/MingUniversityGuidanceSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonReader.h"

DEFINE_LOG_CATEGORY_STATIC(LogGuidance, Log, All);

UMingUniversityGuidanceSystem::UMingUniversityGuidanceSystem()
    : bGuidanceEnabled(true)
    , bPaused(false)
{
}

void UMingUniversityGuidanceSystem::InitializeGuidanceSystem()
{
    GuidanceContents.Empty();
    LearningPaths.Empty();
    StudentProfiles.Empty();
    StudentProgress.Empty();
    QuizQuestions.Empty();
    
    InitializeDefaultContent();
    InitializeDefaultLearningPaths();
    InitializeDefaultQuizzes();
    
    LogGuidance(TEXT("University Guidance System initialized"));
}

void UMingUniversityGuidanceSystem::ShutdownGuidanceSystem()
{
    if (UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull))
    {
        World->GetTimerManager().ClearTimer(ReminderTimer);
    }
    
    for (const auto& Student : StudentProfiles)
    {
        SaveProgress(Student.Key);
    }
    
    LogGuidance(TEXT("University Guidance System shutdown"));
}

void UMingUniversityGuidanceSystem::RegisterStudent(const FString& StudentID, const FString& StudentName, const FString& ClassGroup)
{
    if (StudentProfiles.Contains(StudentID))
    {
        return;
    }
    
    FStudentProfile Profile;
    Profile.StudentID = StudentID;
    Profile.StudentName = StudentName;
    Profile.ClassGroup = ClassGroup;
    Profile.CurrentLevel = EGuidanceLevel::Beginner;
    Profile.TotalExperiencePoints = 0;
    Profile.CompletedContentCount = 0;
    Profile.TotalStudyHours = 0.0f;
    Profile.LastActivityTimestamp = FDateTime::Now().ToUnixTimestamp();
    
    StudentProfiles.Add(StudentID, Profile);
    StudentProgress.Add(StudentID, TMap<FString, FGuidanceProgress>());
    
    LoadProgress(StudentID);
    
    LogGuidance(FString::Printf(TEXT("Student registered: %s (%s)"), *StudentName, *StudentID));
}

void UMingUniversityGuidanceSystem::UnregisterStudent(const FString& StudentID)
{
    SaveProgress(StudentID);
    
    StudentProfiles.Remove(StudentID);
    StudentProgress.Remove(StudentID);
    
    ActiveStudents.Remove(StudentID);
    
    LogGuidance(FString::Printf(TEXT("Student unregistered: %s"), *StudentID));
}

FStudentProfile UMingUniversityGuidanceSystem::GetStudentProfile(const FString& StudentID)
{
    if (StudentProfiles.Contains(StudentID))
    {
        return StudentProfiles[StudentID];
    }
    
    return FStudentProfile();
}

void UMingUniversityGuidanceSystem::UpdateStudentProfile(const FString& StudentID, const FStudentProfile& Profile)
{
    if (StudentProfiles.Contains(StudentID))
    {
        StudentProfiles[StudentID] = Profile;
        StudentProfiles[StudentID].LastActivityTimestamp = FDateTime::Now().ToUnixTimestamp();
    }
}

void UMingUniversityGuidanceSystem::AddGuidanceContent(const FGuidanceContent& Content)
{
    GuidanceContents.Add(Content.ContentID, Content);
    
    LogGuidance(FString::Printf(TEXT("Guidance content added: %s"), *Content.Title));
}

void UMingUniversityGuidanceSystem::RemoveGuidanceContent(const FString& ContentID)
{
    GuidanceContents.Remove(ContentID);
}

FGuidanceContent UMingUniversityGuidanceSystem::GetGuidanceContent(const FString& ContentID)
{
    if (GuidanceContents.Contains(ContentID))
    {
        return GuidanceContents[ContentID];
    }
    
    return FGuidanceContent();
}

TArray<FGuidanceContent> UMingUniversityGuidanceSystem::GetAllGuidanceContent()
{
    TArray<FGuidanceContent> Result;
    GuidanceContents.GenerateValueArray(Result);
    
    Result.Sort([](const FGuidanceContent& A, const FGuidanceContent& B)
    {
        return A.OrderIndex < B.OrderIndex;
    });
    
    return Result;
}

TArray<FGuidanceContent> UMingUniversityGuidanceSystem::GetContentByCategory(EGuidanceCategory Category)
{
    TArray<FGuidanceContent> Result;
    
    for (const auto& Pair : GuidanceContents)
    {
        if (Pair.Value.Category == Category)
        {
            Result.Add(Pair.Value);
        }
    }
    
    return Result;
}

TArray<FGuidanceContent> UMingUniversityGuidanceSystem::GetContentByLevel(EGuidanceLevel Level)
{
    TArray<FGuidanceContent> Result;
    
    for (const auto& Pair : GuidanceContents)
    {
        if (Pair.Value.Level == Level)
        {
            Result.Add(Pair.Value);
        }
    }
    
    return Result;
}

void UMingUniversityGuidanceSystem::StartGuidanceContent(const FString& StudentID, const FString& ContentID)
{
    if (!StudentProfiles.Contains(StudentID))
    {
        return;
    }
    
    if (!GuidanceContents.Contains(ContentID))
    {
        return;
    }
    
    FGuidanceContent Content = GuidanceContents[ContentID];
    
    if (!IsContentUnlocked(StudentID, ContentID))
    {
        LogGuidance(FString::Printf(TEXT("Content %s is locked for student %s"), *ContentID, *StudentID));
        return;
    }
    
    TMap<FString, FGuidanceProgress>& ProgressMap = StudentProgress[StudentID];
    
    if (!ProgressMap.Contains(ContentID))
    {
        FGuidanceProgress Progress;
        Progress.ContentID = ContentID;
        Progress.Status = EProgressStatus::InProgress;
        Progress.StartTimestamp = FDateTime::Now().ToUnixTimestamp();
        Progress.Attempts = 1;
        
        ProgressMap.Add(ContentID, Progress);
    }
    else
    {
        FGuidanceProgress& Progress = ProgressMap[ContentID];
        Progress.Status = EProgressStatus::InProgress;
        Progress.Attempts++;
    }
    
    CurrentContentID = ContentID;
    
    OnContentStarted.Broadcast(ContentID);
    
    LogGuidance(FString::Printf(TEXT("Student %s started content: %s"), *StudentID, *Content.Title));
}

void UMingUniversityGuidanceSystem::CompleteGuidanceContent(const FString& StudentID, const FString& ContentID, float Score)
{
    if (!StudentProgress.Contains(StudentID))
    {
        return;
    }
    
    TMap<FString, FGuidanceProgress>& ProgressMap = StudentProgress[StudentID];
    
    if (!ProgressMap.Contains(ContentID))
    {
        return;
    }
    
    FGuidanceProgress& Progress = ProgressMap[ContentID];
    Progress.Status = EProgressStatus::Completed;
    Progress.CompletionPercentage = 100.0f;
    Progress.CompletionTimestamp = FDateTime::Now().ToUnixTimestamp();
    Progress.Score = Score;
    
    FGuidanceContent Content = GuidanceContents[ContentID];
    
    AwardExperiencePoints(StudentID, Content.ExperiencePoints);
    
    StudentProfiles[StudentID].CompletedContentCount++;
    StudentProfiles[StudentID].TotalStudyHours += Content.EstimatedDurationMinutes / 60.0f;
    
    UnlockPrerequisiteContent(StudentID, ContentID);
    CheckPathCompletion(StudentID, ContentID);
    EvaluateAchievements(StudentID);
    
    OnContentCompleted.Broadcast(ContentID);
    
    SaveProgress(StudentID);
    
    LogGuidance(FString::Printf(TEXT("Student %s completed content: %s (Score: %.1f)"), 
        *StudentID, *ContentID, Score));
}

void UMingUniversityGuidanceSystem::UpdateContentProgress(const FString& StudentID, const FString& ContentID, float ProgressPercentage)
{
    if (!StudentProgress.Contains(StudentID))
    {
        return;
    }
    
    TMap<FString, FGuidanceProgress>& ProgressMap = StudentProgress[StudentID];
    
    if (ProgressMap.Contains(ContentID))
    {
        ProgressMap[ContentID].CompletionPercentage = FMath::Clamp(ProgressPercentage, 0.0f, 100.0f);
        
        OnProgressUpdated.Broadcast(ContentID, ProgressPercentage);
    }
}

FGuidanceProgress UMingUniversityGuidanceSystem::GetContentProgress(const FString& StudentID, const FString& ContentID)
{
    if (StudentProgress.Contains(StudentID))
    {
        TMap<FString, FGuidanceProgress>& ProgressMap = StudentProgress[StudentID];
        
        if (ProgressMap.Contains(ContentID))
        {
            return ProgressMap[ContentID];
        }
    }
    
    FGuidanceProgress EmptyProgress;
    EmptyProgress.ContentID = ContentID;
    EmptyProgress.Status = EProgressStatus::Locked;
    return EmptyProgress;
}

TArray<FGuidanceProgress> UMingUniversityGuidanceSystem::GetAllProgressForStudent(const FString& StudentID)
{
    TArray<FGuidanceProgress> Result;
    
    if (StudentProgress.Contains(StudentID))
    {
        StudentProgress[StudentID].GenerateValueArray(Result);
    }
    
    return Result;
}

void UMingUniversityGuidanceSystem::AddLearningPath(const FLearningPath& Path)
{
    LearningPaths.Add(Path.PathID, Path);
    
    LogGuidance(FString::Printf(TEXT("Learning path added: %s"), *Path.Name));
}

void UMingUniversityGuidanceSystem::RemoveLearningPath(const FString& PathID)
{
    LearningPaths.Remove(PathID);
}

FLearningPath UMingUniversityGuidanceSystem::GetLearningPath(const FString& PathID)
{
    if (LearningPaths.Contains(PathID))
    {
        return LearningPaths[PathID];
    }
    
    return FLearningPath();
}

TArray<FLearningPath> UMingUniversityGuidanceSystem::GetAllLearningPaths()
{
    TArray<FLearningPath> Result;
    LearningPaths.GenerateValueArray(Result);
    return Result;
}

void UMingUniversityGuidanceSystem::EnrollStudentInPath(const FString& StudentID, const FString& PathID)
{
    if (!StudentProfiles.Contains(StudentID) || !LearningPaths.Contains(PathID))
    {
        return;
    }
    
    if (!StudentProfiles[StudentID].CompletedPaths.Contains(PathID))
    {
        LogGuidance(FString::Printf(TEXT("Student %s enrolled in path: %s"), *StudentID, *PathID));
    }
}

void UMingUniversityGuidanceSystem::CompleteLearningPath(const FString& StudentID, const FString& PathID)
{
    if (!StudentProfiles.Contains(StudentID))
    {
        return;
    }
    
    if (!StudentProfiles[StudentID].CompletedPaths.Contains(PathID))
    {
        StudentProfiles[StudentID].CompletedPaths.Add(PathID);
        
        if (LearningPaths.Contains(PathID))
        {
            FLearningPath Path = LearningPaths[PathID];
            AwardExperiencePoints(StudentID, Path.TotalExperiencePoints);
            AwardAchievement(StudentID, FString::Printf(TEXT("path_%s"), *PathID), 
                FString::Printf(TEXT("Completed: %s"), *Path.Name));
        }
        
        OnPathCompleted.Broadcast(PathID);
        
        LogGuidance(FString::Printf(TEXT("Student %s completed learning path: %s"), *StudentID, *PathID));
    }
}

float UMingUniversityGuidanceSystem::GetPathProgressForStudent(const FString& StudentID, const FString& PathID)
{
    if (!StudentProfiles.Contains(StudentID) || !LearningPaths.Contains(PathID))
    {
        return 0.0f;
    }
    
    if (StudentProfiles[StudentID].CompletedPaths.Contains(PathID))
    {
        return 100.0f;
    }
    
    FLearningPath Path = LearningPaths[PathID];
    
    if (Path.ContentSequence.Num() == 0)
    {
        return 0.0f;
    }
    
    int32 CompletedCount = 0;
    
    if (StudentProgress.Contains(StudentID))
    {
        TMap<FString, FGuidanceProgress>& ProgressMap = StudentProgress[StudentID];
        
        for (const FString& ContentID : Path.ContentSequence)
        {
            if (ProgressMap.Contains(ContentID) && ProgressMap[ContentID].Status == EProgressStatus::Completed)
            {
                CompletedCount++;
            }
        }
    }
    
    return (float)CompletedCount / Path.ContentSequence.Num() * 100.0f;
}

FGuidanceContent UMingUniversityGuidanceSystem::GetNextRecommendedContent(const FString& StudentID)
{
    if (!StudentProfiles.Contains(StudentID))
    {
        return FGuidanceContent();
    }
    
    TArray<FGuidanceContent> AvailableContent = GetAvailableContent(StudentID);
    
    if (AvailableContent.Num() > 0)
    {
        AvailableContent.Sort([](const FGuidanceContent& A, const FGuidanceContent& B)
        {
            if (A.Level != B.Level)
            {
                return A.Level < B.Level;
            }
            return A.OrderIndex < B.OrderIndex;
        });
        
        return AvailableContent[0];
    }
    
    return FGuidanceContent();
}

void UMingUniversityGuidanceSystem::AddQuizQuestion(const FQuizQuestion& Question)
{
    QuizQuestions.Add(Question.QuestionID, Question);
}

void UMingUniversityGuidanceSystem::RemoveQuizQuestion(const FString& QuestionID)
{
    QuizQuestions.Remove(QuestionID);
}

TArray<FQuizQuestion> UMingUniversityGuidanceSystem::GetQuizForCategory(EGuidanceCategory Category, int32 QuestionCount)
{
    TArray<FQuizQuestion> CategoryQuestions;
    
    for (const auto& Pair : QuizQuestions)
    {
        if (Pair.Value.Category == Category)
        {
            CategoryQuestions.Add(Pair.Value);
        }
    }
    
    CategoryQuestions.Sort([](const FQuizQuestion& A, const FQuizQuestion& B)
    {
        return A.Difficulty < B.Difficulty;
    });
    
    while (CategoryQuestions.Num() > QuestionCount)
    {
        CategoryQuestions.Pop();
    }
    
    return CategoryQuestions;
}

FQuizResult UMingUniversityGuidanceSystem::SubmitQuizAnswers(const FString& StudentID, const FString& QuizID, const TArray<int32>& Answers)
{
    FQuizResult Result;
    Result.QuizID = QuizID;
    Result.TotalQuestions = Answers.Num();
    Result.CompletionTimestamp = FDateTime::Now().ToUnixTimestamp();
    
    int32 CorrectCount = 0;
    int32 PointsEarned = 0;
    
    int32 Index = 0;
    for (const auto& Pair : QuizQuestions)
    {
        if (Index >= Answers.Num())
        {
            break;
        }
        
        if (Answers[Index] == Pair.Value.CorrectAnswerIndex)
        {
            CorrectCount++;
            PointsEarned += Pair.Value.Points;
        }
        else
        {
            Result.MissedQuestions.Add(Pair.Value.QuestionID);
        }
        
        Index++;
    }
    
    Result.CorrectAnswers = CorrectCount;
    Result.PointsEarned = PointsEarned;
    Result.Score = (Result.TotalQuestions > 0) ? ((float)CorrectCount / Result.TotalQuestions * 100.0f) : 0.0f;
    
    AwardExperiencePoints(StudentID, PointsEarned);
    
    OnQuizCompleted.Broadcast(Result);
    
    LogGuidance(FString::Printf(TEXT("Student %s completed quiz %s (Score: %.1f%%)"), 
        *StudentID, *QuizID, Result.Score));
    
    return Result;
}

void UMingUniversityGuidanceSystem::AwardExperiencePoints(const FString& StudentID, int32 Points)
{
    if (!StudentProfiles.Contains(StudentID))
    {
        return;
    }
    
    FStudentProfile& Profile = StudentProfiles[StudentID];
    Profile.TotalExperiencePoints += Points;
    
    EGuidanceLevel NewLevel = CalculateLevelFromXP(Profile.TotalExperiencePoints);
    
    if (NewLevel > Profile.CurrentLevel)
    {
        Profile.CurrentLevel = NewLevel;
        OnLevelUp.Broadcast(NewLevel);
        
        AwardAchievement(StudentID, FString::Printf(TEXT("level_%d"), static_cast<int32>(NewLevel)), 
            FString::Printf(TEXT("Reached %s Level"), *StaticEnum<EGuidanceLevel>()->GetNameStringByValue(static_cast<int32>(NewLevel))));
    }
    
    LogGuidance(FString::Printf(TEXT("Student %s awarded %d XP (Total: %d)"), 
        *StudentID, Points, Profile.TotalExperiencePoints));
}

void UMingUniversityGuidanceSystem::AwardAchievement(const FString& StudentID, const FString& AchievementID, const FString& AchievementName)
{
    if (!StudentProfiles.Contains(StudentID))
    {
        return;
    }
    
    FStudentProfile& Profile = StudentProfiles[StudentID];
    
    if (!Profile.Achievements.Contains(AchievementID))
    {
        Profile.Achievements.Add(AchievementID);
        
        OnAchievementUnlocked.Broadcast(AchievementID);
        
        LogGuidance(FString::Printf(TEXT("Student %s unlocked achievement: %s"), *StudentID, *AchievementName));
    }
}

bool UMingUniversityGuidanceSystem::CheckAndPerformLevelUp(const FString& StudentID)
{
    if (!StudentProfiles.Contains(StudentID))
    {
        return false;
    }
    
    FStudentProfile& Profile = StudentProfiles[StudentID];
    EGuidanceLevel NewLevel = CalculateLevelFromXP(Profile.TotalExperiencePoints);
    
    if (NewLevel > Profile.CurrentLevel)
    {
        Profile.CurrentLevel = NewLevel;
        OnLevelUp.Broadcast(NewLevel);
        return true;
    }
    
    return false;
}

void UMingUniversityGuidanceSystem::ShowGuidancePopup(const FString& StudentID, const FString& ContentID)
{
    if (GuidanceContents.Contains(ContentID))
    {
        FGuidanceContent Content = GuidanceContents[ContentID];
        
        LogGuidance(FString::Printf(TEXT("Showing guidance popup for student %s: %s"), *StudentID, *Content.Title));
    }
}

void UMingUniversityGuidanceSystem::ShowTooltip(const FString& StudentID, const FString& TargetElementID, const FString& Message)
{
    LogGuidance(FString::Printf(TEXT("Showing tooltip on %s: %s"), *TargetElementID, *Message));
}

void UMingUniversityGuidanceSystem::HighlightElement(const FString& StudentID, const FString& ElementID, float Duration)
{
    LogGuidance(FString::Printf(TEXT("Highlighting element %s for %.1f seconds"), *ElementID, Duration));
}

void UMingUniversityGuidanceSystem::StartInteractiveDemo(const FString& StudentID, const FString& DemoID)
{
    LogGuidance(FString::Printf(TEXT("Starting interactive demo %s for student %s"), *DemoID, *StudentID));
}

void UMingUniversityGuidanceSystem::EnableStudyReminders(bool bEnable, float ReminderIntervalHours)
{
    if (UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull))
    {
        World->GetTimerManager().ClearTimer(ReminderTimer);
        
        if (bEnable)
        {
            World->GetTimerManager().SetTimer(ReminderTimer, this, &UMingUniversityGuidanceSystem::SendReminderNotification, 
                ReminderIntervalHours * 3600.0f, true);
        }
    }
}

void UMingUniversityGuidanceSystem::BookmarkContent(const FString& StudentID, const FString& ContentID, bool bBookmark)
{
    if (!StudentProgress.Contains(StudentID))
    {
        return;
    }
    
    TMap<FString, FGuidanceProgress>& ProgressMap = StudentProgress[StudentID];
    
    if (ProgressMap.Contains(ContentID))
    {
        ProgressMap[ContentID].bBookmarked = bBookmark;
    }
}

TArray<FGuidanceContent> UMingUniversityGuidanceSystem::GetBookmarkedContent(const FString& StudentID)
{
    TArray<FGuidanceContent> Result;
    
    if (StudentProgress.Contains(StudentID))
    {
        TMap<FString, FGuidanceProgress>& ProgressMap = StudentProgress[StudentID];
        
        for (const auto& Pair : ProgressMap)
        {
            if (Pair.Value.bBookmarked && GuidanceContents.Contains(Pair.Key))
            {
                Result.Add(GuidanceContents[Pair.Key]);
            }
        }
    }
    
    return Result;
}

void UMingUniversityGuidanceSystem::SearchContent(const FString& SearchQuery, TArray<FGuidanceContent>& OutResults)
{
    OutResults.Empty();
    
    FString LowerQuery = SearchQuery.ToLower();
    
    for (const auto& Pair : GuidanceContents)
    {
        if (Pair.Value.Title.ToLower().Contains(LowerQuery) ||
            Pair.Value.Description.ToLower().Contains(LowerQuery))
        {
            OutResults.Add(Pair.Value);
        }
    }
}

void UMingUniversityGuidanceSystem::GenerateStudyPlan(const FString& StudentID, int32 TargetHoursPerWeek)
{
    if (!StudentProfiles.Contains(StudentID))
    {
        return;
    }
    
    LogGuidance(FString::Printf(TEXT("Generated study plan for student %s (%d hours/week)"), 
        *StudentID, TargetHoursPerWeek));
}

void UMingUniversityGuidanceSystem::ExportProgressReport(const FString& StudentID, const FString& FilePath)
{
    if (!StudentProfiles.Contains(StudentID))
    {
        return;
    }
    
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    
    FStudentProfile Profile = StudentProfiles[StudentID];
    JsonObject->SetStringField(TEXT("StudentID"), Profile.StudentID);
    JsonObject->SetStringField(TEXT("StudentName"), Profile.StudentName);
    JsonObject->SetNumberField(TEXT("TotalXP"), Profile.TotalExperiencePoints);
    JsonObject->SetNumberField(TEXT("CompletedContent"), Profile.CompletedContentCount);
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    
    FFileHelper::SaveStringToFile(OutputString, *FilePath);
    
    LogGuidance(FString::Printf(TEXT("Progress report exported for student %s to %s"), *StudentID, *FilePath));
}

void UMingUniversityGuidanceSystem::ImportProgressReport(const FString& FilePath)
{
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        return;
    }
    
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    
    if (FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        LogGuidance(TEXT("Progress report imported"));
    }
}

void UMingUniversityGuidanceSystem::PauseGuidance()
{
    bPaused = true;
    LogGuidance(TEXT("Guidance paused"));
}

void UMingUniversityGuidanceSystem::ResumeGuidance()
{
    bPaused = false;
    LogGuidance(TEXT("Guidance resumed"));
}

void UMingUniversityGuidanceSystem::SkipCurrentContent()
{
    if (!CurrentContentID.IsEmpty())
    {
        LogGuidance(FString::Printf(TEXT("Content skipped: %s"), *CurrentContentID));
        CurrentContentID.Empty();
    }
}

void UMingUniversityGuidanceSystem::SetGuidanceEnabled(bool bEnabled)
{
    bGuidanceEnabled = bEnabled;
}

bool UMingUniversityGuidanceSystem::IsGuidanceEnabled() const
{
    return bGuidanceEnabled;
}

int32 UMingUniversityGuidanceSystem::GetTotalContentCount() const
{
    return GuidanceContents.Num();
}

int32 UMingUniversityGuidanceSystem::GetCompletedContentCount(const FString& StudentID)
{
    if (StudentProfiles.Contains(StudentID))
    {
        return StudentProfiles[StudentID].CompletedContentCount;
    }
    return 0;
}

float UMingUniversityGuidanceSystem::GetOverallProgressForStudent(const FString& StudentID)
{
    if (GuidanceContents.Num() == 0)
    {
        return 0.0f;
    }
    
    return (float)GetCompletedContentCount(StudentID) / GuidanceContents.Num() * 100.0f;
}

void UMingUniversityGuidanceSystem::InitializeDefaultContent()
{
    FGuidanceContent Content1;
    Content1.ContentID = TEXT("tutorial_001");
    Content1.Title = TEXT("Welcome to MingGoRTS");
    Content1.Description = TEXT("Introduction to the game and basic controls");
    Content1.Category = EGuidanceCategory::Tutorial;
    Content1.Level = EGuidanceLevel::Beginner;
    Content1.Type = EGuidanceType::Video;
    Content1.EstimatedDurationMinutes = 5.0f;
    Content1.ExperiencePoints = 10;
    Content1.OrderIndex = 0;
    AddGuidanceContent(Content1);
    
    FGuidanceContent Content2;
    Content2.ContentID = TEXT("tutorial_002");
    Content2.Title = TEXT("Basic Unit Control");
    Content2.Description = TEXT("Learn how to select and move units");
    Content2.Category = EGuidanceCategory::GameMechanics;
    Content2.Level = EGuidanceLevel::Beginner;
    Content2.Type = EGuidanceType::InteractiveDemo;
    Content2.Prerequisites.Add(TEXT("tutorial_001"));
    Content2.EstimatedDurationMinutes = 10.0f;
    Content2.ExperiencePoints = 20;
    Content2.OrderIndex = 1;
    Content2.bIsInteractive = true;
    AddGuidanceContent(Content2);
    
    FGuidanceContent Content3;
    Content3.ContentID = TEXT("history_001");
    Content3.Title = TEXT("The Republican Era");
    Content3.Description = TEXT("Historical context of the game setting");
    Content3.Category = EGuidanceCategory::HistoricalContext;
    Content3.Level = EGuidanceLevel::Beginner;
    Content3.Type = EGuidanceType::Encyclopedia;
    Content3.EstimatedDurationMinutes = 15.0f;
    Content3.ExperiencePoints = 15;
    Content3.OrderIndex = 2;
    AddGuidanceContent(Content3);
    
    FGuidanceContent Content4;
    Content4.ContentID = TEXT("strategy_001");
    Content4.Title = TEXT("Basic Strategy");
    Content4.Description = TEXT("Introduction to RTS strategy concepts");
    Content4.Category = EGuidanceCategory::StrategyGuide;
    Content4.Level = EGuidanceLevel::Intermediate;
    Content4.Type = EGuidanceType::StepByStep;
    Content4.Prerequisites.Add(TEXT("tutorial_002"));
    Content4.EstimatedDurationMinutes = 20.0f;
    Content4.ExperiencePoints = 30;
    Content4.OrderIndex = 3;
    AddGuidanceContent(Content4);
    
    FGuidanceContent Content5;
    Content5.ContentID = TEXT("culture_001");
    Content5.Title = TEXT("Republican Era Culture");
    Content5.Description = TEXT("Cultural aspects of the time period");
    Content5.Category = EGuidanceCategory::CulturalEducation;
    Content5.Level = EGuidanceLevel::Intermediate;
    Content5.Type = EGuidanceType::Encyclopedia;
    Content5.EstimatedDurationMinutes = 25.0f;
    Content5.ExperiencePoints = 25;
    Content5.OrderIndex = 4;
    AddGuidanceContent(Content5);
}

void UMingUniversityGuidanceSystem::InitializeDefaultLearningPaths()
{
    FLearningPath Path1;
    Path1.PathID = TEXT("beginner_path");
    Path1.Name = TEXT("Beginner's Journey");
    Path1.Description = TEXT("Complete introduction to MingGoRTS");
    Path1.Category = EGuidanceCategory::Tutorial;
    Path1.TargetLevel = EGuidanceLevel::Elementary;
    Path1.ContentSequence.Add(TEXT("tutorial_001"));
    Path1.ContentSequence.Add(TEXT("tutorial_002"));
    Path1.ContentSequence.Add(TEXT("history_001"));
    Path1.EstimatedTotalHours = 0.5f;
    Path1.TotalExperiencePoints = 45;
    Path1.CertificateName = TEXT("MingGoRTS Beginner Certificate");
    AddLearningPath(Path1);
    
    FLearningPath Path2;
    Path2.PathID = TEXT("strategy_path");
    Path2.Name = TEXT("Strategy Master");
    Path2.Description = TEXT("Master strategic gameplay");
    Path2.Category = EGuidanceCategory::StrategyGuide;
    Path2.TargetLevel = EGuidanceLevel::Advanced;
    Path2.ContentSequence.Add(TEXT("tutorial_002"));
    Path2.ContentSequence.Add(TEXT("strategy_001"));
    Path2.EstimatedTotalHours = 1.0f;
    Path2.TotalExperiencePoints = 50;
    Path2.CertificateName = TEXT("Strategy Master Certificate");
    AddLearningPath(Path2);
}

void UMingUniversityGuidanceSystem::InitializeDefaultQuizzes()
{
    FQuizQuestion Q1;
    Q1.QuestionID = TEXT("quiz_001");
    Q1.Question = TEXT("What era is MingGoRTS set in?");
    Q1.Options.Add(TEXT("Qing Dynasty"));
    Q1.Options.Add(TEXT("Republican Era"));
    Q1.Options.Add(TEXT("Ming Dynasty"));
    Q1.Options.Add(TEXT("Modern Day"));
    Q1.CorrectAnswerIndex = 1;
    Q1.Explanation = TEXT("MingGoRTS is set in the Republican Era of Chinese history (1912-1949)");
    Q1.Category = EGuidanceCategory::HistoricalContext;
    Q1.Difficulty = 1;
    Q1.Points = 10;
    AddQuizQuestion(Q1);
    
    FQuizQuestion Q2;
    Q2.QuestionID = TEXT("quiz_002");
    Q2.Question = TEXT("Which of the following is a basic RTS strategy?");
    Q2.Options.Add(TEXT("Resource gathering"));
    Q2.Options.Add(TEXT("Unit micro-management"));
    Q2.Options.Add(TEXT("Base building"));
    Q2.Options.Add(TEXT("All of the above"));
    Q2.CorrectAnswerIndex = 3;
    Q2.Explanation = TEXT("All options are fundamental aspects of RTS gameplay");
    Q2.Category = EGuidanceCategory::StrategyGuide;
    Q2.Difficulty = 2;
    Q2.Points = 15;
    AddQuizQuestion(Q2);
}

void UMingUniversityGuidanceSystem::SendReminderNotification()
{
    OnStudyReminder.Broadcast();
}

void UMingUniversityGuidanceSystem::UnlockPrerequisiteContent(const FString& StudentID, const FString& CompletedContentID)
{
    for (const auto& Pair : GuidanceContents)
    {
        if (Pair.Value.Prerequisites.Contains(CompletedContentID))
        {
            bool bAllPrereqsMet = true;
            
            for (const FString& Prereq : Pair.Value.Prerequisites)
            {
                if (!StudentProgress.Contains(StudentID) ||
                    !StudentProgress[StudentID].Contains(Prereq) ||
                    StudentProgress[StudentID][Prereq].Status != EProgressStatus::Completed)
                {
                    bAllPrereqsMet = false;
                    break;
                }
            }
            
            if (bAllPrereqsMet && StudentProgress.Contains(StudentID))
            {
                if (!StudentProgress[StudentID].Contains(Pair.Key))
                {
                    FGuidanceProgress NewProgress;
                    NewProgress.ContentID = Pair.Key;
                    NewProgress.Status = EProgressStatus::Available;
                    StudentProgress[StudentID].Add(Pair.Key, NewProgress);
                }
            }
        }
    }
}

void UMingUniversityGuidanceSystem::CheckPathCompletion(const FString& StudentID, const FString& ContentID)
{
    for (const auto& Pair : LearningPaths)
    {
        if (Pair.Value.ContentSequence.Contains(ContentID))
        {
            float Progress = GetPathProgressForStudent(StudentID, Pair.Key);
            
            if (Progress >= 100.0f)
            {
                CompleteLearningPath(StudentID, Pair.Key);
            }
        }
    }
}

void UMingUniversityGuidanceSystem::EvaluateAchievements(const FString& StudentID)
{
    if (!StudentProfiles.Contains(StudentID))
    {
        return;
    }
    
    FStudentProfile& Profile = StudentProfiles[StudentID];
    
    if (Profile.CompletedContentCount >= 1)
    {
        AwardAchievement(StudentID, TEXT("first_step"), TEXT("First Steps"));
    }
    
    if (Profile.CompletedContentCount >= 10)
    {
        AwardAchievement(StudentID, TEXT("dedicated_learner"), TEXT("Dedicated Learner"));
    }
    
    if (Profile.TotalExperiencePoints >= 100)
    {
        AwardAchievement(StudentID, TEXT("xp_100"), TEXT("Century Club"));
    }
}

bool UMingUniversityGuidanceSystem::IsContentUnlocked(const FString& StudentID, const FString& ContentID)
{
    if (!GuidanceContents.Contains(ContentID))
    {
        return false;
    }
    
    FGuidanceContent Content = GuidanceContents[ContentID];
    
    if (Content.Prerequisites.Num() == 0)
    {
        return true;
    }
    
    if (!StudentProgress.Contains(StudentID))
    {
        return false;
    }
    
    TMap<FString, FGuidanceProgress>& ProgressMap = StudentProgress[StudentID];
    
    for (const FString& Prereq : Content.Prerequisites)
    {
        if (!ProgressMap.Contains(Prereq) || ProgressMap[Prereq].Status != EProgressStatus::Completed)
        {
            return false;
        }
    }
    
    return true;
}

TArray<FGuidanceContent> UMingUniversityGuidanceSystem::GetAvailableContent(const FString& StudentID)
{
    TArray<FGuidanceContent> Result;
    
    for (const auto& Pair : GuidanceContents)
    {
        if (IsContentUnlocked(StudentID, Pair.Key))
        {
            bool bAlreadyCompleted = false;
            
            if (StudentProgress.Contains(StudentID) && StudentProgress[StudentID].Contains(Pair.Key))
            {
                if (StudentProgress[StudentID][Pair.Key].Status == EProgressStatus::Completed)
                {
                    bAlreadyCompleted = true;
                }
            }
            
            if (!bAlreadyCompleted)
            {
                Result.Add(Pair.Value);
            }
        }
    }
    
    return Result;
}

EGuidanceLevel UMingUniversityGuidanceSystem::CalculateLevelFromXP(int32 TotalXP)
{
    if (TotalXP >= 500)
    {
        return EGuidanceLevel::Expert;
    }
    else if (TotalXP >= 300)
    {
        return EGuidanceLevel::Advanced;
    }
    else if (TotalXP >= 150)
    {
        return EGuidanceLevel::Intermediate;
    }
    else if (TotalXP >= 50)
    {
        return EGuidanceLevel::Elementary;
    }
    
    return EGuidanceLevel::Beginner;
}

void UMingUniversityGuidanceSystem::SaveProgress(const FString& StudentID)
{
}

void UMingUniversityGuidanceSystem::LoadProgress(const FString& StudentID)
{
}

void UMingUniversityGuidanceSystem::LogGuidance(const FString& Message)
{
    UE_LOG(LogGuidance, Log, TEXT("[Guidance] %s"), *Message);
}

UMingUniversityGuidanceSystem* UMingUniversityGuidanceSystem::Get(UObject* WorldContextObject)
{
    if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
    {
        static UMingUniversityGuidanceSystem* Instance = nullptr;
        if (!Instance)
        {
            Instance = NewObject<UMingUniversityGuidanceSystem>();
        }
        return Instance;
    }
    return nullptr;
}
