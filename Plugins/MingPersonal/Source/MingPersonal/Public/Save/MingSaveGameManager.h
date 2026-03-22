#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSaveGameManager.generated.h"

UENUM(BlueprintType)
enum class EMingSaveGameResult : uint8
{
    Success UMETA(DisplayName = "Success"),
    Failed_NoSpace UMETA(DisplayName = "Failed - No Space"),
    Failed_CorruptData UMETA(DisplayName = "Failed - Corrupt Data"),
    Failed_VersionMismatch UMETA(DisplayName = "Failed - Version Mismatch"),
    Failed_Encryption UMETA(DisplayName = "Failed - Encryption Error"),
    Failed_FileLocked UMETA(DisplayName = "Failed - File Locked"),
    Failed_Unknown UMETA(DisplayName = "Failed - Unknown Error")
};

USTRUCT(BlueprintType)
struct FMingSaveGameSlotInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 SlotIndex;

    UPROPERTY(BlueprintReadWrite)
    FString SaveName;

    UPROPERTY(BlueprintReadWrite)
    FDateTime SaveDateTime;

    UPROPERTY(BlueprintReadWrite)
    int32 PlayTimeSeconds;

    UPROPERTY(BlueprintReadWrite)
    FString CurrentLevel;

    UPROPERTY(BlueprintReadWrite)
    bool bIsEmpty;

    UPROPERTY(BlueprintReadWrite)
    bool bIsAutosave;

    UPROPERTY(BlueprintReadWrite)
    UTexture2D* ThumbnailImage;

    UPROPERTY(BlueprintReadWrite)
    TMap<FString, FString> Metadata;

    FMingSaveGameSlotInfo()
        : SlotIndex(0)
        , PlayTimeSeconds(0)
        , bIsEmpty(true)
        , bIsAutosave(false)
        , ThumbnailImage(nullptr)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaveGameComplete, EMingSaveGameResult, Result, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLoadGameComplete, EMingSaveGameResult, Result, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeleteGameComplete, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAutosaveTriggered, int32, SlotIndex);

/**
 * Save Game Manager
 * Handles all save/load operations for the Personal System
 */
UCLASS(ClassGroup = (Save), Blueprintable)
class MINGPERSONAL_API UMingSaveGameManager : public UObject
{
    GENERATED_BODY()

public:
    UMingSaveGameManager();

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    void Initialize();

    UFUNCTION(BlueprintPure, Category = "Save Game")
    bool IsInitialized() const { return bIsInitialized; }

    // Save Operations
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    EMingSaveGameResult SaveGame(int32 SlotIndex, const FString& SaveName, bool bIsAutosave = false);

    UFUNCTION(BlueprintCallable, Category = "Save Game")
    EMingSaveGameResult SaveGameWithMetadata(int32 SlotIndex, const FString& SaveName, const TMap<FString, FString>& Metadata);

    UFUNCTION(BlueprintCallable, Category = "Save Game")
    EMingSaveGameResult QuickSave();

    UFUNCTION(BlueprintCallable, Category = "Save Game")
    void SaveGameAsync(int32 SlotIndex, const FString& SaveName);

    // Load Operations
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    EMingSaveGameResult LoadGame(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Save Game")
    void LoadGameAsync(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Save Game")
    bool LoadMostRecentSave();

    // Delete Operations
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    bool DeleteSaveGame(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Save Game")
    bool DeleteAllSaves();

    // Slot Management
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    TArray<FMingSaveGameSlotInfo> GetAllSaveSlots() const;

    UFUNCTION(BlueprintCallable, Category = "Save Game")
    FMingSaveGameSlotInfo GetSaveSlotInfo(int32 SlotIndex) const;

    UFUNCTION(BlueprintPure, Category = "Save Game")
    bool DoesSaveExist(int32 SlotIndex) const;

    UFUNCTION(BlueprintCallable, Category = "Save Game")
    int32 GetNextAvailableSlot() const;

    UFUNCTION(BlueprintPure, Category = "Save Game")
    int32 GetMostRecentSaveSlot() const;

    UFUNCTION(BlueprintPure, Category = "Save Game")
    int32 GetQuickSaveSlot() const { return 0; }

    UFUNCTION(BlueprintPure, Category = "Save Game")
    int32 GetAutoSaveSlot() const { return 1; }

    // Current Save
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    void SetCurrentSaveSlot(int32 SlotIndex);

    UFUNCTION(BlueprintPure, Category = "Save Game")
    int32 GetCurrentSaveSlot() const { return CurrentSaveSlot; }

    UFUNCTION(BlueprintPure, Category = "Save Game")
    UMingSaveGame* GetCurrentSaveGame() const { return CurrentSaveGame; }

    // Autosave
    UFUNCTION(BlueprintCallable, Category = "Autosave")
    void SetAutosaveEnabled(bool bEnabled);

    UFUNCTION(BlueprintPure, Category = "Autosave")
    bool IsAutosaveEnabled() const { return bAutosaveEnabled; }

    UFUNCTION(BlueprintCallable, Category = "Autosave")
    void SetAutosaveInterval(float IntervalMinutes);

    UFUNCTION(BlueprintPure, Category = "Autosave")
    float GetAutosaveInterval() const { return AutosaveIntervalMinutes; }

    UFUNCTION(BlueprintCallable, Category = "Autosave")
    void TriggerAutosave();

    UFUNCTION(BlueprintCallable, Category = "Autosave")
    void CheckAutosaveTimer(float DeltaTime);

    // Data Collection
    UFUNCTION()
    void CollectRelationshipData(UMingSaveGame* SaveGame);

    UFUNCTION()
    void CollectReputationData(UMingSaveGame* SaveGame);

    UFUNCTION()
    void CollectQuestData(UMingSaveGame* SaveGame);

    UFUNCTION()
    void CollectAudioSettings(UMingSaveGame* SaveGame);

    UFUNCTION()
    void CollectUISettings(UMingSaveGame* SaveGame);

    UFUNCTION()
    void CollectGameState(UMingSaveGame* SaveGame);

    // Data Restoration
    UFUNCTION()
    void RestoreRelationshipData(UMingSaveGame* SaveGame);

    UFUNCTION()
    void RestoreReputationData(UMingSaveGame* SaveGame);

    UFUNCTION()
    void RestoreQuestData(UMingSaveGame* SaveGame);

    UFUNCTION()
    void RestoreAudioSettings(UMingSaveGame* SaveGame);

    UFUNCTION()
    void RestoreUISettings(UMingSaveGame* SaveGame);

    UFUNCTION()
    void RestoreGameState(UMingSaveGame* SaveGame);

    // Thumbnails
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    void CaptureSaveThumbnail(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Save Game")
    UTexture2D* LoadSaveThumbnail(int32 SlotIndex) const;

    // Import/Export
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    bool ExportSaveToFile(int32 SlotIndex, const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Save Game")
    bool ImportSaveFromFile(const FString& FilePath, int32 TargetSlotIndex);

    // Validation
    UFUNCTION(BlueprintPure, Category = "Save Game")
    bool ValidateSaveSlot(int32 SlotIndex) const;

    UFUNCTION(BlueprintPure, Category = "Save Game")
    FString GetSaveValidationError() const;

    // Compression & Encryption
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    void SetCompressionEnabled(bool bEnabled);

    UFUNCTION(BlueprintPure, Category = "Save Game")
    bool IsCompressionEnabled() const { return bCompressionEnabled; }

    UFUNCTION(BlueprintCallable, Category = "Save Game")
    void SetEncryptionEnabled(bool bEnabled, const FString& Key);

    UFUNCTION(BlueprintPure, Category = "Save Game")
    bool IsEncryptionEnabled() const { return bEncryptionEnabled; }

    // Backup
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    bool CreateBackup(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Save Game")
    bool RestoreFromBackup(int32 SlotIndex);

    // Event Delegates
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSaveGameComplete OnSaveGameComplete;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLoadGameComplete OnLoadGameComplete;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDeleteGameComplete OnDeleteGameComplete;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnAutosaveTriggered OnAutosaveTriggered;

    // Debug
    UFUNCTION(BlueprintCallable, Category = "Debug")
    void EnableDebugLogging(bool bEnable);

    UFUNCTION(BlueprintPure, Category = "Debug")
    FString GetDebugInfo() const;

protected:
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    int32 CurrentSaveSlot;

    UPROPERTY()
    UMingSaveGame* CurrentSaveGame;

    UPROPERTY()
    bool bAutosaveEnabled;

    UPROPERTY()
    float AutosaveIntervalMinutes;

    UPROPERTY()
    float AutosaveTimer;

    UPROPERTY()
    bool bCompressionEnabled;

    UPROPERTY()
    bool bEncryptionEnabled;

    UPROPERTY()
    FString EncryptionKey;

    UPROPERTY()
    bool bDebugLogging;

    UPROPERTY()
    FString LastError;

    UPROPERTY()
    class UMingPersonalManager* PersonalManager;

    UPROPERTY()
    class UMingMetaSoundsSystem* AudioSystem;

    // Internal functions
    FString GetSaveGamePath(int32 SlotIndex) const;
    FString GetThumbnailPath(int32 SlotIndex) const;
    FString GetBackupPath(int32 SlotIndex) const;
    
    void SerializeSaveGame(UMingSaveGame* SaveGame, TArray<uint8>& OutData);
    bool DeserializeSaveGame(const TArray<uint8>& Data, UMingSaveGame* OutSaveGame);
    
    EMingSaveGameResult WriteSaveToDisk(int32 SlotIndex, const TArray<uint8>& Data);
    EMingSaveGameResult ReadSaveFromDisk(int32 SlotIndex, TArray<uint8>& OutData);
    
    void UpdateAutosaveTimer(float DeltaTime);
    void OnAsyncSaveComplete(bool bSuccess);
    void OnAsyncLoadComplete(bool bSuccess);
    
    void LogSaveEvent(const FString& Event, int32 SlotIndex);

public:
    // Static access
    UFUNCTION(BlueprintPure, Category = "Save Game", meta = (WorldContext = "WorldContextObject"))
    static UMingSaveGameManager* GetSaveGameManager(UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "Save Game")
    static FString GetSaveDirectoryPath();

    UFUNCTION(BlueprintPure, Category = "Save Game")
    static int64 GetAvailableDiskSpace();

    UFUNCTION(BlueprintPure, Category = "Save Game")
    static int64 GetSaveGameTotalSize();
};
