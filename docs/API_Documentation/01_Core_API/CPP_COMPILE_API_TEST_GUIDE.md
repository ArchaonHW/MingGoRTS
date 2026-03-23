# C++ 編譯 API 測試指南

## 概述

本指南將幫助您測試和使用新創建的 C++ 編譯 API 系統。

## 快速開始

### 1. 初始化編譯 API

```cpp
// 在您的遊戲模式或任何 Actor 中
#include "MingCompileAPI.h"

// 創建編譯 API 實例
UMingCompileAPI* CompileAPI = NewObject<UMingCompileAPI>();

// 初始化系統
CompileAPI->InitializeCompileAPI();

// 綁定事件（可選）
CompileAPI->OnCompileStarted.AddDynamic(this, &AMyActor::OnCompileStarted);
CompileAPI->OnCompileCompleted.AddDynamic(this, &AMyActor::OnCompileCompleted);
```

### 2. 配置編譯參數

```cpp
// 獲取默認配置
FMingCompileConfiguration Config = CompileAPI->GetDefaultConfiguration();

// 自定義配置
Config.CompileMode = EMingCompileMode::Development;
Config.Target = EMingCompileTarget::Game;
Config.ProjectPath = TEXT("C:\\HW\\MingGoRTS");
Config.OutputPath = TEXT("C:\\HW\\MingGoRTS\\Build\\Binaries");
Config.bCleanBuild = true;
Config.ParallelJobs = 8;
Config.AdditionalArguments.Add(TEXT("-logcommands"));
Config.AdditionalArguments.Add(TEXT("-frommsbuild"));
```

### 3. 執行編譯

```cpp
// 同步編譯
FMingCompileResult Result = CompileAPI->CompileProject(Config);

// 檢查結果
if (Result.Status == EMingCompileStatus::Success)
{
    UE_LOG(LogTemp, Log, TEXT("編譯成功！用時 %.2f 秒"), Result.CompileTime);
}
else
{
    UE_LOG(LogTemp, Error, TEXT("編譯失敗： %s"), *Result.ErrorMessage);
}

// 異步編譯
CompileAPI->CompileProjectAsync(Config);
```

### 4. 運行測試

```cpp
// 配置測試參數
FMingTestConfiguration TestConfig;
TestConfig.bRunUnitTests = true;
TestConfig.bRunIntegrationTests = true;
TestConfig.bRunPerformanceTests = false;
TestConfig.bRunStressTests = false;
TestConfig.TestTimeout = 300; // 5 分鐘

// 執行測試
FMingTestResult TestResult = CompileAPI->RunTests(TestConfig);

// 檢查測試結果
if (TestResult.bSuccess)
{
    UE_LOG(LogTemp, Log, TEXT("測試通過：%d/%d 通過"), TestResult.PassedTests, TestResult.TotalTests);
}
else
{
    UE_LOG(LogTemp, Error, TEXT("測試失敗：%d 個失敗"), TestResult.FailedTests);
}
```

## 完整測試範例

### 遊戲模式集成

```cpp
// 在您的遊戲模式中
AMyGameMode::AMyGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // 初始化編譯 API
    CompileAPI = NewObject<UMingCompileAPI>();
    CompileAPI->InitializeCompileAPI();
    
    // 綁定事件
    CompileAPI->OnCompileStarted.AddDynamic(this, &AMyGameMode::OnCompileStarted);
    CompileAPI->OnCompileProgress.AddDynamic(this, &AMyGameMode::OnCompileProgress);
    CompileAPI->OnCompileCompleted.AddDynamic(this, &AMyGameMode::OnCompileCompleted);
    CompileAPI->OnTestStarted.AddDynamic(this, &AMyGameMode::OnTestStarted);
    CompileAPI->OnTestProgress.AddDynamic(this, &AMyGameMode::OnTestProgress);
    CompileAPI->OnTestCompleted.AddDynamic(this, &AMyGameMode::OnTestCompleted);
}

// 事件處理函數
UFUNCTION()
void AMyGameMode::OnCompileStarted(const FMingCompileConfiguration& Configuration)
{
    UE_LOG(LogTemp, Log, TEXT("編譯開始：%s"), *Configuration.ProjectPath);
}

UFUNCTION()
void AMyGameMode::OnCompileProgress(float Progress, const FString& CurrentTask)
{
    UE_LOG(LogTemp, Log, TEXT("編譯進度：%.1f%% - %s"), Progress * 100.0f, *CurrentTask);
}

UFUNCTION()
void AMyGameMode::OnCompileCompleted(const FMingCompileResult& Result)
{
    if (Result.Status == EMingCompileStatus::Success)
    {
        UE_LOG(LogTemp, Log, TEXT("編譯完成！警告：%d，錯誤：%d"), Result.WarningCount, Result.ErrorCount);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("編譯失敗：%s"), *Result.ErrorMessage);
    }
}
```

### 藍圖集成

```cpp
// 在藍圖中使用編譯 API
UCLASS(BlueprintType, Blueprintable)
class MYGAME_API UMyBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Compilation")
    static bool CompileMingGoRTSProject(EMingCompileMode CompileMode, bool bCleanBuild)
    {
        UMingCompileAPI* CompileAPI = NewObject<UMingCompileAPI>();
        CompileAPI->InitializeCompileAPI();
        
        FMingCompileConfiguration Config = CompileAPI->GetDefaultConfiguration();
        Config.CompileMode = CompileMode;
        Config.bCleanBuild = bCleanBuild;
        
        FMingCompileResult Result = CompileAPI->CompileProject(Config);
        return Result.Status == EMingCompileStatus::Success;
    }

    UFUNCTION(BlueprintCallable, Category = "Compilation")
    static bool RunMingGoRTSTests(bool bUnitTests, bool bIntegrationTests)
    {
        UMingCompileAPI* CompileAPI = NewObject<UMingCompileAPI>();
        CompileAPI->InitializeCompileAPI();
        
        FMingTestConfiguration TestConfig;
        TestConfig.bRunUnitTests = bUnitTests;
        TestConfig.bRunIntegrationTests = bIntegrationTests;
        
        FMingTestResult Result = CompileAPI->RunTests(TestConfig);
        return Result.bSuccess;
    }
};
```

## 測試套件使用

### 運行所有測試

```cpp
// 在您的測試模式或任何 Actor 中
#include "MingCompileAPITest.h"

void AMyTestActor::BeginPlay()
{
    Super::BeginPlay();
    
    // 創建測試套件
    UMingCompileAPITest* TestSuite = NewObject<UMingCompileAPITest>();
    
    // 運行所有測試
    bool bAllTestsPassed = TestSuite->RunAllTests();
    
    if (bAllTestsPassed)
    {
        UE_LOG(LogTemp, Log, TEXT("所有測試通過！"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("部分測試失敗"));
    }
}
```

### 個別測試

```cpp
// 運行特定測試
UMingCompileAPITest* TestSuite = NewObject<UMingCompileAPITest>();

// 測試編譯 API 初始化
bool bInitTest = TestSuite->TestCompileAPIInitialization();

// 測試項目編譯
bool bCompileTest = TestSuite->TestProjectCompilation();

// 測試異步編譯
bool bAsyncTest = TestSuite->TestAsyncCompilation();

// 測試報告生成
bool bReportTest = TestSuite->TestReportGeneration();
```

## 配置選項

### 編譯模式

- **Debug**: 調試模式，包含調試信息
- **Development**: 開發模式，平衡性能和調試
- **Shipping**: 發布模式，最優化性能
- **Test**: 測試模式，包含測試功能

### 編譯目標

- **Editor**: 編輯器版本
- **Game**: 遊戲版本
- **Server**: 服務器版本

### 測試類型

- **單元測試**: 測試單個功能模塊
- **集成測試**: 測試模塊間交互
- **性能測試**: 測試性能指標
- **壓力測試**: 測試系統極限

## 故障排除

### 常見問題

1. **編譯失敗**
   - 檢查項目路徑是否正確
   - 確認 Unreal Build Tool 路徑存在
   - 檢查磁碟空間是否足夠

2. **測試失敗**
   - 確認測試可執行文件存在
   - 檢查測試超時設置
   - 驗證測試環境配置

3. **報告生成失敗**
   - 檢查報告目錄權限
   - 確認磁碟空間
   - 檢查文件路徑格式

### 調試技巧

1. **啟用詳細日誌**
   ```cpp
   // 在初始化前設置日誌級別
   UE_LOG(LogTemp, VeryVerbose, TEXT("編譯 API 詳細日誌啟用"));
   ```

2. **檢查進度事件**
   ```cpp
   // 綁定進度事件以監控編譯狀態
   CompileAPI->OnCompileProgress.AddDynamic(this, &AMyActor::OnCompileProgress);
   ```

3. **驗證配置**
   ```cpp
   // 在編譯前驗證配置
   if (!CompileAPI->ValidateProjectStructure(Config.ProjectPath))
   {
       UE_LOG(LogTemp, Error, TEXT("項目結構驗證失敗"));
       return;
   }
   ```

## 性能優化

### 編譯優化

1. **並行編譯**
   ```cpp
   Config.ParallelJobs = FMath::Clamp(FPlatformMisc::NumberOfCoresIncludingHyperthreads() / 2, 1, 8);
   ```

2. **增量編譯**
   ```cpp
   Config.bCleanBuild = false; // 只編譯更改的文件
   ```

3. **輸出優化**
   ```cpp
   Config.AdditionalArguments.Add(TEXT("-logcommands"));
   Config.AdditionalArguments.Add(TEXT("-frommsbuild"));
   ```

### 測試優化

1. **測試並行化**
   ```cpp
   // 在不同線程運行不同測試類型
   ```

2. **測試緩存**
   ```cpp
   // 重用測試環境和資源
   ```

## 進階用法

### 自動化工作流程

```cpp
// 完整的自動化編譯和測試流程
void AMyBuildManager::AutomatedBuildAndTest()
{
    UMingCompileAPI* CompileAPI = NewObject<UMingCompileAPI>();
    CompileAPI->InitializeCompileAPI();
    
    // 配置編譯
    FMingCompileConfiguration Config = CompileAPI->GetDefaultConfiguration();
    Config.CompileMode = EMingCompileMode::Shipping;
    Config.bCleanBuild = true;
    
    // 執行編譯
    FMingCompileResult CompileResult = CompileAPI->CompileProject(Config);
    
    if (CompileResult.Status == EMingCompileStatus::Success)
    {
        // 編譯成功，運行測試
        FMingTestConfiguration TestConfig;
        TestConfig.bRunUnitTests = true;
        TestConfig.bRunIntegrationTests = true;
        TestConfig.bRunPerformanceTests = true;
        
        FMingTestResult TestResult = CompileAPI->RunTests(TestConfig);
        
        if (TestResult.bSuccess)
        {
            // 生成報告
            FString ReportPath = FPaths::Combine(*FPaths::ProjectDir(), TEXT("Reports/BuildAndTestReport.html"));
            CompileAPI->GenerateCompileReport(CompileResult, ReportPath);
        }
    }
}
```

### CI/CD 集成

```cpp
// 在持續集成環境中使用
void AMyCIBuild::ExecuteCIBuild()
{
    UMingCompileAPI* CompileAPI = NewObject<UMingCompileAPI>();
    CompileAPI->InitializeCompileAPI();
    
    // 從環境變數獲取配置
    FMingCompileConfiguration Config;
    Config.CompileMode = GetCompileModeFromEnv();
    Config.Target = GetTargetFromEnv();
    Config.ProjectPath = GetProjectPathFromEnv();
    
    // 執行編譯
    FMingCompileResult Result = CompileAPI->CompileProject(Config);
    
    // 設置退出代碼
    int32 ExitCode = (Result.Status == EMingCompileStatus::Success) ? 0 : 1;
    FPlatformMisc::RequestExit(ExitCode);
}
```

## 總結

C++ 編譯 API 系統提供了強大而靈活的編譯和測試功能，完全取代了原有的 PowerShell 腳本系統。通過本指南，您可以快速上手並充分利用系統的所有功能。

關於更多詳細信息，請參考 API 文檔和源代碼註釋。
