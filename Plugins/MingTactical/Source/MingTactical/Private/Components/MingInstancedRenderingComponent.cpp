#include "Components/MingInstancedRenderingComponent.h"
#include "Units/MingTacticalUnit.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"

UMingInstancedRenderingComponent::UMingInstancedRenderingComponent()
    : UpdateTimer(0.0f)
    , UpdateFrequency(0.1f) // 預設每0.1秒更新一次
    , VisibleInstanceCount(0)
    , CulledInstanceCount(0)
    , LastUpdatedInstanceCount(0)
    , CameraLocation(FVector::ZeroVector)
    , MaxBatchSize(1000)
    , MinBatchSize(50)
    , bEnableLOD(true)
    , bEnableDynamicBatching(true)
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PostUpdateWork;
    
    InitializeLODThresholds();
}

void UMingInstancedRenderingComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // 預分配容量
    RegisteredUnits.Reserve(2000);
    RenderBatches.Reserve(10);
    
    UE_LOG(LogTemp, Log, TEXT("InstancedRenderingComponent initialized with LOD support"));
}

void UMingInstancedRenderingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 清理資源
    RegisteredUnits.Empty();
    UnitToBatchIndex.Empty();
    RenderBatches.Empty();
    
    Super::EndPlay(EndPlayReason);
}

void UMingInstancedRenderingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // 限制更新頻率
    UpdateTimer += DeltaTime;
    if (UpdateTimer < UpdateFrequency)
    {
        return;
    }
    UpdateTimer = 0.0f;
    
    // 更新相機位置
    UpdateCameraLocation();
    
    // 執行LOD更新
    if (bEnableLOD)
    {
        PerformLODUpdate();
    }
    
    // 更新實例可見性
    UpdateInstanceVisibility();
    
    // 動態批次調整
    if (bEnableDynamicBatching)
    {
        MergeSmallBatches();
        SplitLargeBatches();
    }
}

void UMingInstancedRenderingComponent::RegisterUnit(AMingTacticalUnit* Unit)
{
    if (!Unit || RegisteredUnits.Contains(Unit))
    {
        return;
    }
    
    // 添加到註冊列表
    RegisteredUnits.Add(Unit);
    
    // 獲取或創建批次
    int32 BatchIndex = GetOrCreateBatchForUnit(Unit);
    UnitToBatchIndex.Add(Unit, BatchIndex);
    
    // 更新批次
    UpdateBatchInstanceData(BatchIndex);
    
    UE_LOG(LogTemp, Verbose, TEXT("Unit registered to instanced rendering (Total: %d)"), 
        RegisteredUnits.Num());
}

void UMingInstancedRenderingComponent::UnregisterUnit(AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return;
    }
    
    // 從註冊列表移除
    RegisteredUnits.Remove(Unit);
    
    // 從批次中移除
    if (int32* BatchIndexPtr = UnitToBatchIndex.Find(Unit))
    {
        int32 BatchIndex = *BatchIndexPtr;
        if (RenderBatches.IsValidIndex(BatchIndex))
        {
            FRenderBatch& Batch = RenderBatches[BatchIndex];
            
            // 找到並移除該單位的實例數據
            // 這裡簡化處理，實際應該有更複雜的映射
            if (Batch.InstanceCount > 0)
            {
                Batch.InstanceCount--;
            }
        }
        
        UnitToBatchIndex.Remove(Unit);
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Unit unregistered from instanced rendering (Total: %d)"),
        RegisteredUnits.Num());
}

void UMingInstancedRenderingComponent::UpdateUnitTransform(AMingTacticalUnit* Unit, const FTransform& NewTransform)
{
    if (!Unit)
    {
        return;
    }
    
    if (int32* BatchIndexPtr = UnitToBatchIndex.Find(Unit))
    {
        int32 BatchIndex = *BatchIndexPtr;
        if (RenderBatches.IsValidIndex(BatchIndex))
        {
            FRenderBatch& Batch = RenderBatches[BatchIndex];
            
            // 找到該單位的實例索引並更新變換
            // 這裡需要建立單位到實例索引的映射
            FMatrix NewMatrix = NewTransform.ToMatrixWithScale();
            
            // 標記需要更新
            // 實際更新將在下一個更新週期進行
        }
    }
}

void UMingInstancedRenderingComponent::SetUnitVisibility(AMingTacticalUnit* Unit, bool bVisible)
{
    if (!Unit)
    {
        return;
    }
    
    if (int32* BatchIndexPtr = UnitToBatchIndex.Find(Unit))
    {
        int32 BatchIndex = *BatchIndexPtr;
        if (RenderBatches.IsValidIndex(BatchIndex))
        {
            FRenderBatch& Batch = RenderBatches[BatchIndex];
            
            // 找到該單位的實例索引並設置可見性
            // 這裡需要建立單位到實例索引的映射
            
            // 更新統計
            if (bVisible)
            {
                VisibleInstanceCount++;
            }
            else
            {
                CulledInstanceCount++;
            }
        }
    }
}

ELODLevel UMingInstancedRenderingComponent::CalculateLODLevel(AMingTacticalUnit* Unit) const
{
    if (!bEnableLOD || LODDistanceThresholds.Num() == 0)
    {
        return ELODLevel::LOD0;
    }
    
    if (!Unit)
    {
        return ELODLevel::Culled;
    }
    
    // 計算單位到相機的距離
    const FVector UnitLocation = Unit->GetActorLocation();
    const float Distance = FVector::Dist(UnitLocation, CameraLocation);
    
    // 根據距離確定LOD級別
    for (int32 i = 0; i < LODDistanceThresholds.Num(); ++i)
    {
        if (Distance <= LODDistanceThresholds[i])
        {
            return static_cast<ELODLevel>(i);
        }
    }
    
    // 超出所有閾值，完全剔除
    return ELODLevel::Culled;
}

void UMingInstancedRenderingComponent::SetLODDistanceThresholds(const TArray<float>& Thresholds)
{
    LODDistanceThresholds = Thresholds;
    
    // 確保閾值按升序排列
    LODDistanceThresholds.Sort();
    
    UE_LOG(LogTemp, Log, TEXT("LOD thresholds updated: %d levels"), LODDistanceThresholds.Num());
}

void UMingInstancedRenderingComponent::ForceUpdateAllInstances()
{
    // 強制更新所有批次
    for (int32 i = 0; i < RenderBatches.Num(); ++i)
    {
        UpdateBatchInstanceData(i);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Forced update of %d batches"), RenderBatches.Num());
}

void UMingInstancedRenderingComponent::SetUpdateFrequency(float Frequency)
{
    UpdateFrequency = FMath::Max(0.016f, Frequency); // 最小16ms (約60 FPS)
    UE_LOG(LogTemp, Log, TEXT("Update frequency set to %.3f seconds"), UpdateFrequency);
}

void UMingInstancedRenderingComponent::InitializeLODThresholds()
{
    // 預設LOD閾值 (單位: 厘米/UE單位)
    LODDistanceThresholds.Empty();
    LODDistanceThresholds.Add(1000.0f);   // LOD 0: 0-10米
    LODDistanceThresholds.Add(3000.0f);   // LOD 1: 10-30米
    LODDistanceThresholds.Add(6000.0f);   // LOD 2: 30-60米
    LODDistanceThresholds.Add(10000.0f);  // LOD 3: 60-100米
    // LOD 4: 100米+
}

int32 UMingInstancedRenderingComponent::GetOrCreateBatchForUnit(AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return -1;
    }
    
    // 查找現有批次
    for (int32 i = 0; i < RenderBatches.Num(); ++i)
    {
        if (RenderBatches[i].InstanceCount < MaxBatchSize)
        {
            return i;
        }
    }
    
    // 創建新批次
    FRenderBatch NewBatch;
    NewBatch.BatchId = RenderBatches.Num();
    NewBatch.InstanceTransforms.Reserve(MaxBatchSize);
    NewBatch.InstanceColors.Reserve(MaxBatchSize);
    NewBatch.InstanceVisibility.Reserve(MaxBatchSize);
    
    RenderBatches.Add(NewBatch);
    
    UE_LOG(LogTemp, Verbose, TEXT("Created new render batch %d"), NewBatch.BatchId);
    
    return NewBatch.BatchId;
}

void UMingInstancedRenderingComponent::UpdateBatchInstanceData(int32 BatchIndex)
{
    if (!RenderBatches.IsValidIndex(BatchIndex))
    {
        return;
    }
    
    FRenderBatch& Batch = RenderBatches[BatchIndex];
    
    // 這裡將實例數據提交到GPU
    // 實際的GPU實例化渲染將在這裡處理
    
    // 更新統計
    LastUpdatedInstanceCount = Batch.InstanceCount;
}

void UMingInstancedRenderingComponent::PerformLODUpdate()
{
    if (RegisteredUnits.Num() == 0)
    {
        return;
    }
    
    VisibleInstanceCount = 0;
    CulledInstanceCount = 0;
    
    // 為每個單位計算LOD級別
    for (AMingTacticalUnit* Unit : RegisteredUnits)
    {
        if (!Unit)
        {
            continue;
        }
        
        ELODLevel LODLevel = CalculateLODLevel(Unit);
        
        if (LODLevel == ELODLevel::Culled)
        {
            CulledInstanceCount++;
        }
        else
        {
            VisibleInstanceCount++;
        }
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("LOD Update: %d visible, %d culled"),
        VisibleInstanceCount, CulledInstanceCount);
}

void UMingInstancedRenderingComponent::UpdateInstanceVisibility()
{
    // 根據相機視錐剔除和LOD結果更新實例可見性
    // 這裡將更新GPU實例的可見性標記
}

void UMingInstancedRenderingComponent::UpdateCameraLocation()
{
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PlayerController = World->GetFirstPlayerController())
        {
            if (APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager)
            {
                CameraLocation = CameraManager->GetCameraLocation();
            }
        }
    }
}

void UMingInstancedRenderingComponent::MergeSmallBatches()
{
    if (RenderBatches.Num() < 2)
    {
        return;
    }
    
    // 查找小批次並合併
    for (int32 i = RenderBatches.Num() - 1; i >= 0; --i)
    {
        if (RenderBatches[i].InstanceCount < MinBatchSize)
        {
            // 查找可以合併的批次
            for (int32 j = 0; j < i; ++j)
            {
                if (RenderBatches[j].InstanceCount + RenderBatches[i].InstanceCount <= MaxBatchSize)
                {
                    // 執行合併
                    RenderBatches[j].InstanceCount += RenderBatches[i].InstanceCount;
                    
                    // 移除小批次
                    RenderBatches.RemoveAt(i);
                    
                    UE_LOG(LogTemp, Verbose, TEXT("Merged batch %d into batch %d"), i, j);
                    break;
                }
            }
        }
    }
}

void UMingInstancedRenderingComponent::SplitLargeBatches()
{
    for (int32 i = 0; i < RenderBatches.Num(); ++i)
    {
        if (RenderBatches[i].InstanceCount > MaxBatchSize)
        {
            // 創建新批次
            FRenderBatch NewBatch;
            NewBatch.BatchId = RenderBatches.Num();
            
            // 分割實例數據
            int32 HalfCount = RenderBatches[i].InstanceCount / 2;
            NewBatch.InstanceCount = HalfCount;
            RenderBatches[i].InstanceCount -= HalfCount;
            
            // 複製數據 (簡化處理)
            RenderBatches.Add(NewBatch);
            
            UE_LOG(LogTemp, Verbose, TEXT("Split batch %d, created batch %d"), i, NewBatch.BatchId);
        }
    }
}
