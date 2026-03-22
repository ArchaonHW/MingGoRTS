#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSpatialPartition.generated.h"

class AMingTacticalUnit;

/**
 * ç©ºé?ç¶²æ ¼?®å? | Spatial Grid Cell
 * å­˜å„²ä½æ–¼è©²ç¶²?¼å…§?„å–®ä½?| Store units located within this grid cell
 */
USTRUCT()
struct MINGTACTICAL_API FSpatialGridCell
{
    GENERATED_BODY()

    // ç½‘æ ¼?æ?
    UPROPERTY()
    FIntVector GridCoord;

    // ?…å«?„å?ä½?    UPROPERTY()
    TArray<TWeakObjectPtr<AMingTacticalUnit>> Units;

    // ?€?æ›´?°æ—¶??    float LastUpdateTime = 0.0f;

    // ?…å›´??    FBox Bounds;

    FMingSpatialGridCell() = default;
    explicit FMingSpatialGridCell(const FIntVector& InCoord) : GridCoord(InCoord) {}

    void AddUnit(AMingTacticalUnit* Unit};
    void RemoveUnit(AMingTacticalUnit* Unit};
    bool Contains(AMingTacticalUnit* Unit) const;
    void Clear(};
    int32 GetUnitCount() const { return Units.Num(}; }
};

/**
 * ç©ºé??¥è©¢çµæ? | Spatial Query Result
 */
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FSpatialQueryResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TArray<AMingTacticalUnit*> FoundUnits;

    UPROPERTY(BlueprintReadOnly)
    int32 CellsChecked = 0;

    UPROPERTY(BlueprintReadOnly)
    float QueryTimeMs = 0.0f;

    void AddUnit(AMingTacticalUnit* Unit};
};

/**
 * ç©ºé??†å?ç³»çµ± | Spatial Partitioning System
 * 
 * ?ªå?å¤§è?æ¨¡å–®ä½å ´?¯ç?ç©ºé??¥è©¢?§èƒ½ï¼?| Optimize spatial query performance for large-scale unit scenes:
 * - ?®ä?ä½ç½®è¿½è¹¤ | Unit position tracking
 * - ç¯„å??¥è©¢ | Range queries
 * - ?€è¿‘é„°?¥è©¢ | Nearest neighbor queries
 * - è¦–ç??¥è©¢ | Line of sight queries
 * 
 * ?©ç”¨??1000+ ?®ä??„å¤§è¦æ¨¡?°é¬¥?´æ™¯ | Suitable for 1000+ unit large-scale combat scenarios
 */
UCLASS(ClassGroup = (Optimization), Blueprintable)
class MINGTACTICAL_API UMingSpatialPartition : public UObject
{
    GENERATED_BODY()

public:
    UMingSpatialPartition(};

    /**
     * ?å??–ç©º?´å??ºç³»ç»?     * @param WorldBounds ä¸–ç?è¾¹ç?
     * @param CellSize ç½‘æ ¼?•å?å¤§å?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void Initialize(const FBox& WorldBounds, float CellSize = 1000.0f};

    /**
     * ?³é—­ç³»ç?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void Shutdown(};

    /**
     * æ³¨å??•ä??°ç©º?´å???     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void RegisterUnit(AMingTacticalUnit* Unit};

    /**
     * ä»ç©º?´å??ºæ³¨?€?•ä?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void UnregisterUnit(AMingTacticalUnit* Unit};

    /**
     * ?´æ–°?•ä?ä½ç½®
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void UpdateUnitPosition(AMingTacticalUnit* Unit, const FVector& NewLocation};

    /**
     * ?ƒå›´?¥è¯¢ - ?·å??‡å??ƒå›´?…ç??€?‰å?ä½?     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    FSpatialQueryResult QueryUnitsInRadius(const FVector& Center, float Radius, int32 TeamId = -1};

    /**
     * æ¡†æŸ¥è¯?- ?·å??‡å??…å›´?’å??„æ??‰å?ä½?     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    FSpatialQueryResult QueryUnitsInBox(const FBox& Box, int32 TeamId = -1};

    /**
     * ?·å??€è¿‘ç??•ä?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    AMingTacticalUnit* GetNearestUnit(const FVector& Location, float MaxRadius, int32 TeamId = -1};

    /**
     * ?·å??€è¿‘ç??Œæ–¹?•ä?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    AMingTacticalUnit* GetNearestEnemyUnit(AMingTacticalUnit* SourceUnit, float MaxRadius};

    /**
     * ?·å??ƒå›´?…ç??Œæ–¹?•ä?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    FSpatialQueryResult GetEnemyUnitsInRadius(AMingTacticalUnit* SourceUnit, float Radius};

    /**
     * ?·å??ƒå›´?…ç??‹æ–¹?•ä?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    FSpatialQueryResult GetFriendlyUnitsInRadius(AMingTacticalUnit* SourceUnit, float Radius};

    /**
     * è§†çº¿?¥è¯¢ - æ£€æµ‹ä¸¤ä¸ªä?ç½®é—´?¯å¦?‰å?ä½é˜»??     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    bool HasLineOfSight(const FVector& Start, const FVector& End, float CheckRadius = 50.0f};

    /**
     * ?·å?è§†çº¿?…ç??•ä?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    FSpatialQueryResult GetUnitsInLineOfSight(const FVector& Start, const FVector& End, float Width};

    /**
     * ?·å??‡å?ç½‘æ ¼?•å??…ç??•ä?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    TArray<AMingTacticalUnit*> GetUnitsInCell(const FIntVector& GridCoord};

    /**
     * ?·å??•ä??€?¨ç?ç½‘æ ¼?æ?
     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition")
    FIntVector GetUnitGridCoord(AMingTacticalUnit* Unit) const;

    /**
     * ?·å?ä¸–ç?ä½ç½®å¯¹å??„ç??¼å???     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition")
    FIntVector WorldToGrid(const FVector& WorldLocation) const;

    /**
     * ?·å?ç½‘æ ¼?æ??„ä??Œä¸­å¿ƒä?ç½?     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition")
    FVector GridToWorld(const FIntVector& GridCoord) const;

    /**
     * ?·å?ç³»ç?ç»Ÿè®¡ä¿¡æ¯
     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition|Stats")
    int32 GetTotalCellCount() const { return GridCells.Num(}; }

    UFUNCTION(BlueprintPure, Category = "Spatial Partition|Stats")
    int32 GetOccupiedCellCount() const;

    UFUNCTION(BlueprintPure, Category = "Spatial Partition|Stats")
    int32 GetTotalUnitCount() const { return TrackedUnits.Num(}; }

    UFUNCTION(BlueprintPure, Category = "Spatial Partition|Stats")
    float GetAverageUnitsPerCell() const;

    /**
     * è°ƒè?ç»˜åˆ¶
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Debug")
    void DrawDebugVisualization(bool bDrawGrid = true, bool bDrawUnits = false};

    /**
     * éªŒè?ç³»ç?å®Œæ•´??     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Debug")
    bool ValidateIntegrity(};

    /**
     * æ¸…ç?? æ?å¼•ç”¨
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void CleanupInvalidReferences(};

    // ==== ?¹é??ä? ====

    /**
     * ?¹é?æ³¨å??•ä?
     */
    void BatchRegisterUnits(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ?¹é?æ³¨é??•ä?
     */
    void BatchUnregisterUnits(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ?¹é??´æ–°ä½ç½®
     */
    void BatchUpdatePositions(const TMap<AMingTacticalUnit*, FVector>& PositionUpdates};

private:
    // ç½‘æ ¼?•å?? å?
    UPROPERTY()
    TMap<FIntVector, FSpatialGridCell> GridCells;

    // ?•ä??°ç??¼å??‡ç?? å?
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingTacticalUnit>, FIntVector> UnitToGridMap;

    // è¢«è¿½è¸ªç??•ä?
    UPROPERTY()
    TArray<TWeakObjectPtr<AMingTacticalUnit>> TrackedUnits;

    // ?ç½®
    float CellSize;
    FBox WorldBounds;
    bool bIsInitialized;

    // ç»Ÿè®¡
    int32 QueryCount;
    float TotalQueryTime;

    // ?·å??–å?å»ºç??¼å???    FSpatialGridCell* GetOrCreateCell(const FIntVector& Coord};
    FSpatialGridCell* GetCell(const FIntVector& Coord};

    // ?·å??¸é‚»ç½‘æ ¼?æ?
    TArray<FIntVector> GetNeighboringCells(const FIntVector& Center, int32 Radius = 1};

    // ?·å??¥è¯¢?€?€?„ç??¼è???    TArray<FIntVector> GetCellsInRadius(const FVector& Center, float Radius};
    TArray<FIntVector> GetCellsInBox(const FBox& Box};

    // æ¸…ç?ç©ºå???    void CleanupEmptyCells(};

    // éªŒè??æ??¯å¦?¨è¾¹?Œå?
    bool IsValidGridCoord(const FIntVector& Coord) const;

    // ?´æ–°ç»Ÿè®¡
    void RecordQueryTime(float TimeMs};
};

