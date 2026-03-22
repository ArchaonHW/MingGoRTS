#include "Optimization/MingSpatialPartition.h"
#include "Units/MingTacticalUnit.h"
#include "DrawDebugHelpers.h"

void FSpatialGridCell::AddUnit(AMingTacticalUnit* Unit)
{
    if (!Unit) return;
    
    // 检查是否已存在
    for (const auto& WeakUnit : Units)
    {
        if (WeakUnit.Get() == Unit)
        {
            return;
        }
    }
    
    Units.Add(Unit);
    LastUpdateTime = FPlatformTime::Seconds();
}

void FSpatialGridCell::RemoveUnit(AMingTacticalUnit* Unit)
{
    if (!Unit) return;
    
    for (int32 i = Units.Num() - 1; i >= 0; --i)
    {
        if (Units[i].Get() == Unit)
        {
            Units.RemoveAt(i);
            LastUpdateTime = FPlatformTime::Seconds();
            return;
        }
    }
}

bool FSpatialGridCell::Contains(AMingTacticalUnit* Unit) const
{
    if (!Unit) return false;
    
    for (const auto& WeakUnit : Units)
    {
        if (WeakUnit.Get() == Unit)
        {
            return true;
        }
    }
    return false;
}

void FSpatialGridCell::Clear()
{
    Units.Empty();
    LastUpdateTime = FPlatformTime::Seconds();
}

void FSpatialQueryResult::AddUnit(AMingTacticalUnit* Unit)
{
    if (Unit && !FoundUnits.Contains(Unit))
    {
        FoundUnits.Add(Unit);
    }
}

UMingSpatialPartition::UMingSpatialPartition()
    : CellSize(1000.0f)
    , bIsInitialized(false)
    , QueryCount(0)
    , TotalQueryTime(0.0f)
{
}

void UMingSpatialPartition::Initialize(const FBox& InWorldBounds, float InCellSize)
{
    if (bIsInitialized)
    {
        Shutdown();
    }

    WorldBounds = InWorldBounds;
    CellSize = FMath::Max(100.0f, InCellSize);

    // 预分配空间
    FVector Extent = WorldBounds.GetExtent();
    int32 EstimatedCellsX = FMath::CeilToInt(Extent.X * 2.0f / CellSize);
    int32 EstimatedCellsY = FMath::CeilToInt(Extent.Y * 2.0f / CellSize);
    int32 EstimatedTotalCells = EstimatedCellsX * EstimatedCellsY;
    
    GridCells.Reserve(FMath::Min(EstimatedTotalCells, 10000));
    UnitToGridMap.Reserve(2000);
    TrackedUnits.Reserve(2000);

    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("SpatialPartition initialized - CellSize: %.1f, Estimated Cells: %d"),
        CellSize, EstimatedTotalCells);
}

void UMingSpatialPartition::Shutdown()
{
    GridCells.Empty();
    UnitToGridMap.Empty();
    TrackedUnits.Empty();
    
    bIsInitialized = false;
    QueryCount = 0;
    TotalQueryTime = 0.0f;
}

void UMingSpatialPartition::RegisterUnit(AMingTacticalUnit* Unit)
{
    if (!bIsInitialized || !Unit) return;

    // 检查是否已注册
    if (UnitToGridMap.Contains(Unit))
    {
        return;
    }

    FVector Location = Unit->GetActorLocation();
    FIntVector GridCoord = WorldToGrid(Location);

    FSpatialGridCell* Cell = GetOrCreateCell(GridCoord);
    if (Cell)
    {
        Cell->AddUnit(Unit);
        UnitToGridMap.Add(Unit, GridCoord);
        TrackedUnits.Add(Unit);
    }
}

void UMingSpatialPartition::UnregisterUnit(AMingTacticalUnit* Unit)
{
    if (!bIsInitialized || !Unit) return;

    FIntVector* GridCoord = UnitToGridMap.Find(Unit);
    if (GridCoord)
    {
        FSpatialGridCell* Cell = GetCell(*GridCoord);
        if (Cell)
        {
            Cell->RemoveUnit(Unit);
        }
        
        UnitToGridMap.Remove(Unit);
    }

    TrackedUnits.RemoveAll([Unit](const TWeakObjectPtr<AMingTacticalUnit>& WeakUnit) {
        return WeakUnit.Get() == Unit;
    });
}

void UMingSpatialPartition::UpdateUnitPosition(AMingTacticalUnit* Unit, const FVector& NewLocation)
{
    if (!bIsInitialized || !Unit) return;

    FIntVector* OldCoord = UnitToGridMap.Find(Unit);
    FIntVector NewCoord = WorldToGrid(NewLocation);

    if (OldCoord && *OldCoord != NewCoord)
    {
        // 从旧单元移除
        FSpatialGridCell* OldCell = GetCell(*OldCoord);
        if (OldCell)
        {
            OldCell->RemoveUnit(Unit);
        }

        // 添加到新单元
        FSpatialGridCell* NewCell = GetOrCreateCell(NewCoord);
        if (NewCell)
        {
            NewCell->AddUnit(Unit);
        }

        UnitToGridMap.Add(Unit, NewCoord);
    }
    else if (!OldCoord)
    {
        // 未注册，执行注册
        RegisterUnit(Unit);
    }
}

FSpatialQueryResult UMingSpatialPartition::QueryUnitsInRadius(const FVector& Center, float Radius, int32 TeamId)
{
    FSpatialQueryResult Result;
    double StartTime = FPlatformTime::Seconds();

    if (!bIsInitialized || Radius <= 0.0f)
    {
        Result.QueryTimeMs = 0.0f;
        return Result;
    }

    TArray<FIntVector> CellsToCheck = GetCellsInRadius(Center, Radius);
    float RadiusSquared = Radius * Radius;

    for (const FIntVector& CellCoord : CellsToCheck)
    {
        FSpatialGridCell* Cell = GetCell(CellCoord);
        if (!Cell) continue;

        Result.CellsChecked++;

        for (const auto& WeakUnit : Cell->Units)
        {
            AMingTacticalUnit* Unit = WeakUnit.Get();
            if (!Unit || !Unit->IsValidLowLevel()) continue;

            // 队伍过滤
            if (TeamId >= 0 && Unit->GetTeamId() != TeamId)
            {
                continue;
            }

            // 距离检查
            float DistSquared = FVector::DistSquared(Unit->GetActorLocation(), Center);
            if (DistSquared <= RadiusSquared)
            {
                Result.AddUnit(Unit);
            }
        }
    }

    Result.QueryTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
    RecordQueryTime(Result.QueryTimeMs);

    return Result;
}

FSpatialQueryResult UMingSpatialPartition::QueryUnitsInBox(const FBox& Box, int32 TeamId)
{
    FSpatialQueryResult Result;
    double StartTime = FPlatformTime::Seconds();

    if (!bIsInitialized || !Box.IsValid)
    {
        Result.QueryTimeMs = 0.0f;
        return Result;
    }

    TArray<FIntVector> CellsToCheck = GetCellsInBox(Box);

    for (const FIntVector& CellCoord : CellsToCheck)
    {
        FSpatialGridCell* Cell = GetCell(CellCoord);
        if (!Cell) continue;

        Result.CellsChecked++;

        for (const auto& WeakUnit : Cell->Units)
        {
            AMingTacticalUnit* Unit = WeakUnit.Get();
            if (!Unit || !Unit->IsValidLowLevel()) continue;

            // 队伍过滤
            if (TeamId >= 0 && Unit->GetTeamId() != TeamId)
            {
                continue;
            }

            // 包围盒检查
            if (Box.IsInsideOrOn(Unit->GetActorLocation()))
            {
                Result.AddUnit(Unit);
            }
        }
    }

    Result.QueryTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
    RecordQueryTime(Result.QueryTimeMs);

    return Result;
}

AMingTacticalUnit* UMingSpatialPartition::GetNearestUnit(const FVector& Location, float MaxRadius, int32 TeamId)
{
    if (!bIsInitialized) return nullptr;

    FSpatialQueryResult QueryResult = QueryUnitsInRadius(Location, MaxRadius, TeamId);
    
    AMingTacticalUnit* Nearest = nullptr;
    float MinDistanceSquared = MaxRadius * MaxRadius;

    for (AMingTacticalUnit* Unit : QueryResult.FoundUnits)
    {
        if (!Unit) continue;

        float DistSquared = FVector::DistSquared(Unit->GetActorLocation(), Location);
        if (DistSquared < MinDistanceSquared)
        {
            MinDistanceSquared = DistSquared;
            Nearest = Unit;
        }
    }

    return Nearest;
}

AMingTacticalUnit* UMingSpatialPartition::GetNearestEnemyUnit(AMingTacticalUnit* SourceUnit, float MaxRadius)
{
    if (!bIsInitialized || !SourceUnit) return nullptr;

    int32 EnemyTeamId = (SourceUnit->GetTeamId() == 0) ? 1 : 0; // 简化处理
    FVector Location = SourceUnit->GetActorLocation();

    return GetNearestUnit(Location, MaxRadius, EnemyTeamId);
}

FSpatialQueryResult UMingSpatialPartition::GetEnemyUnitsInRadius(AMingTacticalUnit* SourceUnit, float Radius)
{
    if (!bIsInitialized || !SourceUnit)
    {
        return FSpatialQueryResult();
    }

    int32 EnemyTeamId = (SourceUnit->GetTeamId() == 0) ? 1 : 0;
    return QueryUnitsInRadius(SourceUnit->GetActorLocation(), Radius, EnemyTeamId);
}

FSpatialQueryResult UMingSpatialPartition::GetFriendlyUnitsInRadius(AMingTacticalUnit* SourceUnit, float Radius)
{
    if (!bIsInitialized || !SourceUnit)
    {
        return FSpatialQueryResult();
    }

    return QueryUnitsInRadius(SourceUnit->GetActorLocation(), Radius, SourceUnit->GetTeamId());
}

bool UMingSpatialPartition::HasLineOfSight(const FVector& Start, const FVector& End, float CheckRadius)
{
    if (!bIsInitialized) return true;

    // 获取线段经过的网格单元
    FVector Direction = End - Start;
    float Distance = Direction.Size();
    Direction.Normalize();

    float StepSize = CellSize * 0.5f;
    int32 Steps = FMath::CeilToInt(Distance / StepSize);

    for (int32 i = 0; i <= Steps; ++i)
    {
        FVector CheckPoint = Start + Direction * (i * StepSize);
        FIntVector GridCoord = WorldToGrid(CheckPoint);
        
        FSpatialGridCell* Cell = GetCell(GridCoord);
        if (!Cell) continue;

        for (const auto& WeakUnit : Cell->Units)
        {
            AMingTacticalUnit* Unit = WeakUnit.Get();
            if (!Unit) continue;

            FVector UnitLocation = Unit->GetActorLocation();
            
            // 检查单位是否在线段附近
            FVector ClosestPoint = FMath::ClosestPointOnLine(Start, End, UnitLocation);
            float DistToLine = FVector::Dist(UnitLocation, ClosestPoint);
            
            if (DistToLine < CheckRadius)
            {
                return false; // 有阻挡
            }
        }
    }

    return true;
}

FSpatialQueryResult UMingSpatialPartition::GetUnitsInLineOfSight(const FVector& Start, const FVector& End, float Width)
{
    FSpatialQueryResult Result;
    
    if (!bIsInitialized) return Result;

    // 创建一个包围盒来查询
    FVector Center = (Start + End) * 0.5f;
    FVector Direction = End - Start;
    float Length = Direction.Size();
    Direction.Normalize();

    FVector Right = FVector::CrossProduct(Direction, FVector::UpVector);
    if (Right.IsNearlyZero())
    {
        Right = FVector::CrossProduct(Direction, FVector::ForwardVector);
    }
    Right.Normalize();

    FVector Up = FVector::CrossProduct(Right, Direction);

    FBox QueryBox(
        Center - Right * Width - Up * Width - Direction * (Length * 0.5f),
        Center + Right * Width + Up * Width + Direction * (Length * 0.5f)
    );

    FSpatialQueryResult BoxResult = QueryUnitsInBox(QueryBox);

    // 过滤出真正在线段"管道"内的单位
    for (AMingTacticalUnit* Unit : BoxResult.FoundUnits)
    {
        if (!Unit) continue;

        FVector UnitLocation = Unit->GetActorLocation();
        FVector ClosestPoint = FMath::ClosestPointOnLine(Start, End, UnitLocation);
        float DistToLine = FVector::Dist(UnitLocation, ClosestPoint);

        if (DistToLine <= Width)
        {
            Result.AddUnit(Unit);
        }
    }

    return Result;
}

TArray<AMingTacticalUnit*> UMingSpatialPartition::GetUnitsInCell(const FIntVector& GridCoord)
{
    TArray<AMingTacticalUnit*> Result;
    
    FSpatialGridCell* Cell = GetCell(GridCoord);
    if (!Cell) return Result;

    for (const auto& WeakUnit : Cell->Units)
    {
        AMingTacticalUnit* Unit = WeakUnit.Get();
        if (Unit)
        {
            Result.Add(Unit);
        }
    }

    return Result;
}

FIntVector UMingSpatialPartition::GetUnitGridCoord(AMingTacticalUnit* Unit) const
{
    if (!Unit) return FIntVector::ZeroValue;

    const FIntVector* Coord = UnitToGridMap.Find(Unit);
    return Coord ? *Coord : FIntVector::ZeroValue;
}

FIntVector UMingSpatialPartition::WorldToGrid(const FVector& WorldLocation) const
{
    FVector RelativePos = WorldLocation - WorldBounds.Min;
    
    return FIntVector(
        FMath::FloorToInt(RelativePos.X / CellSize),
        FMath::FloorToInt(RelativePos.Y / CellSize),
        FMath::FloorToInt(RelativePos.Z / CellSize)
    );
}

FVector UMingSpatialPartition::GridToWorld(const FIntVector& GridCoord) const
{
    return WorldBounds.Min + FVector(
        GridCoord.X * CellSize + CellSize * 0.5f,
        GridCoord.Y * CellSize + CellSize * 0.5f,
        GridCoord.Z * CellSize + CellSize * 0.5f
    );
}

int32 UMingSpatialPartition::GetOccupiedCellCount() const
{
    int32 Count = 0;
    for (const auto& Pair : GridCells)
    {
        if (Pair.Value.GetUnitCount() > 0)
        {
            Count++;
        }
    }
    return Count;
}

float UMingSpatialPartition::GetAverageUnitsPerCell() const
{
    int32 Occupied = GetOccupiedCellCount();
    if (Occupied == 0) return 0.0f;
    
    return static_cast<float>(GetTotalUnitCount()) / Occupied;
}

void UMingSpatialPartition::DrawDebugVisualization(bool bDrawGrid, bool bDrawUnits)
{
    #if ENABLE_DRAW_DEBUG
    if (!bIsInitialized) return;

    if (bDrawGrid)
    {
        // 绘制网格单元
        for (const auto& Pair : GridCells)
        {
            const FSpatialGridCell& Cell = Pair.Value;
            if (Cell.GetUnitCount() == 0) continue;

            FVector CellCenter = GridToWorld(Cell.GridCoord);
            FBox CellBox(
                CellCenter - FVector(CellSize * 0.5f),
                CellCenter + FVector(CellSize * 0.5f)
            );

            // 根据单位数量改变颜色
            int32 UnitCount = Cell.GetUnitCount();
            FColor DrawColor = FColor::Green;
            if (UnitCount > 10) DrawColor = FColor::Yellow;
            if (UnitCount > 20) DrawColor = FColor::Orange;
            if (UnitCount > 50) DrawColor = FColor::Red;

            DrawDebugBox(GetWorld(), CellCenter, CellBox.GetExtent(), DrawColor, false, -1.0f, 0, 5.0f);
            
            // 显示单位数量
            FString CountText = FString::Printf(TEXT("%d"), UnitCount);
            DrawDebugString(GetWorld(), CellCenter, CountText, nullptr, DrawColor, 0.0f, false);
        }
    }

    if (bDrawUnits)
    {
        // 绘制所有追踪的单位
        for (const auto& WeakUnit : TrackedUnits)
        {
            AMingTacticalUnit* Unit = WeakUnit.Get();
            if (!Unit) continue;

            FVector Location = Unit->GetActorLocation();
            DrawDebugSphere(GetWorld(), Location, 100.0f, 8, FColor::Cyan, false, -1.0f, 0, 2.0f);
        }
    }
    #endif
}

bool UMingSpatialPartition::ValidateIntegrity()
{
    bool bValid = true;

    // 验证所有单位都有对应的网格记录
    for (const auto& WeakUnit : TrackedUnits)
    {
        AMingTacticalUnit* Unit = WeakUnit.Get();
        if (!Unit) continue;

        if (!UnitToGridMap.Contains(Unit))
        {
            UE_LOG(LogTemp, Warning, TEXT("Unit %s not found in grid map"), *Unit->GetName());
            bValid = false;
        }
    }

    // 验证网格单元中的单位都被正确追踪
    for (auto& Pair : GridCells)
    {
        FSpatialGridCell& Cell = Pair.Value;
        for (const auto& WeakUnit : Cell.Units)
        {
            AMingTacticalUnit* Unit = WeakUnit.Get();
            if (!Unit) continue;

            if (!TrackedUnits.ContainsByPredicate([Unit](const TWeakObjectPtr<AMingTacticalUnit>& Ptr) {
                return Ptr.Get() == Unit;
            }))
            {
                UE_LOG(LogTemp, Warning, TEXT("Unit %s in cell but not in tracked list"), *Unit->GetName());
                bValid = false;
            }
        }
    }

    return bValid;
}

void UMingSpatialPartition::CleanupInvalidReferences()
{
    // 清理无效的单位引用
    for (auto& Pair : GridCells)
    {
        FSpatialGridCell& Cell = Pair.Value;
        Cell.Units.RemoveAll([](const TWeakObjectPtr<AMingTacticalUnit>& WeakUnit) {
            return !WeakUnit.IsValid();
        });
    }

    // 清理映射中的无效项
    TArray<TWeakObjectPtr<AMingTacticalUnit>> KeysToRemove;
    for (const auto& Pair : UnitToGridMap)
    {
        if (!Pair.Key.IsValid())
        {
            KeysToRemove.Add(Pair.Key);
        }
    }
    for (const auto& Key : KeysToRemove)
    {
        UnitToGridMap.Remove(Key);
    }

    // 清理追踪列表
    TrackedUnits.RemoveAll([](const TWeakObjectPtr<AMingTacticalUnit>& WeakUnit) {
        return !WeakUnit.IsValid();
    });

    // 清理空单元
    CleanupEmptyCells();
}

void UMingSpatialPartition::BatchRegisterUnits(const TArray<AMingTacticalUnit*>& Units)
{
    for (AMingTacticalUnit* Unit : Units)
    {
        RegisterUnit(Unit);
    }
}

void UMingSpatialPartition::BatchUnregisterUnits(const TArray<AMingTacticalUnit*>& Units)
{
    for (AMingTacticalUnit* Unit : Units)
    {
        UnregisterUnit(Unit);
    }
}

void UMingSpatialPartition::BatchUpdatePositions(const TMap<AMingTacticalUnit*, FVector>& PositionUpdates)
{
    for (const auto& Pair : PositionUpdates)
    {
        UpdateUnitPosition(Pair.Key, Pair.Value);
    }
}

FSpatialGridCell* UMingSpatialPartition::GetOrCreateCell(const FIntVector& Coord)
{
    if (!IsValidGridCoord(Coord)) return nullptr;

    FSpatialGridCell* Cell = GridCells.Find(Coord);
    if (!Cell)
    {
        FSpatialGridCell NewCell(Coord);
        FVector CellCenter = GridToWorld(Coord);
        NewCell.Bounds = FBox(
            CellCenter - FVector(CellSize * 0.5f),
            CellCenter + FVector(CellSize * 0.5f)
        );
        GridCells.Add(Coord, NewCell);
        Cell = GridCells.Find(Coord);
    }

    return Cell;
}

FSpatialGridCell* UMingSpatialPartition::GetCell(const FIntVector& Coord)
{
    return GridCells.Find(Coord);
}

TArray<FIntVector> UMingSpatialPartition::GetNeighboringCells(const FIntVector& Center, int32 Radius)
{
    TArray<FIntVector> Result;
    Result.Reserve((Radius * 2 + 1) * (Radius * 2 + 1));

    for (int32 x = -Radius; x <= Radius; ++x)
    {
        for (int32 y = -Radius; y <= Radius; ++y)
        {
            Result.Add(FIntVector(Center.X + x, Center.Y + y, Center.Z));
        }
    }

    return Result;
}

TArray<FIntVector> UMingSpatialPartition::GetCellsInRadius(const FVector& Center, float Radius)
{
    TArray<FIntVector> Result;
    
    FIntVector CenterCoord = WorldToGrid(Center);
    int32 CellRadius = FMath::CeilToInt(Radius / CellSize);

    // 使用包围盒范围
    for (int32 x = -CellRadius; x <= CellRadius; ++x)
    {
        for (int32 y = -CellRadius; y <= CellRadius; ++y)
        {
            Result.Add(FIntVector(CenterCoord.X + x, CenterCoord.Y + y, CenterCoord.Z));
        }
    }

    return Result;
}

TArray<FIntVector> UMingSpatialPartition::GetCellsInBox(const FBox& Box)
{
    TArray<FIntVector> Result;

    FIntVector MinCoord = WorldToGrid(Box.Min);
    FIntVector MaxCoord = WorldToGrid(Box.Max);

    for (int32 x = MinCoord.X; x <= MaxCoord.X; ++x)
    {
        for (int32 y = MinCoord.Y; y <= MaxCoord.Y; ++y)
        {
            for (int32 z = MinCoord.Z; z <= MaxCoord.Z; ++z)
            {
                Result.Add(FIntVector(x, y, z));
            }
        }
    }

    return Result;
}

void UMingSpatialPartition::CleanupEmptyCells()
{
    TArray<FIntVector> KeysToRemove;
    for (const auto& Pair : GridCells)
    {
        if (Pair.Value.GetUnitCount() == 0)
        {
            KeysToRemove.Add(Pair.Key);
        }
    }
    for (const auto& Key : KeysToRemove)
    {
        GridCells.Remove(Key);
    }
}

bool UMingSpatialPartition::IsValidGridCoord(const FIntVector& Coord) const
{
    // 可以添加更复杂的边界检查
    return true;
}

void UMingSpatialPartition::RecordQueryTime(float TimeMs)
{
    QueryCount++;
    TotalQueryTime += TimeMs;
}
