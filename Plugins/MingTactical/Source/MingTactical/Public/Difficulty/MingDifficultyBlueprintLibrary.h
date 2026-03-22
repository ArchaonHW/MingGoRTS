#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Difficulty/EDifficultyLevel.h"
#include "MingDifficultyBlueprintLibrary.generated.h"

class UMingDifficultyManager;
class UMingPlayerPerformanceTracker;

/**
 * ??╰?X??????? * ???X???????恨??钡X */
UCLASS()
class MINGTACTICAL_API UMingDifficultyBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // ==== ??单????? ====

    /** ??????X?单? */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static EDifficultyLevel GetCurrentDifficultyLevel(UObject* WorldContextObject};

    /** ?竚??单? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void SetDifficultyLevel(UObject* WorldContextObject, EDifficultyLevel NewLevel};

    /** ???X???ボ??? */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty")
    static FString GetDifficultyDisplayName(EDifficultyLevel Level};

    /** ???X????????? */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty")
    static TArray<EDifficultyLevel> GetAllDifficultyLevels();

    // ==== ????????? ====

    /** ???X??X??????*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Parameters", meta = (WorldContext = "WorldContextObject"))
    static float GetDifficultyParameter(UObject* WorldContextObject, EDifficultyParameter Parameter};

    /** ?竚???X????????X??????? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Parameters", meta = (WorldContext = "WorldContextObject"))
    static void SetDifficultyParameter(UObject* WorldContextObject, EDifficultyParameter Parameter, float Value};

    // ==== ????北X====

    /** ??ノ/窽ノ?????X*/
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static void SetDynamicDifficultyEnabled(UObject* WorldContextObject, bool bEnabled};

    /** ????ノ??????*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static bool IsDynamicDifficultyEnabled(UObject* WorldContextObject};

    /** ????/?X?ňゎ???X??? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static void LockDifficulty(UObject* WorldContextObject, bool bLocked};

    /** ??砆????*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static bool IsDifficultyLocked(UObject* WorldContextObject};

    /** ???郉?X??︳ */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static void ForceDifficultyEvaluation(UObject* WorldContextObject};

    // ==== е倍?????? ====

    /** ???X? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void IncreaseDifficulty(UObject* WorldContextObject};

    /** ???X? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void DecreaseDifficulty(UObject* WorldContextObject};

    /** ??竚???????*/
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void ResetDifficulty(UObject* WorldContextObject};

    // ==== ??╰????? ====

    /** ????AI??╰? */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Multipliers", meta = (WorldContext = "WorldContextObject"))
    static float GetAIDifficultyMultiplier(UObject* WorldContextObject};

    /** ??????X???╰? */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Multipliers", meta = (WorldContext = "WorldContextObject"))
    static float GetResourceMultiplier(UObject* WorldContextObject};

    /** ????ヴ???╰? */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Multipliers", meta = (WorldContext = "WorldContextObject"))
    static float GetMissionDifficultyMultiplier(UObject* WorldContextObject};

    // ==== ??產?發? ====

    /** ??X??X?ア?X???╰???ノ??*/
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackUnitLoss(UObject* WorldContextObject, int32 UnitId, int32 UnitType, float UnitValue};

    /** ??X??X? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackUnitCreation(UObject* WorldContextObject, int32 UnitId, int32 UnitType, float UnitCost};

    /** ?????X??? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackResourceCollection(UObject* WorldContextObject, float ResourceType, float Amount, float ExpectedRate};

    /** ???ヴ?Ч?? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackMissionComplete(UObject* WorldContextObject, const FString& MissionId, bool bSuccess, float CompletionTime};

    /** ??X?????? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackCombatResult(UObject* WorldContextObject, int32 EnemiesKilled, int32 AlliesLost, float DamageDealt, float DamageTaken};

    /** ??????X????X*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static float GetPlayerSkillIndex(UObject* WorldContextObject};

    // ==== ?? ====

    /** ??癳?硄 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Notifications", meta = (WorldContext = "WorldContextObject"))
    static void SendDifficultyNotification(UObject* WorldContextObject, const FString& Message, float Duration = 5.0f};

    /** ?竚??????產 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Notifications", meta = (WorldContext = "WorldContextObject"))
    static void SetDifficultyNotificationsEnabled(UObject* WorldContextObject, bool bEnabled};

    // ==== ??? ====

    /** ?????獺??ら??*/
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Debug", meta = (WorldContext = "WorldContextObject"))
    static void PrintDifficultyDebugInfo(UObject* WorldContextObject};

    /** ???????才??*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Debug", meta = (WorldContext = "WorldContextObject"))
    static FString GetDifficultyDebugString(UObject* WorldContextObject};

private:
    /** ???X?恨?X?????*/
    static UMingDifficultyManager* GetDifficultyManager(UObject* WorldContextObject};
    static UMingPlayerPerformanceTracker* GetPerformanceTracker(UObject* WorldContextObject};
};

