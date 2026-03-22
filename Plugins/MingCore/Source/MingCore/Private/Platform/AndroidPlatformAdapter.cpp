#include "Platform/AndroidPlatformAdapter.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#endif

UAndroidPlatformAdapter::UAndroidPlatformAdapter()
    : bInitialized(false)
    , CurrentPerformanceMode(1) // Android 默認平衡模式
    , bSafeZoneCached(false)
{
}

EPlatformType UAndroidPlatformAdapter::GetPlatformType() const
{
    return EPlatformType::Android;
}

FPlatformCapabilities UAndroidPlatformAdapter::GetCapabilities() const
{
    FPlatformCapabilities Capabilities;
    Capabilities.bSupportsMultiTouch = true;
    Capabilities.MaxTouchPoints = 10;
    Capabilities.bSupportsHardwareKeyboard = false; // 大部分 Android 設備沒有物理鍵盤
    Capabilities.bSupportsMouse = false; // 默認不支持
    Capabilities.DefaultInputType = EInputType::Touch;
    Capabilities.bRequiresBatteryOptimization = true;
    Capabilities.RecommendedMaxUnits = 500; // 移動端減少單位數
    Capabilities.RecommendedQualityLevel = 2; // 移動端中等畫質
    
    return Capabilities;
}

void UAndroidPlatformAdapter::Initialize()
{
    if (bInitialized)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Android Platform Adapter initialized"));
    UE_LOG(LogTemp, Log, TEXT("Android Version: %s"), *GetAndroidVersion());
    
    // 初始化安全區域
    CachedSafeZone = FMargin(0, 0, 0, 0);
    bSafeZoneCached = false;
    
    bInitialized = true;
}

void UAndroidPlatformAdapter::Shutdown()
{
    if (!bInitialized)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Android Platform Adapter shutdown"));
    bInitialized = false;
}

float UAndroidPlatformAdapter::GetScreenDPI() const
{
    // Android DPI 變化範圍很大，通常在 160-640 之間
    // 這裡返回一個估計值，實際應該從 Java 層獲取
    return 320.0f;
}

FMargin UAndroidPlatformAdapter::GetSafeZone() const
{
    if (!bSafeZoneCached)
    {
        // TODO: 從 Java 層獲取安全區域（劉海屏、圓角等）
        // 這裡使用默認值
        CachedSafeZone = FMargin(0, 40, 0, 0); // 頂部預留 40 像素給狀態欄
        bSafeZoneCached = true;
    }
    
    return CachedSafeZone;
}

bool UAndroidPlatformAdapter::IsTouchDevice() const
{
    return true;
}

void UAndroidPlatformAdapter::SetPerformanceMode(int32 Mode)
{
    CurrentPerformanceMode = Mode;
    
#if PLATFORM_ANDROID
    // 調用 Java 方法設置性能模式
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    {
        // 這裡可以調用 Android 的 setPerformanceMode API
        UE_LOG(LogTemp, Log, TEXT("Setting Android performance mode: %d"), Mode);
    }
#endif
    
    switch (Mode)
    {
    case 0: // 省電模式
        UE_LOG(LogTemp, Log, TEXT("Android Performance Mode: Power Saving"));
        break;
    case 1: // 平衡模式
        UE_LOG(LogTemp, Log, TEXT("Android Performance Mode: Balanced"));
        break;
    case 2: // 性能模式
        UE_LOG(LogTemp, Log, TEXT("Android Performance Mode: Performance"));
        break;
    default:
        break;
    }
}

float UAndroidPlatformAdapter::GetBatteryLevel() const
{
#if PLATFORM_ANDROID
    // 通過 JNI 調用 Android 電池 API
    // 返回 0.0 到 1.0 的電量百分比
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    {
        // TODO: 實現 JNI 調用獲取電量
        return 0.75f; // 暫時返回 75%
    }
#endif
    return -1.0f;
}

bool UAndroidPlatformAdapter::IsCharging() const
{
#if PLATFORM_ANDROID
    // 通過 JNI 檢查充電狀態
#endif
    return false;
}

void UAndroidPlatformAdapter::ShowPlatformDialog(const FString& Title, const FString& Message)
{
#if PLATFORM_ANDROID
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    {
        // 調用 Java 顯示 AlertDialog
        UE_LOG(LogTemp, Log, TEXT("Showing Android dialog: %s"), *Title);
    }
#endif
}

void UAndroidPlatformAdapter::ShareContent(const FString& Content)
{
#if PLATFORM_ANDROID
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    {
        // 調用 Android 分享 Intent
        UE_LOG(LogTemp, Log, TEXT("Sharing content on Android: %s"), *Content);
    }
#endif
}

void UAndroidPlatformAdapter::RequestAppRating()
{
#if PLATFORM_ANDROID
    // 打開 Google Play 評價頁面
    UE_LOG(LogTemp, Log, TEXT("Requesting app rating on Android"));
    // TODO: 啟動 Play Store 評價 Intent
#endif
}

bool UAndroidPlatformAdapter::SaveToPlatformStorage(const FString& Key, const FString& Value)
{
#if PLATFORM_ANDROID
    // 使用 Android SharedPreferences
    UE_LOG(LogTemp, Log, TEXT("Saving to Android storage: %s"), *Key);
    return true; // 暫時返回成功
#endif
    return false;
}

FString UAndroidPlatformAdapter::LoadFromPlatformStorage(const FString& Key) const
{
#if PLATFORM_ANDROID
    UE_LOG(LogTemp, Log, TEXT("Loading from Android storage: %s"), *Key);
    return TEXT(""); // 暫時返回空
#endif
    return FString();
}

FString UAndroidPlatformAdapter::GetAndroidVersion() const
{
    return TEXT("Android API Level: Unknown");
}

bool UAndroidPlatformAdapter::IsTablet() const
{
    // 根據屏幕尺寸判斷是否為平板
    // 平板通常屏幕更大，DPI 更低
    return false; // 暫時返回 false
}

void UAndroidPlatformAdapter::ShowToast(const FString& Message, int32 Duration)
{
#if PLATFORM_ANDROID
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    {
        // 調用 Java Toast API
        UE_LOG(LogTemp, Log, TEXT("Showing Android toast: %s (duration: %d)"), *Message, Duration);
    }
#endif
}

void UAndroidPlatformAdapter::Vibrate(int32 Duration)
{
#if PLATFORM_ANDROID
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    {
        // 調用 Android Vibrator API
        UE_LOG(LogTemp, Log, TEXT("Android vibrate: %d ms"), Duration);
    }
#endif
}
