#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingObjectPoolSystem.generated.h"

class AMingTacticalUnit;
class AMingCombatAI;

/**
 * 池�?象�X| Pool Object State
 */
UENUM(BlueprintType)
enum class EPoolObjectState : uint8
{
    Unused      UMETA(DisplayName = "Unused"),      // ?�使X    Available   UMETA(DisplayName = "Available"),   // ?�用 | Available
    InUse       UMETA(DisplayName = "In Use"),      // 使用�?| In Use
    Pending     UMETA(DisplayName = "Pending Return"), // 待歸X| Pending Return
    Disabled    UMETA(DisplayName = "Disabled")     // 禁用 | Disabled
};

/**
 * ?��?池�? | Unit Pool Item
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
 * AI池�? | AI Pool Item
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
 * 對象池系�?| Object Pool System
 * 
 * ?��?大�X��?/AI?�創建�X��X�能�?| Optimize creation/destruction performance:
 * - ?��X��?�?| Pre-allocation
 * - 復用已銷毀?��?�?| Object reuse
 * - ?�制?��?佔用 | Memory management
 * 
 * ?�用X1000+ ?��X�大規模?�鬥?�景 | Suitable for 1000+ unit scenarios
 */
UCLASS(ClassGroup = (Optimization), Blueprintable)
class MINGTACTICAL_API UMingObjectPoolSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingObjectPoolSystem(};

    /**
     * ?��X�对象�?
     * @param InitialUnitPoolSize ?��X��?池大�?     * @param InitialAIPoolSize ?��?AI池大�?     * @param MaxPoolSize ?�大�?大�?
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    void Initialize(int32 InitialUnitPoolSize = 100, int32 InitialAIPoolSize = 20, int32 MaxPoolSize = 5000};

    /**
     * ?�闭对象池系�?     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    void Shutdown(};

    /**
     * 预填?�对象�?
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    void PrepopulatePools(};

    // ==== ?��?池�?�?====

    /**
     * 从�X��?一个�?�?     * @return ?�用?��?位�?例�?如�?没�X��Xnullptr
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Units")
    AMingTacticalUnit* AcquireUnit(UClass* UnitClass = nullptr};

    /**
     * 将�?位�?还到池中
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Units")
    void ReturnUnit(AMingTacticalUnit* Unit};

    /**
     * ?��X��X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Units")
    TArray<AMingTacticalUnit*> AcquireUnits(int32 Count, UClass* UnitClass = nullptr};

    /**
     * ?��?归�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Units")
    void ReturnUnits(const TArray<AMingTacticalUnit*>& Units};

    // ==== AI池�?�?====

    /**
     * 从�X��?一个AI
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|AI")
    AMingCombatAI* AcquireAI(UClass* AIClass = nullptr};

    /**
     * 将AI归�X��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|AI")
    void ReturnAI(AMingCombatAI* AI};

    // ==== 池管X====

    /**
     * ?��X��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Management")
    void ExpandUnitPool(int32 AdditionalCount};

    /**
     * ?��?AI�?     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Management")
    void ExpandAIPool(int32 AdditionalCount};

    /**
     * ?�缩池�X�除?�使?��?对象�?     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Management")
    void ShrinkUnusedPools(};

    /**
     * 强制清�X�?�对象�X�用�?     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Management")
    void ForceClearAll(};

    /**
     * 设置?�大�?大�X�制
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Management")
    void SetMaxPoolSize(int32 NewMaxSize};

    /**
     * 设置?�动?��X��?     * 当可?�对象�?于此?��?比时?�动?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Management")
    void SetAutoExpandThreshold(float Percentage};

    /**
     * ?�用/禁用?�动?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool|Management")
    void SetAutoExpandEnabled(bool bEnabled};

    // ==== 统计信息 ====

    UFUNCTION(BlueprintPure, Category = "Object Pool|Stats")
    int32 GetTotalUnitPoolSize() const { return UnitPool.Num(}; }

    UFUNCTION(BlueprintPure, Category = "Object Pool|Stats")
    int32 GetAvailableUnitCount() const;

    UFUNCTION(BlueprintPure, Category = "Object Pool|Stats")
    int32 GetInUseUnitCount() const;

    UFUNCTION(BlueprintPure, Category = "Object Pool|Stats")
    int32 GetTotalAIPoolSize() const { return AIPool.Num(}; }

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

    // ==== 调�? ====

    UFUNCTION(BlueprintCallable, Category = "Object Pool|Debug")
    void PrintDebugInfo(};

    UFUNCTION(BlueprintCallable, Category = "Object Pool|Debug")
    FString GetDebugString() const;

    UFUNCTION(BlueprintCallable, Category = "Object Pool|Debug")
    bool ValidateIntegrity(};

    UFUNCTION(BlueprintCallable, Category = "Object Pool|Debug")
    void DumpPoolContents(};

    /**
     * Tick ?�新（�X��X��?检?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    void Tick(float DeltaTime};

private:
    // ?��?�?    UPROPERTY()
    TArray<FUnitPoolItem> UnitPool;

    // AI�?    UPROPERTY()
    TArray<FAIPoolItem> AIPool;

    // ?�置
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

    // ?�用?�件索�?快�?（避?��X��?尋�?
    TArray<int32> AvailableUnitIndices;
    TArray<int32> AvailableAIIndices;

    // ?��X�却（防止�?度扩展�?
    float LastExpandTime;
    float ExpandCooldown;

    // ?��?类�?默认�?    UPROPERTY()
    TSubclassOf<AMingTacticalUnit> DefaultUnitClass;

    UPROPERTY()
    TSubclassOf<AMingCombatAI> DefaultAIClass;

    // ?�建?��?�?    AMingTacticalUnit* CreateNewUnit(UClass* UnitClass};

    // ?�建?�AI
    AMingCombatAI* CreateNewAI(UClass* AIClass};

    // ?�置?��X��?    void ResetUnit(AMingTacticalUnit* Unit};

    // ?�置AI?��?    void ResetAI(AMingCombatAI* AI};

    // ?�正?�毁对象�?归�X��X��?
    void DestroyUnit(AMingTacticalUnit* Unit};
    void DestroyAI(AMingCombatAI* AI};

    // 检?�是?��?要自?�扩�?    void CheckAutoExpand(};

    // 清�X��?引用
    void CleanupInvalidReferences(};

    // ?�找?�用?��?�?    int32 FindAvailableUnitIndex() const;
    int32 FindAvailableAIIndex() const;

    // ?�找?��?对象?��?索�?
    int32 FindUnitIndex(AMingTacticalUnit* Unit) const;
    int32 FindAIIndex(AMingCombatAI* AI) const;
};

