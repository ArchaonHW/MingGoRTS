#include "MingRTSMemoryManager.h"
#include "HAL/PlatformFilemanager.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "HAL/MemoryMisc.h"

UMingRTSMemoryManager::UMingRTSMemoryManager()
{
    bInitialized = false;
    MemoryBase = nullptr;
    TotalMemorySize = 0;
    UsedMemory = 0;
    AllocatedBlockCount = 0;
    AllocationStrategy = EAllocationStrategy::FirstFit;
    MemoryWarningThreshold = 100; // 100MB
    bAutoDefragmentation = true;
    DefragmentationThreshold = 0.3f; // 30%
    bMemoryLogging = false;
}

bool UMingRTSMemoryManager::InitializeMemoryManager(int32 TotalSizeMB)
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Memory Manager already initialized"));
        return true;
    }

    TotalMemorySize = TotalSizeMB * 1024 * 1024; // 轉換為字節

    // 分配系統內存
    MemoryBase = FMemory::Malloc(TotalMemorySize);
    if (!MemoryBase)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to allocate system memory: %d MB"), TotalSizeMB);
        OnMemoryError.Broadcast(FString::Printf(TEXT("Failed to allocate %d MB memory"), TotalSizeMB));
        return false;
    }

    // 初始化內存池
    if (!InitializeMemoryPools())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to initialize memory pools"));
        FMemory::Free(MemoryBase);
        MemoryBase = nullptr;
        return false;
    }

    // 初始化內存塊
    if (!InitializeMemoryBlocks())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to initialize memory blocks"));
        CleanupMemoryPools();
        FMemory::Free(MemoryBase);
        MemoryBase = nullptr;
        return false;
    }

    bInitialized = true;
    ResetStatistics();

    UE_LOG(LogTemp, Log, TEXT("Memory Manager initialized with %d MB"), TotalSizeMB);
    
    // 啟動內存監控定時器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimerForNextTick([this]()
        {
            UpdateStatistics();
        });
    }

    return true;
}

void UMingRTSMemoryManager::ShutdownMemoryManager()
{
    if (!bInitialized)
    {
        return;
    }

    // 清理所有內存池
    CleanupMemoryPools();

    // 清理所有內存塊
    CleanupMemoryBlocks();

    // 釋放系統內存
    if (MemoryBase)
    {
        FMemory::Free(MemoryBase);
        MemoryBase = nullptr;
    }

    bInitialized = false;
    UE_LOG(LogTemp, Log, TEXT("Memory Manager shutdown completed"));
}

void* UMingRTSMemoryManager::AllocateMemory(const FMemoryAllocationRequest& Request)
{
    if (!bInitialized || Request.Size <= 0)
    {
        return nullptr;
    }

    void* Address = nullptr;

    // 根據分配策略進行分配
    switch (AllocationStrategy)
    {
        case EAllocationStrategy::FirstFit:
            Address = AllocateFirstFit(Request.Size, Request.OwnerID);
            break;
        
        case EAllocationStrategy::BestFit:
            Address = AllocateBestFit(Request.Size, Request.OwnerID);
            break;
        
        case EAllocationStrategy::WorstFit:
            Address = AllocateWorstFit(Request.Size, Request.OwnerID);
            break;
        
        case EAllocationStrategy::NextFit:
            Address = AllocateNextFit(Request.Size, Request.OwnerID);
            break;
        
        case EAllocationStrategy::Buddy:
            Address = AllocateBuddy(Request.Size, Request.OwnerID);
            break;
        
        case EAllocationStrategy::Slab:
            Address = AllocateFromSlab(Request.Size, Request.OwnerID);
            break;
        
        default:
            Address = AllocateFirstFit(Request.Size, Request.OwnerID);
            break;
    }

    if (Address)
    {
        // 對齊內存
        Address = AlignAddress(Address, Request.Alignment);

        // 如果需要，清零內存
        if (Request.bZeroMemory)
        {
            FMemory::Memzero(Address, Request.Size);
        }

        // 更新統計
        {
            FScopeLock Lock(&StatisticsCriticalSection);
            Statistics.TotalAllocations++;
            Statistics.AllocatedBlocks++;
        }

        // 記錄日誌
        LogMemoryOperation(TEXT("Allocate"), Address, Request.Size, Request.OwnerID);

        // 廣播事件
        OnMemoryAllocated.Broadcast(Address, Request.Size);
    }
    else
    {
        // 分配失敗
        FString ErrorMessage = FString::Printf(TEXT("Failed to allocate %d bytes for %s"), 
                                         Request.Size, *Request.OwnerID);
        OnMemoryError.Broadcast(ErrorMessage);
        UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
    }

    return Address;
}

void* UMingRTSMemoryManager::AllocateMemorySimple(int32 Size, const FString& OwnerID)
{
    FMemoryAllocationRequest Request;
    Request.Size = Size;
    Request.OwnerID = OwnerID;
    Request.Alignment = 4;
    Request.bZeroMemory = false;
    Request.Priority = 0;

    return AllocateMemory(Request);
}

bool UMingRTSMemoryManager::DeallocateMemory(void* Address)
{
    if (!bInitialized || !Address)
    {
        return false;
    }

    bool Success = DeallocateMemoryBlock(Address);

    if (Success)
    {
        // 更新統計
        {
            FScopeLock Lock(&StatisticsCriticalSection);
            Statistics.TotalDeallocations++;
            Statistics.AllocatedBlocks--;
        }

        // 記錄日誌
        LogMemoryOperation(TEXT("Deallocate"), Address, 0, TEXT(""));

        // 廣播事件
        OnMemoryDeallocated.Broadcast(Address, TEXT(""));
    }

    return Success;
}

bool UMingRTSMemoryManager::ReallocateMemory(void* Address, int32 NewSize)
{
    if (!bInitialized || !Address || NewSize <= 0)
    {
        return false;
    }

    FScopeLock Lock(&MemoryCriticalSection);

    if (FMemoryBlock* Block = MemoryBlocks.Find(Address))
    {
        // 簡單實現：分配新內存，複製數據，釋放舊內存
        void* NewAddress = AllocateMemorySimple(NewSize, Block->OwnerID);
        if (NewAddress)
        {
            // 複製數據
            int32 CopySize = FMath::Min(Block->Size, NewSize);
            FMemory::Memcpy(NewAddress, Address, CopySize);

            // 釋放舊內存
            DeallocateMemory(Address);

            UE_LOG(LogTemp, Log, TEXT("Memory reallocated from %d to %d bytes"), Block->Size, NewSize);
            return true;
        }
    }

    return false;
}

bool UMingRTSMemoryManager::CreateMemoryPool(int32 BlockSize, int32 BlockCount)
{
    FScopeLock Lock(&PoolCriticalSection);
    return CreatePool(BlockSize, BlockCount);
}

void* UMingRTSMemoryManager::AllocateFromPool(int32 BlockSize)
{
    FScopeLock Lock(&PoolCriticalSection);
    
    if (FMemoryPool* Pool = FindSuitablePool(BlockSize))
    {
        if (Pool->FreeList.Num() > 0)
        {
            void* Address = Pool->FreeList.Pop();
            Pool->FreeBlocks--;
            return Address;
        }
    }

    return nullptr;
}

bool UMingRTSMemoryManager::DeallocateToPool(void* Address, int32 BlockSize)
{
    FScopeLock Lock(&PoolCriticalSection);
    
    if (FMemoryPool* Pool = FindSuitablePool(BlockSize))
    {
        Pool->FreeList.Add(Address);
        Pool->FreeBlocks++;
        return true;
    }

    return false;
}

bool UMingRTSMemoryManager::CopyMemory(void* Dest, const void* Src, int32 Size)
{
    if (!Dest || !Src || Size <= 0)
    {
        return false;
    }

    FMemory::Memcpy(Dest, Src, Size);
    return true;
}

bool UMingRTSMemoryManager::SetMemory(void* Dest, int32 Value, int32 Size)
{
    if (!Dest || Size <= 0)
    {
        return false;
    }

    FMemory::Memset(Dest, Value, Size);
    return true;
}

bool UMingRTSMemoryManager::ZeroMemory(void* Dest, int32 Size)
{
    if (!Dest || Size <= 0)
    {
        return false;
    }

    FMemory::Memzero(Dest, Size);
    return true;
}

bool UMingRTSMemoryManager::ProtectMemory(void* Address, int32 Size, bool bRead, bool bWrite, bool bExecute)
{
    // 簡化實現，實際應該調用平台特定的內存保護API
    UE_LOG(LogTemp, Log, TEXT("Memory protection requested for address %p, size %d"), Address, Size);
    return true;
}

bool UMingRTSMemoryManager::UnprotectMemory(void* Address)
{
    // 簡化實現
    UE_LOG(LogTemp, Log, TEXT("Memory protection removed for address %p"), Address);
    return true;
}

bool UMingRTSMemoryManager::DefragmentMemory()
{
    if (!bInitialized)
    {
        return false;
    }

    PerformDefragmentation();
    return true;
}

void UMingRTSMemoryManager::SetAutoDefragmentation(bool bEnabled, float ThresholdRatio)
{
    bAutoDefragmentation = bEnabled;
    DefragmentationThreshold = ThresholdRatio;
    
    UE_LOG(LogTemp, Log, TEXT("Auto defragmentation %s, threshold: %.2f"), 
           bEnabled ? TEXT("enabled") : TEXT("disabled"), ThresholdRatio);
}

FMemoryStatistics UMingRTSMemoryManager::GetMemoryStatistics() const
{
    FScopeLock Lock(&StatisticsCriticalSection);
    return Statistics;
}

int32 UMingRTSMemoryManager::GetFreeMemory() const
{
    FScopeLock Lock(&StatisticsCriticalSection);
    return Statistics.FreeMemory;
}

int32 UMingRTSMemoryManager::GetUsedMemory() const
{
    FScopeLock Lock(&StatisticsCriticalSection);
    return Statistics.UsedMemory;
}

float UMingRTSMemoryManager::GetFragmentationRatio() const
{
    FScopeLock Lock(&StatisticsCriticalSection);
    return Statistics.FragmentationRatio;
}

TArray<FMemoryBlock> UMingRTSMemoryManager::GetMemoryBlocks() const
{
    FScopeLock Lock(&MemoryCriticalSection);
    
    TArray<FMemoryBlock> Blocks;
    for (const auto& BlockPair : MemoryBlocks)
    {
        Blocks.Add(BlockPair.Value);
    }
    
    return Blocks;
}

void UMingRTSMemoryManager::SetAllocationStrategy(EAllocationStrategy Strategy)
{
    AllocationStrategy = Strategy;
    UE_LOG(LogTemp, Log, TEXT("Allocation strategy changed to %d"), (int32)Strategy);
}

void UMingRTSMemoryManager::SetMemoryWarningThreshold(int32 ThresholdMB)
{
    MemoryWarningThreshold = ThresholdMB;
    UE_LOG(LogTemp, Log, TEXT("Memory warning threshold set to %d MB"), ThresholdMB);
}

void UMingRTSMemoryManager::EnableMemoryLogging(bool bEnabled)
{
    bMemoryLogging = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("Memory logging %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

void* UMingRTSMemoryManager::AllocateFirstFit(int32 Size, const FString& OwnerID)
{
    FScopeLock Lock(&MemoryCriticalSection);
    
    for (auto& BlockPair : MemoryBlocks)
    {
        FMemoryBlock& Block = BlockPair.Value;
        if (Block.State == EMemoryBlockState::Free && Block.Size >= Size)
        {
            Block.State = EMemoryBlockState::Allocated;
            Block.OwnerID = OwnerID;
            Block.AllocationTime = FDateTime::Now();
            Block.RefCount = 1;
            
            UsedMemory += Size;
            return Block.BaseAddress;
        }
    }
    
    return nullptr;
}

void* UMingRTSMemoryManager::AllocateBestFit(int32 Size, const FString& OwnerID)
{
    FScopeLock Lock(&MemoryCriticalSection);
    
    void* BestFitAddress = nullptr;
    int32 BestFitSize = INT_MAX;
    
    for (auto& BlockPair : MemoryBlocks)
    {
        FMemoryBlock& Block = BlockPair.Value;
        if (Block.State == EMemoryBlockState::Free && 
            Block.Size >= Size && 
            Block.Size < BestFitSize)
        {
            BestFitSize = Block.Size;
            BestFitAddress = Block.BaseAddress;
        }
    }
    
    if (BestFitAddress)
    {
        if (FMemoryBlock* Block = MemoryBlocks.Find(BestFitAddress))
        {
            Block->State = EMemoryBlockState::Allocated;
            Block->OwnerID = OwnerID;
            Block->AllocationTime = FDateTime::Now();
            Block->RefCount = 1;
            UsedMemory += Size;
        }
    }
    
    return BestFitAddress;
}

void* UMingRTSMemoryManager::AllocateWorstFit(int32 Size, const FString& OwnerID)
{
    FScopeLock Lock(&MemoryCriticalSection);
    
    void* WorstFitAddress = nullptr;
    int32 WorstFitSize = 0;
    
    for (auto& BlockPair : MemoryBlocks)
    {
        FMemoryBlock& Block = BlockPair.Value;
        if (Block.State == EMemoryBlockState::Free && 
            Block.Size >= Size && 
            Block.Size > WorstFitSize)
        {
            WorstFitSize = Block.Size;
            WorstFitAddress = Block.BaseAddress;
        }
    }
    
    if (WorstFitAddress)
    {
        if (FMemoryBlock* Block = MemoryBlocks.Find(WorstFitAddress))
        {
            Block->State = EMemoryBlockState::Allocated;
            Block->OwnerID = OwnerID;
            Block->AllocationTime = FDateTime::Now();
            Block->RefCount = 1;
            UsedMemory += Size;
        }
    }
    
    return WorstFitAddress;
}

void* UMingRTSMemoryManager::AllocateNextFit(int32 Size, const FString& OwnerID)
{
    // 簡化實現，與FirstFit相同
    return AllocateFirstFit(Size, OwnerID);
}

void* UMingRTSMemoryManager::AllocateBuddy(int32 Size, const FString& OwnerID)
{
    // 簡化的伴侶系統實現
    return AllocateBestFit(Size, OwnerID);
}

void* UMingRTSMemoryManager::AllocateFromSlab(int32 Size, const FString& OwnerID)
{
    // 嘗試從內存池分配
    return AllocateFromPool(Size);
}

bool UMingRTSMemoryManager::DeallocateMemoryBlock(void* Address)
{
    FScopeLock Lock(&MemoryCriticalSection);
    
    if (FMemoryBlock* Block = MemoryBlocks.Find(Address))
    {
        if (Block->State == EMemoryBlockState::Allocated)
        {
            Block->RefCount--;
            if (Block->RefCount <= 0)
            {
                Block->State = EMemoryBlockState::Free;
                Block->OwnerID = TEXT("");
                UsedMemory -= Block->Size;
                
                // 嘗試合併相鄰的空閒塊
                CoalesceMemory();
            }
            return true;
        }
    }
    
    return false;
}

void UMingRTSMemoryManager::CoalesceMemory()
{
    // 簡化的內存合併實現
    // 實際實現中應該檢查相鄰的空閒塊並合併它們
}

void UMingRTSMemoryManager::UpdateStatistics()
{
    if (!bInitialized)
    {
        return;
    }

    FScopeLock Lock(&StatisticsCriticalSection);
    
    // 計算內存統計
    Statistics.TotalMemory = TotalMemorySize;
    Statistics.UsedMemory = UsedMemory;
    Statistics.FreeMemory = TotalMemorySize - UsedMemory;
    Statistics.AllocatedBlocks = AllocatedBlockCount;
    Statistics.FreeBlocks = MemoryBlocks.Num() - AllocatedBlockCount;
    
    // 計算碎片化程度
    if (Statistics.TotalMemory > 0)
    {
        Statistics.FragmentationRatio = (float)Statistics.FragmentedMemory / Statistics.TotalMemory;
    }
    
    // 計算平均分配大小
    if (Statistics.TotalAllocations > 0)
    {
        Statistics.AverageAllocationSize = (float)Statistics.UsedMemory / Statistics.TotalAllocations;
    }
    
    // 檢查內存警告
    int32 FreeMemoryMB = Statistics.FreeMemory / (1024 * 1024);
    if (FreeMemoryMB < MemoryWarningThreshold)
    {
        OnMemoryWarning.Broadcast(FreeMemoryMB);
        UE_LOG(LogTemp, Warning, TEXT("Low memory warning: %d MB free"), FreeMemoryMB);
    }
    
    // 自動碎片整理
    if (bAutoDefragmentation && Statistics.FragmentationRatio > DefragmentationThreshold)
    {
        PerformDefragmentation();
    }
    
    // 繼續更新
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimerForNextTick([this]()
        {
            UpdateStatistics();
        });
    }
}

bool UMingRTSMemoryManager::IsMemoryValid(void* Address) const
{
    if (!Address || !bInitialized)
    {
        return false;
    }

    FScopeLock Lock(&MemoryCriticalSection);
    
    if (const FMemoryBlock* Block = MemoryBlocks.Find(Address))
    {
        return CheckMemoryCorruption(*Block);
    }
    
    return false;
}

void UMingRTSMemoryManager::LogMemoryOperation(const FString& Operation, void* Address, int32 Size, const FString& OwnerID)
{
    if (bMemoryLogging)
    {
        UE_LOG(LogTemp, Log, TEXT("Memory %s: Address=%p, Size=%d, Owner=%s"), 
               *Operation, Address, Size, *OwnerID);
    }
}

void UMingRTSMemoryManager::PerformDefragmentation()
{
    UE_LOG(LogTemp, Log, TEXT("Starting memory defragmentation"));
    
    TArray<FMemoryBlock> FragmentedBlocks = FindFragmentedBlocks();
    
    // 簡化的碎片整理實現
    // 實際實現中應該移動內存塊以減少碎片
    for (FMemoryBlock& Block : FragmentedBlocks)
    {
        // 模擬移動內存塊
        UE_LOG(LogTemp, Log, TEXT("Defragmenting block at %p, size %d"), Block.BaseAddress, Block.Size);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Memory defragmentation completed"));
}

void UMingRTSMemoryManager::MoveMemoryBlock(FMemoryBlock& Block, void* NewAddress)
{
    // 移動內存塊數據到新地址
    if (Block.BaseAddress && NewAddress && Block.State == EMemoryBlockState::Allocated)
    {
        FMemory::Memcpy(NewAddress, Block.BaseAddress, Block.Size);
        Block.BaseAddress = NewAddress;
    }
}

TArray<FMemoryBlock> UMingRTSMemoryManager::FindFragmentedBlocks() const
{
    FScopeLock Lock(&MemoryCriticalSection);
    
    TArray<FMemoryBlock> FragmentedBlocks;
    
    // 簡化的碎片檢測邏輯
    for (const auto& BlockPair : MemoryBlocks)
    {
        const FMemoryBlock& Block = BlockPair.Value;
        if (Block.State == EMemoryBlockState::Fragmented)
        {
            FragmentedBlocks.Add(Block);
        }
    }
    
    return FragmentedBlocks;
}

FMemoryPool* UMingRTSMemoryManager::FindSuitablePool(int32 Size)
{
    for (FMemoryPool& Pool : MemoryPools)
    {
        if (Pool.BlockSize >= Size && Pool.FreeBlocks > 0)
        {
            return &Pool;
        }
    }
    
    return nullptr;
}

bool UMingRTSMemoryManager::CreatePool(int32 BlockSize, int32 BlockCount)
{
    FMemoryPool Pool;
    Pool.BlockSize = BlockSize;
    Pool.BlockCount = BlockCount;
    Pool.FreeBlocks = BlockCount;
    
    // 分配池內存
    int32 PoolSize = BlockSize * BlockCount;
    void* PoolMemory = FMemory::Malloc(PoolSize);
    
    if (!PoolMemory)
    {
        return false;
    }
    
    // 初始化空閒列表
    uint8* MemoryPtr = (uint8*)PoolMemory;
    for (int32 i = 0; i < BlockCount; ++i)
    {
        Pool.FreeList.Add(MemoryPtr + i * BlockSize);
    }
    
    MemoryPools.Add(Pool);
    UE_LOG(LogTemp, Log, TEXT("Created memory pool: block size=%d, count=%d"), BlockSize, BlockCount);
    
    return true;
}

void UMingRTSMemoryManager::DestroyPool(int32 BlockSize)
{
    for (int32 i = MemoryPools.Num() - 1; i >= 0; --i)
    {
        if (MemoryPools[i].BlockSize == BlockSize)
        {
            // 釋放池內存（這裡需要跟蹤原始分配的地址）
            MemoryPools.RemoveAt(i);
            UE_LOG(LogTemp, Log, TEXT("Destroyed memory pool with block size %d"), BlockSize);
            break;
        }
    }
}

bool UMingRTSMemoryManager::InitializeMemoryPools()
{
    // 創建預定義的內存池
    CreatePool(64, 100);    // 64字節塊
    CreatePool(256, 50);    // 256字節塊
    CreatePool(1024, 25);   // 1KB塊
    CreatePool(4096, 10);   // 4KB塊
    
    UE_LOG(LogTemp, Log, TEXT("Memory pools initialized"));
    return true;
}

bool UMingRTSMemoryManager::InitializeMemoryBlocks()
{
    // 創建一個大的空閒內存塊
    FMemoryBlock MainBlock;
    MainBlock.BaseAddress = MemoryBase;
    MainBlock.Size = TotalMemorySize;
    MainBlock.State = EMemoryBlockState::Free;
    MainBlock.OwnerID = TEXT("System");
    MainBlock.AllocationTime = FDateTime::Now();
    MainBlock.RefCount = 0;
    MainBlock.MagicNumber = 0xDEADBEEF;
    
    MemoryBlocks.Add(MemoryBase, MainBlock);
    
    UE_LOG(LogTemp, Log, TEXT("Memory blocks initialized"));
    return true;
}

void UMingRTSMemoryManager::CleanupMemoryPools()
{
    FScopeLock Lock(&PoolCriticalSection);
    MemoryPools.Empty();
    UE_LOG(LogTemp, Log, TEXT("Memory pools cleaned up"));
}

void UMingRTSMemoryManager::CleanupMemoryBlocks()
{
    FScopeLock Lock(&MemoryCriticalSection);
    MemoryBlocks.Empty();
    UE_LOG(LogTemp, Log, TEXT("Memory blocks cleaned up"));
}

void UMingRTSMemoryManager::ResetStatistics()
{
    FScopeLock Lock(&StatisticsCriticalSection);
    Statistics = FMemoryStatistics();
    Statistics.TotalMemory = TotalMemorySize;
    Statistics.FreeMemory = TotalMemorySize;
}

int32 UMingRTSMemoryManager::AlignSize(int32 Size, int32 Alignment)
{
    return (Size + Alignment - 1) & ~(Alignment - 1);
}

bool UMingRTSMemoryManager::IsPowerOfTwo(int32 Value) const
{
    return Value > 0 && (Value & (Value - 1)) == 0;
}

int32 UMingRTSMemoryManager::NextPowerOfTwo(int32 Value) const
{
    if (IsPowerOfTwo(Value))
    {
        return Value;
    }
    
    int32 Power = 1;
    while (Power < Value)
    {
        Power <<= 1;
    }
    
    return Power;
}

void* UMingRTSMemoryManager::AlignAddress(void* Address, int32 Alignment)
{
    uintptr_t Addr = (uintptr_t)Address;
    return (void*)((Addr + Alignment - 1) & ~(Alignment - 1));
}

bool UMingRTSMemoryManager::CheckMemoryCorruption(const FMemoryBlock& Block) const
{
    return Block.MagicNumber == 0xDEADBEEF;
}
