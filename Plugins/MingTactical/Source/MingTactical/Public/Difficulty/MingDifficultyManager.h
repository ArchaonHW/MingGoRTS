#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Difficulty/FMingDifficultySettings.h"
#include "Difficulty/EDifficultyLevel.h"
#include "MingDifficultyManager.generated.h"

// 前向声明
class UMingPlayerPerformanceTracker;

/**
 * 难度变化事件
 */
USTRUCT(BlueprintType)
struct FDifficultyChangedEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Event")
    EDifficultyLevel OldLevel;

    UPROPERTY(BlueprintReadOnly, Category = "Event")
    EDifficultyLevel NewLevel;

    UPROPERTY(BlueprintReadOnly, Category = "Event")
    float OldSkillIndex;

    UPROPERTY(BlueprintReadOnly, Category = "Event")
    float NewSkillIndex;

    UPROPERTY(BlueprintReadOnly, Category = "Event")
    FString Reason;

    FDifficultyChangedEvent()
        : OldLevel(EDifficultyLevel::Normal)
        , NewLevel(EDifficultyLevel::Normal)
        , OldSkillIndex(50.0f)
        , NewSkillIndex(50.0f)
    {}
};

/**
 * 难度管理器
 * 管理游戏难度配置和动态调整
 * 
 * 功能：
 * - 难度等级管理 (Easy/Normal/Hard/Expert/Custom)
 * - 动态难度调整
 * - 难度事件触发
 * - 平滑过渡算法
 */
UCLASS()
class MINGTACTICAL_API UMingDifficultyManager : public UObject
{
    GENERATED_BODY()

public:
    UMingDifficultyManager();

    virtual void Initialize();
    virtual void Shutdown();
    void Tick(float DeltaTime);

    // ==== 难度等级管理 ====

    /** 设置难度等级 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    void SetDifficultyLevel(EDifficultyLevel NewLevel);

    /** 获取当前难度等级 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    EDifficultyLevel GetCurrentDifficultyLevel() const;

    /** 获取当前难度显示名称 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    FString GetCurrentDifficultyDisplayName() const;

    /** 获取所有可用的难度等级 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    static TArray<EDifficultyLevel> GetAvailableDifficultyLevels();

    /** 获取预设难度配置 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty")
    FMingPresetDifficultyConfig GetPresetConfig(EDifficultyLevel Level) const;

    // ==== 动态难度调整 ====

    /** 启用/禁用动态难度调整 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    void SetDynamicAdjustmentEnabled(bool bEnabled);

    /** 是否启用动态调整 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    bool IsDynamicAdjustmentEnabled() const;

    /** 手动触发难度评估和调整 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    void ForceDifficultyEvaluation();

    /** 锁定难度（防止自动调整） */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    void LockDifficulty(bool bLocked);

    /** 难度是否被锁定 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
    bool IsDifficultyLocked() const;

    // ==== 参数调整接口 ====

    /** 获取参数当前值 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    float GetParameterValue(EDifficultyParameter Parameter) const;

    /** 设置参数值（适用于自定义难度） */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    void SetParameterValue(EDifficultyParameter Parameter, float Value);

    /** 调整参数值（带增量） */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    bool AdjustParameter(EDifficultyParameter Parameter, float Delta);

    /** 获取所有参数设置 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    FMingDifficultySettings GetDifficultySettings() const;

    /** 应用完整设置 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Parameters")
    void ApplyDifficultySettings(const FMingDifficultySettings& Settings);

    // ==== 特定功能调整 ====

    /** 调整AI难度 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|AI")
    void AdjustAIDifficulty(float NewReactionSpeed, float NewIntelligence);

    /** 获取AI难度系数 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|AI")
    float GetAIDifficultyMultiplier() const;

    /** 调整资源生成率 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Resources")
    void AdjustResourceGeneration(float NewCollectionRate, float NewConsumptionRate);

    /** 获取资源收益系数 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Resources")
    float GetResourceMultiplier() const;

    /** 调整任务目标难度 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Missions")
    void AdjustMissionDifficulty(float NewTimeLimit, float NewObjectiveScale);

    /** 获取任务难度系数 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Missions")
    float GetMissionDifficultyMultiplier() const;

    // ==== 平滑过渡 ====

    /** 是否正在过渡中 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Transition")
    bool IsTransitioning() const;

    /** 获取当前过渡进度 0-1 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Transition")
    float GetTransitionProgress() const;

    /** 中断正在进行的过渡 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Transition")
    void AbortTransition();

    // ==== 通知系统 ====

    /** 设置是否通知玩家难度变化 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Notifications")
    void SetPlayerNotificationEnabled(bool bEnabled);

    /** 发送自定义难度通知 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Notifications")
    void SendDifficultyNotification(const FString& Message, float Duration = 5.0f);

    // ==== 持久化 ====

    /** 保存当前难度配置 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Save")
    void SaveDifficultySettings();

    /** 加载难度配置 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Save")
    void LoadDifficultySettings();

    /** 重置为默认配置 */
    UFUNCTION(BlueprintCallable, Category = "Difficulty|Save")
    void ResetToDefault();

    // ==== 调试 ====

    UFUNCTION(BlueprintCallable, Category = "Difficulty|Debug")
    void PrintDebugInfo();

    UFUNCTION(BlueprintCallable, Category = "Difficulty|Debug")
    FString GetDebugString() const;

    // ==== 事件委托 ====

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDifficultyChanged, const FDifficultyChangedEvent&, Event);

    UPROPERTY(BlueprintAssignable, Category = "Difficulty|Events")
    FOnDifficultyChanged OnDifficultyChanged;

protected:
    /** 执行难度评估 */
    void PerformDifficultyEvaluation();

    /** 根据玩家表现计算建议的难度变化 */
    EDifficultyChangeDirection CalculateRecommendedChange() const;

    /** 执行难度调整 */
    void ExecuteDifficultyAdjustment(EDifficultyChangeDirection Direction);

    /** 平滑过渡更新 */
    void UpdateTransition(float DeltaTime);

    /** 完成过渡 */
    void CompleteTransition();

    /** 触发难度变化事件 */
    void BroadcastDifficultyChanged(const FString& Reason);

    /** 防止振荡：检查最近的调整历史 */
    bool ShouldPreventOscillation() const;

    /** 订阅核心事件 */
    void SetupEventSubscriptions();
    void CleanupEventSubscriptions();

private:
    bool bIsInitialized;
    bool bIsLocked;
    bool bIsTransitioning;

    FMingDifficultySettings CurrentSettings;
    FMingDifficultySettings TargetSettings;
    FMingDifficultySettings TransitionStartSettings;

    float TransitionProgress;
    float TimeSinceLastEvaluation;

    TArray<FDifficultyChangedEvent> AdjustmentHistory;
    int32 MaxHistorySize;

    TWeakObjectPtr<UMingPlayerPerformanceTracker> PerformanceTracker;

    // 默认配置缓存
    static TMap<EDifficultyLevel, FMingPresetDifficultyConfig> CachedPresetConfigs;
};
