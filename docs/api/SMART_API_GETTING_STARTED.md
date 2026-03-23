# MingGoRTS 智慧API系統使用指南

## 🚀 快速開始

### 第一步：包含必要的頭文件
```cpp
#include "MingRTSSmartAPIActivator.h"
#include "MingRTSAPISystem.h"
#include "MingRTSIntelligentAPIEnhancements.h"
```

### 第二步：創建智慧API啟用器
```cpp
// 在您的類中添加成員變數
UPROPERTY()
TObjectPtr<UMingRTSSmartAPIActivator> SmartAPIActivator;

// 初始化
void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // 創建智慧API啟用器
    SmartAPIActivator = NewObject<UMingRTSSmartAPIActivator>();
    SmartAPIActivator->EnableSmartAPISystem();
}
```

### 第三步：配置智慧功能
```cpp
// 配置智慧模式
SmartAPIActivator->ConfigureIntelligentMode(true);
SmartAPIActivator->SetAutoLearning(true);
SmartAPIActivator->EnablePredictiveFeatures(true);
SmartAPIActivator->SetCoordinationMode(TEXT("Intelligent"));
```

## 📋 基礎使用場景

### 1. 智慧編譯
```cpp
// 一鍵編譯並優化
SmartAPIActivator->QuickCompileAndOptimize();

// 或者使用詳細的編譯API
UMingRTSAPISystem* APISystem = SmartAPIActivator->GetAPISystem();
if (APISystem && APISystem->GetCompilationAPI())
{
    FCompilationContext Context;
    Context.ProjectPath = TEXT("c:/HW/MingGoRTS/MingGoRTS.uproject");
    Context.TargetConfiguration = TEXT("Development");
    
    FCompilationResult Result = APISystem->GetCompilationAPI()->CompileProject(Context);
    
    if (Result.bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("編譯成功！耗時: %.2f秒"), Result.CompilationTime);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("編譯失敗: %s"), *Result.ErrorMessage);
    }
}
```

### 2. 智慧除錯
```cpp
// 快速除錯和修復
SmartAPIActivator->QuickDebugAndFix();

// 或者使用詳細的除錯API
if (APISystem && APISystem->GetDebugAPI())
{
    FDebugSession Session = APISystem->GetDebugAPI()->StartDebugSession(EDebugMode::Step);
    
    // 設置斷點
    FDebugBreakpoint Breakpoint;
    Breakpoint.FilePath = TEXT("MyActor.cpp");
    Breakpoint.LineNumber = 42;
    Breakpoint.Type = EBreakpointType::CodeLine;
    Breakpoint.Condition = TEXT("Variable > 100");
    
    APISystem->GetDebugAPI()->SetBreakpoint(Breakpoint);
    
    // 單步執行
    APISystem->GetDebugAPI()->StepOver();
    APISystem->GetDebugAPI()->StepInto();
}
```

### 3. 性能優化
```cpp
// 一鍵性能優化
SmartAPIActivator->StartPerformanceOptimization();

// 或者使用詳細的優化API
if (APISystem && APISystem->GetOptimizationAPI())
{
    FOptimizationProfile Profile;
    Profile.Strategy = EOptimizationStrategy::Adaptive;
    Profile.ProfileName = TEXT("GamePerformance");
    
    FOptimizationResult Result = APISystem->GetOptimizationAPI()->OptimizePerformance(Profile);
    
    if (Result.bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("性能優化完成！提升: %.2f%%"), Result.PerformanceImprovement);
    }
}
```

### 4. 代碼品質檢查
```cpp
// 使用代碼標準API
if (APISystem && APISystem->GetCodeStandardsAPI())
{
    FCodeStandardProfile Profile;
    Profile.ProfileName = TEXT("MingGoRTS_Standards");
    Profile.bStrictMode = true;
    Profile.bAutoFixEnabled = true;
    
    FCodeAnalysisResult Result = APISystem->GetCodeStandardsAPI()->AnalyzeCode(
        TEXT("c:/HW/MingGoRTS/Source/MyClass.cpp"), Profile);
    
    UE_LOG(LogTemp, Log, TEXT("代碼品質: %s, 分數: %.2f"), 
        *UEnum::GetValueAsString(Result.OverallQuality), Result.QualityScore);
    
    // 自動修復
    if (Result.Violations.Num() > 0)
    {
        APISystem->GetCodeStandardsAPI()->AutoFixViolations(
            TEXT("c:/HW/MingGoRTS/Source/MyClass.cpp"), Result.Violations);
    }
}
```

### 5. 智慧面板監控
```cpp
// 創建智慧面板
if (APISystem && APISystem->GetSmartPanelAPI())
{
    FPanelConfiguration PanelConfig;
    PanelConfig.PanelType = EPanelType::Dashboard;
    PanelConfig.PanelName = TEXT("GameMonitor");
    PanelConfig.Mode = EPanelMode::Overview;
    PanelConfig.bAutoRefresh = true;
    PanelConfig.RefreshInterval = TEXT("5s");
    
    FPanelConfiguration CreatedPanel = APISystem->GetSmartPanelAPI()->CreatePanel(
        EPanelType::Dashboard, TEXT("GameMonitor"));
    
    // 添加性能監控組件
    FPanelWidget PerformanceWidget;
    PerformanceWidget.WidgetId = TEXT("PerformanceMetrics");
    PerformanceWidget.WidgetType = TEXT("PerformanceChart");
    PerformanceWidget.Title = TEXT("系統性能");
    PerformanceWidget.bInteractive = true;
    
    APISystem->GetSmartPanelAPI()->AddWidget(CreatedPanel.PanelName, PerformanceWidget);
    
    // 更新面板數據
    FPanelData Data;
    Data.DataSource = TEXT("PerformanceAPI");
    Data.Value = TEXT("85.5");
    Data.Unit = TEXT("FPS");
    Data.Status = TEXT("Good");
    
    APISystem->GetSmartPanelAPI()->UpdatePanelData(
        CreatedPanel.PanelName, PerformanceWidget.WidgetId, Data);
}
```

## 🧠 高級智慧功能

### 1. 跨API協調
```cpp
// 使用智慧協調執行複雜任務
if (APISystem)
{
    // 編譯並優化整個項目
    TArray<EAPIType> APIs = {
        EAPIType::CodeStandards,
        EAPIType::Compilation,
        EAPIType::Log,
        EAPIType::Debug,
        EAPIType::Optimization
    };
    
    FAPICoordinationResult Result = APISystem->CoordinateAPIs(APIs, 
        TEXT("compile_optimize_project"));
    
    if (Result.bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("協調任務完成！耗時: %.2f秒"), Result.TotalTime);
        
        // 查看建議
        for (const FString& Recommendation : Result.Recommendations)
        {
            UE_LOG(LogTemp, Log, TEXT("建議: %s"), *Recommendation);
        }
    }
}
```

### 2. 智慧工作流
```cpp
// 創建自動化工作流
void AMyGameMode::SetupIntelligentWorkflow()
{
    if (SmartAPIActivator)
    {
        // 啟動智慧編譯工作流
        SmartAPIActivator->StartIntelligentCompilation();
        
        // 啟動性能監控
        SmartAPIActivator->StartSystemMonitoring();
        
        // 配置事件處理
        SetupEventHandlers();
    }
}

void AMyGameMode::SetupEventHandlers()
{
    if (SmartAPIActivator)
    {
        // 綁定智慧系統事件
        SmartAPIActivator->OnSmartSystemEnabled.AddDynamic(
            this, &AMyGameMode::OnSmartSystemEnabled);
        
        SmartAPIActivator->OnIntelligentActionCompleted.AddDynamic(
            this, &AMyGameMode::OnIntelligentActionCompleted);
        
        SmartAPIActivator->OnSystemHealthUpdated.AddDynamic(
            this, &AMyGameMode::OnSystemHealthUpdated);
    }
}

UFUNCTION()
void AMyGameMode::OnSmartSystemEnabled(bool bEnabled)
{
    if (bEnabled)
    {
        UE_LOG(LogTemp, Log, TEXT("智慧API系統已啟用"));
        
        // 顯示系統狀態
        TArray<FString> Status = SmartAPIActivator->GetSystemStatus();
        for (const FString& StatusItem : Status)
        {
            UE_LOG(LogTemp, Log, TEXT("狀態: %s"), *StatusItem);
        }
    }
}

UFUNCTION()
void AMyGameMode::OnIntelligentActionCompleted(const FString& ActionName)
{
    UE_LOG(LogTemp, Log, TEXT("智慧操作完成: %s"), *ActionName);
    
    // 根據操作結果執行後續動作
    if (ActionName == TEXT("QuickCompileAndOptimize"))
    {
        // 編譯優化完成後的處理
        OnCompilationOptimized();
    }
}
```

### 3. 預測分析
```cpp
// 使用預測功能
void AMyGameMode::EnablePredictiveFeatures()
{
    if (SmartAPIActivator)
    {
        SmartAPIActivator->EnablePredictiveFeatures(true);
        
        // 獲取性能指標
        TMap<FString, float> Metrics = SmartAPIActivator->GetPerformanceMetrics();
        
        for (const auto& Metric : Metrics)
        {
            UE_LOG(LogTemp, Log, TEXT("指標 %s: %.2f"), 
                *Metric.Key, Metric.Value);
        }
    }
}
```

## 🎯 實際應用場景

### 場景1：開發環境設置
```cpp
void AMyGameMode::SetupDevelopmentEnvironment()
{
    // 創建智慧API啟用器
    SmartAPIActivator = NewObject<UMingRTSSmartAPIActivator>();
    SmartAPIActivator->EnableSmartAPISystem();
    
    // 配置開發模式
    SmartAPIActivator->ConfigureIntelligentMode(true);
    SmartAPIActivator->SetAutoLearning(true);
    SmartAPIActivator->EnablePredictiveFeatures(true);
    
    // 設置開發專用面板
    CreateDevelopmentDashboard();
    
    // 啟動持續監控
    SmartAPIActivator->StartSystemMonitoring();
}

void AMyGameMode::CreateDevelopmentDashboard()
{
    if (SmartAPIActivator)
    {
        UMingRTSAPISystem* APISystem = SmartAPIActivator->GetAPISystem();
        if (APISystem && APISystem->GetSmartPanelAPI())
        {
            // 創建開發面板
            FPanelConfiguration DevPanel = APISystem->GetSmartPanelAPI()->CreatePanel(
                EPanelType::Dashboard, TEXT("DevelopmentDashboard"));
            
            // 添加編譯狀態組件
            FPanelWidget CompileWidget;
            CompileWidget.WidgetId = TEXT("CompileStatus");
            CompileWidget.WidgetType = TEXT("StatusIndicator");
            CompileWidget.Title = TEXT("編譯狀態");
            
            // 添加性能監控組件
            FPanelWidget PerformanceWidget;
            PerformanceWidget.WidgetId = TEXT("PerformanceMonitor");
            PerformanceWidget.WidgetType = TEXT("PerformanceChart");
            PerformanceWidget.Title = TEXT("性能監控");
            
            // 添加錯誤日誌組件
            FPanelWidget ErrorWidget;
            ErrorWidget.WidgetId = TEXT("ErrorLog");
            ErrorWidget.WidgetType = TEXT("LogViewer");
            ErrorWidget.Title = TEXT("錯誤日誌");
            
            // 添加組件到面板
            APISystem->GetSmartPanelAPI()->AddWidget(DevPanel.PanelName, CompileWidget);
            APISystem->GetSmartPanelAPI()->AddWidget(DevPanel.PanelName, PerformanceWidget);
            APISystem->GetSmartPanelAPI()->AddWidget(DevPanel.PanelName, ErrorWidget);
        }
    }
}
```

### 場景2：持續集成
```cpp
void AMyGameMode::SetupContinuousIntegration()
{
    if (SmartAPIActivator)
    {
        // 配置自動化工作流
        SmartAPIActivator->SetCoordinationMode(TEXT("Automatic"));
        
        // 綁定編譯完成事件
        UMingRTSAPISystem* APISystem = SmartAPIActivator->GetAPISystem();
        if (APISystem && APISystem->GetCompilationAPI())
        {
            APISystem->GetCompilationAPI()->OnCompilationCompleted.AddDynamic(
                this, &AMyGameMode::OnCompilationCompleted);
        }
    }
}

UFUNCTION()
void AMyGameMode::OnCompilationCompleted(const FCompilationResult& Result)
{
    if (Result.bSuccess)
    {
        // 編譯成功，運行測試
        RunAutomatedTests();
        
        // 更新性能指標
        UpdatePerformanceMetrics();
    }
    else
    {
        // 編譯失敗，自動修復
        TriggerAutoFix(Result);
    }
}
```

## 🔧 故障排除

### 常見問題

#### 1. 系統無法啟用
```cpp
// 檢查系統狀態
if (!SmartAPIActivator->IsSmartAPISystemEnabled())
{
    UE_LOG(LogTemp, Error, TEXT("智慧API系統未啟用"));
    
    // 嘗試重新啟用
    SmartAPIActivator->EnableSmartAPISystem();
}
```

#### 2. API協調失敗
```cpp
// 檢查API健康狀態
TArray<FString> HealthStatus = SmartAPIActivator->GetSystemStatus();
for (const FString& Status : HealthStatus)
{
    UE_LOG(LogTemp, Log, TEXT("系統狀態: %s"), *Status);
}

// 檢查性能指標
TMap<FString, float> Metrics = SmartAPIActivator->GetPerformanceMetrics();
for (const auto& Metric : Metrics)
{
    if (Metric.Value > 1000.0f) // 響應時間過長
    {
        UE_LOG(LogTemp, Warning, TEXT("API %s 響應時間過長: %.2fms"), 
            *Metric.Key, Metric.Value);
    }
}
```

#### 3. 學習功能異常
```cpp
// 重新配置學習功能
SmartAPIActivator->SetAutoLearning(false);
SmartAPIActivator->SetAutoLearning(true);

// 檢查學習數據
UMingRTSAPISystem* APISystem = SmartAPIActivator->GetAPISystem();
if (APISystem && APISystem->GetOptimizationAPI())
{
    // 獲取優化歷史
    TArray<FOptimizationResult> History = APISystem->GetOptimizationAPI()->GetHistoricalMetrics(
        FDateTime::Now() - FTimespan::FromDays(7), FDateTime::Now());
    
    UE_LOG(LogTemp, Log, TEXT("優化歷史記錄數量: %d"), History.Num());
}
```

## 📚 最佳實踐

### 1. 初始化最佳實踐
- 在BeginPlay中初始化智慧API系統
- 配置適合您需求的智慧級別
- 設置事件處理器監控系統狀態
- 創建適合的監控面板

### 2. 性能最佳實踐
- 使用預測功能避免性能問題
- 定期檢查系統健康狀態
- 利用自動化工作流提高效率
- 監控API響應時間

### 3. 學習最佳實踐
- 保持自動學習功能啟用
- 定期檢查學習效果
- 根據學習結果調整配置
- 利用預測功能主動預防問題

## 🎉 開始使用！

現在您可以開始使用MingGoRTS智慧API系統了：

1. **包含頭文件** - 添加必要的API頭文件
2. **創建啟用器** - 初始化智慧API系統
3. **配置功能** - 根據需求配置智慧功能
4. **使用API** - 開始使用各種智慧功能
5. **監控狀態** - 持續監控系統健康狀態

祝您使用愉快！🚀
