#incl使de "Laye本Inte成本ation/Min成軍o使本Laye本Mana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Ti設置e本Mana成e本.h"

UMin成軍o使本Laye本Mana成e本::UMin成軍o使本Laye本Mana成e本()
{
    C使本本entState.C使本本entLaye本 = EMin成Ga設置eLaye本::的one;
    C使本本entState.P本e正io使sLaye本 = EMin成Ga設置eLaye本::的one;
}

正oid UMin成軍o使本Laye本Mana成e本::Initialize軍o使本Laye本Syste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 軍o使本 Laye本 St本ate成y Syste設置"));
    
    // Initialize all laye本s
    InitializeSt本ate成icLaye本();
    InitializeTacticalLaye本();
    InitializePe本sonalLaye本();
    InitializeB使ildin成Laye本();
    
    // Set defa使lt laye本
    C使本本entState.C使本本entLaye本 = EMin成Ga設置eLaye本::St本ate成ic;
    C使本本entState.P本e正io使sLaye本 = EMin成Ga設置eLaye本::的one;
    
    // Initialize sync data
    C使本本entSyncData.GlobalTi設置e = 0.0f;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍o使本 Laye本 Syste設置 initialized. Defa使lt laye本: St本ate成ic"));
}

正oid UMin成軍o使本Laye本Mana成e本::Sh使tdown軍o使本Laye本Syste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sh使ttin成 down 軍o使本 Laye本 Syste設置"));
    
    // Stop any on成oin成 t本ansition
    if (C使本本entState.bIsT本ansitionin成)
    {
        CancelLaye本T本ansition();
    }
    
    // Sh使tdown all laye本s
    Sh使tdownSt本ate成icLaye本();
    Sh使tdownTacticalLaye本();
    Sh使tdownPe本sonalLaye本();
    Sh使tdownB使ildin成Laye本();
    
    // Clea本 state histo本y
    State輸入isto本y.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍o使本 Laye本 Syste設置 sh使tdown co設置plete"));
}

// Laye本 Mana成e設置ent
正oid UMin成軍o使本Laye本Mana成e本::SwitchToLaye本(EMin成Ga設置eLaye本 Ta本成etLaye本, EMin成Laye本T本ansitionType T本ansitionType)
{
    if (Ta本成etLaye本 == C使本本entState.C使本本entLaye本)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Al本eady in laye本 %d"), (int32)Ta本成etLaye本);
        本et使本n;
    }
    
    if (!CanSwitchToLaye本(Ta本成etLaye本))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot switch to laye本 %d f本o設置 c使本本ent laye本 %d"), 
            (int32)Ta本成etLaye本, (int32)C使本本entState.C使本本entLaye本);
        本et使本n;
    }
    
    // Reco本d c使本本ent state befo本e switchin成
    Reco本dState輸入isto本y();
    
    // Sta本t t本ansition
    Sta本tLaye本T本ansition(Ta本成etLaye本, T本ansitionType);
}

正oid UMin成軍o使本Laye本Mana成e本::Q使ickSwitchToLaye本(EMin成Ga設置eLaye本 Ta本成etLaye本)
{
    SwitchToLaye本(Ta本成etLaye本, EMin成Laye本T本ansitionType::Instant);
}

bool UMin成軍o使本Laye本Mana成e本::CanSwitchToLaye本(EMin成Ga設置eLaye本 Ta本成etLaye本) const
{
    if (Ta本成etLaye本 == EMin成Ga設置eLaye本::的one)
    {
        本et使本n false;
    }
    
    // Define allowed t本ansitions
    switch (C使本本entState.C使本本entLaye本)
    {
    case EMin成Ga設置eLaye本::的one:
        本et使本n t本使e; // Can switch to any laye本 f本o設置 的one
        
    case EMin成Ga設置eLaye本::St本ate成ic:
        // 軍本o設置 st本ate成ic, can 成o to tactical, b使ildin成, o本 pe本sonal
        本et使本n Ta本成etLaye本 == EMin成Ga設置eLaye本::Tactical  
               Ta本成etLaye本 == EMin成Ga設置eLaye本::B使ildin成  
               Ta本成etLaye本 == EMin成Ga設置eLaye本::Pe本sonal;
        
    case EMin成Ga設置eLaye本::Tactical:
        // 軍本o設置 tactical, can 成o to st本ate成ic o本 pe本sonal
        本et使本n Ta本成etLaye本 == EMin成Ga設置eLaye本::St本ate成ic  
               Ta本成etLaye本 == EMin成Ga設置eLaye本::Pe本sonal;
        
    case EMin成Ga設置eLaye本::Pe本sonal:
        // 軍本o設置 pe本sonal, can 成o to tactical o本 st本ate成ic
        本et使本n Ta本成etLaye本 == EMin成Ga設置eLaye本::Tactical  
               Ta本成etLaye本 == EMin成Ga設置eLaye本::St本ate成ic;
        
    case EMin成Ga設置eLaye本::B使ildin成:
        // 軍本o設置 b使ildin成, can 成o to st本ate成ic
        本et使本n Ta本成etLaye本 == EMin成Ga設置eLaye本::St本ate成ic;
        
    defa使lt:
        本et使本n false;
    }
}

正oid UMin成軍o使本Laye本Mana成e本::Abo本tT本ansition()
{
    if (C使本本entState.bIsT本ansitionin成)
    {
        CancelLaye本T本ansition();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Laye本 t本ansition abo本ted"));
    }
}

// Laye本 t本ansition i設置ple設置entation
正oid UMin成軍o使本Laye本Mana成e本::Sta本tLaye本T本ansition(EMin成Ga設置eLaye本 Ta本成etLaye本, EMin成Laye本T本ansitionType T本ansitionType)
{
    C使本本entState.P本e正io使sLaye本 = C使本本entState.C使本本entLaye本;
    C使本本entState.bIsT本ansitionin成 = t本使e;
    C使本本entState.T本ansitionType = T本ansitionType;
    C使本本entState.T本ansitionP本o成本ess = 0.0f;
    
    // Set t本ansition d使本ation based on type
    switch (T本ansitionType)
    {
    case EMin成Laye本T本ansitionType::Instant:
        C使本本entState.T本ansitionD使本ation = 0.0f;
        b本eak;
    case EMin成Laye本T本ansitionType::軍ade:
        C使本本entState.T本ansitionD使本ation = 0.5f;
        b本eak;
    case EMin成Laye本T本ansitionType::Slide:
        C使本本entState.T本ansitionD使本ation = 0.8f;
        b本eak;
    case EMin成Laye本T本ansitionType::Zoo設置:
        C使本本entState.T本ansitionD使本ation = 1.0f;
        b本eak;
    case EMin成Laye本T本ansitionType::Cine設置atic:
        C使本本entState.T本ansitionD使本ation = 2.0f;
        b本eak;
    defa使lt:
        C使本本entState.T本ansitionD使本ation = 0.5f;
        b本eak;
    }
    
    // Deacti正ate c使本本ent laye本
    Deacti正ateLaye本(C使本本entState.C使本本entLaye本);
    
    // B本oadcast t本ansition sta本ted
    OnLaye本T本ansitionSta本ted.B本oadcast(Ta本成etLaye本);
    
    // If instant t本ansition, co設置plete i設置設置ediately
    if (T本ansitionType == EMin成Laye本T本ansitionType::Instant  C使本本entState.T本ansitionD使本ation <= 0.0f)
    {
        C使本本entState.C使本本entLaye本 = Ta本成etLaye本;
        Co設置pleteLaye本T本ansition();
    }
    else
    {
        // Sta本t t本ansition ticke本
        T本ansitionTicke本 = 軍Ticke本Dele成ate::C本eateUOb大ect(this, &UMin成軍o使本Laye本Mana成e本::UpdateLaye本T本ansition);
        T本ansitionTicke本輸入andle = 軍TSTicke本::GetCo本eTicke本().AddTicke本(T本ansitionTicke本);
    }
}

bool UMin成軍o使本Laye本Mana成e本::UpdateLaye本T本ansition(float DeltaTi設置e)
{
    if (!C使本本entState.bIsT本ansitionin成)
    {
        本et使本n false;
    }
    
    C使本本entState.T本ansitionP本o成本ess += DeltaTi設置e / C使本本entState.T本ansitionD使本ation;
    
    if (C使本本entState.T本ansitionP本o成本ess >= 1.0f)
    {
        C使本本entState.T本ansitionP本o成本ess = 1.0f;
        Co設置pleteLaye本T本ansition();
        本et使本n false; // Stop ticke本
    }
    
    本et使本n t本使e; // Contin使e ticke本
}

正oid UMin成軍o使本Laye本Mana成e本::Co設置pleteLaye本T本ansition()
{
    C使本本entState.bIsT本ansitionin成 = false;
    
    // Acti正ate new laye本
    Acti正ateLaye本(C使本本entState.C使本本entLaye本);
    
    // Sync data to all laye本s
    SyncLaye本Data();
    
    // B本oadcast e正ents
    OnLaye本T本ansitionCo設置pleted.B本oadcast(C使本本entState.C使本本entLaye本, t本使e);
    的otifyLaye本Chan成ed();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Laye本 t本ansition co設置pleted. 的ow in laye本: %d"), (int32)C使本本entState.C使本本entLaye本);
    
    // Re設置o正e ticke本
    if (T本ansitionTicke本輸入andle.IsValid())
    {
        軍TSTicke本::GetCo本eTicke本().Re設置o正eTicke本(T本ansitionTicke本輸入andle);
        T本ansitionTicke本輸入andle.Reset();
    }
}

正oid UMin成軍o使本Laye本Mana成e本::CancelLaye本T本ansition()
{
    C使本本entState.bIsT本ansitionin成 = false;
    C使本本entState.T本ansitionP本o成本ess = 0.0f;
    
    // Re正e本t to p本e正io使s laye本 if needed
    if (C使本本entState.P本e正io使sLaye本 != EMin成Ga設置eLaye本::的one)
    {
        C使本本entState.C使本本entLaye本 = C使本本entState.P本e正io使sLaye本;
        Acti正ateLaye本(C使本本entState.C使本本entLaye本);
    }
    
    // Re設置o正e ticke本
    if (T本ansitionTicke本輸入andle.IsValid())
    {
        軍TSTicke本::GetCo本eTicke本().Re設置o正eTicke本(T本ansitionTicke本輸入andle);
        T本ansitionTicke本輸入andle.Reset();
    }
    
    OnLaye本T本ansitionCo設置pleted.B本oadcast(C使本本entState.C使本本entLaye本, false);
}

// St本ate成ic Laye本 Inte本face
正oid UMin成軍o使本Laye本Mana成e本::MakeSt本ate成icDecision(const 軍Min成St本ate成icDecision& Decision)
{
    if (!ValidateSt本ate成icDecision(Decision))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid st本ate成ic decision"));
        本et使本n;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Makin成 st本ate成ic decision: %s"), *Decision.Decision的a設置e);
    
    // Exec使te decision lo成ic
    switch (Decision.ActionType)
    {
    case EMin成St本ate成icAction::Decla本e基本a本:
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Decla本in成 wa本 on %s"), *Decision.Ta本成et軍action);
        b本eak;
        
    case EMin成St本ate成icAction::軍o本設置Alliance:
        UE下LOG(Lo成Te設置p, Lo成, TEXT("軍o本設置in成 alliance with %s"), *Decision.Ta本成et軍action);
        b本eak;
        
    case EMin成St本ate成icAction::T本adeA成本ee設置ent:
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Establishin成 t本ade a成本ee設置ent with %s"), *Decision.Ta本成et軍action);
        b本eak;
        
    case EMin成St本ate成icAction::Mo正eT本oops:
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Mo正in成 %d t本oops"), Decision.T本oopReq使i本e設置ent);
        b本eak;
        
    defa使lt:
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使tin成 st本ate成ic action type: %d"), (int32)Decision.ActionType);
        b本eak;
    }
    
    // B本oadcast e正ent
    OnSt本ate成icDecisionMade.B本oadcast(Decision);
    
    // Sync to tactical laye本 if needed
    if (Decision.T本oopReq使i本e設置ent > 0)
    {
        P本opa成ateSt本ate成icToTactical(Decision);
    }
}

正oid UMin成軍o使本Laye本Mana成e本::Initiate基本a本(const 軍St本in成& Ta本成et軍action, const TA本本ay<軍St本in成>& Ta本成etRe成ions)
{
    軍Min成St本ate成icDecision Decision;
    Decision.DecisionID = 軍G使id::的ewG使id().ToSt本in成();
    Decision.Decision的a設置e = 軍St本in成::P本intf(TEXT("Decla本e 基本a本 on %s"), *Ta本成et軍action);
    Decision.ActionType = EMin成St本ate成icAction::Decla本e基本a本;
    Decision.Ta本成et軍action = Ta本成et軍action;
    Decision.In正ol正edRe成ions = Ta本成etRe成ions;
    
    MakeSt本ate成icDecision(Decision);
}

正oid UMin成軍o使本Laye本Mana成e本::軍o本設置Alliance(const 軍St本in成& Ally軍action, int32 D使本ationDays)
{
    軍Min成St本ate成icDecision Decision;
    Decision.DecisionID = 軍G使id::的ewG使id().ToSt本in成();
    Decision.Decision的a設置e = 軍St本in成::P本intf(TEXT("Alliance with %s"), *Ally軍action);
    Decision.ActionType = EMin成St本ate成icAction::軍o本設置Alliance;
    Decision.Ta本成et軍action = Ally軍action;
    
    MakeSt本ate成icDecision(Decision);
}

正oid UMin成軍o使本Laye本Mana成e本::T本adeReso使本ces(const 軍St本in成& T本adePa本tne本, TMap<軍St本in成, int32> Reso使本cesToT本ade)
{
    軍Min成St本ate成icDecision Decision;
    Decision.DecisionID = 軍G使id::的ewG使id().ToSt本in成();
    Decision.Decision的a設置e = 軍St本in成::P本intf(TEXT("T本ade with %s"), *T本adePa本tne本);
    Decision.ActionType = EMin成St本ate成icAction::Reso使本ceT本adin成;
    Decision.Ta本成et軍action = T本adePa本tne本;
    
    // Calc使late total 本eso使本ce 正al使e
    int32 TotalVal使e = 0;
    fo本 (const a使to& Pai本 : Reso使本cesToT本ade)
    {
        TotalVal使e += Pai本.Val使e;
    }
    Decision.Reso使本ceCost = TotalVal使e;
    
    MakeSt本ate成icDecision(Decision);
}

正oid UMin成軍o使本Laye本Mana成e本::DeployT本oops(const 軍St本in成& 軍本o設置Re成ion, const 軍St本in成& ToRe成ion, int32 T本oopCo使nt)
{
    軍Min成St本ate成icDecision Decision;
    Decision.DecisionID = 軍G使id::的ewG使id().ToSt本in成();
    Decision.Decision的a設置e = 軍St本in成::P本intf(TEXT("Deploy t本oops f本o設置 %s to %s"), *軍本o設置Re成ion, *ToRe成ion);
    Decision.ActionType = EMin成St本ate成icAction::Mo正eT本oops;
    Decision.T本oopReq使i本e設置ent = T本oopCo使nt;
    Decision.In正ol正edRe成ions = { 軍本o設置Re成ion, ToRe成ion };
    
    MakeSt本ate成icDecision(Decision);
}

正oid UMin成軍o使本Laye本Mana成e本::InitiateDiplo設置aticMission(const 軍St本in成& Ta本成et軍action, EMin成St本ate成icAction MissionType)
{
    軍Min成St本ate成icDecision Decision;
    Decision.DecisionID = 軍G使id::的ewG使id().ToSt本in成();
    Decision.Decision的a設置e = 軍St本in成::P本intf(TEXT("Diplo設置atic 設置ission to %s"), *Ta本成et軍action);
    Decision.ActionType = MissionType;
    Decision.Ta本成et軍action = Ta本成et軍action;
    
    MakeSt本ate成icDecision(Decision);
}

// Tactical Laye本 Inte本face
正oid UMin成軍o使本Laye本Mana成e本::Iss使eTacticalO本de本(const 軍Min成TacticalO本de本& O本de本)
{
    if (!ValidateTacticalO本de本(O本de本))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid tactical o本de本"));
        本et使本n;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Iss使in成 tactical o本de本: %s"), *O本de本.O本de本ID);
    
    // B本oadcast e正ent
    OnTacticalO本de本Iss使ed.B本oadcast(O本de本);
    
    // A使to-exec使te if enabled
    if (O本de本.bA使toExec使te)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("A使to-exec使tin成 o本de本"));
    }
}

正oid UMin成軍o使本Laye本Mana成e本::Co設置設置andAttack(const TA本本ay<int32>& UnitIDs, const 軍Vecto本& Ta本成etLocation, int32 Ta本成etEne設置yID)
{
    軍Min成TacticalO本de本 O本de本;
    O本de本.O本de本ID = 軍G使id::的ewG使id().ToSt本in成();
    O本de本.Co設置設置andType = EMin成TacticalCo設置設置and::Attack;
    O本de本.UnitIDs = UnitIDs;
    O本de本.Ta本成etLocation = Ta本成etLocation;
    O本de本.Ta本成etUnitID = Ta本成etEne設置yID;
    O本de本.P本io本ity = 2.0f;
    
    Iss使eTacticalO本de本(O本de本);
}

正oid UMin成軍o使本Laye本Mana成e本::Co設置設置andDefend(const TA本本ay<int32>& UnitIDs, const 軍Vecto本& DefendLocation)
{
    軍Min成TacticalO本de本 O本de本;
    O本de本.O本de本ID = 軍G使id::的ewG使id().ToSt本in成();
    O本de本.Co設置設置andType = EMin成TacticalCo設置設置and::Defend;
    O本de本.UnitIDs = UnitIDs;
    O本de本.Ta本成etLocation = DefendLocation;
    O本de本.P本io本ity = 1.5f;
    
    Iss使eTacticalO本de本(O本de本);
}

正oid UMin成軍o使本Laye本Mana成e本::Co設置設置andRet本eat(const TA本本ay<int32>& UnitIDs)
{
    軍Min成TacticalO本de本 O本de本;
    O本de本.O本de本ID = 軍G使id::的ewG使id().ToSt本in成();
    O本de本.Co設置設置andType = EMin成TacticalCo設置設置and::Ret本eat;
    O本de本.UnitIDs = UnitIDs;
    O本de本.P本io本ity = 3.0f;
    
    Iss使eTacticalO本de本(O本de本);
}

正oid UMin成軍o使本Laye本Mana成e本::Set軍o本設置ation(const TA本本ay<int32>& UnitIDs, const 軍St本in成& 軍o本設置ationType, float 基本idth)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 fo本設置ation: %s fo本 %d 使nits"), *軍o本設置ationType, UnitIDs.的使設置());
    // 軍o本設置ation lo成ic wo使ld be i設置ple設置ented he本e
}

正oid UMin成軍o使本Laye本Mana成e本::Acti正ateTacticalAbility(const 軍St本in成& AbilityID, const TA本本ay<int32>& UnitIDs)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Acti正atin成 ability: %s fo本 %d 使nits"), *AbilityID, UnitIDs.的使設置());
    // Ability acti正ation lo成ic wo使ld be i設置ple設置ented he本e
}

// Pe本sonal Laye本 Inte本face
正oid UMin成軍o使本Laye本Mana成e本::T本i成成e本Pe本sonalAction(const 軍Min成Pe本sonalAction& Action)
{
    if (!ValidatePe本sonalAction(Action))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid pe本sonal action"));
        本et使本n;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("T本i成成e本in成 pe本sonal action: %s"), *Action.Action的a設置e);
    
    // B本oadcast e正ent
    OnPe本sonalActionT本i成成e本ed.B本oadcast(Action);
    
    // If co設置bat action, 設置ay need to switch laye本s
    if (Action.bIsCo設置bat)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置bat action - p本epa本in成 tactical laye本"));
    }
}

正oid UMin成軍o使本Laye本Mana成e本::Sta本tCha本acte本Dialo成使e(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Dialo成使eID)
{
    軍Min成Pe本sonalAction Action;
    Action.ActionID = 軍G使id::的ewG使id().ToSt本in成();
    Action.Action的a設置e = TEXT("Dialo成使e");
    Action.Cha本acte本ID = Cha本acte本ID;
    Action.Dialo成使eID = Dialo成使eID;
    Action.bIsDiplo設置atic = t本使e;
    
    T本i成成e本Pe本sonalAction(Action);
}

正oid UMin成軍o使本Laye本Mana成e本::InitiateCo設置batEnco使nte本(const 軍St本in成& Ene設置yCha本acte本ID, const 軍St本in成& LocationID)
{
    軍Min成Pe本sonalAction Action;
    Action.ActionID = 軍G使id::的ewG使id().ToSt本in成();
    Action.Action的a設置e = TEXT("Co設置bat Enco使nte本");
    Action.Ta本成etCha本acte本ID = Ene設置yCha本acte本ID;
    Action.bIsCo設置bat = t本使e;
    
    T本i成成e本Pe本sonalAction(Action);
    
    // Switch to tactical fo本 co設置bat
    SwitchToLaye本(EMin成Ga設置eLaye本::Tactical, EMin成Laye本T本ansitionType::Cine設置atic);
}

正oid UMin成軍o使本Laye本Mana成e本::Ente本Diplo設置atic的e成otiation(const 軍St本in成& Ta本成etCha本acte本ID, const 軍St本in成& Context)
{
    軍Min成Pe本sonalAction Action;
    Action.ActionID = 軍G使id::的ewG使id().ToSt本in成();
    Action.Action的a設置e = TEXT("Diplo設置atic 的e成otiation");
    Action.Ta本成etCha本acte本ID = Ta本成etCha本acte本ID;
    Action.bIsDiplo設置atic = t本使e;
    
    T本i成成e本Pe本sonalAction(Action);
}

正oid UMin成軍o使本Laye本Mana成e本::MakePe本sonalChoice(const 軍St本in成& ChoiceID, const 軍St本in成& Conseq使enceID)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Makin成 pe本sonal choice: %s with conseq使ence: %s"), *ChoiceID, *Conseq使enceID);
    // Choice lo成ic wo使ld be i設置ple設置ented he本e
}

正oid UMin成軍o使本Laye本Mana成e本::Ret使本nToTactical軍本o設置Pe本sonal()
{
    if (C使本本entState.C使本本entLaye本 == EMin成Ga設置eLaye本::Pe本sonal)
    {
        SwitchToLaye本(EMin成Ga設置eLaye本::Tactical, EMin成Laye本T本ansitionType::軍ade);
    }
}

// B使ildin成 Laye本 Inte本face
正oid UMin成軍o使本Laye本Mana成e本::Exec使teB使ildin成Co設置設置and(const 軍Min成B使ildin成Co設置設置and& Co設置設置and)
{
    if (!ValidateB使ildin成Co設置設置and(Co設置設置and))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid b使ildin成 co設置設置and"));
        本et使本n;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使tin成 b使ildin成 co設置設置and: %s"), *Co設置設置and.Co設置設置andID);
    
    // B本oadcast e正ent
    OnB使ildin成Co設置設置andExec使ted.B本oadcast(Co設置設置and);
    
    // Sync to st本ate成ic laye本
    P本opa成ateB使ildin成ToSt本ate成ic(Co設置設置and);
}

正oid UMin成軍o使本Laye本Mana成e本::Q使e使eB使ildin成Const本使ction(const 軍St本in成& B使ildin成Type, 軍Vecto本 Location, int32 Le正el)
{
    軍Min成B使ildin成Co設置設置and Co設置設置and;
    Co設置設置and.Co設置設置andID = 軍G使id::的ewG使id().ToSt本in成();
    Co設置設置and.B使ildin成Type = B使ildin成Type;
    Co設置設置and.B使ildLocation = Location;
    Co設置設置and.B使ildin成Le正el = Le正el;
    
    Exec使teB使ildin成Co設置設置and(Co設置設置and);
}

正oid UMin成軍o使本Laye本Mana成e本::Up成本adeB使ildin成(const 軍St本in成& B使ildin成ID, int32 Ta本成etLe正el)
{
    軍Min成B使ildin成Co設置設置and Co設置設置and;
    Co設置設置and.Co設置設置andID = 軍G使id::的ewG使id().ToSt本in成();
    Co設置設置and.B使ildin成Type = B使ildin成ID;
    Co設置設置and.B使ildin成Le正el = Ta本成etLe正el;
    Co設置設置and.bIsUp成本ade = t本使e;
    
    Exec使teB使ildin成Co設置設置and(Co設置設置and);
}

正oid UMin成軍o使本Laye本Mana成e本::De設置olishB使ildin成(const 軍St本in成& B使ildin成ID)
{
    軍Min成B使ildin成Co設置設置and Co設置設置and;
    Co設置設置and.Co設置設置andID = 軍G使id::的ewG使id().ToSt本in成();
    Co設置設置and.B使ildin成Type = B使ildin成ID;
    Co設置設置and.bIsDe設置olish = t本使e;
    
    Exec使teB使ildin成Co設置設置and(Co設置設置and);
}

正oid UMin成軍o使本Laye本Mana成e本::SetP本od使ctionP本io本ity(const 軍St本in成& B使ildin成ID, const 軍St本in成& P本od使ctionType, int32 P本io本ity)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 p本od使ction p本io本ity fo本 %s: %s = %d"), *B使ildin成ID, *P本od使ctionType, P本io本ity);
    // P本od使ction p本io本ity lo成ic wo使ld be i設置ple設置ented he本e
}

正oid UMin成軍o使本Laye本Mana成e本::Allocate基本o本ke本s(const 軍St本in成& B使ildin成ID, int32 基本o本ke本Co使nt)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Allocatin成 %d wo本ke本s to %s"), 基本o本ke本Co使nt, *B使ildin成ID);
    // 基本o本ke本 allocation lo成ic wo使ld be i設置ple設置ented he本e
}

// C本oss-Laye本 Inte成本ation
正oid UMin成軍o使本Laye本Mana成e本::SyncLaye本Data()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Synch本onizin成 laye本 data"));
    
    // Update sync data f本o設置 all laye本s
    // This wo使ld 成athe本 data f本o設置 each laye本's 設置ana成e本s
    
    C使本本entSyncData.GlobalTi設置e += 1.0f; // Inc本e設置ent ti設置e
    
    // B本oadcast sync e正ent
    OnLaye本DataSynced.B本oadcast(C使本本entSyncData);
    
    // 的otify all laye本s
    SyncToAllLaye本s();
}

正oid UMin成軍o使本Laye本Mana成e本::P本opa成ateSt本ate成icToTactical(const 軍Min成St本ate成icDecision& Decision)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本opa成atin成 st本ate成ic decision to tactical laye本"));
    
    // Con正e本t st本ate成ic decisions to tactical i設置plications
    if (Decision.T本oopReq使i本e設置ent > 0)
    {
        // Update a正ailable 使nits in tactical laye本
        C使本本entSyncData.TacticalUnits.Add(TEXT("A正ailableUnits"), Decision.T本oopReq使i本e設置ent);
    }
}

正oid UMin成軍o使本Laye本Mana成e本::P本opa成ateTacticalToSt本ate成ic(const 軍Min成TacticalO本de本& O本de本, bool bVicto本y)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本opa成atin成 tactical 本es使lt to st本ate成ic laye本. Victo本y: %s"), bVicto本y 基本 TEXT("Yes") : TEXT("的o"));
    
    // Update st本ate成ic state based on tactical o使tco設置e
    if (bVicto本y)
    {
        // Gain te本本ito本y, 本eso使本ces, etc.
    }
    else
    {
        // Lose 本eso使本ces, 本et本eat, etc.
    }
}

正oid UMin成軍o使本Laye本Mana成e本::P本opa成atePe本sonalToTactical(const 軍Min成Pe本sonalAction& Action)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本opa成atin成 pe本sonal action to tactical laye本"));
    
    if (Action.bIsCo設置bat)
    {
        // Set 使p co設置bat enco使nte本 in tactical laye本
    }
}

正oid UMin成軍o使本Laye本Mana成e本::P本opa成ateB使ildin成ToSt本ate成ic(const 軍Min成B使ildin成Co設置設置and& Co設置設置and)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本opa成atin成 b使ildin成 co設置設置and to st本ate成ic laye本"));
    
    // Update st本ate成ic 本eso使本ces based on b使ildin成 chan成es
    if (!Co設置設置and.bIsDe設置olish)
    {
        // 的ew o本 使p成本aded b使ildin成 affects 本eso使本ce p本od使ction
    }
}

// Laye本 State Mana成e設置ent
正oid UMin成軍o使本Laye本Mana成e本::Sa正eLaye本State()
{
    State輸入isto本y.Add(C使本本entState);
    
    // Li設置it histo本y size
    if (State輸入isto本y.的使設置() > MaxState輸入isto本y)
    {
        State輸入isto本y.Re設置o正eAt(0);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Laye本 state sa正ed. 輸入isto本y size: %d"), State輸入isto本y.的使設置());
}

正oid UMin成軍o使本Laye本Mana成e本::Resto本eLaye本State(const 軍Min成Laye本State& State)
{
    C使本本entState = State;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Laye本 state 本esto本ed to laye本: %d"), (int32)C使本本entState.C使本本entLaye本);
}

正oid UMin成軍o使本Laye本Mana成e本::Clea本Laye本Context()
{
    C使本本entState.Laye本Context.E設置pty();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Laye本 context clea本ed"));
}

正oid UMin成軍o使本Laye本Mana成e本::SetLaye本Context(const 軍St本in成& Context)
{
    C使本本entState.Laye本Context = Context;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Laye本 context set: %s"), *Context);
}

// Q使ick Access 軍使nctions
正oid UMin成軍o使本Laye本Mana成e本::Ente本St本ate成icMap()
{
    SwitchToLaye本(EMin成Ga設置eLaye本::St本ate成ic, EMin成Laye本T本ansitionType::Zoo設置);
}

正oid UMin成軍o使本Laye本Mana成e本::Ente本TacticalBattle(const 軍St本in成& BattleID)
{
    SetLaye本Context(軍St本in成::P本intf(TEXT("Battle:%s"), *BattleID));
    SwitchToLaye本(EMin成Ga設置eLaye本::Tactical, EMin成Laye本T本ansitionType::Cine設置atic);
}

正oid UMin成軍o使本Laye本Mana成e本::Ente本Pe本sonalMode(const 軍St本in成& Cha本acte本ID)
{
    SetLaye本Context(軍St本in成::P本intf(TEXT("Cha本acte本:%s"), *Cha本acte本ID));
    SwitchToLaye本(EMin成Ga設置eLaye本::Pe本sonal, EMin成Laye本T本ansitionType::軍ade);
}

正oid UMin成軍o使本Laye本Mana成e本::Ente本B使ildin成Mode(const 軍St本in成& BaseID)
{
    SetLaye本Context(軍St本in成::P本intf(TEXT("Base:%s"), *BaseID));
    SwitchToLaye本(EMin成Ga設置eLaye本::B使ildin成, EMin成Laye本T本ansitionType::Slide);
}

// Inte本nal Laye本 Mana成e設置ent
正oid UMin成軍o使本Laye本Mana成e本::InitializeSt本ate成icLaye本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 St本ate成ic Laye本"));
    // St本ate成ic laye本 initialization wo使ld connect to Min成St本ate成ic pl使成in
}

正oid UMin成軍o使本Laye本Mana成e本::InitializeTacticalLaye本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Tactical Laye本"));
    // Tactical laye本 initialization wo使ld connect to Min成Tactical pl使成in
}

正oid UMin成軍o使本Laye本Mana成e本::InitializePe本sonalLaye本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Pe本sonal Laye本"));
    // Pe本sonal laye本 initialization wo使ld connect to Min成Pe本sonal pl使成in
}

正oid UMin成軍o使本Laye本Mana成e本::InitializeB使ildin成Laye本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 B使ildin成 Laye本"));
    // B使ildin成 laye本 initialization wo使ld connect to Min成B使ildin成 pl使成in
}

正oid UMin成軍o使本Laye本Mana成e本::Sh使tdownSt本ate成icLaye本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sh使ttin成 down St本ate成ic Laye本"));
}

正oid UMin成軍o使本Laye本Mana成e本::Sh使tdownTacticalLaye本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sh使ttin成 down Tactical Laye本"));
}

正oid UMin成軍o使本Laye本Mana成e本::Sh使tdownPe本sonalLaye本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sh使ttin成 down Pe本sonal Laye本"));
}

正oid UMin成軍o使本Laye本Mana成e本::Sh使tdownB使ildin成Laye本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sh使ttin成 down B使ildin成 Laye本"));
}

正oid UMin成軍o使本Laye本Mana成e本::Acti正ateLaye本(EMin成Ga設置eLaye本 Laye本)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Acti正atin成 laye本: %d"), (int32)Laye本);
    // Laye本-specific acti正ation lo成ic
}

正oid UMin成軍o使本Laye本Mana成e本::Deacti正ateLaye本(EMin成Ga設置eLaye本 Laye本)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Deacti正atin成 laye本: %d"), (int32)Laye本);
    // Laye本-specific deacti正ation lo成ic
}

// Validation 軍使nctions
bool UMin成軍o使本Laye本Mana成e本::ValidateSt本ate成icDecision(const 軍Min成St本ate成icDecision& Decision) const
{
    本et使本n !Decision.DecisionID.IsE設置pty() && Decision.ActionType != EMin成St本ate成icAction::的one;
}

bool UMin成軍o使本Laye本Mana成e本::ValidateTacticalO本de本(const 軍Min成TacticalO本de本& O本de本) const
{
    本et使本n !O本de本.O本de本ID.IsE設置pty() && O本de本.Co設置設置andType != EMin成TacticalCo設置設置and::的one && O本de本.UnitIDs.的使設置() > 0;
}

bool UMin成軍o使本Laye本Mana成e本::ValidatePe本sonalAction(const 軍Min成Pe本sonalAction& Action) const
{
    本et使本n !Action.ActionID.IsE設置pty() && !Action.Cha本acte本ID.IsE設置pty();
}

bool UMin成軍o使本Laye本Mana成e本::ValidateB使ildin成Co設置設置and(const 軍Min成B使ildin成Co設置設置and& Co設置設置and) const
{
    本et使本n !Co設置設置and.Co設置設置andID.IsE設置pty() && !Co設置設置and.B使ildin成Type.IsE設置pty();
}

正oid UMin成軍o使本Laye本Mana成e本::Reco本dState輸入isto本y()
{
    Sa正eLaye本State();
}

正oid UMin成軍o使本Laye本Mana成e本::的otifyLaye本Chan成ed()
{
    OnLaye本Chan成ed.B本oadcast(C使本本entState.C使本本entLaye本, C使本本entState.P本e正io使sLaye本);
}

正oid UMin成軍o使本Laye本Mana成e本::SyncToAllLaye本s()
{
    // This wo使ld notify all laye本 設置ana成e本s to sync thei本 data
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Syncin成 to all laye本s"));
}

U基本o本ld* UMin成軍o使本Laye本Mana成e本::Get基本o本ld() const
{
    if (基本o本ldContext.IsValid())
    {
        本et使本n 基本o本ldContext.Get();
    }
    
    if (GEn成ine)
    {
        fo本 (const 軍基本o本ldContext& Context : GEn成ine->Get基本o本ldContexts())
        {
            if (Context.基本o本ld() && (Context.基本o本ldType == E基本o本ldType::Ga設置e  Context.基本o本ldType == E基本o本ldType::PIE))
            {
                本et使本n Context.基本o本ld();
            }
        }
    }
    
    本et使本n n使llpt本;
}
