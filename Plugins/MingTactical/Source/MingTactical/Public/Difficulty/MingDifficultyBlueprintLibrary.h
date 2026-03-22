#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Difficulty/EDifficultyLevel.h"
#include "MingDifficultyBlueprintLibrary.generated.h"

class UMingDifficultyManager;
class UMingPlayerPerformanceTracker;

/**
 * ?¾åº¦ç³»ç??å›¾?½æ•°åº? * ?ä??å›¾?‹å¥½?„éš¾åº¦ç®¡?†æ¥?? */
UCLASS()
class MINGTACTICAL_API UMingDifficultyBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // ==== ?¾åº¦ç­‰çº§?ä? ====

    /** ?·å?å½“å??¾åº¦ç­‰çº§ */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static EDifficultyLevel GetCurrentDifficultyLevel(UObject* WorldContextObject};

    /** è®¾ç½®?¾åº¦ç­‰çº§ */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void SetDifficultyLevel(UObject* WorldContextObject, EDifficultyLevel NewLevel};

    /** ?·å??¾åº¦?¾ç¤º?ç§° */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty")
    static FString GetDifficultyDisplayName(EDifficultyLevel Level};

    /** ?·å??€?‰éš¾åº¦ç?çº§é€‰é¡¹ */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty")
    static TArray<EDifficultyLevel> GetAllDifficultyLevels(};

    // ==== ?¾åº¦?‚æ•°?ä? ====

    /** ?·å??‡å??‚æ•°?„å€?*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Parameters", meta = (WorldContext = "WorldContextObject"))
    static float GetDifficultyParameter(UObject* WorldContextObject, EDifficultyParameter Parameter};

    /** è®¾ç½®?‡å??‚æ•°?„å€¼ï?ä»…é??ªå?ä¹‰éš¾åº¦ï? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Parameters", meta = (WorldContext = "WorldContextObject"))
    static void SetDifficultyParameter(UObject* WorldContextObject, EDifficultyParameter Parameter, float Value};

    // ==== ?¨æ€éš¾åº¦æ§??====

    /** ?¯ç”¨/ç¦ç”¨?¨æ€éš¾åº¦è???*/
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static void SetDynamicDifficultyEnabled(UObject* WorldContextObject, bool bEnabled};

    /** ?¯å¦?¯ç”¨äº†åŠ¨?éš¾åº?*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static bool IsDynamicDifficultyEnabled(UObject* WorldContextObject};

    /** ?å?/è§???¾åº¦ï¼ˆé˜²æ­¢è‡ª?¨è??´ï? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static void LockDifficulty(UObject* WorldContextObject, bool bLocked};

    /** ?¯å¦è¢«é?å®?*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static bool IsDifficultyLocked(UObject* WorldContextObject};

    /** ?‹åŠ¨è§¦å??¾åº¦è¯„ä¼° */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Dynamic", meta = (WorldContext = "WorldContextObject"))
    static void ForceDifficultyEvaluation(UObject* WorldContextObject};

    // ==== å¿«æ·?¾åº¦?ä? ====

    /** ?é??¾åº¦ */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void IncreaseDifficulty(UObject* WorldContextObject};

    /** ?ä??¾åº¦ */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void DecreaseDifficulty(UObject* WorldContextObject};

    /** ?ç½®ä¸ºé?è®¤è®¾ç½?*/
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty", meta = (WorldContext = "WorldContextObject"))
    static void ResetDifficulty(UObject* WorldContextObject};

    // ==== ?¾åº¦ç³»æ•°?·å? ====

    /** ?·å?AI?¾åº¦ç³»æ•° */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Multipliers", meta = (WorldContext = "WorldContextObject"))
    static float GetAIDifficultyMultiplier(UObject* WorldContextObject};

    /** ?·å?èµ„æ??¶ç?ç³»æ•° */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Multipliers", meta = (WorldContext = "WorldContextObject"))
    static float GetResourceMultiplier(UObject* WorldContextObject};

    /** ?·å?ä»»åŠ¡?¾åº¦ç³»æ•° */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Multipliers", meta = (WorldContext = "WorldContextObject"))
    static float GetMissionDifficultyMultiplier(UObject* WorldContextObject};

    // ==== ?©å®¶è¡¨ç°è¿½è¸ª ====

    /** è®°å??•ä??Ÿå¤±ï¼ˆä??˜æ?ç³»ç?è°ƒç”¨ï¼?*/
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackUnitLoss(UObject* WorldContextObject, int32 UnitId, int32 UnitType, float UnitValue};

    /** è®°å??•ä??›å»º */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackUnitCreation(UObject* WorldContextObject, int32 UnitId, int32 UnitType, float UnitCost};

    /** è®°å?èµ„æ??¶é? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackResourceCollection(UObject* WorldContextObject, float ResourceType, float Amount, float ExpectedRate};

    /** è®°å?ä»»åŠ¡å®Œæ? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackMissionComplete(UObject* WorldContextObject, const FString& MissionId, bool bSuccess, float CompletionTime};

    /** è®°å??˜æ?ç»“æ? */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static void TrackCombatResult(UObject* WorldContextObject, int32 EnemiesKilled, int32 AlliesLost, float DamageDealt, float DamageTaken};

    /** ?·å?å½“å??€?½æ???*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Tracking", meta = (WorldContext = "WorldContextObject"))
    static float GetPlayerSkillIndex(UObject* WorldContextObject};

    // ==== ?šçŸ¥ ====

    /** ?‘é€éš¾åº¦é€šçŸ¥ */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Notifications", meta = (WorldContext = "WorldContextObject"))
    static void SendDifficultyNotification(UObject* WorldContextObject, const FString& Message, float Duration = 5.0f};

    /** è®¾ç½®?¯å¦?šçŸ¥?©å®¶ */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Notifications", meta = (WorldContext = "WorldContextObject"))
    static void SetDifficultyNotificationsEnabled(UObject* WorldContextObject, bool bEnabled};

    // ==== è°ƒè? ====

    /** ?“å°è°ƒè?ä¿¡æ¯?°æ—¥å¿?*/
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Difficulty|Debug", meta = (WorldContext = "WorldContextObject"))
    static void PrintDifficultyDebugInfo(UObject* WorldContextObject};

    /** ?·å?è°ƒè?å­—ç¬¦ä¸?*/
    UFUNCTION(BlueprintPure, Category = "MingGoRTS|Difficulty|Debug", meta = (WorldContext = "WorldContextObject"))
    static FString GetDifficultyDebugString(UObject* WorldContextObject};

private:
    /** ?·å??¾åº¦ç®¡ç??¨å?ä¾?*/
    static UMingDifficultyManager* GetDifficultyManager(UObject* WorldContextObject};
    static UMingPlayerPerformanceTracker* GetPerformanceTracker(UObject* WorldContextObject};
};

