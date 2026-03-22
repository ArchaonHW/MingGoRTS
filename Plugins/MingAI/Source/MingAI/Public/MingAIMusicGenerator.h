#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAIMusicGenerator.generated.h"

UENUM(BlueprintType)
enum class EMingMusicGenre: uint8 {
    None UMETA(DisplayName = "None"),
    EpicOrchestral UMETA(DisplayName = "Epic Orchestral"),
    Ambient UMETA(DisplayName = "Ambient"),
    ChineseTraditional UMETA(DisplayName = "Chinese Traditional"),
    Battle UMETA(DisplayName = "Battle"),
    Peaceful UMETA(DisplayName = "Peaceful"),
    Suspense UMETA(DisplayName = "Suspense"),
    Victory UMETA(DisplayName = "Victory"),
    Defeat UMETA(DisplayName = "Defeat"),
    Exploration UMETA(DisplayName = "Exploration"),
    Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMingSoundEffectCategory: uint8 {
    None UMETA(DisplayName = "None"),
    UI UMETA(DisplayName = "UI"),
    Combat UMETA(DisplayName = "Combat"),
    Environment UMETA(DisplayName = "Environment"),
    Footsteps UMETA(DisplayName = "Footsteps"),
    Magic UMETA(DisplayName = "Magic"),
    Mechanical UMETA(DisplayName = "Mechanical"),
    Nature UMETA(DisplayName = "Nature"),
    Weather UMETA(DisplayName = "Weather"),
    Building UMETA(DisplayName = "Building"),
    Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FMingMusicGenerationConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Music Generation")
    EMingMusicGenre Genre;

    UPROPERTY(BlueprintReadWrite, Category = "Music Generation")
    float Duration;

    UPROPERTY(BlueprintReadWrite, Category = "Music Generation")
    int32 Tempo;

    UPROPERTY(BlueprintReadWrite, Category = "Music Generation")
    FString Key;

    UPROPERTY(BlueprintReadWrite, Category = "Music Generation")
    bool bIncludeChineseInstruments;

    UPROPERTY(BlueprintReadWrite, Category = "Music Generation")
    bool bLoopable;

    UPROPERTY(BlueprintReadWrite, Category = "Music Generation")
    int32 Variations;

    FMingMusicGenerationConfig()
        : Genre(EMingMusicGenre::EpicOrchestral)
        , Duration(60.0f)
        , Tempo(120)
        , Key(TEXT("C Minor"))
        , bIncludeChineseInstruments(true)
        , bLoopable(false)
        , Variations(1)
    {}
};

USTRUCT(BlueprintType)
struct FMingSoundEffectConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Sound Effect")
    EMingSoundEffectCategory Category;

    UPROPERTY(BlueprintReadWrite, Category = "Sound Effect")
    float Duration;

    UPROPERTY(BlueprintReadWrite, Category = "Sound Effect")
    bool bRandomize;

    UPROPERTY(BlueprintReadWrite, Category = "Sound Effect")
    int32 Variations;

    UPROPERTY(BlueprintReadWrite, Category = "Sound Effect")
    TMap<FString, FString> Parameters;

    FMingSoundEffectConfig()
        : Category(EMingSoundEffectCategory::None)
        , Duration(1.0f)
        , bRandomize(false)
        , Variations(1)
    {}
};

/**
 * AI Music and Sound Effect Generator
 * Specialized system for generating audio content using AI
 */
UCLASS(ClassGroup = (AI, Audio), Blueprintable)
class MINGAI_API UMingAIMusicGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingAIMusicGenerator();

    UFUNCTION(BlueprintCallable, Category = "AI Music Generation")
    void InitializeGenerator();

    // Music Generation
    UFUNCTION(BlueprintCallable, Category = "AI Music Generation")
    FGuid GenerateBackgroundMusic(const FString& SceneDescription, EMingMusicGenre Genre, float Duration);

    UFUNCTION(BlueprintCallable, Category = "AI Music Generation")
    FGuid GenerateBattleMusic(float Intensity, float Duration);

    UFUNCTION(BlueprintCallable, Category = "AI Music Generation")
    FGuid GenerateAmbientMusic(const FString& EnvironmentType, float Duration, bool bDaytime);

    UFUNCTION(BlueprintCallable, Category = "AI Music Generation")
    FGuid GenerateVictoryMusic(float Duration);

    UFUNCTION(BlueprintCallable, Category = "AI Music Generation")
    FGuid GenerateDefeatMusic(float Duration);

    UFUNCTION(BlueprintCallable, Category = "AI Music Generation")
    FGuid GenerateChineseTraditionalMusic(const FString& Mood, float Duration);

    // Sound Effect Generation
    UFUNCTION(BlueprintCallable, Category = "AI Sound Effects")
    FGuid GenerateUIEffect(const FString& EffectType);

    UFUNCTION(BlueprintCallable, Category = "AI Sound Effects")
    FGuid GenerateCombatEffect(const FString& WeaponType, const FString& ImpactType);

    UFUNCTION(BlueprintCallable, Category = "AI Sound Effects")
    FGuid GenerateEnvironmentEffect(const FString& EnvironmentType, const FString& Element);

    UFUNCTION(BlueprintCallable, Category = "AI Sound Effects")
    FGuid GenerateFootstepEffect(const FString& SurfaceType, const FString& MovementType);

    UFUNCTION(BlueprintCallable, Category = "AI Sound Effects")
    FGuid GenerateBuildingEffect(const FString& ActionType, const FString& BuildingMaterial);

    UFUNCTION(BlueprintCallable, Category = "AI Sound Effects")
    FGuid GenerateWeatherEffect(const FString& WeatherType, float Intensity);

    // Voice Generation
    UFUNCTION(BlueprintCallable, Category = "AI Voice Generation")
    FGuid GenerateVoiceLine(const FString& Text, const FString& VoiceID, float Speed, float Pitch);

    UFUNCTION(BlueprintCallable, Category = "AI Voice Generation")
    FGuid GenerateNarration(const FString& Text, float Speed);

    UFUNCTION(BlueprintCallable, Category = "AI Voice Generation")
    FGuid GenerateCharacterVoice(const FString& Text, const FString& CharacterType, const FString& Emotion);

    // Batch Generation
    UFUNCTION(BlueprintCallable, Category = "AI Audio Batch")
    TArray<FGuid> GenerateSoundEffectBatch(EMingSoundEffectCategory Category, const TArray<FString>& Descriptions);

    UFUNCTION(BlueprintCallable, Category = "AI Audio Batch")
    TArray<FGuid> GenerateMusicVariations(const FString& BaseDescription, EMingMusicGenre Genre, int32 Count);

    // Presets
    UFUNCTION(BlueprintPure, Category = "AI Music Generation")
    FMingMusicGenerationConfig GetMusicPreset(const FString& PresetName) const;

    UFUNCTION(BlueprintCallable, Category = "AI Music Generation")
    void SaveMusicPreset(const FString& PresetName, const FMingMusicGenerationConfig& Config);

    UFUNCTION(BlueprintPure, Category = "AI Sound Effects")
    FMingSoundEffectConfig GetSFXPreset(const FString& PresetName) const;

    UFUNCTION(BlueprintCallable, Category = "AI Sound Effects")
    void SaveSFXPreset(const FString& PresetName, const FMingSoundEffectConfig& Config);

protected:
    UPROPERTY()
    TMap<FString, FMingMusicGenerationConfig> MusicPresets;

    UPROPERTY()
    TMap<FString, FMingSoundEffectConfig> SFXPresets;

    void InitializeDefaultPresets();
    FString BuildMusicPrompt(const FString& Description, EMingMusicGenre Genre, const FMingMusicGenerationConfig& Config) const;
    FString BuildSoundEffectPrompt(EMingSoundEffectCategory Category, const FString& Description, const FMingSoundEffectConfig& Config) const;
    FString BuildVoicePrompt(const FString& Text, const FString& VoiceCharacteristics, float Speed, float Pitch) const;
    FString GetGenreKeywords(EMingMusicGenre Genre) const;
    FString GetCategoryKeywords(EMingSoundEffectCategory Category) const;
};

