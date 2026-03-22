#include "MingRTSAlgorithmOptimizer.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

UMingRTSAlgorithmOptimizer::UMingRTSAlgorithmOptimizer()
{
    TimeWeight = 0.5f;
    SpaceWeight = 0.3f;
    AccuracyWeight = 0.2f;
    bEnableAdvancedOptimizations = true;
}

void UMingRTSAlgorithmOptimizer::InitializeAlgorithmOptimizer()
{
    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Initializing algorithm optimizer..."));

    // 初始化算法庫
    InitializeAlgorithmLibrary();

    // 加載標準算法
    LoadStandardAlgorithms();

    // 設置優化參數
    SetupOptimizationParameters();

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Algorithm optimizer initialized"));
}

FAlgorithmProfile UMingRTSAlgorithmOptimizer::AnalyzeAlgorithm(const FString& AlgorithmCode, EAlgorithmType AlgorithmType)
{
    FAlgorithmProfile Profile;
    Profile.AlgorithmName = FString::Printf(TEXT("Algorithm_%d"), FMath::RandRange(1000, 9999));
    Profile.AlgorithmType = AlgorithmType;

    // 計算複雜度
    Profile.TimeComplexity = AnalyzeTimeComplexity(AlgorithmCode);
    Profile.SpaceComplexity = AnalyzeSpaceComplexity(AlgorithmCode);

    // 估算執行時間和內存使用
    Profile.ExecutionTime = EstimateExecutionTime(AlgorithmCode, Profile.TimeComplexity);
    Profile.MemoryUsage = EstimateMemoryUsage(AlgorithmCode, Profile.SpaceComplexity);

    // 分析輸入大小
    Profile.InputSize = EstimateInputSize(AlgorithmCode);

    // 記錄配置文件
    AlgorithmProfiles.Add(Profile.AlgorithmName, Profile);

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Analyzed algorithm %s - Time: %.2f, Space: %.2f"), 
           *Profile.AlgorithmName, Profile.TimeComplexity, Profile.SpaceComplexity);

    return Profile;
}

FOptimizationResult UMingRTSAlgorithmOptimizer::OptimizeAlgorithm(const FString& AlgorithmCode, EOptimizationTechnique Technique)
{
    FOptimizationResult Result;
    Result.OriginalAlgorithm = AlgorithmCode;
    Result.Technique = Technique;

    // 檢查算法是否可優化
    if (!IsAlgorithmOptimizable(AlgorithmCode, Technique))
    {
        Result.bSuccess = false;
        Result.OptimizationSteps.Add(TEXT("Algorithm is not optimizable with this technique"));
        return Result;
    }

    // 獲取原始性能
    FAlgorithmProfile OriginalProfile = AnalyzeAlgorithm(AlgorithmCode, GetAlgorithmType(AlgorithmCode));

    // 應用優化
    switch (Technique)
    {
        case EOptimizationTechnique::TimeComplexity:
            Result = OptimizeTimeComplexity(AlgorithmCode);
            break;
        case EOptimizationTechnique::SpaceComplexity:
            Result = OptimizeSpaceComplexity(AlgorithmCode);
            break;
        case EOptimizationTechnique::CacheOptimization:
            Result = OptimizeCachePerformance(AlgorithmCode);
            break;
        case EOptimizationTechnique::Parallelization:
            Result = OptimizeForParallelization(AlgorithmCode);
            break;
        case EOptimizationTechnique::Memoization:
            Result = ApplyMemoizationOptimization(AlgorithmCode);
            break;
        case EOptimizationTechnique::Approximation:
            Result = ApplyApproximationOptimization(AlgorithmCode);
            break;
        case EOptimizationTechnique::Heuristic:
            Result = ApplyHeuristicOptimization(AlgorithmCode);
            break;
        case EOptimizationTechnique::Hybrid:
            Result = ApplyHybridOptimization(AlgorithmCode);
            break;
    }

    if (Result.bSuccess)
    {
        // 分析優化後的性能
        FAlgorithmProfile OptimizedProfile = AnalyzeAlgorithm(Result.OptimizedAlgorithm, GetAlgorithmType(AlgorithmCode));

        // 計算性能提升
        Result.TimeImprovement = (OriginalProfile.ExecutionTime - OptimizedProfile.ExecutionTime) / OriginalProfile.ExecutionTime;
        Result.SpaceImprovement = (OriginalProfile.MemoryUsage - OptimizedProfile.MemoryUsage) / OriginalProfile.MemoryUsage;
        Result.OverallImprovement = (Result.TimeImprovement * TimeWeight + Result.SpaceImprovement * SpaceWeight);

        // 更新統計
        UpdateOptimizationStatistics(OriginalProfile.AlgorithmName, OptimizedProfile);

        // 觸發事件
        OnAlgorithmOptimized.Broadcast(OriginalProfile.AlgorithmName, Result);
        OnOptimizationCompleted.Broadcast(OriginalProfile.AlgorithmName, Technique, Result.OverallImprovement);

        UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Optimized algorithm with %s technique - Improvement: %.2f%%"), 
               *GetOptimizationTechniqueName(Technique), Result.OverallImprovement * 100.0f);
    }

    return Result;
}

TArray<FOptimizationResult> UMingRTSAlgorithmOptimizer::BatchOptimizeAlgorithms(const TArray<FString>& AlgorithmCodes, const TArray<EOptimizationTechnique>& Techniques)
{
    TArray<FOptimizationResult> Results;

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Starting batch optimization of %d algorithms with %d techniques"), 
           AlgorithmCodes.Num(), Techniques.Num());

    for (const FString& AlgorithmCode : AlgorithmCodes)
    {
        for (EOptimizationTechnique Technique : Techniques)
        {
            FOptimizationResult Result = OptimizeAlgorithm(AlgorithmCode, Technique);
            Results.Add(Result);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Batch optimization completed with %d results"), Results.Num());
    return Results;
}

TMap<FString, float> UMingRTSAlgorithmOptimizer::CompareAlgorithms(const TArray<FString>& AlgorithmCodes)
{
    TMap<FString, float> Comparison;

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Comparing %d algorithms"), AlgorithmCodes.Num());

    for (const FString& AlgorithmCode : AlgorithmCodes)
    {
        FAlgorithmProfile Profile = AnalyzeAlgorithm(AlgorithmCode, GetAlgorithmType(AlgorithmCode));
        
        // 計算綜合性能分數
        float PerformanceScore = CalculatePerformanceScore(Profile);
        Comparison.Add(Profile.AlgorithmName, PerformanceScore);
    }

    return Comparison;
}

TArray<FString> UMingRTSAlgorithmOptimizer::GenerateOptimizationSuggestions(const FString& AlgorithmCode)
{
    TArray<FString> Suggestions;

    // 分析算法特徵
    FAlgorithmProfile Profile = AnalyzeAlgorithm(AlgorithmCode, GetAlgorithmType(AlgorithmCode));

    // 基於性能分析生成建議
    if (Profile.TimeComplexity > 1000.0f)
    {
        Suggestions.Add(TEXT("Consider using more efficient data structures"));
        Suggestions.Add(TEXT("Look for opportunities to reduce nested loops"));
        Suggestions.Add(TEXT("Consider memoization or dynamic programming");
    }

    if (Profile.SpaceComplexity > 1000.0f)
    {
        Suggestions.Add(TEXT("Consider in-place algorithms"));
        Suggestions.Add(TEXT("Use memory pooling techniques"));
        Suggestions.Add(TEXT("Optimize data structure sizes"));
    }

    if (Profile.ExecutionTime > 1000.0f)
    {
        Suggestions.Add(TEXT("Consider parallelization"));
        Suggestions.Add(TEXT("Use approximation algorithms"));
        Suggestions.Add(TEXT("Apply heuristic optimization"));
    }

    // 基於算法類型生成特定建議
    EAlgorithmType AlgorithmType = GetAlgorithmType(AlgorithmCode);
    TArray<FString> TypeSpecificSuggestions = GetTypeSpecificSuggestions(AlgorithmType);
    Suggestions.Append(TypeSpecificSuggestions);

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Generated %d optimization suggestions"), Suggestions.Num());
    return Suggestions;
}

FString UMingRTSAlgorithmOptimizer::SelectBestAlgorithm(const TArray<FString>& AlgorithmCodes, EAlgorithmType AlgorithmType)
{
    FString BestAlgorithm;
    float BestScore = -FLT_MAX;

    for (const FString& AlgorithmCode : AlgorithmCodes)
    {
        FAlgorithmProfile Profile = AnalyzeAlgorithm(AlgorithmCode, AlgorithmType);
        float Score = CalculatePerformanceScore(Profile);

        if (Score > BestScore)
        {
            BestScore = Score;
            BestAlgorithm = AlgorithmCode;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Selected best algorithm with score %.2f"), BestScore);
    return BestAlgorithm;
}

TMap<FString, float> UMingRTSAlgorithmOptimizer::CalculateComplexity(const FString& AlgorithmCode)
{
    TMap<FString, float> Complexity;

    // 計算各種複雜度
    float TimeComplexity = CalculateTimeComplexityInternal(AlgorithmCode);
    float SpaceComplexity = CalculateSpaceComplexityInternal(AlgorithmCode);
    float BigOComplexity = CalculateBigOComplexity(AlgorithmCode);

    Complexity.Add(TEXT("Time"), TimeComplexity);
    Complexity.Add(TEXT("Space"), SpaceComplexity);
    Complexity.Add(TEXT("BigO"), BigOComplexity);

    return Complexity;
}

FString UMingRTSAlgorithmOptimizer::OptimizeDataStructure(const FString& AlgorithmCode)
{
    FString OptimizedCode = AlgorithmCode;

    // 分析數據結構使用
    TArray<FString> DataStructures = AnalyzeDataStructures(AlgorithmCode);

    // 優化數據結構
    for (const FString& Structure : DataStructures)
    {
        FString OptimizedStructure = OptimizeDataStructureType(Structure);
        OptimizedCode = OptimizedCode.Replace(*Structure, *OptimizedStructure);
    }

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Data structure optimization completed"));
    return OptimizedCode;
}

FString UMingRTSAlgorithmOptimizer::ApplyMemoization(const FString& AlgorithmCode)
{
    FString MemoizedCode = AlgorithmCode;

    // 識別可記憶化的函數
    TArray<FString> MemoizableFunctions = IdentifyMemoizableFunctions(AlgorithmCode);

    // 應用記憶化
    for (const FString& Function : MemoizableFunctions)
    {
        FString MemoizedFunction = ApplyMemoizationToFunction(Function);
        MemoizedCode = MemoizedCode.Replace(*Function, *MemoizedFunction);
    }

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Memoization applied to %d functions"), MemoizableFunctions.Num());
    return MemoizedCode;
}

FString UMingRTSAlgorithmOptimizer::ParallelizeAlgorithm(const FString& AlgorithmCode)
{
    FString ParallelizedCode = AlgorithmCode;

    // 識別可並行化的部分
    TArray<FString> ParallelizableSections = IdentifyParallelizableSections(AlgorithmCode);

    // 應用並行化
    for (const FString& Section : ParallelizableSections)
    {
        FString ParallelizedSection = ParallelizeSection(Section);
        ParallelizedCode = ParallelizedCode.Replace(*Section, *ParallelizedSection);
    }

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Parallelization applied to %d sections"), ParallelizableSections.Num());
    return ParallelizedCode;
}

FString UMingRTSAlgorithmOptimizer::ApplyHeuristics(const FString& AlgorithmCode)
{
    FString HeuristicCode = AlgorithmCode;

    // 識別可應用啟發式的部分
    TArray<FString> HeuristicApplicable = IdentifyHeuristicApplicableSections(AlgorithmCode);

    // 應用啟發式
    for (const FString& Section : HeuristicApplicable)
    {
        FString HeuristicSection = ApplyHeuristicToSection(Section);
        HeuristicCode = HeuristicCode.Replace(*Section, *HeuristicSection);
    }

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Heuristics applied to %d sections"), HeuristicApplicable.Num());
    return HeuristicCode;
}

TMap<EAlgorithmType, TArray<FString>> UMingRTSAlgorithmOptimizer::GetAlgorithmLibrary() const
{
    return AlgorithmLibrary;
}

void UMingRTSAlgorithmOptimizer::AddCustomAlgorithm(EAlgorithmType AlgorithmType, const FString& AlgorithmCode)
{
    if (!AlgorithmLibrary.Contains(AlgorithmType))
    {
        TArray<FString> EmptyArray;
        AlgorithmLibrary.Add(AlgorithmType, EmptyArray);
    }

    AlgorithmLibrary[AlgorithmType].Add(AlgorithmCode);

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Added custom algorithm to %s library"), *GetAlgorithmTypeName(AlgorithmType));
}

void UMingRTSAlgorithmOptimizer::SetOptimizationParameters(float TimeWeight, float SpaceWeight, float AccuracyWeight)
{
    float TotalWeight = TimeWeight + SpaceWeight + AccuracyWeight;
    
    if (TotalWeight > 0.0f)
    {
        this->TimeWeight = TimeWeight / TotalWeight;
        this->SpaceWeight = SpaceWeight / TotalWeight;
        this->AccuracyWeight = AccuracyWeight / TotalWeight;
    }

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Updated optimization parameters - Time: %.2f, Space: %.2f, Accuracy: %.2f"), 
           this->TimeWeight, this->SpaceWeight, this->AccuracyWeight);
}

TMap<EAlgorithmType, FAlgorithmProfile> UMingRTSAlgorithmOptimizer::GetOptimizationStatistics() const
{
    return OptimizationStatistics;
}

// 私有方法實現

void UMingRTSAlgorithmOptimizer::InitializeAlgorithmLibrary()
{
    AlgorithmLibrary.Empty();

    // 初始化所有算法類型的庫
    for (int32 i = 0; i < 10; i++)
    {
        EAlgorithmType AlgorithmType = (EAlgorithmType)i;
        TArray<FString> EmptyArray;
        AlgorithmLibrary.Add(AlgorithmType, EmptyArray);
    }

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Algorithm library initialized"));
}

void UMingRTSAlgorithmOptimizer::LoadStandardAlgorithms()
{
    AddSortingAlgorithms();
    AddSearchingAlgorithms();
    AddPathfindingAlgorithms();
    AddMachineLearningAlgorithms();

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Standard algorithms loaded"));
}

void UMingRTSAlgorithmOptimizer::SetupOptimizationParameters()
{
    // 設置默認優化參數
    TimeWeight = 0.5f;
    SpaceWeight = 0.3f;
    AccuracyWeight = 0.2f;

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Optimization parameters setup completed"));
}

FOptimizationResult UMingRTSAlgorithmOptimizer::OptimizeTimeComplexity(const FString& AlgorithmCode)
{
    FOptimizationResult Result;
    Result.OriginalAlgorithm = AlgorithmCode;
    Result.Technique = EOptimizationTechnique::TimeComplexity;

    FString OptimizedCode = AlgorithmCode;

    // 應用時間複雜度優化
    OptimizedCode = OptimizeLoops(OptimizedCode);
    OptimizedCode = OptimizeRecursion(OptimizedCode);
    OptimizedCode = OptimizeDataAccess(OptimizedCode);

    Result.OptimizedAlgorithm = OptimizedCode;
    Result.bSuccess = (OptimizedCode != AlgorithmCode);
    Result.OptimizationSteps.Add(TEXT("Loop optimization"));
    Result.OptimizationSteps.Add(TEXT("Recursion optimization"));
    Result.OptimizationSteps.Add(TEXT("Data access optimization"));

    return Result;
}

FOptimizationResult UMingRTSAlgorithmOptimizer::OptimizeSpaceComplexity(const FString& AlgorithmCode)
{
    FOptimizationResult Result;
    Result.OriginalAlgorithm = AlgorithmCode;
    Result.Technique = EOptimizationTechnique::SpaceComplexity;

    FString OptimizedCode = AlgorithmCode;

    // 應用空間複雜度優化
    OptimizedCode = OptimizeMemoryAllocation(OptimizedCode);
    OptimizedCode = OptimizeDataStructures(OptimizedCode);
    OptimizedCode = OptimizeVariableUsage(OptimizedCode);

    Result.OptimizedAlgorithm = OptimizedCode;
    Result.bSuccess = (OptimizedCode != AlgorithmCode);
    Result.OptimizationSteps.Add(TEXT("Memory allocation optimization"));
    Result.OptimizationSteps.Add(TEXT("Data structure optimization"));
    Result.OptimizationSteps.Add(TEXT("Variable usage optimization"));

    return Result;
}

FOptimizationResult UMingRTSAlgorithmOptimizer::OptimizeCachePerformance(const FString& AlgorithmCode)
{
    FOptimizationResult Result;
    Result.OriginalAlgorithm = AlgorithmCode;
    Result.Technique = EOptimizationTechnique::CacheOptimization;

    FString OptimizedCode = AlgorithmCode;

    // 應用緩存優化
    OptimizedCode = OptimizeCacheLocality(OptimizedCode);
    OptimizedCode = OptimizeMemoryAccessPatterns(OptimizedCode);
    OptimizedCode = OptimizeDataLayout(OptimizedCode);

    Result.OptimizedAlgorithm = OptimizedCode;
    Result.bSuccess = (OptimizedCode != AlgorithmCode);
    Result.OptimizationSteps.Add(TEXT("Cache locality optimization"));
    Result.OptimizationSteps.Add(TEXT("Memory access pattern optimization"));
    Result.OptimizationSteps.Add(TEXT("Data layout optimization"));

    return Result;
}

FOptimizationResult UMingRTSAlgorithmOptimizer::OptimizeForParallelization(const FString& AlgorithmCode)
{
    FOptimizationResult Result;
    Result.OriginalAlgorithm = AlgorithmCode;
    Result.Technique = EOptimizationTechnique::Parallelization;

    FString OptimizedCode = AlgorithmCode;

    // 應用並行化優化
    OptimizedCode = ParallelizeAlgorithm(OptimizedCode);
    OptimizedCode = AddSynchronizationPrimitives(OptimizedCode);
    OptimizedCode = OptimizeLoadBalancing(OptimizedCode);

    Result.OptimizedAlgorithm = OptimizedCode;
    Result.bSuccess = (OptimizedCode != AlgorithmCode);
    Result.OptimizationSteps.Add(TEXT("Algorithm parallelization"));
    Result.OptimizationSteps.Add(TEXT("Synchronization primitives"));
    Result.OptimizationSteps.Add(TEXT("Load balancing optimization"));

    return Result;
}

FOptimizationResult UMingRTSAlgorithmOptimizer::ApplyMemoizationOptimization(const FString& AlgorithmCode)
{
    FOptimizationResult Result;
    Result.OriginalAlgorithm = AlgorithmCode;
    Result.Technique = EOptimizationTechnique::Memoization;

    FString OptimizedCode = ApplyMemoization(AlgorithmCode);

    Result.OptimizedAlgorithm = OptimizedCode;
    Result.bSuccess = (OptimizedCode != AlgorithmCode);
    Result.OptimizationSteps.Add(TEXT("Memoization applied"));

    return Result;
}

FOptimizationResult UMingRTSAlgorithmOptimizer::ApplyApproximationOptimization(const FString& AlgorithmCode)
{
    FOptimizationResult Result;
    Result.OriginalAlgorithm = AlgorithmCode;
    Result.Technique = EOptimizationTechnique::Approximation;

    FString OptimizedCode = AlgorithmCode;

    // 應用近似算法
    OptimizedCode = ApplyApproximationTechniques(OptimizedCode);
    OptimizedCode = AddErrorBounds(OptimizedCode);
    OptimizedCode = OptimizeApproximationAccuracy(OptimizedCode);

    Result.OptimizedAlgorithm = OptimizedCode;
    Result.bSuccess = (OptimizedCode != AlgorithmCode);
    Result.OptimizationSteps.Add(TEXT("Approximation techniques applied"));
    Result.OptimizationSteps.Add(TEXT("Error bounds added"));
    Result.OptimizationSteps.Add(TEXT("Approximation accuracy optimized"));

    return Result;
}

FOptimizationResult UMingRTSAlgorithmOptimizer::ApplyHeuristicOptimization(const FString& AlgorithmCode)
{
    FOptimizationResult Result;
    Result.OriginalAlgorithm = AlgorithmCode;
    Result.Technique = EOptimizationTechnique::Heuristic;

    FString OptimizedCode = ApplyHeuristics(AlgorithmCode);

    Result.OptimizedAlgorithm = OptimizedCode;
    Result.bSuccess = (OptimizedCode != AlgorithmCode);
    Result.OptimizationSteps.Add(TEXT("Heuristics applied"));

    return Result;
}

FOptimizationResult UMingRTSAlgorithmOptimizer::ApplyHybridOptimization(const FString& AlgorithmCode)
{
    FOptimizationResult Result;
    Result.OriginalAlgorithm = AlgorithmCode;
    Result.Technique = EOptimizationTechnique::Hybrid;

    FString OptimizedCode = AlgorithmCode;

    // 應用混合優化
    OptimizedCode = CombineMultipleOptimizations(OptimizedCode);
    OptimizedCode = OptimizeAlgorithmSelection(OptimizedCode);
    OptimizedCode = AddAdaptiveBehavior(OptimizedCode);

    Result.OptimizedAlgorithm = OptimizedCode;
    Result.bSuccess = (OptimizedCode != AlgorithmCode);
    Result.OptimizationSteps.Add(TEXT("Multiple optimizations combined"));
    Result.OptimizationSteps.Add(TEXT("Algorithm selection optimized"));
    Result.OptimizationSteps.Add(TEXT("Adaptive behavior added"));

    return Result;
}

float UMingRTSAlgorithmOptimizer::AnalyzeTimeComplexity(const FString& AlgorithmCode)
{
    return CalculateTimeComplexityInternal(AlgorithmCode);
}

float UMingRTSAlgorithmOptimizer::AnalyzeSpaceComplexity(const FString& AlgorithmCode)
{
    return CalculateSpaceComplexityInternal(AlgorithmCode);
}

float UMingRTSAlgorithmOptimizer::AnalyzeCacheEfficiency(const FString& AlgorithmCode)
{
    // 計算緩存效率
    int32 SequentialAccess = CountSequentialAccess(AlgorithmCode);
    int32 RandomAccess = CountRandomAccess(AlgorithmCode);
    int32 TotalAccess = SequentialAccess + RandomAccess;

    if (TotalAccess == 0) return 1.0f;

    return (float)SequentialAccess / TotalAccess;
}

float UMingRTSAlgorithmOptimizer::AnalyzeParallelizability(const FString& AlgorithmCode)
{
    // 計算可並行化程度
    int32 ParallelizableSections = CountParallelizableSections(AlgorithmCode);
    int32 TotalSections = CountTotalSections(AlgorithmCode);

    if (TotalSections == 0) return 0.0f;

    return (float)ParallelizableSections / TotalSections;
}

float UMingRTSAlgorithmOptimizer::CalculateTimeComplexityInternal(const FString& AlgorithmCode)
{
    // 簡單的時間複雜度計算
    int32 LoopCount = CountLoops(AlgorithmCode);
    int32 NestedLoopCount = CountNestedLoops(AlgorithmCode);
    int32 RecursionCount = CountRecursion(AlgorithmCode);

    // 基本複雜度計算
    float Complexity = LoopCount * 1.0f + NestedLoopCount * 10.0f + RecursionCount * 5.0f;

    return Complexity;
}

float UMingRTSAlgorithmOptimizer::CalculateSpaceComplexityInternal(const FString& AlgorithmCode)
{
    // 簡單的空間複雜度計算
    int32 VariableCount = CountVariables(AlgorithmCode);
    int32 ArrayCount = CountArrays(AlgorithmCode);
    int32 StructCount = CountStructs(AlgorithmCode);

    // 基本空間複雜度計算
    float Complexity = VariableCount * 1.0f + ArrayCount * 10.0f + StructCount * 20.0f;

    return Complexity;
}

float UMingRTSAlgorithmOptimizer::CalculateBigOComplexity(const FString& AlgorithmCode)
{
    // 計算BigO複雜度
    float TimeComplexity = CalculateTimeComplexityInternal(AlgorithmCode);
    float SpaceComplexity = CalculateSpaceComplexityInternal(AlgorithmCode);

    // 返回較高的複雜度
    return FMath::Max(TimeComplexity, SpaceComplexity);
}

FString UMingRTSAlgorithmOptimizer::OptimizeLoops(const FString& AlgorithmCode)
{
    FString OptimizedCode = AlgorithmCode;

    // 循環優化
    OptimizedCode = UnrollLoops(OptimizedCode);
    OptimizedCode = FuseLoops(OptimizedCode);
    OptimizedCode = InterchangeLoops(OptimizedCode);

    return OptimizedCode;
}

FString UMingRTSAlgorithmOptimizer::OptimizeRecursion(const FString& AlgorithmCode)
{
    FString OptimizedCode = AlgorithmCode;

    // 遞歸優化
    OptimizedCode = ConvertRecursionToIteration(OptimizedCode);
    OptimizedCode = AddTailRecursion(OptimizedCode);
    OptimizedCode = OptimizeRecursiveCalls(OptimizedCode);

    return OptimizedCode;
}

FString UMingRTSAlgorithmOptimizer::OptimizeDataAccess(const FString& AlgorithmCode)
{
    FString OptimizedCode = AlgorithmCode;

    // 數據訪問優化
    OptimizedCode = OptimizeArrayAccess(OptimizedCode);
    OptimizedCode = OptimizePointerAccess(OptimizedCode);
    OptimizedCode = OptimizeMemoryAccess(OptimizedCode);

    return OptimizedCode;
}

void UMingRTSAlgorithmOptimizer::UpdateOptimizationStatistics(const FString& AlgorithmName, const FAlgorithmProfile& Profile)
{
    EAlgorithmType AlgorithmType = Profile.AlgorithmType;
    OptimizationStatistics.Add(AlgorithmType, Profile);
}

bool UMingRTSAlgorithmOptimizer::IsAlgorithmOptimizable(const FString& AlgorithmCode, EOptimizationTechnique Technique)
{
    // 檢查算法是否可優化
    switch (Technique)
    {
        case EOptimizationTechnique::TimeComplexity:
            return AlgorithmCode.Contains(TEXT("for")) || AlgorithmCode.Contains(TEXT("while"));
        case EOptimizationTechnique::SpaceComplexity:
            return AlgorithmCode.Contains(TEXT("new")) || AlgorithmCode.Contains(TEXT("malloc"));
        case EOptimizationTechnique::Parallelization:
            return CountParallelizableSections(AlgorithmCode) > 0;
        case EOptimizationTechnique::Memoization:
            return IdentifyMemoizableFunctions(AlgorithmCode).Num() > 0;
        default:
            return true;
    }
}

float UMingRTSAlgorithmOptimizer::CalculatePerformanceGain(const FAlgorithmProfile& Original, const FAlgorithmProfile& Optimized)
{
    float TimeGain = (Original.ExecutionTime - Optimized.ExecutionTime) / Original.ExecutionTime;
    float SpaceGain = (Original.MemoryUsage - Optimized.MemoryUsage) / Original.MemoryUsage;

    return TimeGain * TimeWeight + SpaceGain * SpaceWeight;
}

// 標準算法加載

void UMingRTSAlgorithmOptimizer::AddSortingAlgorithms()
{
    // 快速排序
    FString QuickSort = TEXT("void quickSort(int arr[], int low, int high) { if (low < high) { int pi = partition(arr, low, high); quickSort(arr, low, pi - 1); quickSort(arr, pi + 1, high); } }");
    AlgorithmLibrary[EAlgorithmType::Sorting].Add(QuickSort);

    // 歸併排序
    FString MergeSort = TEXT("void mergeSort(int arr[], int l, int r) { if (l < r) { int m = l + (r - l) / 2; mergeSort(arr, l, m); mergeSort(arr, m + 1, r); merge(arr, l, m, r); } }");
    AlgorithmLibrary[EAlgorithmType::Sorting].Add(MergeSort);

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Added sorting algorithms"));
}

void UMingRTSAlgorithmOptimizer::AddSearchingAlgorithms()
{
    // 二分查找
    FString BinarySearch = TEXT("int binarySearch(int arr[], int l, int r, int x) { while (l <= r) { int m = l + (r - l) / 2; if (arr[m] == x) return m; if (arr[m] < x) l = m + 1; else r = m - 1; } return -1; }");
    AlgorithmLibrary[EAlgorithmType::Searching].Add(BinarySearch);

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Added searching algorithms"));
}

void UMingRTSAlgorithmOptimizer::AddPathfindingAlgorithms()
{
    // A*算法
    FString AStar = TEXT("void aStar(Node start, Node goal) { PriorityQueue openSet; openSet.add(start); while (!openSet.isEmpty()) { Node current = openSet.remove(); if (current == goal) return reconstructPath(current); for (Node neighbor : getNeighbors(current)) { if (!openSet.contains(neighbor)) { openSet.add(neighbor); } } } }");
    AlgorithmLibrary[EAlgorithmType::Pathfinding].Add(AStar);

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Added pathfinding algorithms"));
}

void UMingRTSAlgorithmOptimizer::AddMachineLearningAlgorithms()
{
    // 簡單的神經網絡
    FString NeuralNetwork = TEXT("void neuralNetwork(float[] inputs, float[] weights, float[] outputs) { for (int i = 0; i < outputs.length; i++) { outputs[i] = 0; for (int j = 0; j < inputs.length; j++) { outputs[i] += inputs[j] * weights[i * inputs.length + j]; } outputs[i] = sigmoid(outputs[i]); } }");
    AlgorithmLibrary[EAlgorithmType::MachineLearning].Add(NeuralNetwork);

    UE_LOG(LogTemp, Log, TEXT("MingRTSAlgorithmOptimizer: Added machine learning algorithms"));
}

// 輔助方法

EAlgorithmType UMingRTSAlgorithmOptimizer::GetAlgorithmType(const FString& AlgorithmCode) const
{
    if (AlgorithmCode.Contains(TEXT("sort")) || AlgorithmCode.Contains(TEXT("Sort")))
        return EAlgorithmType::Sorting;
    if (AlgorithmCode.Contains(TEXT("search")) || AlgorithmCode.Contains(TEXT("Search")))
        return EAlgorithmType::Searching;
    if (AlgorithmCode.Contains(TEXT("path")) || AlgorithmCode.Contains(TEXT("Path")))
        return EAlgorithmType::Pathfinding;
    if (AlgorithmCode.Contains(TEXT("neural")) || AlgorithmCode.Contains(TEXT("Neural")))
        return EAlgorithmType::MachineLearning;
    
    return EAlgorithmType::Sorting; // 默認
}

float UMingRTSAlgorithmOptimizer::CalculatePerformanceScore(const FAlgorithmProfile& Profile)
{
    float TimeScore = 1.0f / (1.0f + Profile.TimeComplexity);
    float SpaceScore = 1.0f / (1.0f + Profile.SpaceComplexity);
    float ExecutionScore = 1.0f / (1.0f + Profile.ExecutionTime);

    return TimeScore * TimeWeight + SpaceScore * SpaceWeight + ExecutionScore * AccuracyWeight;
}

TArray<FString> UMingRTSAlgorithmOptimizer::GetTypeSpecificSuggestions(EAlgorithmType AlgorithmType)
{
    TArray<FString> Suggestions;

    switch (AlgorithmType)
    {
        case EAlgorithmType::Sorting:
            Suggestions.Add(TEXT("Consider using radix sort for uniform keys"));
            Suggestions.Add(TEXT("Use insertion sort for small arrays"));
            break;
        case EAlgorithmType::Searching:
            Suggestions.Add(TEXT("Use hash tables for O(1) lookup"));
            Suggestions.Add(TEXT("Consider interpolation search for uniform data"));
            break;
        case EAlgorithmType::Pathfinding:
            Suggestions.Add(TEXT("Use hierarchical pathfinding for large maps"));
            Suggestions.Add(TEXT("Consider jump point search for grid-based paths"));
            break;
        case EAlgorithmType::MachineLearning:
            Suggestions.Add(TEXT("Use GPU acceleration for neural networks"));
            Suggestions.Add(TEXT("Consider quantization for model compression"));
            break;
        default:
            break;
    }

    return Suggestions;
}

FString UMingRTSAlgorithmOptimizer::GetOptimizationTechniqueName(EOptimizationTechnique Technique) const
{
    switch (Technique)
    {
        case EOptimizationTechnique::TimeComplexity: return TEXT("Time Complexity");
        case EOptimizationTechnique::SpaceComplexity: return TEXT("Space Complexity");
        case EOptimizationTechnique::CacheOptimization: return TEXT("Cache Optimization");
        case EOptimizationTechnique::Parallelization: return TEXT("Parallelization");
        case EOptimizationTechnique::Memoization: return TEXT("Memoization");
        case EOptimizationTechnique::Approximation: return TEXT("Approximation");
        case EOptimizationTechnique::Heuristic: return TEXT("Heuristic");
        case EOptimizationTechnique::Hybrid: return TEXT("Hybrid");
        default: return TEXT("Unknown");
    }
}

FString UMingRTSAlgorithmOptimizer::GetAlgorithmTypeName(EAlgorithmType AlgorithmType) const
{
    switch (AlgorithmType)
    {
        case EAlgorithmType::Sorting: return TEXT("Sorting");
        case EAlgorithmType::Searching: return TEXT("Searching");
        case EAlgorithmType::Pathfinding: return TEXT("Pathfinding");
        case EAlgorithmType::MachineLearning: return TEXT("Machine Learning");
        case EAlgorithmType::DataCompression: return TEXT("Data Compression");
        case EAlgorithmType::Cryptography: return TEXT("Cryptography");
        case EAlgorithmType::Graph: return TEXT("Graph");
        case EAlgorithmType::DynamicProgramming: return TEXT("Dynamic Programming");
        case EAlgorithmType::Greedy: return TEXT("Greedy");
        case EAlgorithmType::DivideAndConquer: return TEXT("Divide and Conquer");
        default: return TEXT("Unknown");
    }
}

// 更多輔助方法的實現...

float UMingRTSAlgorithmOptimizer::EstimateExecutionTime(const FString& AlgorithmCode, float TimeComplexity)
{
    // 估算執行時間
    return TimeComplexity * 0.001f; // 簡單估算
}

float UMingRTSAlgorithmOptimizer::EstimateMemoryUsage(const FString& AlgorithmCode, float SpaceComplexity)
{
    // 估算內存使用
    return SpaceComplexity * 4.0f; // 簡單估算
}

int32 UMingRTSAlgorithmOptimizer::EstimateInputSize(const FString& AlgorithmCode)
{
    // 估算輸入大小
    return CountArrays(AlgorithmCode) * 100; // 簡單估算
}

// 計數方法
int32 UMingRTSAlgorithmOptimizer::CountLoops(const FString& AlgorithmCode)
{
    int32 Count = 0;
    TArray<FString> Lines = AlgorithmCode.ParseIntoArrayLines();
    for (const FString& Line : Lines)
    {
        if (Line.Contains(TEXT("for")) || Line.Contains(TEXT("while")))
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingRTSAlgorithmOptimizer::CountNestedLoops(const FString& AlgorithmCode)
{
    // 簡單的嵌套循環計數
    int32 Count = 0;
    TArray<FString> Lines = AlgorithmCode.ParseIntoArrayLines();
    int32 CurrentNesting = 0;
    
    for (const FString& Line : Lines)
    {
        if (Line.Contains(TEXT("for")) || Line.Contains(TEXT("while")))
        {
            CurrentNesting++;
            if (CurrentNesting > 1) Count++;
        }
        if (Line.Contains(TEXT("}")))
        {
            CurrentNesting = FMath::Max(0, CurrentNesting - 1);
        }
    }
    return Count;
}

int32 UMingRTSAlgorithmOptimizer::CountRecursion(const FString& AlgorithmCode)
{
    int32 Count = 0;
    TArray<FString> Lines = AlgorithmCode.ParseIntoArrayLines();
    for (const FString& Line : Lines)
    {
        if (Line.Contains(TEXT("return")) && Line.Contains(TEXT("(")))
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingRTSAlgorithmOptimizer::CountVariables(const FString& AlgorithmCode)
{
    int32 Count = 0;
    TArray<FString> Lines = AlgorithmCode.ParseIntoArrayLines();
    for (const FString& Line : Lines)
    {
        if (Line.Contains(TEXT("int")) || Line.Contains(TEXT("float")) || Line.Contains(TEXT("double")))
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingRTSAlgorithmOptimizer::CountArrays(const FString& AlgorithmCode)
{
    int32 Count = 0;
    TArray<FString> Lines = AlgorithmCode.ParseIntoArrayLines();
    for (const FString& Line : Lines)
    {
        if (Line.Contains(TEXT("[")) && Line.Contains(TEXT("]")))
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingRTSAlgorithmOptimizer::CountStructs(const FString& AlgorithmCode)
{
    int32 Count = 0;
    TArray<FString> Lines = AlgorithmCode.ParseIntoArrayLines();
    for (const FString& Line : Lines)
    {
        if (Line.Contains(TEXT("struct")) || Line.Contains(TEXT("class")))
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingRTSAlgorithmOptimizer::CountSequentialAccess(const FString& AlgorithmCode)
{
    // 計算順序訪問次數
    return CountArrays(AlgorithmCode); // 簡化實現
}

int32 UMingRTSAlgorithmOptimizer::CountRandomAccess(const FString& AlgorithmCode)
{
    // 計算隨機訪問次數
    return 0; // 簡化實現
}

int32 UMingRTSAlgorithmOptimizer::CountParallelizableSections(const FString& AlgorithmCode)
{
    int32 Count = 0;
    TArray<FString> Lines = AlgorithmCode.ParseIntoArrayLines();
    for (const FString& Line : Lines)
    {
        if (Line.Contains(TEXT("for")) && !Line.Contains(TEXT("nested")))
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingRTSAlgorithmOptimizer::CountTotalSections(const FString& AlgorithmCode)
{
    return CountLoops(AlgorithmCode) + 1; // 簡化實現
}

// 更多優化方法的實現...

FString UMingRTSAlgorithmOptimizer::UnrollLoops(const FString& AlgorithmCode)
{
    // 循環展開
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::FuseLoops(const FString& AlgorithmCode)
{
    // 循環融合
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::InterchangeLoops(const FString& AlgorithmCode)
{
    // 循環交換
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::ConvertRecursionToIteration(const FString& AlgorithmCode)
{
    // 遞歸轉迭代
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::AddTailRecursion(const FString& AlgorithmCode)
{
    // 添加尾遞歸
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::OptimizeRecursiveCalls(const FString& AlgorithmCode)
{
    // 優化遞歸調用
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::OptimizeArrayAccess(const FString& AlgorithmCode)
{
    // 優化數組訪問
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::OptimizePointerAccess(const FString& AlgorithmCode)
{
    // 優化指針訪問
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::OptimizeMemoryAccess(const FString& AlgorithmCode)
{
    // 優化內存訪問
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::OptimizeMemoryAllocation(const FString& AlgorithmCode)
{
    // 優化內存分配
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::OptimizeDataStructures(const FString& AlgorithmCode)
{
    // 優化數據結構
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::OptimizeVariableUsage(const FString& AlgorithmCode)
{
    // 優化變量使用
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::OptimizeCacheLocality(const FString& AlgorithmCode)
{
    // 優化緩存局部性
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::OptimizeMemoryAccessPatterns(const FString& AlgorithmCode)
{
    // 優化內存訪問模式
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::OptimizeDataLayout(const FString& AlgorithmCode)
{
    // 優化數據佈局
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::AddSynchronizationPrimitives(const FString& AlgorithmCode)
{
    // 添加同步原語
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::OptimizeLoadBalancing(const FString& AlgorithmCode)
{
    // 優化負載平衡
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::ApplyApproximationTechniques(const FString& AlgorithmCode)
{
    // 應用近似技術
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::AddErrorBounds(const FString& AlgorithmCode)
{
    // 添加誤差邊界
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::OptimizeApproximationAccuracy(const FString& AlgorithmCode)
{
    // 優化近似精度
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::CombineMultipleOptimizations(const FString& AlgorithmCode)
{
    // 組合多種優化
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::OptimizeAlgorithmSelection(const FString& AlgorithmCode)
{
    // 優化算法選擇
    return AlgorithmCode;
}

FString UMingRTSAlgorithmOptimizer::AddAdaptiveBehavior(const FString& AlgorithmCode)
{
    // 添加自適應行為
    return AlgorithmCode;
}

TArray<FString> UMingRTSAlgorithmOptimizer::AnalyzeDataStructures(const FString& AlgorithmCode)
{
    // 分析數據結構
    TArray<FString> Structures;
    return Structures;
}

FString UMingRTSAlgorithmOptimizer::OptimizeDataStructureType(const FString& Structure)
{
    // 優化數據結構類型
    return Structure;
}

TArray<FString> UMingRTSAlgorithmOptimizer::IdentifyMemoizableFunctions(const FString& AlgorithmCode)
{
    // 識別可記憶化的函數
    TArray<FString> Functions;
    return Functions;
}

FString UMingRTSAlgorithmOptimizer::ApplyMemoizationToFunction(const FString& Function)
{
    // 對函數應用記憶化
    return Function;
}

TArray<FString> UMingRTSAlgorithmOptimizer::IdentifyParallelizableSections(const FString& AlgorithmCode)
{
    // 識別可並行化的部分
    TArray<FString> Sections;
    return Sections;
}

FString UMingRTSAlgorithmOptimizer::ParallelizeSection(const FString& Section)
{
    // 並行化部分
    return Section;
}

TArray<FString> UMingRTSAlgorithmOptimizer::IdentifyHeuristicApplicableSections(const FString& AlgorithmCode)
{
    // 識別可應用啟發式的部分
    TArray<FString> Sections;
    return Sections;
}

FString UMingRTSAlgorithmOptimizer::ApplyHeuristicToSection(const FString& Section)
{
    // 對部分應用啟發式
    return Section;
}
