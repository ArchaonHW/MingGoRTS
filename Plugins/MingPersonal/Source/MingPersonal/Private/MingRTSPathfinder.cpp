#include "MingRTSPathfinder.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"

UMingRTSPathfinder::UMingRTSPathfinder()
{
    CurrentWorld = nullptr;
    CurrentAlgorithm = EPathfindingAlgorithm::AStar;
    GridSize = 100.0f;
    MaxPathfindingTime = 50.0f; // 50ms
    bIsPathfinding = false;
    ObstacleCheckRadius = 50.0f;
    PathSmoothingStrength = 0.5f;
    MaxPathNodes = 1000;
}

void UMingRTSPathfinder::InitializePathfinder(UWorld* World)
{
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSPathfinder: Invalid world provided"));
        return;
    }

    CurrentWorld = World;
    CleanupPathGrid();
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSPathfinder initialized successfully"));
}

void UMingRTSPathfinder::FindPathAsync(const FPathfindingRequest& Request)
{
    if (bIsPathfinding)
    {
        UE_LOG(LogTemp, Warning, TEXT("Pathfinding already in progress"));
        return;
    }

    if (!CurrentWorld)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot find path without valid world"));
        return;
    }

    CurrentRequest = Request;
    bIsPathfinding = true;
    PathfindingStartTime = FDateTime::Now().ToUnixTimestamp() * 1000.0 + FDateTime::Now().GetMillisecond();

    // 在後台線程執行路徑尋找
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this]()
    {
        ExecutePathfinding();
    });
}

FPathfindingResult UMingRTSPathfinder::FindPathSync(const FPathfindingRequest& Request)
{
    FPathfindingResult Result;
    
    if (!CurrentWorld)
    {
        Result.ErrorMessage = TEXT("No valid world available");
        return Result;
    }

    double StartTime = FDateTime::Now().ToUnixTimestamp() * 1000.0 + FDateTime::Now().GetMillisecond();

    // 根據算法選擇路徑尋找方法
    switch (Request.Algorithm)
    {
        case EPathfindingAlgorithm::AStar:
            Result = FindPathAStar(Request);
            break;
        
        case EPathfindingAlgorithm::Dijkstra:
            Result = FindPathDijkstra(Request);
            break;
        
        case EPathfindingAlgorithm::Custom:
            Result = FindPathCustom(Request);
            break;
        
        default:
            Result = FindPathAStar(Request);
            break;
    }

    double EndTime = FDateTime::Now().ToUnixTimestamp() * 1000.0 + FDateTime::Now().GetMillisecond();
    Result.PathfindingTime = EndTime - StartTime;

    // 如果路徑過長，返回部分路徑
    if (Result.TotalPathLength > Request.MaxPathLength && Request.bAllowPartialPath)
    {
        // 截斷路徑到最大長度
        float CurrentLength = 0.0f;
        TArray<FVector> PartialPath;
        
        for (int32 i = 0; i < Result.PathPoints.Num() - 1; i++)
        {
            float SegmentLength = FVector::Dist(Result.PathPoints[i], Result.PathPoints[i + 1]);
            if (CurrentLength + SegmentLength > Request.MaxPathLength)
            {
                // 添加最後一個點並停止
                float Ratio = (Request.MaxPathLength - CurrentLength) / SegmentLength;
                FVector LastPoint = Result.PathPoints[i] + (Result.PathPoints[i + 1] - Result.PathPoints[i]) * Ratio;
                PartialPath.Add(LastPoint);
                break;
            }
            
            PartialPath.Add(Result.PathPoints[i]);
            CurrentLength += SegmentLength;
        }
        
        Result.PathPoints = PartialPath;
        Result.TotalPathLength = CurrentLength;
        Result.ErrorMessage = TEXT("Path truncated to maximum length");
    }

    return Result;
}

FPathfindingResult UMingRTSPathfinder::GetSimplePath(FVector Start, FVector End)
{
    FPathfindingResult Result;
    
    if (!CurrentWorld)
    {
        Result.ErrorMessage = TEXT("No valid world available");
        return Result;
    }

    // 檢查直線路徑是否可通行
    if (IsLineWalkable(Start, End))
    {
        Result.bPathFound = true;
        Result.PathPoints.Add(Start);
        Result.PathPoints.Add(End);
        Result.TotalPathLength = FVector::Dist(Start, End);
    }
    else
    {
        Result.bPathFound = false;
        Result.ErrorMessage = TEXT("Direct path is blocked");
    }

    return Result;
}

bool UMingRTSPathfinder::IsPathValid(const TArray<FVector>& PathPoints) const
{
    if (PathPoints.Num() < 2)
    {
        return false;
    }

    for (int32 i = 0; i < PathPoints.Num() - 1; i++)
    {
        if (!IsLineWalkable(PathPoints[i], PathPoints[i + 1]))
        {
            return false;
        }
    }

    return true;
}

TArray<FVector> UMingRTSPathfinder::SmoothPath(const TArray<FVector>& PathPoints) const
{
    if (PathPoints.Num() < 3)
    {
        return PathPoints;
    }

    TArray<FVector> SmoothedPath;
    SmoothedPath.Add(PathPoints[0]);

    for (int32 i = 1; i < PathPoints.Num() - 1; i++)
    {
        FVector PrevPoint = PathPoints[i - 1];
        FVector CurrentPoint = PathPoints[i];
        FVector NextPoint = PathPoints[i + 1];

        // 計算平滑後的位置
        FVector SmoothedPoint = CurrentPoint * (1.0f - PathSmoothingStrength) +
                                (PrevPoint + NextPoint) * 0.5f * PathSmoothingStrength;

        // 確保平滑後的位置是可通行的
        if (IsPositionWalkable(SmoothedPoint))
        {
            SmoothedPath.Add(SmoothedPoint);
        }
        else
        {
            SmoothedPath.Add(CurrentPoint);
        }
    }

    SmoothedPath.Add(PathPoints.Last());
    return SmoothedPath;
}

TArray<FVector> UMingRTSPathfinder::OptimizePath(const TArray<FVector>& PathPoints) const
{
    if (PathPoints.Num() < 3)
    {
        return PathPoints;
    }

    TArray<FVector> OptimizedPath;
    OptimizedPath.Add(PathPoints[0]);

    int32 CurrentIndex = 0;
    
    while (CurrentIndex < PathPoints.Num() - 1)
    {
        int32 FarthestIndex = CurrentIndex + 1;
        
        // 尋找最遠的可直線到達的點
        for (int32 i = PathPoints.Num() - 1; i > CurrentIndex + 1; i--)
        {
            if (IsLineWalkable(PathPoints[CurrentIndex], PathPoints[i]))
            {
                FarthestIndex = i;
                break;
            }
        }
        
        OptimizedPath.Add(PathPoints[FarthestIndex]);
        CurrentIndex = FarthestIndex;
    }

    return OptimizedPath;
}

bool UMingRTSPathfinder::IsPositionWalkable(FVector Position) const
{
    if (!CurrentWorld)
    {
        return false;
    }

    // 檢查位置是否在地面上
    FVector TraceStart = Position + FVector(0, 0, 100);
    FVector TraceEnd = Position - FVector(0, 0, 200);
    
    FHitResult HitResult;
    if (CurrentWorld->LineTraceSingleByChannel(
        HitResult, 
        TraceStart, 
        TraceEnd, 
        ECC_WorldStatic))
    {
        // 檢查地面角度是否可通行
        if (HitResult.Normal.Z < 0.7f) // 地面角度大於45度
        {
            return false;
        }
    }
    else
    {
        return false; // 沒有地面
    }

    // 檢查周圍是否有障礙物
    TArray<FHitResult> HitResults;
    FCollisionShape CollisionShape;
    CollisionShape.SetSphere(ObstacleCheckRadius);
    
    if (CurrentWorld->SweepMultiByChannel(
        HitResults, 
        Position, 
        Position, 
        FQuat::Identity, 
        ECC_WorldStatic, 
        CollisionShape))
    {
        for (const FHitResult& Hit : HitResults)
        {
            // 檢查是否是障礙物（根據需要添加更多條件）
            if (Hit.GetComponent() && Hit.GetComponent()->GetCollisionResponseToChannel(ECC_Pawn) == ECR_Block)
            {
                return false;
            }
        }
    }

    return true;
}

FVector UMingRTSPathfinder::GetNearestWalkablePosition(FVector Position) const
{
    if (!CurrentWorld)
    {
        return Position;
    }

    // 如果位置本身可通行，直接返回
    if (IsPositionWalkable(Position))
    {
        return Position;
    }

    // 在周圍搜索可通行位置
    float SearchRadius = GridSize * 2.0f;
    float StepSize = GridSize * 0.5f;
    
    for (float Radius = StepSize; Radius <= SearchRadius; Radius += StepSize)
    {
        for (float Angle = 0.0f; Angle < 360.0f; Angle += 45.0f)
        {
            FVector TestPosition = Position + FVector(
                FMath::Cos(Angle * PI / 180.0f) * Radius,
                FMath::Sin(Angle * PI / 180.0f) * Radius,
                0.0f
            );
            
            if (IsPositionWalkable(TestPosition))
            {
                return TestPosition;
            }
        }
    }

    // 如果找不到，返回原始位置
    return Position;
}

float UMingRTSPathfinder::CalculatePathLength(const TArray<FVector>& PathPoints) const
{
    if (PathPoints.Num() < 2)
    {
        return 0.0f;
    }

    float TotalLength = 0.0f;
    for (int32 i = 0; i < PathPoints.Num() - 1; i++)
    {
        TotalLength += FVector::Dist(PathPoints[i], PathPoints[i + 1]);
    }

    return TotalLength;
}

void UMingRTSPathfinder::CancelCurrentPathfinding()
{
    if (bIsPathfinding)
    {
        bIsPathfinding = false;
        UE_LOG(LogTemp, Log, TEXT("Pathfinding cancelled"));
    }
}

FPathfindingResult UMingRTSPathfinder::FindPathAStar(const FPathfindingRequest& Request)
{
    FPathfindingResult Result;
    
    // 使用 UE 內建的導航系統
    if (CurrentWorld)
    {
        UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(CurrentWorld);
        if (NavSystem)
        {
            FNavPathSharedPtr NavPath;
            FNavAgentProperties AgentProperties;
            
            // 設置代理屬性（根據單位類型調整）
            AgentProperties.AgentRadius = 30.0f;
            AgentProperties.AgentHeight = 100.0f;
            
            bool bPathFound = NavSystem->FindPathToLocationSynchronously(
                CurrentWorld,
                Request.StartPosition,
                Request.TargetPosition,
                nullptr,
                AgentProperties,
                NavPath
            );

            if (bPathFound && NavPath.IsValid())
            {
                Result.bPathFound = true;
                Result.PathPoints = NavPath->GetPathPoints();
                Result.TotalPathLength = NavPath->GetPathLength();
                
                // 應用路徑優化
                Result.PathPoints = OptimizePath(Result.PathPoints);
                Result.PathPoints = SmoothPath(Result.PathPoints);
                Result.TotalPathLength = CalculatePathLength(Result.PathPoints);
            }
            else
            {
                Result.ErrorMessage = TEXT("Navigation system could not find path");
            }
        }
        else
        {
            Result.ErrorMessage = TEXT("Navigation system not available");
        }
    }
    else
    {
        Result.ErrorMessage = TEXT("No valid world available");
    }

    return Result;
}

FPathfindingResult UMingRTSPathfinder::FindPathDijkstra(const FPathfindingRequest& Request)
{
    // Dijkstra 算法實現（簡化版，實際項目中可能需要更完整的實現）
    // 這裡使用 A* 的結果作為 Dijkstra 的近似結果
    return FindPathAStar(Request);
}

FPathfindingResult UMingRTSPathfinder::FindPathCustom(const FPathfindingRequest& Request)
{
    FPathfindingResult Result;
    
    // 自定義算法：結合直線路徑和導航系統
    FPathfindingResult DirectPath = GetSimplePath(Request.StartPosition, Request.TargetPosition);
    
    if (DirectPath.bPathFound)
    {
        Result = DirectPath;
    }
    else
    {
        // 如果直線路徑不通，使用導航系統
        Result = FindPathAStar(Request);
    }

    return Result;
}

FPathfindingNode* UMingRTSPathfinder::CreatePathNode(FVector Position)
{
    FPathfindingNode* Node = new FPathfindingNode();
    Node->Position = Position;
    Node->bIsWalkable = IsPositionWalkable(Position);
    
    PathNodePool.Add(Node);
    return Node;
}

TArray<FPathfindingNode*> UMingRTSPathfinder::GetNodeNeighbors(FPathfindingNode* Node)
{
    TArray<FPathfindingNode*> Neighbors;
    
    if (!Node)
    {
        return Neighbors;
    }

    // 獲取 8 方向的鄰居節點
    TArray<FVector2D> Directions = {
        FVector2D(1, 0), FVector2D(-1, 0), FVector2D(0, 1), FVector2D(0, -1),
        FVector2D(1, 1), FVector2D(-1, 1), FVector2D(1, -1), FVector2D(-1, -1)
    };

    for (const FVector2D& Dir : Directions)
    {
        FVector NeighborPos = Node->Position + FVector(Dir.X * GridSize, Dir.Y * GridSize, 0);
        
        if (IsPositionWalkable(NeighborPos))
        {
            FPathfindingNode* NeighborNode = CreatePathNode(NeighborPos);
            Neighbors.Add(NeighborNode);
        }
    }

    return Neighbors;
}

float UMingRTSPathfinder::CalculateHeuristicCost(FVector From, FVector To) const
{
    // 使用歐幾里得距離作為啟發式函數
    return FVector::Dist(From, To);
}

bool UMingRTSPathfinder::IsLineWalkable(FVector Start, FVector End) const
{
    if (!CurrentWorld)
    {
        return false;
    }

    // 執行線段檢測
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActors(TArray<AActor*>()); // 根據需要添加忽略的Actor
    
    return !CurrentWorld->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_WorldStatic,
        QueryParams
    );
}

TArray<FVector> UMingRTSPathfinder::ReconstructPath(FPathfindingNode* EndNode) const
{
    TArray<FVector> Path;
    
    if (!EndNode)
    {
        return Path;
    }

    FPathfindingNode* CurrentNode = EndNode;
    while (CurrentNode)
    {
        Path.Insert(CurrentNode->Position, 0);
        CurrentNode = CurrentNode->Parent;
    }

    return Path;
}

void UMingRTSPathfinder::ExecutePathfinding()
{
    if (!bIsPathfinding)
    {
        return;
    }

    FPathfindingResult Result = FindPathSync(CurrentRequest);
    
    // 在主線程中觸發事件
    AsyncTask(ENamedThreads::GameThread, [this, Result]()
    {
        OnPathfindingCompleted.Broadcast(Result);
        bIsPathfinding = false;
    });
}

void UMingRTSPathfinder::CleanupPathGrid()
{
    // 清理路徑節點池
    for (FPathfindingNode* Node : PathNodePool)
    {
        delete Node;
    }
    PathNodePool.Empty();
}

FVector UMingRTSPathfinder::GridToWorld(int32 X, int32 Y) const
{
    return FVector(X * GridSize, Y * GridSize, 0);
}

void UMingRTSPathfinder::WorldToGrid(FVector WorldPos, int32& X, int32& Y) const
{
    X = FMath::RoundToInt(WorldPos.X / GridSize);
    Y = FMath::RoundToInt(WorldPos.Y / GridSize);
}

FPathfindingNode* UMingRTSPathfinder::GetGridNode(int32 X, int32 Y)
{
    // 簡化實現，實際項目中可能需要更複雜的網格管理
    FVector WorldPos = GridToWorld(X, Y);
    return CreatePathNode(WorldPos);
}

bool UMingRTSPathfinder::IsGridNodeWalkable(int32 X, int32 Y) const
{
    FVector WorldPos = GridToWorld(X, Y);
    return IsPositionWalkable(WorldPos);
}

bool UMingRTSPathfinder::LineTraceObstacle(FVector Start, FVector End) const
{
    return !IsLineWalkable(Start, End);
}

TArray<FVector> UMingRTSPathfinder::InterpolatePath(const TArray<FVector>& PathPoints) const
{
    if (PathPoints.Num() < 2)
    {
        return PathPoints;
    }

    TArray<FVector> InterpolatedPath;
    InterpolatedPath.Add(PathPoints[0]);

    for (int32 i = 0; i < PathPoints.Num() - 1; i++)
    {
        FVector Start = PathPoints[i];
        FVector End = PathPoints[i + 1];
        float Distance = FVector::Dist(Start, End);
        
        // 根據距離插值點
        int32 InterpolationPoints = FMath::RoundToInt(Distance / (GridSize * 0.5f));
        
        for (int32 j = 1; j < InterpolationPoints; j++)
        {
            float Alpha = (float)j / InterpolationPoints;
            FVector InterpolatedPoint = FMath::Lerp(Start, End, Alpha);
            InterpolatedPath.Add(InterpolatedPoint);
        }
        
        InterpolatedPath.Add(End);
    }

    return InterpolatedPath;
}

TArray<FVector> UMingRTSPathfinder::RemoveRedundantPoints(const TArray<FVector>& PathPoints) const
{
    return OptimizePath(PathPoints);
}

float UMingRTSPathfinder::CalculatePathCurvature(const TArray<FVector>& PathPoints) const
{
    if (PathPoints.Num() < 3)
    {
        return 0.0f;
    }

    float TotalCurvature = 0.0f;
    
    for (int32 i = 1; i < PathPoints.Num() - 1; i++)
    {
        FVector Prev = PathPoints[i - 1];
        FVector Current = PathPoints[i];
        FVector Next = PathPoints[i + 1];
        
        FVector Dir1 = (Current - Prev).GetSafeNormal();
        FVector Dir2 = (Next - Current).GetSafeNormal();
        
        float DotProduct = FVector::DotProduct(Dir1, Dir2);
        float Angle = FMath::Acos(FMath::Clamp(DotProduct, -1.0f, 1.0f));
        
        TotalCurvature += Angle;
    }

    return TotalCurvature;
}

TArray<FVector> UMingRTSPathfinder::ApplyDynamicAvoidance(const TArray<FVector>& PathPoints, AActor* Unit) const
{
    // 動態避障實現（簡化版）
    // 實際項目中需要考慮其他單位的位置和移動方向
    return PathPoints;
}
