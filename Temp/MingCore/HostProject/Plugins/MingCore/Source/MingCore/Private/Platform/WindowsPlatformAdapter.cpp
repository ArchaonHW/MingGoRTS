#include "Platform/WindowsPlatformAdapter.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "GenericPlatform/GenericPlatformMisc.h"

UWindowsPlatformAdapter::UWindowsPlatformAdapter()
    : bInitialized(false)
    , CurrentPerformanceMode(0)
{
}

EPlatformType UWindowsPlatformAdapter::GetPlatformType() const
{
    return EPlatformType::Windows;
}

FPlatformCapabilities UWindowsPlatformAdapter::GetCapabilities() const
{
    FPlatformCapabilities Capabilities;
    Capabilities.bSupportsMultiTouch = false;
    Capabilities.MaxTouchPoints = 0;
    Capabilities.bSupportsHardwareKeyboard = true;
    Capabilities.bSupportsMouse = true;
    Capabilities.DefaultInputType = EInputType::MouseKeyboard;
    Capabilities.bRequiresBatteryOptimization = false;
    Capabilities.RecommendedMaxUnits = 1000;
    Capabilities.RecommendedQualityLevel = 5; // Windows PC 使用最高畫質
    
    return Capabilities;
}

void UWindowsPlatformAdapter::Initialize()
{
    if (bInitialized)
    {
        return;
    }
    
    // Windows 平台初始化
    UE_LOG(LogTemp, Log, TEXT("Windows Platform Adapter initialized"));
    UE_LOG(LogTemp, Log, TEXT("Windows Version: %s"), *GetWindowsVersion());
    
    bInitialized = true;
}

void UWindowsPlatformAdapter::Shutdown()
{
    if (!bInitialized)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Windows Platform Adapter shutdown"));
    bInitialized = false;
}

float UWindowsPlatformAdapter::GetScreenDPI() const
{
    // Windows 默認 DPI 為 96
    return 96.0f;
}

FMargin UWindowsPlatformAdapter::GetSafeZone() const
{
    // Windows 沒有安全區域問題
    return FMargin(0, 0, 0, 0);
}

bool UWindowsPlatformAdapter::IsTouchDevice() const
{
    return false;
}

void UWindowsPlatformAdapter::SetPerformanceMode(int32 Mode)
{
    CurrentPerformanceMode = Mode;
    
    switch (Mode)
    {
    case 0: // 省電模式
        UE_LOG(LogTemp, Log, TEXT("Windows Performance Mode: Power Saving"));
        break;
    case 1: // 平衡模式
        UE_LOG(LogTemp, Log, TEXT("Windows Performance Mode: Balanced"));
        break;
    case 2: // 性能模式
        UE_LOG(LogTemp, Log, TEXT("Windows Performance Mode: Performance"));
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown performance mode: %d"), Mode);
        break;
    }
}

float UWindowsPlatformAdapter::GetBatteryLevel() const
{
    // Windows 桌面端不一定有電池，返回 -1 表示不支持
    if (IsRunningOnBattery())
    {
        // 這裡可以調用 Windows API 獲取電量
        return -1.0f;
    }
    return -1.0f;
}

bool UWindowsPlatformAdapter::IsCharging() const
{
    // 桌面端默認認為正在供電
    return !IsRunningOnBattery();
}

void UWindowsPlatformAdapter::ShowPlatformDialog(const FString& Title, const FString& Message)
{
    // Windows 使用標準的消息框
    FGenericPlatformMisc::MessageBoxExt(EAppMsgType::Ok, *Message, *Title);
}

void UWindowsPlatformAdapter::ShareContent(const FString& Content)
{
    // Windows 桌面端分享功能，可以複製到剪貼板
    UE_LOG(LogTemp, Log, TEXT("Share content on Windows: %s"), *Content);
    // TODO: 實現複製到剪貼板
}

void UWindowsPlatformAdapter::RequestAppRating()
{
    // Windows 桌面端可以打開 Steam 評價頁面或其他平台
    UE_LOG(LogTemp, Log, TEXT("Request app rating on Windows"));
    // TODO: 打開平台特定的評價頁面
}

bool UWindowsPlatformAdapter::SaveToPlatformStorage(const FString& Key, const FString& Value)
{
    // Windows 使用標準的文件系統
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("PlatformStorage") / Key;
    return FFileHelper::SaveStringToFile(Value, *SavePath);
}

FString UWindowsPlatformAdapter::LoadFromPlatformStorage(const FString& Key) const
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("PlatformStorage") / Key;
    FString Value;
    if (FFileHelper::LoadFileToString(Value, *SavePath))
    {
        return Value;
    }
    return FString();
}

bool UWindowsPlatformAdapter::IsRunningOnBattery() const
{
    // 使用 Windows API 檢查電源狀態
    SYSTEM_POWER_STATUS PowerStatus;
    if (GetSystemPowerStatus(&PowerStatus))
    {
        return PowerStatus.ACLineStatus == 0;
    }
    return false;
}

FString UWindowsPlatformAdapter::GetWindowsVersion() const
{
    // 獲取 Windows 版本信息
    OSVERSIONINFOEXW OSVersionInfo;
    ZeroMemory(&OSVersionInfo, sizeof(OSVERSIONINFOEXW));
    OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
    
    #pragma warning(push)
    #pragma warning(disable: 4996) // 忽略棄用警告
    if (GetVersionExW(reinterpret_cast<LPOSVERSIONINFOW>(&OSVersionInfo)))
    {
        return FString::Printf(TEXT("Windows %d.%d (Build %d)"),
            OSVersionInfo.dwMajorVersion,
            OSVersionInfo.dwMinorVersion,
            OSVersionInfo.dwBuildNumber);
    }
    #pragma warning(pop)
    
    return TEXT("Unknown Windows Version");
}
