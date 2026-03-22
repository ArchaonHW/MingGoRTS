# 🚀 MingGoRTS 一鍵啟動系統

## ⚡ 最簡單的使用方式

### 🎯 一鍵啟動 - 只需一行代碼

```cpp
// 在您的遊戲模式中，只需一行代碼即可啟動整個優化系統
void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // 🚀 一鍵啟動優化編譯系統
    UMingRTSOneClickLauncher::LaunchOptimizationSystem();
}
```

## 🔧 多種啟動模式

### 1. 🚀 靜態一鍵啟動（推薦）
```cpp
// 最簡單的方式 - 自動配置並啟動
UMingRTSOneClickLauncher* Launcher = UMingRTSOneClickLauncher::LaunchOptimizationSystem();
```

### 2. ⚡ 快速啟動
```cpp
// 創建實例並快速啟動
UMingRTSOneClickLauncher* Launcher = NewObject<UMingRTSOneClickLauncher>();
Launcher->QuickLaunchAndOptimize();
```

### 3. 🎯 智能啟動
```cpp
// 根據系統資源自動選擇最佳配置
UMingRTSOneClickLauncher* Launcher = NewObject<UMingRTSOneClickLauncher>();
Launcher->SmartLaunch();
```

### 4. 🔥 極致啟動
```cpp
// 最高性能配置，適合高端系統
UMingRTSOneClickLauncher* Launcher = NewObject<UMingRTSOneClickLauncher>();
Launcher->ExtremeLaunch();
```

## 📊 系統狀態監控

### 獲取啟動狀態
```cpp
FString Status = Launcher->GetLaunchStatus();
UE_LOG(LogTemp, Log, TEXT("%s"), *Status);
```

### 輸出示例：
```
🚀 MingGoRTS 一鍵啟動系統狀態

系統狀態: 🟢 已啟動
優化狀態: 🔄 運行中
運行時間: 00:05:23
系統就緒: ✅ 就緒
監控指標: 8個

📊 實時性能:
  🟢 CPU: 良好
  🟡 內存: 一般
  🟢 GPU: 優秀
  🟢 渲染: 良好
  🟢 音頻: 優秀
  🟢 物理: 良好
  🟢 網絡: 優秀
  🟢 磁盤: 良好

💚 系統健康度: 85.0% 🟢 優秀
```

### 獲取性能報告
```cpp
FString Report = Launcher->GetPerformanceReport();
UE_LOG(LogTemp, Log, TEXT("性能報告: %s"), *Report);
```

### 系統健康度檢查
```cpp
float Health = Launcher->GetSystemHealth();
if (Health >= 0.8f)
{
    UE_LOG(LogTemp, Log, TEXT("系統健康度優秀: %.1f%%"), Health * 100.0f);
}
else
{
    UE_LOG(LogTemp, Warning, TEXT("系統健康度需要關注: %.1f%%"), Health * 100.0f);
}
```

## 🎮 完整遊戲集成

### 頭文件
```cpp
// AMyGameMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Launcher/MingRTSOneClickLauncher.h"
#include "MyGameMode.generated.h"

UCLASS()
class MINGGORTS_API AMyGameMode : public AGameModeBase
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    
    UPROPERTY(BlueprintReadOnly, Category = "Optimization")
    TObjectPtr<UMingRTSOneClickLauncher> OptimizationLauncher;

public:
    UFUNCTION(BlueprintCallable, Category = "Optimization", meta = (DisplayName = "🚀 一鍵啟動優化"))
    void LaunchOptimization();
    
    UFUNCTION(BlueprintCallable, Category = "Optimization", meta = (DisplayName = "🎯 性能檢查"))
    void PerformanceCheck();
    
    UFUNCTION(BlueprintCallable, Category = "Optimization", meta = (DisplayName = "📊 獲取系統狀態"))
    FString GetSystemStatus();
};
```

### 實現文件
```cpp
// AMyGameMode.cpp
#include "MyGameMode.h"

void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // 🚀 一鍵啟動 - 自動初始化優化系統
    OptimizationLauncher = UMingRTSOneClickLauncher::LaunchOptimizationSystem();
    
    UE_LOG(LogTemp, Log, TEXT("🎮 MingGoRTS 遊戲模式已啟動優化系統"));
}

void AMyGameMode::LaunchOptimization()
{
    if (!OptimizationLauncher)
    {
        OptimizationLauncher = UMingRTSOneClickLauncher::LaunchOptimizationSystem();
    }
    else
    {
        OptimizationLauncher->RestartSystem();
    }
}

void AMyGameMode::PerformanceCheck()
{
    if (OptimizationLauncher)
    {
        OptimizationLauncher->OneClickPerformanceCheck();
    }
}

FString AMyGameMode::GetSystemStatus()
{
    if (OptimizationLauncher)
    {
        return OptimizationLauncher->GetLaunchStatus();
    }
    return TEXT("優化系統未啟動");
}
```

## 🎯 Blueprint 使用

### 在Blueprint中使用
1. 創建一個新的Blueprint類，繼承自您的遊戲模式
2. 在Event BeginPlay中添加以下節點：
   - 調用 "🚀 一鍵啟動優化" 函數
   - 可選：調用 "🎯 性能檢查" 函數
   - 可選：調用 "📊 獲取系統狀態" 函數

### Blueprint節點說明
- **🚀 一鍵啟動優化**: 自動啟動並配置優化系統
- **⚡ 快速啟動並優化**: 快速啟動模式
- **🎯 智能啟動**: 智能配置模式
- **🔥 極致啟動**: 最高性能模式
- **📊 獲取系統狀態**: 獲取詳細狀態信息
- **🎮 獲取性能報告**: 獲取性能報告
- **🔄 重新啟動系統**: 重新啟動優化系統
- **⏹️ 停止優化系統**: 停止優化系統

## 🔍 實時監控面板

### 在HUD中顯示性能信息
```cpp
// AMyHUD.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

UCLASS()
class MINGGORTS_API AMyHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    bool bShowPerformanceInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FVector2D PerformanceInfoPosition;
};

// AMyHUD.cpp
#include "MyHUD.h"
#include "MyGameMode.h"

void AMyHUD::DrawHUD()
{
    Super::DrawHUD();
    
    if (bShowPerformanceInfo)
    {
        AMyGameMode* GameMode = Cast<AMyGameMode>(GetWorld()->GetAuthGameMode());
        if (GameMode && GameMode->OptimizationLauncher)
        {
            FString Status = GameMode->OptimizationLauncher->GetLaunchStatus();
            
            // 在屏幕上顯示性能狀態
            DrawText(Status, FColor::White, PerformanceInfoPosition.X, PerformanceInfoPosition.Y, nullptr, 0.8f);
        }
    }
}
```

## ⚡ 高級功能

### 自動性能檢查
```cpp
// 在遊戲循環中定期檢查性能
void AMyGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // 每60秒檢查一次性能
    static float LastCheckTime = 0.0f;
    LastCheckTime += DeltaTime;
    
    if (LastCheckTime >= 60.0f)
    {
        LastCheckTime = 0.0f;
        
        if (OptimizationLauncher)
        {
            float Health = OptimizationLauncher->GetSystemHealth();
            if (Health < 0.6f)
            {
                UE_LOG(LogTemp, Warning, TEXT("系統健康度較低 (%.1f%%)，建議優化"), Health * 100.0f);
                OptimizationLauncher->OneClickPerformanceCheck();
            }
        }
    }
}
```

### 自動重啟系統
```cpp
// 系統健康度過低時自動重啟
void AMyGameMode::HandleLowSystemHealth()
{
    if (OptimizationLauncher)
    {
        float Health = OptimizationLauncher->GetSystemHealth();
        if (Health < 0.4f)
        {
            UE_LOG(LogTemp, Warning, TEXT("系統健康度過低 (%.1f%%)，自動重啟"), Health * 100.0f);
            OptimizationLauncher->RestartSystem();
        }
    }
}
```

## 🎯 使用建議

### 開發階段
```cpp
// 使用智能啟動，平衡性能和開發效率
OptimizationLauncher->SmartLaunch();
```

### 測試階段
```cpp
// 使用快速啟動，快速編譯和基礎優化
OptimizationLauncher->QuickLaunchAndOptimize();
```

### 發布階段
```cpp
// 使用極致啟動，獲得最佳性能
OptimizationLauncher->ExtremeLaunch();
```

## 🚨 故障排除

### 系統未啟動
```cpp
if (!OptimizationLauncher || OptimizationLauncher->GetSystemHealth() == 0.0f)
{
    UE_LOG(LogTemp, Error, TEXT("優化系統未啟動，嘗試重新啟動"));
    OptimizationLauncher = UMingRTSOneClickLauncher::LaunchOptimizationSystem();
}
```

### 性能較差
```cpp
float Health = OptimizationLauncher->GetSystemHealth();
if (Health < 0.6f)
{
    UE_LOG(LogTemp, Warning, TEXT("系統性能較差，執行性能檢查"));
    OptimizationLauncher->OneClickPerformanceCheck();
}
```

## 📋 快速檢查清單

啟動後確認：
- ✅ 系統狀態顯示 "已啟動"
- ✅ 優化狀態顯示 "運行中"
- ✅ 系統健康度 > 60%
- ✅ 性能監控正常工作
- ✅ 無錯誤信息

## 🎉 立即開始

**現在就使用一鍵啟動系統！**

1. 複製上面的代碼到您的項目
2. 在您的遊戲模式中添加一行代碼
3. 編譯並運行
4. 享受自動化的優化體驗！

**🚀 您的MingGoRTS項目現在將以最佳性能自動運行！** ⚡✨🎯

---

## 📞 技術支持

如果遇到問題：
1. 檢查系統資源是否充足
2. 確認UE5版本兼容性
3. 查看控制台錯誤信息
4. 參考故障排除部分

**一鍵啟動，讓優化變得前所未有的簡單！** 🎯⚡
