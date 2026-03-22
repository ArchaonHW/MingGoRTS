#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSModSystem.generated.h"

/**
 * Ê®°Á?È°ûÂ?
 */
UENUM(BlueprintType)
enum class EModType : uint8
{
    Map                   UMETA(DisplayName = "?∞Â?"),
    Unit                  UMETA(DisplayName = "?Æ‰?"),
    Building              UMETA(DisplayName = "Âª∫Á?"),
    Technology            UMETA(DisplayName = "ÁßëÊ?"),
    Faction               UMETA(DisplayName = "?¢Â?"),
    Campaign              UMETA(DisplayName = "?∞ÂΩπ"),
    UI                    UMETA(DisplayName = "?åÈù¢"),
    Audio                 UMETA(DisplayName = "?≥È†ª"),
    Graphics              UMETA(DisplayName = "?ñÂΩ¢"),
    Gameplay              UMETA(DisplayName = "?©Ê?"),
    TotalConversion       UMETA(DisplayName = "?®Èù¢ËΩâÊ?")
};

/**
 * Ê®°Á??Ä?? */
UENUM(BlueprintType)
enum class EModStatus : uint8
{
    NotInstalled          UMETA(DisplayName = "?™Â?Ë£?),
    Installing            UMETA(DisplayName = "ÂÆâË?‰∏?),
    Installed             UMETA(DisplayName = "Â∑≤Â?Ë£?),
    Enabled               UMETA(DisplayName = "Â∑≤Â???),
    Disabled              UMETA(DisplayName = "Â∑≤Á???),
    NeedsUpdate           UMETA(DisplayName = "?ÄË¶ÅÊõ¥??),
    Error                 UMETA(DisplayName = "?ØË™§")
};

/**
 * Ê®°Á?‰ø°ÊÅØ
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
 * Ê®°Á?Á≥ªÁµ± - ÁÆ°Á?Ê®°Á??åÂâµ?èÂ∑•?äÂ??? */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSModSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSModSystem(};
    
    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Mod")
    void InitializeModSystem(};
    
    // ÂæûÂâµ?èÂ∑•?äÁç≤?ñÊ®°ÁµÑÂ?Ë°?    UFUNCTION(BlueprintCallable, Category = "Mod")
    TArray<FModInfo> GetWorkshopMods(const FString& SearchQuery, const TArray<EModType>& FilterTypes};
    
    // ?≤Â?Â∑≤Â?Ë£ùÊ®°Áµ?    UFUNCTION(BlueprintCallable, Category = "Mod")
    TArray<FModInfo> GetInstalledMods() const;
    
    // ?≤Â?Â∑≤Â??®Ê®°Áµ?    UFUNCTION(BlueprintCallable, Category = "Mod")
    TArray<FModInfo> GetEnabledMods() const;
    
    // ?≤Â?Ê®°Á?Ë©≥Ê?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    bool GetModInfo(const FString& ModID, FModInfo& OutModInfo) const;
    
    // ‰∏ãË?Ê®°Á?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void DownloadMod(const FString& ModID};
    
    // ?ñÊ?‰∏ãË?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void CancelDownload(const FString& ModID};
    
    // ÂÆâË?Ê®°Á?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    bool InstallMod(const FString& FilePath};
    
    // ?∏Ë?Ê®°Á?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void UninstallMod(const FString& ModID};
    
    // ?üÁî®Ê®°Á?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void EnableMod(const FString& ModID};
    
    // Á¶ÅÁî®Ê®°Á?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void DisableMod(const FString& ModID};
    
    // ?¥Êñ∞Ê®°Á?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void UpdateMod(const FString& ModID};
    
    // Ê™¢Êü•?¥Êñ∞
    UFUNCTION(BlueprintCallable, Category = "Mod")
    TArray<FModInfo> CheckForUpdates(};
    
    // ‰∏äÂÇ≥Ê®°Á??∞Ââµ?èÂ∑•??    UFUNCTION(BlueprintCallable, Category = "Mod")
    bool UploadMod(const FString& ModID, const FString& Description, const TArray<FString>& Tags};
    
    // ?µÂª∫?¨Âú∞Ê®°Á?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    FString CreateLocalMod(const FString& Name, EModType Type, const FString& Description};
    
    // Ë®ÇÈñ±Ê®°Á?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void SubscribeToMod(const FString& ModID};
    
    // ?ñÊ?Ë®ÇÈñ±
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void UnsubscribeFromMod(const FString& ModID};
    
    // Ë©ïÂ?Ê®°Á?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void RateMod(const FString& ModID, float Rating};
    
    // ?±Â?Ê®°Á?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void ReportMod(const FString& ModID, const FString& Reason};
    
    // È©óË?Ê®°Á?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    bool ValidateMod(const FString& ModID};
    
    // ?ÉÊ?Ê®°Á?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void ScanForMods(};
    
    // ?†Ë??Ä?âÂ??®Á?Ê®°Á?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void LoadEnabledMods(};
    
    // ?≤Â?Ê®°Á??ÆÈ?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    FString GetModsDirectory() const;
    
    // Ë®≠ÁΩÆ?üÂ??ÜÂ?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    void SetModLoadOrder(const TArray<FString>& ModIDs};
    
    // ?≤Â?‰æùË≥¥Ë°ùÁ?
    UFUNCTION(BlueprintCallable, Category = "Mod")
    TArray<FString> GetDependencyConflicts(const FString& ModID) const;
    
    // ‰∫ã‰ª∂ÂßîË?
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

