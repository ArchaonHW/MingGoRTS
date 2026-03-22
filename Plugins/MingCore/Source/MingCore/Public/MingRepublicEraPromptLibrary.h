#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRepublicEraPromptLibrary.generated.h"

/**
 * ?X??X?ܵw
 * 摧毀vǽT摧毀??z??AI겣摧毀
 */
UCLASS(BlueprintType)
class MINGCORE_API UMingRepublicEraPromptLibrary : public UObject
{
    GENERATED_BODY()

public:
    UMingRepublicEraPromptLibrary();

    /**
     * 摧毀?X??X摧毀?z
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetEraStyleDescription(ERepublicEraStyle EraStyle);

    /**
     * 摧毀目標數量     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetCharacterPortraitPrompt(ERepublicEraStyle EraStyle, const FString& CharacterType) {};

    /**
     * ??目標數量??
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetBuildingPrompt(ERepublicEraStyle EraStyle, const FString& BuildingType) {};

    /**
     * 摧毀Z摧毀     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetWeaponPrompt(ERepublicEraStyle EraStyle, const FString& WeaponType) {};

    /**
     * 摧毀x?X???     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetUniformPrompt(ERepublicEraStyle EraStyle, const FString& MilitaryBranch) {};

    /**
     * 目標數量摧毀     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetMusicPrompt(ERepublicEraStyle EraStyle, const FString& MusicMood) {};

    /**
     * 目標數量??     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetScenePrompt(ERepublicEraStyle EraStyle, const FString& SceneType) {};

    /**
     * ???X?ν?X摧毀?     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetQualityEnhancers();

    /**
     * 摧毀t摧毀     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetNegativePrompts();

    /**
     * 摧毀目標數量??
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetFullPrompt(ERepublicEraStyle EraStyle, EAIAssetType AssetType, const FString& SpecificDescription) {};

private:
    // ?X???v??
    static const FString EarlyRepublicContext;
    static const FString NanjingDecadeContext;
    static const FString WarOfResistanceContext;
    static const FString CivilWarContext;

    // ?X?z
    static const FString SoldierDescription;
    static const FString OfficerDescription;
    static const FString CivilianDescription;
    static const FString ScholarDescription;
    static const FString MerchantDescription;

    // ???X?z
    static const FString GovernmentBuildingDescription;
    static const FString TraditionalHouseDescription;
    static const FString WesternStyleBuildingDescription;
    static const FString MilitaryFacilityDescription;
    static const FString TempleDescription;

    // Z?X?z
    static const FString RifleDescription;
    static const FString PistolDescription;
    static const FString MachineGunDescription;
    static const FString ArtilleryDescription;
    static const FString SwordDescription;

    // 摧毀??z
    static const FString PatrioticMusicDescription;
    static const FString DramaticMusicDescription;
    static const FString TraditionalMusicDescription;
    static const FString MilitaryMarchDescription;

    // ???X?z
    static const FString UrbanSceneDescription;
    static const FString RuralSceneDescription;
    static const FString BattlefieldDescription;
    static const FString StreetSceneDescription;
};

