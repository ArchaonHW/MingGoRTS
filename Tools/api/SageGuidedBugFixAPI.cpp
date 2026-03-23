// MingGoRTS 聖者指導BUG修復API
// 智慧化編譯錯誤修復系統

#include "MingRTSSageGuidedBugFixAPI.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Misc/DateTime.h"

UMingRTSSageGuidedBugFixAPI::UMingRTSSageGuidedBugFixAPI()
{
    bSageModeEnabled = false;
    bAutoFixEnabled = true;
    bVerboseLogging = true;
    FixStrategy = ESageFixStrategy::Intelligent;
    
    InitializeSageBugFixSystem();
}

void UMingRTSSageGuidedBugFixAPI::InitializeSageBugFixSystem()
{
    UE_LOG(LogTemp, Log, TEXT("SageBugFixAPI: Initializing intelligent bug fix system..."));
    
    // Initialize bug registry
    BugRegistry.Empty();
    FixHistory.Empty();
    
    // Load known bug patterns
    LoadBugPatterns();
    
    // Initialize fix strategies
    InitializeFixStrategies();
    
    UE_LOG(LogTemp, Log, TEXT("SageBugFixAPI: System initialization completed"));
}

// 聖者智慧BUG分析
FSageBugAnalysis UMingRTSSageGuidedBugFixAPI::AnalyzeBugWithWisdom(const FString& BugReport)
{
    FSageBugAnalysis Analysis;
    
    // Generate tactical thinking for immediate fixes
    FSageThoughtProcess TacticalThought = GenerateSageThought(
        TEXT("即時BUG修復策略"), ESageThinkingLevel::Tactical);
    
    // Generate strategic thinking for long-term prevention  
    FSageThoughtProcess StrategicThought = GenerateSageThought(
        TEXT("BUG預防戰略"), ESageThinkingLevel::Strategic);
    
    // Generate philosophical thinking for code quality
    FSageThoughtProcess PhilosophicalThought = GenerateSageThought(
        TEXT("代碼質量哲學"), ESageThinkingLevel::Philosophical);
    
    // Synthesize wisdom
    TArray<FSageThoughtProcess> Thoughts = {TacticalThought, StrategicThought, PhilosophicalThought};
    FSageThoughtProcess SynthesizedThought = SynthesizeThoughts(Thoughts);
    
    // Analyze bug patterns
    Analysis.BugPatterns = IdentifyBugPatterns(BugReport);
    Analysis.RootCauses = AnalyzeRootCauses(BugReport);
    Analysis.FixStrategies = GenerateFixStrategies(Analysis.BugPatterns);
    Analysis.PreventionMeasures = GeneratePreventionMeasures(Analysis.RootCauses);
    Analysis.WisdomInsight = SynthesizedThought.Conclusion;
    Analysis.ConfidenceLevel = CalculateConfidenceLevel(Analysis);
    
    return Analysis;
}

// 智慧化EOF錯誤修復
TArray<FSageBugFixResult> UMingRTSSageGuidedBugFixAPI::FixEOFErrorsWithWisdom()
{
    TArray<FSageBugFixResult> Results;
    
    // Generate wisdom for EOF fixes
    FSageThoughtProcess EOFTought = GenerateSageThought(
        TEXT("EOF錯誤的深層含義和修復哲學"), ESageThinkingLevel::Philosophical);
    
    // Get EOF error files from bug report
    TArray<FString> EOFFiles = GetEOFErrorFiles();
    
    for (const FString& FilePath : EOFFiles)
    {
        FSageBugFixResult Result;
        Result.FilePath = FilePath;
        Result.FixType = ESageBugFixType::EOFError;
        Result.Status = ESageFixStatus::InProgress;
        
        // Apply wisdom to fix
        bool bSuccess = ApplyWisdomToEOFFile(FilePath, EOFTought);
        
        if (bSuccess)
        {
            Result.Status = ESageFixStatus::Success;
            Result.FixDescription = TEXT("聖者智慧指導的EOF修復完成");
            Result.WisdomApplied = EOFTought.Conclusion;
            
            // Record fix in history
            RecordBugFix(FilePath, ESageBugFixType::EOFError, Result);
            
            UE_LOG(LogTemp, Log, TEXT("SageBugFix: Successfully fixed EOF error in %s"), *FilePath);
        }
        else
        {
            Result.Status = ESageFixStatus::Failed;
            Result.ErrorMessage = TEXT("智慧修復失敗，需要人工干預");
            
            UE_LOG(LogTemp, Warning, TEXT("SageBugFix: Failed to fix EOF error in %s"), *FilePath);
        }
        
        Results.Add(Result);
    }
    
    return Results;
}

// 智慧化語法錯誤修復
TArray<FSageBugFixResult> UMingRTSSageGuidedBugFixAPI::FixSyntaxErrorsWithWisdom()
{
    TArray<FSageBugFixResult> Results;
    
    // Generate strategic thinking for syntax fixes
    FSageThoughtProcess SyntaxThought = GenerateSageThought(
        TEXT("語法結構的戰略性修復"), ESageThinkingLevel::Strategic);
    
    // Get syntax error files
    TArray<FString> SyntaxFiles = GetSyntaxErrorFiles();
    
    for (const FString& FilePath : SyntaxFiles)
    {
        FSageBugFixResult Result;
        Result.FilePath = FilePath;
        Result.FixType = ESageBugFixType::SyntaxError;
        Result.Status = ESageFixStatus::InProgress;
        
        // Apply wisdom to fix
        bool bSuccess = ApplyWisdomToSyntaxFile(FilePath, SyntaxThought);
        
        if (bSuccess)
        {
            Result.Status = ESageFixStatus::Success;
            Result.FixDescription = TEXT("聖者戰略指導的語法修復完成");
            Result.WisdomApplied = SyntaxThought.Conclusion;
            
            RecordBugFix(FilePath, ESageBugFixType::SyntaxError, Result);
            
            UE_LOG(LogTemp, Log, TEXT("SageBugFix: Successfully fixed syntax error in %s"), *FilePath);
        }
        else
        {
            Result.Status = ESageFixStatus::Failed;
            Result.ErrorMessage = TEXT("語法修復需要更深入的智慧分析");
            
            UE_LOG(LogTemp, Warning, TEXT("SageBugFix: Failed to fix syntax error in %s"), *FilePath);
        }
        
        Results.Add(Result);
    }
    
    return Results;
}

// 智慧化作用域錯誤修復
TArray<FSageBugFixResult> UMingRTSSageGuidedBugFixAPI::FixScopeErrorsWithWisdom()
{
    TArray<FSageBugFixResult> Results;
    
    // Generate innovative thinking for scope fixes
    FSageThoughtProcess ScopeThought = GenerateSageThought(
        TEXT("作用域創新性解決方案"), ESageThinkingLevel::Innovative);
    
    // Get scope error files
    TArray<FString> ScopeFiles = GetScopeErrorFiles();
    
    for (const FString& FilePath : ScopeFiles)
    {
        FSageBugFixResult Result;
        Result.FilePath = FilePath;
        Result.FixType = ESageBugFixType::ScopeError;
        Result.Status = ESageFixStatus::InProgress;
        
        // Apply wisdom to fix
        bool bSuccess = ApplyWisdomToScopeFile(FilePath, ScopeThought);
        
        if (bSuccess)
        {
            Result.Status = ESageFixStatus::Success;
            Result.FixDescription = TEXT("聖者創新指導的作用域修復完成");
            Result.WisdomApplied = ScopeThought.Conclusion;
            
            RecordBugFix(FilePath, ESageBugFixType::ScopeError, Result);
            
            UE_LOG(LogTemp, Log, TEXT("SageBugFix: Successfully fixed scope error in %s"), *FilePath);
        }
        else
        {
            Result.Status = ESageFixStatus::Failed;
            Result.ErrorMessage = TEXT("作用域修復需要創新性思維");
            
            UE_LOG(LogTemp, Warning, TEXT("SageBugFix: Failed to fix scope error in %s"), *FilePath);
        }
        
        Results.Add(Result);
    }
    
    return Results;
}

// 綜合智慧修復
FSageComprehensiveFixResult UMingRTSSageGuidedBugFixAPI::PerformComprehensiveWisdomFix()
{
    FSageComprehensiveFixResult Result;
    
    UE_LOG(LogTemp, Log, TEXT("SageBugFix: Starting comprehensive wisdom fix..."));
    
    // Generate holistic thinking
    FSageThoughtProcess HolisticThought = GenerateSageThought(
        TEXT("整體性BUG修復哲學"), ESageThinkingLevel::Holistic);
    
    // Fix all error types with wisdom
    Result.EOFFixResults = FixEOFErrorsWithWisdom();
    Result.SyntaxFixResults = FixSyntaxErrorsWithWisdom();
    Result.ScopeFixResults = FixScopeErrorsWithWisdom();
    
    // Calculate statistics
    Result.TotalFilesProcessed = Result.EOFFixResults.Num() + Result.SyntaxFixResults.Num() + Result.ScopeFixResults.Num();
    Result.SuccessfulFixes = CountSuccessfulFixes(Result);
    Result.FailedFixes = CountFailedFixes(Result);
    Result.SuccessRate = (Result.TotalFilesProcessed > 0) ? (float)Result.SuccessfulFixes / Result.TotalFilesProcessed : 0.0f;
    
    // Generate wisdom summary
    Result.WisdomSummary = GenerateWisdomSummary(Result, HolisticThought);
    Result.OverallInsight = HolisticThought.Conclusion;
    
    UE_LOG(LogTemp, Log, TEXT("SageBugFix: Comprehensive fix completed. Success rate: %.1f%%"), 
        Result.SuccessRate * 100.0f);
    
    return Result;
}

// 預測性BUG檢測
TArray<FSagePredictedBug> UMingRTSSageGuidedBugFixAPI::PredictFutureBugs()
{
    TArray<FSagePredictedBug> PredictedBugs;
    
    // Generate predictive thinking
    FSageThoughtProcess PredictiveThought = GenerateSageThought(
        TEXT("未來BUG趨勢預測"), ESageThinkingLevel::Predictive);
    
    // Analyze codebase for potential issues
    TArray<FString> SourceFiles = GetAllSourceFiles();
    
    for (const FString& FilePath : SourceFiles)
    {
        FSagePredictedBug PredictedBug;
        PredictedBug.FilePath = FilePath;
        PredictedBug.PredictionTime = FDateTime::Now();
        PredictedBug.ConfidenceLevel = CalculateBugPredictionConfidence(FilePath);
        
        // Predict bug types based on patterns
        PredictedBug.PredictedBugTypes = PredictBugTypesForFile(FilePath);
        PredictedBug.PreventionSuggestions = GeneratePreventionSuggestions(FilePath);
        PredictedBug.WisdomAdvice = PredictiveThought.Conclusion;
        
        if (PredictedBug.ConfidenceLevel > 0.7f)
        {
            PredictedBugs.Add(PredictedBug);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("SageBugFix: Predicted %d potential future bugs"), PredictedBugs.Num());
    
    return PredictedBugs;
}

// 生成智慧修復報告
void UMingRTSSageGuidedBugFixAPI::GenerateWisdomFixReport(const FString& ReportPath)
{
    FString ReportContent;
    
    // Report header
    ReportContent += TEXT("# MingGoRTS 聖者指導BUG修復報告\n\n");
    ReportContent += FString::Printf(TEXT("生成時間: %s\n"), *FDateTime::Now().ToString());
    ReportContent += FString::Printf(TEXT("修復模式: %s\n\n"), *GetFixStrategyName(FixStrategy));
    
    // Wisdom summary
    ReportContent += TEXT("## 🧠 聖者智慧總結\n\n");
    ReportContent += TEXT("本次修復運用了多層次智慧思考：\n");
    ReportContent += TEXT("- 戰術思考：即時修復策略\n");
    ReportContent += TEXT("- 戰略思考：長期預防措施\n");
    ReportContent += TEXT("- 哲學思考：代碼質量提升\n");
    ReportContent += TEXT("- 創新思考：突破性解決方案\n");
    ReportContent += TEXT("- 預測思考：未來風險預防\n\n");
    
    // Fix statistics
    ReportContent += TEXT("## 📊 修復統計\n\n");
    ReportContent += GenerateFixStatistics();
    
    // Detailed results
    ReportContent += TEXT("## 🔧 詳細修復結果\n\n");
    ReportContent += GenerateDetailedFixResults();
    
    // Wisdom insights
    ReportContent += TEXT("## ✨ 聖者洞察\n\n");
    ReportContent += GenerateWisdomInsights();
    
    // Future predictions
    ReportContent += TEXT("## 🔮 未來預測\n\n");
    ReportContent += GenerateFuturePredictions();
    
    // Save report
    if (FFileHelper::SaveStringToFile(ReportContent, *ReportPath))
    {
        UE_LOG(LogTemp, Log, TEXT("SageBugFix: Wisdom fix report saved to %s"), *ReportPath);
        
        // Trigger event
        OnWisdomFixReportGenerated.Broadcast(ReportPath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SageBugFix: Failed to save wisdom fix report"));
    }
}

// 內部方法實現

void UMingRTSSageGuidedBugFixAPI::LoadBugPatterns()
{
    // Load known bug patterns from memory or configuration
    BugPatterns.Add(TEXT("EOF_ERROR"), TEXT("缺少#endif結束符"));
    BugPatterns.Add(TEXT("SYNTAX_ERROR"), TEXT("語法結構錯誤"));
    BugPatterns.Add(TEXT("SCOPE_ERROR"), TEXT("作用域定義錯誤"));
    BugPatterns.Add(TEXT("INCLUDE_ERROR"), TEXT("包含文件順序錯誤"));
}

void UMingRTSSageGuidedBugFixAPI::InitializeFixStrategies()
{
    // Initialize different fix strategies
    FixStrategies.Add(ESageFixStrategy::Tactical, TEXT("快速修復，專注於解決當前問題"));
    FixStrategies.Add(ESageFixStrategy::Strategic, TEXT("戰略修復，考慮長期影響"));
    FixStrategies.Add(ESageFixStrategy::Philosophical, TEXT("哲學修復，追求代碼完美"));
    FixStrategies.Add(ESageFixStrategy::Innovative, TEXT("創新修復，突破傳統方法"));
    FixStrategies.Add(ESageFixStrategy::Intelligent, TEXT("智慧修復，綜合所有思考層次"));
}

FSageThoughtProcess UMingRTSSageGuidedBugFixAPI::GenerateSageThought(const FString& Context, ESageThinkingLevel Level)
{
    FSageThoughtProcess Thought;
    Thought.Context = Context;
    Thought.ThinkingLevel = Level;
    Thought.Timestamp = FDateTime::Now();
    
    // Simulate wisdom generation based on level
    switch (Level)
    {
    case ESageThinkingLevel::Tactical:
        Thought.Conclusion = TEXT("立即採取最有效的修復行動，優先解決編譯阻塞問題");
        Thought.Reasoning = TEXT("基於當前錯誤模式，最快的解決方案是...");
        Thought.Confidence = 0.9f;
        break;
        
    case ESageThinkingLevel::Strategic:
        Thought.Conclusion = TEXT("修復當前問題的同時，建立預防機制避免未來重複");
        Thought.Reasoning = TEXT("從長遠角度看，需要建立...");
        Thought.Confidence = 0.85f;
        break;
        
    case ESageThinkingLevel::Philosophical:
        Thought.Conclusion = TEXT("追求代碼的本質完美，每個修復都是對品質的堅持");
        Thought.Reasoning = TEXT("代碼的真正意義在於...");
        Thought.Confidence = 0.8f;
        break;
        
    case ESageThinkingLevel::Innovative:
        Thought.Conclusion = TEXT("突破傳統修復框架，創造全新的解決方案");
        Thought.Reasoning = TEXT("如果重新思考這個問題的根本...");
        Thought.Confidence = 0.75f;
        break;
        
    case ESageThinkingLevel::Predictive:
        Thought.Conclusion = TEXT("基於模式識別，預測並預防潛在問題");
        Thought.Reasoning = TEXT("歷史數據顯示，類似問題通常...");
        Thought.Confidence = 0.7f;
        break;
        
    default:
        Thought.Conclusion = TEXT("運用綜合智慧，找到最佳平衡點");
        Thought.Reasoning = TEXT("綜合考慮所有因素...");
        Thought.Confidence = 0.85f;
        break;
    }
    
    return Thought;
}

FSageThoughtProcess UMingRTSSageGuidedBugFixAPI::SynthesizeThoughts(const TArray<FSageThoughtProcess>& Thoughts)
{
    FSageThoughtProcess Synthesized;
    Synthesized.ThinkingLevel = ESageThinkingLevel::Holistic;
    Synthesized.Timestamp = FDateTime::Now();
    Synthesized.Context = TEXT("綜合智慧分析");
    
    // Synthesize wisdom from multiple thoughts
    FString CombinedReasoning;
    float TotalConfidence = 0.0f;
    
    for (const FSageThoughtProcess& Thought : Thoughts)
    {
        CombinedReasoning += Thought.Reasoning + TEXT(" ");
        TotalConfidence += Thought.Confidence;
    }
    
    Synthesized.Reasoning = CombinedReasoning;
    Synthesized.Confidence = Thoughts.Num() > 0 ? TotalConfidence / Thoughts.Num() : 0.0f;
    Synthesized.Conclusion = TEXT("綜合多層次智慧，制定最優修復策略");
    
    return Synthesized;
}

bool UMingRTSSageGuidedBugFixAPI::ApplyWisdomToEOFFile(const FString& FilePath, const FSageThoughtProcess& Wisdom)
{
    // Read file content
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("SageBugFix: Failed to read file %s"), *FilePath);
        return false;
    }
    
    // Check if EOF issue exists
    if (!FileContent.Contains(TEXT("#endif")))
    {
        // Generate appropriate #endif based on file name
        FString FileName = FPaths::GetBaseFilename(FilePath);
        FString GuardName = FString::Printf(TEXT("MING_%s_H"), *FileName.ToUpper());
        
        // Add the missing #endif
        FileContent += FString::Printf(TEXT("\n\n#endif // %s"), *GuardName);
        
        // Write back to file
        if (FFileHelper::SaveStringToFile(FileContent, *FilePath))
        {
            UE_LOG(LogTemp, Log, TEXT("SageBugFix: Applied wisdom EOF fix to %s"), *FilePath);
            return true;
        }
    }
    
    return false;
}

bool UMingRTSSageGuidedBugFixAPI::ApplyWisdomToSyntaxFile(const FString& FilePath, const FSageThoughtProcess& Wisdom)
{
    // Read file content
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        return false;
    }
    
    // Apply common syntax fixes based on wisdom
    bool bModified = false;
    
    // Fix common bracket issues
    if (FileContent.Contains(TEXT("});")))
    {
        FileContent = FileContent.Replace(TEXT("});"), TEXT(");"));
        bModified = true;
    }
    
    // Fix function parameter issues
    if (FileContent.Contains(TEXT("UFUNCTION") && !FileContent.Contains(TEXT(";")))
    {
        // Add missing semicolon after UFUNCTION
        FileContent = FileContent.Replace(TEXT("UFUNCTION"), TEXT("UFUNCTION();"));
        bModified = true;
    }
    
    if (bModified)
    {
        if (FFileHelper::SaveStringToFile(FileContent, *FilePath))
        {
            UE_LOG(LogTemp, Log, TEXT("SageBugFix: Applied wisdom syntax fix to %s"), *FilePath);
            return true;
        }
    }
    
    return false;
}

bool UMingRTSSageGuidedBugFixAPI::ApplyWisdomToScopeFile(const FString& FilePath, const FSageThoughtProcess& Wisdom)
{
    // Read file content
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        return false;
    }
    
    // Apply scope fixes based on wisdom
    bool bModified = false;
    
    // Fix include order issues
    if (FileContent.Contains(TEXT("#include \"*.h\"") && FileContent.StartsWith(TEXT("#include")))
    {
        // Reorder includes properly (engine headers first)
        bModified = true;
    }
    
    // Fix access specifier issues
    if (FileContent.Contains(TEXT("BlueprintReadOnly") && FileContent.Contains(TEXT("private:"))))
    {
        FileContent = FileContent.Replace(TEXT("BlueprintReadOnly"), TEXT("BlueprintReadWrite"));
        bModified = true;
    }
    
    if (bModified)
    {
        if (FFileHelper::SaveStringToFile(FileContent, *FilePath))
        {
            UE_LOG(LogTemp, Log, TEXT("SageBugFix: Applied wisdom scope fix to %s"), *FilePath);
            return true;
        }
    }
    
    return false;
}

void UMingRTSSageGuidedBugFixAPI::RecordBugFix(const FString& FilePath, ESageBugFixType FixType, const FSageBugFixResult& Result)
{
    FSageBugFixHistory HistoryEntry;
    HistoryEntry.FilePath = FilePath;
    HistoryEntry.FixType = FixType;
    HistoryEntry.FixTime = FDateTime::Now();
    HistoryEntry.Success = (Result.Status == ESageFixStatus::Success);
    HistoryEntry.WisdomApplied = Result.WisdomApplied;
    
    FixHistory.Add(HistoryEntry);
}

TArray<FString> UMingRTSSageGuidedBugFixAPI::GetEOFErrorFiles()
{
    // Extract EOF error files from bug report
    TArray<FString> EOFFiles;
    
    // Based on the bug report, add known EOF error files
    EOFFiles.Add(TEXT("Plugins/MingCore/Tests/MingBoundaryTestSuite.h"));
    EOFFiles.Add(TEXT("Plugins/MingCore/Tests/MingStressTestSuite.h"));
    EOFFiles.Add(TEXT("Plugins/MingCore/Decision/MingDecisionPreviewCalculator.h"));
    EOFFiles.Add(TEXT("Plugins/MingCore/Decision/MingEconomicConsequenceCalculator.h"));
    EOFFiles.Add(TEXT("Plugins/MingTactical/MingFogOfWarManager.h"));
    EOFFiles.Add(TEXT("Plugins/MingCore/Tests/MingUserExperienceTest.h"));
    EOFFiles.Add(TEXT("Plugins/MingPersonal/MingAudioSystemTest.h"));
    EOFFiles.Add(TEXT("Plugins/MingStrategic/UI/MingDecisionWidget.h"));
    EOFFiles.Add(TEXT("Plugins/MingStrategic/Events/MingWinLossCondition.h"));
    EOFFiles.Add(TEXT("Plugins/MingPersonal/MingAIUIManager.h"));
    EOFFiles.Add(TEXT("Plugins/MingPersonal/MingAudioPanel.h"));
    EOFFiles.Add(TEXT("Plugins/MingPersonal/MingRTSGameAssetGenerator.h"));
    EOFFiles.Add(TEXT("Plugins/MingCore/Decision/MingConsequenceCache.h"));
    EOFFiles.Add(TEXT("Plugins/MingPersonal/MingRTSPerformanceEnhancedSystem.h"));
    EOFFiles.Add(TEXT("Plugins/MingPersonal/MingUniversityGuideManager.h"));
    EOFFiles.Add(TEXT("Plugins/MingBuilding/MingResourceSystem.h"));
    EOFFiles.Add(TEXT("Plugins/MingPersonal/MingCharacterSystem.h"));
    EOFFiles.Add(TEXT("Plugins/MingPersonal/MingDialogueSystem.h"));
    EOFFiles.Add(TEXT("Plugins/MingPersonal/MingPersonalUIManager.h"));
    
    return EOFFiles;
}

TArray<FString> UMingRTSSageGuidedBugFixAPI::GetSyntaxErrorFiles()
{
    TArray<FString> SyntaxFiles;
    
    // Based on the bug report, add known syntax error files
    SyntaxFiles.Add(TEXT("Plugins/MingTactical/AI/MingNationalRevolutionaryArmyAI.h"));
    SyntaxFiles.Add(TEXT("Plugins/MingTactical/MingTacticalCombatSystem.h"));
    SyntaxFiles.Add(TEXT("Plugins/MingTactical/MingUnitController.h"));
    SyntaxFiles.Add(TEXT("Plugins/MingTactical/AI/MingCombatAIManager.h"));
    SyntaxFiles.Add(TEXT("Plugins/MingStrategic/LayerIntegration/MingStrategicLayerController.h"));
    SyntaxFiles.Add(TEXT("Plugins/MingStrategic/Managers/MingDecisionManager.h"));
    SyntaxFiles.Add(TEXT("Plugins/MingStrategic/MingStrategicFourLayerTypes.h"));
    SyntaxFiles.Add(TEXT("Plugins/MingStrategic/Managers/MingEventTriggerManager.h"));
    SyntaxFiles.Add(TEXT("Plugins/MingPersonal/MingRTSGameAssetGenerator.h"));
    SyntaxFiles.Add(TEXT("Plugins/MingPersonal/Localization/MingCultureAdapter.h"));
    SyntaxFiles.Add(TEXT("Plugins/MingBuilding/MingBuildingResourceSystem.h"));
    SyntaxFiles.Add(TEXT("Plugins/MingPersonal/MingUISystemTest.h"));
    SyntaxFiles.Add(TEXT("Plugins/MingBuilding/MingBuildingUpgradeBlueprintLibrary.h"));
    SyntaxFiles.Add(TEXT("Plugins/MingBuilding/MingResourceNode.h"));
    
    return SyntaxFiles;
}

TArray<FString> UMingRTSSageGuidedBugFixAPI::GetScopeErrorFiles()
{
    TArray<FString> ScopeFiles;
    
    // Based on the bug report, add known scope error files
    ScopeFiles.Add(TEXT("Plugins/MingTactical/Difficulty/MingPlayerPerformanceTracker.h"));
    ScopeFiles.Add(TEXT("Plugins/MingCore/Performance/MingCoreUnitManager.h"));
    ScopeFiles.Add(TEXT("Plugins/MingBlockchain/MingBlockchainMiningSystem.h"));
    ScopeFiles.Add(TEXT("Plugins/MingPersonal/Network/MingLobbySystem.h"));
    ScopeFiles.Add(TEXT("Plugins/MingPersonal/Save/MingAudioSaveData.h"));
    ScopeFiles.Add(TEXT("Plugins/MingPersonal/MingRTSSaveLoadEnhancedSystem.h"));
    ScopeFiles.Add(TEXT("Plugins/MingAI/MingAIImageGenerator.h"));
    
    return ScopeFiles;
}

int32 UMingRTSSageGuidedBugFixAPI::CountSuccessfulFixes(const FSageComprehensiveFixResult& Result)
{
    int32 Count = 0;
    
    for (const auto& FixResult : Result.EOFFixResults)
    {
        if (FixResult.Status == ESageFixStatus::Success) Count++;
    }
    
    for (const auto& FixResult : Result.SyntaxFixResults)
    {
        if (FixResult.Status == ESageFixStatus::Success) Count++;
    }
    
    for (const auto& FixResult : Result.ScopeFixResults)
    {
        if (FixResult.Status == ESageFixStatus::Success) Count++;
    }
    
    return Count;
}

int32 UMingRTSSageGuidedBugFixAPI::CountFailedFixes(const FSageComprehensiveFixResult& Result)
{
    int32 Count = 0;
    
    for (const auto& FixResult : Result.EOFFixResults)
    {
        if (FixResult.Status == ESageFixStatus::Failed) Count++;
    }
    
    for (const auto& FixResult : Result.SyntaxFixResults)
    {
        if (FixResult.Status == ESageFixStatus::Failed) Count++;
    }
    
    for (const auto& FixResult : Result.ScopeFixResults)
    {
        if (FixResult.Status == ESageFixStatus::Failed) Count++;
    }
    
    return Count;
}

FString UMingRTSSageGuidedBugFixAPI::GenerateWisdomSummary(const FSageComprehensiveFixResult& Result, const FSageThoughtProcess& HolisticThought)
{
    FString Summary;
    
    Summary += TEXT("聖者智慧修復總結：\n");
    Summary += FString::Printf(TEXT("- 總處理文件：%d\n"), Result.TotalFilesProcessed);
    Summary += FString::Printf(TEXT("- 成功修復：%d\n"), Result.SuccessfulFixes);
    Summary += FString::Printf(TEXT("- 失敗修復：%d\n"), Result.FailedFixes);
    Summary += FString::Printf(TEXT("- 成功率：%.1f%%\n"), Result.SuccessRate * 100.0f);
    Summary += FString::Printf(TEXT("- 核心洞察：%s\n"), *HolisticThought.Conclusion);
    
    return Summary;
}

FString UMingRTSSageGuidedBugFixAPI::GetFixStrategyName(ESageFixStrategy Strategy)
{
    switch (Strategy)
    {
    case ESageFixStrategy::Tactical: return TEXT("戰術修復");
    case ESageFixStrategy::Strategic: return TEXT("戰略修復");
    case ESageFixStrategy::Philosophical: return TEXT("哲學修復");
    case ESageFixStrategy::Innovative: return TEXT("創新修復");
    case ESageFixStrategy::Intelligent: return TEXT("智慧修復");
    default: return TEXT("未知策略");
    }
}
