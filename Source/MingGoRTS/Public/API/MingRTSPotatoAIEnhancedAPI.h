// MingGoRTS Enhanced Potato AI API
// Enhanced C++ conversion of PowerShell Potato AI functionality with advanced features

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "MingRTSPotatoAIEnhancedAPI.generated.h"

// Enhanced code generation modes
UENUM(BlueprintType)
enum class EPotatoAIEnhancedMode : uint8
{
    Intelligent     UMETA(DisplayName = "Intelligent AI Generation"),
    Template        UMETA(DisplayName = "Template-Based Generation"),
    Hybrid          UMETA(DisplayName = "Hybrid AI + Template"),
    Learning        UMETA(DisplayName = "Learning-Based Generation"),
    Optimized       UMETA(DisplayName = "Performance Optimized")
};

// Code complexity levels
UENUM(BlueprintType)
enum class EPotatoAICodeComplexity : uint8
{
    Simple          UMETA(DisplayName = "Simple"),
    Medium          UMETA(DisplayName = "Medium"),
    Complex         UMETA(DisplayName = "Complex"),
    Enterprise      UMETA(DisplayName = "Enterprise"),
    Advanced        UMETA(DisplayName = "Advanced")
};

// AI model types
UENUM(BlueprintType)
enum class EPotatoAIModel : uint8
{
    GPT4            UMETA(DisplayName = "GPT-4"),
    Claude3         UMETA(DisplayName = "Claude 3"),
    Gemini          UMETA(DisplayName = "Gemini"),
    Local           UMETA(DisplayName = "Local Model"),
    Hybrid          UMETA(DisplayName = "Hybrid Models")
};

// Enhanced code quality assessment
USTRUCT(BlueprintType)
struct MINGGORTS_API FPotatoAIEnhancedQuality
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Quality")
    int32 OverallScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Quality")
    int32 CodeStructureScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Quality")
    int32 PerformanceScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Quality")
    int32 MaintainabilityScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Quality")
    int32 SecurityScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Quality")
    int32 DocumentationScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Quality")
    int32 BlueprintCompatibilityScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Quality")
    TArray<FString> Recommendations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Quality")
    FString DetailedReport;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Quality")
    bool bMeetsStandards;

    FPotatoAIEnhancedQuality()
    {
        OverallScore = 0;
        CodeStructureScore = 0;
        PerformanceScore = 0;
        MaintainabilityScore = 0;
        SecurityScore = 0;
        DocumentationScore = 0;
        BlueprintCompatibilityScore = 0;
        Recommendations = TArray<FString>();
        DetailedReport = TEXT("");
        bMeetsStandards = false;
    }
};

// Enhanced generation options
USTRUCT(BlueprintType)
struct MINGGORTS_API FPotatoAIEnhancedOptions
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Options")
    EPotatoAIEnhancedMode GenerationMode = EPotatoAIEnhancedMode::Intelligent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Options")
    EPotatoAICodeComplexity Complexity = EPotatoAICodeComplexity::Medium;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Options")
    EPotatoAIModel AIModel = EPotatoAIModel::GPT4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Options")
    bool bEnableLearning = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Options")
    bool bAutoOptimize = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Options")
    bool bIncludeTests = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Options")
    bool bIncludeDocumentation = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Options")
    bool bIncludeExamples = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Options")
    float Temperature = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Options")
    int32 MaxTokens = 2048;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Options")
    TMap<FString, FString> CustomParameters;

    FPotatoAIEnhancedOptions()
    {
        GenerationMode = EPotatoAIEnhancedMode::Intelligent;
        Complexity = EPotatoAICodeComplexity::Medium;
        AIModel = EPotatoAIModel::GPT4;
        bEnableLearning = true;
        bAutoOptimize = true;
        bIncludeTests = true;
        bIncludeDocumentation = true;
        bIncludeExamples = true;
        Temperature = 0.7f;
        MaxTokens = 2048;
        CustomParameters = TMap<FString, FString>();
    }
};

// Enhanced generation result
USTRUCT(BlueprintType)
struct MINGGORTS_API FPotatoAIEnhancedResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Result")
    FString GeneratedCode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Result")
    FString HeaderCode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Result")
    FString ImplementationCode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Result")
    FString TestCode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Result")
    FString Documentation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Result")
    TArray<FString> UsageExamples;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Result")
    FPotatoAIEnhancedQuality QualityAssessment;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Result")
    bool bGenerationSuccessful;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Result")
    float GenerationTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Result")
    FString ErrorMessage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Result")
    TArray<FString> GeneratedFiles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Result")
    FString ClassName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Result")
    FString ModulePath;

    FPotatoAIEnhancedResult()
    {
        GeneratedCode = TEXT("");
        HeaderCode = TEXT("");
        ImplementationCode = TEXT("");
        TestCode = TEXT("");
        Documentation = TEXT("");
        UsageExamples = TArray<FString>();
        QualityAssessment = FPotatoAIEnhancedQuality();
        bGenerationSuccessful = false;
        GenerationTime = 0.0f;
        ErrorMessage = TEXT("");
        GeneratedFiles = TArray<FString>();
        ClassName = TEXT("");
        ModulePath = TEXT("");
    }
};

// Learning data structure
USTRUCT(BlueprintType)
struct MINGGORTS_API FPotatoAILearningData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Data")
    FString Requirement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Data")
    FString GeneratedCode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Data")
    FPotatoAIEnhancedQuality QualityScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Data")
    FDateTime Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Data")
    TArray<FString> UserFeedback;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Data")
    bool bWasAccepted;

    FPotatoAILearningData()
    {
        Requirement = TEXT("");
        GeneratedCode = TEXT("");
        QualityScore = FPotatoAIEnhancedQuality();
        Timestamp = FDateTime::Now();
        UserFeedback = TArray<FString>();
        bWasAccepted = false;
    }
};

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPotatoAIEnhancedCodeGenerated, const FString&, Requirement, const FPotatoAIEnhancedResult&, Result, float, GenerationTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPotatoAIQualityAnalysisCompleted, const FString&, Code, const FPotatoAIEnhancedQuality&, Quality);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPotatoAILearningDataUpdated, const FPotatoAILearningData&, LearningData);

/**
 * MingGoRTS Enhanced Potato AI API
 * Advanced C++ conversion with enhanced AI capabilities and learning features
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|API|Enhanced Potato AI")
class MINGGORTS_API UMingRTSPotatoAIEnhancedAPI : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingRTSPotatoAIEnhancedAPI();

    // Initialize enhanced Potato AI system
    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    bool InitializeEnhancedPotatoAI();

    // Enhanced code generation
    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    FPotatoAIEnhancedResult GenerateEnhancedCode(
        const FString& Requirement,
        const FPotatoAIEnhancedOptions& Options = FPotatoAIEnhancedOptions()
    );

    // Intelligent code generation with learning
    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    FPotatoAIEnhancedResult GenerateIntelligentCode(
        const FString& Requirement,
        EPotatoAICodeComplexity Complexity = EPotatoAICodeComplexity::Medium
    );

    // Multi-model code generation
    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    FPotatoAIEnhancedResult GenerateWithMultipleModels(
        const FString& Requirement,
        const TArray<EPotatoAIModel>& Models,
        const FPotatoAIEnhancedOptions& Options = FPotatoAIEnhancedOptions()
    );

    // Learning-based generation
    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    FPotatoAIEnhancedResult GenerateWithLearning(
        const FString& Requirement,
        bool bUseHistoricalData = true
    );

    // Advanced quality analysis
    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    FPotatoAIEnhancedQuality PerformAdvancedQualityAnalysis(const FString& Code);

    // Code optimization
    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    FString OptimizeCodeForPerformance(const FString& Code);

    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    FString OptimizeCodeForBlueprint(const FString& Code);

    // Learning system
    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    void RecordLearningData(const FPotatoAILearningData& LearningData);

    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    TArray<FPotatoAILearningData> GetLearningHistory();

    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    void ClearLearningData();

    // Template management
    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    bool SaveTemplate(const FString& TemplateName, const FString& TemplateCode);

    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    FString LoadTemplate(const FString& TemplateName);

    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    TArray<FString> GetAvailableTemplates();

    // Batch generation
    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    TArray<FPotatoAIEnhancedResult> GenerateBatch(
        const TArray<FString>& Requirements,
        const FPotatoAIEnhancedOptions& Options = FPotatoAIEnhancedOptions()
    );

    // Code comparison and merging
    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    FString CompareAndMergeCode(const TArray<FString>& CodeSnippets);

    // Export and import
    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    bool ExportLearningData(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    bool ImportLearningData(const FString& FilePath);

    // Configuration
    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    void SetDefaultAIModel(EPotatoAIModel Model) { DefaultAIModel = Model; }

    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    EPotatoAIModel GetDefaultAIModel() const { return DefaultAIModel; }

    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    void SetLearningEnabled(bool bEnabled) { bLearningEnabled = bEnabled; }

    UFUNCTION(BlueprintCallable, Category = "Enhanced Potato AI")
    bool IsLearningEnabled() const { return bLearningEnabled; }

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Enhanced Potato AI")
    FOnPotatoAIEnhancedCodeGenerated OnPotatoAIEnhancedCodeGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Enhanced Potato AI")
    FOnPotatoAIQualityAnalysisCompleted OnPotatoAIQualityAnalysisCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Enhanced Potato AI")
    FOnPotatoAILearningDataUpdated OnPotatoAILearningDataUpdated;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    // Configuration
    UPROPERTY(EditAnywhere, Category = "Configuration")
    EPotatoAIModel DefaultAIModel = EPotatoAIModel::GPT4;

    UPROPERTY(EditAnywhere, Category = "Configuration")
    bool bLearningEnabled = true;

    UPROPERTY(EditAnywhere, Category = "Configuration")
    FString LearningDataPath = TEXT("PotatoAI/LearningData.json");

    UPROPERTY(EditAnywhere, Category = "Configuration")
    FString TemplatesPath = TEXT("PotatoAI/Templates/");

    UPROPERTY(EditAnywhere, Category = "Configuration")
    int32 MaxLearningDataEntries = 1000;

    // Learning data storage
    UPROPERTY(EditAnywhere, Category = "Learning")
    TArray<FPotatoAILearningData> LearningHistory;

    // Template storage
    UPROPERTY(EditAnywhere, Category = "Templates")
    TMap<FString, FString> CodeTemplates;

    // Statistics
    UPROPERTY(EditAnywhere, Category = "Statistics")
    int32 TotalGenerations;

    UPROPERTY(EditAnywhere, Category = "Statistics")
    float AverageQualityScore;

    UPROPERTY(EditAnywhere, Category = "Statistics")
    TMap<EPotatoAIModel, int32> ModelUsageStats;

    // Internal methods
    FString GenerateCodeWithAI(const FString& Requirement, const FPotatoAIEnhancedOptions& Options);
    FString GenerateCodeWithTemplate(const FString& Requirement, const FPotatoAIEnhancedOptions& Options);
    FString GenerateCodeHybrid(const FString& Requirement, const FPotatoAIEnhancedOptions& Options);
    
    FPotatoAIEnhancedQuality AnalyzeCodeStructure(const FString& Code);
    FPotatoAIEnhancedQuality AnalyzePerformance(const FString& Code);
    FPotatoAIEnhancedQuality AnalyzeSecurity(const FString& Code);
    FPotatoAIEnhancedQuality AnalyzeMaintainability(const FString& Code);
    
    void ApplyLearningOptimization(FString& Code, const FString& Requirement);
    void GenerateDocumentation(FString& Code, const FString& Requirement);
    void GenerateTestCode(FString& TestCode, const FString& GeneratedCode, const FString& ClassName);
    void GenerateUsageExamples(TArray<FString>& Examples, const FString& Code, const FString& ClassName);
    
    bool SaveLearningDataToFile();
    bool LoadLearningDataFromFile();
    void UpdateLearningStatistics(const FPotatoAILearningData& Data);
    
    FString GenerateClassName(const FString& Requirement);
    FString GenerateModulePath(const FString& ClassName);
    FString GenerateHeaderCode(const FString& ClassName, const FString& Implementation);
    FString GenerateImplementationCode(const FString& ClassName, const FString& Header);
    
    void LogGeneration(const FString& Requirement, const FPotatoAIEnhancedResult& Result);
    void UpdateModelUsageStats(EPotatoAIModel Model);
};
