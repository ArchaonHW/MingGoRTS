#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCharacterGrowthSystem.h"
#include "MingItemManager.h"
#include "MingQuestManager.generated.h"

// 任務類型
UENUM(BlueprintType)
enum class EMingQuestType : uint8
{
    Main,              // 主線任務
    Side,              // 支線任務
    Daily,              // 每日任務
    Weekly,             // 每周任務
    Event,              // 活動任務
    Hidden,             // 隱藏任務
    Tutorial,           // 教程任務
    Achievement         // 成就任務
};

// 任務狀態
UENUM(BlueprintType)
enum class EMingQuestStatus : uint8
{
    Available,          // 可接取
    Active,             // 進行中
    Completed,          // 已完成
    Failed,             // 失敗
    Abandoned,          // 放棄
    Locked              // 鎖定
};

// 任務目標類型
UENUM(BlueprintType)
enum class EMingQuestObjectiveType : uint8
{
    Kill,               // 擊殺
    Collect,            // 收集
    Deliver,            // 運送
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
enum class EMingStoryBranch : uint8
{
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

    // 當前進度
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

    // 是否可選擇
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

// 故事選項
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingStoryChoice
{
    GENERATED_BODY()

    // 選項ID
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

    // 選項條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ChoiceConditions;

    // 選項結果
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> ChoiceResults;

    // 道德影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoralImpact;

    // 聲望影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReputationImpact;

    // 關係影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> RelationshipImpact;

    // 是否已選擇
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsChosen;

    // 選項權重
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

// 任務數據
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingQuestData
{
    GENERATED_BODY()

    // 任務ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 QuestID;

    // 任務名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestName;

    // 任務描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestDescription;

    // 任務類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingQuestType QuestType;

    // 任務狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingQuestStatus QuestStatus;

    // 任務等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 QuestLevel;

    // 建議等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SuggestedLevel;

    // 任務目標列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingQuestObjective> QuestObjectives;

    // 故事選項列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingStoryChoice> StoryChoices;

    // 任務獎勵
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> QuestRewards;

    // 經驗獎勵
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExperienceReward;

    // 聲望獎勵
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReputationReward;

    // 物品獎勵
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingItemData> ItemRewards;

    // 前置任務
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> PrerequisiteQuests;

    // 解鎖條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UnlockConditions;

    // 任務開始時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime StartTime;

    // 任務截止時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime DeadlineTime;

    // 任務位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector QuestLocation;

    // 任務NPC
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestGiver;

    // 任務圖標路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestIconPath;

    // 任務標籤
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> QuestTags;

    // 故事重要性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StoryImportance;

    // 重複次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RepeatCount;

    // 最大重複次數
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

// 故事節點
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingStoryNode
{
    GENERATED_BODY()

    // 節點ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NodeID;

    // 節點類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NodeType;

    // 節點內容
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NodeContent;

    // 節點選項
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingStoryChoice> NodeChoices;

    // 節點條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> NodeConditions;

    // 節點結果
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> NodeResults;

    // 節點權重
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float NodeWeight;

    // 是否已訪問
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsVisited;

    // 節點位置
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

// 任務管理事件委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestAccepted, const FMingQuestData&, Quest, const FString&, QuestGiver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestCompleted, const FMingQuestData&, Quest, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnQuestObjectiveUpdated, int32, QuestID, int32, ObjectiveID, int32, NewProgress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStoryChoiceMade, int32, QuestID, int32, ChoiceID, const FMingStoryChoice&, Choice);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestFailed, const FMingQuestData&, Quest, const FString&, FailureReason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestAbandoned, int32, QuestID, const FString&, Reason);

/**
 * 任務管理器
 * 負責管理任務和故事系統
 */
UCLASS(ClassGroup = (Personal), Blueprintable, BlueprintType)
class MINGPERSONAL_API UMingQuestManager : public UObject
{
    GENERATED_BODY()

public:
    UMingQuestManager();

    // 初始化任務系統
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool InitializeQuestSystem();

    // 接受任務
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool AcceptQuest(int32 QuestID, const FString& QuestGiver);

    // 拒絕任務
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool DeclineQuest(int32 QuestID);

    // 放棄任務
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool AbandonQuest(int32 QuestID, const FString& Reason);

    // 完成任務
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool CompleteQuest(int32 QuestID, bool bSuccess = true);

    // 更新任務目標
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool UpdateQuestObjective(int32 QuestID, int32 ObjectiveID, int32 Progress);

    // 做出故事選擇
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool MakeStoryChoice(int32 QuestID, int32 ChoiceID);

    // 獲取任務列表
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingQuestData> GetAvailableQuests() const;

    // 獲取活躍任務
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingQuestData> GetActiveQuests() const;

    // 獲取已完成任務
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingQuestData> GetCompletedQuests() const;

    // 獲取特定任務
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    FMingQuestData GetQuest(int32 QuestID) const;

    // 檢查任務條件
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    bool CanAcceptQuest(int32 QuestID) const;

    // 獲取任務進度
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    float GetQuestProgress(int32 QuestID) const;

    // 獲取故事節點
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingStoryNode> GetStoryNodes(int32 QuestID) const;

    // 解鎖新任務
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    void UnlockNewQuests();

    // 重置任務
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool ResetQuest(int32 QuestID);

    // 獲取任務統計
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TMap<EMingQuestType, int32> GetQuestStatistics() const;

    // 獲取故事分支
    UFUNCTION(BlueprintPure, Category = "Quest Manager")
    TArray<FMingStoryChoice> GetStoryChoices(int32 QuestID) const;

    // 保存任務數據
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool SaveQuestData();

    // 載入任務數據
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    bool LoadQuestData();

    // 清除所有任務數據
    UFUNCTION(BlueprintCallable, Category = "Quest Manager")
    void ClearAllQuestData();

    // 事件委託
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
    // 任務數據庫
    UPROPERTY()
    TMap<int32, FMingQuestData> QuestDatabase;

    // 故事節點數據庫 - 注意：TMap<TArray> 不支持 UPROPERTY
    TMap<int32, TArray<FMingStoryNode>> StoryNodeDatabase;

    // 活躍任務列表
    UPROPERTY()
    TArray<int32> ActiveQuests;

    // 已完成任務列表
    UPROPERTY()
    TArray<int32> CompletedQuests;

    // 故事選擇歷史 - 注意：TMap<TArray> 不支持 UPROPERTY
    TMap<int32, TArray<int32>> StoryChoiceHistory;

    // 是否已初始化
    UPROPERTY()
    bool bInitialized;

private:
    // 載入預設任務
    void LoadDefaultQuests();

    // 載入預設故事節點
    void LoadDefaultStoryNodes();

    // 驗證任務數據
    bool ValidateQuestData(const FMingQuestData& Quest) const;

    // 檢查任務完成條件
    bool CheckQuestCompletion(int32 QuestID) const;

    // 處理任務獎勵
    void ProcessQuestRewards(const FMingQuestData& Quest);

    // 更新故事分支
    void UpdateStoryBranch(int32 QuestID, int32 ChoiceID);

    // 計算任務重要性
    float CalculateQuestImportance(const FMingQuestData& Quest) const;

    // 生成隨機任務
    FMingQuestData GenerateRandomQuest(EMingQuestType QuestType) const;

    // 獲取可接取任務
    TArray<int32> GetAvailableQuestIDs() const;

    // 檢查前置任務
    bool CheckPrerequisiteQuests(const FMingQuestData& Quest) const;

    // 檢查解鎖條件
    bool CheckUnlockConditions(const FMingQuestData& Quest) const;

    // 更新任務狀態
    void UpdateQuestStatus(int32 QuestID, EMingQuestStatus NewStatus);

    // 處理故事後果
    void ProcessStoryConsequences(int32 QuestID, const FMingStoryChoice& Choice);

    // 計算選擇權重
    float CalculateChoiceWeight(const FMingStoryChoice& Choice) const;

    // 獲取故事線索
    TArray<FString> GetStoryClues(int32 QuestID) const;

    // 更新任務日誌
    void UpdateQuestLog(int32 QuestID, const FString& LogEntry);

    // 保存故事快照
    void SaveStorySnapshot();

    // 載入故事快照
    void LoadStorySnapshot();

    // 計算任務難度
    float CalculateQuestDifficulty(const FMingQuestData& Quest) const;

    // 調整任務難度
    void AdjustQuestDifficulty(int32 QuestID, float DifficultyModifier);

    // 獲取任務建議
    TArray<FString> GetQuestSuggestions(int32 QuestID) const;

    // 處理任務失敗
    void HandleQuestFailure(int32 QuestID, const FString& FailureReason);

    // 處理任務放棄
    void HandleQuestAbandonment(int32 QuestID, const FString& Reason);

    // 計算故事影響
    TMap<FString, float> CalculateStoryImpact(int32 QuestID, int32 ChoiceID) const;

    // 更新世界狀態
    void UpdateWorldState(int32 QuestID, const TMap<FString, FString>& Results);

    // 生成任務標記
    void GenerateQuestMarkers(int32 QuestID);

    // 移除任務標記
    void RemoveQuestMarkers(int32 QuestID);

    // 計算任務價值
    float CalculateQuestValue(const FMingQuestData& Quest) const;

    // 獲取任務時間線
    TArray<FDateTime> GetQuestTimeline(int32 QuestID) const;

    // 檢查任務衝突
    bool CheckQuestConflict(int32 QuestID1, int32 QuestID2) const;

    // 解決任務衝突
    void ResolveQuestConflict(int32 QuestID1, int32 QuestID2);

    // 獲取任務依賴
    TArray<int32> GetQuestDependencies(int32 QuestID) const;

    // 更新任務依賴
    void UpdateQuestDependencies(int32 QuestID);

    // 計算故事分支概率
    float CalculateStoryBranchProbability(const FMingStoryChoice& Choice) const;

    // 隨機化故事選項
    void RandomizeStoryChoices(int32 QuestID);

    // 鎖定故事分支
    void LockStoryBranch(int32 QuestID, int32 ChoiceID);

    // 解鎖故事分支
    void UnlockStoryBranch(int32 QuestID, int32 ChoiceID);

    // 獲取故事統計
    TMap<FString, int32> GetStoryStatistics() const;

    // 計算故事深度
    int32 CalculateStoryDepth(int32 QuestID) const;

    // 獲取故事預覽
    FString GetStoryPreview(int32 QuestID) const;

    // 處理時間限制
    void ProcessTimeLimits();

    // 更新任務截止時間
    void UpdateQuestDeadlines();

    // 檢查任務過期
    void CheckQuestExpiration();

    // 處理過期任務
    void HandleExpiredQuest(int32 QuestID);

    // 生成任務提示
    TArray<FString> GenerateQuestHints(int32 QuestID) const;

    // 計算任務完成率
    float CalculateQuestCompletionRate() const;

    // 獲取任務推薦
    TArray<int32> GetQuestRecommendations() const;

    // 更新任務推薦
    void UpdateQuestRecommendations();

    // 計算故事分支影響
    TMap<FString, float> CalculateBranchImpact(const FMingStoryChoice& Choice) const;
};
