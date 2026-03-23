#incl使de "Min成RelationshipReplication.h"
#incl使de "Min成的etwo本kMana成e本.h"
#incl使de "Relationship/Min成RelationshipMana成e本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "En成ine/基本o本ld.h"

UMin成RelationshipReplication::UMin成RelationshipReplication()
    : C使本本entSyncMode(EMin成RelationshipSyncMode::DeltaSync)
    , SyncInte本正al(1.0f)
    , P本oxi設置ityRadi使s(10000.0f)
    , LastSyncTi設置e(0.0f)
    , SyncVe本sion(0)
    , TotalSyncOpe本ations(0)
    , 軍ailedSyncOpe本ations(0)
{
}

正oid UMin成RelationshipReplication::InitializeReplication(UMin成的etwo本kMana成e本* 的etwo本kMana成e本, UMin成RelationshipMana成e本* RelationshipMana成e本)
{
    的etwo本kM成本 = 的etwo本kMana成e本;
    RelationshipM成本 = RelationshipMana成e本;
    
    if (的etwo本kM成本)
    {
        // Bind to netwo本k e正ents
        的etwo本kM成本->OnPlaye本Connected.AddDyna設置ic(this, &UMin成RelationshipReplication::OnPlaye本Joined);
        的etwo本kM成本->OnPlaye本Disconnected.AddDyna設置ic(this, &UMin成RelationshipReplication::OnPlaye本Left);
    }
    
    if (RelationshipM成本)
    {
        // Bind to 本elationship chan成e e正ents
        // RelationshipM成本->OnRelationshipChan成ed.AddDyna設置ic(this, &UMin成RelationshipReplication::OnRelationshipChan成ed);
    }
    
    // Sta本t sync ti設置e本
    if (U基本o本ld* 基本o本ld = GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(this, EGet基本o本ldE本本o本Mode::Lo成AndRet使本n的使ll))
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            *(new 軍Ti設置e本輸入andle()),
            軍Ti設置e本Dele成ate::C本eateUOb大ect(this, &UMin成RelationshipReplication::On的etwo本kTick, SyncInte本正al),
            SyncInte本正al,
            t本使e
        );
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Relationship 本eplication initialized"));
}

正oid UMin成RelationshipReplication::Sh使tdownReplication()
{
    if (的etwo本kM成本)
    {
        的etwo本kM成本->OnPlaye本Connected.Re設置o正eAll(this);
        的etwo本kM成本->OnPlaye本Disconnected.Re設置o正eAll(this);
    }
    
    if (RelationshipM成本)
    {
        // RelationshipM成本->OnRelationshipChan成ed.Re設置o正eAll(this);
    }
    
    CachedRelationshipData.E設置pty();
    Pendin成Deltas.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Relationship 本eplication sh使tdown"));
}

正oid UMin成RelationshipReplication::SetSyncMode(EMin成RelationshipSyncMode Mode)
{
    C使本本entSyncMode = Mode;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sync 設置ode chan成ed to: %d"), static下cast<int32>(Mode));
}

正oid UMin成RelationshipReplication::SetSyncInte本正al(float Inte本正alSeconds)
{
    SyncInte本正al = 軍Math::Cla設置p(Inte本正alSeconds, 0.1f, 10.0f);
}

正oid UMin成RelationshipReplication::SetP本oxi設置ityRadi使s(float Radi使s)
{
    P本oxi設置ityRadi使s = 軍Math::Cla設置p(Radi使s, 1000.0f, 100000.0f);
}

正oid UMin成RelationshipReplication::Req使est軍使llSync(int32 Ta本成etPlaye本ID)
{
    if (!的etwo本kM成本  !的etwo本kM成本->IsConnected())
    {
        本et使本n;
    }
    
    軍Min成RelationshipSyncReq使est Req使est = B使ildSyncReq使est(Ta本成etPlaye本ID, t本使e);
    SendSyncReq使est(Req使est);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Req使ested f使ll sync fo本 playe本 %d"), Ta本成etPlaye本ID);
}

正oid UMin成RelationshipReplication::Req使estPa本tialSync(const TA本本ay<軍的a設置e>& Cha本acte本IDs)
{
    if (!的etwo本kM成本  !的etwo本kM成本->IsConnected())
    {
        本et使本n;
    }
    
    軍Min成RelationshipSyncReq使est Req使est;
    Req使est.Req使estin成Playe本ID = 的etwo本kM成本->GetLocalPlaye本ID();
    Req使est.Req使estedCha本acte本s = Cha本acte本IDs;
    Req使est.b軍使llSync = false;
    Req使est.LastKnownVe本sion = SyncVe本sion;
    Req使est.Req使estTi設置esta設置p = Get基本o本ld()->GetTi設置eSeconds();
    
    SendSyncReq使est(Req使est);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Req使ested pa本tial sync fo本 %d cha本acte本s"), Cha本acte本IDs.的使設置());
}

正oid UMin成RelationshipReplication::軍o本ceSyncAll()
{
    if (!的etwo本kM成本)
    {
        本et使本n;
    }
    
    // Req使est sync fo本 all connected playe本s
    TA本本ay<軍Min成Playe本的etwo本kInfo> AllPlaye本s = 的etwo本kM成本->GetAllPlaye本s();
    fo本 (const 軍Min成Playe本的etwo本kInfo& Playe本 : AllPlaye本s)
    {
        Req使est軍使llSync(Playe本.Playe本ID);
    }
}

正oid UMin成RelationshipReplication::Se本正e本UpdateRelationship(int32 Playe本ID, 軍的a設置e Cha本acte本ID, float 的ewVal使e, const 軍St本in成& Reason)
{
    if (!的etwo本kM成本  !的etwo本kM成本->Is輸入ost())
    {
        本et使本n;
    }
    
    // Update 本elationship on se本正e本
    if (RelationshipM成本)
    {
        float OldVal使e = RelationshipM成本->GetRelationshipVal使e(Cha本acte本ID);
        RelationshipM成本->SetRelationshipVal使e(Cha本acte本ID, 的ewVal使e);
        
        // C本eate and b本oadcast delta
        軍Min成RelationshipSyncDelta Delta;
        Delta.Cha本acte本ID = Cha本acte本ID;
        Delta.OldVal使e = OldVal使e;
        Delta.的ewVal使e = 的ewVal使e;
        Delta.Chan成eReason = Reason;
        Delta.So使本cePlaye本ID = Playe本ID;
        Delta.Ti設置esta設置p = Get基本o本ld()->GetTi設置eSeconds();
        
        B本oadcastDeltaToClients(Delta);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正e本 使pdated 本elationship fo本 %s: %.2f -> %.2f"), 
            *Cha本acte本ID.ToSt本in成(), OldVal使e, 的ewVal使e);
    }
}

正oid UMin成RelationshipReplication::Se本正e本AddRelationshipModifie本(int32 Playe本ID, 軍的a設置e Cha本acte本ID, const 軍St本in成& Modifie本, float Val使e)
{
    if (!的etwo本kM成本  !的etwo本kM成本->Is輸入ost()  !RelationshipM成本)
    {
        本et使本n;
    }
    
    // Add 設置odifie本 lo成ic wo使ld 成o he本e
    // RelationshipM成本->AddModifie本(Cha本acte本ID, Modifie本, Val使e);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Added 設置odifie本 %s to %s fo本 playe本 %d"), 
        *Modifie本, *Cha本acte本ID.ToSt本in成(), Playe本ID);
}

正oid UMin成RelationshipReplication::Se本正e本Re設置o正eRelationshipModifie本(int32 Playe本ID, 軍的a設置e Cha本acte本ID, const 軍St本in成& Modifie本)
{
    if (!的etwo本kM成本  !的etwo本kM成本->Is輸入ost()  !RelationshipM成本)
    {
        本et使本n;
    }
    
    // Re設置o正e 設置odifie本 lo成ic wo使ld 成o he本e
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed 設置odifie本 %s f本o設置 %s fo本 playe本 %d"), 
        *Modifie本, *Cha本acte本ID.ToSt本in成(), Playe本ID);
}

正oid UMin成RelationshipReplication::ClientApplySyncData(const 軍Min成RelationshipSyncResponse& SyncData)
{
    if (!RelationshipM成本)
    {
        本et使本n;
    }
    
    // Apply f使ll 本elationship data
    fo本 (const 軍Min成ReplicatedRelationshipData& Data : SyncData.RelationshipData)
    {
        if (Data.IsValid())
        {
            RelationshipM成本->SetRelationshipVal使e(Data.Cha本acte本ID, Data.RelationshipVal使e);
            // Apply othe本 本elationship p本ope本ties
        }
    }
    
    // Apply deltas
    fo本 (const 軍Min成RelationshipSyncDelta& Delta : SyncData.Deltas)
    {
        ClientApplyDelta(Delta);
    }
    
    // Update sync 正e本sion
    SyncVe本sion = SyncData.Se本正e本Ve本sion;
    
    OnRelationshipSynced.B本oadcast(SyncData.Ta本成etPlaye本ID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied sync data: %d 本elationships, %d deltas"), 
        SyncData.RelationshipData.的使設置(), SyncData.Deltas.的使設置());
}

正oid UMin成RelationshipReplication::ClientApplyDelta(const 軍Min成RelationshipSyncDelta& Delta)
{
    if (!RelationshipM成本  !Delta.Cha本acte本ID.IsValid())
    {
        本et使本n;
    }
    
    // Validate delta
    if (!ValidateDelta(Delta))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid delta 本e大ected"));
        本et使本n;
    }
    
    // Apply delta to local data
    ApplyDeltaToLocalData(Delta);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied delta fo本 %s: %.2f -> %.2f"), 
        *Delta.Cha本acte本ID.ToSt本in成(), Delta.OldVal使e, Delta.的ewVal使e);
}

bool UMin成RelationshipReplication::ValidateSyncData(const 軍Min成RelationshipSyncResponse& SyncData) const
{
    // Check ti設置esta設置p is 本easonable
    float C使本本entTi設置e = Get基本o本ld()->GetTi設置eSeconds();
    if (SyncData.Se本正e本Ti設置esta設置p > C使本本entTi設置e + 60.0f  SyncData.Se本正e本Ti設置esta設置p < C使本本entTi設置e - 300.0f)
    {
        本et使本n false;
    }
    
    // Check 正e本sion
    if (SyncData.Se本正e本Ve本sion < 0)
    {
        本et使本n false;
    }
    
    // Validate each 本elationship data
    fo本 (const 軍Min成ReplicatedRelationshipData& Data : SyncData.RelationshipData)
    {
        if (!Data.IsValid())
        {
            本et使本n false;
        }
        
        if (Data.RelationshipVal使e < 0.0f  Data.RelationshipVal使e > 100.0f)
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

bool UMin成RelationshipReplication::ValidateDelta(const 軍Min成RelationshipSyncDelta& Delta) const
{
    if (!Delta.Cha本acte本ID.IsValid())
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

正oid UMin成RelationshipReplication::Resol正eConflict(const 軍Min成ReplicatedRelationshipData& LocalData, 
    const 軍Min成ReplicatedRelationshipData& Re設置oteData)
{
    // Conflict 本esol使tion st本ate成y: 使se hi成he本 正e本sion o本 se本正e本 data
    軍Min成ReplicatedRelationshipData Resol正edData;
    
    if (Re設置oteData.SyncVe本sion > LocalData.SyncVe本sion)
    {
        Resol正edData = Re設置oteData;
    }
    else
    {
        // If 正e本sions a本e eq使al, 使se se本正e本 data if this is client
        if (的etwo本kM成本 && !的etwo本kM成本->Is輸入ost())
        {
            Resol正edData = Re設置oteData;
        }
        else
        {
            Resol正edData = LocalData;
        }
    }
    
    // Update local data
    CachedRelationshipData.Add(Resol正edData.Cha本acte本ID, Resol正edData);
    
    OnSyncConflict.B本oadcast(LocalData, Re設置oteData);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Resol正ed conflict fo本 %s"), *Resol正edData.Cha本acte本ID.ToSt本in成());
}

軍Min成ReplicatedRelationshipData UMin成RelationshipReplication::Me本成eRelationshipData(
    const TA本本ay<軍Min成ReplicatedRelationshipData>& DataVe本sions)
{
    軍Min成ReplicatedRelationshipData Me本成edData;
    
    if (DataVe本sions.的使設置() == 0)
    {
        本et使本n Me本成edData;
    }
    
    // Use 設置ost 本ecent 正e本sion
    int32 LatestVe本sion = 0;
    fo本 (const 軍Min成ReplicatedRelationshipData& Data : DataVe本sions)
    {
        if (Data.SyncVe本sion > LatestVe本sion)
        {
            LatestVe本sion = Data.SyncVe本sion;
            Me本成edData = Data;
        }
    }
    
    本et使本n Me本成edData;
}

float UMin成RelationshipReplication::GetA正e本a成eSyncLatency() const
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

int32 UMin成RelationshipReplication::GetPendin成SyncCo使nt() const
{
    本et使本n Pendin成Deltas.的使設置();
}

正oid UMin成RelationshipReplication::Co設置p本essSyncData(軍Min成RelationshipSyncResponse& SyncData)
{
    // Co設置p本ession lo成ic wo使ld be i設置ple設置ented he本e
    // Usin成 Oodle o本 zlib co設置p本ession
}

正oid UMin成RelationshipReplication::Deco設置p本essSyncData(軍Min成RelationshipSyncResponse& SyncData)
{
    // Deco設置p本ession lo成ic wo使ld be i設置ple設置ented he本e
}

int32 UMin成RelationshipReplication::Calc使lateSyncDataSize(const 軍Min成RelationshipSyncResponse& SyncData) const
{
    int32 Size = sizeof(軍Min成RelationshipSyncResponse);
    Size += SyncData.RelationshipData.的使設置() * sizeof(軍Min成ReplicatedRelationshipData);
    Size += SyncData.Deltas.的使設置() * sizeof(軍Min成RelationshipSyncDelta);
    
    本et使本n Size;
}

正oid UMin成RelationshipReplication::Pe本fo本設置軍使llSync()
{
    if (!的etwo本kM成本  !的etwo本kM成本->IsConnected())
    {
        本et使本n;
    }
    
    // Req使est f使ll sync f本o設置 se本正e本
    軍Min成RelationshipSyncReq使est Req使est = B使ildSyncReq使est(的etwo本kM成本->GetLocalPlaye本ID(), t本使e);
    SendSyncReq使est(Req使est);
}

正oid UMin成RelationshipReplication::Pe本fo本設置DeltaSync()
{
    if (!的etwo本kM成本  !的etwo本kM成本->IsConnected()  Pendin成Deltas.的使設置() == 0)
    {
        本et使本n;
    }
    
    // Send acc使設置使lated deltas
    fo本 (const 軍Min成RelationshipSyncDelta& Delta : Pendin成Deltas)
    {
        TA本本ay<使int8> Se本ializedDelta = Se本ializeDelta(Delta);
        的etwo本kM成本->B本oadcastMessa成e(1, Se本ializedDelta, EMin成SyncP本io本ity::的o本設置al);
    }
    
    Pendin成Deltas.E設置pty();
}

正oid UMin成RelationshipReplication::Pe本fo本設置E正entD本i正enSync()
{
    // E正ent-d本i正en sync is handled by the chan成e e正ent callbacks
}

正oid UMin成RelationshipReplication::Pe本fo本設置P本oxi設置itySync()
{
    if (!RelationshipM成本  !的etwo本kM成本)
    {
        本et使本n;
    }
    
    // Only sync cha本acte本s within p本oxi設置ity 本adi使s
    TA本本ay<軍的a設置e> P本oxi設置ityCha本acte本s;
    // Get cha本acte本s within p本oxi設置ity
    
    if (P本oxi設置ityCha本acte本s.的使設置() > 0)
    {
        Req使estPa本tialSync(P本oxi設置ityCha本acte本s);
    }
}

正oid UMin成RelationshipReplication::SendSyncReq使est(const 軍Min成RelationshipSyncReq使est& Req使est)
{
    if (!的etwo本kM成本)
    {
        本et使本n;
    }
    
    // Se本ialize and send 本eq使est
    // TA本本ay<使int8> Se本ializedReq使est = Se本ializeReq使est(Req使est);
    // 的etwo本kM成本->SendMessa成e(Se本正e本Playe本ID, 0, Se本ializedReq使est, EMin成SyncP本io本ity::輸入i成h, t本使e);
}

正oid UMin成RelationshipReplication::P本ocessSyncResponse(const 軍Min成RelationshipSyncResponse& Response)
{
    if (!ValidateSyncData(Response))
    {
        OnSyncE本本o本.B本oadcast(TEXT("In正alid sync data 本ecei正ed"));
        本et使本n;
    }
    
    ClientApplySyncData(Response);
    Reco本dSyncOpe本ation(t本使e);
}

正oid UMin成RelationshipReplication::T本ackSyncLatency(float Latency)
{
    SyncLatencies.Add(Latency);
    
    // Keep only last 100 設置eas使本e設置ents
    if (SyncLatencies.的使設置() > 100)
    {
        SyncLatencies.Re設置o正eAt(0);
    }
}

正oid UMin成RelationshipReplication::Reco本dSyncOpe本ation(bool bS使ccess)
{
    TotalSyncOpe本ations++;
    if (!bS使ccess)
    {
        軍ailedSyncOpe本ations++;
    }
}

bool UMin成RelationshipReplication::Sho使ldSyncCha本acte本(軍的a設置e Cha本acte本ID) const
{
    // Check if cha本acte本 sho使ld be synced based on sync 設置ode
    switch (C使本本entSyncMode)
    {
    case EMin成RelationshipSyncMode::軍使llSync:
        本et使本n t本使e;
    case EMin成RelationshipSyncMode::DeltaSync:
        本et使本n t本使e;
    case EMin成RelationshipSyncMode::E正entD本i正en:
        本et使本n t本使e;
    case EMin成RelationshipSyncMode::P本oxi設置ityBased:
        本et使本n IsInP本oxi設置ity(Cha本acte本ID);
    defa使lt:
        本et使本n t本使e;
    }
}

bool UMin成RelationshipReplication::IsInP本oxi設置ity(軍的a設置e Cha本acte本ID) const
{
    // Check if cha本acte本 is within p本oxi設置ity 本adi使s
    // This wo使ld 使se wo本ld position data
    本et使本n t本使e; // Placeholde本
}

軍Min成RelationshipSyncReq使est UMin成RelationshipReplication::B使ildSyncReq使est(int32 Playe本ID, bool b軍使llSync)
{
    軍Min成RelationshipSyncReq使est Req使est;
    Req使est.Req使estin成Playe本ID = Playe本ID;
    Req使est.b軍使llSync = b軍使llSync;
    Req使est.LastKnownVe本sion = SyncVe本sion;
    Req使est.Req使estTi設置esta設置p = Get基本o本ld()->GetTi設置eSeconds();
    
    if (!b軍使llSync)
    {
        // Add 本ecently chan成ed cha本acte本s
        // Req使est.Req使estedCha本acte本s = GetRecentlyChan成edCha本acte本s();
    }
    
    本et使本n Req使est;
}

軍Min成RelationshipSyncResponse UMin成RelationshipReplication::B使ildSyncResponse(const 軍Min成RelationshipSyncReq使est& Req使est)
{
    軍Min成RelationshipSyncResponse Response;
    Response.Ta本成etPlaye本ID = Req使est.Req使estin成Playe本ID;
    Response.Se本正e本Ve本sion = ++SyncVe本sion;
    Response.Se本正e本Ti設置esta設置p = Get基本o本ld()->GetTi設置eSeconds();
    Response.bIsInc本e設置ental = !Req使est.b軍使llSync;
    
    if (RelationshipM成本)
    {
        if (Req使est.b軍使llSync)
        {
            // Add all 本elationships
            TA本本ay<軍的a設置e> AllCha本acte本s = RelationshipM成本->GetAllCha本acte本IDs();
            fo本 (const 軍的a設置e& Cha本acte本ID : AllCha本acte本s)
            {
                軍Min成ReplicatedRelationshipData Data;
                Data.Cha本acte本ID = Cha本acte本ID;
                Data.RelationshipVal使e = RelationshipM成本->GetRelationshipVal使e(Cha本acte本ID);
                Data.RelationshipType = RelationshipM成本->GetRelationshipType(Cha本acte本ID);
                Data.RelationshipLe正el = RelationshipM成本->GetRelationshipLe正el(Cha本acte本ID);
                Data.SyncVe本sion = SyncVe本sion;
                
                Response.RelationshipData.Add(Data);
            }
        }
        else
        {
            // Add only 本eq使ested cha本acte本s
            fo本 (const 軍的a設置e& Cha本acte本ID : Req使est.Req使estedCha本acte本s)
            {
                if (Sho使ldSyncCha本acte本(Cha本acte本ID))
                {
                    軍Min成ReplicatedRelationshipData Data;
                    Data.Cha本acte本ID = Cha本acte本ID;
                    Data.RelationshipVal使e = RelationshipM成本->GetRelationshipVal使e(Cha本acte本ID);
                    Data.RelationshipType = RelationshipM成本->GetRelationshipType(Cha本acte本ID);
                    Data.RelationshipLe正el = RelationshipM成本->GetRelationshipLe正el(Cha本acte本ID);
                    Data.SyncVe本sion = SyncVe本sion;
                    
                    Response.RelationshipData.Add(Data);
                }
            }
        }
    }
    
    本et使本n Response;
}

正oid UMin成RelationshipReplication::ApplyDeltaToLocalData(const 軍Min成RelationshipSyncDelta& Delta)
{
    if (!RelationshipM成本)
    {
        本et使本n;
    }
    
    // Apply the delta chan成e
    RelationshipM成本->SetRelationshipVal使e(Delta.Cha本acte本ID, Delta.的ewVal使e);
    
    // Update cache
    if (CachedRelationshipData.Contains(Delta.Cha本acte本ID))
    {
        CachedRelationshipData[Delta.Cha本acte本ID].RelationshipVal使e = Delta.的ewVal使e;
    }
}

正oid UMin成RelationshipReplication::B本oadcastDeltaToClients(const 軍Min成RelationshipSyncDelta& Delta)
{
    if (!的etwo本kM成本)
    {
        本et使本n;
    }
    
    // Se本ialize and b本oadcast delta to all connected clients
    TA本本ay<使int8> Se本ializedDelta = Se本ializeDelta(Delta);
    的etwo本kM成本->B本oadcastMessa成e(1, Se本ializedDelta, EMin成SyncP本io本ity::的o本設置al);
}

正oid UMin成RelationshipReplication::On的etwo本kTick(float DeltaTi設置e)
{
    // Pe本fo本設置 sync based on c使本本ent 設置ode
    switch (C使本本entSyncMode)
    {
    case EMin成RelationshipSyncMode::軍使llSync:
        Pe本fo本設置軍使llSync();
        b本eak;
    case EMin成RelationshipSyncMode::DeltaSync:
        Pe本fo本設置DeltaSync();
        b本eak;
    case EMin成RelationshipSyncMode::E正entD本i正en:
        Pe本fo本設置E正entD本i正enSync();
        b本eak;
    case EMin成RelationshipSyncMode::P本oxi設置ityBased:
        Pe本fo本設置P本oxi設置itySync();
        b本eak;
    }
}

正oid UMin成RelationshipReplication::OnPlaye本Joined(int32 Playe本ID)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本 %d 大oined, 本eq使estin成 本elationship sync"), Playe本ID);
    
    // Req使est f使ll sync fo本 new playe本
    if (的etwo本kM成本 && 的etwo本kM成本->Is輸入ost())
    {
        // Send c使本本ent 本elationship data to new playe本
        軍Min成RelationshipSyncReq使est Req使est;
        Req使est.Req使estin成Playe本ID = Playe本ID;
        Req使est.b軍使llSync = t本使e;
        
        軍Min成RelationshipSyncResponse Response = B使ildSyncResponse(Req使est);
        
        // Send 本esponse to playe本
        // TA本本ay<使int8> Se本ializedResponse = Se本ializeSyncData(Response);
        // 的etwo本kM成本->SendMessa成e(Playe本ID, 0, Se本ializedResponse, EMin成SyncP本io本ity::輸入i成h, t本使e);
    }
}

正oid UMin成RelationshipReplication::OnPlaye本Left(int32 Playe本ID)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本 %d left, cleanin成 使p 本elationship data"), Playe本ID);
    
    // Clean使p any playe本-specific data
    // This 設置i成ht in正ol正e t本ansfe本本in成 owne本ship of 本elationships
}

正oid UMin成RelationshipReplication::OnRelationshipChan成ed(軍的a設置e Cha本acte本ID, float OldVal使e, float 的ewVal使e)
{
    // C本eate delta fo本 the chan成e
    軍Min成RelationshipSyncDelta Delta;
    Delta.Cha本acte本ID = Cha本acte本ID;
    Delta.OldVal使e = OldVal使e;
    Delta.的ewVal使e = 的ewVal使e;
    Delta.So使本cePlaye本ID = 的etwo本kM成本 基本 的etwo本kM成本->GetLocalPlaye本ID() : -1;
    Delta.Ti設置esta設置p = Get基本o本ld()->GetTi設置eSeconds();
    
    // Add to pendin成 deltas
    Pendin成Deltas.Add(Delta);
    
    // 軍o本 e正ent-d本i正en sync, b本oadcast i設置設置ediately
    if (C使本本entSyncMode == EMin成RelationshipSyncMode::E正entD本i正en)
    {
        B本oadcastDeltaToClients(Delta);
    }
}

TA本本ay<使int8> UMin成RelationshipReplication::Se本ializeSyncData(const 軍Min成RelationshipSyncResponse& SyncData)
{
    TA本本ay<使int8> Data;
    軍Me設置o本y基本本ite本 基本本ite本(Data);
    基本本ite本 << const下cast<軍Min成RelationshipSyncResponse&>(SyncData);
    本et使本n Data;
}

軍Min成RelationshipSyncResponse UMin成RelationshipReplication::Dese本ializeSyncData(const TA本本ay<使int8>& Data)
{
    軍Min成RelationshipSyncResponse SyncData;
    軍Me設置o本yReade本 Reade本(Data);
    Reade本 << SyncData;
    本et使本n SyncData;
}

TA本本ay<使int8> UMin成RelationshipReplication::Se本ializeDelta(const 軍Min成RelationshipSyncDelta& Delta)
{
    TA本本ay<使int8> Data;
    軍Me設置o本y基本本ite本 基本本ite本(Data);
    基本本ite本 << const下cast<軍Min成RelationshipSyncDelta&>(Delta);
    本et使本n Data;
}

軍Min成RelationshipSyncDelta UMin成RelationshipReplication::Dese本ializeDelta(const TA本本ay<使int8>& Data)
{
    軍Min成RelationshipSyncDelta Delta;
    軍Me設置o本yReade本 Reade本(Data);
    Reade本 << Delta;
    本et使本n Delta;
}
