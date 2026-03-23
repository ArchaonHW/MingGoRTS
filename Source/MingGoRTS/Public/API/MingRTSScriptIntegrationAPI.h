// MingGoRTS Script Integration API
// Converts PowerShell scripts to C++ integrated functionality
// Generated from PowerShell scripts conversion

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Dom/JsonObject.h"
#include "Templates/SharedPointer.h"
#include "MingRTSScriptIntegrationAPI.generated.h"

// Script execution modes
UENUM(BlueprintType)
enum class EScriptExecutionMode : uint8
{
    Sequential   UMETA(DisplayName = "Sequential"),
    Parallel      UMETA(DisplayName = "Parallel"),
    Pipeline      UMETA(DisplayName = "Pipeline"),
    Interactive  UMETA(DisplayName = "Interactive")
};

// Script categories
UENUM(BlueprintType)
enum class EScriptCategory : uint8
{
    Build         UMETA(DisplayName = "Build"),
    Automation    UMETA(DisplayName = "Automation"),
    AI            UMETA(DisplayName = "AI"),
    API           UMETA(DisplayName = "API"),
    Optimization  UMETA(DisplayName = "Optimization"),
    Debug         UMETA(DisplayName = "Debug"),
    Security      UMETA(DisplayName = "Security"),
    Maintenance   UMETA(DisplayName = "Maintenance"),
    All           UMETA(DisplayName = "All")
};

// Script execution status
UENUM(BlueprintType)
enum class EScriptExecutionStatus : uint8
{
    Pending       UMETA(DisplayName = "Pending"),
    Running       UMETA(DisplayName = "Running"),
    Completed     UMETA(DisplayName = "Completed"),
    Failed        UMETA(DisplayName = "Failed"),
    Cancelled     UMETA(DisplayName = "Cancelled"),
    Timeout       UMETA(DisplayName = "Timeout")
};

// Script metadata structure
USTRUCT(BlueprintType)
struct MINGGORTS_API FScriptMetadata
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Metadata")
    FString ScriptPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Metadata")
    EScriptCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Metadata")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Metadata")
    int32 Size;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Metadata")
    FDateTime LastModified;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Metadata")
    TArray<FString> Dependencies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Metadata")
    TArray<FString> Parameters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Metadata")
    float EstimatedTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Metadata")
    float SuccessRate;

    FScriptMetadata()
    {
        ScriptPath = TEXT("");
        Category = EScriptCategory::All;
        Name = TEXT("");
        Size = 0;
        LastModified = FDateTime::Now();
        Dependencies = TArray<FString>();
        Parameters = TArray<FString>();
        EstimatedTime = 5.0f;
        SuccessRate = 0.95f;
    }
};

// Script execution result
USTRUCT(BlueprintType)
struct MINGGORTS_API FScriptExecutionResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Result")
    FString ScriptName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Result")
    EScriptExecutionStatus Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Result")
    float ExecutionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Result")
    FString Output;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Result")
    FString ErrorMessage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Script Result")
    int32 ReturnCode;

    FScriptExecutionResult()
    {
        ScriptName = TEXT("");
        Status = EScriptExecutionStatus::Pending;
        ExecutionTime = 0.0f;
        Output = TEXT("");
        ErrorMessage = TEXT("");
        ReturnCode = 0;
    }
};

// Integration session data
USTRUCT(BlueprintType)
struct MINGGORTS_API FIntegrationSession
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Integration Session")
    FString SessionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Integration Session")
    EScriptExecutionMode Mode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Integration Session")
    EScriptCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Integration Session")
    FDateTime StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Integration Session")
    TArray<FScriptExecutionResult> Results;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Integration Session")
    bool bDryRun;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Integration Session")
    int32 SuccessCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Integration Session")
    int32 FailureCount;

    FIntegrationSession()
    {
        SessionID = FGuid::NewGuid().ToString();
        Mode = EScriptExecutionMode::Sequential;
        Category = EScriptCategory::All;
        StartTime = FDateTime::Now();
        Results = TArray<FScriptExecutionResult>();
        bDryRun = false;
        SuccessCount = 0;
        FailureCount = 0;
    }
};

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScriptExecutionStarted, const FString&, ScriptName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScriptExecutionProgress, const FString&, ScriptName, float, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScriptExecutionCompleted, const FString&, ScriptName, const FScriptExecutionResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntegrationSessionCompleted, const FIntegrationSession&, Session);

/**
 * MingGoRTS Script Integration API
 * Converts PowerShell script functionality to integrated C++ systems
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|API|Script Integration")
class MINGGORTS_API UMingRTSScriptIntegrationAPI : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingRTSScriptIntegrationAPI();

    // Initialize the script integration system
    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    bool InitializeScriptIntegration();

    // Execute scripts in different modes
    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    FIntegrationSession ExecuteScriptsSequential(EScriptCategory Category = EScriptCategory::All, bool bDryRun = false);

    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    FIntegrationSession ExecuteScriptsParallel(EScriptCategory Category = EScriptCategory::All, bool bDryRun = false);

    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    FIntegrationSession ExecuteScriptsPipeline(EScriptCategory Category = EScriptCategory::All, bool bDryRun = false);

    // Execute individual script
    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    FScriptExecutionResult ExecuteScript(const FString& ScriptPath, const TMap<FString, FString>& Parameters = TMap<FString, FString>());

    // Get script metadata
    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    TArray<FScriptMetadata> GetScriptMetadata(EScriptCategory Category = EScriptCategory::All);

    // Get available scripts
    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    TArray<FString> GetAvailableScripts(EScriptCategory Category = EScriptCategory::All);

    // Generate integration report
    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    bool GenerateIntegrationReport(const FIntegrationSession& Session, const FString& ReportPath);

    // Script management
    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    bool ValidateScript(const FString& ScriptPath);

    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    bool InstallScript(const FString& SourcePath, const FString& DestinationPath);

    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    bool UninstallScript(const FString& ScriptPath);

    // Configuration
    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    void SetMaxParallelJobs(int32 MaxJobs) { MaxParallelJobs = MaxJobs; }

    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    int32 GetMaxParallelJobs() const { return MaxParallelJobs; }

    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    void SetScriptTimeout(float TimeoutSeconds) { ScriptTimeout = TimeoutSeconds; }

    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    float GetScriptTimeout() const { return ScriptTimeout; }

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Script Integration")
    FOnScriptExecutionStarted OnScriptExecutionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Script Integration")
    FOnScriptExecutionProgress OnScriptExecutionProgress;

    UPROPERTY(BlueprintAssignable, Category = "Script Integration")
    FOnScriptExecutionCompleted OnScriptExecutionCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Script Integration")
    FOnIntegrationSessionCompleted OnIntegrationSessionCompleted;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    // Script categories and paths
    TMap<EScriptCategory, TArray<FString>> ScriptCategories;
    
    // Script metadata cache
    TMap<FString, FScriptMetadata> ScriptMetadataCache;
    
    // Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    int32 MaxParallelJobs = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    float ScriptTimeout = 300.0f; // 5 minutes default

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString ProjectRoot = TEXT("C:\\HW\\MingGoRTS");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString LogPath = TEXT("Logs/ScriptIntegration_");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString ReportPath = TEXT("Reports/ScriptIntegrationReport_");

    // Internal methods
    void InitializeScriptCategories();
    void InitializeScriptMetadata();
    FScriptMetadata AnalyzeScriptMetadata(const FString& ScriptPath);
    bool ExecuteScriptInternal(const FString& ScriptPath, const TMap<FString, FString>& Parameters, FScriptExecutionResult& OutResult);
    void LogMessage(const FString& Message, const FString& Level = TEXT("INFO"));
    FString GenerateReportContent(const FIntegrationSession& Session);
    void UpdateSessionStatistics(FIntegrationSession& Session, const FScriptExecutionResult& Result);
    
    // Async execution support
    void ExecuteScriptAsync(const FString& ScriptPath, const TMap<FString, FString>& Parameters);
    void OnScriptExecutionComplete(const FString& ScriptPath, FScriptExecutionResult Result);
    
    // Performance monitoring
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    bool bEnablePerformanceMonitoring = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    float PerformanceReportInterval = 60.0f; // seconds

    void UpdatePerformanceMetrics();
};
