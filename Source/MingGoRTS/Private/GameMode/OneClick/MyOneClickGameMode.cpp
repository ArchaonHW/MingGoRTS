#include "MyOneClickGameMode.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/HUD.h"
#include "Components/TextRenderComponent.h"

AMyOneClickGameMode::AMyOneClickGameMode()
{
    bAutoLaunchOnBeginPlay = true;
    bShowPerformanceInfo = true;
    bAutoPerformanceCheck = true;
    PerformanceCheckInterval = 60.0f; // 60秒檢查一次
    bEnableNotifications = true;
    
    LastPerformanceCheck = 0.0f;
    LastDisplayUpdate = 0.0f;
    bPerformanceDisplayActive = false;
    bSystemInitialized = false;
}

void AMyOneClickGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Log, TEXT("🎮 MingGoRTS 一鍵啟動遊戲模式開始"));
    
    // 初始化一鍵系統
    InitializeOneClickSystem();
    
    // 自動啟動優化系統
    if (bAutoLaunchOnBeginPlay)
    {
        LaunchOptimizationSystem();
    }
    
    // 設置自動性能檢查
    if (bAutoPerformanceCheck)
    {
        SetupAutoPerformanceCheck();
    }
    
    // 顯示啟動信息
    if (bEnableNotifications)
    {
        SendPerformanceNotification();
    }
    
    bSystemInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("✅ 一鍵啟動遊戲模式初始化完成"));
}

void AMyOneClickGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!bSystemInitialized)
    {
        return;
    }
    
    // 自動性能檢查
    if (bAutoPerformanceCheck && OneClickLauncher.IsValid())
    {
        LastPerformanceCheck += DeltaTime;
        if (LastPerformanceCheck >= PerformanceCheckInterval)
        {
            LastPerformanceCheck = 0.0f;
            
            float Health = OneClickLauncher->GetSystemHealth();
            if (Health < 0.6f)
            {
                UE_LOG(LogTemp, Warning, TEXT("⚠️ 系統健康度較低 (%.1f%%)，執行性能檢查"), Health * 100.0f);
                OneClickPerformanceCheck();
            }
        }
    }
    
    // 更新性能顯示
    if (bShowPerformanceInfo && bPerformanceDisplayActive)
    {
        LastDisplayUpdate += DeltaTime;
        if (LastDisplayUpdate >= 1.0f) // 每秒更新一次
        {
            LastDisplayUpdate = 0.0f;
            UpdatePerformanceDisplay();
        }
    }
    
    // 處理性能警告
    HandlePerformanceWarnings();
}

void AMyOneClickGameMode::LaunchOptimizationSystem()
{
    UE_LOG(LogTemp, Log, TEXT("🚀 一鍵啟動優化系統..."));
    
    if (OneClickLauncher.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("優化系統已經啟動"));
        return;
    }
    
    // 使用靜態方法一鍵啟動
    OneClickLauncher = UMingRTSOneClickLauncher::LaunchOptimizationSystem();
    
    if (OneClickLauncher.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("✅ 一鍵啟動成功！"));
        
        // 發送通知
        if (bEnableNotifications)
        {
            SendPerformanceNotification();
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ 一鍵啟動失敗"));
    }
}

void AMyOneClickGameMode::SmartLaunchSystem()
{
    UE_LOG(LogTemp, Log, TEXT("🎯 智能啟動系統..."));
    
    if (!OneClickLauncher.IsValid())
    {
        OneClickLauncher = NewObject<UMingRTSOneClickLauncher>();
    }
    
    OneClickLauncher->SmartLaunch();
    
    UE_LOG(LogTemp, Log, TEXT("✅ 智能啟動完成！"));
}

void AMyOneClickGameMode::ExtremeLaunchSystem()
{
    UE_LOG(LogTemp, Log, TEXT("🔥 極致啟動系統..."));
    
    if (!OneClickLauncher.IsValid())
    {
        OneClickLauncher = NewObject<UMingRTSOneClickLauncher>();
    }
    
    OneClickLauncher->ExtremeLaunch();
    
    UE_LOG(LogTemp, Log, TEXT("✅ 極致啟動完成！"));
}

FString AMyOneClickGameMode::GetSystemStatus()
{
    if (!OneClickLauncher.IsValid())
    {
        return TEXT("❌ 優化系統未啟動");
    }
    
    return OneClickLauncher->GetLaunchStatus();
}

FString AMyOneClickGameMode::GetPerformanceReport()
{
    if (!OneClickLauncher.IsValid())
    {
        return TEXT("❌ 優化系統未啟動");
    }
    
    return OneClickLauncher->GetPerformanceReport();
}

void AMyOneClickGameMode::RestartOptimizationSystem()
{
    UE_LOG(LogTemp, Log, TEXT("🔄 重新啟動優化系統..."));
    
    if (OneClickLauncher.IsValid())
    {
        OneClickLauncher->RestartSystem();
        UE_LOG(LogTemp, Log, TEXT("✅ 系統重新啟動完成"));
    }
    else
    {
        LaunchOptimizationSystem();
    }
}

void AMyOneClickGameMode::OneClickPerformanceCheck()
{
    UE_LOG(LogTemp, Log, TEXT("🎯 執行一鍵性能檢查..."));
    
    if (!OneClickLauncher.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("❌ 優化系統未啟動"));
        return;
    }
    
    OneClickLauncher->OneClickPerformanceCheck();
    
    // 發送檢查結果通知
    if (bEnableNotifications)
    {
        SendPerformanceNotification();
    }
}

void AMyOneClickGameMode::StopOptimizationSystem()
{
    UE_LOG(LogTemp, Log, TEXT("⏹️ 停止優化系統..."));
    
    if (OneClickLauncher.IsValid())
    {
        OneClickLauncher->StopSystem();
        UE_LOG(LogTemp, Log, TEXT("✅ 優化系統已停止"));
    }
}

float AMyOneClickGameMode::GetSystemHealth()
{
    if (!OneClickLauncher.IsValid())
    {
        return 0.0f;
    }
    
    return OneClickLauncher->GetSystemHealth();
}

void AMyOneClickGameMode::TogglePerformanceDisplay()
{
    bPerformanceDisplayActive = !bPerformanceDisplayActive;
    
    UE_LOG(LogTemp, Log, TEXT("🎨 性能顯示 %s"), bPerformanceDisplayActive ? TEXT("已開啟") : TEXT("已關閉"));
    
    if (bPerformanceDisplayActive)
    {
        UpdatePerformanceDisplay();
    }
}

void AMyOneClickGameMode::SendPerformanceNotification()
{
    if (!OneClickLauncher.IsValid())
    {
        return;
    }
    
    float Health = OneClickLauncher->GetSystemHealth();
    FString HealthStatus = TEXT("未知");
    
    if (Health >= 0.8f)
    {
        HealthStatus = TEXT("🟢 優秀");
    }
    else if (Health >= 0.6f)
    {
        HealthStatus = TEXT("🟡 良好");
    }
    else if (Health >= 0.4f)
    {
        HealthStatus = TEXT("🟠 一般");
    }
    else
    {
        HealthStatus = TEXT("🔴 較差");
    }
    
    FString Notification = FString::Printf(TEXT("🚀 MingGoRTS 優化系統\n💚 系統健康度: %.1f%% %s"), 
                                           Health * 100.0f, *HealthStatus);
    
    // 在屏幕上顯示通知
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        // 這裡可以添加UI通知或屏幕打印
        UE_LOG(LogTemp, Log, TEXT("📱 通知: %s"), *Notification);
    }
}

// 私有方法實現

void AMyOneClickGameMode::InitializeOneClickSystem()
{
    UE_LOG(LogTemp, Log, TEXT("初始化一鍵系統..."));
    
    // 一鍵啟動器將在需要時創建
    // 這樣可以確保系統按需啟動，節省資源
}

void AMyOneClickGameMode::SetupAutoPerformanceCheck()
{
    UE_LOG(LogTemp, Log, TEXT("設置自動性能檢查，間隔: %.1f秒"), PerformanceCheckInterval);
    LastPerformanceCheck = 0.0f;
}

void AMyOneClickGameMode::HandlePerformanceWarnings()
{
    if (!OneClickLauncher.IsValid())
    {
        return;
    }
    
    float Health = OneClickLauncher->GetSystemHealth();
    
    // 如果健康度過低，發送警告
    if (Health < 0.3f && bEnableNotifications)
    {
        static float LastWarningTime = 0.0f;
        static float WarningInterval = 30.0f; // 30秒警告一次
        
        if (GetWorld()->GetTimeSeconds() - LastWarningTime > WarningInterval)
        {
            LastWarningTime = GetWorld()->GetTimeSeconds();
            
            UE_LOG(LogTemp, Error, TEXT("🚨 系統健康度嚴重不足 (%.1f%%)，建議立即優化！"), Health * 100.0f);
            
            // 可以在這裡添加屏幕警告或UI通知
            FString WarningMessage = FString::Printf(TEXT("🚨 警告：系統健康度 %.1f%%，建議優化"), Health * 100.0f);
            
            if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
            {
                // 顯示警告消息
                PC->ClientMessage(WarningMessage);
            }
        }
    }
}

void AMyOneClickGameMode::UpdatePerformanceDisplay()
{
    if (!OneClickLauncher.IsValid() || !bPerformanceDisplayActive)
    {
        return;
    }
    
    FString Status = OneClickLauncher->GetLaunchStatus();
    
    // 這裡可以添加在屏幕上顯示性能信息的邏輯
    // 例如：更新HUD、創建UI元素等
    
    // 為了演示，我們只打印到日誌
    static int32 LogCounter = 0;
    LogCounter++;
    
    if (LogCounter % 60 == 0) // 每60秒打印一次完整狀態
    {
        UE_LOG(LogTemp, Log, TEXT("📊 性能狀態更新:\n%s"), *Status);
    }
}

// Blueprint可調用的輔助方法

UFUNCTION(BlueprintCallable, Category = "One Click Game Mode")
void AMyOneClickGameMode::SetAutoLaunch(bool bEnabled)
{
    bAutoLaunchOnBeginPlay = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("設置自動啟動: %s"), bEnabled ? TEXT("啟用") : TEXT("禁用"));
}

UFUNCTION(BlueprintCallable, Category = "One Click Game Mode")
void AMyOneClickGameMode::SetPerformanceCheckInterval(float Interval)
{
    PerformanceCheckInterval = FMath::Max(10.0f, Interval);
    UE_LOG(LogTemp, Log, TEXT("設置性能檢查間隔: %.1f秒"), PerformanceCheckInterval);
}

UFUNCTION(BlueprintCallable, Category = "One Click Game Mode")
void AMyOneClickGameMode::SetNotificationsEnabled(bool bEnabled)
{
    bEnableNotifications = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("設置通知: %s"), bEnabled ? TEXT("啟用") : TEXT("禁用"));
}

UFUNCTION(BlueprintCallable, Category = "One Click Game Mode")
bool AMyOneClickGameMode::IsSystemHealthy() const
{
    if (!OneClickLauncher.IsValid())
    {
        return false;
    }
    
    return OneClickLauncher->GetSystemHealth() >= 0.6f;
}

UFUNCTION(BlueprintCallable, Category = "One Click Game Mode")
FString GetOptimizationMode() const
{
    if (!OneClickLauncher.IsValid())
    {
        return TEXT("未啟動");
    }
    
    // 這裡可以從啟動器獲取當前模式信息
    return TEXT("運行中");
}
