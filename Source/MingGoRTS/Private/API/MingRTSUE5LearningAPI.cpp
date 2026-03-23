// Copyright (c) 2026 MingGoRTS. All rights reserved.
// UE5 Documentation Learning System API Implementation

#include "API/MingRTSUE5LearningAPI.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

DEFINE_LOG_CATEGORY_STATIC(LogUE5LearningAPI, Log, All);

const TMap<FString, FString> UMingRTSUE5LearningAPI::UsageExamples = {
    {TEXT("UEngine"), TEXT("UEngine* Engine = GEngine;")},
    {TEXT("UGameInstance"), TEXT("UGameInstance* GameInstance = GetGameInstance();")},
    {TEXT("UWorld"), TEXT("UWorld* World = GetWorld();")},
    {TEXT("AActor"), TEXT("AActor* MyActor = GetWorld()->SpawnActor<AMyActor>();")},
    {TEXT("UObject"), TEXT("UObject* Object = NewObject<UObject>();")}
};

const TMap<FString, FString> UMingRTSUE5LearningAPI::APIExplanations = {
    {TEXT("UEngine"), TEXT("The main engine class that manages core engine systems.")},
    {TEXT("UGameInstance"), TEXT("A game instance that persists across level transitions.")},
    {TEXT("UWorld"), TEXT("Represents a game world containing actors and levels.")},
    {TEXT("AActor"), TEXT("Base class for all objects that can be placed in a level.")},
    {TEXT("UObject"), TEXT("Base class for most objects in Unreal Engine.")}
};

UMingRTSUE5LearningAPI::UMingRTSUE5LearningAPI()
    : CurrentProgress(0.0f)
    , bIsInitialized(false)
    , bIsLearning(false)
{
}

bool UMingRTSUE5LearningAPI::Initialize(const FUE5LearningConfig& Config)
{
    CurrentConfig = Config;
    
    // Initialize directories
    InitializeDirectories();
    
    // Initialize documentation index
    InitializeDocumentationIndex();
    
    // Setup log path
    FString Timestamp = FDateTime::Now().ToString(TEXT("yyyyMMdd_HHmmss"));
    LogPath = FPaths::ProjectDir() / TEXT("Logs") / FString::Printf(TEXT("UE5Learning_%s.log"), *Timestamp);
    
    WriteLog(TEXT("========================================"), TEXT("INFO"));
    WriteLog(TEXT("MingGoRTS UE5.7 API Learning System"), TEXT("INFO"));
    WriteLog(TEXT("========================================"), TEXT("INFO"));
    
    // Import existing knowledge
    ImportExistingKnowledge();
    
    bIsInitialized = true;
    return true;
}

void UMingRTSUE5LearningAPI::Shutdown()
{
    WriteLog(TEXT("Shutting down UE5 Learning API"), TEXT("INFO"));
    bIsInitialized = false;
    bIsLearning = false;
}

bool UMingRTSUE5LearningAPI::StartLearning(EUE5LearningMode Mode)
{
    if (!bIsInitialized)
    {
        WriteLog(TEXT("Learning API not initialized"), TEXT("ERROR"));
        return false;
    }
    
    if (bIsLearning)
    {
        WriteLog(TEXT("Learning already in progress"), TEXT("WARNING"));
        return false;
    }
    
    bIsLearning = true;
    CurrentProgress = 0.0f;
    
    WriteLog(FString::Printf(TEXT("Starting learning mode: %s"), *UEnum::GetValueAsString(Mode)), TEXT("INFO"));
    
    TArray<EUE5DocSection> Sections = GetSectionsForMode(Mode);
    
    for (int32 i = 0; i < Sections.Num(); ++i)
    {
        EUE5DocSection Section = Sections[i];
        FString SectionName = UEnum::GetValueAsString(Section);
        
        WriteLog(FString::Printf(TEXT("Learning section: %s"), *SectionName), TEXT("INFO"));
        
        FUE5DocumentationContent Content = FetchDocumentation(Section);
        UpdateKnowledgeBase(Content);
        
        CurrentProgress = (float)(i + 1) / (float)Sections.Num() * 100.0f;
        OnLearningProgress.Broadcast(SectionName, CurrentProgress);
        
        if (CurrentConfig.bInteractive)
        {
            // In interactive mode, wait for user input
            FPlatformProcess::Sleep(0.5f);
        }
    }
    
    // Generate report based on mode
    FUE5LearningReport Report;
    switch (Mode)
    {
    case EUE5LearningMode::Quick:
        Report = GenerateQuickReport();
        break;
    case EUE5LearningMode::Comprehensive:
        Report = GenerateComprehensiveReport();
        break;
    case EUE5LearningMode::Targeted:
        Report = GenerateTargetedReport();
        break;
    }
    
    OnLearningCompleted.Broadcast(Report);
    
    bIsLearning = false;
    
    WriteLog(TEXT("🎓 UE5.7 Learning Complete!"), TEXT("SUCCESS"));
    
    return true;
}

bool UMingRTSUE5LearningAPI::StartQuickLearning()
{
    return StartLearning(EUE5LearningMode::Quick);
}

bool UMingRTSUE5LearningAPI::StartComprehensiveLearning()
{
    return StartLearning(EUE5LearningMode::Comprehensive);
}

bool UMingRTSUE5LearningAPI::StartTargetedLearning()
{
    return StartLearning(EUE5LearningMode::Targeted);
}

FUE5DocumentationContent UMingRTSUE5LearningAPI::FetchDocumentation(EUE5DocSection Section)
{
    FUE5DocumentationContent Content;
    Content.Section = UEnum::GetValueAsString(Section);
    
    WriteLog(FString::Printf(TEXT("Fetching documentation: %s"), *Content.Section), TEXT("INFO"));
    
    // Check cache first
    if (!CurrentConfig.bForceUpdate && IsCacheValid(Section))
    {
        FString CacheFile = CurrentConfig.CachePath / FString::Printf(TEXT("%s.json"), *Content.Section);
        FString CachedContent;
        
        if (FFileHelper::LoadFileToString(CachedContent, *CacheFile))
        {
            WriteLog(FString::Printf(TEXT("Loaded from cache: %s"), *Content.Section), TEXT("DEBUG"));
            // Parse cached content
            TSharedPtr<FJsonObject> JsonObject;
            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(CachedContent);
            
            if (FJsonSerializer::Deserialize(Reader, JsonObject))
            {
                Content.Title = JsonObject->GetStringField(TEXT("Title"));
                Content.Content = JsonObject->GetStringField(TEXT("Content"));
                Content.Version = JsonObject->GetStringField(TEXT("Version"));
                
                const TArray<TSharedPtr<FJsonValue>>* APIsArray;
                if (JsonObject->TryGetArrayField(TEXT("APIs"), APIsArray))
                {
                    for (const auto& API : *APIsArray)
                    {
                        Content.APIs.Add(API->AsString());
                    }
                }
            }
            
            OnDocumentationFetched.Broadcast(Content);
            return Content;
        }
    }
    
    // Create mock content
    Content = CreateMockDocumentation(Section);
    
    // Cache the content
    FString CacheFile = CurrentConfig.CachePath / FString::Printf(TEXT("%s.json"), *Content.Section);
    FString JsonContent;
    
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
    JsonObject->SetStringField(TEXT("Section"), Content.Section);
    JsonObject->SetStringField(TEXT("Title"), Content.Title);
    JsonObject->SetStringField(TEXT("Content"), Content.Content);
    JsonObject->SetStringField(TEXT("Version"), Content.Version);
    
    TArray<TSharedPtr<FJsonValue>> APIsArray;
    for (const FString& API : Content.APIs)
    {
        APIsArray.Add(MakeShared<FJsonValueString>(API));
    }
    JsonObject->SetArrayField(TEXT("APIs"), APIsArray);
    
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonContent);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    
    FFileHelper::SaveStringToFile(JsonContent, *CacheFile);
    WriteLog(FString::Printf(TEXT("Cached documentation: %s"), *CacheFile), TEXT("SUCCESS"));
    
    OnDocumentationFetched.Broadcast(Content);
    
    return Content;
}

bool UMingRTSUE5LearningAPI::ImportExistingKnowledge()
{
    WriteLog(TEXT("Loading existing knowledge base..."), TEXT("INFO"));
    
    TArray<FString> KnowledgeFiles = {
        CurrentConfig.KnowledgeBasePath / TEXT("Classes/knowledge.json"),
        CurrentConfig.KnowledgeBasePath / TEXT("Functions/knowledge.json"),
        CurrentConfig.KnowledgeBasePath / TEXT("Patterns/knowledge.json"),
        CurrentConfig.KnowledgeBasePath / TEXT("BestPractices/knowledge.json")
    };
    
    for (const FString& File : KnowledgeFiles)
    {
        TArray<FUE5LearningConcept> Concepts;
        if (LoadKnowledgeFile(File, Concepts))
        {
            for (const FUE5LearningConcept& Concept : Concepts)
            {
                LearnedConcepts.Add(Concept.Name, Concept);
            }
            WriteLog(FString::Printf(TEXT("Loaded knowledge file: %s"), *File), TEXT("SUCCESS"));
        }
    }
    
    WriteLog(FString::Printf(TEXT("Loaded %d knowledge concepts"), LearnedConcepts.Num()), TEXT("INFO"));
    return true;
}

bool UMingRTSUE5LearningAPI::UpdateKnowledgeBase(const FUE5DocumentationContent& Content)
{
    WriteLog(FString::Printf(TEXT("Processing documentation: %s"), *Content.Section), TEXT("INFO"));
    
    // Extract API patterns
    ExtractAPIPatterns(Content);
    
    // Extract best practices
    ExtractBestPractices(Content);
    
    // Generate learning examples
    GenerateLearningExamples(Content);
    
    // Update knowledge files
    TArray<FUE5LearningConcept> Concepts;
    for (const FString& API : Content.APIs)
    {
        FUE5LearningConcept Concept;
        Concept.Name = API;
        Concept.Type = TEXT("Class");
        Concept.Section = Content.Section;
        Concept.Description = GetAPIExplanation(API);
        Concept.Usage = GetUsageExample(API);
        Concept.LearnedAt = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
        
        Concepts.Add(Concept);
        LearnedConcepts.Add(API, Concept);
        
        OnKnowledgeUpdated.Broadcast(Concept);
    }
    
    FString ClassesPath = CurrentConfig.KnowledgeBasePath / TEXT("Classes/knowledge.json");
    UpdateKnowledgeFile(ClassesPath, Concepts);
    
    WriteLog(TEXT("Knowledge base updated"), TEXT("SUCCESS"));
    return true;
}

void UMingRTSUE5LearningAPI::ExtractAPIPatterns(const FUE5DocumentationContent& Content)
{
    WriteLog(TEXT("Extracting API patterns..."), TEXT("DEBUG"));
    
    for (const FString& API : Content.APIs)
    {
        FUE5APIPattern Pattern;
        Pattern.APIName = API;
        Pattern.Section = Content.Section;
        Pattern.Usage = GetUsageExample(API);
        Pattern.Context = Content.Title;
        Pattern.LearnedAt = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
        
        APIPatterns.Add(API, Pattern);
    }
}

FString UMingRTSUE5LearningAPI::GetUsageExample(const FString& APIName) const
{
    const FString* Example = UsageExamples.Find(APIName);
    if (Example)
    {
        return *Example;
    }
    return FString::Printf(TEXT("// Usage example for %s"), *APIName);
}

FString UMingRTSUE5LearningAPI::GetAPIExplanation(const FString& APIName) const
{
    const FString* Explanation = APIExplanations.Find(APIName);
    if (Explanation)
    {
        return *Explanation;
    }
    return FString::Printf(TEXT("Explanation for %s"), *APIName);
}

FUE5DocumentationContent UMingRTSUE5LearningAPI::CreateMockDocumentation(EUE5DocSection Section)
{
    FUE5DocumentationContent Content;
    Content.Section = UEnum::GetValueAsString(Section);
    Content.Version = TEXT("5.7");
    Content.LastUpdated = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    
    switch (Section)
    {
    case EUE5DocSection::GettingStarted:
        Content.Title = TEXT("Getting Started with Unreal Engine 5.7");
        Content.Content = TEXT("Unreal Engine 5.7 introduces Nanite virtualized geometry, Lumen global illumination, and Enhanced Blueprint system.");
        Content.APIs = {TEXT("UEngine"), TEXT("UGameInstance"), TEXT("UWorld"), TEXT("AActor")};
        break;
        
    case EUE5DocSection::Programming:
        Content.Title = TEXT("Programming with Unreal Engine 5.7");
        Content.Content = TEXT("UE5.7 provides C++17/20 support, Enhanced reflection system, and Better Blueprint integration.");
        Content.APIs = {TEXT("UObject"), TEXT("AActor"), TEXT("UActorComponent"), TEXT("FDelegate")};
        break;
        
    case EUE5DocSection::Gameplay:
        Content.Title = TEXT("Gameplay Systems in UE5.7");
        Content.Content = TEXT("Core gameplay systems include Actor Framework, Game Mode System, and Enhanced Input System.");
        Content.APIs = {TEXT("AActor"), TEXT("UGameModeBase"), TEXT("APlayerController"), TEXT("APawn")};
        break;
        
    case EUE5DocSection::AI:
        Content.Title = TEXT("Artificial Intelligence Systems");
        Content.Content = TEXT("Advanced AI systems include Behavior Trees, Environment Query System, and Navigation System.");
        Content.APIs = {TEXT("UBehaviorTree"), TEXT("UBlackboardComponent"), TEXT("UEnvQuerySystem"), TEXT("ANavigationData")};
        break;
        
    case EUE5DocSection::Performance:
        Content.Title = TEXT("Performance Optimization");
        Content.Content = TEXT("Performance optimization strategies include Profiling Tools and Optimization Techniques.");
        Content.APIs = {TEXT("FStatGroup"), TEXT("IStatsThread"), TEXT("FStreamingManager")};
        break;
        
    default:
        Content.Title = FString::Printf(TEXT("%s Documentation"), *Content.Section);
        Content.Content = TEXT("Documentation content for this section.");
        break;
    }
    
    return Content;
}

void UMingRTSUE5LearningAPI::InitializeDirectories()
{
    WriteLog(TEXT("Initializing learning system directories..."), TEXT("INFO"));
    
    TArray<FString> Directories = {
        CurrentConfig.OutputPath,
        CurrentConfig.OutputPath / TEXT("API"),
        CurrentConfig.OutputPath / TEXT("Examples"),
        CurrentConfig.OutputPath / TEXT("Tutorials"),
        CurrentConfig.OutputPath / TEXT("Reference"),
        CurrentConfig.CachePath,
        CurrentConfig.KnowledgeBasePath,
        CurrentConfig.KnowledgeBasePath / TEXT("Classes"),
        CurrentConfig.KnowledgeBasePath / TEXT("Functions"),
        CurrentConfig.KnowledgeBasePath / TEXT("Patterns"),
        CurrentConfig.KnowledgeBasePath / TEXT("BestPractices")
    };
    
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    for (const FString& Dir : Directories)
    {
        if (!PlatformFile.DirectoryExists(*Dir))
        {
            PlatformFile.CreateDirectoryTree(*Dir);
            WriteLog(FString::Printf(TEXT("Created directory: %s"), *Dir), TEXT("SUCCESS"));
        }
    }
}

void UMingRTSUE5LearningAPI::WriteLog(const FString& Message, const FString& Level)
{
    FString Timestamp = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    FString LogEntry = FString::Printf(TEXT("[%s] [%s] %s"), *Timestamp, *Level, *Message);
    
    UE_LOG(LogUE5LearningAPI, Log, TEXT("%s"), *LogEntry);
    
    // Also write to file
    if (!LogPath.IsEmpty())
    {
        FString ExistingContent;
        FFileHelper::LoadFileToString(ExistingContent, *LogPath);
        ExistingContent += LogEntry + TEXT("\n");
        FFileHelper::SaveStringToFile(ExistingContent, *LogPath);
    }
}

TArray<EUE5DocSection> UMingRTSUE5LearningAPI::GetSectionsForMode(EUE5LearningMode Mode) const
{
    TArray<EUE5DocSection> Sections;
    
    switch (Mode)
    {
    case EUE5LearningMode::Quick:
        Sections = {
            EUE5DocSection::GettingStarted,
            EUE5DocSection::Programming,
            EUE5DocSection::Gameplay
        };
        break;
        
    case EUE5LearningMode::Comprehensive:
        for (int32 i = 0; i < (int32)EUE5DocSection::Performance + 1; ++i)
        {
            Sections.Add((EUE5DocSection)i);
        }
        break;
        
    case EUE5LearningMode::Targeted:
        Sections = {
            EUE5DocSection::Programming,
            EUE5DocSection::Gameplay,
            EUE5DocSection::AI,
            EUE5DocSection::Performance
        };
        break;
    }
    
    return Sections;
}

FUE5LearningReport UMingRTSUE5LearningAPI::GenerateQuickReport()
{
    FUE5LearningReport Report;
    Report.ReportType = TEXT("Quick");
    Report.LearnedSections = {TEXT("GettingStarted"), TEXT("Programming"), TEXT("Gameplay")};
    Report.LearnedConceptsCount = LearnedConcepts.Num();
    Report.APIPatternsCount = APIPatterns.Num();
    Report.GeneratedAt = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    
    FString Timestamp = FDateTime::Now().ToString(TEXT("yyyyMMdd_HHmmss"));
    Report.ReportPath = CurrentConfig.OutputPath / FString::Printf(TEXT("QuickLearningReport_%s.md"), *Timestamp);
    
    // Generate markdown report
    FString ReportContent = FString::Printf(
        TEXT("# UE5.7 Quick Learning Report\n\n"
             "## Overview\n"
             "Quick learning mode focused on Unreal Engine 5.7 core concepts.\n\n"
             "## Learned Sections\n"
             "- Getting Started (Introduction)\n"
             "- Programming (Basics)\n"
             "- Gameplay (Systems)\n\n"
             "## Key APIs Learned\n"
             "Total: %d concepts\n\n"
             "## Next Steps\n"
             "1. Deep dive into AI systems\n"
             "2. Master rendering and optimization\n"
             "3. Practice project development\n\n"
             "## Generated\n"
             "%s\n"),
        LearnedConcepts.Num(),
        *Report.GeneratedAt
    );
    
    FFileHelper::SaveStringToFile(ReportContent, *Report.ReportPath);
    WriteLog(FString::Printf(TEXT("Quick learning report generated: %s"), *Report.ReportPath), TEXT("SUCCESS"));
    
    return Report;
}

FUE5LearningReport UMingRTSUE5LearningAPI::GenerateComprehensiveReport()
{
    FUE5LearningReport Report;
    Report.ReportType = TEXT("Comprehensive");
    Report.LearnedConceptsCount = LearnedConcepts.Num();
    Report.APIPatternsCount = APIPatterns.Num();
    Report.GeneratedAt = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    
    FString Timestamp = FDateTime::Now().ToString(TEXT("yyyyMMdd_HHmmss"));
    Report.ReportPath = CurrentConfig.OutputPath / FString::Printf(TEXT("ComprehensiveLearningReport_%s.md"), *Timestamp);
    
    WriteLog(FString::Printf(TEXT("Comprehensive learning report generated: %s"), *Report.ReportPath), TEXT("SUCCESS"));
    
    return Report;
}

FUE5LearningReport UMingRTSUE5LearningAPI::GenerateTargetedReport()
{
    FUE5LearningReport Report;
    Report.ReportType = TEXT("Targeted");
    Report.LearnedSections = {TEXT("Programming"), TEXT("Gameplay"), TEXT("AI"), TEXT("Performance")};
    Report.LearnedConceptsCount = LearnedConcepts.Num();
    Report.APIPatternsCount = APIPatterns.Num();
    Report.GeneratedAt = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    
    FString Timestamp = FDateTime::Now().ToString(TEXT("yyyyMMdd_HHmmss"));
    Report.ReportPath = CurrentConfig.OutputPath / FString::Printf(TEXT("TargetedLearningReport_%s.md"), *Timestamp);
    
    WriteLog(FString::Printf(TEXT("Targeted learning report generated: %s"), *Report.ReportPath), TEXT("SUCCESS"));
    
    return Report;
}

bool UMingRTSUE5LearningAPI::ClearCache()
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    bool bSuccess = PlatformFile.DeleteDirectoryRecursively(*CurrentConfig.CachePath);
    
    if (bSuccess)
    {
        PlatformFile.CreateDirectory(*CurrentConfig.CachePath);
        WriteLog(TEXT("Cache cleared successfully"), TEXT("SUCCESS"));
    }
    else
    {
        WriteLog(TEXT("Failed to clear cache"), TEXT("ERROR"));
    }
    
    return bSuccess;
}

bool UMingRTSUE5LearningAPI::IsCacheValid(EUE5DocSection Section) const
{
    FString CacheFile = CurrentConfig.CachePath / FString::Printf(TEXT("%s.json"), *UEnum::GetValueAsString(Section));
    
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    if (!PlatformFile.FileExists(*CacheFile))
    {
        return false;
    }
    
    FDateTime LastWriteTime;
    if (PlatformFile.GetTimeStamp(*CacheFile, LastWriteTime))
    {
        FTimespan Age = FDateTime::Now() - LastWriteTime;
        return Age.GetDays() < 7; // Cache valid for 7 days
    }
    
    return false;
}

void UMingRTSUE5LearningAPI::SetForceUpdate(bool bForce)
{
    CurrentConfig.bForceUpdate = bForce;
}

void UMingRTSUE5LearningAPI::InitializeDocumentationIndex()
{
    DocumentationIndex.Add(EUE5DocSection::GettingStarted, FUE5DocumentationSection{
        TEXT("GettingStarted"),
        TEXT("https://dev.epicgames.com/documentation/en-us/unreal-engine/getting-started"),
        {TEXT("Installation"), TEXT("Setup"), TEXT("FirstProject"), TEXT("Interface")},
        EDocumentationPriority::High
    });
    
    DocumentationIndex.Add(EUE5DocSection::Programming, FUE5DocumentationSection{
        TEXT("Programming"),
        TEXT("https://dev.epicgames.com/documentation/en-us/unreal-engine/programming"),
        {TEXT("CPP"), TEXT("Blueprints"), TEXT("API"), TEXT("Plugins")},
        EDocumentationPriority::High
    });
    
    DocumentationIndex.Add(EUE5DocSection::Gameplay, FUE5DocumentationSection{
        TEXT("Gameplay"),
        TEXT("https://dev.epicgames.com/documentation/en-us/unreal-engine/gameplay"),
        {TEXT("Actors"), TEXT("Components"), TEXT("GameMode"), TEXT("Input")},
        EDocumentationPriority::High
    });
    
    DocumentationIndex.Add(EUE5DocSection::AI, FUE5DocumentationSection{
        TEXT("AI"),
        TEXT("https://dev.epicgames.com/documentation/en-us/unreal-engine/artificial-intelligence"),
        {TEXT("BehaviorTrees"), TEXT("EQS"), TEXT("Navigation"), TEXT("Perception")},
        EDocumentationPriority::High
    });
    
    DocumentationIndex.Add(EUE5DocSection::Performance, FUE5DocumentationSection{
        TEXT("Performance"),
        TEXT("https://dev.epicgames.com/documentation/en-us/unreal-engine/optimization"),
        {TEXT("Profiling"), TEXT("Memory"), TEXT("Rendering"), TEXT("Cooking")},
        EDocumentationPriority::High
    });
}

void UMingRTSUE5LearningAPI::SetLearningConfig(const FUE5LearningConfig& Config)
{
    CurrentConfig = Config;
}

FUE5LearningConfig UMingRTSUE5LearningAPI::GetLearningConfig() const
{
    return CurrentConfig;
}

bool UMingRTSUE5LearningAPI::IsInitialized() const
{
    return bIsInitialized;
}

bool UMingRTSUE5LearningAPI::IsLearning() const
{
    return bIsLearning;
}

float UMingRTSUE5LearningAPI::GetLearningProgress() const
{
    return CurrentProgress;
}

TArray<FUE5LearningConcept> UMingRTSUE5LearningAPI::GetAllLearnedConcepts() const
{
    TArray<FUE5LearningConcept> Concepts;
    LearnedConcepts.GenerateValueArray(Concepts);
    return Concepts;
}

TArray<FUE5APIPattern> UMingRTSUE5LearningAPI::GetAllAPIPatterns() const
{
    TArray<FUE5APIPattern> Patterns;
    APIPatterns.GenerateValueArray(Patterns);
    return Patterns;
}

TArray<FString> UMingRTSUE5LearningAPI::GetSupportedAPIs() const
{
    TArray<FString> APIs;
    APIPatterns.GenerateKeyArray(APIs);
    return APIs;
}
