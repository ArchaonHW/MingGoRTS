#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Difficulty/EDifficultyLevel.h"
#include "MingDifficultyBlueprintLibrary.generated.h"

class UMingDifficultyManager;
class UMingPlayerPerformanceTracker;

/**
 * ?�度系�X�图?�数�? * ?��X�图?�好?�难度管?�接X */
UCLASS()
class MINGTACTICAL_API UMingDifficultyBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // ==== ?�度等级?��? ====

    /** ?��?当�X�度等级 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static EDifficultyLevel GetCurrentDifficultyLevel(UObject* WorldContextObject};

    /** 设置?�度等级 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void SetDifficultyLevel(UObject* WorldContextObject, EDifficultyLevel NewLevel};

    /** ?��X�度?�示?�称 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty")
    static FString GetDifficultyDisplayName(EDifficultyLevel Level};

    /** ?��X�?�难度�?级选项 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty")
    static TArray<EDifficultyLevel> GetAllDifficultyLevels(};

    // ==== ?�度?�数?��? ====

    /** ?��X��X�数?��?*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Parameters", meta = (WorldContext = "WorldContextObject"))
    static float GetDifficultyParameter(UObject* WorldContextObject, EDifficultyParameter Parameter};

    /** 设置?��X�数?�值�?仅�X��?义难度�? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Parameters", meta = (WorldContext = "WorldContextObject"))
    static void SetDifficultyParameter(UObject* WorldContextObject, EDifficultyParameter Parameter, float Value};

    // ==== ?�态难度控X====

    /** ?�用/禁用?�态难度�X*/
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static void SetDynamicDifficultyEnabled(UObject* WorldContextObject, bool bEnabled};

    /** ?�否?�用了动?�难�?*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static bool IsDynamicDifficultyEnabled(UObject* WorldContextObject};

    /** ?��?/�X�度（防止自?��X��? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static void LockDifficulty(UObject* WorldContextObject, bool bLocked};

    /** ?�否被�?�?*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static bool IsDifficultyLocked(UObject* WorldContextObject};

    /** ?�动触�X�度评估 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static void ForceDifficultyEvaluation(UObject* WorldContextObject};

    // ==== 快捷?�度?��? ====

    /** ?��X�度 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void IncreaseDifficulty(UObject* WorldContextObject};

    /** ?��X�度 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void DecreaseDifficulty(UObject* WorldContextObject};

    /** ?�置为�?认设�?*/
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void ResetDifficulty(UObject* WorldContextObject};

    // ==== ?�度系数?��? ====

    /** ?��?AI?�度系数 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Multipliers", meta = (WorldContext = "WorldContextObject"))
    static float GetAIDifficultyMultiplier(UObject* WorldContextObject};

    /** ?��?资�X��?系数 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Multipliers", meta = (WorldContext = "WorldContextObject"))
    static float GetResourceMultiplier(UObject* WorldContextObject};

    /** ?��?任务?�度系数 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Multipliers", meta = (WorldContext = "WorldContextObject"))
    static float GetMissionDifficultyMultiplier(UObject* WorldContextObject};

    // ==== ?�家表现追踪 ====

    /** 记�X��X�失（�X��?系�?调用�?*/
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackUnitLoss(UObject* WorldContextObject, int32 UnitId, int32 UnitType, float UnitValue};

    /** 记�X��X�建 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackUnitCreation(UObject* WorldContextObject, int32 UnitId, int32 UnitType, float UnitCost};

    /** 记�?资�X��? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackResourceCollection(UObject* WorldContextObject, float ResourceType, float Amount, float ExpectedRate};

    /** 记�?任务完�? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackMissionComplete(UObject* WorldContextObject, const FString& MissionId, bool bSuccess, float CompletionTime};

    /** 记�X��?结�? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackCombatResult(UObject* WorldContextObject, int32 EnemiesKilled, int32 AlliesLost, float DamageDealt, float DamageTaken};

    /** ?��?当�X�?��X*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static float GetPlayerSkillIndex(UObject* WorldContextObject};

    // ==== ?�知 ====

    /** ?�送难度通知 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Notifications", meta = (WorldContext = "WorldContextObject"))
    static void SendDifficultyNotification(UObject* WorldContextObject, const FString& Message, float Duration = 5.0f};

    /** 设置?�否?�知?�家 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Notifications", meta = (WorldContext = "WorldContextObject"))
    static void SetDifficultyNotificationsEnabled(UObject* WorldContextObject, bool bEnabled};

    // ==== 调�? ====

    /** ?�印调�?信息?�日�?*/
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Debug", meta = (WorldContext = "WorldContextObject"))
    static void PrintDifficultyDebugInfo(UObject* WorldContextObject};

    /** ?��?调�?字符�?*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Debug", meta = (WorldContext = "WorldContextObject"))
    static FString GetDifficultyDebugString(UObject* WorldContextObject};

private:
    /** ?��X�度管�X��?�?*/
    static UMingDifficultyManager* GetDifficultyManager(UObject* WorldContextObject};
    static UMingPlayerPerformanceTracker* GetPerformanceTracker(UObject* WorldContextObject};
};

