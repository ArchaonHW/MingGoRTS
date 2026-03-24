#include "MingOptimizedAICore.h"
#include "Engine/World.h"
#include "Async/ParallelFor.h"

// ============================================================================
// AI 代理物件池實現
// ============================================================================

void UMingAIAgentPool::Initialize(int32 PoolSize)
{
    // 初始化所有代理槽位
    for (int32 i = 0; i < AgentSlots.Capacity(); ++i)
    {
        AgentSlots[i].bInUse = false;
        ResetAgentProfile(AgentSlots[i].Profile);
    }
    
    // 填充可用代理列表
    AvailableAgents.Empty();
    for (int32 i = 0; i < FMath::Min(PoolSize, AgentSlots.Capacity()); ++i)
    {
        AvailableAgents.Add(i);
    }
    
    ActiveCount = 0;
    
    UE_LOG(LogTemp, Log, TEXT("[AIAgentPool] Initialized with %d agents"), PoolSize);
}

int32 UMingAIAgentPool::AcquireAgent()
{
    if (AvailableAgents.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("[AIAgentPool] No available agents"));
        return -1;
    }
    
    int32 SlotIndex = AvailableAgents[AvailableAgents.Num() - 1];
    AvailableAgents.RemoveAt(AvailableAgents.Num() - 1);
    
    AgentSlots[SlotIndex].bInUse = true;
    ResetAgentProfile(AgentSlots[SlotIndex].Profile);
    AgentSlots[SlotIndex].Profile.AgentId = SlotIndex;
    
    ++ActiveCount;
    
    return SlotIndex;
}

void UMingAIAgentPool::ReleaseAgent(int32 AgentId)
{
    if (AgentId < 0 || AgentId >= AgentSlots.Capacity())
    {
        return;
    }
    
    if (!AgentSlots[AgentId].bInUse)
    {
        return;
    }
    
    AgentSlots[AgentId].bInUse = false;
    ResetAgentProfile(AgentSlots[AgentId].Profile);
    
    AvailableAgents.Add(AgentId);
    --ActiveCount;
}

FMingOptimizedAIAgentProfile* UMingAIAgentPool::GetAgentProfile(int32 AgentId)
{
    if (AgentId < 0 || AgentId >= AgentSlots.Capacity())
    {
        return nullptr;
    }
    
    if (!AgentSlots[AgentId].bInUse)
    {
        return nullptr;
    }
    
    return &AgentSlots[AgentId].Profile;
}

int32 UMingAIAgentPool::GetActiveAgentCount() const
{
    return ActiveCount;
}

int32 UMingAIAgentPool::GetAvailableAgentCount() const
{
    return AvailableAgents.Num();
}

void UMingAIAgentPool::Clear()
{
    for (int32 i = 0; i < AgentSlots.Capacity(); ++i)
    {
        AgentSlots[i].bInUse = false;
        ResetAgentProfile(AgentSlots[i].Profile);
    }
    
    AvailableAgents.Empty();
    for (int32 i = 0; i < AgentSlots.Capacity(); ++i)
    {
        AvailableAgents.Add(i);
    }
    
    ActiveCount = 0;
}

void UMingAIAgentPool::ResetAgentProfile(FMingOptimizedAIAgentProfile& Profile)
{
    Profile.AgentId = -1;
    Profile.AgentName = FMingOptimizedString();
    Profile.PrimaryRole = 0;
    Profile.SetIntelligence(0.5f);
    Profile.SetCooperationWillingness(0.5f);
    Profile.SetCommunicationEfficiency(0.5f);
    Profile.SetSpecializationScore(0.5f);
    Profile.TrustedAgents.Empty();
}

// ============================================================================
// AI 感知系統實現
// ============================================================================

void UMingOptimizedPerceptionSystem::Initialize(FVector WorldOrigin, FVector WorldExtent)
{
    // i用 500 單元N網格j小
    new (&SpatialGrid) TUniformGridSpatialPartition<int32>(WorldOrigin, WorldExtent, 500.0f);
}

void UMingOptimizedPerceptionSystem::RegisterAgent(int32 AgentId, FVector Location)
{
    SpatialGrid.InsertObject(&AgentId, Location);
    AgentLocations.Add(AgentId, Location);
}

void UMingOptimizedPerceptionSystem::UpdateAgentLocation(int32 AgentId, FVector NewLocation)
{
    FVector* OldLocation = AgentLocations.Find(AgentId);
    if (OldLocation)
    {
        SpatialGrid.RemoveObject(&AgentId, *OldLocation);
        SpatialGrid.InsertObject(&AgentId, NewLocation);
        *OldLocation = NewLocation;
    }
}

void UMingOptimizedPerceptionSystem::UnregisterAgent(int32 AgentId)
{
    FVector* Location = AgentLocations.Find(AgentId);
    if (Location)
    {
        SpatialGrid.RemoveObject(&AgentId, *Location);
        AgentLocations.Remove(AgentId);
    }
}

TArray<int32> UMingOptimizedPerceptionSystem::GetNearbyAgents(int32 AgentId, float Radius) const
{
    TArray<int32> Results;
    Results.Reserve(32);
    
    const FVector* Location = AgentLocations.Find(AgentId);
    if (!Location)
    {
        return Results;
    }
    
    TArray<int32*> NearbyPointers;
    SpatialGrid.QueryNearbyObjects(*Location, Radius, NearbyPointers);
    
    for (int32* Ptr : NearbyPointers)
    {
        if (Ptr && *Ptr != AgentId)
        {
            Results.Add(*Ptr);
        }
    }
    
    return Results;
}

TArray<int32> UMingOptimizedPerceptionSystem::GetAgentsInViewCone(int32 AgentId, FVector Direction, float HalfAngle, float MaxDistance) const
{
    TArray<int32> Results;
    
    // 先獲取範圍內N所有代理
    TArray<int32> NearbyAgents = GetNearbyAgents(AgentId, MaxDistance);
    
    const FVector* AgentLocation = AgentLocations.Find(AgentId);
    if (!AgentLocation)
    {
        return Results;
    }
    
    Direction.Normalize();
    float CosHalfAngle = FMath::Cos(FMath::DegreesToRadians(HalfAngle));
    
    // 篩選在視野錐形內N代理
    for (int32 OtherAgentId : NearbyAgents)
    {
        const FVector* OtherLocation = AgentLocations.Find(OtherAgentId);
        if (!OtherLocation)
        {
            continue;
        }
        
        FVector ToOther = *OtherLocation - *AgentLocation;
        float Distance = ToOther.Size();
        
        if (Distance > MaxDistance)
        {
            continue;
        }
        
        ToOther.Normalize();
        float DotProduct = FVector::DotProduct(Direction, ToOther);
        
        if (DotProduct >= CosHalfAngle)
        {
            Results.Add(OtherAgentId);
        }
    }
    
    return Results;
}

void UMingOptimizedPerceptionSystem::Clear()
{
    SpatialGrid.Clear();
    AgentLocations.Empty();
}

// ============================================================================
// AI 決策快取實現
// ============================================================================

bool UMingAIDecisionCache::GetCachedDecision(int32 AgentId, uint32 SituationHash, FMingOptimizedAIDecision& OutDecision)
{
    FCacheKey Key;
    Key.AgentId = AgentId;
    Key.SituationHash = SituationHash;
    
    return DecisionCache.Get(Key, OutDecision);
}

void UMingAIDecisionCache::CacheDecision(int32 AgentId, uint32 SituationHash, const FMingOptimizedAIDecision& Decision)
{
    FCacheKey Key;
    Key.AgentId = AgentId;
    Key.SituationHash = SituationHash;
    
    DecisionCache.Put(Key, Decision);
}

void UMingAIDecisionCache::ClearCache()
{
    DecisionCache.Clear();
}

void UMingAIDecisionCache::ClearAgentCache(int32 AgentId)
{
    // LRU 快取不支持單個代理清除
    // 實際實現可以i用更複雜N數據結構
    ClearCache();
}

int32 UMingAIDecisionCache::GetCacheSize() const
{
    return DecisionCache.Num();
}

// ============================================================================
// AI 批次處理實現
// ============================================================================

void UMingAIBatchProcessor::InitializeBatch(int32 MaxAgentsPerBatch)
{
    CurrentBatchSize = 0;
    MaxBatchSize = FMath::Min(MaxAgentsPerBatch, BatchAgents.Capacity());
    BatchAgents.Empty();
}

void UMingAIBatchProcessor::AddAgentToBatch(int32 AgentId)
{
    if (CurrentBatchSize >= MaxBatchSize)
    {
        return;
    }
    
    BatchAgents.Add(AgentId);
    ++CurrentBatchSize;
}

void UMingAIBatchProcessor::ProcessBatch(float DeltaTime)
{
    if (CurrentBatchSize == 0)
    {
        return;
    }
    
    // i用平行處理來更新批次中N代理
    ParallelFor(CurrentBatchSize, [this, DeltaTime](int32 Index)
    {
        int32 AgentId = BatchAgents[Index];
        ProcessAgentPerception(AgentId, DeltaTime);
        ProcessAgentDecision(AgentId, DeltaTime);
        ProcessAgentAction(AgentId, DeltaTime);
    });
}

void UMingAIBatchProcessor::ClearBatch()
{
    BatchAgents.Empty();
    CurrentBatchSize = 0;
}

bool UMingAIBatchProcessor::IsBatchFull() const
{
    return CurrentBatchSize >= MaxBatchSize;
}

void UMingAIBatchProcessor::ProcessAgentDecision(int32 AgentId, float DeltaTime)
{
    // 實際決策處理邏輯
    double StartTime = FPlatformTime::Seconds();
    
    // 模擬決策過程...
    
    double DecisionTime = (FPlatformTime::Seconds() - StartTime) * 1000.0;
    UMingAIPerformanceMonitor::RecordDecisionTime(AgentId, (float)DecisionTime);
}

void UMingAIBatchProcessor::ProcessAgentPerception(int32 AgentId, float DeltaTime)
{
    double StartTime = FPlatformTime::Seconds();
    
    // 模擬感知處理...
    
    double PerceptionTime = (FPlatformTime::Seconds() - StartTime) * 1000.0;
    UMingAIPerformanceMonitor::RecordPerceptionTime(AgentId, (float)PerceptionTime);
}

void UMingAIBatchProcessor::ProcessAgentAction(int32 AgentId, float DeltaTime)
{
    // 動作執行邏輯
}

// ============================================================================
// AI 效能監控實現
// ============================================================================

float UMingAIPerformanceMonitor::TotalDecisionTime = 0.0f;
float UMingAIPerformanceMonitor::TotalPerceptionTime = 0.0f;
int32 UMingAIPerformanceMonitor::DecisionCount = 0;
int32 UMingAIPerformanceMonitor::PerceptionCount = 0;
int32 UMingAIPerformanceMonitor::CacheHits = 0;
int32 UMingAIPerformanceMonitor::CacheMisses = 0;

void UMingAIPerformanceMonitor::RecordDecisionTime(int32 AgentId, float TimeMs)
{
    TotalDecisionTime += TimeMs;
    ++DecisionCount;
}

void UMingAIPerformanceMonitor::RecordPerceptionTime(int32 AgentId, float TimeMs)
{
    TotalPerceptionTime += TimeMs;
    ++PerceptionCount;
}

void UMingAIPerformanceMonitor::RecordCacheHit()
{
    ++CacheHits;
}

void UMingAIPerformanceMonitor::RecordCacheMiss()
{
    ++CacheMisses;
}

FMingAIPerformanceMetrics UMingAIPerformanceMonitor::GetMetrics()
{
    FMingAIPerformanceMetrics Metrics;
    
    Metrics.AverageDecisionTime = DecisionCount > 0 ? TotalDecisionTime / DecisionCount : 0.0f;
    Metrics.AveragePerceptionTime = PerceptionCount > 0 ? TotalPerceptionTime / PerceptionCount : 0.0f;
    
    int32 TotalCacheAccesses = CacheHits + CacheMisses;
    Metrics.CacheHitRate = TotalCacheAccesses > 0 ? (CacheHits * 100 / TotalCacheAccesses) : 0;
    
    // 獲取記憶體i用
    FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
    Metrics.MemoryUsageMB = static_cast<int32>(MemoryStats.UsedPhysical / (1024 * 1024));
    
    return Metrics;
}

void UMingAIPerformanceMonitor::ResetMetrics()
{
    TotalDecisionTime = 0.0f;
    TotalPerceptionTime = 0.0f;
    DecisionCount = 0;
    PerceptionCount = 0;
    CacheHits = 0;
    CacheMisses = 0;
}

bool UMingAIPerformanceMonitor::ShouldOptimize()
{
    FMingAIPerformanceMetrics Metrics = GetMetrics();
    
    // 如果平均決策時間超過 5ms，需要優化
    if (Metrics.AverageDecisionTime > 5.0f)
    {
        return true;
    }
    
    // 如果快取命中率低於 50%，需要優化
    if (Metrics.CacheHitRate < 50)
    {
        return true;
    }
    
    // 如果記憶體i用超過 1GB，需要優化
    if (Metrics.MemoryUsageMB > 1024)
    {
        return true;
    }
    
    return false;
}

// ============================================================================
// 輔助函數
// ============================================================================

namespace MingAIOptimization
{
    /**
     * 計算情境雜湊值，用於快取
     */
    uint32 CalculateSituationHash(const FVector& Location, const TArray<int32>& NearbyAgents, const FString& Context)
    {
        uint32 Hash = 0;
        
        // 位置雜湊 (降低精度以提升快取命中率)
        FVector QuantizedLocation(
            FMath::RoundToInt(Location.X / 100.0f) * 100.0f,
            FMath::RoundToInt(Location.Y / 100.0f) * 100.0f,
            FMath::RoundToInt(Location.Z / 100.0f) * 100.0f
        );
        
        Hash = HashCombine(Hash, GetTypeHash(QuantizedLocation));
        
        // 鄰居數量雜湊
        Hash = HashCombine(Hash, GetTypeHash(NearbyAgents.Num()));
        
        // 情境雜湊 (只取前 32 字元)
        FString TruncatedContext = Context.Left(32);
        Hash = HashCombine(Hash, FCrc::StrCrc32(*TruncatedContext));
        
        return Hash;
    }
    
    /**
     * 優化 AI 更新頻率
     */
    EUpdateFrequency CalculateOptimalUpdateFrequency(
        const FVector& AgentLocation,
        const FVector& PlayerLocation,
        float BaseImportance
    )
    {
        float Distance = FVector::Dist(AgentLocation, PlayerLocation);
        float DistanceFactor = FMath::Clamp(Distance / 5000.0f, 0.0f, 1.0f);
        
        float Priority = BaseImportance * (1.0f - DistanceFactor);
        
        if (Priority > 0.8f)
        {
            return EUpdateFrequency::EveryFrame;
        }
        else if (Priority > 0.6f)
        {
            return EUpdateFrequency::Every2Frames;
        }
        else if (Priority > 0.4f)
        {
            return EUpdateFrequency::Every4Frames;
        }
        else if (Priority > 0.2f)
        {
            return EUpdateFrequency::Every8Frames;
        }
        else
        {
            return EUpdateFrequency::Every16Frames;
        }
    }
}
