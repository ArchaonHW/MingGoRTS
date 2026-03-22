#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MingSaveGame.generated.h"

UENUM(BlueprintType)
enum class EMingSaveGameVersion : uint8
{
    Initial = 1,
    Version_1_1 = 2,
    Version_1_2 = 3,
    Current = Version_1_2
};

USTRUCT(BlueprintType)
struct FMingSaveGameHeader
{
    GENERATED_BODY()

    UPROPERTY()
    FString SaveGameName;

    UPROPERTY()
    FString PlayerName;

    UPROPERTY()
    FDateTime SaveDateTime;

    UPROPERTY()
    EMingSaveGameVersion Version;

    UPROPERTY()
    int32 PlayTimeSeconds;

    UPROPERTY()
    FString CurrentLevel;

    UPROPERTY()
    int32 SaveSlotIndex;

    UPROPERTY()
    FString ThumbnailImagePath;

    UPROPERTY()
    TMap<FString, FString> Metadata;

    FMingSaveGameHeader()
        : Version(EMingSaveGameVersion::Current)
        , PlayTimeSeconds(0)
        , SaveSlotIndex(0)
    {}
};

/**
 * Main Save Game class for MingGoRTS
 * Contains all persistent data
 */
UCLASS(ClassGroup = (Save), Blueprintable)
class MINGPERSONAL_API UMingSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UMingSaveGame(};

    // Header Information
    UPROPERTY(VisibleAnywhere, Category = "Save Info")
    FMingSaveGameHeader Header;

    // Relationship Data
    UPROPERTY(VisibleAnywhere, Category = "Relationship Data")
    TArray<uint8> RelationshipData;

    // Reputation Data
    UPROPERTY(VisibleAnywhere, Category = "Reputation Data")
    TArray<uint8> ReputationData;

    // Quest Data
    UPROPERTY(VisibleAnywhere, Category = "Quest Data")
    TArray<uint8> QuestData;

    // Audio Settings
    UPROPERTY(VisibleAnywhere, Category = "Audio Settings")
    TMap<FString, float> AudioSettings;

    // UI Settings
    UPROPERTY(VisibleAnywhere, Category = "UI Settings")
    TMap<FString, FString> UISettings;

    // Game State
    UPROPERTY(VisibleAnywhere, Category = "Game State")
    TMap<FString, FString> GameStateData;

    // Player Preferences
    UPROPERTY(VisibleAnywhere, Category = "Preferences")
    TMap<FString, FString> PlayerPreferences;

    // Version compatibility
    UFUNCTION()
    bool IsCompatibleVersion() const;

    UFUNCTION()
    void UpgradeToCurrentVersion(};

    // Serialization helpers
    UFUNCTION()
    void SetRelationshipData(const TArray<uint8>& Data};

    UFUNCTION()
    TArray<uint8> GetRelationshipData() const;

    UFUNCTION()
    void SetReputationData(const TArray<uint8>& Data};

    UFUNCTION()
    TArray<uint8> GetReputationData() const;

    UFUNCTION()
    void SetQuestData(const TArray<uint8>& Data};

    UFUNCTION()
    TArray<uint8> GetQuestData() const;

    UFUNCTION()
    void SetAudioSetting(const FString& SettingName, float Value};

    UFUNCTION()
    float GetAudioSetting(const FString& SettingName, float DefaultValue = 1.0f) const;

    UFUNCTION()
    void SetUISetting(const FString& SettingName, const FString& Value};

    UFUNCTION()
    FString GetUISetting(const FString& SettingName, const FString& DefaultValue = TEXT("")) const;

    UFUNCTION()
    void SetGameStateData(const FString& Key, const FString& Value};

    UFUNCTION()
    FString GetGameStateData(const FString& Key, const FString& DefaultValue = TEXT("")) const;

    UFUNCTION()
    void SetPlayerPreference(const FString& Key, const FString& Value};

    UFUNCTION()
    FString GetPlayerPreference(const FString& Key, const FString& DefaultValue = TEXT("")) const;

    // Metadata helpers
    UFUNCTION()
    void SetMetadata(const FString& Key, const FString& Value};

    UFUNCTION()
    FString GetMetadata(const FString& Key, const FString& DefaultValue = TEXT("")) const;

    // Play time tracking
    UFUNCTION()
    void UpdatePlayTime(int32 AdditionalSeconds};

    UFUNCTION()
    FString GetFormattedPlayTime() const;

    // Save validation
    UFUNCTION()
    bool ValidateSaveData() const;

    UFUNCTION()
    TArray<FString> GetValidationErrors() const;

    // Compression
    UFUNCTION()
    void CompressData(};

    UFUNCTION()
    void DecompressData(};

    // Encryption (optional)
    UFUNCTION()
    void EncryptData(const FString& EncryptionKey};

    UFUNCTION()
    void DecryptData(const FString& EncryptionKey};

    // Static factory
    UFUNCTION(BlueprintCallable, Category = "Save Game", meta = (WorldContext = "WorldContextObject"))
    static UMingSaveGame* CreateSaveGame(UObject* WorldContextObject, const FString& SaveName, int32 SlotIndex};

    UFUNCTION(BlueprintPure, Category = "Save Game")
    static FString GetSaveSlotName(int32 SlotIndex};

    UFUNCTION(BlueprintPure, Category = "Save Game")
    static int32 GetMaxSaveSlots(};

    // Migration helpers
    UFUNCTION()
    static bool MigrateFromVersion1(const UMingSaveGame* OldSave, UMingSaveGame* NewSave};

    UFUNCTION()
    static bool MigrateFromVersion2(const UMingSaveGame* OldSave, UMingSaveGame* NewSave};
};

