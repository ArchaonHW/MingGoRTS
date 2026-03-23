#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSReplayManager.generated.h"

struct FReplayMetadata;

/**
 * 動��目標數量
 */
UENUM(BlueprintType)
enum class EReplaySortMethod: uuint8 {
    DateNewest          UMETA(DisplayName = "(動X"),
    DateOldest          UMETA(DisplayName = "(動X"),
    NameAscending       UMETA(DisplayName = "動��()"),
    NameDescending      UMETA(DisplayName = "動��()"),
    DurationLongest     UMETA(DisplayName = "動��(動X"),
    DurationShortest    UMETA(DisplayName = "動��(動X")
};

/**
 * 動��動�o����
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
 * 動���務- ��池��動��目標數量�˯�
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableConponent))
class MINGRTS_API UMingRTSReplayManager : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSReplayManager(};
    
    // 動務
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void InitializeReplayManager(};
    
    // 目標數量務
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> GetAllReplays() const;
    
    // 動�o動��
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> FilterReplays(const FReplayFilter& Filter) const;
    
    // 動池��
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> SortReplays(const TArray<FReplayMetadata>& Replays, EReplaySortMethod SortMethod) const;
    
    // 動��動��
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> SearchReplays(const FString& SearchQuery) const;
    
    // 動池���動
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    FReplayMetadata GetReplayMetadata(const FString& ReplayID) const;
    
    // 動�R動務
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool RenameReplay(const FString& ReplayID, const FString& NewName};
    
    // 動��動��
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool DeleteReplay(const FString& ReplayID};
    
    // 動池��
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    int32 DeleteReplaysBatch(const TArray<FString>& ReplayIDs};
    
    // �ɥrX動���w��動
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool ExportReplay(const FString& ReplayID, const FString& ExportPath};
    
    // �q動��池務
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    FReplayMetadata ImportReplay(const FString& FilePath};
    
    // �s�x�ϥ� (MB)
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    float GetStorageUsage() const;
    
    // �s�x (MB)
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    float GetStorageQuota() const;
    
    // �]�m�s�x
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void SetStorageQuota(float QuotaMB};
    
    // �M池務
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    int32 CleanupOldReplays(int32 DaysOld};
    
    // 動��動��動��動
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool CreateReplayFolder(const FString& FolderName};
    
    // 動池��
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FString> GetReplayFolders() const;
    
    // ��池����
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    bool MoveReplayToFolder(const FString& ReplayID, const FString& FolderName};
    
    // 動��動��
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void AddReplayToFavorites(const FString& ReplayID};
    
    // ����
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    void RemoveReplayFromFavorites(const FString& ReplayID};
    
    // 故事選項X
    UFUNCTION(BlueprintCallable, Category = "ReplayManager")
    TArray<FReplayMetadata> GetFavoriteReplays() const;
    
    // �ɥ礎�e動
    
    
    
    
    UPROPERTY(BlueprintAssignable, Category = "ReplayManagerEvents")
    FOnReplayAdded OnReplayAdded;
    
    UPROPERTY(BlueprintAssignable, Category = "ReplayManagerEvents")
    FOnReplayDeleted OnReplayDeleted;
    
    UPROPERTY(BlueprintAssignable, Category = "ReplayManagerEvents")
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


