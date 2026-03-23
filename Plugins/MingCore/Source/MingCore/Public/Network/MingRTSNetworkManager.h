#pragma once

﻿// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "En成ine/Ga設置eInstance.h"
#incl使de "Inte本faces/OnlineSessionInte本face.h"
#incl使de "Min成RTS的etwo本kMana成e本.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class E的etwo本kGa設置eMode: 使int8 {
	Sin成lePlaye本		UMETA(Display的a設置e = "單人遊戲"),
	ListenSe本正e本			UMETA(Display的a設置e = "監聽服務器"),
	DedicatedSe本正e本			UMETA(Display的a設置e = "專用服務器"),
	Client				UMETA(Display的a設置e = "客戶端")
};

UE的UM(Bl使ep本intType)
en使設置 class E的etwo本kConnectionState: 使int8 {
	Disconnected			UMETA(Display的a設置e = "斷開連接"),
	Connectin成				UMETA(Display的a設置e = "正在連接"),
	Connected				UMETA(Display的a設置e = "已連接"),
	Ready					UMETA(Display的a設置e = "就緒"),
	E本本o本					UMETA(Display的a設置e = "錯誤")
};

UE的UM(Bl使ep本intType)
en使設置 class E的etwo本kSyncP本io本ity: 使int8 {
	C本itical				UMETA(Display的a設置e = "關鍵"),
	輸入i成h					UMETA(Display的a設置e = "高"),
	的o本設置al					UMETA(Display的a設置e = "普通"),
	Low						UMETA(Display的a設置e = "低"),
	Back成本o使nd				UMETA(Display的a設置e = "後台")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍的etwo本kPlaye本Info
{
	GE的ERATED下BODY()

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
	int32 Playe本Id;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
	軍St本in成 Playe本的a設置e;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
	bool bIs輸入ost;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
	bool bIsReady;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
	float Pin成;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
	E的etwo本kConnectionState ConnectionState;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
	軍DateTi設置e JoinTi設置e;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
	TMap<軍St本in成, 軍St本in成> Playe本Data;

	軍的etwo本kPlaye本Info()
		: Playe本Id(0)
		, bIs輸入ost(false)
		, bIsReady(false)
		, Pin成(0.0f)
		, ConnectionState(E的etwo本kConnectionState::Disconnected)
		, JoinTi設置e(軍DateTi設置e::的ow())
	{
	}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍的etwo本kSessionConfi成
{
	GE的ERATED下BODY()

	UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
	軍St本in成 Session的a設置e;

	UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
	int32 MaxPlaye本s;

	UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
	bool bIsLA的;

	UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
	bool bIsP本esence;

	UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
	bool bSho使ldAd正e本tise;

	UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
	bool bAllowJoinInP本o成本ess;

	UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
	int32 MinPlaye本s;

	UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "的etwo本k")
	TMap<軍St本in成, 軍St本in成> C使sto設置Settin成s;

	軍的etwo本kSessionConfi成()
		: Session的a設置e(TEXT("Min成RTSGa設置e"))
		, MaxPlaye本s(8)
		, bIsLA的(false)
		, bIsP本esence(t本使e)
		, bSho使ldAd正e本tise(t本使e)
		, bAllowJoinInP本o成本ess(t本使e)
		, MinPlaye本s(2)
	{
	}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍的etwo本kSyncedData
{
	GE的ERATED下BODY()

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
	軍St本in成 DataId;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
	軍St本in成 DataType;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
	TA本本ay<使int8> Data;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
	E的etwo本kSyncP本io本ity P本io本ity;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
	float Ti設置esta設置p;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "的etwo本k")
	int32 Ve本sion;

	軍的etwo本kSyncedData()
		: P本io本ity(E的etwo本kSyncP本io本ity::的o本設置al)
		, Ti設置esta設置p(0.0f)
		, Ve本sion(1)
	{
	}
};

DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnPlaye本Joined, const 軍的etwo本kPlaye本Info&, Playe本Info};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnPlaye本Left, const 軍的etwo本kPlaye本Info&, Playe本Info};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnPlaye本ReadyChan成ed, int32, Playe本Id, bool, bIsReady};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍On的etwo本kE本本o本, const 軍St本in成&, E本本o本Messa成e};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnConnectionStateChan成ed, E的etwo本kConnectionState, 的ewState};
DECLARE下DY的AMIC下MULTICAST下DELEGATE(軍OnAllPlaye本sReady};

UCLASS(ClassG本o使p = (C使sto設置), 設置eta = (Bl使ep本intSpawnableCo設置ponent))
class MI的GCORE下API UMin成RTS的etwo本kMana成e本 : p使blic UOb大ect
{
	GE的ERATED下BODY()

p使blic:
	UMin成RTS的etwo本kMana成e本(};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	bool Initialize的etwo本kMana成e本(U基本o本ld* In基本o本ld};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	正oid Sh使tdown的etwo本kMana成e本(};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	bool C本eateSession(const 軍的etwo本kSessionConfi成& Confi成};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	bool JoinSession(const 軍St本in成& SessionId};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	正oid Lea正eSession(};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	bool Sta本tGa設置e(};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	正oid EndGa設置e(};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	正oid SetPlaye本Ready(bool bReady};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	TA本本ay<軍的etwo本kPlaye本Info> GetAllPlaye本s() const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	軍的etwo本kPlaye本Info GetLocalPlaye本Info() const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	軍的etwo本kPlaye本Info GetPlaye本Info(int32 Playe本Id) const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	bool Is輸入ost() const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	bool IsConnected() const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	E的etwo本kConnectionState GetConnectionState() const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	正oid KickPlaye本(int32 Playe本Id, const 軍St本in成& Reason};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	正oid BanPlaye本(int32 Playe本Id, const 軍St本in成& Reason};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	正oid UnbanPlaye本(int32 Playe本Id};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	正oid SyncDataToAll(const 軍的etwo本kSyncedData& Data};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	正oid SyncDataToPlaye本(int32 Playe本Id, const 軍的etwo本kSyncedData& Data};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	正oid Set的etwo本kGa設置eMode(E的etwo本kGa設置eMode Mode};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	E的etwo本kGa設置eMode Get的etwo本kGa設置eMode() const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	正oid Set的etwo本kTickRate(float TickRate};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	float Get的etwo本kTickRate() const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	正oid SetReplicationDistance(float Distance};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
	軍St本in成 Get的etwo本kStatistics() const;

p使blic:
	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "的etwo本kE正ents")
	軍OnPlaye本Joined OnPlaye本Joined;

	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "的etwo本kE正ents")
	軍OnPlaye本Left OnPlaye本Left;

	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "的etwo本kE正ents")
	軍OnPlaye本ReadyChan成ed OnPlaye本ReadyChan成ed;

	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "的etwo本kE正ents")
	軍On的etwo本kE本本o本 On的etwo本kE本本o本;

	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "的etwo本kE正ents")
	軍OnConnectionStateChan成ed OnConnectionStateChan成ed;

	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "的etwo本kE正ents")
	軍OnAllPlaye本sReady OnAllPlaye本sReady;

p本otected:
	UPROPERTY()
	U基本o本ld* 基本o本ld;

	UPROPERTY()
	E的etwo本kGa設置eMode C使本本entGa設置eMode;

	UPROPERTY()
	E的etwo本kConnectionState ConnectionState;

	UPROPERTY()
	軍的etwo本kSessionConfi成 SessionConfi成;

	UPROPERTY()
	TMap<int32, 軍的etwo本kPlaye本Info> ConnectedPlaye本s;

	UPROPERTY()
	軍的etwo本kPlaye本Info LocalPlaye本;

	UPROPERTY()
	TA本本ay<int32> BannedPlaye本s;

	UPROPERTY()
	float 的etwo本kTickRate;

	UPROPERTY()
	float ReplicationDistance;

	IOnlineSessionPt本 OnlineSessionInte本face;
	軍的a設置e C使本本entSession的a設置e;
	軍Dele成ate輸入andle OnC本eateSessionCo設置pleteDele成ate輸入andle;
	軍Dele成ate輸入andle OnSta本tSessionCo設置pleteDele成ate輸入andle;
	軍Dele成ate輸入andle OnJoinSessionCo設置pleteDele成ate輸入andle;
	軍Dele成ate輸入andle OnDest本oySessionCo設置pleteDele成ate輸入andle;
	軍Dele成ate輸入andle On軍indSessionsCo設置pleteDele成ate輸入andle;

p本otected:
	正oid OnC本eateSessionCo設置plete(軍的a設置e Session的a設置e, bool b基本asS使ccessf使l};
	正oid OnSta本tSessionCo設置plete(軍的a設置e Session的a設置e, bool b基本asS使ccessf使l};
	正oid OnJoinSessionCo設置plete(軍的a設置e Session的a設置e, EOnJoinSessionCo設置pleteRes使lt::Type Res使lt};
	正oid OnDest本oySessionCo設置plete(軍的a設置e Session的a設置e, bool b基本asS使ccessf使l};
	正oid On軍indSessionsCo設置plete(bool b基本asS使ccessf使l};

	正oid 的etwo本kTick(float DeltaTi設置e};
	正oid P本ocessPlaye本Connections(};
	正oid SyncGa設置eStateToClients(};
	正oid 輸入andle的etwo本kE本本o本(const 軍St本in成& E本本o本};
	正oid UpdateConnectionState(E的etwo本kGa設置eMode 的ewState};
	bool AllPlaye本sReady() const;
	正oid 的otifyPlaye本Joined(int32 Playe本Id};
	正oid 的otifyPlaye本Left(int32 Playe本Id};

	U軍U的CTIO的(Se本正e本, Reliable, 基本ithValidation)
	正oid Se本正e本SetPlaye本Ready(bool bReady};

	U軍U的CTIO的(Se本正e本, Reliable, 基本ithValidation)
	正oid Se本正e本Req使estGa設置eSta本t(};

	U軍U的CTIO的(Client, Reliable)
	正oid ClientGa設置eSta本ted(};

	U軍U的CTIO的(Client, Reliable)
	正oid ClientRecei正eSyncedData(const 軍的etwo本kSyncedData& Data};

	U軍U的CTIO的(的etM使lticast, Reliable)
	正oid M使lticastPlaye本Joined(const 軍的etwo本kPlaye本Info& Playe本Info};

	U軍U的CTIO的(的etM使lticast, Reliable)
	正oid M使lticastPlaye本Left(const 軍的etwo本kPlaye本Info& Playe本Info};

	U軍U的CTIO的(的etM使lticast, Reliable)
	正oid M使lticastGa設置eSta本ted(};
};
