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
    Version_2_0 = 4,
    Current = Version_2_0
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
    UMingSaveGame();

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

    // 新系統數據 - 派系管理
    UPROPERTY(VisibleAnywhere, Category = "Faction System")
    TArray<uint8> FactionSystemData;

    // 新系統數據 - 動態歷史
    UPROPERTY(VisibleAnywhere, Category = "History System")
    TArray<uint8> DynamicHistoryData;

    // 新系統數據 - 自學習系統
    UPROPERTY(VisibleAnywhere, Category = "Learning System")
    TArray<uint8> SelfLearningData;

    // 新系統數據 - 場景生成器
    UPROPERTY(VisibleAnywhere, Category = "Scene Generator")
    TArray<uint8> SceneGeneratorData;

    // 新系統數據 - 資產生成器
    UPROPERTY(VisibleAnywhere, Category = "Asset Generator")
    TArray<uint8> AssetGeneratorData;

    // 新系統數據 - 本地化系統
    UPROPERTY(VisibleAnywhere, Category = "Localization System")
    TArray<uint8> LocalizationData;

    // 新系統數據 - 性能系統
    UPROPERTY(VisibleAnywhere, Category = "Performance System")
    TArray<uint8> PerformanceData;

    // 新系統數據 - UI增強系統
    UPROPERTY(VisibleAnywhere, Category = "UI Enhanced System")
    TArray<uint8> UIEnhancedData;

    // 新系統數據 - 音頻增強系統
    UPROPERTY(VisibleAnywhere, Category = "Audio Enhanced System")
    TArray<uint8> AudioEnhancedData;

    // Version compatibility
    UFUNCTION()
    bool IsCompatibleVersion() const;

    UFUNCTION()
    void UpgradeToCurrentVersion();

    // Serialization helpers
    UFUNCTION()
    void SetRelationshipData(const TArray<uint8>& Data);

    UFUNCTION()
    TArray<uint8> GetRelationshipData() const;

    UFUNCTION()
    void SetReputationData(const TArray<uint8>& Data);

    UFUNCTION()
    TArray<uint8> GetReputationData() const;

    UFUNCTION()
    void SetQuestData(const TArray<uint8>& Data);

    UFUNCTION()
    TArray<uint8> GetQuestData() const;

    UFUNCTION()
    void SetAudioSetting(const FString& SettingName, float Value);

    UFUNCTION()
    float GetAudioSetting(const FString& SettingName, float DefaultValue = 1.0f) const;

    UFUNCTION()
    void SetUISetting(const FString& SettingName, const FString& Value);

    UFUNCTION()
    FString GetUISetting(const FString& SettingName, const FString& DefaultValue = TEXT("")) const;

    UFUNCTION()
    void SetGameStateData(const FString& Key, const FString& Value);

    UFUNCTION()
    FString GetGameStateData(const FString& Key, const FString& DefaultValue = TEXT("")) const;

    UFUNCTION()
    void SetPlayerPreference(const FString& Key, const FString& Value);

    UFUNCTION()
    FString GetPlayerPreference(const FString& Key, const FString& DefaultValue = TEXT("")) const;

    // Metadata helpers
    UFUNCTION()
    void SetMetadata(const FString& Key, const FString& Value);

    UFUNCTION()
    FString GetMetadata(const FString& Key, const FString& DefaultValue = TEXT("")) const;

    // Play time tracking
    UFUNCTION()
    void UpdatePlayTime(int32 AdditionalSeconds);

    UFUNCTION()
    FString GetFormattedPlayTime() const;

    // Save validation
    UFUNCTION()
    bool ValidateSaveData() const;

    UFUNCTION()
    TArray<FString> GetValidationErrors() const;

    // Compression
    UFUNCTION()
    void CompressData();

    UFUNCTION()
    void DecompressData();

    // Encryption (optional)
    UFUNCTION()
    void EncryptData(const FString& EncryptionKey);

    UFUNCTION()
    void DecryptData(const FString& EncryptionKey);

    // Static factory
    UFUNCTION(BlueprintCallable, Category = "Save Game", meta = (WorldContext = "WorldContextObject"))
    static UMingSaveGame* CreateSaveGame(UObject* WorldContextObject, const FString& SaveName, int32 SlotIndex);

    UFUNCTION(BlueprintPure, Category = "Save Game")
    static FString GetSaveSlotName(int32 SlotIndex);

    UFUNCTION(BlueprintPure, Category = "Save Game")
    static int32 GetMaxSaveSlots();

    // Migration helpers
    UFUNCTION()
    static bool MigrateFromVersion1(const UMingSaveGame* OldSave, UMingSaveGame* NewSave);

    UFUNCTION()
    static bool MigrateFromVersion2(const UMingSaveGame* OldSave, UMingSaveGame* NewSave);

    UFUNCTION()
    static bool MigrateFromVersion1_2(const UMingSaveGame* OldSave, UMingSaveGame* NewSave);

    // 新系統數據接口
    UFUNCTION()
    void SetFactionSystemData(const TArray<uint8>& Data);

    UFUNCTION()
    TArray<uint8> GetFactionSystemData() const;

    UFUNCTION()
    void SetDynamicHistoryData(const TArray<uint8>& Data);

    UFUNCTION()
    TArray<uint8> GetDynamicHistoryData() const;

    UFUNCTION()
    void SetSelfLearningData(const TArray<uint8>& Data);

    UFUNCTION()
    TArray<uint8> GetSelfLearningData() const;

    UFUNCTION()
    void SetSceneGeneratorData(const TArray<uint8>& Data);

    UFUNCTION()
    TArray<uint8> GetSceneGeneratorData() const;

    UFUNCTION()
    void SetAssetGeneratorData(const TArray<uint8>& Data);

    UFUNCTION()
    TArray<uint8> GetAssetGeneratorData() const;

    UFUNCTION()
    void SetLocalizationData(const TArray<uint8>& Data);

    UFUNCTION()
    TArray<uint8> GetLocalizationData() const;

    UFUNCTION()
    void SetPerformanceData(const TArray<uint8>& Data);

    UFUNCTION()
    TArray<uint8> GetPerformanceData() const;

    UFUNCTION()
    void SetUIEnhancedData(const TArray<uint8>& Data);

    UFUNCTION()
    TArray<uint8> GetUIEnhancedData() const;

    UFUNCTION()
    void SetAudioEnhancedData(const TArray<uint8>& Data);

    UFUNCTION()
    TArray<uint8> GetAudioEnhancedData() const;

    // 系統集成保存/加載
    UFUNCTION(BlueprintCallable, Category = "System Integration")
    bool SaveAllSystemsData();

    UFUNCTION(BlueprintCallable, Category = "System Integration")
    bool LoadAllSystemsData();

    // 數據驗證
    UFUNCTION()
    bool ValidateNewSystemsData() const;
};
