#include "SageCommand/MingYinYangFiveElements.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingYinYangFiveElements::UMingYinYangFiveElements()
{
    // 初始化五行元素狀態
    ElementStates.SetNum(5); // 5個元素：金木水火土
    for (int32 i = 0; i < ElementStates.Num(); ++i)
    {
        FElementState& State = ElementStates[i];
        State.Element = static_cast<EFiveElements>(i);
        State.State = EElementState::Dormant;
        State.PowerLevel = 50.0f;
        State.Influence = 0.0f;
        State.Stability = 50.0f;
        State.TransitionProgress = 0.0f;
    }
    
    // 初始化陰陽平衡
    CurrentBalance.YinLevel = 50.0f;
    CurrentBalance.YangLevel = 50.0f;
    CurrentBalance.BalanceScore = 0.0f;
    CurrentBalance.HarmonyLevel = 50.0f;
    
    // 設置默認親和元素
    CurrentAffinityElement = EFiveElements::Earth;
}

bool UMingYinYangFiveElements::Initialize()
{
    if (bSystemActive)
    {
        return true;
    }

    // 初始化系統狀態
    bSystemActive = true;
    SystemStability = 100.0f;

    // 設置更新定時器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            UpdateTimerHandle,
            this,
            &UMingYinYangFiveElements::UpdateElementStates,
            0.1f,
            true
        );
    }

    return true;
}

void UMingYinYangFiveElements::Cleanup()
{
    bSystemActive = false;
    
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(UpdateTimerHandle);
    }
    
    ElementStates.Empty();
    EventHistory.Empty();
}

FYinYangBalance UMingYinYangFiveElements::GetCurrentBalance() const
{
    return CurrentBalance;
}

bool UMingYinYangFiveElements::AdjustYinYangBalance(float YinChange, float YangChange, const FString& Reason)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    // 調整陰陽水平
    CurrentBalance.YinLevel = FMath::Clamp(CurrentBalance.YinLevel + YinChange, 0.0f, 100.0f);
    CurrentBalance.YangLevel = FMath::Clamp(CurrentBalance.YangLevel + YangChange, 0.0f, 100.0f);
    
    // 重新計算平衡分數
    CurrentBalance.BalanceScore = CalculateBalanceScore();
    CurrentBalance.HarmonyLevel = CalculateHarmonyLevel();
    
    // 更新陰陽屬性
    UpdateYinYangBalance(0.0f);
    
    // 記錄事件
    RecordYinYangEvent(FString::Printf(TEXT("陰陽平衡調整：%s"), *Reason), 
                       CurrentBalance.CurrentAttribute, CurrentAffinityElement, 
                       FMath::Abs(YinChange) + FMath::Abs(YangChange));
    
    // 觸發事件
    OnYinYangBalanceChanged.Broadcast(CurrentBalance);
    
    return true;
}

bool UMingYinYangFiveElements::SetYinYangAttribute(EYinYangAttribute Attribute)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    CurrentBalance.CurrentAttribute = Attribute;
    
    // 根據屬性調整陰陽水平
    switch (Attribute)
    {
    case EYinYangAttribute::PureYin:
        CurrentBalance.YinLevel = 80.0f;
        CurrentBalance.YangLevel = 20.0f;
        break;
    case EYinYangAttribute::YangInYin:
        CurrentBalance.YinLevel = 65.0f;
        CurrentBalance.YangLevel = 35.0f;
        break;
    case EYinYangAttribute::BalancedYinYang:
        CurrentBalance.YinLevel = 50.0f;
        CurrentBalance.YangLevel = 50.0f;
        break;
    case EYinYangAttribute::YinInYang:
        CurrentBalance.YinLevel = 35.0f;
        CurrentBalance.YangLevel = 65.0f;
        break;
    case EYinYangAttribute::PureYang:
        CurrentBalance.YinLevel = 20.0f;
        CurrentBalance.YangLevel = 80.0f;
        break;
    }
    
    // 重新計算平衡分數
    CurrentBalance.BalanceScore = CalculateBalanceScore();
    CurrentBalance.HarmonyLevel = CalculateHarmonyLevel();
    
    // 觸發事件
    OnYinYangBalanceChanged.Broadcast(CurrentBalance);
    
    return true;
}

float UMingYinYangFiveElements::GetHarmonyLevel() const
{
    return CurrentBalance.HarmonyLevel;
}

FElementState UMingYinYangFiveElements::GetElementState(EFiveElements Element) const
{
    int32 Index = static_cast<int32>(Element);
    if (ElementStates.IsValidIndex(Index))
    {
        return ElementStates[Index];
    }
    return FElementState();
}

bool UMingYinYangFiveElements::SetElementAffinity(EFiveElements Element)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    CurrentAffinityElement = Element;
    
    // 增強親和元素的能量
    int32 Index = static_cast<int32>(Element);
    if (ElementStates.IsValidIndex(Index))
    {
        ElementStates[Index].PowerLevel = FMath::Clamp(ElementStates[Index].PowerLevel + 20.0f, 0.0f, 100.0f);
        ElementStates[Index].Influence = CalculateElementInfluence(Element);
        
        // 觸發事件
        OnElementStateChanged.Broadcast(ElementStates[Index]);
    }
    
    return true;
}

EFiveElements UMingYinYangFiveElements::GetCurrentElement() const
{
    return CurrentAffinityElement;
}

float UMingYinYangFiveElements::GetElementPower(EFiveElements Element) const
{
    int32 Index = static_cast<int32>(Element);
    if (ElementStates.IsValidIndex(Index))
    {
        return ElementStates[Index].PowerLevel;
    }
    return 0.0f;
}

bool UMingYinYangFiveElements::EnhanceElementPower(EFiveElements Element, float Amount)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    int32 Index = static_cast<int32>(Element);
    if (ElementStates.IsValidIndex(Index))
    {
        ElementStates[Index].PowerLevel = FMath::Clamp(ElementStates[Index].PowerLevel + Amount, 0.0f, 100.0f);
        ElementStates[Index].Influence = CalculateElementInfluence(Element);
        
        // 觸發事件
        OnElementStateChanged.Broadcast(ElementStates[Index]);
        
        return true;
    }
    
    return false;
}

FElementRelationship UMingYinYangFiveElements::AnalyzeElementRelationship(EFiveElements Source, EFiveElements Target) const
{
    FElementRelationship Relationship;
    Relationship.SourceElement = Source;
    Relationship.TargetElement = Target;
    Relationship.RelationshipType = DetermineRelationship(Source, Target);
    Relationship.Strength = CalculateRelationshipStrength(Source, Target);
    Relationship.Effectiveness = Relationship.Strength;
    
    return Relationship;
}

float UMingYinYangFiveElements::GetRelationshipStrength(EFiveElements Source, EFiveElements Target) const
{
    return CalculateRelationshipStrength(Source, Target);
}

TArray<EFiveElements> UMingYinYangFiveElements::GetSupportingElements(EFiveElements Element) const
{
    TArray<EFiveElements> SupportingElements;
    
    // 五行相生關係
    switch (Element)
    {
    case EFiveElements::Metal:
        SupportingElements.Add(EFiveElements::Earth); // 土生金
        break;
    case EFiveElements::Wood:
        SupportingElements.Add(EFiveElements::Water); // 水生木
        break;
    case EFiveElements::Water:
        SupportingElements.Add(EFiveElements::Metal); // 金生水
        break;
    case EFiveElements::Fire:
        SupportingElements.Add(EFiveElements::Wood); // 木生火
        break;
    case EFiveElements::Earth:
        SupportingElements.Add(EFiveElements::Fire); // 火生土
        break;
    }
    
    return SupportingElements;
}

TArray<EFiveElements> UMingYinYangFiveElements::GetConflictingElements(EFiveElements Element) const
{
    TArray<EFiveElements> ConflictingElements;
    
    // 五行相克關係
    switch (Element)
    {
    case EFiveElements::Metal:
        ConflictingElements.Add(EFiveElements::Wood); // 金克木
        break;
    case EFiveElements::Wood:
        ConflictingElements.Add(EFiveElements::Earth); // 木克土
        break;
    case EFiveElements::Water:
        ConflictingElements.Add(EFiveElements::Fire); // 水克火
        break;
    case EFiveElements::Fire:
        ConflictingElements.Add(EFiveElements::Metal); // 火克金
        break;
    case EFiveElements::Earth:
        ConflictingElements.Add(EFiveElements::Water); // 土克水
        break;
    }
    
    return ConflictingElements;
}

EYinYangAttribute UMingYinYangFiveElements::GetOptimalAttribute(const FString& Context) const
{
    // 根據上下文確定最佳陰陽屬性
    if (Context.Contains("防守") || Context.Contains("防禦") || Context.Contains("守護"))
    {
        return EYinYangAttribute::PureYin;
    }
    else if (Context.Contains("攻擊") || Context.Contains("進攻") || Context.Contains("戰鬥"))
    {
        return EYinYangAttribute::PureYang;
    }
    else if (Context.Contains("平衡") || Context.Contains("穩定"))
    {
        return EYinYangAttribute::BalancedYinYang;
    }
    else if (Context.Contains("策略") || Context.Contains("計謀"))
    {
        return EYinYangAttribute::YinInYang;
    }
    else
    {
        return EYinYangAttribute::BalancedYinYang;
    }
}

EFiveElements UMingYinYangFiveElements::GetOptimalElement(const FString& Context) const
{
    // 根據上下文確定最佳五行元素
    if (Context.Contains("攻擊") || Context.Contains("戰鬥"))
    {
        return EFiveElements::Fire; // 火主攻擊
    }
    else if (Context.Contains("防禦") || Context.Contains("守護"))
    {
        return EFiveElements::Earth; // 土主防禦
    }
    else if (Context.Contains("移動") || Context.Contains("速度"))
    {
        return EFiveElements::Water; // 水主流動
    }
    else if (Context.Contains("建設") || Context.Contains("成長"))
    {
        return EFiveElements::Wood; // 木主成長
    }
    else if (Context.Contains("堅固") || Context.Contains("防護"))
    {
        return EFiveElements::Metal; // 金主堅固
    }
    else
    {
        return CurrentAffinityElement;
    }
}

float UMingYinYangFiveElements::CalculateStrategicAdvantage(EYinYangAttribute Attribute, EFiveElements Element) const
{
    float AttributeBonus = 1.0f;
    float ElementBonus = 1.0f;
    
    // 計算陰陽屬性加成
    switch (Attribute)
    {
    case EYinYangAttribute::PureYin:
        AttributeBonus = 1.2f; // 防禦加成
        break;
    case EYinYangAttribute::PureYang:
        AttributeBonus = 1.3f; // 攻擊加成
        break;
    case EYinYangAttribute::BalancedYinYang:
        AttributeBonus = 1.1f; // 平衡加成
        break;
    default:
        AttributeBonus = 1.0f;
        break;
    }
    
    // 計算五行元素加成
    int32 Index = static_cast<int32>(Element);
    if (ElementStates.IsValidIndex(Index))
    {
        ElementBonus = 1.0f + (ElementStates[Index].PowerLevel / 100.0f);
    }
    
    return AttributeBonus * ElementBonus;
}

TArray<FString> UMingYinYangFiveElements::GetStrategicRecommendations() const
{
    TArray<FString> Recommendations;
    
    // 基於當前狀態生成建議
    if (CurrentBalance.BalanceScore < -30.0f)
    {
        Recommendations.Add(TEXT("陰陽失衡，建議進行平衡調整"));
    }
    
    if (CurrentBalance.HarmonyLevel < 50.0f)
    {
        Recommendations.Add(TEXT("和諧度不足，建議增強陰陽協調"));
    }
    
    // 檢查元素狀態
    for (const FElementState& State : ElementStates)
    {
        if (State.PowerLevel < 30.0f)
        {
            Recommendations.Add(FString::Printf(TEXT("%s元素能量不足，建議增強"), *GetElementName(State.Element)));
        }
    }
    
    return Recommendations;
}

bool UMingYinYangFiveElements::TriggerElementTransition(EFiveElements Element)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    int32 Index = static_cast<int32>(Element);
    if (!ElementStates.IsValidIndex(Index))
    {
        return false;
    }
    
    // 設置為轉換狀態
    ElementStates[Index].State = EElementState::Transitioning;
    ElementStates[Index].TransitionProgress = 0.0f;
    
    return true;
}

bool UMingYinYangFiveElements::BalanceYinYang()
{
    if (!bSystemActive)
    {
        return false;
    }
    
    // 設置為平衡狀態
    CurrentBalance.YinLevel = 50.0f;
    CurrentBalance.YangLevel = 50.0f;
    CurrentBalance.CurrentAttribute = EYinYangAttribute::BalancedYinYang;
    CurrentBalance.BalanceScore = 0.0f;
    CurrentBalance.HarmonyLevel = 100.0f;
    
    // 觸發事件
    OnYinYangBalanceChanged.Broadcast(CurrentBalance);
    
    return true;
}

bool UMingYinYangFiveElements::HarmonizeElements()
{
    if (!bSystemActive)
    {
        return false;
    }
    
    // 平衡所有元素能量
    for (FElementState& State : ElementStates)
    {
        State.PowerLevel = 50.0f;
        State.Stability = 100.0f;
        State.Influence = CalculateElementInfluence(State.Element);
        State.State = EElementState::Peak;
        
        // 觸發事件
        OnElementStateChanged.Broadcast(State);
    }
    
    return true;
}

FYinYangBalance UMingYinYangFiveElements::PredictBalance(float TimeInFuture) const
{
    FYinYangBalance PredictedBalance = CurrentBalance;
    
    // 簡單的預測邏輯：基於當前衰減率預測未來狀態
    float DecayAmount = YinYangDecayRate * TimeInFuture;
    PredictedBalance.YinLevel = FMath::Clamp(PredictedBalance.YinLevel - DecayAmount, 0.0f, 100.0f);
    PredictedBalance.YangLevel = FMath::Clamp(PredictedBalance.YangLevel - DecayAmount, 0.0f, 100.0f);
    
    // 重新計算平衡分數
    float Difference = PredictedBalance.YinLevel - PredictedBalance.YangLevel;
    PredictedBalance.BalanceScore = Difference;
    PredictedBalance.HarmonyLevel = 100.0f - FMath::Abs(Difference);
    
    return PredictedBalance;
}

EFiveElements UMingYinYangFiveElements::PredictDominantElement(float TimeInFuture) const
{
    EFiveElements DominantElement = CurrentAffinityElement;
    float MaxPower = 0.0f;
    
    // 預測未來最強元素
    for (const FElementState& State : ElementStates)
    {
        float PredictedPower = State.PowerLevel - (ElementDecayRate * TimeInFuture);
        if (PredictedPower > MaxPower)
        {
            MaxPower = PredictedPower;
            DominantElement = State.Element;
        }
    }
    
    return DominantElement;
}

TArray<FYinYangEvent> UMingYinYangFiveElements::PredictUpcomingEvents(int32 EventCount) const
{
    TArray<FYinYangEvent> PredictedEvents;
    
    // 簡單的事件預測邏輯
    for (int32 i = 0; i < EventCount; ++i)
    {
        FYinYangEvent Event;
        Event.EventID = FString::Printf(TEXT("PREDICTED_%d"), i);
        Event.Description = TEXT("預測的陰陽五行事件");
        Event.YinYangChange = CurrentBalance.CurrentAttribute;
        Event.ElementChange = CurrentAffinityElement;
        Event.ImpactLevel = FMath::FRandRange(20.0f, 80.0f);
        Event.Timestamp = FDateTime::Now() + FTimespan::FromHours(i + 1);
        
        PredictedEvents.Add(Event);
    }
    
    return PredictedEvents;
}

TArray<FYinYangEvent> UMingYinYangFiveElements::GetEventHistory() const
{
    return EventHistory;
}

FYinYangEvent UMingYinYangFiveElements::GetLastEvent() const
{
    if (EventHistory.Num() > 0)
    {
        return EventHistory.Last();
    }
    return FYinYangEvent();
}

void UMingYinYangFiveElements::ClearEventHistory()
{
    EventHistory.Empty();
}

// 私有方法實現

float UMingYinYangFiveElements::CalculateYinLevel() const
{
    return CurrentBalance.YinLevel;
}

float UMingYinYangFiveElements::CalculateYangLevel() const
{
    return CurrentBalance.YangLevel;
}

float UMingYinYangFiveElements::CalculateBalanceScore() const
{
    return CurrentBalance.YinLevel - CurrentBalance.YangLevel;
}

float UMingYinYangFiveElements::CalculateHarmonyLevel() const
{
    float Difference = FMath::Abs(CurrentBalance.YinLevel - CurrentBalance.YangLevel);
    return FMath::Clamp(100.0f - Difference, 0.0f, 100.0f);
}

float UMingYinYangFiveElements::CalculateElementPower(EFiveElements Element) const
{
    int32 Index = static_cast<int32>(Element);
    if (ElementStates.IsValidIndex(Index))
    {
        return ElementStates[Index].PowerLevel;
    }
    return 0.0f;
}

float UMingYinYangFiveElements::CalculateElementInfluence(EFiveElements Element) const
{
    float Power = CalculateElementPower(Element);
    return Power * 0.8f; // 影響力是能量的80%
}

float UMingYinYangFiveElements::CalculateElementStability(EFiveElements Element) const
{
    int32 Index = static_cast<int32>(Element);
    if (ElementStates.IsValidIndex(Index))
    {
        return ElementStates[Index].Stability;
    }
    return 0.0f;
}

EElementRelationship UMingYinYangFiveElements::DetermineRelationship(EFiveElements Source, EFiveElements Target) const
{
    if (IsGeneratingRelationship(Source, Target))
    {
        return EElementRelationship::Generating;
    }
    else if (IsOvercomingRelationship(Source, Target))
    {
        return EElementRelationship::Overcoming;
    }
    else if (IsGeneratingRelationship(Target, Source))
    {
        return EElementRelationship::Supporting;
    }
    else if (IsOvercomingRelationship(Target, Source))
    {
        return EElementRelationship::Insulting;
    }
    else
    {
        return EElementRelationship::Neutral;
    }
}

float UMingYinYangFiveElements::CalculateRelationshipStrength(EFiveElements Source, EFiveElements Target) const
{
    EElementRelationship Relationship = DetermineRelationship(Source, Target);
    
    switch (Relationship)
    {
    case EElementRelationship::Generating:
        return 1.5f;
    case EElementRelationship::Overcoming:
        return 1.2f;
    case EElementRelationship::Supporting:
        return 1.3f;
    case EElementRelationship::Insulting:
        return 0.8f;
    default:
        return 1.0f;
    }
}

void UMingYinYangFiveElements::UpdateElementStates(float DeltaTime)
{
    if (!bSystemActive)
    {
        return;
    }
    
    // 更新元素狀態
    for (FElementState& State : ElementStates)
    {
        // 應用衰減
        State.PowerLevel = FMath::Clamp(State.PowerLevel - ElementDecayRate * DeltaTime, 0.0f, 100.0f);
        
        // 更新影響力
        State.Influence = CalculateElementInfluence(State.Element);
        
        // 處理轉換狀態
        if (State.State == EElementState::Transitioning)
        {
            State.TransitionProgress += TransitionSpeed * DeltaTime;
            if (State.TransitionProgress >= 1.0f)
            {
                State.State = EElementState::Peak;
                State.TransitionProgress = 0.0f;
            }
        }
    }
    
    // 更新陰陽平衡
    UpdateYinYangBalance(DeltaTime);
    
    // 更新系統穩定性
    UpdateSystemStability();
}

void UMingYinYangFiveElements::UpdateYinYangBalance(float DeltaTime)
{
    // 應用衰減
    CurrentBalance.YinLevel = FMath::Clamp(CurrentBalance.YinLevel - YinYangDecayRate * DeltaTime, 0.0f, 100.0f);
    CurrentBalance.YangLevel = FMath::Clamp(CurrentBalance.YangLevel - YinYangDecayRate * DeltaTime, 0.0f, 100.0f);
    
    // 重新計算分數
    CurrentBalance.BalanceScore = CalculateBalanceScore();
    CurrentBalance.HarmonyLevel = CalculateHarmonyLevel();
    
    // 更新屬性
    float Difference = CurrentBalance.YinLevel - CurrentBalance.YangLevel;
    if (Difference > 30.0f)
    {
        CurrentBalance.CurrentAttribute = EYinYangAttribute::PureYin;
    }
    else if (Difference > 10.0f)
    {
        CurrentBalance.CurrentAttribute = EYinYangAttribute::YangInYin;
    }
    else if (Difference > -10.0f)
    {
        CurrentBalance.CurrentAttribute = EYinYangAttribute::BalancedYinYang;
    }
    else if (Difference > -30.0f)
    {
        CurrentBalance.CurrentAttribute = EYinYangAttribute::YinInYang;
    }
    else
    {
        CurrentBalance.CurrentAttribute = EYinYangAttribute::PureYang;
    }
}

void UMingYinYangFiveElements::UpdateSystemStability()
{
    // 基於陰陽平衡和元素狀態計算系統穩定性
    float YinYangStability = CurrentBalance.HarmonyLevel;
    
    float ElementStability = 0.0f;
    for (const FElementState& State : ElementStates)
    {
        ElementStability += State.Stability;
    }
    ElementStability /= ElementStates.Num();
    
    SystemStability = (YinYangStability + ElementStability) / 2.0f;
}

void UMingYinYangFiveElements::RecordYinYangEvent(const FString& Description, EYinYangAttribute YinYangChange, EFiveElements ElementChange, float Impact)
{
    FYinYangEvent Event;
    Event.EventID = FString::Printf(TEXT("YY_%lld"), FDateTime::Now().GetTicks());
    Event.Description = Description;
    Event.YinYangChange = YinYangChange;
    Event.ElementChange = ElementChange;
    Event.ImpactLevel = Impact;
    Event.Timestamp = FDateTime::Now();
    
    EventHistory.Add(Event);
    
    // 限制歷史記錄數量
    if (EventHistory.Num() > 1000)
    {
        EventHistory.RemoveAt(0);
    }
}

void UMingYinYangFiveElements::ProcessYinYangEvent(const FYinYangEvent& Event)
{
    // 處理事件的影響
    // 這裡可以添加具體的事件處理邏輯
}

FString UMingYinYangFiveElements::GetElementName(EFiveElements Element) const
{
    switch (Element)
    {
    case EFiveElements::Metal: return TEXT("金");
    case EFiveElements::Wood: return TEXT("木");
    case EFiveElements::Water: return TEXT("水");
    case EFiveElements::Fire: return TEXT("火");
    case EFiveElements::Earth: return TEXT("土");
    default: return TEXT("未知");
    }
}

FString UMingYinYangFiveElements::GetAttributeName(EYinYangAttribute Attribute) const
{
    switch (Attribute)
    {
    case EYinYangAttribute::PureYin: return TEXT("純陰");
    case EYinYangAttribute::YangInYin: return TEXT("陰中陽");
    case EYinYangAttribute::BalancedYinYang: return TEXT("陰陽平衡");
    case EYinYangAttribute::YinInYang: return TEXT("陽中陰");
    case EYinYangAttribute::PureYang: return TEXT("純陽");
    default: return TEXT("未知");
    }
}

FString UMingYinYangFiveElements::GetStateName(EElementState State) const
{
    switch (State)
    {
    case EElementState::Dormant: return TEXT("潛伏");
    case EElementState::Rising: return TEXT("上升");
    case EElementState::Peak: return TEXT("巔峰");
    case EElementState::Declining: return TEXT("衰退");
    case EElementState::Transitioning: return TEXT("轉換");
    default: return TEXT("未知");
    }
}

FString UMingYinYangFiveElements::GetRelationshipName(EElementRelationship Relationship) const
{
    switch (Relationship)
    {
    case EElementRelationship::Generating: return TEXT("相生");
    case EElementRelationship::Overcoming: return TEXT("相克");
    case EElementRelationship::Insulting: return TEXT("相侮");
    case EElementRelationship::Supporting: return TEXT("支持");
    case EElementRelationship::Neutral: return TEXT("中性");
    default: return TEXT("未知");
    }
}

bool UMingYinYangFiveElements::IsGeneratingRelationship(EFiveElements Source, EFiveElements Target) const
{
    // 五行相生關係
    return (Source == EFiveElements::Metal && Target == EFiveElements::Wood) ||
           (Source == EFiveElements::Wood && Target == EFiveElements::Fire) ||
           (Source == EFiveElements::Water && Target == EFiveElements::Wood) ||
           (Source == EFiveElements::Fire && Target == EFiveElements::Earth) ||
           (Source == EFiveElements::Earth && Target == EFiveElements::Metal);
}

bool UMingYinYangFiveElements::IsOvercomingRelationship(EFiveElements Source, EFiveElements Target) const
{
    // 五行相克關係
    return (Source == EFiveElements::Metal && Target == EFiveElements::Wood) ||
           (Source == EFiveElements::Wood && Target == EFiveElements::Earth) ||
           (Source == EFiveElements::Water && Target == EFiveElements::Fire) ||
           (Source == EFiveElements::Fire && Target == EFiveElements::Metal) ||
           (Source == EFiveElements::Earth && Target == EFiveElements::Water);
}
