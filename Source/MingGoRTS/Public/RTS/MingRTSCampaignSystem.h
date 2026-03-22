#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCampaignSystem.generated.h"

UENUM(BlueprintType)
enum class ERTSCampaignType: uint8 {
    MainStory      UMETA(DisplayName = "Main Story"),
    SideQuest      UMETA(DisplayName = "Side Quest"),
    Tutorial       UMETA(DisplayName = "Tutorial"),
    Challenge      UMETA(DisplayName = "Challenge"),
    Survival       UMETA(DisplayName = "Survival"),
    Sandbox        UMETA(DisplayName = "Sandbox"),
    Multiplayer    UMETA(DisplayName = "Multiplayer"),
    Custom         UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class ERTSCampaignDifficulty: uint8 {
    Easy       UMETA(DisplayName = "Easy"),
    Normal     UMETA(DisplayName = "Normal"),
    Hard       UMETA(DisplayName = "Hard"),
    Expert     UMETA(DisplayName = "Expert"),
    Nightmare  UMETA(DisplayName = "Nightmare"),
    Custom     UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class ERTSCampaignState: uint8 {
    NotStarted  UMETA(DisplayName = "Not Started"),
    InProgress  UMETA(DisplayName = "In Progress"),
    Paused      UMETA(DisplayName = "Paused"),
    Completed   UMETA(DisplayName = "Completed"),
    Failed      UMETA(DisplayName = "Failed"),
    Abandoned   UMETA(DisplayName = "Abandoned")
};

UENUM(BlueprintType)
enum class ERTSObjectiveType: uint8 {
    Destroy     UMETA(DisplayName = "Destroy"),
    Capture     UMETA(DisplayName = "Capture"),
    Defend      UMETA(DisplayName = "Defend"),
    Escort      UMETA(DisplayName = "Escort"),
    Survive     UMETA(DisplayName = "Survive"),
    Collect     UMETA(DisplayName = "Collect"),
    Explore     UMETA(DisplayName = "Explore"),
    Rescue      UMETA(DisplayName = "Rescue"),
    Build       UMETA(DisplayName = "Build"),
    Research    UMETA(DisplayName = "Research")
};

USTRUCT(BlueprintType)
struct FRTSCampaignObjective
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ObjectiveID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ObjectiveName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ERTSObjectiveType ObjectiveType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Progress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bOptional;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Prerequisites;

    FRTSCampaignObjective()
        : ObjectiveType(ERTSObjectiveType::Destroy)
        , Progress(0.0f)
        , bCompleted(false)
        , bOptional(false)
    {}
};

USTRUCT(BlueprintType)
struct FRTSCampaignChapter
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ChapterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ChapterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChapterNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FRTSCampaignObjective> Objectives;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UnlockRequirements;

    FRTSCampaignChapter()
        : ChapterNumber(0)
        , bCompleted(false)
    {}
};

USTRUCT(BlueprintType)
struct FRTSStoryEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText EventName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TriggerCondition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bTriggered;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Consequences;

    FRTSStoryEvent()
        : bTriggered(false)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCampaignStarted, FString, CampaignID, ERTSCampaignType, CampaignType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCampaignCompleted, FString, CampaignID, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObjectiveUpdated, FString, ObjectiveID, float, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChapterUnlocked, FString, ChapterID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStoryEventTriggered, FString, EventID);

UCLASS(ClassGroup=(RTS), meta=(BlueprintSpawnableComponent))
class MINGRTS_API UMingRTSCampaignSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCampaignSystem();

    UFUNCTION(BlueprintCallable, Category = "RTS Campaign")
    void InitializeCampaignSystem();

    UFUNCTION(BlueprintCallable, Category = "RTS Campaign")
    void StartCampaign(const FString& CampaignID, ERTSCampaignType CampaignType, ERTSCampaignDifficulty Difficulty);

    UFUNCTION(BlueprintCallable, Category = "RTS Campaign")
    void CompleteCampaign(const FString& CampaignID, bool bSuccess);

    UFUNCTION(BlueprintCallable, Category = "RTS Campaign")
    void PauseCampaign(const FString& CampaignID);

    UFUNCTION(BlueprintCallable, Category = "RTS Campaign")
    void ResumeCampaign(const FString& CampaignID);

    UFUNCTION(BlueprintCallable, Category = "RTS Campaign")
    void AbandonCampaign(const FString& CampaignID);

    UFUNCTION(BlueprintCallable, Category = "RTS Campaign")
    void RegisterCampaignChapter(const FString& CampaignID, const FRTSCampaignChapter& Chapter);

    UFUNCTION(BlueprintCallable, Category = "RTS Campaign")
    void UpdateObjectiveProgress(const FString& ObjectiveID, float NewProgress);

    UFUNCTION(BlueprintCallable, Category = "RTS Campaign")
    void CompleteObjective(const FString& ObjectiveID);

    UFUNCTION(BlueprintCallable, Category = "RTS Campaign")
    void TriggerStoryEvent(const FString& EventID);

    UFUNCTION(BlueprintCallable, Category = "RTS Campaign")
    void RegisterStoryEvent(const FRTSStoryEvent& StoryEvent);

    UFUNCTION(BlueprintCallable, Category = "RTS Campaign")
    FRTSCampaignObjective GetObjective(const FString& ObjectiveID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Campaign")
    TArray<FRTSCampaignObjective> GetActiveObjectives(const FString& CampaignID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Campaign")
    float GetCampaignProgress(const FString& CampaignID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Campaign")
    ERTSCampaignState GetCampaignState(const FString& CampaignID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Campaign")
    TArray<FRTSCampaignChapter> GetCampaignChapters(const FString& CampaignID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Campaign")
    void SetCampaignDifficulty(ERTSCampaignDifficulty NewDifficulty);

    UFUNCTION(BlueprintPure, Category = "RTS Campaign")
    ERTSCampaignDifficulty GetCampaignDifficulty() const { return CurrentDifficulty; }

    UPROPERTY(BlueprintAssignable, Category = "RTS Campaign Events")
    FOnCampaignStarted OnCampaignStarted;

    UPROPERTY(BlueprintAssignable, Category = "RTS Campaign Events")
    FOnCampaignCompleted OnCampaignCompleted;

    UPROPERTY(BlueprintAssignable, Category = "RTS Campaign Events")
    FOnObjectiveUpdated OnObjectiveUpdated;

    UPROPERTY(BlueprintAssignable, Category = "RTS Campaign Events")
    FOnChapterUnlocked OnChapterUnlocked;

    UPROPERTY(BlueprintAssignable, Category = "RTS Campaign Events")
    FOnStoryEventTriggered OnStoryEventTriggered;

protected:
    UPROPERTY()
    TMap<FString, FRTSCampaignChapter> CampaignChapters;

    UPROPERTY()
    TMap<FString, FRTSCampaignObjective> Objectives;

    UPROPERTY()
    TMap<FString, FRTSStoryEvent> StoryEvents;

    UPROPERTY()
    TMap<FString, ERTSCampaignState> CampaignStates;

    UPROPERTY()
    ERTSCampaignDifficulty CurrentDifficulty;

    UPROPERTY()
    FString ActiveCampaignID;

    UFUNCTION()
    void CheckChapterCompletion(const FString& ChapterID);

    UFUNCTION()
    void UnlockNextChapter(const FString& CurrentChapterID);

    UFUNCTION()
    void ProcessStoryEventConsequences(const FString& EventID);

    UFUNCTION()
    bool ArePrerequisitesMet(const TArray<FString>& Prerequisites) const;
};
