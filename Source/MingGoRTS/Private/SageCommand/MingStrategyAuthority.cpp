// Copyright Epic Games, Inc. All Rights Reserved.

#include "SageCommand/MingStrategyAuthority.h"

UMingStrategyAuthority::UMingStrategyAuthority()
    : CurrentDirection(EStrategyDirection::Righteous)
    , bIsInitialized(false)
    , ConsecutiveEvilUses(0)
    , ConsecutiveRighteousUses(0)
{
}

void UMingStrategyAuthority::InitializeStrategyAuthority()
{
    if (bIsInitialized)
    {
        return;
    }

    InitializeDefaultStrategies();

    bIsInitialized = true;
}

void UMingStrategyAuthority::InitializeDefaultStrategies()
{
    // 正六策初始化
    
    // 立國 (正) - 士氣+30%, 徵兵+20%
    {
        FStrategyData Data;
        Data.StrategyType = ESixStrategyType::EstablishNation;
        Data.Direction = EStrategyDirection::Righteous;
        Data.CooldownDuration = 0.0f; // 無冷卻
        Data.Duration = 0.0f; // 永久
        Data.EffectIntensity = 1.0f;
        Data.FallValueImpact = 0;
        StrategyMap.Add(ESixStrategyType::EstablishNation, Data);

        FStrategyEffect Effect;
        Effect.MoraleImpact = 0.30f;
        Effect.RecruitmentSpeedImpact = 0.20f;
        StrategyEffectMap.Add(ESixStrategyType::EstablishNation, Effect);
    }

    // 立制 (正) - 經濟效率+25%
    {
        FStrategyData Data;
        Data.StrategyType = ESixStrategyType::EstablishOrder;
        Data.Direction = EStrategyDirection::Righteous;
        Data.CooldownDuration = 0.0f;
        Data.Duration = 0.0f;
        Data.EffectIntensity = 1.0f;
        Data.FallValueImpact = 0;
        StrategyMap.Add(ESixStrategyType::EstablishOrder, Data);

        FStrategyEffect Effect;
        Effect.EconomicEfficiencyImpact = 0.25f;
        StrategyEffectMap.Add(ESixStrategyType::EstablishOrder, Effect);
    }

    // 立人 (正) - 將領成長+40%
    {
        FStrategyData Data;
        Data.StrategyType = ESixStrategyType::EstablishPeople;
        Data.Direction = EStrategyDirection::Righteous;
        Data.CooldownDuration = 0.0f;
        Data.Duration = 0.0f;
        Data.EffectIntensity = 1.0f;
        Data.FallValueImpact = 0;
        StrategyMap.Add(ESixStrategyType::EstablishPeople, Data);

        FStrategyEffect Effect;
        Effect.LeaderGrowthImpact = 0.40f;
        StrategyEffectMap.Add(ESixStrategyType::EstablishPeople, Effect);
    }

    // 逆六策初始化

    // 破局 (逆) - 突破包圍/逆轉戰局
    {
        FStrategyData Data;
        Data.StrategyType = ESixStrategyType::BreakSituation;
        Data.Direction = EStrategyDirection::Evil;
        Data.CooldownDuration = 30.0f; // 30秒冷卻
        Data.Duration = 0.0f;
        Data.EffectIntensity = 1.0f;
        Data.FallValueImpact = 20;
        StrategyMap.Add(ESixStrategyType::BreakSituation, Data);

        FStrategyEffect Effect;
        Effect.MoraleImpact = 0.15f; // 士氣微增
        Effect.ReputationImpact = -0.50f; // 信譽下降
        StrategyEffectMap.Add(ESixStrategyType::BreakSituation, Effect);
    }

    // 破結構 (逆) - 敵軍內亂+40%
    {
        FStrategyData Data;
        Data.StrategyType = ESixStrategyType::BreakStructure;
        Data.Direction = EStrategyDirection::Evil;
        Data.CooldownDuration = 45.0f; // 45秒冷卻
        Data.Duration = 180.0f; // 持續3分鐘
        Data.EffectIntensity = 1.0f;
        Data.FallValueImpact = 30;
        StrategyMap.Add(ESixStrategyType::BreakStructure, Data);

        FStrategyEffect Effect;
        Effect.EnemyChaosImpact = 0.40f;
        Effect.ReputationImpact = -0.30f;
        StrategyEffectMap.Add(ESixStrategyType::BreakStructure, Effect);
    }

    // 不破人 (逆) - 永久策反敵將
    {
        FStrategyData Data;
        Data.StrategyType = ESixStrategyType::BreakNotPeople;
        Data.Direction = EStrategyDirection::Evil;
        Data.CooldownDuration = 60.0f; // 60秒冷卻
        Data.Duration = 0.0f; // 永久
        Data.EffectIntensity = 1.0f;
        Data.FallValueImpact = 40;
        StrategyMap.Add(ESixStrategyType::BreakNotPeople, Data);

        FStrategyEffect Effect;
        Effect.EnemyChaosImpact = 0.60f;
        Effect.ReputationImpact = -0.80f; // 嚴重信譽損失
        StrategyEffectMap.Add(ESixStrategyType::BreakNotPeople, Effect);
    }
}

bool UMingStrategyAuthority::UseStrategy(ESixStrategyType StrategyType, float Multiplier)
{
    if (!bIsInitialized)
    {
        return false;
    }

    FStrategyData* Data = StrategyMap.Find(StrategyType);
    if (!Data)
    {
        return false;
    }

    // 檢查策略是否可用
    if (!IsStrategyAvailable(StrategyType))
    {
        return false;
    }

    // 更新使用時間和冷卻狀態
    Data->LastUsedTime = FDateTime::Now();
    if (Data->CooldownDuration > 0)
    {
        Data->bIsOnCooldown = true;
    }

    // 應用效果
    ApplyStrategyEffect(StrategyType, Multiplier);

    // 更新連續使用計數
    if (Data->Direction == EStrategyDirection::Evil)
    {
        ConsecutiveEvilUses++;
        ConsecutiveRighteousUses = 0;
    }
    else
    {
        ConsecutiveRighteousUses++;
        ConsecutiveEvilUses = 0;
    }

    // 獲取效果並廣播事件
    FStrategyEffect Effect = GetStrategyEffect(StrategyType);
    OnStrategyUsed.Broadcast(StrategyType, Effect);

    return true;
}

bool UMingStrategyAuthority::SwitchStrategyDirection(EStrategyDirection NewDirection)
{
    if (NewDirection == CurrentDirection)
    {
        return true;
    }

    if (!CanSwitchDirection(NewDirection))
    {
        return false;
    }

    CurrentDirection = NewDirection;
    OnDirectionSwitched.Broadcast(NewDirection);

    return true;
}

bool UMingStrategyAuthority::ForceSwitchToRighteous()
{
    if (CurrentDirection != EStrategyDirection::Righteous)
    {
        CurrentDirection = EStrategyDirection::Righteous;
        ConsecutiveEvilUses = 0;
        ConsecutiveRighteousUses = 1;
        OnDirectionSwitched.Broadcast(EStrategyDirection::Righteous);
        return true;
    }
    return false;
}

FStrategyData UMingStrategyAuthority::GetStrategyData(ESixStrategyType StrategyType) const
{
    if (const FStrategyData* Data = StrategyMap.Find(StrategyType))
    {
        return *Data;
    }
    return FStrategyData();
}

FStrategyEffect UMingStrategyAuthority::GetStrategyEffect(ESixStrategyType StrategyType) const
{
    if (const FStrategyEffect* Effect = StrategyEffectMap.Find(StrategyType))
    {
        return *Effect;
    }
    return FStrategyEffect();
}

bool UMingStrategyAuthority::IsStrategyAvailable(ESixStrategyType StrategyType) const
{
    if (!bIsInitialized)
    {
        return false;
    }

    const FStrategyData* Data = StrategyMap.Find(StrategyType);
    if (!Data)
    {
        return false;
    }

    // 檢查是否啟用
    if (!Data->bIsEnabled)
    {
        return false;
    }

    // 檢查是否正在冷卻
    if (Data->bIsOnCooldown && Data->CooldownDuration > 0)
    {
        FTimespan Elapsed = FDateTime::Now() - Data->LastUsedTime;
        if (Elapsed.GetTotalSeconds() < Data->CooldownDuration)
        {
            return false;
        }
    }

    return true;
}

bool UMingStrategyAuthority::IsUsingEvilStrategy() const
{
    return CurrentDirection == EStrategyDirection::Evil;
}

TArray<ESixStrategyType> UMingStrategyAuthority::GetRecommendedStrategies() const
{
    TArray<ESixStrategyType> Recommendations;

    // 根據當前方向推薦策略
    for (const auto& Pair : StrategyMap)
    {
        ESixStrategyType StrategyType = Pair.Key;
        const FStrategyData& Data = Pair.Value;

        if (Data.Direction == CurrentDirection && IsStrategyAvailable(StrategyType))
        {
            Recommendations.Add(StrategyType);
        }
    }

    return Recommendations;
}

FString UMingStrategyAuthority::GetStrategyDescription(ESixStrategyType StrategyType) const
{
    switch (StrategyType)
    {
    case ESixStrategyType::EstablishNation:
        return TEXT("立國 (正)：立大義、立旗幟、立共同之志。使敵心生疑、我心生歸；使眾知所向、敵知所忌。效果：士氣+30%, 徵兵+20%");
    case ESixStrategyType::EstablishOrder:
        return TEXT("立制 (正)：立規則、立秩序、立賞罰、立權責。使勢有常軌、命有定分。效果：經濟效率+25%");
    case ESixStrategyType::EstablishPeople:
        return TEXT("立人 (正)：立賢才、立榜樣、立信望、立傳承。使氣有依歸、影有真象。效果：將領成長+40%");
    case ESixStrategyType::BreakSituation:
        return TEXT("破局 (逆)：暗移時機、速轉勢向、乘隙而入、不對稱擊。使敵盛極而忽衰。冷卻：30秒，墮落值+20，信譽-50%");
    case ESixStrategyType::BreakStructure:
        return TEXT("破結構 (逆)：暗鬆根基、亂其秩序、離其盟友、壞其框架。使敵命脈先斷。冷卻：45秒，墮落值+30，敵內亂+40%");
    case ESixStrategyType::BreakNotPeople:
        return TEXT("不破人 (逆)：留其形骸、保其名位、暗奪其志、藏我真跡。使敵身在而心亡。冷卻：60秒，墮落值+40，永久策反");
    default:
        return TEXT("未知策略");
    }
}

FString UMingStrategyAuthority::GetDirectionDescription(EStrategyDirection Direction) const
{
    switch (Direction)
    {
    case EStrategyDirection::Righteous:
        return TEXT("正道：順天應人、堂堂之陣、陽剛之用。可久，可傳，可責。");
    case EStrategyDirection::Evil:
        return TEXT("逆術：違時背理、隱隱之行、陰柔之變。可速，可破，可轉。");
    default:
        return TEXT("未知方向");
    }
}

bool UMingStrategyAuthority::IsHealthy() const
{
    // 策權健康狀況檢查
    // 1. 連續使用逆策不超過5次
    // 2. 至少有一些策略可用

    if (ConsecutiveEvilUses >= 5)
    {
        return false;
    }

    int32 AvailableCount = 0;
    for (const auto& Pair : StrategyMap)
    {
        if (IsStrategyAvailable(Pair.Key))
        {
            AvailableCount++;
        }
    }

    return AvailableCount > 0;
}

void UMingStrategyAuthority::UpdateCooldowns()
{
    FDateTime CurrentTime = FDateTime::Now();

    for (auto& Pair : StrategyMap)
    {
        FStrategyData& Data = Pair.Value;

        if (Data.bIsOnCooldown && Data.CooldownDuration > 0)
        {
            FTimespan Elapsed = CurrentTime - Data.LastUsedTime;
            if (Elapsed.GetTotalSeconds() >= Data.CooldownDuration)
            {
                Data.bIsOnCooldown = false;
                OnStrategyCooldownComplete.Broadcast(Pair.Key);
            }
        }
    }
}

void UMingStrategyAuthority::ApplyStrategyEffect(ESixStrategyType StrategyType, float Multiplier)
{
    // 這裡可以實際應用效果到遊戲系統
    // 例如調用其他管理器來修改數值

    // 目前僅作為示例，實際效果應該由遊戲管理器處理
}

bool UMingStrategyAuthority::CanSwitchDirection(EStrategyDirection NewDirection) const
{
    // 檢查是否可以切換到目標方向
    // 某些情況下可能無法切換（例如魔王無法停止逆策）

    // 這裡應該檢查角色特性，但為了避免循環依賴，簡化處理
    return true;
}

FString UMingStrategyAuthority::GetStrategyWuXing(ESixStrategyType StrategyType) const
{
    switch (StrategyType)
    {
    case ESixStrategyType::EstablishNation:
        return TEXT("木 (春/立名)");
    case ESixStrategyType::EstablishOrder:
        return TEXT("火→土 (夏/長夏/造勢/收權)");
    case ESixStrategyType::EstablishPeople:
        return TEXT("水→木 (冬/春/養機/立人)");
    case ESixStrategyType::BreakSituation:
        return TEXT("水→木 (冬/春/養機/破局)");
    case ESixStrategyType::BreakStructure:
        return TEXT("金→水 (秋/冬/裁斷/破結構)");
    case ESixStrategyType::BreakNotPeople:
        return TEXT("土→金 (長夏/秋/收權/不破人)");
    default:
        return TEXT("未知");
    }
}
