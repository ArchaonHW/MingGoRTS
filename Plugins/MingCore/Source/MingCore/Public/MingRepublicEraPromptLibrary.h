#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRepublicEraPromptLibrary.generated.h"

/**
 * Ê∞ëÂ??ÇÊ??êÁ§∫Ë©ûÂ∫´
 * ?ê‰?Ê≠∑Âè≤Ê∫ñÁ¢∫?ÑÈ¢®?ºÊ?Ëø∞Áî®?ºAIË≥áÁî¢?üÊ?
 */
UCLASS(BlueprintType)
class MINGCORE_API UMingRepublicEraPromptLibrary : public UObject
{
    GENERATED_BODY()

public:
    UMingRepublicEraPromptLibrary();

    /**
     * ?≤Â?Ê∞ëÂ??ÇÊ??¥È?È¢®Ê†º?èËø∞
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetEraStyleDescription(ERepublicEraStyle EraStyle);

    /**
     * ?≤Â?ËßíËâ≤?ñÂ??êÁ§∫Ë©?     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetCharacterPortraitPrompt(ERepublicEraStyle EraStyle, const FString& CharacterType);

    /**
     * ?≤Â?Âª∫Á??©Ê?Á§∫Ë?
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetBuildingPrompt(ERepublicEraStyle EraStyle, const FString& BuildingType);

    /**
     * ?≤Â?Ê≠¶Âô®?êÁ§∫Ë©?     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetWeaponPrompt(ERepublicEraStyle EraStyle, const FString& WeaponType);

    /**
     * ?≤Â?ËªçÊ??êÁ§∫Ë©?     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetUniformPrompt(ERepublicEraStyle EraStyle, const FString& MilitaryBranch);

    /**
     * ?≤Â??≥Ê?È¢®Ê†º?êÁ§∫Ë©?     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetMusicPrompt(ERepublicEraStyle EraStyle, const FString& MusicMood);

    /**
     * ?≤Â??¥ÊôØ?êÁ§∫Ë©?     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetScenePrompt(ERepublicEraStyle EraStyle, const FString& SceneType);

    /**
     * ?≤Â??öÁî®Ë≥™È??êÂ?Ë©?     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetQualityEnhancers();

    /**
     * ?≤Â?Ë≤†Èù¢?êÁ§∫Ë©?     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetNegativePrompts();

    /**
     * ?≤Â?ÂÆåÊï¥?ÑÁ??àÊ?Á§∫Ë?
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetFullPrompt(ERepublicEraStyle EraStyle, EAIAssetType AssetType, const FString& SpecificDescription);

private:
    // Ê∞ëÂ??ÇÊ?Ê≠∑Âè≤?åÊôØ
    static const FString EarlyRepublicContext;
    static const FString NanjingDecadeContext;
    static const FString WarOfResistanceContext;
    static const FString CivilWarContext;

    // ËßíËâ≤È°ûÂ??èËø∞
    static const FString SoldierDescription;
    static const FString OfficerDescription;
    static const FString CivilianDescription;
    static const FString ScholarDescription;
    static const FString MerchantDescription;

    // Âª∫Á?È°ûÂ??èËø∞
    static const FString GovernmentBuildingDescription;
    static const FString TraditionalHouseDescription;
    static const FString WesternStyleBuildingDescription;
    static const FString MilitaryFacilityDescription;
    static const FString TempleDescription;

    // Ê≠¶Âô®È°ûÂ??èËø∞
    static const FString RifleDescription;
    static const FString PistolDescription;
    static const FString MachineGunDescription;
    static const FString ArtilleryDescription;
    static const FString SwordDescription;

    // ?≥Ê?È¢®Ê†º?èËø∞
    static const FString PatrioticMusicDescription;
    static const FString DramaticMusicDescription;
    static const FString TraditionalMusicDescription;
    static const FString MilitaryMarchDescription;

    // ?¥ÊôØÈ°ûÂ??èËø∞
    static const FString UrbanSceneDescription;
    static const FString RuralSceneDescription;
    static const FString BattlefieldDescription;
    static const FString StreetSceneDescription;
};
