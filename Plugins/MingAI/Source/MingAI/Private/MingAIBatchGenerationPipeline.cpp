#include "MingAIBatchGenerationPipeline.h"
#include "MingAIGeneratedContentSystem.h"

UMingAIBatchGenerationPipeline::UMingAIBatchGenerationPipeline()
    : MaxConcurrentTasks(5)
    , bInitialized(false)
{
}

void UMingAIBatchGenerationPipeline::InitializePipeline()
{
    if (bInitialized)
    {
        return;
    }

    MaxConcurrentTasks = 5;
    bInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("AI Batch Generation Pipeline initialized"));
}

void UMingAIBatchGenerationPipeline::ShutdownPipeline()
{
    // Cancel all active batches
    for (const FGuid& BatchID : ActiveBatches)
    {
        CancelBatch(BatchID);
    }

    BatchConfigs.Empty();
    BatchItems.Empty();
    ItemData.Empty();
    BatchStatuses.Empty();
    ActiveBatches.Empty();

    bInitialized = false;
    UE_LOG(LogTemp, Log, TEXT("AI Batch Generation Pipeline shutdown"));
}

FGuid UMingAIBatchGenerationPipeline::CreateBatch(const FString& BatchName, const FMingBatchConfiguration& Config)
{
    FGuid BatchID = FGuid::NewGuid();
    
    FMingBatchConfiguration NewConfig = Config;
    NewConfig.BatchName = BatchName;
    
    BatchConfigs.Add(BatchID, NewConfig);
    BatchItems.Add(BatchID, TArray<FGuid>());
    BatchStatuses.Add(BatchID, EMingBatchStatus::Pending);
    
    UE_LOG(LogTemp, Log, TEXT("Created batch %s with ID %s"), *BatchName, *BatchID.ToString());
    
    if (Config.bAutoStart)
    {
        StartBatch(BatchID);
    }
    
    return BatchID;
}

bool UMingAIBatchGenerationPipeline::AddItemToBatch(const FGuid& BatchID, const FMingAIContentRequest& Request)
{
    if (!BatchConfigs.Contains(BatchID))
    {
        return false;
    }
    
    FGuid ItemID = FGuid::NewGuid();
    
    FMingBatchItem NewItem;
    NewItem.ItemID = ItemID;
    NewItem.Request = Request;
    NewItem.Status = EMingBatchStatus::Pending;
    NewItem.Progress = 0.0f;
    
    ItemData.Add(ItemID, NewItem);
    BatchItems[BatchID].Add(ItemID);
    
    UE_LOG(LogTemp, Log, TEXT("Added item %s to batch %s"), *ItemID.ToString(), *BatchID.ToString());
    return true;
}

bool UMingAIBatchGenerationPipeline::AddItemsToBatch(const FGuid& BatchID, const TArray<FMingAIContentRequest>& Requests)
{
    bool bAllAdded = true;
    for (const FMingAIContentRequest& Request : Requests)
    {
        if (!AddItemToBatch(BatchID, Request))
        {
            bAllAdded = false;
        }
    }
    return bAllAdded;
}

bool UMingAIBatchGenerationPipeline::RemoveItemFromBatch(const FGuid& BatchID, const FGuid& ItemID)
{
    if (!BatchItems.Contains(BatchID) || !ItemData.Contains(ItemID))
    {
        return false;
    }
    
    BatchItems[BatchID].Remove(ItemID);
    ItemData.Remove(ItemID);
    
    UE_LOG(LogTemp, Log, TEXT("Removed item %s from batch %s"), *ItemID.ToString(), *BatchID.ToString());
    return true;
}

bool UMingAIBatchGenerationPipeline::StartBatch(const FGuid& BatchID)
{
    if (!BatchConfigs.Contains(BatchID))
    {
        return false;
    }
    
    if (BatchStatuses[BatchID] != EMingBatchStatus::Pending && 
        BatchStatuses[BatchID] != EMingBatchStatus::Paused)
    {
        return false;
    }
    
    BatchStatuses[BatchID] = EMingBatchStatus::Processing;
    ActiveBatches.Add(BatchID);
    
    UE_LOG(LogTemp, Log, TEXT("Started batch %s"), *BatchID.ToString());
    
    ProcessBatch(BatchID);
    return true;
}

bool UMingAIBatchGenerationPipeline::PauseBatch(const FGuid& BatchID)
{
    if (!BatchConfigs.Contains(BatchID) || BatchStatuses[BatchID] != EMingBatchStatus::Processing)
    {
        return false;
    }
    
    BatchStatuses[BatchID] = EMingBatchStatus::Paused;
    
    UE_LOG(LogTemp, Log, TEXT("Paused batch %s"), *BatchID.ToString());
    return true;
}

bool UMingAIBatchGenerationPipeline::ResumeBatch(const FGuid& BatchID)
{
    if (!BatchConfigs.Contains(BatchID) || BatchStatuses[BatchID] != EMingBatchStatus::Paused)
    {
        return false;
    }
    
    BatchStatuses[BatchID] = EMingBatchStatus::Processing;
    
    UE_LOG(LogTemp, Log, TEXT("Resumed batch %s"), *BatchID.ToString());
    
    ProcessBatch(BatchID);
    return true;
}

bool UMingAIBatchGenerationPipeline::CancelBatch(const FGuid& BatchID)
{
    if (!BatchConfigs.Contains(BatchID))
    {
        return false;
    }
    
    BatchStatuses[BatchID] = EMingBatchStatus::Cancelled;
    ActiveBatches.Remove(BatchID);
    
    // Mark all pending items as cancelled
    if (BatchItems.Contains(BatchID))
    {
        for (const FGuid& ItemID : BatchItems[BatchID])
        {
            if (ItemData.Contains(ItemID) && ItemData[ItemID].Status == EMingBatchStatus::Pending)
            {
                ItemData[ItemID].Status = EMingBatchStatus::Cancelled;
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Cancelled batch %s"), *BatchID.ToString());
    return true;
}

bool UMingAIBatchGenerationPipeline::RetryFailedItems(const FGuid& BatchID)
{
    if (!BatchConfigs.Contains(BatchID))
    {
        return false;
    }
    
    bool bHasFailedItems = false;
    
    if (BatchItems.Contains(BatchID))
    {
        for (const FGuid& ItemID : BatchItems[BatchID])
        {
            if (ItemData.Contains(ItemID) && ItemData[ItemID].Status == EMingBatchStatus::Failed)
            {
                ItemData[ItemID].Status = EMingBatchStatus::Pending;
                ItemData[ItemID].Progress = 0.0f;
                ItemData[ItemID].ErrorMessage.Empty();
                bHasFailedItems = true;
            }
        }
    }
    
    if (bHasFailedItems)
    {
        StartBatch(BatchID);
    }
    
    return bHasFailedItems;
}

EMingBatchStatus UMingAIBatchGenerationPipeline::GetBatchStatus(const FGuid& BatchID) const
{
    if (BatchStatuses.Contains(BatchID))
    {
        return BatchStatuses[BatchID];
    }
    return EMingBatchStatus::Failed;
}

FMingBatchStatistics UMingAIBatchGenerationPipeline::GetBatchStatistics(const FGuid& BatchID) const
{
    return CalculateStatistics(BatchID);
}

TArray<FGuid> UMingAIBatchGenerationPipeline::GetBatchItemIDs(const FGuid& BatchID) const
{
    if (BatchItems.Contains(BatchID))
    {
        return BatchItems[BatchID];
    }
    return TArray<FGuid>();
}

FMingBatchItem UMingAIBatchGenerationPipeline::GetBatchItem(const FGuid& BatchID, const FGuid& ItemID) const
{
    if (ItemData.Contains(ItemID))
    {
        return ItemData[ItemID];
    }
    return FMingBatchItem();
}

TArray<FGuid> UMingAIBatchGenerationPipeline::GetActiveBatchIDs() const
{
    TArray<FGuid> Result;
    for (const FGuid& BatchID : ActiveBatches)
    {
        Result.Add(BatchID);
    }
    return Result;
}

FGuid UMingAIBatchGenerationPipeline::CreateCharacterBatch(const TArray<FString>& CharacterDescriptions, EMingAIQualityLevel Quality)
{
    FMingBatchConfiguration Config;
    Config.BatchName = TEXT("Character Generation Batch");
    Config.Priority = EMingBatchPriority::High;
    Config.MaxConcurrentTasks = 3;
    Config.bAutoStart = true;
    
    FGuid BatchID = CreateBatch(Config.BatchName, Config);
    
    for (const FString& Description : CharacterDescriptions)
    {
        FMingAIContentRequest Request;
        Request.ContentType = EMingAIContentType::Image;
        Request.Prompt = Description;
        Request.Provider = EMingAIProvider::StableDiffusion;
        Request.QualityLevel = Quality;
        Request.Width = 1024;
        Request.Height = 1024;
        
        AddItemToBatch(BatchID, Request);
    }
    
    return BatchID;
}

FGuid UMingAIBatchGenerationPipeline::CreateEnvironmentBatch(const TArray<FString>& EnvironmentDescriptions, EMingAIQualityLevel Quality)
{
    FMingBatchConfiguration Config;
    Config.BatchName = TEXT("Environment Generation Batch");
    Config.Priority = EMingBatchPriority::Normal;
    Config.MaxConcurrentTasks = 2;
    Config.bAutoStart = true;
    
    FGuid BatchID = CreateBatch(Config.BatchName, Config);
    
    for (const FString& Description : EnvironmentDescriptions)
    {
        FMingAIContentRequest Request;
        Request.ContentType = EMingAIContentType::Image;
        Request.Prompt = Description;
        Request.Provider = EMingAIProvider::StableDiffusion;
        Request.QualityLevel = Quality;
        Request.Width = 1920;
        Request.Height = 1080;
        
        AddItemToBatch(BatchID, Request);
    }
    
    return BatchID;
}

FGuid UMingAIBatchGenerationPipeline::CreateSFXBatch(const TArray<FString>& SoundDescriptions, EMingAIQualityLevel Quality)
{
    FMingBatchConfiguration Config;
    Config.BatchName = TEXT("Sound Effects Generation Batch");
    Config.Priority = EMingBatchPriority::Normal;
    Config.MaxConcurrentTasks = 5;
    Config.bAutoStart = true;
    
    FGuid BatchID = CreateBatch(Config.BatchName, Config);
    
    for (const FString& Description : SoundDescriptions)
    {
        FMingAIContentRequest Request;
        Request.ContentType = EMingAIContentType::SoundEffect;
        Request.Prompt = Description;
        Request.Provider = EMingAIProvider::Custom;
        Request.QualityLevel = Quality;
        
        AddItemToBatch(BatchID, Request);
    }
    
    return BatchID;
}

FGuid UMingAIBatchGenerationPipeline::CreateMusicBatch(const TArray<FString>& MusicDescriptions, EMingAIQualityLevel Quality)
{
    FMingBatchConfiguration Config;
    Config.BatchName = TEXT("Music Generation Batch");
    Config.Priority = EMingBatchPriority::High;
    Config.MaxConcurrentTasks = 2;
    Config.bAutoStart = true;
    
    FGuid BatchID = CreateBatch(Config.BatchName, Config);
    
    for (const FString& Description : MusicDescriptions)
    {
        FMingAIContentRequest Request;
        Request.ContentType = EMingAIContentType::Music;
        Request.Prompt = Description;
        Request.Provider = EMingAIProvider::AIVA;
        Request.QualityLevel = Quality;
        Request.AdditionalParameters.Add(TEXT("Duration"), TEXT("120.0"));
        
        AddItemToBatch(BatchID, Request);
    }
    
    return BatchID;
}

FGuid UMingAIBatchGenerationPipeline::CreateCompleteProjectBatch(const FString& ProjectName, const TArray<FMingAIContentRequest>& AllRequirements)
{
    FMingBatchConfiguration Config;
    Config.BatchName = ProjectName;
    Config.Priority = EMingBatchPriority::Critical;
    Config.MaxConcurrentTasks = 5;
    Config.bAutoStart = true;
    
    FGuid BatchID = CreateBatch(ProjectName, Config);
    AddItemsToBatch(BatchID, AllRequirements);
    
    return BatchID;
}

void UMingAIBatchGenerationPipeline::ProcessBatch(FGuid BatchID)
{
    if (!BatchConfigs.Contains(BatchID) || BatchStatuses[BatchID] != EMingBatchStatus::Processing)
    {
        return;
    }
    
    // Process items until we hit the concurrent limit
    while (CanProcessMoreItems(BatchID))
    {
        ProcessNextItem(BatchID);
    }
    
    // Check if batch is complete
    FMingBatchStatistics Stats = CalculateStatistics(BatchID);
    if (Stats.CompletedItems + Stats.FailedItems >= Stats.TotalItems)
    {
        CompleteBatch(BatchID);
    }
}

void UMingAIBatchGenerationPipeline::ProcessNextItem(FGuid BatchID)
{
    if (!BatchItems.Contains(BatchID))
    {
        return;
    }
    
    // Find next pending item
    for (const FGuid& ItemID : BatchItems[BatchID])
    {
        if (ItemData.Contains(ItemID) && ItemData[ItemID].Status == EMingBatchStatus::Pending)
        {
            ItemData[ItemID].Status = EMingBatchStatus::Processing;
            
            // Start generation
            UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
            ContentSystem->InitializeSystem();
            FGuid RequestID = ContentSystem->GenerateContent(ItemData[ItemID].Request);
            
            UE_LOG(LogTemp, Log, TEXT("Processing item %s in batch %s"), *ItemID.ToString(), *BatchID.ToString());
            break;
        }
    }
}

void UMingAIBatchGenerationPipeline::UpdateBatchProgress(FGuid BatchID)
{
    if (!BatchConfigs.Contains(BatchID))
    {
        return;
    }
    
    FMingBatchStatistics Stats = CalculateStatistics(BatchID);
    
    OnBatchProgress.Broadcast(BatchID, Stats.OverallProgress, Stats.CompletedItems);
}

void UMingAIBatchGenerationPipeline::OnItemGenerationCompleted(FGuid BatchID, FGuid ItemID, bool bSuccess, const FString& ResultPath)
{
    if (!ItemData.Contains(ItemID))
    {
        return;
    }
    
    if (bSuccess)
    {
        ItemData[ItemID].Status = EMingBatchStatus::Completed;
        ItemData[ItemID].ResultPath = ResultPath;
        ItemData[ItemID].Progress = 1.0f;
        
        OnItemCompleted.Broadcast(BatchID, ItemID);
    }
    else
    {
        ItemData[ItemID].Status = EMingBatchStatus::Failed;
        ItemData[ItemID].ErrorMessage = TEXT("Generation failed");
        
        OnItemFailed.Broadcast(BatchID, ItemID);
    }
    
    UpdateBatchProgress(BatchID);
    
    // Continue processing if batch is still active
    if (BatchStatuses.Contains(BatchID) && BatchStatuses[BatchID] == EMingBatchStatus::Processing)
    {
        ProcessBatch(BatchID);
    }
}

void UMingAIBatchGenerationPipeline::CompleteBatch(FGuid BatchID)
{
    if (!BatchConfigs.Contains(BatchID))
    {
        return;
    }
    
    BatchStatuses[BatchID] = EMingBatchStatus::Completed;
    ActiveBatches.Remove(BatchID);
    
    FMingBatchStatistics Stats = CalculateStatistics(BatchID);
    
    UE_LOG(LogTemp, Log, TEXT("Batch %s completed: %d/%d items successful"), 
        *BatchID.ToString(), Stats.CompletedItems, Stats.TotalItems);
    
    OnBatchCompleted.Broadcast(BatchID);
}

bool UMingAIBatchGenerationPipeline::CanProcessMoreItems(FGuid BatchID) const
{
    if (!BatchConfigs.Contains(BatchID))
    {
        return false;
    }
    
    int32 MaxConcurrent = BatchConfigs[BatchID].MaxConcurrentTasks;
    int32 ActiveCount = GetActiveItemCount(BatchID);
    
    return ActiveCount < MaxConcurrent;
}

int32 UMingAIBatchGenerationPipeline::GetActiveItemCount(FGuid BatchID) const
{
    if (!BatchItems.Contains(BatchID))
    {
        return 0;
    }
    
    int32 Count = 0;
    for (const FGuid& ItemID : BatchItems[BatchID])
    {
        if (ItemData.Contains(ItemID) && ItemData[ItemID].Status == EMingBatchStatus::Processing)
        {
            Count++;
        }
    }
    return Count;
}

FMingBatchStatistics UMingAIBatchGenerationPipeline::CalculateStatistics(FGuid BatchID) const
{
    FMingBatchStatistics Stats;
    
    if (!BatchItems.Contains(BatchID))
    {
        return Stats;
    }
    
    Stats.TotalItems = BatchItems[BatchID].Num();
    
    for (const FGuid& ItemID : BatchItems[BatchID])
    {
        if (ItemData.Contains(ItemID))
        {
            const FMingBatchItem& Item = ItemData[ItemID];
            
            switch (Item.Status)
            {
            case EMingBatchStatus::Completed:
                Stats.CompletedItems++;
                break;
            case EMingBatchStatus::Failed:
                Stats.FailedItems++;
                break;
            case EMingBatchStatus::Pending:
                Stats.PendingItems++;
                break;
            default:
                break;
            }
        }
    }
    
    if (Stats.TotalItems > 0)
    {
        Stats.OverallProgress = static_cast<float>(Stats.CompletedItems + Stats.FailedItems) / Stats.TotalItems;
    }
    
    return Stats;
}
