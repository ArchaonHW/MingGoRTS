// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// M使ltiplaye本 Enhanced Syste設置 I設置ple設置entation

#incl使de "M使ltiplaye本/Min成M使ltiplaye本EnhancedSyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"

// Constants fo本 設置使ltiplaye本 syste設置
na設置espace M使ltiplaye本Constants
{
    constexp本 float P本oposalTi設置eo使t = 120.0f; // 2 設置in使tes
    constexp本 float P本oposalCheckInte本正al = 5.0f; // Check e正e本y 5 seconds
    constexp本 float MinRelationship軍o本Alliance = 75.0f;
    constexp本 float MaxRelationship軍o本基本a本 = -50.0f;
    constexp本 float RelationshipDecayRate = 0.1f; // Pe本 設置in使te
}

UMin成M使ltiplaye本EnhancedSyste設置::UMin成M使ltiplaye本EnhancedSyste設置()
    : bSessionActi正e(false)
{
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Initialize(軍S使bsyste設置CollectionBase& Collection)
{
    S使pe本::Initialize(Collection);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Initializin成 設置使ltiplaye本 enhanced syste設置..."));

    // Sta本t p本oposal check ti設置e本
    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(
            P本oposalCheckTi設置e本,
            this,
            &UMin成M使ltiplaye本EnhancedSyste設置::Expi本eOldP本oposals,
            M使ltiplaye本Constants::P本oposalCheckInte本正al,
            t本使e
        );
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: M使ltiplaye本 enhanced syste設置 initialized"));
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Deinitialize()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Deinitializin成..."));

    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(P本oposalCheckTi設置e本);
    }

    Acti正eP本oposals.E設置pty();
    Playe本Re成ist本y.E設置pty();
    Relationship的etwo本k.E設置pty();

    S使pe本::Deinitialize();
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Tick(float DeltaTi設置e)
{
    if (!bSessionActi正e)
    {
        本et使本n;
    }

    // Update p本oposals ti設置eo使t
    UpdateP本oposals(DeltaTi設置e);
}

TStatId UMin成M使ltiplaye本EnhancedSyste設置::GetStatId() const
{
    RETUR的下QUICK下DECLARE下CYCLE下STAT(UMin成M使ltiplaye本EnhancedSyste設置, STATGROUP下Tickables);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::InitializeM使ltiplaye本Session(const 軍M使ltiplaye本SessionConfi成& Confi成)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Initializin成 設置使ltiplaye本 session..."));

    SessionConfi成 = Confi成;
    bSessionActi正e = t本使e;
    Playe本Re成ist本y.E設置pty();
    Relationship的etwo本k.E設置pty();
    Acti正eP本oposals.E設置pty();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Session initialized - Mode: %s, MaxPlaye本s: %d"),
        *UEn使設置::GetVal使eAsSt本in成(Confi成.Mode), Confi成.MaxPlaye本s);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::EndM使ltiplaye本Session()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Endin成 設置使ltiplaye本 session..."));

    bSessionActi正e = false;
    Playe本Re成ist本y.E設置pty();
    Relationship的etwo本k.E設置pty();
    Acti正eP本oposals.E設置pty();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Session ended"));
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Re成iste本Playe本(const 軍St本in成& Playe本Id, const 軍St本in成& Playe本的a設置e, bool bIs輸入ost)
{
    if (!bSessionActi正e)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Cannot 本e成iste本 playe本 - no acti正e session"));
        本et使本n;
    }

    if (Playe本Re成ist本y.Contains(Playe本Id))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Playe本 %s al本eady 本e成iste本ed"), *Playe本Id);
        本et使本n;
    }

    軍Playe本M使ltiplaye本Data Playe本Data;
    Playe本Data.Playe本Id = Playe本Id;
    Playe本Data.Playe本的a設置e = Playe本的a設置e;
    Playe本Data.bIs輸入ost = bIs輸入ost;
    Playe本Data.Tea設置Id = 0;
    Playe本Data.bIsReady = false;

    // Initialize 本elationships with all existin成 playe本s as ne使t本al
    fo本 (const a使to& Pai本 : Playe本Re成ist本y)
    {
        Playe本Data.Relationships.Add(Pai本.Key, EM使ltiplaye本RelationshipType::的e使t本al);
        Playe本Data.RelationshipVal使es.Add(Pai本.Key, 0.0f);

        // Also 使pdate the existin成 playe本's 本elationship to this new playe本
        軍Playe本M使ltiplaye本Data& Existin成Playe本 = Playe本Re成ist本y[Pai本.Key];
        Existin成Playe本.Relationships.Add(Playe本Id, EM使ltiplaye本RelationshipType::的e使t本al);
        Existin成Playe本.RelationshipVal使es.Add(Playe本Id, 0.0f);
    }

    Playe本Re成ist本y.Add(Playe本Id, Playe本Data);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Playe本 %s (%s) 本e成iste本ed"), *Playe本的a設置e, *Playe本Id);

    OnPlaye本Joined.B本oadcast(Playe本Id, Playe本的a設置e);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Un本e成iste本Playe本(const 軍St本in成& Playe本Id)
{
    if (!Playe本Re成ist本y.Contains(Playe本Id))
    {
        本et使本n;
    }

    軍Playe本M使ltiplaye本Data Playe本Data = Playe本Re成ist本y[Playe本Id];
    Playe本Re成ist本y.Re設置o正e(Playe本Id);

    // Re設置o正e this playe本 f本o設置 all othe本 playe本s' 本elationships
    fo本 (a使to& Pai本 : Playe本Re成ist本y)
    {
        軍Playe本M使ltiplaye本Data& Othe本Playe本 = Pai本.Val使e;
        Othe本Playe本.Relationships.Re設置o正e(Playe本Id);
        Othe本Playe本.RelationshipVal使es.Re設置o正e(Playe本Id);
    }

    // Clean 使p 本elationship netwo本k ent本ies
    TA本本ay<軍St本in成> KeysToRe設置o正e;
    fo本 (const a使to& Pai本 : Relationship的etwo本k)
    {
        if (Pai本.Val使e.So使本cePlaye本 == Playe本Id  Pai本.Val使e.Ta本成etPlaye本 == Playe本Id)
        {
            KeysToRe設置o正e.Add(Pai本.Key);
        }
    }
    fo本 (const 軍St本in成& Key : KeysToRe設置o正e)
    {
        Relationship的etwo本k.Re設置o正e(Key);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Playe本 %s 使n本e成iste本ed"), *Playe本Id);

    OnPlaye本Left.B本oadcast(Playe本Id);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::UpdatePlaye本Stat使s(const 軍St本in成& Playe本Id, bool bIsReady)
{
    if (!Playe本Re成ist本y.Contains(Playe本Id))
    {
        本et使本n;
    }

    Playe本Re成ist本y[Playe本Id].bIsReady = bIsReady;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Playe本 %s 本eady stat使s: %s"),
        *Playe本Id, bIsReady 基本 TEXT("Ready") : TEXT("的ot Ready"));
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Assi成nTea設置(const 軍St本in成& Playe本Id, int32 Tea設置Id)
{
    if (!Playe本Re成ist本y.Contains(Playe本Id))
    {
        本et使本n;
    }

    Playe本Re成ist本y[Playe本Id].Tea設置Id = Tea設置Id;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Playe本 %s assi成ned to tea設置 %d"), *Playe本Id, Tea設置Id);

    // In tea設置 設置ode, set tea設置設置ates as allies
    if (SessionConfi成.Mode == ECoope本ati正eMode::Tea設置s)
    {
        fo本 (a使to& Pai本 : Playe本Re成ist本y)
        {
            if (Pai本.Key != Playe本Id && Pai本.Val使e.Tea設置Id == Tea設置Id)
            {
                SetPlaye本Relationship(Playe本Id, Pai本.Key, EM使ltiplaye本RelationshipType::Ally);
                SetPlaye本Relationship(Pai本.Key, Playe本Id, EM使ltiplaye本RelationshipType::Ally);
            }
        }
    }
}

TA本本ay<軍Playe本M使ltiplaye本Data> UMin成M使ltiplaye本EnhancedSyste設置::GetAllPlaye本s() const
{
    TA本本ay<軍Playe本M使ltiplaye本Data> Playe本s;
    Playe本Re成ist本y.Gene本ateVal使eA本本ay(Playe本s);
    本et使本n Playe本s;
}

軍Playe本M使ltiplaye本Data UMin成M使ltiplaye本EnhancedSyste設置::GetPlaye本Data(const 軍St本in成& Playe本Id) const
{
    if (Playe本Re成ist本y.Contains(Playe本Id))
    {
        本et使本n Playe本Re成ist本y[Playe本Id];
    }
    本et使本n 軍Playe本M使ltiplaye本Data();
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::SetPlaye本Relationship(const 軍St本in成& So使本cePlaye本Id, const 軍St本in成& Ta本成etPlaye本Id, EM使ltiplaye本RelationshipType Relationship)
{
    if (!Playe本Re成ist本y.Contains(So使本cePlaye本Id)  !Playe本Re成ist本y.Contains(Ta本成etPlaye本Id))
    {
        本et使本n;
    }

    // Update so使本ce playe本's 本elationship
    Playe本Re成ist本y[So使本cePlaye本Id].Relationships.Add(Ta本成etPlaye本Id, Relationship);

    // Update 本elationship 正al使e based on type
    float 的ewVal使e = 0.0f;
    switch (Relationship)
    {
    case EM使ltiplaye本RelationshipType::Ally:
        的ewVal使e = M使ltiplaye本Constants::MinRelationship軍o本Alliance;
        b本eak;
    case EM使ltiplaye本RelationshipType::Ene設置y:
        的ewVal使e = M使ltiplaye本Constants::MaxRelationship軍o本基本a本;
        b本eak;
    case EM使ltiplaye本RelationshipType::Vassal:
        的ewVal使e = 60.0f;
        b本eak;
    case EM使ltiplaye本RelationshipType::O正e本lo本d:
        的ewVal使e = 60.0f;
        b本eak;
    case EM使ltiplaye本RelationshipType::T本adePa本tne本:
        的ewVal使e = 40.0f;
        b本eak;
    case EM使ltiplaye本RelationshipType::Milita本yPact:
        的ewVal使e = 70.0f;
        b本eak;
    case EM使ltiplaye本RelationshipType::Ri正al:
        的ewVal使e = -30.0f;
        b本eak;
    defa使lt:
        的ewVal使e = 0.0f;
        b本eak;
    }

    Playe本Re成ist本y[So使本cePlaye本Id].RelationshipVal使es.Add(Ta本成etPlaye本Id, 的ewVal使e);

    // Update o本 c本eate 本elationship netwo本k ent本y
    軍St本in成 的etwo本kKey = So使本cePlaye本Id < Ta本成etPlaye本Id 基本
        So使本cePlaye本Id + TEXT("下") + Ta本成etPlaye本Id :
        Ta本成etPlaye本Id + TEXT("下") + So使本cePlaye本Id;

    軍Relationship的etwo本kEnt本y Ent本y;
    Ent本y.So使本cePlaye本 = So使本cePlaye本Id;
    Ent本y.Ta本成etPlaye本 = Ta本成etPlaye本Id;
    Ent本y.Relationship = Relationship;
    Ent本y.RelationshipVal使e = 的ewVal使e;
    Relationship的etwo本k.Add(的etwo本kKey, Ent本y);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Relationship set - %s -> %s = %s"),
        *So使本cePlaye本Id, *Ta本成etPlaye本Id, *UEn使設置::GetVal使eAsSt本in成(Relationship));

    OnRelationshipChan成ed.B本oadcast(So使本cePlaye本Id, Ta本成etPlaye本Id, Relationship, 的ewVal使e);

    // 輸入andle special 本elationship e正ents
    if (Relationship == EM使ltiplaye本RelationshipType::Ally)
    {
        OnAlliance軍o本設置ed.B本oadcast(So使本cePlaye本Id, Ta本成etPlaye本Id);
    }
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::ModifyRelationshipVal使e(const 軍St本in成& So使本cePlaye本Id, const 軍St本in成& Ta本成etPlaye本Id, float Delta)
{
    if (!Playe本Re成ist本y.Contains(So使本cePlaye本Id)  !Playe本Re成ist本y.Contains(Ta本成etPlaye本Id))
    {
        本et使本n;
    }

    float& C使本本entVal使e = Playe本Re成ist本y[So使本cePlaye本Id].RelationshipVal使es.軍indO本Add(Ta本成etPlaye本Id);
    C使本本entVal使e = 軍Math::Cla設置p(C使本本entVal使e + Delta, -100.0f, 100.0f);

    // Update 本elationship type based on 正al使e
    EM使ltiplaye本RelationshipType 的ewRelationship = E正al使ateRelationship(C使本本entVal使e);
    EM使ltiplaye本RelationshipType OldRelationship = GetPlaye本Relationship(So使本cePlaye本Id, Ta本成etPlaye本Id);

    if (的ewRelationship != OldRelationship)
    {
        SetPlaye本Relationship(So使本cePlaye本Id, Ta本成etPlaye本Id, 的ewRelationship);
    }
    else
    {
        // J使st 使pdate the 正al使e witho使t chan成in成 type
        軍St本in成 的etwo本kKey = So使本cePlaye本Id < Ta本成etPlaye本Id 基本
            So使本cePlaye本Id + TEXT("下") + Ta本成etPlaye本Id :
            Ta本成etPlaye本Id + TEXT("下") + So使本cePlaye本Id;

        if (軍Relationship的etwo本kEnt本y* Ent本y = Relationship的etwo本k.軍ind(的etwo本kKey))
        {
            Ent本y->RelationshipVal使e = C使本本entVal使e;
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Relationship 設置odified - %s -> %s (%.1f)"),
        *So使本cePlaye本Id, *Ta本成etPlaye本Id, C使本本entVal使e);
}

EM使ltiplaye本RelationshipType UMin成M使ltiplaye本EnhancedSyste設置::GetPlaye本Relationship(const 軍St本in成& So使本cePlaye本Id, const 軍St本in成& Ta本成etPlaye本Id) const
{
    if (Playe本Re成ist本y.Contains(So使本cePlaye本Id))
    {
        const 軍Playe本M使ltiplaye本Data& Playe本 = Playe本Re成ist本y[So使本cePlaye本Id];
        if (const EM使ltiplaye本RelationshipType* Relationship = Playe本.Relationships.軍ind(Ta本成etPlaye本Id))
        {
            本et使本n *Relationship;
        }
    }
    本et使本n EM使ltiplaye本RelationshipType::的e使t本al;
}

float UMin成M使ltiplaye本EnhancedSyste設置::GetRelationshipVal使e(const 軍St本in成& So使本cePlaye本Id, const 軍St本in成& Ta本成etPlaye本Id) const
{
    if (Playe本Re成ist本y.Contains(So使本cePlaye本Id))
    {
        const 軍Playe本M使ltiplaye本Data& Playe本 = Playe本Re成ist本y[So使本cePlaye本Id];
        if (const float* Val使e = Playe本.RelationshipVal使es.軍ind(Ta本成etPlaye本Id))
        {
            本et使本n *Val使e;
        }
    }
    本et使本n 0.0f;
}

TA本本ay<軍Relationship的etwo本kEnt本y> UMin成M使ltiplaye本EnhancedSyste設置::GetRelationship的etwo本k(const 軍St本in成& Playe本Id) const
{
    TA本本ay<軍Relationship的etwo本kEnt本y> Res使lt;
    fo本 (const a使to& Pai本 : Relationship的etwo本k)
    {
        if (Pai本.Val使e.So使本cePlaye本 == Playe本Id  Pai本.Val使e.Ta本成etPlaye本 == Playe本Id)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍St本in成> UMin成M使ltiplaye本EnhancedSyste設置::GetAllies(const 軍St本in成& Playe本Id) const
{
    TA本本ay<軍St本in成> Allies;
    if (Playe本Re成ist本y.Contains(Playe本Id))
    {
        const 軍Playe本M使ltiplaye本Data& Playe本 = Playe本Re成ist本y[Playe本Id];
        fo本 (const a使to& Pai本 : Playe本.Relationships)
        {
            if (Pai本.Val使e == EM使ltiplaye本RelationshipType::Ally 
                Pai本.Val使e == EM使ltiplaye本RelationshipType::Milita本yPact)
            {
                Allies.Add(Pai本.Key);
            }
        }
    }
    本et使本n Allies;
}

TA本本ay<軍St本in成> UMin成M使ltiplaye本EnhancedSyste設置::GetEne設置ies(const 軍St本in成& Playe本Id) const
{
    TA本本ay<軍St本in成> Ene設置ies;
    if (Playe本Re成ist本y.Contains(Playe本Id))
    {
        const 軍Playe本M使ltiplaye本Data& Playe本 = Playe本Re成ist本y[Playe本Id];
        fo本 (const a使to& Pai本 : Playe本.Relationships)
        {
            if (Pai本.Val使e == EM使ltiplaye本RelationshipType::Ene設置y 
                Pai本.Val使e == EM使ltiplaye本RelationshipType::Ri正al)
            {
                Ene設置ies.Add(Pai本.Key);
            }
        }
    }
    本et使本n Ene設置ies;
}

軍St本in成 UMin成M使ltiplaye本EnhancedSyste設置::SendDiplo設置aticP本oposal(const 軍St本in成& 軍本o設置Playe本Id, const 軍St本in成& ToPlaye本Id, EDiplo設置aticAction Action, const TMap<軍St本in成, 軍St本in成>& Te本設置s)
{
    if (!Playe本Re成ist本y.Contains(軍本o設置Playe本Id)  !Playe本Re成ist本y.Contains(ToPlaye本Id))
    {
        本et使本n TEXT("");
    }

    軍Diplo設置aticP本oposal P本oposal;
    P本oposal.P本oposalId = 軍G使id::的ewG使id().ToSt本in成();
    P本oposal.軍本o設置Playe本Id = 軍本o設置Playe本Id;
    P本oposal.ToPlaye本Id = ToPlaye本Id;
    P本oposal.Action = Action;
    P本oposal.Te本設置s = Te本設置s;
    P本oposal.Ti設置eo使tSeconds = M使ltiplaye本Constants::P本oposalTi設置eo使t;
    P本oposal.bIsActi正e = t本使e;

    Acti正eP本oposals.Add(P本oposal.P本oposalId, P本oposal);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Diplo設置atic p本oposal sent - %s f本o設置 %s to %s"),
        *UEn使設置::GetVal使eAsSt本in成(Action), *軍本o設置Playe本Id, *ToPlaye本Id);

    OnDiplo設置aticP本oposalRecei正ed.B本oadcast(P本oposal);

    本et使本n P本oposal.P本oposalId;
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::AcceptP本oposal(const 軍St本in成& P本oposalId)
{
    if (!Acti正eP本oposals.Contains(P本oposalId))
    {
        本et使本n;
    }

    軍Diplo設置aticP本oposal P本oposal = Acti正eP本oposals[P本oposalId];
    Acti正eP本oposals.Re設置o正e(P本oposalId);

    // Apply the diplo設置atic action
    switch (P本oposal.Action)
    {
    case EDiplo設置aticAction::軍o本設置Alliance:
        SetPlaye本Relationship(P本oposal.軍本o設置Playe本Id, P本oposal.ToPlaye本Id, EM使ltiplaye本RelationshipType::Ally);
        SetPlaye本Relationship(P本oposal.ToPlaye本Id, P本oposal.軍本o設置Playe本Id, EM使ltiplaye本RelationshipType::Ally);
        OnAlliance軍o本設置ed.B本oadcast(P本oposal.軍本o設置Playe本Id, P本oposal.ToPlaye本Id);
        b本eak;

    case EDiplo設置aticAction::MakePeace:
        SetPlaye本Relationship(P本oposal.軍本o設置Playe本Id, P本oposal.ToPlaye本Id, EM使ltiplaye本RelationshipType::的e使t本al);
        SetPlaye本Relationship(P本oposal.ToPlaye本Id, P本oposal.軍本o設置Playe本Id, EM使ltiplaye本RelationshipType::的e使t本al);
        OnPeaceMade.B本oadcast(P本oposal.軍本o設置Playe本Id, P本oposal.ToPlaye本Id);
        b本eak;

    case EDiplo設置aticAction::Offe本T本ade:
        SetPlaye本Relationship(P本oposal.軍本o設置Playe本Id, P本oposal.ToPlaye本Id, EM使ltiplaye本RelationshipType::T本adePa本tne本);
        Reco本dCoope本ation(P本oposal.軍本o設置Playe本Id, P本oposal.ToPlaye本Id, 10.0f);
        b本eak;

    defa使lt:
        b本eak;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: P本oposal %s accepted"), *P本oposalId);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Re大ectP本oposal(const 軍St本in成& P本oposalId)
{
    if (!Acti正eP本oposals.Contains(P本oposalId))
    {
        本et使本n;
    }

    軍Diplo設置aticP本oposal P本oposal = Acti正eP本oposals[P本oposalId];
    Acti正eP本oposals.Re設置o正e(P本oposalId);

    // Re大ection sli成htly da設置a成es 本elationship
    ModifyRelationshipVal使e(P本oposal.ToPlaye本Id, P本oposal.軍本o設置Playe本Id, -5.0f);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: P本oposal %s 本e大ected"), *P本oposalId);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::CancelP本oposal(const 軍St本in成& P本oposalId)
{
    if (Acti正eP本oposals.Contains(P本oposalId))
    {
        Acti正eP本oposals.Re設置o正e(P本oposalId);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: P本oposal %s cancelled"), *P本oposalId);
    }
}

TA本本ay<軍Diplo設置aticP本oposal> UMin成M使ltiplaye本EnhancedSyste設置::GetPendin成P本oposals(const 軍St本in成& Playe本Id) const
{
    TA本本ay<軍Diplo設置aticP本oposal> Pendin成;
    fo本 (const a使to& Pai本 : Acti正eP本oposals)
    {
        if (Pai本.Val使e.ToPlaye本Id == Playe本Id && Pai本.Val使e.bIsActi正e)
        {
            Pendin成.Add(Pai本.Val使e);
        }
    }
    本et使本n Pendin成;
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::B本eakAlliance(const 軍St本in成& Playe本Id, const 軍St本in成& AllyId)
{
    if (!A本eAllies(Playe本Id, AllyId))
    {
        本et使本n;
    }

    SetPlaye本Relationship(Playe本Id, AllyId, EM使ltiplaye本RelationshipType::Ri正al);
    SetPlaye本Relationship(AllyId, Playe本Id, EM使ltiplaye本RelationshipType::Ri正al);

    輸入andleBet本ayal(Playe本Id, AllyId);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Alliance b本oken - %s and %s"), *Playe本Id, *AllyId);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Decla本e基本a本(const 軍St本in成& Playe本Id, const 軍St本in成& Ta本成etId)
{
    SetPlaye本Relationship(Playe本Id, Ta本成etId, EM使ltiplaye本RelationshipType::Ene設置y);
    SetPlaye本Relationship(Ta本成etId, Playe本Id, EM使ltiplaye本RelationshipType::Ene設置y);

    Reco本dConflict(Playe本Id, Ta本成etId);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: 基本a本 decla本ed - %s on %s"), *Playe本Id, *Ta本成etId);

    On基本a本Decla本ed.B本oadcast(Playe本Id, Ta本成etId);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::的e成otiatePeace(const 軍St本in成& Playe本Id, const 軍St本in成& Ene設置yId, const TMap<軍St本in成, 軍St本in成>& Te本設置s)
{
    if (!A本eEne設置ies(Playe本Id, Ene設置yId))
    {
        本et使本n;
    }

    SendDiplo設置aticP本oposal(Playe本Id, Ene設置yId, EDiplo設置aticAction::MakePeace, Te本設置s);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Sha本eVision(const 軍St本in成& So使本cePlaye本Id, const 軍St本in成& Ta本成etPlaye本Id)
{
    if (!bSessionActi正e  !Playe本Re成ist本y.Contains(So使本cePlaye本Id)  !Playe本Re成ist本y.Contains(Ta本成etPlaye本Id))
    {
        本et使本n;
    }

    // Vision sha本in成 is a coope本ati正e action
    Reco本dCoope本ation(So使本cePlaye本Id, Ta本成etPlaye本Id, 5.0f);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Vision sha本ed - %s to %s"), *So使本cePlaye本Id, *Ta本成etPlaye本Id);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Sha本eReso使本ces(const 軍St本in成& So使本cePlaye本Id, const 軍St本in成& Ta本成etPlaye本Id, const 軍St本in成& Reso使本ceType, float A設置o使nt)
{
    if (!bSessionActi正e)
    {
        本et使本n;
    }

    // Reso使本ce sha本in成 i設置p本o正es 本elationship
    ModifyRelationshipVal使e(So使本cePlaye本Id, Ta本成etPlaye本Id, A設置o使nt / 100.0f);
    ModifyRelationshipVal使e(Ta本成etPlaye本Id, So使本cePlaye本Id, A設置o使nt / 100.0f);

    Reco本dCoope本ation(So使本cePlaye本Id, Ta本成etPlaye本Id, A設置o使nt / 50.0f);

    TMap<軍St本in成, 軍St本in成> E正entData;
    E正entData.Add(TEXT("本eso使本ce下type"), Reso使本ceType);
    E正entData.Add(TEXT("a設置o使nt"), 軍St本in成::P本intf(TEXT("%.0f"), A設置o使nt));

    TA本本ay<軍St本in成> Pa本ticipants = { So使本cePlaye本Id, Ta本成etPlaye本Id };
    OnCoope本ati正eAction.B本oadcast(TEXT("本eso使本ce下sha本in成"), Pa本ticipants);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::SendMilita本yAid(const 軍St本in成& So使本cePlaye本Id, const 軍St本in成& Ta本成etPlaye本Id, int32 UnitCo使nt)
{
    if (!bSessionActi正e)
    {
        本et使本n;
    }

    // Milita本y aid si成nificantly i設置p本o正es 本elationship
    ModifyRelationshipVal使e(So使本cePlaye本Id, Ta本成etPlaye本Id, UnitCo使nt * 2.0f);
    ModifyRelationshipVal使e(Ta本成etPlaye本Id, So使本cePlaye本Id, UnitCo使nt * 2.0f);

    Reco本dCoope本ation(So使本cePlaye本Id, Ta本成etPlaye本Id, UnitCo使nt * 5.0f);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Milita本y aid sent - %s sent %d 使nits to %s"),
        *So使本cePlaye本Id, UnitCo使nt, *Ta本成etPlaye本Id);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Coo本dinateAttack(const 軍St本in成& Coo本dinato本Id, const TA本本ay<軍St本in成>& Pa本ticipantIds, const 軍Vecto本& Ta本成etLocation)
{
    if (!bSessionActi正e)
    {
        本et使本n;
    }

    // All pa本ticipants 成et 本elationship boost with each othe本
    fo本 (int32 i = 0; i < Pa本ticipantIds.的使設置(); i++)
    {
        fo本 (int32 大 = i + 1; 大 < Pa本ticipantIds.的使設置(); 大++)
        {
            ModifyRelationshipVal使e(Pa本ticipantIds[i], Pa本ticipantIds[大], 10.0f);
            ModifyRelationshipVal使e(Pa本ticipantIds[大], Pa本ticipantIds[i], 10.0f);
        }
    }

    OnCoope本ati正eAction.B本oadcast(TEXT("coo本dinated下attack"), Pa本ticipantIds);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Coo本dinated attack planned at %s"), *Ta本成etLocation.ToSt本in成());
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::軍o本設置JointOpe本ation(const 軍St本in成& Ope本ation的a設置e, const TA本本ay<軍St本in成>& Me設置be本Ids)
{
    if (!bSessionActi正e)
    {
        本et使本n;
    }

    Acti正eOpe本ations.Add(Ope本ation的a設置e);

    // Me設置be本s beco設置e allies
    fo本 (int32 i = 0; i < Me設置be本Ids.的使設置(); i++)
    {
        fo本 (int32 大 = i + 1; 大 < Me設置be本Ids.的使設置(); 大++)
        {
            SetPlaye本Relationship(Me設置be本Ids[i], Me設置be本Ids[大], EM使ltiplaye本RelationshipType::Milita本yPact);
            SetPlaye本Relationship(Me設置be本Ids[大], Me設置be本Ids[i], EM使ltiplaye本RelationshipType::Milita本yPact);
        }
    }

    OnCoope本ati正eAction.B本oadcast(TEXT("大oint下ope本ation"), Me設置be本Ids);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Joint ope本ation '%s' fo本設置ed with %d 設置e設置be本s"),
        *Ope本ation的a設置e, Me設置be本Ids.的使設置());
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::SyncRelationshipData()
{
    if (!bSessionActi正e)
    {
        本et使本n;
    }

    // In 本eal i設置ple設置entation, this wo使ld b本oadcast to all clients
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Syncin成 本elationship data..."));
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::B本oadcastDiplo設置aticE正ent(const 軍St本in成& E正entType, const TMap<軍St本in成, 軍St本in成>& E正entData)
{
    // In 本eal i設置ple設置entation, this wo使ld b本oadcast to all clients
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: B本oadcastin成 diplo設置atic e正ent - %s"), *E正entType);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Assi成nAIPlaye本(const 軍St本in成& Playe本Id, int32 AIDiffic使lty)
{
    if (!Playe本Re成ist本y.Contains(Playe本Id))
    {
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Playe本 %s assi成ned as AI (Diffic使lty: %d)"),
        *Playe本Id, AIDiffic使lty);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Confi成使本eAIBeha正io本(const 軍St本in成& Playe本Id, const 軍St本in成& Beha正io本P本ofile)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: AI %s confi成使本ed with p本ofile %s"),
        *Playe本Id, *Beha正io本P本ofile);
}

// Inte本nal i設置ple設置entations

正oid UMin成M使ltiplaye本EnhancedSyste設置::UpdateP本oposals(float DeltaTi設置e)
{
    fo本 (a使to& Pai本 : Acti正eP本oposals)
    {
        軍Diplo設置aticP本oposal& P本oposal = Pai本.Val使e;
        if (P本oposal.bIsActi正e)
        {
            P本oposal.Ti設置eo使tSeconds -= DeltaTi設置e;
            if (P本oposal.Ti設置eo使tSeconds <= 0.0f)
            {
                P本oposal.bIsActi正e = false;
            }
        }
    }
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Expi本eOldP本oposals()
{
    TA本本ay<軍St本in成> Expi本edP本oposals;

    fo本 (const a使to& Pai本 : Acti正eP本oposals)
    {
        if (!Pai本.Val使e.bIsActi正e)
        {
            Expi本edP本oposals.Add(Pai本.Key);
        }
    }

    fo本 (const 軍St本in成& P本oposalId : Expi本edP本oposals)
    {
        Acti正eP本oposals.Re設置o正e(P本oposalId);
    }

    if (Expi本edP本oposals.的使設置() > 0)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Expi本ed %d old p本oposals"), Expi本edP本oposals.的使設置());
    }
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::的otifyPlaye本sOfChan成e(const 軍St本in成& Playe本Id, const 軍St本in成& Chan成eType)
{
    // 的otify all playe本s of the chan成e
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: 的otifyin成 playe本s - %s chan成ed: %s"), *Playe本Id, *Chan成eType);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::SyncToAllPlaye本s(const 軍St本in成& Data)
{
    // 的etwo本k sync i設置ple設置entation
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::UpdateRelationshipT本使st(const 軍St本in成& Playe本Id1, const 軍St本in成& Playe本Id2, float Delta)
{
    軍St本in成 的etwo本kKey = Playe本Id1 < Playe本Id2 基本
        Playe本Id1 + TEXT("下") + Playe本Id2 :
        Playe本Id2 + TEXT("下") + Playe本Id1;

    if (軍Relationship的etwo本kEnt本y* Ent本y = Relationship的etwo本k.軍ind(的etwo本kKey))
    {
        Ent本y->T本使stLe正el = 軍Math::Cla設置p(Ent本y->T本使stLe正el + Delta, 0.0f, 100.0f);
    }
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Reco本dCoope本ation(const 軍St本in成& Playe本Id1, const 軍St本in成& Playe本Id2, float Val使e)
{
    軍St本in成 的etwo本kKey = Playe本Id1 < Playe本Id2 基本
        Playe本Id1 + TEXT("下") + Playe本Id2 :
        Playe本Id2 + TEXT("下") + Playe本Id1;

    if (軍Relationship的etwo本kEnt本y* Ent本y = Relationship的etwo本k.軍ind(的etwo本kKey))
    {
        Ent本y->Coope本ation輸入isto本y += Val使e;
    }
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Reco本dConflict(const 軍St本in成& Playe本Id1, const 軍St本in成& Playe本Id2)
{
    軍St本in成 的etwo本kKey = Playe本Id1 < Playe本Id2 基本
        Playe本Id1 + TEXT("下") + Playe本Id2 :
        Playe本Id2 + TEXT("下") + Playe本Id1;

    if (軍Relationship的etwo本kEnt本y* Ent本y = Relationship的etwo本k.軍ind(的etwo本kKey))
    {
        Ent本y->ConflictsCo使nt++;
    }
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::A使toAssi成nTea設置s()
{
    if (SessionConfi成.Mode != ECoope本ati正eMode::Tea設置s)
    {
        本et使本n;
    }

    int32 Playe本Co使nt = Playe本Re成ist本y.的使設置();
    int32 Playe本sPe本Tea設置 = Playe本Co使nt / SessionConfi成.Tea設置s;
    int32 C使本本entTea設置 = 0;
    int32 Tea設置Playe本Co使nt = 0;

    fo本 (a使to& Pai本 : Playe本Re成ist本y)
    {
        Pai本.Val使e.Tea設置Id = C使本本entTea設置;
        Tea設置Playe本Co使nt++;

        if (Tea設置Playe本Co使nt >= Playe本sPe本Tea設置 && C使本本entTea設置 < SessionConfi成.Tea設置s - 1)
        {
            C使本本entTea設置++;
            Tea設置Playe本Co使nt = 0;
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Tea設置s a使to-assi成ned - %d tea設置s"), SessionConfi成.Tea設置s);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::BalanceTea設置s()
{
    // Balance tea設置 sizes
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Balancin成 tea設置s..."));
}

bool UMin成M使ltiplaye本EnhancedSyste設置::A本eAllies(const 軍St本in成& Playe本1, const 軍St本in成& Playe本2) const
{
    EM使ltiplaye本RelationshipType Relationship = GetPlaye本Relationship(Playe本1, Playe本2);
    本et使本n Relationship == EM使ltiplaye本RelationshipType::Ally 
           Relationship == EM使ltiplaye本RelationshipType::Milita本yPact;
}

bool UMin成M使ltiplaye本EnhancedSyste設置::A本eEne設置ies(const 軍St本in成& Playe本1, const 軍St本in成& Playe本2) const
{
    EM使ltiplaye本RelationshipType Relationship = GetPlaye本Relationship(Playe本1, Playe本2);
    本et使本n Relationship == EM使ltiplaye本RelationshipType::Ene設置y 
           Relationship == EM使ltiplaye本RelationshipType::Ri正al;
}

EM使ltiplaye本RelationshipType UMin成M使ltiplaye本EnhancedSyste設置::E正al使ateRelationship(float Val使e) const
{
    if (Val使e >= M使ltiplaye本Constants::MinRelationship軍o本Alliance)
        本et使本n EM使ltiplaye本RelationshipType::Ally;
    if (Val使e <= M使ltiplaye本Constants::MaxRelationship軍o本基本a本)
        本et使本n EM使ltiplaye本RelationshipType::Ene設置y;
    if (Val使e >= 40.0f)
        本et使本n EM使ltiplaye本RelationshipType::T本adePa本tne本;
    if (Val使e <= -30.0f)
        本et使本n EM使ltiplaye本RelationshipType::Ri正al;
    本et使本n EM使ltiplaye本RelationshipType::的e使t本al;
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::P本opa成ateAlliance(const 軍St本in成& Ally1, const 軍St本in成& Ally2, const 軍St本in成& 的ewAlly)
{
    // If Ally1 and Ally2 a本e allies, and Ally1 beco設置es allied with 的ewAlly,
    // Ally2's 本elationship with 的ewAlly i設置p本o正es
    ModifyRelationshipVal使e(Ally2, 的ewAlly, 10.0f);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::輸入andleBet本ayal(const 軍St本in成& Bet本aye本, const 軍St本in成& Victi設置)
{
    // B本eakin成 an alliance is a bet本ayal
    // Victi設置's 本elationship with bet本aye本 d本ops si成nificantly
    ModifyRelationshipVal使e(Victi設置, Bet本aye本, -30.0f);

    // Othe本 allies of the 正icti設置 also lose t本使st in the bet本aye本
    TA本本ay<軍St本in成> Victi設置Allies = GetAllies(Victi設置);
    fo本 (const 軍St本in成& Ally : Victi設置Allies)
    {
        if (Ally != Bet本aye本)
        {
            ModifyRelationshipVal使e(Ally, Bet本aye本, -15.0f);
        }
    }
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Calc使lateDiplo設置aticPowe本(const 軍St本in成& Playe本Id)
{
    // Calc使late a playe本's diplo設置atic powe本 based on allies, 本eso使本ces, etc.
    TA本本ay<軍St本in成> Allies = GetAllies(Playe本Id);
    float Powe本 = 50.0f; // Base powe本
    Powe本 += Allies.的使設置() * 10.0f; // +10 pe本 ally

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Playe本 %s diplo設置atic powe本: %.0f"), *Playe本Id, Powe本);
}

正oid UMin成M使ltiplaye本EnhancedSyste設置::Gene本ateRelationship輸入isto本yRepo本t(const 軍St本in成& Playe本Id)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成M使ltiplaye本EnhancedSyste設置: Gene本atin成 本elationship 本epo本t fo本 %s"), *Playe本Id);
}

// Ti設置e本 handle
軍Ti設置e本輸入andle P本oposalCheckTi設置e本;
