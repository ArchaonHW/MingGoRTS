#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRepublicEraPromptLibrary.generated.h"

/**
 * 民�X��X�示詞庫
 * ?��?歷史準確?�風?��?述用?�AI資產?��?
 */
UCLASS(BlueprintType)
class MINGCORE_API UMingRepublicEraPromptLibrary : public UObject
{
    GENERATED_BODY()

public:
    UMingRepublicEraPromptLibrary(};

    /**
     * ?��?民�X��X��?風格?�述
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetEraStyleDescription(ERepublicEraStyle EraStyle};

    /**
     * ?��?角色?��X�示�?     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetCharacterPortraitPrompt(ERepublicEraStyle EraStyle, const FString& CharacterType) {};

    /**
     * ?��?建�X��?示�?
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetBuildingPrompt(ERepublicEraStyle EraStyle, const FString& BuildingType) {};

    /**
     * ?��?武器?�示�?     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetWeaponPrompt(ERepublicEraStyle EraStyle, const FString& WeaponType) {};

    /**
     * ?��?軍�X�示�?     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetUniformPrompt(ERepublicEraStyle EraStyle, const FString& MilitaryBranch) {};

    /**
     * ?��X��?風格?�示�?     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetMusicPrompt(ERepublicEraStyle EraStyle, const FString& MusicMood) {};

    /**
     * ?��X�景?�示�?     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetScenePrompt(ERepublicEraStyle EraStyle, const FString& SceneType) {};

    /**
     * ?��X�用質�X��?�?     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetQualityEnhancers(};

    /**
     * ?��?負面?�示�?     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetNegativePrompts(};

    /**
     * ?��?完整?��X��?示�?
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetFullPrompt(ERepublicEraStyle EraStyle, EAIAssetType AssetType, const FString& SpecificDescription) {};

private:
    // 民�X��?歷史?�景
    static const FString EarlyRepublicContext;
    static const FString NanjingDecadeContext;
    static const FString WarOfResistanceContext;
    static const FString CivilWarContext;

    // 角色類�X�述
    static const FString SoldierDescription;
    static const FString OfficerDescription;
    static const FString CivilianDescription;
    static const FString ScholarDescription;
    static const FString MerchantDescription;

    // 建�?類�X�述
    static const FString GovernmentBuildingDescription;
    static const FString TraditionalHouseDescription;
    static const FString WesternStyleBuildingDescription;
    static const FString MilitaryFacilityDescription;
    static const FString TempleDescription;

    // 武器類�X�述
    static const FString RifleDescription;
    static const FString PistolDescription;
    static const FString MachineGunDescription;
    static const FString ArtilleryDescription;
    static const FString SwordDescription;

    // ?��?風格?�述
    static const FString PatrioticMusicDescription;
    static const FString DramaticMusicDescription;
    static const FString TraditionalMusicDescription;
    static const FString MilitaryMarchDescription;

    // ?�景類�X�述
    static const FString UrbanSceneDescription;
    static const FString RuralSceneDescription;
    static const FString BattlefieldDescription;
    static const FString StreetSceneDescription;
};

