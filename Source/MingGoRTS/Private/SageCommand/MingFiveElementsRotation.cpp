#include "SageCommand/MingFiveElementsRotation.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingFiveElementsRotation::UMingFiveElementsRotation()
{
    // 初始化五行輪轉順序（按相生順序）
    RotationOrder.Add(EFiveElements::Metal);  // 金
    RotationOrder.Add(EFiveElements::Wood);   // 木
    RotationOrder.Add(EFiveElements::Water);  // 水
    RotationOrder.Add(EFiveElements::Fire);   // 火
    RotationOrder.Add(EFiveElements::Earth);  // 土
    
    // 初始化輪轉配置
    RotationConfig.Mode = ERotationMode::Natural;
    RotationConfig.DefaultAlignment = EAlignmentAttribute::Righteous;
    RotationConfig.RotationSpeed = 1.0f;
    RotationConfig.TransitionDuration = 5.0f;
    RotationConfig.bAutoRotate = true;
    RotationConfig.bAllowEvilRotation = false;
    RotationConfig.EvilThreshold = 70.0f;
    
    // 初始化當前狀態
    CurrentRotationState = ERotationState::Idle;
    CurrentElement = EFiveElements::Earth;
    CurrentAlignment = EAlignmentAttribute::Righteous;
    bFallPreventionActive = true;
}

bool UMingFiveElementsRotation::Initialize()
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
            RotationTimerHandle,
            this,
            &UMingFiveElementsRotation::ExecuteRotation,
            1.0f,
            true
        );
        
        World->GetTimerManager().SetTimer(
            EffectUpdateTimerHandle,
            this,
            &UMingFiveElementsRotation::UpdateActiveEffects,
            0.1f,
            true
        );
        
        World->GetTimerManager().SetTimer(
            AlignmentUpdateTimerHandle,
            this,
            &UMingFiveElementsRotation::UpdateAlignment,
            2.0f,
            true
        );
    }

    return true;
}

void UMingFiveElementsRotation::Cleanup()
{
    bSystemActive = false;
    
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(RotationTimerHandle);
        World->GetTimerManager().ClearTimer(EffectUpdateTimerHandle);
        World->GetTimerManager().ClearTimer(AlignmentUpdateTimerHandle);
    }
    
    ActiveEffects.Empty();
    RotationHistory.Empty();
}

bool UMingFiveElementsRotation::StartRotation(ERotationMode Mode)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    if (CurrentRotationState == ERotationState::Rotating)
    {
        return false; // 已經在輪轉中
    }
    
    RotationConfig.Mode = Mode;
    CurrentRotationState = ERotationState::Rotating;
    
    // 記錄事件
    RecordRotationEvent(FString::Printf(TEXT("開始輪轉：%s"), *GetRotationModeName(Mode)), 
                       CurrentElement, CurrentElement, CurrentAlignment, CurrentRotationState, 50.0f);
    
    return true;
}

bool UMingFiveElementsRotation::StopRotation()
{
    if (!bSystemActive)
    {
        return false;
    }
    
    if (CurrentRotationState != ERotationState::Rotating)
    {
        return false;
    }
    
    CurrentRotationState = ERotationState::Idle;
    
    // 記錄事件
    RecordRotationEvent(TEXT("停止輪轉"), CurrentElement, CurrentElement, CurrentAlignment, CurrentRotationState, 30.0f);
    
    return true;
}

bool UMingFiveElementsRotation::PauseRotation()
{
    if (!bSystemActive)
    {
        return false;
    }
    
    if (CurrentRotationState != ERotationState::Rotating)
    {
        return false;
    }
    
    CurrentRotationState = ERotationState::Locked;
    
    // 記錄事件
    RecordRotationEvent(TEXT("暫停輪轉"), CurrentElement, CurrentElement, CurrentAlignment, CurrentRotationState, 25.0f);
    
    return true;
}

bool UMingFiveElementsRotation::ResumeRotation()
{
    if (!bSystemActive)
    {
        return false;
    }
    
    if (CurrentRotationState != ERotationState::Locked)
    {
        return false;
    }
    
    CurrentRotationState = ERotationState::Rotating;
    
    // 記錄事件
    RecordRotationEvent(TEXT("恢復輪轉"), CurrentElement, CurrentElement, CurrentAlignment, CurrentRotationState, 35.0f);
    
    return true;
}

ERotationState UMingFiveElementsRotation::GetRotationState() const
{
    return CurrentRotationState;
}

bool UMingFiveElementsRotation::RotateToElement(EFiveElements TargetElement)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    if (CurrentElement == TargetElement)
    {
        return false; // 已經是目標元素
    }
    
    EFiveElements PreviousElement = CurrentElement;
    
    // 開始轉換
    TransitionToElement(TargetElement);
    
    // 記錄事件
    RecordRotationEvent(FString::Printf(TEXT("輪轉到元素：%s"), *GetElementName(TargetElement)), 
                       PreviousElement, TargetElement, CurrentAlignment, ERotationState::Transitioning, 60.0f);
    
    // 觸發事件
    OnElementRotated.Broadcast(TargetElement);
    
    return true;
}

EFiveElements UMingFiveElementsRotation::GetCurrentElement() const
{
    return CurrentElement;
}

EFiveElements UMingFiveElementsRotation::GetNextElement() const
{
    int32 CurrentIndex = RotationOrder.IndexOfByKey(CurrentElement);
    if (CurrentIndex != INDEX_NONE)
    {
        int32 NextIndex = (CurrentIndex + 1) % RotationOrder.Num();
        return RotationOrder[NextIndex];
    }
    return CurrentElement;
}

EFiveElements UMingFiveElementsRotation::GetPreviousElement() const
{
    int32 CurrentIndex = RotationOrder.IndexOfByKey(CurrentElement);
    if (CurrentIndex != INDEX_NONE)
    {
        int32 PrevIndex = (CurrentIndex - 1 + RotationOrder.Num()) % RotationOrder.Num();
        return RotationOrder[PrevIndex];
    }
    return CurrentElement;
}

bool UMingFiveElementsRotation::SetRotationOrder(const TArray<EFiveElements>& Order)
{
    if (!bSystemActive || Order.Num() != 5)
    {
        return false;
    }
    
    RotationOrder = Order;
    
    // 記錄事件
    RecordRotationEvent(TEXT("設置輪轉順序"), CurrentElement, CurrentElement, CurrentAlignment, CurrentRotationState, 40.0f);
    
    return true;
}

TArray<EFiveElements> UMingFiveElementsRotation::GetRotationOrder() const
{
    return RotationOrder;
}

EAlignmentAttribute UMingFiveElementsRotation::GetCurrentAlignment() const
{
    return CurrentAlignment;
}

bool UMingFiveElementsRotation::SetAlignment(EAlignmentAttribute NewAlignment)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    // 檢查是否允許邪惡輪轉
    if (NewAlignment == EAlignmentAttribute::Evil && !RotationConfig.bAllowEvilRotation)
    {
        return false;
    }
    
    EAlignmentAttribute PreviousAlignment = CurrentAlignment;
    CurrentAlignment = NewAlignment;
    
    // 應用屬性效果
    ApplyAlignmentEffects();
    
    // 記錄事件
    RecordRotationEvent(FString::Printf(TEXT("設置正邪屬性：%s"), *GetAlignmentName(NewAlignment)), 
                       CurrentElement, CurrentElement, NewAlignment, CurrentRotationState, 70.0f);
    
    // 觸發事件
    OnAlignmentChanged.Broadcast(NewAlignment);
    
    return true;
}

bool UMingFiveElementsRotation::IsAlignmentRighteous() const
{
    return CurrentAlignment == EAlignmentAttribute::Righteous || 
           CurrentAlignment == EAlignmentAttribute::Neutral || 
           CurrentAlignment == EAlignmentAttribute::Orderly;
}

float UMingFiveElementsRotation::GetAlignmentPurity() const
{
    // 計算屬性純度
    switch (CurrentAlignment)
    {
    case EAlignmentAttribute::Righteous:
        return 100.0f;
    case EAlignmentAttribute::Neutral:
        return 75.0f;
    case EAlignmentAttribute::Evil:
        return 0.0f;
    case EAlignmentAttribute::Chaotic:
        return 25.0f;
    case EAlignmentAttribute::Orderly:
        return 90.0f;
    default:
        return 50.0f;
    }
}

bool UMingFiveElementsRotation::PurifyAlignment()
{
    if (!bSystemActive)
    {
        return false;
    }
    
    // 淨化為正義屬性
    return SetAlignment(EAlignmentAttribute::Righteous);
}

FRotationEffect UMingFiveElementsRotation::GetCurrentRotationEffect() const
{
    if (ActiveEffects.Num() > 0)
    {
        return ActiveEffects[0]; // 返回第一個活動效果
    }
    
    FRotationEffect EmptyEffect;
    EmptyEffect.FromElement = CurrentElement;
    EmptyEffect.ToElement = CurrentElement;
    EmptyEffect.Alignment = CurrentAlignment;
    EmptyEffect.PowerBonus = 0.0f;
    EmptyEffect.Duration = 0.0f;
    EmptyEffect.EffectDescription = TEXT("無活動效果");
    
    return EmptyEffect;
}

TArray<FRotationEffect> UMingFiveElementsRotation::GetActiveEffects() const
{
    return ActiveEffects;
}

bool UMingFiveElementsRotation::ApplyRotationEffect(const FRotationEffect& Effect)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    ActiveEffects.Add(Effect);
    
    // 觸發事件
    OnRotationEffectApplied.Broadcast(Effect);
    
    return true;
}

bool UMingFiveElementsRotation::RemoveRotationEffect(const FString& EffectID)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    for (int32 i = 0; i < ActiveEffects.Num(); ++i)
    {
        if (ActiveEffects[i].EffectDescription == EffectID)
        {
            ActiveEffects.RemoveAt(i);
            return true;
        }
    }
    
    return false;
}

void UMingFiveElementsRotation::ClearAllEffects()
{
    ActiveEffects.Empty();
}

FRotationConfiguration UMingFiveElementsRotation::GetRotationConfiguration() const
{
    return RotationConfig;
}

bool UMingFiveElementsRotation::SetRotationConfiguration(const FRotationConfiguration& Config)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    RotationConfig = Config;
    
    // 記錄事件
    RecordRotationEvent(TEXT("更新輪轉配置"), CurrentElement, CurrentElement, CurrentAlignment, CurrentRotationState, 45.0f);
    
    return true;
}

bool UMingFiveElementsRotation::SetRotationSpeed(float Speed)
{
    if (!bSystemActive)
    {
        return false;
    }
    
    RotationConfig.RotationSpeed = FMath::Clamp(Speed, 0.1f, 5.0f);
    
    return true;
}

float UMingFiveElementsRotation::GetRotationSpeed() const
{
    return RotationConfig.RotationSpeed;
}

bool UMingFiveElementsRotation::IsFallPreventionActive() const
{
    return bFallPreventionActive;
}

bool UMingFiveElementsRotation::ActivateFallPrevention()
{
    if (!bSystemActive)
    {
        return false;
    }
    
    bFallPreventionActive = true;
    
    // 記錄事件
    RecordRotationEvent(TEXT("啟動防墮機制"), CurrentElement, CurrentElement, CurrentAlignment, CurrentRotationState, 80.0f);
    
    return true;
}

bool UMingFiveElementsRotation::DeactivateFallPrevention()
{
    if (!bSystemActive)
    {
        return false;
    }
    
    bFallPreventionActive = false;
    
    // 記錄事件
    RecordRotationEvent(TEXT("停用防墮機制"), CurrentElement, CurrentElement, CurrentAlignment, CurrentRotationState, 20.0f);
    
    return true;
}

float UMingFiveElementsRotation::GetFallRisk() const
{
    // 計算墮落風險
    float Risk = 0.0f;
    
    // 基於當前屬性計算風險
    switch (CurrentAlignment)
    {
    case EAlignmentAttribute::Righteous:
        Risk = 0.0f;
        break;
    case EAlignmentAttribute::Neutral:
        Risk = 20.0f;
        break;
    case EAlignmentAttribute::Evil:
        Risk = 80.0f;
        break;
    case EAlignmentAttribute::Chaotic:
        Risk = 60.0f;
        break;
    case EAlignmentAttribute::Orderly:
        Risk = 10.0f;
        break;
    }
    
    // 防墮機制降低風險
    if (bFallPreventionActive)
    {
        Risk *= 0.5f;
    }
    
    return Risk;
}

bool UMingFiveElementsRotation::PreventEvilRotation()
{
    if (!bSystemActive)
    {
        return false;
    }
    
    // 如果當前是邪惡屬性，轉換為正義
    if (CurrentAlignment == EAlignmentAttribute::Evil)
    {
        return SetAlignment(EAlignmentAttribute::Righteous);
    }
    
    return true;
}

EFiveElements UMingFiveElementsRotation::GetOptimalElementForSituation(const FString& Situation) const
{
    // 根據情況確定最佳元素
    if (Situation.Contains("攻擊") || Situation.Contains("戰鬥"))
    {
        return EFiveElements::Fire; // 火主攻擊
    }
    else if (Situation.Contains("防禦") || Situation.Contains("守護"))
    {
        return EFiveElements::Earth; // 土主防禦
    }
    else if (Situation.Contains("移動") || Situation.Contains("速度"))
    {
        return EFiveElements::Water; // 水主流動
    }
    else if (Situation.Contains("建設") || Situation.Contains("成長"))
    {
        return EFiveElements::Wood; // 木主成長
    }
    else if (Situation.Contains("堅固") || Situation.Contains("防護"))
    {
        return EFiveElements::Metal; // 金主堅固
    }
    else
    {
        return CurrentElement;
    }
}

EAlignmentAttribute UMingFiveElementsRotation::GetOptimalAlignmentForSituation(const FString& Situation) const
{
    // 根據情況確定最佳屬性
    if (Situation.Contains("正義") || Situation.Contains("道德"))
    {
        return EAlignmentAttribute::Righteous;
    }
    else if (Situation.Contains("平衡") || Situation.Contains("中立"))
    {
        return EAlignmentAttribute::Neutral;
    }
    else if (Situation.Contains("秩序") || Situation.Contains("規律"))
    {
        return EAlignmentAttribute::Orderly;
    }
    else if (Situation.Contains("混亂") || Situation.Contains("變化"))
    {
        return EAlignmentAttribute::Chaotic;
    }
    else
    {
        return CurrentAlignment;
    }
}

TArray<FString> UMingFiveElementsRotation::GetStrategicRecommendations() const
{
    TArray<FString> Recommendations;
    
    // 基於當前狀態生成建議
    if (CurrentRotationState == ERotationState::Idle)
    {
        Recommendations.Add(TEXT("建議啟動輪轉以獲得戰術優勢"));
    }
    
    if (CurrentAlignment == EAlignmentAttribute::Evil && bFallPreventionActive)
    {
        Recommendations.Add(TEXT("檢測到邪惡屬性，防墮機制已啟動"));
    }
    
    if (ActiveEffects.Num() == 0)
    {
        Recommendations.Add(TEXT("當前無活動效果，建議進行輪轉獲得增益"));
    }
    
    if (GetFallRisk() > 50.0f)
    {
        Recommendations.Add(TEXT("墮落風險較高，建議立即進行淨化"));
    }
    
    return Recommendations;
}

float UMingFiveElementsRotation::CalculateStrategicAdvantage(EFiveElements Element, EAlignmentAttribute Alignment) const
{
    float ElementBonus = 1.0f;
    float AlignmentBonus = 1.0f;
    
    // 計算元素加成
    if (Element == CurrentElement)
    {
        ElementBonus = 1.5f; // 當前元素有加成
    }
    
    // 計算屬性加成
    switch (Alignment)
    {
    case EAlignmentAttribute::Righteous:
        AlignmentBonus = 1.3f;
        break;
    case EAlignmentAttribute::Neutral:
        AlignmentBonus = 1.1f;
        break;
    case EAlignmentAttribute::Evil:
        AlignmentBonus = 1.4f; // 邪惡屬性有高風險高回報
        break;
    case EAlignmentAttribute::Chaotic:
        AlignmentBonus = 1.2f;
        break;
    case EAlignmentAttribute::Orderly:
        AlignmentBonus = 1.15f;
        break;
    }
    
    return ElementBonus * AlignmentBonus;
}

TArray<FRotationEvent> UMingFiveElementsRotation::GetRotationHistory() const
{
    return RotationHistory;
}

FRotationEvent UMingFiveElementsRotation::GetLastRotationEvent() const
{
    if (RotationHistory.Num() > 0)
    {
        return RotationHistory.Last();
    }
    return FRotationEvent();
}

void UMingFiveElementsRotation::ClearRotationHistory()
{
    RotationHistory.Empty();
}

// 私有方法實現

void UMingFiveElementsRotation::ExecuteRotation()
{
    if (!bSystemActive || CurrentRotationState != ERotationState::Rotating)
    {
        return;
    }
    
    // 根據輪轉模式執行相應邏輯
    switch (RotationConfig.Mode)
    {
    case ERotationMode::Natural:
        ExecuteNaturalRotation();
        break;
    case ERotationMode::Strategic:
        ExecuteStrategicRotation();
        break;
    case ERotationMode::Defensive:
        ExecuteDefensiveRotation();
        break;
    case ERotationMode::Offensive:
        ExecuteOffensiveRotation();
        break;
    case ERotationMode::Adaptive:
        ExecuteAdaptiveRotation();
        break;
    case ERotationMode::Manual:
        // 手動模式不自動輪轉
        break;
    }
}

void UMingFiveElementsRotation::TransitionToElement(EFiveElements TargetElement)
{
    CurrentRotationState = ERotationState::Transitioning;
    
    // 創建轉換效果
    FRotationEffect TransitionEffect;
    TransitionEffect.FromElement = CurrentElement;
    TransitionEffect.ToElement = TargetElement;
    TransitionEffect.Alignment = CurrentAlignment;
    TransitionEffect.PowerBonus = 20.0f;
    TransitionEffect.Duration = RotationConfig.TransitionDuration;
    TransitionEffect.EffectDescription = FString::Printf(TEXT("輪轉效果：%s→%s"), 
                                                       *GetElementName(CurrentElement), *GetElementName(TargetElement));
    
    ApplyRotationEffect(TransitionEffect);
    
    // 更新當前元素
    CurrentElement = TargetElement;
    
    // 完成轉換
    CompleteRotation();
}

void UMingFiveElementsRotation::CompleteRotation()
{
    CurrentRotationState = ERotationState::Rotating;
    
    // 應用輪轉效果
    ApplyRotationEffects();
}

void UMingFiveElementsRotation::ExecuteNaturalRotation()
{
    // 自然輪轉：按相生順序輪轉
    static float RotationTimer = 0.0f;
    RotationTimer += RotationConfig.RotationSpeed;
    
    if (RotationTimer >= 10.0f) // 每10秒輪轉一次
    {
        EFiveElements NextElement = GetNextElement();
        RotateToElement(NextElement);
        RotationTimer = 0.0f;
    }
}

void UMingFiveElementsRotation::ExecuteStrategicRotation()
{
    // 戰略輪轉：基於戰略需求
    static float StrategyTimer = 0.0f;
    StrategyTimer += RotationConfig.RotationSpeed;
    
    if (StrategyTimer >= 15.0f) // 每15秒評估一次
    {
        FString Situation = TEXT("戰略評估");
        EFiveElements OptimalElement = GetOptimalElementForSituation(Situation);
        
        if (OptimalElement != CurrentElement)
        {
            RotateToElement(OptimalElement);
        }
        
        StrategyTimer = 0.0f;
    }
}

void UMingFiveElementsRotation::ExecuteDefensiveRotation()
{
    // 防禦輪轉：優先防禦元素
    static float DefensiveTimer = 0.0f;
    DefensiveTimer += RotationConfig.RotationSpeed;
    
    if (DefensiveTimer >= 12.0f) // 每12秒檢查一次
    {
        if (CurrentElement != EFiveElements::Earth)
        {
            RotateToElement(EFiveElements::Earth); // 優先轉向土元素
        }
        
        DefensiveTimer = 0.0f;
    }
}

void UMingFiveElementsRotation::ExecuteOffensiveRotation()
{
    // 攻擊輪轉：優先攻擊元素
    static float OffensiveTimer = 0.0f;
    OffensiveTimer += RotationConfig.RotationSpeed;
    
    if (OffensiveTimer >= 8.0f) // 每8秒檢查一次
    {
        if (CurrentElement != EFiveElements::Fire)
        {
            RotateToElement(EFiveElements::Fire); // 優先轉向火元素
        }
        
        OffensiveTimer = 0.0f;
    }
}

void UMingFiveElementsRotation::ExecuteAdaptiveRotation()
{
    // 自適應輪轉：根據情況自動調整
    static float AdaptiveTimer = 0.0f;
    AdaptiveTimer += RotationConfig.RotationSpeed;
    
    if (AdaptiveTimer >= 20.0f) // 每20秒評估一次
    {
        // 模擬情況分析
        FString Situation = TEXT("自適應分析");
        EFiveElements OptimalElement = GetOptimalElementForSituation(Situation);
        EAlignmentAttribute OptimalAlignment = GetOptimalAlignmentForSituation(Situation);
        
        if (OptimalElement != CurrentElement)
        {
            RotateToElement(OptimalElement);
        }
        
        if (OptimalAlignment != CurrentAlignment)
        {
            SetAlignment(OptimalAlignment);
        }
        
        AdaptiveTimer = 0.0f;
    }
}

void UMingFiveElementsRotation::UpdateAlignment()
{
    if (!bSystemActive)
    {
        return;
    }
    
    // 檢查屬性純度
    CheckAlignmentPurity();
    
    // 應用屬性效果
    ApplyAlignmentEffects();
}

void UMingFiveElementsRotation::CheckAlignmentPurity()
{
    // 檢查是否需要防墮
    if (bFallPreventionActive && GetFallRisk() > FallPreventionThreshold)
    {
        HandleEvilRotation();
    }
}

void UMingFiveElementsRotation::ApplyAlignmentEffects()
{
    // 根據當前屬性應用效果
    switch (CurrentAlignment)
    {
    case EAlignmentAttribute::Righteous:
        // 正義屬性提升系統穩定性
        SystemStability = FMath::Clamp(SystemStability + 1.0f, 0.0f, 100.0f);
        break;
    case EAlignmentAttribute::Evil:
        // 邪惡屬性降低系統穩定性
        SystemStability = FMath::Clamp(SystemStability - 2.0f, 0.0f, 100.0f);
        break;
    case EAlignmentAttribute::Chaotic:
        // 混亂屬性增加不確定性
        SystemStability = FMath::Clamp(SystemStability - 1.0f, 0.0f, 100.0f);
        break;
    case EAlignmentAttribute::Orderly:
        // 秩序屬性提升穩定性
        SystemStability = FMath::Clamp(SystemStability + 0.5f, 0.0f, 100.0f);
        break;
    case EAlignmentAttribute::Neutral:
        // 中立屬性保持平衡
        break;
    }
}

void UMingFiveElementsRotation::HandleEvilRotation()
{
    if (CurrentAlignment == EAlignmentAttribute::Evil)
    {
        // 嘗試防止邪惡輪轉
        PreventEvilRotation();
        
        // 記錄事件
        RecordRotationEvent(TEXT("防墮機制觸發"), CurrentElement, CurrentElement, CurrentAlignment, CurrentRotationState, 90.0f);
    }
}

void UMingFiveElementsRotation::ApplyRotationEffects()
{
    // 應用所有活動效果
    for (const FRotationEffect& Effect : ActiveEffects)
    {
        // 這裡可以添加具體的效果應用邏輯
        // 例如：提升單位能力、增加資源產出等
    }
}

void UMingFiveElementsRotation::UpdateActiveEffects(float DeltaTime)
{
    if (!bSystemActive)
    {
        return;
    }
    
    // 更新效果持續時間
    for (int32 i = ActiveEffects.Num() - 1; i >= 0; --i)
    {
        FRotationEffect& Effect = ActiveEffects[i];
        Effect.Duration -= DeltaTime;
        
        if (Effect.Duration <= 0.0f)
        {
            ActiveEffects.RemoveAt(i);
        }
    }
}

void UMingFiveElementsRotation::RemoveExpiredEffects()
{
    // 移除過期效果
    for (int32 i = ActiveEffects.Num() - 1; i >= 0; --i)
    {
        if (ActiveEffects[i].Duration <= 0.0f)
        {
            ActiveEffects.RemoveAt(i);
        }
    }
}

void UMingFiveElementsRotation::RecordRotationEvent(const FString& Description, EFiveElements PreviousElement, EFiveElements CurrentElement, EAlignmentAttribute Alignment, ERotationState State, float Impact)
{
    FRotationEvent Event;
    Event.EventID = FString::Printf(TEXT("ROTATION_%lld"), FDateTime::Now().GetTicks());
    Event.Description = Description;
    Event.PreviousElement = PreviousElement;
    Event.CurrentElement = CurrentElement;
    Event.Alignment = Alignment;
    Event.RotationState = State;
    Event.ImpactLevel = Impact;
    Event.Timestamp = FDateTime::Now();
    
    RotationHistory.Add(Event);
    
    // 限制歷史記錄數量
    if (RotationHistory.Num() > 1000)
    {
        RotationHistory.RemoveAt(0);
    }
}

FString UMingFiveElementsRotation::GetElementName(EFiveElements Element) const
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

FString UMingFiveElementsRotation::GetAlignmentName(EAlignmentAttribute Alignment) const
{
    switch (Alignment)
    {
    case EAlignmentAttribute::Righteous: return TEXT("正義");
    case EAlignmentAttribute::Neutral: return TEXT("中立");
    case EAlignmentAttribute::Evil: return TEXT("邪惡");
    case EAlignmentAttribute::Chaotic: return TEXT("混亂");
    case EAlignmentAttribute::Orderly: return TEXT("秩序");
    default: return TEXT("未知");
    }
}

FString UMingFiveElementsRotation::GetRotationModeName(ERotationMode Mode) const
{
    switch (Mode)
    {
    case ERotationMode::Natural: return TEXT("自然輪轉");
    case ERotationMode::Strategic: return TEXT("戰略輪轉");
    case ERotationMode::Defensive: return TEXT("防禦輪轉");
    case ERotationMode::Offensive: return TEXT("攻擊輪轉");
    case ERotationMode::Adaptive: return TEXT("自適應輪轉");
    case ERotationMode::Manual: return TEXT("手動輪轉");
    default: return TEXT("未知模式");
    }
}

FString UMingFiveElementsRotation::GetStateName(ERotationState State) const
{
    switch (State)
    {
    case ERotationState::Idle: return TEXT("閒置");
    case ERotationState::Rotating: return TEXT("輪轉中");
    case ERotationState::Transitioning: return TEXT("轉換中");
    case ERotationState::Stabilizing: return TEXT("穩定化");
    case ERotationState::Locked: return TEXT("鎖定");
    default: return TEXT("未知狀態");
    }
}

EFiveElements UMingFiveElementsRotation::GetGeneratingElement(EFiveElements Element) const
{
    // 五行相生關係
    switch (Element)
    {
    case EFiveElements::Metal: return EFiveElements::Earth; // 土生金
    case EFiveElements::Wood: return EFiveElements::Water; // 水生木
    case EFiveElements::Water: return EFiveElements::Metal; // 金生水
    case EFiveElements::Fire: return EFiveElements::Wood; // 木生火
    case EFiveElements::Earth: return EFiveElements::Fire; // 火生土
    default: return EFiveElements::Earth;
    }
}

EFiveElements UMingFiveElementsRotation::GetOvercomingElement(EFiveElements Element) const
{
    // 五行相克關係
    switch (Element)
    {
    case EFiveElements::Metal: return EFiveElements::Wood; // 金克木
    case EFiveElements::Wood: return EFiveElements::Earth; // 木克土
    case EFiveElements::Water: return EFiveElements::Fire; // 水克火
    case EFiveElements::Fire: return EFiveElements::Metal; // 火克金
    case EFiveElements::Earth: return EFiveElements::Water; // 土克水
    default: return EFiveElements::Earth;
    }
}

bool UMingFiveElementsRotation::IsGeneratingRelationship(EFiveElements Source, EFiveElements Target) const
{
    return GetGeneratingElement(Target) == Source;
}

bool UMingFiveElementsRotation::IsOvercomingRelationship(EFiveElements Source, EFiveElements Target) const
{
    return GetOvercomingElement(Target) == Source;
}

float UMingFiveElementsRotation::AnalyzeSituationRequirements(const FString& Situation) const
{
    // 分析情況需求
    float RequirementScore = 50.0f;
    
    if (Situation.Contains("攻擊"))
    {
        RequirementScore += 20.0f;
    }
    if (Situation.Contains("防禦"))
    {
        RequirementScore += 15.0f;
    }
    if (Situation.Contains("平衡"))
    {
        RequirementScore += 10.0f;
    }
    
    return FMath::Clamp(RequirementScore, 0.0f, 100.0f);
}

EFiveElements UMingFiveElementsRotation::SelectOptimalElement(const TArray<EFiveElements>& Candidates) const
{
    if (Candidates.Num() == 0)
    {
        return CurrentElement;
    }
    
    // 簡單的選擇邏輯：選擇第一個候選元素
    return Candidates[0];
}

EAlignmentAttribute UMingFiveElementsRotation::SelectOptimalAlignment(const TArray<EAlignmentAttribute>& Candidates) const
{
    if (Candidates.Num() == 0)
    {
        return CurrentAlignment;
    }
    
    // 優先選擇正義屬性
    for (EAlignmentAttribute Alignment : Candidates)
    {
        if (Alignment == EAlignmentAttribute::Righteous)
        {
            return Alignment;
        }
    }
    
    // 否則選擇第一個候選屬性
    return Candidates[0];
}
