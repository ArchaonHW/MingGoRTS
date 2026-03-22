#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingUnitController.h"
#include "MingUnitBlueprintLibrary.generated.h"

class AMingTacticalUnit;

/**
 * ?Æ‰??çÂ??ΩÊï∏Â∫? * ?ê‰??çÂ??ØË™ø?®Á??Æ‰??ßÂà∂Á≥ªÁµ±?üËÉΩ
 */
UCLASS()
class MINGTACTICAL_API UMingUnitBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?≤Â??Æ‰??ßÂà∂??     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static AMingUnitController* GetUnitController();

    /**
     * ?ùÂ??ñÂñÆ‰ΩçÊéß?∂Âô®
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static void InitializeUnitController();

    /**
     * ?úÈ??Æ‰??ßÂà∂??     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static void ShutdownUnitController();

    /**
     * Ê∑ªÂ??óÊéß?Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool AddControlledUnit(AMingTacticalUnit* Unit);

    /**
     * ÁßªÈô§?óÊéß?Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool RemoveControlledUnit(AMingTacticalUnit* Unit);

    /**
     * ?≤Â??Ä?âÂ??ßÂñÆ‰Ω?     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static TArray<AMingTacticalUnit*> GetControlledUnits();

    /**
     * ?≤Ê?ÂÆöÈ??ãÁ??Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static TArray<AMingTacticalUnit*> GetUnitsByType(const FString& UnitType);

    /**
     * ?≤Â?Ê¥ªË??Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static TArray<AMingTacticalUnit*> GetActiveUnits();

    /**
     * ?ºÂ??Æ‰??Ω‰ª§
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult IssueUnitCommand(const FUnitCommand& Command);

    /**
     * ?πÈ??ºÂ??Ω‰ª§
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static TArray<FUnitControlResult> BatchIssueCommands(const TArray<FUnitCommand>& Commands);

    /**
     * ?ñÊ??Ω‰ª§
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool CancelCommand(const FString& CommandID);

    /**
     * ?ñÊ??Ä?âÂëΩ‰ª?     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool CancelAllCommands();

    /**
     * ÁßªÂ??Æ‰??∞‰?ÁΩ?     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult MoveUnitsToLocation(const TArray<AMingTacticalUnit*>& Units, const FVector& TargetLocation, EUnitMovementType MovementType);

    /**
     * ?ªÊ??ÆÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult AttackTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetUnit, EUnitAttackType AttackType);

    /**
     * ?ªÊ?Âª∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult AttackBuilding(const TArray<AMingTacticalUnit*>& Units, AActor* TargetBuilding, EUnitAttackType AttackType);

    /**
     * ?úÊ≠¢?Ä?âÂ?‰Ω?     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult StopUnits(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ?≤Á¶¶‰ΩçÁΩÆ
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult HoldPosition(const TArray<AMingTacticalUnit*>& Units, const FVector& HoldLocation);

    /**
     * Â∑°È?Ë∑ØÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult PatrolPath(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& PatrolPoints);

    /**
     * ÂÆàË≠∑?ÆÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult GuardTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetToGuard);

    /**
     * ?§ÈÄÄ
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult RetreatUnits(const TArray<AMingTacticalUnit*>& Units, const FVector& RetreatLocation);

    /**
     * ÁµÑÁ?Á∑®È?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult FormUpUnits(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& FormationCenter);

    /**
     * ‰ΩøÁî®?Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static FUnitControlResult UseAbility(const TArray<AMingTacticalUnit*>& Units, const FString& AbilityID, AMingTacticalUnit* TargetUnit = nullptr);

    /**
     * ?≤Â??Æ‰??Ä??     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static EMingUnitState GetUnitState(AMingTacticalUnit* Unit);

    /**
     * Ë®≠ÁΩÆ?Æ‰??Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    static bool SetUnitState(AMingTacticalUnit* Unit, EMingUnitState NewState);

    /**
     * ?≤Â??Æ‰??Ω‰ª§?äÂ?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static TArray<FUnitCommand> GetUnitCommandQueue(AMingTacticalUnit* Unit);

    /**
     * ?≤Â??∂Â??Ω‰ª§
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FUnitCommand GetCurrentCommand(AMingTacticalUnit* Unit);

    /**
     * Ê™¢Êü•?Æ‰??ØÂê¶?ØÂü∑Ë°åÂëΩ‰ª?     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static bool CanExecuteCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    /**
     * ?≤Â??Æ‰??ßÂà∂Áµ±Ë?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static TMap<FString, int32> GetUnitControlStatistics();

    /**
     * ?≤Â??ßÂà∂?®ÊÄßËÉΩ?áÊ?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static TMap<FString, float> GetPerformanceMetrics();

    /**
     * ?≤Â??Ä?ãÂ?Á®?     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetStateName(EMingUnitState State);

    /**
     * ?≤Â??Ω‰ª§È°ûÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetCommandTypeName(EUnitCommandType CommandType);

    /**
     * ?≤Â??Ω‰ª§?™Â?Á¥öÂ?Á®?     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetPriorityName(EUnitCommandPriority Priority);

    /**
     * ?≤Â?ÁßªÂ?È°ûÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetMovementTypeName(EUnitMovementType MovementType);

    /**
     * ?≤Â??ªÊ?È°ûÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetAttackTypeName(EUnitAttackType AttackType);

    /**
     * ?≤Â?Á∑®È?È°ûÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    static FString GetFormationTypeName(EUnitFormationType FormationType);

    /**
     * ?µÂª∫?Æ‰??Ω‰ª§
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
     * Ë®≠ÁΩÆ?Ω‰ª§?ÉÊï∏
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Creation")
    static FUnitCommand SetCommandParameter(const FUnitCommand& Command, const FString& ParameterName, const FString& ParameterValue);

    /**
     * Ë®≠ÁΩÆ?Ω‰ª§?çË©¶Ê¨°Êï∏
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Creation")
    static FUnitCommand SetCommandRetryCount(const FUnitCommand& Command, int32 MaxRetryCount);

    /**
     * ?πÈ?ÁßªÂ??Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Batch")
    static TArray<FUnitControlResult> BatchMoveUnits(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& TargetLocations, EUnitMovementType MovementType);

    /**
     * ?πÈ??ªÊ??ÆÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Batch")
    static TArray<FUnitControlResult> BatchAttackTargets(const TArray<AMingTacticalUnit*>& Units, const TArray<AMingTacticalUnit*>& TargetUnits, EUnitAttackType AttackType);

    /**
     * ?πÈ??úÊ≠¢?Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Batch")
    static TArray<FUnitControlResult> BatchStopUnits(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ?≤Â?Á≥ªÁµ±?Ä??     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static bool IsUnitControllerInitialized();

    /**
     * ?≤Â?Á≥ªÁµ±?àÊú¨
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static FString GetUnitControllerVersion();

    /**
     * ?≤Â??óÊéß?Æ‰??∏È?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static int32 GetControlledUnitCount();

    /**
     * ?≤Â?Ê¥ªË??Æ‰??∏È?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static int32 GetActiveUnitCount();

    /**
     * ?≤Â??Ω‰ª§Á∏ΩÊï∏
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static int32 GetTotalCommandCount();

    /**
     * ?≤Â??∑Ë?‰∏≠ÂëΩ‰ª§Êï∏??     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|System")
    static int32 GetExecutingCommandCount();

    /**
     * Ê™¢Êü•?Æ‰??ØÂê¶?óÊéß
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitControlled(AMingTacticalUnit* Unit);

    /**
     * Ê™¢Êü•?Æ‰??ØÂê¶Ê¥ªË?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitActive(AMingTacticalUnit* Unit);

    /**
     * Ê™¢Êü•?Æ‰??ØÂê¶Á©∫È?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitIdle(AMingTacticalUnit* Unit);

    /**
     * Ê™¢Êü•?Æ‰??ØÂê¶?®Áßª??     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitMoving(AMingTacticalUnit* Unit);

    /**
     * Ê™¢Êü•?Æ‰??ØÂê¶?®Êîª??     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitAttacking(AMingTacticalUnit* Unit);

    /**
     * Ê™¢Êü•?Æ‰??ØÂê¶?®Èò≤Á¶?     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static bool IsUnitDefending(AMingTacticalUnit* Unit);

    /**
     * ?≤Â??Æ‰?‰ΩçÁΩÆ
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static FVector GetUnitLocation(AMingTacticalUnit* Unit);

    /**
     * ?≤Â??Æ‰??ãË?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Info")
    static FRotator GetUnitRotation(AMingTacticalUnit* Unit);

    /**
     * Ë®≠ÁΩÆ?Æ‰?‰ΩçÁΩÆ
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Info")
    static bool SetUnitLocation(AMingTacticalUnit* Unit, const FVector& NewLocation);

    /**
     * Ë®≠ÁΩÆ?Æ‰??ãË?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Info")
    static bool SetUnitRotation(AMingTacticalUnit* Unit, const FRotator& NewRotation);

    /**
     * Ë®àÁ??©È?Ë∑ùÈõ¢
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Math")
    static float CalculateDistance(const FVector& PointA, const FVector& PointB);

    /**
     * Ë®àÁ??Æ‰??ìË???     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Math")
    static float CalculateUnitDistance(AMingTacticalUnit* UnitA, AMingTacticalUnit* UnitB);

    /**
     * Ê™¢Êü•?ØÂê¶?®Á??çÂÖß
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Math")
    static bool IsInRange(const FVector& PointA, const FVector& PointB, float Range);

    /**
     * Ê™¢Êü•?Æ‰??ØÂê¶?®Êîª?äÁ??çÂÖß
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Math")
    static bool IsUnitInAttackRange(AMingTacticalUnit* Attacker, AMingTacticalUnit* Target);

    /**
     * ?≤Â?ÁßªÂ??üÂ∫¶
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Calculation")
    static float GetMovementSpeed(AMingTacticalUnit* Unit, EUnitMovementType MovementType);

    /**
     * Ë®àÁ?ÁßªÂ??ÇÈ?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Calculation")
    static float CalculateMovementTime(AMingTacticalUnit* Unit, const FVector& StartLocation, const FVector& TargetLocation, EUnitMovementType MovementType);

    /**
     * Ë®àÁ??ªÊ??∑ÂÆ≥
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Calculation")
    static float CalculateAttackDamage(AMingTacticalUnit* Attacker, AMingTacticalUnit* Target, EUnitAttackType AttackType);

    /**
     * Ë®àÁ?Á∑®È?‰ΩçÁΩÆ
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Formation")
    static TArray<FVector> CalculateFormationPositions(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& Center);

    /**
     * Ê™¢Êü•Á∑®È?ÂÆåÊï¥??     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller|Formation")
    static bool IsFormationComplete(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions);

    /**
     * ?¥Êñ∞Á∑®È?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Formation")
    static bool UpdateFormation(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions);

    /**
     * ?≤Â??Æ‰?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<FString> GetUnitRecommendations(AMingTacticalUnit* Unit);

    /**
     * ?≤Â??∞Ë?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<FString> GetTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ?≤Â??ªÊ?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<AMingTacticalUnit*> GetAttackTargets(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ?≤Â??≤Á¶¶Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<FVector> GetDefensePositions(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ?≤Â??§Èõ¢Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|AI")
    static TArray<FVector> GetRetreatPositions(const TArray<AMingTacticalUnit*>& Units);

    /**
     * Ê®°Êì¨?∞È¨•ÁµêÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Simulation")
    static TArray<FUnitControlResult> SimulateBattle(const TArray<AMingTacticalUnit*>& Attackers, const TArray<AMingTacticalUnit*>& Defenders, int32 SimulationCount);

    /**
     * Ê®°Êì¨ÁßªÂ?Ë∑ØÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Simulation")
    static TArray<FVector> SimulateMovementPath(AMingTacticalUnit* Unit, const FVector& StartLocation, const FVector& TargetLocation);

    /**
     * ?≤Â??Æ‰?Ê≠∑Âè≤
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|History")
    static TArray<FUnitCommand> GetUnitHistory(AMingTacticalUnit* Unit);

    /**
     * ?≤Â??Ω‰ª§Ê≠∑Âè≤
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|History")
    static TArray<FUnitControlResult> GetCommandHistory();

    /**
     * ?≤Â??∞È¨•Áµ±Ë?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Statistics")
    static TMap<FString, int32> GetBattleStatistics();

    /**
     * ?≤Â??Æ‰?Áµ±Ë?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Statistics")
    static TMap<FString, int32> GetUnitStatistics();

    /**
     * ?≤Â??ßËÉΩÁµ±Ë?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Statistics")
    static TMap<FString, float> GetPerformanceStatistics();

    /**
     * Â∞éÂá∫?Æ‰??∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Export")
    static bool ExportUnitData(const FString& FilePath);

    /**
     * Â∞éÂÖ•?Æ‰??∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Import")
    static bool ImportUnitData(const FString& FilePath);

    /**
     * ?≤Â??Æ‰??±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Report")
    static FString GenerateUnitReport(AMingTacticalUnit* Unit);

    /**
     * ?≤Â??ßÂà∂?®Â†±??     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Report")
    static FString GenerateControllerReport();

    /**
     * ?≤Â??∞È¨•?±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|Report")
    static FString GenerateBattleReport(const TArray<AMingTacticalUnit*>& Attackers, const TArray<AMingTacticalUnit*>& Defenders);

    /**
     * ?çÁΩÆ?Æ‰??ßÂà∂??     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static void ResetUnitController();

    /**
     * Ê∏ÖÈô§?Ä?âÂëΩ‰ª?     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static void ClearAllCommands();

    /**
     * Ê∏ÖÈô§?Æ‰?Ê≠∑Âè≤
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static void ClearUnitHistory();

    /**
     * ?ô‰ªΩ?Æ‰??∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static bool BackupUnitData(const FString& BackupPath);

    /**
     * ?¢Âæ©?Æ‰??∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller|System")
    static bool RestoreUnitData(const FString& BackupPath);
};
