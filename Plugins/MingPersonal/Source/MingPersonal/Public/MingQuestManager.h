#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCharacterGrowthSystem.h"
#include "MingItemManager.h"
#include "MingQuestManager.generated.h"

// 任??類??
UENUM(BlueprintType)
enum class EMingQuestType: uint8 {
    Main,              // 主線任務
    Side,              // 支線任務
    Daily,              // 每日任務
    Weekly,             // 每周任務
    Event,              // 活動任務
    Hidden,             // 隱藏任務
    Tutorial,           // 教學任務
    Achievement         // 成就任務
};

// 任務狀態UENUM(BlueprintType)
enum class EMingQuestStatus: uint8 {
    Available,          // 可接受
    Active,             // 進行中
    Completed,          // 已完成
    Failed,             // 失敗
    Abandoned,          // 已放棄
    Locked              // 已鎖定
};

// 任務目標類型
UENUM(BlueprintType)
enum class EMingQuestObjectiveType: uint8 {
    Kill,               // 擊殺
    Collect,            // 收集
    Deliver,            // 交付
    Escort,             // 護送
    Defend,             // 防禦
    Explore,            // 探索
    Talk,               // 對話
    Interact,           // 互動
    Survive,            // 生存
    Capture,            // 佔領
    Destroy             // 摧毀
};

// 故事分支類型
UENUM(BlueprintType)
enum class EMingStoryBranch: uint8 {
    Linear,             // 線性
    Choice,             // 選擇分支
    Conditional,        // 條件分支
    Random,             // 隨機分支
    Moral,              // 道德選擇
    Strategic,          // 戰略選擇
    Personal            // 個人選擇
};

// 任務目標
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingQuestObjective
{
    GENERATED_BODY()

    // 目標ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ObjectiveID;

    // 目標描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ObjectiveDescription;

    // 目標類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingQuestObjectiveType ObjectiveType;

    // 目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetCount;

    // 目標進度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentProgress;

    // 目標參數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> ObjectiveParameters;

    // 是否可見
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsVisible;

    // 是否完成
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCompleted;

    // 是否可選
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsOptional;

    // 目標位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;

    // 目標NPC
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetNPC;

    // 時間限制
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeLimit;

    // 失敗條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> FailureConditions;

    FMingQuestObjective()
    {
        ObjectiveID = -1;
        ObjectiveDescription = TEXT("");
        ObjectiveType = EMingQuestObjectiveType::Kill;
        TargetCount = 0;
        CurrentProgress = 0;
        ObjectiveParameters.Empty();
        bIsVisible = true;
        bIsCompleted = false;
        bIsOptional = false;
        TargetLocation = FVector::ZeroVector;
        TargetNPC = TEXT("");
        TimeLimit = 0.0f;
        FailureConditions.Empty();
    }
};

// 目標數量
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingStoryChoice
{
    GENERATED_BODY()

    // 目標ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChoiceID;

    // 選項文本
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ChoiceText;

    // 選項描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ChoiceDescription;

    // 選項類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingStoryBranch ChoiceType;

    // 摧毀條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ChoiceConditions;

    // 摧毀結??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> ChoiceResults;

    // 道德影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoralImpact;

    // 摧毀影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReputationImpact;

    // 摧毀影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> RelationshipImpact;

    // ??否已選X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsChosen;

    // 摧毀權??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ChoiceWeight;

    FMingStoryChoice()
    {
        ChoiceID = -1;
        ChoiceText = TEXT("");
        ChoiceDescription = TEXT("");
        ChoiceType = EMingStoryBranch::Choice;
        ChoiceConditions.Empty();
        ChoiceResults.Empty();
        MoralImpact = 0.0f;
        ReputationImpact = 0.0f;
        RelationshipImpact.Empty();
        bIsChosen = false;
        ChoiceWeight = 1.0f;
    }
};

// 任務目標
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingQuestData
{
    GENERATED_BODY()

    // 任??ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 QuestID;

    // 任務名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestName;

    // 任務描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestDescription;

    // 任??類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingQuestType QuestType;

    // 任務狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingQuestStatus QuestStatus;

    // 任??等??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 QuestLevel;

    // 建議等??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SuggestedLevel;

    // 任務目標列表"
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingQuestObjective> QuestObjectives;

    // 故事選項"
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingStoryChoice> StoryChoices;

    // 任務獎勵
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> QuestRewards;

    // 經驗獎勵
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExperienceReward;

    // 物品獎勵
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReputationReward;

    // 物品獎勵
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingItemData> ItemRewards;

    // 重置任務
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> PrerequisiteQuests;

    // ?X條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UnlockConditions;

    // 任務目標列表?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime StartTime;

    // 任?X?止摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime DeadlineTime;

    // 任??位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector QuestLocation;

    // 任??NPC
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestGiver;

    // 任務目標路??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestIconPath;

    // 任??標籤
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> QuestTags;

    // 故事重要性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StoryImportance;

    // 摧毀次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RepeatCount;

    // ??大??複次X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxRepeats;

    FMingQuestData()
    {
        QuestID = -1;
        QuestName = TEXT("");
        QuestDescription = TEXT("");
        QuestType = EMingQuestType::Main;
        QuestStatus = EMingQuestStatus::Locked;
        QuestLevel = 1;
        SuggestedLevel = 1;
        QuestObjectives.Empty();
        StoryChoices.Empty();
        QuestRewards.Empty();
        ExperienceReward = 0.0f;
        ReputationReward = 0.0f;
        ItemRewards.Empty();
        PrerequisiteQuests.Empty();
        UnlockConditions.Empty();
        StartTime = FDateTime::MinValue();
        DeadlineTime = FDateTime::MaxValue();
        QuestLocation = FVector::ZeroVector;
        QuestGiver = TEXT("");
        QuestIconPath = TEXT("");
        QuestTags.Empty();
        StoryImportance = 1.0f;
        RepeatCount = 0;
        MaxRepeats = 0;
    }
};

// 摧毀節??USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingStoryNode
{
    GENERATED_BODY()

    // 節點ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NodeID;

    // 節點?X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NodeType;

    // 節點內??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NodeContent;

    // 節點選X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingStoryChoice> NodeChoices;

    // 節點摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> NodeConditions;

    // 節點?X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> NodeResults;

    // 節點?X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float NodeWeight;

    // ??否已訪X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsVisited;

    // 節點摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector NodePosition;

    FMingStoryNode()
    {
        NodeID = -1;
        NodeType = TEXT("");
        NodeContent = TEXT("");
        NodeChoices.Empty();
        NodeConditions.Empty();
        NodeResults.Empty();
        NodeWeight = 1.0f;
        bIsVisited = false;
        NodePosition = FVector::ZeroVector;
    }
};

// 任??管??事件委??
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestAccepted, const FMingQuestData&, Quest, const FString&, QuestGiver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestCompleted, const FMingQuestData&, Quest, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnQuestObjectiveUpdated, int32, QuestID, int32, ObjectiveID, int32, NewProgress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStoryChoiceMade, int32, QuestID, int32, ChoiceID, const FMingStoryChoice&, Choice);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestFailed, const FMingQuestData&, Quest, const FString&, FailureReason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestAbandoned, int32, QuestID, const FString&, Reason);

/**
 * 任務管理器 * 負責管??任務目標事系?? */
UCLASS(ClassGroup = (Personal), Blueprintable, BlueprintType)
class MINGPERSONAL_API UMingQuestManager : public UObject
{
    GENERATED_BODY()

public:
    UMingQuestManager();

    // 初始化任務系統
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool InitializeQuestSystem();

    // 放棄任務
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool AcceptQuest(int32 QuestID, const FString& QuestGiver);

    // 放棄任務
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool DeclineQuest(int32 QuestID);

    // 放棄任務
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool AbandonQuest(int32 QuestID, const FString& Reason);

    // 完成任務
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool CompleteQuest(int32 QuestID, bool bSuccess = true);

    // ??新任務目標
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool UpdateQuestObjective(int32 QuestID, int32 ObjectiveID, int32 Progress);

    // ??出目標數量
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool MakeStoryChoice(int32 QuestID, int32 ChoiceID);

    // 摧毀任?X??"
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingQuestData> GetAvailableQuests() const;

    // 摧毀活動任務
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingQuestData> GetActiveQuests() const;

    // 摧毀已完成?任X
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingQuestData> GetCompletedQuests() const;

    // 目標數量任??
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    FMingQuestData GetQuest(int32 QuestID) const;

    // 檢查任務條件
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    bool CanAcceptQuest(int32 QuestID) const;

    // 摧毀任?X?度
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    float GetQuestProgress(int32 QuestID) const;

    // 目標數量節??
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingStoryNode> GetStoryNodes(int32 QuestID) const;

    // 解鎖任務
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    void UnlockNewQuests();

    // 重置任務
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool ResetQuest(int32 QuestID);

    // 放棄任務統??
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TMap<EMingQuestType, int32> GetQuestStatistics() const;

    // 故事重要性?支
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingStoryChoice> GetStoryChoices(int32 QuestID) const;

    // 保??任務目標
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool SaveQuestData();

    // 載入任務目標
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool LoadQuestData();

    // 清除放棄任務數X
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    void ClearAllQuestData();

    // 事件委??
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
    // 任務目標??
    UPROPERTY()
    TMap<int32, FMingQuestData> QuestDatabase;

    // 摧毀節點數??庫 - 注??：TMap<TArray> 不支XUPROPERTY
    TMap<int32, TArray<FMingStoryNode>> StoryNodeDatabase;

    // 活??任?X??"
    UPROPERTY()
    TArray<int32> ActiveQuests;

    // 已完成?任摧毀??
    UPROPERTY()
    TArray<int32> CompletedQuests;

    // 目標數量歷史 - 注??：TMap<TArray> 不支XUPROPERTY
    TMap<int32, TArray<int32>> StoryChoiceHistory;

    // ??否已??始??
    UPROPERTY()
    bool bInitialized;

private:
    // 載入??設任??
    void LoadDefaultQuests();

    // 載入??設摧毀節??
    void LoadDefaultStoryNodes();

    // 驗??任務目標
    bool ValidateQuestData(const FMingQuestData& Quest) const;

    // 檢查任務完成條件
    bool CheckQuestCompletion(int32 QuestID) const;

    // 摧毀任務獎勵
    void ProcessQuestRewards(const FMingQuestData& Quest);

    // ??新隨機分支
    void UpdateStoryBranch(int32 QuestID, int32 ChoiceID);

    // 計算任務重要性
    float CalculateQuestImportance(const FMingQuestData& Quest) const;

    // 目標數量任??
    FMingQuestData GenerateRandomQuest(EMingQuestType QuestType) const;

    // ???X?接??任X
    TArray<int32> GetAvailableQuestIDs() const;

    // 檢查重置任務
    bool CheckPrerequisiteQuests(const FMingQuestData& Quest) const;

    // 檢查解鎖條件
    bool CheckUnlockConditions(const FMingQuestData& Quest) const;

    // ??新任務狀態
    void UpdateQuestStatus(int32 QuestID, EMingQuestStatus NewStatus);

    // 目標數量後??
    void ProcessStoryConsequences(int32 QuestID, const FMingStoryChoice& Choice);

    // 計算選項權重
    float CalculateChoiceWeight(const FMingStoryChoice& Choice) const;

    // 目標數量線索
    TArray<FString> GetStoryClues(int32 QuestID) const;

    // ??新任務目標
    void UpdateQuestLog(int32 QuestID, const FString& LogEntry);

    // 保存物品快照
    void SaveStorySnapshot();

    // 載入摧毀快照
    void LoadStorySnapshot();

    // 計??任?X??
    float CalculateQuestDifficulty(const FMingQuestData& Quest) const;

    // 調整任?X??
    void AdjustQuestDifficulty(int32 QuestID, float DifficultyModifier);

    // 放棄任務建議
    TArray<FString> GetQuestSuggestions(int32 QuestID) const;

    // 放棄任務失敗
    void HandleQuestFailure(int32 QuestID, const FString& FailureReason);

    // 摧毀任務目標
    void HandleQuestAbandonment(int32 QuestID, const FString& Reason);

    // 計?X???影響
    TMap<FString, float> CalculateStoryImpact(int32 QuestID, int32 ChoiceID) const;

    // ??新世?X?X
    void UpdateWorldState(int32 QuestID, const TMap<FString, FString>& Results);

    // 放棄任務標??
    void GenerateQuestMarkers(int32 QuestID);

    // 移除任??標??
    void RemoveQuestMarkers(int32 QuestID);

    // 計??任務目標
    float CalculateQuestValue(const FMingQuestData& Quest) const;

    // 摧毀任務目標??
    TArray<FDateTime> GetQuestTimeline(int32 QuestID) const;

    // 檢查任??衝??
    bool CheckQuestConflict(int32 QuestID1, int32 QuestID2) const;

    // 放棄任務衝??
    void ResolveQuestConflict(int32 QuestID1, int32 QuestID2);

    // 放棄任務依賴
    TArray<int32> GetQuestDependencies(int32 QuestID) const;

    // ??新任??依賴
    void UpdateQuestDependencies(int32 QuestID);

    // 計?X??X?支概??
    float CalculateStoryBranchProbability(const FMingStoryChoice& Choice) const;

    // 目標數量事選X
    void RandomizeStoryChoices(int32 QuestID);

    // 故事重要性?支
    void LockStoryBranch(int32 QuestID, int32 ChoiceID);

    // ?X??X?支
    void UnlockStoryBranch(int32 QuestID, int32 ChoiceID);

    // 目標數量統??
    TMap<FString, int32> GetStoryStatistics() const;

    // 計?X???深度
    int32 CalculateStoryDepth(int32 QuestID) const;

    // 故事重要性?覽
    FString GetStoryPreview(int32 QuestID) const;

    // 故事重要性?制
    void ProcessTimeLimits();

    // ??新任?X?止摧毀
    void UpdateQuestDeadlines();

    // 檢查任務目標
    void CheckQuestExpiration();

    // 目標數量任??
    void HandleExpiredQuest(int32 QuestID);

    // 摧毀任?X?示
    TArray<FString> GenerateQuestHints(int32 QuestID) const;

    // 計??任??完?X
    float CalculateQuestCompletionRate() const;

    // 摧毀任?X?薦
    TArray<int32> GetQuestRecommendations() const;

    // ??新任?X?薦
    void UpdateQuestRecommendations();

    // 計?X??X?支影響
    TMap<FString, float> CalculateBranchImpact(const FMingStoryChoice& Choice) const;
};

