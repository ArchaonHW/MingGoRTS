#include "Replay/MingRTSReplayManager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Replay/MingRTSReplaySystem.h"

UMingRTSReplayManager::UMingRTSReplayManager()
    : StorageQuotaMB(1024.0f) // 默認1GB配額
{
}

void UMingRTSReplayManager::InitializeReplayManager()
{
    // 確保回放目錄存在
    FString ReplayDir = GetReplayDirectory();
    IFileManager::Get().MakeDirectory(*ReplayDir, true);
    
    // 載入收藏列表
    // 這裡應該從配置文件載入，簡化處理
    
    UE_LOG(LogTemp, Log, TEXT("Replay Manager Initialized"));
}

TArray<FReplayMetadata> UMingRTSReplayManager::GetAllReplays() const
{
    TArray<FReplayMetadata> Replays;
    
    FString ReplayDir = GetReplayDirectory();
    TArray<FString> Files;
    IFileManager::Get().FindFiles(Files, *(ReplayDir / TEXT("*.replaymetadata")), true, false);
    
    for (const FString& File : Files)
    {
        FString ReplayID = FPaths::GetBaseFilename(File);
        FReplayMetadata Metadata;
        if (LoadMetadataFromFile(ReplayID, Metadata))
        {
            Replays.Add(Metadata);
        }
    }
    
    return Replays;
}

TArray<FReplayMetadata> UMingRTSReplayManager::FilterReplays(const FReplayFilter& Filter) const
{
    TArray<FReplayMetadata> AllReplays = GetAllReplays();
    TArray<FReplayMetadata> FilteredReplays;
    
    for (const FReplayMetadata& Replay : AllReplays)
    {
        bool bMatch = true;
        
        // 地圖名稱過濾
        if (!Filter.MapNameFilter.IsEmpty())
        {
            if (!Replay.MapName.Contains(Filter.MapNameFilter))
            {
                bMatch = false;
            }
        }
        
        // 玩家過濾
        if (!Filter.PlayerFilter.IsEmpty())
        {
            bool bPlayerFound = false;
            for (const FString& Player : Replay.Players)
            {
                if (Player.Contains(Filter.PlayerFilter))
                {
                    bPlayerFound = true;
                    break;
                }
            }
            if (!bPlayerFound)
            {
                bMatch = false;
            }
        }
        
        // 時長過濾
        if (Filter.MinDuration > 0.0f && Replay.Duration < Filter.MinDuration)
        {
            bMatch = false;
        }
        
        if (Filter.MaxDuration > 0.0f && Replay.Duration > Filter.MaxDuration)
        {
            bMatch = false;
        }
        
        // 日期過濾
        if (!Filter.DateFrom.IsEmpty())
        {
            if (Replay.CreationDate < Filter.DateFrom)
            {
                bMatch = false;
            }
        }
        
        if (!Filter.DateTo.IsEmpty())
        {
            if (Replay.CreationDate > Filter.DateTo)
            {
                bMatch = false;
            }
        }
        
        // 版本過濾
        if (!Filter.GameVersion.IsEmpty())
        {
            if (!Replay.GameVersion.Contains(Filter.GameVersion))
            {
                bMatch = false;
            }
        }
        
        if (bMatch)
        {
            FilteredReplays.Add(Replay);
        }
    }
    
    return FilteredReplays;
}

TArray<FReplayMetadata> UMingRTSReplayManager::SortReplays(const TArray<FReplayMetadata>& Replays, EReplaySortMethod SortMethod) const
{
    TArray<FReplayMetadata> SortedReplays = Replays;
    
    switch (SortMethod)
    {
    case EReplaySortMethod::DateNewest:
        SortedReplays.Sort([](const FReplayMetadata& A, const FReplayMetadata& B) {
            return A.CreationDate > B.CreationDate;
        });
        break;
        
    case EReplaySortMethod::DateOldest:
        SortedReplays.Sort([](const FReplayMetadata& A, const FReplayMetadata& B) {
            return A.CreationDate < B.CreationDate;
        });
        break;
        
    case EReplaySortMethod::NameAscending:
        SortedReplays.Sort([](const FReplayMetadata& A, const FReplayMetadata& B) {
            return A.ReplayName < B.ReplayName;
        });
        break;
        
    case EReplaySortMethod::NameDescending:
        SortedReplays.Sort([](const FReplayMetadata& A, const FReplayMetadata& B) {
            return A.ReplayName > B.ReplayName;
        });
        break;
        
    case EReplaySortMethod::DurationLongest:
        SortedReplays.Sort([](const FReplayMetadata& A, const FReplayMetadata& B) {
            return A.Duration > B.Duration;
        });
        break;
        
    case EReplaySortMethod::DurationShortest:
        SortedReplays.Sort([](const FReplayMetadata& A, const FReplayMetadata& B) {
            return A.Duration < B.Duration;
        });
        break;
    }
    
    return SortedReplays;
}

TArray<FReplayMetadata> UMingRTSReplayManager::SearchReplays(const FString& SearchQuery) const
{
    TArray<FReplayMetadata> AllReplays = GetAllReplays();
    TArray<FReplayMetadata> SearchResults;
    
    FString LowerQuery = SearchQuery.ToLower();
    
    for (const FReplayMetadata& Replay : AllReplays)
    {
        if (Replay.ReplayName.ToLower().Contains(LowerQuery) ||
            Replay.MapName.ToLower().Contains(LowerQuery) ||
            Replay.MatchID.ToLower().Contains(LowerQuery))
        {
            SearchResults.Add(Replay);
        }
    }
    
    return SearchResults;
}

FReplayMetadata UMingRTSReplayManager::GetReplayMetadata(const FString& ReplayID) const
{
    FReplayMetadata Metadata;
    LoadMetadataFromFile(ReplayID, Metadata);
    return Metadata;
}

bool UMingRTSReplayManager::RenameReplay(const FString& ReplayID, const FString& NewName)
{
    FReplayMetadata Metadata;
    if (!LoadMetadataFromFile(ReplayID, Metadata))
    {
        return false;
    }
    
    Metadata.ReplayName = NewName;
    
    if (SaveMetadataToFile(ReplayID, Metadata))
    {
        OnReplayRenamed.Broadcast(ReplayID);
        UE_LOG(LogTemp, Log, TEXT("Renamed replay %s to %s"), *ReplayID, *NewName);
        return true;
    }
    
    return false;
}

bool UMingRTSReplayManager::DeleteReplay(const FString& ReplayID)
{
    // 刪除回放文件
    FString ReplayFile = GetReplayDirectory() / ReplayID + TEXT(".replay");
    FString MetadataFile = GetMetadataFilePath(ReplayID);
    
    bool bSuccess = true;
    
    if (IFileManager::Get().FileExists(*ReplayFile))
    {
        if (!IFileManager::Get().Delete(*ReplayFile))
        {
            bSuccess = false;
        }
    }
    
    if (IFileManager::Get().FileExists(*MetadataFile))
    {
        if (!IFileManager::Get().Delete(*MetadataFile))
        {
            bSuccess = false;
        }
    }
    
    if (bSuccess)
    {
        // 從收藏中移除
        FavoriteReplayIDs.Remove(ReplayID);
        
        // 從文件夾映射中移除
        ReplayFolderMap.Remove(ReplayID);
        
        OnReplayDeleted.Broadcast(ReplayID);
        UE_LOG(LogTemp, Log, TEXT("Deleted replay: %s"), *ReplayID);
    }
    
    return bSuccess;
}

int32 UMingRTSReplayManager::DeleteReplaysBatch(const TArray<FString>& ReplayIDs)
{
    int32 DeletedCount = 0;
    
    for (const FString& ReplayID : ReplayIDs)
    {
        if (DeleteReplay(ReplayID))
        {
            DeletedCount++;
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch deleted %d replays"), DeletedCount);
    return DeletedCount;
}

bool UMingRTSReplayManager::ExportReplay(const FString& ReplayID, const FString& ExportPath)
{
    FString SourcePath = GetReplayDirectory() / ReplayID + TEXT(".replay");
    
    if (!IFileManager::Get().FileExists(*SourcePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Replay not found: %s"), *ReplayID);
        return false;
    }
    
    // 創建目標目錄
    FString TargetDir = FPaths::GetPath(ExportPath);
    IFileManager::Get().MakeDirectory(*TargetDir, true);
    
    if (IFileManager::Get().Copy(*ExportPath, *SourcePath, true, true))
    {
        // 同時導出元數據
        FString MetadataSource = GetMetadataFilePath(ReplayID);
        FString MetadataTarget = ExportPath + TEXT(".metadata");
        IFileManager::Get().Copy(*MetadataTarget, *MetadataSource, true, true);
        
        UE_LOG(LogTemp, Log, TEXT("Exported replay %s to %s"), *ReplayID, *ExportPath);
        return true;
    }
    
    UE_LOG(LogTemp, Error, TEXT("Failed to export replay %s"), *ReplayID);
    return false;
}

FReplayMetadata UMingRTSReplayManager::ImportReplay(const FString& FilePath)
{
    FReplayMetadata Metadata;
    
    if (!IFileManager::Get().FileExists(*FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Import file not found: %s"), *FilePath);
        return Metadata;
    }
    
    // 生成新的回放ID
    FString NewReplayID = FString::Printf(TEXT("IMPORT_%s"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
    FString DestPath = GetReplayDirectory() / NewReplayID + TEXT(".replay");
    
    if (IFileManager::Get().Copy(*DestPath, *FilePath, true, true))
    {
        // 嘗試載入元數據
        FString MetadataSource = FilePath + TEXT(".metadata");
        FString MetadataDest = GetMetadataFilePath(NewReplayID);
        
        if (IFileManager::Get().FileExists(*MetadataSource))
        {
            IFileManager::Get().Copy(*MetadataDest, *MetadataSource, true, true);
            LoadMetadataFromFile(NewReplayID, Metadata);
        }
        
        Metadata.ReplayID = NewReplayID;
        
        OnReplayAdded.Broadcast(Metadata);
        UE_LOG(LogTemp, Log, TEXT("Imported replay as %s"), *NewReplayID);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to import replay"));
    }
    
    return Metadata;
}

float UMingRTSReplayManager::GetStorageUsage() const
{
    FString ReplayDir = GetReplayDirectory();
    
    int64 TotalSize = 0;
    TArray<FString> Files;
    IFileManager::Get().FindFilesRecursive(Files, *ReplayDir, TEXT("*"), true, false);
    
    for (const FString& File : Files)
    {
        TotalSize += IFileManager::Get().FileSize(*(ReplayDir / File));
    }
    
    // 轉換為MB
    return TotalSize / (1024.0f * 1024.0f);
}

float UMingRTSReplayManager::GetStorageQuota() const
{
    return StorageQuotaMB;
}

void UMingRTSReplayManager::SetStorageQuota(float QuotaMB)
{
    StorageQuotaMB = QuotaMB;
    UE_LOG(LogTemp, Log, TEXT("Storage quota set to %.1f MB"), StorageQuotaMB);
}

int32 UMingRTSReplayManager::CleanupOldReplays(int32 DaysOld)
{
    TArray<FReplayMetadata> AllReplays = GetAllReplays();
    int32 DeletedCount = 0;
    
    FDateTime Now = FDateTime::Now();
    FTimespan AgeThreshold = FTimespan::FromDays(DaysOld);
    
    for (const FReplayMetadata& Replay : AllReplays)
    {
        FDateTime CreationDate;
        if (FDateTime::Parse(Replay.CreationDate, CreationDate))
        {
            if (Now - CreationDate > AgeThreshold)
            {
                if (DeleteReplay(Replay.ReplayID))
                {
                    DeletedCount++;
                }
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Cleaned up %d old replays (older than %d days)"), DeletedCount, DaysOld);
    return DeletedCount;
}

bool UMingRTSReplayManager::CreateReplayFolder(const FString& FolderName)
{
    FString FolderPath = GetReplayDirectory() / FolderName;
    return IFileManager::Get().MakeDirectory(*FolderPath, true);
}

TArray<FString> UMingRTSReplayManager::GetReplayFolders() const
{
    TArray<FString> Folders;
    
    FString ReplayDir = GetReplayDirectory();
    IFileManager::Get().FindFiles(Folders, *(ReplayDir / TEXT("*")), false, true);
    
    return Folders;
}

bool UMingRTSReplayManager::MoveReplayToFolder(const FString& ReplayID, const FString& FolderName)
{
    // 更新文件夾映射
    if (FolderName.IsEmpty())
    {
        ReplayFolderMap.Remove(ReplayID);
    }
    else
    {
        ReplayFolderMap.Add(ReplayID, FolderName);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Moved replay %s to folder %s"), *ReplayID, *FolderName);
    return true;
}

void UMingRTSReplayManager::AddReplayToFavorites(const FString& ReplayID)
{
    if (!FavoriteReplayIDs.Contains(ReplayID))
    {
        FavoriteReplayIDs.Add(ReplayID);
        UE_LOG(LogTemp, Log, TEXT("Added replay %s to favorites"), *ReplayID);
    }
}

void UMingRTSReplayManager::RemoveReplayFromFavorites(const FString& ReplayID)
{
    FavoriteReplayIDs.Remove(ReplayID);
    UE_LOG(LogTemp, Log, TEXT("Removed replay %s from favorites"), *ReplayID);
}

TArray<FReplayMetadata> UMingRTSReplayManager::GetFavoriteReplays() const
{
    TArray<FReplayMetadata> Favorites;
    
    for (const FString& ReplayID : FavoriteReplayIDs)
    {
        FReplayMetadata Metadata;
        if (LoadMetadataFromFile(ReplayID, Metadata))
        {
            Favorites.Add(Metadata);
        }
    }
    
    return Favorites;
}

FString UMingRTSReplayManager::GetReplayDirectory() const
{
    return FPaths::ProjectSavedDir() / TEXT("Replays");
}

FString UMingRTSReplayManager::GetMetadataFilePath(const FString& ReplayID) const
{
    return GetReplayDirectory() / ReplayID + TEXT(".replaymetadata");
}

bool UMingRTSReplayManager::LoadMetadataFromFile(const FString& ReplayID, FReplayMetadata& OutMetadata) const
{
    FString FilePath = GetMetadataFilePath(ReplayID);
    
    if (!IFileManager::Get().FileExists(*FilePath))
    {
        return false;
    }
    
    FString JsonData;
    if (!FFileHelper::LoadFileToString(JsonData, *FilePath))
    {
        return false;
    }
    
    // 簡化的JSON解析，實際應該使用JSON庫
    // 這裡只是一個示例實現
    OutMetadata.ReplayID = ReplayID;
    
    // 從JSON中提取字段
    // 注意：這裡應該使用正式的JSON解析
    
    return true;
}

bool UMingRTSReplayManager::SaveMetadataToFile(const FString& ReplayID, const FReplayMetadata& Metadata)
{
    FString FilePath = GetMetadataFilePath(ReplayID);
    
    // 簡化的JSON生成
    FString JsonData;
    JsonData += FString::Printf(TEXT("{\n"));
    JsonData += FString::Printf(TEXT("  \"ReplayID\": \"%s\",\n"), *Metadata.ReplayID);
    JsonData += FString::Printf(TEXT("  \"ReplayName\": \"%s\",\n"), *Metadata.ReplayName);
    JsonData += FString::Printf(TEXT("  \"MatchID\": \"%s\",\n"), *Metadata.MatchID);
    JsonData += FString::Printf(TEXT("  \"MapName\": \"%s\",\n"), *Metadata.MapName);
    JsonData += FString::Printf(TEXT("  \"GameVersion\": \"%s\",\n"), *Metadata.GameVersion);
    JsonData += FString::Printf(TEXT("  \"CreationDate\": \"%s\",\n"), *Metadata.CreationDate);
    JsonData += FString::Printf(TEXT("  \"Duration\": %.2f,\n"), Metadata.Duration);
    JsonData += FString::Printf(TEXT("  \"TotalFrames\": %d,\n"), Metadata.TotalFrames);
    JsonData += FString::Printf(TEXT("  \"TotalEvents\": %d,\n"), Metadata.TotalEvents);
    JsonData += FString::Printf(TEXT("  \"FileSize\": %d\n"), Metadata.FileSize);
    JsonData += FString::Printf(TEXT("}\n"));
    
    return FFileHelper::SaveStringToFile(JsonData, *FilePath);
}
