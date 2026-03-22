#include "Events/MingEventTrigger.h"

UMingEventTrigger::UMingEventTrigger()
    : TriggerType(EEventTriggerType::Base)
    , CurrentState(EEventTriggerState::Inactive)
    , bEnabled(true)
    , Priority(0)
    , bOneShot(false)
    , CooldownTime(0.0f)
    , MaxTriggerCount(-1)
    , CurrentCooldown(0.0f)
    , TriggerCount(0)
{
}

void UMingEventTrigger::Initialize()
{
    CurrentState = EEventTriggerState::Active;
    CurrentCooldown = 0.0f;
    TriggerCount = 0;
    
    UE_LOG(LogTemp, Verbose, TEXT("EventTrigger %s initialized"), *TriggerId);
}

void UMingEventTrigger::Shutdown()
{
    CurrentState = EEventTriggerState::Inactive;
    OnTriggered.Clear();
    
    UE_LOG(LogTemp, Verbose, TEXT("EventTrigger %s shutdown"), *TriggerId);
}

void UMingEventTrigger::Tick(float DeltaTime)
{
    if (!bEnabled || CurrentState == EEventTriggerState::Inactive)
    {
        return;
    }
    
    // 更新冷卻
    UpdateCooldown(DeltaTime);
    
    // 檢查是否可以觸發
    if (CurrentState == EEventTriggerState::Active)
    {
        if (CheckTriggerCondition())
        {
            Trigger();
        }
    }
}

bool UMingEventTrigger::CanTrigger() const
{
    if (!bEnabled)
    {
        return false;
    }
    
    if (CurrentState == EEventTriggerState::Cooldown && CurrentCooldown > 0)
    {
        return false;
    }
    
    if (MaxTriggerCount >= 0 && TriggerCount >= MaxTriggerCount)
    {
        return false;
    }
    
    return CurrentState == EEventTriggerState::Active || 
           CurrentState == EEventTriggerState::Cooldown;
}

bool UMingEventTrigger::Trigger()
{
    if (!CanTrigger())
    {
        return false;
    }
    
    // 執行內部觸發邏輯
    if (!PerformTrigger())
    {
        return false;
    }
    
    // 更新狀態
    TriggerCount++;
    
    if (bOneShot)
    {
        SetState(EEventTriggerState::Triggered);
    }
    else if (CooldownTime > 0)
    {
        CurrentCooldown = CooldownTime;
        SetState(EEventTriggerState::Cooldown);
    }
    
    // 發布事件
    OnTriggered.Broadcast(EventId);
    
    UE_LOG(LogTemp, Log, TEXT("EventTrigger %s triggered event %s (count: %d)"),
        *TriggerId, *EventId, TriggerCount);
    
    return true;
}

void UMingEventTrigger::Reset()
{
    CurrentState = bEnabled ? EEventTriggerState::Active : EEventTriggerState::Inactive;
    CurrentCooldown = 0.0f;
    TriggerCount = 0;
    
    UE_LOG(LogTemp, Verbose, TEXT("EventTrigger %s reset"), *TriggerId);
}

void UMingEventTrigger::SetEnabled(bool bInEnabled)
{
    bEnabled = bInEnabled;
    
    if (!bEnabled)
    {
        SetState(EEventTriggerState::Inactive);
    }
    else if (CurrentState == EEventTriggerState::Inactive)
    {
        SetState(EEventTriggerState::Active);
    }
}

void UMingEventTrigger::UpdateCooldown(float DeltaTime)
{
    if (CurrentState == EEventTriggerState::Cooldown && CurrentCooldown > 0)
    {
        CurrentCooldown -= DeltaTime;
        
        if (CurrentCooldown <= 0)
        {
            CurrentCooldown = 0;
            SetState(EEventTriggerState::Active);
            
            UE_LOG(LogTemp, Verbose, TEXT("EventTrigger %s cooldown finished"), *TriggerId);
        }
    }
}

void UMingEventTrigger::SetState(EEventTriggerState NewState)
{
    if (CurrentState != NewState)
    {
        EEventTriggerState OldState = CurrentState;
        CurrentState = NewState;
        
        UE_LOG(LogTemp, Verbose, TEXT("EventTrigger %s state: %s -> %s"),
            *TriggerId,
            *UEnum::GetValueAsString(OldState),
            *UEnum::GetValueAsString(NewState));
    }
}

bool UMingEventTrigger::PerformTrigger()
{
    // 基類默認返回成功，子類應該重寫此方法
    return true;
}

bool UMingEventTrigger::CheckTriggerCondition() const
{
    // 基類默認返回 false，子類應該重寫此方法
    return false;
}
