#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAIGeneratedContentSystem.h"
#include "MingAIContentRenderer.h"
#include "MingAIContentManager.h"
#include "MingAutoSceneGenerator.generated.h"

class UWorld;
class AGameModeBase;
class APlayerController;

UENUM(BlueprintType)
enum class ESceneType : uint8
{
    Battle,         // ?∞È¨•?¥ÊôØ
    Village,        // ?ëË??¥ÊôØ
    Palace,         // ÂÆÆÊÆø?¥ÊôØ
    Forest,         // Ê£ÆÊ??¥ÊôØ
    Mountain,       // Â±±Âú∞?¥ÊôØ
    River,          // Ê≤≥Ê??¥ÊôØ
    Desert,         // Ê≤ôÊ??¥ÊôØ
    City,           // ?éÂ??¥ÊôØ
    Temple,         // ÂØ∫Â??¥ÊôØ
    Battlefield,    // ?∞Â†¥?¥ÊôØ
    Custom          // ?™Â?Áæ©Â†¥??};

UENUM(BlueprintType)
enum class ESceneComplexity : uint8
{
    Simple,         // Á∞°ÂñÆ?¥ÊôØ
    Medium,         // ‰∏≠Á??¥ÊôØ
    Complex,        // Ë§áÈ??¥ÊôØ
    Epic            // ?≤Ë©©?¥ÊôØ
};

UENUM(BlueprintType)
enum class ESceneStyle : uint8
{
    Realistic,      // ÂØ´ÂØ¶È¢®Ê†º
    Stylized,       // È¢®Ê†º??    Cartoon,        // ?°ÈÄöÈ¢®??    Watercolor,     // Ê∞¥ÂΩ©È¢®Ê†º
    Ink,            // Ê∞¥Â¢®È¢®Ê†º
    OilPainting,    // Ê≤πÁï´È¢®Ê†º
    PixelArt,       // ?èÁ??ùË?
    LowPoly,        // ‰ΩéÂ??äÂΩ¢
    CelShaded,      // Ë≥ΩÁ???    Photorealistic  // ?ßÁ?Á¥öÁ?ÂØ?};

USTRUCT(BlueprintType)
struct FSceneRequirement
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Scene Requirement")
    ESceneType SceneType;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Requirement")
    ESceneComplexity Complexity;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Requirement")
    ESceneStyle Style;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Requirement")
    FString Theme;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Requirement")
    FString Description;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Requirement")
    TArray<EAIContentType> RequiredAssets;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Requirement")
    TMap<EAIContentType, FString> AssetPrompts;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Requirement")
    float Duration;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Requirement")
    bool bHasCharacters;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Requirement")
    bool bHasBuildings;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Requirement")
    bool bHasEnvironment;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Requirement")
    bool bHasEffects;

    FSceneRequirement()
    {
        SceneType = ESceneType::Custom;
        Complexity = ESceneComplexity::Medium;
        Style = ESceneStyle::Realistic;
        Theme = TEXT("");
        Description = TEXT("");
        Duration = 30.0f;
        bHasCharacters = true;
        bHasBuildings = true;
        bHasEnvironment = true;
        bHasEffects = true;
    }
};

USTRUCT(BlueprintType)
struct FGeneratedScene
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Generated Scene")
    FString SceneName;

    UPROPERTY(BlueprintReadWrite, Category = "Generated Scene")
    FString ScenePath;

    UPROPERTY(BlueprintReadWrite, Category = "Generated Scene")
    TMap<EAIContentType, FString> GeneratedAssets;

    UPROPERTY(BlueprintReadWrite, Category = "Generated Scene")
    FAISceneConfiguration SceneConfiguration;

    UPROPERTY(BlueprintReadWrite, Category = "Generated Scene")
    bool bIsComplete;

    UPROPERTY(BlueprintReadWrite, Category = "Generated Scene")
    float GenerationProgress;

    UPROPERTY(BlueprintReadWrite, Category = "Generated Scene")
    FDateTime GenerationTime;

    UPROPERTY(BlueprintReadWrite, Category = "Generated Scene")
    FString ThumbnailPath;

    FGeneratedScene()
    {
        SceneName = TEXT("");
        ScenePath = TEXT("");
        bIsComplete = false;
        GenerationProgress = 0.0f;
        GenerationTime = FDateTime::Now();
        ThumbnailPath = TEXT("");
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSceneGenerationStarted, const FString&, SceneName, const FSceneRequirement&, Requirements);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSceneGenerationProgress, const FString&, SceneName, float, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSceneGenerationCompleted, const FString&, SceneName, const FGeneratedScene&, GeneratedScene);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSceneGenerationFailed, const FString&, SceneName, const FString&, ErrorMessage);

/**
 * ?™Â??¥ÊôØ?üÊ??? * ?πÊ??ÄÊ±ÇËá™?ïÁ??êÁ??ê‰∏¶Âª∫Á?ÂÆåÊï¥?¥ÊôØ
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingAutoSceneGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingAutoSceneGenerator();

    // ?ùÂ??ñÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void InitializeAutoSceneGenerator();

    // ?¥ÊôØ?üÊ?
    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    FString GenerateScene(const FSceneRequirement& Requirements);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void GenerateSceneAsync(const FSceneRequirement& Requirements);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    TArray<FString> GenerateMultipleScenes(const TArray<FSceneRequirement>& RequirementsList);

    // ?êË®≠?¥ÊôØÊ®°Êùø
    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    FSceneRequirement GetBattleSceneTemplate(const FString& BattleDescription);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    FSceneRequirement GetVillageSceneTemplate(const FString& VillageTheme);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    FSceneRequirement GetPalaceSceneTemplate(const FString& PalaceStyle);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    FSceneRequirement GetNatureSceneTemplate(ESceneType NatureType, const FString& Environment);

    // ?¥ÊôØÁÆ°Á?
    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    TArray<FGeneratedScene> GetAllGeneratedScenes() const;

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    FGeneratedScene GetGeneratedScene(const FString& SceneName) const;

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void DeleteGeneratedScene(const FString& SceneName);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void LoadSceneIntoWorld(const FString& SceneName, UWorld* World);

    // ?¥ÊôØ?êË¶Ω
    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void StartScenePreview(const FString& SceneName);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void StopScenePreview();

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    bool IsPreviewActive() const { return bPreviewActive; }

    // ?¥ÊôØÂ∞éÂá∫
    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void ExportScene(const FString& SceneName, const FString& ExportPath);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void ExportSceneAsVideo(const FString& SceneName, float Duration, const FString& OutputPath);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void GenerateSceneThumbnail(const FString& SceneName);

    // ?¥ÊôØ?ÜÊ?
    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    float GetSceneComplexityScore(const FString& SceneName) const;

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    TArray<EAIContentType> GetSceneAssetTypes(const FString& SceneName) const;

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    int32 GetSceneAssetCount(const FString& SceneName) const;

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    float GetSceneGenerationTime(const FString& SceneName) const;

    // ?πÈ??ç‰?
    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void BatchGenerateScenes(const TArray<FSceneRequirement>& RequirementsList);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void BatchExportScenes(const TArray<FString>& SceneNames, const FString& ExportFormat);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void OptimizeAllScenes();

    // Ê®°ÊùøÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void SaveSceneTemplate(const FString& TemplateName, const FSceneRequirement& Requirements);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    FSceneRequirement LoadSceneTemplate(const FString& TemplateName);

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    TArray<FString> GetAvailableTemplates() const;

    // ?¥ÊôØËÆäÈ?
    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    TArray<FSceneRequirement> GenerateSceneVariants(const FSceneRequirement& BaseRequirements, int32 VariantCount);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void GenerateSceneVariantsAsync(const FSceneRequirement& BaseRequirements, int32 VariantCount);

    // ?∫ËÉΩ?®Ëñ¶
    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    TArray<FSceneRequirement> GetRecommendedScenes(const FString& Context);

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    FSceneRequirement OptimizeSceneRequirements(const FSceneRequirement& Requirements);

    // ?ßËÉΩ??éß
    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    float GetAverageSceneGenerationTime() const;

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    int32 GetActiveSceneGenerations() const;

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    int32 GetTotalGeneratedScenes() const;

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void ClearSceneCache();

    // ?çÁΩÆÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void SetDefaultQuality(EGenerationQuality Quality);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void SetDefaultStyle(ESceneStyle Style);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void SetOutputDirectory(const FString& Directory);

    // ‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "Auto Scene Events")
    FOnSceneGenerationStarted OnSceneGenerationStarted;

    UPROPERTY(BlueprintAssignable, Category = "Auto Scene Events")
    FOnSceneGenerationProgress OnSceneGenerationProgress;

    UPROPERTY(BlueprintAssignable, Category = "Auto Scene Events")
    FOnSceneGenerationCompleted OnSceneGenerationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Auto Scene Events")
    FOnSceneGenerationFailed OnSceneGenerationFailed;

protected:
    // Á≥ªÁµ±ÁµÑ‰ª∂
    UPROPERTY()
    TObjectPtr<UMingAIContentManager> AIContentManager;

    // ?¥ÊôØ?∏Ê?
    UPROPERTY()
    TMap<FString, FGeneratedScene> GeneratedScenes;

    UPROPERTY()
    TMap<FString, FSceneRequirement> SceneTemplates;

    // ?Ä??    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    bool bPreviewActive;

    UPROPERTY()
    FString CurrentPreviewScene;

    UPROPERTY()
    TArray<FString> ActiveGenerations;

    // ?çÁΩÆ
    UPROPERTY()
    EGenerationQuality DefaultQuality;

    UPROPERTY()
    ESceneStyle DefaultStyle;

    UPROPERTY()
    FString OutputDirectory;

    // Áµ±Ë?
    UPROPERTY()
    int32 TotalSceneCount;

    UPROPERTY()
    float TotalGenerationTime;

    // ?ßÈÉ®?ΩÊï∏
    void ProcessSceneGeneration(const FString& SceneName, const FSceneRequirement& Requirements);
    void GenerateRequiredAssets(const FString& SceneName, const FSceneRequirement& Requirements);
    void SetupSceneConfiguration(const FString& SceneName, const FSceneRequirement& Requirements);
    void CompleteSceneGeneration(const FString& SceneName, bool bSuccess);
    void UpdateGenerationProgress(const FString& SceneName, float Progress);
    void NotifySceneGenerationStarted(const FString& SceneName, const FSceneRequirement& Requirements);
    void NotifySceneGenerationProgress(const FString& SceneName, float Progress);
    void NotifySceneGenerationCompleted(const FString& SceneName, const FGeneratedScene& GeneratedScene);
    void NotifySceneGenerationFailed(const FString& SceneName, const FString& ErrorMessage);

private:
    // ËºîÂä©?ΩÊï∏
    FString GenerateUniqueSceneName() const;
    FString GetScenePath(const FString& SceneName) const;
    FSceneRequirement CreateDefaultRequirements(ESceneType SceneType) const;
    TArray<EAIContentType> DetermineRequiredAssets(const FSceneRequirement& Requirements) const;
    TMap<EAIContentType, FString> GenerateAssetPrompts(const FSceneRequirement& Requirements) const;
    FAISceneConfiguration CreateSceneConfiguration(const FSceneRequirement& Requirements) const;
    float EstimateGenerationTime(const FSceneRequirement& Requirements) const;
    void LoadDefaultTemplates();
    void SaveSceneData();
    void LoadSceneData();
};
