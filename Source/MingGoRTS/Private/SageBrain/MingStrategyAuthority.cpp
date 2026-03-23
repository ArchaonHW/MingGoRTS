#include "SageBrain/MingStrategyAuthority.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

UMingStrategyAuthority::UMingStrategyAuthority()
{
    bIsInitialized = false;
    MaxStrategicThinking = 100.0f;
    StrategyRestoreRate = 1.0f;
    MaxActiveStrategies = 5;
    
    InitializeDefaultStrategies();
}

void UMingStrategyAuthority::InitializeStrategyAuthority()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Strategy Authority already initialized"));
        return;
    }

    CurrentStrategyStatus = FStrategyStatus();
    CurrentStrategyStatus.StrategicThinking = 50.0f;
    CurrentStrategyStatus.PlanningAbility = 50.0f;
    CurrentStrategyStatus.Adaptability = 50.0f;
    CurrentStrategyStatus.Foresight = 50.0f;
    CurrentStrategyStatus.ResourceManagement = 50.0f;
    
    ActiveTactics.Empty();
    
    bIsInitialized = true;
    
    OnStrategyAuthorityInitialized();
    
    UE_LOG(LogTemp, Log, TEXT("Strategy Authority initialized successfully"));
}

void UMingStrategyAuthority::SetStrategicThinking(float ThinkingLevel)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Strategy Authority not initialized"));
        return;
    }

    CurrentStrategyStatus.StrategicThinking = FMath::Clamp(ThinkingLevel, 0.0f, MaxStrategicThinking);
    UpdateStrategyStatus();
    
    UE_LOG(LogTemp, Log, TEXT("Strategic Thinking set to %.2f"), CurrentStrategyStatus.StrategicThinking);
}

bool UMingStrategyAuthority::ExecuteStrategy(EStrategyType StrategyType, const FString& StrategyName)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Strategy Authority not initialized"));
        return false;
    }

    if (!CanExecuteStrategy(StrategyType, 20.0f))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot execute strategy: insufficient power or requirements"));
        return false;
    }

    if (ActiveTactics.Num() >= MaxActiveStrategies)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot execute strategy: maximum active tactics reached"));
        return false;
    }

    FTacticPlan NewTactic;
    NewTactic.TacticName = StrategyName;
    NewTactic.StrategyType = StrategyType;
    NewTactic.SuccessChance = CalculateStrategySuccessChance(NewTactic);
    NewTactic.bIsExecuting = true;
    NewTactic.ExecutionProgress = 0.0f;
    
    ActiveTactics.Add(NewTactic);
    
    OnStrategyExecuted.Broadcast(StrategyName);
    OnTacticStarted(StrategyName);
    
    UE_LOG(LogTemp, Log, TEXT("Strategy executed: %s"), *StrategyName);
    return true;
}

bool UMingStrategyAuthority::PlanTactic(const FTacticPlan& Tactic)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Strategy Authority not initialized"));
        return false;
    }

    if (!ValidateTacticPlan(Tactic))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid tactic plan"));
        return false;
    }

    TacticLibrary.Add(Tactic.TacticName, Tactic);
    
    UE_LOG(LogTemp, Log, TEXT("Tactic planned: %s"), *Tactic.TacticName);
    return true;
}

void UMingStrategyAuthority::UpdateTacticExecution(const FString& TacticName, float DeltaTime)
{
    if (!bIsInitialized)
    {
        return;
    }

    for (FTacticPlan& Tactic : ActiveTactics)
    {
        if (Tactic.TacticName == TacticName && Tactic.bIsExecuting)
        {
            Tactic.ExecutionProgress += DeltaTime / Tactic.ExecutionTime;
            
            if (Tactic.ExecutionProgress >= 1.0f)
            {
                CompleteTactic(TacticName, FMath::FRand() < Tactic.SuccessChance);
            }
            break;
        }
    }
}

bool UMingStrategyAuthority::CanExecuteStrategy(EStrategyType StrategyType, float RequiredPower) const
{
    if (!bIsInitialized || CurrentStrategyStatus.StrategicThinking < RequiredPower)
    {
        return false;
    }

    if (!HasStrategyType(StrategyType))
    {
        return false;
    }

    return ActiveTactics.Num() < MaxActiveStrategies;
}

TArray<FString> UMingStrategyAuthority::GetAvailableStrategies(EStrategyType StrategyType) const
{
    if (const TArray<FString>* Strategies = StrategyTypeMap.Find(StrategyType))
    {
        return *Strategies;
    }
    return TArray<FString>();
}

bool UMingStrategyAuthority::CancelTactic(const FString& TacticName)
{
    if (!bIsInitialized)
    {
        return false;
    }

    for (int32 i = 0; i < ActiveTactics.Num(); ++i)
    {
        if (ActiveTactics[i].TacticName == TacticName)
        {
            ActiveTactics.RemoveAt(i);
            UE_LOG(LogTemp, Log, TEXT("Tactic cancelled: %s"), *TacticName);
            return true;
        }
    }
    
    return false;
}

TArray<FTacticPlan> UMingStrategyAuthority::GetActiveTactics() const
{
    return ActiveTactics;
}

void UMingStrategyAuthority::UpgradeStrategySkill(EStrategyType StrategyType, float Amount)
{
    if (!bIsInitialized)
    {
        return;
    }

    switch (StrategyType)
    {
    case EStrategyType::Offensive:
        CurrentStrategyStatus.PlanningAbility += Amount * 0.3f;
        CurrentStrategyStatus.Adaptability += Amount * 0.2f;
        break;
    case EStrategyType::Defensive:
        CurrentStrategyStatus.ResourceManagement += Amount * 0.4f;
        CurrentStrategyStatus.Foresight += Amount * 0.3f;
        break;
    case EStrategyType::Diplomatic:
        CurrentStrategyStatus.StrategicThinking += Amount * 0.4f;
        CurrentStrategyStatus.Adaptability += Amount * 0.3f;
        break;
    case EStrategyType::Economic:
        CurrentStrategyStatus.ResourceManagement += Amount * 0.5f;
        CurrentStrategyStatus.PlanningAbility += Amount * 0.2f;
        break;
    case EStrategyType::Intelligence:
        CurrentStrategyStatus.Foresight += Amount * 0.4f;
        CurrentStrategyStatus.StrategicThinking += Amount * 0.3f;
        break;
    default:
        break;
    }
    
    UpdateStrategyStatus();
    
    UE_LOG(LogTemp, Log, TEXT("Strategy skill upgraded for type: %d"), (int32)StrategyType);
}

float UMingStrategyAuthority::CalculateStrategySuccessChance(const FTacticPlan& Tactic) const
{
    float BaseChance = 0.5f;
    
    switch (Tactic.Complexity)
    {
    case ETacticComplexity::Simple:
        BaseChance += 0.3f;
        break;
    case ETacticComplexity::Moderate:
        BaseChance += 0.1f;
        break;
    case ETacticComplexity::Complex:
        BaseChance -= 0.1f;
        break;
    case ETacticComplexity::Master:
        BaseChance -= 0.2f;
        break;
    }
    
    float SkillBonus = CurrentStrategyStatus.StrategicThinking / 100.0f * 0.3f;
    float PlanningBonus = CurrentStrategyStatus.PlanningAbility / 100.0f * 0.2f;
    
    return FMath::Clamp(BaseChance + SkillBonus + PlanningBonus, 0.1f, 0.9f);
}

void UMingStrategyAuthority::ApplyStrategyModifier(const FString& ModifierName, float ModifierValue)
{
    if (!bIsInitialized)
    {
        return;
    }

    CurrentStrategyStatus.StrategyModifiers.Add(ModifierName, ModifierValue);
    UpdateStrategyStatus();
    
    UE_LOG(LogTemp, Log, TEXT("Strategy modifier applied: %s (%.2f)"), *ModifierName, ModifierValue);
}

void UMingStrategyAuthority::RemoveStrategyModifier(const FString& ModifierName)
{
    if (!bIsInitialized)
    {
        return;
    }

    CurrentStrategyStatus.StrategyModifiers.Remove(ModifierName);
    UpdateStrategyStatus();
    
    UE_LOG(LogTemp, Log, TEXT("Strategy modifier removed: %s"), *ModifierName);
}

bool UMingStrategyAuthority::HasStrategyType(EStrategyType StrategyType) const
{
    return CurrentStrategyStatus.MasteredStrategies.Contains(StrategyType);
}

void UMingStrategyAuthority::MasterStrategyType(EStrategyType StrategyType)
{
    if (!bIsInitialized || HasStrategyType(StrategyType))
    {
        return;
    }

    CurrentStrategyStatus.MasteredStrategies.Add(StrategyType);
    CurrentStrategyStatus.ActiveStrategies++;
    
    OnStrategyTypeMastered(StrategyType);
    
    UE_LOG(LogTemp, Log, TEXT("Strategy type mastered: %d"), (int32)StrategyType);
}

ETacticComplexity UMingStrategyAuthority::GetMaxComplexityLevel() const
{
    float AverageSkill = (CurrentStrategyStatus.StrategicThinking + 
                         CurrentStrategyStatus.PlanningAbility + 
                         CurrentStrategyStatus.Adaptability + 
                         CurrentStrategyStatus.Foresight) / 4.0f;
    
    if (AverageSkill >= 80.0f) return ETacticComplexity::Master;
    if (AverageSkill >= 65.0f) return ETacticComplexity::Complex;
    if (AverageSkill >= 45.0f) return ETacticComplexity::Moderate;
    return ETacticComplexity::Simple;
}

FString UMingStrategyAuthority::GetStrategyRecommendation() const
{
    if (CurrentStrategyStatus.StrategicThinking < 30.0f)
    {
        return TEXT("建議專注基礎戰略訓練，提升思維能力");
    }
    else if (CurrentStrategyStatus.PlanningAbility < 40.0f)
    {
        return TEXT("建議加強規劃能力，制定更詳細的戰術計畫");
    }
    else if (CurrentStrategyStatus.Adaptability < 35.0f)
    {
        return TEXT("建議提升適應性，學會應對突發狀況");
    }
    else if (CurrentStrategyStatus.Foresight < 40.0f)
    {
        return TEXT("建議培養遠見，提前預測戰局發展");
    }
    
    return TEXT("戰略能力均衡，可以執行複雜的戰術計畫");
}

void UMingStrategyAuthority::InitializeDefaultStrategies()
{
    StrategyTypeMap.Add(EStrategyType::Offensive, TArray<FString>{
        TEXT("閃電戰"), TEXT("包圍戰術"), TEXT("突襲攻擊"), TEXT("強攻突破"), TEXT("分割消滅")
    });
    
    StrategyTypeMap.Add(EStrategyType::Defensive, TArray<FString>{
        TEXT("防禦部署"), TEXT("誘敵深入"), TEXT("堅壁清野"), TEXT("反擊防禦"), TEXT("機動防禦")
    });
    
    StrategyTypeMap.Add(EStrategyType::Diplomatic, TArray<FString>{
        TEXT("外交談判"), TEXT("聯盟結交"), TEXT("離間計"), TEXT("威懾外交"), TEXT("和平協議")
    });
    
    StrategyTypeMap.Add(EStrategyType::Economic, TArray<FString>{
        TEXT("經濟制裁"), TEXT("資源控制"), TEXT("貿易戰"), TEXT("經濟援助"), TEXT("市場操控")
    });
    
    StrategyTypeMap.Add(EStrategyType::Intelligence, TArray<FString>{
        TEXT("情報收集"), TEXT("反間計"), TEXT("心理戰"), TEXT("假情報"), TEXT("網絡戰")
    });
}

void UMingStrategyAuthority::UpdateStrategyStatus()
{
    OnStrategyStatusChanged(CurrentStrategyStatus);
}

bool UMingStrategyAuthority::ValidateTacticPlan(const FTacticPlan& Tactic) const
{
    return !Tactic.TacticName.IsEmpty() && 
           Tactic.StrategyType != EStrategyType::None &&
           Tactic.ExecutionTime > 0.0f &&
           Tactic.ResourceCost >= 0.0f;
}

void UMingStrategyAuthority::CompleteTactic(const FString& TacticName, bool bSuccess)
{
    for (int32 i = 0; i < ActiveTactics.Num(); ++i)
    {
        if (ActiveTactics[i].TacticName == TacticName)
        {
            ActiveTactics.RemoveAt(i);
            OnTacticCompleted.Broadcast(TacticName, bSuccess);
            
            if (bSuccess)
            {
                ExecutedStrategies.Add(TacticName);
                CurrentStrategyStatus.StrategicThinking += 1.0f;
            }
            
            UE_LOG(LogTemp, Log, TEXT("Tactic completed: %s (Success: %s)"), *TacticName, bSuccess ? TEXT("Yes") : TEXT("No"));
            break;
        }
    }
}
