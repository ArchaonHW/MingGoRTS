#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAIContentTypes.h"
#include "MingAIBatchGenerationPipeline.generated.h"

UENUM(BlueprintType)
enum class EMingBatchStatus : uint8
{
    Pending UMETA(DisplayName = "Pending"),
    Processing UMETA(DisplayName = "Processing"),
    Paused UMETA(DisplayName = "Paused"),
    Completed UMETA(DisplayName = "Completed"),
    Failed UMETA(DisplayName = "Failed"),
    Cancelled UMETA(DisplayName = "Cancelled"),
    Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMingBatchPriority : uint8
{
    Low UMETA(DisplayName = "Low"),
    Normal UMETA(DisplayName = "Normal"),
    High UMETA(DisplayName = "High"),
    Critical UMETA(DisplayName = "Critical"),
    Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FMingBatchItem
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Batch Generation")
    FGuid ItemID;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Generation")
    FMingAIContentRequest Request;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Generation")
    EMingBatchStatus Status;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Generation")
    float Progress;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Generation")
    FString ResultPath;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Generation")
    FString ErrorMessage;

    FMingBatchItem()
        : Status(EMingBatchStatus::Pending)
        , Progress(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingBatchConfiguration
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    FString BatchName;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    EMingBatchPriority Priority;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    int32 MaxConcurrentTasks;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    bool bAutoStart;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    bool bAutoRetryOnFailure;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    int32 MaxRetries;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    float RetryDelay;

    FMingBatchConfiguration()
        : Priority(EMingBatchPriority::Normal)
        , MaxConcurrentTasks(3)
        , bAutoStart(true)
        , bAutoRetryOnFailure(true)
        , MaxRetries(3)
        , RetryDelay(5.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingBatchStatistics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Batch Statistics")
    int32 TotalItems;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Statistics")
    int32 CompletedItems;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Statistics")
    int32 FailedItems;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Statistics")
    int32 PendingItems;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Statistics")
    float OverallProgress;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Statistics")
    float TotalTime;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Statistics")
    float AverageTimePerItem;

    FMingBatchStatistics()
        : TotalItems(0)
        , CompletedItems(0)
        , FailedItems(0)
        , PendingItems(0)
        , OverallProgress(0.0f)
        , TotalTime(0.0f)
        , AverageTimePerItem(0.0f)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBatchItemCompleted, const FGuid&, BatchID, const FGuid&, ItemID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBatchItemFailed, const FGuid&, BatchID, const FGuid&, ItemID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBatchProgress, const FGuid&, BatchID, float, Progress, int32, CompletedCount};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBatchCompleted, const FGuid&, BatchID};

/**
 * AI Batch Generation Pipeline
 * Manages batch generation of multiple AI content items
 */
UCLASS(ClassGroup = (AI, Content), Blueprintable)
class MINGAI_API UMingAIBatchGenerationPipeline : public UObject
{
    GENERATED_BODY()

public:
    UMingAIBatchGenerationPipeline();

    UFUNCTION(BlueprintCallable, Category = "Batch Generation")
    void InitializePipeline();

    UFUNCTION(BlueprintCallable, Category = "Batch Generation")
    void ShutdownPipeline();

    // Batch Management
    UFUNCTION(BlueprintCallable, Category = "Batch Generation")
    FGuid CreateBatch(const FString& BatchName, const FMingBatchConfiguration& Config};

    UFUNCTION(BlueprintCallable, Category = "Batch Generation")
    bool AddItemToBatch(const FGuid& BatchID, const FMingAIContentRequest& Request};

    UFUNCTION(BlueprintCallable, Category = "Batch Generation")
    bool AddItemsToBatch(const FGuid& BatchID, const TArray<FMingAIContentRequest>& Requests};

    UFUNCTION(BlueprintCallable, Category = "Batch Generation")
    bool RemoveItemFromBatch(const FGuid& BatchID, const FGuid& ItemID};

    UFUNCTION(BlueprintCallable, Category = "Batch Generation")
    bool StartBatch(const FGuid& BatchID};

    UFUNCTION(BlueprintCallable, Category = "Batch Generation")
    bool PauseBatch(const FGuid& BatchID};

    UFUNCTION(BlueprintCallable, Category = "Batch Generation")
    bool ResumeBatch(const FGuid& BatchID};

    UFUNCTION(BlueprintCallable, Category = "Batch Generation")
    bool CancelBatch(const FGuid& BatchID};

    UFUNCTION(BlueprintCallable, Category = "Batch Generation")
    bool RetryFailedItems(const FGuid& BatchID};

    // Batch Information
    UFUNCTION(BlueprintPure, Category = "Batch Generation")
    EMingBatchStatus GetBatchStatus(const FGuid& BatchID) const;

    UFUNCTION(BlueprintPure, Category = "Batch Generation")
    FMingBatchStatistics GetBatchStatistics(const FGuid& BatchID) const;

    UFUNCTION(BlueprintPure, Category = "Batch Generation")
    TArray<FGuid> GetBatchItemIDs(const FGuid& BatchID) const;

    UFUNCTION(BlueprintPure, Category = "Batch Generation")
    FMingBatchItem GetBatchItem(const FGuid& BatchID, const FGuid& ItemID) const;

    UFUNCTION(BlueprintPure, Category = "Batch Generation")
    TArray<FGuid> GetActiveBatchIDs() const;

    // Preset Batches
    UFUNCTION(BlueprintCallable, Category = "Batch Generation")
    FGuid CreateCharacterBatch(const TArray<FString>& CharacterDescriptions, EMingAIQualityLevel Quality};

    UFUNCTION(BlueprintCallable, Category = "Batch Generation")
    FGuid CreateEnvironmentBatch(const TArray<FString>& EnvironmentDescriptions, EMingAIQualityLevel Quality};

    UFUNCTION(BlueprintCallable, Category = "Batch Generation")
    FGuid CreateSFXBatch(const TArray<FString>& SoundDescriptions, EMingAIQualityLevel Quality};

    UFUNCTION(BlueprintCallable, Category = "Batch Generation")
    FGuid CreateMusicBatch(const TArray<FString>& MusicDescriptions, EMingAIQualityLevel Quality};

    UFUNCTION(BlueprintCallable, Category = "Batch Generation")
    FGuid CreateCompleteProjectBatch(const FString& ProjectName, const TArray<FMingAIContentRequest>& AllRequirements};

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Batch Events")
    FOnBatchItemCompleted OnItemCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Batch Events")
    FOnBatchItemFailed OnItemFailed;

    UPROPERTY(BlueprintAssignable, Category = "Batch Events")
    FOnBatchProgress OnBatchProgress;

    UPROPERTY(BlueprintAssignable, Category = "Batch Events")
    FOnBatchCompleted OnBatchCompleted;

protected:
    UPROPERTY()
    TMap<FGuid, FMingBatchConfiguration> BatchConfigs;

    UPROPERTY()
    TMap<FGuid, TArray<FGuid>> BatchItems;

    UPROPERTY()
    TMap<FGuid, FMingBatchItem> ItemData;

    UPROPERTY()
    TMap<FGuid, EMingBatchStatus> BatchStatuses;

    UPROPERTY()
    TSet<FGuid> ActiveBatches;

    UPROPERTY()
    int32 MaxConcurrentTasks;

    UPROPERTY()
    bool bInitialized;

    // Internal processing
    void ProcessBatch(FGuid BatchID};
    void ProcessNextItem(FGuid BatchID};
    void UpdateBatchProgress(FGuid BatchID};
    void OnItemGenerationCompleted(FGuid BatchID, FGuid ItemID, bool bSuccess, const FString& ResultPath};
    void CompleteBatch(FGuid BatchID};
    
    bool CanProcessMoreItems(FGuid BatchID) const;
    int32 GetActiveItemCount(FGuid BatchID) const;
    FMingBatchStatistics CalculateStatistics(FGuid BatchID) const;
};

