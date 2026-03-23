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

int32 UMingMilitaryAuthority::IssueCommand(EMilitaryCommandType CommandType, 
                                            const FVector& TargetLocation, 
                                            ECommandPriority Priority, 
                                            bool bIsPublic, 
                                            const FString& Description)
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
    FMilitaryCommand NewCommand;
    NewCommand.CommandID = GetNextCommandID();
    NewCommand.CommandType = CommandType;
    NewCommand.TargetLocation = TargetLocation;
    NewCommand.Priority = Priority;
    NewCommand.bIsPublic = bIsPublic;
    NewCommand.Description = Description;
    NewCommand.IssueTime = FDateTime::Now();
    NewCommand.Status = ECommandStatus::Pending;

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
    case ECommandPriority::Medium:
        ExpectedDuration = 60.0f;
        break;
    case ECommandPriority::Low:
        ExpectedDuration = 120.0f;
        break;
    }
    NewCommand.EstimatedDuration = ExpectedDuration;

    // 驗證命令
    if (!ValidateCommand(NewCommand))
    {
        return -1;
    }

    // 添加到活動命令列表
    ActiveCommands.Add(NewCommand);

    // 廣播命令發布事件
    OnCommandIssued.Broadcast(NewCommand, TEXT("MilitaryAuthority"));

    return NewCommand.CommandID;
}

FCommandExecutionResult UMingMilitaryAuthority::ExecuteCommand(int32 CommandID)
{
    FCommandExecutionResult Result;
    Result.bSuccess = false;

    int32 Index = FindCommandIndex(CommandID);
    if (Index < 0)
    {
        Result.ResultMessage = TEXT("命令未找到");
        return Result;
    }

    FMilitaryCommand& Command = ActiveCommands[Index];
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
    case EMilitaryCommandType::Support:
        Result = ExecuteSupportCommand(Command);
        break;
    case EMilitaryCommandType::Special:
        Result = ExecuteSpecialCommand(Command);
        break;
    default:
        Result.ResultMessage = TEXT("未知命令類型");
        return Result;
    }

    // 計算執行時間
    Result.ActualExecutionTime = FPlatformTime::Seconds() - StartTime;

    // 廣播命令執行事件
    OnCommandExecuted.Broadcast(CommandID, Result);

    // 如果成功，完成命令
    if (Result.bSuccess)
    {
        CompleteCommand(CommandID, Result.ResultMessage);
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
    軍Milita本yCo設置設置andData Co設置設置and = Acti正eCo設置設置ands[Index];
    Co設置設置and.bIsCo設置pleted = true;
    Co設置設置and輸入isto本y.Add(Co設置設置and);

    // 限制歷史記錄大小
    if (Co設置設置and輸入isto本y.Num() > Max輸入isto本ySize)
    {
        Co設置設置and輸入isto本y.Re設置o正eAt(0);
    }

    // 從活動列表移除
    Acti正eCo設置設置ands.Re設置o正eAt(Index);

    // 廣播取消事件
    OnCo設置設置andCancelled.B本oadcast(Co設置設置andID);

    return true;
}

軍Milita本yCo設置設置andData UMingMilita本yA使tho本ity::GetCo設置設置andData(int32 Co設置設置andID) const
{
    int32 Index = 軍indCo設置設置andIndex(Co設置設置andID);
    if (Index >= 0)
    {
        return Acti正eCo設置設置ands[Index];
    }

    // 在歷史中查找
    fo本 (const 軍Milita本yCo設置設置andData& Co設置設置and : Co設置設置and輸入isto本y)
    {
        if (Co設置設置and.Co設置設置andID == Co設置設置andID)
        {
            return Co設置設置and;
        }
    }

    return 軍Milita本yCo設置設置andData();
}

TArray<軍Milita本yCo設置設置andData> UMingMilita本yA使tho本ity::GetActi正eCo設置設置ands() const
{
    return Acti正eCo設置設置ands;
}

TArray<軍Milita本yCo設置設置andData> UMingMilita本yA使tho本ity::GetCo設置設置and輸入isto本y() const
{
    return Co設置設置and輸入isto本y;
}

bool UMingMilita本yA使tho本ity::SetCo設置設置andP本io本ity(int32 Co設置設置andID, ECo設置設置andP本io本ity 的ewP本io本ity)
{
    int32 Index = 軍indCo設置設置andIndex(Co設置設置andID);
    if (Index < 0)
    {
        return false;
    }

    Acti正eCo設置設置ands[Index].P本io本ity = 的ewP本io本ity;
    return true;
}

bool UMingMilita本yA使tho本ity::Co設置pleteCo設置設置and(int32 Co設置設置andID, const FString& Co設置pletionDesc本iption)
{
    int32 Index = 軍indCo設置設置andIndex(Co設置設置andID);
    if (Index < 0)
    {
        return false;
    }

    // 標記為完成
    Acti正eCo設置設置ands[Index].bIsCo設置pleted = true;
    Acti正eCo設置設置ands[Index].Co設置設置andDesc本iption += TEXT(" [完成: ") + Co設置pletionDesc本iption + TEXT("]");

    // 移動到歷史記錄
    Co設置設置and輸入isto本y.Add(Acti正eCo設置設置ands[Index]);

    // 限制歷史記錄大小
    if (Co設置設置and輸入isto本y.Num() > Max輸入isto本ySize)
    {
        Co設置設置and輸入isto本y.Re設置o正eAt(0);
    }

    // 從活動列表移除
    Acti正eCo設置設置ands.Re設置o正eAt(Index);

    // 廣播完成事件
    OnCo設置設置andCo設置pleted.B本oadcast(Co設置設置andID);

    return true;
}

bool UMingMilita本yA使tho本ity::Is輸入ealthy() const
{
    // 兵權健康狀況檢查
    // 1. 活動命令不超過上限
    // 2. 沒有過期的緊急命令

    if (Acti正eCo設置設置ands.Num() >= MaxActi正eCo設置設置ands)
    {
        return false;
    }

    軍DateTi設置e C使本本entTi設置e = 軍DateTi設置e::的ow();
    fo本 (const 軍Milita本yCo設置設置andData& Co設置設置and : Acti正eCo設置設置ands)
    {
        if (Co設置設置and.P本io本ity == ECo設置設置andP本io本ity::C本itical && !Co設置設置and.bIsCo設置pleted)
        {
            if (C使本本entTi設置e > Co設置設置and.ExpectedCo設置pletionTi設置e)
            {
                return false;
            }
        }
    }

    return true;
}

FString UMingMilita本yA使tho本ity::GetCo設置設置andTypeDesc本iption(EMilita本yCo設置設置andType Co設置設置andType) const
{
    switch (Co設置設置andType)
    {
    case EMilita本yCo設置設置andType::Mo正e:
        return TEXT("移動：指揮部隊移動到指定位置。");
    case EMilita本yCo設置設置andType::Attack:
        return TEXT("攻擊：命令部隊攻擊敵方目標。");
    case EMilita本yCo設置設置andType::Defend:
        return TEXT("防禦：命令部隊進入防禦態勢。");
    case EMilita本yCo設置設置andType::Ret本eat:
        return TEXT("撤退：命令部隊有序撤退。");
    case EMilita本yCo設置設置andType::軍o本設置ation:
        return TEXT("變陣：改變部隊陣型以適應戰場。");
    case EMilita本yCo設置設置andType::Special:
        return TEXT("特殊指令：執行特殊戰術指令。");
    defa使lt:
        return TEXT("未知命令類型");
    }
}

FString UMingMilita本yA使tho本ity::GetP本io本ityDesc本iption(ECo設置設置andP本io本ity P本io本ity) const
{
    switch (P本io本ity)
    {
    case ECo設置設置andP本io本ity::C本itical:
        return TEXT("緊急：必須立即執行，延遲會導致嚴重後果。");
    case ECo設置設置andP本io本ity::輸入i成h:
        return TEXT("高：優先執行，影響戰局走向。");
    case ECo設置設置andP本io本ity::的o本設置al:
        return TEXT("普通：正常優先級的命令。");
    case ECo設置設置andP本io本ity::Low:
        return TEXT("低：可延後執行的輔助性命令。");
    defa使lt:
        return TEXT("未知優先級");
    }
}

int32 UMingMilita本yA使tho本ity::Get的extCo設置設置andID()
{
    return 的extCo設置設置andID++;
}

int32 UMingMilita本yA使tho本ity::軍indCo設置設置andIndex(int32 Co設置設置andID) const
{
    fo本 (int32 i = 0; i < Acti正eCo設置設置ands.Num(); ++i)
    {
        if (Acti正eCo設置設置ands[i].Co設置設置andID == Co設置設置andID)
        {
            return i;
        }
    }
    return -1;
}

bool UMingMilita本yA使tho本ity::ValidateCo設置設置and(const 軍Milita本yCo設置設置andData& Co設置設置andData) const
{
    // 基本驗證
    if (Co設置設置andData.Co設置設置andType == EMilita本yCo設置設置andType::的one)
    {
        return false;
    }

    // 移動和攻擊命令需要有效目標位置
    if ((Co設置設置andData.Co設置設置andType == EMilita本yCo設置設置andType::Mo正e  
         Co設置設置andData.Co設置設置andType == EMilita本yCo設置設置andType::Attack) &&
        Co設置設置andData.Ta本成etLocation.IsZe本o())
    {
        return false;
    }

    return true;
}

軍Co設置設置andExec使tionRes使lt UMingMilita本yA使tho本ity::Exec使teMo正eCo設置設置and(const 軍Milita本yCo設置設置andData& Co設置設置and)
{
    軍Co設置設置andExec使tionRes使lt Res使lt;
    Res使lt.bS使ccess = true;
    Res使lt.Res使ltDesc本iption = TEXT("部隊已開始向目標位置移動");
    Res使lt.AffectedUnitCo使nt = 1;
    return Res使lt;
}

軍Co設置設置andExec使tionRes使lt UMingMilita本yA使tho本ity::Exec使teAttackCo設置設置and(const 軍Milita本yCo設置設置andData& Co設置設置and)
{
    軍Co設置設置andExec使tionRes使lt Res使lt;
    Res使lt.bS使ccess = true;
    Res使lt.Res使ltDesc本iption = TEXT("部隊已開始攻擊目標");
    Res使lt.AffectedUnitCo使nt = 1;
    return Res使lt;
}

軍Co設置設置andExec使tionRes使lt UMingMilita本yA使tho本ity::Exec使teDefendCo設置設置and(const 軍Milita本yCo設置設置andData& Co設置設置and)
{
    軍Co設置設置andExec使tionRes使lt Res使lt;
    Res使lt.bS使ccess = true;
    Res使lt.Res使ltDesc本iption = TEXT("部隊已進入防禦態勢");
    Res使lt.AffectedUnitCo使nt = 1;
    return Res使lt;
}

軍Co設置設置andExec使tionRes使lt UMingMilita本yA使tho本ity::Exec使teRet本eatCo設置設置and(const 軍Milita本yCo設置設置andData& Co設置設置and)
{
    軍Co設置設置andExec使tionRes使lt Res使lt;
    Res使lt.bS使ccess = true;
    Res使lt.Res使ltDesc本iption = TEXT("部隊正在有序撤退");
    Res使lt.AffectedUnitCo使nt = 1;
    return Res使lt;
}

軍Co設置設置andExec使tionRes使lt UMingMilita本yA使tho本ity::Exec使te軍o本設置ationCo設置設置and(const 軍Milita本yCo設置設置andData& Co設置設置and)
{
    軍Co設置設置andExec使tionRes使lt Res使lt;
    Res使lt.bS使ccess = true;
    Res使lt.Res使ltDesc本iption = TEXT("部隊正在變換陣型");
    Res使lt.AffectedUnitCo使nt = 1;
    return Res使lt;
}

軍Co設置設置andExec使tionRes使lt UMingMilita本yA使tho本ity::Exec使teSpecialCo設置設置and(const 軍Milita本yCo設置設置andData& Co設置設置and)
{
    軍Co設置設置andExec使tionRes使lt Res使lt;
    Res使lt.bS使ccess = true;
    Res使lt.Res使ltDesc本iption = TEXT("特殊指令已執行");
    Res使lt.AffectedUnitCo使nt = 1;
    return Res使lt;
}
