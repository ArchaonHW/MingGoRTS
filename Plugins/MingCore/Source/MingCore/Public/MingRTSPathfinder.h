#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSPathfinder.generated.h"

// 前向聲明
class AMingGoRTSUnit;
class UMingRTSUnitManager;

UENUM(BlueprintType)
enum class ERTSPathfindingAlgorithm : uint8
{
    AStar,          // A*算法
    Dijkstra,       // Dijkstra算法
    FloydWarshall,  // Floyd-Warshall算法
    Custom          // 自定義算法
};

UENUM(BlueprintType)
enum class ERTSTerrainType : uint8
{
    Walkable,       // 可行走
    Blocked,        // 阻擋
    Difficult,      // 困難地形
    Water,          // 水域
    Mountain,       // 山地
    Forest          // 森林
};

UENUM(BlueprintType)
enum class ERTSPathfindingState : uint8
{
    Idle,           // 空閒
    Calculating,    // 計算中
    Ready,          // 就緒
    Failed          // 失敗
};

USTRUCT(BlueprintType)
struct FRTSPathNode
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Path Node")
    FVector Position;

    UPROPERTY(BlueprintReadOnly, Category = "Path Node")
    float GCost;        // 從起點到當前節點的實際成本

    UPROPERTY(BlueprintReadOnly, Category = "Path Node")
    float HCost;        // 從當前節點到終點的預估成本

    UPROPERTY(BlueprintReadOnly, Category = "Path Node")
    float FCost;        // GCost + HCost

    // 注意：FRTSPathNode 是指標類型，不適合 UPROPERTY
    FRTSPathNode* Parent;

    UPROPERTY(BlueprintReadOnly, Category = "Path Node")
    bool bIsWalkable;

    UPROPERTY(BlueprintReadOnly, Category = "Path Node")
    ERTSTerrainType TerrainType;

    FRTSPathNode()
    {
        Position = FVector::ZeroVector;
        GCost = 0.0f;
        HCost = 0.0f;
        FCost = 0.0f;
        Parent = nullptr;
        bIsWalkable = true;
        TerrainType = ERTSTerrainType::Walkable;
    }

    FRTSPathNode(FVector InPosition, bool bInIsWalkable = true, ERTSTerrainType InTerrainType = ERTSTerrainType::Walkable)
    {
        Position = InPosition;
        GCost = 0.0f;
        HCost = 0.0f;
        FCost = 0.0f;
        Parent = nullptr;
        bIsWalkable = bInIsWalkable;
        TerrainType = InTerrainType;
    }

    void UpdateFCost()
    {
        FCost = GCost + HCost;
    }
};

USTRUCT(BlueprintType)
struct FRTSPathRequest
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Path Request")
    FVector StartLocation;

    UPROPERTY(BlueprintReadOnly, Category = "Path Request")
    FVector TargetLocation;

    UPROPERTY(BlueprintReadOnly, Category = "Path Request")
    TObjectPtr<AMingGoRTSUnit> RequestingUnit;

    UPROPERTY(BlueprintReadOnly, Category = "Path Request")
    ERTSPathfindingAlgorithm Algorithm;

    UPROPERTY(BlueprintReadOnly, Category = "Path Request")
    float MaxPathLength;

    UPROPERTY(BlueprintReadOnly, Category = "Path Request")
    bool bAllowPartialPath;

    UPROPERTY(BlueprintReadOnly, Category = "Path Request")
    FDateTime RequestTime;

    FRTSPathRequest()
    {
        StartLocation = FVector::ZeroVector;
        TargetLocation = FVector::ZeroVector;
        RequestingUnit = nullptr;
        Algorithm = ERTSPathfindingAlgorithm::AStar;
        MaxPathLength = 10000.0f;
        bAllowPartialPath = true;
        RequestTime = FDateTime::Now();
    }
};

USTRUCT(BlueprintType)
struct FRTSPathResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Path Result")
    TArray<FVector> PathPoints;

    UPROPERTY(BlueprintReadOnly, Category = "Path Result")
    bool bPathFound;

    UPROPERTY(BlueprintReadOnly, Category = "Path Result")
    float PathLength;

    UPROPERTY(BlueprintReadOnly, Category = "Path Result")
    float CalculationTime;

    UPROPERTY(BlueprintReadOnly, Category = "Path Result")
    ERTSPathfindingAlgorithm AlgorithmUsed;

    UPROPERTY(BlueprintReadOnly, Category = "Path Result")
    FDateTime CompletionTime;

    FRTSPathResult()
    {
        bPathFound = false;
        PathLength = 0.0f;
        CalculationTime = 0.0f;
        AlgorithmUsed = ERTSPathfindingAlgorithm::AStar;
        CompletionTime = FDateTime::Now();
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPathCalculated, const FRTSPathRequest&, Request, const FRTSPathResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPathfindingStateChanged, ERTSPathfindingState, NewState);

/**
 * RTS路徑規劃器
 * 提供多種尋路算法和動態避障功能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSPathfinder : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSPathfinder();

    // 初始化
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void InitializePathfinder(UMingRTSUnitManager* InUnitManager);

    // 路徑計算
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void CalculatePathAsync(const FRTSPathRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    FRTSPathResult CalculatePath(const FRTSPathRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    TArray<FVector> FindPath(FVector Start, FVector Target, ERTSPathfindingAlgorithm Algorithm = ERTSPathfindingAlgorithm::AStar);

    // 路徑優化
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    TArray<FVector> OptimizePath(const TArray<FVector>& Path);

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    TArray<FVector> SmoothPath(const TArray<FVector>& Path, int32 SmoothingIterations = 3);

    // 動態避障
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    bool IsPathBlocked(const TArray<FVector>& Path);

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    TArray<FVector> RecalculatePath(const TArray<FVector>& OriginalPath, FVector CurrentPosition, FVector TargetPosition);

    // 地形系統
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void SetTerrainType(FVector Location, ERTSTerrainType TerrainType);

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    ERTSTerrainType GetTerrainType(FVector Location) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    bool IsLocationWalkable(FVector Location) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void UpdateTerrainMap();

    // 網格系統
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void CreateNavigationGrid(FVector Center, float GridSize, int32 GridResolution);

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    FRTSPathNode* GetNodeAtLocation(FVector Location) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    TArray<FRTSPathNode*> GetNeighborNodes(FRTSPathNode* Node) const;

    // 性能監控
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    float GetAverageCalculationTime() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    int32 GetPendingRequestCount() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void ClearPathCache();

    // 調試功能
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void DrawDebugPath(const TArray<FVector>& Path, FLinearColor Color = FLinearColor::Green, float Duration = 5.0f);

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void DrawDebugGrid(FLinearColor Color = FLinearColor::White, float Duration = 10.0f);

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "RTS Pathfinder")
    FOnPathCalculated OnPathCalculated;

    UPROPERTY(BlueprintAssignable, Category = "RTS Pathfinder")
    FOnPathfindingStateChanged OnPathfindingStateChanged;

protected:
    // 核心算法
    TArray<FVector> CalculateAStarPath(FVector Start, FVector Target);
    TArray<FVector> CalculateDijkstraPath(FVector Start, FVector Target);
    TArray<FVector> CalculateFloydWarshallPath(FVector Start, FVector Target);

    // 輔助方法
    float CalculateHeuristic(FVector From, FVector To) const;
    float CalculateTerrainCost(FVector Location) const;
    bool IsValidLocation(FVector Location) const;
    TArray<FVector> ReconstructPath(FRTSPathNode* EndNode) const;

    // 網格管理
    void InitializeGrid();
    void ClearGrid();
    FRTSPathNode* CreateNode(FVector Location, bool bIsWalkable = true, ERTSTerrainType TerrainType = ERTSTerrainType::Walkable);

    // 異步處理
    void ProcessPendingRequests();
    void CompletePathRequest(const FRTSPathRequest& Request, const FRTSPathResult& Result);

protected:
    // 核心組件
    UPROPERTY()
    TObjectPtr<UMingRTSUnitManager> UnitManager;

    // 路徑計算
    UPROPERTY()
    TArray<FRTSPathRequest> PendingRequests;

    UPROPERTY()
    TMap<FString, FRTSPathResult> PathCache;

    // 網格系統
    // 注意：指標陣列不適合 UPROPERTY
    TArray<TArray<FRTSPathNode*>> NavigationGrid;

    UPROPERTY()
    FVector GridCenter;

    UPROPERTY()
    float GridSize;

    UPROPERTY()
    int32 GridResolution;

    UPROPERTY()
    float NodeSize;

    // 狀態
    UPROPERTY(BlueprintReadOnly, Category = "Pathfinding State")
    ERTSPathfindingState CurrentState;

    // 性能統計
    UPROPERTY()
    TArray<float> CalculationTimes;

    UPROPERTY()
    float TotalCalculationTime;

    UPROPERTY()
    int32 CompletedRequests;

    // 配置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding Config")
    ERTSPathfindingAlgorithm DefaultAlgorithm;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding Config")
    float MaxCalculationTime = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding Config")
    int32 MaxPendingRequests = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding Config")
    bool bEnablePathCache = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding Config")
    int32 MaxCacheSize = 1000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding Config")
    bool bEnableDebugDrawing = false;

    // 地形成本
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Costs")
    float WalkableCost = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Costs")
    float DifficultCost = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Costs")
    float WaterCost = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Costs")
    float MountainCost = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Costs")
    float ForestCost = 2.5f;
};
