// Copyright (c) 2026 MingGoRTS. All rights reserved.
// UE5 Documentation Learning System API
// Converted from PowerShell: Learn-UE5-Documentation-Simplified.ps1

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRTSUE5LearningAPI.generated.h"

UENUM(BlueprintType)
enum class EUE5LearningMode : uint8
{
    Quick,          // 快速學習 - 核心概念
    Comprehensive,  // 全面學習 - 所有文檔
    Targeted        // 定向學習 - 高優先級內容
};

UENUM(BlueprintType)
enum class EUE5DocSection : uint8
{
    GettingStarted  UMETA(DisplayName = "Getting Started"),
    Programming     UMETA(DisplayName = "Programming"),
    Gameplay        UMETA(DisplayName = "Gameplay"),
    Rendering       UMETA(DisplayName = "Rendering"),
    Physics         UMETA(DisplayName = "Physics"),
    AI              UMETA(DisplayName = "AI"),
    Networking      UMETA(DisplayName = "Networking"),
    Audio           UMETA(DisplayName = "Audio"),
    UI              UMETA(DisplayName = "UI"),
    Performance     UMETA(DisplayName = "Performance")
};

UENUM(BlueprintType)
enum class EDocumentationPriority : uint8
{
    Low,
    Medium,
    High
};

USTRUCT(BlueprintType)
struct FUE5DocumentationSection
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    FString SectionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    FString URL;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    TArray<FString> Categories;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    EDocumentationPriority Priority;

    FUE5DocumentationSection()
        : Priority(EDocumentationPriority::Medium)
    {}
};

USTRUCT(BlueprintType)
struct FUE5DocumentationContent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
    FString Section;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
    FString Content;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
    TArray<FString> APIs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
    FString LastUpdated;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
    FString Version;

    FUE5DocumentationContent()
        : Version(TEXT("5.7"))
    {}
};

USTRUCT(BlueprintType)
struct FUE5APIPattern
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Pattern")
    FString APIName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Pattern")
    FString Section;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Pattern")
    FString Usage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Pattern")
    FString Context;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Pattern")
    FString LearnedAt;

    FUE5APIPattern()
    {}
};

USTRUCT(BlueprintType)
struct FUE5LearningConcept
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept")
    FString Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept")
    FString Section;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept")
    FString Usage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concept")
    FString LearnedAt;

    FUE5LearningConcept()
    {}
};

USTRUCT(BlueprintType)
struct FUE5LearningConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    EUE5LearningMode LearningMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    FString DocumentationURL;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    FString OutputPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    FString CachePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    FString KnowledgeBasePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    bool bInteractive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    bool bForceUpdate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    bool bVerbose;

    FUE5LearningConfig()
        : LearningMode(EUE5LearningMode::Comprehensive)
        , DocumentationURL(TEXT("https://dev.epicgames.com/documentation/en-us/unreal-engine/unreal-engine-5-7-documentation"))
        , OutputPath(TEXT("C:/HW/MingGoRTS/Docs/UE5_Learning"))
        , CachePath(TEXT("C:/HW/MingGoRTS/Cache/UE5_Docs"))
        , KnowledgeBasePath(TEXT("C:/HW/MingGoRTS/Knowledge/UE5_5.7"))
        , bInteractive(false)
        , bForceUpdate(false)
        , bVerbose(false)
    {}
};

USTRUCT(BlueprintType)
struct FUE5LearningReport
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Report")
    FString ReportType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Report")
    TArray<FString> LearnedSections;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Report")
    int32 LearnedConceptsCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Report")
    int32 APIPatternsCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Report")
    FString ReportPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Report")
    FString GeneratedAt;

    FUE5LearningReport()
        : LearnedConceptsCount(0)
        , APIPatternsCount(0)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDocumentationFetched, const FUE5DocumentationContent&, Content);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKnowledgeUpdated, const FUE5LearningConcept&, Concept);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLearningCompleted, const FUE5LearningReport&, Report);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLearningProgress, const FString&, Section, float, Progress);

/**
 * UE5 Documentation Learning System API
 * Provides comprehensive learning capabilities for Unreal Engine 5.7 documentation
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSUE5LearningAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSUE5LearningAPI();

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    bool Initialize(const FUE5LearningConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    void Shutdown();

    // Learning Control
    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    bool StartLearning(EUE5LearningMode Mode);

    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    bool StartQuickLearning();

    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    bool StartComprehensiveLearning();

    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    bool StartTargetedLearning();

    // Documentation Fetching
    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    FUE5DocumentationContent FetchDocumentation(EUE5DocSection Section);

    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    bool FetchAllDocumentation();

    // Knowledge Management
    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    bool ImportExistingKnowledge();

    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    bool UpdateKnowledgeBase(const FUE5DocumentationContent& Content);

    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    TArray<FUE5LearningConcept> GetAllLearnedConcepts() const;

    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    TArray<FUE5APIPattern> GetAllAPIPatterns() const;

    // API Pattern Management
    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    FString GetUsageExample(const FString& APIName) const;

    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    FString GetAPIExplanation(const FString& APIName) const;

    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    TArray<FString> GetSupportedAPIs() const;

    // Cache Management
    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    bool ClearCache();

    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    bool IsCacheValid(EUE5DocSection Section) const;

    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    void SetForceUpdate(bool bForce);

    // Report Generation
    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    FUE5LearningReport GenerateQuickReport();

    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    FUE5LearningReport GenerateComprehensiveReport();

    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    FUE5LearningReport GenerateTargetedReport();

    // Configuration
    UFUNCTION(BlueprintCallable, Category = "UE5 Learning API")
    void SetLearningConfig(const FUE5LearningConfig& Config);

    UFUNCTION(BlueprintPure, Category = "UE5 Learning API")
    FUE5LearningConfig GetLearningConfig() const;

    // Status
    UFUNCTION(BlueprintPure, Category = "UE5 Learning API")
    bool IsInitialized() const;

    UFUNCTION(BlueprintPure, Category = "UE5 Learning API")
    bool IsLearning() const;

    UFUNCTION(BlueprintPure, Category = "UE5 Learning API")
    float GetLearningProgress() const;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "UE5 Learning API Events")
    FOnDocumentationFetched OnDocumentationFetched;

    UPROPERTY(BlueprintAssignable, Category = "UE5 Learning API Events")
    FOnKnowledgeUpdated OnKnowledgeUpdated;

    UPROPERTY(BlueprintAssignable, Category = "UE5 Learning API Events")
    FOnLearningCompleted OnLearningCompleted;

    UPROPERTY(BlueprintAssignable, Category = "UE5 Learning API Events")
    FOnLearningProgress OnLearningProgress;

protected:
    // Internal functions
    void InitializeDirectories();
    void WriteLog(const FString& Message, const FString& Level);
    
    FUE5DocumentationContent CreateMockDocumentation(EUE5DocSection Section);
    void ExtractAPIPatterns(const FUE5DocumentationContent& Content);
    void ExtractBestPractices(const FUE5DocumentationContent& Content);
    void GenerateLearningExamples(const FUE5DocumentationContent& Content);
    void UpdateKnowledgeFile(const FString& FilePath, const TArray<FUE5LearningConcept>& Concepts);
    
    bool LoadKnowledgeFile(const FString& FilePath, TArray<FUE5LearningConcept>& OutConcepts);
    bool SaveKnowledgeFile(const FString& FilePath, const TArray<FUE5LearningConcept>& Concepts);

    // Documentation section mappings
    void InitializeDocumentationIndex();
    TArray<EUE5DocSection> GetSectionsForMode(EUE5LearningMode Mode) const;

private:
    UPROPERTY()
    FUE5LearningConfig CurrentConfig;

    UPROPERTY()
    TMap<EUE5DocSection, FUE5DocumentationSection> DocumentationIndex;

    UPROPERTY()
    TMap<FString, FUE5LearningConcept> LearnedConcepts;

    UPROPERTY()
    TMap<FString, FUE5APIPattern> APIPatterns;

    UPROPERTY()
    float CurrentProgress;

    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    bool bIsLearning;

    UPROPERTY()
    FString LogPath;

    // Predefined usage examples
    static const TMap<FString, FString> UsageExamples;
    static const TMap<FString, FString> APIExplanations;
};
