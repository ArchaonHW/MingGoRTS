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
    Battle,         // 戰鬥場景
    Village,        // 村莊場景
    Palace,         // 宮殿場景
    Forest,         // 森林場景
    Mountain,       // 山地場景
    River,          // 河流場景
    Desert,         // 沙漠場景
    City,           // 城市場景
    Temple,         // 寺廟場景
    Battlefield,    // 戰場場景
    Custom          // 自定義場景
};

UENUM(BlueprintType)
enum class ESceneComplexity : uint8
{
    Simple,         // 簡單場景
    Medium,         // 中等場景
    Complex,        // 複雜場景
    Epic            // 史詩場景
};

UENUM(BlueprintType)
enum class ESceneStyle : uint8
{
    Realistic,      // 寫實風格
    Stylized,       // 風格化
    Cartoon,        // 卡通風格
    Watercolor,     // 水彩風格
    Ink,            // 水墨風格
    OilPainting,    // 油畫風格
    PixelArt,       // 像素藝術
    LowPoly,        // 低多邊形
    CelShaded,      // 賽璐璐
    Photorealistic  // 照片級真實
};

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
 * 自動場景生成器
 * 根據需求自動生成素材並建立完整場景
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingAutoSceneGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingAutoSceneGenerator();

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void InitializeAutoSceneGenerator();

    // 場景生成
    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    FString GenerateScene(const FSceneRequirement& Requirements);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void GenerateSceneAsync(const FSceneRequirement& Requirements);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    TArray<FString> GenerateMultipleScenes(const TArray<FSceneRequirement>& RequirementsList);

    // 預設場景模板
    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    FSceneRequirement GetBattleSceneTemplate(const FString& BattleDescription);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    FSceneRequirement GetVillageSceneTemplate(const FString& VillageTheme);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    FSceneRequirement GetPalaceSceneTemplate(const FString& PalaceStyle);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    FSceneRequirement GetNatureSceneTemplate(ESceneType NatureType, const FString& Environment);

    // 場景管理
    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    TArray<FGeneratedScene> GetAllGeneratedScenes() const;

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    FGeneratedScene GetGeneratedScene(const FString& SceneName) const;

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void DeleteGeneratedScene(const FString& SceneName);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void LoadSceneIntoWorld(const FString& SceneName, UWorld* World);

    // 場景預覽
    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void StartScenePreview(const FString& SceneName);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void StopScenePreview();

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    bool IsPreviewActive() const { return bPreviewActive; }

    // 場景導出
    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void ExportScene(const FString& SceneName, const FString& ExportPath);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void ExportSceneAsVideo(const FString& SceneName, float Duration, const FString& OutputPath);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void GenerateSceneThumbnail(const FString& SceneName);

    // 場景分析
    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    float GetSceneComplexityScore(const FString& SceneName) const;

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    TArray<EAIContentType> GetSceneAssetTypes(const FString& SceneName) const;

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    int32 GetSceneAssetCount(const FString& SceneName) const;

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    float GetSceneGenerationTime(const FString& SceneName) const;

    // 批量操作
    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void BatchGenerateScenes(const TArray<FSceneRequirement>& RequirementsList);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void BatchExportScenes(const TArray<FString>& SceneNames, const FString& ExportFormat);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void OptimizeAllScenes();

    // 模板管理
    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void SaveSceneTemplate(const FString& TemplateName, const FSceneRequirement& Requirements);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    FSceneRequirement LoadSceneTemplate(const FString& TemplateName);

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    TArray<FString> GetAvailableTemplates() const;

    // 場景變體
    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    TArray<FSceneRequirement> GenerateSceneVariants(const FSceneRequirement& BaseRequirements, int32 VariantCount);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void GenerateSceneVariantsAsync(const FSceneRequirement& BaseRequirements, int32 VariantCount);

    // 智能推薦
    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    TArray<FSceneRequirement> GetRecommendedScenes(const FString& Context);

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    FSceneRequirement OptimizeSceneRequirements(const FSceneRequirement& Requirements);

    // 性能監控
    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    float GetAverageSceneGenerationTime() const;

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    int32 GetActiveSceneGenerations() const;

    UFUNCTION(BlueprintPure, Category = "Auto Scene Generator")
    int32 GetTotalGeneratedScenes() const;

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void ClearSceneCache();

    // 配置管理
    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void SetDefaultQuality(EGenerationQuality Quality);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void SetDefaultStyle(ESceneStyle Style);

    UFUNCTION(BlueprintCallable, Category = "Auto Scene Generator")
    void SetOutputDirectory(const FString& Directory);

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Auto Scene Events")
    FOnSceneGenerationStarted OnSceneGenerationStarted;

    UPROPERTY(BlueprintAssignable, Category = "Auto Scene Events")
    FOnSceneGenerationProgress OnSceneGenerationProgress;

    UPROPERTY(BlueprintAssignable, Category = "Auto Scene Events")
    FOnSceneGenerationCompleted OnSceneGenerationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Auto Scene Events")
    FOnSceneGenerationFailed OnSceneGenerationFailed;

protected:
    // 系統組件
    UPROPERTY()
    TObjectPtr<UMingAIContentManager> AIContentManager;

    // 場景數據
    UPROPERTY()
    TMap<FString, FGeneratedScene> GeneratedScenes;

    UPROPERTY()
    TMap<FString, FSceneRequirement> SceneTemplates;

    // 狀態
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    bool bPreviewActive;

    UPROPERTY()
    FString CurrentPreviewScene;

    UPROPERTY()
    TArray<FString> ActiveGenerations;

    // 配置
    UPROPERTY()
    EGenerationQuality DefaultQuality;

    UPROPERTY()
    ESceneStyle DefaultStyle;

    UPROPERTY()
    FString OutputDirectory;

    // 統計
    UPROPERTY()
    int32 TotalSceneCount;

    UPROPERTY()
    float TotalGenerationTime;

    // 內部函數
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
    // 輔助函數
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
