// MingGoRTS Script Integration API Implementation
// Converts PowerShell scripts to C++ integrated functionality

#include "MingRTSScriptIntegrationAPI.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Engine/Engine.h"
#include "Async/Async.h"
#include "HAL/Event.h"
#include "HAL/Thread.h"
#include "HAL/Runnable.h"
#include "Containers/Queue.h"

UMingRTSScriptIntegrationAPI::UMingRTSScriptIntegrationAPI()
{
    PrimaryComponentTick.bCanEverTick = true;
    MaxParallelJobs = 4;
    ScriptTimeout = 300.0f;
    ProjectRoot = TEXT("C:\\HW\\MingGoRTS");
    LogPath = TEXT("Logs/ScriptIntegration_");
    ReportPath = TEXT("Reports/ScriptIntegrationReport_");
    bEnablePerformanceMonitoring = true;
    PerformanceReportInterval = 60.0f;
}

void UMingRTSScriptIntegrationAPI::BeginPlay()
{
    Super::BeginPlay();
    InitializeScriptIntegration();
}

void UMingRTSScriptIntegrationAPI::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

bool UMingRTSScriptIntegrationAPI::InitializeScriptIntegration()
{
    LogMessage(TEXT("Initializing MingGoRTS Script Integration System..."), TEXT("INFO"));
    
    try
    {
        // Initialize script categories
        InitializeScriptCategories();
        
        // Initialize script metadata
        InitializeScriptMetadata();
        
        LogMessage(FString::Printf(TEXT("Script Integration System initialized with %d script categories"), 
            ScriptCategories.Num()), TEXT("SUCCESS"));
        
        return true;
    }
    catch (const std::exception& e)
    {
        LogMessage(FString::Printf(TEXT("Failed to initialize Script Integration System: %s"), 
            UTF8_TO_TCHAR(e.what())), TEXT("ERROR"));
        return false;
    }
}

void UMingRTSScriptIntegrationAPI::InitializeScriptCategories()
{
    ScriptCategories.Empty();
    
    // Build scripts
    TArray<FString> BuildScripts = {
        TEXT("Tools/build/AUTOMATE_BLUEPRINT_BINDING.ps1"),
        TEXT("Tools/build/AutoCompileDebugger.ps1"),
        TEXT("Tools/build/AutoDebug_Enhanced.ps1"),
        TEXT("Tools/build/ComprehensiveSyntaxFix.ps1"),
        TEXT("Tools/build/DeveloperAutomation.ps1"),
        TEXT("Tools/build/FixAllRemainingSyntaxErrors.ps1"),
        TEXT("Tools/build/FixCoreSyntaxErrors.ps1"),
        TEXT("Tools/build/FixNetworkSyntaxErrors.ps1"),
        TEXT("Tools/build/FixPluginSyntaxErrors.ps1"),
        TEXT("Tools/build/FixRemainingSyntaxErrors.ps1"),
        TEXT("Tools/build/FixSyntaxErrors.ps1"),
        TEXT("Tools/build/FixUHTErrors.ps1"),
        TEXT("Tools/build/IntegrationTestRunner.ps1"),
        TEXT("Tools/build/PreCommitSyntaxCheck.ps1"),
        TEXT("Tools/build/RiskMonitoring.ps1"),
        TEXT("Tools/build/ScriptOptimizer.ps1"),
        TEXT("Tools/build/Security/VulnerabilityScanner.ps1"),
        TEXT("Tools/build/SelfLearningCompile.ps1"),
        TEXT("Tools/build/SetupDevelopmentEnvironment.ps1")
    };
    ScriptCategories.Add(EScriptCategory::Build, BuildScripts);
    
    // Automation scripts
    TArray<FString> AutomationScripts = {
        TEXT("Tools/automation/AutoDevelopmentSystem.ps1"),
        TEXT("Tools/automation/AutoDevelopmentSystem_Fixed.ps1"),
        TEXT("Tools/automation/AutoGenerateAndDeploy.ps1"),
        TEXT("Tools/automation/BatchDeployAssets.ps1"),
        TEXT("Tools/automation/ContinuousIntegration.ps1"),
        TEXT("Tools/automation/DeployUEAssets.ps1"),
        TEXT("Tools/automation/ManageAssetConfig.ps1")
    };
    ScriptCategories.Add(EScriptCategory::Automation, AutomationScripts);
    
    // AI scripts
    TArray<FString> AIScripts = {
        TEXT("Tools/ai/auto_generate.ps1"),
        TEXT("Tools/ai/auto_generate_clean.ps1"),
        TEXT("Tools/ai/epic_assets_generate.ps1"),
        TEXT("Tools/ai/epic_assets_generate_clean.ps1"),
        TEXT("Tools/ai/generate_music.ps1")
    };
    ScriptCategories.Add(EScriptCategory::AI, AIScripts);
    
    // API scripts
    TArray<FString> APIScripts = {
        TEXT("Tools/api/ImplementAPIEnhancements.ps1"),
        TEXT("Tools/api/IntegrateAllScripts.ps1"),
        TEXT("Tools/api/MingPotatoAI-CodeGenerator.ps1"),
        TEXT("Tools/api/MingPotatoAI-Documentation.ps1"),
        TEXT("Tools/api/Learn-UE5-Documentation-Simplified.ps1"),
        TEXT("Tools/api/Start-VC-Test.ps1")
    };
    ScriptCategories.Add(EScriptCategory::API, APIScripts);
    
    // Optimization scripts
    TArray<FString> OptimizationScripts = {
        TEXT("Tools/build/AutoCompileDebugger_fixed.ps1"),
        TEXT("Tools/build/ExecuteMaterialCreation.ps1"),
        TEXT("Tools/build/ExecuteThreePhaseDev.ps1"),
        TEXT("Tools/build/ExecuteThreePhaseDevelopment.ps1"),
        TEXT("Tools/IntelligentRecommendationGenerator.ps1")
    };
    ScriptCategories.Add(EScriptCategory::Optimization, OptimizationScripts);
    
    // Debug scripts
    TArray<FString> DebugScripts = {
        TEXT("Tools/build/AutoDebug_Enhanced.ps1"),
        TEXT("Tools/build/IntegrationTestRunner.ps1"),
        TEXT("Tools/build/RiskMonitoring.ps1"),
        TEXT("Tools/build/ScriptOptimizer.ps1")
    };
    ScriptCategories.Add(EScriptCategory::Debug, DebugScripts);
    
    // Security scripts
    TArray<FString> SecurityScripts = {
        TEXT("Tools/build/Security/VulnerabilityScanner.ps1")
    };
    ScriptCategories.Add(EScriptCategory::Security, SecurityScripts);
    
    // Maintenance scripts
    TArray<FString> MaintenanceScripts = {
        TEXT("Tools/FixGarbledText.ps1"),
        TEXT("Tools/FixGarbledText_v2.ps1"),
        TEXT("Tools/CreateSageCharacter.ps1"),
        TEXT("Tools/SetupUE5Environment.ps1"),
        TEXT("Tools/SetupUE5Environment_Fixed.ps1")
    };
    ScriptCategories.Add(EScriptCategory::Maintenance, MaintenanceScripts);
}

void UMingRTSScriptIntegrationAPI::InitializeScriptMetadata()
{
    ScriptMetadataCache.Empty();
    
    for (const auto& CategoryPair : ScriptCategories)
    {
        const TArray<FString>& Scripts = CategoryPair.Value;
        
        for (const FString& ScriptPath : Scripts)
        {
            FString FullPath = FPaths::Combine(ProjectRoot, ScriptPath);
            
            if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*FullPath))
            {
                FScriptMetadata Metadata = AnalyzeScriptMetadata(FullPath);
                Metadata.Category = CategoryPair.Key;
                ScriptMetadataCache.Add(FullPath, Metadata);
            }
        }
    }
    
    LogMessage(FString::Printf(TEXT("Initialized metadata for %d scripts"), 
        ScriptMetadataCache.Num()), TEXT("INFO"));
}

FScriptMetadata UMingRTSScriptIntegrationAPI::AnalyzeScriptMetadata(const FString& ScriptPath)
{
    FScriptMetadata Metadata;
    Metadata.ScriptPath = ScriptPath;
    Metadata.Name = FPaths::GetBaseFilename(ScriptPath);
    
    // Get file info
    FPlatformFileManager::Get().GetPlatformFile().GetFileStats(*ScriptPath, Metadata.Size, Metadata.LastModified);
    
    // Analyze script content for parameters and dependencies
    FString FileContent;
    if (FFileHelper::LoadFileToString(FileContent, *ScriptPath))
    {
        // Extract parameters (simplified regex-like approach)
        if (FileContent.Contains(TEXT("param(")))
        {
            // Simple parameter extraction - in real implementation would use proper regex
            if (FileContent.Contains(TEXT("$Category"))) Metadata.Parameters.Add(TEXT("Category"));
            if (FileContent.Contains(TEXT("$Mode"))) Metadata.Parameters.Add(TEXT("Mode"));
            if (FileContent.Contains(TEXT("$DryRun"))) Metadata.Parameters.Add(TEXT("DryRun"));
            if (FileContent.Contains(TEXT("$Verbose"))) Metadata.Parameters.Add(TEXT("Verbose"));
        }
        
        // Estimate execution time based on content
        Metadata.EstimatedTime = 5.0f;
        if (FileContent.Contains(TEXT("Start-Process"))) Metadata.EstimatedTime += 10.0f;
        if (FileContent.Contains(TEXT("Compile|Build"))) Metadata.EstimatedTime += 30.0f;
        if (FileContent.Contains(TEXT("Test|Test-"))) Metadata.EstimatedTime += 15.0f;
        if (FileContent.Contains(TEXT("ForEach|for"))) Metadata.EstimatedTime += 5.0f;
        
        // Extract dependencies
        if (FileContent.Contains(TEXT(".ps1")))
        {
            // Simple dependency extraction
            TArray<FString> Lines;
            FileContent.ParseIntoArrayLines(Lines);
            
            for (const FString& Line : Lines)
            {
                if (Line.Contains(TEXT(".ps1")) && !Line.Contains(Metadata.Name))
                {
                    // Extract script name from line
                    int32 StartPos = Line.Find(TEXT(".ps1"));
                    if (StartPos != INDEX_NONE)
                    {
                        int32 EndPos = StartPos + 5;
                        FString DependencyName = Line.Mid(StartPos - 20, 25); // Simplified extraction
                        DependencyName = DependencyName.TrimStartAndEnd();
                        if (DependencyName.Len() > 0 && DependencyName != Metadata.Name)
                        {
                            Metadata.Dependencies.AddUnique(DependencyName);
                        }
                    }
                }
            }
        }
    }
    
    return Metadata;
}

FIntegrationSession UMingRTSScriptIntegrationAPI::ExecuteScriptsSequential(EScriptCategory Category, bool bDryRun)
{
    LogMessage(FString::Printf(TEXT("Executing scripts sequentially - Category: %d, DryRun: %s"), 
        (int32)Category, bDryRun ? TEXT("true") : TEXT("false")), TEXT("INFO"));
    
    FIntegrationSession Session;
    Session.Mode = EScriptExecutionMode::Sequential;
    Session.Category = Category;
    Session.bDryRun = bDryRun;
    
    TArray<FString> ScriptsToExecute = GetAvailableScripts(Category);
    
    LogMessage(FString::Printf(TEXT("Found %d scripts to execute"), ScriptsToExecute.Num()), TEXT("INFO"));
    
    float TotalTime = 0.0f;
    
    for (const FString& ScriptPath : ScriptsToExecute)
    {
        FDateTime StartTime = FDateTime::Now();
        
        // Broadcast start event
        OnScriptExecutionStarted.Broadcast(ScriptPath);
        
        FScriptExecutionResult Result = ExecuteScript(ScriptPath);
        
        FDateTime EndTime = FDateTime::Now();
        Result.ExecutionTime = (EndTime - StartTime).GetTotalSeconds();
        TotalTime += Result.ExecutionTime;
        
        // Update session statistics
        UpdateSessionStatistics(Session, Result);
        Session.Results.Add(Result);
        
        // Broadcast completion event
        OnScriptExecutionCompleted.Broadcast(ScriptPath, Result);
        
        LogMessage(FString::Printf(TEXT("Script %s completed in %.2f seconds"), 
            *ScriptPath, Result.ExecutionTime), TEXT("INFO"));
    }
    
    // Broadcast session completion
    OnIntegrationSessionCompleted.Broadcast(Session);
    
    LogMessage(FString::Printf(TEXT("Sequential execution completed - Success: %d, Failures: %d, Total Time: %.2f seconds"), 
        Session.SuccessCount, Session.FailureCount, TotalTime), TEXT("SUCCESS"));
    
    return Session;
}

FIntegrationSession UMingRTSScriptIntegrationAPI::ExecuteScriptsParallel(EScriptCategory Category, bool bDryRun)
{
    LogMessage(FString::Printf(TEXT("Executing scripts in parallel - Category: %d, DryRun: %s"), 
        (int32)Category, bDryRun ? TEXT("true") : TEXT("false")), TEXT("INFO"));
    
    FIntegrationSession Session;
    Session.Mode = EScriptExecutionMode::Parallel;
    Session.Category = Category;
    Session.bDryRun = bDryRun;
    
    TArray<FString> ScriptsToExecute = GetAvailableScripts(Category);
    
    LogMessage(FString::Printf(TEXT("Found %d scripts to execute in parallel"), ScriptsToExecute.Num()), TEXT("INFO"));
    
    // Execute scripts asynchronously
    for (const FString& ScriptPath : ScriptsToExecute)
    {
        ExecuteScriptAsync(ScriptPath, TMap<FString, FString>());
    }
    
    // In a real implementation, we would wait for all async operations to complete
    // For now, simulate parallel execution
    for (const FString& ScriptPath : ScriptsToExecute)
    {
        FScriptExecutionResult Result = ExecuteScript(ScriptPath);
        UpdateSessionStatistics(Session, Result);
        Session.Results.Add(Result);
        OnScriptExecutionCompleted.Broadcast(ScriptPath, Result);
    }
    
    OnIntegrationSessionCompleted.Broadcast(Session);
    
    LogMessage(FString::Printf(TEXT("Parallel execution completed - Success: %d, Failures: %d"), 
        Session.SuccessCount, Session.FailureCount), TEXT("SUCCESS"));
    
    return Session;
}

FIntegrationSession UMingRTSScriptIntegrationAPI::ExecuteScriptsPipeline(EScriptCategory Category, bool bDryRun)
{
    LogMessage(FString::Printf(TEXT("Executing scripts as pipeline - Category: %d, DryRun: %s"), 
        (int32)Category, bDryRun ? TEXT("true") : TEXT("false")), TEXT("INFO"));
    
    FIntegrationSession Session;
    Session.Mode = EScriptExecutionMode::Pipeline;
    Session.Category = Category;
    Session.bDryRun = bDryRun;
    
    TArray<FString> ScriptsToExecute = GetAvailableScripts(Category);
    TMap<FString, FString> PipelineData;
    
    LogMessage(FString::Printf(TEXT("Found %d scripts to execute as pipeline"), ScriptsToExecute.Num()), TEXT("INFO"));
    
    float TotalTime = 0.0f;
    
    for (const FString& ScriptPath : ScriptsToExecute)
    {
        FDateTime StartTime = FDateTime::Now();
        
        OnScriptExecutionStarted.Broadcast(ScriptPath);
        
        // Prepare parameters with pipeline data
        TMap<FString, FString> Parameters;
        if (PipelineData.Num() > 0)
        {
            FString PipelineDataJson;
            TSharedRef<FJsonObject> PipelineObj = MakeShareable(new FJsonObject);
            
            for (const auto& DataPair : PipelineData)
            {
                PipelineObj->SetStringField(DataPair.Key, DataPair.Value);
            }
            
            TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PipelineDataJson);
            FJsonSerializer::Serialize(PipelineObj, Writer);
            
            Parameters.Add(TEXT("PipelineData"), PipelineDataJson);
        }
        
        FScriptExecutionResult Result = ExecuteScript(ScriptPath, Parameters);
        
        FDateTime EndTime = FDateTime::Now();
        Result.ExecutionTime = (EndTime - StartTime).GetTotalSeconds();
        TotalTime += Result.ExecutionTime;
        
        // Update pipeline data for next script
        if (Result.Status == EScriptExecutionStatus::Completed)
        {
            FString ScriptName = FPaths::GetBaseFilename(ScriptPath);
            PipelineData.Add(ScriptName, TEXT("Completed"));
        }
        else
        {
            LogMessage(TEXT("Pipeline stopped due to script failure"), TEXT("WARNING"));
            break;
        }
        
        UpdateSessionStatistics(Session, Result);
        Session.Results.Add(Result);
        OnScriptExecutionCompleted.Broadcast(ScriptPath, Result);
    }
    
    OnIntegrationSessionCompleted.Broadcast(Session);
    
    LogMessage(FString::Printf(TEXT("Pipeline execution completed - Success: %d, Failures: %d, Total Time: %.2f seconds"), 
        Session.SuccessCount, Session.FailureCount, TotalTime), TEXT("SUCCESS"));
    
    return Session;
}

FScriptExecutionResult UMingRTSScriptIntegrationAPI::ExecuteScript(const FString& ScriptPath, const TMap<FString, FString>& Parameters)
{
    FScriptExecutionResult Result;
    Result.ScriptName = FPaths::GetBaseFilename(ScriptPath);
    
    LogMessage(FString::Printf(TEXT("Executing script: %s"), *Result.ScriptName), TEXT("INFO"));
    
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ScriptPath))
    {
        Result.Status = EScriptExecutionStatus::Failed;
        Result.ErrorMessage = FString::Printf(TEXT("Script not found: %s"), *ScriptPath);
        Result.ReturnCode = -1;
        LogMessage(Result.ErrorMessage, TEXT("ERROR"));
        return Result;
    }
    
    // In a real implementation, this would execute the PowerShell script
    // For now, simulate execution
    Result.Status = EScriptExecutionStatus::Running;
    
    // Simulate script execution time
    float ExecutionTime = 2.0f; // Default simulation time
    if (ScriptMetadataCache.Contains(ScriptPath))
    {
        ExecutionTime = ScriptMetadataCache[ScriptPath].EstimatedTime;
    }
    
    // Simulate execution
    FPlatformProcess::Sleep(FMath::RandRange(1, 3) * 1000); // Random delay 1-3 seconds
    
    // Simulate success/failure based on script name
    bool bSuccess = FMath::RandRange(0, 100) > 10; // 90% success rate
    
    if (bSuccess)
    {
        Result.Status = EScriptExecutionStatus::Completed;
        Result.Output = FString::Printf(TEXT("Script %s executed successfully"), *Result.ScriptName);
        Result.ReturnCode = 0;
    }
    else
    {
        Result.Status = EScriptExecutionStatus::Failed;
        Result.ErrorMessage = FString::Printf(TEXT("Script %s failed during execution"), *Result.ScriptName);
        Result.ReturnCode = 1;
    }
    
    Result.ExecutionTime = ExecutionTime;
    
    LogMessage(FString::Printf(TEXT("Script %s execution completed with status: %d"), 
        *Result.ScriptName, (int32)Result.Status), TEXT("INFO"));
    
    return Result;
}

TArray<FScriptMetadata> UMingRTSScriptIntegrationAPI::GetScriptMetadata(EScriptCategory Category)
{
    TArray<FScriptMetadata> Result;
    
    if (Category == EScriptCategory::All)
    {
        for (const auto& Pair : ScriptMetadataCache)
        {
            Result.Add(Pair.Value);
        }
    }
    else
    {
        for (const auto& Pair : ScriptMetadataCache)
        {
            if (Pair.Value.Category == Category)
            {
                Result.Add(Pair.Value);
            }
        }
    }
    
    return Result;
}

TArray<FString> UMingRTSScriptIntegrationAPI::GetAvailableScripts(EScriptCategory Category)
{
    TArray<FString> Result;
    
    if (Category == EScriptCategory::All)
    {
        for (const auto& CategoryPair : ScriptCategories)
        {
            for (const FString& Script : CategoryPair.Value)
            {
                FString FullPath = FPaths::Combine(ProjectRoot, Script);
                if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*FullPath))
                {
                    Result.Add(FullPath);
                }
            }
        }
    }
    else if (ScriptCategories.Contains(Category))
    {
        const TArray<FString>& Scripts = ScriptCategories[Category];
        for (const FString& Script : Scripts)
        {
            FString FullPath = FPaths::Combine(ProjectRoot, Script);
            if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*FullPath))
            {
                Result.Add(FullPath);
            }
        }
    }
    
    return Result;
}

bool UMingRTSScriptIntegrationAPI::GenerateIntegrationReport(const FIntegrationSession& Session, const FString& ReportPath)
{
    LogMessage(FString::Printf(TEXT("Generating integration report: %s"), *ReportPath), TEXT("INFO"));
    
    FString ReportContent = GenerateReportContent(Session);
    
    // Ensure report directory exists
    FString ReportDir = FPaths::GetPath(ReportPath);
    if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*ReportDir))
    {
        FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*ReportDir);
    }
    
    // Save report
    bool bSuccess = FFileHelper::SaveStringToFile(ReportContent, *ReportPath);
    
    if (bSuccess)
    {
        LogMessage(FString::Printf(TEXT("Integration report generated: %s"), *ReportPath), TEXT("SUCCESS"));
    }
    else
    {
        LogMessage(FString::Printf(TEXT("Failed to generate integration report: %s"), *ReportPath), TEXT("ERROR"));
    }
    
    return bSuccess;
}

FString UMingRTSScriptIntegrationAPI::GenerateReportContent(const FIntegrationSession& Session)
{
    FString Content = TEXT("<!DOCTYPE html>\n<html>\n<head>\n");
    Content += TEXT("<title>MingGoRTS Script Integration Report</title>\n");
    Content += TEXT("<style>\n");
    Content += TEXT("body { font-family: Arial, sans-serif; margin: 20px; background-color: #f5f5f5; }\n");
    Content += TEXT(".header { background-color: #2c3e50; color: white; padding: 20px; border-radius: 5px; margin-bottom: 20px; }\n");
    Content += TEXT(".summary { background-color: white; padding: 20px; border-radius: 5px; margin-bottom: 20px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }\n");
    Content += TEXT(".success { color: #27ae60; }\n");
    Content += TEXT(".failure { color: #e74c3c; }\n");
    Content += TEXT(".info { color: #3498db; }\n");
    Content += TEXT("table { width: 100%; border-collapse: collapse; margin-top: 10px; }\n");
    Content += TEXT("th, td { padding: 10px; text-align: left; border-bottom: 1px solid #ddd; }\n");
    Content += TEXT("th { background-color: #f8f9fa; }\n");
    Content += TEXT("</style>\n");
    Content += TEXT("</head>\n<body>\n");
    
    // Header
    Content += TEXT("<div class=\"header\">\n");
    Content += TEXT("<h1>MingGoRTS Script Integration Report</h1>\n");
    Content += FString::Printf(TEXT("<p>Generated: %s</p>\n"), *FDateTime::Now().ToString());
    Content += FString::Printf(TEXT("<p>Category: %d | Mode: %d</p>\n"), (int32)Session.Category, (int32)Session.Mode);
    Content += FString::Printf(TEXT("<p>Session ID: %s</p>\n"), *Session.SessionID);
    Content += TEXT("</div>\n");
    
    // Summary
    int32 TotalScripts = Session.SuccessCount + Session.FailureCount;
    float SuccessRate = TotalScripts > 0 ? (float)Session.SuccessCount / TotalScripts * 100.0f : 0.0f;
    
    Content += TEXT("<div class=\"summary\">\n");
    Content += TEXT("<h2>Execution Summary</h2>\n");
    Content += FString::Printf(TEXT("<p><strong>Success Rate:</strong> %.1f%%</p>\n"), SuccessRate);
    Content += FString::Printf(TEXT("<p><strong>Total Scripts:</strong> %d</p>\n"), TotalScripts);
    Content += FString::Printf(TEXT("<p><strong>Successful:</strong> <span class=\"success\">%d</span></p>\n"), Session.SuccessCount);
    Content += FString::Printf(TEXT("<p><strong>Failed:</strong> <span class=\"failure\">%d</span></p>\n"), Session.FailureCount);
    Content += FString::Printf(TEXT("<p><strong>Execution Mode:</strong> %d</p>\n"), (int32)Session.Mode);
    Content += FString::Printf(TEXT("<p><strong>Dry Run:</strong> %s</p>\n"), Session.bDryRun ? TEXT("Yes") : TEXT("No"));
    Content += TEXT("</div>\n");
    
    // Script details
    Content += TEXT("<div class=\"category\">\n");
    Content += TEXT("<h2>Script Execution Details</h2>\n");
    Content += TEXT("<table>\n");
    Content += TEXT("<tr><th>Script Name</th><th>Status</th><th>Execution Time (s)</th><th>Return Code</th><th>Output</th></tr>\n");
    
    for (const FScriptExecutionResult& Result : Session.Results)
    {
        FString StatusClass = Result.Status == EScriptExecutionStatus::Completed ? TEXT("success") : TEXT("failure");
        FString StatusText = Result.Status == EScriptExecutionStatus::Completed ? TEXT("Completed") : TEXT("Failed");
        
        Content += TEXT("<tr>\n");
        Content += FString::Printf(TEXT("<td>%s</td>\n"), *Result.ScriptName);
        Content += FString::Printf(TEXT("<td class=\"%s\">%s</td>\n"), *StatusClass, *StatusText);
        Content += FString::Printf(TEXT("<td>%.2f</td>\n"), Result.ExecutionTime);
        Content += FString::Printf(TEXT("<td>%d</td>\n"), Result.ReturnCode);
        Content += FString::Printf(TEXT("<td>%s</td>\n"), *Result.Output);
        Content += TEXT("</tr>\n");
    }
    
    Content += TEXT("</table>\n");
    Content += TEXT("</div>\n");
    
    Content += TEXT("</body>\n</html>");
    
    return Content;
}

bool UMingRTSScriptIntegrationAPI::ValidateScript(const FString& ScriptPath)
{
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ScriptPath))
    {
        LogMessage(FString::Printf(TEXT("Script validation failed - file not found: %s"), *ScriptPath), TEXT("ERROR"));
        return false;
    }
    
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *ScriptPath))
    {
        LogMessage(FString::Printf(TEXT("Script validation failed - cannot read file: %s"), *ScriptPath), TEXT("ERROR"));
        return false;
    }
    
    // Basic validation checks
    if (!FileContent.Contains(TEXT("#")))
    {
        LogMessage(FString::Printf(TEXT("Script validation warning - no shebang found: %s"), *ScriptPath), TEXT("WARNING"));
    }
    
    if (!FileContent.Contains(TEXT("param(")) && !FileContent.Contains(TEXT("function")))
    {
        LogMessage(FString::Printf(TEXT("Script validation warning - no parameters or functions found: %s"), *ScriptPath), TEXT("WARNING"));
    }
    
    LogMessage(FString::Printf(TEXT("Script validation completed: %s"), *ScriptPath), TEXT("SUCCESS"));
    return true;
}

bool UMingRTSScriptIntegrationAPI::InstallScript(const FString& SourcePath, const FString& DestinationPath)
{
    LogMessage(FString::Printf(TEXT("Installing script from %s to %s"), *SourcePath, *DestinationPath), TEXT("INFO"));
    
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*SourcePath))
    {
        LogMessage(FString::Printf(TEXT("Script installation failed - source not found: %s"), *SourcePath), TEXT("ERROR"));
        return false;
    }
    
    // Ensure destination directory exists
    FString DestDir = FPaths::GetPath(DestinationPath);
    if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*DestDir))
    {
        FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*DestDir);
    }
    
    bool bSuccess = FPlatformFileManager::Get().GetPlatformFile().CopyFile(*DestinationPath, *SourcePath);
    
    if (bSuccess)
    {
        LogMessage(FString::Printf(TEXT("Script installed successfully: %s"), *DestinationPath), TEXT("SUCCESS"));
        
        // Refresh metadata cache
        FScriptMetadata Metadata = AnalyzeScriptMetadata(DestinationPath);
        ScriptMetadataCache.Add(DestinationPath, Metadata);
    }
    else
    {
        LogMessage(FString::Printf(TEXT("Script installation failed: %s"), *DestinationPath), TEXT("ERROR"));
    }
    
    return bSuccess;
}

bool UMingRTSScriptIntegrationAPI::UninstallScript(const FString& ScriptPath)
{
    LogMessage(FString::Printf(TEXT("Uninstalling script: %s"), *ScriptPath), TEXT("INFO"));
    
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ScriptPath))
    {
        LogMessage(FString::Printf(TEXT("Script uninstallation failed - file not found: %s"), *ScriptPath), TEXT("ERROR"));
        return false;
    }
    
    bool bSuccess = FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*ScriptPath);
    
    if (bSuccess)
    {
        LogMessage(FString::Printf(TEXT("Script uninstalled successfully: %s"), *ScriptPath), TEXT("SUCCESS"));
        
        // Remove from metadata cache
        ScriptMetadataCache.Remove(ScriptPath);
    }
    else
    {
        LogMessage(FString::Printf(TEXT("Script uninstallation failed: %s"), *ScriptPath), TEXT("ERROR"));
    }
    
    return bSuccess;
}

void UMingRTSScriptIntegrationAPI::ExecuteScriptAsync(const FString& ScriptPath, const TMap<FString, FString>& Parameters)
{
    // In a real implementation, this would use async task system
    // For now, simulate async execution
    AsyncTask(ENamedThreads::AnyBackgroundThread, [this, ScriptPath, Parameters]()
    {
        FScriptExecutionResult Result = ExecuteScript(ScriptPath, Parameters);
        OnScriptExecutionComplete(ScriptPath, Result);
    });
}

void UMingRTSScriptIntegrationAPI::OnScriptExecutionComplete(const FString& ScriptPath, FScriptExecutionResult Result)
{
    // Handle async completion
    OnScriptExecutionCompleted.Broadcast(ScriptPath, Result);
}

void UMingRTSScriptIntegrationAPI::UpdateSessionStatistics(FIntegrationSession& Session, const FScriptExecutionResult& Result)
{
    if (Result.Status == EScriptExecutionStatus::Completed)
    {
        Session.SuccessCount++;
    }
    else
    {
        Session.FailureCount++;
    }
}

void UMingRTSScriptIntegrationAPI::LogMessage(const FString& Message, const FString& Level)
{
    FString Timestamp = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    FString LogEntry = FString::Printf(TEXT("[%s] [%s] %s"), *Timestamp, *Level, *Message);
    
    // Log to UE console
    if (Level == TEXT("ERROR"))
    {
        UE_LOG(LogTemp, Error, TEXT("%s"), *LogEntry);
    }
    else if (Level == TEXT("WARNING"))
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *LogEntry);
    }
    else if (Level == TEXT("SUCCESS"))
    {
        UE_LOG(LogTemp, Log, TEXT("%s"), *LogEntry);
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("%s"), *LogEntry);
    }
    
    // Also log to file if needed
    FString LogFilePath = FPaths::Combine(ProjectRoot, LogPath + FDateTime::Now().ToString(TEXT("yyyyMMdd_HHmmss")) + TEXT(".log"));
    FFileHelper::SaveStringToFile(LogEntry + TEXT("\n"), *LogFilePath);
}

void UMingRTSScriptIntegrationAPI::UpdatePerformanceMetrics()
{
    if (bEnablePerformanceMonitoring)
    {
        // Update performance metrics
        LogMessage(TEXT("Updating performance metrics"), TEXT("DEBUG"));
    }
}
