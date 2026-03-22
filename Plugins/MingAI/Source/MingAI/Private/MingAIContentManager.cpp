#include "MingAIContentManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

UMingAIContentManager::UMingAIContentManager()
    : bInitialized(false)
{
}

void UMingAIContentManager::InitializeManager()
{
    if (bInitialized)
    {
        return;
    }

    ContentDirectory = FPaths::ProjectSavedDir() / TEXT("AIContent");
    CacheDirectory = FPaths::ProjectSavedDir() / TEXT("AIContent/Cache");

    // Ensure directories exist
    IFileManager::Get().MakeDirectory(*ContentDirectory, true);
    IFileManager::Get().MakeDirectory(*CacheDirectory, true);

    LoadRegistry();

    bInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("AI Content Manager initialized. Content directory: %s"), *ContentDirectory);
}

void UMingAIContentManager::ShutdownManager()
{
    SaveRegistry();
    
    ContentRegistry.Empty();
    Projects.Empty();
    ProjectContents.Empty();
    
    bInitialized = false;
    UE_LOG(LogTemp, Log, TEXT("AI Content Manager shutdown"));
}

FGuid UMingAIContentManager::RegisterContent(const FMingContentItemMetadata& Metadata)
{
    if (!ValidateMetadata(Metadata))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to register content: invalid metadata"));
        return FGuid();
    }

    FGuid ItemID = Metadata.ItemID.IsValid() ? Metadata.ItemID : FGuid::NewGuid();
    
    FMingContentItemMetadata NewMetadata = Metadata;
    NewMetadata.ItemID = ItemID;
    NewMetadata.CreationDate = FDateTime::Now();
    NewMetadata.LastModified = FDateTime::Now();
    
    if (NewMetadata.ThumbnailPath.IsEmpty() && !NewMetadata.FilePath.IsEmpty())
    {
        FString ThumbnailPath = CacheDirectory / FString::Printf(TEXT("%s_thumb.jpg"), *ItemID.ToString());
        if (CreateThumbnail(NewMetadata.FilePath, ThumbnailPath))
        {
            NewMetadata.ThumbnailPath = ThumbnailPath;
        }
    }

    ContentRegistry.Add(ItemID, NewMetadata);
    SaveRegistry();

    UE_LOG(LogTemp, Log, TEXT("Registered content item %s: %s"), *ItemID.ToString(), *NewMetadata.ItemName);
    return ItemID;
}

bool UMingAIContentManager::UpdateContentMetadata(const FGuid& ItemID, const FMingContentItemMetadata& Metadata)
{
    if (!ContentRegistry.Contains(ItemID))
    {
        return false;
    }

    FMingContentItemMetadata UpdatedMetadata = Metadata;
    UpdatedMetadata.ItemID = ItemID;
    UpdatedMetadata.LastModified = FDateTime::Now();
    UpdatedMetadata.CreationDate = ContentRegistry[ItemID].CreationDate; // Preserve creation date

    ContentRegistry[ItemID] = UpdatedMetadata;
    SaveRegistry();

    UE_LOG(LogTemp, Log, TEXT("Updated metadata for content item %s"), *ItemID.ToString());
    return true;
}

bool UMingAIContentManager::DeleteContent(const FGuid& ItemID)
{
    if (!ContentRegistry.Contains(ItemID))
    {
        return false;
    }

    // Delete physical files
    FMingContentItemMetadata& Metadata = ContentRegistry[ItemID];
    if (!Metadata.FilePath.IsEmpty() && FPaths::FileExists(Metadata.FilePath))
    {
        IFileManager::Get().Delete(*Metadata.FilePath);
    }
    if (!Metadata.ThumbnailPath.IsEmpty() && FPaths::FileExists(Metadata.ThumbnailPath))
    {
        IFileManager::Get().Delete(*Metadata.ThumbnailPath);
    }

    // Remove from projects
    for (auto& Pair : ProjectContents)
    {
        Pair.Value.Remove(ItemID);
    }

    ContentRegistry.Remove(ItemID);
    SaveRegistry();

    UE_LOG(LogTemp, Log, TEXT("Deleted content item %s"), *ItemID.ToString());
    return true;
}

bool UMingAIContentManager::ArchiveContent(const FGuid& ItemID)
{
    if (!ContentRegistry.Contains(ItemID))
    {
        return false;
    }

    ContentRegistry[ItemID].Status = EMingContentItemStatus::Archived;
    ContentRegistry[ItemID].LastModified = FDateTime::Now();
    SaveRegistry();

    UE_LOG(LogTemp, Log, TEXT("Archived content item %s"), *ItemID.ToString());
    return true;
}

FMingContentItemMetadata UMingAIContentManager::GetContentMetadata(const FGuid& ItemID) const
{
    if (ContentRegistry.Contains(ItemID))
    {
        return ContentRegistry[ItemID];
    }
    return FMingContentItemMetadata();
}

TArray<FGuid> UMingAIContentManager::GetAllContentIDs() const
{
    TArray<FGuid> Result;
    for (const auto& Pair : ContentRegistry)
    {
        if (Pair.Value.Status != EMingContentItemStatus::Deleted)
        {
            Result.Add(Pair.Key);
        }
    }
    return Result;
}

TArray<FGuid> UMingAIContentManager::GetContentByType(EMingAIContentType ContentType) const
{
    TArray<FGuid> Result;
    for (const auto& Pair : ContentRegistry)
    {
        if (Pair.Value.ContentType == ContentType && Pair.Value.Status == EMingContentItemStatus::Active)
        {
            Result.Add(Pair.Key);
        }
    }
    return Result;
}

TArray<FGuid> UMingAIContentManager::GetContentByCategory(const FString& Category) const
{
    TArray<FGuid> Result;
    for (const auto& Pair : ContentRegistry)
    {
        if (Pair.Value.Category.Equals(Category, ESearchCase::IgnoreCase) && 
            Pair.Value.Status == EMingContentItemStatus::Active)
        {
            Result.Add(Pair.Key);
        }
    }
    return Result;
}

TArray<FGuid> UMingAIContentManager::GetContentByTag(const FString& Tag) const
{
    TArray<FGuid> Result;
    FString SearchTag = Tag.ToLower();
    
    for (const auto& Pair : ContentRegistry)
    {
        FString TagsLower = Pair.Value.Tags.ToLower();
        if (TagsLower.Contains(SearchTag) && Pair.Value.Status == EMingContentItemStatus::Active)
        {
            Result.Add(Pair.Key);
        }
    }
    return Result;
}

TArray<FGuid> UMingAIContentManager::SearchContent(const FString& SearchQuery) const
{
    TArray<FGuid> Result;
    FString QueryLower = SearchQuery.ToLower();
    
    for (const auto& Pair : ContentRegistry)
    {
        const FMingContentItemMetadata& Metadata = Pair.Value;
        
        if (Metadata.ItemName.ToLower().Contains(QueryLower) ||
            Metadata.Description.ToLower().Contains(QueryLower) ||
            Metadata.Tags.ToLower().Contains(QueryLower) ||
            Metadata.Category.ToLower().Contains(QueryLower))
        {
            if (Metadata.Status == EMingContentItemStatus::Active)
            {
                Result.Add(Pair.Key);
            }
        }
    }
    return Result;
}

bool UMingAIContentManager::MoveContentToCategory(const FGuid& ItemID, const FString& NewCategory)
{
    if (!ContentRegistry.Contains(ItemID))
    {
        return false;
    }

    ContentRegistry[ItemID].Category = NewCategory;
    ContentRegistry[ItemID].LastModified = FDateTime::Now();
    SaveRegistry();

    return true;
}

bool UMingAIContentManager::AddTagToContent(const FGuid& ItemID, const FString& Tag)
{
    if (!ContentRegistry.Contains(ItemID))
    {
        return false;
    }

    FString& Tags = ContentRegistry[ItemID].Tags;
    if (!Tags.Contains(Tag))
    {
        if (!Tags.IsEmpty())
        {
            Tags += TEXT(",");
        }
        Tags += Tag;
        ContentRegistry[ItemID].LastModified = FDateTime::Now();
        SaveRegistry();
    }
    return true;
}

bool UMingAIContentManager::RemoveTagFromContent(const FGuid& ItemID, const FString& Tag)
{
    if (!ContentRegistry.Contains(ItemID))
    {
        return false;
    }

    FString& Tags = ContentRegistry[ItemID].Tags;
    Tags = Tags.Replace(*(Tag + TEXT(",")), TEXT(""));
    Tags = Tags.Replace(*Tag, TEXT(""));
    Tags = Tags.Replace(TEXT(",,"), TEXT(","));
    Tags = Tags.TrimStartAndEnd();
    
    ContentRegistry[ItemID].LastModified = FDateTime::Now();
    SaveRegistry();

    return true;
}

bool UMingAIContentManager::ImportContent(const FString& FilePath, const FMingContentItemMetadata& Metadata)
{
    if (!FPaths::FileExists(FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot import content: file not found %s"), *FilePath);
        return false;
    }

    // Copy file to content directory
    FString FileName = FPaths::GetCleanFilename(FilePath);
    FString DestinationPath = ContentDirectory / FileName;
    
    // If file exists, generate unique name
    int32 Counter = 1;
    FString BaseName = FPaths::GetBaseFilename(FileName);
    FString Extension = FPaths::GetExtension(FileName);
    
    while (FPaths::FileExists(DestinationPath))
    {
        DestinationPath = ContentDirectory / FString::Printf(TEXT("%s_%d.%s"), *BaseName, Counter++, *Extension);
    }

    if (!IFileManager::Get().Copy(*DestinationPath, *FilePath) == COPY_OK)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to copy file to content directory"));
        return false;
    }

    // Register the content
    FMingContentItemMetadata NewMetadata = Metadata;
    NewMetadata.FilePath = DestinationPath;
    NewMetadata.FileSize = IFileManager::Get().FileSize(*DestinationPath);
    
    FGuid ItemID = RegisterContent(NewMetadata);
    return ItemID.IsValid();
}

bool UMingAIContentManager::ExportContent(const FGuid& ItemID, const FString& DestinationPath)
{
    if (!ContentRegistry.Contains(ItemID))
    {
        return false;
    }

    const FString& SourcePath = ContentRegistry[ItemID].FilePath;
    if (!FPaths::FileExists(SourcePath))
    {
        return false;
    }

    return IFileManager::Get().Copy(*DestinationPath, *SourcePath) == COPY_OK;
}

bool UMingAIContentManager::DuplicateContent(const FGuid& ItemID, const FString& NewName)
{
    if (!ContentRegistry.Contains(ItemID))
    {
        return false;
    }

    const FMingContentItemMetadata& SourceMetadata = ContentRegistry[ItemID];
    
    // Copy file
    FString SourcePath = SourceMetadata.FilePath;
    FString Extension = FPaths::GetExtension(SourcePath);
    FString DestinationPath = ContentDirectory / FString::Printf(TEXT("%s.%s"), *NewName, *Extension);
    
    int32 Counter = 1;
    while (FPaths::FileExists(DestinationPath))
    {
        DestinationPath = ContentDirectory / FString::Printf(TEXT("%s_%d.%s"), *NewName, Counter++, *Extension);
    }

    if (!IFileManager::Get().Copy(*DestinationPath, *SourcePath) == COPY_OK)
    {
        return false;
    }

    // Register new content
    FMingContentItemMetadata NewMetadata = SourceMetadata;
    NewMetadata.ItemName = NewName;
    NewMetadata.FilePath = DestinationPath;
    NewMetadata.UsageCount = 0;
    NewMetadata.Rating = 0.0f;

    RegisterContent(NewMetadata);
    return true;
}

bool UMingAIContentManager::PreviewContent(const FGuid& ItemID)
{
    if (!ContentRegistry.Contains(ItemID))
    {
        return false;
    }

    const FString& FilePath = ContentRegistry[ItemID].FilePath;
    if (!FPaths::FileExists(FilePath))
    {
        return false;
    }

    // Update usage count
    UpdateContentUsage(ItemID);

    // In a real implementation, this would open a preview window
    UE_LOG(LogTemp, Log, TEXT("Previewing content: %s"), *FilePath);
    return true;
}

FGuid UMingAIContentManager::CreateProject(const FMingContentProject& Project)
{
    FGuid ProjectID = FGuid::NewGuid();
    
    FMingContentProject NewProject = Project;
    NewProject.ProjectID = ProjectID;
    
    Projects.Add(ProjectID, NewProject);
    ProjectContents.Add(ProjectID, TArray<FGuid>());

    SaveRegistry();

    UE_LOG(LogTemp, Log, TEXT("Created content project %s: %s"), *ProjectID.ToString(), *NewProject.ProjectName);
    return ProjectID;
}

bool UMingAIContentManager::UpdateProject(const FGuid& ProjectID, const FMingContentProject& Project)
{
    if (!Projects.Contains(ProjectID))
    {
        return false;
    }

    FMingContentProject UpdatedProject = Project;
    UpdatedProject.ProjectID = ProjectID;
    
    Projects[ProjectID] = UpdatedProject;
    SaveRegistry();

    UE_LOG(LogTemp, Log, TEXT("Updated project %s"), *ProjectID.ToString());
    return true;
}

bool UMingAIContentManager::DeleteProject(const FGuid& ProjectID)
{
    if (!Projects.Contains(ProjectID))
    {
        return false;
    }

    Projects.Remove(ProjectID);
    ProjectContents.Remove(ProjectID);
    SaveRegistry();

    UE_LOG(LogTemp, Log, TEXT("Deleted project %s"), *ProjectID.ToString());
    return true;
}

FMingContentProject UMingAIContentManager::GetProject(const FGuid& ProjectID) const
{
    if (Projects.Contains(ProjectID))
    {
        return Projects[ProjectID];
    }
    return FMingContentProject();
}

TArray<FGuid> UMingAIContentManager::GetAllProjects() const
{
    TArray<FGuid> Result;
    for (const auto& Pair : Projects)
    {
        Result.Add(Pair.Key);
    }
    return Result;
}

bool UMingAIContentManager::AddContentToProject(const FGuid& ProjectID, const FGuid& ContentID)
{
    if (!Projects.Contains(ProjectID) || !ContentRegistry.Contains(ContentID))
    {
        return false;
    }

    if (!ProjectContents[ProjectID].Contains(ContentID))
    {
        ProjectContents[ProjectID].Add(ContentID);
        SaveRegistry();
    }
    return true;
}

bool UMingAIContentManager::RemoveContentFromProject(const FGuid& ProjectID, const FGuid& ContentID)
{
    if (!Projects.Contains(ProjectID))
    {
        return false;
    }

    ProjectContents[ProjectID].Remove(ContentID);
    SaveRegistry();
    return true;
}

TArray<FGuid> UMingAIContentManager::GetProjectContents(const FGuid& ProjectID) const
{
    if (ProjectContents.Contains(ProjectID))
    {
        return ProjectContents[ProjectID];
    }
    return TArray<FGuid>();
}

int32 UMingAIContentManager::GetTotalContentCount() const
{
    return ContentRegistry.Num();
}

int32 UMingAIContentManager::GetContentCountByType(EMingAIContentType ContentType) const
{
    int32 Count = 0;
    for (const auto& Pair : ContentRegistry)
    {
        if (Pair.Value.ContentType == ContentType)
        {
            Count++;
        }
    }
    return Count;
}

int64 UMingAIContentManager::GetTotalStorageUsed() const
{
    int64 TotalSize = 0;
    for (const auto& Pair : ContentRegistry)
    {
        TotalSize += Pair.Value.FileSize;
    }
    return TotalSize;
}

TArray<FString> UMingAIContentManager::GetAllCategories() const
{
    TSet<FString> Categories;
    for (const auto& Pair : ContentRegistry)
    {
        if (!Pair.Value.Category.IsEmpty())
        {
            Categories.Add(Pair.Value.Category);
        }
    }
    return Categories.Array();
}

TArray<FString> UMingAIContentManager::GetAllTags() const
{
    TSet<FString> AllTags;
    for (const auto& Pair : ContentRegistry)
    {
        TArray<FString> ItemTags;
        Pair.Value.Tags.ParseIntoArray(ItemTags, TEXT(","), true);
        for (const FString& Tag : ItemTags)
        {
            FString TrimmedTag = Tag.TrimStartAndEnd();
            if (!TrimmedTag.IsEmpty())
            {
                AllTags.Add(TrimmedTag);
            }
        }
    }
    return AllTags.Array();
}

void UMingAIContentManager::ClearCache()
{
    IFileManager::Get().DeleteDirectoryRecursively(*CacheDirectory);
    IFileManager::Get().MakeDirectory(*CacheDirectory, true);
    UE_LOG(LogTemp, Log, TEXT("Cleared AI content cache"));
}

void UMingAIContentManager::CleanUnusedContent(int32 DaysUnused)
{
    FDateTime Now = FDateTime::Now();
    FTimespan AgeLimit(0, DaysUnused, 0, 0);

    TArray<FGuid> ItemsToRemove;
    for (const auto& Pair : ContentRegistry)
    {
        const FMingContentItemMetadata& Metadata = Pair.Value;
        
        // Check if item is archived or hasn't been used for specified days
        if (Metadata.Status == EMingContentItemStatus::Archived ||
            (Metadata.UsageCount == 0 && Now - Metadata.CreationDate > AgeLimit))
        {
            ItemsToRemove.Add(Pair.Key);
        }
    }

    for (const FGuid& ItemID : ItemsToRemove)
    {
        DeleteContent(ItemID);
    }

    UE_LOG(LogTemp, Log, TEXT("Cleaned %d unused content items"), ItemsToRemove.Num());
}

int64 UMingAIContentManager::GetCacheSize() const
{
    int64 TotalSize = 0;
    
    // Calculate thumbnail cache size
    TArray<FString> CacheFiles;
    IFileManager::Get().FindFiles(CacheFiles, *(CacheDirectory / TEXT("*")), true, false);
    
    for (const FString& File : CacheFiles)
    {
        FString FullPath = CacheDirectory / File;
        TotalSize += IFileManager::Get().FileSize(*FullPath);
    }
    
    return TotalSize;
}

bool UMingAIContentManager::ValidateMetadata(const FMingContentItemMetadata& Metadata) const
{
    if (Metadata.ItemName.IsEmpty())
    {
        return false;
    }
    if (Metadata.ContentType == EMingAIContentType::None)
    {
        return false;
    }
    return true;
}

FString UMingAIContentManager::GenerateUniqueFileName(EMingAIContentType ContentType) const
{
    FString BaseName = FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S"));
    FGuid Guid = FGuid::NewGuid();
    FString Extension = GetFileExtension(ContentType);
    
    return FString::Printf(TEXT("%s_%s.%s"), *BaseName, *Guid.ToString(EGuidFormats::Digits), *Extension);
}

bool UMingAIContentManager::CreateThumbnail(const FString& SourcePath, const FString& ThumbnailPath)
{
    // In a real implementation, this would generate a thumbnail image
    // For now, return true as placeholder
    return true;
}

void UMingAIContentManager::UpdateContentUsage(const FGuid& ItemID)
{
    if (ContentRegistry.Contains(ItemID))
    {
        ContentRegistry[ItemID].UsageCount++;
        ContentRegistry[ItemID].LastModified = FDateTime::Now();
        SaveRegistry();
    }
}

void UMingAIContentManager::SaveRegistry()
{
    // In a real implementation, this would serialize the registry to disk
    // For now, just log
    UE_LOG(LogTemp, Log, TEXT("Saving AI content registry: %d items, %d projects"), 
        ContentRegistry.Num(), Projects.Num());
}

void UMingAIContentManager::LoadRegistry()
{
    // In a real implementation, this would deserialize the registry from disk
    // For now, just log
    UE_LOG(LogTemp, Log, TEXT("Loading AI content registry"));
}

FString UMingAIContentManager::GetContentTypeString(EMingAIContentType ContentType) const
{
    switch (ContentType)
    {
    case EMingAIContentType::Image: return TEXT("Image");
    case EMingAIContentType::Video: return TEXT("Video");
    case EMingAIContentType::Music: return TEXT("Music");
    case EMingAIContentType::SoundEffect: return TEXT("SoundEffect");
    case EMingAIContentType::Texture: return TEXT("Texture");
    case EMingAIContentType::Mesh: return TEXT("Mesh");
    case EMingAIContentType::Material: return TEXT("Material");
    case EMingAIContentType::Animation: return TEXT("Animation");
    case EMingAIContentType::Level: return TEXT("Level");
    case EMingAIContentType::UI: return TEXT("UI");
    case EMingAIContentType::Text: return TEXT("Text");
    case EMingAIContentType::Voice: return TEXT("Voice");
    default: return TEXT("Unknown");
    }
}

FString UMingAIContentManager::GetFileExtension(EMingAIContentType ContentType) const
{
    switch (ContentType)
    {
    case EMingAIContentType::Image:
    case EMingAIContentType::Texture:
        return TEXT("png");
    case EMingAIContentType::Video:
        return TEXT("mp4");
    case EMingAIContentType::Music:
    case EMingAIContentType::SoundEffect:
        return TEXT("wav");
    case EMingAIContentType::Mesh:
        return TEXT("fbx");
    case EMingAIContentType::Material:
        return TEXT("uasset");
    case EMingAIContentType::Animation:
        return TEXT("uasset");
    case EMingAIContentType::Text:
        return TEXT("txt");
    case EMingAIContentType::Voice:
        return TEXT("mp3");
    default:
        return TEXT("bin");
    }
}
