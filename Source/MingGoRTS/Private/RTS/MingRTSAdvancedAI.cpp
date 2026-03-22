#include "RTS/MingRTSAdvancedAI.h"

UMingRTSAdvancedAI::UMingRTSAdvancedAI()
{
}

void UMingRTSAdvancedAI::InitializeAdvancedAI()
{
    AIBehaviorPatterns.Empty();
    AITacticalRoles.Empty();
    AIPersonalities.Empty();
    AIMemories.Empty();
    AIEmotionalStates.Empty();
    AIStrategyPlans.Empty();
    TeamMembers.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSAdvancedAI: Advanced AI system initialized"));
}

void UMingRTSAdvancedAI::SetAIBehaviorPattern(const FString& AIControllerID, ERTSAIBehaviorPattern NewPattern)
{
    AIBehaviorPatterns.Add(AIControllerID, NewPattern);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSAdvancedAI: AI %s behavior pattern set to %d"), 
        *AIControllerID, (int32)NewPattern);
    
    OnAIBehaviorChanged.Broadcast(AIControllerID, NewPattern);
    
    // Create corresponding strategy plan
    CreateStrategyPlan(AIControllerID, NewPattern);
}

ERTSAIBehaviorPattern UMingRTSAdvancedAI::GetAIBehaviorPattern(const FString& AIControllerID) const
{
    if (AIBehaviorPatterns.Contains(AIControllerID))
    {
        return AIBehaviorPatterns[AIControllerID];
    }
    return ERTSAIBehaviorPattern::Balanced;
}

void UMingRTSAdvancedAI::SetAITacticalRole(const FString& AIControllerID, ERTSAITacticalRole NewRole)
{
    AITacticalRoles.Add(AIControllerID, NewRole);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSAdvancedAI: AI %s tactical role set to %d"), 
        *AIControllerID, (int32)NewRole);
}

void UMingRTSAdvancedAI::AssignAIPersonality(const FString& AIControllerID, const FRTSAIPersonality& Personality)
{
    AIPersonalities.Add(AIControllerID, Personality);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSAdvancedAI: AI %s personality assigned (Aggression: %.2f, Adaptability: %.2f)"), 
        *AIControllerID, Personality.Aggression, Personality.Adaptability);
}

FRTSAIPersonality UMingRTSAdvancedAI::GetAIPersonality(const FString& AIControllerID) const
{
    if (AIPersonalities.Contains(AIControllerID))
    {
        return AIPersonalities[AIControllerID];
    }
    return FRTSAIPersonality();
}

void UMingRTSAdvancedAI::AddAIMemory(const FString& AIControllerID, const FRTSAIMemory& Memory)
{
    if (!AIMemories.Contains(AIControllerID))
    {
        AIMemories.Add(AIControllerID, TArray<FRTSAIMemory>());
    }
    
    AIMemories[AIControllerID].Add(Memory);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSAdvancedAI: AI %s added %s memory (Importance: %.2f)"), 
        *AIControllerID, *UEnum::GetValueAsString(Memory.MemoryType), Memory.Importance);
    
    // Cleanup old memories if too many
    if (AIMemories[AIControllerID].Num() > 100)
    {
        CleanupOldMemories(AIControllerID);
    }
}

TArray<FRTSAIMemory> UMingRTSAdvancedAI::GetAIMemories(const FString& AIControllerID, ERTSAIMemoryType MemoryType) const
{
    TArray<FRTSAIMemory> FilteredMemories;
    
    if (AIMemories.Contains(AIControllerID))
    {
        for (const auto& Memory : AIMemories[AIControllerID])
        {
            if (Memory.MemoryType == MemoryType)
            {
                FilteredMemories.Add(Memory);
            }
        }
    }
    
    return FilteredMemories;
}

void UMingRTSAdvancedAI::UpdateAIEmotionalState(const FString& AIControllerID, ERTSAIEmotionalState NewState)
{
    ERTSAIEmotionalState OldState = ERTSAIEmotionalState::Calm;
    if (AIEmotionalStates.Contains(AIControllerID))
    {
        OldState = AIEmotionalStates[AIControllerID];
    }
    
    AIEmotionalStates.Add(AIControllerID, NewState);
    
    if (OldState != NewState)
    {
        UE_LOG(LogTemp, Log, TEXT("MingRTSAdvancedAI: AI %s emotional state changed from %d to %d"), 
            *AIControllerID, (int32)OldState, (int32)NewState);
        
        OnAIEmotionalStateChanged.Broadcast(AIControllerID, NewState);
    }
}

ERTSAIEmotionalState UMingRTSAdvancedAI::GetAIEmotionalState(const FString& AIControllerID) const
{
    if (AIEmotionalStates.Contains(AIControllerID))
    {
        return AIEmotionalStates[AIControllerID];
    }
    return ERTSAIEmotionalState::Calm;
}

FRTSTacticalDecision UMingRTSAdvancedAI::MakeTacticalDecision(const FString& AIControllerID)
{
    FRTSTacticalDecision Decision;
    
    TArray<FRTSTacticalDecision> Options = GenerateTacticalOptions(AIControllerID);
    
    if (Options.Num() > 0)
    {
        // Choose best option based on confidence
        float BestConfidence = 0.0f;
        int32 BestIndex = 0;
        
        for (int32 i = 0; i < Options.Num(); ++i)
        {
            float Confidence = CalculateDecisionConfidence(AIControllerID, Options[i]);
            if (Confidence > BestConfidence)
            {
                BestConfidence = Confidence;
                BestIndex = i;
            }
        }
        
        Decision = Options[BestIndex];
        Decision.Confidence = BestConfidence;
        
        UE_LOG(LogTemp, Log, TEXT("MingRTSAdvancedAI: AI %s made tactical decision %s (Confidence: %.2f)"), 
            *AIControllerID, *Decision.DecisionID, Decision.Confidence);
        
        OnAITacticalDecisionMade.Broadcast(AIControllerID, Decision);
    }
    
    return Decision;
}

void UMingRTSAdvancedAI::ExecuteTacticalDecision(const FString& AIControllerID, const FRTSTacticalDecision& Decision)
{
    UE_LOG(LogTemp, Log, TEXT("MingRTSAdvancedAI: AI %s executing decision %s at location (%.1f, %.1f, %.1f)"), 
        *AIControllerID, *Decision.DecisionID, Decision.TargetLocation.X, Decision.TargetLocation.Y, Decision.TargetLocation.Z);
    
    // Implementation would integrate with actual unit control
    // This is a placeholder for the execution logic
}

void UMingRTSAdvancedAI::CreateStrategyPlan(const FString& AIControllerID, ERTSAIBehaviorPattern StrategyType)
{
    FRTSAIStrategyPlan Plan;
    Plan.PlanID = FString::Printf(TEXT("Plan_%s_%d"), *AIControllerID, FMath::Rand());
    Plan.StrategyType = StrategyType;
    Plan.TotalPhases = 3.0f;
    Plan.CurrentPhase = 0.0f;
    
    // Generate tactical decisions based on strategy type
    for (int32 i = 0; i < 5; ++i)
    {
        FRTSTacticalDecision Decision;
        Decision.DecisionID = FString::Printf(TEXT("Decision_%d"), i);
        Decision.Priority = 1.0f - (i * 0.2f);
        Plan.Decisions.Add(Decision);
    }
    
    AIStrategyPlans.Add(AIControllerID, Plan);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSAdvancedAI: AI %s strategy plan created (Type: %d, Phases: %.0f)"), 
        *AIControllerID, (int32)StrategyType, Plan.TotalPhases);
}

void UMingRTSAdvancedAI::ExecuteStrategyPlan(const FString& AIControllerID)
{
    if (!AIStrategyPlans.Contains(AIControllerID))
    {
        return;
    }
    
    FRTSAIStrategyPlan& Plan = AIStrategyPlans[AIControllerID];
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSAdvancedAI: AI %s executing strategy plan (Phase %.0f/%.0f)"), 
        *AIControllerID, Plan.CurrentPhase + 1, Plan.TotalPhases);
    
    // Execute current phase decisions
    for (auto& Decision : Plan.Decisions)
    {
        ExecuteTacticalDecision(AIControllerID, Decision);
    }
    
    Plan.CurrentPhase += 1.0f;
    
    if (Plan.CurrentPhase >= Plan.TotalPhases)
    {
        UE_LOG(LogTemp, Log, TEXT("MingRTSAdvancedAI: AI %s strategy plan completed"), *AIControllerID);
    }
}

void UMingRTSAdvancedAI::AdaptToPlayerStrategy(const FString& AIControllerID, const FString& PlayerID)
{
    ERTSAIBehaviorPattern PlayerPattern = AnalyzePlayerPattern(PlayerID);
    ERTSAIBehaviorPattern CounterPattern;
    
    // Determine counter strategy
    switch (PlayerPattern)
    {
        case ERTSAIBehaviorPattern::Rush:
            CounterPattern = ERTSAIBehaviorPattern::Defensive;
            break;
        case ERTSAIBehaviorPattern::Turtle:
            CounterPattern = ERTSAIBehaviorPattern::Boom;
            break;
        case ERTSAIBehaviorPattern::Boom:
            CounterPattern = ERTSAIBehaviorPattern::Rush;
            break;
        case ERTSAIBehaviorPattern::Aggressive:
            CounterPattern = ERTSAIBehaviorPattern::Turtle;
            break;
        default:
            CounterPattern = ERTSAIBehaviorPattern::Balanced;
            break;
    }
    
    SetAIBehaviorPattern(AIControllerID, CounterPattern);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSAdvancedAI: AI %s adapted to counter player %s strategy (Pattern: %d -> Counter: %d)"), 
        *AIControllerID, *PlayerID, (int32)PlayerPattern, (int32)CounterPattern);
    
    OnAIAdaptationOccurred.Broadcast(AIControllerID);
}

void UMingRTSAdvancedAI::LearnFromMatch(const FString& AIControllerID, bool bWon, const FString& MatchData)
{
    UE_LOG(LogTemp, Log, TEXT("MingRTSAdvancedAI: AI %s learning from match (Result: %s)"), 
        *AIControllerID, bWon ? TEXT("Victory") : TEXT("Defeat"));
    
    UpdateAIPersonalityFromExperience(AIControllerID, bWon);
    
    // Store match data as memory
    FRTSAIMemory MatchMemory;
    MatchMemory.MemoryID = FString::Printf(TEXT("Match_%s"), *MatchData);
    MatchMemory.EventDescription = bWon ? TEXT("Victory") : TEXT("Defeat");
    MatchMemory.MemoryType = ERTSAIMemoryType::LongTerm;
    MatchMemory.Importance = 0.8f;
    MatchMemory.Timestamp = FPlatformTime::Seconds();
    
    AddAIMemory(AIControllerID, MatchMemory);
}

void UMingRTSAdvancedAI::SetTeamCooperation(const FString& AIControllerID, int32 TeamID, bool bEnableCooperation)
{
    if (bEnableCooperation)
    {
        if (!TeamMembers.Contains(TeamID))
        {
            TeamMembers.Add(TeamID, TArray<FString>());
        }
        
        if (!TeamMembers[TeamID].Contains(AIControllerID))
        {
            TeamMembers[TeamID].Add(AIControllerID);
        }
        
        UE_LOG(LogTemp, Log, TEXT("MingRTSAdvancedAI: AI %s joined team %d"), *AIControllerID, TeamID);
    }
    else
    {
        if (TeamMembers.Contains(TeamID))
        {
            TeamMembers[TeamID].Remove(AIControllerID);
            UE_LOG(LogTemp, Log, TEXT("MingRTSAdvancedAI: AI %s left team %d"), *AIControllerID, TeamID);
        }
    }
}

void UMingRTSAdvancedAI::ProcessTeamCoordination(int32 TeamID)
{
    if (!TeamMembers.Contains(TeamID))
    {
        return;
    }
    
    const TArray<FString>& Members = TeamMembers[TeamID];
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSAdvancedAI: Processing coordination for team %d (%d members)"), 
        TeamID, Members.Num());
    
    // Simple coordination: assign different tactical roles
    for (int32 i = 0; i < Members.Num(); ++i)
    {
        ERTSAITacticalRole Role = static_cast<ERTSAITacticalRole>(i % static_cast<int32>(ERTSAITacticalRole::Specialist) + 1);
        SetAITacticalRole(Members[i], Role);
    }
}

float UMingRTSAdvancedAI::EvaluateThreatLevel(const FString& AIControllerID, const FVector& Location)
{
    // Simple threat evaluation based on memory
    float ThreatLevel = 0.0f;
    
    if (AIMemories.Contains(AIControllerID))
    {
        for (const auto& Memory : AIMemories[AIControllerID])
        {
            if (Memory.MemoryType == ERTSAIMemoryType::Tactical)
            {
                float Distance = FVector::Distance(Location, Memory.Location);
                if (Distance < 1000.0f)
                {
                    ThreatLevel += Memory.Importance * (1.0f - Distance / 1000.0f);
                }
            }
        }
    }
    
    return FMath::Clamp(ThreatLevel, 0.0f, 1.0f);
}

TArray<FRTSTacticalDecision> UMingRTSAdvancedAI::GenerateTacticalOptions(const FString& AIControllerID)
{
    TArray<FRTSTacticalDecision> Options;
    
    // Generate different tactical options based on behavior pattern
    ERTSAIBehaviorPattern Pattern = GetAIBehaviorPattern(AIControllerID);
    
    FRTSTacticalDecision AttackOption;
    AttackOption.DecisionID = TEXT("Attack");
    AttackOption.ActionType = TEXT("Attack");
    AttackOption.Priority = (Pattern == ERTSAIBehaviorPattern::Aggressive || Pattern == ERTSAIBehaviorPattern::Rush) ? 0.9f : 0.5f;
    Options.Add(AttackOption);
    
    FRTSTacticalDecision DefendOption;
    DefendOption.DecisionID = TEXT("Defend");
    DefendOption.ActionType = TEXT("Defend");
    DefendOption.Priority = (Pattern == ERTSAIBehaviorPattern::Turtle || Pattern == ERTSAIBehaviorPattern::Defensive) ? 0.9f : 0.5f;
    Options.Add(DefendOption);
    
    FRTSTacticalDecision ExpandOption;
    ExpandOption.DecisionID = TEXT("Expand");
    ExpandOption.ActionType = TEXT("Expand");
    ExpandOption.Priority = (Pattern == ERTSAIBehaviorPattern::Boom || Pattern == ERTSAIBehaviorPattern::Expand) ? 0.9f : 0.6f;
    Options.Add(ExpandOption);
    
    FRTSTacticalDecision ScoutOption;
    ScoutOption.DecisionID = TEXT("Scout");
    ScoutOption.ActionType = TEXT("Scout");
    ScoutOption.Priority = 0.4f;
    Options.Add(ScoutOption);
    
    return Options;
}

ERTSAIBehaviorPattern UMingRTSAdvancedAI::AnalyzePlayerPattern(const FString& PlayerID) const
{
    // Placeholder for player pattern analysis
    // Would analyze historical data to determine player behavior
    return ERTSAIBehaviorPattern::Balanced;
}

float UMingRTSAdvancedAI::CalculateDecisionConfidence(const FString& AIControllerID, const FRTSTacticalDecision& Decision) const
{
    float BaseConfidence = Decision.Priority;
    
    // Adjust confidence based on personality
    if (AIPersonalities.Contains(AIControllerID))
    {
        const FRTSAIPersonality& Personality = AIPersonalities[AIControllerID];
        
        if (Decision.ActionType == TEXT("Attack"))
        {
            BaseConfidence *= Personality.Aggression;
        }
        else if (Decision.ActionType == TEXT("Defend"))
        {
            BaseConfidence *= Personality.Defensiveness;
        }
        else if (Decision.ActionType == TEXT("Expand"))
        {
            BaseConfidence *= Personality.Expansionism;
        }
    }
    
    return FMath::Clamp(BaseConfidence, 0.0f, 1.0f);
}

void UMingRTSAdvancedAI::UpdateAIPersonalityFromExperience(const FString& AIControllerID, bool bSuccess)
{
    if (!AIPersonalities.Contains(AIControllerID))
    {
        return;
    }
    
    FRTSAIPersonality& Personality = AIPersonalities[AIControllerID];
    
    // Small adjustments based on success/failure
    if (bSuccess)
    {
        Personality.Adaptability = FMath::Min(Personality.Adaptability + 0.01f, 1.0f);
        Personality.RiskTolerance = FMath::Min(Personality.RiskTolerance + 0.01f, 1.0f);
    }
    else
    {
        Personality.Adaptability = FMath::Min(Personality.Adaptability + 0.02f, 1.0f); // Learn more from failure
        Personality.RiskTolerance = FMath::Max(Personality.RiskTolerance - 0.01f, 0.0f);
    }
}

void UMingRTSAdvancedAI::CleanupOldMemories(const FString& AIControllerID)
{
    if (!AIMemories.Contains(AIControllerID))
    {
        return;
    }
    
    TArray<FRTSAIMemory>& Memories = AIMemories[AIControllerID];
    
    // Remove oldest short-term memories
    Memories.Sort([](const FRTSAIMemory& A, const FRTSAIMemory& B) {
        return A.Timestamp > B.Timestamp;
    });
    
    // Keep only the most important and recent memories
    for (int32 i = Memories.Num() - 1; i >= 50; --i)
    {
        if (Memories[i].MemoryType == ERTSAIMemoryType::ShortTerm && Memories[i].Importance < 0.5f)
        {
            Memories.RemoveAt(i);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSAdvancedAI: AI %s memory cleaned up (%d memories remaining)"), 
        *AIControllerID, Memories.Num());
}

ERTSAIEmotionalState UMingRTSAdvancedAI::DetermineEmotionalResponse(const FString& AIControllerID, float ThreatLevel, float ResourceLevel)
{
    if (ThreatLevel > 0.8f && ResourceLevel < 0.3f)
    {
        return ERTSAIEmotionalState::Desperate;
    }
    else if (ThreatLevel > 0.6f)
    {
        return ERTSAIEmotionalState::Stressed;
    }
    else if (ResourceLevel > 0.7f && ThreatLevel < 0.3f)
    {
        return ERTSAIEmotionalState::Confident;
    }
    else if (ThreatLevel > 0.4f)
    {
        return ERTSAIEmotionalState::Cautious;
    }
    
    return ERTSAIEmotionalState::Calm;
}
