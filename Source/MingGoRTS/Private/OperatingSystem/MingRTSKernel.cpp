// Copyright Epic Games, Inc. All Rights Reserved.

#include "OperatingSystem/MingRTSKernel.h"
#include "OperatingSystem/MingRTSKernelSageCommandExtension.h"
#include "TimerManager.h"
#include "Engine/World.h"

UMingRTSKernel::UMingRTSKernel()
    : KernelState(EKernelState::Uninitialized)
    , NextSystemCallID(1)
    , SystemCallTimeout(30.0f)
    , bSystemCallLoggingEnabled(false)
    , MaxSystemCallsPerSecond(1000)
    , bSageCommandExtensionEnabled(false)
    , SageCommandExtension(nullptr)
{
}

bool UMingRTSKernel::Initialize()
{
    if (KernelState != EKernelState::Uninitialized)
    {
        return false;
    }

    SetKernelState(EKernelState::Initializing);

    // 初始化內核信息
    KernelInfo.StartTime = FDateTime::Now();
    KernelInfo.BuildTime = FDateTime::Now();
    StartTime = FDateTime::Now();

    // 重置系統調用管理
    PendingSystemCalls.Empty();
    NextSystemCallID = 1;

    // 清空性能統計
    SystemCallTimes.Empty();
    SystemCallCounts.Empty();
    TotalSystemCalls = 0;

    // 設置內核狀態為運行中
    SetKernelState(EKernelState::Running);

    // 自動初始化至聖者指揮學擴展
    InitializeSageCommandExtension();

    return true;
}

void UMingRTSKernel::Shutdown()
{
    if (KernelState == EKernelState::Stopped || KernelState == EKernelState::ShuttingDown)
    {
        return;
    }

    SetKernelState(EKernelState::ShuttingDown);

    // 清理至聖者指揮學擴展
    if (SageCommandExtension)
    {
        SageCommandExtension = nullptr;
        bSageCommandExtensionEnabled = false;
    }

    // 取消所有待處理的系統調用
    PendingSystemCalls.Empty();

    // 清除定時器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(KernelUpdateTimer);
    }

    SetKernelState(EKernelState::Stopped);
}

void UMingRTSKernel::Restart()
{
    Shutdown();
    Initialize();
}

FSystemCallResponse UMingRTSKernel::MakeSystemCall(const FSystemCallRequest& Request)
{
    FSystemCallResponse Response;
    Response.RequestID = Request.RequestID;
    Response.Timestamp = FDateTime::Now();

    // 檢查內核狀態
    if (KernelState != EKernelState::Running)
    {
        Response.bSuccess = false;
        Response.ErrorMessage = TEXT("內核未運行");
        return Response;
    }

    // 檢查速率限制
    if (TotalSystemCalls >= MaxSystemCallsPerSecond)
    {
        Response.bSuccess = false;
        Response.ErrorMessage = TEXT("系統調用速率超過限制");
        return Response;
    }

    float StartTime = FPlatformTime::Seconds();

    // 處理系統調用
    Response = HandleSystemCall(Request);

    // 記錄性能統計
    float ExecutionTime = FPlatformTime::Seconds() - StartTime;
    if (!SystemCallTimes.Contains(Request.CallType))
    {
        SystemCallTimes.Add(Request.CallType, 0.0f);
        SystemCallCounts.Add(Request.CallType, 0);
    }
    SystemCallTimes[Request.CallType] += ExecutionTime;
    SystemCallCounts[Request.CallType]++;
    TotalSystemCalls++;

    // 廣播系統調用事件
    OnSystemCall.Broadcast(Request, Response);

    return Response;
}

int32 UMingRTSKernel::MakeAsyncSystemCall(const FSystemCallRequest& Request)
{
    int32 RequestID = NextSystemCallID++;

    FSystemCallRequest AsyncRequest = Request;
    AsyncRequest.RequestID = RequestID;

    // 創建待處理響應
    FSystemCallResponse PendingResponse;
    PendingResponse.RequestID = RequestID;
    PendingResponse.bSuccess = false;
    PendingResponse.Timestamp = FDateTime::Now();
    PendingSystemCalls.Add(RequestID, PendingResponse);

    // 異步處理
    HandleAsyncSystemCall(AsyncRequest, RequestID);

    return RequestID;
}

FSystemCallResponse UMingRTSKernel::GetSystemCallResponse(int32 RequestID)
{
    if (PendingSystemCalls.Contains(RequestID))
    {
        return PendingSystemCalls[RequestID];
    }

    FSystemCallResponse Response;
    Response.RequestID = RequestID;
    Response.bSuccess = false;
    Response.ErrorMessage = TEXT("請求未找到");
    return Response;
}

bool UMingRTSKernel::IsSystemCallCompleted(int32 RequestID) const
{
    // 如果不在待處理列表中，則認為已完成
    return !PendingSystemCalls.Contains(RequestID);
}

FKernelInfo UMingRTSKernel::GetKernelInfo() const
{
    FKernelInfo Info = KernelInfo;
    
    // 更新運行時間
    FTimespan Uptime = FDateTime::Now() - StartTime;
    Info.UptimeSeconds = Uptime.GetTotalSeconds();
    
    // 更新系統服務數量
    Info.TotalProcesses = SystemServices.Num();
    Info.ActiveProcesses = SystemServices.Num();

    return Info;
}

FString UMingRTSKernel::GetSystemVersion() const
{
    return FString::Printf(TEXT("MingGoRTS Kernel v%s (Build %s)"), 
        *KernelInfo.KernelVersion, *KernelInfo.BuildNumber);
}

FDateTime UMingRTSKernel::GetSystemTime() const
{
    return FDateTime::Now();
}

bool UMingRTSKernel::SetSystemTime(const FDateTime& NewTime)
{
    // 在遊戲環境中，系統時間通常由引擎管理
    // 這裡僅作為示例
    return false;
}

bool UMingRTSKernel::RegisterInterruptHandler(int32 InterruptNumber, UObject* Handler)
{
    if (!Handler)
    {
        return false;
    }

    InterruptHandlers.Add(InterruptNumber, Handler);
    return true;
}

bool UMingRTSKernel::UnregisterInterruptHandler(int32 InterruptNumber)
{
    return InterruptHandlers.Remove(InterruptNumber) > 0;
}

void UMingRTSKernel::TriggerInterrupt(int32 InterruptNumber)
{
    ProcessInterrupt(InterruptNumber);
    OnInterrupt.Broadcast(InterruptNumber);
}

bool UMingRTSKernel::RegisterSystemService(const FString& ServiceName, UObject* Service)
{
    if (ServiceName.IsEmpty() || !Service)
    {
        return false;
    }

    SystemServices.Add(ServiceName, Service);
    OnSystemServiceRegistered.Broadcast(ServiceName, Service);
    return true;
}

bool UMingRTSKernel::UnregisterSystemService(const FString& ServiceName)
{
    return SystemServices.Remove(ServiceName) > 0;
}

UObject* UMingRTSKernel::GetSystemService(const FString& ServiceName)
{
    if (SystemServices.Contains(ServiceName))
    {
        return SystemServices[ServiceName];
    }
    return nullptr;
}

TArray<FString> UMingRTSKernel::GetRegisteredServices() const
{
    TArray<FString> ServiceNames;
    SystemServices.GetKeys(ServiceNames);
    return ServiceNames;
}

void UMingRTSKernel::SetMaxSystemCalls(int32 MaxCalls)
{
    MaxSystemCallsPerSecond = FMath::Max(1, MaxCalls);
}

void UMingRTSKernel::SetSystemCallTimeout(float TimeoutSeconds)
{
    SystemCallTimeout = FMath::Max(1.0f, TimeoutSeconds);
}

void UMingRTSKernel::EnableSystemCallLogging(bool bEnabled)
{
    bSystemCallLoggingEnabled = bEnabled;
}

float UMingRTSKernel::GetSystemCallAverageTime() const
{
    if (TotalSystemCalls == 0)
    {
        return 0.0f;
    }

    float TotalTime = 0.0f;
    for (const auto& Pair : SystemCallTimes)
    {
        TotalTime += Pair.Value;
    }

    return TotalTime / TotalSystemCalls;
}

int32 UMingRTSKernel::GetSystemCallCount() const
{
    return TotalSystemCalls;
}

TArray<FString> UMingRTSKernel::GetPerformanceMetrics() const
{
    TArray<FString> Metrics;

    Metrics.Add(FString::Printf(TEXT("總系統調用數: %d"), TotalSystemCalls));
    Metrics.Add(FString::Printf(TEXT("平均調用時間: %.3f ms"), GetSystemCallAverageTime() * 1000.0f));
    Metrics.Add(FString::Printf(TEXT("註冊服務數: %d"), SystemServices.Num()));
    Metrics.Add(FString::Printf(TEXT("中斷處理器數: %d"), InterruptHandlers.Num()));

    FTimespan Uptime = FDateTime::Now() - StartTime;
    Metrics.Add(FString::Printf(TEXT("系統運行時間: %.1f 秒"), Uptime.GetTotalSeconds()));

    // 至聖者指揮學擴展狀態
    Metrics.Add(FString::Printf(TEXT("指揮學擴展狀態: %s"), 
        bSageCommandExtensionEnabled ? TEXT("已啟用") : TEXT("未啟用")));

    return Metrics;
}

FString UMingRTSKernel::GenerateKernelDump() const
{
    FString Dump = TEXT("========================================\n");
    Dump += TEXT("MingGoRTS 內核轉儲\n");
    Dump += TEXT("========================================\n\n");

    Dump += FString::Printf(TEXT("內核版本: %s\n"), *KernelInfo.KernelVersion);
    Dump += FString::Printf(TEXT("構建號: %s\n"), *KernelInfo.BuildNumber);
    Dump += FString::Printf(TEXT("當前狀態: %d\n"), (int32)KernelState);

    FTimespan Uptime = FDateTime::Now() - StartTime;
    Dump += FString::Printf(TEXT("運行時間: %.1f 秒\n\n"), Uptime.GetTotalSeconds());

    Dump += TEXT("--- 系統服務 ---\n");
    for (const auto& Pair : SystemServices)
    {
        Dump += FString::Printf(TEXT("  %s: %p\n"), *Pair.Key, Pair.Value);
    }

    Dump += TEXT("\n--- 性能統計 ---\n");
    for (const auto& Pair : SystemCallCounts)
    {
        float AvgTime = 0.0f;
        if (Pair.Value > 0 && SystemCallTimes.Contains(Pair.Key))
        {
            AvgTime = SystemCallTimes[Pair.Key] / Pair.Value * 1000.0f;
        }
        Dump += FString::Printf(TEXT("  調用類型 %d: %d 次 (平均 %.3f ms)\n"), 
            (int32)Pair.Key, Pair.Value, AvgTime);
    }

    // 至聖者指揮學擴展信息
    if (bSageCommandExtensionEnabled && SageCommandExtension)
    {
        Dump += TEXT("\n--- 至聖者指揮學擴展 ---\n");
        Dump += TEXT("  狀態: 已啟用\n");
        Dump += TEXT("  版本: 1.0.0-Sage\n");
    }

    Dump += TEXT("\n========================================\n");

    return Dump;
}

bool UMingRTSKernel::SaveKernelState(const FString& FilePath) const
{
    // 序列化內核狀態到文件
    // 這裡簡化處理，實際應使用更完整的序列化機制
    FString StateData = GenerateKernelDump();
    return FFileHelper::SaveStringToFile(StateData, *FilePath);
}

bool UMingRTSKernel::LoadKernelState(const FString& FilePath)
{
    // 從文件加載內核狀態
    // 這裡簡化處理
    return false;
}

bool UMingRTSKernel::InitializeSageCommandExtension()
{
    if (bSageCommandExtensionEnabled)
    {
        return true;
    }

    // 創建指揮學擴展
    SageCommandExtension = NewObject<UMingRTSKernelSageCommandExtension>(this);
    if (!SageCommandExtension)
    {
        return false;
    }

    // 初始化擴展
    if (!SageCommandExtension->InitializeSageCommandExtension())
    {
        SageCommandExtension = nullptr;
        return false;
    }

    // 綁定到內核
    if (!SageCommandExtension->BindToKernel(this))
    {
        SageCommandExtension = nullptr;
        return false;
    }

    bSageCommandExtensionEnabled = true;
    return true;
}

bool UMingRTSKernel::IsSageCommandExtensionEnabled() const
{
    return bSageCommandExtensionEnabled;
}

FString UMingRTSKernel::GetSageCommandVersion() const
{
    if (bSageCommandExtensionEnabled && SageCommandExtension)
    {
        return TEXT("1.0.0-Sage");
    }
    return TEXT("未啟用");
}

UObject* UMingRTSKernel::GetSageCommandExtension() const
{
    return SageCommandExtension;
}

void UMingRTSKernel::BeginDestroy()
{
    Shutdown();
    Super::BeginDestroy();
}

FSystemCallResponse UMingRTSKernel::HandleSystemCall(const FSystemCallRequest& Request)
{
    FSystemCallResponse Response;
    Response.RequestID = Request.RequestID;
    Response.Timestamp = FDateTime::Now();

    // 處理至聖者指揮學系統調用
    if (Request.CallType >= ESystemCall::SageCommand_CreateCharacter && 
        Request.CallType <= ESystemCall::SageCommand_GetStatus)
    {
        if (bSageCommandExtensionEnabled && SageCommandExtension)
        {
            return SageCommandExtension->HandleSageCommandSystemCall(Request);
        }
        else
        {
            Response.bSuccess = false;
            Response.ErrorMessage = TEXT("至聖者指揮學擴展未啟用");
            return Response;
        }
    }

    // 處理其他系統調用
    switch (Request.CallType)
    {
    case ESystemCall::GetSystemInfo:
        return HandleGetSystemInfo(Request.Parameters);

    case ESystemCall::GetVersion:
        Response.bSuccess = true;
        // 將版本字符串轉換為字節數組
        FTCHARToUTF8 Converter(*GetSystemVersion());
        Response.ResultData.Append((uint8*)Converter.Get(), Converter.Length());
        return Response;

    case ESystemCall::GetTime:
        Response.bSuccess = true;
        // 這裡簡化處理
        return Response;

    default:
        Response.bSuccess = false;
        Response.ErrorMessage = FString::Printf(TEXT("未實現的系統調用: %d"), (int32)Request.CallType);
        return Response;
    }
}

void UMingRTSKernel::HandleAsyncSystemCall(const FSystemCallRequest& Request, int32 RequestID)
{
    // 異步處理系統調用
    FSystemCallResponse Response = HandleSystemCall(Request);
    Response.RequestID = RequestID;

    // 更新待處理響應
    if (PendingSystemCalls.Contains(RequestID))
    {
        PendingSystemCalls[RequestID] = Response;
    }
}

FSystemCallResponse UMingRTSKernel::HandleCreateProcess(const TArray<uint8>& Parameters)
{
    FSystemCallResponse Response;
    Response.bSuccess = false;
    Response.ErrorMessage = TEXT("進程管理功能未實現");
    return Response;
}

FSystemCallResponse UMingRTSKernel::HandleTerminateProcess(const TArray<uint8>& Parameters)
{
    FSystemCallResponse Response;
    Response.bSuccess = false;
    Response.ErrorMessage = TEXT("進程終止功能未實現");
    return Response;
}

FSystemCallResponse UMingRTSKernel::HandleAllocateMemory(const TArray<uint8>& Parameters)
{
    FSystemCallResponse Response;
    Response.bSuccess = false;
    Response.ErrorMessage = TEXT("內存分配功能未實現");
    return Response;
}

FSystemCallResponse UMingRTSKernel::HandleFreeMemory(const TArray<uint8>& Parameters)
{
    FSystemCallResponse Response;
    Response.bSuccess = false;
    Response.ErrorMessage = TEXT("內存釋放功能未實現");
    return Response;
}

FSystemCallResponse UMingRTSKernel::HandleOpenFile(const TArray<uint8>& Parameters)
{
    FSystemCallResponse Response;
    Response.bSuccess = false;
    Response.ErrorMessage = TEXT("文件操作功能未實現");
    return Response;
}

FSystemCallResponse UMingRTSKernel::HandleReadFile(const TArray<uint8>& Parameters)
{
    FSystemCallResponse Response;
    Response.bSuccess = false;
    Response.ErrorMessage = TEXT("文件讀取功能未實現");
    return Response;
}

FSystemCallResponse UMingRTSKernel::HandleWriteFile(const TArray<uint8>& Parameters)
{
    FSystemCallResponse Response;
    Response.bSuccess = false;
    Response.ErrorMessage = TEXT("文件寫入功能未實現");
    return Response;
}

FSystemCallResponse UMingRTSKernel::HandleGetSystemInfo(const TArray<uint8>& Parameters)
{
    FSystemCallResponse Response;
    Response.bSuccess = true;

    // 簡化處理：僅返回內核運行狀態
    FKernelInfo Info = GetKernelInfo();
    Response.ResultData.Add(static_cast<uint8>(Info.CurrentState));

    return Response;
}

void UMingRTSKernel::ProcessInterrupt(int32 InterruptNumber)
{
    // 處理中斷
    // 這裡簡化處理，實際應調用註冊的中斷處理器
}

void UMingRTSKernel::SetKernelState(EKernelState NewState)
{
    EKernelState OldState = KernelState;
    KernelState = NewState;
    KernelInfo.CurrentState = NewState;

    OnKernelStateChanged.Broadcast(OldState, NewState);
}

void UMingRTSKernel::UpdateKernelInfo()
{
    // 更新內核信息
    FTimespan Uptime = FDateTime::Now() - StartTime;
    KernelInfo.UptimeSeconds = Uptime.GetTotalSeconds();
}
