#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAIContentTypes.h"
#include "MingAIContentManager.generated.h"

UENUM(BlueprintType)
enum class EMingContentItemStatus : uint8
{
    Active UMETA(DisplayName = "Active"),
    Archived UMETA(DisplayName = "Archived"),
    Deleted UMETA(DisplayName = "Deleted"),
    Draft UMETA(DisplayName = "Draft"),
    Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FMingContentItemMetadata
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Content Metadata")
    FGuid ItemID;

    UPROPERTY(BlueprintReadWrite, Category = "Content Metadata")
    FString ItemName;

    UPROPERTY(BlueprintReadWrite, Category = "Content Metadata")
    FString Description;

    UPROPERTY(BlueprintReadWrite, Category = "Content Metadata")
    EMingAIContentType ContentType;

    UPROPERTY(BlueprintReadWrite, Category = "Content Metadata")
    EMingAIQualityLevel QualityLevel;

    UPROPERTY(BlueprintReadWrite, Category = "Content Metadata")
    FString FilePath;

    UPROPERTY(BlueprintReadWrite, Category = "Content Metadata")
    FString ThumbnailPath;

    UPROPERTY(BlueprintReadWrite, Category = "Content Metadata")
    FString Tags;

    UPROPERTY(BlueprintReadWrite, Category = "Content Metadata")
    FString Category;

    UPROPERTY(BlueprintReadWrite, Category = "Content Metadata")
    FDateTime CreationDate;

    UPROPERTY(BlueprintReadWrite, Category = "Content Metadata")
    FDateTime LastModified;

    UPROPERTY(BlueprintReadWrite, Category = "Content Metadata")
    int64 FileSize;

    UPROPERTY(BlueprintReadWrite, Category = "Content Metadata")
    EMingContentItemStatus Status;

    UPROPERTY(BlueprintReadWrite, Category = "Content Metadata")
    int32 UsageCount;

    UPROPERTY(BlueprintReadWrite, Category = "Content Metadata")
    float Rating;

    UPROPERTY(BlueprintReadWrite, Category = "Content Metadata")
    TMap<FString, FString> CustomProperties;

    FMingContentItemMetadata()
        : ContentType(EMingAIContentType::None)
        , QualityLevel(EMingAIQualityLevel::Standard)
        , FileSize(0)
        , Status(EMingContentItemStatus::Active)
        , UsageCount(0)
        , Rating(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingContentProject
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Content Project")
    FGuid ProjectID;

    UPROPERTY(BlueprintReadWrite, Category = "Content Project")
    FString ProjectName;

    UPROPERTY(BlueprintReadWrite, Category = "Content Project")
    FString Description;

    UPROPERTY(BlueprintReadWrite, Category = "Content Project")
    TArray<EMingAIContentType> RequiredContentTypes;

    UPROPERTY(BlueprintReadWrite, Category = "Content Project")
    TMap<EMingAIContentType, FString> ContentPrompts;

    UPROPERTY(BlueprintReadWrite, Category = "Content Project")
    EMingAIQualityLevel TargetQuality;

    UPROPERTY(BlueprintReadWrite, Category = "Content Project")
    FString OutputDirectory;

    FMingContentProject()
        : TargetQuality(EMingAIQualityLevel::High)
    {}
};

/**
 * AI Content Manager
 * Central manager for organizing and managing AI-generated content
 */
UCLASS(ClassGroup = (AI, Content), Blueprintable)
class MINGAI_API UMingAIContentManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAIContentManager();

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void InitializeManager();

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void ShutdownManager();

    // Content Registration
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    FGuid RegisterContent(const FMingContentItemMetadata& Metadata);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    bool UpdateContentMetadata(const FGuid& ItemID, const FMingContentItemMetadata& Metadata);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    bool DeleteContent(const FGuid& ItemID);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    bool ArchiveContent(const FGuid& ItemID);

    // Content Retrieval
    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    FMingContentItemMetadata GetContentMetadata(const FGuid& ItemID) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    TArray<FGuid> GetAllContentIDs() const;

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    TArray<FGuid> GetContentByType(EMingAIContentType ContentType) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    TArray<FGuid> GetContentByCategory(const FString& Category) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    TArray<FGuid> GetContentByTag(const FString& Tag) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    TArray<FGuid> SearchContent(const FString& SearchQuery) const;

    // Content Organization
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    bool MoveContentToCategory(const FGuid& ItemID, const FString& NewCategory);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    bool AddTagToContent(const FGuid& ItemID, const FString& Tag);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    bool RemoveTagFromContent(const FGuid& ItemID, const FString& Tag);

    // Content Operations
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    bool ImportContent(const FString& FilePath, const FMingContentItemMetadata& Metadata);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    bool ExportContent(const FGuid& ItemID, const FString& DestinationPath);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    bool DuplicateContent(const FGuid& ItemID, const FString& NewName);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    bool PreviewContent(const FGuid& ItemID);

    // Project Management
    UFUNCTION(BlueprintCallable, Category = "AI Content Projects")
    FGuid CreateProject(const FMingContentProject& Project);

    UFUNCTION(BlueprintCallable, Category = "AI Content Projects")
    bool UpdateProject(const FGuid& ProjectID, const FMingContentProject& Project);

    UFUNCTION(BlueprintCallable, Category = "AI Content Projects")
    bool DeleteProject(const FGuid& ProjectID);

    UFUNCTION(BlueprintPure, Category = "AI Content Projects")
    FMingContentProject GetProject(const FGuid& ProjectID) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Projects")
    TArray<FGuid> GetAllProjects() const;

    UFUNCTION(BlueprintCallable, Category = "AI Content Projects")
    bool AddContentToProject(const FGuid& ProjectID, const FGuid& ContentID);

    UFUNCTION(BlueprintCallable, Category = "AI Content Projects")
    bool RemoveContentFromProject(const FGuid& ProjectID, const FGuid& ContentID);

    UFUNCTION(BlueprintPure, Category = "AI Content Projects")
    TArray<FGuid> GetProjectContents(const FGuid& ProjectID) const;

    // Statistics and Reporting
    UFUNCTION(BlueprintPure, Category = "AI Content Statistics")
    int32 GetTotalContentCount() const;

    UFUNCTION(BlueprintPure, Category = "AI Content Statistics")
    int32 GetContentCountByType(EMingAIContentType ContentType) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Statistics")
    int64 GetTotalStorageUsed() const;

    UFUNCTION(BlueprintPure, Category = "AI Content Statistics")
    TArray<FString> GetAllCategories() const;

    UFUNCTION(BlueprintPure, Category = "AI Content Statistics")
    TArray<FString> GetAllTags() const;

    // Cache Management
    UFUNCTION(BlueprintCallable, Category = "AI Content Cache")
    void ClearCache();

    UFUNCTION(BlueprintCallable, Category = "AI Content Cache")
    void CleanUnusedContent(int32 DaysUnused);

    UFUNCTION(BlueprintPure, Category = "AI Content Cache")
    int64 GetCacheSize() const;

protected:
    UPROPERTY()
    TMap<FGuid, FMingContentItemMetadata> ContentRegistry;

    UPROPERTY()
    TMap<FGuid, FMingContentProject> Projects;

    UPROPERTY()
    TMap<FGuid, TArray<FGuid>> ProjectContents;

    UPROPERTY()
    FString ContentDirectory;

    UPROPERTY()
    FString CacheDirectory;

    UPROPERTY()
    bool bInitialized;

    // Internal methods
    bool ValidateMetadata(const FMingContentItemMetadata& Metadata) const;
    FString GenerateUniqueFileName(EMingAIContentType ContentType) const;
    bool CreateThumbnail(const FString& SourcePath, const FString& ThumbnailPath);
    void UpdateContentUsage(const FGuid& ItemID);
    void SaveRegistry();
    void LoadRegistry();
    FString GetContentTypeString(EMingAIContentType ContentType) const;
    FString GetFileExtension(EMingAIContentType ContentType) const;
};
