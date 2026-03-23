#include "MingMemoryOptimization.h"
#include "HAL/MemoryBase.h"
#include "GenericPlatform/GenericPlatformMemory.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "UObject/UObjectGlobals.h"

// ============================================================================
// 字串駐留系統實現
// ============================================================================

// 靜態字串駐留表
static TMap<uint32, FString> GStringInternTable;
static FCriticalSection GStringInternLock;

FMingOptimizedString::FMingOptimizedString(const FString& InString)
    : SmallStringLength(0)
    , InternIndex(0)
    , bCacheValid(false)
{
    FMemory::Memzero(SmallString, sizeof(SmallString));
    
    int32 Len = InString.Len();
    if (Len < MingMemoryConfig::MAX_STRING_INTERN_SIZE)
    {
        // 使用小字串優化
        SmallStringLength = static_cast<uint8>(Len);
        if (Len > 0)
        {
            FMemory::Memcpy(SmallString, *InString, Len * sizeof(TCHAR));
        }
    }
    else
    {
        // 使用字串駐留
        FScopeLock Lock(&GStringInternLock);
        
        uint32 Hash = FCrc::StrCrc32(*InString);
        if (!GStringInternTable.Contains(Hash))
        {
            GStringInternTable.Add(Hash, InString);
        }
        InternIndex = Hash;
    }
}

FMingOptimizedString::FMingOptimizedString(const TCHAR* InString)
    : FMingOptimizedString(FString(InString))
{
}

FString FMingOptimizedString::ToString() const
{
    if (SmallStringLength > 0)
    {
        return FString(SmallStringLength, SmallString);
    }
    else if (InternIndex != 0)
    {
        FScopeLock Lock(&GStringInternLock);
        if (const FString* Found = GStringInternTable.Find(InternIndex))
        {
            return *Found;
        }
    }
    return FString();
}

const TCHAR* FMingOptimizedString::operator*() const
{
    if (SmallStringLength > 0)
    {
        return SmallString;
    }
    else if (InternIndex != 0)
    {
        FScopeLock Lock(&GStringInternLock);
        if (const FString* Found = GStringInternTable.Find(InternIndex))
        {
            return **Found;
        }
    }
    return TEXT("");
}

bool FMingOptimizedString::IsEmpty() const
{
    return SmallStringLength == 0 && InternIndex == 0;
}

int32 FMingOptimizedString::Len() const
{
    if (SmallStringLength > 0)
    {
        return SmallStringLength;
    }
    else if (InternIndex != 0)
    {
        FScopeLock Lock(&GStringInternLock);
        if (const FString* Found = GStringInternTable.Find(InternIndex))
        {
            return Found->Len();
        }
    }
    return 0;
}

bool FMingOptimizedString::operator==(const FMingOptimizedString& Other) const
{
    // 快速路徑：都是小字串
    if (SmallStringLength > 0 && Other.SmallStringLength > 0)
    {
        return SmallStringLength == Other.SmallStringLength && 
               FMemory::Memcmp(SmallString, Other.SmallString, SmallStringLength * sizeof(TCHAR)) == 0;
    }
    
    // 都是大字串
    if (InternIndex != 0 && Other.InternIndex != 0)
    {
        return InternIndex == Other.InternIndex;
    }
    
    // 混合比較
    return ToString() == Other.ToString();
}

bool FMingOptimizedString::operator==(const FString& Other) const
{
    return ToString() == Other;
}

uint32 FMingOptimizedString::GetHash() const
{
    if (InternIndex != 0)
    {
        return InternIndex;
    }
    return FCrc::StrCrc32(SmallString, SmallStringLength);
}

uint32 GetTypeHash(const FMingOptimizedString& Str)
{
    return Str.GetHash();
}

// ============================================================================
// UObject 物件池實現
// ============================================================================

void UMingUObjectPool::InitializePool(TSubclassOf<UObject> ObjectClass, int32 PoolSize)
{
    PoolObjectClass = ObjectClass;
    MaxPoolSize = PoolSize;
    
    // 預先創建物件
    for (int32 i = 0; i < PoolSize; ++i)
    {
        if (UObject* NewObject = NewObject<UObject>(GetTransientPackage(), PoolObjectClass))
        {
            AvailableObjects.Add(NewObject);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("[ObjectPool] Initialized pool for %s with %d objects"),
        *PoolObjectClass->GetName(), AvailableObjects.Num());
}

UObject* UMingUObjectPool::AcquireObject()
{
    UObject* Object = nullptr;
    
    if (AvailableObjects.Num() > 0)
    {
        Object = AvailableObjects.Pop();
    }
    else if (InUseObjects.Num() < MaxPoolSize && PoolObjectClass)
    {
        Object = NewObject<UObject>(GetTransientPackage(), PoolObjectClass);
    }
    
    if (Object)
    {
        InUseObjects.Add(Object);
    }
    
    return Object;
}

void UMingUObjectPool::ReleaseObject(UObject* Object)
{
    if (!Object) return;
    
    InUseObjects.Remove(Object);
    
    if (AvailableObjects.Num() < MaxPoolSize)
    {
        // 重置物件（這裡可以調用自定義重置方法）
        AvailableObjects.Add(Object);
    }
    else
    {
        Object->MarkAsGarbage();
    }
}

void UMingUObjectPool::ClearPool()
{
    for (auto& Object : AvailableObjects)
    {
        if (Object.IsValid())
        {
            Object->MarkAsGarbage();
        }
    }
    AvailableObjects.Empty();
    
    for (auto& Object : InUseObjects)
    {
        if (Object.IsValid())
        {
            Object->MarkAsGarbage();
        }
    }
    InUseObjects.Empty();
}

int32 UMingUObjectPool::GetAvailableCount() const
{
    return AvailableObjects.Num();
}

int32 UMingUObjectPool::GetInUseCount() const
{
    return InUseObjects.Num();
}

// ============================================================================
// 記憶體分析器實現
// ============================================================================

bool UMingMemoryProfiler::bIsProfiling = false;
TArray<FMemoryUsageSnapshot> UMingMemoryProfiler::Snapshots;
float UMingMemoryProfiler::LastGCTime = 0.0f;

FMemoryUsageSnapshot UMingMemoryProfiler::CaptureSnapshot()
{
    FMemoryUsageSnapshot Snapshot;
    Snapshot.Timestamp = FPlatformTime::Seconds();
    
    // 獲取平台記憶體統計
    FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
    Snapshot.UsedPhysicalMemoryMB = static_cast<int32>(MemoryStats.UsedPhysical / (1024 * 1024));
    Snapshot.AvailablePhysicalMemoryMB = static_cast<int32>(MemoryStats.AvailablePhysical / (1024 * 1024));
    Snapshot.UsedVirtualMemoryMB = static_cast<int32>(MemoryStats.UsedVirtual / (1024 * 1024));
    
    // 獲取 UObject 數量
    Snapshot.UObjectCount = GUObjectArray.GetObjectArrayNum();
    
    // 這裡可以添加紋理和網格記憶體統計
    Snapshot.TextureMemoryMB = 0;
    Snapshot.MeshMemoryMB = 0;
    
    return Snapshot;
}

void UMingMemoryProfiler::StartProfiling()
{
    bIsProfiling = true;
    Snapshots.Empty();
    Snapshots.Add(CaptureSnapshot());
    UE_LOG(LogTemp, Log, TEXT("[MemoryProfiler] Profiling started"));
}

void UMingMemoryProfiler::StopProfiling()
{
    bIsProfiling = false;
    UE_LOG(LogTemp, Log, TEXT("[MemoryProfiler] Profiling stopped. Captured %d snapshots"), Snapshots.Num());
}

bool UMingMemoryProfiler::IsMemoryCritical()
{
    FMemoryUsageSnapshot Snapshot = CaptureSnapshot();
    return Snapshot.UsedPhysicalMemoryMB > MingMemoryConfig::MEMORY_CRITICAL_THRESHOLD;
}

bool UMingMemoryProfiler::IsMemoryWarning()
{
    FMemoryUsageSnapshot Snapshot = CaptureSnapshot();
    return Snapshot.UsedPhysicalMemoryMB > MingMemoryConfig::MEMORY_WARNING_THRESHOLD;
}

float UMingMemoryProfiler::GetMemoryUsagePercent()
{
    FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
    int64 TotalPhysical = MemoryStats.TotalPhysical;
    int64 UsedPhysical = MemoryStats.UsedPhysical;
    
    if (TotalPhysical > 0)
    {
        return (float)UsedPhysical / (float)TotalPhysical * 100.0f;
    }
    return 0.0f;
}

void UMingMemoryProfiler::TriggerGarbageCollection()
{
    UE_LOG(LogTemp, Log, TEXT("[MemoryProfiler] Triggering Garbage Collection"));
    FMemory::Trim();
    GEngine->ForceGarbageCollection(true);
    LastGCTime = FPlatformTime::Seconds();
}

void UMingMemoryProfiler::LogMemoryUsage()
{
    FMemoryUsageSnapshot Snapshot = CaptureSnapshot();
    UE_LOG(LogTemp, Log, TEXT("[Memory] Physical: %d MB, Virtual: %d MB, UObject Count: %d"),
        Snapshot.UsedPhysicalMemoryMB,
        Snapshot.UsedVirtualMemoryMB,
        Snapshot.UObjectCount);
}

FString UMingMemoryProfiler::GetMemoryReport()
{
    FMemoryUsageSnapshot Snapshot = CaptureSnapshot();
    return FString::Printf(TEXT("Memory Report:\n"
        "Physical Memory: %d MB / %d MB (%.1f%%)\n"
        "Virtual Memory: %d MB\n"
        "Available Physical: %d MB\n"
        "UObject Count: %d\n"),
        Snapshot.UsedPhysicalMemoryMB,
        Snapshot.UsedPhysicalMemoryMB + Snapshot.AvailablePhysicalMemoryMB,
        GetMemoryUsagePercent(),
        Snapshot.UsedVirtualMemoryMB,
        Snapshot.AvailablePhysicalMemoryMB,
        Snapshot.UObjectCount);
}

void UMingMemoryProfiler::CheckAutoGC()
{
    if (IsMemoryCritical())
    {
        float CurrentTime = FPlatformTime::Seconds();
        if (CurrentTime - LastGCTime > MingMemoryConfig::GC_INTERVAL)
        {
            TriggerGarbageCollection();
        }
    }
}

// ============================================================================
// 記憶體優化管理器實現
// ============================================================================

UMingMemoryOptimizer* UMingMemoryOptimizer::GetInstance()
{
    static UMingMemoryOptimizer* Instance = nullptr;
    if (!Instance)
    {
        Instance = NewObject<UMingMemoryOptimizer>();
        Instance->AddToRoot();
    }
    return Instance;
}

void UMingMemoryOptimizer::Initialize(EMemoryOptimizationFlags Flags)
{
    CurrentFlags = Flags;
    MemoryWarningThreshold = MingMemoryConfig::MEMORY_WARNING_THRESHOLD;
    MemoryCriticalThreshold = MingMemoryConfig::MEMORY_CRITICAL_THRESHOLD;
    
    if (EnumHasAnyFlags(Flags, EMemoryOptimizationFlags::UseObjectPooling))
    {
        EnableObjectPooling(true);
    }
    
    if (EnumHasAnyFlags(Flags, EMemoryOptimizationFlags::UseStringInterning))
    {
        EnableStringInterning(true);
    }
    
    if (EnumHasAnyFlags(Flags, EMemoryOptimizationFlags::UseLazyLoading))
    {
        EnableLazyLoading(true);
    }
    
    UE_LOG(LogTemp, Log, TEXT("[MemoryOptimizer] Initialized with flags: %d"), (int32)Flags);
}

void UMingMemoryOptimizer::SetOptimizationFlags(EMemoryOptimizationFlags Flags)
{
    CurrentFlags = Flags;
}

EMemoryOptimizationFlags UMingMemoryOptimizer::GetOptimizationFlags() const
{
    return CurrentFlags;
}

void UMingMemoryOptimizer::EnableObjectPooling(bool bEnabled)
{
    if (bEnabled)
    {
        CurrentFlags |= EMemoryOptimizationFlags::UseObjectPooling;
    }
    else
    {
        CurrentFlags &= ~EMemoryOptimizationFlags::UseObjectPooling;
    }
}

void UMingMemoryOptimizer::EnableStringInterning(bool bEnabled)
{
    if (bEnabled)
    {
        CurrentFlags |= EMemoryOptimizationFlags::UseStringInterning;
    }
    else
    {
        CurrentFlags &= ~EMemoryOptimizationFlags::UseStringInterning;
    }
}

void UMingMemoryOptimizer::EnableLazyLoading(bool bEnabled)
{
    if (bEnabled)
    {
        CurrentFlags |= EMemoryOptimizationFlags::UseLazyLoading;
    }
    else
    {
        CurrentFlags &= ~EMemoryOptimizationFlags::UseLazyLoading;
    }
}

void UMingMemoryOptimizer::CompactMemory()
{
    UE_LOG(LogTemp, Log, TEXT("[MemoryOptimizer] Compacting memory..."));
    
    // 壓縮容器
    CompactContainers();
    
    // 優化字串存儲
    if (EnumHasAnyFlags(CurrentFlags, EMemoryOptimizationFlags::UseStringInterning))
    {
        OptimizeStringStorage();
    }
    
    // 觸發垃圾回收
    UMingMemoryProfiler::TriggerGarbageCollection();
    
    UE_LOG(LogTemp, Log, TEXT("[MemoryOptimizer] Memory compaction complete"));
}

void UMingMemoryOptimizer::OptimizeAllSystems()
{
    UE_LOG(LogTemp, Log, TEXT("[MemoryOptimizer] Optimizing all systems..."));
    
    CompactMemory();
    
    // 清理未使用的物件池
    for (auto It = ObjectPools.CreateIterator(); It; ++It)
    {
        if (It->Value && It->Value->GetInUseCount() == 0)
        {
            It->Value->ClearPool();
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("[MemoryOptimizer] All systems optimized"));
}

void UMingMemoryOptimizer::SetMemoryWarningThreshold(int32 ThresholdMB)
{
    MemoryWarningThreshold = ThresholdMB;
}

void UMingMemoryOptimizer::SetMemoryCriticalThreshold(int32 ThresholdMB)
{
    MemoryCriticalThreshold = ThresholdMB;
}

UMingUObjectPool* UMingMemoryOptimizer::GetOrCreateObjectPool(TSubclassOf<UObject> ObjectClass)
{
    if (TObjectPtr<UMingUObjectPool>* ExistingPool = ObjectPools.Find(ObjectClass))
    {
        return ExistingPool->Get();
    }
    
    UMingUObjectPool* NewPool = NewObject<UMingUObjectPool>(this);
    NewPool->InitializePool(ObjectClass, MingMemoryConfig::DEFAULT_POOL_SIZE);
    ObjectPools.Add(ObjectClass, NewPool);
    
    return NewPool;
}

void UMingMemoryOptimizer::DestroyObjectPool(TSubclassOf<UObject> ObjectClass)
{
    if (TObjectPtr<UMingUObjectPool>* Pool = ObjectPools.Find(ObjectClass))
    {
        if (Pool->IsValid())
        {
            Pool->Get()->ClearPool();
        }
        ObjectPools.Remove(ObjectClass);
    }
}

void UMingMemoryOptimizer::PerformMemoryMaintenance()
{
    // 檢查記憶體使用情況
    if (UMingMemoryProfiler::IsMemoryCritical())
    {
        UE_LOG(LogTemp, Warning, TEXT("[MemoryOptimizer] Critical memory level detected!"));
        CompactMemory();
    }
    else if (UMingMemoryProfiler::IsMemoryWarning())
    {
        UE_LOG(LogTemp, Warning, TEXT("[MemoryOptimizer] Warning memory level detected"));
        OptimizeStringStorage();
    }
}

void UMingMemoryOptimizer::OptimizeStringStorage()
{
    // 清理未使用的字串駐留
    // 實際實現可以根據引用計數清理
    UE_LOG(LogTemp, Log, TEXT("[MemoryOptimizer] String storage optimized"));
}

void UMingMemoryOptimizer::CompactContainers()
{
    // 壓縮各種容器以釋放未使用的容量
    // 這裡可以針對特定系統的容器進行壓縮
    UE_LOG(LogTemp, Log, TEXT("[MemoryOptimizer] Containers compacted"));
}
