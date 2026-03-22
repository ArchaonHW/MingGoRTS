#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSDevelopmentTaskOptimizer.generated.h"

UENUM(BlueprintType)
enum class EDevelopmentTaskType : uint8
{
    Coding, UMETA(DisplayName = "Coding"),
    Testing, UMETA(DisplayName = "Testing"),
    Documentation, UMETA(DisplayName = "Documentation"),
    Review, UMETA(DisplayName = "Review"),
    Deployment, UMETA(DisplayName = "Deployment")
};

UENUM(BlueprintType)
enum class EDevelopmentStatus : uint8
{
    Pending, UMETA(DisplayName = "Pending"),
    InProgress, UMETA(DisplayName = "In Progress"),
    Completed, UMETA(DisplayName = "Completed"),
    Blocked, UMETA(DisplayName = "Blocked"),
    Cancelled, UMETA(DisplayName = "Cancelled")
};

UENUM(BlueprintType)
enum class EDevelopmentResource : uint8
{
    Developer, UMETA(DisplayName = "Developer"),
    Tester, UMETA(DisplayName = "Tester"),
    Designer, UMETA(DisplayName = "Designer"),
    Manager, UMETA(DisplayName = "Manager"),
    Analyst, UMETA(DisplayName = "Analyst")
};

USTRUCT(BlueprintType)
struct FDevelopmentTask
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Task")
    FString TaskId;

    UPROPERTY(BlueprintReadOnly, Category = "Task")
    FString TaskName;

    UPROPERTY(BlueprintReadOnly, Category = "Task")
    EDevelopmentTaskType TaskType;

    UPROPERTY(BlueprintReadOnly, Category = "Task")
    EDevelopmentStatus Status;

    UPROPERTY(BlueprintReadOnly, Category = "Task")
    int32 EstimatedHours;

    UPROPERTY(BlueprintReadOnly, Category = "Task")
    int32 ActualHours;

    UPROPERTY(BlueprintReadOnly, Category = "Task")
    TArray<EDevelopmentResource> RequiredResources;

    FDevelopmentTask()
        : TaskId(TEXT(""))
        , TaskName(TEXT(""))
        , TaskType(EDevelopmentTaskType::Coding)
        , Status(EDevelopmentStatus::Pending)
        , EstimatedHours(0)
        , ActualHours(0)
    {}
};

/**
 * MingGoRTS Development Task Optimizer
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSDevelopmentTaskOptimizer : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSDevelopmentTaskOptimizer();

    UFUNCTION(BlueprintCallable, Category = "Task Optimizer")
    void InitializeTaskOptimizer();

    UFUNCTION(BlueprintCallable, Category = "Task Optimizer")
    bool AddTask(const FDevelopmentTask& Task);

    UFUNCTION(BlueprintCallable, Category = "Task Optimizer")
    bool UpdateTaskStatus(const FString& TaskId, EDevelopmentStatus NewStatus);

    UFUNCTION(BlueprintPure, Category = "Task Optimizer")
    TArray<FDevelopmentTask> GetAllTasks();

    UFUNCTION(BlueprintCallable, Category = "Task Optimizer")
    bool OptimizeTaskSchedule();

protected:
    UPROPERTY()
    TArray<FDevelopmentTask> TaskList;

    void InitializeDefaultTasks();
};

// Delegate declarations
DECLARE_DYNAMIC_DELEGATE(FOnScheduleOptimized, const FOptimizedSchedule&, OptimizedSchedule);
DECLARE_DYNAMIC_DELEGATE(FOnTaskStatusChanged, const FString&, TaskId, EDevelopmentStatus, NewStatus);
DECLARE_DYNAMIC_DELEGATE(FOnResourcesReallocated, const TMap<EDevelopmentResource, TArray<FResourceAllocation>>&, NewAllocations);

#include "MingRTSDevelopmentTaskOptimizer.generated.h"
