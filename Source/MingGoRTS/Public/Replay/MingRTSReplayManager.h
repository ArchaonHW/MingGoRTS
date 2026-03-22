#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSReplayManager.generated.h"

struct FReplayMetadata;

/**
 * ?ûÊîæ?íÂ??πÂ?
 */
UENUM(BlueprintType)
enum class EReplaySortMethod : uint8
{
    DateNewest          UMETA(DisplayName = "?•Ê?(?Ä??"),
    DateOldest          UMETA(DisplayName = "?•Ê?(?Ä??"),
    NameAscending       UMETA(DisplayName = "?çÁ®±(?áÂ?)"),
    NameDescending      UMETA(DisplayName = "?çÁ®±(?çÂ?)"),
    DurationLongest     UMETA(DisplayName = "?ÇÈï∑(?Ä??"),
    DurationShortest    UMETA(DisplayName = "?ÇÈï∑(?Ä??")
};

/**
 * ?ûÊîæ?éÊøæÊ¢ù‰ª∂
 */
USTRUCT(BlueprintType)
struct FReplayFilter
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MapNameFilter;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerFilter;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinDuration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxDuration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DateFrom;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DateTo;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString GameVersion;
    
    FReplayFilter()
        : MinDuration(0.0f)
        , MaxDuration(-1.0f)
    {}
};

/**
 * ?ûÊîæÁÆ°Á???- ÁÆ°Á??ûÊîæ?á‰ª∂?ÑÂ??≤Â?Ê™¢Á¥¢
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSReplayManager : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSReplayManager(};
    
    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void InitializeReplayManager(};
    
    // ?≤Â??Ä?âÂ???    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> GetAllReplays() const;
    
    // ?éÊøæ?ûÊîæ
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> FilterReplays(const FReplayFilter& Filter) const;
    
    // ?íÂ??ûÊîæ
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> SortReplays(const TArray<FReplayMetadata>& Replays, EReplaySortMethod SortMethod) const;
    
    // ?úÁ¥¢?ûÊîæ
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> SearchReplays(const FString& SearchQuery) const;
    
    // ?≤Â??ûÊîæË©≥Ê?
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    FReplayMetadata GetReplayMetadata(const FString& ReplayID) const;
    
    // ?çÂëΩ?çÂ???    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool RenameReplay(const FString& ReplayID, const FString& NewName};
    
    // ?™Èô§?ûÊîæ
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool DeleteReplay(const FString& ReplayID};
    
    // ?πÈ??™Èô§
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    int32 DeleteReplaysBatch(const TArray<FString>& ReplayIDs};
    
    // Â∞éÂá∫?ûÊîæ?∞Ê?ÂÆöË∑ØÂæ?    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool ExportReplay(const FString& ReplayID, const FString& ExportPath};
    
    // ÂæûÊ?‰ª∂Â??•Â???    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    FReplayMetadata ImportReplay(const FString& FilePath};
    
    // ?≤Â?Â≠òÂÑ≤‰ΩøÁî®?ÖÊ? (MB)
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    float GetStorageUsage() const;
    
    // ?≤Â?Â≠òÂÑ≤?çÈ? (MB)
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    float GetStorageQuota() const;
    
    // Ë®≠ÁΩÆÂ≠òÂÑ≤?çÈ?
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void SetStorageQuota(float QuotaMB};
    
    // Ê∏ÖÁ??äÂ???    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    int32 CleanupOldReplays(int32 DaysOld};
    
    // ?µÂª∫?ûÊîæ?á‰ª∂Â§?    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool CreateReplayFolder(const FString& FolderName};
    
    // ?≤Â??á‰ª∂Â§æÂ?Ë°?    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FString> GetReplayFolders() const;
    
    // ÁßªÂ??ûÊîæ?∞Ê?‰ª∂Â§æ
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool MoveReplayToFolder(const FString& ReplayID, const FString& FolderName};
    
    // ?µÂª∫?ûÊîæ?∂Ë?
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void AddReplayToFavorites(const FString& ReplayID};
    
    // ÁßªÈô§?∂Ë?
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void RemoveReplayFromFavorites(const FString& ReplayID};
    
    // ?≤Â??∂Ë??ÑÂ???    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> GetFavoriteReplays() const;
    
    // ‰∫ã‰ª∂ÂßîË?
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReplayAdded, const FReplayMetadata&, Replay};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReplayDeleted, const FString&, ReplayID};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReplayRenamed, const FString&, ReplayID};
    
    UPROPERTY(BlueprintAssignable, Category = "ReplayManager|Events")
    FOnReplayAdded OnReplayAdded;
    
    UPROPERTY(BlueprintAssignable, Category = "ReplayManager|Events")
    FOnReplayDeleted OnReplayDeleted;
    
    UPROPERTY(BlueprintAssignable, Category = "ReplayManager|Events")
    FOnReplayRenamed OnReplayRenamed;
    
private:
    UPROPERTY()
    float StorageQuotaMB;
    
    UPROPERTY()
    TArray<FString> FavoriteReplayIDs;
    
    UPROPERTY()
    TMap<FString, FString> ReplayFolderMap;
    
    FString GetReplayDirectory() const;
    FString GetMetadataFilePath(const FString& ReplayID) const;
    bool LoadMetadataFromFile(const FString& ReplayID, FReplayMetadata& OutMetadata) const;
    bool SaveMetadataToFile(const FString& ReplayID, const FReplayMetadata& Metadata};
};

