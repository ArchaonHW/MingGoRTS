#include "MingRTSVirtualFileSystem.h"
#include "HAL/PlatformFilemanager.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingRTSVirtualFileSystem::UMingRTSVirtualFileSystem()
{
    bInitialized = false;
    TotalFileSystemSize = 0;
    UsedFileSystemSize = 0;
    bFileCacheEnabled = true;
    MaxCacheSize = 256; // 256MB
    CurrentCacheSize = 0;
}

bool UMingRTSVirtualFileSystem::InitializeFileSystem(int64 TotalSizeMB)
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Virtual File System already initialized"));
        return true;
    }

    TotalFileSystemSize = TotalSizeMB * 1024 * 1024; // 轉換為字節

    // 初始化根目錄
    if (!InitializeRootDirectory())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to initialize root directory"));
        return false;
    }

    // 初始化文件緩存
    if (!InitializeCache())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to initialize file cache"));
        return false;
    }

    bInitialized = true;
    ResetStatistics();

    UE_LOG(LogTemp, Log, TEXT("Virtual File System initialized with %d MB"), TotalSizeMB);
    return true;
}

void UMingRTSVirtualFileSystem::ShutdownFileSystem()
{
    if (!bInitialized)
    {
        return;
    }

    // 清理虛擬文件
    CleanupVirtualFiles();

    // 清理虛擬目錄
    CleanupVirtualDirectories();

    // 清理文件緩存
    CleanupCache();

    bInitialized = false;
    UE_LOG(LogTemp, Log, TEXT("Virtual File System shutdown completed"));
}

bool UMingRTSVirtualFileSystem::CreateFile(const FString& FilePath, const FString& Content, EFilePermission Permissions)
{
    if (!bInitialized || !ValidatePath(FilePath))
    {
        return false;
    }

    // 檢查文件是否已存在
    if (FileExists(FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("File already exists: %s"), *FilePath);
        return false;
    }

    // 檢查磁盤空間
    if (UsedFileSystemSize + Content.Len() > TotalFileSystemSize)
    {
        UE_LOG(LogTemp, Error, TEXT("Insufficient disk space for file: %s"), *FilePath);
        OnFileSystemError.Broadcast(FString::Printf(TEXT("Insufficient disk space for %s"), *FilePath));
        return false;
    }

    if (CreateVirtualFile(FilePath, Content, Permissions))
    {
        // 更新統計
        UpdateStatistics(EFileSystemOperation::Create, Content.Len());

        // 添加到緩存
        AddToCache(FilePath, Content);

        // 廣播事件
        FVirtualFileInfo FileInfo = GetFileInfo(FilePath);
        OnFileCreated.Broadcast(FilePath, FileInfo);

        LogFileSystemOperation(EFileSystemOperation::Create, FilePath, true);
        UE_LOG(LogTemp, Log, TEXT("File created: %s"), *FilePath);
        return true;
    }

    LogFileSystemOperation(EFileSystemOperation::Create, FilePath, false);
    return false;
}

bool UMingRTSVirtualFileSystem::CreateDirectory(const FString& DirectoryPath, EFilePermission Permissions)
{
    if (!bInitialized || !ValidatePath(DirectoryPath))
    {
        return false;
    }

    // 檢查目錄是否已存在
    if (DirectoryExists(DirectoryPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Directory already exists: %s"), *DirectoryPath);
        return false;
    }

    if (CreateVirtualDirectory(DirectoryPath, Permissions))
    {
        // 更新統計
        UpdateStatistics(EFileSystemOperation::Create);

        LogFileSystemOperation(EFileSystemOperation::Create, DirectoryPath, true);
        UE_LOG(LogTemp, Log, TEXT("Directory created: %s"), *DirectoryPath);
        return true;
    }

    LogFileSystemOperation(EFileSystemOperation::Create, DirectoryPath, false);
    return false;
}

bool UMingRTSVirtualFileSystem::DeleteFile(const FString& FilePath)
{
    if (!bInitialized || !ValidatePath(FilePath))
    {
        return false;
    }

    if (!FileExists(FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("File does not exist: %s"), *FilePath);
        return false;
    }

    FVirtualFileInfo FileInfo = GetFileInfo(FilePath);

    if (DeleteVirtualFile(FilePath))
    {
        // 更新統計
        UpdateStatistics(EFileSystemOperation::Delete, FileInfo.FileSize);

        // 從緩存中移除
        RemoveFromCache(FilePath);

        // 廣播事件
        FString FileName = GetFileName(FilePath);
        OnFileDeleted.Broadcast(FilePath, FileName);

        LogFileSystemOperation(EFileSystemOperation::Delete, FilePath, true);
        UE_LOG(LogTemp, Log, TEXT("File deleted: %s"), *FilePath);
        return true;
    }

    LogFileSystemOperation(EFileSystemOperation::Delete, FilePath, false);
    return false;
}

bool UMingRTSVirtualFileSystem::DeleteDirectory(const FString& DirectoryPath, bool bRecursive)
{
    if (!bInitialized || !ValidatePath(DirectoryPath))
    {
        return false;
    }

    if (!DirectoryExists(DirectoryPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Directory does not exist: %s"), *DirectoryPath);
        return false;
    }

    if (DeleteVirtualDirectory(DirectoryPath, bRecursive))
    {
        // 更新統計
        UpdateStatistics(EFileSystemOperation::Delete);

        LogFileSystemOperation(EFileSystemOperation::Delete, DirectoryPath, true);
        UE_LOG(LogTemp, Log, TEXT("Directory deleted: %s"), *DirectoryPath);
        return true;
    }

    LogFileSystemOperation(EFileSystemOperation::Delete, DirectoryPath, false);
    return false;
}

bool UMingRTSVirtualFileSystem::MoveFile(const FString& SourcePath, const FString& DestinationPath)
{
    if (!bInitialized || !ValidatePath(SourcePath) || !ValidatePath(DestinationPath))
    {
        return false;
    }

    if (!FileExists(SourcePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Source file does not exist: %s"), *SourcePath);
        return false;
    }

    if (FileExists(DestinationPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Destination file already exists: %s"), *DestinationPath);
        return false;
    }

    // 讀取源文件
    FString Content = ReadFile(SourcePath);
    if (Content.IsEmpty())
    {
        return false;
    }

    // 創建目標文件
    if (CreateFile(DestinationPath, Content))
    {
        // 刪除源文件
        if (DeleteFile(SourcePath))
        {
            LogFileSystemOperation(EFileSystemOperation::Move, SourcePath, true);
            UE_LOG(LogTemp, Log, TEXT("File moved from %s to %s"), *SourcePath, *DestinationPath);
            return true;
        }
    }

    LogFileSystemOperation(EFileSystemOperation::Move, SourcePath, false);
    return false;
}

bool UMingRTSVirtualFileSystem::CopyFile(const FString& SourcePath, const FString& DestinationPath)
{
    if (!bInitialized || !ValidatePath(SourcePath) || !ValidatePath(DestinationPath))
    {
        return false;
    }

    if (!FileExists(SourcePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Source file does not exist: %s"), *SourcePath);
        return false;
    }

    if (FileExists(DestinationPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Destination file already exists: %s"), *DestinationPath);
        return false;
    }

    // 讀取源文件
    FString Content = ReadFile(SourcePath);
    if (Content.IsEmpty())
    {
        return false;
    }

    // 創建目標文件
    if (CreateFile(DestinationPath, Content))
    {
        LogFileSystemOperation(EFileSystemOperation::Copy, SourcePath, true);
        UE_LOG(LogTemp, Log, TEXT("File copied from %s to %s"), *SourcePath, *DestinationPath);
        return true;
    }

    LogFileSystemOperation(EFileSystemOperation::Copy, SourcePath, false);
    return false;
}

FString UMingRTSVirtualFileSystem::ReadFile(const FString& FilePath, bool bBinary)
{
    if (!bInitialized || !ValidatePath(FilePath))
    {
        return TEXT("");
    }

    // 嘗試從緩存讀取
    FString CachedContent;
    if (GetFromCache(FilePath, CachedContent))
    {
        return CachedContent;
    }

    // 從虛擬文件系統讀取
    FVirtualFileInfo* FileInfo = FindVirtualFile(FilePath);
    if (FileInfo)
    {
        // 在實際實現中，這裡應該讀取文件內容
        // 簡化實現：返回模擬內容
        FString Content = FString::Printf(TEXT("Content of file: %s"), *FilePath);
        
        // 添加到緩存
        AddToCache(FilePath, Content);
        
        LogFileSystemOperation(EFileSystemOperation::Read, FilePath, true);
        return Content;
    }

    LogFileSystemOperation(EFileSystemOperation::Read, FilePath, false);
    UE_LOG(LogTemp, Warning, TEXT("File not found: %s"), *FilePath);
    return TEXT("");
}

bool UMingRTSVirtualFileSystem::WriteFile(const FString& FilePath, const FString& Content, bool bAppend)
{
    if (!bInitialized || !ValidatePath(FilePath))
    {
        return false;
    }

    FString FinalContent = Content;
    
    if (bAppend && FileExists(FilePath))
    {
        // 讀取現有內容並追加
        FString ExistingContent = ReadFile(FilePath);
        FinalContent = ExistingContent + Content;
    }

    if (CreateFile(FilePath, FinalContent))
    {
        LogFileSystemOperation(EFileSystemOperation::Write, FilePath, true);
        UE_LOG(LogTemp, Log, TEXT("File written: %s"), *FilePath);
        return true;
    }

    LogFileSystemOperation(EFileSystemOperation::Write, FilePath, false);
    return false;
}

bool UMingRTSVirtualFileSystem::AppendFile(const FString& FilePath, const FString& Content)
{
    return WriteFile(FilePath, Content, true);
}

TArray<FDirectoryEntry> UMingRTSVirtualFileSystem::ListDirectory(const FString& DirectoryPath, bool bIncludeHidden)
{
    TArray<FDirectoryEntry> Entries;

    if (!bInitialized || !ValidatePath(DirectoryPath))
    {
        return Entries;
    }

    // 獲取目錄條目
    TArray<FDirectoryEntry>* DirectoryEntries = FindVirtualDirectory(DirectoryPath);
    if (DirectoryEntries)
    {
        for (const FDirectoryEntry& Entry : *DirectoryEntries)
        {
            if (bIncludeHidden || !Entry.Name.StartsWith(TEXT(".")))
            {
                Entries.Add(Entry);
            }
        }
    }

    LogFileSystemOperation(EFileSystemOperation::List, DirectoryPath, true);
    return Entries;
}

TArray<FString> UMingRTSVirtualFileSystem::FindFiles(const FString& Pattern, const FString& SearchPath, bool bRecursive)
{
    TArray<FString> FoundFiles;

    if (!bInitialized || !ValidatePath(SearchPath))
    {
        return FoundFiles;
    }

    // 簡化的文件搜索實現
    TArray<FDirectoryEntry> Entries = ListDirectory(SearchPath, true);
    
    for (const FDirectoryEntry& Entry : Entries)
    {
        if (Entry.Type == EVirtualFileType::Regular)
        {
            // 簡單的模式匹配（實際應該使用更複雜的通配符匹配）
            if (Entry.Name.Contains(Pattern))
            {
                FoundFiles.Add(SearchPath / Entry.Name);
            }
        }
        
        // 遞歸搜索子目錄
        if (bRecursive && Entry.Type == EVirtualFileType::Directory)
        {
            FString SubPath = SearchPath / Entry.Name;
            TArray<FString> SubFiles = FindFiles(Pattern, SubPath, true);
            FoundFiles.Append(SubFiles);
        }
    }

    return FoundFiles;
}

bool UMingRTSVirtualFileSystem::FileExists(const FString& FilePath) const
{
    if (!bInitialized || !ValidatePath(FilePath))
    {
        return false;
    }

    return FindVirtualFile(FilePath) != nullptr;
}

bool UMingRTSVirtualFileSystem::DirectoryExists(const FString& DirectoryPath) const
{
    if (!bInitialized || !ValidatePath(DirectoryPath))
    {
        return false;
    }

    return FindVirtualDirectory(DirectoryPath) != nullptr;
}

FVirtualFileInfo UMingRTSVirtualFileSystem::GetFileInfo(const FString& FilePath) const
{
    if (!bInitialized || !ValidatePath(FilePath))
    {
        return FVirtualFileInfo();
    }

    if (const FVirtualFileInfo* FileInfo = FindVirtualFile(FilePath))
    {
        return *FileInfo;
    }

    return FVirtualFileInfo();
}

bool UMingRTSVirtualFileSystem::SetFilePermissions(const FString& FilePath, EFilePermission Permissions)
{
    if (!bInitialized || !ValidatePath(FilePath))
    {
        return false;
    }

    FVirtualFileInfo* FileInfo = FindVirtualFile(FilePath);
    if (FileInfo)
    {
        FileInfo->Permissions = Permissions;
        LogFileSystemOperation(EFileSystemOperation::Chmod, FilePath, true);
        return true;
    }

    LogFileSystemOperation(EFileSystemOperation::Chmod, FilePath, false);
    return false;
}

bool UMingRTSVirtualFileSystem::SetFileOwner(const FString& FilePath, const FString& OwnerID)
{
    if (!bInitialized || !ValidatePath(FilePath))
    {
        return false;
    }

    FVirtualFileInfo* FileInfo = FindVirtualFile(FilePath);
    if (FileInfo)
    {
        FileInfo->OwnerID = OwnerID;
        LogFileSystemOperation(EFileSystemOperation::Chown, FilePath, true);
        return true;
    }

    LogFileSystemOperation(EFileSystemOperation::Chown, FilePath, false);
    return false;
}

bool UMingRTSVirtualFileSystem::SetFileAttributes(const FString& FilePath, const TArray<FString>& Attributes)
{
    if (!bInitialized || !ValidatePath(FilePath))
    {
        return false;
    }

    FVirtualFileInfo* FileInfo = FindVirtualFile(FilePath);
    if (FileInfo)
    {
        FileInfo->ExtendedAttributes = Attributes;
        return true;
    }

    return false;
}

FString UMingRTSVirtualFileSystem::NormalizePath(const FString& Path) const
{
    FString NormalizedPath = Path;
    
    // 替換反斜杠為正斜杠
    NormalizedPath.ReplaceInline(TEXT("\\"), TEXT("/"));
    
    // 移除重複的斜杠
    while (NormalizedPath.Contains(TEXT("//")))
    {
        NormalizedPath.ReplaceInline(TEXT("//"), TEXT("/"));
    }
    
    // 移除末尾的斜杠（除非是根目錄）
    if (NormalizedPath.Len() > 1 && NormalizedPath.EndsWith(TEXT("/")))
    {
        NormalizedPath = NormalizedPath.LeftChop(1);
    }
    
    return NormalizedPath;
}

FString UMingRTSVirtualFileSystem::GetParentDirectory(const FString& Path) const
{
    FString NormalizedPath = NormalizePath(Path);
    
    if (NormalizedPath == TEXT("/"))
    {
        return TEXT("/");
    }
    
    int32 LastSlashIndex = NormalizedPath.Find(TEXT("/"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
    if (LastSlashIndex != INDEX_NONE)
    {
        return NormalizedPath.Left(LastSlashIndex);
    }
    
    return TEXT("/");
}

FString UMingRTSVirtualFileSystem::GetFileName(const FString& Path) const
{
    FString NormalizedPath = NormalizePath(Path);
    
    int32 LastSlashIndex = NormalizedPath.Find(TEXT("/"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
    if (LastSlashIndex != INDEX_NONE)
    {
        return NormalizedPath.RightChop(LastSlashIndex + 1);
    }
    
    return NormalizedPath;
}

FString UMingRTSVirtualFileSystem::GetFileExtension(const FString& Path) const
{
    FString FileName = GetFileName(Path);
    
    int32 DotIndex = FileName.Find(TEXT("."), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
    if (DotIndex != INDEX_NONE)
    {
        return FileName.RightChop(DotIndex);
    }
    
    return TEXT("");
}

bool UMingRTSVirtualFileSystem::IsAbsolutePath(const FString& Path) const
{
    return Path.StartsWith(TEXT("/")) || Path.Contains(TEXT(":"));
}

FFileSystemStatistics UMingRTSVirtualFileSystem::GetFileSystemStatistics() const
{
    FScopeLock Lock(&StatisticsCriticalSection);
    return Statistics;
}

int64 UMingRTSVirtualFileSystem::GetFreeSpace() const
{
    FScopeLock Lock(&StatisticsCriticalSection);
    return Statistics.FreeSize;
}

int64 UMingRTSVirtualFileSystem::GetUsedSpace() const
{
    FScopeLock Lock(&StatisticsCriticalSection);
    return Statistics.UsedSize;
}

void UMingRTSVirtualFileSystem::EnableFileCache(bool bEnabled, int32 MaxCacheSizeMB)
{
    bFileCacheEnabled = bEnabled;
    MaxCacheSize = MaxCacheSizeMB;
    
    if (!bEnabled)
    {
        ClearFileCache();
    }
    
    UE_LOG(LogTemp, Log, TEXT("File cache %s, max size: %d MB"), 
           bEnabled ? TEXT("enabled") : TEXT("disabled"), MaxCacheSizeMB);
}

void UMingRTSVirtualFileSystem::ClearFileCache()
{
    FScopeLock Lock(&CacheCriticalSection);
    FileCache.Empty();
    CurrentCacheSize = 0;
    
    UE_LOG(LogTemp, Log, TEXT("File cache cleared"));
}

bool UMingRTSVirtualFileSystem::IsFileCached(const FString& FilePath) const
{
    FScopeLock Lock(&CacheCriticalSection);
    return FileCache.Contains(FilePath);
}

bool UMingRTSVirtualFileSystem::ValidatePath(const FString& Path) const
{
    if (Path.IsEmpty())
    {
        return false;
    }
    
    // 檢查非法字符
    TArray<FString> IllegalChars = {TEXT("<"), TEXT(">"), TEXT("|"), TEXT("\""), TEXT("?"), TEXT("*")};
    for (const FString& Char : IllegalChars)
    {
        if (Path.Contains(Char))
        {
            return false;
        }
    }
    
    return true;
}

bool UMingRTSVirtualFileSystem::ValidateFileName(const FString& FileName) const
{
    if (FileName.IsEmpty())
    {
        return false;
    }
    
    // 檢查保留名稱
    TArray<FString> ReservedNames = {TEXT("CON"), TEXT("PRN"), TEXT("AUX"), TEXT("NUL")};
    for (const FString& Name : ReservedNames)
    {
        if (FileName.Equals(Name, ESearchCase::IgnoreCase))
        {
            return false;
        }
    }
    
    return ValidateFileName(FileName);
}

FString UMingRTSVirtualFileSystem::GenerateUniqueFileName(const FString& BasePath, const FString& BaseName) const
{
    FString UniqueName = BaseName;
    int32 Counter = 1;
    
    while (FileExists(BasePath / UniqueName))
    {
        UniqueName = FString::Printf(TEXT("%s_%d"), *BaseName, Counter);
        Counter++;
    }
    
    return UniqueName;
}

bool UMingRTSVirtualFileSystem::IsPathSeparator(TCHAR Char) const
{
    return Char == TEXT('/') || Char == TEXT('\\');
}

TArray<FString> UMingRTSVirtualFileSystem::SplitPath(const FString& Path) const
{
    TArray<FString> Components;
    FString CurrentPath = NormalizePath(Path);
    
    if (CurrentPath.StartsWith(TEXT("/")))
    {
        CurrentPath.RightChop(1);
    }
    
    CurrentPath.ParseIntoArray(Components, TEXT("/"));
    
    return Components;
}

FString UMingRTSVirtualFileSystem::JoinPath(const TArray<FString>& Components) const
{
    FString Result;
    
    for (int32 i = 0; i < Components.Num(); ++i)
    {
        Result += Components[i];
        if (i < Components.Num() - 1)
        {
            Result += TEXT("/");
        }
    }
    
    return NormalizePath(Result);
}

void UMingRTSVirtualFileSystem::UpdateStatistics(EFileSystemOperation Operation, int64 Size)
{
    FScopeLock Lock(&StatisticsCriticalSection);
    
    switch (Operation)
    {
        case EFileSystemOperation::Create:
            if (Size > 0)
            {
                Statistics.TotalFiles++;
                Statistics.UsedSize += Size;
                Statistics.FreeSize = TotalFileSystemSize - Statistics.UsedSize;
            }
            else
            {
                Statistics.TotalDirectories++;
            }
            break;
        
        case EFileSystemOperation::Delete:
            if (Size > 0)
            {
                Statistics.TotalFiles--;
                Statistics.UsedSize -= Size;
                Statistics.FreeSize = TotalFileSystemSize - Statistics.UsedSize;
            }
            else
            {
                Statistics.TotalDirectories--;
            }
            break;
        
        case EFileSystemOperation::Read:
            if (IsFileCached(TEXT("")))
            {
                Statistics.CacheHits++;
            }
            else
            {
                Statistics.CacheMisses++;
            }
            break;
        
        default:
            break;
    }
}

void UMingRTSVirtualFileSystem::LogFileSystemOperation(EFileSystemOperation Operation, const FString& Path, bool bSuccess)
{
    if (!bSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("File system operation failed: %d on %s"), (int32)Operation, *Path);
        OnFileSystemError.Broadcast(FString::Printf(TEXT("Operation %d failed on %s"), (int32)Operation, *Path));
    }
}

bool UMingRTSVirtualFileSystem::AddToCache(const FString& FilePath, const FString& Content)
{
    if (!bFileCacheEnabled)
    {
        return false;
    }

    FScopeLock Lock(&CacheCriticalSection);
    
    // 檢查緩存大小限制
    int32 ContentSize = Content.Len();
    if (CurrentCacheSize + ContentSize > MaxCacheSize * 1024 * 1024)
    {
        // 清理部分緩存
        CleanupCache();
    }
    
    FileCache.Add(FilePath, Content);
    CurrentCacheSize += ContentSize;
    
    return true;
}

bool UMingRTSVirtualFileSystem::GetFromCache(const FString& FilePath, FString& OutContent)
{
    if (!bFileCacheEnabled)
    {
        return false;
    }

    FScopeLock Lock(&CacheCriticalSection);
    
    if (const FString* CachedContent = FileCache.Find(FilePath))
    {
        OutContent = *CachedContent;
        return true;
    }
    
    return false;
}

void UMingRTSVirtualFileSystem::RemoveFromCache(const FString& FilePath)
{
    FScopeLock Lock(&CacheCriticalSection);
    
    if (const FString* Content = FileCache.Find(FilePath))
    {
        CurrentCacheSize -= Content->Len();
        FileCache.Remove(FilePath);
    }
}

void UMingRTSVirtualFileSystem::CleanupCache()
{
    // 簡化的LRU緩存清理
    // 實際實現中應該有更複雜的LRU算法
    
    int32 TargetSize = (MaxCacheSize * 1024 * 1024) * 0.8f; // 清理到80%
    
    while (CurrentCacheSize > TargetSize && FileCache.Num() > 0)
    {
        // 移除第一個元素（簡化實現）
        auto It = FileCache.CreateConstIterator();
        if (It)
        {
            CurrentCacheSize -= It->Value().Len();
            FileCache.Remove(It->Key());
            break;
        }
    }
}

bool UMingRTSVirtualFileSystem::CreateVirtualFile(const FString& FilePath, const FString& Content, EFilePermission Permissions)
{
    FScopeLock Lock(&FileCriticalSection);
    
    FVirtualFileInfo FileInfo;
    FileInfo.FileName = GetFileName(FilePath);
    FileInfo.FilePath = FilePath;
    FileInfo.FileType = EVirtualFileType::Regular;
    FileInfo.FileSize = Content.Len();
    FileInfo.Permissions = Permissions;
    FileInfo.CreationTime = FDateTime::Now();
    FileInfo.ModificationTime = FDateTime::Now();
    FileInfo.AccessTime = FDateTime::Now();
    
    VirtualFiles.Add(FilePath, FileInfo);
    return true;
}

bool UMingRTSVirtualFileSystem::CreateVirtualDirectory(const FString& DirectoryPath, EFilePermission Permissions)
{
    FScopeLock Lock(&DirectoryCriticalSection);
    
    TArray<FDirectoryEntry> Entries;
    VirtualDirectories.Add(DirectoryPath, Entries);
    
    return true;
}

bool UMingRTSVirtualFileSystem::DeleteVirtualFile(const FString& FilePath)
{
    FScopeLock Lock(&FileCriticalSection);
    return VirtualFiles.Remove(FilePath) > 0;
}

bool UMingRTSVirtualFileSystem::DeleteVirtualDirectory(const FString& DirectoryPath, bool bRecursive)
{
    FScopeLock Lock(&DirectoryCriticalSection);
    
    if (bRecursive)
    {
        // 遞歸刪除子目錄和文件
        TArray<FDirectoryEntry>* Entries = VirtualDirectories.Find(DirectoryPath);
        if (Entries)
        {
            for (const FDirectoryEntry& Entry : *Entries)
            {
                if (Entry.Type == EVirtualFileType::Directory)
                {
                    DeleteVirtualDirectory(DirectoryPath / Entry.Name, true);
                }
                else
                {
                    DeleteVirtualFile(DirectoryPath / Entry.Name);
                }
            }
        }
    }
    
    return VirtualDirectories.Remove(DirectoryPath) > 0;
}

FVirtualFileInfo* UMingRTSVirtualFileSystem::FindVirtualFile(const FString& FilePath) const
{
    FScopeLock Lock(&FileCriticalSection);
    return VirtualFiles.Find(FilePath);
}

TArray<FDirectoryEntry>* UMingRTSVirtualFileSystem::FindVirtualDirectory(const FString& DirectoryPath) const
{
    FScopeLock Lock(&DirectoryCriticalSection);
    return VirtualDirectories.Find(DirectoryPath);
}

bool UMingRTSVirtualFileSystem::UpdateVirtualFileInfo(const FString& FilePath, const FVirtualFileInfo& NewInfo)
{
    FScopeLock Lock(&FileCriticalSection);
    
    if (FVirtualFileInfo* FileInfo = VirtualFiles.Find(FilePath))
    {
        *FileInfo = NewInfo;
        return true;
    }
    
    return false;
}

bool UMingRTSVirtualFileSystem::InitializeRootDirectory()
{
    return CreateVirtualDirectory(TEXT("/"), EFilePermission::ReadWriteExecute);
}

bool UMingRTSVirtualFileSystem::InitializeCache()
{
    FScopeLock Lock(&CacheCriticalSection);
    FileCache.Empty();
    CurrentCacheSize = 0;
    return true;
}

void UMingRTSVirtualFileSystem::CleanupVirtualFiles()
{
    FScopeLock Lock(&FileCriticalSection);
    VirtualFiles.Empty();
}

void UMingRTSVirtualFileSystem::CleanupVirtualDirectories()
{
    FScopeLock Lock(&DirectoryCriticalSection);
    VirtualDirectories.Empty();
}

void UMingRTSVirtualFileSystem::CleanupCache()
{
    FScopeLock Lock(&CacheCriticalSection);
    FileCache.Empty();
    CurrentCacheSize = 0;
}

void UMingRTSVirtualFileSystem::ResetStatistics()
{
    FScopeLock Lock(&StatisticsCriticalSection);
    Statistics = FFileSystemStatistics();
    Statistics.TotalSize = TotalFileSystemSize;
    Statistics.FreeSize = TotalFileSystemSize;
}
