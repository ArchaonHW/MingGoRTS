#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCharacterGrowthSystem.h"
#include "MingItemManager.h"
#include "MingQuestManager.generated.h"

// 任�?類�?
UENUM(BlueprintType)
enum class EMingQuestType : uint8
{
    Main,              // 主�?任�?
    Side,              // ?��?任�?
    Daily,              // 每日任�?
    Weekly,             // 每周任�?
    Event,              // 活�?任�?
    Hidden,             // ?��?任�?
    Tutorial,           // ?��?任�?
    Achievement         // ?�就任�?
};

// 任�X�XUENUM(BlueprintType)
enum class EMingQuestStatus : uint8
{
    Available,          // ?�接X    Active,             // ?��?�?    Completed,          // 已�X    Failed,             // 失�?
    Abandoned,          // ?��?
    Locked              // ?��?
};

// 任�X��?類�?
UENUM(BlueprintType)
enum class EMingQuestObjectiveType : uint8
{
    Kill,               // ?�殺
    Collect,            // ?��?
    Deliver,            // ?��?    Escort,             // 護�?    Defend,             // ?�禦
    Explore,            // ?�索
    Talk,               // 對話
    Interact,           // 互�?
    Survive,            // ?��?
    Capture,            // 佔�?
    Destroy             // ?��?
};

// ?��X�支類�?
UENUM(BlueprintType)
enum class EMingStoryBranch : uint8
{
    Linear,             // 線�?    Choice,             // ?��X�支
    Conditional,        // 條件?�支
    Random,             // ?��X�支
    Moral,              // ?�德?��?
    Strategic,          // ?�略?��?
    Personal            // ?�人?��?
};

// 任�X��?
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingQuestObjective
{
    GENERATED_BODY()

    // ?��?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ObjectiveID;

    // ?��X�述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ObjectiveDescription;

    // ?��?類�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingQuestObjectiveType ObjectiveType;

    // ?��X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetCount;

    // ?��X�度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentProgress;

    // ?��X�數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> ObjectiveParameters;

    // ?�否?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsVisible;

    // ?�否完�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCompleted;

    // ?�否?�選X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsOptional;

    // ?��?位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;

    // ?��?NPC
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetNPC;

    // ?��X�制
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeLimit;

    // 失�?條件
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

// ?��X��?
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingStoryChoice
{
    GENERATED_BODY()

    // ?��?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChoiceID;

    // ?��X�本
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ChoiceText;

    // ?��X�述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ChoiceDescription;

    // ?��?類�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingStoryBranch ChoiceType;

    // ?��?條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ChoiceConditions;

    // ?��?結�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> ChoiceResults;

    // ?�德影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoralImpact;

    // ?��?影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReputationImpact;

    // ?��?影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> RelationshipImpact;

    // ?�否已選X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsChosen;

    // ?��?權�?
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

// 任�X��?
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingQuestData
{
    GENERATED_BODY()

    // 任�?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 QuestID;

    // 任�X�稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestName;

    // 任�X�述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestDescription;

    // 任�?類�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingQuestType QuestType;

    // 任�X�X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingQuestStatus QuestStatus;

    // 任�?等�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 QuestLevel;

    // 建議等�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SuggestedLevel;

    // 任�X��X��"
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingQuestObjective> QuestObjectives;

    // ?��X��X��"
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingStoryChoice> StoryChoices;

    // 任�X�勵
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> QuestRewards;

    // 經�X�勵
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExperienceReward;

    // ?��X�勵
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReputationReward;

    // ?��X�勵
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingItemData> ItemRewards;

    // ?�置任�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> PrerequisiteQuests;

    // �X條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UnlockConditions;

    // 任�X��X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime StartTime;

    // 任�X�止?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime DeadlineTime;

    // 任�?位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector QuestLocation;

    // 任�?NPC
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestGiver;

    // 任�X��?路�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestIconPath;

    // 任�?標籤
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> QuestTags;

    // ?��X��X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StoryImportance;

    // ?��?次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RepeatCount;

    // ?�大�?複次X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// ?��?節�?USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingStoryNode
{
    GENERATED_BODY()

    // 節點ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NodeID;

    // 節點�X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NodeType;

    // 節點內�?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NodeContent;

    // 節點選X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingStoryChoice> NodeChoices;

    // 節點�?�?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> NodeConditions;

    // 節點�X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> NodeResults;

    // 節點�X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float NodeWeight;

    // ?�否已訪X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsVisited;

    // 節點�?�?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// 任�?管�?事件委�?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestAccepted, const FMingQuestData&, Quest, const FString&, QuestGiver};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestCompleted, const FMingQuestData&, Quest, bool, bSuccess};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnQuestObjectiveUpdated, int32, QuestID, int32, ObjectiveID, int32, NewProgress};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStoryChoiceMade, int32, QuestID, int32, ChoiceID, const FMingStoryChoice&, Choice};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestFailed, const FMingQuestData&, Quest, const FString&, FailureReason};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestAbandoned, int32, QuestID, const FString&, Reason};

/**
 * 任�?管�X * 負責管�?任�X��?事系�? */
UCLASS(ClassGroup = (Personal), Blueprintable, BlueprintType)
class MINGPERSONAL_API UMingQuestManager : public UObject
{
    GENERATED_BODY()

public:
    UMingQuestManager(};

    // ?��X�任?�系�?    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool InitializeQuestSystem(};

    // ?��?任�?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool AcceptQuest(int32 QuestID, const FString& QuestGiver};

    // ?��?任�?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool DeclineQuest(int32 QuestID};

    // ?��?任�?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool AbandonQuest(int32 QuestID, const FString& Reason};

    // 完�?任�?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool CompleteQuest(int32 QuestID, bool bSuccess = true};

    // ?�新任�X��?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool UpdateQuestObjective(int32 QuestID, int32 ObjectiveID, int32 Progress};

    // ?�出?��X��?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool MakeStoryChoice(int32 QuestID, int32 ChoiceID};

    // ?��?任�X��"
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingQuestData> GetAvailableQuests() const;

    // ?��?活�?任�?
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingQuestData> GetActiveQuests() const;

    // ?��?已�X�任X    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingQuestData> GetCompletedQuests() const;

    // ?��X��?任�?
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    FMingQuestData GetQuest(int32 QuestID) const;

    // 檢查任�?條件
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    bool CanAcceptQuest(int32 QuestID) const;

    // ?��?任�X�度
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    float GetQuestProgress(int32 QuestID) const;

    // ?��X��?節�?    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingStoryNode> GetStoryNodes(int32 QuestID) const;

    // �X�任X    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    void UnlockNewQuests(};

    // ?�置任�?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool ResetQuest(int32 QuestID};

    // ?��?任�?統�?
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TMap<EMingQuestType, int32> GetQuestStatistics() const;

    // ?��X��X�支
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingStoryChoice> GetStoryChoices(int32 QuestID) const;

    // 保�?任�X��?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool SaveQuestData(};

    // 載入任�X��?
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool LoadQuestData(};

    // 清除?�?�任?�數X    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    void ClearAllQuestData(};

    // 事件委�?
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
    // 任�X��?�?    UPROPERTY()
    TMap<int32, FMingQuestData> QuestDatabase;

    // ?��?節點數?�庫 - 注�?：TMap<TArray> 不支XUPROPERTY
    TMap<int32, TArray<FMingStoryNode>> StoryNodeDatabase;

    // 活�?任�X��"
    UPROPERTY()
    TArray<int32> ActiveQuests;

    // 已�X�任?��?�?    UPROPERTY()
    TArray<int32> CompletedQuests;

    // ?��X��?歷史 - 注�?：TMap<TArray> 不支XUPROPERTY
    TMap<int32, TArray<int32>> StoryChoiceHistory;

    // ?�否已�?始�?
    UPROPERTY()
    bool bInitialized;

private:
    // 載入?�設任�?
    void LoadDefaultQuests(};

    // 載入?�設?��?節�?    void LoadDefaultStoryNodes(};

    // 驗�?任�X��?
    bool ValidateQuestData(const FMingQuestData& Quest) const;

    // 檢查任�?完�?條件
    bool CheckQuestCompletion(int32 QuestID) const;

    // ?��?任�X�勵
    void ProcessQuestRewards(const FMingQuestData& Quest};

    // ?�新?��X�支
    void UpdateStoryBranch(int32 QuestID, int32 ChoiceID};

    // 計�?任�X��X    float CalculateQuestImportance(const FMingQuestData& Quest) const;

    // ?��X��?任�?
    FMingQuestData GenerateRandomQuest(EMingQuestType QuestType) const;

    // ?��X�接?�任X    TArray<int32> GetAvailableQuestIDs() const;

    // 檢查?�置任�?
    bool CheckPrerequisiteQuests(const FMingQuestData& Quest) const;

    // 檢查�X條件
    bool CheckUnlockConditions(const FMingQuestData& Quest) const;

    // ?�新任�X�X    void UpdateQuestStatus(int32 QuestID, EMingQuestStatus NewStatus};

    // ?��X��?後�?
    void ProcessStoryConsequences(int32 QuestID, const FMingStoryChoice& Choice};

    // 計�X��?權�?
    float CalculateChoiceWeight(const FMingStoryChoice& Choice) const;

    // ?��X��?線索
    TArray<FString> GetStoryClues(int32 QuestID) const;

    // ?�新任�X��?
    void UpdateQuestLog(int32 QuestID, const FString& LogEntry};

    // 保�X��?快照
    void SaveStorySnapshot(};

    // 載入?��?快照
    void LoadStorySnapshot(};

    // 計�?任�X��
    float CalculateQuestDifficulty(const FMingQuestData& Quest) const;

    // 調整任�X��
    void AdjustQuestDifficulty(int32 QuestID, float DifficultyModifier};

    // ?��?任�?建議
    TArray<FString> GetQuestSuggestions(int32 QuestID) const;

    // ?��?任�?失�?
    void HandleQuestFailure(int32 QuestID, const FString& FailureReason};

    // ?��?任�X��?
    void HandleQuestAbandonment(int32 QuestID, const FString& Reason};

    // 計�X��?影響
    TMap<FString, float> CalculateStoryImpact(int32 QuestID, int32 ChoiceID) const;

    // ?�新世�X�X    void UpdateWorldState(int32 QuestID, const TMap<FString, FString>& Results};

    // ?��?任�?標�?
    void GenerateQuestMarkers(int32 QuestID};

    // 移除任�?標�?
    void RemoveQuestMarkers(int32 QuestID};

    // 計�?任�X��?    float CalculateQuestValue(const FMingQuestData& Quest) const;

    // ?��?任�X��?�?    TArray<FDateTime> GetQuestTimeline(int32 QuestID) const;

    // 檢查任�?衝�?
    bool CheckQuestConflict(int32 QuestID1, int32 QuestID2) const;

    // �?��任�?衝�?
    void ResolveQuestConflict(int32 QuestID1, int32 QuestID2};

    // ?��?任�?依賴
    TArray<int32> GetQuestDependencies(int32 QuestID) const;

    // ?�新任�?依賴
    void UpdateQuestDependencies(int32 QuestID};

    // 計�X��X�支概�?
    float CalculateStoryBranchProbability(const FMingStoryChoice& Choice) const;

    // ?��X��?事選X    void RandomizeStoryChoices(int32 QuestID};

    // ?��X��X�支
    void LockStoryBranch(int32 QuestID, int32 ChoiceID};

    // �X��X�支
    void UnlockStoryBranch(int32 QuestID, int32 ChoiceID};

    // ?��X��?統�?
    TMap<FString, int32> GetStoryStatistics() const;

    // 計�X��?深度
    int32 CalculateStoryDepth(int32 QuestID) const;

    // ?��X��X�覽
    FString GetStoryPreview(int32 QuestID) const;

    // ?��X��X�制
    void ProcessTimeLimits(};

    // ?�新任�X�止?��?
    void UpdateQuestDeadlines(};

    // 檢查任�X��?
    void CheckQuestExpiration(};

    // ?��X��?任�?
    void HandleExpiredQuest(int32 QuestID};

    // ?��?任�X�示
    TArray<FString> GenerateQuestHints(int32 QuestID) const;

    // 計�?任�?完�X    float CalculateQuestCompletionRate() const;

    // ?��?任�X�薦
    TArray<int32> GetQuestRecommendations() const;

    // ?�新任�X�薦
    void UpdateQuestRecommendations(};

    // 計�X��X�支影響
    TMap<FString, float> CalculateBranchImpact(const FMingStoryChoice& Choice) const;
};

