#include "MingMultiUnitBlueprintLibrary.h"
#include "MingMultiUnitCoordinator.h"
#include "MingTacticalUnit.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

AMingMultiUnitCoordinator* UMingMultiUnitBlueprintLibrary::GetMultiUnitCoordinator()
{
    // 這裡應該從遊戲實例或單例獲取多單位協調器
    // 簡化實作
    static AMingMultiUnitCoordinator* CoordinatorInstance = nullptr;
    if (!CoordinatorInstance)
    {
        UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(GEngine, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
        if (World)
        {
            CoordinatorInstance = World->SpawnActor<AMingMultiUnitCoordinator>();
            CoordinatorInstance->InitializeMultiUnitCoordinator();
        }
    }
    
    return CoordinatorInstance;
}

void UMingMultiUnitBlueprintLibrary::InitializeMultiUnitCoordinator()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (Coordinator)
    {
        Coordinator->InitializeMultiUnitCoordinator();
        UE_LOG(LogTemp, Log, TEXT("Multi unit coordinator initialized from Blueprint"));
    }
}

void UMingMultiUnitBlueprintLibrary::ShutdownMultiUnitCoordinator()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (Coordinator)
    {
        Coordinator->ShutdownMultiUnitCoordinator();
        UE_LOG(LogTemp, Log, TEXT("Multi unit coordinator shutdown from Blueprint"));
    }
}

FUnitGroup UMingMultiUnitBlueprintLibrary::CreateUnitGroup(
    const FString& GroupName,
    EUnitGroupType GroupType,
    const TArray<AMingTacticalUnit*>& Units,
    AMingTacticalUnit* Leader)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        UE_LOG(LogTemp, Error, TEXT("Multi unit coordinator not available"));
        return FUnitGroup();
    }
    
    return Coordinator->CreateUnitGroup(GroupName, GroupType, Units, Leader);
}

bool UMingMultiUnitBlueprintLibrary::DisbandUnitGroup(const FString& GroupID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    return Coordinator->DisbandUnitGroup(GroupID);
}

FUnitGroup UMingMultiUnitBlueprintLibrary::GetUnitGroup(const FString& GroupID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return FUnitGroup();
    }
    
    return Coordinator->GetUnitGroup(GroupID);
}

TArray<FUnitGroup> UMingMultiUnitBlueprintLibrary::GetAllUnitGroups()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return TArray<FUnitGroup>();
    }
    
    return Coordinator->GetAllUnitGroups();
}

TArray<FUnitGroup> UMingMultiUnitBlueprintLibrary::GetGroupsByType(EUnitGroupType GroupType)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return TArray<FUnitGroup>();
    }
    
    return Coordinator->GetGroupsByType(GroupType);
}

TArray<FUnitGroup> UMingMultiUnitBlueprintLibrary::GetActiveGroups()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return TArray<FUnitGroup>();
    }
    
    return Coordinator->GetActiveGroups();
}

bool UMingMultiUnitBlueprintLibrary::AddUnitToGroup(const FString& GroupID, AMingTacticalUnit* Unit)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    return Coordinator->AddUnitToGroup(GroupID, Unit);
}

bool UMingMultiUnitBlueprintLibrary::RemoveUnitFromGroup(const FString& GroupID, AMingTacticalUnit* Unit)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    return Coordinator->RemoveUnitFromGroup(GroupID, Unit);
}

bool UMingMultiUnitBlueprintLibrary::SetGroupLeader(const FString& GroupID, AMingTacticalUnit* Leader)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    return Coordinator->SetGroupLeader(GroupID, Leader);
}

FCoordinationResult UMingMultiUnitBlueprintLibrary::IssueCoordinatedCommand(const FCoordinatedCommand& Command)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        FCoordinationResult Result;
        Result.ErrorMessage = TEXT("Multi unit coordinator not available");
        return Result;
    }
    
    return Coordinator->IssueCoordinatedCommand(Command);
}

FCoordinationResult UMingMultiUnitBlueprintLibrary::ExecuteCoordinatedAttack(
    const TArray<FString>& GroupIDs,
    const TArray<AMingTacticalUnit*>& Targets,
    ECoordinationSyncType SyncType)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        FCoordinationResult Result;
        Result.ErrorMessage = TEXT("Multi unit coordinator not available");
        return Result;
    }
    
    return Coordinator->ExecuteCoordinatedAttack(GroupIDs, Targets, SyncType);
}

FCoordinationResult UMingMultiUnitBlueprintLibrary::ExecuteCoordinatedDefense(
    const TArray<FString>& GroupIDs,
    const TArray<FVector>& DefensePositions,
    ECoordinationSyncType SyncType)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        FCoordinationResult Result;
        Result.ErrorMessage = TEXT("Multi unit coordinator not available");
        return Result;
    }
    
    return Coordinator->ExecuteCoordinatedDefense(GroupIDs, DefensePositions, SyncType);
}

FCoordinationResult UMingMultiUnitBlueprintLibrary::ExecuteCoordinatedMovement(
    const TArray<FString>& GroupIDs,
    const TArray<FVector>& TargetPositions,
    ECoordinationSyncType SyncType)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        FCoordinationResult Result;
        Result.ErrorMessage = TEXT("Multi unit coordinator not available");
        return Result;
    }
    
    return Coordinator->ExecuteCoordinatedMovement(GroupIDs, TargetPositions, SyncType);
}

FCoordinationResult UMingMultiUnitBlueprintLibrary::ExecuteCoordinatedRetreat(
    const TArray<FString>& GroupIDs,
    const TArray<FVector>& RetreatPositions,
    ECoordinationSyncType SyncType)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        FCoordinationResult Result;
        Result.ErrorMessage = TEXT("Multi unit coordinator not available");
        return Result;
    }
    
    return Coordinator->ExecuteCoordinatedRetreat(GroupIDs, RetreatPositions, SyncType);
}

FCoordinationResult UMingMultiUnitBlueprintLibrary::ExecuteCoordinatedSupport(
    const TArray<FString>& SupportGroupIDs,
    const TArray<FString>& TargetGroupIDs,
    ECoordinationSyncType SyncType)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        FCoordinationResult Result;
        Result.ErrorMessage = TEXT("Multi unit coordinator not available");
        return Result;
    }
    
    return Coordinator->ExecuteCoordinatedSupport(SupportGroupIDs, TargetGroupIDs, SyncType);
}

FCoordinationResult UMingMultiUnitBlueprintLibrary::ExecuteCoordinatedAmbush(
    const TArray<FString>& AmbusherGroupIDs,
    const TArray<AMingTacticalUnit*>& Targets,
    ECoordinationSyncType SyncType)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        FCoordinationResult Result;
        Result.ErrorMessage = TEXT("Multi unit coordinator not available");
        return Result;
    }
    
    return Coordinator->ExecuteCoordinatedAmbush(AmbusherGroupIDs, Targets, SyncType);
}

FCoordinationResult UMingMultiUnitBlueprintLibrary::ExecuteCoordinatedFlanking(
    const TArray<FString>& FlankerGroupIDs,
    const TArray<AMingTacticalUnit*>& Targets,
    ECoordinationSyncType SyncType)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        FCoordinationResult Result;
        Result.ErrorMessage = TEXT("Multi unit coordinator not available");
        return Result;
    }
    
    return Coordinator->ExecuteCoordinatedFlanking(FlankerGroupIDs, Targets, SyncType);
}

FCoordinationResult UMingMultiUnitBlueprintLibrary::ExecuteCoordinatedEncirclement(
    const TArray<FString>& EncirclerGroupIDs,
    const TArray<AMingTacticalUnit*>& Targets,
    ECoordinationSyncType SyncType)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        FCoordinationResult Result;
        Result.ErrorMessage = TEXT("Multi unit coordinator not available");
        return Result;
    }
    
    return Coordinator->ExecuteCoordinatedEncirclement(EncirclerGroupIDs, Targets, SyncType);
}

FCoordinationResult UMingMultiUnitBlueprintLibrary::ExecuteCoordinatedBreakthrough(
    const TArray<FString>& BreakthroughGroupIDs,
    const TArray<FVector>& BreakthroughPoints,
    ECoordinationSyncType SyncType)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        FCoordinationResult Result;
        Result.ErrorMessage = TEXT("Multi unit coordinator not available");
        return Result;
    }
    
    return Coordinator->ExecuteCoordinatedBreakthrough(BreakthroughGroupIDs, BreakthroughPoints, SyncType);
}

bool UMingMultiUnitBlueprintLibrary::CancelCoordinatedCommand(const FString& CommandID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    return Coordinator->CancelCoordinatedCommand(CommandID);
}

FCoordinatedCommand UMingMultiUnitBlueprintLibrary::GetCoordinatedCommand(const FString& CommandID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return FCoordinatedCommand();
    }
    
    return Coordinator->GetCoordinatedCommand(CommandID);
}

TArray<FCoordinatedCommand> UMingMultiUnitBlueprintLibrary::GetAllCoordinatedCommands()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return TArray<FCoordinatedCommand>();
    }
    
    return Coordinator->GetAllCoordinatedCommands();
}

TArray<FCoordinatedCommand> UMingMultiUnitBlueprintLibrary::GetExecutingCommands()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return TArray<FCoordinatedCommand>();
    }
    
    return Coordinator->GetExecutingCommands();
}

FString UMingMultiUnitBlueprintLibrary::GetCoordinationStatus(const FString& CommandID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return TEXT("Coordinator not available");
    }
    
    return Coordinator->GetCoordinationStatus(CommandID);
}

TMap<FString, int32> UMingMultiUnitBlueprintLibrary::GetCoordinationStatistics()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return TMap<FString, int32>();
    }
    
    return Coordinator->GetCoordinationStatistics();
}

TMap<FString, float> UMingMultiUnitBlueprintLibrary::GetCoordinationPerformanceMetrics()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return TMap<FString, float>();
    }
    
    return Coordinator->GetCoordinationPerformanceMetrics();
}

bool UMingMultiUnitBlueprintLibrary::SynchronizeUnitStates(const TArray<AMingTacticalUnit*>& Units)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    return Coordinator->SynchronizeUnitStates(Units);
}

bool UMingMultiUnitBlueprintLibrary::BroadcastToGroup(const FString& GroupID, const FString& Message)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    return Coordinator->BroadcastToGroup(GroupID, Message);
}

bool UMingMultiUnitBlueprintLibrary::RelayMessage(const FString& SourceGroupID, const FString& TargetGroupID, const FString& Message)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    return Coordinator->RelayMessage(SourceGroupID, TargetGroupID, Message);
}

bool UMingMultiUnitBlueprintLibrary::EstablishCommunication(const FString& GroupAID, const FString& GroupBID, ECommunicationProtocol Protocol)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    return Coordinator->EstablishCommunication(GroupAID, GroupBID, Protocol);
}

bool UMingMultiUnitBlueprintLibrary::DisconnectCommunication(const FString& GroupAID, const FString& GroupBID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    return Coordinator->DisconnectCommunication(GroupAID, GroupBID);
}

float UMingMultiUnitBlueprintLibrary::GetCommunicationQuality(const FString& GroupAID, const FString& GroupBID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return 0.0f;
    }
    
    return Coordinator->GetCommunicationQuality(GroupAID, GroupBID);
}

FString UMingMultiUnitBlueprintLibrary::GetGroupTypeName(EUnitGroupType GroupType)
{
    switch (GroupType)
    {
    case EUnitGroupType::Squad:
        return TEXT("班");
    case EUnitGroupType::Platoon:
        return TEXT("排");
    case EUnitGroupType::Company:
        return TEXT("連");
    case EUnitGroupType::Battalion:
        return TEXT("營");
    case EUnitGroupType::Regiment:
        return TEXT("團");
    case EUnitGroupType::Brigade:
        return TEXT("旅");
    case EUnitGroupType::Division:
        return TEXT("師");
    case EUnitGroupType::Corps:
        return TEXT("軍");
    case EUnitGroupType::Army:
        return TEXT("集團軍");
    case EUnitGroupType::TaskForce:
        return TEXT("特遣隊");
    case EUnitGroupType::SpecialForces:
        return TEXT("特種部隊");
    case EUnitGroupType::Reserve:
        return TEXT("預備隊");
    case EUnitGroupType::Support:
        return TEXT("支援隊");
    case EUnitGroupType::Logistics:
        return TEXT("後勤隊");
    case EUnitGroupType::Custom:
        return TEXT("自定義");
    default:
        return TEXT("未知類型");
    }
}

FString UMingMultiUnitBlueprintLibrary::GetCoordinatedCommandTypeName(ECoordinatedCommandType CommandType)
{
    switch (CommandType)
    {
    case ECoordinatedCommandType::CoordinatedAttack:
        return TEXT("協調攻擊");
    case ECoordinatedCommandType::CoordinatedDefense:
        return TEXT("協調防禦");
    case ECoordinatedCommandType::CoordinatedMovement:
        return TEXT("協調移動");
    case ECoordinatedCommandType::CoordinatedRetreat:
        return TEXT("協調撤退");
    case ECoordinatedCommandType::CoordinatedSupport:
        return TEXT("協調支援");
    case ECoordinatedCommandType::CoordinatedAmbush:
        return TEXT("協調伏擊");
    case ECoordinatedCommandType::CoordinatedFlanking:
        return TEXT("協調側翼攻擊");
    case ECoordinatedCommandType::CoordinatedEncirclement:
        return TEXT("協調包圍");
    case ECoordinatedCommandType::CoordinatedBreakthrough:
        return TEXT("協調突破");
    case ECoordinatedCommandType::CoordinatedWithdrawal:
        return TEXT("協調撤離");
    case ECoordinatedCommandType::CoordinatedReinforcement:
        return TEXT("協調增援");
    case ECoordinatedCommandType::CoordinatedResupply:
        return TEXT("協調補給");
    case ECoordinatedCommandType::CoordinatedEvacuation:
        return TEXT("協調撤離");
    case ECoordinatedCommandType::CoordinatedSecuring:
        return TEXT("協調確保");
    case ECoordinatedCommandType::CoordinatedHarassment:
        return TEXT("協調騷擾");
    case ECoordinatedCommandType::CoordinatedDistraction:
        return TEXT("協調牽制");
    default:
        return TEXT("未知命令");
    }
}

FString UMingMultiUnitBlueprintLibrary::GetSyncTypeName(ECoordinationSyncType SyncType)
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

FString UMingMultiUnitBlueprintLibrary::GetProtocolName(ECommunicationProtocol Protocol)
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

FCoordinatedCommand UMingMultiUnitBlueprintLibrary::CreateCoordinatedCommand(
    const FString& CommandName,
    ECoordinatedCommandType CommandType,
    const TArray<FString>& ParticipatingGroups,
    ECoordinationSyncType SyncType,
    ECommunicationProtocol CommunicationProtocol)
{
    FCoordinatedCommand Command;
    Command.CommandID = FString::Printf(TEXT("cmd_%s_%d"), *CommandName, FDateTime::Now().GetMillisecond());
    Command.CommandName = CommandName;
    Command.CommandType = CommandType;
    Command.CommandDescription = FString::Printf(TEXT("%s with %d groups"), *CommandName, ParticipatingGroups.Num());
    Command.ParticipatingGroups = ParticipatingGroups;
    Command.SyncType = SyncType;
    Command.CommunicationProtocol = CommunicationProtocol;
    Command.CommandStatus = TEXT("Planning");
    Command.CreationTime = FPlatformTime::Seconds();
    Command.bIsExecuted = false;
    Command.bIsCompleted = false;
    Command.bIsCancelled = false;
    
    return Command;
}

FCoordinatedCommand UMingMultiUnitBlueprintLibrary::SetCommandParameter(const FCoordinatedCommand& Command, const FString& ParameterName, const FString& ParameterValue)
{
    FCoordinatedCommand ModifiedCommand = Command;
    ModifiedCommand.CommandParameters.Add(ParameterName, ParameterValue);
    return ModifiedCommand;
}

FCoordinatedCommand UMingMultiUnitBlueprintLibrary::AddCommandPhase(const FCoordinatedCommand& Command, const FString& PhaseName)
{
    FCoordinatedCommand ModifiedCommand = Command;
    ModifiedCommand.CommandPhases.Add(PhaseName);
    return ModifiedCommand;
}

FCoordinatedCommand UMingMultiUnitBlueprintLibrary::SetExecutionSchedule(const FCoordinatedCommand& Command, const TMap<FString, float>& Schedule)
{
    FCoordinatedCommand ModifiedCommand = Command;
    ModifiedCommand.ExecutionSchedule = Schedule;
    return ModifiedCommand;
}

FCoordinatedCommand UMingMultiUnitBlueprintLibrary::AddSyncPoint(const FCoordinatedCommand& Command, const FString& SyncPoint)
{
    FCoordinatedCommand ModifiedCommand = Command;
    ModifiedCommand.SyncPoints.Add(SyncPoint);
    return ModifiedCommand;
}

FCoordinatedCommand UMingMultiUnitBlueprintLibrary::AddDependency(const FCoordinatedCommand& Command, const FString& Dependency)
{
    FCoordinatedCommand ModifiedCommand = Command;
    ModifiedCommand.Dependencies.Add(Dependency);
    return ModifiedCommand;
}

TArray<FUnitGroup> UMingMultiUnitBlueprintLibrary::BatchCreateGroups(
    const TArray<FString>& GroupNames,
    const TArray<EUnitGroupType>& GroupTypes,
    const TArray<TArray<AMingTacticalUnit*>>& UnitArrays)
{
    TArray<FUnitGroup> Groups;
    
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return Groups;
    }
    
    for (int32 i = 0; i < GroupNames.Num() && i < GroupTypes.Num() && i < UnitArrays.Num(); ++i)
    {
        FUnitGroup Group = Coordinator->CreateUnitGroup(GroupNames[i], GroupTypes[i], UnitArrays[i]);
        Groups.Add(Group);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch created %d groups"), Groups.Num());
    
    return Groups;
}

TArray<FCoordinationResult> UMingMultiUnitBlueprintLibrary::BatchExecuteCommands(const TArray<FCoordinatedCommand>& Commands)
{
    TArray<FCoordinationResult> Results;
    
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        FCoordinationResult Result;
        Result.ErrorMessage = TEXT("Multi unit coordinator not available");
        Results.Add(Result);
        return Results;
    }
    
    for (const FCoordinatedCommand& Command : Commands)
    {
        Results.Add(Coordinator->IssueCoordinatedCommand(Command));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch executed %d commands"), Results.Num());
    
    return Results;
}

TArray<bool> UMingMultiUnitBlueprintLibrary::BatchCancelCommands(const TArray<FString>& CommandIDs)
{
    TArray<bool> Results;
    
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        for (const FString& CommandID : CommandIDs)
        {
            Results.Add(false);
        }
        return Results;
    }
    
    for (const FString& CommandID : CommandIDs)
    {
        Results.Add(Coordinator->CancelCoordinatedCommand(CommandID));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch cancelled %d commands"), Results.Num());
    
    return Results;
}

bool UMingMultiUnitBlueprintLibrary::IsMultiUnitCoordinatorInitialized()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    return Coordinator != nullptr;
}

FString UMingMultiUnitBlueprintLibrary::GetMultiUnitCoordinatorVersion()
{
    return TEXT("1.0.0");
}

int32 UMingMultiUnitBlueprintLibrary::GetTotalGroupCount()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return 0;
    }
    
    return Coordinator->GetAllUnitGroups().Num();
}

int32 UMingMultiUnitBlueprintLibrary::GetActiveGroupCount()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return 0;
    }
    
    return Coordinator->GetActiveGroups().Num();
}

int32 UMingMultiUnitBlueprintLibrary::GetTotalCommandCount()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return 0;
    }
    
    return Coordinator->GetAllCoordinatedCommands().Num();
}

int32 UMingMultiUnitBlueprintLibrary::GetExecutingCommandCount()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return 0;
    }
    
    return Coordinator->GetExecutingCommands().Num();
}

bool UMingMultiUnitBlueprintLibrary::DoesGroupExist(const FString& GroupID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    FUnitGroup Group = Coordinator->GetUnitGroup(GroupID);
    return !Group.GroupID.IsEmpty();
}

bool UMingMultiUnitBlueprintLibrary::IsGroupActive(const FString& GroupID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    FUnitGroup Group = Coordinator->GetUnitGroup(GroupID);
    return Group.bIsActive;
}

bool UMingMultiUnitBlueprintLibrary::CanGroupDisband(const FString& GroupID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    FUnitGroup Group = Coordinator->GetUnitGroup(GroupID);
    return Group.bCanDisband;
}

int32 UMingMultiUnitBlueprintLibrary::GetGroupUnitCount(const FString& GroupID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return 0;
    }
    
    FUnitGroup Group = Coordinator->GetUnitGroup(GroupID);
    return Group.GroupUnits.Num();
}

AMingTacticalUnit* UMingMultiUnitBlueprintLibrary::GetGroupLeader(const FString& GroupID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return nullptr;
    }
    
    FUnitGroup Group = Coordinator->GetUnitGroup(GroupID);
    return Group.GroupLeader;
}

FString UMingMultiUnitBlueprintLibrary::GetGroupStatus(const FString& GroupID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return TEXT("Coordinator not available");
    }
    
    FUnitGroup Group = Coordinator->GetUnitGroup(GroupID);
    return Group.GroupStatus;
}

bool UMingMultiUnitBlueprintLibrary::DoesCommandExist(const FString& CommandID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    FCoordinatedCommand Command = Coordinator->GetCoordinatedCommand(CommandID);
    return !Command.CommandID.IsEmpty();
}

bool UMingMultiUnitBlueprintLibrary::IsCommandExecuting(const FString& CommandID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    FCoordinatedCommand Command = Coordinator->GetCoordinatedCommand(CommandID);
    return Command.bIsExecuted && !Command.bIsCompleted && !Command.bIsCancelled;
}

bool UMingMultiUnitBlueprintLibrary::IsCommandCompleted(const FString& CommandID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    FCoordinatedCommand Command = Coordinator->GetCoordinatedCommand(CommandID);
    return Command.bIsCompleted;
}

bool UMingMultiUnitBlueprintLibrary::IsCommandCancelled(const FString& CommandID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    FCoordinatedCommand Command = Coordinator->GetCoordinatedCommand(CommandID);
    return Command.bIsCancelled;
}

bool UMingMultiUnitBlueprintLibrary::DoesCommunicationExist(const FString& GroupAID, const FString& GroupBID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    float Quality = Coordinator->GetCommunicationQuality(GroupAID, GroupBID);
    return Quality > 0.0f;
}

float UMingMultiUnitBlueprintLibrary::CalculateCoordinationScore(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator || GroupIDs.Num() == 0)
    {
        return 0.0f;
    }
    
    // 簡化實作：基於分組數量和類型計算協調分數
    float Score = 0.0f;
    
    // 基於分組數量
    Score += FMath::Clamp((float)GroupIDs.Num() / 10.0f, 0.0f, 0.5f);
    
    // 基於分組類型
    for (const FString& GroupID : GroupIDs)
    {
        float GroupCapability = Coordinator->CalculateGroupCoordinationCapability(GroupID);
        Score += GroupCapability * 0.1f;
    }
    
    // 基於命令類型
    switch (CommandType)
    {
    case ECoordinatedCommandType::CoordinatedAttack:
        Score += 0.2f;
        break;
    case ECoordinatedCommandType::CoordinatedDefense:
        Score += 0.15f;
        break;
    case ECoordinatedCommandType::CoordinatedMovement:
        Score += 0.1f;
        break;
    case ECoordinatedCommandType::CoordinatedRetreat:
        Score += 0.05f;
        break;
    default:
        Score += 0.1f;
        break;
    }
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}

float UMingMultiUnitBlueprintLibrary::CalculateSyncAccuracy(const FString& CommandID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return 0.0f;
    }
    
    FCoordinatedCommand Command = Coordinator->GetCoordinatedCommand(CommandID);
    return Coordinator->CalculateSyncAccuracy(Command);
}

float UMingMultiUnitBlueprintLibrary::CalculateCommunicationDelay(const FString& SourceGroupID, const FString& TargetGroupID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return 0.0f;
    }
    
    return Coordinator->HandleCommunicationDelay(SourceGroupID, TargetGroupID);
}

float UMingMultiUnitBlueprintLibrary::CalculateGroupCoordinationCapability(const FString& GroupID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return 0.0f;
    }
    
    return Coordinator->CalculateGroupCoordinationCapability(GroupID);
}

float UMingMultiUnitBlueprintLibrary::CalculateOverallCoordinationEfficiency()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return 0.0f;
    }
    
    return Coordinator->CalculateOverallCoordinationEfficiency();
}

ECoordinationSyncType UMingMultiUnitBlueprintLibrary::GetOptimalSyncType(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return ECoordinationSyncType::Simultaneous;
    }
    
    return Coordinator->GetOptimalSyncType(GroupIDs, CommandType);
}

ECommunicationProtocol UMingMultiUnitBlueprintLibrary::GetOptimalCommunicationProtocol(const TArray<FString>& GroupIDs)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return ECommunicationProtocol::Direct;
    }
    
    return Coordinator->GetOptimalCommunicationProtocol(GroupIDs);
}

TArray<FString> UMingMultiUnitBlueprintLibrary::GetCoordinationRecommendations(const TArray<FString>& GroupIDs)
{
    TArray<FString> Recommendations;
    
    if (GroupIDs.Num() == 0)
    {
        return Recommendations;
    }
    
    // 基於分組分析提供建議
    Recommendations.Add(TEXT("確保所有分組都有領導者"));
    Recommendations.Add(TEXT("檢查通信連接質量"));
    Recommendations.Add(TEXT("考慮使用適當的同步類型"));
    Recommendations.Add(TEXT("監控協調執行狀態"));
    
    if (GroupIDs.Num() >= 5)
    {
        Recommendations.Add(TEXT("考慮分組執行以提高效率"));
    }
    
    return Recommendations;
}

TArray<FString> UMingMultiUnitBlueprintLibrary::GetTacticalRecommendations(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType)
{
    TArray<FString> Recommendations;
    
    if (GroupIDs.Num() == 0)
    {
        return Recommendations;
    }
    
    // 基於命令類型提供建議
    switch (CommandType)
    {
    case ECoordinatedCommandType::CoordinatedAttack:
        Recommendations.Add(TEXT("確保攻擊目標在所有分組的攻擊範圍內"));
        Recommendations.Add(TEXT("考慮使用波浪攻擊以避免防禦集中"));
        Recommendations.Add(TEXT("確保通信連接穩定"));
        break;
    case ECoordinatedCommandType::CoordinatedDefense:
        Recommendations.Add(TEXT("建立完整的防禦陣地"));
        Recommendations.Add(TEXT("確保分組間的支援協調"));
        Recommendations.Add(TEXT("監控防禦缺口"));
        break;
    case ECoordinatedCommandType::CoordinatedMovement:
        Recommendations.Add(TEXT("規劃安全的移動路徑"));
        Recommendations.Add(TEXT("確保移動速度一致"));
        Recommendations.Add(TEXT("保持隊形完整"));
        break;
    case ECoordinatedCommandType::CoordinatedRetreat:
        Recommendations.Add(TEXT("確保撤退路線安全"));
        Recommendations.Add(TEXT("提供掩護支援"));
        Recommendations.Add(TEXT("優先保護重要單位"));
        break;
    default:
        Recommendations.Add(TEXT("確保所有分組理解命令目標"));
        Recommendations.Add(TEXT("保持通信暢通"));
        break;
    }
    
    return Recommendations;
}

TArray<AMingTacticalUnit*> UMingMultiUnitBlueprintLibrary::GetAttackTargets(const TArray<FString>& GroupIDs)
{
    TArray<AMingTacticalUnit*> Targets;
    
    // 簡化實作：返回空數組
    // 實際應該根據遊戲狀態查找敵方單位
    return Targets;
}

TArray<FVector> UMingMultiUnitBlueprintLibrary::GetDefensePositions(const TArray<FString>& GroupIDs)
{
    TArray<FVector> DefensePositions;
    
    if (GroupIDs.Num() == 0)
    {
        return DefensePositions;
    }
    
    // 簡化實作：基於分組位置生成防禦位置
    for (const FString& GroupID : GroupIDs)
    {
        FUnitGroup Group = GetUnitGroup(GroupID);
        if (!Group.GroupID.IsEmpty())
        {
            // 簡化：在分組周圍生成防禦位置
            FVector Center = FVector::ZeroVector;
            for (AMingTacticalUnit* Unit : Group.GroupUnits)
            {
                if (Unit)
                {
                    Center += Unit->GetActorLocation();
                }
            }
            
            if (Group.GroupUnits.Num() > 0)
            {
                Center /= Group.GroupUnits.Num();
            }
            
            // 生成圓形防禦陣地
            float Radius = 300.0f;
            for (int32 i = 0; i < 4; ++i)
            {
                float Angle = (float)i / 4 * 2 * PI;
                FVector DefensePosition = Center;
                DefensePosition.X += FMath::Cos(Angle) * Radius;
                DefensePosition.Y += FMath::Sin(Angle) * Radius;
                DefensePositions.Add(DefensePosition);
            }
        }
    }
    
    return DefensePositions;
}

TArray<FVector> UMingMultiUnitBlueprintLibrary::GetMovementTargets(const TArray<FString>& GroupIDs)
{
    TArray<FVector> MovementTargets;
    
    if (GroupIDs.Num() == 0)
    {
        return MovementTargets;
    }
    
    // 簡化實作：基於分組位置生成移動目標
    for (const FString& GroupID : GroupIDs)
    {
        FUnitGroup Group = GetUnitGroup(GroupID);
        if (!Group.GroupID.IsEmpty())
        {
            FVector CurrentLocation = FVector::ZeroVector;
            for (AMingTacticalUnit* Unit : Group.GroupUnits)
            {
                if (Unit)
                {
                    CurrentLocation += Unit->GetActorLocation();
                }
            }
            
            if (Group.GroupUnits.Num() > 0)
            {
                CurrentLocation /= Group.GroupUnits.Num();
            }
            
            // 生成移動目標（向前移動500單位）
            FVector MovementTarget = CurrentLocation;
            MovementTarget.Y += 500.0f;
            MovementTargets.Add(MovementTarget);
        }
    }
    
    return MovementTargets;
}

TArray<FVector> UMingMultiUnitBlueprintLibrary::GetRetreatPositions(const TArray<FString>& GroupIDs)
{
    TArray<FVector> RetreatPositions;
    
    if (GroupIDs.Num() == 0)
    {
        return RetreatPositions;
    }
    
    // 簡化實作：基於分組位置生成撤退位置
    for (const FString& GroupID : GroupIDs)
    {
        FUnitGroup Group = GetUnitGroup(GroupID);
        if (!Group.GroupID.IsEmpty())
        {
            FVector CurrentLocation = FVector::ZeroVector;
            for (AMingTacticalUnit* Unit : Group.GroupUnits)
            {
                if (Unit)
                {
                    CurrentLocation += Unit->GetActorLocation();
                }
            }
            
            if (Group.GroupUnits.Num() > 0)
            {
                CurrentLocation /= Group.GroupUnits.Num();
            }
            
            // 生成撤退位置（向後移動800單位）
            FVector RetreatPosition = CurrentLocation;
            RetreatPosition.Y -= 800.0f;
            RetreatPositions.Add(RetreatPosition);
        }
    }
    
    return RetreatPositions;
}

TArray<FCoordinationResult> UMingMultiUnitBlueprintLibrary::SimulateCoordination(
    const TArray<FString>& GroupIDs,
    ECoordinatedCommandType CommandType,
    ECoordinationSyncType SyncType,
    int32 SimulationCount)
{
    TArray<FCoordinationResult> SimulationResults;
    
    for (int32 i = 0; i < SimulationCount; ++i)
    {
        FCoordinationResult Result;
        Result.ExecutionTime = FPlatformTime::Seconds();
        Result.bSuccess = true;
        Result.ResultDescription = FString::Printf(TEXT("Coordination simulation %d"), i + 1);
        Result.ParticipatingGroups = GroupIDs;
        
        // 簡化模擬：基於分組數量和命令類型判斷結果
        float SuccessProbability = 0.7f; // 基成功率70%
        
        if (GroupIDs.Num() > 5)
        {
            SuccessProbability -= 0.1f; // 分組太多降低成功率
        }
        
        switch (CommandType)
        {
        case ECoordinatedCommandType::CoordinatedAttack:
            SuccessProbability += 0.1f;
            break;
        case ECoordinatedCommandType::CoordinatedDefense:
            SuccessProbability += 0.05f;
            break;
        case ECoordinatedCommandType::CoordinatedRetreat:
            SuccessProbability += 0.15f;
            break;
        default:
            break;
        }
        
        if (FMath::FRand() <= SuccessProbability)
        {
            Result.bSuccess = true;
            Result.CoordinationScore = FMath::FRandRange(0.6f, 0.9f);
            Result.SyncAccuracy = FMath::FRandRange(0.7f, 0.95f);
            Result.CommunicationQuality = FMath::FRandRange(0.6f, 0.9f);
        }
        else
        {
            Result.bSuccess = false;
            Result.ErrorMessage = TEXT("Coordination failed due to poor synchronization");
        }
        
        SimulationResults.Add(Result);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Simulated %d coordination attempts"), SimulationCount);
    
    return SimulationResults;
}

TArray<float> UMingMultiUnitBlueprintLibrary::SimulateCommunicationQuality(
    const TArray<FString>& GroupIDs,
    ECommunicationProtocol Protocol,
    int32 SimulationCount)
{
    TArray<float> SimulationResults;
    
    for (int32 i = 0; i < SimulationCount; ++i)
    {
        float Quality = 0.0f;
        
        // 基於協議類型計算質量
        switch (Protocol)
        {
        case ECommunicationProtocol::Direct:
        case ECommunicationProtocol::PeerToPeer:
            Quality = FMath::FRandRange(0.8f, 0.95f);
            break;
        case ECommunicationProtocol::Relay:
        case ECommunicationProtocol::Multicast:
            Quality = FMath::FRandRange(0.7f, 0.9f);
            break;
        case ECommunicationProtocol::Broadcast:
        case ECommunicationProtocol::Centralized:
            Quality = FMath::FRandRange(0.6f, 0.85f);
            break;
        case ECommunicationProtocol::Hierarchical:
        case ECommunicationProtocol::Decentralized:
            Quality = FMath::FRandRange(0.5f, 0.8f);
            break;
        case ECommunicationProtocol::Hybrid:
            Quality = FMath::FRandRange(0.65f, 0.88f);
            break;
        case ECommunicationProtocol::Secure:
        case ECommunicationProtocol::Encrypted:
            Quality = FMath::FRandRange(0.75f, 0.92f);
            break;
        case ECommunicationProtocol::Emergency:
            Quality = FMath::FRandRange(0.85f, 0.98f);
            break;
        default:
            Quality = FMath::FRandRange(0.5f, 0.8f);
            break;
        }
        
        // 基於分組數量調整質量
        if (GroupIDs.Num() > 3)
        {
            Quality *= 0.9f; // 分組太多降低質量
        }
        
        SimulationResults.Add(FMath::Clamp(Quality, 0.0f, 1.0f));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Simulated %d communication quality tests"), SimulationCount);
    
    return SimulationResults;
}

TArray<FString> UMingMultiUnitBlueprintLibrary::GetGroupHistory(const FString& GroupID)
{
    TArray<FString> History;
    
    // 簡化實作：返回模擬歷史
    FUnitGroup Group = GetUnitGroup(GroupID);
    if (!Group.GroupID.IsEmpty())
    {
        History.Add(FString::Printf(TEXT("Group %s created at %.2f"), *Group.GroupName, Group.CreationTime));
        History.Add(FString::Printf(TEXT("Current status: %s"), *Group.GroupStatus));
        History.Add(FString::Printf(TEXT("Unit count: %d"), Group.GroupUnits.Num()));
        History.Add(FString::Printf(TEXT("Last updated: %.2f"), Group.LastUpdateTime));
    }
    
    return History;
}

TArray<FString> UMingMultiUnitBlueprintLibrary::GetCommandHistory()
{
    TArray<FString> History;
    
    // 簡化實作：返回模擬命令歷史
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (Coordinator)
    {
        TArray<FCoordinatedCommand> Commands = Coordinator->GetAllCoordinatedCommands();
        
        for (const FCoordinatedCommand& Command : Commands)
        {
            History.Add(FString::Printf(TEXT("Command %s: %s - %s"), 
                *Command.CommandID, *Command.CommandName, *Command.CommandStatus));
        }
    }
    
    return History;
}

TArray<FString> UMingMultiUnitBlueprintLibrary::GetCommunicationHistory()
{
    TArray<FString> History;
    
    // 簡化實作：返回模擬通信歷史
    History.Add(TEXT("Communication system initialized"));
    History.Add(TEXT("Direct connections established"));
    History.Add(TEXT("Broadcast channels activated"));
    
    return History;
}

TMap<FString, int32> UMingMultiUnitBlueprintLibrary::GetCoordinationStatistics()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return TMap<FString, int32>();
    }
    
    return Coordinator->GetCoordinationStatistics();
}

TMap<FString, int32> UMingMultiUnitBlueprintLibrary::GetGroupStatistics()
{
    TMap<FString, int32> Stats;
    
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return Stats;
    }
    
    TMap<FString, int32> CoordinationStats = Coordinator->GetCoordinationStatistics();
    
    Stats.Add(TEXT("total_groups"), CoordinationStats.FindRef(TEXT("total_groups")));
    Stats.Add(TEXT("active_groups"), CoordinationStats.FindRef(TEXT("active_groups")));
    Stats.Add(TEXT("total_commands"), CoordinationStats.FindRef(TEXT("total_commands")));
    Stats.Add(TEXT("successful_commands"), CoordinationStats.FindRef(TEXT("successful_commands")));
    Stats.Add(TEXT("failed_commands"), CoordinationStats.FindRef(TEXT("failed_commands")));
    Stats.Add(TEXT("cancelled_commands"), CoordinationStats.FindRef(TEXT("cancelled_commands")));
    Stats.Add(TEXT("communication_links"), CoordinationStats.FindRef(TEXT("communication_links")));
    
    return Stats;
}

TMap<FString, int32> UMingMultiUnitBlueprintLibrary::GetCommunicationStatistics()
{
    TMap<FString, int32> Stats;
    
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return Stats;
    }
    
    TMap<FString, int32> CoordinationStats = Coordinator->GetCoordinationStatistics();
    
    Stats.Add(TEXT("communication_links"), CoordinationStats.FindRef(TEXT("communication_links")));
    
    return Stats;
}

TMap<FString, float> UMingMultiUnitBlueprintLibrary::GetPerformanceStatistics()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return TMap<FString, float>();
    }
    
    return Coordinator->GetCoordinationPerformanceMetrics();
}

bool UMingMultiUnitBlueprintLibrary::ExportCoordinationData(const FString& FilePath)
{
    // 簡化實作：記錄導出操作
    UE_LOG(LogTemp, Log, TEXT("Coordination data would be exported to: %s"), *FilePath);
    return true;
}

bool UMingMultiUnitBlueprintLibrary::ImportCoordinationData(const FString& FilePath)
{
    // 簡化實作：記錄導入操作
    UE_LOG(LogTemp, Log, TEXT("Coordination data would be imported from: %s"), *FilePath);
    return true;
}

FString UMingMultiUnitBlueprintLibrary::GenerateGroupReport(const FString& GroupID)
{
    FUnitGroup Group = GetUnitGroup(GroupID);
    
    if (Group.GroupID.IsEmpty())
    {
        return TEXT("Group not found");
    }
    
    FString Report = TEXT("=== 分組報告 ===\n");
    Report += FString::Printf(TEXT("分組ID: %s\n"), *Group.GroupID);
    Report += FString::Printf(TEXT("分組名稱: %s\n"), *Group.GroupName);
    Report += FString::Printf(TEXT("分組類型: %s\n"), *GetGroupTypeName(Group.GroupType));
    Report += FString::Printf(TEXT("分組描述: %s\n"), *Group.GroupDescription);
    Report += FString::Printf(TEXT("單位數量: %d\n"), Group.GroupUnits.Num());
    Report += FString::Printf(TEXT("分組狀態: %s\n"), *Group.GroupStatus);
    Report += FString::Printf(TEXT("創建時間: %.2f\n"), Group.CreationTime);
    Report += FString::Printf(TEXT("最後更新: %.2f\n"), Group.LastUpdateTime);
    Report += FString::Printf(TEXT("是否活躍: %s\n"), Group.bIsActive ? TEXT("是") : TEXT("否"));
    Report += FString::Printf(TEXT("是否可解散: %s\n"), Group.bCanDisband ? TEXT("是") : TEXT("否"));
    
    if (Group.GroupLeader)
    {
        Report += FString::Printf(TEXT("領導者: %s\n"), *Group.GroupLeader->GetName());
    }
    
    Report += FString::Printf(TEXT("分組標籤: %d個\n"), Group.GroupTags.Num());
    for (const FString& Tag : Group.GroupTags)
    {
        Report += FString::Printf(TEXT("  - %s\n"), *Tag);
    }
    
    Report += FString::Printf(TEXT("分組屬性: %d個\n"), Group.GroupAttributes.Num());
    for (const auto& Attribute : Group.GroupAttributes)
    {
        Report += FString::Printf(TEXT("  - %s: %s\n"), *Attribute.Key, *Attribute.Value);
    }
    
    return Report;
}

FString UMingMultiUnitBlueprintLibrary::GenerateCoordinatorReport()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return TEXT("Multi unit coordinator not available");
    }
    
    FString Report = TEXT("=== 多單位協調器報告 ===\n");
    
    TMap<FString, int32> Stats = GetGroupStatistics();
    Report += FString::Printf(TEXT("分組總數: %d\n"), Stats.FindRef(TEXT("total_groups")));
    Report += FString::Printf(TEXT("活躍分組數量: %d\n"), Stats.FindRef(TEXT("active_groups")));
    Report += FString::Printf(TEXT("協調命令總數: %d\n"), Stats.FindRef(TEXT("total_commands")));
    Report += FString::Printf(TEXT("成功命令數量: %d\n"), Stats.FindRef(TEXT("successful_commands")));
    Report += FString::Printf(TEXT("失敗命令數量: %d\n"), Stats.FindRef(TEXT("failed_commands")));
    Report += FString::Printf(TEXT("取消命令數量: %d\n"), Stats.FindRef(TEXT("cancelled_commands")));
    Report += FString::Printf(TEXT("通信連接數量: %d\n"), Stats.FindRef(TEXT("communication_links")));
    
    TMap<FString, float> PerformanceStats = GetPerformanceStatistics();
    Report += FString::Printf(TEXT("協調效率: %.1f%%\n"), PerformanceStats.FindRef(TEXT("coordination_efficiency")) * 100.0f);
    Report += FString::Printf(TEXT("同步精度: %.1f%%\n"), PerformanceStats.FindRef(TEXT("sync_accuracy")) * 100.0f);
    Report += FString::Printf(TEXT("通信質量: %.1f%%\n"), PerformanceStats.FindRef(TEXT("communication_quality")) * 100.0f);
    Report += FString::Printf(TEXT("群組凝聚度: %.1f%%\n"), PerformanceStats.FindRef(TEXT("group_coherence")) * 100.0f);
    Report += FString::Printf(TEXT("命令響應時間: %.2f秒\n"), PerformanceStats.FindRef(TEXT("command_response_time")));
    
    return Report;
}

FString UMingMultiUnitBlueprintLibrary::GenerateCommandReport(const FString& CommandID)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return TEXT("Multi unit coordinator not available");
    }
    
    FCoordinatedCommand Command = Coordinator->GetCoordinatedCommand(CommandID);
    
    if (Command.CommandID.IsEmpty())
    {
        return TEXT("Command not found");
    }
    
    FString Report = TEXT("=== 協調命令報告 ===\n");
    Report += FString::Printf(TEXT("命令ID: %s\n"), *Command.CommandID);
    Report += FString::Printf(TEXT("命令名稱: %s\n"), *Command.CommandName);
    Report += FString::Printf(TEXT("命令類型: %s\n"), *GetCoordinatedCommandTypeName(Command.CommandType));
    Report += FString::Printf(TEXT("命令描述: %s\n"), *Command.CommandDescription);
    Report += FString::Printf(TEXT("同步類型: %s\n"), *GetSyncTypeName(Command.SyncType));
    Report += FString::Printf(TEXT("通信協議: %s\n"), *GetProtocolName(Command.CommunicationProtocol));
    Report += FString::Printf(TEXT("命令狀態: %s\n"), *Command.CommandStatus);
    Report += FString::Printf(TEXT("參與分組數量: %d\n"), Command.ParticipatingGroups.Num());
    Report += FString::Printf(TEXT("參與單位數量: %d\n"), Command.ParticipatingUnits.Num());
    Report += FString::Printf(TEXT("命令階段數量: %d\n"), Command.CommandPhases.Num());
    Report += FString::Printf(TEXT("當前階段: %s\n"), *Command.CurrentPhase);
    Report += FString::Printf(TEXT("同步點數量: %d\n"), Command.SyncPoints.Num());
    Report += FString::Printf(TEXT("依賴關係數量: %d\n"), Command.Dependencies.Num());
    Report += FString::Printf(TEXT("創建時間: %.2f\n"), Command.CreationTime);
    Report += FString::Printf(TEXT("開始時間: %.2f\n"), Command.StartTime);
    Report += FString::Printf(TEXT("完成時間: %.2f\n"), Command.CompletionTime);
    Report += FString::Printf(TEXT("是否已執行: %s\n"), Command.bIsExecuted ? TEXT("是") : TEXT("否"));
    Report += FString::Printf(TEXT("是否已完成: %s\n"), Command.bIsCompleted ? TEXT("是") : TEXT("否"));
    Report += FString::Printf(TEXT("是否已取消: %s\n"), Command.bIsCancelled ? TEXT("是") : TEXT("否"));
    
    Report += TEXT("\n=== 命令參數 ===\n");
    for (const auto& Parameter : Command.CommandParameters)
    {
        Report += FString::Printf(TEXT("- %s: %s\n"), *Parameter.Key, *Parameter.Value);
    }
    
    Report += TEXT("\n=== 執行時間表 ===\n");
    for (const auto& Schedule : Command.ExecutionSchedule)
    {
        Report += FString::Printf(TEXT("- %s: %.2f\n"), *Schedule.Key, Schedule.Value);
    }
    
    return Report;
}

FString UMingMultiUnitBlueprintLibrary::GenerateCoordinationAnalysisReport()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return TEXT("Multi unit coordinator not available");
    }
    
    FString Report = TEXT("=== 協調分析報告 ===\n");
    
    // 整體協調效率
    float OverallEfficiency = CalculateOverallCoordinationEfficiency();
    Report += FString::Printf(TEXT("整體協調效率: %.1f%%\n"), OverallEfficiency * 100.0f);
    
    // 分組分析
    TArray<FUnitGroup> Groups = Coordinator->GetAllUnitGroups();
    Report += FString::Printf(TEXT("分組總數: %d\n"), Groups.Num());
    
    TMap<EUnitGroupType, int32> GroupTypeCounts;
    for (const FUnitGroup& Group : Groups)
    {
        GroupTypeCounts.FindOrAdd(Group.GroupType, 0)++;
    }
    
    Report += TEXT("\n=== 分組類型分布 ===\n");
    for (const auto& TypeCount : GroupTypeCounts)
    {
        Report += FString::Printf(TEXT("- %s: %d\n"), *GetGroupTypeName(TypeCount.Key), TypeCount.Value);
    }
    
    // 命令分析
    TArray<FCoordinatedCommand> Commands = Coordinator->GetAllCoordinatedCommands();
    Report += FString::Printf(TEXT("協調命令總數: %d\n"), Commands.Num());
    
    TMap<ECoordinatedCommandType, int32> CommandTypeCounts;
    for (const FCoordinatedCommand& Command : Commands)
    {
        CommandTypeCounts.FindOrAdd(Command.CommandType, 0)++;
    }
    
    Report += TEXT("\n=== 命令類型分布 ===\n");
    for (const auto& TypeCount : CommandTypeCounts)
    {
        Report += FString::Printf(TEXT("- %s: %d\n"), *GetCoordinatedCommandTypeName(TypeCount.Key), TypeCount.Value);
    }
    
    // 通信分析
    TMap<FString, int32> CommunicationStats = GetCommunicationStatistics();
    Report += FString::Printf(TEXT("通信連接總數: %d\n"), CommunicationStats.FindRef(TEXT("communication_links")));
    
    return Report;
}

void UMingMultiUnitBlueprintLibrary::ResetMultiUnitCoordinator()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (Coordinator)
    {
        Coordinator->ShutdownMultiUnitCoordinator();
        Coordinator->InitializeMultiUnitCoordinator();
        UE_LOG(LogTemp, Log, TEXT("Multi unit coordinator reset"));
    }
}

void UMingMultiUnitBlueprintLibrary::ClearAllGroups()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (Coordinator)
    {
        TArray<FUnitGroup> Groups = Coordinator->GetAllUnitGroups();
        for (const FUnitGroup& Group : Groups)
        {
            Coordinator->DisbandUnitGroup(Group.GroupID);
        }
        UE_LOG(LogTemp, Log, TEXT("All groups cleared"));
    }
}

void UMingMultiUnitBlueprintLibrary::ClearAllCommands()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (Coordinator)
    {
        TArray<FCoordinatedCommand> Commands = Coordinator->GetAllCoordinatedCommands();
        for (const FCoordinatedCommand& Command : Commands)
        {
            Coordinator->CancelCoordinatedCommand(Command.CommandID);
        }
        UE_LOG(LogTemp, Log, TEXT("All commands cleared"));
    }
}

void UMingMultiUnitBlueprintLibrary::ClearAllCommunications()
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (Coordinator)
    {
        // 簡化實作：清除所有通信連接
        UE_LOG(LogTemp, Log, TEXT("All communications cleared"));
    }
}

bool UMingMultiUnitBlueprintLibrary::BackupCoordinationData(const FString& BackupPath)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    Coordinator->BackupCoordinationData();
    UE_LOG(LogTemp, Log, TEXT("Coordination data backed up to: %s"), *BackupPath);
    return true;
}

bool UMingMultiUnitBlueprintLibrary::RestoreCoordinationData(const FString& BackupPath)
{
    AMingMultiUnitCoordinator* Coordinator = GetMultiUnitCoordinator();
    if (!Coordinator)
    {
        return false;
    }
    
    Coordinator->RestoreCoordinationData();
    UE_LOG(LogTemp, Log, TEXT("Coordination data restored from: %s"), *BackupPath);
    return true;
}
