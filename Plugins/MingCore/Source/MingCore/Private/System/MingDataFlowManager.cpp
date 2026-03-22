// Copyright Epic Games, Inc. All Rights Reserved.

#include "System/MingDataFlowManager.h"
#include "TimerManager.h"
#include "Engine/GameInstance.h"
#include "Misc/Compression.h"

UMingDataFlowManager::UMingDataFlowManager()
	: bCompressionEnabled(true)
	, MaxPacketSize(65536)
{
	// Enable all channels by default
	EnabledChannels.Add(EDataChannel::Gameplay);
	EnabledChannels.Add(EDataChannel::UI);
	EnabledChannels.Add(EDataChannel::Network);
	EnabledChannels.Add(EDataChannel::Audio);
	EnabledChannels.Add(EDataChannel::Graphics);
	EnabledChannels.Add(EDataChannel::AI);
	EnabledChannels.Add(EDataChannel::Database);
	EnabledChannels.Add(EDataChannel::SaveLoad);
	EnabledChannels.Add(EDataChannel::Analytics);
	EnabledChannels.Add(EDataChannel::Debug);
}

void UMingDataFlowManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Start processing timer
	if (GetGameInstance())
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &UMingDataFlowManager::OnProcessingTick);
		GetGameInstance()->GetTimerManager().SetTimer(ProcessingTimerHandle, TimerDelegate, 0.016f, true);
	}

	UE_LOG(LogTemp, Log, TEXT("Data Flow Manager initialized"));
}

void UMingDataFlowManager::Deinitialize()
{
	if (GetGameInstance())
	{
		GetGameInstance()->GetTimerManager().ClearTimer(ProcessingTimerHandle);
	}

	Endpoints.Empty();
	PendingPackets.Empty();

	Super::Deinitialize();
}

FString UMingDataFlowManager::RegisterEndpoint(const FString& SystemName, EDataChannel Channel, EDataFlowType FlowType)
{
	FString EndpointId = FString::Printf(TEXT("%s_%s_%s"), 
		*SystemName, 
		*UEnum::GetValueAsString(Channel),
		*FGuid::NewGuid().ToString());

	FDataFlowEndpoint Endpoint;
	Endpoint.EndpointId = EndpointId;
	Endpoint.SystemName = SystemName;
	Endpoint.Channel = Channel;
	Endpoint.FlowType = FlowType;
	Endpoint.bActive = true;
	Endpoint.DataRate = 0.0f;

	{
		FScopeLock Lock(&DataLock);
		Endpoints.Add(EndpointId, Endpoint);
	}

	OnEndpointConnected.Broadcast(EndpointId, Channel);

	UE_LOG(LogTemp, Log, TEXT("Registered data flow endpoint: %s (%s)"), 
		*EndpointId, *SystemName);

	return EndpointId;
}

void UMingDataFlowManager::UnregisterEndpoint(const FString& EndpointId)
{
	{
		FScopeLock Lock(&DataLock);
		Endpoints.Remove(EndpointId);
	}

	OnEndpointDisconnected.Broadcast(EndpointId);

	UE_LOG(LogTemp, Log, TEXT("Unregistered data flow endpoint: %s"), *EndpointId);
}

void UMingDataFlowManager::SendData(const FString& SourceId, const FString& DestinationId, 
	const TArray<uint8>& Data, int32 Priority)
{
	FDataFlowPacket Packet;
	Packet.SourceId = SourceId;
	Packet.DestinationId = DestinationId;
	Packet.Channel = EDataChannel::Gameplay; // Default channel
	Packet.Timestamp = FPlatformTime::Seconds();
	Packet.Priority = Priority;

	// Check if compression is needed
	if (bCompressionEnabled && Data.Num() > 1024)
	{
		Packet.Data = CompressData(Data);
		Packet.bCompressed = true;
	}
	else
	{
		Packet.Data = Data;
		Packet.bCompressed = false;
	}

	// Check if source and destination are valid
	bool bSourceValid = false;
	bool bDestValid = false;
	
	{
		FScopeLock Lock(&DataLock);
		const FDataFlowEndpoint* Source = Endpoints.Find(SourceId);
		const FDataFlowEndpoint* Dest = Endpoints.Find(DestinationId);
		
		if (Source && Source->bActive && EnabledChannels.Contains(Source->Channel))
		{
			Packet.Channel = Source->Channel;
			bSourceValid = true;
		}
		
		bDestValid = (Dest != nullptr && Dest->bActive);
	}

	if (bSourceValid && bDestValid)
	{
		FScopeLock Lock(&DataLock);
		PendingPackets.Add(Packet);
		UpdateStatistics(Packet, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot send data: Source or destination endpoint invalid/inactive"));
	}
}

void UMingDataFlowManager::BroadcastToChannel(const FString& SourceId, EDataChannel Channel, 
	const TArray<uint8>& Data, int32 Priority)
{
	bool bSourceValid = false;
	
	{
		FScopeLock Lock(&DataLock);
		const FDataFlowEndpoint* Source = Endpoints.Find(SourceId);
		bSourceValid = (Source && Source->bActive && Source->Channel == Channel);
	}

	if (!bSourceValid || !EnabledChannels.Contains(Channel))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot broadcast: Source invalid or channel disabled"));
		return;
	}

	// Get all active endpoints on this channel
	TArray<FString> TargetEndpoints;
	
	{
		FScopeLock Lock(&DataLock);
		for (const auto& Pair : Endpoints)
		{
			if (Pair.Value.Channel == Channel && Pair.Value.bActive && Pair.Key != SourceId)
			{
				TargetEndpoints.Add(Pair.Key);
			}
		}
	}

	// Send to all targets
	for (const FString& DestId : TargetEndpoints)
	{
		SendData(SourceId, DestId, Data, Priority);
	}
}

void UMingDataFlowManager::SetEndpointActive(const FString& EndpointId, bool bActive)
{
	FScopeLock Lock(&DataLock);
	
	FDataFlowEndpoint* Endpoint = Endpoints.Find(EndpointId);
	if (Endpoint)
	{
		Endpoint->bActive = bActive;
	}
}

bool UMingDataFlowManager::IsEndpointActive(const FString& EndpointId) const
{
	FScopeLock Lock(&DataLock);
	
	const FDataFlowEndpoint* Endpoint = Endpoints.Find(EndpointId);
	return Endpoint && Endpoint->bActive;
}

TArray<FDataFlowEndpoint> UMingDataFlowManager::GetAllEndpoints() const
{
	FScopeLock Lock(&DataLock);
	
	TArray<FDataFlowEndpoint> Result;
	Endpoints.GenerateValueArray(Result);
	return Result;
}

TArray<FDataFlowEndpoint> UMingDataFlowManager::GetEndpointsByChannel(EDataChannel Channel) const
{
	FScopeLock Lock(&DataLock);
	
	TArray<FDataFlowEndpoint> Result;
	for (const auto& Pair : Endpoints)
	{
		if (Pair.Value.Channel == Channel)
		{
			Result.Add(Pair.Value);
		}
	}
	return Result;
}

FDataFlowStatistics UMingDataFlowManager::GetStatistics() const
{
	FScopeLock Lock(&DataLock);
	return Statistics;
}

void UMingDataFlowManager::ResetStatistics()
{
	FScopeLock Lock(&DataLock);
	
	Statistics = FDataFlowStatistics();
}

void UMingDataFlowManager::SetCompressionEnabled(bool bEnabled)
{
	bCompressionEnabled = bEnabled;
}

void UMingDataFlowManager::SetMaxPacketSize(int32 MaxSize)
{
	MaxPacketSize = FMath::Max(1024, MaxSize);
}

void UMingDataFlowManager::EnableChannel(EDataChannel Channel, bool bEnabled)
{
	if (bEnabled)
	{
		EnabledChannels.Add(Channel);
	}
	else
	{
		EnabledChannels.Remove(Channel);
	}
}

bool UMingDataFlowManager::IsChannelEnabled(EDataChannel Channel) const
{
	return EnabledChannels.Contains(Channel);
}

FString UMingDataFlowManager::GetDataFlowReport() const
{
	FScopeLock Lock(&DataLock);
	
	FString Report;
	Report += TEXT("=== Data Flow Manager Report ===\n\n");
	
	Report += FString::Printf(TEXT("Registered Endpoints: %d\n"), Endpoints.Num());
	Report += FString::Printf(TEXT("Pending Packets: %d\n"), PendingPackets.Num());
	Report += FString::Printf(TEXT("Compression: %s\n"), bCompressionEnabled ? TEXT("Enabled") : TEXT("Disabled"));
	Report += FString::Printf(TEXT("Max Packet Size: %d bytes\n\n"), MaxPacketSize);
	
	Report += TEXT("Enabled Channels:\n");
	for (EDataChannel Channel : EnabledChannels)
	{
		Report += FString::Printf(TEXT("  - %s\n"), *UEnum::GetValueAsString(Channel));
	}
	
	Report += TEXT("\nEndpoints by Channel:\n");
	TMap<EDataChannel, int32> ChannelCounts;
	for (const auto& Pair : Endpoints)
	{
		int32& Count = ChannelCounts.FindOrAdd(Pair.Value.Channel);
		Count++;
	}
	for (const auto& Pair : ChannelCounts)
	{
		Report += FString::Printf(TEXT("  %s: %d endpoints\n"), 
			*UEnum::GetValueAsString(Pair.Key), Pair.Value);
	}
	
	Report += TEXT("\nStatistics:\n");
	Report += FString::Printf(TEXT("  Total Packets Sent: %lld\n"), Statistics.TotalPacketsSent);
	Report += FString::Printf(TEXT("  Total Packets Received: %lld\n"), Statistics.TotalPacketsReceived);
	Report += FString::Printf(TEXT("  Total Bytes: %.2f MB\n"), Statistics.TotalBytesTransferred / (1024.0f * 1024.0f));
	Report += FString::Printf(TEXT("  Average Latency: %.2f ms\n"), Statistics.AverageLatency * 1000.0f);
	Report += FString::Printf(TEXT("  Peak Bandwidth: %.2f MB/s\n"), Statistics.PeakBandwidth / (1024.0f * 1024.0f));
	
	return Report;
}

void UMingDataFlowManager::ProcessPendingPackets()
{
	TArray<FDataFlowPacket> PacketsToProcess;
	
	{
		FScopeLock Lock(&DataLock);
		// Sort by priority
		PendingPackets.Sort([](const FDataFlowPacket& A, const FDataFlowPacket& B) {
			return A.Priority > B.Priority;
		});
		
		PacketsToProcess = PendingPackets;
		PendingPackets.Empty();
	}
	
	for (const auto& Packet : PacketsToProcess)
	{
		DeliverPacket(Packet);
	}
}

void UMingDataFlowManager::DeliverPacket(const FDataFlowPacket& Packet)
{
	FDataFlowPacket DeliveredPacket = Packet;
	
	// Decompress if needed
	if (DeliveredPacket.bCompressed)
	{
		DeliveredPacket.Data = DecompressData(DeliveredPacket.Data);
		DeliveredPacket.bCompressed = false;
	}
	
	// Update statistics
	UpdateStatistics(DeliveredPacket, false);
	
	// Broadcast to listeners
	OnDataReceived.Broadcast(DeliveredPacket.SourceId, DeliveredPacket);
}

TArray<uint8> UMingDataFlowManager::CompressData(const TArray<uint8>& Data) const
{
	// Simple compression using UE's built-in compression
	TArray<uint8> Compressed;
	
	if (Data.Num() > 0)
	{
		int32 CompressedSize = FCompression::CompressMemoryBound(NAME_Zlib, Data.Num());
		Compressed.SetNum(CompressedSize);
		
		if (FCompression::CompressMemory(NAME_Zlib, Compressed.GetData(), CompressedSize, 
			Data.GetData(), Data.Num()))
		{
			Compressed.SetNum(CompressedSize);
		}
		else
		{
			// Compression failed, return original
			Compressed = Data;
		}
	}
	
	return Compressed;
}

TArray<uint8> UMingDataFlowManager::DecompressData(const TArray<uint8>& Data) const
{
	// This is a placeholder - in real implementation would need original size
	// For now, just return the data as-is
	return Data;
}

void UMingDataFlowManager::UpdateStatistics(const FDataFlowPacket& Packet, bool bOutgoing)
{
	if (bOutgoing)
	{
		Statistics.TotalPacketsSent++;
	}
	else
	{
		Statistics.TotalPacketsReceived++;
		
		// Calculate latency
		float Latency = FPlatformTime::Seconds() - Packet.Timestamp;
		Statistics.AverageLatency = (Statistics.AverageLatency * 0.9f) + (Latency * 0.1f);
	}
	
	Statistics.TotalBytesTransferred += Packet.Data.Num();
	Statistics.ChannelUsage.FindOrAdd(Packet.Channel) += Packet.Data.Num();
	
	// Update peak bandwidth
	float CurrentBandwidth = Packet.Data.Num() / 0.016f; // per tick
	if (CurrentBandwidth > Statistics.PeakBandwidth)
	{
		Statistics.PeakBandwidth = CurrentBandwidth;
	}
}

void UMingDataFlowManager::OnProcessingTick()
{
	ProcessPendingPackets();
}
