#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSAPISystem.h"
#include "MingRTSAPIExample.generated.h"

/**
 * API使用示例 - 展示如何使用智慧API系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSAPIExample : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSAPIExample();

    // 初始化API系統
    UFUNCTION(BlueprintCallable, Category = "API Example")
    void InitializeAPIs();

    // 編譯示例
    UFUNCTION(BlueprintCallable, Category = "API Example")
    void CompileProjectExample();

    UFUNCTION(BlueprintCallable, Category = "API Example")
    void CompileWithErrorHandlingExample();

    // 日誌分析示例
    UFUNCTION(BlueprintCallable, Category = "API Example")
    void AnalyzeLogsExample();

    UFUNCTION(BlueprintCallable, Category = "API Example")
    void DetectAnomaliesExample();

    // 除錯示例
    UFUNCTION(BlueprintCallable, Category = "API Example")
    void StartDebugSessionExample();

    UFUNCTION(BlueprintCallable, Category = "API Example")
    void SmartBreakpointExample();

    // 優化示例
    UFUNCTION(BlueprintCallable, Category = "API Example")
    void OptimizePerformanceExample();

    UFUNCTION(BlueprintCallable, Category = "API Example")
    void PredictiveOptimizationExample();

    // 智慧面板示例
    UFUNCTION(BlueprintCallable, Category = "API Example")
    void CreateDashboardExample();

    UFUNCTION(BlueprintCallable, Category = "API Example")
    void SmartAlertsExample();

    // 跨API協調示例
    UFUNCTION(BlueprintCallable, Category = "API Example")
    void IntelligentTaskCoordinationExample();

    UFUNCTION(BlueprintCallable, Category = "API Example")
    void AutomatedWorkflowExample();

    // 自我學習示例
    UFUNCTION(BlueprintCallable, Category = "API Example")
    void LearningFromErrorsExample();

    UFUNCTION(BlueprintCallable, Category = "API Example")
    void AdaptiveOptimizationExample();

private:
    UPROPERTY()
    TObjectPtr<UMingRTSAPISystem> APISystem;

    // 事件處理
    UFUNCTION()
    void OnCompilationCompleted(const FCompilationResult& Result);

    UFUNCTION()
    void OnLogAnalysisCompleted(const FLogAnalysisResult& Analysis);

    UFUNCTION()
    void OnDebugAnalysisCompleted(const FDebugAnalysisResult& Analysis);

    UFUNCTION()
    void OnOptimizationCompleted(const FOptimizationResult& Result);

    UFUNCTION()
    void OnPanelDataUpdated(const FString& PanelId, const FString& WidgetId, const FPanelData& Data);

    UFUNCTION()
    void OnAlertTriggered(const FPanelAlert& Alert);

    UFUNCTION()
    void OnCoordinationCompleted(const FAPICoordinationResult& Result);
};

/*
使用示例說明：

1. 基礎初始化：
```cpp
UMingRTSAPIExample* Example = NewObject<UMingRTSAPIExample>();
Example->InitializeAPIs();
```

2. 編譯並自動處理錯誤：
```cpp
Example->CompileWithErrorHandlingExample();
```

3. 智慧任務協調：
```cpp
Example->IntelligentTaskCoordinationExample();
```

4. 自動化工作流：
```cpp
Example->AutomatedWorkflowExample();
```

5. 自我學習：
```cpp
Example->LearningFromErrorsExample();
Example->AdaptiveOptimizationExample();
```

系統特性：
- 每個API都具備自我學習能力
- API間可以互相調用和協調
- 智慧決策和預測功能
- 自動化工作流執行
- 實時監控和警報
- 性能優化和適應

跨API調用場景：
1. 編譯失敗 → 自動觸發日誌分析 → 啟動除錯會話 → 應用修復
2. 性能問題 → 分析日誌 → 識別瓶頸 → 自動優化 → 更新面板
3. 錯誤檢測 → 記錄模式 → 學習修復方案 → 預防未來錯誤
4. 用戶交互 → 學習偏好 → 優化界面 → 個人化體驗

自我學習機制：
- 錯誤模式識別和修復
- 性能趨勢分析和預測
- 用戶行為學習和適應
- API協調優化
- 智慧決策改進
*/
