// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Developer Efficiency System - Header
// 
// This system provides comprehensive developer tools to increase development efficiency,
// including rapid prototyping, automated testing, code generation, and performance profiling.
// It supports 10 developer tool categories with 50+ features for streamlined development.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/World.h"
#include "Misc/DateTime.h"
#include "Misc/Timespan.h"
#include "HAL/PlatformFilemanager.h"

// Forward declarations
class UMingRTSIntegrationTestSuite;
class UMingRTSSelfLearningSystem;

/**
 * Developer tool categories for organizing functionality
 */
UENUM(BlueprintType)
enum class EDeveloperToolCategory : uint8
{
    RapidPrototyping      UMETA(DisplayName = "Rapid Prototyping"),
    AutomatedTesting      UMETA(DisplayName = "Automated Testing"),
    CodeGeneration        UMETA(DisplayName = "Code Generation"),
    PerformanceProfiling  UMETA(DisplayName = "Performance Profiling"),
    AssetManagement       UMETA(DisplayName = "Asset Management"),
    Debugging             UMETA(DisplayName = "Debugging"),
    Documentation         UMETA(DisplayName = "Documentation"),
    WorkflowAutomation    UMETA(DisplayName = "Workflow Automation"),
    Analytics             UMETA(DisplayName = "Analytics"),
    Collaboration         UMETA(DisplayName = "Collaboration")
};

/**
 * Developer session tracking information
 */
USTRUCT(BlueprintType)
struct FDeveloperSessionInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FDateTime SessionStartTime;

    UPROPERTY(BlueprintReadOnly)
    FString DeveloperName;

    UPROPERTY(BlueprintReadOnly)
    FString SessionId;

    UPROPERTY(BlueprintReadOnly)
    int32 FilesModified;

    UPROPERTY(BlueprintReadOnly)
    int32 TestsRun;

    UPROPERTY(BlueprintReadOnly)
    int32 TestsPassed;

    UPROPERTY(BlueprintReadOnly)
    float ProductivityScore;

    FDeveloperSessionInfo()
        : SessionStartTime(FDateTime::Now())
        , FilesModified(0)
        , TestsRun(0)
        , TestsPassed(0)
        , ProductivityScore(0.0f)
    {}
};

/**
 * Code template structure for code generation
 */
USTRUCT(BlueprintType)
struct FCodeTemplate
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TemplateName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TemplateDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TemplateCode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> PlaceholderVariables;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDeveloperToolCategory Category;

    FCodeTemplate()
        : Category(EDeveloperToolCategory::CodeGeneration)
    {}
};

/**
 * Performance metric for profiling
 */
USTRUCT(BlueprintType)
struct FPerformanceMetric
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString MetricName;

    UPROPERTY(BlueprintReadOnly)
    float CurrentValue;

    UPROPERTY(BlueprintReadOnly)
    float AverageValue;

    UPROPERTY(BlueprintReadOnly)
    float MinValue;

    UPROPERTY(BlueprintReadOnly)
    float MaxValue;

    UPROPERTY(BlueprintReadOnly)
    int32 SampleCount;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastUpdateTime;

    FPerformanceMetric()
        : CurrentValue(0.0f)
        , AverageValue(0.0f)
        , MinValue(MAX_FLT)
        , MaxValue(MIN_FLT)
        , SampleCount(0)
        , LastUpdateTime(FDateTime::Now())
    {}

    void AddSample(float Value)
    {
        CurrentValue = Value;
        MinValue = FMath::Min(MinValue, Value};
        MaxValue = FMath::Max(MaxValue, Value};
        
        // Update rolling average
        AverageValue = (AverageValue * SampleCount + Value) / (SampleCount + 1};
        SampleCount++;
        LastUpdateTime = FDateTime::Now(};
    }
};

/**
 * Development task for workflow management
 */
USTRUCT(BlueprintType)
struct FDevelopmentTask
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TaskId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TaskName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TaskDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDeveloperToolCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Priority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CreatedTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CompletedTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EstimatedHours;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ActualHours;

    FDevelopmentTask()
        : Category(EDeveloperToolCategory::RapidPrototyping)
        , Priority(5)
        , bCompleted(false)
        , CreatedTime(FDateTime::Now())
        , EstimatedHours(1.0f)
        , ActualHours(0.0f)
    {}
};

/**
 * Developer Efficiency System - Main class for boosting development productivity
 * 
 * Features:
 * - Rapid prototyping tools for quick iteration
 * - Automated testing execution and reporting
 * - Code generation templates for common patterns
 * - Performance profiling and optimization suggestions
 * - Asset management automation
 * - Integrated debugging tools
 * - Documentation generation
 * - Workflow automation
 * - Development analytics
 * - Team collaboration tools
 */
UCLASS()
class MINGGORTS_API UMingRTSDeveloperToolSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // Constructor and lifecycle
    UMingRTSDeveloperToolSystem(};
    
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    //=========================================================================
    // 1. Rapid Prototyping Tools
    //=========================================================================
    
    /** Initialize rapid prototyping environment */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Rapid Prototyping")
    void InitializePrototypingEnvironment(};

    /** Create a quick test scenario */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Rapid Prototyping")
    FString CreateQuickTestScenario(const FString& ScenarioName, const FString& ScenarioType};

    /** Generate test units for debugging */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Rapid Prototyping")
    int32 SpawnTestUnits(int32 Count, const FVector& Location};

    /** Quick setup for combat testing */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Rapid Prototyping")
    void SetupCombatTestEnvironment(};

    /** Toggle rapid iteration mode (faster compile/reload) */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Rapid Prototyping")
    void SetRapidIterationMode(bool bEnabled};

    //=========================================================================
    // 2. Automated Testing Tools
    //=========================================================================
    
    /** Run all integration tests with progress tracking */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Testing")
    void RunAutomatedTestSuite(};

    /** Run specific test category */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Testing")
    void RunTestCategory(EDeveloperToolCategory Category};

    /** Schedule continuous testing during development */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Testing")
    void ScheduleContinuousTesting(float IntervalMinutes};

    /** Generate test coverage report */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Testing")
    FString GenerateTestCoverageReport(};

    /** Validate all systems integration */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Testing")
    bool ValidateSystemIntegration(};

    //=========================================================================
    // 3. Code Generation Tools
    //=========================================================================
    
    /** Generate system boilerplate code */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Code Generation")
    FString GenerateSystemBoilerplate(const FString& SystemName, EDeveloperToolCategory Category};

    /** Generate test code for a system */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Code Generation")
    FString GenerateTestCode(const FString& SystemName};

    /** Create new faction template */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Code Generation")
    FString GenerateFactionTemplate(const FString& FactionName, int32 Difficulty};

    /** Generate AI behavior pattern code */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Code Generation")
    FString GenerateAIPatternCode(const FString& PatternName};

    /** Register custom code template */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Code Generation")
    void RegisterCodeTemplate(const FCodeTemplate& Template};

    /** Get available code templates */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Code Generation")
    TArray<FCodeTemplate> GetAvailableTemplates(EDeveloperToolCategory Category};

    //=========================================================================
    // 4. Performance Profiling Tools
    //=========================================================================
    
    /** Start performance profiling session */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Profiling")
    void StartProfilingSession(const FString& SessionName};

    /** End profiling and generate report */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Profiling")
    FString EndProfilingSession(};

    /** Record performance metric */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Profiling")
    void RecordPerformanceMetric(const FString& MetricName, float Value};

    /** Get performance report for a metric */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Profiling")
    FPerformanceMetric GetPerformanceMetric(const FString& MetricName};

    /** Analyze performance bottlenecks */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Profiling")
    TArray<FString> AnalyzePerformanceBottlenecks(};

    /** Get optimization suggestions */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Profiling")
    TArray<FString> GetOptimizationSuggestions(};

    //=========================================================================
    // 5. Asset Management Automation
    //=========================================================================
    
    /** Batch process assets */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Asset Management")
    void BatchProcessAssets(const TArray<FString>& AssetPaths, const FString& Operation};

    /** Validate asset integrity */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Asset Management")
    TArray<FString> ValidateAssetIntegrity(};

    /** Generate asset usage report */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Asset Management")
    FString GenerateAssetUsageReport(};

    /** Auto-organize assets by type */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Asset Management")
    void AutoOrganizeAssets(};

    /** Clean up unused assets */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Asset Management")
    TArray<FString> CleanupUnusedAssets(};

    //=========================================================================
    // 6. Debugging Tools
    //=========================================================================
    
    /** Enable advanced debugging mode */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Debugging")
    void SetAdvancedDebugMode(bool bEnabled};

    /** Log all system events for debugging */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Debugging")
    void EnableSystemEventLogging(bool bEnabled};

    /** Visualize data flow between systems */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Debugging")
    void VisualizeSystemDataFlow(};

    /** Generate system state snapshot */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Debugging")
    FString GenerateSystemStateSnapshot(};

    /** Compare system states */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Debugging")
    TArray<FString> CompareSystemStates(const FString& SnapshotA, const FString& SnapshotB};

    //=========================================================================
    // 7. Documentation Generation
    //=========================================================================
    
    /** Generate API documentation */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Documentation")
    FString GenerateAPIDocumentation(};

    /** Generate system architecture diagram */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Documentation")
    FString GenerateArchitectureDiagram(};

    /** Generate development guide */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Documentation")
    FString GenerateDevelopmentGuide(};

    /** Auto-document code */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Documentation")
    void AutoDocumentCode(const FString& FilePath};

    /** Generate changelog from commits */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Documentation")
    FString GenerateChangelog(};

    //=========================================================================
    // 8. Workflow Automation
    //=========================================================================
    
    /** Create development task */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Workflow")
    FDevelopmentTask CreateDevelopmentTask(const FString& TaskName, const FString& Description, int32 Priority};

    /** Complete development task */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Workflow")
    void CompleteDevelopmentTask(const FString& TaskId};

    /** Get pending tasks */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Workflow")
    TArray<FDevelopmentTask> GetPendingTasks(};

    /** Automate build process */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Workflow")
    void AutomateBuildProcess(};

    /** Setup CI/CD integration */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Workflow")
    void SetupCICDIntegration(};

    //=========================================================================
    // 9. Development Analytics
    //=========================================================================
    
    /** Get developer session info */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Analytics")
    FDeveloperSessionInfo GetCurrentSessionInfo(};

    /** Calculate development velocity */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Analytics")
    float CalculateDevelopmentVelocity(};

    /** Track code quality metrics */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Analytics")
    FPerformanceMetric GetCodeQualityMetrics(};

    /** Generate productivity report */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Analytics")
    FString GenerateProductivityReport(};

    /** Track time spent on features */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Analytics")
    void TrackFeatureDevelopmentTime(const FString& FeatureName, float Hours};

    //=========================================================================
    // 10. Collaboration Tools
    //=========================================================================
    
    /** Share development session */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Collaboration")
    void ShareDevelopmentSession(const FString& TeamMemberId};

    /** Broadcast message to team */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Collaboration")
    void BroadcastTeamMessage(const FString& Message};

    /** Sync development tasks */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Collaboration")
    void SyncDevelopmentTasks(};

    /** Get team member status */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Collaboration")
    TArray<FString> GetTeamMemberStatus(};

    //=========================================================================
    // Utility Functions
    //=========================================================================
    
    /** Export developer report */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Utilities")
    void ExportDeveloperReport(const FString& FilePath};

    /** Import development settings */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Utilities")
    void ImportDevelopmentSettings(const FString& FilePath};

    /** Reset developer tools to defaults */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Utilities")
    void ResetToDefaults(};

    /** Get system statistics */
    UFUNCTION(BlueprintCallable, Category = "Developer Tools|Utilities")
    FString GetSystemStatistics(};

    //=========================================================================
    // Delegates
    //=========================================================================
    
    /** Called when test suite completes */
    UPROPERTY(BlueprintAssignable, Category = "Developer Tools|Events")
    FOnTestCompleted OnTestCompleted;

    /** Called when code generation completes */
    UPROPERTY(BlueprintAssignable, Category = "Developer Tools|Events")
    FOnCodeGenerated OnCodeGenerated;

    /** Called when profiling data updates */
    UPROPERTY(BlueprintAssignable, Category = "Developer Tools|Events")
    FOnProfilingDataUpdated OnProfilingDataUpdated;

    /** Called when task status changes */
    UPROPERTY(BlueprintAssignable, Category = "Developer Tools|Events")
    FOnTaskStatusChanged OnTaskStatusChanged;

private:
    // Internal state
    FDeveloperSessionInfo CurrentSession;
    TArray<FCodeTemplate> CodeTemplates;
    TArray<FDevelopmentTask> DevelopmentTasks;
    TMap<FString, FPerformanceMetric> PerformanceMetrics;
    TArray<FString> SystemSnapshots;
    
    bool bRapidIterationMode;
    bool bAdvancedDebugMode;
    bool bSystemEventLogging;
    
    FTimespan TotalDevelopmentTime;
    int32 CompletedTasks;
    int32 TotalTasks;
    
    // Internal methods
    void InitializeCodeTemplates(};
    void StartSessionTracking(};
    void StopSessionTracking(};
    void AutoSaveDevelopmentState(};
    void LoadDevelopmentState(};
    
    FString GenerateUniqueId(};
    float CalculateProductivityScore(};
};

// Delegate declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestCompleted, const FString&, TestResults};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCodeGenerated, const FString&, GeneratedCode};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProfilingDataUpdated, const FString&, MetricName, float, NewValue};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTaskStatusChanged, const FString&, TaskId, bool, bCompleted};

#include "Developer/MingRTSDeveloperToolSystem.generated.h"

