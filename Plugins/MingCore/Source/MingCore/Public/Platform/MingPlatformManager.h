#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Platform\IPlatformInterface.h"
#include "MingPlatformManager.generated.h"

/**
 * å¹³å°ç®¡ç??? * è² è²¬ç®¡ç??Œå?èª¿å¹³?°é©?å™¨
 */
UCLASS()
class MINGCORE_API UMingPlatformManager : public UObject
{
    GENERATED_BODY()

public:
    UMingPlatformManager();

    // ?å??–å¹³?°ç®¡?†å™¨
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void Initialize();

    // ?œé?å¹³å°ç®¡ç???    UFUNCTION(BlueprintCallable, Category = "Platform")
    void Shutdown();

    // ?²å??¶å?å¹³å°?©é???    UFUNCTION(BlueprintCallable, Category = "Platform")
    TScriptInterface<IPlatformInterface> GetCurrentPlatform() const;

    // ?²å??¶å?å¹³å°é¡å?
    UFUNCTION(BlueprintCallable, Category = "Platform")
    EPlatformType GetCurrentPlatformType() const;

    // ?²å??¶å?å¹³å°?½å?
    UFUNCTION(BlueprintCallable, Category = "Platform")
    FPlatformCapabilities GetPlatformCapabilities() const;

    // ?²å?å±å?DPI
    UFUNCTION(BlueprintCallable, Category = "Platform")
    float GetScreenDPI() const;

    // ?²å?å®‰å…¨?€??    UFUNCTION(BlueprintCallable, Category = "Platform")
    FMargin GetSafeZone() const;

    // ?¯å¦?ºè§¸?§è¨­??    UFUNCTION(BlueprintCallable, Category = "Platform")
    bool IsTouchDevice() const;

    // è¨­ç½®?§èƒ½æ¨¡å?
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void SetPerformanceMode(int32 Mode);

    // ?²å??»æ??»é?
    UFUNCTION(BlueprintCallable, Category = "Platform")
    float GetBatteryLevel() const;

    // é¡¯ç¤ºå¹³å°å°è©±æ¡?    UFUNCTION(BlueprintCallable, Category = "Platform")
    void ShowPlatformDialog(const FString& Title, const FString& Message);

    // ?†äº«?§å®¹
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void ShareContent(const FString& Content);

    // è«‹æ??‰ç”¨è©•å?
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void RequestAppRating();

    // ?œæ??²å?å¯¦ä?ï¼ˆæ–¹ä¾¿å…¨å±€è¨ªå?ï¼?    static UMingPlatformManager* Get();

private:
    bool bInitialized;
    
    // ?¶å?å¹³å°?©é???    UPROPERTY()
    TObjectPtr<UObject> CurrentPlatformAdapter;
    
    // ?®ä?å¯¦ä?
    static UMingPlatformManager* Instance;
    
    // ?µå»ºå¹³å°?©é???    void CreatePlatformAdapter();
};
