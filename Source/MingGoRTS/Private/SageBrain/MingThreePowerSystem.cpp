#include "SageBrain/MingThreePowerSystem.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

UMingThreePowerSystem::UMingThreePowerSystem()
{
    bIsInitialized = false;
    PowerRestoreRate = 1.0f;
    PowerTransferEfficiency = 0.8f;
    BalanceThreshold = 0.2f;
    MaxPowerValue = 100.0f;
    
    // 初始化默認權力行動
    InitializeDefaultPowerActions();
}

void UMingThreePowerSystem::InitializeThreePowerSystem()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Three Power System already initialized"));
        return;
    }

    // 初始化三權平衡狀態
    CurrentBalance = FThreePowerBalance();
    
    // 設置初始權力值
    CurrentBalance.MoralAuthority.CurrentPower = 50.0f;
    CurrentBalance.StrategyAuthority.CurrentPower = 50.0f;
    CurrentBalance.MilitaryAuthority.CurrentPower = 50.0f;
    
    // 計算初始平衡
    CalculateBalance();
    
    bIsInitialized = true;
    
    // 觸發初始化事件
    OnPowerSystemInitialized();
    
    UE_LOG(LogTemp, Log, TEXT("Three Power System initialized successfully"));
}

void UMingThreePowerSystem::SetPowerLevel(EPowerType PowerType, float PowerLevel)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Three Power System not initialized"));
        return;
    }

    FPowerStatus* PowerStatus = GetPowerStatusRef(PowerType);
    if (!PowerStatus)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid power type specified"));
        return;
    }

    float OldPower = PowerStatus->CurrentPower;
    PowerStatus->CurrentPower = FMath::Clamp(PowerLevel, 0.0f, MaxPowerValue);
    
    // 更新權力效率
    UpdatePowerEfficiency(PowerType);
    
    // 重新計算平衡
    CalculateBalance();
    
    // 觸發狀態更改事件
    OnPowerStatusChanged(PowerType, *PowerStatus);
    
    UE_LOG(LogTemp, Log, TEXT("Power %s changed from %.2f to %.2f"), 
           *UEnum::GetValueAsString(PowerType), OldPower, PowerStatus->CurrentPower);
}

FPowerStatus UMingThreePowerSystem::GetPowerStatus(EPowerType PowerType) const
{
    const FPowerStatus* PowerStatus = GetPowerStatusRef(PowerType);
    if (PowerStatus)
    {
        return *PowerStatus;
    }
    
    return FPowerStatus();
}

bool UMingThreePowerSystem::TransferPower(EPowerType FromPower, EPowerType ToPower, float Amount)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Three Power System not initialized"));
        return false;
    }

    if (FromPower == ToPower)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot transfer power to same type"));
        return false;
    }

    FPowerStatus* FromStatus = GetPowerStatusRef(FromPower);
    FPowerStatus* ToStatus = GetPowerStatusRef(ToPower);
    
    if (!FromStatus || !ToStatus)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid power types for transfer"));
        return false;
    }

    // 檢查源權力是否足夠
    if (FromStatus->CurrentPower < Amount)
    {
        UE_LOG(LogTemp, Warning, TEXT("Insufficient power for transfer"));
        return false;
    }

    // 應用轉移效率
    float TransferAmount = Amount * PowerTransferEfficiency;
    
    // 執行轉移
    FromStatus->CurrentPower -= Amount;
    ToStatus->CurrentPower = FMath::Clamp(ToStatus->CurrentPower + TransferAmount, 0.0f, MaxPowerValue);
    
    // 更新權力效率
    UpdatePowerEfficiency(FromPower);
    UpdatePowerEfficiency(ToPower);
    
    // 重新計算平衡
    CalculateBalance();
    
    // 觸發轉移事件
    OnPowerTransferred.Broadcast(FromPower, ToPower, TransferAmount);
    
    UE_LOG(LogTemp, Log, TEXT("Transferred %.2f power from %s to %s"), 
           TransferAmount, *UEnum::GetValueAsString(FromPower), *UEnum::GetValueAsString(ToPower));
    
    return true;
}

bool UMingThreePowerSystem::ConsumePower(EPowerType PowerType, float Amount)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Three Power System not initialized"));
        return false;
    }

    FPowerStatus* PowerStatus = GetPowerStatusRef(PowerType);
    if (!PowerStatus)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid power type"));
        return false;
    }

    if (PowerStatus->CurrentPower < Amount)
    {
        UE_LOG(LogTemp, Warning, TEXT("Insufficient power to consume"));
        return false;
    }

    PowerStatus->CurrentPower -= Amount;
    
    // 更新權力效率
    UpdatePowerEfficiency(PowerType);
    
    // 重新計算平衡
    CalculateBalance();
    
    UE_LOG(LogTemp, Log, TEXT("Consumed %.2f power from %s"), 
           Amount, *UEnum::GetValueAsString(PowerType));
    
    return true;
}

void UMingThreePowerSystem::RestorePower(EPowerType PowerType, float Amount)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Three Power System not initialized"));
        return;
    }

    FPowerStatus* PowerStatus = GetPowerStatusRef(PowerType);
    if (!PowerStatus)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid power type"));
        return;
    }

    float OldPower = PowerStatus->CurrentPower;
    PowerStatus->CurrentPower = FMath::Clamp(PowerStatus->CurrentPower + Amount, 0.0f, MaxPowerValue);
    
    // 更新權力效率
    UpdatePowerEfficiency(PowerType);
    
    // 重新計算平衡
    CalculateBalance();
    
    UE_LOG(LogTemp, Log, TEXT("Restored %.2f power to %s (from %.2f to %.2f)"), 
           Amount, *UEnum::GetValueAsString(PowerType), OldPower, PowerStatus->CurrentPower);
}

void UMingThreePowerSystem::CheckPowerBalance()
{
    if (!bIsInitialized)
    {
        return;
    }

    // 重新計算平衡
    CalculateBalance();
    
    // 檢測衝突
    DetectPowerConflicts();
    
    // 觸發平衡更改事件
    bool bIsStable = GetSystemStability() > 0.7f;
    OnPowerBalanceChanged.Broadcast(CurrentBalance, bIsStable);
    
    UE_LOG(LogTemp, Log, TEXT("Power balance checked. Stability: %.2f, Balanced: %s"), 
           GetSystemStability(), CurrentBalance.bIsBalanced ? TEXT("Yes") : TEXT("No"));
}

float UMingThreePowerSystem::CalculatePowerEfficiency(EPowerType PowerType) const
{
    const FPowerStatus* PowerStatus = GetPowerStatusRef(PowerType);
    if (!PowerStatus)
    {
        return 0.0f;
    }

    // 效率基於當前權力與最大權力的比例
    float PowerRatio = PowerStatus->CurrentPower / MaxPowerValue;
    
    // 應用修飾符
    float Efficiency = PowerRatio;
    for (const auto& Modifier : PowerStatus->PowerModifiers)
    {
        Efficiency *= Modifier.Value;
    }
    
    return FMath::Clamp(Efficiency, 0.0f, 2.0f);
}

EPowerType UMingThreePowerSystem::GetDominantPower() const
{
    if (!bIsInitialized)
    {
        return EPowerType::None;
    }

    float MoralPower = CurrentBalance.MoralAuthority.CurrentPower;
    float StrategyPower = CurrentBalance.StrategyAuthority.CurrentPower;
    float MilitaryPower = CurrentBalance.MilitaryAuthority.CurrentPower;
    
    if (MoralPower >= StrategyPower && MoralPower >= MilitaryPower)
    {
        return EPowerType::MoralAuthority;
    }
    else if (StrategyPower >= MilitaryPower)
    {
        return EPowerType::StrategyAuthority;
    }
    else
    {
        return EPowerType::MilitaryAuthority;
    }
}

bool UMingThreePowerSystem::CanExecutePowerAction(EPowerType PowerType, float RequiredPower) const
{
    const FPowerStatus* PowerStatus = GetPowerStatusRef(PowerType);
    if (!PowerStatus)
    {
        return false;
    }

    return PowerStatus->CurrentPower >= RequiredPower;
}

bool UMingThreePowerSystem::ExecutePowerAction(EPowerType PowerType, const FString& ActionName, float PowerCost)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Three Power System not initialized"));
        return false;
    }

    // 檢查是否可以執行
    if (!CanExecutePowerAction(PowerType, PowerCost))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot execute power action: insufficient power"));
        OnPowerActionExecuted(PowerType, ActionName, false);
        return false;
    }

    // 檢查行動是否可用
    TArray<FString> AvailableActions = GetAvailablePowerActions(PowerType);
    if (!AvailableActions.Contains(ActionName))
    {
        UE_LOG(LogTemp, Warning, TEXT("Power action not available: %s"), *ActionName);
        OnPowerActionExecuted(PowerType, ActionName, false);
        return false;
    }

    // 消耗權力
    if (ConsumePower(PowerType, PowerCost))
    {
        UE_LOG(LogTemp, Log, TEXT("Executed power action: %s"), *ActionName);
        OnPowerActionExecuted(PowerType, ActionName, true);
        return true;
    }

    OnPowerActionExecuted(PowerType, ActionName, false);
    return false;
}

TArray<FString> UMingThreePowerSystem::GetAvailablePowerActions(EPowerType PowerType) const
{
    if (const TArray<FString>* Actions = PowerActionsMap.Find(PowerType))
    {
        return *Actions;
    }
    
    return TArray<FString>();
}

bool UMingThreePowerSystem::UpgradePowerLevel(EPowerType PowerType)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Three Power System not initialized"));
        return false;
    }

    FPowerStatus* PowerStatus = GetPowerStatusRef(PowerType);
    if (!PowerStatus)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid power type"));
        return false;
    }

    // 檢查升級條件
    if (PowerStatus->CurrentPower < MaxPowerValue * 0.8f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Insufficient power for upgrade"));
        return false;
    }

    // 升級
    PowerStatus->ControlLevel++;
    PowerStatus->MaxPower += 20.0f;
    
    // 觸發升級事件
    OnPowerLevelUp.Broadcast(PowerType);
    
    UE_LOG(LogTemp, Log, TEXT("Power %s upgraded to level %d"), 
           *UEnum::GetValueAsString(PowerType), PowerStatus->ControlLevel);
    
    return true;
}

void UMingThreePowerSystem::ApplyPowerModifier(EPowerType PowerType, const FString& ModifierName, float ModifierValue)
{
    FPowerStatus* PowerStatus = GetPowerStatusRef(PowerType);
    if (!PowerStatus)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid power type"));
        return;
    }

    PowerStatus->PowerModifiers.Add(ModifierName, ModifierValue);
    UpdatePowerEfficiency(PowerType);
    
    UE_LOG(LogTemp, Log, TEXT("Applied power modifier %s to %s with value %.2f"), 
           *ModifierName, *UEnum::GetValueAsString(PowerType), ModifierValue);
}

void UMingThreePowerSystem::RemovePowerModifier(EPowerType PowerType, const FString& ModifierName)
{
    FPowerStatus* PowerStatus = GetPowerStatusRef(PowerType);
    if (!PowerStatus)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid power type"));
        return;
    }

    if (PowerStatus->PowerModifiers.Remove(ModifierName) > 0)
    {
        UpdatePowerEfficiency(PowerType);
        UE_LOG(LogTemp, Log, TEXT("Removed power modifier %s from %s"), 
               *ModifierName, *UEnum::GetValueAsString(PowerType));
    }
}

TArray<EPowerType> UMingThreePowerSystem::GetConflictingPowers() const
{
    return CurrentBalance.ConflictingPowers;
}

void UMingThreePowerSystem::ResolvePowerConflicts()
{
    if (!bIsInitialized)
    {
        return;
    }

    if (CurrentBalance.ConflictingPowers.Num() == 0)
    {
        return; // 沒有衝突
    }

    // 解決衝突：平衡權力分配
    for (EPowerType PowerType : CurrentBalance.ConflictingPowers)
    {
        FPowerStatus* PowerStatus = GetPowerStatusRef(PowerType);
        if (PowerStatus)
        {
            // 將權力調整到平衡水平
            float TargetPower = MaxPowerValue * 0.5f;
            PowerStatus->CurrentPower = FMath::Lerp(PowerStatus->CurrentPower, TargetPower, 0.3f);
            UpdatePowerEfficiency(PowerType);
        }
    }

    // 重新計算平衡
    CalculateBalance();
    
    UE_LOG(LogTemp, Log, TEXT("Power conflicts resolved"));
}

void UMingThreePowerSystem::ResetPowerSystem()
{
    if (!bIsInitialized)
    {
        return;
    }

    // 重置所有權力到初始狀態
    CurrentBalance.MoralAuthority = FPowerStatus();
    CurrentBalance.MoralAuthority.PowerType = EPowerType::MoralAuthority;
    CurrentBalance.StrategyAuthority = FPowerStatus();
    CurrentBalance.StrategyAuthority.PowerType = EPowerType::StrategyAuthority;
    CurrentBalance.MilitaryAuthority = FPowerStatus();
    CurrentBalance.MilitaryAuthority.PowerType = EPowerType::MilitaryAuthority;
    
    // 重新計算平衡
    CalculateBalance();
    
    UE_LOG(LogTemp, Log, TEXT("Three Power System reset"));
}

float UMingThreePowerSystem::GetSystemStability() const
{
    if (!bIsInitialized)
    {
        return 0.0f;
    }

    // 穩定性基於權力平衡程度
    float MoralPower = CurrentBalance.MoralAuthority.CurrentPower;
    float StrategyPower = CurrentBalance.StrategyAuthority.CurrentPower;
    float MilitaryPower = CurrentBalance.MilitaryAuthority.CurrentPower;
    
    float TotalPower = MoralPower + StrategyPower + MilitaryPower;
    if (TotalPower <= 0.0f)
    {
        return 0.0f;
    }
    
    // 計算權力分配的標準差
    float MeanPower = TotalPower / 3.0f;
    float Variance = FMath::Square(MoralPower - MeanPower) + 
                     FMath::Square(StrategyPower - MeanPower) + 
                     FMath::Square(MilitaryPower - MeanPower);
    float StdDev = FMath::Sqrt(Variance / 3.0f);
    
    // 穩定性 = 1 - (標準差 / 平均值)
    float Stability = 1.0f - (StdDev / MeanPower);
    return FMath::Clamp(Stability, 0.0f, 1.0f);
}

void UMingThreePowerSystem::CalculateBalance()
{
    if (!bIsInitialized)
    {
        return;
    }

    float MoralPower = CurrentBalance.MoralAuthority.CurrentPower;
    float StrategyPower = CurrentBalance.StrategyAuthority.CurrentPower;
    float MilitaryPower = CurrentBalance.MilitaryAuthority.CurrentPower;
    
    float TotalPower = MoralPower + StrategyPower + MilitaryPower;
    if (TotalPower <= 0.0f)
    {
        CurrentBalance.OverallBalance = 0.0f;
        CurrentBalance.bIsBalanced = false;
        return;
    }
    
    // 計算主導權力比例
    float MaxPower = FMath::Max3(MoralPower, StrategyPower, MilitaryPower);
    CurrentBalance.DominantPowerRatio = MaxPower / TotalPower;
    
    // 計算整體平衡度
    float MeanPower = TotalPower / 3.0f;
    float Deviation = FMath::Abs(MoralPower - MeanPower) + 
                      FMath::Abs(StrategyPower - MeanPower) + 
                      FMath::Abs(MilitaryPower - MeanPower);
    
    CurrentBalance.OverallBalance = 1.0f - (Deviation / TotalPower);
    CurrentBalance.bIsBalanced = Deviation < (BalanceThreshold * TotalPower);
    
    // 更新權力狀態
    CurrentBalance.MoralAuthority.PowerEfficiency = CalculatePowerEfficiency(EPowerType::MoralAuthority);
    CurrentBalance.StrategyAuthority.PowerEfficiency = CalculatePowerEfficiency(EPowerType::StrategyAuthority);
    CurrentBalance.MilitaryAuthority.PowerEfficiency = CalculatePowerEfficiency(EPowerType::MilitaryAuthority);
}

void UMingThreePowerSystem::DetectPowerConflicts()
{
    if (!bIsInitialized)
    {
        return;
    }

    CurrentBalance.ConflictingPowers.Empty();
    
    float MoralPower = CurrentBalance.MoralAuthority.CurrentPower;
    float StrategyPower = CurrentBalance.StrategyAuthority.CurrentPower;
    float MilitaryPower = CurrentBalance.MilitaryAuthority.CurrentPower;
    
    float TotalPower = MoralPower + StrategyPower + MilitaryPower;
    if (TotalPower <= 0.0f)
    {
        return;
    }
    
    // 檢測權力不平衡
    float MeanPower = TotalPower / 3.0f;
    
    if (FMath::Abs(MoralPower - MeanPower) > BalanceThreshold * TotalPower)
    {
        CurrentBalance.ConflictingPowers.Add(EPowerType::MoralAuthority);
    }
    
    if (FMath::Abs(StrategyPower - MeanPower) > BalanceThreshold * TotalPower)
    {
        CurrentBalance.ConflictingPowers.Add(EPowerType::StrategyAuthority);
    }
    
    if (FMath::Abs(MilitaryPower - MeanPower) > BalanceThreshold * TotalPower)
    {
        CurrentBalance.ConflictingPowers.Add(EPowerType::MilitaryAuthority);
    }
    
    // 如果檢測到衝突，觸發事件
    if (CurrentBalance.ConflictingPowers.Num() > 0)
    {
        OnPowerConflictDetected(CurrentBalance.ConflictingPowers);
    }
}

void UMingThreePowerSystem::UpdatePowerEfficiency(EPowerType PowerType)
{
    FPowerStatus* PowerStatus = GetPowerStatusRef(PowerType);
    if (!PowerStatus)
    {
        return;
    }

    PowerStatus->PowerEfficiency = CalculatePowerEfficiency(PowerType);
}

void UMingThreePowerSystem::InitializeDefaultPowerActions()
{
    // 道權行動
    PowerActionsMap.Add(EPowerType::MoralAuthority, TArray<FString>{
        TEXT("道德感召"), TEXT("正義審判"), TEXT("民心歸向"), TEXT("道德淨化"), TEXT("聖光加持")
    });
    
    // 策權行動
    PowerActionsMap.Add(EPowerType::StrategyAuthority, TArray<FString>{
        TEXT("戰略規劃"), TEXT("謀略執行"), TEXT("情報收集"), TEXT("外交談判"), TEXT("陰謀策劃")
    });
    
    // 兵權行動
    PowerActionsMap.Add(EPowerType::MilitaryAuthority, TArray<FString>{
        TEXT("軍事指揮"), TEXT("部隊調動"), TEXT("戰術執行"), TEXT("防禦部署"), TEXT("攻擊命令")
    });
    
    // 設置行動成本
    PowerActionCosts.Add(TEXT("道德感召"), 20.0f);
    PowerActionCosts.Add(TEXT("正義審判"), 30.0f);
    PowerActionCosts.Add(TEXT("民心歸向"), 25.0f);
    PowerActionCosts.Add(TEXT("道德淨化"), 15.0f);
    PowerActionCosts.Add(TEXT("聖光加持"), 35.0f);
    
    PowerActionCosts.Add(TEXT("戰略規劃"), 15.0f);
    PowerActionCosts.Add(TEXT("謀略執行"), 25.0f);
    PowerActionCosts.Add(TEXT("情報收集"), 10.0f);
    PowerActionCosts.Add(TEXT("外交談判"), 20.0f);
    PowerActionCosts.Add(TEXT("陰謀策劃"), 30.0f);
    
    PowerActionCosts.Add(TEXT("軍事指揮"), 20.0f);
    PowerActionCosts.Add(TEXT("部隊調動"), 15.0f);
    PowerActionCosts.Add(TEXT("戰術執行"), 25.0f);
    PowerActionCosts.Add(TEXT("防禦部署"), 20.0f);
    PowerActionCosts.Add(TEXT("攻擊命令"), 30.0f);
}

FPowerStatus* UMingThreePowerSystem::GetPowerStatusRef(EPowerType PowerType)
{
    switch (PowerType)
    {
        case EPowerType::MoralAuthority:
            return &CurrentBalance.MoralAuthority;
        case EPowerType::StrategyAuthority:
            return &CurrentBalance.StrategyAuthority;
        case EPowerType::MilitaryAuthority:
            return &CurrentBalance.MilitaryAuthority;
        default:
            return nullptr;
    }
}

const FPowerStatus* UMingThreePowerSystem::GetPowerStatusRef(EPowerType PowerType) const
{
    switch (PowerType)
    {
        case EPowerType::MoralAuthority:
            return &CurrentBalance.MoralAuthority;
        case EPowerType::StrategyAuthority:
            return &CurrentBalance.StrategyAuthority;
        case EPowerType::MilitaryAuthority:
            return &CurrentBalance.MilitaryAuthority;
        default:
            return nullptr;
    }
}
