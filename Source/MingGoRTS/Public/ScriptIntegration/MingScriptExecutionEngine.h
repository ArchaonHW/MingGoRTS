#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/WeakObjectPtr.h"
#include "Containers/Map.h"
#include "Containers/Array.h"
#include "Containers/Queue.h"
#include "HAL/PlatformProcess.h"
#include "HAL/ThreadSafeCounter.h"
#include "Misc/DateTime.h"
#include "Engine/Engine.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "MingScriptExecutionEngine.generated.h"

UENUM(BlueprintType)
enum class EScriptCategory : uint8
{
    Build       UMETA(DisplayName = "Build"),
    Automation  UMETA(DisplayName = "Automation"),
    AI          UMETA(DisplayName = "AI"),
    API         UMETA(DisplayName = "API"),
    Optimization UMETA(DisplayName = "Optimization"),
    Debug       UMETA(DisplayName = "Debug"),
    Security    UMETA(DisplayName = "Security"),
    Maintenance UMETA(DisplayName = "Maintenance"),
    Testing     UMETA(DisplayName = "Testing"),
    All         UMETA(DisplayName = "All")
};

UENUM(BlueprintType)
enum class EScriptExecutionMode : uint8
{
    Sequential  UMETA(DisplayName = "Sequential"),
    Parallel    UMETA(DisplayName = "Parallel"),
    Pipeline    UMETA(DisplayName = "Pipeline"),
    Conditional UMETA(DisplayName = "Conditional"),
    EventDriven UMETA(DisplayName = "Event Driven"),
    Smart       UMETA(DisplayName = "Smart")
};

UENUM(BlueprintType)
enum class EScriptStatus : uint8
{
    Pending     UMETA(DisplayName = "Pending"),
    Running     UMETA(DisplayName = "Running"),
    Completed   UMETA(DisplayName = "Completed"),
    Failed      UMETA(DisplayName = "Failed"),
    Cancelled   UMETA(DisplayName = "Cancelled"),
    Paused      UMETA(DisplayName = "Paused")
};

UENUM(BlueprintType)
enum class EScriptPriority : uint8
{
    Critical    UMETA(DisplayName = "Critical"),
    High        UMETA(DisplayName = "High"),
    Medium      UMETA(DisplayName = "Medium"),
    Low         UMETA(DisplayName = "Low"),
    Background  UMETA(DisplayName = "Background")
};

USTRUCT(BlueprintType)
struct FScriptMetadata
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    FString ScriptName;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    FString ScriptPath;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    EScriptCategory Category;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    FString Description;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    TArray<FString> Dependencies;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    TArray<FString> Parameters;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    float EstimatedDuration;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    EScriptPriority Priority;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    bool bIsEnabled;

    FScriptMetadata()
        : ScriptName(TEXT(""))
        , ScriptPath(TEXT(""))
        , Category(EScriptCategory::Build)
        , Description(TEXT(""))
        , EstimatedDuration(0.0f)
        , Priority(EScriptPriority::Medium)
        , bIsEnabled(true)
    {
    }
};

USTRUCT(BlueprintType)
struct FScriptExecutionResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    FString ScriptName;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    EScriptStatus Status;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    FString Output;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    FString ErrorMessage;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    FDateTime StartTime;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    FDateTime EndTime;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    float ExecutionTime;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    int32 ExitCode;

    FScriptExecutionResult()
        : ScriptName(TEXT(""))
        , Status(EScriptStatus::Pending)
        , Output(TEXT(""))
        , ErrorMessage(TEXT(""))
        , ExecutionTime(0.0f)
        , ExitCode(0)
    {
    }
};

USTRUCT(BlueprintType)
struct FScriptExecutionSummary
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    TArray<FScriptExecutionResult> Results;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    FDateTime StartTime;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    FDateTime EndTime;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    float TotalExecutionTime;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    int32 TotalScripts;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    int32 SuccessfulScripts;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    int32 FailedScripts;

    UPROPERTY(BlueprintReadWrite, Category = "Script")
    int32 SkippedScripts;

    FScriptExecutionSummary()
        : TotalExecutionTime(0.0f)
        , TotalScripts(0)
        , SuccessfulScripts(0)
        , FailedScripts(0)
        , SkippedScripts(0)
    {
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnScriptStarted, const FString&, ScriptName, const FScriptMetadata&, Metadata, const FString&, TaskID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnScriptCompleted, const FString&, ScriptName, const FScriptExecutionResult&, Result, const FString&, TaskID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnScriptFailed, const FString&, ScriptName, const FString&, ErrorMessage, const FString&, TaskID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExecutionProgress, const FString&, TaskID, float, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExecutionSummary, const FScriptExecutionSummary&, Summary);

/**
 * C++ Script Execution Engine - Replaces all PowerShell and Python scripts
 * Provides unified script management and execution capabilities
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (MingGoRTS))
class MINGGORTS_API UMingScriptExecutionEngine : public UObject
{
    GENERATED_BODY()

public:
    UMingScriptExecutionEngine();

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    void InitializeScriptEngine();

    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    void ShutdownScriptEngine();

    // Script Discovery and Management
    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    void DiscoverAllScripts();

    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    TArray<FScriptMetadata> GetScriptsByCategory(EScriptCategory Category);

    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    FScriptMetadata GetScriptMetadata(const FString& ScriptName);

    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    bool IsScriptEnabled(const FString& ScriptName);

    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    void SetScriptEnabled(const FString& ScriptName, bool bEnabled);

    // Script Execution
    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    FString ExecuteScript(const FString& ScriptName, const TArray<FString>& Parameters = TArray<FString>());

    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    FString ExecuteScriptAsync(const FString& ScriptName, const TArray<FString>& Parameters = TArray<FString>());

    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    FString ExecuteScriptsByCategory(EScriptCategory Category, EScriptExecutionMode Mode = EScriptExecutionMode::Sequential);

    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    FString ExecuteAllScripts(EScriptExecutionMode Mode = EScriptExecutionMode::Sequential);

    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    void CancelExecution(const FString& TaskID);

    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    void PauseExecution(const FString& TaskID);

    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    void ResumeExecution(const FString& TaskID);

    // AI Asset Generation
    UFUNCTION(BlueprintCallable, Category = "Script Engine|AI")
    FString GenerateAIMusic(const FString& Description, float Duration = 60.0f);

    UFUNCTION(BlueprintCallable, Category = "Script Engine|AI")
    FString GenerateAIAsset(const FString& AssetType, const FString& Description, const FString& OutputPath);

    UFUNCTION(BlueprintCallable, Category = "Script Engine|AI")
    FString GenerateEpicAssets(const FString& EpicID, const TArray<FString>& AssetTypes = TArray<FString>());

    // Build System
    UFUNCTION(BlueprintCallable, Category = "Script Engine|Build")
    FString CompileProject(const FString& Configuration = TEXT("Development"));

    UFUNCTION(BlueprintCallable, Category = "Script Engine|Build")
    FString FixSyntaxErrors();

    UFUNCTION(BlueprintCallable, Category = "Script Engine|Build")
    FString RunTests(const FString& TestCategory = TEXT("All"));

    UFUNCTION(BlueprintCallable, Category = "Script Engine|Build")
    FString CheckProjectHealth();

    // API Management
    UFUNCTION(BlueprintCallable, Category = "Script Engine|API")
    FString GenerateAPIDocumentation();

    UFUNCTION(BlueprintCallable, Category = "Script Engine|API")
    FString EnhanceAPI(const FString& EnhancementType);

    UFUNCTION(BlueprintCallable, Category = "Script Engine|API")
    FString IntegrateAllScripts();

    // Encoding and Text Processing
    UFUNCTION(BlueprintCallable, Category = "Script Engine|Maintenance")
    FString FixEncodingIssues();

    UFUNCTION(BlueprintCallable, Category = "Script Engine|Maintenance")
    FString FixGarbledText();

    UFUNCTION(BlueprintCallable, Category = "Script Engine|Maintenance")
    FString CheckNamingConventions();

    // Testing System
    UFUNCTION(BlueprintCallable, Category = "Script Engine|Testing")
    FString RunCharacterCreationTests();

    UFUNCTION(BlueprintCallable, Category = "Script Engine|Testing")
    FString RunDialogueSystemTests();

    UFUNCTION(BlueprintCallable, Category = "Script Engine|Testing")
    FString RunRelationshipNetworkTests();

    UFUNCTION(BlueprintCallable, Category = "Script Engine|Testing")
    FString RunHistoricalRoleplayTests();

    UFUNCTION(BlueprintCallable, Category = "Script Engine|Testing")
    FString RunRTScoreTests();

    UFUNCTION(BlueprintCallable, Category = "Script Engine|Testing")
    FString RunSkillSystemTests();

    // Progress and Status
    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    FScriptExecutionResult GetExecutionStatus(const FString& TaskID);

    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    bool IsExecutionComplete(const FString& TaskID);

    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    float GetExecutionProgress(const FString& TaskID);

    // Reporting
    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    FString GenerateExecutionReport(const FString& TaskID);

    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    FString GenerateHTMLReport(const FScriptExecutionSummary& Summary);

    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    void SaveExecutionLog(const FString& TaskID, const FString& LogPath);

    // Configuration
    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    void SetMaxConcurrentExecutions(int32 MaxConcurrent);

    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    void SetDefaultTimeout(float TimeoutSeconds);

    UFUNCTION(BlueprintCallable, Category = "Script Engine")
    void SetExecutionMode(EScriptExecutionMode Mode);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Script Engine")
    FOnScriptStarted OnScriptStarted;

    UPROPERTY(BlueprintAssignable, Category = "Script Engine")
    FOnScriptCompleted OnScriptCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Script Engine")
    FOnScriptFailed OnScriptFailed;

    UPROPERTY(BlueprintAssignable, Category = "Script Engine")
    FOnExecutionProgress OnExecutionProgress;

    UPROPERTY(BlueprintAssignable, Category = "Script Engine")
    FOnExecutionSummary OnExecutionSummary;

protected:
    // Internal execution methods
    FString ExecuteScriptInternal(const FString& ScriptName, const TArray<FString>& Parameters, const FString& TaskID);
    FString ExecuteSequentialScripts(const TArray<FString>& ScriptNames, const FString& TaskID);
    FString ExecuteParallelScripts(const TArray<FString>& ScriptNames, const FString& TaskID);
    FString ExecutePipelineScripts(const TArray<FString>& ScriptNames, const FString& TaskID);

    // Script type handlers
    FString HandleBuildScript(const FScriptMetadata& Script, const TArray<FString>& Parameters);
    FString HandleAIScript(const FScriptMetadata& Script, const TArray<FString>& Parameters);
    FString HandleAPIScript(const FScriptMetadata& Script, const TArray<FString>& Parameters);
    FString HandleTestingScript(const FScriptMetadata& Script, const TArray<FString>& Parameters);
    FString HandleMaintenanceScript(const FScriptMetadata& Script, const TArray<FString>& Parameters);

    // Utility methods
    void ScanScriptsDirectory(const FString& DirectoryPath);
    void ParseScriptMetadata(const FString& ScriptPath, FScriptMetadata& OutMetadata);
    FString GenerateTaskID();
    void UpdateExecutionProgress(const FString& TaskID, float Progress);
    void LogExecution(const FString& Message, const FString& Category = TEXT("ScriptEngine"));

    // Asset generation helpers
    FString GeneratePlaceholderAsset(const FString& AssetType, const FString& Name, const FString& Description, const FString& OutputPath);
    FString CreateOutputDirectory(const FString& Path);
    bool ValidateAssetParameters(const FString& AssetType, const FString& Description);
    FString GetAssetExtension(const FString& AssetType);

    // Script registration
    void RegisterBuiltinScripts();
    void RegisterScript(const FString& ScriptName, EScriptCategory Category, const FString& Description, float EstimatedDuration);

    // Script type handlers
    FString HandleBuildScript(const FScriptMetadata& Script, const TArray<FString>& Parameters);
    FString HandleAIScript(const FScriptMetadata& Script, const TArray<FString>& Parameters);
    FString HandleAPIScript(const FScriptMetadata& Script, const TArray<FString>& Parameters);
    FString HandleTestingScript(const FScriptMetadata& Script, const TArray<FString>& Parameters);
    FString HandleMaintenanceScript(const FScriptMetadata& Script, const TArray<FString>& Parameters);

    // Individual script handlers
    FString HandleCompileProject(const TArray<FString>& Parameters);
    FString HandleFixSyntaxErrors(const TArray<FString>& Parameters);
    FString HandleCheckProjectHealth(const TArray<FString>& Parameters);
    FString HandleGenerateAIMusic(const TArray<FString>& Parameters);
    FString HandleGenerateAIAsset(const TArray<FString>& Parameters);
    FString HandleGenerateEpicAssets(const TArray<FString>& Parameters);
    FString HandleGenerateAPIDocumentation(const TArray<FString>& Parameters);
    FString HandleEnhanceAPI(const TArray<FString>& Parameters);
    FString HandleIntegrateAllScripts(const TArray<FString>& Parameters);
    FString HandleRunTests(const TArray<FString>& Parameters);
    FString HandleRunCharacterCreationTests(const TArray<FString>& Parameters);
    FString HandleRunDialogueSystemTests(const TArray<FString>& Parameters);
    FString HandleRunRelationshipNetworkTests(const TArray<FString>& Parameters);
    FString HandleRunHistoricalRoleplayTests(const TArray<FString>& Parameters);
    FString HandleRunRTScoreTests(const TArray<FString>& Parameters);
    FString HandleRunSkillSystemTests(const TArray<FString>& Parameters);
    FString HandleFixEncodingIssues(const TArray<FString>& Parameters);
    FString HandleFixGarbledText(const TArray<FString>& Parameters);
    FString HandleCheckNamingConventions(const TArray<FString>& Parameters);

private:
    // Script registry
    UPROPERTY()
    TMap<FString, FScriptMetadata> ScriptRegistry;

    UPROPERTY()
    TMap<EScriptCategory, TArray<FString>> CategoryScripts;

    // Execution tracking
    UPROPERTY()
    TMap<FString, FScriptExecutionSummary> ActiveExecutions;

    UPROPERTY()
    TQueue<FString> PendingTasks;

    // Configuration
    UPROPERTY()
    int32 MaxConcurrentExecutions;

    UPROPERTY()
    float DefaultTimeout;

    UPROPERTY()
    EScriptExecutionMode CurrentExecutionMode;

    // State
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    TAtomic<int32> ActiveExecutionCount;

    // Paths
    UPROPERTY()
    FString ToolsDirectory;

    UPROPERTY()
    FString OutputDirectory;

    UPROPERTY()
    FString LogDirectory;
};
