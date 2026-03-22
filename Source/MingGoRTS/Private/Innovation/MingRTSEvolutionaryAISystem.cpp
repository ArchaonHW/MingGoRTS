#include "MingRTSEvolutionaryAISystem.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "HAL/PlatformFilemanager.h"

UMingRTSEvolutionaryAISystem::UMingRTSEvolutionaryAISystem()
{
    CrossoverRate = 0.7f;
    MutationRate = 0.1f;
    SelectionPressure = 0.8f;
    MaxGeneration = 100;
    FitnessThreshold = 0.8f;
}

void UMingRTSEvolutionaryAISystem::InitializeEvolutionarySystem()
{
    UE_LOG(LogTemp, Log, TEXT("MingRTSEvolutionaryAISystem: Initializing evolutionary AI system..."));

    // 初始化環境因素權重
    EnvironmentalWeights.Empty();
    EnvironmentalWeights.Add(TEXT("CombatPerformance"), 0.3f);
    EnvironmentalWeights.Add(TEXT("EconomicEfficiency"), 0.25f);
    EnvironmentalWeights.Add(TEXT("StrategicPlanning"), 0.2f);
    EnvironmentalWeights.Add(TEXT("Adaptability"), 0.15f);
    EnvironmentalWeights.Add(TEXT("LearningSpeed"), 0.1f);

    UE_LOG(LogTemp, Log, TEXT("MingRTSEvolutionaryAISystem: Evolutionary AI system initialized successfully"));
}

FAIGenome UMingRTSEvolutionaryAISystem::CreateAIGenome(const FString& AIID, const TArray<FEvolutionaryTrait>& InitialTraits)
{
    FAIGenome NewGenome;
    NewGenome.GenomeID = FString::Printf(TEXT("Genome_%s_%d"), *AIID, FMath::RandRange(1000, 9999));
    NewGenome.Traits = InitialTraits;
    NewGenome.FitnessLevel = EAIFitnessLevel::Medium;
    NewGenome.FitnessScore = 0.5f;
    NewGenome.Generation = 1;
    
    AIGenomes.Add(AIID, NewGenome);
    
    // 初始化進化歷史
    TArray<FEvolutionRecord> EmptyHistory;
    EvolutionHistories.Add(AIID, EmptyHistory);
    
    // 設置初始進化階段
    AIEvolutionPhases.Add(AIID, EEvolutionPhase::Adaptation);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSEvolutionaryAISystem: Created genome for AI %s"), *AIID);
    return NewGenome;
}

void UMingRTSEvolutionaryAISystem::ProcessEvolution(const FString& AIID, const TArray<FString>& EnvironmentalFactors)
{
    if (!AIGenomes.Contains(AIID))
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSEvolutionaryAISystem: AI %s not found for evolution"), *AIID);
        return;
    }

    FAIGenome CurrentGenome = AIGenomes[AIID];
    
    // 檢查是否需要進化
    if (!ShouldEvolve(AIID))
    {
        return;
    }

    // 評估當前適應度
    float CurrentFitness = EvaluateFitness(AIID, CurrentGenome);
    
    // 根據環境因素決定進化策略
    EEvolutionPhase CurrentPhase = AIEvolutionPhases[AIID];
    
    FAIGenome NewGenome = CurrentGenome;
    
    switch (CurrentPhase)
    {
        case EEvolutionPhase::Adaptation:
            // 適應階段：微調現有特徵
            PerformMutation(NewGenome);
            break;
            
        case EEvolutionPhase::Learning:
            // 學習階段：從環境中學習
            NewGenome = LearnFromEnvironment(CurrentGenome, EnvironmentalFactors);
            break;
            
        case EEvolutionPhase::Evolution:
            // 進化階段：大幅變化
            PerformMutation(NewGenome);
            PerformMutation(NewGenome); // 雙重變異
            break;
            
        case EEvolutionPhase::Specialization:
            // 特化階段：強化優勢特徵
            NewGenome = SpecializeGenome(CurrentGenome, EnvironmentalFactors);
            break;
    }
    
    // 評估新基因組適應度
    float NewFitness = EvaluateFitness(AIID, NewGenome);
    
    // 如果新基因組更優，則接受進化
    if (NewFitness > CurrentFitness)
    {
        NewGenome.Generation = CurrentGenome.Generation + 1;
        NewGenome.FitnessScore = NewFitness;
        NewGenome.FitnessLevel = DetermineFitnessLevel(NewFitness);
        
        AIGenomes[AIID] = NewGenome;
        
        // 記錄進化
        RecordEvolution(AIID, CurrentGenome, NewGenome, TEXT("EnvironmentalAdaptation"));
        
        // 觸發進化事件
        OnAIEvolution.Broadcast(AIID, CurrentGenome, NewGenome);
        OnFitnessUpdate.Broadcast(AIID, NewFitness);
        
        UE_LOG(LogTemp, Log, TEXT("MingRTSEvolutionaryAISystem: AI %s evolved to generation %d with fitness %.3f"), 
               *AIID, NewGenome.Generation, NewFitness);
    }
    
    // 更新進化階段
    UpdateEvolutionPhase(AIID);
}

FAIGenome UMingRTSEvolutionaryAISystem::CrossoverGenomes(const FAIGenome& Parent1, const FAIGenome& Parent2)
{
    FAIGenome Offspring;
    Offspring.GenomeID = FString::Printf(TEXT("Cross_%s_%s"), *Parent1.GenomeID, *Parent2.GenomeID);
    Offspring.Generation = FMath::Max(Parent1.Generation, Parent2.Generation) + 1;
    Offspring.ParentGenomes.Add(Parent1.GenomeID);
    Offspring.ParentGenomes.Add(Parent2.GenomeID);
    
    // 執行基因交叉
    PerformCrossover(Offspring, Parent1, Parent2);
    
    // 應用變異
    if (FMath::FRand() < MutationRate)
    {
        PerformMutation(Offspring);
    }
    
    // 評估適應度
    Offspring.FitnessScore = (Parent1.FitnessScore + Parent2.FitnessScore) / 2.0f;
    Offspring.FitnessLevel = DetermineFitnessLevel(Offspring.FitnessScore);
    
    return Offspring;
}

FAIGenome UMingRTSEvolutionaryAISystem::MutateGenome(const FAIGenome& Genome, float MutationIntensity)
{
    FAIGenome MutatedGenome = Genome;
    MutatedGenome.GenomeID = FString::Printf(TEXT("Mut_%s_%d"), *Genome.GenomeID, FMath::RandRange(1000, 9999));
    
    // 調整變異率
    float OriginalMutationRate = MutationRate;
    MutationRate = FMath::Clamp(MutationIntensity, 0.0f, 1.0f);
    
    PerformMutation(MutatedGenome);
    
    // 恢復原始變異率
    MutationRate = OriginalMutationRate;
    
    // 重新評估適應度
    MutatedGenome.FitnessScore = EvaluateFitness(TEXT(""), MutatedGenome);
    MutatedGenome.FitnessLevel = DetermineFitnessLevel(MutatedGenome.FitnessScore);
    
    return MutatedGenome;
}

float UMingRTSEvolutionaryAISystem::EvaluateFitness(const FString& AIID, const FAIGenome& Genome)
{
    float TotalFitness = 0.0f;
    float WeightSum = 0.0f;
    
    for (const FEvolutionaryTrait& Trait : Genome.Traits)
    {
        float TraitFitness = CalculateTraitFitness(Trait, GetActiveEnvironmentalFactors());
        float Weight = EnvironmentalWeights.FindRef(Trait.TraitName);
        
        if (Weight > 0.0f)
        {
            TotalFitness += TraitFitness * Weight;
            WeightSum += Weight;
        }
    }
    
    if (WeightSum > 0.0f)
    {
        TotalFitness /= WeightSum;
    }
    
    // 根據進化階段調整適應度
    if (AIEvolutionPhases.Contains(AIID))
    {
        EEvolutionPhase Phase = AIEvolutionPhases[AIID];
        
        switch (Phase)
        {
            case EEvolutionPhase::Adaptation:
                TotalFitness *= 1.1f; // 適應階段加成
                break;
            case EEvolutionPhase::Learning:
                TotalFitness *= 1.05f; // 學習階段輕微加成
                break;
            case EEvolutionPhase::Evolution:
                TotalFitness *= 0.95f; // 進化階段輕微懲罰（風險）
                break;
            case EEvolutionPhase::Specialization:
                TotalFitness *= 1.2f; // 特化階段大幅加成
                break;
        }
    }
    
    return FMath::Clamp(TotalFitness, 0.0f, 1.0f);
}

TArray<FAIGenome> UMingRTSEvolutionaryAISystem::SelectBestGenomes(const TArray<FAIGenome>& Population, int32 SelectionCount)
{
    TArray<FAIGenome> SortedPopulation = Population;
    
    // 按適應度排序
    SortedPopulation.Sort([](const FAIGenome& A, const FAIGenome& B)
    {
        return A.FitnessScore > B.FitnessScore;
    });
    
    // 選擇前N個最佳基因組
    TArray<FAIGenome> BestGenomes;
    for (int32 i = 0; i < FMath::Min(SelectionCount, SortedPopulation.Num()); i++)
    {
        BestGenomes.Add(SortedPopulation[i]);
    }
    
    return BestGenomes;
}

void UMingRTSEvolutionaryAISystem::SetEvolutionPhase(const FString& AIID, EEvolutionPhase NewPhase)
{
    if (AIEvolutionPhases.Contains(AIID))
    {
        EEvolutionPhase OldPhase = AIEvolutionPhases[AIID];
        AIEvolutionPhases[AIID] = NewPhase;
        
        OnEvolutionPhase.Broadcast(AIID, NewPhase);
        
        UE_LOG(LogTemp, Log, TEXT("MingRTSEvolutionaryAISystem: AI %s evolution phase changed from %d to %d"), 
               *AIID, (int32)OldPhase, (int32)NewPhase);
    }
}

FAIGenome UMingRTSEvolutionaryAISystem::GetAIGenome(const FString& AIID) const
{
    return AIGenomes.FindRef(AIID);
}

TArray<FEvolutionRecord> UMingRTSEvolutionaryAISystem::GetEvolutionHistory(const FString& AIID) const
{
    return EvolutionHistories.FindRef(AIID);
}

void UMingRTSEvolutionaryAISystem::SetEvolutionParameters(float CrossoverRate, float MutationRate, float SelectionPressure)
{
    this->CrossoverRate = FMath::Clamp(CrossoverRate, 0.0f, 1.0f);
    this->MutationRate = FMath::Clamp(MutationRate, 0.0f, 1.0f);
    this->SelectionPressure = FMath::Clamp(SelectionPressure, 0.0f, 1.0f);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSEvolutionaryAISystem: Evolution parameters updated - Crossover: %.2f, Mutation: %.2f, Selection: %.2f"), 
           this->CrossoverRate, this->MutationRate, this->SelectionPressure);
}

void UMingRTSEvolutionaryAISystem::ProcessBatchEvolution(const TArray<FString>& AIIDs)
{
    for (const FString& AIID : AIIDs)
    {
        if (AIGenomes.Contains(AIID))
        {
            TArray<FString> EnvironmentalFactors = GetActiveEnvironmentalFactors();
            ProcessEvolution(AIID, EnvironmentalFactors);
        }
    }
}

TMap<FString, float> UMingRTSEvolutionaryAISystem::GetEvolutionStatistics() const
{
    TMap<FString, float> Statistics;
    
    // 計算平均適應度
    float TotalFitness = 0.0f;
    int32 ValidGenomes = 0;
    
    for (const TPair<FString, FAIGenome>& Pair : AIGenomes)
    {
        TotalFitness += Pair.Value.FitnessScore;
        ValidGenomes++;
    }
    
    if (ValidGenomes > 0)
    {
        Statistics.Add(TEXT("AverageFitness"), TotalFitness / ValidGenomes);
    }
    
    // 計算最高適應度
    float MaxFitness = 0.0f;
    for (const TPair<FString, FAIGenome>& Pair : AIGenomes)
    {
        MaxFitness = FMath::Max(MaxFitness, Pair.Value.FitnessScore);
    }
    Statistics.Add(TEXT("MaxFitness"), MaxFitness);
    
    // 計算平均代數
    float TotalGeneration = 0.0f;
    for (const TPair<FString, FAIGenome>& Pair : AIGenomes)
    {
        TotalGeneration += Pair.Value.Generation;
    }
    if (ValidGenomes > 0)
    {
        Statistics.Add(TEXT("AverageGeneration"), TotalGeneration / ValidGenomes);
    }
    
    // 統計進化階段分佈
    TMap<EEvolutionPhase, int32> PhaseCounts;
    for (const TPair<FString, EEvolutionPhase>& Pair : AIEvolutionPhases)
    {
        PhaseCounts.FindOrAdd(Pair.Value)++;
    }
    
    Statistics.Add(TEXT("AdaptationPhaseCount"), PhaseCounts.FindRef(EEvolutionPhase::Adaptation));
    Statistics.Add(TEXT("LearningPhaseCount"), PhaseCounts.FindRef(EEvolutionPhase::Learning));
    Statistics.Add(TEXT("EvolutionPhaseCount"), PhaseCounts.FindRef(EEvolutionPhase::Evolution));
    Statistics.Add(TEXT("SpecializationPhaseCount"), PhaseCounts.FindRef(EEvolutionPhase::Specialization));
    
    return Statistics;
}

void UMingRTSEvolutionaryAISystem::ResetAIEvolution(const FString& AIID)
{
    if (AIGenomes.Contains(AIID))
    {
        FAIGenome& Genome = AIGenomes[AIID];
        Genome.Generation = 1;
        Genome.FitnessScore = 0.5f;
        Genome.FitnessLevel = EAIFitnessLevel::Medium;
        
        // 重置特徵值
        for (FEvolutionaryTrait& Trait : Genome.Traits)
        {
            Trait.TraitValue = 0.5f;
        }
        
        // 清空進化歷史
        if (EvolutionHistories.Contains(AIID))
        {
            EvolutionHistories[AIID].Empty();
        }
        
        // 重置進化階段
        AIEvolutionPhases[AIID] = EEvolutionPhase::Adaptation;
        
        UE_LOG(LogTemp, Log, TEXT("MingRTSEvolutionaryAISystem: AI %s evolution reset"), *AIID);
    }
}

void UMingRTSEvolutionaryAISystem::PerformCrossover(FAIGenome& Offspring, const FAIGenome& Parent1, const FAIGenome& Parent2)
{
    // 確保父母基因組有相同數量的特徵
    int32 TraitCount = FMath::Min(Parent1.Traits.Num(), Parent2.Traits.Num());
    
    for (int32 i = 0; i < TraitCount; i++)
    {
        if (i < Parent1.Traits.Num() && i < Parent2.Traits.Num())
        {
            // 混合特徵
            FEvolutionaryTrait BlendedTrait = BlendTraits(Parent1.Traits[i], Parent2.Traits[i]);
            Offspring.Traits.Add(BlendedTrait);
        }
        else if (i < Parent1.Traits.Num())
        {
            Offspring.Traits.Add(Parent1.Traits[i]);
        }
        else if (i < Parent2.Traits.Num())
        {
            Offspring.Traits.Add(Parent2.Traits[i]);
        }
    }
}

void UMingRTSEvolutionaryAISystem::PerformMutation(FAIGenome& Genome)
{
    for (FEvolutionaryTrait& Trait : Genome.Traits)
    {
        if (FMath::FRand() < MutationRate)
        {
            // 執行變異
            float MutationAmount = FMath::FRandRange(-0.2f, 0.2f);
            Trait.TraitValue = FMath::Clamp(Trait.TraitValue + MutationAmount, 0.0f, 1.0f);
            
            // 偶爾調整變異率
            if (FMath::FRand() < 0.1f)
            {
                Trait.MutationRate = FMath::Clamp(Trait.MutationRate + FMath::FRandRange(-0.05f, 0.05f), 0.01f, 0.5f);
            }
        }
    }
}

float UMingRTSEvolutionaryAISystem::CalculateTraitFitness(const FEvolutionaryTrait& Trait, const TArray<FString>& EnvironmentalFactors)
{
    float BaseFitness = Trait.TraitValue;
    
    // 根據環境因素調整適應度
    for (const FString& Factor : EnvironmentalFactors)
    {
        if (Factor == Trait.TraitName)
        {
            BaseFitness *= 1.2f; // 環境匹配加成
        }
    }
    
    return BaseFitness;
}

void UMingRTSEvolutionaryAISystem::UpdateEvolutionPhase(const FString& AIID)
{
    if (!AIGenomes.Contains(AIID) || !AIEvolutionPhases.Contains(AIID))
    {
        return;
    }
    
    const FAIGenome& Genome = AIGenomes[AIID];
    EEvolutionPhase CurrentPhase = AIEvolutionPhases[AIID];
    
    // 根據適應度和代數決定是否轉換階段
    bool ShouldAdvance = false;
    
    switch (CurrentPhase)
    {
        case EEvolutionPhase::Adaptation:
            if (Genome.FitnessScore > 0.6f && Genome.Generation >= 3)
            {
                ShouldAdvance = true;
            }
            break;
            
        case EEvolutionPhase::Learning:
            if (Genome.FitnessScore > 0.7f && Genome.Generation >= 5)
            {
                ShouldAdvance = true;
            }
            break;
            
        case EEvolutionPhase::Evolution:
            if (Genome.FitnessScore > 0.8f && Genome.Generation >= 8)
            {
                ShouldAdvance = true;
            }
            break;
            
        case EEvolutionPhase::Specialization:
            // 特化階段是終極階段，不會自動轉換
            break;
    }
    
    if (ShouldAdvance)
    {
        EEvolutionPhase NewPhase = (EEvolutionPhase)((int32)CurrentPhase + 1);
        SetEvolutionPhase(AIID, NewPhase);
    }
}

void UMingRTSEvolutionaryAISystem::RecordEvolution(const FString& AIID, const FAIGenome& OldGenome, const FAIGenome& NewGenome, const FString& Trigger)
{
    FEvolutionRecord Record;
    Record.RecordID = FString::Printf(TEXT("Evolution_%s_%s"), *AIID, *FDateTime::Now().ToString());
    Record.Timestamp = FDateTime::Now();
    Record.Phase = AIEvolutionPhases.FindRef(AIID);
    Record.PreviousGenome = OldGenome;
    Record.CurrentGenome = NewGenome;
    Record.EvolutionTrigger = Trigger;
    Record.EnvironmentalFactors = GetActiveEnvironmentalFactors();
    
    if (EvolutionHistories.Contains(AIID))
    {
        EvolutionHistories[AIID].Add(Record);
        
        // 限制歷史記錄數量
        if (EvolutionHistories[AIID].Num() > 100)
        {
            EvolutionHistories[AIID].RemoveAt(0);
        }
    }
}

float UMingRTSEvolutionaryAISystem::CalculateAdaptationFitness(const FAIGenome& Genome)
{
    // 適應階段：穩定性和基礎能力
    float AdaptationScore = 0.0f;
    
    for (const FEvolutionaryTrait& Trait : Genome.Traits)
    {
        // 獎勵中等且穩定的特徵值
        float StabilityScore = 1.0f - FMath::Abs(Trait.TraitValue - 0.5f);
        AdaptationScore += StabilityScore;
    }
    
    return Genome.Traits.Num() > 0 ? AdaptationScore / Genome.Traits.Num() : 0.0f;
}

float UMingRTSEvolutionaryAISystem::CalculateLearningFitness(const FAIGenome& Genome)
{
    // 學習階段：學習能力和適應性
    float LearningScore = 0.0f;
    
    for (const FEvolutionaryTrait& Trait : Genome.Traits)
    {
        // 獎勵高變異率（學習潛力）
        LearningScore += Trait.MutationRate * Trait.TraitValue;
    }
    
    return Genome.Traits.Num() > 0 ? LearningScore / Genome.Traits.Num() : 0.0f;
}

float UMingRTSEvolutionaryAISystem::CalculateEvolutionFitness(const FAIGenome& Genome)
{
    // 進化階段：創新和多樣性
    float EvolutionScore = 0.0f;
    
    for (const FEvolutionaryTrait& Trait : Genome.Traits)
    {
        // 獎勵極端特徵值（專業化）
        float Extremeness = FMath::Max(Trait.TraitValue, 1.0f - Trait.TraitValue);
        EvolutionScore += Extremeness;
    }
    
    return Genome.Traits.Num() > 0 ? EvolutionScore / Genome.Traits.Num() : 0.0f;
}

float UMingRTSEvolutionaryAISystem::CalculateSpecializationFitness(const FAIGenome& Genome)
{
    // 特化階段：專業化和優勢特徵
    float SpecializationScore = 0.0f;
    
    for (const FEvolutionaryTrait& Trait : Genome.Traits)
    {
        // 獎勵高特徵值和低變異率（穩定的專業化）
        if (Trait.TraitValue > 0.8f && Trait.MutationRate < 0.1f)
        {
            SpecializationScore += 1.0f;
        }
    }
    
    return Genome.Traits.Num() > 0 ? SpecializationScore / Genome.Traits.Num() : 0.0f;
}

FEvolutionaryTrait UMingRTSEvolutionaryAISystem::BlendTraits(const FEvolutionaryTrait& Trait1, const FEvolutionaryTrait& Trait2)
{
    FEvolutionaryTrait BlendedTrait;
    BlendedTrait.TraitName = Trait1.TraitName;
    BlendedTrait.TraitValue = (Trait1.TraitValue + Trait2.TraitValue) / 2.0f;
    BlendedTrait.MutationRate = (Trait1.MutationRate + Trait2.MutationRate) / 2.0f;
    BlendedTrait.InheritanceStrength = (Trait1.InheritanceStrength + Trait2.InheritanceStrength) / 2.0f;
    
    return BlendedTrait;
}

bool UMingRTSEvolutionaryAISystem::ShouldEvolve(const FString& AIID)
{
    if (!AIGenomes.Contains(AIID))
    {
        return false;
    }
    
    const FAIGenome& Genome = AIGenomes[AIID];
    
    // 檢查代數限制
    if (Genome.Generation >= MaxGeneration)
    {
        return false;
    }
    
    // 檢查適應度閾值
    if (Genome.FitnessScore >= FitnessThreshold)
    {
        return false;
    }
    
    // 隨機進化機率
    float EvolutionChance = 0.3f + (1.0f - Genome.FitnessScore) * 0.5f;
    return FMath::FRand() < EvolutionChance;
}

TArray<FString> UMingRTSEvolutionaryAISystem::GetActiveEnvironmentalFactors() const
{
    TArray<FString> Factors;
    Factors.Add(TEXT("CombatPerformance"));
    Factors.Add(TEXT("EconomicEfficiency"));
    Factors.Add(TEXT("StrategicPlanning"));
    Factors.Add(TEXT("Adaptability"));
    Factors.Add(TEXT("LearningSpeed"));
    return Factors;
}

EAIFitnessLevel UMingRTSEvolutionaryAISystem::DetermineFitnessLevel(float FitnessScore)
{
    if (FitnessScore >= 0.9f) return EAIFitnessLevel::Exceptional;
    if (FitnessScore >= 0.8f) return EAIFitnessLevel::VeryHigh;
    if (FitnessScore >= 0.6f) return EAIFitnessLevel::High;
    if (FitnessScore >= 0.4f) return EAIFitnessLevel::Medium;
    if (FitnessScore >= 0.2f) return EAIFitnessLevel::Low;
    return EAIFitnessLevel::VeryLow;
}

FAIGenome UMingRTSEvolutionaryAISystem::LearnFromEnvironment(const FAIGenome& Genome, const TArray<FString>& EnvironmentalFactors)
{
    FAIGenome LearnedGenome = Genome;
    
    // 根據環境因素調整特徵
    for (FEvolutionaryTrait& Trait : LearnedGenome.Traits)
    {
        for (const FString& Factor : EnvironmentalFactors)
        {
            if (Factor == Trait.TraitName)
            {
                // 增強匹配環境的特徵
                Trait.TraitValue = FMath::Clamp(Trait.TraitValue + 0.1f, 0.0f, 1.0f);
                Trait.MutationRate = FMath::Clamp(Trait.MutationRate - 0.02f, 0.01f, 0.5f);
            }
        }
    }
    
    return LearnedGenome;
}

FAIGenome UMingRTSEvolutionaryAISystem::SpecializeGenome(const FAIGenome& Genome, const TArray<FString>& EnvironmentalFactors)
{
    FAIGenome SpecializedGenome = Genome;
    
    // 找出最強的特徵並進一步強化
    float MaxTraitValue = 0.0f;
    FString StrongestTraitName;
    
    for (const FEvolutionaryTrait& Trait : Genome.Traits)
    {
        if (Trait.TraitValue > MaxTraitValue)
        {
            MaxTraitValue = Trait.TraitValue;
            StrongestTraitName = Trait.TraitName;
        }
    }
    
    // 強化最強特徵
    for (FEvolutionaryTrait& Trait : SpecializedGenome.Traits)
    {
        if (Trait.TraitName == StrongestTraitName)
        {
            Trait.TraitValue = FMath::Clamp(Trait.TraitValue + 0.15f, 0.0f, 1.0f);
            Trait.MutationRate = FMath::Clamp(Trait.MutationRate - 0.05f, 0.01f, 0.5f);
        }
        else
        {
            // 削弱其他特徵以專注於強項
            Trait.TraitValue = FMath::Clamp(Trait.TraitValue - 0.05f, 0.0f, 1.0f);
        }
    }
    
    return SpecializedGenome;
}
