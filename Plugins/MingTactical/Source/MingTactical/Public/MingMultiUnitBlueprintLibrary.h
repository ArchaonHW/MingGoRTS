#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingMultiUnitCoordinator.h"
#include "MingMultiUnitBlueprintLibrary.generated.h"

class AMingTacticalUnit;

/**
 * 多單位藍圖函數庫
 * 提供藍圖可調用的多單位協調系統功能
 */
UCLASS()
class MINGTACTICAL_API UMingMultiUnitBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * 獲取多單位協調器
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static AMingMultiUnitCoordinator* GetMultiUnitCoordinator();

    /**
     * 初始化多單位協調器
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static void InitializeMultiUnitCoordinator();

    /**
     * 關閉多單位協調器
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static void ShutdownMultiUnitCoordinator();

    /**
     * 創建單位分組
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FUnitGroup CreateUnitGroup(
        const FString& GroupName,
        EUnitGroupType GroupType,
        const TArray<AMingTacticalUnit*>& Units,
        AMingTacticalUnit* Leader = nullptr
    );

    /**
     * 解散單位分組
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool DisbandUnitGroup(const FString& GroupID);

    /**
     * 獲取單位分組
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FUnitGroup GetUnitGroup(const FString& GroupID);

    /**
     * 獲取所有單位分組
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static TArray<FUnitGroup> GetAllUnitGroups();

    /**
     * 獲取指定類型的分組
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static TArray<FUnitGroup> GetGroupsByType(EUnitGroupType GroupType);

    /**
     * 獲取活躍分組
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static TArray<FUnitGroup> GetActiveGroups();

    /**
     * 添加單位到分組
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool AddUnitToGroup(const FString& GroupID, AMingTacticalUnit* Unit);

    /**
     * 從分組移除單位
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool RemoveUnitFromGroup(const FString& GroupID, AMingTacticalUnit* Unit);

    /**
     * 設置分組領導者
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool SetGroupLeader(const FString& GroupID, AMingTacticalUnit* Leader);

    /**
     * 發布協調命令
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult IssueCoordinatedCommand(const FCoordinatedCommand& Command);

    /**
     * 執行協調攻擊
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedAttack(
        const TArray<FString>& GroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    );

    /**
     * 執行協調防禦
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedDefense(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& DefensePositions,
        ECoordinationSyncType SyncType
    );

    /**
     * 執行協調移動
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedMovement(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& TargetPositions,
        ECoordinationSyncType SyncType
    );

    /**
     * 執行協調撤退
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedRetreat(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& RetreatPositions,
        ECoordinationSyncType SyncType
    );

    /**
     * 執行協調支援
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedSupport(
        const TArray<FString>& SupportGroupIDs,
        const TArray<FString>& TargetGroupIDs,
        ECoordinationSyncType SyncType
    );

    /**
     * 執行協調伏擊
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedAmbush(
        const TArray<FString>& AmbusherGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    );

    /**
     * 執行協調側翼攻擊
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedFlanking(
        const TArray<FString>& FlankerGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    );

    /**
     * 執行協調包圍
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedEncirclement(
        const TArray<FString>& EncirclerGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    );

    /**
     * 執行協調突破
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static FCoordinationResult ExecuteCoordinatedBreakthrough(
        const TArray<FString>& BreakthroughGroupIDs,
        const TArray<FVector>& BreakthroughPoints,
        ECoordinationSyncType SyncType
    );

    /**
     * 取消協調命令
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool CancelCoordinatedCommand(const FString& CommandID);

    /**
     * 獲取協調命令
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FCoordinatedCommand GetCoordinatedCommand(const FString& CommandID);

    /**
     * 獲取所有協調命令
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static TArray<FCoordinatedCommand> GetAllCoordinatedCommands();

    /**
     * 獲取執行中的命令
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static TArray<FCoordinatedCommand> GetExecutingCommands();

    /**
     * 檢查協調狀態
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetCoordinationStatus(const FString& CommandID);

    /**
     * 獲取協調統計
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static TMap<FString, int32> GetCoordinationStatistics();

    /**
     * 獲取協調性能指標
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static TMap<FString, float> GetCoordinationPerformanceMetrics();

    /**
     * 同步單位狀態
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool SynchronizeUnitStates(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 廣播消息到分組
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool BroadcastToGroup(const FString& GroupID, const FString& Message);

    /**
     * 中繼消息到分組
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool RelayMessage(const FString& SourceGroupID, const FString& TargetGroupID, const FString& Message);

    /**
     * 建立通信連接
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool EstablishCommunication(const FString& GroupAID, const FString& GroupBID, ECommunicationProtocol Protocol);

    /**
     * 斷開通信連接
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    static bool DisconnectCommunication(const FString& GroupAID, const FString& GroupBID);

    /**
     * 獲取通信質量
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static float GetCommunicationQuality(const FString& GroupAID, const FString& GroupBID);

    /**
     * 獲取分組類型名稱
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetGroupTypeName(EUnitGroupType GroupType);

    /**
     * 獲取協調命令類型名稱
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetCoordinatedCommandTypeName(ECoordinatedCommandType CommandType);

    /**
     * 獲取同步類型名稱
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetSyncTypeName(ECoordinationSyncType SyncType);

    /**
     * 獲取通信協議名稱
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    static FString GetProtocolName(ECommunicationProtocol Protocol);

    /**
     * 創建協調命令
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
     * 設置命令參數
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand SetCommandParameter(const FCoordinatedCommand& Command, const FString& ParameterName, const FString& ParameterValue);

    /**
     * 添加命令階段
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand AddCommandPhase(const FCoordinatedCommand& Command, const FString& PhaseName);

    /**
     * 設置執行時間表
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand SetExecutionSchedule(const FCoordinatedCommand& Command, const TMap<FString, float>& Schedule);

    /**
     * 添加同步點
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand AddSyncPoint(const FCoordinatedCommand& Command, const FString& SyncPoint);

    /**
     * 添加依賴關係
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Creation")
    static FCoordinatedCommand AddDependency(const FCoordinatedCommand& Command, const FString& Dependency);

    /**
     * 批量創建分組
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Batch")
    static TArray<FUnitGroup> BatchCreateGroups(
        const TArray<FString>& GroupNames,
        const TArray<EUnitGroupType>& GroupTypes,
        const TArray<TArray<AMingTacticalUnit*>>& UnitArrays
    );

    /**
     * 批量執行協調命令
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Batch")
    static TArray<FCoordinationResult> BatchExecuteCommands(const TArray<FCoordinatedCommand>& Commands);

    /**
     * 批量取消命令
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Batch")
    static TArray<bool> BatchCancelCommands(const TArray<FString>& CommandIDs);

    /**
     * 獲取系統狀態
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static bool IsMultiUnitCoordinatorInitialized();

    /**
     * 獲取系統版本
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static FString GetMultiUnitCoordinatorVersion();

    /**
     * 獲取分組總數
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static int32 GetTotalGroupCount();

    /**
     * 獲取活躍分組數量
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static int32 GetActiveGroupCount();

    /**
     * 獲取協調命令總數
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static int32 GetTotalCommandCount();

    /**
     * 獲取執行中命令數量
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|System")
    static int32 GetExecutingCommandCount();

    /**
     * 檢查分組是否存在
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool DoesGroupExist(const FString& GroupID);

    /**
     * 檢查分組是否活躍
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool IsGroupActive(const FString& GroupID);

    /**
     * 檢查分組是否可解散
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool CanGroupDisband(const FString& GroupID);

    /**
     * 獲取分組單位數量
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static int32 GetGroupUnitCount(const FString& GroupID);

    /**
     * 獲取分組領導者
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static AMingTacticalUnit* GetGroupLeader(const FString& GroupID);

    /**
     * 獲取分組狀態
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static FString GetGroupStatus(const FString& GroupID);

    /**
     * 檢查命令是否存在
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool DoesCommandExist(const FString& CommandID);

    /**
     * 檢查命令是否正在執行
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool IsCommandExecuting(const FString& CommandID);

    /**
     * 檢查命令是否已完成
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool IsCommandCompleted(const FString& CommandID);

    /**
     * 檢查命令是否已取消
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool IsCommandCancelled(const FString& CommandID);

    /**
     * 檢查通信連接是否存在
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Info")
    static bool DoesCommunicationExist(const FString& GroupAID, const FString& GroupBID);

    /**
     * 計算協調分數
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateCoordinationScore(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType);

    /**
     * 計算同步精度
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateSyncAccuracy(const FString& CommandID);

    /**
     * 計算通信延遲
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateCommunicationDelay(const FString& SourceGroupID, const FString& TargetGroupID);

    /**
     * 計算分組協調能力
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateGroupCoordinationCapability(const FString& GroupID);

    /**
     * 計算整體協調效率
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Calculation")
    static float CalculateOverallCoordinationEfficiency();

    /**
     * 獲取最佳同步類型
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Optimization")
    static ECoordinationSyncType GetOptimalSyncType(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType);

    /**
     * 獲取最佳通信協議
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Optimization")
    static ECommunicationProtocol GetOptimalCommunicationProtocol(const TArray<FString>& GroupIDs);

    /**
     * 獲取協調建議
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FString> GetCoordinationRecommendations(const TArray<FString>& GroupIDs);

    /**
     * 獲取戰術建議
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FString> GetTacticalRecommendations(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType);

    /**
     * 獲取攻擊建議
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<AMingTacticalUnit*> GetAttackTargets(const TArray<FString>& GroupIDs);

    /**
     * 獲取防禦建議
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FVector> GetDefensePositions(const TArray<FString>& GroupIDs);

    /**
     * 獲取移動建議
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FVector> GetMovementTargets(const TArray<FString>& GroupIDs);

    /**
     * 獲取撤退建議
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|AI")
    static TArray<FVector> GetRetreatPositions(const TArray<FString>& GroupIDs);

    /**
     * 模擬協調結果
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Simulation")
    static TArray<FCoordinationResult> SimulateCoordination(
        const TArray<FString>& GroupIDs,
        ECoordinatedCommandType CommandType,
        ECoordinationSyncType SyncType,
        int32 SimulationCount
    );

    /**
     * 模擬通信效果
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Simulation")
    static TArray<float> SimulateCommunicationQuality(
        const TArray<FString>& GroupIDs,
        ECommunicationProtocol Protocol,
        int32 SimulationCount
    );

    /**
     * 獲取分組歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|History")
    static TArray<FString> GetGroupHistory(const FString& GroupID);

    /**
     * 獲取命令歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|History")
    static TArray<FString> GetCommandHistory();

    /**
     * 獲取通信歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|History")
    static TArray<FString> GetCommunicationHistory();

    /**
     * 獲取協調統計
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Statistics")
    static TMap<FString, int32> GetCoordinationStatistics();

    /**
     * 獲取分組統計
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Statistics")
    static TMap<FString, int32> GetGroupStatistics();

    /**
     * 獲取通信統計
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Statistics")
    static TMap<FString, int32> GetCommunicationStatistics();

    /**
     * 獲取性能統計
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Statistics")
    static TMap<FString, float> GetPerformanceStatistics();

    /**
     * 導出協調數據
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Export")
    static bool ExportCoordinationData(const FString& FilePath);

    /**
     * 導入協調數據
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Import")
    static bool ImportCoordinationData(const FString& FilePath);

    /**
     * 獲取分組報告
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Report")
    static FString GenerateGroupReport(const FString& GroupID);

    /**
     * 獲取協調器報告
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Report")
    static FString GenerateCoordinatorReport();

    /**
     * 獲取命令報告
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Report")
    static FString GenerateCommandReport(const FString& CommandID);

    /**
     * 獲取協調分析報告
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Report")
    static FString GenerateCoordinationAnalysisReport();

    /**
     * 重置多單位協調器
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static void ResetMultiUnitCoordinator();

    /**
     * 清除所有分組
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static void ClearAllGroups();

    /**
     * 清除所有命令
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static void ClearAllCommands();

    /**
     * 清除所有通信連接
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static void ClearAllCommunications();

    /**
     * 備份協調數據
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static bool BackupCoordinationData(const FString& BackupPath);

    /**
     * 恢復協調數據
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|System")
    static bool RestoreCoordinationData(const FString& BackupPath);
};
