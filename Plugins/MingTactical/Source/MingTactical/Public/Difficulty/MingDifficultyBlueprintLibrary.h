#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Difficulty/EDifficultyLevel.h"
#include "MingDifficultyBlueprintLibrary.generated.h"

class UMingDifficultyManager;
class UMingPlayerPerformanceTracker;

/**
 * ??רt?X摧毀??? * 目標數量?n???׺??X */
UCLASS()
class MINGTACTICAL_API UMingDifficultyBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // ==== ??׵摧毀? ====

    /** 摧毀??X?׵? */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static EDifficultyLevel GetCurrentDifficultyLevel(UObject* WorldContextObject);

    /** ?m??׵? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void SetDifficultyLevel(UObject* WorldContextObject, EDifficultyLevel NewLevel);

    /** 目標數量??? */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty")
    static FString GetDifficultyDisplayName(EDifficultyLevel Level);

    /** 目標數量摧毀?? */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty")
    static TArray<EDifficultyLevel> GetAllDifficultyLevels();

    // ==== 摧毀摧毀? ====

    /** 故事重要性摧毀??*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Parameters", meta = (WorldContext = "WorldContextObject"))
    static float GetDifficultyParameter(UObject* WorldContextObject, EDifficultyParameter Parameter);

    /** ?m目標數量??目標數量摧毀 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Parameters", meta = (WorldContext = "WorldContextObject"))
    static void SetDifficultyParameter(UObject* WorldContextObject, EDifficultyParameter Parameter, float Value);

    // ==== 摧毀ױX====

    /** ??/T摧毀?X*/
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static void SetDynamicDifficultyEnabled(UObject* WorldContextObject, bool bEnabled);

    /** ??_??ΤF摧毀??*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static bool IsDynamicDifficultyEnabled(UObject* WorldContextObject);

    /** 摧毀/?X?ס]目標數量 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static void LockDifficulty(UObject* WorldContextObject, bool bLocked);

    /** ??_Q摧毀*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static bool IsDifficultyLocked(UObject* WorldContextObject);

    /** ???D?X?? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static void ForceDifficultyEvaluation(UObject* WorldContextObject);

    // ==== ֱ摧毀?? ====

    /** ???X? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void IncreaseDifficulty(UObject* WorldContextObject);

    /** ???X? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void DecreaseDifficulty(UObject* WorldContextObject);

    /** ??m摧毀???*/
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void ResetDifficulty(UObject* WorldContextObject);

    // ==== ??רt摧毀? ====

    /** 摧毀AI??רt? */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Multipliers", meta = (WorldContext = "WorldContextObject"))
    static float GetAIDifficultyMultiplier(UObject* WorldContextObject);

    /** ???目標數量t? */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Multipliers", meta = (WorldContext = "WorldContextObject"))
    static float GetResourceMultiplier(UObject* WorldContextObject);

    /** 摧毀???רt? */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Multipliers", meta = (WorldContext = "WorldContextObject"))
    static float GetMissionDifficultyMultiplier(UObject* WorldContextObject);

    // ==== ??a?l? ====

    /** ??X??X?]?X???t摧毀?*/
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackUnitLoss(UObject* WorldContextObject, int32 UnitId, int32 UnitType, float UnitValue);

    /** ??X??X? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackUnitCreation(UObject* WorldContextObject, int32 UnitId, int32 UnitType, float UnitCost);

    /** ??目標數量 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackResourceCollection(UObject* WorldContextObject, float ResourceType, float Amount, float ExpectedRate);

    /** 摧毀?? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackMissionComplete(UObject* WorldContextObject, const FString& MissionId, bool bSuccess, float CompletionTime);

    /** ??X摧毀?? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackCombatResult(UObject* WorldContextObject, int32 EnemiesKilled, int32 AlliesLost, float DamageDealt, float DamageTaken);

    /** ???目標數量?X*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static float GetPlayerSkillIndex(UObject* WorldContextObject);

    // ==== ?? ====

    /** ??e?׳q */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Notifications", meta = (WorldContext = "WorldContextObject"))
    static void SendDifficultyNotification(UObject* WorldContextObject, const FString& Message, float Duration = 5.0f);

    /** ?m??_摧毀a */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Notifications", meta = (WorldContext = "WorldContextObject"))
    static void SetDifficultyNotificationsEnabled(UObject* WorldContextObject, bool bEnabled);

    // ==== ??? ====

    /** ??L???H摧毀*/
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Debug", meta = (WorldContext = "WorldContextObject"))
    static void PrintDifficultyDebugInfo(UObject* WorldContextObject);

    /** 摧毀???r??*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Debug", meta = (WorldContext = "WorldContextObject"))
    static FString GetDifficultyDebugString(UObject* WorldContextObject);

private:
    /** ???X?׺?X摧毀?*/
    static UMingDifficultyManager* GetDifficultyManager(UObject* WorldContextObject);
    static UMingPlayerPerformanceTracker* GetPerformanceTracker(UObject* WorldContextObject);
);

