#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCloudSystem.generated.h"

/**
 * 雲端服務類型
 */
UENUM(BlueprintType)
enum class ECloudServiceType: uint8 {
    SaveSync              UMETA(DisplayName = "存檔同步"),
    Leaderboard           UMETA(DisplayName = "排行榜"),
    Matchmaking           UMETA(DisplayName = "配對系統"),
    Analytics             UMETA(DisplayName = "數據分析"),
    ContentDelivery       UMETA(DisplayName = "內容分發"),
    Multiplayer           UMETA(DisplayName = "多人遊戲"),
    Authentication        UMETA(DisplayName = "認證服務"),
    Storage               UMETA(DisplayName = "存儲服務"),
    Compute               UMETA(DisplayName = "計算服務"),
    Custom                UMETA(DisplayName = "自定義")
};

/**
 * 雲端服務狀態
 */
UENUM(BlueprintType)
enum class ECloudServiceStatus: uint8 {
    Disconnected          UMETA(DisplayName = "未連接"),
    Connecting            UMETA(DisplayName = "連接中"),
    Connected             UMETA(DisplayName = "已連接"),
    Authenticating        UMETA(DisplayName = "認證中"),
    Authenticated         UMETA(DisplayName = "已認證"),
    Error                 UMETA(DisplayName = "錯誤"),
    Maintenance           UMETA(DisplayName = "維護中")
};

/**
 * 雲端存檔數據
 */
USTRUCT(BlueprintType)
struct FCloudSaveData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SaveID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CharacterName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Level;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PlayTimeMinutes;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LastPlayed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FileSize;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ThumbnailURL;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCurrentDevice;
    
    FCloudSaveData()
        : Level(1)
        , PlayTimeMinutes(0)
        , FileSize(0)
        , bIsCurrentDevice(false)
    {}
};

/**
 * 雲端用戶資料
 */
USTRUCT(BlueprintType)
struct FCloudUserProfile
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UserID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DisplayName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Email;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AvatarURL;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalPlayTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AccountCreated;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LastLogin;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ECloudServiceType> LinkedServices;
    
    FCloudUserProfile()
        : TotalPlayTime(0)
    {}
};

/**
 * 雲端資產
 */
USTRUCT(BlueprintType)
struct FCloudAsset
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AssetID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AssetName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AssetType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DownloadURL;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FileSize;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Version;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UploadDate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsInstalled;
    
    FCloudAsset()
        : FileSize(0)
        , bIsInstalled(false)
    {}
};

/**
 * 雲端服務統計
 */
USTRUCT(BlueprintType)
struct FCloudServiceStats
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalUploads;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalDownloads;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 TotalBytesUploaded;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 TotalBytesDownloaded;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FailedOperations;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AverageLatency;
    
    FCloudServiceStats()
        : TotalUploads(0)
        , TotalDownloads(0)
        , TotalBytesUploaded(0)
        , TotalBytesDownloaded(0)
        , FailedOperations(0)
        , AverageLatency(0.0f)
    {}
};

/**
 * 雲端服務系統 - 管理雲端功能和服務集成
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSCloudSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSCloudSystem();
    
    // 初始化
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    void InitializeCloudSystem();
    
    // 連接到雲端服務
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    void ConnectToCloud();
    
    // 斷開連接
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    void DisconnectFromCloud();
    
    // 用戶認證
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    void AuthenticateUser(const FString& Username, const FString& Password);
    
    // 第三方登入
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    void AuthenticateWithThirdParty(const FString& Provider, const FString& Token);
    
    // 登出
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    void Logout();
    
    // 上傳存檔
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    void UploadSave(const FString& SaveSlot, const FString& SaveName);
    
    // 下載存檔
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    void DownloadSave(const FString& SaveID);
    
    // 刪除雲端存檔
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    void DeleteCloudSave(const FString& SaveID);
    
    // 獲取雲端存檔列表
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    TArray<FCloudSaveData> GetCloudSaves() const;
    
    // 同步存檔
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    void SyncSaves();
    
    // 獲取用戶資料
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    FCloudUserProfile GetUserProfile() const;
    
    // 更新用戶資料
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    void UpdateUserProfile(const FCloudUserProfile& Profile);
    
    // 獲取雲端資產列表
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    TArray<FCloudAsset> GetCloudAssets(const FString& AssetType) const;
    
    // 下載雲端資產
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    void DownloadAsset(const FString& AssetID);
    
    // 上傳用戶內容
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    void UploadUserContent(const FString& ContentPath, const FString& Description);
    
    // 獲取服務統計
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    FCloudServiceStats GetServiceStats() const;
    
    // 檢查連接狀態
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    ECloudServiceStatus GetConnectionStatus() const;
    
    // 檢查服務是否可用
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    bool IsServiceAvailable(ECloudServiceType ServiceType) const;
    
    // 啟用/禁用服務
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    void SetServiceEnabled(ECloudServiceType ServiceType, bool bEnabled);
    
    // 獲取當前設備ID
    UFUNCTION(BlueprintCallable, Category = "Cloud")
    FString GetCurrentDeviceID() const;
    
    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloudConnected, bool, bSuccess);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloudDisconnected, const FString&, Reason);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthenticationSuccess, const FCloudUserProfile&, Profile);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthenticationFailed, const FString&, ErrorMessage);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaveUploadComplete, const FString&, SaveID, bool, bSuccess);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaveDownloadComplete, const FString&, SaveID, bool, bSuccess);
    
    UPROPERTY(BlueprintAssignable, Category = "Cloud|Events")
    FOnCloudConnected OnCloudConnected;
    
    UPROPERTY(BlueprintAssignable, Category = "Cloud|Events")
    FOnCloudDisconnected OnCloudDisconnected;
    
    UPROPERTY(BlueprintAssignable, Category = "Cloud|Events")
    FOnAuthenticationSuccess OnAuthenticationSuccess;
    
    UPROPERTY(BlueprintAssignable, Category = "Cloud|Events")
    FOnAuthenticationFailed OnAuthenticationFailed;
    
    UPROPERTY(BlueprintAssignable, Category = "Cloud|Events")
    FOnSaveUploadComplete OnSaveUploadComplete;
    
    UPROPERTY(BlueprintAssignable, Category = "Cloud|Events")
    FOnSaveDownloadComplete OnSaveDownloadComplete;
    
private:
    UPROPERTY()
    ECloudServiceStatus ConnectionStatus;
    
    UPROPERTY()
    FCloudUserProfile CurrentUser;
    
    UPROPERTY()
    TArray<FCloudSaveData> CloudSaves;
    
    UPROPERTY()
    TArray<FCloudAsset> CloudAssets;
    
    UPROPERTY()
    FCloudServiceStats ServiceStats;
    
    UPROPERTY()
    TSet<ECloudServiceType> EnabledServices;
    
    UPROPERTY()
    FString CurrentDeviceID;
    
    UPROPERTY()
    bool bAutoSyncEnabled;
    
    UPROPERTY()
    float AutoSyncInterval;
    
    FTimerHandle AutoSyncTimerHandle;
    
    void GenerateDeviceID();
    void StartAutoSync();
    void StopAutoSync();
    void OnAutoSyncTick();
    void UploadSaveInternal(const FString& SaveSlot, const FString& SaveData);
    void DownloadSaveInternal(const FString& SaveID);
    void SyncSaveList();
    bool ValidateSaveData(const FString& SaveData) const;
    void CompressAndUpload(const FString& SaveSlot);
    void DownloadAndDecompress(const FString& SaveID);
};
