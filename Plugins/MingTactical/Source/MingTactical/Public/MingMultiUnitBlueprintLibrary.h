#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingMultiUnitCoordinator.h"
#include "MingMultiUnitBlueprintLibrary.generated.h"

class AMingTacticalUnit;

/**
 * 故事重要性摧毀? (?? TArray<TArray<>> O??)
 */
USTRUCT(BlueprintType)
struct FUnitArrayWrapper
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    TArray<AMingTacticalUnit*> Units;

    FUnitArrayWrapper() {}
};

/**
 * h?X???w
 * 故事重要性摧毀?h??ըt?X */
UCLASS()
class MINGTACTICAL_API UMingMultiUnitBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * 摧毀h??վ
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static AMingMultiUnitCoordinator* GetMultiUnitCoordinator();

    /**
     * 故事選項X?X     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static void InitializeMultiUnitCoordinator();

    /**
     * 摧毀h??վ
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static void ShutdownMultiUnitCoordinator();

    /**
     * ??目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FUnitGroup CreateUnitGroup(
        const FString& GroupName,
        EUnitGroupType GroupType,
        const TArray<AMingTacticalUnit*>& Units,
        AMingTacticalUnit* Leader = nullptr
    );

    /**
     * ??目標數量 (摧毀)
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Batch")
    static TArray<FUnitGroup> BatchCreateGroups(
        const TArray<FString>& GroupNames,
        const TArray<EUnitGroupType>& GroupTypes,
        const TArray<FUnitArrayWrapper>& UnitArrays
    );

    /**
     * 摧毀目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool DisbandUnitGroup(const FString& GroupID);

    /**
     * 故事選項?
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FUnitGroup GetUnitGroup(const FString& GroupID);

    /**
     * 目標數量摧毀     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static TArray<FUnitGroup> GetAllUnitGroups();

    /**
     * 目標數量?X摧毀?     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static TArray<FUnitGroup> GetGroupsByType(EUnitGroupType GroupType);

    /**
     * ??目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static TArray<FUnitGroup> GetActiveGroups();

    /**
     * K?X??X摧毀?     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool AddUnitToGroup(const FString& GroupID, AMingTacticalUnit* Unit);

    /**
     * q??ղ摧毀     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool RemoveUnitFromGroup(const FString& GroupID, AMingTacticalUnit* Unit);

    /**
     * ]m故事重要性     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool SetGroupLeader(const FString& GroupID, AMingTacticalUnit* Leader);

    /**
     * 目標數量O
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult IssueCoordinatedCommand(const FCoordinatedCommand& Command);

    /**
     * 目標數量??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedAttack(
        const TArray<FString>& GroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    );

    /**
     * 目標數量m
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedDefense(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& DefensePositions,
        ECoordinationSyncType SyncType
    );

    /**
     * ???X?ղ??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedMovement(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& TargetPositions,
        ECoordinationSyncType SyncType
    );

    /**
     * 目標數量h
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedRetreat(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& RetreatPositions,
        ECoordinationSyncType SyncType
    );

    /**
     * 目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedSupport(
        const TArray<FString>& SupportGroupIDs,
        const TArray<FString>& TargetGroupIDs,
        ECoordinationSyncType SyncType
    );

    /**
     * ???X?ե??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedAmbush(
        const TArray<FString>& AmbusherGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    );

    /**
     * 目標數量l摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedFlanking(
        const TArray<FString>& FlankerGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    );

    /**
     * 目標數量??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedEncirclement(
        const TArray<FString>& EncirclerGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    );

    /**
     * ???X?լ}
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedBreakthrough(
        const TArray<FString>& BreakthroughGroupIDs,
        const TArray<FVector>& BreakthroughPoints,
        ECoordinationSyncType SyncType
    );

    /**
     * 目標數量O
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool CancelCoordinatedCommand(const FString& CommandID);

    /**
     * 目標數量O
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FCoordinatedCommand GetCoordinatedCommand(const FString& CommandID);

    /**
     * 目標數量??թR??     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static TArray<FCoordinatedCommand> GetAllCoordinatedCommands();

    /**
     * 目標數量?X?O
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static TArray<FCoordinatedCommand> GetExecutingCommands();

    /**
     * ˬd摧毀X     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetCoordinationStatus(const FString& CommandID);

    /**
     * ???X?ղ??
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static TMap<FString, int32> GetCoordinationStats_BPLibrary();

    /**
     * 目標數量摧毀
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static TMap<FString, float> GetCoordinationPerformanceMetrics();

    /**
     * ??B???X?X     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool SynchronizeUnitStates(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 摧毀摧毀??     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool BroadcastToGroup(const FString& GroupID, const FString& Message);

    /**
     * ~摧毀??     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool RelayMessage(const FString& SourceGroupID, const FString& TargetGroupID, const FString& Message);

    /**
     * ?X?HX??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool EstablishCommunication(const FString& GroupAID, const FString& GroupBID, ECommunicationProtocol Protocol);

    /**
     * ???X?HX??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool DisconnectCommunication(const FString& GroupAID, const FString& GroupBID);

    /**
     * ???X?H??
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static float GetCommunicationQuality(const FString& GroupAID, const FString& GroupBID);

    /**
     * 目標數量?X?
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetGroupTypeName(EUnitGroupType GroupType);

    /**
     * 目標數量O?X?
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetCoordinatedCommandTypeName(ECoordinatedCommandType CommandType);

    /**
     * ???X?B?X?
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetSyncTypeName(ECoordinationSyncType SyncType);

    /**
     * ???X?H??ĳ??
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetProtocolName(ECommunicationProtocol Protocol);

    /**
     * 摧毀??O
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand CreateCoordinatedCommand(
        const FString& CommandName,
        ECoordinatedCommandType CommandType,
        const TArray<FString>& ParticipatingGroups,
        ECoordinationSyncType SyncType,
        ECommunicationProtocol CommunicationProtocol
    );

    /**
     * ]m??O??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand SetCommandParameter(const FCoordinatedCommand& Command, const FString& ParameterName, const FString& ParameterValue);

    /**
     * K?X?O??q
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand AddCommandPhase(const FCoordinatedCommand& Command, const FString& PhaseName);

    /**
     * ]m目標數量??     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand SetExecutionSchedule(const FCoordinatedCommand& Command, const TMap<FString, float>& Schedule);

    /**
     * K?X?B??     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand AddSyncPoint(const FCoordinatedCommand& Command, const FString& SyncPoint);

    /**
     * K??̿摧毀
     */
    UFUNCTION(Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand AddDependency(const FCoordinatedCommand& Command, const FString& Dependency);

    /**
     * 目標數量??
     */
    static TArray<FUnitGroup> BatchCreateGroups(
        const TArray<FString>& GroupNames,
        const TArray<EUnitGroupType>& GroupTypes,
        const TArray<TArray<AMingTacticalUnit*>>& UnitArrays
    );

    /**
     * 故事選項?O
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Batch")
    static TArray<FCoordinationResult> BatchExecuteCommands(const TArray<FCoordinatedCommand>& Commands);

    /**
     * 故事重要性?O
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Batch")
    static TArray<bool> BatchCancelCommands(const TArray<FString>& CommandIDs);

    /**
     * 摧毀t??X     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static bool IsMultiUnitCoordinatorInitialized();

    /**
     * 摧毀t??
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static FString GetMultiUnitCoordinatorVersion();

    /**
     * 目標數量`
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static int32 GetTotalGroupCount();

    /**
     * 摧毀?X??X???
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static int32 GetActiveGroupCount();

    /**
     * 目標數量O`
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static int32 GetTotalCommandCount();

    /**
     * 目標數量ROX     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static int32 GetExecutingCommandCount();

    /**
     * ˬd???X?_sb
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool DoesGroupExist(const FString& GroupID);

    /**
     * ˬd???X?_??
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool IsGroupActive(const FString& GroupID);

    /**
     * ˬd???X?_??X     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool CanGroupDisband(const FString& GroupID);

    /**
     * 故事選項X???
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static int32 GetGroupUnitCount(const FString& GroupID);

    /**
     * 故事選項X     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static AMingTacticalUnit* GetGroupLeader(const FString& GroupID);

    /**
     * 故事重要性?X     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static FString GetGroupStatus(const FString& GroupID);

    /**
     * ˬd??O??_sb
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool DoesCommandExist(const FString& CommandID);

    /**
     * ˬd??O??_摧毀摧毀
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool IsCommandExecuting(const FString& CommandID);

    /**
     * ˬd??O??_w?X     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool IsCommandCompleted(const FString& CommandID);

    /**
     * ˬd??O??_w摧毀     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool IsCommandCancelled(const FString& CommandID);

    /**
     * ˬd??HX摧毀_sb
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool DoesCommunicationExist(const FString& GroupAID, const FString& GroupBID);

    /**
     * p?X???
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateCoordinationScore(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType);

    /**
     * p?X?B
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateSyncAccuracy(const FString& CommandID);

    /**
     * p?X?H
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateCommunicationDelay(const FString& SourceGroupID, const FString& TargetGroupID);

    /**
     * p?X??X摧毀?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateGroupCoordinationCapability(const FString& GroupID);

    /**
     * p?X??X摧毀?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateOverallCoordinationEfficiency();

    /**
     * 目標數量B?X     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Optimization")
    static ECoordinationSyncType GetOptimalSyncType(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType);

    /**
     * ???X?γqH??ĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Optimization")
    static ECommunicationProtocol GetOptimalCommunicationProtocol(const TArray<FString>& GroupIDs);

    /**
     * ???X?իĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FString> GetCoordinationRecommendations(const TArray<FString>& GroupIDs);

    /**
     * 目標數量ĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FString> GetTacticalRecommendations(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType);

    /**
     * 目標數量ĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<AMingTacticalUnit*> GetAttackTargets(const TArray<FString>& GroupIDs);

    /**
     * ???X?mĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FVector> GetDefensePositions(const TArray<FString>& GroupIDs);

    /**
     * 摧毀??ĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FVector> GetMovementTargets(const TArray<FString>& GroupIDs);

    /**
     * ???X?hĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FVector> GetRetreatPositions(const TArray<FString>& GroupIDs);

    /**
     * ??յ??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Simulation")
    static TArray<FCoordinationResult> SimulateCoordination(
        const TArray<FString>& GroupIDs,
        ECoordinatedCommandType CommandType,
        ECoordinationSyncType SyncType,
        int32 SimulationCount
    );

    /**
     * ??H摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Simulation")
    static TArray<float> SimulateCommunicationQuality(
        const TArray<FString>& GroupIDs,
        ECommunicationProtocol Protocol,
        int32 SimulationCount
    );

    /**
     * 目標數量v
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|History")
    static TArray<FString> GetGroupHistory(const FString& GroupID);

    /**
     * ???X?Ov
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|History")
    static TArray<FString> GetCommandHistory();

    /**
     * ???X?Hv
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|History")
    static TArray<FString> GetCommunicationHistory();

    /**
     * ???X?ղ??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Statistics")
    static TMap<FString, int32> GetCoordinationStats_BPLibrary();

    /**
     * 目標數量??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Statistics")
    static TMap<FString, int32> GetGroupStatistics();

    /**
     * ???X?H??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Statistics")
    static TMap<FString, int32> GetCommunicationStatistics();

    /**
     * 目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Statistics")
    static TMap<FString, float> GetPerformanceStatistics();

    /**
     * ɥX摧毀??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Export")
    static bool ExportCoordinationData(const FString& FilePath);

    /**
     * ɤJ摧毀??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Import")
    static bool ImportCoordinationData(const FString& FilePath);

    /**
     * 故事選項?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Report")
    static FString GenerateGroupReport(const FString& GroupID);

    /**
     * 目標數量X     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Report")
    static FString GenerateCoordinatorReport();

    /**
     * ???X?O摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Report")
    static FString GenerateCommandReport(const FString& CommandID);

    /**
     * 目標數量?X???
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Report")
    static FString GenerateCoordinationAnalysisReport();

    /**
     * ??mh??վ
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static void ResetMultiUnitCoordinator();

    /**
     * M摧毀摧毀     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static void ClearAllGroups();

    /**
     * M摧毀R??     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static void ClearAllCommands();

    /**
     * M摧毀qHX??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static void ClearAllCommunications();

    /**
     * 摧毀摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static bool BackupCoordinationData(const FString& BackupPath);

    /**
     * ??_摧毀??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static bool RestoreCoordinationData(const FString& BackupPath);
};

