// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingDataFlowManager.generated.h"

UENUM(BlueprintType)
enum class EDataFlowType : uint8
{
	Input,
	Output,
	Bidirectional
};

UENUM(BlueprintType)
enum class EDataChannel : uint8
{
	Gameplay,
	UI,
	Network,
	Audio,
	Graphics,
	AI,
	Database,
	SaveLoad,
	Analytics,
	Debug
};

USTRUCT(BlueprintType)
struct FDataFlowEndpoint
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "DataFlow")
	FString EndpointId;

	UPROPERTY(BlueprintReadWrite, Category = "DataFlow")
	FString SystemName;

	UPROPERTY(BlueprintReadWrite, Category = "DataFlow")
	EDataChannel Channel;

	UPROPERTY(BlueprintReadWrite, Category = "DataFlow")
	EDataFlowType FlowType;

	UPROPERTY(BlueprintReadWrite, Category = "DataFlow")
	bool bActive;

	UPROPERTY(BlueprintReadWrite, Category = "DataFlow")
	float DataRate;

	FDataFlowEndpoint()
		: Channel(EDataChannel::Gameplay)
		, FlowType(EDataFlowType::Bidirectional)
		, bActive(true)
		, DataRate(0.0f)
	{
	}
};

USTRUCT(BlueprintType)
struct FDataFlowPacket
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "DataFlow")
	FGuid PacketId;

	UPROPERTY(BlueprintReadOnly, Category = "DataFlow")
	FString SourceId;

	UPROPERTY(BlueprintReadOnly, Category = "DataFlow")
	FString DestinationId;

	UPROPERTY(BlueprintReadOnly, Category = "DataFlow")
	EDataChannel Channel;

	UPROPERTY(BlueprintReadOnly, Category = "DataFlow")
	TArray<uint8> Data;

	UPROPERTY(BlueprintReadOnly, Category = "DataFlow")
	float Timestamp;

	UPROPERTY(BlueprintReadOnly, Category = "DataFlow")
	int32 Priority;

	UPROPERTY(BlueprintReadOnly, Category = "DataFlow")
	bool bCompressed;

	FDataFlowPacket()
		: PacketId(FGuid::NewGuid())
		, Channel(EDataChannel::Gameplay)
		, Timestamp(0.0f)
		, Priority(0)
		, bCompressed(false)
	{
	}
};

USTRUCT(BlueprintType)
struct FDataFlowStatistics
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "DataFlow")
	int64 TotalPacketsSent;

	UPROPERTY(BlueprintReadOnly, Category = "DataFlow")
	int64 TotalPacketsReceived;

	UPROPERTY(BlueprintReadOnly, Category = "DataFlow")
	int64 TotalBytesTransferred;

	UPROPERTY(BlueprintReadOnly, Category = "DataFlow")
	float AverageLatency;

	UPROPERTY(BlueprintReadOnly, Category = "DataFlow")
	float PeakBandwidth;

	UPROPERTY(BlueprintReadOnly, Category = "DataFlow")
	TMap<EDataChannel, int64> ChannelUsage;

	FDataFlowStatistics()
		: TotalPacketsSent(0)
		, TotalPacketsReceived(0)
		, TotalBytesTransferred(0)
		, AverageLatency(0.0f)
		, PeakBandwidth(0.0f)
	{
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDataReceived, const FString&, SourceId, const FDataFlowPacket&, Packet);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEndpointConnected, const FString&, EndpointId, EDataChannel, Channel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndpointDisconnected, const FString&, EndpointId);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGCORE_API UMingDataFlowManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMingDataFlowManager();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "DataFlow")
	FString RegisterEndpoint(const FString& SystemName, EDataChannel Channel, EDataFlowType FlowType);

	UFUNCTION(BlueprintCallable, Category = "DataFlow")
	void UnregisterEndpoint(const FString& EndpointId);

	UFUNCTION(BlueprintCallable, Category = "DataFlow")
	void SendData(const FString& SourceId, const FString& DestinationId, const TArray<uint8>& Data, int32 Priority = 0);

	UFUNCTION(BlueprintCallable, Category = "DataFlow")
	void BroadcastToChannel(const FString& SourceId, EDataChannel Channel, const TArray<uint8>& Data, int32 Priority = 0);

	UFUNCTION(BlueprintCallable, Category = "DataFlow")
	void SetEndpointActive(const FString& EndpointId, bool bActive);

	UFUNCTION(BlueprintCallable, Category = "DataFlow")
	bool IsEndpointActive(const FString& EndpointId) const;

	UFUNCTION(BlueprintCallable, Category = "DataFlow")
	TArray<FDataFlowEndpoint> GetAllEndpoints() const;

	UFUNCTION(BlueprintCallable, Category = "DataFlow")
	TArray<FDataFlowEndpoint> GetEndpointsByChannel(EDataChannel Channel) const;

	UFUNCTION(BlueprintCallable, Category = "DataFlow")
	FDataFlowStatistics GetStatistics() const;

	UFUNCTION(BlueprintCallable, Category = "DataFlow")
	void ResetStatistics();

	UFUNCTION(BlueprintCallable, Category = "DataFlow")
	void SetCompressionEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "DataFlow")
	void SetMaxPacketSize(int32 MaxSize);

	UFUNCTION(BlueprintCallable, Category = "DataFlow")
	void EnableChannel(EDataChannel Channel, bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "DataFlow")
	bool IsChannelEnabled(EDataChannel Channel) const;

	UFUNCTION(BlueprintCallable, Category = "DataFlow")
	FString GetDataFlowReport() const;

public:
	UPROPERTY(BlueprintAssignable, Category = "DataFlow|Events")
	FOnDataReceived OnDataReceived;

	UPROPERTY(BlueprintAssignable, Category = "DataFlow|Events")
	FOnEndpointConnected OnEndpointConnected;

	UPROPERTY(BlueprintAssignable, Category = "DataFlow|Events")
	FOnEndpointDisconnected OnEndpointDisconnected;

protected:
	UPROPERTY()
	TMap<FString, FDataFlowEndpoint> Endpoints;

	UPROPERTY()
	TArray<FDataFlowPacket> PendingPackets;

	UPROPERTY()
	FDataFlowStatistics Statistics;

	UPROPERTY()
	TSet<EDataChannel> EnabledChannels;

	UPROPERTY()
	bool bCompressionEnabled;

	UPROPERTY()
	int32 MaxPacketSize;

	UPROPERTY()
	FTimerHandle ProcessingTimerHandle;

	UPROPERTY()
	mutable FCriticalSection DataLock;

protected:
	void ProcessPendingPackets();
	void DeliverPacket(const FDataFlowPacket& Packet);
	TArray<uint8> CompressData(const TArray<uint8>& Data) const;
	TArray<uint8> DecompressData(const TArray<uint8>& Data) const;
	void UpdateStatistics(const FDataFlowPacket& Packet, bool bOutgoing);
	void OnProcessingTick();
};
