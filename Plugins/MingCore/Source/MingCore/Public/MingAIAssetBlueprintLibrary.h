#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingAIAssetBlueprintLibrary.generated.h"

class UMingAIAssetGenerator;
class UMingAIAssetManager;

/**
 * AI資產???X?數?? * ???X??X?調????AI資產???X?管???X */
UCLASS()
class MINGCORE_API UMingAIAssetBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ????AI資產???X     */
    UFUNCTION(BlueprintPure, Category = "AI Asset")
    static UMingAIAssetGenerator* GetAIAssetGenerator();

    /**
     * ????AI資產管?X     */
    UFUNCTION(BlueprintPure, Category = "AI Asset")
    static UMingAIAssetManager* GetAIAssetManager();

    /**
     * ???X?AI資產系統
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset")
    static void InitializeAIAssetSystem();

    /**
     * ????角色????
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Characters")
    static FString GenerateCharacterPortrait(
        const FString& CharacterDescription, 
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    };

    /**
     * ????建?X     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Buildings")
    static FString GenerateBuilding(
        const FString& BuildingDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    };

    /**
     * ????武器
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Weapons")
    static FString GenerateWeapon(
        const FString& WeaponDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    };

    /**
     * ????軍??
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Uniforms")
    static FString GenerateUniform(
        const FString& UniformDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    };

    /**
     * ???X?景????
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Music")
    static FString GenerateBackgroundMusic(
        const FString& MusicDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        float Duration = 30.0f,
        const FString& MusicStyle = TEXT("Classical")
    };

    /**
     * ???X?景
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Scenes")
    static FString GenerateScene(
        const FString& SceneDescription,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        int32 ImageWidth = 1024,
        int32 ImageHeight = 512
    };

    /**
     * ????民?X??X?示??     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetRepublicEraPrompt(ERepublicEraStyle EraStyle, EAIAssetType AssetType};

    /**
     * ????角色???X?示??     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetCharacterPortraitPrompt(ERepublicEraStyle EraStyle, const FString& CharacterType) {};

    /**
     * ????建?X???示??
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetBuildingPrompt(ERepublicEraStyle EraStyle, const FString& BuildingType) {};

    /**
     * ????武器??示??     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetWeaponPrompt(ERepublicEraStyle EraStyle, const FString& WeaponType) {};

    /**
     * ???X??X?示??     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Prompts")
    static FString GetMusicPrompt(ERepublicEraStyle EraStyle, const FString& MusicMood) {};

    /**
     * ??索資產
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
     * ????資產信息
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Management")
    static FString GetAssetInfo(const FString& AssetID) {};

    /**
     * 標??資產??收X     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static bool SetAssetFavorite(const FString& AssetID, bool bFavorite};

    /**
     * 評??資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static bool RateAsset(const FString& AssetID, float Rating};

    /**
     * 導出資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static bool ExportAsset(const FString& AssetID, const FString& ExportPath) {};

    /**
     * ????資產統??
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Management")
    static TMap<EAssetCategory, int32> GetAssetStatistics();

    /**
     * ???X?薦資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static TArray<FString> GetRecommendedAssets(EAssetCategory Category, int32 Count = 10};

    /**
     * ????存儲使用????
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|Management")
    static float GetStorageUsageMB();

    /**
     * 清?X???資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Management")
    static int32 CleanupInvalidAssets();

    /**
     * ???X???角色???X     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Batch Generation")
    static TArray<FString> GenerateCharacterPortraitSet(
        const TArray<FString>& CharacterDescriptions,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade
    };

    /**
     * ???X???建?X???
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Batch Generation")
    static TArray<FString> GenerateBuildingSet(
        const TArray<FString>& BuildingDescriptions,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade
    };

    /**
     * ???X??X??X     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Batch Generation")
    static TArray<FString> GenerateMusicSet(
        const TArray<FString>& MusicDescriptions,
        ERepublicEraStyle EraStyle = ERepublicEraStyle::NanjingDecade,
        float Duration = 30.0f
    };

    /**
     * 設置Stable Diffusion端??
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Configuration")
    static void SetStableDiffusionEndpoint(const FString& Endpoint) {};

    /**
     * 設置AIVA端??
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|Configuration")
    static void SetAIVAEndpoint(const FString& Endpoint) {};

    /**
     * ????系統??X     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|System")
    static bool IsAIAssetSystemInitialized();

    /**
     * ???X??X??X?X     */
    UFUNCTION(BlueprintPure, Category = "AI Asset|System")
    static int32 GetGenerationQueueCount();

    /**
     * ???X????X?任X     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset|System")
    static void CancelAllGenerations();
};

