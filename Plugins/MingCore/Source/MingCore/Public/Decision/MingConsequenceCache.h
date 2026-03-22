#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/MingDecisionConsequenceManager.h"
#include "MingConsequenceCache.generated.h"

// 緩存條目
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
 * 後果計算緩存系統
 * 提升重複決策的計算性能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingConsequenceCache : public UObject
{
    GENERATED_BODY()

public:
    // 建構子
    UMingConsequenceCache();

    // 初始化緩存
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    void Initialize(int32 MaxCacheSize = 1000, float CacheExpirationHours = 24.0f);

    // 獲取緩存結果
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    bool GetCachedResult(const FString& CacheKey, FMingConsequenceResult& OutResult);

    // 緩存結果
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    void CacheResult(const FString& CacheKey, const FMingConsequenceResult& Result);

    // 清除過期緩存
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    void ClearExpiredCache();

    // 清除所有緩存
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    void ClearAllCache();

    // 獲取緩存統計
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    void GetCacheStatistics(int32& TotalEntries, int32& CacheHits, int32& CacheMisses, float& HitRate);

    // 生成緩存鍵
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    FString GenerateCacheKey(const FMingDecisionContext& Context) const;

protected:
    // 緩存存儲
    TMap<FString, FMingConsequenceCacheEntry> CacheMap;

    // 緩存配置
    int32 MaxCacheSize;
    float CacheExpirationHours;

    // 統計數據
    int32 CacheHits;
    int32 CacheMisses;

    // 清理低權重緩存
    void CleanupLowWeightCache();

    // 計算緩存權重
    float CalculateCacheWeight(const FMingConsequenceCacheEntry& Entry) const;

    // 檢查緩存是否過期
    bool IsCacheExpired(const FMingConsequenceCacheEntry& Entry) const;

    // 更新緩存訪問統計
    void UpdateCacheAccess(const FString& CacheKey);

private:
    // 是否已初始化
    bool bInitialized = false;
};
