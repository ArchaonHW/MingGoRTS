#include "Platform/IOSPlatformAdapter.h"

#if PLATFORM_IOS
#include "IOS/IOSAppDelegate.h"
#include "IOS/IOSView.h"
#endif

UIOSPlatformAdapter::UIOSPlatformAdapter()
    : bInitialized(false)
    , CurrentPerformanceMode(1) // iOS 默認平衡模式
    , bSafeZoneCached(false)
{
}

EPlatformType UIOSPlatformAdapter::GetPlatformType() const
{
    return EPlatformType::IOS;
}

FPlatformCapabilities UIOSPlatformAdapter::GetCapabilities() const
{
    FPlatformCapabilities Capabilities;
    Capabilities.bSupportsMultiTouch = true;
    Capabilities.MaxTouchPoints = 5; // iOS 默認 5 點觸控
    Capabilities.bSupportsHardwareKeyboard = false; // 大部分 iOS 設備沒有物理鍵盤
    Capabilities.bSupportsMouse = false; // iOS 13+ 支持鼠標，但默認不使用
    Capabilities.DefaultInputType = EInputType::Touch;
    Capabilities.bRequiresBatteryOptimization = true;
    Capabilities.RecommendedMaxUnits = 600; // iOS 性能較好，可以比 Android 多一點
    Capabilities.RecommendedQualityLevel = 3; // iOS 可以支持較高畫質
    
    return Capabilities;
}

void UIOSPlatformAdapter::Initialize()
{
    if (bInitialized)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("iOS Platform Adapter initialized"));
    UE_LOG(LogTemp, Log, TEXT("iOS Version: %s"), *GetIOSVersion());
    UE_LOG(LogTemp, Log, TEXT("Device: %s"), IsIPad() ? TEXT("iPad") : TEXT("iPhone/iPod"));
    
    // 初始化安全區域
    CachedSafeZone = FMargin(0, 0, 0, 0);
    bSafeZoneCached = false;
    
    bInitialized = true;
}

void UIOSPlatformAdapter::Shutdown()
{
    if (!bInitialized)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("iOS Platform Adapter shutdown"));
    bInitialized = false;
}

float UIOSPlatformAdapter::GetScreenDPI() const
{
    // iOS 設備 DPI:
    // iPhone: 326 (標準), 458 (Retina HD), 460 (Super Retina)
    // iPad: 264 (標準), 326 (Retina)
    // 這裡返回常見值，實際應該根據具體設備返回
    return IsIPad() ? 264.0f : 326.0f;
}

FMargin UIOSPlatformAdapter::GetSafeZone() const
{
    if (!bSafeZoneCached)
    {
#if PLATFORM_IOS
        // iOS 11+ 有安全區域概念 (劉海屏、Home Indicator)
        // 這裡應該從 iOS API 獲取實際的安全區域
        // 暫時使用估計值
        bool bHasNotch = !IsIPad(); // iPhone X 及以後有劉海
        if (bHasNotch)
        {
            // iPhone 有劉海屏：頂部 44 像素，底部 34 像素 (Home Indicator)
            CachedSafeZone = FMargin(0, 44, 0, 34);
        }
        else
        {
            // iPad 或舊 iPhone：只有狀態欄
            CachedSafeZone = FMargin(0, 20, 0, 0);
        }
#else
        CachedSafeZone = FMargin(0, 20, 0, 0);
#endif
        bSafeZoneCached = true;
    }
    
    return CachedSafeZone;
}

bool UIOSPlatformAdapter::IsTouchDevice() const
{
    return true;
}

void UIOSPlatformAdapter::SetPerformanceMode(int32 Mode)
{
    CurrentPerformanceMode = Mode;
    
#if PLATFORM_IOS
    // iOS 可以使用 CADisplayLink 優化幀率
    // 或者調整圖形設置
#endif
    
    switch (Mode)
    {
    case 0: // 省電模式
        UE_LOG(LogTemp, Log, TEXT("iOS Performance Mode: Power Saving (30 FPS)"));
        break;
    case 1: // 平衡模式
        UE_LOG(LogTemp, Log, TEXT("iOS Performance Mode: Balanced (60 FPS)"));
        break;
    case 2: // 性能模式
        UE_LOG(LogTemp, Log, TEXT("iOS Performance Mode: Performance (60 FPS, High Quality)"));
        break;
    default:
        break;
    }
}

float UIOSPlatformAdapter::GetBatteryLevel() const
{
#if PLATFORM_IOS
    // 使用 UIDevice 獲取電量
    // [UIDevice currentDevice].batteryLevel 返回 0.0 到 1.0
    // 暫時返回估計值
    return 0.80f; // 80%
#endif
    return -1.0f;
}

bool UIOSPlatformAdapter::IsCharging() const
{
#if PLATFORM_IOS
    // [UIDevice currentDevice].batteryState
    // UIDeviceBatteryStateCharging 或 UIDeviceBatteryStateFull
#endif
    return false;
}

void UIOSPlatformAdapter::ShowPlatformDialog(const FString& Title, const FString& Message)
{
#if PLATFORM_IOS
    // 使用 UIAlertController 顯示對話框
    UE_LOG(LogTemp, Log, TEXT("Showing iOS alert: %s"), *Title);
#endif
}

void UIOSPlatformAdapter::ShareContent(const FString& Content)
{
#if PLATFORM_IOS
    // 使用 UIActivityViewController 分享
    UE_LOG(LogTemp, Log, TEXT("Sharing content on iOS: %s"), *Content);
#endif
}

void UIOSPlatformAdapter::RequestAppRating()
{
#if PLATFORM_IOS
    // 使用 StoreKit 的 SKStoreReviewController
    // [SKStoreReviewController requestReview]
    UE_LOG(LogTemp, Log, TEXT("Requesting app rating on iOS"));
#endif
}

bool UIOSPlatformAdapter::SaveToPlatformStorage(const FString& Key, const FString& Value)
{
#if PLATFORM_IOS
    // 使用 NSUserDefaults
    UE_LOG(LogTemp, Log, TEXT("Saving to iOS UserDefaults: %s"), *Key);
    return true;
#endif
    return false;
}

FString UIOSPlatformAdapter::LoadFromPlatformStorage(const FString& Key) const
{
#if PLATFORM_IOS
    // 從 NSUserDefaults 讀取
    UE_LOG(LogTemp, Log, TEXT("Loading from iOS UserDefaults: %s"), *Key);
    return TEXT("");
#endif
    return FString();
}

FString UIOSPlatformAdapter::GetIOSVersion() const
{
#if PLATFORM_IOS
    // 使用 UIDevice systemVersion
    // [[UIDevice currentDevice] systemVersion]
    return TEXT("iOS 15.0+"); // 暫時返回
#endif
    return TEXT("Unknown iOS Version");
}

bool UIOSPlatformAdapter::IsIPad() const
{
#if PLATFORM_IOS
    // 使用 UIDevice userInterfaceIdiom
    // UIUserInterfaceIdiomPad
    // 根據屏幕尺寸判斷也可以
    return false; // 暫時返回 false
#endif
    return false;
}

void UIOSPlatformAdapter::HapticFeedback(int32 Intensity)
{
#if PLATFORM_IOS
    // iOS 10+ 支持 Core Haptics
    // UIImpactFeedbackGenerator
    // UINotificationFeedbackGenerator
    // UISelectionFeedbackGenerator
    
    switch (Intensity)
    {
    case 0: // 輕
        UE_LOG(LogTemp, Verbose, TEXT("iOS Haptic: Light"));
        break;
    case 1: // 中
        UE_LOG(LogTemp, Verbose, TEXT("iOS Haptic: Medium"));
        break;
    case 2: // 重
        UE_LOG(LogTemp, Verbose, TEXT("iOS Haptic: Heavy"));
        break;
    default:
        break;
    }
#endif
}

void UIOSPlatformAdapter::RegisterForPushNotifications()
{
#if PLATFORM_IOS
    // 使用 UNUserNotificationCenter
    // requestAuthorizationWithOptions
    UE_LOG(LogTemp, Log, TEXT("Registering for push notifications on iOS"));
#endif
}
