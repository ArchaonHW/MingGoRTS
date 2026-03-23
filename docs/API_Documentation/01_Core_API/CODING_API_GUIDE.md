# MingRTS Coding API 開發指南

## 概述

MingRTS Coding API 是一個統合的程式開發 API 系統，整合了專案中所有程式語言相關功能，並將它們分配到對應的 API 域中。

## 系統架構

### 核心組件

1. **UMingRTSCodingAPI** - 統合管理器
   - 功能發現與查詢
   - 任務分配與工作流
   - 跨域 API 調用
   - 智能推薦系統

2. **UMingRTSCodingAPIMappings** - 功能映射管理
   - 功能到 API 的映射
   - 反向查找
   - 報告生成

## API 域分類

| 域 | 來源 API | 主要功能 |
|----|---------|---------|
| **CodeGeneration** | MingRTSPotatoAICodeGeneratorAPI | AI 代碼生成、模板管理 |
| **AICodeGeneration** | MingRTSPotatoAIEnhancedAPI | 智能代碼生成、需求分析 |
| **CodeAnalysis** | MingRTSCodeReviewAPI, MingRTSCodeStandardsAPI | 代碼審查、規範檢查 |
| **Debugging** | MingRTSDebugAPI | 除錯、斷點管理、性能分析 |
| **Compilation** | MingRTSCompilationAPI | 編譯、優化、錯誤修復 |
| **VersionControl** | MingRTSAPISystem | Git 操作、分支管理 |
| **MultiAgent** | MingRTSBMADSystem | 多智能體協作、決策 |
| **ScriptIntegration** | MingRTSScriptIntegrationAPI | PowerShell/Python 整合 |
| **AdvancedFeatures** | MingRTSAdvancedAPIFeatures | NLP、數據挖掘、安全分析 |
| **LearningSystem** | MingRTSUE5LearningAPI | 學習系統、文檔生成 |

## 快速入門

### 1. 初始化 Coding API

```cpp
// 創建 Coding API 實例
UMingRTSCodingAPI* CodingAPI = NewObject<UMingRTSCodingAPI>();

// 初始化系統
if (CodingAPI->InitializeCodingAPI())
{
    UE_LOG(LogTemp, Log, TEXT("Coding API initialized successfully"));
}
```

### 2. 代碼生成

```cpp
// 使用代碼生成功能
FCodingAPIResult Result = CodingAPI->GenerateCode(
    ECodingAPIDomain::CodeGeneration,
    TEXT("Create a player controller class with movement and combat"),
    ESupportedLanguage::CPP
);

if (Result.bSuccess)
{
    FString GeneratedCode = Result.OutputData[TEXT("code")];
    UE_LOG(LogTemp, Log, TEXT("Generated code: %s"), *GeneratedCode);
}
```

### 3. 代碼分析

```cpp
// 分析代碼品質
FCodingAPIResult Result = CodingAPI->AnalyzeCode(
    SourceCode,
    ECodingAPIDomain::CodeAnalysis
);

if (Result.bSuccess)
{
    TArray<FString> Issues = Result.OutputData[TEXT("issues")].ParseIntoArray();
    for (const FString& Issue : Issues)
    {
        UE_LOG(LogTemp, Warning, TEXT("Issue found: %s"), *Issue);
    }
}
```

### 4. 除錯功能

```cpp
// 啟動除錯會話
FCodingAPIResult Result = CodingAPI->DebugCode(
    SourceCode,
    ErrorMessage
);
```

### 5. 編譯項目

```cpp
TMap<FString, FString> CompileOptions;
CompileOptions.Add(TEXT("configuration"), TEXT("Development"));
CompileOptions.Add(TEXT("platform"), TEXT("Win64"));

FCodingAPIResult Result = CodingAPI->CompileProject(
    ProjectPath,
    CompileOptions
);
```

### 6. 版本控制

```cpp
TMap<FString, FString> GitParams;
GitParams.Add(TEXT("message"), TEXT("Feature: Add new combat system"));

FCodingAPIResult Result = CodingAPI->ExecuteVersionControl(
    TEXT("commit"),
    GitParams
);
```

## 任務分配

### 自動任務分配

Coding API 可以根據任務類型自動分配相關的 API 域：

```cpp
// 獲取功能開發相關的 API
TArray<ECodingAPIDomain> Domains = CodingAPI->GetDomainsForTask(ECodingTaskType::FeatureDevelopment);
// 返回: [CodeGeneration, AICodeGeneration, CodeAnalysis]

// 獲取錯誤修復相關的 API
TArray<ECodingAPIDomain> Domains = CodingAPI->GetDomainsForTask(ECodingTaskType::BugFix);
// 返回: [Debugging, CodeAnalysis, Compilation]

// 獲取重構相關的 API
TArray<ECodingAPIDomain> Domains = CodingAPI->GetDomainsForTask(ECodingTaskType::Refactoring);
// 返回: [SmartRefactoring, CodeAnalysis, CodeReview]
```

### 工作流推薦

```cpp
// 獲取推薦工作流
FString Workflow = CodingAPI->GetRecommendedWorkflow(ECodingTaskType::FeatureDevelopment);
// 返回: "1. AI代碼生成 (AICodeGeneration) -> 2. 代碼審查 (CodeReview) -> 3. 編譯測試 (Compilation) -> 4. 版本控制 (VersionControl)"
```

## 功能映射查詢

### 初始化映射

```cpp
UMingRTSCodingAPIMappings* Mappings = NewObject<UMingRTSCodingAPIMappings>();
Mappings->InitializeMappings();
```

### 查詢功能映射

```cpp
// 查詢特定功能的完整路徑
FString FunctionPath = Mappings->GetFunctionMapping(
    ECodingAPIDomain::CodeGeneration,
    TEXT("GenerateUE5Class")
);
// 返回: "UMingRTSPotatoAICodeGeneratorAPI::GenerateUE5Class"
```

### 獲取域內所有功能

```cpp
// 獲取代碼生成域的所有功能
TArray<FString> Functions = Mappings->GetAllFunctionsInDomain(ECodingAPIDomain::CodeGeneration);
// 返回: ["GenerateUE5Class", "GenerateUE5Function", "GenerateUE5System", ...]
```

### 反向查找

```cpp
// 根據功能名稱查找所屬域
ECodingAPIDomain Domain = Mappings->GetDomainForFunction(TEXT("StartDebugSession"));
// 返回: ECodingAPIDomain::Debugging
```

## 跨域整合

### 域間相容性

```cpp
// 檢查兩個域是否可以整合
bool bCanIntegrate = CodingAPI->CanDomainsIntegrate(
    ECodingAPIDomain::CodeGeneration,
    ECodingAPIDomain::CodeAnalysis
);
// 返回: true (代碼生成後可以進行分析)
```

### 跨域調用

```cpp
// 從代碼生成域調用分析域
TMap<FString, FString> Params;
Params.Add(TEXT("generated_code"), GeneratedCode);

FCodingAPIResult Result = CodingAPI->ExecuteCrossDomain(
    ECodingAPIDomain::CodeGeneration,  // 源域
    ECodingAPIDomain::CodeAnalysis,    // 目標域
    Params
);
```

### 管道執行

```cpp
// 定義執行管道
TArray<ECodingAPIDomain> Pipeline = {
    ECodingAPIDomain::CodeGeneration,
    ECodingAPIDomain::CodeAnalysis,
    ECodingAPIDomain::Compilation
};

TMap<FString, FString> InitialParams;
InitialParams.Add(TEXT("requirement"), TEXT("Create a save game system"));

TArray<FCodingAPIResult> Results = CodingAPI->ExecutePipeline(Pipeline, InitialParams);
```

## 智能推薦

### 基於上下文的推薦

```cpp
// 根據上下文獲取推薦功能
TArray<FCodingAPIFunction> Recommendations = CodingAPI->GetRecommendedFunctions(
    TEXT("I need to debug a memory leak in my game")
);
// 返回: [除錯功能, 記憶體分析功能, 性能分析功能]
```

### 最佳域推薦

```cpp
// 推薦最適合處理該任務的域
ECodingAPIDomain BestDomain = CodingAPI->SuggestBestDomain(
    TEXT("Create a new AI behavior tree for enemy combat")
);
// 返回: ECodingAPIDomain::AICodeGeneration
```

### 生成代碼示例

```cpp
// 生成 API 使用示例
FString Example = CodingAPI->GenerateAPICodeExample(
    ECodingAPIDomain::CodeGeneration,
    TEXT("GenerateUE5Class")
);
```

## 報告與統計

### API 使用統計

```cpp
// 獲取 API 指標
FCodingAPIMetrics Metrics = CodingAPI->GetAPIMetrics();

UE_LOG(LogTemp, Log, TEXT("Total functions: %d"), Metrics.TotalFunctions);
UE_LOG(LogTemp, Log, TEXT("Active domains: %d"), Metrics.ActiveDomains);
UE_LOG(LogTemp, Log, TEXT("Average response time: %.2f ms"), Metrics.AverageResponseTime);
UE_LOG(LogTemp, Log, TEXT("Success rate: %.1f%%"), Metrics.SuccessRate * 100.0f);
```

### 生成報告

```cpp
// 生成 API 報告
FString Report = CodingAPI->GenerateAPIReport();
UE_LOG(LogTemp, Log, TEXT("%s"), *Report);
```

### 導出文檔

```cpp
// 導出 API 文檔
bool bExported = CodingAPI->ExportAPIDocumentation(TEXT("docs/CODING_API_REFERENCE.md"));
```

### 映射報告

```cpp
// 生成功能映射報告
UMingRTSCodingAPIMappings* Mappings = NewObject<UMingRTSCodingAPIMappings>();
Mappings->InitializeMappings();

FString MappingReport = Mappings->GenerateMappingReport();
UE_LOG(LogTemp, Log, TEXT("%s"), *MappingReport);
```

## 支援語言

Coding API 支援多種程式語言：

| 語言 | 枚舉值 | 主要用途 |
|------|--------|----------|
| C++ | ESupportedLanguage::CPP | 核心遊戲邏輯 |
| Blueprint | ESupportedLanguage::Blueprint | 視覺腳本 |
| Python | ESupportedLanguage::Python | 工具腳本 |
| JavaScript | ESupportedLanguage::JavaScript | Web 整合 |
| PowerShell | ESupportedLanguage::PowerShell | 自動化 |
| Shader | ESupportedLanguage::ShaderLanguage | 著色器 |
| INI | ESupportedLanguage::INI | 配置檔案 |
| JSON | ESupportedLanguage::JSON | 資料交換 |
| XML | ESupportedLanguage::XML | 資料定義 |
| Markdown | ESupportedLanguage::Markdown | 文檔 |

## 事件系統

### 訂閱事件

```cpp
// 訂閱域註冊事件
CodingAPI->OnDomainRegistered.AddDynamic(this, &UMyClass::OnDomainRegistered);

// 訂閱功能調用事件
CodingAPI->OnFunctionCalled.AddDynamic(this, &UMyClass::OnFunctionCalled);

// 訂閱任務完成事件
CodingAPI->OnTaskCompleted.AddDynamic(this, &UMyClass::OnTaskCompleted);

// 訂閱指標更新事件
CodingAPI->OnMetricsUpdated.AddDynamic(this, &UMyClass::OnMetricsUpdated);
```

### 事件處理

```cpp
UFUNCTION()
void OnDomainRegistered(ECodingAPIDomain Domain, const FString& ModuleName)
{
    UE_LOG(LogTemp, Log, TEXT("New domain registered: %s - %s"),
        *MingCodingAPI::DomainToString(Domain),
        *ModuleName);
}

UFUNCTION()
void OnFunctionCalled(ECodingAPIDomain Domain, const FString& FunctionName)
{
    UE_LOG(LogTemp, Verbose, TEXT("Function called: %s.%s"),
        *MingCodingAPI::DomainToString(Domain),
        *FunctionName);
}

UFUNCTION()
void OnTaskCompleted(const FCodingAPIResult& Result)
{
    UE_LOG(LogTemp, Log, TEXT("Task completed: %s (%.2f ms) - %s"),
        Result.bSuccess ? TEXT("Success") : TEXT("Failed"),
        Result.ExecutionTime,
        *Result.Message);
}
```

## 最佳實踐

### 1. 錯誤處理

```cpp
FCodingAPIResult Result = CodingAPI->GenerateCode(...);

if (!Result.bSuccess)
{
    // 處理錯誤
    UE_LOG(LogTemp, Error, TEXT("Code generation failed: %s"), *Result.Message);
    
    // 嘗試替代方案
    if (Result.ExecutionTime > 10000) // 超過10秒
    {
        // 使用簡化需求重試
        Result = CodingAPI->GenerateCode(...);
    }
}
```

### 2. 性能優化

```cpp
// 批量操作而非逐個調用
TArray<FString> Requirements = { Req1, Req2, Req3 };
// 使用批次 API 而非循環調用單個生成
```

### 3. 緩存結果

```cpp
// 對於重複需求，緩存結果
TMap<FString, FString> CodeCache;

FString GetCachedOrGenerate(const FString& Requirement)
{
    if (FString* Cached = CodeCache.Find(Requirement))
    {
        return *Cached;
    }
    
    FCodingAPIResult Result = CodingAPI->GenerateCode(...);
    if (Result.bSuccess)
    {
        CodeCache.Add(Requirement, Result.OutputData[TEXT("code")]);
    }
    
    return Result.OutputData[TEXT("code")];
}
```

### 4. 合理使用管道

```cpp
// 對於複雜工作流，使用管道而非多次單獨調用
TArray<ECodingAPIDomain> Pipeline = {
    ECodingAPIDomain::CodeGeneration,
    ECodingAPIDomain::CodeAnalysis,
    ECodingAPIDomain::CodeReview
};

TArray<FCodingAPIResult> Results = CodingAPI->ExecutePipeline(Pipeline, Params);
// 比單獨調用三次更高效，支持數據自動傳遞
```

## 檔案結構

```
Source/MingGoRTS/
├── Public/API/
│   ├── MingRTSCodingAPI.h                      # 核心 Coding API
│   ├── MingRTSCodingAPIFunctionMappings.h     # 功能映射定義
│   ├── MingRTSAPIOrchestrator.h               # API 協調器
│   ├── MingRTSPotatoAICodeGeneratorAPI.h      # AI 代碼生成
│   ├── MingRTSPotatoAIEnhancedAPI.h           # 增強 AI 生成
│   ├── MingRTSCodeReviewAPI.h                 # 代碼審查
│   ├── MingRTSCodeStandardsAPI.h              # 代碼規範
│   ├── MingRTSDebugAPI.h                      # 除錯 API
│   ├── MingRTSCompilationAPI.h                # 編譯 API
│   ├── MingRTSBMADSystem.h                    # 多智能體系統
│   └── ... (其他 API 檔案)
├── Private/API/
│   ├── MingRTSCodingAPI.cpp                   # Coding API 實現
│   ├── MingRTSCodingAPIFunctionMappings.cpp   # 映射實現
│   └── ... (其他實現檔案)
└── docs/
    └── CODING_API_GUIDE.md                    # 本開發指南
```

## 常見問題

### Q: 如何擴展新的 API 域？
A: 1. 在 `ECodingAPIDomain` 枚舉中添加新域
   2. 創建對應的 API 類
   3. 在 `MingRTSCodingAPIMappings` 中添加映射
   4. 在 `InitializeDefaultModules` 中註冊

### Q: 如何查詢某個功能是否存在？
A: 使用 `UMingRTSCodingAPIMappings::IsFunctionAvailable` 或搜尋功能列表。

### Q: 支援哪些程式語言？
A: 目前支援 C++, Blueprint, Python, JavaScript, PowerShell, Shader, INI, JSON, XML, Markdown。

### Q: 如何處理跨語言的功能調用？
A: 使用 `ESupportedLanguage` 指定目標語言，Coding API 會自動選擇適當的處理器。

## 更新日誌

### v1.0 (2026-03-23)
- 初始版本發布
- 整合 10 個核心 API 域
- 支援 10 種程式語言
- 實現 200+ 個功能映射
- 智能推薦系統
- 跨域整合功能

## 聯繫與支援

如有問題或建議，請參考：
- 完整 API 文檔: `docs/FUNCTIONS_AND_COMMANDS_REFERENCE.md`
- 功能映射參考: `Source/MingGoRTS/Public/API/MingRTSCodingAPIFunctionMappings.h`
- 範例代碼: `Source/MingGoRTS/Public/API/MingRTSAPIExample.h`
