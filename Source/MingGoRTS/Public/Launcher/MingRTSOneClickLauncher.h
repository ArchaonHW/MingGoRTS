#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimization/MingRTSOptimizationSystemManager.h"
#include "MingRTSOneClickLauncher.generated.h"

/**
 * MingGoRTS ä¸€?µå??•ç³»çµ? * ?€ç°¡å–®?„æ–¹å¼å??•ä¸¦?‹è??ªå?ç·¨è­¯ç³»çµ±
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingGoRTS")
class MINGGORTS_API UMingRTSOneClickLauncher : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOneClickLauncher();

    // ?? ä¸€?µå???- ?€ç°¡å–®?„ä½¿?¨æ–¹å¼?    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "?? ä¸€?µå??•å„ª?–ç³»çµ?))
    static UMingRTSOneClickLauncher* LaunchOptimizationSystem();

    // ??å¿«é€Ÿå???- ?ªå??ç½®ä¸¦é?å§‹å„ª??    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "??å¿«é€Ÿå??•ä¸¦?ªå?"))
    void QuickLaunchAndOptimize();

    // ?¯ ?ºèƒ½?Ÿå? - ?¹æ?ç³»çµ±?ªå??¸æ??€ä½³é?ç½?    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "?¯ ?ºèƒ½?Ÿå?"))
    void SmartLaunch();

    // ?”¥ æ¥µè‡´?Ÿå? - ?€é«˜æ€§èƒ½?ç½®
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "?”¥ æ¥µè‡´?Ÿå?"))
    void ExtremeLaunch();

    // ?? ?²å??Ÿå??€??    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "?? ?²å?ç³»çµ±?€??))
    FString GetLaunchStatus();

    // ?® ?²å??§èƒ½?±å?
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "?® ?²å??§èƒ½?±å?"))
    FString GetPerformanceReport();

    // ?? ?æ–°?Ÿå?
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "?? ?æ–°?Ÿå?ç³»çµ±"))
    void RestartSystem();

    // ?¹ï? ?œæ­¢ç³»çµ±
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "?¹ï? ?œæ­¢?ªå?ç³»çµ±"))
    void StopSystem();

    // ?? ?²å?ç³»çµ±?¥åº·åº?    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "?? ?²å?ç³»çµ±?¥åº·åº?))
    float GetSystemHealth();

    // ?¯ ä¸€?µæ€§èƒ½æª¢æŸ¥
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "?¯ ä¸€?µæ€§èƒ½æª¢æŸ¥"))
    void OneClickPerformanceCheck();

protected:
    // ?ªå?ç³»çµ±ç®¡ç???    UPROPERTY(BlueprintReadOnly, Category = "One Click Launcher")
    TObjectPtr<UMingRTSOptimizationSystemManager> OptimizationManager;

    // ç³»çµ±?€??    UPROPERTY(BlueprintReadOnly, Category = "One Click Launcher")
    bool bSystemLaunched;

    UPROPERTY(BlueprintReadOnly, Category = "One Click Launcher")
    bool bOptimizationActive;

    UPROPERTY(BlueprintReadOnly, Category = "One Click Launcher")
    FDateTime LaunchTime;

private:
    // ?§éƒ¨?Ÿå??è¼¯
    void InitializeLauncher();
    void ConfigureForMode(EOptimizationMode Mode);
    void StartOptimization();
    
    // ?€?‹æª¢??    bool IsSystemHealthy();
    void UpdateLaunchStatus();
    
    // é¡¯ç¤º?Ÿèƒ½
    void ShowLaunchSuccess();
    void ShowPerformanceResults();
    void DisplaySystemInfo();
    
    // è¼”åŠ©?¹æ?
    FString GetLaunchModeName(EOptimizationMode Mode);
    FString GetSystemHealthStatus(float Health);
    FString GenerateLaunchReport();
};
