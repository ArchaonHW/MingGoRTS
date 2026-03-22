#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/MingDecisionConsequenceManager.h"
#include "MingConsequenceCache.generated.h"

// Á∑©Â?Ê¢ùÁõÆ
USTRUCT(BlueprintType)
struct MINGCORE_API FMingConsequenceCacheEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString CacheKey;

    UPROPERTY(BlueprintReadOnly)
    FMingConsequenceResult CachedResult;

    UPROPERTY(BlueprintReadOnly)
    FDateTime CacheTime;

    UPROPERTY(BlueprintReadOnly)
    int32 AccessCount;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastAccessTime;

    UPROPERTY(BlueprintReadOnly)
    float CacheWeight;
};

/**
 * ÂæåÊ?Ë®àÁ?Á∑©Â?Á≥ªÁµ±
 * ?êÂ??çË?Ê±∫Á??ÑË?ÁÆóÊÄßËÉΩ
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingConsequenceCache : public UObject
{
    GENERATED_BODY()

public:
    // Âª∫Ê?Â≠?    UMingConsequenceCache();

    // ?ùÂ??ñÁ∑©Â≠?    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    void Initialize(int32 MaxCacheSize = 1000, float CacheExpirationHours = 24.0f);

    // ?≤Â?Á∑©Â?ÁµêÊ?
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    bool GetCachedResult(const FString& CacheKey, FMingConsequenceResult& OutResult);

    // Á∑©Â?ÁµêÊ?
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    void CacheResult(const FString& CacheKey, const FMingConsequenceResult& Result);

    // Ê∏ÖÈô§?éÊ?Á∑©Â?
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    void ClearExpiredCache();

    // Ê∏ÖÈô§?Ä?âÁ∑©Â≠?    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    void ClearAllCache();

    // ?≤Â?Á∑©Â?Áµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    void GetCacheStatistics(int32& TotalEntries, int32& CacheHits, int32& CacheMisses, float& HitRate);

    // ?üÊ?Á∑©Â???    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    FString GenerateCacheKey(const FMingDecisionContext& Context) const;

protected:
    // Á∑©Â?Â≠òÂÑ≤
    TMap<FString, FMingConsequenceCacheEntry> CacheMap;

    // Á∑©Â??çÁΩÆ
    int32 MaxCacheSize;
    float CacheExpirationHours;

    // Áµ±Ë??∏Ê?
    int32 CacheHits;
    int32 CacheMisses;

    // Ê∏ÖÁ?‰ΩéÊ??çÁ∑©Â≠?    void CleanupLowWeightCache();

    // Ë®àÁ?Á∑©Â?Ê¨äÈ?
    float CalculateCacheWeight(const FMingConsequenceCacheEntry& Entry) const;

    // Ê™¢Êü•Á∑©Â??ØÂê¶?éÊ?
    bool IsCacheExpired(const FMingConsequenceCacheEntry& Entry) const;

    // ?¥Êñ∞Á∑©Â?Ë®™Â?Áµ±Ë?
    void UpdateCacheAccess(const FString& CacheKey);

private:
    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bInitialized = false;
};
