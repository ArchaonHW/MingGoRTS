#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingObjectPoolSystem.generated.h"

class AMingTacticalUnit;
class AMingCombatAI;

/**
 * 池對象狀態 | Pool Object State
 */
UENUM(BlueprintType)
enum class EPoolObjectState : uint8
{
    Unused      UMETA(DisplayName = "Unused"),      // 未使用
    Available   UMETA(DisplayName = "Available"),   // 可用 | Available
    InUse       UMETA(DisplayName = "In Use"),      // 使用中 | In Use
    Pending     UMETA(DisplayName = "Pending Return"), // 待歸還 | Pending Return
    Disabled    UMETA(DisplayName = "Disabled")     // 禁用 | Disabled
};

/**
 * 單位池項 | Unit Pool Item
 */
USTRUCT()
struct MINGTACTICAL_API FUnitPoolItem
{
    GENERATED_BODY()

    UPROPERTY()
    TWeakObjectPtr<AMingTacticalUnit> Unit;

    UPROPERTY()
    EPoolObjectState State;

    UPROPERTY()
    float LastUsedTime;

    UPROPERTY()
    int32 UseCount;

    FUnitPoolItem()
        : State(EPoolObjectState::Unused)
        , LastUsedTime(0.0f)
        , UseCount(0)
    {}
};

/**
 * AI池項 | AI Pool Item
 */
USTRUCT()
struct MINGTACTICAL_API FAIPoolItem
{
    GENERATED_BODY()

    UPROPERTY()
    TWeakObjectPtr<AMingCombatAI> AI;

    UPROPERTY()
    EPoolObjectState State;

    UPROPERTY()
    float LastUsedTime;

    UPROPERTY()
    int32 UseCount;

    FAIPoolItem()
        : State(EPoolObjectState::Unused)
        , LastUsedTime(0.0f)
        , UseCount(0)
    {}
};

/**
 * 對象池系統 | Object Pool System
 * 
 * 優化大量單位/AI的創建和銷毀性能： | Optimize creation/destruction performance:
 * - 預分配對象 | Pre-allocation
 * - 復用已銷毀的對象 | Object reuse
 * - 控制內存佔用 | Memory management
 * 
 * 適用於 1000+ 單位的大規模戰鬥場景 | Suitable for 1000+ unit scenarios
 */
UCLASS(ClassGroup = (Optimization), Blueprintable)
class MINGTACTICAL_API UMingObjectPoolSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingObjectPoolSystem();

    /**
     * 初始化对象池
     * @param InitialUnitPoolSize 初始单位池大小
     * @param InitialAIPoolSize 初始AI池大小
     * @param MaxPoolSize 最大池大小
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    void Initialize(int32 InitialUnitPoolSize = 100, int32 InitialAIPoolSize = 20, int32 MaxPoolSize = 5000);

    /**
     * 关闭对象池系统
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    void Shutdown();

    /**
     * 预填充对象池
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    void PrepopulatePools();

    // ==== 单位池操作 ====

    /**
     * 从池获取一个单位
     * @return 可用的单位实例，如果没有则返回 nullptr
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Units")
    AMingTacticalUnit* AcquireUnit(UClass* UnitClass = nullptr);

    /**
     * 将单位归还到池中
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Units")
    void ReturnUnit(AMingTacticalUnit* Unit);

    /**
     * 批量获取单位
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Units")
    TArray<AMingTacticalUnit*> AcquireUnits(int32 Count, UClass* UnitClass = nullptr);

    /**
     * 批量归还单位
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Units")
    void ReturnUnits(const TArray<AMingTacticalUnit*>& Units);

    // ==== AI池操作 ====

    /**
     * 从池获取一个AI
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|AI")
    AMingCombatAI* AcquireAI(UClass* AIClass = nullptr);

    /**
     * 将AI归还到池中
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|AI")
    void ReturnAI(AMingCombatAI* AI);

    // ==== 池管理 ====

    /**
     * 扩展单位池
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Management")
    void ExpandUnitPool(int32 AdditionalCount);

    /**
     * 扩展AI池
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Management")
    void ExpandAIPool(int32 AdditionalCount);

    /**
     * 收缩池（删除未使用的对象）
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Management")
    void ShrinkUnusedPools();

    /**
     * 强制清理所有对象（慎用）
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Management")
    void ForceClearAll();

    /**
     * 设置最大池大小限制
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Management")
    void SetMaxPoolSize(int32 NewMaxSize);

    /**
     * 设置自动扩展阈值
     * 当可用对象低于此百分比时自动扩展
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Management")
    void SetAutoExpandThreshold(float Percentage);

    /**
     * 启用/禁用自动扩展
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Management")
    void SetAutoExpandEnabled(bool bEnabled);

    // ==== 统计信息 ====

    UFUNCTION(BlueprintPure, Category = "Object Pool|Stats")
    int32 GetTotalUnitPoolSize() const { return UnitPool.Num(); }

    UFUNCTION(BlueprintPure, Category = "Object Pool|Stats")
    int32 GetAvailableUnitCount() const;

    UFUNCTION(BlueprintPure, Category = "Object Pool|Stats")
    int32 GetInUseUnitCount() const;

    UFUNCTION(BlueprintPure, Category = "Object Pool|Stats")
    int32 GetTotalAIPoolSize() const { return AIPool.Num(); }

    UFUNCTION(BlueprintPure, Category = "Object Pool|Stats")
    int32 GetAvailableAICount() const;

    UFUNCTION(BlueprintPure, Category = "Object Pool|Stats")
    int32 GetInUseAICount() const;

    UFUNCTION(BlueprintPure, Category = "Object Pool|Stats")
    float GetUnitPoolUtilization() const;

    UFUNCTION(BlueprintPure, Category = "Object Pool|Stats")
    float GetAIPoolUtilization() const;

    UFUNCTION(BlueprintPure, Category = "Object Pool|Stats")
    int32 GetTotalCreatedCount() const { return TotalCreatedCount; }

    UFUNCTION(BlueprintPure, Category = "Object Pool|Stats")
    int32 GetTotalReusedCount() const { return TotalReusedCount; }

    UFUNCTION(BlueprintPure, Category = "Object Pool|Stats")
    float GetAverageReuseRate() const;

    // ==== 调试 ====

    UFUNCTION(BlueprintCallable, Category = "Object Pool|Debug")
    void PrintDebugInfo();

    UFUNCTION(BlueprintCallable, Category = "Object Pool|Debug")
    FString GetDebugString() const;

    UFUNCTION(BlueprintCallable, Category = "Object Pool|Debug")
    bool ValidateIntegrity();

    UFUNCTION(BlueprintCallable, Category = "Object Pool|Debug")
    void DumpPoolContents();

    /**
     * Tick 更新（定期清理和检查）
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    void Tick(float DeltaTime);

private:
    // 单位池
    UPROPERTY()
    TArray<FUnitPoolItem> UnitPool;

    // AI池
    UPROPERTY()
    TArray<FAIPoolItem> AIPool;

    // 配置
    int32 MaxPoolSize;
    int32 InitialUnitPoolSize;
    int32 InitialAIPoolSize;
    float AutoExpandThreshold;
    bool bAutoExpandEnabled;
    bool bIsInitialized;

    // 统计
    int32 TotalCreatedCount;
    int32 TotalReusedCount;
    int32 TotalUnitAcquires;
    int32 TotalAIAcquires;

    // 可用物件索引快取（避免線性搜尋）
    TArray<int32> AvailableUnitIndices;
    TArray<int32> AvailableAIIndices;

    // 扩展冷却（防止过度扩展）
    float LastExpandTime;
    float ExpandCooldown;

    // 单位类（默认）
    UPROPERTY()
    TSubclassOf<AMingTacticalUnit> DefaultUnitClass;

    UPROPERTY()
    TSubclassOf<AMingCombatAI> DefaultAIClass;

    // 创建新单位
    AMingTacticalUnit* CreateNewUnit(UClass* UnitClass);

    // 创建新AI
    AMingCombatAI* CreateNewAI(UClass* AIClass);

    // 重置单位状态
    void ResetUnit(AMingTacticalUnit* Unit);

    // 重置AI状态
    void ResetAI(AMingCombatAI* AI);

    // 真正销毁对象（归还到引擎）
    void DestroyUnit(AMingTacticalUnit* Unit);
    void DestroyAI(AMingCombatAI* AI);

    // 检查是否需要自动扩展
    void CheckAutoExpand();

    // 清理无效引用
    void CleanupInvalidReferences();

    // 查找可用的池项
    int32 FindAvailableUnitIndex() const;
    int32 FindAvailableAIIndex() const;

    // 查找指定对象的池索引
    int32 FindUnitIndex(AMingTacticalUnit* Unit) const;
    int32 FindAIIndex(AMingCombatAI* AI) const;
};
