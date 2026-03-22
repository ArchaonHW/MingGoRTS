#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingAIAssetBlueprintLibrary.generated.h"

class UMingAIAssetGenerator;
class UMingAIAssetManager;

/**
 * AIË≥áÁî¢?çÂ??ΩÊï∏Â∫? * ?ê‰??çÂ??ØË™ø?®Á?AIË≥áÁî¢?üÊ??åÁÆ°?ÜÂ??? */
UCLASS()
class MINGCORE_API UMingAIAssetBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?≤Â?AIË≥áÁî¢?üÊ???     */
    UFUNCTION(BlueprintPure, Category = "AI Asset")
    static UMingAIAssetGenerator* GetAIAssetGenerator();

    /**
     * ?≤Â?AIË≥áÁî¢ÁÆ°Á???     */
    UFUNCTION(BlueprintPure, Category = "AI Asset")
    static UMingAIAssetManager* GetAIAssetManager();

    /**
     * ?ùÂ??ñAIË≥áÁî¢Á≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset")
    static void InitializeAIAssetSystem();

    /**
     * ?üÊ?ËßíËâ≤?ñÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Characters")
    static FString GenerateCharacterPortrait(
        const FString& CharacterDescription, 
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    );

    /**
     * ?üÊ?Âª∫Á???     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Buildings")
    static FString GenerateBuilding(
        const FString& BuildingDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    );

    /**
     * ?üÊ?Ê≠¶Âô®
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Weapons")
    static FString GenerateWeapon(
        const FString& WeaponDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    );

    /**
     * ?üÊ?ËªçÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Uniforms")
    static FString GenerateUniform(
        const FString& UniformDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    );

    /**
     * ?üÊ??åÊôØ?≥Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Music")
    static FString GenerateBackgroundMusic(
        const FString& MusicDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        float Duration = 30.0f,
        const FString& MusicStyle = TEXT("Classical")
    );

    /**
     * ?üÊ??¥ÊôØ
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Scenes")
    static FString GenerateScene(
        const FString& SceneDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 1024,
        int32 ImageHeight = 512
    );

    /**
     * ?≤Â?Ê∞ëÂ??ÇÊ??êÁ§∫Ë©?     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetRepublicEraPrompt(ERepublicEraStyle EraStyle, EAIAssetType AssetType);

    /**
     * ?≤Â?ËßíËâ≤?ñÂ??êÁ§∫Ë©?     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetCharacterPortraitPrompt(ERepublicEraStyle EraStyle, const FString& CharacterType);

    /**
     * ?≤Â?Âª∫Á??©Ê?Á§∫Ë?
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetBuildingPrompt(ERepublicEraStyle EraStyle, const FString& BuildingType);

    /**
     * ?≤Â?Ê≠¶Âô®?êÁ§∫Ë©?     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetWeaponPrompt(ERepublicEraStyle EraStyle, const FString& WeaponType);

    /**
     * ?≤Â??≥Ê??êÁ§∫Ë©?     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetMusicPrompt(ERepublicEraStyle EraStyle, const FString& MusicMood);

    /**
     * ?úÁ¥¢Ë≥áÁî¢
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
     * ?≤Â?Ë≥áÁî¢‰ø°ÊÅØ
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Management")
    static FString GetAssetInfo(const FString& AssetID);

    /**
     * Ê®ôË?Ë≥áÁî¢?∫Êî∂??     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static bool SetAssetFavorite(const FString& AssetID, bool bFavorite);

    /**
     * Ë©ïÂ?Ë≥áÁî¢
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static bool RateAsset(const FString& AssetID, float Rating);

    /**
     * Â∞éÂá∫Ë≥áÁî¢
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static bool ExportAsset(const FString& AssetID, const FString& ExportPath);

    /**
     * ?≤Â?Ë≥áÁî¢Áµ±Ë?
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Management")
    static TMap<EAssetCategory, int32> GetAssetStatistics();

    /**
     * ?≤Â??®Ëñ¶Ë≥áÁî¢
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static TArray<FString> GetRecommendedAssets(EAssetCategory Category, int32 Count = 10);

    /**
     * ?≤Â?Â≠òÂÑ≤‰ΩøÁî®?ÖÊ?
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Management")
    static float GetStorageUsageMB();

    /**
     * Ê∏ÖÁ??°Ê?Ë≥áÁî¢
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static int32 CleanupInvalidAssets();

    /**
     * ?πÈ??üÊ?ËßíËâ≤?ñÂ???     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Batch Generation")
    static TArray<FString> GenerateCharacterPortraitSet(
        const TArray<FString>& CharacterDescriptions,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade
    );

    /**
     * ?πÈ??üÊ?Âª∫Á??©È?
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Batch Generation")
    static TArray<FString> GenerateBuildingSet(
        const TArray<FString>& BuildingDescriptions,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade
    );

    /**
     * ?πÈ??üÊ??≥Ê???     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Batch Generation")
    static TArray<FString> GenerateMusicSet(
        const TArray<FString>& MusicDescriptions,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        float Duration = 30.0f
    );

    /**
     * Ë®≠ÁΩÆStable DiffusionÁ´ØÈ?
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Configuration")
    static void SetStableDiffusionEndpoint(const FString& Endpoint);

    /**
     * Ë®≠ÁΩÆAIVAÁ´ØÈ?
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Configuration")
    static void SetAIVAEndpoint(const FString& Endpoint);

    /**
     * ?≤Â?Á≥ªÁµ±?Ä??     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|System")
    static bool IsAIAssetSystemInitialized();

    /**
     * ?≤Â??üÊ??äÂ??Ä??     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|System")
    static int32 GetGenerationQueueCount();

    /**
     * ?ñÊ??Ä?âÁ??ê‰ªª??     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|System")
    static void CancelAllGenerations();
};
