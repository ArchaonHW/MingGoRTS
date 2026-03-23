# MingGoRTS 任務分析系統使用指南

## 概述

MingGoRTS 任務分析系統是一個智能的編譯日誌分析工具，能夠自動將編譯過程中產生的日誌分析並生成結構化的任務需求單，按照除錯、優化、性能、安全等類型分類，並根據嚴重程度自動設定優先層級。

## 快速開始

### 1. 初始化系統

```cpp
// 在您的遊戲模式或任何 Actor 中
#include "MingTaskAnalyzer.h"

// 創建任務分析器實例
UMingTaskAnalyzer* TaskAnalyzer = NewObject<UMingTaskAnalyzer>();

// 初始化系統
TaskAnalyzer->InitializeTaskAnalyzer();

// 綁定事件（可選）
TaskAnalyzer->OnLogAnalysisStarted.AddDynamic(this, &AMyActor::OnLogAnalysisStarted);
TaskAnalyzer->OnLogAnalysisProgress.AddDynamic(this, &AMyActor::OnLogAnalysisProgress);
TaskAnalyzer->OnLogAnalysisCompleted.AddDynamic(this, &AMyActor::OnLogAnalysisCompleted);
```

### 2. 編譯專案並分析

```cpp
// 配置編譯參數
FMingCompileConfiguration Config;
Config.CompileMode = EMingCompileMode::Development;
Config.Target = EMingCompileTarget::Game;
Config.bCleanBuild = true;
Config.AdditionalArguments.Add(TEXT("-logcommands"));
Config.AdditionalArguments.Add(TEXT("-verbose"));

// 編譯專案並生成日誌
FMingCompileResult CompileResult = TaskAnalyzer->CompileProjectAndGenerateLog(Config);

// 檢查編譯結果
if (CompileResult.Status == EMingCompileStatus::Success)
{
    UE_LOG(LogTemp, Log, TEXT("編譯成功，開始分析日誌..."));
    
    // 分析編譯日誌
    FString LogPath = FPaths::Combine(*FPaths::ProjectDir(), TEXT("Logs/CompileLog_TaskAnalysis.txt"));
    FMingLogAnalysisResult AnalysisResult = TaskAnalyzer->AnalyzeCompileLog(LogPath);
    
    // 生成任務報告
    FString ReportPath = FPaths::Combine(*FPaths::ProjectDir(), TEXT("Reports/TaskAnalysisReport.html"));
    TaskAnalyzer->GenerateTaskReport(AnalysisResult, ReportPath);
}
else
{
    UE_LOG(LogTemp, Error, TEXT("編譯失敗：%s"), *CompileResult.ErrorMessage);
}
```

### 3. 直接分析現有日誌

```cpp
// 如果您已有編譯日誌文件
FString ExistingLogPath = TEXT("C:\\HW\\MingGoRTS\\Logs\\PreviousCompile.log");
FMingLogAnalysisResult AnalysisResult = TaskAnalyzer->AnalyzeCompileLog(ExistingLogPath);

// 或者直接從文本分析
FString LogText = TEXT("您的編譯日誌內容...");
FMingLogAnalysisResult AnalysisResult = TaskAnalyzer->AnalyzeLogFromText(LogText);
```

## 詳細功能說明

### 任務類型分類

系統會自動將日誌中的問題分類為以下類型：

#### 🐛 除錯任務 (Debug)
- **編譯錯誤**: error, fatal error, compilation failed
- **語法錯誤**: syntax error, cannot parse
- **鏈接錯誤**: undefined reference, unresolved external
- **文件錯誤**: cannot open, file not found

#### ⚡ 優化任務 (Optimization)
- **未使用變量**: unused variable, unreferenced
- **過時函數**: deprecated, obsolete
- **隱式轉換**: implicit conversion, narrowing
- **代碼重複**: code duplication

#### 🚀 性能任務 (Performance)
- **內存問題**: memory leak, memory usage
- **性能瓶頸**: performance warning, bottleneck
- **緩存問題**: cache miss, inefficient
- **CPU 使用**: cpu usage, slow

#### 🔒 安全任務 (Security)
- **緩衝區溢出**: buffer overflow, stack overflow
- **訪問違規**: access violation, memory corruption
- **安全漏洞**: security vulnerability, injection
- **權限問題**: privilege escalation

### 優先級自動分配

系統會根據問題類型和嚴重程度自動設定優先級：

#### 🚨 關鍵級 (Critical)
- **處理時間**: 立即 (24小時內)
- **包含**: 編譯錯誤、安全漏洞、內存洩漏
- **特點**: 系統無法構建或存在嚴重安全風險

#### 🔴 高級 (High)
- **處理時間**: 優先 (3天內)
- **包含**: 性能瓶頸、重要警告、功能缺陷
- **特點**: 影響系統功能或性能

#### 🟡 中級 (Medium)
- **處理時間**: 正常 (1週內)
- **包含**: 一般警告、代碼優化建議
- **特點**: 不影響核心功能的一般問題

#### 🟢 低級 (Low)
- **處理時間**: 後續 (2週內)
- **包含**: 代碼風格、輕微優化建議
- **特點**: 輕微問題，可延後處理

#### ⚪ 延遲級 (Deferred)
- **處理時間**: 可選 (未來版本)
- **包含**: 未來功能改進、長期重構
- **特點**: 可選的改進項目

## 任務需求單結構

每個生成的任務需求單包含以下信息：

```cpp
struct FMingTaskRequirement
{
    FString TaskID;              // 唯一識別碼 (格式: TYPE_XXXX)
    FString Title;               // 任務標題
    FString Description;         // 詳細描述
    EMingTaskType TaskType;      // 任務類型
    EMingTaskPriority Priority;  // 優先級
    EMingTaskStatus Status;      // 狀態
    FString SourceFile;          // 源文件路徑
    int32 LineNumber;            // 行號
    FString LogMessage;          // 原始日誌
    FString ErrorMessage;        // 錯誤詳情
    FString SuggestedFix;        // 建議修復方案
    float EstimatedHours;        // 預估工時
    FDateTime CreatedTime;       // 創建時間
    FDateTime DueTime;           // 到期時間
    TArray<FString> Dependencies; // 依賴關係
    FString AssignedTo;          // 分配對象
};
```

## 報告生成

### HTML 報告

系統會生成詳細的 HTML 報告，包含：

- 📊 **統計摘要**: 總任務數、錯誤數、警告數、預估工時
- 🚨 **關鍵任務**: 立即需要處理的關鍵問題
- 🐛 **除錯任務**: 所有除錯相關任務列表
- ⚡ **優化任務**: 代碼優化建議
- 🚀 **性能任務**: 性能改進機會
- 🔒 **安全任務**: 安全問題和風險

### 數據導出

```cpp
// 導出為 JSON 格式
FString JSONData = TaskAnalyzer->ExportTasksToJSON(AnalysisResult);
FFileHelper::SaveStringToFile(JSONData, TEXT("Tasks.json"));

// 導出為 CSV 格式
FString CSVData = TaskAnalyzer->ExportTasksToCSV(AnalysisResult);
FFileHelper::SaveStringToFile(CSVData, TEXT("Tasks.csv"));
```

## 事件處理

```cpp
// 日誌分析開始
UFUNCTION()
void AMyActor::OnLogAnalysisStarted(const FString& LogPath)
{
    UE_LOG(LogTemp, Log, TEXT("開始分析日誌：%s"), *LogPath);
}

// 分析進度更新
UFUNCTION()
void AMyActor::OnLogAnalysisProgress(float Progress, const FString& CurrentTask)
{
    UE_LOG(LogTemp, Log, TEXT("分析進度：%.1f%% - %s"), Progress * 100.0f, *CurrentTask);
}

// 分析完成
UFUNCTION()
void AMyActor::OnLogAnalysisCompleted(const FMingLogAnalysisResult& Result)
{
    UE_LOG(LogTemp, Log, TEXT("分析完成！發現 %d 個任務"), Result.TotalMessages);
    
    // 獲取關鍵任務
    TArray<FMingTaskRequirement> CriticalTasks = TaskAnalyzer->GetCriticalTasks(Result);
    
    // 獲取高優先級任務
    TArray<FMingTaskRequirement> HighPriorityTasks = TaskAnalyzer->GetHighPriorityTasks(Result);
    
    // 計算任務統計
    TMap<EMingTaskType, int32> Statistics = TaskAnalyzer->CalculateTaskStatistics(Result);
}
```

## 藍圖集成

```cpp
// 藍圖函數庫
UCLASS()
class MYGAME_API UMyTaskAnalysisLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Task Analysis")
    static FMingLogAnalysisResult AnalyzeCompileLogBlueprint(const FString& LogPath)
    {
        UMingTaskAnalyzer* Analyzer = NewObject<UMingTaskAnalyzer>();
        Analyzer->InitializeTaskAnalyzer();
        return Analyzer->AnalyzeCompileLog(LogPath);
    }

    UFUNCTION(BlueprintCallable, Category = "Task Analysis")
    static bool GenerateTaskReportBlueprint(const FMingLogAnalysisResult& AnalysisResult, const FString& ReportPath)
    {
        UMingTaskAnalyzer* Analyzer = NewObject<UMingTaskAnalyzer>();
        return Analyzer->GenerateTaskReport(AnalysisResult, ReportPath);
    }
};
```

## 實際使用範例

### 範例 1: 日常構建分析

```cpp
void AMyBuildManager::DailyBuildAnalysis()
{
    UMingTaskAnalyzer* Analyzer = NewObject<UMingTaskAnalyzer>();
    Analyzer->InitializeTaskAnalyzer();
    
    // 配置日常構建
    FMingCompileConfiguration Config;
    Config.CompileMode = EMingCompileMode::Development;
    Config.Target = EMingCompileTarget::Game;
    Config.bCleanBuild = false; // 增量構建
    
    // 執行構建和分析
    FMingCompileResult CompileResult = Analyzer->CompileProjectAndGenerateLog(Config);
    FMingLogAnalysisResult AnalysisResult = Analyzer->AnalyzeCompileLog(TEXT("Logs/DailyBuild.log"));
    
    // 生成報告
    FString ReportPath = FString::Printf(TEXT("Reports/DailyBuild_%s.html"), 
        *FDateTime::Now().ToString(TEXT("%Y%m%d")));
    Analyzer->GenerateTaskReport(AnalysisResult, ReportPath);
    
    // 發送通知
    if (AnalysisResult.TotalErrors > 0)
    {
        SendBuildFailureNotification(AnalysisResult.TotalErrors);
    }
}
```

### 範例 2: 發布前檢查

```cpp
void AMyReleaseManager::PreReleaseCheck()
{
    UMingTaskAnalyzer* Analyzer = NewObject<UMingTaskAnalyzer>();
    Analyzer->InitializeTaskAnalyzer();
    
    // 配置發布構建
    FMingCompileConfiguration Config;
    Config.CompileMode = EMingCompileMode::Shipping;
    Config.Target = EMingCompileTarget::Game;
    Config.bCleanBuild = true; // 清理構建
    Config.AdditionalArguments.Add(TEXT("-logcommands"));
    Config.AdditionalArguments.Add(TEXT("-verbose"));
    
    // 執行完整構建和分析
    FMingCompileResult CompileResult = Analyzer->CompileProjectAndGenerateLog(Config);
    FMingLogAnalysisResult AnalysisResult = Analyzer->AnalyzeCompileLog(TEXT("Logs/ReleaseBuild.log"));
    
    // 檢查關鍵問題
    TArray<FMingTaskRequirement> CriticalTasks = Analyzer->GetCriticalTasks(AnalysisResult);
    if (CriticalTasks.Num() > 0)
    {
        UE_LOG(LogTemp, Error, TEXT("發布前檢查失敗！發現 %d 個關鍵問題"), CriticalTasks.Num());
        
        // 生成詳細報告
        FString ReportPath = TEXT("Reports/ReleaseCheck_FAILED.html");
        Analyzer->GenerateTaskReport(AnalysisResult, ReportPath);
        
        return false; // 阻止發布
    }
    
    UE_LOG(LogTemp, Log, TEXT("發布前檢查通過！"));
    return true;
}
```

### 範例 3: CI/CD 集成

```cpp
void AMyCIBuild::AutomatedTaskAnalysis()
{
    UMingTaskAnalyzer* Analyzer = NewObject<UMingTaskAnalyzer>();
    Analyzer->InitializeTaskAnalyzer();
    
    // 從環境變數獲取配置
    FMingCompileConfiguration Config = GetConfigFromEnvironment();
    
    // 執行構建和分析
    FMingCompileResult CompileResult = Analyzer->CompileProjectAndGenerateLog(Config);
    FMingLogAnalysisResult AnalysisResult = Analyzer->AnalyzeCompileLog(TEXT("Logs/CIBuild.log"));
    
    // 導出數據供其他系統使用
    FString JSONData = Analyzer->ExportTasksToJSON(AnalysisResult);
    FString CSVData = Analyzer->ExportTasksToCSV(AnalysisResult);
    
    // 保存到共享位置
    FFileHelper::SaveStringToFile(JSONData, TEXT("Shared/CITasks.json"));
    FFileHelper::SaveStringToFile(CSVData, TEXT("Shared/CITasks.csv"));
    
    // 設置 CI 退出代碼
    int32 ExitCode = (AnalysisResult.TotalErrors == 0) ? 0 : 1;
    FPlatformMisc::RequestExit(ExitCode);
}
```

## 故障排除

### 常見問題

1. **編譯失敗**
   - 檢查編譯配置是否正確
   - 確認專案路徑存在
   - 檢查 Unreal Build Tool 路徑

2. **日誌分析失敗**
   - 確認日誌文件存在且可讀
   - 檢查日誌格式是否正確
   - 驗證文件權限

3. **報告生成失敗**
   - 檢查報告目錄權限
   - 確認磁碟空間足夠
   - 檢查文件路徑格式

### 調試技巧

1. **啟用詳細日誌**
   ```cpp
   UE_LOG(LogTemp, VeryVerbose, TEXT("任務分析詳細日誌"));
   ```

2. **檢查分析進度**
   ```cpp
   Analyzer->OnLogAnalysisProgress.AddDynamic(this, &AMyActor::OnProgress);
   ```

3. **驗證任務分類**
   ```cpp
   TMap<EMingTaskType, int32> Stats = Analyzer->CalculateTaskStatistics(Result);
   for (const auto& Stat : Stats)
   {
       UE_LOG(LogTemp, Log, TEXT("%s: %d"), *TaskTypeToString(Stat.Key), Stat.Value);
   }
   ```

## 性能優化

### 大型日誌處理

```cpp
// 對於大型日誌文件，可以分批處理
void ProcessLargeLogInBatches(const FString& LogPath)
{
    FString LogContent;
    FFileHelper::LoadFileToString(LogContent, *LogPath);
    
    // 分批處理 (每 1000 行一批)
    TArray<FString> Lines;
    LogContent.ParseIntoArrayLines(Lines);
    
    for (int32 i = 0; i < Lines.Num(); i += 1000)
    {
        TArray<FString> Batch;
        for (int32 j = 0; j < 1000 && (i + j) < Lines.Num(); j++)
        {
            Batch.Add(Lines[i + j]);
        }
        
        FString BatchText = FString::Join(Batch, TEXT("\n"));
        FMingLogAnalysisResult BatchResult = TaskAnalyzer->AnalyzeLogFromText(BatchText);
        
        // 處理批次結果...
    }
}
```

### 並行處理

```cpp
// 並行處理多個日誌文件
void ParallelLogAnalysis(const TArray<FString>& LogPaths)
{
    TArray<TFuture<FMingLogAnalysisResult>> Futures;
    
    for (const FString& LogPath : LogPaths)
    {
        TFuture<FMingLogAnalysisResult> Future = Async<FMingLogAnalysisResult>([LogPath]()
        {
            UMingTaskAnalyzer* Analyzer = NewObject<UMingTaskAnalyzer>();
            Analyzer->InitializeTaskAnalyzer();
            return Analyzer->AnalyzeCompileLog(LogPath);
        });
        
        Futures.Add(Future);
    }
    
    // 等待所有任務完成
    for (auto& Future : Futures)
    {
        FMingLogAnalysisResult Result = Future.Get();
        // 處理結果...
    }
}
```

## 總結

MingGoRTS 任務分析系統提供了強大的自動化日誌分析能力，能夠：

- ✅ **自動編譯分析** - 無需手動執行編譯
- ✅ **智能任務分類** - 自動識別問題類型
- ✅ **優先級自動分配** - 根據嚴重程度設定優先級
- ✅ **詳細報告生成** - HTML 報告和數據導出
- ✅ **事件驅動架構** - 實時進度更新
- ✅ **藍圖集成** - 支持藍圖調用
- ✅ **擴展性設計** - 易於擴展和自定義

通過本系統，開發團隊可以大幅提高問題識別和處理效率，確保代碼質量和系統穩定性。
