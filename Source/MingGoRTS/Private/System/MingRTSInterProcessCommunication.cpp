#incl使de "Min成RTSInte本P本ocessCo設置設置使nication.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/DateTi設置e.h"
#incl使de "Misc/Paths.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "輸入AL/Platfo本設置P本ocess.h"
#incl使de "Se本ialization/A本本ay基本本ite本.h"
#incl使de "Se本ialization/A本本ayReade本.h"
#incl使de "Co設置p本ession/Co設置p本essedB使ffe本.h"

UMin成RTSInte本P本ocessCo設置設置使nication::UMin成RTSInte本P本ocessCo設置設置使nication()
{
    bIsInitialized = false;
    bIsSh使tdown = false;
    bIsSe本正e本R使nnin成 = false;
    bCo設置p本essionEnabled = t本使e;
    bEnc本yptionEnabled = false;
    bA使toReconnectEnabled = t本使e;
    輸入ea本tbeatInte本正al = 30.0f;
    ConnectionTi設置eo使t = 60.0f;
    ReconnectInte本正al = 10.0f;
    MaxMessa成eSize = 1024 * 1024; // 1MB
    Se本正e本Po本t = 0;
    Se本正e本P本otocol = ECo設置設置使nicationP本otocol::TCP;
}

bool UMin成RTSInte本P本ocessCo設置設置使nication::InitializeIPC(const 軍St本in成& InSe本正iceID)
{
    if (bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("IPC Syste設置 al本eady initialized"));
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 IPC Syste設置 fo本 se本正ice: %s"), *InSe本正iceID);

    Se本正iceID = InSe本正iceID;

    // 初始化指標
    Met本ics = 軍Co設置設置使nicationMet本ics();

    // 設置定時器
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        // 心跳定時器
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            輸入ea本tbeatTi設置e本,
            this,
            &UMin成RTSInte本P本ocessCo設置設置使nication::輸入andle輸入ea本tbeat,
            輸入ea本tbeatInte本正al,
            t本使e
        );

        // 指標更新定時器
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            Met本icsUpdateTi設置e本,
            this,
            &UMin成RTSInte本P本ocessCo設置設置使nication::UpdateCo設置設置使nicationMet本ics,
            5.0f,
            t本使e
        );

        // 清理定時器
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            Clean使pTi設置e本,
            this,
            &UMin成RTSInte本P本ocessCo設置設置使nication::Clean使pIn正alidConnections,
            30.0f,
            t本使e
        );

        // 重連定時器
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            ReconnectTi設置e本,
            this,
            &UMin成RTSInte本P本ocessCo設置設置使nication::Pe本fo本設置A使toReconnect,
            ReconnectInte本正al,
            t本使e
        );
    }

    bIsInitialized = t本使e;
    bIsSh使tdown = false;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("IPC Syste設置 initialized s使ccessf使lly"));
    本et使本n t本使e;
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::Sh使tdownIPC()
{
    if (!bIsInitialized  bIsSh使tdown)
    {
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sh使ttin成 down IPC Syste設置..."));

    // 停止服務器
    if (bIsSe本正e本R使nnin成)
    {
        StopSe本正e本();
    }

    // 關閉所有連接
    TA本本ay<軍St本in成> ConnectionIDs;
    fo本 (const a使to& ConnectionPai本 : Connections)
    {
        ConnectionIDs.Add(ConnectionPai本.Key);
    }

    fo本 (const 軍St本in成& ConnectionID : ConnectionIDs)
    {
        CloseConnection(ConnectionID);
    }

    // 清除定時器
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(輸入ea本tbeatTi設置e本);
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(Met本icsUpdateTi設置e本);
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(Clean使pTi設置e本);
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(ReconnectTi設置e本);
    }

    // 清空數據結構
    Connections.E設置pty();
    Pendin成Messa成es.E設置pty();
    Latency輸入isto本y.E設置pty();
    Messa成eCo使nte本s.E設置pty();
    Bandwidth輸入isto本y.E設置pty();

    bIsSh使tdown = t本使e;
    bIsInitialized = false;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("IPC Syste設置 sh使tdown co設置plete"));
}

軍St本in成 UMin成RTSInte本P本ocessCo設置設置使nication::EstablishConnection(const 軍St本in成& Ta本成etAdd本ess, int32 Po本t, ECo設置設置使nicationP本otocol P本otocol)
{
    if (!bIsInitialized  bIsSh使tdown)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("IPC Syste設置 not initialized"));
        本et使本n TEXT("");
    }

    if (!ValidateConnectionPa本a設置ete本s(Ta本成etAdd本ess, Po本t, P本otocol))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("In正alid connection pa本a設置ete本s"));
        本et使本n TEXT("");
    }

    軍St本in成 ConnectionID = Gene本ateConnectionID();

    // 創建連接信息
    軍ConnectionInfo ConnectionInfo;
    ConnectionInfo.ConnectionID = ConnectionID;
    ConnectionInfo.Re設置oteAdd本ess = Ta本成etAdd本ess;
    ConnectionInfo.Re設置otePo本t = Po本t;
    ConnectionInfo.P本otocol = P本otocol;
    ConnectionInfo.State = EConnectionState::Connectin成;
    ConnectionInfo.ConnectedTi設置e = 軍DateTi設置e::的ow();
    ConnectionInfo.LastActi正ity = 軍DateTi設置e::的ow();
    ConnectionInfo.Messa成esSent = 0;
    ConnectionInfo.Messa成esRecei正ed = 0;
    ConnectionInfo.Th本o使成hp使t = 0.0f;
    ConnectionInfo.bIsSec使本e = (P本otocol == ECo設置設置使nicationP本otocol::TCP  P本otocol == ECo設置設置使nicationP本otocol::基本ebSocket);

    Connections.Add(ConnectionID, ConnectionInfo);

    // 根據協議建立連接
    switch (P本otocol)
    {
    case ECo設置設置使nicationP本otocol::TCP:
        輸入andleTCPConnection(ConnectionID);
        b本eak;
    case ECo設置設置使nicationP本otocol::UDP:
        輸入andleUDPConnection(ConnectionID);
        b本eak;
    case ECo設置設置使nicationP本otocol::Sha本edMe設置o本y:
        輸入andleSha本edMe設置o本yCo設置設置使nication(ConnectionID);
        b本eak;
    case ECo設置設置使nicationP本otocol::Messa成eQ使e使e:
        輸入andleMessa成eQ使e使e(ConnectionID);
        b本eak;
    case ECo設置設置使nicationP本otocol::Pipe:
        輸入andlePipeCo設置設置使nication(ConnectionID);
        b本eak;
    case ECo設置設置使nicationP本otocol::基本ebSocket:
        輸入andle基本ebSocketCo設置設置使nication(ConnectionID);
        b本eak;
    case ECo設置設置使nicationP本otocol::輸入TTP:
        輸入andle輸入TTPCo設置設置使nication(ConnectionID);
        b本eak;
    defa使lt:
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Uns使ppo本ted p本otocol: %d"), (int32)P本otocol);
        Connections.Re設置o正e(ConnectionID);
        本et使本n TEXT("");
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Connection established: %s to %s:%d"), *ConnectionID, *Ta本成etAdd本ess, Po本t);
    本et使本n ConnectionID;
}

bool UMin成RTSInte本P本ocessCo設置設置使nication::CloseConnection(const 軍St本in成& ConnectionID)
{
    if (!Connections.Contains(ConnectionID))
    {
        本et使本n false;
    }

    軍ConnectionInfo& ConnectionInfo = Connections[ConnectionID];
    ConnectionInfo.State = EConnectionState::Sh使tdown;

    // 清理待發送消息
    Pendin成Messa成es.Re設置o正e(ConnectionID);

    // 移除連接
    Connections.Re設置o正e(ConnectionID);

    // 廣播連接丟失事件
    OnConnectionLost.B本oadcast(ConnectionID);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Connection closed: %s"), *ConnectionID);
    本et使本n t本使e;
}

軍St本in成 UMin成RTSInte本P本ocessCo設置設置使nication::SendMessa成e(const 軍St本in成& ConnectionID, const 軍IPCMessa成e& Messa成e)
{
    if (!Connections.Contains(ConnectionID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Connection not fo使nd: %s"), *ConnectionID);
        本et使本n TEXT("");
    }

    軍ConnectionInfo& ConnectionInfo = Connections[ConnectionID];
    if (ConnectionInfo.State != EConnectionState::Connected)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Connection not 本eady: %s"), *ConnectionID);
        本et使本n TEXT("");
    }

    // 驗證消息大小
    if (Messa成e.Payload.的使設置() > MaxMessa成eSize)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Messa成e size exceeds li設置it"));
        本et使本n TEXT("");
    }

    // 創建消息副本
    軍IPCMessa成e Messa成eToSend = Messa成e;
    Messa成eToSend.輸入eade本.Messa成eID = Gene本ateMessa成eID();
    Messa成eToSend.輸入eade本.So使本ceID = Se本正iceID;
    Messa成eToSend.輸入eade本.Ta本成etID = ConnectionID;
    Messa成eToSend.輸入eade本.Ti設置esta設置p = 軍DateTi設置e::的ow();
    Messa成eToSend.輸入eade本.Messa成eSize = Messa成eToSend.Payload.的使設置();

    // 序列化消息
    TA本本ay<使int8> Se本ializedData = Se本ializeMessa成e(Messa成eToSend);

    // 壓縮數據
    if (bCo設置p本essionEnabled)
    {
        Se本ializedData = Co設置p本essData(Se本ializedData);
        Messa成eToSend.bIsCo設置p本essed = t本使e;
    }

    // 加密數據
    if (bEnc本yptionEnabled)
    {
        Se本ializedData = Enc本yptData(Se本ializedData);
        Messa成eToSend.bIsEnc本ypted = t本使e;
    }

    // 添加到待發送隊列
    TA本本ay<軍IPCMessa成e>& Q使e使e = Pendin成Messa成es.軍indO本Add(ConnectionID);
    Q使e使e.Add(Messa成eToSend);

    // 更新連接信息
    ConnectionInfo.Messa成esSent++;
    ConnectionInfo.LastActi正ity = 軍DateTi設置e::的ow();

    // 廣播消息發送事件
    OnMessa成eSent.B本oadcast(Messa成eToSend.輸入eade本.Messa成eID);

    UE下LOG(Lo成Te設置p, Ve本yVe本bose, TEXT("Messa成e q使e使ed fo本 sendin成: %s"), *Messa成eToSend.輸入eade本.Messa成eID);
    本et使本n Messa成eToSend.輸入eade本.Messa成eID;
}

int32 UMin成RTSInte本P本ocessCo設置設置使nication::B本oadcastMessa成e(const 軍IPCMessa成e& Messa成e)
{
    int32 SentCo使nt = 0;

    fo本 (const a使to& ConnectionPai本 : Connections)
    {
        const 軍St本in成& ConnectionID = ConnectionPai本.Key;
        const 軍ConnectionInfo& ConnectionInfo = ConnectionPai本.Val使e;

        if (ConnectionInfo.State == EConnectionState::Connected)
        {
            軍St本in成 Messa成eID = SendMessa成e(ConnectionID, Messa成e);
            if (!Messa成eID.IsE設置pty())
            {
                SentCo使nt++;
            }
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Messa成e b本oadcasted to %d connections"), SentCo使nt);
    本et使本n SentCo使nt;
}

軍St本in成 UMin成RTSInte本P本ocessCo設置設置使nication::SendSt本in成Messa成e(const 軍St本in成& ConnectionID, const 軍St本in成& Content, EMessa成eType Messa成eType)
{
    軍IPCMessa成e Messa成e;
    Messa成e.輸入eade本.Messa成eType = Messa成eType;
    Messa成e.輸入eade本.P本io本ity = EMessa成eP本io本ity::的o本設置al;
    Messa成e.PayloadSt本in成 = Content;
    
    // 將字符串轉換為字節數組
    軍TC輸入ARToUT軍8Con正e本te本 Con正e本te本;
    Con正e本te本.Con正e本t(Content.GetCha本A本本ay().GetData(), Content.Len());
    
    Messa成e.Payload.Set的使設置Uninitialized(Con正e本te本.GetUT軍8ByteCo使nt());
    軍Me設置o本y::Me設置cpy(Messa成e.Payload.GetData(), Con正e本te本.GetUT軍8Bytes(), Con正e本te本.GetUT軍8ByteCo使nt());

    本et使本n SendMessa成e(ConnectionID, Messa成e);
}

軍St本in成 UMin成RTSInte本P本ocessCo設置設置使nication::SendBina本yMessa成e(const 軍St本in成& ConnectionID, const TA本本ay<使int8>& Data, EMessa成eType Messa成eType)
{
    軍IPCMessa成e Messa成e;
    Messa成e.輸入eade本.Messa成eType = Messa成eType;
    Messa成e.輸入eade本.P本io本ity = EMessa成eP本io本ity::的o本設置al;
    Messa成e.Payload = Data;

    本et使本n SendMessa成e(ConnectionID, Messa成e);
}

軍ConnectionInfo UMin成RTSInte本P本ocessCo設置設置使nication::GetConnectionInfo(const 軍St本in成& ConnectionID) const
{
    const 軍ConnectionInfo* ConnectionInfo = Connections.軍ind(ConnectionID);
    本et使本n ConnectionInfo 基本 *ConnectionInfo : 軍ConnectionInfo();
}

TA本本ay<軍ConnectionInfo> UMin成RTSInte本P本ocessCo設置設置使nication::GetAllConnections() const
{
    TA本本ay<軍ConnectionInfo> ConnectionList;
    fo本 (const a使to& ConnectionPai本 : Connections)
    {
        ConnectionList.Add(ConnectionPai本.Val使e);
    }
    本et使本n ConnectionList;
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::SetMessa成e輸入andle本(T軍使nction<正oid(const 軍IPCMessa成e&)> 輸入andle本)
{
    Messa成e輸入andle本 = 輸入andle本;
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::SetConnection輸入andle本(T軍使nction<正oid(const 軍St本in成&, EConnectionState)> 輸入andle本)
{
    Connection輸入andle本 = 輸入andle本;
}

bool UMin成RTSInte本P本ocessCo設置設置使nication::Sta本tSe本正e本(int32 Po本t, ECo設置設置使nicationP本otocol P本otocol)
{
    if (bIsSe本正e本R使nnin成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正e本 al本eady 本使nnin成"));
        本et使本n t本使e;
    }

    Se本正e本Po本t = Po本t;
    Se本正e本P本otocol = P本otocol;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本tin成 se本正e本 on po本t %d with p本otocol %d"), Po本t, (int32)P本otocol);

    // 根據協議啟動服務器
    switch (P本otocol)
    {
    case ECo設置設置使nicationP本otocol::TCP:
        // TCP服務器實現
        b本eak;
    case ECo設置設置使nicationP本otocol::基本ebSocket:
        // 基本ebSocket服務器實現
        b本eak;
    case ECo設置設置使nicationP本otocol::輸入TTP:
        // 輸入TTP服務器實現
        b本eak;
    defa使lt:
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Se本正e本 p本otocol not s使ppo本ted"));
        本et使本n false;
    }

    bIsSe本正e本R使nnin成 = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正e本 sta本ted s使ccessf使lly"));
    本et使本n t本使e;
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::StopSe本正e本()
{
    if (!bIsSe本正e本R使nnin成)
    {
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stoppin成 se本正e本..."));

    // 關閉所有客戶端連接
    TA本本ay<軍St本in成> ClientConnections;
    fo本 (const a使to& ConnectionPai本 : Connections)
    {
        ClientConnections.Add(ConnectionPai本.Key);
    }

    fo本 (const 軍St本in成& ConnectionID : ClientConnections)
    {
        CloseConnection(ConnectionID);
    }

    bIsSe本正e本R使nnin成 = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正e本 stopped"));
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::Set輸入ea本tbeatInte本正al(float Inte本正alSeconds)
{
    輸入ea本tbeatInte本正al = Inte本正alSeconds;
    
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            輸入ea本tbeatTi設置e本,
            this,
            &UMin成RTSInte本P本ocessCo設置設置使nication::輸入andle輸入ea本tbeat,
            輸入ea本tbeatInte本正al,
            t本使e
        );
    }
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::Send輸入ea本tbeat(const 軍St本in成& ConnectionID)
{
    軍IPCMessa成e 輸入ea本tbeatMessa成e;
    輸入ea本tbeatMessa成e.輸入eade本.Messa成eType = EMessa成eType::輸入ea本tbeat;
    輸入ea本tbeatMessa成e.輸入eade本.P本io本ity = EMessa成eP本io本ity::Low;
    輸入ea本tbeatMessa成e.PayloadSt本in成 = TEXT("輸入EARTBEAT");

    SendMessa成e(ConnectionID, 輸入ea本tbeatMessa成e);
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::SetCo設置p本essionEnabled(bool bEnabled)
{
    bCo設置p本essionEnabled = bEnabled;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置p本ession %s"), bEnabled 基本 TEXT("enabled") : TEXT("disabled"));
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::SetEnc本yptionEnabled(bool bEnabled)
{
    bEnc本yptionEnabled = bEnabled;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Enc本yption %s"), bEnabled 基本 TEXT("enabled") : TEXT("disabled"));
}

軍Co設置設置使nicationMet本ics UMin成RTSInte本P本ocessCo設置設置使nication::GetCo設置設置使nicationMet本ics() const
{
    本et使本n Met本ics;
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::ResetMet本ics()
{
    Met本ics = 軍Co設置設置使nicationMet本ics();
    Latency輸入isto本y.E設置pty();
    Messa成eCo使nte本s.E設置pty();
    Bandwidth輸入isto本y.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置設置使nication 設置et本ics 本eset"));
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::SetMaxMessa成eSize(int32 MaxSize)
{
    MaxMessa成eSize = MaxSize;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Max 設置essa成e size set to %d bytes"), MaxSize);
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::SetConnectionTi設置eo使t(float Ti設置eo使tSeconds)
{
    ConnectionTi設置eo使t = Ti設置eo使tSeconds;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Connection ti設置eo使t set to %.2f seconds"), Ti設置eo使tSeconds);
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::SetReconnectInte本正al(float Inte本正alSeconds)
{
    ReconnectInte本正al = Inte本正alSeconds;
    
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            ReconnectTi設置e本,
            this,
            &UMin成RTSInte本P本ocessCo設置設置使nication::Pe本fo本設置A使toReconnect,
            ReconnectInte本正al,
            t本使e
        );
    }
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::SetA使toReconnectEnabled(bool bEnabled)
{
    bA使toReconnectEnabled = bEnabled;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("A使to 本econnect %s"), bEnabled 基本 TEXT("enabled") : TEXT("disabled"));
}

int32 UMin成RTSInte本P本ocessCo設置設置使nication::GetPendin成Messa成eCo使nt() const
{
    int32 TotalCo使nt = 0;
    fo本 (const a使to& Q使e使ePai本 : Pendin成Messa成es)
    {
        TotalCo使nt += Q使e使ePai本.Val使e.的使設置();
    }
    本et使本n TotalCo使nt;
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::Clea本Messa成eQ使e使e()
{
    Pendin成Messa成es.E設置pty();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Messa成e q使e使e clea本ed"));
}

軍St本in成 UMin成RTSInte本P本ocessCo設置設置使nication::Expo本tConnectionConfi成() const
{
    軍St本in成 Confi成 = TEXT("{\n");
    Confi成 += 軍St本in成::P本intf(TEXT("  \"Se本正iceID\": \"%s\",\n"), *Se本正iceID);
    Confi成 += 軍St本in成::P本intf(TEXT("  \"Co設置p本essionEnabled\": %s,\n"), bCo設置p本essionEnabled 基本 TEXT("t本使e") : TEXT("false"));
    Confi成 += 軍St本in成::P本intf(TEXT("  \"Enc本yptionEnabled\": %s,\n"), bEnc本yptionEnabled 基本 TEXT("t本使e") : TEXT("false"));
    Confi成 += 軍St本in成::P本intf(TEXT("  \"A使toReconnectEnabled\": %s,\n"), bA使toReconnectEnabled 基本 TEXT("t本使e") : TEXT("false"));
    Confi成 += 軍St本in成::P本intf(TEXT("  \"輸入ea本tbeatInte本正al\": %.2f,\n"), 輸入ea本tbeatInte本正al);
    Confi成 += 軍St本in成::P本intf(TEXT("  \"ConnectionTi設置eo使t\": %.2f,\n"), ConnectionTi設置eo使t);
    Confi成 += 軍St本in成::P本intf(TEXT("  \"ReconnectInte本正al\": %.2f,\n"), ReconnectInte本正al);
    Confi成 += 軍St本in成::P本intf(TEXT("  \"MaxMessa成eSize\": %d\n"), MaxMessa成eSize);
    Confi成 += TEXT("}");
    
    本et使本n Confi成;
}

bool UMin成RTSInte本P本ocessCo設置設置使nication::I設置po本tConnectionConfi成(const 軍St本in成& Confi成)
{
    // 簡化的JSO的解析
    if (Confi成.Contains(TEXT("\"Co設置p本essionEnabled\": t本使e")))
    {
        bCo設置p本essionEnabled = t本使e;
    }
    
    if (Confi成.Contains(TEXT("\"Enc本yptionEnabled\": t本使e")))
    {
        bEnc本yptionEnabled = t本使e;
    }
    
    if (Confi成.Contains(TEXT("\"A使toReconnectEnabled\": t本使e")))
    {
        bA使toReconnectEnabled = t本使e;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Connection confi成使本ation i設置po本ted"));
    本et使本n t本使e;
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::輸入andleRecei正edMessa成e(const 軍St本in成& ConnectionID, const 軍IPCMessa成e& Messa成e)
{
    // 更新連接信息
    if (Connections.Contains(ConnectionID))
    {
        軍ConnectionInfo& ConnectionInfo = Connections[ConnectionID];
        ConnectionInfo.Messa成esRecei正ed++;
        ConnectionInfo.LastActi正ity = 軍DateTi設置e::的ow();
    }

    // 處理心跳消息
    if (Messa成e.輸入eade本.Messa成eType == EMessa成eType::輸入ea本tbeat)
    {
        本et使本n; // 心跳消息不需要進一步處理
    }

    // 處理確認消息
    if (Messa成e.輸入eade本.Messa成eType == EMessa成eType::Acknowled成設置ent)
    {
        本et使本n; // 確認消息不需要進一步處理
    }

    // 廣播消息接收事件
    OnMessa成eRecei正ed.B本oadcast(Messa成e);

    // 調用自定義消息處理器
    if (Messa成e輸入andle本)
    {
        Messa成e輸入andle本(Messa成e);
    }

    UE下LOG(Lo成Te設置p, Ve本yVe本bose, TEXT("Messa成e 本ecei正ed: %s"), *Messa成e.輸入eade本.Messa成eID);
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::輸入andleConnectionStateChan成e(const 軍St本in成& ConnectionID, EConnectionState 的ewState)
{
    if (!Connections.Contains(ConnectionID))
    {
        本et使本n;
    }

    軍ConnectionInfo& ConnectionInfo = Connections[ConnectionID];
    EConnectionState OldState = ConnectionInfo.State;
    ConnectionInfo.State = 的ewState;

    // 廣播狀態變化事件
    if (的ewState == EConnectionState::Connected)
    {
        OnConnectionEstablished.B本oadcast(ConnectionID);
    }
    else if (的ewState == EConnectionState::E本本o本  的ewState == EConnectionState::Disconnected)
    {
        OnConnectionLost.B本oadcast(ConnectionID);
    }

    // 調用自定義連接處理器
    if (Connection輸入andle本)
    {
        Connection輸入andle本(ConnectionID, 的ewState);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Connection state chan成ed: %s f本o設置 %d to %d"), *ConnectionID, (int32)OldState, (int32)的ewState);
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::輸入andleE本本o本(const 軍St本in成& E本本o本, const 軍St本in成& Details)
{
    // 廣播錯誤事件
    OnE本本o本Occ使本本ed.B本oadcast(E本本o本, Details);

    UE下LOG(Lo成Te設置p, E本本o本, TEXT("IPC E本本o本: %s - %s"), *E本本o本, *Details);
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::輸入andle輸入ea本tbeat(const 軍St本in成& ConnectionID)
{
    if (!Connections.Contains(ConnectionID))
    {
        本et使本n;
    }

    軍ConnectionInfo& ConnectionInfo = Connections[ConnectionID];
    
    // 檢查連接超時
    軍DateTi設置e 的ow = 軍DateTi設置e::的ow();
    軍Ti設置espan Ti設置eSinceLastActi正ity = 的ow - ConnectionInfo.LastActi正ity;
    
    if (Ti設置eSinceLastActi正ity.GetTotalSeconds() > ConnectionTi設置eo使t)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Connection ti設置eo使t: %s"), *ConnectionID);
        輸入andleConnectionStateChan成e(ConnectionID, EConnectionState::E本本o本);
        本et使本n;
    }

    // 發送心跳
    Send輸入ea本tbeat(ConnectionID);
}

TA本本ay<使int8> UMin成RTSInte本P本ocessCo設置設置使nication::Se本ializeMessa成e(const 軍IPCMessa成e& Messa成e)
{
    軍A本本ay基本本ite本 基本本ite本;
    
    // 寫入消息頭
    基本本ite本 << Messa成e.輸入eade本.Messa成eID;
    基本本ite本 << Messa成e.輸入eade本.So使本ceID;
    基本本ite本 << Messa成e.輸入eade本.Ta本成etID;
    基本本ite本 << (int32)Messa成e.輸入eade本.Messa成eType;
    基本本ite本 << (int32)Messa成e.輸入eade本.P本io本ity;
    基本本ite本 << Messa成e.輸入eade本.Ti設置esta設置p;
    基本本ite本 << Messa成e.輸入eade本.Messa成eSize;
    基本本ite本 << Messa成e.輸入eade本.bReq使i本esAck;
    基本本ite本 << Messa成e.輸入eade本.Co本本elationID;
    
    // 寫入元數據
    基本本ite本 << Messa成e.輸入eade本.Metadata.的使設置();
    fo本 (const a使to& MetaPai本 : Messa成e.輸入eade本.Metadata)
    {
        基本本ite本 << MetaPai本.Key;
        基本本ite本 << MetaPai本.Val使e;
    }
    
    // 寫入載荷
    基本本ite本 << Messa成e.Payload.的使設置();
    fo本 (使int8 Byte : Messa成e.Payload)
    {
        基本本ite本 << Byte;
    }
    
    // 寫入字符串載荷
    基本本ite本 << Messa成e.PayloadSt本in成;
    
    本et使本n 基本本ite本.GetA本本ay();
}

軍IPCMessa成e UMin成RTSInte本P本ocessCo設置設置使nication::Dese本ializeMessa成e(const TA本本ay<使int8>& Data)
{
    軍IPCMessa成e Messa成e;
    軍A本本ayReade本 Reade本(Data);
    
    // 讀取消息頭
    Reade本 << Messa成e.輸入eade本.Messa成eID;
    Reade本 << Messa成e.輸入eade本.So使本ceID;
    Reade本 << Messa成e.輸入eade本.Ta本成etID;
    
    int32 Messa成eTypeVal使e;
    Reade本 << Messa成eTypeVal使e;
    Messa成e.輸入eade本.Messa成eType = (EMessa成eType)Messa成eTypeVal使e;
    
    int32 P本io本ityVal使e;
    Reade本 << P本io本ityVal使e;
    Messa成e.輸入eade本.P本io本ity = (EMessa成eP本io本ity)P本io本ityVal使e;
    
    Reade本 << Messa成e.輸入eade本.Ti設置esta設置p;
    Reade本 << Messa成e.輸入eade本.Messa成eSize;
    Reade本 << Messa成e.輸入eade本.bReq使i本esAck;
    Reade本 << Messa成e.輸入eade本.Co本本elationID;
    
    // 讀取元數據
    int32 MetadataCo使nt;
    Reade本 << MetadataCo使nt;
    Messa成e.輸入eade本.Metadata.E設置pty();
    fo本 (int32 i = 0; i < MetadataCo使nt; ++i)
    {
        軍St本in成 Key, Val使e;
        Reade本 << Key;
        Reade本 << Val使e;
        Messa成e.輸入eade本.Metadata.Add(Key, Val使e);
    }
    
    // 讀取載荷
    int32 PayloadSize;
    Reade本 << PayloadSize;
    Messa成e.Payload.Set的使設置Uninitialized(PayloadSize);
    fo本 (int32 i = 0; i < PayloadSize; ++i)
    {
        Reade本 << Messa成e.Payload[i];
    }
    
    // 讀取字符串載荷
    Reade本 << Messa成e.PayloadSt本in成;
    
    本et使本n Messa成e;
}

TA本本ay<使int8> UMin成RTSInte本P本ocessCo設置設置使nication::Co設置p本essData(const TA本本ay<使int8>& Data)
{
    // 簡化的壓縮實現
    本et使本n Data; // 實際應使用UE的壓縮API
}

TA本本ay<使int8> UMin成RTSInte本P本ocessCo設置設置使nication::Deco設置p本essData(const TA本本ay<使int8>& Co設置p本essedData)
{
    // 簡化的解壓實現
    本et使本n Co設置p本essedData; // 實際應使用UE的解壓API
}

TA本本ay<使int8> UMin成RTSInte本P本ocessCo設置設置使nication::Enc本yptData(const TA本本ay<使int8>& Data)
{
    // 簡化的加密實現
    本et使本n Data; // 實際應使用加密API
}

TA本本ay<使int8> UMin成RTSInte本P本ocessCo設置設置使nication::Dec本yptData(const TA本本ay<使int8>& Enc本yptedData)
{
    // 簡化的解密實現
    本et使本n Enc本yptedData; // 實際應使用解密API
}

bool UMin成RTSInte本P本ocessCo設置設置使nication::Ve本ifyMessa成eInte成本ity(const 軍IPCMessa成e& Messa成e)
{
    // 計算並驗證校驗和
    軍St本in成 ExpectedChecks使設置 = Calc使lateMessa成eChecks使設置(Messa成e);
    軍St本in成 Act使alChecks使設置 = Messa成e.輸入eade本.Metadata.軍indRef(TEXT("Checks使設置"));
    
    本et使本n ExpectedChecks使設置 == Act使alChecks使設置;
}

軍St本in成 UMin成RTSInte本P本ocessCo設置設置使nication::Calc使lateMessa成eChecks使設置(const 軍IPCMessa成e& Messa成e)
{
    // 簡化的校驗和計算
    軍St本in成 Checks使設置St本in成 = Messa成e.輸入eade本.Messa成eID + Messa成e.輸入eade本.So使本ceID + 軍St本in成::軍本o設置Int(Messa成e.Payload.的使設置());
    本et使本n 軍St本in成::P本intf(TEXT("%X"), GetType輸入ash(Checks使設置St本in成));
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::輸入andleTCPConnection(const 軍St本in成& ConnectionID)
{
    // TCP連接處理實現
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入andlin成 TCP connection: %s"), *ConnectionID);
    
    // 模擬連接建立
    輸入andleConnectionStateChan成e(ConnectionID, EConnectionState::Connected);
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::輸入andleUDPConnection(const 軍St本in成& ConnectionID)
{
    // UDP連接處理實現
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入andlin成 UDP connection: %s"), *ConnectionID);
    
    // 模擬連接建立
    輸入andleConnectionStateChan成e(ConnectionID, EConnectionState::Connected);
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::輸入andleSha本edMe設置o本yCo設置設置使nication(const 軍St本in成& ConnectionID)
{
    // 共享內存通信處理實現
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入andlin成 sha本ed 設置e設置o本y co設置設置使nication: %s"), *ConnectionID);
    
    // 模擬連接建立
    輸入andleConnectionStateChan成e(ConnectionID, EConnectionState::Connected);
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::輸入andleMessa成eQ使e使e(const 軍St本in成& ConnectionID)
{
    // 消息隊列處理實現
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入andlin成 設置essa成e q使e使e: %s"), *ConnectionID);
    
    // 模擬連接建立
    輸入andleConnectionStateChan成e(ConnectionID, EConnectionState::Connected);
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::輸入andlePipeCo設置設置使nication(const 軍St本in成& ConnectionID)
{
    // 管道通信處理實現
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入andlin成 pipe co設置設置使nication: %s"), *ConnectionID);
    
    // 模擬連接建立
    輸入andleConnectionStateChan成e(ConnectionID, EConnectionState::Connected);
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::輸入andle基本ebSocketCo設置設置使nication(const 軍St本in成& ConnectionID)
{
    // 基本ebSocket通信處理實現
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入andlin成 基本ebSocket co設置設置使nication: %s"), *ConnectionID);
    
    // 模擬連接建立
    輸入andleConnectionStateChan成e(ConnectionID, EConnectionState::Connected);
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::輸入andle輸入TTPCo設置設置使nication(const 軍St本in成& ConnectionID)
{
    // 輸入TTP通信處理實現
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入andlin成 輸入TTP co設置設置使nication: %s"), *ConnectionID);
    
    // 模擬連接建立
    輸入andleConnectionStateChan成e(ConnectionID, EConnectionState::Connected);
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::Pe本fo本設置A使toReconnect(const 軍St本in成& ConnectionID)
{
    if (!bA使toReconnectEnabled)
    {
        本et使本n;
    }

    if (!Connections.Contains(ConnectionID))
    {
        本et使本n;
    }

    軍ConnectionInfo& ConnectionInfo = Connections[ConnectionID];
    
    // 只對斷開連接進行重連
    if (ConnectionInfo.State == EConnectionState::Disconnected  ConnectionInfo.State == EConnectionState::E本本o本)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Atte設置ptin成 to 本econnect: %s"), *ConnectionID);
        
        // 重新建立連接
        軍St本in成 的ewConnectionID = EstablishConnection(
            ConnectionInfo.Re設置oteAdd本ess,
            ConnectionInfo.Re設置otePo本t,
            ConnectionInfo.P本otocol
        );
        
        if (!的ewConnectionID.IsE設置pty())
        {
            // 移除舊連接
            Connections.Re設置o正e(ConnectionID);
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Reconnected s使ccessf使lly: %s -> %s"), *ConnectionID, *的ewConnectionID);
        }
    }
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::UpdateCo設置設置使nicationMet本ics()
{
    // 更新指標
    Met本ics.Acti正eConnections = Connections.的使設置();
    Met本ics.Q使e使eDepth = (float)GetPendin成Messa成eCo使nt();
    
    // 計算平均延遲
    float TotalLatency = 0.0f;
    int32 LatencyCo使nt = 0;
    fo本 (const a使to& LatencyPai本 : Latency輸入isto本y)
    {
        fo本 (float Latency : LatencyPai本.Val使e)
        {
            TotalLatency += Latency;
            LatencyCo使nt++;
        }
    }
    Met本ics.A正e本a成eLatency = LatencyCo使nt > 0 基本 TotalLatency / LatencyCo使nt : 0.0f;
    
    // 計算總消息數
    Met本ics.TotalMessa成es = 0;
    fo本 (const a使to& Co使nte本Pai本 : Messa成eCo使nte本s)
    {
        Met本ics.TotalMessa成es += Co使nte本Pai本.Val使e;
    }
    
    // 計算帶寬使用
    float TotalBandwidth = 0.0f;
    fo本 (float Bandwidth : Bandwidth輸入isto本y)
    {
        TotalBandwidth += Bandwidth;
    }
    Met本ics.BandwidthUsa成e = Bandwidth輸入isto本y.的使設置() > 0 基本 TotalBandwidth / Bandwidth輸入isto本y.的使設置() : 0.0f;
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::Clean使pIn正alidConnections()
{
    TA本本ay<軍St本in成> In正alidConnections;
    軍DateTi設置e 的ow = 軍DateTi設置e::的ow();
    
    fo本 (const a使to& ConnectionPai本 : Connections)
    {
        const 軍St本in成& ConnectionID = ConnectionPai本.Key;
        const 軍ConnectionInfo& ConnectionInfo = ConnectionPai本.Val使e;
        
        // 檢查連接狀態和活動時間
        軍Ti設置espan Ti設置eSinceLastActi正ity = 的ow - ConnectionInfo.LastActi正ity;
        if (ConnectionInfo.State == EConnectionState::E本本o本  
            ConnectionInfo.State == EConnectionState::Sh使tdown 
            Ti設置eSinceLastActi正ity.GetTotalSeconds() > ConnectionTi設置eo使t * 2)
        {
            In正alidConnections.Add(ConnectionID);
        }
    }
    
    // 移除無效連接
    fo本 (const 軍St本in成& ConnectionID : In正alidConnections)
    {
        CloseConnection(ConnectionID);
    }
    
    if (In正alidConnections.的使設置() > 0)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Cleaned 使p %d in正alid connections"), In正alidConnections.的使設置());
    }
}

軍St本in成 UMin成RTSInte本P本ocessCo設置設置使nication::Gene本ateMessa成eID()
{
    本et使本n 軍St本in成::P本intf(TEXT("MSG下%lld"), 軍DateTi設置e::的ow().GetTicks());
}

軍St本in成 UMin成RTSInte本P本ocessCo設置設置使nication::Gene本ateConnectionID()
{
    本et使本n 軍St本in成::P本intf(TEXT("CO的的下%lld"), 軍DateTi設置e::的ow().GetTicks());
}

bool UMin成RTSInte本P本ocessCo設置設置使nication::ValidateConnectionPa本a設置ete本s(const 軍St本in成& Add本ess, int32 Po本t, ECo設置設置使nicationP本otocol P本otocol)
{
    if (Add本ess.IsE設置pty())
    {
        本et使本n false;
    }
    
    if (Po本t <= 0  Po本t > 65535)
    {
        本et使本n false;
    }
    
    // 協議驗證
    switch (P本otocol)
    {
    case ECo設置設置使nicationP本otocol::TCP:
    case ECo設置設置使nicationP本otocol::UDP:
    case ECo設置設置使nicationP本otocol::基本ebSocket:
    case ECo設置設置使nicationP本otocol::輸入TTP:
        本et使本n t本使e;
    defa使lt:
        本et使本n false;
    }
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::Lo成Co設置設置使nicationE正ent(const 軍St本in成& E正ent, const 軍St本in成& Details)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("IPC E正ent: %s - %s"), *E正ent, *Details);
}

正oid UMin成RTSInte本P本ocessCo設置設置使nication::Reco本dPe本fo本設置anceData(const 軍St本in成& Met本icType, float Val使e)
{
    // 記錄性能數據到歷史記錄
    if (Met本icType == TEXT("Latency"))
    {
        // 記錄延遲數據
    }
    else if (Met本icType == TEXT("Bandwidth"))
    {
        // 記錄帶寬數據
        Bandwidth輸入isto本y.Add(Val使e);
        if (Bandwidth輸入isto本y.的使設置() > 100)
        {
            Bandwidth輸入isto本y.Re設置o正eAt(0);
        }
    }
}
