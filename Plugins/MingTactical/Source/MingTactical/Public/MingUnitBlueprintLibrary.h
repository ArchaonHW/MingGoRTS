#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingUnitController.h"
#include "MingUnitBlueprintLibrary.generated.h"

class AMingTacticalUnit;

/**
 * ???X??X?數?? * ???X??X?調???X??X?制系統??能
 */
UCLASS()
class MINGTACTICAL_API UMingUnitBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ???X??X?制X     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static AMingUnitController* GetUnitController();

    /**
     * ???X?單位控??器
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static void InitializeUnitController();

    /**
     * ???X??X?制X     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static void ShutdownUnitController();

    /**
     * 添?X?控????
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool AddControlledUnit(AMingTacticalUnit* Unit};

    /**
     * 移除??控????
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool RemoveControlledUnit(AMingTacticalUnit* Unit};

    /**
     * ???X????X?單??     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static TArray<AMingTacticalUnit*> GetControlledUnits();

    /**
     * ????定?X??X???
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static TArray<AMingTacticalUnit*> GetUnitsByType(const FString& UnitType};

    /**
     * ????活?X???
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static TArray<AMingTacticalUnit*> GetActiveUnits();

    /**
     * ???X??X?令
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult IssueUnitCommand(const FUnitCommand& Command};

    /**
     * ???X??X?令
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static TArray<FUnitControlResult> BatchIssueCommands(const TArray<FUnitCommand>& Commands};

    /**
     * ???X?令
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool CancelCommand(const FString& CommandID};

    /**
     * ???X???命??     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool CancelAllCommands();

    /**
     * 移?X??X?????     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult MoveUnitsToLocation(const TArray<AMingTacticalUnit*>& Units, const FVector& TargetLocation, EUnitMovementType MovementType};

    /**
     * ???X???
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult AttackTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetUnit, EUnitAttackType AttackType};

    /**
     * ????建??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult AttackBuilding(const TArray<AMingTacticalUnit*>& Units, AActor* TargetBuilding, EUnitAttackType AttackType};

    /**
     * ??止????????     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult StopUnits(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ??禦位置
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult HoldPosition(const TArray<AMingTacticalUnit*>& Units, const FVector& HoldLocation};

    /**
     * 巡??路??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult PatrolPath(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& PatrolPoints};

    /**
     * 守護????
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult GuardTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetToGuard};

    /**
     * ??退
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult RetreatUnits(const TArray<AMingTacticalUnit*>& Units, const FVector& RetreatLocation};

    /**
     * 組??編??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult FormUpUnits(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& FormationCenter};

    /**
     * 使用??X     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult UseAbility(const TArray<AMingTacticalUnit*>& Units, const FString& AbilityID, AMingTacticalUnit* TargetUnit = nullptr};

    /**
     * ???X??X?X     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static EMingUnitState GetUnitState(AMingTacticalUnit* Unit};

    /**
     * 設置???X?X     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool SetUnitState(AMingTacticalUnit* Unit, EMingUnitState NewState};

    /**
     * ???X??X?令????
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static TArray<FUnitCommand> GetUnitCommandQueue(AMingTacticalUnit* Unit};

    /**
     * ???X??X?令
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FUnitCommand GetCurrentCommand(AMingTacticalUnit* Unit};

    /**
     * 檢查???X?否??執行命??     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static bool CanExecuteCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    /**
     * ???X??X?制統??
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static TMap<FString, int32> GetUnitControlStatistics();

    /**
     * ???X?制??性能????
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static TMap<FString, float> GetPerformanceMetrics();

    /**
     * ???X???????     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetStateName(EMingUnitState State};

    /**
     * ???X?令類?X?稱
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetCommandTypeName(EUnitCommandType CommandType};

    /**
     * ???X?令????級????     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetPriorityName(EUnitCommandPriority Priority};

    /**
     * ????移??類?X?稱
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetMovementTypeName(EUnitMovementType MovementType};

    /**
     * ???X???類?X?稱
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetAttackTypeName(EUnitAttackType AttackType};

    /**
     * ????編??類?X?稱
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetFormationTypeName(EUnitFormationType FormationType};

    /**
     * ??建???X?令
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
    };

    /**
     * 設置??令??數
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Creation")
    static FUnitCommand SetCommandParameter(const FUnitCommand& Command, const FString& ParameterName, const FString& ParameterValue};

    /**
     * 設置??令??試次數
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Creation")
    static FUnitCommand SetCommandRetryCount(const FUnitCommand& Command, int32 MaxRetryCount};

    /**
     * ????移?X???
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Batch")
    static TArray<FUnitControlResult> BatchMoveUnits(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& TargetLocations, EUnitMovementType MovementType};

    /**
     * ???X??X???
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Batch")
    static TArray<FUnitControlResult> BatchAttackTargets(const TArray<AMingTacticalUnit*>& Units, const TArray<AMingTacticalUnit*>& TargetUnits, EUnitAttackType AttackType};

    /**
     * ???X?止????
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Batch")
    static TArray<FUnitControlResult> BatchStopUnits(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ????系統??X     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static bool IsUnitControllerInitialized();

    /**
     * ????系統??本
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static FString GetUnitControllerVersion();

    /**
     * ???X?控???X???
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static int32 GetControlledUnitCount();

    /**
     * ????活?X??X???
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static int32 GetActiveUnitCount();

    /**
     * ???X?令總數
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static int32 GetTotalCommandCount();

    /**
     * ???X???中命令數X     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static int32 GetExecutingCommandCount();

    /**
     * 檢查???X?否??控
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitControlled(AMingTacticalUnit* Unit};

    /**
     * 檢查???X?否活??
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitActive(AMingTacticalUnit* Unit};

    /**
     * 檢查???X?否空??
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitIdle(AMingTacticalUnit* Unit};

    /**
     * 檢查???X?否??移X     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitMoving(AMingTacticalUnit* Unit};

    /**
     * 檢查???X?否??攻X     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitAttacking(AMingTacticalUnit* Unit};

    /**
     * 檢查???X?否??防??     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitDefending(AMingTacticalUnit* Unit};

    /**
     * ???X???位置
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static FVector GetUnitLocation(AMingTacticalUnit* Unit};

    /**
     * ???X??X???
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static FRotator GetUnitRotation(AMingTacticalUnit* Unit};

    /**
     * 設置????位置
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Info")
    static bool SetUnitLocation(AMingTacticalUnit* Unit, const FVector& NewLocation};

    /**
     * 設置???X???
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Info")
    static bool SetUnitRotation(AMingTacticalUnit* Unit, const FRotator& NewRotation};

    /**
     * 計?X???距離
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Math")
    static float CalculateDistance(const FVector& PointA, const FVector& PointB};

    /**
     * 計?X??X??X     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Math")
    static float CalculateUnitDistance(AMingTacticalUnit* UnitA, AMingTacticalUnit* UnitB};

    /**
     * 檢查??否???X?內
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Math")
    static bool IsInRange(const FVector& PointA, const FVector& PointB, float Range};

    /**
     * 檢查???X?否??攻???X?內
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Math")
    static bool IsUnitInAttackRange(AMingTacticalUnit* Attacker, AMingTacticalUnit* Target};

    /**
     * ????移?X?度
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Calculation")
    static float GetMovementSpeed(AMingTacticalUnit* Unit, EUnitMovementType MovementType};

    /**
     * 計??移?X???
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Calculation")
    static float CalculateMovementTime(AMingTacticalUnit* Unit, const FVector& StartLocation, const FVector& TargetLocation, EUnitMovementType MovementType};

    /**
     * 計?X??X?害
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Calculation")
    static float CalculateAttackDamage(AMingTacticalUnit* Attacker, AMingTacticalUnit* Target, EUnitAttackType AttackType};

    /**
     * 計??編??位置
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Formation")
    static TArray<FVector> CalculateFormationPositions(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& Center};

    /**
     * 檢查編??完整X     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Formation")
    static bool IsFormationComplete(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions};

    /**
     * ??新編??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Formation")
    static bool UpdateFormation(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions};

    /**
     * ???X???建議
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<FString> GetUnitRecommendations(AMingTacticalUnit* Unit};

    /**
     * ???X???建議
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<FString> GetTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ???X???建議
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<AMingTacticalUnit*> GetAttackTargets(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ???X?禦建議
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<FVector> GetDefensePositions(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ???X?離建議
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<FVector> GetRetreatPositions(const TArray<AMingTacticalUnit*>& Units};

    /**
     * 模擬??鬥結??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Simulation")
    static TArray<FUnitControlResult> SimulateBattle(const TArray<AMingTacticalUnit*>& Attackers, const TArray<AMingTacticalUnit*>& Defenders, int32 SimulationCount};

    /**
     * 模擬移??路??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Simulation")
    static TArray<FVector> SimulateMovementPath(AMingTacticalUnit* Unit, const FVector& StartLocation, const FVector& TargetLocation};

    /**
     * ???X???歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|History")
    static TArray<FUnitCommand> GetUnitHistory(AMingTacticalUnit* Unit};

    /**
     * ???X?令歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|History")
    static TArray<FUnitControlResult> GetCommandHistory();

    /**
     * ???X?鬥統??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Statistics")
    static TMap<FString, int32> GetBattleStatistics();

    /**
     * ???X???統??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Statistics")
    static TMap<FString, int32> GetUnitStatistics();

    /**
     * ???X?能統??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Statistics")
    static TMap<FString, float> GetPerformanceStatistics();

    /**
     * 導出???X???
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Export")
    static bool ExportUnitData(const FString& FilePath};

    /**
     * 導入???X???
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Import")
    static bool ImportUnitData(const FString& FilePath};

    /**
     * ???X??X???
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Report")
    static FString GenerateUnitReport(AMingTacticalUnit* Unit};

    /**
     * ???X?制??報X     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Report")
    static FString GenerateControllerReport();

    /**
     * ???X?鬥????
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Report")
    static FString GenerateBattleReport(const TArray<AMingTacticalUnit*>& Attackers, const TArray<AMingTacticalUnit*>& Defenders};

    /**
     * ??置???X?制X     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static void ResetUnitController();

    /**
     * 清除????命??     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static void ClearAllCommands();

    /**
     * 清除????歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static void ClearUnitHistory();

    /**
     * ??份???X???
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static bool BackupUnitData(const FString& BackupPath};

    /**
     * ??復???X???
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static bool RestoreUnitData(const FString& BackupPath};
};

