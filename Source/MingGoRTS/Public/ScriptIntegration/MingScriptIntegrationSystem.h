#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "MingScriptIntegrationSystem.generated.h"

UENUM(BlueprintType)
enum class EMingScriptCategory : uint8
{
    Build,
    Automation,
    AI,
    API,
    Optimization,
    Debug,
    Security,
    Maintenance
};

UENUM(BlueprintType)
enum class EMingExecutionMode : uint8
{
    Sequential,
    Parallel,
    Pipeline
};

UENUM(BlueprintType)
enum class EMingScriptResult : uint8
{
    Success,
    Failed,
    Skipped
};

USTRUCT(BlueprintType)
struct FMingScriptMetadata
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Script Metadata")
    FString ScriptPath;

    UPROPERTY(BlueprintReadWrite, Category = "Script Metadata")
    EMingScriptCategory Category;

    UPROPERTY(BlueprintReadWrite, Category = "Script Metadata")
    FString Name;

    UPROPERTY(BlueprintReadWrite, Category = "Script Metadata")
    int32 Size;

    UPROPERTY(BlueprintReadWrite, Category = "Script Metadata")
    FDateTime LastModified;

    UPROPERTY(BlueprintReadWrite, Category = "Script Metadata")
    TArray<FString> Dependencies;

    UPROPERTY(BlueprintReadWrite, Category = "Script Metadata")
    TArray<FString> Parameters;

    UPROPERTY(BlueprintReadWrite, Category = "Script Metadata")
    float EstimatedTime;

    UPROPERTY(BlueprintReadWrite, Category = "Script Metadata")
    float SuccessRate;

    FMingScriptMetadata()
    {
        ScriptPath = TEXT("");
        Category = EMingScriptCategory::Build;
        Name = TEXT("");
        Size = 0;
        LastModified = FDateTime::Now();
        Dependencies.Empty();
        Parameters.Empty();
        EstimatedTime = 5.0f;
        SuccessRate = 0.95f;
    }
};

USTRUCT(BlueprintType)
struct FMingExecutionResults
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Execution Results")
    int32 SuccessCount;

    UPROPERTY(BlueprintReadWrite, Category = "Execution Results")
    int32 FailureCount;

    UPROPERTY(BlueprintReadWrite, Category = "Execution Results")
    float TotalTime;

    UPROPERTY(BlueprintReadWrite, Category = "Execution Results")
    EMingExecutionMode Mode;

    UPROPERTY(BlueprintReadWrite, Category = "Execution Results")
    TMap<FString, FString> PipelineData;

    FMingExecutionResults()
    {
        SuccessCount = 0;
        FailureCount = 0;
        TotalTime = 0.0f;
        Mode = EMingExecutionMode::Sequential;
        PipelineData.Empty();
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScriptExecutionCompleted, const FString&, ScriptName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScriptExecutionFailed, const FString&, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExecutionProgress, const FString&, ScriptName, float, Progress);

/**
 * MingGoRTS Script Integration System
 * 
 * This system provides comprehensive script management and execution capabilities,
 * replacing the original PowerShell-based script integration system.
 * 
 * Features:
 * - Script categorization and metadata management
 * - Multiple execution modes (Sequential, Parallel, Pipeline)
 * - Progress tracking and error handling
 * - HTML report generation
 * - C++ native performance
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Script Integration")
class MINGGORTS_API UMingScriptIntegrationSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingScriptIntegrationSystem();

    // Initialize the script integration system
    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    void InitializeScriptIntegrationSystem();

    // Execute scripts by category
    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    FMingExecutionResults ExecuteScriptsByCategory(EMingScriptCategory Category, EMingExecutionMode Mode = EMingExecutionMode::Sequential, bool bForce = false);

    // Execute specific script
    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    bool ExecuteScript(const FString& ScriptPath, const TMap<FString, FString>& Parameters);

    // Get script metadata
    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    FMingScriptMetadata GetScriptMetadata(const FString& ScriptPath);

    // List all scripts
    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    TArray<FMingScriptMetadata> GetAllScripts();

    // Generate execution report
    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    bool GenerateExecutionReport(const FMingExecutionResults& Results, const FString& ReportPath);

    // Clean up generated files
    UFUNCTION(BlueprintCallable, Category = "Script Integration")
    void CleanupGeneratedFiles();

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Script Integration")
    FOnScriptExecutionCompleted OnScriptExecutionCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Script Integration")
    FOnScriptExecutionFailed OnScriptExecutionFailed;

    UPROPERTY(BlueprintAssignable, Category = "Script Integration")
    FOnExecutionProgress OnExecutionProgress;

protected:
    // Initialize script metadata
    void InitializeScriptMetadata();

    // Execute scripts sequentially
    FMingExecutionResults ExecuteScriptsSequential(const TArray<FString>& Scripts);

    // Execute scripts in parallel
    FMingExecutionResults ExecuteScriptsParallel(const TArray<FString>& Scripts);

    // Execute scripts as pipeline
    FMingExecutionResults ExecuteScriptsPipeline(const TArray<FString>& Scripts);

    // Write log entry
    void WriteLog(const FString& Message, const FString& Level = TEXT("INFO"));

    // Get scripts for category
    TArray<FString> GetScriptsForCategory(EMingScriptCategory Category);

    // Analyze script content for metadata
    void AnalyzeScriptContent(const FString& ScriptPath, FMingScriptMetadata& OutMetadata);

private:
    // Script metadata storage
    UPROPERTY()
    TMap<FString, FMingScriptMetadata> ScriptMetadata;

    // Project paths
    FString ProjectRoot;
    FString ToolsPath;
    FString LogPath;
    FString ReportPath;

    // Script categories
    TMap<EMingScriptCategory, TArray<FString>> ScriptCategories;

    // System state
    bool bIsInitialized;
};
