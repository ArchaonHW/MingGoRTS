#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Difficulty/EDifficultyLevel.h"
#include "MingDifficultyBlueprintLibrary.generated.h"

class UMingDifficultyManager;
class UMingPlayerPerformanceTracker;

/**
 * 难度系统蓝图函数库
 * 提供蓝图友好的难度管理接口
 */
UCLASS()
class MINGTACTICAL_API UMingDifficultyBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // ==== 难度等级操作 ====

    /** 获取当前难度等级 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static EDifficultyLevel GetCurrentDifficultyLevel(UObject* WorldContextObject);

    /** 设置难度等级 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void SetDifficultyLevel(UObject* WorldContextObject, EDifficultyLevel NewLevel);

    /** 获取难度显示名称 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty")
    static FString GetDifficultyDisplayName(EDifficultyLevel Level);

    /** 获取所有难度等级选项 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty")
    static TArray<EDifficultyLevel> GetAllDifficultyLevels();

    // ==== 难度参数操作 ====

    /** 获取指定参数的值 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Parameters", meta = (WorldContext = "WorldContextObject"))
    static float GetDifficultyParameter(UObject* WorldContextObject, EDifficultyParameter Parameter);

    /** 设置指定参数的值（仅限自定义难度） */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Parameters", meta = (WorldContext = "WorldContextObject"))
    static void SetDifficultyParameter(UObject* WorldContextObject, EDifficultyParameter Parameter, float Value);

    // ==== 动态难度控制 ====

    /** 启用/禁用动态难度调整 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static void SetDynamicDifficultyEnabled(UObject* WorldContextObject, bool bEnabled);

    /** 是否启用了动态难度 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static bool IsDynamicDifficultyEnabled(UObject* WorldContextObject);

    /** 锁定/解锁难度（防止自动调整） */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static void LockDifficulty(UObject* WorldContextObject, bool bLocked);

    /** 是否被锁定 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static bool IsDifficultyLocked(UObject* WorldContextObject);

    /** 手动触发难度评估 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static void ForceDifficultyEvaluation(UObject* WorldContextObject);

    // ==== 快捷难度操作 ====

    /** 提高难度 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void IncreaseDifficulty(UObject* WorldContextObject);

    /** 降低难度 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void DecreaseDifficulty(UObject* WorldContextObject);

    /** 重置为默认设置 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void ResetDifficulty(UObject* WorldContextObject);

    // ==== 难度系数获取 ====

    /** 获取AI难度系数 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Multipliers", meta = (WorldContext = "WorldContextObject"))
    static float GetAIDifficultyMultiplier(UObject* WorldContextObject);

    /** 获取资源收益系数 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Multipliers", meta = (WorldContext = "WorldContextObject"))
    static float GetResourceMultiplier(UObject* WorldContextObject);

    /** 获取任务难度系数 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Multipliers", meta = (WorldContext = "WorldContextObject"))
    static float GetMissionDifficultyMultiplier(UObject* WorldContextObject);

    // ==== 玩家表现追踪 ====

    /** 记录单位损失（供战斗系统调用） */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackUnitLoss(UObject* WorldContextObject, int32 UnitId, int32 UnitType, float UnitValue);

    /** 记录单位创建 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackUnitCreation(UObject* WorldContextObject, int32 UnitId, int32 UnitType, float UnitCost);

    /** 记录资源收集 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackResourceCollection(UObject* WorldContextObject, float ResourceType, float Amount, float ExpectedRate);

    /** 记录任务完成 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackMissionComplete(UObject* WorldContextObject, const FString& MissionId, bool bSuccess, float CompletionTime);

    /** 记录战斗结果 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackCombatResult(UObject* WorldContextObject, int32 EnemiesKilled, int32 AlliesLost, float DamageDealt, float DamageTaken);

    /** 获取当前技能指数 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static float GetPlayerSkillIndex(UObject* WorldContextObject);

    // ==== 通知 ====

    /** 发送难度通知 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Notifications", meta = (WorldContext = "WorldContextObject"))
    static void SendDifficultyNotification(UObject* WorldContextObject, const FString& Message, float Duration = 5.0f);

    /** 设置是否通知玩家 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Notifications", meta = (WorldContext = "WorldContextObject"))
    static void SetDifficultyNotificationsEnabled(UObject* WorldContextObject, bool bEnabled);

    // ==== 调试 ====

    /** 打印调试信息到日志 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Debug", meta = (WorldContext = "WorldContextObject"))
    static void PrintDifficultyDebugInfo(UObject* WorldContextObject);

    /** 获取调试字符串 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Debug", meta = (WorldContext = "WorldContextObject"))
    static FString GetDifficultyDebugString(UObject* WorldContextObject);

private:
    /** 获取难度管理器实例 */
    static UMingDifficultyManager* GetDifficultyManager(UObject* WorldContextObject);
    static UMingPlayerPerformanceTracker* GetPerformanceTracker(UObject* WorldContextObject);
};
