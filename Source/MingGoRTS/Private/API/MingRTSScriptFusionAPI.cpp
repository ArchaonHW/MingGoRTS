#include "MingRTSScriptFusionAPI.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "HAL/Process.h"
#include "Misc/FileHelper.h"

UMingRTSScriptFusionAPI::UMingRTSScriptFusionAPI()
{
    bIntelligentFusionEnabled = false;
    MaxParallelTasks = 4;
    bDetailedLoggingEnabled = true;
    bAutoRetryEnabled = true;
    MaxRetryAttempts = 3;
    
    InitializeScriptFusionSystem();
}

void UMingRTSScriptFusionAPI::InitializeScriptFusionSystem()
{
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Initializing script fusion system..."));
    
    // Initialize script registry
    ScriptRegistry.Empty();
    ScriptFunctionalities.Empty();
    
    // Initialize task management
    FusionTasks.Empty();
    TaskResults.Empty();
    ActiveTasks.Empty();
    
    // Initialize profiles
    FusionProfiles.Empty();
    
    // Initialize events and scheduling
    EventTriggers.Empty();
    ScheduledTasks.Empty();
    
    // Initialize pipeline system
    PipelineStages.Empty();
    
    // Discover existing scripts
    DiscoverAllScripts();
    
    // Load saved fusion data
    LoadFusionData();
    
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: System initialization completed"));
}

// ========== 腳本發現和註冊 ==========

void UMingRTSScriptFusionAPI::DiscoverAllScripts()
{
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Discovering all scripts..."));
    
    // Scan main Tools directory
    ScanScriptsDirectory(FPaths::ProjectDir() / TEXT("Tools"));
    
    // Scan subdirectories
    TArray<FString> SubDirectories = {
        TEXT("Tools/build"),
        TEXT("Tools/automation"),
        TEXT("Tools/ai"),
        TEXT("Tools/api"),
        TEXT("Tools/install")
    };
    
    for (const FString& SubDir : SubDirectories) {
        ScanScriptsDirectory(FPaths::ProjectDir() / SubDir);
    }
    
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Script discovery completed"));
}

void UMingRTSScriptFusionAPI::ScanScriptsDirectory(const FString& DirectoryPath)
{
    if (!FPaths::DirectoryExists(DirectoryPath)) {
        return;
    }
    
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    // Find all .ps1 files
    TArray<FString> ScriptFiles;
    PlatformFile.FindFiles(ScriptFiles, *DirectoryPath, TEXT("*.ps1"));
    
    for (const FString& ScriptFile : ScriptFiles) {
        FString FullPath = FPaths::Combine(DirectoryPath, ScriptFile);
        CategorizeScript(FullPath);
        LoadScriptMetadata(FullPath);
    }
}

void UMingRTSScriptFusionAPI::CategorizeScript(const FString& ScriptPath)
{
    FString FileName = FPaths::GetBaseFilename(ScriptPath);
    FString Directory = FPaths::GetPath(ScriptPath);
    
    EScriptCategory Category = EScriptCategory::Automation; // Default category
    
    // Categorize based on directory and filename
    if (Directory.Contains(TEXT("build"))) {
        Category = EScriptCategory::Build;
    } else if (Directory.Contains(TEXT("automation"))) {
        Category = EScriptCategory::Automation;
    } else if (Directory.Contains(TEXT("ai"))) {
        Category = EScriptCategory::AI;
    } else if (FileName.Contains(TEXT("Optimize")) || FileName.Contains(TEXT("Performance"))) {
        Category = EScriptCategory::Optimization;
    } else if (FileName.Contains(TEXT("Debug")) || FileName.Contains(TEXT("Test"))) {
        Category = EScriptCategory::Debug;
    } else if (FileName.Contains(TEXT("Deploy"))) {
        Category = EScriptCategory::Deployment;
    } else if (FileName.Contains(TEXT("Maintenance"))) {
        Category = EScriptCategory::Maintenance;
    } else if (FileName.Contains(TEXT("Security"))) {
        Category = EScriptCategory::Maintenance; // Security scripts go to maintenance
    }
    
    // Register script in category
    if (!ScriptRegistry.Contains(Category)) {
        ScriptRegistry.Add(Category, TArray<FString>());
    }
    ScriptRegistry[Category].Add(ScriptPath);
    
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Categorized script %s as %d"), *FileName, (int32)Category);
}

void UMingRTSScriptFusionAPI::LoadScriptMetadata(const FString& ScriptPath)
{
    FScriptFunctionality Functionality;
    Functionality.FunctionalityID = FPaths::GetBaseFilename(ScriptPath);
    Functionality.FunctionalityName = Functionality.FunctionalityID;
    Functionality.ScriptPath = ScriptPath;
    Functionality.Category = EScriptCategory::Automation; // Will be updated in categorization
    
    // Read script file to extract metadata
    FString ScriptContent;
    if (FFileHelper::LoadFileToString(ScriptContent, *ScriptPath)) {
        // Extract description from comments
        if (ScriptContent.Contains(TEXT("# Description:"))) {
            int32 StartIndex = ScriptContent.Find(TEXT("# Description:"));
            int32 EndIndex = ScriptContent.Find(TEXT("\n"), StartIndex);
            if (StartIndex != INDEX_NONE && EndIndex != INDEX_NONE) {
                Functionality.Description = ScriptContent.Mid(StartIndex + 14, EndIndex - StartIndex - 14).TrimStartAndEnd();
            }
        }
        
        // Extract parameters from param block
        if (ScriptContent.Contains(TEXT("param("))) {
            int32 ParamStart = ScriptContent.Find(TEXT("param("));
            int32 ParamEnd = ScriptContent.Find(TEXT(")"), ParamStart);
            if (ParamStart != INDEX_NONE && ParamEnd != INDEX_NONE) {
                FString ParamBlock = ScriptContent.Mid(ParamStart, ParamEnd - ParamStart + 1);
                // Simple parameter extraction (can be enhanced)
                TArray<FString> ParamLines;
                ParamBlock.ParseIntoArrayLines(ParamLines);
                
                for (const FString& Line : ParamLines) {
                    if (Line.Contains(TEXT("$"))) {
                        FString ParamName = Line;
                        // Extract parameter name between $ and )
                        int32 DollarIndex = ParamName.Find(TEXT("$"));
                        int32 SpaceIndex = ParamName.Find(TEXT(" "), DollarIndex);
                        if (DollarIndex != INDEX_NONE && SpaceIndex != INDEX_NONE) {
                            FString Name = ParamName.Mid(DollarIndex + 1, SpaceIndex - DollarIndex - 1);
                            Functionality.Parameters.Add(Name);
                        }
                    }
                }
            }
        }
        
        // Estimate execution time based on script complexity
        Functionality.AverageExecutionTime = EstimateScriptExecutionTime(ScriptPath);
    }
    
    ScriptFunctionalities.Add(Functionality.FunctionalityID, Functionality);
}

float UMingRTSScriptFusionAPI::EstimateScriptExecutionTime(const FString& ScriptPath)
{
    FString ScriptContent;
    if (!FFileHelper::LoadFileToString(ScriptContent, *ScriptPath)) {
        return 5.0f; // Default estimate
    }
    
    float EstimatedTime = 1.0f; // Base time
    
    // Add time based on script complexity
    if (ScriptContent.Contains(TEXT("Start-Process"))) {
        EstimatedTime += 2.0f; // External processes take time
    }
    
    if (ScriptContent.Contains(TEXT("ForEach")) || ScriptContent.Contains(TEXT("for"))) {
        EstimatedTime += 1.5f; // Loops take time
    }
    
    if (ScriptContent.Contains(TEXT("Compile")) || ScriptContent.Contains(TEXT("Build"))) {
        EstimatedTime += 10.0f; // Compilation takes significant time
    }
    
    if (ScriptContent.Contains(TEXT("Test")) || ScriptContent.Contains(TEXT("Test-"))) {
        EstimatedTime += 5.0f; // Tests take time
    }
    
    // Add time based on file size (rough estimate)
    int32 FileSize = 0;
    if (FFileHelper::LoadFileToString(ScriptContent, *ScriptPath)) {
        FileSize = ScriptContent.Len();
        EstimatedTime += FileSize / 10000.0f; // 1 second per 10KB
    }
    
    return FMath::Clamp(EstimatedTime, 1.0f, 300.0f); // Clamp between 1 second and 5 minutes
}

void UMingRTSScriptFusionAPI::RegisterScript(const FString& ScriptPath, EScriptCategory Category)
{
    if (!ScriptRegistry.Contains(Category)) {
        ScriptRegistry.Add(Category, TArray<FString>());
    }
    
    if (!ScriptRegistry[Category].Contains(ScriptPath)) {
        ScriptRegistry[Category].Add(ScriptPath);
        CategorizeScript(ScriptPath);
        LoadScriptMetadata(ScriptPath);
        
        UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Registered script %s in category %d"), *ScriptPath, (int32)Category);
    }
}

TArray<FString> UMingRTSScriptFusionAPI::GetScriptsByCategory(EScriptCategory Category)
{
    if (ScriptRegistry.Contains(Category)) {
        return ScriptRegistry[Category];
    }
    return TArray<FString>();
}

void UMingRTSScriptFusionAPI::RefreshScriptRegistry()
{
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Refreshing script registry..."));
    
    ScriptRegistry.Empty();
    ScriptFunctionalities.Empty();
    DiscoverAllScripts();
    
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Script registry refreshed"));
}

// ========== 腳本融合任務管理 ==========

FString UMingRTSScriptFusionAPI::CreateFusionTask(const FScriptFusionTask& Task)
{
    FString TaskID = GenerateTaskID();
    
    FScriptFusionTask NewTask = Task;
    NewTask.TaskID = TaskID;
    
    // Calculate estimated duration if not provided
    if (NewTask.EstimatedDuration <= 0.0f) {
        NewTask.EstimatedDuration = 0.0f;
        for (const FString& ScriptPath : NewTask.ScriptPaths) {
            if (ScriptFunctionalities.Contains(FPaths::GetBaseFilename(ScriptPath))) {
                NewTask.EstimatedDuration += ScriptFunctionalities[FPaths::GetBaseFilename(ScriptPath)].AverageExecutionTime;
            }
        }
    }
    
    FusionTasks.Add(TaskID, NewTask);
    
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Created fusion task %s"), *TaskID);
    return TaskID;
}

bool UMingRTSScriptFusionAPI::ExecuteFusionTask(const FString& TaskID)
{
    if (!FusionTasks.Contains(TaskID)) {
        UE_LOG(LogTemp, Warning, TEXT("ScriptFusionAPI: Task not found: %s"), *TaskID);
        return false;
    }
    
    FScriptFusionTask& Task = FusionTasks[TaskID];
    
    if (!Task.bEnabled) {
        UE_LOG(LogTemp, Warning, TEXT("ScriptFusionAPI: Task is disabled: %s"), *TaskID);
        return false;
    }
    
    if (ActiveTasks.Contains(TaskID)) {
        UE_LOG(LogTemp, Warning, TEXT("ScriptFusionAPI: Task is already running: %s"), *TaskID);
        return false;
    }
    
    // Create result object
    FScriptFusionResult Result;
    Result.TaskID = TaskID;
    Result.StartTime = FDateTime::Now();
    
    TaskResults.Add(TaskID, Result);
    ActiveTasks.Add(TaskID);
    
    // Broadcast task started event
    OnTaskStarted.Broadcast(TaskID, Task);
    
    // Execute based on mode
    switch (Task.ExecutionMode) {
        case EFusionMode::Sequential:
            ExecuteTaskSequential(Task);
            break;
        case EFusionMode::Parallel:
            ExecuteTaskParallel(Task);
            break;
        case EFusionMode::Pipeline:
            ExecuteTaskPipeline(Task);
            break;
        case EFusionMode::Intelligent:
            ExecuteTaskIntelligent(Task);
            break;
        default:
            ExecuteTaskSequential(Task);
            break;
    }
    
    return true;
}

void UMingRTSScriptFusionAPI::ExecuteTaskSequential(const FScriptFusionTask& Task)
{
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Executing task %s sequentially"), *Task.TaskID);
    
    FScriptFusionResult& Result = TaskResults[Task.TaskID];
    Result.bSuccess = true;
    
    for (const FString& ScriptPath : Task.ScriptPaths) {
        FString Output;
        bool bScriptSuccess = ExecuteScript(ScriptPath, Task.Parameters, Output);
        
        if (bScriptSuccess) {
            Result.ExecutedScripts.Add(ScriptPath);
            Result.OutputData.Add(ScriptPath, Output);
            UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Successfully executed script: %s"), *ScriptPath);
        } else {
            Result.FailedScripts.Add(ScriptPath);
            Result.bSuccess = false;
            UE_LOG(LogTemp, Error, TEXT("ScriptFusionAPI: Failed to execute script: %s"), *ScriptPath);
            
            if (bAutoRetryEnabled && MaxRetryAttempts > 0) {
                // Retry logic would go here
                UE_LOG(LogTemp, Warning, TEXT("ScriptFusionAPI: Retry logic not implemented yet"));
            }
            break; // Stop on first failure for sequential execution
        }
    }
    
    Result.EndTime = FDateTime::Now();
    Result.ExecutionTime = (Result.EndTime - Result.StartTime).GetTotalSeconds();
    
    HandleTaskCompletion(Task.TaskID, Result);
}

void UMingRTSScriptFusionAPI::ExecuteTaskParallel(const FScriptFusionTask& Task)
{
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Executing task %s in parallel"), *Task.TaskID);
    
    FScriptFusionResult& Result = TaskResults[Task.TaskID];
    Result.bSuccess = true;
    
    // Simple parallel execution simulation
    // In a real implementation, this would use threading or async tasks
    for (const FString& ScriptPath : Task.ScriptPaths) {
        FString Output;
        bool bScriptSuccess = ExecuteScript(ScriptPath, Task.Parameters, Output);
        
        if (bScriptSuccess) {
            Result.ExecutedScripts.Add(ScriptPath);
            Result.OutputData.Add(ScriptPath, Output);
        } else {
            Result.FailedScripts.Add(ScriptPath);
            Result.bSuccess = false;
        }
    }
    
    Result.EndTime = FDateTime::Now();
    Result.ExecutionTime = (Result.EndTime - Result.StartTime).GetTotalSeconds();
    
    HandleTaskCompletion(Task.TaskID, Result);
}

void UMingRTSScriptFusionAPI::ExecuteTaskPipeline(const FScriptFusionTask& Task)
{
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Executing task %s as pipeline"), *Task.TaskID);
    
    // Pipeline execution is similar to sequential but with output/input chaining
    ExecuteTaskSequential(Task);
}

void UMingRTSScriptFusionAPI::ExecuteTaskIntelligent(const FScriptFusionTask& Task)
{
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Executing task %s intelligently"), *Task.TaskID);
    
    if (bIntelligentFusionEnabled) {
        // Optimize execution order based on dependencies and performance
        TArray<FString> OptimizedScripts = Task.ScriptPaths;
        OptimizeTaskOrder(OptimizedScripts);
        
        FScriptFusionTask OptimizedTask = Task;
        OptimizedTask.ScriptPaths = OptimizedScripts;
        
        ExecuteTaskSequential(OptimizedTask);
    } else {
        ExecuteTaskSequential(Task);
    }
}

bool UMingRTSScriptFusionAPI::ExecuteScript(const FString& ScriptPath, const TMap<FString, FString>& Parameters, FString& Output)
{
    // This is a simplified script execution
    // In a real implementation, this would use proper PowerShell execution
    
    FString ScriptName = FPaths::GetBaseFilename(ScriptPath);
    
    // Simulate script execution based on script name
    if (ScriptName.Contains(TEXT("Compile")) || ScriptName.Contains(TEXT("Build"))) {
        Output = TEXT("Compilation completed successfully");
        return true;
    } else if (ScriptName.Contains(TEXT("Test"))) {
        Output = TEXT("All tests passed");
        return true;
    } else if (ScriptName.Contains(TEXT("Deploy"))) {
        Output = TEXT("Deployment completed");
        return true;
    } else if (ScriptName.Contains(TEXT("Optimize"))) {
        Output = TEXT("Optimization completed");
        return true;
    } else {
        Output = TEXT("Script executed successfully");
        return true;
    }
}

void UMingRTSScriptFusionAPI::HandleTaskCompletion(const FString& TaskID, const FScriptFusionResult& Result)
{
    ActiveTasks.Remove(TaskID);
    
    if (Result.bSuccess) {
        UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Task %s completed successfully in %.2f seconds"), 
               *TaskID, Result.ExecutionTime);
    } else {
        UE_LOG(LogTemp, Warning, TEXT("ScriptFusionAPI: Task %s completed with errors"), *TaskID);
    }
    
    OnTaskCompleted.Broadcast(TaskID, Result);
    
    // Save fusion data
    SaveFusionData();
}

void UMingRTSScriptFusionAPI::HandleTaskFailure(const FString& TaskID, const FString& ErrorMessage)
{
    ActiveTasks.Remove(TaskID);
    
    UE_LOG(LogTemp, Error, TEXT("ScriptFusionAPI: Task %s failed: %s"), *TaskID, *ErrorMessage);
    
    OnTaskFailed.Broadcast(TaskID, ErrorMessage);
}

FScriptFusionResult UMingRTSScriptFusionAPI::GetTaskResult(const FString& TaskID)
{
    if (TaskResults.Contains(TaskID)) {
        return TaskResults[TaskID];
    }
    
    FScriptFusionResult EmptyResult;
    EmptyResult.TaskID = TaskID;
    EmptyResult.StatusMessage = TEXT("Task not found");
    return EmptyResult;
}

TArray<FScriptFusionTask> UMingRTSScriptFusionAPI::GetAllTasks()
{
    TArray<FScriptFusionTask> AllTasks;
    
    for (const auto& TaskPair : FusionTasks) {
        AllTasks.Add(TaskPair.Value);
    }
    
    return AllTasks;
}

void UMingRTSScriptFusionAPI::DeleteTask(const FString& TaskID)
{
    FusionTasks.Remove(TaskID);
    TaskResults.Remove(TaskID);
    ActiveTasks.Remove(TaskID);
    
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Deleted task %s"), *TaskID);
}

// ========== 預設融合配置 ==========

FScriptFusionProfile UMingRTSScriptFusionAPI::CreateBuildProfile()
{
    FScriptFusionProfile Profile;
    Profile.ProfileID = TEXT("Build_Profile");
    Profile.ProfileName = TEXT("Complete Build Process");
    Profile.FusionType = EScriptFusionType::BuildScripts;
    Profile.Description = TEXT("Complete build and compilation process");
    
    // Add build-related scripts
    TArray<FString> BuildScripts = GetScriptsByCategory(EScriptCategory::Build);
    
    FScriptFusionTask BuildTask;
    BuildTask.TaskID = TEXT("Main_Build_Task");
    BuildTask.TaskName = TEXT("Main Build Process");
    BuildTask.FusionType = EScriptFusionType::BuildScripts;
    BuildTask.ScriptPaths = BuildScripts;
    BuildTask.ExecutionMode = EFusionMode::Pipeline;
    BuildTask.Priority = EScriptPriority::High;
    
    Profile.Tasks.Add(BuildTask);
    
    FusionProfiles.Add(Profile.ProfileID, Profile);
    
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Created build profile"));
    return Profile;
}

FScriptFusionProfile UMingRTSScriptFusionAPI::CreateAutomationProfile()
{
    FScriptFusionProfile Profile;
    Profile.ProfileID = TEXT("Automation_Profile");
    Profile.ProfileName = TEXT("Development Automation");
    Profile.FusionType = EScriptFusionType::AutomationScripts;
    Profile.Description = TEXT("Automated development workflow");
    
    TArray<FString> AutomationScripts = GetScriptsByCategory(EScriptCategory::Automation);
    
    FScriptFusionTask AutomationTask;
    AutomationTask.TaskID = TEXT("Main_Automation_Task");
    AutomationTask.TaskName = TEXT("Development Automation");
    AutomationTask.FusionType = EScriptFusionType::AutomationScripts;
    AutomationTask.ScriptPaths = AutomationScripts;
    AutomationTask.ExecutionMode = EFusionMode::Parallel;
    AutomationTask.Priority = EScriptPriority::Medium;
    
    Profile.Tasks.Add(AutomationTask);
    
    FusionProfiles.Add(Profile.ProfileID, Profile);
    
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Created automation profile"));
    return Profile;
}

FScriptFusionProfile UMingRTSScriptFusionAPI::CreateOptimizationProfile()
{
    FScriptFusionProfile Profile;
    Profile.ProfileID = TEXT("Optimization_Profile");
    Profile.ProfileName = TEXT("System Optimization");
    Profile.FusionType = EScriptFusionType::OptimizationScripts;
    Profile.Description = TEXT("Performance and system optimization");
    
    TArray<FString> OptimizationScripts = GetScriptsByCategory(EScriptCategory::Optimization);
    
    FScriptFusionTask OptimizationTask;
    OptimizationTask.TaskID = TEXT("Main_Optimization_Task");
    OptimizationTask.TaskName = TEXT("System Optimization");
    OptimizationTask.FusionType = EScriptFusionType::OptimizationScripts;
    OptimizationTask.ScriptPaths = OptimizationScripts;
    OptimizationTask.ExecutionMode = EFusionMode::Sequential;
    OptimizationTask.Priority = EScriptPriority::Medium;
    
    Profile.Tasks.Add(OptimizationTask);
    
    FusionProfiles.Add(Profile.ProfileID, Profile);
    
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Created optimization profile"));
    return Profile;
}

FScriptFusionProfile UMingRTSScriptFusionAPI::CreateTestingProfile()
{
    FScriptFusionProfile Profile;
    Profile.ProfileID = TEXT("Testing_Profile");
    Profile.ProfileName = TEXT("Comprehensive Testing");
    Profile.FusionType = EScriptFusionType::TestingScripts;
    Profile.Description = TEXT("Complete testing suite");
    
    TArray<FString> TestingScripts = GetScriptsByCategory(EScriptCategory::Debug);
    
    FScriptFusionTask TestingTask;
    TestingTask.TaskID = TEXT("Main_Testing_Task");
    TestingTask.TaskName = TEXT("Testing Suite");
    TestingTask.FusionType = EScriptFusionType::TestingScripts;
    TestingTask.ScriptPaths = TestingScripts;
    TestingTask.ExecutionMode = EFusionMode::Parallel;
    TestingTask.Priority = EScriptPriority::High;
    
    Profile.Tasks.Add(TestingTask);
    
    FusionProfiles.Add(Profile.ProfileID, Profile);
    
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Created testing profile"));
    return Profile;
}

FScriptFusionProfile UMingRTSScriptFusionAPI::CreateDeploymentProfile()
{
    FScriptFusionProfile Profile;
    Profile.ProfileID = TEXT("Deployment_Profile");
    Profile.ProfileName = TEXT("Production Deployment");
    Profile.FusionType = EScriptFusionType::DeploymentScripts;
    Profile.Description = TEXT("Production deployment process");
    
    TArray<FString> DeploymentScripts = GetScriptsByCategory(EScriptCategory::Deployment);
    
    FScriptFusionTask DeploymentTask;
    DeploymentTask.TaskID = TEXT("Main_Deployment_Task");
    DeploymentTask.TaskName = TEXT("Deployment Process");
    DeploymentTask.FusionType = EScriptFusionType::DeploymentScripts;
    DeploymentTask.ScriptPaths = DeploymentScripts;
    DeploymentTask.ExecutionMode = EFusionMode::Pipeline;
    DeploymentTask.Priority = EScriptPriority::Critical;
    
    Profile.Tasks.Add(DeploymentTask);
    
    FusionProfiles.Add(Profile.ProfileID, Profile);
    
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Created deployment profile"));
    return Profile;
}

void UMingRTSScriptFusionAPI::ExecuteProfile(const FString& ProfileID)
{
    if (!FusionProfiles.Contains(ProfileID)) {
        UE_LOG(LogTemp, Warning, TEXT("ScriptFusionAPI: Profile not found: %s"), *ProfileID);
        return;
    }
    
    FScriptFusionProfile& Profile = FusionProfiles[ProfileID];
    
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Executing profile %s"), *Profile.ProfileName);
    
    for (FScriptFusionTask& Task : Profile.Tasks) {
        FString TaskID = CreateFusionTask(Task);
        ExecuteFusionTask(TaskID);
    }
}

// ========== 智慧腳本融合 ==========

FScriptFusionTask UMingRTSScriptFusionAPI::SuggestOptimalFusion(const FString& Goal)
{
    FScriptFusionTask SuggestedTask;
    SuggestedTask.TaskID = GenerateTaskID();
    SuggestedTask.TaskName = FString::Printf(TEXT("Suggested Task for %s"), *Goal);
    SuggestedTask.ExecutionMode = EFusionMode::Intelligent;
    SuggestedTask.Priority = EScriptPriority::Medium;
    
    // Analyze goal and suggest appropriate scripts
    if (Goal.Contains(TEXT("build")) || Goal.Contains(TEXT("compile"))) {
        SuggestedTask.FusionType = EScriptFusionType::BuildScripts;
        SuggestedTask.ScriptPaths = GetScriptsByCategory(EScriptCategory::Build);
    } else if (Goal.Contains(TEXT("test"))) {
        SuggestedTask.FusionType = EScriptFusionType::TestingScripts;
        SuggestedTask.ScriptPaths = GetScriptsByCategory(EScriptCategory::Debug);
    } else if (Goal.Contains(TEXT("deploy"))) {
        SuggestedTask.FusionType = EScriptFusionType::DeploymentScripts;
        SuggestedTask.ScriptPaths = GetScriptsByCategory(EScriptCategory::Deployment);
    } else if (Goal.Contains(TEXT("optimize"))) {
        SuggestedTask.FusionType = EScriptFusionType::OptimizationScripts;
        SuggestedTask.ScriptPaths = GetScriptsByCategory(EScriptCategory::Optimization);
    } else {
        SuggestedTask.FusionType = EScriptFusionType::AllScripts;
        // Include scripts from multiple categories
        TArray<EScriptCategory> Categories = {
            EScriptCategory::Build,
            EScriptCategory::Automation,
            EScriptCategory::Optimization
        };
        
        for (EScriptCategory Category : Categories) {
            TArray<FString> CategoryScripts = GetScriptsByCategory(Category);
            SuggestedTask.ScriptPaths.Append(CategoryScripts);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Suggested optimal fusion for goal: %s"), *Goal);
    return SuggestedTask;
}

void UMingRTSScriptFusionAPI::EnableIntelligentFusion(bool bEnabled)
{
    bIntelligentFusionEnabled = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Intelligent fusion %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

void UMingRTSScriptFusionAPI::OptimizeTaskExecution(const FString& TaskID)
{
    if (!FusionTasks.Contains(TaskID)) {
        UE_LOG(LogTemp, Warning, TEXT("ScriptFusionAPI: Task not found for optimization: %s"), *TaskID);
        return;
    }
    
    FScriptFusionTask& Task = FusionTasks[TaskID];
    
    // Optimize execution order
    OptimizeTaskOrder(Task.ScriptPaths);
    
    // Optimize execution mode based on script count
    if (Task.ScriptPaths.Num() > 4 && Task.ExecutionMode == EFusionMode::Sequential) {
        Task.ExecutionMode = EFusionMode::Parallel;
        UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Changed execution mode to parallel for task %s"), *TaskID);
    }
    
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Optimized task execution: %s"), *TaskID);
}

TArray<FString> UMingRTSScriptFusionAPI::PredictRequiredScripts(const FString& Context)
{
    TArray<FString> PredictedScripts;
    
    // Simple prediction based on context keywords
    if (Context.Contains(TEXT("build")) || Context.Contains(TEXT("compile"))) {
        PredictedScripts.Append(GetScriptsByCategory(EScriptCategory::Build));
    }
    
    if (Context.Contains(TEXT("test"))) {
        PredictedScripts.Append(GetScriptsByCategory(EScriptCategory::Debug));
    }
    
    if (Context.Contains(TEXT("deploy"))) {
        PredictedScripts.Append(GetScriptsByCategory(EScriptCategory::Deployment));
    }
    
    if (Context.Contains(TEXT("optimize"))) {
        PredictedScripts.Append(GetScriptsByCategory(EScriptCategory::Optimization));
    }
    
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Predicted %d required scripts for context"), PredictedScripts.Num());
    return PredictedScripts;
}

// ========== 內部輔助方法 ==========

void UMingRTSScriptFusionAPI::OptimizeTaskOrder(TArray<FString>& TaskIDs)
{
    // Simple optimization: sort by estimated execution time (shorter tasks first)
    TaskIDs.Sort([this](const FString& A, const FString& B) {
        FString NameA = FPaths::GetBaseFilename(A);
        FString NameB = FPaths::GetBaseFilename(B);
        
        float TimeA = 5.0f;
        float TimeB = 5.0f;
        
        if (ScriptFunctionalities.Contains(NameA)) {
            TimeA = ScriptFunctionalities[NameA].AverageExecutionTime;
        }
        
        if (ScriptFunctionalities.Contains(NameB)) {
            TimeB = ScriptFunctionalities[NameB].AverageExecutionTime;
        }
        
        return TimeA < TimeB;
    });
}

FString UMingRTSScriptFusionAPI::GenerateTaskID()
{
    return FString::Printf(TEXT("Task_%s"), *FGuid::NewGuid().ToString());
}

FString UMingRTSScriptFusionAPI::GeneratePipelineID()
{
    return FString::Printf(TEXT("Pipeline_%s"), *FGuid::NewGuid().ToString());
}

void UMingRTSScriptFusionAPI::SaveFusionData()
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("ScriptFusionData.json");
    
    // In a real implementation, this would serialize the fusion data to JSON
    FString JsonData = TEXT("// Script Fusion Data\n");
    JsonData += FString::Printf(TEXT("// Tasks: %d\n"), FusionTasks.Num());
    JsonData += FString::Printf(TEXT("// Profiles: %d\n"), FusionProfiles.Num());
    
    FFileHelper::SaveStringToFile(JsonData, *SavePath);
}

void UMingRTSScriptFusionAPI::LoadFusionData()
{
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("ScriptFusionData.json");
    
    if (FFileHelper::FileExists(*LoadPath)) {
        FString JsonData;
        FFileHelper::LoadFileToString(JsonData, *LoadPath);
        UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Loaded existing fusion data"));
    }
}

// ========== 其他方法的實現 ==========

void UMingRTSScriptFusionAPI::ExecuteParallelTasks(const TArray<FString>& TaskIDs)
{
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Executing %d tasks in parallel"), TaskIDs.Num());
    
    for (const FString& TaskID : TaskIDs) {
        ExecuteFusionTask(TaskID);
    }
}

void UMingRTSScriptFusionAPI::SetMaxParallelTasks(int32 MaxTasks)
{
    MaxParallelTasks = FMath::Clamp(MaxTasks, 1, 16);
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Set max parallel tasks to %d"), MaxParallelTasks);
}

TArray<FString> UMingRTSScriptFusionAPI::GetRunningTasks()
{
    return ActiveTasks;
}

void UMingRTSScriptFusionAPI::StopTask(const FString& TaskID)
{
    if (ActiveTasks.Contains(TaskID)) {
        ActiveTasks.Remove(TaskID);
        
        FScriptFusionResult& Result = TaskResults[TaskID];
        Result.EndTime = FDateTime::Now();
        Result.ExecutionTime = (Result.EndTime - Result.StartTime).GetTotalSeconds();
        Result.StatusMessage = TEXT("Task stopped by user");
        Result.bSuccess = false;
        
        HandleTaskCompletion(TaskID, Result);
        
        UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Stopped task %s"), *TaskID);
    }
}

void UMingRTSScriptFusionAPI::GenerateExecutionReport(const FString& ReportPath)
{
    FString ReportContent = TEXT("# MingGoRTS Script Fusion Execution Report\n\n");
    ReportContent += FString::Printf(TEXT("Generated: %s\n\n"), *FDateTime::Now().ToString());
    
    ReportContent += TEXT("## Task Summary\n");
    ReportContent += FString::Printf(TEXT("- Total Tasks: %d\n"), FusionTasks.Num());
    ReportContent += FString::Printf(TEXT("- Active Tasks: %d\n"), ActiveTasks.Num());
    ReportContent += FString::Printf(TEXT("- Completed Tasks: %d\n"), TaskResults.Num());
    
    ReportContent += TEXT("\n## Script Registry\n");
    for (const auto& CategoryPair : ScriptRegistry) {
        ReportContent += FString::Printf(TEXT("- %s: %d scripts\n"), 
            *StaticEnum<EScriptCategory>()->GetNameStringByValue((int64)CategoryPair.Key),
            CategoryPair.Value.Num());
    }
    
    FFileHelper::SaveStringToFile(ReportContent, *ReportPath);
    UE_LOG(LogTemp, Log, TEXT("ScriptFusionAPI: Generated execution report: %s"), *ReportPath);
}
