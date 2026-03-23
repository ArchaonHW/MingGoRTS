#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成的etwo本kMana成e本.成ene本ated.h"

// 軍o本wa本d decla本ations
class UMin成Pe本sonalMana成e本;
class UMin成LobbySyste設置;
class UMin成RelationshipReplication;
class UMin成Rep使tationReplication;

// 的etwo本k connection state
UE的UM(Bl使ep本intType)
en使設置 class EMin成的etwo本kConnectionState: 使int8 {
    Disconnected      UMETA(Display的a設置e = "Disconnected"),
    Connectin成        UMETA(Display的a設置e = "Connectin成"),
    Connected         UMETA(Display的a設置e = "Connected"),
    Disconnectin成     UMETA(Display的a設置e = "Disconnectin成"),
    Reconnectin成      UMETA(Display的a設置e = "Reconnectin成"),
    E本本o本             UMETA(Display的a設置e = "E本本o本")
};

// 的etwo本k 本ole
UE的UM(Bl使ep本intType)
en使設置 class EMin成的etwo本kRole: 使int8 {
    的one              UMETA(Display的a設置e = "的one"),
    輸入ost              UMETA(Display的a設置e = "輸入ost"),
    Client            UMETA(Display的a設置e = "Client"),
    ListenSe本正e本      UMETA(Display的a設置e = "Listen Se本正e本")
};

// 的etwo本k confi成使本ation
USTRUCT(Bl使ep本intType)
st本使ct 軍Min成的etwo本kConfi成
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
    軍St本in成 Se本正e本的a設置e;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
    int32 MaxPlaye本s;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
    int32 Po本t;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
    軍St本in成 Map的a設置e;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
    bool bUseLA的;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
    bool bIsP本i正ate;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
    軍St本in成 Passwo本d;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
    軍St本in成 Ga設置eMode;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
    int32 TickRate;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
    bool bEnableVoiceChat;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
    bool bAllowJoinInP本o成本ess;

    軍Min成的etwo本kConfi成()
        : Se本正e本的a設置e(TEXT("Min成GoRTS Se本正e本"))
        , MaxPlaye本s(4)
        , Po本t(7777)
        , Map的a設置e(TEXT("MainMap"))
        , bUseLA的(false)
        , bIsP本i正ate(false)
        , Passwo本d(TEXT(""))
        , Ga設置eMode(TEXT("Min成RTSGa設置eMode"))
        , TickRate(30)
        , bEnableVoiceChat(t本使e)
        , bAllowJoinInP本o成本ess(t本使e)
    {}
};

// Playe本 netwo本k info
USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Playe本的etwo本kInfo
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    int32 Playe本ID;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    軍St本in成 Playe本的a設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    bool bIs輸入ost;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    bool bIsReady;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    float Pin成;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    軍St本in成 IPAdd本ess;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    int32 Tea設置ID;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    bool bIsSpectato本;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    int32 Playe本Sco本e;

    軍Min成Playe本的etwo本kInfo()
        : Playe本ID(-1)
        , Playe本的a設置e(TEXT(""))
        , bIs輸入ost(false)
        , bIsReady(false)
        , Pin成(0.0f)
        , IPAdd本ess(TEXT(""))
        , Tea設置ID(0)
        , bIsSpectato本(false)
        , Playe本Sco本e(0)
    {}
};

// 的etwo本k statistics
USTRUCT(Bl使ep本intType)
st本使ct 軍Min成的etwo本kStats
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    float A正e本a成ePin成;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    float PacketLossRate;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    int32 BytesSentPe本Second;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    int32 BytesRecei正edPe本Second;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    int32 ConnectedPlaye本s;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    float Se本正e本軍本a設置eTi設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    int32 ReplicatedActo本s;

    軍Min成的etwo本kStats()
        : A正e本a成ePin成(0.0f)
        , PacketLossRate(0.0f)
        , BytesSentPe本Second(0)
        , BytesRecei正edPe本Second(0)
        , ConnectedPlaye本s(0)
        , Se本正e本軍本a設置eTi設置e(0.0f)
        , ReplicatedActo本s(0)
    {}
};

// Chat 設置essa成e
USTRUCT(Bl使ep本intType)
st本使ct 軍Min成ChatMessa成e
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    int32 Sende本Playe本ID;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    軍St本in成 Sende本的a設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    軍St本in成 Messa成e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    float Ti設置esta設置p;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    bool bIsSyste設置Messa成e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    bool bIsP本i正ate;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
    int32 Ta本成etPlaye本ID;

    軍Min成ChatMessa成e()
        : Sende本Playe本ID(-1)
        , Sende本的a設置e(TEXT(""))
        , Messa成e(TEXT(""))
        , Ti設置esta設置p(0.0f)
        , bIsSyste設置Messa成e(false)
        , bIsP本i正ate(false)
        , Ta本成etPlaye本ID(-1)
    {}
};

// 的etwo本k e本本o本 types
UE的UM(Bl使ep本intType)
en使設置 class EMin成的etwo本kE本本o本: 使int8 {
    的one                      UMETA(Display的a設置e = "的one"),
    Connection軍ailed          UMETA(Display的a設置e = "Connection 軍ailed"),
    ConnectionLost            UMETA(Display的a設置e = "Connection Lost"),
    Se本正e本軍使ll                UMETA(Display的a設置e = "Se本正e本 軍使ll"),
    In正alidPasswo本d           UMETA(Display的a設置e = "In正alid Passwo本d"),
    Ve本sionMis設置atch           UMETA(Display的a設置e = "Ve本sion Mis設置atch"),
    Ti設置eo使t                   UMETA(Display的a設置e = "Ti設置eo使t"),
    輸入ostMi成本ation軍ailed       UMETA(Display的a設置e = "輸入ost Mi成本ation 軍ailed"),
    的atP使nchth本o使成h軍ailed     UMETA(Display的a設置e = "的AT P使nchth本o使成h 軍ailed"),
    Se本正e本Sh使tdown            UMETA(Display的a設置e = "Se本正e本 Sh使tdown"),
    Kicked                    UMETA(Display的a設置e = "Kicked"),
    Banned                    UMETA(Display的a設置e = "Banned"),
    Gene本icE本本o本              UMETA(Display的a設置e = "Gene本ic E本本o本")
};

// Dele成ates
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍On的etwo本kConnectionStateChan成ed, EMin成的etwo本kConnectionState, 的ewState};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnPlaye本Joined, 軍Min成Playe本的etwo本kInfo, Playe本Info};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnPlaye本Left, int32, Playe本ID};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnPlaye本ReadyChan成ed, int32, Playe本ID};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnChatMessa成eRecei正ed, 軍Min成ChatMessa成e, Messa成e};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍On的etwo本kE本本o本, EMin成的etwo本kE本本o本, E本本o本};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍On輸入ostMi成本ated, int32, 的ew輸入ostPlaye本ID};
DECLARE下DY的AMIC下MULTICAST下DELEGATE(軍OnGa設置eSta本ted};
DECLARE下DY的AMIC下MULTICAST下DELEGATE(軍OnGa設置eEnded};

/**
 * 網絡管務 * 多人動戲動網絡動玩家管故事重要性基本步
 */
UCLASS(ClassG本o使p = (Min成GoRTS), 設置eta = (Bl使ep本intSpawnableCo設置ponent))
class MI的GPERSO的AL下API UMin成的etwo本kMana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成的etwo本kMana成e本(};

    // Initialize/Sh使tdown
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid Initialize(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid Sh使tdown(};

    // Se本正e本 c本eation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    bool C本eateSe本正e本(const 軍Min成的etwo本kConfi成& Confi成};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid CloseSe本正e本(};

    // Client connection
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    bool ConnectToSe本正e本(const 軍St本in成& Se本正e本Add本ess, int32 Po本t, const 軍St本in成& Passwo本d = TEXT("")};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid Disconnect(};

    // Connection state
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    EMin成的etwo本kConnectionState GetConnectionState() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    bool IsConnected() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    bool Is輸入ost() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    bool IsClient() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    EMin成的etwo本kRole Get的etwo本kRole() const;

    // Playe本 設置ana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid SetPlaye本Ready(bool bReady};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    bool IsPlaye本Ready(int32 Playe本ID) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    TA本本ay<軍Min成Playe本的etwo本kInfo> GetAllPlaye本s() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    軍Min成Playe本的etwo本kInfo GetPlaye本Info(int32 Playe本ID) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    軍Min成Playe本的etwo本kInfo GetLocalPlaye本Info() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    int32 GetLocalPlaye本ID() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid KickPlaye本(int32 Playe本ID, const 軍St本in成& Reason};

    // Ga設置e cont本ol
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid Sta本tGa設置e(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid EndGa設置e(};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    bool IsGa設置eInP本o成本ess() const;

    // Chat syste設置
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid SendChatMessa成e(const 軍St本in成& Messa成e, bool bIsP本i正ate = false, int32 Ta本成etPlaye本ID = -1};

    // 輸入ost 設置i成本ation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    bool Pe本fo本設置輸入ostMi成本ation(};

    // 的etwo本k stats
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    軍Min成的etwo本kStats Get的etwo本kStats() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid GetDetailed的etwo本kStats(軍St本in成& O使tStats) const;

    // E本本o本 handlin成
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    EMin成的etwo本kE本本o本 GetLastE本本o本() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    軍St本in成 GetLastE本本o本Messa成e() const;

    // Confi成使本ation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid Set的etwo本kConfi成(const 軍Min成的etwo本kConfi成& 的ewConfi成};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k")
    軍Min成的etwo本kConfi成 Get的etwo本kConfi成() const;

    // Dele成ates
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "的etwo本k E正ents")
    軍On的etwo本kConnectionStateChan成ed OnConnectionStateChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "的etwo本k E正ents")
    軍OnPlaye本Joined OnPlaye本Joined;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "的etwo本k E正ents")
    軍OnPlaye本Left OnPlaye本Left;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "的etwo本k E正ents")
    軍OnPlaye本ReadyChan成ed OnPlaye本ReadyChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "的etwo本k E正ents")
    軍OnChatMessa成eRecei正ed OnChatMessa成eRecei正ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "的etwo本k E正ents")
    軍On的etwo本kE本本o本 On的etwo本kE本本o本;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "的etwo本k E正ents")
    軍On輸入ostMi成本ated On輸入ostMi成本ated;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "的etwo本k E正ents")
    軍OnGa設置eSta本ted OnGa設置eSta本ted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "的etwo本k E正ents")
    軍OnGa設置eEnded OnGa設置eEnded;

p本i正ate:
    // 的etwo本k state
    UPROPERTY()
    EMin成的etwo本kConnectionState ConnectionState;

    UPROPERTY()
    EMin成的etwo本kRole 的etwo本kRole;

    UPROPERTY()
    軍Min成的etwo本kConfi成 C使本本entConfi成;

    UPROPERTY()
    TA本本ay<軍Min成Playe本的etwo本kInfo> ConnectedPlaye本s;

    UPROPERTY()
    int32 LocalPlaye本ID;

    UPROPERTY()
    bool bGa設置eInP本o成本ess;

    UPROPERTY()
    EMin成的etwo本kE本本o本 LastE本本o本;

    UPROPERTY()
    軍St本in成 LastE本本o本Messa成e;

    UPROPERTY()
    軍Min成的etwo本kStats 的etwo本kStats;

    // Inte本nal f使nctions
    正oid UpdateConnectionState(EMin成的etwo本kConnectionState 的ewState};
    正oid 輸入andleConnectionS使ccess(};
    正oid 輸入andleConnection軍ail使本e(EMin成的etwo本kE本本o本 E本本o本, const 軍St本in成& Messa成e};
    正oid Update的etwo本kStats(};
    正oid B本oadcastChatMessa成eToAll(const 軍Min成ChatMessa成e& Messa成e};
    正oid B本oadcastChatMessa成eToPlaye本(const 軍Min成ChatMessa成e& Messa成e, int32 Ta本成etPlaye本ID};
    正oid AddPlaye本(const 軍Min成Playe本的etwo本kInfo& Playe本Info};
    正oid Re設置o正ePlaye本(int32 Playe本ID};
    正oid UpdatePlaye本ReadyState(int32 Playe本ID, bool bReady};
    int32 Gene本atePlaye本ID(};
    bool ValidatePasswo本d(const 軍St本in成& Passwo本d) const;
    正oid OnRep下ConnectionState(};
};

