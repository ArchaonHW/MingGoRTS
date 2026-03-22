#pragma once

#include "CoreMinimal.h"
#include "Platform\IPlatformInterface.h"
#include "WindowsPlatformAdapter.generated.h"

/**
 * Windows 平台???X */
UCLASS()
class MINGCORE_API UWindowsPlatformAdapter : public UObject, public IPlatformInterface
{
    GENERATED_BODY()

public:
    UWindowsPlatformAdapter();

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

    // Windows ???X?能
    UFUNCTION(BlueprintCallable, Category = "Platform|Windows")
    bool IsRunningOnBattery() const;

    UFUNCTION(BlueprintCallable, Category = "Platform|Windows")
    FString GetWindowsVersion() const;

private:
    bool bInitialized;
    int32 CurrentPerformanceMode;
};

