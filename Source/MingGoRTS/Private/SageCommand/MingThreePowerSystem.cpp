// Copyright Epic Games, Inc. All Rights Reserved.

#include "MingThreePowerSystem.h"
#include "MingMoralAuthority.h"
#include "MingStrategyAuthority.h"
#include "MingMilitaryAuthority.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"

UMingThreePowerSystem::UMingThreePowerSystem()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // 初始化狀態
    bSystemInitialized = false;
    bAutoBalancingActive = false;
    
    // g默認權力分配
    CurrentDistribution.ActiveAuthority = ESupremeAuthorityType::DaoAuthority;
    CurrentDistribution.DaoAuthorityPower = 40.0f;
    CurrentDistribution.StrategyAuthorityPower = 35.0f;
    CurrentDistribution.MilitaryAuthorityPower = 25.0f;
    CurrentDistribution.TotalBalanceIndex = 0.0f;
}

void UMingThreePowerSystem::InitializeThreePowerSystem()
{
    if (bIsInitialized)
    {
        return;
    }

    // 初始化默認狀態
    CurrentStatus = FThreePowerStats();
    CurrentDistribution = FPowerDistribution();

    // 初始化子系統
    if (!MoralAuthority)
    {
        MoralAuthority = NewObject<UMingMoralAuthority>(this);
        MoralAuthority->InitializeMoralAuthority();
    }

    if (!StrategyAuthority)
    {
        StrategyAuthority = NewObject<UMingStrategyAuthority>(this);
        StrategyAuthority->InitializeStrategyAuthority();
    }

    if (!MilitaryAuthority)
    {
        MilitaryAuthority = NewObject<UMingMilitaryAuthority>(this);
        MilitaryAuthority->InitializeMilitaryAuthority();
    }

    bIsInitialized = true;
}

void UMingThreePowerSystem::SetPowerAuthorities(UMingMoralAuthority* MoralAuth, 
                                                 UMingStrategyAuthority* StrategyAuth, 
                                                 UMingMilitaryAuthority* MilitaryAuth)
{
    MoralAuthority = MoralAuth;
    StrategyAuthority = StrategyAuth;
    MilitaryAuthority = MilitaryAuth;

    // 初始化各子系統
    if (MoralAuthority)
    {
        MoralAuthority->InitializeMoralAuthority();
    }

    if (StrategyAuthority)
    {
        StrategyAuthority->InitializeStrategyAuthority();
    }

    if (MilitaryAuthority)
    {
        MilitaryAuthority->InitializeMilitaryAuthority();
    }
}

bool UMingThreePowerSystem::CoordinatePowers()
{
    if (!bIsInitialized)
    {
        return false;
    }

    // 檢查權力衝突
    if (CheckPowerConflicts())
    {
        ResolvePowerConflicts();
    }

    // 計算新N平衡值
    CurrentStatus.PowerBalanceValue = CalculateBalanceValue();
    CurrentStatus.LastCoordinationTime = FDateTime::Now();

    // 檢查是否平衡
    if (!IsPowerBalanced())
    {
        OnPowerImbalance.Broadcast(CurrentStatus);
        
        // 如果啟用動態調整，自動調整分配
        if (CurrentDistribution.bDynamicAdjustment)
        {
            AutoAdjustDistribution();
        }
    }
    else
    {
        OnPowerCoordinated.Broadcast();
    }

    return IsPowerBalanced();
}

void UMingThreePowerSystem::SetPowerDistribution(const FPowerDistribution& Distribution)
{
    // 驗證分配比例總和為1.0
    float TotalShare = Distribution.MoralPowerShare + 
                       Distribution.StrategyPowerShare + 
                       Distribution.MilitaryPowerShare;
    
    if (FMath::Abs(TotalShare - 1.0f) > KINDA_SMALL_NUMBER)
    {
        // 如果不等於1.0，進行歸一化
        CurrentDistribution.MoralPowerShare = Distribution.MoralPowerShare / TotalShare;
        CurrentDistribution.StrategyPowerShare = Distribution.StrategyPowerShare / TotalShare;
        CurrentDistribution.MilitaryPowerShare = Distribution.MilitaryPowerShare / TotalShare;
    }
    else
    {
        CurrentDistribution = Distribution;
    }
}

bool UMingThreePowerSystem::IsPowerBalanced() const
{
    return CurrentStatus.PowerBalanceValue >= BalanceThreshold;
}

TArray<EThreePowerType> UMingThreePowerSystem::GetUnbalancedPowers() const
{
    TArray<EThreePowerType> UnbalancedPowers;

    // 檢查各權力N活躍狀態
    if (!CurrentStatus.bMoralAuthorityActive)
    {
        UnbalancedPowers.Add(EThreePowerType::Moral);
    }

    if (!CurrentStatus.bStrategyAuthorityActive)
    {
        UnbalancedPowers.Add(EThreePowerType::Strategy);
    }

    if (!CurrentStatus.bMilitaryAuthorityActive)
    {
        UnbalancedPowers.Add(EThreePowerType::Military);
    }

    return UnbalancedPowers;
}

void UMingThreePowerSystem::SetPowerActive(EThreePowerType PowerType, bool bActive)
{
    switch (PowerType)
    {
    case EThreePowerType::Moral:
        CurrentStatus.bMoralAuthorityActive = bActive;
        break;
    case EThreePowerType::Strategy:
        CurrentStatus.bStrategyAuthorityActive = bActive;
        break;
    case EThreePowerType::Military:
        CurrentStatus.bMilitaryAuthorityActive = bActive;
        break;
    default:
        return;
    }

    OnPowerChanged.Broadcast(PowerType, bActive);
}

FString UMingThreePowerSystem::GetPowerDisplayName(EThreePowerType PowerType) const
{
    switch (PowerType)
    {
    case EThreePowerType::Moral:
        return TEXT("道權");
    case EThreePowerType::Strategy:
        return TEXT("策權");
    case EThreePowerType::Military:
        return TEXT("兵權");
    default:
        return TEXT("未知");
    }
}

FString UMingThreePowerSystem::GetPowerDescription(EThreePowerType PowerType) const
{
    switch (PowerType)
    {
    case EThreePowerType::Moral:
        return TEXT("道權掌天道、掌j義、掌不傳之秘。監測墮落徵象，確保指揮者不墮入魔道。");
    case EThreePowerType::Strategy:
        return TEXT("策權掌v逆、掌陰陽、掌五行節奏。決定何時i用v道，何時i用逆術。");
    case EThreePowerType::Military:
        return TEXT("兵權掌執行、掌表象、掌眾目之_。在白日之_發號施令，承擔後果。");
    default:
        return TEXT("未知權力類型");
    }
}

bool UMingThreePowerSystem::CheckMoralAuthority() const
{
    if (!MoralAuthority || !CurrentStatus.bMoralAuthorityActive)
    {
        return false;
    }

    return MoralAuthority->PerformMoralCheck();
}

bool UMingThreePowerSystem::ExecuteStrategyDecision() const
{
    if (!StrategyAuthority || !CurrentStatus.bStrategyAuthorityActive)
    {
        return false;
    }

    return StrategyAuthority->ExecuteStrategySwitch();
}

bool UMingThreePowerSystem::ExecuteMilitaryCommand() const
{
    if (!MilitaryAuthority || !CurrentStatus.bMilitaryAuthorityActive)
    {
        return false;
    }

    return MilitaryAuthority->ExecuteCommand();
}

int32 UMingThreePowerSystem::CalculateBalanceValue() const
{
    int32 BalanceScore = 100;

    // 根據各權力N活躍狀態和協調程度計算平衡值
    if (!CurrentStatus.bMoralAuthorityActive)
    {
        BalanceScore -= 20;
    }

    if (!CurrentStatus.bStrategyAuthorityActive)
    {
        BalanceScore -= 30;
    }

    if (!CurrentStatus.bMilitaryAuthorityActive)
    {
        BalanceScore -= 25;
    }

    // 檢查各子系統N健康狀況
    if (MoralAuthority && !MoralAuthority->IsHealthy())
    {
        BalanceScore -= 10;
    }

    if (StrategyAuthority && !StrategyAuthority->IsHealthy())
    {
        BalanceScore -= 10;
    }

    if (MilitaryAuthority && !MilitaryAuthority->IsHealthy())
    {
        BalanceScore -= 10;
    }

    return FMath::Clamp(BalanceScore, 0, 100);
}

void UMingThreePowerSystem::AutoAdjustDistribution()
{
    // 根據當前狀態自動調整權力分配
    int32 ActiveCount = 0;
    if (CurrentStatus.bMoralAuthorityActive) ActiveCount++;
    if (CurrentStatus.bStrategyAuthorityActive) ActiveCount++;
    if (CurrentStatus.bMilitaryAuthorityActive) ActiveCount++;

    if (ActiveCount == 0)
    {
        // 如果都未激活，均分
        CurrentDistribution.MoralPowerShare = 0.33f;
        CurrentDistribution.StrategyPowerShare = 0.33f;
        CurrentDistribution.MilitaryPowerShare = 0.34f;
    }
    else
    {
        // 根據活躍狀態調整
        float SharePerActive = 1.0f / ActiveCount;
        
        CurrentDistribution.MoralPowerShare = CurrentStatus.bMoralAuthorityActive ? SharePerActive : 0.0f;
        CurrentDistribution.StrategyPowerShare = CurrentStatus.bStrategyAuthorityActive ? SharePerActive : 0.0f;
        CurrentDistribution.MilitaryPowerShare = CurrentStatus.bMilitaryAuthorityActive ? SharePerActive : 0.0f;
    }
}

bool UMingThreePowerSystem::CheckPowerConflicts() const
{
    // 檢查各權力之間是否存在衝突
    // 例如：策權選擇逆策，但道權監測到墮落風險
    
    if (StrategyAuthority && MoralAuthority)
    {
        if (StrategyAuthority->IsUsingEvilStrategy() && MoralAuthority->IsFallRiskHigh())
        {
            return true;
        }
    }

    return false;
}

void UMingThreePowerSystem::ResolvePowerConflicts()
{
    // 解決權力衝突N邏輯
    // 通常道權優先，因為防墮是最重要N
    
    if (StrategyAuthority && MoralAuthority)
    {
        if (StrategyAuthority->IsUsingEvilStrategy() && MoralAuthority->IsFallRiskHigh())
        {
            // 暫時禁用逆策，直到墮落風險降低
            StrategyAuthority->ForceSwitchToRighteous();
        }
    }
}
