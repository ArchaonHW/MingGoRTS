#include "MingAIMusicGenerator.h"
#include "MingAIGeneratedContentSystem.h"

UMingAIMusicGenerator::UMingAIMusicGenerator()
{
}

void UMingAIMusicGenerator::InitializeGenerator()
{
    InitializeDefaultPresets();
}

void UMingAIMusicGenerator::InitializeDefaultPresets()
{
    // Battle Music Preset
    FMingMusicGenerationConfig BattleConfig;
    BattleConfig.Genre = EMingMusicGenre::Battle;
    BattleConfig.Duration = 120.0f;
    BattleConfig.Tempo = 140;
    BattleConfig.Key = TEXT("D Minor");
    BattleConfig.bIncludeChineseInstruments = true;
    BattleConfig.bLoopable = true;
    BattleConfig.Variations = 1;
    MusicPresets.Add(TEXT("Battle"), BattleConfig);

    // Ambient Music Preset
    FMingMusicGenerationConfig AmbientConfig;
    AmbientConfig.Genre = EMingMusicGenre::Ambient;
    AmbientConfig.Duration = 180.0f;
    AmbientConfig.Tempo = 80;
    AmbientConfig.Key = TEXT("C Major");
    AmbientConfig.bIncludeChineseInstruments = true;
    AmbientConfig.bLoopable = true;
    AmbientConfig.Variations = 1;
    MusicPresets.Add(TEXT("Ambient"), AmbientConfig);

    // Victory Music Preset
    FMingMusicGenerationConfig VictoryConfig;
    VictoryConfig.Genre = EMingMusicGenre::Victory;
    VictoryConfig.Duration = 30.0f;
    VictoryConfig.Tempo = 120;
    VictoryConfig.Key = TEXT("C Major");
    VictoryConfig.bIncludeChineseInstruments = true;
    VictoryConfig.bLoopable = false;
    VictoryConfig.Variations = 1;
    MusicPresets.Add(TEXT("Victory"), VictoryConfig);

    // Exploration Music Preset
    FMingMusicGenerationConfig ExplorationConfig;
    ExplorationConfig.Genre = EMingMusicGenre::Exploration;
    ExplorationConfig.Duration = 150.0f;
    ExplorationConfig.Tempo = 100;
    ExplorationConfig.Key = TEXT("G Major");
    ExplorationConfig.bIncludeChineseInstruments = true;
    ExplorationConfig.bLoopable = true;
    ExplorationConfig.Variations = 1;
    MusicPresets.Add(TEXT("Exploration"), ExplorationConfig);

    // Combat SFX Preset
    FMingSoundEffectConfig CombatSFX;
    CombatSFX.Category = EMingSoundEffectCategory::Combat;
    CombatSFX.Duration = 1.0f;
    CombatSFX.bRandomize = true;
    CombatSFX.Variations = 3;
    SFXPresets.Add(TEXT("Combat"), CombatSFX);

    // UI SFX Preset
    FMingSoundEffectConfig UISFX;
    UISFX.Category = EMingSoundEffectCategory::UI;
    UISFX.Duration = 0.5f;
    UISFX.bRandomize = false;
    UISFX.Variations = 1;
    SFXPresets.Add(TEXT("UI"), UISFX);

    // Environment SFX Preset
    FMingSoundEffectConfig EnvSFX;
    EnvSFX.Category = EMingSoundEffectCategory::Environment;
    EnvSFX.Duration = 5.0f;
    EnvSFX.bRandomize = true;
    EnvSFX.Variations = 5;
    SFXPresets.Add(TEXT("Environment"), EnvSFX);
}

FGuid UMingAIMusicGenerator::GenerateBackgroundMusic(const FString& SceneDescription, EMingMusicGenre Genre, float Duration)
{
    FMingMusicGenerationConfig Config;
    Config.Genre = Genre;
    Config.Duration = Duration;

    FString Prompt = BuildMusicPrompt(SceneDescription, Genre, Config);

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Music;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::AIVA;
    Request.QualityLevel = EMingAIQualityLevel::High;
    Request.AdditionalParameters.Add(TEXT("Duration"), FString::Printf(TEXT("%.2f"), Duration));
    Request.AdditionalParameters.Add(TEXT("Genre"), GetGenreKeywords(Genre));

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIMusicGenerator::GenerateBattleMusic(float Intensity, float Duration)
{
    FMingMusicGenerationConfig Config = GetMusicPreset(TEXT("Battle"));
    Config.Duration = Duration;

    FString IntensityStr = Intensity < 0.5f ? TEXT("low") : TEXT("high");
    FString Prompt = FString::Printf(TEXT("Epic battle music, %s intensity, orchestral with Chinese traditional instruments, dramatic, intense, action music, suitable for strategy game combat, %s tempo"),
        *IntensityStr, *FString::FromInt(Config.Tempo));

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Music;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::AIVA;
    Request.QualityLevel = EMingAIQualityLevel::High;
    Request.AdditionalParameters.Add(TEXT("Duration"), FString::Printf(TEXT("%.2f"), Duration));
    Request.AdditionalParameters.Add(TEXT("Intensity"), FString::Printf(TEXT("%.2f"), Intensity));

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIMusicGenerator::GenerateAmbientMusic(const FString& EnvironmentType, float Duration, bool bDaytime)
{
    FMingMusicGenerationConfig Config = GetMusicPreset(TEXT("Ambient"));
    Config.Duration = Duration;

    FString TimeOfDay = bDaytime ? TEXT("daytime") : TEXT("nighttime");
    FString Prompt = FString::Printf(TEXT("Ambient background music for %s during %s, peaceful, atmospheric, traditional Chinese instruments, subtle, relaxing, immersive, suitable for exploration in strategy game"),
        *EnvironmentType, *TimeOfDay);

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Music;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::AIVA;
    Request.QualityLevel = EMingAIQualityLevel::Standard;
    Request.AdditionalParameters.Add(TEXT("Duration"), FString::Printf(TEXT("%.2f"), Duration));
    Request.AdditionalParameters.Add(TEXT("Loopable"), TEXT("true"));

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIMusicGenerator::GenerateVictoryMusic(float Duration)
{
    FMingMusicGenerationConfig Config = GetMusicPreset(TEXT("Victory"));
    Config.Duration = Duration;

    FString Prompt = TEXT("Victory fanfare music, triumphant, heroic, orchestral with Chinese traditional instruments, uplifting, celebration, success theme, suitable for game victory screen");

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Music;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::AIVA;
    Request.QualityLevel = EMingAIQualityLevel::High;
    Request.AdditionalParameters.Add(TEXT("Duration"), FString::Printf(TEXT("%.2f"), Duration));

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIMusicGenerator::GenerateDefeatMusic(float Duration)
{
    FString Prompt = TEXT("Defeat music, somber, melancholic, traditional Chinese instruments, emotional, reflective, suitable for game defeat screen, slow tempo");

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Music;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::AIVA;
    Request.QualityLevel = EMingAIQualityLevel::Standard;
    Request.AdditionalParameters.Add(TEXT("Duration"), FString::Printf(TEXT("%.2f"), Duration));

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIMusicGenerator::GenerateChineseTraditionalMusic(const FString& Mood, float Duration)
{
    FString Prompt = FString::Printf(TEXT("Traditional Chinese music, %s mood, guzheng, erhu, dizi, pipa, authentic ancient Chinese style, historical atmosphere, suitable for Ming dynasty strategy game"),
        *Mood);

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Music;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::AIVA;
    Request.QualityLevel = EMingAIQualityLevel::High;
    Request.AdditionalParameters.Add(TEXT("Duration"), FString::Printf(TEXT("%.2f"), Duration));
    Request.AdditionalParameters.Add(TEXT("Style"), TEXT("Traditional Chinese"));

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIMusicGenerator::GenerateUIEffect(const FString& EffectType)
{
    FMingSoundEffectConfig Config = GetSFXPreset(TEXT("UI"));

    FString Prompt = FString::Printf(TEXT("UI sound effect: %s, clean, modern, subtle, game interface sound, suitable for strategy game menu"),
        *EffectType);

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::SoundEffect;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::Custom;
    Request.QualityLevel = EMingAIQualityLevel::Standard;
    Request.AdditionalParameters.Add(TEXT("Category"), TEXT("UI"));
    Request.AdditionalParameters.Add(TEXT("Duration"), FString::Printf(TEXT("%.2f"), Config.Duration));

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIMusicGenerator::GenerateCombatEffect(const FString& WeaponType, const FString& ImpactType)
{
    FMingSoundEffectConfig Config = GetSFXPreset(TEXT("Combat"));

    FString Prompt = FString::Printf(TEXT("Combat sound effect: %s hitting %s, realistic, impactful, game combat audio, action sound"),
        *WeaponType, *ImpactType);

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::SoundEffect;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::Custom;
    Request.QualityLevel = EMingAIQualityLevel::High;
    Request.AdditionalParameters.Add(TEXT("Category"), TEXT("Combat"));
    Request.AdditionalParameters.Add(TEXT("WeaponType"), WeaponType);
    Request.AdditionalParameters.Add(TEXT("ImpactType"), ImpactType);

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIMusicGenerator::GenerateEnvironmentEffect(const FString& EnvironmentType, const FString& Element)
{
    FMingSoundEffectConfig Config = GetSFXPreset(TEXT("Environment"));

    FString Prompt = FString::Printf(TEXT("Environment sound effect: %s with %s, ambient, atmospheric, immersive, game environment audio, loopable"),
        *EnvironmentType, *Element);

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::SoundEffect;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::Custom;
    Request.QualityLevel = EMingAIQualityLevel::Standard;
    Request.AdditionalParameters.Add(TEXT("Category"), TEXT("Environment"));
    Request.AdditionalParameters.Add(TEXT("Environment"), EnvironmentType);
    Request.AdditionalParameters.Add(TEXT("Element"), Element);

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIMusicGenerator::GenerateFootstepEffect(const FString& SurfaceType, const FString& MovementType)
{
    FString Prompt = FString::Printf(TEXT("Footstep sound effect: %s movement on %s surface, realistic, subtle, game audio"),
        *MovementType, *SurfaceType);

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::SoundEffect;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::Custom;
    Request.QualityLevel = EMingAIQualityLevel::Standard;
    Request.AdditionalParameters.Add(TEXT("Category"), TEXT("Footsteps"));
    Request.AdditionalParameters.Add(TEXT("Surface"), SurfaceType);
    Request.AdditionalParameters.Add(TEXT("Movement"), MovementType);

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIMusicGenerator::GenerateBuildingEffect(const FString& ActionType, const FString& BuildingMaterial)
{
    FString Prompt = FString::Printf(TEXT("Building sound effect: %s %s structure, construction audio, game building mechanics"),
        *ActionType, *BuildingMaterial);

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::SoundEffect;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::Custom;
    Request.QualityLevel = EMingAIQualityLevel::Standard;
    Request.AdditionalParameters.Add(TEXT("Category"), TEXT("Building"));
    Request.AdditionalParameters.Add(TEXT("Action"), ActionType);
    Request.AdditionalParameters.Add(TEXT("Material"), BuildingMaterial);

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIMusicGenerator::GenerateWeatherEffect(const FString& WeatherType, float Intensity)
{
    FString IntensityStr = Intensity < 0.33f ? TEXT("light") : (Intensity < 0.66f ? TEXT("moderate") : TEXT("heavy"));
    FString Prompt = FString::Printf(TEXT("Weather sound effect: %s %s, ambient, atmospheric, game environment audio, loopable"),
        *IntensityStr, *WeatherType);

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::SoundEffect;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::Custom;
    Request.QualityLevel = EMingAIQualityLevel::Standard;
    Request.AdditionalParameters.Add(TEXT("Category"), TEXT("Weather"));
    Request.AdditionalParameters.Add(TEXT("WeatherType"), WeatherType);
    Request.AdditionalParameters.Add(TEXT("Intensity"), FString::Printf(TEXT("%.2f"), Intensity));

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIMusicGenerator::GenerateVoiceLine(const FString& Text, const FString& VoiceID, float Speed, float Pitch)
{
    FString Prompt = BuildVoicePrompt(Text, VoiceID, Speed, Pitch);

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Voice;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::ElevenLabs;
    Request.QualityLevel = EMingAIQualityLevel::High;
    Request.AdditionalParameters.Add(TEXT("VoiceID"), VoiceID);
    Request.AdditionalParameters.Add(TEXT("Speed"), FString::Printf(TEXT("%.2f"), Speed));
    Request.AdditionalParameters.Add(TEXT("Pitch"), FString::Printf(TEXT("%.2f"), Pitch));

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIMusicGenerator::GenerateNarration(const FString& Text, float Speed)
{
    FString Prompt = BuildVoicePrompt(Text, TEXT("narrator, professional, clear"), Speed, 1.0f);

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Voice;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::ElevenLabs;
    Request.QualityLevel = EMingAIQualityLevel::High;
    Request.AdditionalParameters.Add(TEXT("VoiceID"), TEXT("narrator"));
    Request.AdditionalParameters.Add(TEXT("Speed"), FString::Printf(TEXT("%.2f"), Speed));

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

FGuid UMingAIMusicGenerator::GenerateCharacterVoice(const FString& Text, const FString& CharacterType, const FString& Emotion)
{
    FString VoiceCharacteristics = FString::Printf(TEXT("%s character, %s emotion"), *CharacterType, *Emotion);
    FString Prompt = BuildVoicePrompt(Text, VoiceCharacteristics, 1.0f, 1.0f);

    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Voice;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::ElevenLabs;
    Request.QualityLevel = EMingAIQualityLevel::High;
    Request.AdditionalParameters.Add(TEXT("CharacterType"), CharacterType);
    Request.AdditionalParameters.Add(TEXT("Emotion"), Emotion);

    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(Request);
}

TArray<FGuid> UMingAIMusicGenerator::GenerateSoundEffectBatch(EMingSoundEffectCategory Category, const TArray<FString>& Descriptions)
{
    TArray<FGuid> Results;
    for (const FString& Description : Descriptions)
    {
        FMingSoundEffectConfig Config;
        Config.Category = Category;
        Config.Duration = 2.0f;

        FString Prompt = BuildSoundEffectPrompt(Category, Description, Config);

        FMingAIContentRequest Request;
        Request.ContentType = EMingAIContentType::SoundEffect;
        Request.Prompt = Prompt;
        Request.Provider = EMingAIProvider::Custom;
        Request.QualityLevel = EMingAIQualityLevel::Standard;

        UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
        ContentSystem->InitializeSystem();
        FGuid ID = ContentSystem->GenerateContent(Request);
        Results.Add(ID);
    }
    return Results;
}

TArray<FGuid> UMingAIMusicGenerator::GenerateMusicVariations(const FString& BaseDescription, EMingMusicGenre Genre, int32 Count)
{
    TArray<FGuid> Results;
    for (int32 i = 0; i < Count; i++)
    {
        FString VariationDesc = FString::Printf(TEXT("%s variation %d"), *BaseDescription, i + 1);
        FGuid ID = GenerateBackgroundMusic(VariationDesc, Genre, 60.0f);
        Results.Add(ID);
    }
    return Results;
}

FMingMusicGenerationConfig UMingAIMusicGenerator::GetMusicPreset(const FString& PresetName) const
{
    if (MusicPresets.Contains(PresetName))
    {
        return MusicPresets[PresetName];
    }
    return FMingMusicGenerationConfig();
}

void UMingAIMusicGenerator::SaveMusicPreset(const FString& PresetName, const FMingMusicGenerationConfig& Config)
{
    MusicPresets.Add(PresetName, Config);
}

FMingSoundEffectConfig UMingAIMusicGenerator::GetSFXPreset(const FString& PresetName) const
{
    if (SFXPresets.Contains(PresetName))
    {
        return SFXPresets[PresetName];
    }
    return FMingSoundEffectConfig();
}

void UMingAIMusicGenerator::SaveSFXPreset(const FString& PresetName, const FMingSoundEffectConfig& Config)
{
    SFXPresets.Add(PresetName, Config);
}

FString UMingAIMusicGenerator::BuildMusicPrompt(const FString& Description, EMingMusicGenre Genre, const FMingMusicGenerationConfig& Config) const
{
    FString GenreKeywords = GetGenreKeywords(Genre);
    FString ChineseInstruments = Config.bIncludeChineseInstruments ? TEXT("including guzheng, erhu, dizi, traditional Chinese instruments") : TEXT("");
    FString LoopableStr = Config.bLoopable ? TEXT("loopable, seamless loop") : TEXT("");

    return FString::Printf(TEXT("%s, %s, %s, tempo %d, key %s, %s, high quality, game music, suitable for strategy game"),
        *Description, *GenreKeywords, *ChineseInstruments, Config.Tempo, *Config.Key, *LoopableStr);
}

FString UMingAIMusicGenerator::BuildSoundEffectPrompt(EMingSoundEffectCategory Category, const FString& Description, const FMingSoundEffectConfig& Config) const
{
    FString CategoryKeywords = GetCategoryKeywords(Category);
    return FString::Printf(TEXT("%s: %s, %s, duration %.2f seconds, game audio, high quality"),
        *CategoryKeywords, *Description, Config.bRandomize ? TEXT("randomized variations") : TEXT("consistent"), Config.Duration);
}

FString UMingAIMusicGenerator::BuildVoicePrompt(const FString& Text, const FString& VoiceCharacteristics, float Speed, float Pitch) const
{
    return FString::Printf(TEXT("Voice: %s, speaking text: %s, speed %.2f, pitch %.2f, clear, professional, game voiceover"),
        *VoiceCharacteristics, *Text, Speed, Pitch);
}

FString UMingAIMusicGenerator::GetGenreKeywords(EMingMusicGenre Genre) const
{
    switch (Genre)
    {
    case EMingMusicGenre::EpicOrchestral:
        return TEXT("epic orchestral, dramatic, cinematic, intense");
    case EMingMusicGenre::Ambient:
        return TEXT("ambient, atmospheric, subtle, background music");
    case EMingMusicGenre::ChineseTraditional:
        return TEXT("traditional Chinese, ancient, historical, authentic");
    case EMingMusicGenre::Battle:
        return TEXT("battle music, intense, action, combat");
    case EMingMusicGenre::Peaceful:
        return TEXT("peaceful, calm, relaxing, serene");
    case EMingMusicGenre::Suspense:
        return TEXT("suspense, tension, mysterious, dramatic");
    case EMingMusicGenre::Victory:
        return TEXT("victory, triumphant, heroic, celebratory");
    case EMingMusicGenre::Defeat:
        return TEXT("defeat, somber, melancholic, reflective");
    case EMingMusicGenre::Exploration:
        return TEXT("exploration, discovery, adventurous, curious");
    default:
        return TEXT("game music, high quality");
    }
}

FString UMingAIMusicGenerator::GetCategoryKeywords(EMingSoundEffectCategory Category) const
{
    switch (Category)
    {
    case EMingSoundEffectCategory::UI:
        return TEXT("UI sound effect");
    case EMingSoundEffectCategory::Combat:
        return TEXT("combat sound effect");
    case EMingSoundEffectCategory::Environment:
        return TEXT("environment sound effect");
    case EMingSoundEffectCategory::Footsteps:
        return TEXT("footstep sound effect");
    case EMingSoundEffectCategory::Magic:
        return TEXT("magic sound effect");
    case EMingSoundEffectCategory::Mechanical:
        return TEXT("mechanical sound effect");
    case EMingSoundEffectCategory::Nature:
        return TEXT("nature sound effect");
    case EMingSoundEffectCategory::Weather:
        return TEXT("weather sound effect");
    case EMingSoundEffectCategory::Building:
        return TEXT("building sound effect");
    default:
        return TEXT("sound effect");
    }
}
