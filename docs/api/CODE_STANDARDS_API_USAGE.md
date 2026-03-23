# MingGoRTS 程式碼規範API使用指南

## 概述

MingGoRTS 程式碼規範API 提供了完整的代碼質量分析和規範檢查功能，支持命名規範、格式化規範、文檔規範等多種標準的檢查和自動修復。

## 主要功能

### 1. 代碼質量分析

```cpp
// 初始化API
UMingRTSCodeStandardsAPI* CodeStandardsAPI = NewObject<UMingRTSCodeStandardsAPI>();
CodeStandardsAPI->InitializeCodeStandards();

// 分析單個文件
FCodeQualityReport Report = CodeStandardsAPI->AnalyzeFile(TEXT("C:/Project/Source/MyClass.h"));

// 分析整個項目
TArray<FCodeQualityReport> ProjectReports = CodeStandardsAPI->AnalyzeProject(TEXT("C:/Project/Source"));

// 分析特定目錄
TArray<FCodeQualityReport> DirectoryReports = CodeStandardsAPI->AnalyzeDirectory(TEXT("C:/Project/Source/Core"));
```

### 2. 規範檢查

```cpp
// 檢查命名規範
TArray<FCodeViolation> NamingViolations = CodeStandardsAPI->CheckNamingStandards(TEXT("MyClass.h"));

// 檢查格式化規範
TArray<FCodeViolation> FormattingViolations = CodeStandardsAPI->CheckFormattingStandards(TEXT("MyClass.h"));
```

### 3. 自動修復

```cpp
// 自動修復單個文件
bool bFixed = CodeStandardsAPI->AutoFixCode(TEXT("MyClass.h"), Violations);

// 批量自動修復
TArray<FString> FilesToFix = {TEXT("File1.h"), TEXT("File2.cpp"), TEXT("File3.h")};
TArray<FString> FixedFiles = CodeStandardsAPI->BatchAutoFix(FilesToFix);
```

### 4. 報告生成

```cpp
// 生成文本報告
FString TextReport = CodeStandardsAPI->GenerateQualityReport(ProjectReports);

// 生成HTML報告
FString HTMLReport = CodeStandardsAPI->GenerateHTMLReport(ProjectReports);

// 導出報告到文件
bool bExported = CodeStandardsAPI->ExportReport(TextReport, TEXT("C:/Reports/QualityReport.txt"));
```

### 5. 規範配置

```cpp
// 添加命名規則
FNamingRule ClassNameRule;
ClassNameRule.Pattern = TEXT("^class [A-Z][a-zA-Z0-9]*");
ClassNameRule.Convention = ENamingConvention::PascalCase;
ClassNameRule.Description = TEXT("Class names should use PascalCase");
CodeStandardsAPI->AddNamingRule(ClassNameRule);

// 添加格式化規則
FFormattingRule IndentRule;
IndentRule.RuleName = TEXT("Indentation");
IndentRule.Pattern = TEXT("^( {4}|\t)");
IndentRule.Replacement = TEXT("    ");
IndentRule.Description = TEXT("Use 4 spaces for indentation");
CodeStandardsAPI->AddFormattingRule(IndentRule);

// 獲取和設置規範配置
FCodeStandardConfig Config = CodeStandardsAPI->GetStandardConfig(ECodeStandardType::Naming);
Config.bEnabled = true;
CodeStandardsAPI->SetStandardConfig(Config);
```

### 6. 實時監控

```cpp
// 啟用實時監控
CodeStandardsAPI->EnableRealTimeMonitoring(true);

// 設置監控間隔（秒）
CodeStandardsAPI->SetMonitoringInterval(30.0f);

// 檢查監控狀態
bool bIsActive = CodeStandardsAPI->IsMonitoringActive();
```

### 7. 事件處理

```cpp
// 綁定事件委託
CodeStandardsAPI->OnViolationFound.AddDynamic(this, &AMyActor::OnViolationFound);
CodeStandardsAPI->OnQualityReportGenerated.AddDynamic(this, &AMyActor::OnQualityReportGenerated);
CodeStandardsAPI->OnAutoFixCompleted.AddDynamic(this, &AMyActor::OnAutoFixCompleted);
CodeStandardsAPI->OnStandardViolation.AddDynamic(this, &AMyActor::OnStandardViolation);

// 事件處理函數
UFUNCTION()
void OnViolationFound(const FCodeViolation& Violation)
{
    UE_LOG(LogTemp, Warning, TEXT("Violation found: %s"), *Violation.Description);
}

UFUNCTION()
void OnQualityReportGenerated(const FCodeQualityReport& Report)
{
    UE_LOG(LogTemp, Log, TEXT("Quality report generated for: %s"), *Report.FilePath);
}

UFUNCTION()
void OnAutoFixCompleted(const FString& FilePath)
{
    UE_LOG(LogTemp, Log, TEXT("Auto fix completed for: %s"), *FilePath);
}

UFUNCTION()
void OnStandardViolation(ECodeStandardType StandardType, const FString& Message)
{
    UE_LOG(LogTemp, Warning, TEXT("Standard violation: %s - %s"), 
        *StaticEnum<ECodeStandardType>()->GetNameStringByValue((int64)StandardType), *Message);
}
```

### 8. 統計分析

```cpp
// 獲取代碼質量統計
TMap<ECodeQuality, int32> QualityStats = CodeStandardsAPI->GetQualityStatistics(ProjectReports);

// 獲取違規統計
TMap<ECodeStandardType, int32> ViolationStats = CodeStandardsAPI->GetViolationStatistics(ProjectReports);

// 獲取最常見違規
TArray<FString> CommonViolations = CodeStandardsAPI->GetMostCommonViolations(10);
```

### 9. 團隊協作

```cpp
// 分享規範配置給團隊成員
CodeStandardsAPI->ShareStandardsConfig(TEXT("Developer1"));

// 導入團隊規範
bool bImported = CodeStandardsAPI->ImportTeamStandards(TEXT("C:/TeamStandards/Config.json"));
```

### 10. 代碼審查

```cpp
// 審查代碼變更
TArray<FCodeViolation> ChangeViolations = CodeStandardsAPI->ReviewCodeChanges(TEXT("Changeset123"));

// 審查Pull Request
FCodeQualityReport PRReport = CodeStandardsAPI->ReviewPullRequest(TEXT("PR456"));
```

## 規範類型

### ECodeStandardType 枚舉

- `Naming`: 命名規範
- `Formatting`: 格式化規範
- `Documentation`: 文檔規範
- `Architecture`: 架構規範
- `Security`: 安全規範
- `Performance`: 性能規範
- `Testing`: 測試規範
- `VersionControl`: 版本控制規範
- `Deployment`: 部署規範
- `Custom`: 自定義規範

### EViolationSeverity 枚舉

- `Info`: 信息級別
- `Warning`: 警告級別
- `Error`: 錯誤級別
- `Critical`: 嚴重錯誤級別

### ECodeQuality 枚舉

- `Excellent`: 優秀 (90-100分)
- `Good`: 良好 (75-89分)
- `Average`: 一般 (60-74分)
- `Poor`: 較差 (40-59分)
- `Critical`: 嚴重 (0-39分)

## 最佳實踐

1. **初始化**: 在項目啟動時初始化Code Standards API
2. **配置**: 根據團隊需求配置適當的規範規則
3. **監控**: 啟用實時監控以及時發現問題
4. **自動修復**: 對可自動修復的問題使用批量修復功能
5. **報告**: 定期生成質量報告並跟蹤趨勢
6. **團隊協作**: 分享規範配置確保團隊一致性

## 示例工作流

```cpp
// 完整的代碼質量檢查工作流
void PerformCodeQualityCheck()
{
    // 1. 初始化API
    UMingRTSCodeStandardsAPI* API = NewObject<UMingRTSCodeStandardsAPI>();
    API->InitializeCodeStandards();
    
    // 2. 配置規範
    API->SetStandardEnabled(ECodeStandardType::Naming, true);
    API->SetStandardEnabled(ECodeStandardType::Formatting, true);
    
    // 3. 分析項目
    TArray<FCodeQualityReport> Reports = API->AnalyzeProject(FPaths::ProjectDir());
    
    // 4. 生成報告
    FString HTMLReport = API->GenerateHTMLReport(Reports);
    API->ExportReport(HTMLReport, FPaths::ProjectDir() / TEXT("Reports/QualityReport.html"));
    
    // 5. 自動修復
    TArray<FString> FilesToFix;
    for (const FCodeQualityReport& Report : Reports)
    {
        if (Report.QualityScore < 80.0f)
        {
            FilesToFix.Add(Report.FilePath);
        }
    }
    
    TArray<FString> FixedFiles = API->BatchAutoFix(FilesToFix);
    
    // 6. 輸出結果
    UE_LOG(LogTemp, Log, TEXT("Code quality check completed. Fixed %d files."), FixedFiles.Num());
}
```

## 注意事項

1. 確保在調用API前已正確初始化
2. 自動修復功能會直接修改文件，建議先備份
3. 實時監控會消耗一定性能，根據需要調整監控間隔
4. 規範配置建議通過團隊協作功能統一管理
5. 大型項目建議分批次進行分析和修復
