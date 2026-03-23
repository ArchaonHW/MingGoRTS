// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Cloud Services System - Epic 10.1
//
// This system provides cloud-based save synchronization,
// AI computation offloading, and player analytics.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MingCloudServices.generated.h"

UENUM(BlueprintType)
enum class ECloudServiceType : uint8 {
    SaveSync            UMETA(DisplayName = "Save Sync"),
    AIComputation       UMETA(DisplayName = "AI Computation"),
    Analytics           UMETA(DisplayName = "Analytics"),
    AssetStreaming      UMETA(DisplayName = "Asset Streaming"),
    Multiplayer         UMETA(DisplayName = "Multiplayer"),
    Matchmaking         UMETA(DisplayName = "Matchmaking"),
    Leaderboards        UMETA(DisplayName = "Leaderboards"),
    Achievements        UMETA(DisplayName = "Achievements")
};

UENUM(BlueprintType)
enum class ECloudConnectionStatus : uint8 {
    Disconnected        UMETA(DisplayName = "Disconnected"),
    Connecting          UMETA(DisplayName = "Connecting"),
    Connected           UMETA(DisplayName = "Connected"),
    Reconnecting        UMETA(DisplayName = "Reconnecting"),
    Error               UMETA(DisplayName = "Error")
};

USTRUCT(BlueprintType)
struct FCloudServiceConfig {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECloudServiceType ServiceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ServerURL;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Port;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUseSSL;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString APIKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeoutSeconds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxRetries;

    FCloudServiceConfig()
        : ServiceType(ECloudServiceType::SaveSync)
        , Port(443)
        , bUseSSL(true)
        , TimeoutSeconds(30.0f)
        , MaxRetries(3) {}
};

USTRUCT(BlueprintType)
struct FCloudSaveData {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString SaveID;

    UPROPERTY(BlueprintReadOnly)
    FString PlayerID;

    UPROPERTY(BlueprintReadOnly)
    TArray<uint8> SaveData;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    FString Checksum;

    UPROPERTY(BlueprintReadOnly)
    int32 Version;

    FCloudSaveData()
        : Version(1) {}
};

USTRUCT(BlueprintType)
struct FAIComputationRequest {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString RequestID;

    UPROPERTY(BlueprintReadOnly)
    FString RequestType;

    UPROPERTY(BlueprintReadOnly)
    TArray<uint8> InputData;

    UPROPERTY(BlueprintReadOnly)
    int32 Priority;

    UPROPERTY(BlueprintReadOnly)
    float MaxWaitTime;

    FAIComputationRequest()
        : Priority(0)
        , MaxWaitTime(5.0f) {}
};

USTRUCT(BlueprintType)
struct FAnalyticsEvent {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    FString EventType;

    UPROPERTY(BlueprintReadOnly)
    FString PlayerID;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> EventData;

    UPROPERTY(BlueprintReadOnly)
    FString SessionID;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCloudConnected, ECloudServiceType, ServiceType, FString, ServerInfo};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCloudDisconnected, ECloudServiceType, ServiceType, FString, Reason};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCloudSaveUploaded, FString, SaveID, bool, bSuccess, FString, Message};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCloudSaveDownloaded, FString, SaveID, bool, bSuccess, TArray<uint8>, Data};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIComputationComplete, FString, RequestID, FAIComputationRequest, Result};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnalyticsEventSent, FString, EventID};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGPERSONAL_API UMingCloudServices : public UObject {
    GENERATED_BODY()

public:
    UMingCloudServices(};

    UFUNCTION(BlueprintCallable, Category = "Cloud Services")
    void InitializeCloudServices(};

    UFUNCTION(BlueprintCallable, Category = "Cloud Services")
    void ShutdownCloudServices(};

    UFUNCTION(BlueprintCallable, Category = "Cloud Services Connection")
    void ConnectToService(ECloudServiceType ServiceType};

    UFUNCTION(BlueprintCallable, Category = "Cloud Services Connection")
    void DisconnectFromService(ECloudServiceType ServiceType};

    UFUNCTION(BlueprintPure, Category = "Cloud Services Connection")
    bool IsConnected(ECloudServiceType ServiceType) const;

    UFUNCTION(BlueprintPure, Category = "Cloud Services Connection")
    ECloudConnectionStatus GetConnectionStatus(ECloudServiceType ServiceType) const;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Services Events")
    FOnCloudConnected OnCloudConnected;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Services Events")
    FOnCloudDisconnected OnCloudDisconnected;

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    void UploadSave(const FString& SaveID, const TArray<uint8>& SaveData};

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    void DownloadSave(const FString& SaveID};

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    void DeleteCloudSave(const FString& SaveID};

    UFUNCTION(BlueprintCallable, Category = "Cloud Save")
    TArray<FCloudSaveData> ListCloudSaves(const FString& PlayerID) const;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Services Events")
    FOnCloudSaveUploaded OnCloudSaveUploaded;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Services Events")
    FOnCloudSaveDownloaded OnCloudSaveDownloaded;

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    FString RequestAIComputation(const FAIComputationRequest& Request};

    UFUNCTION(BlueprintCallable, Category = "Cloud AI")
    void CancelAIComputation(const FString& RequestID};

    UFUNCTION(BlueprintPure, Category = "Cloud AI")
    bool IsAIComputationComplete(const FString& RequestID) const;

    UPROPERTY(BlueprintAssignable, Category = "Cloud Services Events")
    FOnAIComputationComplete OnAIComputationComplete;

    UFUNCTION(BlueprintCallable, Category = "Cloud Analytics")
    void SendAnalyticsEvent(const FAnalyticsEvent& Event};

    UFUNCTION(BlueprintCallable, Category = "Cloud Analytics")
    void FlushAnalyticsBuffer(};

    UPROPERTY(BlueprintAssignable, Category = "Cloud Services Events")
    FOnAnalyticsEventSent OnAnalyticsEventSent;

    UFUNCTION(BlueprintCallable, Category = "Cloud Services Config")
    void SetServiceConfig(const FCloudServiceConfig& Config};

    UFUNCTION(BlueprintPure, Category = "Cloud Services Config")
    FCloudServiceConfig GetServiceConfig(ECloudServiceType ServiceType) const;

    UFUNCTION(BlueprintCallable, Category = "Cloud Services")
    void SetPlayerCredentials(const FString& PlayerID, const FString& AuthToken};

    UFUNCTION(BlueprintCallable, Category = "Cloud Services")
    void Tick(float DeltaTime};

protected:
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    TMap<ECloudServiceType, ECloudConnectionStatus> ConnectionStatuses;

    UPROPERTY()
    TMap<ECloudServiceType, FCloudServiceConfig> ServiceConfigs;

    UPROPERTY()
    FString CurrentPlayerID;

    UPROPERTY()
    FString CurrentAuthToken;

    UPROPERTY()
    TArray<FAnalyticsEvent> AnalyticsBuffer;

    UPROPERTY()
    TMap<FString, FAIComputationRequest> PendingAIRequests;

    void ProcessConnectionUpdates(float DeltaTime};
    void ProcessPendingAIRequests(float DeltaTime};
    void FlushAnalyticsIfNeeded(};
    bool ValidateConnection(ECloudServiceType ServiceType) const;
    FString ComputeChecksum(const TArray<uint8>& Data) const;
    void HandleConnectionError(ECloudServiceType ServiceType, const FString& Error};
};
