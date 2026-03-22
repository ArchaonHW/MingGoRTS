#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSReplayManager.generated.h"

struct FReplayMetadata;

/**
 * ??��???X???
 */
UENUM(BlueprintType)
enum class EReplaySortMethod: uint8 {
    DateNewest          UMETA(DisplayName = "????(??X"),
    DateOldest          UMETA(DisplayName = "????(??X"),
    NameAscending       UMETA(DisplayName = "??��(????)"),
    NameDescending      UMETA(DisplayName = "??��(????)"),
    DurationLongest     UMETA(DisplayName = "??��(??X"),
    DurationShortest    UMETA(DisplayName = "??��(??X")
};

/**
 * ??��??�o����
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
 * ??���?X- ��?X?��??��???X???�˯�
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSReplayManager : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSReplayManager();
    
    // ???X
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void InitializeReplayManager();
    
    // ???X????X
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> GetAllReplays() const;
    
    // ??�o??��
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> FilterReplays(const FReplayFilter& Filter) const;
    
    // ???X?��
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> SortReplays(const TArray<FReplayMetadata>& Replays, EReplaySortMethod SortMethod) const;
    
    // ??��??��
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> SearchReplays(const FString& SearchQuery) const;
    
    // ???X?���??
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    FReplayMetadata GetReplayMetadata(const FString& ReplayID) const;
    
    // ??�R???X
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool RenameReplay(const FString& ReplayID, const FString& NewName);
    
    // ??��??��
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool DeleteReplay(const FString& ReplayID);
    
    // ???X?��
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    int32 DeleteReplaysBatch(const TArray<FString>& ReplayIDs);
    
    // �ɥX??��????�w��??
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool ExportReplay(const FString& ReplayID, const FString& ExportPath);
    
    // �q??��?X??X
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    FReplayMetadata ImportReplay(const FString& FilePath);
    
    // ????�s�x�ϥ�???? (MB)
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    float GetStorageUsage() const;
    
    // ????�s�x???? (MB)
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    float GetStorageQuota() const;
    
    // �]�m�s�x????
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void SetStorageQuota(float QuotaMB);
    
    // �M?X??X
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    int32 CleanupOldReplays(int32 DaysOld);
    
    // ??��??��??��??
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool CreateReplayFolder(const FString& FolderName);
    
    // ???X?��????
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FString> GetReplayFolders() const;
    
    // ��?X?��????��
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool MoveReplayToFolder(const FString& ReplayID, const FString& FolderName);
    
    // ??��??��????
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void AddReplayToFavorites(const FString& ReplayID);
    
    // ����????
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void RemoveReplayFromFavorites(const FString& ReplayID);
    
    // ???X??X??X
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> GetFavoriteReplays() const;
    
    // �ƥ�e??
    
    
    
    
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


