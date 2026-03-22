#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHierarchicalTickSystem.generated.h"

class AMingTacticalUnit;
class AMingCombatAI;

/**
 * Tick 層級類型 | Tick Level Type
 */
UENUM(BlueprintType)
enum class ETickLevel : uint8
{
    Critical    UMETA(DisplayName = "Critical"),    // 每幀更新 | Every Frame (最高優先級 | Highest Priority)
    High        UMETA(DisplayName = "High"),        // 每幀更新 | Every Frame
    Normal      UMETA(DisplayName = "Normal"),      // 30 FPS
    Low         UMETA(DisplayName = "Low"),         // 15 FPS
    VeryLow     UMETA(DisplayName = "Very Low"),    // 5 FPS
    Background  UMETA(DisplayName = "Background"),   // 按需更新 | On Demand
    Paused      UMETA(DisplayName = "Paused")       // 暫停更新 | Paused
};

/**
 * Tick 組定義 | Tick Group Definition
 */
USTRUCT()
struct MINGTACTICAL_API FTickGroup
{
    GENERATED_BODY()

    UPROPERTY()
    ETickLevel TickLevel;

    UPROPERTY()
    TArray<TWeakObjectPtr<AMingTacticalUnit>> Units;

    UPROPERTY()
    TArray<TWeakObjectPtr<AMingCombatAI>> AIControllers;

    float TickInterval;
    float TimeSinceLastTick;
    int32 MaxUnitsPerFrame;
    int32 CurrentIndex;

    FTickGroup()
        : TickLevel(ETickLevel::Normal)
        , TickInterval(1.0f / 30.0f)
        , TimeSinceLastTick(0.0f)
        , MaxUnitsPerFrame(100)
        , CurrentIndex(0)
    {}

    void AddUnit(AMingTacticalUnit* Unit);
    void RemoveUnit(AMingTacticalUnit* Unit);
    void AddAI(AMingCombatAI* AI);
    void RemoveAI(AMingCombatAI* AI);
    bool ShouldTick(float DeltaTime);
    void ProcessTick(float DeltaTime);
    void CleanupInvalidReferences();
    int32 GetActiveCount() const;
};

/**
 * 分层 Tick 系统
 * 
 * 优化大规模单位的 Tick 性能：
 * - 根据单位状态分配不同 Tick 频率
 * - 支持每帧更新的单位数量有限
 * - 自动负载均衡
 * 
 * 适用于 1000+ 单位的大规模战斗场景
 */
UCLASS(ClassGroup = (Optimization), Blueprintable)
class MINGTACTICAL_API UMingHierarchicalTickSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingHierarchicalTickSystem();

    /**
     * 初始化系统
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void Initialize();

    /**
     * 关闭系统
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void Shutdown();

    /**
     * 主 Tick 函数
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void Tick(float DeltaTime);

    // ==== 单位管理 ====

    /**
     * 注册单位到系统
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void RegisterUnit(AMingTacticalUnit* Unit, ETickLevel InitialLevel = ETickLevel::Normal);

    /**
     * 注销单位
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void UnregisterUnit(AMingTacticalUnit* Unit);

    /**
     * 设置单位的 Tick 层级
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void SetUnitTickLevel(AMingTacticalUnit* Unit, ETickLevel NewLevel);

    /**
     * 获取单位的当前 Tick 层级
     */
    UFUNCTION(BlueprintPure, Category = "Hierarchical Tick")
    ETickLevel GetUnitTickLevel(AMingTacticalUnit* Unit) const;

    // ==== AI 管理 ====

    /**
     * 注册 AI 控制器
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void RegisterAI(AMingCombatAI* AI, ETickLevel InitialLevel = ETickLevel::Normal);

    /**
     * 注销 AI 控制器
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void UnregisterAI(AMingCombatAI* AI);

    /**
     * 设置 AI 的 Tick 层级
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick")
    void SetAITickLevel(AMingCombatAI* AI, ETickLevel NewLevel);

    // ==== 优先级调整 ====

    /**
     * 将单位提升到 Critical 层级
     * (例如：被玩家选中的单位)
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void PromoteToCritical(AMingTacticalUnit* Unit, float DurationSeconds = 5.0f);

    /**
     * 将单位降级到背景层级
     * (例如：屏幕外的单位)
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void DemoteToBackground(AMingTacticalUnit* Unit);

    /**
     * 根据距离相机的距离自动调整层级
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void AutoAdjustLevelByDistance(AMingTacticalUnit* Unit, float DistanceToCamera);

    /**
     * 根据战斗状态调整层级
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Priority")
    void AdjustLevelByCombatState(AMingTacticalUnit* Unit, bool bInCombat);

    // ==== 配置 ====

    /**
     * 设置指定层级的 Tick 间隔
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Config")
    void SetTickInterval(ETickLevel Level, float IntervalSeconds);

    /**
     * 设置指定层级的每帧最大处理单位数
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Config")
    void SetMaxUnitsPerFrame(ETickLevel Level, int32 MaxUnits);

    /**
     * 启用/禁用自动负载均衡
     */
    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Config")
    void SetAutoBalancingEnabled(bool bEnabled);

    // ==== 统计信息 ====

    UFUNCTION(BlueprintPure, Category = "Hierarchical Tick|Stats")
    int32 GetTotalRegisteredUnits() const;

    UFUNCTION(BlueprintPure, Category = "Hierarchical Tick|Stats")
    int32 GetUnitCountByLevel(ETickLevel Level) const;

    UFUNCTION(BlueprintPure, Category = "Hierarchical Tick|Stats")
    float GetAverageTickTimeMs() const { return AverageTickTimeMs; }

    UFUNCTION(BlueprintPure, Category = "Hierarchical Tick|Stats")
    int32 GetTickedUnitsLastFrame() const { return TickedUnitsLastFrame; }

    UFUNCTION(BlueprintPure, Category = "Hierarchical Tick|Stats")
    float GetCurrentFPS() const;

    // ==== 调试 ====

    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Debug")
    void PrintDebugInfo();

    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Debug")
    void DrawDebugVisualization();

    UFUNCTION(BlueprintCallable, Category = "Hierarchical Tick|Debug")
    FString GetDebugString() const;

private:
    // Tick 组映射
    UPROPERTY()
    TMap<ETickLevel, FTickGroup> TickGroups;

    // 单位到层级的映射
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingTacticalUnit>, ETickLevel> UnitLevels;

    // AI 到层级的映射
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingCombatAI>, ETickLevel> AILevels;

    // 临时升级的单位和恢复时间
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingTacticalUnit>, float> PromotedUnits;

    // 配置
    bool bIsInitialized;
    bool bAutoBalancingEnabled;

    // 统计
    float AverageTickTimeMs;
    int32 TickedUnitsLastFrame;
    float CurrentTickTimeMs;
    TArray<float> TickTimeHistory;
    int32 TickHistoryIndex;

    // 初始化 Tick 组
    void InitializeTickGroups();

    // 处理临时升级
    void ProcessPromotedUnits(float DeltaTime);

    // 自动负载均衡
    void PerformLoadBalancing();

    // 计算推荐的层级
    ETickLevel CalculateRecommendedLevel(AMingTacticalUnit* Unit) const;

    // 更新统计
    void UpdateTickStats(float TickTimeMs);

    // 清理无效引用
    void CleanupInvalidReferences();

    // 获取 Tick 组的引用
    FTickGroup* GetTickGroup(ETickLevel Level);
    const FTickGroup* GetTickGroup(ETickLevel Level) const;

    // 获取层级的默认间隔
    static float GetDefaultInterval(ETickLevel Level);

    // 获取层级的默认每帧最大单位数
    static int32 GetDefaultMaxUnits(ETickLevel Level);
};
