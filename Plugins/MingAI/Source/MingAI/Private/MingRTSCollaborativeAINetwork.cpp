#include "MingRTSCollaborativeAINetwork.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"

UMingRTSCollaborativeAINetwork::UMingRTSCollaborativeAINetwork()
    : CurrentProtocol(ECommunicationProtocol::Mesh)
    , NextAgentId(1)
    , NextTaskId(1)
    , MaxAgentCount(100)
{
}

void UMingRTSCollaborativeAINetwork::InitializeCollaborativeNetwork(int32 MaxAgents)
{
    AgentRegistry.Empty();
    TaskRegistry.Empty();
    CollaborationGroups.Empty();
    MessageQueue.Empty();
    MaxAgentCount = MaxAgents;
    NextAgentId = 1;
    NextTaskId = 1;
    CurrentProtocol = ECommunicationProtocol::Mesh;
}

void UMingRTSCollaborativeAINetwork::ShutdownCollaborativeNetwork()
{
    AgentRegistry.Empty();
    TaskRegistry.Empty();
    CollaborationGroups.Empty();
    MessageQueue.Empty();
}

int32 UMingRTSCollaborativeAINetwork::RegisterAIAgent(const FAIAgentProfile& Profile)
{
    if (AgentRegistry.Num() >= MaxAgentCount)
    {
        UE_LOG(LogTemp, Warning, TEXT("Collaborative AI Network has reached maximum agent capacity"));
        return -1;
    }

    int32 NewAgentId = NextAgentId++;
    FAIAgentProfile NewProfile = Profile;
    NewProfile.AgentId = NewAgentId;
    AgentRegistry.Add(NewAgentId, NewProfile);

    OnAgentRegistered.Broadcast(NewAgentId);
    return NewAgentId;
}

void UMingRTSCollaborativeAINetwork::UnregisterAIAgent(int32 AgentId)
{
    if (AgentRegistry.Contains(AgentId))
    {
        // 移除該智能體參與的所有協作組
        for (auto& Pair : CollaborationGroups)
        {
            Pair.Value.Remove(AgentId);
        }

        // 移除該智能體的任務分配
        for (auto& Pair : TaskRegistry)
        {
            Pair.Value.AssignedAgents.Remove(AgentId);
        }

        AgentRegistry.Remove(AgentId);
    }
}

void UMingRTSCollaborativeAINetwork::UpdateAgentProfile(int32 AgentId, const FAIAgentProfile& NewProfile)
{
    if (AgentRegistry.Contains(AgentId))
    {
        FAIAgentProfile UpdatedProfile = NewProfile;
        UpdatedProfile.AgentId = AgentId;
        AgentRegistry[AgentId] = UpdatedProfile;
    }
}

FAIAgentProfile UMingRTSCollaborativeAINetwork::GetAgentProfile(int32 AgentId) const
{
    if (const FAIAgentProfile* Profile = AgentRegistry.Find(AgentId))
    {
        return *Profile;
    }
    return FAIAgentProfile();
}

TArray<int32> UMingRTSCollaborativeAINetwork::GetAllAgents() const
{
    TArray<int32> Result;
    AgentRegistry.GetKeys(Result);
    return Result;
}

TArray<int32> UMingRTSCollaborativeAINetwork::GetAgentsByRole(EAIAgentRole Role) const
{
    TArray<int32> Result;
    for (const auto& Pair : AgentRegistry)
    {
        if (Pair.Value.PrimaryRole == Role || Pair.Value.SecondaryRoles.Contains(Role))
        {
            Result.Add(Pair.Key);
        }
    }
    return Result;
}

FAIAgentDecision UMingRTSCollaborativeAINetwork::MakeCollaborativeDecision(int32 AgentId, const FString& DecisionContext)
{
    FAIAgentDecision Decision;
    Decision.AgentId = AgentId;
    Decision.DecisionType = DecisionContext;

    if (!AgentRegistry.Contains(AgentId))
    {
        Decision.Confidence = 0.0f;
        return Decision;
    }

    const FAIAgentProfile& Profile = AgentRegistry[AgentId];

    // 基於智能體屬性計算決策信心度
    Decision.Confidence = Profile.Intelligence * 0.4f + 
                          Profile.SpecializationScore * 0.3f + 
                          Profile.CooperationWillingness * 0.3f;

    // 查找協作組中的其他智能體
    TArray<int32> GroupMembers = GetCollaborationGroup(AgentId);
    if (GroupMembers.Num() > 1)
    {
        // 群體決策提升信心度
        Decision.Confidence = FMath::Min(1.0f, Decision.Confidence * (1.0f + (GroupMembers.Num() - 1) * 0.1f));

        // 協商目標
        FVector ConsensusTarget = FVector::ZeroVector;
        int32 ValidTargets = 0;
        for (int32 MemberId : GroupMembers)
        {
            if (AgentRegistry.Contains(MemberId))
            {
                // 這裡可以基於智能體位置計算共識目標
                ConsensusTarget += FVector(MemberId * 100.0f, MemberId * 100.0f, 0.0f);
                ValidTargets++;
            }
        }
        if (ValidTargets > 0)
        {
            Decision.TargetLocation = ConsensusTarget / ValidTargets;
        }

        Decision.TargetIds = GroupMembers;
    }

    Decision.Priority = FMath::RoundToInt(Decision.Confidence * 10);
    Decision.EstimatedOutcome = Decision.Confidence * 100.0f;

    return Decision;
}

TArray<FAIAgentDecision> UMingRTSCollaborativeAINetwork::ReachConsensus(const TArray<int32>& AgentIds, const FString& DecisionTopic)
{
    TArray<FAIAgentDecision> ConsensusDecisions;

    if (AgentIds.Num() == 0)
    {
        return ConsensusDecisions;
    }

    // 收集所有智能體的決策
    TMap<FString, TArray<FAIAgentDecision>> DecisionGroups;
    for (int32 AgentId : AgentIds)
    {
        FAIAgentDecision Decision = MakeCollaborativeDecision(AgentId, DecisionTopic);
        DecisionGroups.FindOrAdd(Decision.DecisionType).Add(Decision);
    }

    // 找出最受支持的決策
    FString BestDecisionType;
    int32 MaxSupport = 0;
    float HighestConfidence = 0.0f;

    for (const auto& Pair : DecisionGroups)
    {
        int32 Support = Pair.Value.Num();
        float AvgConfidence = 0.0f;
        for (const auto& Decision : Pair.Value)
        {
            AvgConfidence += Decision.Confidence;
        }
        AvgConfidence /= Pair.Value.Num();

        if (Support > MaxSupport || (Support == MaxSupport && AvgConfidence > HighestConfidence))
        {
            MaxSupport = Support;
            BestDecisionType = Pair.Key;
            HighestConfidence = AvgConfidence;
        }
    }

    // 返回達成共識的決策
    if (DecisionGroups.Contains(BestDecisionType))
    {
        ConsensusDecisions = DecisionGroups[BestDecisionType];
    }

    if (ConsensusDecisions.Num() > 0)
    {
        OnConsensusReached.Broadcast(AgentIds, BestDecisionType);
    }

    return ConsensusDecisions;
}

float UMingRTSCollaborativeAINetwork::EvaluateDecisionQuality(const FAIAgentDecision& Decision)
{
    float Quality = Decision.Confidence * 0.4f;
    Quality += (Decision.TargetIds.Num() > 0 ? 0.3f : 0.0f);
    Quality += (Decision.Priority / 10.0f) * 0.2f;
    Quality += (Decision.EstimatedOutcome / 100.0f) * 0.1f;
    return FMath::Clamp(Quality, 0.0f, 1.0f);
}

int32 UMingRTSCollaborativeAINetwork::CreateCollaborativeTask(const FString& TaskName, const FString& Description, float Complexity)
{
    int32 NewTaskId = NextTaskId++;
    FCollaborativeTask NewTask;
    NewTask.TaskId = NewTaskId;
    NewTask.TaskName = TaskName;
    NewTask.TaskDescription = Description;
    NewTask.TaskComplexity = FMath::Clamp(Complexity, 0.1f, 10.0f);
    TaskRegistry.Add(NewTaskId, NewTask);
    return NewTaskId;
}

void UMingRTSCollaborativeAINetwork::AssignTaskToAgents(int32 TaskId, const TArray<int32>& AgentIds)
{
    if (FCollaborativeTask* Task = TaskRegistry.Find(TaskId))
    {
        Task->AssignedAgents = AgentIds;
    }
}

void UMingRTSCollaborativeAINetwork::UpdateTaskProgress(int32 TaskId, float ProgressDelta)
{
    if (FCollaborativeTask* Task = TaskRegistry.Find(TaskId))
    {
        Task->Progress = FMath::Clamp(Task->Progress + ProgressDelta, 0.0f, 1.0f);
        if (Task->Progress >= 1.0f && !Task->bIsCompleted)
        {
            CompleteTask(TaskId);
        }
    }
}

void UMingRTSCollaborativeAINetwork::CompleteTask(int32 TaskId)
{
    if (FCollaborativeTask* Task = TaskRegistry.Find(TaskId))
    {
        Task->bIsCompleted = true;
        Task->Progress = 1.0f;
        Task->CompletionTime = FPlatformTime::Seconds();
        OnTaskCompleted.Broadcast(TaskId);
    }
}

FCollaborativeTask UMingRTSCollaborativeAINetwork::GetTaskInfo(int32 TaskId) const
{
    if (const FCollaborativeTask* Task = TaskRegistry.Find(TaskId))
    {
        return *Task;
    }
    return FCollaborativeTask();
}

TArray<int32> UMingRTSCollaborativeAINetwork::FindOptimalTaskAssignment(int32 TaskId)
{
    TArray<int32> OptimalAssignment;
    const FCollaborativeTask* Task = TaskRegistry.Find(TaskId);
    if (!Task)
    {
        return OptimalAssignment;
    }

    // 計算需要的智能體數量（基於任務複雜度）
    int32 RequiredAgents = FMath::CeilToInt(Task->TaskComplexity);

    // 根據任務類型選擇合適角色的智能體
    TArray<int32> Candidates = GetAllAgents();
    TArray<TPair<int32, float>> ScoredCandidates;

    for (int32 AgentId : Candidates)
    {
        if (AgentRegistry.Contains(AgentId))
        {
            const FAIAgentProfile& Profile = AgentRegistry[AgentId];
            float Score = Profile.Intelligence * 0.3f + 
                          Profile.CooperationWillingness * 0.3f +
                          Profile.CommunicationEfficiency * 0.2f +
                          Profile.SpecializationScore * 0.2f;
            ScoredCandidates.Add(TPair<int32, float>(AgentId, Score));
        }
    }

    // 按分數排序
    ScoredCandidates.Sort([](const TPair<int32, float>& A, const TPair<int32, float>& B) {
        return A.Value > B.Value;
    });

    // 選擇最佳智能體
    for (int32 i = 0; i < FMath::Min(RequiredAgents, ScoredCandidates.Num()); ++i)
    {
        OptimalAssignment.Add(ScoredCandidates[i].Key);
    }

    return OptimalAssignment;
}

void UMingRTSCollaborativeAINetwork::SetCommunicationProtocol(ECommunicationProtocol Protocol)
{
    CurrentProtocol = Protocol;
}

void UMingRTSCollaborativeAINetwork::SendMessage(int32 SenderId, int32 ReceiverId, const FString& Message, const FString& MessageType)
{
    if (!AgentRegistry.Contains(SenderId) || !AgentRegistry.Contains(ReceiverId))
    {
        return;
    }

    FPendingMessage PendingMessage;
    PendingMessage.SenderId = SenderId;
    PendingMessage.ReceiverId = ReceiverId;
    PendingMessage.Message = Message;
    PendingMessage.MessageType = MessageType;
    PendingMessage.Timestamp = FPlatformTime::Seconds();
    MessageQueue.Add(PendingMessage);
}

void UMingRTSCollaborativeAINetwork::BroadcastMessage(int32 SenderId, const TArray<int32>& ReceiverIds, const FString& Message)
{
    for (int32 ReceiverId : ReceiverIds)
    {
        SendMessage(SenderId, ReceiverId, Message, TEXT("Broadcast"));
    }
}

void UMingRTSCollaborativeAINetwork::ProcessMessageQueue()
{
    // 根據通信協議處理消息
    switch (CurrentProtocol)
    {
    case ECommunicationProtocol::Direct:
        // 直接通信，立即處理
        for (const auto& Msg : MessageQueue)
        {
            // 處理消息
            UE_LOG(LogTemp, Log, TEXT("Direct Message from %d to %d: %s"), 
                Msg.SenderId, Msg.ReceiverId, *Msg.Message);
        }
        break;

    case ECommunicationProtocol::Gossip:
        // Gossip協議，消息會傳播給鄰居
        for (const auto& Msg : MessageQueue)
        {
            PropagateInformation(Msg.SenderId, Msg.Message);
        }
        break;

    case ECommunicationProtocol::Hierarchical:
        // 層級協議，消息向上傳遞
        // 實現層級處理邏輯
        break;

    default:
        break;
    }

    MessageQueue.Empty();
}

void UMingRTSCollaborativeAINetwork::UpdateAgentRelationship(int32 AgentA, int32 AgentB, float RelationshipValue)
{
    if (AgentRegistry.Contains(AgentA) && AgentRegistry.Contains(AgentB))
    {
        float ClampedValue = FMath::Clamp(RelationshipValue, -1.0f, 1.0f);
        AgentRegistry[AgentA].AgentRelationships.Add(AgentB, ClampedValue);
        AgentRegistry[AgentB].AgentRelationships.Add(AgentA, ClampedValue);
    }
}

float UMingRTSCollaborativeAINetwork::GetAgentRelationship(int32 AgentA, int32 AgentB) const
{
    if (const FAIAgentProfile* ProfileA = AgentRegistry.Find(AgentA))
    {
        if (const float* Value = ProfileA->AgentRelationships.Find(AgentB))
        {
            return *Value;
        }
    }
    return 0.0f;
}

void UMingRTSCollaborativeAINetwork::FormCollaborationGroup(const TArray<int32>& AgentIds, ECollaborationType CollaborationType)
{
    if (AgentIds.Num() < 2)
    {
        return;
    }

    // 為每個智能體記錄協作組
    for (int32 AgentId : AgentIds)
    {
        if (AgentRegistry.Contains(AgentId))
        {
            CollaborationGroups.Add(AgentId, AgentIds);
        }
    }

    // 更新智能體間的關係（協作提升關係）
    for (int32 i = 0; i < AgentIds.Num(); ++i)
    {
        for (int32 j = i + 1; j < AgentIds.Num(); ++j)
        {
            float CurrentRelation = GetAgentRelationship(AgentIds[i], AgentIds[j]);
            UpdateAgentRelationship(AgentIds[i], AgentIds[j], CurrentRelation + 0.1f);
        }
    }
}

TArray<int32> UMingRTSCollaborativeAINetwork::GetCollaborationGroup(int32 AgentId) const
{
    if (const TArray<int32>* Group = CollaborationGroups.Find(AgentId))
    {
        return *Group;
    }
    return TArray<int32>();
}

FVector UMingRTSCollaborativeAINetwork::CalculateSwarmMovement(const TArray<int32>& AgentIds, FVector TargetPosition)
{
    if (AgentIds.Num() == 0)
    {
        return TargetPosition;
    }

    FVector Centroid = FVector::ZeroVector;
    FVector AverageVelocity = FVector::ZeroVector;

    for (int32 AgentId : AgentIds)
    {
        // 計算智能體的當前位置（這裡使用簡化的示例）
        FVector AgentPosition = FVector(AgentId * 100.0f, AgentId * 100.0f, 0.0f);
        Centroid += AgentPosition;
    }

    Centroid /= AgentIds.Num();

    // 群體運動規則：
    // 1. 分離（避免碰撞）
    // 2. 對齊（朝向相同方向）
    // 3. 聚合（向群體中心移動）
    // 4. 目標追蹤

    FVector MoveDirection = (TargetPosition - Centroid).GetSafeNormal();
    FVector SwarmTarget = TargetPosition + (Centroid - TargetPosition) * 0.2f;

    return SwarmTarget;
}

void UMingRTSCollaborativeAINetwork::ExecuteDistributedAlgorithm(const FString& AlgorithmName, const TArray<int32>& ParticipatingAgents)
{
    if (AlgorithmName == TEXT("LeaderElection"))
    {
        // 領導者選舉算法
        int32 BestLeader = -1;
        float BestScore = -1.0f;

        for (int32 AgentId : ParticipatingAgents)
        {
            if (AgentRegistry.Contains(AgentId))
            {
                const FAIAgentProfile& Profile = AgentRegistry[AgentId];
                float Score = Profile.Intelligence + Profile.CommunicationEfficiency;
                if (Profile.PrimaryRole == EAIAgentRole::Commander)
                {
                    Score += 2.0f;
                }

                if (Score > BestScore)
                {
                    BestScore = Score;
                    BestLeader = AgentId;
                }
            }
        }

        UE_LOG(LogTemp, Log, TEXT("Leader Election Result: Agent %d"), BestLeader);
    }
    else if (AlgorithmName == TEXT("Consensus"))
    {
        // 共識算法
        ReachConsensus(ParticipatingAgents, TEXT("DistributedConsensus"));
    }
    else if (AlgorithmName == TEXT("TaskAllocation"))
    {
        // 任務分配算法
        for (const auto& Pair : TaskRegistry)
        {
            if (Pair.Value.AssignedAgents.Num() == 0)
            {
                TArray<int32> Assignment = FindOptimalTaskAssignment(Pair.Key);
                AssignTaskToAgents(Pair.Key, Assignment);
            }
        }
    }
}

float UMingRTSCollaborativeAINetwork::CalculateCollectiveIntelligence(const TArray<int32>& AgentIds)
{
    if (AgentIds.Num() == 0)
    {
        return 0.0f;
    }

    float TotalIntelligence = 0.0f;
    float TotalCooperation = 0.0f;
    float TotalCommunication = 0.0f;

    for (int32 AgentId : AgentIds)
    {
        if (AgentRegistry.Contains(AgentId))
        {
            const FAIAgentProfile& Profile = AgentRegistry[AgentId];
            TotalIntelligence += Profile.Intelligence;
            TotalCooperation += Profile.CooperationWillingness;
            TotalCommunication += Profile.CommunicationEfficiency;
        }
    }

    int32 ValidAgents = AgentIds.Num();
    float AvgIntelligence = TotalIntelligence / ValidAgents;
    float AvgCooperation = TotalCooperation / ValidAgents;
    float AvgCommunication = TotalCommunication / ValidAgents;

    // 集體智能 = 平均智能 × 協作意願 × 通信效率 × 多樣性加成
    float Diversity = FMath::Min(1.0f, AgentIds.Num() / 10.0f);
    float CollectiveIntelligence = AvgIntelligence * AvgCooperation * AvgCommunication * (1.0f + Diversity * 0.2f);

    return FMath::Clamp(CollectiveIntelligence, 0.0f, 1.0f);
}

FCollaborativeNetworkStats UMingRTSCollaborativeAINetwork::GetNetworkStatistics() const
{
    FCollaborativeNetworkStats Stats;
    Stats.TotalAgents = AgentRegistry.Num();
    Stats.ActiveCollaborations = CollaborationGroups.Num();
    Stats.CompletedTasks = 0;

    for (const auto& Pair : TaskRegistry)
    {
        if (Pair.Value.bIsCompleted)
        {
            Stats.CompletedTasks++;
        }
    }

    // 計算平均協作效率
    float TotalEfficiency = 0.0f;
    int32 GroupCount = 0;
    for (const auto& Pair : CollaborationGroups)
    {
        TArray<int32> UniqueGroup = Pair.Value;
        float Efficiency = CalculateCollaborationEfficiency(UniqueGroup);
        TotalEfficiency += Efficiency;
        GroupCount++;
    }

    if (GroupCount > 0)
    {
        Stats.AverageCollaborationEfficiency = TotalEfficiency / GroupCount;
    }

    Stats.MessagesExchanged = 0; // 可以從消息隊列統計
    Stats.NetworkCohesion = CalculateNetworkEfficiency();

    return Stats;
}

void UMingRTSCollaborativeAINetwork::GenerateCollaborationReport(const FString& ReportPath)
{
    FCollaborativeNetworkStats Stats = GetNetworkStatistics();

    FString Report = FString::Printf(
        TEXT("=== 協作AI網絡報告 ===\n")
        TEXT("總智能體數: %d\n")
        TEXT("活躍協作組: %d\n")
        TEXT("完成任務數: %d\n")
        TEXT("平均協作效率: %.2f\n")
        TEXT("網絡凝聚力: %.2f\n")
        TEXT("通信延遲: %.2f ms\n")
        TEXT("消息交換數: %d\n"),
        Stats.TotalAgents,
        Stats.ActiveCollaborations,
        Stats.CompletedTasks,
        Stats.AverageCollaborationEfficiency,
        Stats.NetworkCohesion,
        Stats.CommunicationLatency,
        Stats.MessagesExchanged
    );

    // 這裡可以將報告保存到文件
    UE_LOG(LogTemp, Log, TEXT("%s"), *Report);
}

float UMingRTSCollaborativeAINetwork::CalculateNetworkEfficiency() const
{
    if (AgentRegistry.Num() == 0)
    {
        return 0.0f;
    }

    // 計算網絡效率基於多個因素
    float AgentCountFactor = FMath::Min(1.0f, AgentRegistry.Num() / (float)MaxAgentCount);
    float CollaborationFactor = CollaborationGroups.Num() / FMath::Max(1.0f, AgentRegistry.Num() / 3.0f);
    float TaskCompletionFactor = 0.0f;

    if (TaskRegistry.Num() > 0)
    {
        int32 CompletedCount = 0;
        for (const auto& Pair : TaskRegistry)
        {
            if (Pair.Value.bIsCompleted)
            {
                CompletedCount++;
            }
        }
        TaskCompletionFactor = CompletedCount / (float)TaskRegistry.Num();
    }

    return (AgentCountFactor * 0.3f + CollaborationFactor * 0.4f + TaskCompletionFactor * 0.3f);
}

float UMingRTSCollaborativeAINetwork::CalculateCollaborationEfficiency(const TArray<int32>& AgentIds) const
{
    if (AgentIds.Num() < 2)
    {
        return 0.0f;
    }

    float TotalEfficiency = 0.0f;
    int32 PairCount = 0;

    for (int32 i = 0; i < AgentIds.Num(); ++i)
    {
        for (int32 j = i + 1; j < AgentIds.Num(); ++j)
        {
            if (AgentRegistry.Contains(AgentIds[i]) && AgentRegistry.Contains(AgentIds[j]))
            {
                const FAIAgentProfile& ProfileA = AgentRegistry[AgentIds[i]];
                const FAIAgentProfile& ProfileB = AgentRegistry[AgentIds[j]];

                float RoleCompatibility = CalculateRoleCompatibility(ProfileA.PrimaryRole, ProfileB.PrimaryRole);
                float Relationship = GetAgentRelationship(AgentIds[i], AgentIds[j]);

                float PairEfficiency = (ProfileA.CommunicationEfficiency + ProfileB.CommunicationEfficiency) / 2.0f;
                PairEfficiency *= (1.0f + RoleCompatibility * 0.2f);
                PairEfficiency *= (1.0f + Relationship * 0.1f);

                TotalEfficiency += PairEfficiency;
                PairCount++;
            }
        }
    }

    return PairCount > 0 ? TotalEfficiency / PairCount : 0.0f;
}

float UMingRTSCollaborativeAINetwork::CalculateRoleCompatibility(EAIAgentRole RoleA, EAIAgentRole RoleB) const
{
    // 定義角色兼容性矩陣
    if (RoleA == RoleB)
    {
        return 0.5f; // 相同角色有一定兼容性
    }

    // 特殊組合的兼容性
    if ((RoleA == EAIAgentRole::Commander && RoleB == EAIAgentRole::Strategist) ||
        (RoleA == EAIAgentRole::Strategist && RoleB == EAIAgentRole::Commander))
    {
        return 1.0f;
    }

    if ((RoleA == EAIAgentRole::Attacker && RoleB == EAIAgentRole::Defender) ||
        (RoleA == EAIAgentRole::Defender && RoleB == EAIAgentRole::Attacker))
    {
        return 0.8f;
    }

    if ((RoleA == EAIAgentRole::Scout && RoleB == EAIAgentRole::Commander) ||
        (RoleA == EAIAgentRole::Commander && RoleB == EAIAgentRole::Scout))
    {
        return 0.9f;
    }

    if ((RoleA == EAIAgentRole::Economist && RoleB == EAIAgentRole::Diplomat) ||
        (RoleA == EAIAgentRole::Diplomat && RoleB == EAIAgentRole::Economist))
    {
        return 0.7f;
    }

    return 0.6f; // 默認兼容性
}

void UMingRTSCollaborativeAINetwork::PropagateInformation(int32 SourceAgent, const FString& Information)
{
    // Gossip協議實現：信息傳播給信任的鄰居
    if (!AgentRegistry.Contains(SourceAgent))
    {
        return;
    }

    const FAIAgentProfile& Profile = AgentRegistry[SourceAgent];
    for (int32 TrustedAgent : Profile.TrustedAgents)
    {
        if (AgentRegistry.Contains(TrustedAgent))
        {
            // 信息傳播（這裡可以添加衰減或變形邏輯）
            UE_LOG(LogTemp, Log, TEXT("Information propagated from %d to %d: %s"),
                SourceAgent, TrustedAgent, *Information);
        }
    }
}

void UMingRTSCollaborativeAINetwork::ResolveConflicts(const TArray<int32>& ConflictingAgents)
{
    if (ConflictingAgents.Num() < 2)
    {
        return;
    }

    // 簡單的衝突解決：尋找最適合調解的智能體
    int32 Mediator = -1;
    float BestMediatorScore = -1.0f;

    for (int32 AgentId : ConflictingAgents)
    {
        if (AgentRegistry.Contains(AgentId))
        {
            const FAIAgentProfile& Profile = AgentRegistry[AgentId];
            float Score = Profile.Intelligence * 0.3f + Profile.CooperationWillingness * 0.7f;

            if (Profile.PrimaryRole == EAIAgentRole::Diplomat || Profile.PrimaryRole == EAIAgentRole::Coordinator)
            {
                Score += 1.0f;
            }

            if (Score > BestMediatorScore)
            {
                BestMediatorScore = Score;
                Mediator = AgentId;
            }
        }
    }

    if (Mediator != -1)
    {
        UE_LOG(LogTemp, Log, TEXT("Conflict resolution: Agent %d selected as mediator"), Mediator);
        // 提升調解者與衝突方的關係
        for (int32 AgentId : ConflictingAgents)
        {
            if (AgentId != Mediator)
            {
                float CurrentRelation = GetAgentRelationship(Mediator, AgentId);
                UpdateAgentRelationship(Mediator, AgentId, CurrentRelation + 0.05f);
            }
        }
    }
}

void UMingRTSCollaborativeAINetwork::UpdateNetworkCohesion()
{
    // 這個函數可以定期調用以更新網絡凝聚力指標
    // 可以基於平均關係值、協作組數量等因素計算
}
