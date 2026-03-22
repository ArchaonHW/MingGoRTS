#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSModSystem.generated.h"

/**
 * 模�?類�?
 */
UENUM(BlueprintType)
enum class EModType : uint8
{
    Map                   UMETA(DisplayName = "?��?"),
    Unit                  UMETA(DisplayName = "?��?"),
    Building              UMETA(DisplayName = "建�?"),
    Technology            UMETA(DisplayName = "科�?"),
    Faction               UMETA(DisplayName = "?��?"),
    Campaign              UMETA(DisplayName = "?�役"),
    UI                    UMETA(DisplayName = "?�面"),
    Audio                 UMETA(DisplayName = "?�頻"),
    Graphics              UMETA(DisplayName = "?�形"),
    Gameplay              UMETA(DisplayName = "?��?"),
    TotalConversion       UMETA(DisplayName = "?�面轉�?")
};

/**
 * 模�X�X */
UENUM(BlueprintType)
enum class EModStatus : uint8
{
    NotInstalled          UMETA(DisplayName = "?��?�?),
    Installing            UMETA(DisplayName = "安�?�?),
    Installed             UMETA(DisplayName = "已�?�?),
    Enabled               UMETA(DisplayName = "已�X),
    Disabled              UMETA(DisplayName = "已�X),
    NeedsUpdate           UMETA(DisplayName = "?�要更X),
    Error                 UMETA(DisplayName = "?�誤")
};

/**
 * 模�?信息
 */
USTRUCT(BlueprintType)
struct FModInfo
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ModID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Author;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Version;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString GameVersion;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EModType> Types;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Tags;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FileSize;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ThumbnailPath;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DownloadURL;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DownloadCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Rating;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UploadDate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LocalPath;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EModStatus Status;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Dependencies;
    
    FModInfo()
        : FileSize(0)
        , DownloadCount(0)
        , Rating(0.0f)
        , Status(EModStatus::NotInstalled)
    {}
};

/**
 * 模�?系統 - 管�?模�X�創?�工?��X */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSModSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSModSystem(};
    
    // ?��X    UFUNCTION(BlueprintCallable, Category = "Mod")
    void InitializeModSystem(};
    
    // 從創?�工?�獲?�模組�?�?    UFUNCTION(BlueprintCallable, Category = "Mod")
    TArray<FModInfo> GetWorkshopMods(const FString& SearchQuery, const TArray<EModType>& FilterTypes};
    
    // ?��?已�?裝模�?    UFUNCTION(BlueprintCallable, Category = "Mod")
    TArray<FModInfo> GetInstalledMods() const;
    
    // ?��?已�X�模�?    UFUNCTION(BlueprintCallable, Category = "Mod")
    TArray<FModInfo> GetEnabledMods() const;
    
    // ?��?模�?詳�?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    bool GetModInfo(const FString& ModID, FModInfo& OutModInfo) const;
    
    // 下�?模�?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void DownloadMod(const FString& ModID};
    
    // ?��?下�?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void CancelDownload(const FString& ModID};
    
    // 安�?模�?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    bool InstallMod(const FString& FilePath};
    
    // ?��?模�?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void UninstallMod(const FString& ModID};
    
    // ?�用模�?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void EnableMod(const FString& ModID};
    
    // 禁用模�?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void DisableMod(const FString& ModID};
    
    // ?�新模�?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void UpdateMod(const FString& ModID};
    
    // 檢查?�新
    UFUNCTION(BlueprintCallable, Category = "Mod")
    TArray<FModInfo> CheckForUpdates(};
    
    // 上傳模�X�創?�工X    UFUNCTION(BlueprintCallable, Category = "Mod")
    bool UploadMod(const FString& ModID, const FString& Description, const TArray<FString>& Tags};
    
    // ?�建?�地模�?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    FString CreateLocalMod(const FString& Name, EModType Type, const FString& Description};
    
    // 訂閱模�?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void SubscribeToMod(const FString& ModID};
    
    // ?��?訂閱
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void UnsubscribeFromMod(const FString& ModID};
    
    // 評�?模�?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void RateMod(const FString& ModID, float Rating};
    
    // ?��?模�?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void ReportMod(const FString& ModID, const FString& Reason};
    
    // 驗�?模�?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    bool ValidateMod(const FString& ModID};
    
    // ?��?模�?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void ScanForMods(};
    
    // ?��X�?��X��?模�?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void LoadEnabledMods(};
    
    // ?��?模�X��?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    FString GetModsDirectory() const;
    
    // 設置?��X��?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void SetModLoadOrder(const TArray<FString>& ModIDs};
    
    // ?��?依賴衝�?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    TArray<FString> GetDependencyConflicts(const FString& ModID) const;
    
    // 事件委�?
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModDownloaded, const FString&, ModID};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModInstalled, const FString&, ModID};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModUninstalled, const FString&, ModID};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModEnabled, const FString&, ModID};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModDisabled, const FString&, ModID};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModError, const FString&, ErrorMessage};
    
    UPROPERTY(BlueprintAssignable, Category = "Mod|Events")
    FOnModDownloaded OnModDownloaded;
    
    UPROPERTY(BlueprintAssignable, Category = "Mod|Events")
    FOnModInstalled OnModInstalled;
    
    UPROPERTY(BlueprintAssignable, Category = "Mod|Events")
    FOnModUninstalled OnModUninstalled;
    
    UPROPERTY(BlueprintAssignable, Category = "Mod|Events")
    FOnModEnabled OnModEnabled;
    
    UPROPERTY(BlueprintAssignable, Category = "Mod|Events")
    FOnModDisabled OnModDisabled;
    
    UPROPERTY(BlueprintAssignable, Category = "Mod|Events")
    FOnModError OnModError;
    
private:
    UPROPERTY()
    TMap<FString, FModInfo> InstalledMods;
    
    UPROPERTY()
    TSet<FString> EnabledModIDs;
    
    UPROPERTY()
    TArray<FString> ModLoadOrder;
    
    FString GetModFilePath(const FString& ModID) const;
    bool ExtractModArchive(const FString& ArchivePath, const FString& DestPath};
    bool ValidateModDependencies(const FModInfo& Mod};
    void SaveModList(};
    void LoadModList(};
    void RegisterMod(const FModInfo& ModInfo};
    void UnregisterMod(const FString& ModID};
    void BroadcastModEvent(EModStatus NewStatus, const FString& ModID};
};

