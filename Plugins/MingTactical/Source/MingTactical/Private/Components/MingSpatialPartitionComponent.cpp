#include "Components/MingSpatialPartitionComponent.h"
#include "Units/MingTacticalUnit.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"

UMingSpatialPartitionComponent::UMingSpatialPartitionComponent()
    : CellSize(500.0f)
    , UpdateRadius(5000.0f)
    , CameraLocation(FVector::ZeroVector)
    , CameraRotation(FRotator::ZeroRotator)
    , UpdateTimer(0.0f)
    , UpdateFrequency(0.033f) // 約30 FPS更新頻率
    , MaxCellsPerFrame(10)
    , CurrentUpdateIndex(0)
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void UMingSpatialPartitionComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // 預分配容量
    GridCells.Reserve(100);
    RegisteredUnits.Reserve(2000);
    
    UE_LOG(LogTemp, Log, TEXT("SpatialPartitionComponent initialized with cell size: %.1f"), CellSize);
}

void UMingSpatialPartitionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 清理資源
    GridCells.Empty();
    UnitToCellMap.Empty();
    RegisteredUnits.Empty();
    
    Super::EndPlay(EndPlayReason);
}

void UMingSpatialPartitionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // 限制更新頻率
    UpdateTimer += DeltaTime;
    if (UpdateTimer < UpdateFrequency)
    {
        return;
    }
    UpdateTimer = 0.0f;
    
    // 更新相機資訊
    UpdateCameraInfo();
    
    // 更新網格活躍狀態
    UpdateCellActivity();
    
    // 執行視錐剔除
    PerformFrustumCulling();
    
    // 更新活躍網格中的單位
    UpdateActiveCells(DeltaTime);
}

void UMingSpatialPartitionComponent::RegisterUnit(AMingTacticalUnit* Unit)
{
    if (!Unit || RegisteredUnits.Contains(Unit))
    {
        return;
    }
    
    // 添加到註冊列表
    RegisteredUnits.Add(Unit);
    
    // 計算網格位置
    FIntVector GridPos = WorldToGrid(Unit->GetActorLocation());
    
    // 添加到對應網格
    AddUnitToCell(Unit, GridPos);
    UnitToCellMap.Add(Unit, GridPos);
    
    UE_LOG(LogTemp, Verbose, TEXT("Unit registered to spatial partition at grid (%d, %d, %d)"),
        GridPos.X, GridPos.Y, GridPos.Z);
}

void UMingSpatialPartitionComponent::UnregisterUnit(AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return;
    }
    
    // 從網格中移除
    if (FIntVector* CellPos = UnitToCellMap.Find(Unit))
    {
        RemoveUnitFromCell(Unit, *CellPos);
        UnitToCellMap.Remove(Unit);
    }
    
    // 從註冊列表移除
    RegisteredUnits.Remove(Unit);
    
    UE_LOG(LogTemp, Verbose, TEXT("Unit unregistered from spatial partition"));
}

void UMingSpatialPartitionComponent::UpdateUnitPosition(AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return;
    }
    
    // 獲取當前網格位置
    FIntVector* OldCellPos = UnitToCellMap.Find(Unit);
    if (!OldCellPos)
    {
        return;
    }
    
    // 計算新網格位置
    FIntVector NewCellPos = WorldToGrid(Unit->GetActorLocation());
    
    // 如果網格改變，移動單位
    if (*OldCellPos != NewCellPos)
    {
        RemoveUnitFromCell(Unit, *OldCellPos);
        AddUnitToCell(Unit, NewCellPos);
        *OldCellPos = NewCellPos;
        
        UE_LOG(LogTemp, Verbose, TEXT("Unit moved from grid (%d, %d, %d) to (%d, %d, %d)"),
            OldCellPos->X, OldCellPos->Y, OldCellPos->Z,
            NewCellPos.X, NewCellPos.Y, NewCellPos.Z);
    }
}

TArray<AMingTacticalUnit*> UMingSpatialPartitionComponent::GetUnitsInRadius(const FVector& Center, float Radius) const
{
    TArray<AMingTacticalUnit*> Result;
    
    // 計算半徑覆蓋的網格範圍
    FIntVector CenterGrid = WorldToGrid(Center);
    int32 GridRadius = FMath::CeilToInt(Radius / CellSize);
    
    for (int32 x = -GridRadius; x <= GridRadius; ++x)
    {
        for (int32 y = -GridRadius; y <= GridRadius; ++y)
        {
            for (int32 z = -GridRadius; z <= GridRadius; ++z)
            {
                FIntVector GridPos = CenterGrid + FIntVector(x, y, z);
                
                const FSpatialGridCell* Cell = GridCells.Find(GridPos);
                if (Cell)
                {
                    for (AMingTacticalUnit* Unit : Cell->Units)
                    {
                        if (Unit && FVector::Dist(Unit->GetActorLocation(), Center) <= Radius)
                        {
                            Result.Add(Unit);
                        }
                    }
                }
            }
        }
    }
    
    return Result;
}

TArray<AMingTacticalUnit*> UMingSpatialPartitionComponent::GetUnitsInCell(const FIntVector& GridPosition) const
{
    TArray<AMingTacticalUnit*> Result;
    
    const FSpatialGridCell* Cell = GridCells.Find(GridPosition);
    if (Cell)
    {
        Result = Cell->Units;
    }
    
    return Result;
}

TArray<FIntVector> UMingSpatialPartitionComponent::GetVisibleGridCells() const
{
    TArray<FIntVector> Result;
    
    for (const auto& Pair : GridCells)
    {
        if (Pair.Value.bIsActive)
        {
            Result.Add(Pair.Key);
        }
    }
    
    return Result;
}

void UMingSpatialPartitionComponent::SetCellSize(float NewCellSize)
{
    CellSize = FMath::Max(100.0f, NewCellSize);
    
    // 重新註冊所有單位
    TArray<AMingTacticalUnit*> TempUnits = RegisteredUnits;
    GridCells.Empty();
    UnitToCellMap.Empty();
    
    for (AMingTacticalUnit* Unit : TempUnits)
    {
        if (Unit)
        {
            RegisterUnit(Unit);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Cell size updated to %.1f, re-registered %d units"), CellSize, RegisteredUnits.Num());
}

void UMingSpatialPartitionComponent::SetUpdateRadius(float NewRadius)
{
    UpdateRadius = FMath::Max(1000.0f, NewRadius);
    UE_LOG(LogTemp, Log, TEXT("Update radius set to %.1f"), UpdateRadius);
}

int32 UMingSpatialPartitionComponent::GetActiveGridCellCount() const
{
    int32 Count = 0;
    
    for (const auto& Pair : GridCells)
    {
        if (Pair.Value.bIsActive)
        {
            Count++;
        }
    }
    
    return Count;
}

FIntVector UMingSpatialPartitionComponent::WorldToGrid(const FVector& WorldPosition) const
{
    return FIntVector(
        FMath::FloorToInt(WorldPosition.X / CellSize),
        FMath::FloorToInt(WorldPosition.Y / CellSize),
        FMath::FloorToInt(WorldPosition.Z / CellSize)
    );
}

FVector UMingSpatialPartitionComponent::GridToWorld(const FIntVector& GridPosition) const
{
    return FVector(
        GridPosition.X * CellSize + CellSize * 0.5f,
        GridPosition.Y * CellSize + CellSize * 0.5f,
        GridPosition.Z * CellSize + CellSize * 0.5f
    );
}

void UMingSpatialPartitionComponent::ForceUpdateAllCells()
{
    // 喚醒所有網格並更新
    for (auto& Pair : GridCells)
    {
        FSpatialGridCell& Cell = Pair.Value;
        Cell.bIsActive = true;
        Cell.LastUpdateTime = GetWorld()->GetTimeSeconds();
        
        // 喚醒所有單位
        WakeUnitsInCell(Pair.Key);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Forced update of %d grid cells"), GridCells.Num());
}

void UMingSpatialPartitionComponent::GetDebugGridInfo(TArray<FVector>& CellCenters, TArray<int32>& CellUnitCounts) const
{
    CellCenters.Empty();
    CellUnitCounts.Empty();
    
    for (const auto& Pair : GridCells)
    {
        FVector Center = GridToWorld(Pair.Key);
        int32 Count = Pair.Value.Units.Num();
        
        CellCenters.Add(Center);
        CellUnitCounts.Add(Count);
    }
}

void UMingSpatialPartitionComponent::UpdateCameraInfo()
{
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PlayerController = World->GetFirstPlayerController())
        {
            if (APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager)
            {
                CameraLocation = CameraManager->GetCameraLocation();
                CameraRotation = CameraManager->GetCameraRotation();
            }
        }
    }
}

FSpatialGridCell& UMingSpatialPartitionComponent::GetOrCreateCell(const FIntVector& GridPosition)
{
    FSpatialGridCell* Cell = GridCells.Find(GridPosition);
    
    if (!Cell)
    {
        FSpatialGridCell NewCell;
        NewCell.GridPosition = GridPosition;
        NewCell.Units.Reserve(50);
        
        GridCells.Add(GridPosition, NewCell);
        Cell = GridCells.Find(GridPosition);
    }
    
    return *Cell;
}

void UMingSpatialPartitionComponent::RemoveUnitFromCell(AMingTacticalUnit* Unit, const FIntVector& CellPosition)
{
    FSpatialGridCell* Cell = GridCells.Find(CellPosition);
    
    if (Cell)
    {
        Cell->Units.Remove(Unit);
        
        // 如果網格為空，標記為非活躍
        if (Cell->Units.Num() == 0)
        {
            Cell->bIsActive = false;
        }
    }
}

void UMingSpatialPartitionComponent::AddUnitToCell(AMingTacticalUnit* Unit, const FIntVector& CellPosition)
{
    FSpatialGridCell& Cell = GetOrCreateCell(CellPosition);
    
    if (!Cell.Units.Contains(Unit))
    {
        Cell.Units.Add(Unit);
        Cell.bIsActive = true;
    }
}

void UMingSpatialPartitionComponent::UpdateCellActivity()
{
    if (GridCells.Num() == 0)
    {
        return;
    }
    
    // 計算每個網格到相機的距離和優先級
    for (auto& Pair : GridCells)
    {
        FSpatialGridCell& Cell = Pair.Value;
        
        // 計算網格中心到相機的距離
        FVector CellCenter = GridToWorld(Pair.Key);
        float Distance = FVector::Dist(CellCenter, CameraLocation);
        
        // 設置活躍狀態
        Cell.bIsActive = (Distance <= UpdateRadius);
        
        // 計算更新優先級 (距離越近優先級越高)
        if (Cell.bIsActive)
        {
            Cell.UpdatePriority = 1.0f - (Distance / UpdateRadius);
        }
        else
        {
            Cell.UpdatePriority = 0.0f;
        }
    }
}

void UMingSpatialPartitionComponent::UpdateActiveCells(float DeltaTime)
{
    // 收集活躍網格
    TArray<FIntVector> ActiveCells;
    
    for (const auto& Pair : GridCells)
    {
        if (Pair.Value.bIsActive)
        {
            ActiveCells.Add(Pair.Key);
        }
    }
    
    // 按優先級排序
    ActiveCells.Sort([this](const FIntVector& A, const FIntVector& B) {
        const FSpatialGridCell* CellA = GridCells.Find(A);
        const FSpatialGridCell* CellB = GridCells.Find(B);
        
        if (CellA && CellB)
        {
            return CellA->UpdatePriority > CellB->UpdatePriority;
        }
        
        return false;
    });
    
    // 限制每幀更新的網格數
    int32 CellsToUpdate = FMath::Min(ActiveCells.Num(), MaxCellsPerFrame);
    
    for (int32 i = 0; i < CellsToUpdate; ++i)
    {
        FSpatialGridCell* Cell = GridCells.Find(ActiveCells[i]);
        
        if (Cell)
        {
            // 批量更新該網格中的單位
            BatchUpdateUnits(Cell->Units, DeltaTime);
            
            // 更新時間戳
            Cell->LastUpdateTime = GetWorld()->GetTimeSeconds();
        }
    }
}

void UMingSpatialPartitionComponent::PerformFrustumCulling()
{
    // 獲取視錐角點
    TArray<FVector> FrustumCorners;
    GetFrustumCorners(FrustumCorners);
    
    // 對每個活躍網格進行視錐剔除測試
    for (auto& Pair : GridCells)
    {
        if (Pair.Value.bIsActive)
        {
            // 簡化的視錐剔除測試：檢查網格是否在視錐內
            bool bInFrustum = IsCellInFrustum(Pair.Key);
            
            // 如果不在視錐內，可以降低更新頻率或暫停更新
            if (!bInFrustum)
            {
                Pair.Value.UpdatePriority *= 0.5f; // 降低優先級
            }
        }
    }
}

bool UMingSpatialPartitionComponent::IsCellInFrustum(const FIntVector& CellPosition) const
{
    // 獲取網格的AABB
    FBox CellBounds = GetCellBounds(CellPosition);
    
    // 簡化的視錐剔除：檢查網格中心是否在相機前方
    FVector CellCenter = CellBounds.GetCenter();
    FVector ToCell = CellCenter - CameraLocation;
    
    // 使用點積檢查是否在相機前方
    FVector CameraForward = CameraRotation.Vector();
    float DotProduct = FVector::DotProduct(ToCell.GetSafeNormal(), CameraForward);
    
    // 如果點積為正，說明在相機前方
    return DotProduct > -0.1f; // 允許一點容差
}

void UMingSpatialPartitionComponent::GetFrustumCorners(TArray<FVector>& OutCorners) const
{
    // 簡化的視錐計算
    // 在實際實現中，這裡應該從相機獲取準確的視錐參數
    OutCorners.Empty(8);
    
    // 使用相機位置和旋轉計算視錐角點
    FVector Forward = CameraRotation.Vector();
    FVector Right = FVector::CrossProduct(Forward, FVector::UpVector);
    FVector Up = FVector::CrossProduct(Right, Forward);
    
    // 近裁面和遠裁面距離
    float NearPlane = 100.0f;
    float FarPlane = 10000.0f;
    float FOV = 90.0f; // 水平視野角度
    
    // 計算視錐角點
    float TanHalfFOV = FMath::Tan(FMath::DegreesToRadians(FOV * 0.5f));
    
    FVector NearCenter = CameraLocation + Forward * NearPlane;
    FVector FarCenter = CameraLocation + Forward * FarPlane;
    
    float NearHeight = NearPlane * TanHalfFOV;
    float NearWidth = NearHeight * 1.333f; // 假設4:3寬高比
    float FarHeight = FarPlane * TanHalfFOV;
    float FarWidth = FarHeight * 1.333f;
    
    // 近裁面四個角
    OutCorners.Add(NearCenter + Up * NearHeight - Right * NearWidth);
    OutCorners.Add(NearCenter + Up * NearHeight + Right * NearWidth);
    OutCorners.Add(NearCenter - Up * NearHeight - Right * NearWidth);
    OutCorners.Add(NearCenter - Up * NearHeight + Right * NearWidth);
    
    // 遠裁面四個角
    OutCorners.Add(FarCenter + Up * FarHeight - Right * FarWidth);
    OutCorners.Add(FarCenter + Up * FarHeight + Right * FarWidth);
    OutCorners.Add(FarCenter - Up * FarHeight - Right * FarWidth);
    OutCorners.Add(FarCenter - Up * FarHeight + Right * FarWidth);
}

FBox UMingSpatialPartitionComponent::GetCellBounds(const FIntVector& CellPosition) const
{
    FVector Center = GridToWorld(CellPosition);
    FVector Extent(CellSize * 0.5f, CellSize * 0.5f, CellSize * 0.5f);
    
    return FBox(Center - Extent, Center + Extent);
}

float UMingSpatialPartitionComponent::CalculateCellPriority(const FIntVector& CellPosition) const
{
    FVector CellCenter = GridToWorld(CellPosition);
    float Distance = FVector::Dist(CellCenter, CameraLocation);
    
    if (Distance > UpdateRadius)
    {
        return 0.0f;
    }
    
    return 1.0f - (Distance / UpdateRadius);
}

void UMingSpatialPartitionComponent::BatchUpdateUnits(const TArray<AMingTacticalUnit*>& Units, float DeltaTime)
{
    // 批量更新單位
    for (AMingTacticalUnit* Unit : Units)
    {
        if (Unit && Unit->IsAlive())
        {
            // 這裡可以調用單位的更新函數
            // 例如：更新動畫、檢查戰鬥狀態等
        }
    }
}

void UMingSpatialPartitionComponent::SleepUnitsInCell(const FIntVector& CellPosition)
{
    FSpatialGridCell* Cell = GridCells.Find(CellPosition);
    
    if (Cell)
    {
        for (AMingTacticalUnit* Unit : Cell->Units)
        {
            if (Unit)
            {
                // 休眠單位，減少Tick頻率或完全暫停
                // 這裡可以設置單位的休眠狀態
            }
        }
    }
}

void UMingSpatialPartitionComponent::WakeUnitsInCell(const FIntVector& CellPosition)
{
    FSpatialGridCell* Cell = GridCells.Find(CellPosition);
    
    if (Cell)
    {
        for (AMingTacticalUnit* Unit : Cell->Units)
        {
            if (Unit)
            {
                // 喚醒單位，恢復正常Tick
                // 這裡可以設置單位的激活狀態
            }
        }
    }
}
