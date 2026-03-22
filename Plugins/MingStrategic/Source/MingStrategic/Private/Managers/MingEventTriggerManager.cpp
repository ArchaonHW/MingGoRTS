#include "Managers/MingEventTriggerManager.h"

// 靜態實例初始化
UMingEventTriggerManager* UMingEventTriggerManager::Instance = nullptr;

UMingEventTriggerManager::UMingEventTriggerManager()
    : bInitialized(false)
    , bPaused(false)
{
}

void UMingEventTriggerManager::Initialize()
{
    if (bInitialized)
    {
        return;
    }
    
    Triggers.Empty();
    TriggerIdMap.Empty();
    bPaused = false;
    
    Instance = this;
    bInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("EventTriggerManager initialized"));
}

void UMingEventTriggerManager::Shutdown()
{
    if (!bInitialized)
    {
        return;
    }
    
    // 關閉所有觸發器
    for (auto& Trigger : Triggers)
    {
        if (Trigger.IsValid())
        {
            Trigger->Shutdown();
        }
    }
    
    Triggers.Empty();
    TriggerIdMap.Empty();
    Instance = nullptr;
    bInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("EventTriggerManager shutdown"));
}

void UMingEventTriggerManager::Tick(float DeltaTime)
{
    if (!bInitialized || bPaused)
    {
        return;
    }
    
    ProcessTriggerTick(DeltaTime);
}

void UMingEventTriggerManager::RegisterTrigger(UMingEventTrigger* Trigger)
{
    if (!Trigger || !bInitialized)
    {
        return;
    }
    
    // 檢查ID是否已存在
    if (TriggerIdMap.Contains(Trigger->TriggerId))
    {
        UE_LOG(LogTemp, Warning, TEXT("Trigger %s already registered"), *Trigger->TriggerId);
        return;
    }
    
    // 添加到列表
    int32 Index = Triggers.Add(Trigger);
    TriggerIdMap.Add(Trigger->TriggerId, Index);
    
    // 初始化觸發器
    Trigger->Initialize();
    
    // 綁定觸發事件
    Trigger->OnTriggered.AddDynamic(this, &UMingEventTriggerManager::OnTriggerFired);
    
    UE_LOG(LogTemp, Log, TEXT("Trigger registered: %s (Type: %s, Event: %s)"),
        *Trigger->TriggerId,
        *UEnum::GetValueAsString(Trigger->GetTriggerType()),
        *Trigger->EventId);
}

void UMingEventTriggerManager::UnregisterTrigger(const FString& TriggerId)
{
    if (!bInitialized)
    {
        return;
    }
    
    int32* Index = TriggerIdMap.Find(TriggerId);
    if (Index && Triggers.IsValidIndex(*Index))
    {
        // 關閉並移除觸發器
        if (Triggers[*Index].IsValid())
        {
            Triggers[*Index]->Shutdown();
        }
        
        Triggers.RemoveAt(*Index);
        TriggerIdMap.Remove(TriggerId);
        
        // 重建索引映射
        TriggerIdMap.Empty();
        for (int32 i = 0; i < Triggers.Num(); ++i)
        {
            if (Triggers[i].IsValid())
            {
                TriggerIdMap.Add(Triggers[i]->TriggerId, i);
            }
        }
        
        UE_LOG(LogTemp, Log, TEXT("Trigger unregistered: %s"), *TriggerId);
    }
}

bool UMingEventTriggerManager::ManualTrigger(const FString& TriggerId)
{
    UMingEventTrigger* Trigger = GetTrigger(TriggerId);
    if (Trigger)
    {
        return Trigger->Trigger();
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Manual trigger failed: %s not found"), *TriggerId);
    return false;
}

UMingEventTrigger* UMingEventTriggerManager::GetTrigger(const FString& TriggerId) const
{
    const int32* Index = TriggerIdMap.Find(TriggerId);
    if (Index && Triggers.IsValidIndex(*Index) && Triggers[*Index].IsValid())
    {
        return Triggers[*Index].Get();
    }
    return nullptr;
}

TArray<UMingEventTrigger*> UMingEventTriggerManager::GetAllTriggers() const
{
    TArray<UMingEventTrigger*> Result;
    for (const auto& Trigger : Triggers)
    {
        if (Trigger.IsValid())
        {
            Result.Add(Trigger.Get());
        }
    }
    return Result;
}

TArray<UMingEventTrigger*> UMingEventTriggerManager::GetTriggersByType(EEventTriggerType Type) const
{
    TArray<UMingEventTrigger*> Result;
    for (const auto& Trigger : Triggers)
    {
        if (Trigger.IsValid() && Trigger->GetTriggerType() == Type)
        {
            Result.Add(Trigger.Get());
        }
    }
    return Result;
}

void UMingEventTriggerManager::SetTriggerEnabled(const FString& TriggerId, bool bEnabled)
{
    UMingEventTrigger* Trigger = GetTrigger(TriggerId);
    if (Trigger)
    {
        Trigger->SetEnabled(bEnabled);
        UE_LOG(LogTemp, Verbose, TEXT("Trigger %s %s"),
            *TriggerId, bEnabled ? TEXT("enabled") : TEXT("disabled"));
    }
}

void UMingEventTriggerManager::ResetTrigger(const FString& TriggerId)
{
    UMingEventTrigger* Trigger = GetTrigger(TriggerId);
    if (Trigger)
    {
        Trigger->Reset();
        UE_LOG(LogTemp, Log, TEXT("Trigger %s reset"), *TriggerId);
    }
}

void UMingEventTriggerManager::ResetAllTriggers()
{
    for (auto& Trigger : Triggers)
    {
        if (Trigger.IsValid())
        {
            Trigger->Reset();
        }
    }
    UE_LOG(LogTemp, Log, TEXT("All triggers reset"));
}

void UMingEventTriggerManager::SetAllTriggersPaused(bool bInPaused)
{
    bPaused = bInPaused;
    UE_LOG(LogTemp, Log, TEXT("All triggers %s"), bPaused ? TEXT("paused") : TEXT("resumed"));
}

UMingEventTrigger* UMingEventTriggerManager::CreateTrigger(TSubclassOf<UMingEventTrigger> TriggerClass, 
    const FString& TriggerId, const FString& EventId)
{
    if (!TriggerClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("CreateTrigger failed: Invalid class"));
        return nullptr;
    }
    
    // 創建觸發器實例
    UMingEventTrigger* NewTrigger = NewObject<UMingEventTrigger>(this, TriggerClass);
    if (NewTrigger)
    {
        NewTrigger->TriggerId = TriggerId;
        NewTrigger->EventId = EventId;
        
        // 註冊到管理器
        RegisterTrigger(NewTrigger);
    }
    
    return NewTrigger;
}

void UMingEventTriggerManager::ProcessTriggerTick(float DeltaTime)
{
    for (auto& Trigger : Triggers)
    {
        if (Trigger.IsValid())
        {
            Trigger->Tick(DeltaTime);
        }
    }
}

void UMingEventTriggerManager::OnTriggerFired(const FString& EventId)
{
    // 轉發觸發事件
    OnAnyTriggerFired.Broadcast(EventId);
    
    UE_LOG(LogTemp, Verbose, TEXT("Event triggered: %s"), *EventId);
}

void UMingEventTriggerManager::CleanupInvalidTriggers()
{
    // 清理無效的觸發器引用
    for (int32 i = Triggers.Num() - 1; i >= 0; --i)
    {
        if (!Triggers[i].IsValid())
        {
            Triggers.RemoveAt(i);
        }
    }
    
    // 重建映射
    TriggerIdMap.Empty();
    for (int32 i = 0; i < Triggers.Num(); ++i)
    {
        if (Triggers[i].IsValid())
        {
            TriggerIdMap.Add(Triggers[i]->TriggerId, i);
        }
    }
}

UMingEventTriggerManager* UMingEventTriggerManager::Get()
{
    return Instance;
}
