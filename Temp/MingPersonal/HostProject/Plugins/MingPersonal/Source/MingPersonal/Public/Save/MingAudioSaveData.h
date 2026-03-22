#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAudioSaveData.generated.h"

UENUM(BlueprintType)
enum class EMingAudioCategory : uint8
{
    Master UMETA(DisplayName = "Master"),
    Music UMETA(DisplayName = "Music"),
    SFX UMETA(DisplayName = "SFX"),
    Dialogue UMETA(DisplayName = "Dialogue"),
    Ambient UMETA(DisplayName = "Ambient"),
    UI UMETA(DisplayName = "UI")
};

UENUM(BlueprintType)
enum class EMingAudioOutputDevice : uint8
{
    Default UMETA(DisplayName = "Default"),
    Headphones UMETA(DisplayName = "Headphones"),
    Speakers UMETA(DisplayName = "Speakers"),
    Surround UMETA(DisplayName = "Surround"),
    Stereo UMETA(DisplayName = "Stereo")
};

USTRUCT(BlueprintType)
struct FMingAudioSettingsEntry
{
    GENERATED_BODY()

    UPROPERTY()
    EMingAudioCategory Category;

    UPROPERTY()
    float Volume;

    UPROPERTY()
    bool bMuted;

    UPROPERTY()
    bool bEnabled;

    UPROPERTY()
    TMap<FString, float> SubCategoryVolumes;

    FMingAudioSettingsEntry()
        : Category(EMingAudioCategory::Master)
        , Volume(1.0f)
        , bMuted(false)
        , bEnabled(true)
    {}
};

USTRUCT(BlueprintType)
struct FMingAudioPreferences
{
    GENERATED_BODY()

    UPROPERTY()
    EMingAudioOutputDevice OutputDevice;

    UPROPERTY()
    bool bSubtitlesEnabled;

    UPROPERTY()
    bool bSubtitleBackground;

    UPROPERTY()
    float SubtitleSize;

    UPROPERTY()
    FString Language;

    UPROPERTY()
    bool bDolbyAtmosEnabled;

    UPROPERTY()
    bool bSpatialAudioEnabled;

    UPROPERTY()
    bool bDynamicRangeCompression;

    UPROPERTY()
    float AudioLatencyOffset;

    FMingAudioPreferences()
        : OutputDevice(EMingAudioOutputDevice::Default)
        , bSubtitlesEnabled(true)
        , bSubtitleBackground(false)
        , SubtitleSize(1.0f)
        , Language(TEXT("zh-CN"))
        , bDolbyAtmosEnabled(false)
        , bSpatialAudioEnabled(true)
        , bDynamicRangeCompression(false)
        , AudioLatencyOffset(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingRepublicEraAudioPreferences
{
    GENERATED_BODY()

    UPROPERTY()
    bool bEnableCulturalMusic;

    UPROPERTY()
    bool bEnableRegionalAccents;

    UPROPERTY()
    float TraditionalInstrumentVolume;

    UPROPERTY()
    bool bUsePeriodAppropriateAudio;

    UPROPERTY()
    TMap<FName, float> RegionalAudioVolumes;

    FMingRepublicEraAudioPreferences()
        : bEnableCulturalMusic(true)
        , bEnableRegionalAccents(true)
        , TraditionalInstrumentVolume(0.8f)
        , bUsePeriodAppropriateAudio(true)
    {}
};

USTRUCT(BlueprintType)
struct FMingAudioSaveData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 SaveVersion;

    UPROPERTY()
    FDateTime SaveTimestamp;

    UPROPERTY()
    TArray<FMingAudioSettingsEntry> AudioSettings;

    UPROPERTY()
    FMingAudioPreferences AudioPreferences;

    UPROPERTY()
    FMingRepublicEraAudioPreferences RepublicEraPreferences;

    UPROPERTY()
    TMap<FString, FString> SoundBankStates;

    UPROPERTY()
    TMap<FName, float> ActiveMixVolumes;

    UPROPERTY()
    TArray<FString> ActiveAudioThemes;

    UPROPERTY()
    bool bWasPlayingAudio;

    UPROPERTY()
    FName LastPlayedTheme;

    FMingAudioSaveData()
        : SaveVersion(1)
        , bWasPlayingAudio(false)
    {}
};

/**
 * Audio Settings Save Handler
 * Handles saving and loading audio settings
 */
UCLASS(ClassGroup = (Save, Audio))
class MINGPERSONAL_API UMingAudioSaveDataHandler : public UObject
{
    GENERATED_BODY()

public:
    UMingAudioSaveDataHandler();

    // Serialization
    UFUNCTION(BlueprintCallable, Category = "Audio Save")
    TArray<uint8> SerializeAudioSettings(class UMingMetaSoundsSystem* AudioSystem);

    UFUNCTION(BlueprintCallable, Category = "Audio Save")
    bool DeserializeAudioSettings(const TArray<uint8>& Data, UMingMetaSoundsSystem* AudioSystem);

    UFUNCTION(BlueprintCallable, Category = "Audio Save")
    FMingAudioSaveData ExportAudioSettings(UMingMetaSoundsSystem* AudioSystem);

    UFUNCTION(BlueprintCallable, Category = "Audio Save")
    bool ImportAudioSettings(const FMingAudioSaveData& SaveData, UMingMetaSoundsSystem* AudioSystem);

    // Validation
    UFUNCTION(BlueprintPure, Category = "Audio Save")
    bool ValidateAudioData(const TArray<uint8>& Data) const;

    UFUNCTION(BlueprintPure, Category = "Audio Save")
    bool ValidateAudioSaveData(const FMingAudioSaveData& SaveData) const;

    // Settings Helpers
    UFUNCTION(BlueprintCallable, Category = "Audio Save")
    void ApplyDefaultSettings(UMingMetaSoundsSystem* AudioSystem);

    UFUNCTION(BlueprintCallable, Category = "Audio Save")
    void ApplyRepublicEraDefaults(UMingMetaSoundsSystem* AudioSystem);

    UFUNCTION(BlueprintPure, Category = "Audio Save")
    bool AreSettingsEqual(const FMingAudioSaveData& DataA, const FMingAudioSaveData& DataB) const;

    // Utilities
    UFUNCTION(BlueprintPure, Category = "Audio Save")
    int32 GetSerializedDataSize(const UMingMetaSoundsSystem* AudioSystem) const;

    UFUNCTION(BlueprintPure, Category = "Audio Save")
    FString GetSaveDataChecksum(const TArray<uint8>& Data) const;

    // Category Helpers
    UFUNCTION(BlueprintPure, Category = "Audio Save")
    float GetCategoryVolume(const FMingAudioSaveData& SaveData, EMingAudioCategory Category) const;

    UFUNCTION(BlueprintCallable, Category = "Audio Save")
    void SetCategoryVolume(FMingAudioSaveData& SaveData, EMingAudioCategory Category, float Volume);

    UFUNCTION(BlueprintPure, Category = "Audio Save")
    bool IsCategoryMuted(const FMingAudioSaveData& SaveData, EMingAudioCategory Category) const;

    UFUNCTION(BlueprintCallable, Category = "Audio Save")
    void SetCategoryMuted(FMingAudioSaveData& SaveData, EMingAudioCategory Category, bool bMuted);

    // Export/Import
    UFUNCTION(BlueprintCallable, Category = "Audio Save")
    bool ExportToJson(const FMingAudioSaveData& SaveData, FString& OutJson);

    UFUNCTION(BlueprintCallable, Category = "Audio Save")
    bool ImportFromJson(const FString& Json, FMingAudioSaveData& OutSaveData);

    UFUNCTION(BlueprintCallable, Category = "Audio Save")
    bool ExportToFile(const FMingAudioSaveData& SaveData, const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Audio Save")
    bool ImportFromFile(const FString& FilePath, FMingAudioSaveData& OutSaveData);

    // Static Defaults
    UFUNCTION(BlueprintPure, Category = "Audio Save", meta = (StaticFunction))
    static FMingAudioSaveData GetDefaultAudioSettings();

    UFUNCTION(BlueprintPure, Category = "Audio Save", meta = (StaticFunction))
    static FMingAudioSaveData GetRepublicEraAudioSettings();

protected:
    UFUNCTION()
    void BuildAudioSettings(UMingMetaSoundsSystem* AudioSystem, FMingAudioSaveData& OutSaveData);

    UFUNCTION()
    void ApplyAudioSettings(UMingMetaSoundsSystem* AudioSystem, const FMingAudioSaveData& SaveData);

    UFUNCTION()
    void SerializeToBinary(const FMingAudioSaveData& SaveData, TArray<uint8>& OutData);

    UFUNCTION()
    bool DeserializeFromBinary(const TArray<uint8>& Data, FMingAudioSaveData& OutSaveData);

    UFUNCTION()
    EMingAudioCategory AudioCategoryFromString(const FString& CategoryName) const;

    UFUNCTION()
    FString AudioCategoryToString(EMingAudioCategory Category) const;

    UFUNCTION()
    FMingAudioSettingsEntry CreateDefaultEntry(EMingAudioCategory Category) const;
};
