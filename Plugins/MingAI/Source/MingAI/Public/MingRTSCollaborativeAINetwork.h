#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCollaborativeAINetwork.generated.h"

UENUM(BlueprintType)
enum class EAIAgentRole : uint8
{
    Commander UMETA(DisplayName = "指揮官"),
    Scout UMETA(DisplayName = "偵察兵"),
    Defender UMETA(DisplayName = "防禦者"),
    Attacker UMETA(DisplayName = "攻擊者"),
    Support UMETA(DisplayName = "支援者"),
    Diplomat UMETA(DisplayName = "外交官"),
    Economist UMETA(DisplayName = "經濟專家"),
    Strategist UMETA(DisplayName = "戰略家"),
    Coordinator UMETA(DisplayName = "協調者"),
    Specialist UMETA(DisplayName = "專家"),
    Count UMETA(DisplayName = "角色數量")
};

UENUM(BlueprintType)
enum class ECollaborationType : uint8
{
    None UMETA(DisplayName = "無"),
    Coordination UMETA(DisplayName = "協調"),
    Cooperation UMETA(DisplayName = "合作"),
    Collaboration UMETA(DisplayName = "協作"),
    Coalition UMETA(DisplayName = "聯盟"),
    Swarm UMETA(DisplayName = "群體"),
    Hierarchy UMETA(DisplayName = "階層"),
    Network UMETA(DisplayName = "網絡"),
    Count UMETA(DisplayName = "協作類型數量")
};

UENUM(BlueprintType)
enum class ECommunicationProtocol : uint8
{
    Direct UMETA(DisplayName = "直接通信"),
    Broadcast UMETA(DisplayName = "廣播"),
    Multicast UMETA(DisplayName = "多播"),
    Chain UMETA(DisplayName = "鏈式"),
    Mesh UMETA(DisplayName = "網狀"),
    Star UMETA(DisplayName = "星型"),
    Hierarchical UMETA(DisplayName = "層級"),
    Gossip UMETA(DisplayName = "Gossip協議"),
    Count UMETA(DisplayName = "協議數量")
};

USTRUCT(BlueprintType)
struct FAIAgentDecision
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 AgentId;

    UPROPERTY(BlueprintReadWrite)
    FString DecisionType;

    UPROPERTY(BlueprintReadWrite)
    float Confidence;

    UPROPERTY(BlueprintReadWrite)
    TArray<int32> TargetIds;

    UPROPERTY(BlueprintReadWrite)
    FVector TargetLocation;

    UPROPERTY(BlueprintReadWrite)
    int32 Priority;

    UPROPERTY(BlueprintReadWrite)
    float EstimatedOutcome;

    FAIAgentDecision()
        : AgentId(0)
        , Confidence(0.0f)
        , Priority(0)
        , EstimatedOutcome(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FCollaborativeTask
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 TaskId;

    UPROPERTY(BlueprintReadWrite)
    FString TaskName;

    UPROPERTY(BlueprintReadWrite)
    FString TaskDescription;

    UPROPERTY(BlueprintReadWrite)
    TArray<int32> AssignedAgents;

    UPROPERTY(BlueprintReadWrite)
    float TaskComplexity;

    UPROPERTY(BlueprintReadWrite)
    float Progress;

    UPROPERTY(BlueprintReadWrite)
    bool bIsCompleted;

    UPROPERTY(BlueprintReadWrite)
    float CompletionTime;

    FCollaborativeTask()
        : TaskId(0)
        , TaskComplexity(1.0f)
        , Progress(0.0f)
        , bIsCompleted(false)
        , CompletionTime(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FAIAgentProfile
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 AgentId;

    UPROPERTY(BlueprintReadWrite)
    FString AgentName;

    UPROPERTY(BlueprintReadWrite)
    EAIAgentRole PrimaryRole;

    UPROPERTY(BlueprintReadWrite)
    TArray<EAIAgentRole> SecondaryRoles;

    UPROPERTY(BlueprintReadWrite)
    float Intelligence;

    UPROPERTY(BlueprintReadWrite)
    float CooperationWillingness;

    UPROPERTY(BlueprintReadWrite)
    float CommunicationEfficiency;

    UPROPERTY(BlueprintReadWrite)
    float SpecializationScore;

    UPROPERTY(BlueprintReadWrite)
    TArray<int32> TrustedAgents;

    UPROPERTY(BlueprintReadWrite)
    TMap<int32, float> AgentRelationships;

    FAIAgentProfile()
        : AgentId(0)
        , PrimaryRole(EAIAgentRole::Commander)
        , Intelligence(0.5f)
        , CooperationWillingness(0.5f)
        , CommunicationEfficiency(0.5f)
        , SpecializationScore(0.5f)
    {}
};

USTRUCT(BlueprintType)
struct FCollaborativeNetworkStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 TotalAgents;

    UPROPERTY(BlueprintReadOnly)
    int32 ActiveCollaborations;

    UPROPERTY(BlueprintReadOnly)
    int32 CompletedTasks;

    UPROPERTY(BlueprintReadOnly)
    float AverageCollaborationEfficiency;

    UPROPERTY(BlueprintReadOnly)
    float NetworkCohesion;

    UPROPERTY(BlueprintReadOnly)
    float CommunicationLatency;

    UPROPERTY(BlueprintReadOnly)
    int32 MessagesExchanged;

    FCollaborativeNetworkStats()
        : TotalAgents(0)
        , ActiveCollaborations(0)
        , CompletedTasks(0)
        , AverageCollaborationEfficiency(0.0f)
        , NetworkCohesion(0.0f)
        , CommunicationLatency(0.0f)
        , MessagesExchanged(0)
    {}
};

/**
 * 協作AI網絡 - 多AI智能體協作系統
 */
UCLASS(ClassGroup = (MingAI), BlueprintType, Blueprintable)
class MINGAI_API UMingRTSCollaborativeAINetwork : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCollaborativeAINetwork();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|System")
    void InitializeCollaborativeNetwork(int32 MaxAgents = 100);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|System")
    void ShutdownCollaborativeNetwork();

    // AI智能體管理
    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Agents")
    int32 RegisterAIAgent(const FAIAgentProfile& Profile);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Agents")
    void UnregisterAIAgent(int32 AgentId);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Agents")
    void UpdateAgentProfile(int32 AgentId, const FAIAgentProfile& NewProfile);

    UFUNCTION(BlueprintPure, Category = "MingRTS|AI|Collaborative|Agents")
    FAIAgentProfile GetAgentProfile(int32 AgentId) const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|AI|Collaborative|Agents")
    TArray<int32> GetAllAgents() const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|AI|Collaborative|Agents")
    TArray<int32> GetAgentsByRole(EAIAgentRole Role) const;

    // 協作決策
    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Decision")
    FAIAgentDecision MakeCollaborativeDecision(int32 AgentId, const FString& DecisionContext);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Decision")
    TArray<FAIAgentDecision> ReachConsensus(const TArray<int32>& AgentIds, const FString& DecisionTopic);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Decision")
    float EvaluateDecisionQuality(const FAIAgentDecision& Decision);

    // 任務分配與協作
    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Tasks")
    int32 CreateCollaborativeTask(const FString& TaskName, const FString& Description, float Complexity);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Tasks")
    void AssignTaskToAgents(int32 TaskId, const TArray<int32>& AgentIds);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Tasks")
    void UpdateTaskProgress(int32 TaskId, float ProgressDelta);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Tasks")
    void CompleteTask(int32 TaskId);

    UFUNCTION(BlueprintPure, Category = "MingRTS|AI|Collaborative|Tasks")
    FCollaborativeTask GetTaskInfo(int32 TaskId) const;

    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Tasks")
    TArray<int32> FindOptimalTaskAssignment(int32 TaskId);

    // 通信協議
    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Communication")
    void SetCommunicationProtocol(ECommunicationProtocol Protocol);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Communication")
    void SendMessage(int32 SenderId, int32 ReceiverId, const FString& Message, const FString& MessageType);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Communication")
    void BroadcastMessage(int32 SenderId, const TArray<int32>& ReceiverIds, const FString& Message);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Communication")
    void ProcessMessageQueue();

    // 協作關係管理
    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Relations")
    void UpdateAgentRelationship(int32 AgentA, int32 AgentB, float RelationshipValue);

    UFUNCTION(BlueprintPure, Category = "MingRTS|AI|Collaborative|Relations")
    float GetAgentRelationship(int32 AgentA, int32 AgentB) const;

    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Relations")
    void FormCollaborationGroup(const TArray<int32>& AgentIds, ECollaborationType CollaborationType);

    UFUNCTION(BlueprintPure, Category = "MingRTS|AI|Collaborative|Relations")
    TArray<int32> GetCollaborationGroup(int32 AgentId) const;

    // 集體智能算法
    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Swarm")
    FVector CalculateSwarmMovement(const TArray<int32>& AgentIds, FVector TargetPosition);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Swarm")
    void ExecuteDistributedAlgorithm(const FString& AlgorithmName, const TArray<int32>& ParticipatingAgents);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Swarm")
    float CalculateCollectiveIntelligence(const TArray<int32>& AgentIds);

    // 統計與分析
    UFUNCTION(BlueprintPure, Category = "MingRTS|AI|Collaborative|Analytics")
    FCollaborativeNetworkStats GetNetworkStatistics() const;

    UFUNCTION(BlueprintCallable, Category = "MingRTS|AI|Collaborative|Analytics")
    void GenerateCollaborationReport(const FString& ReportPath);

    UFUNCTION(BlueprintPure, Category = "MingRTS|AI|Collaborative|Analytics")
    float CalculateNetworkEfficiency() const;

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "MingRTS|AI|Collaborative|Events")
    FOnAgentRegistered OnAgentRegistered;

    UPROPERTY(BlueprintAssignable, Category = "MingRTS|AI|Collaborative|Events")
    FOnTaskCompleted OnTaskCompleted;

    UPROPERTY(BlueprintAssignable, Category = "MingRTS|AI|Collaborative|Events")
    FOnConsensusReached OnConsensusReached;

private:
    UPROPERTY()
    TMap<int32, FAIAgentProfile> AgentRegistry;

    UPROPERTY()
    TMap<int32, FCollaborativeTask> TaskRegistry;

    UPROPERTY()
    TMap<int32, TArray<int32>> CollaborationGroups;

    UPROPERTY()
    ECommunicationProtocol CurrentProtocol;

    UPROPERTY()
    int32 NextAgentId;

    UPROPERTY()
    int32 NextTaskId;

    UPROPERTY()
    int32 MaxAgentCount;

    // 消息隊列
    struct FPendingMessage
    {
        int32 SenderId;
        int32 ReceiverId;
        FString Message;
        FString MessageType;
        float Timestamp;
    };
    TArray<FPendingMessage> MessageQueue;

    // 內部函數
    float CalculateCollaborationEfficiency(const TArray<int32>& AgentIds) const;
    float CalculateRoleCompatibility(EAIAgentRole RoleA, EAIAgentRole RoleB) const;
    void PropagateInformation(int32 SourceAgent, const FString& Information);
    void ResolveConflicts(const TArray<int32>& ConflictingAgents);
    void UpdateNetworkCohesion();
};

// 事件委託聲明
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAgentRegistered, int32, AgentId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTaskCompleted, int32, TaskId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConsensusReached, TArray<int32>, AgentIds, FString, Decision);
