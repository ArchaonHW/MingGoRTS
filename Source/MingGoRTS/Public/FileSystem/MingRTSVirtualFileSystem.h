#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HAL/CriticalSection.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Containers/Queue.h"

// 文件類型
UENUM(BlueprintType)
enum class EVirtualFileType : uint8
{
    Regular,        // 常規文件
    Directory,      // 目錄
    Symlink,        // 符號鏈接
    Device,         // 設備文件
    Pipe,           // 管道文件
    Socket          // 套接字文件
};

// 文件權限
UENUM(BlueprintType)
enum class EFilePermission : uint8
{
    None,           // 無權限
    Read,           // 讀權限
    Write,          // 寫權限
    Execute,        // 執行權限
    ReadWrite,      // 讀寫權限
    ReadExecute,    // 讀執行權限
    ReadWriteExecute, // 讀寫執行權限
    All             // 所有權限
};

// 文件系統操作
UENUM(BlueprintType)
enum class EFileSystemOperation : uint8
{
    Read,           // 讀取
    Write,          // 寫入
    Create,         // 創建
    Delete,         // 刪除
    Move,           // 移動
    Copy,           // 複製
    List,           // 列表
    Stat,           // 狀態
    Chmod,          // 權限修改
    Chown,          // 所有者修改
    Mount,          // 掛載
    Unmount         // 卸載
};

// 虛擬文件信息
USTRUCT(BlueprintType)
struct FVirtualFileInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "File Info")
    FString FileName;

    UPROPERTY(BlueprintReadOnly, Category = "File Info")
    FString FilePath;

    UPROPERTY(BlueprintReadOnly, Category = "File Info")
    EVirtualFileType FileType;

    UPROPERTY(BlueprintReadOnly, Category = "File Info")
    int64 FileSize;

    UPROPERTY(BlueprintReadOnly, Category = "File Info")
    EFilePermission Permissions;

    UPROPERTY(BlueprintReadOnly, Category = "File Info")
    FString OwnerID;

    UPROPERTY(BlueprintReadOnly, Category = "File Info")
    FString GroupID;

    UPROPERTY(BlueprintReadOnly, Category = "File Info")
    FDateTime CreationTime;

    UPROPERTY(BlueprintReadOnly, Category = "File Info")
    FDateTime ModificationTime;

    UPROPERTY(BlueprintReadOnly, Category = "File Info")
    FDateTime AccessTime;

    UPROPERTY(BlueprintReadOnly, Category = "File Info")
    TArray<FString> ExtendedAttributes;

    FVirtualFileInfo()
    {
        FileType = EVirtualFileType::Regular;
        FileSize = 0;
        Permissions = EFilePermission::None;
        CreationTime = FDateTime::Now();
        ModificationTime = FDateTime::Now();
        AccessTime = FDateTime::Now();
    }
};

// 目錄條目
USTRUCT(BlueprintType)
struct FDirectoryEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Directory Entry")
    FString Name;

    UPROPERTY(BlueprintReadOnly, Category = "Directory Entry")
    EVirtualFileType Type;

    UPROPERTY(BlueprintReadOnly, Category = "Directory Entry")
    int64 Size;

    UPROPERTY(BlueprintReadOnly, Category = "Directory Entry")
    FDateTime ModificationTime;

    FDirectoryEntry()
    {
        Type = EVirtualFileType::Regular;
        Size = 0;
        ModificationTime = FDateTime::Now();
    }
};

// 文件系統統計
USTRUCT(BlueprintType)
struct FFileSystemStatistics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "File System Stats")
    int64 TotalFiles;

    UPROPERTY(BlueprintReadOnly, Category = "File System Stats")
    int64 TotalDirectories;

    UPROPERTY(BlueprintReadOnly, Category = "File System Stats")
    int64 TotalSize;

    UPROPERTY(BlueprintReadOnly, Category = "File System Stats")
    int64 UsedSize;

    UPROPERTY(BlueprintReadOnly, Category = "File System Stats")
    int64 FreeSize;

    UPROPERTY(BlueprintReadOnly, Category = "File System Stats")
    int32 OpenFiles;

    UPROPERTY(BlueprintReadOnly, Category = "File System Stats")
    int32 MaxOpenFiles;

    UPROPERTY(BlueprintReadOnly, Category = "File System Stats")
    int32 CacheHits;

    UPROPERTY(BlueprintReadOnly, Category = "File System Stats")
    int32 CacheMisses;

    FFileSystemStatistics()
    {
        TotalFiles = 0;
        TotalDirectories = 0;
        TotalSize = 0;
        UsedSize = 0;
        FreeSize = 0;
        OpenFiles = 0;
        MaxOpenFiles = 1024;
        CacheHits = 0;
        CacheMisses = 0;
    }
};

// 文件系統事件委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFileCreated, const FString&, FilePath, const FVirtualFileInfo&, FileInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFileDeleted, const FString&, FilePath, const FString&, FileName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFileModified, const FString&, FilePath, int64, NewSize);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFileSystemError, const FString&, ErrorMessage);

/**
 * 虛擬文件系統
 * 提供完整的文件系統功能，包括文件操作、目錄管理、權限控制等
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSVirtualFileSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSVirtualFileSystem();

    // 初始化和控制
    UFUNCTION(BlueprintCallable, Category = "File System")
    bool InitializeFileSystem(int64 TotalSizeMB = 10240);

    UFUNCTION(BlueprintCallable, Category = "File System")
    void ShutdownFileSystem();

    UFUNCTION(BlueprintPure, Category = "File System")
    bool IsInitialized() const { return bInitialized; }

    // 文件操作
    UFUNCTION(BlueprintCallable, Category = "File System")
    bool CreateFile(const FString& FilePath, const FString& Content = TEXT(""), EFilePermission Permissions = EFilePermission::ReadWrite);

    UFUNCTION(BlueprintCallable, Category = "File System")
    bool CreateDirectory(const FString& DirectoryPath, EFilePermission Permissions = EFilePermission::ReadWriteExecute);

    UFUNCTION(BlueprintCallable, Category = "File System")
    bool DeleteFile(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "File System")
    bool DeleteDirectory(const FString& DirectoryPath, bool bRecursive = false);

    UFUNCTION(BlueprintCallable, Category = "File System")
    bool MoveFile(const FString& SourcePath, const FString& DestinationPath);

    UFUNCTION(BlueprintCallable, Category = "File System")
    bool CopyFile(const FString& SourcePath, const FString& DestinationPath);

    UFUNCTION(BlueprintCallable, Category = "File System")
    FString ReadFile(const FString& FilePath, bool bBinary = false);

    UFUNCTION(BlueprintCallable, Category = "File System")
    bool WriteFile(const FString& FilePath, const FString& Content, bool bAppend = false);

    UFUNCTION(BlueprintCallable, Category = "File System")
    bool AppendFile(const FString& FilePath, const FString& Content);

    // 目錄操作
    UFUNCTION(BlueprintCallable, Category = "File System")
    TArray<FDirectoryEntry> ListDirectory(const FString& DirectoryPath, bool bIncludeHidden = false);

    UFUNCTION(BlueprintCallable, Category = "File System")
    TArray<FString> FindFiles(const FString& Pattern, const FString& SearchPath = TEXT("/"), bool bRecursive = false);

    UFUNCTION(BlueprintCallable, Category = "File System")
    bool FileExists(const FString& FilePath) const;

    UFUNCTION(BlueprintCallable, Category = "File System")
    bool DirectoryExists(const FString& DirectoryPath) const;

    // 文件信息
    UFUNCTION(BlueprintCallable, Category = "File System")
    FVirtualFileInfo GetFileInfo(const FString& FilePath) const;

    UFUNCTION(BlueprintCallable, Category = "File System")
    bool SetFilePermissions(const FString& FilePath, EFilePermission Permissions);

    UFUNCTION(BlueprintCallable, Category = "File System")
    bool SetFileOwner(const FString& FilePath, const FString& OwnerID);

    UFUNCTION(BlueprintCallable, Category = "File System")
    bool SetFileAttributes(const FString& FilePath, const TArray<FString>& Attributes);

    // 路徑操作
    UFUNCTION(BlueprintCallable, Category = "File System")
    FString NormalizePath(const FString& Path) const;

    UFUNCTION(BlueprintCallable, Category = "File System")
    FString GetParentDirectory(const FString& Path) const;

    UFUNCTION(BlueprintCallable, Category = "File System")
    FString GetFileName(const FString& Path) const;

    UFUNCTION(BlueprintCallable, Category = "File System")
    FString GetFileExtension(const FString& Path) const;

    UFUNCTION(BlueprintCallable, Category = "File System")
    bool IsAbsolutePath(const FString& Path) const;

    // 文件系統統計
    UFUNCTION(BlueprintPure, Category = "File System")
    FFileSystemStatistics GetFileSystemStatistics() const;

    UFUNCTION(BlueprintPure, Category = "File System")
    int64 GetFreeSpace() const;

    UFUNCTION(BlueprintPure, Category = "File System")
    int64 GetUsedSpace() const;

    // 緩存管理
    UFUNCTION(BlueprintCallable, Category = "File System")
    void EnableFileCache(bool bEnabled, int32 MaxCacheSizeMB = 256);

    UFUNCTION(BlueprintCallable, Category = "File System")
    void ClearFileCache();

    UFUNCTION(BlueprintCallable, Category = "File System")
    bool IsFileCached(const FString& FilePath) const;

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "File System Events")
    FOnFileCreated OnFileCreated;

    UPROPERTY(BlueprintAssignable, Category = "File System Events")
    FOnFileDeleted OnFileDeleted;

    UPROPERTY(BlueprintAssignable, Category = "File System Events")
    FOnFileModified OnFileModified;

    UPROPERTY(BlueprintAssignable, Category = "File System Events")
    FOnFileSystemError OnFileSystemError;

protected:
    // 文件系統狀態
    UPROPERTY()
    bool bInitialized;

    UPROPERTY()
    int64 TotalFileSystemSize;

    UPROPERTY()
    int64 UsedFileSystemSize;

    // 虛擬文件存儲
    UPROPERTY()
    TMap<FString, FVirtualFileInfo> VirtualFiles;

    UPROPERTY()
    TMap<FString, TArray<FDirectoryEntry>> VirtualDirectories;

    // 文件緩存
    UPROPERTY()
    TMap<FString, FString> FileCache;

    UPROPERTY()
    bool bFileCacheEnabled;

    UPROPERTY()
    int32 MaxCacheSize;

    UPROPERTY()
    int32 CurrentCacheSize;

    // 統計信息
    UPROPERTY()
    FFileSystemStatistics Statistics;

    // 同步機制
    mutable FCriticalSection FileCriticalSection;
    mutable FCriticalSection DirectoryCriticalSection;
    mutable FCriticalSection CacheCriticalSection;
    mutable FCriticalSection StatisticsCriticalSection;

    // 內部方法
    bool ValidatePath(const FString& Path) const;
    bool ValidateFileName(const FString& FileName) const;
    FString GenerateUniqueFileName(const FString& BasePath, const FString& BaseName) const;
    bool IsPathSeparator(TCHAR Char) const;
    TArray<FString> SplitPath(const FString& Path) const;
    FString JoinPath(const TArray<FString>& Components) const;
    void UpdateStatistics(EFileSystemOperation Operation, int64 Size = 0);
    void LogFileSystemOperation(EFileSystemOperation Operation, const FString& Path, bool bSuccess);
    bool AddToCache(const FString& FilePath, const FString& Content);
    bool GetFromCache(const FString& FilePath, FString& OutContent);
    void RemoveFromCache(const FString& FilePath);
    void CleanupCache();

private:
    // 初始化方法
    bool InitializeRootDirectory();
    bool InitializeCache();

    // 清理方法
    void CleanupVirtualFiles();
    void CleanupVirtualDirectories();
    void CleanupCache();
    void ResetStatistics();

    // 輔助方法
    bool CreateVirtualFile(const FString& FilePath, const FString& Content, EFilePermission Permissions);
    bool CreateVirtualDirectory(const FString& DirectoryPath, EFilePermission Permissions);
    bool DeleteVirtualFile(const FString& FilePath);
    bool DeleteVirtualDirectory(const FString& DirectoryPath, bool bRecursive);
    FVirtualFileInfo* FindVirtualFile(const FString& FilePath);
    TArray<FDirectoryEntry>* FindVirtualDirectory(const FString& DirectoryPath);
    bool UpdateVirtualFileInfo(const FString& FilePath, const FVirtualFileInfo& NewInfo);
};
