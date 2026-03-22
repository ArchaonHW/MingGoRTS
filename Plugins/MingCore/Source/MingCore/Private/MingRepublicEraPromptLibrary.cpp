#include "MingRepublicEraPromptLibrary.h"
#include "MingAIAssetGenerator.h"

// 民國時期歷史背景
const FString UMingRepublicEraPromptLibrary::EarlyRepublicContext = TEXT(
    "1912-1928 Chinese Republic era, early republican period, "
    "revolutionary atmosphere, overthrow of Qing dynasty, "
    "traditional Chinese clothing mixed with Western influences, "
    "political upheaval, warlord era, modernization efforts, "
    "nationalist revolution, Sun Yat-sen influence"
);

const FString UMingRepublicEraPromptLibrary::NanjingDecadeContext = TEXT(
    "1928-1937 Nanjing decade, golden age of Republic of China, "
    "Art Deco architecture, modern Chinese nationalism, "
    "traditional culture revival, economic prosperity, "
    "Chiang Kai-shek era, modern infrastructure development, "
    "Shanghai international settlement, cultural renaissance"
);

const FString UMingRepublicEraPromptLibrary::WarOfResistanceContext = TEXT(
    "1937-1945 War of Resistance against Japan, wartime atmosphere, "
    "military uniforms, destroyed buildings, refugee camps, "
    "patriotic themes, Japanese invasion, Chinese resistance, "
    "bombing raids, wartime shortages, underground resistance, "
    "united front between Nationalists and Communists"
);

const FString UMingRepublicEraPromptLibrary::CivilWarContext = TEXT(
    "1945-1949 Chinese Civil War, post-war reconstruction, "
    "political tension, divided China, communist vs nationalist themes, "
    "rural revolution, land reform, guerrilla warfare, "
    "Nationalist retreat to Taiwan, Communist victory, "
    "socialist transformation, political propaganda"
);

// 角色類型描述
const FString UMingRepublicEraPromptLibrary::SoldierDescription = TEXT(
    "Chinese soldier, military uniform, determined expression, "
    "battle-hardened, patriotic, weathered face, military discipline, "
    "carrying rifle, combat gear, historical accuracy"
);

const FString UMingRepublicEraPromptLibrary::OfficerDescription = TEXT(
    "Chinese military officer, officer uniform, confident posture, "
    "educated, leadership qualities, formal bearing, military academy training, "
    "decorations, command presence, strategic thinking"
);

const FString UMingRepublicEraPromptLibrary::CivilianDescription = TEXT(
    "Chinese civilian, everyday clothing, realistic expression, "
    "working class, merchant class, intellectual, farmer, "
    "traditional lifestyle, cultural authenticity, period-accurate"
);

const FString UMingRepublicEraPromptLibrary::ScholarDescription = TEXT(
    "Chinese scholar, traditional robes, intellectual appearance, "
    "educated, refined, glasses, books, calligraphy, "
    "cultural preservation, traditional values, academic"
);

const FString UMingRepublicEraPromptLibrary::MerchantDescription = TEXT(
    "Chinese merchant, business attire, prosperous appearance, "
    "Western-influenced clothing, shop owner, trader, "
    "commercial success, modern business practices, urban"
);

// 建築類型描述
const FString UMingRepublicEraPromptLibrary::GovernmentBuildingDescription = TEXT(
    "government building, official architecture, republican style, "
    "national symbols, formal design, stone construction, "
    "administrative center, political power, monumental"
);

const FString UMingRepublicEraPromptLibrary::TraditionalHouseDescription = TEXT(
    "traditional Chinese house, courtyard design, wooden construction, "
    "curved roofs, intricate details, family residence, "
    "cultural heritage, historical architecture, authentic"
);

const FString UMingRepublicEraPromptLibrary::WesternStyleBuildingDescription = TEXT(
    "Western-style building, colonial architecture, Art Deco influence, "
    "modern construction, foreign concession, international style, "
    "urban development, cosmopolitan atmosphere"
);

const FString UMingRepublicEraPromptLibrary::MilitaryFacilityDescription = TEXT(
    "military facility, barracks, fortification, defensive structure, "
    "strategic location, functional design, military architecture, "
    "combat readiness, wartime construction"
);

const FString UMingRepublicEraPromptLibrary::TempleDescription = TEXT(
    "Chinese temple, traditional architecture, religious building, "
    "spiritual atmosphere, ornate decorations, cultural significance, "
    "historical importance, worship space, authentic"
);

// 武器類型描述
const FString UMingRepublicEraPromptLibrary::RifleDescription = TEXT(
    "rifle, military firearm, Chinese army weapon, bolt-action rifle, "
    "Mauser design, historical accuracy, military standard issue, "
    "combat weapon, period-accurate, detailed"
);

const FString UMingRepublicEraPromptLibrary::PistolDescription = TEXT(
    "pistol, sidearm, officer weapon, handgun, military pistol, "
    "personal defense, detailed craftsmanship, historical model, "
    "period firearm, authentic"
);

const FString UMingRepublicEraPromptLibrary::MachineGunDescription = TEXT(
    "machine gun, heavy weapon, crew-served weapon, military firepower, "
    "bipod mount, ammunition belt, defensive weapon, "
    "battlefield effectiveness, historical model"
);

const FString UMingRepublicEraPromptLibrary::ArtilleryDescription = TEXT(
    "artillery piece, cannon, heavy weapon, military gun, "
    "long-range weapon, field artillery, combat support, "
    "historical artillery, detailed design"
);

const FString UMingRepublicEraPromptLibrary::SwordDescription = TEXT(
    "sword, traditional Chinese weapon, ceremonial blade, "
    "military sword, officer weapon, detailed craftsmanship, "
    "historical design, cultural significance"
);

// 音樂風格描述
const FString UMingRepublicEraPromptLibrary::PatrioticMusicDescription = TEXT(
    "patriotic music, national anthem style, orchestral arrangement, "
    "march rhythm, triumphant melody, Chinese instruments, "
    "nationalistic themes, inspiring, heroic"
);

const FString UMingRepublicEraPromptLibrary::DramaticMusicDescription = TEXT(
    "dramatic music, emotional score, cinematic quality, "
    "orchestral depth, tension building, narrative support, "
    "historical drama, atmospheric, evocative"
);

const FString UMingRepublicEraPromptLibrary::TraditionalMusicDescription = TEXT(
    "traditional Chinese music, classical instruments, cultural heritage, "
    "erhu, guzheng, pipa, dizi, pentatonic scale, "
    "historical authenticity, cultural preservation"
);

const FString UMingRepublicEraPromptLibrary::MilitaryMarchDescription = TEXT(
    "military march, marching band, rhythmic precision, "
    "drum corps, brass instruments, military discipline, "
    "patriotic melody, ceremonial, formal"
);

// 場景類型描述
const FString UMingRepublicEraPromptLibrary::UrbanSceneDescription = TEXT(
    "urban scene, city street, bustling activity, Chinese city, "
    "period architecture, street life, commercial activity, "
    "historical urban landscape, authentic atmosphere"
);

const FString UMingRepublicEraPromptLibrary::RuralSceneDescription = TEXT(
    "rural scene, countryside, agricultural landscape, "
    "traditional farming, village life, natural beauty, "
    "historical rural China, peaceful, authentic"
);

const FString UMingRepublicEraPromptLibrary::BattlefieldDescription = TEXT(
    "battlefield, war zone, combat scene, military engagement, "
    "destruction, smoke, explosions, military conflict, "
    "wartime atmosphere, dramatic, historical"
);

const FString UMingRepublicEraPromptLibrary::StreetSceneDescription = TEXT(
    "street scene, daily life, market street, urban activity, "
    "period vehicles, pedestrians, shop fronts, "
    "historical street, authentic detail, cultural atmosphere"
);

UMingRepublicEraPromptLibrary::UMingRepublicEraPromptLibrary()
{
}

FString UMingRepublicEraPromptLibrary::GetEraStyleDescription(ERepublicEraStyle EraStyle)
{
    switch (EraStyle)
    {
    case ERepublicEraStyle::EarlyRepublic:
        return EarlyRepublicContext;
        
    case ERepublicEraStyle::NanjingDecade:
        return NanjingDecadeContext;
        
    case ERepublicEraStyle::WarOfResistance:
        return WarOfResistanceContext;
        
    case ERepublicEraStyle::CivilWar:
        return CivilWarContext;
        
    default:
        return TEXT("");
    }
}

FString UMingRepublicEraPromptLibrary::GetCharacterPortraitPrompt(ERepublicEraStyle EraStyle, const FString& CharacterType)
{
    FString BasePrompt = GetEraStyleDescription(EraStyle);
    FString CharacterDesc;
    
    if (CharacterType.Contains(TEXT("soldier")))
    {
        CharacterDesc = SoldierDescription;
    }
    else if (CharacterType.Contains(TEXT("officer")))
    {
        CharacterDesc = OfficerDescription;
    }
    else if (CharacterType.Contains(TEXT("scholar")))
    {
        CharacterDesc = ScholarDescription;
    }
    else if (CharacterType.Contains(TEXT("merchant")))
    {
        CharacterDesc = MerchantDescription;
    }
    else
    {
        CharacterDesc = CivilianDescription;
    }
    
    return BasePrompt + TEXT(", ") + CharacterDesc + TEXT(", character portrait, realistic, detailed face, historical accuracy");
}

FString UMingRepublicEraPromptLibrary::GetBuildingPrompt(ERepublicEraStyle EraStyle, const FString& BuildingType)
{
    FString BasePrompt = GetEraStyleDescription(EraStyle);
    FString BuildingDesc;
    
    if (BuildingType.Contains(TEXT("government")))
    {
        BuildingDesc = GovernmentBuildingDescription;
    }
    else if (BuildingType.Contains(TEXT("traditional")))
    {
        BuildingDesc = TraditionalHouseDescription;
    }
    else if (BuildingType.Contains(TEXT("western")))
    {
        BuildingDesc = WesternStyleBuildingDescription;
    }
    else if (BuildingType.Contains(TEXT("military")))
    {
        BuildingDesc = MilitaryFacilityDescription;
    }
    else if (BuildingType.Contains(TEXT("temple")))
    {
        BuildingDesc = TempleDescription;
    }
    else
    {
        BuildingDesc = TraditionalHouseDescription;
    }
    
    return BasePrompt + TEXT(", ") + BuildingDesc + TEXT(", architecture, detailed, historical accuracy");
}

FString UMingRepublicEraPromptLibrary::GetWeaponPrompt(ERepublicEraStyle EraStyle, const FString& WeaponType)
{
    FString BasePrompt = GetEraStyleDescription(EraStyle);
    FString WeaponDesc;
    
    if (WeaponType.Contains(TEXT("rifle")))
    {
        WeaponDesc = RifleDescription;
    }
    else if (WeaponType.Contains(TEXT("pistol")))
    {
        WeaponDesc = PistolDescription;
    }
    else if (WeaponType.Contains(TEXT("machine gun")))
    {
        WeaponDesc = MachineGunDescription;
    }
    else if (WeaponType.Contains(TEXT("artillery")))
    {
        WeaponDesc = ArtilleryDescription;
    }
    else if (WeaponType.Contains(TEXT("sword")))
    {
        WeaponDesc = SwordDescription;
    }
    else
    {
        WeaponDesc = RifleDescription;
    }
    
    return BasePrompt + TEXT(", ") + WeaponDesc + TEXT(", detailed, historical weapon, authentic");
}

FString UMingRepublicEraPromptLibrary::GetUniformPrompt(ERepublicEraStyle EraStyle, const FString& MilitaryBranch)
{
    FString BasePrompt = GetEraStyleDescription(EraStyle);
    
    return BasePrompt + TEXT(", military uniform, Chinese army, period-accurate costume, detailed fabric, authentic design");
}

FString UMingRepublicEraPromptLibrary::GetMusicPrompt(ERepublicEraStyle EraStyle, const FString& MusicMood)
{
    FString BasePrompt = GetEraStyleDescription(EraStyle);
    FString MusicDesc;
    
    if (MusicMood.Contains(TEXT("patriotic")))
    {
        MusicDesc = PatrioticMusicDescription;
    }
    else if (MusicMood.Contains(TEXT("dramatic")))
    {
        MusicDesc = DramaticMusicDescription;
    }
    else if (MusicMood.Contains(TEXT("traditional")))
    {
        MusicDesc = TraditionalMusicDescription;
    }
    else if (MusicMood.Contains(TEXT("march")))
    {
        MusicDesc = MilitaryMarchDescription;
    }
    else
    {
        MusicDesc = PatrioticMusicDescription;
    }
    
    return BasePrompt + TEXT(", ") + MusicDesc;
}

FString UMingRepublicEraPromptLibrary::GetScenePrompt(ERepublicEraStyle EraStyle, const FString& SceneType)
{
    FString BasePrompt = GetEraStyleDescription(EraStyle);
    FString SceneDesc;
    
    if (SceneType.Contains(TEXT("urban")))
    {
        SceneDesc = UrbanSceneDescription;
    }
    else if (SceneType.Contains(TEXT("rural")))
    {
        SceneDesc = RuralSceneDescription;
    }
    else if (SceneType.Contains(TEXT("battlefield")))
    {
        SceneDesc = BattlefieldDescription;
    }
    else if (SceneType.Contains(TEXT("street")))
    {
        SceneDesc = StreetSceneDescription;
    }
    else
    {
        SceneDesc = UrbanSceneDescription;
    }
    
    return BasePrompt + TEXT(", ") + SceneDesc + TEXT(", detailed scene, historical accuracy");
}

FString UMingRepublicEraPromptLibrary::GetQualityEnhancers()
{
    return TEXT("high quality, detailed, photorealistic, 8k, professional photography, sharp focus, dramatic lighting, historical accuracy, authentic");
}

FString UMingRepublicEraPromptLibrary::GetNegativePrompts()
{
    return TEXT("modern, contemporary, anachronistic, low quality, blurry, distorted, unrealistic, fantasy, sci-fi, cartoon, anime, 3d render, digital art, painting, sketch");
}

FString UMingRepublicEraPromptLibrary::GetFullPrompt(ERepublicEraStyle EraStyle, EAIAssetType AssetType, const FString& SpecificDescription)
{
    FString BasePrompt = GetEraStyleDescription(EraStyle);
    FString TypePrompt;
    FString QualityPrompt = GetQualityEnhancers();
    
    switch (AssetType)
    {
    case EAIAssetType::CharacterPortrait:
        TypePrompt = GetCharacterPortraitPrompt(EraStyle, SpecificDescription);
        break;
        
    case EAIAssetType::Building:
        TypePrompt = GetBuildingPrompt(EraStyle, SpecificDescription);
        break;
        
    case EAIAssetType::Weapon:
        TypePrompt = GetWeaponPrompt(EraStyle, SpecificDescription);
        break;
        
    case EAIAssetType::Uniform:
        TypePrompt = GetUniformPrompt(EraStyle, SpecificDescription);
        break;
        
    case EAIAssetType::BackgroundMusic:
        TypePrompt = GetMusicPrompt(EraStyle, SpecificDescription);
        break;
        
    case EAIAssetType::Landscape:
        TypePrompt = GetScenePrompt(EraStyle, SpecificDescription);
        break;
        
    default:
        TypePrompt = BasePrompt + TEXT(", ") + SpecificDescription;
        break;
    }
    
    return TypePrompt + TEXT(", ") + QualityPrompt;
}
