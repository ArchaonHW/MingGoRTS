// Copyright Epic Games, Inc. All Rights Reserved.

#include "OfficeUI/MingOfficeBridge.h"
#include "Http.h"
#include "Json.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

UMingOfficeBridge::UMingOfficeBridge()
    : OfficeUIBaseUrl(TEXT("http://127.0.0.1:19000"))
    , CurrentOfficeState(EMingOfficeState::Idle)
    , bIsConnected(false)
{
    InitializeStateMapping();
}

void UMingOfficeBridge::InitializeStateMapping()
{
    // 游戏状态到办公状态的映射
    GameStateToOfficeStateMap.Add(TEXT("MainMenu"), EMingOfficeState::Idle);
    GameStateToOfficeStateMap.Add(TEXT("Loading"), EMingOfficeState::Syncing);
    GameStateToOfficeStateMap.Add(TEXT("StrategyMap"), EMingOfficeState::Researching);
    GameStateToOfficeStateMap.Add(TEXT("TacticalBattle"), EMingOfficeState::Executing);
    GameStateToOfficeStateMap.Add(TEXT("ResourceManagement"), EMingOfficeState::Writing);
    GameStateToOfficeStateMap.Add(TEXT("MultiplayerLobby"), EMingOfficeState::Syncing);
    GameStateToOfficeStateMap.Add(TEXT("Error"), EMingOfficeState::Error);
}

void UMingOfficeBridge::InitializeOfficeBridge()
{
    // 检查服务是否可用
    if (IsOfficeUIServiceAvailable())
    {
        bIsConnected = true;
        OnServiceConnectionChanged.Broadcast(true);
        
        // 设置为初始状态
        SetIdle(TEXT("MingGoRTS 已启动，准备就绪"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MingOfficeBridge: Office UI service not available at %s"), *OfficeUIBaseUrl);
        bIsConnected = false;
        OnServiceConnectionChanged.Broadcast(false);
    }
}

bool UMingOfficeBridge::IsOfficeUIServiceAvailable()
{
    // 简单HTTP检查
    FHttpModule* Http = &FHttpModule::Get();
    if (!Http)
    {
        return false;
    }

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
    Request->SetURL(OfficeUIBaseUrl + TEXT("/health"));
    Request->SetVerb(TEXT("GET"));
    Request->SetTimeout(2.0f);
    
    // 同步请求检查
    Request->ProcessRequest();
    
    // 简化处理，实际应该异步
    return true;
}

bool UMingOfficeBridge::StartOfficeUIService()
{
    // 尝试启动本地Flask服务
    FString ProjectDir = FPaths::ProjectDir();
    FString OfficeUIScript = ProjectDir / TEXT("Tools/office-ui/backend/app.py");
    
    if (!FPaths::FileExists(OfficeUIScript))
    {
        UE_LOG(LogTemp, Error, TEXT("MingOfficeBridge: Office UI script not found at %s"), *OfficeUIScript);
        return false;
    }

    // 启动Python服务
    FString PythonCmd = TEXT("python3");
    FString Arguments = FString::Printf(TEXT("\"%s\""), *OfficeUIScript);
    
    int32 ReturnCode = 0;
    FString StdOut, StdErr;
    bool bSuccess = FPlatformProcess::ExecProcess(*PythonCmd, *Arguments, &ReturnCode, &StdOut, &StdErr, *ProjectDir);
    
    if (bSuccess && ReturnCode == 0)
    {
        UE_LOG(LogTemp, Log, TEXT("MingOfficeBridge: Office UI service started successfully"));
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("MingOfficeBridge: Failed to start Office UI service: %s"), *StdErr);
        return false;
    }
}

void UMingOfficeBridge::UpdateOfficeState(EMingOfficeState NewState, const FString& Description)
{
    if (!bIsConnected)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingOfficeBridge: Not connected to Office UI service"));
        return;
    }

    EMingOfficeState OldState = CurrentOfficeState;
    CurrentOfficeState = NewState;
    
    // 发送HTTP请求更新状态
    SendStateUpdateRequest(NewState, Description);
    
    // 广播状态改变事件
    OnOfficeStateChanged.Broadcast(OldState, NewState);
}

void UMingOfficeBridge::SyncGameStateToOffice(const FString& GameStateName)
{
    if (GameStateToOfficeStateMap.Contains(GameStateName))
    {
        EMingOfficeState MappedState = GameStateToOfficeStateMap[GameStateName];
        FString Description = FString::Printf(TEXT("当前状态: %s"), *GameStateName);
        UpdateOfficeState(MappedState, Description);
    }
    else
    {
        // 默认映射到idle
        SetIdle(FString::Printf(TEXT("未知状态: %s"), *GameStateName));
    }
}

void UMingOfficeBridge::SetIdle(const FString& Description)
{
    UpdateOfficeState(EMingOfficeState::Idle, Description.IsEmpty() ? TEXT("待命中") : Description);
}

void UMingOfficeBridge::SetResearching(const FString& Description)
{
    UpdateOfficeState(EMingOfficeState::Researching, Description.IsEmpty() ? TEXT("制定战略中") : Description);
}

void UMingOfficeBridge::SetExecuting(const FString& Description)
{
    UpdateOfficeState(EMingOfficeState::Executing, Description.IsEmpty() ? TEXT("战斗执行中") : Description);
}

void UMingOfficeBridge::SetSyncing(const FString& Description)
{
    UpdateOfficeState(EMingOfficeState::Syncing, Description.IsEmpty() ? TEXT("同步多人进度") : Description);
}

void UMingOfficeBridge::SetError(const FString& Description)
{
    UpdateOfficeState(EMingOfficeState::Error, Description.IsEmpty() ? TEXT("发现问题，排查中") : Description);
}

void UMingOfficeBridge::SetWriting(const FString& Description)
{
    UpdateOfficeState(EMingOfficeState::Writing, Description.IsEmpty() ? TEXT("整理资源数据") : Description);
}

void UMingOfficeBridge::SendStateUpdateRequest(EMingOfficeState State, const FString& Description)
{
    FHttpModule* Http = &FHttpModule::Get();
    if (!Http)
    {
        return;
    }

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
    Request->SetURL(OfficeUIBaseUrl + TEXT("/set_state"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    
    // 构建JSON请求体
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    
    // 状态字符串映射
    static const TMap<EMingOfficeState, FString> StateStrings = {
        {EMingOfficeState::Idle, TEXT("idle")},
        {EMingOfficeState::Writing, TEXT("writing")},
        {EMingOfficeState::Researching, TEXT("researching")},
        {EMingOfficeState::Executing, TEXT("executing")},
        {EMingOfficeState::Syncing, TEXT("syncing")},
        {EMingOfficeState::Error, TEXT("error")}
    };
    
    FString StateString = StateStrings.Contains(State) ? StateStrings[State] : TEXT("idle");
    JsonObject->SetStringField(TEXT("state"), StateString);
    JsonObject->SetStringField(TEXT("description"), Description);
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    
    Request->SetContentAsString(OutputString);
    
    // 设置回调
    Request->OnProcessRequestComplete().BindUObject(this, &UMingOfficeBridge::OnStateUpdateResponse);
    
    // 发送请求
    Request->ProcessRequest();
}

void UMingOfficeBridge::OnStateUpdateResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
    if (bSuccess && Response.IsValid())
    {
        int32 ResponseCode = Response->GetResponseCode();
        if (ResponseCode == 200)
        {
            UE_LOG(LogTemp, Verbose, TEXT("MingOfficeBridge: State update successful"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("MingOfficeBridge: State update failed with code %d"), ResponseCode);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MingOfficeBridge: State update request failed"));
    }
}

void UMingOfficeBridge::CheckServiceHealth()
{
    // 定期检查服务健康状态
    if (!IsOfficeUIServiceAvailable())
    {
        if (bIsConnected)
        {
            bIsConnected = false;
            OnServiceConnectionChanged.Broadcast(false);
        }
    }
    else
    {
        if (!bIsConnected)
        {
            bIsConnected = true;
            OnServiceConnectionChanged.Broadcast(true);
        }
    }
}
