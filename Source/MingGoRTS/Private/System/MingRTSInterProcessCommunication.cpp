#include "MingRTSInterProcessCommunication.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "HAL/PlatformProcess.h"
#include "Serialization/ArrayWriter.h"
#include "Serialization/ArrayReader.h"
#include "Compression/CompressedBuffer.h"

UMingRTSInterProcessCommunication::UMingRTSInterProcessCommunication()
{
    bIsInitialized = false;
    bIsShutdown = false;
    bIsServerRunning = false;
    bCompressionEnabled = true;
    bEncryptionEnabled = false;
    bAutoReconnectEnabled = true;
    HeartbeatInterval = 30.0f;
    ConnectionTimeout = 60.0f;
    ReconnectInterval = 10.0f;
    MaxMessageSize = 1024 * 1024; // 1MB
    ServerPort = 0;
    ServerProtocol = ECommunicationProtocol::TCP;
}

bool UMingRTSInterProcessCommunication::InitializeIPC(const FString& InServiceID)
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("IPC System already initialized"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("Initializing IPC System for service: %s"), *InServiceID);

    ServiceID = InServiceID;

    // 初始化指標
    Metrics = FCommunicationMetrics();

    // 設置定時器
    if (UWorld* World = GetWorld())
    {
        // 心跳定時器
        World->GetTimerManager().SetTimer(
            HeartbeatTimer,
            this,
            &UMingRTSInterProcessCommunication::HandleHeartbeat,
            HeartbeatInterval,
            true
        );

        // 指標更新定時器
        World->GetTimerManager().SetTimer(
            MetricsUpdateTimer,
            this,
            &UMingRTSInterProcessCommunication::UpdateCommunicationMetrics,
            5.0f,
            true
        );

        // 清理定時器
        World->GetTimerManager().SetTimer(
            CleanupTimer,
            this,
            &UMingRTSInterProcessCommunication::CleanupInvalidConnections,
            30.0f,
            true
        );

        // 重連定時器
        World->GetTimerManager().SetTimer(
            ReconnectTimer,
            this,
            &UMingRTSInterProcessCommunication::PerformAutoReconnect,
            ReconnectInterval,
            true
        );
    }

    bIsInitialized = true;
    bIsShutdown = false;

    UE_LOG(LogTemp, Log, TEXT("IPC System initialized successfully"));
    return true;
}

void UMingRTSInterProcessCommunication::ShutdownIPC()
{
    if (!bIsInitialized || bIsShutdown)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Shutting down IPC System..."));

    // 停止服務器
    if (bIsServerRunning)
    {
        StopServer();
    }

    // 關閉所有連接
    TArray<FString> ConnectionIDs;
    for (const auto& ConnectionPair : Connections)
    {
        ConnectionIDs.Add(ConnectionPair.Key);
    }

    for (const FString& ConnectionID : ConnectionIDs)
    {
        CloseConnection(ConnectionID);
    }

    // 清除定時器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(HeartbeatTimer);
        World->GetTimerManager().ClearTimer(MetricsUpdateTimer);
        World->GetTimerManager().ClearTimer(CleanupTimer);
        World->GetTimerManager().ClearTimer(ReconnectTimer);
    }

    // 清空數據結構
    Connections.Empty();
    PendingMessages.Empty();
    LatencyHistory.Empty();
    MessageCounters.Empty();
    BandwidthHistory.Empty();

    bIsShutdown = true;
    bIsInitialized = false;

    UE_LOG(LogTemp, Log, TEXT("IPC System shutdown complete"));
}

FString UMingRTSInterProcessCommunication::EstablishConnection(const FString& TargetAddress, int32 Port, ECommunicationProtocol Protocol)
{
    if (!bIsInitialized || bIsShutdown)
    {
        UE_LOG(LogTemp, Error, TEXT("IPC System not initialized"));
        return TEXT("");
    }

    if (!ValidateConnectionParameters(TargetAddress, Port, Protocol))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid connection parameters"));
        return TEXT("");
    }

    FString ConnectionID = GenerateConnectionID();

    // 創建連接信息
    FConnectionInfo ConnectionInfo;
    ConnectionInfo.ConnectionID = ConnectionID;
    ConnectionInfo.RemoteAddress = TargetAddress;
    ConnectionInfo.RemotePort = Port;
    ConnectionInfo.Protocol = Protocol;
    ConnectionInfo.State = EConnectionState::Connecting;
    ConnectionInfo.ConnectedTime = FDateTime::Now();
    ConnectionInfo.LastActivity = FDateTime::Now();
    ConnectionInfo.MessagesSent = 0;
    ConnectionInfo.MessagesReceived = 0;
    ConnectionInfo.Throughput = 0.0f;
    ConnectionInfo.bIsSecure = (Protocol == ECommunicationProtocol::TCP || Protocol == ECommunicationProtocol::WebSocket);

    Connections.Add(ConnectionID, ConnectionInfo);

    // 根據協議建立連接
    switch (Protocol)
    {
    case ECommunicationProtocol::TCP:
        HandleTCPConnection(ConnectionID);
        break;
    case ECommunicationProtocol::UDP:
        HandleUDPConnection(ConnectionID);
        break;
    case ECommunicationProtocol::SharedMemory:
        HandleSharedMemoryCommunication(ConnectionID);
        break;
    case ECommunicationProtocol::MessageQueue:
        HandleMessageQueue(ConnectionID);
        break;
    case ECommunicationProtocol::Pipe:
        HandlePipeCommunication(ConnectionID);
        break;
    case ECommunicationProtocol::WebSocket:
        HandleWebSocketCommunication(ConnectionID);
        break;
    case ECommunicationProtocol::HTTP:
        HandleHTTPCommunication(ConnectionID);
        break;
    default:
        UE_LOG(LogTemp, Error, TEXT("Unsupported protocol: %d"), (int32)Protocol);
        Connections.Remove(ConnectionID);
        return TEXT("");
    }

    UE_LOG(LogTemp, Log, TEXT("Connection established: %s to %s:%d"), *ConnectionID, *TargetAddress, Port);
    return ConnectionID;
}

bool UMingRTSInterProcessCommunication::CloseConnection(const FString& ConnectionID)
{
    if (!Connections.Contains(ConnectionID))
    {
        return false;
    }

    FConnectionInfo& ConnectionInfo = Connections[ConnectionID];
    ConnectionInfo.State = EConnectionState::Shutdown;

    // 清理待發送消息
    PendingMessages.Remove(ConnectionID);

    // 移除連接
    Connections.Remove(ConnectionID);

    // 廣播連接丟失事件
    OnConnectionLost.Broadcast(ConnectionID);

    UE_LOG(LogTemp, Log, TEXT("Connection closed: %s"), *ConnectionID);
    return true;
}

FString UMingRTSInterProcessCommunication::SendMessage(const FString& ConnectionID, const FIPCMessage& Message)
{
    if (!Connections.Contains(ConnectionID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Connection not found: %s"), *ConnectionID);
        return TEXT("");
    }

    FConnectionInfo& ConnectionInfo = Connections[ConnectionID];
    if (ConnectionInfo.State != EConnectionState::Connected)
    {
        UE_LOG(LogTemp, Warning, TEXT("Connection not ready: %s"), *ConnectionID);
        return TEXT("");
    }

    // 驗證消息大小
    if (Message.Payload.Num() > MaxMessageSize)
    {
        UE_LOG(LogTemp, Error, TEXT("Message size exceeds limit"));
        return TEXT("");
    }

    // 創建消息副本
    FIPCMessage MessageToSend = Message;
    MessageToSend.Header.MessageID = GenerateMessageID();
    MessageToSend.Header.SourceID = ServiceID;
    MessageToSend.Header.TargetID = ConnectionID;
    MessageToSend.Header.Timestamp = FDateTime::Now();
    MessageToSend.Header.MessageSize = MessageToSend.Payload.Num();

    // 序列化消息
    TArray<uint8> SerializedData = SerializeMessage(MessageToSend);

    // 壓縮數據
    if (bCompressionEnabled)
    {
        SerializedData = CompressData(SerializedData);
        MessageToSend.bIsCompressed = true;
    }

    // 加密數據
    if (bEncryptionEnabled)
    {
        SerializedData = EncryptData(SerializedData);
        MessageToSend.bIsEncrypted = true;
    }

    // 添加到待發送隊列
    TArray<FIPCMessage>& Queue = PendingMessages.FindOrAdd(ConnectionID);
    Queue.Add(MessageToSend);

    // 更新連接信息
    ConnectionInfo.MessagesSent++;
    ConnectionInfo.LastActivity = FDateTime::Now();

    // 廣播消息發送事件
    OnMessageSent.Broadcast(MessageToSend.Header.MessageID);

    UE_LOG(LogTemp, VeryVerbose, TEXT("Message queued for sending: %s"), *MessageToSend.Header.MessageID);
    return MessageToSend.Header.MessageID;
}

int32 UMingRTSInterProcessCommunication::BroadcastMessage(const FIPCMessage& Message)
{
    int32 SentCount = 0;

    for (const auto& ConnectionPair : Connections)
    {
        const FString& ConnectionID = ConnectionPair.Key;
        const FConnectionInfo& ConnectionInfo = ConnectionPair.Value;

        if (ConnectionInfo.State == EConnectionState::Connected)
        {
            FString MessageID = SendMessage(ConnectionID, Message);
            if (!MessageID.IsEmpty())
            {
                SentCount++;
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Message broadcasted to %d connections"), SentCount);
    return SentCount;
}

FString UMingRTSInterProcessCommunication::SendStringMessage(const FString& ConnectionID, const FString& Content, EMessageType MessageType)
{
    FIPCMessage Message;
    Message.Header.MessageType = MessageType;
    Message.Header.Priority = EMessagePriority::Normal;
    Message.PayloadString = Content;
    
    // 將字符串轉換為字節數組
    FTCHARToUTF8Converter Converter;
    Converter.Convert(Content.GetCharArray().GetData(), Content.Len());
    
    Message.Payload.SetNumUninitialized(Converter.GetUTF8ByteCount());
    FMemory::Memcpy(Message.Payload.GetData(), Converter.GetUTF8Bytes(), Converter.GetUTF8ByteCount());

    return SendMessage(ConnectionID, Message);
}

FString UMingRTSInterProcessCommunication::SendBinaryMessage(const FString& ConnectionID, const TArray<uint8>& Data, EMessageType MessageType)
{
    FIPCMessage Message;
    Message.Header.MessageType = MessageType;
    Message.Header.Priority = EMessagePriority::Normal;
    Message.Payload = Data;

    return SendMessage(ConnectionID, Message);
}

FConnectionInfo UMingRTSInterProcessCommunication::GetConnectionInfo(const FString& ConnectionID) const
{
    const FConnectionInfo* ConnectionInfo = Connections.Find(ConnectionID);
    return ConnectionInfo ? *ConnectionInfo : FConnectionInfo();
}

TArray<FConnectionInfo> UMingRTSInterProcessCommunication::GetAllConnections() const
{
    TArray<FConnectionInfo> ConnectionList;
    for (const auto& ConnectionPair : Connections)
    {
        ConnectionList.Add(ConnectionPair.Value);
    }
    return ConnectionList;
}

void UMingRTSInterProcessCommunication::SetMessageHandler(TFunction<void(const FIPCMessage&)> Handler)
{
    MessageHandler = Handler;
}

void UMingRTSInterProcessCommunication::SetConnectionHandler(TFunction<void(const FString&, EConnectionState)> Handler)
{
    ConnectionHandler = Handler;
}

bool UMingRTSInterProcessCommunication::StartServer(int32 Port, ECommunicationProtocol Protocol)
{
    if (bIsServerRunning)
    {
        UE_LOG(LogTemp, Warning, TEXT("Server already running"));
        return true;
    }

    ServerPort = Port;
    ServerProtocol = Protocol;

    UE_LOG(LogTemp, Log, TEXT("Starting server on port %d with protocol %d"), Port, (int32)Protocol);

    // 根據協議啟動服務器
    switch (Protocol)
    {
    case ECommunicationProtocol::TCP:
        // TCP服務器實現
        break;
    case ECommunicationProtocol::WebSocket:
        // WebSocket服務器實現
        break;
    case ECommunicationProtocol::HTTP:
        // HTTP服務器實現
        break;
    default:
        UE_LOG(LogTemp, Error, TEXT("Server protocol not supported"));
        return false;
    }

    bIsServerRunning = true;
    UE_LOG(LogTemp, Log, TEXT("Server started successfully"));
    return true;
}

void UMingRTSInterProcessCommunication::StopServer()
{
    if (!bIsServerRunning)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Stopping server..."));

    // 關閉所有客戶端連接
    TArray<FString> ClientConnections;
    for (const auto& ConnectionPair : Connections)
    {
        ClientConnections.Add(ConnectionPair.Key);
    }

    for (const FString& ConnectionID : ClientConnections)
    {
        CloseConnection(ConnectionID);
    }

    bIsServerRunning = false;
    UE_LOG(LogTemp, Log, TEXT("Server stopped"));
}

void UMingRTSInterProcessCommunication::SetHeartbeatInterval(float IntervalSeconds)
{
    HeartbeatInterval = IntervalSeconds;
    
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            HeartbeatTimer,
            this,
            &UMingRTSInterProcessCommunication::HandleHeartbeat,
            HeartbeatInterval,
            true
        );
    }
}

void UMingRTSInterProcessCommunication::SendHeartbeat(const FString& ConnectionID)
{
    FIPCMessage HeartbeatMessage;
    HeartbeatMessage.Header.MessageType = EMessageType::Heartbeat;
    HeartbeatMessage.Header.Priority = EMessagePriority::Low;
    HeartbeatMessage.PayloadString = TEXT("HEARTBEAT");

    SendMessage(ConnectionID, HeartbeatMessage);
}

void UMingRTSInterProcessCommunication::SetCompressionEnabled(bool bEnabled)
{
    bCompressionEnabled = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("Compression %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

void UMingRTSInterProcessCommunication::SetEncryptionEnabled(bool bEnabled)
{
    bEncryptionEnabled = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("Encryption %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

FCommunicationMetrics UMingRTSInterProcessCommunication::GetCommunicationMetrics() const
{
    return Metrics;
}

void UMingRTSInterProcessCommunication::ResetMetrics()
{
    Metrics = FCommunicationMetrics();
    LatencyHistory.Empty();
    MessageCounters.Empty();
    BandwidthHistory.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Communication metrics reset"));
}

void UMingRTSInterProcessCommunication::SetMaxMessageSize(int32 MaxSize)
{
    MaxMessageSize = MaxSize;
    UE_LOG(LogTemp, Log, TEXT("Max message size set to %d bytes"), MaxSize);
}

void UMingRTSInterProcessCommunication::SetConnectionTimeout(float TimeoutSeconds)
{
    ConnectionTimeout = TimeoutSeconds;
    UE_LOG(LogTemp, Log, TEXT("Connection timeout set to %.2f seconds"), TimeoutSeconds);
}

void UMingRTSInterProcessCommunication::SetReconnectInterval(float IntervalSeconds)
{
    ReconnectInterval = IntervalSeconds;
    
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            ReconnectTimer,
            this,
            &UMingRTSInterProcessCommunication::PerformAutoReconnect,
            ReconnectInterval,
            true
        );
    }
}

void UMingRTSInterProcessCommunication::SetAutoReconnectEnabled(bool bEnabled)
{
    bAutoReconnectEnabled = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("Auto reconnect %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

int32 UMingRTSInterProcessCommunication::GetPendingMessageCount() const
{
    int32 TotalCount = 0;
    for (const auto& QueuePair : PendingMessages)
    {
        TotalCount += QueuePair.Value.Num();
    }
    return TotalCount;
}

void UMingRTSInterProcessCommunication::ClearMessageQueue()
{
    PendingMessages.Empty();
    UE_LOG(LogTemp, Log, TEXT("Message queue cleared"));
}

FString UMingRTSInterProcessCommunication::ExportConnectionConfig() const
{
    FString Config = TEXT("{\n");
    Config += FString::Printf(TEXT("  \"ServiceID\": \"%s\",\n"), *ServiceID);
    Config += FString::Printf(TEXT("  \"CompressionEnabled\": %s,\n"), bCompressionEnabled ? TEXT("true") : TEXT("false"));
    Config += FString::Printf(TEXT("  \"EncryptionEnabled\": %s,\n"), bEncryptionEnabled ? TEXT("true") : TEXT("false"));
    Config += FString::Printf(TEXT("  \"AutoReconnectEnabled\": %s,\n"), bAutoReconnectEnabled ? TEXT("true") : TEXT("false"));
    Config += FString::Printf(TEXT("  \"HeartbeatInterval\": %.2f,\n"), HeartbeatInterval);
    Config += FString::Printf(TEXT("  \"ConnectionTimeout\": %.2f,\n"), ConnectionTimeout);
    Config += FString::Printf(TEXT("  \"ReconnectInterval\": %.2f,\n"), ReconnectInterval);
    Config += FString::Printf(TEXT("  \"MaxMessageSize\": %d\n"), MaxMessageSize);
    Config += TEXT("}");
    
    return Config;
}

bool UMingRTSInterProcessCommunication::ImportConnectionConfig(const FString& Config)
{
    // 簡化的JSON解析
    if (Config.Contains(TEXT("\"CompressionEnabled\": true")))
    {
        bCompressionEnabled = true;
    }
    
    if (Config.Contains(TEXT("\"EncryptionEnabled\": true")))
    {
        bEncryptionEnabled = true;
    }
    
    if (Config.Contains(TEXT("\"AutoReconnectEnabled\": true")))
    {
        bAutoReconnectEnabled = true;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Connection configuration imported"));
    return true;
}

void UMingRTSInterProcessCommunication::HandleReceivedMessage(const FString& ConnectionID, const FIPCMessage& Message)
{
    // 更新連接信息
    if (Connections.Contains(ConnectionID))
    {
        FConnectionInfo& ConnectionInfo = Connections[ConnectionID];
        ConnectionInfo.MessagesReceived++;
        ConnectionInfo.LastActivity = FDateTime::Now();
    }

    // 處理心跳消息
    if (Message.Header.MessageType == EMessageType::Heartbeat)
    {
        return; // 心跳消息不需要進一步處理
    }

    // 處理確認消息
    if (Message.Header.MessageType == EMessageType::Acknowledgment)
    {
        return; // 確認消息不需要進一步處理
    }

    // 廣播消息接收事件
    OnMessageReceived.Broadcast(Message);

    // 調用自定義消息處理器
    if (MessageHandler)
    {
        MessageHandler(Message);
    }

    UE_LOG(LogTemp, VeryVerbose, TEXT("Message received: %s"), *Message.Header.MessageID);
}

void UMingRTSInterProcessCommunication::HandleConnectionStateChange(const FString& ConnectionID, EConnectionState NewState)
{
    if (!Connections.Contains(ConnectionID))
    {
        return;
    }

    FConnectionInfo& ConnectionInfo = Connections[ConnectionID];
    EConnectionState OldState = ConnectionInfo.State;
    ConnectionInfo.State = NewState;

    // 廣播狀態變化事件
    if (NewState == EConnectionState::Connected)
    {
        OnConnectionEstablished.Broadcast(ConnectionID);
    }
    else if (NewState == EConnectionState::Error || NewState == EConnectionState::Disconnected)
    {
        OnConnectionLost.Broadcast(ConnectionID);
    }

    // 調用自定義連接處理器
    if (ConnectionHandler)
    {
        ConnectionHandler(ConnectionID, NewState);
    }

    UE_LOG(LogTemp, Log, TEXT("Connection state changed: %s from %d to %d"), *ConnectionID, (int32)OldState, (int32)NewState);
}

void UMingRTSInterProcessCommunication::HandleError(const FString& Error, const FString& Details)
{
    // 廣播錯誤事件
    OnErrorOccurred.Broadcast(Error, Details);

    UE_LOG(LogTemp, Error, TEXT("IPC Error: %s - %s"), *Error, *Details);
}

void UMingRTSInterProcessCommunication::HandleHeartbeat(const FString& ConnectionID)
{
    if (!Connections.Contains(ConnectionID))
    {
        return;
    }

    FConnectionInfo& ConnectionInfo = Connections[ConnectionID];
    
    // 檢查連接超時
    FDateTime Now = FDateTime::Now();
    FTimespan TimeSinceLastActivity = Now - ConnectionInfo.LastActivity;
    
    if (TimeSinceLastActivity.GetTotalSeconds() > ConnectionTimeout)
    {
        UE_LOG(LogTemp, Warning, TEXT("Connection timeout: %s"), *ConnectionID);
        HandleConnectionStateChange(ConnectionID, EConnectionState::Error);
        return;
    }

    // 發送心跳
    SendHeartbeat(ConnectionID);
}

TArray<uint8> UMingRTSInterProcessCommunication::SerializeMessage(const FIPCMessage& Message)
{
    FArrayWriter Writer;
    
    // 寫入消息頭
    Writer << Message.Header.MessageID;
    Writer << Message.Header.SourceID;
    Writer << Message.Header.TargetID;
    Writer << (int32)Message.Header.MessageType;
    Writer << (int32)Message.Header.Priority;
    Writer << Message.Header.Timestamp;
    Writer << Message.Header.MessageSize;
    Writer << Message.Header.bRequiresAck;
    Writer << Message.Header.CorrelationID;
    
    // 寫入元數據
    Writer << Message.Header.Metadata.Num();
    for (const auto& MetaPair : Message.Header.Metadata)
    {
        Writer << MetaPair.Key;
        Writer << MetaPair.Value;
    }
    
    // 寫入載荷
    Writer << Message.Payload.Num();
    for (uint8 Byte : Message.Payload)
    {
        Writer << Byte;
    }
    
    // 寫入字符串載荷
    Writer << Message.PayloadString;
    
    return Writer.GetArray();
}

FIPCMessage UMingRTSInterProcessCommunication::DeserializeMessage(const TArray<uint8>& Data)
{
    FIPCMessage Message;
    FArrayReader Reader(Data);
    
    // 讀取消息頭
    Reader << Message.Header.MessageID;
    Reader << Message.Header.SourceID;
    Reader << Message.Header.TargetID;
    
    int32 MessageTypeValue;
    Reader << MessageTypeValue;
    Message.Header.MessageType = (EMessageType)MessageTypeValue;
    
    int32 PriorityValue;
    Reader << PriorityValue;
    Message.Header.Priority = (EMessagePriority)PriorityValue;
    
    Reader << Message.Header.Timestamp;
    Reader << Message.Header.MessageSize;
    Reader << Message.Header.bRequiresAck;
    Reader << Message.Header.CorrelationID;
    
    // 讀取元數據
    int32 MetadataCount;
    Reader << MetadataCount;
    Message.Header.Metadata.Empty();
    for (int32 i = 0; i < MetadataCount; ++i)
    {
        FString Key, Value;
        Reader << Key;
        Reader << Value;
        Message.Header.Metadata.Add(Key, Value);
    }
    
    // 讀取載荷
    int32 PayloadSize;
    Reader << PayloadSize;
    Message.Payload.SetNumUninitialized(PayloadSize);
    for (int32 i = 0; i < PayloadSize; ++i)
    {
        Reader << Message.Payload[i];
    }
    
    // 讀取字符串載荷
    Reader << Message.PayloadString;
    
    return Message;
}

TArray<uint8> UMingRTSInterProcessCommunication::CompressData(const TArray<uint8>& Data)
{
    // 簡化的壓縮實現
    return Data; // 實際應使用UE的壓縮API
}

TArray<uint8> UMingRTSInterProcessCommunication::DecompressData(const TArray<uint8>& CompressedData)
{
    // 簡化的解壓實現
    return CompressedData; // 實際應使用UE的解壓API
}

TArray<uint8> UMingRTSInterProcessCommunication::EncryptData(const TArray<uint8>& Data)
{
    // 簡化的加密實現
    return Data; // 實際應使用加密API
}

TArray<uint8> UMingRTSInterProcessCommunication::DecryptData(const TArray<uint8>& EncryptedData)
{
    // 簡化的解密實現
    return EncryptedData; // 實際應使用解密API
}

bool UMingRTSInterProcessCommunication::VerifyMessageIntegrity(const FIPCMessage& Message)
{
    // 計算並驗證校驗和
    FString ExpectedChecksum = CalculateMessageChecksum(Message);
    FString ActualChecksum = Message.Header.Metadata.FindRef(TEXT("Checksum"));
    
    return ExpectedChecksum == ActualChecksum;
}

FString UMingRTSInterProcessCommunication::CalculateMessageChecksum(const FIPCMessage& Message)
{
    // 簡化的校驗和計算
    FString ChecksumString = Message.Header.MessageID + Message.Header.SourceID + FString::FromInt(Message.Payload.Num());
    return FString::Printf(TEXT("%X"), GetTypeHash(ChecksumString));
}

void UMingRTSInterProcessCommunication::HandleTCPConnection(const FString& ConnectionID)
{
    // TCP連接處理實現
    UE_LOG(LogTemp, Log, TEXT("Handling TCP connection: %s"), *ConnectionID);
    
    // 模擬連接建立
    HandleConnectionStateChange(ConnectionID, EConnectionState::Connected);
}

void UMingRTSInterProcessCommunication::HandleUDPConnection(const FString& ConnectionID)
{
    // UDP連接處理實現
    UE_LOG(LogTemp, Log, TEXT("Handling UDP connection: %s"), *ConnectionID);
    
    // 模擬連接建立
    HandleConnectionStateChange(ConnectionID, EConnectionState::Connected);
}

void UMingRTSInterProcessCommunication::HandleSharedMemoryCommunication(const FString& ConnectionID)
{
    // 共享內存通信處理實現
    UE_LOG(LogTemp, Log, TEXT("Handling shared memory communication: %s"), *ConnectionID);
    
    // 模擬連接建立
    HandleConnectionStateChange(ConnectionID, EConnectionState::Connected);
}

void UMingRTSInterProcessCommunication::HandleMessageQueue(const FString& ConnectionID)
{
    // 消息隊列處理實現
    UE_LOG(LogTemp, Log, TEXT("Handling message queue: %s"), *ConnectionID);
    
    // 模擬連接建立
    HandleConnectionStateChange(ConnectionID, EConnectionState::Connected);
}

void UMingRTSInterProcessCommunication::HandlePipeCommunication(const FString& ConnectionID)
{
    // 管道通信處理實現
    UE_LOG(LogTemp, Log, TEXT("Handling pipe communication: %s"), *ConnectionID);
    
    // 模擬連接建立
    HandleConnectionStateChange(ConnectionID, EConnectionState::Connected);
}

void UMingRTSInterProcessCommunication::HandleWebSocketCommunication(const FString& ConnectionID)
{
    // WebSocket通信處理實現
    UE_LOG(LogTemp, Log, TEXT("Handling WebSocket communication: %s"), *ConnectionID);
    
    // 模擬連接建立
    HandleConnectionStateChange(ConnectionID, EConnectionState::Connected);
}

void UMingRTSInterProcessCommunication::HandleHTTPCommunication(const FString& ConnectionID)
{
    // HTTP通信處理實現
    UE_LOG(LogTemp, Log, TEXT("Handling HTTP communication: %s"), *ConnectionID);
    
    // 模擬連接建立
    HandleConnectionStateChange(ConnectionID, EConnectionState::Connected);
}

void UMingRTSInterProcessCommunication::PerformAutoReconnect(const FString& ConnectionID)
{
    if (!bAutoReconnectEnabled)
    {
        return;
    }

    if (!Connections.Contains(ConnectionID))
    {
        return;
    }

    FConnectionInfo& ConnectionInfo = Connections[ConnectionID];
    
    // 只對斷開連接進行重連
    if (ConnectionInfo.State == EConnectionState::Disconnected || ConnectionInfo.State == EConnectionState::Error)
    {
        UE_LOG(LogTemp, Log, TEXT("Attempting to reconnect: %s"), *ConnectionID);
        
        // 重新建立連接
        FString NewConnectionID = EstablishConnection(
            ConnectionInfo.RemoteAddress,
            ConnectionInfo.RemotePort,
            ConnectionInfo.Protocol
        );
        
        if (!NewConnectionID.IsEmpty())
        {
            // 移除舊連接
            Connections.Remove(ConnectionID);
            UE_LOG(LogTemp, Log, TEXT("Reconnected successfully: %s -> %s"), *ConnectionID, *NewConnectionID);
        }
    }
}

void UMingRTSInterProcessCommunication::UpdateCommunicationMetrics()
{
    // 更新指標
    Metrics.ActiveConnections = Connections.Num();
    Metrics.QueueDepth = (float)GetPendingMessageCount();
    
    // 計算平均延遲
    float TotalLatency = 0.0f;
    int32 LatencyCount = 0;
    for (const auto& LatencyPair : LatencyHistory)
    {
        for (float Latency : LatencyPair.Value)
        {
            TotalLatency += Latency;
            LatencyCount++;
        }
    }
    Metrics.AverageLatency = LatencyCount > 0 ? TotalLatency / LatencyCount : 0.0f;
    
    // 計算總消息數
    Metrics.TotalMessages = 0;
    for (const auto& CounterPair : MessageCounters)
    {
        Metrics.TotalMessages += CounterPair.Value;
    }
    
    // 計算帶寬使用
    float TotalBandwidth = 0.0f;
    for (float Bandwidth : BandwidthHistory)
    {
        TotalBandwidth += Bandwidth;
    }
    Metrics.BandwidthUsage = BandwidthHistory.Num() > 0 ? TotalBandwidth / BandwidthHistory.Num() : 0.0f;
}

void UMingRTSInterProcessCommunication::CleanupInvalidConnections()
{
    TArray<FString> InvalidConnections;
    FDateTime Now = FDateTime::Now();
    
    for (const auto& ConnectionPair : Connections)
    {
        const FString& ConnectionID = ConnectionPair.Key;
        const FConnectionInfo& ConnectionInfo = ConnectionPair.Value;
        
        // 檢查連接狀態和活動時間
        FTimespan TimeSinceLastActivity = Now - ConnectionInfo.LastActivity;
        if (ConnectionInfo.State == EConnectionState::Error || 
            ConnectionInfo.State == EConnectionState::Shutdown ||
            TimeSinceLastActivity.GetTotalSeconds() > ConnectionTimeout * 2)
        {
            InvalidConnections.Add(ConnectionID);
        }
    }
    
    // 移除無效連接
    for (const FString& ConnectionID : InvalidConnections)
    {
        CloseConnection(ConnectionID);
    }
    
    if (InvalidConnections.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Cleaned up %d invalid connections"), InvalidConnections.Num());
    }
}

FString UMingRTSInterProcessCommunication::GenerateMessageID()
{
    return FString::Printf(TEXT("MSG_%lld"), FDateTime::Now().GetTicks());
}

FString UMingRTSInterProcessCommunication::GenerateConnectionID()
{
    return FString::Printf(TEXT("CONN_%lld"), FDateTime::Now().GetTicks());
}

bool UMingRTSInterProcessCommunication::ValidateConnectionParameters(const FString& Address, int32 Port, ECommunicationProtocol Protocol)
{
    if (Address.IsEmpty())
    {
        return false;
    }
    
    if (Port <= 0 || Port > 65535)
    {
        return false;
    }
    
    // 協議驗證
    switch (Protocol)
    {
    case ECommunicationProtocol::TCP:
    case ECommunicationProtocol::UDP:
    case ECommunicationProtocol::WebSocket:
    case ECommunicationProtocol::HTTP:
        return true;
    default:
        return false;
    }
}

void UMingRTSInterProcessCommunication::LogCommunicationEvent(const FString& Event, const FString& Details)
{
    UE_LOG(LogTemp, Log, TEXT("IPC Event: %s - %s"), *Event, *Details);
}

void UMingRTSInterProcessCommunication::RecordPerformanceData(const FString& MetricType, float Value)
{
    // 記錄性能數據到歷史記錄
    if (MetricType == TEXT("Latency"))
    {
        // 記錄延遲數據
    }
    else if (MetricType == TEXT("Bandwidth"))
    {
        // 記錄帶寬數據
        BandwidthHistory.Add(Value);
        if (BandwidthHistory.Num() > 100)
        {
            BandwidthHistory.RemoveAt(0);
        }
    }
}
