// MingGoRTS 統合 PowerShell 腳本管理 API
// 整合專案內所有 PowerShell 腳本功能

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "Misc/DateTime.h"
#include "Dom/JsonObject.h"
#include "Templates/SharedPointer.h"
#include "MingRTSPowerShellIntegrationAPI.generated.h"

// PowerShell 腳本分類
UENUM(BlueprintType)
enum class EPowerShellScriptCategory : uint8
{
    Build           UMETA(DisplayName = "建置"),
    Security        UMETA(DisplayName = "安全掃描"),
    Testing         UMETA(DisplayName = "測試"),
    Installation    UMETA(DisplayName = "安裝"),
    Quality         UMETA(DisplayName = "品質檢查"),
    Requirements    UMETA(DisplayName = "需求管理"),
    Content         UMETA(DisplayName = "內容生成"),
    Version         UMETA(DisplayName = "版本控制"),
    All             UMETA(DisplayName = "全部")
};

// 腳本執行狀態
UENUM(BlueprintType)
enum class EScriptExecutionStatus : uint8
{
    Pending     UMETA(DisplayName = "待執行"),
    Running     UMETA(DisplayName = "執行中"),
    Completed   UMETA(DisplayName = "已完成"),
    Failed      UMETA(DisplayName = "失敗"),
    Cancelled   UMETA(DisplayName = "已取消")
};

// 腳本資訊結構
USTRUCT(BlueprintType)
struct MINGGORTS_API FPowerShellScriptInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Info")
    FString ScriptName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Info")
    FString FilePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Info")
    EPowerShellScriptCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Info")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Info")
    TArray<FString> Parameters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Info")
    bool bIsConverted;

    FPowerShellScriptInfo()
    {
        ScriptName = TEXT("");
        FilePath = TEXT("");
        Category = EPowerShellScriptCategory::All;
        Description = TEXT("");
        Parameters = TArray<FString>();
        bIsConverted = false;
    }
};

// 腳本執行結果
USTRUCT(BlueprintType)
struct MINGGORTS_API FScriptExecutionResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution Result")
    FString ScriptName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution Result")
    EScriptExecutionStatus Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution Result")
    FString Output;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution Result")
    FString Error;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution Result")
    float ExecutionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution Result")
    FDateTime Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution Result")
    int32 ExitCode;

    FScriptExecutionResult()
    {
        ScriptName = TEXT("");
        Status = EScriptExecutionStatus::Pending;
        Output = TEXT("");
        Error = TEXT("");
        ExecutionTime = 0.0f;
        Timestamp = FDateTime::Now();
        ExitCode = 0;
    }
};

// 委派
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScriptExecutionCompleted, const FScriptExecutionResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScriptConverted, const FString&, ScriptName, bool, bSuccess);

/**
 * MingGoRTS PowerShell 腳本整合 API
 * 統合管理專案內所有 PowerShell 腳本
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|API|PowerShell")
class MINGGORTS_API UMingRTSPowerShellIntegrationAPI : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingRTSPowerShellIntegrationAPI();

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "PowerShell Integration")
    bool InitializePowerShellIntegration();

    // 掃描並收集所有 PowerShell 腳本
    UFUNCTION(BlueprintCallable, Category = "PowerShell Integration")
    TArray<FPowerShellScriptInfo> ScanAllPowerShellScripts();

    // 依分類取得腳本
    UFUNCTION(BlueprintCallable, Category = "PowerShell Integration")
    TArray<FPowerShellScriptInfo> GetScriptsByCategory(EPowerShellScriptCategory Category);

    // 執行特定腳本
    UFUNCTION(BlueprintCallable, Category = "PowerShell Integration")
    FScriptExecutionResult ExecuteScript(const FString& ScriptPath, const TMap<FString, FString>& Parameters);

    // 批次執行多個腳本
    UFUNCTION(BlueprintCallable, Category = "PowerShell Integration")
    TArray<FScriptExecutionResult> ExecuteScriptsBatch(const TArray<FString>& ScriptPaths, bool bParallel = false);

    // 標記腳本為已轉換
    UFUNCTION(BlueprintCallable, Category = "PowerShell Integration")
    bool MarkScriptAsConverted(const FString& ScriptPath);

    // 移除已轉換的腳本
    UFUNCTION(BlueprintCallable, Category = "PowerShell Integration")
    bool RemoveConvertedScripts();

    // 取得轉換統計
    UFUNCTION(BlueprintCallable, Category = "PowerShell Integration")
    void GetConversionStatistics(int32& TotalScripts, int32& ConvertedScripts, int32& RemainingScripts);

    // 內建功能替代腳本執行
    UFUNCTION(BlueprintCallable, Category = "PowerShell Integration")
    FScriptExecutionResult ExecuteBuildVerification();

    UFUNCTION(BlueprintCallable, Category = "PowerShell Integration")
    FScriptExecutionResult ExecuteSecurityScan();

    UFUNCTION(BlueprintCallable, Category = "PowerShell Integration")
    FScriptExecutionResult ExecuteCodeQualityCheck();

    UFUNCTION(BlueprintCallable, Category = "PowerShell Integration")
    FScriptExecutionResult ExecuteIntegrationTests();

    UFUNCTION(BlueprintCallable, Category = "PowerShell Integration")
    FScriptExecutionResult ExecuteVersionCommit(const FString& Version, const FString& Description);

    // 報告生成
    UFUNCTION(BlueprintCallable, Category = "PowerShell Integration")
    bool GenerateIntegrationReport(const FString& OutputPath);

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "PowerShell Integration")
    FOnScriptExecutionCompleted OnScriptExecutionCompleted;

    UPROPERTY(BlueprintAssignable, Category = "PowerShell Integration")
    FOnScriptConverted OnScriptConverted;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    // 腳本註冊表
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Registry")
    TArray<FPowerShellScriptInfo> ScriptRegistry;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Registry")
    TMap<FString, bool> ConvertedScripts;

    // 專案路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString ProjectRoot;

    // 掃描特定目錄
    void ScanDirectory(const FString& Directory, EPowerShellScriptCategory Category);

    // 解析腳本內容
    FPowerShellScriptInfo ParseScriptInfo(const FString& FilePath);

    // 執行 PowerShell 命令
    bool ExecutePowerShellCommand(const FString& Command, FString& Output, FString& Error, int32& ExitCode);

    // 記錄日誌
    void LogPowerShellMessage(const FString& Message);

    // 內建實現方法
    bool PerformBuildVerification(FString& Output);
    bool PerformSecurityScan(FString& Output);
    bool PerformCodeQualityCheck(FString& Output);
    bool PerformIntegrationTests(FString& Output);
    bool PerformVersionCommit(const FString& Version, const FString& Description, FString& Output);
};
