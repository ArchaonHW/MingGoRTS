#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSSmartAPIActivator.h"
#include "MingRTSQuickStart.generated.h"

/**
 * 智慧API快速開始 - 最簡單的使用方式
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSQuickStart : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSQuickStart();

    // 🚀 一鍵開始
    UFUNCTION(BlueprintCallable, Category = "Quick Start", meta = (DisplayName = "一鍵啟用智慧API"))
    static void EnableSmartAPI();

    // ⚡ 快速操作
    UFUNCTION(BlueprintCallable, Category = "Quick Start", meta = (DisplayName = "快速編譯"))
    static void QuickCompile();

    UFUNCTION(BlueprintCallable, Category = "Quick Start", meta = (DisplayName = "快速除錯"))
    static void QuickDebug();

    UFUNCTION(BlueprintCallable, Category = "Quick Start", meta = (DisplayName = "快速優化"))
    static void QuickOptimize();

    UFUNCTION(BlueprintCallable, Category = "Quick Start", meta = (DisplayName = "檢查代碼品質"))
    static void QuickCodeCheck();

    // 📊 狀態檢查
    UFUNCTION(BlueprintCallable, Category = "Quick Start", meta = (DisplayName = "系統狀態"))
    static TArray<FString> GetSystemStatus();

    UFUNCTION(BlueprintCallable, Category = "Quick Start", meta = (DisplayName = "性能指標"))
    static TMap<FString, float> GetPerformanceMetrics();

    // 🎯 智慧任務
    UFUNCTION(BlueprintCallable, Category = "Quick Start", meta = (DisplayName = "智慧編譯優化"))
    static void SmartCompileAndOptimize();

    UFUNCTION(BlueprintCallable, Category = "Quick Start", meta = (DisplayName = "智慧除錯修復"))
    static void SmartDebugAndFix();

    UFUNCTION(BlueprintCallable, Category = "Quick Start", meta = (DisplayName = "智慧分析改進"))
    static void SmartAnalyzeAndImprove();

private:
    static UPROPERTY()
    TObjectPtr<UMingRTSSmartAPIActivator> GlobalActivator;

    static void InitializeGlobalActivator();
    static bool IsGlobalActivatorValid();
};

/*
🚀 最簡單的使用方式：

### 1. 一鍵啟用
```cpp
UMingRTSQuickStart::EnableSmartAPI();
```

### 2. 快速操作
```cpp
UMingRTSQuickStart::QuickCompile();        // 快速編譯
UMingRTSQuickStart::QuickDebug();          // 快速除錯
UMingRTSQuickStart::QuickOptimize();        // 快速優化
UMingRTSQuickStart::QuickCodeCheck();       // 檢查代碼品質
```

### 3. 智慧任務
```cpp
UMingRTSQuickStart::SmartCompileAndOptimize();  // 智慧編譯優化
UMingRTSQuickStart::SmartDebugAndFix();         // 智慧除錯修復
UMingRTSQuickStart::SmartAnalyzeAndImprove();    // 智慧分析改進
```

### 4. 狀態檢查
```cpp
TArray<FString> Status = UMingRTSQuickStart::GetSystemStatus();
TMap<FString, float> Metrics = UMingRTSQuickStart::GetPerformanceMetrics();
```

### Blueprint中使用：
1. 拖拽 "Quick Start" 節點到藍圖
2. 選擇所需的功能
3. 連接事件處理器
4. 運行遊戲即可使用

✨ 就這麼簡單！智慧API系統已準備就緒！
*/
