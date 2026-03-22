#include "MingDynamicTaskGenerator.h"
#include "Engine/DataTable.h"
#include "Kismet/KismetSystemLibrary.h"

UMingDynamicTaskGenerator::UMingDynamicTaskGenerator()
    : PersonalizationWeight(0.7f)
    , AdaptationThreshold(0.3f)
    , MaxConcurrentTasks(5)
{
}

void UMingDynamicTaskGenerator::InitializeTaskGenerator()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Dynamic Task Generator..."));
    
    // Load task templates
    LoadTaskTemplates();
    
    UE_LOG(LogTemp, Log, TEXT("Dynamic Task Generator initialized with %d task templates"), TaskTemplates.Num());
}

FDynamicTask UMingDynamicTaskGenerator::GeneratePersonalizedTask(const FString& PlayerID, const FPlayerBehaviorProfile& Profile)
{
    // Update player profile
    UpdatePlayerProfile(PlayerID, Profile);
    
    // Determine preferred task type based on player preferences
    ETaskType PreferredType = ETaskType::Combat;
    float MaxPreference = 0.0f;
    
    if (Profile.CombatPreference > MaxPreference)
    {
        MaxPreference = Profile.CombatPreference;
        PreferredType = ETaskType::Combat;
    }
    if (Profile.ExplorationPreference > MaxPreference)
    {
        MaxPreference = Profile.ExplorationPreference;
        PreferredType = ETaskType::Exploration;
    }
    if (Profile.ConstructionPreference > MaxPreference)
    {
        MaxPreference = Profile.ConstructionPreference;
        PreferredType = ETaskType::Construction;
    }
    if (Profile.DiplomacyPreference > MaxPreference)
    {
        MaxPreference = Profile.DiplomacyPreference;
        PreferredType = ETaskType::Diplomacy;
    }
    if (Profile.ResourceManagementPreference > MaxPreference)
    {
        MaxPreference = Profile.ResourceManagementPreference;
        PreferredType = ETaskType::Resource;
    }
    if (Profile.StoryPreference > MaxPreference)
    {
        MaxPreference = Profile.StoryPreference;
        PreferredType = ETaskType::Story;
    }
    
    // Create task from template
    FDynamicTask NewTask = CreateTaskFromTemplate(PreferredType, Profile);
    
    // Set personalization flags
    NewTask.bIsPersonalized = true;
    NewTask.AdaptationScore = CalculateTaskScore(NewTask, Profile);
    
    // Generate unique task ID
    NewTask.TaskID = FString::Printf(TEXT("TASK_%s_%lld"), *PlayerID, FDateTime::Now().GetTicks());
    
    UE_LOG(LogTemp, Log, TEXT("Generated personalized task %s for player %s"), *NewTask.TaskID, *PlayerID);
    
    return NewTask;
}

FDynamicTask UMingDynamicTaskGenerator::GenerateContextualTask(const FString& Context, ETaskType PreferredType)
{
    FDynamicTask ContextualTask;
    
    // Create a basic task based on context
    ContextualTask.TaskID = FString::Printf(TEXT("CONTEXT_%s_%lld"), *Context, FDateTime::Now().GetTicks());
    ContextualTask.Type = PreferredType;
    ContextualTask.Difficulty = ETaskDifficulty::Normal;
    ContextualTask.bIsPersonalized = false;
    ContextualTask.AdaptationScore = 0.5f;
    
    // Set contextual title and description
    if (Context.Contains("battle") || Context.Contains("combat"))
    {
        ContextualTask.Title = TEXT("Emergency Combat Mission");
        ContextualTask.Description = TEXT("Enemy forces detected in the area. Engage and eliminate the threat.");
        ContextualTask.Type = ETaskType::Combat;
        ContextualTask.RewardXP = 150;
    }
    else if (Context.Contains("explore") || Context.Contains("scout"))
    {
        ContextualTask.Title = TEXT("Reconnaissance Mission");
        ContextualTask.Description = TEXT("Scout the designated area and gather intelligence on enemy positions.");
        ContextualTask.Type = ETaskType::Exploration;
        ContextualTask.RewardXP = 100;
    }
    else if (Context.Contains("build") || Context.Contains("construct"))
    {
        ContextualTask.Title = TEXT("Construction Project");
        ContextualTask.Description = TEXT("Construct essential buildings to strengthen our position.");
        ContextualTask.Type = ETaskType::Construction;
        ContextualTask.RewardXP = 120;
    }
    else
    {
        ContextualTask.Title = TEXT("General Mission");
        ContextualTask.Description = TEXT("Complete the assigned objectives to advance our cause.");
        ContextualTask.RewardXP = 100;
    }
    
    // Set basic requirements
    ContextualTask.Requirements.MinLevel = 1;
    ContextualTask.Requirements.RequiredResources = 50;
    ContextualTask.Requirements.TimeLimit = 30.0f; // 30 minutes
    
    // Add basic objectives
    ContextualTask.Objectives.Add(TEXT("Complete primary objective"));
    ContextualTask.Objectives.Add(TEXT("Return to base"));
    
    UE_LOG(LogTemp, Log, TEXT("Generated contextual task %s for context: %s"), *ContextualTask.TaskID, *Context);
    
    return ContextualTask;
}

void UMingDynamicTaskGenerator::UpdatePlayerProfile(const FString& PlayerID, const FPlayerBehaviorProfile& NewProfile)
{
    PlayerProfiles.Add(PlayerID, NewProfile);
    SavePlayerProfile(PlayerID);
    
    UE_LOG(LogTemp, Log, TEXT("Updated profile for player %s"), *PlayerID);
}

ETaskDifficulty UMingDynamicTaskGenerator::GetAdaptiveDifficulty(const FString& PlayerID)
{
    if (!PlayerProfiles.Contains(PlayerID))
    {
        return ETaskDifficulty::Normal;
    }
    
    const FPlayerBehaviorProfile& Profile = PlayerProfiles[PlayerID];
    return CalculateOptimalDifficulty(Profile);
}

void UMingDynamicTaskGenerator::AnalyzeTaskCompletion(const FString& PlayerID, const FDynamicTask& CompletedTask, float CompletionTime)
{
    if (!PlayerProfiles.Contains(PlayerID))
    {
        return;
    }
    
    FPlayerBehaviorProfile& Profile = PlayerProfiles[PlayerID];
    
    // Update skill level based on task performance
    float ExpectedTime = Profile.AverageCompletionTime;
    if (ExpectedTime > 0.0f)
    {
        float PerformanceRatio = ExpectedTime / CompletionTime;
        if (PerformanceRatio > 1.2f)
        {
            Profile.PlayerSkillLevel = FMath::Min(Profile.PlayerSkillLevel + 1, 100);
        }
        else if (PerformanceRatio < 0.8f)
        {
            Profile.PlayerSkillLevel = FMath::Max(Profile.PlayerSkillLevel - 1, 1);
        }
    }
    
    // Update average completion time
    Profile.AverageCompletionTime = (Profile.AverageCompletionTime + CompletionTime) / 2.0f;
    
    // Update task type preferences based on completion
    switch (CompletedTask.Type)
    {
        case ETaskType::Combat:
            Profile.CombatPreference = FMath::Min(Profile.CombatPreference + 0.1f, 1.0f);
            break;
        case ETaskType::Exploration:
            Profile.ExplorationPreference = FMath::Min(Profile.ExplorationPreference + 0.1f, 1.0f);
            break;
        case ETaskType::Construction:
            Profile.ConstructionPreference = FMath::Min(Profile.ConstructionPreference + 0.1f, 1.0f);
            break;
        case ETaskType::Diplomacy:
            Profile.DiplomacyPreference = FMath::Min(Profile.DiplomacyPreference + 0.1f, 1.0f);
            break;
        case ETaskType::Resource:
            Profile.ResourceManagementPreference = FMath::Min(Profile.ResourceManagementPreference + 0.1f, 1.0f);
            break;
        case ETaskType::Story:
            Profile.StoryPreference = FMath::Min(Profile.StoryPreference + 0.1f, 1.0f);
            break;
    }
    
    SavePlayerProfile(PlayerID);
    
    UE_LOG(LogTemp, Log, TEXT("Analyzed task completion for player %s. New skill level: %d"), *PlayerID, Profile.PlayerSkillLevel);
}

TArray<FDynamicTask> UMingDynamicTaskGenerator::GetRecommendedTasks(const FString& PlayerID, int32 Count)
{
    TArray<FDynamicTask> RecommendedTasks;
    
    if (!PlayerProfiles.Contains(PlayerID))
    {
        return RecommendedTasks;
    }
    
    const FPlayerBehaviorProfile& Profile = PlayerProfiles[PlayerID];
    
    // Generate tasks based on player preferences
    for (int32 i = 0; i < Count && i < TaskTemplates.Num(); ++i)
    {
        FDynamicTask Task = CreateTaskFromTemplate(TaskTemplates[i].Type, Profile);
        Task.TaskID = FString::Printf(TEXT("RECOMMEND_%s_%d"), *PlayerID, i);
        Task.AdaptationScore = CalculateTaskScore(Task, Profile);
        
        RecommendedTasks.Add(Task);
    }
    
    // Sort by adaptation score
    RecommendedTasks.Sort([](const FDynamicTask& A, const FDynamicTask& B)
    {
        return A.AdaptationScore > B.AdaptationScore;
    });
    
    UE_LOG(LogTemp, Log, TEXT("Generated %d recommended tasks for player %s"), RecommendedTasks.Num(), *PlayerID);
    
    return RecommendedTasks;
}

FDynamicTask UMingDynamicTaskGenerator::CreateTaskFromTemplate(ETaskType Type, const FPlayerBehaviorProfile& Profile)
{
    FDynamicTask Task;
    
    // Find matching template
    FDynamicTask* Template = TaskTemplates.FindByPredicate([Type](const FDynamicTask& T)
    {
        return T.Type == Type;
    });
    
    if (Template)
    {
        Task = *Template;
    }
    else
    {
        // Create default task if no template found
        Task.Type = Type;
        Task.Title = TEXT("Generated Task");
        Task.Description = TEXT("Auto-generated task based on player behavior");
        Task.Difficulty = CalculateOptimalDifficulty(Profile);
        Task.RewardXP = 100;
        Task.Requirements.MinLevel = 1;
        Task.Requirements.RequiredResources = 50;
        Task.Objectives.Add(TEXT("Complete primary objective"));
    }
    
    // Personalize based on profile
    Task.Requirements.MinLevel = FMath::Max(1, Profile.PlayerSkillLevel - 2);
    Task.Requirements.RequiredResources = FMath::RoundToInt(50.0f * (1.0f + Profile.PlayerSkillLevel / 100.0f));
    
    return Task;
}

float UMingDynamicTaskGenerator::CalculateTaskScore(const FDynamicTask& Task, const FPlayerBehaviorProfile& Profile)
{
    float Score = 0.0f;
    
    // Calculate score based on task type preference
    switch (Task.Type)
    {
        case ETaskType::Combat:
            Score += Profile.CombatPreference;
            break;
        case ETaskType::Exploration:
            Score += Profile.ExplorationPreference;
            break;
        case ETaskType::Construction:
            Score += Profile.ConstructionPreference;
            break;
        case ETaskType::Diplomacy:
            Score += Profile.DiplomacyPreference;
            break;
        case ETaskType::Resource:
            Score += Profile.ResourceManagementPreference;
            break;
        case ETaskType::Story:
            Score += Profile.StoryPreference;
            break;
    }
    
    // Adjust for difficulty
    float DifficultyScore = 1.0f;
    switch (Task.Difficulty)
    {
        case ETaskDifficulty::VeryEasy:
            DifficultyScore = Profile.PlayerSkillLevel < 20 ? 1.0f : 0.3f;
            break;
        case ETaskDifficulty::Easy:
            DifficultyScore = Profile.PlayerSkillLevel < 40 ? 1.0f : 0.5f;
            break;
        case ETaskDifficulty::Normal:
            DifficultyScore = Profile.PlayerSkillLevel >= 20 && Profile.PlayerSkillLevel <= 80 ? 1.0f : 0.7f;
            break;
        case ETaskDifficulty::Hard:
            DifficultyScore = Profile.PlayerSkillLevel > 60 ? 1.0f : 0.4f;
            break;
        case ETaskDifficulty::VeryHard:
            DifficultyScore = Profile.PlayerSkillLevel > 80 ? 1.0f : 0.2f;
            break;
        case ETaskDifficulty::Adaptive:
            DifficultyScore = 1.0f;
            break;
    }
    
    Score *= DifficultyScore;
    
    return Score;
}

ETaskDifficulty UMingDynamicTaskGenerator::CalculateOptimalDifficulty(const FPlayerBehaviorProfile& Profile)
{
    int32 SkillLevel = Profile.PlayerSkillLevel;
    
    if (SkillLevel < 20)
    {
        return ETaskDifficulty::VeryEasy;
    }
    else if (SkillLevel < 40)
    {
        return ETaskDifficulty::Easy;
    }
    else if (SkillLevel < 60)
    {
        return ETaskDifficulty::Normal;
    }
    else if (SkillLevel < 80)
    {
        return ETaskDifficulty::Hard;
    }
    else
    {
        return ETaskDifficulty::VeryHard;
    }
}

void UMingDynamicTaskGenerator::LoadTaskTemplates()
{
    // Initialize with default task templates
    TaskTemplates.Empty();
    
    // Combat task template
    FDynamicTask CombatTask;
    CombatTask.Type = ETaskType::Combat;
    CombatTask.Title = TEXT("Combat Engagement");
    CombatTask.Description = TEXT("Engage enemy forces and secure the objective");
    CombatTask.Difficulty = ETaskDifficulty::Normal;
    CombatTask.RewardXP = 150;
    CombatTask.Requirements.MinLevel = 5;
    CombatTask.Requirements.RequiredResources = 100;
    CombatTask.Objectives.Add(TEXT("Eliminate enemy units"));
    CombatTask.Objectives.Add(TEXT("Secure the position"));
    CombatTask.RewardItems.Add(TEXT("Combat Experience"));
    TaskTemplates.Add(CombatTask);
    
    // Exploration task template
    FDynamicTask ExplorationTask;
    ExplorationTask.Type = ETaskType::Exploration;
    ExplorationTask.Title = TEXT("Area Reconnaissance");
    ExplorationTask.Description = TEXT("Explore the designated area and gather intelligence");
    ExplorationTask.Difficulty = ETaskDifficulty::Easy;
    ExplorationTask.RewardXP = 100;
    ExplorationTask.Requirements.MinLevel = 1;
    ExplorationTask.Requirements.RequiredResources = 50;
    ExplorationTask.Objectives.Add(TEXT("Scout the area"));
    ExplorationTask.Objectives.Add(TEXT("Report findings"));
    ExplorationTask.RewardItems.Add(TEXT("Reconnaissance Report"));
    TaskTemplates.Add(ExplorationTask);
    
    // Construction task template
    FDynamicTask ConstructionTask;
    ConstructionTask.Type = ETaskType::Construction;
    ConstructionTask.Title = TEXT("Base Development");
    ConstructionTask.Description = TEXT("Construct buildings to strengthen our position");
    ConstructionTask.Difficulty = ETaskDifficulty::Normal;
    ConstructionTask.RewardXP = 120;
    ConstructionTask.Requirements.MinLevel = 3;
    ConstructionTask.Requirements.RequiredResources = 200;
    ConstructionTask.Objectives.Add(TEXT("Build defensive structures"));
    ConstructionTask.Objectives.Add(TEXT("Establish supply lines"));
    ConstructionTask.RewardItems.Add(TEXT("Construction Materials"));
    TaskTemplates.Add(ConstructionTask);
    
    // Diplomacy task template
    FDynamicTask DiplomacyTask;
    DiplomacyTask.Type = ETaskType::Diplomacy;
    DiplomacyTask.Title = TEXT("Diplomatic Mission");
    DiplomacyTask.Description = TEXT("Negotiate with local factions to secure alliances");
    DiplomacyTask.Difficulty = ETaskDifficulty::Hard;
    DiplomacyTask.RewardXP = 200;
    DiplomacyTask.Requirements.MinLevel = 10;
    DiplomacyTask.Requirements.RequiredResources = 150;
    DiplomacyTask.Objectives.Add(TEXT("Meet with faction leaders"));
    DiplomacyTask.Objectives.Add(TEXT("Negotiate terms"));
    DiplomacyTask.RewardItems.Add(TEXT("Alliance Treaty"));
    TaskTemplates.Add(DiplomacyTask);
    
    // Resource task template
    FDynamicTask ResourceTask;
    ResourceTask.Type = ETaskType::Resource;
    ResourceTask.Title = TEXT("Resource Acquisition");
    ResourceTask.Description = TEXT("Gather resources to support our operations");
    ResourceTask.Difficulty = ETaskDifficulty::Easy;
    ResourceTask.RewardXP = 80;
    ResourceTask.Requirements.MinLevel = 1;
    ResourceTask.Requirements.RequiredResources = 25;
    ResourceTask.Objectives.Add(TEXT("Collect resources"));
    ResourceTask.Objectives.Add(TEXT("Return to base"));
    ResourceTask.RewardItems.Add(TEXT("Resource Cache"));
    TaskTemplates.Add(ResourceTask);
    
    // Story task template
    FDynamicTask StoryTask;
    StoryTask.Type = ETaskType::Story;
    StoryTask.Title = TEXT("Story Mission");
    StoryTask.Description = TEXT("Complete a story-driven objective");
    StoryTask.Difficulty = ETaskDifficulty::Normal;
    StoryTask.RewardXP = 180;
    StoryTask.Requirements.MinLevel = 5;
    StoryTask.Requirements.RequiredResources = 75;
    StoryTask.Objectives.Add(TEXT("Follow the story"));
    StoryTask.Objectives.Add(TEXT("Make key decisions"));
    StoryTask.RewardItems.Add(TEXT("Story Progress"));
    TaskTemplates.Add(StoryTask);
}

void UMingDynamicTaskGenerator::SavePlayerProfile(const FString& PlayerID)
{
    // In a real implementation, this would save to a file or database
    // For now, we'll just log the save action
    UE_LOG(LogTemp, Log, TEXT("Saved profile for player %s"), *PlayerID);
}
