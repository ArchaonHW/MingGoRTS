#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Platform\IPlatformInterface.h"
#include "MingPlatformManager.generated.h"

/**
 * 平台管理器
 * 負責管理和協調平台適配器
 */
UCLASS()
class MINGCORE_API UMingPlatformManager : public UObject
{
    GENERATED_BODY()

public:
    UMingPlatformManager();

    // 初始化平台管理器
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void Initialize();

    // 關閉平台管理器
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void Shutdown();

    // 獲取當前平台適配器
    UFUNCTION(BlueprintCallable, Category = "Platform")
    IPlatformInterface* GetCurrentPlatform() const;

    // 獲取當前平台類型
    UFUNCTION(BlueprintCallable, Category = "Platform")
    EPlatformType GetCurrentPlatformType() const;

    // 獲取當前平台能力
    UFUNCTION(BlueprintCallable, Category = "Platform")
    FPlatformCapabilities GetPlatformCapabilities() const;

    // 獲取屏幕DPI
    UFUNCTION(BlueprintCallable, Category = "Platform")
    float GetScreenDPI() const;

    // 獲取安全區域
    UFUNCTION(BlueprintCallable, Category = "Platform")
    FMargin GetSafeZone() const;

    // 是否為觸控設備
    UFUNCTION(BlueprintCallable, Category = "Platform")
    bool IsTouchDevice() const;

    // 設置性能模式
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void SetPerformanceMode(int32 Mode);

    // 獲取電池電量
    UFUNCTION(BlueprintCallable, Category = "Platform")
    float GetBatteryLevel() const;

    // 顯示平台對話框
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void ShowPlatformDialog(const FString& Title, const FString& Message);

    // 分享內容
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void ShareContent(const FString& Content);

    // 請求應用評分
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void RequestAppRating();

    // 靜態獲取實例（方便全局訪問）
    static UMingPlatformManager* Get();

private:
    bool bInitialized;
    
    // 當前平台適配器
    UPROPERTY()
    TObjectPtr<UObject> CurrentPlatformAdapter;
    
    // 單例實例
    static UMingPlatformManager* Instance;
    
    // 創建平台適配器
    void CreatePlatformAdapter();
};
