#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/MingDecisionConsequenceManager.h"
#include "MingConsequenceCache.generated.h"

// �w??����
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
 * ��??�p??�w??�t��
 * 目標數量�M?X???��ʯ�
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingConsequenceCache : public UObject
{
    GENERATED_BODY()

public:
    // ��摧毀
    UMingConsequenceCache();

    
    void Initialize(int32 MaxCacheSize = 1000, float CacheExpirationHours = 24.0f);

    // 摧毀�w??��??
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    bool GetCachedResult(const FString& CacheKey, FMingConsequenceResult& OutResult);

    // �w??��??
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    void CacheResult(const FString& CacheKey, const FMingConsequenceResult& Result);

    // �M��摧毀�w??
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    void ClearExpiredCache();

    
    void ClearAllCache();

    // 摧毀�w??��??
    UFUNCTION(BlueprintCallable, Category = "Ming|Cache")
    void GetCacheStatistics(int32& TotalEntries, int32& CacheHits, int32& CacheMisses, float& HitRate);

    
    FString GenerateCacheKey(const FMingDecisionContext& Context) const;

protected:
    // �w??�s�x
    TMap<FString, FMingConsequenceCacheEntry> CacheMap;

    // �w?X?�m
    int32 MaxCacheSize;
    float CacheExpirationHours;

    // ��?X???
    int32 CacheHits;
    int32 CacheMisses;

    // �M??�C?X?�w??
    void CleanupLowWeightCache();

    // �p??�w??�v??
    float CalculateCacheWeight(const FMingConsequenceCacheEntry& Entry) const;

    // �ˬd�w?X?�_摧毀
    bool IsCacheExpired(const FMingConsequenceCacheEntry& Entry) const;

    // ??�s�w??�X??��??
    void UpdateCacheAccess(const FString& CacheKey);

private:
    // ??�_�w??�l??
    bool bInitialized = false;
);

