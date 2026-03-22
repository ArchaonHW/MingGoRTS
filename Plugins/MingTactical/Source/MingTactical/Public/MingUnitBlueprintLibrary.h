#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingUnitController.h"
#include "MingUnitBlueprintLibrary.generated.h"

class AMingTacticalUnit;

/**
 * 故事選項? * 故事重要性摧毀X??X?t??
 */
UCLASS()
class MINGTACTICAL_API UMingUnitBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * 故事重要性?X     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static AMingUnitController* GetUnitController();

    /**
     * ???X?챱??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static void InitializeUnitController();

    /**
     * 故事重要性?X     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static void ShutdownUnitController();

    /**
     * K?X摧毀?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool AddControlledUnit(AMingTacticalUnit* Unit);

    /**
     * 摧毀??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool RemoveControlledUnit(AMingTacticalUnit* Unit);

    /**
     * 目標數量?X???     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static TArray<AMingTacticalUnit*> GetControlledUnits();

    /**
     * 摧毀w?X??X???
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static TArray<AMingTacticalUnit*> GetUnitsByType(const FString& UnitType);

    /**
     * ??目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static TArray<AMingTacticalUnit*> GetActiveUnits();

    /**
     * 故事重要性?O
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult IssueUnitCommand(const FUnitCommand& Command);

    /**
     * 故事重要性?O
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static TArray<FUnitControlResult> BatchIssueCommands(const TArray<FUnitCommand>& Commands);

    /**
     * ???X?O
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool CancelCommand(const FString& CommandID);

    /**
     * 目標數量R??     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool CancelAllCommands();

    /**
     * ?X??X摧毀?     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult MoveUnitsToLocation(const TArray<AMingTacticalUnit*>& Units, const FVector& TargetLocation, EUnitMovementType MovementType);

    /**
     * 目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult AttackTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetUnit, EUnitAttackType AttackType);

    /**
     * 摧毀??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult AttackBuilding(const TArray<AMingTacticalUnit*>& Units, AActor* TargetBuilding, EUnitAttackType AttackType);

    /**
     * 摧毀摧毀??     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult StopUnits(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ??mm
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult HoldPosition(const TArray<AMingTacticalUnit*>& Units, const FVector& HoldLocation);

    /**
     * 摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult PatrolPath(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& PatrolPoints);

    /**
     * u@摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult GuardTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetToGuard);

    /**
     * ??h
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult RetreatUnits(const TArray<AMingTacticalUnit*>& Units, const FVector& RetreatLocation);

    /**
     * ??s??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult FormUpUnits(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& FormationCenter);

    /**
     * ϥ??X     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult UseAbility(const TArray<AMingTacticalUnit*>& Units, const FString& AbilityID, AMingTacticalUnit* TargetUnit = nullptr);

    /**
     * 故事重要性?X     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static EMingUnitState GetUnitState(AMingTacticalUnit* Unit);

    /**
     * ]m???X?X     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool SetUnitState(AMingTacticalUnit* Unit, EMingUnitState NewState);

    /**
     * 故事重要性?O摧毀
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static TArray<FUnitCommand> GetUnitCommandQueue(AMingTacticalUnit* Unit);

    /**
     * 故事重要性?O
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FUnitCommand GetCurrentCommand(AMingTacticalUnit* Unit);

    /**
     * ˬd???X?_??R??     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static bool CanExecuteCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    /**
     * 故事選項?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static TMap<FString, int32> GetUnitControlStatistics();

    /**
     * 目標數量ʯ摧毀
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static TMap<FString, float> GetPerformanceMetrics();

    /**
     * 目標數量摧毀     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetStateName(EMingUnitState State);

    /**
     * ???X?O?X?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetCommandTypeName(EUnitCommandType CommandType);

    /**
     * ???X?O摧毀摧毀     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetPriorityName(EUnitCommandPriority Priority);

    /**
     * 摧毀???X?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetMovementTypeName(EUnitMovementType MovementType);

    /**
     * 目標數量?X?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetAttackTypeName(EUnitAttackType AttackType);

    /**
     * 摧毀s???X?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetFormationTypeName(EUnitFormationType FormationType);

    /**
     * 摧毀?X?O
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
     * ]m??O??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Creation")
    static FUnitCommand SetCommandParameter(const FUnitCommand& Command, const FString& ParameterName, const FString& ParameterValue);

    /**
     * ]m??O??զ
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Creation")
    static FUnitCommand SetCommandRetryCount(const FUnitCommand& Command, int32 MaxRetryCount);

    /**
     * ??目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Batch")
    static TArray<FUnitControlResult> BatchMoveUnits(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& TargetLocations, EUnitMovementType MovementType);

    /**
     * 故事選項?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Batch")
    static TArray<FUnitControlResult> BatchAttackTargets(const TArray<AMingTacticalUnit*>& Units, const TArray<AMingTacticalUnit*>& TargetUnits, EUnitAttackType AttackType);

    /**
     * 目標數量??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Batch")
    static TArray<FUnitControlResult> BatchStopUnits(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 摧毀t??X     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static bool IsUnitControllerInitialized();

    /**
     * 摧毀t??
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static FString GetUnitControllerVersion();

    /**
     * 目標數量?X???
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static int32 GetControlledUnitCount();

    /**
     * 摧毀?X??X???
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static int32 GetActiveUnitCount();

    /**
     * ???X?O`
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static int32 GetTotalCommandCount();

    /**
     * 目標數量ROX     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static int32 GetExecutingCommandCount();

    /**
     * ˬd???X?_??
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitControlled(AMingTacticalUnit* Unit);

    /**
     * ˬd???X?_??
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitActive(AMingTacticalUnit* Unit);

    /**
     * ˬd???X?_??
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitIdle(AMingTacticalUnit* Unit);

    /**
     * ˬd???X?_??X     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitMoving(AMingTacticalUnit* Unit);

    /**
     * ˬd???X?_??X     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitAttacking(AMingTacticalUnit* Unit);

    /**
     * ˬd???X?_摧毀     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitDefending(AMingTacticalUnit* Unit);

    /**
     * 目標數量m
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static FVector GetUnitLocation(AMingTacticalUnit* Unit);

    /**
     * 故事選項?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static FRotator GetUnitRotation(AMingTacticalUnit* Unit);

    /**
     * ]m摧毀m
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Info")
    static bool SetUnitLocation(AMingTacticalUnit* Unit, const FVector& NewLocation);

    /**
     * ]m目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Info")
    static bool SetUnitRotation(AMingTacticalUnit* Unit, const FRotator& NewRotation);

    /**
     * p?X???Z
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Math")
    static float CalculateDistance(const FVector& PointA, const FVector& PointB);

    /**
     * p?X??X??X     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Math")
    static float CalculateUnitDistance(AMingTacticalUnit* UnitA, AMingTacticalUnit* UnitB);

    /**
     * ˬd??_???X?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Math")
    static bool IsInRange(const FVector& PointA, const FVector& PointB, float Range);

    /**
     * ˬd???X?_摧毀?X?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Math")
    static bool IsUnitInAttackRange(AMingTacticalUnit* Attacker, AMingTacticalUnit* Target);

    /**
     * 摧毀?X?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Calculation")
    static float GetMovementSpeed(AMingTacticalUnit* Unit, EUnitMovementType MovementType);

    /**
     * p目標數量
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Calculation")
    static float CalculateMovementTime(AMingTacticalUnit* Unit, const FVector& StartLocation, const FVector& TargetLocation, EUnitMovementType MovementType);

    /**
     * p?X??X?`
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Calculation")
    static float CalculateAttackDamage(AMingTacticalUnit* Attacker, AMingTacticalUnit* Target, EUnitAttackType AttackType);

    /**
     * p??s??m
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Formation")
    static TArray<FVector> CalculateFormationPositions(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& Center);

    /**
     * ˬds??X     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Formation")
    static bool IsFormationComplete(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions);

    /**
     * ??ss??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Formation")
    static bool UpdateFormation(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions);

    /**
     * 目標數量ĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<FString> GetUnitRecommendations(AMingTacticalUnit* Unit);

    /**
     * 目標數量ĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<FString> GetTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 目標數量ĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<AMingTacticalUnit*> GetAttackTargets(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ???X?mĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<FVector> GetDefensePositions(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ???X?ĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<FVector> GetRetreatPositions(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Simulation")
    static TArray<FUnitControlResult> SimulateBattle(const TArray<AMingTacticalUnit*>& Attackers, const TArray<AMingTacticalUnit*>& Defenders, int32 SimulationCount);

    /**
     * 摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Simulation")
    static TArray<FVector> SimulateMovementPath(AMingTacticalUnit* Unit, const FVector& StartLocation, const FVector& TargetLocation);

    /**
     * 目標數量v
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|History")
    static TArray<FUnitCommand> GetUnitHistory(AMingTacticalUnit* Unit);

    /**
     * ???X?Ov
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|History")
    static TArray<FUnitControlResult> GetCommandHistory();

    /**
     * 目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Statistics")
    static TMap<FString, int32> GetBattleStatistics();

    /**
     * 目標數量??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Statistics")
    static TMap<FString, int32> GetUnitStatistics();

    /**
     * 目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Statistics")
    static TMap<FString, float> GetPerformanceStatistics();

    /**
     * ɥX目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Export")
    static bool ExportUnitData(const FString& FilePath);

    /**
     * ɤJ目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Import")
    static bool ImportUnitData(const FString& FilePath);

    /**
     * 故事選項?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Report")
    static FString GenerateUnitReport(AMingTacticalUnit* Unit);

    /**
     * 目標數量X     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Report")
    static FString GenerateControllerReport();

    /**
     * 目標數量??
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Report")
    static FString GenerateBattleReport(const TArray<AMingTacticalUnit*>& Attackers, const TArray<AMingTacticalUnit*>& Defenders);

    /**
     * ??m???X?X     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static void ResetUnitController();

    /**
     * M摧毀R??     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static void ClearAllCommands();

    /**
     * M摧毀v
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static void ClearUnitHistory();

    /**
     * ??目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static bool BackupUnitData(const FString& BackupPath);

    /**
     * ??_目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static bool RestoreUnitData(const FString& BackupPath);
);
};
