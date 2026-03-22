#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MingDynamicTaskGenerator.generated.h"

UENUM(BlueprintType)
enum class ETaskType: uint8 {
    Combat,
    Exploration,
    Construction,
    Diplomacy,
    Resource,
    Story,
    Custom
};

UENUM(BlueprintType)
enum class ETaskDifficulty: uint8 {
    VeryEasy,
    Easy,
    Normal,
    Hard,
    VeryHard,
    Adaptive
};

USTRUCT(BlueprintType)
struct FTaskRequirement
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RequiredResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> PrerequisiteTasks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeLimit;
};

USTRUCT(BlueprintType)
struct FDynamicTask
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TaskID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETaskType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETaskDifficulty Difficulty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTaskRequirement Requirements;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Objectives;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RewardXP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RewardItems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPersonalized;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AdaptationScore;
};

USTRUCT(BlueprintType)
struct FPlayerBehaviorProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CombatPreference;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExplorationPreference;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConstructionPreference;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DiplomacyPreference;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResourceManagementPreference;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StoryPreference;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PlayerSkillLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AverageCompletionTime;
};

UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingDynamicTaskGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingDynamicTaskGenerator();

    // Initialize the task generator
    UFUNCTION(BlueprintCallable, Category = "Dynamic Task")
    void InitializeTaskGenerator();

    // Generate personalized task based on player behavior
    UFUNCTION(BlueprintCallable, Category = "Dynamic Task")
    FDynamicTask GeneratePersonalizedTask(const FString& PlayerID, const FPlayerBehaviorProfile& Profile);

    // Generate task for specific context
    UFUNCTION(BlueprintCallable, Category = "Dynamic Task")
    FDynamicTask GenerateContextualTask(const FString& Context, ETaskType PreferredType);

    // Update player behavior profile
    UFUNCTION(BlueprintCallable, Category = "Dynamic Task")
    void UpdatePlayerProfile(const FString& PlayerID, const FPlayerBehaviorProfile& NewProfile);

    // Get adaptive difficulty
    UFUNCTION(BlueprintCallable, Category = "Dynamic Task")
    ETaskDifficulty GetAdaptiveDifficulty(const FString& PlayerID);

    // Analyze task completion patterns
    UFUNCTION(BlueprintCallable, Category = "Dynamic Task")
    void AnalyzeTaskCompletion(const FString& PlayerID, const FDynamicTask& CompletedTask, float CompletionTime);

    // Get recommended tasks
    UFUNCTION(BlueprintCallable, Category = "Dynamic Task")
    TArray<FDynamicTask> GetRecommendedTasks(const FString& PlayerID, int32 Count = 3);

protected:
    // Player behavior profiles
    UPROPERTY(BlueprintReadOnly, Category = "Dynamic Task")
    TMap<FString, FPlayerBehaviorProfile> PlayerProfiles;

    // Task templates
    UPROPERTY(BlueprintReadOnly, Category = "Dynamic Task")
    TArray<FDynamicTask> TaskTemplates;

    // Task generation parameters
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Task")
    float PersonalizationWeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Task")
    float AdaptationThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Task")
    int32 MaxConcurrentTasks;

private:
    // Internal helper functions
    FDynamicTask CreateTaskFromTemplate(ETaskType Type, const FPlayerBehaviorProfile& Profile);
    float CalculateTaskScore(const FDynamicTask& Task, const FPlayerBehaviorProfile& Profile);
    ETaskDifficulty CalculateOptimalDifficulty(const FPlayerBehaviorProfile& Profile);
    void LoadTaskTemplates();
    void SavePlayerProfile(const FString& PlayerID);
};
