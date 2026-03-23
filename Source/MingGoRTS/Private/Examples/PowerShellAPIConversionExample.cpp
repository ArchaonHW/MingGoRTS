// Copyright (c) 2026 MingGoRTS. All rights reserved.
// PowerShell to C++ API Conversion Example Implementation

#include "Examples/PowerShellAPIConversionExample.h"

APowerShellAPIConversionExample::APowerShellAPIConversionExample()
{
    PrimaryActorTick.bCanEverTick = false;
}

void APowerShellAPIConversionExample::BeginPlay()
{
    Super::BeginPlay();

    // Initialize API system
    APISystem = NewObject<UMingRTSAPISystem>();
    if (APISystem)
    {
        APISystem->InitializeAPISystem();
        
        // Get individual APIs
        LearningAPI = APISystem->GetUE5LearningAPI();
        CodeGeneratorAPI = APISystem->GetPotatoAICodeGeneratorAPI();
    }
}

// ===== UE5 Learning API Examples =====

void APowerShellAPIConversionExample::Example_StartUE5Learning()
{
    if (!LearningAPI)
    {
        UE_LOG(LogTemp, Warning, TEXT("Learning API not initialized"));
        return;
    }

    // Configure learning settings
    FUE5LearningConfig Config;
    Config.LearningMode = EUE5LearningMode::Comprehensive;
    Config.DocumentationURL = TEXT("https://dev.epicgames.com/documentation/en-us/unreal-engine/unreal-engine-5-7-documentation");
    Config.OutputPath = TEXT("C:/HW/MingGoRTS/Docs/UE5_Learning");
    Config.CachePath = TEXT("C:/HW/MingGoRTS/Cache/UE5_Docs");
    Config.KnowledgeBasePath = TEXT("C:/HW/MingGoRTS/Knowledge/UE5_5.7");
    Config.bInteractive = true;
    Config.bVerbose = true;

    // Initialize and start learning
    if (LearningAPI->Initialize(Config))
    {
        // Bind to events
        LearningAPI->OnLearningCompleted.AddDynamic(this, &APowerShellAPIConversionExample::OnLearningCompleted);
        LearningAPI->OnLearningProgress.AddDynamic(this, &APowerShellAPIConversionExample::OnLearningProgress);

        // Start comprehensive learning
        bool bSuccess = LearningAPI->StartComprehensiveLearning();
        
        UE_LOG(LogTemp, Log, TEXT("UE5 Learning started: %s"), bSuccess ? TEXT("Success") : TEXT("Failed"));
    }
}

void APowerShellAPIConversionExample::Example_FetchDocumentation()
{
    if (!LearningAPI)
    {
        UE_LOG(LogTemp, Warning, TEXT("Learning API not initialized"));
        return;
    }

    // Fetch documentation for specific sections
    TArray<EUE5DocSection> SectionsToFetch = {
        EUE5DocSection::GettingStarted,
        EUE5DocSection::Programming,
        EUE5DocSection::Gameplay,
        EUE5DocSection::AI
    };

    for (EUE5DocSection Section : SectionsToFetch)
    {
        FUE5DocumentationContent Content = LearningAPI->FetchDocumentation(Section);
        
        UE_LOG(LogTemp, Log, TEXT("Fetched: %s - Title: %s"), 
            *Content.Section, 
            *Content.Title);
        
        UE_LOG(LogTemp, Log, TEXT("APIs found: %d"), Content.APIs.Num());
    }
}

void APowerShellAPIConversionExample::Example_GetAPIPatterns()
{
    if (!LearningAPI)
    {
        UE_LOG(LogTemp, Warning, TEXT("Learning API not initialized"));
        return;
    }

    // Get all learned API patterns
    TArray<FUE5APIPattern> Patterns = LearningAPI->GetAllAPIPatterns();
    
    UE_LOG(LogTemp, Log, TEXT("Total API Patterns learned: %d"), Patterns.Num());
    
    for (const FUE5APIPattern& Pattern : Patterns)
    {
        UE_LOG(LogTemp, Log, TEXT("API: %s - Section: %s"), 
            *Pattern.APIName, 
            *Pattern.Section);
        
        // Get usage example
        FString Usage = LearningAPI->GetUsageExample(Pattern.APIName);
        UE_LOG(LogTemp, Log, TEXT("Usage: %s"), *Usage);
    }
}

void APowerShellAPIConversionExample::Example_GenerateLearningReport()
{
    if (!LearningAPI)
    {
        UE_LOG(LogTemp, Warning, TEXT("Learning API not initialized"));
        return;
    }

    // Generate different types of reports
    FUE5LearningReport QuickReport = LearningAPI->GenerateQuickReport();
    UE_LOG(LogTemp, Log, TEXT("Quick Report: %s - Concepts: %d"), 
        *QuickReport.ReportPath, 
        QuickReport.LearnedConceptsCount);

    FUE5LearningReport ComprehensiveReport = LearningAPI->GenerateComprehensiveReport();
    UE_LOG(LogTemp, Log, TEXT("Comprehensive Report: %s - Concepts: %d"), 
        *ComprehensiveReport.ReportPath, 
        ComprehensiveReport.LearnedConceptsCount);
}

// ===== Potato AI Code Generator Examples =====

void APowerShellAPIConversionExample::Example_GenerateUE5Class()
{
    if (!CodeGeneratorAPI)
    {
        UE_LOG(LogTemp, Warning, TEXT("Code Generator API not initialized"));
        return;
    }

    // Set up generation options
    FCodeGenerationOptions Options;
    Options.bAddComments = true;
    Options.bAddErrorHandling = true;
    Options.bOptimizeForBlueprint = true;
    Options.bAddLogging = true;

    // Generate a UE5 class for a player character
    FString Requirement = TEXT("Create a player character class with movement and combat abilities");
    FGeneratedCodeResult Result = CodeGeneratorAPI->GenerateUE5Class(Requirement, Options);

    UE_LOG(LogTemp, Log, TEXT("Generated Class Quality Score: %d/100"), Result.QualityScore);
    UE_LOG(LogTemp, Log, TEXT("Template Type: %s"), 
        *UEnum::GetValueAsString(Result.TemplateType));

    // Display suggestions
    for (const FString& Suggestion : Result.Suggestions)
    {
        UE_LOG(LogTemp, Log, TEXT("Suggestion: %s"), *Suggestion);
    }

    // Export to file
    FString FilePath = TEXT("C:/HW/MingGoRTS/Generated/PlayerCharacter.h");
    if (CodeGeneratorAPI->ExportCodeToFile(Result, FilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("Code exported to: %s"), *FilePath);
    }
}

void APowerShellAPIConversionExample::Example_GenerateUE5Function()
{
    if (!CodeGeneratorAPI)
    {
        UE_LOG(LogTemp, Warning, TEXT("Code Generator API not initialized"));
        return;
    }

    FCodeGenerationOptions Options;
    Options.bAddComments = true;
    Options.bAddErrorHandling = true;

    // Generate a function for calculating damage
    FString Requirement = TEXT("Calculate combat damage based on weapon stats and player level");
    FGeneratedCodeResult Result = CodeGeneratorAPI->GenerateUE5Function(Requirement, Options);

    UE_LOG(LogTemp, Log, TEXT("Generated Function - Quality: %d/100"), Result.QualityScore);
    
    // Check code quality
    FCodeQualityReport QualityReport = CodeGeneratorAPI->CheckCodeQuality(Result.CodeContent);
    UE_LOG(LogTemp, Log, TEXT("Quality Check - Overall Score: %d"), QualityReport.OverallScore);
}

void APowerShellAPIConversionExample::Example_GenerateUE5System()
{
    if (!CodeGeneratorAPI)
    {
        UE_LOG(LogTemp, Warning, TEXT("Code Generator API not initialized"));
        return;
    }

    FCodeGenerationOptions Options;
    Options.bAddComments = true;
    Options.bAddErrorHandling = true;
    Options.bOptimizeForBlueprint = true;

    // Generate an audio system
    FString Requirement = TEXT("Create an audio management system with background music and SFX support");
    FGeneratedCodeResult Result = CodeGeneratorAPI->GenerateUE5System(Requirement, Options);

    UE_LOG(LogTemp, Log, TEXT("Generated System - Quality: %d/100"), Result.QualityScore);
    
    // Copy to clipboard for immediate use
    CodeGeneratorAPI->ExportToClipboard(Result);
    UE_LOG(LogTemp, Log, TEXT("System code copied to clipboard"));
}

void APowerShellAPIConversionExample::Example_BatchCodeGeneration()
{
    if (!CodeGeneratorAPI)
    {
        UE_LOG(LogTemp, Warning, TEXT("Code Generator API not initialized"));
        return;
    }

    // Define multiple requirements for batch generation
    TArray<FString> Requirements = {
        TEXT("Create enemy character class"),
        TEXT("Create weapon item class"),
        TEXT("Create inventory manager"),
        TEXT("Create quest system"),
        TEXT("Create dialogue manager")
    };

    // Generate all in batch
    TArray<FGeneratedCodeResult> Results = CodeGeneratorAPI->BatchGenerateCode(
        Requirements, 
        ECodeTemplateType::UE5_Class);

    UE_LOG(LogTemp, Log, TEXT("Batch generation completed: %d items"), Results.Num());

    int32 TotalQuality = 0;
    for (const FGeneratedCodeResult& Result : Results)
    {
        TotalQuality += Result.QualityScore;
        UE_LOG(LogTemp, Log, TEXT("Generated: %s - Quality: %d"), 
            *Result.Requirement, 
            Result.QualityScore);
    }

    float AverageQuality = (float)TotalQuality / Results.Num();
    UE_LOG(LogTemp, Log, TEXT("Average Quality Score: %.1f"), AverageQuality);
}

void APowerShellAPIConversionExample::Example_CheckCodeQuality()
{
    if (!CodeGeneratorAPI)
    {
        UE_LOG(LogTemp, Warning, TEXT("Code Generator API not initialized"));
        return;
    }

    // Generate some code to check
    FString Requirement = TEXT("Create save game system with compression");
    FGeneratedCodeResult Result = CodeGeneratorAPI->GenerateUE5Class(Requirement, FCodeGenerationOptions());

    // Perform quality check
    FCodeQualityReport QualityReport = CodeGeneratorAPI->CheckCodeQuality(Result.CodeContent);

    UE_LOG(LogTemp, Log, TEXT("=== Code Quality Report ==="));
    UE_LOG(LogTemp, Log, TEXT("Overall Score: %d/100"), QualityReport.OverallScore);
    UE_LOG(LogTemp, Log, TEXT("Has Includes: %s"), QualityReport.bHasIncludes ? TEXT("Yes") : TEXT("No"));
    UE_LOG(LogTemp, Log, TEXT("Has Class Definition: %s"), QualityReport.bHasClassDefinition ? TEXT("Yes") : TEXT("No"));
    UE_LOG(LogTemp, Log, TEXT("Has Functions: %s"), QualityReport.bHasFunctions ? TEXT("Yes") : TEXT("No"));
    UE_LOG(LogTemp, Log, TEXT("Has UE5 Macros: %s"), QualityReport.bHasUE5Macros ? TEXT("Yes") : TEXT("No"));
    UE_LOG(LogTemp, Log, TEXT("Has Comments: %s"), QualityReport.bHasComments ? TEXT("Yes") : TEXT("No"));

    if (QualityReport.Recommendations.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("=== Recommendations ==="));
        for (const FString& Rec : QualityReport.Recommendations)
        {
            UE_LOG(LogTemp, Log, TEXT("- %s"), *Rec);
        }
    }
}

// ===== API System Coordination Examples =====

void APowerShellAPIConversionExample::Example_CoordinateLearningAndGeneration()
{
    if (!APISystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("API System not initialized"));
        return;
    }

    // Coordinate UE5 Learning and Code Generation APIs
    TArray<EAPIType> APIsToCoordinate = {
        EAPIType::UE5Learning,
        EAPIType::PotatoAICodeGen,
        EAPIType::CodeStandards
    };

    FString Task = TEXT("Learn UE5 documentation patterns and generate optimized code based on best practices");

    FAPICoordinationResult Result = APISystem->CoordinateAPIs(APIsToCoordinate, Task);

    UE_LOG(LogTemp, Log, TEXT("Coordination Result: %s"), Result.bSuccess ? TEXT("Success") : TEXT("Failed"));
    UE_LOG(LogTemp, Log, TEXT("Total Time: %.2f seconds"), Result.TotalTime);
    UE_LOG(LogTemp, Log, TEXT("Interactions: %d"), Result.Interactions.Num());

    if (Result.Recommendations.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("=== Recommendations ==="));
        for (const FString& Rec : Result.Recommendations)
        {
            UE_LOG(LogTemp, Log, TEXT("- %s"), *Rec);
        }
    }
}

void APowerShellAPIConversionExample::Example_ExecuteIntelligentWorkflow()
{
    if (!APISystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("API System not initialized"));
        return;
    }

    // Let the API system intelligently determine which APIs to use
    FString Task = TEXT("Generate code for a complete RTS unit system");
    
    FAPICoordinationResult Result = APISystem->ExecuteIntelligentTask(Task);

    UE_LOG(LogTemp, Log, TEXT("Intelligent Task Execution: %s"), Result.bSuccess ? TEXT("Success") : TEXT("Failed"));
    UE_LOG(LogTemp, Log, TEXT("Coordination ID: %s"), *Result.CoordinationId);

    // Display all interactions
    for (const FAPIInteraction& Interaction : Result.Interactions)
    {
        UE_LOG(LogTemp, Log, TEXT("Interaction: %s -> %s (%s)"),
            *UEnum::GetValueAsString(Interaction.SourceAPI),
            *UEnum::GetValueAsString(Interaction.TargetAPI),
            *Interaction.Action);
    }
}

// ===== Event Handlers =====

void APowerShellAPIConversionExample::OnLearningCompleted(const FUE5LearningReport& Report)
{
    UE_LOG(LogTemp, Log, TEXT("=== Learning Completed ==="));
    UE_LOG(LogTemp, Log, TEXT("Report Type: %s"), *Report.ReportType);
    UE_LOG(LogTemp, Log, TEXT("Learned Concepts: %d"), Report.LearnedConceptsCount);
    UE_LOG(LogTemp, Log, TEXT("API Patterns: %d"), Report.APIPatternsCount);
    UE_LOG(LogTemp, Log, TEXT("Report Path: %s"), *Report.ReportPath);

    // Now generate code based on learned patterns
    if (CodeGeneratorAPI && Report.APIPatternsCount > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Auto-generating code templates based on learned APIs..."));
        
        for (const FString& Section : Report.LearnedSections)
        {
            FString Requirement = FString::Printf(TEXT("Create %s system"), *Section);
            FGeneratedCodeResult CodeResult = CodeGeneratorAPI->GenerateUE5Class(Requirement, FCodeGenerationOptions());
            
            UE_LOG(LogTemp, Log, TEXT("Generated: %s (Quality: %d)"), 
                *Requirement, 
                CodeResult.QualityScore);
        }
    }
}

void APowerShellAPIConversionExample::OnCodeGenerated(const FGeneratedCodeResult& Result, int32 QualityScore)
{
    UE_LOG(LogTemp, Log, TEXT("=== Code Generated ==="));
    UE_LOG(LogTemp, Log, TEXT("Template: %s"), *UEnum::GetValueAsString(Result.TemplateType));
    UE_LOG(LogTemp, Log, TEXT("Quality Score: %d/100"), QualityScore);
    UE_LOG(LogTemp, Log, TEXT("Generated At: %s"), *Result.GeneratedAt);
}

void APowerShellAPIConversionExample::OnLearningProgress(const FString& Section, float Progress)
{
    UE_LOG(LogTemp, Log, TEXT("Learning Progress: %s - %.1f%%"), *Section, Progress);
}
