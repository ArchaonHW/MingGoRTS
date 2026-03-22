#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingAIAssetBlueprintLibrary.generated.h"

class UMingAIAssetGenerator;
class UMingAIAssetManager;

/**
 * AI資產藍圖函數庫
 * 提供藍圖可調用的AI資產生成和管理功能
 */
UCLASS()
class MINGCORE_API UMingAIAssetBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * 獲取AI資產生成器
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset")
    static UMingAIAssetGenerator* GetAIAssetGenerator();

    /**
     * 獲取AI資產管理器
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset")
    static UMingAIAssetManager* GetAIAssetManager();

    /**
     * 初始化AI資產系統
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset")
    static void InitializeAIAssetSystem();

    /**
     * 生成角色肖像
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Characters")
    static FString GenerateCharacterPortrait(
        const FString& CharacterDescription, 
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    );

    /**
     * 生成建築物
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Buildings")
    static FString GenerateBuilding(
        const FString& BuildingDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    );

    /**
     * 生成武器
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Weapons")
    static FString GenerateWeapon(
        const FString& WeaponDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    );

    /**
     * 生成軍服
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Uniforms")
    static FString GenerateUniform(
        const FString& UniformDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    );

    /**
     * 生成背景音樂
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Music")
    static FString GenerateBackgroundMusic(
        const FString& MusicDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        float Duration = 30.0f,
        const FString& MusicStyle = TEXT("Classical")
    );

    /**
     * 生成場景
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Scenes")
    static FString GenerateScene(
        const FString& SceneDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 1024,
        int32 ImageHeight = 512
    );

    /**
     * 獲取民國時期提示詞
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetRepublicEraPrompt(ERepublicEraStyle EraStyle, EAIAssetType AssetType);

    /**
     * 獲取角色肖像提示詞
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetCharacterPortraitPrompt(ERepublicEraStyle EraStyle, const FString& CharacterType);

    /**
     * 獲取建築物提示詞
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetBuildingPrompt(ERepublicEraStyle EraStyle, const FString& BuildingType);

    /**
     * 獲取武器提示詞
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetWeaponPrompt(ERepublicEraStyle EraStyle, const FString& WeaponType);

    /**
     * 獲取音樂提示詞
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetMusicPrompt(ERepublicEraStyle EraStyle, const FString& MusicMood);

    /**
     * 搜索資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static TArray<FString> SearchAssets(
        const FString& SearchKeyword = TEXT(""),
        EAssetCategory Category = EAssetCategory::Characters,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        float MinRating = 0.0f,
        bool bFavoritesOnly = false
    );

    /**
     * 獲取資產信息
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Management")
    static FString GetAssetInfo(const FString& AssetID);

    /**
     * 標記資產為收藏
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static bool SetAssetFavorite(const FString& AssetID, bool bFavorite);

    /**
     * 評分資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static bool RateAsset(const FString& AssetID, float Rating);

    /**
     * 導出資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static bool ExportAsset(const FString& AssetID, const FString& ExportPath);

    /**
     * 獲取資產統計
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Management")
    static TMap<EAssetCategory, int32> GetAssetStatistics();

    /**
     * 獲取推薦資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static TArray<FString> GetRecommendedAssets(EAssetCategory Category, int32 Count = 10);

    /**
     * 獲取存儲使用情況
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Management")
    static float GetStorageUsageMB();

    /**
     * 清理無效資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static int32 CleanupInvalidAssets();

    /**
     * 批量生成角色肖像集
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Batch Generation")
    static TArray<FString> GenerateCharacterPortraitSet(
        const TArray<FString>& CharacterDescriptions,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade
    );

    /**
     * 批量生成建築物集
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Batch Generation")
    static TArray<FString> GenerateBuildingSet(
        const TArray<FString>& BuildingDescriptions,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade
    );

    /**
     * 批量生成音樂集
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Batch Generation")
    static TArray<FString> GenerateMusicSet(
        const TArray<FString>& MusicDescriptions,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        float Duration = 30.0f
    );

    /**
     * 設置Stable Diffusion端點
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Configuration")
    static void SetStableDiffusionEndpoint(const FString& Endpoint);

    /**
     * 設置AIVA端點
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Configuration")
    static void SetAIVAEndpoint(const FString& Endpoint);

    /**
     * 獲取系統狀態
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|System")
    static bool IsAIAssetSystemInitialized();

    /**
     * 獲取生成隊列狀態
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|System")
    static int32 GetGenerationQueueCount();

    /**
     * 取消所有生成任務
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|System")
    static void CancelAllGenerations();
};
