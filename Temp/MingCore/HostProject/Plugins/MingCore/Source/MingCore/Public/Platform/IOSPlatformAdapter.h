#pragma once

#include "CoreMinimal.h"
#include "Platform\IPlatformInterface.h"
#include "IOSPlatformAdapter.generated.h"

/**
 * iOS 平台適配器
 */
UCLASS()
class MINGCORE_API UIOSPlatformAdapter : public UObject, public IPlatformInterface
{
    GENERATED_BODY()

public:
    UIOSPlatformAdapter();

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

    // iOS 特定功能
    UFUNCTION(BlueprintCallable, Category = "Platform|iOS")
    FString GetIOSVersion() const;

    UFUNCTION(BlueprintCallable, Category = "Platform|iOS")
    bool IsIPad() const;

    UFUNCTION(BlueprintCallable, Category = "Platform|iOS")
    void HapticFeedback(int32 Intensity = 1);

    UFUNCTION(BlueprintCallable, Category = "Platform|iOS")
    void RegisterForPushNotifications();

private:
    bool bInitialized;
    int32 CurrentPerformanceMode;
    
    // 安全區域緩存
    FMargin CachedSafeZone;
    bool bSafeZoneCached;
};
