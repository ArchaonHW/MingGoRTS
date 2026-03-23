#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "MingRTSSceneGenerationAPI.generated.h"

/**
 * 場景生成API系統
 * 專門負責遊戲場景生成和管理功能 (從PythonAPI分配而來)
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSSceneGenerationAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSSceneGenerationAPI();

    // 初始化場景生成系統
    UFUNCTION(BlueprintCallable, Category = "Scene Generation API")
    bool InitializeSceneGeneration();

    // 基礎場景生成功能
    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Generation")
    FString GenerateScene(const FString& Description, const FString& SceneType = TEXT("battle"), const FString& Complexity = TEXT("medium"), const FString& Style = TEXT("realistic"));

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Generation")
    FString GenerateSceneFromRequirement(const FSceneRequirement& Requirement);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Generation")
    TArray<FString> GenerateSceneVariants(const FString& BaseDescription, int32 VariantCount = 3);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Generation")
    FString GenerateSceneAsync(const FSceneRequirement& Requirement);

    // 場景模板系統
    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Templates")
    FSceneRequirement GetBattleSceneTemplate(const FString& Description);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Templates")
    FSceneRequirement GetVillageSceneTemplate(const FString& Description);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Templates")
    FSceneRequirement GetPalaceSceneTemplate(const FString& Description);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Templates")
    FSceneRequirement GetForestSceneTemplate(const FString& Description);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Templates")
    FSceneRequirement GetMountainSceneTemplate(const FString& Description);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Templates")
    FSceneRequirement GetRiverSceneTemplate(const FString& Description);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Templates")
    FSceneRequirement GetDesertSceneTemplate(const FString& Description);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Templates")
    FSceneRequirement GetCitySceneTemplate(const FString& Description);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Templates")
    FSceneRequirement GetTempleSceneTemplate(const FString& Description);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Templates")
    FSceneRequirement GetBattlefieldSceneTemplate(const FString& Description);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Templates")
    bool AddCustomSceneTemplate(const FString& TemplateName, const FSceneRequirement& Requirement);

    // 智能場景推薦
    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Recommendations")
    TArray<FSceneRequirement> GetRecommendedScenes(const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Recommendations")
    TArray<FSceneRequirement> GetScenesForGameProgress(const FString& GameProgress);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Recommendations")
    TArray<FSceneRequirement> GetScenesForCharacterLevel(int32 CharacterLevel);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Recommendations")
    FSceneRequirement GetOptimalSceneForContext(const FString& Context, ESceneComplexity MaxComplexity);

    // 場景管理功能
    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Management")
    TArray<FGeneratedScene> GetAllGeneratedScenes();

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Management")
    FGeneratedScene GetGeneratedScene(const FString& SceneID);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Management")
    bool DeleteGeneratedScene(const FString& SceneID);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Management")
    bool ExportScene(const FString& SceneID, const FString& ExportPath);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Management")
    bool ImportScene(const FString& ImportPath);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Management")
    TArray<FString> GetSceneLibrary();

    // 場景渲染和預覽
    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Rendering")
    FString RenderScenePreview(const FString& SceneID, EScenePreviewQuality Quality);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Rendering")
    FString RenderHighQualityScene(const FString& SceneID);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Rendering")
    FString GenerateSceneThumbnail(const FString& SceneID);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Rendering")
    TArray<FString> RenderSceneVariations(const FString& SceneID, int32 VariationCount);

    // 場景分析和優化
    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Analysis")
    TMap<FString, float> AnalyzeSceneComplexity(const FString& SceneID);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Analysis")
    TArray<FString> GetSceneOptimizationSuggestions(const FString& SceneID);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Analysis")
    float CalculateSceneQuality(const FString& SceneID);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Analysis")
    bool ValidateSceneIntegrity(const FString& SceneID);

    // 場景配置和設置
    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Configuration")
    bool SetSceneGenerationParameters(const FSceneGenerationParams& Parameters);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Configuration")
    FSceneGenerationParams GetSceneGenerationParameters();

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Configuration")
    bool LoadSceneConfiguration(const FString& ConfigPath);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Configuration")
    bool SaveSceneConfiguration(const FString& ConfigPath);

    // 批量場景操作
    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Batch")
    FString GenerateSceneBatch(const TArray<FSceneRequirement>& Requirements);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Batch")
    TArray<FString> GetBatchGenerationProgress(const FString& BatchID);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Batch")
    bool CancelBatchGeneration(const FString& BatchID);

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Batch")
    FString ExportSceneBatch(const FString& BatchID, const FString& ExportPath);

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnSceneGenerationStarted, const FString&, SceneID, const FSceneRequirement&, Requirement);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParam(FOnSceneGenerationProgress, const FString&, SceneID, float, Progress, const FString&, Status);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnSceneGenerationCompleted, const FString&, SceneID, const FGeneratedScene&, GeneratedScene);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnSceneGenerationFailed, const FString&, SceneID, const FString&, ErrorMessage);

    UPROPERTY(BlueprintAssignable, Category = "Scene Generation|Events")
    FOnSceneGenerationStarted OnSceneGenerationStarted;

    UPROPERTY(BlueprintAssignable, Category = "Scene Generation|Events")
    FOnSceneGenerationProgress OnSceneGenerationProgress;

    UPROPERTY(BlueprintAssignable, Category = "Scene Generation|Events")
    FOnSceneGenerationCompleted OnSceneGenerationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Scene Generation|Events")
    FOnSceneGenerationFailed OnSceneGenerationFailed;

    // 狀態查詢
    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Status")
    bool IsInitialized() const { return bIsInitialized; }

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Status")
    FString GetSystemStatus() const;

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Status")
    TArray<FString> GetAvailableFunctions() const;

    UFUNCTION(BlueprintCallable, Category = "Scene Generation|Status")
    TMap<FString, int32> GetSceneStatistics() const;

private:
    bool bIsInitialized;
    
    // 場景模板
    TMap<FString, FSceneRequirement> SceneTemplates;
    
    // 已生成場景
    TMap<FString, FGeneratedScene> GeneratedScenes;
    
    // 批量生成追蹤
    TMap<FString, TArray<FSceneRequirement>> BatchGenerationProgress;
    
    // 場景生成參數
    FSceneGenerationParams GenerationParameters;

    // 內部輔助函數
    bool LoadSceneTemplates();
    bool LoadGeneratedScenes();
    bool LoadSceneConfiguration();
    
    FString GenerateUniqueSceneID() const;
    FString ProcessSceneGeneration(const FSceneRequirement& Requirement);
    bool ValidateSceneRequirement(const FSceneRequirement& Requirement) const;
    
    FString GetSceneOutputDirectory() const;
    bool EnsureSceneDirectoryExists() const;
    
    // 場景分析輔助函數
    TMap<FString, float> AnalyzeSceneAssets(const FGeneratedScene& Scene);
    float CalculateSceneComplexityScore(const FSceneRequirement& Requirement) const;
    
    // 數據持久化
    TSharedPtr<FJsonObject> SaveSceneData() const;
    bool LoadSceneData(TSharedPtr<FJsonObject> SceneData);
};

// 場景類型枚舉
UENUM(BlueprintType)
enum class ESceneType : uint8
{
    Battle     UMETA(DisplayName = "Battle"),
    Village    UMETA(DisplayName = "Village"),
    Palace     UMETA(DisplayName = "Palace"),
    Forest     UMETA(DisplayName = "Forest"),
    Mountain   UMETA(DisplayName = "Mountain"),
    River      UMETA(DisplayName = "River"),
    Desert     UMETA(DisplayName = "Desert"),
    City       UMETA(DisplayName = "City"),
    Temple     UMETA(DisplayName = "Temple"),
    Battlefield UMETA(DisplayName = "Battlefield"),
    Custom     UMETA(DisplayName = "Custom")
};

// 場景複雜度枚舉
UENUM(BlueprintType)
enum class ESceneComplexity : uint8
{
    Simple    UMETA(DisplayName = "Simple"),
    Medium    UMETA(DisplayName = "Medium"),
    Complex   UMETA(DisplayName = "Complex"),
    Epic      UMETA(DisplayName = "Epic")
};

// 場景風格枚舉
UENUM(BlueprintType)
enum class ESceneStyle : uint8
{
    Realistic        UMETA(DisplayName = "Realistic"),
    Stylized         UMETA(DisplayName = "Stylized"),
    Cartoon          UMETA(DisplayName = "Cartoon"),
    Watercolor       UMETA(DisplayName = "Watercolor"),
    InkWash          UMETA(DisplayName = "Ink Wash"),
    OilPainting      UMETA(DisplayName = "Oil Painting"),
    PixelArt         UMETA(DisplayName = "Pixel Art"),
    LowPoly          UMETA(DisplayName = "Low Poly"),
    CelShading       UMETA(DisplayName = "Cel Shading"),
    Photorealistic   UMETA(DisplayName = "Photorealistic")
};

// 場景預覽質量枚舉
UENUM(BlueprintType)
enum class EScenePreviewQuality : uint8
{
    Draft      UMETA(DisplayName = "Draft"),
    Standard   UMETA(DisplayName = "Standard"),
    High       UMETA(DisplayName = "High"),
    Ultra      UMETA(DisplayName = "Ultra")
};

// 場景需求結構
USTRUCT(BlueprintType)
struct FSCENE_REQUIREMENT
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Requirement")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Requirement")
    ESceneType SceneType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Requirement")
    ESceneComplexity Complexity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Requirement")
    ESceneStyle Style;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Requirement")
    bool bHasCharacters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Requirement")
    bool bHasEnvironment;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Requirement")
    bool bHasEffects;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Requirement")
    int32 Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Requirement")
    TArray<FString> RequiredAssets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Requirement")
    TMap<FString, FString> AdditionalParameters;

    FSCENE_REQUIREMENT()
    {
        Description = TEXT("");
        SceneType = ESceneType::Battle;
        Complexity = ESceneComplexity::Medium;
        Style = ESceneStyle::Realistic;
        bHasCharacters = false;
        bHasEnvironment = false;
        bHasEffects = false;
        Duration = 30;
    }
};

// 生成場景結構
USTRUCT(BlueprintType)
struct FGENERATED_SCENE
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generated Scene")
    FString SceneID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generated Scene")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generated Scene")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generated Scene")
    ESceneType SceneType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generated Scene")
    ESceneComplexity Complexity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generated Scene")
    ESceneStyle Style;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generated Scene")
    FString ScenePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generated Scene")
    TArray<FString> GeneratedAssets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generated Scene")
    FDateTime GeneratedAt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generated Scene")
    float QualityScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generated Scene")
    TMap<FString, FString> Metadata;

    FGENERATED_SCENE()
    {
        SceneID = TEXT("");
        Name = TEXT("");
        Description = TEXT("");
        SceneType = ESceneType::Battle;
        Complexity = ESceneComplexity::Medium;
        Style = ESceneStyle::Realistic;
        ScenePath = TEXT("");
        QualityScore = 0.0f;
    }
};

// 場景生成參數結構
USTRUCT(BlueprintType)
struct FSCENE_GENERATION_PARAMS
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Generation Parameters")
    int32 MaxConcurrentGenerations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Generation Parameters")
    float DefaultQualityThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Generation Parameters")
    int32 GenerationTimeout;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Generation Parameters")
    bool bEnableAutoOptimization;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Generation Parameters")
    bool bEnableCaching;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Generation Parameters")
    FString DefaultOutputPath;

    FSCENE_GENERATION_PARAMS()
    {
        MaxConcurrentGenerations = 3;
        DefaultQualityThreshold = 0.7f;
        GenerationTimeout = 300;
        bEnableAutoOptimization = true;
        bEnableCaching = true;
        DefaultOutputPath = TEXT("");
    }
};
