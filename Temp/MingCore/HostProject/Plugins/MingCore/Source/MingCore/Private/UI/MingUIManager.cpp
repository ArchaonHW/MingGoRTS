#include "UI/MingUIManager.h"
#include "Platform/MingPlatformManager.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Engine.h"

// 靜態實例初始化
UMingUIManager* UMingUIManager::Instance = nullptr;

UMingUIManager::UMingUIManager()
    : bInitialized(false)
    , CurrentLayoutType(EUILayoutType::Desktop)
    , LastScreenSize(FVector2D::ZeroVector)
{
}

void UMingUIManager::Initialize()
{
    if (bInitialized)
    {
        return;
    }
    
    // 初始化屏幕尺寸定義
    InitializeScreenSizeDefinitions();
    
    // 獲取當前屏幕尺寸
    if (GEngine && GEngine->GameViewport)
    {
        FIntPoint ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
        LastScreenSize = FVector2D(ViewportSize.X, ViewportSize.Y);
        
        UE_LOG(LogTemp, Log, TEXT("UIManager initialized, screen size: %f x %f"),
            LastScreenSize.X, LastScreenSize.Y);
    }
    
    // 初始化 UI 布局
    UpdateUILayout();
    
    Instance = this;
    bInitialized = true;
}

void UMingUIManager::Shutdown()
{
    if (!bInitialized)
    {
        return;
    }
    
    ScreenSizeChangedCallbacks.Empty();
    ScreenSizeDefinitions.Empty();
    
    Instance = nullptr;
    bInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("UIManager shutdown"));
}

void UMingUIManager::Tick(float DeltaTime)
{
    if (!bInitialized)
    {
        return;
    }
    
    // 檢測屏幕尺寸變化
    CheckScreenSizeChange();
}

float UMingUIManager::GetScaledSize(float BaseSize) const
{
    return BaseSize * CurrentScaleSettings.GlobalUIScale;
}

float UMingUIManager::GetMinTouchTargetSize() const
{
    return CurrentSizeCategory.MinTouchTargetSize;
}

bool UMingUIManager::NeedsSafeZonePadding() const
{
    UMingPlatformManager* PlatformManager = UMingPlatformManager::Get();
    if (PlatformManager)
    {
        FMargin SafeZone = PlatformManager->GetSafeZone();
        return SafeZone.Top > 0 || SafeZone.Bottom > 0 || 
               SafeZone.Left > 0 || SafeZone.Right > 0;
    }
    return false;
}

FMargin UMingUIManager::GetSafeZonePadding() const
{
    UMingPlatformManager* PlatformManager = UMingPlatformManager::Get();
    if (PlatformManager)
    {
        return PlatformManager->GetSafeZone();
    }
    return FMargin(0, 0, 0, 0);
}

void UMingUIManager::RegisterScreenSizeChangedCallback(UObject* Listener, FName FunctionName)
{
    if (!Listener || FunctionName.IsNone())
    {
        return;
    }
    
    ScreenSizeChangedCallbacks.Add(Listener, FunctionName);
}

void UMingUIManager::UnregisterScreenSizeChangedCallback(UObject* Listener)
{
    ScreenSizeChangedCallbacks.Remove(Listener);
}

void UMingUIManager::InitializeScreenSizeDefinitions()
{
    ScreenSizeDefinitions.Empty();
    
    // 桌面布局 (>= 1024px 寬度)
    {
        FScreenSizeCategory Desktop;
        Desktop.MinWidth = 1024;
        Desktop.MaxWidth = -1;
        Desktop.MinHeight = 768;
        Desktop.MaxHeight = -1;
        Desktop.LayoutType = EUILayoutType::Desktop;
        Desktop.MinTouchTargetSize = 32.0f; // 桌面可以使用較小的觸控目標
        ScreenSizeDefinitions.Add(Desktop);
    }
    
    // 平板布局 (768px - 1024px 寬度)
    {
        FScreenSizeCategory Tablet;
        Tablet.MinWidth = 768;
        Tablet.MaxWidth = 1023;
        Tablet.MinHeight = 600;
        Tablet.MaxHeight = -1;
        Tablet.LayoutType = EUILayoutType::Tablet;
        Tablet.MinTouchTargetSize = 40.0f;
        ScreenSizeDefinitions.Add(Tablet);
    }
    
    // 手機橫屏 (480px - 768px 寬度)
    {
        FScreenSizeCategory PhoneLandscape;
        PhoneLandscape.MinWidth = 480;
        PhoneLandscape.MaxWidth = 767;
        PhoneLandscape.MinHeight = 320;
        PhoneLandscape.MaxHeight = -1;
        PhoneLandscape.LayoutType = EUILayoutType::PhoneLandscape;
        PhoneLandscape.MinTouchTargetSize = 44.0f;
        ScreenSizeDefinitions.Add(PhoneLandscape);
    }
    
    // 手機豎屏 (< 480px 寬度)
    {
        FScreenSizeCategory PhonePortrait;
        PhonePortrait.MinWidth = 0;
        PhonePortrait.MaxWidth = 479;
        PhonePortrait.MinHeight = 0;
        PhonePortrait.MaxHeight = -1;
        PhonePortrait.LayoutType = EUILayoutType::PhonePortrait;
        PhonePortrait.MinTouchTargetSize = 44.0f; // iOS Human Interface Guidelines 建議 44pt
        ScreenSizeDefinitions.Add(PhonePortrait);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Initialized %d screen size categories"), ScreenSizeDefinitions.Num());
}

void UMingUIManager::CheckScreenSizeChange()
{
    if (!GEngine || !GEngine->GameViewport)
    {
        return;
    }
    
    FIntPoint ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
    FVector2D CurrentScreenSize(ViewportSize.X, ViewportSize.Y);
    
    // 檢測屏幕尺寸變化（超過 10 像素閾值）
    if (FVector2D::Distance(LastScreenSize, CurrentScreenSize) > 10.0f)
    {
        UE_LOG(LogTemp, Log, TEXT("Screen size changed from %s to %s"),
            *LastScreenSize.ToString(), *CurrentScreenSize.ToString());
        
        LastScreenSize = CurrentScreenSize;
        UpdateUILayout();
        NotifyScreenSizeChanged();
    }
}

void UMingUIManager::UpdateUILayout()
{
    float ScreenWidth = LastScreenSize.X;
    float ScreenHeight = LastScreenSize.Y;
    
    // 根據屏幕尺寸選擇布局
    for (const FScreenSizeCategory& Category : ScreenSizeDefinitions)
    {
        bool bWidthMatch = (ScreenWidth >= Category.MinWidth) && 
                          (Category.MaxWidth < 0 || ScreenWidth <= Category.MaxWidth);
        bool bHeightMatch = (ScreenHeight >= Category.MinHeight) && 
                           (Category.MaxHeight < 0 || ScreenHeight <= Category.MaxHeight);
        
        if (bWidthMatch && bHeightMatch)
        {
            if (CurrentLayoutType != Category.LayoutType)
            {
                EUILayoutType OldLayout = CurrentLayoutType;
                CurrentLayoutType = Category.LayoutType;
                CurrentSizeCategory = Category;
                
                UE_LOG(LogTemp, Log, TEXT("UI layout changed from %s to %s"),
                    *UEnum::GetValueAsString(OldLayout),
                    *UEnum::GetValueAsString(CurrentLayoutType));
            }
            break;
        }
    }
    
    // 計算縮放設置
    CalculateScaleSettings();
}

void UMingUIManager::CalculateScaleSettings()
{
    UMingPlatformManager* PlatformManager = UMingPlatformManager::Get();
    
    // 基礎 DPI 縮放
    float BaseDPI = 96.0f; // 標準 Windows DPI
    float CurrentDPI = BaseDPI;
    
    if (PlatformManager)
    {
        CurrentDPI = PlatformManager->GetScreenDPI();
    }
    
    CurrentScaleSettings.DPIScale = CurrentDPI / BaseDPI;
    
    // 全局 UI 縮放
    switch (CurrentLayoutType)
    {
    case EUILayoutType::Desktop:
        CurrentScaleSettings.GlobalUIScale = 1.0f;
        break;
    case EUILayoutType::Tablet:
        CurrentScaleSettings.GlobalUIScale = 1.2f;
        break;
    case EUILayoutType::PhoneLandscape:
        CurrentScaleSettings.GlobalUIScale = 0.9f;
        break;
    case EUILayoutType::PhonePortrait:
        CurrentScaleSettings.GlobalUIScale = 0.85f;
        break;
    default:
        CurrentScaleSettings.GlobalUIScale = 1.0f;
        break;
    }
    
    // 應用 DPI 縮放到全局縮放
    CurrentScaleSettings.GlobalUIScale *= CurrentScaleSettings.DPIScale;
    
    // 字體縮放通常與全局縮放一致，但可以單獨調整
    CurrentScaleSettings.FontScale = CurrentScaleSettings.GlobalUIScale;
    
    // 圖標縮放
    CurrentScaleSettings.IconScale = CurrentScaleSettings.GlobalUIScale;
    
    UE_LOG(LogTemp, Verbose, TEXT("UI Scale: DPI=%f, Global=%f, Font=%f, Icon=%f"),
        CurrentScaleSettings.DPIScale,
        CurrentScaleSettings.GlobalUIScale,
        CurrentScaleSettings.FontScale,
        CurrentScaleSettings.IconScale);
}

void UMingUIManager::NotifyScreenSizeChanged()
{
    // 通知所有註冊的監聽器
    for (auto It = ScreenSizeChangedCallbacks.CreateIterator(); It; ++It)
    {
        if (It->Key.IsValid())
        {
            UObject* Listener = It->Key.Get();
            FName FunctionName = It->Value;
            
            UFunction* Function = Listener->FindFunction(FunctionName);
            if (Function)
            {
                struct FScreenSizeCallbackParams
                {
                    FVector2D NewScreenSize;
                    EUILayoutType NewLayoutType;
                };
                
                FScreenSizeCallbackParams Params;
                Params.NewScreenSize = LastScreenSize;
                Params.NewLayoutType = CurrentLayoutType;
                
                Listener->ProcessEvent(Function, &Params);
            }
        }
        else
        {
            It.RemoveCurrent();
        }
    }
}

UMingUIManager* UMingUIManager::Get()
{
    return Instance;
}
