#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "Misc/EnumClassFlags.h"
#include "MingMemoryOptimization.generated.h"

// ============================================================================
// 記憶體優化配置常量
// ============================================================================

/** 記憶體池配置 */
namespace MingMemoryConfig
{
    // 物件池大小限制
    constexpr int32 MAX_POOL_SIZE = 1024;
    constexpr int32 DEFAULT_POOL_SIZE = 256;
    
    // 字串優化
    constexpr int32 MAX_STRING_INTERN_SIZE = 64;
    constexpr int32 STRING_INTERN_CAPACITY = 4096;
    
    // 陣列預分配
    constexpr int32 SMALL_ARRAY_CAPACITY = 8;
    constexpr int32 MEDIUM_ARRAY_CAPACITY = 32;
    constexpr int32 LARGE_ARRAY_CAPACITY = 128;
    
    // 記憶體警告閾值 (MB)
    constexpr int32 MEMORY_WARNING_THRESHOLD = 512;
    constexpr int32 MEMORY_CRITICAL_THRESHOLD = 1024;
    
    // 垃圾回收間隔 (秒)
    constexpr float GC_INTERVAL = 30.0f;
}

// ============================================================================
// 記憶體優化標誌
// ============================================================================

UENUM(BlueprintType, meta = (Flags, "UseEnumValuesAsMaskValuesInEditor"))
enum class EMemoryOptimizationFlags : uint8
{
    None = 0,
    UseObjectPooling = 1 << 0,      // 使用物件池
    UseStringInterning = 1 << 1,     // 使用字串駐留
    UseLazyLoading = 1 << 2,         // 使用延遲載入
    UseMemoryCompaction = 1 << 3,    // 使用記憶體壓縮
    UseSparseArrays = 1 << 4,        // 使用稀疏陣列
    UseBitpacking = 1 << 5,          // 使用位元封裝
    UseStaticAllocation = 1 << 6,    // 使用靜態分配
    AggressiveOptimization = 1 << 7  // 激進優化模式
};
ENUM_CLASS_FLAGS(EMemoryOptimizationFlags);

// ============================================================================
// 優化的FString - 字串駐留系統
// ============================================================================

/**
 * 優化的字串類別
 * 對於短字串使用內部存儲，對於長字串使用駐留
 */
USTRUCT(BlueprintType)
struct MINGGORTS_API FMingOptimizedString
{
    GENERATED_BODY()

private:
    // 小字串優化 (SSO) - 直接存儲在結構內
    TCHAR SmallString[MingMemoryConfig::MAX_STRING_INTERN_SIZE];
    uint8 SmallStringLength;
    
    // 大字串使用駐留索引
    uint32 InternIndex;
    mutable FString CachedString;
    mutable bool bCacheValid;
    
public:
    FMingOptimizedString()
        : SmallStringLength(0)
        , InternIndex(0)
        , bCacheValid(false)
    {
        FMemory::Memzero(SmallString, sizeof(SmallString));
    }
    
    FMingOptimizedString(const FString& InString);
    FMingOptimizedString(const TCHAR* InString);
    
    FString ToString() const;
    const TCHAR* operator*() const;
    bool IsEmpty() const;
    int32 Len() const;
    
    bool operator==(const FMingOptimizedString& Other) const;
    bool operator==(const FString& Other) const;
    uint32 GetHash() const;
};

// ============================================================================
// 優化的TMap - 固定大小哈希表
// ============================================================================

/**
 * 固定大小的優化映射表
 * 適合已知最大大小的場景，避免動態重新分配
 */
template<typename KeyType, typename ValueType, int32 MaxSize>
class TMingFixedMap
{
private:
    struct FEntry
    {
        KeyType Key;
        ValueType Value;
        bool bUsed;
        uint32 Hash;
        
        FEntry() : bUsed(false), Hash(0) {}
    };
    
    FEntry Entries[MaxSize];
    int32 NumEntries;
    
    uint32 HashKey(const KeyType& Key) const
    {
        return GetTypeHash(Key);
    }
    
    int32 FindSlot(uint32 Hash, const KeyType& Key) const
    {
        int32 StartSlot = Hash % MaxSize;
        for (int32 i = 0; i < MaxSize; ++i)
        {
            int32 Slot = (StartSlot + i) % MaxSize;
            if (!Entries[Slot].bUsed || (Entries[Slot].Hash == Hash && Entries[Slot].Key == Key))
            {
                return Slot;
            }
        }
        return -1;
    }

public:
    TMingFixedMap() : NumEntries(0) {}
    
    FORCEINLINE int32 Num() const { return NumEntries; }
    FORCEINLINE bool IsEmpty() const { return NumEntries == 0; }
    FORCEINLINE int32 Capacity() const { return MaxSize; }
    
    bool Contains(const KeyType& Key) const
    {
        uint32 Hash = HashKey(Key);
        int32 Slot = FindSlot(Hash, Key);
        return Slot >= 0 && Entries[Slot].bUsed;
    }
    
    ValueType* Find(const KeyType& Key)
    {
        uint32 Hash = HashKey(Key);
        int32 Slot = FindSlot(Hash, Key);
        if (Slot >= 0 && Entries[Slot].bUsed)
        {
            return &Entries[Slot].Value;
        }
        return nullptr;
    }
    
    const ValueType* Find(const KeyType& Key) const
    {
        uint32 Hash = HashKey(Key);
        int32 Slot = FindSlot(Hash, Key);
        if (Slot >= 0 && Entries[Slot].bUsed)
        {
            return &Entries[Slot].Value;
        }
        return nullptr;
    }
    
    bool Add(const KeyType& Key, const ValueType& Value)
    {
        if (NumEntries >= MaxSize) return false;
        
        uint32 Hash = HashKey(Key);
        int32 Slot = FindSlot(Hash, Key);
        if (Slot < 0) return false;
        
        if (!Entries[Slot].bUsed)
        {
            ++NumEntries;
        }
        
        Entries[Slot].Key = Key;
        Entries[Slot].Value = Value;
        Entries[Slot].bUsed = true;
        Entries[Slot].Hash = Hash;
        return true;
    }
    
    bool Remove(const KeyType& Key)
    {
        uint32 Hash = HashKey(Key);
        int32 Slot = FindSlot(Hash, Key);
        if (Slot >= 0 && Entries[Slot].bUsed)
        {
            Entries[Slot].bUsed = false;
            --NumEntries;
            return true;
        }
        return false;
    }
    
    void Empty()
    {
        for (int32 i = 0; i < MaxSize; ++i)
        {
            Entries[i].bUsed = false;
        }
        NumEntries = 0;
    }
    
    // 迭代器
    class Iterator
    {
        const TMingFixedMap& Map;
        int32 CurrentIndex;
        
    public:
        Iterator(const TMingFixedMap& InMap, int32 StartIndex)
            : Map(InMap), CurrentIndex(StartIndex)
        {
            AdvanceToUsed();
        }
        
        void AdvanceToUsed()
        {
            while (CurrentIndex < MaxSize && !Map.Entries[CurrentIndex].bUsed)
            {
                ++CurrentIndex;
            }
        }
        
        bool operator!=(const Iterator& Other) const { return CurrentIndex != Other.CurrentIndex; }
        void operator++() { ++CurrentIndex; AdvanceToUsed(); }
        
        const KeyType& Key() const { return Map.Entries[CurrentIndex].Key; }
        ValueType& Value() { return const_cast<ValueType&>(Map.Entries[CurrentIndex].Value); }
        const ValueType& Value() const { return Map.Entries[CurrentIndex].Value; }
    };
    
    Iterator begin() const { return Iterator(*this, 0); }
    Iterator end() const { return Iterator(*this, MaxSize); }
};

// ============================================================================
// 優化的TArray - 預分配版本
// ============================================================================

/**
 * 固定容量的優化陣列
 * 適合已知最大大小的場景，避免動態重新分配
 */
template<typename ElementType, int32 MaxCapacity>
class TMingFixedArray
{
private:
    ElementType Data[MaxCapacity];
    int32 NumElements;

public:
    TMingFixedArray() : NumElements(0) {}
    
    FORCEINLINE int32 Num() const { return NumElements; }
    FORCEINLINE int32 Capacity() const { return MaxCapacity; }
    FORCEINLINE bool IsEmpty() const { return NumElements == 0; }
    FORCEINLINE bool IsFull() const { return NumElements >= MaxCapacity; }
    
    ElementType& operator[](int32 Index)
    {
        check(Index >= 0 && Index < NumElements);
        return Data[Index];
    }
    
    const ElementType& operator[](int32 Index) const
    {
        check(Index >= 0 && Index < NumElements);
        return Data[Index];
    }
    
    bool Add(const ElementType& Element)
    {
        if (NumElements >= MaxCapacity) return false;
        Data[NumElements++] = Element;
        return true;
    }
    
    bool AddUnique(const ElementType& Element)
    {
        for (int32 i = 0; i < NumElements; ++i)
        {
            if (Data[i] == Element) return false;
        }
        return Add(Element);
    }
    
    bool Insert(const ElementType& Element, int32 Index)
    {
        if (NumElements >= MaxCapacity || Index < 0 || Index > NumElements) return false;
        
        for (int32 i = NumElements; i > Index; --i)
        {
            Data[i] = Data[i - 1];
        }
        Data[Index] = Element;
        ++NumElements;
        return true;
    }
    
    bool Remove(const ElementType& Element)
    {
        for (int32 i = 0; i < NumElements; ++i)
        {
            if (Data[i] == Element)
            {
                RemoveAt(i);
                return true;
            }
        }
        return false;
    }
    
    void RemoveAt(int32 Index)
    {
        check(Index >= 0 && Index < NumElements);
        for (int32 i = Index; i < NumElements - 1; ++i)
        {
            Data[i] = Data[i + 1];
        }
        --NumElements;
    }
    
    void Empty()
    {
        NumElements = 0;
    }
    
    int32 Find(const ElementType& Element) const
    {
        for (int32 i = 0; i < NumElements; ++i)
        {
            if (Data[i] == Element) return i;
        }
        return INDEX_NONE;
    }
    
    bool Contains(const ElementType& Element) const
    {
        return Find(Element) != INDEX_NONE;
    }
    
    ElementType* GetData() { return Data; }
    const ElementType* GetData() const { return Data; }
    
    ElementType* begin() { return Data; }
    ElementType* end() { return Data + NumElements; }
    const ElementType* begin() const { return Data; }
    const ElementType* end() const { return Data + NumElements; }
};

// ============================================================================
// 物件池系統
// ============================================================================

/**
 * 泛型物件池
 * 用於高效重用物件，減少記憶體分配開銷
 */
template<typename ObjectType>
class TMingObjectPool
{
private:
    TArray<ObjectType*> AvailableObjects;
    TArray<ObjectType*> InUseObjects;
    int32 MaxPoolSize;
    int32 InitialSize;
    
public:
    TMingObjectPool(int32 InInitialSize = MingMemoryConfig::DEFAULT_POOL_SIZE, 
                    int32 InMaxPoolSize = MingMemoryConfig::MAX_POOL_SIZE)
        : MaxPoolSize(InMaxPoolSize)
        , InitialSize(InInitialSize)
    {
        PreallocateObjects();
    }
    
    ~TMingObjectPool()
    {
        Empty();
    }
    
    void PreallocateObjects()
    {
        for (int32 i = 0; i < InitialSize; ++i)
        {
            if (AvailableObjects.Num() < MaxPoolSize)
            {
                ObjectType* NewObject = new ObjectType();
                AvailableObjects.Add(NewObject);
            }
        }
    }
    
    ObjectType* Acquire()
    {
        ObjectType* Object = nullptr;
        
        if (AvailableObjects.Num() > 0)
        {
            Object = AvailableObjects.Pop();
        }
        else if (InUseObjects.Num() < MaxPoolSize)
        {
            Object = new ObjectType();
        }
        
        if (Object)
        {
            InUseObjects.Add(Object);
        }
        
        return Object;
    }
    
    void Release(ObjectType* Object)
    {
        if (!Object) return;
        
        InUseObjects.Remove(Object);
        
        if (AvailableObjects.Num() < MaxPoolSize)
        {
            // 重置物件狀態
            *Object = ObjectType();
            AvailableObjects.Add(Object);
        }
        else
        {
            delete Object;
        }
    }
    
    void Empty()
    {
        for (ObjectType* Object : AvailableObjects)
        {
            delete Object;
        }
        AvailableObjects.Empty();
        
        for (ObjectType* Object : InUseObjects)
        {
            delete Object;
        }
        InUseObjects.Empty();
    }
    
    int32 GetAvailableCount() const { return AvailableObjects.Num(); }
    int32 GetInUseCount() const { return InUseObjects.Num(); }
    int32 GetTotalCount() const { return AvailableObjects.Num() + InUseObjects.Num(); }
};

// ============================================================================
// UObject 物件池 (UE5特定)
// ============================================================================

/**
 * UObject 物件池系統
 * 專為 UE5 UObject 優化的物件池
 */
UCLASS()
class MINGGORTS_API UMingUObjectPool : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Memory|ObjectPool")
    void InitializePool(TSubclassOf<UObject> ObjectClass, int32 PoolSize);
    
    UFUNCTION(BlueprintCallable, Category = "Memory|ObjectPool")
    UObject* AcquireObject();
    
    UFUNCTION(BlueprintCallable, Category = "Memory|ObjectPool")
    void ReleaseObject(UObject* Object);
    
    UFUNCTION(BlueprintCallable, Category = "Memory|ObjectPool")
    void ClearPool();
    
    UFUNCTION(BlueprintCallable, Category = "Memory|ObjectPool")
    int32 GetAvailableCount() const;
    
    UFUNCTION(BlueprintCallable, Category = "Memory|ObjectPool")
    int32 GetInUseCount() const;
    
private:
    UPROPERTY()
    TArray<TObjectPtr<UObject>> AvailableObjects;
    
    UPROPERTY()
    TArray<TObjectPtr<UObject>> InUseObjects;
    
    UPROPERTY()
    TSubclassOf<UObject> PoolObjectClass;
    
    int32 MaxPoolSize;
};

// ============================================================================
// 記憶體分析器
// ============================================================================

USTRUCT(BlueprintType)
struct FMemoryUsageSnapshot
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    float Timestamp;
    
    UPROPERTY(BlueprintReadOnly)
    int32 UsedPhysicalMemoryMB;
    
    UPROPERTY(BlueprintReadOnly)
    int32 AvailablePhysicalMemoryMB;
    
    UPROPERTY(BlueprintReadOnly)
    int32 UsedVirtualMemoryMB;
    
    UPROPERTY(BlueprintReadOnly)
    int32 UObjectCount;
    
    UPROPERTY(BlueprintReadOnly)
    int32 TextureMemoryMB;
    
    UPROPERTY(BlueprintReadOnly)
    int32 MeshMemoryMB;
};

/**
 * 記憶體分析與監控系統
 */
UCLASS()
class MINGGORTS_API UMingMemoryProfiler : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Memory|Profiler")
    static FMemoryUsageSnapshot CaptureSnapshot();
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Profiler")
    static void StartProfiling();
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Profiler")
    static void StopProfiling();
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Profiler")
    static bool IsMemoryCritical();
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Profiler")
    static bool IsMemoryWarning();
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Profiler")
    static float GetMemoryUsagePercent();
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Profiler")
    static void TriggerGarbageCollection();
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Profiler")
    static void LogMemoryUsage();
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Profiler")
    static FString GetMemoryReport();
    
private:
    static bool bIsProfiling;
    static TArray<FMemoryUsageSnapshot> Snapshots;
    static float LastGCTime;
    
    static void CheckAutoGC();
};

// ============================================================================
// 記憶體優化管理器
// ============================================================================

/**
 * 中央記憶體優化管理器
 * 協調所有記憶體優化功能
 */
UCLASS()
class MINGGORTS_API UMingMemoryOptimizer : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Memory|Optimizer")
    static UMingMemoryOptimizer* GetInstance();
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Optimizer")
    void Initialize(EMemoryOptimizationFlags Flags);
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Optimizer")
    void SetOptimizationFlags(EMemoryOptimizationFlags Flags);
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Optimizer")
    EMemoryOptimizationFlags GetOptimizationFlags() const;
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Optimizer")
    void EnableObjectPooling(bool bEnabled);
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Optimizer")
    void EnableStringInterning(bool bEnabled);
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Optimizer")
    void EnableLazyLoading(bool bEnabled);
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Optimizer")
    void CompactMemory();
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Optimizer")
    void OptimizeAllSystems();
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Optimizer")
    void SetMemoryWarningThreshold(int32 ThresholdMB);
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Optimizer")
    void SetMemoryCriticalThreshold(int32 ThresholdMB);
    
    // UObject池管理
    UFUNCTION(BlueprintCallable, Category = "Memory|Optimizer")
    UMingUObjectPool* GetOrCreateObjectPool(TSubclassOf<UObject> ObjectClass);
    
    UFUNCTION(BlueprintCallable, Category = "Memory|Optimizer")
    void DestroyObjectPool(TSubclassOf<UObject> ObjectClass);

private:
    UPROPERTY()
    EMemoryOptimizationFlags CurrentFlags;
    
    UPROPERTY()
    TMap<TSubclassOf<UObject>, TObjectPtr<UMingUObjectPool>> ObjectPools;
    
    int32 MemoryWarningThreshold;
    int32 MemoryCriticalThreshold;
    
    void OnWorldTick(float DeltaTime);
    void PerformMemoryMaintenance();
    void OptimizeStringStorage();
    void CompactContainers();
};

// ============================================================================
// 輔助宏定義
// ============================================================================

/** 使用優化容器的快捷宏 */
#define MING_SMALL_ARRAY(Type, Name) TMingFixedArray<Type, MingMemoryConfig::SMALL_ARRAY_CAPACITY> Name
#define MING_MEDIUM_ARRAY(Type, Name) TMingFixedArray<Type, MingMemoryConfig::MEDIUM_ARRAY_CAPACITY> Name
#define MING_LARGE_ARRAY(Type, Name) TMingFixedArray<Type, MingMemoryConfig::LARGE_ARRAY_CAPACITY> Name

#define MING_SMALL_MAP(Key, Value, Name) TMingFixedMap<Key, Value, MingMemoryConfig::SMALL_ARRAY_CAPACITY> Name
#define MING_MEDIUM_MAP(Key, Value, Name) TMingFixedMap<Key, Value, MingMemoryConfig::MEDIUM_ARRAY_CAPACITY> Name
#define MING_LARGE_MAP(Key, Value, Name) TMingFixedMap<Key, Value, MingMemoryConfig::LARGE_ARRAY_CAPACITY> Name

/** 記憶體分析輔助宏 */
#define MING_MEMORY_SCOPE(Name) \
    struct FMingMemoryScope_##Name { \
        FMemoryUsageSnapshot StartSnapshot; \
        FString ScopeName; \
        FMingMemoryScope_##Name(const FString& InName) : ScopeName(InName) { \
            StartSnapshot = UMingMemoryProfiler::CaptureSnapshot(); \
        } \
        ~FMingMemoryScope_##Name() { \
            FMemoryUsageSnapshot EndSnapshot = UMingMemoryProfiler::CaptureSnapshot(); \
            int32 DeltaMB = EndSnapshot.UsedPhysicalMemoryMB - StartSnapshot.UsedPhysicalMemoryMB; \
            UE_LOG(LogTemp, Log, TEXT("[Memory] %s: %d MB %s"), *ScopeName, FMath::Abs(DeltaMB), \
                DeltaMB > 0 ? TEXT("increase") : TEXT("decrease")); \
        } \
    } MingMemoryScope_##Name(TEXT(#Name))

/** 物件池輔助宏 */
#define MING_ACQUIRE_FROM_POOL(Pool, Type) static_cast<Type*>(Pool->Acquire())
#define MING_RELEASE_TO_POOL(Pool, Object) Pool->Release(Object)
