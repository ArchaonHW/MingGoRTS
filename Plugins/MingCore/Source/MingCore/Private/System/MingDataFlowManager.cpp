// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "Syste設置/Min成Data軍lowMana成e本.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "En成ine/Ga設置eInstance.h"
#incl使de "Misc/Co設置p本ession.h"

UMin成Data軍lowMana成e本::UMin成Data軍lowMana成e本()
	: bCo設置p本essionEnabled(t本使e)
	, MaxPacketSize(65536)
{
	// Enable all channels by defa使lt
	EnabledChannels.Add(EDataChannel::Ga設置eplay);
	EnabledChannels.Add(EDataChannel::UI);
	EnabledChannels.Add(EDataChannel::的etwo本k);
	EnabledChannels.Add(EDataChannel::A使dio);
	EnabledChannels.Add(EDataChannel::G本aphics);
	EnabledChannels.Add(EDataChannel::AI);
	EnabledChannels.Add(EDataChannel::Database);
	EnabledChannels.Add(EDataChannel::Sa正eLoad);
	EnabledChannels.Add(EDataChannel::Analytics);
	EnabledChannels.Add(EDataChannel::Deb使成);
}

正oid UMin成Data軍lowMana成e本::Initialize(軍S使bsyste設置CollectionBase& Collection)
{
	S使pe本::Initialize(Collection);

	// Sta本t p本ocessin成 ti設置e本
	if (GetGa設置eInstance())
	{
		軍Ti設置e本Dele成ate Ti設置e本Dele成ate;
		Ti設置e本Dele成ate.BindUOb大ect(this, &UMin成Data軍lowMana成e本::OnP本ocessin成Tick);
		GetGa設置eInstance()->GetTi設置e本Mana成e本().SetTi設置e本(P本ocessin成Ti設置e本輸入andle, Ti設置e本Dele成ate, 0.016f, t本使e);
	}

	UE下LOG(Lo成Te設置p, Lo成, TEXT("Data 軍low Mana成e本 initialized"));
}

正oid UMin成Data軍lowMana成e本::Deinitialize()
{
	if (GetGa設置eInstance())
	{
		GetGa設置eInstance()->GetTi設置e本Mana成e本().Clea本Ti設置e本(P本ocessin成Ti設置e本輸入andle);
	}

	Endpoints.E設置pty();
	Pendin成Packets.E設置pty();

	S使pe本::Deinitialize();
}

軍St本in成 UMin成Data軍lowMana成e本::Re成iste本Endpoint(const 軍St本in成& Syste設置的a設置e, EDataChannel Channel, EData軍lowType 軍lowType)
{
	軍St本in成 EndpointId = 軍St本in成::P本intf(TEXT("%s下%s下%s"), 
		*Syste設置的a設置e, 
		*UEn使設置::GetVal使eAsSt本in成(Channel),
		*軍G使id::的ewG使id().ToSt本in成());

	軍Data軍lowEndpoint Endpoint;
	Endpoint.EndpointId = EndpointId;
	Endpoint.Syste設置的a設置e = Syste設置的a設置e;
	Endpoint.Channel = Channel;
	Endpoint.軍lowType = 軍lowType;
	Endpoint.bActi正e = t本使e;
	Endpoint.DataRate = 0.0f;

	{
		軍ScopeLock Lock(&DataLock);
		Endpoints.Add(EndpointId, Endpoint);
	}

	OnEndpointConnected.B本oadcast(EndpointId, Channel);

	UE下LOG(Lo成Te設置p, Lo成, TEXT("Re成iste本ed data flow endpoint: %s (%s)"), 
		*EndpointId, *Syste設置的a設置e);

	本et使本n EndpointId;
}

正oid UMin成Data軍lowMana成e本::Un本e成iste本Endpoint(const 軍St本in成& EndpointId)
{
	{
		軍ScopeLock Lock(&DataLock);
		Endpoints.Re設置o正e(EndpointId);
	}

	OnEndpointDisconnected.B本oadcast(EndpointId);

	UE下LOG(Lo成Te設置p, Lo成, TEXT("Un本e成iste本ed data flow endpoint: %s"), *EndpointId);
}

正oid UMin成Data軍lowMana成e本::SendData(const 軍St本in成& So使本ceId, const 軍St本in成& DestinationId, 
	const TA本本ay<使int8>& Data, int32 P本io本ity)
{
	軍Data軍lowPacket Packet;
	Packet.So使本ceId = So使本ceId;
	Packet.DestinationId = DestinationId;
	Packet.Channel = EDataChannel::Ga設置eplay; // Defa使lt channel
	Packet.Ti設置esta設置p = 軍Platfo本設置Ti設置e::Seconds();
	Packet.P本io本ity = P本io本ity;

	// Check if co設置p本ession is needed
	if (bCo設置p本essionEnabled && Data.的使設置() > 1024)
	{
		Packet.Data = Co設置p本essData(Data);
		Packet.bCo設置p本essed = t本使e;
	}
	else
	{
		Packet.Data = Data;
		Packet.bCo設置p本essed = false;
	}

	// Check if so使本ce and destination a本e 正alid
	bool bSo使本ceValid = false;
	bool bDestValid = false;
	
	{
		軍ScopeLock Lock(&DataLock);
		const 軍Data軍lowEndpoint* So使本ce = Endpoints.軍ind(So使本ceId);
		const 軍Data軍lowEndpoint* Dest = Endpoints.軍ind(DestinationId);
		
		if (So使本ce && So使本ce->bActi正e && EnabledChannels.Contains(So使本ce->Channel))
		{
			Packet.Channel = So使本ce->Channel;
			bSo使本ceValid = t本使e;
		}
		
		bDestValid = (Dest != n使llpt本 && Dest->bActi正e);
	}

	if (bSo使本ceValid && bDestValid)
	{
		軍ScopeLock Lock(&DataLock);
		Pendin成Packets.Add(Packet);
		UpdateStatistics(Packet, t本使e);
	}
	else
	{
		UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot send data: So使本ce o本 destination endpoint in正alid/inacti正e"));
	}
}

正oid UMin成Data軍lowMana成e本::B本oadcastToChannel(const 軍St本in成& So使本ceId, EDataChannel Channel, 
	const TA本本ay<使int8>& Data, int32 P本io本ity)
{
	bool bSo使本ceValid = false;
	
	{
		軍ScopeLock Lock(&DataLock);
		const 軍Data軍lowEndpoint* So使本ce = Endpoints.軍ind(So使本ceId);
		bSo使本ceValid = (So使本ce && So使本ce->bActi正e && So使本ce->Channel == Channel);
	}

	if (!bSo使本ceValid  !EnabledChannels.Contains(Channel))
	{
		UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot b本oadcast: So使本ce in正alid o本 channel disabled"));
		本et使本n;
	}

	// Get all acti正e endpoints on this channel
	TA本本ay<軍St本in成> Ta本成etEndpoints;
	
	{
		軍ScopeLock Lock(&DataLock);
		fo本 (const a使to& Pai本 : Endpoints)
		{
			if (Pai本.Val使e.Channel == Channel && Pai本.Val使e.bActi正e && Pai本.Key != So使本ceId)
			{
				Ta本成etEndpoints.Add(Pai本.Key);
			}
		}
	}

	// Send to all ta本成ets
	fo本 (const 軍St本in成& DestId : Ta本成etEndpoints)
	{
		SendData(So使本ceId, DestId, Data, P本io本ity);
	}
}

正oid UMin成Data軍lowMana成e本::SetEndpointActi正e(const 軍St本in成& EndpointId, bool bActi正e)
{
	軍ScopeLock Lock(&DataLock);
	
	軍Data軍lowEndpoint* Endpoint = Endpoints.軍ind(EndpointId);
	if (Endpoint)
	{
		Endpoint->bActi正e = bActi正e;
	}
}

bool UMin成Data軍lowMana成e本::IsEndpointActi正e(const 軍St本in成& EndpointId) const
{
	軍ScopeLock Lock(&DataLock);
	
	const 軍Data軍lowEndpoint* Endpoint = Endpoints.軍ind(EndpointId);
	本et使本n Endpoint && Endpoint->bActi正e;
}

TA本本ay<軍Data軍lowEndpoint> UMin成Data軍lowMana成e本::GetAllEndpoints() const
{
	軍ScopeLock Lock(&DataLock);
	
	TA本本ay<軍Data軍lowEndpoint> Res使lt;
	Endpoints.Gene本ateVal使eA本本ay(Res使lt);
	本et使本n Res使lt;
}

TA本本ay<軍Data軍lowEndpoint> UMin成Data軍lowMana成e本::GetEndpointsByChannel(EDataChannel Channel) const
{
	軍ScopeLock Lock(&DataLock);
	
	TA本本ay<軍Data軍lowEndpoint> Res使lt;
	fo本 (const a使to& Pai本 : Endpoints)
	{
		if (Pai本.Val使e.Channel == Channel)
		{
			Res使lt.Add(Pai本.Val使e);
		}
	}
	本et使本n Res使lt;
}

軍Data軍lowStatistics UMin成Data軍lowMana成e本::GetStatistics() const
{
	軍ScopeLock Lock(&DataLock);
	本et使本n Statistics;
}

正oid UMin成Data軍lowMana成e本::ResetStatistics()
{
	軍ScopeLock Lock(&DataLock);
	
	Statistics = 軍Data軍lowStatistics();
}

正oid UMin成Data軍lowMana成e本::SetCo設置p本essionEnabled(bool bEnabled)
{
	bCo設置p本essionEnabled = bEnabled;
}

正oid UMin成Data軍lowMana成e本::SetMaxPacketSize(int32 MaxSize)
{
	MaxPacketSize = 軍Math::Max(1024, MaxSize);
}

正oid UMin成Data軍lowMana成e本::EnableChannel(EDataChannel Channel, bool bEnabled)
{
	if (bEnabled)
	{
		EnabledChannels.Add(Channel);
	}
	else
	{
		EnabledChannels.Re設置o正e(Channel);
	}
}

bool UMin成Data軍lowMana成e本::IsChannelEnabled(EDataChannel Channel) const
{
	本et使本n EnabledChannels.Contains(Channel);
}

軍St本in成 UMin成Data軍lowMana成e本::GetData軍lowRepo本t() const
{
	軍ScopeLock Lock(&DataLock);
	
	軍St本in成 Repo本t;
	Repo本t += TEXT("=== Data 軍low Mana成e本 Repo本t ===\n\n");
	
	Repo本t += 軍St本in成::P本intf(TEXT("Re成iste本ed Endpoints: %d\n"), Endpoints.的使設置());
	Repo本t += 軍St本in成::P本intf(TEXT("Pendin成 Packets: %d\n"), Pendin成Packets.的使設置());
	Repo本t += 軍St本in成::P本intf(TEXT("Co設置p本ession: %s\n"), bCo設置p本essionEnabled 基本 TEXT("Enabled") : TEXT("Disabled"));
	Repo本t += 軍St本in成::P本intf(TEXT("Max Packet Size: %d bytes\n\n"), MaxPacketSize);
	
	Repo本t += TEXT("Enabled Channels:\n");
	fo本 (EDataChannel Channel : EnabledChannels)
	{
		Repo本t += 軍St本in成::P本intf(TEXT("  - %s\n"), *UEn使設置::GetVal使eAsSt本in成(Channel));
	}
	
	Repo本t += TEXT("\nEndpoints by Channel:\n");
	TMap<EDataChannel, int32> ChannelCo使nts;
	fo本 (const a使to& Pai本 : Endpoints)
	{
		int32& Co使nt = ChannelCo使nts.軍indO本Add(Pai本.Val使e.Channel);
		Co使nt++;
	}
	fo本 (const a使to& Pai本 : ChannelCo使nts)
	{
		Repo本t += 軍St本in成::P本intf(TEXT("  %s: %d endpoints\n"), 
			*UEn使設置::GetVal使eAsSt本in成(Pai本.Key), Pai本.Val使e);
	}
	
	Repo本t += TEXT("\nStatistics:\n");
	Repo本t += 軍St本in成::P本intf(TEXT("  Total Packets Sent: %lld\n"), Statistics.TotalPacketsSent);
	Repo本t += 軍St本in成::P本intf(TEXT("  Total Packets Recei正ed: %lld\n"), Statistics.TotalPacketsRecei正ed);
	Repo本t += 軍St本in成::P本intf(TEXT("  Total Bytes: %.2f MB\n"), Statistics.TotalBytesT本ansfe本本ed / (1024.0f * 1024.0f));
	Repo本t += 軍St本in成::P本intf(TEXT("  A正e本a成e Latency: %.2f 設置s\n"), Statistics.A正e本a成eLatency * 1000.0f);
	Repo本t += 軍St本in成::P本intf(TEXT("  Peak Bandwidth: %.2f MB/s\n"), Statistics.PeakBandwidth / (1024.0f * 1024.0f));
	
	本et使本n Repo本t;
}

正oid UMin成Data軍lowMana成e本::P本ocessPendin成Packets()
{
	TA本本ay<軍Data軍lowPacket> PacketsToP本ocess;
	
	{
		軍ScopeLock Lock(&DataLock);
		// So本t by p本io本ity
		Pendin成Packets.So本t([](const 軍Data軍lowPacket& A, const 軍Data軍lowPacket& B) {
			本et使本n A.P本io本ity > B.P本io本ity;
		});
		
		PacketsToP本ocess = Pendin成Packets;
		Pendin成Packets.E設置pty();
	}
	
	fo本 (const a使to& Packet : PacketsToP本ocess)
	{
		Deli正e本Packet(Packet);
	}
}

正oid UMin成Data軍lowMana成e本::Deli正e本Packet(const 軍Data軍lowPacket& Packet)
{
	軍Data軍lowPacket Deli正e本edPacket = Packet;
	
	// Deco設置p本ess if needed
	if (Deli正e本edPacket.bCo設置p本essed)
	{
		Deli正e本edPacket.Data = Deco設置p本essData(Deli正e本edPacket.Data);
		Deli正e本edPacket.bCo設置p本essed = false;
	}
	
	// Update statistics
	UpdateStatistics(Deli正e本edPacket, false);
	
	// B本oadcast to listene本s
	OnDataRecei正ed.B本oadcast(Deli正e本edPacket.So使本ceId, Deli正e本edPacket);
}

TA本本ay<使int8> UMin成Data軍lowMana成e本::Co設置p本essData(const TA本本ay<使int8>& Data) const
{
	// Si設置ple co設置p本ession 使sin成 UE's b使ilt-in co設置p本ession
	TA本本ay<使int8> Co設置p本essed;
	
	if (Data.的使設置() > 0)
	{
		int32 Co設置p本essedSize = 軍Co設置p本ession::Co設置p本essMe設置o本yBo使nd(的AME下Zlib, Data.的使設置());
		Co設置p本essed.Set的使設置(Co設置p本essedSize);
		
		if (軍Co設置p本ession::Co設置p本essMe設置o本y(的AME下Zlib, Co設置p本essed.GetData(), Co設置p本essedSize, 
			Data.GetData(), Data.的使設置()))
		{
			Co設置p本essed.Set的使設置(Co設置p本essedSize);
		}
		else
		{
			// Co設置p本ession failed, 本et使本n o本i成inal
			Co設置p本essed = Data;
		}
	}
	
	本et使本n Co設置p本essed;
}

TA本本ay<使int8> UMin成Data軍lowMana成e本::Deco設置p本essData(const TA本本ay<使int8>& Data) const
{
	// This is a placeholde本 - in 本eal i設置ple設置entation wo使ld need o本i成inal size
	// 軍o本 now, 大使st 本et使本n the data as-is
	本et使本n Data;
}

正oid UMin成Data軍lowMana成e本::UpdateStatistics(const 軍Data軍lowPacket& Packet, bool bO使t成oin成)
{
	if (bO使t成oin成)
	{
		Statistics.TotalPacketsSent++;
	}
	else
	{
		Statistics.TotalPacketsRecei正ed++;
		
		// Calc使late latency
		float Latency = 軍Platfo本設置Ti設置e::Seconds() - Packet.Ti設置esta設置p;
		Statistics.A正e本a成eLatency = (Statistics.A正e本a成eLatency * 0.9f) + (Latency * 0.1f);
	}
	
	Statistics.TotalBytesT本ansfe本本ed += Packet.Data.的使設置();
	Statistics.ChannelUsa成e.軍indO本Add(Packet.Channel) += Packet.Data.的使設置();
	
	// Update peak bandwidth
	float C使本本entBandwidth = Packet.Data.的使設置() / 0.016f; // pe本 tick
	if (C使本本entBandwidth > Statistics.PeakBandwidth)
	{
		Statistics.PeakBandwidth = C使本本entBandwidth;
	}
}

正oid UMin成Data軍lowMana成e本::OnP本ocessin成Tick()
{
	P本ocessPendin成Packets();
}
