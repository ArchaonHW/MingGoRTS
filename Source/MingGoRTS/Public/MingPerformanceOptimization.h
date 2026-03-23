#pragma once

#include "CoreMinimal.h"
#include "MingMemoryOptimization.h"
#include "MingSageBrainOptimizedTypes.h"
#include "MingPerformanceOptimization.generated.h"

// ============================================================================
// 效能優化配置
// ============================================================================

USTRUCT(BlueprintType)
struct FPerformanceOptimizationConfig
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    bool bEnableMultithreading;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    bool bEnableLODSystem;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    bool bEnableObjectCulling;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    bool bEnableTextureStreaming;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    int32 MaxSimultaneousAIAgents;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    float TickRateReductionThreshold;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    float DistanceCullingThreshold;
    
    FPerformanceOptimizationConfig()
        : bEnableMultithreading(true)
        , bEnableLODSystem(true)
        , bEnableObjectCulling(true)
        , bEnableTextureStreaming(true)
        , MaxSimultaneousAIAgents(100)
        , TickRateReductionThreshold(0.8f)
        , DistanceCullingThreshold(5000.0f)
    {}
};

// ============================================================================
// 執行緒安全資料結構
// ============================================================================

/**
 * 無鎖佇列 (Lock-free Queue)
 * 用於多執行緒之間的高效資料傳遞
 */
template<typename T, int32 Capacity>
class TLockFreeQueue
{
private:
    struct FNode
    {
        T Data;
        std::atomic<int32> Next;
        
        FNode() : Next(-1) {}
    };
    
    TArray<FNode> Nodes;
    std::atomic<int32> Head;
    std::atomic<int32> Tail;
    std::atomic<int32> FreeList;
    
public:
    TLockFreeQueue()
    {
        Nodes.SetNum(Capacity);
        Head.store(0);
        Tail.store(0);
        FreeList.store(-1);
        
        // 初始化自由列表
        for (int32 i = 0; i < Capacity; ++i)
        {
            Nodes[i].Next.store(i + 1);
        }
        Nodes[Capacity - 1].Next.store(-1);
        FreeList.store(0);
    }
    
    bool Enqueue(const T& Item)
    {
        int32 NewNode = AllocateNode();
        if (NewNode == -1) return false;
        
        Nodes[NewNode].Data = Item;
        Nodes[NewNode].Next.store(-1);
        
        int32 CurrentTail = Tail.load();
        while (!Tail.compare_exchange_weak(CurrentTail, NewNode))
        {
            CurrentTail = Tail.load();
        }
        
        Nodes[CurrentTail].Next.store(NewNode);
        return true;
    }
    
    bool Dequeue(T& OutItem)
    {
        int32 CurrentHead = Head.load();
        int32 Next = Nodes[CurrentHead].Next.load();
        
        if (Next == -1) return false;
        
        if (Head.compare_exchange_weak(CurrentHead, Next))
        {
            OutItem = Nodes[Next].Data;
            FreeNode(CurrentHead);
            return true;
        }
        
        return false;
    }
    
private:
    int32 AllocateNode()
    {
        int32 Node = FreeList.load();
        while (Node != -1)
        {
            int32 Next = Nodes[Node].Next.load();
            if (FreeList.compare_exchange_weak(Node, Next))
            {
                return Node;
            }
        }
        return -1;
    }
    
    void FreeNode(int32 Node)
    {
        int32 CurrentFree = FreeList.load();
        Nodes[Node].Next.store(CurrentFree);
        while (!FreeList.compare_exchange_weak(CurrentFree, Node))
        {
            Nodes[Node].Next.store(CurrentFree);
        }
    }
};

// ============================================================================
// 分層更新系統
// ============================================================================

UENUM(BlueprintType)
enum class EUpdateFrequency : uint8
{
    EveryFrame,      // 每幀更新
    Every2Frames,    // 每2幀
    Every4Frames,    // 每4幀
    Every8Frames,    // 每8幀
    Every16Frames,   // 每16幀
    Every32Frames,   // 每32幀
    AsNeeded         // 按需更新
};

/**
 * 可更新物件介面
 */
class IMingTickableObject
{
public:
    virtual ~IMingTickableObject() = default;
    virtual void Tick(float DeltaTime) = 0;
    virtual EUpdateFrequency GetUpdateFrequency() const = 0;
    virtual bool ShouldSkipUpdate() const { return false; }
};

/**
 * 分層更新管理器
 * 根據距離和重要性分配不同的更新頻率
 */
UCLASS()
class MINGGORTS_API UMingHierarchicalTickManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Performance|Tick")
    void RegisterTickableObject(IMingTickableObject* Object);
    
    UFUNCTION(BlueprintCallable, Category = "Performance|Tick")
    void UnregisterTickableObject(IMingTickableObject* Object);
    
    UFUNCTION(BlueprintCallable, Category = "Performance|Tick")
    void ProcessTick(float DeltaTime);
    
    UFUNCTION(BlueprintCallable, Category = "Performance|Tick")
    void SetPlayerLocation(FVector Location);
    
    UFUNCTION(BlueprintCallable, Category = "Performance|Tick")
    int32 GetActiveObjectCount() const;

private:
    struct FTickBucket
    {
        TArray<IMingTickableObject*> Objects;
        int32 CurrentFrame;
    };
    
    TMap<EUpdateFrequency, FTickBucket> TickBuckets;
    FVector LastPlayerLocation;
    int32 GlobalFrameCounter;
    
    void DistributeObjectsToBuckets();
    bool ShouldUpdateThisFrame(EUpdateFrequency Frequency) const;
};

// ============================================================================
// 空間分割系統
// ============================================================================

/**
 * 均勻網格空間分割
 * 用於快速的鄰居查詢和碰撞檢測
 */
template<typename T>
class TUniformGridSpatialPartition
{
private:
    struct FGridCell
    {
        TMingFixedArray<T*, 16> Objects;
    };
    
    TArray<FGridCell> Cells;
    FVector GridOrigin;
    float CellSize;
    int32 GridSizeX;
    int32 GridSizeY;
    int32 GridSizeZ;
    
public:
    TUniformGridSpatialPartition(FVector Origin, FVector Extent, float InCellSize)
        : GridOrigin(Origin)
        , CellSize(InCellSize)
    {
        GridSizeX = FMath::CeilToInt(Extent.X * 2 / CellSize);
        GridSizeY = FMath::CeilToInt(Extent.Y * 2 / CellSize);
        GridSizeZ = FMath::CeilToInt(Extent.Z * 2 / CellSize);
        
        Cells.SetNum(GridSizeX * GridSizeY * GridSizeZ);
    }
    
    void InsertObject(T* Object, FVector Location)
    {
        int32 CellIndex = GetCellIndex(Location);
        if (CellIndex >= 0 && CellIndex < Cells.Num())
        {
            Cells[CellIndex].Objects.Add(Object);
        }
    }
    
    void RemoveObject(T* Object, FVector Location)
    {
        int32 CellIndex = GetCellIndex(Location);
        if (CellIndex >= 0 && CellIndex < Cells.Num())
        {
            Cells[CellIndex].Objects.Remove(Object);
        }
    }
    
    void QueryNearbyObjects(FVector Location, float Radius, TArray<T*>& OutResults) const
    {
        int32 CellRadius = FMath::CeilToInt(Radius / CellSize);
        FVector LocalPos = Location - GridOrigin;
        
        int32 CenterX = FMath::FloorToInt(LocalPos.X / CellSize);
        int32 CenterY = FMath::FloorToInt(LocalPos.Y / CellSize);
        int32 CenterZ = FMath::FloorToInt(LocalPos.Z / CellSize);
        
        for (int32 x = -CellRadius; x <= CellRadius; ++x)
        {
            for (int32 y = -CellRadius; y <= CellRadius; ++y)
            {
                for (int32 z = -CellRadius; z <= CellRadius; ++z)
                {
                    int32 CellX = CenterX + x;
                    int32 CellY = CenterY + y;
                    int32 CellZ = CenterZ + z;
                    
                    if (IsValidCell(CellX, CellY, CellZ))
                    {
                        int32 Index = GetCellIndexFromCoords(CellX, CellY, CellZ);
                        for (T* Object : Cells[Index].Objects)
                        {
                            OutResults.AddUnique(Object);
                        }
                    }
                }
            }
        }
    }
    
    void Clear()
    {
        for (auto& Cell : Cells)
        {
            Cell.Objects.Empty();
        }
    }

private:
    int32 GetCellIndex(FVector Location) const
    {
        FVector LocalPos = Location - GridOrigin;
        int32 x = FMath::FloorToInt(LocalPos.X / CellSize);
        int32 y = FMath::FloorToInt(LocalPos.Y / CellSize);
        int32 z = FMath::FloorToInt(LocalPos.Z / CellSize);
        
        return GetCellIndexFromCoords(x, y, z);
    }
    
    int32 GetCellIndexFromCoords(int32 x, int32 y, int32 z) const
    {
        return x + y * GridSizeX + z * GridSizeX * GridSizeY;
    }
    
    bool IsValidCell(int32 x, int32 y, int32 z) const
    {
        return x >= 0 && x < GridSizeX &&
               y >= 0 && y < GridSizeY &&
               z >= 0 && z < GridSizeZ;
    }
};

// ============================================================================
// 資料壓縮工具
// ============================================================================

/**
 * 簡易向量壓縮
 * 用於網路傳輸和記憶體存儲
 */
USTRUCT(BlueprintType)
struct FMingCompressedVector
{
    GENERATED_BODY()
    
    UPROPERTY()
    int16 X_Packed;
    
    UPROPERTY()
    int16 Y_Packed;
    
    UPROPERTY()
    int16 Z_Packed;
    
    static constexpr float PackScale = 100.0f;
    
    FMingCompressedVector()
        : X_Packed(0)
        , Y_Packed(0)
        , Z_Packed(0)
    {}
    
    FMingCompressedVector(FVector Vector)
    {
        X_Packed = FMath::Clamp(FMath::RoundToInt(Vector.X * PackScale), -32768, 32767);
        Y_Packed = FMath::Clamp(FMath::RoundToInt(Vector.Y * PackScale), -32768, 32767);
        Z_Packed = FMath::Clamp(FMath::RoundToInt(Vector.Z * PackScale), -32768, 32767);
    }
    
    FVector ToVector() const
    {
        return FVector(
            X_Packed / PackScale,
            Y_Packed / PackScale,
            Z_Packed / PackScale
        );
    }
    
    // 6 bytes vs 12 bytes (50% reduction)
    static constexpr int32 SizeInBytes = 6;
};

/**
 * 簡易旋轉壓縮
 */
USTRUCT(BlueprintType)
struct FMingCompressedRotator
{
    GENERATED_BODY()
    
    UPROPERTY()
    uint16 Pitch_Packed;
    
    UPROPERTY()
    uint16 Yaw_Packed;
    
    UPROPERTY()
    uint16 Roll_Packed;
    
    FMingCompressedRotator()
        : Pitch_Packed(0)
        , Yaw_Packed(0)
        , Roll_Packed(0)
    {}
    
    FMingCompressedRotator(FRotator Rotator)
    {
        Pitch_Packed = FMath::RoundToInt((Rotator.Pitch + 180.0f) / 360.0f * 65535.0f);
        Yaw_Packed = FMath::RoundToInt((Rotator.Yaw + 180.0f) / 360.0f * 65535.0f);
        Roll_Packed = FMath::RoundToInt((Rotator.Roll + 180.0f) / 360.0f * 65535.0f);
    }
    
    FRotator ToRotator() const
    {
        return FRotator(
            Pitch_Packed / 65535.0f * 360.0f - 180.0f,
            Yaw_Packed / 65535.0f * 360.0f - 180.0f,
            Roll_Packed / 65535.0f * 360.0f - 180.0f
        );
    }
};

// ============================================================================
// 快取系統
// ============================================================================

/**
 * LRU (Least Recently Used) 快取
 * 用於資料查詢結果的快取
 */
template<typename KeyType, typename ValueType, int32 MaxSize>
class T LRUCache
{
private:
    struct FCacheEntry
    {
        KeyType Key;
        ValueType Value;
        uint32 AccessCounter;
    };
    
    TMingFixedArray<FCacheEntry, MaxSize> Entries;
    uint32 CurrentCounter;
    int32 NumEntries;
    
public:
    T LRUCache() : CurrentCounter(0), NumEntries(0) {}
    
    bool Get(const KeyType& Key, ValueType& OutValue)
    {
        for (int32 i = 0; i < NumEntries; ++i)
        {
            if (Entries[i].Key == Key)
            {
                Entries[i].AccessCounter = ++CurrentCounter;
                OutValue = Entries[i].Value;
                return true;
            }
        }
        return false;
    }
    
    void Put(const KeyType& Key, const ValueType& Value)
    {
        // 檢查是否已存在
        for (int32 i = 0; i < NumEntries; ++i)
        {
            if (Entries[i].Key == Key)
            {
                Entries[i].Value = Value;
                Entries[i].AccessCounter = ++CurrentCounter;
                return;
            }
        }
        
        // 新增項目
        if (NumEntries < MaxSize)
        {
            Entries[NumEntries].Key = Key;
            Entries[NumEntries].Value = Value;
            Entries[NumEntries].AccessCounter = ++CurrentCounter;
            ++NumEntries;
        }
        else
        {
            // 替換最少使用的項目
            int32 LRUIndex = 0;
            uint32 MinCounter = Entries[0].AccessCounter;
            
            for (int32 i = 1; i < MaxSize; ++i)
            {
                if (Entries[i].AccessCounter < MinCounter)
                {
                    MinCounter = Entries[i].AccessCounter;
                    LRUIndex = i;
                }
            }
            
            Entries[LRUIndex].Key = Key;
            Entries[LRUIndex].Value = Value;
            Entries[LRUIndex].AccessCounter = ++CurrentCounter;
        }
    }
    
    void Clear()
    {
        NumEntries = 0;
        CurrentCounter = 0;
    }
    
    int32 Num() const { return NumEntries; }
};

// ============================================================================
// 批次處理系統
// ============================================================================

/**
 * 批次處理管理器
 * 用於合併多個小操作以減少開銷
 */
UCLASS()
class MINGGORTS_API UMingBatchProcessingManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Performance|Batch")
    void InitializeBatch(int32 ExpectedOperationCount);
    
    UFUNCTION(BlueprintCallable, Category = "Performance|Batch")
    void AddOperation(const FMingOptimizedString& OperationType, const FMingOptimizedString& Target, float Value);
    
    UFUNCTION(BlueprintCallable, Category = "Performance|Batch")
    void ExecuteBatch();
    
    UFUNCTION(BlueprintCallable, Category = "Performance|Batch")
    void ClearBatch();
    
    UFUNCTION(BlueprintCallable, Category = "Performance|Batch")
    int32 GetPendingOperationCount() const;
    
    UFUNCTION(BlueprintCallable, Category = "Performance|Batch")
    bool IsBatchFull() const;

private:
    struct FBatchOperation
    {
        FMingOptimizedString OperationType;
        FMingOptimizedString Target;
        float Value;
    };
    
    TMingFixedArray<FBatchOperation, 128> PendingOperations;
    int32 OperationCount;
};

// ============================================================================
// 效能監控
// ============================================================================

USTRUCT(BlueprintType)
struct FPerformanceMetrics
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    float FrameTime;
    
    UPROPERTY(BlueprintReadOnly)
    float GameThreadTime;
    
    UPROPERTY(BlueprintReadOnly)
    float RenderThreadTime;
    
    UPROPERTY(BlueprintReadOnly)
    float GPUFrameTime;
    
    UPROPERTY(BlueprintReadOnly)
    int32 DrawCalls;
    
    UPROPERTY(BlueprintReadOnly)
    int32 TrianglesRendered;
    
    UPROPERTY(BlueprintReadOnly)
    int32 ActiveAIAgents;
    
    UPROPERTY(BlueprintReadOnly)
    int32 PhysicsBodies;
};

UCLASS()
class MINGGORTS_API UMingPerformanceMonitor : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Performance|Monitor")
    static void StartMonitoring();
    
    UFUNCTION(BlueprintCallable, Category = "Performance|Monitor")
    static void StopMonitoring();
    
    UFUNCTION(BlueprintCallable, Category = "Performance|Monitor")
    static FPerformanceMetrics GetCurrentMetrics();
    
    UFUNCTION(BlueprintCallable, Category = "Performance|Monitor")
    static bool IsPerformanceCritical();
    
    UFUNCTION(BlueprintCallable, Category = "Performance|Monitor")
    static void ApplyEmergencyOptimizations();
    
    UFUNCTION(BlueprintCallable, Category = "Performance|Monitor")
    static FString GetPerformanceReport();
    
    UFUNCTION(BlueprintCallable, Category = "Performance|Monitor")
    static void SetTargetFrameRate(int32 TargetFPS);
    
    UFUNCTION(BlueprintCallable, Category = "Performance|Monitor")
    static void EnableDynamicResolution(bool bEnable);
};
