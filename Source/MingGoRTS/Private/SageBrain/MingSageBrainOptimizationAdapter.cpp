#include "MingSageBrainOptimizationAdapter.h"
#include "MingSageBrainOptimizedTypes.h"
#include "Engine/Engine.h"

void UMingSageBrainOptimizationAdapter::Initialize()
{
    bUseOptimizedMode = true;
    TotalProcessingTime = 0.0f;
    TotalMemorySaved = 0.0f;
    ProcessCount = 0;

    // 初始化記憶體優化器
    MemoryOptimizer = UMingMemoryOptimizer::GetInstance();
    if (MemoryOptimizer)
    {
        MemoryOptimizer->Initialize(
            EMemoryOptimizationFlags::UseObjectPooling |
            EMemoryOptimizationFlags::UseStringInterning |
            EMemoryOptimizationFlags::UseLazyLoading |
            EMemoryOptimizationFlags::UseMemoryCompaction
        );
    }

    // 初始化物件池
    InitializeObjectPools();

    // 創建核心系統實例
    CoreSystem = NewObject<UMingSageBrainCoreSystem>(this);
    if (CoreSystem)
    {
        CoreSystem->InitializeSageBrain();
    }

    UE_LOG(LogTemp, Log, TEXT("[SageBrainOptimizationAdapter] Initialized with optimized mode"));
}

void UMingSageBrainOptimizationAdapter::EnableOptimizedMode(bool bEnable)
{
    bUseOptimizedMode = bEnable;
    UE_LOG(LogTemp, Log, TEXT("[SageBrainOptimizationAdapter] Optimized mode: %s"), 
        bEnable ? TEXT("Enabled") : TEXT("Disabled"));
}

void UMingSageBrainOptimizationAdapter::InitializeObjectPools()
{
    if (MemoryOptimizer)
    {
        // 創建思考結果物件池
        ThoughtResultPool = MemoryOptimizer->GetOrCreateObjectPool(
            UMingSageBrainCoreSystem::StaticClass()
        );
    }
}

void UMingSageBrainOptimizationAdapter::ReleaseObjectPools()
{
    if (MemoryOptimizer)
    {
        MemoryOptimizer->DestroyObjectPool(UMingSageBrainCoreSystem::StaticClass());
    }
    ThoughtResultPool = nullptr;
}

FSageBrainThoughtResult UMingSageBrainOptimizationAdapter::ProcessThoughtOptimized(
    const FString& Input, 
    ESageBrainThinkingLayer TargetLayer)
{
    double StartTime = FPlatformTime::Seconds();

    FSageBrainThoughtResult Result;

    if (bUseOptimizedMode)
    {
        // 使用優化路徑
        FSageBrainThoughtResult_Optimized OptimizedResult = 
            ProcessThoughtInternal(Input, TargetLayer);
        
        // 轉換回標準格式
        Result = FMingSageBrainTypeConverter::ConvertFromOptimized(OptimizedResult);
        
        // 更新統計
        double ProcessingTime = FPlatformTime::Seconds() - StartTime;
        float MemorySaved = sizeof(FSageBrainThoughtResult) - sizeof(FSageBrainThoughtResult_Optimized);
        UpdateOptimizationStats((float)ProcessingTime, MemorySaved);
    }
    else
    {
        // 使用標準路徑
        if (CoreSystem)
        {
            Result = CoreSystem->ProcessThought(Input, TargetLayer);
        }
    }

    return Result;
}

TArray<FSageBrainThoughtResult> UMingSageBrainOptimizationAdapter::ProcessThoughtBatchOptimized(
    const TArray<FString>& Inputs,
    ESageBrainThinkingLayer TargetLayer)
{
    TArray<FSageBrainThoughtResult> Results;
    Results.Reserve(Inputs.Num());

    double StartTime = FPlatformTime::Seconds();
    
    // 記憶體範圍分析
    MING_MEMORY_SCOPE(BatchProcessing);

    if (bUseOptimizedMode)
    {
        // 批次處理，使用優化結構
        for (const FString& Input : Inputs)
        {
            FSageBrainThoughtResult_Optimized OptimizedResult = 
                ProcessThoughtInternal(Input, TargetLayer);
            
            Results.Add(FMingSageBrainTypeConverter::ConvertFromOptimized(OptimizedResult));

            // 緩存結果（如果空間允許）
            if (ThoughtCache.Num() < 64)
            {
                ThoughtCache.Add(OptimizedResult);
            }
        }

        double ProcessingTime = FPlatformTime::Seconds() - StartTime;
        float MemorySaved = Inputs.Num() * (sizeof(FSageBrainThoughtResult) - sizeof(FSageBrainThoughtResult_Optimized));
        UpdateOptimizationStats((float)ProcessingTime, MemorySaved);
    }
    else
    {
        // 標準批次處理
        if (CoreSystem)
        {
            for (const FString& Input : Inputs)
            {
                Results.Add(CoreSystem->ProcessThought(Input, TargetLayer));
            }
        }
    }

    return Results;
}

FSageBrainThoughtResult_Optimized UMingSageBrainOptimizationAdapter::ProcessThoughtInternal(
    const FString& Input, 
    ESageBrainThinkingLayer TargetLayer)
{
    FSageBrainThoughtResult_Optimized Result;
    Result.Layer = TargetLayer;
    Result.Timestamp = FDateTime::Now();

    // 模擬思考處理
    // 實際實現中會調用 AI 處理邏輯
    Result.Content = FMingOptimizedString(FString::Printf(
        TEXT("思考結果 [%s]: %s"),
        *UEnum::GetValueAsString(TargetLayer),
        *Input.Left(50)
    ));

    // 計算信心度（模擬）
    Result.Confidence = FMath::RandRange(0.7f, 0.95f);

    // 添加元數據（限制最多32項）
    Result.Metadata.Add(FMingOptimizedString(TEXT("Layer")), (float)TargetLayer);
    Result.Metadata.Add(FMingOptimizedString(TEXT("Confidence")), Result.Confidence);
    Result.Metadata.Add(FMingOptimizedString(TEXT("Time")), FMath::RandRange(0.1f, 2.0f));

    return Result;
}

FSageBrainLearningResult UMingSageBrainOptimizationAdapter::TrainAlgorithmOptimized(
    ESageBrainLearningAlgorithm Algorithm, 
    const FString& TrainingData)
{
    double StartTime = FPlatformTime::Seconds();

    FSageBrainLearningResult Result;

    if (bUseOptimizedMode)
    {
        FSageBrainLearningResult_Optimized OptimizedResult = 
            TrainAlgorithmInternal(Algorithm, TrainingData);
        
        Result.Algorithm = OptimizedResult.Algorithm;
        Result.LearningData = OptimizedResult.LearningData.ToString();
        Result.Performance = OptimizedResult.Performance;
        Result.Accuracy = OptimizedResult.Accuracy;
        Result.Timestamp = OptimizedResult.Timestamp;

        // 緩存結果
        if (LearningCache.Num() < 32)
        {
            LearningCache.Add(OptimizedResult);
        }

        double ProcessingTime = FPlatformTime::Seconds() - StartTime;
        float MemorySaved = sizeof(FSageBrainLearningResult) - sizeof(FSageBrainLearningResult_Optimized);
        UpdateOptimizationStats((float)ProcessingTime, MemorySaved);
    }
    else
    {
        if (CoreSystem)
        {
            Result = CoreSystem->TrainAlgorithm(Algorithm, TrainingData);
        }
    }

    return Result;
}

FSageBrainLearningResult_Optimized UMingSageBrainOptimizationAdapter::TrainAlgorithmInternal(
    ESageBrainLearningAlgorithm Algorithm, 
    const FString& TrainingData)
{
    FSageBrainLearningResult_Optimized Result;
    Result.Algorithm = Algorithm;
    Result.LearningData = FMingOptimizedString(TrainingData.Left(100));
    Result.Timestamp = FDateTime::Now();

    // 模擬訓練結果
    Result.Performance = FMath::RandRange(0.8f, 0.98f);
    Result.Accuracy = FMath::RandRange(0.75f, 0.95f);
    Result.PackedData = ((uint32)(Result.Performance * 255) << 16) | 
                        ((uint32)(Result.Accuracy * 255) << 8);

    return Result;
}

TArray<FSageBrainLearningResult> UMingSageBrainOptimizationAdapter::GetLearningStatisticsOptimized()
{
    TArray<FSageBrainLearningResult> Results;

    // 從緩存返回結果
    for (const auto& CachedResult : LearningCache)
    {
        FSageBrainLearningResult Result;
        Result.Algorithm = CachedResult.Algorithm;
        Result.LearningData = CachedResult.LearningData.ToString();
        Result.Performance = CachedResult.Performance;
        Result.Accuracy = CachedResult.Accuracy;
        Result.Timestamp = CachedResult.Timestamp;
        Results.Add(Result);
    }

    // 如果緩存不足，從核心系統獲取
    if (Results.Num() == 0 && CoreSystem)
    {
        return CoreSystem->GetLearningStatistics();
    }

    return Results;
}

FSageBrainPhilosophyAnalysis UMingSageBrainOptimizationAdapter::AnalyzeWithPhilosophyOptimized(
    const FString& Input, 
    ESageBrainPhilosophy Philosophy)
{
    double StartTime = FPlatformTime::Seconds();

    FSageBrainPhilosophyAnalysis Result;

    if (bUseOptimizedMode)
    {
        FSageBrainPhilosophyAnalysis_Optimized OptimizedResult = 
            AnalyzePhilosophyInternal(Input, Philosophy);

        Result.Philosophy = OptimizedResult.Philosophy;
        Result.Analysis = OptimizedResult.Analysis.ToString();
        Result.Relevance = OptimizedResult.Relevance;
        Result.Timestamp = OptimizedResult.Timestamp;

        // 轉換關鍵原則
        for (const auto& Principle : OptimizedResult.KeyPrinciples)
        {
            Result.KeyPrinciples.Add(Principle.ToString());
        }

        // 緩存結果
        if (PhilosophyCache.Num() < 16)
        {
            PhilosophyCache.Add(OptimizedResult);
        }

        double ProcessingTime = FPlatformTime::Seconds() - StartTime;
        float MemorySaved = sizeof(FSageBrainPhilosophyAnalysis) - sizeof(FSageBrainPhilosophyAnalysis_Optimized);
        UpdateOptimizationStats((float)ProcessingTime, MemorySaved);
    }
    else
    {
        if (CoreSystem)
        {
            Result = CoreSystem->AnalyzeWithPhilosophy(Input, Philosophy);
        }
    }

    return Result;
}

FSageBrainPhilosophyAnalysis_Optimized UMingSageBrainOptimizationAdapter::AnalyzePhilosophyInternal(
    const FString& Input, 
    ESageBrainPhilosophy Philosophy)
{
    FSageBrainPhilosophyAnalysis_Optimized Result;
    Result.Philosophy = Philosophy;
    Result.Timestamp = FDateTime::Now();

    // 模擬哲學分析
    static const TCHAR* PhilosophyNames[] = {
        TEXT("儒家"), TEXT("道家"), TEXT("佛家"), TEXT("法家"),
        TEXT("墨家"), TEXT("兵家"), TEXT("陰陽家"), TEXT("雜家")
    };

    int32 PhilIndex = (int32)Philosophy;
    const TCHAR* PhilName = (PhilIndex >= 0 && PhilIndex < 8) ? 
        PhilosophyNames[PhilIndex] : TEXT("未知");

    Result.Analysis = FMingOptimizedString(FString::Printf(
        TEXT("從%s角度分析: %s"),
        PhilName,
        *Input.Left(50)
    ));

    Result.Relevance = FMath::RandRange(0.6f, 0.9f);

    // 添加關鍵原則（限制16個）
    Result.KeyPrinciples.Add(FMingOptimizedString(FString::Printf(TEXT("%s核心原則1"), PhilName)));
    Result.KeyPrinciples.Add(FMingOptimizedString(FString::Printf(TEXT("%s核心原則2"), PhilName)));
    Result.KeyPrinciples.Add(FMingOptimizedString(FString::Printf(TEXT("%s核心原則3"), PhilName)));

    return Result;
}

TArray<FSageBrainPhilosophyAnalysis> UMingSageBrainOptimizationAdapter::GetPhilosophicalPerspectivesOptimized(
    const FString& Input)
{
    TArray<FSageBrainPhilosophyAnalysis> Results;
    Results.Reserve(8);

    // 從所有8個哲學體系獲取觀點
    for (int32 i = 0; i < 8; ++i)
    {
        ESageBrainPhilosophy Philosophy = (ESageBrainPhilosophy)i;
        Results.Add(AnalyzeWithPhilosophyOptimized(Input, Philosophy));
    }

    return Results;
}

void UMingSageBrainOptimizationAdapter::CompactMemory()
{
    if (MemoryOptimizer)
    {
        MemoryOptimizer->CompactMemory();
    }

    // 清理緩存
    ThoughtCache.Empty();
    LearningCache.Empty();
    PhilosophyCache.Empty();

    UE_LOG(LogTemp, Log, TEXT("[SageBrainOptimizationAdapter] Memory compacted"));
}

FString UMingSageBrainOptimizationAdapter::GetMemoryReport() const
{
    FString Report = TEXT("SageBrain 記憶體報告:\n");
    
    // 緩存統計
    Report += FString::Printf(TEXT("思考結果緩存: %d/%d\n"), ThoughtCache.Num(), 64);
    Report += FString::Printf(TEXT("學習結果緩存: %d/%d\n"), LearningCache.Num(), 32);
    Report += FString::Printf(TEXT("哲學分析緩存: %d/%d\n"), PhilosophyCache.Num(), 16);

    // 優化統計
    if (ProcessCount > 0)
    {
        Report += FString::Printf(TEXT("\n優化統計:\n"));
        Report += FString::Printf(TEXT("處理次數: %d\n"), ProcessCount);
        Report += FString::Printf(TEXT("平均處理時間: %.3f ms\n"), 
            (TotalProcessingTime / ProcessCount) * 1000.0f);
        Report += FString::Printf(TEXT("總記憶體節省: %.2f KB\n"), TotalMemorySaved / 1024.0f);
        Report += FString::Printf(TEXT("每次處理平均節省: %.2f KB\n"), 
            (TotalMemorySaved / ProcessCount) / 1024.0f);
    }

    // 系統記憶體報告
    if (MemoryOptimizer)
    {
        Report += TEXT("\n系統記憶體:\n");
        Report += UMingMemoryProfiler::GetMemoryReport();
    }

    return Report;
}

bool UMingSageBrainOptimizationAdapter::IsMemoryCritical() const
{
    return UMingMemoryProfiler::IsMemoryCritical();
}

void UMingSageBrainOptimizationAdapter::StartProfiling()
{
    UMingMemoryProfiler::StartProfiling();
    UE_LOG(LogTemp, Log, TEXT("[SageBrainOptimizationAdapter] Profiling started"));
}

void UMingSageBrainOptimizationAdapter::StopProfiling()
{
    UMingMemoryProfiler::StopProfiling();
    UE_LOG(LogTemp, Log, TEXT("[SageBrainOptimizationAdapter] Profiling stopped"));
}

TMap<FString, float> UMingSageBrainOptimizationAdapter::GetOptimizationStats() const
{
    TMap<FString, float> Stats;

    Stats.Add(TEXT("ProcessCount"), (float)ProcessCount);
    Stats.Add(TEXT("AvgProcessingTimeMS"), 
        ProcessCount > 0 ? (TotalProcessingTime / ProcessCount) * 1000.0f : 0.0f);
    Stats.Add(TEXT("TotalMemorySavedKB"), TotalMemorySaved / 1024.0f);
    Stats.Add(TEXT("AvgMemorySavedPerProcessKB"), 
        ProcessCount > 0 ? (TotalMemorySaved / ProcessCount) / 1024.0f : 0.0f);
    Stats.Add(TEXT("ThoughtCacheUtilization"), ThoughtCache.Num() / 64.0f * 100.0f);
    Stats.Add(TEXT("LearningCacheUtilization"), LearningCache.Num() / 32.0f * 100.0f);
    Stats.Add(TEXT("PhilosophyCacheUtilization"), PhilosophyCache.Num() / 16.0f * 100.0f);

    return Stats;
}

void UMingSageBrainOptimizationAdapter::UpdateOptimizationStats(float ProcessingTime, float MemorySaved)
{
    TotalProcessingTime += ProcessingTime;
    TotalMemorySaved += MemorySaved;
    ProcessCount++;
}
