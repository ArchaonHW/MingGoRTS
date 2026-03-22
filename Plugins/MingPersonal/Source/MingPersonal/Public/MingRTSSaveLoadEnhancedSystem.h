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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveStarted, const FString&, SaveID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaveProgress, const FString&, SaveID, float, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaveCompleted, const FString&, SaveID, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadStarted, const FString&, SaveID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLoadProgress, const FString&, SaveID, float, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLoadCompleted, const FString&, SaveID, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveSlotCreated, const FSaveSlot&, SaveSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveSlotDeleted, const FString&, SlotID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAutoSaveTriggered, const FString&, Reason);

/**
 * MingGoRTS 增強保存和載入系統
 * 處理遊戲數據的保存、載入、備份、恢復等功能
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingRTS")
class MINGPERSONAL_API UMingRTSSaveLoadEnhancedSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSSaveLoadEnhancedSystem();

    // 初始化增強保存載入系統
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void InitializeSaveLoadEnhancedSystem(UWorld* World);

    // 更新增強保存載入系統（每幀調用）
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void UpdateSaveLoadEnhancedSystem(float DeltaTime);

    // 保存遊戲數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FString SaveGameData(const FString& SaveName, ESaveDataType DataType, const TArray<uint8>& Data, ESaveFormat Format = ESaveFormat::Binary);

    // 載入遊戲數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool LoadGameData(const FString& SaveID, TArray<uint8>& OutData);

    // 保存到槽位
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool SaveToSlot(const FString& SlotName, const FString& Description, const FString& ThumbnailPath = TEXT(""));

    // 從槽位載入
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool LoadFromSlot(const FString& SlotID);

    // 創建保存槽位
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FString CreateSaveSlot(const FString& SlotName, const FString& Description, int32 MaxSaves = 10);

    // 刪除保存槽位
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool DeleteSaveSlot(const FString& SlotID);

    // 獲取保存槽位
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FSaveSlot GetSaveSlot(const FString& SlotID) const;

    // 獲取所有保存槽位
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<FSaveSlot> GetAllSaveSlots() const;

    // 獲取保存數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FSaveGameData GetSaveGameData(const FString& SaveID) const;

    // 自動保存
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void AutoSave(const FString& Reason = TEXT("AutoSave"));

    // 設置自動保存間隔
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void SetAutoSaveInterval(float Interval);

    // 備份保存數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool BackupSaveData(const FString& SaveID, const FString& BackupLocation = TEXT(""));

    // 恢復備份數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool RestoreBackup(const FString& BackupID);

    // 驗證保存數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool ValidateSaveData(const FString& SaveID);

    // 修復損壞的保存數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool RepairSaveData(const FString& SaveID);

    // 壓縮保存數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool CompressSaveData(const FString& SaveID);

    // 解壓縮保存數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool DecompressSaveData(const FString& SaveID);

    // 加密保存數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool EncryptSaveData(const FString& SaveID, const FString& EncryptionKey);

    // 解密保存數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool DecryptSaveData(const FString& SaveID, const FString& EncryptionKey);

    // 導出保存數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool ExportSaveData(const FString& SaveID, const FString& ExportPath, ESaveFormat Format = ESaveFormat::JSON);

    // 導入保存數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool ImportSaveData(const FString& ImportPath, ESaveFormat Format = ESaveFormat::JSON);

    // 獲取保存統計
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TMap<FString, float> GetSaveStatistics() const;

    // 清理舊的保存數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void CleanupOldSaveData(float MaxAge = 30.0f);

    // 優化保存性能
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void OptimizeSavePerformance();

    // 設置保存優先級
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void SetSavePriority(const FString& SaveID, ESavePriority Priority);

    // 批量保存
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void BatchSave(const TArray<FString>& SaveIDs);

    // 批量載入
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void BatchLoad(const TArray<FString>& SaveIDs);

public:
    // 事件委託
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
    // 處理保存操作
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void ProcessSaveOperations(float DeltaTime);

    // 處理載入操作
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void ProcessLoadOperations(float DeltaTime);

    // 處理自動保存
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void ProcessAutoSave(float DeltaTime);

    // 處理備份操作
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    void ProcessBackupOperations(float DeltaTime);

    // 驗證保存完整性
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool ValidateSaveIntegrity(const FSaveGameData& SaveData) const;

    // 計算校驗和
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FString CalculateChecksum(const TArray<uint8>& Data) const;

    // 序列化數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<uint8> SerializeData(const TMap<FString, FString>& Data, ESaveFormat Format) const;

    // 反序列化數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TMap<FString, FString> DeserializeData(const TArray<uint8>& Data, ESaveFormat Format) const;

    // 壓縮數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<uint8> CompressData(const TArray<uint8>& Data) const;

    // 解壓縮數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<uint8> DecompressData(const TArray<uint8>& Data) const;

    // 加密數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<uint8> EncryptData(const TArray<uint8>& Data, const FString& Key) const;

    // 解密數據
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    TArray<uint8> DecryptData(const TArray<uint8>& Data, const FString& Key) const;

    // 生成縮略圖
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FString GenerateThumbnail(const FString& SlotID) const;

    // 獲取保存路徑
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    FString GetSavePath(ESaveLocation Location, const FString& FileName) const;

    // 創建保存目錄
    UFUNCTION(BlueprintCallable, Category = "Save Load Enhanced System")
    bool CreateSaveDirectory(const FString& Path) const;

protected:
    // 當前世界引用
    UPROPERTY()
    TObjectPtr<UWorld> CurrentWorld;

    // 保存數據映射
    UPROPERTY()
    TMap<FString, FSaveGameData> SaveGameData;

    // 保存槽位映射
    UPROPERTY()
    TMap<FString, FSaveSlot> SaveSlots;

    // 保存操作映射
    UPROPERTY()
    TMap<FString, FSaveOperation> SaveOperations;

    // 當前槽位ID
    UPROPERTY()
    FString CurrentSlotID;

    // 保存更新間隔
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    float SaveUpdateInterval;

    // 上次保存更新時間
    UPROPERTY()
    float LastSaveUpdateTime;

    // 自動保存間隔
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    float AutoSaveInterval;

    // 上次自動保存時間
    UPROPERTY()
    float LastAutoSaveTime;

    // 最大保存數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    int32 MaxSaveCount;

    // 最大槽位數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    int32 MaxSlotCount;

    // 默認保存格式
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    ESaveFormat DefaultFormat;

    // 默認保存位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    ESaveLocation DefaultLocation;

    // 是否啟用自動保存
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    bool bAutoSaveEnabled;

    // 是否啟用壓縮
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    bool bCompressionEnabled;

    // 是否啟用加密
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Load Enhanced System")
    bool bEncryptionEnabled;

private:
    // 獲取保存數據名稱
    FString GetSaveDataName(ESaveDataType DataType) const;

    // 獲取保存格式名稱
    FString GetSaveFormatName(ESaveFormat Format) const;

    // 獲取保存位置名稱
    FString GetSaveLocationName(ESaveLocation Location) const;

    // 生成唯一ID
    FString GenerateUniqueID(const FString& Prefix) const;

    // 驗證保存數據
    bool ValidateSaveData(const FSaveGameData& SaveData) const;

    // 驗證保存槽位
    bool ValidateSaveSlot(const FSaveSlot& SaveSlot) const;

    // 處理保存錯誤
    void HandleSaveError(const FString& OperationID, const FString& ErrorMessage);

    // 處理載入錯誤
    void HandleLoadError(const FString& OperationID, const FString& ErrorMessage);

    // 清理無效保存數據
    void CleanupInvalidSaveData();

    // 清理無效保存槽位
    void CleanupInvalidSaveSlots();

    // 獲取保存數據模板
    FSaveGameData GetSaveDataTemplate() const;

    // 獲取保存槽位模板
    FSaveSlot GetSaveSlotTemplate() const;

    // 獲取保存操作模板
    FSaveOperation GetSaveOperationTemplate() const;

    // 記錄保存統計
    void RecordSaveStatistics();

    // 預測保存負載
    float PredictSaveLoad() const;

    // 平衡保存負載
    void BalanceSaveLoad();

    // 處理保存衝突
    void ResolveSaveConflicts();

    // 優化保存性能
    void OptimizeSavePerformance();

    // 獲取當前遊戲狀態
    TMap<FString, FString> GetCurrentGameState() const;

    // 設置遊戲狀態
    void SetGameState(const TMap<FString, FString>& GameState);

    // 獲取當前玩家數據
    TMap<FString, FString> GetCurrentPlayerData() const;

    // 設置玩家數據
    void SetPlayerData(const TMap<FString, FString>& PlayerData);

    // 獲取當前世界數據
    TMap<FString, FString> GetCurrentWorldData() const;

    // 設置世界數據
    void SetWorldData(const TMap<FString, FString>& WorldData);
};
