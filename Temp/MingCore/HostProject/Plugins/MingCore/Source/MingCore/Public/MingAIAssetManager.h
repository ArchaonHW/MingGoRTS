#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAIAssetGenerator.h"
#include "MingAIAssetManager.generated.h"

/**
 * 資產?��X��?
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
 * 資產?�數?��?�? */
USTRUCT(BlueprintType)
struct FAssetMetadata
{
    GENERATED_BODY()

    // 資產ID
    UPROPERTY(BlueprintReadOnly)
    FString AssetID;

    // 資產?�稱
    UPROPERTY(BlueprintReadOnly)
    FString AssetName;

    // 資產類�?
    UPROPERTY(BlueprintReadOnly)
    EAIAssetType AssetType;

    // 資產?��?
    UPROPERTY(BlueprintReadOnly)
    EAssetCategory Category;

    // 民�X��?風格
    UPROPERTY(BlueprintReadOnly)
    ERepublicEraStyle EraStyle;

    // ?�建?��?
    UPROPERTY(BlueprintReadOnly)
    FDateTime CreatedTime;

    // ?�件大�? (bytes)
    UPROPERTY(BlueprintReadOnly)
    int64 FileSize;

    // ?�件路�?
    UPROPERTY(BlueprintReadOnly)
    FString FilePath;

    // 縮略?�路�?    UPROPERTY(BlueprintReadOnly)
    FString ThumbnailPath;

    // 標籤
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Tags;

    // 評�? (1-5)
    UPROPERTY(BlueprintReadOnly)
    float Rating;

    // 使用次數
    UPROPERTY(BlueprintReadOnly)
    int32 UsageCount;

    // ?�否?��?
    UPROPERTY(BlueprintReadOnly)
    bool bIsFavorite;

    // ?��?義屬X    UPROPERTY(BlueprintReadOnly)
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
 * 資產?�索?�濾X */
USTRUCT(BlueprintType)
struct FAssetFilter
{
    GENERATED_BODY()

    // ?�索?�鍵�?    UPROPERTY(BlueprintReadWrite)
    FString SearchKeyword;

    // 資產類�?
    UPROPERTY(BlueprintReadWrite)
    TArray<EAIAssetType> AssetTypes;

    // 資產?��?
    UPROPERTY(BlueprintReadWrite)
    TArray<EAssetCategory> Categories;

    // 民�X��?風格
    UPROPERTY(BlueprintReadWrite)
    TArray<ERepublicEraStyle> EraStyles;

    // ?�小�X    UPROPERTY(BlueprintReadWrite)
    float MinRating;

    // ?�顯示收X    UPROPERTY(BlueprintReadWrite)
    bool bFavoritesOnly;

    // ?�顯示已使用
    UPROPERTY(BlueprintReadWrite)
    bool bUsedOnly;

    FAssetFilter()
        : MinRating(0.0f)
        , bFavoritesOnly(false)
        , bUsedOnly(false)
    {}
};

/**
 * AI資產管�X * 管�X�?�AI?��X��X */
UCLASS(BlueprintType)
class MINGCORE_API UMingAIAssetManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAIAssetManager(};

    /**
     * ?��X��X�管?�器
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    void Initialize(};

    /**
     * ?��?資產管�X     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    void Shutdown(};

    /**
     * 添�?資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool AddAsset(const FAIAssetGenerationResult& GenerationResult) {};

    /**
     * 移除資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool RemoveAsset(const FString& AssetID) {};

    /**
     * ?��?資產
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Manager")
    FAssetMetadata GetAsset(const FString& AssetID) const;

    /**
     * ?�索資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    TArray<FAssetMetadata> SearchAssets(const FAssetFilter& Filter) const;

    /**
     * ?��X�?��X     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Manager")
    TArray<FAssetMetadata> GetAllAssets() const;

    /**
     * ?��?資產統�?
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Manager")
    TMap<EAssetCategory, int32> GetAssetStatistics() const;

    /**
     * 標�?資產?�收X     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool SetAssetFavorite(const FString& AssetID, bool bFavorite};

    /**
     * 評�?資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool RateAsset(const FString& AssetID, float Rating};

    /**
     * 增�?使用次數
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool IncrementUsage(const FString& AssetID) {};

    /**
     * 導出資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool ExportAsset(const FString& AssetID, const FString& ExportPath) const;

    /**
     * ?��?導出資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool ExportAssets(const TArray<FString>& AssetIDs, const FString& ExportDirectory) const;

    /**
     * 導入資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool ImportAsset(const FString& FilePath, EAssetCategory Category};

    /**
     * 清�X��?資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    int32 CleanupInvalidAssets(};

    /**
     * ?��?存儲空�?使用?��?
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Manager")
    float GetStorageUsageMB() const;

    /**
     * ?��X�薦資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    TArray<FAssetMetadata> GetRecommendedAssets(EAssetCategory Category, int32 Count = 10) const;

    /**
     * ?��X��?實�?
     */
    static UMingAIAssetManager* Get(};

protected:
    // 資產?�表
    UPROPERTY()
    TArray<FAssetMetadata> AssetList;

    // ?�否已�?始�?
    bool bIsInitialized;

    // ?��?實�?
    static UMingAIAssetManager* Instance;

    // 資產?��X�件路�?
    FString AssetDatabasePath;

    // 資產存儲?��?
    FString AssetStorageDirectory;

    // ?��?資產?��?�?    bool LoadAssetDatabase(};

    // 保�?資產?��?�?    bool SaveAssetDatabase() const;

    // ?�建資產?�數X    FAssetMetadata CreateAssetMetadata(const FAIAssetGenerationResult& GenerationResult) {};

    // 確�?資產?��?
    EAssetCategory DetermineAssetCategory(EAIAssetType AssetType};

    // 驗�?資產?�件
    bool ValidateAssetFile(const FString& FilePath) const;

    // ?��?縮略X    FString GenerateThumbnail(const FString& AssetPath) const;

    // ?��X�件大�?
    int64 GetFileSize(const FString& FilePath) const;

    // ?�索?��?檢查
    bool MatchesFilter(const FAssetMetadata& Asset, const FAssetFilter& Filter) const;

    // ?��?資產
    void SortAssets(TArray<FAssetMetadata>& Assets, const FString& SortBy) const;

    // 建�?資產?��?
    bool CreateAssetDirectories(};
};

