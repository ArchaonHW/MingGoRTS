#include "MingAIAssetManager.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Engine/Engine.h"

// 靜態實例初始化
UMingAIAssetManager* UMingAIAssetManager::Instance = nullptr;

UMingAIAssetManager::UMingAIAssetManager()
    : bIsInitialized(false)
{
    // 設置路徑
    AssetDatabasePath = FPaths::ProjectSavedDir() / TEXT("AIAssetDatabase.json");
    AssetStorageDirectory = FPaths::ProjectSavedDir() / TEXT("AIAssets");
    
    // 預分配容量
    AssetList.Reserve(1000);
}

void UMingAIAssetManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }
    
    // 創建資產目錄
    if (!CreateAssetDirectories())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create asset directories"));
        return;
    }
    
    // 加載資產數據庫
    LoadAssetDatabase();
    
    bIsInitialized = true;
    Instance = this;
    
    UE_LOG(LogTemp, Log, TEXT("AIAssetManager initialized"));
    UE_LOG(LogTemp, Log, TEXT("Database path: %s"), *AssetDatabasePath);
    UE_LOG(LogTemp, Log, TEXT("Storage directory: %s"), *AssetStorageDirectory);
    UE_LOG(LogTemp, Log, TEXT("Loaded %d assets"), AssetList.Num());
}

void UMingAIAssetManager::Shutdown()
{
    if (!bIsInitialized)
    {
        return;
    }
    
    // 保存資產數據庫
    SaveAssetDatabase();
    
    AssetList.Empty();
    bIsInitialized = false;
    Instance = nullptr;
    
    UE_LOG(LogTemp, Log, TEXT("AIAssetManager shutdown"));
}

bool UMingAIAssetManager::AddAsset(const FAIAssetGenerationResult& GenerationResult)
{
    if (!bIsInitialized || !GenerationResult.bSuccess)
    {
        return false;
    }
    
    // 檢查是否已存在
    for (const FAssetMetadata& Asset : AssetList)
    {
        if (Asset.AssetID == GenerationResult.AssetID)
        {
            UE_LOG(LogTemp, Warning, TEXT("Asset %s already exists"), *GenerationResult.AssetID);
            return false;
        }
    }
    
    // 創建資產元數據
    FAssetMetadata Metadata = CreateAssetMetadata(GenerationResult);
    
    // 驗證資產文件
    if (!ValidateAssetFile(Metadata.FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Asset file validation failed: %s"), *Metadata.FilePath);
        return false;
    }
    
    // 添加到列表
    AssetList.Add(Metadata);
    
    // 保存數據庫
    SaveAssetDatabase();
    
    UE_LOG(LogTemp, Log, TEXT("Asset added: %s (%s)"), *Metadata.AssetID, *Metadata.AssetName);
    
    return true;
}

bool UMingAIAssetManager::RemoveAsset(const FString& AssetID)
{
    if (!bIsInitialized)
    {
        return false;
    }
    
    for (int32 i = 0; i < AssetList.Num(); ++i)
    {
        if (AssetList[i].AssetID == AssetID)
        {
            const FAssetMetadata& Asset = AssetList[i];
            
            // 刪除文件
            IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
            
            if (!Asset.FilePath.IsEmpty())
            {
                PlatformFile.DeleteFile(*Asset.FilePath);
            }
            
            if (!Asset.ThumbnailPath.IsEmpty())
            {
                PlatformFile.DeleteFile(*Asset.ThumbnailPath);
            }
            
            // 從列表移除
            AssetList.RemoveAt(i);
            
            // 保存數據庫
            SaveAssetDatabase();
            
            UE_LOG(LogTemp, Log, TEXT("Asset removed: %s"), *AssetID);
            
            return true;
        }
    }
    
    return false;
}

FAssetMetadata UMingAIAssetManager::GetAsset(const FString& AssetID) const
{
    for (const FAssetMetadata& Asset : AssetList)
    {
        if (Asset.AssetID == AssetID)
        {
            return Asset;
        }
    }
    
    return FAssetMetadata();
}

TArray<FAssetMetadata> UMingAIAssetManager::SearchAssets(const FAssetFilter& Filter) const
{
    TArray<FAssetMetadata> FilteredAssets;
    
    for (const FAssetMetadata& Asset : AssetList)
    {
        if (MatchesFilter(Asset, Filter))
        {
            FilteredAssets.Add(Asset);
        }
    }
    
    // 排序
    SortAssets(FilteredAssets, TEXT("CreatedTime"));
    
    return FilteredAssets;
}

TArray<FAssetMetadata> UMingAIAssetManager::GetAllAssets() const
{
    return AssetList;
}

TMap<EAssetCategory, int32> UMingAIAssetManager::GetAssetStatistics() const
{
    TMap<EAssetCategory, int32> Statistics;
    
    // 初始化所有分類
    for (int32 i = 0; i < static_cast<int32>(EAssetCategory::Voices) + 1; ++i)
    {
        Statistics.Add(static_cast<EAssetCategory>(i), 0);
    }
    
    // 統計資產
    for (const FAssetMetadata& Asset : AssetList)
    {
        int32* Count = Statistics.Find(Asset.Category);
        if (Count)
        {
            (*Count)++;
        }
    }
    
    return Statistics;
}

bool UMingAIAssetManager::SetAssetFavorite(const FString& AssetID, bool bFavorite)
{
    for (FAssetMetadata& Asset : AssetList)
    {
        if (Asset.AssetID == AssetID)
        {
            Asset.bIsFavorite = bFavorite;
            SaveAssetDatabase();
            
            UE_LOG(LogTemp, Log, TEXT("Asset %s favorite status: %s"), 
                *AssetID, bFavorite ? TEXT("true") : TEXT("false"));
            
            return true;
        }
    }
    
    return false;
}

bool UMingAIAssetManager::RateAsset(const FString& AssetID, float Rating)
{
    for (FAssetMetadata& Asset : AssetList)
    {
        if (Asset.AssetID == AssetID)
        {
            Asset.Rating = FMath::Clamp(Rating, 1.0f, 5.0f);
            SaveAssetDatabase();
            
            UE_LOG(LogTemp, Log, TEXT("Asset %s rated: %.1f"), *AssetID, Asset.Rating);
            
            return true;
        }
    }
    
    return false;
}

bool UMingAIAssetManager::IncrementUsage(const FString& AssetID)
{
    for (FAssetMetadata& Asset : AssetList)
    {
        if (Asset.AssetID == AssetID)
        {
            Asset.UsageCount++;
            SaveAssetDatabase();
            
            UE_LOG(LogTemp, Verbose, TEXT("Asset %s usage count: %d"), *AssetID, Asset.UsageCount);
            
            return true;
        }
    }
    
    return false;
}

bool UMingAIAssetManager::ExportAsset(const FString& AssetID, const FString& ExportPath) const
{
    const FAssetMetadata Asset = GetAsset(AssetID);
    
    if (Asset.AssetID.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Asset not found: %s"), *AssetID);
        return false;
    }
    
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    // 複製資產文件
    bool bSuccess = PlatformFile.CopyFile(*ExportPath, *Asset.FilePath);
    
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Asset exported: %s -> %s"), *AssetID, *ExportPath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to export asset: %s"), *AssetID);
    }
    
    return bSuccess;
}

bool UMingAIAssetManager::ExportAssets(const TArray<FString>& AssetIDs, const FString& ExportDirectory) const
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    // 創建導出目錄
    if (!PlatformFile.CreateDirectoryTree(*ExportDirectory))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create export directory: %s"), *ExportDirectory);
        return false;
    }
    
    int32 SuccessCount = 0;
    
    for (const FString& AssetID : AssetIDs)
    {
        const FAssetMetadata Asset = GetAsset(AssetID);
        
        if (!Asset.AssetID.IsEmpty())
        {
            FString ExportPath = ExportDirectory / FPaths::GetBaseFilename(Asset.FilePath);
            
            if (ExportAsset(AssetID, ExportPath))
            {
                SuccessCount++;
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Exported %d/%d assets to %s"), SuccessCount, AssetIDs.Num(), *ExportDirectory);
    
    return SuccessCount == AssetIDs.Num();
}

bool UMingAIAssetManager::ImportAsset(const FString& FilePath, EAssetCategory Category)
{
    if (!bIsInitialized)
    {
        return false;
    }
    
    // 驗證文件
    if (!ValidateAssetFile(FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid asset file: %s"), *FilePath);
        return false;
    }
    
    // 生成資產ID
    FString AssetID = FString::Printf(TEXT("Imported_%s_%d"), 
        *FPaths::GetBaseFilename(FilePath), 
        FMath::RandRange(1000, 9999));
    
    // 創建資產元數據
    FAssetMetadata Metadata;
    Metadata.AssetID = AssetID;
    Metadata.AssetName = FPaths::GetBaseFilename(FilePath);
    Metadata.Category = Category;
    Metadata.FilePath = FilePath;
    Metadata.CreatedTime = FDateTime::Now();
    Metadata.FileSize = GetFileSize(FilePath);
    
    // 生成縮略圖
    Metadata.ThumbnailPath = GenerateThumbnail(FilePath);
    
    // 添加到列表
    AssetList.Add(Metadata);
    SaveAssetDatabase();
    
    UE_LOG(LogTemp, Log, TEXT("Asset imported: %s"), *AssetID);
    
    return true;
}

int32 UMingAIAssetManager::CleanupInvalidAssets()
{
    if (!bIsInitialized)
    {
        return 0;
    }
    
    int32 RemovedCount = 0;
    
    for (int32 i = AssetList.Num() - 1; i >= 0; --i)
    {
        const FAssetMetadata& Asset = AssetList[i];
        
        if (!ValidateAssetFile(Asset.FilePath))
        {
            UE_LOG(LogTemp, Warning, TEXT("Removing invalid asset: %s"), *Asset.AssetID);
            
            AssetList.RemoveAt(i);
            RemovedCount++;
        }
    }
    
    if (RemovedCount > 0)
    {
        SaveAssetDatabase();
        UE_LOG(LogTemp, Log, TEXT("Cleaned up %d invalid assets"), RemovedCount);
    }
    
    return RemovedCount;
}

float UMingAIAssetManager::GetStorageUsageMB() const
{
    int64 TotalBytes = 0;
    
    for (const FAssetMetadata& Asset : AssetList)
    {
        TotalBytes += Asset.FileSize;
    }
    
    return TotalBytes / (1024.0f * 1024.0f); // 轉換為MB
}

TArray<FAssetMetadata> UMingAIAssetManager::GetRecommendedAssets(EAssetCategory Category, int32 Count) const
{
    TArray<FAssetMetadata> RecommendedAssets;
    
    // 收集指定分類的資產
    TArray<FAssetMetadata> CategoryAssets;
    
    for (const FAssetMetadata& Asset : AssetList)
    {
        if (Asset.Category == Category)
        {
            CategoryAssets.Add(Asset);
        }
    }
    
    // 按評分和使用次數排序
    CategoryAssets.Sort([](const FAssetMetadata& A, const FAssetMetadata& B)
    {
        float ScoreA = A.Rating * 0.7f + (A.UsageCount / 100.0f) * 0.3f;
        float ScoreB = B.Rating * 0.7f + (B.UsageCount / 100.0f) * 0.3f;
        
        return ScoreA > ScoreB;
    });
    
    // 返回前Count個
    for (int32 i = 0; i < FMath::Min(Count, CategoryAssets.Num()); ++i)
    {
        RecommendedAssets.Add(CategoryAssets[i]);
    }
    
    return RecommendedAssets;
}

UMingAIAssetManager* UMingAIAssetManager::Get()
{
    return Instance;
}

bool UMingAIAssetManager::LoadAssetDatabase()
{
    // 簡化處理：實際應該從JSON文件加載
    UE_LOG(LogTemp, Log, TEXT("Loading asset database from: %s"), *AssetDatabasePath);
    
    // 模擬加載一些資產
    FAssetMetadata SampleAsset;
    SampleAsset.AssetID = TEXT("Sample_001");
    SampleAsset.AssetName = TEXT("Sample Character");
    SampleAsset.AssetType = EAIAssetType::CharacterPortrait;
    SampleAsset.Category = EAssetCategory::Characters;
    SampleAsset.EraStyle = ERepublicEraStyle::NanjingDecade;
    SampleAsset.CreatedTime = FDateTime::Now();
    SampleAsset.Rating = 4.5f;
    SampleAsset.UsageCount = 10;
    
    AssetList.Add(SampleAsset);
    
    return true;
}

bool UMingAIAssetManager::SaveAssetDatabase() const
{
    // 簡化處理：實際應該保存到JSON文件
    UE_LOG(LogTemp, Log, TEXT("Saving asset database to: %s"), *AssetDatabasePath);
    UE_LOG(LogTemp, Log, TEXT("Saved %d assets"), AssetList.Num());
    
    return true;
}

FAssetMetadata UMingAIAssetManager::CreateAssetMetadata(const FAIAssetGenerationResult& GenerationResult)
{
    FAssetMetadata Metadata;
    
    Metadata.AssetID = GenerationResult.AssetID;
    Metadata.AssetName = GenerationResult.AssetID;
    Metadata.AssetType = GenerationResult.AssetType;
    Metadata.Category = DetermineAssetCategory(GenerationResult.AssetType);
    Metadata.FilePath = GenerationResult.AssetPath;
    Metadata.ThumbnailPath = GenerationResult.ThumbnailPath;
    Metadata.CreatedTime = FDateTime::Now();
    Metadata.FileSize = GetFileSize(GenerationResult.AssetPath);
    Metadata.Tags = GenerationResult.Tags;
    Metadata.CustomProperties = GenerationResult.Metadata;
    
    return Metadata;
}

EAssetCategory UMingAIAssetManager::DetermineAssetCategory(EAIAssetType AssetType)
{
    switch (AssetType)
    {
    case EAIAssetType::CharacterPortrait:
        return EAssetCategory::Characters;
        
    case EAIAssetType::Building:
        return EAssetCategory::Buildings;
        
    case EAIAssetType::Weapon:
        return EAssetCategory::Weapons;
        
    case EAIAssetType::Uniform:
        return EAssetCategory::Uniforms;
        
    case EAIAssetType::Prop:
        return EAssetCategory::Props;
        
    case EAIAssetType::Landscape:
        return EAssetCategory::Landscapes;
        
    case EAIAssetType::BackgroundMusic:
        return EAssetCategory::Music;
        
    case EAIAssetType::SoundEffect:
        return EAssetCategory::Sounds;
        
    case EAIAssetType::VoiceLine:
        return EAssetCategory::Voices;
        
    default:
        return EAssetCategory::Props;
    }
}

bool UMingAIAssetManager::ValidateAssetFile(const FString& FilePath) const
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    return PlatformFile.FileExists(*FilePath);
}

FString UMingAIAssetManager::GenerateThumbnail(const FString& AssetPath) const
{
    // 簡化處理：實際應該生成縮略圖
    FString ThumbnailPath = AssetPath + TEXT("_thumb.jpg");
    
    UE_LOG(LogTemp, Verbose, TEXT("Thumbnail would be generated: %s"), *ThumbnailPath);
    
    return ThumbnailPath;
}

int64 UMingAIAssetManager::GetFileSize(const FString& FilePath) const
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    return PlatformFile.FileSize(*FilePath);
}

bool UMingAIAssetManager::MatchesFilter(const FAssetMetadata& Asset, const FAssetFilter& Filter) const
{
    // 搜索關鍵詞
    if (!Filter.SearchKeyword.IsEmpty())
    {
        if (!Asset.AssetName.Contains(Filter.SearchKeyword) && 
            !Asset.AssetID.Contains(Filter.SearchKeyword))
        {
            return false;
        }
    }
    
    // 資產類型
    if (Filter.AssetTypes.Num() > 0 && !Filter.AssetTypes.Contains(Asset.AssetType))
    {
        return false;
    }
    
    // 資產分類
    if (Filter.Categories.Num() > 0 && !Filter.Categories.Contains(Asset.Category))
    {
        return false;
    }
    
    // 民國時期風格
    if (Filter.EraStyles.Num() > 0 && !Filter.EraStyles.Contains(Asset.EraStyle))
    {
        return false;
    }
    
    // 最小評分
    if (Asset.Rating < Filter.MinRating)
    {
        return false;
    }
    
    // 只顯示收藏
    if (Filter.bFavoritesOnly && !Asset.bIsFavorite)
    {
        return false;
    }
    
    // 只顯示已使用
    if (Filter.bUsedOnly && Asset.UsageCount == 0)
    {
        return false;
    }
    
    return true;
}

void UMingAIAssetManager::SortAssets(TArray<FAssetMetadata>& Assets, const FString& SortBy) const
{
    if (SortBy == TEXT("CreatedTime"))
    {
        Assets.Sort([](const FAssetMetadata& A, const FAssetMetadata& B)
        {
            return A.CreatedTime > B.CreatedTime;
        });
    }
    else if (SortBy == TEXT("Rating"))
    {
        Assets.Sort([](const FAssetMetadata& A, const FAssetMetadata& B)
        {
            return A.Rating > B.Rating;
        });
    }
    else if (SortBy == TEXT("UsageCount"))
    {
        Assets.Sort([](const FAssetMetadata& A, const FAssetMetadata& B)
        {
            return A.UsageCount > B.UsageCount;
        });
    }
}

bool UMingAIAssetManager::CreateAssetDirectories()
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    bool bSuccess = true;
    
    // 創建主目錄
    bSuccess &= PlatformFile.CreateDirectoryTree(*AssetStorageDirectory);
    
    // 創建子目錄
    bSuccess &= PlatformFile.CreateDirectoryTree(*(AssetStorageDirectory / TEXT("Images")));
    bSuccess &= PlatformFile.CreateDirectoryTree(*(AssetStorageDirectory / TEXT("Music")));
    bSuccess &= PlatformFile.CreateDirectoryTree(*(AssetStorageDirectory / TEXT("Sounds")));
    bSuccess &= PlatformFile.CreateDirectoryTree(*(AssetStorageDirectory / TEXT("Thumbnails")));
    
    return bSuccess;
}
