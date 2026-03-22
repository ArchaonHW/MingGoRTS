#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HAL/CriticalSection.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Containers/Queue.h"

// 內存塊狀態
UENUM(BlueprintType)
enum class EMemoryBlockState : uint8
{
    Free,           // 空閒
    Allocated,      // 已分配
    Reserved,       // 保留
    Fragmented,     // 碎片
    Locked          // 鎖定
};

// 內存分配策略
UENUM(BlueprintType)
enum class EAllocationStrategy : uint8
{
    FirstFit,       // 首次適應
    BestFit,        // 最佳適應
    WorstFit,       // 最差適應
    NextFit,        // 下次適應
    Buddy,          // 伴侶系統
    Slab            // Slab分配器
};

// 內存塊信息
USTRUCT(BlueprintType)
struct FMemoryBlock
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Memory Block")
    void* BaseAddress;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Block")
    int32 Size;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Block")
    EMemoryBlockState State;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Block")
    FString OwnerID;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Block")
    FDateTime AllocationTime;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Block")
    int32 RefCount;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Block")
    int32 MagicNumber; // 用於檢測內存損壞

    FMemoryBlock()
    {
        BaseAddress = nullptr;
        Size = 0;
        State = EMemoryBlockState::Free;
        RefCount = 0;
        MagicNumber = 0xDEADBEEF; // 魔術數字
    }
};

// 內存池信息
USTRUCT(BlueprintType)
struct FMemoryPool
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Memory Pool")
    int32 BlockSize;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Pool")
    int32 BlockCount;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Pool")
    int32 FreeBlocks;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Pool")
    TArray<void*> FreeList;

    FMemoryPool()
    {
        BlockSize = 0;
        BlockCount = 0;
        FreeBlocks = 0;
    }
};

// 內存統計
USTRUCT(BlueprintType)
struct FMemoryStatistics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Memory Stats")
    int32 TotalMemory;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Stats")
    int32 UsedMemory;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Stats")
    int32 FreeMemory;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Stats")
    int32 FragmentedMemory;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Stats")
    int32 AllocatedBlocks;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Stats")
    int32 FreeBlocks;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Stats")
    float FragmentationRatio;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Stats")
    int32 TotalAllocations;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Stats")
    int32 TotalDeallocations;

    UPROPERTY(BlueprintReadOnly, Category = "Memory Stats")
    float AverageAllocationSize;

    FMemoryStatistics()
    {
        TotalMemory = 0;
        UsedMemory = 0;
        FreeMemory = 0;
        FragmentedMemory = 0;
        AllocatedBlocks = 0;
        FreeBlocks = 0;
        FragmentationRatio = 0.0f;
        TotalAllocations = 0;
        TotalDeallocations = 0;
        AverageAllocationSize = 0.0f;
    }
};

// 內存分配請求
USTRUCT(BlueprintType)
struct FMemoryAllocationRequest
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Memory Request")
    int32 Size;

    UPROPERTY(BlueprintReadWrite, Category = "Memory Request")
    FString OwnerID;

    UPROPERTY(BlueprintReadWrite, Category = "Memory Request")
    int32 Alignment;

    UPROPERTY(BlueprintReadWrite, Category = "Memory Request")
    bool bZeroMemory;

    UPROPERTY(BlueprintReadWrite, Category = "Memory Request")
    int32 Priority;

    FMemoryAllocationRequest()
    {
        Size = 0;
        Alignment = 4;
        bZeroMemory = false;
        Priority = 0;
    }
};

// 內存事件委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMemoryAllocated, void*, Address, int32, Size);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMemoryDeallocated, void*, Address, const FString&, OwnerID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMemoryWarning, int32, FreeMemoryKB);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMemoryError, const FString&, ErrorMessage);

/**
 * 內存管理器
 * 提供動態內存分配、碎片整理、內存保護等功能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSMemoryManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSMemoryManager();

    // 初始化和控制
    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    bool InitializeMemoryManager(int32 TotalSizeMB = 1024);

    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    void ShutdownMemoryManager();

    UFUNCTION(BlueprintPure, Category = "Memory Manager")
    bool IsInitialized() const { return bInitialized; }

    // 內存分配
    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    void* AllocateMemory(const FMemoryAllocationRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    void* AllocateMemorySimple(int32 Size, const FString& OwnerID = TEXT(""));

    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    bool DeallocateMemory(void* Address);

    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    bool ReallocateMemory(void* Address, int32 NewSize);

    // 內存池管理
    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    bool CreateMemoryPool(int32 BlockSize, int32 BlockCount);

    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    void* AllocateFromPool(int32 BlockSize);

    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    bool DeallocateToPool(void* Address, int32 BlockSize);

    // 內存操作
    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    bool CopyMemory(void* Dest, const void* Src, int32 Size);

    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    bool SetMemory(void* Dest, int32 Value, int32 Size);

    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    bool ZeroMemory(void* Dest, int32 Size);

    // 內存保護
    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    bool ProtectMemory(void* Address, int32 Size, bool bRead = true, bool bWrite = true, bool bExecute = false);

    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    bool UnprotectMemory(void* Address);

    // 碎片整理
    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    bool DefragmentMemory();

    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    void SetAutoDefragmentation(bool bEnabled, float ThresholdRatio = 0.3f);

    // 內存查詢
    UFUNCTION(BlueprintPure, Category = "Memory Manager")
    FMemoryStatistics GetMemoryStatistics() const;

    UFUNCTION(BlueprintPure, Category = "Memory Manager")
    int32 GetFreeMemory() const;

    UFUNCTION(BlueprintPure, Category = "Memory Manager")
    int32 GetUsedMemory() const;

    UFUNCTION(BlueprintPure, Category = "Memory Manager")
    float GetFragmentationRatio() const;

    UFUNCTION(BlueprintPure, Category = "Memory Manager")
    TArray<FMemoryBlock> GetMemoryBlocks() const;

    // 配置
    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    void SetAllocationStrategy(EAllocationStrategy Strategy);

    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    void SetMemoryWarningThreshold(int32 ThresholdMB);

    UFUNCTION(BlueprintCallable, Category = "Memory Manager")
    void EnableMemoryLogging(bool bEnabled);

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Memory Events")
    FOnMemoryAllocated OnMemoryAllocated;

    UPROPERTY(BlueprintAssignable, Category = "Memory Events")
    FOnMemoryDeallocated OnMemoryDeallocated;

    UPROPERTY(BlueprintAssignable, Category = "Memory Events")
    FOnMemoryWarning OnMemoryWarning;

    UPROPERTY(BlueprintAssignable, Category = "Memory Events")
    FOnMemoryError OnMemoryError;

protected:
    // 內存池
    UPROPERTY()
    TArray<FMemoryPool> MemoryPools;

    // 內存塊映射
    UPROPERTY()
    TMap<void*, FMemoryBlock> MemoryBlocks;

    // 分配策略
    UPROPERTY()
    EAllocationStrategy AllocationStrategy;

    // 內存配置
    UPROPERTY()
    int32 TotalMemorySize;

    UPROPERTY()
    int32 MemoryWarningThreshold;

    UPROPERTY()
    bool bAutoDefragmentation;

    UPROPERTY()
    float DefragmentationThreshold;

    UPROPERTY()
    bool bMemoryLogging;

    // 狀態
    UPROPERTY()
    bool bInitialized;

    UPROPERTY()
    void* MemoryBase;

    UPROPERTY()
    int32 UsedMemory;

    UPROPERTY()
    int32 AllocatedBlockCount;

    // 統計
    UPROPERTY()
    FMemoryStatistics Statistics;

    // 同步機制
    mutable FCriticalSection MemoryCriticalSection;
    mutable FCriticalSection PoolCriticalSection;
    mutable FCriticalSection StatisticsCriticalSection;

    // 內部方法
    void* AllocateFirstFit(int32 Size, const FString& OwnerID);
    void* AllocateBestFit(int32 Size, const FString& OwnerID);
    void* AllocateWorstFit(int32 Size, const FString& OwnerID);
    void* AllocateNextFit(int32 Size, const FString& OwnerID);
    void* AllocateBuddy(int32 Size, const FString& OwnerID);
    void* AllocateFromSlab(int32 Size, const FString& OwnerID);

    bool DeallocateMemoryBlock(void* Address);
    void CoalesceMemory();
    void UpdateStatistics();
    bool IsMemoryValid(void* Address) const;
    void LogMemoryOperation(const FString& Operation, void* Address, int32 Size, const FString& OwnerID);

    // 碎片整理
    void PerformDefragmentation();
    void MoveMemoryBlock(FMemoryBlock& Block, void* NewAddress);
    TArray<FMemoryBlock> FindFragmentedBlocks() const;

    // 內存池操作
    FMemoryPool* FindSuitablePool(int32 Size);
    bool CreatePool(int32 BlockSize, int32 BlockCount);
    void DestroyPool(int32 BlockSize);

private:
    // 初始化方法
    bool InitializeMemoryPools();
    bool InitializeMemoryBlocks();

    // 清理方法
    void CleanupMemoryPools();
    void CleanupMemoryBlocks();
    void ResetStatistics();

    // 輔助方法
    int32 AlignSize(int32 Size, int32 Alignment);
    bool IsPowerOfTwo(int32 Value) const;
    int32 NextPowerOfTwo(int32 Value) const;
    void* AlignAddress(void* Address, int32 Alignment);
    bool CheckMemoryCorruption(const FMemoryBlock& Block) const;
};
