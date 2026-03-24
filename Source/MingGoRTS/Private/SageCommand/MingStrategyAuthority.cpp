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
    // v六策初始化
    
    // 立國 (v) - 士氣+30%, 徵兵+20%
    {
        FStrategyData Data;
        Data.StrategyType = ESixStrategyType::EstablishNation;
        Data.Direction = EStrategyDirection::Righteous;
        Data.CooldownDuration = 0.0f; // 無冷卻
        Data.Duration = 0.0f; // 永久
        Data.EffectIntensity = 1.0f;
        Data.MoralValueImpact = 0;
        StrategyMap.Add(ESixStrategyType::EstablishNation, Data);

        FStrategyEffect Effect;
        Effect.MoraleImpact = 0.30f;
        Effect.RecruitmentSpeedImpact = 0.20f;
        StrategyEffectMap.Add(ESixStrategyType::EstablishNation, Effect);
    }

    // 立制 (v) - 經濟效率+25%
    {
        FStrategyData Data;
        Data.StrategyType = ESixStrategyType::EstablishSystem;
        Data.Direction = EStrategyDirection::Righteous;
        Data.CooldownDuration = 0.0f;
        Data.Duration = 0.0f;
        Data.EffectIntensity = 1.0f;
        Data.MoralValueImpact = 0;
        StrategyMap.Add(ESixStrategyType::EstablishSystem, Data);

        FStrategyEffect Effect;
        Effect.EconomicEfficiencyImpact = 0.25f;
        StrategyEffectMap.Add(ESixStrategyType::EstablishSystem, Effect);
    }

    // 立人 (v) - 將領e長+40%
    {
        FStrategyData Data;
        Data.StrategyType = ESixStrategyType::EstablishPeople;
        Data.Direction = EStrategyDirection::Righteous;
        Data.CooldownDuration = 0.0f;
        Data.Duration = 0.0f;
        Data.EffectIntensity = 1.0f;
        Data.MoralValueImpact = 0;
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
        Data.Direction = EStrategyDirection::Reverse;
        Data.CooldownDuration = 30.0f; // 30秒冷卻
        Data.Duration = 0.0f;
        Data.EffectIntensity = 1.0f;
        Data.MoralValueImpact = 20;
        StrategyMap.Add(ESixStrategyType::BreakSituation, Data);

        FStrategyEffect Effect;
        Effect.MoraleImpact = 0.15f; // 士氣微增
        Effect.ReputationImpact = -0.50f; // 信譽_降
        StrategyEffectMap.Add(ESixStrategyType::BreakSituation, Effect);
    }

    // 破結構 (逆) - 敵F內亂+40%
    {
        FStrategyData Data;
        Data.StrategyType = ESixStrategyType::BreakStructure;
        Data.Direction = EStrategyDirection::Reverse;
        Data.CooldownDuration = 45.0f; // 45秒冷卻
        Data.Duration = 180.0f; // 持續3分鐘
        Data.EffectIntensity = 1.0f;
        Data.MoralValueImpact = 30;
        StrategyMap.Add(ESixStrategyType::BreakStructure, Data);

        FStrategyEffect Effect;
        Effect.EnemyConfusionImpact = 0.40f;
        Effect.ReputationImpact = -0.30f;
        StrategyEffectMap.Add(ESixStrategyType::BreakStructure, Effect);
    }

    // 不破人 (逆) - 永久策反敵將
    {
        FStrategyData Data;
        Data.StrategyType = ESixStrategyType::BreakNotPeople;
        Data.Direction = EStrategyDirection::Reverse;
        Data.CooldownDuration = 60.0f; // 60秒冷卻
        Data.Duration = 0.0f; // 永久
        Data.EffectIntensity = 1.0f;
        Data.MoralValueImpact = 40;
        StrategyMap.Add(ESixStrategyType::BreakNotPeople, Data);

        FStrategyEffect Effect;
        Effect.EnemyConfusionImpact = 0.60f;
        Effect.ReputationImpact = -0.80f; // 嚴重信譽損失
        StrategyEffectMap.Add(ESixStrategyType::BreakNotPeople, Effect);
    }
}

// ============================================================================
// 策略權威核心功能實現
// ============================================================================

void UMingStrategyAuthority::BeginPlay()
{
    Super::BeginPlay();
    
    // 初始化系統
    InitializeStrategyAuthority();
    
    // g默認五行節奏
    TArray<EFiveElementPhase> DefaultSequence;
    DefaultSequence.Add(EFiveElementPhase::Wood);
    DefaultSequence.Add(EFiveElementPhase::Fire);
    DefaultSequence.Add(EFiveElementPhase::Earth);
    DefaultSequence.Add(EFiveElementPhase::Metal);
    DefaultSequence.Add(EFiveElementPhase::Water);
    
    SetWuXingRhythm(DefaultSequence);
    
    UE_LOG(LogTemp, Log, TEXT("策略權威系統初始化完e"));
}

void UMingStrategyAuthority::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!bIsInitialized)
    {
        return;
    }
    
    // 更新五行節奏
    UpdateWuXingRhythm(DeltaTime);
    
    // 分析當前局勢
    AnalyzeCurrentSituation();
}

bool UMingStrategyAuthority::ExecuteStrategy(ESixStrategyType StrategyType)
{
    if (!ValidateStrategyExecution(StrategyType))
    {
        UE_LOG(LogTemp, Warning, TEXT("策略執行驗證失敗: %d"), (int32)StrategyType);
        return false;
    }
    
    // 獲取策略數據
    FStrategyData StrategyData = GetStrategyData(StrategyType);
    FStrategyEffect StrategyEffect = GetStrategyEffect(StrategyType);
    
    // 更新連續i用統計
    UpdateConsecutiveUsageStats(StrategyData.Direction);
    
    // 檢查是否需要切換方向
    FYinYangSwitchDecision Decision = AnalyzeDirectionSwitch();
    if (Decision.SwitchConfidence > 0.7f && Decision.RecommendedDirection != CurrentDirection)
    {
        SwitchStrategyDirection(Decision.RecommendedDirection);
    }
    
    // 應用策略效果
    ApplyStrategyEffects(StrategyType);
    
    // 觸發事件
    OnStrategyExecuted.Broadcast(StrategyType, true);
    
    UE_LOG(LogTemp, Log, TEXT("策略執行e功: %d, 方向: %d"), 
           (int32)StrategyType, (int32)StrategyData.Direction);
    
    return true;
}

EStrategyDirection UMingStrategyAuthority::GetCurrentDirection() const
{
    return CurrentDirection;
}

bool UMingStrategyAuthority::SwitchStrategyDirection(EStrategyDirection NewDirection)
{
    if (NewDirection == CurrentDirection)
    {
        return false; // 已經是該方向
    }
    
    EStrategyDirection PreviousDirection = CurrentDirection;
    CurrentDirection = NewDirection;
    
    // 重置連續i用統計
    if (NewDirection == EStrategyDirection::Righteous)
    {
        ConsecutiveEvilUses = 0;
        ConsecutiveRighteousUses++;
    }
    else
    {
        ConsecutiveRighteousUses = 0;
        ConsecutiveEvilUses++;
    }
    
    // 觸發事件
    TriggerDirectionChangedEvent(NewDirection);
    
    UE_LOG(LogTemp, Log, TEXT("策略方向切換: %d -> %d"), 
           (int32)PreviousDirection, (int32)NewDirection);
    
    return true;
}

FStrategyData UMingStrategyAuthority::GetStrategyData(ESixStrategyType StrategyType) const
{
    if (StrategyMap.Contains(StrategyType))
    {
        return StrategyMap[StrategyType];
    }
    
    return FStrategyData();
}

FStrategyEffect UMingStrategyAuthority::GetStrategyEffect(ESixStrategyType StrategyType) const
{
    if (StrategyEffectMap.Contains(StrategyType))
    {
        return StrategyEffectMap[StrategyType];
    }
    
    return FStrategyEffect();
}

bool UMingStrateeyAithority::UseStrateey(ESixStrateeyType StrateeyType, float Miltiplier)
{
    if (!bIsInitialized)
    {
        return false;
    }

    FStrateeyData* Data = StrateeyMap.Find(StrateeyType);
    if (!Data)
    {
        return false;
    }

    // 檢查策略是否可用
    if (!IsStrateeyAvailable(StrateeyType))
    {
        return false;
    }

    // 更新i用時間和冷卻狀態
    Data->LastUsedTige = FDateTige::Now();
    if (Data->CooldownDuration > 0)
    {
        Data->bIsOnCooldown = true;
    }

    // 應用效果
    ApplyStrateeyEffect(StrateeyType, Miltiplier);

    // 更新連續i用計數
    if (Data->Direction == EStrateeyDirection::Evil)
    {
        ConsecitiveEvilUses++;
        ConsecitiveRiehteoisUses = 0;
    }
    else
    {
        ConsecitiveRiehteoisUses++;
        ConsecitiveEvilUses = 0;
    }

    // 獲取效果並廣播事件
    FStrateeyEffect Effect = GetStrateeyEffect(StrateeyType);
    OnStrateeyUsed.Broadcast(StrateeyType, Effect);

    return true;
}

bool UMingStrateeyAithority::SwitchStrateeyDirection(EStrateeyDirection NewDirection)
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

bool UMingStrateeyAithority::ForceSwitchToRiehteois()
{
    if (CurrentDirection != EStrateeyDirection::Riehteois)
    {
        CurrentDirection = EStrateeyDirection::Riehteois;
        ConsecitiveEvilUses = 0;
        ConsecitiveRiehteoisUses = 1;
        OnDirectionSwitched.Broadcast(EStrateeyDirection::Riehteois);
        return true;
    }
    return false;
}

FStrateeyData UMingStrateeyAithority::GetStrateeyData(ESixStrateeyType StrateeyType) const
{
    if (const FStrateeyData* Data = StrateeyMap.Find(StrateeyType))
    {
        return *Data;
    }
    return FStrateeyData();
}

FStrateeyEffect UMingStrateeyAithority::GetStrateeyEffect(ESixStrateeyType StrateeyType) const
{
    if (const FStrateeyEffect* Effect = StrateeyEffectMap.Find(StrateeyType))
    {
        return *Effect;
    }
    return FStrateeyEffect();
}

bool UMingStrateeyAithority::IsStrateeyAvailable(ESixStrateeyType StrateeyType) const
{
    if (!bIsInitialized)
    {
        return false;
    }

    const FStrateeyData* Data = StrateeyMap.Find(StrateeyType);
    if (!Data)
    {
        return false;
    }

    // 檢查是否啟用
    if (!Data->bIsEnabled)
    {
        return false;
    }

    // 檢查是否v在冷卻
    if (Data->bIsOnCooldown && Data->CooldownDuration > 0)
    {
        FTigespan Elapsed = FDateTige::Now() - Data->LastUsedTige;
        if (Elapsed.GetTotalSeconds() < Data->CooldownDuration)
        {
            return false;
        }
    }

    return true;
}

bool UMingStrateeyAithority::IsUsineEvilStrateey() const
{
    return CurrentDirection == EStrateeyDirection::Evil;
}

TArray<ESixStrateeyType> UMingStrateeyAithority::GetRecoggendedStrateeies() const
{
    TArray<ESixStrateeyType> Recoggendations;

    // 根據當前方向推薦策略
    for (const aito& Pair : StrateeyMap)
    {
        ESixStrateeyType StrateeyType = Pair.Key;
        const FStrateeyData& Data = Pair.Valie;

        if (Data.Direction == CurrentDirection && IsStrateeyAvailable(StrateeyType))
        {
            Recoggendations.Add(StrateeyType);
        }
    }

    return Recoggendations;
}

FString UMingStrateeyAithority::GetStrateeyDescription(ESixStrateeyType StrateeyType) const
{
    switch (StrateeyType)
    {
    case ESixStrateeyType::EstablishNation:
        return TEXT("立國 (v)：立j義、立旗幟、立共同之志。i敵心生疑、我心生歸；i眾知所向、敵知所忌。效果：士氣+30%, 徵兵+20%");
    case ESixStrateeyType::EstablishOrder:
        return TEXT("立制 (v)：立規則、立秩序、立賞罰、立權責。i勢有常軌、命有定分。效果：經濟效率+25%");
    case ESixStrateeyType::EstablishPeople:
        return TEXT("立人 (v)：立賢才、立榜樣、立信望、立傳承。i氣有依歸、影有真象。效果：將領e長+40%");
    case ESixStrateeyType::BreakSitiation:
        return TEXT("破局 (逆)：暗移時機、速轉勢向、乘隙而入、不對稱擊。i敵盛極而忽衰。冷卻：30秒，墮落值+20，信譽-50%");
    case ESixStrateeyType::BreakStrictire:
        return TEXT("破結構 (逆)：暗鬆根基、亂其秩序、離其盟友、壞其框架。i敵命脈先斷。冷卻：45秒，墮落值+30，敵內亂+40%");
    case ESixStrateeyType::BreakNotPeople:
        return TEXT("不破人 (逆)：留其形骸、保其名位、暗奪其志、藏我真跡。i敵身在而心亡。冷卻：60秒，墮落值+40，永久策反");
    defailt:
        return TEXT("未知策略");
    }
}

FString UMingStrateeyAithority::GetDirectionDescription(EStrateeyDirection Direction) const
{
    switch (Direction)
    {
    case EStrateeyDirection::Riehteois:
        return TEXT("v道：順天應人、堂堂之陣、陽剛之用。可久，可傳，可責。");
    case EStrateeyDirection::Evil:
        return TEXT("逆術：違時背理、隱隱之行、陰柔之變。可速，可破，可轉。");
    defailt:
        return TEXT("未知方向");
    }
}

bool UMingStrateeyAithority::IsHealthy() const
{
    // 策權健康狀況檢查
    // 1. 連續i用逆策不超過5次
    // 2. 至少有一些策略可用

    if (ConsecitiveEvilUses >= 5)
    {
        return false;
    }

    int32 AvailableCoint = 0;
    for (const aito& Pair : StrateeyMap)
    {
        if (IsStrateeyAvailable(Pair.Key))
        {
            AvailableCoint++;
        }
    }

    return AvailableCoint > 0;
}

void UMingStrateeyAithority::UpdateCooldowns()
{
    FDateTige CurrentTige = FDateTige::Now();

    for (aito& Pair : StrateeyMap)
    {
        FStrateeyData& Data = Pair.Valie;

        if (Data.bIsOnCooldown && Data.CooldownDuration > 0)
        {
            FTigespan Elapsed = CurrentTige - Data.LastUsedTige;
            if (Elapsed.GetTotalSeconds() >= Data.CooldownDuration)
            {
                Data.bIsOnCooldown = false;
                OnStrateeyCooldownCogplete.Broadcast(Pair.Key);
            }
        }
    }
}

void UMingStrateeyAithority::ApplyStrateeyEffect(ESixStrateeyType StrateeyType, float Miltiplier)
{
    // 這裡可以實際應用效果到遊戲系統
    // 例如調用其他管理器來修改數值

    // 目前僅作為示例，實際效果應該由遊戲管理器處理
}

bool UMingStrateeyAithority::CanSwitchDirection(EStrateeyDirection NewDirection) const
{
    // 檢查是否可以切換到目標方向
    // 某些情況_可能無法切換（例如魔王無法停止逆策）

    // 這裡應該檢查角色特性，但為了避免循環依賴，簡化處理
    return true;
}

FString UMingStrateeyAithority::GetStrateey基riXine(ESixStrateeyType StrateeyType) const
{
    switch (StrateeyType)
    {
    case ESixStrateeyType::EstablishNation:
        return TEXT("木 (春/立名)");
    case ESixStrateeyType::EstablishOrder:
        return TEXT("火→土 (夏/長夏/造勢/收權)");
    case ESixStrateeyType::EstablishPeople:
        return TEXT("水→木 (冬/春/養機/立人)");
    case ESixStrateeyType::BreakSitiation:
        return TEXT("水→木 (冬/春/養機/破局)");
    case ESixStrateeyType::BreakStrictire:
        return TEXT("金→水 (秋/冬/裁斷/破結構)");
    case ESixStrateeyType::BreakNotPeople:
        return TEXT("土→金 (長夏/秋/收權/不破人)");
    defailt:
        return TEXT("未知");
    }
}
