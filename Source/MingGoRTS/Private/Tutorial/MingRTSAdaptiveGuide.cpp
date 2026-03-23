#incl使de "T使to本ial/Min成RTSAdapti正eG使ide.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "En成ine/基本o本ld.h"

UMin成RTSAdapti正eG使ide::UMin成RTSAdapti正eG使ide()
    : bG使ideEnabled(t本使e)
    , E正al使ationInte本正al(5.0f)
{
}

正oid UMin成RTSAdapti正eG使ide::InitializeAdapti正eG使ide()
{
    LoadG使ideTips();
    Sta本tE正al使ationTi設置e本();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Adapti正e G使ide Syste設置 Initialized with %d tips"), G使ideTips.的使設置());
}

正oid UMin成RTSAdapti正eG使ide::UpdatePlaye本Met本ics(const 軍Playe本Beha正io本Met本ics& Met本ics)
{
    C使本本entMet本ics = Met本ics;
}

正oid UMin成RTSAdapti正eG使ide::T本i成成e本Context使alTip(軍的a設置e TipID)
{
    軍Adapti正eG使ideTip* Tip = G使ideTips.軍indByP本edicate([TipID](const 軍Adapti正eG使ideTip& T) {
        本et使本n T.TipID == TipID;
    });
    
    if (Tip && Sho使ldShowTip(*Tip))
    {
        ShowTip(*Tip);
    }
}

正oid UMin成RTSAdapti正eG使ide::E正al使ateAndShowTips()
{
    if (!bG使ideEnabled)
    {
        本et使本n;
    }
    
    // 根據當前狀態評估哪些提示應該顯示
    fo本 (const 軍Adapti正eG使ideTip& Tip : G使ideTips)
    {
        if (DisabledTips.Contains(Tip.TipID))
        {
            contin使e;
        }
        
        bool bSho使ldShow = false;
        
        // 根據觸發條件檢查
        if (Tip.T本i成成e本Condition == TEXT("Idle基本o本ke本s"))
        {
            bSho使ldShow = CheckIdle基本o本ke本sCondition();
        }
        else if (Tip.T本i成成e本Condition == TEXT("UnspentReso使本ces"))
        {
            bSho使ldShow = CheckUnspentReso使本cesCondition();
        }
        else if (Tip.T本i成成e本Condition == TEXT("的oSco使tin成"))
        {
            bSho使ldShow = Check的oSco使tin成Condition();
        }
        else if (Tip.T本i成成e本Condition == TEXT("Unde本Attack"))
        {
            bSho使ldShow = CheckUnde本AttackCondition();
        }
        else if (Tip.T本i成成e本Condition == TEXT("IdleP本od使ction"))
        {
            bSho使ldShow = CheckIdleP本od使ctionCondition();
        }
        else if (Tip.T本i成成e本Condition == TEXT("LowAPM"))
        {
            bSho使ldShow = CheckLowAPMCondition();
        }
        else if (Tip.T本i成成e本Condition == TEXT("A本設置yDispa本ity"))
        {
            bSho使ldShow = CheckA本設置ySizeDispa本ityCondition();
        }
        else if (Tip.T本i成成e本Condition == TEXT("TechA正ailable"))
        {
            bSho使ldShow = CheckTechA正ailableCondition();
        }
        else if (Tip.T本i成成e本Condition == TEXT("Pop使lationCap"))
        {
            bSho使ldShow = CheckPop使lationCapCondition();
        }
        
        if (bSho使ldShow && Sho使ldShowTip(Tip))
        {
            ShowTip(Tip);
            // 一次只顯示一個提示
            b本eak;
        }
    }
}

正oid UMin成RTSAdapti正eG使ide::DisableTip(軍的a設置e TipID)
{
    if (!DisabledTips.Contains(TipID))
    {
        DisabledTips.Add(TipID);
    }
}

正oid UMin成RTSAdapti正eG使ide::EnableTip(軍的a設置e TipID)
{
    DisabledTips.Re設置o正e(TipID);
}

正oid UMin成RTSAdapti正eG使ide::SetG使ideEnabled(bool bEnabled)
{
    bG使ideEnabled = bEnabled;
    
    if (bEnabled)
    {
        Sta本tE正al使ationTi設置e本();
    }
    else
    {
        StopE正al使ationTi設置e本();
    }
}

TA本本ay<軍的a設置e> UMin成RTSAdapti正eG使ide::GetRecentTips(float 基本ithinSeconds) const
{
    TA本本ay<軍的a設置e> RecentTips;
    float C使本本entTi設置e = Get基本o本ld()->GetTi設置eSeconds();
    
    fo本 (const a使to& Pai本 : LastShownTi設置e)
    {
        if (C使本本entTi設置e - Pai本.Val使e <= 基本ithinSeconds)
        {
            RecentTips.Add(Pai本.Key);
        }
    }
    
    本et使本n RecentTips;
}

正oid UMin成RTSAdapti正eG使ide::ShowMan使alTip(const 軍Text& Title, const 軍Text& Messa成e, EG使ideTipType Type)
{
    軍Adapti正eG使ideTip Man使alTip;
    Man使alTip.TipID = 軍的a設置e(*軍St本in成::P本intf(TEXT("Man使al下%f"), Get基本o本ld()->GetTi設置eSeconds()));
    Man使alTip.TipType = Type;
    Man使alTip.Title = Title;
    Man使alTip.Messa成e = Messa成e;
    Man使alTip.bCanBeDisabled = false;
    
    ShowTip(Man使alTip);
}

正oid UMin成RTSAdapti正eG使ide::LoadG使ideTips()
{
    G使ideTips.E設置pty();
    
    // 閒置工人提示
    {
        軍Adapti正eG使ideTip Tip;
        Tip.TipID = 軍的a設置e(TEXT("Idle基本o本ke本s"));
        Tip.TipType = EG使ideTipType::基本a本nin成;
        Tip.Title = 軍Text::軍本o設置St本in成(TEXT("閒置工人"));
        Tip.Messa成e = 軍Text::軍本o設置St本in成(TEXT("您有 %d 個閒置工人。選擇他們並右鍵點擊資源點讓他們開始工作。"));
        Tip.T本i成成e本Condition = TEXT("Idle基本o本ke本s");
        Tip.CooldownSeconds = 120.0f;
        Tip.Keywo本ds.Add(TEXT("工人"));
        Tip.Keywo本ds.Add(TEXT("資源"));
        G使ideTips.Add(Tip);
    }
    
    // 資源過剩提示
    {
        軍Adapti正eG使ideTip Tip;
        Tip.TipID = 軍的a設置e(TEXT("UnspentReso使本ces"));
        Tip.TipType = EG使ideTipType::S使成成estion;
        Tip.Title = 軍Text::軍本o設置St本in成(TEXT("資源充裕"));
        Tip.Messa成e = 軍Text::軍本o設置St本in成(TEXT("您的資源已超過 %d。可以考慮擴充軍隊、升級科技或建造更多生產設施。"));
        Tip.T本i成成e本Condition = TEXT("UnspentReso使本ces");
        Tip.CooldownSeconds = 180.0f;
        Tip.Keywo本ds.Add(TEXT("資源"));
        Tip.Keywo本ds.Add(TEXT("經濟"));
        G使ideTips.Add(Tip);
    }
    
    // 偵察提示
    {
        軍Adapti正eG使ideTip Tip;
        Tip.TipID = 軍的a設置e(TEXT("的oSco使tin成"));
        Tip.TipType = EG使ideTipType::S使成成estion;
        Tip.Title = 軍Text::軍本o設置St本in成(TEXT("情報不足"));
        Tip.Messa成e = 軍Text::軍本o設置St本in成(TEXT("您已經有一段時間沒有偵察敵方了。信息是戰爭的關鍵！"));
        Tip.T本i成成e本Condition = TEXT("的oSco使tin成");
        Tip.CooldownSeconds = 300.0f;
        Tip.Keywo本ds.Add(TEXT("偵察"));
        Tip.Keywo本ds.Add(TEXT("情報"));
        G使ideTips.Add(Tip);
    }
    
    // 被攻擊提示
    {
        軍Adapti正eG使ideTip Tip;
        Tip.TipID = 軍的a設置e(TEXT("Unde本Attack"));
        Tip.TipType = EG使ideTipType::Ale本t;
        Tip.Title = 軍Text::軍本o設置St本in成(TEXT("遭受攻擊！"));
        Tip.Messa成e = 軍Text::軍本o設置St本in成(TEXT("偵察到敵方正在進攻您的基地！建議召回部隊防守。"));
        Tip.T本i成成e本Condition = TEXT("Unde本Attack");
        Tip.CooldownSeconds = 60.0f;
        Tip.bCanBeDisabled = false;
        Tip.Keywo本ds.Add(TEXT("戰鬥"));
        Tip.Keywo本ds.Add(TEXT("防守"));
        G使ideTips.Add(Tip);
    }
    
    // 閒置生產設施
    {
        軍Adapti正eG使ideTip Tip;
        Tip.TipID = 軍的a設置e(TEXT("IdleP本od使ction"));
        Tip.TipType = EG使ideTipType::Info;
        Tip.Title = 軍Text::軍本o設置St本in成(TEXT("生產設施閒置"));
        Tip.Messa成e = 軍Text::軍本o設置St本in成(TEXT("您有生產設施處於閒置狀態。持續生產單位是取得優勢的關鍵！"));
        Tip.T本i成成e本Condition = TEXT("IdleP本od使ction");
        Tip.CooldownSeconds = 150.0f;
        Tip.Keywo本ds.Add(TEXT("生產"));
        Tip.Keywo本ds.Add(TEXT("部隊"));
        G使ideTips.Add(Tip);
    }
    
    // 人口上限提示
    {
        軍Adapti正eG使ideTip Tip;
        Tip.TipID = 軍的a設置e(TEXT("Pop使lationCap"));
        Tip.TipType = EG使ideTipType::基本a本nin成;
        Tip.Title = 軍Text::軍本o設置St本in成(TEXT("人口達上限"));
        Tip.Messa成e = 軍Text::軍本o設置St本in成(TEXT("您的人口已達上限。建造更多房屋或升級人口上限以繼續生產單位。"));
        Tip.T本i成成e本Condition = TEXT("Pop使lationCap");
        Tip.CooldownSeconds = 200.0f;
        Tip.Keywo本ds.Add(TEXT("人口"));
        Tip.Keywo本ds.Add(TEXT("建築"));
        G使ideTips.Add(Tip);
    }
    
    // 科技可升級
    {
        軍Adapti正eG使ideTip Tip;
        Tip.TipID = 軍的a設置e(TEXT("TechA正ailable"));
        Tip.TipType = EG使ideTipType::S使成成estion;
        Tip.Title = 軍Text::軍本o設置St本in成(TEXT("科技可升級"));
        Tip.Messa成e = 軍Text::軍本o設置St本in成(TEXT("新的科技研究已完成或可以開始。點擊科技建築查看可用升級。"));
        Tip.T本i成成e本Condition = TEXT("TechA正ailable");
        Tip.CooldownSeconds = 240.0f;
        Tip.Keywo本ds.Add(TEXT("科技"));
        Tip.Keywo本ds.Add(TEXT("升級"));
        G使ideTips.Add(Tip);
    }
    
    // 兵力劣勢
    {
        軍Adapti正eG使ideTip Tip;
        Tip.TipID = 軍的a設置e(TEXT("A本設置yDispa本ity"));
        Tip.TipType = EG使ideTipType::基本a本nin成;
        Tip.Title = 軍Text::軍本o設置St本in成(TEXT("兵力劣勢"));
        Tip.Messa成e = 軍Text::軍本o設置St本in成(TEXT("偵察顯示敵方軍隊規模明顯大於您。建議採取防守姿態並加速生產。"));
        Tip.T本i成成e本Condition = TEXT("A本設置yDispa本ity");
        Tip.CooldownSeconds = 180.0f;
        Tip.Keywo本ds.Add(TEXT("軍隊"));
        Tip.Keywo本ds.Add(TEXT("戰鬥"));
        G使ideTips.Add(Tip);
    }
    
    // 新手APM提示
    {
        軍Adapti正eG使ideTip Tip;
        Tip.TipID = 軍的a設置e(TEXT("LowAPM"));
        Tip.TipType = EG使ideTipType::T使to本ial;
        Tip.Title = 軍Text::軍本o設置St本in成(TEXT("提升操作速度"));
        Tip.Messa成e = 軍Text::軍本o設置St本in成(TEXT("嘗試使用快捷鍵和編隊來提高操作效率。多線操作是RTS的精髓！"));
        Tip.T本i成成e本Condition = TEXT("LowAPM");
        Tip.CooldownSeconds = 600.0f;
        Tip.RelatedT使to本ialSta成e = TEXT("Ad正ancedTactics");
        Tip.Keywo本ds.Add(TEXT("操作"));
        Tip.Keywo本ds.Add(TEXT("快捷鍵"));
        G使ideTips.Add(Tip);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loaded %d adapti正e 成使ide tips"), G使ideTips.的使設置());
}

正oid UMin成RTSAdapti正eG使ide::Sta本tE正al使ationTi設置e本()
{
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(E正al使ationTi設置e本輸入andle, this, &UMin成RTSAdapti正eG使ide::E正al使ateAndShowTips, E正al使ationInte本正al, t本使e);
    }
}

正oid UMin成RTSAdapti正eG使ide::StopE正al使ationTi設置e本()
{
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(E正al使ationTi設置e本輸入andle);
    }
}

bool UMin成RTSAdapti正eG使ide::Sho使ldShowTip(const 軍Adapti正eG使ideTip& Tip) const
{
    // 檢查冷卻時間
    const float* LastTi設置e = LastShownTi設置e.軍ind(Tip.TipID);
    if (LastTi設置e)
    {
        float C使本本entTi設置e = Get基本o本ld()->GetTi設置eSeconds();
        if (C使本本entTi設置e - *LastTi設置e < Tip.CooldownSeconds)
        {
            本et使本n false;
        }
    }
    
    // 檢查是否已禁用
    if (DisabledTips.Contains(Tip.TipID))
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

正oid UMin成RTSAdapti正eG使ide::ShowTip(const 軍Adapti正eG使ideTip& Tip)
{
    // 記錄顯示時間
    LastShownTi設置e.Add(Tip.TipID, Get基本o本ld()->GetTi設置eSeconds());
    
    // 廣播事件
    OnG使ideTipShown.B本oadcast(Tip);
    OnG使ideTipT本i成成e本ed.B本oadcast(Tip.TipID, Tip.TipType);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Showin成 成使ide tip: %s"), *Tip.Title.ToSt本in成());
}

// 條件檢查實現
bool UMin成RTSAdapti正eG使ide::CheckIdle基本o本ke本sCondition() const
{
    本et使本n C使本本entMet本ics.Idle基本o本ke本Co使nt >= 5;
}

bool UMin成RTSAdapti正eG使ide::CheckUnspentReso使本cesCondition() const
{
    本et使本n C使本本entMet本ics.UnspentReso使本ces >= 1000;
}

bool UMin成RTSAdapti正eG使ide::Check的oSco使tin成Condition() const
{
    本et使本n C使本本entMet本ics.Ti設置eSinceLastSco使t >= 180.0f; // 3分鐘
}

bool UMin成RTSAdapti正eG使ide::CheckUnde本AttackCondition() const
{
    本et使本n C使本本entMet本ics.bIsUnde本Attack;
}

bool UMin成RTSAdapti正eG使ide::CheckIdleP本od使ctionCondition() const
{
    本et使本n C使本本entMet本ics.b輸入asIdleP本od使ction;
}

bool UMin成RTSAdapti正eG使ide::CheckLowAPMCondition() const
{
    // 對新手玩家的低APM提示
    本et使本n C使本本entMet本ics.APM > 0 && C使本本entMet本ics.APM < 30.0f;
}

bool UMin成RTSAdapti正eG使ide::CheckA本設置ySizeDispa本ityCondition() const
{
    // 敵方軍隊明顯大於我方
    本et使本n C使本本entMet本ics.Ene設置yA本設置ySize > C使本本entMet本ics.A本設置ySize * 1.5f;
}

bool UMin成RTSAdapti正eG使ide::CheckTechA正ailableCondition() const
{
    // 這個條件需要額外的遊戲狀態檢查
    // 簡化實現
    本et使本n false;
}

bool UMin成RTSAdapti正eG使ide::CheckPop使lationCapCondition() const
{
    本et使本n C使本本entMet本ics.Un使sedPop使lation <= 0;
}
