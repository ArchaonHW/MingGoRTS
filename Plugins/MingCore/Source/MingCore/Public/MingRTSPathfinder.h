#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSPathfinder.generated.h"

// ?çÂ??≤Ê?
class AMingGoRTSUnit;
class UMingRTSUnitManager;

UENUM(BlueprintType)
enum class ERTSPathfindingAlgorithm : uint8
{
    AStar,          // A*ÁÆóÊ?
    Dijkstra,       // DijkstraÁÆóÊ?
    FloydWarshall,  // Floyd-WarshallÁÆóÊ?
    Custom          // ?™Â?Áæ©Á?Ê≥?};

UENUM(BlueprintType)
enum class ERTSTerrainType : uint8
{
    Walkable,       // ?ØË?Ëµ?    Blocked,        // ?ªÊ?
    Difficult,      // ?∞Èõ£?∞ÂΩ¢
    Water,          // Ê∞¥Â?
    Mountain,       // Â±±Âú∞
    Forest          // Ê£ÆÊ?
};

UENUM(BlueprintType)
enum class ERTSPathfindingState : uint8
{
    Idle,           // Á©∫È?
    Calculating,    // Ë®àÁ?‰∏?    Ready,          // Â∞±Á?
    Failed          // Â§±Ê?
};

USTRUCT(BlueprintType)
struct FRTSPathNode
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Path Node")
    FVector Position;

    UPROPERTY(BlueprintReadOnly, Category = "Path Node")
    float GCost;        // ÂæûËµ∑ÈªûÂà∞?∂Â?ÁØÄÈªûÁ?ÂØ¶È??êÊú¨

    UPROPERTY(BlueprintReadOnly, Category = "Path Node")
    float HCost;        // ÂæûÁï∂?çÁ?ÈªûÂà∞ÁµÇÈ??ÑÈ?‰º∞Ê???
    UPROPERTY(BlueprintReadOnly, Category = "Path Node")
    float FCost;        // GCost + HCost

    // Ê≥®Ê?ÔºöFRTSPathNode ?ØÊ?Ê®ôÈ??ãÔ?‰∏çÈÅ©??UPROPERTY
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

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??©Â? UPROPERTY
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
 * RTSË∑ØÂ?Ë¶èÂ??? * ?ê‰?Â§öÁ®ÆÂ∞ãË∑ØÁÆóÊ??åÂ??ãÈÅø?úÂ??? */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSPathfinder : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSPathfinder();

    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void InitializePathfinder(UMingRTSUnitManager* InUnitManager);

    // Ë∑ØÂ?Ë®àÁ?
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void CalculatePathAsync(const FRTSPathRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    FRTSPathResult CalculatePath(const FRTSPathRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    TArray<FVector> FindPath(FVector Start, FVector Target, ERTSPathfindingAlgorithm Algorithm = ERTSPathfindingAlgorithm::AStar);

    // Ë∑ØÂ??™Â?
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    TArray<FVector> OptimizePath(const TArray<FVector>& Path);

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    TArray<FVector> SmoothPath(const TArray<FVector>& Path, int32 SmoothingIterations = 3);

    // ?ïÊ??øÈ?
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    bool IsPathBlocked(const TArray<FVector>& Path);

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    TArray<FVector> RecalculatePath(const TArray<FVector>& OriginalPath, FVector CurrentPosition, FVector TargetPosition);

    // ?∞ÂΩ¢Á≥ªÁµ±
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void SetTerrainType(FVector Location, ERTSTerrainType TerrainType);

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    ERTSTerrainType GetTerrainType(FVector Location) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    bool IsLocationWalkable(FVector Location) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void UpdateTerrainMap();

    // Á∂≤Ê†ºÁ≥ªÁµ±
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void CreateNavigationGrid(FVector Center, float GridSize, int32 GridResolution);

    // Ê≥®Ê?ÔºöFRTSPathNode ?áÊ?‰∏çÈÅ©??UFUNCTION BlueprintCallable
    FRTSPathNode* GetNodeAtLocation(FVector Location) const;

    // Ê≥®Ê?ÔºöFRTSPathNode ?áÊ?‰∏çÈÅ©??UFUNCTION BlueprintCallable
    TArray<FRTSPathNode*> GetNeighborNodes(FRTSPathNode* Node) const;

    // ?ßËÉΩ??éß
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    float GetAverageCalculationTime() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    int32 GetPendingRequestCount() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void ClearPathCache();

    // Ë™øË©¶?üËÉΩ
    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void DrawDebugPath(const TArray<FVector>& Path, FLinearColor Color = FLinearColor::Green, float Duration = 5.0f);

    UFUNCTION(BlueprintCallable, Category = "RTS Pathfinder")
    void DrawDebugGrid(FLinearColor Color = FLinearColor::White, float Duration = 10.0f);

    // ‰∫ã‰ª∂ÂßîË?
    UPROPERTY(BlueprintAssignable, Category = "RTS Pathfinder")
    FOnPathCalculated OnPathCalculated;

    UPROPERTY(BlueprintAssignable, Category = "RTS Pathfinder")
    FOnPathfindingStateChanged OnPathfindingStateChanged;

protected:
    // ?∏Â?ÁÆóÊ?
    TArray<FVector> CalculateAStarPath(FVector Start, FVector Target);
    TArray<FVector> CalculateDijkstraPath(FVector Start, FVector Target);
    TArray<FVector> CalculateFloydWarshallPath(FVector Start, FVector Target);

    // ËºîÂä©?πÊ?
    float CalculateHeuristic(FVector From, FVector To) const;
    float CalculateTerrainCost(FVector Location) const;
    bool IsValidLocation(FVector Location) const;
    TArray<FVector> ReconstructPath(FRTSPathNode* EndNode) const;

    // Á∂≤Ê†ºÁÆ°Á?
    void InitializeGrid();
    void ClearGrid();
    FRTSPathNode* CreateNode(FVector Location, bool bIsWalkable = true, ERTSTerrainType TerrainType = ERTSTerrainType::Walkable);

    // ?∞Ê≠•?ïÁ?
    void ProcessPendingRequests();
    void CompletePathRequest(const FRTSPathRequest& Request, const FRTSPathResult& Result);

protected:
    // ?∏Â?ÁµÑ‰ª∂
    UPROPERTY()
    TObjectPtr<UMingRTSUnitManager> UnitManager;

    // Ë∑ØÂ?Ë®àÁ?
    UPROPERTY()
    TArray<FRTSPathRequest> PendingRequests;

    UPROPERTY()
    TMap<FString, FRTSPathResult> PathCache;

    // Á∂≤Ê†ºÁ≥ªÁµ±
    // Ê≥®Ê?ÔºöÊ?Ê®ôÈô£?ó‰??©Â? UPROPERTY
    TArray<TArray<FRTSPathNode*>> NavigationGrid;

    UPROPERTY()
    FVector GridCenter;

    UPROPERTY()
    float GridSize;

    UPROPERTY()
    int32 GridResolution;

    UPROPERTY()
    float NodeSize;

    // ?Ä??    UPROPERTY(BlueprintReadOnly, Category = "Pathfinding State")
    ERTSPathfindingState CurrentState;

    // ?ßËÉΩÁµ±Ë?
    UPROPERTY()
    TArray<float> CalculationTimes;

    UPROPERTY()
    float TotalCalculationTime;

    UPROPERTY()
    int32 CompletedRequests;

    // ?çÁΩÆ
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

    // ?∞ÂΩ¢?êÊú¨
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
