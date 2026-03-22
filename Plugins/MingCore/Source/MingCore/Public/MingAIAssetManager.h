#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAIAssetGenerator.h"
#include "MingAIAssetManager.generated.h"

/**
 * 資產分類枚舉
 */
UENUM(BlueprintType)
enum class EAssetCategory : uint8
{
    Characters      UMETA(DisplayName = "Characters"),
    Buildings       UMETA(DisplayName = "Buildings"),
    Weapons         UMETA(DisplayName = "Weapons"),
    Uniforms        UMETA(DisplayName = "Uniforms"),
    Props           UMETA(DisplayName = "Props"),
    Landscapes      UMETA(DisplayName = "Landscapes"),
    Music           UMETA(DisplayName = "Music"),
    Sounds          UMETA(DisplayName = "Sounds"),
    Voices          UMETA(DisplayName = "Voices")
};

/**
 * 資產元數據結構
 */
USTRUCT(BlueprintType)
struct FAssetMetadata
{
    GENERATED_BODY()

    // 資產ID
    UPROPERTY(BlueprintReadOnly)
    FString AssetID;

    // 資產名稱
    UPROPERTY(BlueprintReadOnly)
    FString AssetName;

    // 資產類型
    UPROPERTY(BlueprintReadOnly)
    EAIAssetType AssetType;

    // 資產分類
    UPROPERTY(BlueprintReadOnly)
    EAssetCategory Category;

    // 民國時期風格
    UPROPERTY(BlueprintReadOnly)
    ERepublicEraStyle EraStyle;

    // 創建時間
    UPROPERTY(BlueprintReadOnly)
    FDateTime CreatedTime;

    // 文件大小 (bytes)
    UPROPERTY(BlueprintReadOnly)
    int64 FileSize;

    // 文件路徑
    UPROPERTY(BlueprintReadOnly)
    FString FilePath;

    // 縮略圖路徑
    UPROPERTY(BlueprintReadOnly)
    FString ThumbnailPath;

    // 標籤
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Tags;

    // 評分 (1-5)
    UPROPERTY(BlueprintReadOnly)
    float Rating;

    // 使用次數
    UPROPERTY(BlueprintReadOnly)
    int32 UsageCount;

    // 是否收藏
    UPROPERTY(BlueprintReadOnly)
    bool bIsFavorite;

    // 自定義屬性
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CustomProperties;

    FAssetMetadata()
        : AssetType(EAIAssetType::CharacterPortrait)
        , Category(EAssetCategory::Characters)
        , EraStyle(ERepublicEraStyle::NanjingDecade)
        , FileSize(0)
        , Rating(0.0f)
        , UsageCount(0)
        , bIsFavorite(false)
    {}
};

/**
 * 資產搜索過濾器
 */
USTRUCT(BlueprintType)
struct FAssetFilter
{
    GENERATED_BODY()

    // 搜索關鍵詞
    UPROPERTY(BlueprintReadWrite)
    FString SearchKeyword;

    // 資產類型
    UPROPERTY(BlueprintReadWrite)
    TArray<EAIAssetType> AssetTypes;

    // 資產分類
    UPROPERTY(BlueprintReadWrite)
    TArray<EAssetCategory> Categories;

    // 民國時期風格
    UPROPERTY(BlueprintReadWrite)
    TArray<ERepublicEraStyle> EraStyles;

    // 最小評分
    UPROPERTY(BlueprintReadWrite)
    float MinRating;

    // 只顯示收藏
    UPROPERTY(BlueprintReadWrite)
    bool bFavoritesOnly;

    // 只顯示已使用
    UPROPERTY(BlueprintReadWrite)
    bool bUsedOnly;

    FAssetFilter()
        : MinRating(0.0f)
        , bFavoritesOnly(false)
        , bUsedOnly(false)
    {}
};

/**
 * AI資產管理器
 * 管理所有AI生成的資產
 */
UCLASS(BlueprintType)
class MINGCORE_API UMingAIAssetManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAIAssetManager();

    /**
     * 初始化資產管理器
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    void Initialize();

    /**
     * 關閉資產管理器
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    void Shutdown();

    /**
     * 添加資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool AddAsset(const FAIAssetGenerationResult& GenerationResult);

    /**
     * 移除資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool RemoveAsset(const FString& AssetID);

    /**
     * 獲取資產
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Manager")
    FAssetMetadata GetAsset(const FString& AssetID) const;

    /**
     * 搜索資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    TArray<FAssetMetadata> SearchAssets(const FAssetFilter& Filter) const;

    /**
     * 獲取所有資產
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Manager")
    TArray<FAssetMetadata> GetAllAssets() const;

    /**
     * 獲取資產統計
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Manager")
    TMap<EAssetCategory, int32> GetAssetStatistics() const;

    /**
     * 標記資產為收藏
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool SetAssetFavorite(const FString& AssetID, bool bFavorite);

    /**
     * 評分資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool RateAsset(const FString& AssetID, float Rating);

    /**
     * 增加使用次數
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool IncrementUsage(const FString& AssetID);

    /**
     * 導出資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool ExportAsset(const FString& AssetID, const FString& ExportPath) const;

    /**
     * 批量導出資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool ExportAssets(const TArray<FString>& AssetIDs, const FString& ExportDirectory) const;

    /**
     * 導入資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool ImportAsset(const FString& FilePath, EAssetCategory Category);

    /**
     * 清理無效資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    int32 CleanupInvalidAssets();

    /**
     * 獲取存儲空間使用情況
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Manager")
    float GetStorageUsageMB() const;

    /**
     * 獲取推薦資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    TArray<FAssetMetadata> GetRecommendedAssets(EAssetCategory Category, int32 Count = 10) const;

    /**
     * 靜態獲取實例
     */
    static UMingAIAssetManager* Get();

protected:
    // 資產列表
    UPROPERTY()
    TArray<FAssetMetadata> AssetList;

    // 是否已初始化
    bool bIsInitialized;

    // 單例實例
    static UMingAIAssetManager* Instance;

    // 資產數據文件路徑
    FString AssetDatabasePath;

    // 資產存儲目錄
    FString AssetStorageDirectory;

    // 加載資產數據庫
    bool LoadAssetDatabase();

    // 保存資產數據庫
    bool SaveAssetDatabase() const;

    // 創建資產元數據
    FAssetMetadata CreateAssetMetadata(const FAIAssetGenerationResult& GenerationResult);

    // 確定資產分類
    EAssetCategory DetermineAssetCategory(EAIAssetType AssetType);

    // 驗證資產文件
    bool ValidateAssetFile(const FString& FilePath) const;

    // 生成縮略圖
    FString GenerateThumbnail(const FString& AssetPath) const;

    // 獲取文件大小
    int64 GetFileSize(const FString& FilePath) const;

    // 搜索匹配檢查
    bool MatchesFilter(const FAssetMetadata& Asset, const FAssetFilter& Filter) const;

    // 排序資產
    void SortAssets(TArray<FAssetMetadata>& Assets, const FString& SortBy) const;

    // 建立資產目錄
    bool CreateAssetDirectories();
};
