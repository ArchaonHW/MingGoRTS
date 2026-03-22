#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingUXOptimizer.generated.h"

UENUM(BlueprintType)
enum class EMingUXOptimizationType : uint8
{
    ColorBlindness UMETA(DisplayName = "Color Blindness"),
    InterfaceClarity UMETA(DisplayName = "Interface Clarity"),
    LoadingOptimization UMETA(DisplayName = "Loading Optimization"),
    NewPlayerExperience UMETA(DisplayName = "New Player Experience"),
    NavigationImprovement UMETA(DisplayName = "Navigation Improvement"),
    FeedbackEnhancement UMETA(DisplayName = "Feedback Enhancement"),
    Accessibility UMETA(DisplayName = "Accessibility"),
    PerformanceOptimization UMETA(DisplayName = "Performance Optimization")
};

UENUM(BlueprintType)
enum class EMingColorBlindMode : uint8
{
    Normal UMETA(DisplayName = "Normal"),
    Deuteranopia UMETA(DisplayName = "Deuteranopia (Red-Green)"),
    Protanopia UMETA(DisplayName = "Protanopia (Red-Green)"),
    Tritanopia UMETA(DisplayName = "Tritanopia (Blue-Yellow)"),
    Achromatopsia UMETA(DisplayName = "Achromatopsia (Monochrome)")
};

USTRUCT(BlueprintType)
struct FMingUXOptimizationConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingUXOptimizationType OptimizationType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Priority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TargetScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> Settings;

    FMingUXOptimizationConfig()
        : OptimizationType(EMingUXOptimizationType::ColorBlindness)
        , bEnabled(true)
        , Priority(5)
        , TargetScore(80.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingUXImprovementResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ImprovementID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingUXOptimizationType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BeforeScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AfterScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Improvement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccessful;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ImplementationDetails;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ChangesMade;

    FMingUXImprovementResult()
        : Type(EMingUXOptimizationType::ColorBlindness)
        , BeforeScore(0.0f)
        , AfterScore(0.0f)
        , Improvement(0.0f)
        , bSuccessful(false)
    {}
};

/**
 * UX Optimizer for MingGoRTS
 * Optimizes user experience based on test results and feedback
 */
UCLASS(ClassGroup = (UX), Blueprintable)
class MINGUX_API UMingUXOptimizer : public UObject
{
    GENERATED_BODY()

public:
    UMingUXOptimizer();

    // Optimization Control
    UFUNCTION(BlueprintCallable, Category = "UX Optimization")
    void InitializeOptimizer();

    UFUNCTION(BlueprintCallable, Category = "UX Optimization")
    void RunAllOptimizations();

    UFUNCTION(BlueprintCallable, Category = "UX Optimization")
    void RunOptimization(EMingUXOptimizationType OptimizationType);

    // Color Blindness Optimization
    UFUNCTION(BlueprintCallable, Category = "Color Blindness")
    void OptimizeColorBlindnessSupport();

    UFUNCTION(BlueprintCallable, Category = "Color Blindness")
    void EnableColorBlindMode(EMingColorBlindMode Mode);

    UFUNCTION(BlueprintCallable, Category = "Color Blindness")
    void SetHighContrastColors(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "Color Blindness")
    void AddPatternIndicators();

    UFUNCTION(BlueprintCallable, Category = "Color Blindness")
    void TestColorCombinations();

    // Interface Clarity Optimization
    UFUNCTION(BlueprintCallable, Category = "Interface Clarity")
    void OptimizeInterfaceClarity();

    UFUNCTION(BlueprintCallable, Category = "Interface Clarity")
    void ImproveTextReadability();

    UFUNCTION(BlueprintCallable, Category = "Interface Clarity")
    void EnhanceVisualHierarchy();

    UFUNCTION(BlueprintCallable, Category = "Interface Clarity")
    void SimplifyComplexElements();

    // Loading Time Optimization
    UFUNCTION(BlueprintCallable, Category = "Loading Optimization")
    void OptimizeLoadingTimes();

    UFUNCTION(BlueprintCallable, Category = "Loading Optimization")
    void ImplementAsyncLoading();

    UFUNCTION(BlueprintCallable, Category = "Loading Optimization")
    void OptimizeAssetLoading();

    UFUNCTION(BlueprintCallable, Category = "Loading Optimization")
    void AddLoadingProgressFeedback();

    // New Player Experience Optimization
    UFUNCTION(BlueprintCallable, Category = "New Player Experience")
    void OptimizeNewPlayerExperience();

    UFUNCTION(BlueprintCallable, Category = "New Player Experience")
    void EnhanceTutorialSystem();

    UFUNCTION(BlueprintCallable, Category = "New Player Experience")
    void AddContextualHints();

    UFUNCTION(BlueprintCallable, Category = "New Player Experience")
    void ImplementGuidedOnboarding();

    // Results and Analysis
    UFUNCTION(BlueprintPure, Category = "Results")
    TArray<FMingUXImprovementResult> GetOptimizationResults() const;

    UFUNCTION(BlueprintPure, Category = "Results")
    float GetOverallUXImprovement() const;

    UFUNCTION(BlueprintPure, Category = "Results")
    bool AreTargetsMet() const;

    UFUNCTION(BlueprintCallable, Category = "Results")
    void GenerateOptimizationReport();

    UFUNCTION(BlueprintCallable, Category = "Results")
    void ExportOptimizationData(const FString& FilePath);

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetOptimizationTypeName(EMingUXOptimizationType Type);

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetColorBlindModeName(EMingColorBlindMode Mode);

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString SaveUXOptimizationData() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void LoadUXOptimizationData(const FString& JsonString);

protected:
    UPROPERTY()
    TMap<EMingUXOptimizationType, FMingUXOptimizationConfig> OptimizationConfigs;

    UPROPERTY()
    TMap<FString, FMingUXImprovementResult> OptimizationResults;

    UPROPERTY()
    EMingColorBlindMode CurrentColorBlindMode;

    UPROPERTY()
    bool bHighContrastEnabled;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal Functions
    void InitializeOptimizationConfigs();
    void AnalyzeCurrentUXState();
    void PrioritizeOptimizations();

    // Color Blindness Implementation
    void ApplyColorBlindPalette();
    void AddSymbolIndicators();
    void ImplementPatternDifferentiation();
    void TestColorAccessibility();

    // Interface Clarity Implementation
    void IncreaseFontSizes();
    void ImproveContrastRatios();
    void SimplifyLayouts();
    void AddVisualGuidance();

    // Loading Optimization Implementation
    void SetupAsyncLoading();
    void OptimizeTextureLoading();
    void ImplementAssetCaching();
    void AddLoadingScreenImprovements();

    // New Player Experience Implementation
    void CreateInteractiveTutorial();
    void AddHelpTooltips();
    void ImplementSmartHints();
    void DesignProgressiveDisclosure();

    // Results Processing
    void ProcessOptimizationResults();
    void CalculateImprovements();
    void VerifyOptimizationSuccess();
    void GenerateRecommendations();

    // Helpers
    FMingUXOptimizationConfig* FindOptimizationConfig(EMingUXOptimizationType Type);
    FMingUXImprovementResult* FindOptimizationResult(const FString& ResultID);
    void AddOptimizationResult(const FMingUXImprovementResult& Result);
    void LogOptimizationEvent(const FString& Event, const FString& Details);
    float CalculateImprovementScore(float Before, float After) const;
};
