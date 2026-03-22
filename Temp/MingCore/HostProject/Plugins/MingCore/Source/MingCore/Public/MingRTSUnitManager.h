#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSUnitManager.generated.h"

class AMingGoRTSUnit;
class AMingGoRTSPlayerController;

UENUM(BlueprintType)
enum class ERTSUnitType : uint8
{
    Infantry,       // 步兵
    Cavalry,        // 騎兵
    Artillery,      // 火炮
    Scout,          // 斥候
    Engineer,       // 工兵
    Medic,          // 醫療兵
    Commander,      // 指揮官
    Supply          // 補給單位
};

UENUM(BlueprintType)
enum class ERTSUnitState : uint8
{
    Idle,           // 空閒
    Moving,         // 移動中
    Attacking,      // 攻擊中
    Defending,      // 防禦中
    Retreating,     // 撤退中
    Dead,           // 死亡
    Building        // 建造中
};

UENUM(BlueprintType)
enum class ERTSFormationType : uint8
{
    None,           // 無陣型
    Line,           // 線形陣型
    Column,         // 縱隊陣型
    Wedge,          // 楔形陣型
    Circle,         // 圓形陣型
    Square          // 方形陣型
};

USTRUCT(BlueprintType)
struct FRTSUnitData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Unit Data")
    FString UnitID;

    UPROPERTY(BlueprintReadOnly, Category = "Unit Data")
    ERTSUnitType UnitType;

    UPROPERTY(BlueprintReadOnly, Category = "Unit Data")
    ERTSUnitState CurrentState;

    UPROPERTY(BlueprintReadOnly, Category = "Unit Data")
    FVector CurrentLocation;

    UPROPERTY(BlueprintReadOnly, Category = "Unit Data")
    FVector TargetLocation;

    UPROPERTY(BlueprintReadOnly, Category = "Unit Data")
    float Health;

    UPROPERTY(BlueprintReadOnly, Category = "Unit Data")
    float MaxHealth;

    UPROPERTY(BlueprintReadOnly, Category = "Unit Data")
    float AttackPower;

    UPROPERTY(BlueprintReadOnly, Category = "Unit Data")
    float DefensePower;

    UPROPERTY(BlueprintReadOnly, Category = "Unit Data")
    float MovementSpeed;

    UPROPERTY(BlueprintReadOnly, Category = "Unit Data")
    float AttackRange;

    UPROPERTY(BlueprintReadOnly, Category = "Unit Data")
    int32 UnitLevel;

    UPROPERTY(BlueprintReadOnly, Category = "Unit Data")
    bool bIsSelected;

    UPROPERTY(BlueprintReadOnly, Category = "Unit Data")
    FDateTime LastActionTime;

    FRTSUnitData()
    {
        UnitID = TEXT("");
        UnitType = ERTSUnitType::Infantry;
        CurrentState = ERTSUnitState::Idle;
        CurrentLocation = FVector::ZeroVector;
        TargetLocation = FVector::ZeroVector;
        Health = 100.0f;
        MaxHealth = 100.0f;
        AttackPower = 10.0f;
        DefensePower = 5.0f;
        MovementSpeed = 300.0f;
        AttackRange = 200.0f;
        UnitLevel = 1;
        bIsSelected = false;
    }
};

USTRUCT(BlueprintType)
struct FRTSFormationData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Formation Data")
    ERTSFormationType FormationType;

    UPROPERTY(BlueprintReadOnly, Category = "Formation Data")
    FVector FormationCenter;

    UPROPERTY(BlueprintReadOnly, Category = "Formation Data")
    float FormationSpacing;

    UPROPERTY(BlueprintReadOnly, Category = "Formation Data")
    FRotator FormationRotation;

    UPROPERTY(BlueprintReadOnly, Category = "Formation Data")
    TArray<FVector> UnitPositions;

    FRTSFormationData()
    {
        FormationType = ERTSFormationType::None;
        FormationCenter = FVector::ZeroVector;
        FormationSpacing = 100.0f;
        FormationRotation = FRotator::ZeroRotator;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitSelected, AMingGoRTSUnit*, Unit, bool, bIsSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitStateChanged, AMingGoRTSUnit*, Unit, ERTSUnitState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUnitsMoved, const TArray<AMingGoRTSUnit*>&, Units, const FVector&, TargetLocation, bool, bIsFormation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitDamaged, AMingGoRTSUnit*, Unit);

/**
 * RTS單位管理器
 * 管理所有RTS單位的選擇、移動、陣型和狀態
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSUnitManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSUnitManager();

    // 初始化
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void InitializeUnitManager(AMingGoRTSPlayerController* InPlayerController);

    // 單位管理
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void RegisterUnit(AMingGoRTSUnit* Unit);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void UnregisterUnit(AMingGoRTSUnit* Unit);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    AMingGoRTSUnit* GetUnit(const FString& UnitID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    TArray<AMingGoRTSUnit*> GetAllUnits() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    TArray<AMingGoRTSUnit*> GetUnitsByType(ERTSUnitType UnitType) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    TArray<AMingGoRTSUnit*> GetUnitsByState(ERTSUnitState State) const;

    // 選擇系統
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void SelectUnit(AMingGoRTSUnit* Unit, bool bAddToSelection = false);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void SelectUnits(const TArray<AMingGoRTSUnit*>& Units);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void SelectUnitsInRect(FVector2D ScreenStart, FVector2D ScreenEnd);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void DeselectAllUnits();

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void DeselectUnit(AMingGoRTSUnit* Unit);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    TArray<AMingGoRTSUnit*> GetSelectedUnits() const { return SelectedUnits; }

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    int32 GetSelectedUnitCount() const { return SelectedUnits.Num(); }

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    bool IsUnitSelected(AMingGoRTSUnit* Unit) const;

    // 移動系統
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void MoveUnits(const TArray<AMingGoRTSUnit*>& Units, const FVector& TargetLocation);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void MoveSelectedUnits(const FVector& TargetLocation);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void StopUnitMovement(AMingGoRTSUnit* Unit);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void StopAllUnitMovement();

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    bool IsUnitMoving(AMingGoRTSUnit* Unit) const;

    // 陣型系統
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void SetFormation(ERTSFormationType FormationType);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void ApplyFormationToSelectedUnits(const FVector& Center);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    FRTSFormationData CalculateFormation(ERTSFormationType FormationType, const TArray<AMingGoRTSUnit*>& Units, const FVector& Center);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void UpdateFormation();

    // 狀態管理
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void SetUnitState(AMingGoRTSUnit* Unit, ERTSUnitState NewState);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    ERTSUnitState GetUnitState(AMingGoRTSUnit* Unit) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void UpdateUnitStates(float DeltaTime);

    // 戰鬥系統
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void AttackUnit(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void StopAttack(AMingGoRTSUnit* Unit);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    bool CanAttackUnit(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    TArray<AMingGoRTSUnit*> GetUnitsInRange(AMingGoRTSUnit* Unit, float Range) const;

    // 單位數據
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    FRTSUnitData GetUnitData(AMingGoRTSUnit* Unit) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void SetUnitData(AMingGoRTSUnit* Unit, const FRTSUnitData& UnitData);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void UpdateUnitHealth(AMingGoRTSUnit* Unit, float HealthChange);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    bool IsUnitAlive(AMingGoRTSUnit* Unit) const;

    // 路徑規劃
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    TArray<FVector> CalculatePath(AMingGoRTSUnit* Unit, const FVector& TargetLocation);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    bool HasValidPath(AMingGoRTSUnit* Unit) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void RecalculatePaths();

    // 群組控制
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void CreateUnitGroup(const TArray<AMingGoRTSUnit*>& Units, const FString& GroupName);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    void SelectUnitGroup(const FString& GroupName);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Manager")
    TArray<FString> GetUnitGroups() const;

    // 統計信息
    UFUNCTION(BlueprintPure, Category = "RTS Unit Manager")
    int32 GetTotalUnitCount() const;

    UFUNCTION(BlueprintPure, Category = "RTS Unit Manager")
    int32 GetUnitCountByType(ERTSUnitType UnitType) const;

    UFUNCTION(BlueprintPure, Category = "RTS Unit Manager")
    int32 GetAliveUnitCount() const;

    UFUNCTION(BlueprintPure, Category = "RTS Unit Manager")
    int32 GetMovingUnitCount() const;

    UFUNCTION(BlueprintPure, Category = "RTS Unit Manager")
    int32 GetAttackingUnitCount() const;

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "RTS Unit Events")
    FOnUnitSelected OnUnitSelected;

    UPROPERTY(BlueprintAssignable, Category = "RTS Unit Events")
    FOnUnitStateChanged OnUnitStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "RTS Unit Events")
    FOnUnitsMoved OnUnitsMoved;

    UPROPERTY(BlueprintAssignable, Category = "RTS Unit Events")
    FOnUnitDamaged OnUnitDamaged;

protected:
    // 系統引用
    UPROPERTY()
    TObjectPtr<AMingGoRTSPlayerController> PlayerController;

    // 單位存儲
    // 注意：AMingGoRTSUnit 是主專案類型，不適合 UPROPERTY
    TMap<FString, AMingGoRTSUnit*> AllUnits;

    // 注意：AMingGoRTSUnit 是主專案類型，不適合 UPROPERTY
    TArray<AMingGoRTSUnit*> SelectedUnits;

    // 注意：TMap<TArray> 不支持 UPROPERTY
    TMap<FString, TArray<AMingGoRTSUnit*>> UnitGroups;

    // 陣型數據
    UPROPERTY()
    ERTSFormationType CurrentFormation;

    UPROPERTY()
    FRTSFormationData CurrentFormationData;

    // 狀態
    UPROPERTY()
    bool bIsInitialized;

    // 設置
    UPROPERTY(BlueprintReadWrite, Category = "RTS Settings")
    float SelectionBoxMargin = 50.0f;

    UPROPERTY(BlueprintReadWrite, Category = "RTS Settings")
    float FormationSpacing = 150.0f;

    UPROPERTY(BlueprintReadWrite, Category = "RTS Settings")
    bool bAutoFormation = true;

    // 內部函數
    void UpdateUnitSelection();
    void ProcessUnitMovement(float DeltaTime);
    void ProcessCombat(float DeltaTime);
    FVector CalculateFormationPosition(AMingGoRTSUnit* Unit, const FRTSFormationData& Formation) const;
    bool IsUnitInSelectionRect(AMingGoRTSUnit* Unit, FVector2D ScreenStart, FVector2D ScreenEnd) const;
    void CleanupDeadUnits();
    FString GenerateUnitID(ERTSUnitType UnitType) const;

private:
    // 輔助函數
    void NotifyUnitSelected(AMingGoRTSUnit* Unit, bool bIsSelected);
    void NotifyUnitStateChanged(AMingGoRTSUnit* Unit, ERTSUnitState NewState);
    void NotifyUnitsMoved(const TArray<AMingGoRTSUnit*>& Units, const FVector& TargetLocation, bool bIsFormation);
    void NotifyUnitDamaged(AMingGoRTSUnit* Unit);
};
