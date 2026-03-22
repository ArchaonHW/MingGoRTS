// Copyright Epic Games, Inc. All Rights Reserved.

#include "SageCommand/MingMilitaryAuthority.h"

UMingMilitaryAuthority::UMingMilitaryAuthority()
    : NextCommandID(1)
    , MaxActiveCommands(50)
    , MaxHistorySize(100)
    , bIsInitialized(false)
{
}

void UMingMilitaryAuthority::InitializeMilitaryAuthority()
{
    if (bIsInitialized)
    {
        return;
    }

    ActiveCommands.Empty();
    CommandHistory.Empty();
    NextCommandID = 1;

    bIsInitialized = true;
}

int32 UMingMilitaryAuthority::IssueCommand(EMilitaryCommandType CommandType, const FVector& TargetLocation, 
                                            ECommandPriority Priority, bool bIsPublic, const FString& Description)
{
    if (!bIsInitialized)
    {
        return -1;
    }

    // 檢查活動命令數量上限
    if (ActiveCommands.Num() >= MaxActiveCommands)
    {
        // 取消最低優先級的命令
        int32 LowestPriorityIndex = -1;
        ECommandPriority LowestPriority = ECommandPriority::Critical;

        for (int32 i = 0; i < ActiveCommands.Num(); ++i)
        {
            if (ActiveCommands[i].Priority < LowestPriority)
            {
                LowestPriority = ActiveCommands[i].Priority;
                LowestPriorityIndex = i;
            }
        }

        if (LowestPriorityIndex >= 0)
        {
            CancelCommand(ActiveCommands[LowestPriorityIndex].CommandID);
        }
    }

    // 創建新命令
    FMilitaryCommandData NewCommand;
    NewCommand.CommandID = GetNextCommandID();
    NewCommand.CommandType = CommandType;
    NewCommand.TargetLocation = TargetLocation;
    NewCommand.Priority = Priority;
    NewCommand.bIsPublic = bIsPublic;
    NewCommand.CommandDescription = Description;
    NewCommand.IssueTime = FDateTime::Now();
    NewCommand.bIsCompleted = false;

    // 設置預期完成時間 (根據優先級)
    float ExpectedDuration = 60.0f; // 默認1分鐘
    switch (Priority)
    {
    case ECommandPriority::Critical:
        ExpectedDuration = 10.0f;
        break;
    case ECommandPriority::High:
        ExpectedDuration = 30.0f;
        break;
    case ECommandPriority::Normal:
        ExpectedDuration = 60.0f;
        break;
    case ECommandPriority::Low:
        ExpectedDuration = 120.0f;
        break;
    }
    NewCommand.ExpectedCompletionTime = NewCommand.IssueTime + FTimespan::FromSeconds(ExpectedDuration);

    // 驗證命令
    if (!ValidateCommand(NewCommand))
    {
        return -1;
    }

    // 添加到活動命令列表
    ActiveCommands.Add(NewCommand);

    // 廣播命令發布事件
    OnCommandIssued.Broadcast(NewCommand);

    return NewCommand.CommandID;
}

FCommandExecutionResult UMingMilitaryAuthority::ExecuteCommand(int32 CommandID)
{
    FCommandExecutionResult Result;
    Result.bSuccess = false;

    int32 Index = FindCommandIndex(CommandID);
    if (Index < 0)
    {
        Result.ResultDescription = TEXT("命令未找到");
        return Result;
    }

    FMilitaryCommandData& Command = ActiveCommands[Index];
    float StartTime = FPlatformTime::Seconds();

    // 根據命令類型執行
    switch (Command.CommandType)
    {
    case EMilitaryCommandType::Move:
        Result = ExecuteMoveCommand(Command);
        break;
    case EMilitaryCommandType::Attack:
        Result = ExecuteAttackCommand(Command);
        break;
    case EMilitaryCommandType::Defend:
        Result = ExecuteDefendCommand(Command);
        break;
    case EMilitaryCommandType::Retreat:
        Result = ExecuteRetreatCommand(Command);
        break;
    case EMilitaryCommandType::Formation:
        Result = ExecuteFormationCommand(Command);
        break;
    case EMilitaryCommandType::Special:
        Result = ExecuteSpecialCommand(Command);
        break;
    default:
        Result.ResultDescription = TEXT("未知命令類型");
        return Result;
    }

    // 計算執行時間
    Result.ExecutionTime = FPlatformTime::Seconds() - StartTime;

    // 廣播命令執行事件
    OnCommandExecuted.Broadcast(CommandID, Result);

    // 如果成功，完成命令
    if (Result.bSuccess)
    {
        CompleteCommand(CommandID, Result.ResultDescription);
    }

    return Result;
}

bool UMingMilitaryAuthority::CancelCommand(int32 CommandID)
{
    int32 Index = FindCommandIndex(CommandID);
    if (Index < 0)
    {
        return false;
    }

    // 移動到歷史記錄
    FMilitaryCommandData Command = ActiveCommands[Index];
    Command.bIsCompleted = true;
    CommandHistory.Add(Command);

    // 限制歷史記錄大小
    if (CommandHistory.Num() > MaxHistorySize)
    {
        CommandHistory.RemoveAt(0);
    }

    // 從活動列表移除
    ActiveCommands.RemoveAt(Index);

    // 廣播取消事件
    OnCommandCancelled.Broadcast(CommandID);

    return true;
}

FMilitaryCommandData UMingMilitaryAuthority::GetCommandData(int32 CommandID) const
{
    int32 Index = FindCommandIndex(CommandID);
    if (Index >= 0)
    {
        return ActiveCommands[Index];
    }

    // 在歷史中查找
    for (const FMilitaryCommandData& Command : CommandHistory)
    {
        if (Command.CommandID == CommandID)
        {
            return Command;
        }
    }

    return FMilitaryCommandData();
}

TArray<FMilitaryCommandData> UMingMilitaryAuthority::GetActiveCommands() const
{
    return ActiveCommands;
}

TArray<FMilitaryCommandData> UMingMilitaryAuthority::GetCommandHistory() const
{
    return CommandHistory;
}

bool UMingMilitaryAuthority::SetCommandPriority(int32 CommandID, ECommandPriority NewPriority)
{
    int32 Index = FindCommandIndex(CommandID);
    if (Index < 0)
    {
        return false;
    }

    ActiveCommands[Index].Priority = NewPriority;
    return true;
}

bool UMingMilitaryAuthority::CompleteCommand(int32 CommandID, const FString& CompletionDescription)
{
    int32 Index = FindCommandIndex(CommandID);
    if (Index < 0)
    {
        return false;
    }

    // 標記為完成
    ActiveCommands[Index].bIsCompleted = true;
    ActiveCommands[Index].CommandDescription += TEXT(" [完成: ") + CompletionDescription + TEXT("]");

    // 移動到歷史記錄
    CommandHistory.Add(ActiveCommands[Index]);

    // 限制歷史記錄大小
    if (CommandHistory.Num() > MaxHistorySize)
    {
        CommandHistory.RemoveAt(0);
    }

    // 從活動列表移除
    ActiveCommands.RemoveAt(Index);

    // 廣播完成事件
    OnCommandCompleted.Broadcast(CommandID);

    return true;
}

bool UMingMilitaryAuthority::IsHealthy() const
{
    // 兵權健康狀況檢查
    // 1. 活動命令不超過上限
    // 2. 沒有過期的緊急命令

    if (ActiveCommands.Num() >= MaxActiveCommands)
    {
        return false;
    }

    FDateTime CurrentTime = FDateTime::Now();
    for (const FMilitaryCommandData& Command : ActiveCommands)
    {
        if (Command.Priority == ECommandPriority::Critical && !Command.bIsCompleted)
        {
            if (CurrentTime > Command.ExpectedCompletionTime)
            {
                return false;
            }
        }
    }

    return true;
}

FString UMingMilitaryAuthority::GetCommandTypeDescription(EMilitaryCommandType CommandType) const
{
    switch (CommandType)
    {
    case EMilitaryCommandType::Move:
        return TEXT("移動：指揮部隊移動到指定位置。");
    case EMilitaryCommandType::Attack:
        return TEXT("攻擊：命令部隊攻擊敵方目標。");
    case EMilitaryCommandType::Defend:
        return TEXT("防禦：命令部隊進入防禦態勢。");
    case EMilitaryCommandType::Retreat:
        return TEXT("撤退：命令部隊有序撤退。");
    case EMilitaryCommandType::Formation:
        return TEXT("變陣：改變部隊陣型以適應戰場。");
    case EMilitaryCommandType::Special:
        return TEXT("特殊指令：執行特殊戰術指令。");
    default:
        return TEXT("未知命令類型");
    }
}

FString UMingMilitaryAuthority::GetPriorityDescription(ECommandPriority Priority) const
{
    switch (Priority)
    {
    case ECommandPriority::Critical:
        return TEXT("緊急：必須立即執行，延遲會導致嚴重後果。");
    case ECommandPriority::High:
        return TEXT("高：優先執行，影響戰局走向。");
    case ECommandPriority::Normal:
        return TEXT("普通：正常優先級的命令。");
    case ECommandPriority::Low:
        return TEXT("低：可延後執行的輔助性命令。");
    default:
        return TEXT("未知優先級");
    }
}

int32 UMingMilitaryAuthority::GetNextCommandID()
{
    return NextCommandID++;
}

int32 UMingMilitaryAuthority::FindCommandIndex(int32 CommandID) const
{
    for (int32 i = 0; i < ActiveCommands.Num(); ++i)
    {
        if (ActiveCommands[i].CommandID == CommandID)
        {
            return i;
        }
    }
    return -1;
}

bool UMingMilitaryAuthority::ValidateCommand(const FMilitaryCommandData& CommandData) const
{
    // 基本驗證
    if (CommandData.CommandType == EMilitaryCommandType::None)
    {
        return false;
    }

    // 移動和攻擊命令需要有效目標位置
    if ((CommandData.CommandType == EMilitaryCommandType::Move || 
         CommandData.CommandType == EMilitaryCommandType::Attack) &&
        CommandData.TargetLocation.IsZero())
    {
        return false;
    }

    return true;
}

FCommandExecutionResult UMingMilitaryAuthority::ExecuteMoveCommand(const FMilitaryCommandData& Command)
{
    FCommandExecutionResult Result;
    Result.bSuccess = true;
    Result.ResultDescription = TEXT("部隊已開始向目標位置移動");
    Result.AffectedUnitCount = 1;
    return Result;
}

FCommandExecutionResult UMingMilitaryAuthority::ExecuteAttackCommand(const FMilitaryCommandData& Command)
{
    FCommandExecutionResult Result;
    Result.bSuccess = true;
    Result.ResultDescription = TEXT("部隊已開始攻擊目標");
    Result.AffectedUnitCount = 1;
    return Result;
}

FCommandExecutionResult UMingMilitaryAuthority::ExecuteDefendCommand(const FMilitaryCommandData& Command)
{
    FCommandExecutionResult Result;
    Result.bSuccess = true;
    Result.ResultDescription = TEXT("部隊已進入防禦態勢");
    Result.AffectedUnitCount = 1;
    return Result;
}

FCommandExecutionResult UMingMilitaryAuthority::ExecuteRetreatCommand(const FMilitaryCommandData& Command)
{
    FCommandExecutionResult Result;
    Result.bSuccess = true;
    Result.ResultDescription = TEXT("部隊正在有序撤退");
    Result.AffectedUnitCount = 1;
    return Result;
}

FCommandExecutionResult UMingMilitaryAuthority::ExecuteFormationCommand(const FMilitaryCommandData& Command)
{
    FCommandExecutionResult Result;
    Result.bSuccess = true;
    Result.ResultDescription = TEXT("部隊正在變換陣型");
    Result.AffectedUnitCount = 1;
    return Result;
}

FCommandExecutionResult UMingMilitaryAuthority::ExecuteSpecialCommand(const FMilitaryCommandData& Command)
{
    FCommandExecutionResult Result;
    Result.bSuccess = true;
    Result.ResultDescription = TEXT("特殊指令已執行");
    Result.AffectedUnitCount = 1;
    return Result;
}
