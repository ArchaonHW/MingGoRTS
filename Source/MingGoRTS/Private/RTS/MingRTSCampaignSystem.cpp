#include "RTS/MingRTSCampaignSystem.h"

UMingRTSCampaignSystem::UMingRTSCampaignSystem()
{
    CurrentDifficulty = ERTSCampaignDifficulty::Normal;
}

void UMingRTSCampaignSystem::InitializeCampaignSystem()
{
    CampaignChapters.Empty();
    Objectives.Empty();
    StoryEvents.Empty();
    CampaignStates.Empty();
    ActiveCampaignID.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCampaignSystem: Campaign system initialized"));
}

void UMingRTSCampaignSystem::StartCampaign(const FString& CampaignID, ERTSCampaignType CampaignType, ERTSCampaignDifficulty Difficulty)
{
    CurrentDifficulty = Difficulty;
    ActiveCampaignID = CampaignID;
    CampaignStates.Add(CampaignID, ERTSCampaignState::InProgress);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCampaignSystem: Campaign %s started (Type: %d, Difficulty: %d)"), 
        *CampaignID, (int32)CampaignType, (int32)Difficulty);
    
    OnCampaignStarted.Broadcast(CampaignID, CampaignType);
    
    // Unlock first chapter if available
    TArray<FRTSCampaignChapter> Chapters = GetCampaignChapters(CampaignID);
    if (Chapters.Num() > 0)
    {
        OnChapterUnlocked.Broadcast(Chapters[0].ChapterID);
    }
}

void UMingRTSCampaignSystem::CompleteCampaign(const FString& CampaignID, bool bSuccess)
{
    CampaignStates.Add(CampaignID, bSuccess ? ERTSCampaignState::Completed : ERTSCampaignState::Failed);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCampaignSystem: Campaign %s %s"), 
        *CampaignID, bSuccess ? TEXT("completed successfully") : TEXT("failed"));
    
    OnCampaignCompleted.Broadcast(CampaignID, bSuccess);
    
    if (ActiveCampaignID == CampaignID)
    {
        ActiveCampaignID.Empty();
    }
}

void UMingRTSCampaignSystem::PauseCampaign(const FString& CampaignID)
{
    CampaignStates.Add(CampaignID, ERTSCampaignState::Paused);
    UE_LOG(LogTemp, Log, TEXT("MingRTSCampaignSystem: Campaign %s paused"), *CampaignID);
}

void UMingRTSCampaignSystem::ResumeCampaign(const FString& CampaignID)
{
    CampaignStates.Add(CampaignID, ERTSCampaignState::InProgress);
    UE_LOG(LogTemp, Log, TEXT("MingRTSCampaignSystem: Campaign %s resumed"), *CampaignID);
}

void UMingRTSCampaignSystem::AbandonCampaign(const FString& CampaignID)
{
    CampaignStates.Add(CampaignID, ERTSCampaignState::Abandoned);
    UE_LOG(LogTemp, Log, TEXT("MingRTSCampaignSystem: Campaign %s abandoned"), *CampaignID);
    
    if (ActiveCampaignID == CampaignID)
    {
        ActiveCampaignID.Empty();
    }
}

void UMingRTSCampaignSystem::RegisterCampaignChapter(const FString& CampaignID, const FRTSCampaignChapter& Chapter)
{
    CampaignChapters.Add(Chapter.ChapterID, Chapter);
    UE_LOG(LogTemp, Log, TEXT("MingRTSCampaignSystem: Chapter %s registered for campaign %s"), 
        *Chapter.ChapterID, *CampaignID);
}

void UMingRTSCampaignSystem::UpdateObjectiveProgress(const FString& ObjectiveID, float NewProgress)
{
    if (Objectives.Contains(ObjectiveID))
    {
        FRTSCampaignObjective& Objective = Objectives[ObjectiveID];
        Objective.Progress = FMath::Clamp(NewProgress, 0.0f, 1.0f);
        
        OnObjectiveUpdated.Broadcast(ObjectiveID, Objective.Progress);
        
        if (Objective.Progress >= 1.0f && !Objective.bCompleted)
        {
            CompleteObjective(ObjectiveID);
        }
    }
}

void UMingRTSCampaignSystem::CompleteObjective(const FString& ObjectiveID)
{
    if (Objectives.Contains(ObjectiveID))
    {
        FRTSCampaignObjective& Objective = Objectives[ObjectiveID];
        Objective.bCompleted = true;
        Objective.Progress = 1.0f;
        
        UE_LOG(LogTemp, Log, TEXT("MingRTSCampaignSystem: Objective %s completed"), *ObjectiveID);
        
        // Check if this completes a chapter
        for (auto& ChapterPair : CampaignChapters)
        {
            for (auto& ChapterObjective : ChapterPair.Value.Objectives)
            {
                if (ChapterObjective.ObjectiveID == ObjectiveID)
                {
                    CheckChapterCompletion(ChapterPair.Key);
                    break;
                }
            }
        }
    }
}

void UMingRTSCampaignSystem::TriggerStoryEvent(const FString& EventID)
{
    if (StoryEvents.Contains(EventID))
    {
        FRTSStoryEvent& Event = StoryEvents[EventID];
        Event.bTriggered = true;
        
        UE_LOG(LogTemp, Log, TEXT("MingRTSCampaignSystem: Story event %s triggered"), *EventID);
        
        OnStoryEventTriggered.Broadcast(EventID);
        ProcessStoryEventConsequences(EventID);
    }
}

void UMingRTSCampaignSystem::RegisterStoryEvent(const FRTSStoryEvent& StoryEvent)
{
    StoryEvents.Add(StoryEvent.EventID, StoryEvent);
    UE_LOG(LogTemp, Log, TEXT("MingRTSCampaignSystem: Story event %s registered"), *StoryEvent.EventID);
}

FRTSCampaignObjective UMingRTSCampaignSystem::GetObjective(const FString& ObjectiveID) const
{
    if (Objectives.Contains(ObjectiveID))
    {
        return Objectives[ObjectiveID];
    }
    return FRTSCampaignObjective();
}

TArray<FRTSCampaignObjective> UMingRTSCampaignSystem::GetActiveObjectives(const FString& CampaignID) const
{
    TArray<FRTSCampaignObjective> ActiveObjectives;
    
    for (const auto& ChapterPair : CampaignChapters)
    {
        if (ChapterPair.Value.Objectives.Num() > 0)
        {
            for (const auto& Objective : ChapterPair.Value.Objectives)
            {
                if (!Objective.bCompleted)
                {
                    ActiveObjectives.Add(Objective);
                }
            }
        }
    }
    
    return ActiveObjectives;
}

float UMingRTSCampaignSystem::GetCampaignProgress(const FString& CampaignID) const
{
    TArray<FRTSCampaignObjective> AllObjectives;
    int32 CompletedCount = 0;
    
    for (const auto& ChapterPair : CampaignChapters)
    {
        for (const auto& Objective : ChapterPair.Value.Objectives)
        {
            AllObjectives.Add(Objective);
            if (Objective.bCompleted)
            {
                CompletedCount++;
            }
        }
    }
    
    if (AllObjectives.Num() == 0)
    {
        return 0.0f;
    }
    
    return (float)CompletedCount / (float)AllObjectives.Num();
}

ERTSCampaignState UMingRTSCampaignSystem::GetCampaignState(const FString& CampaignID) const
{
    if (CampaignStates.Contains(CampaignID))
    {
        return CampaignStates[CampaignID];
    }
    return ERTSCampaignState::NotStarted;
}

TArray<FRTSCampaignChapter> UMingRTSCampaignSystem::GetCampaignChapters(const FString& CampaignID) const
{
    TArray<FRTSCampaignChapter> Chapters;
    
    for (const auto& ChapterPair : CampaignChapters)
    {
        Chapters.Add(ChapterPair.Value);
    }
    
    // Sort by chapter number
    Chapters.Sort([](const FRTSCampaignChapter& A, const FRTSCampaignChapter& B) {
        return A.ChapterNumber < B.ChapterNumber;
    });
    
    return Chapters;
}

void UMingRTSCampaignSystem::SetCampaignDifficulty(ERTSCampaignDifficulty NewDifficulty)
{
    CurrentDifficulty = NewDifficulty;
    UE_LOG(LogTemp, Log, TEXT("MingRTSCampaignSystem: Campaign difficulty set to %d"), (int32)NewDifficulty);
}

void UMingRTSCampaignSystem::CheckChapterCompletion(const FString& ChapterID)
{
    if (!CampaignChapters.Contains(ChapterID))
    {
        return;
    }
    
    FRTSCampaignChapter& Chapter = CampaignChapters[ChapterID];
    bool bAllObjectivesCompleted = true;
    bool bHasRequiredObjectives = false;
    
    for (auto& Objective : Chapter.Objectives)
    {
        if (!Objective.bOptional)
        {
            bHasRequiredObjectives = true;
            if (!Objective.bCompleted)
            {
                bAllObjectivesCompleted = false;
                break;
            }
        }
    }
    
    if (bAllObjectivesCompleted && bHasRequiredObjectives && !Chapter.bCompleted)
    {
        Chapter.bCompleted = true;
        UE_LOG(LogTemp, Log, TEXT("MingRTSCampaignSystem: Chapter %s completed"), *ChapterID);
        UnlockNextChapter(ChapterID);
    }
}

void UMingRTSCampaignSystem::UnlockNextChapter(const FString& CurrentChapterID)
{
    if (!CampaignChapters.Contains(CurrentChapterID))
    {
        return;
    }
    
    FRTSCampaignChapter& CurrentChapter = CampaignChapters[CurrentChapterID];
    int32 NextChapterNumber = CurrentChapter.ChapterNumber + 1;
    
    for (const auto& ChapterPair : CampaignChapters)
    {
        if (ChapterPair.Value.ChapterNumber == NextChapterNumber)
        {
            // Check if prerequisites are met
            if (ArePrerequisitesMet(ChapterPair.Value.UnlockRequirements))
            {
                OnChapterUnlocked.Broadcast(ChapterPair.Key);
                UE_LOG(LogTemp, Log, TEXT("MingRTSCampaignSystem: Chapter %s unlocked"), *ChapterPair.Key);
            }
            break;
        }
    }
}

void UMingRTSCampaignSystem::ProcessStoryEventConsequences(const FString& EventID)
{
    if (!StoryEvents.Contains(EventID))
    {
        return;
    }
    
    const FRTSStoryEvent& Event = StoryEvents[EventID];
    
    for (const FString& Consequence : Event.Consequences)
    {
        UE_LOG(LogTemp, Log, TEXT("MingRTSCampaignSystem: Processing consequence: %s"), *Consequence);
        // Implement consequence logic here
        // This could unlock new objectives, change AI behavior, etc.
    }
}

bool UMingRTSCampaignSystem::ArePrerequisitesMet(const TArray<FString>& Prerequisites) const
{
    if (Prerequisites.Num() == 0)
    {
        return true;
    }
    
    for (const FString& Prerequisite : Prerequisites)
    {
        bool bMet = false;
        
        // Check if prerequisite is a completed objective
        if (Objectives.Contains(Prerequisite))
        {
            bMet = Objectives[Prerequisite].bCompleted;
        }
        // Check if prerequisite is a completed chapter
        else if (CampaignChapters.Contains(Prerequisite))
        {
            bMet = CampaignChapters[Prerequisite].bCompleted;
        }
        // Check if prerequisite is a triggered story event
        else if (StoryEvents.Contains(Prerequisite))
        {
            bMet = StoryEvents[Prerequisite].bTriggered;
        }
        
        if (!bMet)
        {
            return false;
        }
    }
    
    return true;
}
