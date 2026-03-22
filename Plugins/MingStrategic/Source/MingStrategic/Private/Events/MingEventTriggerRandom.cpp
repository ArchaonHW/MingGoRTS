#include "Events/MingEventTriggerRandom.h"
#include "Math/UnrealMathUtility.h"

UMingEventTriggerRandom::UMingEventTriggerRandom()
    : BaseProbability(0.3f)
    , CurrentProbability(0.3f)
    , ProbabilityDecay(0.9f)
    , ProbabilityGrowth(0.01f)
    , MinTriggerInterval(5.0f)
    , MaxTriggerInterval(60.0f)
    , TimeSinceLastTrigger(0.0f)
    , bCanTriggerInCurrentWindow(false)
    , bUseWeightedOptions(false)
{
    TriggerType = EEventTriggerType::Random;
    bOneShot = false; // 隨機觸發器通常是可重複的
}

void UMingEventTriggerRandom::Initialize()
{
    Super::Initialize();
    
    CurrentProbability = BaseProbability;
    TimeSinceLastTrigger = 0.0f;
    bCanTriggerInCurrentWindow = true;
    
    UE_LOG(LogTemp, Log, TEXT("RandomTrigger %s initialized (Probability: %.2f)"),
        *TriggerId, CurrentProbability);
}

void UMingEventTriggerRandom::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // 更新時間計數
    TimeSinceLastTrigger += DeltaTime;
    
    // 更新觸發窗口
    UpdateTriggerWindow(DeltaTime);
    
    // 重新計算概率
    RecalculateProbability(DeltaTime);
}

void UMingEventTriggerRandom::SetBaseProbability(float Probability)
{
    BaseProbability = FMath::Clamp(Probability, 0.0f, 1.0f);
    CurrentProbability = BaseProbability;
}

void UMingEventTriggerRandom::SetProbabilityDecay(float DecayFactor)
{
    ProbabilityDecay = FMath::Clamp(DecayFactor, 0.0f, 1.0f);
}

void UMingEventTriggerRandom::SetIntervalRange(float MinInterval, float MaxInterval)
{
    MinTriggerInterval = FMath::Max(0.0f, MinInterval);
    MaxTriggerInterval = FMath::Max(MinTriggerInterval, MaxInterval);
}

void UMingEventTriggerRandom::AddWeightedOption(const FRandomTriggerOption& Option)
{
    WeightedOptions.Add(Option);
    bUseWeightedOptions = true;
    
    UE_LOG(LogTemp, Verbose, TEXT("RandomTrigger %s added weighted option: %s (Weight: %.2f)"),
        *TriggerId, *Option.OptionEventId, Option.Weight);
}

void UMingEventTriggerRandom::ClearOptions()
{
    WeightedOptions.Empty();
    bUseWeightedOptions = false;
}

float UMingEventTriggerRandom::GetTimeToNextPossibleTrigger() const
{
    if (bCanTriggerInCurrentWindow)
    {
        return 0.0f;
    }
    
    float RemainingTime = MinTriggerInterval - TimeSinceLastTrigger;
    return FMath::Max(0.0f, RemainingTime);
}

FString UMingEventTriggerRandom::TriggerRandomSelection()
{
    if (bUseWeightedOptions && WeightedOptions.Num() > 0)
    {
        return SelectWeightedOption();
    }
    else
    {
        return EventId;
    }
}

void UMingEventTriggerRandom::RecalculateProbability(float DeltaTime)
{
    // 隨時間增加概率 (如果長時間未觸發)
    if (TimeSinceLastTrigger > MinTriggerInterval)
    {
        CurrentProbability += ProbabilityGrowth * DeltaTime;
        CurrentProbability = FMath::Min(CurrentProbability, 1.0f);
    }
}

bool UMingEventTriggerRandom::PerformTrigger()
{
    // 執行隨機檢查
    if (!RollProbability())
    {
        return false;
    }
    
    // 如果使用加權選項，選擇一個
    if (bUseWeightedOptions && WeightedOptions.Num() > 0)
    {
        FString SelectedEventId = SelectWeightedOption();
        if (!SelectedEventId.IsEmpty())
        {
            // 廣播選中的事件
            OnTriggered.Broadcast(SelectedEventId);
            
            UE_LOG(LogTemp, Log, TEXT("RandomTrigger %s selected event: %s"),
                *TriggerId, *SelectedEventId);
        }
    }
    
    // 應用概率衰減
    CurrentProbability *= ProbabilityDecay;
    CurrentProbability = FMath::Max(CurrentProbability, BaseProbability * 0.1f);
    
    // 重置時間計數
    TimeSinceLastTrigger = 0.0f;
    bCanTriggerInCurrentWindow = false;
    
    return true;
}

bool UMingEventTriggerRandom::CheckTriggerCondition() const
{
    // 檢查是否通過最小間隔
    if (!HasPassedMinInterval())
    {
        return false;
    }
    
    // 檢查是否在觸發窗口內
    if (!bCanTriggerInCurrentWindow)
    {
        return false;
    }
    
    return true;
}

bool UMingEventTriggerRandom::RollProbability() const
{
    // 生成0-1的隨機數
    float Roll = FMath::FRand();
    return Roll <= CurrentProbability;
}

FString UMingEventTriggerRandom::SelectWeightedOption()
{
    if (WeightedOptions.Num() == 0)
    {
        return EventId;
    }
    
    // 計算總權重
    float TotalWeight = 0.0f;
    for (const auto& Option : WeightedOptions)
    {
        // 檢查冷卻
        if (Option.LastTriggerTime < 0 || 
            TimeSinceLastTrigger >= Option.MinInterval)
        {
            TotalWeight += Option.Weight;
        }
    }
    
    if (TotalWeight <= 0.0f)
    {
        return EventId; // 所有選項都在冷卻中
    }
    
    // 加權隨機選擇
    float RandomValue = FMath::FRandRange(0.0f, TotalWeight);
    float CumulativeWeight = 0.0f;
    
    for (auto& Option : WeightedOptions)
    {
        // 跳過冷卻中的選項
        if (Option.LastTriggerTime >= 0 && 
            TimeSinceLastTrigger < Option.MinInterval)
        {
            continue;
        }
        
        CumulativeWeight += Option.Weight;
        if (RandomValue <= CumulativeWeight)
        {
            Option.LastTriggerTime = TimeSinceLastTrigger;
            return Option.OptionEventId;
        }
    }
    
    // 默認返回最後一個有效選項
    for (int32 i = WeightedOptions.Num() - 1; i >= 0; --i)
    {
        if (WeightedOptions[i].LastTriggerTime < 0 || 
            TimeSinceLastTrigger >= WeightedOptions[i].MinInterval)
        {
            WeightedOptions[i].LastTriggerTime = TimeSinceLastTrigger;
            return WeightedOptions[i].OptionEventId;
        }
    }
    
    return EventId;
}

void UMingEventTriggerRandom::UpdateTriggerWindow(float DeltaTime)
{
    // 在最小間隔之後開啟觸發窗口
    if (TimeSinceLastTrigger >= MinTriggerInterval && !bCanTriggerInCurrentWindow)
    {
        bCanTriggerInCurrentWindow = true;
        UE_LOG(LogTemp, Verbose, TEXT("RandomTrigger %s trigger window opened"), *TriggerId);
    }
}

bool UMingEventTriggerRandom::HasPassedMinInterval() const
{
    return TimeSinceLastTrigger >= MinTriggerInterval;
}

void UMingEventTriggerRandom::Reset()
{
    Super::Reset();
    
    CurrentProbability = BaseProbability;
    TimeSinceLastTrigger = 0.0f;
    bCanTriggerInCurrentWindow = true;
    
    // 重置所有選項的冷卻
    for (auto& Option : WeightedOptions)
    {
        Option.LastTriggerTime = -1.0f;
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("RandomTrigger %s reset"), *TriggerId);
}
