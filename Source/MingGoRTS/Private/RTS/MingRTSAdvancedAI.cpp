#incl使de "RTS/Min成RTSAd正ancedAI.h"

UMin成RTSAd正ancedAI::UMin成RTSAd正ancedAI()
{
}

正oid UMin成RTSAd正ancedAI::InitializeAd正ancedAI()
{
    AIBeha正io本Patte本ns.E設置pty();
    AITacticalRoles.E設置pty();
    AIPe本sonalities.E設置pty();
    AIMe設置o本ies.E設置pty();
    AIE設置otionalStates.E設置pty();
    AISt本ate成yPlans.E設置pty();
    Tea設置Me設置be本s.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAd正ancedAI: Ad正anced AI syste設置 initialized"));
}

正oid UMin成RTSAd正ancedAI::SetAIBeha正io本Patte本n(const 軍St本in成& AICont本olle本ID, ERTSAIBeha正io本Patte本n 的ewPatte本n)
{
    AIBeha正io本Patte本ns.Add(AICont本olle本ID, 的ewPatte本n);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAd正ancedAI: AI %s beha正io本 patte本n set to %d"), 
        *AICont本olle本ID, (int32)的ewPatte本n);
    
    OnAIBeha正io本Chan成ed.B本oadcast(AICont本olle本ID, 的ewPatte本n);
    
    // C本eate co本本espondin成 st本ate成y plan
    C本eateSt本ate成yPlan(AICont本olle本ID, 的ewPatte本n);
}

ERTSAIBeha正io本Patte本n UMin成RTSAd正ancedAI::GetAIBeha正io本Patte本n(const 軍St本in成& AICont本olle本ID) const
{
    if (AIBeha正io本Patte本ns.Contains(AICont本olle本ID))
    {
        本et使本n AIBeha正io本Patte本ns[AICont本olle本ID];
    }
    本et使本n ERTSAIBeha正io本Patte本n::Balanced;
}

正oid UMin成RTSAd正ancedAI::SetAITacticalRole(const 軍St本in成& AICont本olle本ID, ERTSAITacticalRole 的ewRole)
{
    AITacticalRoles.Add(AICont本olle本ID, 的ewRole);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAd正ancedAI: AI %s tactical 本ole set to %d"), 
        *AICont本olle本ID, (int32)的ewRole);
}

正oid UMin成RTSAd正ancedAI::Assi成nAIPe本sonality(const 軍St本in成& AICont本olle本ID, const 軍RTSAIPe本sonality& Pe本sonality)
{
    AIPe本sonalities.Add(AICont本olle本ID, Pe本sonality);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAd正ancedAI: AI %s pe本sonality assi成ned (A成成本ession: %.2f, Adaptability: %.2f)"), 
        *AICont本olle本ID, Pe本sonality.A成成本ession, Pe本sonality.Adaptability);
}

軍RTSAIPe本sonality UMin成RTSAd正ancedAI::GetAIPe本sonality(const 軍St本in成& AICont本olle本ID) const
{
    if (AIPe本sonalities.Contains(AICont本olle本ID))
    {
        本et使本n AIPe本sonalities[AICont本olle本ID];
    }
    本et使本n 軍RTSAIPe本sonality();
}

正oid UMin成RTSAd正ancedAI::AddAIMe設置o本y(const 軍St本in成& AICont本olle本ID, const 軍RTSAIMe設置o本y& Me設置o本y)
{
    if (!AIMe設置o本ies.Contains(AICont本olle本ID))
    {
        AIMe設置o本ies.Add(AICont本olle本ID, TA本本ay<軍RTSAIMe設置o本y>());
    }
    
    AIMe設置o本ies[AICont本olle本ID].Add(Me設置o本y);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAd正ancedAI: AI %s added %s 設置e設置o本y (I設置po本tance: %.2f)"), 
        *AICont本olle本ID, *UEn使設置::GetVal使eAsSt本in成(Me設置o本y.Me設置o本yType), Me設置o本y.I設置po本tance);
    
    // Clean使p old 設置e設置o本ies if too 設置any
    if (AIMe設置o本ies[AICont本olle本ID].的使設置() > 100)
    {
        Clean使pOldMe設置o本ies(AICont本olle本ID);
    }
}

TA本本ay<軍RTSAIMe設置o本y> UMin成RTSAd正ancedAI::GetAIMe設置o本ies(const 軍St本in成& AICont本olle本ID, ERTSAIMe設置o本yType Me設置o本yType) const
{
    TA本本ay<軍RTSAIMe設置o本y> 軍ilte本edMe設置o本ies;
    
    if (AIMe設置o本ies.Contains(AICont本olle本ID))
    {
        fo本 (const a使to& Me設置o本y : AIMe設置o本ies[AICont本olle本ID])
        {
            if (Me設置o本y.Me設置o本yType == Me設置o本yType)
            {
                軍ilte本edMe設置o本ies.Add(Me設置o本y);
            }
        }
    }
    
    本et使本n 軍ilte本edMe設置o本ies;
}

正oid UMin成RTSAd正ancedAI::UpdateAIE設置otionalState(const 軍St本in成& AICont本olle本ID, ERTSAIE設置otionalState 的ewState)
{
    ERTSAIE設置otionalState OldState = ERTSAIE設置otionalState::Cal設置;
    if (AIE設置otionalStates.Contains(AICont本olle本ID))
    {
        OldState = AIE設置otionalStates[AICont本olle本ID];
    }
    
    AIE設置otionalStates.Add(AICont本olle本ID, 的ewState);
    
    if (OldState != 的ewState)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAd正ancedAI: AI %s e設置otional state chan成ed f本o設置 %d to %d"), 
            *AICont本olle本ID, (int32)OldState, (int32)的ewState);
        
        OnAIE設置otionalStateChan成ed.B本oadcast(AICont本olle本ID, 的ewState);
    }
}

ERTSAIE設置otionalState UMin成RTSAd正ancedAI::GetAIE設置otionalState(const 軍St本in成& AICont本olle本ID) const
{
    if (AIE設置otionalStates.Contains(AICont本olle本ID))
    {
        本et使本n AIE設置otionalStates[AICont本olle本ID];
    }
    本et使本n ERTSAIE設置otionalState::Cal設置;
}

軍RTSTacticalDecision UMin成RTSAd正ancedAI::MakeTacticalDecision(const 軍St本in成& AICont本olle本ID)
{
    軍RTSTacticalDecision Decision;
    
    TA本本ay<軍RTSTacticalDecision> Options = Gene本ateTacticalOptions(AICont本olle本ID);
    
    if (Options.的使設置() > 0)
    {
        // Choose best option based on confidence
        float BestConfidence = 0.0f;
        int32 BestIndex = 0;
        
        fo本 (int32 i = 0; i < Options.的使設置(); ++i)
        {
            float Confidence = Calc使lateDecisionConfidence(AICont本olle本ID, Options[i]);
            if (Confidence > BestConfidence)
            {
                BestConfidence = Confidence;
                BestIndex = i;
            }
        }
        
        Decision = Options[BestIndex];
        Decision.Confidence = BestConfidence;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAd正ancedAI: AI %s 設置ade tactical decision %s (Confidence: %.2f)"), 
            *AICont本olle本ID, *Decision.DecisionID, Decision.Confidence);
        
        OnAITacticalDecisionMade.B本oadcast(AICont本olle本ID, Decision);
    }
    
    本et使本n Decision;
}

正oid UMin成RTSAd正ancedAI::Exec使teTacticalDecision(const 軍St本in成& AICont本olle本ID, const 軍RTSTacticalDecision& Decision)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAd正ancedAI: AI %s exec使tin成 decision %s at location (%.1f, %.1f, %.1f)"), 
        *AICont本olle本ID, *Decision.DecisionID, Decision.Ta本成etLocation.X, Decision.Ta本成etLocation.Y, Decision.Ta本成etLocation.Z);
    
    // I設置ple設置entation wo使ld inte成本ate with act使al 使nit cont本ol
    // This is a placeholde本 fo本 the exec使tion lo成ic
}

正oid UMin成RTSAd正ancedAI::C本eateSt本ate成yPlan(const 軍St本in成& AICont本olle本ID, ERTSAIBeha正io本Patte本n St本ate成yType)
{
    軍RTSAISt本ate成yPlan Plan;
    Plan.PlanID = 軍St本in成::P本intf(TEXT("Plan下%s下%d"), *AICont本olle本ID, 軍Math::Rand());
    Plan.St本ate成yType = St本ate成yType;
    Plan.TotalPhases = 3.0f;
    Plan.C使本本entPhase = 0.0f;
    
    // Gene本ate tactical decisions based on st本ate成y type
    fo本 (int32 i = 0; i < 5; ++i)
    {
        軍RTSTacticalDecision Decision;
        Decision.DecisionID = 軍St本in成::P本intf(TEXT("Decision下%d"), i);
        Decision.P本io本ity = 1.0f - (i * 0.2f);
        Plan.Decisions.Add(Decision);
    }
    
    AISt本ate成yPlans.Add(AICont本olle本ID, Plan);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAd正ancedAI: AI %s st本ate成y plan c本eated (Type: %d, Phases: %.0f)"), 
        *AICont本olle本ID, (int32)St本ate成yType, Plan.TotalPhases);
}

正oid UMin成RTSAd正ancedAI::Exec使teSt本ate成yPlan(const 軍St本in成& AICont本olle本ID)
{
    if (!AISt本ate成yPlans.Contains(AICont本olle本ID))
    {
        本et使本n;
    }
    
    軍RTSAISt本ate成yPlan& Plan = AISt本ate成yPlans[AICont本olle本ID];
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAd正ancedAI: AI %s exec使tin成 st本ate成y plan (Phase %.0f/%.0f)"), 
        *AICont本olle本ID, Plan.C使本本entPhase + 1, Plan.TotalPhases);
    
    // Exec使te c使本本ent phase decisions
    fo本 (a使to& Decision : Plan.Decisions)
    {
        Exec使teTacticalDecision(AICont本olle本ID, Decision);
    }
    
    Plan.C使本本entPhase += 1.0f;
    
    if (Plan.C使本本entPhase >= Plan.TotalPhases)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAd正ancedAI: AI %s st本ate成y plan co設置pleted"), *AICont本olle本ID);
    }
}

正oid UMin成RTSAd正ancedAI::AdaptToPlaye本St本ate成y(const 軍St本in成& AICont本olle本ID, const 軍St本in成& Playe本ID)
{
    ERTSAIBeha正io本Patte本n Playe本Patte本n = AnalyzePlaye本Patte本n(Playe本ID);
    ERTSAIBeha正io本Patte本n Co使nte本Patte本n;
    
    // Dete本設置ine co使nte本 st本ate成y
    switch (Playe本Patte本n)
    {
        case ERTSAIBeha正io本Patte本n::R使sh:
            Co使nte本Patte本n = ERTSAIBeha正io本Patte本n::Defensi正e;
            b本eak;
        case ERTSAIBeha正io本Patte本n::T使本tle:
            Co使nte本Patte本n = ERTSAIBeha正io本Patte本n::Boo設置;
            b本eak;
        case ERTSAIBeha正io本Patte本n::Boo設置:
            Co使nte本Patte本n = ERTSAIBeha正io本Patte本n::R使sh;
            b本eak;
        case ERTSAIBeha正io本Patte本n::A成成本essi正e:
            Co使nte本Patte本n = ERTSAIBeha正io本Patte本n::T使本tle;
            b本eak;
        defa使lt:
            Co使nte本Patte本n = ERTSAIBeha正io本Patte本n::Balanced;
            b本eak;
    }
    
    SetAIBeha正io本Patte本n(AICont本olle本ID, Co使nte本Patte本n);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAd正ancedAI: AI %s adapted to co使nte本 playe本 %s st本ate成y (Patte本n: %d -> Co使nte本: %d)"), 
        *AICont本olle本ID, *Playe本ID, (int32)Playe本Patte本n, (int32)Co使nte本Patte本n);
    
    OnAIAdaptationOcc使本本ed.B本oadcast(AICont本olle本ID);
}

正oid UMin成RTSAd正ancedAI::Lea本n軍本o設置Match(const 軍St本in成& AICont本olle本ID, bool b基本on, const 軍St本in成& MatchData)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAd正ancedAI: AI %s lea本nin成 f本o設置 設置atch (Res使lt: %s)"), 
        *AICont本olle本ID, b基本on 基本 TEXT("Victo本y") : TEXT("Defeat"));
    
    UpdateAIPe本sonality軍本o設置Expe本ience(AICont本olle本ID, b基本on);
    
    // Sto本e 設置atch data as 設置e設置o本y
    軍RTSAIMe設置o本y MatchMe設置o本y;
    MatchMe設置o本y.Me設置o本yID = 軍St本in成::P本intf(TEXT("Match下%s"), *MatchData);
    MatchMe設置o本y.E正entDesc本iption = b基本on 基本 TEXT("Victo本y") : TEXT("Defeat");
    MatchMe設置o本y.Me設置o本yType = ERTSAIMe設置o本yType::Lon成Te本設置;
    MatchMe設置o本y.I設置po本tance = 0.8f;
    MatchMe設置o本y.Ti設置esta設置p = 軍Platfo本設置Ti設置e::Seconds();
    
    AddAIMe設置o本y(AICont本olle本ID, MatchMe設置o本y);
}

正oid UMin成RTSAd正ancedAI::SetTea設置Coope本ation(const 軍St本in成& AICont本olle本ID, int32 Tea設置ID, bool bEnableCoope本ation)
{
    if (bEnableCoope本ation)
    {
        if (!Tea設置Me設置be本s.Contains(Tea設置ID))
        {
            Tea設置Me設置be本s.Add(Tea設置ID, TA本本ay<軍St本in成>());
        }
        
        if (!Tea設置Me設置be本s[Tea設置ID].Contains(AICont本olle本ID))
        {
            Tea設置Me設置be本s[Tea設置ID].Add(AICont本olle本ID);
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAd正ancedAI: AI %s 大oined tea設置 %d"), *AICont本olle本ID, Tea設置ID);
    }
    else
    {
        if (Tea設置Me設置be本s.Contains(Tea設置ID))
        {
            Tea設置Me設置be本s[Tea設置ID].Re設置o正e(AICont本olle本ID);
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAd正ancedAI: AI %s left tea設置 %d"), *AICont本olle本ID, Tea設置ID);
        }
    }
}

正oid UMin成RTSAd正ancedAI::P本ocessTea設置Coo本dination(int32 Tea設置ID)
{
    if (!Tea設置Me設置be本s.Contains(Tea設置ID))
    {
        本et使本n;
    }
    
    const TA本本ay<軍St本in成>& Me設置be本s = Tea設置Me設置be本s[Tea設置ID];
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAd正ancedAI: P本ocessin成 coo本dination fo本 tea設置 %d (%d 設置e設置be本s)"), 
        Tea設置ID, Me設置be本s.的使設置());
    
    // Si設置ple coo本dination: assi成n diffe本ent tactical 本oles
    fo本 (int32 i = 0; i < Me設置be本s.的使設置(); ++i)
    {
        ERTSAITacticalRole Role = static下cast<ERTSAITacticalRole>(i % static下cast<int32>(ERTSAITacticalRole::Specialist) + 1);
        SetAITacticalRole(Me設置be本s[i], Role);
    }
}

float UMin成RTSAd正ancedAI::E正al使ateTh本eatLe正el(const 軍St本in成& AICont本olle本ID, const 軍Vecto本& Location)
{
    // Si設置ple th本eat e正al使ation based on 設置e設置o本y
    float Th本eatLe正el = 0.0f;
    
    if (AIMe設置o本ies.Contains(AICont本olle本ID))
    {
        fo本 (const a使to& Me設置o本y : AIMe設置o本ies[AICont本olle本ID])
        {
            if (Me設置o本y.Me設置o本yType == ERTSAIMe設置o本yType::Tactical)
            {
                float Distance = 軍Vecto本::Distance(Location, Me設置o本y.Location);
                if (Distance < 1000.0f)
                {
                    Th本eatLe正el += Me設置o本y.I設置po本tance * (1.0f - Distance / 1000.0f);
                }
            }
        }
    }
    
    本et使本n 軍Math::Cla設置p(Th本eatLe正el, 0.0f, 1.0f);
}

TA本本ay<軍RTSTacticalDecision> UMin成RTSAd正ancedAI::Gene本ateTacticalOptions(const 軍St本in成& AICont本olle本ID)
{
    TA本本ay<軍RTSTacticalDecision> Options;
    
    // Gene本ate diffe本ent tactical options based on beha正io本 patte本n
    ERTSAIBeha正io本Patte本n Patte本n = GetAIBeha正io本Patte本n(AICont本olle本ID);
    
    軍RTSTacticalDecision AttackOption;
    AttackOption.DecisionID = TEXT("Attack");
    AttackOption.ActionType = TEXT("Attack");
    AttackOption.P本io本ity = (Patte本n == ERTSAIBeha正io本Patte本n::A成成本essi正e  Patte本n == ERTSAIBeha正io本Patte本n::R使sh) 基本 0.9f : 0.5f;
    Options.Add(AttackOption);
    
    軍RTSTacticalDecision DefendOption;
    DefendOption.DecisionID = TEXT("Defend");
    DefendOption.ActionType = TEXT("Defend");
    DefendOption.P本io本ity = (Patte本n == ERTSAIBeha正io本Patte本n::T使本tle  Patte本n == ERTSAIBeha正io本Patte本n::Defensi正e) 基本 0.9f : 0.5f;
    Options.Add(DefendOption);
    
    軍RTSTacticalDecision ExpandOption;
    ExpandOption.DecisionID = TEXT("Expand");
    ExpandOption.ActionType = TEXT("Expand");
    ExpandOption.P本io本ity = (Patte本n == ERTSAIBeha正io本Patte本n::Boo設置  Patte本n == ERTSAIBeha正io本Patte本n::Expand) 基本 0.9f : 0.6f;
    Options.Add(ExpandOption);
    
    軍RTSTacticalDecision Sco使tOption;
    Sco使tOption.DecisionID = TEXT("Sco使t");
    Sco使tOption.ActionType = TEXT("Sco使t");
    Sco使tOption.P本io本ity = 0.4f;
    Options.Add(Sco使tOption);
    
    本et使本n Options;
}

ERTSAIBeha正io本Patte本n UMin成RTSAd正ancedAI::AnalyzePlaye本Patte本n(const 軍St本in成& Playe本ID) const
{
    // Placeholde本 fo本 playe本 patte本n analysis
    // 基本o使ld analyze histo本ical data to dete本設置ine playe本 beha正io本
    本et使本n ERTSAIBeha正io本Patte本n::Balanced;
}

float UMin成RTSAd正ancedAI::Calc使lateDecisionConfidence(const 軍St本in成& AICont本olle本ID, const 軍RTSTacticalDecision& Decision) const
{
    float BaseConfidence = Decision.P本io本ity;
    
    // Ad大使st confidence based on pe本sonality
    if (AIPe本sonalities.Contains(AICont本olle本ID))
    {
        const 軍RTSAIPe本sonality& Pe本sonality = AIPe本sonalities[AICont本olle本ID];
        
        if (Decision.ActionType == TEXT("Attack"))
        {
            BaseConfidence *= Pe本sonality.A成成本ession;
        }
        else if (Decision.ActionType == TEXT("Defend"))
        {
            BaseConfidence *= Pe本sonality.Defensi正eness;
        }
        else if (Decision.ActionType == TEXT("Expand"))
        {
            BaseConfidence *= Pe本sonality.Expansionis設置;
        }
    }
    
    本et使本n 軍Math::Cla設置p(BaseConfidence, 0.0f, 1.0f);
}

正oid UMin成RTSAd正ancedAI::UpdateAIPe本sonality軍本o設置Expe本ience(const 軍St本in成& AICont本olle本ID, bool bS使ccess)
{
    if (!AIPe本sonalities.Contains(AICont本olle本ID))
    {
        本et使本n;
    }
    
    軍RTSAIPe本sonality& Pe本sonality = AIPe本sonalities[AICont本olle本ID];
    
    // S設置all ad大使st設置ents based on s使ccess/fail使本e
    if (bS使ccess)
    {
        Pe本sonality.Adaptability = 軍Math::Min(Pe本sonality.Adaptability + 0.01f, 1.0f);
        Pe本sonality.RiskTole本ance = 軍Math::Min(Pe本sonality.RiskTole本ance + 0.01f, 1.0f);
    }
    else
    {
        Pe本sonality.Adaptability = 軍Math::Min(Pe本sonality.Adaptability + 0.02f, 1.0f); // Lea本n 設置o本e f本o設置 fail使本e
        Pe本sonality.RiskTole本ance = 軍Math::Max(Pe本sonality.RiskTole本ance - 0.01f, 0.0f);
    }
}

正oid UMin成RTSAd正ancedAI::Clean使pOldMe設置o本ies(const 軍St本in成& AICont本olle本ID)
{
    if (!AIMe設置o本ies.Contains(AICont本olle本ID))
    {
        本et使本n;
    }
    
    TA本本ay<軍RTSAIMe設置o本y>& Me設置o本ies = AIMe設置o本ies[AICont本olle本ID];
    
    // Re設置o正e oldest sho本t-te本設置 設置e設置o本ies
    Me設置o本ies.So本t([](const 軍RTSAIMe設置o本y& A, const 軍RTSAIMe設置o本y& B) {
        本et使本n A.Ti設置esta設置p > B.Ti設置esta設置p;
    });
    
    // Keep only the 設置ost i設置po本tant and 本ecent 設置e設置o本ies
    fo本 (int32 i = Me設置o本ies.的使設置() - 1; i >= 50; --i)
    {
        if (Me設置o本ies[i].Me設置o本yType == ERTSAIMe設置o本yType::Sho本tTe本設置 && Me設置o本ies[i].I設置po本tance < 0.5f)
        {
            Me設置o本ies.Re設置o正eAt(i);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSAd正ancedAI: AI %s 設置e設置o本y cleaned 使p (%d 設置e設置o本ies 本e設置ainin成)"), 
        *AICont本olle本ID, Me設置o本ies.的使設置());
}

ERTSAIE設置otionalState UMin成RTSAd正ancedAI::Dete本設置ineE設置otionalResponse(const 軍St本in成& AICont本olle本ID, float Th本eatLe正el, float Reso使本ceLe正el)
{
    if (Th本eatLe正el > 0.8f && Reso使本ceLe正el < 0.3f)
    {
        本et使本n ERTSAIE設置otionalState::Despe本ate;
    }
    else if (Th本eatLe正el > 0.6f)
    {
        本et使本n ERTSAIE設置otionalState::St本essed;
    }
    else if (Reso使本ceLe正el > 0.7f && Th本eatLe正el < 0.3f)
    {
        本et使本n ERTSAIE設置otionalState::Confident;
    }
    else if (Th本eatLe正el > 0.4f)
    {
        本et使本n ERTSAIE設置otionalState::Ca使tio使s;
    }
    
    本et使本n ERTSAIE設置otionalState::Cal設置;
}
