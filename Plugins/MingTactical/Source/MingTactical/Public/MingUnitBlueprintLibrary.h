#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingUnitController.h"
#include "MingUnitBlueprintLibrary.generated.h"

class AMingTacticalUnit;

/**
 * 單位藍圖函數庫
 * 提供藍圖可調用的單位控制系統功能
 */
UCLASS()
class MINGTACTICAL_API UMingUnitBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * 獲取單位控制器
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static AMingUnitController* GetUnitController();

    /**
     * 初始化單位控制器
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static void InitializeUnitController();

    /**
     * 關閉單位控制器
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static void ShutdownUnitController();

    /**
     * 添加受控單位
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool AddControlledUnit(AMingTacticalUnit* Unit);

    /**
     * 移除受控單位
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool RemoveControlledUnit(AMingTacticalUnit* Unit);

    /**
     * 獲取所有受控單位
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static TArray<AMingTacticalUnit*> GetControlledUnits();

    /**
     * 獲指定類型的單位
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static TArray<AMingTacticalUnit*> GetUnitsByType(const FString& UnitType);

    /**
     * 獲取活躍單位
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static TArray<AMingTacticalUnit*> GetActiveUnits();

    /**
     * 發布單位命令
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult IssueUnitCommand(const FUnitCommand& Command);

    /**
     * 批量發布命令
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static TArray<FUnitControlResult> BatchIssueCommands(const TArray<FUnitCommand>& Commands);

    /**
     * 取消命令
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool CancelCommand(const FString& CommandID);

    /**
     * 取消所有命令
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool CancelAllCommands();

    /**
     * 移動單位到位置
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult MoveUnitsToLocation(const TArray<AMingTacticalUnit*>& Units, const FVector& TargetLocation, EUnitMovementType MovementType);

    /**
     * 攻擊目標
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult AttackTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetUnit, EUnitAttackType AttackType);

    /**
     * 攻擊建築
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult AttackBuilding(const TArray<AMingTacticalUnit*>& Units, AActor* TargetBuilding, EUnitAttackType AttackType);

    /**
     * 停止所有動作
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult StopUnits(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 防禦位置
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult HoldPosition(const TArray<AMingTacticalUnit*>& Units, const FVector& HoldLocation);

    /**
     * 巡邏路徑
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult PatrolPath(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& PatrolPoints);

    /**
     * 守護目標
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult GuardTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetToGuard);

    /**
     * 撤退
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult RetreatUnits(const TArray<AMingTacticalUnit*>& Units, const FVector& RetreatLocation);

    /**
     * 組織編隊
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult FormUpUnits(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& FormationCenter);

    /**
     * 使用技能
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult UseAbility(const TArray<AMingTacticalUnit*>& Units, const FString& AbilityID, AMingTacticalUnit* TargetUnit = nullptr);

    /**
     * 獲取單位狀態
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static EMingUnitState GetUnitState(AMingTacticalUnit* Unit);

    /**
     * 設置單位狀態
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool SetUnitState(AMingTacticalUnit* Unit, EMingUnitState NewState);

    /**
     * 獲取單位命令隊列
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static TArray<FUnitCommand> GetUnitCommandQueue(AMingTacticalUnit* Unit);

    /**
     * 獲取當前命令
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FUnitCommand GetCurrentCommand(AMingTacticalUnit* Unit);

    /**
     * 檢查單位是否可執行命令
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static bool CanExecuteCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    /**
     * 獲取單位控制統計
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static TMap<FString, int32> GetUnitControlStatistics();

    /**
     * 獲取控制器性能指標
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static TMap<FString, float> GetPerformanceMetrics();

    /**
     * 獲取狀態名稱
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetStateName(EMingUnitState State);

    /**
     * 獲取命令類型名稱
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetCommandTypeName(EUnitCommandType CommandType);

    /**
     * 獲取命令優先級名稱
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetPriorityName(EUnitCommandPriority Priority);

    /**
     * 獲取移動類型名稱
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetMovementTypeName(EUnitMovementType MovementType);

    /**
     * 獲取攻擊類型名稱
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetAttackTypeName(EUnitAttackType AttackType);

    /**
     * 獲取編隊類型名稱
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetFormationTypeName(EUnitFormationType FormationType);

    /**
     * 創建單位命令
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Creation")
    static FUnitCommand CreateUnitCommand(
        EUnitCommandType CommandType,
        EUnitCommandPriority Priority,
        const FVector& TargetLocation,
        AMingTacticalUnit* TargetUnit,
        AActor* TargetBuilding,
        EUnitMovementType MovementType,
        EUnitAttackType AttackType,
        EUnitFormationType FormationType
    );

    /**
     * 設置命令參數
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Creation")
    static FUnitCommand SetCommandParameter(const FUnitCommand& Command, const FString& ParameterName, const FString& ParameterValue);

    /**
     * 設置命令重試次數
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Creation")
    static FUnitCommand SetCommandRetryCount(const FUnitCommand& Command, int32 MaxRetryCount);

    /**
     * 批量移動單位
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Batch")
    static TArray<FUnitControlResult> BatchMoveUnits(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& TargetLocations, EUnitMovementType MovementType);

    /**
     * 批量攻擊目標
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Batch")
    static TArray<FUnitControlResult> BatchAttackTargets(const TArray<AMingTacticalUnit*>& Units, const TArray<AMingTacticalUnit*>& TargetUnits, EUnitAttackType AttackType);

    /**
     * 批量停止單位
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Batch")
    static TArray<FUnitControlResult> BatchStopUnits(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 獲取系統狀態
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static bool IsUnitControllerInitialized();

    /**
     * 獲取系統版本
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static FString GetUnitControllerVersion();

    /**
     * 獲取受控單位數量
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static int32 GetControlledUnitCount();

    /**
     * 獲取活躍單位數量
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static int32 GetActiveUnitCount();

    /**
     * 獲取命令總數
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static int32 GetTotalCommandCount();

    /**
     * 獲取執行中命令數量
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static int32 GetExecutingCommandCount();

    /**
     * 檢查單位是否受控
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitControlled(AMingTacticalUnit* Unit);

    /**
     * 檢查單位是否活躍
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitActive(AMingTacticalUnit* Unit);

    /**
     * 檢查單位是否空閒
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitIdle(AMingTacticalUnit* Unit);

    /**
     * 檢查單位是否在移動
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitMoving(AMingTacticalUnit* Unit);

    /**
     * 檢查單位是否在攻擊
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitAttacking(AMingTacticalUnit* Unit);

    /**
     * 檢查單位是否在防禦
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitDefending(AMingTacticalUnit* Unit);

    /**
     * 獲取單位位置
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static FVector GetUnitLocation(AMingTacticalUnit* Unit);

    /**
     * 獲取單位旋轉
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static FRotator GetUnitRotation(AMingTacticalUnit* Unit);

    /**
     * 設置單位位置
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Info")
    static bool SetUnitLocation(AMingTacticalUnit* Unit, const FVector& NewLocation);

    /**
     * 設置單位旋轉
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Info")
    static bool SetUnitRotation(AMingTacticalUnit* Unit, const FRotator& NewRotation);

    /**
     * 計算兩點距離
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Math")
    static float CalculateDistance(const FVector& PointA, const FVector& PointB);

    /**
     * 計算單位間距離
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Math")
    static float CalculateUnitDistance(AMingTacticalUnit* UnitA, AMingTacticalUnit* UnitB);

    /**
     * 檢查是否在範圍內
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Math")
    static bool IsInRange(const FVector& PointA, const FVector& PointB, float Range);

    /**
     * 檢查單位是否在攻擊範圍內
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Math")
    static bool IsUnitInAttackRange(AMingTacticalUnit* Attacker, AMingTacticalUnit* Target);

    /**
     * 獲取移動速度
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Calculation")
    static float GetMovementSpeed(AMingTacticalUnit* Unit, EUnitMovementType MovementType);

    /**
     * 計算移動時間
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Calculation")
    static float CalculateMovementTime(AMingTacticalUnit* Unit, const FVector& StartLocation, const FVector& TargetLocation, EUnitMovementType MovementType);

    /**
     * 計算攻擊傷害
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Calculation")
    static float CalculateAttackDamage(AMingTacticalUnit* Attacker, AMingTacticalUnit* Target, EUnitAttackType AttackType);

    /**
     * 計算編隊位置
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Formation")
    static TArray<FVector> CalculateFormationPositions(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& Center);

    /**
     * 檢查編隊完整性
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Formation")
    static bool IsFormationComplete(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions);

    /**
     * 更新編隊
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Formation")
    static bool UpdateFormation(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions);

    /**
     * 獲取單位建議
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<FString> GetUnitRecommendations(AMingTacticalUnit* Unit);

    /**
     * 獲取戰術建議
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<FString> GetTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 獲取攻擊建議
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<AMingTacticalUnit*> GetAttackTargets(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 獲取防禦建議
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<FVector> GetDefensePositions(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 獲取撤離建議
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<FVector> GetRetreatPositions(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 模擬戰鬥結果
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Simulation")
    static TArray<FUnitControlResult> SimulateBattle(const TArray<AMingTacticalUnit*>& Attackers, const TArray<AMingTacticalUnit*>& Defenders, int32 SimulationCount);

    /**
     * 模擬移動路徑
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Simulation")
    static TArray<FVector> SimulateMovementPath(AMingTacticalUnit* Unit, const FVector& StartLocation, const FVector& TargetLocation);

    /**
     * 獲取單位歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|History")
    static TArray<FUnitCommand> GetUnitHistory(AMingTacticalUnit* Unit);

    /**
     * 獲取命令歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|History")
    static TArray<FUnitControlResult> GetCommandHistory();

    /**
     * 獲取戰鬥統計
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Statistics")
    static TMap<FString, int32> GetBattleStatistics();

    /**
     * 獲取單位統計
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Statistics")
    static TMap<FString, int32> GetUnitStatistics();

    /**
     * 獲取性能統計
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Statistics")
    static TMap<FString, float> GetPerformanceStatistics();

    /**
     * 導出單位數據
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Export")
    static bool ExportUnitData(const FString& FilePath);

    /**
     * 導入單位數據
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Import")
    static bool ImportUnitData(const FString& FilePath);

    /**
     * 獲取單位報告
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Report")
    static FString GenerateUnitReport(AMingTacticalUnit* Unit);

    /**
     * 獲取控制器報告
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Report")
    static FString GenerateControllerReport();

    /**
     * 獲取戰鬥報告
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Report")
    static FString GenerateBattleReport(const TArray<AMingTacticalUnit*>& Attackers, const TArray<AMingTacticalUnit*>& Defenders);

    /**
     * 重置單位控制器
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static void ResetUnitController();

    /**
     * 清除所有命令
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static void ClearAllCommands();

    /**
     * 清除單位歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static void ClearUnitHistory();

    /**
     * 備份單位數據
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static bool BackupUnitData(const FString& BackupPath);

    /**
     * 恢復單位數據
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static bool RestoreUnitData(const FString& BackupPath);
};
