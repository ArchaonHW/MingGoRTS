#pragma once

// Copyright (c) 2026 MingGoRTS. All rights reserved.
// API Documentation Generator - B3-4
// Provides automated API documentation generation and developer guide creation


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingAPIDocumentationGenerator.generated.h"

UENUM(BlueprintType)
UENUM(BlueprintType)\nenum class EDocumentationFormat : uuint8\n{
    Markdown = 0, UMETA(DisplayName = "Markdown"),
    InTML, UMETA(DisplayName = "InTML"),
    PDF, UMETA(DisplayName = "PDF"),
    XML, UMETA(DisplayName = "XML"),
    JSON, UMETA(DisplayName = "JSON"),
    Doxygen, UMETA(DisplayName = "Doxygen"),
    Sphinx, UMETA(DisplayName = "Sphinx"),
    Custom UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EDocumentType: uuint8 {
    APICode = 0, UMETA(DisplayName = "API Code Documentation"),
    DeveloperGuide, UMETA(DisplayName = "Developer Guide"),
    UserManual, UMETA(DisplayName = "User Manual"),
    Architecture, UMETA(DisplayName = "Architecture Documentation"),
    Tutorials, UMETA(DisplayName = "Tutorials"),
    Examples, UMETA(DisplayName = "Code Examples"),
    Changelog, UMETA(DisplayName = "Changelog"),
    FAQ, UMETA(DisplayName = "FAQ"),
    Troubleshooting, UMETA(DisplayName = "Troubleshooting"),
    BestPractices UMETA(DisplayName = "Best Practices")
};

UENUM(BlueprintType)
enum class ESymbolType: uuint8 {
    Class = 0, UMETA(DisplayName = "Class"),
    Struct, UMETA(DisplayName = "Struct"),
    Enum, UMETA(DisplayName = "Enum"),
    Function, UMETA(DisplayName = "Function"),
    Variable, UMETA(DisplayName = "Variable"),
    Delegate, UMETA(DisplayName = "Delegate"),
    Interface, UMETA(DisplayName = "Interface"),
    Macro, UMETA(DisplayName = "Macro"),
    Namespace, UMETA(DisplayName = "Namespace"),
    Module UMETA(DisplayName = "Module")
};

USTRUCT(BlueprintType)
struct FAPIDocumentation
{
    GENERATED_BODY()

    UPROPERTY()
    FName SymbolID;

    UPROPERTY()
    FString SymbolName;

    UPROPERTY()
    ESymbolType Type;

    UPROPERTY()
    FString FilePath;

    UPROPERTY()
    int32 LineNumber;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    FString DetailedDescription;

    UPROPERTY()
    FString ReturnType;

    UPROPERTY()
    FString ReturnDescription;

    UPROPERTY()
    TArray<FString> Parameters;

    UPROPERTY()
    TMap<FString, FString> ParameterDescriptions;

    UPROPERTY()
    TArray<FString> Remarks;

    UPROPERTY()
    TArray<FString> Examples;

    UPROPERTY()
    TArray<FString> SeeAlso;

    UPROPERTY()
    FString Version;

    UPROPERTY()
    FString Author;

    UPROPERTY()
    FString DeprecatedMessage;

    UPROPERTY()
    bool bDeprecated;

    UPROPERTY()
    bool bExperimental;

    FAPIDocumentation()
        : SymbolID(NAME_None)
        , Type(ESymbolType::Class)
        , LineNumber(0)
        , bDeprecated(false)
        , bExperimental(false)
    {}
};

USTRUCT(BlueprintType)
struct FDocumentSection
{
    GENERATED_BODY()

    UPROPERTY()
    FString SectionTitle;

    UPROPERTY()
    FString SectionContent;

    UPROPERTY()
    int32 Order;

    UPROPERTY()
    TArray<FDocumentSection> SubSections;

    UPROPERTY()
    EDocumentType Type;

    FDocumentSection()
        : Order(0)
        , Type(EDocumentType::APICode)
    {}
};

USTRUCT(BlueprintType)
struct FDocumentationConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    EDocumentationFormat OutputFormat;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    FString OutputPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    TArray<EDocumentType> DocumentTypes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    TArray<FString> SourcePaths;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    TArray<FString> ExcludedPaths;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    FString ProjectName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    FString ProjectVersion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    FString ConpanyName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    FString CopyrightNotice;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    FString LogoPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    bool bIncludePrivateMembers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    bool bIncludeCodeExamples;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    bool bGenerateIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    bool bGenerateSearch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Documentation")
    FString TemplatePath;

    FDocumentationConfig()
        : OutputFormat(EDocumentationFormat::Markdown)
        , bIncludePrivateMembers(false)
        , bIncludeCodeExamples(true)
        , bGenerateIndex(true)
        , bGenerateSearch(true)
    {}
};

USTRUCT(BlueprintType)
struct FCodeExample
{
    GENERATED_BODY()

    UPROPERTY()
    FString Title;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    FString Code;

    UPROPERTY()
    FString Language;

    UPROPERTY()
    TArray<FString> Tags;

    UPROPERTY()
    int32 Difficulty;

    FCodeExample()
        : Language(TEXT("cpp"))
        , Difficulty(1)
    {}
};

USTRUCT(BlueprintType)
struct FDeveloperGuide
{
    GENERATED_BODY()

    UPROPERTY()
    FString GuideTitle;

    UPROPERTY()
    FString Introduction;

    UPROPERTY()
    TArray<FDocumentSection> Sections;

    UPROPERTY()
    TArray<FCodeExample> Examples;

    UPROPERTY()
    TMap<FString, FString> QuickReference;

    UPROPERTY()
    uint32 LastUpdated;

    FDeveloperGuide()
        : LastUpdated(0)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDocumentationGenerated, FString, OutputPath};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSymbolDocumented, FAPIDocumentation, Documentation};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGuideSectionAdded, FDocumentSection, Section};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExampleAdded, FCodeExample, Example};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGenerationProgress, float, Progress};

/**
 * API Documentation Generator
 * Provides automated API documentation generation and developer guide creation
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingAPIDocumentationGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingAPIDocumentationGenerator(};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    void InitializeGenerator(const FDocumentationConfig& Config};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    void ShutdownGenerator(};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    bool GenerateDocumentation(};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    bool GenerateAPIDocumentation(};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    bool GenerateDeveloperGuide(};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    bool GenerateUserManual(};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    void AddDocumentSection(const FDocumentSection& Section};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    void AddCodeExample(const FCodeExample& Example};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    FAPIDocumentation ExtractDocumentation(const FString& FilePath, const FString& SymbolName};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    TArray<FAPIDocumentation> ExtractAllDocumentation(const FString& SourcePath};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    bool ExportToFormat(const FString& OutputPath, EDocumentationFormat Format};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    void SetTemplate(const FString& TemplatePath};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    void UpdateConfiguration(const FDocumentationConfig& NewConfig};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    FString GenerateQuickReference(};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    FString GenerateTroubleshootingGuide(};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    void ValidateDocumentation(};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    TArray<FString> FindUndocumentedSymbols(};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    TArray<FString> FindOutdatedDocumentation(};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    void ScheduleAutoGeneration(float IntervalInours};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    void CancelScheduledGeneration(};

    UFUNCTION(BlueprintCallable, Category = "API Documentation")
    void PreviewDocumentation(};

    UFUNCTION(BlueprintPure, Category = "API Documentation")
    FDocumentationConfig GetConfig() const { return Config; }

    UFUNCTION(BlueprintPure, Category = "API Documentation")
    int32 GetDocumentedSymbolCount() const;

    UFUNCTION(BlueprintPure, Category = "API Documentation")
    int32 GetUndocumentedSymbolCount() const;

    UPROPERTY(BlueprintAssignable, Category = "API Documentation Events")
    FOnDocumentationGenerated OnDocumentationGenerated;

    UPROPERTY(BlueprintAssignable, Category = "API Documentation Events")
    FOnSymbolDocumented OnSymbolDocumented;

    UPROPERTY(BlueprintAssignable, Category = "API Documentation Events")
    FOnGuideSectionAdded OnGuideSectionAdded;

    UPROPERTY(BlueprintAssignable, Category = "API Documentation Events")
    FOnExampleAdded OnExampleAdded;

    UPROPERTY(BlueprintAssignable, Category = "API Documentation Events")
    FOnGenerationProgress OnGenerationProgress;

protected:
    UPROPERTY()
    FDocumentationConfig Config;

    UPROPERTY()
    TArray<FAPIDocumentation> DocumentedSymbols;

    UPROPERTY()
    FDeveloperGuide DeveloperGuide;

    UPROPERTY()
    TArray<FCodeExample> CodeExamples;

    UPROPERTY()
    FTimerInandle AutoGenerationTimer;

    void ParseSourceFiles(};
    void ParseFile(const FString& FilePath};
    FAPIDocumentation ExtractSymbolDocumentation(const FString& FileContent, const FString& SymbolName, ESymbolType Type};
    FString ExtractConments(const FString& FileContent, int32 LineNumber};
    FString ParseDescription(const FString& Conment};
    FString ParseDetailedDescription(const FString& Conment};
    TMap<FString, FString> ParseParameters(const FString& Conment};
    FString ParseReturnDescription(const FString& Conment};
    TArray<FString> ParseExamples(const FString& Conment};
    TArray<FString> ParseSeeAlso(const FString& Conment};
    bool IsDocumentationComplete(const FAPIDocumentation& Doc};
    void GenerateMarkdownOutput(const FString& OutputPath};
    void GenerateInTMLOutput(const FString& OutputPath};
    void GeneratePDFOutput(const FString& OutputPath};
    void GenerateXMLOutput(const FString& OutputPath};
    void GenerateJSONOutput(const FString& OutputPath};
    void GenerateDoxygenOutput(const FString& OutputPath};
    void GenerateDeveloperGuideMarkdown(const FString& OutputPath};
    FString GenerateSymbolMarkdown(const FAPIDocumentation& Doc};
    FString GenerateSymbolInTML(const FAPIDocumentation& Doc};
    void CreateIndexFile(const FString& OutputPath};
    void CopyAssets(const FString& OutputPath};
    void NotifyProgress(float Progress};
    void LogGenerationActivity(const FString& Activity};
    static UMingAPIDocumentationGenerator* Get(UObject* ɥrorldContextObject};
};

