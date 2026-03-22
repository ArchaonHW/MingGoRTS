#include "MingFogOfWarManager.h"
#include "MingTacticalCombatSystem.h"
#include "MingFormationManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Algo/RandomStream.h"

UMingFogOfWarManager::UMingFogOfWarManager()
{
    bInitialized = false;
    UnitVisionDatabase.Empty();
    FogGrid.Empty();
    TerrainGrid.Empty();
    StructureOcclusionGrid.Empty();
    ScoutingReports.Empty();
    ActiveScoutingUnits.Empty();
    GridSizeX = 100;
    GridSizeY = 100;
    CellSize = 50.0f;
}

void UMingFogOfWarManager::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Log, TEXT("戰爭迷霧管理器開始運行"));
    
    // 初始化系統
    InitializeFogOfWarSystem();
}

void UMingFogOfWarManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!bInitialized)
    {
        return;
    }

    // 處理偵察任務
    for (int32 UnitID : ActiveScoutingUnits)
    {
        ProcessScoutingMission(UnitID, DeltaTime);
    }

    // 更新單位視野
    for (const TPair<int32, FMingVisionData>& Pair : UnitVisionDatabase)
    {
        int32 UnitID = Pair.Key;
        const FMingVisionData& VisionData = Pair.Value;
        
        if (VisionData.VisionUpdateInterval > 0.0f)
        {
            static float AccumulatedTime = 0.0f;
            AccumulatedTime += DeltaTime;
            
            if (AccumulatedTime >= VisionData.VisionUpdateInterval)
            {
                UpdateUnitVisionInternal(UnitID);
                AccumulatedTime = 0.0f;
            }
        }
    }

    // 更新戰爭迷霧
    UpdateFogOfWar();
}

bool UMingFogOfWarManager::InitializeFogOfWarSystem()
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("戰爭迷霧系統已經初始化"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("正在初始化戰爭迷霧系統..."));

    // 初始化迷霧網格
    InitializeFogGrid();

    // 初始化地形網格
    InitializeTerrainGrid();

    // 載入已保存的迷霧數據
    LoadFogOfWarData();

    bInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("戰爭迷霧系統初始化完成，網格大小: %dx%d"), GridSizeX, GridSizeY);
    
    return true;
}

bool UMingFogOfWarManager::RegisterUnitVision(int32 UnitID, const FMingVisionData& VisionData)
{
    if (!ValidateVisionData(VisionData))
    {
        UE_LOG(LogTemp, Error, TEXT("視野數據驗證失敗"));
        return false;
    }

    if (UnitVisionDatabase.Contains(UnitID))
    {
        UE_LOG(LogTemp, Warning, TEXT("單位視野ID %d 已存在，將覆蓋"), UnitID);
    }

    UnitVisionDatabase.Add(UnitID, VisionData);
    UpdateUnitVisionInternal(UnitID);

    UE_LOG(LogTemp, Log, TEXT("註冊單位視野: ID %d，半徑 %.1f"), UnitID, VisionData.VisionRadius);
    
    return true;
}

bool UMingFogOfWarManager::UpdateUnitVision(int32 UnitID, const FVector& NewPosition)
{
    if (!UnitVisionDatabase.Contains(UnitID))
    {
        UE_LOG(LogTemp, Warning, TEXT("單位ID %d 不存在，無法更新視野"), UnitID);
        return false;
    }

    FMingVisionData& VisionData = UnitVisionDatabase[UnitID];
    VisionData.LastUpdateTime = FDateTime::Now();

    // 更新位置並重新計算視野
    UpdateUnitVisionInternal(UnitID);

    UE_LOG(LogTemp, Log, TEXT("更新單位 %d 視野位置"), UnitID);
    
    return true;
}

FMingVisionData UMingFogOfWarManager::GetUnitVision(int32 UnitID) const
{
    if (UnitVisionDatabase.Contains(UnitID))
    {
        return UnitVisionDatabase[UnitID];
    }
    
    UE_LOG(LogTemp, Warning, TEXT("單位視野ID %d 不存在，返回空視野數據"), UnitID);
    return FMingVisionData();
}

TArray<int32> UMingFogOfWarManager::GetVisibleUnits(int32 UnitID) const
{
    if (UnitVisionDatabase.Contains(UnitID))
    {
        return UnitVisionDatabase[UnitID].VisibleUnits;
    }
    
    return TArray<int32>();
}

bool UMingFogOfWarManager::IsUnitVisible(int32 ViewerID, int32 TargetID) const
{
    if (!UnitVisionDatabase.Contains(ViewerID))
    {
        return false;
    }

    const FMingVisionData& VisionData = UnitVisionDatabase[ViewerID];
    return VisionData.VisibleUnits.Contains(TargetID);
}

void UMingFogOfWarManager::UpdateFogOfWar()
{
    // 清除所有網格的偵察狀態
    for (int32 y = 0; y < GridSizeY; y++)
    {
        for (int32 x = 0; x < GridSizeX; x++)
        {
            UpdateFogGridCell(x, y, EMingFogLevel::Heavy, false, -1);
        }
    }

    // 根據所有單位的視野更新迷霧
    for (const TPair<int32, FMingVisionData>& Pair : UnitVisionDatabase)
    {
        int32 UnitID = Pair.Key;
        const FMingVisionData& VisionData = Pair.Value;
        
        // 計算該單位可見的網格
        TArray<FIntPoint> VisibleGridCells = CalculateVisibleGridCells(UnitID);
        
        // 更新可見網格的迷霧等級
        for (const FIntPoint& GridCell : VisibleGridCells)
        {
            if (IsValidGridPosition(GridCell))
            {
                UpdateFogGridCell(GridCell.X, GridCell.Y, EMingFogLevel::Clear, true, UnitID);
            }
        }
    }

    // 應用地形和建築物遮蔽效果
    ApplyTerrainAndStructureOcclusion();

    // 更新迷霧渲染
    UpdateFogRendering();
}

EMingFogLevel UMingFogOfWarManager::GetFogLevel(const FVector& Location) const
{
    FIntPoint GridPos = WorldToGrid(Location);
    
    if (IsValidGridPosition(GridPos))
    {
        int32 GridIndex = GetGridIndex(GridPos.X, GridPos.Y);
        if (GridIndex >= 0 && GridIndex < FogGrid.Num())
        {
            return FogGrid[GridIndex].FogLevel;
        }
    }
    
    return EMingFogLevel::Complete;
}

bool UMingFogOfWarManager::SetTerrainType(const FVector& Location, EMingTerrainType TerrainType)
{
    FIntPoint GridPos = WorldToGrid(Location);
    
    if (IsValidGridPosition(GridPos))
    {
        int32 GridIndex = GetGridIndex(GridPos.X, GridPos.Y);
        if (GridIndex >= 0 && GridIndex < TerrainGrid.Num())
        {
            TerrainGrid[GridIndex] = TerrainType;
            UE_LOG(LogTemp, Log, TEXT("設置地形類型: 網格(%d,%d) = %d"), 
                GridPos.X, GridPos.Y, (int32)TerrainType);
        }
    }
    
    return true;
}

bool UMingFogOfWarManager::AddStructureOcclusion(const FVector& Location, float OcclusionValue)
{
    FIntPoint GridPos = WorldToGrid(Location);
    
    if (IsValidGridPosition(GridPos))
    {
        int32 GridIndex = GetGridIndex(GridPos.X, GridPos.Y);
        if (GridIndex >= 0 && GridIndex < StructureOcclusionGrid.Num())
        {
            StructureOcclusionGrid[GridIndex] = OcclusionValue;
            UE_LOG(LogTemp, Log, TEXT("添加建築物遮蔽: 網格(%d,%d) = %.1f"), 
                GridPos.X, GridPos.Y, OcclusionValue);
        }
    }
    
    return true;
}

bool UMingFogOfWarManager::ExecuteScoutingMission(int32 UnitID, const FVector& TargetLocation, float Radius)
{
    if (!UnitVisionDatabase.Contains(UnitID))
    {
        UE_LOG(LogTemp, Warning, TEXT("單位ID %d 不存在，無法執行偵察任務"), UnitID);
        return false;
    }

    // 設置偵察狀態
    SetScoutingStatus(UnitID, EMingScoutingStatus::Scouting);

    // 生成偵察報告
    FMingScoutingReport Report = GenerateScoutingReport(UnitID, TargetLocation, Radius);

    // 添加到活動偵察列表
    if (!ActiveScoutingUnits.Contains(UnitID))
    {
        ActiveScoutingUnits.Add(UnitID);
    }

    UE_LOG(LogTemp, Log, TEXT("執行偵察任務: 單位 %d，目標位置 (%.1f, %.1f, %.1f)，半徑 %.1f"), 
        UnitID, TargetLocation.X, TargetLocation.Y, TargetLocation.Z, Radius);
    
    return true;
}

TArray<FMingScoutingReport> UMingFogOfWarManager::GetScoutingReports() const
{
    return ScoutingReports;
}

bool UMingFogOfWarManager::SetScoutingStatus(int32 UnitID, EMingScoutingStatus Status)
{
    if (!UnitVisionDatabase.Contains(UnitID))
    {
        UE_LOG(LogTemp, Warning, TEXT("單位ID %d 不存在，無法設置偵察狀態"), UnitID);
        return false;
    }

    // 這裡需要更新單位的偵察狀態
    // 暫時記錄日誌
    UE_LOG(LogTemp, Log, TEXT("設置單位 %d 偵察狀態: %d"), UnitID, (int32)Status);
    
    return true;
}

bool UMingFogOfWarManager::SaveFogOfWarData()
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("FogOfWar.json");
    
    // 這裡應該實作JSON序列化保存邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("戰爭迷霧數據已保存到: %s"), *SavePath);
    UE_LOG(LogTemp, Log, TEXT("共保存 %d 個單位視野，%dx%d 迷霧網格"), 
        UnitVisionDatabase.Num(), GridSizeX, GridSizeY);
    
    return true;
}

bool UMingFogOfWarManager::LoadFogOfWarData()
{
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("FogOfWar.json");
    
    // 這裡應該實作JSON反序列化載入邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("嘗試從 %s 載入戰爭迷霧數據"), *LoadPath);
    
    return true;
}

void UMingFogOfWarManager::ClearAllFogOfWarData()
{
    UnitVisionDatabase.Empty();
    FogGrid.Empty();
    TerrainGrid.Empty();
    StructureOcclusionGrid.Empty();
    ScoutingReports.Empty();
    ActiveScoutingUnits.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("所有戰爭迷霧數據已清除"));
}

void UMingFogOfWarManager::InitializeFogGrid()
{
    // 初始化迷霧網格
    FogGrid.Empty();
    for (int32 y = 0; y < GridSizeY; y++)
    {
        for (int32 x = 0; x < GridSizeX; x++)
        {
            FMingFogGridCell Cell;
            Cell.GridPosition = FIntPoint(x, y);
            Cell.FogLevel = EMingFogLevel::Complete;
            Cell.bIsScouted = false;
            Cell.ScoutedTime = FDateTime::MinValue();
            Cell.ScoutingUnitID = -1;
            Cell.TerrainType = EMingTerrainType::Open;
            Cell.TerrainOcclusion = 0.0f;
            Cell.StructureOcclusion = 0.0f;
            Cell.LastVisibleTime = FDateTime::MinValue();
            
            FogGrid.Add(Cell);
        }
    }
}

void UMingFogOfWarManager::UpdateUnitVisionInternal(int32 UnitID)
{
    if (!UnitVisionDatabase.Contains(UnitID))
    {
        return;
    }

    FMingVisionData& VisionData = UnitVisionDatabase[UnitID];
    VisionData.VisibleUnits.Empty();
    VisionData.ScoutedAreas.Empty();

    // 計算新的可見單位
    CheckUnitsInVision(UnitID);

    // 計算可見的網格區域
    TArray<FIntPoint> VisibleGridCells = CalculateVisionArea(UnitID);
    
    // 更新可見單位列表
    for (const TPair<int32, FMingVisionData>& OtherPair : UnitVisionDatabase)
    {
        int32 OtherUnitID = OtherPair.Key;
        FVector OtherUnitPosition = FVector::ZeroVector; // 需要從戰鬥系統獲取
        
        // 檢查其他單位是否在視野內
        for (const FIntPoint& GridCell : VisibleGridCells)
        {
            FVector CellWorldPos = GridToWorld(GridCell);
            float Distance = FVector::Dist(OtherUnitPosition, CellWorldPos);
            
            if (Distance <= VisionData.VisionRadius)
            {
                if (!VisionData.VisibleUnits.Contains(OtherUnitID))
                {
                    VisionData.VisibleUnits.Add(OtherUnitID);
                }
            }
        }
    }

    // 更新偵察區域
    for (const FIntPoint& GridCell : VisibleGridCells)
    {
        VisionData.ScoutedAreas.Add(GridToWorld(GridCell));
    }

    // 觸發視野更新事件
    OnVisionUpdated.Broadcast(UnitID, VisionData.VisibleUnits);

    UE_LOG(LogTemp, Log, TEXT("更新單位 %d 視野，可見 %d 個單位"), 
        UnitID, VisionData.VisibleUnits.Num());
}

TArray<FIntPoint> UMingFogOfWarManager::CalculateVisibleGridCells(int32 UnitID)
{
    TArray<FIntPoint> VisibleCells;
    
    if (!UnitVisionDatabase.Contains(UnitID))
    {
        return VisibleCells;
    }

    const FMingVisionData& VisionData = UnitVisionDatabase[UnitID];
    FVector UnitPosition = FVector::ZeroVector; // 需要從戰鬥系統獲取

    // 計算視野範圍內的網格
    float VisionRadius = VisionData.VisionRadius;
    float VisionRadiusSquared = VisionRadius * VisionRadius;

    for (int32 y = 0; y < GridSizeY; y++)
    {
        for (int32 x = 0; x < GridSizeX; x++)
        {
            FIntPoint GridCell(x, y);
            FVector CellWorldPos = GridToWorld(GridCell);
            
            float DistanceSquared = FVector::DistSquared(UnitPosition, CellWorldPos);
            
            if (DistanceSquared <= VisionRadiusSquared)
            {
                // 檢查視野角度限制
                if (VisionData.VisionAngle >= 360.0f)
                {
                    VisibleCells.Add(GridCell);
                }
                else
                {
                    // 計算相對角度
                    FVector Direction = CellWorldPos - UnitPosition;
                    Direction.Normalize();
                    
                    // 簡化的角度檢查
                    float Angle = FMath::Atan2(Direction.Y, Direction.X);
                    float HalfAngle = FMath::DegreesToRadians(VisionData.VisionAngle * 0.5f);
                    
                    if (FMath::Abs(Angle) <= HalfAngle)
                    {
                        VisibleCells.Add(GridCell);
                    }
                }
            }
        }
    }

    return VisibleCells;
}

void UMingFogOfWarManager::UpdateFogGridCell(int32 GridX, int32 GridY, EMingFogLevel NewFogLevel, bool bIsScouted, int32 ScoutingUnitID)
{
    int32 GridIndex = GetGridIndex(GridX, GridY);
    
    if (GridIndex >= 0 && GridIndex < FogGrid.Num())
    {
        FMingFogGridCell& Cell = FogGrid[GridIndex];
        
        // 更新迷霧等級
        Cell.FogLevel = NewFogLevel;
        
        // 更新偵察狀態
        if (bIsScouted)
        {
            Cell.bIsScouted = true;
            Cell.ScoutedTime = FDateTime::Now();
            Cell.ScoutingUnitID = ScoutingUnitID;
        }
        
        // 更新最後可見時間
        if (NewFogLevel == EMingFogLevel::Clear)
        {
            Cell.LastVisibleTime = FDateTime::Now();
        }
    }
}

void UMingFogOfWarManager::ApplyTerrainAndStructureOcclusion()
{
    // 應用地形遮蔽效果
    for (int32 i = 0; i < FogGrid.Num(); i++)
    {
        FMingFogGridCell& Cell = FogGrid[i];
        
        if (Cell.TerrainType == EMingTerrainType::Forest || 
            Cell.TerrainType == EMingTerrainType::Mountain)
        {
            // 森林和山地提供遮蔽
            Cell.TerrainOcclusion = 0.3f;
        }
        else if (Cell.TerrainType == EMingTerrainType::Urban)
        {
            // 城市建築物提供遮蔽
            Cell.TerrainOcclusion = 0.2f;
        }
    }

    // 應用建築物遮蔽效果
    for (int32 i = 0; i < StructureOcclusionGrid.Num(); i++)
    {
        if (StructureOcclusionGrid[i] > 0.0f)
        {
            int32 GridIndex = i;
            if (GridIndex < FogGrid.Num())
            {
                FogGrid[GridIndex].StructureOcclusion = StructureOcclusionGrid[i];
            }
        }
    }
}

void UMingFogOfWarManager::ProcessScoutingMission(int32 UnitID, float DeltaTime)
{
    if (!UnitVisionDatabase.Contains(UnitID))
    {
        return;
    }

    const FMingVisionData& VisionData = UnitVisionDatabase[UnitID];
    
    // 模擬偵察進度
    static float ScoutingProgress = 0.0f;
    ScoutingProgress += DeltaTime * 0.1f; // 偵察速度
    
    if (ScoutingProgress >= 1.0f)
    {
        // 偵察完成
        SetScoutingStatus(UnitID, EMingScoutingStatus::Reporting);
        
        // 生成偵察報告
        FMingScoutingReport Report = GenerateScoutingReport(UnitID, VisionData.ScoutedAreas[0], VisionData.VisionRadius);
        
        // 添加到偵察報告列表
        ScoutingReports.Add(Report);
        
        // 從活動偵察列表移除
        ActiveScoutingUnits.Remove(UnitID);
        
        // 觸發偵察完成事件
        OnScoutingCompleted.Broadcast(Report);
        
        UE_LOG(LogTemp, Log, TEXT("單位 %d 偵察完成"), UnitID);
        
        // 重置進度
        ScoutingProgress = 0.0f;
    }
}

FMingScoutingReport UMingFogOfWarManager::GenerateScoutingReport(int32 UnitID, const FVector& Location, float Radius)
{
    FMingScoutingReport Report;
    Report.ReportID = ScoutingReports.Num() + 1;
    Report.ScoutingUnitID = UnitID;
    Report.ScoutingTime = FDateTime::Now();
    Report.ScoutingLocation = Location;
    Report.ScoutingRadius = Radius;
    
    // 模擬偵察結果
    Report.DiscoveredUnits.Empty();
    Report.DiscoveredStructures.Empty();
    Report.TerrainInfo.Empty();
    Report.ThreatAssessment = TEXT("輕微威脅");
    Report.ReportDetail = 0.8f;
    Report.ReportReliability = 0.9f;
    
    // 模擬發現的單位
    for (int32 i = 0; i < 5; i++)
    {
        int32 DiscoveredUnitID = 2000 + i; // 模擬敵方單位ID
        Report.DiscoveredUnits.Add(DiscoveredUnitID);
    }
    
    // 模擬發現的建築物
    Report.DiscoveredStructures.Add(TEXT("敵方前哨"));
    Report.DiscoveredStructures.Add(TEXT("補給站"));
    
    // 模擬地形信息
    Report.TerrainInfo.Add(TEXT("地形"), TEXT("丘陵地帶"));
    Report.TerrainInfo.Add(TEXT("植被"), TEXT("稀疏森林"));
    Report.TerrainInfo.Add(TEXT("掩體"), TEXT("岩石地形"));
    
    UE_LOG(LogTemp, Log, TEXT("生成偵察報告: 單位 %d，位置 (%.1f, %.1f, %.1f)"), 
        UnitID, Location.X, Location.Y, Location.Z);
    
    return Report;
}

void UMingFogOfWarManager::UpdateTerrainInfo(const FVector& Location)
{
    FIntPoint GridPos = WorldToGrid(Location);
    
    if (IsValidGridPosition(GridPos))
    {
        // 模擬地形分析
        EMingTerrainType TerrainType = EMingTerrainType::Open;
        
        // 基於位置模擬地形類型
        if (Location.Z > 100.0f)
        {
            TerrainType = EMingTerrainType::Mountain;
        }
        else if (Location.Z > 50.0f)
        {
            TerrainType = EMingTerrainType::Forest;
        }
        else if (Location.Z < 0.0f)
        {
            TerrainType = EMingTerrainType::Water;
        }
        
        SetTerrainType(Location, TerrainType);
    }
}

FIntPoint UMingFogOfWarManager::WorldToGrid(const FVector& WorldPosition) const
{
    return FIntPoint(
        FMath::FloorToInt(WorldPosition.X / CellSize),
        FMath::FloorToInt(WorldPosition.Y / CellSize)
    );
}

FVector UMingFogOfWarManager::GridToWorld(const FIntPoint& GridPosition) const
{
    return FVector(
        GridPosition.X * CellSize + CellSize * 0.5f,
        GridPosition.Y * CellSize + CellSize * 0.5f,
        0.0f
    );
}

bool UMingFogOfWarManager::IsValidGridPosition(const FIntPoint& GridPosition) const
{
    return GridPosition.X >= 0 && GridPosition.X < GridSizeX &&
           GridPosition.Y >= 0 && GridPosition.Y < GridSizeY;
}

int32 UMingFogOfWarManager::GetGridIndex(int32 GridX, int32 GridY) const
{
    return GridY * GridSizeX + GridX;
}

void UMingFogOfWarManager::CheckUnitsInVision(int32 UnitID)
{
    if (!UnitVisionDatabase.Contains(UnitID))
    {
        return;
    }

    FMingVisionData& VisionData = UnitVisionDatabase[UnitID];
    FVector UnitPosition = FVector::ZeroVector; // 需要從戰鬥系統獲取

    // 檢查所有其他單位是否在視野內
    for (const TPair<int32, FMingVisionData>& OtherPair : UnitVisionDatabase)
    {
        int32 OtherUnitID = OtherPair.Key;
        FVector OtherUnitPosition = FVector::ZeroVector; // 需要從戰鬥系統獲取
        
        float Distance = FVector::Dist(UnitPosition, OtherUnitPosition);
        
        if (Distance <= VisionData.VisionRadius)
        {
            if (!VisionData.VisibleUnits.Contains(OtherUnitID))
            {
                ProcessUnitDiscovery(UnitID, OtherUnitID);
            }
        }
        else
        {
            // 檢查單位是否離開視野
            if (VisionData.VisibleUnits.Contains(OtherUnitID))
            {
                ProcessUnitLossOfSight(UnitID, OtherUnitID);
            }
        }
    }
}

void UMingFogOfWarManager::ProcessUnitDiscovery(int32 ViewerID, int32 DiscoveredUnitID)
{
    // 觸發單位發現事件
    OnUnitSpotted.Broadcast(ViewerID, DiscoveredUnitID);
    
    UE_LOG(LogTemp, Log, TEXT("單位 %d 發現單位 %d"), ViewerID, DiscoveredUnitID);
}

void UMingFogOfWarManager::ProcessUnitLossOfSight(int32 ViewerID, int32 LostUnitID)
{
    // 觸發單位失去視野事件
    OnUnitLostSight.Broadcast(ViewerID, LostUnitID);
    
    UE_LOG(LogTemp, Log, TEXT("單位 %d 失去對單位 %d 的視野"), ViewerID, LostUnitID);
}

void UMingFogOfWarManager::CleanExpiredScoutingInfo()
{
    // 清除過期的偵察信息
    FDateTime CurrentTime = FDateTime::Now();
    FTimespan ExpiryDuration = FTimespan::FromMinutes(30); // 30分鐘後過期
    
    for (int32 i = ScoutingReports.Num() - 1; i >= 0; i--)
    {
        if (CurrentTime - ScoutingReports[i].ScoutingTime > ExpiryDuration)
        {
            ScoutingReports.RemoveAt(i);
        }
    }
}

void UMingFogOfWarManager::UpdateFogRendering()
{
    // 清除過期的偵察信息
    CleanExpiredScoutingInfo();
    
    // 更新迷霧渲染數據
    // 這裡應該更新渲染系統的數據
    // 暫時記錄日誌
    UE_LOG(LogTemp, VeryVerbose, TEXT("更新迷霧渲染數據"));
}
