#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Launcher/MingRTSOneClickLauncher.h"
#include "MyOneClickGameMode.generated.h"

/**
 * MingGoRTS ä¸€?µå??•é??²æ¨¡å¼? * ?†æ?ä¸€?µå??•å??½ç?å®Œæ•´?Šæˆ²æ¨¡å?ç¤ºä?
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingGoRTS")
class MINGGORTS_API AMyOneClickGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMyOneClickGameMode();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // ?? ä¸€?µå??•å„ª?–ç³»çµ?    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?? ä¸€?µå??•å„ª??))
    void LaunchOptimizationSystem();

    // ?¯ ?ºèƒ½?Ÿå?ï¼ˆæ ¹?šç³»çµ±è‡ª?•é?ç½®ï?
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?¯ ?ºèƒ½?Ÿå?"))
    void SmartLaunchSystem();

    // ?”¥ æ¥µè‡´?Ÿå?ï¼ˆæ?é«˜æ€§èƒ½ï¼?    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?”¥ æ¥µè‡´?Ÿå?"))
    void ExtremeLaunchSystem();

    // ?? ?²å?ç³»çµ±?€??    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?? ?²å?ç³»çµ±?€??))
    FString GetSystemStatus();

    // ?® ?²å??§èƒ½?±å?
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?® ?²å??§èƒ½?±å?"))
    FString GetPerformanceReport();

    // ?? ?æ–°?Ÿå?ç³»çµ±
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?? ?æ–°?Ÿå?"))
    void RestartOptimizationSystem();

    // ?¯ ä¸€?µæ€§èƒ½æª¢æŸ¥
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?¯ ä¸€?µæ€§èƒ½æª¢æŸ¥"))
    void OneClickPerformanceCheck();

    // ?¹ï? ?œæ­¢?ªå?ç³»çµ±
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?¹ï? ?œæ­¢?ªå?"))
    void StopOptimizationSystem();

    // ?? ?²å?ç³»çµ±?¥åº·åº?    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?? ?²å??¥åº·åº?))
    float GetSystemHealth();

    // ?¨ é¡¯ç¤º?§èƒ½ä¿¡æ¯
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?¨ é¡¯ç¤º?§èƒ½ä¿¡æ¯"))
    void TogglePerformanceDisplay();

    // ?“± ?¼é€æ€§èƒ½?šçŸ¥
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?“± ?¼é€é€šçŸ¥"))
    void SendPerformanceNotification();

protected:
    // ä¸€?µå??•å™¨
    UPROPERTY(BlueprintReadOnly, Category = "One Click Game Mode")
    TObjectPtr<UMingRTSOneClickLauncher> OneClickLauncher;

    // ç³»çµ±?ç½®
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "One Click Game Mode")
    bool bAutoLaunchOnBeginPlay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "One Click Game Mode")
    bool bShowPerformanceInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "One Click Game Mode")
    bool bAutoPerformanceCheck;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "One Click Game Mode")
    float PerformanceCheckInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "One Click Game Mode")
    bool bEnableNotifications;

private:
    // ?§éƒ¨?è¼¯
    void InitializeOneClickSystem();
    void SetupAutoPerformanceCheck();
    void HandlePerformanceWarnings();
    void UpdatePerformanceDisplay();
    
    // è¨ˆæ???    float LastPerformanceCheck;
    float LastDisplayUpdate;
    
    // ?€?‹è?è¸?    bool bPerformanceDisplayActive;
    bool bSystemInitialized;

public:
    // ?²å??Ÿå??¨å¯¦ä¾‹ï??¨æ–¼?¶ä?é¡è¨ª?ï?
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode")
    UMingRTSOneClickLauncher* GetOneClickLauncher() const { return OneClickLauncher; }
};
