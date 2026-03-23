#include "API/MingRTSIntelligentScriptIntegration.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Engine/Engine.h"

UMingRTSIntelligentScriptIntegration::UMingRTSIntelligentScriptIntegration()
{
    bContinuousLearningEnabled = false;
    bPredictiveExecutionEnabled = false;
    bAutoHealingEnabled = false;
    bContinuousDevelopmentMode = false;
    CurrentProfile.LearningProgress = 0.0f;
}

void UMingRTSIntelligentScriptIntegration::InitializeScriptIntegration()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Intelligent Script Integration System"));
    
    InitializeDefaultScripts();
    LoadIntegrationData();
    
    if (IntelligentAPI)
    {
        SyncWithIntelligentAPI();
    }
    
    if (AdvancedAPI)
    {
        SyncWithAdvancedAPI();
    }
    
    UE_LOG(LogTemp, Log, TEXT("Script Integration System Initialized with %d scripts"), RegisteredScripts.Num());
}

void UMingRTSIntelligentScriptIntegration::ConnectToIntelligentAPI(UMingRTSIntelligentAPIEnhancements* InIntelligenceAPI)
{
    if (InIntelligenceAPI)
    {
        IntelligentAPI = InIntelligenceAPI;
        IntelligentAPI->OnInsightGenerated.AddDynamic(this, &UMingRTSIntelligentScriptIntegration::HandleIntelligentAPIInsight);
        UE_LOG(LogTemp, Log, TEXT("Connected to Intelligent API"));
    }
}

void UMingRTSIntelligentScriptIntegration::ConnectToAdvancedAPI(UMingRTSAdvancedAPIFeatures* InAdvancedAPI)
{
    if (InAdvancedAPI)
    {
        AdvancedAPI = InAdvancedAPI;
        AdvancedAPI->OnAnomalyDetected.AddDynamic(this, &UMingRTSIntelligentScriptIntegration::HandleAdvancedAPIAnomaly);
        UE_LOG(LogTemp, Log, TEXT("Connected to Advanced API"));
    }
}

void UMingRTSIntelligentScriptIntegration::ConnectToAPISystem(UMingRTSAPISystem* InAPISystem)
{
    if (InAPISystem)
    {
        APISystem = InAPISystem;
        UE_LOG(LogTemp, Log, TEXT("Connected to API System"));
    }
}

EIntegrationStatus UMingRTSIntelligentScriptIntegration::GetIntegrationStatus() const
{
    if (!IntelligentAPI && !AdvancedAPI && !APISystem)
    {
        return EIntegrationStatus::Disconnected;
    }
    
    if (RegisteredScripts.Num() > 0)
    {
        return EIntegrationStatus::Active;
    }
    
    return EIntegrationStatus::Connected;
}

void UMingRTSIntelligentScriptIntegration::RegisterScriptFunctionality(const FScriptFunctionality& Functionality)
{
    if (!Functionality.FunctionalityID.IsEmpty())
    {
        RegisteredScripts.Add(Functionality.FunctionalityID, Functionality);
        ScriptsByCategory.FindOrAdd(Functionality.Category).Add(Functionality.FunctionalityID);
        UE_LOG(LogTemp, Log, TEXT("Registered script: %s"), *Functionality.FunctionalityID);
    }
}

void UMingRTSIntelligentScriptIntegration::UnregisterScriptFunctionality(const FString& FunctionalityID)
{
    if (FScriptFunctionality* Existing = RegisteredScripts.Find(FunctionalityID))
    {
        if (TArray<FString>* CategoryScripts = ScriptsByCategory.Find(Existing->Category))
        {
            CategoryScripts->Remove(FunctionalityID);
        }
        RegisteredScripts.Remove(FunctionalityID);
        UE_LOG(LogTemp, Log, TEXT("Unregistered script: %s"), *FunctionalityID);
    }
}

TArray<FScriptFunctionality> UMingRTSIntelligentScriptIntegration::GetRegisteredScripts() const
{
    TArray<FScriptFunctionality> Result;
    RegisteredScripts.GenerateValueArray(Result);
    return Result;
}

TArray<FScriptFunctionality> UMingRTSIntelligentScriptIntegration::GetScriptsByCategory(EScriptCategory Category) const
{
    TArray<FScriptFunctionality> Result;
    if (const TArray<FString>* ScriptIDs = ScriptsByCategory.Find(Category))
    {
        for (const FString& ID : *ScriptIDs)
        {
            if (const FScriptFunctionality* Script = RegisteredScripts.Find(ID))
            {
                Result.Add(*Script);
            }
        }
    }
    return Result;
}

FScriptFunctionality UMingRTSIntelligentScriptIntegration::GetScriptByID(const FString& FunctionalityID) const
{
    if (const FScriptFunctionality* Script = RegisteredScripts.Find(FunctionalityID))
    {
        return *Script;
    }
    return FScriptFunctionality();
}

FScriptExecutionResult UMingRTSIntelligentScriptIntegration::ExecuteScript(const FString& FunctionalityID, const FScriptExecutionContext& Context)
{
    FScriptExecutionResult Result;
    Result.ExecutionID = GenerateExecutionID();
    
    if (const FScriptFunctionality* Script = RegisteredScripts.Find(FunctionalityID))
    {
        double StartTime = FPlatformTime::Seconds();
        
        FString Command = BuildScriptCommand(Script->ScriptPath, Context.Parameters);
        FString Output = ExecutePowerShellScript(Command, Context.Parameters);
        
        Result.ExecutionTime = FPlatformTime::Seconds() - StartTime;
        Result.bSuccess = !Output.Contains(TEXT("Error")) && !Output.Contains(TEXT("Exception"));
        Result.Output = Output;
        Result.CompletedAt = FDateTime::Now();
        
        ProcessExecutionResult(FunctionalityID, Result);
        OnScriptExecutionCompleted.Broadcast(FunctionalityID, Result);
    }
    else
    {
        Result.ErrorMessage = TEXT("Script not found: ") + FunctionalityID;
        Result.bSuccess = false;
    }
    
    return Result;
}

FScriptExecutionResult UMingRTSIntelligentScriptIntegration::ExecuteScriptWithIntelligence(const FString& FunctionalityID, const FScriptExecutionContext& Context)
{
    FScriptExecutionResult Result = ExecuteScript(FunctionalityID, Context);
    
    if (bContinuousLearningEnabled)
    {
        LearnFromScriptExecution(Result);
    }
    
    if (IntelligentAPI && !Result.bSuccess)
    {
        TArray<FInsightData> Insights = IntelligentAPI->AnalyzeAPIPatterns({});
        for (const FInsightData& Insight : Insights)
        {
            if (Insight.Impact > 0.7f)
            {
                OnIntelligentFixApplied.Broadcast(Insight.Description);
            }
        }
    }
    
    return Result;
}

void UMingRTSIntelligentScriptIntegration::ExecuteScriptAsync(const FString& FunctionalityID, const FScriptExecutionContext& Context)
{
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, FunctionalityID, Context]()
    {
        FScriptExecutionResult Result = ExecuteScript(FunctionalityID, Context);
        
        AsyncTask(ENamedThreads::GameThread, [this, FunctionalityID, Result]()
        {
            OnScriptExecutionCompleted.Broadcast(FunctionalityID, Result);
        });
    });
}

FScriptExecutionResult UMingRTSIntelligentScriptIntegration::RunFullDevelopmentCycle(const FString& Target, bool bSkipBuild, bool bSkipTest, bool bSkipDeploy)
{
    FScriptExecutionContext Context;
    Context.ExecutionMode = EScriptExecutionMode::Immediate;
    Context.Parameters.Add(TEXT("Target"), Target);
    Context.Parameters.Add(TEXT("SkipBuild"), bSkipBuild ? TEXT("true") : TEXT("false"));
    Context.Parameters.Add(TEXT("SkipTest"), bSkipTest ? TEXT("true") : TEXT("false"));
    Context.Parameters.Add(TEXT("SkipDeploy"), bSkipDeploy ? TEXT("true") : TEXT("false"));
    
    return ExecuteScript(TEXT("AutoDevelopmentSystem"), Context);
}

FScriptExecutionResult UMingRTSIntelligentScriptIntegration::ExecuteCodeQualityCheck()
{
    FScriptExecutionContext Context;
    Context.ExecutionMode = EScriptExecutionMode::Immediate;
    return ExecuteScript(TEXT("CodeQualityCheck"), Context);
}

FScriptExecutionResult UMingRTSIntelligentScriptIntegration::RunEnvironmentCheck()
{
    FScriptExecutionContext Context;
    Context.ExecutionMode = EScriptExecutionMode::Immediate;
    return ExecuteScript(TEXT("EnvironmentCheck"), Context);
}

void UMingRTSIntelligentScriptIntegration::StartContinuousDevelopmentMode()
{
    bContinuousDevelopmentMode = true;
    UE_LOG(LogTemp, Log, TEXT("Continuous Development Mode Started"));
    
    if (IntelligentAPI)
    {
        FAutomationWorkflow Workflow;
        Workflow.WorkflowID = TEXT("ContinuousDev");
        Workflow.WorkflowName = TEXT("Continuous Development");
        Workflow.TriggerType = EAutomationTrigger::TimeBased;
        IntelligentAPI->CreateIntelligentWorkflow(Workflow);
    }
}

void UMingRTSIntelligentScriptIntegration::StopContinuousDevelopmentMode()
{
    bContinuousDevelopmentMode = false;
    UE_LOG(LogTemp, Log, TEXT("Continuous Development Mode Stopped"));
}

FScriptExecutionResult UMingRTSIntelligentScriptIntegration::ExecuteSelfLearningCompile(const FString& LearningMode, const FString& Strategy)
{
    FScriptExecutionContext Context;
    Context.ExecutionMode = EScriptExecutionMode::Immediate;
    Context.Parameters.Add(TEXT("LearningMode"), LearningMode);
    Context.Parameters.Add(TEXT("Strategy"), Strategy);
    Context.Parameters.Add(TEXT("AutoFix"), TEXT("true"));
    Context.Parameters.Add(TEXT("GenerateReport"), TEXT("true"));
    
    FScriptExecutionResult Result = ExecuteScript(TEXT("SelfLearningCompile"), Context);
    
    if (bContinuousLearningEnabled)
    {
        UpdateCompilationLearningData(Result);
    }
    
    return Result;
}

FScriptExecutionResult UMingRTSIntelligentScriptIntegration::AnalyzeCompilationErrors(const FString& LogContent)
{
    FScriptExecutionContext Context;
    Context.ExecutionMode = EScriptExecutionMode::Immediate;
    Context.Parameters.Add(TEXT("LogContent"), LogContent);
    Context.Parameters.Add(TEXT("AnalyzeOnly"), TEXT("true"));
    
    return ExecuteScript(TEXT("CompilationAnalyzer"), Context);
}

TArray<FString> UMingRTSIntelligentScriptIntegration::SuggestCompilationFixes(const TArray<FString>& ErrorCodes)
{
    TArray<FString> Suggestions;
    
    for (const FString& ErrorCode : ErrorCodes)
    {
        if (AdvancedAPI)
        {
            FContextualHelp Help = AdvancedAPI->GenerateContextualHelp(ErrorCode, TEXT("How to fix ") + ErrorCode);
            Suggestions.Add(Help.Answer);
        }
    }
    
    return Suggestions;
}

void UMingRTSIntelligentScriptIntegration::UpdateCompilationLearningData(const FScriptExecutionResult& Result)
{
    if (Result.bSuccess)
    {
        CurrentProfile.LearningProgress += 0.01f;
    }
    
    CurrentProfile.SuccessRates.FindOrAdd(TEXT("Compilation")) = 
        (CurrentProfile.SuccessRates.FindOrAdd(TEXT("Compilation")) * 0.9f) + (Result.bSuccess ? 0.1f : 0.0f);
    
    OnLearningDataUpdated.Broadcast(CurrentProfile);
    SaveIntegrationData();
}

float UMingRTSIntelligentScriptIntegration::GetCompilationLearningProgress() const
{
    return CurrentProfile.LearningProgress;
}

FScriptExecutionResult UMingRTSIntelligentScriptIntegration::FixGarbledTextInFiles(const TArray<FString>& FilePaths)
{
    FScriptExecutionContext Context;
    Context.ExecutionMode = EScriptExecutionMode::Immediate;
    
    FString FileList = FString::Join(FilePaths, TEXT(","));
    Context.Parameters.Add(TEXT("FilePaths"), FileList);
    
    FScriptExecutionResult Result = ExecuteScript(TEXT("FixGarbledText"), Context);
    
    if (Result.bSuccess)
    {
        OnIntelligentFixApplied.Broadcast(TEXT("Fixed garbled text in files"));
    }
    
    return Result;
}

FScriptExecutionResult UMingRTSIntelligentScriptIntegration::FixSyntaxErrors(const FString& TargetDirectory)
{
    FScriptExecutionContext Context;
    Context.ExecutionMode = EScriptExecutionMode::Immediate;
    Context.Parameters.Add(TEXT("TargetDirectory"), TargetDirectory);
    
    FScriptExecutionResult Result = ExecuteScript(TEXT("FixSyntaxErrors"), Context);
    
    if (Result.bSuccess)
    {
        OnIntelligentFixApplied.Broadcast(TEXT("Fixed syntax errors in directory: ") + TargetDirectory);
    }
    
    return Result;
}

FScriptExecutionResult UMingRTSIntelligentScriptIntegration::FixEncodingIssues(const TArray<FString>& FilePaths)
{
    FScriptExecutionContext Context;
    Context.ExecutionMode = EScriptExecutionMode::Immediate;
    
    FString FileList = FString::Join(FilePaths, TEXT(","));
    Context.Parameters.Add(TEXT("FilePaths"), FileList);
    
    return ExecuteScript(TEXT("FixEncoding"), Context);
}

void UMingRTSIntelligentScriptIntegration::EnableAutoHealingMode(bool bEnabled)
{
    bAutoHealingEnabled = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("Auto Healing Mode: %s"), bEnabled ? TEXT("Enabled") : TEXT("Disabled"));
    
    if (AdvancedAPI)
    {
        AdvancedAPI->EnableAutoHealing(bEnabled);
    }
}

TArray<FString> UMingRTSIntelligentScriptIntegration::DetectFileIssues(const FString& DirectoryPath)
{
    TArray<FString> Issues;
    
    if (AdvancedAPI)
    {
        TArray<FString> Anomalies = AdvancedAPI->DetectSystemAnomalies({});
        Issues.Append(Anomalies);
    }
    
    return Issues;
}

TArray<FString> UMingRTSIntelligentScriptIntegration::GenerateIntelligentRecommendations(const FString& Context)
{
    TArray<FString> Recommendations;
    
    if (IntelligentAPI)
    {
        TArray<FInsightData> Insights = IntelligentAPI->GenerateSystemInsights();
        for (const FInsightData& Insight : Insights)
        {
            Recommendations.Add(Insight.Title + TEXT(": ") + Insight.Description);
        }
    }
    
    if (AdvancedAPI)
    {
        TArray<FContextualHelp> HelpTopics = AdvancedAPI->GetRelevantHelpTopics(Context);
        for (const FContextualHelp& Help : HelpTopics)
        {
            Recommendations.Add(Help.Question + TEXT(": ") + Help.Answer);
        }
    }
    
    return Recommendations;
}

TArray<FString> UMingRTSIntelligentScriptIntegration::SuggestOptimizationStrategies(const TArray<FOptimizationMetric>& Metrics)
{
    TArray<FString> Strategies;
    
    if (IntelligentAPI)
    {
        FInsightData Insight = IntelligentAPI->GeneratePerformanceInsight(Metrics);
        Strategies.Append(Insight.Recommendations);
    }
    
    return Strategies;
}

void UMingRTSIntelligentScriptIntegration::ApplyRecommendedFixes(const TArray<FString>& RecommendationIDs)
{
    for (const FString& RecID : RecommendationIDs)
    {
        FScriptExecutionContext Context;
        Context.ExecutionMode = EScriptExecutionMode::Immediate;
        Context.Parameters.Add(TEXT("RecommendationID"), RecID);
        
        ExecuteScript(TEXT("ApplyFix"), Context);
    }
}

FScriptExecutionResult UMingRTSIntelligentScriptIntegration::RunContinuousIntegration()
{
    FScriptExecutionContext Context;
    Context.ExecutionMode = EScriptExecutionMode::Immediate;
    return ExecuteScript(TEXT("ContinuousIntegration"), Context);
}

FScriptExecutionResult UMingRTSIntelligentScriptIntegration::DeployAssets(const FString& TargetEnvironment)
{
    FScriptExecutionContext Context;
    Context.ExecutionMode = EScriptExecutionMode::Immediate;
    Context.Parameters.Add(TEXT("TargetEnvironment"), TargetEnvironment);
    return ExecuteScript(TEXT("DeployAssets"), Context);
}

void UMingRTSIntelligentScriptIntegration::SetupCIEnvironment()
{
    FScriptExecutionContext Context;
    Context.ExecutionMode = EScriptExecutionMode::Immediate;
    ExecuteScript(TEXT("SetupCIEnvironment"), Context);
}

TArray<FString> UMingRTSIntelligentScriptIntegration::MonitorSystemRisks()
{
    TArray<FString> Risks;
    
    if (AdvancedAPI)
    {
        TArray<FSecurityThreat> Threats = AdvancedAPI->GetActiveThreats();
        for (const FSecurityThreat& Threat : Threats)
        {
            Risks.Add(Threat.ThreatType + TEXT(": ") + Threat.Description);
        }
    }
    
    return Risks;
}

FSecurityThreat UMingRTSIntelligentScriptIntegration::DetectSecurityThreats()
{
    if (AdvancedAPI)
    {
        return AdvancedAPI->DetectSecurityThreat(TEXT("System"));
    }
    return FSecurityThreat();
}

void UMingRTSIntelligentScriptIntegration::EnableRealTimeRiskMonitoring(bool bEnabled)
{
    if (AdvancedAPI)
    {
        AdvancedAPI->EnableRealTimeAnomalyDetection(bEnabled);
    }
}

TArray<FString> UMingRTSIntelligentScriptIntegration::GetRiskAssessmentReport()
{
    return MonitorSystemRisks();
}

void UMingRTSIntelligentScriptIntegration::CreateIntelligentWorkflow(const FString& WorkflowName, const TArray<FString>& ScriptSequence)
{
    IntelligentWorkflows.Add(WorkflowName, ScriptSequence);
    UE_LOG(LogTemp, Log, TEXT("Created workflow: %s with %d steps"), *WorkflowName, ScriptSequence.Num());
}

void UMingRTSIntelligentScriptIntegration::ExecuteIntelligentWorkflow(const FString& WorkflowName)
{
    if (const TArray<FString>* Sequence = IntelligentWorkflows.Find(WorkflowName))
    {
        ActiveWorkflow = WorkflowName;
        
        for (int32 i = 0; i < Sequence->Num(); ++i)
        {
            const FString& ScriptID = (*Sequence)[i];
            FScriptExecutionContext Context;
            Context.ExecutionMode = EScriptExecutionMode::Immediate;
            
            FScriptExecutionResult Result = ExecuteScript(ScriptID, Context);
            
            FString StepDesc = FString::Printf(TEXT("Step %d/%d: %s - %s"), 
                i + 1, Sequence->Num(), *ScriptID, Result.bSuccess ? TEXT("Success") : TEXT("Failed"));
            OnWorkflowStepCompleted.Broadcast(StepDesc);
            
            if (!Result.bSuccess)
            {
                UE_LOG(LogTemp, Error, TEXT("Workflow step failed: %s"), *ScriptID);
                break;
            }
        }
        
        ActiveWorkflow.Empty();
    }
}

TArray<FString> UMingRTSIntelligentScriptIntegration::SuggestOptimalScriptSequence(const FString& Goal)
{
    TArray<FString> SuggestedSequence;
    
    if (IntelligentAPI)
    {
        FAutomationWorkflow OptimalWorkflow = IntelligentAPI->SuggestOptimalWorkflow(Goal);
        
        for (const FAPIInteraction& Step : OptimalWorkflow.Steps)
        {
            SuggestedSequence.Add(Step.Action);
        }
    }
    
    return SuggestedSequence;
}

TArray<FString> UMingRTSIntelligentScriptIntegration::PredictRequiredScripts(const FString& UpcomingTask)
{
    TArray<FString> PredictedScripts;
    
    if (IntelligentAPI)
    {
        TArray<FPredictionResult> Predictions = IntelligentAPI->PredictUserNeeds(UpcomingTask);
        
        for (const FPredictionResult& Prediction : Predictions)
        {
            if (Prediction.Confidence > 0.7f)
            {
                PredictedScripts.Add(Prediction.Prediction);
            }
        }
    }
    
    return PredictedScripts;
}

FScriptExecutionContext UMingRTSIntelligentScriptIntegration::PredictOptimalExecutionContext(const FString& FunctionalityID)
{
    FScriptExecutionContext Context;
    Context.ExecutionID = GenerateExecutionID();
    
    if (const FScriptFunctionality* Script = RegisteredScripts.Find(FunctionalityID))
    {
        Context.ExecutionMode = EScriptExecutionMode::Immediate;
        Context.Priority = 5;
        
        for (const FString& Param : Script->Parameters)
        {
            if (const FString* DefaultValue = Script->DefaultValues.Find(Param))
            {
                Context.Parameters.Add(Param, *DefaultValue);
            }
        }
    }
    
    return Context;
}

float UMingRTSIntelligentScriptIntegration::PredictScriptSuccessRate(const FString& FunctionalityID, const FScriptExecutionContext& Context)
{
    if (const FScriptFunctionality* Script = RegisteredScripts.Find(FunctionalityID))
    {
        return Script->SuccessRate;
    }
    return 0.0f;
}

void UMingRTSIntelligentScriptIntegration::EnablePredictiveScriptExecution(bool bEnabled)
{
    bPredictiveExecutionEnabled = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("Predictive Script Execution: %s"), bEnabled ? TEXT("Enabled") : TEXT("Disabled"));
    
    if (bEnabled && IntelligentAPI)
    {
        IntelligentAPI->EnablePredictiveAutomation(true);
    }
}

void UMingRTSIntelligentScriptIntegration::LearnFromScriptExecution(const FScriptExecutionResult& Result)
{
    if (!Result.bSuccess)
    {
        return;
    }
    
    CurrentProfile.LearningProgress = FMath::Min(CurrentProfile.LearningProgress + 0.01f, 1.0f);
    
    OnLearningDataUpdated.Broadcast(CurrentProfile);
    SaveIntegrationData();
}

void UMingRTSIntelligentScriptIntegration::UpdateScriptIntelligenceProfile(const FScriptIntelligenceProfile& Profile)
{
    CurrentProfile = Profile;
    SaveIntegrationData();
}

FScriptIntelligenceProfile UMingRTSIntelligentScriptIntegration::GetCurrentIntelligenceProfile() const
{
    return CurrentProfile;
}

void UMingRTSIntelligentScriptIntegration::EnableContinuousLearning(bool bEnabled)
{
    bContinuousLearningEnabled = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("Continuous Learning: %s"), bEnabled ? TEXT("Enabled") : TEXT("Disabled"));
    
    if (IntelligentAPI)
    {
        IntelligentAPI->EnableContinuousLearning(bEnabled);
    }
}

void UMingRTSIntelligentScriptIntegration::AdaptScriptParameters(const FString& FunctionalityID)
{
    if (FScriptFunctionality* Script = RegisteredScripts.Find(FunctionalityID))
    {
        OptimizeScriptParameters(FunctionalityID);
        UE_LOG(LogTemp, Log, TEXT("Adapted parameters for script: %s"), *FunctionalityID);
    }
}

FScriptExecutionResult UMingRTSIntelligentScriptIntegration::ExecuteScriptFromNaturalLanguage(const FString& NaturalLanguageCommand)
{
    FScriptExecutionResult Result;
    Result.ExecutionID = GenerateExecutionID();
    
    if (AdvancedAPI)
    {
        FString ProcessedCommand = AdvancedAPI->ProcessNaturalLanguage(NaturalLanguageCommand, TEXT("zh-TW"));
        
        TArray<FString> Keywords = AdvancedAPI->ExtractKeywords(ProcessedCommand);
        
        for (const FString& Keyword : Keywords)
        {
            for (const auto& ScriptPair : RegisteredScripts)
            {
                if (ScriptPair.Value.FunctionalityName.Contains(Keyword) || 
                    ScriptPair.Value.Description.Contains(Keyword))
                {
                    FScriptExecutionContext Context;
                    Context.ExecutionMode = EScriptExecutionMode::Immediate;
                    return ExecuteScript(ScriptPair.Key, Context);
                }
            }
        }
    }
    
    Result.ErrorMessage = TEXT("Could not find matching script for command: ") + NaturalLanguageCommand;
    Result.bSuccess = false;
    return Result;
}

FString UMingRTSIntelligentScriptIntegration::GenerateScriptDescription(const FString& FunctionalityID)
{
    if (const FScriptFunctionality* Script = RegisteredScripts.Find(FunctionalityID))
    {
        return FString::Printf(TEXT("Script: %s\nDescription: %s\nCategory: %d\nPath: %s"),
            *Script->FunctionalityName,
            *Script->Description,
            (int32)Script->Category,
            *Script->ScriptPath);
    }
    return TEXT("Script not found");
}

TArray<FString> UMingRTSIntelligentScriptIntegration::FindScriptsByDescription(const FString& DescriptionQuery)
{
    TArray<FString> MatchingScripts;
    
    for (const auto& ScriptPair : RegisteredScripts)
    {
        if (ScriptPair.Value.Description.Contains(DescriptionQuery) ||
            ScriptPair.Value.FunctionalityName.Contains(DescriptionQuery))
        {
            MatchingScripts.Add(ScriptPair.Key);
        }
    }
    
    return MatchingScripts;
}

TArray<FScriptExecutionResult> UMingRTSIntelligentScriptIntegration::GetExecutionHistory(const FDateTime& StartTime, const FDateTime& EndTime)
{
    TArray<FScriptExecutionResult> History;
    
    for (const auto& ResultPair : ExecutionHistory)
    {
        if (ResultPair.Value.CompletedAt >= StartTime && ResultPair.Value.CompletedAt <= EndTime)
        {
            History.Add(ResultPair.Value);
        }
    }
    
    return History;
}

TMap<EScriptCategory, float> UMingRTSIntelligentScriptIntegration::GetScriptCategoryPerformance()
{
    TMap<EScriptCategory, float> Performance;
    
    for (const auto& CategoryPair : ScriptsByCategory)
    {
        float TotalSuccess = 0.0f;
        int32 Count = 0;
        
        for (const FString& ScriptID : CategoryPair.Value)
        {
            if (const FScriptFunctionality* Script = RegisteredScripts.Find(ScriptID))
            {
                TotalSuccess += Script->SuccessRate;
                Count++;
            }
        }
        
        if (Count > 0)
        {
            Performance.Add(CategoryPair.Key, TotalSuccess / Count);
        }
    }
    
    return Performance;
}

void UMingRTSIntelligentScriptIntegration::GenerateIntegrationReport(const FString& ReportPath)
{
    FString Report = TEXT("=== MingRTS Intelligent Script Integration Report ===\n\n");
    
    Report += FString::Printf(TEXT("Total Registered Scripts: %d\n"), RegisteredScripts.Num());
    Report += FString::Printf(TEXT("Integration Status: %d\n"), (int32)GetIntegrationStatus());
    Report += FString::Printf(TEXT("Learning Progress: %.2f%%\n"), CurrentProfile.LearningProgress * 100);
    Report += TEXT("\n");
    
    Report += TEXT("Scripts by Category:\n");
    for (const auto& CategoryPair : ScriptsByCategory)
    {
        Report += FString::Printf(TEXT("  Category %d: %d scripts\n"), (int32)CategoryPair.Key, CategoryPair.Value.Num());
    }
    
    Report += TEXT("\nCategory Performance:\n");
    TMap<EScriptCategory, float> Performance = GetScriptCategoryPerformance();
    for (const auto& PerfPair : Performance)
    {
        Report += FString::Printf(TEXT("  Category %d: %.2f%% success rate\n"), (int32)PerfPair.Key, PerfPair.Value * 100);
    }
    
    FFileHelper::SaveStringToFile(Report, *ReportPath);
    UE_LOG(LogTemp, Log, TEXT("Integration report saved to: %s"), *ReportPath);
}

TArray<FString> UMingRTSIntelligentScriptIntegration::GetSystemHealthStatus()
{
    TArray<FString> Status;
    
    Status.Add(FString::Printf(TEXT("Registered Scripts: %d"), RegisteredScripts.Num()));
    Status.Add(FString::Printf(TEXT("Intelligent API Connected: %s"), IntelligentAPI ? TEXT("Yes") : TEXT("No")));
    Status.Add(FString::Printf(TEXT("Advanced API Connected: %s"), AdvancedAPI ? TEXT("Yes") : TEXT("No")));
    Status.Add(FString::Printf(TEXT("API System Connected: %s"), APISystem ? TEXT("Yes") : TEXT("No")));
    Status.Add(FString::Printf(TEXT("Continuous Learning: %s"), bContinuousLearningEnabled ? TEXT("Enabled") : TEXT("Disabled")));
    Status.Add(FString::Printf(TEXT("Predictive Execution: %s"), bPredictiveExecutionEnabled ? TEXT("Enabled") : TEXT("Disabled")));
    Status.Add(FString::Printf(TEXT("Auto Healing: %s"), bAutoHealingEnabled ? TEXT("Enabled") : TEXT("Disabled")));
    Status.Add(FString::Printf(TEXT("Learning Progress: %.2f%%"), CurrentProfile.LearningProgress * 100));
    
    return Status;
}

// ========== 內部方法實現 ==========

void UMingRTSIntelligentScriptIntegration::InitializeDefaultScripts()
{
    RegisterBuildScripts();
    RegisterAutomationScripts();
    RegisterAIScripts();
    RegisterOptimizationScripts();
    RegisterDebugScripts();
    RegisterDeploymentScripts();
    RegisterTestingScripts();
    RegisterMaintenanceScripts();
}

void UMingRTSIntelligentScriptIntegration::RegisterBuildScripts()
{
    FScriptFunctionality SelfLearningCompile;
    SelfLearningCompile.FunctionalityID = TEXT("SelfLearningCompile");
    SelfLearningCompile.FunctionalityName = TEXT("Self-Learning Compilation");
    SelfLearningCompile.ScriptPath = GetScriptDirectoryPath() + TEXT("/build/SelfLearningCompile.ps1");
    SelfLearningCompile.Category = EScriptCategory::Build;
    SelfLearningCompile.Parameters = { TEXT("LearningMode"), TEXT("Strategy"), TEXT("AutoFix"), TEXT("GenerateReport") };
    SelfLearningCompile.DefaultValues.Add(TEXT("LearningMode"), TEXT("Advanced"));
    SelfLearningCompile.DefaultValues.Add(TEXT("Strategy"), TEXT("Adaptive"));
    SelfLearningCompile.DefaultValues.Add(TEXT("AutoFix"), TEXT("true"));
    SelfLearningCompile.DefaultValues.Add(TEXT("GenerateReport"), TEXT("true"));
    SelfLearningCompile.Description = TEXT("自學習編譯系統，根據歷史錯誤模式自動優化編譯過程");
    RegisterScriptFunctionality(SelfLearningCompile);
}

void UMingRTSIntelligentScriptIntegration::RegisterAutomationScripts()
{
    FScriptFunctionality AutoDevelopment;
    AutoDevelopment.FunctionalityID = TEXT("AutoDevelopmentSystem");
    AutoDevelopment.FunctionalityName = TEXT("Automated Development System");
    AutoDevelopment.ScriptPath = GetScriptDirectoryPath() + TEXT("/automation/AutoDevelopmentSystem.ps1");
    AutoDevelopment.Category = EScriptCategory::Automation;
    AutoDevelopment.Parameters = { TEXT("Mode"), TEXT("Target"), TEXT("SkipBuild"), TEXT("SkipTest"), TEXT("SkipDeploy") };
    AutoDevelopment.DefaultValues.Add(TEXT("Mode"), TEXT("Full"));
    AutoDevelopment.DefaultValues.Add(TEXT("Target"), TEXT("Development"));
    AutoDevelopment.Description = TEXT("完整自動化開發工作流程，包含構建、測試、部署");
    RegisterScriptFunctionality(AutoDevelopment);
    
    FScriptFunctionality ContinuousIntegration;
    ContinuousIntegration.FunctionalityID = TEXT("ContinuousIntegration");
    ContinuousIntegration.FunctionalityName = TEXT("Continuous Integration");
    ContinuousIntegration.ScriptPath = GetScriptDirectoryPath() + TEXT("/automation/ContinuousIntegration.ps1");
    ContinuousIntegration.Category = EScriptCategory::Automation;
    ContinuousIntegration.Description = TEXT("持續整合系統，自動化構建和測試流程");
    RegisterScriptFunctionality(ContinuousIntegration);
}

void UMingRTSIntelligentScriptIntegration::RegisterAIScripts()
{
    FScriptFunctionality IntelligentRecommendations;
    IntelligentRecommendations.FunctionalityID = TEXT("IntelligentRecommendations");
    IntelligentRecommendations.FunctionalityName = TEXT("Intelligent Recommendation Generator");
    IntelligentRecommendations.ScriptPath = GetScriptDirectoryPath() + TEXT("/IntelligentRecommendationGenerator.ps1");
    IntelligentRecommendations.Category = EScriptCategory::AI;
    IntelligentRecommendations.Description = TEXT("基於AI的智能推薦生成器，提供開發建議");
    RegisterScriptFunctionality(IntelligentRecommendations);
    
    FScriptFunctionality AutoGenerate;
    AutoGenerate.FunctionalityID = TEXT("AutoGenerate");
    AutoGenerate.FunctionalityName = TEXT("Auto Content Generation");
    AutoGenerate.ScriptPath = GetScriptDirectoryPath() + TEXT("/ai/auto_generate.ps1");
    AutoGenerate.Category = EScriptCategory::AI;
    AutoGenerate.Description = TEXT("AI自動內容生成腳本");
    RegisterScriptFunctionality(AutoGenerate);
}

void UMingRTSIntelligentScriptIntegration::RegisterOptimizationScripts()
{
    FScriptFunctionality ScriptOptimizer;
    ScriptOptimizer.FunctionalityID = TEXT("ScriptOptimizer");
    ScriptOptimizer.FunctionalityName = TEXT("Script Optimizer");
    ScriptOptimizer.ScriptPath = GetScriptDirectoryPath() + TEXT("/build/ScriptOptimizer.ps1");
    ScriptOptimizer.Category = EScriptCategory::Optimization;
    ScriptOptimizer.Description = TEXT("腳本性能優化工具");
    RegisterScriptFunctionality(ScriptOptimizer);
}

void UMingRTSIntelligentScriptIntegration::RegisterDebugScripts()
{
    FScriptFunctionality AutoCompileDebugger;
    AutoCompileDebugger.FunctionalityID = TEXT("AutoCompileDebugger");
    AutoCompileDebugger.FunctionalityName = TEXT("Auto Compilation Debugger");
    AutoCompileDebugger.ScriptPath = GetScriptDirectoryPath() + TEXT("/build/AutoCompileDebugger.ps1");
    AutoCompileDebugger.Category = EScriptCategory::Debug;
    AutoCompileDebugger.Description = TEXT("自動編譯錯誤檢測和修復");
    RegisterScriptFunctionality(AutoCompileDebugger);
    
    FScriptFunctionality FixSyntaxErrors;
    FixSyntaxErrors.FunctionalityID = TEXT("FixSyntaxErrors");
    FixSyntaxErrors.FunctionalityName = TEXT("Fix Syntax Errors");
    FixSyntaxErrors.ScriptPath = GetScriptDirectoryPath() + TEXT("/build/FixSyntaxErrors.ps1");
    FixSyntaxErrors.Category = EScriptCategory::Debug;
    FixSyntaxErrors.Parameters = { TEXT("TargetDirectory") };
    FixSyntaxErrors.Description = TEXT("自動修復語法錯誤");
    RegisterScriptFunctionality(FixSyntaxErrors);
    
    FScriptFunctionality FixGarbledText;
    FixGarbledText.FunctionalityID = TEXT("FixGarbledText");
    FixGarbledText.FunctionalityName = TEXT("Fix Garbled Text");
    FixGarbledText.ScriptPath = GetScriptDirectoryPath() + TEXT("/FixGarbledText.ps1");
    FixGarbledText.Category = EScriptCategory::Debug;
    FixGarbledText.Parameters = { TEXT("FilePaths") };
    FixGarbledText.Description = TEXT("修復文件亂碼問題");
    RegisterScriptFunctionality(FixGarbledText);
    
    FScriptFunctionality FixEncoding;
    FixEncoding.FunctionalityID = TEXT("FixEncoding");
    FixEncoding.FunctionalityName = TEXT("Fix Encoding Issues");
    FixEncoding.ScriptPath = GetScriptDirectoryPath() + TEXT("/build/FixEncoding.ps1");
    FixEncoding.Category = EScriptCategory::Debug;
    FixEncoding.Parameters = { TEXT("FilePaths") };
    FixEncoding.Description = TEXT("修復文件編碼問題");
    RegisterScriptFunctionality(FixEncoding);
}

void UMingRTSIntelligentScriptIntegration::RegisterDeploymentScripts()
{
    FScriptFunctionality DeployAssets;
    DeployAssets.FunctionalityID = TEXT("DeployAssets");
    DeployAssets.FunctionalityName = TEXT("Deploy Assets");
    DeployAssets.ScriptPath = GetScriptDirectoryPath() + TEXT("/automation/DeployUEAssets.ps1");
    DeployAssets.Category = EScriptCategory::Deployment;
    DeployAssets.Parameters = { TEXT("TargetEnvironment") };
    DeployAssets.Description = TEXT("UE資源自動化部署");
    RegisterScriptFunctionality(DeployAssets);
    
    FScriptFunctionality BatchDeploy;
    BatchDeploy.FunctionalityID = TEXT("BatchDeployAssets");
    BatchDeploy.FunctionalityName = TEXT("Batch Deploy Assets");
    BatchDeploy.ScriptPath = GetScriptDirectoryPath() + TEXT("/automation/BatchDeployAssets.ps1");
    BatchDeploy.Category = EScriptCategory::Deployment;
    BatchDeploy.Description = TEXT("批量資產部署");
    RegisterScriptFunctionality(BatchDeploy);
}

void UMingRTSIntelligentScriptIntegration::RegisterTestingScripts()
{
    FScriptFunctionality IntegrationTests;
    IntegrationTests.FunctionalityID = TEXT("IntegrationTests");
    IntegrationTests.FunctionalityName = TEXT("Integration Test Runner");
    IntegrationTests.ScriptPath = GetScriptDirectoryPath() + TEXT("/build/IntegrationTestRunner.ps1");
    IntegrationTests.Category = EScriptCategory::Testing;
    IntegrationTests.Description = TEXT("整合測試執行器");
    RegisterScriptFunctionality(IntegrationTests);
}

void UMingRTSIntelligentScriptIntegration::RegisterMaintenanceScripts()
{
    FScriptFunctionality RiskMonitoring;
    RiskMonitoring.FunctionalityID = TEXT("RiskMonitoring");
    RiskMonitoring.FunctionalityName = TEXT("Risk Monitoring");
    RiskMonitoring.ScriptPath = GetScriptDirectoryPath() + TEXT("/build/RiskMonitoring.ps1");
    RiskMonitoring.Category = EScriptCategory::Maintenance;
    RiskMonitoring.Description = TEXT("系統風險監控");
    RegisterScriptFunctionality(RiskMonitoring);
}

FString UMingRTSIntelligentScriptIntegration::ExecutePowerShellScript(const FString& Command, const TMap<FString, FString>& Parameters)
{
    FString FullCommand = Command;
    
    for (const auto& Param : Parameters)
    {
        FullCommand += FString::Printf(TEXT(" -%s \"%s\""), *Param.Key, *Param.Value);
    }
    
    int32 ReturnCode = 0;
    FString StdOut;
    FString StdErr;
    
    FPlatformProcess::ExecProcess(TEXT("powershell.exe"), 
        *FString::Printf(TEXT("-ExecutionPolicy Bypass -Command \"%s\""), *FullCommand),
        &ReturnCode, &StdOut, &StdErr,
        GetProjectRootPath());
    
    if (ReturnCode != 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Script execution failed: %s"), *StdErr);
        return FString::Printf(TEXT("Error: %s\nOutput: %s"), *StdErr, *StdOut);
    }
    
    return StdOut;
}

bool UMingRTSIntelligentScriptIntegration::ValidateScriptPath(const FString& ScriptPath)
{
    return FPlatformFileManager::Get().GetPlatformFile().FileExists(*ScriptPath);
}

FString UMingRTSIntelligentScriptIntegration::BuildScriptCommand(const FString& ScriptPath, const TMap<FString, FString>& Parameters)
{
    return ScriptPath;
}

void UMingRTSIntelligentScriptIntegration::ProcessExecutionResult(const FString& FunctionalityID, const FScriptExecutionResult& Result)
{
    ExecutionHistory.Add(Result.ExecutionID, Result);
    
    if (FScriptFunctionality* Script = RegisteredScripts.Find(FunctionalityID))
    {
        Script->ExecutionCount++;
        
        float NewSuccessRate = ((Script->SuccessRate * (Script->ExecutionCount - 1)) + (Result.bSuccess ? 1.0f : 0.0f)) / Script->ExecutionCount;
        Script->SuccessRate = NewSuccessRate;
        
        if (Script->ExecutionCount > 1)
        {
            Script->AverageExecutionTime = ((Script->AverageExecutionTime * (Script->ExecutionCount - 1)) + Result.ExecutionTime) / Script->ExecutionCount;
        }
        else
        {
            Script->AverageExecutionTime = Result.ExecutionTime;
        }
    }
    
    if (bContinuousLearningEnabled)
    {
        LearnFromScriptExecution(Result);
    }
}

void UMingRTSIntelligentScriptIntegration::UpdateLearningData(const FString& FunctionalityID, bool bSuccess, float ExecutionTime)
{
    if (FScriptIntelligenceProfile* Profile = &CurrentProfile)
    {
        Profile->ExecutionPatterns.FindOrAdd(FunctionalityID) = ExecutionTime;
        
        float CurrentRate = Profile->SuccessRates.FindOrAdd(FunctionalityID);
        Profile->SuccessRates.Add(FunctionalityID, (CurrentRate * 0.9f) + (bSuccess ? 0.1f : 0.0f));
        
        if (bSuccess && !Profile->FrequentlyUsedScripts.Contains(FunctionalityID))
        {
            Profile->FrequentlyUsedScripts.Add(FunctionalityID);
        }
    }
}

void UMingRTSIntelligentScriptIntegration::TriggerPredictiveExecution()
{
    if (!bPredictiveExecutionEnabled || !IntelligentAPI)
    {
        return;
    }
    
    TArray<FString> PredictedScripts = PredictRequiredScripts(TEXT("Current Context"));
    
    for (const FString& ScriptID : PredictedScripts)
    {
        FScriptExecutionContext Context = PredictOptimalExecutionContext(ScriptID);
        OnPredictiveExecutionTriggered.Broadcast(ScriptID, Context);
        
        ExecuteScriptAsync(ScriptID, Context);
    }
}

TArray<FString> UMingRTSIntelligentScriptIntegration::AnalyzeExecutionPatterns()
{
    TArray<FString> Patterns;
    
    for (const auto& HistoryPair : ExecutionHistory)
    {
        if (HistoryPair.Value.ExecutionTime > 10.0f)
        {
            Patterns.Add(HistoryPair.Key + TEXT(": Long execution time"));
        }
    }
    
    return Patterns;
}

void UMingRTSIntelligentScriptIntegration::OptimizeScriptParameters(const FString& FunctionalityID)
{
    if (FScriptFunctionality* Script = RegisteredScripts.Find(FunctionalityID))
    {
        UE_LOG(LogTemp, Log, TEXT("Optimizing parameters for: %s"), *FunctionalityID);
    }
}

FString UMingRTSIntelligentScriptIntegration::GenerateScriptRecommendation(const FString& Context)
{
    if (IntelligentAPI)
    {
        TArray<FPredictionResult> Predictions = IntelligentAPI->PredictUserNeeds(Context);
        if (Predictions.Num() > 0)
        {
            return Predictions[0].Recommendations.Num() > 0 ? Predictions[0].Recommendations[0] : TEXT("No specific recommendation");
        }
    }
    return TEXT("Use AutoDevelopmentSystem for general tasks");
}

void UMingRTSIntelligentScriptIntegration::SyncWithIntelligentAPI()
{
    if (!IntelligentAPI)
    {
        return;
    }
    
    IntelligentAPI->InitializeIntelligenceSystem();
    UE_LOG(LogTemp, Log, TEXT("Synchronized with Intelligent API"));
}

void UMingRTSIntelligentScriptIntegration::SyncWithAdvancedAPI()
{
    if (!AdvancedAPI)
    {
        return;
    }
    
    AdvancedAPI->EnableAutoHealing(bAutoHealingEnabled);
    UE_LOG(LogTemp, Log, TEXT("Synchronized with Advanced API"));
}

void UMingRTSIntelligentScriptIntegration::HandleIntelligentAPIInsight(const FInsightData& Insight)
{
    UE_LOG(LogTemp, Log, TEXT("Received insight: %s - %s"), *Insight.Title, *Insight.Description);
    
    if (Insight.Impact > 0.8f)
    {
        OnIntelligentFixApplied.Broadcast(Insight.Recommendations.Num() > 0 ? Insight.Recommendations[0] : Insight.Description);
    }
}

void UMingRTSIntelligentScriptIntegration::HandleAdvancedAPIAnomaly(const FString& AnomalyDescription)
{
    UE_LOG(LogTemp, Warning, TEXT("Anomaly detected: %s"), *AnomalyDescription);
    
    if (bAutoHealingEnabled)
    {
        TArray<FString> MatchingScripts = FindScriptsByDescription(TEXT("Fix"));
        if (MatchingScripts.Num() > 0)
        {
            FScriptExecutionContext Context;
            Context.ExecutionMode = EScriptExecutionMode::Immediate;
            ExecuteScriptAsync(MatchingScripts[0], Context);
        }
    }
}

void UMingRTSIntelligentScriptIntegration::SaveIntegrationData()
{
    FString SavePath = GetProjectRootPath() + TEXT("/Config/ScriptIntegrationData.json");
    
    FString JsonData = TEXT("{\n");
    JsonData += FString::Printf(TEXT("  \"LearningProgress\": %.2f,\n"), CurrentProfile.LearningProgress);
    JsonData += FString::Printf(TEXT("  \"ContinuousLearning\": %s,\n"), bContinuousLearningEnabled ? TEXT("true") : TEXT("false"));
    JsonData += FString::Printf(TEXT("  \"PredictiveExecution\": %s,\n"), bPredictiveExecutionEnabled ? TEXT("true") : TEXT("false"));
    JsonData += FString::Printf(TEXT("  \"AutoHealing\": %s,\n"), bAutoHealingEnabled ? TEXT("true") : TEXT("false"));
    JsonData += TEXT("  \"RegisteredScripts\": [\n");
    
    bool bFirst = true;
    for (const auto& ScriptPair : RegisteredScripts)
    {
        if (!bFirst) JsonData += TEXT(",\n");
        bFirst = false;
        JsonData += FString::Printf(TEXT("    \"%s\""), *ScriptPair.Key);
    }
    JsonData += TEXT("\n  ]\n}");
    
    FFileHelper::SaveStringToFile(JsonData, *SavePath);
}

void UMingRTSIntelligentScriptIntegration::LoadIntegrationData()
{
    FString LoadPath = GetProjectRootPath() + TEXT("/Config/ScriptIntegrationData.json");
    
    FString JsonData;
    if (FFileHelper::LoadFileToString(JsonData, *LoadPath))
    {
        UE_LOG(LogTemp, Log, TEXT("Loaded integration data from: %s"), *LoadPath);
    }
}

FString UMingRTSIntelligentScriptIntegration::GenerateExecutionID()
{
    return FGuid::NewGuid().ToString(EGuidFormats::Digits);
}

FString UMingRTSIntelligentScriptIntegration::GetProjectRootPath() const
{
    return FPaths::ProjectDir();
}

FString UMingRTSIntelligentScriptIntegration::GetScriptDirectoryPath() const
{
    return GetProjectRootPath() + TEXT("Tools");
}

FAPICoordinationResult UMingRTSIntelligentScriptIntegration::CoordinateScriptsWithAPIs(const TArray<FString>& ScriptIDs, const TArray<EAPIType>& APIs, const FString& Task)
{
    FAPICoordinationResult Result;
    Result.CoordinationId = GenerateExecutionID();
    
    if (APISystem)
    {
        Result = APISystem->CoordinateAPIs(APIs, Task);
    }
    
    for (const FString& ScriptID : ScriptIDs)
    {
        FScriptExecutionContext Context;
        Context.ExecutionMode = EScriptExecutionMode::Immediate;
        FScriptExecutionResult ScriptResult = ExecuteScript(ScriptID, Context);
        
        Result.bSuccess = Result.bSuccess && ScriptResult.bSuccess;
        Result.Results.Add(ScriptResult.Output);
    }
    
    return Result;
}
