#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSmartResourceManager.generated.h"

UENUM(BlueprintType)
enum class EResourcePriority: uint8 {
    Critical,	// 關鍵資源，必須立即加載
    High,		// 高優先級，優先加載
    Normal,		// 普通優先級
    Low,		// 低優先級，空閒時加載
    Background	// 背景加載
};

UENUM(BlueprintType)
enum class EResourceType: uint8 {
    Texture,
    Mesh,
    Sound,
    Animation,
    Blueprint,
    Particle,
    Material,
    DataTable,
    Custom
};

USTRUCT(BlueprintType)
struct FResourceHandle
{
    GENERATED_BODY()

    UPROPERTY()
    FString ResourceID;

    UPROPERTY()
    EResourceType ResourceType;

    UPROPERTY()
    EResourcePriority Priority;

    UPROPERTY()
    float LoadProgress;

    UPROPERTY()
    bool bIsLoaded;

    UPROPERTY()
    bool bIsCached;

    UPROPERTY()
    int32 ReferenceCount;

    UPROPERTY()
    float LastAccessTime;

    FResourceHandle()
        : ResourceType(EResourceType::Custom)
        , Priority(EResourcePriority::Normal)
        , LoadProgress(0.0f)
        , bIsLoaded(false)
        , bIsCached(false)
        , ReferenceCount(0)
        , LastAccessTime(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FResourcePoolConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxPoolSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 InitialPoolSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CleanupInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MemoryThresholdMB;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableAutoCleanup;

    FResourcePoolConfig()
        : MaxPoolSize(1000)
        , InitialPoolSize(100)
        , CleanupInterval(30.0f)
        , MemoryThresholdMB(512.0f)
        , bEnableAutoCleanup(true)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceLoaded, const FString&, ResourceID, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourcePoolCleanup, int32, FreedResourceCount);

UCLASS(ClassGroup=(MingGoRTS), Blueprintable)
class MINGGORTS_API UMingSmartResourceManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Resource Management")
    void InitializeManager(const FResourcePoolConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Resource Management")
    void ShutdownManager();

    // 智能資源加載
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Resource Management")
    FResourceHandle RequestResource(const FString& ResourcePath, EResourceType Type, EResourcePriority Priority);

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Resource Management")
    void PreloadResources(const TArray<FString>& ResourcePaths, EResourceType Type);

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Resource Management")
    void ReleaseResource(const FString& ResourceID);

    // 資源池管理
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Resource Management")
    void CleanupUnusedResources();

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Resource Management")
    void SetResourcePriority(const FString& ResourceID, EResourcePriority NewPriority);

    // 性能監控
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Resource Management")
    float GetCurrentMemoryUsage() const;

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Resource Management")
    int32 GetActiveResourceCount() const;

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Resource Management")
    float GetAverageLoadTime() const;

    // 緩存管理
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Resource Management")
    void ClearCache();

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Resource Management")
    void SetCacheSizeLimit(float MaxCacheSizeMB);

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "MingGoRTS|Resource Management")
    FOnResourceLoaded OnResourceLoaded;

    UPROPERTY(BlueprintAssignable, Category = "MingGoRTS|Resource Management")
    FOnResourcePoolCleanup OnResourcePoolCleanup;

    // 單例訪問
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Resource Management", meta = (WorldContext = "WorldContextObject"))
    static UMingSmartResourceManager* Get(UObject* WorldContextObject);

protected:
    UPROPERTY()
    FResourcePoolConfig PoolConfig;

    UPROPERTY()
    TMap<FString, FResourceHandle> ResourceRegistry;

    UPROPERTY()
    TMap<FString, UObject*> LoadedResources;

    UPROPERTY()
    TArray<FString> ResourceLoadQueue;

    UPROPERTY()
    float CurrentMemoryUsageMB;

    UPROPERTY()
    float TotalLoadTime;

    UPROPERTY()
    int32 TotalLoadCount;

    UPROPERTY()
    FTimerHandle CleanupTimerHandle;

    void ProcessResourceQueue();
    void PerformResourceCleanup();
    void UpdateResourceMetrics(const FResourceHandle& Handle, float LoadTime);
    bool ShouldCleanupResource(const FResourceHandle& Handle) const;

    virtual void BeginDestroy() override;
};
