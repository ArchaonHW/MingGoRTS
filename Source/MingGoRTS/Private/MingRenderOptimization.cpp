#include "MingRenderOptimization.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/Texture2D.h"
#include "Particles/ParticleSystemComponent.h"
#include "RenderUtils.h"

// ============================================================================
// 靜態網格批次管理器實現
// ============================================================================

void UMingStaticMeshBatchingManager::InitializeBatching()
{
    Batches.Empty();
    NextBatchId = 0;
}

void UMingStaticMeshBatchingManager::RegisterMeshForBatching(UStaticMeshComponent* MeshComponent, int32 BatchGroupId)
{
    if (!MeshComponent) return;
    
    FMeshBatch& Batch = Batches.FindOrAdd(BatchGroupId);
    
    if (!Batch.Mesh)
    {
        Batch.Mesh = MeshComponent->GetStaticMesh();
        Batch.Material = MeshComponent->GetMaterial(0);
    }
    
    Batch.SourceComponents.Add(MeshComponent);
    Batch.InstanceTransforms.Add(MeshComponent->GetComponentTransform().ToMatrixWithScale());
}

void UMingStaticMeshBatchingManager::UnregisterMesh(UStaticMeshComponent* MeshComponent)
{
    if (!MeshComponent) return;
    
    for (auto& Pair : Batches)
    {
        FMeshBatch& Batch = Pair.Value;
        int32 Index = Batch.SourceComponents.Find(MeshComponent);
        if (Index != INDEX_NONE)
        {
            Batch.SourceComponents.RemoveAt(Index);
            if (Index < Batch.InstanceTransforms.Num())
            {
                Batch.InstanceTransforms.RemoveAt(Index);
            }
        }
    }
}

void UMingStaticMeshBatchingManager::UpdateBatchTransforms(int32 BatchGroupId)
{
    FMeshBatch* Batch = Batches.Find(BatchGroupId);
    if (!Batch) return;
    
    Batch->InstanceTransforms.Empty();
    for (UStaticMeshComponent* Component : Batch->SourceComponents)
    {
        if (Component)
        {
            Batch->InstanceTransforms.Add(Component->GetComponentTransform().ToMatrixWithScale());
        }
    }
}

void UMingStaticMeshBatchingManager::ExecuteBatchedRender()
{
    // 實際的批次渲染會在渲染執行緒執行
    // 這裡只是統計
    UE_LOG(LogTemp, Verbose, TEXT("[MeshBatching] Executing %d batches"), Batches.Num());
}

int32 UMingStaticMeshBatchingManager::GetBatchedDrawCallCount() const
{
    return Batches.Num();
}

// ============================================================================
// LOD 管理器實現
// ============================================================================

void UMingHierarchicalLODManager::InitializeLODSystem()
{
    for (int32 i = 0; i < LODComponents.Capacity(); ++i)
    {
        LODComponents[i].Component = nullptr;
        LODComponents[i].CurrentLODLevel = 0;
        LODComponents[i].ForcedLODLevel = -1;
    }
}

void UMingHierarchicalLODManager::RegisterLODComponent(UPrimitiveComponent* Component, const TArray<float>& LODDistances)
{
    if (!Component) return;
    
    // 尋找空槽位
    for (int32 i = 0; i < LODComponents.Capacity(); ++i)
    {
        if (!LODComponents[i].Component)
        {
            FLODComponent& LODComp = LODComponents[i];
            LODComp.Component = Component;
            LODComp.LODDistances = LODDistances;
            LODComp.CurrentLODLevel = 0;
            LODComp.ForcedLODLevel = -1;
            LODComp.LastLocation = Component->GetComponentLocation();
            break;
        }
    }
}

void UMingHierarchicalLODManager::UpdateLODLevels(FVector CameraLocation)
{
    LastCameraLocation = CameraLocation;
    
    for (int32 i = 0; i < LODComponents.Capacity(); ++i)
    {
        FLODComponent& LODComp = LODComponents[i];
        if (!LODComp.Component)
        {
            continue;
        }
        
        if (LODComp.ForcedLODLevel >= 0)
        {
            LODComp.CurrentLODLevel = LODComp.ForcedLODLevel;
            continue;
        }
        
        float Distance = FVector::Dist(CameraLocation, LODComp.Component->GetComponentLocation());
        LODComp.CurrentLODLevel = CalculateLODLevel(LODComp, Distance);
        
        // 應用 LOD
        LODComp.Component->SetForcedLodModel(LODComp.CurrentLODLevel);
    }
}

void UMingHierarchicalLODManager::SetLODOverride(int32 ComponentId, int32 ForcedLODLevel)
{
    if (ComponentId < 0 || ComponentId >= LODComponents.Capacity())
    {
        return;
    }
    
    LODComponents[ComponentId].ForcedLODLevel = FMath::Clamp(ForcedLODLevel, -1, 7);
}

void UMingHierarchicalLODManager::ClearLODOverride(int32 ComponentId)
{
    if (ComponentId < 0 || ComponentId >= LODComponents.Capacity())
    {
        return;
    }
    
    LODComponents[ComponentId].ForcedLODLevel = -1;
}

int32 UMingHierarchicalLODManager::GetActiveLODLevel(int32 ComponentId) const
{
    if (ComponentId < 0 || ComponentId >= LODComponents.Capacity())
    {
        return 0;
    }
    
    return LODComponents[ComponentId].CurrentLODLevel;
}

int32 UMingHierarchicalLODManager::CalculateLODLevel(const FLODComponent& LODComp, float Distance) const
{
    for (int32 i = 0; i < LODComp.LODDistances.Num(); ++i)
    {
        if (Distance < LODComp.LODDistances[i])
        {
            return i;
        }
    }
    
    return LODComp.LODDistances.Num() > 0 ? LODComp.LODDistances.Num() - 1 : 0;
}

// ============================================================================
// 遮罩剔除實現
// ============================================================================

void UMingSoftwareOcclusionCulling::InitializeOcclusionSystem(int32 ResolutionX, int32 ResolutionY)
{
    BufferWidth = ResolutionX;
    BufferHeight = ResolutionY;
    
    DepthBuffer.SetNum(BufferWidth * BufferHeight);
    for (auto& Cell : DepthBuffer)
    {
        Cell.Depth = 255;
        Cell.bIsOccluded = false;
    }
}

void UMingSoftwareOcclusionCulling::RegisterOccludee(UPrimitiveComponent* Component)
{
    if (!Component) return;
    
    // 尋找空槽位
    for (int32 i = 0; i < Occludees.Capacity(); ++i)
    {
        if (!Occludees[i].Component)
        {
            FOccludeeData& Data = Occludees[i];
            Data.Component = Component;
            
            FBoxSphereBounds Bounds = Component->Bounds;
            Data.BoundsCenter = Bounds.Origin;
            Data.BoundsExtent = Bounds.BoxExtent;
            Data.bWasVisible = true;
            break;
        }
    }
}

void UMingSoftwareOcclusionCulling::UnregisterOccludee(UPrimitiveComponent* Component)
{
    if (!Component) return;
    
    for (int32 i = 0; i < Occludees.Capacity(); ++i)
    {
        if (Occludees[i].Component == Component)
        {
            Occludees[i].Component = nullptr;
        }
    }
}

void UMingSoftwareOcclusionCulling::UpdateOcclusionData(FVector CameraLocation, FRotator CameraRotation, float FOV)
{
    // 構建視圖投影矩陣
    FMatrix ViewMatrix = FLookAtMatrix(CameraLocation, CameraLocation + CameraRotation.Vector(), FVector::UpVector);
    
    float AspectRatio = (float)BufferWidth / (float)BufferHeight;
    FMatrix ProjectionMatrix = FPerspectiveMatrix(FMath::DegreesToRadians(FOV), AspectRatio, 10.0f, 10000.0f);
    
    ViewProjectionMatrix = ViewMatrix * ProjectionMatrix;
}

bool UMingSoftwareOcclusionCulling::IsVisible(UPrimitiveComponent* Component) const
{
    for (int32 i = 0; i < Occludees.Capacity(); ++i)
    {
        if (Occludees[i].Component == Component)
        {
            return Occludees[i].bWasVisible;
        }
    }
    
    return true; // 預設為可見
}

void UMingSoftwareOcclusionCulling::PerformOcclusionCulling()
{
    RasterizeOccluders();
    
    // 測試每個遮罩物件的可見性
    for (int32 i = 0; i < Occludees.Capacity(); ++i)
    {
        FOccludeeData& Data = Occludees[i];
        if (!Data.Component)
        {
            continue;
        }
        
        Data.bWasVisible = TestBoundingBoxVisibility(Data.BoundsCenter, Data.BoundsExtent);
        
        // 應用可見性
        Data.Component->SetVisibility(Data.bWasVisible);
    }
}

int32 UMingSoftwareOcclusionCulling::GetVisibleObjectCount() const
{
    int32 Count = 0;
    for (int32 i = 0; i < Occludees.Capacity(); ++i)
    {
        if (Occludees[i].Component && Occludees[i].bWasVisible)
        {
            ++Count;
        }
    }
    return Count;
}

void UMingSoftwareOcclusionCulling::RasterizeOccluders()
{
    // 清空深度緩衝
    for (auto& Cell : DepthBuffer)
    {
        Cell.Depth = 255;
        Cell.bIsOccluded = false;
    }
    
    // 這裡應該光柵化遮罩物體
    // 簡化實現：只填充深度值
}

bool UMingSoftwareOcclusionCulling::TestBoundingBoxVisibility(const FVector& Center, const FVector& Extent) const
{
    // 簡化的 AABB 可見性測試
    FVector2D ScreenMin = WorldToScreen(Center - Extent);
    FVector2D ScreenMax = WorldToScreen(Center + Extent);
    
    // 檢查是否在螢幕範圍內
    if (ScreenMax.X < 0 || ScreenMin.X > BufferWidth ||
        ScreenMax.Y < 0 || ScreenMin.Y > BufferHeight)
    {
        return false;
    }
    
    return true;
}

FVector2D UMingSoftwareOcclusionCulling::WorldToScreen(const FVector& WorldPos) const
{
    FVector4 Projected = ViewProjectionMatrix.TransformFVector4(FVector4(WorldPos, 1.0f));
    
    if (Projected.W > 0.0f)
    {
        float InvW = 1.0f / Projected.W;
        float NormalizedX = (Projected.X * InvW + 1.0f) * 0.5f;
        float NormalizedY = (1.0f - Projected.Y * InvW) * 0.5f;
        
        return FVector2D(NormalizedX * BufferWidth, NormalizedY * BufferHeight);
    }
    
    return FVector2D(-1, -1);
}

// ============================================================================
// 紋理串流管理器實現
// ============================================================================

void UMingTextureStreamingManager::InitializeStreaming()
{
    for (int32 i = 0; i < TextureEntries.Capacity(); ++i)
    {
        TextureEntries[i].Texture = nullptr;
    }
    
    CurrentMemoryUsage = 0;
    MemoryBudget = 512 * 1024 * 1024; // 512MB
    CurrentFrame = 0;
}

void UMingTextureStreamingManager::RegisterTexture(UTexture2D* Texture, float Priority)
{
    if (!Texture) return;
    
    // 尋找空槽位或已存在的紋理
    for (int32 i = 0; i < TextureEntries.Capacity(); ++i)
    {
        if (TextureEntries[i].Texture == nullptr)
        {
            FTextureEntry& Entry = TextureEntries[i];
            Entry.Texture = Texture;
            Entry.Priority = Priority;
            Entry.LastUsedFrame = CurrentFrame;
            Entry.MemorySize = Texture->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal);
            Entry.bIsStreaming = false;
            break;
        }
    }
}

void UMingTextureStreamingManager::UpdateStreaming(FVector CameraLocation, float MaxStreamingDistance)
{
    ++CurrentFrame;
    
    for (int32 i = 0; i < TextureEntries.Capacity(); ++i)
    {
        FTextureEntry& Entry = TextureEntries[i];
        if (!Entry.Texture)
        {
            continue;
        }
        
        // 計算距離優先度
        // 這裡簡化實現
        float DistancePriority = 1.0f;
        
        Entry.Priority *= 0.95f; // 衰減
        Entry.Priority += DistancePriority * 0.05f;
        
        // 根據記憶體預算決定串流
        if (!Entry.bIsStreaming && CurrentMemoryUsage + Entry.MemorySize < MemoryBudget)
        {
            if (Entry.Priority > 0.5f)
            {
                StreamInTexture(Entry);
            }
        }
        else if (Entry.bIsStreaming && Entry.Priority < 0.2f)
        {
            StreamOutTexture(Entry);
        }
    }
}

void UMingTextureStreamingManager::ForceLoadTexture(UTexture2D* Texture)
{
    if (!Texture) return;
    
    Texture->StreamIn(0);
}

void UMingTextureStreamingManager::UnloadUnusedTextures()
{
    for (int32 i = 0; i < TextureEntries.Capacity(); ++i)
    {
        FTextureEntry& Entry = TextureEntries[i];
        if (!Entry.Texture)
        {
            continue;
        }
        
        // 如果長時間未使用，卸載
        if (CurrentFrame - Entry.LastUsedFrame > 300 && Entry.bIsStreaming) // 5秒 (60fps)
        {
            StreamOutTexture(Entry);
        }
    }
}

int32 UMingTextureStreamingManager::GetMemoryBudget() const
{
    return MemoryBudget / (1024 * 1024);
}

int32 UMingTextureStreamingManager::GetCurrentMemoryUsage() const
{
    return CurrentMemoryUsage / (1024 * 1024);
}

void UMingTextureStreamingManager::StreamInTexture(FTextureEntry& Entry)
{
    if (Entry.bIsStreaming) return;
    
    Entry.bIsStreaming = true;
    CurrentMemoryUsage += Entry.MemorySize;
    
    if (Entry.Texture)
    {
        Entry.Texture->StreamIn(0);
    }
}

void UMingTextureStreamingManager::StreamOutTexture(FTextureEntry& Entry)
{
    if (!Entry.bIsStreaming) return;
    
    Entry.bIsStreaming = false;
    CurrentMemoryUsage -= Entry.MemorySize;
    
    if (Entry.Texture)
    {
        // 實際卸載邏輯
    }
}

// ============================================================================
// GPU 效能監控實現
// ============================================================================

bool UMingGPUPerformanceMonitor::bIsMonitoring = false;
TArray<FMingGPUPerformanceMetrics> UMingGPUPerformanceMonitor::MetricsHistory;

void UMingGPUPerformanceMonitor::StartGPUMonitoring()
{
    bIsMonitoring = true;
    MetricsHistory.Empty();
}

void UMingGPUPerformanceMonitor::StopGPUMonitoring()
{
    bIsMonitoring = false;
}

FMingGPUPerformanceMetrics UMingGPUPerformanceMonitor::GetCurrentMetrics()
{
    FMingGPUPerformanceMetrics Metrics;
    
    // 這裡應該從引擎獲取實際的 GPU 統計
    // 簡化實現
    Metrics.GPUTime = 16.67f; // 60fps = 16.67ms
    Metrics.DrawCalls = 1000;
    Metrics.Triangles = 500000;
    Metrics.ShaderSwitches = 50;
    Metrics.TextureSwitches = 30;
    Metrics.RenderTargetSwitches = 5;
    Metrics.MemoryUsedMB = 256;
    
    if (bIsMonitoring)
    {
        MetricsHistory.Add(Metrics);
    }
    
    return Metrics;
}

bool UMingGPUPerformanceMonitor::IsGPUBottleneck()
{
    FMingGPUPerformanceMetrics Metrics = GetCurrentMetrics();
    
    // 如果 GPU 時間超過 16ms (60fps)，可能是 GPU 瓶頸
    return Metrics.GPUTime > 16.0f;
}

void UMingGPUPerformanceMonitor::ApplyGPUOptimizations()
{
    UE_LOG(LogTemp, Warning, TEXT("[GPUPerformance] Applying GPU optimizations"));
    
    // 降低陰影品質
    // 減少粒子效果
    // 降低後處理效果
    // 等等...
}

FString UMingGPUPerformanceMonitor::GetGPUReport()
{
    if (MetricsHistory.Num() == 0)
    {
        return TEXT("No GPU metrics available");
    }
    
    float AvgGPUTime = 0.0f;
    int32 AvgDrawCalls = 0;
    
    for (const auto& Metrics : MetricsHistory)
    {
        AvgGPUTime += Metrics.GPUTime;
        AvgDrawCalls += Metrics.DrawCalls;
    }
    
    AvgGPUTime /= MetricsHistory.Num();
    AvgDrawCalls /= MetricsHistory.Num();
    
    return FString::Printf(TEXT(
        "GPU Performance Report:\n"
        "Samples: %d\n"
        "Avg GPU Time: %.2f ms (%.1f FPS)\n"
        "Avg Draw Calls: %d\n"),
        MetricsHistory.Num(),
        AvgGPUTime,
        1000.0f / AvgGPUTime,
        AvgDrawCalls);
}

// ============================================================================
// 動態解析度實現
// ============================================================================

void UMingDynamicResolution::Initialize(float InMinScale, float InMaxScale)
{
    MinScale = InMinScale;
    MaxScale = InMaxScale;
    CurrentScale = 1.0f;
    bEnabled = true;
    
    FrameTimeHistory.SetNum(60);
    for (float& Time : FrameTimeHistory)
    {
        Time = 16.67f;
    }
    HistoryIndex = 0;
}

void UMingDynamicResolution::UpdateResolution(float TargetFrameTime)
{
    if (!bEnabled) return;
    
    // 記錄當前幀時間
    float CurrentFrameTime = FPlatformTime::Seconds() * 1000.0f;
    FrameTimeHistory[HistoryIndex] = CurrentFrameTime;
    HistoryIndex = (HistoryIndex + 1) % FrameTimeHistory.Num();
    
    float AvgFrameTime = CalculateAverageFrameTime();
    
    // 調整解析度
    if (AvgFrameTime > TargetFrameTime * 1.1f)
    {
        // 效能不足，降低解析度
        CurrentScale = FMath::Max(MinScale, CurrentScale - 0.05f);
    }
    else if (AvgFrameTime < TargetFrameTime * 0.9f)
    {
        // 效能良好，提高解析度
        CurrentScale = FMath::Min(MaxScale, CurrentScale + 0.02f);
    }
    
    // 應用解析度
    IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
    if (CVar)
    {
        CVar->Set(CurrentScale * 100.0f);
    }
}

float UMingDynamicResolution::GetCurrentResolutionScale() const
{
    return CurrentScale;
}

void UMingDynamicResolution::SetResolutionScale(float Scale)
{
    CurrentScale = FMath::Clamp(Scale, MinScale, MaxScale);
}

void UMingDynamicResolution::EnableDynamicResolution(bool bEnable)
{
    bEnabled = bEnable;
}

float UMingDynamicResolution::CalculateAverageFrameTime() const
{
    float Sum = 0.0f;
    for (float Time : FrameTimeHistory)
    {
        Sum += Time;
    }
    return Sum / FrameTimeHistory.Num();
}

// ============================================================================
// 渲染優化管理器實現
// ============================================================================

void UMingRenderOptimizationManager::InitializeRenderOptimization()
{
    BatchingManager = NewObject<UMingStaticMeshBatchingManager>(this);
    BatchingManager->InitializeBatching();
    
    LODManager = NewObject<UMingHierarchicalLODManager>(this);
    LODManager->InitializeLODSystem();
    
    OcclusionCulling = NewObject<UMingSoftwareOcclusionCulling>(this);
    OcclusionCulling->InitializeOcclusionSystem(512, 256);
    
    TextureStreaming = NewObject<UMingTextureStreamingManager>(this);
    TextureStreaming->InitializeStreaming();
    
    DynamicResolution = NewObject<UMingDynamicResolution>(this);
    DynamicResolution->Initialize(0.5f, 1.0f);
    
    ParticleLOD = NewObject<UMingParticleLODSystem>(this);
    
    bOptimizationEnabled = true;
    
    UE_LOG(LogTemp, Log, TEXT("[RenderOptimization] Initialized"));
}

void UMingRenderOptimizationManager::Update(float DeltaTime, FVector CameraLocation, FRotator CameraRotation)
{
    if (!bOptimizationEnabled) return;
    
    UpdateLOD(CameraLocation);
    UpdateCulling(CameraLocation, CameraRotation);
    UpdateStreaming(CameraLocation);
    UpdateParticles(CameraLocation);
    
    // 動態解析度
    if (CurrentConfig.bEnableInstancedRendering)
    {
        DynamicResolution->UpdateResolution(16.67f);
    }
    
    // 批次更新
    if (BatchingManager)
    {
        BatchingManager->ExecuteBatchedRender();
    }
}

void UMingRenderOptimizationManager::ApplyOptimizationConfig(const FMingRenderOptimizationConfig& Config)
{
    CurrentConfig = Config;
    
    // 應用設定
    DynamicResolution->EnableDynamicResolution(Config.bEnableInstancedRendering);
}

void UMingRenderOptimizationManager::EnableOptimization(bool bEnable)
{
    bOptimizationEnabled = bEnable;
}

void UMingRenderOptimizationManager::EmergencyOptimize()
{
    UE_LOG(LogTemp, Warning, TEXT("[RenderOptimization] Emergency optimization"));
    
    // 緊急優化措施
    DynamicResolution->SetResolutionScale(0.5f);
    
    if (LODManager)
    {
        // 強制最低 LOD
    }
    
    if (ParticleLOD)
    {
        ParticleLOD->SetParticleQuality(0);
    }
}

FString UMingRenderOptimizationManager::GetRenderStats() const
{
    FString Stats = TEXT("Render Stats:\n");
    
    if (BatchingManager)
    {
        Stats += FString::Printf(TEXT("Batched Draw Calls: %d\n"), BatchingManager->GetBatchedDrawCallCount());
    }
    
    if (OcclusionCulling)
    {
        Stats += FString::Printf(TEXT("Visible Objects: %d\n"), OcclusionCulling->GetVisibleObjectCount());
    }
    
    if (TextureStreaming)
    {
        Stats += FString::Printf(TEXT("Texture Memory: %d / %d MB\n"), 
            TextureStreaming->GetCurrentMemoryUsage(),
            TextureStreaming->GetMemoryBudget());
    }
    
    if (DynamicResolution)
    {
        Stats += FString::Printf(TEXT("Resolution Scale: %.2f\n"), DynamicResolution->GetCurrentResolutionScale());
    }
    
    return Stats;
}

void UMingRenderOptimizationManager::UpdateCulling(FVector CameraLocation, FRotator CameraRotation)
{
    if (!CurrentConfig.bEnableFrustumCulling || !OcclusionCulling) return;
    
    OcclusionCulling->UpdateOcclusionData(CameraLocation, CameraRotation, 90.0f);
    OcclusionCulling->PerformOcclusionCulling();
}

void UMingRenderOptimizationManager::UpdateStreaming(FVector CameraLocation)
{
    if (!CurrentConfig.bEnableTextureStreaming || !TextureStreaming) return;
    
    TextureStreaming->UpdateStreaming(CameraLocation, 5000.0f);
    TextureStreaming->UnloadUnusedTextures();
}

void UMingRenderOptimizationManager::UpdateLOD(FVector CameraLocation)
{
    if (!CurrentConfig.bEnableLODBatching || !LODManager) return;
    
    LODManager->UpdateLODLevels(CameraLocation);
}

void UMingRenderOptimizationManager::UpdateParticles(FVector CameraLocation)
{
    if (!ParticleLOD) return;
    
    ParticleLOD->UpdateParticleLOD(CameraLocation);
}

void UMingRenderOptimizationManager::AdjustQualityBasedOnPerformance()
{
    // 根據效能自動調整品質
    if (UMingGPUPerformanceMonitor::IsGPUBottleneck())
    {
        CurrentConfig.LODDistanceScale *= 0.9f;
    }
    else
    {
        CurrentConfig.LODDistanceScale = FMath::Min(1.0f, CurrentConfig.LODDistanceScale * 1.05f);
    }
}
