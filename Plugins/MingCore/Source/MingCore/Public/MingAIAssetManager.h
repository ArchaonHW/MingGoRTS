#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAIAssetGenerator.h"
#include "MingAIAssetManager.generated.h"

/**
 * è³‡ç”¢?†é??šè?
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
 * è³‡ç”¢?ƒæ•¸?šç?æ§? */
USTRUCT(BlueprintType)
struct FAssetMetadata
{
    GENERATED_BODY()

    // è³‡ç”¢ID
    UPROPERTY(BlueprintReadOnly)
    FString AssetID;

    // è³‡ç”¢?ç¨±
    UPROPERTY(BlueprintReadOnly)
    FString AssetName;

    // è³‡ç”¢é¡å?
    UPROPERTY(BlueprintReadOnly)
    EAIAssetType AssetType;

    // è³‡ç”¢?†é?
    UPROPERTY(BlueprintReadOnly)
    EAssetCategory Category;

    // æ°‘å??‚æ?é¢¨æ ¼
    UPROPERTY(BlueprintReadOnly)
    ERepublicEraStyle EraStyle;

    // ?µå»º?‚é?
    UPROPERTY(BlueprintReadOnly)
    FDateTime CreatedTime;

    // ?‡ä»¶å¤§å? (bytes)
    UPROPERTY(BlueprintReadOnly)
    int64 FileSize;

    // ?‡ä»¶è·¯å?
    UPROPERTY(BlueprintReadOnly)
    FString FilePath;

    // ç¸®ç•¥?–è·¯å¾?    UPROPERTY(BlueprintReadOnly)
    FString ThumbnailPath;

    // æ¨™ç±¤
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Tags;

    // è©•å? (1-5)
    UPROPERTY(BlueprintReadOnly)
    float Rating;

    // ä½¿ç”¨æ¬¡æ•¸
    UPROPERTY(BlueprintReadOnly)
    int32 UsageCount;

    // ?¯å¦?¶è?
    UPROPERTY(BlueprintReadOnly)
    bool bIsFavorite;

    // ?ªå?ç¾©å±¬??    UPROPERTY(BlueprintReadOnly)
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
 * è³‡ç”¢?œç´¢?æ¿¾?? */
USTRUCT(BlueprintType)
struct FAssetFilter
{
    GENERATED_BODY()

    // ?œç´¢?œéµè©?    UPROPERTY(BlueprintReadWrite)
    FString SearchKeyword;

    // è³‡ç”¢é¡å?
    UPROPERTY(BlueprintReadWrite)
    TArray<EAIAssetType> AssetTypes;

    // è³‡ç”¢?†é?
    UPROPERTY(BlueprintReadWrite)
    TArray<EAssetCategory> Categories;

    // æ°‘å??‚æ?é¢¨æ ¼
    UPROPERTY(BlueprintReadWrite)
    TArray<ERepublicEraStyle> EraStyles;

    // ?€å°è???    UPROPERTY(BlueprintReadWrite)
    float MinRating;

    // ?ªé¡¯ç¤ºæ”¶??    UPROPERTY(BlueprintReadWrite)
    bool bFavoritesOnly;

    // ?ªé¡¯ç¤ºå·²ä½¿ç”¨
    UPROPERTY(BlueprintReadWrite)
    bool bUsedOnly;

    FAssetFilter()
        : MinRating(0.0f)
        , bFavoritesOnly(false)
        , bUsedOnly(false)
    {}
};

/**
 * AIè³‡ç”¢ç®¡ç??? * ç®¡ç??€?‰AI?Ÿæ??„è??? */
UCLASS(BlueprintType)
class MINGCORE_API UMingAIAssetManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAIAssetManager();

    /**
     * ?å??–è??¢ç®¡?†å™¨
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    void Initialize();

    /**
     * ?œé?è³‡ç”¢ç®¡ç???     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    void Shutdown();

    /**
     * æ·»å?è³‡ç”¢
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool AddAsset(const FAIAssetGenerationResult& GenerationResult);

    /**
     * ç§»é™¤è³‡ç”¢
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool RemoveAsset(const FString& AssetID);

    /**
     * ?²å?è³‡ç”¢
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Manager")
    FAssetMetadata GetAsset(const FString& AssetID) const;

    /**
     * ?œç´¢è³‡ç”¢
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    TArray<FAssetMetadata> SearchAssets(const FAssetFilter& Filter) const;

    /**
     * ?²å??€?‰è???     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Manager")
    TArray<FAssetMetadata> GetAllAssets() const;

    /**
     * ?²å?è³‡ç”¢çµ±è?
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Manager")
    TMap<EAssetCategory, int32> GetAssetStatistics() const;

    /**
     * æ¨™è?è³‡ç”¢?ºæ”¶??     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool SetAssetFavorite(const FString& AssetID, bool bFavorite);

    /**
     * è©•å?è³‡ç”¢
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool RateAsset(const FString& AssetID, float Rating);

    /**
     * å¢å?ä½¿ç”¨æ¬¡æ•¸
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool IncrementUsage(const FString& AssetID);

    /**
     * å°å‡ºè³‡ç”¢
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool ExportAsset(const FString& AssetID, const FString& ExportPath) const;

    /**
     * ?¹é?å°å‡ºè³‡ç”¢
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool ExportAssets(const TArray<FString>& AssetIDs, const FString& ExportDirectory) const;

    /**
     * å°å…¥è³‡ç”¢
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    bool ImportAsset(const FString& FilePath, EAssetCategory Category);

    /**
     * æ¸…ç??¡æ?è³‡ç”¢
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    int32 CleanupInvalidAssets();

    /**
     * ?²å?å­˜å„²ç©ºé?ä½¿ç”¨?…æ?
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Manager")
    float GetStorageUsageMB() const;

    /**
     * ?²å??¨è–¦è³‡ç”¢
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Manager")
    TArray<FAssetMetadata> GetRecommendedAssets(EAssetCategory Category, int32 Count = 10) const;

    /**
     * ?œæ??²å?å¯¦ä?
     */
    static UMingAIAssetManager* Get();

protected:
    // è³‡ç”¢?—è¡¨
    UPROPERTY()
    TArray<FAssetMetadata> AssetList;

    // ?¯å¦å·²å?å§‹å?
    bool bIsInitialized;

    // ?®ä?å¯¦ä?
    static UMingAIAssetManager* Instance;

    // è³‡ç”¢?¸æ??‡ä»¶è·¯å?
    FString AssetDatabasePath;

    // è³‡ç”¢å­˜å„²?®é?
    FString AssetStorageDirectory;

    // ? è?è³‡ç”¢?¸æ?åº?    bool LoadAssetDatabase();

    // ä¿å?è³‡ç”¢?¸æ?åº?    bool SaveAssetDatabase() const;

    // ?µå»ºè³‡ç”¢?ƒæ•¸??    FAssetMetadata CreateAssetMetadata(const FAIAssetGenerationResult& GenerationResult);

    // ç¢ºå?è³‡ç”¢?†é?
    EAssetCategory DetermineAssetCategory(EAIAssetType AssetType);

    // é©—è?è³‡ç”¢?‡ä»¶
    bool ValidateAssetFile(const FString& FilePath) const;

    // ?Ÿæ?ç¸®ç•¥??    FString GenerateThumbnail(const FString& AssetPath) const;

    // ?²å??‡ä»¶å¤§å?
    int64 GetFileSize(const FString& FilePath) const;

    // ?œç´¢?¹é?æª¢æŸ¥
    bool MatchesFilter(const FAssetMetadata& Asset, const FAssetFilter& Filter) const;

    // ?’å?è³‡ç”¢
    void SortAssets(TArray<FAssetMetadata>& Assets, const FString& SortBy) const;

    // å»ºç?è³‡ç”¢?®é?
    bool CreateAssetDirectories();
};
