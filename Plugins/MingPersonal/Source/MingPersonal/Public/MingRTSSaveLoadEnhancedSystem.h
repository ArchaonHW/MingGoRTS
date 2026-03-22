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
enum class ESaveDataType: uint8 {
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
enum class ESaveFormat: uint8 {
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
enum class ESaveLocation: uint8 {
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
enum class ESaveStatus: uint8 {
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
enum class ESavePriority: uint8 {
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
        SaveID = TEXT("");
        SaveName = TEXT("");
        DataType = ESaveDataType::GameState;
        Format = ESaveFormat::Binary;
        Location = ESaveLocation::Local;
        Status = ESaveStatus::None;
        FilePath = TEXT("");
        Timestamp = 0.0f;
        Version = 1;
        Size = 0;
        Checksum = TEXT("");
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
        SlotID = TEXT("");
        SlotName = TEXT("");
        Description = TEXT("");
        ThumbnailPath = TEXT("");
        PlayTime = 0.0f;
        LevelName = TEXT("");
        PlayerName = TEXT("");
        Chapter = 0;
        CampaignID = TEXT("");
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
        OperationID = TEXT("");
        OperationType = TEXT("");
        TargetID = TEXT("");
        DataType = ESaveDataType::GameState;
        Priority = ESavePriority::Medium;
        StartTime = 0.0f;
        Progress = 0.0f;
        bIsCompleted = false;
        bIsFailed = false;
        ErrorMessage = TEXT("");
    }
};









DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAutoSaveTriggered, const FString&, Reason);

/**
 * MingGoRTS �W�j�O?X??X?�t?? * ???X?��目標數量�s�B?X?�B??���B��_?X?��
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingRTS")
class MINGPERSONAL_API UMingRTSSaveLoadEnhancedSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSSaveLoadEnhancedSystem();

    // 目標數量�j??�s?X?�t??
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void InitializeSaveLoadEnhancedSystem(UWorld* World);

    // ??�s�W�j�O??���J�t�Ρ]??�V�ե�??
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void UpdateSaveLoadEnhancedSystem(float DeltaTime);

    // �O?X?��摧毀
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FString SaveGameData(const FString& SaveName, ESaveDataType DataType, const TArray<uint8>& Data, ESaveFormat Format = ESaveFormat::Binary);

    // ���J??��摧毀
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool LoadGameData(const FString& SaveID, TArray<uint8>& OutData);

    // �O?X?��??
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool SaveToSlot(const FString& SlotName, const FString& Description, const FString& ThumbnailPath = TEXT("")};

    // �q�Ѧ�?X
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool LoadFromSlot(const FString& SlotID);

    // ??�ثO??��??
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FString CreateSaveSlot(const FString& SlotName, const FString& Description, int32 MaxSaves = 10);

    // ??���O??��??
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool DeleteSaveSlot(const FString& SlotID);

    // 摧毀�O??��??
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FSaveSlot GetSaveSlot(const FString& SlotID) const;

    // 目標數量??�s��??
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<FSaveSlot> GetAllSaveSlots() const;

    // 摧毀�O?X???
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FSaveGameData GetSaveGameData(const FString& SaveID) const;

    // 摧毀�O??
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void AutoSave(const FString& Reason = TEXT("AutoSave")};

    // �]�m摧毀�O?X???
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void SetAutoSaveInterval(float Interval);

    // ??���O?X???
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool BackupSaveData(const FString& SaveID, const FString& BackupLocation = TEXT("")};

    // ??�_??��摧毀
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool RestoreBackup(const FString& BackupID);

    // ��??�O?X???
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool ValidateSaveData(const FString& SaveID);

    // �״_目標數量�s��X
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool RepairSaveData(const FString& SaveID);

    // ���Y�O?X???
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool CompressSaveData(const FString& SaveID);

    // ?X�Y??�s��X
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool DecompressSaveData(const FString& SaveID);

    // 摧毀�O?X???
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool EncryptSaveData(const FString& SaveID, const FString& EncryptionKey);

    // ?X�O?X???
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool DecryptSaveData(const FString& SaveID, const FString& EncryptionKey);

    // �ɥX�O?X???
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool ExportSaveData(const FString& SaveID, const FString& ExportPath, ESaveFormat Format = ESaveFormat::JSON);

    // �ɤJ�O?X???
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool ImportSaveData(const FString& ImportPath, ESaveFormat Format = ESaveFormat::JSON);

    // 摧毀�O??��??
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TMap<FString, float> GetSaveStatistics() const;

    // �M?X???�O?X???
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void CleanupOldSaveData(float MaxAge = 30.0f);

    // 摧毀�O?X?��
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void OptimizeSavePerformance();

    // �]�m�O?X摧毀?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void SetSavePriority(const FString& SaveID, ESavePriority Priority);

    // 摧毀�O??
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void BatchSave(const TArray<FString>& SaveIDs);

    // 摧毀���J
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void BatchLoad(const TArray<FString>& SaveIDs);

public:
    // �ƥ�e??
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
    // 摧毀�O?X???
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void ProcessSaveOperations(float DeltaTime);

    // 摧毀���J摧毀
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void ProcessLoadOperations(float DeltaTime);

    // 目標數量�O??
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void ProcessAutoSave(float DeltaTime);

    // ???X?��摧毀
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void ProcessBackupOperations(float DeltaTime);

    // ��??�O??����X
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool ValidateSaveIntegrity(const FSaveGameData& SaveData) const;

    // �p?X??X
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FString CalculateChecksum(const TArray<uint8>& Data) const;

    // ��?X?��X
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<uint8> SerializeData(const TMap<FString, FString>& Data, ESaveFormat Format) const;

    // 故事選項?
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TMap<FString, FString> DeserializeData(const TArray<uint8>& Data, ESaveFormat Format) const;

    // ���Y摧毀
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<uint8> CompressData(const TArray<uint8>& Data) const;

    // ?X�Y��X
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<uint8> DecompressData(const TArray<uint8>& Data) const;

    // 目標數量
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<uint8> EncryptData(const TArray<uint8>& Data, const FString& Key) const;

    // ?X???
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<uint8> DecryptData(const TArray<uint8>& Data, const FString& Key) const;

    // 摧毀�Y��X
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FString GenerateThumbnail(const FString& SlotID) const;

    // 摧毀�O??��??
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FString GetSavePath(ESaveLocation Location, const FString& FileName) const;

    // ??�ثO?X???
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool CreateSaveDirectory(const FString& Path) const;

protected:
    // 摧毀�@??�ޥ�
    UPROPERTY()
    TObjectPtr<UWorld> CurrentWorld;

    // �O?X??X???
    UPROPERTY()
    TMap<FString, FSaveGameData> SaveGameData;

    // �O??��?X???
    UPROPERTY()
    TMap<FString, FSaveSlot> SaveSlots;

    // �O?X??X???
    UPROPERTY()
    TMap<FString, FSaveOperation> SaveOperations;

    // 摧毀��??ID
    UPROPERTY()
    FString CurrentSlotID;

    // �O?X?�s摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    float SaveUpdateInterval;

    // �W���O?X?�s摧毀
    UPROPERTY()
    float LastSaveUpdateTime;

    // 摧毀�O?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    float AutoSaveInterval;

    // �W��摧毀�O?X???
    UPROPERTY()
    float LastAutoSaveTime;

    // ??�j??�s��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    int32 MaxSaveCount;

    // ??�j�Ѧ��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    int32 MaxSlotCount;

    // �q??�O?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    ESaveFormat DefaultFormat;

    // �q??�O??��m
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    ESaveLocation DefaultLocation;

    // ??�_??��摧毀�O??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    bool bAutoSaveEnabled;

    // ??�_??�����Y
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    bool bCompressionEnabled;

    // ??�_??��摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    bool bEncryptionEnabled;

private:
    // 摧毀�O?X??X?��
    FString GetSaveDataName(ESaveDataType DataType) const;

    // 摧毀�O?X??X?��
    FString GetSaveFormatName(ESaveFormat Format) const;

    // 摧毀�O??��m??��
    FString GetSaveLocationName(ESaveLocation Location) const;

    // 目標數量ID
    FString GenerateUniqueID(const FString& Prefix) const;

    // ��??�O?X???
    bool ValidateSaveData(const FSaveGameData& SaveData) const;

    // ��??�O??��??
    bool ValidateSaveSlot(const FSaveSlot& SaveSlot) const;

    // 摧毀�O?X?�~
    void HandleSaveError(const FString& OperationID, const FString& ErrorMessage);

    // 摧毀���J??�~
    void HandleLoadError(const FString& OperationID, const FString& ErrorMessage);

    // �M?X???�O?X???
    void CleanupInvalidSaveData();

    // �M?X???�O??��??
    void CleanupInvalidSaveSlots();

    // 摧毀�O?X???�ҪO
    FSaveGameData GetSaveDataTemplate() const;

    // 摧毀�O??��??�ҪO
    FSaveSlot GetSaveSlotTemplate() const;

    // 摧毀�O?X???�ҪO
    FSaveOperation GetSaveOperationTemplate() const;

    // �O??�O??��??
    void RecordSaveStatistics();

    // ??���O??�t??
    float PredictSaveLoad() const;

    // ���ūO??�t??
    void BalanceSaveLoad();

    // 摧毀�O??��??
    void ResolveSaveConflicts();

    // 摧毀�O?X?��
    void OptimizeSavePerformance();

    // 故事重要性?��??X
    TMap<FString, FString> GetCurrentGameState() const;

    // �]�m??��??X
    void SetGameState(const TMap<FString, FString>& GameState);

    // 故事重要性?�a摧毀
    TMap<FString, FString> GetCurrentPlayerData() const;

    // �]�m??�a摧毀
    void SetPlayerData(const TMap<FString, FString>& PlayerData);

    // 目標數量�@?X???
    TMap<FString, FString> GetCurrentWorldData() const;

    // �]�m�@?X???
    void SetWorldData(const TMap<FString, FString>& WorldData);
);

#endif // MINGRTSSAVELOADENHANCEDSYSTEM_H

