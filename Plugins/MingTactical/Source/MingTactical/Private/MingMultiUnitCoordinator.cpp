#include "MingMultiUnitCoordinator.h"
#include "MingUnitController.h"
#include "MingTacticalUnit.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"

AMingMultiUnitCoordinator::AMingMultiUnitCoordinator()
    : bIsInitialized(false)
    , UnitController(nullptr)
{
    PrimaryActorTick.bCanEverTick = true;
    
    // 預分配容量
    UnitGroups.Reserve(100);
    CoordinatedCommands.Reserve(50);
    CommunicationConnections.Reserve(100);
    CoordinationStatistics.Reserve(50);
    PerformanceMetrics.Reserve(20);
}

void AMingMultiUnitCoordinator::InitializeMultiUnitCoordinator()
{
    if (bIsInitialized)
    {
        return;
    }

    // 獲取單位控制器
    UWorld* World = GetWorld();
    if (World)
    {
        // 簡化實作：假設單位控制器已存在
        // UnitController = World->GetSubsystem<AMingUnitController>();
    }

    // 初始化統計數據
    CoordinationStatistics.Add(TEXT("total_groups"), 0);
    CoordinationStatistics.Add(TEXT("active_groups"), 0);
    CoordinationStatistics.Add(TEXT("total_commands"), 0);
    CoordinationStatistics.Add(TEXT("successful_commands"), 0);
    CoordinationStatistics.Add(TEXT("failed_commands"), 0);
    CoordinationStatistics.Add(TEXT("cancelled_commands"), 0);
    CoordinationStatistics.Add(TEXT("communication_links"), 0);
    
    // 初始化性能指標
    PerformanceMetrics.Add(TEXT("coordination_efficiency"), 0.0f);
    PerformanceMetrics.Add(TEXT("sync_accuracy"), 0.0f);
    PerformanceMetrics.Add(TEXT("communication_quality"), 0.0f);
    PerformanceMetrics.Add(TEXT("group_coherence"), 0.0f);
    PerformanceMetrics.Add(TEXT("command_response_time"), 0.0f);
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingMultiUnitCoordinator initialized"));
}

void AMingMultiUnitCoordinator::ShutdownMultiUnitCoordinator()
{
    if (!bIsInitialized)
    {
        return;
    }

    // 取消所有協調命令
    for (auto& CommandPair : CoordinatedCommands)
    {
        CancelCoordinatedCommand(CommandPair.Key);
    }
    
    // 清理所有分組
    UnitGroups.Empty();
    CoordinatedCommands.Empty();
    CommunicationConnections.Empty();
    
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("MingMultiUnitCoordinator shutdown"));
}

FUnitGroup AMingMultiUnitCoordinator::CreateUnitGroup(
    const FString& GroupName,
    EUnitGroupType GroupType,
    const TArray<AMingTacticalUnit*>& Units,
    AMingTacticalUnit* Leader)
{
    FUnitGroup Group;
    
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("MultiUnitCoordinator not initialized"));
        return Group;
    }

    // 生成分組ID
    Group.GroupID = FString::Printf(TEXT("group_%s_%d"), *GroupName, FDateTime::Now().GetMillisecond());
    Group.GroupName = GroupName;
    Group.GroupType = GroupType;
    Group.GroupUnits = Units;
    Group.GroupLeader = Leader;
    Group.GroupLevel = 1;
    Group.GroupPriority = 1;
    Group.GroupStatus = TEXT("Active");
    Group.CreationTime = FPlatformTime::Seconds();
    Group.LastUpdateTime = Group.CreationTime;
    Group.bIsActive = true;
    Group.bCanDisband = true;
    
    // 設置分組描述
    Group.GroupDescription = FString::Printf(TEXT("%s group of %d units"), *GroupName, Units.Num());
    
    // 設置分組標籤
    Group.GroupTags.Add(UEnum::GetValueAsString(GroupType));
    Group.GroupTags.Add(TEXT("active"));
    
    // 設置分組屬性
    Group.GroupAttributes.Add(TEXT("unit_count"), FString::FromInt(Units.Num()));
    Group.GroupAttributes.Add(TEXT("formation"), TEXT("none"));
    Group.GroupAttributes.Add(TEXT("coordination_capability"), TEXT("medium"));
    
    // 驗證分組配置
    if (!ValidateGroupConfig(Group))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid group configuration: %s"), *GroupName);
        return Group;
    }
    
    // 添加到分組映射
    UnitGroups.Add(Group.GroupID, Group);
    
    // 更新統計
    int32* TotalGroups = CoordinationStatistics.Find(TEXT("total_groups"));
    if (TotalGroups)
    {
        (*TotalGroups)++;
    }
    
    int32* ActiveGroups = CoordinationStatistics.Find(TEXT("active_groups"));
    if (ActiveGroups)
    {
        (*ActiveGroups)++;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Unit group created: %s (%s) with %d units"), 
        *GroupName, *UEnum::GetValueAsString(GroupType), Units.Num());
    
    return Group;
}

bool AMingMultiUnitCoordinator::DisbandUnitGroup(const FString& GroupID)
{
    if (!bIsInitialized)
    {
        return false;
    }

    FUnitGroup* Group = UnitGroups.Find(GroupID);
    if (!Group)
    {
        UE_LOG(LogTemp, Warning, TEXT("Group not found: %s"), *GroupID);
        return false;
    }
    
    if (!Group->bCanDisband)
    {
        UE_LOG(LogTemp, Warning, TEXT("Group cannot be disbanded: %s"), *GroupID);
        return false;
    }
    
    // 取消該分組的所有協調命令
    for (auto& CommandPair : CoordinatedCommands)
    {
        if (CommandPair.Value.ParticipatingGroups.Contains(GroupID))
        {
            CancelCoordinatedCommand(CommandPair.Key);
        }
    }
    
    // 斷開該分組的所有通信連接
    CommunicationConnections.Remove(GroupID);
    
    // 從分組映射中移除
    UnitGroups.Remove(GroupID);
    
    // 更新統計
    int32* ActiveGroups = CoordinationStatistics.Find(TEXT("active_groups"));
    if (ActiveGroups && *ActiveGroups > 0)
    {
        (*ActiveGroups)--;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Unit group disbanded: %s"), *GroupID);
    
    return true;
}

FUnitGroup AMingMultiUnitCoordinator::GetUnitGroup(const FString& GroupID) const
{
    const FUnitGroup* Group = UnitGroups.Find(GroupID);
    return Group ? *Group : FUnitGroup();
}

TArray<FUnitGroup> AMingMultiUnitCoordinator::GetAllUnitGroups() const
{
    TArray<FUnitGroup> Groups;
    
    for (const auto& GroupPair : UnitGroups)
    {
        Groups.Add(GroupPair.Value);
    }
    
    return Groups;
}

TArray<FUnitGroup> AMingMultiUnitCoordinator::GetGroupsByType(EUnitGroupType GroupType) const
{
    TArray<FUnitGroup> Groups;
    
    for (const auto& GroupPair : UnitGroups)
    {
        if (GroupPair.Value.GroupType == GroupType)
        {
            Groups.Add(GroupPair.Value);
        }
    }
    
    return Groups;
}

TArray<FUnitGroup> AMingMultiUnitCoordinator::GetActiveGroups() const
{
    TArray<FUnitGroup> Groups;
    
    for (const auto& GroupPair : UnitGroups)
    {
        if (GroupPair.Value.bIsActive)
        {
            Groups.Add(GroupPair.Value);
        }
    }
    
    return Groups;
}

bool AMingMultiUnitCoordinator::AddUnitToGroup(const FString& GroupID, AMingTacticalUnit* Unit)
{
    if (!bIsInitialized || !Unit)
    {
        return false;
    }
    
    FUnitGroup* Group = UnitGroups.Find(GroupID);
    if (!Group)
    {
        UE_LOG(LogTemp, Warning, TEXT("Group not found: %s"), *GroupID);
        return false;
    }
    
    if (Group->GroupUnits.Contains(Unit))
    {
        UE_LOG(LogTemp, Warning, TEXT("Unit already in group: %s"), *Unit->GetName());
        return false;
    }
    
    // 添加單位到分組
    Group->GroupUnits.Add(Unit);
    Group->LastUpdateTime = FPlatformTime::Seconds();
    
    // 更新分組屬性
    Group->GroupAttributes.Add(TEXT("unit_count"), FString::FromInt(Group->GroupUnits.Num()));
    
    UE_LOG(LogTemp, Log, TEXT("Unit %s added to group %s"), *Unit->GetName(), *GroupID);
    
    return true;
}

bool AMingMultiUnitCoordinator::RemoveUnitFromGroup(const FString& GroupID, AMingTacticalUnit* Unit)
{
    if (!bIsInitialized || !Unit)
    {
        return false;
    }
    
    FUnitGroup* Group = UnitGroups.Find(GroupID);
    if (!Group)
    {
        UE_LOG(LogTemp, Warning, TEXT("Group not found: %s"), *GroupID);
        return false;
    }
    
    if (!Group->GroupUnits.Contains(Unit))
    {
        UE_LOG(LogTemp, Warning, TEXT("Unit not in group: %s"), *Unit->GetName());
        return false;
    }
    
    // 如果是領導者，需要重新指定
    if (Group->GroupLeader == Unit)
    {
        Group->GroupLeader = Group->GroupUnits.Num() > 0 ? Group->GroupUnits[0] : nullptr;
    }
    
    // 從分組移除單位
    Group->GroupUnits.Remove(Unit);
    Group->LastUpdateTime = FPlatformTime::Seconds();
    
    // 更新分組屬性
    Group->GroupAttributes.Add(TEXT("unit_count"), FString::FromInt(Group->GroupUnits.Num()));
    
    UE_LOG(LogTemp, Log, TEXT("Unit %s removed from group %s"), *Unit->GetName(), *GroupID);
    
    return true;
}

bool AMingMultiUnitCoordinator::SetGroupLeader(const FString& GroupID, AMingTacticalUnit* Leader)
{
    if (!bIsInitialized || !Leader)
    {
        return false;
    }
    
    FUnitGroup* Group = UnitGroups.Find(GroupID);
    if (!Group)
    {
        UE_LOG(LogTemp, Warning, TEXT("Group not found: %s"), *GroupID);
        return false;
    }
    
    if (!Group->GroupUnits.Contains(Leader))
    {
        UE_LOG(LogTemp, Warning, TEXT("Leader not in group: %s"), *Leader->GetName());
        return false;
    }
    
    Group->GroupLeader = Leader;
    Group->LastUpdateTime = FPlatformTime::Seconds();
    
    UE_LOG(LogTemp, Log, TEXT("Group leader set: %s for group %s"), *Leader->GetName(), *GroupID);
    
    return true;
}

FCoordinationResult AMingMultiUnitCoordinator::IssueCoordinatedCommand(const FCoordinatedCommand& Command)
{
    FCoordinationResult Result;
    Result.ExecutionTime = FPlatformTime::Seconds();
    Result.CoordinationID = FString::Printf(TEXT("coord_%s"), *Command.CommandID);
    Result.CommandID = Command.CommandID;
    
    if (!bIsInitialized)
    {
        Result.ErrorMessage = TEXT("MultiUnitCoordinator not initialized");
        return Result;
    }

    if (!ValidateCoordinatedCommand(Command))
    {
        Result.ErrorMessage = TEXT("Invalid coordinated command");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("Issuing coordinated command: %s"), *GetCommandDescription(Command.CommandID));

    // 添加到協調命令映射
    FCoordinatedCommand ModifiedCommand = Command;
    ModifiedCommand.StartTime = FPlatformTime::Seconds();
    ModifiedCommand.bIsExecuted = true;
    CoordinatedCommands.Add(ModifiedCommand.CommandID, ModifiedCommand);
    
    // 設置參與分組和單位
    for (const FString& GroupID : Command.ParticipatingGroups)
    {
        FUnitGroup* Group = UnitGroups.Find(GroupID);
        if (Group)
        {
            Result.ParticipatingGroups.Add(GroupID);
            Result.ParticipatingUnits.Append(Group->GroupUnits);
        }
    }
    
    // 執行協調命令
    ExecuteCoordinatedCommand(ModifiedCommand);
    
    // 更新統計
    int32* TotalCommands = CoordinationStatistics.Find(TEXT("total_commands"));
    if (TotalCommands)
    {
        (*TotalCommands)++;
    }
    
    // 計算協調分數
    Result.CoordinationScore = CalculateCoordinationScore(Result);
    
    UE_LOG(LogTemp, Log, TEXT("Coordinated command issued: %s - Score: %.2f"), 
        *GetCommandDescription(Command.CommandID), Result.CoordinationScore);
    
    return Result;
}

FCoordinationResult AMingMultiUnitCoordinator::ExecuteCoordinatedAttack(
    const TArray<FString>& GroupIDs,
    const TArray<AMingTacticalUnit*>& Targets,
    ECoordinationSyncType SyncType)
{
    FCoordinatedCommand Command;
    Command.CommandID = FString::Printf(TEXT("attack_%d"), FDateTime::Now().GetMillisecond());
    Command.CommandName = TEXT("Coordinated Attack");
    Command.CommandType = ECoordinatedCommandType::CoordinatedAttack;
    Command.CommandDescription = FString::Printf(TEXT("Coordinated attack with %d groups"), GroupIDs.Num());
    Command.ParticipatingGroups = GroupIDs;
    Command.SyncType = SyncType;
    Command.CommunicationProtocol = GetOptimalCommunicationProtocol(GroupIDs);
    Command.CommandStatus = TEXT("Executing");
    
    // 設置命令參數
    Command.CommandParameters.Add(TEXT("target_count"), FString::FromInt(Targets.Num()));
    Command.CommandParameters.Add(TEXT("sync_type"), UEnum::GetValueAsString(SyncType));
    
    // 設置執行時間表
    for (int32 i = 0; i < GroupIDs.Num(); ++i)
    {
        FString PhaseKey = FString::Printf(TEXT("phase_%d"), i);
        Command.ExecutionSchedule.Add(PhaseKey, FPlatformTime::Seconds() + i * 0.5f);
    }
    
    return IssueCoordinatedCommand(Command);
}

FCoordinationResult AMingMultiUnitCoordinator::ExecuteCoordinatedDefense(
    const TArray<FString>& GroupIDs,
    const TArray<FVector>& DefensePositions,
    ECoordinationSyncType SyncType)
{
    FCoordinatedCommand Command;
    Command.CommandID = FString::Printf(TEXT("defense_%d"), FDateTime::Now().GetMillisecond());
    Command.CommandName = TEXT("Coordinated Defense");
    Command.CommandType = ECoordinatedCommandType::CoordinatedDefense;
    Command.CommandDescription = FString::Printf(TEXT("Coordinated defense with %d groups"), GroupIDs.Num());
    Command.ParticipatingGroups = GroupIDs;
    Command.SyncType = SyncType;
    Command.CommunicationProtocol = GetOptimalCommunicationProtocol(GroupIDs);
    Command.CommandStatus = TEXT("Executing");
    
    // 設置命令參數
    Command.CommandParameters.Add(TEXT("position_count"), FString::FromInt(DefensePositions.Num()));
    Command.CommandParameters.Add(TEXT("sync_type"), UEnum::GetValueAsString(SyncType));
    
    // 設置防禦位置
    for (int32 i = 0; i < DefensePositions.Num(); ++i)
    {
        FString PositionKey = FString::Printf(TEXT("position_%d"), i);
        Command.CommandParameters.Add(PositionKey, DefensePositions[i].ToString());
    }
    
    return IssueCoordinatedCommand(Command);
}

FCoordinationResult AMingMultiUnitCoordinator::ExecuteCoordinatedMovement(
    const TArray<FString>& GroupIDs,
    const TArray<FVector>& TargetPositions,
    ECoordinationSyncType SyncType)
{
    FCoordinatedCommand Command;
    Command.CommandID = FString::Printf(TEXT("movement_%d"), FDateTime::Now().GetMillisecond());
    Command.CommandName = TEXT("Coordinated Movement");
    Command.CommandType = ECoordinatedCommandType::CoordinatedMovement;
    Command.CommandDescription = FString::Printf(TEXT("Coordinated movement with %d groups"), GroupIDs.Num());
    Command.ParticipatingGroups = GroupIDs;
    Command.SyncType = SyncType;
    Command.CommunicationProtocol = GetOptimalCommunicationProtocol(GroupIDs);
    Command.CommandStatus = TEXT("Executing");
    
    // 設置命令參數
    Command.CommandParameters.Add(TEXT("position_count"), FString::FromInt(TargetPositions.Num()));
    Command.CommandParameters.Add(TEXT("sync_type"), UEnum::GetValueAsString(SyncType));
    
    // 設置目標位置
    for (int32 i = 0; i < TargetPositions.Num(); ++i)
    {
        FString PositionKey = FString::Printf(TEXT("target_%d"), i);
        Command.CommandParameters.Add(PositionKey, TargetPositions[i].ToString());
    }
    
    return IssueCoordinatedCommand(Command);
}

FCoordinationResult AMingMultiUnitCoordinator::ExecuteCoordinatedRetreat(
    const TArray<FString>& GroupIDs,
    const TArray<FVector>& RetreatPositions,
    ECoordinationSyncType SyncType)
{
    FCoordinatedCommand Command;
    Command.CommandID = FString::Printf(TEXT("retreat_%d"), FDateTime::Now().GetMillisecond());
    Command.CommandName = TEXT("Coordinated Retreat");
    Command.CommandType = ECoordinatedCommandType::CoordinatedRetreat;
    Command.CommandDescription = FString::Printf(TEXT("Coordinated retreat with %d groups"), GroupIDs.Num());
    Command.ParticipatingGroups = GroupIDs;
    Command.SyncType = SyncType;
    Command.CommunicationProtocol = GetOptimalCommunicationProtocol(GroupIDs);
    Command.CommandStatus = TEXT("Executing");
    
    // 設置命令參數
    Command.CommandParameters.Add(TEXT("position_count"), FString::FromInt(RetreatPositions.Num()));
    Command.CommandParameters.Add(TEXT("sync_type"), UEnum::GetValueAsString(SyncType));
    Command.CommandParameters.Add(TEXT("urgency"), TEXT("high"));
    
    // 設置撤退位置
    for (int32 i = 0; i < RetreatPositions.Num(); ++i)
    {
        FString PositionKey = FString::Printf(TEXT("retreat_%d"), i);
        Command.CommandParameters.Add(PositionKey, RetreatPositions[i].ToString());
    }
    
    return IssueCoordinatedCommand(Command);
}

FCoordinationResult AMingMultiUnitCoordinator::ExecuteCoordinatedSupport(
    const TArray<FString>& SupportGroupIDs,
    const TArray<FString>& TargetGroupIDs,
    ECoordinationSyncType SyncType)
{
    FCoordinatedCommand Command;
    Command.CommandID = FString::Printf(TEXT("support_%d"), FDateTime::Now().GetMillisecond());
    Command.CommandName = TEXT("Coordinated Support");
    Command.CommandType = ECoordinatedCommandType::CoordinatedSupport;
    Command.CommandDescription = FString::Printf(TEXT("Coordinated support from %d groups to %d targets"), 
        SupportGroupIDs.Num(), TargetGroupIDs.Num());
    Command.ParticipatingGroups = SupportGroupIDs;
    Command.SyncType = SyncType;
    Command.CommunicationProtocol = GetOptimalCommunicationProtocol(SupportGroupIDs);
    Command.CommandStatus = TEXT("Executing");
    
    // 設置命令參數
    Command.CommandParameters.Add(TEXT("support_groups"), FString::FromInt(SupportGroupIDs.Num()));
    Command.CommandParameters.Add(TEXT("target_groups"), FString::FromInt(TargetGroupIDs.Num()));
    Command.CommandParameters.Add(TEXT("sync_type"), UEnum::GetValueAsString(SyncType));
    
    // 設置目標分組
    for (int32 i = 0; i < TargetGroupIDs.Num(); ++i)
    {
        FString TargetKey = FString::Printf(TEXT("target_%d"), i);
        Command.CommandParameters.Add(TargetKey, TargetGroupIDs[i]);
    }
    
    return IssueCoordinatedCommand(Command);
}

FCoordinationResult AMingMultiUnitCoordinator::ExecuteCoordinatedAmbush(
    const TArray<FString>& AmbusherGroupIDs,
    const TArray<AMingTacticalUnit*>& Targets,
    ECoordinationSyncType SyncType)
{
    FCoordinatedCommand Command;
    Command.CommandID = FString::Printf(TEXT("ambush_%d"), FDateTime::Now().GetMillisecond());
    Command.CommandName = TEXT("Coordinated Ambush");
    Command.CommandType = ECoordinatedCommandType::CoordinatedAmbush;
    Command.CommandDescription = FString::Printf(TEXT("Coordinated ambush with %d groups"), AmbusherGroupIDs.Num());
    Command.ParticipatingGroups = AmbusherGroupIDs;
    Command.SyncType = SyncType;
    Command.CommunicationProtocol = ECommunicationProtocol::Secure;
    Command.CommandStatus = TEXT("Executing");
    
    // 設置命令參數
    Command.CommandParameters.Add(TEXT("target_count"), FString::FromInt(Targets.Num()));
    Command.CommandParameters.Add(TEXT("sync_type"), UEnum::GetValueAsString(SyncType));
    Command.CommandParameters.Add(TEXT("stealth"), TEXT("high"));
    
    return IssueCoordinatedCommand(Command);
}

FCoordinationResult AMingMultiUnitCoordinator::ExecuteCoordinatedFlanking(
    const TArray<FString>& FlankerGroupIDs,
    const TArray<AMingTacticalUnit*>& Targets,
    ECoordinationSyncType SyncType)
{
    FCoordinatedCommand Command;
    Command.CommandID = FString::Printf(TEXT("flanking_%d"), FDateTime::Now().GetMillisecond());
    Command.CommandName = TEXT("Coordinated Flanking");
    Command.CommandType = ECoordinatedCommandType::CoordinatedFlanking;
    Command.CommandDescription = FString::Printf(TEXT("Coordinated flanking with %d groups"), FlankerGroupIDs.Num());
    Command.ParticipatingGroups = FlankerGroupIDs;
    Command.SyncType = SyncType;
    Command.CommunicationProtocol = GetOptimalCommunicationProtocol(FlankerGroupIDs);
    Command.CommandStatus = TEXT("Executing");
    
    // 設置命令參數
    Command.CommandParameters.Add(TEXT("target_count"), FString::FromInt(Targets.Num()));
    Command.CommandParameters.Add(TEXT("sync_type"), UEnum::GetValueAsString(SyncType));
    Command.CommandParameters.Add(TEXT("maneuver"), TEXT("flanking"));
    
    return IssueCoordinatedCommand(Command);
}

FCoordinationResult AMingMultiUnitCoordinator::ExecuteCoordinatedEncirclement(
    const TArray<FString>& EncirclerGroupIDs,
    const TArray<AMingTacticalUnit*>& Targets,
    ECoordinationSyncType SyncType)
{
    FCoordinatedCommand Command;
    Command.CommandID = FString::Printf(TEXT("encirclement_%d"), FDateTime::Now().GetMillisecond());
    Command.CommandName = TEXT("Coordinated Encirclement");
    Command.CommandType = ECoordinatedCommandType::CoordinatedEncirclement;
    Command.CommandDescription = FString::Printf(TEXT("Coordinated encirclement with %d groups"), EncirclerGroupIDs.Num());
    Command.ParticipatingGroups = EncirclerGroupIDs;
    Command.SyncType = SyncType;
    Command.CommunicationProtocol = GetOptimalCommunicationProtocol(EncirclerGroupIDs);
    Command.CommandStatus = TEXT("Executing");
    
    // 設置命令參數
    Command.CommandParameters.Add(TEXT("target_count"), FString::FromInt(Targets.Num()));
    Command.CommandParameters.Add(TEXT("sync_type"), UEnum::GetValueAsString(SyncType));
    Command.CommandParameters.Add(TEXT("tactic"), TEXT("encirclement"));
    
    return IssueCoordinatedCommand(Command);
}

FCoordinationResult AMingMultiUnitCoordinator::ExecuteCoordinatedBreakthrough(
    const TArray<FString>& BreakthroughGroupIDs,
    const TArray<FVector>& BreakthroughPoints,
    ECoordinationSyncType SyncType)
{
    FCoordinatedCommand Command;
    Command.CommandID = FString::Printf(TEXT("breakthrough_%d"), FDateTime::Now().GetMillisecond());
    Command.CommandName = TEXT("Coordinated Breakthrough");
    Command.CommandType = ECoordinatedCommandType::CoordinatedBreakthrough;
    Command.CommandDescription = FString::Printf(TEXT("Coordinated breakthrough with %d groups"), BreakthroughGroupIDs.Num());
    Command.ParticipatingGroups = BreakthroughGroupIDs;
    Command.SyncType = SyncType;
    Command.CommunicationProtocol = GetOptimalCommunicationProtocol(BreakthroughGroupIDs);
    Command.CommandStatus = TEXT("Executing");
    
    // 設置命令參數
    Command.CommandParameters.Add(TEXT("point_count"), FString::FromInt(BreakthroughPoints.Num()));
    Command.CommandParameters.Add(TEXT("sync_type"), UEnum::GetValueAsString(SyncType));
    Command.CommandParameters.Add(TEXT("urgency"), TEXT("critical"));
    
    // 設置突破點
    for (int32 i = 0; i < BreakthroughPoints.Num(); ++i)
    {
        FString PointKey = FString::Printf(TEXT("point_%d"), i);
        Command.CommandParameters.Add(PointKey, BreakthroughPoints[i].ToString());
    }
    
    return IssueCoordinatedCommand(Command);
}

bool AMingMultiUnitCoordinator::CancelCoordinatedCommand(const FString& CommandID)
{
    FCoordinatedCommand* Command = CoordinatedCommands.Find(CommandID);
    if (!Command)
    {
        UE_LOG(LogTemp, Warning, TEXT("Coordinated command not found: %s"), *CommandID);
        return false;
    }
    
    Command->bIsCancelled = true;
    Command->CommandStatus = TEXT("Cancelled");
    Command->CompletionTime = FPlatformTime::Seconds();
    
    // 更新統計
    int32* CancelledCommands = CoordinationStatistics.Find(TEXT("cancelled_commands"));
    if (CancelledCommands)
    {
        (*CancelledCommands)++;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Coordinated command cancelled: %s"), *CommandID);
    
    return true;
}

FCoordinatedCommand AMingMultiUnitCoordinator::GetCoordinatedCommand(const FString& CommandID) const
{
    const FCoordinatedCommand* Command = CoordinatedCommands.Find(CommandID);
    return Command ? *Command : FCoordinatedCommand();
}

TArray<FCoordinatedCommand> AMingMultiUnitCoordinator::GetAllCoordinatedCommands() const
{
    TArray<FCoordinatedCommand> Commands;
    
    for (const auto& CommandPair : CoordinatedCommands)
    {
        Commands.Add(CommandPair.Value);
    }
    
    return Commands;
}

TArray<FCoordinatedCommand> AMingMultiUnitCoordinator::GetExecutingCommands() const
{
    TArray<FCoordinatedCommand> Commands;
    
    for (const auto& CommandPair : CoordinatedCommands)
    {
        if (CommandPair.Value.bIsExecuted && !CommandPair.Value.bIsCompleted && !CommandPair.Value.bIsCancelled)
        {
            Commands.Add(CommandPair.Value);
        }
    }
    
    return Commands;
}

FString AMingMultiUnitCoordinator::GetCoordinationStatus(const FString& CommandID) const
{
    const FCoordinatedCommand* Command = CoordinatedCommands.Find(CommandID);
    return Command ? Command->CommandStatus : TEXT("Not Found");
}

TMap<FString, int32> AMingMultiUnitCoordinator::GetCoordinationStatistics() const
{
    return CoordinationStatistics;
}

TMap<FString, float> AMingMultiUnitCoordinator::GetCoordinationPerformanceMetrics() const
{
    return PerformanceMetrics;
}

bool AMingMultiUnitCoordinator::SynchronizeUnitStates(const TArray<AMingTacticalUnit*>& Units)
{
    if (!bIsInitialized || Units.Num() == 0)
    {
        return false;
    }
    
    // 簡化實作：記錄同步操作
    UE_LOG(LogTemp, Log, TEXT("Synchronizing states for %d units"), Units.Num());
    
    // 這裡應該實現實際的狀態同步邏輯
    // 例如：同步位置、狀態、目標等
    
    return true;
}

bool AMingMultiUnitCoordinator::BroadcastToGroup(const FString& GroupID, const FString& Message)
{
    if (!bIsInitialized)
    {
        return false;
    }
    
    FUnitGroup* Group = UnitGroups.Find(GroupID);
    if (!Group)
    {
        UE_LOG(LogTemp, Warning, TEXT("Group not found: %s"), *GroupID);
        return false;
    }
    
    // 廣播消息到分組中的所有單位
    for (AMingTacticalUnit* Unit : Group->GroupUnits)
    {
        if (Unit)
        {
            // 簡化實作：記錄消息
            UE_LOG(LogTemp, Log, TEXT("Broadcasting to unit %s: %s"), *Unit->GetName(), *Message);
        }
    }
    
    return true;
}

bool AMingMultiUnitCoordinator::RelayMessage(const FString& SourceGroupID, const FString& TargetGroupID, const FString& Message)
{
    if (!bIsInitialized)
    {
        return false;
    }
    
    FUnitGroup* SourceGroup = UnitGroups.Find(SourceGroupID);
    FUnitGroup* TargetGroup = UnitGroups.Find(TargetGroupID);
    
    if (!SourceGroup || !TargetGroup)
    {
        UE_LOG(LogTemp, Warning, TEXT("Source or target group not found"));
        return false;
    }
    
    // 中繼消息
    UE_LOG(LogTemp, Log, TEXT("Relaying message from %s to %s: %s"), 
        *SourceGroup->GroupName, *TargetGroup->GroupName, *Message);
    
    // 處理通信延遲
    float Delay = HandleCommunicationDelay(SourceGroupID, TargetGroupID);
    
    // 處理通信干擾
    float Interference = HandleCommunicationInterference(SourceGroupID, TargetGroupID);
    
    // 處理通信加密
    bool bEncrypted = HandleCommunicationEncryption(SourceGroupID, TargetGroupID);
    
    // 廣播到目標分組
    return BroadcastToGroup(TargetGroupID, Message);
}

bool AMingMultiUnitCoordinator::EstablishCommunication(const FString& GroupAID, const FString& GroupBID, ECommunicationProtocol Protocol)
{
    if (!bIsInitialized)
    {
        return false;
    }
    
    FUnitGroup* GroupA = UnitGroups.Find(GroupAID);
    FUnitGroup* GroupB = UnitGroups.Find(GroupBID);
    
    if (!GroupA || !GroupB)
    {
        UE_LOG(LogTemp, Warning, TEXT("One or both groups not found"));
        return false;
    }
    
    // 建立通信連接
    TMap<FString, ECommunicationProtocol>* Connections = CommunicationConnections.Find(GroupAID);
    if (!Connections)
    {
        TMap<FString, ECommunicationProtocol> NewConnections;
        CommunicationConnections.Add(GroupAID, NewConnections);
        Connections = CommunicationConnections.Find(GroupAID);
    }
    
    Connections->Add(GroupBID, Protocol);
    
    // 雙向連接
    TMap<FString, ECommunicationProtocol>* ReverseConnections = CommunicationConnections.Find(GroupBID);
    if (!ReverseConnections)
    {
        TMap<FString, ECommunicationProtocol> NewConnections;
        CommunicationConnections.Add(GroupBID, NewConnections);
        ReverseConnections = CommunicationConnections.Find(GroupBID);
    }
    
    ReverseConnections->Add(GroupAID, Protocol);
    
    // 更新統計
    int32* CommunicationLinks = CoordinationStatistics.Find(TEXT("communication_links"));
    if (CommunicationLinks)
    {
        (*CommunicationLinks)++;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Communication established between %s and %s using %s"), 
        *GroupA->GroupName, *GroupB->GroupName, *GetProtocolDescription(Protocol));
    
    return true;
}

bool AMingMultiUnitCoordinator::DisconnectCommunication(const FString& GroupAID, const FString& GroupBID)
{
    if (!bIsInitialized)
    {
        return false;
    }
    
    TMap<FString, ECommunicationProtocol>* ConnectionsA = CommunicationConnections.Find(GroupAID);
    TMap<FString, ECommunicationProtocol>* ConnectionsB = CommunicationConnections.Find(GroupBID);
    
    if (ConnectionsA)
    {
        ConnectionsA->Remove(GroupBID);
    }
    
    if (ConnectionsB)
    {
        ConnectionsB->Remove(GroupAID);
    }
    
    // 更新統計
    int32* CommunicationLinks = CoordinationStatistics.Find(TEXT("communication_links"));
    if (CommunicationLinks && *CommunicationLinks > 0)
    {
        (*CommunicationLinks)--;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Communication disconnected between %s and %s"), *GroupAID, *GroupBID);
    
    return true;
}

float AMingMultiUnitCoordinator::GetCommunicationQuality(const FString& GroupAID, const FString& GroupBID) const
{
    const TMap<FString, ECommunicationProtocol>* Connections = CommunicationConnections.Find(GroupAID);
    if (!Connections)
    {
        return 0.0f;
    }
    
    const ECommunicationProtocol* Protocol = Connections->Find(GroupBID);
    if (!Protocol)
    {
        return 0.0f;
    }
    
    // 基於協議類型計算通信質量
    switch (*Protocol)
    {
    case ECommunicationProtocol::Direct:
    case ECommunicationProtocol::PeerToPeer:
        return 0.9f;
    case ECommunicationProtocol::Relay:
    case ECommunicationProtocol::Multicast:
        return 0.8f;
    case ECommunicationProtocol::Broadcast:
    case ECommunicationProtocol::Centralized:
        return 0.7f;
    case ECommunicationProtocol::Hierarchical:
    case ECommunicationProtocol::Decentralized:
        return 0.6f;
    case ECommunicationProtocol::Hybrid:
        return 0.75f;
    case ECommunicationProtocol::Secure:
    case ECommunicationProtocol::Encrypted:
        return 0.85f;
    case ECommunicationProtocol::Emergency:
        return 0.95f;
    default:
        return 0.5f;
    }
}

// === 內部方法實現 ===

void AMingMultiUnitCoordinator::ExecuteCoordinatedCommand(const FCoordinatedCommand& Command)
{
    UE_LOG(LogTemp, Log, TEXT("Executing coordinated command: %s"), *GetCommandDescription(Command.CommandID));
    
    // 根據同步類型執行
    switch (Command.SyncType)
    {
    case ECoordinationSyncType::Simultaneous:
        HandleSimultaneousExecution(Command);
        break;
    case ECoordinationSyncType::Sequential:
        HandleSequentialExecution(Command);
        break;
    case ECoordinationSyncType::Staggered:
        HandleStaggeredExecution(Command);
        break;
    case ECoordinationSyncType::Wave:
        HandleWaveExecution(Command);
        break;
    case ECoordinationSyncType::Phased:
        HandlePhasedExecution(Command);
        break;
    case ECoordinationSyncType::Conditional:
        HandleConditionalExecution(Command);
        break;
    case ECoordinationSyncType::EventDriven:
        HandleEventDrivenExecution(Command);
        break;
    case ECoordinationSyncType::TimeBased:
        HandleTimeBasedExecution(Command);
        break;
    case ECoordinationSyncType::PriorityBased:
        HandlePriorityBasedExecution(Command);
        break;
    case ECoordinationSyncType::Adaptive:
        HandleAdaptiveExecution(Command);
        break;
    case ECoordinationSyncType::Reactive:
        HandleReactiveExecution(Command);
        break;
    case ECoordinationSyncType::Proactive:
        HandleProactiveExecution(Command);
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown sync type: %d"), static_cast<int32>(Command.SyncType));
        break;
    }
}

void AMingMultiUnitCoordinator::HandleSimultaneousExecution(const FCoordinatedCommand& Command)
{
    // 同時執行所有分組
    for (const FString& GroupID : Command.ParticipatingGroups)
    {
        FUnitGroup* Group = UnitGroups.Find(GroupID);
        if (Group && Group->bIsActive)
        {
            // 執行分組命令
            ExecuteGroupCommand(GroupID, Command);
        }
    }
}

void AMingMultiUnitCoordinator::HandleSequentialExecution(const FCoordinatedCommand& Command)
{
    // 順序執行分組
    for (int32 i = 0; i < Command.ParticipatingGroups.Num(); ++i)
    {
        const FString& GroupID = Command.ParticipatingGroups[i];
        FUnitGroup* Group = UnitGroups.Find(GroupID);
        
        if (Group && Group->bIsActive)
        {
            // 執行分組命令
            ExecuteGroupCommand(GroupID, Command);
            
            // 等待完成（簡化實作）
            float WaitTime = 1.0f; // 1秒等待時間
            // 這裡應該實現實際的等待邏輯
        }
    }
}

void AMingMultiUnitCoordinator::HandleStaggeredExecution(const FCoordinatedCommand& Command)
{
    // 交錯執行分組
    for (int32 i = 0; i < Command.ParticipatingGroups.Num(); ++i)
    {
        const FString& GroupID = Command.ParticipatingGroups[i];
        FUnitGroup* Group = UnitGroups.Find(GroupID);
        
        if (Group && Group->bIsActive)
        {
            // 延迟執行
            float Delay = i * 0.5f; // 每個分組延迟0.5秒
            // 這裡應該實現實際的延迟邏輯
            
            // 執行分組命令
            ExecuteGroupCommand(GroupID, Command);
        }
    }
}

void AMingMultiUnitCoordinator::HandleWaveExecution(const FCoordinatedCommand& Command)
{
    // 波浪執行分組
    int32 WaveSize = FMath::CeilToFloat(Command.ParticipatingGroups.Num() / 3.0f); // 分成3波
    
    for (int32 Wave = 0; Wave < 3; ++Wave)
    {
        for (int32 i = Wave * WaveSize; i < FMath::Min((Wave + 1) * WaveSize, Command.ParticipatingGroups.Num()); ++i)
        {
            const FString& GroupID = Command.ParticipatingGroups[i];
            FUnitGroup* Group = UnitGroups.Find(GroupID);
            
            if (Group && Group->bIsActive)
            {
                // 執行分組命令
                ExecuteGroupCommand(GroupID, Command);
            }
        }
        
        // 波浪間延迟
        if (Wave < 2)
        {
            float WaveDelay = 2.0f; // 波浪間延迟2秒
            // 這裡應該實現實際的延迟邏輯
        }
    }
}

void AMingMultiUnitCoordinator::HandlePhasedExecution(const FCoordinatedCommand& Command)
{
    // 階段執行分組
    for (const FString& Phase : Command.CommandPhases)
    {
        // 獲取該階段的分組
        TArray<FString> PhaseGroups;
        for (const FString& GroupID : Command.ParticipatingGroups)
        {
            // 簡化實作：基於階段名稱確定分組
            if (Phase.Contains(TEXT("preparation")) && GroupID.Contains(TEXT("support")))
            {
                PhaseGroups.Add(GroupID);
            }
            else if (Phase.Contains(TEXT("execution")) && GroupID.Contains(TEXT("attack")))
            {
                PhaseGroups.Add(GroupID);
            }
            else if (Phase.Contains(TEXT("completion")) && GroupID.Contains(TEXT("logistics")))
            {
                PhaseGroups.Add(GroupID);
            }
        }
        
        // 執行階段分組
        for (const FString& GroupID : PhaseGroups)
        {
            ExecuteGroupCommand(GroupID, Command);
        }
        
        // 階段間延迟
        float PhaseDelay = 1.5f; // 階段間延迟1.5秒
        // 這裡應該實現實際的延迟邏輯
    }
}

void AMingMultiUnitCoordinator::HandleConditionalExecution(const FCoordinatedCommand& Command)
{
    // 條件執行分組
    for (const FString& GroupID : Command.ParticipatingGroups)
    {
        FUnitGroup* Group = UnitGroups.Find(GroupID);
        
        if (Group && Group->bIsActive)
        {
            // 檢查執行條件
            if (CheckExecutionConditions(GroupID, Command))
            {
                ExecuteGroupCommand(GroupID, Command);
            }
        }
    }
}

void AMingMultiUnitCoordinator::HandleEventDrivenExecution(const FCoordinatedCommand& Command)
{
    // 事件驅動執行
    // 簡化實作：等待特定事件
    for (const FString& GroupID : Command.ParticipatingGroups)
    {
        FUnitGroup* Group = UnitGroups.Find(GroupID);
        
        if (Group && Group->bIsActive)
        {
            // 檢查事件條件
            if (CheckEventConditions(GroupID, Command))
            {
                ExecuteGroupCommand(GroupID, Command);
            }
        }
    }
}

void AMingMultiUnitCoordinator::HandleTimeBasedExecution(const FCoordinatedCommand& Command)
{
    // 基於時間的執行
    float CurrentTime = FPlatformTime::Seconds();
    
    for (const auto& SchedulePair : Command.ExecutionSchedule)
    {
        if (CurrentTime >= SchedulePair.Value)
        {
            // 執行對應的分組
            ExecuteGroupCommand(SchedulePair.Key, Command);
        }
    }
}

void AMingMultiUnitCoordinator::HandlePriorityBasedExecution(const FCoordinatedCommand& Command)
{
    // 基於優先級的執行
    TArray<FString> SortedGroups = Command.ParticipatingGroups;
    
    // 按分組優先級排序
    SortedGroups.Sort([this](const FString& GroupA, const FString& GroupB)
    {
        const FUnitGroup* GroupAPtr = UnitGroups.Find(GroupA);
        const FUnitGroup* GroupBPtr = UnitGroups.Find(GroupB);
        
        if (GroupAPtr && GroupBPtr)
        {
            return GroupAPtr->GroupPriority > GroupBPtr->GroupPriority;
        }
        
        return false;
    });
    
    // 按優先級執行
    for (const FString& GroupID : SortedGroups)
    {
        ExecuteGroupCommand(GroupID, Command);
    }
}

void AMingMultiUnitCoordinator::HandleAdaptiveExecution(const FCoordinatedCommand& Command)
{
    // 自適應執行
    for (const FString& GroupID : Command.ParticipatingGroups)
    {
        FUnitGroup* Group = UnitGroups.Find(GroupID);
        
        if (Group && Group->bIsActive)
        {
            // 根據分組能力調整執行方式
            float CoordinationCapability = CalculateGroupCoordinationCapability(GroupID);
            
            if (CoordinationCapability >= 0.8f)
            {
                // 高協調能力：同時執行
                ExecuteGroupCommand(GroupID, Command);
            }
            else if (CoordinationCapability >= 0.5f)
            {
                // 中等協調能力：延迟執行
                float Delay = 0.5f;
                // 這裡應該實現實際的延迟邏輯
                ExecuteGroupCommand(GroupID, Command);
            }
            else
            {
                // 低協調能力：單獨執行
                ExecuteGroupCommand(GroupID, Command);
            }
        }
    }
}

void AMingMultiUnitCoordinator::HandleReactiveExecution(const FCoordinatedCommand& Command)
{
    // 反應式執行
    for (const FString& GroupID : Command.ParticipatingGroups)
    {
        FUnitGroup* Group = UnitGroups.Find(GroupID);
        
        if (Group && Group->bIsActive)
        {
            // 檢查是否需要反應
            if (CheckReactiveConditions(GroupID, Command))
            {
                ExecuteGroupCommand(GroupID, Command);
            }
        }
    }
}

void AMingMultiUnitCoordinator::HandleProactiveExecution(const FCoordinatedCommand& Command)
{
    // 主動式執行
    for (const FString& GroupID : Command.ParticipatingGroups)
    {
        FUnitGroup* Group = UnitGroups.Find(GroupID);
        
        if (Group && Group->bIsActive)
        {
            // 主動預測和執行
            if (CheckProactiveConditions(GroupID, Command))
            {
                ExecuteGroupCommand(GroupID, Command);
            }
        }
    }
}

bool AMingMultiUnitCoordinator::CheckSyncConditions(const FCoordinatedCommand& Command) const
{
    // 檢查同步條件
    for (const FString& GroupID : Command.ParticipatingGroups)
    {
        const FUnitGroup* Group = UnitGroups.Find(GroupID);
        if (!Group || !Group->bIsActive)
        {
            return false;
        }
    }
    
    return true;
}

float AMingMultiUnitCoordinator::CalculateSyncAccuracy(const FCoordinatedCommand& Command) const
{
    float Accuracy = 0.0f;
    int32 ActiveGroups = 0;
    
    for (const FString& GroupID : Command.ParticipatingGroups)
    {
        const FUnitGroup* Group = UnitGroups.Find(GroupID);
        if (Group && Group->bIsActive)
        {
            Accuracy += CalculateGroupCoordinationCapability(GroupID);
            ActiveGroups++;
        }
    }
    
    return ActiveGroups > 0 ? Accuracy / ActiveGroups : 0.0f;
}

float AMingMultiUnitCoordinator::CalculateCommunicationQuality(const FString& GroupAID, const FString& GroupBID) const
{
    float BaseQuality = GetCommunicationQuality(GroupAID, GroupBID);
    
    // 考慮距離因素
    const FUnitGroup* GroupA = UnitGroups.Find(GroupAID);
    const FUnitGroup* GroupB = UnitGroups.Find(GroupBID);
    
    if (GroupA && GroupB)
    {
        // 簡化實作：基於分組類型調整質量
        if (GroupA->GroupType == EUnitGroupType::SpecialForces || GroupB->GroupType == EUnitGroupType::SpecialForces)
        {
            BaseQuality *= 1.1f; // 特種部隊有更好的通信
        }
        
        if (GroupA->GroupType == EUnitGroupType::Logistics || GroupB->GroupType == EUnitGroupType::Logistics)
        {
            BaseQuality *= 0.9f; // 後勤部隊通信較差
        }
    }
    
    return FMath::Clamp(BaseQuality, 0.0f, 1.0f);
}

void AMingMultiUnitCoordinator::UpdateGroupStatus(const FString& GroupID)
{
    FUnitGroup* Group = UnitGroups.Find(GroupID);
    if (!Group)
    {
        return;
    }
    
    // 檢查分組狀態
    int32 ActiveUnits = 0;
    for (AMingTacticalUnit* Unit : Group->GroupUnits)
    {
        if (Unit && Unit->IsValidLowLevel())
        {
            ActiveUnits++;
        }
    }
    
    if (ActiveUnits == 0)
    {
        Group->bIsActive = false;
        Group->GroupStatus = TEXT("Inactive");
    }
    else if (ActiveUnits < Group->GroupUnits.Num() / 2)
    {
        Group->GroupStatus = TEXT("Weakened");
    }
    else
    {
        Group->GroupStatus = TEXT("Active");
    }
    
    Group->LastUpdateTime = FPlatformTime::Seconds();
}

void AMingMultiUnitCoordinator::UpdateCoordinationStatistics(const FString& CommandID, const FCoordinationResult& Result)
{
    if (Result.bSuccess)
    {
        int32* SuccessfulCommands = CoordinationStatistics.Find(TEXT("successful_commands"));
        if (SuccessfulCommands)
        {
            (*SuccessfulCommands)++;
        }
    }
    else
    {
        int32* FailedCommands = CoordinationStatistics.Find(TEXT("failed_commands"));
        if (FailedCommands)
        {
            (*FailedCommands)++;
        }
    }
}

void AMingMultiUnitCoordinator::UpdatePerformanceMetrics()
{
    // 更新協調效率
    int32 TotalCommands = CoordinationStatistics.FindRef(TEXT("total_commands"));
    int32 SuccessfulCommands = CoordinationStatistics.FindRef(TEXT("successful_commands"));
    
    if (TotalCommands > 0)
    {
        float Efficiency = (float)SuccessfulCommands / TotalCommands;
        PerformanceMetrics.Add(TEXT("coordination_efficiency"), Efficiency);
    }
    
    // 更新同步精度
    float TotalSyncAccuracy = 0.0f;
    int32 ExecutingCommands = 0;
    
    for (const auto& CommandPair : CoordinatedCommands)
    {
        if (CommandPair.Value.bIsExecuted && !CommandPair.Value.bIsCompleted)
        {
            TotalSyncAccuracy += CalculateSyncAccuracy(CommandPair.Value);
            ExecutingCommands++;
        }
    }
    
    if (ExecutingCommands > 0)
    {
        PerformanceMetrics.Add(TEXT("sync_accuracy"), TotalSyncAccuracy / ExecutingCommands);
    }
    
    // 更新通信質量
    float TotalCommunicationQuality = 0.0f;
    int32 CommunicationLinks = 0;
    
    for (const auto& ConnectionPair : CommunicationConnections)
    {
        for (const auto& LinkPair : ConnectionPair.Value)
        {
            TotalCommunicationQuality += CalculateCommunicationQuality(ConnectionPair.Key, LinkPair.Key);
            CommunicationLinks++;
        }
    }
    
    if (CommunicationLinks > 0)
    {
        PerformanceMetrics.Add(TEXT("communication_quality"), TotalCommunicationQuality / CommunicationLinks);
    }
}

bool AMingMultiUnitCoordinator::ValidateGroupConfig(const FUnitGroup& Group) const
{
    if (Group.GroupID.IsEmpty())
    {
        return false;
    }
    
    if (Group.GroupUnits.Num() == 0)
    {
        return false;
    }
    
    if (Group.GroupType == EUnitGroupType::Custom && Group.GroupName.IsEmpty())
    {
        return false;
    }
    
    return true;
}

bool AMingMultiUnitCoordinator::ValidateCoordinatedCommand(const FCoordinatedCommand& Command) const
{
    if (Command.CommandID.IsEmpty())
    {
        return false;
    }
    
    if (Command.ParticipatingGroups.Num() == 0)
    {
        return false;
    }
    
    // 檢查參與分組是否存在
    for (const FString& GroupID : Command.ParticipatingGroups)
    {
        if (!UnitGroups.Contains(GroupID))
        {
            return false;
        }
    }
    
    return true;
}

FString AMingMultiUnitCoordinator::GetGroupDescription(const FString& GroupID) const
{
    const FUnitGroup* Group = UnitGroups.Find(GroupID);
    if (!Group)
    {
        return TEXT("Group not found");
    }
    
    return FString::Printf(TEXT("%s (%s) - %d units - %s"), 
        *Group->GroupName, 
        *UEnum::GetValueAsString(Group->GroupType), 
        Group->GroupUnits.Num(), 
        *Group->GroupStatus);
}

FString AMingMultiUnitCoordinator::GetCommandDescription(const FString& CommandID) const
{
    const FCoordinatedCommand* Command = CoordinatedCommands.Find(CommandID);
    if (!Command)
    {
        return TEXT("Command not found");
    }
    
    return FString::Printf(TEXT("%s (%s) - %s"), 
        *Command->CommandName, 
        *UEnum::GetValueAsString(Command.CommandType),
        *Command->CommandStatus);
}

FString AMingMultiUnitCoordinator::GetSyncTypeDescription(ECoordinationSyncType SyncType) const
{
    switch (SyncType)
    {
    case ECoordinationSyncType::Simultaneous:
        return TEXT("同時執行");
    case ECoordinationSyncType::Sequential:
        return TEXT("順序執行");
    case ECoordinationSyncType::Staggered:
        return TEXT("交錯執行");
    case ECoordinationSyncType::Wave:
        return TEXT("波浪執行");
    case ECoordinationSyncType::Phased:
        return TEXT("階段執行");
    case ECoordinationSyncType::Conditional:
        return TEXT("條件執行");
    case ECoordinationSyncType::EventDriven:
        return TEXT("事件驅動");
    case ECoordinationSyncType::TimeBased:
        return TEXT("時間基礎");
    case ECoordinationSyncType::PriorityBased:
        return TEXT("優先級基礎");
    case ECoordinationSyncType::Adaptive:
        return TEXT("自適應");
    case ECoordinationSyncType::Reactive:
        return TEXT("反應式");
    case ECoordinationSyncType::Proactive:
        return TEXT("主動式");
    default:
        return TEXT("未知同步類型");
    }
}

FString AMingMultiUnitCoordinator::GetProtocolDescription(ECommunicationProtocol Protocol) const
{
    switch (Protocol)
    {
    case ECommunicationProtocol::Direct:
        return TEXT("直接通信");
    case ECommunicationProtocol::Relay:
        return TEXT("中繼通信");
    case ECommunicationProtocol::Broadcast:
        return TEXT("廣播通信");
    case ECommunicationProtocol::Multicast:
        return TEXT("組播通信");
    case ECommunicationProtocol::Hierarchical:
        return TEXT("層次通信");
    case ECommunicationProtocol::PeerToPeer:
        return TEXT("點對點通信");
    case ECommunicationProtocol::Centralized:
        return TEXT("集中式通信");
    case ECommunicationProtocol::Decentralized:
        return TEXT("分散式通信");
    case ECommunicationProtocol::Hybrid:
        return TEXT("混合通信");
    case ECommunicationProtocol::Secure:
        return TEXT("安全通信");
    case ECommunicationProtocol::Encrypted:
        return TEXT("加密通信");
    case ECommunicationProtocol::Emergency:
        return TEXT("緊急通信");
    default:
        return TEXT("未知協議");
    }
}

void AMingMultiUnitCoordinator::LogCoordinationEvent(const FString& EventType, const FString& Details)
{
    UE_LOG(LogTemp, Log, TEXT("[MultiUnitCoordinator] %s: %s"), *EventType, *Details);
}

float AMingMultiUnitCoordinator::CalculateCoordinationScore(const FCoordinationResult& Result) const
{
    float Score = 0.0f;
    
    // 基於成功率
    if (Result.bSuccess)
    {
        Score += 0.5f;
    }
    
    // 基於參與單位
    if (Result.ParticipatingUnits.Num() > 0)
    {
        float SuccessRate = (float)Result.SuccessfulUnits.Num() / Result.ParticipatingUnits.Num();
        Score += SuccessRate * 0.3f;
    }
    
    // 基於同步精度
    Score += Result.SyncAccuracy * 0.1f;
    
    // 基於通信質量
    Score += Result.CommunicationQuality * 0.1f;
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}

float AMingMultiUnitCoordinator::HandleCommunicationDelay(const FString& SourceGroupID, const FString& TargetGroupID) const
{
    // 簡化實作：基於分組距離和通信協議計算延迟
    const TMap<FString, ECommunicationProtocol>* Connections = CommunicationConnections.Find(SourceGroupID);
    if (!Connections)
    {
        return 0.0f;
    }
    
    const ECommunicationProtocol* Protocol = Connections->Find(TargetGroupID);
    if (!Protocol)
    {
        return 0.0f;
    }
    
    switch (*Protocol)
    {
    case ECommunicationProtocol::Direct:
    case ECommunicationProtocol::PeerToPeer:
        return 0.1f; // 0.1秒延迟
    case ECommunicationProtocol::Relay:
    case ECommunicationProtocol::Multicast:
        return 0.3f; // 0.3秒延迟
    case ECommunicationProtocol::Broadcast:
    case ECommunicationProtocol::Centralized:
        return 0.5f; // 0.5秒延迟
    case ECommunicationProtocol::Hierarchical:
    case ECommunicationProtocol::Decentralized:
        return 0.4f; // 0.4秒延迟
    case ECommunicationProtocol::Hybrid:
        return 0.35f; // 0.35秒延迟
    case ECommunicationProtocol::Secure:
    case ECommunicationProtocol::Encrypted:
        return 0.6f; // 0.6秒延迟（加密需要時間）
    case ECommunicationProtocol::Emergency:
        return 0.05f; // 0.05秒延迟（緊急通信最快）
    default:
        return 0.2f; // 默认0.2秒延迟
    }
}

float AMingMultiUnitCoordinator::HandleCommunicationInterference(const FString& SourceGroupID, const FString& TargetGroupID) const
{
    // 簡化實作：基於環境因素計算干擾
    float Interference = 0.0f;
    
    // 基於分組類型
    const FUnitGroup* SourceGroup = UnitGroups.Find(SourceGroupID);
    const FUnitGroup* TargetGroup = UnitGroups.Find(TargetGroupID);
    
    if (SourceGroup && TargetGroup)
    {
        // 特種部隊有更好的抗干擾能力
        if (SourceGroup->GroupType == EUnitGroupType::SpecialForces)
        {
            Interference -= 0.1f;
        }
        
        // 後勤部隊更容易受到干擾
        if (TargetGroup->GroupType == EUnitGroupType::Logistics)
        {
            Interference += 0.1f;
        }
    }
    
    return FMath::Clamp(Interference, 0.0f, 1.0f);
}

bool AMingMultiUnitCoordinator::HandleCommunicationEncryption(const FString& SourceGroupID, const FString& TargetGroupID) const
{
    // 簡化實作：檢查是否需要加密
    const TMap<FString, ECommunicationProtocol>* Connections = CommunicationConnections.Find(SourceGroupID);
    if (!Connections)
    {
        return false;
    }
    
    const ECommunicationProtocol* Protocol = Connections->Find(TargetGroupID);
    if (!Protocol)
    {
        return false;
    }
    
    return *Protocol == ECommunicationProtocol::Secure || *Protocol == ECommunicationProtocol::Encrypted;
}

ECoordinationSyncType AMingMultiUnitCoordinator::GetOptimalSyncType(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType) const
{
    // 簡化實作：基於命令類型和分組數量確定最佳同步類型
    
    if (GroupIDs.Num() == 1)
    {
        return ECoordinationSyncType::Simultaneous;
    }
    
    switch (CommandType)
    {
    case ECoordinatedCommandType::CoordinatedAttack:
        return GroupIDs.Num() <= 3 ? ECoordinationSyncType::Simultaneous : ECoordinationSyncType::Wave;
    case ECoordinatedCommandType::CoordinatedDefense:
        return ECoordinationSyncType::Phased;
    case ECoordinatedCommandType::CoordinatedMovement:
        return ECoordinationSyncType::Staggered;
    case ECoordinatedCommandType::CoordinatedRetreat:
        return ECoordinationSyncType::PriorityBased;
    case ECoordinatedCommandType::CoordinatedSupport:
        return ECoordinationSyncType::Conditional;
    case ECoordinatedCommandType::CoordinatedAmbush:
        return ECoordinationSyncType::EventDriven;
    case ECoordinatedCommandType::CoordinatedFlanking:
        return ECoordinationSyncType::Sequential;
    case ECoordinatedCommandType::CoordinatedEncirclement:
        return ECoordinationSyncType::Adaptive;
    case ECoordinatedCommandType::CoordinatedBreakthrough:
        return ECoordinationSyncType::Proactive;
    default:
        return ECoordinationSyncType::Simultaneous;
    }
}

ECommunicationProtocol AMingMultiUnitCoordinator::GetOptimalCommunicationProtocol(const TArray<FString>& GroupIDs) const
{
    // 簡化實作：基於分組類型和數量確定最佳通信協議
    
    if (GroupIDs.Num() == 1)
    {
        return ECommunicationProtocol::Direct;
    }
    
    if (GroupIDs.Num() <= 3)
    {
        return ECommunicationProtocol::PeerToPeer;
    }
    
    if (GroupIDs.Num() <= 6)
    {
        return ECommunicationProtocol::Multicast;
    }
    
    // 檢查是否有特殊部隊
    bool HasSpecialForces = false;
    for (const FString& GroupID : GroupIDs)
    {
        const FUnitGroup* Group = UnitGroups.Find(GroupID);
        if (Group && Group->GroupType == EUnitGroupType::SpecialForces)
        {
            HasSpecialForces = true;
            break;
        }
    }
    
    if (HasSpecialForces)
    {
        return ECommunicationProtocol::Secure;
    }
    
    return ECommunicationProtocol::Hierarchical;
}

float AMingMultiUnitCoordinator::CalculateGroupCoordinationCapability(const FString& GroupID) const
{
    const FUnitGroup* Group = UnitGroups.Find(GroupID);
    if (!Group)
    {
        return 0.0f;
    }
    
    float Capability = 0.0f;
    
    // 基於分組類型
    switch (Group->GroupType)
    {
    case EUnitGroupType::SpecialForces:
        Capability += 0.9f;
        break;
    case EUnitGroupType::TaskForce:
        Capability += 0.8f;
        break;
    case EUnitGroupType::Company:
    case EUnitGroupType::Battalion:
        Capability += 0.7f;
        break;
    case EUnitGroupType::Platoon:
    case EUnitGroupType::Squad:
        Capability += 0.6f;
        break;
    case EUnitGroupType::Support:
    case EUnitGroupType::Logistics:
        Capability += 0.5f;
        break;
    default:
        Capability += 0.6f;
        break;
    }
    
    // 基於單位數量
    if (Group->GroupUnits.Num() >= 5)
    {
        Capability += 0.1f;
    }
    else if (Group->GroupUnits.Num() < 3)
    {
        Capability -= 0.1f;
    }
    
    // 基於領導者
    if (Group->GroupLeader)
    {
        Capability += 0.1f;
    }
    
    return FMath::Clamp(Capability, 0.0f, 1.0f);
}

float AMingMultiUnitCoordinator::CalculateOverallCoordinationEfficiency() const
{
    float Efficiency = 0.0f;
    
    // 基於協調統計
    int32 TotalCommands = CoordinationStatistics.FindRef(TEXT("total_commands"));
    int32 SuccessfulCommands = CoordinationStatistics.FindRef(TEXT("successful_commands"));
    
    if (TotalCommands > 0)
    {
        Efficiency += (float)SuccessfulCommands / TotalCommands * 0.5f;
    }
    
    // 基於通信質量
    const float* CommunicationQuality = PerformanceMetrics.Find(TEXT("communication_quality"));
    if (CommunicationQuality)
    {
        Efficiency += *CommunicationQuality * 0.3f;
    }
    
    // 基於同步精度
    const float* SyncAccuracy = PerformanceMetrics.Find(TEXT("sync_accuracy"));
    if (SyncAccuracy)
    {
        Efficiency += *SyncAccuracy * 0.2f;
    }
    
    return FMath::Clamp(Efficiency, 0.0f, 1.0f);
}

void AMingMultiUnitCoordinator::HandleCoordinationFailure(const FString& CommandID, const FString& FailureReason)
{
    UE_LOG(LogTemp, Warning, TEXT("Coordination failed: %s - %s"), *CommandID, *FailureReason);
    
    FCoordinatedCommand* Command = CoordinatedCommands.Find(CommandID);
    if (Command)
    {
        Command->CommandStatus = TEXT("Failed");
        Command->CompletionTime = FPlatformTime::Seconds();
    }
    
    // 更新統計
    int32* FailedCommands = CoordinationStatistics.Find(TEXT("failed_commands"));
    if (FailedCommands)
    {
        (*FailedCommands)++;
    }
}

void AMingMultiUnitCoordinator::HandleCoordinationSuccess(const FString& CommandID)
{
    UE_LOG(LogTemp, Log, TEXT("Coordination succeeded: %s"), *CommandID);
    
    FCoordinatedCommand* Command = CoordinatedCommands.Find(CommandID);
    if (Command)
    {
        Command->CommandStatus = TEXT("Completed");
        Command->bIsCompleted = true;
        Command->CompletionTime = FPlatformTime::Seconds();
    }
    
    // 更新統計
    int32* SuccessfulCommands = CoordinationStatistics.Find(TEXT("successful_commands"));
    if (SuccessfulCommands)
    {
        (*SuccessfulCommands)++;
    }
}

void AMingMultiUnitCoordinator::CleanupCompletedCoordinations()
{
    // 清理已完成的協調命令
    TArray<FString> CommandsToRemove;
    
    for (const auto& CommandPair : CoordinatedCommands)
    {
        if (CommandPair.Value.bIsCompleted || CommandPair.Value.bIsCancelled)
        {
            CommandsToRemove.Add(CommandPair.Key);
        }
    }
    
    for (const FString& CommandID : CommandsToRemove)
    {
        CoordinatedCommands.Remove(CommandID);
    }
    
    if (CommandsToRemove.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Cleaned up %d completed coordinations"), CommandsToRemove.Num());
    }
}

void AMingMultiUnitCoordinator::BackupCoordinationData()
{
    // 簡化實作：記錄備份操作
    UE_LOG(LogTemp, Log, TEXT("Coordination data backed up"));
}

void AMingMultiUnitCoordinator::RestoreCoordinationData()
{
    // 簡化實作：記錄恢復操作
    UE_LOG(LogTemp, Log, TEXT("Coordination data restored"));
}

void AMingMultiUnitCoordinator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsInitialized)
    {
        return;
    }

    // 更新所有分組狀態
    for (auto& GroupPair : UnitGroups)
    {
        UpdateGroupStatus(GroupPair.Key);
    }

    // 更新性能指標
    UpdatePerformanceMetrics();

    // 清理已完成的協調
    CleanupCompletedCoordinations();
}

// === 輔助方法實現 ===

void AMingMultiUnitCoordinator::ExecuteGroupCommand(const FString& GroupID, const FCoordinatedCommand& Command)
{
    FUnitGroup* Group = UnitGroups.Find(GroupID);
    if (!Group || !Group->bIsActive)
    {
        return;
    }

    // 根據命令類型執行分組命令
    switch (Command.CommandType)
    {
    case ECoordinatedCommandType::CoordinatedAttack:
        ExecuteGroupAttack(Group, Command);
        break;
    case ECoordinatedCommandType::CoordinatedDefense:
        ExecuteGroupDefense(Group, Command);
        break;
    case ECoordinatedCommandType::CoordinatedMovement:
        ExecuteGroupMovement(Group, Command);
        break;
    case ECoordinatedCommandType::CoordinatedRetreat:
        ExecuteGroupRetreat(Group, Command);
        break;
    case ECoordinatedCommandType::CoordinatedSupport:
        ExecuteGroupSupport(Group, Command);
        break;
    case ECoordinatedCommandType::CoordinatedAmbush:
        ExecuteGroupAmbush(Group, Command);
        break;
    case ECoordinatedCommandType::CoordinatedFlanking:
        ExecuteGroupFlanking(Group, Command);
        break;
    case ECoordinatedCommandType::CoordinatedEncirclement:
        ExecuteGroupEncirclement(Group, Command);
        break;
    case ECoordinatedCommandType::CoordinatedBreakthrough:
        ExecuteGroupBreakthrough(Group, Command);
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown coordinated command type: %d"), static_cast<int32>(Command.CommandType));
        break;
    }
}

void AMingMultiUnitCoordinator::ExecuteGroupAttack(FUnitGroup* Group, const FCoordinatedCommand& Command)
{
    if (!UnitController)
    {
        return;
    }
    
    // 執行分組攻擊
    UE_LOG(LogTemp, Log, TEXT("Executing group attack for %s"), *Group->GroupName);
    
    // 簡化實作：讓所有單位攻擊目標
    FString* TargetCount = Command.CommandParameters.Find(TEXT("target_count"));
    if (TargetCount)
    {
        int32 Count = FCString::Atoi(**TargetCount);
        // 這裡應該實現實際的攻擊邏輯
    }
}

void AMingMultiUnitCoordinator::ExecuteGroupDefense(FUnitGroup* Group, const FCoordinatedCommand& Command)
{
    if (!UnitController)
    {
        return;
    }
    
    // 執行分組防禦
    UE_LOG(LogTemp, Log, TEXT("Executing group defense for %s"), *Group->GroupName);
    
    // 簡化實作：讓所有單位進入防禦狀態
    // 這裡應該實現實際的防禦邏輯
}

void AMingMultiUnitCoordinator::ExecuteGroupMovement(FUnitGroup* Group, const FCoordinatedCommand& Command)
{
    if (!UnitController)
    {
        return;
    }
    
    // 執行分組移動
    UE_LOG(LogTemp, Log, TEXT("Executing group movement for %s"), *Group->GroupName);
    
    // 簡化實作：讓所有單位移動到目標位置
    // 這裡應該實現實際的移動邏輯
}

void AMingMultiUnitCoordinator::ExecuteGroupRetreat(FUnitGroup* Group, const FCoordinatedCommand& Command)
{
    if (!UnitController)
    {
        return;
    }
    
    // 執行分組撤退
    UE_LOG(LogTemp, Log, TEXT("Executing group retreat for %s"), *Group->GroupName);
    
    // 簡化實作：讓所有單位撤退到安全位置
    // 這裡應該實現實際的撤退邏輯
}

void AMingMultiUnitCoordinator::ExecuteGroupSupport(FUnitGroup* Group, const FCoordinatedCommand& Command)
{
    if (!UnitController)
    {
        return;
    }
    
    // 執行分組支援
    UE_LOG(LogTemp, Log, TEXT("Executing group support for %s"), *Group->GroupName);
    
    // 簡化實作：讓所有單位提供支援
    // 這裡應該實現實際的支援邏輯
}

void AMingMultiUnitCoordinator::ExecuteGroupAmbush(FUnitGroup* Group, const FCoordinatedCommand& Command)
{
    if (!UnitController)
    {
        return;
    }
    
    // 執行分組伏擊
    UE_LOG(LogTemp, Log, TEXT("Executing group ambush for %s"), *Group->GroupName);
    
    // 簡化實作：讓所有單位執行伏擊
    // 這裡應該實現實際的伏擊邏輯
}

void AMingMultiUnitCoordinator::ExecuteGroupFlanking(FUnitGroup* Group, const FCoordinatedCommand& Command)
{
    if (!UnitController)
    {
        return;
    }
    
    // 執行分組側翼攻擊
    UE_LOG(LogTemp, Log, TEXT("Executing group flanking for %s"), *Group->GroupName);
    
    // 簡化實作：讓所有單位執行側翼攻擊
    // 這裡應該實現實際的側翼攻擊邏輯
}

void AMingMultiUnitCoordinator::ExecuteGroupEncirclement(FUnitGroup* Group, const FCoordinatedCommand& Command)
{
    if (!UnitController)
    {
        return;
    }
    
    // 執行分組包圍
    UE_LOG(LogTemp, Log, TEXT("Executing group encirclement for %s"), *Group->GroupName);
    
    // 簡化實作：讓所有單位執行包圍
    // 這裡應該實現實際的包圍邏輯
}

void AMingMultiUnitCoordinator::ExecuteGroupBreakthrough(FUnitGroup* Group, const FCoordinatedCommand& Command)
{
    if (!UnitController)
    {
        return;
    }
    
    // 執行分組突破
    UE_LOG(LogTemp, Log, TEXT("Executing group breakthrough for %s"), *Group->GroupName);
    
    // 簡化實作：讓所有單位執行突破
    // 這裡應該實現實際的突破邏輯
}

bool AMingMultiUnitCoordinator::CheckExecutionConditions(const FString& GroupID, const FCoordinatedCommand& Command) const
{
    // 簡化實作：檢查執行條件
    const FUnitGroup* Group = UnitGroups.Find(GroupID);
    if (!Group || !Group->bIsActive)
    {
        return false;
    }
    
    // 檢查單位數量
    if (Group->GroupUnits.Num() < 2)
    {
        return false;
    }
    
    return true;
}

bool AMingMultiUnitCoordinator::CheckEventConditions(const FString& GroupID, const FCoordinatedCommand& Command) const
{
    // 簡化實作：檢查事件條件
    return CheckExecutionConditions(GroupID, Command);
}

bool AMingMultiUnitCoordinator::CheckReactiveConditions(const FString& GroupID, const FCoordinatedCommand& Command) const
{
    // 簡化實作：檢查反應條件
    return CheckExecutionConditions(GroupID, Command);
}

bool AMingMultiUnitCoordinator::CheckProactiveConditions(const FString& GroupID, const FCoordinatedCommand& Command) const
{
    // 簡化實作：檢查主動條件
    return CheckExecutionConditions(GroupID, Command);
}

void AMingMultiUnitCoordinator::HandleTimeBasedExecution(const FCoordinatedCommand& Command)
{
    // 時間基礎執行已在主方法中處理
}

void AMingMultiUnitCoordinator::HandlePriorityBasedExecution(const FCoordinatedCommand& Command)
{
    // 優先級基礎執行已在主方法中處理
}

void AMingMultiUnitCoordinator::HandleAdaptiveExecution(const FCoordinatedCommand& Command)
{
    // 自適應執行已在主方法中處理
}

void AMingMultiUnitCoordinator::HandleReactiveExecution(const FCoordinatedCommand& Command)
{
    // 反應式執行已在主方法中處理
}

void AMingMultiUnitCoordinator::HandleProactiveExecution(const FCoordinatedCommand& Command)
{
    // 主動式執行已在主方法中處理
}
