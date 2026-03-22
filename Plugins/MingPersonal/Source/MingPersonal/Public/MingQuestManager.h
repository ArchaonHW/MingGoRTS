#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCharacterGrowthSystem.h"
#include "MingItemManager.h"
#include "MingQuestManager.generated.h"

// ‰ªªÂ?È°ûÂ?
UENUM(BlueprintType)
enum class EMingQuestType : uint8
{
    Main,              // ‰∏ªÁ?‰ªªÂ?
    Side,              // ?ØÁ?‰ªªÂ?
    Daily,              // ÊØèÊó•‰ªªÂ?
    Weekly,             // ÊØèÂë®‰ªªÂ?
    Event,              // Ê¥ªÂ?‰ªªÂ?
    Hidden,             // ?±Ë?‰ªªÂ?
    Tutorial,           // ?ôÁ?‰ªªÂ?
    Achievement         // ?êÂ∞±‰ªªÂ?
};

// ‰ªªÂ??Ä??UENUM(BlueprintType)
enum class EMingQuestStatus : uint8
{
    Available,          // ?ØÊé•??    Active,             // ?≤Ë?‰∏?    Completed,          // Â∑≤Â???    Failed,             // Â§±Ê?
    Abandoned,          // ?æÊ?
    Locked              // ?ñÂ?
};

// ‰ªªÂ??ÆÊ?È°ûÂ?
UENUM(BlueprintType)
enum class EMingQuestObjectiveType : uint8
{
    Kill,               // ?äÊÆ∫
    Collect,            // ?∂È?
    Deliver,            // ?ãÈÄ?    Escort,             // Ë≠∑ÈÄ?    Defend,             // ?≤Á¶¶
    Explore,            // ?¢Á¥¢
    Talk,               // Â∞çË©±
    Interact,           // ‰∫íÂ?
    Survive,            // ?üÂ?
    Capture,            // ‰ΩîÈ?
    Destroy             // ?ßÊ?
};

// ?Ö‰??ÜÊîØÈ°ûÂ?
UENUM(BlueprintType)
enum class EMingStoryBranch : uint8
{
    Linear,             // Á∑öÊÄ?    Choice,             // ?∏Ê??ÜÊîØ
    Conditional,        // Ê¢ù‰ª∂?ÜÊîØ
    Random,             // ?®Ê??ÜÊîØ
    Moral,              // ?ìÂæ∑?∏Ê?
    Strategic,          // ?∞Áï•?∏Ê?
    Personal            // ?ã‰∫∫?∏Ê?
};

// ‰ªªÂ??ÆÊ?
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingQuestObjective
{
    GENERATED_BODY()

    // ?ÆÊ?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ObjectiveID;

    // ?ÆÊ??èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ObjectiveDescription;

    // ?ÆÊ?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingQuestObjectiveType ObjectiveType;

    // ?ÆÊ??∏È?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetCount;

    // ?∂Â??≤Â∫¶
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentProgress;

    // ?ÆÊ??ÉÊï∏
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> ObjectiveParameters;

    // ?ØÂê¶?ØË?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsVisible;

    // ?ØÂê¶ÂÆåÊ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCompleted;

    // ?ØÂê¶?ØÈÅ∏??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsOptional;

    // ?ÆÊ?‰ΩçÁΩÆ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;

    // ?ÆÊ?NPC
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetNPC;

    // ?ÇÈ??êÂà∂
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeLimit;

    // Â§±Ê?Ê¢ù‰ª∂
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> FailureConditions;

    FMingQuestObjective()
    {
        ObjectiveID = -1;
        ObjectiveDescription = TEXT(""};
        ObjectiveType = EMingQuestObjectiveType::Kill;
        TargetCount = 0;
        CurrentProgress = 0;
        ObjectiveParameters.Empty(};
        bIsVisible = true;
        bIsCompleted = false;
        bIsOptional = false;
        TargetLocation = FVector::ZeroVector;
        TargetNPC = TEXT(""};
        TimeLimit = 0.0f;
        FailureConditions.Empty(};
    }
};

// ?Ö‰??∏È?
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingStoryChoice
{
    GENERATED_BODY()

    // ?∏È?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChoiceID;

    // ?∏È??áÊú¨
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ChoiceText;

    // ?∏È??èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ChoiceDescription;

    // ?∏È?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingStoryBranch ChoiceType;

    // ?∏È?Ê¢ù‰ª∂
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ChoiceConditions;

    // ?∏È?ÁµêÊ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> ChoiceResults;

    // ?ìÂæ∑ÂΩ±Èüø
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoralImpact;

    // ?≤Ê?ÂΩ±Èüø
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReputationImpact;

    // ?ú‰?ÂΩ±Èüø
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> RelationshipImpact;

    // ?ØÂê¶Â∑≤ÈÅ∏??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsChosen;

    // ?∏È?Ê¨äÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ChoiceWeight;

    FMingStoryChoice()
    {
        ChoiceID = -1;
        ChoiceText = TEXT(""};
        ChoiceDescription = TEXT(""};
        ChoiceType = EMingStoryBranch::Choice;
        ChoiceConditions.Empty(};
        ChoiceResults.Empty(};
        MoralImpact = 0.0f;
        ReputationImpact = 0.0f;
        RelationshipImpact.Empty(};
        bIsChosen = false;
        ChoiceWeight = 1.0f;
    }
};

// ‰ªªÂ??∏Ê?
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingQuestData
{
    GENERATED_BODY()

    // ‰ªªÂ?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 QuestID;

    // ‰ªªÂ??çÁ®±
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestName;

    // ‰ªªÂ??èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestDescription;

    // ‰ªªÂ?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingQuestType QuestType;

    // ‰ªªÂ??Ä??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingQuestStatus QuestStatus;

    // ‰ªªÂ?Á≠âÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 QuestLevel;

    // Âª∫Ë≠∞Á≠âÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SuggestedLevel;

    // ‰ªªÂ??ÆÊ??óË"
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingQuestObjective> QuestObjectives;

    // ?Ö‰??∏È??óË"
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingStoryChoice> StoryChoices;

    // ‰ªªÂ??éÂãµ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> QuestRewards;

    // Á∂ìÈ??éÂãµ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExperienceReward;

    // ?≤Ê??éÂãµ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReputationReward;

    // ?©Â??éÂãµ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingItemData> ItemRewards;

    // ?çÁΩÆ‰ªªÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> PrerequisiteQuests;

    // Ëß??Ê¢ù‰ª∂
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UnlockConditions;

    // ‰ªªÂ??ãÂ??ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime StartTime;

    // ‰ªªÂ??™Ê≠¢?ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime DeadlineTime;

    // ‰ªªÂ?‰ΩçÁΩÆ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector QuestLocation;

    // ‰ªªÂ?NPC
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestGiver;

    // ‰ªªÂ??ñÊ?Ë∑ØÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestIconPath;

    // ‰ªªÂ?Ê®ôÁ±§
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> QuestTags;

    // ?Ö‰??çË???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StoryImportance;

    // ?çË?Ê¨°Êï∏
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RepeatCount;

    // ?ÄÂ§ßÈ?Ë§áÊ¨°??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxRepeats;

    FMingQuestData()
    {
        QuestID = -1;
        QuestName = TEXT(""};
        QuestDescription = TEXT(""};
        QuestType = EMingQuestType::Main;
        QuestStatus = EMingQuestStatus::Locked;
        QuestLevel = 1;
        SuggestedLevel = 1;
        QuestObjectives.Empty(};
        StoryChoices.Empty(};
        QuestRewards.Empty(};
        ExperienceReward = 0.0f;
        ReputationReward = 0.0f;
        ItemRewards.Empty(};
        PrerequisiteQuests.Empty(};
        UnlockConditions.Empty(};
        StartTime = FDateTime::MinValue(};
        DeadlineTime = FDateTime::MaxValue(};
        QuestLocation = FVector::ZeroVector;
        QuestGiver = TEXT(""};
        QuestIconPath = TEXT(""};
        QuestTags.Empty(};
        StoryImportance = 1.0f;
        RepeatCount = 0;
        MaxRepeats = 0;
    }
};

// ?Ö‰?ÁØÄÈª?USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingStoryNode
{
    GENERATED_BODY()

    // ÁØÄÈªûID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NodeID;

    // ÁØÄÈªûÈ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NodeType;

    // ÁØÄÈªûÂÖßÂÆ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NodeContent;

    // ÁØÄÈªûÈÅ∏??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingStoryChoice> NodeChoices;

    // ÁØÄÈªûÊ?‰ª?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> NodeConditions;

    // ÁØÄÈªûÁ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> NodeResults;

    // ÁØÄÈªûÊ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float NodeWeight;

    // ?ØÂê¶Â∑≤Ë®™??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsVisited;

    // ÁØÄÈªû‰?ÁΩ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector NodePosition;

    FMingStoryNode()
    {
        NodeID = -1;
        NodeType = TEXT(""};
        NodeContent = TEXT(""};
        NodeChoices.Empty(};
        NodeConditions.Empty(};
        NodeResults.Empty(};
        NodeWeight = 1.0f;
        bIsVisited = false;
        NodePosition = FVector::ZeroVector;
    }
};

// ‰ªªÂ?ÁÆ°Á?‰∫ã‰ª∂ÂßîË?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestAccepted, const FMingQuestData&, Quest, const FString&, QuestGiver};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestCompleted, const FMingQuestData&, Quest, bool, bSuccess};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnQuestObjectiveUpdated, int32, QuestID, int32, ObjectiveID, int32, NewProgress};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStoryChoiceMade, int32, QuestID, int32, ChoiceID, const FMingStoryChoice&, Choice};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestFailed, const FMingQuestData&, Quest, const FString&, FailureReason};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestAbandoned, int32, QuestID, const FString&, Reason};

/**
 * ‰ªªÂ?ÁÆ°Á??? * Ë≤†Ë≤¨ÁÆ°Á?‰ªªÂ??åÊ?‰∫ãÁ≥ªÁµ? */
UCLASS(ClassGroup = (Personal), Blueprintable, BlueprintType)
class MINGPERSONAL_API UMingQuestManager : public UObject
{
    GENERATED_BODY()

public:
    UMingQuestManager(};

    // ?ùÂ??ñ‰ªª?ôÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool InitializeQuestSystem(};

    // ?•Â?‰ªªÂ?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool AcceptQuest(int32 QuestID, const FString& QuestGiver};

    // ?íÁ?‰ªªÂ?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool DeclineQuest(int32 QuestID};

    // ?æÊ?‰ªªÂ?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool AbandonQuest(int32 QuestID, const FString& Reason};

    // ÂÆåÊ?‰ªªÂ?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool CompleteQuest(int32 QuestID, bool bSuccess = true};

    // ?¥Êñ∞‰ªªÂ??ÆÊ?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool UpdateQuestObjective(int32 QuestID, int32 ObjectiveID, int32 Progress};

    // ?öÂá∫?Ö‰??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool MakeStoryChoice(int32 QuestID, int32 ChoiceID};

    // ?≤Â?‰ªªÂ??óË"
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingQuestData> GetAvailableQuests() const;

    // ?≤Â?Ê¥ªË?‰ªªÂ?
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingQuestData> GetActiveQuests() const;

    // ?≤Â?Â∑≤Â??ê‰ªª??    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingQuestData> GetCompletedQuests() const;

    // ?≤Â??πÂ?‰ªªÂ?
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    FMingQuestData GetQuest(int32 QuestID) const;

    // Ê™¢Êü•‰ªªÂ?Ê¢ù‰ª∂
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    bool CanAcceptQuest(int32 QuestID) const;

    // ?≤Â?‰ªªÂ??≤Â∫¶
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    float GetQuestProgress(int32 QuestID) const;

    // ?≤Â??Ö‰?ÁØÄÈª?    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingStoryNode> GetStoryNodes(int32 QuestID) const;

    // Ëß???∞‰ªª??    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    void UnlockNewQuests(};

    // ?çÁΩÆ‰ªªÂ?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool ResetQuest(int32 QuestID};

    // ?≤Â?‰ªªÂ?Áµ±Ë?
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TMap<EMingQuestType, int32> GetQuestStatistics() const;

    // ?≤Â??Ö‰??ÜÊîØ
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingStoryChoice> GetStoryChoices(int32 QuestID) const;

    // ‰øùÂ?‰ªªÂ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool SaveQuestData(};

    // ËºâÂÖ•‰ªªÂ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool LoadQuestData(};

    // Ê∏ÖÈô§?Ä?â‰ªª?ôÊï∏??    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    void ClearAllQuestData(};

    // ‰∫ã‰ª∂ÂßîË?
    UPROPERTY(BlueprintAssignable)
    FOnQuestAccepted OnQuestAccepted;

    UPROPERTY(BlueprintAssignable)
    FOnQuestCompleted OnQuestCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnQuestObjectiveUpdated OnQuestObjectiveUpdated;

    UPROPERTY(BlueprintAssignable)
    FOnStoryChoiceMade OnStoryChoiceMade;

    UPROPERTY(BlueprintAssignable)
    FOnQuestFailed OnQuestFailed;

    UPROPERTY(BlueprintAssignable)
    FOnQuestAbandoned OnQuestAbandoned;

protected:
    // ‰ªªÂ??∏Ê?Â∫?    UPROPERTY()
    TMap<int32, FMingQuestData> QuestDatabase;

    // ?Ö‰?ÁØÄÈªûÊï∏?öÂ∫´ - Ê≥®Ê?ÔºöTMap<TArray> ‰∏çÊîØ??UPROPERTY
    TMap<int32, TArray<FMingStoryNode>> StoryNodeDatabase;

    // Ê¥ªË?‰ªªÂ??óË"
    UPROPERTY()
    TArray<int32> ActiveQuests;

    // Â∑≤Â??ê‰ªª?ôÂ?Ë°?    UPROPERTY()
    TArray<int32> CompletedQuests;

    // ?Ö‰??∏Ê?Ê≠∑Âè≤ - Ê≥®Ê?ÔºöTMap<TArray> ‰∏çÊîØ??UPROPERTY
    TMap<int32, TArray<int32>> StoryChoiceHistory;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    UPROPERTY()
    bool bInitialized;

private:
    // ËºâÂÖ•?êË®≠‰ªªÂ?
    void LoadDefaultQuests(};

    // ËºâÂÖ•?êË®≠?Ö‰?ÁØÄÈª?    void LoadDefaultStoryNodes(};

    // È©óË?‰ªªÂ??∏Ê?
    bool ValidateQuestData(const FMingQuestData& Quest) const;

    // Ê™¢Êü•‰ªªÂ?ÂÆåÊ?Ê¢ù‰ª∂
    bool CheckQuestCompletion(int32 QuestID) const;

    // ?ïÁ?‰ªªÂ??éÂãµ
    void ProcessQuestRewards(const FMingQuestData& Quest};

    // ?¥Êñ∞?Ö‰??ÜÊîØ
    void UpdateStoryBranch(int32 QuestID, int32 ChoiceID};

    // Ë®àÁ?‰ªªÂ??çË???    float CalculateQuestImportance(const FMingQuestData& Quest) const;

    // ?üÊ??®Ê?‰ªªÂ?
    FMingQuestData GenerateRandomQuest(EMingQuestType QuestType) const;

    // ?≤Â??ØÊé•?ñ‰ªª??    TArray<int32> GetAvailableQuestIDs() const;

    // Ê™¢Êü•?çÁΩÆ‰ªªÂ?
    bool CheckPrerequisiteQuests(const FMingQuestData& Quest) const;

    // Ê™¢Êü•Ëß??Ê¢ù‰ª∂
    bool CheckUnlockConditions(const FMingQuestData& Quest) const;

    // ?¥Êñ∞‰ªªÂ??Ä??    void UpdateQuestStatus(int32 QuestID, EMingQuestStatus NewStatus};

    // ?ïÁ??Ö‰?ÂæåÊ?
    void ProcessStoryConsequences(int32 QuestID, const FMingStoryChoice& Choice};

    // Ë®àÁ??∏Ê?Ê¨äÈ?
    float CalculateChoiceWeight(const FMingStoryChoice& Choice) const;

    // ?≤Â??Ö‰?Á∑öÁ¥¢
    TArray<FString> GetStoryClues(int32 QuestID) const;

    // ?¥Êñ∞‰ªªÂ??•Ë?
    void UpdateQuestLog(int32 QuestID, const FString& LogEntry};

    // ‰øùÂ??Ö‰?Âø´ÁÖß
    void SaveStorySnapshot(};

    // ËºâÂÖ•?Ö‰?Âø´ÁÖß
    void LoadStorySnapshot(};

    // Ë®àÁ?‰ªªÂ???∫¶
    float CalculateQuestDifficulty(const FMingQuestData& Quest) const;

    // Ë™øÊï¥‰ªªÂ???∫¶
    void AdjustQuestDifficulty(int32 QuestID, float DifficultyModifier};

    // ?≤Â?‰ªªÂ?Âª∫Ë≠∞
    TArray<FString> GetQuestSuggestions(int32 QuestID) const;

    // ?ïÁ?‰ªªÂ?Â§±Ê?
    void HandleQuestFailure(int32 QuestID, const FString& FailureReason};

    // ?ïÁ?‰ªªÂ??æÊ?
    void HandleQuestAbandonment(int32 QuestID, const FString& Reason};

    // Ë®àÁ??Ö‰?ÂΩ±Èüø
    TMap<FString, float> CalculateStoryImpact(int32 QuestID, int32 ChoiceID) const;

    // ?¥Êñ∞‰∏ñÁ??Ä??    void UpdateWorldState(int32 QuestID, const TMap<FString, FString>& Results};

    // ?üÊ?‰ªªÂ?Ê®ôË?
    void GenerateQuestMarkers(int32 QuestID};

    // ÁßªÈô§‰ªªÂ?Ê®ôË?
    void RemoveQuestMarkers(int32 QuestID};

    // Ë®àÁ?‰ªªÂ??πÂÄ?    float CalculateQuestValue(const FMingQuestData& Quest) const;

    // ?≤Â?‰ªªÂ??ÇÈ?Á∑?    TArray<FDateTime> GetQuestTimeline(int32 QuestID) const;

    // Ê™¢Êü•‰ªªÂ?Ë°ùÁ?
    bool CheckQuestConflict(int32 QuestID1, int32 QuestID2) const;

    // Ëß?±∫‰ªªÂ?Ë°ùÁ?
    void ResolveQuestConflict(int32 QuestID1, int32 QuestID2};

    // ?≤Â?‰ªªÂ?‰æùË≥¥
    TArray<int32> GetQuestDependencies(int32 QuestID) const;

    // ?¥Êñ∞‰ªªÂ?‰æùË≥¥
    void UpdateQuestDependencies(int32 QuestID};

    // Ë®àÁ??Ö‰??ÜÊîØÊ¶ÇÁ?
    float CalculateStoryBranchProbability(const FMingStoryChoice& Choice) const;

    // ?®Ê??ñÊ?‰∫ãÈÅ∏??    void RandomizeStoryChoices(int32 QuestID};

    // ?ñÂ??Ö‰??ÜÊîØ
    void LockStoryBranch(int32 QuestID, int32 ChoiceID};

    // Ëß???Ö‰??ÜÊîØ
    void UnlockStoryBranch(int32 QuestID, int32 ChoiceID};

    // ?≤Â??Ö‰?Áµ±Ë?
    TMap<FString, int32> GetStoryStatistics() const;

    // Ë®àÁ??Ö‰?Ê∑±Â∫¶
    int32 CalculateStoryDepth(int32 QuestID) const;

    // ?≤Â??Ö‰??êË¶Ω
    FString GetStoryPreview(int32 QuestID) const;

    // ?ïÁ??ÇÈ??êÂà∂
    void ProcessTimeLimits(};

    // ?¥Êñ∞‰ªªÂ??™Ê≠¢?ÇÈ?
    void UpdateQuestDeadlines(};

    // Ê™¢Êü•‰ªªÂ??éÊ?
    void CheckQuestExpiration(};

    // ?ïÁ??éÊ?‰ªªÂ?
    void HandleExpiredQuest(int32 QuestID};

    // ?üÊ?‰ªªÂ??êÁ§∫
    TArray<FString> GenerateQuestHints(int32 QuestID) const;

    // Ë®àÁ?‰ªªÂ?ÂÆåÊ???    float CalculateQuestCompletionRate() const;

    // ?≤Â?‰ªªÂ??®Ëñ¶
    TArray<int32> GetQuestRecommendations() const;

    // ?¥Êñ∞‰ªªÂ??®Ëñ¶
    void UpdateQuestRecommendations(};

    // Ë®àÁ??Ö‰??ÜÊîØÂΩ±Èüø
    TMap<FString, float> CalculateBranchImpact(const FMingStoryChoice& Choice) const;
};

