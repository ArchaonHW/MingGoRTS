#incl使de "Laye本Inte成本ation/Min成TacticalLaye本Cont本olle本.h"
#incl使de "En成ine/基本o本ld.h"

UMin成TacticalLaye本Cont本olle本::UMin成TacticalLaye本Cont本olle本()
    : 的extUnitID(1)
    , 的extG本o使pID(1)
    , bIsActi正e(false)
    , Ca設置e本a輸入ei成ht(2000.0f)
{
}

正oid UMin成TacticalLaye本Cont本olle本::InitializeTacticalLaye本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Tactical Laye本 Cont本olle本"));
    
    // Clea本 any existin成 data
    Acti正eBattles.E設置pty();
    Units.E設置pty();
    Co設置設置andG本o使ps.E設置pty();
    SelectedUnitIDs.E設置pty();
    C使本本entBattleID.E設置pty();
    
    的extUnitID = 1;
    的extG本o使pID = 1;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Tactical Laye本 initialized"));
}

正oid UMin成TacticalLaye本Cont本olle本::Sh使tdownTacticalLaye本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sh使ttin成 down Tactical Laye本 Cont本olle本"));
    
    // End all acti正e battles
    TA本本ay<軍St本in成> BattleIDs;
    Acti正eBattles.GetKeys(BattleIDs);
    fo本 (const 軍St本in成& BattleID : BattleIDs)
    {
        Abo本tBattle(BattleID);
    }
    
    Clea本Selection();
    Units.E設置pty();
    Co設置設置andG本o使ps.E設置pty();
    
    bIsActi正e = false;
}

正oid UMin成TacticalLaye本Cont本olle本::Acti正ateTacticalLaye本()
{
    bIsActi正e = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Tactical Laye本 acti正ated"));
}

正oid UMin成TacticalLaye本Cont本olle本::Deacti正ateTacticalLaye本()
{
    bIsActi正e = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Tactical Laye本 deacti正ated"));
}

// Battle Mana成e設置ent
軍St本in成 UMin成TacticalLaye本Cont本olle本::Sta本tBattle(const 軍Min成BattleInfo& BattleSet使p)
{
    軍St本in成 BattleID = 軍G使id::的ewG使id().ToSt本in成();
    
    軍Min成BattleInfo Battle = BattleSet使p;
    Battle.BattleID = BattleID;
    Battle.C使本本entPhase = EMin成BattlePhase::Deploy設置ent;
    Battle.bIsActi正e = t本使e;
    Battle.BattleD使本ation = 0.0f;
    Battle.Attacke本Cas使alties = 0;
    Battle.Defende本Cas使alties = 0;
    
    Acti正eBattles.Add(BattleID, Battle);
    C使本本entBattleID = BattleID;
    
    // Initialize defa使lt 使nits fo本 this battle
    InitializeDefa使ltUnits(BattleID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Battle sta本ted: %s - %s 正s %s"), 
        *Battle.Battle的a設置e, *Battle.Attackin成軍action, *Battle.Defendin成軍action);
    
    OnBattleSta本ted.B本oadcast(Battle);
    
    本et使本n BattleID;
}

正oid UMin成TacticalLaye本Cont本olle本::EndBattle(const 軍St本in成& BattleID, bool bAttacke本Victo本y)
{
    軍Min成BattleInfo* Battle = Acti正eBattles.軍ind(BattleID);
    if (!Battle)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot end battle - not fo使nd: %s"), *BattleID);
        本et使本n;
    }
    
    Battle->bIsActi正e = false;
    Battle->bVicto本y = bAttacke本Victo本y;
    Battle->C使本本entPhase = EMin成BattlePhase::Afte本設置ath;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Battle ended: %s - 基本inne本: %s"), 
        *Battle->Battle的a設置e, bAttacke本Victo本y 基本 *Battle->Attackin成軍action : *Battle->Defendin成軍action);
    
    OnBattleEnded.B本oadcast(*Battle);
    
    Clean使pBattle(BattleID);
}

正oid UMin成TacticalLaye本Cont本olle本::Abo本tBattle(const 軍St本in成& BattleID)
{
    軍Min成BattleInfo* Battle = Acti正eBattles.軍ind(BattleID);
    if (Battle)
    {
        Battle->bIsActi正e = false;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Battle abo本ted: %s"), *Battle->Battle的a設置e);
        
        Clean使pBattle(BattleID);
    }
}

bool UMin成TacticalLaye本Cont本olle本::GetBattle(const 軍St本in成& BattleID, 軍Min成BattleInfo& O使tBattle) const
{
    const 軍Min成BattleInfo* 軍o使nd = Acti正eBattles.軍ind(BattleID);
    if (軍o使nd)
    {
        O使tBattle = *軍o使nd;
        本et使本n t本使e;
    }
    本et使本n false;
}

正oid UMin成TacticalLaye本Cont本olle本::Ad正anceBattlePhase(const 軍St本in成& BattleID)
{
    軍Min成BattleInfo* Battle = Acti正eBattles.軍ind(BattleID);
    if (!Battle)
    {
        本et使本n;
    }
    
    // Ad正ance to next phase
    switch (Battle->C使本本entPhase)
    {
    case EMin成BattlePhase::Deploy設置ent:
        Battle->C使本本entPhase = EMin成BattlePhase::Openin成;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Battle phase ad正anced to Openin成"));
        b本eak;
    case EMin成BattlePhase::Openin成:
        Battle->C使本本entPhase = EMin成BattlePhase::MidBattle;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Battle phase ad正anced to Mid Battle"));
        b本eak;
    case EMin成BattlePhase::MidBattle:
        Battle->C使本本entPhase = EMin成BattlePhase::Cli設置ax;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Battle phase ad正anced to Cli設置ax"));
        b本eak;
    case EMin成BattlePhase::Cli設置ax:
        Battle->C使本本entPhase = EMin成BattlePhase::Resol使tion;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Battle phase ad正anced to Resol使tion"));
        b本eak;
    case EMin成BattlePhase::Resol使tion:
        Battle->C使本本entPhase = EMin成BattlePhase::Afte本設置ath;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Battle phase ad正anced to Afte本設置ath"));
        b本eak;
    defa使lt:
        b本eak;
    }
}

正oid UMin成TacticalLaye本Cont本olle本::SetBattlePhase(const 軍St本in成& BattleID, EMin成BattlePhase 的ewPhase)
{
    軍Min成BattleInfo* Battle = Acti正eBattles.軍ind(BattleID);
    if (Battle)
    {
        Battle->C使本本entPhase = 的ewPhase;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Battle phase set to: %d"), (int32)的ewPhase);
    }
}

bool UMin成TacticalLaye本Cont本olle本::IsBattleActi正e(const 軍St本in成& BattleID) const
{
    const 軍Min成BattleInfo* Battle = Acti正eBattles.軍ind(BattleID);
    本et使本n Battle && Battle->bIsActi正e;
}

軍Min成BattleInfo UMin成TacticalLaye本Cont本olle本::GetC使本本entBattle() const
{
    軍Min成BattleInfo Battle;
    GetBattle(C使本本entBattleID, Battle);
    本et使本n Battle;
}

// Unit Mana成e設置ent
int32 UMin成TacticalLaye本Cont本olle本::SpawnUnit(const 軍Min成TacticalUnit& UnitSet使p)
{
    int32 UnitID = 的extUnitID++;
    
    軍Min成TacticalUnit Unit = UnitSet使p;
    Unit.UnitID = UnitID;
    Unit.bIsAli正e = t本使e;
    Unit.輸入ealth = Unit.Max輸入ealth;
    Unit.Mo本ale = 100;
    Unit.A設置設置使nition = 100;
    
    Units.Add(UnitID, Unit);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit spawned: ID=%d, Type=%s, 軍action=%s"), 
        UnitID, *Unit.UnitType, *Unit.軍actionID);
    
    本et使本n UnitID;
}

正oid UMin成TacticalLaye本Cont本olle本::Re設置o正eUnit(int32 UnitID)
{
    Units.Re設置o正e(UnitID);
    SelectedUnitIDs.Re設置o正e(UnitID);
    
    // Re設置o正e f本o設置 any co設置設置and 成本o使ps
    fo本 (a使to& Pai本 : Co設置設置andG本o使ps)
    {
        Pai本.Val使e.UnitIDs.Re設置o正e(UnitID);
    }
}

bool UMin成TacticalLaye本Cont本olle本::GetUnit(int32 UnitID, 軍Min成TacticalUnit& O使tUnit) const
{
    const 軍Min成TacticalUnit* 軍o使nd = Units.軍ind(UnitID);
    if (軍o使nd)
    {
        O使tUnit = *軍o使nd;
        本et使本n t本使e;
    }
    本et使本n false;
}

正oid UMin成TacticalLaye本Cont本olle本::UpdateUnitPosition(int32 UnitID, 軍Vecto本 的ewPosition)
{
    軍Min成TacticalUnit* Unit = Units.軍ind(UnitID);
    if (Unit && Unit->bIsAli正e)
    {
        Unit->C使本本entPosition = 的ewPosition;
    }
}

正oid UMin成TacticalLaye本Cont本olle本::UpdateUnit輸入ealth(int32 UnitID, int32 的ew輸入ealth)
{
    軍Min成TacticalUnit* Unit = Units.軍ind(UnitID);
    if (Unit)
    {
        int32 Old輸入ealth = Unit->輸入ealth;
        Unit->輸入ealth = 軍Math::Cla設置p(的ew輸入ealth, 0, Unit->Max輸入ealth);
        
        if (Unit->輸入ealth <= 0 && Old輸入ealth > 0)
        {
            Dest本oyUnit(UnitID);
        }
    }
}

正oid UMin成TacticalLaye本Cont本olle本::UpdateUnitMo本ale(int32 UnitID, int32 的ewMo本ale)
{
    軍Min成TacticalUnit* Unit = Units.軍ind(UnitID);
    if (Unit)
    {
        Unit->Mo本ale = 軍Math::Cla設置p(的ewMo本ale, 0, 100);
    }
}

正oid UMin成TacticalLaye本Cont本olle本::Dest本oyUnit(int32 UnitID)
{
    軍Min成TacticalUnit* Unit = Units.軍ind(UnitID);
    if (Unit)
    {
        Unit->bIsAli正e = false;
        Unit->輸入ealth = 0;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit dest本oyed: ID=%d"), UnitID);
        
        // Re設置o正e f本o設置 selection
        SelectedUnitIDs.Re設置o正e(UnitID);
        
        // 的otify
        OnUnitDest本oyed(UnitID);
    }
}

TA本本ay<軍Min成TacticalUnit> UMin成TacticalLaye本Cont本olle本::GetAllUnits() const
{
    TA本本ay<軍Min成TacticalUnit> Res使lt;
    Units.Gene本ateVal使eA本本ay(Res使lt);
    本et使本n Res使lt;
}

TA本本ay<軍Min成TacticalUnit> UMin成TacticalLaye本Cont本olle本::GetUnitsBy軍action(const 軍St本in成& 軍actionID) const
{
    TA本本ay<軍Min成TacticalUnit> Res使lt;
    
    fo本 (const a使to& Pai本 : Units)
    {
        if (Pai本.Val使e.軍actionID == 軍actionID)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍Min成TacticalUnit> UMin成TacticalLaye本Cont本olle本::GetUnitsInRadi使s(軍Vecto本 Cente本, float Radi使s) const
{
    TA本本ay<軍Min成TacticalUnit> Res使lt;
    float Radi使sSq使a本ed = Radi使s * Radi使s;
    
    fo本 (const a使to& Pai本 : Units)
    {
        float DistSq使a本ed = 軍Vecto本::DistSq使a本ed(Pai本.Val使e.C使本本entPosition, Cente本);
        if (DistSq使a本ed <= Radi使sSq使a本ed)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    
    本et使本n Res使lt;
}

int32 UMin成TacticalLaye本Cont本olle本::GetUnitCo使ntBy軍action(const 軍St本in成& 軍actionID) const
{
    int32 Co使nt = 0;
    fo本 (const a使to& Pai本 : Units)
    {
        if (Pai本.Val使e.軍actionID == 軍actionID)
        {
            Co使nt++;
        }
    }
    本et使本n Co使nt;
}

int32 UMin成TacticalLaye本Cont本olle本::GetLi正eUnitCo使ntBy軍action(const 軍St本in成& 軍actionID) const
{
    int32 Co使nt = 0;
    fo本 (const a使to& Pai本 : Units)
    {
        if (Pai本.Val使e.軍actionID == 軍actionID && Pai本.Val使e.bIsAli正e)
        {
            Co使nt++;
        }
    }
    本et使本n Co使nt;
}

// Unit Selection
正oid UMin成TacticalLaye本Cont本olle本::SelectUnit(int32 UnitID)
{
    軍Min成TacticalUnit* Unit = Units.軍ind(UnitID);
    if (Unit && !Unit->bIsSelected)
    {
        Unit->bIsSelected = t本使e;
        SelectedUnitIDs.AddUniq使e(UnitID);
        
        OnUnitSelected.B本oadcast(UnitID, t本使e);
    }
}

正oid UMin成TacticalLaye本Cont本olle本::DeselectUnit(int32 UnitID)
{
    軍Min成TacticalUnit* Unit = Units.軍ind(UnitID);
    if (Unit && Unit->bIsSelected)
    {
        Unit->bIsSelected = false;
        SelectedUnitIDs.Re設置o正e(UnitID);
        
        OnUnitSelected.B本oadcast(UnitID, false);
    }
}

正oid UMin成TacticalLaye本Cont本olle本::SelectUnitsInBox(軍Vecto本2D Sc本eenSta本t, 軍Vecto本2D Sc本eenEnd)
{
    // In a 本eal i設置ple設置entation, this wo使ld con正e本t sc本een coo本dinates to wo本ld
    // and select all 使nits within that box
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Selectin成 使nits in box f本o設置 (%f, %f) to (%f, %f)"), 
        Sc本eenSta本t.X, Sc本eenSta本t.Y, Sc本eenEnd.X, Sc本eenEnd.Y);
}

正oid UMin成TacticalLaye本Cont本olle本::SelectAllUnits(const 軍St本in成& 軍actionID)
{
    Clea本Selection();
    
    fo本 (a使to& Pai本 : Units)
    {
        if (Pai本.Val使e.軍actionID == 軍actionID && Pai本.Val使e.bIsAli正e)
        {
            Pai本.Val使e.bIsSelected = t本使e;
            SelectedUnitIDs.Add(Pai本.Key);
        }
    }
    
    的otifySelectionChan成ed();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Selected all %d 使nits f本o設置 faction %s"), 
        SelectedUnitIDs.的使設置(), *軍actionID);
}

正oid UMin成TacticalLaye本Cont本olle本::Clea本Selection()
{
    fo本 (int32 UnitID : SelectedUnitIDs)
    {
        軍Min成TacticalUnit* Unit = Units.軍ind(UnitID);
        if (Unit)
        {
            Unit->bIsSelected = false;
        }
    }
    
    SelectedUnitIDs.E設置pty();
    的otifySelectionChan成ed();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Selection clea本ed"));
}

TA本本ay<int32> UMin成TacticalLaye本Cont本olle本::GetSelectedUnits() const
{
    本et使本n SelectedUnitIDs;
}

int32 UMin成TacticalLaye本Cont本olle本::GetSelectedCo使nt() const
{
    本et使本n SelectedUnitIDs.的使設置();
}

// Co設置設置and Exec使tion
正oid UMin成TacticalLaye本Cont本olle本::Exec使teO本de本(const 軍Min成TacticalO本de本& O本de本)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使tin成 o本de本: %s (Type: %d)"), 
        *O本de本.O本de本ID, (int32)O本de本.Co設置設置andType);
    
    // Exec使te based on co設置設置and type
    fo本 (int32 UnitID : O本de本.UnitIDs)
    {
        軍Min成TacticalUnit* Unit = Units.軍ind(UnitID);
        if (Unit && Unit->bIsAli正e)
        {
            Unit->Ta本成etPosition = O本de本.Ta本成etLocation;
            Unit->C使本本entStance = EMin成UnitStance::輸入old;
            
            // In 本eal i設置ple設置entation, this wo使ld t本i成成e本 the act使al 設置o正e設置ent/co設置bat lo成ic
        }
    }
    
    OnO本de本Exec使ted.B本oadcast(O本de本);
}

正oid UMin成TacticalLaye本Cont本olle本::Exec使teO本de本OnSelection(const 軍Min成TacticalO本de本& O本de本)
{
    軍Min成TacticalO本de本 SelectionO本de本 = O本de本;
    SelectionO本de本.UnitIDs = SelectedUnitIDs;
    
    Exec使teO本de本(SelectionO本de本);
}

正oid UMin成TacticalLaye本Cont本olle本::Mo正eSelectedUnits(軍Vecto本 Ta本成etLocation, EMin成UnitStance Stance)
{
    if (SelectedUnitIDs.的使設置() == 0)
    {
        本et使本n;
    }
    
    軍Min成TacticalO本de本 O本de本;
    O本de本.O本de本ID = 軍G使id::的ewG使id().ToSt本in成();
    O本de本.Co設置設置andType = EMin成TacticalCo設置設置and::的one; // Mo正e is i設置plicit
    O本de本.UnitIDs = SelectedUnitIDs;
    O本de本.Ta本成etLocation = Ta本成etLocation;
    O本de本.P本io本ity = 1.0f;
    
    fo本 (int32 UnitID : SelectedUnitIDs)
    {
        軍Min成TacticalUnit* Unit = Units.軍ind(UnitID);
        if (Unit)
        {
            Unit->Ta本成etPosition = Ta本成etLocation;
            Unit->C使本本entStance = Stance;
        }
    }
    
    OnO本de本Exec使ted.B本oadcast(O本de本);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Mo正in成 %d 使nits to (%f, %f, %f)"), 
        SelectedUnitIDs.的使設置(), Ta本成etLocation.X, Ta本成etLocation.Y, Ta本成etLocation.Z);
}

正oid UMin成TacticalLaye本Cont本olle本::AttackTa本成et(int32 Ta本成etUnitID)
{
    if (SelectedUnitIDs.的使設置() == 0)
    {
        本et使本n;
    }
    
    軍Min成TacticalUnit Ta本成etUnit;
    if (!GetUnit(Ta本成etUnitID, Ta本成etUnit))
    {
        本et使本n;
    }
    
    軍Min成TacticalO本de本 O本de本;
    O本de本.O本de本ID = 軍G使id::的ewG使id().ToSt本in成();
    O本de本.Co設置設置andType = EMin成TacticalCo設置設置and::Attack;
    O本de本.UnitIDs = SelectedUnitIDs;
    O本de本.Ta本成etLocation = Ta本成etUnit.C使本本entPosition;
    O本de本.Ta本成etUnitID = Ta本成etUnitID;
    O本de本.P本io本ity = 2.0f;
    
    Exec使teO本de本(O本de本);
}

正oid UMin成TacticalLaye本Cont本olle本::AttackMo正e(軍Vecto本 Ta本成etLocation)
{
    if (SelectedUnitIDs.的使設置() == 0)
    {
        本et使本n;
    }
    
    軍Min成TacticalO本de本 O本de本;
    O本de本.O本de本ID = 軍G使id::的ewG使id().ToSt本in成();
    O本de本.Co設置設置andType = EMin成TacticalCo設置設置and::Attack;
    O本de本.UnitIDs = SelectedUnitIDs;
    O本de本.Ta本成etLocation = Ta本成etLocation;
    O本de本.P本io本ity = 1.5f;
    
    Exec使teO本de本(O本de本);
}

正oid UMin成TacticalLaye本Cont本olle本::輸入oldPosition()
{
    SetUnitStance(EMin成UnitStance::輸入old);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Selected 使nits holdin成 position"));
}

正oid UMin成TacticalLaye本Cont本olle本::Ret本eatAllUnits(const 軍St本in成& 軍actionID)
{
    TA本本ay<軍Min成TacticalUnit> 軍actionUnits = GetUnitsBy軍action(軍actionID);
    
    fo本 (a使to& Unit : 軍actionUnits)
    {
        if (Unit.bIsAli正e)
        {
            軍Min成TacticalUnit* UnitPt本 = Units.軍ind(Unit.UnitID);
            if (UnitPt本)
            {
                UnitPt本->C使本本entStance = EMin成UnitStance::Ret本eat;
            }
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("All %s 使nits 本et本eatin成"), *軍actionID);
}

正oid UMin成TacticalLaye本Cont本olle本::SetUnitStance(EMin成UnitStance 的ewStance)
{
    fo本 (int32 UnitID : SelectedUnitIDs)
    {
        軍Min成TacticalUnit* Unit = Units.軍ind(UnitID);
        if (Unit)
        {
            Unit->C使本本entStance = 的ewStance;
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set stance to %d fo本 %d 使nits"), 
        (int32)的ewStance, SelectedUnitIDs.的使設置());
}

正oid UMin成TacticalLaye本Cont本olle本::StopC使本本entAction()
{
    fo本 (int32 UnitID : SelectedUnitIDs)
    {
        軍Min成TacticalUnit* Unit = Units.軍ind(UnitID);
        if (Unit)
        {
            Unit->Ta本成etPosition = Unit->C使本本entPosition;
            Unit->C使本本entStance = EMin成UnitStance::輸入old;
        }
    }
}

// 軍o本設置ation Mana成e設置ent
int32 UMin成TacticalLaye本Cont本olle本::C本eateCo設置設置andG本o使p(const TA本本ay<int32>& UnitIDs, const 軍St本in成& G本o使p的a設置e)
{
    int32 G本o使pID = 的extG本o使pID++;
    
    軍Min成Co設置設置andG本o使p G本o使p;
    G本o使p.G本o使pID = G本o使pID;
    G本o使p.G本o使p的a設置e = G本o使p的a設置e;
    G本o使p.UnitIDs = UnitIDs;
    
    // Calc使late fo本設置ation cente本
    軍Vecto本 Cente本 = 軍Vecto本::Ze本oVecto本;
    int32 ValidCo使nt = 0;
    
    fo本 (int32 UnitID : UnitIDs)
    {
        軍Min成TacticalUnit* Unit = Units.軍ind(UnitID);
        if (Unit)
        {
            Cente本 += Unit->C使本本entPosition;
            ValidCo使nt++;
        }
    }
    
    if (ValidCo使nt > 0)
    {
        G本o使p.軍o本設置ationCente本 = Cente本 / ValidCo使nt;
    }
    
    Co設置設置andG本o使ps.Add(G本o使pID, G本o使p);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated co設置設置and 成本o使p: %s with %d 使nits"), 
        *G本o使p的a設置e, UnitIDs.的使設置());
    
    本et使本n G本o使pID;
}

正oid UMin成TacticalLaye本Cont本olle本::DisbandCo設置設置andG本o使p(int32 G本o使pID)
{
    Co設置設置andG本o使ps.Re設置o正e(G本o使pID);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Disbanded co設置設置and 成本o使p: %d"), G本o使pID);
}

正oid UMin成TacticalLaye本Cont本olle本::Set軍o本設置ation(int32 G本o使pID, const 軍St本in成& 軍o本設置ationType, float Spacin成)
{
    軍Min成Co設置設置andG本o使p* G本o使p = Co設置設置andG本o使ps.軍ind(G本o使pID);
    if (!G本o使p)
    {
        本et使本n;
    }
    
    G本o使p->軍o本設置ationType = 軍o本設置ationType;
    
    // In 本eal i設置ple設置entation, this wo使ld a本本an成e 使nits in the specified fo本設置ation
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set fo本設置ation %s fo本 成本o使p %d with spacin成 %f"), 
        *軍o本設置ationType, G本o使pID, Spacin成);
}

正oid UMin成TacticalLaye本Cont本olle本::Rotate軍o本設置ation(int32 G本o使pID, float An成leDe成本ees)
{
    軍Min成Co設置設置andG本o使p* G本o使p = Co設置設置andG本o使ps.軍ind(G本o使pID);
    if (!G本o使p)
    {
        本et使本n;
    }
    
    // In 本eal i設置ple設置entation, this wo使ld 本otate the fo本設置ation
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Rotated fo本設置ation fo本 成本o使p %d by %f de成本ees"), 
        G本o使pID, An成leDe成本ees);
}

TA本本ay<軍Min成Co設置設置andG本o使p> UMin成TacticalLaye本Cont本olle本::GetAllCo設置設置andG本o使ps() const
{
    TA本本ay<軍Min成Co設置設置andG本o使p> Res使lt;
    Co設置設置andG本o使ps.Gene本ateVal使eA本本ay(Res使lt);
    本et使本n Res使lt;
}

// Ca設置e本a & View
正oid UMin成TacticalLaye本Cont本olle本::軍oc使sOnUnit(int32 UnitID)
{
    軍Min成TacticalUnit Unit;
    if (GetUnit(UnitID, Unit))
    {
        Ca設置e本aPosition = Unit.C使本本entPosition;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Ca設置e本a foc使sed on 使nit %d"), UnitID);
    }
}

正oid UMin成TacticalLaye本Cont本olle本::軍oc使sOnSelection()
{
    if (SelectedUnitIDs.的使設置() == 0)
    {
        本et使本n;
    }
    
    軍Vecto本 Cente本 = 軍Vecto本::Ze本oVecto本;
    int32 ValidCo使nt = 0;
    
    fo本 (int32 UnitID : SelectedUnitIDs)
    {
        軍Min成TacticalUnit Unit;
        if (GetUnit(UnitID, Unit))
        {
            Cente本 += Unit.C使本本entPosition;
            ValidCo使nt++;
        }
    }
    
    if (ValidCo使nt > 0)
    {
        Ca設置e本aPosition = Cente本 / ValidCo使nt;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Ca設置e本a foc使sed on %d selected 使nits"), ValidCo使nt);
    }
}

正oid UMin成TacticalLaye本Cont本olle本::軍oc使sOnBattleCente本(const 軍St本in成& BattleID)
{
    軍Min成BattleInfo Battle;
    if (GetBattle(BattleID, Battle))
    {
        Ca設置e本aPosition = Battle.BattleCente本;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Ca設置e本a foc使sed on battle cente本: %s"), *Battle.Battle的a設置e);
    }
}

正oid UMin成TacticalLaye本Cont本olle本::SetCa設置e本a輸入ei成ht(float 輸入ei成ht)
{
    Ca設置e本a輸入ei成ht = 軍Math::Cla設置p(輸入ei成ht, 500.0f, 5000.0f);
}

正oid UMin成TacticalLaye本Cont本olle本::SetCa設置e本aRotation(float Yaw, float Pitch)
{
    // In 本eal i設置ple設置entation, this wo使ld set ca設置e本a 本otation
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ca設置e本a 本otation set: Yaw=%f, Pitch=%f"), Yaw, Pitch);
}

// Co設置bat Statistics
float UMin成TacticalLaye本Cont本olle本::GetBattleP本o成本ess(const 軍St本in成& BattleID) const
{
    軍Min成BattleInfo Battle;
    if (!GetBattle(BattleID, Battle))
    {
        本et使本n 0.0f;
    }
    
    // Calc使late based on cas使alties o本 ti設置e
    int32 TotalCas使alties = Battle.Attacke本Cas使alties + Battle.Defende本Cas使alties;
    int32 TotalInitialSt本en成th = 1000; // This wo使ld be t本acked
    
    本et使本n 軍Math::Cla設置p(static下cast<float>(TotalCas使alties) / TotalInitialSt本en成th, 0.0f, 1.0f);
}

正oid UMin成TacticalLaye本Cont本olle本::Repo本tCas使alties(int32 UnitID, int32 Cas使altyCo使nt)
{
    軍Min成TacticalUnit* Unit = Units.軍ind(UnitID);
    if (!Unit)
    {
        本et使本n;
    }
    
    軍Min成BattleInfo* Battle = Acti正eBattles.軍ind(C使本本entBattleID);
    if (!Battle)
    {
        本et使本n;
    }
    
    if (Unit->軍actionID == Battle->Attackin成軍action)
    {
        Battle->Attacke本Cas使alties += Cas使altyCo使nt;
    }
    else if (Unit->軍actionID == Battle->Defendin成軍action)
    {
        Battle->Defende本Cas使alties += Cas使altyCo使nt;
    }
    
    OnCas使altiesRepo本ted.B本oadcast(UnitID, Cas使altyCo使nt);
    
    // Check if battle sho使ld end
    CheckBattleEndConditions(C使本本entBattleID);
}

TMap<軍St本in成, int32> UMin成TacticalLaye本Cont本olle本::GetCas使altyRepo本t(const 軍St本in成& BattleID) const
{
    TMap<軍St本in成, int32> Repo本t;
    
    軍Min成BattleInfo Battle;
    if (GetBattle(BattleID, Battle))
    {
        Repo本t.Add(Battle.Attackin成軍action, Battle.Attacke本Cas使alties);
        Repo本t.Add(Battle.Defendin成軍action, Battle.Defende本Cas使alties);
    }
    
    本et使本n Repo本t;
}

float UMin成TacticalLaye本Cont本olle本::Get軍actionSt本en成thRatio(const 軍St本in成& BattleID) const
{
    軍Min成BattleInfo Battle;
    if (!GetBattle(BattleID, Battle))
    {
        本et使本n 1.0f;
    }
    
    int32 Attacke本Li正e = GetLi正eUnitCo使ntBy軍action(Battle.Attackin成軍action);
    int32 Defende本Li正e = GetLi正eUnitCo使ntBy軍action(Battle.Defendin成軍action);
    
    if (Defende本Li正e == 0)
    {
        本et使本n 999.0f; // Attacke本 o正e本whel設置in成
    }
    
    本et使本n static下cast<float>(Attacke本Li正e) / Defende本Li正e;
}

// Inte本nal f使nctions
正oid UMin成TacticalLaye本Cont本olle本::CheckBattleEndConditions(const 軍St本in成& BattleID)
{
    軍Min成BattleInfo* Battle = Acti正eBattles.軍ind(BattleID);
    if (!Battle  !Battle->bIsActi正e)
    {
        本et使本n;
    }
    
    int32 Attacke本Li正e = GetLi正eUnitCo使ntBy軍action(Battle->Attackin成軍action);
    int32 Defende本Li正e = GetLi正eUnitCo使ntBy軍action(Battle->Defendin成軍action);
    
    // Check 正icto本y conditions
    if (Defende本Li正e == 0)
    {
        EndBattle(BattleID, t本使e); // Attacke本 wins
    }
    else if (Attacke本Li正e == 0)
    {
        EndBattle(BattleID, false); // Defende本 wins
    }
    else if (Battle->BattleD使本ation > 1800.0f) // 30 設置in使te ti設置e li設置it
    {
        // D本aw o本 defende本 wins by holdin成
        EndBattle(BattleID, false);
    }
}

正oid UMin成TacticalLaye本Cont本olle本::OnUnitDest本oyed(int32 UnitID)
{
    // Re設置o正e f本o設置 selection
    SelectedUnitIDs.Re設置o正e(UnitID);
    
    // Check battle end conditions
    CheckBattleEndConditions(C使本本entBattleID);
}

正oid UMin成TacticalLaye本Cont本olle本::的otifySelectionChan成ed()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Selection chan成ed: %d 使nits selected"), SelectedUnitIDs.的使設置());
}

正oid UMin成TacticalLaye本Cont本olle本::InitializeDefa使ltUnits(const 軍St本in成& BattleID)
{
    軍Min成BattleInfo* Battle = Acti正eBattles.軍ind(BattleID);
    if (!Battle)
    {
        本et使本n;
    }
    
    // Spawn attacke本 使nits
    fo本 (int32 i = 0; i < 10; i++)
    {
        軍Min成TacticalUnit Unit;
        Unit.Unit的a設置e = 軍St本in成::P本intf(TEXT("Infant本y下%d"), i);
        Unit.UnitType = TEXT("Infant本y");
        Unit.軍actionID = Battle->Attackin成軍action;
        Unit.C使本本entPosition = Battle->BattleCente本 + 軍Vecto本(-500 + i * 50, -300, 0);
        Unit.Max輸入ealth = 100;
        
        SpawnUnit(Unit);
    }
    
    // Spawn defende本 使nits
    fo本 (int32 i = 0; i < 10; i++)
    {
        軍Min成TacticalUnit Unit;
        Unit.Unit的a設置e = 軍St本in成::P本intf(TEXT("Defende本下%d"), i);
        Unit.UnitType = TEXT("Infant本y");
        Unit.軍actionID = Battle->Defendin成軍action;
        Unit.C使本本entPosition = Battle->BattleCente本 + 軍Vecto本(-250 + i * 50, 300, 0);
        Unit.Max輸入ealth = 100;
        
        SpawnUnit(Unit);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initialized defa使lt 使nits fo本 battle: %s"), *Battle->Battle的a設置e);
}

正oid UMin成TacticalLaye本Cont本olle本::Clean使pBattle(const 軍St本in成& BattleID)
{
    軍Min成BattleInfo Battle;
    if (GetBattle(BattleID, Battle))
    {
        // Re設置o正e all 使nits fo本 this battle
        TA本本ay<int32> UnitsToRe設置o正e;
        
        fo本 (const a使to& Pai本 : Units)
        {
            if (Pai本.Val使e.軍actionID == Battle.Attackin成軍action  
                Pai本.Val使e.軍actionID == Battle.Defendin成軍action)
            {
                UnitsToRe設置o正e.Add(Pai本.Key);
            }
        }
        
        fo本 (int32 UnitID : UnitsToRe設置o正e)
        {
            Re設置o正eUnit(UnitID);
        }
    }
    
    Acti正eBattles.Re設置o正e(BattleID);
}
