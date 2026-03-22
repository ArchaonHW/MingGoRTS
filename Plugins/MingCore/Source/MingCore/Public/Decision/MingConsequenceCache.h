#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/MingDecisionConsequenceManager.h"
#include "MingConsequenceCache.generated.h"

// 緩??條目
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
 * 後??計??緩??系統
 * ???X???決?X???算性能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingConsequenceCache : public UObject
{
    GENERATED_BODY()

public:
    // 建????    UMingConsequenceCache();

    
    void Initialize(int32 MaxCacheSize = 1000, float CacheExpirationHours = 24.0f};

    // ????緩??結??
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    bool GetCachedResult(const FString& CacheKey, FMingConsequenceResult& OutResult};

    // 緩??結??
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    void CacheResult(const FString& CacheKey, const FMingConsequenceResult& Result};

    // 清除????緩??
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    void ClearExpiredCache();

    
    void ClearAllCache();

    // ????緩??統??
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    void GetCacheStatistics(int32& TotalEntries, int32& CacheHits, int32& CacheMisses, float& HitRate};

    
    FString GenerateCacheKey(const FMingDecisionContext& Context) const;

protected:
    // 緩??存儲
    TMap<FString, FMingConsequenceCacheEntry> CacheMap;

    // 緩?X?置
    int32 MaxCacheSize;
    float CacheExpirationHours;

    // 統?X???
    int32 CacheHits;
    int32 CacheMisses;

    // 清??低?X?緩??    void CleanupLowWeightCache();

    // 計??緩??權??
    float CalculateCacheWeight(const FMingConsequenceCacheEntry& Entry) const;

    // 檢查緩?X?否????
    bool IsCacheExpired(const FMingConsequenceCacheEntry& Entry) const;

    // ??新緩??訪??統??
    void UpdateCacheAccess(const FString& CacheKey};

private:
    // ??否已??始??
    bool bInitialized = false;
};

