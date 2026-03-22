#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Platform\IPlatformInterface.h"
#include "MingPlatformManager.generated.h"

/**
 * ���x��?X * �t�d��?X???�ե�??�A??��
 */
UCLASS()
class MINGCORE_API UMingPlatformManager : public UObject
{
    GENERATED_BODY()

public:
    UMingPlatformManager();

    // ???X?��??��??��
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void Initialize();

    
    void Shutdown();

    
    TScriptInterface<IPlatformInterface> GetCurrentPlatform() const;

    // 目標數量���x��??
    UFUNCTION(BlueprintCallable, Category = "Platform")
    EPlatformType GetCurrentPlatformType() const;

    // 目標數量���x摧毀
    UFUNCTION(BlueprintCallable, Category = "Platform")
    FPlatformCapabilities GetPlatformCapabilities() const;

    // 摧毀��??DPI
    UFUNCTION(BlueprintCallable, Category = "Platform")
    float GetScreenDPI() const;

    
    FMargin GetSafeZone() const;

    
    bool IsTouchDevice() const;

    // �]�m??���??
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void SetPerformanceMode(int32 Mode);

    // 故事選項?
    UFUNCTION(BlueprintCallable, Category = "Platform")
    float GetBatteryLevel() const;

    
    void ShowPlatformDialog(const FString& Title, const FString& Message);

    // ??��??�e
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void ShareContent(const FString& Content);

    // ��?X?�ε�??
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void RequestAppRating();

    // 目標數量��??�]��K�����X摧毀
    static UMingPlatformManager* Get();

private:
    bool bInitialized;
    
    
    TObjectPtr<UObject> CurrentPlatformAdapter;
    
    // 摧毀��??
    static UMingPlatformManager* Instance;
    
    // ??�إ��x???X
    void CreatePlatformAdapter();
};

