#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRegionalContentManager.generated.h"

// Forward declarations
class UMingLocalizationManager;

// Content availability status
UENUM(BlueprintType)
enum class ERegionalContentStatus : uint8
{
    Available          UMETA(DisplayName = "Available"),
    Restricted         UMETA(DisplayName = "Restricted"),
    Modified           UMETA(DisplayName = "Modified"),
    Unavailable        UMETA(DisplayName = "Unavailable"),
    Pending            UMETA(DisplayName = "Pending"),
    ComingSoon         UMETA(DisplayName = "Coming Soon")
};

// Regional content type
UENUM(BlueprintType)
enum class ERegionalContentType : uint8
{
    Character          UMETA(DisplayName = "Character"),
    Story              UMETA(DisplayName = "Story"),
    Quest              UMETA(DisplayName = "Quest"),
    Item               UMETA(DisplayName = "Item"),
    Location           UMETA(DisplayName = "Location"),
    Audio              UMETA(DisplayName = "Audio"),
    Visual             UMETA(DisplayName = "Visual"),
    Event              UMETA(DisplayName = "Event"),
    Feature            UMETA(DisplayName = "Feature"),
    DLC                UMETA(DisplayName = "DLC")
};

// Regional content information
USTRUCT(BlueprintType)
struct FRegionalContentInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ContentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ERegionalContentType ContentType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ContentName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> AvailableRegions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RestrictedRegions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ERegionalContentStatus Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AlternativeContentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RegionSpecificVersion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime ReleaseDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Version;

    FRegionalContentInfo()
        : ContentType(ERegionalContentType::Character)
        , Status(ERegionalContentStatus::Available)
    {}
};

// Region-specific content variant
USTRUCT(BlueprintType)
struct FRegionalVariant
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OriginalContentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RegionalContentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RegionCode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString VariantDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Differences;

    FRegionalVariant()
    {}
};

// Content bundle for a region
USTRUCT(BlueprintType)
struct FRegionalContentBundle
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BundleID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RegionCode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BundleName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ContentIDs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BundleSizeMB;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsRequired;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsInstalled;

    FRegionalContentBundle()
        : BundleSizeMB(0.0f)
        , bIsRequired(false)
        , bIsInstalled(false)
    {}
};

// Content availability report
USTRUCT(BlueprintType)
struct FContentAvailabilityReport
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString RegionCode;

    UPROPERTY(BlueprintReadOnly)
    int32 TotalContent;

    UPROPERTY(BlueprintReadOnly)
    int32 AvailableContent;

    UPROPERTY(BlueprintReadOnly)
    int32 RestrictedContent;

    UPROPERTY(BlueprintReadOnly)
    int32 ModifiedContent;

    UPROPERTY(BlueprintReadOnly)
    int32 UnavailableContent;

    UPROPERTY(BlueprintReadOnly)
    TArray<FRegionalContentInfo> ContentList;

    FContentAvailabilityReport()
        : TotalContent(0)
        , AvailableContent(0)
        , RestrictedContent(0)
        , ModifiedContent(0)
        , UnavailableContent(0)
    {}
};

/**
 * 地區內容管理器
 * 管理不同地區的內容差異和可用性
 */
UCLASS(ClassGroup = (Localization), Blueprintable)
class MINGPERSONAL_API UMingRegionalContentManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRegionalContentManager();

    // 初始化
    void Initialize();
    void Shutdown();

    UFUNCTION(BlueprintPure, Category = "Regional Content")
    bool IsInitialized() const { return bIsInitialized; }

    // === 內容可用性 ===

    // 檢查內容在特定地區是否可用
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Availability")
    bool IsContentAvailable(const FString& ContentID, const FString& RegionCode) const;

    // 獲取內容在特定地區的狀態
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Availability")
    ERegionalContentStatus GetContentStatus(const FString& ContentID, const FString& RegionCode) const;

    // 獲取內容的替代版本ID
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Availability")
    FString GetAlternativeContentID(const FString& ContentID, const FString& RegionCode) const;

    // === 內容註冊 ===

    // 註冊地區內容
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Registry")
    void RegisterRegionalContent(const FRegionalContentInfo& ContentInfo);

    // 更新內容可用性
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Registry")
    void UpdateContentAvailability(const FString& ContentID, const FString& RegionCode, ERegionalContentStatus Status);

    // 移除內容註冊
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Registry")
    void UnregisterContent(const FString& ContentID);

    // === 內容查詢 ===

    // 獲取特定地區的所有可用內容
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Query")
    TArray<FRegionalContentInfo> GetAvailableContentForRegion(const FString& RegionCode, ERegionalContentType ContentType = ERegionalContentType::Character) const;

    // 獲取特定地區的受限內容
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Query")
    TArray<FRegionalContentInfo> GetRestrictedContentForRegion(const FString& RegionCode) const;

    // 獲取內容的所有地區變體
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Query")
    TArray<FRegionalVariant> GetRegionalVariants(const FString& ContentID) const;

    // 獲取內容的詳細信息
    UFUNCTION(BlueprintPure, Category = "Regional Content|Query")
    FRegionalContentInfo GetContentInfo(const FString& ContentID) const;

    // === 內容包管理 ===

    // 註冊內容包
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Bundles")
    void RegisterContentBundle(const FRegionalContentBundle& Bundle);

    // 獲取地區的內容包
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Bundles")
    TArray<FRegionalContentBundle> GetContentBundlesForRegion(const FString& RegionCode) const;

    // 安裝內容包
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Bundles")
    bool InstallContentBundle(const FString& BundleID);

    // 卸載內容包
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Bundles")
    bool UninstallContentBundle(const FString& BundleID);

    // === 報告和統計 ===

    // 生成內容可用性報告
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Reports")
    FContentAvailabilityReport GenerateAvailabilityReport(const FString& RegionCode) const;

    // 比較兩個地區的內容差異
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Reports")
    TArray<FRegionalContentInfo> CompareRegions(const FString& RegionCode1, const FString& RegionCode2) const;

    // 獲取內容的全球可用性
    UFUNCTION(BlueprintPure, Category = "Regional Content|Reports")
    int32 GetGlobalContentAvailability(const FString& ContentID) const;

    // === 動態內容調整 ===

    // 根據地區調整內容
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Adjustment")
    FString AdjustContentForRegion(const FString& ContentID, const FString& RegionCode) const;

    // 獲取地區特定的內容描述
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Adjustment")
    FString GetRegionalDescription(const FString& ContentID, const FString& RegionCode) const;

    // 檢查內容是否需要地區適配
    UFUNCTION(BlueprintPure, Category = "Regional Content|Adjustment")
    bool NeedsRegionalAdaptation(const FString& ContentID, const FString& RegionCode) const;

    // === 特定內容類型 ===

    // 獲取地區特定角色
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Specific")
    TArray<FRegionalContentInfo> GetRegionalCharacters(const FString& RegionCode) const;

    // 獲取地區特定故事
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Specific")
    TArray<FRegionalContentInfo> GetRegionalStories(const FString& RegionCode) const;

    // 獲取地區特定任務
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Specific")
    TArray<FRegionalContentInfo> GetRegionalQuests(const FString& RegionCode) const;

    // 獲取地區特定物品
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Specific")
    TArray<FRegionalContentInfo> GetRegionalItems(const FString& RegionCode) const;

    // === 數據管理 ===

    // 加載地區內容數據
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Data")
    bool LoadRegionalData(const FString& FilePath);

    // 保存地區內容數據
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Data")
    bool SaveRegionalData(const FString& FilePath) const;

    // 導出地區內容列表
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Data")
    bool ExportContentList(const FString& FilePath, const FString& RegionCode) const;

    // 從配置初始化
    UFUNCTION(BlueprintCallable, Category = "Regional Content|Data")
    void InitializeFromConfig();

private:
    bool bIsInitialized;

    // 內容註冊表
    UPROPERTY()
    TMap<FString, FRegionalContentInfo> ContentRegistry;

    // 地區變體存儲
    UPROPERTY()
    TMap<FString, TArray<FRegionalVariant>> RegionalVariants;

    // 內容包註冊表
    UPROPERTY()
    TArray<FRegionalContentBundle> ContentBundles;

    // 初始化默認內容
    void InitializeDefaultContent();

    // 加載預設內容規則
    void LoadPresetContent();
};
