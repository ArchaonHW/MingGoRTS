#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSReplayManager.generated.h"

struct FReplayMetadata;

/**
 * ??放???X???
 */
UENUM(BlueprintType)
enum class EReplaySortMethod : uint8
{
    DateNewest          UMETA(DisplayName = "????(??X"),
    DateOldest          UMETA(DisplayName = "????(??X"),
    NameAscending       UMETA(DisplayName = "??稱(????)"),
    NameDescending      UMETA(DisplayName = "??稱(????)"),
    DurationLongest     UMETA(DisplayName = "??長(??X"),
    DurationShortest    UMETA(DisplayName = "??長(??X")
};

/**
 * ??放??濾條件
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
 * ??放管?X- 管?X?放??件???X???檢索
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSReplayManager : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSReplayManager();
    
    // ???X    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void InitializeReplayManager();
    
    // ???X????X    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> GetAllReplays() const;
    
    // ??濾??放
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> FilterReplays(const FReplayFilter& Filter) const;
    
    // ???X?放
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> SortReplays(const TArray<FReplayMetadata>& Replays, EReplaySortMethod SortMethod) const;
    
    // ??索??放
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> SearchReplays(const FString& SearchQuery) const;
    
    // ???X?放詳??
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    FReplayMetadata GetReplayMetadata(const FString& ReplayID) const;
    
    // ??命???X    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool RenameReplay(const FString& ReplayID, const FString& NewName};
    
    // ??除??放
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool DeleteReplay(const FString& ReplayID};
    
    // ???X?除
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    int32 DeleteReplaysBatch(const TArray<FString>& ReplayIDs};
    
    // 導出??放????定路??    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool ExportReplay(const FString& ReplayID, const FString& ExportPath};
    
    // 從??件?X??X    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    FReplayMetadata ImportReplay(const FString& FilePath};
    
    // ????存儲使用???? (MB)
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    float GetStorageUsage() const;
    
    // ????存儲???? (MB)
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    float GetStorageQuota() const;
    
    // 設置存儲????
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void SetStorageQuota(float QuotaMB};
    
    // 清?X??X    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    int32 CleanupOldReplays(int32 DaysOld};
    
    // ??建??放??件??    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool CreateReplayFolder(const FString& FolderName};
    
    // ???X?件夾????    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FString> GetReplayFolders() const;
    
    // 移?X?放????件夾
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool MoveReplayToFolder(const FString& ReplayID, const FString& FolderName};
    
    // ??建??放????
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void AddReplayToFavorites(const FString& ReplayID};
    
    // 移除????
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void RemoveReplayFromFavorites(const FString& ReplayID};
    
    // ???X??X??X    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> GetFavoriteReplays() const;
    
    // 事件委??
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
    bool SaveMetadataToFile(const FString& ReplayID, const FReplayMetadata& Metadata);
};

