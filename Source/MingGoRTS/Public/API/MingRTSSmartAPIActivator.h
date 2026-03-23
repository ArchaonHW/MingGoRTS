#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSAPISystem.h"
#include "MingRTSSmartAPIActivator.generated.h"

/**
 * 智慧API啟用器 - 一鍵啟用整個智慧API系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSSmartAPIActivator : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSSmartAPIActivator();

    // 主要啟用功能
    UFUNCTION(BlueprintCallable, Category = "Smart API Activator")
    void EnableSmartAPISystem();

    UFUNCTION(BlueprintCallable, Category = "Smart API Activator")
    void DisableSmartAPISystem();

    UFUNCTION(BlueprintCallable, Category = "Smart API Activator")
    bool IsSmartAPISystemEnabled();

    // 智慧功能配置
    UFUNCTION(BlueprintCallable, Category = "Smart API Activator|Configuration")
    void ConfigureIntelligentMode(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Smart API Activator|Configuration")
    void SetAutoLearning(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Smart API Activator|Configuration")
    void EnablePredictiveFeatures(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Smart API Activator|Configuration")
    void SetCoordinationMode(const FString& Mode);

    // 一鍵智慧操作
    UFUNCTION(BlueprintCallable, Category = "Smart API Activator|Quick Actions")
    void QuickCompileAndOptimize();

    UFUNCTION(BlueprintCallable, Category = "Smart API Activator|Quick Actions")
    void QuickDebugAndFix();

    UFUNCTION(BlueprintCallable, Category = "Smart API Activator|Quick Actions")
    void QuickAnalyzeAndImprove();

    UFUNCTION(BlueprintCallable, Category = "Smart API Activator|Quick Actions")
    void QuickHealthCheck();

    // 智慧工作流
    UFUNCTION(BlueprintCallable, Category = "Smart API Activator|Workflows")
    void StartIntelligentCompilation();

    UFUNCTION(BlueprintCallable, Category = "Smart API Activator|Workflows")
    void StartPerformanceOptimization();

    UFUNCTION(BlueprintCallable, Category = "Smart API Activator|Workflows")
    void StartCodeQualityImprovement();

    UFUNCTION(BlueprintCallable, Category = "Smart API Activator|Workflows")
    void StartSystemMonitoring();

    // 系統狀態
    UFUNCTION(BlueprintCallable, Category = "Smart API Activator|Status")
    TArray<FString> GetSystemStatus();

    UFUNCTION(BlueprintCallable, Category = "Smart API Activator|Status")
    TMap<FString, float> GetPerformanceMetrics();

    UFUNCTION(BlueprintCallable, Category = "Smart API Activator|Status")
    TArray<FString> GetActiveFeatures();

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSmartSystemEnabled, bool, bEnabled);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntelligentActionCompleted, const FString&, ActionName);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSystemHealthUpdated, const TArray<FString>&, HealthStatus);

    UPROPERTY(BlueprintAssignable)
    FOnSmartSystemEnabled OnSmartSystemEnabled;

    UPROPERTY(BlueprintAssignable)
    FOnIntelligentActionCompleted OnIntelligentActionCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnSystemHealthUpdated OnSystemHealthUpdated;

private:
    UPROPERTY()
    TObjectPtr<UMingRTSAPISystem> APISystem;

    UPROPERTY()
    bool bSystemEnabled;

    UPROPERTY()
    bool bIntelligentModeEnabled;

    UPROPERTY()
    bool bAutoLearningEnabled;

    UPROPERTY()
    bool bPredictiveFeaturesEnabled;

    UPROPERTY()
    FString CoordinationMode;

    // 內部方法
    void InitializeSmartFeatures();
    void SetupIntelligentWorkflows();
    void StartBackgroundMonitoring();
    void ConfigureAPICoordination();
    void EnableCrossAPICommunication();
    void InitializeLearningSystems();
    void StartPredictiveAnalysis();
};

/*
智慧API啟用器使用指南：

1. 基礎啟用：
```cpp
UMingRTSSmartAPIActivator* Activator = NewObject<UMingRTSSmartAPIActivator>();
Activator->EnableSmartAPISystem();
```

2. 智慧功能配置：
```cpp
Activator->ConfigureIntelligentMode(true);
Activator->SetAutoLearning(true);
Activator->EnablePredictiveFeatures(true);
```

3. 一鍵智慧操作：
```cpp
Activator->QuickCompileAndOptimize();  // 編譯並優化
Activator->QuickDebugAndFix();         // 除錯並修復
Activator->QuickAnalyzeAndImprove();    // 分析並改進
Activator->QuickHealthCheck();          // 健康檢查
```

4. 智慧工作流：
```cpp
Activator->StartIntelligentCompilation();     // 智慧編譯
Activator->StartPerformanceOptimization();    // 性能優化
Activator->StartCodeQualityImprovement();    // 代碼品質改進
Activator->StartSystemMonitoring();          // 系統監控
```

系統特性：
- 🧠 智慧決策 - 自動選擇最佳API組合
- 🔗 跨API協調 - 無縫API間通信
- 📚 自我學習 - 持續改進性能
- 🔮 預測分析 - 預見系統需求
- ⚡ 自動化 - 一鍵執行複雜任務
- 📊 實時監控 - 全方位系統監控

智慧工作流示例：
1. 智慧編譯：分析代碼 → 應用標準 → 編譯 → 檢測錯誤 → 自動修復 → 優化
2. 性能優化：監控性能 → 識別瓶頸 → 分析原因 → 應用優化 → 驗證效果
3. 代碼品質：檢查規範 → 識別問題 → 建議改進 → 自動修復 → 更新標準
4. 系統監控：收集指標 → 分析趨勢 → 預測問題 → 主動預防 → 持續改進
*/
