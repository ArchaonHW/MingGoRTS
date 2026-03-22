#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "MingRTSSaveLoadEnhancedSystem.generated.h"

UENUM(BlueprintType)
enum class ESaveDataType : uint8
{
    GameState      UMETA(DisplayName = "Game State"),
    PlayerData     UMETA(DisplayName = "Player Data"),
    WorldData      UMETA(DisplayName = "World Data"),
    Inventory      UMETA(DisplayName = "Inventory"),
    Quests         UMETA(DisplayName = "Quests"),
    Relationships  UMETA(DisplayName = "Relationships"),
    Campaign       UMETA(DisplayName = "Campaign"),
    Settings       UMETA(DisplayName = "Settings"),
    Statistics     UMETA(DisplayName = "Statistics"),
    Custom         UMETA(DisplayName = "Custom"),
    All            UMETA(DisplayName = "All Data")
};

UENUM(BlueprintType)
enum class ESaveFormat : uint8
{
    Binary         UMETA(DisplayName = "Binary"),
    JSON           UMETA(DisplayName = "JSON"),
    XML            UMETA(DisplayName = "XML"),
    CSV            UMETA(DisplayName = "CSV"),
    Text           UMETA(DisplayName = "Text"),
    Compressed     UMETA(DisplayName = "Compressed"),
    Encrypted      UMETA(DisplayName = "Encrypted"),
    Cloud          UMETA(DisplayName = "Cloud"),
    Database       UMETA(DisplayName = "Database"),
    Custom         UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class ESaveLocation : uint8
{
    Local          UMETA(DisplayName = "Local"),
    Cloud          UMETA(DisplayName = "Cloud"),
    Network        UMETA(DisplayName = "Network"),
    USB            UMETA(DisplayName = "USB"),
    Memory         UMETA(DisplayName = "Memory"),
    Cache          UMETA(DisplayName = "Cache"),
    Temp           UMETA(DisplayName = "Temporary"),
    Backup         UMETA(DisplayName = "Backup"),
    Archive        UMETA(DisplayName = "Archive"),
    Custom         UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class ESaveStatus : uint8
{
    None           UMETA(DisplayName = "None"),
    Saving         UMETA(DisplayName = "Saving"),
    Loading        UMETA(DisplayName = "Loading"),
    Saved          UMETA(DisplayName = "Saved"),
    Loaded         UMETA(DisplayName = "Loaded"),
    Failed         UMETA(DisplayName = "Failed"),
    Corrupted      UMETA(DisplayName = "Corrupted"),
    Missing        UMETA(DisplayName = "Missing"),
    Outdated       UMETA(DisplayName = "Outdated"),
    Locked         UMETA(DisplayName = "Locked")
};

UENUM(BlueprintType)
enum class ESavePriority : uint8
{
    Low            UMETA(DisplayName = "Low"),
    Medium         UMETA(DisplayName = "Medium"),
    High           UMETA(DisplayName = "High"),
    Critical       UMETA(DisplayName = "Critical"),
    Realtime       UMETA(DisplayName = "Realtime"),
    Background     UMETA(DisplayName = "Background"),
    Immediate      UMETA(DisplayName = "Immediate"),
    Deferred       UMETA(DisplayName = "Deferred"),
    Custom         UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct FSaveGameData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    FString SaveID;

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    FString SaveName;

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    ESaveDataType DataType;

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    ESaveFormat Format;

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    ESaveLocation Location;

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    ESaveStatus Status;

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    FString FilePath;

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    TArray<uint8> Data;

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    float Timestamp;

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    int32 Version;

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    int32 Size;

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    FString Checksum;

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    bool bIsCompressed;

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    bool bIsEncrypted;

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    bool bIsAutoSave;

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    bool bIsValid;

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    TMap<FString, FString> Metadata;

    UPROPERTY(BlueprintReadOnly, Category = "Save Game Data")
    TArray<FString> Dependencies;

    FSaveGameData()
    {
        SaveID = TEXT(""};
        SaveName = TEXT(""};
        DataType = ESaveDataType::GameState;
        Format = ESaveFormat::Binary;
        Location = ESaveLocation::Local;
        Status = ESaveStatus::None;
        FilePath = TEXT(""};
        Timestamp = 0.0f;
        Version = 1;
        Size = 0;
        Checksum = TEXT(""};
        bIsCompressed = false;
        bIsEncrypted = false;
        bIsAutoSave = false;
        bIsValid = false;
    }
};

USTRUCT(BlueprintType)
struct FSaveSlot
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Save Slot")
    FString SlotID;

    UPROPERTY(BlueprintReadOnly, Category = "Save Slot")
    FString SlotName;

    UPROPERTY(BlueprintReadOnly, Category = "Save Slot")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "Save Slot")
    FString ThumbnailPath;

    UPROPERTY(BlueprintReadOnly, Category = "Save Slot")
    float PlayTime;

    UPROPERTY(BlueprintReadOnly, Category = "Save Slot")
    FString LevelName;

    UPROPERTY(BlueprintReadOnly, Category = "Save Slot")
    FString PlayerName;

    UPROPERTY(BlueprintReadOnly, Category = "Save Slot")
    int32 Chapter;

    UPROPERTY(BlueprintReadOnly, Category = "Save Slot")
    FString CampaignID;

    UPROPERTY(BlueprintReadOnly, Category = "Save Slot")
    TArray<FString> SaveIDs;

    UPROPERTY(BlueprintReadOnly, Category = "Save Slot")
    float LastModified;

    UPROPERTY(BlueprintReadOnly, Category = "Save Slot")
    bool bIsEmpty;

    UPROPERTY(BlueprintReadOnly, Category = "Save Slot")
    bool bIsCorrupted;

    UPROPERTY(BlueprintReadOnly, Category = "Save Slot")
    bool bIsReadOnly;

    UPROPERTY(BlueprintReadOnly, Category = "Save Slot")
    int32 MaxSaves;

    FSaveSlot()
    {
        SlotID = TEXT(""};
        SlotName = TEXT(""};
        Description = TEXT(""};
        ThumbnailPath = TEXT(""};
        PlayTime = 0.0f;
        LevelName = TEXT(""};
        PlayerName = TEXT(""};
        Chapter = 0;
        CampaignID = TEXT(""};
        LastModified = 0.0f;
        bIsEmpty = true;
        bIsCorrupted = false;
        bIsReadOnly = false;
        MaxSaves = 10;
    }
};

USTRUCT(BlueprintType)
struct FSaveOperation
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Save Operation")
    FString OperationID;

    UPROPERTY(BlueprintReadOnly, Category = "Save Operation")
    FString OperationType; // "Save" or "Load"

    UPROPERTY(BlueprintReadOnly, Category = "Save Operation")
    FString TargetID;

    UPROPERTY(BlueprintReadOnly, Category = "Save Operation")
    ESaveDataType DataType;

    UPROPERTY(BlueprintReadOnly, Category = "Save Operation")
    ESavePriority Priority;

    UPROPERTY(BlueprintReadOnly, Category = "Save Operation")
    float StartTime;

    UPROPERTY(BlueprintReadOnly, Category = "Save Operation")
    float Progress;

    UPROPERTY(BlueprintReadOnly, Category = "Save Operation")
    bool bIsCompleted;

    UPROPERTY(BlueprintReadOnly, Category = "Save Operation")
    bool bIsFailed;

    UPROPERTY(BlueprintReadOnly, Category = "Save Operation")
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly, Category = "Save Operation")
    TMap<FString, FString> OperationData;

    FSaveOperation()
    {
        OperationID = TEXT(""};
        OperationType = TEXT(""};
        TargetID = TEXT(""};
        DataType = ESaveDataType::GameState;
        Priority = ESavePriority::Medium;
        StartTime = 0.0f;
        Progress = 0.0f;
        bIsCompleted = false;
        bIsFailed = false;
        ErrorMessage = TEXT(""};
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveStarted, const FString&, SaveID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaveProgress, const FString&, SaveID, float, Progress};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaveCompleted, const FString&, SaveID, bool, bSuccess};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadStarted, const FString&, SaveID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLoadProgress, const FString&, SaveID, float, Progress};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLoadCompleted, const FString&, SaveID, bool, bSuccess};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveSlotCreated, const FSaveSlot&, SaveSlot};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveSlotDeleted, const FString&, SlotID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAutoSaveTriggered, const FString&, Reason};

/**
 * MingGoRTS Â¢ûÂº∑‰øùÂ??åË??•Á≥ªÁµ? * ?ïÁ??äÊà≤?∏Ê??Ñ‰?Â≠ò„ÄÅË??•„ÄÅÂ?‰ªΩ„ÄÅÊÅ¢Âæ©Á??üËÉΩ
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingRTS")
class MINGPERSONAL_API UMingRTSSaveLoadEnhancedSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSSaveLoadEnhancedSystem(};

    // ?ùÂ??ñÂ?Âº∑‰?Â≠òË??•Á≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void InitializeSaveLoadEnhancedSystem(UWorld* World};

    // ?¥Êñ∞Â¢ûÂº∑‰øùÂ?ËºâÂÖ•Á≥ªÁµ±ÔºàÊ?ÂπÄË™øÁî®Ôº?    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void UpdateSaveLoadEnhancedSystem(float DeltaTime};

    // ‰øùÂ??äÊà≤?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FString SaveGameData(const FString& SaveName, ESaveDataType DataType, const TArray<uint8>& Data, ESaveFormat Format = ESaveFormat::Binary};

    // ËºâÂÖ•?äÊà≤?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool LoadGameData(const FString& SaveID, TArray<uint8>& OutData};

    // ‰øùÂ??∞ÊßΩ‰Ω?    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool SaveToSlot(const FString& SlotName, const FString& Description, const FString& ThumbnailPath = TEXT("")};

    // ÂæûÊßΩ‰ΩçË???    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool LoadFromSlot(const FString& SlotID};

    // ?µÂª∫‰øùÂ?ÊßΩ‰?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FString CreateSaveSlot(const FString& SlotName, const FString& Description, int32 MaxSaves = 10};

    // ?™Èô§‰øùÂ?ÊßΩ‰?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool DeleteSaveSlot(const FString& SlotID};

    // ?≤Â?‰øùÂ?ÊßΩ‰?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FSaveSlot GetSaveSlot(const FString& SlotID) const;

    // ?≤Â??Ä?â‰?Â≠òÊßΩ‰Ω?    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<FSaveSlot> GetAllSaveSlots() const;

    // ?≤Â?‰øùÂ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FSaveGameData GetSaveGameData(const FString& SaveID) const;

    // ?™Â?‰øùÂ?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void AutoSave(const FString& Reason = TEXT("AutoSave")};

    // Ë®≠ÁΩÆ?™Â?‰øùÂ??ìÈ?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void SetAutoSaveInterval(float Interval};

    // ?ô‰ªΩ‰øùÂ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool BackupSaveData(const FString& SaveID, const FString& BackupLocation = TEXT("")};

    // ?¢Âæ©?ô‰ªΩ?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool RestoreBackup(const FString& BackupID};

    // È©óË?‰øùÂ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool ValidateSaveData(const FString& SaveID};

    // ‰øÆÂæ©?çÂ??Ñ‰?Â≠òÊï∏??    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool RepairSaveData(const FString& SaveID};

    // Â£ìÁ∏Æ‰øùÂ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool CompressSaveData(const FString& SaveID};

    // Ëß??Á∏Æ‰?Â≠òÊï∏??    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool DecompressSaveData(const FString& SaveID};

    // ?†Â?‰øùÂ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool EncryptSaveData(const FString& SaveID, const FString& EncryptionKey};

    // Ëß??‰øùÂ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool DecryptSaveData(const FString& SaveID, const FString& EncryptionKey};

    // Â∞éÂá∫‰øùÂ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool ExportSaveData(const FString& SaveID, const FString& ExportPath, ESaveFormat Format = ESaveFormat::JSON};

    // Â∞éÂÖ•‰øùÂ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool ImportSaveData(const FString& ImportPath, ESaveFormat Format = ESaveFormat::JSON};

    // ?≤Â?‰øùÂ?Áµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TMap<FString, float> GetSaveStatistics() const;

    // Ê∏ÖÁ??äÁ?‰øùÂ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void CleanupOldSaveData(float MaxAge = 30.0f};

    // ?™Â?‰øùÂ??ßËÉΩ
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void OptimizeSavePerformance(};

    // Ë®≠ÁΩÆ‰øùÂ??™Â?Á¥?    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void SetSavePriority(const FString& SaveID, ESavePriority Priority};

    // ?πÈ?‰øùÂ?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void BatchSave(const TArray<FString>& SaveIDs};

    // ?πÈ?ËºâÂÖ•
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void BatchLoad(const TArray<FString>& SaveIDs};

public:
    // ‰∫ã‰ª∂ÂßîË?
    UPROPERTY(BlueprintAssignable, Category = "Save Load Enhanced System Events")
    FOnSaveStarted OnSaveStarted;

    UPROPERTY(BlueprintAssignable, Category = "Save Load Enhanced System Events")
    FOnSaveProgress OnSaveProgress;

    UPROPERTY(BlueprintAssignable, Category = "Save Load Enhanced System Events")
    FOnSaveCompleted OnSaveCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Save Load Enhanced System Events")
    FOnLoadStarted OnLoadStarted;

    UPROPERTY(BlueprintAssignable, Category = "Save Load Enhanced System Events")
    FOnLoadProgress OnLoadProgress;

    UPROPERTY(BlueprintAssignable, Category = "Save Load Enhanced System Events")
    FOnLoadCompleted OnLoadCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Save Load Enhanced System Events")
    FOnSaveSlotCreated OnSaveSlotCreated;

    UPROPERTY(BlueprintAssignable, Category = "Save Load Enhanced System Events")
    FOnSaveSlotDeleted OnSaveSlotDeleted;

    UPROPERTY(BlueprintAssignable, Category = "Save Load Enhanced System Events")
    FOnAutoSaveTriggered OnAutoSaveTriggered;

protected:
    // ?ïÁ?‰øùÂ??ç‰?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void ProcessSaveOperations(float DeltaTime};

    // ?ïÁ?ËºâÂÖ•?ç‰?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void ProcessLoadOperations(float DeltaTime};

    // ?ïÁ??™Â?‰øùÂ?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void ProcessAutoSave(float DeltaTime};

    // ?ïÁ??ô‰ªΩ?ç‰?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void ProcessBackupOperations(float DeltaTime};

    // È©óË?‰øùÂ?ÂÆåÊï¥??    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool ValidateSaveIntegrity(const FSaveGameData& SaveData) const;

    // Ë®àÁ??°È???    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FString CalculateChecksum(const TArray<uint8>& Data) const;

    // Â∫èÂ??ñÊï∏??    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<uint8> SerializeData(const TMap<FString, FString>& Data, ESaveFormat Format) const;

    // ?çÂ??óÂ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TMap<FString, FString> DeserializeData(const TArray<uint8>& Data, ESaveFormat Format) const;

    // Â£ìÁ∏Æ?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<uint8> CompressData(const TArray<uint8>& Data) const;

    // Ëß??Á∏ÆÊï∏??    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<uint8> DecompressData(const TArray<uint8>& Data) const;

    // ?†Â??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<uint8> EncryptData(const TArray<uint8>& Data, const FString& Key) const;

    // Ëß???∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<uint8> DecryptData(const TArray<uint8>& Data, const FString& Key) const;

    // ?üÊ?Á∏ÆÁï•??    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FString GenerateThumbnail(const FString& SlotID) const;

    // ?≤Â?‰øùÂ?Ë∑ØÂ?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FString GetSavePath(ESaveLocation Location, const FString& FileName) const;

    // ?µÂª∫‰øùÂ??ÆÈ?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool CreateSaveDirectory(const FString& Path) const;

protected:
    // ?∂Â?‰∏ñÁ?ÂºïÁî®
    UPROPERTY()
    TObjectPtr<UWorld> CurrentWorld;

    // ‰øùÂ??∏Ê??†Â?
    UPROPERTY()
    TMap<FString, FSaveGameData> SaveGameData;

    // ‰øùÂ?ÊßΩ‰??†Â?
    UPROPERTY()
    TMap<FString, FSaveSlot> SaveSlots;

    // ‰øùÂ??ç‰??†Â?
    UPROPERTY()
    TMap<FString, FSaveOperation> SaveOperations;

    // ?∂Â?ÊßΩ‰?ID
    UPROPERTY()
    FString CurrentSlotID;

    // ‰øùÂ??¥Êñ∞?ìÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    float SaveUpdateInterval;

    // ‰∏äÊ¨°‰øùÂ??¥Êñ∞?ÇÈ?
    UPROPERTY()
    float LastSaveUpdateTime;

    // ?™Â?‰øùÂ??ìÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    float AutoSaveInterval;

    // ‰∏äÊ¨°?™Â?‰øùÂ??ÇÈ?
    UPROPERTY()
    float LastAutoSaveTime;

    // ?ÄÂ§ß‰?Â≠òÊï∏??    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    int32 MaxSaveCount;

    // ?ÄÂ§ßÊßΩ‰ΩçÊï∏??    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    int32 MaxSlotCount;

    // ÈªòË?‰øùÂ??ºÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    ESaveFormat DefaultFormat;

    // ÈªòË?‰øùÂ?‰ΩçÁΩÆ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    ESaveLocation DefaultLocation;

    // ?ØÂê¶?üÁî®?™Â?‰øùÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    bool bAutoSaveEnabled;

    // ?ØÂê¶?üÁî®Â£ìÁ∏Æ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    bool bCompressionEnabled;

    // ?ØÂê¶?üÁî®?†Â?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    bool bEncryptionEnabled;

private:
    // ?≤Â?‰øùÂ??∏Ê??çÁ®±
    FString GetSaveDataName(ESaveDataType DataType) const;

    // ?≤Â?‰øùÂ??ºÂ??çÁ®±
    FString GetSaveFormatName(ESaveFormat Format) const;

    // ?≤Â?‰øùÂ?‰ΩçÁΩÆ?çÁ®±
    FString GetSaveLocationName(ESaveLocation Location) const;

    // ?üÊ??Ø‰?ID
    FString GenerateUniqueID(const FString& Prefix) const;

    // È©óË?‰øùÂ??∏Ê?
    bool ValidateSaveData(const FSaveGameData& SaveData) const;

    // È©óË?‰øùÂ?ÊßΩ‰?
    bool ValidateSaveSlot(const FSaveSlot& SaveSlot) const;

    // ?ïÁ?‰øùÂ??ØË™§
    void HandleSaveError(const FString& OperationID, const FString& ErrorMessage};

    // ?ïÁ?ËºâÂÖ•?ØË™§
    void HandleLoadError(const FString& OperationID, const FString& ErrorMessage};

    // Ê∏ÖÁ??°Ê?‰øùÂ??∏Ê?
    void CleanupInvalidSaveData(};

    // Ê∏ÖÁ??°Ê?‰øùÂ?ÊßΩ‰?
    void CleanupInvalidSaveSlots(};

    // ?≤Â?‰øùÂ??∏Ê?Ê®°Êùø
    FSaveGameData GetSaveDataTemplate() const;

    // ?≤Â?‰øùÂ?ÊßΩ‰?Ê®°Êùø
    FSaveSlot GetSaveSlotTemplate() const;

    // ?≤Â?‰øùÂ??ç‰?Ê®°Êùø
    FSaveOperation GetSaveOperationTemplate() const;

    // Ë®òÈ?‰øùÂ?Áµ±Ë?
    void RecordSaveStatistics(};

    // ?êÊ∏¨‰øùÂ?Ë≤†Ë?
    float PredictSaveLoad() const;

    // Âπ≥Ë°°‰øùÂ?Ë≤†Ë?
    void BalanceSaveLoad(};

    // ?ïÁ?‰øùÂ?Ë°ùÁ?
    void ResolveSaveConflicts(};

    // ?™Â?‰øùÂ??ßËÉΩ
    void OptimizeSavePerformance(};

    // ?≤Â??∂Â??äÊà≤?Ä??    TMap<FString, FString> GetCurrentGameState() const;

    // Ë®≠ÁΩÆ?äÊà≤?Ä??    void SetGameState(const TMap<FString, FString>& GameState};

    // ?≤Â??∂Â??©ÂÆ∂?∏Ê?
    TMap<FString, FString> GetCurrentPlayerData() const;

    // Ë®≠ÁΩÆ?©ÂÆ∂?∏Ê?
    void SetPlayerData(const TMap<FString, FString>& PlayerData};

    // ?≤Â??∂Â?‰∏ñÁ??∏Ê?
    TMap<FString, FString> GetCurrentWorldData() const;

    // Ë®≠ÁΩÆ‰∏ñÁ??∏Ê?
    void SetWorldData(const TMap<FString, FString>& WorldData};
};

