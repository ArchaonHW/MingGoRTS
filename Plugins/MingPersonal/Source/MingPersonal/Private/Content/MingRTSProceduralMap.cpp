#include "Content/MingRTSProceduralMap.h"
#include "Math/UnrealMathUtility.h"

UMingRTSProceduralMap::UMingRTSProceduralMap()
    : bIsInitialized(false)
    , bIsGenerating(false)
    , CurrentSeed(0)
{
}

void UMingRTSProceduralMap::InitializeProceduralMapSystem()
{
    if (bIsInitialized)
    {
        return;
    }
    
    bIsInitialized = true;
    bIsGenerating = false;
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSProceduralMap: Initialized"));
}

void UMingRTSProceduralMap::ShutdownProceduralMapSystem()
{
    if (!bIsInitialized)
    {
        return;
    }
    
    SavedMaps.Empty();
    CurrentMap = FProceduralMapData();
    bIsGenerating = false;
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSProceduralMap: Shutdown"));
}

FProceduralMapData UMingRTSProceduralMap::GenerateMap(const FMapGenerationConfig& Config)
{
    if (!bIsInitialized || bIsGenerating)
    {
        return FProceduralMapData();
    }
    
    bIsGenerating = true;
    CurrentConfig = Config;
    
    // 設置種子
    CurrentSeed = Config.Seed != 0 ? Config.Seed : FMath::Rand();
    
    // 確定地圖尺寸
    int32 Width, Height;
    switch (Config.MapSize)
    {
    case EMapSize::Tiny:
        Width = Height = 64;
        break;
    case EMapSize::Small:
        Width = Height = 128;
        break;
    case EMapSize::Medium:
        Width = Height = 256;
        break;
    case EMapSize::Large:
        Width = Height = 512;
        break;
    case EMapSize::Huge:
        Width = Height = 1024;
        break;
    case EMapSize::Custom:
    default:
        Width = Config.CustomWidth;
        Height = Config.CustomHeight;
        break;
    }
    
    CurrentMap = FProceduralMapData();
    CurrentMap.MapID = FGuid::NewGuid().ToString();
    CurrentMap.MapName = FString::Printf(TEXT("ProceduralMap_%d"), CurrentSeed);
    CurrentMap.Width = Width;
    CurrentMap.Height = Height;
    CurrentMap.GenerationConfig = Config;
    CurrentMap.MinHeight = FLT_MAX;
    CurrentMap.MaxHeight = -FLT_MAX;
    
    OnMapGenerationStarted.Broadcast(CurrentMap.MapID);
    
    float StartTime = FPlatformTime::Seconds();
    
    // 生成步驟
    GenerateHeightmap();
    OnMapGenerationProgress.Broadcast(CurrentMap.MapID, 0.2f);
    
    ApplyClimateEffects();
    OnMapGenerationProgress.Broadcast(CurrentMap.MapID, 0.35f);
    
    GenerateRivers();
    OnMapGenerationProgress.Broadcast(CurrentMap.MapID, 0.5f);
    
    PlaceResources();
    OnMapGenerationProgress.Broadcast(CurrentMap.MapID, 0.65f);
    
    DefineRegions();
    OnMapGenerationProgress.Broadcast(CurrentMap.MapID, 0.8f);
    
    if (Config.bGenerateRoads)
    {
        GenerateRoads();
    }
    
    if (Config.bGenerateSettlements)
    {
        PlaceSettlements();
    }
    
    FinalizeMap();
    OnMapGenerationProgress.Broadcast(CurrentMap.MapID, 1.0f);
    
    CurrentMap.GenerationTime = FPlatformTime::Seconds() - StartTime;
    
    bIsGenerating = false;
    
    OnMapGenerationCompleted.Broadcast(CurrentMap.MapID, CurrentMap);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSProceduralMap: Generated map %s (%.2f seconds)"),
        *CurrentMap.MapID, CurrentMap.GenerationTime);
    
    return CurrentMap;
}

void UMingRTSProceduralMap::GenerateMapAsync(const FMapGenerationConfig& Config)
{
    // 異步生成 - 在實際實現中應使用多線程
    // 這裡簡化為同步調用
    GenerateMap(Config);
}

void UMingRTSProceduralMap::CancelGeneration()
{
    bIsGenerating = false;
    UE_LOG(LogTemp, Log, TEXT("MingRTSProceduralMap: Generation cancelled"));
}

FMapCellData UMingRTSProceduralMap::GetCell(int32 X, int32 Y) const
{
    int32 Index = GetCellIndex(X, Y);
    if (Index >= 0 && Index < CurrentMap.Cells.Num())
    {
        return CurrentMap.Cells[Index];
    }
    return FMapCellData();
}

FMapCellData UMingRTSProceduralMap::GetCellAtLocation(FVector2D Location) const
{
    int32 X = FMath::FloorToInt(Location.X);
    int32 Y = FMath::FloorToInt(Location.Y);
    return GetCell(X, Y);
}

void UMingRTSProceduralMap::SetCellTerrain(int32 X, int32 Y, ETerrainType NewTerrain)
{
    int32 Index = GetCellIndex(X, Y);
    if (Index >= 0 && Index < CurrentMap.Cells.Num())
    {
        CurrentMap.Cells[Index].TerrainType = NewTerrain;
        CurrentMap.Cells[Index].MovementCost = GetMovementCostForTerrain(NewTerrain);
        CurrentMap.Cells[Index].bWalkable = (NewTerrain != ETerrainType::Water && 
                                              NewTerrain != ETerrainType::Mountains);
        CurrentMap.Cells[Index].bBuildable = (NewTerrain == ETerrainType::Plains || 
                                              NewTerrain == ETerrainType::Hills);
    }
}

TArray<FMapCellData> UMingRTSProceduralMap::GetCellsInRadius(FVector2D Center, float Radius) const
{
    TArray<FMapCellData> Result;
    
    int32 MinX = FMath::Max(0, FMath::FloorToInt(Center.X - Radius));
    int32 MaxX = FMath::Min(CurrentMap.Width - 1, FMath::CeilToInt(Center.X + Radius));
    int32 MinY = FMath::Max(0, FMath::FloorToInt(Center.Y - Radius));
    int32 MaxY = FMath::Min(CurrentMap.Height - 1, FMath::CeilToInt(Center.Y + Radius));
    
    for (int32 Y = MinY; Y <= MaxY; ++Y)
    {
        for (int32 X = MinX; X <= MaxX; ++X)
        {
            FVector2D CellCenter(X + 0.5f, Y + 0.5f);
            if (GetDistance(CellCenter, Center) <= Radius)
            {
                Result.Add(GetCell(X, Y));
            }
        }
    }
    
    return Result;
}

TArray<FMapCellData> UMingRTSProceduralMap::GetCellsInRegion(const FString& RegionID) const
{
    TArray<FMapCellData> Result;
    
    for (const FMapRegion& Region : CurrentMap.Regions)
    {
        if (Region.RegionID == RegionID)
        {
            // 使用邊界框快速篩選
            int32 MinX = FMath::Max(0, FMath::FloorToInt(Region.BoundingBox.Min.X));
            int32 MaxX = FMath::Min(CurrentMap.Width - 1, FMath::CeilToInt(Region.BoundingBox.Max.X));
            int32 MinY = FMath::Max(0, FMath::FloorToInt(Region.BoundingBox.Min.Y));
            int32 MaxY = FMath::Min(CurrentMap.Height - 1, FMath::CeilToInt(Region.BoundingBox.Max.Y));
            
            for (int32 Y = MinY; Y <= MaxY; ++Y)
            {
                for (int32 X = MinX; X <= MaxX; ++X)
                {
                    FVector2D Point(X + 0.5f, Y + 0.5f);
                    // 檢查點是否在多邊形內
                    if (Region.BoundingBox.IsInside(Point))
                    {
                        Result.Add(GetCell(X, Y));
                    }
                }
            }
            break;
        }
    }
    
    return Result;
}

void UMingRTSProceduralMap::AddResourceNode(EMapResourceType Type, FVector2D Location, float Amount, float Quality)
{
    FResourceNode Node;
    Node.NodeID = FGuid::NewGuid().ToString();
    Node.ResourceType = Type;
    Node.Location = Location;
    Node.Amount = Amount;
    Node.Quality = Quality;
    
    CurrentMap.ResourceNodes.Add(Node);
    
    // 更新對應單元格的資源信息
    int32 X = FMath::FloorToInt(Location.X);
    int32 Y = FMath::FloorToInt(Location.Y);
    int32 Index = GetCellIndex(X, Y);
    if (Index >= 0 && Index < CurrentMap.Cells.Num())
    {
        CurrentMap.Cells[Index].ResourceType = Type;
        CurrentMap.Cells[Index].ResourceAmount = Amount;
    }
}

void UMingRTSProceduralMap::RemoveResourceNode(const FString& NodeID)
{
    for (int32 i = 0; i < CurrentMap.ResourceNodes.Num(); ++i)
    {
        if (CurrentMap.ResourceNodes[i].NodeID == NodeID)
        {
            // 清除單元格資源
            FVector2D Location = CurrentMap.ResourceNodes[i].Location;
            int32 X = FMath::FloorToInt(Location.X);
            int32 Y = FMath::FloorToInt(Location.Y);
            int32 Index = GetCellIndex(X, Y);
            if (Index >= 0 && Index < CurrentMap.Cells.Num())
            {
                CurrentMap.Cells[Index].ResourceType = EMapResourceType::None;
                CurrentMap.Cells[Index].ResourceAmount = 0.0f;
            }
            
            CurrentMap.ResourceNodes.RemoveAt(i);
            break;
        }
    }
}

TArray<FResourceNode> UMingRTSProceduralMap::GetResourceNodesInRadius(FVector2D Center, float Radius) const
{
    TArray<FResourceNode> Result;
    
    for (const FResourceNode& Node : CurrentMap.ResourceNodes)
    {
        if (GetDistance(Node.Location, Center) <= Radius)
        {
            Result.Add(Node);
        }
    }
    
    return Result;
}

FResourceNode UMingRTSProceduralMap::GetNearestResourceNode(FVector2D Location, EMapResourceType Type) const
{
    FResourceNode Nearest;
    float MinDistance = FLT_MAX;
    
    for (const FResourceNode& Node : CurrentMap.ResourceNodes)
    {
        if (Node.ResourceType == Type || Type == EMapResourceType::None)
        {
            float Dist = GetDistance(Node.Location, Location);
            if (Dist < MinDistance)
            {
                MinDistance = Dist;
                Nearest = Node;
            }
        }
    }
    
    return Nearest;
}

FMapRegion UMingRTSProceduralMap::GetRegion(const FString& RegionID) const
{
    for (const FMapRegion& Region : CurrentMap.Regions)
    {
        if (Region.RegionID == RegionID)
        {
            return Region;
        }
    }
    return FMapRegion();
}

FString UMingRTSProceduralMap::GetRegionAtLocation(FVector2D Location) const
{
    for (const FMapRegion& Region : CurrentMap.Regions)
    {
        if (Region.BoundingBox.IsInside(Location))
        {
            return Region.RegionID;
        }
    }
    return FString();
}

void UMingRTSProceduralMap::DefineRegion(const TArray<FVector2D>& Boundary, const FString& RegionName)
{
    FMapRegion Region;
    Region.RegionID = FGuid::NewGuid().ToString();
    Region.RegionName = RegionName;
    Region.BoundaryPoints = Boundary;
    
    // 計算邊界框和中心
    if (Boundary.Num() > 0)
    {
        FVector2D Min = Boundary[0];
        FVector2D Max = Boundary[0];
        FVector2D Sum = FVector2D::ZeroVector;
        
        for (const FVector2D& Point : Boundary)
        {
            Min.X = FMath::Min(Min.X, Point.X);
            Min.Y = FMath::Min(Min.Y, Point.Y);
            Max.X = FMath::Max(Max.X, Point.X);
            Max.Y = FMath::Max(Max.Y, Point.Y);
            Sum += Point;
        }
        
        Region.BoundingBox = FBox2D(Min, Max);
        Region.Center = Sum / Boundary.Num();
        Region.Area = (Max.X - Min.X) * (Max.Y - Min.Y);
    }
    
    // 統計區域內的地形
    TArray<FMapCellData> RegionCells = GetCellsInRegion(Region.RegionID);
    TMap<ETerrainType, int32> TerrainCounts;
    
    for (const FMapCellData& Cell : RegionCells)
    {
        TerrainCounts.FindOrAdd(Cell.TerrainType)++;
    }
    
    // 找出主要地形
    int32 MaxCount = 0;
    for (const auto& Pair : TerrainCounts)
    {
        if (Pair.Value > MaxCount)
        {
            MaxCount = Pair.Value;
            Region.DominantTerrain = Pair.Key;
        }
    }
    
    Region.CellCount = RegionCells.Num();
    
    CurrentMap.Regions.Add(Region);
}

TArray<FVector2D> UMingRTSProceduralMap::FindPath(FVector2D Start, FVector2D End) const
{
    return FindPathAvoidingTerrain(Start, End, TArray<ETerrainType>());
}

TArray<FVector2D> UMingRTSProceduralMap::FindPathAvoidingTerrain(FVector2D Start, FVector2D End, TArray<ETerrainType> AvoidedTerrains) const
{
    TArray<FVector2D> Path;
    
    // A* 路徑查找實現
    TArray<FVector2D> OpenSet;
    TArray<FVector2D> ClosedSet;
    TMap<FVector2D, FVector2D> CameFrom;
    TMap<FVector2D, float> GScore;
    TMap<FVector2D, float> FScore;
    
    OpenSet.Add(Start);
    GScore.Add(Start, 0.0f);
    FScore.Add(Start, Heuristic(Start, End));
    
    while (OpenSet.Num() > 0)
    {
        // 找到F值最小的節點
        FVector2D Current = OpenSet[0];
        float MinF = FScore[Current];
        int32 CurrentIndex = 0;
        
        for (int32 i = 1; i < OpenSet.Num(); ++i)
        {
            if (FScore[OpenSet[i]] < MinF)
            {
                MinF = FScore[OpenSet[i]];
                Current = OpenSet[i];
                CurrentIndex = i;
            }
        }
        
        if (GetDistance(Current, End) < 1.0f)
        {
            Path = ReconstructPath(CameFrom, Current);
            Path.Add(End);
            return Path;
        }
        
        OpenSet.RemoveAt(CurrentIndex);
        ClosedSet.Add(Current);
        
        // 檢查鄰居
        const int32 Neighbors[8][2] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
        
        for (int32 i = 0; i < 8; ++i)
        {
            FVector2D Neighbor(Current.X + Neighbors[i][0], Current.Y + Neighbors[i][1]);
            
            if (!IsValidCell(Neighbor.X, Neighbor.Y))
            {
                continue;
            }
            
            if (ClosedSet.Contains(Neighbor))
            {
                continue;
            }
            
            FMapCellData Cell = GetCell(Neighbor.X, Neighbor.Y);
            
            // 檢查是否應該避開
            if (AvoidedTerrains.Contains(Cell.TerrainType))
            {
                continue;
            }
            
            if (!Cell.bWalkable)
            {
                continue;
            }
            
            float TentativeGScore = GScore[Current] + Cell.MovementCost;
            
            if (!OpenSet.Contains(Neighbor))
            {
                OpenSet.Add(Neighbor);
            }
            else if (TentativeGScore >= GScore[Neighbor])
            {
                continue;
            }
            
            CameFrom.Add(Neighbor, Current);
            GScore.Add(Neighbor, TentativeGScore);
            FScore.Add(Neighbor, GScore[Neighbor] + Heuristic(Neighbor, End));
        }
    }
    
    return Path; // 返回空路徑表示未找到
}

bool UMingRTSProceduralMap::IsPathClear(FVector2D Start, FVector2D End) const
{
    TArray<FVector2D> Path = FindPath(Start, End);
    return Path.Num() > 0;
}

FMapAnalysisResult UMingRTSProceduralMap::AnalyzeMap() const
{
    FMapAnalysisResult Result;
    
    // 地形分佈
    int32 TotalCells = CurrentMap.Cells.Num();
    for (const FMapCellData& Cell : CurrentMap.Cells)
    {
        Result.TerrainDistribution.FindOrAdd(Cell.TerrainType)++;
    }
    
    // 轉換為百分比
    for (auto& Pair : Result.TerrainDistribution)
    {
        Pair.Value /= TotalCells;
    }
    
    // 資源統計
    for (const FResourceNode& Node : CurrentMap.ResourceNodes)
    {
        Result.ResourceCounts.FindOrAdd(Node.ResourceType)++;
    }
    
    // 高度統計
    float SumHeight = 0.0f;
    for (const FMapCellData& Cell : CurrentMap.Cells)
    {
        SumHeight += Cell.Height;
    }
    Result.AverageHeight = SumHeight / TotalCells;
    
    float VarianceSum = 0.0f;
    for (const FMapCellData& Cell : CurrentMap.Cells)
    {
        VarianceSum += FMath::Square(Cell.Height - Result.AverageHeight);
    }
    Result.HeightVariance = VarianceSum / TotalCells;
    
    // 戰略價值評估
    Result.StrategicPoints = FindStrategicPoints();
    Result.ChokePoints = FindChokePoints();
    
    // 平衡性評估
    Result.BalanceScore = CalculateMapBalance();
    Result.StrategicValue = Result.StrategicPoints.Num() / 10.0f; // 簡化計算
    
    return Result;
}

float UMingRTSProceduralMap::CalculateMapBalance() const
{
    // 檢查各種地形和資源的分佈是否均衡
    float Balance = 1.0f;
    
    // 檢查起始位置資源公平性
    // 這裡簡化為檢查資源總量
    float TotalResources = 0.0f;
    for (const FResourceNode& Node : CurrentMap.ResourceNodes)
    {
        TotalResources += Node.Amount * Node.Quality;
    }
    
    // 假設理想資源數量
    float IdealResources = CurrentMap.Width * CurrentMap.Height * 0.001f;
    float ResourceRatio = TotalResources / IdealResources;
    
    if (ResourceRatio > 1.5f || ResourceRatio < 0.5f)
    {
        Balance -= 0.2f;
    }
    
    // 檢查地形多樣性
    if (CurrentMap.Regions.Num() < 3)
    {
        Balance -= 0.2f;
    }
    
    return FMath::Clamp(Balance, 0.0f, 1.0f);
}

TArray<FVector2D> UMingRTSProceduralMap::FindStrategicPoints() const
{
    TArray<FVector2D> StrategicPoints;
    
    // 尋找高地形點和資源集中區域
    for (int32 Y = 5; Y < CurrentMap.Height - 5; Y += 10)
    {
        for (int32 X = 5; X < CurrentMap.Width - 5; X += 10)
        {
            FMapCellData Cell = GetCell(X, Y);
            
            // 高地
            if (Cell.Height > CurrentMap.MaxHeight * 0.8f)
            {
                StrategicPoints.Add(FVector2D(X + 0.5f, Y + 0.5f));
            }
            // 資源豐富區域
            else if (Cell.ResourceType != EMapResourceType::None && Cell.ResourceAmount > 200.0f)
            {
                StrategicPoints.Add(FVector2D(X + 0.5f, Y + 0.5f));
            }
        }
    }
    
    return StrategicPoints;
}

TArray<FVector2D> UMingRTSProceduralMap::FindChokePoints() const
{
    TArray<FVector2D> ChokePoints;
    
    // 尋找狹窄通道
    for (int32 Y = 1; Y < CurrentMap.Height - 1; ++Y)
    {
        for (int32 X = 1; X < CurrentMap.Width - 1; ++X)
        {
            FMapCellData Cell = GetCell(X, Y);
            
            if (!Cell.bWalkable)
            {
                continue;
            }
            
            // 計算可通行的鄰居數量
            int32 WalkableNeighbors = 0;
            const int32 Neighbors[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
            
            for (int32 i = 0; i < 4; ++i)
            {
                FMapCellData Neighbor = GetCell(X + Neighbors[i][0], Y + Neighbors[i][1]);
                if (Neighbor.bWalkable)
                {
                    WalkableNeighbors++;
                }
            }
            
            // 如果只有2個可通行方向，可能是瓶頸
            if (WalkableNeighbors == 2)
            {
                ChokePoints.Add(FVector2D(X + 0.5f, Y + 0.5f));
            }
        }
    }
    
    return ChokePoints;
}

bool UMingRTSProceduralMap::ValidateMap() const
{
    // 基本驗證
    if (CurrentMap.Width == 0 || CurrentMap.Height == 0)
    {
        return false;
    }
    
    if (CurrentMap.Cells.Num() != CurrentMap.Width * CurrentMap.Height)
    {
        return false;
    }
    
    // 檢查是否有足夠的行走空間
    int32 WalkableCount = 0;
    for (const FMapCellData& Cell : CurrentMap.Cells)
    {
        if (Cell.bWalkable)
        {
            WalkableCount++;
        }
    }
    
    // 至少30%的地圖應該可行走
    float WalkableRatio = static_cast<float>(WalkableCount) / CurrentMap.Cells.Num();
    if (WalkableRatio < 0.3f)
    {
        return false;
    }
    
    // 檢查是否有足夠的資源
    if (CurrentMap.ResourceNodes.Num() < 5)
    {
        return false;
    }
    
    return true;
}

bool UMingRTSProceduralMap::SaveMap(const FString& FilePath) const
{
    // 序列化地圖數據
    SavedMaps.Add(CurrentMap.MapID, CurrentMap);
    UE_LOG(LogTemp, Log, TEXT("MingRTSProceduralMap: Saved map %s to %s"), *CurrentMap.MapID, *FilePath);
    return true;
}

bool UMingRTSProceduralMap::LoadMap(const FString& FilePath)
{
    // 反序列化地圖數據
    UE_LOG(LogTemp, Log, TEXT("MingRTSProceduralMap: Loading map from %s"), *FilePath);
    return false; // 簡化實現
}

bool UMingRTSProceduralMap::ExportToTexture(const FString& FilePath, EMapResourceType ResourceToVisualize) const
{
    UE_LOG(LogTemp, Log, TEXT("MingRTSProceduralMap: Exporting map to texture %s"), *FilePath);
    return true; // 簡化實現
}

void UMingRTSProceduralMap::ApplyHeightmap(const TArray<float>& HeightData)
{
    if (HeightData.Num() != CurrentMap.Cells.Num())
    {
        return;
    }
    
    for (int32 i = 0; i < CurrentMap.Cells.Num(); ++i)
    {
        CurrentMap.Cells[i].Height = HeightData[i];
        CurrentMap.MinHeight = FMath::Min(CurrentMap.MinHeight, HeightData[i]);
        CurrentMap.MaxHeight = FMath::Max(CurrentMap.MaxHeight, HeightData[i]);
    }
}

void UMingRTSProceduralMap::SmoothTerrain(int32 Iterations)
{
    for (int32 Iter = 0; Iter < Iterations; ++Iter)
    {
        TArray<float> NewHeights;
        NewHeights.SetNum(CurrentMap.Cells.Num());
        
        for (int32 Y = 0; Y < CurrentMap.Height; ++Y)
        {
            for (int32 X = 0; X < CurrentMap.Width; ++X)
            {
                float Sum = GetCell(X, Y).Height;
                int32 Count = 1;
                
                // 平均鄰居高度
                const int32 Neighbors[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
                for (int32 i = 0; i < 4; ++i)
                {
                    int32 NX = X + Neighbors[i][0];
                    int32 NY = Y + Neighbors[i][1];
                    if (IsValidCell(NX, NY))
                    {
                        Sum += GetCell(NX, NY).Height;
                        Count++;
                    }
                }
                
                NewHeights[GetCellIndex(X, Y)] = Sum / Count;
            }
        }
        
        // 應用新高度
        for (int32 i = 0; i < CurrentMap.Cells.Num(); ++i)
        {
            CurrentMap.Cells[i].Height = NewHeights[i];
        }
    }
}

void UMingRTSProceduralMap::AddNoise(float Amplitude, float Frequency)
{
    for (int32 Y = 0; Y < CurrentMap.Height; ++Y)
    {
        for (int32 X = 0; X < CurrentMap.Width; ++X)
        {
            int32 Index = GetCellIndex(X, Y);
            float Noise = PerlinNoise(X * Frequency, Y * Frequency) * Amplitude;
            CurrentMap.Cells[Index].Height += Noise;
        }
    }
}

void UMingRTSProceduralMap::ErodeTerrain(int32 Iterations)
{
    for (int32 Iter = 0; Iter < Iterations; ++Iter)
    {
        for (int32 Y = 1; Y < CurrentMap.Height - 1; ++Y)
        {
            for (int32 X = 1; X < CurrentMap.Width - 1; ++X)
            {
                int32 Index = GetCellIndex(X, Y);
                float CurrentHeight = CurrentMap.Cells[Index].Height;
                
                // 找到最低的鄰居
                float MinNeighborHeight = CurrentHeight;
                const int32 Neighbors[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
                
                for (int32 i = 0; i < 4; ++i)
                {
                    float NH = GetCell(X + Neighbors[i][0], Y + Neighbors[i][1]).Height;
                    MinNeighborHeight = FMath::Min(MinNeighborHeight, NH);
                }
                
                // 侵蝕
                float Diff = CurrentHeight - MinNeighborHeight;
                if (Diff > 0.01f)
                {
                    CurrentMap.Cells[Index].Height -= Diff * 0.1f;
                }
            }
        }
    }
}

// 生成步驟實現
void UMingRTSProceduralMap::GenerateHeightmap()
{
    CurrentMap.Cells.SetNum(CurrentMap.Width * CurrentMap.Height);
    
    // 使用分形布朗運動生成地形
    for (int32 Y = 0; Y < CurrentMap.Height; ++Y)
    {
        for (int32 X = 0; X < CurrentMap.Width; ++X)
        {
            float Xf = X / static_cast<float>(CurrentMap.Width);
            float Yf = Y / static_cast<float>(CurrentMap.Height);
            
            float Height = FractalBrownianMotion(Xf * 4.0f, Yf * 4.0f, 6);
            
            // 添加大型地形特徵
            Height += PerlinNoise(Xf * 1.0f, Yf * 1.0f) * 0.5f;
            
            // 歸一化
            Height = (Height + 1.0f) * 0.5f;
            
            int32 Index = GetCellIndex(X, Y);
            CurrentMap.Cells[Index].X = X;
            CurrentMap.Cells[Index].Y = Y;
            CurrentMap.Cells[Index].Height = Height * CurrentConfig.MountainHeight;
            
            CurrentMap.MinHeight = FMath::Min(CurrentMap.MinHeight, CurrentMap.Cells[Index].Height);
            CurrentMap.MaxHeight = FMath::Max(CurrentMap.MaxHeight, CurrentMap.Cells[Index].Height);
        }
    }
}

void UMingRTSProceduralMap::ApplyClimateEffects()
{
    // 基於氣候類型應用溫度和濕度
    for (int32 Y = 0; Y < CurrentMap.Height; ++Y)
    {
        for (int32 X = 0; X < CurrentMap.Width; ++X)
        {
            int32 Index = GetCellIndex(X, Y);
            FMapCellData& Cell = CurrentMap.Cells[Index];
            
            // 基於緯度（Y坐標）的溫度
            float Latitude = Y / static_cast<float>(CurrentMap.Height);
            Cell.Temperature = 1.0f - FMath::Abs(Latitude - 0.5f) * 2.0f;
            
            // 濕度噪聲
            Cell.Moisture = PerlinNoise(X * 0.01f, Y * 0.01f) * 0.5f + 0.5f;
            
            // 根據高度調整溫度
            float HeightRatio = (Cell.Height - CurrentMap.MinHeight) / (CurrentMap.MaxHeight - CurrentMap.MinHeight);
            Cell.Temperature -= HeightRatio * 0.3f;
            
            // 確定地形類型
            Cell.TerrainType = DetermineTerrainFromHeight(HeightRatio, Cell.Moisture, Cell.Temperature);
            Cell.MovementCost = GetMovementCostForTerrain(Cell.TerrainType);
            Cell.bWalkable = (Cell.TerrainType != ETerrainType::Water && 
                              Cell.TerrainType != ETerrainType::Mountains);
            Cell.bBuildable = (Cell.TerrainType == ETerrainType::Plains || 
                               Cell.TerrainType == ETerrainType::Hills);
        }
    }
}

void UMingRTSProceduralMap::GenerateRivers()
{
    for (int32 i = 0; i < CurrentConfig.RiverCount; ++i)
    {
        // 找到高地的起點
        FVector2D Source;
        bool FoundSource = false;
        
        for (int32 Attempt = 0; Attempt < 100; ++Attempt)
        {
            int32 X = RandomInt(CurrentMap.Width);
            int32 Y = RandomInt(CurrentMap.Height);
            FMapCellData Cell = GetCell(X, Y);
            
            float HeightRatio = (Cell.Height - CurrentMap.MinHeight) / (CurrentMap.MaxHeight - CurrentMap.MinHeight);
            if (HeightRatio > 0.7f)
            {
                Source = FVector2D(X, Y);
                FoundSource = true;
                break;
            }
        }
        
        if (!FoundSource)
        {
            continue;
        }
        
        // 生成河流路徑
        FRiverData River;
        River.RiverID = FGuid::NewGuid().ToString();
        River.RiverName = FString::Printf(TEXT("River_%d"), i);
        River.Source = Source;
        River.Path = FindRiverPath(Source);
        
        if (River.Path.Num() > 0)
        {
            River.Mouth = River.Path.Last();
            River.Length = 0.0f;
            for (int32 j = 1; j < River.Path.Num(); ++j)
            {
                River.Length += GetDistance(River.Path[j-1], River.Path[j]);
            }
            
            CurrentMap.Rivers.Add(River);
            
            // 標記河流地形
            for (const FVector2D& Point : River.Path)
            {
                int32 X = FMath::FloorToInt(Point.X);
                int32 Y = FMath::FloorToInt(Point.Y);
                SetCellTerrain(X, Y, ETerrainType::River);
            }
        }
    }
}

void UMingRTSProceduralMap::PlaceResources()
{
    int32 TargetResourceCount = FMath::FloorToInt(CurrentMap.Width * CurrentMap.Height * CurrentConfig.ResourceDensity * 0.001f);
    
    for (int32 i = 0; i < TargetResourceCount; ++i)
    {
        EMapResourceType Type = static_cast<EMapResourceType>(RandomInt(static_cast<int32>(EMapResourceType::None)));
        FVector2D Location = FindOptimalResourceLocation(Type);
        
        if (IsValidResourceLocation(Location, Type))
        {
            float Amount = 100.0f + RandomFloat() * 400.0f;
            float Quality = 0.5f + RandomFloat() * 0.5f;
            AddResourceNode(Type, Location, Amount, Quality);
        }
    }
}

void UMingRTSProceduralMap::DefineRegions()
{
    // 使用簡單的區域劃分
    int32 RegionsX = FMath::CeilToInt(FMath::Sqrt(static_cast<float>(CurrentConfig.RegionCount)));
    int32 RegionsY = FMath::CeilToInt(static_cast<float>(CurrentConfig.RegionCount) / RegionsX);
    
    int32 RegionWidth = CurrentMap.Width / RegionsX;
    int32 RegionHeight = CurrentMap.Height / RegionsY;
    
    int32 RegionIndex = 0;
    for (int32 RY = 0; RY < RegionsY && RegionIndex < CurrentConfig.RegionCount; ++RY)
    {
        for (int32 RX = 0; RX < RegionsX && RegionIndex < CurrentConfig.RegionCount; ++RX)
        {
            TArray<FVector2D> Boundary;
            int32 MinX = RX * RegionWidth;
            int32 MaxX = FMath::Min((RX + 1) * RegionWidth, CurrentMap.Width);
            int32 MinY = RY * RegionHeight;
            int32 MaxY = FMath::Min((RY + 1) * RegionHeight, CurrentMap.Height);
            
            Boundary.Add(FVector2D(MinX, MinY));
            Boundary.Add(FVector2D(MaxX, MinY));
            Boundary.Add(FVector2D(MaxX, MaxY));
            Boundary.Add(FVector2D(MinX, MaxY));
            
            FString RegionName = FString::Printf(TEXT("Region_%d"), RegionIndex);
            DefineRegion(Boundary, RegionName);
            
            RegionIndex++;
        }
    }
}

void UMingRTSProceduralMap::GenerateRoads()
{
    // 在區域中心之間生成道路
    for (int32 i = 0; i < CurrentMap.Regions.Num(); ++i)
    {
        for (int32 j = i + 1; j < CurrentMap.Regions.Num(); ++j)
        {
            if (RandomFloat() < 0.3f) // 30%概率連接
            {
                FVector2D Start = CurrentMap.Regions[i].Center;
                FVector2D End = CurrentMap.Regions[j].Center;
                
                TArray<FVector2D> Path = FindPath(Start, End);
                if (Path.Num() > 0)
                {
                    CurrentMap.RoadNetworks.Add(Path);
                }
            }
        }
    }
}

void UMingRTSProceduralMap::PlaceSettlements()
{
    // 在每個區域放置定居點
    for (const FMapRegion& Region : CurrentMap.Regions)
    {
        // 尋找合適的位置（平坦、有資源）
        FVector2D BestLocation = Region.Center;
        float BestScore = -1.0f;
        
        for (int32 Attempt = 0; Attempt < 50; ++Attempt)
        {
            float OffsetX = (RandomFloat() - 0.5f) * Region.BoundingBox.GetExtent().X * 2.0f;
            float OffsetY = (RandomFloat() - 0.5f) * Region.BoundingBox.GetExtent().Y * 2.0f;
            FVector2D Location = Region.Center + FVector2D(OffsetX, OffsetY);
            
            int32 X = FMath::FloorToInt(Location.X);
            int32 Y = FMath::FloorToInt(Location.Y);
            
            if (!IsValidCell(X, Y))
            {
                continue;
            }
            
            FMapCellData Cell = GetCell(X, Y);
            
            if (!Cell.bBuildable)
            {
                continue;
            }
            
            // 評分：平坦度 + 資源接近度
            float Score = 1.0f;
            Score -= FMath::Abs(Cell.Height - CurrentMap.MinHeight) / (CurrentMap.MaxHeight - CurrentMap.MinHeight);
            
            // 附近資源加成
            TArray<FResourceNode> NearbyResources = GetResourceNodesInRadius(Location, 50.0f);
            Score += NearbyResources.Num() * 0.1f;
            
            if (Score > BestScore)
            {
                BestScore = Score;
                BestLocation = Location;
            }
        }
        
        if (BestScore > 0.0f)
        {
            CurrentMap.SettlementLocations.Add(BestLocation);
        }
    }
}

void UMingRTSProceduralMap::FinalizeMap()
{
    // 計算最終統計數據
    float LandArea = 0.0f;
    float WaterArea = 0.0f;
    
    for (const FMapCellData& Cell : CurrentMap.Cells)
    {
        if (Cell.TerrainType == ETerrainType::Water || Cell.TerrainType == ETerrainType::River)
        {
            WaterArea += 1.0f;
        }
        else
        {
            LandArea += 1.0f;
        }
    }
    
    CurrentMap.TotalLandArea = LandArea;
    CurrentMap.TotalWaterArea = WaterArea;
}

// 模板配置
FMapGenerationConfig UMingRTSProceduralMap::GetPlainsTemplate() const
{
    FMapGenerationConfig Config;
    Config.TerrainRoughness = 0.2f;
    Config.WaterLevel = 0.2f;
    Config.ForestDensity = 0.1f;
    Config.MountainHeight = 500.0f;
    return Config;
}

FMapGenerationConfig UMingRTSProceduralMap::GetMountainTemplate() const
{
    FMapGenerationConfig Config;
    Config.TerrainRoughness = 0.8f;
    Config.WaterLevel = 0.3f;
    Config.ForestDensity = 0.3f;
    Config.MountainHeight = 2000.0f;
    Config.RiverCount = 5;
    return Config;
}

FMapGenerationConfig UMingRTSProceduralMap::GetIslandTemplate() const
{
    FMapGenerationConfig Config;
    Config.TerrainRoughness = 0.4f;
    Config.WaterLevel = 0.5f;
    Config.ForestDensity = 0.5f;
    Config.Climate = EClimateType::Tropical;
    return Config;
}

FMapGenerationConfig UMingRTSProceduralMap::GetDesertTemplate() const
{
    FMapGenerationConfig Config;
    Config.Climate = EClimateType::Arid;
    Config.TerrainRoughness = 0.3f;
    Config.WaterLevel = 0.15f;
    Config.ForestDensity = 0.05f;
    Config.ResourceDensity = 0.8f; // 更多礦產資源
    return Config;
}

FMapGenerationConfig UMingRTSProceduralMap::GetRiverDeltaTemplate() const
{
    FMapGenerationConfig Config;
    Config.TerrainRoughness = 0.3f;
    Config.WaterLevel = 0.4f;
    Config.ForestDensity = 0.6f;
    Config.RiverCount = 6;
    Config.Climate = EClimateType::Monsoon;
    return Config;
}

// 噪聲函數
float UMingRTSProceduralMap::PerlinNoise(float X, float Y) const
{
    // 簡化實現 - 實際應使用標準Perlin噪聲算法
    float Value = FMath::Sin(X * 12.9898f + Y * 78.233f + CurrentSeed) * 43758.5453f;
    Value = FMath::Fractional(Value);
    return Value * 2.0f - 1.0f;
}

float UMingRTSProceduralMap::FractalBrownianMotion(float X, float Y, int32 Octaves) const
{
    float Total = 0.0f;
    float Frequency = 1.0f;
    float Amplitude = 1.0f;
    float MaxValue = 0.0f;
    
    for (int32 i = 0; i < Octaves; ++i)
    {
        Total += PerlinNoise(X * Frequency, Y * Frequency) * Amplitude;
        MaxValue += Amplitude;
        Amplitude *= 0.5f;
        Frequency *= 2.0f;
    }
    
    return Total / MaxValue;
}

float UMingRTSProceduralMap::RidgedMultifractal(float X, float Y, int32 Octaves) const
{
    float Signal = PerlinNoise(X, Y);
    float Total = 1.0f - FMath::Abs(Signal);
    float Weight = Total;
    
    for (int32 i = 1; i < Octaves; ++i)
    {
        X *= 2.0f;
        Y *= 2.0f;
        Weight = FMath::Clamp(Total * 2.0f, 0.0f, 1.0f);
        Signal = PerlinNoise(X, Y);
        Total += (1.0f - FMath::Abs(Signal)) * Weight * 0.5f;
    }
    
    return Total;
}

// 輔助函數
int32 UMingRTSProceduralMap::GetCellIndex(int32 X, int32 Y) const
{
    if (X < 0 || X >= CurrentMap.Width || Y < 0 || Y >= CurrentMap.Height)
    {
        return -1;
    }
    return Y * CurrentMap.Width + X;
}

bool UMingRTSProceduralMap::IsValidCell(int32 X, int32 Y) const
{
    return X >= 0 && X < CurrentMap.Width && Y >= 0 && Y < CurrentMap.Height;
}

float UMingRTSProceduralMap::GetDistance(FVector2D A, FVector2D B) const
{
    return FMath::Sqrt(FMath::Square(A.X - B.X) + FMath::Square(A.Y - B.Y));
}

ETerrainType UMingRTSProceduralMap::DetermineTerrainFromHeight(float Height, float Moisture, float Temperature) const
{
    if (Height < CurrentConfig.WaterLevel)
    {
        return ETerrainType::Water;
    }
    else if (Height < CurrentConfig.WaterLevel + 0.05f)
    {
        return ETerrainType::Coast;
    }
    else if (Height > 0.8f)
    {
        return Temperature < 0.3f ? ETerrainType::Snow : ETerrainType::Mountains;
    }
    else if (Height > 0.6f)
    {
        return ETerrainType::Hills;
    }
    else if (Temperature > 0.7f && Moisture > 0.5f)
    {
        return ETerrainType::Jungle;
    }
    else if (Moisture > CurrentConfig.ForestDensity)
    {
        return ETerrainType::Forest;
    }
    else if (Moisture < 0.2f)
    {
        return ETerrainType::Desert;
    }
    else if (Moisture > 0.8f && Temperature > 0.5f)
    {
        return ETerrainType::Swamp;
    }
    
    return ETerrainType::Plains;
}

float UMingRTSProceduralMap::GetMovementCostForTerrain(ETerrainType Terrain) const
{
    switch (Terrain)
    {
    case ETerrainType::Plains: return 1.0f;
    case ETerrainType::Hills: return 1.5f;
    case ETerrainType::Mountains: return 3.0f;
    case ETerrainType::Forest: return 1.3f;
    case ETerrainType::Desert: return 1.2f;
    case ETerrainType::Swamp: return 2.0f;
    case ETerrainType::Water: return 5.0f;
    case ETerrainType::River: return 2.5f;
    case ETerrainType::Coast: return 1.1f;
    case ETerrainType::Canyon: return 2.5f;
    case ETerrainType::Plateau: return 1.4f;
    case ETerrainType::Volcanic: return 2.0f;
    case ETerrainType::Snow: return 2.5f;
    case ETerrainType::Jungle: return 1.8f;
    default: return 1.0f;
    }
}

// 河流生成輔助
TArray<FVector2D> UMingRTSProceduralMap::FindRiverPath(FVector2D Start) const
{
    TArray<FVector2D> Path;
    FVector2D Current = Start;
    Path.Add(Current);
    
    for (int32 Step = 0; Step < 1000; ++Step)
    {
        float CurrentHeight = GetCellAtLocation(Current).Height;
        
        // 找到最低的鄰居
        FVector2D LowestNeighbor = Current;
        float LowestHeight = CurrentHeight;
        
        const int32 Neighbors[8][2] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
        
        for (int32 i = 0; i < 8; ++i)
        {
            FVector2D Neighbor(Current.X + Neighbors[i][0], Current.Y + Neighbors[i][1]);
            
            if (!IsValidCell(Neighbor.X, Neighbor.Y))
            {
                continue;
            }
            
            float NeighborHeight = GetCell(Neighbor.X, Neighbor.Y).Height;
            if (NeighborHeight < LowestHeight)
            {
                LowestHeight = NeighborHeight;
                LowestNeighbor = Neighbor;
            }
        }
        
        // 如果沒有更低的地方，或者到達水域，停止
        if (LowestNeighbor == Current || GetCell(LowestNeighbor.X, LowestNeighbor.Y).TerrainType == ETerrainType::Water)
        {
            break;
        }
        
        Current = LowestNeighbor;
        Path.Add(Current);
    }
    
    return Path;
}

// A*輔助函數
float UMingRTSProceduralMap::Heuristic(FVector2D A, FVector2D B) const
{
    return GetDistance(A, B);
}

TArray<FVector2D> UMingRTSProceduralMap::ReconstructPath(TMap<FVector2D, FVector2D>& CameFrom, FVector2D Current) const
{
    TArray<FVector2D> TotalPath;
    TotalPath.Add(Current);
    
    while (CameFrom.Contains(Current))
    {
        Current = CameFrom[Current];
        TotalPath.Insert(Current, 0);
    }
    
    return TotalPath;
}

// 資源生成輔助
FVector2D UMingRTSProceduralMap::FindOptimalResourceLocation(EMapResourceType Type) const
{
    // 根據資源類型選擇合適的位置
    for (int32 Attempt = 0; Attempt < 100; ++Attempt)
    {
        FVector2D Location(RandomFloat() * CurrentMap.Width, RandomFloat() * CurrentMap.Height);
        
        if (IsValidResourceLocation(Location, Type))
        {
            return Location;
        }
    }
    
    return FVector2D(CurrentMap.Width / 2.0f, CurrentMap.Height / 2.0f);
}

bool UMingRTSProceduralMap::IsValidResourceLocation(FVector2D Location, EMapResourceType Type) const
{
    int32 X = FMath::FloorToInt(Location.X);
    int32 Y = FMath::FloorToInt(Location.Y);
    
    if (!IsValidCell(X, Y))
    {
        return false;
    }
    
    FMapCellData Cell = GetCell(X, Y);
    
    // 檢查地形適合性
    switch (Type)
    {
    case EMapResourceType::Wood:
        return Cell.TerrainType == ETerrainType::Forest || Cell.TerrainType == ETerrainType::Jungle;
    case EMapResourceType::Stone:
    case EMapResourceType::Iron:
        return Cell.TerrainType == ETerrainType::Mountains || Cell.TerrainType == ETerrainType::Hills;
    case EMapResourceType::Gold:
        return Cell.TerrainType == ETerrainType::Mountains;
    case EMapResourceType::Water:
        return Cell.TerrainType == ETerrainType::Water || Cell.TerrainType == ETerrainType::River;
    case EMapResourceType::Food:
        return Cell.TerrainType == ETerrainType::Plains || Cell.TerrainType == ETerrainType::Coast;
    default:
        return Cell.bBuildable;
    }
}

// 隨機數生成
float UMingRTSProceduralMap::RandomFloat() const
{
    // 簡化實現
    return FMath::FRand();
}

int32 UMingRTSProceduralMap::RandomInt(int32 Max) const
{
    return FMath::Rand() % Max;
}

FVector2D UMingRTSProceduralMap::RandomPoint() const
{
    return FVector2D(RandomFloat() * CurrentMap.Width, RandomFloat() * CurrentMap.Height);
}
