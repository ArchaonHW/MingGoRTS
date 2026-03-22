#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingAIAssetBlueprintLibrary.generated.h"

class UMingAIAssetGenerator;
class UMingAIAssetManager;

/**
 * AI�겣???X?��?? * 故事重要性?��摧毀AI�겣???X?��???X */
UCLASS()
class MINGCORE_API UMingAIAssetBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * 摧毀AI�겣???X     */
    UFUNCTION(BlueprintPure, Category = "AI Asset")
    static UMingAIAssetGenerator* GetAIAssetGenerator();

    /**
     * 摧毀AI�겣��?X     */
    UFUNCTION(BlueprintPure, Category = "AI Asset")
    static UMingAIAssetManager* GetAIAssetManager();

    /**
     * ???X?AI�겣�t��
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset")
    static void InitializeAIAssetSystem();

    /**
     * 摧毀����摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Characters")
    static FString GenerateCharacterPortrait(
        const FString& CharacterDescription, 
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    );

    /**
     * 摧毀��?X     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Buildings")
    static FString GenerateBuilding(
        const FString& BuildingDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    );

    /**
     * 摧毀�Z��
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Weapons")
    static FString GenerateWeapon(
        const FString& WeaponDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    );

    /**
     * 摧毀�x??
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Uniforms")
    static FString GenerateUniform(
        const FString& UniformDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    );

    /**
     * ???X?��摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Music")
    static FString GenerateBackgroundMusic(
        const FString& MusicDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        float Duration = 30.0f,
        const FString& MusicStyle = TEXT("Classical")
    };

    /**
     * ???X?��
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Scenes")
    static FString GenerateScene(
        const FString& SceneDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 1024,
        int32 ImageHeight = 512
    );

    /**
     * 摧毀��?X??X?��??     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetRepublicEraPrompt(ERepublicEraStyle EraStyle, EAIAssetType AssetType);

    /**
     * 摧毀����???X?��??     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetCharacterPortraitPrompt(ERepublicEraStyle EraStyle, const FString& CharacterType) {};

    /**
     * 摧毀��?X???��??
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetBuildingPrompt(ERepublicEraStyle EraStyle, const FString& BuildingType) {};

    /**
     * 摧毀�Z��??��??     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetWeaponPrompt(ERepublicEraStyle EraStyle, const FString& WeaponType) {};

    /**
     * 故事重要性?��??     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetMusicPrompt(ERepublicEraStyle EraStyle, const FString& MusicMood) {};

    /**
     * ??���겣
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static TArray<FString> SearchAssets(
        const FString& SearchKeyword = TEXT(""),
        EAssetCategory Category = EAssetCategory::Characters,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        float MinRating = 0.0f,
        bool bFavoritesOnly = false
    };

    /**
     * 摧毀�겣�H��
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Management")
    static FString GetAssetInfo(const FString& AssetID) {};

    /**
     * ��??�겣??��X     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static bool SetAssetFavorite(const FString& AssetID, bool bFavorite);

    /**
     * ��??�겣
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static bool RateAsset(const FString& AssetID, float Rating);

    /**
     * �ɥX�겣
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static bool ExportAsset(const FString& AssetID, const FString& ExportPath) {};

    /**
     * 摧毀�겣��??
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Management")
    static TMap<EAssetCategory, int32> GetAssetStatistics();

    /**
     * ???X?�˸겣
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static TArray<FString> GetRecommendedAssets(EAssetCategory Category, int32 Count = 10);

    /**
     * 摧毀�s�x�ϥ�摧毀
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Management")
    static float GetStorageUsageMB();

    /**
     * �M?X???�겣
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static int32 CleanupInvalidAssets();

    /**
     * 目標數量����???X     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Batch Generation")
    static TArray<FString> GenerateCharacterPortraitSet(
        const TArray<FString>& CharacterDescriptions,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade
    );

    /**
     * 目標數量��?X???
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Batch Generation")
    static TArray<FString> GenerateBuildingSet(
        const TArray<FString>& BuildingDescriptions,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade
    );

    /**
     * 故事選項X     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Batch Generation")
    static TArray<FString> GenerateMusicSet(
        const TArray<FString>& MusicDescriptions,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        float Duration = 30.0f
    );

    /**
     * �]�mStable Diffusion��??
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Configuration")
    static void SetStableDiffusionEndpoint(const FString& Endpoint) {};

    /**
     * �]�mAIVA��??
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Configuration")
    static void SetAIVAEndpoint(const FString& Endpoint) {};

    /**
     * 摧毀�t��??X     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|System")
    static bool IsAIAssetSystemInitialized();

    /**
     * 故事選項X?X     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|System")
    static int32 GetGenerationQueueCount();

    /**
     * 目標數量?X?��X     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|System")
    static void CancelAllGenerations();
};

