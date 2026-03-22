#include "Platform/MingPlatformManager.h"
#include "Platform/WindowsPlatformAdapter.h"
#include "Platform/AndroidPlatformAdapter.h"
#include "Platform/IOSPlatformAdapter.h"

// 靜態實例初始化
UMingPlatformManager* UMingPlatformManager::Instance = nullptr;

UMingPlatformManager::UMingPlatformManager()
    : bInitialized(false)
    , CurrentPlatformAdapter(nullptr)
{
}

void UMingPlatformManager::Initialize()
{
    if (bInitialized)
    {
        return;
    }
    
    // 創建平台適配器
    CreatePlatformAdapter();
    
    // 初始化平台適配器
    if (CurrentPlatformAdapter)
    {
        IPlatformInterface* Platform = Cast<IPlatformInterface>(CurrentPlatformAdapter);
        if (Platform)
        {
            Platform->Initialize();
            
            FPlatformCapabilities Capabilities = Platform->GetCapabilities();
            UE_LOG(LogTemp, Log, TEXT("Platform initialized: %s"), 
                *UEnum::GetValueAsString(Platform->GetPlatformType()));
            UE_LOG(LogTemp, Log, TEXT("Default input: %s, Max units: %d"),
                *UEnum::GetValueAsString(Capabilities.DefaultInputType),
                Capabilities.RecommendedMaxUnits);
        }
    }
    
    Instance = this;
    bInitialized = true;
}

void UMingPlatformManager::Shutdown()
{
    if (!bInitialized)
    {
        return;
    }
    
    if (CurrentPlatformAdapter)
    {
        IPlatformInterface* Platform = Cast<IPlatformInterface>(CurrentPlatformAdapter);
        if (Platform)
        {
            Platform->Shutdown();
        }
    }
    
    CurrentPlatformAdapter = nullptr;
    Instance = nullptr;
    bInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("PlatformManager shutdown"));
}

void UMingPlatformManager::CreatePlatformAdapter()
{
    // 根據當前平台創建對應的適配器
    #if PLATFORM_WINDOWS
    {
        CurrentPlatformAdapter = NewObject<UWindowsPlatformAdapter>(this);
        UE_LOG(LogTemp, Log, TEXT("Created Windows platform adapter"));
    }
    #elif PLATFORM_ANDROID
    {
        CurrentPlatformAdapter = NewObject<UAndroidPlatformAdapter>(this);
        UE_LOG(LogTemp, Log, TEXT("Created Android platform adapter"));
    }
    #elif PLATFORM_IOS
    {
        CurrentPlatformAdapter = NewObject<UIOSPlatformAdapter>(this);
        UE_LOG(LogTemp, Log, TEXT("Created iOS platform adapter"));
    }
    #else
    {
        UE_LOG(LogTemp, Error, TEXT("Unsupported platform!"));
    }
    #endif
}

IPlatformInterface* UMingPlatformManager::GetCurrentPlatform() const
{
    if (CurrentPlatformAdapter)
    {
        return Cast<IPlatformInterface>(CurrentPlatformAdapter);
    }
    return nullptr;
}

EPlatformType UMingPlatformManager::GetCurrentPlatformType() const
{
    IPlatformInterface* Platform = GetCurrentPlatform();
    if (Platform)
    {
        return Platform->GetPlatformType();
    }
    return EPlatformType::Unknown;
}

FPlatformCapabilities UMingPlatformManager::GetPlatformCapabilities() const
{
    IPlatformInterface* Platform = GetCurrentPlatform();
    if (Platform)
    {
        return Platform->GetCapabilities();
    }
    return FPlatformCapabilities();
}

float UMingPlatformManager::GetScreenDPI() const
{
    IPlatformInterface* Platform = GetCurrentPlatform();
    if (Platform)
    {
        return Platform->GetScreenDPI();
    }
    return 96.0f; // 默認 Windows DPI
}

FMargin UMingPlatformManager::GetSafeZone() const
{
    IPlatformInterface* Platform = GetCurrentPlatform();
    if (Platform)
    {
        return Platform->GetSafeZone();
    }
    return FMargin(0, 0, 0, 0);
}

bool UMingPlatformManager::IsTouchDevice() const
{
    IPlatformInterface* Platform = GetCurrentPlatform();
    if (Platform)
    {
        return Platform->IsTouchDevice();
    }
    return false;
}

void UMingPlatformManager::SetPerformanceMode(int32 Mode)
{
    IPlatformInterface* Platform = GetCurrentPlatform();
    if (Platform)
    {
        Platform->SetPerformanceMode(Mode);
    }
}

float UMingPlatformManager::GetBatteryLevel() const
{
    IPlatformInterface* Platform = GetCurrentPlatform();
    if (Platform)
    {
        return Platform->GetBatteryLevel();
    }
    return -1.0f;
}

void UMingPlatformManager::ShowPlatformDialog(const FString& Title, const FString& Message)
{
    IPlatformInterface* Platform = GetCurrentPlatform();
    if (Platform)
    {
        Platform->ShowPlatformDialog(Title, Message);
    }
}

void UMingPlatformManager::ShareContent(const FString& Content)
{
    IPlatformInterface* Platform = GetCurrentPlatform();
    if (Platform)
    {
        Platform->ShareContent(Content);
    }
}

void UMingPlatformManager::RequestAppRating()
{
    IPlatformInterface* Platform = GetCurrentPlatform();
    if (Platform)
    {
        Platform->RequestAppRating();
    }
}

UMingPlatformManager* UMingPlatformManager::Get()
{
    return Instance;
}
