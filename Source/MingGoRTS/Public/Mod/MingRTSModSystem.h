#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSModSystem.generated.h"

/**
 * 家??摸??
 */
UENUM(BlueprintType)
enum class EModType : uint8
{
    Map                   UMETA(DisplayName = "????"),
    Unit                  UMETA(DisplayName = "????"),
    Building              UMETA(DisplayName = "??"),
    Technology            UMETA(DisplayName = "??"),
    Faction               UMETA(DisplayName = "????"),
    Campaign              UMETA(DisplayName = "??"),
    UI                    UMETA(DisplayName = "??"),
    Audio                 UMETA(DisplayName = "??繵"),
    Graphics              UMETA(DisplayName = "??"),
    Gameplay              UMETA(DisplayName = "????"),
    TotalConversion       UMETA(DisplayName = "??锣??")
};

/**
 * 家?X?X */
UENUM(BlueprintType)
enum class EModStatus : uint8
{
    NotInstalled          UMETA(DisplayName = "??????),
    Installing            UMETA(DisplayName = "????),
    Installed             UMETA(DisplayName = "????),
    Enabled               UMETA(DisplayName = "?X),
    Disabled              UMETA(DisplayName = "?X),
    NeedsUpdate           UMETA(DisplayName = "??璶X),
    Error                 UMETA(DisplayName = "??粇")
};

/**
 * 家??獺
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
 * 家??╰参 - 恨??家?X?承?????X */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSModSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSModSystem();
    
    // ???X    UFUNCTION(BlueprintCallable, Category = "Mod")
    void InitializeModSystem();
    
    // 眖承????莉??家舱????    UFUNCTION(BlueprintCallable, Category = "Mod")
    TArray<FModInfo> GetWorkshopMods(const FString& SearchQuery, const TArray<EModType>& FilterTypes};
    
    // ??????杆家??    UFUNCTION(BlueprintCallable, Category = "Mod")
    TArray<FModInfo> GetInstalledMods() const;
    
    // ?????X?家??    UFUNCTION(BlueprintCallable, Category = "Mod")
    TArray<FModInfo> GetEnabledMods() const;
    
    // ????家??冈??
    UFUNCTION(BlueprintCallable, Category = "Mod")
    bool GetModInfo(const FString& ModID, FModInfo& OutModInfo) const;
    
    // ??家??
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void DownloadMod(const FString& ModID};
    
    // ??????
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void CancelDownload(const FString& ModID};
    
    // ??家??
    UFUNCTION(BlueprintCallable, Category = "Mod")
    bool InstallMod(const FString& FilePath};
    
    // ????家??
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void UninstallMod(const FString& ModID};
    
    // ??ノ家??
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void EnableMod(const FString& ModID};
    
    // 窽ノ家??
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void DisableMod(const FString& ModID};
    
    // ??穝家??
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void UpdateMod(const FString& ModID};
    
    // 浪琩??穝
    UFUNCTION(BlueprintCallable, Category = "Mod")
    TArray<FModInfo> CheckForUpdates();
    
    // 肚家?X?承??X    UFUNCTION(BlueprintCallable, Category = "Mod")
    bool UploadMod(const FString& ModID, const FString& Description, const TArray<FString>& Tags};
    
    // ????家??
    UFUNCTION(BlueprintCallable, Category = "Mod")
    FString CreateLocalMod(const FString& Name, EModType Type, const FString& Description};
    
    // 璹綷家??
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void SubscribeToMod(const FString& ModID};
    
    // ????璹綷
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void UnsubscribeFromMod(const FString& ModID};
    
    // 蝶??家??
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void RateMod(const FString& ModID, float Rating};
    
    // ????家??
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void ReportMod(const FString& ModID, const FString& Reason};
    
    // 喷??家??
    UFUNCTION(BlueprintCallable, Category = "Mod")
    bool ValidateMod(const FString& ModID};
    
    // ????家??
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void ScanForMods();
    
    // ???X????X???家??
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void LoadEnabledMods();
    
    // ????家?X???
    UFUNCTION(BlueprintCallable, Category = "Mod")
    FString GetModsDirectory() const;
    
    // 砞竚???X???
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void SetModLoadOrder(const TArray<FString>& ModIDs};
    
    // ????ㄌ苦侥??
    UFUNCTION(BlueprintCallable, Category = "Mod")
    TArray<FString> GetDependencyConflicts(const FString& ModID) const;
    
    // ㄆン〆??
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
    void SaveModList();
    void LoadModList();
    void RegisterMod(const FModInfo& ModInfo};
    void UnregisterMod(const FString& ModID};
    void BroadcastModEvent(EModStatus NewStatus, const FString& ModID};
};

