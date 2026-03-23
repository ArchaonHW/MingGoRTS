#include "SageBrain/MingMilitaryAuthority.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

UMingMilitaryAuthority::UMingMilitaryAuthority()
{
    bIsInitialized = false;
    MaxMilitaryCommand = 100.0f;
    CommandRestoreRate = 1.0f;
    MaxActiveOrders = 8;
    BaseMoraleDecayRate = 0.1f;
    DisciplineDecayRate = 0.05f;
    
    InitializeDefaultOrders();
}

void UMingMilitaryAuthority::InitializeMilitaryAuthority()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Military Authority already initialized"));
        return;
    }

    CurrentMilitaryStatus = FMilitaryStatus();
    CurrentMilitaryStatus.MilitaryCommand = 50.0f;
    CurrentMilitaryStatus.LeadershipAbility = 50.0f;
    CurrentMilitaryStatus.CombatEffectiveness = 50.0f;
    CurrentMilitaryStatus.TroopMorale = 50.0f;
    CurrentMilitaryStatus.DisciplineLevel = 50.0f;
    CurrentMilitaryStatus.CommandRadius = 100;
    
    ActiveOrders.Empty();
    
    bIsInitialized = true;
    
    OnMilitaryAuthorityInitialized();
    
    UE_LOG(LogTemp, Log, TEXT("Military Authority initialized successfully"));
}

void UMingMilitaryAuthority::SetMilitaryCommand(float CommandLevel)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Military Authority not initialized"));
        return;
    }

    CurrentMilitaryStatus.MilitaryCommand = FMath::Clamp(CommandLevel, 0.0f, MaxMilitaryCommand);
    UpdateMilitaryStatus();
    
    UE_LOG(LogTemp, Log, TEXT("Military Command set to %.2f"), CurrentMilitaryStatus.MilitaryCommand);
}

bool UMingMilitaryAuthority::IssueMilitaryOrder(const FMilitaryOrder& Order)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Military Authority not initialized"));
        return false;
    }

    if (!ValidateMilitaryOrder(Order))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid military order"));
        return false;
    }

    if (!CanIssueOrder(Order.CommandType, 20.0f))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot issue order: insufficient command power"));
        return false;
    }

    if (ActiveOrders.Num() >= MaxActiveOrders)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot issue order: maximum active orders reached"));
        return false;
    }

    FMilitaryOrder NewOrder = Order;
    NewOrder.bIsExecuting = true;
    NewOrder.ExecutionProgress = 0.0f;
    
    ActiveOrders.Add(NewOrder);
    
    OnMilitaryOrderIssued.Broadcast(Order.OrderName);
    OnOrderStarted(Order.OrderName);
    
    UE_LOG(LogTemp, Log, TEXT("Military order issued: %s"), *Order.OrderName);
    return true;
}

void UMingMilitaryAuthority::UpdateOrderExecution(const FString& OrderName, float DeltaTime)
{
    if (!bIsInitialized)
    {
        return;
    }

    for (FMilitaryOrder& Order : ActiveOrders)
    {
        if (Order.OrderName == OrderName && Order.bIsExecuting)
        {
            Order.ExecutionProgress += DeltaTime / Order.ExecutionTime;
            
            if (Order.ExecutionProgress >= 1.0f)
            {
                CompleteOrder(OrderName, true);
            }
            break;
        }
    }
}

bool UMingMilitaryAuthority::CanIssueOrder(EMilitaryCommandType CommandType, float RequiredCommand) const
{
    if (!bIsInitialized || CurrentMilitaryStatus.MilitaryCommand < RequiredCommand)
    {
        return false;
    }

    if (!HasCommandType(CommandType))
    {
        return false;
    }

    return ActiveOrders.Num() < MaxActiveOrders;
}

TArray<FString> UMingMilitaryAuthority::GetAvailableOrders(EMilitaryCommandType CommandType) const
{
    if (const TArray<FString>* Orders = CommandTypeMap.Find(CommandType))
    {
        return *Orders;
    }
    return TArray<FString>();
}

bool UMingMilitaryAuthority::CancelOrder(const FString& OrderName)
{
    if (!bIsInitialized)
    {
        return false;
    }

    for (int32 i = 0; i < ActiveOrders.Num(); ++i)
    {
        if (ActiveOrders[i].OrderName == OrderName)
        {
            ActiveOrders.RemoveAt(i);
            UE_LOG(LogTemp, Log, TEXT("Military order cancelled: %s"), *OrderName);
            return true;
        }
    }
    
    return false;
}

TArray<FMilitaryOrder> UMingMilitaryAuthority::GetActiveOrders() const
{
    return ActiveOrders;
}

void UMingMilitaryAuthority::AddTroops(EUnitType UnitType, int32 Count)
{
    if (!bIsInitialized || Count <= 0)
    {
        return;
    }

    int32& CurrentCount = CurrentMilitaryStatus.UnitCounts.FindOrAdd(UnitType, 0);
    CurrentCount += Count;
    
    CalculateTotalTroops();
    OnTroopsChanged(UnitType, CurrentCount);
    
    UE_LOG(LogTemp, Log, TEXT("Added %d troops of type %d (total: %d)"), Count, (int32)UnitType, CurrentCount);
}

void UMingMilitaryAuthority::RemoveTroops(EUnitType UnitType, int32 Count)
{
    if (!bIsInitialized || Count <= 0)
    {
        return;
    }

    int32& CurrentCount = CurrentMilitaryStatus.UnitCounts.FindOrAdd(UnitType, 0);
    CurrentCount = FMath::Max(0, CurrentCount - Count);
    
    CalculateTotalTroops();
    OnTroopsChanged(UnitType, CurrentCount);
    
    UE_LOG(LogTemp, Log, TEXT("Removed %d troops of type %d (total: %d)"), Count, (int32)UnitType, CurrentCount);
}

void UMingMilitaryAuthority::UpgradeMilitarySkill(EMilitaryCommandType CommandType, float Amount)
{
    if (!bIsInitialized)
    {
        return;
    }

    switch (CommandType)
    {
    case EMilitaryCommandType::DirectCommand:
        CurrentMilitaryStatus.MilitaryCommand += Amount * 0.4f;
        CurrentMilitaryStatus.LeadershipAbility += Amount * 0.3f;
        CurrentMilitaryStatus.DisciplineLevel += Amount * 0.2f;
        break;
    case EMilitaryCommandType::StrategicCommand:
        CurrentMilitaryStatus.MilitaryCommand += Amount * 0.3f;
        CurrentMilitaryStatus.CombatEffectiveness += Amount * 0.4f;
        break;
    case EMilitaryCommandType::TacticalCommand:
        CurrentMilitaryStatus.LeadershipAbility += Amount * 0.4f;
        CurrentMilitaryStatus.CombatEffectiveness += Amount * 0.3f;
        break;
    case EMilitaryCommandType::SupportCommand:
        CurrentMilitaryStatus.TroopMorale += Amount * 0.4f;
        CurrentMilitaryStatus.LeadershipAbility += Amount * 0.2f;
        break;
    case EMilitaryCommandType::SpecialCommand:
        CurrentMilitaryStatus.MilitaryCommand += Amount * 0.5f;
        CurrentMilitaryStatus.CombatEffectiveness += Amount * 0.3f;
        break;
    default:
        break;
    }
    
    UpdateMilitaryStatus();
    
    UE_LOG(LogTemp, Log, TEXT("Military skill upgraded for command type: %d"), (int32)CommandType);
}

float UMingMilitaryAuthority::CalculateCombatEffectiveness() const
{
    float BaseEffectiveness = CurrentMilitaryStatus.CombatEffectiveness;
    
    float MoraleBonus = CurrentMilitaryStatus.TroopMorale / 100.0f * 0.3f;
    float DisciplineBonus = CurrentMilitaryStatus.DisciplineLevel / 100.0f * 0.2f;
    float LeadershipBonus = CurrentMilitaryStatus.LeadershipAbility / 100.0f * 0.2f;
    
    return FMath::Clamp(BaseEffectiveness + MoraleBonus + DisciplineBonus + LeadershipBonus, 0.0f, 100.0f);
}

void UMingMilitaryAuthority::ApplyMilitaryModifier(const FString& ModifierName, float ModifierValue)
{
    if (!bIsInitialized)
    {
        return;
    }

    CurrentMilitaryStatus.MilitaryModifiers.Add(ModifierName, ModifierValue);
    UpdateMilitaryStatus();
    
    UE_LOG(LogTemp, Log, TEXT("Military modifier applied: %s (%.2f)"), *ModifierName, ModifierValue);
}

void UMingMilitaryAuthority::RemoveMilitaryModifier(const FString& ModifierName)
{
    if (!bIsInitialized)
    {
        return;
    }

    CurrentMilitaryStatus.MilitaryModifiers.Remove(ModifierName);
    UpdateMilitaryStatus();
    
    UE_LOG(LogTemp, Log, TEXT("Military modifier removed: %s"), *ModifierName);
}

bool UMingMilitaryAuthority::HasCommandType(EMilitaryCommandType CommandType) const
{
    return CurrentMilitaryStatus.MasteredCommands.Contains(CommandType);
}

void UMingMilitaryAuthority::MasterCommandType(EMilitaryCommandType CommandType)
{
    if (!bIsInitialized || HasCommandType(CommandType))
    {
        return;
    }

    CurrentMilitaryStatus.MasteredCommands.Add(CommandType);
    
    OnCommandTypeMastered(CommandType);
    
    UE_LOG(LogTemp, Log, TEXT("Command type mastered: %d"), (int32)CommandType);
}

int32 UMingMilitaryAuthority::GetTotalTroopCount() const
{
    return CurrentMilitaryStatus.TotalTroops;
}

void UMingMilitaryAuthority::BoostTroopMorale(float Amount)
{
    if (!bIsInitialized)
    {
        return;
    }

    CurrentMilitaryStatus.TroopMorale = FMath::Clamp(CurrentMilitaryStatus.TroopMorale + Amount, 0.0f, 100.0f);
    UpdateMilitaryStatus();
    
    UE_LOG(LogTemp, Log, TEXT("Troop morale boosted by %.2f"), Amount);
}

void UMingMilitaryAuthority::IncreaseDiscipline(float Amount)
{
    if (!bIsInitialized)
    {
        return;
    }

    CurrentMilitaryStatus.DisciplineLevel = FMath::Clamp(CurrentMilitaryStatus.DisciplineLevel + Amount, 0.0f, 100.0f);
    UpdateMilitaryStatus();
    
    UE_LOG(LogTemp, Log, TEXT("Discipline increased by %.2f"), Amount);
}

FString UMingMilitaryAuthority::GetMilitaryRecommendation() const
{
    if (CurrentMilitaryStatus.MilitaryCommand < 30.0f)
    {
        return TEXT("建議加強軍事指揮訓練，提升領導能力");
    }
    else if (CurrentMilitaryStatus.TroopMorale < 40.0f)
    {
        return TEXT("建議提升部隊士氣，加強精神建設");
    }
    else if (CurrentMilitaryStatus.DisciplineLevel < 35.0f)
    {
        return TEXT("建議強化軍紀訓練，提高部隊紀律性");
    }
    else if (CurrentMilitaryStatus.CombatEffectiveness < 40.0f)
    {
        return TEXT("建議提升戰鬥效能，加強實戰訓練");
    }
    else if (CurrentMilitaryStatus.TotalTroops < 50)
    {
        return TEXT("建議擴充部隊規模，增加兵力儲備");
    }
    
    return TEXT("軍事能力均衡，可以執行複雜的軍事行動");
}

void UMingMilitaryAuthority::InitializeDefaultOrders()
{
    CommandTypeMap.Add(EMilitaryCommandType::DirectCommand, TArray<FString>{
        TEXT("前進命令"), TEXT("後退命令"), TEXT("停止命令"), TEXT("集結命令"), TEXT("分散命令")
    });
    
    CommandTypeMap.Add(EMilitaryCommandType::StrategicCommand, TArray<FString>{
        TEXT("戰略部署"), TEXT("兵力調動"), TEXT("戰線推進"), TEXT("戰術撤退"), TEXT("包圍戰術")
    });
    
    CommandTypeMap.Add(EMilitaryCommandType::TacticalCommand, TArray<FString>{
        TEXT("突擊命令"), TEXT("側翼攻擊"), TEXT("埋伏戰術"), TEXT("火力支援"), TEXT("掩護行動")
    });
    
    CommandTypeMap.Add(EMilitaryCommandType::SupportCommand, TArray<FString>{
        TEXT("後勤支援"), TEXT("醫療支援"), TEXT("情報支援"), TEXT("工程支援"), TEXT("補給支援")
    });
    
    CommandTypeMap.Add(EMilitaryCommandType::SpecialCommand, TArray<FString>{
        TEXT("特殊作戰"), TEXT("滲透行動"), TEXT("破壞行動"), TEXT("救援行動"), TEXT("護衛任務")
    });
}

void UMingMilitaryAuthority::UpdateMilitaryStatus()
{
    OnMilitaryStatusChanged(CurrentMilitaryStatus);
}

bool UMingMilitaryAuthority::ValidateMilitaryOrder(const FMilitaryOrder& Order) const
{
    return !Order.OrderName.IsEmpty() && 
           Order.CommandType != EMilitaryCommandType::None &&
           Order.ExecutionTime > 0.0f &&
           Order.OrderPriority > 0.0f;
}

void UMingMilitaryAuthority::CompleteOrder(const FString& OrderName, bool bSuccess)
{
    for (int32 i = 0; i < ActiveOrders.Num(); ++i)
    {
        if (ActiveOrders[i].OrderName == OrderName)
        {
            ActiveOrders.RemoveAt(i);
            OnOrderCompleted.Broadcast(OrderName, bSuccess);
            
            if (bSuccess)
            {
                ExecutedOrders.Add(OrderName);
                CurrentMilitaryStatus.MilitaryCommand += 1.0f;
                CurrentMilitaryStatus.TroopMorale += 2.0f;
            }
            
            UE_LOG(LogTemp, Log, TEXT("Military order completed: %s (Success: %s)"), *OrderName, bSuccess ? TEXT("Yes") : TEXT("No"));
            break;
        }
    }
}

void UMingMilitaryAuthority::CalculateTotalTroops()
{
    CurrentMilitaryStatus.TotalTroops = 0;
    for (const auto& UnitPair : CurrentMilitaryStatus.UnitCounts)
    {
        CurrentMilitaryStatus.TotalTroops += UnitPair.Value;
    }
}
