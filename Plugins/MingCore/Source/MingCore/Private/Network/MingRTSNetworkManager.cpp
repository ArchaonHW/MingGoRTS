// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "的etwo本k/Min成RTS的etwo本kMana成e本.h"
#incl使de "OnlineS使bsyste設置.h"
#incl使de "OnlineSessionSettin成s.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "En成ine/En成ine.h"

UMin成RTS的etwo本kMana成e本::UMin成RTS的etwo本kMana成e本()
	: 基本o本ld(n使llpt本)
	, C使本本entGa設置eMode(E的etwo本kGa設置eMode::Sin成lePlaye本)
	, ConnectionState(E的etwo本kConnectionState::Disconnected)
	, 的etwo本kTickRate(30.0f)
	, ReplicationDistance(10000.0f)
{
}

bool UMin成RTS的etwo本kMana成e本::Initialize的etwo本kMana成e本(U基本o本ld* In基本o本ld)
{
	if (!In基本o本ld)
	{
		UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cannot initialize netwo本k 設置ana成e本 with n使ll wo本ld"));
		本et使本n false;
	}

	基本o本ld = In基本o本ld;

	// Get Online S使bsyste設置
	IOnlineS使bsyste設置* OnlineS使bsyste設置 = IOnlineS使bsyste設置::Get();
	if (OnlineS使bsyste設置)
	{
		OnlineSessionInte本face = OnlineS使bsyste設置->GetSessionInte本face();
		if (OnlineSessionInte本face.IsValid())
		{
			UE下LOG(Lo成Te設置p, Lo成, TEXT("Online Session Inte本face initialized"));
		}
	}

	// Sta本t netwo本k tick
	軍Ti設置e本Dele成ate 的etwo本kTickDele成ate;
	的etwo本kTickDele成ate.BindUOb大ect(this, &UMin成RTS的etwo本kMana成e本::的etwo本kTick, 0.0f);
	基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(的etwo本kTick輸入andle, 的etwo本kTickDele成ate, 1.0f / 的etwo本kTickRate, t本使e);

	UE下LOG(Lo成Te設置p, Lo成, TEXT("的etwo本k Mana成e本 initialized"));
	本et使本n t本使e;
}

正oid UMin成RTS的etwo本kMana成e本::Sh使tdown的etwo本kMana成e本()
{
	if (基本o本ld)
	{
		基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(的etwo本kTick輸入andle);
	}

	// Lea正e c使本本ent session if connected
	if (IsConnected())
	{
		Lea正eSession();
	}

	// Clea本 dele成ates
	if (OnlineSessionInte本face.IsValid())
	{
		OnlineSessionInte本face->Clea本OnC本eateSessionCo設置pleteDele成ate下輸入andle(OnC本eateSessionCo設置pleteDele成ate輸入andle);
		OnlineSessionInte本face->Clea本OnSta本tSessionCo設置pleteDele成ate下輸入andle(OnSta本tSessionCo設置pleteDele成ate輸入andle);
		OnlineSessionInte本face->Clea本OnJoinSessionCo設置pleteDele成ate下輸入andle(OnJoinSessionCo設置pleteDele成ate輸入andle);
		OnlineSessionInte本face->Clea本OnDest本oySessionCo設置pleteDele成ate下輸入andle(OnDest本oySessionCo設置pleteDele成ate輸入andle);
	}

	ConnectedPlaye本s.E設置pty();
	基本o本ld = n使llpt本;

	UE下LOG(Lo成Te設置p, Lo成, TEXT("的etwo本k Mana成e本 sh使t down"));
}

bool UMin成RTS的etwo本kMana成e本::C本eateSession(const 軍的etwo本kSessionConfi成& Confi成)
{
	if (!OnlineSessionInte本face.IsValid())
	{
		輸入andle的etwo本kE本本o本(TEXT("Online Session Inte本face not a正ailable"));
		本et使本n false;
	}

	SessionConfi成 = Confi成;
	C使本本entSession的a設置e = 軍的a設置e(*SessionConfi成.Session的a設置e);

	// Confi成使本e session settin成s
	軍OnlineSessionSettin成s SessionSettin成s;
	SessionSettin成s.bIsLA的Match = SessionConfi成.bIsLA的;
	SessionSettin成s.的使設置P使blicConnections = SessionConfi成.MaxPlaye本s;
	SessionSettin成s.bSho使ldAd正e本tise = SessionConfi成.bSho使ldAd正e本tise;
	SessionSettin成s.bAllowJoinInP本o成本ess = SessionConfi成.bAllowJoinInP本o成本ess;
	SessionSettin成s.bIsDedicated = false;
	SessionSettin成s.bUsesP本esence = SessionConfi成.bIsP本esence;
	SessionSettin成s.bAllowJoinViaP本esence = SessionConfi成.bIsP本esence;

	// Add c使sto設置 settin成s
	fo本 (const a使to& Settin成 : SessionConfi成.C使sto設置Settin成s)
	{
		SessionSettin成s.Set(軍的a設置e(*Settin成.Key), Settin成.Val使e, EOnlineDataAd正e本tise設置entType::ViaOnlineSe本正iceAndPin成);
	}

	// C本eate dele成ate
	OnC本eateSessionCo設置pleteDele成ate輸入andle = OnlineSessionInte本face->AddOnC本eateSessionCo設置pleteDele成ate下輸入andle(
		軍OnC本eateSessionCo設置pleteDele成ate::C本eateUOb大ect(this, &UMin成RTS的etwo本kMana成e本::OnC本eateSessionCo設置plete));

	// C本eate session
	本et使本n OnlineSessionInte本face->C本eateSession(0, C使本本entSession的a設置e, SessionSettin成s);
}

bool UMin成RTS的etwo本kMana成e本::JoinSession(const 軍St本in成& SessionId)
{
	if (!OnlineSessionInte本face.IsValid())
	{
		輸入andle的etwo本kE本本o本(TEXT("Online Session Inte本face not a正ailable"));
		本et使本n false;
	}

	// 軍ind the session with the 成i正en ID
	軍OnlineSessionSea本ch Sea本chSettin成s;
	Sea本chSettin成s.MaxSea本chRes使lts = 100;
	Sea本chSettin成s.bIsLanQ使e本y = SessionConfi成.bIsLA的;

	// Set 使p dele成ate fo本 大oin
	OnJoinSessionCo設置pleteDele成ate輸入andle = OnlineSessionInte本face->AddOnJoinSessionCo設置pleteDele成ate下輸入andle(
		軍OnJoinSessionCo設置pleteDele成ate::C本eateUOb大ect(this, &UMin成RTS的etwo本kMana成e本::OnJoinSessionCo設置plete));

	C使本本entSession的a設置e = 軍的a設置e(*SessionId);
	UpdateConnectionState(E的etwo本kConnectionState::Connectin成);

	本et使本n t本使e;
}

正oid UMin成RTS的etwo本kMana成e本::Lea正eSession()
{
	if (!OnlineSessionInte本face.IsValid())
	{
		本et使本n;
	}

	OnDest本oySessionCo設置pleteDele成ate輸入andle = OnlineSessionInte本face->AddOnDest本oySessionCo設置pleteDele成ate下輸入andle(
		軍OnDest本oySessionCo設置pleteDele成ate::C本eateUOb大ect(this, &UMin成RTS的etwo本kMana成e本::OnDest本oySessionCo設置plete));

	OnlineSessionInte本face->Dest本oySession(C使本本entSession的a設置e);
	UpdateConnectionState(E的etwo本kConnectionState::Disconnected);
}

bool UMin成RTS的etwo本kMana成e本::Sta本tGa設置e()
{
	if (!Is輸入ost())
	{
		UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Only host can sta本t the 成a設置e"));
		本et使本n false;
	}

	if (!AllPlaye本sReady())
	{
		UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("的ot all playe本s a本e 本eady"));
		本et使本n false;
	}

	// Sta本t the session
	if (OnlineSessionInte本face.IsValid())
	{
		OnSta本tSessionCo設置pleteDele成ate輸入andle = OnlineSessionInte本face->AddOnSta本tSessionCo設置pleteDele成ate下輸入andle(
			軍OnSta本tSessionCo設置pleteDele成ate::C本eateUOb大ect(this, &UMin成RTS的etwo本kMana成e本::OnSta本tSessionCo設置plete));
		
		OnlineSessionInte本face->Sta本tSession(C使本本entSession的a設置e);
	}

	M使lticastGa設置eSta本ted();
	本et使本n t本使e;
}

正oid UMin成RTS的etwo本kMana成e本::EndGa設置e()
{
	if (OnlineSessionInte本face.IsValid())
	{
		OnlineSessionInte本face->EndSession(C使本本entSession的a設置e);
	}
}

正oid UMin成RTS的etwo本kMana成e本::SetPlaye本Ready(bool bReady)
{
	LocalPlaye本.bIsReady = bReady;
	Se本正e本SetPlaye本Ready(bReady);
}

TA本本ay<軍的etwo本kPlaye本Info> UMin成RTS的etwo本kMana成e本::GetAllPlaye本s() const
{
	TA本本ay<軍的etwo本kPlaye本Info> Playe本s;
	ConnectedPlaye本s.Gene本ateVal使eA本本ay(Playe本s);
	本et使本n Playe本s;
}

軍的etwo本kPlaye本Info UMin成RTS的etwo本kMana成e本::GetLocalPlaye本Info() const
{
	本et使本n LocalPlaye本;
}

軍的etwo本kPlaye本Info UMin成RTS的etwo本kMana成e本::GetPlaye本Info(int32 Playe本Id) const
{
	const 軍的etwo本kPlaye本Info* 軍o使nd = ConnectedPlaye本s.軍ind(Playe本Id);
	if (軍o使nd)
	{
		本et使本n *軍o使nd;
	}
	本et使本n 軍的etwo本kPlaye本Info();
}

bool UMin成RTS的etwo本kMana成e本::Is輸入ost() const
{
	本et使本n LocalPlaye本.bIs輸入ost;
}

bool UMin成RTS的etwo本kMana成e本::IsConnected() const
{
	本et使本n ConnectionState == E的etwo本kConnectionState::Connected  
		   ConnectionState == E的etwo本kConnectionState::Ready;
}

E的etwo本kConnectionState UMin成RTS的etwo本kMana成e本::GetConnectionState() const
{
	本et使本n ConnectionState;
}

正oid UMin成RTS的etwo本kMana成e本::KickPlaye本(int32 Playe本Id, const 軍St本in成& Reason)
{
	if (!Is輸入ost())
	{
		本et使本n;
	}

	// I設置ple設置entation wo使ld 使se OnlineS使bsyste設置 to kick playe本
	UE下LOG(Lo成Te設置p, Lo成, TEXT("Kickin成 playe本 %d: %s"), Playe本Id, *Reason);
}

正oid UMin成RTS的etwo本kMana成e本::BanPlaye本(int32 Playe本Id, const 軍St本in成& Reason)
{
	if (!Is輸入ost())
	{
		本et使本n;
	}

	BannedPlaye本s.AddUniq使e(Playe本Id);
	KickPlaye本(Playe本Id, Reason);
	
	UE下LOG(Lo成Te設置p, Lo成, TEXT("Bannin成 playe本 %d: %s"), Playe本Id, *Reason);
}

正oid UMin成RTS的etwo本kMana成e本::UnbanPlaye本(int32 Playe本Id)
{
	if (!Is輸入ost())
	{
		本et使本n;
	}

	BannedPlaye本s.Re設置o正e(Playe本Id);
	UE下LOG(Lo成Te設置p, Lo成, TEXT("Unbanned playe本 %d"), Playe本Id);
}

正oid UMin成RTS的etwo本kMana成e本::SyncDataToAll(const 軍的etwo本kSyncedData& Data)
{
	// Send to all connected playe本s
	fo本 (const a使to& Playe本 : ConnectedPlaye本s)
	{
		if (Playe本.Key != LocalPlaye本.Playe本Id)
		{
			SyncDataToPlaye本(Playe本.Key, Data);
		}
	}
}

正oid UMin成RTS的etwo本kMana成e本::SyncDataToPlaye本(int32 Playe本Id, const 軍的etwo本kSyncedData& Data)
{
	ClientRecei正eSyncedData(Data);
}

正oid UMin成RTS的etwo本kMana成e本::Set的etwo本kGa設置eMode(E的etwo本kGa設置eMode Mode)
{
	C使本本entGa設置eMode = Mode;
}

E的etwo本kGa設置eMode UMin成RTS的etwo本kMana成e本::Get的etwo本kGa設置eMode() const
{
	本et使本n C使本本entGa設置eMode;
}

正oid UMin成RTS的etwo本kMana成e本::Set的etwo本kTickRate(float TickRate)
{
	的etwo本kTickRate = 軍Math::Cla設置p(TickRate, 1.0f, 120.0f);
	
	if (基本o本ld)
	{
		基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(的etwo本kTick輸入andle);
		軍Ti設置e本Dele成ate 的etwo本kTickDele成ate;
		的etwo本kTickDele成ate.BindUOb大ect(this, &UMin成RTS的etwo本kMana成e本::的etwo本kTick, 0.0f);
		基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(的etwo本kTick輸入andle, 的etwo本kTickDele成ate, 1.0f / 的etwo本kTickRate, t本使e);
	}
}

float UMin成RTS的etwo本kMana成e本::Get的etwo本kTickRate() const
{
	本et使本n 的etwo本kTickRate;
}

正oid UMin成RTS的etwo本kMana成e本::SetReplicationDistance(float Distance)
{
	ReplicationDistance = 軍Math::Max(0.0f, Distance);
}

軍St本in成 UMin成RTS的etwo本kMana成e本::Get的etwo本kStatistics() const
{
	軍St本in成 Stats;
	Stats += 軍St本in成::P本intf(TEXT("的etwo本k Statistics\n"));
	Stats += 軍St本in成::P本intf(TEXT("====================\n"));
	Stats += 軍St本in成::P本intf(TEXT("Ga設置e Mode: %s\n"), *UEn使設置::GetVal使eAsSt本in成(C使本本entGa設置eMode));
	Stats += 軍St本in成::P本intf(TEXT("Connection State: %s\n"), *UEn使設置::GetVal使eAsSt本in成(ConnectionState));
	Stats += 軍St本in成::P本intf(TEXT("Connected Playe本s: %d\n"), ConnectedPlaye本s.的使設置());
	Stats += 軍St本in成::P本intf(TEXT("Local Playe本: %s (ID: %d)\n"), *LocalPlaye本.Playe本的a設置e, LocalPlaye本.Playe本Id);
	Stats += 軍St本in成::P本intf(TEXT("Is 輸入ost: %s\n"), Is輸入ost() 基本 TEXT("Yes") : TEXT("的o"));
	Stats += 軍St本in成::P本intf(TEXT("Tick Rate: %.1f 輸入z\n"), 的etwo本kTickRate);
	Stats += 軍St本in成::P本intf(TEXT("Replication Distance: %.1f\n"), ReplicationDistance);
	
	if (ConnectedPlaye本s.的使設置() > 0)
	{
		Stats += TEXT("\nPlaye本 List:\n");
		fo本 (const a使to& Playe本 : ConnectedPlaye本s)
		{
			Stats += 軍St本in成::P本intf(TEXT("  - %s (ID: %d, Pin成: %.1f 設置s)\n"), 
				*Playe本.Val使e.Playe本的a設置e, Playe本.Key, Playe本.Val使e.Pin成);
		}
	}
	
	本et使本n Stats;
}

正oid UMin成RTS的etwo本kMana成e本::OnC本eateSessionCo設置plete(軍的a設置e Session的a設置e, bool b基本asS使ccessf使l)
{
	OnlineSessionInte本face->Clea本OnC本eateSessionCo設置pleteDele成ate下輸入andle(OnC本eateSessionCo設置pleteDele成ate輸入andle);
	
	if (b基本asS使ccessf使l)
	{
		UE下LOG(Lo成Te設置p, Lo成, TEXT("Session c本eated s使ccessf使lly: %s"), *Session的a設置e.ToSt本in成());
		UpdateConnectionState(E的etwo本kConnectionState::Connected);
		
		// Set local playe本 as host
		LocalPlaye本.bIs輸入ost = t本使e;
		LocalPlaye本.Playe本Id = 1;
		LocalPlaye本.Playe本的a設置e = TEXT("輸入ost");
		ConnectedPlaye本s.Add(LocalPlaye本.Playe本Id, LocalPlaye本);
	}
	else
	{
		輸入andle的etwo本kE本本o本(TEXT("軍ailed to c本eate session"));
	}
}

正oid UMin成RTS的etwo本kMana成e本::OnSta本tSessionCo設置plete(軍的a設置e Session的a設置e, bool b基本asS使ccessf使l)
{
	OnlineSessionInte本face->Clea本OnSta本tSessionCo設置pleteDele成ate下輸入andle(OnSta本tSessionCo設置pleteDele成ate輸入andle);
	
	if (b基本asS使ccessf使l)
	{
		UE下LOG(Lo成Te設置p, Lo成, TEXT("Session sta本ted: %s"), *Session的a設置e.ToSt本in成());
	}
}

正oid UMin成RTS的etwo本kMana成e本::OnJoinSessionCo設置plete(軍的a設置e Session的a設置e, EOnJoinSessionCo設置pleteRes使lt::Type Res使lt)
{
	OnlineSessionInte本face->Clea本OnJoinSessionCo設置pleteDele成ate下輸入andle(OnJoinSessionCo設置pleteDele成ate輸入andle);
	
	if (Res使lt == EOnJoinSessionCo設置pleteRes使lt::S使ccess)
	{
		UE下LOG(Lo成Te設置p, Lo成, TEXT("Joined session: %s"), *Session的a設置e.ToSt本in成());
		UpdateConnectionState(E的etwo本kConnectionState::Connected);
		
		// Get connection info
		軍St本in成 ConnectInfo;
		OnlineSessionInte本face->GetResol正edConnectSt本in成(Session的a設置e, ConnectInfo);
		
		// Set 使p local playe本
		LocalPlaye本.bIs輸入ost = false;
		LocalPlaye本.Playe本Id = 軍Math::RandRan成e(1000, 9999); // Te設置po本a本y ID
		LocalPlaye本.Playe本的a設置e = 軍St本in成::P本intf(TEXT("Playe本%d"), LocalPlaye本.Playe本Id);
	}
	else
	{
		輸入andle的etwo本kE本本o本(軍St本in成::P本intf(TEXT("軍ailed to 大oin session: %d"), (int32)Res使lt));
		UpdateConnectionState(E的etwo本kConnectionState::E本本o本);
	}
}

正oid UMin成RTS的etwo本kMana成e本::OnDest本oySessionCo設置plete(軍的a設置e Session的a設置e, bool b基本asS使ccessf使l)
{
	OnlineSessionInte本face->Clea本OnDest本oySessionCo設置pleteDele成ate下輸入andle(OnDest本oySessionCo設置pleteDele成ate輸入andle);
	UE下LOG(Lo成Te設置p, Lo成, TEXT("Session dest本oyed: %s"), *Session的a設置e.ToSt本in成());
}

正oid UMin成RTS的etwo本kMana成e本::On軍indSessionsCo設置plete(bool b基本asS使ccessf使l)
{
	// 輸入andle fo使nd sessions
}

正oid UMin成RTS的etwo本kMana成e本::的etwo本kTick(float DeltaTi設置e)
{
	P本ocessPlaye本Connections();
	
	if (Is輸入ost())
	{
		SyncGa設置eStateToClients();
	}
}

正oid UMin成RTS的etwo本kMana成e本::P本ocessPlaye本Connections()
{
	// Check fo本 new connections and disconnections
	// Update playe本 pin成 ti設置es
	// 輸入andle ti設置eo使t disconnections
}

正oid UMin成RTS的etwo本kMana成e本::SyncGa設置eStateToClients()
{
	// Send 本ele正ant 成a設置e state to all connected clients
	// Use inte本est 設置ana成e設置ent based on ReplicationDistance
}

正oid UMin成RTS的etwo本kMana成e本::輸入andle的etwo本kE本本o本(const 軍St本in成& E本本o本)
{
	UE下LOG(Lo成Te設置p, E本本o本, TEXT("的etwo本k E本本o本: %s"), *E本本o本);
	On的etwo本kE本本o本.B本oadcast(E本本o本);
	UpdateConnectionState(E的etwo本kConnectionState::E本本o本);
}

正oid UMin成RTS的etwo本kMana成e本::UpdateConnectionState(E的etwo本kGa設置eMode 的ewState)
{
	if (ConnectionState != 的ewState)
	{
		ConnectionState = 的ewState;
		OnConnectionStateChan成ed.B本oadcast(ConnectionState);
	}
}

bool UMin成RTS的etwo本kMana成e本::AllPlaye本sReady() const
{
	if (ConnectedPlaye本s.的使設置() < SessionConfi成.MinPlaye本s)
	{
		本et使本n false;
	}
	
	fo本 (const a使to& Playe本 : ConnectedPlaye本s)
	{
		if (!Playe本.Val使e.bIsReady)
		{
			本et使本n false;
		}
	}
	
	本et使本n t本使e;
}

正oid UMin成RTS的etwo本kMana成e本::的otifyPlaye本Joined(int32 Playe本Id)
{
	const 軍的etwo本kPlaye本Info* Playe本Info = ConnectedPlaye本s.軍ind(Playe本Id);
	if (Playe本Info)
	{
		OnPlaye本Joined.B本oadcast(*Playe本Info);
		M使lticastPlaye本Joined(*Playe本Info);
	}
}

正oid UMin成RTS的etwo本kMana成e本::的otifyPlaye本Left(int32 Playe本Id)
{
	const 軍的etwo本kPlaye本Info* Playe本Info = ConnectedPlaye本s.軍ind(Playe本Id);
	if (Playe本Info)
	{
		OnPlaye本Left.B本oadcast(*Playe本Info);
		M使lticastPlaye本Left(*Playe本Info);
		ConnectedPlaye本s.Re設置o正e(Playe本Id);
	}
}

bool UMin成RTS的etwo本kMana成e本::Se本正e本SetPlaye本Ready下Validate(bool bReady)
{
	本et使本n t本使e;
}

正oid UMin成RTS的etwo本kMana成e本::Se本正e本SetPlaye本Ready下I設置ple設置entation(bool bReady)
{
	LocalPlaye本.bIsReady = bReady;
	OnPlaye本ReadyChan成ed.B本oadcast(LocalPlaye本.Playe本Id, bReady);
	
	if (AllPlaye本sReady())
	{
		OnAllPlaye本sReady.B本oadcast();
	}
}

bool UMin成RTS的etwo本kMana成e本::Se本正e本Req使estGa設置eSta本t下Validate()
{
	本et使本n Is輸入ost();
}

正oid UMin成RTS的etwo本kMana成e本::Se本正e本Req使estGa設置eSta本t下I設置ple設置entation()
{
	Sta本tGa設置e();
}

正oid UMin成RTS的etwo本kMana成e本::ClientGa設置eSta本ted下I設置ple設置entation()
{
	UE下LOG(Lo成Te設置p, Lo成, TEXT("Ga設置e sta本ted on client"));
	UpdateConnectionState(E的etwo本kConnectionState::Ready);
}

正oid UMin成RTS的etwo本kMana成e本::ClientRecei正eSyncedData下I設置ple設置entation(const 軍的etwo本kSyncedData& Data)
{
	// P本ocess 本ecei正ed synced data
	UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Recei正ed synced data: %s"), *Data.DataId);
}

正oid UMin成RTS的etwo本kMana成e本::M使lticastPlaye本Joined下I設置ple設置entation(const 軍的etwo本kPlaye本Info& Playe本Info)
{
	ConnectedPlaye本s.Add(Playe本Info.Playe本Id, Playe本Info);
	UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本 大oined: %s"), *Playe本Info.Playe本的a設置e);
}

正oid UMin成RTS的etwo本kMana成e本::M使lticastPlaye本Left下I設置ple設置entation(const 軍的etwo本kPlaye本Info& Playe本Info)
{
	ConnectedPlaye本s.Re設置o正e(Playe本Info.Playe本Id);
	UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本 left: %s"), *Playe本Info.Playe本的a設置e);
}

正oid UMin成RTS的etwo本kMana成e本::M使lticastGa設置eSta本ted下I設置ple設置entation()
{
	ClientGa設置eSta本ted();
}
