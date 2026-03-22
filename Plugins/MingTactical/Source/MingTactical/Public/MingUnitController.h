#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "MingUnitController.generated.h"

class AMingTacticalUnit;
class UMingSelectionManager;

/**
 * ?��X�X */
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
 * ?��X�令類�?
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
 * ?��X�令?��?�? */
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
 * ?��?移�?類�?
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
 * ?��X��?類�?
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
 * ?��?編�?類�?
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
 * ?��X�令
 */
USTRUCT(BlueprintType)
struct FUnitCommand
{
    GENERATED_BODY()

    // ?�令ID
    UPROPERTY(BlueprintReadOnly)
    FString CommandID;

    // ?�令類�?
    UPROPERTY(BlueprintReadOnly)
    EUnitCommandType CommandType;

    // ?�令?��?�?    UPROPERTY(BlueprintReadOnly)
    EUnitCommandPriority Priority;

    // ?��?位置
    UPROPERTY(BlueprintReadOnly)
    FVector TargetLocation;

    // ?��X��?
    UPROPERTY(BlueprintReadOnly)
    AMingTacticalUnit* TargetUnit;

    // ?��?建�?
    UPROPERTY(BlueprintReadOnly)
    AActor* TargetBuilding;

    // 移�?類�?
    UPROPERTY(BlueprintReadOnly)
    EUnitMovementType MovementType;

    // ?��?類�?
    UPROPERTY(BlueprintReadOnly)
    EUnitAttackType AttackType;

    // 編�?類�?
    UPROPERTY(BlueprintReadOnly)
    EUnitFormationType FormationType;

    // ?�令?�數
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CommandParameters;

    // ?�令?��?
    UPROPERTY(BlueprintReadOnly)
    float CommandTime;

    // ?��X��?
    UPROPERTY(BlueprintReadOnly)
    float ExecuteTime;

    // ?�否已執�?    UPROPERTY(BlueprintReadOnly)
    bool bIsExecuted;

    // ?�否已�X    UPROPERTY(BlueprintReadOnly)
    bool bIsCompleted;

    // ?�否已�?�?    UPROPERTY(BlueprintReadOnly)
    bool bIsCancelled;

    // ?�試次數
    UPROPERTY(BlueprintReadOnly)
    int32 RetryCount;

    // ?�大�?試次X    UPROPERTY(BlueprintReadOnly)
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
 * ?��X�制結�?
 */
USTRUCT(BlueprintType)
struct FUnitControlResult
{
    GENERATED_BODY()

    // ?�否?��?
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // ?�令ID
    UPROPERTY(BlueprintReadOnly)
    FString CommandID;

    // ?��X��?
    UPROPERTY(BlueprintReadOnly)
    float ExecuteTime;

    // 結�X�述
    UPROPERTY(BlueprintReadOnly)
    FString ResultDescription;

    // 影響?�單�?    UPROPERTY(BlueprintReadOnly)
    TArray<AMingTacticalUnit*> AffectedUnits;

    // 影響?�建�?    UPROPERTY(BlueprintReadOnly)
    TArray<AActor*> AffectedBuildings;

    // ?��X�傷�?    UPROPERTY(BlueprintReadOnly)
    float DamageDealt;

    // 消耗�?資�?
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> ResourceCost;

    // ?��X��?�?    UPROPERTY(BlueprintReadOnly)
    int32 ExperienceGained;

    // ?�誤信息
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
 * ?��X�制X * 管�X��X��X�控?��?輯�?行為
 */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API AMingUnitController : public AActor
{
    GENERATED_BODY()

public:
    AMingUnitController(};

    /**
     * ?��X�單位控?�器
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    void InitializeUnitController(};

    /**
     * ?��X��X�制X     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    void ShutdownUnitController(};

    /**
     * 添�X�控?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    bool AddControlledUnit(AMingTacticalUnit* Unit};

    /**
     * 移除?�控?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    bool RemoveControlledUnit(AMingTacticalUnit* Unit};

    /**
     * ?��X�?��X�單�?     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    TArray<AMingTacticalUnit*> GetControlledUnits() const;

    /**
     * ?��?定�X��X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    TArray<AMingTacticalUnit*> GetUnitsByType(const FString& UnitType) const;

    /**
     * ?��?活�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    TArray<AMingTacticalUnit*> GetActiveUnits() const;

    /**
     * ?��X��X�令
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult IssueUnitCommand(const FUnitCommand& Command};

    /**
     * ?��X��X�令
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    TArray<FUnitControlResult> BatchIssueCommands(const TArray<FUnitCommand>& Commands};

    /**
     * ?��X�令
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    bool CancelCommand(const FString& CommandID};

    /**
     * ?��X�?�命�?     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    bool CancelAllCommands(};

    /**
     * 移�X��X��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult MoveUnitsToLocation(const TArray<AMingTacticalUnit*>& Units, const FVector& TargetLocation, EUnitMovementType MovementType};

    /**
     * ?��X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult AttackTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetUnit, EUnitAttackType AttackType};

    /**
     * ?��?建�?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult AttackBuilding(const TArray<AMingTacticalUnit*>& Units, AActor* TargetBuilding, EUnitAttackType AttackType};

    /**
     * ?�止?�?��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult StopUnits(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ?�禦位置
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult HoldPosition(const TArray<AMingTacticalUnit*>& Units, const FVector& HoldLocation};

    /**
     * 巡�?路�?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult PatrolPath(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& PatrolPoints};

    /**
     * 守護?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult GuardTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetToGuard};

    /**
     * ?�退
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult RetreatUnits(const TArray<AMingTacticalUnit*>& Units, const FVector& RetreatLocation};

    /**
     * 組�?編�?
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult FormUpUnits(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& FormationCenter};

    /**
     * 使用?�X     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult UseAbility(const TArray<AMingTacticalUnit*>& Units, const FString& AbilityID, AMingTacticalUnit* TargetUnit = nullptr};

    /**
     * ?��X��X�X     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    EMingUnitState GetUnitState(AMingTacticalUnit* Unit) const;

    /**
     * 設置?��X�X     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    bool SetUnitState(AMingTacticalUnit* Unit, EMingUnitState NewState};

    /**
     * ?��X��X�令?��?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    TArray<FUnitCommand> GetUnitCommandQueue(AMingTacticalUnit* Unit) const;

    /**
     * ?��X��X�令
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    FUnitCommand GetCurrentCommand(AMingTacticalUnit* Unit) const;

    /**
     * 檢查?��X�否?�執行命�?     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    bool CanExecuteCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command) const;

    /**
     * ?��X��X�制統�?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    TMap<FString, int32> GetUnitControlStatistics() const;

    /**
     * ?��X�制?�性能?��?
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    TMap<FString, float> GetPerformanceMetrics() const;

protected:
    // ?�控?��X�表
    UPROPERTY()
    TArray<AMingTacticalUnit*> ControlledUnits;

    // 注�?：無 BlueprintType，避?��X��?稱�?�?    UPROPERTY()
    TMap<AMingTacticalUnit*, EMingUnitState> UnitStates;

    // ?��X�令?��? - 注�?：TMap<TArray> 不支XUPROPERTY
    TMap<AMingTacticalUnit*, TArray<FUnitCommand>> UnitCommandQueues;

    // ?��X��X�令
    UPROPERTY()
    TMap<AMingTacticalUnit*, FUnitCommand> CurrentCommands;

    // ?��X�制統�?
    UPROPERTY()
    TMap<FString, int32> ControlStatistics;

    // ?�能?��?
    UPROPERTY()
    TMap<FString, float> PerformanceMetrics;

    // ?��?管�X��X    UPROPERTY()
    UMingSelectionManager* SelectionManager;

    // ?�否已�?始�?
    bool bIsInitialized;

    // Tick?�新
    virtual void Tick(float DeltaTime) override;

    // ?��X��X�令
    void ExecuteUnitCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?��?移�X�令
    void HandleMoveCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?��X��X�令
    void HandleAttackCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?��X�止?�令
    void HandleStopCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?��X�禦?�令
    void HandleHoldCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?��?巡�X�令
    void HandlePatrolCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?��?守護?�令
    void HandleGuardCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?��X�退?�令
    void HandleRetreatCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?��?建設?�令
    void HandleBuildCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?��?修�X�令
    void HandleRepairCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?��?治�X�令
    void HandleHealCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?��X�獲?�令
    void HandleCaptureCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?��X�?�命�?    void HandleAbilityCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?��?編�X�令
    void HandleFormUpCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // 檢查?�令完�?
    bool IsCommandCompleted(AMingTacticalUnit* Unit, const FUnitCommand& Command) const;

    // 檢查?�令失�?
    bool IsCommandFailed(AMingTacticalUnit* Unit, const FUnitCommand& Command) const;

    // ?��X�令完�?
    void OnCommandCompleted(AMingTacticalUnit* Unit, const FUnitCommand& Command};

    // ?��X�令失�?
    void OnCommandFailed(AMingTacticalUnit* Unit, const FUnitCommand& Command, const FString& ErrorReason};

    // ?�新?��X�X    void UpdateUnitState(AMingTacticalUnit* Unit};

    // ?��?下�X�命�?    FUnitCommand GetNextCommand(AMingTacticalUnit* Unit};

    // 清�?已�X�命�?    void CleanupCompletedCommands(AMingTacticalUnit* Unit};

    // 計�?路�?
    TArray<FVector> CalculatePath(AMingTacticalUnit* Unit, const FVector& TargetLocation};

    // 檢查路�X��X    bool IsPathValid(AMingTacticalUnit* Unit, const TArray<FVector>& Path) const;

    // 計�X��?範�?
    float GetAttackRange(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit) const;

    // 檢查?��?範�?
    bool IsInAttackRange(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit) const;

    // 計�?移�X�度
    float GetMovementSpeed(AMingTacticalUnit* Unit, EUnitMovementType MovementType) const;

    // 計�X��X�害
    float CalculateAttackDamage(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit, EUnitAttackType AttackType) const;

    // ?�用?�害
    void ApplyDamage(AMingTacticalUnit* TargetUnit, float Damage, AMingTacticalUnit* AttackerUnit};

    // 檢查?��X��X�X    bool IsUnitAlive(AMingTacticalUnit* Unit) const;

    // ?��X��?位置
    FVector GetUnitLocation(AMingTacticalUnit* Unit) const;

    // 設置?��?位置
    void SetUnitLocation(AMingTacticalUnit* Unit, const FVector& NewLocation};

    // ?��X��X��?
    FRotator GetUnitRotation(AMingTacticalUnit* Unit) const;

    // 設置?��X��?
    void SetUnitRotation(AMingTacticalUnit* Unit, const FRotator& NewRotation};

    // ?�新?�能?��?
    void UpdatePerformanceMetrics(};

    // 記�X�制事件
    void LogControlEvent(const FString& EventType, const FString& Details};

    // ?��X�制?��?
    FString GenerateControlReport() const;

    // 驗�X��?
    bool ValidateUnit(AMingTacticalUnit* Unit) const;

    // 驗�X�令
    bool ValidateCommand(const FUnitCommand& Command) const;

    // ?��X�令?�述
    FString GetCommandDescription(const FUnitCommand& Command) const;

    // ?��X�?��?�?    FString GetStateDescription(EUnitState State) const;

    // 計�?編�?位置
    TArray<FVector> CalculateFormationPositions(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& Center};

    // 檢查編�?完整X    bool IsFormationComplete(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions};

    // ?�新編�?
    void UpdateFormation(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions};
};

