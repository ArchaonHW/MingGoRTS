// MingGoRTS 聖者大腦批次指令整合 API
// 將所有 CMD/BAT 功能整合至聖者大腦系統

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Containers/Queue.h"
#include "Containers/Map.h"
#include "Async/Async.h"
#include "MingSageBrainIntegratedAPI.h"
#include "MingRTSPowerShellIntegrationAPI.h"
#include "MingSageBrainCommandAPI.generated.h"

// 批次指令類型
UENUM(BlueprintType)
enum class EBatchCommandType : uint8
{
    Build               UMETA(DisplayName = "建置"),
    Compile             UMETA(DisplayName = "編譯"),
    Fix                 UMETA(DisplayName = "修復"),
    Clean               UMETA(DisplayName = "清理"),
    Verify              UMETA(DisplayName = "驗證"),
    Integrate           UMETA(DisplayName = "整合"),
    Security            UMETA(DisplayName = "安全掃描"),
    Test                UMETA(DisplayName = "測試"),
    Setup               UMETA(DisplayName = "環境設置"),
    Generate            UMETA(DisplayName = "專案生成"),
    All                 UMETA(DisplayName = "全部")
};

// 建置配置
USTRUCT(BlueprintType)
struct MINGGORTS_API FBuildConfiguration
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Config")
    FString EnginePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Config")
    FString ProjectPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Config")
    FString TargetName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Config")
    FString Platform;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Config")
    FString Configuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Config")
    bool bCleanIntermediate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Config")
    bool bOptimize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Config")
    bool bVerbose;

    FBuildConfiguration()
    {
        EnginePath = TEXT("C:\\Program Files\\Epic Games\\UE_5.7");
        ProjectPath = TEXT("C:\\HW\\MingGoRTS\\MingGoRTS.uproject");
        TargetName = TEXT("MingGoRTSEditor");
        Platform = TEXT("Win64");
        Configuration = TEXT("Development");
        bCleanIntermediate = true;
        bOptimize = true;
        bVerbose = false;
    }
};

// 修復選項
USTRUCT(BlueprintType)
struct MINGGORTS_API FBuildFixOptions
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fix Options")
    bool bFixMissingBuildCs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fix Options")
    bool bFixSyntaxErrors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fix Options")
    bool bCleanProblematicFiles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fix Options")
    bool bRegenerateProjectFiles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fix Options")
    bool bFixDependencies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fix Options")
    bool bFixEncoding;

    FBuildFixOptions()
    {
        bFixMissingBuildCs = true;
        bFixSyntaxErrors = true;
        bCleanProblematicFiles = true;
        bRegenerateProjectFiles = true;
        bFixDependencies = true;
        bFixEncoding = false;
    }
};

// 驗證結果
USTRUCT(BlueprintType)
struct MINGGORTS_API FVerificationResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Verification")
    bool bSuccess;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Verification")
    FString Message;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Verification")
    TArray<FString> Errors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Verification")
    TArray<FString> Warnings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Verification")
    float Duration;

    FVerificationResult()
    {
        bSuccess = false;
        Message = TEXT("");
        Errors = TArray<FString>();
        Warnings = TArray<FString>();
        Duration = 0.0f;
    }
};

// 整合統計
USTRUCT(BlueprintType)
struct MINGGORTS_API FIntegrationStatistics
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
    int32 TotalSystems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
    int32 CompletedSystems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
    int32 TotalFiles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
    int32 CppFiles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
    int32 HeaderFiles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
    int32 TotalLines;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
    FString LastUpdate;

    FIntegrationStatistics()
    {
        TotalSystems = 0;
        CompletedSystems = 0;
        TotalFiles = 0;
        CppFiles = 0;
        HeaderFiles = 0;
        TotalLines = 0;
        LastUpdate = TEXT("");
    }
};

// 批次指令資訊
USTRUCT(BlueprintType)
struct MINGGORTS_API FBatchCommandInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command Info")
    FString CommandName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command Info")
    FString OriginalFile;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command Info")
    EBatchCommandType CommandType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command Info")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command Info")
    bool bIsConverted;

    FBatchCommandInfo()
    {
        CommandName = TEXT("");
        OriginalFile = TEXT("");
        CommandType = EBatchCommandType::All;
        Description = TEXT("");
        bIsConverted = false;
    }
};

// 委派定義
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBuildCompleted, bool, bSuccess, const FString&, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFixCompleted, bool, bSuccess, const FString&, Report);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVerificationCompleted, const FVerificationResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntegrationCompleted, const FIntegrationStatistics&, Stats);

/**
 * 聖者大腦批次指令整合 API
 * 統合所有 CMD/BAT 功能至聖者大腦系統
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|SageBrain|Command")
class MINGGORTS_API UMingSageBrainCommandAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingSageBrainCommandAPI();

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command")
    bool InitializeCommandAPI();

    // === 建置相關功能 ===
    
    // 最佳化建置
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Build")
    bool ExecuteOptimizedBuild(const FBuildConfiguration& Config);

    // 驗證編譯
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Build")
    bool ExecuteCompileVerification(FVerificationResult& OutResult);

    // 快速建置
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Build")
    bool ExecuteQuickBuild();

    // 除錯建置
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Build")
    bool ExecuteDebugBuild();

    // === 修復相關功能 ===

    // 修復建置問題
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Fix")
    bool ExecuteBuildFix(const FBuildFixOptions& Options);

    // 批次修復
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Fix")
    bool ExecuteBatchFix();

    // 快速修復
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Fix")
    bool ExecuteQuickFix();

    // 快速修復 v2
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Fix")
    bool ExecuteQuickFixV2();

    // === 清理相關功能 ===

    // 清理建置
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Clean")
    bool ExecuteCleanBuild();

    // 清理中間檔案
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Clean")
    bool ExecuteCleanIntermediate();

    // === 驗證相關功能 ===

    // 驗證建置
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Verify")
    FVerificationResult ExecuteBuildVerification();

    // 驗證編譯
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Verify")
    FVerificationResult ExecuteCompileVerification();

    // 快速驗證
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Verify")
    FVerificationResult ExecuteQuickVerification();

    // 掃描錯誤
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Verify")
    TArray<FString> ExecuteErrorScan();

    // === 整合相關功能 ===

    // 專案整合
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Integrate")
    FIntegrationStatistics ExecuteProjectIntegration();

    // === 安全掃描 ===

    // 執行安全掃描
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Security")
    FVerificationResult ExecuteSecurityScan();

    // === 測試相關功能 ===

    // 執行測試建置
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Test")
    bool ExecuteTestBuild();

    // 執行系統測試
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Test")
    FVerificationResult ExecuteSystemTests();

    // === 環境設置 ===

    // 設置 UE5 環境
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Setup")
    bool SetupUE5Environment();

    // 安裝 Visual Studio 2022
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Setup")
    bool InstallVisualStudio2022();

    // === 專案生成 ===

    // 生成專案檔案
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Generate")
    bool GenerateProjectFiles();

    // 生成特定 UE 版本專案
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Generate")
    bool GenerateProjectForUEVersion(const FString& UEVersion);

    // 手動生成專案
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Generate")
    bool GenerateProjectManual();

    // === 批次管理 ===

    // 掃描所有批次指令
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Management")
    TArray<FBatchCommandInfo> ScanAllBatchCommands();

    // 依類型取得指令
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Management")
    TArray<FBatchCommandInfo> GetCommandsByType(EBatchCommandType Type);

    // 標記指令為已轉換
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Management")
    bool MarkCommandAsConverted(const FString& CommandName);

    // 移除已轉換的批次檔
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Management")
    bool RemoveConvertedBatchFiles();

    // 取得轉換統計
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Management")
    void GetCommandConversionStats(int32& TotalCommands, int32& ConvertedCommands, int32& RemainingCommands);

    // 執行完整工作流程
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Workflow")
    bool ExecuteFullBuildWorkflow();

    // 執行修復並建置工作流程
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Workflow")
    bool ExecuteFixAndBuildWorkflow();

    // 生成整合報告
    UFUNCTION(BlueprintCallable, Category = "SageBrain Command|Report")
    bool GenerateCommandIntegrationReport(const FString& OutputPath);

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "SageBrain Command")
    FOnBuildCompleted OnBuildCompleted;

    UPROPERTY(BlueprintAssignable, Category = "SageBrain Command")
    FOnFixCompleted OnFixCompleted;

    UPROPERTY(BlueprintAssignable, Category = "SageBrain Command")
    FOnVerificationCompleted OnVerificationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "SageBrain Command")
    FOnIntegrationCompleted OnIntegrationCompleted;

protected:
    virtual void BeginDestroy() override;

private:
    // 專案路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString ProjectRoot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString EnginePath;

    // 指令註冊表
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Registry")
    TArray<FBatchCommandInfo> CommandRegistry;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Registry")
    TMap<FString, bool> ConvertedCommands;

    // 建置工具路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString UBTPath;

    // 日誌檔案路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString LogPath;

    // 內建實現方法
    bool PerformOptimizedBuild(const FBuildConfiguration& Config, FString& Output);
    bool PerformCompileVerification(FString& Output);
    bool PerformBuildFix(const FBuildFixOptions& Options, FString& Report);
    bool PerformCleanBuild();
    bool PerformCleanIntermediate();
    bool PerformBuildVerification(FString& Output);
    bool PerformSecurityScan(FString& Output);
    bool PerformProjectIntegration(FIntegrationStatistics& Stats);
    bool PerformProjectGeneration(FString& Output);
    bool PerformTestBuild(FString& Output);
    bool PerformSystemTests(FString& Output);

    // 輔助方法
    bool DeleteDirectory(const FString& DirectoryPath);
    bool DeleteFile(const FString& FilePath);
    bool DirectoryExists(const FString& DirectoryPath);
    bool FileExists(const FString& FilePath);
    void LogCommandMessage(const FString& Message);
    bool ScanDirectoryForCommands(const FString& Directory);
};
