#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRepublicEraPromptLibrary.generated.h"

/**
 * 民國時期提示詞庫
 * 提供歷史準確的風格描述用於AI資產生成
 */
UCLASS(BlueprintType)
class MINGCORE_API UMingRepublicEraPromptLibrary : public UObject
{
    GENERATED_BODY()

public:
    UMingRepublicEraPromptLibrary();

    /**
     * 獲取民國時期整體風格描述
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetEraStyleDescription(ERepublicEraStyle EraStyle);

    /**
     * 獲取角色肖像提示詞
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetCharacterPortraitPrompt(ERepublicEraStyle EraStyle, const FString& CharacterType);

    /**
     * 獲取建築物提示詞
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetBuildingPrompt(ERepublicEraStyle EraStyle, const FString& BuildingType);

    /**
     * 獲取武器提示詞
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetWeaponPrompt(ERepublicEraStyle EraStyle, const FString& WeaponType);

    /**
     * 獲取軍服提示詞
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetUniformPrompt(ERepublicEraStyle EraStyle, const FString& MilitaryBranch);

    /**
     * 獲取音樂風格提示詞
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetMusicPrompt(ERepublicEraStyle EraStyle, const FString& MusicMood);

    /**
     * 獲取場景提示詞
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetScenePrompt(ERepublicEraStyle EraStyle, const FString& SceneType);

    /**
     * 獲取通用質量提升詞
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetQualityEnhancers();

    /**
     * 獲取負面提示詞
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetNegativePrompts();

    /**
     * 獲取完整的組合提示詞
     */
    UFUNCTION(BlueprintPure, Category = "Republic Era Prompts")
    static FString GetFullPrompt(ERepublicEraStyle EraStyle, EAIAssetType AssetType, const FString& SpecificDescription);

private:
    // 民國時期歷史背景
    static const FString EarlyRepublicContext;
    static const FString NanjingDecadeContext;
    static const FString WarOfResistanceContext;
    static const FString CivilWarContext;

    // 角色類型描述
    static const FString SoldierDescription;
    static const FString OfficerDescription;
    static const FString CivilianDescription;
    static const FString ScholarDescription;
    static const FString MerchantDescription;

    // 建築類型描述
    static const FString GovernmentBuildingDescription;
    static const FString TraditionalHouseDescription;
    static const FString WesternStyleBuildingDescription;
    static const FString MilitaryFacilityDescription;
    static const FString TempleDescription;

    // 武器類型描述
    static const FString RifleDescription;
    static const FString PistolDescription;
    static const FString MachineGunDescription;
    static const FString ArtilleryDescription;
    static const FString SwordDescription;

    // 音樂風格描述
    static const FString PatrioticMusicDescription;
    static const FString DramaticMusicDescription;
    static const FString TraditionalMusicDescription;
    static const FString MilitaryMarchDescription;

    // 場景類型描述
    static const FString UrbanSceneDescription;
    static const FString RuralSceneDescription;
    static const FString BattlefieldDescription;
    static const FString StreetSceneDescription;
};
