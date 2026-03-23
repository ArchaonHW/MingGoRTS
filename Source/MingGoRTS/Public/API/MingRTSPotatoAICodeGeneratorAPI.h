// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Potato AI Code Generator API
// Converted from PowerShell: MingPotatoAI-CodeGenerator.ps1

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRTSPotatoAICodeGeneratorAPI.generated.h"

UENUM(BlueprintType)
enum class ECodeTemplateType : uint8
{
    UE5_Class       UMETA(DisplayName = "UE5 Class"),
    UE5_Function    UMETA(DisplayName = "UE5 Function"),
    UE5_System      UMETA(DisplayName = "UE5 System"),
    UE5_Gameplay    UMETA(DisplayName = "UE5 Gameplay"),
    UE5_Generic     UMETA(DisplayName = "UE5 Generic")
};

UENUM(BlueprintType)
enum class ECodeGenerationLanguage : uint8
{
    CPP,
    Blueprint,
    Python,
    JavaScript
};

USTRUCT(BlueprintType)
struct FCodeGenerationOptions
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
    bool bAddComments = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
    bool bAddErrorHandling = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
    bool bOptimizeForBlueprint = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
    bool bIncludeHeaderGuard = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
    bool bAddLogging = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
    FString CustomPrefix;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
    TMap<FString, FString> AdditionalParams;

    FCodeGenerationOptions()
    {}
};

USTRUCT(BlueprintType)
struct FGeneratedCodeResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
    FString CodeContent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
    ECodeTemplateType TemplateType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
    FString Requirement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
    int32 QualityScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
    FString GeneratedAt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
    TArray<FString> Suggestions;

    FGeneratedCodeResult()
        : QualityScore(0)
    {}
};

USTRUCT(BlueprintType)
struct FCodeTemplateInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template")
    ECodeTemplateType TemplateType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template")
    FString Pattern;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template")
    TArray<FString> Keywords;

    FCodeTemplateInfo()
    {}
};

USTRUCT(BlueprintType)
struct FCodeQualityReport
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality")
    int32 OverallScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality")
    bool bHasIncludes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality")
    bool bHasClassDefinition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality")
    bool bHasFunctions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality")
    bool bHasUE5Macros;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality")
    bool bHasComments;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality")
    TArray<FString> Recommendations;

    FCodeQualityReport()
        : OverallScore(0)
        , bHasIncludes(false)
        , bHasClassDefinition(false)
        , bHasFunctions(false)
        , bHasUE5Macros(false)
        , bHasComments(false)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCodeGenerated, const FGeneratedCodeResult&, Result, int32, QualityScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQualityCheckCompleted, const FCodeQualityReport&, Report);

/**
 * Potato AI Code Generator API
 * AI-powered code generation system for UE5 development
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSPotatoAICodeGeneratorAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSPotatoAICodeGeneratorAPI();

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Potato AI Code Generator")
    bool Initialize();

    UFUNCTION(BlueprintCallable, Category = "Potato AI Code Generator")
    void Shutdown();

    // Main Generation Functions
    UFUNCTION(BlueprintCallable, Category = "Potato AI Code Generator")
    FGeneratedCodeResult GenerateCode(const FString& Requirement, ECodeGenerationLanguage Language, ECodeTemplateType Template);

    UFUNCTION(BlueprintCallable, Category = "Potato AI Code Generator")
    FGeneratedCodeResult GenerateUE5Class(const FString& Requirement, const FCodeGenerationOptions& Options);

    UFUNCTION(BlueprintCallable, Category = "Potato AI Code Generator")
    FGeneratedCodeResult GenerateUE5Function(const FString& Requirement, const FCodeGenerationOptions& Options);

    UFUNCTION(BlueprintCallable, Category = "Potato AI Code Generator")
    FGeneratedCodeResult GenerateUE5System(const FString& Requirement, const FCodeGenerationOptions& Options);

    UFUNCTION(BlueprintCallable, Category = "Potato AI Code Generator")
    FGeneratedCodeResult GenerateUE5Gameplay(const FString& Requirement, const FCodeGenerationOptions& Options);

    UFUNCTION(BlueprintCallable, Category = "Potato AI Code Generator")
    FGeneratedCodeResult GenerateGenericCode(const FString& Requirement, const FCodeGenerationOptions& Options);

    // Template Selection
    UFUNCTION(BlueprintPure, Category = "Potato AI Code Generator")
    ECodeTemplateType SelectCodeTemplate(const FString& Requirement, ECodeGenerationLanguage Language);

    UFUNCTION(BlueprintCallable, Category = "Potato AI Code Generator")
    TArray<FCodeTemplateInfo> GetAvailableTemplates() const;

    // Quality Check
    UFUNCTION(BlueprintCallable, Category = "Potato AI Code Generator")
    FCodeQualityReport CheckCodeQuality(const FString& Code);

    UFUNCTION(BlueprintCallable, Category = "Potato AI Code Generator")
    int32 CalculateQualityScore(const FString& Code);

    // Smart Name Generation
    UFUNCTION(BlueprintPure, Category = "Potato AI Code Generator")
    FString GenerateClassName(const FString& Requirement) const;

    UFUNCTION(BlueprintPure, Category = "Potato AI Code Generator")
    FString GenerateFunctionName(const FString& Requirement) const;

    UFUNCTION(BlueprintPure, Category = "Potato AI Code Generator")
    FString GenerateSystemName(const FString& Requirement) const;

    UFUNCTION(BlueprintPure, Category = "Potato AI Code Generator")
    FString GenerateActorName(const FString& Requirement) const;

    UFUNCTION(BlueprintPure, Category = "Potato AI Code Generator")
    FString DetermineReturnType(const FString& Requirement) const;

    // Batch Generation
    UFUNCTION(BlueprintCallable, Category = "Potato AI Code Generator")
    TArray<FGeneratedCodeResult> BatchGenerateCode(const TArray<FString>& Requirements, ECodeTemplateType Template);

    // Export Functions
    UFUNCTION(BlueprintCallable, Category = "Potato AI Code Generator")
    bool ExportCodeToFile(const FGeneratedCodeResult& Result, const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Potato AI Code Generator")
    bool ExportToClipboard(const FGeneratedCodeResult& Result);

    // Status
    UFUNCTION(BlueprintPure, Category = "Potato AI Code Generator")
    bool IsInitialized() const;

    UFUNCTION(BlueprintPure, Category = "Potato AI Code Generator")
    int32 GetTotalGeneratedCount() const;

    UFUNCTION(BlueprintPure, Category = "Potato AI Code Generator")
    float GetAverageQualityScore() const;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Potato AI Code Generator Events")
    FOnCodeGenerated OnCodeGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Potato AI Code Generator Events")
    FOnQualityCheckCompleted OnQualityCheckCompleted;

protected:
    // Internal generation methods
    FString GenerateUE5ClassCode(const FString& ClassName, const FCodeGenerationOptions& Options);
    FString GenerateUE5FunctionCode(const FString& FunctionName, const FString& ReturnType, const FCodeGenerationOptions& Options);
    FString GenerateUE5SystemCode(const FString& SystemName, const FCodeGenerationOptions& Options);
    FString GenerateUE5GameplayCode(const FString& ActorName, const FCodeGenerationOptions& Options);
    FString GenerateGenericCodeContent(const FString& Description, const FCodeGenerationOptions& Options);

    // Helper functions
    void InitializeTemplateDatabase();
    TArray<FString> ExtractKeywords(const FString& Requirement) const;
    bool MatchesTemplate(const FString& Requirement, const FCodeTemplateInfo& Template) const;
    void AddQualitySuggestions(FCodeQualityReport& Report, const FString& Code);

private:
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    TMap<ECodeTemplateType, FCodeTemplateInfo> TemplateDatabase;

    UPROPERTY()
    TArray<FGeneratedCodeResult> GenerationHistory;

    UPROPERTY()
    int32 TotalGeneratedCount;

    UPROPERTY()
    float AverageQualityScore;

    // Predefined naming mappings
    static const TMap<FString, FString> ClassNameMappings;
    static const TMap<FString, FString> SystemNameMappings;
    static const TMap<FString, FString> ActorNameMappings;
    static const TMap<FString, FString> FunctionNameMappings;
};
