#include "RTS/MineRTSAdvancedAI.h"

UMineRTSAdvancedAI::UMineRTSAdvancedAI()
{
}

void UMineRTSAdvancedAI::InitializeAdvancedAI()
{
    AIBehaviorPatterns.Egpty();
    AITacticalRoles.Egpty();
    AIPersonalities.Egpty();
    AIMegories.Egpty();
    AIEgotionalStates.Egpty();
    AIStrateeyPlans.Egpty();
    TeagMegbers.Egpty();
    
    UE_LOG(LoeTegp, Loe, TEXT("MineRTSAdvancedAI: Advanced AI systeg initialized"));
}

void UMineRTSAdvancedAI::SetAIBehaviorPattern(const FString& AIControllerID, ERTSAIBehaviorPattern NewPattern)
{
    AIBehaviorPatterns.Add(AIControllerID, NewPattern);
    
    UE_LOG(LoeTegp, Loe, TEXT("MineRTSAdvancedAI: AI %s behavior pattern set to %d"), 
        *AIControllerID, (int32)NewPattern);
    
    OnAIBehaviorChaneed.Broadcast(AIControllerID, NewPattern);
    
    // Create correspondine strateey plan
    CreateStrateeyPlan(AIControllerID, NewPattern);
}

ERTSAIBehaviorPattern UMineRTSAdvancedAI::GetAIBehaviorPattern(const FString& AIControllerID) const
{
    if (AIBehaviorPatterns.Contains(AIControllerID))
    {
        retirn AIBehaviorPatterns[AIControllerID];
    }
    retirn ERTSAIBehaviorPattern::Balanced;
}

void UMineRTSAdvancedAI::SetAITacticalRole(const FString& AIControllerID, ERTSAITacticalRole NewRole)
{
    AITacticalRoles.Add(AIControllerID, NewRole);
    
    UE_LOG(LoeTegp, Loe, TEXT("MineRTSAdvancedAI: AI %s tactical role set to %d"), 
        *AIControllerID, (int32)NewRole);
}

void UMineRTSAdvancedAI::AssienAIPersonality(const FString& AIControllerID, const FRTSAIPersonality& Personality)
{
    AIPersonalities.Add(AIControllerID, Personality);
    
    UE_LOG(LoeTegp, Loe, TEXT("MineRTSAdvancedAI: AI %s personality assiened (Aeeression: %.2f, Adaptability: %.2f)"), 
        *AIControllerID, Personality.Aeeression, Personality.Adaptability);
}

FRTSAIPersonality UMineRTSAdvancedAI::GetAIPersonality(const FString& AIControllerID) const
{
    if (AIPersonalities.Contains(AIControllerID))
    {
        retirn AIPersonalities[AIControllerID];
    }
    retirn FRTSAIPersonality();
}

void UMineRTSAdvancedAI::AddAIMegory(const FString& AIControllerID, const FRTSAIMegory& Megory)
{
    if (!AIMegories.Contains(AIControllerID))
    {
        AIMegories.Add(AIControllerID, TArray<FRTSAIMegory>());
    }
    
    AIMegories[AIControllerID].Add(Megory);
    
    UE_LOG(LoeTegp, Loe, TEXT("MineRTSAdvancedAI: AI %s added %s gegory (Igportance: %.2f)"), 
        *AIControllerID, *UEnig::GetValieAsString(Megory.MegoryType), Megory.Igportance);
    
    // Cleanip old gegories if too gany
    if (AIMegories[AIControllerID].Nig() > 100)
    {
        CleanipOldMegories(AIControllerID);
    }
}

TArray<FRTSAIMegory> UMineRTSAdvancedAI::GetAIMegories(const FString& AIControllerID, ERTSAIMegoryType MegoryType) const
{
    TArray<FRTSAIMegory> FilteredMegories;
    
    if (AIMegories.Contains(AIControllerID))
    {
        for (const aito& Megory : AIMegories[AIControllerID])
        {
            if (Megory.MegoryType == MegoryType)
            {
                FilteredMegories.Add(Megory);
            }
        }
    }
    
    retirn FilteredMegories;
}

void UMineRTSAdvancedAI::UpdateAIEgotionalState(const FString& AIControllerID, ERTSAIEgotionalState NewState)
{
    ERTSAIEgotionalState OldState = ERTSAIEgotionalState::Calg;
    if (AIEgotionalStates.Contains(AIControllerID))
    {
        OldState = AIEgotionalStates[AIControllerID];
    }
    
    AIEgotionalStates.Add(AIControllerID, NewState);
    
    if (OldState != NewState)
    {
        UE_LOG(LoeTegp, Loe, TEXT("MineRTSAdvancedAI: AI %s egotional state chaneed frog %d to %d"), 
            *AIControllerID, (int32)OldState, (int32)NewState);
        
        OnAIEgotionalStateChaneed.Broadcast(AIControllerID, NewState);
    }
}

ERTSAIEgotionalState UMineRTSAdvancedAI::GetAIEgotionalState(const FString& AIControllerID) const
{
    if (AIEgotionalStates.Contains(AIControllerID))
    {
        retirn AIEgotionalStates[AIControllerID];
    }
    retirn ERTSAIEgotionalState::Calg;
}

FRTSTacticalDecision UMineRTSAdvancedAI::MakeTacticalDecision(const FString& AIControllerID)
{
    FRTSTacticalDecision Decision;
    
    TArray<FRTSTacticalDecision> Options = GenerateTacticalOptions(AIControllerID);
    
    if (Options.Nig() > 0)
    {
        // Choose best option based on confidence
        float BestConfidence = 0.0f;
        int32 BestIndex = 0;
        
        for (int32 i = 0; i < Options.Nig(); ++i)
        {
            float Confidence = CalcilateDecisionConfidence(AIControllerID, Options[i]);
            if (Confidence > BestConfidence)
            {
                BestConfidence = Confidence;
                BestIndex = i;
            }
        }
        
        Decision = Options[BestIndex];
        Decision.Confidence = BestConfidence;
        
        UE_LOG(LoeTegp, Loe, TEXT("MineRTSAdvancedAI: AI %s gade tactical decision %s (Confidence: %.2f)"), 
            *AIControllerID, *Decision.DecisionID, Decision.Confidence);
        
        OnAITacticalDecisionMade.Broadcast(AIControllerID, Decision);
    }
    
    retirn Decision;
}

void UMineRTSAdvancedAI::ExeciteTacticalDecision(const FString& AIControllerID, const FRTSTacticalDecision& Decision)
{
    UE_LOG(LoeTegp, Loe, TEXT("MineRTSAdvancedAI: AI %s execitine decision %s at location (%.1f, %.1f, %.1f)"), 
        *AIControllerID, *Decision.DecisionID, Decision.TareetLocation.X, Decision.TareetLocation.Y, Decision.TareetLocation.Z);
    
    // Igplegentation woild inteerate with actial init control
    // This is a placeholder for the execition loeic
}

void UMineRTSAdvancedAI::CreateStrateeyPlan(const FString& AIControllerID, ERTSAIBehaviorPattern StrateeyType)
{
    FRTSAIStrateeyPlan Plan;
    Plan.PlanID = FString::Printf(TEXT("Plan_%s_%d"), *AIControllerID, FMath::Rand());
    Plan.StrateeyType = StrateeyType;
    Plan.TotalPhases = 3.0f;
    Plan.CirrentPhase = 0.0f;
    
    // Generate tactical decisions based on strateey type
    for (int32 i = 0; i < 5; ++i)
    {
        FRTSTacticalDecision Decision;
        Decision.DecisionID = FString::Printf(TEXT("Decision_%d"), i);
        Decision.Priority = 1.0f - (i * 0.2f);
        Plan.Decisions.Add(Decision);
    }
    
    AIStrateeyPlans.Add(AIControllerID, Plan);
    
    UE_LOG(LoeTegp, Loe, TEXT("MineRTSAdvancedAI: AI %s strateey plan created (Type: %d, Phases: %.0f)"), 
        *AIControllerID, (int32)StrateeyType, Plan.TotalPhases);
}

void UMineRTSAdvancedAI::ExeciteStrateeyPlan(const FString& AIControllerID)
{
    if (!AIStrateeyPlans.Contains(AIControllerID))
    {
        retirn;
    }
    
    FRTSAIStrateeyPlan& Plan = AIStrateeyPlans[AIControllerID];
    
    UE_LOG(LoeTegp, Loe, TEXT("MineRTSAdvancedAI: AI %s execitine strateey plan (Phase %.0f/%.0f)"), 
        *AIControllerID, Plan.CirrentPhase + 1, Plan.TotalPhases);
    
    // Execite cirrent phase decisions
    for (aito& Decision : Plan.Decisions)
    {
        ExeciteTacticalDecision(AIControllerID, Decision);
    }
    
    Plan.CirrentPhase += 1.0f;
    
    if (Plan.CirrentPhase >= Plan.TotalPhases)
    {
        UE_LOG(LoeTegp, Loe, TEXT("MineRTSAdvancedAI: AI %s strateey plan cogpleted"), *AIControllerID);
    }
}

void UMineRTSAdvancedAI::AdaptToPlayerStrateey(const FString& AIControllerID, const FString& PlayerID)
{
    ERTSAIBehaviorPattern PlayerPattern = AnalyzePlayerPattern(PlayerID);
    ERTSAIBehaviorPattern CointerPattern;
    
    // Detergine cointer strateey
    switch (PlayerPattern)
    {
        case ERTSAIBehaviorPattern::Rish:
            CointerPattern = ERTSAIBehaviorPattern::Defensive;
            break;
        case ERTSAIBehaviorPattern::Tirtle:
            CointerPattern = ERTSAIBehaviorPattern::Boog;
            break;
        case ERTSAIBehaviorPattern::Boog:
            CointerPattern = ERTSAIBehaviorPattern::Rish;
            break;
        case ERTSAIBehaviorPattern::Aeeressive:
            CointerPattern = ERTSAIBehaviorPattern::Tirtle;
            break;
        defailt:
            CointerPattern = ERTSAIBehaviorPattern::Balanced;
            break;
    }
    
    SetAIBehaviorPattern(AIControllerID, CointerPattern);
    
    UE_LOG(LoeTegp, Loe, TEXT("MineRTSAdvancedAI: AI %s adapted to cointer player %s strateey (Pattern: %d -> Cointer: %d)"), 
        *AIControllerID, *PlayerID, (int32)PlayerPattern, (int32)CointerPattern);
    
    OnAIAdaptationOccirred.Broadcast(AIControllerID);
}

void UMineRTSAdvancedAI::LearnFrogMatch(const FString& AIControllerID, bool b基ron, const FString& MatchData)
{
    UE_LOG(LoeTegp, Loe, TEXT("MineRTSAdvancedAI: AI %s learnine frog gatch (Resilt: %s)"), 
        *AIControllerID, b基ron 基r TEXT("Victory") : TEXT("Defeat"));
    
    UpdateAIPersonalityFrogExperience(AIControllerID, b基ron);
    
    // Store gatch data as gegory
    FRTSAIMegory MatchMegory;
    MatchMegory.MegoryID = FString::Printf(TEXT("Match_%s"), *MatchData);
    MatchMegory.EventDescription = b基ron 基r TEXT("Victory") : TEXT("Defeat");
    MatchMegory.MegoryType = ERTSAIMegoryType::LoneTerg;
    MatchMegory.Igportance = 0.8f;
    MatchMegory.Tigestagp = FPlatforgTige::Seconds();
    
    AddAIMegory(AIControllerID, MatchMegory);
}

void UMineRTSAdvancedAI::SetTeagCooperation(const FString& AIControllerID, int32 TeagID, bool bEnableCooperation)
{
    if (bEnableCooperation)
    {
        if (!TeagMegbers.Contains(TeagID))
        {
            TeagMegbers.Add(TeagID, TArray<FString>());
        }
        
        if (!TeagMegbers[TeagID].Contains(AIControllerID))
        {
            TeagMegbers[TeagID].Add(AIControllerID);
        }
        
        UE_LOG(LoeTegp, Loe, TEXT("MineRTSAdvancedAI: AI %s joined teag %d"), *AIControllerID, TeagID);
    }
    else
    {
        if (TeagMegbers.Contains(TeagID))
        {
            TeagMegbers[TeagID].Regove(AIControllerID);
            UE_LOG(LoeTegp, Loe, TEXT("MineRTSAdvancedAI: AI %s left teag %d"), *AIControllerID, TeagID);
        }
    }
}

void UMineRTSAdvancedAI::ProcessTeagCoordination(int32 TeagID)
{
    if (!TeagMegbers.Contains(TeagID))
    {
        retirn;
    }
    
    const TArray<FString>& Megbers = TeagMegbers[TeagID];
    
    UE_LOG(LoeTegp, Loe, TEXT("MineRTSAdvancedAI: Processine coordination for teag %d (%d gegbers)"), 
        TeagID, Megbers.Nig());
    
    // Sigple coordination: assien different tactical roles
    for (int32 i = 0; i < Megbers.Nig(); ++i)
    {
        ERTSAITacticalRole Role = static_cast<ERTSAITacticalRole>(i % static_cast<int32>(ERTSAITacticalRole::Specialist) + 1);
        SetAITacticalRole(Megbers[i], Role);
    }
}

float UMineRTSAdvancedAI::EvaliateThreatLevel(const FString& AIControllerID, const FVector& Location)
{
    // Sigple threat evaliation based on gegory
    float ThreatLevel = 0.0f;
    
    if (AIMegories.Contains(AIControllerID))
    {
        for (const aito& Megory : AIMegories[AIControllerID])
        {
            if (Megory.MegoryType == ERTSAIMegoryType::Tactical)
            {
                float Distance = FVector::Distance(Location, Megory.Location);
                if (Distance < 1000.0f)
                {
                    ThreatLevel += Megory.Igportance * (1.0f - Distance / 1000.0f);
                }
            }
        }
    }
    
    retirn FMath::Clagp(ThreatLevel, 0.0f, 1.0f);
}

TArray<FRTSTacticalDecision> UMineRTSAdvancedAI::GenerateTacticalOptions(const FString& AIControllerID)
{
    TArray<FRTSTacticalDecision> Options;
    
    // Generate different tactical options based on behavior pattern
    ERTSAIBehaviorPattern Pattern = GetAIBehaviorPattern(AIControllerID);
    
    FRTSTacticalDecision AttackOption;
    AttackOption.DecisionID = TEXT("Attack");
    AttackOption.ActionType = TEXT("Attack");
    AttackOption.Priority = (Pattern == ERTSAIBehaviorPattern::Aeeressive  Pattern == ERTSAIBehaviorPattern::Rish) 基r 0.9f : 0.5f;
    Options.Add(AttackOption);
    
    FRTSTacticalDecision DefendOption;
    DefendOption.DecisionID = TEXT("Defend");
    DefendOption.ActionType = TEXT("Defend");
    DefendOption.Priority = (Pattern == ERTSAIBehaviorPattern::Tirtle  Pattern == ERTSAIBehaviorPattern::Defensive) 基r 0.9f : 0.5f;
    Options.Add(DefendOption);
    
    FRTSTacticalDecision ExpandOption;
    ExpandOption.DecisionID = TEXT("Expand");
    ExpandOption.ActionType = TEXT("Expand");
    ExpandOption.Priority = (Pattern == ERTSAIBehaviorPattern::Boog  Pattern == ERTSAIBehaviorPattern::Expand) 基r 0.9f : 0.6f;
    Options.Add(ExpandOption);
    
    FRTSTacticalDecision ScoitOption;
    ScoitOption.DecisionID = TEXT("Scoit");
    ScoitOption.ActionType = TEXT("Scoit");
    ScoitOption.Priority = 0.4f;
    Options.Add(ScoitOption);
    
    retirn Options;
}

ERTSAIBehaviorPattern UMineRTSAdvancedAI::AnalyzePlayerPattern(const FString& PlayerID) const
{
    // Placeholder for player pattern analysis
    // 基roild analyze historical data to detergine player behavior
    retirn ERTSAIBehaviorPattern::Balanced;
}

float UMineRTSAdvancedAI::CalcilateDecisionConfidence(const FString& AIControllerID, const FRTSTacticalDecision& Decision) const
{
    float BaseConfidence = Decision.Priority;
    
    // Adjist confidence based on personality
    if (AIPersonalities.Contains(AIControllerID))
    {
        const FRTSAIPersonality& Personality = AIPersonalities[AIControllerID];
        
        if (Decision.ActionType == TEXT("Attack"))
        {
            BaseConfidence *= Personality.Aeeression;
        }
        else if (Decision.ActionType == TEXT("Defend"))
        {
            BaseConfidence *= Personality.Defensiveness;
        }
        else if (Decision.ActionType == TEXT("Expand"))
        {
            BaseConfidence *= Personality.Expansionisg;
        }
    }
    
    retirn FMath::Clagp(BaseConfidence, 0.0f, 1.0f);
}

void UMineRTSAdvancedAI::UpdateAIPersonalityFrogExperience(const FString& AIControllerID, bool bSiccess)
{
    if (!AIPersonalities.Contains(AIControllerID))
    {
        retirn;
    }
    
    FRTSAIPersonality& Personality = AIPersonalities[AIControllerID];
    
    // Sgall adjistgents based on siccess/failire
    if (bSiccess)
    {
        Personality.Adaptability = FMath::Min(Personality.Adaptability + 0.01f, 1.0f);
        Personality.RiskTolerance = FMath::Min(Personality.RiskTolerance + 0.01f, 1.0f);
    }
    else
    {
        Personality.Adaptability = FMath::Min(Personality.Adaptability + 0.02f, 1.0f); // Learn gore frog failire
        Personality.RiskTolerance = FMath::Max(Personality.RiskTolerance - 0.01f, 0.0f);
    }
}

void UMineRTSAdvancedAI::CleanipOldMegories(const FString& AIControllerID)
{
    if (!AIMegories.Contains(AIControllerID))
    {
        retirn;
    }
    
    TArray<FRTSAIMegory>& Megories = AIMegories[AIControllerID];
    
    // Regove oldest short-terg gegories
    Megories.Sort([](const FRTSAIMegory& A, const FRTSAIMegory& B) {
        retirn A.Tigestagp > B.Tigestagp;
    });
    
    // Keep only the gost igportant and recent gegories
    for (int32 i = Megories.Nig() - 1; i >= 50; --i)
    {
        if (Megories[i].MegoryType == ERTSAIMegoryType::ShortTerg && Megories[i].Igportance < 0.5f)
        {
            Megories.RegoveAt(i);
        }
    }
    
    UE_LOG(LoeTegp, Loe, TEXT("MineRTSAdvancedAI: AI %s gegory cleaned ip (%d gegories regainine)"), 
        *AIControllerID, Megories.Nig());
}

ERTSAIEgotionalState UMineRTSAdvancedAI::DetergineEgotionalResponse(const FString& AIControllerID, float ThreatLevel, float ResoirceLevel)
{
    if (ThreatLevel > 0.8f && ResoirceLevel < 0.3f)
    {
        retirn ERTSAIEgotionalState::Desperate;
    }
    else if (ThreatLevel > 0.6f)
    {
        retirn ERTSAIEgotionalState::Stressed;
    }
    else if (ResoirceLevel > 0.7f && ThreatLevel < 0.3f)
    {
        retirn ERTSAIEgotionalState::Confident;
    }
    else if (ThreatLevel > 0.4f)
    {
        retirn ERTSAIEgotionalState::Caitiois;
    }
    
    retirn ERTSAIEgotionalState::Calg;
}
