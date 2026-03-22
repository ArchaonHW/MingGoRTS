// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Development Task Optimization System - Header
// 
// This system optimizes development task execution order, resource allocation,
// and scheduling to maximize development efficiency and minimize project completion time.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Developer/MingRTSDeveloperToolSystem.h"
#include "MingRTSDevelopmentTaskOptimizer.generated.h"

/**
 * Task priority levels for development scheduling
 */
UENUM(BlueprintType)
enum class EDevelopmentPriority : uint8
{
    Critical      UMETA(DisplayName = "Critical - Blocker"),
    High          UMETA(DisplayName = "High - Major Feature"),
    Medium        UMETA(DisplayName = "Medium - Standard"),
    Low           UMETA(DisplayName = "Low - Enhancement"),
    Background    UMETA(DisplayName = "Background - Nice to have")
};

/**
 * Task status for tracking progress
 */
UENUM(BlueprintType)
enum class EDevelopmentStatus : uint8
{
    Pending       UMETA(DisplayName = "Pending"),
    Blocked       UMETA(DisplayName = "Blocked"),
    InProgress    UMETA(DisplayName = "In Progress"),
    Testing       UMETA(DisplayName = "Testing"),
    Review        UMETA(DisplayName = "Code Review"),
    Completed     UMETA(DisplayName = "Completed"),
    Cancelled     UMETA(DisplayName = "Cancelled")
};

/**
 * Resource types for development
 */
UENUM(BlueprintType)
enum class EDevelopmentResource : uint8
{
    Programmer    UMETA(DisplayName = "Programmer"),
    Designer      UMETA(DisplayName = "Designer"),
    Artist        UMETA(DisplayName = "Artist"),
    Tester        UMETA(DisplayName = "Tester"),
    DevOps        UMETA(DisplayName = "DevOps")
};

/**
 * Optimized development task with dependency and resource tracking
 */
USTRUCT(BlueprintType)
struct FOptimizedDevTask
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TaskId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TaskName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TaskDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDevelopmentPriority Priority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDevelopmentStatus Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EstimatedHours;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ActualHours;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Dependencies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EDevelopmentResource> RequiredResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ComplexityScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RiskFactor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CompletionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OptimizedOrder;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EfficiencyScore;

    FOptimizedDevTask()
        : Priority(EDevelopmentPriority::Medium)
        , Status(EDevelopmentStatus::Pending)
        , EstimatedHours(1.0f)
        , ActualHours(0.0f)
        , ComplexityScore(1.0f)
        , RiskFactor(1.0f)
        , OptimizedOrder(0)
        , EfficiencyScore(0.0f)
    {}

    // Calculate critical path weight
    float GetCriticalPathWeight() const
    {
        return static_cast<float>(Priority) * ComplexityScore * RiskFactor;
    }
};

/**
 * Resource allocation for a specific task
 */
USTRUCT(BlueprintType)
struct FResourceAllocation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TaskId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDevelopmentResource ResourceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ResourceName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AllocatedHours;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime AllocationStart;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime AllocationEnd;

    FResourceAllocation()
        : AllocatedHours(0.0f)
    {}
};

/**
 * Development schedule optimization result
 */
USTRUCT(BlueprintType)
struct FOptimizedSchedule
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FOptimizedDevTask> OrderedTasks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EDevelopmentResource, TArray<FResourceAllocation>> ResourceAllocations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TotalEstimatedHours;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OptimizedDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ParallelizationFactor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EfficiencyGain;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime ProjectedCompletion;

    FOptimizedSchedule()
        : TotalEstimatedHours(0.0f)
        , OptimizedDuration(0.0f)
        , ParallelizationFactor(1)
        , EfficiencyGain(0.0f)
    {}
};

/**
 * Critical path analysis result
 */
USTRUCT(BlueprintType)
struct FCriticalPathAnalysis
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> CriticalPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CriticalPathDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> BottleneckTasks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ParallelizableTasks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SlackTime;
};

/**
 * Development metrics and KPIs
 */
USTRUCT(BlueprintType)
struct FDevelopmentMetrics
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalTasks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CompletedTasks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 InProgressTasks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BlockedTasks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AverageCompletionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ScheduleVariance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResourceUtilization;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Velocity; // Tasks per day

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BurnDownRate;

    FDevelopmentMetrics()
        : TotalTasks(0)
        , CompletedTasks(0)
        , InProgressTasks(0)
        , BlockedTasks(0)
        , AverageCompletionTime(0.0f)
        , ScheduleVariance(0.0f)
        , ResourceUtilization(0.0f)
        , Velocity(0.0f)
        , BurnDownRate(0.0f)
    {}
};

/**
 * Development Task Optimizer - Maximizes development efficiency through
 * intelligent task scheduling, resource allocation, and dependency management.
 * 
 * Features:
 * - Critical path analysis for optimal task ordering
 * - Resource allocation optimization
 * - Parallel execution planning
 * - Risk assessment and mitigation
 * - Schedule compression algorithms
 * - Real-time progress tracking
 * - Predictive completion estimation
 */
UCLASS()
class MINGGORTS_API UMingRTSDevelopmentTaskOptimizer : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UMingRTSDevelopmentTaskOptimizer(};
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    //=========================================================================
    // Task Management
    //=========================================================================
    
    /** Add a development task to the optimizer */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Management")
    FOptimizedDevTask AddDevelopmentTask(
        const FString& TaskName,
        const FString& Description,
        EDevelopmentPriority Priority,
        float EstimatedHours,
        const TArray<FString>& Dependencies,
        const TArray<EDevelopmentResource>& RequiredResources
    };

    /** Remove a task from optimization pool */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Management")
    void RemoveDevelopmentTask(const FString& TaskId};

    /** Update task status and progress */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Management")
    void UpdateTaskStatus(const FString& TaskId, EDevelopmentStatus NewStatus};

    /** Update task actual hours spent */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Management")
    void UpdateTaskProgress(const FString& TaskId, float HoursSpent};

    /** Get all tasks in the system */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Management")
    TArray<FOptimizedDevTask> GetAllTasks(};

    /** Get tasks by status */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Management")
    TArray<FOptimizedDevTask> GetTasksByStatus(EDevelopmentStatus Status};

    /** Get tasks by priority */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Management")
    TArray<FOptimizedDevTask> GetTasksByPriority(EDevelopmentPriority Priority};

    //=========================================================================
    // Optimization Algorithms
    //=========================================================================
    
    /** Perform critical path analysis */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Analysis")
    FCriticalPathAnalysis AnalyzeCriticalPath(};

    /** Generate optimized development schedule */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Scheduling")
    FOptimizedSchedule GenerateOptimizedSchedule(};

    /** Optimize task execution order for maximum parallelism */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Scheduling")
    TArray<FOptimizedDevTask> OptimizeTaskOrder(};

    /** Calculate optimal resource allocation */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Resource Management")
    TMap<EDevelopmentResource, TArray<FResourceAllocation>> OptimizeResourceAllocation(};

    /** Identify tasks that can be parallelized */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Analysis")
    TArray<TArray<FString>> IdentifyParallelTaskGroups(};

    /** Detect and resolve task dependencies */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Analysis")
    TArray<FString> ResolveDependencies(};

    //=========================================================================
    // Scheduling and Planning
    //=========================================================================
    
    /** Generate development roadmap with milestones */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Planning")
    TArray<FString> GenerateDevelopmentRoadmap(};

    /** Estimate project completion date */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Planning")
    FDateTime EstimateCompletionDate(};

    /** Calculate buffer time for critical tasks */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Planning")
    TMap<FString, float> CalculateTaskBuffers(};

    /** Generate sprint plan for agile development */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Planning")
    TArray<FOptimizedDevTask> GenerateSprintPlan(float SprintDurationHours};

    /** Identify schedule compression opportunities */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Planning")
    TArray<FString> IdentifyCompressionOpportunities(};

    //=========================================================================
    // Resource Management
    //=========================================================================
    
    /** Allocate resources to tasks optimally */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Resource Management")
    void AllocateResources(const FString& TaskId, const TArray<EDevelopmentResource>& Resources};

    /** Get resource utilization report */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Resource Management")
    TMap<EDevelopmentResource, float> GetResourceUtilization(};

    /** Identify resource bottlenecks */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Resource Management")
    TArray<EDevelopmentResource> IdentifyResourceBottlenecks(};

    /** Balance resource load across tasks */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Resource Management")
    void BalanceResourceLoad(};

    //=========================================================================
    // Metrics and Reporting
    //=========================================================================
    
    /** Calculate development metrics and KPIs */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Metrics")
    FDevelopmentMetrics CalculateDevelopmentMetrics(};

    /** Generate efficiency report */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Metrics")
    FString GenerateEfficiencyReport(};

    /** Calculate velocity trend */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Metrics")
    TArray<float> CalculateVelocityTrend(int32 Days};

    /** Predict project completion probability */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Metrics")
    float PredictCompletionProbability(FDateTime TargetDate};

    /** Get burndown chart data */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Metrics")
    TMap<FDateTime, float> GetBurndownData(int32 Days};

    //=========================================================================
    // Risk Management
    //=========================================================================
    
    /** Assess task risks */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Risk")
    TMap<FString, float> AssessTaskRisks(};

    /** Identify high-risk tasks */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Risk")
    TArray<FOptimizedDevTask> IdentifyHighRiskTasks(};

    /** Suggest risk mitigation strategies */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Risk")
    TMap<FString, TArray<FString>> SuggestMitigationStrategies(};

    /** Calculate risk-adjusted schedule */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Risk")
    FOptimizedSchedule CalculateRiskAdjustedSchedule(};

    //=========================================================================
    // Automation
    //=========================================================================
    
    /** Auto-assign tasks based on resource availability */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Automation")
    void AutoAssignTasks(};

    /** Auto-rebalance schedule when tasks complete early/late */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Automation")
    void AutoRebalanceSchedule(};

    /** Enable real-time schedule optimization */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Automation")
    void EnableRealTimeOptimization(bool bEnabled};

    /** Import tasks from external system */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Automation")
    void ImportTasks(const FString& SourceData};

    /** Export optimized schedule */
    UFUNCTION(BlueprintCallable, Category = "Task Optimization|Automation")
    FString ExportOptimizedSchedule(};

    //=========================================================================
    // Delegates
    //=========================================================================
    
    /** Called when schedule is optimized */
    UPROPERTY(BlueprintAssignable, Category = "Task Optimization|Events")
    FOnScheduleOptimized OnScheduleOptimized;

    /** Called when a task status changes */
    UPROPERTY(BlueprintAssignable, Category = "Task Optimization|Events")
    FOnTaskStatusChanged OnTaskStatusChanged;

    /** Called when resources are reallocated */
    UPROPERTY(BlueprintAssignable, Category = "Task Optimization|Events")
    FOnResourcesReallocated OnResourcesReallocated;

private:
    // Internal task storage
    TArray<FOptimizedDevTask> DevelopmentTasks;
    TMap<EDevelopmentResource, TArray<FResourceAllocation>> ResourcePool;
    
    // Optimization state
    bool bRealTimeOptimizationEnabled;
    FOptimizedSchedule CurrentOptimizedSchedule;
    
    // Internal methods
    void InitializeDefaultTasks(};
    void SortTasksByCriticalPath(};
    void CalculateDependencyGraph(};
    void OptimizeResourcePool(};
    float CalculateTaskEfficiency(const FOptimizedDevTask& Task};
    TArray<FString> FindCriticalPath(};
    void UpdateScheduleMetrics(};
    
    // Algorithm helpers
    TArray<FOptimizedDevTask> TopologicalSort(};
    TMap<FString, float> CalculateEarlyStartTimes(};
    TMap<FString, float> CalculateLateStartTimes(};
    void ApplyScheduleCompression(};
};

// Delegate declarations
DECLARE_DYNAMIC_DELEGATE(FOnScheduleOptimized, const FOptimizedSchedule&, OptimizedSchedule};
DECLARE_DYNAMIC_DELEGATE(FOnTaskStatusChanged, const FString&, TaskId, EDevelopmentStatus, NewStatus};
DECLARE_DYNAMIC_DELEGATE(FOnResourcesReallocated, const TMap<EDevelopmentResource, TArray<FResourceAllocation>>&, NewAllocations};

#include "MingRTSDevelopmentTaskOptimizer.generated.h"

