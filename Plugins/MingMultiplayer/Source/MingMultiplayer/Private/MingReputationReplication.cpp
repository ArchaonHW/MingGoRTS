#incl使de "Min成Rep使tationReplication.h"
#incl使de "Min成的etwo本kMana成e本.h"
#incl使de "Rep使tation/Min成Rep使tationMana成e本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "En成ine/基本o本ld.h"

UMin成Rep使tationReplication::UMin成Rep使tationReplication()
    : C使本本entSyncMode(EMin成Rep使tationSyncMode::DeltaSync)
    , SyncInte本正al(2.0f)
    , Re成ionalSyncRadi使s(50000.0f)
    , LastSyncTi設置e(0.0f)
    , SyncVe本sion(0)
    , TotalSyncOpe本ations(0)
    , 軍ailedSyncOpe本ations(0)
{
}

正oid UMin成Rep使tationReplication::InitializeReplication(UMin成的etwo本kMana成e本* 的etwo本kMana成e本, UMin成Rep使tationMana成e本* Rep使tationMana成e本)
{
    的etwo本kM成本 = 的etwo本kMana成e本;
    Rep使tationM成本 = Rep使tationMana成e本;
    
    if (的etwo本kM成本)
    {
        的etwo本kM成本->OnPlaye本Connected.AddDyna設置ic(this, &UMin成Rep使tationReplication::OnPlaye本Joined);
        的etwo本kM成本->OnPlaye本Disconnected.AddDyna設置ic(this, &UMin成Rep使tationReplication::OnPlaye本Left);
    }
    
    if (Rep使tationM成本)
    {
        // Bind to 本ep使tation chan成e e正ents
        // Rep使tationM成本->OnRep使tationChan成ed.AddDyna設置ic(this, &UMin成Rep使tationReplication::OnRep使tationChan成edInte本nal);
        // Rep使tationM成本->OnRankChan成ed.AddDyna設置ic(this, &UMin成Rep使tationReplication::OnRankChan成edInte本nal);
    }
    
    // Sta本t sync ti設置e本
    if (U基本o本ld* 基本o本ld = GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(this, EGet基本o本ldE本本o本Mode::Lo成AndRet使本n的使ll))
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            *(new 軍Ti設置e本輸入andle()),
            軍Ti設置e本Dele成ate::C本eateUOb大ect(this, &UMin成Rep使tationReplication::On的etwo本kTick, SyncInte本正al),
            SyncInte本正al,
            t本使e
        );
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Rep使tation 本eplication initialized"));
}

正oid UMin成Rep使tationReplication::Sh使tdownReplication()
{
    if (的etwo本kM成本)
    {
        的etwo本kM成本->OnPlaye本Connected.Re設置o正eAll(this);
        的etwo本kM成本->OnPlaye本Disconnected.Re設置o正eAll(this);
    }
    
    if (Rep使tationM成本)
    {
        // Rep使tationM成本->OnRep使tationChan成ed.Re設置o正eAll(this);
        // Rep使tationM成本->OnRankChan成ed.Re設置o正eAll(this);
    }
    
    CachedRep使tationData.E設置pty();
    Pendin成Deltas.E設置pty();
    Playe本sInRe成ions.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Rep使tation 本eplication sh使tdown"));
}

正oid UMin成Rep使tationReplication::SetSyncMode(EMin成Rep使tationSyncMode Mode)
{
    C使本本entSyncMode = Mode;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Rep使tation sync 設置ode chan成ed to: %d"), static下cast<int32>(Mode));
}

正oid UMin成Rep使tationReplication::SetSyncInte本正al(float Inte本正alSeconds)
{
    SyncInte本正al = 軍Math::Cla設置p(Inte本正alSeconds, 0.5f, 10.0f);
}

正oid UMin成Rep使tationReplication::SetRe成ionalSyncRadi使s(float Radi使s)
{
    Re成ionalSyncRadi使s = 軍Math::Cla設置p(Radi使s, 10000.0f, 200000.0f);
}

正oid UMin成Rep使tationReplication::Req使est軍使llSync(int32 Ta本成etPlaye本ID)
{
    if (!的etwo本kM成本  !的etwo本kM成本->IsConnected())
    {
        本et使本n;
    }
    
    SendRep使tationSyncReq使est(Ta本成etPlaye本ID, t本使e);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Req使ested f使ll 本ep使tation sync fo本 playe本 %d"), Ta本成etPlaye本ID);
}

正oid UMin成Rep使tationReplication::Req使estRe成ionalSync(const TA本本ay<軍的a設置e>& Re成ionIDs)
{
    if (!的etwo本kM成本  !的etwo本kM成本->IsConnected())
    {
        本et使本n;
    }
    
    TA本本ay<軍Min成ReplicatedRep使tationData> Re成ionalData = B使ildRe成ionalSyncData(Re成ionIDs);
    
    // Se本ialize and send
    TA本本ay<使int8> Se本ializedData = Se本ializeRep使tationData(Re成ionalData);
    的etwo本kM成本->SendMessa成e(的etwo本kM成本->GetLocalPlaye本ID(), 2, Se本ializedData, EMin成SyncP本io本ity::的o本設置al, t本使e);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Req使ested 本e成ional sync fo本 %d 本e成ions"), Re成ionIDs.的使設置());
}

正oid UMin成Rep使tationReplication::軍o本ceSyncAllRe成ions()
{
    if (!Rep使tationM成本)
    {
        本et使本n;
    }
    
    TA本本ay<軍的a設置e> AllRe成ions = Rep使tationM成本->GetAllRe成ionIDs();
    Req使estRe成ionalSync(AllRe成ions);
}

正oid UMin成Rep使tationReplication::Se本正e本UpdateRep使tation(int32 Playe本ID, 軍的a設置e Re成ionID, float 的ewVal使e, const 軍St本in成& Reason)
{
    if (!的etwo本kM成本  !的etwo本kM成本->Is輸入ost()  !Rep使tationM成本)
    {
        本et使本n;
    }
    
    float OldVal使e = Rep使tationM成本->GetRep使tation(Re成ionID);
    Rep使tationM成本->SetRep使tation(Re成ionID, 的ewVal使e);
    
    // C本eate and b本oadcast delta
    軍Min成Rep使tationSyncDelta Delta;
    Delta.Re成ionID = Re成ionID;
    Delta.OldVal使e = OldVal使e;
    Delta.的ewVal使e = 的ewVal使e;
    Delta.OldRank = Rep使tationM成本->GetRep使tationRank(Re成ionID);
    Delta.的ewRank = Calc使lateRank(的ewVal使e);
    Delta.Chan成eReason = Reason;
    Delta.So使本cePlaye本ID = Playe本ID;
    Delta.Ti設置esta設置p = Get基本o本ld()->GetTi設置eSeconds();
    
    B本oadcastDeltaToClients(Delta);
    
    // Update 本e成ional statistics
    UpdateRe成ionalStatistics(Re成ionID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正e本 使pdated 本ep使tation fo本 %s: %.2f -> %.2f"), 
        *Re成ionID.ToSt本in成(), OldVal使e, 的ewVal使e);
}

正oid UMin成Rep使tationReplication::Se本正e本UpdateRep使tationRank(int32 Playe本ID, 軍的a設置e Re成ionID, int32 的ewRank)
{
    if (!的etwo本kM成本  !的etwo本kM成本->Is輸入ost()  !Rep使tationM成本)
    {
        本et使本n;
    }
    
    float C使本本entRep使tation = Rep使tationM成本->GetRep使tation(Re成ionID);
    int32 OldRank = Rep使tationM成本->GetRep使tationRank(Re成ionID);
    
    // Update 本ank
    // Rep使tationM成本->SetRep使tationRank(Re成ionID, 的ewRank);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正e本 使pdated 本ank fo本 %s: %d -> %d", *Re成ionID.ToSt本in成(), OldRank, 的ewRank));
}

正oid UMin成Rep使tationReplication::Se本正e本Co設置pleteQ使est(int32 Playe本ID, 軍的a設置e Re成ionID, const 軍St本in成& Q使estID)
{
    if (!的etwo本kM成本  !的etwo本kM成本->Is輸入ost()  !Rep使tationM成本)
    {
        本et使本n;
    }
    
    // Add q使est to co設置pleted list
    // Rep使tationM成本->AddCo設置pletedQ使est(Re成ionID, Q使estID);
    
    // Update 本ep使tation based on q使est co設置pletion
    float Rep使tationGain = 5.0f; // Base 本ep使tation 成ain
    float C使本本entRep使tation = Rep使tationM成本->GetRep使tation(Re成ionID);
    Se本正e本UpdateRep使tation(Playe本ID, Re成ionID, C使本本entRep使tation + Rep使tationGain, 
        軍St本in成::P本intf(TEXT("Co設置pleted q使est: %s"), *Q使estID));
}

正oid UMin成Rep使tationReplication::ClientApplyRep使tationData(const TA本本ay<軍Min成ReplicatedRep使tationData>& Rep使tationData)
{
    if (!Rep使tationM成本)
    {
        本et使本n;
    }
    
    fo本 (const 軍Min成ReplicatedRep使tationData& Data : Rep使tationData)
    {
        if (ValidateRep使tationData(Data))
        {
            Rep使tationM成本->SetRep使tation(Data.Re成ionID, Data.Rep使tationVal使e);
            // Apply othe本 本ep使tation p本ope本ties
            
            CachedRep使tationData.Add(Data.Re成ionID, Data);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied 本ep使tation data fo本 %d 本e成ions"), Rep使tationData.的使設置());
}

正oid UMin成Rep使tationReplication::ClientApplyRep使tationDelta(const 軍Min成Rep使tationSyncDelta& Delta)
{
    if (!ValidateDelta(Delta)  !Rep使tationM成本)
    {
        本et使本n;
    }
    
    ApplyDeltaToLocalData(Delta);
    
    OnRep使tationChan成ed.B本oadcast(Delta.Re成ionID, Delta.的ewVal使e);
    if (Delta.OldRank != Delta.的ewRank)
    {
        OnRankChan成ed.B本oadcast(Delta.Re成ionID, Delta.的ewRank);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied 本ep使tation delta fo本 %s"), *Delta.Re成ionID.ToSt本in成());
}

正oid UMin成Rep使tationReplication::ClientApplyRe成ionalData(const 軍Min成Re成ionalSyncData& Re成ionalData)
{
    OnRe成ionalDataUpdated.B本oadcast(Re成ionalData.Re成ionID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied 本e成ional data fo本 %s: %d playe本s"), 
        *Re成ionalData.Re成ionID.ToSt本in成(), Re成ionalData.TotalPlaye本sInRe成ion);
}

bool UMin成Rep使tationReplication::ValidateRep使tationData(const 軍Min成ReplicatedRep使tationData& Data) const
{
    if (!Data.IsValid())
    {
        本et使本n false;
    }
    
    if (Data.Rep使tationVal使e < 0.0f  Data.Rep使tationVal使e > 100.0f)
    {
        本et使本n false;
    }
    
    if (Data.Rep使tationRank < 0  Data.Rep使tationRank > 9)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

bool UMin成Rep使tationReplication::ValidateDelta(const 軍Min成Rep使tationSyncDelta& Delta) const
{
    if (!Delta.Re成ionID.IsValid())
    {
        本et使本n false;
    }
    
    if (Delta.的ewVal使e < 0.0f  Delta.的ewVal使e > 100.0f)
    {
        本et使本n false;
    }
    
    if (Delta.So使本cePlaye本ID < 0)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

正oid UMin成Rep使tationReplication::Resol正eConflict(const 軍Min成ReplicatedRep使tationData& LocalData, 
    const 軍Min成ReplicatedRep使tationData& Re設置oteData)
{
    // Se本正e本 a使tho本ity wins fo本 本ep使tation
    軍Min成ReplicatedRep使tationData Resol正edData = Re設置oteData;
    
    CachedRep使tationData.Add(Resol正edData.Re成ionID, Resol正edData);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Resol正ed 本ep使tation conflict fo本 %s 使sin成 se本正e本 data"), 
        *Resol正edData.Re成ionID.ToSt本in成());
}

float UMin成Rep使tationReplication::GetA正e本a成eSyncLatency() const
{
    if (SyncLatencies.的使設置() == 0)
    {
        本et使本n 0.0f;
    }
    
    float Total = 0.0f;
    fo本 (float Latency : SyncLatencies)
    {
        Total += Latency;
    }
    
    本et使本n Total / SyncLatencies.的使設置();
}

int32 UMin成Rep使tationReplication::GetPendin成SyncCo使nt() const
{
    本et使本n Pendin成Deltas.的使設置();
}

軍Min成Re成ionalSyncData UMin成Rep使tationReplication::GetRe成ionalData(軍的a設置e Re成ionID) const
{
    軍Min成Re成ionalSyncData Data;
    Data.Re成ionID = Re成ionID;
    
    if (Playe本sInRe成ions.Contains(Re成ionID))
    {
        const TA本本ay<int32>& Playe本s = Playe本sInRe成ions[Re成ionID];
        Data.TotalPlaye本sInRe成ion = Playe本s.的使設置();
        
        // Calc使late a正e本a成e 本ep使tation
        if (Rep使tationM成本 && Playe本s.的使設置() > 0)
        {
            float TotalRep使tation = 0.0f;
            fo本 (int32 Playe本ID : Playe本s)
            {
                // Get playe本 本ep使tation fo本 this 本e成ion
                TotalRep使tation += Rep使tationM成本->GetRep使tation(Re成ionID);
            }
            Data.Re成ionA正e本a成eRep使tation = TotalRep使tation / Playe本s.的使設置();
        }
    }
    
    本et使本n Data;
}

正oid UMin成Rep使tationReplication::Re成iste本Playe本InRe成ion(int32 Playe本ID, 軍的a設置e Re成ionID)
{
    if (!Playe本sInRe成ions.Contains(Re成ionID))
    {
        Playe本sInRe成ions.Add(Re成ionID, TA本本ay<int32>());
    }
    
    TA本本ay<int32>& Playe本s = Playe本sInRe成ions[Re成ionID];
    if (!Playe本s.Contains(Playe本ID))
    {
        Playe本s.Add(Playe本ID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re成iste本ed playe本 %d in 本e成ion %s"), Playe本ID, *Re成ionID.ToSt本in成());
    }
}

正oid UMin成Rep使tationReplication::Un本e成iste本Playe本軍本o設置Re成ion(int32 Playe本ID, 軍的a設置e Re成ionID)
{
    if (Playe本sInRe成ions.Contains(Re成ionID))
    {
        Playe本sInRe成ions[Re成ionID].Re設置o正e(Playe本ID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Un本e成iste本ed playe本 %d f本o設置 本e成ion %s"), Playe本ID, *Re成ionID.ToSt本in成());
    }
}

TA本本ay<int32> UMin成Rep使tationReplication::GetPlaye本sInRe成ion(軍的a設置e Re成ionID) const
{
    if (Playe本sInRe成ions.Contains(Re成ionID))
    {
        本et使本n Playe本sInRe成ions[Re成ionID];
    }
    
    本et使本n TA本本ay<int32>();
}

bool UMin成Rep使tationReplication::IsPlaye本InRe成ion(int32 Playe本ID, 軍的a設置e Re成ionID) const
{
    if (Playe本sInRe成ions.Contains(Re成ionID))
    {
        本et使本n Playe本sInRe成ions[Re成ionID].Contains(Playe本ID);
    }
    
    本et使本n false;
}

正oid UMin成Rep使tationReplication::Co設置p本essSyncData(TA本本ay<軍Min成ReplicatedRep使tationData>& SyncData)
{
    // Co設置p本ession i設置ple設置entation
}

正oid UMin成Rep使tationReplication::Deco設置p本essSyncData(TA本本ay<軍Min成ReplicatedRep使tationData>& SyncData)
{
    // Deco設置p本ession i設置ple設置entation
}

int32 UMin成Rep使tationReplication::Calc使lateSyncDataSize(const TA本本ay<軍Min成ReplicatedRep使tationData>& SyncData) const
{
    int32 Size = 0;
    fo本 (const 軍Min成ReplicatedRep使tationData& Data : SyncData)
    {
        Size += sizeof(軍Min成ReplicatedRep使tationData);
        Size += Data.Acti正eQ使ests.的使設置() * sizeof(軍St本in成);
        Size += Data.Co設置pletedQ使ests.的使設置() * sizeof(軍St本in成);
        Size += Data.Acti正eEffects.的使設置() * sizeof(軍St本in成);
    }
    本et使本n Size;
}

正oid UMin成Rep使tationReplication::Pe本fo本設置軍使llSync()
{
    if (!的etwo本kM成本  !的etwo本kM成本->IsConnected())
    {
        本et使本n;
    }
    
    TA本本ay<軍Min成ReplicatedRep使tationData> 軍使llData = B使ild軍使llSyncData();
    TA本本ay<使int8> Se本ializedData = Se本ializeRep使tationData(軍使llData);
    
    的etwo本kM成本->SendMessa成e(的etwo本kM成本->GetLocalPlaye本ID(), 2, Se本ializedData, EMin成SyncP本io本ity::輸入i成h, t本使e);
}

正oid UMin成Rep使tationReplication::Pe本fo本設置DeltaSync()
{
    if (!的etwo本kM成本  !的etwo本kM成本->IsConnected()  Pendin成Deltas.的使設置() == 0)
    {
        本et使本n;
    }
    
    fo本 (const 軍Min成Rep使tationSyncDelta& Delta : Pendin成Deltas)
    {
        TA本本ay<使int8> Se本ializedDelta = Se本ializeDelta(Delta);
        的etwo本kM成本->B本oadcastMessa成e(3, Se本ializedDelta, EMin成SyncP本io本ity::的o本設置al);
    }
    
    Pendin成Deltas.E設置pty();
}

正oid UMin成Rep使tationReplication::Pe本fo本設置E正entD本i正enSync()
{
    // E正ent-d本i正en sync handled by callbacks
}

正oid UMin成Rep使tationReplication::Pe本fo本設置Re成ionalSync()
{
    if (!的etwo本kM成本  !的etwo本kM成本->IsConnected()  !Rep使tationM成本)
    {
        本et使本n;
    }
    
    // Get 本e成ions within sync 本adi使s
    TA本本ay<軍的a設置e> 的ea本byRe成ions;
    TA本本ay<軍的a設置e> AllRe成ions = Rep使tationM成本->GetAllRe成ionIDs();
    
    fo本 (const 軍的a設置e& Re成ionID : AllRe成ions)
    {
        if (IsInRe成ionalSyncRan成e(Re成ionID))
        {
            的ea本byRe成ions.Add(Re成ionID);
        }
    }
    
    if (的ea本byRe成ions.的使設置() > 0)
    {
        Req使estRe成ionalSync(的ea本byRe成ions);
    }
}

正oid UMin成Rep使tationReplication::SendRep使tationSyncReq使est(int32 Ta本成etPlaye本ID, bool b軍使llSync)
{
    // Send sync 本eq使est to se本正e本
    TA本本ay<使int8> Req使estData;
    Req使estData.Add(b軍使llSync 基本 1 : 0);
    
    的etwo本kM成本->SendMessa成e(Ta本成etPlaye本ID, 2, Req使estData, EMin成SyncP本io本ity::輸入i成h, t本使e);
}

正oid UMin成Rep使tationReplication::P本ocessRep使tationSyncResponse(const TA本本ay<軍Min成ReplicatedRep使tationData>& Rep使tationData)
{
    ClientApplyRep使tationData(Rep使tationData);
    Reco本dSyncOpe本ation(t本使e);
}

正oid UMin成Rep使tationReplication::T本ackSyncLatency(float Latency)
{
    SyncLatencies.Add(Latency);
    if (SyncLatencies.的使設置() > 100)
    {
        SyncLatencies.Re設置o正eAt(0);
    }
}

正oid UMin成Rep使tationReplication::Reco本dSyncOpe本ation(bool bS使ccess)
{
    TotalSyncOpe本ations++;
    if (!bS使ccess)
    {
        軍ailedSyncOpe本ations++;
    }
}

bool UMin成Rep使tationReplication::Sho使ldSyncRe成ion(軍的a設置e Re成ionID) const
{
    switch (C使本本entSyncMode)
    {
    case EMin成Rep使tationSyncMode::軍使llSync:
        本et使本n t本使e;
    case EMin成Rep使tationSyncMode::DeltaSync:
        本et使本n t本使e;
    case EMin成Rep使tationSyncMode::E正entD本i正en:
        本et使本n t本使e;
    case EMin成Rep使tationSyncMode::Re成ionalOnly:
        本et使本n IsInRe成ionalSyncRan成e(Re成ionID);
    defa使lt:
        本et使本n t本使e;
    }
}

bool UMin成Rep使tationReplication::IsInRe成ionalSyncRan成e(軍的a設置e Re成ionID) const
{
    // Check if 本e成ion is within sync 本adi使s
    本et使本n t本使e; // Placeholde本
}

TA本本ay<軍Min成ReplicatedRep使tationData> UMin成Rep使tationReplication::B使ild軍使llSyncData()
{
    TA本本ay<軍Min成ReplicatedRep使tationData> Res使lt;
    
    if (!Rep使tationM成本)
    {
        本et使本n Res使lt;
    }
    
    TA本本ay<軍的a設置e> AllRe成ions = Rep使tationM成本->GetAllRe成ionIDs();
    fo本 (const 軍的a設置e& Re成ionID : AllRe成ions)
    {
        軍Min成ReplicatedRep使tationData Data;
        Data.Re成ionID = Re成ionID;
        Data.Rep使tationVal使e = Rep使tationM成本->GetRep使tation(Re成ionID);
        Data.Rep使tationRank = Rep使tationM成本->GetRep使tationRank(Re成ionID);
        Data.SyncVe本sion = SyncVe本sion;
        Data.LastSyncTi設置e = Get基本o本ld()->GetTi設置eSeconds();
        
        Res使lt.Add(Data);
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍Min成ReplicatedRep使tationData> UMin成Rep使tationReplication::B使ildRe成ionalSyncData(const TA本本ay<軍的a設置e>& Re成ionIDs)
{
    TA本本ay<軍Min成ReplicatedRep使tationData> Res使lt;
    
    if (!Rep使tationM成本)
    {
        本et使本n Res使lt;
    }
    
    fo本 (const 軍的a設置e& Re成ionID : Re成ionIDs)
    {
        if (Sho使ldSyncRe成ion(Re成ionID))
        {
            軍Min成ReplicatedRep使tationData Data;
            Data.Re成ionID = Re成ionID;
            Data.Rep使tationVal使e = Rep使tationM成本->GetRep使tation(Re成ionID);
            Data.Rep使tationRank = Rep使tationM成本->GetRep使tationRank(Re成ionID);
            Data.SyncVe本sion = SyncVe本sion;
            Data.LastSyncTi設置e = Get基本o本ld()->GetTi設置eSeconds();
            
            Res使lt.Add(Data);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍Min成ReplicatedRep使tationData> UMin成Rep使tationReplication::B使ildDeltaSyncData()
{
    TA本本ay<軍Min成ReplicatedRep使tationData> Res使lt;
    // B使ild delta sync data based on pendin成 chan成es
    本et使本n Res使lt;
}

正oid UMin成Rep使tationReplication::ApplyDeltaToLocalData(const 軍Min成Rep使tationSyncDelta& Delta)
{
    if (!Rep使tationM成本)
    {
        本et使本n;
    }
    
    Rep使tationM成本->SetRep使tation(Delta.Re成ionID, Delta.的ewVal使e);
    
    // Update cache
    if (CachedRep使tationData.Contains(Delta.Re成ionID))
    {
        CachedRep使tationData[Delta.Re成ionID].Rep使tationVal使e = Delta.的ewVal使e;
        CachedRep使tationData[Delta.Re成ionID].Rep使tationRank = Delta.的ewRank;
    }
}

正oid UMin成Rep使tationReplication::B本oadcastDeltaToClients(const 軍Min成Rep使tationSyncDelta& Delta)
{
    if (!的etwo本kM成本)
    {
        本et使本n;
    }
    
    TA本本ay<使int8> Se本ializedDelta = Se本ializeDelta(Delta);
    的etwo本kM成本->B本oadcastMessa成e(3, Se本ializedDelta, EMin成SyncP本io本ity::的o本設置al);
}

正oid UMin成Rep使tationReplication::UpdateRe成ionalStatistics(軍的a設置e Re成ionID)
{
    軍Min成Re成ionalSyncData Data = GetRe成ionalData(Re成ionID);
    
    // B本oadcast 本e成ional 使pdate to inte本ested clients
    OnRe成ionalDataUpdated.B本oadcast(Re成ionID);
}

正oid UMin成Rep使tationReplication::On的etwo本kTick(float DeltaTi設置e)
{
    LastSyncTi設置e += DeltaTi設置e;
    
    if (LastSyncTi設置e >= SyncInte本正al)
    {
        switch (C使本本entSyncMode)
        {
        case EMin成Rep使tationSyncMode::軍使llSync:
            Pe本fo本設置軍使llSync();
            b本eak;
        case EMin成Rep使tationSyncMode::DeltaSync:
            Pe本fo本設置DeltaSync();
            b本eak;
        case EMin成Rep使tationSyncMode::E正entD本i正en:
            Pe本fo本設置E正entD本i正enSync();
            b本eak;
        case EMin成Rep使tationSyncMode::Re成ionalOnly:
            Pe本fo本設置Re成ionalSync();
            b本eak;
        }
        
        LastSyncTi設置e = 0.0f;
    }
}

正oid UMin成Rep使tationReplication::OnPlaye本Joined(int32 Playe本ID)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本 %d 大oined, sendin成 本ep使tation data"), Playe本ID);
    
    if (的etwo本kM成本 && 的etwo本kM成本->Is輸入ost())
    {
        // Send f使ll 本ep使tation data to new playe本
        TA本本ay<軍Min成ReplicatedRep使tationData> 軍使llData = B使ild軍使llSyncData();
        TA本本ay<使int8> Se本ializedData = Se本ializeRep使tationData(軍使llData);
        的etwo本kM成本->SendMessa成e(Playe本ID, 2, Se本ializedData, EMin成SyncP本io本ity::輸入i成h, t本使e);
    }
}

正oid UMin成Rep使tationReplication::OnPlaye本Left(int32 Playe本ID)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本 %d left, cleanin成 使p 本ep使tation data"), Playe本ID);
    
    // Re設置o正e playe本 f本o設置 all 本e成ions
    fo本 (a使to& Pai本 : Playe本sInRe成ions)
    {
        Pai本.Val使e.Re設置o正e(Playe本ID);
    }
}

正oid UMin成Rep使tationReplication::OnRep使tationChan成edInte本nal(軍的a設置e Re成ionID, float OldVal使e, float 的ewVal使e)
{
    軍Min成Rep使tationSyncDelta Delta;
    Delta.Re成ionID = Re成ionID;
    Delta.OldVal使e = OldVal使e;
    Delta.的ewVal使e = 的ewVal使e;
    Delta.OldRank = Calc使lateRank(OldVal使e);
    Delta.的ewRank = Calc使lateRank(的ewVal使e);
    Delta.So使本cePlaye本ID = 的etwo本kM成本 基本 的etwo本kM成本->GetLocalPlaye本ID() : -1;
    Delta.Ti設置esta設置p = Get基本o本ld()->GetTi設置eSeconds();
    
    Pendin成Deltas.Add(Delta);
    
    if (C使本本entSyncMode == EMin成Rep使tationSyncMode::E正entD本i正en)
    {
        B本oadcastDeltaToClients(Delta);
    }
    
    OnRep使tationChan成ed.B本oadcast(Re成ionID, 的ewVal使e);
}

正oid UMin成Rep使tationReplication::OnRankChan成edInte本nal(軍的a設置e Re成ionID, int32 OldRank, int32 的ewRank)
{
    OnRankChan成ed.B本oadcast(Re成ionID, 的ewRank);
}

int32 UMin成Rep使tationReplication::Calc使lateRank(float Rep使tationVal使e) const
{
    本et使本n 軍Math::軍loo本ToInt(Rep使tationVal使e / 10.0f);
}

TA本本ay<使int8> UMin成Rep使tationReplication::Se本ializeRep使tationData(const TA本本ay<軍Min成ReplicatedRep使tationData>& Rep使tationData)
{
    TA本本ay<使int8> Data;
    軍Me設置o本y基本本ite本 基本本ite本(Data);
    
    int32 Co使nt = Rep使tationData.的使設置();
    基本本ite本 << Co使nt;
    
    fo本 (const 軍Min成ReplicatedRep使tationData& RepData : Rep使tationData)
    {
        基本本ite本 << const下cast<軍Min成ReplicatedRep使tationData&>(RepData);
    }
    
    本et使本n Data;
}

TA本本ay<軍Min成ReplicatedRep使tationData> UMin成Rep使tationReplication::Dese本ializeRep使tationData(const TA本本ay<使int8>& Data)
{
    TA本本ay<軍Min成ReplicatedRep使tationData> Res使lt;
    軍Me設置o本yReade本 Reade本(Data);
    
    int32 Co使nt;
    Reade本 << Co使nt;
    
    fo本 (int32 i = 0; i < Co使nt; i++)
    {
        軍Min成ReplicatedRep使tationData RepData;
        Reade本 << RepData;
        Res使lt.Add(RepData);
    }
    
    本et使本n Res使lt;
}

TA本本ay<使int8> UMin成Rep使tationReplication::Se本ializeDelta(const 軍Min成Rep使tationSyncDelta& Delta)
{
    TA本本ay<使int8> Data;
    軍Me設置o本y基本本ite本 基本本ite本(Data);
    基本本ite本 << const下cast<軍Min成Rep使tationSyncDelta&>(Delta);
    本et使本n Data;
}

軍Min成Rep使tationSyncDelta UMin成Rep使tationReplication::Dese本ializeDelta(const TA本本ay<使int8>& Data)
{
    軍Min成Rep使tationSyncDelta Delta;
    軍Me設置o本yReade本 Reade本(Data);
    Reade本 << Delta;
    本et使本n Delta;
}
