#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAICoreContentManager.generated.h"

/**
 * AI Core Content Manager
 * Manages AI-generated content assets and resources
 */
UCLASS(BlueprintType, Blueprintable)
class MINGAI_API UMingAICoreContentManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAICoreContentManager();

    /**
     * Initialize content manager
     */
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void InitializeContentManager();

    /**
     * Load content assets
     */
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    bool LoadContentAssets(const TArray<FString>& AssetPaths);

    /**
     * Unload content assets
     */
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    bool UnloadContentAssets(const TArray<FString>& AssetPaths);

    /**
     * Get content asset
     */
    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    UObject* GetContentAsset(const FString& AssetPath);

    /**
     * Cache content asset
     */
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    bool CacheContentAsset(const FString& AssetPath, UObject* Asset);

    /**
     * Clear content cache
     */
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void ClearContentCache();

    /**
     * Get cache size
     */
    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    int32 GetCacheSize() const;

    /**
     * Optimize cache
     */
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void OptimizeCache();

protected:
    // Content asset cache
    UPROPERTY()
    TMap<FString, UObject*> ContentCache;

    // Maximum cache size
    UPROPERTY()
    int32 MaxCacheSize;

    // Cache access order
    UPROPERTY()
    TArray<FString> CacheAccessOrder;

    // Load asset from disk
    UObject* LoadAssetFromDisk(const FString& AssetPath);

    // Unload asset from memory
    bool UnloadAssetFromMemory(const FString& AssetPath);

    // Update cache access order
    void UpdateCacheAccessOrder(const FString& AssetPath);

    // Remove least recently used assets
    void RemoveLeastRecentlyUsedAssets(int32 Count);

    // Validate asset path
    bool ValidateAssetPath(const FString& AssetPath) const;

    // Get asset memory usage
    int32 GetAssetMemoryUsage(UObject* Asset) const;

    // Log cache statistics
    void LogCacheStatistics();
};
