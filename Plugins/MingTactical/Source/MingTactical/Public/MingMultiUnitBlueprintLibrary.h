#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingMultiUnitCoordinator.h"
#include "MingMultiUnitBlueprintLibrary.generated.h"

class AMingTacticalUnit;

/**
 * ???X??X???結?? (??於 TArray<TArray<>> 嵌??)
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
 * 多單位?X?函??庫
 * ???X??X?調????多單位??調系統?X */
UCLASS()
class MINGTACTICAL_API UMingMultiUnitBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ????多單位??調器
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static AMingMultiUnitCoordinator* GetMultiUnitCoordinator();

    /**
     * ???X??X??X?調X     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static void InitializeMultiUnitCoordinator();

    /**
     * ????多單位??調器
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static void ShutdownMultiUnitCoordinator();

    /**
     * ??建???X???
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FUnitGroup CreateUnitGroup(
        const FString& GroupName,
        EUnitGroupType GroupType,
        const TArray<AMingTacticalUnit*>& Units,
        AMingTacticalUnit* Leader = nullptr
    };

    /**
     * ??建???X??? (????)
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Batch")
    static TArray<FUnitGroup> BatchCreateGroups(
        const TArray<FString>& GroupNames,
        const TArray<EUnitGroupType>& GroupTypes,
        const TArray<FUnitArrayWrapper>& UnitArrays
    };

    /**
     * ???????X???
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool DisbandUnitGroup(const FString& GroupID};

    /**
     * ???X??X???
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FUnitGroup GetUnitGroup(const FString& GroupID};

    /**
     * ???X???單位????     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static TArray<FUnitGroup> GetAllUnitGroups();

    /**
     * ???X???類?X?????     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static TArray<FUnitGroup> GetGroupsByType(EUnitGroupType GroupType};

    /**
     * ????活?X???
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static TArray<FUnitGroup> GetActiveGroups();

    /**
     * 添?X??X?????     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool AddUnitToGroup(const FString& GroupID, AMingTacticalUnit* Unit};

    /**
     * 從??組移??單??     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool RemoveUnitFromGroup(const FString& GroupID, AMingTacticalUnit* Unit};

    /**
     * 設置???X??X     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool SetGroupLeader(const FString& GroupID, AMingTacticalUnit* Leader};

    /**
     * ???X?調??令
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult IssueCoordinatedCommand(const FCoordinatedCommand& Command};

    /**
     * ???X?調????
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedAttack(
        const TArray<FString>& GroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    };

    /**
     * ???X?調??禦
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedDefense(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& DefensePositions,
        ECoordinationSyncType SyncType
    };

    /**
     * ???X?調移??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedMovement(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& TargetPositions,
        ECoordinationSyncType SyncType
    };

    /**
     * ???X?調??退
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedRetreat(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& RetreatPositions,
        ECoordinationSyncType SyncType
    };

    /**
     * ???X?調??援
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedSupport(
        const TArray<FString>& SupportGroupIDs,
        const TArray<FString>& TargetGroupIDs,
        ECoordinationSyncType SyncType
    };

    /**
     * ???X?調伏??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedAmbush(
        const TArray<FString>& AmbusherGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    };

    /**
     * ???X?調??翼????
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedFlanking(
        const TArray<FString>& FlankerGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    };

    /**
     * ???X?調????
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedEncirclement(
        const TArray<FString>& EncirclerGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    };

    /**
     * ???X?調突破
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedBreakthrough(
        const TArray<FString>& BreakthroughGroupIDs,
        const TArray<FVector>& BreakthroughPoints,
        ECoordinationSyncType SyncType
    };

    /**
     * ???X?調??令
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool CancelCoordinatedCommand(const FString& CommandID};

    /**
     * ???X?調??令
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FCoordinatedCommand GetCoordinatedCommand(const FString& CommandID};

    /**
     * ???X?????調命??     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static TArray<FCoordinatedCommand> GetAllCoordinatedCommands();

    /**
     * ???X???中?X?令
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static TArray<FCoordinatedCommand> GetExecutingCommands();

    /**
     * 檢查??調??X     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetCoordinationStatus(const FString& CommandID};

    /**
     * ???X?調統??
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static TMap<FString, int32> GetCoordinationStats_BPLibrary();

    /**
     * ???X?調??能????
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static TMap<FString, float> GetCoordinationPerformanceMetrics();

    /**
     * ??步???X?X     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool SynchronizeUnitStates(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ????消息??????     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool BroadcastToGroup(const FString& GroupID, const FString& Message};

    /**
     * 中繼消息??????     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool RelayMessage(const FString& SourceGroupID, const FString& TargetGroupID, const FString& Message};

    /**
     * 建?X?信X??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool EstablishCommunication(const FString& GroupAID, const FString& GroupBID, ECommunicationProtocol Protocol};

    /**
     * ???X?信X??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool DisconnectCommunication(const FString& GroupAID, const FString& GroupBID};

    /**
     * ???X?信質??
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static float GetCommunicationQuality(const FString& GroupAID, const FString& GroupBID};

    /**
     * ???X???類?X?稱
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetGroupTypeName(EUnitGroupType GroupType};

    /**
     * ???X?調??令類?X?稱
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetCoordinatedCommandTypeName(ECoordinatedCommandType CommandType};

    /**
     * ???X?步類?X?稱
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetSyncTypeName(ECoordinationSyncType SyncType};

    /**
     * ???X?信??議??稱
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetProtocolName(ECommunicationProtocol Protocol};

    /**
     * ??建??調??令
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand CreateCoordinatedCommand(
        const FString& CommandName,
        ECoordinatedCommandType CommandType,
        const TArray<FString>& ParticipatingGroups,
        ECoordinationSyncType SyncType,
        ECommunicationProtocol CommunicationProtocol
    };

    /**
     * 設置??令??數
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand SetCommandParameter(const FCoordinatedCommand& Command, const FString& ParameterName, const FString& ParameterValue};

    /**
     * 添?X?令??段
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand AddCommandPhase(const FCoordinatedCommand& Command, const FString& PhaseName};

    /**
     * 設置???X?????     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand SetExecutionSchedule(const FCoordinatedCommand& Command, const TMap<FString, float>& Schedule};

    /**
     * 添?X?步??     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand AddSyncPoint(const FCoordinatedCommand& Command, const FString& SyncPoint};

    /**
     * 添??依賴????
     */
    UFUNCTION(Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand AddDependency(const FCoordinatedCommand& Command, const FString& Dependency};

    /**
     * ???X?建????
     */
    static TArray<FUnitGroup> BatchCreateGroups(
        const TArray<FString>& GroupNames,
        const TArray<EUnitGroupType>& GroupTypes,
        const TArray<TArray<AMingTacticalUnit*>>& UnitArrays
    };

    /**
     * ???X??X?調??令
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Batch")
    static TArray<FCoordinationResult> BatchExecuteCommands(const TArray<FCoordinatedCommand>& Commands};

    /**
     * ???X??X?令
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Batch")
    static TArray<bool> BatchCancelCommands(const TArray<FString>& CommandIDs};

    /**
     * ????系統??X     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static bool IsMultiUnitCoordinatorInitialized();

    /**
     * ????系統??本
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static FString GetMultiUnitCoordinatorVersion();

    /**
     * ???X???總數
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static int32 GetTotalGroupCount();

    /**
     * ????活?X??X???
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static int32 GetActiveGroupCount();

    /**
     * ???X?調??令總數
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static int32 GetTotalCommandCount();

    /**
     * ???X???中命令數X     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static int32 GetExecutingCommandCount();

    /**
     * 檢查???X?否存在
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool DoesGroupExist(const FString& GroupID};

    /**
     * 檢查???X?否活??
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool IsGroupActive(const FString& GroupID};

    /**
     * 檢查???X?否??解X     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool CanGroupDisband(const FString& GroupID};

    /**
     * ???X??X??X???
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static int32 GetGroupUnitCount(const FString& GroupID};

    /**
     * ???X??X??X     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static AMingTacticalUnit* GetGroupLeader(const FString& GroupID};

    /**
     * ???X??X?X     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static FString GetGroupStatus(const FString& GroupID};

    /**
     * 檢查??令??否存在
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool DoesCommandExist(const FString& CommandID};

    /**
     * 檢查??令??否????????
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool IsCommandExecuting(const FString& CommandID};

    /**
     * 檢查??令??否已?X     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool IsCommandCompleted(const FString& CommandID};

    /**
     * 檢查??令??否已????     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool IsCommandCancelled(const FString& CommandID};

    /**
     * 檢查??信X????否存在
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool DoesCommunicationExist(const FString& GroupAID, const FString& GroupBID};

    /**
     * 計?X?調??數
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateCoordinationScore(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType};

    /**
     * 計?X?步精度
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateSyncAccuracy(const FString& CommandID};

    /**
     * 計?X?信延遲
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateCommunicationDelay(const FString& SourceGroupID, const FString& TargetGroupID};

    /**
     * 計?X??X?調????
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateGroupCoordinationCapability(const FString& GroupID};

    /**
     * 計?X??X?調????
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateOverallCoordinationEfficiency();

    /**
     * ???X?佳??步?X     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Optimization")
    static ECoordinationSyncType GetOptimalSyncType(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType};

    /**
     * ???X?佳通信??議
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Optimization")
    static ECommunicationProtocol GetOptimalCommunicationProtocol(const TArray<FString>& GroupIDs};

    /**
     * ???X?調建議
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FString> GetCoordinationRecommendations(const TArray<FString>& GroupIDs};

    /**
     * ???X???建議
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FString> GetTacticalRecommendations(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType};

    /**
     * ???X???建議
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<AMingTacticalUnit*> GetAttackTargets(const TArray<FString>& GroupIDs};

    /**
     * ???X?禦建議
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FVector> GetDefensePositions(const TArray<FString>& GroupIDs};

    /**
     * ????移??建議
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FVector> GetMovementTargets(const TArray<FString>& GroupIDs};

    /**
     * ???X?退建議
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FVector> GetRetreatPositions(const TArray<FString>& GroupIDs};

    /**
     * 模擬??調結??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Simulation")
    static TArray<FCoordinationResult> SimulateCoordination(
        const TArray<FString>& GroupIDs,
        ECoordinatedCommandType CommandType,
        ECoordinationSyncType SyncType,
        int32 SimulationCount
    };

    /**
     * 模擬??信????
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Simulation")
    static TArray<float> SimulateCommunicationQuality(
        const TArray<FString>& GroupIDs,
        ECommunicationProtocol Protocol,
        int32 SimulationCount
    };

    /**
     * ???X???歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|History")
    static TArray<FString> GetGroupHistory(const FString& GroupID};

    /**
     * ???X?令歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|History")
    static TArray<FString> GetCommandHistory();

    /**
     * ???X?信歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|History")
    static TArray<FString> GetCommunicationHistory();

    /**
     * ???X?調統??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Statistics")
    static TMap<FString, int32> GetCoordinationStats_BPLibrary();

    /**
     * ???X???統??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Statistics")
    static TMap<FString, int32> GetGroupStatistics();

    /**
     * ???X?信統??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Statistics")
    static TMap<FString, int32> GetCommunicationStatistics();

    /**
     * ???X?能統??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Statistics")
    static TMap<FString, float> GetPerformanceStatistics();

    /**
     * 導出??調????
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Export")
    static bool ExportCoordinationData(const FString& FilePath};

    /**
     * 導入??調????
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Import")
    static bool ImportCoordinationData(const FString& FilePath};

    /**
     * ???X??X???
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Report")
    static FString GenerateGroupReport(const FString& GroupID};

    /**
     * ???X?調??報X     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Report")
    static FString GenerateCoordinatorReport();

    /**
     * ???X?令????
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Report")
    static FString GenerateCommandReport(const FString& CommandID};

    /**
     * ???X?調???X???
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Report")
    static FString GenerateCoordinationAnalysisReport();

    /**
     * ??置多單位??調器
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static void ResetMultiUnitCoordinator();

    /**
     * 清除????????     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static void ClearAllGroups();

    /**
     * 清除????命??     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static void ClearAllCommands();

    /**
     * 清除????通信X??
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static void ClearAllCommunications();

    /**
     * ??份??調????
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static bool BackupCoordinationData(const FString& BackupPath};

    /**
     * ??復??調????
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static bool RestoreCoordinationData(const FString& BackupPath};
};

