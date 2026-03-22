#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MingTacticalUnit.generated.h"

/**
 * 單位狀態枚舉
 */
UENUM(BlueprintType)
enum class EUnitState : uint8
{
    Idle              UMETA(DisplayName = "Idle"),
    Moving            UMETA(DisplayName = "Moving"),
    Attacking         UMETA(DisplayName = "Attacking"),
    TakingDamage      UMETA(DisplayName = "Taking Damage"),
    Dead              UMETA(DisplayName = "Dead"),
    Stunned           UMETA(DisplayName = "Stunned")
};

/**
 * 單位類型枚舉
 */
UENUM(BlueprintType)
enum class EUnitType : uint8
{
    Infantry          UMETA(DisplayName = "Infantry"),
    Cavalry           UMETA(DisplayName = "Cavalry"),
    Artillery         UMETA(DisplayName = "Artillery"),
    Support           UMETA(DisplayName = "Support"),
    Commander         UMETA(DisplayName = "Commander")
};

/**
 * 單位屬性結構
 */
USTRUCT(BlueprintType)
struct FUnitStats
{
    GENERATED_BODY()
    
    // 生命值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHealth;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    float CurrentHealth;
    
    // 移動速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MoveSpeed;
    
    // 攻擊力
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float AttackDamage;
    
    // 攻擊範圍
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float AttackRange;
    
    // 攻擊速度 (每秒攻擊次數)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float AttackSpeed;
    
    // 防禦力
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Defense;
    
    // 視野範圍
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float SightRange;
    
    FUnitStats()
        : MaxHealth(100.0f)
        , CurrentHealth(100.0f)
        , MoveSpeed(300.0f)
        , AttackDamage(20.0f)
        , AttackRange(100.0f)
        , AttackSpeed(1.0f)
        , Defense(5.0f)
        , SightRange(500.0f)
    {}
};

class UMingUnitMovementComponent;
class UMingUnitCombatComponent;

/**
 * 戰術單位基類
 * 大規模戰鬥系統的核心單位
 */
UCLASS()
class MINGTACTICAL_API AMingTacticalUnit : public AActor
{
    GENERATED_BODY()

public:
    AMingTacticalUnit();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // 單位初始化
    UFUNCTION(BlueprintCallable, Category = "Unit")
    void InitializeUnit(EUnitType InUnitType, int32 InTeamId);

    // 選擇相關
    UFUNCTION(BlueprintCallable, Category = "Unit|Selection")
    void SetSelected(bool bSelected);

    UFUNCTION(BlueprintPure, Category = "Unit|Selection")
    bool IsSelected() const { return bIsSelected; }

    // 生命值相關
    UFUNCTION(BlueprintCallable, Category = "Unit|Health")
    void TakeDamage(float DamageAmount, AActor* DamageCauser);

    UFUNCTION(BlueprintCallable, Category = "Unit|Health")
    void Heal(float HealAmount);

    UFUNCTION(BlueprintPure, Category = "Unit|Health")
    bool IsAlive() const { return UnitStats.CurrentHealth > 0; }

    UFUNCTION(BlueprintPure, Category = "Unit|Health")
    float GetHealthPercent() const;

    // 狀態相關
    UFUNCTION(BlueprintCallable, Category = "Unit|State")
    void ChangeState(EUnitState NewState);

    UFUNCTION(BlueprintPure, Category = "Unit|State")
    EUnitState GetCurrentState() const { return CurrentState; }

    // 獲取組件
    UFUNCTION(BlueprintPure, Category = "Unit")
    UMingUnitMovementComponent* GetMovementComponent() const { return MovementComponent; }

    UFUNCTION(BlueprintPure, Category = "Unit")
    UMingUnitCombatComponent* GetCombatComponent() const { return CombatComponent; }

    // 單位ID (用於事件系統)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit")
    int32 UnitId;

    // 隊伍ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    int32 TeamId;

    // 單位類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    EUnitType UnitType;

    // 單位屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    FUnitStats UnitStats;

    // 靜態計數器 (用於生成唯一ID)
    static int32 GlobalUnitCounter;

protected:
    // 組件
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> UnitMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UMingUnitMovementComponent> MovementComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UMingUnitCombatComponent> CombatComponent;

    // 選擇狀態
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit")
    bool bIsSelected;

    // 當前狀態
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit")
    EUnitState CurrentState;

    // 更新選擇視覺效果
    void UpdateSelectionVisuals();

    // 死亡處理
    UFUNCTION()
    void OnDeath();

    // 發布單位選擇事件
    void PublishUnitSelectedEvent();

    // 發布單位死亡事件
    void PublishUnitDeathEvent();
};
