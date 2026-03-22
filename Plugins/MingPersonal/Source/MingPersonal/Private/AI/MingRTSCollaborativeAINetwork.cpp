#include "AI/MingRTSCollaborativeAINetwork.h"
#include "Math/UnrealMathUtility.h"

UMingRTSCollaborativeAINetwork::UMingRTSCollaborativeAINetwork()
    : bIsInitialized(false)
    , CurrentGameTime(0.0f)
    , CurrentStrategy(EAICollaborationStrategy::Cooperative)
{
}

void UMingRTSCollaborativeAINetwork::InitializeNetwork(EAICollaborationStrategy Strategy)
{
    if (bIsInitialized)
    {
        return;
    }
    
    CurrentStrategy = Strategy;
    CurrentGameTime = 0.0f;
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCollaborativeAINetwork: Initialized with strategy %d"), 
        static_cast<int32>(Strategy));
}

void UMingRTSCollaborativeAINetwork::ShutdownNetwork()
{
    if (!bIsInitialized)
    {
        return;
    }
    
    // Clear all data
    RegisteredAgents.Empty();
    ActiveTasks.Empty();
    AgentMessageQueues.Empty();
    CollectiveKnowledgeBase.Empty();
    TaskHistory.Empty();
    
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCollaborativeAINetwork: Shutdown"));
}

FString UMingRTSCollaborativeAINetwork::RegisterAgent(const FString& AgentName, EAIAgentRole PrimaryRole)
{
    FString AgentID = GenerateAgentID();
    
    FAIAgentData NewAgent;
    NewAgent.AgentID = AgentID;
    NewAgent.AgentName = AgentName;
    NewAgent.PrimaryRole = PrimaryRole;
    NewAgent.Status = EAIAgentStatus::Idle;
    NewAgent.ExperienceLevel = 1.0f;
    NewAgent.EfficiencyScore = 1.0f;
    NewAgent.TrustLevel = 0.5f;
    NewAgent.LastActivityTime = CurrentGameTime;
    
    // Initialize default skills based on role
    switch (PrimaryRole)
    {
    case EAIAgentRole::Commander:
        NewAgent.SkillLevels.Add(TEXT("Leadership"), 1.0f);
        NewAgent.SkillLevels.Add(TEXT("Strategy"), 0.9f);
        NewAgent.SkillLevels.Add(TEXT("Coordination"), 0.95f);
        break;
    case EAIAgentRole::Scout:
        NewAgent.SkillLevels.Add(TEXT("Stealth"), 1.0f);
        NewAgent.SkillLevels.Add(TEXT("Speed"), 0.95f);
        NewAgent.SkillLevels.Add(TEXT("Detection"), 0.9f);
        break;
    case EAIAgentRole::Attacker:
        NewAgent.SkillLevels.Add(TEXT("Combat"), 1.0f);
        NewAgent.SkillLevels.Add(TEXT("Aggression"), 0.9f);
        NewAgent.SkillLevels.Add(TEXT("Tactics"), 0.85f);
        break;
    case EAIAgentRole::Defender:
        NewAgent.SkillLevels.Add(TEXT("Defense"), 1.0f);
        NewAgent.SkillLevels.Add(TEXT("Fortification"), 0.9f);
        NewAgent.SkillLevels.Add(TEXT("Patience"), 0.85f);
        break;
    case EAIAgentRole::Support:
        NewAgent.SkillLevels.Add(TEXT("Logistics"), 1.0f);
        NewAgent.SkillLevels.Add(TEXT("Communication"), 0.95f);
        NewAgent.SkillLevels.Add(TEXT("Adaptability"), 0.9f);
        break;
    case EAIAgentRole::ResourceGatherer:
        NewAgent.SkillLevels.Add(TEXT("Gathering"), 1.0f);
        NewAgent.SkillLevels.Add(TEXT("Efficiency"), 0.95f);
        NewAgent.SkillLevels.Add(TEXT("ResourceManagement"), 0.9f);
        break;
    case EAIAgentRole::Builder:
        NewAgent.SkillLevels.Add(TEXT("Construction"), 1.0f);
        NewAgent.SkillLevels.Add(TEXT("Planning"), 0.9f);
        NewAgent.SkillLevels.Add(TEXT("Engineering"), 0.85f);
        break;
    case EAIAgentRole::Healer:
        NewAgent.SkillLevels.Add(TEXT("Healing"), 1.0f);
        NewAgent.SkillLevels.Add(TEXT("Support"), 0.95f);
        NewAgent.SkillLevels.Add(TEXT("Protection"), 0.9f);
        break;
    case EAIAgentRole::Specialist:
        NewAgent.SkillLevels.Add(TEXT("Specialization"), 1.0f);
        NewAgent.SkillLevels.Add(TEXT("Expertise"), 0.95f);
        NewAgent.SkillLevels.Add(TEXT("Precision"), 0.9f);
        break;
    case EAIAgentRole::Coordinator:
        NewAgent.SkillLevels.Add(TEXT("Organization"), 1.0f);
        NewAgent.SkillLevels.Add(TEXT("Communication"), 0.95f);
        NewAgent.SkillLevels.Add(TEXT("Planning"), 0.9f);
        break;
    }
    
    RegisteredAgents.Add(AgentID, NewAgent);
    AgentMessageQueues.Add(AgentID, TArray<FAIAgentMessage>());
    
    OnAgentJoined.Broadcast(AgentID, PrimaryRole);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCollaborativeAINetwork: Registered agent %s (%s) with role %d"),
        *AgentID, *AgentName, static_cast<int32>(PrimaryRole));
    
    return AgentID;
}

void UMingRTSCollaborativeAINetwork::UnregisterAgent(const FString& AgentID)
{
    if (!RegisteredAgents.Contains(AgentID))
    {
        return;
    }
    
    // Remove from all tasks
    for (auto& TaskPair : ActiveTasks)
    {
        FAICollaborativeTask& Task = TaskPair.Value;
        Task.AssignedAgents.Remove(AgentID);
        Task.AgentContributions.Remove(AgentID);
    }
    
    // Remove from other agents' connections
    for (auto& AgentPair : RegisteredAgents)
    {
        FAIAgentData& Agent = AgentPair.Value;
        Agent.ConnectedAgents.Remove(AgentID);
    }
    
    // Remove messages
    AgentMessageQueues.Remove(AgentID);
    
    // Remove agent
    RegisteredAgents.Remove(AgentID);
    
    OnAgentLeft.Broadcast(AgentID);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCollaborativeAINetwork: Unregistered agent %s"), *AgentID);
}

void UMingRTSCollaborativeAINetwork::UpdateAgentStatus(const FString& AgentID, EAIAgentStatus NewStatus)
{
    if (!RegisteredAgents.Contains(AgentID))
    {
        return;
    }
    
    FAIAgentData& Agent = RegisteredAgents[AgentID];
    Agent.Status = NewStatus;
    Agent.LastActivityTime = CurrentGameTime;
    
    UE_LOG(LogTemp, Verbose, TEXT("MingRTSCollaborativeAINetwork: Agent %s status updated to %d"),
        *AgentID, static_cast<int32>(NewStatus));
}

void UMingRTSCollaborativeAINetwork::SetAgentRole(const FString& AgentID, EAIAgentRole NewRole)
{
    if (!RegisteredAgents.Contains(AgentID))
    {
        return;
    }
    
    FAIAgentData& Agent = RegisteredAgents[AgentID];
    
    // Add current role to secondary roles if not already there
    if (!Agent.SecondaryRoles.Contains(Agent.PrimaryRole))
    {
        Agent.SecondaryRoles.Add(Agent.PrimaryRole);
    }
    
    Agent.PrimaryRole = NewRole;
    Agent.LastActivityTime = CurrentGameTime;
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCollaborativeAINetwork: Agent %s role changed to %d"),
        *AgentID, static_cast<int32>(NewRole));
}

FAIAgentData UMingRTSCollaborativeAINetwork::GetAgentData(const FString& AgentID) const
{
    if (RegisteredAgents.Contains(AgentID))
    {
        return RegisteredAgents[AgentID];
    }
    return FAIAgentData();
}

TArray<FString> UMingRTSCollaborativeAINetwork::GetAllAgentIDs() const
{
    TArray<FString> AgentIDs;
    RegisteredAgents.GetKeys(AgentIDs);
    return AgentIDs;
}

TArray<FString> UMingRTSCollaborativeAINetwork::GetAgentsByRole(EAIAgentRole Role) const
{
    TArray<FString> MatchingAgents;
    
    for (const auto& AgentPair : RegisteredAgents)
    {
        const FAIAgentData& Agent = AgentPair.Value;
        if (Agent.PrimaryRole == Role || Agent.SecondaryRoles.Contains(Role))
        {
            MatchingAgents.Add(Agent.AgentID);
        }
    }
    
    return MatchingAgents;
}

void UMingRTSCollaborativeAINetwork::SendMessage(const FString& SenderID, const TArray<FString>& RecipientIDs,
    EAICommunicationType Type, const FString& Content, float Priority)
{
    if (!RegisteredAgents.Contains(SenderID))
    {
        return;
    }
    
    FAIAgentMessage Message;
    Message.MessageID = GenerateMessageID();
    Message.SenderID = SenderID;
    Message.RecipientIDs = RecipientIDs;
    Message.CommunicationType = Type;
    Message.MessageContent = Content;
    Message.Timestamp = CurrentGameTime;
    Message.Priority = Priority;
    Message.bRequiresAcknowledgment = (Type == EAICommunicationType::Emergency || 
                                        Type == EAICommunicationType::Strategy);
    
    // Route the message
    RouteMessage(Message);
    
    // Update sender status
    UpdateAgentStatus(SenderID, EAIAgentStatus::Communicating);
    
    UE_LOG(LogTemp, Verbose, TEXT("MingRTSCollaborativeAINetwork: Message %s sent from %s to %d recipients"),
        *Message.MessageID, *SenderID, RecipientIDs.Num());
}

void UMingRTSCollaborativeAINetwork::BroadcastMessage(const FString& SenderID, EAICommunicationType Type,
    const FString& Content, float Priority)
{
    // Get all agent IDs except sender
    TArray<FString> AllAgents = GetAllAgentIDs();
    AllAgents.Remove(SenderID);
    
    SendMessage(SenderID, AllAgents, Type, Content, Priority);
}

void UMingRTSCollaborativeAINetwork::SendEmergencyMessage(const FString& SenderID, const FString& Content)
{
    TArray<FString> AllAgents = GetAllAgentIDs();
    
    FAIAgentMessage Message;
    Message.MessageID = GenerateMessageID();
    Message.SenderID = SenderID;
    Message.RecipientIDs = AllAgents;
    Message.CommunicationType = EAICommunicationType::Emergency;
    Message.MessageContent = Content;
    Message.Timestamp = CurrentGameTime;
    Message.Priority = 10.0f; // Highest priority
    Message.bRequiresAcknowledgment = true;
    Message.ExpirationTime = 5.0f; // Short expiration for emergencies
    
    // Immediate delivery for emergencies
    for (const FString& AgentID : AllAgents)
    {
        if (AgentMessageQueues.Contains(AgentID))
        {
            AgentMessageQueues[AgentID].Insert(Message, 0); // Insert at front
            OnMessageReceived.Broadcast(AgentID, Message);
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("MingRTSCollaborativeAINetwork: EMERGENCY message sent from %s: %s"),
        *SenderID, *Content);
}

TArray<FAIAgentMessage> UMingRTSCollaborativeAINetwork::GetAgentMessages(const FString& AgentID) const
{
    if (AgentMessageQueues.Contains(AgentID))
    {
        return AgentMessageQueues[AgentID];
    }
    return TArray<FAIAgentMessage>();
}

void UMingRTSCollaborativeAINetwork::AcknowledgeMessage(const FString& AgentID, const FString& MessageID)
{
    if (!AgentMessageQueues.Contains(AgentID))
    {
        return;
    }
    
    auto& Messages = AgentMessageQueues[AgentID];
    for (int32 i = 0; i < Messages.Num(); ++i)
    {
        if (Messages[i].MessageID == MessageID)
        {
            Messages.RemoveAt(i);
            break;
        }
    }
}

FString UMingRTSCollaborativeAINetwork::CreateCollaborativeTask(const FString& TaskName, EAITaskPriority Priority,
    EAIAgentRole RequiredRole, int32 MinAgents, int32 MaxAgents, const FVector& TargetLocation)
{
    FString TaskID = GenerateTaskID();
    
    FAICollaborativeTask NewTask;
    NewTask.TaskID = TaskID;
    NewTask.TaskName = TaskName;
    NewTask.Priority = Priority;
    NewTask.RequiredRole = RequiredRole;
    NewTask.MinAgentsRequired = MinAgents;
    NewTask.MaxAgentsAllowed = MaxAgents;
    NewTask.TargetLocation = TargetLocation;
    NewTask.EstimatedDuration = 60.0f; // Default 60 seconds
    
    // Calculate task value based on priority
    switch (Priority)
    {
    case EAITaskPriority::Critical:
        NewTask.TaskValue = 10.0f;
        break;
    case EAITaskPriority::High:
        NewTask.TaskValue = 7.0f;
        break;
    case EAITaskPriority::Medium:
        NewTask.TaskValue = 5.0f;
        break;
    case EAITaskPriority::Low:
        NewTask.TaskValue = 3.0f;
        break;
    case EAITaskPriority::Background:
        NewTask.TaskValue = 1.0f;
        break;
    }
    
    ActiveTasks.Add(TaskID, NewTask);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCollaborativeAINetwork: Created task %s (%s), requires %d agents"),
        *TaskID, *TaskName, MinAgents);
    
    // Auto-assign if using swarm or market-based strategy
    if (CurrentStrategy == EAICollaborationStrategy::Swarm || 
        CurrentStrategy == EAICollaborationStrategy::MarketBased)
    {
        AssignTaskAutomatically(TaskID);
    }
    
    return TaskID;
}

void UMingRTSCollaborativeAINetwork::AssignTaskToAgents(const FString& TaskID, const TArray<FString>& AgentIDs)
{
    if (!ActiveTasks.Contains(TaskID))
    {
        return;
    }
    
    FAICollaborativeTask& Task = ActiveTasks[TaskID];
    
    // Validate agents
    TArray<FString> ValidAgents;
    for (const FString& AgentID : AgentIDs)
    {
        if (RegisteredAgents.Contains(AgentID))
        {
            FAIAgentData& Agent = RegisteredAgents[AgentID];
            if (Agent.Status == EAIAgentStatus::Idle || Agent.Status == EAIAgentStatus::Busy)
            {
                ValidAgents.Add(AgentID);
                Task.AssignedAgents.Add(AgentID);
                Task.AgentContributions.Add(AgentID, 0.0f);
                
                // Update agent status
                Agent.Status = EAIAgentStatus::PerformingTask;
                Agent.CurrentTaskID = TaskID;
                
                // Connect agents to each other for this task
                for (const FString& OtherAgentID : ValidAgents)
                {
                    if (OtherAgentID != AgentID)
                    {
                        Agent.ConnectedAgents.AddUnique(OtherAgentID);
                    }
                }
            }
        }
    }
    
    // Assign coordinator if multiple agents
    if (ValidAgents.Num() > 1)
    {
        // Find agent with highest coordination skill
        FString BestCoordinator = ValidAgents[0];
        float BestCoordinationScore = 0.0f;
        
        for (const FString& AgentID : ValidAgents)
        {
            FAIAgentData& Agent = RegisteredAgents[AgentID];
            float CoordinationScore = Agent.SkillLevels.Contains(TEXT("Coordination")) ? 
                Agent.SkillLevels[TEXT("Coordination")] : 0.5f;
            
            if (CoordinationScore > BestCoordinationScore)
            {
                BestCoordinationScore = CoordinationScore;
                BestCoordinator = AgentID;
            }
        }
        
        Task.CoordinatorAgentID = BestCoordinator;
    }
    else if (ValidAgents.Num() == 1)
    {
        Task.CoordinatorAgentID = ValidAgents[0];
    }
    
    OnTaskAssigned.Broadcast(TaskID, ValidAgents);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCollaborativeAINetwork: Assigned task %s to %d agents, coordinator: %s"),
        *TaskID, ValidAgents.Num(), *Task.CoordinatorAgentID);
}

void UMingRTSCollaborativeAINetwork::AssignTaskAutomatically(const FString& TaskID)
{
    if (!ActiveTasks.Contains(TaskID))
    {
        return;
    }
    
    FAICollaborativeTask& Task = ActiveTasks[TaskID];
    
    // Find best agents for this task
    TArray<FString> AvailableAgents = GetAgentsByRole(Task.RequiredRole);
    
    // Filter for idle agents
    TArray<FString> IdleAgents;
    for (const FString& AgentID : AvailableAgents)
    {
        if (RegisteredAgents[AgentID].Status == EAIAgentStatus::Idle)
        {
            IdleAgents.Add(AgentID);
        }
    }
    
    // Sort by suitability
    IdleAgents.Sort([this, &Task](const FString& A, const FString& B)
    {
        return CalculateAgentSuitability(A, Task) > CalculateAgentSuitability(B, Task);
    });
    
    // Take best agents up to MaxAgentsAllowed
    int32 NumToAssign = FMath::Min(Task.MaxAgentsAllowed, IdleAgents.Num());
    if (NumToAssign < Task.MinAgentsRequired)
    {
        // Add agents from other roles if needed
        TArray<FString> OtherAgents;
        for (const auto& AgentPair : RegisteredAgents)
        {
            if (AgentPair.Value.Status == EAIAgentStatus::Idle && !IdleAgents.Contains(AgentPair.Key))
            {
                OtherAgents.Add(AgentPair.Key);
            }
        }
        
        OtherAgents.Sort([this, &Task](const FString& A, const FString& B)
        {
            return CalculateAgentSuitability(A, Task) > CalculateAgentSuitability(B, Task);
        });
        
        while (NumToAssign < Task.MinAgentsRequired && OtherAgents.Num() > 0)
        {
            IdleAgents.Add(OtherAgents[0]);
            OtherAgents.RemoveAt(0);
            NumToAssign++;
        }
    }
    
    if (NumToAssign >= Task.MinAgentsRequired)
    {
        TArray<FString> AgentsToAssign;
        for (int32 i = 0; i < NumToAssign; ++i)
        {
            AgentsToAssign.Add(IdleAgents[i]);
        }
        
        AssignTaskToAgents(TaskID, AgentsToAssign);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSCollaborativeAINetwork: Could not assign task %s - insufficient agents"),
            *TaskID);
    }
}

void UMingRTSCollaborativeAINetwork::UpdateTaskProgress(const FString& TaskID, float Progress)
{
    if (!ActiveTasks.Contains(TaskID))
    {
        return;
    }
    
    FAICollaborativeTask& Task = ActiveTasks[TaskID];
    Task.Progress = FMath::Clamp(Progress, 0.0f, 1.0f);
    
    // Distribute progress among agents
    if (Task.AssignedAgents.Num() > 0)
    {
        float ProgressPerAgent = Progress / Task.AssignedAgents.Num();
        for (const FString& AgentID : Task.AssignedAgents)
        {
            Task.AgentContributions[AgentID] += ProgressPerAgent;
        }
    }
    
    if (Task.Progress >= 1.0f)
    {
        CompleteTask(TaskID);
    }
}

void UMingRTSCollaborativeAINetwork::CompleteTask(const FString& TaskID)
{
    if (!ActiveTasks.Contains(TaskID))
    {
        return;
    }
    
    FAICollaborativeTask& Task = ActiveTasks[TaskID];
    Task.bIsComplete = true;
    Task.Progress = 1.0f;
    
    // Update agent statuses
    for (const FString& AgentID : Task.AssignedAgents)
    {
        if (RegisteredAgents.Contains(AgentID))
        {
            FAIAgentData& Agent = RegisteredAgents[AgentID];
            Agent.Status = EAIAgentStatus::Idle;
            Agent.CurrentTaskID = FString();
            
            // Track performance
            float AgentContribution = Task.AgentContributions.Contains(AgentID) ? 
                Task.AgentContributions[AgentID] : 0.0f;
            TrackAgentPerformance(AgentID, AgentContribution);
        }
    }
    
    // Distribute rewards
    DistributeTaskRewards(TaskID);
    
    // Add to history
    TaskHistory.Add(TaskID);
    
    OnTaskCompleted.Broadcast(TaskID);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCollaborativeAINetwork: Completed task %s (%s)"),
        *TaskID, *Task.TaskName);
}

void UMingRTSCollaborativeAINetwork::CancelTask(const FString& TaskID)
{
    if (!ActiveTasks.Contains(TaskID))
    {
        return;
    }
    
    FAICollaborativeTask& Task = ActiveTasks[TaskID];
    
    // Free up agents
    for (const FString& AgentID : Task.AssignedAgents)
    {
        if (RegisteredAgents.Contains(AgentID))
        {
            FAIAgentData& Agent = RegisteredAgents[AgentID];
            Agent.Status = EAIAgentStatus::Idle;
            Agent.CurrentTaskID = FString();
        }
    }
    
    ActiveTasks.Remove(TaskID);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCollaborativeAINetwork: Cancelled task %s"), *TaskID);
}

FAICollaborativeTask UMingRTSCollaborativeAINetwork::GetTaskInfo(const FString& TaskID) const
{
    if (ActiveTasks.Contains(TaskID))
    {
        return ActiveTasks[TaskID];
    }
    return FAICollaborativeTask();
}

TArray<FString> UMingRTSCollaborativeAINetwork::GetActiveTaskIDs() const
{
    TArray<FString> TaskIDs;
    for (const auto& TaskPair : ActiveTasks)
    {
        if (!TaskPair.Value.bIsComplete)
        {
            TaskIDs.Add(TaskPair.Key);
        }
    }
    return TaskIDs;
}

void UMingRTSCollaborativeAINetwork::ShareKnowledge(const FString& AgentID, const FString& Category,
    const FString& Content, float Confidence)
{
    if (!RegisteredAgents.Contains(AgentID))
    {
        return;
    }
    
    FString KnowledgeID = GenerateKnowledgeID();
    
    FCollectiveIntelligenceData Knowledge;
    Knowledge.KnowledgeID = KnowledgeID;
    Knowledge.KnowledgeCategory = Category;
    Knowledge.KnowledgeContent = Content;
    Knowledge.ConfidenceLevel = FMath::Clamp(Confidence, 0.0f, 1.0f);
    Knowledge.ContributorCount = 1;
    Knowledge.CreationTime = CurrentGameTime;
    Knowledge.LastUpdateTime = CurrentGameTime;
    
    // Check if similar knowledge already exists
    bool bMerged = false;
    for (auto& ExistingPair : CollectiveKnowledgeBase)
    {
        FCollectiveIntelligenceData& Existing = ExistingPair.Value;
        if (Existing.KnowledgeCategory == Category && 
            Existing.KnowledgeContent == Content)
        {
            // Merge knowledge
            Existing.ConfidenceLevel = FMath::Max(Existing.ConfidenceLevel, Confidence);
            Existing.ContributorCount++;
            Existing.LastUpdateTime = CurrentGameTime;
            Existing.UsageCount++;
            
            KnowledgeID = Existing.KnowledgeID;
            bMerged = true;
            break;
        }
    }
    
    if (!bMerged)
    {
        CollectiveKnowledgeBase.Add(KnowledgeID, Knowledge);
    }
    
    // Propagate to connected agents
    FAIAgentData& Agent = RegisteredAgents[AgentID];
    PropagateKnowledge(KnowledgeID, Agent.ConnectedAgents);
    
    OnCollectiveIntelligenceUpdate.Broadcast(KnowledgeID, 
        bMerged ? CollectiveKnowledgeBase[KnowledgeID].ConfidenceLevel : Confidence);
    
    UE_LOG(LogTemp, Verbose, TEXT("MingRTSCollaborativeAINetwork: Agent %s shared knowledge in category %s"),
        *AgentID, *Category);
}

void UMingRTSCollaborativeAINetwork::UpdateKnowledgeConfidence(const FString& KnowledgeID, float NewConfidence)
{
    if (!CollectiveKnowledgeBase.Contains(KnowledgeID))
    {
        return;
    }
    
    FCollectiveIntelligenceData& Knowledge = CollectiveKnowledgeBase[KnowledgeID];
    Knowledge.ConfidenceLevel = FMath::Clamp(NewConfidence, 0.0f, 1.0f);
    Knowledge.LastUpdateTime = CurrentGameTime;
    
    OnCollectiveIntelligenceUpdate.Broadcast(KnowledgeID, NewConfidence);
}

TArray<FCollectiveIntelligenceData> UMingRTSCollaborativeAINetwork::GetCollectiveKnowledge(const FString& Category) const
{
    TArray<FCollectiveIntelligenceData> Result;
    
    for (const auto& KnowledgePair : CollectiveKnowledgeBase)
    {
        if (KnowledgePair.Value.KnowledgeCategory == Category)
        {
            Result.Add(KnowledgePair.Value);
        }
    }
    
    // Sort by confidence level
    Result.Sort([](const FCollectiveIntelligenceData& A, const FCollectiveIntelligenceData& B)
    {
        return A.ConfidenceLevel > B.ConfidenceLevel;
    });
    
    return Result;
}

FCollectiveIntelligenceData UMingRTSCollaborativeAINetwork::QueryKnowledge(const FString& Query) const
{
    // Simple string matching for now
    FCollectiveIntelligenceData BestMatch;
    float BestScore = 0.0f;
    
    for (const auto& KnowledgePair : CollectiveKnowledgeBase)
    {
        const FCollectiveIntelligenceData& Knowledge = KnowledgePair.Value;
        
        // Calculate relevance score
        float Score = 0.0f;
        if (Knowledge.KnowledgeContent.Contains(Query))
        {
            Score = 1.0f;
        }
        else if (Knowledge.KnowledgeCategory.Contains(Query))
        {
            Score = 0.8f;
        }
        
        // Weight by confidence
        Score *= Knowledge.ConfidenceLevel;
        
        // Weight by usage
        Score *= (1.0f + Knowledge.UsageCount * 0.1f);
        
        if (Score > BestScore)
        {
            BestScore = Score;
            BestMatch = Knowledge;
        }
    }
    
    return BestMatch;
}

void UMingRTSCollaborativeAINetwork::OptimalRoleAssignment(const FString& TaskID)
{
    if (!ActiveTasks.Contains(TaskID))
    {
        return;
    }
    
    FAICollaborativeTask& Task = ActiveTasks[TaskID];
    
    // Remove current assignments
    for (const FString& AgentID : Task.AssignedAgents)
    {
        if (RegisteredAgents.Contains(AgentID))
        {
            RegisteredAgents[AgentID].Status = EAIAgentStatus::Idle;
        }
    }
    Task.AssignedAgents.Empty();
    Task.AgentContributions.Empty();
    
    // Find optimal team using current strategy
    switch (CurrentStrategy)
    {
    case EAICollaborationStrategy::Hierarchical:
        ExecuteHierarchicalStrategy(Task);
        break;
    case EAICollaborationStrategy::Democratic:
        ExecuteDemocraticStrategy(Task);
        break;
    case EAICollaborationStrategy::Competitive:
        ExecuteCompetitiveStrategy(Task);
        break;
    case EAICollaborationStrategy::Cooperative:
        ExecuteCooperativeStrategy(Task);
        break;
    case EAICollaborationStrategy::Swarm:
        ExecuteSwarmStrategy(Task);
        break;
    case EAICollaborationStrategy::MarketBased:
        ExecuteMarketBasedStrategy(Task);
        break;
    }
}

void UMingRTSCollaborativeAINetwork::FormTaskGroup(EAIAgentRole GroupType, int32 GroupSize)
{
    TArray<FString> AvailableAgents = GetAgentsByRole(GroupType);
    
    // Filter for idle agents
    TArray<FString> IdleAgents;
    for (const FString& AgentID : AvailableAgents)
    {
        if (RegisteredAgents[AgentID].Status == EAIAgentStatus::Idle)
        {
            IdleAgents.Add(AgentID);
        }
    }
    
    if (IdleAgents.Num() >= GroupSize)
    {
        // Form connections between agents
        for (int32 i = 0; i < GroupSize; ++i)
        {
            FAIAgentData& Agent = RegisteredAgents[IdleAgents[i]];
            
            for (int32 j = 0; j < GroupSize; ++j)
            {
                if (i != j)
                {
                    Agent.ConnectedAgents.AddUnique(IdleAgents[j]);
                }
            }
        }
        
        UE_LOG(LogTemp, Log, TEXT("MingRTSCollaborativeAINetwork: Formed task group of %d %d-role agents"),
            GroupSize, static_cast<int32>(GroupType));
    }
}

void UMingRTSCollaborativeAINetwork::ReassignRolesBasedOnPerformance()
{
    // Track agent performance in different roles
    TMap<FString, TMap<EAIAgentRole, float>> RolePerformance;
    
    for (const auto& AgentPair : RegisteredAgents)
    {
        const FAIAgentData& Agent = AgentPair.Value;
        RolePerformance.Add(AgentPair.Key, TMap<EAIAgentRole, float>());
        RolePerformance[AgentPair.Key].Add(Agent.PrimaryRole, Agent.EfficiencyScore);
    }
    
    // Reassign roles for underperforming agents
    for (const auto& PerformancePair : RolePerformance)
    {
        const FString& AgentID = PerformancePair.Key;
        const auto& RoleScores = PerformancePair.Value;
        
        if (RoleScores.Num() > 0)
        {
            // Find best role
            EAIAgentRole BestRole = EAIAgentRole::Commander;
            float BestScore = 0.0f;
            
            for (const auto& RoleScore : RoleScores)
            {
                if (RoleScore.Value > BestScore)
                {
                    BestScore = RoleScore.Value;
                    BestRole = RoleScore.Key;
                }
            }
            
            // Update if different and better
            if (BestRole != RegisteredAgents[AgentID].PrimaryRole && BestScore > 0.7f)
            {
                SetAgentRole(AgentID, BestRole);
            }
        }
    }
}

void UMingRTSCollaborativeAINetwork::SetCollaborationStrategy(EAICollaborationStrategy NewStrategy)
{
    CurrentStrategy = NewStrategy;
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCollaborativeAINetwork: Strategy changed to %d"),
        static_cast<int32>(NewStrategy));
}

EAICollaborationStrategy UMingRTSCollaborativeAINetwork::GetCurrentStrategy() const
{
    return CurrentStrategy;
}

FAINetworkMetrics UMingRTSCollaborativeAINetwork::GetNetworkMetrics() const
{
    FAINetworkMetrics Metrics;
    
    Metrics.TotalAgents = RegisteredAgents.Num();
    
    int32 ActiveCount = 0;
    for (const auto& AgentPair : RegisteredAgents)
    {
        if (AgentPair.Value.Status != EAIAgentStatus::Offline &&
            AgentPair.Value.Status != EAIAgentStatus::Error)
        {
            ActiveCount++;
        }
        
        // Count by role
        EAIAgentRole Role = AgentPair.Value.PrimaryRole;
        if (!Metrics.RoleDistribution.Contains(Role))
        {
            Metrics.RoleDistribution.Add(Role, 0);
        }
        Metrics.RoleDistribution[Role]++;
    }
    Metrics.ActiveAgents = ActiveCount;
    
    // Task metrics
    Metrics.TotalTasks = ActiveTasks.Num() + TaskHistory.Num();
    
    int32 CompletedCount = 0;
    float TotalCompletionTime = 0.0f;
    for (const auto& TaskPair : ActiveTasks)
    {
        if (TaskPair.Value.bIsComplete)
        {
            CompletedCount++;
        }
    }
    CompletedCount += TaskHistory.Num();
    Metrics.CompletedTasks = CompletedCount;
    
    if (CompletedCount > 0)
    {
        Metrics.AverageTaskCompletionTime = TotalCompletionTime / CompletedCount;
    }
    
    // Calculate network efficiency
    float EfficiencySum = 0.0f;
    int32 EfficiencyCount = 0;
    for (const auto& AgentPair : RegisteredAgents)
    {
        EfficiencySum += AgentPair.Value.EfficiencyScore;
        EfficiencyCount++;
    }
    if (EfficiencyCount > 0)
    {
        Metrics.NetworkEfficiency = EfficiencySum / EfficiencyCount;
    }
    
    // Collaboration success rate
    if (Metrics.TotalTasks > 0)
    {
        Metrics.CollaborationSuccessRate = static_cast<float>(Metrics.CompletedTasks) / Metrics.TotalTasks;
    }
    
    // Collective intelligence score
    float KnowledgeSum = 0.0f;
    int32 KnowledgeCount = 0;
    for (const auto& KnowledgePair : CollectiveKnowledgeBase)
    {
        KnowledgeSum += KnowledgePair.Value.ConfidenceLevel;
        KnowledgeCount++;
    }
    if (KnowledgeCount > 0)
    {
        Metrics.CollectiveIntelligenceScore = KnowledgeSum / KnowledgeCount;
    }
    
    return Metrics;
}

float UMingRTSCollaborativeAINetwork::CalculateTeamSynergy(const TArray<FString>& AgentIDs) const
{
    if (AgentIDs.Num() < 2)
    {
        return 1.0f;
    }
    
    float TotalSynergy = 0.0f;
    int32 PairCount = 0;
    
    for (int32 i = 0; i < AgentIDs.Num(); ++i)
    {
        for (int32 j = i + 1; j < AgentIDs.Num(); ++j)
        {
            float Compatibility = CalculateAgentCompatibility(AgentIDs[i], AgentIDs[j]);
            TotalSynergy += Compatibility;
            PairCount++;
        }
    }
    
    return PairCount > 0 ? TotalSynergy / PairCount : 1.0f;
}

TArray<FString> UMingRTSCollaborativeAINetwork::FindOptimalTeam(EAIAgentRole TaskType, int32 TeamSize) const
{
    TArray<FString> Candidates = GetAgentsByRole(TaskType);
    
    // Filter for available agents
    TArray<FString> AvailableAgents;
    for (const FString& AgentID : Candidates)
    {
        EAIAgentStatus Status = RegisteredAgents[AgentID].Status;
        if (Status == EAIAgentStatus::Idle || Status == EAIAgentStatus::Busy)
        {
            AvailableAgents.Add(AgentID);
        }
    }
    
    if (AvailableAgents.Num() <= TeamSize)
    {
        return AvailableAgents;
    }
    
    // Try all combinations to find best synergy (for small teams)
    if (TeamSize <= 4 && AvailableAgents.Num() <= 10)
    {
        TArray<FString> BestTeam;
        float BestSynergy = -1.0f;
        
        // Generate combinations using bitmask
        int32 TotalAgents = AvailableAgents.Num();
        for (int32 Mask = 0; Mask < (1 << TotalAgents); ++Mask)
        {
            if (__builtin_popcount(Mask) == TeamSize)
            {
                TArray<FString> Team;
                for (int32 i = 0; i < TotalAgents; ++i)
                {
                    if (Mask & (1 << i))
                    {
                        Team.Add(AvailableAgents[i]);
                    }
                }
                
                float Synergy = CalculateTeamSynergy(Team);
                if (Synergy > BestSynergy)
                {
                    BestSynergy = Synergy;
                    BestTeam = Team;
                }
            }
        }
        
        return BestTeam;
    }
    else
    {
        // Greedy selection for larger teams
        TArray<FString> Team;
        
        // Start with best individual
        AvailableAgents.Sort([this, TaskType](const FString& A, const FString& B)
        {
            const FAIAgentData& AgentA = RegisteredAgents[A];
            const FAIAgentData& AgentB = RegisteredAgents[B];
            
            float ScoreA = AgentA.PrimaryRole == TaskType ? 1.0f : 0.5f;
            ScoreA *= AgentA.EfficiencyScore;
            
            float ScoreB = AgentB.PrimaryRole == TaskType ? 1.0f : 0.5f;
            ScoreB *= AgentB.EfficiencyScore;
            
            return ScoreA > ScoreB;
        });
        
        for (int32 i = 0; i < FMath::Min(TeamSize, AvailableAgents.Num()); ++i)
        {
            Team.Add(AvailableAgents[i]);
        }
        
        return Team;
    }
}

float UMingRTSCollaborativeAINetwork::GetAgentContributionScore(const FString& AgentID) const
{
    if (!RegisteredAgents.Contains(AgentID))
    {
        return 0.0f;
    }
    
    const FAIAgentData& Agent = RegisteredAgents[AgentID];
    
    // Calculate based on completed tasks
    float TaskContribution = 0.0f;
    for (const auto& TaskPair : ActiveTasks)
    {
        const FAICollaborativeTask& Task = TaskPair.Value;
        if (Task.AssignedAgents.Contains(AgentID))
        {
            float Contribution = Task.AgentContributions.Contains(AgentID) ? 
                Task.AgentContributions[AgentID] : 0.0f;
            TaskContribution += Contribution * Task.TaskValue;
        }
    }
    
    // Weight by efficiency and experience
    float Score = TaskContribution * Agent.EfficiencyScore * Agent.ExperienceLevel;
    
    return Score;
}

void UMingRTSCollaborativeAINetwork::Tick(float DeltaTime)
{
    if (!bIsInitialized)
    {
        return;
    }
    
    CurrentGameTime += DeltaTime;
    
    // Process all subsystems
    ProcessMessageQueues(DeltaTime);
    UpdateAgentStates(DeltaTime);
    ProcessTaskAssignments(DeltaTime);
    UpdateCollectiveIntelligence(DeltaTime);
    CleanupExpiredMessages();
    OptimizeResourceAllocation();
    UpdateNetworkEfficiency();
    UpdateCollectiveIntelligenceScore();
}

void UMingRTSCollaborativeAINetwork::ProcessMessageQueues(float DeltaTime)
{
    // Process emergency messages first
    ProcessEmergencyMessages();
    
    // Process other messages
    for (auto& QueuePair : AgentMessageQueues)
    {
        const FString& AgentID = QueuePair.Key;
        auto& Messages = QueuePair.Value;
        
        // Sort by priority
        Messages.Sort([](const FAIAgentMessage& A, const FAIAgentMessage& B)
        {
            return A.Priority > B.Priority;
        });
        
        // Deliver messages
        for (const FAIAgentMessage& Message : Messages)
        {
            DeliverMessage(AgentID, Message);
        }
    }
}

void UMingRTSCollaborativeAINetwork::UpdateAgentStates(float DeltaTime)
{
    // Check for inactive agents
    for (auto& AgentPair : RegisteredAgents)
    {
        FAIAgentData& Agent = AgentPair.Value;
        
        float TimeSinceLastActivity = CurrentGameTime - Agent.LastActivityTime;
        
        // Mark as idle if inactive for too long
        if (TimeSinceLastActivity > 300.0f && Agent.Status == EAIAgentStatus::Busy)
        {
            Agent.Status = EAIAgentStatus::Idle;
        }
        
        // Update trust level based on performance
        if (Agent.EfficiencyScore > 0.8f)
        {
            Agent.TrustLevel = FMath::Min(1.0f, Agent.TrustLevel + 0.001f * DeltaTime);
        }
        else if (Agent.EfficiencyScore < 0.5f)
        {
            Agent.TrustLevel = FMath::Max(0.0f, Agent.TrustLevel - 0.001f * DeltaTime);
        }
        
        // Slowly increase experience
        Agent.ExperienceLevel = FMath::Min(10.0f, Agent.ExperienceLevel + 0.0001f * DeltaTime);
    }
}

void UMingRTSCollaborativeAINetwork::ProcessTaskAssignments(float DeltaTime)
{
    // Process incomplete tasks
    for (auto& TaskPair : ActiveTasks)
    {
        FAICollaborativeTask& Task = TaskPair.Value;
        
        if (Task.bIsComplete)
        {
            continue;
        }
        
        // Check if task needs more agents
        if (Task.AssignedAgents.Num() < Task.MinAgentsRequired)
        {
            AssignTaskAutomatically(TaskPair.Key);
        }
        
        // Update progress simulation
        float ProgressRate = 0.0f;
        for (const FString& AgentID : Task.AssignedAgents)
        {
            if (RegisteredAgents.Contains(AgentID))
            {
                ProgressRate += RegisteredAgents[AgentID].EfficiencyScore;
            }
        }
        
        // Apply synergy bonus
        float Synergy = CalculateTeamSynergy(Task.AssignedAgents);
        ProgressRate *= (1.0f + Synergy * 0.5f);
        
        // Update progress
        float ProgressDelta = ProgressRate * DeltaTime / Task.EstimatedDuration;
        UpdateTaskProgress(TaskPair.Key, Task.Progress + ProgressDelta);
    }
}

void UMingRTSCollaborativeAINetwork::UpdateCollectiveIntelligence(float DeltaTime)
{
    // Decay old knowledge confidence
    for (auto& KnowledgePair : CollectiveKnowledgeBase)
    {
        FCollectiveIntelligenceData& Knowledge = KnowledgePair.Value;
        
        float Age = CurrentGameTime - Knowledge.LastUpdateTime;
        if (Age > 3600.0f) // 1 hour
        {
            // Slowly decay unused knowledge
            Knowledge.ConfidenceLevel = FMath::Max(0.1f, Knowledge.ConfidenceLevel - 0.0001f * DeltaTime);
        }
    }
}

void UMingRTSCollaborativeAINetwork::CleanupExpiredMessages()
{
    for (auto& QueuePair : AgentMessageQueues)
    {
        auto& Messages = QueuePair.Value;
        
        for (int32 i = Messages.Num() - 1; i >= 0; --i)
        {
            float Age = CurrentGameTime - Messages[i].Timestamp;
            if (Age > Messages[i].ExpirationTime)
            {
                Messages.RemoveAt(i);
            }
        }
    }
}

void UMingRTSCollaborativeAINetwork::ProcessEmergencyMessages()
{
    // Emergency messages are already processed immediately upon sending
}

void UMingRTSCollaborativeAINetwork::OptimizeResourceAllocation()
{
    // Balance resource allocation among agents
    if (RegisteredAgents.Num() == 0)
    {
        return;
    }
    
    float TotalResources = 0.0f;
    for (const auto& AgentPair : RegisteredAgents)
    {
        TotalResources += AgentPair.Value.ResourceAllocation;
    }
    
    float AverageResource = TotalResources / RegisteredAgents.Num();
    
    // Reallocate based on efficiency and task load
    for (auto& AgentPair : RegisteredAgents)
    {
        FAIAgentData& Agent = AgentPair.Value;
        
        float TargetAllocation = AverageResource;
        
        // Increase allocation for high-performing agents
        if (Agent.EfficiencyScore > 1.0f)
        {
            TargetAllocation *= (1.0f + (Agent.EfficiencyScore - 1.0f) * 0.5f);
        }
        
        // Decrease for agents with many tasks
        if (!Agent.CurrentTaskID.IsEmpty())
        {
            TargetAllocation *= 0.8f;
        }
        
        // Smooth transition
        Agent.ResourceAllocation = FMath::Lerp(Agent.ResourceAllocation, TargetAllocation, 0.1f);
    }
}

void UMingRTSCollaborativeAINetwork::TrackAgentPerformance(const FString& AgentID, float TaskPerformance)
{
    if (!RegisteredAgents.Contains(AgentID))
    {
        return;
    }
    
    FAIAgentData& Agent = RegisteredAgents[AgentID];
    
    // Update efficiency score with exponential moving average
    float Alpha = 0.3f;
    Agent.EfficiencyScore = FMath::Lerp(Agent.EfficiencyScore, TaskPerformance, Alpha);
    
    // Clamp to reasonable range
    Agent.EfficiencyScore = FMath::Clamp(Agent.EfficiencyScore, 0.1f, 2.0f);
}

void UMingRTSCollaborativeAINetwork::UpdateNetworkEfficiency()
{
    // Metrics are calculated on-demand in GetNetworkMetrics
}

void UMingRTSCollaborativeAINetwork::UpdateCollectiveIntelligenceScore()
{
    // Score is calculated based on knowledge base state
    float TotalConfidence = 0.0f;
    int32 KnowledgeCount = 0;
    
    for (const auto& KnowledgePair : CollectiveKnowledgeBase)
    {
        TotalConfidence += KnowledgePair.Value.ConfidenceLevel;
        KnowledgeCount++;
    }
}

float UMingRTSCollaborativeAINetwork::CalculateAgentSuitability(const FString& AgentID, const FAICollaborativeTask& Task) const
{
    if (!RegisteredAgents.Contains(AgentID))
    {
        return 0.0f;
    }
    
    const FAIAgentData& Agent = RegisteredAgents[AgentID];
    
    // Base suitability based on role match
    float RoleMatch = 0.0f;
    if (Agent.PrimaryRole == Task.RequiredRole)
    {
        RoleMatch = 1.0f;
    }
    else if (Agent.SecondaryRoles.Contains(Task.RequiredRole))
    {
        RoleMatch = 0.7f;
    }
    else
    {
        RoleMatch = 0.3f;
    }
    
    // Weight by efficiency and experience
    float Suitability = RoleMatch * Agent.EfficiencyScore * (0.5f + 0.5f * (Agent.ExperienceLevel / 10.0f));
    
    // Reduce suitability if agent is busy
    if (Agent.Status != EAIAgentStatus::Idle)
    {
        Suitability *= 0.5f;
    }
    
    // Consider location proximity
    float Distance = FVector::Distance(Agent.CurrentLocation, Task.TargetLocation);
    float ProximityFactor = FMath::Exp(-Distance / 5000.0f); // Exponential decay with distance
    Suitability *= (0.5f + 0.5f * ProximityFactor);
    
    return Suitability;
}

float UMingRTSCollaborativeAINetwork::CalculateAgentCompatibility(const FString& Agent1ID, const FString& Agent2ID) const
{
    if (!RegisteredAgents.Contains(Agent1ID) || !RegisteredAgents.Contains(Agent2ID))
    {
        return 0.0f;
    }
    
    const FAIAgentData& Agent1 = RegisteredAgents[Agent1ID];
    const FAIAgentData& Agent2 = RegisteredAgents[Agent2ID];
    
    // Trust-based compatibility
    float TrustCompatibility = FMath::Min(Agent1.TrustLevel, Agent2.TrustLevel);
    
    // Role complementarity
    float RoleCompatibility = 0.5f;
    if (Agent1.PrimaryRole != Agent2.PrimaryRole)
    {
        RoleCompatibility = 1.0f; // Different roles complement each other
    }
    
    // Check if they're already connected
    float ConnectionBonus = 0.0f;
    if (Agent1.ConnectedAgents.Contains(Agent2ID))
    {
        ConnectionBonus = 0.2f;
    }
    
    // Experience balance
    float ExperienceCompatibility = 1.0f - FMath::Abs(Agent1.ExperienceLevel - Agent2.ExperienceLevel) / 10.0f;
    
    float TotalCompatibility = (TrustCompatibility + RoleCompatibility + ExperienceCompatibility) / 3.0f + ConnectionBonus;
    
    return FMath::Clamp(TotalCompatibility, 0.0f, 1.0f);
}

void UMingRTSCollaborativeAINetwork::DistributeTaskRewards(const FString& TaskID)
{
    if (!ActiveTasks.Contains(TaskID))
    {
        return;
    }
    
    FAICollaborativeTask& Task = ActiveTasks[TaskID];
    
    // Calculate total contribution
    float TotalContribution = 0.0f;
    for (const auto& Contribution : Task.AgentContributions)
    {
        TotalContribution += Contribution.Value;
    }
    
    if (TotalContribution == 0.0f)
    {
        return;
    }
    
    // Distribute rewards proportionally
    for (const FString& AgentID : Task.AssignedAgents)
    {
        if (!RegisteredAgents.Contains(AgentID))
        {
            continue;
        }
        
        FAIAgentData& Agent = RegisteredAgents[AgentID];
        
        float Contribution = Task.AgentContributions.Contains(AgentID) ? 
            Task.AgentContributions[AgentID] : 0.0f;
        
        float RewardRatio = Contribution / TotalContribution;
        float ExperienceGain = RewardRatio * Task.TaskValue * 0.1f;
        
        // Update agent experience
        Agent.ExperienceLevel = FMath::Min(10.0f, Agent.ExperienceLevel + ExperienceGain);
        
        // Increase trust for successful collaboration
        for (const FString& OtherAgentID : Task.AssignedAgents)
        {
            if (OtherAgentID != AgentID && RegisteredAgents.Contains(OtherAgentID))
            {
                Agent.TrustLevel = FMath::Min(1.0f, Agent.TrustLevel + 0.01f);
            }
        }
    }
}

void UMingRTSCollaborativeAINetwork::PropagateKnowledge(const FString& KnowledgeID, const TArray<FString>& TargetAgents)
{
    if (!CollectiveKnowledgeBase.Contains(KnowledgeID))
    {
        return;
    }
    
    // Share knowledge with connected agents
    for (const FString& AgentID : TargetAgents)
    {
        if (RegisteredAgents.Contains(AgentID))
        {
            // Send knowledge sharing message
            TArray<FString> Recipients;
            Recipients.Add(AgentID);
            
            FCollectiveIntelligenceData& Knowledge = CollectiveKnowledgeBase[KnowledgeID];
            FString Content = FString::Printf(TEXT("Knowledge Share [%s]: %s"), 
                *Knowledge.KnowledgeCategory, *Knowledge.KnowledgeContent);
            
            SendMessage(TEXT("SYSTEM"), Recipients, EAICommunicationType::Intelligence, Content, 0.5f);
        }
    }
}

FString UMingRTSCollaborativeAINetwork::GenerateAgentID() const
{
    return FString::Printf(TEXT("AI_AGENT_%s"), *FGuid::NewGuid().ToString());
}

FString UMingRTSCollaborativeAINetwork::GenerateTaskID() const
{
    return FString::Printf(TEXT("AI_TASK_%s"), *FGuid::NewGuid().ToString());
}

FString UMingRTSCollaborativeAINetwork::GenerateMessageID() const
{
    return FString::Printf(TEXT("AI_MSG_%s"), *FGuid::NewGuid().ToString());
}

FString UMingRTSCollaborativeAINetwork::GenerateKnowledgeID() const
{
    return FString::Printf(TEXT("AI_KNOW_%s"), *FGuid::NewGuid().ToString());
}

void UMingRTSCollaborativeAINetwork::RouteMessage(const FAIAgentMessage& Message)
{
    // Route based on communication type
    switch (Message.CommunicationType)
    {
    case EAICommunicationType::Direct:
        for (const FString& RecipientID : Message.RecipientIDs)
        {
            if (AgentMessageQueues.Contains(RecipientID))
            {
                AgentMessageQueues[RecipientID].Add(Message);
            }
        }
        break;
        
    case EAICommunicationType::Broadcast:
        for (const auto& QueuePair : AgentMessageQueues)
        {
            if (QueuePair.Key != Message.SenderID)
            {
                AgentMessageQueues[QueuePair.Key].Add(Message);
            }
        }
        break;
        
    case EAICommunicationType::Group:
        // Group messages go to specific recipients only
        for (const FString& RecipientID : Message.RecipientIDs)
        {
            if (AgentMessageQueues.Contains(RecipientID))
            {
                AgentMessageQueues[RecipientID].Add(Message);
            }
        }
        break;
        
    case EAICommunicationType::Emergency:
        // Emergency messages are handled immediately
        for (const FString& RecipientID : Message.RecipientIDs)
        {
            if (AgentMessageQueues.Contains(RecipientID))
            {
                AgentMessageQueues[RecipientID].Insert(Message, 0);
            }
        }
        break;
        
    default:
        for (const FString& RecipientID : Message.RecipientIDs)
        {
            if (AgentMessageQueues.Contains(RecipientID))
            {
                AgentMessageQueues[RecipientID].Add(Message);
            }
        }
        break;
    }
}

void UMingRTSCollaborativeAINetwork::DeliverMessage(const FString& AgentID, const FAIAgentMessage& Message)
{
    // Update agent status
    if (RegisteredAgents.Contains(AgentID))
    {
        UpdateAgentStatus(AgentID, EAIAgentStatus::Communicating);
    }
    
    // Broadcast message received event
    OnMessageReceived.Broadcast(AgentID, Message);
}

bool UMingRTSCollaborativeAINetwork::ValidateMessage(const FAIAgentMessage& Message) const
{
    // Validate sender exists
    if (!Message.SenderID.IsEmpty() && !RegisteredAgents.Contains(Message.SenderID) && 
        Message.SenderID != TEXT("SYSTEM"))
    {
        return false;
    }
    
    // Validate recipients exist
    for (const FString& RecipientID : Message.RecipientIDs)
    {
        if (!AgentMessageQueues.Contains(RecipientID))
        {
            return false;
        }
    }
    
    // Validate content is not empty
    if (Message.MessageContent.IsEmpty())
    {
        return false;
    }
    
    return true;
}

// Strategy implementations
void UMingRTSCollaborativeAINetwork::ExecuteHierarchicalStrategy(FAICollaborativeTask& Task)
{
    // Find commander
    TArray<FString> Commanders = GetAgentsByRole(EAIAgentRole::Commander);
    TArray<FString> AvailableCommanders;
    
    for (const FString& AgentID : Commanders)
    {
        if (RegisteredAgents[AgentID].Status == EAIAgentStatus::Idle)
        {
            AvailableCommanders.Add(AgentID);
        }
    }
    
    if (AvailableCommanders.Num() > 0)
    {
        Task.CoordinatorAgentID = AvailableCommanders[0];
        
        // Commander assigns other agents
        TArray<FString> Subordinates;
        for (const auto& AgentPair : RegisteredAgents)
        {
            if (AgentPair.Value.Status == EAIAgentStatus::Idle && 
                AgentPair.Key != Task.CoordinatorAgentID)
            {
                Subordinates.Add(AgentPair.Key);
                if (Subordinates.Num() >= Task.MaxAgentsAllowed - 1)
                {
                    break;
                }
            }
        }
        
        Subordinates.Insert(Task.CoordinatorAgentID, 0);
        AssignTaskToAgents(Task.TaskID, Subordinates);
    }
    else
    {
        // Fall back to automatic assignment
        AssignTaskAutomatically(Task.TaskID);
    }
}

void UMingRTSCollaborativeAINetwork::ExecuteDemocraticStrategy(FAICollaborativeTask& Task)
{
    // All suitable agents vote on participation
    TArray<FString> Candidates;
    for (const auto& AgentPair : RegisteredAgents)
    {
        if (AgentPair.Value.Status == EAIAgentStatus::Idle)
        {
            float Suitability = CalculateAgentSuitability(AgentPair.Key, Task);
            if (Suitability > 0.5f)
            {
                Candidates.Add(AgentPair.Key);
            }
        }
    }
    
    // Sort by suitability (democratic selection of best candidates)
    Candidates.Sort([this, &Task](const FString& A, const FString& B)
    {
        return CalculateAgentSuitability(A, Task) > CalculateAgentSuitability(B, Task);
    });
    
    // Take top candidates
    int32 NumToSelect = FMath::Min(Task.MaxAgentsAllowed, Candidates.Num());
    if (NumToSelect >= Task.MinAgentsRequired)
    {
        TArray<FString> SelectedAgents;
        for (int32 i = 0; i < NumToSelect; ++i)
        {
            SelectedAgents.Add(Candidates[i]);
        }
        
        // Democratic coordinator selection - most experienced agent
        FString Coordinator = SelectedAgents[0];
        float MaxExperience = 0.0f;
        for (const FString& AgentID : SelectedAgents)
        {
            float Exp = RegisteredAgents[AgentID].ExperienceLevel;
            if (Exp > MaxExperience)
            {
                MaxExperience = Exp;
                Coordinator = AgentID;
            }
        }
        Task.CoordinatorAgentID = Coordinator;
        
        AssignTaskToAgents(Task.TaskID, SelectedAgents);
    }
}

void UMingRTSCollaborativeAINetwork::ExecuteCompetitiveStrategy(FAICollaborativeTask& Task)
{
    // Agents compete for task assignment based on efficiency
    TArray<FString> Candidates;
    for (const auto& AgentPair : RegisteredAgents)
    {
        if (AgentPair.Value.Status == EAIAgentStatus::Idle)
        {
            Candidates.Add(AgentPair.Key);
        }
    }
    
    // Sort by efficiency score (competition)
    Candidates.Sort([this](const FString& A, const FString& B)
    {
        return RegisteredAgents[A].EfficiencyScore > RegisteredAgents[B].EfficiencyScore;
    });
    
    // Take best performers
    int32 NumToSelect = FMath::Min(Task.MaxAgentsAllowed, Candidates.Num());
    if (NumToSelect >= Task.MinAgentsRequired)
    {
        TArray<FString> SelectedAgents;
        for (int32 i = 0; i < NumToSelect; ++i)
        {
            SelectedAgents.Add(Candidates[i]);
        }
        
        // Best performer is coordinator
        Task.CoordinatorAgentID = SelectedAgents[0];
        
        AssignTaskToAgents(Task.TaskID, SelectedAgents);
    }
}

void UMingRTSCollaborativeAINetwork::ExecuteCooperativeStrategy(FAICollaborativeTask& Task)
{
    // Focus on team synergy
    TArray<FString> BestTeam = FindOptimalTeam(Task.RequiredRole, Task.MaxAgentsAllowed);
    
    if (BestTeam.Num() >= Task.MinAgentsRequired)
    {
        // Find best coordinator for this team
        float BestCoordinationScore = 0.0f;
        FString BestCoordinator = BestTeam[0];
        
        for (const FString& AgentID : BestTeam)
        {
            const FAIAgentData& Agent = RegisteredAgents[AgentID];
            float CoordinationScore = Agent.SkillLevels.Contains(TEXT("Coordination")) ? 
                Agent.SkillLevels[TEXT("Coordination")] : 0.5f;
            
            if (CoordinationScore > BestCoordinationScore)
            {
                BestCoordinationScore = CoordinationScore;
                BestCoordinator = AgentID;
            }
        }
        
        Task.CoordinatorAgentID = BestCoordinator;
        AssignTaskToAgents(Task.TaskID, BestTeam);
    }
    else
    {
        // Fall back to automatic assignment
        AssignTaskAutomatically(Task.TaskID);
    }
}

void UMingRTSCollaborativeAINetwork::ExecuteSwarmStrategy(FAICollaborativeTask& Task)
{
    // Swarm behavior - many agents work together with simple rules
    TArray<FString> AvailableAgents;
    for (const auto& AgentPair : RegisteredAgents)
    {
        if (AgentPair.Value.Status == EAIAgentStatus::Idle)
        {
            AvailableAgents.Add(AgentPair.Key);
        }
    }
    
    // Use many agents (up to max allowed)
    int32 SwarmSize = FMath::Min(Task.MaxAgentsAllowed, AvailableAgents.Num());
    if (SwarmSize >= Task.MinAgentsRequired)
    {
        TArray<FString> SwarmAgents;
        for (int32 i = 0; i < SwarmSize; ++i)
        {
            SwarmAgents.Add(AvailableAgents[i]);
        }
        
        // In swarm, no single coordinator - decentralized control
        Task.CoordinatorAgentID = TEXT("SWARM");
        
        AssignTaskToAgents(Task.TaskID, SwarmAgents);
    }
}

void UMingRTSCollaborativeAINetwork::ExecuteMarketBasedStrategy(FAICollaborativeTask& Task)
{
    // Market-based allocation - agents bid on tasks
    struct FAgentBid
    {
        FString AgentID;
        float BidValue;
        float Suitability;
    };
    
    TArray<FAgentBid> Bids;
    
    for (const auto& AgentPair : RegisteredAgents)
    {
        const FAIAgentData& Agent = AgentPair.Value;
        if (Agent.Status == EAIAgentStatus::Idle)
        {
            FAgentBid Bid;
            Bid.AgentID = AgentPair.Key;
            Bid.Suitability = CalculateAgentSuitability(AgentPair.Key, Task);
            
            // Bid is based on suitability and efficiency (lower is better)
            Bid.BidValue = (1.0f / Agent.EfficiencyScore) * (2.0f - Bid.Suitability);
            
            Bids.Add(Bid);
        }
    }
    
    // Sort by bid value (ascending - lower bids win)
    Bids.Sort([](const FAgentBid& A, const FAgentBid& B)
    {
        return A.BidValue < B.BidValue;
    });
    
    // Select best bidders
    int32 NumToSelect = FMath::Min(Task.MaxAgentsAllowed, Bids.Num());
    if (NumToSelect >= Task.MinAgentsRequired)
    {
        TArray<FString> SelectedAgents;
        for (int32 i = 0; i < NumToSelect; ++i)
        {
            SelectedAgents.Add(Bids[i].AgentID);
        }
        
        // Best bidder is coordinator
        Task.CoordinatorAgentID = SelectedAgents[0];
        
        AssignTaskToAgents(Task.TaskID, SelectedAgents);
    }
}
