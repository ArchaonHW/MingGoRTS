#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingMultiUnitCoordinator.h"
#include "MingMultiUnitBlueprintLibrary.generated.h"

class AMingTacticalUnit;

/**
 * ?Æ‰??∏Á??ÖË?ÁµêÊ? (?®Êñº TArray<TArray<>> ÂµåÂ?)
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
 * Â§öÂñÆ‰ΩçË??ñÂáΩ?∏Â∫´
 * ?ê‰??çÂ??ØË™ø?®Á?Â§öÂñÆ‰ΩçÂ?Ë™øÁ≥ªÁµ±Â??? */
UCLASS()
class MINGTACTICAL_API UMingMultiUnitBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?≤Â?Â§öÂñÆ‰ΩçÂ?Ë™øÂô®
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static AMingMultiUnitCoordinator* GetMultiUnitCoordinator();

    /**
     * ?ùÂ??ñÂ??Æ‰??îË™ø??     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static void InitializeMultiUnitCoordinator();

    /**
     * ?úÈ?Â§öÂñÆ‰ΩçÂ?Ë™øÂô®
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static void ShutdownMultiUnitCoordinator();

    /**
     * ?µÂª∫?Æ‰??ÜÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FUnitGroup CreateUnitGroup(
        const FString& GroupName,
        EUnitGroupType GroupType,
        const TArray<AMingTacticalUnit*>& Units,
        AMingTacticalUnit* Leader = nullptr
    );

    /**
     * ?µÂª∫?Æ‰??ÜÁ? (?πÈ?)
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Batch")
    static TArray<FUnitGroup> BatchCreateGroups(
        const TArray<FString>& GroupNames,
        const TArray<EUnitGroupType>& GroupTypes,
        const TArray<FUnitArrayWrapper>& UnitArrays
    );

    /**
     * Ëß?ï£?Æ‰??ÜÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool DisbandUnitGroup(const FString& GroupID);

    /**
     * ?≤Â??Æ‰??ÜÁ?
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FUnitGroup GetUnitGroup(const FString& GroupID);

    /**
     * ?≤Â??Ä?âÂñÆ‰ΩçÂ?Áµ?     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static TArray<FUnitGroup> GetAllUnitGroups();

    /**
     * ?≤Â??áÂ?È°ûÂ??ÑÂ?Áµ?     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static TArray<FUnitGroup> GetGroupsByType(EUnitGroupType GroupType);

    /**
     * ?≤Â?Ê¥ªË??ÜÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static TArray<FUnitGroup> GetActiveGroups();

    /**
     * Ê∑ªÂ??Æ‰??∞Â?Áµ?     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool AddUnitToGroup(const FString& GroupID, AMingTacticalUnit* Unit);

    /**
     * ÂæûÂ?ÁµÑÁßª?§ÂñÆ‰Ω?     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool RemoveUnitFromGroup(const FString& GroupID, AMingTacticalUnit* Unit);

    /**
     * Ë®≠ÁΩÆ?ÜÁ??òÂ???     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool SetGroupLeader(const FString& GroupID, AMingTacticalUnit* Leader);

    /**
     * ?ºÂ??îË™ø?Ω‰ª§
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult IssueCoordinatedCommand(const FCoordinatedCommand& Command);

    /**
     * ?∑Ë??îË™ø?ªÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedAttack(
        const TArray<FString>& GroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    );

    /**
     * ?∑Ë??îË™ø?≤Á¶¶
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedDefense(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& DefensePositions,
        ECoordinationSyncType SyncType
    );

    /**
     * ?∑Ë??îË™øÁßªÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedMovement(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& TargetPositions,
        ECoordinationSyncType SyncType
    );

    /**
     * ?∑Ë??îË™ø?§ÈÄÄ
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedRetreat(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& RetreatPositions,
        ECoordinationSyncType SyncType
    );

    /**
     * ?∑Ë??îË™ø?ØÊè¥
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedSupport(
        const TArray<FString>& SupportGroupIDs,
        const TArray<FString>& TargetGroupIDs,
        ECoordinationSyncType SyncType
    );

    /**
     * ?∑Ë??îË™ø‰ºèÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedAmbush(
        const TArray<FString>& AmbusherGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    );

    /**
     * ?∑Ë??îË™ø?¥Áøº?ªÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedFlanking(
        const TArray<FString>& FlankerGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    );

    /**
     * ?∑Ë??îË™ø?ÖÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedEncirclement(
        const TArray<FString>& EncirclerGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    );

    /**
     * ?∑Ë??îË™øÁ™ÅÁ†¥
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedBreakthrough(
        const TArray<FString>& BreakthroughGroupIDs,
        const TArray<FVector>& BreakthroughPoints,
        ECoordinationSyncType SyncType
    );

    /**
     * ?ñÊ??îË™ø?Ω‰ª§
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool CancelCoordinatedCommand(const FString& CommandID);

    /**
     * ?≤Â??îË™ø?Ω‰ª§
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FCoordinatedCommand GetCoordinatedCommand(const FString& CommandID);

    /**
     * ?≤Â??Ä?âÂ?Ë™øÂëΩ‰ª?     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static TArray<FCoordinatedCommand> GetAllCoordinatedCommands();

    /**
     * ?≤Â??∑Ë?‰∏≠Á??Ω‰ª§
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static TArray<FCoordinatedCommand> GetExecutingCommands();

    /**
     * Ê™¢Êü•?îË™ø?Ä??     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetCoordinationStatus(const FString& CommandID);

    /**
     * ?≤Â??îË™øÁµ±Ë?
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static TMap<FString, int32> GetCoordinationStats_BPLibrary();

    /**
     * ?≤Â??îË™ø?ßËÉΩ?áÊ?
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static TMap<FString, float> GetCoordinationPerformanceMetrics();

    /**
     * ?åÊ≠•?Æ‰??Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool SynchronizeUnitStates(const TArray<AMingTacticalUnit*>& Units);

    /**
     * Âª?í≠Ê∂àÊÅØ?∞Â?Áµ?     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool BroadcastToGroup(const FString& GroupID, const FString& Message);

    /**
     * ‰∏≠ÁπºÊ∂àÊÅØ?∞Â?Áµ?     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool RelayMessage(const FString& SourceGroupID, const FString& TargetGroupID, const FString& Message);

    /**
     * Âª∫Á??ö‰ø°??é•
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool EstablishCommunication(const FString& GroupAID, const FString& GroupBID, ECommunicationProtocol Protocol);

    /**
     * ?∑È??ö‰ø°??é•
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool DisconnectCommunication(const FString& GroupAID, const FString& GroupBID);

    /**
     * ?≤Â??ö‰ø°Ë≥™È?
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static float GetCommunicationQuality(const FString& GroupAID, const FString& GroupBID);

    /**
     * ?≤Â??ÜÁ?È°ûÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetGroupTypeName(EUnitGroupType GroupType);

    /**
     * ?≤Â??îË™ø?Ω‰ª§È°ûÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetCoordinatedCommandTypeName(ECoordinatedCommandType CommandType);

    /**
     * ?≤Â??åÊ≠•È°ûÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetSyncTypeName(ECoordinationSyncType SyncType);

    /**
     * ?≤Â??ö‰ø°?îË≠∞?çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetProtocolName(ECommunicationProtocol Protocol);

    /**
     * ?µÂª∫?îË™ø?Ω‰ª§
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
     * Ë®≠ÁΩÆ?Ω‰ª§?ÉÊï∏
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand SetCommandParameter(const FCoordinatedCommand& Command, const FString& ParameterName, const FString& ParameterValue);

    /**
     * Ê∑ªÂ??Ω‰ª§?éÊÆµ
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand AddCommandPhase(const FCoordinatedCommand& Command, const FString& PhaseName);

    /**
     * Ë®≠ÁΩÆ?∑Ë??ÇÈ?Ë°?     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand SetExecutionSchedule(const FCoordinatedCommand& Command, const TMap<FString, float>& Schedule);

    /**
     * Ê∑ªÂ??åÊ≠•Èª?     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand AddSyncPoint(const FCoordinatedCommand& Command, const FString& SyncPoint);

    /**
     * Ê∑ªÂ?‰æùË≥¥?ú‰?
     */
    UFUNCTION(Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand AddDependency(const FCoordinatedCommand& Command, const FString& Dependency);

    /**
     * ?πÈ??µÂª∫?ÜÁ?
     */
    static TArray<FUnitGroup> BatchCreateGroups(
        const TArray<FString>& GroupNames,
        const TArray<EUnitGroupType>& GroupTypes,
        const TArray<TArray<AMingTacticalUnit*>>& UnitArrays
    );

    /**
     * ?πÈ??∑Ë??îË™ø?Ω‰ª§
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Batch")
    static TArray<FCoordinationResult> BatchExecuteCommands(const TArray<FCoordinatedCommand>& Commands);

    /**
     * ?πÈ??ñÊ??Ω‰ª§
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Batch")
    static TArray<bool> BatchCancelCommands(const TArray<FString>& CommandIDs);

    /**
     * ?≤Â?Á≥ªÁµ±?Ä??     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static bool IsMultiUnitCoordinatorInitialized();

    /**
     * ?≤Â?Á≥ªÁµ±?àÊú¨
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static FString GetMultiUnitCoordinatorVersion();

    /**
     * ?≤Â??ÜÁ?Á∏ΩÊï∏
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static int32 GetTotalGroupCount();

    /**
     * ?≤Â?Ê¥ªË??ÜÁ??∏È?
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static int32 GetActiveGroupCount();

    /**
     * ?≤Â??îË™ø?Ω‰ª§Á∏ΩÊï∏
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static int32 GetTotalCommandCount();

    /**
     * ?≤Â??∑Ë?‰∏≠ÂëΩ‰ª§Êï∏??     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static int32 GetExecutingCommandCount();

    /**
     * Ê™¢Êü•?ÜÁ??ØÂê¶Â≠òÂú®
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool DoesGroupExist(const FString& GroupID);

    /**
     * Ê™¢Êü•?ÜÁ??ØÂê¶Ê¥ªË?
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool IsGroupActive(const FString& GroupID);

    /**
     * Ê™¢Êü•?ÜÁ??ØÂê¶?ØËß£??     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool CanGroupDisband(const FString& GroupID);

    /**
     * ?≤Â??ÜÁ??Æ‰??∏È?
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static int32 GetGroupUnitCount(const FString& GroupID);

    /**
     * ?≤Â??ÜÁ??òÂ???     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static AMingTacticalUnit* GetGroupLeader(const FString& GroupID);

    /**
     * ?≤Â??ÜÁ??Ä??     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static FString GetGroupStatus(const FString& GroupID);

    /**
     * Ê™¢Êü•?Ω‰ª§?ØÂê¶Â≠òÂú®
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool DoesCommandExist(const FString& CommandID);

    /**
     * Ê™¢Êü•?Ω‰ª§?ØÂê¶Ê≠?ú®?∑Ë?
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool IsCommandExecuting(const FString& CommandID);

    /**
     * Ê™¢Êü•?Ω‰ª§?ØÂê¶Â∑≤Â???     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool IsCommandCompleted(const FString& CommandID);

    /**
     * Ê™¢Êü•?Ω‰ª§?ØÂê¶Â∑≤Â?Ê∂?     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool IsCommandCancelled(const FString& CommandID);

    /**
     * Ê™¢Êü•?ö‰ø°??é•?ØÂê¶Â≠òÂú®
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool DoesCommunicationExist(const FString& GroupAID, const FString& GroupBID);

    /**
     * Ë®àÁ??îË™ø?ÜÊï∏
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateCoordinationScore(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType);

    /**
     * Ë®àÁ??åÊ≠•Á≤æÂ∫¶
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateSyncAccuracy(const FString& CommandID);

    /**
     * Ë®àÁ??ö‰ø°Âª∂ÈÅ≤
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateCommunicationDelay(const FString& SourceGroupID, const FString& TargetGroupID);

    /**
     * Ë®àÁ??ÜÁ??îË™ø?ΩÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateGroupCoordinationCapability(const FString& GroupID);

    /**
     * Ë®àÁ??¥È??îË™ø?àÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateOverallCoordinationEfficiency();

    /**
     * ?≤Â??Ä‰Ω≥Â?Ê≠•È???     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Optimization")
    static ECoordinationSyncType GetOptimalSyncType(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType);

    /**
     * ?≤Â??Ä‰Ω≥ÈÄö‰ø°?îË≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Optimization")
    static ECommunicationProtocol GetOptimalCommunicationProtocol(const TArray<FString>& GroupIDs);

    /**
     * ?≤Â??îË™øÂª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FString> GetCoordinationRecommendations(const TArray<FString>& GroupIDs);

    /**
     * ?≤Â??∞Ë?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FString> GetTacticalRecommendations(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType);

    /**
     * ?≤Â??ªÊ?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<AMingTacticalUnit*> GetAttackTargets(const TArray<FString>& GroupIDs);

    /**
     * ?≤Â??≤Á¶¶Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FVector> GetDefensePositions(const TArray<FString>& GroupIDs);

    /**
     * ?≤Â?ÁßªÂ?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FVector> GetMovementTargets(const TArray<FString>& GroupIDs);

    /**
     * ?≤Â??§ÈÄÄÂª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FVector> GetRetreatPositions(const TArray<FString>& GroupIDs);

    /**
     * Ê®°Êì¨?îË™øÁµêÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Simulation")
    static TArray<FCoordinationResult> SimulateCoordination(
        const TArray<FString>& GroupIDs,
        ECoordinatedCommandType CommandType,
        ECoordinationSyncType SyncType,
        int32 SimulationCount
    );

    /**
     * Ê®°Êì¨?ö‰ø°?àÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Simulation")
    static TArray<float> SimulateCommunicationQuality(
        const TArray<FString>& GroupIDs,
        ECommunicationProtocol Protocol,
        int32 SimulationCount
    );

    /**
     * ?≤Â??ÜÁ?Ê≠∑Âè≤
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|History")
    static TArray<FString> GetGroupHistory(const FString& GroupID);

    /**
     * ?≤Â??Ω‰ª§Ê≠∑Âè≤
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|History")
    static TArray<FString> GetCommandHistory();

    /**
     * ?≤Â??ö‰ø°Ê≠∑Âè≤
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|History")
    static TArray<FString> GetCommunicationHistory();

    /**
     * ?≤Â??îË™øÁµ±Ë?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Statistics")
    static TMap<FString, int32> GetCoordinationStats_BPLibrary();

    /**
     * ?≤Â??ÜÁ?Áµ±Ë?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Statistics")
    static TMap<FString, int32> GetGroupStatistics();

    /**
     * ?≤Â??ö‰ø°Áµ±Ë?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Statistics")
    static TMap<FString, int32> GetCommunicationStatistics();

    /**
     * ?≤Â??ßËÉΩÁµ±Ë?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Statistics")
    static TMap<FString, float> GetPerformanceStatistics();

    /**
     * Â∞éÂá∫?îË™ø?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Export")
    static bool ExportCoordinationData(const FString& FilePath);

    /**
     * Â∞éÂÖ•?îË™ø?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Import")
    static bool ImportCoordinationData(const FString& FilePath);

    /**
     * ?≤Â??ÜÁ??±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Report")
    static FString GenerateGroupReport(const FString& GroupID);

    /**
     * ?≤Â??îË™ø?®Â†±??     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Report")
    static FString GenerateCoordinatorReport();

    /**
     * ?≤Â??Ω‰ª§?±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Report")
    static FString GenerateCommandReport(const FString& CommandID);

    /**
     * ?≤Â??îË™ø?ÜÊ??±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Report")
    static FString GenerateCoordinationAnalysisReport();

    /**
     * ?çÁΩÆÂ§öÂñÆ‰ΩçÂ?Ë™øÂô®
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static void ResetMultiUnitCoordinator();

    /**
     * Ê∏ÖÈô§?Ä?âÂ?Áµ?     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static void ClearAllGroups();

    /**
     * Ê∏ÖÈô§?Ä?âÂëΩ‰ª?     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static void ClearAllCommands();

    /**
     * Ê∏ÖÈô§?Ä?âÈÄö‰ø°??é•
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static void ClearAllCommunications();

    /**
     * ?ô‰ªΩ?îË™ø?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static bool BackupCoordinationData(const FString& BackupPath);

    /**
     * ?¢Âæ©?îË™ø?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static bool RestoreCoordinationData(const FString& BackupPath);
};
