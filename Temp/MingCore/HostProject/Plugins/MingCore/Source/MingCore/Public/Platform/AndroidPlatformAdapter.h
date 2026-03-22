#pragma once

#include "CoreMinimal.h"
#include "Platform\IPlatformInterface.h"
#include "AndroidPlatformAdapter.generated.h"

/**
 * Android 平台適配器
 */
UCLASS()
class MINGCORE_API UAndroidPlatformAdapter : public UObject, public IPlatformInterface
{
    GENERATED_BODY()

public:
    UAndroidPlatformAdapter();

    // IPlatformInterface 實現
    virtual EPlatformType GetPlatformType() const override;
    virtual FPlatformCapabilities GetCapabilities() const override;
    virtual void Initialize() override;
    virtual void Shutdown() override;
    virtual float GetScreenDPI() const override;
    virtual FMargin GetSafeZone() const override;
    virtual bool IsTouchDevice() const override;
    virtual void SetPerformanceMode(int32 Mode) override;
    virtual float GetBatteryLevel() const override;
    virtual bool IsCharging() const override;
    virtual void ShowPlatformDialog(const FString& Title, const FString& Message) override;
    virtual void ShareContent(const FString& Content) override;
    virtual void RequestAppRating() override;
    virtual bool SaveToPlatformStorage(const FString& Key, const FString& Value) override;
    virtual FString LoadFromPlatformStorage(const FString& Key) const override;

    // Android 特定功能
    UFUNCTION(BlueprintCallable, Category = "Platform|Android")
    FString GetAndroidVersion() const;

    UFUNCTION(BlueprintCallable, Category = "Platform|Android")
    bool IsTablet() const;

    UFUNCTION(BlueprintCallable, Category = "Platform|Android")
    void ShowToast(const FString& Message, int32 Duration = 2000);

    UFUNCTION(BlueprintCallable, Category = "Platform|Android")
    void Vibrate(int32 Duration = 100);

private:
    bool bInitialized;
    int32 CurrentPerformanceMode;
    
    // 安全區域緩存
    FMargin CachedSafeZone;
    bool bSafeZoneCached;
};
