#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "MingUnitController.generated.h"

class AMingTacticalUnit;
class UMingSelectionManager;

/**
 * 單位狀態
 */
UENUM(BlueprintType)
enum class EUnitState : uint8
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
 * 單位命令類型
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
 * 單位命令優先級
 */
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
 * 單位移動類型
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
 * 單位攻擊類型
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
 * 單位編隊類型
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
 * 單位命令
 */
USTRUCT(BlueprintType)
struct FUnitCommand
{
    GENERATED_BODY()

    // 命令ID
    UPROPERTY(BlueprintReadOnly)
    FString CommandID;

    // 命令類型
    UPROPERTY(BlueprintReadOnly)
    EUnitCommandType CommandType;

    // 命令優先級
    UPROPERTY(BlueprintReadOnly)
    EUnitCommandPriority Priority;

    // 目標位置
    UPROPERTY(BlueprintReadOnly)
    FVector TargetLocation;

    // 目標單位
    UPROPERTY(BlueprintReadOnly)
    AMingTacticalUnit* TargetUnit;

    // 目標建築
    UPROPERTY(BlueprintReadOnly)
    AActor* TargetBuilding;

    // 移動類型
    UPROPERTY(BlueprintReadOnly)
    EUnitMovementType MovementType;

    // 攻擊類型
    UPROPERTY(BlueprintReadOnly)
    EUnitAttackType AttackType;

    // 編隊類型
    UPROPERTY(BlueprintReadOnly)
    EUnitFormationType FormationType;

    // 命令參數
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CommandParameters;

    // 命令時間
    UPROPERTY(BlueprintReadOnly)
    float CommandTime;

    // 執行時間
    UPROPERTY(BlueprintReadOnly)
    float ExecuteTime;

    // 是否已執行
    UPROPERTY(BlueprintReadOnly)
    bool bIsExecuted;

    // 是否已完成
    UPROPERTY(BlueprintReadOnly)
    bool bIsCompleted;

    // 是否已取消
    UPROPERTY(BlueprintReadOnly)
    bool bIsCancelled;

    // 重試次數
    UPROPERTY(BlueprintReadOnly)
    int32 RetryCount;

    // 最大重試次數
    UPROPERTY(BlueprintReadOnly)
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
 * 單位控制結果
 */
USTRUCT(BlueprintType)
struct FUnitControlResult
{
    GENERATED_BODY()

    // 是否成功
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // 命令ID
    UPROPERTY(BlueprintReadOnly)
    FString CommandID;

    // 執行時間
    UPROPERTY(BlueprintReadOnly)
    float ExecuteTime;

    // 結果描述
    UPROPERTY(BlueprintReadOnly)
    FString ResultDescription;

    // 影響的單位
    UPROPERTY(BlueprintReadOnly)
    TArray<AMingTacticalUnit*> AffectedUnits;

    // 影響的建築
    UPROPERTY(BlueprintReadOnly)
    TArray<AActor*> AffectedBuildings;

    // 造成的傷害
    UPROPERTY(BlueprintReadOnly)
    float DamageDealt;

    // 消耗的資源
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> ResourceCost;

    // 獲得的經驗
    UPROPERTY(BlueprintReadOnly)
    int32 ExperienceGained;

    // 錯誤信息
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
 * 單位控制器
 * 管理單位的所有控制邏輯和行為
 */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API AMingUnitController : public AActor
{
    GENERATED_BODY()

public:
    AMingUnitController();

    /**
     * 初始化單位控制器
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    void InitializeUnitController();

    /**
     * 關閉單位控制器
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    void ShutdownUnitController();

    /**
     * 添加受控單位
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    bool AddControlledUnit(AMingTacticalUnit* Unit);

    /**
     * 移除受控單位
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    bool RemoveControlledUnit(AMingTacticalUnit* Unit);

    /**
     * 獲取所有受控單位
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    TArray<AMingTacticalUnit*> GetControlledUnits() const;

    /**
     * 獲指定類型的單位
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    TArray<AMingTacticalUnit*> GetUnitsByType(const FString& UnitType) const;

    /**
     * 獲取活躍單位
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    TArray<AMingTacticalUnit*> GetActiveUnits() const;

    /**
     * 發布單位命令
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult IssueUnitCommand(const FUnitCommand& Command);

    /**
     * 批量發布命令
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    TArray<FUnitControlResult> BatchIssueCommands(const TArray<FUnitCommand>& Commands);

    /**
     * 取消命令
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    bool CancelCommand(const FString& CommandID);

    /**
     * 取消所有命令
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    bool CancelAllCommands();

    /**
     * 移動單位到位置
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult MoveUnitsToLocation(const TArray<AMingTacticalUnit*>& Units, const FVector& TargetLocation, EUnitMovementType MovementType);

    /**
     * 攻擊目標
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult AttackTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetUnit, EUnitAttackType AttackType);

    /**
     * 攻擊建築
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult AttackBuilding(const TArray<AMingTacticalUnit*>& Units, AActor* TargetBuilding, EUnitAttackType AttackType);

    /**
     * 停止所有動作
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult StopUnits(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 防禦位置
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult HoldPosition(const TArray<AMingTacticalUnit*>& Units, const FVector& HoldLocation);

    /**
     * 巡邏路徑
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult PatrolPath(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& PatrolPoints);

    /**
     * 守護目標
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult GuardTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetToGuard);

    /**
     * 撤退
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult RetreatUnits(const TArray<AMingTacticalUnit*>& Units, const FVector& RetreatLocation);

    /**
     * 組織編隊
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult FormUpUnits(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& FormationCenter);

    /**
     * 使用技能
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    FUnitControlResult UseAbility(const TArray<AMingTacticalUnit*>& Units, const FString& AbilityID, AMingTacticalUnit* TargetUnit = nullptr);

    /**
     * 獲取單位狀態
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    EUnitState GetUnitState(AMingTacticalUnit* Unit) const;

    /**
     * 設置單位狀態
     */
    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    bool SetUnitState(AMingTacticalUnit* Unit, EUnitState NewState);

    /**
     * 獲取單位命令隊列
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    TArray<FUnitCommand> GetUnitCommandQueue(AMingTacticalUnit* Unit) const;

    /**
     * 獲取當前命令
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    FUnitCommand GetCurrentCommand(AMingTacticalUnit* Unit) const;

    /**
     * 檢查單位是否可執行命令
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    bool CanExecuteCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command) const;

    /**
     * 獲取單位控制統計
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    TMap<FString, int32> GetUnitControlStatistics() const;

    /**
     * 獲取控制器性能指標
     */
    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    TMap<FString, float> GetPerformanceMetrics() const;

protected:
    // 受控單位列表
    UPROPERTY()
    TArray<AMingTacticalUnit*> ControlledUnits;

    // 單位狀態映射
    UPROPERTY()
    TMap<AMingTacticalUnit*, EUnitState> UnitStates;

    // 單位命令隊列
    UPROPERTY()
    TMap<AMingTacticalUnit*, TArray<FUnitCommand>> UnitCommandQueues;

    // 當前執行命令
    UPROPERTY()
    TMap<AMingTacticalUnit*, FUnitCommand> CurrentCommands;

    // 單位控制統計
    UPROPERTY()
    TMap<FString, int32> ControlStatistics;

    // 性能指標
    UPROPERTY()
    TMap<FString, float> PerformanceMetrics;

    // 選擇管理器引用
    UPROPERTY()
    UMingSelectionManager* SelectionManager;

    // 是否已初始化
    bool bIsInitialized;

    // Tick更新
    virtual void Tick(float DeltaTime) override;

    // 執行單位命令
    void ExecuteUnitCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    // 處理移動命令
    void HandleMoveCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    // 處理攻擊命令
    void HandleAttackCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    // 處理停止命令
    void HandleStopCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    // 處理防禦命令
    void HandleHoldCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    // 處理巡邏命令
    void HandlePatrolCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    // 處理守護命令
    void HandleGuardCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    // 處理撤退命令
    void HandleRetreatCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    // 處理建設命令
    void HandleBuildCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    // 處理修理命令
    void HandleRepairCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    // 處理治療命令
    void HandleHealCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    // 處理捕獲命令
    void HandleCaptureCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    // 處理技能命令
    void HandleAbilityCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    // 處理編隊命令
    void HandleFormUpCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    // 檢查命令完成
    bool IsCommandCompleted(AMingTacticalUnit* Unit, const FUnitCommand& Command) const;

    // 檢查命令失敗
    bool IsCommandFailed(AMingTacticalUnit* Unit, const FUnitCommand& Command) const;

    // 處理命令完成
    void OnCommandCompleted(AMingTacticalUnit* Unit, const FUnitCommand& Command);

    // 處理命令失敗
    void OnCommandFailed(AMingTacticalUnit* Unit, const FUnitCommand& Command, const FString& ErrorReason);

    // 更新單位狀態
    void UpdateUnitState(AMingTacticalUnit* Unit);

    // 獲取下一個命令
    FUnitCommand GetNextCommand(AMingTacticalUnit* Unit);

    // 清理已完成命令
    void CleanupCompletedCommands(AMingTacticalUnit* Unit);

    // 計算路徑
    TArray<FVector> CalculatePath(AMingTacticalUnit* Unit, const FVector& TargetLocation);

    // 檢查路徑有效性
    bool IsPathValid(AMingTacticalUnit* Unit, const TArray<FVector>& Path) const;

    // 計算攻擊範圍
    float GetAttackRange(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit) const;

    // 檢查攻擊範圍
    bool IsInAttackRange(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit) const;

    // 計算移動速度
    float GetMovementSpeed(AMingTacticalUnit* Unit, EUnitMovementType MovementType) const;

    // 計算攻擊傷害
    float CalculateAttackDamage(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit, EUnitAttackType AttackType) const;

    // 應用傷害
    void ApplyDamage(AMingTacticalUnit* TargetUnit, float Damage, AMingTacticalUnit* AttackerUnit);

    // 檢查單位生存狀態
    bool IsUnitAlive(AMingTacticalUnit* Unit) const;

    // 獲取單位位置
    FVector GetUnitLocation(AMingTacticalUnit* Unit) const;

    // 設置單位位置
    void SetUnitLocation(AMingTacticalUnit* Unit, const FVector& NewLocation);

    // 獲取單位旋轉
    FRotator GetUnitRotation(AMingTacticalUnit* Unit) const;

    // 設置單位旋轉
    void SetUnitRotation(AMingTacticalUnit* Unit, const FRotator& NewRotation);

    // 更新性能指標
    void UpdatePerformanceMetrics();

    // 記錄控制事件
    void LogControlEvent(const FString& EventType, const FString& Details);

    // 生成控制報告
    FString GenerateControlReport() const;

    // 驗證單位
    bool ValidateUnit(AMingTacticalUnit* Unit) const;

    // 驗證命令
    bool ValidateCommand(const FUnitCommand& Command) const;

    // 獲取命令描述
    FString GetCommandDescription(const FUnitCommand& Command) const;

    // 獲取狀態描述
    FString GetStateDescription(EUnitState State) const;

    // 計算編隊位置
    TArray<FVector> CalculateFormationPositions(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& Center);

    // 檢查編隊完整性
    bool IsFormationComplete(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions);

    // 更新編隊
    void UpdateFormation(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions);
};
