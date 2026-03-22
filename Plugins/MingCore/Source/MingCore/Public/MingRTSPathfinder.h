#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSPathfinder.generated.h"

// X
class AMingGoRTSUnit;
class UMingRTSUnitManager;

UENUM(BlueprintType)
enum class ERTSPathfindingAlgorithm: uint8 {
    AStar,          // A*??X
    Dijkstra,       // Dijkstra??X
    FloydWarshall,  // Floyd-Warshall??X
    Custom          // ??w?q??k
};

UENUM(BlueprintType)
enum class ERTSTerrainType: uint8 {
    Walkable,       // ?i??
    Blocked,        // 摧毀X
    Difficult,      // X??X??
    Water,          // ??X
    Mountain,       // ?s?a
    Forest          // ??X
};

UENUM(BlueprintType)
enum class ERTSPathfindingState: uint8 {
    Idle,           // ??X
    Calculating,    // ?pX
    Ready,          // ?NX
    Failed          // ??X
};

USTRUCT(BlueprintType)
struct FRTSPathNode
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Path Node")
    FVector Position;

    UPROPERTY(BlueprintReadOnly, Category = "Path Node")
    float GCost;        // ?q?_?I??X?`?IX??X??

    UPROPERTY(BlueprintReadOnly, Category = "Path Node")
    float HCost;        // ?q??X?I故事重要性
    UPROPERTY(BlueprintReadOnly, Category = "Path Node")
    float FCost;        // GCost + HCost

    
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




/**
 * RTS??X?WX * X?h??M故事重要性 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSPathfinder : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSPathfinder();

    
    void InitializePathfinder(UMingRTSUnitManager* InUnitManager);

    // ??X?pX
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void CalculatePathAsync(const FRTSPathRequest& Request) {};

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    FRTSPathResult CalculatePath(const FRTSPathRequest& Request) {};

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    TArray<FVector> FindPath(FVector Start, FVector Target, ERTSPathfindingAlgorithm Algorithm = ERTSPathfindingAlgorithm::AStar);

    // ??X
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    TArray<FVector> OptimizePath(const TArray<FVector>& Path);

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    TArray<FVector> SmoothPath(const TArray<FVector>& Path, int32 SmoothingIterations = 3);

    // X
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    bool IsPathBlocked(const TArray<FVector>& Path);

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    TArray<FVector> RecalculatePath(const TArray<FVector>& OriginalPath, FVector CurrentPosition, FVector TargetPosition);

    // X?�Zt??
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void SetTerrainType(FVector Location, ERTSTerrainType TerrainType);

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    ERTSTerrainType GetTerrainType(FVector Location) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    bool IsLocationWalkable(FVector Location) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void UpdateTerrainMap();

    // ???t??
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void CreateNavigationGrid(FVector Center, float GridSize, int32 GridResolution);

    
    FRTSPathNode* GetNodeAtLocation(FVector Location) const;

    
    TArray<FRTSPathNode*> GetNeighborNodes(FRTSPathNode* Node) const;

    // X??X
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    float GetAverageCalculationTime() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    int32 GetPendingRequestCount() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void ClearPathCache();

    // ???X??
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void DrawDebugPath(const TArray<FVector>& Path, FLinearColor Color = FLinearColor::Green, float Duration = 5.0f);

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void DrawDebugGrid(FLinearColor Color = FLinearColor::White, float Duration = 10.0f);

    // ???eX
    UPROPERTY(BlueprintAssignable, Category = "RTS Pathfinder")
    FOnPathCalculated OnPathCalculated;

    UPROPERTY(BlueprintAssignable, Category = "RTS Pathfinder")
    FOnPathfindingStateChanged OnPathfindingStateChanged;

protected:
    // X??X
    TArray<FVector> CalculateAStarPath(FVector Start, FVector Target);
    TArray<FVector> CalculateDijkstraPath(FVector Start, FVector Target);
    TArray<FVector> CalculateFloydWarshallPath(FVector Start, FVector Target);

    // ???UX
    float CalculateHeuristic(FVector From, FVector To) const;
    float CalculateTerrainCost(FVector Location) const;
    bool IsValidLocation(FVector Location) const;
    TArray<FVector> ReconstructPath(FRTSPathNode* EndNode) const;

    // 摧毀X
    void InitializeGrid();
    void ClearGrid();
    FRTSPathNode* CreateNode(FVector Location, bool bIsWalkable = true, ERTSTerrainType TerrainType = ERTSTerrainType::Walkable);

    // X?BX
    void ProcessPendingRequests();
    void CompletePathRequest(const FRTSPathRequest& Request, const FRTSPathResult& Result) {};

protected:
    // X???
    UPROPERTY()
    TObjectPtr<UMingRTSUnitManager> UnitManager;

    // ??X?pX
    UPROPERTY()
    TArray<FRTSPathRequest> PendingRequests;

    UPROPERTY()
    TMap<FString, FRTSPathResult> PathCache;

    // ???t??
    
    TArray<TArray<FRTSPathNode*>> NavigationGrid;

    UPROPERTY()
    FVector GridCenter;

    UPROPERTY()
    float GridSize;

    UPROPERTY()
    int32 GridResolution;

    UPROPERTY()
    float NodeSize;

    
    ERTSPathfindingState CurrentState;

    // X???X
    UPROPERTY()
    TArray<float> CalculationTimes;

    UPROPERTY()
    float TotalCalculationTime;

    UPROPERTY()
    int32 CompletedRequests;

    // X?m
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

    // X??X??
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

