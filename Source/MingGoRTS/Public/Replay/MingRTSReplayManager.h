#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSReplayManager.generated.h"

struct FReplayMetadata;

/**
 * ??��目標數量
 */
UENUM(BlueprintType)
enum class EReplaySortMethod: uint8 {
    DateNewest          UMETA(DisplayName = "摧毀(??X"),
    DateOldest          UMETA(DisplayName = "摧毀(??X"),
    NameAscending       UMETA(DisplayName = "??��(摧毀)"),
    NameDescending      UMETA(DisplayName = "??��(摧毀)"),
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
 * ??���?X- ��?X?��??��目標數量�˯�
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGRTS_API UMingRTSReplayManager : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSReplayManager();
    
    // ???X
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void InitializeReplayManager();
    
    // 目標數量?X
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
    
    // �ɥX??��摧毀�w��??
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool ExportReplay(const FString& ReplayID, const FString& ExportPath);
    
    // �q??��?X??X
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    FReplayMetadata ImportReplay(const FString& FilePath);
    
    // 摧毀�s�x�ϥ�摧毀 (MB)
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    float GetStorageUsage() const;
    
    // 摧毀�s�x摧毀 (MB)
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    float GetStorageQuota() const;
    
    // �]�m�s�x摧毀
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void SetStorageQuota(float QuotaMB);
    
    // �M?X??X
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    int32 CleanupOldReplays(int32 DaysOld);
    
    // ??��??��??��??
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool CreateReplayFolder(const FString& FolderName);
    
    // ???X?��摧毀
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FString> GetReplayFolders() const;
    
    // ��?X?��摧毀��
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool MoveReplayToFolder(const FString& ReplayID, const FString& FolderName);
    
    // ??��??��摧毀
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void AddReplayToFavorites(const FString& ReplayID);
    
    // ����摧毀
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void RemoveReplayFromFavorites(const FString& ReplayID);
    
    // 故事選項X
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


