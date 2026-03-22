#include "MingRTSPathfinder.h"
#include "MingRTSUnitManager.h"
#include "MingGoRTSUnit.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

UMingRTSPathfinder::UMingRTSPathfinder()
{
    UnitManager = nullptr;
    CurrentState = ERTSPathfindingState::Idle;
    GridCenter = FVector::ZeroVector;
    GridSize = 10000.0f;
    GridResolution = 100;
    NodeSize = GridSize / GridResolution;
    DefaultAlgorithm = ERTSPathfindingAlgorithm::AStar;
    TotalCalculationTime = 0.0f;
    CompletedRequests = 0;
}

void UMingRTSPathfinder::InitializePathfinder(UMingRTSUnitManager* InUnitManager)
{
    UnitManager = InUnitManager;
    InitializeGrid();
}

void UMingRTSPathfinder::CalculatePathAsync(const FRTSPathRequest& Request)
{
    if (PendingRequests.Num() >= MaxPendingRequests)
    {
        // 清理舊的請求
        PendingRequests.RemoveAt(0, PendingRequests.Num() - MaxPendingRequests + 1);
    }

    PendingRequests.Add(Request);
    CurrentState = ERTSPathfindingState::Calculating;
    OnPathfindingStateChanged.Broadcast(CurrentState);
}

FRTSPathResult UMingRTSPathfinder::CalculatePath(const FRTSPathRequest& Request)
{
    FRTSPathResult Result;
    double StartTime = FPlatformTime::Seconds();

    // 檢查緩存
    if (bEnablePathCache)
    {
        FString CacheKey = FString::Printf(TEXT("%s_%s_%d"), 
            *Request.StartLocation.ToString(), 
            *Request.TargetLocation.ToString(), 
            (int32)Request.Algorithm);
        
        FRTSPathResult* CachedResult = PathCache.Find(CacheKey);
        if (CachedResult)
        {
            Result = *CachedResult;
            Result.CalculationTime = FPlatformTime::Seconds() - StartTime;
            return Result;
        }
    }

    // 驗證輸入
    if (!IsValidLocation(Request.StartLocation) || !IsValidLocation(Request.TargetLocation))
    {
        Result.bPathFound = false;
        Result.CalculationTime = FPlatformTime::Seconds() - StartTime;
        return Result;
    }

    // 計算路徑
    TArray<FVector> PathPoints;
    switch (Request.Algorithm)
    {
    case ERTSPathfindingAlgorithm::AStar:
        PathPoints = CalculateAStarPath(Request.StartLocation, Request.TargetLocation);
        break;
    case ERTSPathfindingAlgorithm::Dijkstra:
        PathPoints = CalculateDijkstraPath(Request.StartLocation, Request.TargetLocation);
        break;
    case ERTSPathfindingAlgorithm::FloydWarshall:
        PathPoints = CalculateFloydWarshallPath(Request.StartLocation, Request.TargetLocation);
        break;
    default:
        PathPoints = CalculateAStarPath(Request.StartLocation, Request.TargetLocation);
        break;
    }

    // 構建結果
    Result.PathPoints = PathPoints;
    Result.bPathFound = PathPoints.Num() > 0;
    Result.AlgorithmUsed = Request.Algorithm;
    Result.CalculationTime = FPlatformTime::Seconds() - StartTime;
    Result.CompletionTime = FDateTime::Now();

    // 計算路徑長度
    if (Result.bPathFound)
    {
        for (int32 i = 0; i < PathPoints.Num() - 1; i++)
        {
            Result.PathLength += FVector::Dist(PathPoints[i], PathPoints[i + 1]);
        }
    }

    // 添加到緩存
    if (bEnablePathCache && Result.bPathFound)
    {
        if (PathCache.Num() >= MaxCacheSize)
        {
            PathCache.Empty();
        }
        
        FString CacheKey = FString::Printf(TEXT("%s_%s_%d"), 
            *Request.StartLocation.ToString(), 
            *Request.TargetLocation.ToString(), 
            (int32)Request.Algorithm);
        PathCache.Add(CacheKey, Result);
    }

    // 更新統計
    CalculationTimes.Add(Result.CalculationTime);
    TotalCalculationTime += Result.CalculationTime;
    CompletedRequests++;

    return Result;
}

TArray<FVector> UMingRTSPathfinder::FindPath(FVector Start, FVector Target, ERTSPathfindingAlgorithm Algorithm)
{
    FRTSPathRequest Request;
    Request.StartLocation = Start;
    Request.TargetLocation = Target;
    Request.Algorithm = Algorithm;

    FRTSPathResult Result = CalculatePath(Request);
    return Result.PathPoints;
}

TArray<FVector> UMingRTSPathfinder::OptimizePath(const TArray<FVector>& Path)
{
    if (Path.Num() <= 2)
    {
        return Path;
    }

    TArray<FVector> OptimizedPath;
    OptimizedPath.Add(Path[0]);

    for (int32 i = 1; i < Path.Num() - 1; i++)
    {
        FVector Current = Path[i];
        FVector Next = Path[i + 1];

        // 檢查是否可以直接從前一點到下一點
        if (OptimizedPath.Num() > 0)
        {
            FVector Previous = OptimizedPath.Last();
            if (IsLocationWalkable(Next) && !IsPathBlocked({Previous, Next}))
            {
                // 移除當前點，直接連接前一點和下一點
                continue;
            }
        }

        OptimizedPath.Add(Current);
    }

    if (Path.Num() > 0)
    {
        OptimizedPath.Add(Path.Last());
    }

    return OptimizedPath;
}

TArray<FVector> UMingRTSPathfinder::SmoothPath(const TArray<FVector>& Path, int32 SmoothingIterations)
{
    TArray<FVector> SmoothedPath = Path;

    for (int32 Iteration = 0; Iteration < SmoothingIterations; Iteration++)
    {
        for (int32 i = 1; i < SmoothedPath.Num() - 1; i++)
        {
            FVector Previous = SmoothedPath[i - 1];
            FVector Current = SmoothedPath[i];
            FVector Next = SmoothedPath[i + 1];

            // 簡單的平滑算法：取周圍點的平均值
            FVector Average = (Previous + Current + Next) / 3.0f;
            
            // 確保平均點是可行的
            if (IsLocationWalkable(Average))
            {
                SmoothedPath[i] = Average;
            }
        }
    }

    return SmoothedPath;
}

bool UMingRTSPathfinder::IsPathBlocked(const TArray<FVector>& Path)
{
    for (int32 i = 0; i < Path.Num() - 1; i++)
    {
        if (!IsLocationWalkable(Path[i]) || !IsLocationWalkable(Path[i + 1]))
        {
            return true;
        }

        // 檢查兩點之間是否有障礙物
        FVector Direction = Path[i + 1] - Path[i];
        float Distance = Direction.Size();
        Direction.Normalize();

        // 沿著路徑檢查多個點
        int32 CheckPoints = FMath::CeilToInt(Distance / NodeSize);
        for (int32 j = 1; j < CheckPoints; j++)
        {
            FVector CheckPoint = Path[i] + Direction * (j * NodeSize);
            if (!IsLocationWalkable(CheckPoint))
            {
                return true;
            }
        }
    }

    return false;
}

TArray<FVector> UMingRTSPathfinder::RecalculatePath(const TArray<FVector>& OriginalPath, FVector CurrentPosition, FVector TargetPosition)
{
    // 如果當前位置偏離原路徑太遠，重新計算整個路徑
    if (OriginalPath.Num() == 0)
    {
        return FindPath(CurrentPosition, TargetPosition);
    }

    // 找到原路徑上離當前位置最近的點
    float MinDistance = FLT_MAX;
    int32 ClosestIndex = 0;

    for (int32 i = 0; i < OriginalPath.Num(); i++)
    {
        float Distance = FVector::Dist(CurrentPosition, OriginalPath[i]);
        if (Distance < MinDistance)
        {
            MinDistance = Distance;
            ClosestIndex = i;
        }
    }

    // 如果偏離太遠，重新計算
    if (MinDistance > NodeSize * 2)
    {
        return FindPath(CurrentPosition, TargetPosition);
    }

    // 從最近點重新計算到目標的路徑
    TArray<FVector> NewPath;
    for (int32 i = ClosestIndex; i < OriginalPath.Num(); i++)
    {
        NewPath.Add(OriginalPath[i]);
    }

    return NewPath;
}

void UMingRTSPathfinder::SetTerrainType(FVector Location, ERTSTerrainType TerrainType)
{
    FRTSPathNode* Node = GetNodeAtLocation(Location);
    if (Node)
    {
        Node->TerrainType = TerrainType;
        Node->bIsWalkable = (TerrainType != ERTSTerrainType::Blocked);
    }
}

ERTSTerrainType UMingRTSPathfinder::GetTerrainType(FVector Location) const
{
    FRTSPathNode* Node = GetNodeAtLocation(Location);
    if (Node)
    {
        return Node->TerrainType;
    }
    return ERTSTerrainType::Walkable;
}

bool UMingRTSPathfinder::IsLocationWalkable(FVector Location) const
{
    FRTSPathNode* Node = GetNodeAtLocation(Location);
    if (Node)
    {
        return Node->bIsWalkable;
    }
    return true; // 默認可行走
}

void UMingRTSPathfinder::UpdateTerrainMap()
{
    // 重新掃描地形，更新網格
    ClearGrid();
    InitializeGrid();
}

void UMingRTSPathfinder::CreateNavigationGrid(FVector Center, float InGridSize, int32 GridResolution)
{
    GridCenter = Center;
    GridSize = InGridSize;
    this->GridResolution = GridResolution;
    NodeSize = GridSize / GridResolution;

    ClearGrid();
    InitializeGrid();
}

FRTSPathNode* UMingRTSPathfinder::GetNodeAtLocation(FVector Location) const
{
    // 將世界坐標轉換為網格坐標
    FVector RelativeLocation = Location - GridCenter;
    int32 X = FMath::RoundToInt(RelativeLocation.X / NodeSize);
    int32 Y = FMath::RoundToInt(RelativeLocation.Y / NodeSize);

    // 檢查邊界
    if (X < 0 || X >= GridResolution || Y < 0 || Y >= GridResolution)
    {
        return nullptr;
    }

    return NavigationGrid[X][Y];
}

TArray<FRTSPathNode*> UMingRTSPathfinder::GetNeighborNodes(FRTSPathNode* Node) const
{
    TArray<FRTSPathNode*> Neighbors;

    if (!Node)
    {
        return Neighbors;
    }

    // 找到節點在網格中的位置
    FVector RelativeLocation = Node->Position - GridCenter;
    int32 X = FMath::RoundToInt(RelativeLocation.X / NodeSize);
    int32 Y = FMath::RoundToInt(RelativeLocation.Y / NodeSize);

    // 檢查8個方向的鄰居
    for (int32 dx = -1; dx <= 1; dx++)
    {
        for (int32 dy = -1; dy <= 1; dy++)
        {
            if (dx == 0 && dy == 0)
            {
                continue; // 跳過自己
            }

            int32 NeighborX = X + dx;
            int32 NeighborY = Y + dy;

            // 檢查邊界
            if (NeighborX >= 0 && NeighborX < GridResolution && 
                NeighborY >= 0 && NeighborY < GridResolution)
            {
                FRTSPathNode* Neighbor = NavigationGrid[NeighborX][NeighborY];
                if (Neighbor && Neighbor->bIsWalkable)
                {
                    Neighbors.Add(Neighbor);
                }
            }
        }
    }

    return Neighbors;
}

float UMingRTSPathfinder::GetAverageCalculationTime() const
{
    if (CompletedRequests == 0)
    {
        return 0.0f;
    }
    return TotalCalculationTime / CompletedRequests;
}

int32 UMingRTSPathfinder::GetPendingRequestCount() const
{
    return PendingRequests.Num();
}

void UMingRTSPathfinder::ClearPathCache()
{
    PathCache.Empty();
}

void UMingRTSPathfinder::DrawDebugPath(const TArray<FVector>& Path, FLinearColor Color, float Duration)
{
    if (!bEnableDebugDrawing || Path.Num() < 2)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    for (int32 i = 0; i < Path.Num() - 1; i++)
    {
        DrawDebugLine(World, Path[i], Path[i + 1], Color.ToFColor(true), false, Duration, 0, 2.0f);
        DrawDebugSphere(World, Path[i], 20.0f, 8, Color.ToFColor(true), false, Duration);
    }

    // 繪製終點
    if (Path.Num() > 0)
    {
        DrawDebugSphere(World, Path.Last(), 30.0f, 12, Color.ToFColor(true), false, Duration);
    }
}

void UMingRTSPathfinder::DrawDebugGrid(FLinearColor Color, float Duration)
{
    if (!bEnableDebugDrawing)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    FVector Start = GridCenter - FVector(GridSize / 2, GridSize / 2, 0);

    // 繪製網格線
    for (int32 i = 0; i <= GridResolution; i++)
    {
        float Offset = i * NodeSize;
        
        // 垂直線
        FVector VerticalStart = Start + FVector(Offset, 0, 0);
        FVector VerticalEnd = Start + FVector(Offset, GridSize, 0);
        DrawDebugLine(World, VerticalStart, VerticalEnd, Color.ToFColor(true), false, Duration, 0, 1.0f);

        // 水平線
        FVector HorizontalStart = Start + FVector(0, Offset, 0);
        FVector HorizontalEnd = Start + FVector(GridSize, Offset, 0);
        DrawDebugLine(World, HorizontalStart, HorizontalEnd, Color.ToFColor(true), false, Duration, 0, 1.0f);
    }

    // 繪製不可行走的節點
    for (int32 x = 0; x < GridResolution; x++)
    {
        for (int32 y = 0; y < GridResolution; y++)
        {
            FRTSPathNode* Node = NavigationGrid[x][y];
            if (Node && !Node->bIsWalkable)
            {
                DrawDebugBox(World, Node->Position, FVector(NodeSize / 2, NodeSize / 2, 10), FRotator::ZeroRotator, FColor::Red, false, Duration);
            }
        }
    }
}

TArray<FVector> UMingRTSPathfinder::CalculateAStarPath(FVector Start, FVector Target)
{
    TArray<FVector> Path;

    FRTSPathNode* StartNode = GetNodeAtLocation(Start);
    FRTSPathNode* TargetNode = GetNodeAtLocation(Target);

    if (!StartNode || !TargetNode || !StartNode->bIsWalkable || !TargetNode->bIsWalkable)
    {
        return Path;
    }

    // A*算法實現
    TArray<FRTSPathNode*> OpenSet;
    TArray<FRTSPathNode*> ClosedSet;

    OpenSet.Add(StartNode);
    StartNode->GCost = 0.0f;
    StartNode->HCost = CalculateHeuristic(Start, Target);
    StartNode->UpdateFCost();

    while (OpenSet.Num() > 0)
    {
        // 找到F成本最低的節點
        FRTSPathNode* CurrentNode = OpenSet[0];
        int32 CurrentIndex = 0;

        for (int32 i = 1; i < OpenSet.Num(); i++)
        {
            if (OpenSet[i]->FCost < CurrentNode->FCost || 
                (OpenSet[i]->FCost == CurrentNode->FCost && OpenSet[i]->HCost < CurrentNode->HCost))
            {
                CurrentNode = OpenSet[i];
                CurrentIndex = i;
            }
        }

        OpenSet.RemoveAt(CurrentIndex);
        ClosedSet.Add(CurrentNode);

        // 到達目標
        if (CurrentNode == TargetNode)
        {
            return ReconstructPath(TargetNode);
        }

        // 檢查鄰居
        TArray<FRTSPathNode*> Neighbors = GetNeighborNodes(CurrentNode);
        for (FRTSPathNode* Neighbor : Neighbors)
        {
            if (ClosedSet.Contains(Neighbor))
            {
                continue;
            }

            float NewGCost = CurrentNode->GCost + CalculateTerrainCost(Neighbor->Position);

            if (!OpenSet.Contains(Neighbor))
            {
                OpenSet.Add(Neighbor);
            }
            else if (NewGCost >= Neighbor->GCost)
            {
                continue;
            }

            Neighbor->Parent = CurrentNode;
            Neighbor->GCost = NewGCost;
            Neighbor->HCost = CalculateHeuristic(Neighbor->Position, Target);
            Neighbor->UpdateFCost();
        }
    }

    return Path; // 沒有找到路徑
}

TArray<FVector> UMingRTSPathfinder::CalculateDijkstraPath(FVector Start, FVector Target)
{
    // Dijkstra算法實現（類似A*但不使用啟發式）
    TArray<FVector> Path;

    FRTSPathNode* StartNode = GetNodeAtLocation(Start);
    FRTSPathNode* TargetNode = GetNodeAtLocation(Target);

    if (!StartNode || !TargetNode || !StartNode->bIsWalkable || !TargetNode->bIsWalkable)
    {
        return Path;
    }

    // 初始化所有節點的距離為無窮大
    TArray<FRTSPathNode*> AllNodes;
    for (int32 x = 0; x < GridResolution; x++)
    {
        for (int32 y = 0; y < GridResolution; y++)
        {
            FRTSPathNode* Node = NavigationGrid[x][y];
            if (Node && Node->bIsWalkable)
            {
                Node->GCost = FLT_MAX;
                AllNodes.Add(Node);
            }
        }
    }

    StartNode->GCost = 0.0f;

    while (AllNodes.Num() > 0)
    {
        // 找到距離最小的節點
        FRTSPathNode* CurrentNode = AllNodes[0];
        int32 CurrentIndex = 0;

        for (int32 i = 1; i < AllNodes.Num(); i++)
        {
            if (AllNodes[i]->GCost < CurrentNode->GCost)
            {
                CurrentNode = AllNodes[i];
                CurrentIndex = i;
            }
        }

        AllNodes.RemoveAt(CurrentIndex);

        if (CurrentNode == TargetNode)
        {
            return ReconstructPath(TargetNode);
        }

        TArray<FRTSPathNode*> Neighbors = GetNeighborNodes(CurrentNode);
        for (FRTSPathNode* Neighbor : Neighbors)
        {
            if (!AllNodes.Contains(Neighbor))
            {
                continue;
            }

            float NewGCost = CurrentNode->GCost + CalculateTerrainCost(Neighbor->Position);
            if (NewGCost < Neighbor->GCost)
            {
                Neighbor->Parent = CurrentNode;
                Neighbor->GCost = NewGCost;
            }
        }
    }

    return Path;
}

TArray<FVector> UMingRTSPathfinder::CalculateFloydWarshallPath(FVector Start, FVector Target)
{
    // Floyd-Warshall算法（對於大型地圖可能較慢，但能找到最短路徑）
    // 這裡簡化實現，實際應用中可能需要預計算
    return CalculateAStarPath(Start, Target);
}

float UMingRTSPathfinder::CalculateHeuristic(FVector From, FVector To) const
{
    // 使用歐幾里得距離作為啟發式
    return FVector::Dist(From, To);
}

float UMingRTSPathfinder::CalculateTerrainCost(FVector Location) const
{
    ERTSTerrainType TerrainType = GetTerrainType(Location);
    
    switch (TerrainType)
    {
    case ERTSTerrainType::Walkable:
        return WalkableCost;
    case ERTSTerrainType::Difficult:
        return DifficultCost;
    case ERTSTerrainType::Water:
        return WaterCost;
    case ERTSTerrainType::Mountain:
        return MountainCost;
    case ERTSTerrainType::Forest:
        return ForestCost;
    case ERTSTerrainType::Blocked:
        return FLT_MAX;
    default:
        return WalkableCost;
    }
}

bool UMingRTSPathfinder::IsValidLocation(FVector Location) const
{
    // 檢查位置是否在網格範圍內
    FVector RelativeLocation = Location - GridCenter;
    float HalfGridSize = GridSize / 2.0f;

    return (FMath::Abs(RelativeLocation.X) <= HalfGridSize && 
            FMath::Abs(RelativeLocation.Y) <= HalfGridSize);
}

TArray<FVector> UMingRTSPathfinder::ReconstructPath(FRTSPathNode* EndNode) const
{
    TArray<FVector> Path;
    
    if (!EndNode)
    {
        return Path;
    }

    FRTSPathNode* CurrentNode = EndNode;
    while (CurrentNode)
    {
        Path.Insert(CurrentNode->Position, 0);
        CurrentNode = CurrentNode->Parent;
    }

    return Path;
}

void UMingRTSPathfinder::InitializeGrid()
{
    NavigationGrid.SetNum(GridResolution);
    for (int32 x = 0; x < GridResolution; x++)
    {
        NavigationGrid[x].SetNum(GridResolution);
        for (int32 y = 0; y < GridResolution; y++)
        {
            FVector NodeLocation = GridCenter + FVector(
                (x - GridResolution / 2) * NodeSize,
                (y - GridResolution / 2) * NodeSize,
                0
            );

            // 默認所有節點都可行走
            NavigationGrid[x][y] = CreateNode(NodeLocation, true, ERTSTerrainType::Walkable);
        }
    }
}

void UMingRTSPathfinder::ClearGrid()
{
    for (int32 x = 0; x < NavigationGrid.Num(); x++)
    {
        for (int32 y = 0; y < NavigationGrid[x].Num(); y++)
        {
            if (NavigationGrid[x][y])
            {
                delete NavigationGrid[x][y];
                NavigationGrid[x][y] = nullptr;
            }
        }
        NavigationGrid[x].Empty();
    }
    NavigationGrid.Empty();
}

FRTSPathNode* UMingRTSPathfinder::CreateNode(FVector Location, bool bIsWalkable, ERTSTerrainType TerrainType)
{
    FRTSPathNode* Node = new FRTSPathNode(Location, bIsWalkable, TerrainType);
    return Node;
}

void UMingRTSPathfinder::ProcessPendingRequests()
{
    while (PendingRequests.Num() > 0)
    {
        FRTSPathRequest Request = PendingRequests[0];
        PendingRequests.RemoveAt(0);

        FRTSPathResult Result = CalculatePath(Request);
        CompletePathRequest(Request, Result);
    }

    CurrentState = ERTSPathfindingState::Idle;
    OnPathfindingStateChanged.Broadcast(CurrentState);
}

void UMingRTSPathfinder::CompletePathRequest(const FRTSPathRequest& Request, const FRTSPathResult& Result)
{
    OnPathCalculated.Broadcast(Request, Result);
}
