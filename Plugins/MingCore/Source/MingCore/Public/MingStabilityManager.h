#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingStabilityManager.generated.h"

UENUM(BlueprintType)
enum class EMingErrorSeverity : uint8
{
    Info UMETA(DisplayName = "Info"),
    Warning UMETA(DisplayName = "Warning"),
    Error UMETA(DisplayName = "Error"),
    Critical UMETA(DisplayName = "Critical"),
    Fatal UMETA(DisplayName = "Fatal")
};

UENUM(BlueprintType)
enum class EMingErrorCategory : uint8
{
    System UMETA(DisplayName = "System"),
    Gameplay UMETA(DisplayName = "Gameplay"),
    Network UMETA(DisplayName = "Network"),
    AI UMETA(DisplayName = "AI"),
    UI UMETA(DisplayName = "UI"),
    Audio UMETA(DisplayName = "Audio"),
    Physics UMETA(DisplayName = "Physics"),
    Memory UMETA(DisplayName = "Memory"),
    SaveLoad UMETA(DisplayName = "Save/Load"),
    Input UMETA(DisplayName = "Input")
};

USTRUCT(BlueprintType)
struct FMingErrorRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ErrorID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingErrorSeverity Severity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingErrorCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Message;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Context;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString StackTrace;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OccurrenceCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bResolved;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Resolution;

    FMingErrorRecord()
        : Severity(EMingErrorSeverity::Error)
        , Category(EMingErrorCategory::System)
        , Timestamp(0.0f)
        , OccurrenceCount(1)
        , bResolved(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingStabilityMetrics
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Uptime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalErrors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CriticalErrors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Warnings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CrashRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AverageFPS;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MemoryUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StabilityScore;

    FMingStabilityMetrics()
        : Uptime(0.0f)
        , TotalErrors(0)
        , CriticalErrors(0)
        , Warnings(0)
        , CrashRate(0.0f)
        , AverageFPS(60.0f)
        , MemoryUsage(0.0f)
        , StabilityScore(100.0f)
    {}
};

/**
 * Stability Manager for MingGoRTS
 * Monitors and manages game stability and error handling
 */
UCLASS(ClassGroup = (Stability), Blueprintable)
class MINGSTABILITY_API UMingStabilityManager : public UObject
{
    GENERATED_BODY()

public:
    UMingStabilityManager();

    // Stability Monitoring
    UFUNCTION(BlueprintCallable, Category = "Stability Monitoring")
    void InitializeStabilityManager();

    UFUNCTION(BlueprintCallable, Category = "Stability Monitoring")
    void StartMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Stability Monitoring")
    void StopMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Stability Monitoring")
    void UpdateStabilityMetrics();

    UFUNCTION(BlueprintPure, Category = "Stability Monitoring")
    FMingStabilityMetrics GetStabilityMetrics() const;

    UFUNCTION(BlueprintPure, Category = "Stability Monitoring")
    bool IsSystemStable() const;

    // Error Handling
    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    void ReportError(const FString& Message, EMingErrorSeverity Severity, EMingErrorCategory Category};

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    void LogWarning(const FString& Message, EMingErrorCategory Category};

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    void LogInfo(const FString& Message, EMingErrorCategory Category};

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    void HandleCrash(const FString& Context) {};

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    void RecoverFromError(const FString& ErrorID) {};

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    void MarkErrorResolved(const FString& ErrorID, const FString& Resolution) {};

    // Error Analysis
    UFUNCTION(BlueprintPure, Category = "Error Analysis")
    TArray<FMingErrorRecord> GetAllErrors() const;

    UFUNCTION(BlueprintPure, Category = "Error Analysis")
    TArray<FMingErrorRecord> GetErrorsBySeverity(EMingErrorSeverity Severity) const;

    UFUNCTION(BlueprintPure, Category = "Error Analysis")
    TArray<FMingErrorRecord> GetErrorsByCategory(EMingErrorCategory Category) const;

    UFUNCTION(BlueprintPure, Category = "Error Analysis")
    TArray<FMingErrorRecord> GetUnresolvedErrors() const;

    UFUNCTION(BlueprintCallable, Category = "Error Analysis")
    void AnalyzeErrorPatterns();

    UFUNCTION(BlueprintCallable, Category = "Error Analysis")
    void GenerateErrorReport();

    // Stability Improvements
    UFUNCTION(BlueprintCallable, Category = "Stability Improvements")
    void ApplyStabilityFixes();

    UFUNCTION(BlueprintCallable, Category = "Stability Improvements")
    void OptimizeMemoryUsage();

    UFUNCTION(BlueprintCallable, Category = "Stability Improvements")
    void ImproveErrorHandling();

    UFUNCTION(BlueprintCallable, Category = "Stability Improvements")
    void AddRedundancySystems();

    // Crash Prevention
    UFUNCTION(BlueprintCallable, Category = "Crash Prevention")
    void SetupCrashPrevention();

    UFUNCTION(BlueprintCallable, Category = "Crash Prevention")
    void ValidateSystemState();

    UFUNCTION(BlueprintCallable, Category = "Crash Prevention")
    void CheckForMemoryLeaks();

    UFUNCTION(BlueprintCallable, Category = "Crash Prevention")
    void MonitorPerformance();

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetSeverityName(EMingErrorSeverity Severity};

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetCategoryName(EMingErrorCategory Category};

    UFUNCTION(BlueprintCallable, Category = "Utility")
    FString ExportErrorLog() const;

    UFUNCTION(BlueprintCallable, Category = "Utility")
    void ClearErrorLog();

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCriticalError, const FMingErrorRecord&, Error};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSystemUnstable, float, StabilityScore};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSystemRecovered};

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCriticalError OnCriticalError;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSystemUnstable OnSystemUnstable;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSystemRecovered OnSystemRecovered;

protected:
    UPROPERTY()
    TArray<FMingErrorRecord> ErrorLog;

    UPROPERTY()
    FMingStabilityMetrics CurrentMetrics;

    UPROPERTY()
    bool bIsMonitoring;

    UPROPERTY()
    float StartTime;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal Functions
    void InitializeErrorHandling();
    void SetupPerformanceMonitoring();
    void SetupMemoryMonitoring();

    // Error Processing
    void ProcessNewError(const FMingErrorRecord& Error) {};
    void CategorizeError(FMingErrorRecord& Error};
    void DetermineSeverity(FMingErrorRecord& Error};
    void UpdateErrorCounters();

    // Stability Analysis
    void CalculateStabilityScore();
    void CheckStabilityThresholds();
    void PredictPotentialIssues();
    void GenerateStabilityReport();

    // Recovery Functions
    void AttemptAutomaticRecovery();
    void SaveErrorState();
    void NotifyDevelopers(const FMingErrorRecord& Error) {};
    void GracefulDegradation();

    // Monitoring Functions
    void MonitorFPS();
    void MonitorMemory();
    void MonitorNetwork();
    void MonitorAI();

    // Helpers
    FString GenerateErrorID() const;
    FMingErrorRecord* FindError(const FString& ErrorID) {};
    void LogStabilityEvent(const FString& Event, const FString& Details) {};
};

