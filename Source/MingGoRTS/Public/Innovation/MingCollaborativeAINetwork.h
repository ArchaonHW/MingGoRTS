#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Interface.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "MingCollaborativeAINetwork.generated.h"

// AI智能體類型枚舉
UENUM(BlueprintType)
enum class EAIAgentType: uint8 {
    Strategic = 0,        // 戰略智能體
    Tactical = 1,         // 戰術智能體
    Operational = 2,      // 作戰智能體
    Support = 3,          // 支援智能體
    Communication = 4,    // 通信智能體
    Coordination = 5,     // 協調智能體
    Learning = 6,         // 學習智能體
    Adaptive = 7          // 適應智能體
};

// AI智能體狀態枚舉
UENUM(BlueprintType)
enum class EAIAgentState: uint8 {
    Idle = 0,             // 空閒狀態
    Active = 1,           // 活動狀態
    Processing = 2,       // 處理中
    Communicating = 3,    // 通信中
    Coordinating = 4,     // 協調中
    Learning = 5,         // 學習中
    Error = 6             // 錯誤狀態
};

// 協作任務類型枚舉
UENUM(BlueprintType)
enum class ECollaborationTaskType: uint8 {
    StrategicPlanning = 0,    // 戰略規劃
    TacticalExecution = 1,    // 戰術執行
    ResourceAllocation = 2,   // 資源分配
    ThreatAssessment = 3,     // 威脅評估
    DecisionMaking = 4,       // 決策制定
    InformationSharing = 5,   // 信息共享
    LearningAndAdaptation = 6, // 學習和適應
    CrisisManagement = 7      // 危機管理
};

// 通信協議類型枚舉
UENUM(BlueprintType)
enum class ECommunicationProtocol: uint8 {
    DirectMessage = 0,        // 直接消息
    Broadcast = 1,           // 廣播
    Multicast = 2,           // 多播
    RequestResponse = 3,     // 請求-響應
    PublishSubscribe = 4,     // 發布-訂閱
    Stream = 5               // 流式傳輸
};

// AI智能體信息結構
USTRUCT(BlueprintType)
struct FAIAgentInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString AgentID;

    UPROPERTY(BlueprintReadOnly)
    EAIAgentType AgentType = EAIAgentType::Strategic;

    UPROPERTY(BlueprintReadOnly)
    EAIAgentState AgentState = EAIAgentState::Idle;

    UPROPERTY(BlueprintReadOnly)
    FString AgentName;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Capabilities;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Specializations;

    UPROPERTY(BlueprintReadOnly)
    float ProcessingPower = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    float MemoryCapacity = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    float CommunicationRange = 1000.0f;

    UPROPERTY(BlueprintReadOnly)
    FDateTime CreationTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastActiveTime;

    UPROPERTY(BlueprintReadOnly)
    int32 TaskCount = 0;

    UPROPERTY(BlueprintReadOnly)
    float PerformanceScore = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    bool bIsActive = true;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CustomProperties;
};

// 協作任務結構
USTRUCT(BlueprintType)
struct FCollaborationTask
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TaskID;

    UPROPERTY(BlueprintReadOnly)
    ECollaborationTaskType TaskType = ECollaborationTaskType::StrategicPlanning;

    UPROPERTY(BlueprintReadOnly)
    FString TaskName;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RequiredAgents;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AssignedAgents;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> TaskParameters;

    UPROPERTY(BlueprintReadOnly)
    float Priority = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    float Complexity = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    float EstimatedDuration = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float Progress = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    FDateTime CreationTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Deadline;

    UPROPERTY(BlueprintReadOnly)
    bool bIsCompleted = false;

    UPROPERTY(BlueprintReadOnly)
    bool bIsInProgress = false;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SubTasks;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Results;
};

// 通信消息結構
USTRUCT(BlueprintType)
struct FAICommunicationMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString MessageID;

    UPROPERTY(BlueprintReadOnly)
    FString SenderID;

    UPROPERTY(BlueprintReadOnly)
    FString ReceiverID;

    UPROPERTY(BlueprintReadOnly)
    ECommunicationProtocol Protocol = ECommunicationProtocol::DirectMessage;

    UPROPERTY(BlueprintReadOnly)
    FString MessageType;

    UPROPERTY(BlueprintReadOnly)
    FString MessageContent;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Metadata;

    UPROPERTY(BlueprintReadOnly)
    float Priority = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    bool bRequiresResponse = false;

    UPROPERTY(BlueprintReadOnly)
    FString ResponseTo;

    UPROPERTY(BlueprintReadOnly)
    bool bIsDelivered = false;

    UPROPERTY(BlueprintReadOnly)
    bool bIsRead = false;
};

// 集體智能決策結構
USTRUCT(BlueprintType)
struct FCollectiveIntelligenceDecision
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString DecisionID;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionContext;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ParticipatingAgents;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> AgentContributions;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionResult;

    UPROPERTY(BlueprintReadOnly)
    float ConfidenceLevel = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AlternativeOptions;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> OptionScores;

    UPROPERTY(BlueprintReadOnly)
    FString Rationale;

    UPROPERTY(BlueprintReadOnly)
    FDateTime DecisionTime;

    UPROPERTY(BlueprintReadOnly)
    float ExecutionTime = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    bool bIsExecuted = false;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Dependencies;
};

// 學習數據結構
USTRUCT(BlueprintType)
struct FAILearningData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString LearningID;

    UPROPERTY(BlueprintReadOnly)
    FString AgentID;

    UPROPERTY(BlueprintReadOnly)
    FString LearningType;

    UPROPERTY(BlueprintReadOnly)
    FString LearningContent;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> LearningParameters;

    UPROPERTY(BlueprintReadOnly)
    float PerformanceImprovement = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float LearningRate = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LearningTime;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RelatedAgents;

    UPROPERTY(BlueprintReadOnly)
    bool bIsShared = false;
};

// 委託聲明
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIAgentCreated, const FAIAgentInfo&, AgentInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIAgentStateChanged, const FAIAgentInfo&, AgentInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTaskCreated, const FCollaborationTask&, Task);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTaskCompleted, const FCollaborationTask&, Task);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageSent, const FAICommunicationMessage&, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDecisionMade, const FCollectiveIntelligenceDecision&, Decision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLearningOccurred, const FAILearningData&, LearningData);

/**
 * 協作AI網絡系統接口
 * 提供多AI智能體協作、分布式決策和集體智能學習功能
 */
UINTERFACE(BlueprintType)
class UCollaborativeAINetworkInterface : public UInterface
{
    GENERATED_BODY()
};

class ICollaborativeAINetworkInterface
{
    GENERATED_BODY()

public:
    // 初始化系統
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Collaborative AI Network")
    bool InitializeCollaborativeAINetwork();

    // 創建AI智能體
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Collaborative AI Network")
    FString CreateAIAgent(const FString& AgentName, EAIAgentType AgentType, const TArray<FString>& Capabilities);

    // 獲取AI智能體信息
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Collaborative AI Network")
    FAIAgentInfo GetAIAgentInfo(const FString& AgentID) const;

    // 更新AI智能體狀態
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Collaborative AI Network")
    bool UpdateAIAgentState(const FString& AgentID, EAIAgentState NewState);

    // 創建協作任務
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Collaborative AI Network")
    FString CreateCollaborationTask(ECollaborationTaskType TaskType, const FString& TaskName, const FString& Description, const TArray<FString>& RequiredAgents);

    // 分配任務給智能體
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Collaborative AI Network")
    bool AssignTaskToAgents(const FString& TaskID, const TArray<FString>& AgentIDs);

    // 發送通信消息
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Collaborative AI Network")
    bool SendMessage(const FString& SenderID, const FString& ReceiverID, const FString& MessageType, const FString& MessageContent);

    // 廣播消息
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Collaborative AI Network")
    bool BroadcastMessage(const FString& SenderID, const FString& MessageType, const FString& MessageContent);

    // 執行集體決策
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Collaborative AI Network")
    FString ExecuteCollectiveDecision(const FString& DecisionContext, const TArray<FString>& ParticipatingAgents);

    // 執行協作學習
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Collaborative AI Network")
    bool ExecuteCollaborativeLearning(const TArray<FString>& LearningAgents, const FString& LearningContent);

    // 獲取網絡統計信息
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Collaborative AI Network")
    TMap<FString, float> GetNetworkStatistics() const;

    // 優化網絡性能
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Collaborative AI Network")
    bool OptimizeNetworkPerformance();
};

/**
 * 協作AI網絡系統
 * 實現多AI智能體協作、分布式決策和集體智能學習
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Innovation))
class MINGGORTS_API UMingCollaborativeAINetwork : public UObject, public ICollaborativeAINetworkInterface
{
    GENERATED_BODY()

public:
    UMingCollaborativeAINetwork();

    // 系統初始化
    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    bool InitializeCollaborativeAINetwork() override;

    // 創建AI智能體
    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    FString CreateAIAgent(const FString& AgentName, EAIAgentType AgentType, const TArray<FString>& Capabilities) override;

    // 獲取AI智能體信息
    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    FAIAgentInfo GetAIAgentInfo(const FString& AgentID) const override;

    // 更新AI智能體狀態
    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    bool UpdateAIAgentState(const FString& AgentID, EAIAgentState NewState) override;

    // 創建協作任務
    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    FString CreateCollaborationTask(ECollaborationTaskType TaskType, const FString& TaskName, const FString& Description, const TArray<FString>& RequiredAgents) override;

    // 分配任務給智能體
    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    bool AssignTaskToAgents(const FString& TaskID, const TArray<FString>& AgentIDs) override;

    // 發送通信消息
    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    bool SendMessage(const FString& SenderID, const FString& ReceiverID, const FString& MessageType, const FString& MessageContent) override;

    // 廣播消息
    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    bool BroadcastMessage(const FString& SenderID, const FString& MessageType, const FString& MessageContent) override;

    // 執行集體決策
    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    FString ExecuteCollectiveDecision(const FString& DecisionContext, const TArray<FString>& ParticipatingAgents) override;

    // 執行協作學習
    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    bool ExecuteCollaborativeLearning(const TArray<FString>& LearningAgents, const FString& LearningContent) override;

    // 獲取網絡統計信息
    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    TMap<FString, float> GetNetworkStatistics() const override;

    // 優化網絡性能
    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    bool OptimizeNetworkPerformance() override;

    // 高級功能
    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    TArray<FAIAgentInfo> GetActiveAgents() const;

    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    TArray<FCollaborationTask> GetActiveTasks() const;

    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    TArray<FAICommunicationMessage> GetPendingMessages() const;

    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    TArray<FCollectiveIntelligenceDecision> GetRecentDecisions(int32 Count = 10) const;

    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    bool RemoveAIAgent(const FString& AgentID);

    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    bool CancelTask(const FString& TaskID);

    UFUNCTION(BlueprintCallable, Category = "Collaborative AI Network")
    bool UpdateTaskProgress(const FString& TaskID, float Progress);

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnAIAgentCreated OnAIAgentCreated;

    UPROPERTY(BlueprintAssignable)
    FOnAIAgentStateChanged OnAIAgentStateChanged;

    UPROPERTY(BlueprintAssignable)
    FOnTaskCreated OnTaskCreated;

    UPROPERTY(BlueprintAssignable)
    FOnTaskCompleted OnTaskCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnMessageSent OnMessageSent;

    UPROPERTY(BlueprintAssignable)
    FOnDecisionMade OnDecisionMade;

    UPROPERTY(BlueprintAssignable)
    FOnLearningOccurred OnLearningOccurred;

protected:
    // 系統組件
    UPROPERTY(BlueprintReadOnly, Category = "Collaborative AI Network")
    TMap<FString, FAIAgentInfo> AIAgents;

    UPROPERTY(BlueprintReadOnly, Category = "Collaborative AI Network")
    TMap<FString, FCollaborationTask> CollaborationTasks;

    UPROPERTY(BlueprintReadOnly, Category = "Collaborative AI Network")
    TArray<FAICommunicationMessage> MessageQueue;

    UPROPERTY(BlueprintReadOnly, Category = "Collaborative AI Network")
    TArray<FCollectiveIntelligenceDecision> DecisionHistory;

    UPROPERTY(BlueprintReadOnly, Category = "Collaborative AI Network")
    TArray<FAILearningData> LearningHistory;

    // 配置參數
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collaborative AI Network")
    int32 MaxAgents = 100;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collaborative AI Network")
    int32 MaxConcurrentTasks = 50;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collaborative AI Network")
    float CommunicationDelay = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collaborative AI Network")
    float DecisionTimeout = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collaborative AI Network")
    float LearningRate = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collaborative AI Network")
    bool bEnableAutoOptimization = true;

    // 狀態變數
    UPROPERTY(BlueprintReadOnly, Category = "Collaborative AI Network")
    bool bIsInitialized = false;

    UPROPERTY(BlueprintReadOnly, Category = "Collaborative AI Network")
    int32 ActiveAgentCount = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Collaborative AI Network")
    int32 ActiveTaskCount = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Collaborative AI Network")
    float NetworkEfficiency = 1.0f;

private:
    // 內部方法
    FString GenerateUniqueAgentID() const;
    FString GenerateUniqueTaskID() const;
    FString GenerateUniqueMessageID() const;
    FString GenerateUniqueDecisionID() const;
    FString GenerateUniqueLearningID() const;

    bool ValidateAgentCreation(const FString& AgentName, EAIAgentType AgentType, const TArray<FString>& Capabilities) const;
    bool ValidateTaskAssignment(const FString& TaskID, const TArray<FString>& AgentIDs) const;
    bool ValidateCommunication(const FString& SenderID, const FString& ReceiverID) const;

    void ProcessMessageQueue();
    void UpdateAgentPerformance();
    void OptimizeTaskDistribution();
    void PerformCollectiveLearning();

    FAIAgentInfo CreateDefaultAgentInfo(const FString& AgentID, const FString& AgentName, EAIAgentType AgentType, const TArray<FString>& Capabilities);
    FCollaborationTask CreateDefaultTask(const FString& TaskID, ECollaborationTaskType TaskType, const FString& TaskName, const FString& Description);
    FAICommunicationMessage CreateDefaultMessage(const FString& MessageID, const FString& SenderID, const FString& ReceiverID, const FString& MessageType, const FString& MessageContent);

    // 定時器
    FTimerHandle MessageProcessingTimerHandle;
    FTimerHandle PerformanceUpdateTimerHandle;
    FTimerHandle OptimizationTimerHandle;
    FTimerHandle LearningTimerHandle;

    // 統計數據
    UPROPERTY()
    TMap<FString, float> NetworkStats;

    UPROPERTY()
    float LastOptimizationTime = 0.0f;

    UPROPERTY()
    int32 TotalMessagesSent = 0;

    UPROPERTY()
    int32 TotalDecisionsMade = 0;

    UPROPERTY()
    int32 TotalLearningSessions = 0;
};
