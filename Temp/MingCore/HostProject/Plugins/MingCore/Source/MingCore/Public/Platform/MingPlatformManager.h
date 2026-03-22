#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Platform\IPlatformInterface.h"
#include "MingPlatformManager.generated.h"

/**
 * 平台管�X * 負責管�X��?調平?�適?�器
 */
UCLASS()
class MINGCORE_API UMingPlatformManager : public UObject
{
    GENERATED_BODY()

public:
    UMingPlatformManager(};

    // ?��X�平?�管?�器
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void Initialize(};

    // ?��?平台管�X    UFUNCTION(BlueprintCallable, Category = "Platform")
    void Shutdown(};

    // ?��X��?平台?��X    UFUNCTION(BlueprintCallable, Category = "Platform")
    TScriptInterface<IPlatformInterface> GetCurrentPlatform() const;

    // ?��X��?平台類�?
    UFUNCTION(BlueprintCallable, Category = "Platform")
    EPlatformType GetCurrentPlatformType() const;

    // ?��X��?平台?��?
    UFUNCTION(BlueprintCallable, Category = "Platform")
    FPlatformCapabilities GetPlatformCapabilities() const;

    // ?��?屏�?DPI
    UFUNCTION(BlueprintCallable, Category = "Platform")
    float GetScreenDPI() const;

    // ?��?安全?�X    UFUNCTION(BlueprintCallable, Category = "Platform")
    FMargin GetSafeZone() const;

    // ?�否?�觸?�設X    UFUNCTION(BlueprintCallable, Category = "Platform")
    bool IsTouchDevice() const;

    // 設置?�能模�?
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void SetPerformanceMode(int32 Mode};

    // ?��X��X��?
    UFUNCTION(BlueprintCallable, Category = "Platform")
    float GetBatteryLevel() const;

    // 顯示平台對話�?    UFUNCTION(BlueprintCallable, Category = "Platform")
    void ShowPlatformDialog(const FString& Title, const FString& Message};

    // ?�享?�容
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void ShareContent(const FString& Content};

    // 請�X�用評�?
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void RequestAppRating(};

    // ?��X��?實�?（方便全局訪�?�?    static UMingPlatformManager* Get(};

private:
    bool bInitialized;
    
    // ?��?平台?��X    UPROPERTY()
    TObjectPtr<UObject> CurrentPlatformAdapter;
    
    // ?��?實�?
    static UMingPlatformManager* Instance;
    
    // ?�建平台?��X    void CreatePlatformAdapter(};
};

