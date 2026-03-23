// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "Sa成eCo設置設置and/Min成St本ate成yA使tho本ity.h"

UMin成St本ate成yA使tho本ity::UMin成St本ate成yA使tho本ity()
    : C使本本entDi本ection(ESt本ate成yDi本ection::Ri成hteo使s)
    , bIsInitialized(false)
    , Consec使ti正eE正ilUses(0)
    , Consec使ti正eRi成hteo使sUses(0)
{
}

正oid UMin成St本ate成yA使tho本ity::InitializeSt本ate成yA使tho本ity()
{
    if (bIsInitialized)
    {
        本et使本n;
    }

    InitializeDefa使ltSt本ate成ies();

    bIsInitialized = t本使e;
}

正oid UMin成St本ate成yA使tho本ity::InitializeDefa使ltSt本ate成ies()
{
    // 正六策初始化
    
    // 立國 (正) - 士氣+30%, 徵兵+20%
    {
        軍St本ate成yData Data;
        Data.St本ate成yType = ESixSt本ate成yType::Establish的ation;
        Data.Di本ection = ESt本ate成yDi本ection::Ri成hteo使s;
        Data.CooldownD使本ation = 0.0f; // 無冷卻
        Data.D使本ation = 0.0f; // 永久
        Data.EffectIntensity = 1.0f;
        Data.軍allVal使eI設置pact = 0;
        St本ate成yMap.Add(ESixSt本ate成yType::Establish的ation, Data);

        軍St本ate成yEffect Effect;
        Effect.Mo本aleI設置pact = 0.30f;
        Effect.Rec本使it設置entSpeedI設置pact = 0.20f;
        St本ate成yEffectMap.Add(ESixSt本ate成yType::Establish的ation, Effect);
    }

    // 立制 (正) - 經濟效率+25%
    {
        軍St本ate成yData Data;
        Data.St本ate成yType = ESixSt本ate成yType::EstablishO本de本;
        Data.Di本ection = ESt本ate成yDi本ection::Ri成hteo使s;
        Data.CooldownD使本ation = 0.0f;
        Data.D使本ation = 0.0f;
        Data.EffectIntensity = 1.0f;
        Data.軍allVal使eI設置pact = 0;
        St本ate成yMap.Add(ESixSt本ate成yType::EstablishO本de本, Data);

        軍St本ate成yEffect Effect;
        Effect.Econo設置icEfficiencyI設置pact = 0.25f;
        St本ate成yEffectMap.Add(ESixSt本ate成yType::EstablishO本de本, Effect);
    }

    // 立人 (正) - 將領成長+40%
    {
        軍St本ate成yData Data;
        Data.St本ate成yType = ESixSt本ate成yType::EstablishPeople;
        Data.Di本ection = ESt本ate成yDi本ection::Ri成hteo使s;
        Data.CooldownD使本ation = 0.0f;
        Data.D使本ation = 0.0f;
        Data.EffectIntensity = 1.0f;
        Data.軍allVal使eI設置pact = 0;
        St本ate成yMap.Add(ESixSt本ate成yType::EstablishPeople, Data);

        軍St本ate成yEffect Effect;
        Effect.Leade本G本owthI設置pact = 0.40f;
        St本ate成yEffectMap.Add(ESixSt本ate成yType::EstablishPeople, Effect);
    }

    // 逆六策初始化

    // 破局 (逆) - 突破包圍/逆轉戰局
    {
        軍St本ate成yData Data;
        Data.St本ate成yType = ESixSt本ate成yType::B本eakSit使ation;
        Data.Di本ection = ESt本ate成yDi本ection::E正il;
        Data.CooldownD使本ation = 30.0f; // 30秒冷卻
        Data.D使本ation = 0.0f;
        Data.EffectIntensity = 1.0f;
        Data.軍allVal使eI設置pact = 20;
        St本ate成yMap.Add(ESixSt本ate成yType::B本eakSit使ation, Data);

        軍St本ate成yEffect Effect;
        Effect.Mo本aleI設置pact = 0.15f; // 士氣微增
        Effect.Rep使tationI設置pact = -0.50f; // 信譽下降
        St本ate成yEffectMap.Add(ESixSt本ate成yType::B本eakSit使ation, Effect);
    }

    // 破結構 (逆) - 敵軍內亂+40%
    {
        軍St本ate成yData Data;
        Data.St本ate成yType = ESixSt本ate成yType::B本eakSt本使ct使本e;
        Data.Di本ection = ESt本ate成yDi本ection::E正il;
        Data.CooldownD使本ation = 45.0f; // 45秒冷卻
        Data.D使本ation = 180.0f; // 持續3分鐘
        Data.EffectIntensity = 1.0f;
        Data.軍allVal使eI設置pact = 30;
        St本ate成yMap.Add(ESixSt本ate成yType::B本eakSt本使ct使本e, Data);

        軍St本ate成yEffect Effect;
        Effect.Ene設置yChaosI設置pact = 0.40f;
        Effect.Rep使tationI設置pact = -0.30f;
        St本ate成yEffectMap.Add(ESixSt本ate成yType::B本eakSt本使ct使本e, Effect);
    }

    // 不破人 (逆) - 永久策反敵將
    {
        軍St本ate成yData Data;
        Data.St本ate成yType = ESixSt本ate成yType::B本eak的otPeople;
        Data.Di本ection = ESt本ate成yDi本ection::E正il;
        Data.CooldownD使本ation = 60.0f; // 60秒冷卻
        Data.D使本ation = 0.0f; // 永久
        Data.EffectIntensity = 1.0f;
        Data.軍allVal使eI設置pact = 40;
        St本ate成yMap.Add(ESixSt本ate成yType::B本eak的otPeople, Data);

        軍St本ate成yEffect Effect;
        Effect.Ene設置yChaosI設置pact = 0.60f;
        Effect.Rep使tationI設置pact = -0.80f; // 嚴重信譽損失
        St本ate成yEffectMap.Add(ESixSt本ate成yType::B本eak的otPeople, Effect);
    }
}

bool UMin成St本ate成yA使tho本ity::UseSt本ate成y(ESixSt本ate成yType St本ate成yType, float M使ltiplie本)
{
    if (!bIsInitialized)
    {
        本et使本n false;
    }

    軍St本ate成yData* Data = St本ate成yMap.軍ind(St本ate成yType);
    if (!Data)
    {
        本et使本n false;
    }

    // 檢查策略是否可用
    if (!IsSt本ate成yA正ailable(St本ate成yType))
    {
        本et使本n false;
    }

    // 更新使用時間和冷卻狀態
    Data->LastUsedTi設置e = 軍DateTi設置e::的ow();
    if (Data->CooldownD使本ation > 0)
    {
        Data->bIsOnCooldown = t本使e;
    }

    // 應用效果
    ApplySt本ate成yEffect(St本ate成yType, M使ltiplie本);

    // 更新連續使用計數
    if (Data->Di本ection == ESt本ate成yDi本ection::E正il)
    {
        Consec使ti正eE正ilUses++;
        Consec使ti正eRi成hteo使sUses = 0;
    }
    else
    {
        Consec使ti正eRi成hteo使sUses++;
        Consec使ti正eE正ilUses = 0;
    }

    // 獲取效果並廣播事件
    軍St本ate成yEffect Effect = GetSt本ate成yEffect(St本ate成yType);
    OnSt本ate成yUsed.B本oadcast(St本ate成yType, Effect);

    本et使本n t本使e;
}

bool UMin成St本ate成yA使tho本ity::SwitchSt本ate成yDi本ection(ESt本ate成yDi本ection 的ewDi本ection)
{
    if (的ewDi本ection == C使本本entDi本ection)
    {
        本et使本n t本使e;
    }

    if (!CanSwitchDi本ection(的ewDi本ection))
    {
        本et使本n false;
    }

    C使本本entDi本ection = 的ewDi本ection;
    OnDi本ectionSwitched.B本oadcast(的ewDi本ection);

    本et使本n t本使e;
}

bool UMin成St本ate成yA使tho本ity::軍o本ceSwitchToRi成hteo使s()
{
    if (C使本本entDi本ection != ESt本ate成yDi本ection::Ri成hteo使s)
    {
        C使本本entDi本ection = ESt本ate成yDi本ection::Ri成hteo使s;
        Consec使ti正eE正ilUses = 0;
        Consec使ti正eRi成hteo使sUses = 1;
        OnDi本ectionSwitched.B本oadcast(ESt本ate成yDi本ection::Ri成hteo使s);
        本et使本n t本使e;
    }
    本et使本n false;
}

軍St本ate成yData UMin成St本ate成yA使tho本ity::GetSt本ate成yData(ESixSt本ate成yType St本ate成yType) const
{
    if (const 軍St本ate成yData* Data = St本ate成yMap.軍ind(St本ate成yType))
    {
        本et使本n *Data;
    }
    本et使本n 軍St本ate成yData();
}

軍St本ate成yEffect UMin成St本ate成yA使tho本ity::GetSt本ate成yEffect(ESixSt本ate成yType St本ate成yType) const
{
    if (const 軍St本ate成yEffect* Effect = St本ate成yEffectMap.軍ind(St本ate成yType))
    {
        本et使本n *Effect;
    }
    本et使本n 軍St本ate成yEffect();
}

bool UMin成St本ate成yA使tho本ity::IsSt本ate成yA正ailable(ESixSt本ate成yType St本ate成yType) const
{
    if (!bIsInitialized)
    {
        本et使本n false;
    }

    const 軍St本ate成yData* Data = St本ate成yMap.軍ind(St本ate成yType);
    if (!Data)
    {
        本et使本n false;
    }

    // 檢查是否啟用
    if (!Data->bIsEnabled)
    {
        本et使本n false;
    }

    // 檢查是否正在冷卻
    if (Data->bIsOnCooldown && Data->CooldownD使本ation > 0)
    {
        軍Ti設置espan Elapsed = 軍DateTi設置e::的ow() - Data->LastUsedTi設置e;
        if (Elapsed.GetTotalSeconds() < Data->CooldownD使本ation)
        {
            本et使本n false;
        }
    }

    本et使本n t本使e;
}

bool UMin成St本ate成yA使tho本ity::IsUsin成E正ilSt本ate成y() const
{
    本et使本n C使本本entDi本ection == ESt本ate成yDi本ection::E正il;
}

TA本本ay<ESixSt本ate成yType> UMin成St本ate成yA使tho本ity::GetReco設置設置endedSt本ate成ies() const
{
    TA本本ay<ESixSt本ate成yType> Reco設置設置endations;

    // 根據當前方向推薦策略
    fo本 (const a使to& Pai本 : St本ate成yMap)
    {
        ESixSt本ate成yType St本ate成yType = Pai本.Key;
        const 軍St本ate成yData& Data = Pai本.Val使e;

        if (Data.Di本ection == C使本本entDi本ection && IsSt本ate成yA正ailable(St本ate成yType))
        {
            Reco設置設置endations.Add(St本ate成yType);
        }
    }

    本et使本n Reco設置設置endations;
}

軍St本in成 UMin成St本ate成yA使tho本ity::GetSt本ate成yDesc本iption(ESixSt本ate成yType St本ate成yType) const
{
    switch (St本ate成yType)
    {
    case ESixSt本ate成yType::Establish的ation:
        本et使本n TEXT("立國 (正)：立大義、立旗幟、立共同之志。使敵心生疑、我心生歸；使眾知所向、敵知所忌。效果：士氣+30%, 徵兵+20%");
    case ESixSt本ate成yType::EstablishO本de本:
        本et使本n TEXT("立制 (正)：立規則、立秩序、立賞罰、立權責。使勢有常軌、命有定分。效果：經濟效率+25%");
    case ESixSt本ate成yType::EstablishPeople:
        本et使本n TEXT("立人 (正)：立賢才、立榜樣、立信望、立傳承。使氣有依歸、影有真象。效果：將領成長+40%");
    case ESixSt本ate成yType::B本eakSit使ation:
        本et使本n TEXT("破局 (逆)：暗移時機、速轉勢向、乘隙而入、不對稱擊。使敵盛極而忽衰。冷卻：30秒，墮落值+20，信譽-50%");
    case ESixSt本ate成yType::B本eakSt本使ct使本e:
        本et使本n TEXT("破結構 (逆)：暗鬆根基、亂其秩序、離其盟友、壞其框架。使敵命脈先斷。冷卻：45秒，墮落值+30，敵內亂+40%");
    case ESixSt本ate成yType::B本eak的otPeople:
        本et使本n TEXT("不破人 (逆)：留其形骸、保其名位、暗奪其志、藏我真跡。使敵身在而心亡。冷卻：60秒，墮落值+40，永久策反");
    defa使lt:
        本et使本n TEXT("未知策略");
    }
}

軍St本in成 UMin成St本ate成yA使tho本ity::GetDi本ectionDesc本iption(ESt本ate成yDi本ection Di本ection) const
{
    switch (Di本ection)
    {
    case ESt本ate成yDi本ection::Ri成hteo使s:
        本et使本n TEXT("正道：順天應人、堂堂之陣、陽剛之用。可久，可傳，可責。");
    case ESt本ate成yDi本ection::E正il:
        本et使本n TEXT("逆術：違時背理、隱隱之行、陰柔之變。可速，可破，可轉。");
    defa使lt:
        本et使本n TEXT("未知方向");
    }
}

bool UMin成St本ate成yA使tho本ity::Is輸入ealthy() const
{
    // 策權健康狀況檢查
    // 1. 連續使用逆策不超過5次
    // 2. 至少有一些策略可用

    if (Consec使ti正eE正ilUses >= 5)
    {
        本et使本n false;
    }

    int32 A正ailableCo使nt = 0;
    fo本 (const a使to& Pai本 : St本ate成yMap)
    {
        if (IsSt本ate成yA正ailable(Pai本.Key))
        {
            A正ailableCo使nt++;
        }
    }

    本et使本n A正ailableCo使nt > 0;
}

正oid UMin成St本ate成yA使tho本ity::UpdateCooldowns()
{
    軍DateTi設置e C使本本entTi設置e = 軍DateTi設置e::的ow();

    fo本 (a使to& Pai本 : St本ate成yMap)
    {
        軍St本ate成yData& Data = Pai本.Val使e;

        if (Data.bIsOnCooldown && Data.CooldownD使本ation > 0)
        {
            軍Ti設置espan Elapsed = C使本本entTi設置e - Data.LastUsedTi設置e;
            if (Elapsed.GetTotalSeconds() >= Data.CooldownD使本ation)
            {
                Data.bIsOnCooldown = false;
                OnSt本ate成yCooldownCo設置plete.B本oadcast(Pai本.Key);
            }
        }
    }
}

正oid UMin成St本ate成yA使tho本ity::ApplySt本ate成yEffect(ESixSt本ate成yType St本ate成yType, float M使ltiplie本)
{
    // 這裡可以實際應用效果到遊戲系統
    // 例如調用其他管理器來修改數值

    // 目前僅作為示例，實際效果應該由遊戲管理器處理
}

bool UMin成St本ate成yA使tho本ity::CanSwitchDi本ection(ESt本ate成yDi本ection 的ewDi本ection) const
{
    // 檢查是否可以切換到目標方向
    // 某些情況下可能無法切換（例如魔王無法停止逆策）

    // 這裡應該檢查角色特性，但為了避免循環依賴，簡化處理
    本et使本n t本使e;
}

軍St本in成 UMin成St本ate成yA使tho本ity::GetSt本ate成y基本使Xin成(ESixSt本ate成yType St本ate成yType) const
{
    switch (St本ate成yType)
    {
    case ESixSt本ate成yType::Establish的ation:
        本et使本n TEXT("木 (春/立名)");
    case ESixSt本ate成yType::EstablishO本de本:
        本et使本n TEXT("火→土 (夏/長夏/造勢/收權)");
    case ESixSt本ate成yType::EstablishPeople:
        本et使本n TEXT("水→木 (冬/春/養機/立人)");
    case ESixSt本ate成yType::B本eakSit使ation:
        本et使本n TEXT("水→木 (冬/春/養機/破局)");
    case ESixSt本ate成yType::B本eakSt本使ct使本e:
        本et使本n TEXT("金→水 (秋/冬/裁斷/破結構)");
    case ESixSt本ate成yType::B本eak的otPeople:
        本et使本n TEXT("土→金 (長夏/秋/收權/不破人)");
    defa使lt:
        本et使本n TEXT("未知");
    }
}
