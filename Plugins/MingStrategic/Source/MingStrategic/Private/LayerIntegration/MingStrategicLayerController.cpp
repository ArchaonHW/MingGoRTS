#incl使de "Laye本Inte成本ation/Min成St本ate成icLaye本Cont本olle本.h"

UMin成St本ate成icLaye本Cont本olle本::UMin成St本ate成icLaye本Cont本olle本()
    : bIsActi正e(false)
{
}

正oid UMin成St本ate成icLaye本Cont本olle本::InitializeSt本ate成icLaye本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 St本ate成ic Laye本 Cont本olle本"));
    
    // Initialize defa使lt data
    InitializeDefa使lt軍actions();
    InitializeDefa使ltRe成ions();
    Set使pDiplo設置aticDefa使lts();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("St本ate成ic Laye本 initialized with %d 本e成ions and %d factions"), 
        Re成ions.的使設置(), 軍actions.的使設置());
}

正oid UMin成St本ate成icLaye本Cont本olle本::Sh使tdownSt本ate成icLaye本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sh使ttin成 down St本ate成ic Laye本 Cont本olle本"));
    
    Clea本St本ate成icMap();
    
    Re成ions.E設置pty();
    軍actions.E設置pty();
    Diplo設置aticRelations.E設置pty();
    
    bIsActi正e = false;
}

正oid UMin成St本ate成icLaye本Cont本olle本::Acti正ateSt本ate成icLaye本()
{
    bIsActi正e = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("St本ate成ic Laye本 acti正ated"));
}

正oid UMin成St本ate成icLaye本Cont本olle本::Deacti正ateSt本ate成icLaye本()
{
    bIsActi正e = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("St本ate成ic Laye本 deacti正ated"));
}

// Map Mana成e設置ent
正oid UMin成St本ate成icLaye本Cont本olle本::LoadSt本ate成icMap(const 軍St本in成& MapID)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loadin成 st本ate成ic 設置ap: %s"), *MapID);
    
    C使本本entMapID = MapID;
    
    // In a 本eal i設置ple設置entation, this wo使ld load 設置ap data f本o設置 assets
    // 軍o本 now, we initialize with defa使lts
    InitializeDefa使ltRe成ions();
}

正oid UMin成St本ate成icLaye本Cont本olle本::Clea本St本ate成icMap()
{
    Re成ions.E設置pty();
    SelectedRe成ionID.E設置pty();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("St本ate成ic 設置ap clea本ed"));
}

正oid UMin成St本ate成icLaye本Cont本olle本::AddRe成ion(const 軍Min成MapRe成ion& Re成ion)
{
    if (Re成ion.Re成ionID.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot add 本e成ion with e設置pty ID"));
        本et使本n;
    }
    
    Re成ions.Add(Re成ion.Re成ionID, Re成ion);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Added 本e成ion: %s"), *Re成ion.Re成ion的a設置e);
}

正oid UMin成St本ate成icLaye本Cont本olle本::Re設置o正eRe成ion(const 軍St本in成& Re成ionID)
{
    Re成ions.Re設置o正e(Re成ionID);
    
    if (SelectedRe成ionID == Re成ionID)
    {
        SelectedRe成ionID.E設置pty();
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed 本e成ion: %s"), *Re成ionID);
}

bool UMin成St本ate成icLaye本Cont本olle本::GetRe成ion(const 軍St本in成& Re成ionID, 軍Min成MapRe成ion& O使tRe成ion) const
{
    const 軍Min成MapRe成ion* 軍o使ndRe成ion = Re成ions.軍ind(Re成ionID);
    if (軍o使ndRe成ion)
    {
        O使tRe成ion = *軍o使ndRe成ion;
        本et使本n t本使e;
    }
    本et使本n false;
}

TA本本ay<軍Min成MapRe成ion> UMin成St本ate成icLaye本Cont本olle本::GetAllRe成ions() const
{
    TA本本ay<軍Min成MapRe成ion> Res使lt;
    Re成ions.Gene本ateVal使eA本本ay(Res使lt);
    本et使本n Res使lt;
}

TA本本ay<軍Min成MapRe成ion> UMin成St本ate成icLaye本Cont本olle本::GetRe成ionsBy軍action(const 軍St本in成& 軍actionID) const
{
    TA本本ay<軍Min成MapRe成ion> Res使lt;
    
    fo本 (const a使to& Pai本 : Re成ions)
    {
        if (Pai本.Val使e.Cont本ollin成軍action == 軍actionID)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍Min成MapRe成ion> UMin成St本ate成icLaye本Cont本olle本::GetContestedRe成ions() const
{
    TA本本ay<軍Min成MapRe成ion> Res使lt;
    
    fo本 (const a使to& Pai本 : Re成ions)
    {
        if (Pai本.Val使e.bIsContested)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    
    本et使本n Res使lt;
}

// 軍action Mana成e設置ent
正oid UMin成St本ate成icLaye本Cont本olle本::Re成iste本軍action(const 軍Min成軍actionInfo& 軍action)
{
    if (軍action.軍actionID.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot 本e成iste本 faction with e設置pty ID"));
        本et使本n;
    }
    
    軍actions.Add(軍action.軍actionID, 軍action);
    
    if (軍action.bIsPlaye本)
    {
        Playe本軍actionID = 軍action.軍actionID;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re成iste本ed faction: %s"), *軍action.軍action的a設置e);
}

正oid UMin成St本ate成icLaye本Cont本olle本::Un本e成iste本軍action(const 軍St本in成& 軍actionID)
{
    軍actions.Re設置o正e(軍actionID);
    
    // Re設置o正e 本elated diplo設置atic 本elations
    TA本本ay<軍St本in成> KeysToRe設置o正e;
    fo本 (const a使to& Pai本 : Diplo設置aticRelations)
    {
        if (Pai本.Val使e.軍actionA == 軍actionID  Pai本.Val使e.軍actionB == 軍actionID)
        {
            KeysToRe設置o正e.Add(Pai本.Key);
        }
    }
    
    fo本 (const 軍St本in成& Key : KeysToRe設置o正e)
    {
        Diplo設置aticRelations.Re設置o正e(Key);
    }
    
    if (Selected軍actionID == 軍actionID)
    {
        Selected軍actionID.E設置pty();
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Un本e成iste本ed faction: %s"), *軍actionID);
}

bool UMin成St本ate成icLaye本Cont本olle本::Get軍action(const 軍St本in成& 軍actionID, 軍Min成軍actionInfo& O使t軍action) const
{
    const 軍Min成軍actionInfo* 軍o使nd軍action = 軍actions.軍ind(軍actionID);
    if (軍o使nd軍action)
    {
        O使t軍action = *軍o使nd軍action;
        本et使本n t本使e;
    }
    本et使本n false;
}

軍Min成軍actionInfo UMin成St本ate成icLaye本Cont本olle本::GetPlaye本軍action() const
{
    軍Min成軍actionInfo Playe本軍action;
    Get軍action(Playe本軍actionID, Playe本軍action);
    本et使本n Playe本軍action;
}

TA本本ay<軍Min成軍actionInfo> UMin成St本ate成icLaye本Cont本olle本::GetAll軍actions() const
{
    TA本本ay<軍Min成軍actionInfo> Res使lt;
    軍actions.Gene本ateVal使eA本本ay(Res使lt);
    本et使本n Res使lt;
}

正oid UMin成St本ate成icLaye本Cont本olle本::Update軍actionSt本en成th(const 軍St本in成& 軍actionID, int32 Milita本yDelta, int32 Econo設置icDelta)
{
    軍Min成軍actionInfo* 軍action = 軍actions.軍ind(軍actionID);
    if (軍action)
    {
        軍action->Milita本ySt本en成th = 軍Math::Max(0, 軍action->Milita本ySt本en成th + Milita本yDelta);
        軍action->Econo設置icSt本en成th = 軍Math::Max(0, 軍action->Econo設置icSt本en成th + Econo設置icDelta);
        
        的otify軍actionChan成ed(軍actionID);
    }
}

// Diplo設置acy Mana成e設置ent
正oid UMin成St本ate成icLaye本Cont本olle本::SetDiplo設置aticRelation(const 軍St本in成& 軍actionA, const 軍St本in成& 軍actionB, EMin成軍actionRelation Relation)
{
    軍St本in成 Key = Gene本ateRelationKey(軍actionA, 軍actionB);
    
    軍Min成Diplo設置aticRelation DiploRelation;
    DiploRelation.軍actionA = 軍actionA;
    DiploRelation.軍actionB = 軍actionB;
    DiploRelation.RelationType = Relation;
    
    switch (Relation)
    {
    case EMin成軍actionRelation::Allied:
        DiploRelation.b輸入asAlliance = t本使e;
        DiploRelation.RelationVal使e = 80;
        b本eak;
    case EMin成軍actionRelation::軍本iendly:
        DiploRelation.RelationVal使e = 50;
        b本eak;
    case EMin成軍actionRelation::的e使t本al:
        DiploRelation.RelationVal使e = 0;
        b本eak;
    case EMin成軍actionRelation::輸入ostile:
        DiploRelation.RelationVal使e = -50;
        b本eak;
    case EMin成軍actionRelation::At基本a本:
        DiploRelation.bIsAt基本a本 = t本使e;
        DiploRelation.RelationVal使e = -100;
        b本eak;
    }
    
    Diplo設置aticRelations.Add(Key, DiploRelation);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set 本elation between %s and %s to %d"), 
        *軍actionA, *軍actionB, (int32)Relation);
}

軍Min成Diplo設置aticRelation UMin成St本ate成icLaye本Cont本olle本::GetDiplo設置aticRelation(const 軍St本in成& 軍actionA, const 軍St本in成& 軍actionB) const
{
    軍St本in成 Key = Gene本ateRelationKey(軍actionA, 軍actionB);
    const 軍Min成Diplo設置aticRelation* 軍o使nd = Diplo設置aticRelations.軍ind(Key);
    
    if (軍o使nd)
    {
        本et使本n *軍o使nd;
    }
    
    // Ret使本n defa使lt ne使t本al 本elation
    軍Min成Diplo設置aticRelation Defa使lt;
    Defa使lt.軍actionA = 軍actionA;
    Defa使lt.軍actionB = 軍actionB;
    本et使本n Defa使lt;
}

正oid UMin成St本ate成icLaye本Cont本olle本::Decla本e基本a本(const 軍St本in成& Attacke本軍action, const 軍St本in成& Defende本軍action)
{
    SetDiplo設置aticRelation(Attacke本軍action, Defende本軍action, EMin成軍actionRelation::At基本a本);
    
    // Update wa本 stat使s
    軍St本in成 Key = Gene本ateRelationKey(Attacke本軍action, Defende本軍action);
    軍Min成Diplo設置aticRelation* Relation = Diplo設置aticRelations.軍ind(Key);
    if (Relation)
    {
        Relation->bIsAt基本a本 = t本使e;
        Relation->基本a本D使本ationDays = 0;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("基本a本 decla本ed: %s 正s %s"), *Attacke本軍action, *Defende本軍action);
    
    OnDiplo設置aticAction.B本oadcast(Defende本軍action, EMin成St本ate成icAction::Decla本e基本a本);
}

正oid UMin成St本ate成icLaye本Cont本olle本::的e成otiatePeace(const 軍St本in成& 軍actionA, const 軍St本in成& 軍actionB)
{
    軍St本in成 Key = Gene本ateRelationKey(軍actionA, 軍actionB);
    軍Min成Diplo設置aticRelation* Relation = Diplo設置aticRelations.軍ind(Key);
    
    if (Relation && Relation->bIsAt基本a本)
    {
        Relation->bIsAt基本a本 = false;
        Relation->基本a本D使本ationDays = 0;
        Relation->RelationType = EMin成軍actionRelation::輸入ostile;
        Relation->RelationVal使e = -50;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Peace ne成otiated between %s and %s"), *軍actionA, *軍actionB);
    }
}

正oid UMin成St本ate成icLaye本Cont本olle本::軍o本設置Alliance(const 軍St本in成& 軍actionA, const 軍St本in成& 軍actionB, int32 D使本ationDays)
{
    SetDiplo設置aticRelation(軍actionA, 軍actionB, EMin成軍actionRelation::Allied);
    
    軍St本in成 Key = Gene本ateRelationKey(軍actionA, 軍actionB);
    軍Min成Diplo設置aticRelation* Relation = Diplo設置aticRelations.軍ind(Key);
    if (Relation)
    {
        Relation->b輸入asAlliance = t本使e;
        Relation->Acti正eT本eaties.Add(軍St本in成::P本intf(TEXT("Alliance (%d days)"), D使本ationDays));
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Alliance fo本設置ed: %s and %s fo本 %d days"), 
        *軍actionA, *軍actionB, D使本ationDays);
    
    OnDiplo設置aticAction.B本oadcast(軍actionB, EMin成St本ate成icAction::軍o本設置Alliance);
}

正oid UMin成St本ate成icLaye本Cont本olle本::EstablishT本adeA成本ee設置ent(const 軍St本in成& 軍actionA, const 軍St本in成& 軍actionB)
{
    軍St本in成 Key = Gene本ateRelationKey(軍actionA, 軍actionB);
    軍Min成Diplo設置aticRelation* Relation = Diplo設置aticRelations.軍ind(Key);
    
    if (Relation)
    {
        Relation->b輸入asT本adeA成本ee設置ent = t本使e;
        Relation->Acti正eT本eaties.Add(TEXT("T本ade A成本ee設置ent"));
        
        if (Relation->RelationType == EMin成軍actionRelation::的e使t本al)
        {
            Relation->RelationType = EMin成軍actionRelation::軍本iendly;
            Relation->RelationVal使e = 30;
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("T本ade a成本ee設置ent established: %s and %s"), *軍actionA, *軍actionB);
    
    OnDiplo設置aticAction.B本oadcast(軍actionB, EMin成St本ate成icAction::T本adeA成本ee設置ent);
}

正oid UMin成St本ate成icLaye本Cont本olle本::B本eakT本eaty(const 軍St本in成& 軍actionA, const 軍St本in成& 軍actionB, const 軍St本in成& T本eatyType)
{
    軍St本in成 Key = Gene本ateRelationKey(軍actionA, 軍actionB);
    軍Min成Diplo設置aticRelation* Relation = Diplo設置aticRelations.軍ind(Key);
    
    if (Relation)
    {
        if (T本eatyType == TEXT("Alliance"))
        {
            Relation->b輸入asAlliance = false;
            Relation->RelationType = EMin成軍actionRelation::的e使t本al;
        }
        else if (T本eatyType == TEXT("T本ade"))
        {
            Relation->b輸入asT本adeA成本ee設置ent = false;
        }
        
        Relation->Acti正eT本eaties.Re設置o正e(T本eatyType);
        Relation->RelationVal使e -= 20; // B本eakin成 t本eaty da設置a成es 本elations
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("T本eaty b本oken: %s between %s and %s"), *T本eatyType, *軍actionA, *軍actionB);
    }
}

TA本本ay<軍Min成Diplo設置aticRelation> UMin成St本ate成icLaye本Cont本olle本::Get軍actionRelations(const 軍St本in成& 軍actionID) const
{
    TA本本ay<軍Min成Diplo設置aticRelation> Res使lt;
    
    fo本 (const a使to& Pai本 : Diplo設置aticRelations)
    {
        if (Pai本.Val使e.軍actionA == 軍actionID  Pai本.Val使e.軍actionB == 軍actionID)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍St本in成> UMin成St本ate成icLaye本Cont本olle本::GetAllies(const 軍St本in成& 軍actionID) const
{
    TA本本ay<軍St本in成> Res使lt;
    
    fo本 (const a使to& Pai本 : Diplo設置aticRelations)
    {
        if (Pai本.Val使e.RelationType == EMin成軍actionRelation::Allied)
        {
            if (Pai本.Val使e.軍actionA == 軍actionID)
            {
                Res使lt.Add(Pai本.Val使e.軍actionB);
            }
            else if (Pai本.Val使e.軍actionB == 軍actionID)
            {
                Res使lt.Add(Pai本.Val使e.軍actionA);
            }
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍St本in成> UMin成St本ate成icLaye本Cont本olle本::GetEne設置ies(const 軍St本in成& 軍actionID) const
{
    TA本本ay<軍St本in成> Res使lt;
    
    fo本 (const a使to& Pai本 : Diplo設置aticRelations)
    {
        if (Pai本.Val使e.bIsAt基本a本)
        {
            if (Pai本.Val使e.軍actionA == 軍actionID)
            {
                Res使lt.Add(Pai本.Val使e.軍actionB);
            }
            else if (Pai本.Val使e.軍actionB == 軍actionID)
            {
                Res使lt.Add(Pai本.Val使e.軍actionA);
            }
        }
    }
    
    本et使本n Res使lt;
}

// St本ate成ic Actions
正oid UMin成St本ate成icLaye本Cont本olle本::Exec使teSt本ate成icDecision(const 軍Min成St本ate成icDecision& Decision)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使tin成 st本ate成ic decision: %s"), *Decision.Decision的a設置e);
    
    OnSt本ate成icO本de本Iss使ed.B本oadcast(Decision);
    
    // Exec使te based on action type
    switch (Decision.ActionType)
    {
    case EMin成St本ate成icAction::Mo正eT本oops:
        if (Decision.In正ol正edRe成ions.的使設置() >= 2)
        {
            Mo正eT本oops(Decision.In正ol正edRe成ions[0], Decision.In正ol正edRe成ions[1], Decision.T本oopReq使i本e設置ent, TEXT("Playe本"));
        }
        b本eak;
        
    case EMin成St本ate成icAction::Decla本e基本a本:
        Decla本e基本a本(TEXT("Playe本"), Decision.Ta本成et軍action);
        b本eak;
        
    case EMin成St本ate成icAction::軍o本設置Alliance:
        軍o本設置Alliance(TEXT("Playe本"), Decision.Ta本成et軍action, 30);
        b本eak;
        
    defa使lt:
        UE下LOG(Lo成Te設置p, Lo成, TEXT("St本ate成ic action type %d exec使ted"), (int32)Decision.ActionType);
        b本eak;
    }
}

正oid UMin成St本ate成icLaye本Cont本olle本::Mo正eT本oops(const 軍St本in成& 軍本o設置Re成ion, const 軍St本in成& ToRe成ion, int32 T本oopCo使nt, const 軍St本in成& 軍actionID)
{
    軍Min成MapRe成ion* So使本ceRe成ion = Re成ions.軍ind(軍本o設置Re成ion);
    軍Min成MapRe成ion* Ta本成etRe成ion = Re成ions.軍ind(ToRe成ion);
    
    if (So使本ceRe成ion && Ta本成etRe成ion)
    {
        // Check if 本e成ions a本e connected
        if (So使本ceRe成ion->ConnectedRe成ions.Contains(ToRe成ion))
        {
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Mo正in成 %d t本oops f本o設置 %s to %s"), 
                T本oopCo使nt, *So使本ceRe成ion->Re成ion的a設置e, *Ta本成etRe成ion->Re成ion的a設置e);
            
            // In 本eal i設置ple設置entation, this wo使ld act使ally 設置o正e t本oops
            // Update 成a本本ison st本en成ths
            So使本ceRe成ion->Ga本本isonSt本en成th = 軍Math::Max(0, So使本ceRe成ion->Ga本本isonSt本en成th - T本oopCo使nt);
        }
        else
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Re成ions %s and %s a本e not connected"), *軍本o設置Re成ion, *ToRe成ion);
        }
    }
}

正oid UMin成St本ate成icLaye本Cont本olle本::軍o本tifyRe成ion(const 軍St本in成& Re成ionID, int32 Ga本本isonInc本ease)
{
    軍Min成MapRe成ion* Re成ion = Re成ions.軍ind(Re成ionID);
    if (Re成ion)
    {
        Re成ion->Ga本本isonSt本en成th += Ga本本isonInc本ease;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("軍o本tified %s. Ga本本ison now: %d"), 
            *Re成ion->Re成ion的a設置e, Re成ion->Ga本本isonSt本en成th);
        
        的otifyRe成ionChan成ed(Re成ionID);
    }
}

正oid UMin成St本ate成icLaye本Cont本olle本::De正elopRe成ion(const 軍St本in成& Re成ionID, const 軍St本in成& De正elop設置entType)
{
    軍Min成MapRe成ion* Re成ion = Re成ions.軍ind(Re成ionID);
    if (Re成ion)
    {
        if (De正elop設置entType == TEXT("Econo設置ic"))
        {
            Re成ion->Reso使本ceVal使e += 10;
        }
        else if (De正elop設置entType == TEXT("Milita本y"))
        {
            Re成ion->St本ate成icVal使e += 10;
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("De正eloped %s: %s"), *Re成ion->Re成ion的a設置e, *De正elop設置entType);
        的otifyRe成ionChan成ed(Re成ionID);
    }
}

正oid UMin成St本ate成icLaye本Cont本olle本::La使nchCa設置pai成n(const 軍St本in成& 軍本o設置Re成ion, const 軍St本in成& Ta本成etRe成ion, int32 T本oopCo使nt)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("La使nchin成 ca設置pai成n f本o設置 %s to %s with %d t本oops"), 
        *軍本o設置Re成ion, *Ta本成etRe成ion, T本oopCo使nt);
    
    軍Min成MapRe成ion* Ta本成et = Re成ions.軍ind(Ta本成etRe成ion);
    if (Ta本成et)
    {
        Ta本成et->bIsContested = t本使e;
        
        // C本eate a st本ate成ic decision fo本 the battle
        軍Min成St本ate成icDecision Decision;
        Decision.DecisionID = 軍G使id::的ewG使id().ToSt本in成();
        Decision.Decision的a設置e = 軍St本in成::P本intf(TEXT("Ca設置pai成n: %s"), *Ta本成et->Re成ion的a設置e);
        Decision.ActionType = EMin成St本ate成icAction::的one; // This t本i成成e本s tactical laye本
        Decision.In正ol正edRe成ions = { 軍本o設置Re成ion, Ta本成etRe成ion };
        Decision.T本oopReq使i本e設置ent = T本oopCo使nt;
        
        OnSt本ate成icO本de本Iss使ed.B本oadcast(Decision);
    }
}

// UI Inte本face
正oid UMin成St本ate成icLaye本Cont本olle本::SelectRe成ion(const 軍St本in成& Re成ionID)
{
    SelectedRe成ionID = Re成ionID;
    
    軍Min成MapRe成ion Re成ion;
    if (GetRe成ion(Re成ionID, Re成ion))
    {
        OnRe成ionSelected.B本oadcast(Re成ion);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Selected 本e成ion: %s"), *Re成ion.Re成ion的a設置e);
    }
}

正oid UMin成St本ate成icLaye本Cont本olle本::Select軍action(const 軍St本in成& 軍actionID)
{
    Selected軍actionID = 軍actionID;
    
    軍Min成軍actionInfo 軍action;
    if (Get軍action(軍actionID, 軍action))
    {
        On軍actionSelected.B本oadcast(軍action);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Selected faction: %s"), *軍action.軍action的a設置e);
    }
}

正oid UMin成St本ate成icLaye本Cont本olle本::Clea本Selection()
{
    SelectedRe成ionID.E設置pty();
    Selected軍actionID.E設置pty();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Selection clea本ed"));
}

// Inte本nal initialization
正oid UMin成St本ate成icLaye本Cont本olle本::InitializeDefa使lt軍actions()
{
    // Playe本 faction (Rep使blican A本設置y)
    軍Min成軍actionInfo Playe本軍action;
    Playe本軍action.軍actionID = TEXT("Playe本");
    Playe本軍action.軍action的a設置e = TEXT("國民革命軍");
    Playe本軍action.軍actionColo本 = 軍Linea本Colo本(0.0f, 0.3f, 0.7f);
    Playe本軍action.Milita本ySt本en成th = 1000;
    Playe本軍action.Econo設置icSt本en成th = 500;
    Playe本軍action.Diplo設置aticInfl使ence = 300;
    Playe本軍action.bIsPlaye本 = t本使e;
    Re成iste本軍action(Playe本軍action);
    
    // 基本a本lo本d faction
    軍Min成軍actionInfo 基本a本lo本d軍action;
    基本a本lo本d軍action.軍actionID = TEXT("基本a本lo本d");
    基本a本lo本d軍action.軍action的a設置e = TEXT("北洋軍閥");
    基本a本lo本d軍action.軍actionColo本 = 軍Linea本Colo本(0.7f, 0.2f, 0.1f);
    基本a本lo本d軍action.Milita本ySt本en成th = 800;
    基本a本lo本d軍action.Econo設置icSt本en成th = 400;
    基本a本lo本d軍action.Diplo設置aticInfl使ence = 200;
    Re成iste本軍action(基本a本lo本d軍action);
    
    // Japanese faction
    軍Min成軍actionInfo Japanese軍action;
    Japanese軍action.軍actionID = TEXT("Japanese");
    Japanese軍action.軍action的a設置e = TEXT("關東軍");
    Japanese軍action.軍actionColo本 = 軍Linea本Colo本(0.9f, 0.1f, 0.1f);
    Japanese軍action.Milita本ySt本en成th = 1200;
    Japanese軍action.Econo設置icSt本en成th = 600;
    Japanese軍action.Diplo設置aticInfl使ence = 250;
    Re成iste本軍action(Japanese軍action);
    
    // Co設置設置使nist faction
    軍Min成軍actionInfo Co設置設置使nist軍action;
    Co設置設置使nist軍action.軍actionID = TEXT("Co設置設置使nist");
    Co設置設置使nist軍action.軍action的a設置e = TEXT("工農紅軍");
    Co設置設置使nist軍action.軍actionColo本 = 軍Linea本Colo本(0.9f, 0.1f, 0.1f);
    Co設置設置使nist軍action.Milita本ySt本en成th = 400;
    Co設置設置使nist軍action.Econo設置icSt本en成th = 200;
    Co設置設置使nist軍action.Diplo設置aticInfl使ence = 150;
    Re成iste本軍action(Co設置設置使nist軍action);
}

正oid UMin成St本ate成icLaye本Cont本olle本::InitializeDefa使ltRe成ions()
{
    // Bei大in成
    軍Min成MapRe成ion Bei大in成;
    Bei大in成.Re成ionID = TEXT("Bei大in成");
    Bei大in成.Re成ion的a設置e = TEXT("北平");
    Bei大in成.Re成ionType = EMin成MapRe成ionType::City;
    Bei大in成.MapPosition = 軍Vecto本2D(500, 300);
    Bei大in成.Cont本ollin成軍action = TEXT("基本a本lo本d");
    Bei大in成.Reso使本ceVal使e = 50;
    Bei大in成.St本ate成icVal使e = 100;
    Bei大in成.Ga本本isonSt本en成th = 200;
    Bei大in成.bIsCapital = t本使e;
    Bei大in成.ConnectedRe成ions = { TEXT("Tian大in"), TEXT("Shi大iazh使an成") };
    AddRe成ion(Bei大in成);
    
    // Shan成hai
    軍Min成MapRe成ion Shan成hai;
    Shan成hai.Re成ionID = TEXT("Shan成hai");
    Shan成hai.Re成ion的a設置e = TEXT("上海");
    Shan成hai.Re成ionType = EMin成MapRe成ionType::City;
    Shan成hai.MapPosition = 軍Vecto本2D(600, 500);
    Shan成hai.Cont本ollin成軍action = TEXT("Playe本");
    Shan成hai.Reso使本ceVal使e = 80;
    Shan成hai.St本ate成icVal使e = 90;
    Shan成hai.Ga本本isonSt本en成th = 150;
    Shan成hai.ConnectedRe成ions = { TEXT("的an大in成"), TEXT("輸入an成zho使") };
    AddRe成ion(Shan成hai);
    
    // 的an大in成
    軍Min成MapRe成ion 的an大in成;
    的an大in成.Re成ionID = TEXT("的an大in成");
    的an大in成.Re成ion的a設置e = TEXT("南京");
    的an大in成.Re成ionType = EMin成MapRe成ionType::City;
    的an大in成.MapPosition = 軍Vecto本2D(550, 480);
    的an大in成.Cont本ollin成軍action = TEXT("Playe本");
    的an大in成.Reso使本ceVal使e = 60;
    的an大in成.St本ate成icVal使e = 85;
    的an大in成.Ga本本isonSt本en成th = 180;
    的an大in成.ConnectedRe成ions = { TEXT("Shan成hai"), TEXT("輸入efei") };
    AddRe成ion(的an大in成);
    
    // 基本使han
    軍Min成MapRe成ion 基本使han;
    基本使han.Re成ionID = TEXT("基本使han");
    基本使han.Re成ion的a設置e = TEXT("武漢");
    基本使han.Re成ionType = EMin成MapRe成ionType::St本ate成icPass;
    基本使han.MapPosition = 軍Vecto本2D(450, 450);
    基本使han.Cont本ollin成軍action = TEXT("基本a本lo本d");
    基本使han.Reso使本ceVal使e = 40;
    基本使han.St本ate成icVal使e = 95;
    基本使han.Ga本本isonSt本en成th = 100;
    基本使han.ConnectedRe成ions = { TEXT("的an大in成"), TEXT("Chan成sha") };
    AddRe成ion(基本使han);
}

正oid UMin成St本ate成icLaye本Cont本olle本::Set使pDiplo設置aticDefa使lts()
{
    // Playe本 正s 基本a本lo本d - At 基本a本
    Decla本e基本a本(TEXT("Playe本"), TEXT("基本a本lo本d"));
    
    // Playe本 正s Co設置設置使nist - 的e使t本al
    SetDiplo設置aticRelation(TEXT("Playe本"), TEXT("Co設置設置使nist"), EMin成軍actionRelation::的e使t本al);
    
    // 基本a本lo本d 正s Japanese - Allied
    軍o本設置Alliance(TEXT("基本a本lo本d"), TEXT("Japanese"), 60);
}

軍St本in成 UMin成St本ate成icLaye本Cont本olle本::Gene本ateRelationKey(const 軍St本in成& 軍actionA, const 軍St本in成& 軍actionB) const
{
    // So本t to ens使本e consistent key 本e成a本dless of o本de本
    if (軍actionA < 軍actionB)
    {
        本et使本n 軍St本in成::P本intf(TEXT("%s下%s"), *軍actionA, *軍actionB);
    }
    else
    {
        本et使本n 軍St本in成::P本intf(TEXT("%s下%s"), *軍actionB, *軍actionA);
    }
}

正oid UMin成St本ate成icLaye本Cont本olle本::的otifyRe成ionChan成ed(const 軍St本in成& Re成ionID)
{
    軍Min成MapRe成ion Re成ion;
    if (GetRe成ion(Re成ionID, Re成ion))
    {
        OnRe成ionSelected.B本oadcast(Re成ion);
    }
}

正oid UMin成St本ate成icLaye本Cont本olle本::的otify軍actionChan成ed(const 軍St本in成& 軍actionID)
{
    軍Min成軍actionInfo 軍action;
    if (Get軍action(軍actionID, 軍action))
    {
        On軍actionSelected.B本oadcast(軍action);
    }
}
