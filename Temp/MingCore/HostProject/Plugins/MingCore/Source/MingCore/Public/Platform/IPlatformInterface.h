#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPlatformInterface.generated.h"

/**
 * 平台類型枚舉
 */
UENUM(BlueprintType)
enum class EPlatformType : uint8
{
    Windows       UMETA(DisplayName = "Windows"),
    Android       UMETA(DisplayName = "Android"),
    IOS           UMETA(DisplayName = "iOS"),
    Unknown       UMETA(DisplayName = "Unknown")
};

/**
 * 輸入類型枚舉
 */
UENUM(BlueprintType)
enum class EInputType : uint8
{
    MouseKeyboard UMETA(DisplayName = "Mouse & Keyboard"),
    Touch         UMETA(DisplayName = "Touch"),
    Gamepad       UMETA(DisplayName = "Gamepad")
};

/**
 * 平台能力結構
 */
USTRUCT(BlueprintType)
struct FPlatformCapabilities
{
    GENERATED_BODY()
    
    // 是否支持多點觸控
    UPROPERTY(BlueprintReadOnly)
    bool bSupportsMultiTouch;
    
    // 最大同時觸控點數
    UPROPERTY(BlueprintReadOnly)
    int32 MaxTouchPoints;
    
    // 是否支持物理鍵盤
    UPROPERTY(BlueprintReadOnly)
    bool bSupportsHardwareKeyboard;
    
    // 是否支持鼠標
    UPROPERTY(BlueprintReadOnly)
    bool bSupportsMouse;
    
    // 默認輸入類型
    UPROPERTY(BlueprintReadOnly)
    EInputType DefaultInputType;
    
    // 是否需要電池優化
    UPROPERTY(BlueprintReadOnly)
    bool bRequiresBatteryOptimization;
    
    // 建議的最大同時單位數
    UPROPERTY(BlueprintReadOnly)
    int32 RecommendedMaxUnits;
    
    // 建議的畫質等級
    UPROPERTY(BlueprintReadOnly)
    int32 RecommendedQualityLevel;
    
    FPlatformCapabilities()
        : bSupportsMultiTouch(false)
        , MaxTouchPoints(1)
        , bSupportsHardwareKeyboard(true)
        , bSupportsMouse(true)
        , DefaultInputType(EInputType::MouseKeyboard)
        , bRequiresBatteryOptimization(false)
        , RecommendedMaxUnits(1000)
        , RecommendedQualityLevel(3)
    {}
};

/**
 * 平台抽象接口
 * 定義所有平台相關功能的統一接口
 */
UINTERFACE(MinimalAPI)
class MINGCORE_API UPlatformInterface : public UInterface
{
    GENERATED_BODY()
};

class MINGCORE_API IPlatformInterface
{
    GENERATED_BODY()

public:
    virtual ~IPlatformInterface() {}
    
    // 獲取當前平台類型
    virtual EPlatformType GetPlatformType() const = 0;
    
    // 獲取平台能力
    virtual FPlatformCapabilities GetCapabilities() const = 0;
    
    // 初始化平台
    virtual void Initialize() = 0;
    
    // 關閉平台
    virtual void Shutdown() = 0;
    
    // 獲取屏幕DPI
    virtual float GetScreenDPI() const = 0;
    
    // 獲取安全區域 (用於移動端的劉海屏等)
    virtual FMargin GetSafeZone() const = 0;
    
    // 檢查是否為觸控設備
    virtual bool IsTouchDevice() const = 0;
    
    // 設置性能模式
    virtual void SetPerformanceMode(int32 Mode) = 0;
    
    // 獲取當前電池電量 (0-1, -1表示不支持)
    virtual float GetBatteryLevel() const = 0;
    
    // 是否正在充電
    virtual bool IsCharging() const = 0;
    
    // 顯示平台特定的對話框
    virtual void ShowPlatformDialog(const FString& Title, const FString& Message) = 0;
    
    // 分享功能
    virtual void ShareContent(const FString& Content) = 0;
    
    // 評分請求
    virtual void RequestAppRating() = 0;
    
    // 保存數據到平台特定存儲
    virtual bool SaveToPlatformStorage(const FString& Key, const FString& Value) = 0;
    
    // 從平台特定存儲讀取數據
    virtual FString LoadFromPlatformStorage(const FString& Key) const = 0;
};
