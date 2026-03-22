#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "MingUnitController.generated.h"

class AMingTacticalUnit;
class UMingSelectionManager;

/**
 * ?Æ‰??Ä?? */
UENUM(BlueprintType)
enum class EMingUnitState : uint8
{
    Idle                UMETA(DisplayName = "Idle"),
    Moving              UMETA(DisplayName = "Moving"),
    Attacking           UMETA(DisplayName = "Attacking"),
    Defending           UMETA(DisplayName = "Defending"),
    Patrolling          UMETA(DisplayName = "Patrolling"),
    Guarding            UMETA(DisplayName = "Guarding"),
    Retreating          UMETA(DisplayName = "Retreating"),
    Dead                UMETA(DisplayName = "Dead"),
    Disabled            UMETA(DisplayName = "Disabled"),
    Captured            UMETA(DisplayName = "Captured"),
    Building            UMETA(DisplayName = "Building"),
    Healing             UMETA(DisplayName = "Healing"),
    Repairing           UMETA(DisplayName = "Repairing")
};

/**
 * ?Æ‰??Ω‰ª§È°ûÂ?
 */
UENUM(BlueprintType)
enum class EUnitCommandType : uint8
{
    Move                UMETA(DisplayName = "Move"),
    Attack              UMETA(DisplayName = "Attack"),
    Stop                UMETA(DisplayName = "Stop"),
    Hold                UMETA(DisplayName = "Hold"),
    Patrol              UMETA(DisplayName = "Patrol"),
    Guard               UMETA(DisplayName = "Guard"),
    Retreat             UMETA(DisplayName = "Retreat"),
    Build               UMETA(DisplayName = "Build"),
    Repair              UMETA(DisplayName = "Repair"),
    Heal                UMETA(DisplayName = "Heal"),
    Capture             UMETA(DisplayName = "Capture"),
    UseAbility          UMETA(DisplayName = "Use Ability"),
    FormUp              UMETA(DisplayName = "Form Up"),
    Dismiss             UMETA(DisplayName = "Dismiss")
};

/**
 * ?Æ‰??Ω‰ª§?™Â?Á¥? */
UENUM(BlueprintType)
enum class EUnitCommandPriority : uint8
{
    Critical            UMETA(DisplayName = "Critical"),
    High                UMETA(DisplayName = "High"),
    Medium              UMETA(DisplayName = "Medium"),
    Low                 UMETA(DisplayName = "Low"),
    Background          UMETA(DisplayName = "Background")
};

/**
 * ?Æ‰?ÁßªÂ?È°ûÂ?
 */
UENUM(BlueprintType)
enum class EUnitMovementType : uint8
{
    Walk                UMETA(DisplayName = "Walk"),
    Run                 UMETA(DisplayName = "Run"),
    Sprint              UMETA(DisplayName = "Sprint"),
    Crawl               UMETA(DisplayName = "Crawl"),
    Swim                UMETA(DisplayName = "Swim"),
    Fly                 UMETA(DisplayName = "Fly"),
    Teleport            UMETA(DisplayName = "Teleport"),
    Charge              UMETA(DisplayName = "Charge")
};

/**
 * ?Æ‰??ªÊ?È°ûÂ?
 */
UENUM(BlueprintType)
enum class EUnitAttackType : uint8
{
    Melee               UMETA(DisplayName = "Melee"),
    Ranged              UMETA(DisplayName = "Ranged"),
    Area                UMETA(DisplayName = "Area"),
    Splash              UMETA(DisplayName = "Splash"),
    DOT                 UMETA(DisplayName = "Damage Over Time"),
    Debuff              UMETA(DisplayName = "Debuff"),
    Buff                UMETA(DisplayName = "Buff"),
    Heal                UMETA(DisplayName = "Heal")
};

/**
 * ?Æ‰?Á∑®È?È°ûÂ?
 */
UENUM(BlueprintType)
enum class EUnitFormationType : uint8
{
    Line                UMETA(DisplayName = "Line"),
    Column              UMETA(DisplayName = "Column"),
    Wedge               UMETA(DisplayName = "Wedge"),
    Circle              UMETA(DisplayName = "Circle"),
    Square              UMETA(DisplayName = "Square"),
    Diamond             UMETA(DisplayName = "Diamond"),
    Scattered           UMETA(DisplayName = "Scattered"),
    Custom              UMETA(DisplayName = "Custom")
};

/**
 * ?Æ‰??Ω‰ª§
 */
USTRUCT(BlueprintType)
struct FUnitCommand
{
    GENERATED_BODY()

    // ?Ω‰ª§ID
    UPROPERTY(BlueprintReadOnly)
    FString CommandID;

    // ?Ω‰ª§È°ûÂ?
    UPROPERTY(BlueprintReadOnly)
    EUnitCommandType CommandType;

    // ?Ω‰ª§?™Â?Á¥?    UPROPERTY(BlueprintReadOnly)
    EUnitCommandPriority Priority;

    // ?ÆÊ?‰ΩçÁΩÆ
    UPROPERTY(BlueprintReadOnly)
    FVector TargetLocation;

    // ?ÆÊ??Æ‰?
    UPROPERTY(BlueprintReadOnly)
    AMingTacticalUnit* TargetUnit;

    // ?ÆÊ?Âª∫Á?
    UPROPERTY(BlueprintReadOnly)
    AActor* TargetBuilding;

    // ÁßªÂ?È°ûÂ?
    UPROPERTY(BlueprintReadOnly)
    EUnitMovementType MovementType;

    // ?ªÊ?È°ûÂ?
    UPROPERTY(BlueprintReadOnly)
    EUnitAttackType AttackType;

    // Á∑®È?È°ûÂ?
    UPROPERTY(BlueprintReadOnly)
    EUnitFormationType FormationType;

    // ?Ω‰ª§?ÉÊï∏
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CommandParameters;

    // ?Ω‰ª§?ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float CommandTime;

    // ?∑Ë??ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float ExecuteTime;

    // ?ØÂê¶Â∑≤Âü∑Ë°?    UPROPERTY(BlueprintReadOnly)
    bool bIsExecuted;

    // ?ØÂê¶Â∑≤Â???    UPROPERTY(BlueprintReadOnly)
    bool bIsCompleted;

    // ?ØÂê¶Â∑≤Â?Ê∂?    UPROPERTY(BlueprintReadOnly)
    bool bIsCancelled;

    // ?çË©¶Ê¨°Êï∏
    UPROPERTY(BlueprintReadOnly)
    int32 RetryCount;

    // ?ÄÂ§ßÈ?Ë©¶Ê¨°??    UPROPERTY(BlueprintReadOnly)
    int32 MaxRetryCount;

    FUnitCommand()
        : CommandID(TEXT(""))
        , CommandType(EUnitCommandType::Move)
        , Priority(EUnitCommandPriority::Medium)
        , TargetLocation(FVector::ZeroVector)
        , TargetUnit(nullptr)
        , TargetBuilding(nullptr)
        , MovementType(EUnitMovementType::Run)
        , AttackType(EUnitAttackType::Melee)
        , FormationType(EUnitFormationType::Line)
        , CommandTime(0.0f)
        , ExecuteTime(0.0f)
        , bIsExecuted(false)
        , bIsCompleted(false)
        , bIsCancelled(false)
        , RetryCount(0)
        , MaxRetryCount(3)
    {}
};

/**
 * ?Æ‰??ßÂà∂ÁµêÊ?
 */
USTRUCT(BlueprintType)
struct FUnitControlResult
{
    GENERATED_BODY()

    // ?ØÂê¶?êÂ?
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // ?Ω‰ª§ID
    UPROPERTY(BlueprintReadOnly)
    FString CommandID;

    // ?∑Ë??ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float ExecuteTime;

    // ÁµêÊ??èËø∞
    UPROPERTY(BlueprintReadOnly)
    FString ResultDescription;

    // ÂΩ±Èüø?ÑÂñÆ‰Ω?    UPROPERTY(BlueprintReadOnly)
    TArray<AMingTacticalUnit*> AffectedUnits;

    // ÂΩ±Èüø?ÑÂª∫ÁØ?    UPROPERTY(BlueprintReadOnly)
    TArray<AActor*> AffectedBuildings;

    // ?†Ê??ÑÂÇ∑ÂÆ?    UPROPERTY(BlueprintReadOnly)
    float DamageDealt;

    // Ê∂àËÄóÁ?Ë≥áÊ?
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> ResourceCost;

    // ?≤Â??ÑÁ?È©?    UPROPERTY(BlueprintReadOnly)
    int32 ExperienceGained;

    // ?ØË™§‰ø°ÊÅØ
    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    FUnitControlResult()
        : bSuccess(false)
        , CommandID(TEXT(""))
        , ExecuteTime(0.0f)
        , ResultDescription(TEXT(""))
        , DamageDealt(0.0f)
        , ExperienceGained(0)
        , ErrorMessage(TEXT(""))
    {}
};

/**
 * ?Æ‰??ßÂà∂?? * ÁÆ°Á??Æ‰??ÑÊ??âÊéß?∂È?ËºØÂ?Ë°åÁÇ∫
 */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API AMingUnitController : public AActor
{
    GENERATED_BODY()

public:
    AMingUnitController(};

    /**
     * ?ùÂ??ñÂñÆ‰ΩçÊéß?∂Âô®
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    void InitializeUnitController(};

    /**
     * ?úÈ??Æ‰??ßÂà∂??     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    void ShutdownUnitController(};

    /**
     * Ê∑ªÂ??óÊéß?Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    bool AddControlledUnit(AMingTacticalUnit* Unit};

    /**
     * ÁßªÈô§?óÊéß?Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    bool RemoveControlledUnit(AMingTacticalUnit* Unit};

    /**
     * ?≤Â??Ä?âÂ??ßÂñÆ‰Ω?     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    TArray<AMingTacticalUnit*> GetControlledUnits() const;

    /**
     * ?≤Ê?ÂÆöÈ??ãÁ??Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    TArray<AMingTacticalUnit*> GetUnitsByType(const FString& UnitType) const;

    /**
     * ?≤Â?Ê¥ªË??Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    TArray<AMingTacticalUnit*> GetActiveUnits() const;

    /**
     * ?ºÂ??Æ‰??Ω‰ª§
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult IssueUnitCommand(const FUnitCommand& Command};

    /**
     * ?πÈ??ºÂ??Ω‰ª§
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    TArray<FUnitControlResult> BatchIssueCommands(const TArray<FUnitCommand>& Commands};

    /**
     * ?ñÊ??Ω‰ª§
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    bool CancelCommand(const FString& CommandID};

    /**
     * ?ñÊ??Ä?âÂëΩ‰ª?     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    bool CancelAllCommands(};

    /**
     * ÁßªÂ??Æ‰??∞‰?ÁΩ?     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult MoveUnitsToLocation(const TArray<AMingTacticalUnit*>& Units, const FVector& TargetLocation, EUnitMovementType MovementType};

    /**
     * ?ªÊ??ÆÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult AttackTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetUnit, EUnitAttackType AttackType};

    /**
     * ?ªÊ?Âª∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult AttackBuilding(const TArray<AMingTacticalUnit*>& Units, AActor* TargetBuilding, EUnitAttackType AttackType};

    /**
     * ?úÊ≠¢?Ä?âÂ?‰Ω?     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult StopUnits(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ?≤Á¶¶‰ΩçÁΩÆ
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult HoldPosition(const TArray<AMingTacticalUnit*>& Units, const FVector& HoldLocation};

    /**
     * Â∑°È?Ë∑ØÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult PatrolPath(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& PatrolPoints};

    /**
     * ÂÆàË≠∑?ÆÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult GuardTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetToGuard};

    /**
     * ?§ÈÄÄ
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult RetreatUnits(const TArray<AMingTacticalUnit*>& Units, const FVector& RetreatLocation};

    /**
     * ÁµÑÁ?Á∑®È?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult FormUpUnits(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& FormationCenter};

    /**
     * ‰ΩøÁî®?Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult UseAbility(const TArray<AMingTacticalUnit*>& Units, const FString& AbilityID, AMingTacticalUnit* TargetUnit = nullptr};

    /**
     * ?≤Â??Æ‰??Ä??     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    EMingUnitState GetUnitState(AMingTacticalUnit* Unit) const;

    /**
     * Ë®≠ÁΩÆ?Æ‰??Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    bool SetUnitState(AMingTacticalUnit* Unit, EMingUnitState NewState};

    /**
     * ?≤Â??Æ‰??Ω‰ª§?äÂ?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    TArray<FUnitCommand> GetUnitCommandQueue(AMingTacticalUnit* Unit) const;

    /**
     * ?≤Â??∂Â??Ω‰ª§
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    FUnitCommand GetCurrentCommand(AMingTacticalUnit* Unit) const;

    /**
     * Ê™¢Êü•?Æ‰??ØÂê¶?ØÂü∑Ë°åÂëΩ‰ª?     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    bool CanExecuteCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command) const;

    /**
     * ?≤Â??Æ‰??ßÂà∂Áµ±Ë?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    TMap<FString, int32> GetUnitControlStatistics() const;

    /**
     * ?≤Â??ßÂà∂?®ÊÄßËÉΩ?áÊ?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    TMap<FString, float> GetPerformanceMetrics() const;

protected:
    // ?óÊéß?Æ‰??óË°®
    UPROPERTY()
    TArray<AMingTacticalUnit*> ControlledUnits;

    // Ê≥®Ê?ÔºöÁÑ° BlueprintTypeÔºåÈÅø?çÂ??éÂ?Á®±Ë?Á™?    UPROPERTY()
    TMap<AMingTacticalUnit*, EMingUnitState> UnitStates;

    // ?Æ‰??Ω‰ª§?äÂ? - Ê≥®Ê?ÔºöTMap<TArray> ‰∏çÊîØ??UPROPERTY
    TMap<AMingTacticalUnit*, TArray<FUnitCommand>> UnitCommandQueues;

    // ?∂Â??∑Ë??Ω‰ª§
    UPROPERTY()
    TMap<AMingTacticalUnit*, FUnitCommand> CurrentCommands;

    // ?Æ‰??ßÂà∂Áµ±Ë?
    UPROPERTY()
    TMap<FString, int32> ControlStatistics;

    // ?ßËÉΩ?áÊ?
    UPROPERTY()
    TMap<FString, float> PerformanceMetrics;

    // ?∏Ê?ÁÆ°Á??®Â???    UPROPERTY()
    UMingSelectionManager* SelectionManager;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bIsInitialized;

    // Tick?¥Êñ∞
    virtual void Tick(float DeltaTime) override;

    // ?∑Ë??Æ‰??Ω‰ª§
    void ExecuteUnitCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?ïÁ?ÁßªÂ??Ω‰ª§
    void HandleMoveCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?ïÁ??ªÊ??Ω‰ª§
    void HandleAttackCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?ïÁ??úÊ≠¢?Ω‰ª§
    void HandleStopCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?ïÁ??≤Á¶¶?Ω‰ª§
    void HandleHoldCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?ïÁ?Â∑°È??Ω‰ª§
    void HandlePatrolCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?ïÁ?ÂÆàË≠∑?Ω‰ª§
    void HandleGuardCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?ïÁ??§ÈÄÄ?Ω‰ª§
    void HandleRetreatCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?ïÁ?Âª∫Ë®≠?Ω‰ª§
    void HandleBuildCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?ïÁ?‰øÆÁ??Ω‰ª§
    void HandleRepairCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?ïÁ?Ê≤ªÁ??Ω‰ª§
    void HandleHealCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?ïÁ??ïÁç≤?Ω‰ª§
    void HandleCaptureCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?ïÁ??Ä?ΩÂëΩ‰ª?    void HandleAbilityCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?ïÁ?Á∑®È??Ω‰ª§
    void HandleFormUpCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // Ê™¢Êü•?Ω‰ª§ÂÆåÊ?
    bool IsCommandCompleted(AMingTacticalUnit* Unit, const FUnitCommand& Command) const;

    // Ê™¢Êü•?Ω‰ª§Â§±Ê?
    bool IsCommandFailed(AMingTacticalUnit* Unit, const FUnitCommand& Command) const;

    // ?ïÁ??Ω‰ª§ÂÆåÊ?
    void OnCommandCompleted(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?ïÁ??Ω‰ª§Â§±Ê?
    void OnCommandFailed(AMingTacticalUnit* Unit, const FUnitCommand& Command, const FString& ErrorReason};

    // ?¥Êñ∞?Æ‰??Ä??    void UpdateUnitState(AMingTacticalUnit* Unit};

    // ?≤Â?‰∏ã‰??ãÂëΩ‰ª?    FUnitCommand GetNextCommand(AMingTacticalUnit* Unit};

    // Ê∏ÖÁ?Â∑≤Â??êÂëΩ‰ª?    void CleanupCompletedCommands(AMingTacticalUnit* Unit};

    // Ë®àÁ?Ë∑ØÂ?
    TArray<FVector> CalculatePath(AMingTacticalUnit* Unit, const FVector& TargetLocation};

    // Ê™¢Êü•Ë∑ØÂ??âÊ???    bool IsPathValid(AMingTacticalUnit* Unit, const TArray<FVector>& Path) const;

    // Ë®àÁ??ªÊ?ÁØÑÂ?
    float GetAttackRange(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit) const;

    // Ê™¢Êü•?ªÊ?ÁØÑÂ?
    bool IsInAttackRange(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit) const;

    // Ë®àÁ?ÁßªÂ??üÂ∫¶
    float GetMovementSpeed(AMingTacticalUnit* Unit, EUnitMovementType MovementType) const;

    // Ë®àÁ??ªÊ??∑ÂÆ≥
    float CalculateAttackDamage(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit, EUnitAttackType AttackType) const;

    // ?âÁî®?∑ÂÆ≥
    void ApplyDamage(AMingTacticalUnit* TargetUnit, float Damage, AMingTacticalUnit* AttackerUnit};

    // Ê™¢Êü•?Æ‰??üÂ??Ä??    bool IsUnitAlive(AMingTacticalUnit* Unit) const;

    // ?≤Â??Æ‰?‰ΩçÁΩÆ
    FVector GetUnitLocation(AMingTacticalUnit* Unit) const;

    // Ë®≠ÁΩÆ?Æ‰?‰ΩçÁΩÆ
    void SetUnitLocation(AMingTacticalUnit* Unit, const FVector& NewLocation};

    // ?≤Â??Æ‰??ãË?
    FRotator GetUnitRotation(AMingTacticalUnit* Unit) const;

    // Ë®≠ÁΩÆ?Æ‰??ãË?
    void SetUnitRotation(AMingTacticalUnit* Unit, const FRotator& NewRotation};

    // ?¥Êñ∞?ßËÉΩ?áÊ?
    void UpdatePerformanceMetrics(};

    // Ë®òÈ??ßÂà∂‰∫ã‰ª∂
    void LogControlEvent(const FString& EventType, const FString& Details};

    // ?üÊ??ßÂà∂?±Â?
    FString GenerateControlReport() const;

    // È©óË??Æ‰?
    bool ValidateUnit(AMingTacticalUnit* Unit) const;

    // È©óË??Ω‰ª§
    bool ValidateCommand(const FUnitCommand& Command) const;

    // ?≤Â??Ω‰ª§?èËø∞
    FString GetCommandDescription(const FUnitCommand& Command) const;

    // ?≤Â??Ä?ãÊ?Ëø?    FString GetStateDescription(EUnitState State) const;

    // Ë®àÁ?Á∑®È?‰ΩçÁΩÆ
    TArray<FVector> CalculateFormationPositions(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& Center};

    // Ê™¢Êü•Á∑®È?ÂÆåÊï¥??    bool IsFormationComplete(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions};

    // ?¥Êñ∞Á∑®È?
    void UpdateFormation(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions};
};

