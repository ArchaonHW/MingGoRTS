#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSInterProcessCommunication.generated.h"

UENUM(BlueprintType)
enum class EMessageType : uint8
{
    Command,        // 命令消息
    Response,       // 響應消息
    Event,          // 事件消息
    Data,           // 數據消息
    Heartbeat,      // 心跳消息
    Error,          // 錯誤消息
    Notification,   // 通知消息
    Request,        // 請求消息
    Acknowledgment,  // 確認消息
    Custom          // 自定義消息
};

UENUM(BlueprintType)
enum class ECommunicationProtocol : uint8
{
    TCP,            // TCP協議
    UDP,            // UDP協議
    SharedMemory,   // 共享內存
    MessageQueue,   // 消息隊列
    Pipe,           // 管道通信
    WebSocket,      // WebSocket
    HTTP,           // HTTP協議
    Custom          // 自定義協議
};

UENUM(BlueprintType)
enum class EMessagePriority : uint8
{
    Low,            // 低優先級
    Normal,         // 普通優先級
    High,           // 高優先級
    Critical,       // 關鍵優先級
    RealTime        // 實時優先級
};

UENUM(BlueprintType)
enum class EConnectionState : uint8
{
    Disconnected,   // 未連接
    Connecting,     // 連接中
    Connected,      // 已連接
    Reconnecting,   // 重連中
    Error,          // 連接錯誤
    Shutdown        // 連接關閉
};

USTRUCT(BlueprintType)
struct FMessageHeader
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString MessageID;

    UPROPERTY(BlueprintReadOnly)
    FString SourceID;

    UPROPERTY(BlueprintReadOnly)
    FString TargetID;

    UPROPERTY(BlueprintReadOnly)
    EMessageType MessageType;

    UPROPERTY(BlueprintReadOnly)
    EMessagePriority Priority;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    int32 MessageSize;

    UPROPERTY(BlueprintReadOnly)
    bool bRequiresAck;

    UPROPERTY(BlueprintReadOnly)
    FString CorrelationID;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Metadata;

    FMessageHeader()
    {
        MessageID = TEXT("");
        SourceID = TEXT("");
        TargetID = TEXT("");
        MessageType = EMessageType::Data;
        Priority = EMessagePriority::Normal;
        Timestamp = FDateTime::Now();
        MessageSize = 0;
        bRequiresAck = false;
        CorrelationID = TEXT("");
        Metadata.Empty();
    }
};

USTRUCT(BlueprintType)
struct FIPCMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FMessageHeader Header;

    UPROPERTY(BlueprintReadOnly)
    TArray<uint8> Payload;

    UPROPERTY(BlueprintReadOnly)
    FString PayloadString;

    UPROPERTY(BlueprintReadOnly)
    bool bIsCompressed;

    UPROPERTY(BlueprintReadOnly)
    bool bIsEncrypted;

    FIPCMessage()
    {
        Header = FMessageHeader();
        Payload.Empty();
        PayloadString = TEXT("");
        bIsCompressed = false;
        bIsEncrypted = false;
    }
};

USTRUCT(BlueprintType)
struct FConnectionInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ConnectionID;

    UPROPERTY(BlueprintReadOnly)
    FString RemoteAddress;

    UPROPERTY(BlueprintReadOnly)
    int32 RemotePort;

    UPROPERTY(BlueprintReadOnly)
    ECommunicationProtocol Protocol;

    UPROPERTY(BlueprintReadOnly)
    EConnectionState State;

    UPROPERTY(BlueprintReadOnly)
    FDateTime ConnectedTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastActivity;

    UPROPERTY(BlueprintReadOnly)
    int32 MessagesSent;

    UPROPERTY(BlueprintReadOnly)
    int32 MessagesReceived;

    UPROPERTY(BlueprintReadOnly)
    float Throughput;

    UPROPERTY(BlueprintReadOnly)
    bool bIsSecure;

    FConnectionInfo()
    {
        ConnectionID = TEXT("");
        RemoteAddress = TEXT("");
        RemotePort = 0;
        Protocol = ECommunicationProtocol::TCP;
        State = EConnectionState::Disconnected;
        ConnectedTime = FDateTime::Now();
        LastActivity = FDateTime::Now();
        MessagesSent = 0;
        MessagesReceived = 0;
        Throughput = 0.0f;
        bIsSecure = false;
    }
};

USTRUCT(BlueprintType)
struct FCommunicationMetrics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    float AverageLatency;

    UPROPERTY(BlueprintReadOnly)
    float PacketLossRate;

    UPROPERTY(BlueprintReadOnly)
    float ErrorRate;

    UPROPERTY(BlueprintReadOnly)
    int32 TotalMessages;

    UPROPERTY(BlueprintReadOnly)
    int32 SuccessfulMessages;

    UPROPERTY(BlueprintReadOnly)
    int32 FailedMessages;

    UPROPERTY(BlueprintReadOnly)
    float BandwidthUsage;

    UPROPERTY(BlueprintReadOnly)
    int32 ActiveConnections;

    UPROPERTY(BlueprintReadOnly)
    float QueueDepth;

    FCommunicationMetrics()
    {
        AverageLatency = 0.0f;
        PacketLossRate = 0.0f;
        ErrorRate = 0.0f;
        TotalMessages = 0;
        SuccessfulMessages = 0;
        FailedMessages = 0;
        BandwidthUsage = 0.0f;
        ActiveConnections = 0;
        QueueDepth = 0.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageReceived, const FIPCMessage&, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageSent, const FString&, MessageID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectionEstablished, const FString&, ConnectionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectionLost, const FString&, ConnectionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnErrorOccurred, const FString&, Error, const FString&, Details);

/**
 * 進程間通信系統
 * 提供高效的服務間通信機制
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSInterProcessCommunication : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSInterProcessCommunication();

    // 初始化IPC系統
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    bool InitializeIPC(const FString& ServiceID);

    // 關閉IPC系統
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    void ShutdownIPC();

    // 建立連接
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    FString EstablishConnection(const FString& TargetAddress, int32 Port, ECommunicationProtocol Protocol);

    // 關閉連接
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    bool CloseConnection(const FString& ConnectionID);

    // 發送消息
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    FString SendMessage(const FString& ConnectionID, const FIPCMessage& Message);

    // 廣播消息
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    int32 BroadcastMessage(const FIPCMessage& Message);

    // 發送字符串消息
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    FString SendStringMessage(const FString& ConnectionID, const FString& Content, EMessageType MessageType);

    // 發送二進制數據
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    FString SendBinaryMessage(const FString& ConnectionID, const TArray<uint8>& Data, EMessageType MessageType);

    // 獲取連接信息
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    FConnectionInfo GetConnectionInfo(const FString& ConnectionID) const;

    // 獲取所有連接
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    TArray<FConnectionInfo> GetAllConnections() const;

    // 設置消息處理器
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    void SetMessageHandler(TFunction<void(const FIPCMessage&)> Handler);

    // 設置連接處理器
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    void SetConnectionHandler(TFunction<void(const FString&, EConnectionState)> Handler);

    // 啟動服務器
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    bool StartServer(int32 Port, ECommunicationProtocol Protocol);

    // 停止服務器
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    void StopServer();

    // 設置心跳間隔
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    void SetHeartbeatInterval(float IntervalSeconds);

    // 發送心跳
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    void SendHeartbeat(const FString& ConnectionID);

    // 啟用消息壓縮
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    void SetCompressionEnabled(bool bEnabled);

    // 啟用消息加密
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    void SetEncryptionEnabled(bool bEnabled);

    // 獲取通信指標
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    FCommunicationMetrics GetCommunicationMetrics() const;

    // 重置指標
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    void ResetMetrics();

    // 設置最大消息大小
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    void SetMaxMessageSize(int32 MaxSize);

    // 設置連接超時
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    void SetConnectionTimeout(float TimeoutSeconds);

    // 設置重連間隔
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    void SetReconnectInterval(float IntervalSeconds);

    // 啟用自動重連
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    void SetAutoReconnectEnabled(bool bEnabled);

    // 獲取待發送消息數量
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    int32 GetPendingMessageCount() const;

    // 清空消息隊列
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    void ClearMessageQueue();

    // 導出連接配置
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    FString ExportConnectionConfig() const;

    // 導入連接配置
    UFUNCTION(BlueprintCallable, Category = "Inter-Process Communication")
    bool ImportConnectionConfig(const FString& Config);

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "Inter-Process Communication")
    FOnMessageReceived OnMessageReceived;

    UPROPERTY(BlueprintAssignable, Category = "Inter-Process Communication")
    FOnMessageSent OnMessageSent;

    UPROPERTY(BlueprintAssignable, Category = "Inter-Process Communication")
    FOnConnectionEstablished OnConnectionEstablished;

    UPROPERTY(BlueprintAssignable, Category = "Inter-Process Communication")
    FOnConnectionLost OnConnectionLost;

    UPROPERTY(BlueprintAssignable, Category = "Inter-Process Communication")
    FOnErrorOccurred OnErrorOccurred;

protected:
    // 處理接收到的消息
    virtual void HandleReceivedMessage(const FString& ConnectionID, const FIPCMessage& Message);

    // 處理連接狀態變化
    virtual void HandleConnectionStateChange(const FString& ConnectionID, EConnectionState NewState);

    // 處理錯誤
    virtual void HandleError(const FString& Error, const FString& Details);

    // 處理心跳
    virtual void HandleHeartbeat(const FString& ConnectionID);

    // 序列化消息
    virtual TArray<uint8> SerializeMessage(const FIPCMessage& Message);

    // 反序列化消息
    virtual FIPCMessage DeserializeMessage(const TArray<uint8>& Data);

    // 壓縮數據
    virtual TArray<uint8> CompressData(const TArray<uint8>& Data);

    // 解壓縮數據
    virtual TArray<uint8> DecompressData(const TArray<uint8>& CompressedData);

    // 加密數據
    virtual TArray<uint8> EncryptData(const TArray<uint8>& Data);

    // 解密數據
    virtual TArray<uint8> DecryptData(const TArray<uint8>& EncryptedData);

    // 驗證消息完整性
    virtual bool VerifyMessageIntegrity(const FIPCMessage& Message);

    // 計算消息校驗和
    virtual FString CalculateMessageChecksum(const FIPCMessage& Message);

    // 處理TCP連接
    virtual void HandleTCPConnection(const FString& ConnectionID);

    // 處理UDP連接
    virtual void HandleUDPConnection(const FString& ConnectionID);

    // 處理共享內存通信
    virtual void HandleSharedMemoryCommunication(const FString& ConnectionID);

    // 處理消息隊列
    virtual void HandleMessageQueue(const FString& ConnectionID);

    // 處理管道通信
    virtual void HandlePipeCommunication(const FString& ConnectionID);

    // 處理WebSocket通信
    virtual void HandleWebSocketCommunication(const FString& ConnectionID);

    // 處理HTTP通信
    virtual void HandleHTTPCommunication(const FString& ConnectionID);

    // 執行自動重連
    virtual void PerformAutoReconnect(const FString& ConnectionID);

    // 更新通信指標
    virtual void UpdateCommunicationMetrics();

    // 清理無效連接
    virtual void CleanupInvalidConnections();

    // 生成消息ID
    virtual FString GenerateMessageID();

    // 生成連接ID
    virtual FString GenerateConnectionID();

    // 驗證連接參數
    virtual bool ValidateConnectionParameters(const FString& Address, int32 Port, ECommunicationProtocol Protocol);

    // 記錄通信事件
    virtual void LogCommunicationEvent(const FString& Event, const FString& Details);

    // 記錄性能數據
    virtual void RecordPerformanceData(const FString& MetricType, float Value);

private:
    // 服務ID
    UPROPERTY()
    FString ServiceID;

    // 連接信息
    UPROPERTY()
    TMap<FString, FConnectionInfo> Connections;

    // 待發送消息隊列
    UPROPERTY()
    TMap<FString, TArray<FIPCMessage>> PendingMessages;

    // 消息處理器
    UPROPERTY()
    TFunction<void(const FIPCMessage&)> MessageHandler;

    // 連接處理器
    UPROPERTY()
    TFunction<void(const FString&, EConnectionState)> ConnectionHandler;

    // 通信指標
    UPROPERTY()
    FCommunicationMetrics Metrics;

    // 系統狀態
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    bool bIsShutdown;

    UPROPERTY()
    bool bIsServerRunning;

    // 配置參數
    UPROPERTY()
    bool bCompressionEnabled;

    UPROPERTY()
    bool bEncryptionEnabled;

    UPROPERTY()
    bool bAutoReconnectEnabled;

    UPROPERTY()
    float HeartbeatInterval;

    UPROPERTY()
    float ConnectionTimeout;

    UPROPERTY()
    float ReconnectInterval;

    UPROPERTY()
    int32 MaxMessageSize;

    // 服務器信息
    UPROPERTY()
    int32 ServerPort;

    UPROPERTY()
    ECommunicationProtocol ServerProtocol;

    // 計時器
    UPROPERTY()
    FTimerHandle HeartbeatTimer;

    UPROPERTY()
    FTimerHandle MetricsUpdateTimer;

    UPROPERTY()
    FTimerHandle CleanupTimer;

    UPROPERTY()
    FTimerHandle ReconnectTimer;

    // 性能統計
    UPROPERTY()
    TMap<FString, TArray<float>> LatencyHistory;

    UPROPERTY()
    TMap<FString, int32> MessageCounters;

    UPROPERTY()
    TArray<float> BandwidthHistory;
};
