# MingGoRTS Code Review API 使用指南

## 系統概述

Code Review API 是 MingGoRTS 版控系統的智能代碼審查組件，在提交前自動執行全面的代碼質量檢查，確保代碼符合規範標準。

## 核心特性

### 🔍 全面檢查類型
- **語法檢查** - 檢測語法錯誤、缺少分號等
- **代碼風格** - 命名規範、格式一致性
- **性能問題** - 低效操作、內存洩漏風險
- **安全問題** - 硬編碼密碼、SQL注入風險
- **可維護性** - 函數長度、複雜度分析
- **文檔規範** - UFUNCTION文檔完整性
- **測試覆蓋** - 測試用例覆蓋率
- **依賴管理** - 不必要的包含檔案

### 🚀 智能修復功能
- **自動修復** - 可自動修復的簡單問題
- **批量修復** - 一鍵修復多個問題
- **修復建議** - 針對性的修復建議
- **修復驗證** - 修復後重新驗證

### 📊 詳細報告系統
- **HTML報告** - 美觀的可視化報告
- **問題分類** - 按嚴重程度和類型分類
- **統計指標** - 代碼質量分數、問題統計
- **趨勢分析** - 長期質量趨勢追蹤

## 快速開始

### 1. 初始化Code Review API

```cpp
// 獲取Code Review API實例
UMingRTSCodeReviewAPI* CodeReviewAPI = APISystem->GetCodeReviewAPI();
CodeReviewAPI->InitializeCodeReviewSystem();

// 綁定事件
CodeReviewAPI->OnReviewStarted.AddDynamic(this, &AMyGameMode::OnReviewStarted);
CodeReviewAPI->OnIssueFound.AddDynamic(this, &AMyGameMode::OnIssueFound);
CodeReviewAPI->OnReviewCompleted.AddDynamic(this, &AMyGameMode::OnReviewCompleted);
CodeReviewAPI->OnCommitValidated.AddDynamic(this, &AMyGameMode::OnCommitValidated);
```

### 2. 執行完整代碼審查

```cpp
// 指定要檢查的文件
TArray<FString> FilesToReview;
FilesToReview.Add(TEXT("Source/MingGoRTS/Public/API/MingRTSCodeReviewAPI.h"));
FilesToReview.Add(TEXT("Source/MingGoRTS/Private/API/MingRTSCodeReviewAPI.cpp"));

// 執行審查
FCodeReviewReport Report = CodeReviewAPI->PerformCodeReview(FilesToReview);

// 檢查結果
if (Report.bCanCommit)
{
    UE_LOG(LogTemp, Log, TEXT("代碼可以提交"));
}
else
{
    UE_LOG(LogTemp, Warning, TEXT("代碼需要修復後才能提交"));
    UE_LOG(LogTemp, Warning, TEXT("嚴重問題: %d, 錯誤問題: %d"), 
        Report.Metrics.CriticalIssues, Report.Metrics.ErrorIssues);
}
```

### 3. 單獨檢查特定類型問題

```cpp
// 只檢查語法問題
TArray<FCodeReviewIssue> SyntaxIssues = CodeReviewAPI->CheckSyntax(FilePath);

// 只檢查安全問題
TArray<FCodeReviewIssue> SecurityIssues = CodeReviewAPI->CheckSecurity(FilePath);

// 只檢查性能問題
TArray<FCodeReviewIssue> PerformanceIssues = CodeReviewAPI->CheckPerformance(FilePath);
```

### 4. 自動修復問題

```cpp
// 修復單個問題
bool bFixed = CodeReviewAPI->AutoFixIssue(FilePath, IssueID);

// 批量修復所有可修復問題
TArray<FString> FixedIssues = CodeReviewAPI->AutoFixAllIssues(FilePath);

UE_LOG(LogTemp, Log, TEXT("修復了 %d 個問題"), FixedIssues.Num());
```

## 事件處理

### 審查事件回調

```cpp
UFUNCTION()
void AMyGameMode::OnReviewStarted(const FString& FilePath)
{
    UE_LOG(LogTemp, Log, TEXT("開始審查文件: %s"), *FilePath);
    
    // 可以在這裡顯示進度UI
    ShowReviewProgress(FilePath);
}

UFUNCTION()
void AMyGameMode::OnIssueFound(const FString& FilePath, const FCodeReviewIssue& Issue)
{
    UE_LOG(LogTemp, Warning, TEXT("發現問題: %s:%d - %s"), 
        *FilePath, Issue.LineNumber, *Issue.Description);
    
    // 實時顯示問題
    DisplayIssue(Issue);
}

UFUNCTION()
void AMyGameMode::OnReviewCompleted(const FString& FilePath, const FCodeReviewReport& Report)
{
    UE_LOG(LogTemp, Log, TEXT("審查完成: %s"), *FilePath);
    
    // 更新UI顯示結果
    UpdateReviewResults(Report);
}

UFUNCTION()
void AMyGameMode::OnCommitValidated(bool bCanCommit)
{
    if (bCanCommit)
    {
        UE_LOG(LogTemp, Log, TEXT("✅ 代碼可以提交"));
        ShowCommitReadyMessage();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ 代碼需要修復後才能提交"));
        ShowCommitBlockedMessage();
    }
}
```

## 配置和自定義

### 1. 設置審查閾值

```cpp
// 設置不同級別問題的閾值
CodeReviewAPI->SetReviewThresholds(
    0.0f,    // 嚴重問題閾值 (0個嚴重問題都不允許)
    5.0f,    // 錯誤問題閾值 (最多5個錯誤問題)
    10.0f    // 警告問題閾值 (最多10個警告問題)
);
```

### 2. 自定義審查規則

```cpp
// 創建自定義規則
FCodeReviewRule CustomRule;
CustomRule.RuleID = TEXT("CUSTOM_001");
CustomRule.RuleName = TEXT("自定義命名規範");
CustomRule.Description = TEXT("類名必須以M開頭");
CustomRule.Category = ECodeReviewCategory::Style;
CustomRule.DefaultSeverity = ECodeReviewSeverity::Warning;
CustomRule.bEnabled = true;
CustomRule.Pattern = TEXT("^class [^M]");
CustomRule.SuggestionTemplate = TEXT("類名應該以M開頭");

// 添加規則
CodeReviewAPI->UpdateReviewRule(CustomRule);
```

### 3. 獲取和修改現有規則

```cpp
// 獲取所有規則
TArray<FCodeReviewRule> Rules = CodeReviewAPI->GetReviewRules();

// 修改特定規則
for (FCodeReviewRule& Rule : Rules)
{
    if (Rule.RuleID == TEXT("STYLE_001"))
    {
        Rule.bEnabled = false;  // 禁用該規則
        CodeReviewAPI->UpdateReviewRule(Rule);
        break;
    }
}
```

## Git集成

### 1. Pre-commit Hook

系統已經配置了自動的pre-commit hook，每次提交前會自動執行Code Review：

```bash
# Hook會自動執行以下檢查：
# 1. 智能Code Review檢查
# 2. 語法檢查
# 3. 編碼檢查
# 4. 安全檢查
# 5. 分支檢查
# 6. 最終驗證

git add .
git commit -m "feat: 添加新功能"
# Hook會自動運行，如果發現問題會阻止提交
```

### 2. 手動執行Code Review

```bash
# 手動運行Code Review
powershell -ExecutionPolicy Bypass -File "Tools/api/SmartCommitReview.ps1"

# 嚴格模式運行
powershell -ExecutionPolicy Bypass -File "Tools/api/SmartCommitReview.ps1" -Strict

# 自動修復模式
powershell -ExecutionPolicy Bypass -File "Tools/api/SmartCommitReview.ps1" -AutoFix
```

## 報告系統

### 1. 生成HTML報告

```cpp
// 生成詳細的HTML報告
FString ReportContent = CodeReviewAPI->GenerateReviewReport(Report);

// 保存報告
FString ReportPath = FString::Printf(TEXT("Reports/CodeReview_%s.html"), 
    *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
FFileHelper::SaveStringToFile(ReportContent, *ReportPath);
```

### 2. 導出JSON格式

```cpp
// 導出JSON格式的審查結果
FString JsonContent = CodeReviewAPI->ExportReviewResults(Report);

// 保存JSON文件
FString JsonPath = FString::Printf(TEXT("Reports/CodeReview_%s.json"), 
    *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
FFileHelper::SaveStringToFile(JsonContent, *JsonPath);
```

### 3. 報告內容解析

```cpp
// 解析報告指標
FCodeReviewMetrics Metrics = Report.Metrics;

UE_LOG(LogTemp, Log, TEXT("代碼質量報告:"));
UE_LOG(LogTemp, Log, TEXT("  總問題數: %d"), Metrics.TotalIssues);
UE_LOG(LogTemp, Log, TEXT("  嚴重問題: %d"), Metrics.CriticalIssues);
UE_LOG(LogTemp, Log, TEXT("  錯誤問題: %d"), Metrics.ErrorIssues);
UE_LOG(LogTemp, Log, TEXT("  警告問題: %d"), Metrics.WarningIssues);
UE_LOG(LogTemp, Log, TEXT("  代碼質量分數: %.1f"), Metrics.CodeQualityScore);
UE_LOG(LogTemp, Log, TEXT("  可維護性分數: %.1f"), Metrics.MaintainabilityScore);
UE_LOG(LogTemp, Log, TEXT("  安全分數: %.1f"), Metrics.SecurityScore);
```

## 最佳實踐

### 1. 開發流程集成

```cpp
// 在編譯前運行Code Review
void AMyGameMode::PreBuildCheck()
{
    TArray<FString> SourceFiles = GetAllSourceFiles();
    FCodeReviewReport Report = CodeReviewAPI->PerformCodeReview(SourceFiles);
    
    if (!Report.bCanCommit)
    {
        UE_LOG(LogTemp, Error, TEXT("編譯前檢查失敗，請修復問題"));
        return false;
    }
    
    return true;
}
```

### 2. CI/CD集成

```cpp
// 在持續集成中使用
bool RunCIQualityGate()
{
    // 獲取所有修改的文件
    TArray<FString> ChangedFiles = GetChangedFilesInCI();
    
    // 執行Code Review
    FCodeReviewReport Report = CodeReviewAPI->PerformCodeReview(ChangedFiles);
    
    // 質量門檻檢查
    if (Report.Metrics.CodeQualityScore < 80.0f)
    {
        UE_LOG(LogTemp, Error, TEXT("CI質量門檻失敗: 代碼質量分數 %.1f < 80"), 
            Report.Metrics.CodeQualityScore);
        return false;
    }
    
    if (Report.Metrics.CriticalIssues > 0)
    {
        UE_LOG(LogTemp, Error, TEXT("CI質量門檻失敗: 發現 %d 個嚴重問題"), 
            Report.Metrics.CriticalIssues);
        return false;
    }
    
    return true;
}
```

### 3. 團隊協作

```cpp
// 團隊Code Review流程
void TeamCodeReviewWorkflow()
{
    // 1. 開發者本地檢查
    FCodeReviewReport LocalReport = CodeReviewAPI->PerformCodeReview(GetLocalChanges());
    
    // 2. 自動修復可修復問題
    CodeReviewAPI->AutoFixAllIssues(GetLocalChanges());
    
    // 3. 生成報告供團隊審查
    FString ReportPath = GenerateTeamReviewReport(LocalReport);
    
    // 4. 提交到代碼庫
    if (LocalReport.bCanCommit)
    {
        CommitChanges();
    }
    else
    {
        ShowReviewIssues(LocalReport);
    }
}
```

## 故障排除

### 常見問題

1. **PowerShell執行策略問題**
   ```powershell
   # 設置執行策略
   Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
   ```

2. **Git Hook不執行**
   ```bash
   # 確保hook有執行權限
   chmod +x .git/hooks/pre-commit
   ```

3. **Code Review API初始化失敗**
   ```cpp
   // 檢查API系統是否正確初始化
   if (!APISystem)
   {
       UE_LOG(LogTemp, Error, TEXT("API系統未初始化"));
       return;
   }
   ```

### 調試技巧

```cpp
// 啟用詳細日誌
void EnableCodeReviewDebugging()
{
    CodeReviewAPI->SetVerboseMode(true);
    
    // 監控所有事件
    CodeReviewAPI->OnIssueFound.AddDynamic(this, &AMyGameMode::DebugIssueFound);
}

UFUNCTION()
void AMyGameMode::DebugIssueFound(const FString& FilePath, const FCodeReviewIssue& Issue)
{
    UE_LOG(LogTemp, VeryVerbose, TEXT("[Debug] %s:%d - %s [%s]"), 
        *FilePath, Issue.LineNumber, *Issue.Description, 
        *StaticEnum<ECodeReviewSeverity>()->GetNameStringByValue((int64)Issue.Severity));
}
```

## 性能優化

### 1. 並行檢查

```cpp
// 並行檢查多個文件
void ParallelCodeReview(const TArray<FString>& Files)
{
    // 將文件分組並行檢查
    const int32 BatchSize = 4;
    TArray<TFuture<FCodeReviewReport>> Futures;
    
    for (int32 i = 0; i < Files.Num(); i += BatchSize)
    {
        TArray<FString> Batch;
        for (int32 j = 0; j < BatchSize && (i + j) < Files.Num(); j++)
        {
            Batch.Add(Files[i + j]);
        }
        
        TFuture<FCodeReviewReport> Future = AsyncRun(FindRunnableThread(), [this, Batch]()
        {
            return CodeReviewAPI->PerformCodeReview(Batch);
        });
        
        Futures.Add(Future);
    }
    
    // 等待所有檢查完成
    TArray<FCodeReviewReport> Reports;
    for (TFuture<FCodeReviewReport>& Future : Futures)
    {
        Reports.Add(Future.Get());
    }
    
    // 合併報告
    MergeReviewReports(Reports);
}
```

### 2. 增量檢查

```cpp
// 只檢查修改的文件
FCodeReviewReport IncrementalReview()
{
    TArray<FString> ModifiedFiles = GetGitModifiedFiles();
    
    // 過濾只檢查源代碼文件
    TArray<FString> SourceFiles;
    for (const FString& File : ModifiedFiles)
    {
        if (File.EndsWith(TEXT(".h")) || File.EndsWith(TEXT(".cpp")))
        {
            SourceFiles.Add(File);
        }
    }
    
    return CodeReviewAPI->PerformCodeReview(SourceFiles);
}
```

## 未來擴展

### 計劃功能
- **AI增強檢查** - 使用機器學習檢測更複雜問題
- **多語言支持** - 支持更多編程語言
- **雲端檢查** - 分佈式代碼審查服務
- **實時協作** - 多人實時代碼審查

### 技術改進
- **更強大的正則表達式** - 支持複雜模式匹配
- **語法樹分析** - 深度語法結構分析
- **性能分析** - 更精確的性能問題檢測
- **安全掃描** - 更全面的安全漏洞檢測

## 總結

Code Review API 為MingGoRTS提供了全面的代碼質量保障：

- **自動化檢查** - 無需手動執行，自動集成到開發流程
- **智能修復** - 自動修復常見問題，提高開發效率
- **質量門檻** - 確保代碼質量達到標準才能提交
- **詳細報告** - 提供全面的問題分析和修復建議
- **團隊協作** - 統一的代碼質量標準和審查流程

該系統顯著提升了代碼質量，減少了bug數量，提高了開發效率，是現代軟件開發不可或缺的工具。
