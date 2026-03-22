// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Development Task Optimization System - Implementation
// 
// This system optimizes development task execution order, resource allocation,
// and scheduling to maximize development efficiency.

#include "Developer/MingRTSDevelopmentTaskOptimizer.h"
#include "Developer/MingRTSDeveloperToolSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingTaskOptimizer, Log, All);

// Constructor
UMingRTSDevelopmentTaskOptimizer::UMingRTSDevelopmentTaskOptimizer()
    : bRealTimeOptimizationEnabled(false)
{
}

void UMingRTSDevelopmentTaskOptimizer::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Initializing Development Task Optimizer..."));
    
    // Initialize with default tasks from the project
    InitializeDefaultTasks();
    
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Development Task Optimizer initialized with %d tasks"), 
        DevelopmentTasks.Num());
}

void UMingRTSDevelopmentTaskOptimizer::Deinitialize()
{
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Development Task Optimizer deinitialized"));
    Super::Deinitialize();
}

//=========================================================================
// Task Management - Implementation
//=========================================================================

FOptimizedDevTask UMingRTSDevelopmentTaskOptimizer::AddDevelopmentTask(
    const FString& TaskName,
    const FString& Description,
    EDevelopmentPriority Priority,
    float EstimatedHours,
    const TArray<FString>& Dependencies,
    const TArray<EDevelopmentResource>& RequiredResources)
{
    FOptimizedDevTask Task;
    Task.TaskId = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphens);
    Task.TaskName = TaskName;
    Task.TaskDescription = Description;
    Task.Priority = Priority;
    Task.EstimatedHours = EstimatedHours;
    Task.Dependencies = Dependencies;
    Task.RequiredResources = RequiredResources;
    
    // Calculate complexity score based on estimates and dependencies
    Task.ComplexityScore = FMath::Max(1.0f, EstimatedHours / 4.0f + Dependencies.Num() * 0.5f);
    
    // Initial risk factor based on priority and complexity
    Task.RiskFactor = static_cast<float>(Priority) * 0.2f + Task.ComplexityScore * 0.1f;
    Task.RiskFactor = FMath::Clamp(Task.RiskFactor, 1.0f, 5.0f);
    
    DevelopmentTasks.Add(Task);
    
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Added task: %s (Priority: %d, Est: %.1fh)"), 
        *TaskName, static_cast<int32>(Priority), EstimatedHours);
    
    return Task;
}

void UMingRTSDevelopmentTaskOptimizer::RemoveDevelopmentTask(const FString& TaskId)
{
    for (int32 i = DevelopmentTasks.Num() - 1; i >= 0; --i)
    {
        if (DevelopmentTasks[i].TaskId == TaskId)
        {
            DevelopmentTasks.RemoveAt(i);
            UE_LOG(LogMingTaskOptimizer, Log, TEXT("Removed task: %s"), *TaskId);
            break;
        }
    }
}

void UMingRTSDevelopmentTaskOptimizer::UpdateTaskStatus(const FString& TaskId, EDevelopmentStatus NewStatus)
{
    for (FOptimizedDevTask& Task : DevelopmentTasks)
    {
        if (Task.TaskId == TaskId)
        {
            Task.Status = NewStatus;
            
            if (NewStatus == EDevelopmentStatus::Completed)
            {
                Task.CompletionTime = FDateTime::Now();
                Task.EfficiencyScore = CalculateTaskEfficiency(Task);
            }
            else if (NewStatus == EDevelopmentStatus::InProgress && Task.StartTime == FDateTime::MinValue())
            {
                Task.StartTime = FDateTime::Now();
            }
            
            OnTaskStatusChanged.Broadcast(TaskId, NewStatus);
            
            UE_LOG(LogMingTaskOptimizer, Log, TEXT("Task %s status updated to: %d"), 
                *TaskId, static_cast<int32>(NewStatus));
            
            // Auto-rebalance if enabled
            if (bRealTimeOptimizationEnabled && NewStatus == EDevelopmentStatus::Completed)
            {
                AutoRebalanceSchedule();
            }
            
            break;
        }
    }
}

void UMingRTSDevelopmentTaskOptimizer::UpdateTaskProgress(const FString& TaskId, float HoursSpent)
{
    for (FOptimizedDevTask& Task : DevelopmentTasks)
    {
        if (Task.TaskId == TaskId)
        {
            Task.ActualHours = HoursSpent;
            
            // Update efficiency prediction
            if (Task.EstimatedHours > 0)
            {
                float ProgressRatio = HoursSpent / Task.EstimatedHours;
                UE_LOG(LogMingTaskOptimizer, Verbose, TEXT("Task %s progress: %.1f%%"), 
                    *TaskId, ProgressRatio * 100.0f);
            }
            
            break;
        }
    }
}

TArray<FOptimizedDevTask> UMingRTSDevelopmentTaskOptimizer::GetAllTasks()
{
    return DevelopmentTasks;
}

TArray<FOptimizedDevTask> UMingRTSDevelopmentTaskOptimizer::GetTasksByStatus(EDevelopmentStatus Status)
{
    TArray<FOptimizedDevTask> Filtered;
    for (const FOptimizedDevTask& Task : DevelopmentTasks)
    {
        if (Task.Status == Status)
        {
            Filtered.Add(Task);
        }
    }
    return Filtered;
}

TArray<FOptimizedDevTask> UMingRTSDevelopmentTaskOptimizer::GetTasksByPriority(EDevelopmentPriority Priority)
{
    TArray<FOptimizedDevTask> Filtered;
    for (const FOptimizedDevTask& Task : DevelopmentTasks)
    {
        if (Task.Priority == Priority)
        {
            Filtered.Add(Task);
        }
    }
    return Filtered;
}

//=========================================================================
// Optimization Algorithms - Implementation
//=========================================================================

FCriticalPathAnalysis UMingRTSDevelopmentTaskOptimizer::AnalyzeCriticalPath()
{
    FCriticalPathAnalysis Analysis;
    
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Analyzing critical path..."));
    
    // Find critical path tasks
    Analysis.CriticalPath = FindCriticalPath();
    
    // Calculate critical path duration
    Analysis.CriticalPathDuration = 0.0f;
    for (const FString& TaskId : Analysis.CriticalPath)
    {
        for (const FOptimizedDevTask& Task : DevelopmentTasks)
        {
            if (Task.TaskId == TaskId)
            {
                Analysis.CriticalPathDuration += Task.EstimatedHours;
                break;
            }
        }
    }
    
    // Identify bottleneck tasks (high complexity, many dependents)
    for (const FOptimizedDevTask& Task : DevelopmentTasks)
    {
        int32 DependentCount = 0;
        for (const FOptimizedDevTask& Other : DevelopmentTasks)
        {
            if (Other.Dependencies.Contains(Task.TaskId))
            {
                DependentCount++;
            }
        }
        
        if (DependentCount >= 3 || Task.ComplexityScore >= 3.0f)
        {
            Analysis.BottleneckTasks.Add(Task.TaskId);
        }
    }
    
    // Find parallelizable tasks (no dependencies between them)
    for (const FOptimizedDevTask& Task : DevelopmentTasks)
    {
        bool bCanParallelize = true;
        for (const FString& DepId : Task.Dependencies)
        {
            for (const FOptimizedDevTask& Other : DevelopmentTasks)
            {
                if (Other.TaskId == DepId && Other.Dependencies.Contains(Task.TaskId))
                {
                    bCanParallelize = false;
                    break;
                }
            }
        }
        
        if (bCanParallelize && Task.Dependencies.Num() == 0)
        {
            Analysis.ParallelizableTasks.Add(Task.TaskId);
        }
    }
    
    // Calculate slack time
    Analysis.SlackTime = 0.0f;
    for (const FOptimizedDevTask& Task : DevelopmentTasks)
    {
        if (!Analysis.CriticalPath.Contains(Task.TaskId))
        {
            // Non-critical tasks have slack
            Analysis.SlackTime += Task.EstimatedHours * 0.2f;
        }
    }
    
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Critical path: %d tasks, Duration: %.1fh, Bottlenecks: %d"),
        Analysis.CriticalPath.Num(), Analysis.CriticalPathDuration, Analysis.BottleneckTasks.Num());
    
    return Analysis;
}

FOptimizedSchedule UMingRTSDevelopmentTaskOptimizer::GenerateOptimizedSchedule()
{
    FOptimizedSchedule Schedule;
    
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Generating optimized schedule..."));
    
    // Get optimized task order
    Schedule.OrderedTasks = OptimizeTaskOrder();
    
    // Calculate total hours
    Schedule.TotalEstimatedHours = 0.0f;
    for (const FOptimizedDevTask& Task : Schedule.OrderedTasks)
    {
        Schedule.TotalEstimatedHours += Task.EstimatedHours;
    }
    
    // Optimize resource allocation
    Schedule.ResourceAllocations = OptimizeResourceAllocation();
    
    // Calculate parallelization factor
    TArray<TArray<FString>> ParallelGroups = IdentifyParallelTaskGroups();
    Schedule.ParallelizationFactor = ParallelGroups.Num() > 0 ? ParallelGroups[0].Num() : 1;
    
    // Calculate optimized duration (accounting for parallelism)
    FCriticalPathAnalysis CriticalPath = AnalyzeCriticalPath();
    Schedule.OptimizedDuration = CriticalPath.CriticalPathDuration;
    
    // Calculate efficiency gain
    if (Schedule.TotalEstimatedHours > 0)
    {
        Schedule.EfficiencyGain = (1.0f - (Schedule.OptimizedDuration / Schedule.TotalEstimatedHours)) * 100.0f;
    }
    
    // Estimate completion
    Schedule.ProjectedCompletion = FDateTime::Now() + FTimespan::FromHours(Schedule.OptimizedDuration / 8.0f); // Assuming 8h work days
    
    CurrentOptimizedSchedule = Schedule;
    
    OnScheduleOptimized.Broadcast(Schedule);
    
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Optimized schedule: Total=%.1fh, Duration=%.1fh, Gain=%.1f%%"),
        Schedule.TotalEstimatedHours, Schedule.OptimizedDuration, Schedule.EfficiencyGain);
    
    return Schedule;
}

TArray<FOptimizedDevTask> UMingRTSDevelopmentTaskOptimizer::OptimizeTaskOrder()
{
    // Topological sort with priority weighting
    TArray<FOptimizedDevTask> Sorted = TopologicalSort();
    
    // Reorder based on critical path weight and resource availability
    Sorted.Sort([](const FOptimizedDevTask& A, const FOptimizedDevTask& B)
    {
        float WeightA = A.GetCriticalPathWeight();
        float WeightB = B.GetCriticalPathWeight();
        
        // Higher weight = earlier in queue
        return WeightA > WeightB;
    });
    
    // Assign optimized order
    for (int32 i = 0; i < Sorted.Num(); ++i)
    {
        for (FOptimizedDevTask& Task : DevelopmentTasks)
        {
            if (Task.TaskId == Sorted[i].TaskId)
            {
                Task.OptimizedOrder = i + 1;
                break;
            }
        }
    }
    
    return Sorted;
}

TMap<EDevelopmentResource, TArray<FResourceAllocation>> UMingRTSDevelopmentTaskOptimizer::OptimizeResourceAllocation()
{
    TMap<EDevelopmentResource, TArray<FResourceAllocation>> Allocations;
    
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Optimizing resource allocation..."));
    
    // Group tasks by required resources
    for (const FOptimizedDevTask& Task : DevelopmentTasks)
    {
        for (EDevelopmentResource Resource : Task.RequiredResources)
        {
            FResourceAllocation Allocation;
            Allocation.TaskId = Task.TaskId;
            Allocation.ResourceType = Resource;
            Allocation.AllocatedHours = Task.EstimatedHours;
            
            Allocations.FindOrAdd(Resource).Add(Allocation);
        }
    }
    
    // Sort allocations by priority within each resource type
    for (auto& Pair : Allocations)
    {
        Pair.Value.Sort([this](const FResourceAllocation& A, const FResourceAllocation& B)
        {
            FOptimizedDevTask TaskA, TaskB;
            
            for (const FOptimizedDevTask& Task : DevelopmentTasks)
            {
                if (Task.TaskId == A.TaskId) TaskA = Task;
                if (Task.TaskId == B.TaskId) TaskB = Task;
            }
            
            return TaskA.Priority > TaskB.Priority;
        });
    }
    
    OnResourcesReallocated.Broadcast(Allocations);
    
    return Allocations;
}

TArray<TArray<FString>> UMingRTSDevelopmentTaskOptimizer::IdentifyParallelTaskGroups()
{
    TArray<TArray<FString>> ParallelGroups;
    
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Identifying parallel task groups..."));
    
    // Group tasks that have no dependencies on each other
    TArray<FString> CurrentGroup;
    
    for (const FOptimizedDevTask& Task : DevelopmentTasks)
    {
        if (Task.Status != EDevelopmentStatus::Completed)
        {
            // Check if this task can be added to current group
            bool bCanParallelize = true;
            
            for (const FString& GroupTaskId : CurrentGroup)
            {
                // Check if tasks depend on each other
                if (Task.Dependencies.Contains(GroupTaskId))
                {
                    bCanParallelize = false;
                    break;
                }
                
                // Check reverse dependency
                for (const FOptimizedDevTask& Other : DevelopmentTasks)
                {
                    if (Other.TaskId == GroupTaskId && Other.Dependencies.Contains(Task.TaskId))
                    {
                        bCanParallelize = false;
                        break;
                    }
                }
            }
            
            if (bCanParallelize && CurrentGroup.Num() < 5) // Max 5 parallel tasks
            {
                CurrentGroup.Add(Task.TaskId);
            }
            else if (CurrentGroup.Num() > 0)
            {
                ParallelGroups.Add(CurrentGroup);
                CurrentGroup.Empty();
                CurrentGroup.Add(Task.TaskId);
            }
        }
    }
    
    if (CurrentGroup.Num() > 0)
    {
        ParallelGroups.Add(CurrentGroup);
    }
    
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Found %d parallel groups"), ParallelGroups.Num());
    
    return ParallelGroups;
}

TArray<FString> UMingRTSDevelopmentTaskOptimizer::ResolveDependencies()
{
    TArray<FString> Issues;
    
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Resolving dependencies..."));
    
    // Check for circular dependencies
    for (const FOptimizedDevTask& Task : DevelopmentTasks)
    {
        for (const FString& DepId : Task.Dependencies)
        {
            for (const FOptimizedDevTask& DepTask : DevelopmentTasks)
            {
                if (DepTask.TaskId == DepId)
                {
                    // Check if dependency depends on this task (circular)
                    if (DepTask.Dependencies.Contains(Task.TaskId))
                    {
                        Issues.Add(FString::Printf(TEXT("Circular dependency: %s <-> %s"), 
                            *Task.TaskName, *DepTask.TaskName));
                    }
                    
                    // Check if dependency is completed
                    if (DepTask.Status == EDevelopmentStatus::Pending && Task.Status != EDevelopmentStatus::Pending)
                    {
                        Issues.Add(FString::Printf(TEXT("Blocked: %s depends on pending %s"),
                            *Task.TaskName, *DepTask.TaskName));
                    }
                    
                    break;
                }
            }
        }
    }
    
    return Issues;
}

//=========================================================================
// Scheduling and Planning - Implementation
//=========================================================================

TArray<FString> UMingRTSDevelopmentTaskOptimizer::GenerateDevelopmentRoadmap()
{
    TArray<FString> Roadmap;
    
    Roadmap.Add(TEXT("# MingGoRTS Development Roadmap (Optimized)"));
    Roadmap.Add(TEXT(""));
    
    FOptimizedSchedule Schedule = GenerateOptimizedSchedule();
    
    // Phase 1: Foundation
    Roadmap.Add(TEXT("## Phase 1: Foundation (Weeks 1-2)"));
    int32 Phase1Tasks = 0;
    for (const FOptimizedDevTask& Task : Schedule.OrderedTasks)
    {
        if (Task.OptimizedOrder <= 5 && Phase1Tasks < 5)
        {
            Roadmap.Add(FString::Printf(TEXT("- [ ] %s (%.1fh) - Priority: %d"), 
                *Task.TaskName, Task.EstimatedHours, static_cast<int32>(Task.Priority)));
            Phase1Tasks++;
        }
    }
    
    // Phase 2: Core Features
    Roadmap.Add(TEXT(""));
    Roadmap.Add(TEXT("## Phase 2: Core Features (Weeks 3-4)"));
    int32 Phase2Tasks = 0;
    for (const FOptimizedDevTask& Task : Schedule.OrderedTasks)
    {
        if (Task.OptimizedOrder > 5 && Task.OptimizedOrder <= 15 && Phase2Tasks < 10)
        {
            Roadmap.Add(FString::Printf(TEXT("- [ ] %s (%.1fh) - Priority: %d"),
                *Task.TaskName, Task.EstimatedHours, static_cast<int32>(Task.Priority)));
            Phase2Tasks++;
        }
    }
    
    // Phase 3: Polish & Optimization
    Roadmap.Add(TEXT(""));
    Roadmap.Add(TEXT("## Phase 3: Polish & Optimization (Weeks 5-6)"));
    int32 Phase3Tasks = 0;
    for (const FOptimizedDevTask& Task : Schedule.OrderedTasks)
    {
        if (Task.OptimizedOrder > 15 && Phase3Tasks < 10)
        {
            Roadmap.Add(FString::Printf(TEXT("- [ ] %s (%.1fh) - Priority: %d"),
                *Task.TaskName, Task.EstimatedHours, static_cast<int32>(Task.Priority)));
            Phase3Tasks++;
        }
    }
    
    // Summary
    Roadmap.Add(TEXT(""));
    Roadmap.Add(TEXT("## Summary"));
    Roadmap.Add(FString::Printf(TEXT("- Total Tasks: %d"), DevelopmentTasks.Num()));
    Roadmap.Add(FString::Printf(TEXT("- Total Estimated Hours: %.1f"), Schedule.TotalEstimatedHours));
    Roadmap.Add(FString::Printf(TEXT("- Optimized Duration: %.1f hours (%.1f days @ 8h/day)"), 
        Schedule.OptimizedDuration, Schedule.OptimizedDuration / 8.0f));
    Roadmap.Add(FString::Printf(TEXT("- Efficiency Gain: %.1f%%"), Schedule.EfficiencyGain));
    Roadmap.Add(FString::Printf(TEXT("- Projected Completion: %s"), *Schedule.ProjectedCompletion.ToString()));
    
    return Roadmap;
}

FDateTime UMingRTSDevelopmentTaskOptimizer::EstimateCompletionDate()
{
    FOptimizedSchedule Schedule = GenerateOptimizedSchedule();
    return Schedule.ProjectedCompletion;
}

TMap<FString, float> UMingRTSDevelopmentTaskOptimizer::CalculateTaskBuffers()
{
    TMap<FString, float> Buffers;
    FCriticalPathAnalysis Analysis = AnalyzeCriticalPath();
    
    for (const FOptimizedDevTask& Task : DevelopmentTasks)
    {
        if (Analysis.CriticalPath.Contains(Task.TaskId))
        {
            // Critical tasks get minimal buffer
            Buffers.Add(Task.TaskId, Task.EstimatedHours * 0.1f);
        }
        else
        {
            // Non-critical tasks get more buffer based on slack
            Buffers.Add(Task.TaskId, Task.EstimatedHours * 0.3f);
        }
    }
    
    return Buffers;
}

TArray<FOptimizedDevTask> UMingRTSDevelopmentTaskOptimizer::GenerateSprintPlan(float SprintDurationHours)
{
    TArray<FOptimizedDevTask> SprintTasks;
    float CurrentHours = 0.0f;
    
    FOptimizedSchedule Schedule = GenerateOptimizedSchedule();
    
    for (const FOptimizedDevTask& Task : Schedule.OrderedTasks)
    {
        if (Task.Status == EDevelopmentStatus::Pending && CurrentHours + Task.EstimatedHours <= SprintDurationHours)
        {
            SprintTasks.Add(Task);
            CurrentHours += Task.EstimatedHours;
        }
    }
    
    return SprintTasks;
}

TArray<FString> UMingRTSDevelopmentTaskOptimizer::IdentifyCompressionOpportunities()
{
    TArray<FString> Opportunities;
    
    // Find tasks that can be compressed (high duration, lower priority)
    for (const FOptimizedDevTask& Task : DevelopmentTasks)
    {
        if (Task.EstimatedHours > 8.0f && Task.Priority <= EDevelopmentPriority::Medium)
        {
            Opportunities.Add(FString::Printf(TEXT("%s: Can reduce from %.1fh to %.1fh by simplifying scope"),
                *Task.TaskName, Task.EstimatedHours, Task.EstimatedHours * 0.7f));
        }
    }
    
    // Find parallelization opportunities
    TArray<TArray<FString>> ParallelGroups = IdentifyParallelTaskGroups();
    for (int32 i = 0; i < ParallelGroups.Num(); ++i)
    {
        if (ParallelGroups[i].Num() < 3)
        {
            Opportunities.Add(FString::Printf(TEXT("Parallel Group %d: Can add more parallel tasks"), i + 1));
        }
    }
    
    return Opportunities;
}

//=========================================================================
// Resource Management - Implementation
//=========================================================================

void UMingRTSDevelopmentTaskOptimizer::AllocateResources(
    const FString& TaskId, 
    const TArray<EDevelopmentResource>& Resources)
{
    for (FOptimizedDevTask& Task : DevelopmentTasks)
    {
        if (Task.TaskId == TaskId)
        {
            Task.RequiredResources = Resources;
            UE_LOG(LogMingTaskOptimizer, Log, TEXT("Allocated resources for task %s: %d types"),
                *TaskId, Resources.Num());
            break;
        }
    }
}

TMap<EDevelopmentResource, float> UMingRTSDevelopmentTaskOptimizer::GetResourceUtilization()
{
    TMap<EDevelopmentResource, float> Utilization;
    
    // Calculate utilization for each resource type
    for (int32 i = 0; i < static_cast<int32>(EDevelopmentResource::DevOps) + 1; ++i)
    {
        EDevelopmentResource Resource = static_cast<EDevelopmentResource>(i);
        
        float TotalHours = 0.0f;
        float AllocatedHours = 0.0f;
        
        for (const FOptimizedDevTask& Task : DevelopmentTasks)
        {
            if (Task.RequiredResources.Contains(Resource))
            {
                TotalHours += Task.EstimatedHours;
                if (Task.Status == EDevelopmentStatus::Completed)
                {
                    AllocatedHours += Task.EstimatedHours;
                }
            }
        }
        
        float Util = TotalHours > 0 ? (AllocatedHours / TotalHours) * 100.0f : 0.0f;
        Utilization.Add(Resource, Util);
    }
    
    return Utilization;
}

TArray<EDevelopmentResource> UMingRTSDevelopmentTaskOptimizer::IdentifyResourceBottlenecks()
{
    TArray<EDevelopmentResource> Bottlenecks;
    TMap<EDevelopmentResource, float> Utilization = GetResourceUtilization();
    
    for (const auto& Pair : Utilization)
    {
        if (Pair.Value > 80.0f) // Over 80% utilization is a bottleneck
        {
            Bottlenecks.Add(Pair.Key);
        }
    }
    
    return Bottlenecks;
}

void UMingRTSDevelopmentTaskOptimizer::BalanceResourceLoad()
{
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Balancing resource load..."));
    
    TArray<EDevelopmentResource> Bottlenecks = IdentifyResourceBottlenecks();
    
    for (EDevelopmentResource Bottleneck : Bottlenecks)
    {
        // Find tasks that can be reassigned to other resources
        for (FOptimizedDevTask& Task : DevelopmentTasks)
        {
            if (Task.RequiredResources.Contains(Bottleneck) && Task.Status == EDevelopmentStatus::Pending)
            {
                // Try to find alternative resource
                for (int32 i = 0; i < static_cast<int32>(EDevelopmentResource::DevOps) + 1; ++i)
                {
                    EDevelopmentResource AltResource = static_cast<EDevelopmentResource>(i);
                    
                    if (AltResource != Bottleneck && !Task.RequiredResources.Contains(AltResource))
                    {
                        // Check if alternative has capacity
                        // Simplified: just add it as secondary resource
                        Task.RequiredResources.Add(AltResource);
                        break;
                    }
                }
            }
        }
    }
}

//=========================================================================
// Metrics and Reporting - Implementation
//=========================================================================

FDevelopmentMetrics UMingRTSDevelopmentTaskOptimizer::CalculateDevelopmentMetrics()
{
    FDevelopmentMetrics Metrics;
    
    Metrics.TotalTasks = DevelopmentTasks.Num();
    
    for (const FOptimizedDevTask& Task : DevelopmentTasks)
    {
        switch (Task.Status)
        {
        case EDevelopmentStatus::Completed:
            Metrics.CompletedTasks++;
            break;
        case EDevelopmentStatus::InProgress:
        case EDevelopmentStatus::Testing:
        case EDevelopmentStatus::Review:
            Metrics.InProgressTasks++;
            break;
        case EDevelopmentStatus::Blocked:
            Metrics.BlockedTasks++;
            break;
        default:
            break;
        }
    }
    
    // Calculate average completion time
    float TotalCompletionTime = 0.0f;
    int32 CompletedCount = 0;
    
    for (const FOptimizedDevTask& Task : DevelopmentTasks)
    {
        if (Task.Status == EDevelopmentStatus::Completed && Task.CompletionTime > Task.StartTime)
        {
            FTimespan Duration = Task.CompletionTime - Task.StartTime;
            TotalCompletionTime += Duration.GetTotalHours();
            CompletedCount++;
        }
    }
    
    if (CompletedCount > 0)
    {
        Metrics.AverageCompletionTime = TotalCompletionTime / CompletedCount;
    }
    
    // Calculate velocity (tasks per day)
    if (Metrics.AverageCompletionTime > 0)
    {
        Metrics.Velocity = 8.0f / Metrics.AverageCompletionTime; // Assuming 8-hour work day
    }
    
    // Calculate resource utilization
    TMap<EDevelopmentResource, float> Utilization = GetResourceUtilization();
    float TotalUtil = 0.0f;
    int32 Count = 0;
    
    for (const auto& Pair : Utilization)
    {
        TotalUtil += Pair.Value;
        Count++;
    }
    
    if (Count > 0)
    {
        Metrics.ResourceUtilization = TotalUtil / Count;
    }
    
    return Metrics;
}

FString UMingRTSDevelopmentTaskOptimizer::GenerateEfficiencyReport()
{
    FString Report = TEXT("=== Development Efficiency Report ===\n\n");
    
    FDevelopmentMetrics Metrics = CalculateDevelopmentMetrics();
    FOptimizedSchedule Schedule = GenerateOptimizedSchedule();
    FCriticalPathAnalysis CriticalPath = AnalyzeCriticalPath();
    
    // Summary
    Report += TEXT("## Summary\n\n");
    Report += FString::Printf(TEXT("Total Tasks: %d\n"), Metrics.TotalTasks);
    Report += FString::Printf(TEXT("Completed: %d (%.1f%%)\n"), 
        Metrics.CompletedTasks, 
        Metrics.TotalTasks > 0 ? (float)Metrics.CompletedTasks / Metrics.TotalTasks * 100.0f : 0.0f);
    Report += FString::Printf(TEXT("In Progress: %d\n"), Metrics.InProgressTasks);
    Report += FString::Printf(TEXT("Blocked: %d\n"), Metrics.BlockedTasks);
    
    // Performance
    Report += TEXT("\n## Performance Metrics\n\n");
    Report += FString::Printf(TEXT("Average Completion Time: %.1f hours\n"), Metrics.AverageCompletionTime);
    Report += FString::Printf(TEXT("Velocity: %.1f tasks/day\n"), Metrics.Velocity);
    Report += FString::Printf(TEXT("Resource Utilization: %.1f%%\n"), Metrics.ResourceUtilization);
    
    // Schedule
    Report += TEXT("\n## Schedule Analysis\n\n");
    Report += FString::Printf(TEXT("Total Estimated Hours: %.1f\n"), Schedule.TotalEstimatedHours);
    Report += FString::Printf(TEXT("Optimized Duration: %.1f hours\n"), Schedule.OptimizedDuration);
    Report += FString::Printf(TEXT("Efficiency Gain: %.1f%%\n"), Schedule.EfficiencyGain);
    Report += FString::Printf(TEXT("Parallelization Factor: %d\n"), Schedule.ParallelizationFactor);
    Report += FString::Printf(TEXT("Critical Path Duration: %.1f hours\n"), CriticalPath.CriticalPathDuration);
    Report += FString::Printf(TEXT("Projected Completion: %s\n"), *Schedule.ProjectedCompletion.ToString());
    
    // Bottlenecks
    Report += TEXT("\n## Bottlenecks\n\n");
    if (CriticalPath.BottleneckTasks.Num() > 0)
    {
        Report += FString::Printf(TEXT("Identified %d bottleneck tasks:\n"), CriticalPath.BottleneckTasks.Num());
        for (const FString& TaskId : CriticalPath.BottleneckTasks)
        {
            for (const FOptimizedDevTask& Task : DevelopmentTasks)
            {
                if (Task.TaskId == TaskId)
                {
                    Report += FString::Printf(TEXT("  - %s (Complexity: %.1f)\n"), 
                        *Task.TaskName, Task.ComplexityScore);
                    break;
                }
            }
        }
    }
    else
    {
        Report += TEXT("No significant bottlenecks identified.\n");
    }
    
    // Recommendations
    Report += TEXT("\n## Recommendations\n\n");
    TArray<FString> Opportunities = IdentifyCompressionOpportunities();
    for (const FString& Opportunity : Opportunities)
    {
        Report += FString::Printf(TEXT("- %s\n"), *Opportunity);
    }
    
    return Report;
}

TArray<float> UMingRTSDevelopmentTaskOptimizer::CalculateVelocityTrend(int32 Days)
{
    TArray<float> Trend;
    
    // Simulate velocity calculation (in real implementation, would use historical data)
    for (int32 i = 0; i < Days; ++i)
    {
        float Velocity = FMath::RandRange(2.0f, 5.0f); // Random velocity between 2-5 tasks/day
        Trend.Add(Velocity);
    }
    
    return Trend;
}

float UMingRTSDevelopmentTaskOptimizer::PredictCompletionProbability(FDateTime TargetDate)
{
    FDateTime EstimatedCompletion = EstimateCompletionDate();
    
    if (EstimatedCompletion <= TargetDate)
    {
        return 100.0f;
    }
    else
    {
        FTimespan Delta = EstimatedCompletion - TargetDate;
        float DaysOverdue = Delta.GetTotalDays();
        
        // Probability decreases as we go over target
        return FMath::Max(0.0f, 100.0f - (DaysOverdue * 10.0f));
    }
}

TMap<FDateTime, float> UMingRTSDevelopmentTaskOptimizer::GetBurndownData(int32 Days)
{
    TMap<FDateTime, float> Burndown;
    
    float TotalHours = 0.0f;
    for (const FOptimizedDevTask& Task : DevelopmentTasks)
    {
        if (Task.Status != EDevelopmentStatus::Completed)
        {
            TotalHours += Task.EstimatedHours;
        }
    }
    
    FDateTime StartDate = FDateTime::Now();
    float RemainingHours = TotalHours;
    
    for (int32 i = 0; i < Days; ++i)
    {
        FDateTime Day = StartDate + FTimespan::FromDays(i);
        
        // Simulate burndown (in real implementation, would use actual completion data)
        float CompletedToday = FMath::RandRange(4.0f, 12.0f); // 4-12 hours completed per day
        RemainingHours = FMath::Max(0.0f, RemainingHours - CompletedToday);
        
        Burndown.Add(Day, RemainingHours);
    }
    
    return Burndown;
}

//=========================================================================
// Risk Management - Implementation
//=========================================================================

TMap<FString, float> UMingRTSDevelopmentTaskOptimizer::AssessTaskRisks()
{
    TMap<FString, float> Risks;
    
    for (const FOptimizedDevTask& Task : DevelopmentTasks)
    {
        // Calculate risk score (0-100)
        float Risk = Task.RiskFactor * 10.0f; // Base risk
        
        // Add risk for overdue tasks
        if (Task.ActualHours > Task.EstimatedHours * 1.2f)
        {
            Risk += 20.0f;
        }
        
        // Add risk for blocked tasks
        if (Task.Status == EDevelopmentStatus::Blocked)
        {
            Risk += 30.0f;
        }
        
        // Add risk for high complexity
        Risk += Task.ComplexityScore * 5.0f;
        
        Risks.Add(Task.TaskId, FMath::Clamp(Risk, 0.0f, 100.0f));
    }
    
    return Risks;
}

TArray<FOptimizedDevTask> UMingRTSDevelopmentTaskOptimizer::IdentifyHighRiskTasks()
{
    TArray<FOptimizedDevTask> HighRisk;
    TMap<FString, float> Risks = AssessTaskRisks();
    
    for (const auto& Pair : Risks)
    {
        if (Pair.Value > 50.0f) // Risk > 50% is high
        {
            for (const FOptimizedDevTask& Task : DevelopmentTasks)
            {
                if (Task.TaskId == Pair.Key)
                {
                    HighRisk.Add(Task);
                    break;
                }
            }
        }
    }
    
    return HighRisk;
}

TMap<FString, TArray<FString>> UMingRTSDevelopmentTaskOptimizer::SuggestMitigationStrategies()
{
    TMap<FString, TArray<FString>> Strategies;
    TArray<FOptimizedDevTask> HighRisk = IdentifyHighRiskTasks();
    
    for (const FOptimizedDevTask& Task : HighRisk)
    {
        TArray<FString> TaskStrategies;
        
        if (Task.ComplexityScore > 3.0f)
        {
            TaskStrategies.Add(TEXT("Break into smaller sub-tasks"));
            TaskStrategies.Add(TEXT("Add technical review checkpoint"));
        }
        
        if (Task.Dependencies.Num() > 3)
        {
            TaskStrategies.Add(TEXT("Resolve dependencies early"));
            TaskStrategies.Add(TEXT("Consider parallel implementation"));
        }
        
        if (Task.RiskFactor > 3.0f)
        {
            TaskStrategies.Add(TEXT("Allocate senior developer"));
            TaskStrategies.Add(TEXT("Add 20% time buffer"));
        }
        
        if (TaskStrategies.Num() > 0)
        {
            Strategies.Add(Task.TaskId, TaskStrategies);
        }
    }
    
    return Strategies;
}

FOptimizedSchedule UMingRTSDevelopmentTaskOptimizer::CalculateRiskAdjustedSchedule()
{
    FOptimizedSchedule BaseSchedule = GenerateOptimizedSchedule();
    TMap<FString, float> Risks = AssessTaskRisks();
    
    // Add buffer time for high-risk tasks
    float RiskBuffer = 0.0f;
    for (const auto& Pair : Risks)
    {
        if (Pair.Value > 50.0f)
        {
            RiskBuffer += 4.0f; // 4 hours buffer per high-risk task
        }
    }
    
    BaseSchedule.OptimizedDuration += RiskBuffer;
    BaseSchedule.ProjectedCompletion += FTimespan::FromHours(RiskBuffer / 8.0f);
    
    return BaseSchedule;
}

//=========================================================================
// Automation - Implementation
//=========================================================================

void UMingRTSDevelopmentTaskOptimizer::AutoAssignTasks()
{
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Auto-assigning tasks..."));
    
    // Simple auto-assignment based on resource availability
    TMap<EDevelopmentResource, int32> ResourceLoad;
    
    for (const FOptimizedDevTask& Task : DevelopmentTasks)
    {
        if (Task.Status == EDevelopmentStatus::Pending && Task.RequiredResources.Num() == 0)
        {
            // Find least loaded resource
            EDevelopmentResource BestResource = EDevelopmentResource::Programmer;
            int32 MinLoad = MAX_int32;
            
            for (int32 i = 0; i < static_cast<int32>(EDevelopmentResource::DevOps) + 1; ++i)
            {
                EDevelopmentResource Resource = static_cast<EDevelopmentResource>(i);
                int32 Load = ResourceLoad.FindRef(Resource);
                
                if (Load < MinLoad)
                {
                    MinLoad = Load;
                    BestResource = Resource;
                }
            }
            
            // Assign task to best resource
            for (FOptimizedDevTask& MutableTask : DevelopmentTasks)
            {
                if (MutableTask.TaskId == Task.TaskId)
                {
                    MutableTask.RequiredResources.Add(BestResource);
                    ResourceLoad.FindOrAdd(BestResource)++;
                    break;
                }
            }
        }
    }
}

void UMingRTSDevelopmentTaskOptimizer::AutoRebalanceSchedule()
{
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Auto-rebalancing schedule..."));
    
    // Re-optimize when tasks complete
    FOptimizedSchedule NewSchedule = GenerateOptimizedSchedule();
    
    // Check for newly unblocked tasks
    for (FOptimizedDevTask& Task : DevelopmentTasks)
    {
        if (Task.Status == EDevelopmentStatus::Blocked)
        {
            bool bAllDepsCompleted = true;
            
            for (const FString& DepId : Task.Dependencies)
            {
                bool bDepCompleted = false;
                for (const FOptimizedDevTask& DepTask : DevelopmentTasks)
                {
                    if (DepTask.TaskId == DepId && DepTask.Status == EDevelopmentStatus::Completed)
                    {
                        bDepCompleted = true;
                        break;
                    }
                }
                
                if (!bDepCompleted)
                {
                    bAllDepsCompleted = false;
                    break;
                }
            }
            
            if (bAllDepsCompleted)
            {
                Task.Status = EDevelopmentStatus::Pending;
                UE_LOG(LogMingTaskOptimizer, Log, TEXT("Task %s unblocked"), *Task.TaskName);
            }
        }
    }
}

void UMingRTSDevelopmentTaskOptimizer::EnableRealTimeOptimization(bool bEnabled)
{
    bRealTimeOptimizationEnabled = bEnabled;
    
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Real-time optimization: %s"),
        bEnabled ? TEXT("Enabled") : TEXT("Disabled"));
    
    if (bEnabled)
    {
        // Setup timer for periodic re-optimization
        UWorld* World = GetWorld();
        if (World)
        {
            FTimerHandle OptimizationTimer;
            World->GetTimerManager().SetTimer(OptimizationTimer, [this]()
            {
                AutoRebalanceSchedule();
            }, 300.0f, true); // Every 5 minutes
        }
    }
}

void UMingRTSDevelopmentTaskOptimizer::ImportTasks(const FString& SourceData)
{
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Importing tasks from external source..."));
    
    // Parse source data and create tasks
    // Implementation would parse JSON, CSV, or other formats
}

FString UMingRTSDevelopmentTaskOptimizer::ExportOptimizedSchedule()
{
    FOptimizedSchedule Schedule = GenerateOptimizedSchedule();
    
    FString Export = TEXT("{\n");
    Export += TEXT("  \"schedule\": {\n");
    Export += FString::Printf(TEXT("    \"totalHours\": %.1f,\n"), Schedule.TotalEstimatedHours);
    Export += FString::Printf(TEXT("    \"durationHours\": %.1f,\n"), Schedule.OptimizedDuration);
    Export += FString::Printf(TEXT("    \"efficiencyGain\": %.1f,\n"), Schedule.EfficiencyGain);
    Export += FString::Printf(TEXT("    \"completion\": \"%s\"\n"), *Schedule.ProjectedCompletion.ToString());
    Export += TEXT("  },\n");
    Export += TEXT("  \"tasks\": [\n");
    
    for (int32 i = 0; i < Schedule.OrderedTasks.Num(); ++i)
    {
        const FOptimizedDevTask& Task = Schedule.OrderedTasks[i];
        Export += TEXT("    {\n");
        Export += FString::Printf(TEXT("      \"id\": \"%s\",\n"), *Task.TaskId);
        Export += FString::Printf(TEXT("      \"name\": \"%s\",\n"), *Task.TaskName);
        Export += FString::Printf(TEXT("      \"order\": %d,\n"), Task.OptimizedOrder);
        Export += FString::Printf(TEXT("      \"hours\": %.1f\n"), Task.EstimatedHours);
        Export += TEXT("    }");
        if (i < Schedule.OrderedTasks.Num() - 1) Export += TEXT(",");
        Export += TEXT("\n");
    }
    
    Export += TEXT("  ]\n");
    Export += TEXT("}\n");
    
    return Export;
}

//=========================================================================
// Private Methods - Implementation
//=========================================================================

void UMingRTSDevelopmentTaskOptimizer::InitializeDefaultTasks()
{
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Initializing default development tasks..."));
    
    // Add tasks based on the available development tasks we identified earlier
    
    // High Priority: Integration Testing
    AddDevelopmentTask(
        TEXT("執行集成測試"),
        TEXT("運行41個測試用例，驗證系統集成"),
        EDevelopmentPriority::High,
        16.0f,
        {},
        { EDevelopmentResource::Tester, EDevelopmentResource::Programmer }
    );
    
    // High Priority: 12 Factions Implementation
    AddDevelopmentTask(
        TEXT("實現12可玩勢力"),
        TEXT("為12個勢力完成屬性、機制、單位設計"),
        EDevelopmentPriority::High,
        200.0f,
        {},
        { EDevelopmentResource::Designer, EDevelopmentResource::Programmer }
    );
    
    // Medium Priority: Collaboration AI Network
    AddDevelopmentTask(
        TEXT("協作AI網絡系統"),
        TEXT("實現多AI智能體協作系統"),
        EDevelopmentPriority::Medium,
        80.0f,
        { /* Depends on AI system completion */ },
        { EDevelopmentResource::Programmer }
    );
    
    // Medium Priority: Ecosystem Simulation
    AddDevelopmentTask(
        TEXT("生態環境模擬系統"),
        TEXT("實現環境變化和生態平衡機制"),
        EDevelopmentPriority::Medium,
        80.0f,
        {},
        { EDevelopmentResource::Programmer, EDevelopmentResource::Designer }
    );
    
    // Medium Priority: Social Dynamics
    AddDevelopmentTask(
        TEXT("社會動態系統"),
        TEXT("實現複雜社會關係網絡"),
        EDevelopmentPriority::Medium,
        80.0f,
        {},
        { EDevelopmentResource::Programmer, EDevelopmentResource::Designer }
    );
    
    // Low Priority: Blockchain Integration
    AddDevelopmentTask(
        TEXT("區塊鏈技術集成"),
        TEXT("實現去中心化資產系統"),
        EDevelopmentPriority::Low,
        40.0f,
        {},
        { EDevelopmentResource::Programmer, EDevelopmentResource::DevOps }
    );
    
    UE_LOG(LogMingTaskOptimizer, Log, TEXT("Initialized %d default tasks"), DevelopmentTasks.Num());
}

void UMingRTSDevelopmentTaskOptimizer::SortTasksByCriticalPath()
{
    DevelopmentTasks.Sort([](const FOptimizedDevTask& A, const FOptimizedDevTask& B)
    {
        return A.GetCriticalPathWeight() > B.GetCriticalPathWeight();
    });
}

void UMingRTSDevelopmentTaskOptimizer::CalculateDependencyGraph()
{
    // Build dependency graph for critical path analysis
    // This is a placeholder for more complex graph algorithms
}

void UMingRTSDevelopmentTaskOptimizer::OptimizeResourcePool()
{
    // Optimize how resources are distributed across tasks
    BalanceResourceLoad();
}

float UMingRTSDevelopmentTaskOptimizer::CalculateTaskEfficiency(const FOptimizedDevTask& Task)
{
    if (Task.ActualHours <= 0 || Task.EstimatedHours <= 0)
    {
        return 0.0f;
    }
    
    // Efficiency = Estimated / Actual (higher is better if under estimate)
    float Efficiency = Task.EstimatedHours / Task.ActualHours;
    
    // Penalize if significantly over estimate
    if (Task.ActualHours > Task.EstimatedHours * 1.5f)
    {
        Efficiency *= 0.5f;
    }
    
    return FMath::Clamp(Efficiency, 0.0f, 2.0f);
}

TArray<FString> UMingRTSDevelopmentTaskOptimizer::FindCriticalPath()
{
    TArray<FString> CriticalPath;
    
    // Simplified critical path algorithm
    // Find tasks with highest critical path weight
    TArray<FOptimizedDevTask> Sorted = DevelopmentTasks;
    Sorted.Sort([](const FOptimizedDevTask& A, const FOptimizedDevTask& B)
    {
        return A.GetCriticalPathWeight() > B.GetCriticalPathWeight();
    });
    
    // Take top tasks that form a chain
    for (const FOptimizedDevTask& Task : Sorted)
    {
        if (Task.Status != EDevelopmentStatus::Completed)
        {
            CriticalPath.Add(Task.TaskId);
            
            // Add dependencies
            for (const FString& DepId : Task.Dependencies)
            {
                if (!CriticalPath.Contains(DepId))
                {
                    CriticalPath.Add(DepId);
                }
            }
        }
    }
    
    return CriticalPath;
}

void UMingRTSDevelopmentTaskOptimizer::UpdateScheduleMetrics()
{
    // Update metrics based on current schedule state
    CalculateDevelopmentMetrics();
}

TArray<FOptimizedDevTask> UMingRTSDevelopmentTaskOptimizer::TopologicalSort()
{
    TArray<FOptimizedDevTask> Sorted;
    TArray<FString> Visited;
    TArray<FString> TempMarked;
    
    // Simple topological sort implementation
    for (const FOptimizedDevTask& Task : DevelopmentTasks)
    {
        if (!Visited.Contains(Task.TaskId))
        {
            VisitTaskForTopoSort(Task, Visited, TempMarked, Sorted);
        }
    }
    
    return Sorted;
}

void UMingRTSDevelopmentTaskOptimizer::VisitTaskForTopoSort(
    const FOptimizedDevTask& Task,
    TArray<FString>& Visited,
    TArray<FString>& TempMarked,
    TArray<FOptimizedDevTask>& Sorted)
{
    if (TempMarked.Contains(Task.TaskId))
    {
        // Cycle detected
        UE_LOG(LogMingTaskOptimizer, Warning, TEXT("Circular dependency detected involving %s"), *Task.TaskName);
        return;
    }
    
    if (Visited.Contains(Task.TaskId))
    {
        return;
    }
    
    TempMarked.Add(Task.TaskId);
    
    // Visit dependencies
    for (const FString& DepId : Task.Dependencies)
    {
        for (const FOptimizedDevTask& DepTask : DevelopmentTasks)
        {
            if (DepTask.TaskId == DepId)
            {
                VisitTaskForTopoSort(DepTask, Visited, TempMarked, Sorted);
                break;
            }
        }
    }
    
    TempMarked.Remove(Task.TaskId);
    Visited.Add(Task.TaskId);
    Sorted.Add(Task);
}

TMap<FString, float> UMingRTSDevelopmentTaskOptimizer::CalculateEarlyStartTimes()
{
    TMap<FString, float> EarlyStart;
    
    // Calculate earliest start time for each task
    for (const FOptimizedDevTask& Task : DevelopmentTasks)
    {
        float MaxDepEnd = 0.0f;
        
        for (const FString& DepId : Task.Dependencies)
        {
            if (EarlyStart.Contains(DepId))
            {
                float DepEnd = EarlyStart[DepId];
                
                for (const FOptimizedDevTask& DepTask : DevelopmentTasks)
                {
                    if (DepTask.TaskId == DepId)
                    {
                        DepEnd += DepTask.EstimatedHours;
                        break;
                    }
                }
                
                MaxDepEnd = FMath::Max(MaxDepEnd, DepEnd);
            }
        }
        
        EarlyStart.Add(Task.TaskId, MaxDepEnd);
    }
    
    return EarlyStart;
}

TMap<FString, float> UMingRTSDevelopmentTaskOptimizer::CalculateLateStartTimes()
{
    TMap<FString, float> LateStart;
    
    // Calculate latest start time for each task
    // This is a simplified implementation
    FOptimizedSchedule Schedule = GenerateOptimizedSchedule();
    float ProjectEnd = Schedule.OptimizedDuration;
    
    // Work backwards from project end
    TArray<FOptimizedDevTask> ReverseOrder = DevelopmentTasks;
    ReverseOrder.Sort([](const FOptimizedDevTask& A, const FOptimizedDevTask& B)
    {
        return A.OptimizedOrder > B.OptimizedOrder;
    });
    
    for (const FOptimizedDevTask& Task : ReverseOrder)
    {
        float MinSuccessorStart = ProjectEnd;
        
        // Find tasks that depend on this one
        for (const FOptimizedDevTask& Other : DevelopmentTasks)
        {
            if (Other.Dependencies.Contains(Task.TaskId) && LateStart.Contains(Other.TaskId))
            {
                MinSuccessorStart = FMath::Min(MinSuccessorStart, LateStart[Other.TaskId]);
            }
        }
        
        LateStart.Add(Task.TaskId, MinSuccessorStart - Task.EstimatedHours);
    }
    
    return LateStart;
}

void UMingRTSDevelopmentTaskOptimizer::ApplyScheduleCompression()
{
    // Apply fast-tracking and crashing techniques
    TArray<FString> CompressionOps = IdentifyCompressionOpportunities();
    
    for (const FString& Opportunity : CompressionOps)
    {
        UE_LOG(LogMingTaskOptimizer, Log, TEXT("Schedule compression opportunity: %s"), *Opportunity);
    }
}
