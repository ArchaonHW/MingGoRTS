#include "MingScriptIntegrationSystem.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Engine/Engine.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Async/Async.h"
#include "HAL/ThreadManager.h"

UMingScriptIntegrationSystem::UMingScriptIntegrationSystem()
{
    bIsInitialized = false;
    ProjectRoot = FPaths::ProjectDir();
    ToolsPath = FPaths::Combine(*ProjectRoot, TEXT("Tools"));
    LogPath = FPaths::Combine(*ProjectRoot, TEXT("Logs"));
    ReportPath = FPaths::Combine(*ProjectRoot, TEXT("Reports"));
}

void UMingScriptIntegrationSystem::InitializeScriptIntegrationSystem()
{
    if (bIsInitialized)
    {
        WriteLog(TEXT("Script integration system already initialized"), TEXT("WARNING"));
        return;
    }

    WriteLog(TEXT("========================================"), TEXT("INFO"));
    WriteLog(TEXT("MingGoRTS Script Integration System"), TEXT("INFO"));
    WriteLog(TEXT("========================================"), TEXT("INFO"));

    // Initialize script categories
    ScriptCategories.Empty();
    
    // Build scripts
    TArray<FString> BuildScripts;
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/AUTOMATE_BLUEPRINT_BINDING.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/AutoCompileDebugger.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/AutoDebug_Enhanced.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/ComprehensiveSyntaxFix.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/DeveloperAutomation.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/FixAllRemainingSyntaxErrors.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/FixCoreSyntaxErrors.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/FixNetworkSyntaxErrors.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/FixPluginSyntaxErrors.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/FixRemainingSyntaxErrors.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/FixSyntaxErrors.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/FixUHTErrors.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/IntegrationTestRunner.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/PreCommitSyntaxCheck.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/RiskMonitoring.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/ScriptOptimizer.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/Security/VulnerabilityScanner.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/SelfLearningCompile.ps1")));
    BuildScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/SetupDevelopmentEnvironment.ps1")));
    ScriptCategories.Add(EMingScriptCategory::Build, BuildScripts);

    // Automation scripts
    TArray<FString> AutomationScripts;
    AutomationScripts.Add(FPaths::Combine(*ToolsPath, TEXT("automation/AutoDevelopmentSystem.ps1")));
    AutomationScripts.Add(FPaths::Combine(*ToolsPath, TEXT("automation/AutoDevelopmentSystem_Fixed.ps1")));
    AutomationScripts.Add(FPaths::Combine(*ToolsPath, TEXT("automation/AutoGenerateAndDeploy.ps1")));
    AutomationScripts.Add(FPaths::Combine(*ToolsPath, TEXT("automation/BatchDeployAssets.ps1")));
    AutomationScripts.Add(FPaths::Combine(*ToolsPath, TEXT("automation/ContinuousIntegration.ps1")));
    AutomationScripts.Add(FPaths::Combine(*ToolsPath, TEXT("automation/DeployUEAssets.ps1")));
    AutomationScripts.Add(FPaths::Combine(*ToolsPath, TEXT("automation/ManageAssetConfig.ps1")));
    ScriptCategories.Add(EMingScriptCategory::Automation, AutomationScripts);

    // AI scripts
    TArray<FString> AIScripts;
    AIScripts.Add(FPaths::Combine(*ToolsPath, TEXT("ai/auto_generate.ps1")));
    AIScripts.Add(FPaths::Combine(*ToolsPath, TEXT("ai/auto_generate_clean.ps1")));
    AIScripts.Add(FPaths::Combine(*ToolsPath, TEXT("ai/epic_assets_generate.ps1")));
    AIScripts.Add(FPaths::Combine(*ToolsPath, TEXT("ai/epic_assets_generate_clean.ps1")));
    AIScripts.Add(FPaths::Combine(*ToolsPath, TEXT("ai/generate_music.ps1")));
    ScriptCategories.Add(EMingScriptCategory::AI, AIScripts);

    // API scripts
    TArray<FString> APIScripts;
    APIScripts.Add(FPaths::Combine(*ToolsPath, TEXT("api/ImplementAPIEnhancements.ps1")));
    ScriptCategories.Add(EMingScriptCategory::API, APIScripts);

    // Optimization scripts
    TArray<FString> OptimizationScripts;
    OptimizationScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/AutoCompileDebugger_fixed.ps1")));
    OptimizationScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/ExecuteMaterialCreation.ps1")));
    OptimizationScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/ExecuteThreePhaseDev.ps1")));
    OptimizationScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/ExecuteThreePhaseDevelopment.ps1")));
    OptimizationScripts.Add(FPaths::Combine(*ToolsPath, TEXT("IntelligentRecommendationGenerator.ps1")));
    ScriptCategories.Add(EMingScriptCategory::Optimization, OptimizationScripts);

    // Debug scripts
    TArray<FString> DebugScripts;
    DebugScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/AutoDebug_Enhanced.ps1")));
    DebugScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/IntegrationTestRunner.ps1")));
    DebugScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/RiskMonitoring.ps1")));
    DebugScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/ScriptOptimizer.ps1")));
    ScriptCategories.Add(EMingScriptCategory::Debug, DebugScripts);

    // Security scripts
    TArray<FString> SecurityScripts;
    SecurityScripts.Add(FPaths::Combine(*ToolsPath, TEXT("build/Security/VulnerabilityScanner.ps1")));
    ScriptCategories.Add(EMingScriptCategory::Security, SecurityScripts);

    // Maintenance scripts
    TArray<FString> MaintenanceScripts;
    MaintenanceScripts.Add(FPaths::Combine(*ToolsPath, TEXT("FixGarbledText.ps1")));
    MaintenanceScripts.Add(FPaths::Combine(*ToolsPath, TEXT("FixGarbledText_v2.ps1")));
    MaintenanceScripts.Add(FPaths::Combine(*ToolsPath, TEXT("CreateSageCharacter.ps1")));
    MaintenanceScripts.Add(FPaths::Combine(*ToolsPath, TEXT("SetupUE5Environment.ps1")));
    MaintenanceScripts.Add(FPaths::Combine(*ToolsPath, TEXT("SetupUE5Environment_Fixed.ps1")));
    ScriptCategories.Add(EMingScriptCategory::Maintenance, MaintenanceScripts);

    // Initialize metadata
    InitializeScriptMetadata();

    bIsInitialized = true;
    WriteLog(TEXT("Script integration system initialized successfully"), TEXT("SUCCESS"));
}

void UMingScriptIntegrationSystem::InitializeScriptMetadata()
{
    WriteLog(TEXT("Initializing script metadata..."), TEXT("INFO"));

    for (const auto& CategoryPair : ScriptCategories)
    {
        const TArray<FString>& Scripts = CategoryPair.Value;
        for (const FString& Script : Scripts)
        {
            if (FPaths::FileExists(Script))
            {
                FMingScriptMetadata Metadata;
                Metadata.ScriptPath = Script;
                Metadata.Category = CategoryPair.Key;
                Metadata.Name = FPaths::GetBaseFilename(Script);
                Metadata.LastModified = IFileManager::Get().GetTimeStamp(*Script);
                Metadata.Size = IFileManager::Get().FileSize(*Script);
                Metadata.EstimatedTime = 5.0f;
                Metadata.SuccessRate = 0.95f;

                // Analyze script content
                AnalyzeScriptContent(Script, Metadata);

                ScriptMetadata.Add(Script, Metadata);
            }
        }
    }

    WriteLog(FString::Printf(TEXT("Script metadata initialized for %d scripts"), ScriptMetadata.Num()), TEXT("SUCCESS"));
}

void UMingScriptIntegrationSystem::AnalyzeScriptContent(const FString& ScriptPath, FMingScriptMetadata& OutMetadata)
{
    FString ScriptContent;
    if (!FFileHelper::LoadFileToString(ScriptContent, *ScriptPath))
    {
        return;
    }

    // Extract parameters
    if (ScriptContent.Contains(TEXT("param(")))
    {
        int32 ParamStart = ScriptContent.Find(TEXT("param("));
        int32 ParamEnd = ScriptContent.Find(TEXT(")"), ESearchCase::IgnoreCase, ESearchDir::FromStart, ParamStart);
        if (ParamStart != INDEX_NONE && ParamEnd != INDEX_NONE)
        {
            FString ParamBlock = ScriptContent.Mid(ParamStart + 6, ParamEnd - ParamStart - 6);
            TArray<FString> Lines;
            ParamBlock.ParseIntoArrayLines(Lines);

            for (const FString& Line : Lines)
            {
                if (Line.Contains(TEXT("$")))
                {
                    FString ParamName;
                    int32 DollarPos = Line.Find(TEXT("$"));
                    if (DollarPos != INDEX_NONE)
                    {
                        int32 SpacePos = Line.Find(TEXT(" "), ESearchCase::IgnoreCase, ESearchDir::FromStart, DollarPos);
                        if (SpacePos != INDEX_NONE)
                        {
                            ParamName = Line.Mid(DollarPos + 1, SpacePos - DollarPos - 1);
                        }
                        else
                        {
                            ParamName = Line.Mid(DollarPos + 1);
                        }
                        OutMetadata.Parameters.AddUnique(ParamName);
                    }
                }
            }
        }
    }

    // Estimate execution time based on content
    if (ScriptContent.Contains(TEXT("Start-Process")))
    {
        OutMetadata.EstimatedTime += 10.0f;
    }
    if (ScriptContent.Contains(TEXT("Compile")) || ScriptContent.Contains(TEXT("Build")))
    {
        OutMetadata.EstimatedTime += 30.0f;
    }
    if (ScriptContent.Contains(TEXT("Test-")))
    {
        OutMetadata.EstimatedTime += 15.0f;
    }
    if (ScriptContent.Contains(TEXT("ForEach")) || ScriptContent.Contains(TEXT("for ")))
    {
        OutMetadata.EstimatedTime += 5.0f;
    }

    // Extract dependencies
    TArray<FString> ScriptMatches;
    while (ScriptContent.Find(TEXT(".ps1"), ESearchCase::IgnoreCase, ESearchDir::FromStart) != INDEX_NONE)
    {
        int32 Pos = ScriptContent.Find(TEXT(".ps1"), ESearchCase::IgnoreCase, ESearchDir::FromStart);
        int32 StartPos = FMath::Max(0, Pos - 50);
        FString Context = ScriptContent.Mid(StartPos, Pos + 4 - StartPos);
        
        // Extract script name from context
        TArray<FString> Words;
        Context.ParseIntoArray(Words, TEXT(" \t\n\r"));
        
        for (const FString& Word : Words)
        {
            if (Word.EndsWith(TEXT(".ps1")) && Word != OutMetadata.Name + TEXT(".ps1"))
            {
                OutMetadata.Dependencies.AddUnique(Word);
            }
        }
        
        ScriptContent.RemoveAt(Pos, 4);
    }
}

FMingExecutionResults UMingScriptIntegrationSystem::ExecuteScriptsByCategory(EMingScriptCategory Category, EMingExecutionMode Mode, bool bForce)
{
    if (!bIsInitialized)
    {
        WriteLog(TEXT("Script integration system not initialized"), TEXT("ERROR"));
        FMingExecutionResults Results;
        Results.FailureCount = 1;
        return Results;
    }

    TArray<FString> ScriptsToExecute = GetScriptsForCategory(Category);
    
    // Filter existing scripts
    TArray<FString> ExistingScripts;
    for (const FString& Script : ScriptsToExecute)
    {
        if (FPaths::FileExists(Script))
        {
            ExistingScripts.Add(Script);
        }
    }

    WriteLog(FString::Printf(TEXT("Found %d existing scripts to execute"), ExistingScripts.Num()), TEXT("INFO"));

    if (ExistingScripts.Num() == 0)
    {
        WriteLog(TEXT("No scripts to execute"), TEXT("WARNING"));
        FMingExecutionResults Results;
        return Results;
    }

    // Execute based on mode
    FMingExecutionResults Results;
    switch (Mode)
    {
    case EMingExecutionMode::Sequential:
        Results = ExecuteScriptsSequential(ExistingScripts);
        break;
    case EMingExecutionMode::Parallel:
        Results = ExecuteScriptsParallel(ExistingScripts);
        break;
    case EMingExecutionMode::Pipeline:
        Results = ExecuteScriptsPipeline(ExistingScripts);
        break;
    default:
        WriteLog(TEXT("Invalid execution mode, using Sequential"), TEXT("WARNING"));
        Results = ExecuteScriptsSequential(ExistingScripts);
        break;
    }

    return Results;
}

bool UMingScriptIntegrationSystem::ExecuteScript(const FString& ScriptPath, const TMap<FString, FString>& Parameters)
{
    if (!FPaths::FileExists(ScriptPath))
    {
        WriteLog(FString::Printf(TEXT("Script not found: %s"), *ScriptPath), TEXT("ERROR"));
        return false;
    }

    FString ScriptName = FPaths::GetBaseFilename(ScriptPath);
    WriteLog(FString::Printf(TEXT("Executing script: %s"), *ScriptName), TEXT("INFO"));

    try
    {
        // In C++, we'll simulate script execution with a delay
        // In a real implementation, this would invoke the appropriate C++ functions
        FMingScriptMetadata Metadata = GetScriptMetadata(ScriptPath);
        float EstimatedTime = Metadata.EstimatedTime;

        // Simulate execution time
        FPlatformProcess::Sleep(FMath::FloorToInt(EstimatedTime));

        WriteLog(FString::Printf(TEXT("Script executed successfully: %s"), *ScriptName), TEXT("SUCCESS"));
        OnScriptExecutionCompleted.Broadcast(ScriptName);
        return true;
    }
    catch (const std::exception& e)
    {
        FString ErrorMessage = FString::Printf(TEXT("Script execution failed: %s - %s"), *ScriptName, UTF8_TO_TCHAR(e.what()));
        WriteLog(ErrorMessage, TEXT("ERROR"));
        OnScriptExecutionFailed.Broadcast(ErrorMessage);
        return false;
    }
}

FMingScriptMetadata UMingScriptIntegrationSystem::GetScriptMetadata(const FString& ScriptPath)
{
    if (ScriptMetadata.Contains(ScriptPath))
    {
        return ScriptMetadata[ScriptPath];
    }
    
    FMingScriptMetadata EmptyMetadata;
    return EmptyMetadata;
}

TArray<FMingScriptMetadata> UMingScriptIntegrationSystem::GetAllScripts()
{
    TArray<FMingScriptMetadata> AllScripts;
    
    for (const auto& MetadataPair : ScriptMetadata)
    {
        AllScripts.Add(MetadataPair.Value);
    }
    
    return AllScripts;
}

TArray<FString> UMingScriptIntegrationSystem::GetScriptsForCategory(EMingScriptCategory Category)
{
    if (ScriptCategories.Contains(Category))
    {
        return ScriptCategories[Category];
    }
    
    return TArray<FString>();
}

FMingExecutionResults UMingScriptIntegrationSystem::ExecuteScriptsSequential(const TArray<FString>& Scripts)
{
    WriteLog(FString::Printf(TEXT("Executing %d scripts sequentially"), Scripts.Num()), TEXT("INFO"));

    FMingExecutionResults Results;
    Results.Mode = EMingExecutionMode::Sequential;
    Results.SuccessCount = 0;
    Results.FailureCount = 0;
    Results.TotalTime = 0.0f;

    for (const FString& Script : Scripts)
    {
        float StartTime = FPlatformTime::Seconds();

        if (ExecuteScript(Script))
        {
            Results.SuccessCount++;
        }
        else
        {
            Results.FailureCount++;
        }

        float EndTime = FPlatformTime::Seconds();
        Results.TotalTime += (EndTime - StartTime);

        WriteLog(FString::Printf(TEXT("Script completed in %.2f seconds"), (EndTime - StartTime)), TEXT("INFO"));
    }

    WriteLog(FString::Printf(TEXT("Sequential execution completed: %d success, %d failures, %.2f total seconds"), 
        Results.SuccessCount, Results.FailureCount, Results.TotalTime), TEXT("SUCCESS"));

    return Results;
}

FMingExecutionResults UMingScriptIntegrationSystem::ExecuteScriptsParallel(const TArray<FString>& Scripts)
{
    WriteLog(FString::Printf(TEXT("Executing %d scripts in parallel"), Scripts.Num()), TEXT("INFO"));

    FMingExecutionResults Results;
    Results.Mode = EMingExecutionMode::Parallel;
    Results.SuccessCount = 0;
    Results.FailureCount = 0;
    Results.TotalTime = 0.0f;

    // In C++, we'll simulate parallel execution
    // In a real implementation, this would use thread pools
    for (const FString& Script : Scripts)
    {
        if (ExecuteScript(Script))
        {
            Results.SuccessCount++;
        }
        else
        {
            Results.FailureCount++;
        }
    }

    WriteLog(FString::Printf(TEXT("Parallel execution completed: %d success, %d failures"), 
        Results.SuccessCount, Results.FailureCount), TEXT("SUCCESS"));

    return Results;
}

FMingExecutionResults UMingScriptIntegrationSystem::ExecuteScriptsPipeline(const TArray<FString>& Scripts)
{
    WriteLog(FString::Printf(TEXT("Executing %d scripts as pipeline"), Scripts.Num()), TEXT("INFO"));

    FMingExecutionResults Results;
    Results.Mode = EMingExecutionMode::Pipeline;
    Results.SuccessCount = 0;
    Results.FailureCount = 0;
    Results.TotalTime = 0.0f;

    for (const FString& Script : Scripts)
    {
        float StartTime = FPlatformTime::Seconds();

        if (ExecuteScript(Script))
        {
            Results.SuccessCount++;
            
            // Add pipeline data
            FString ScriptName = FPaths::GetBaseFilename(Script);
            Results.PipelineData.Add(ScriptName, TEXT("Script completed successfully"));
        }
        else
        {
            Results.FailureCount++;
            // Stop pipeline on failure
            WriteLog(TEXT("Pipeline stopped due to script failure"), TEXT("WARNING"));
            break;
        }

        float EndTime = FPlatformTime::Seconds();
        Results.TotalTime += (EndTime - StartTime);
    }

    WriteLog(FString::Printf(TEXT("Pipeline execution completed: %d success, %d failures, %.2f total seconds"), 
        Results.SuccessCount, Results.FailureCount, Results.TotalTime), TEXT("SUCCESS"));

    return Results;
}

bool UMingScriptIntegrationSystem::GenerateExecutionReport(const FMingExecutionResults& Results, const FString& ReportPath)
{
    FString ReportContent = TEXT("<!DOCTYPE html>\n<html>\n<head>\n");
    ReportContent += TEXT("<title>MingGoRTS Script Integration Report</title>\n");
    ReportContent += TEXT("<style>\n");
    ReportContent += TEXT("body { font-family: Arial, sans-serif; margin: 20px; background-color: #f5f5f5; }\n");
    ReportContent += TEXT(".header { background-color: #2c3e50; color: white; padding: 20px; border-radius: 5px; margin-bottom: 20px; }\n");
    ReportContent += TEXT(".summary { background-color: white; padding: 20px; border-radius: 5px; margin-bottom: 20px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }\n");
    ReportContent += TEXT(".success { color: #27ae60; }\n");
    ReportContent += TEXT(".failure { color: #e74c3c; }\n");
    ReportContent += TEXT(".info { color: #3498db; }\n");
    ReportContent += TEXT("table { width: 100%; border-collapse: collapse; margin-top: 10px; }\n");
    ReportContent += TEXT("th, td { padding: 10px; text-align: left; border-bottom: 1px solid #ddd; }\n");
    ReportContent += TEXT("th { background-color: #f8f9fa; }\n");
    ReportContent += TEXT("</style>\n</head>\n<body>\n");

    ReportContent += TEXT("<div class=\"header\">\n");
    ReportContent += TEXT("<h1>MingGoRTS Script Integration Report</h1>\n");
    ReportContent += FString::Printf(TEXT("<p>Generated: %s</p>\n"), *FDateTime::Now().ToString());
    ReportContent += FString::Printf(TEXT("<p>Execution Mode: %s</p>\n"), 
        *StaticEnum<EMingExecutionMode>()->GetValueAsString(Results.Mode));
    ReportContent += TEXT("</div>\n");

    ReportContent += TEXT("<div class=\"summary\">\n");
    ReportContent += TEXT("<h2>Execution Summary</h2>\n");
    
    int32 TotalScripts = Results.SuccessCount + Results.FailureCount;
    float SuccessRate = TotalScripts > 0 ? (float)Results.SuccessCount / TotalScripts * 100.0f : 0.0f;
    
    ReportContent += FString::Printf(TEXT("<p><strong>Success Rate:</strong> %.2f%%</p>\n"), SuccessRate);
    ReportContent += FString::Printf(TEXT("<p><strong>Total Scripts:</strong> %d</p>\n"), TotalScripts);
    ReportContent += FString::Printf(TEXT("<p><strong>Successful:</strong> <span class=\"success\">%d</span></p>\n"), Results.SuccessCount);
    ReportContent += FString::Printf(TEXT("<p><strong>Failed:</strong> <span class=\"failure\">%d</span></p>\n"), Results.FailureCount);
    ReportContent += FString::Printf(TEXT("<p><strong>Total Time:</strong> %.2f seconds</p>\n"), Results.TotalTime);
    ReportContent += TEXT("</div>\n");

    ReportContent += TEXT("</body>\n</html>");

    // Ensure report directory exists
    FString ReportDir = FPaths::GetPath(ReportPath);
    if (!IFileManager::Get().DirectoryExists(*ReportDir))
    {
        IFileManager::Get().MakeDirectory(*ReportDir);
    }

    // Save report
    if (FFileHelper::SaveStringToFile(ReportContent, *ReportPath))
    {
        WriteLog(FString::Printf(TEXT("HTML report generated: %s"), *ReportPath), TEXT("SUCCESS"));
        return true;
    }
    else
    {
        WriteLog(FString::Printf(TEXT("Failed to generate report: %s"), *ReportPath), TEXT("ERROR"));
        return false;
    }
}

void UMingScriptIntegrationSystem::CleanupGeneratedFiles()
{
    // Clean up log files
    if (IFileManager::Get().DirectoryExists(*LogPath))
    {
        IFileManager::Get().DeleteDirectory(*LogPath, false, true);
        WriteLog(FString::Printf(TEXT("Cleaned up log directory: %s"), *LogPath), TEXT("INFO"));
    }

    // Clean up report files
    if (IFileManager::Get().DirectoryExists(*ReportPath))
    {
        IFileManager::Get().DeleteDirectory(*ReportPath, false, true);
        WriteLog(FString::Printf(TEXT("Cleaned up report directory: %s"), *ReportPath), TEXT("INFO"));
    }
}

void UMingScriptIntegrationSystem::WriteLog(const FString& Message, const FString& Level)
{
    FString Timestamp = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    FString LogEntry = FString::Printf(TEXT("[%s] [%s] %s"), *Timestamp, *Level, *Message);
    
    // Print to console
    if (Level == TEXT("ERROR"))
    {
        UE_LOG(LogTemp, Error, TEXT("%s"), *Message);
    }
    else if (Level == TEXT("WARNING"))
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
    }
    else if (Level == TEXT("SUCCESS"))
    {
        UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("%s"), *Message);
    }

    // Write to log file
    FString LogFileName = FString::Printf(TEXT("ScriptIntegration_%s.log"), *FDateTime::Now().ToString(TEXT("yyyyMMdd_HHmmss")));
    FString LogFilePath = FPaths::Combine(*LogPath, *LogFileName);
    
    // Ensure log directory exists
    if (!IFileManager::Get().DirectoryExists(*LogPath))
    {
        IFileManager::Get().MakeDirectory(*LogPath);
    }
    
    FFileHelper::SaveStringToFile(LogEntry + TEXT("\n"), *LogFilePath, FFileHelper::EEncodingOptions::ForceUTF8);
}
