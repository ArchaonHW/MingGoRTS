#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "S使bsyste設置s/Ga設置eInstanceS使bsyste設置.h"
#incl使de "Min成M使ltiplaye本Types.h"
#incl使de "Min成的etwo本kMana成e本.成ene本ated.h"








UCLASS(ClassG本o使p = (M使ltiplaye本, 的etwo本k))
class MI的GMULTIPLAYER下API UMin成的etwo本kMana成e本 : p使blic UGa設置eInstanceS使bsyste設置
{
    GE的ERATED下BODY()

p使blic:
    UMin成的etwo本kMana成e本();

    正i本t使al 正oid Initialize(軍S使bsyste設置CollectionBase& Collection) o正e本本ide;
    正i本t使al 正oid Deinitialize() o正e本本ide;
    正i本t使al 正oid Tick(float DeltaTi設置e) o正e本本ide;
    正i本t使al bool IsTickable() const o正e本本ide { 本et使本n t本使e; }
    正i本t使al TStatId GetStatId() const o正e本本ide;

    // Initialization
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid Initialize的etwo本k();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid Sh使tdown的etwo本k();

    // Connection Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    bool ConnectToSe本正e本(const 軍St本in成& Se本正e本Add本ess, int32 Po本t);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid Disconnect();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    bool C本eateSession(const 軍Min成的etwo本kSessionInfo& SessionInfo);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    bool JoinSession(const 軍St本in成& SessionID, const 軍St本in成& Passwo本d);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid Lea正eSession();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid Disco正e本Sessions();

    // Se本正e本 Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    bool Sta本tListenSe本正e本(int32 Po本t, int32 MaxPlaye本s);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    bool Sta本tDedicatedSe本正e本(int32 Po本t);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid StopSe本正e本();

    // Playe本 Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid Re成iste本Playe本(int32 Playe本ID, const 軍Min成Playe本的etwo本kInfo& Playe本Info);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid Un本e成iste本Playe本(int32 Playe本ID);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    軍Min成Playe本的etwo本kInfo GetPlaye本Info(int32 Playe本ID) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    TA本本ay<軍Min成Playe本的etwo本kInfo> GetAllPlaye本s() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    int32 GetLocalPlaye本ID() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    bool Is輸入ost() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    bool IsConnected() const;

    // Messa成e Sendin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    bool SendMessa成e(int32 Ta本成etPlaye本ID, int32 Messa成eType, const TA本本ay<使int8>& Data, 
        EMin成SyncP本io本ity P本io本ity = EMin成SyncP本io本ity::的o本設置al, bool bReliable = t本使e);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    bool B本oadcastMessa成e(int32 Messa成eType, const TA本本ay<使int8>& Data, 
        EMin成SyncP本io本ity P本io本ity = EMin成SyncP本io本ity::的o本設置al);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    bool SendRPC(class UOb大ect* Ta本成etOb大ect, const 軍St本in成& 軍使nction的a設置e, const TA本本ay<使int8>& Pa本a設置ete本s);

    // 的etwo本k State
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    EMin成的etwo本kRole Get的etwo本kRole() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    EMin成ConnectionState GetConnectionState() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid SetConnectionState(EMin成ConnectionState 的ewState);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    軍Min成的etwo本kStats Get的etwo本kStats() const;

    // Latency Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    float GetPlaye本Pin成(int32 Playe本ID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid SetTickRate(int32 TickRate);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    int32 GetTickRate() const;

    // Sec使本ity
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid SetSe本正e本Passwo本d(const 軍St本in成& Passwo本d);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    bool A使thenticatePlaye本(int32 Playe本ID, const 軍St本in成& A使thToken);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid KickPlaye本(int32 Playe本ID, const 軍St本in成& Reason);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid BanPlaye本(int32 Playe本ID, int32 BanD使本ationMin使tes, const 軍St本in成& Reason);

    // E正ents
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnPlaye本Connected OnPlaye本Connected;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnPlaye本Disconnected OnPlaye本Disconnected;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnConnectionStateChan成ed OnConnectionStateChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍On的etwo本kE本本o本 On的etwo本kE本本o本;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnSessionC本eated OnSessionC本eated;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnSessionJoined OnSessionJoined;

p本otected:
    UPROPERTY()
    EMin成的etwo本kRole C使本本entRole;

    UPROPERTY()
    EMin成ConnectionState C使本本entState;

    UPROPERTY()
    int32 LocalPlaye本ID;

    UPROPERTY()
    TMap<int32, 軍Min成Playe本的etwo本kInfo> ConnectedPlaye本s;

    UPROPERTY()
    軍Min成的etwo本kSessionInfo C使本本entSession;

    UPROPERTY()
    軍Min成的etwo本kStats 的etwo本kStats;

    UPROPERTY()
    int32 C使本本entTickRate;

    UPROPERTY()
    軍St本in成 Se本正e本Passwo本d;

    UPROPERTY()
    TSet<int32> BannedPlaye本s;

    UPROPERTY()
    float Last的etwo本kUpdateTi設置e;

    UPROPERTY()
    float 的etwo本kUpdateInte本正al;

    // Inte本nal 軍使nctions
    正oid Update的etwo本kStats(float DeltaTi設置e);
    正oid P本ocessInco設置in成Messa成es();
    正oid 輸入andlePlaye本Connection(int32 Playe本ID);
    正oid 輸入andlePlaye本Disconnection(int32 Playe本ID);
    bool ValidateMessa成e(const 軍Min成的etwo本kMessa成e& Messa成e) const;
    正oid Co設置p本essBatch(軍Min成SyncBatch& Batch);
    正oid Deco設置p本essBatch(軍Min成SyncBatch& Batch);
    正oid Send輸入ea本tbeat();
    正oid CheckConnection輸入ealth();
    正oid Clean使pDisconnectedPlaye本s();

    // Dele成ates
    U軍U的CTIO的()
    正oid On的etwo本k軍ail使本e(U基本o本ld* 基本o本ld, U的etD本i正e本* 的etD本i正e本, E的etwo本k軍ail使本e::Type 軍ail使本eType, const 軍St本in成& E本本o本St本in成);

    U軍U的CTIO的()
    正oid OnT本a正el軍ail使本e(U基本o本ld* 基本o本ld, ET本a正el軍ail使本e::Type 軍ail使本eType, const 軍St本in成& E本本o本St本in成);
);
};
