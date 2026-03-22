// Copyright Epic Games, Inc. All Rights Reserved.

#include "OperatingSystem/MingRTSKernelSageCommandExtension.h"
#include "SageCommand/MingSageCharacterSystem.h"
#include "SageCommand/MingThreePowerSystem.h"
#include "SageCommand/MingMoralAuthority.h"
#include "SageCommand/MingStrategyAuthority.h"
#include "SageCommand/MingMilitaryAuthority.h"
#include "SageCommand/MingWuXingRhythmSystem.h"
#include "SageCommand/MingAntiFallSystem.h"
#include "OperatingSystem/MingRTSKernel.h"
#include "TimerManager.h"

UMingRTSKernelSageCommandExtension::UMingRTSKernelSageCommandExtension()
    : BoundKernel(nullptr)
    , bIsInitialized(false)
    , UpdateInterval(1.0f)
{
}

bool UMingRTSKernelSageCommandExtension::InitializeSageCommandExtension()
{
    if (bIsInitialized)
    {
        return true;
    }

    InitializeSubSystems();

    bIsInitialized = true;
    return true;
}

void UMingRTSKernelSageCommandExtension::InitializeSubSystems()
{
    // 創建並初始化各子系統
    if (!CharacterSystem)
    {
        CharacterSystem = NewObject<UMingSageCharacterSystem>(this);
        CharacterSystem->InitializeCharacterSystem();
    }

    if (!ThreePowerSystem)
    {
        ThreePowerSystem = NewObject<UMingThreePowerSystem>(this);
        ThreePowerSystem->InitializeThreePowerSystem();
    }

    if (!MoralAuthority)
    {
        MoralAuthority = NewObject<UMingMoralAuthority>(this);
        MoralAuthority->InitializeMoralAuthority();
    }

    if (!StrategyAuthority)
    {
        StrategyAuthority = NewObject<UMingStrategyAuthority>(this);
        StrategyAuthority->InitializeStrategyAuthority();
    }

    if (!MilitaryAuthority)
    {
        MilitaryAuthority = NewObject<UMingMilitaryAuthority>(this);
        MilitaryAuthority->InitializeMilitaryAuthority();
    }

    if (!WuXingRhythmSystem)
    {
        WuXingRhythmSystem = NewObject<UMingWuXingRhythmSystem>(this);
        WuXingRhythmSystem->InitializeWuXingSystem();
    }

    if (!AntiFallSystem)
    {
        AntiFallSystem = NewObject<UMingAntiFallSystem>(this);
        AntiFallSystem->InitializeAntiFallSystem();
    }

    // 綁定事件
    if (AntiFallSystem)
    {
        AntiFallSystem->OnFallOccurred.AddDynamic(this, &UMingRTSKernelSageCommandExtension::OnCharacterFallen);
        AntiFallSystem->OnFallRiskWarning.AddDynamic(this, &UMingRTSKernelSageCommandExtension::OnFallRiskWarning);
    }
}

bool UMingRTSKernelSageCommandExtension::BindToKernel(UMingRTSKernel* Kernel)
{
    if (!Kernel)
    {
        return false;
    }

    BoundKernel = Kernel;

    // 註冊為系統服務
    RegisterAsSystemService(Kernel);

    // 綁定內核狀態改變事件
    Kernel->OnKernelStateChanged.AddDynamic(this, &UMingRTSKernelSageCommandExtension::OnKernelStateChanged);

    return true;
}

bool UMingRTSKernelSageCommandExtension::RegisterAsSystemService(UMingRTSKernel* Kernel)
{
    if (!Kernel)
    {
        return false;
    }

    return Kernel->RegisterSystemService(SystemServiceName, this);
}

FSageCommandKernelResponse UMingRTSKernelSageCommandExtension::ExecuteSageCommandCall(const FSageCommandKernelRequest& Request)
{
    FSageCommandKernelResponse Response;

    switch (Request.CallType)
    {
    case ESageCommandSystemCall::CreateSageCharacter:
        Response = HandleCreateCharacter(Request);
        break;
    case ESageCommandSystemCall::UseEvilStrategy:
    case ESageCommandSystemCall::UseRighteousStrategy:
        Response = HandleUseStrategy(Request);
        break;
    case ESageCommandSystemCall::CheckFallStatus:
        Response = HandleCheckFall(Request);
        break;
    case ESageCommandSystemCall::CoordinateThreePowers:
        Response = HandleCoordinatePowers(Request);
        break;
    case ESageCommandSystemCall::StartWuXingCycle:
    case ESageCommandSystemCall::AdvanceWuXingPhase:
        Response = HandleWuXingOperation(Request);
        break;
    case ESageCommandSystemCall::CreateAtonementTask:
    case ESageCommandSystemCall::CompleteAtonementTask:
    case ESageCommandSystemCall::PerformAtonement:
        Response = HandleAtonement(Request);
        break;
    case ESageCommandSystemCall::GetSageCommandStatus:
    default:
        Response = HandleGetStatus(Request);
        break;
    }

    // 更新系統狀態
    UpdateSystemStatus();
    Response.SystemStatus = CachedStatus;

    return Response;
}

FSageCommandKernelResponse UMingRTSKernelSageCommandExtension::HandleCreateCharacter(const FSageCommandKernelRequest& Request)
{
    FSageCommandKernelResponse Response;

    if (!CharacterSystem)
    {
        Response.bSuccess = false;
        Response.ErrorMessage = TEXT("角色系統未初始化");
        return Response;
    }

    ESageCharacterType CharacterType = static_cast<ESageCharacterType>(Request.IntParam1);
    FSageCharacterData CharacterData = CharacterSystem->CreateCharacter(CharacterType, Request.StringParam1);

    Response.bSuccess = (CharacterData.CharacterID != -1);
    Response.IntResult = CharacterData.CharacterID;
    Response.StringResult = CharacterSystem->GetCharacterDescription(CharacterType);

    return Response;
}

FSageCommandKernelResponse UMingRTSKernelSageCommandExtension::HandleUseStrategy(const FSageCommandKernelRequest& Request)
{
    FSageCommandKernelResponse Response;

    if (!StrategyAuthority || !AntiFallSystem)
    {
        Response.bSuccess = false;
        Response.ErrorMessage = TEXT("策略或防墮系統未初始化");
        return Response;
    }

    bool bIsEvil = (Request.CallType == ESageCommandSystemCall::UseEvilStrategy);
    ESixStrategyType StrategyType = static_cast<ESixStrategyType>(Request.IntParam1);

    // 嘗試使用策略
    bool bSuccess = StrategyAuthority->UseStrategy(StrategyType, Request.FloatParam1);

    if (bSuccess)
    {
        // 更新墮落狀態
        FStrategyData StrategyData = StrategyAuthority->GetStrategyData(StrategyType);
        int32 FallDelta = bIsEvil ? StrategyData.FallValueImpact : -10; // 正策減少墮落值

        AntiFallSystem->UpdateFallStatus(FallDelta, bIsEvil);
    }

    Response.bSuccess = bSuccess;
    Response.IntResult = bSuccess ? 1 : 0;

    return Response;
}

FSageCommandKernelResponse UMingRTSKernelSageCommandExtension::HandleCheckFall(const FSageCommandKernelRequest& Request)
{
    FSageCommandKernelResponse Response;

    if (!AntiFallSystem)
    {
        Response.bSuccess = false;
        Response.ErrorMessage = TEXT("防墮系統未初始化");
        return Response;
    }

    FFallStatusData FallStatus = AntiFallSystem->GetFallStatus();

    Response.bSuccess = true;
    Response.IntResult = FallStatus.CurrentFallValue;
    Response.StringResult = AntiFallSystem->GetRiskLevelDescription();

    return Response;
}

FSageCommandKernelResponse UMingRTSKernelSageCommandExtension::HandleCoordinatePowers(const FSageCommandKernelRequest& Request)
{
    FSageCommandKernelResponse Response;

    if (!ThreePowerSystem)
    {
        Response.bSuccess = false;
        Response.ErrorMessage = TEXT("三權系統未初始化");
        return Response;
    }

    bool bCoordinated = ThreePowerSystem->CoordinatePowers();

    Response.bSuccess = bCoordinated;
    Response.IntResult = ThreePowerSystem->CalculateBalanceValue();

    return Response;
}

FSageCommandKernelResponse UMingRTSKernelSageCommandExtension::HandleWuXingOperation(const FSageCommandKernelRequest& Request)
{
    FSageCommandKernelResponse Response;

    if (!WuXingRhythmSystem)
    {
        Response.bSuccess = false;
        Response.ErrorMessage = TEXT("五行系統未初始化");
        return Response;
    }

    bool bSuccess = false;

    if (Request.CallType == ESageCommandSystemCall::StartWuXingCycle)
    {
        WuXingRhythmSystem->StartNewCycle();
        bSuccess = true;
    }
    else if (Request.CallType == ESageCommandSystemCall::AdvanceWuXingPhase)
    {
        bSuccess = WuXingRhythmSystem->AdvanceToNextPhase();
    }

    FWuXingRhythmState State = WuXingRhythmSystem->GetCurrentState();

    Response.bSuccess = bSuccess;
    Response.IntResult = static_cast<int32>(State.CurrentPhase);
    Response.StringResult = WuXingRhythmSystem->GetPhaseDescription(State.CurrentPhase);

    return Response;
}

FSageCommandKernelResponse UMingRTSKernelSageCommandExtension::HandleAtonement(const FSageCommandKernelRequest& Request)
{
    FSageCommandKernelResponse Response;

    if (!AntiFallSystem)
    {
        Response.bSuccess = false;
        Response.ErrorMessage = TEXT("防墮系統未初始化");
        return Response;
    }

    if (Request.CallType == ESageCommandSystemCall::CreateAtonementTask)
    {
        FAtonementTaskData Task = AntiFallSystem->CreateAtonementTask(Request.IntParam1);
        Response.bSuccess = (Task.TaskID != 0);
        Response.IntResult = Task.TaskID;
    }
    else if (Request.CallType == ESageCommandSystemCall::CompleteAtonementTask)
    {
        bool bCompleted = AntiFallSystem->CompleteAtonementTask(Request.IntParam1);
        Response.bSuccess = bCompleted;
        Response.IntResult = bCompleted ? 1 : 0;
    }

    return Response;
}

FSageCommandKernelResponse UMingRTSKernelSageCommandExtension::HandleGetStatus(const FSageCommandKernelRequest& Request)
{
    FSageCommandKernelResponse Response;

    UpdateSystemStatus();

    Response.bSuccess = true;
    Response.SystemStatus = CachedStatus;

    return Response;
}

FSageCommandKernelStatus UMingRTSKernelSageCommandExtension::GetSageCommandStatus() const
{
    return CachedStatus;
}

void UMingRTSKernelSageCommandExtension::UpdateSystemStatus()
{
    if (AntiFallSystem)
    {
        FFallStatusData FallStatus = AntiFallSystem->GetFallStatus();
        CachedStatus.CurrentFallValue = FallStatus.CurrentFallValue;
        CachedStatus.FallThreshold = FallStatus.FallThreshold;
        CachedStatus.bIsFallen = FallStatus.bIsFallen;
        CachedStatus.ConsecutiveEvilUses = FallStatus.ConsecutiveEvilUses;
        CachedStatus.TotalEvilUses = FallStatus.TotalEvilUses;
        CachedStatus.TotalRighteousUses = FallStatus.TotalRighteousUses;
        CachedStatus.ActiveAtonementTasks = AntiFallSystem->GetActiveAtonementTasks().Num();
    }

    if (WuXingRhythmSystem)
    {
        FWuXingRhythmState State = WuXingRhythmSystem->GetCurrentState();
        CachedStatus.CurrentWuXingPhase = static_cast<int32>(State.CurrentPhase);
    }

    if (StrategyAuthority)
    {
        CachedStatus.CurrentStrategyDirection = StrategyAuthority->IsUsingEvilStrategy() ? 1 : 0;
    }

    if (ThreePowerSystem)
    {
        CachedStatus.ThreePowerBalance = ThreePowerSystem->CalculateBalanceValue();
    }

    // 檢查系統健康
    CachedStatus.bSystemHealthy = IsSystemHealthy();
}

bool UMingRTSKernelSageCommandExtension::SetCharacterType(int32 CharacterType, const FString& CharacterName)
{
    if (!CharacterSystem)
    {
        return false;
    }

    ESageCharacterType Type = static_cast<ESageCharacterType>(CharacterType);
    FSageCharacterData CharacterData = CharacterSystem->CreateCharacter(Type, CharacterName);

    if (CharacterData.CharacterID != -1)
    {
        CachedStatus.CurrentCharacterType = CharacterType;
        return true;
    }

    return false;
}

FSageCommandKernelResponse UMingRTSKernelSageCommandExtension::ExecuteStrategy(int32 StrategyType, bool bIsEvil)
{
    FSageCommandKernelRequest Request;
    Request.CallType = bIsEvil ? ESageCommandSystemCall::UseEvilStrategy : ESageCommandSystemCall::UseRighteousStrategy;
    Request.IntParam1 = StrategyType;

    return ExecuteSageCommandCall(Request);
}

bool UMingRTSKernelSageCommandExtension::CoordinateThreePowers()
{
    FSageCommandKernelRequest Request;
    Request.CallType = ESageCommandSystemCall::CoordinateThreePowers;

    FSageCommandKernelResponse Response = ExecuteSageCommandCall(Request);
    return Response.bSuccess;
}

bool UMingRTSKernelSageCommandExtension::StartWuXingCycle()
{
    FSageCommandKernelRequest Request;
    Request.CallType = ESageCommandSystemCall::StartWuXingCycle;

    FSageCommandKernelResponse Response = ExecuteSageCommandCall(Request);
    return Response.bSuccess;
}

bool UMingRTSKernelSageCommandExtension::AdvanceWuXingPhase()
{
    FSageCommandKernelRequest Request;
    Request.CallType = ESageCommandSystemCall::AdvanceWuXingPhase;

    FSageCommandKernelResponse Response = ExecuteSageCommandCall(Request);
    return Response.bSuccess;
}

FSageCommandKernelResponse UMingRTSKernelSageCommandExtension::UpdateFallStatus(int32 FallValueDelta, bool bIsEvil)
{
    if (!AntiFallSystem)
    {
        FSageCommandKernelResponse Response;
        Response.bSuccess = false;
        Response.ErrorMessage = TEXT("防墮系統未初始化");
        return Response;
    }

    AntiFallSystem->UpdateFallStatus(FallValueDelta, bIsEvil);

    FSageCommandKernelResponse Response;
    Response.bSuccess = true;
    Response.IntResult = AntiFallSystem->GetFallStatus().CurrentFallValue;
    Response.StringResult = AntiFallSystem->GetRiskLevelDescription();

    return Response;
}

int32 UMingRTSKernelSageCommandExtension::CreateAtonementTask(int32 DifficultyLevel)
{
    if (!AntiFallSystem)
    {
        return -1;
    }

    FAtonementTaskData Task = AntiFallSystem->CreateAtonementTask(DifficultyLevel);
    return Task.TaskID;
}

bool UMingRTSKernelSageCommandExtension::CompleteAtonementTask(int32 TaskID)
{
    if (!AntiFallSystem)
    {
        return false;
    }

    return AntiFallSystem->CompleteAtonementTask(TaskID);
}

FString UMingRTSKernelSageCommandExtension::GetMoralAdvice() const
{
    if (MoralAuthority)
    {
        return MoralAuthority->GetMoralAdvice();
    }

    return TEXT("道權系統未初始化");
}

FString UMingRTSKernelSageCommandExtension::GetStrategyRecommendation() const
{
    if (StrategyAuthority)
    {
        TArray<ESixStrategyType> Recommendations = StrategyAuthority->GetRecommendedStrategies();
        if (Recommendations.Num() > 0)
        {
            return StrategyAuthority->GetStrategyDescription(Recommendations[0]);
        }
    }

    if (WuXingRhythmSystem)
    {
        return WuXingRhythmSystem->GetRecommendedStrategyForCurrentPhase();
    }

    return TEXT("策略系統未初始化");
}

FString UMingRTSKernelSageCommandExtension::GetWuXingRecommendation() const
{
    if (WuXingRhythmSystem)
    {
        return WuXingRhythmSystem->GetRecommendedStrategyForCurrentPhase();
    }

    return TEXT("五行系統未初始化");
}

bool UMingRTSKernelSageCommandExtension::IsSystemHealthy() const
{
    bool bHealthy = true;

    if (MoralAuthority)
    {
        bHealthy &= MoralAuthority->IsHealthy();
    }

    if (StrategyAuthority)
    {
        bHealthy &= StrategyAuthority->IsHealthy();
    }

    if (MilitaryAuthority)
    {
        bHealthy &= MilitaryAuthority->IsHealthy();
    }

    if (AntiFallSystem)
    {
        FFallStatusData Status = AntiFallSystem->GetFallStatus();
        if (Status.bIsFallen)
        {
            bHealthy = false;
        }
    }

    return bHealthy;
}

FSystemCallResponse UMingRTSKernelSageCommandExtension::HandleSageCommandSystemCall(const FSystemCallRequest& Request)
{
    FSystemCallResponse Response;
    Response.RequestID = Request.RequestID;
    Response.Timestamp = FDateTime::Now();

    // 將通用系統調用轉換為指揮學系統調用
    FSageCommandKernelRequest SageRequest;

    // 從參數中解析調用類型
    if (Request.Parameters.Num() > 0)
    {
        SageRequest.CallType = static_cast<ESageCommandSystemCall>(Request.Parameters[0]);
    }

    // 執行指揮學調用
    FSageCommandKernelResponse SageResponse = ExecuteSageCommandCall(SageRequest);

    Response.bSuccess = SageResponse.bSuccess;

    // 序列化響應
    // 簡化處理：將整數結果放入響應數據
    Response.ResultData.Add(static_cast<uint8>(SageResponse.IntResult));
    Response.ResultData.Add(static_cast<uint8>(SageResponse.IntResult >> 8));
    Response.ResultData.Add(static_cast<uint8>(SageResponse.IntResult >> 16));
    Response.ResultData.Add(static_cast<uint8>(SageResponse.IntResult >> 24));

    if (!SageResponse.bSuccess)
    {
        Response.ErrorMessage = SageResponse.ErrorMessage;
    }

    return Response;
}

void UMingRTSKernelSageCommandExtension::OnKernelStateChanged(EKernelState OldState, EKernelState NewState)
{
    switch (NewState)
    {
    case EKernelState::Running:
        // 內核啟動時初始化指揮學系統
        InitializeSageCommandExtension();
        break;

    case EKernelState::ShuttingDown:
        // 清理定時器
        if (UpdateTimerHandle.IsValid())
        {
            if (UWorld* World = GetWorld())
            {
                World->GetTimerManager().ClearTimer(UpdateTimerHandle);
            }
        }
        break;

    default:
        break;
    }
}

void UMingRTSKernelSageCommandExtension::OnSystemUpdate()
{
    // 定期更新系統狀態
    UpdateSystemStatus();

    // 更新子系統
    if (StrategyAuthority)
    {
        StrategyAuthority->UpdateCooldowns();
    }

    if (WuXingRhythmSystem)
    {
        WuXingRhythmSystem->UpdateSystem();
    }

    // 檢查系統健康並發出警告
    if (!CachedStatus.bSystemHealthy)
    {
        OnSageCommandWarning.Broadcast(TEXT("至聖者指揮學系統處於異常狀態，請檢查三權平衡和墮落狀態。"));
    }
}
