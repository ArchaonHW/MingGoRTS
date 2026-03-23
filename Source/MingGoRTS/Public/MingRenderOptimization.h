#pragma once

#include "CoreMinimal.h"
#include "MingMemoryOptimization.h"
#include "MingRenderOptimization.generated.h"

// ============================================================================
// 渲染優化配置
// ============================================================================

USTRUCT(BlueprintType)
struct FMingRenderOptimizationConfig
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render")
    bool bEnableInstancedRendering;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render")
    bool bEnableOcclusionCulling;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render")
    bool bEnableFrustumCulling;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render")
    bool bEnableLODBatching;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render")
    bool bEnableDrawCallBatching;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render")
    int32 MaxDrawCallsPerFrame;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render")
    int32 MaxTrianglesPerFrame;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render")
    float LODDistanceScale;
    
    FMingRenderOptimizationConfig()
        : bEnableInstancedRendering(true)
        , bEnableOcclusionCulling(true)
        , bEnableFrustumCulling(true)
        , bEnableLODBatching(true)
        , bEnableDrawCallBatching(true)
        , MaxDrawCallsPerFrame(3000)
        , MaxTrianglesPerFrame(1000000)
        , LODDistanceScale(1.0f)
    {}
};

// ============================================================================
// 批次渲染管理器
// ============================================================================

/**
 * 靜態網格批次
 * 合併相同材質和網格的物件以減少 Draw Call
 */
UCLASS()
class MINGGORTS_API UMingStaticMeshBatchingManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Render|Batching")
    void InitializeBatching();
    
    UFUNCTION(BlueprintCallable, Category = "Render|Batching")
    void RegisterMeshForBatching(UStaticMeshComponent* MeshComponent, int32 BatchGroupId);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Batching")
    void UnregisterMesh(UStaticMeshComponent* MeshComponent);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Batching")
    void UpdateBatchTransforms(int32 BatchGroupId);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Batching")
    void ExecuteBatchedRender();
    
    UFUNCTION(BlueprintCallable, Category = "Render|Batching")
    int32 GetBatchedDrawCallCount() const;

private:
    struct FMeshBatch
    {
        UStaticMesh* Mesh;
        UMaterialInterface* Material;
        TArray<FMatrix> InstanceTransforms;
        TArray<UStaticMeshComponent*> SourceComponents;
    };
    
    TMap<int32, FMeshBatch> Batches;
    int32 NextBatchId;
};

// ============================================================================
// LOD 管理系統
// ============================================================================

/**
 * 分層 LOD 管理器
 */
UCLASS()
class MINGGORTS_API UMingHierarchicalLODManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Render|LOD")
    void InitializeLODSystem();
    
    UFUNCTION(BlueprintCallable, Category = "Render|LOD")
    void RegisterLODComponent(UPrimitiveComponent* Component, const TArray<float>& LODDistances);
    
    UFUNCTION(BlueprintCallable, Category = "Render|LOD")
    void UpdateLODLevels(FVector CameraLocation);
    
    UFUNCTION(BlueprintCallable, Category = "Render|LOD")
    void SetLODOverride(int32 ComponentId, int32 ForcedLODLevel);
    
    UFUNCTION(BlueprintCallable, Category = "Render|LOD")
    void ClearLODOverride(int32 ComponentId);
    
    UFUNCTION(BlueprintCallable, Category = "Render|LOD")
    int32 GetActiveLODLevel(int32 ComponentId) const;

private:
    struct FLODComponent
    {
        UPrimitiveComponent* Component;
        TArray<float> LODDistances;
        int32 CurrentLODLevel;
        int32 ForcedLODLevel;
        FVector LastLocation;
    };
    
    TMingFixedArray<FLODComponent, 512> LODComponents;
    FVector LastCameraLocation;
    
    int32 CalculateLODLevel(const FLODComponent& LODComp, float Distance) const;
};

// ============================================================================
// 遮罩剔除系統
// ============================================================================

/**
 * 軟體遮罩剔除
 * 在 CPU 端提前剔除不可見物件
 */
UCLASS()
class MINGGORTS_API UMingSoftwareOcclusionCulling : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Render|Culling")
    void InitializeOcclusionSystem(int32 ResolutionX, int32 ResolutionY);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Culling")
    void RegisterOccludee(UPrimitiveComponent* Component);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Culling")
    void UnregisterOccludee(UPrimitiveComponent* Component);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Culling")
    void UpdateOcclusionData(FVector CameraLocation, FRotator CameraRotation, float FOV);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Culling")
    bool IsVisible(UPrimitiveComponent* Component) const;
    
    UFUNCTION(BlueprintCallable, Category = "Render|Culling")
    void PerformOcclusionCulling();
    
    UFUNCTION(BlueprintCallable, Category = "Render|Culling")
    int32 GetVisibleObjectCount() const;

private:
    struct FOcclusionCell
    {
        uint8 Depth;
        bool bIsOccluded;
    };
    
    TArray<FOcclusionCell> DepthBuffer;
    int32 BufferWidth;
    int32 BufferHeight;
    
    struct FOccludeeData
    {
        UPrimitiveComponent* Component;
        FVector BoundsCenter;
        FVector BoundsExtent;
        bool bWasVisible;
    };
    
    TMingFixedArray<FOccludeeData, 512> Occludees;
    
    FMatrix ViewProjectionMatrix;
    
    void RasterizeOccluders();
    bool TestBoundingBoxVisibility(const FVector& Center, const FVector& Extent) const;
    FVector2D WorldToScreen(const FVector& WorldPos) const;
};

// ============================================================================
// 渲染資源管理
// ============================================================================

/**
 * 紋理串流管理器
 * 動態載入/卸載紋理以控制記憶體使用
 */
UCLASS()
class MINGGORTS_API UMingTextureStreamingManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Render|Streaming")
    void InitializeStreaming();
    
    UFUNCTION(BlueprintCallable, Category = "Render|Streaming")
    void RegisterTexture(UTexture2D* Texture, float Priority);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Streaming")
    void UpdateStreaming(FVector CameraLocation, float MaxStreamingDistance);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Streaming")
    void ForceLoadTexture(UTexture2D* Texture);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Streaming")
    void UnloadUnusedTextures();
    
    UFUNCTION(BlueprintCallable, Category = "Render|Streaming")
    int32 GetMemoryBudget() const;
    
    UFUNCTION(BlueprintCallable, Category = "Render|Streaming")
    int32 GetCurrentMemoryUsage() const;

private:
    struct FTextureEntry
    {
        UTexture2D* Texture;
        float Priority;
        float DistanceToCamera;
        int32 LastUsedFrame;
        int32 MemorySize;
        bool bIsStreaming;
    };
    
    TMingFixedArray<FTextureEntry, 256> TextureEntries;
    int32 CurrentMemoryUsage;
    int32 MemoryBudget;
    int32 CurrentFrame;
    
    void StreamInTexture(FTextureEntry& Entry);
    void StreamOutTexture(FTextureEntry& Entry);
};

// ============================================================================
// GPU 效能監控
// ============================================================================

USTRUCT(BlueprintType)
struct FMingGPUPerformanceMetrics
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    float GPUTime;
    
    UPROPERTY(BlueprintReadOnly)
    int32 DrawCalls;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Triangles;
    
    UPROPERTY(BlueprintReadOnly)
    int32 ShaderSwitches;
    
    UPROPERTY(BlueprintReadOnly)
    int32 TextureSwitches;
    
    UPROPERTY(BlueprintReadOnly)
    int32 RenderTargetSwitches;
    
    UPROPERTY(BlueprintReadOnly)
    int32 MemoryUsedMB;
};

UCLASS()
class MINGGORTS_API UMingGPUPerformanceMonitor : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Render|Performance")
    static void StartGPUMonitoring();
    
    UFUNCTION(BlueprintCallable, Category = "Render|Performance")
    static void StopGPUMonitoring();
    
    UFUNCTION(BlueprintCallable, Category = "Render|Performance")
    static FMingGPUPerformanceMetrics GetCurrentMetrics();
    
    UFUNCTION(BlueprintCallable, Category = "Render|Performance")
    static bool IsGPUBottleneck();
    
    UFUNCTION(BlueprintCallable, Category = "Render|Performance")
    static void ApplyGPUOptimizations();
    
    UFUNCTION(BlueprintCallable, Category = "Render|Performance")
    static FString GetGPUReport();

private:
    static bool bIsMonitoring;
    static TArray<FMingGPUPerformanceMetrics> MetricsHistory;
    
    static void CollectMetrics();
};

// ============================================================================
// 渲染執行緒優化
// ============================================================================

/**
 * 渲染指令批次器
 * 減少渲染執行緒的指令數量
 */
UCLASS()
class MINGGORTS_API UMingRenderCommandBatcher : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Render|Commands")
    void InitializeCommandBatcher();
    
    UFUNCTION(BlueprintCallable, Category = "Render|Commands")
    void QueueRenderCommand(const TFunction<void()>& Command);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Commands")
    void FlushCommands();
    
    UFUNCTION(BlueprintCallable, Category = "Render|Commands")
    void SetMaxCommandsPerFrame(int32 MaxCommands);

private:
    TArray<TFunction<void()>> PendingCommands;
    int32 MaxCommandsPerFrame;
};

// ============================================================================
// 動態解析度縮放
// ============================================================================

UCLASS()
class MINGGORTS_API UMingDynamicResolution : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Render|Resolution")
    void Initialize(float MinScale, float MaxScale);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Resolution")
    void UpdateResolution(float TargetFrameTime);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Resolution")
    float GetCurrentResolutionScale() const;
    
    UFUNCTION(BlueprintCallable, Category = "Render|Resolution")
    void SetResolutionScale(float Scale);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Resolution")
    void EnableDynamicResolution(bool bEnable);

private:
    float CurrentScale;
    float MinScale;
    float MaxScale;
    bool bEnabled;
    
    TArray<float> FrameTimeHistory;
    int32 HistoryIndex;
    
    float CalculateAverageFrameTime() const;
};

// ============================================================================
// 視覺效果優化
// ============================================================================

/**
 * 粒子系統 LOD
 */
UCLASS()
class MINGGORTS_API UMingParticleLODSystem : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Render|Particles")
    void InitializeParticleLOD();
    
    UFUNCTION(BlueprintCallable, Category = "Render|Particles")
    void RegisterParticleSystem(UParticleSystemComponent* ParticleSystem);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Particles")
    void UpdateParticleLOD(FVector CameraLocation);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Particles")
    void SetParticleQuality(int32 QualityLevel);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Particles")
    int32 GetParticleQuality() const;

private:
    struct FParticleLODEntry
    {
        UParticleSystemComponent* ParticleSystem;
        float DistanceToCamera;
        int32 CurrentLODLevel;
    };
    
    TMingFixedArray<FParticleLODEntry, 128> ParticleEntries;
    int32 CurrentQualityLevel;
};

// ============================================================================
// 渲染優化總管
// ============================================================================

UCLASS()
class MINGGORTS_API UMingRenderOptimizationManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Render|Optimization")
    void InitializeRenderOptimization();
    
    UFUNCTION(BlueprintCallable, Category = "Render|Optimization")
    void Update(float DeltaTime, FVector CameraLocation, FRotator CameraRotation);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Optimization")
    void ApplyOptimizationConfig(const FMingRenderOptimizationConfig& Config);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Optimization")
    void EnableOptimization(bool bEnable);
    
    UFUNCTION(BlueprintCallable, Category = "Render|Optimization")
    void EmergencyOptimize();
    
    UFUNCTION(BlueprintCallable, Category = "Render|Optimization")
    FString GetRenderStats() const;

private:
    UPROPERTY()
    UMingStaticMeshBatchingManager* BatchingManager;
    
    UPROPERTY()
    UMingHierarchicalLODManager* LODManager;
    
    UPROPERTY()
    UMingSoftwareOcclusionCulling* OcclusionCulling;
    
    UPROPERTY()
    UMingTextureStreamingManager* TextureStreaming;
    
    UPROPERTY()
    UMingDynamicResolution* DynamicResolution;
    
    UPROPERTY()
    UMingParticleLODSystem* ParticleLOD;
    
    FMingRenderOptimizationConfig CurrentConfig;
    bool bOptimizationEnabled;
    
    void UpdateCulling(FVector CameraLocation, FRotator CameraRotation);
    void UpdateStreaming(FVector CameraLocation);
    void UpdateLOD(FVector CameraLocation);
    void UpdateParticles(FVector CameraLocation);
    void AdjustQualityBasedOnPerformance();
};
