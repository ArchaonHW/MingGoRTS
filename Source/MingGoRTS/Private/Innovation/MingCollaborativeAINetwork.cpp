#include "Innovation/MingCollaborativeAINetwork.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "Algo/Reverse.h"

UMingCollaborativeAINetwork::UMingCollaborativeAINetwork()
{
    MaxAgents = 100;
    CommunicationRange = 1000.0f;
    CollaborationThreshold = 0.7f;
    bEnableDistributedDecision = true;
    bEnableCollectiveLearning = true;
    bEnableTaskAllocation = true;
    bNetworkActive = false;
}

void UMingCollaborativeAINetwork::InitializeCollaborativeNetwork()
{
    // Initialize network state
    bNetworkActive = true;
    
    // Clear existing data
    AIAgents.Empty();
    CommunicationChannels.Empty();
    TaskAllocations.Empty();
    CollectiveKnowledge.Empty();
    
    // Create default communication channel
    CreateCommunicationChannel(TEXT("Global"), ECommunicationType::Broadcast);
    
    UE_LOG(LogTemp, Log, TEXT("Collaborative AI Network initialized"));
}

void UMingCollaborativeAINetwork::ShutdownCollaborativeNetwork()
{
    bNetworkActive = false;
    
    // Clear all data
    AIAgents.Empty();
    CommunicationChannels.Empty();
    TaskAllocations.Empty();
    CollectiveKnowledge.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Collaborative AI Network shutdown"));
}

void UMingCollaborativeAINetwork::RegisterAIAgent(const FAIAgent& Agent)
{
    if (AIAgents.Num() >= MaxAgents)
    {
        UE_LOG(LogTemp, Warning, TEXT("Maximum AI agents limit reached"));
        return;
    }
    
    // Register agent with unique ID
    FAIAgent NewAgent = Agent;
    if (NewAgent.AgentID.IsEmpty())
    {
        NewAgent.AgentID = FString::Printf(TEXT("AIAgent_%d"), AIAgents.Num());
    }
    
    NewAgent.Status = EAgentStatus::Active;
    NewAgent.LastUpdateTime = FDateTime::Now();
    
    AIAgents.Add(NewAgent.AgentID, NewAgent);
    
    UE_LOG(LogTemp, Log, TEXT("Registered AI agent: %s"), *NewAgent.AgentID);
}

void UMingCollaborativeAINetwork::UnregisterAIAgent(const FString& AgentID)
{
    if (AIAgents.Contains(AgentID))
    {
        // Remove agent from all task allocations
        for (auto& TaskPair : TaskAllocations)
        {
            FTaskAllocation& Allocation = TaskPair.Value;
            Allocation.AssignedAgents.Remove(AgentID);
        }
        
        // Remove agent
        AIAgents.Remove(AgentID);
        
        UE_LOG(LogTemp, Log, TEXT("Unregistered AI agent: %s"), *AgentID);
    }
}

FAIAgent UMingCollaborativeAINetwork::GetAIAgent(const FString& AgentID) const
{
    if (AIAgents.Contains(AgentID))
    {
        return AIAgents[AgentID];
    }
    
    return FAIAgent();
}

TArray<FAIAgent> UMingCollaborativeAINetwork::GetAllAIAgents() const
{
    TArray<FAIAgent> AllAgents;
    
    for (const auto& AgentPair : AIAgents)
    {
        AllAgents.Add(AgentPair.Value);
    }
    
    return AllAgents;
}

void UMingCollaborativeAINetwork::CreateCommunicationChannel(const FString& ChannelID, ECommunicationType Type)
{
    if (CommunicationChannels.Contains(ChannelID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Communication channel already exists: %s"), *ChannelID);
        return;
    }
    
    FCommunicationChannel Channel;
    Channel.ChannelID = ChannelID;
    Channel.Type = Type;
    Channel.CreatedTime = FDateTime::Now();
    Channel.bActive = true;
    
    CommunicationChannels.Add(ChannelID, Channel);
    
    UE_LOG(LogTemp, Log, TEXT("Created communication channel: %s"), *ChannelID);
}

void UMingCollaborativeAINetwork::SendMessage(const FAICommunication& Message)
{
    if (!CommunicationChannels.Contains(Message.ChannelID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Communication channel not found: %s"), *Message.ChannelID);
        return;
    }
    
    // Add message to channel
    FCommunicationChannel& Channel = CommunicationChannels[Message.ChannelID];
    Channel.Messages.Add(Message);
    
    // Limit message history
    if (Channel.Messages.Num() > 1000)
    {
        Channel.Messages.RemoveAt(0);
    }
    
    // Process message based on channel type
    ProcessMessage(Message);
    
    UE_LOG(LogTemp, Verbose, TEXT("Sent message from %s to %s"), 
        *Message.SenderID, *Message.ChannelID);
}

TArray<FAICommunication> UMingCollaborativeAINetwork::GetMessages(const FString& ChannelID, int32 Count) const
{
    TArray<FAICommunication> Messages;
    
    if (CommunicationChannels.Contains(ChannelID))
    {
        const FCommunicationChannel& Channel = CommunicationChannels[ChannelID];
        
        // Get recent messages
        int32 StartIndex = FMath::Max(0, Channel.Messages.Num() - Count);
        for (int32 i = StartIndex; i < Channel.Messages.Num(); ++i)
        {
            Messages.Add(Channel.Messages[i]);
        }
    }
    
    return Messages;
}

void UMingCollaborativeAINetwork::AllocateTask(const FTaskAllocation& Allocation)
{
    if (!TaskAllocations.Contains(Allocation.TaskID))
    {
        TaskAllocations.Add(Allocation.TaskID, Allocation);
        
        UE_LOG(LogTemp, Log, TEXT("Allocated task %s to %d agents"), 
            *Allocation.TaskID, Allocation.AssignedAgents.Num());
    }
}

void UMingCollaborativeAINetwork::ReallocateTask(const FString& TaskID, const TArray<FString>& NewAgents)
{
    if (TaskAllocations.Contains(TaskID))
    {
        FTaskAllocation& Allocation = TaskAllocations[TaskID];
        Allocation.AssignedAgents = NewAgents;
        Allocation.Status = ETaskStatus::Reallocated;
        
        UE_LOG(LogTemp, Log, TEXT("Reallocated task %s to %d agents"), 
            *TaskID, NewAgents.Num());
    }
}

TArray<FString> UMingCollaborativeAINetwork::GetOptimalAgentsForTask(const FString& TaskID) const
{
    TArray<FString> OptimalAgents;
    
    // Find agents best suited for the task
    for (const auto& AgentPair : AIAgents)
    {
        const FAIAgent& Agent = AgentPair.Value;
        
        if (Agent.Status == EAgentStatus::Active)
        {
            // Calculate suitability score based on agent capabilities
            float SuitabilityScore = CalculateTaskSuitability(Agent, TaskID);
            
            if (SuitabilityScore > CollaborationThreshold)
            {
                OptimalAgents.Add(Agent.AgentID);
            }
        }
    }
    
    // Sort by suitability (descending)
    OptimalAgents.Sort([&](const FString& A, const FString& B) {
        float ScoreA = AIAgents.Contains(A) ? CalculateTaskSuitability(AIAgents[A], TaskID) : 0.0f;
        float ScoreB = AIAgents.Contains(B) ? CalculateTaskSuitability(AIAgents[B], TaskID) : 0.0f;
        return ScoreA > ScoreB;
    });
    
    return OptimalAgents;
}

void UMingCollaborativeAINetwork::ProcessDistributedDecision()
{
    if (!bEnableDistributedDecision)
    {
        return;
    }
    
    // Collect votes from all active agents
    TMap<FString, TArray<FDecisionVote>> Votes;
    
    for (const auto& AgentPair : AIAgents)
    {
        const FAIAgent& Agent = AgentPair.Value;
        
        if (Agent.Status == EAgentStatus::Active)
        {
            FDecisionVote Vote = GenerateAgentVote(Agent);
            
            if (!Votes.Contains(Vote.DecisionID))
            {
                Votes.Add(Vote.DecisionID, TArray<FDecisionVote>());
            }
            Votes[Vote.DecisionID].Add(Vote);
        }
    }
    
    // Process decisions
    for (const auto& VotePair : Votes)
    {
        const FString& DecisionID = VotePair.Key;
        const TArray<FDecisionVote>& DecisionVotes = VotePair.Value;
        
        FDecisionResult Result = ProcessVotes(DecisionVotes);
        DecisionResults.Add(DecisionID, Result);
        
        UE_LOG(LogTemp, Log, TEXT("Processed distributed decision %s: %s"), 
            *DecisionID, *Result.Decision);
    }
}

void UMingCollaborativeAINetwork::UpdateCollectiveKnowledge()
{
    if (!bEnableCollectiveLearning)
    {
        return;
    }
    
    // Aggregate knowledge from all agents
    TMap<FString, float> AggregatedKnowledge;
    
    for (const auto& AgentPair : AIAgents)
    {
        const FAIAgent& Agent = AgentPair.Value;
        
        for (const auto& KnowledgePair : Agent.KnowledgeBase)
        {
            const FString& Key = KnowledgePair.Key;
            float Value = KnowledgePair.Value;
            
            if (!AggregatedKnowledge.Contains(Key))
            {
                AggregatedKnowledge.Add(Key, 0.0f);
            }
            
            AggregatedKnowledge[Key] += Value;
        }
    }
    
    // Average the knowledge
    int32 AgentCount = AIAgents.Num();
    for (auto& KnowledgePair : AggregatedKnowledge)
    {
        KnowledgePair.Value /= AgentCount;
    }
    
    // Update collective knowledge
    CollectiveKnowledge = AggregatedKnowledge;
    
    // Distribute knowledge back to agents
    DistributeCollectiveKnowledge();
    
    UE_LOG(LogTemp, Log, TEXT("Updated collective knowledge with %d entries"), 
        CollectiveKnowledge.Num());
}

void UMingCollaborativeAINetwork::DistributeCollectiveKnowledge()
{
    for (auto& AgentPair : AIAgents)
    {
        FAIAgent& Agent = AgentPair.Value;
        
        // Update agent's knowledge with collective knowledge
        for (const auto& KnowledgePair : CollectiveKnowledge)
        {
            const FString& Key = KnowledgePair.Key;
            float CollectiveValue = KnowledgePair.Value;
            
            if (!Agent.KnowledgeBase.Contains(Key))
            {
                Agent.KnowledgeBase.Add(Key, CollectiveValue);
            }
            else
            {
                // Blend with existing knowledge
                float ExistingValue = Agent.KnowledgeBase[Key];
                float NewValue = (ExistingValue + CollectiveValue) * 0.5f;
                Agent.KnowledgeBase[Key] = NewValue;
            }
        }
    }
}

FNetworkMetrics UMingCollaborativeAINetwork::GetNetworkMetrics() const
{
    FNetworkMetrics Metrics;
    
    Metrics.TotalAgents = AIAgents.Num();
    Metrics.ActiveAgents = 0;
    Metrics.TotalChannels = CommunicationChannels.Num();
    Metrics.ActiveChannels = 0;
    Metrics.TotalTasks = TaskAllocations.Num();
    Metrics.PendingTasks = 0;
    Metrics.CompletedTasks = 0;
    
    // Count active agents
    for (const auto& AgentPair : AIAgents)
    {
        if (AgentPair.Value.Status == EAgentStatus::Active)
        {
            Metrics.ActiveAgents++;
        }
    }
    
    // Count active channels
    for (const auto& ChannelPair : CommunicationChannels)
    {
        if (ChannelPair.Value.bActive)
        {
            Metrics.ActiveChannels++;
        }
        
        Metrics.TotalMessages += ChannelPair.Value.Messages.Num();
    }
    
    // Count task statuses
    for (const auto& TaskPair : TaskAllocations)
    {
        switch (TaskPair.Value.Status)
        {
            case ETaskStatus::Pending:
                Metrics.PendingTasks++;
                break;
            case ETaskStatus::Completed:
                Metrics.CompletedTasks++;
                break;
            default:
                break;
        }
    }
    
    // Calculate network efficiency
    if (Metrics.TotalAgents > 0)
    {
        Metrics.NetworkEfficiency = (float)Metrics.ActiveAgents / Metrics.TotalAgents;
    }
    
    return Metrics;
}

void UMingCollaborativeAINetwork::OptimizeNetwork()
{
    // Optimize agent assignments
    OptimizeAgentAssignments();
    
    // Optimize communication channels
    OptimizeCommunicationChannels();
    
    // Optimize task allocations
    OptimizeTaskAllocations();
    
    UE_LOG(LogTemp, Log, TEXT("Network optimization completed"));
}

void UMingCollaborativeAINetwork::SimulateCollaboration(float Duration)
{
    if (!bNetworkActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("Network not active for simulation"));
        return;
    }
    
    // Simulate collaboration for specified duration
    float ElapsedTime = 0.0f;
    
    while (ElapsedTime < Duration)
    {
        // Process communication
        ProcessPendingCommunications();
        
        // Update agent states
        UpdateAgentStates();
        
        // Process distributed decisions
        ProcessDistributedDecision();
        
        // Update collective knowledge
        UpdateCollectiveKnowledge();
        
        // Process task allocations
        ProcessTaskAllocations();
        
        ElapsedTime += 0.1f; // 0.1 second time step
    }
    
    UE_LOG(LogTemp, Log, TEXT("Collaboration simulation completed for %.1f seconds"), Duration);
}

// Private helper functions

void UMingCollaborativeAINetwork::ProcessMessage(const FAICommunication& Message)
{
    const FCommunicationChannel& Channel = CommunicationChannels[Message.ChannelID];
    
    switch (Channel.Type)
    {
        case ECommunicationType::Broadcast:
            ProcessBroadcastMessage(Message);
            break;
        case ECommunicationType::PeerToPeer:
            ProcessPeerToPeerMessage(Message);
            break;
        case ECommunicationType::Multicast:
            ProcessMulticastMessage(Message);
            break;
        case ECommunicationType::Hierarchical:
            ProcessHierarchicalMessage(Message);
            break;
        case ECommunicationType::EventDriven:
            ProcessEventDrivenMessage(Message);
            break;
        case ECommunicationType::Stream:
            ProcessStreamMessage(Message);
            break;
    }
}

void UMingCollaborativeAINetwork::ProcessBroadcastMessage(const FAICommunication& Message)
{
    // Send message to all active agents
    for (auto& AgentPair : AIAgents)
    {
        FAIAgent& Agent = AgentPair.Value;
        
        if (Agent.Status == EAgentStatus::Active)
        {
            ReceiveMessage(Agent, Message);
        }
    }
}

void UMingCollaborativeAINetwork::ProcessPeerToPeerMessage(const FAICommunication& Message)
{
    // Send message only to specified recipient
    if (AIAgents.Contains(Message.RecipientID))
    {
        FAIAgent& Recipient = AIAgents[Message.RecipientID];
        ReceiveMessage(Recipient, Message);
    }
}

void UMingCollaborativeAINetwork::ProcessMulticastMessage(const FAICommunication& Message)
{
    // Send message to multiple recipients
    for (const FString& RecipientID : Message.Recipients)
    {
        if (AIAgents.Contains(RecipientID))
        {
            FAIAgent& Recipient = AIAgents[RecipientID];
            ReceiveMessage(Recipient, Message);
        }
    }
}

void UMingCollaborativeAINetwork::ProcessHierarchicalMessage(const FAICommunication& Message)
{
    // Send message based on hierarchy (simplified)
    const FAIAgent& Sender = AIAgents[Message.SenderID];
    
    // Send to agents with lower priority
    for (auto& AgentPair : AIAgents)
    {
        FAIAgent& Agent = AgentPair.Value;
        
        if (Agent.Status == EAgentStatus::Active && 
            Agent.Priority < Sender.Priority)
        {
            ReceiveMessage(Agent, Message);
        }
    }
}

void UMingCollaborativeAINetwork::ProcessEventDrivenMessage(const FAICommunication& Message)
{
    // Process message based on event type
    for (auto& AgentPair : AIAgents)
    {
        FAIAgent& Agent = AgentPair.Value;
        
        if (Agent.Status == EAgentStatus::Active)
        {
            // Check if agent subscribes to this event type
            if (Agent.SubscribedEvents.Contains(Message.MessageType))
            {
                ReceiveMessage(Agent, Message);
            }
        }
    }
}

void UMingCollaborativeAINetwork::ProcessStreamMessage(const FAICommunication& Message)
{
    // Process streaming message (continuous data)
    for (auto& AgentPair : AIAgents)
    {
        FAIAgent& Agent = AgentPair.Value;
        
        if (Agent.Status == EAgentStatus::Active)
        {
            // Check if agent is subscribed to this stream
            if (Agent.SubscribedStreams.Contains(Message.ChannelID))
            {
                ReceiveMessage(Agent, Message);
            }
        }
    }
}

void UMingCollaborativeAINetwork::ReceiveMessage(FAIAgent& Agent, const FAICommunication& Message)
{
    // Add message to agent's inbox
    Agent.MessageInbox.Add(Message);
    
    // Limit inbox size
    if (Agent.MessageInbox.Num() > 100)
    {
        Agent.MessageInbox.RemoveAt(0);
    }
    
    // Update agent's last update time
    Agent.LastUpdateTime = FDateTime::Now();
}

float UMingCollaborativeAINetwork::CalculateTaskSuitability(const FAIAgent& Agent, const FString& TaskID) const
{
    // Calculate suitability based on agent capabilities and task requirements
    float Suitability = 0.0f;
    
    // Base suitability on agent's capabilities
    Suitability += Agent.Capabilities.Num() * 0.1f;
    
    // Consider agent's current workload
    Suitability += (1.0f - Agent.CurrentWorkload) * 0.3f;
    
    // Consider agent's performance history
    Suitability += Agent.PerformanceScore * 0.2f;
    
    // Consider agent's availability
    if (Agent.Status == EAgentStatus::Active)
    {
        Suitability += 0.2f;
    }
    
    return FMath::Clamp(Suitability, 0.0f, 1.0f);
}

FDecisionVote UMingCollaborativeAINetwork::GenerateAgentVote(const FAIAgent& Agent) const
{
    FDecisionVote Vote;
    Vote.AgentID = Agent.AgentID;
    Vote.DecisionID = FString::Printf(TEXT("Decision_%s"), *FDateTime::Now().ToString());
    Vote.Vote = FMath::RandBool(); // Simplified random voting
    Vote.Confidence = Agent.PerformanceScore;
    Vote.Timestamp = FDateTime::Now();
    
    return Vote;
}

FDecisionResult UMingCollaborativeAINetwork::ProcessVotes(const TArray<FDecisionVote>& Votes) const
{
    FDecisionResult Result;
    Result.DecisionID = Votes[0].DecisionID;
    Result.TotalVotes = Votes.Num();
    Result.YesVotes = 0;
    Result.NoVotes = 0;
    Result.AbstainVotes = 0;
    Result.Timestamp = FDateTime::Now();
    
    // Count votes
    for (const FDecisionVote& Vote : Votes)
    {
        switch (Vote.Vote)
        {
            case true:
                Result.YesVotes++;
                break;
            case false:
                Result.NoVotes++;
                break;
        }
    }
    
    // Determine decision
    if (Result.YesVotes > Result.NoVotes)
    {
        Result.Decision = TEXT("Approved");
    }
    else if (Result.NoVotes > Result.YesVotes)
    {
        Result.Decision = TEXT("Rejected");
    }
    else
    {
        Result.Decision = TEXT("Tie");
    }
    
    // Calculate confidence
    float TotalVotes = Result.YesVotes + Result.NoVotes;
    if (TotalVotes > 0)
    {
        float MajorityVotes = FMath::Max(Result.YesVotes, Result.NoVotes);
        Result.Confidence = MajorityVotes / TotalVotes;
    }
    
    return Result;
}

void UMingCollaborativeAINetwork::OptimizeAgentAssignments()
{
    // Reassign agents to improve efficiency
    for (auto& TaskPair : TaskAllocations)
    {
        FTaskAllocation& Allocation = TaskPair.Value;
        
        if (Allocation.Status == ETaskStatus::Pending)
        {
            // Find better agents for this task
            TArray<FString> OptimalAgents = GetOptimalAgentsForTask(Allocation.TaskID);
            
            if (OptimalAgents.Num() > 0)
            {
                ReallocateTask(Allocation.TaskID, OptimalAgents);
            }
        }
    }
}

void UMingCollaborativeAINetwork::OptimizeCommunicationChannels()
{
    // Remove inactive channels
    for (auto& ChannelPair : CommunicationChannels)
    {
        FCommunicationChannel& Channel = ChannelPair.Value;
        
        // Check if channel has been inactive for too long
        FTimespan InactiveTime = FDateTime::Now() - Channel.LastActivity;
        
        if (InactiveTime.GetTotalSeconds() > 300.0f) // 5 minutes
        {
            Channel.bActive = false;
        }
    }
}

void UMingCollaborativeAINetwork::OptimizeTaskAllocations()
{
    // Balance workload across agents
    TMap<FString, float> AgentWorkloads;
    
    // Calculate current workloads
    for (const auto& TaskPair : TaskAllocations)
    {
        const FTaskAllocation& Allocation = TaskPair.Value;
        
        for (const FString& AgentID : Allocation.AssignedAgents)
        {
            if (!AgentWorkloads.Contains(AgentID))
            {
                AgentWorkloads.Add(AgentID, 0.0f);
            }
            
            AgentWorkloads[AgentID] += Allocation.Priority;
        }
    }
    
    // Rebalance tasks if needed
    for (auto& TaskPair : TaskAllocations)
    {
        FTaskAllocation& Allocation = TaskPair.Value;
        
        if (Allocation.Status == ETaskStatus::InProgress)
        {
            // Check if any assigned agent is overloaded
            bool bNeedsRebalancing = false;
            
            for (const FString& AgentID : Allocation.AssignedAgents)
            {
                if (AgentWorkloads.Contains(AgentID) && AgentWorkloads[AgentID] > 0.8f)
                {
                    bNeedsRebalancing = true;
                    break;
                }
            }
            
            if (bNeedsRebalancing)
            {
                // Find less loaded agents
                TArray<FString> LessLoadedAgents;
                
                for (const auto& WorkloadPair : AgentWorkloads)
                {
                    if (WorkloadPair.Value < 0.5f)
                    {
                        LessLoadedAgents.Add(WorkloadPair.Key);
                    }
                }
                
                if (LessLoadedAgents.Num() > 0)
                {
                    // Reallocate some work to less loaded agents
                    ReallocateTask(Allocation.TaskID, LessLoadedAgents);
                }
            }
        }
    }
}

void UMingCollaborativeAINetwork::ProcessPendingCommunications()
{
    // Process all pending messages in agent inboxes
    for (auto& AgentPair : AIAgents)
    {
        FAIAgent& Agent = AgentPair.Value;
        
        for (const FAICommunication& Message : Agent.MessageInbox)
        {
            // Process message based on agent's logic
            ProcessAgentMessage(Agent, Message);
        }
        
        // Clear processed messages
        Agent.MessageInbox.Empty();
    }
}

void UMingCollaborativeAINetwork::ProcessAgentMessage(FAIAgent& Agent, const FAICommunication& Message)
{
    // Update agent's knowledge based on message
    if (Message.MessageData.Contains("knowledge_update"))
    {
        FString KnowledgeKey = Message.MessageData["knowledge_update"];
        float KnowledgeValue = FCString::Atof(*Message.MessageData["knowledge_value"]);
        
        if (!Agent.KnowledgeBase.Contains(KnowledgeKey))
        {
            Agent.KnowledgeBase.Add(KnowledgeKey, KnowledgeValue);
        }
        else
        {
            Agent.KnowledgeBase[KnowledgeKey] = (Agent.KnowledgeBase[KnowledgeKey] + KnowledgeValue) * 0.5f;
        }
    }
    
    // Update agent's performance based on feedback
    if (Message.MessageData.Contains("performance_feedback"))
    {
        float Feedback = FCString::Atof(*Message.MessageData["performance_feedback"]);
        Agent.PerformanceScore = FMath::Clamp(Agent.PerformanceScore + Feedback * 0.1f, 0.0f, 1.0f);
    }
}

void UMingCollaborativeAINetwork::UpdateAgentStates()
{
    for (auto& AgentPair : AIAgents)
    {
        FAIAgent& Agent = AgentPair.Value;
        
        // Update agent's current workload
        Agent.CurrentWorkload = CalculateAgentWorkload(Agent);
        
        // Update agent's performance score
        UpdateAgentPerformance(Agent);
        
        // Update agent's last update time
        Agent.LastUpdateTime = FDateTime::Now();
    }
}

float UMingCollaborativeAINetwork::CalculateAgentWorkload(const FAIAgent& Agent) const
{
    float Workload = 0.0f;
    
    // Calculate based on assigned tasks
    for (const auto& TaskPair : TaskAllocations)
    {
        const FTaskAllocation& Allocation = TaskPair.Value;
        
        if (Allocation.AssignedAgents.Contains(Agent.AgentID))
        {
            Workload += Allocation.Priority;
        }
    }
    
    return FMath::Clamp(Workload / 10.0f, 0.0f, 1.0f); // Normalize to 0-1
}

void UMingCollaborativeAINetwork::UpdateAgentPerformance(FAIAgent& Agent)
{
    // Update performance based on recent task completions
    int32 CompletedTasks = 0;
    int32 TotalTasks = 0;
    
    for (const auto& TaskPair : TaskAllocations)
    {
        const FTaskAllocation& Allocation = TaskPair.Value;
        
        if (Allocation.AssignedAgents.Contains(Agent.AgentID))
        {
            TotalTasks++;
            
            if (Allocation.Status == ETaskStatus::Completed)
            {
                CompletedTasks++;
            }
        }
    }
    
    if (TotalTasks > 0)
    {
        float SuccessRate = (float)CompletedTasks / TotalTasks;
        Agent.PerformanceScore = FMath::Lerp(Agent.PerformanceScore, SuccessRate, 0.1f);
    }
}

void UMingCollaborativeAINetwork::ProcessTaskAllocations()
{
    for (auto& TaskPair : TaskAllocations)
    {
        FTaskAllocation& Allocation = TaskPair.Value;
        
        switch (Allocation.Status)
        {
            case ETaskStatus::Pending:
                // Start task if agents are available
                if (Allocation.AssignedAgents.Num() > 0)
                {
                    Allocation.Status = ETaskStatus::InProgress;
                    Allocation.StartTime = FDateTime::Now();
                }
                break;
                
            case ETaskStatus::InProgress:
                // Check if task should be completed
                if (ShouldCompleteTask(Allocation))
                {
                    Allocation.Status = ETaskStatus::Completed;
                    Allocation.CompletionTime = FDateTime::Now();
                    
                    // Update agent performance
                    for (const FString& AgentID : Allocation.AssignedAgents)
                    {
                        if (AIAgents.Contains(AgentID))
                        {
                            AIAgents[AgentID].PerformanceScore += 0.1f;
                        }
                    }
                }
                break;
                
            case ETaskStatus::Completed:
                // Clean up completed tasks after some time
                FTimespan TimeSinceCompletion = FDateTime::Now() - Allocation.CompletionTime;
                if (TimeSinceCompletion.GetTotalSeconds() > 60.0f) // 1 minute
                {
                    TaskAllocations.Remove(TaskPair.Key);
                }
                break;
                
            default:
                break;
        }
    }
}

bool UMingCollaborativeAINetwork::ShouldCompleteTask(const FTaskAllocation& Allocation) const
{
    // Simplified task completion logic
    FTimespan ElapsedTime = FDateTime::Now() - Allocation.StartTime;
    
    // Task completes based on priority and elapsed time
    float CompletionTime = 10.0f / (Allocation.Priority + 0.1f); // Higher priority = faster completion
    
    return ElapsedTime.GetTotalSeconds() >= CompletionTime;
}
