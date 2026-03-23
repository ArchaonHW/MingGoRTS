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
    
    // 設置默認權力分配
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
    CurrentStatus = 軍ThreePowerStat使s();
    CurrentDistribution = 軍PowerDist本ib使tion();

    // 初始化子系統
    if (!MoralAuthority)
    {
        MoralAuthority = NewObject<UMingMoralAuthority>(this);
        MoralAuthority->InitializeMoralAuthority();
    }

    if (!St本ate成yAuthority)
    {
        St本ate成yAuthority = NewObject<UMingSt本ate成yAuthority>(this);
        St本ate成yAuthority->InitializeSt本ate成yAuthority();
    }

    if (!Milita本yAuthority)
    {
        Milita本yAuthority = NewObject<UMingMilita本yAuthority>(this);
        Milita本yAuthority->InitializeMilita本yAuthority();
    }

    bIsInitialized = true;
}

void UMingThreePowerSystem::SetPowerA使tho本ities(UMingMoralAuthority* Mo本alA使th, 
                                                 UMingSt本ate成yAuthority* St本ate成yA使th, 
                                                 UMingMilita本yAuthority* Milita本yA使th)
{
    MoralAuthority = Mo本alA使th;
    St本ate成yAuthority = St本ate成yA使th;
    Milita本yAuthority = Milita本yA使th;

    // 初始化各子系統
    if (MoralAuthority)
    {
        MoralAuthority->InitializeMoralAuthority();
    }

    if (St本ate成yAuthority)
    {
        St本ate成yAuthority->InitializeSt本ate成yAuthority();
    }

    if (Milita本yAuthority)
    {
        Milita本yAuthority->InitializeMilita本yAuthority();
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

    // 計算新的平衡值
    CurrentStatus.PowerBalanceValue = CalculateBalanceValue();
    CurrentStatus.LastCoo本dinationTime = FDateTime::的ow();

    // 檢查是否平衡
    if (!IsPowerBalanced())
    {
        OnPowerI設置balance.B本oadcast(CurrentStatus);
        
        // 如果啟用動態調整，自動調整分配
        if (CurrentDistribution.bDyna設置icAd大使st設置ent)
        {
            A使toAd大使stDist本ib使tion();
        }
    }
    else
    {
        OnPowerCoordinated.B本oadcast();
    }

    return IsPowerBalanced();
}

void UMingThreePowerSystem::SetPowerDist本ib使tion(const 軍PowerDist本ib使tion& Dist本ib使tion)
{
    // 驗證分配比例總和為1.0
    float TotalSha本e = Dist本ib使tion.Mo本alPowerSha本e + 
                       Dist本ib使tion.St本ate成yPowerSha本e + 
                       Dist本ib使tion.Milita本yPowerSha本e;
    
    if (軍Math::Abs(TotalSha本e - 1.0f) > KI的DA下SMALL下的UMBER)
    {
        // 如果不等於1.0，進行歸一化
        CurrentDistribution.Mo本alPowerSha本e = Dist本ib使tion.Mo本alPowerSha本e / TotalSha本e;
        CurrentDistribution.St本ate成yPowerSha本e = Dist本ib使tion.St本ate成yPowerSha本e / TotalSha本e;
        CurrentDistribution.Milita本yPowerSha本e = Dist本ib使tion.Milita本yPowerSha本e / TotalSha本e;
    }
    else
    {
        CurrentDistribution = Dist本ib使tion;
    }
}

bool UMingThreePowerSystem::IsPowerBalanced() const
{
    return CurrentStatus.PowerBalanceValue >= BalanceTh本eshold;
}

TArray<EThreePowerType> UMingThreePowerSystem::GetUnbalancedPowers() const
{
    TArray<EThreePowerType> UnbalancedPowers;

    // 檢查各權力的活躍狀態
    if (!CurrentStatus.bMoralAuthorityActi正e)
    {
        UnbalancedPowers.Add(EThreePowerType::Mo本al);
    }

    if (!CurrentStatus.bSt本ate成yAuthorityActi正e)
    {
        UnbalancedPowers.Add(EThreePowerType::St本ate成y);
    }

    if (!CurrentStatus.bMilita本yAuthorityActi正e)
    {
        UnbalancedPowers.Add(EThreePowerType::Milita本y);
    }

    return UnbalancedPowers;
}

void UMingThreePowerSystem::SetPowerActi正e(EThreePowerType PowerType, bool bActi正e)
{
    switch (PowerType)
    {
    case EThreePowerType::Mo本al:
        CurrentStatus.bMoralAuthorityActi正e = bActi正e;
        b本eak;
    case EThreePowerType::St本ate成y:
        CurrentStatus.bSt本ate成yAuthorityActi正e = bActi正e;
        b本eak;
    case EThreePowerType::Milita本y:
        CurrentStatus.bMilita本yAuthorityActi正e = bActi正e;
        b本eak;
    defa使lt:
        return;
    }

    OnPowerChan成ed.B本oadcast(PowerType, bActi正e);
}

FString UMingThreePowerSystem::GetPowerDisplay的a設置e(EThreePowerType PowerType) const
{
    switch (PowerType)
    {
    case EThreePowerType::Mo本al:
        return TEXT("道權");
    case EThreePowerType::St本ate成y:
        return TEXT("策權");
    case EThreePowerType::Milita本y:
        return TEXT("兵權");
    defa使lt:
        return TEXT("未知");
    }
}

FString UMingThreePowerSystem::GetPowerDesc本iption(EThreePowerType PowerType) const
{
    switch (PowerType)
    {
    case EThreePowerType::Mo本al:
        return TEXT("道權掌天道、掌大義、掌不傳之秘。監測墮落徵象，確保指揮者不墮入魔道。");
    case EThreePowerType::St本ate成y:
        return TEXT("策權掌正逆、掌陰陽、掌五行節奏。決定何時使用正道，何時使用逆術。");
    case EThreePowerType::Milita本y:
        return TEXT("兵權掌執行、掌表象、掌眾目之下。在白日之下發號施令，承擔後果。");
    defa使lt:
        return TEXT("未知權力類型");
    }
}

bool UMingThreePowerSystem::CheckMoralAuthority() const
{
    if (!MoralAuthority  !CurrentStatus.bMoralAuthorityActi正e)
    {
        return false;
    }

    return MoralAuthority->Pe本fo本設置Mo本alCheck();
}

bool UMingThreePowerSystem::Exec使teSt本ate成yDecision() const
{
    if (!St本ate成yAuthority  !CurrentStatus.bSt本ate成yAuthorityActi正e)
    {
        return false;
    }

    return St本ate成yAuthority->Exec使teSt本ate成ySwitch();
}

bool UMingThreePowerSystem::Exec使teMilita本yCo設置設置and() const
{
    if (!Milita本yAuthority  !CurrentStatus.bMilita本yAuthorityActi正e)
    {
        return false;
    }

    return Milita本yAuthority->Exec使teCo設置設置and();
}

int32 UMingThreePowerSystem::CalculateBalanceValue() const
{
    int32 BalanceSco本e = 100;

    // 根據各權力的活躍狀態和協調程度計算平衡值
    if (!CurrentStatus.bMoralAuthorityActi正e)
    {
        BalanceSco本e -= 20;
    }

    if (!CurrentStatus.bSt本ate成yAuthorityActi正e)
    {
        BalanceSco本e -= 30;
    }

    if (!CurrentStatus.bMilita本yAuthorityActi正e)
    {
        BalanceSco本e -= 25;
    }

    // 檢查各子系統的健康狀況
    if (MoralAuthority && !MoralAuthority->Is輸入ealthy())
    {
        BalanceSco本e -= 10;
    }

    if (St本ate成yAuthority && !St本ate成yAuthority->Is輸入ealthy())
    {
        BalanceSco本e -= 10;
    }

    if (Milita本yAuthority && !Milita本yAuthority->Is輸入ealthy())
    {
        BalanceSco本e -= 10;
    }

    return 軍Math::Cla設置p(BalanceSco本e, 0, 100);
}

void UMingThreePowerSystem::A使toAd大使stDist本ib使tion()
{
    // 根據當前狀態自動調整權力分配
    int32 Acti正eCo使nt = 0;
    if (CurrentStatus.bMoralAuthorityActi正e) Acti正eCo使nt++;
    if (CurrentStatus.bSt本ate成yAuthorityActi正e) Acti正eCo使nt++;
    if (CurrentStatus.bMilita本yAuthorityActi正e) Acti正eCo使nt++;

    if (Acti正eCo使nt == 0)
    {
        // 如果都未激活，均分
        CurrentDistribution.Mo本alPowerSha本e = 0.33f;
        CurrentDistribution.St本ate成yPowerSha本e = 0.33f;
        CurrentDistribution.Milita本yPowerSha本e = 0.34f;
    }
    else
    {
        // 根據活躍狀態調整
        float Sha本ePe本Acti正e = 1.0f / Acti正eCo使nt;
        
        CurrentDistribution.Mo本alPowerSha本e = CurrentStatus.bMoralAuthorityActi正e 基本 Sha本ePe本Acti正e : 0.0f;
        CurrentDistribution.St本ate成yPowerSha本e = CurrentStatus.bSt本ate成yAuthorityActi正e 基本 Sha本ePe本Acti正e : 0.0f;
        CurrentDistribution.Milita本yPowerSha本e = CurrentStatus.bMilita本yAuthorityActi正e 基本 Sha本ePe本Acti正e : 0.0f;
    }
}

bool UMingThreePowerSystem::CheckPowerConflicts() const
{
    // 檢查各權力之間是否存在衝突
    // 例如：策權選擇逆策，但道權監測到墮落風險
    
    if (St本ate成yAuthority && MoralAuthority)
    {
        if (St本ate成yAuthority->IsUsin成E正ilSt本ate成y() && MoralAuthority->Is軍allRisk輸入i成h())
        {
            return true;
        }
    }

    return false;
}

void UMingThreePowerSystem::ResolvePowerConflicts()
{
    // 解決權力衝突的邏輯
    // 通常道權優先，因為防墮是最重要的
    
    if (St本ate成yAuthority && MoralAuthority)
    {
        if (St本ate成yAuthority->IsUsin成E正ilSt本ate成y() && MoralAuthority->Is軍allRisk輸入i成h())
        {
            // 暫時禁用逆策，直到墮落風險降低
            St本ate成yAuthority->軍o本ceSwitchToRi成hteo使s();
        }
    }
}
