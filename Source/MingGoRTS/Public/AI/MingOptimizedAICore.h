#pragma once

#include "CoreMinimal.h"
#include "MingMemoryOptimization.h"
#include "MingPerformanceOptimization.h"
#include "MingOptimizedAICore.generated.h"

// ============================================================================
// AI 記憶體優化配置
// ============================================================================

namespace MingAIOptimizationConfig
{
    // AI 代理池大小
    constexpr int32 MAX_AI_AGENTS = 256;
    constexpr int32 MAX_TASKS = 128;
    constexpr int32 MAX_MESSAGES = 64;
    
    // 更新頻率
    constexpr float HIGH_PRIORITY_UPDATE_INTERVAL = 0.1f;  // 10Hz
    constexpr float NORMAL_UPDATE_INTERVAL = 0.2f;         // 5Hz
    constexpr float LOW_PRIORITY_UPDATE_INTERVAL = 0.5f;   // 2Hz
    
    // 感知系統
    constexpr int32 MAX_PERCEIVED_ENTITIES = 32;
    constexpr float PERCEPTION_RADIUS = 2000.0f;
}

// ============================================================================
// 優化的 AI 代理結構
// ============================================================================

USTRUCT(BlueprintType)
struct FMingOptimizedAIAgentProfile
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    int32 AgentId;
    
    // 優化字串
    UPROPERTY(BlueprintReadOnly)
    FMingOptimizedString AgentName;
    
    UPROPERTY(BlueprintReadOnly)
    uint8 PrimaryRole;  // EAIAgentRole packed
    
    // 打包屬性 (0-255 映射到 0.0-1.0)
    UPROPERTY(BlueprintReadOnly)
    uint8 Intelligence_Packed;
    
    UPROPERTY(BlueprintReadOnly)
    uint8 CooperationWillingness_Packed;
    
    UPROPERTY(BlueprintReadOnly)
    uint8 CommunicationEfficiency_Packed;
    
    UPROPERTY(BlueprintReadOnly)
    uint8 SpecializationScore_Packed;
    
    // 信任關係 (限制64個)
    UPROPERTY(BlueprintReadOnly)
    TMingFixedMap<int32, uint8, 64> TrustedAgents;  // AgentId -> TrustLevel
    
    FMingOptimizedAIAgentProfile()
        : AgentId(0)
        , PrimaryRole(0)
        , Intelligence_Packed(128)
        , CooperationWillingness_Packed(128)
        , CommunicationEfficiency_Packed(128)
        , SpecializationScore_Packed(128)
    {}
    
    float GetIntelligence() const { return Intelligence_Packed / 255.0f; }
    void SetIntelligence(float Value) { Intelligence_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
    
    float GetCooperationWillingness() const { return CooperationWillingness_Packed / 255.0f; }
    void SetCooperationWillingness(float Value) { CooperationWillingness_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
    
    float GetCommunicationEfficiency() const { return CommunicationEfficiency_Packed / 255.0f; }
    void SetCommunicationEfficiency(float Value) { CommunicationEfficiency_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
    
    float GetSpecializationScore() const { return SpecializationScore_Packed / 255.0f; }
    void SetSpecializationScore(float Value) { SpecializationScore_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
};

/**
 * 優化的 AI 決策結構
 */
USTRUCT(BlueprintType)
struct FMingOptimizedAIDecision
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    int32 AgentId;
    
    UPROPERTY(BlueprintReadOnly)
    FMingOptimizedString DecisionType;
    
    // 打包信心度
    UPROPERTY(BlueprintReadOnly)
    uint8 Confidence_Packed;
    
    // 目標位置 (壓縮)
    UPROPERTY(BlueprintReadOnly)
    FMingCompressedVector CompressedTargetLocation;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Priority;
    
    // 打包預期結果
    UPROPERTY(BlueprintReadOnly)
    uint8 EstimatedOutcome_Packed;
    
    // 目標代理 (限制8個)
    UPROPERTY(BlueprintReadOnly)
    TMingFixedArray<int32, 8> TargetAgentIds;
    
    UPROPERTY(BlueprintReadOnly)
    float Timestamp;
    
    FMingOptimizedAIDecision()
        : AgentId(0)
        , Confidence_Packed(128)
        , Priority(0)
        , EstimatedOutcome_Packed(128)
        , Timestamp(0.0f)
    {}
    
    float GetConfidence() const { return Confidence_Packed / 255.0f; }
    void SetConfidence(float Value) { Confidence_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
    
    float GetEstimatedOutcome() const { return EstimatedOutcome_Packed / 255.0f; }
    void SetEstimatedOutcome(float Value) { EstimatedOutcome_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
};

/**
 * 優化的協作任務結構
 */
USTRUCT(BlueprintType)
struct FMingOptimizedCollaborativeTask
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    int32 TaskId;
    
    UPROPERTY(BlueprintReadOnly)
    FMingOptimizedString TaskName;
    
    // 打包複雜度
    UPROPERTY(BlueprintReadOnly)
    uint8 Complexity_Packed;
    
    // 打包進度
    UPROPERTY(BlueprintReadOnly)
    uint8 Progress_Packed;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsCompleted;
    
    UPROPERTY(BlueprintReadOnly)
    float CompletionTime;
    
    // 分配的代理 (限制16個)
    UPROPERTY(BlueprintReadOnly)
    TMingFixedArray<int32, 16> AssignedAgents;
    
    FMingOptimizedCollaborativeTask()
        : TaskId(0)
        , Complexity_Packed(128)
        , Progress_Packed(0)
        , bIsCompleted(false)
        , CompletionTime(0.0f)
    {}
    
    float GetComplexity() const { return Complexity_Packed / 255.0f * 10.0f; }
    void SetComplexity(float Value) { Complexity_Packed = FMath::Clamp(FMath::RoundToInt(Value / 10.0f * 255), 0, 255); }
    
    float GetProgress() const { return Progress_Packed / 255.0f; }
    void SetProgress(float Value) { Progress_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
};

/**
 * 優化的消息結構
 */
USTRUCT(BlueprintType)
struct FMingOptimizedAIMessage
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    int32 SenderId;
    
    UPROPERTY(BlueprintReadOnly)
    int32 ReceiverId;
    
    UPROPERTY(BlueprintReadOnly)
    FMingOptimizedString MessageType;
    
    UPROPERTY(BlueprintReadOnly)
    FMingOptimizedString Content;
    
    UPROPERTY(BlueprintReadOnly)
    float Timestamp;
    
    // 打包重要性
    UPROPERTY(BlueprintReadOnly)
    uint8 Priority_Packed;
    
    FMingOptimizedAIMessage()
        : SenderId(0)
        , ReceiverId(0)
        , Timestamp(0.0f)
        , Priority_Packed(128)
    {}
    
    float GetPriority() const { return Priority_Packed / 255.0f; }
    void SetPriority(float Value) { Priority_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
};

// ============================================================================
// AI 物件池系統
// ============================================================================

/**
 * AI 代理物件池
 */
UCLASS()
class MINGGORTS_API UMingAIAgentPool : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "AI|Optimization")
    void Initialize(int32 PoolSize);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Optimization")
    int32 AcquireAgent();
    
    UFUNCTION(BlueprintCallable, Category = "AI|Optimization")
    void ReleaseAgent(int32 AgentId);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Optimization")
    FMingOptimizedAIAgentProfile* GetAgentProfile(int32 AgentId);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Optimization")
    int32 GetActiveAgentCount() const;
    
    UFUNCTION(BlueprintCallable, Category = "AI|Optimization")
    int32 GetAvailableAgentCount() const;
    
    UFUNCTION(BlueprintCallable, Category = "AI|Optimization")
    void Clear();

private:
    struct FAgentSlot
    {
        FMingOptimizedAIAgentProfile Profile;
        bool bInUse;
    };
    
    TMingFixedArray<FAgentSlot, 256> AgentSlots;
    TMingFixedArray<int32, 256> AvailableAgents;
    int32 ActiveCount;
    
    void ResetAgentProfile(FMingOptimizedAIAgentProfile& Profile);
};

// ============================================================================
// AI 感知系統優化
// ============================================================================

/**
 * 空間哈希感知系統
 * 高效的鄰居查詢和感知
 */
UCLASS()
class MINGGORTS_API UMingOptimizedPerceptionSystem : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "AI|Perception")
    void Initialize(FVector WorldOrigin, FVector WorldExtent);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Perception")
    void RegisterAgent(int32 AgentId, FVector Location);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Perception")
    void UpdateAgentLocation(int32 AgentId, FVector NewLocation);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Perception")
    void UnregisterAgent(int32 AgentId);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Perception")
    TArray<int32> GetNearbyAgents(int32 AgentId, float Radius) const;
    
    UFUNCTION(BlueprintCallable, Category = "AI|Perception")
    TArray<int32> GetAgentsInViewCone(int32 AgentId, FVector Direction, float HalfAngle, float MaxDistance) const;
    
    UFUNCTION(BlueprintCallable, Category = "AI|Perception")
    void Clear();

private:
    TUniformGridSpatialPartition<int32> SpatialGrid;
    TMap<int32, FVector> AgentLocations;
};

// ============================================================================
// AI 決策快取
// ============================================================================

/**
 * 決策結果快取
 * 避免重複計算相同情境的決策
 */
UCLASS()
class MINGGORTS_API UMingAIDecisionCache : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "AI|Cache")
    bool GetCachedDecision(int32 AgentId, uint32 SituationHash, FMingOptimizedAIDecision& OutDecision);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Cache")
    void CacheDecision(int32 AgentId, uint32 SituationHash, const FMingOptimizedAIDecision& Decision);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Cache")
    void ClearCache();
    
    UFUNCTION(BlueprintCallable, Category = "AI|Cache")
    void ClearAgentCache(int32 AgentId);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Cache")
    int32 GetCacheSize() const;

private:
    struct FCacheKey
    {
        int32 AgentId;
        uint32 SituationHash;
        
        bool operator==(const FCacheKey& Other) const
        {
            return AgentId == Other.AgentId && SituationHash == Other.SituationHash;
        }
    };
    
    friend uint32 GetTypeHash(const FCacheKey& Key)
    {
        return HashCombine(GetTypeHash(Key.AgentId), Key.SituationHash);
    }
    
    T LRUCache<FCacheKey, FMingOptimizedAIDecision, 128> DecisionCache;
};

// ============================================================================
// AI 批次處理系統
// ============================================================================

/**
 * AI 批次更新管理器
 */
UCLASS()
class MINGGORTS_API UMingAIBatchProcessor : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "AI|Batch")
    void InitializeBatch(int32 MaxAgentsPerBatch);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Batch")
    void AddAgentToBatch(int32 AgentId);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Batch")
    void ProcessBatch(float DeltaTime);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Batch")
    void ClearBatch();
    
    UFUNCTION(BlueprintCallable, Category = "AI|Batch")
    bool IsBatchFull() const;

private:
    TMingFixedArray<int32, 32> BatchAgents;
    int32 CurrentBatchSize;
    int32 MaxBatchSize;
    
    void ProcessAgentDecision(int32 AgentId, float DeltaTime);
    void ProcessAgentPerception(int32 AgentId, float DeltaTime);
    void ProcessAgentAction(int32 AgentId, float DeltaTime);
};

// ============================================================================
// AI 執行緒優化
// ============================================================================

/**
 * AI 多執行緒任務處理器
 */
UCLASS()
class MINGGORTS_API UMingAIMultithreadedProcessor : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "AI|Multithreading")
    void Initialize(int32 ThreadCount);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Multithreading")
    void QueueAgentUpdate(int32 AgentId);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Multithreading")
    void ProcessQueue();
    
    UFUNCTION(BlueprintCallable, Category = "AI|Multithreading")
    void WaitForCompletion();
    
    UFUNCTION(BlueprintCallable, Category = "AI|Multithreading")
    bool IsProcessing() const;

private:
    TLockFreeQueue<int32, 256> AgentUpdateQueue;
    TArray<FAsyncTask<>> ActiveTasks;
    int32 NumWorkerThreads;
    bool bIsProcessing;
};

// ============================================================================
// AI 效能監控
// ============================================================================

USTRUCT(BlueprintType)
struct FMingAIPerformanceMetrics
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    int32 ActiveAgents;
    
    UPROPERTY(BlueprintReadOnly)
    int32 ActiveTasks;
    
    UPROPERTY(BlueprintReadOnly)
    int32 PendingMessages;
    
    UPROPERTY(BlueprintReadOnly)
    float AverageDecisionTime;
    
    UPROPERTY(BlueprintReadOnly)
    float AveragePerceptionTime;
    
    UPROPERTY(BlueprintReadOnly)
    float AverageActionTime;
    
    UPROPERTY(BlueprintReadOnly)
    int32 CacheHitRate;
    
    UPROPERTY(BlueprintReadOnly)
    int32 MemoryUsageMB;
};

UCLASS()
class MINGGORTS_API UMingAIPerformanceMonitor : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "AI|Performance")
    static void RecordDecisionTime(int32 AgentId, float TimeMs);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Performance")
    static void RecordPerceptionTime(int32 AgentId, float TimeMs);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Performance")
    static void RecordCacheHit();
    
    UFUNCTION(BlueprintCallable, Category = "AI|Performance")
    static void RecordCacheMiss();
    
    UFUNCTION(BlueprintCallable, Category = "AI|Performance")
    static FMingAIPerformanceMetrics GetMetrics();
    
    UFUNCTION(BlueprintCallable, Category = "AI|Performance")
    static void ResetMetrics();
    
    UFUNCTION(BlueprintCallable, Category = "AI|Performance")
    static bool ShouldOptimize();

private:
    static float TotalDecisionTime;
    static float TotalPerceptionTime;
    static int32 DecisionCount;
    static int32 PerceptionCount;
    static int32 CacheHits;
    static int32 CacheMisses;
};
