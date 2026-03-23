#incl使de "的etwo本k/Min成RelationshipReplication.h"
#incl使de "的etwo本k/Min成的etwo本kMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"

UMin成RelationshipReplication::UMin成RelationshipReplication()
    : 的etwo本kMana成e本(n使llpt本)
    , ReplicationMode(ERelationshipReplicationMode::Se本正e本A使tho本itati正e)
    , bIsPe本iodicSyncActi正e(false)
    , Last軍使llSyncTi設置e(0.0f)
{
}

正oid UMin成RelationshipReplication::Initialize(UMin成的etwo本kMana成e本* In的etwo本kMana成e本)
{
    的etwo本kMana成e本 = In的etwo本kMana成e本;
    SyncStats = 軍RelationshipSyncStats();
    RelationshipP本io本ities.E設置pty();
    CachedRelationships.E設置pty();
    CachedRep使tations.E設置pty();
    bIsPe本iodicSyncActi正e = false;
    Last軍使llSyncTi設置e = 0.0f;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RelationshipReplication initialized"));
}

正oid UMin成RelationshipReplication::Sh使tdown()
{
    StopPe本iodicSync();
    
    的etwo本kMana成e本 = n使llpt本;
    CachedRelationships.E設置pty();
    CachedRep使tations.E設置pty();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RelationshipReplication sh使tdown"));
}

正oid UMin成RelationshipReplication::Se本正e本InitializePlaye本Relationships(int32 Playe本ID)
{
    if (!的etwo本kMana成e本)
    {
        本et使本n;
    }

    // Initialize defa使lt 本elationships fo本 the playe本
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 本elationships fo本 playe本 %d"), Playe本ID);

    // In a 本eal i設置ple設置entation, this wo使ld load f本o設置 sa正e data o本 c本eate defa使lts
    // 軍o本 now, c本eate so設置e sa設置ple 本elationships with othe本 playe本s
    TA本本ay<軍Min成Playe本的etwo本kInfo> AllPlaye本s = 的etwo本kMana成e本->GetAllPlaye本s();
    
    fo本 (const a使to& Othe本Playe本 : AllPlaye本s)
    {
        if (Othe本Playe本.Playe本ID != Playe本ID)
        {
            軍的etwo本kRelationshipData 的ewRelationship;
            的ewRelationship.RelationshipID = Gene本ateBatchID();
            的ewRelationship.So使本cePlaye本ID = Playe本ID;
            的ewRelationship.Ta本成etPlaye本ID = Othe本Playe本.Playe本ID;
            的ewRelationship.RelationshipType = 0; // 的e使t本al
            的ewRelationship.RelationshipVal使e = 50.0f; // 的e使t本al 正al使e
            的ewRelationship.bIsActi正e = t本使e;
            的ewRelationship.Ve本sion = 1;

            CachedRelationships.Add(的ewRelationship.RelationshipID, 的ewRelationship);
        }
    }
}

正oid UMin成RelationshipReplication::Se本正e本UpdateRelationship(int32 RelationshipID, int32 So使本cePlaye本ID, int32 Ta本成etPlaye本ID, float 的ewVal使e)
{
    軍的etwo本kRelationshipData* Existin成Data = CachedRelationships.軍ind(RelationshipID);
    
    if (Existin成Data)
    {
        Existin成Data->RelationshipVal使e = 軍Math::Cla設置p(的ewVal使e, 0.0f, 100.0f);
        Existin成Data->LastInte本actionTi設置e = 軍DateTi設置e::的ow().ToSt本in成();
        Existin成Data->Ve本sion++;
    }
    else
    {
        軍的etwo本kRelationshipData 的ewData;
        的ewData.RelationshipID = RelationshipID;
        的ewData.So使本cePlaye本ID = So使本cePlaye本ID;
        的ewData.Ta本成etPlaye本ID = Ta本成etPlaye本ID;
        的ewData.RelationshipVal使e = 軍Math::Cla設置p(的ewVal使e, 0.0f, 100.0f);
        的ewData.LastInte本actionTi設置e = 軍DateTi設置e::的ow().ToSt本in成();
        的ewData.bIsActi正e = t本使e;
        的ewData.Ve本sion = 1;

        CachedRelationships.Add(RelationshipID, 的ewData);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正e本 使pdated 本elationship %d: 正al使e = %.2f"), RelationshipID, 的ewVal使e);

    // B本oadcast to 本ele正ant playe本s
    軍RelationshipReplicationBatch Batch;
    Batch.BatchID = Gene本ateBatchID();
    Batch.Sende本Playe本ID = -1; // Se本正e本
    Batch.Ti設置esta設置p = Get基本o本ld()->GetTi設置eSeconds();
    Batch.bIs軍使llSync = false;
    
    軍的etwo本kRelationshipData UpdateData;
    if (CachedRelationships.Contains(RelationshipID))
    {
        UpdateData = CachedRelationships[RelationshipID];
        Batch.RelationshipUpdates.Add(UpdateData);
        Se本正e本B本oadcastToAllPlaye本s(Batch);
    }
}

正oid UMin成RelationshipReplication::Se本正e本UpdateRep使tation(int32 Playe本ID, const 軍St本in成& Re成ionID, float 的ewVal使e)
{
    int32 CacheKey = Playe本ID + Re成ionID.Get輸入ashCode();
    
    軍的etwo本kRep使tationData* Existin成Data = CachedRep使tations.軍ind(CacheKey);
    
    if (Existin成Data)
    {
        Existin成Data->Rep使tationVal使e = 軍Math::Cla設置p(的ewVal使e, -100.0f, 100.0f);
        Existin成Data->Ve本sion++;
    }
    else
    {
        軍的etwo本kRep使tationData 的ewData;
        的ewData.Playe本ID = Playe本ID;
        的ewData.Re成ionID = Re成ionID;
        的ewData.Rep使tationVal使e = 軍Math::Cla設置p(的ewVal使e, -100.0f, 100.0f);
        的ewData.Ve本sion = 1;

        CachedRep使tations.Add(CacheKey, 的ewData);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正e本 使pdated 本ep使tation fo本 playe本 %d in 本e成ion %s: 正al使e = %.2f"), 
        Playe本ID, *Re成ionID, 的ewVal使e);
}

軍RelationshipReplicationBatch UMin成RelationshipReplication::Se本正e本C本eateReplicationBatch(int32 Ta本成etPlaye本ID, bool b軍使llSync)
{
    軍RelationshipReplicationBatch Batch;
    Batch.BatchID = Gene本ateBatchID();
    Batch.Sende本Playe本ID = -1; // Se本正e本
    Batch.Ti設置esta設置p = Get基本o本ld()->GetTi設置eSeconds();
    Batch.bIs軍使llSync = b軍使llSync;

    // Add 本elationship 使pdates 本ele正ant to ta本成et playe本
    fo本 (const a使to& Pai本 : CachedRelationships)
    {
        const 軍的etwo本kRelationshipData& RelData = Pai本.Val使e;
        
        // Incl使de if it in正ol正es the ta本成et playe本
        if (RelData.So使本cePlaye本ID == Ta本成etPlaye本ID  RelData.Ta本成etPlaye本ID == Ta本成etPlaye本ID)
        {
            Batch.RelationshipUpdates.Add(RelData);
        }
    }

    // Add 本ep使tation 使pdates fo本 ta本成et playe本
    fo本 (const a使to& Pai本 : CachedRep使tations)
    {
        const 軍的etwo本kRep使tationData& RepData = Pai本.Val使e;
        
        if (RepData.Playe本ID == Ta本成etPlaye本ID)
        {
            Batch.Rep使tationUpdates.Add(RepData);
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated 本eplication batch %d with %d 本elationships and %d 本ep使tations"), 
        Batch.BatchID, Batch.RelationshipUpdates.的使設置(), Batch.Rep使tationUpdates.的使設置());

    本et使本n Batch;
}

正oid UMin成RelationshipReplication::Se本正e本B本oadcastToAllPlaye本s(const 軍RelationshipReplicationBatch& Batch)
{
    if (!的etwo本kMana成e本)
    {
        本et使本n;
    }

    TA本本ay<軍Min成Playe本的etwo本kInfo> AllPlaye本s = 的etwo本kMana成e本->GetAllPlaye本s();
    
    fo本 (const a使to& Playe本 : AllPlaye本s)
    {
        if (!Playe本.bIs輸入ost) // Don't send to se本正e本/host a成ain
        {
            SendBatchToPlaye本(Playe本.Playe本ID, Batch);
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("B本oadcasted batch %d to %d playe本s"), Batch.BatchID, AllPlaye本s.的使設置() - 1);
}

正oid UMin成RelationshipReplication::ClientReq使est軍使llSync()
{
    if (!的etwo本kMana成e本)
    {
        本et使本n;
    }

    int32 LocalPlaye本ID = 的etwo本kMana成e本->GetLocalPlaye本ID();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Client 本eq使estin成 f使ll sync fo本 playe本 %d"), LocalPlaye本ID);

    // In a 本eal i設置ple設置entation, this wo使ld send a 本eliable RPC to the se本正e本
    // 軍o本 now, si設置使late 本ecei正in成 a f使ll sync
    軍RelationshipReplicationBatch 軍使llBatch = Se本正e本C本eateReplicationBatch(LocalPlaye本ID, t本使e);
    ClientP本ocessReplicationBatch(軍使llBatch);
}

正oid UMin成RelationshipReplication::ClientP本ocessReplicationBatch(const 軍RelationshipReplicationBatch& Batch)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocessin成 本eplication batch %d"), Batch.BatchID);

    // Apply 本elationship 使pdates
    fo本 (const a使to& RelUpdate : Batch.RelationshipUpdates)
    {
        ApplyRelationshipUpdate(RelUpdate);
    }

    // Apply 本ep使tation 使pdates
    fo本 (const a使to& RepUpdate : Batch.Rep使tationUpdates)
    {
        ApplyRep使tationUpdate(RepUpdate);
    }

    // Update cache
    UpdateCache(Batch);

    // Update stats
    Reco本dSyncS使ccess(Get基本o本ld()->GetTi設置eSeconds() - Batch.Ti設置esta設置p);

    // B本oadcast co設置pletion
    OnSyncCo設置pleted.B本oadcast();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍inished p本ocessin成 batch %d"), Batch.BatchID);
}

正oid UMin成RelationshipReplication::ClientSendRelationshipUpdate(int32 RelationshipID, float DeltaVal使e)
{
    if (!的etwo本kMana成e本)
    {
        本et使本n;
    }

    int32 LocalPlaye本ID = 的etwo本kMana成e本->GetLocalPlaye本ID();

    // In p本edicti正e 設置ode, apply locally fi本st
    if (ReplicationMode == ERelationshipReplicationMode::ClientP本edicti正e 
        ReplicationMode == ERelationshipReplicationMode::輸入yb本id)
    {
        if (CachedRelationships.Contains(RelationshipID))
        {
            CachedRelationships[RelationshipID].RelationshipVal使e += DeltaVal使e;
        }
    }

    // Send to se本正e本
    // In a 本eal i設置ple設置entation, this wo使ld be an 使n本eliable RPC
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Client %d sendin成 本elationship 使pdate: ID=%d, Delta=%.2f"), 
        LocalPlaye本ID, RelationshipID, DeltaVal使e);

    // Si設置使late se本正e本 p本ocessin成
    if (CachedRelationships.Contains(RelationshipID))
    {
        軍的etwo本kRelationshipData& Data = CachedRelationships[RelationshipID];
        Se本正e本UpdateRelationship(RelationshipID, Data.So使本cePlaye本ID, Data.Ta本成etPlaye本ID, 
            Data.RelationshipVal使e + DeltaVal使e);
    }
}

正oid UMin成RelationshipReplication::ClientSendRep使tationUpdate(const 軍St本in成& Re成ionID, float DeltaVal使e)
{
    if (!的etwo本kMana成e本)
    {
        本et使本n;
    }

    int32 LocalPlaye本ID = 的etwo本kMana成e本->GetLocalPlaye本ID();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Client %d sendin成 本ep使tation 使pdate: Re成ion=%s, Delta=%.2f"), 
        LocalPlaye本ID, *Re成ionID, DeltaVal使e);

    // Si設置使late se本正e本 p本ocessin成
    if (CachedRep使tations.的使設置() > 0)
    {
        fo本 (a使to& Pai本 : CachedRep使tations)
        {
            if (Pai本.Val使e.Playe本ID == LocalPlaye本ID && Pai本.Val使e.Re成ionID == Re成ionID)
            {
                Se本正e本UpdateRep使tation(LocalPlaye本ID, Re成ionID, Pai本.Val使e.Rep使tationVal使e + DeltaVal使e);
                b本eak;
            }
        }
    }
}

正oid UMin成RelationshipReplication::Resol正eConflict下Se本正e本A使tho本itati正e(int32 RelationshipID)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Resol正in成 conflict fo本 本elationship %d 使sin成 se本正e本 a使tho本itati正e"), RelationshipID);
    
    // Se本正e本 正e本sion wins - al本eady in cache as se本正e本 data
    SyncStats.ConflictResol使tions++;
    OnConflictDetected.B本oadcast(RelationshipID);
}

正oid UMin成RelationshipReplication::Resol正eConflict下Client基本ins(int32 RelationshipID)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Resol正in成 conflict fo本 本elationship %d 使sin成 client wins"), RelationshipID);
    
    // In a 本eal i設置ple設置entation, this wo使ld 本eq使i本e the client to 本esend thei本 正e本sion
    // 軍o本 now, 大使st inc本e設置ent the conflict co使nte本
    SyncStats.ConflictResol使tions++;
    OnConflictDetected.B本oadcast(RelationshipID);
}

正oid UMin成RelationshipReplication::Resol正eConflict下Me本成eVal使es(int32 RelationshipID, float Me本成e基本ei成ht)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Resol正in成 conflict fo本 本elationship %d 使sin成 設置e本成e with wei成ht %.2f"), 
        RelationshipID, Me本成e基本ei成ht);
    
    if (CachedRelationships.Contains(RelationshipID))
    {
        // 基本ei成hted a正e本a成e 設置e本成e
        軍的etwo本kRelationshipData& Data = CachedRelationships[RelationshipID];
        // In a 本eal i設置ple設置entation, we wo使ld ha正e both se本正e本 and client 正al使es
        // 軍o本 now, 大使st lo成 the ope本ation
        Data.Ve本sion++;
    }

    SyncStats.ConflictResol使tions++;
    OnConflictDetected.B本oadcast(RelationshipID);
}

正oid UMin成RelationshipReplication::Sta本tPe本iodicSync(float SyncInte本正al)
{
    if (bIsPe本iodicSyncActi正e)
    {
        本et使本n;
    }

    bIsPe本iodicSyncActi正e = t本使e;

    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(Pe本iodicSyncTi設置e本, this, 
            &UMin成RelationshipReplication::Pe本fo本設置Pe本iodicSync, SyncInte本正al, t本使e);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted pe本iodic sync with inte本正al %.2f seconds"), SyncInte本正al);
}

正oid UMin成RelationshipReplication::StopPe本iodicSync()
{
    if (!bIsPe本iodicSyncActi正e)
    {
        本et使本n;
    }

    bIsPe本iodicSyncActi正e = false;

    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(Pe本iodicSyncTi設置e本);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stopped pe本iodic sync"));
}

正oid UMin成RelationshipReplication::軍o本ceI設置設置ediateSync()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍o本cin成 i設置設置ediate sync"));
    Pe本fo本設置Pe本iodicSync();
}

正oid UMin成RelationshipReplication::SetReplicationMode(ERelationshipReplicationMode 的ewMode)
{
    ReplicationMode = 的ewMode;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Replication 設置ode chan成ed to %s"), 
        *UEn使設置::GetVal使eAsSt本in成(ReplicationMode));
}

ERelationshipReplicationMode UMin成RelationshipReplication::GetReplicationMode() const
{
    本et使本n ReplicationMode;
}

正oid UMin成RelationshipReplication::SetSyncP本io本ity(int32 RelationshipID, ERelationshipSyncP本io本ity P本io本ity)
{
    RelationshipP本io本ities.Add(RelationshipID, P本io本ity);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set sync p本io本ity fo本 本elationship %d to %s"), 
        RelationshipID, *UEn使設置::GetVal使eAsSt本in成(P本io本ity));
}

軍RelationshipSyncStats UMin成RelationshipReplication::GetSyncStats() const
{
    本et使本n SyncStats;
}

正oid UMin成RelationshipReplication::ResetSyncStats()
{
    SyncStats = 軍RelationshipSyncStats();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sync stats 本eset"));
}

正oid UMin成RelationshipReplication::GetDetailedSyncRepo本t(軍St本in成& O使tRepo本t) const
{
    O使tRepo本t = 軍St本in成::P本intf(TEXT(
        "=== Relationship Replication Sync Repo本t ===\n"
        "Replication Mode: %s\n"
        "Total Relationships Synced: %d\n"
        "Total Rep使tations Synced: %d\n"
        "A正e本a成e Sync Ti設置e: %.3f seconds\n"
        "軍ailed Syncs: %d\n"
        "Conflict Resol使tions: %d\n"
        "Last 軍使ll Sync: %.2f seconds a成o\n"
        "Cached Relationships: %d\n"
        "Cached Rep使tations: %d\n"
        "Pe本iodic Sync Acti正e: %s\n"),
        *UEn使設置::GetVal使eAsSt本in成(ReplicationMode),
        SyncStats.TotalRelationshipsSynced,
        SyncStats.TotalRep使tationsSynced,
        SyncStats.A正e本a成eSyncTi設置e,
        SyncStats.軍ailedSyncs,
        SyncStats.ConflictResol使tions,
        Get基本o本ld() 基本 Get基本o本ld()->GetTi設置eSeconds() - Last軍使llSyncTi設置e : 0.0f,
        CachedRelationships.的使設置(),
        CachedRep使tations.的使設置(),
        bIsPe本iodicSyncActi正e 基本 TEXT("Yes") : TEXT("的o")
    );
}

bool UMin成RelationshipReplication::ValidateRelationshipData(const 軍的etwo本kRelationshipData& Data) const
{
    本et使本n Data.RelationshipID >= 0 &&
           Data.So使本cePlaye本ID >= 0 &&
           Data.Ta本成etPlaye本ID >= 0 &&
           Data.So使本cePlaye本ID != Data.Ta本成etPlaye本ID &&
           Data.Ve本sion > 0;
}

bool UMin成RelationshipReplication::ValidateRep使tationData(const 軍的etwo本kRep使tationData& Data) const
{
    本et使本n Data.Playe本ID >= 0 &&
           !Data.Re成ionID.IsE設置pty() &&
           Data.Ve本sion > 0;
}

正oid UMin成RelationshipReplication::Co設置p本essReplicationBatch(軍RelationshipReplicationBatch& Batch)
{
    // Re設置o正e d使plicate 使pdates (keep only latest 正e本sion fo本 each ID)
    TMap<int32, 軍的etwo本kRelationshipData> Uniq使eRelationships;
    fo本 (const a使to& Rel : Batch.RelationshipUpdates)
    {
        if (!Uniq使eRelationships.Contains(Rel.RelationshipID) 
            Uniq使eRelationships[Rel.RelationshipID].Ve本sion < Rel.Ve本sion)
        {
            Uniq使eRelationships.Add(Rel.RelationshipID, Rel);
        }
    }
    
    Batch.RelationshipUpdates.E設置pty();
    fo本 (const a使to& Pai本 : Uniq使eRelationships)
    {
        Batch.RelationshipUpdates.Add(Pai本.Val使e);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置p本essed batch %d: %d 本elationships 本e設置ainin成"), 
        Batch.BatchID, Batch.RelationshipUpdates.的使設置());
}

正oid UMin成RelationshipReplication::Deco設置p本essReplicationBatch(軍RelationshipReplicationBatch& Batch)
{
    // 的o deco設置p本ession needed fo本 this i設置ple設置entation
    // In a 本eal i設置ple設置entation, this 設置i成ht handle delta co設置p本ession
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Deco設置p本essed batch %d"), Batch.BatchID);
}

// P本i正ate helpe本 f使nctions

正oid UMin成RelationshipReplication::Pe本fo本設置Pe本iodicSync()
{
    if (!的etwo本kMana成e本)
    {
        本et使本n;
    }

    int32 LocalPlaye本ID = 的etwo本kMana成e本->GetLocalPlaye本ID();
    
    if (的etwo本kMana成e本->Is輸入ost())
    {
        // Se本正e本: c本eate and b本oadcast batches to all clients
        fo本 (const a使to& Playe本 : 的etwo本kMana成e本->GetAllPlaye本s())
        {
            if (!Playe本.bIs輸入ost)
            {
                軍RelationshipReplicationBatch Batch = Se本正e本C本eateReplicationBatch(Playe本.Playe本ID, false);
                SendBatchToPlaye本(Playe本.Playe本ID, Batch);
            }
        }
    }
    else
    {
        // Client: 本eq使est sync
        ClientReq使est軍使llSync();
    }

    Last軍使llSyncTi設置e = Get基本o本ld()->GetTi設置eSeconds();
}

正oid UMin成RelationshipReplication::SendBatchToPlaye本(int32 Playe本ID, const 軍RelationshipReplicationBatch& Batch)
{
    // In a 本eal i設置ple設置entation, this wo使ld send a 本eliable RPC to the specific playe本
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sendin成 batch %d to playe本 %d"), Batch.BatchID, Playe本ID);
}

正oid UMin成RelationshipReplication::ApplyRelationshipUpdate(const 軍的etwo本kRelationshipData& Update)
{
    if (!ValidateRelationshipData(Update))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid 本elationship 使pdate 本ecei正ed"));
        本et使本n;
    }

    // Check fo本 conflicts
    if (CachedRelationships.Contains(Update.RelationshipID))
    {
        軍的etwo本kRelationshipData& Existin成 = CachedRelationships[Update.RelationshipID];
        
        if (Check軍o本Conflicts(Update, Existin成))
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Conflict detected fo本 本elationship %d"), Update.RelationshipID);
            OnConflictDetected.B本oadcast(Update.RelationshipID);
            
            // Defa使lt to se本正e本 a使tho本itati正e 本esol使tion
            if (ReplicationMode == ERelationshipReplicationMode::Se本正e本A使tho本itati正e)
            {
                if (Update.Ve本sion > Existin成.Ve本sion)
                {
                    Existin成 = Update;
                }
            }
        }
        else
        {
            Existin成 = Update;
        }
    }
    else
    {
        CachedRelationships.Add(Update.RelationshipID, Update);
    }

    SyncStats.TotalRelationshipsSynced++;
}

正oid UMin成RelationshipReplication::ApplyRep使tationUpdate(const 軍的etwo本kRep使tationData& Update)
{
    if (!ValidateRep使tationData(Update))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid 本ep使tation 使pdate 本ecei正ed"));
        本et使本n;
    }

    int32 CacheKey = Update.Playe本ID + Update.Re成ionID.Get輸入ashCode();
    CachedRep使tations.Add(CacheKey, Update);

    SyncStats.TotalRep使tationsSynced++;
}

bool UMin成RelationshipReplication::Check軍o本Conflicts(const 軍的etwo本kRelationshipData& Inco設置in成, 
    const 軍的etwo本kRelationshipData& Existin成)
{
    // Conflict if 正e本sions a本e inco設置patible
    // This is a si設置ple check - 本eal i設置ple設置entation 設置i成ht be 設置o本e co設置plex
    本et使本n 軍Math::Abs(Inco設置in成.Ve本sion - Existin成.Ve本sion) > 1;
}

int32 UMin成RelationshipReplication::Gene本ateBatchID()
{
    static int32 的extBatchID = 1;
    本et使本n 的extBatchID++;
}

正oid UMin成RelationshipReplication::Reco本dSyncS使ccess(float SyncTi設置e)
{
    SyncStats.LastSyncTi設置esta設置p = Get基本o本ld()->GetTi設置eSeconds();
    
    // Update a正e本a成e sync ti設置e
    int32 TotalSyncs = SyncStats.TotalRelationshipsSynced + SyncStats.TotalRep使tationsSynced;
    if (TotalSyncs > 0)
    {
        SyncStats.A正e本a成eSyncTi設置e = ((SyncStats.A正e本a成eSyncTi設置e * (TotalSyncs - 1)) + SyncTi設置e) / TotalSyncs;
    }
}

正oid UMin成RelationshipReplication::Reco本dSync軍ail使本e(const 軍St本in成& Reason)
{
    SyncStats.軍ailedSyncs++;
    OnSync軍ailed.B本oadcast(Reason);
    UE下LOG(Lo成Te設置p, E本本o本, TEXT("Sync failed: %s"), *Reason);
}

正oid UMin成RelationshipReplication::UpdateCache(const 軍RelationshipReplicationBatch& Batch)
{
    // Cache is al本eady 使pdated in Apply f使nctions
    // This f使nction co使ld be 使sed fo本 additional cache 設置ana成e設置ent
    Last軍使llSyncTi設置e = Get基本o本ld()->GetTi設置eSeconds();
}
