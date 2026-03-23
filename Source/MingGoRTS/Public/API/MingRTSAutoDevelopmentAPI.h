// MingGoRTS 自動化開發系統 API
// 轉換自 PowerShell: AutoDevelopmentSystem.ps1

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "Misc/DateTime.h"
#include "Dom/JsonObject.h"
#include "Templates/SharedPointer.h"
#include "MingRTSAutoDevelopmentAPI.generated.h"

// 開發模式
UENUM(BlueprintType)
enum class EAutoDevelopmentMode : uint8
{
    Full        UMETA(DisplayName = "完整模式"),
    BuildOnly   UMETA(DisplayName = "僅編譯"),
    TestOnly    UMETA(DisplayName = "僅測試"),
    DeployOnly  UMETA(DisplayName = "僅部署"),
    Quick       UMETA(DisplayName = "快速模式")
};

// 開發目標
UENUM(BlueprintType)
enum class EAutoDevelopmentTarget : uint8
{
    Development     UMETA(DisplayName = "開發版本"),
    Testing         UMETA(DisplayName = "測試版本"),
    Staging         UMETA(DisplayName = "預發布版本"),
    Production      UMETA(DisplayName = "生產版本"),
    Debug           UMETA(DisplayName = "調試版本")
};

// 執行階段狀態
UENUM(BlueprintType)
enum class EDevelopmentPhaseStatus : uint8
{
    Pending     UMETA(DisplayName = "待執行"),
    Running     UMETA(DisplayName = "執行中"),
    Completed   UMETA(DisplayName = "已完成"),
    Failed      UMETA(DisplayName = "失敗"),
    Skipped     UMETA(DisplayName = "已跳過")
};

// 程式碼品質問題
USTRUCT(BlueprintType)
struct MINGGORTS_API FCodeQualityIssue
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Quality")
    FString FilePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Quality")
    FString IssueType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Quality")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Quality")
    int32 LineNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Quality")
    FString Suggestion;

    FCodeQualityIssue()
    {
        FilePath = TEXT("");
        IssueType = TEXT("");
        Description = TEXT("");
        LineNumber = 0;
        Suggestion = TEXT("");
    }
};

// 測試結果
USTRUCT(BlueprintType)
struct MINGGORTS_API FAutoTestResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Result")
    FString TestName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Result")
    bool bPassed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Result")
    FString ErrorMessage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Result")
    float ExecutionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Result")
    FDateTime Timestamp;

    FAutoTestResult()
    {
        TestName = TEXT("");
        bPassed = false;
        ErrorMessage = TEXT("");
        ExecutionTime = 0.0f;
        Timestamp = FDateTime::Now();
    }
};

// 效能指標
USTRUCT(BlueprintType)
struct MINGGORTS_API FPerformanceMetrics
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    FDateTime Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    float CPUUsagePercent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    float MemoryUsageMB;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    float DiskUsageGB;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    float CompileTimeSeconds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    float TestTimeSeconds;

    FPerformanceMetrics()
    {
        Timestamp = FDateTime::Now();
        CPUUsagePercent = 0.0f;
        MemoryUsageMB = 0.0f;
        DiskUsageGB = 0.0f;
        CompileTimeSeconds = 0.0f;
        TestTimeSeconds = 0.0f;
    }
};

// 開發階段
USTRUCT(BlueprintType)
struct MINGGORTS_API FDevelopmentPhase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Phase")
    FString PhaseName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Phase")
    EDevelopmentPhaseStatus Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Phase")
    float StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Phase")
    float EndTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Phase")
    FString LogOutput;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Phase")
    bool bCanSkip;

    FDevelopmentPhase()
    {
        PhaseName = TEXT("");
        Status = EDevelopmentPhaseStatus::Pending;
        StartTime = 0.0f;
        EndTime = 0.0f;
        LogOutput = TEXT("");
        bCanSkip = false;
    }
};

// 開發執行結果
USTRUCT(BlueprintType)
struct MINGGORTS_API FAutoDevelopmentResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Result")
    bool bOverallSuccess;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Result")
    EAutoDevelopmentMode Mode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Result")
    EAutoDevelopmentTarget Target;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Result")
    FDateTime StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Result")
    FDateTime EndTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Result")
    TArray<FDevelopmentPhase> Phases;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Result")
    TArray<FCodeQualityIssue> QualityIssues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Result")
    TArray<FAutoTestResult> TestResults;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Result")
    FPerformanceMetrics Performance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Result")
    int32 TotalSourceFiles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Result")
    int32 TotalTestFiles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development Result")
    FString ReportPath;

    FAutoDevelopmentResult()
    {
        bOverallSuccess = false;
        Mode = EAutoDevelopmentMode::Full;
        Target = EAutoDevelopmentTarget::Development;
        StartTime = FDateTime::Now();
        EndTime = FDateTime::Now();
        Phases = TArray<FDevelopmentPhase>();
        QualityIssues = TArray<FCodeQualityIssue>();
        TestResults = TArray<FAutoTestResult>();
        Performance = FPerformanceMetrics();
        TotalSourceFiles = 0;
        TotalTestFiles = 0;
        ReportPath = TEXT("");
    }
};

// 委派
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDevelopmentPhaseStarted, const FString&, PhaseName, EAutoDevelopmentMode, Mode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDevelopmentPhaseCompleted, const FString&, PhaseName, bool, bSuccess, const FString&, Output);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAutoDevelopmentCompleted, const FAutoDevelopmentResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCodeQualityIssueFound, const FString&, FilePath, const FString&, IssueType);

/**
 * MingGoRTS 自動化開發系統 API
 * 轉換自 PowerShell: AutoDevelopmentSystem.ps1
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|API|Auto Development")
class MINGGORTS_API UMingRTSAutoDevelopmentAPI : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingRTSAutoDevelopmentAPI();

    // 初始化自動化開發系統
    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    bool InitializeAutoDevelopmentSystem();

    // 執行完整自動化開發流程
    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    FAutoDevelopmentResult ExecuteFullAutomation(
        EAutoDevelopmentMode Mode = EAutoDevelopmentMode::Full,
        EAutoDevelopmentTarget Target = EAutoDevelopmentTarget::Development
    );

    // 各階段執行函數
    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    bool TestDevelopmentEnvironment();

    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    TArray<FCodeQualityIssue> InvokeCodeQualityCheck();

    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    bool InvokeAutoBuild(EAutoDevelopmentTarget Target = EAutoDevelopmentTarget::Development);

    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    TArray<FAutoTestResult> InvokeAutoTest();

    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    bool InvokeAutoDeploy(EAutoDevelopmentTarget Target = EAutoDevelopmentTarget::Development);

    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    void InvokeGitAutomation();

    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    FPerformanceMetrics InvokePerformanceMonitoring();

    // 報告生成
    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    bool GenerateDevelopmentReport(const FAutoDevelopmentResult& Result);

    // 配置管理
    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    void SetSkipBuild(bool bSkip) { bSkipBuild = bSkip; }

    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    void SetSkipTest(bool bSkip) { bSkipTest = bSkip; }

    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    void SetSkipDeploy(bool bSkip) { bSkipDeploy = bSkip; }

    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    bool ShouldSkipBuild() const { return bSkipBuild; }

    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    bool ShouldSkipTest() const { return bSkipTest; }

    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    bool ShouldSkipDeploy() const { return bSkipDeploy; }

    // 統計資訊
    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    int32 GetTotalSourceFiles() const { return TotalSourceFiles; }

    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    int32 GetTotalTestFiles() const { return TotalTestFiles; }

    UFUNCTION(BlueprintCallable, Category = "Auto Development")
    TArray<FString> GetRecentLogs(int32 Count = 10);

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Auto Development")
    FOnDevelopmentPhaseStarted OnDevelopmentPhaseStarted;

    UPROPERTY(BlueprintAssignable, Category = "Auto Development")
    FOnDevelopmentPhaseCompleted OnDevelopmentPhaseCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Auto Development")
    FOnAutoDevelopmentCompleted OnAutoDevelopmentCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Auto Development")
    FOnCodeQualityIssueFound OnCodeQualityIssueFound;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    // 配置屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    bool bSkipBuild = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    bool bSkipTest = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    bool bSkipDeploy = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString ProjectRoot = TEXT("C:\\HW\\MingGoRTS");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString BuildDir = TEXT("Build");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString LogDir = TEXT("Logs");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString TempDir = TEXT("Temp");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString DeployDir = TEXT("Build\\Deploy");

    // 執行狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution State")
    EAutoDevelopmentMode CurrentMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution State")
    EAutoDevelopmentTarget CurrentTarget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution State")
    TArray<FString> ExecutionLogs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution State")
    int32 TotalSourceFiles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution State")
    int32 TotalTestFiles;

    // 內部方法
    void InitializeDirectories();
    void LogMessage(const FString& Message, const FString& Level = TEXT("INFO"));
    
    bool CheckUE5Installation();
    bool CheckVisualStudioInstallation();
    bool CheckGitInstallation();
    
    TArray<FString> GetSourceFiles();
    TArray<FString> GetTestFiles();
    
    FCodeQualityIssue AnalyzeFileQuality(const FString& FilePath);
    void RecordPerformanceMetrics(FPerformanceMetrics& Metrics);
    
    bool GenerateDeploymentPackage(const FString& TargetDir);
    void CreateDeployInfoFile(const FString& DeployPath, EAutoDevelopmentTarget Target);
    
    void CommitGitChanges();
    void PushGitChanges();
    
    FString GenerateReportJSON(const FAutoDevelopmentResult& Result);
    void SaveMetricsToFile(const FPerformanceMetrics& Metrics);
    
    void UpdatePhaseStatus(FDevelopmentPhase& Phase, EDevelopmentPhaseStatus NewStatus);
    FDevelopmentPhase CreatePhase(const FString& PhaseName, bool bCanSkip = false);
};
