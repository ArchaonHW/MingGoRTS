#include "Optimization/MingTacticalOptimizationManager.h"
#include "Optimization/MingSpatialPartition.h"
#include "Optimization/MingHierarchicalTickSystem.h"
#include "Optimization/MingObjectPoolSystem.h"
#include "Units/MingTacticalUnit.h"
#include "MingCombatAI.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

void FPerformanceStats::Update(float DeltaTime)
{
    // 更新 FPS
    if (DeltaTime > 0.0f)
    {
        CurrentFPS = 1.0f / DeltaTime;
    }

    // 更新帧时间
    FrameTimeMs = DeltaTime * 1000.0f;

    // 内存使用
    MemoryUsageMB = static_cast<float>(FPlatformMemory::GetUsedVirtualMemory()) / (1024.0f * 1024.0f);
}

UMingTacticalOptimizationManager::UMingTacticalOptimizationManager()
    : SpatialPartition(nullptr)
    , TickSystem(nullptr)
    , ObjectPool(nullptr)
    , bIsInitialized(false)
    , bAutoOptimizationEnabled(true)
    , bSpatialPartitionEnabled(true)
    , bHierarchicalTickEnabled(true)
    , bObjectPoolEnabled(true)
    , TargetFPS(60.0f)
    , CriticalFPSThreshold(30.0f)
    , OptimizationLevel(2)
    , LastOptimizationAdjustTime(0.0f)
    , OptimizationAdjustCooldown(2.0f)
    , FPSHistoryIndex(0)
{
    FPSHistory.Init(60.0f, 60); // 60帧历史
}

void UMingTacticalOptimizationManager::Initialize(const FBox& WorldBounds, int32 ExpectedUnitCount)
{
    if (bIsInitialized) return;

    InitializeSubsystems(WorldBounds, ExpectedUnitCount);

    bIsInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("TacticalOptimizationManager initialized - Expected units: %d"), ExpectedUnitCount);
}

void UMingTacticalOptimizationManager::Shutdown()
{
    if (!bIsInitialized) return;

    ShutdownSubsystems();

    bIsInitialized = false;
}

void UMingTacticalOptimizationManager::Tick(float DeltaTime)
{
    if (!bIsInitialized) return;

    // 更新性能统计
    UpdatePerformanceStats(DeltaTime);

    // Tick 子系统
    if (TickSystem)
    {
        TickSystem->Tick(DeltaTime);
    }

    if (ObjectPool)
    {
        ObjectPool->Tick(DeltaTime);
    }

    // 自动优化
    if (bAutoOptimizationEnabled)
    {
        PerformAutoOptimization(DeltaTime);
    }
}

void UMingTacticalOptimizationManager::RegisterUnit(AMingTacticalUnit* Unit)
{
    if (!bIsInitialized || !Unit) return;

    // 注册到空间分区
    if (bSpatialPartitionEnabled && SpatialPartition)
    {
        SpatialPartition->RegisterUnit(Unit);
    }

    // 注册到分层 Tick
    if (bHierarchicalTickEnabled && TickSystem)
    {
        // 根据距离相机的距离决定初始层级
        ETickLevel InitialLevel = ETickLevel::Normal;
        
        // 获取玩家相机位置（简化实现）
        FVector UnitLocation = Unit->GetActorLocation();
        FVector CameraLocation = FVector::ZeroVector; // 应该从玩家控制器获取
        float Distance = FVector::Dist(UnitLocation, CameraLocation);
        
        TickSystem->AutoAdjustLevelByDistance(Unit, Distance);
        TickSystem->RegisterUnit(Unit, InitialLevel);
    }

    CurrentStats.TotalUnitCount++;
}

void UMingTacticalOptimizationManager::UnregisterUnit(AMingTacticalUnit* Unit)
{
    if (!Unit) return;

    if (SpatialPartition)
    {
        SpatialPartition->UnregisterUnit(Unit);
    }

    if (TickSystem)
    {
        TickSystem->UnregisterUnit(Unit);
    }

    CurrentStats.TotalUnitCount--;
}

void UMingTacticalOptimizationManager::BatchRegisterUnits(const TArray<AMingTacticalUnit*>& Units)
{
    if (!bIsInitialized) return;

    // 批量注册到空间分区
    if (bSpatialPartitionEnabled && SpatialPartition)
    {
        SpatialPartition->BatchRegisterUnits(Units);
    }

    // 逐个注册到 Tick 系统（因为每个单位可能有不同的层级）
    if (bHierarchicalTickEnabled && TickSystem)
    {
        for (AMingTacticalUnit* Unit : Units)
        {
            if (Unit)
            {
                TickSystem->RegisterUnit(Unit, ETickLevel::Normal);
            }
        }
    }

    CurrentStats.TotalUnitCount += Units.Num();
}

void UMingTacticalOptimizationManager::BatchUnregisterUnits(const TArray<AMingTacticalUnit*>& Units)
{
    if (bSpatialPartitionEnabled && SpatialPartition)
    {
        SpatialPartition->BatchUnregisterUnits(Units);
    }

    if (TickSystem)
    {
        for (AMingTacticalUnit* Unit : Units)
        {
            if (Unit)
            {
                TickSystem->UnregisterUnit(Unit);
            }
        }
    }

    CurrentStats.TotalUnitCount -= Units.Num();
}

void UMingTacticalOptimizationManager::UpdateUnitPosition(AMingTacticalUnit* Unit, const FVector& NewLocation)
{
    if (!Unit) return;

    if (bSpatialPartitionEnabled && SpatialPartition)
    {
        SpatialPartition->UpdateUnitPosition(Unit, NewLocation);
    }
}

void UMingTacticalOptimizationManager::SetUnitTickLevel(AMingTacticalUnit* Unit, ETickLevel NewLevel)
{
    if (!bHierarchicalTickEnabled || !TickSystem || !Unit) return;

    TickSystem->SetUnitTickLevel(Unit, NewLevel);
}

void UMingTacticalOptimizationManager::PromoteUnitToCritical(AMingTacticalUnit* Unit, float DurationSeconds)
{
    if (!bHierarchicalTickEnabled || !TickSystem || !Unit) return;

    TickSystem->PromoteToCritical(Unit, DurationSeconds);
}

void UMingTacticalOptimizationManager::RegisterAI(AMingCombatAI* AI)
{
    if (!bIsInitialized || !AI) return;

    if (bHierarchicalTickEnabled && TickSystem)
    {
        TickSystem->RegisterAI(AI, ETickLevel::Normal);
    }
}

void UMingTacticalOptimizationManager::UnregisterAI(AMingCombatAI* AI)
{
    if (!AI) return;

    if (TickSystem)
    {
        TickSystem->UnregisterAI(AI);
    }
}

bool UMingTacticalOptimizationManager::IsPerformanceCritical() const
{
    return CurrentStats.CurrentFPS < CriticalFPSThreshold;
}

void UMingTacticalOptimizationManager::SetAutoOptimizationEnabled(bool bEnabled)
{
    bAutoOptimizationEnabled = bEnabled;
}

void UMingTacticalOptimizationManager::AutoAdjustOptimizationLevel()
{
    float CurrentFPS = CurrentStats.CurrentFPS;

    if (CurrentFPS < CriticalFPSThreshold)
    {
        // 性能危急，提升优化级别
        SetOptimizationLevel(FMath::Min(4, OptimizationLevel + 1));
    }
    else if (CurrentFPS > TargetFPS * 1.5f)
    {
        // 性能良好，可以降低优化级别以获得更好的质量
        SetOptimizationLevel(FMath::Max(0, OptimizationLevel - 1));
    }
}

void UMingTacticalOptimizationManager::SetTargetFPS(float InTargetFPS)
{
    TargetFPS = FMath::Clamp(InTargetFPS, 30.0f, 240.0f);
}

void UMingTacticalOptimizationManager::SetPerformanceCriticalThreshold(float FPS)
{
    CriticalFPSThreshold = FMath::Clamp(FPS, 15.0f, 60.0f);
}

void UMingTacticalOptimizationManager::SetSpatialPartitionEnabled(bool bEnabled)
{
    bSpatialPartitionEnabled = bEnabled;
}

void UMingTacticalOptimizationManager::SetHierarchicalTickEnabled(bool bEnabled)
{
    bHierarchicalTickEnabled = bEnabled;
}

void UMingTacticalOptimizationManager::SetObjectPoolEnabled(bool bEnabled)
{
    bObjectPoolEnabled = bEnabled;
}

void UMingTacticalOptimizationManager::PrintDebugInfo()
{
    UE_LOG(LogTemp, Log, TEXT("=== Tactical Optimization Manager ==="));
    UE_LOG(LogTemp, Log, TEXT("FPS: %.1f (Target: %.1f, Critical: %.1f)"),
        CurrentStats.CurrentFPS, TargetFPS, CriticalFPSThreshold);
    UE_LOG(LogTemp, Log, TEXT("Frame Time: %.2f ms"), CurrentStats.FrameTimeMs);
    UE_LOG(LogTemp, Log, TEXT("Active Units: %d / %d"),
        CurrentStats.ActiveUnitCount, CurrentStats.TotalUnitCount);
    UE_LOG(LogTemp, Log, TEXT("Memory: %.1f MB"), CurrentStats.MemoryUsageMB);
    UE_LOG(LogTemp, Log, TEXT("Optimization Level: %d/4"), OptimizationLevel);
    UE_LOG(LogTemp, Log, TEXT("Auto Optimization: %s"), bAutoOptimizationEnabled ? TEXT("ON") : TEXT("OFF"));
    UE_LOG(LogTemp, Log, TEXT("Systems: Spatial=%s, Tick=%s, Pool=%s"),
        bSpatialPartitionEnabled ? TEXT("ON") : TEXT("OFF"),
        bHierarchicalTickEnabled ? TEXT("ON") : TEXT("OFF"),
        bObjectPoolEnabled ? TEXT("ON") : TEXT("OFF"));

    // 子系统统计
    if (SpatialPartition)
    {
        UE_LOG(LogTemp, Log, TEXT("--- Spatial Partition ---"));
        UE_LOG(LogTemp, Log, TEXT("Cells: %d occupied / %d total"),
            SpatialPartition->GetOccupiedCellCount(),
            SpatialPartition->GetTotalCellCount());
        UE_LOG(LogTemp, Log, TEXT("Avg Units per Cell: %.1f"),
            SpatialPartition->GetAverageUnitsPerCell());
    }

    if (TickSystem)
    {
        UE_LOG(LogTemp, Log, TEXT("--- Hierarchical Tick ---"));
        UE_LOG(LogTemp, Log, TEXT("Registered Units: %d"), TickSystem->GetTotalRegisteredUnits());
        UE_LOG(LogTemp, Log, TEXT("Ticked Last Frame: %d"), TickSystem->GetTickedUnitsLastFrame());
        UE_LOG(LogTemp, Log, TEXT("Avg Tick Time: %.3f ms"), TickSystem->GetAverageTickTimeMs());
    }

    if (ObjectPool)
    {
        UE_LOG(LogTemp, Log, TEXT("--- Object Pool ---"));
        UE_LOG(LogTemp, Log, TEXT("Unit Pool: %d/%d (%.1f%%)"),
            ObjectPool->GetInUseUnitCount(),
            ObjectPool->GetTotalUnitPoolSize(),
            ObjectPool->GetUnitPoolUtilization() * 100.0f);
        UE_LOG(LogTemp, Log, TEXT("Reuse Rate: %.1f%%"), ObjectPool->GetAverageReuseRate() * 100.0f);
    }

    UE_LOG(LogTemp, Log, TEXT("===================================="));
}

FString UMingTacticalOptimizationManager::GetDebugString() const
{
    return FString::Printf(TEXT("FPS: %.1f | Units: %d | Mem: %.0fMB | Level: %d"),
        CurrentStats.CurrentFPS,
        CurrentStats.TotalUnitCount,
        CurrentStats.MemoryUsageMB,
        OptimizationLevel);
}

void UMingTacticalOptimizationManager::DrawDebugVisualization(bool bDrawSpatialGrid, bool bDrawTickLevels)
{
    if (bDrawSpatialGrid && SpatialPartition)
    {
        SpatialPartition->DrawDebugVisualization(true, false);
    }

    // Tick 层级可视化可以在这里添加
}

FString UMingTacticalOptimizationManager::GenerateOptimizationReport() const
{
    FString Report;

    Report += FString::Printf(TEXT("=== Epic 2 Performance Report ===\n"));
    Report += FString::Printf(TEXT("Generated at: %s\n"), *FDateTime::Now().ToString());
    Report += FString::Printf(TEXT("\n"));
    
    // 性能统计
    Report += FString::Printf(TEXT("--- Performance Stats ---\n"));
    Report += FString::Printf(TEXT("Current FPS: %.2f\n"), CurrentStats.CurrentFPS);
    Report += FString::Printf(TEXT("Average FPS: %.2f\n"), CurrentStats.AverageFPS);
    Report += FString::Printf(TEXT("Min FPS: %.2f\n"), CurrentStats.MinFPS);
    Report += FString::Printf(TEXT("Max FPS: %.2f\n"), CurrentStats.MaxFPS);
    Report += FString::Printf(TEXT("Frame Time: %.3f ms\n"), CurrentStats.FrameTimeMs);
    Report += FString::Printf(TEXT("Memory Usage: %.2f MB\n"), CurrentStats.MemoryUsageMB);
    Report += FString::Printf(TEXT("\n"));

    // 单位统计
    Report += FString::Printf(TEXT("--- Unit Stats ---\n"));
    Report += FString::Printf(TEXT("Total Units: %d\n"), CurrentStats.TotalUnitCount);
    Report += FString::Printf(TEXT("Active Units: %d\n"), CurrentStats.ActiveUnitCount);
    Report += FString::Printf(TEXT("\n"));

    // 子系统统计
    if (SpatialPartition)
    {
        Report += FString::Printf(TEXT("--- Spatial Partition ---\n"));
        Report += FString::Printf(TEXT("Total Cells: %d\n"), SpatialPartition->GetTotalCellCount());
        Report += FString::Printf(TEXT("Occupied Cells: %d\n"), SpatialPartition->GetOccupiedCellCount());
        Report += FString::Printf(TEXT("Average Units per Cell: %.2f\n"), SpatialPartition->GetAverageUnitsPerCell());
        Report += FString::Printf(TEXT("\n"));
    }

    if (TickSystem)
    {
        Report += FString::Printf(TEXT("--- Hierarchical Tick ---\n"));
        Report += FString::Printf(TEXT("Registered Units: %d\n"), TickSystem->GetTotalRegisteredUnits());
        Report += FString::Printf(TEXT("Ticked Last Frame: %d\n"), TickSystem->GetTickedUnitsLastFrame());
        Report += FString::Printf(TEXT("Average Tick Time: %.3f ms\n"), TickSystem->GetAverageTickTimeMs());
        Report += FString::Printf(TEXT("\n"));

        // 各层级分布
        Report += FString::Printf(TEXT("Units by Level:\n"));
        for (int32 i = 0; i <= static_cast<int32>(ETickLevel::Paused); ++i)
        {
            ETickLevel Level = static_cast<ETickLevel>(i);
            int32 Count = TickSystem->GetUnitCountByLevel(Level);
            if (Count > 0)
            {
                Report += FString::Printf(TEXT("  %s: %d\n"), *UEnum::GetValueAsString(Level), Count);
            }
        }
        Report += FString::Printf(TEXT("\n"));
    }

    if (ObjectPool)
    {
        Report += FString::Printf(TEXT("--- Object Pool ---\n"));
        Report += FString::Printf(TEXT("Unit Pool Size: %d\n"), ObjectPool->GetTotalUnitPoolSize());
        Report += FString::Printf(TEXT("Available Units: %d\n"), ObjectPool->GetAvailableUnitCount());
        Report += FString::Printf(TEXT("In Use Units: %d\n"), ObjectPool->GetInUseUnitCount());
        Report += FString::Printf(TEXT("Utilization: %.1f%%\n"), ObjectPool->GetUnitPoolUtilization() * 100.0f);
        Report += FString::Printf(TEXT("Total Created: %d\n"), ObjectPool->GetTotalCreatedCount());
        Report += FString::Printf(TEXT("Total Reused: %d\n"), ObjectPool->GetTotalReusedCount());
        Report += FString::Printf(TEXT("Reuse Rate: %.1f%%\n"), ObjectPool->GetAverageReuseRate() * 100.0f);
        Report += FString::Printf(TEXT("\n"));
    }

    // 优化建议
    Report += FString::Printf(TEXT("--- Optimization Suggestions ---\n"));
    if (CurrentStats.CurrentFPS < CriticalFPSThreshold)
    {
        Report += FString::Printf(TEXT("! PERFORMANCE CRITICAL - Consider:\n"));
        Report += FString::Printf(TEXT("  - Reducing view distance\n"));
        Report += FString::Printf(TEXT("  - Lowering unit detail\n"));
        Report += FString::Printf(TEXT("  - Increasing tick intervals\n"));
        Report += FString::Printf(TEXT("  - Reducing max units per frame\n"));
    }
    else if (CurrentStats.CurrentFPS < TargetFPS)
    {
        Report += FString::Printf(TEXT("! Below Target FPS - Consider:\n"));
        Report += FString::Printf(TEXT("  - Adjusting tick levels\n"));
        Report += FString::Printf(TEXT("  - Optimizing spatial query frequency\n"));
    }
    else
    {
        Report += FString::Printf(TEXT("Performance OK\n"));
    }

    Report += FString::Printf(TEXT("\n=== End of Report ===\n"));

    return Report;
}

void UMingTacticalOptimizationManager::InitializeSubsystems(const FBox& WorldBounds, int32 ExpectedUnitCount)
{
    // 创建并初始化空间分区
    if (bSpatialPartitionEnabled)
    {
        SpatialPartition = NewObject<UMingSpatialPartition>(this);
        float CellSize = FMath::Clamp(WorldBounds.GetExtent().Size() / 50.0f, 500.0f, 2000.0f);
        SpatialPartition->Initialize(WorldBounds, CellSize);
    }

    // 创建并初始化分层 Tick 系统
    if (bHierarchicalTickEnabled)
    {
        TickSystem = NewObject<UMingHierarchicalTickSystem>(this);
        TickSystem->Initialize();
    }

    // 创建并初始化对象池
    if (bObjectPoolEnabled)
    {
        ObjectPool = NewObject<UMingObjectPoolSystem>(this);
        int32 InitialPoolSize = FMath::Min(ExpectedUnitCount / 2, 500);
        ObjectPool->Initialize(InitialPoolSize, ExpectedUnitCount / 50, ExpectedUnitCount * 2);
        ObjectPool->PrepopulatePools();
    }
}

void UMingTacticalOptimizationManager::ShutdownSubsystems()
{
    if (ObjectPool)
    {
        ObjectPool->Shutdown();
        ObjectPool = nullptr;
    }

    if (TickSystem)
    {
        TickSystem->Shutdown();
        TickSystem = nullptr;
    }

    if (SpatialPartition)
    {
        SpatialPartition->Shutdown();
        SpatialPartition = nullptr;
    }
}

void UMingTacticalOptimizationManager::UpdatePerformanceStats(float DeltaTime)
{
    CurrentStats.Update(DeltaTime);

    // 更新 FPS 历史
    FPSHistory[FPSHistoryIndex] = CurrentStats.CurrentFPS;
    FPSHistoryIndex = (FPSHistoryIndex + 1) % FPSHistory.Num();

    // 计算平均、最小、最大 FPS
    float Sum = 0.0f;
    float Min = 9999.0f;
    float Max = 0.0f;
    int32 ValidCount = 0;

    for (float FPS : FPSHistory)
    {
        if (FPS > 0.0f)
        {
            Sum += FPS;
            Min = FMath::Min(Min, FPS);
            Max = FMath::Max(Max, FPS);
            ValidCount++;
        }
    }

    if (ValidCount > 0)
    {
        CurrentStats.AverageFPS = Sum / ValidCount;
        CurrentStats.MinFPS = Min;
        CurrentStats.MaxFPS = Max;
    }

    // 更新活跃单位数
    if (TickSystem)
    {
        CurrentStats.ActiveUnitCount = TickSystem->GetTickedUnitsLastFrame();
    }

    // 获取线程时间（如果有 GEngine）
    if (GEngine)
    {
        CurrentStats.GameThreadTime = GEngine->GetCurrentGameTime();
    }
}

void UMingTacticalOptimizationManager::PerformAutoOptimization(float DeltaTime)
{
    float CurrentTime = FPlatformTime::Seconds();
    
    // 检查冷却时间
    if (CurrentTime - LastOptimizationAdjustTime < OptimizationAdjustCooldown)
    {
        return;
    }

    float CurrentFPS = CurrentStats.CurrentFPS;

    // 根据 FPS 调整优化级别
    if (CurrentFPS < CriticalFPSThreshold)
    {
        // 性能危急
        if (OptimizationLevel < 4)
        {
            SetOptimizationLevel(OptimizationLevel + 1);
            LastOptimizationAdjustTime = CurrentTime;
        }
    }
    else if (CurrentFPS < TargetFPS * 0.8f)
    {
        // 性能不佳
        if (OptimizationLevel < 3)
        {
            SetOptimizationLevel(OptimizationLevel + 1);
            LastOptimizationAdjustTime = CurrentTime;
        }
    }
    else if (CurrentFPS > TargetFPS * 1.2f && CurrentStats.AverageFPS > TargetFPS)
    {
        // 性能良好，可以降低优化级别
        if (OptimizationLevel > 0)
        {
            SetOptimizationLevel(OptimizationLevel - 1);
            LastOptimizationAdjustTime = CurrentTime;
        }
    }
}

void UMingTacticalOptimizationManager::SetOptimizationLevel(int32 NewLevel)
{
    NewLevel = FMath::Clamp(NewLevel, 0, 4);
    
    if (OptimizationLevel == NewLevel) return;
    
    OptimizationLevel = NewLevel;
    
    UE_LOG(LogTemp, Log, TEXT("Optimization level changed to %d"), OptimizationLevel);

    // 应用优化级别的具体设置
    switch (OptimizationLevel)
    {
    case 0: // 最低优化（最高质量）
        if (TickSystem)
        {
            TickSystem->SetAutoBalancingEnabled(false);
        }
        break;
        
    case 1: // 低优化
        if (TickSystem)
        {
            TickSystem->SetAutoBalancingEnabled(true);
        }
        break;
        
    case 2: // 中等优化（默认）
        if (TickSystem)
        {
            TickSystem->SetAutoBalancingEnabled(true);
        }
        break;
        
    case 3: // 高优化
        if (TickSystem)
        {
            TickSystem->SetAutoBalancingEnabled(true);
        }
        break;
        
    case 4: // 最高优化（最低质量）
        if (TickSystem)
        {
            TickSystem->SetAutoBalancingEnabled(true);
        }
        break;
    }
}

void UMingTacticalOptimizationManager::AdjustPoolSizeForUnitCount(int32 UnitCount)
{
    if (!ObjectPool) return;

    int32 CurrentPoolSize = ObjectPool->GetTotalUnitPoolSize();
    int32 TargetPoolSize = FMath::Min(UnitCount + 100, MaxPoolSize);

    if (TargetPoolSize > CurrentPoolSize)
    {
        int32 ExpandAmount = TargetPoolSize - CurrentPoolSize;
        ObjectPool->ExpandUnitPool(ExpandAmount);
    }
}
