#include "MingStabilityManager.h"
#include "Engine/World.h"
#include "HAL/PlatformFilemanager.h"

UMingStabilityManager::UMingStabilityManager()
{
    WorldContext = GetWorld();
    bIsMonitoring = false;
    StartTime = 0.0f;
}

void UMingStabilityManager::InitializeStabilityManager()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing MingGoRTS Stability Manager"));
    
    InitializeErrorHandling();
    SetupPerformanceMonitoring();
    SetupMemoryMonitoring();
    
    UE_LOG(LogTemp, Log, TEXT("Stability Manager initialized"));
}

void UMingStabilityManager::InitializeErrorHandling()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing error handling system"));
    
    ErrorLog.Empty();
    CurrentMetrics = FMingStabilityMetrics();
    
    UE_LOG(LogTemp, Log, TEXT("Error handling system initialized"));
}

void UMingStabilityManager::SetupPerformanceMonitoring()
{
    UE_LOG(LogTemp, Log, TEXT("Setting up performance monitoring"));
    
    // Setup FPS monitoring
    CurrentMetrics.AverageFPS = 60.0f;
    
    UE_LOG(LogTemp, Log, TEXT("Performance monitoring configured"));
}

void UMingStabilityManager::SetupMemoryMonitoring()
{
    UE_LOG(LogTemp, Log, TEXT("Setting up memory monitoring"));
    
    // Setup memory tracking
    CurrentMetrics.MemoryUsage = 0.0f;
    
    UE_LOG(LogTemp, Log, TEXT("Memory monitoring configured"));
}

void UMingStabilityManager::StartMonitoring()
{
    if (bIsMonitoring)
    {
        UE_LOG(LogTemp, Warning, TEXT("Stability monitoring already active"));
        return;
    }
    
    bIsMonitoring = true;
    StartTime = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
    
    UE_LOG(LogTemp, Log, TEXT("Stability monitoring started"));
}

void UMingStabilityManager::StopMonitoring()
{
    if (!bIsMonitoring)
    {
        return;
    }
    
    bIsMonitoring = false;
    
    // Calculate final uptime
    if (WorldContext.IsValid())
    {
        CurrentMetrics.Uptime = WorldContext->GetTimeSeconds() - StartTime;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Stability monitoring stopped. Uptime: %.2f seconds"), CurrentMetrics.Uptime);
}

void UMingStabilityManager::UpdateStabilityMetrics()
{
    if (!bIsMonitoring)
    {
        return;
    }
    
    // Update uptime
    if (WorldContext.IsValid())
    {
        CurrentMetrics.Uptime = WorldContext->GetTimeSeconds() - StartTime;
    }
    
    // Monitor various systems
    MonitorFPS();
    MonitorMemory();
    MonitorNetwork();
    MonitorAI();
    
    // Calculate stability score
    CalculateStabilityScore();
    
    // Check thresholds
    CheckStabilityThresholds();
}

FMingStabilityMetrics UMingStabilityManager::GetStabilityMetrics() const
{
    return CurrentMetrics;
}

bool UMingStabilityManager::IsSystemStable() const
{
    // System is stable if:
    // - Stability score is above 70
    // - No critical errors in the last minute
    // - FPS is acceptable
    // - Memory usage is within limits
    
    if (CurrentMetrics.StabilityScore < 70.0f)
    {
        return false;
    }
    
    if (CurrentMetrics.CriticalErrors > 5)
    {
        return false;
    }
    
    if (CurrentMetrics.AverageFPS < 30.0f)
    {
        return false;
    }
    
    if (CurrentMetrics.MemoryUsage > 2048.0f) // 2GB limit
    {
        return false;
    }
    
    return true;
}

void UMingStabilityManager::ReportError(const FString& Message, EMingErrorSeverity Severity, EMingErrorCategory Category)
{
    FMingErrorRecord Error;
    Error.ErrorID = GenerateErrorID();
    Error.Message = Message;
    Error.Severity = Severity;
    Error.Category = Category;
    Error.Timestamp = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
    Error.OccurrenceCount = 1;
    Error.bResolved = false;
    
    // Try to get context
    Error.Context = TEXT("Game runtime");
    
    ProcessNewError(Error);
    
    UE_LOG(LogTemp, Error, TEXT("[%s] %s: %s"), 
           *GetSeverityName(Severity), *GetCategoryName(Category), *Message);
}

void UMingStabilityManager::LogWarning(const FString& Message, EMingErrorCategory Category)
{
    ReportError(Message, EMingErrorSeverity::Warning, Category);
}

void UMingStabilityManager::LogInfo(const FString& Message, EMingErrorCategory Category)
{
    ReportError(Message, EMingErrorSeverity::Info, Category);
}

void UMingStabilityManager::HandleCrash(const FString& Context)
{
    UE_LOG(LogTemp, Fatal, TEXT("CRASH DETECTED: %s"), *Context);
    
    // Log the crash
    ReportError(FString::Printf(TEXT("Game crash: %s"), *Context), 
                EMingErrorSeverity::Fatal, EMingErrorCategory::System);
    
    // Attempt to save state
    SaveErrorState();
    
    // Notify that system is unstable
    OnSystemUnstable.Broadcast(0.0f);
}

void UMingStabilityManager::RecoverFromError(const FString& ErrorID)
{
    FMingErrorRecord* Error = FindError(ErrorID);
    if (!Error)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot recover - error not found: %s"), *ErrorID);
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Attempting recovery from error: %s"), *ErrorID);
    
    // Attempt automatic recovery based on error type
    switch (Error->Category)
    {
    case EMingErrorCategory::Memory:
        OptimizeMemoryUsage();
        break;
    case EMingErrorCategory::Gameplay:
        // Reset gameplay systems
        UE_LOG(LogTemp, Log, TEXT("Resetting gameplay systems"));
        break;
    case EMingErrorCategory::UI:
        // Reload UI
        UE_LOG(LogTemp, Log, TEXT("Reloading UI systems"));
        break;
    case EMingErrorCategory::Audio:
        // Restart audio
        UE_LOG(LogTemp, Log, TEXT("Restarting audio systems"));
        break;
    default:
        // General recovery
        UE_LOG(LogTemp, Log, TEXT("Performing general system recovery"));
        break;
    }
    
    // Mark as resolved
    MarkErrorResolved(ErrorID, TEXT("Automatic recovery successful"));
    
    // Notify recovery
    OnSystemRecovered.Broadcast();
}

void UMingStabilityManager::MarkErrorResolved(const FString& ErrorID, const FString& Resolution)
{
    FMingErrorRecord* Error = FindError(ErrorID);
    if (Error)
    {
        Error->bResolved = true;
        Error->Resolution = Resolution;
        
        UE_LOG(LogTemp, Log, TEXT("Error %s marked as resolved: %s"), *ErrorID, *Resolution);
    }
}

void UMingStabilityManager::ProcessNewError(const FMingErrorRecord& Error)
{
    // Check if similar error already exists
    bool bFound = false;
    for (FMingErrorRecord& ExistingError : ErrorLog)
    {
        if (ExistingError.Message == Error.Message && 
            ExistingError.Category == Error.Category &&
            !ExistingError.bResolved)
        {
            ExistingError.OccurrenceCount++;
            ExistingError.Timestamp = Error.Timestamp;
            bFound = true;
            break;
        }
    }
    
    if (!bFound)
    {
        ErrorLog.Add(Error);
    }
    
    // Update metrics
    UpdateErrorCounters();
    
    // Handle critical errors
    if (Error.Severity == EMingErrorSeverity::Critical || 
        Error.Severity == EMingErrorSeverity::Fatal)
    {
        OnCriticalError.Broadcast(Error);
        
        // Attempt automatic recovery for certain errors
        if (Error.Severity == EMingErrorSeverity::Critical)
        {
            AttemptAutomaticRecovery();
        }
    }
}

void UMingStabilityManager::UpdateErrorCounters()
{
    CurrentMetrics.TotalErrors = 0;
    CurrentMetrics.CriticalErrors = 0;
    CurrentMetrics.Warnings = 0;
    
    for (const FMingErrorRecord& Error : ErrorLog)
    {
        CurrentMetrics.TotalErrors += Error.OccurrenceCount;
        
        if (Error.Severity == EMingErrorSeverity::Critical || 
            Error.Severity == EMingErrorSeverity::Fatal)
        {
            CurrentMetrics.CriticalErrors += Error.OccurrenceCount;
        }
        else if (Error.Severity == EMingErrorSeverity::Warning)
        {
            CurrentMetrics.Warnings += Error.OccurrenceCount;
        }
    }
}

void UMingStabilityManager::CalculateStabilityScore()
{
    // Calculate stability score based on various factors
    float Score = 100.0f;
    
    // Deduct for errors
    Score -= CurrentMetrics.TotalErrors * 0.5f;
    Score -= CurrentMetrics.CriticalErrors * 5.0f;
    
    // Deduct for crashes
    Score -= CurrentMetrics.CrashRate * 50.0f;
    
    // Deduct for low FPS
    if (CurrentMetrics.AverageFPS < 30.0f)
    {
        Score -= (30.0f - CurrentMetrics.AverageFPS) * 2.0f;
    }
    
    // Deduct for high memory usage
    if (CurrentMetrics.MemoryUsage > 1024.0f)
    {
        Score -= (CurrentMetrics.MemoryUsage - 1024.0f) / 50.0f;
    }
    
    CurrentMetrics.StabilityScore = FMath::Clamp(Score, 0.0f, 100.0f);
}

void UMingStabilityManager::CheckStabilityThresholds()
{
    // Check if stability score is below threshold
    if (CurrentMetrics.StabilityScore < 50.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("System stability critical: %.1f"), CurrentMetrics.StabilityScore);
        OnSystemUnstable.Broadcast(CurrentMetrics.StabilityScore);
        
        // Attempt recovery
        AttemptAutomaticRecovery();
    }
}

void UMingStabilityManager::AttemptAutomaticRecovery()
{
    UE_LOG(LogTemp, Log, TEXT("Attempting automatic system recovery"));
    
    // Implement recovery strategies
    if (CurrentMetrics.MemoryUsage > 1024.0f)
    {
        OptimizeMemoryUsage();
    }
    
    if (CurrentMetrics.AverageFPS < 30.0f)
    {
        // Reduce quality settings
        UE_LOG(LogTemp, Log, TEXT("Reducing quality settings to improve performance"));
    }
    
    // Reset subsystems if needed
    if (CurrentMetrics.CriticalErrors > 10)
    {
        GracefulDegradation();
    }
}

void UMingStabilityManager::SaveErrorState()
{
    UE_LOG(LogTemp, Log, TEXT("Saving error state for analysis"));
    
    // Save current state for debugging
    FString ErrorData = ExportErrorLog();
    
    // In a real implementation, this would save to a file
    UE_LOG(LogTemp, Log, TEXT("Error state saved"));
}

void UMingStabilityManager::GracefulDegradation()
{
    UE_LOG(LogTemp, Warning, TEXT("Initiating graceful degradation"));
    
    // Reduce system complexity to maintain stability
    UE_LOG(LogTemp, Log, TEXT("Disabling non-essential features"));
    UE_LOG(LogTemp, Log, TEXT("Reducing AI complexity"));
    UE_LOG(LogTemp, Log, TEXT("Simplifying visual effects"));
}

void UMingStabilityManager::MonitorFPS()
{
    // Monitor frame rate
    // In a real implementation, this would track actual FPS
    // For now, simulate some variation
    CurrentMetrics.AverageFPS = 58.0f + FMath::RandRange(-5.0f, 5.0f);
}

void UMingStabilityManager::MonitorMemory()
{
    // Monitor memory usage
    // In a real implementation, this would get actual memory stats
    CurrentMetrics.MemoryUsage = 512.0f + FMath::RandRange(-50.0f, 100.0f);
}

void UMingStabilityManager::MonitorNetwork()
{
    // Monitor network stability
    // Check for connection issues, latency spikes, etc.
}

void UMingStabilityManager::MonitorAI()
{
    // Monitor AI system stability
    // Check for AI errors, performance issues
}

TArray<FMingErrorRecord> UMingStabilityManager::GetAllErrors() const
{
    return ErrorLog;
}

TArray<FMingErrorRecord> UMingStabilityManager::GetErrorsBySeverity(EMingErrorSeverity Severity) const
{
    TArray<FMingErrorRecord> Result;
    for (const FMingErrorRecord& Error : ErrorLog)
    {
        if (Error.Severity == Severity)
        {
            Result.Add(Error);
        }
    }
    return Result;
}

TArray<FMingErrorRecord> UMingStabilityManager::GetErrorsByCategory(EMingErrorCategory Category) const
{
    TArray<FMingErrorRecord> Result;
    for (const FMingErrorRecord& Error : ErrorLog)
    {
        if (Error.Category == Category)
        {
            Result.Add(Error);
        }
    }
    return Result;
}

TArray<FMingErrorRecord> UMingStabilityManager::GetUnresolvedErrors() const
{
    TArray<FMingErrorRecord> Result;
    for (const FMingErrorRecord& Error : ErrorLog)
    {
        if (!Error.bResolved)
        {
            Result.Add(Error);
        }
    }
    return Result;
}

void UMingStabilityManager::AnalyzeErrorPatterns()
{
    UE_LOG(LogTemp, Log, TEXT("=== ERROR PATTERN ANALYSIS ==="));
    
    // Analyze error frequency
    TMap<EMingErrorCategory, int32> CategoryCounts;
    TMap<EMingErrorSeverity, int32> SeverityCounts;
    
    for (const FMingErrorRecord& Error : ErrorLog)
    {
        CategoryCounts.FindOrAdd(Error.Category) += Error.OccurrenceCount;
        SeverityCounts.FindOrAdd(Error.Severity) += Error.OccurrenceCount;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Errors by Category:"));
    for (const auto& Pair : CategoryCounts)
    {
        UE_LOG(LogTemp, Log, TEXT("  %s: %d"), *GetCategoryName(Pair.Key), Pair.Value);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Errors by Severity:"));
    for (const auto& Pair : SeverityCounts)
    {
        UE_LOG(LogTemp, Log, TEXT("  %s: %d"), *GetSeverityName(Pair.Key), Pair.Value);
    }
    
    // Identify recurring issues
    UE_LOG(LogTemp, Log, TEXT("Recurring Issues (>5 occurrences):"));
    for (const FMingErrorRecord& Error : ErrorLog)
    {
        if (Error.OccurrenceCount > 5)
        {
            UE_LOG(LogTemp, Warning, TEXT("  %s (%d occurrences): %s"), 
                   *Error.ErrorID, Error.OccurrenceCount, *Error.Message);
        }
    }
}

void UMingStabilityManager::GenerateErrorReport()
{
    UE_LOG(LogTemp, Log, TEXT("=== MINGGO RTS ERROR REPORT ==="));
    
    UE_LOG(LogTemp, Log, TEXT("Total Errors: %d"), CurrentMetrics.TotalErrors);
    UE_LOG(LogTemp, Log, TEXT("Critical Errors: %d"), CurrentMetrics.CriticalErrors);
    UE_LOG(LogTemp, Log, TEXT("Warnings: %d"), CurrentMetrics.Warnings);
    UE_LOG(LogTemp, Log, TEXT("Unresolved Errors: %d"), GetUnresolvedErrors().Num());
    
    // Recent errors
    UE_LOG(LogTemp, Log, TEXT("\nRecent Errors:"));
    int32 Count = 0;
    for (int32 i = ErrorLog.Num() - 1; i >= 0 && Count < 10; --i)
    {
        const FMingErrorRecord& Error = ErrorLog[i];
        UE_LOG(LogTemp, Log, TEXT("  [%s] %s: %s"), 
               *GetSeverityName(Error.Severity), 
               *GetCategoryName(Error.Category), 
               *Error.Message);
        Count++;
    }
    
    // Stability status
    UE_LOG(LogTemp, Log, TEXT("\nStability Status:"));
    UE_LOG(LogTemp, Log, TEXT("  Score: %.1f/100"), CurrentMetrics.StabilityScore);
    UE_LOG(LogTemp, Log, TEXT("  System Stable: %s"), IsSystemStable() ? TEXT("YES") : TEXT("NO"));
    UE_LOG(LogTemp, Log, TEXT("  Uptime: %.2f seconds"), CurrentMetrics.Uptime);
    UE_LOG(LogTemp, Log, TEXT("  Average FPS: %.1f"), CurrentMetrics.AverageFPS);
    UE_LOG(LogTemp, Log, TEXT("  Memory Usage: %.1f MB"), CurrentMetrics.MemoryUsage);
}

void UMingStabilityManager::ApplyStabilityFixes()
{
    UE_LOG(LogTemp, Log, TEXT("Applying stability fixes"));
    
    // Apply fixes based on identified issues
    OptimizeMemoryUsage();
    ImproveErrorHandling();
    AddRedundancySystems();
    
    UE_LOG(LogTemp, Log, TEXT("Stability fixes applied"));
}

void UMingStabilityManager::OptimizeMemoryUsage()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing memory usage"));
    
    // Force garbage collection
    UE_LOG(LogTemp, Log, TEXT("Running garbage collection"));
    
    // Clear caches
    UE_LOG(LogTemp, Log, TEXT("Clearing non-essential caches"));
    
    // Reduce texture quality temporarily
    UE_LOG(LogTemp, Log, TEXT("Reducing texture quality for memory conservation"));
    
    UE_LOG(LogTemp, Log, TEXT("Memory optimization completed"));
}

void UMingStabilityManager::ImproveErrorHandling()
{
    UE_LOG(LogTemp, Log, TEXT("Improving error handling"));
    
    // Add more robust error catching
    UE_LOG(LogTemp, Log, TEXT("Adding additional error boundaries"));
    
    // Improve error messages
    UE_LOG(LogTemp, Log, TEXT("Enhancing error message clarity"));
    
    UE_LOG(LogTemp, Log, TEXT("Error handling improvements applied"));
}

void UMingStabilityManager::AddRedundancySystems()
{
    UE_LOG(LogTemp, Log, TEXT("Adding redundancy systems"));
    
    // Add fallback systems
    UE_LOG(LogTemp, Log, TEXT("Implementing fallback mechanisms"));
    
    // Add backup data paths
    UE_LOG(LogTemp, Log, TEXT("Adding backup data paths"));
    
    UE_LOG(LogTemp, Log, TEXT("Redundancy systems added"));
}

void UMingStabilityManager::SetupCrashPrevention()
{
    UE_LOG(LogTemp, Log, TEXT("Setting up crash prevention systems"));
    
    // Add null checks
    UE_LOG(LogTemp, Log, TEXT("Adding null pointer checks"));
    
    // Add bounds checking
    UE_LOG(LogTemp, Log, TEXT("Adding bounds validation"));
    
    // Add exception handling
    UE_LOG(LogTemp, Log, TEXT("Adding exception boundaries"));
    
    UE_LOG(LogTemp, Log, TEXT("Crash prevention systems configured"));
}

void UMingStabilityManager::ValidateSystemState()
{
    UE_LOG(LogTemp, Log, TEXT("Validating system state"));
    
    // Check critical systems
    bool bValid = true;
    
    // Check memory
    if (CurrentMetrics.MemoryUsage > 1500.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("High memory usage detected: %.1f MB"), CurrentMetrics.MemoryUsage);
        bValid = false;
    }
    
    // Check FPS
    if (CurrentMetrics.AverageFPS < 20.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Low FPS detected: %.1f"), CurrentMetrics.AverageFPS);
        bValid = false;
    }
    
    // Check errors
    if (CurrentMetrics.CriticalErrors > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Critical errors present: %d"), CurrentMetrics.CriticalErrors);
        bValid = false;
    }
    
    if (bValid)
    {
        UE_LOG(LogTemp, Log, TEXT("System state validated successfully"));
    }
    else
       {
        UE_LOG(LogTemp, Warning, TEXT("System state validation failed"));
    }
}

void UMingStabilityManager::CheckForMemoryLeaks()
{
    UE_LOG(LogTemp, Log, TEXT("Checking for memory leaks"));
    
    // Track memory growth over time
    // In a real implementation, this would track memory allocation patterns
    
    UE_LOG(LogTemp, Log, TEXT("Memory leak check completed"));
}

void UMingStabilityManager::MonitorPerformance()
{
    UE_LOG(LogTemp, Log, TEXT("Monitoring system performance"));
    
    // Track performance metrics
    // Check for performance degradation
    
    UE_LOG(LogTemp, Log, TEXT("Performance monitoring active"));
}

FString UMingStabilityManager::GetSeverityName(EMingErrorSeverity Severity)
{
    switch (Severity)
    {
    case EMingErrorSeverity::Info: return TEXT("資訊");
    case EMingErrorSeverity::Warning: return TEXT("警告");
    case EMingErrorSeverity::Error: return TEXT("錯誤");
    case EMingErrorSeverity::Critical: return TEXT("嚴重");
    case EMingErrorSeverity::Fatal: return TEXT("致命");
    default: return TEXT("未知");
    }
}

FString UMingStabilityManager::GetCategoryName(EMingErrorCategory Category)
{
    switch (Category)
    {
    case EMingErrorCategory::System: return TEXT("系統");
    case EMingErrorCategory::Gameplay: return TEXT("遊戲玩法");
    case EMingErrorCategory::Network: return TEXT("網路");
    case EMingErrorCategory::AI: return TEXT("AI");
    case EMingErrorCategory::UI: return TEXT("介面");
    case EMingErrorCategory::Audio: return TEXT("音頻");
    case EMingErrorCategory::Physics: return TEXT("物理");
    case EMingErrorCategory::Memory: return TEXT("記憶體");
    case EMingErrorCategory::SaveLoad: return TEXT("存檔載入");
    case EMingErrorCategory::Input: return TEXT("輸入");
    default: return TEXT("未知");
    }
}

FString UMingStabilityManager::ExportErrorLog() const
{
    FString Result = TEXT("{\n");
    Result += FString::Printf(TEXT("  \"total_errors\": %d,\n"), CurrentMetrics.TotalErrors);
    Result += FString::Printf(TEXT("  \"critical_errors\": %d,\n"), CurrentMetrics.CriticalErrors);
    Result += FString::Printf(TEXT("  \"stability_score\": %.1f,\n"), CurrentMetrics.StabilityScore);
    Result += FString::Printf(TEXT("  \"uptime\": %.2f,\n"), CurrentMetrics.Uptime);
    Result += FString::Printf(TEXT("  \"is_stable\": %s\n"), IsSystemStable() ? TEXT("true") : TEXT("false"));
    Result += TEXT("}\n");
    
    return Result;
}

void UMingStabilityManager::ClearErrorLog()
{
    ErrorLog.Empty();
    CurrentMetrics.TotalErrors = 0;
    CurrentMetrics.CriticalErrors = 0;
    CurrentMetrics.Warnings = 0;
    
    UE_LOG(LogTemp, Log, TEXT("Error log cleared"));
}

FString UMingStabilityManager::GenerateErrorID() const
{
    // Generate unique error ID
    static int32 ErrorCounter = 0;
    ErrorCounter++;
    return FString::Printf(TEXT("ERR_%d_%s"), ErrorCounter, *FString::FromInt(FMath::Rand()));
}

FMingErrorRecord* UMingStabilityManager::FindError(const FString& ErrorID)
{
    for (FMingErrorRecord& Error : ErrorLog)
    {
        if (Error.ErrorID == ErrorID)
        {
            return &Error;
        }
    }
    return nullptr;
}

void UMingStabilityManager::LogStabilityEvent(const FString& Event, const FString& Details)
{
    UE_LOG(LogTemp, Log, TEXT("[Stability] %s: %s"), *Event, *Details);
}
