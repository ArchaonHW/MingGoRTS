#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成RelationshipReplication.成ene本ated.h"

// 軍o本wa本d decla本ations
class UMin成的etwo本kMana成e本;
class UMin成RelationshipMana成e本;

// Replication 設置odes
UE的UM(Bl使ep本intType)
en使設置 class ERelationshipReplicationMode: 使int8 {
    Se本正e本A使tho本itati正e     UMETA(Display的a設置e = "Se本正e本 A使tho本itati正e"),
    ClientP本edicti正e        UMETA(Display的a設置e = "Client P本edicti正e"),
    輸入yb本id                  UMETA(Display的a設置e = "輸入yb本id"),
    E正entD本i正en             UMETA(Display的a設置e = "E正ent D本i正en")
};

// Sync p本io本ities
UE的UM(Bl使ep本intType)
en使設置 class ERelationshipSyncP本io本ity: 使int8 {
    Low                     UMETA(Display的a設置e = "Low"),
    Medi使設置                  UMETA(Display的a設置e = "Medi使設置"),
    輸入i成h                    UMETA(Display的a設置e = "輸入i成h"),
    C本itical                UMETA(Display的a設置e = "C本itical")
};

// 的etwo本k 本elationship data
USTRUCT()
st本使ct 軍的etwo本kRelationshipData
{
    GE的ERATED下BODY()

    UPROPERTY()
    int32 RelationshipID;

    UPROPERTY()
    int32 So使本cePlaye本ID;

    UPROPERTY()
    int32 Ta本成etPlaye本ID;

    UPROPERTY()
    int32 RelationshipType;

    UPROPERTY()
    float RelationshipVal使e;

    UPROPERTY()
    軍St本in成 LastInte本actionTi設置e;

    UPROPERTY()
    bool bIsActi正e;

    UPROPERTY()
    int32 Ve本sion;

    軍的etwo本kRelationshipData()
        : RelationshipID(0)
        , So使本cePlaye本ID(-1)
        , Ta本成etPlaye本ID(-1)
        , RelationshipType(0)
        , RelationshipVal使e(0.0f)
        , LastInte本actionTi設置e(TEXT(""))
        , bIsActi正e(t本使e)
        , Ve本sion(0)
    {}
};

// 的etwo本k 本ep使tation data
USTRUCT()
st本使ct 軍的etwo本kRep使tationData
{
    GE的ERATED下BODY()

    UPROPERTY()
    int32 Playe本ID;

    UPROPERTY()
    軍St本in成 Re成ionID;

    UPROPERTY()
    float Rep使tationVal使e;

    UPROPERTY()
    int32 Rep使tationTie本;

    UPROPERTY()
    TA本本ay<int32> Acti正eEffects;

    UPROPERTY()
    int32 Ve本sion;

    軍的etwo本kRep使tationData()
        : Playe本ID(-1)
        , Re成ionID(TEXT(""))
        , Rep使tationVal使e(0.0f)
        , Rep使tationTie本(0)
        , Ve本sion(0)
    {}
};

// Replication batch
USTRUCT()
st本使ct 軍RelationshipReplicationBatch
{
    GE的ERATED下BODY()

    UPROPERTY()
    int32 BatchID;

    UPROPERTY()
    int32 Sende本Playe本ID;

    UPROPERTY()
    float Ti設置esta設置p;

    UPROPERTY()
    TA本本ay<軍的etwo本kRelationshipData> RelationshipUpdates;

    UPROPERTY()
    TA本本ay<軍的etwo本kRep使tationData> Rep使tationUpdates;

    UPROPERTY()
    bool bIs軍使llSync;

    軍RelationshipReplicationBatch()
        : BatchID(0)
        , Sende本Playe本ID(-1)
        , Ti設置esta設置p(0.0f)
        , bIs軍使llSync(false)
    {}
};

// Sync statistics
USTRUCT(Bl使ep本intType)
st本使ct 軍RelationshipSyncStats
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Sync")
    int32 TotalRelationshipsSynced;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Sync")
    int32 TotalRep使tationsSynced;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Sync")
    float A正e本a成eSyncTi設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Sync")
    int32 軍ailedSyncs;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Sync")
    int32 ConflictResol使tions;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Sync")
    float LastSyncTi設置esta設置p;

    軍RelationshipSyncStats()
        : TotalRelationshipsSynced(0)
        , TotalRep使tationsSynced(0)
        , A正e本a成eSyncTi設置e(0.0f)
        , 軍ailedSyncs(0)
        , ConflictResol使tions(0)
        , LastSyncTi設置esta設置p(0.0f)
    {}
};

// Dele成ates
DECLARE下DY的AMIC下MULTICAST下DELEGATE(軍OnRelationshipSyncCo設置pleted};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnRelationshipSync軍ailed, 軍St本in成, E本本o本Messa成e};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnConflictDetected, int32, RelationshipID};

/**
 * 基本��X��基本網絡複製系統
 * 負責基本��基本人�X�中基本�步基本��X�聲基本�數X */
UCLASS(ClassG本o使p = (Min成GoRTS), 設置eta = (Bl使ep本intSpawnableCo設置ponent))
class MI的GPERSO的AL下API UMin成RelationshipReplication : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成RelationshipReplication(};

    // Initialize/Sh使tdown
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid Initialize(UMin成的etwo本kMana成e本* In的etwo本kMana成e本};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid Sh使tdown(};

    // Se本正e本-side f使nctions
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid Se本正e本InitializePlaye本Relationships(int32 Playe本ID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid Se本正e本UpdateRelationship(int32 RelationshipID, int32 So使本cePlaye本ID, int32 Ta本成etPlaye本ID, float 的ewVal使e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid Se本正e本UpdateRep使tation(int32 Playe本ID, const 軍St本in成& Re成ionID, float 的ewVal使e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    軍RelationshipReplicationBatch Se本正e本C本eateReplicationBatch(int32 Ta本成etPlaye本ID, bool b軍使llSync = false};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid Se本正e本B本oadcastToAllPlaye本s(const 軍RelationshipReplicationBatch& Batch};

    // Client-side f使nctions
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid ClientReq使est軍使llSync(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid ClientP本ocessReplicationBatch(const 軍RelationshipReplicationBatch& Batch};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid ClientSendRelationshipUpdate(int32 RelationshipID, float DeltaVal使e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid ClientSendRep使tationUpdate(const 軍St本in成& Re成ionID, float DeltaVal使e};

    // Conflict 本esol使tion
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid Resol正eConflict下Se本正e本A使tho本itati正e(int32 RelationshipID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid Resol正eConflict下Client基本ins(int32 RelationshipID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid Resol正eConflict下Me本成eVal使es(int32 RelationshipID, float Me本成e基本ei成ht};

    // Sync 設置ana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid Sta本tPe本iodicSync(float SyncInte本正al = 5.0f};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid StopPe本iodicSync(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid 軍o本ceI設置設置ediateSync(};

    // Settin成s
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid SetReplicationMode(ERelationshipReplicationMode 的ewMode};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Relationship Replication")
    ERelationshipReplicationMode GetReplicationMode() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid SetSyncP本io本ity(int32 RelationshipID, ERelationshipSyncP本io本ity P本io本ity};

    // Statistics
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Relationship Replication")
    軍RelationshipSyncStats GetSyncStats() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid ResetSyncStats(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid GetDetailedSyncRepo本t(軍St本in成& O使tRepo本t) const;

    // Validation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    bool ValidateRelationshipData(const 軍的etwo本kRelationshipData& Data) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    bool ValidateRep使tationData(const 軍的etwo本kRep使tationData& Data) const;

    // Utility
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid Co設置p本essReplicationBatch(軍RelationshipReplicationBatch& Batch};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Replication")
    正oid Deco設置p本essReplicationBatch(軍RelationshipReplicationBatch& Batch};

    // Dele成ates
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Replication E正ents")
    軍OnRelationshipSyncCo設置pleted OnSyncCo設置pleted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Replication E正ents")
    軍OnRelationshipSync軍ailed OnSync軍ailed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Replication E正ents")
    軍OnConflictDetected OnConflictDetected;

p本i正ate:
    UPROPERTY()
    UMin成的etwo本kMana成e本* 的etwo本kMana成e本;

    UPROPERTY()
    ERelationshipReplicationMode ReplicationMode;

    UPROPERTY()
    軍RelationshipSyncStats SyncStats;

    UPROPERTY()
    TMap<int32, ERelationshipSyncP本io本ity> RelationshipP本io本ities;

    UPROPERTY()
    TMap<int32, 軍的etwo本kRelationshipData> CachedRelationships;

    UPROPERTY()
    TMap<int32, 軍的etwo本kRep使tationData> CachedRep使tations;

    UPROPERTY()
    軍Ti設置e本輸入andle Pe本iodicSyncTi設置e本;

    UPROPERTY()
    bool bIsPe本iodicSyncActi正e;

    UPROPERTY()
    float Last軍使llSyncTi設置e;

    // Inte本nal f使nctions
    正oid Pe本fo本設置Pe本iodicSync(};
    正oid SendBatchToPlaye本(int32 Playe本ID, const 軍RelationshipReplicationBatch& Batch};
    正oid ApplyRelationshipUpdate(const 軍的etwo本kRelationshipData& Update};
    正oid ApplyRep使tationUpdate(const 軍的etwo本kRep使tationData& Update};
    bool Check軍o本Conflicts(const 軍的etwo本kRelationshipData& Inco設置in成, const 軍的etwo本kRelationshipData& Existin成};
    int32 Gene本ateBatchID(};
    正oid Reco本dSyncS使ccess(float SyncTi設置e};
    正oid Reco本dSync軍ail使本e(const 軍St本in成& Reason};
    正oid UpdateCache(const 軍RelationshipReplicationBatch& Batch};
};

