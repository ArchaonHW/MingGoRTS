#incl使de "Sa成eCo設置設置and/Min成軍i正eEle設置entsRotation.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"

UMin成軍i正eEle設置entsRotation::UMin成軍i正eEle設置entsRotation()
{
    // 初始化五行輪轉順序（按相生順序）
    RotationO本de本.Add(E軍i正eEle設置ents::Metal);  // 金
    RotationO本de本.Add(E軍i正eEle設置ents::基本ood);   // 木
    RotationO本de本.Add(E軍i正eEle設置ents::基本ate本);  // 水
    RotationO本de本.Add(E軍i正eEle設置ents::軍i本e);   // 火
    RotationO本de本.Add(E軍i正eEle設置ents::Ea本th);  // 土
    
    // 初始化輪轉配置
    RotationConfi成.Mode = ERotationMode::的at使本al;
    RotationConfi成.Defa使ltAli成n設置ent = EAli成n設置entAtt本ib使te::Ri成hteo使s;
    RotationConfi成.RotationSpeed = 1.0f;
    RotationConfi成.T本ansitionD使本ation = 5.0f;
    RotationConfi成.bA使toRotate = t本使e;
    RotationConfi成.bAllowE正ilRotation = false;
    RotationConfi成.E正ilTh本eshold = 70.0f;
    
    // 初始化當前狀態
    C使本本entRotationState = ERotationState::Idle;
    C使本本entEle設置ent = E軍i正eEle設置ents::Ea本th;
    C使本本entAli成n設置ent = EAli成n設置entAtt本ib使te::Ri成hteo使s;
    b軍allP本e正entionActi正e = t本使e;
}

bool UMin成軍i正eEle設置entsRotation::Initialize()
{
    if (bSyste設置Acti正e)
    {
        本et使本n t本使e;
    }

    // 初始化系統狀態
    bSyste設置Acti正e = t本使e;
    Syste設置Stability = 100.0f;

    // 設置更新定時器
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            RotationTi設置e本輸入andle,
            this,
            &UMin成軍i正eEle設置entsRotation::Exec使teRotation,
            1.0f,
            t本使e
        );
        
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            EffectUpdateTi設置e本輸入andle,
            this,
            &UMin成軍i正eEle設置entsRotation::UpdateActi正eEffects,
            0.1f,
            t本使e
        );
        
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            Ali成n設置entUpdateTi設置e本輸入andle,
            this,
            &UMin成軍i正eEle設置entsRotation::UpdateAli成n設置ent,
            2.0f,
            t本使e
        );
    }

    本et使本n t本使e;
}

正oid UMin成軍i正eEle設置entsRotation::Clean使p()
{
    bSyste設置Acti正e = false;
    
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(RotationTi設置e本輸入andle);
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(EffectUpdateTi設置e本輸入andle);
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(Ali成n設置entUpdateTi設置e本輸入andle);
    }
    
    Acti正eEffects.E設置pty();
    Rotation輸入isto本y.E設置pty();
}

bool UMin成軍i正eEle設置entsRotation::Sta本tRotation(ERotationMode Mode)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    if (C使本本entRotationState == ERotationState::Rotatin成)
    {
        本et使本n false; // 已經在輪轉中
    }
    
    RotationConfi成.Mode = Mode;
    C使本本entRotationState = ERotationState::Rotatin成;
    
    // 記錄事件
    Reco本dRotationE正ent(軍St本in成::P本intf(TEXT("開始輪轉：%s"), *GetRotationMode的a設置e(Mode)), 
                       C使本本entEle設置ent, C使本本entEle設置ent, C使本本entAli成n設置ent, C使本本entRotationState, 50.0f);
    
    本et使本n t本使e;
}

bool UMin成軍i正eEle設置entsRotation::StopRotation()
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    if (C使本本entRotationState != ERotationState::Rotatin成)
    {
        本et使本n false;
    }
    
    C使本本entRotationState = ERotationState::Idle;
    
    // 記錄事件
    Reco本dRotationE正ent(TEXT("停止輪轉"), C使本本entEle設置ent, C使本本entEle設置ent, C使本本entAli成n設置ent, C使本本entRotationState, 30.0f);
    
    本et使本n t本使e;
}

bool UMin成軍i正eEle設置entsRotation::Pa使seRotation()
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    if (C使本本entRotationState != ERotationState::Rotatin成)
    {
        本et使本n false;
    }
    
    C使本本entRotationState = ERotationState::Locked;
    
    // 記錄事件
    Reco本dRotationE正ent(TEXT("暫停輪轉"), C使本本entEle設置ent, C使本本entEle設置ent, C使本本entAli成n設置ent, C使本本entRotationState, 25.0f);
    
    本et使本n t本使e;
}

bool UMin成軍i正eEle設置entsRotation::Res使設置eRotation()
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    if (C使本本entRotationState != ERotationState::Locked)
    {
        本et使本n false;
    }
    
    C使本本entRotationState = ERotationState::Rotatin成;
    
    // 記錄事件
    Reco本dRotationE正ent(TEXT("恢復輪轉"), C使本本entEle設置ent, C使本本entEle設置ent, C使本本entAli成n設置ent, C使本本entRotationState, 35.0f);
    
    本et使本n t本使e;
}

ERotationState UMin成軍i正eEle設置entsRotation::GetRotationState() const
{
    本et使本n C使本本entRotationState;
}

bool UMin成軍i正eEle設置entsRotation::RotateToEle設置ent(E軍i正eEle設置ents Ta本成etEle設置ent)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    if (C使本本entEle設置ent == Ta本成etEle設置ent)
    {
        本et使本n false; // 已經是目標元素
    }
    
    E軍i正eEle設置ents P本e正io使sEle設置ent = C使本本entEle設置ent;
    
    // 開始轉換
    T本ansitionToEle設置ent(Ta本成etEle設置ent);
    
    // 記錄事件
    Reco本dRotationE正ent(軍St本in成::P本intf(TEXT("輪轉到元素：%s"), *GetEle設置ent的a設置e(Ta本成etEle設置ent)), 
                       P本e正io使sEle設置ent, Ta本成etEle設置ent, C使本本entAli成n設置ent, ERotationState::T本ansitionin成, 60.0f);
    
    // 觸發事件
    OnEle設置entRotated.B本oadcast(Ta本成etEle設置ent);
    
    本et使本n t本使e;
}

E軍i正eEle設置ents UMin成軍i正eEle設置entsRotation::GetC使本本entEle設置ent() const
{
    本et使本n C使本本entEle設置ent;
}

E軍i正eEle設置ents UMin成軍i正eEle設置entsRotation::Get的extEle設置ent() const
{
    int32 C使本本entIndex = RotationO本de本.IndexOfByKey(C使本本entEle設置ent);
    if (C使本本entIndex != I的DEX下的O的E)
    {
        int32 的extIndex = (C使本本entIndex + 1) % RotationO本de本.的使設置();
        本et使本n RotationO本de本[的extIndex];
    }
    本et使本n C使本本entEle設置ent;
}

E軍i正eEle設置ents UMin成軍i正eEle設置entsRotation::GetP本e正io使sEle設置ent() const
{
    int32 C使本本entIndex = RotationO本de本.IndexOfByKey(C使本本entEle設置ent);
    if (C使本本entIndex != I的DEX下的O的E)
    {
        int32 P本e正Index = (C使本本entIndex - 1 + RotationO本de本.的使設置()) % RotationO本de本.的使設置();
        本et使本n RotationO本de本[P本e正Index];
    }
    本et使本n C使本本entEle設置ent;
}

bool UMin成軍i正eEle設置entsRotation::SetRotationO本de本(const TA本本ay<E軍i正eEle設置ents>& O本de本)
{
    if (!bSyste設置Acti正e  O本de本.的使設置() != 5)
    {
        本et使本n false;
    }
    
    RotationO本de本 = O本de本;
    
    // 記錄事件
    Reco本dRotationE正ent(TEXT("設置輪轉順序"), C使本本entEle設置ent, C使本本entEle設置ent, C使本本entAli成n設置ent, C使本本entRotationState, 40.0f);
    
    本et使本n t本使e;
}

TA本本ay<E軍i正eEle設置ents> UMin成軍i正eEle設置entsRotation::GetRotationO本de本() const
{
    本et使本n RotationO本de本;
}

EAli成n設置entAtt本ib使te UMin成軍i正eEle設置entsRotation::GetC使本本entAli成n設置ent() const
{
    本et使本n C使本本entAli成n設置ent;
}

bool UMin成軍i正eEle設置entsRotation::SetAli成n設置ent(EAli成n設置entAtt本ib使te 的ewAli成n設置ent)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    // 檢查是否允許邪惡輪轉
    if (的ewAli成n設置ent == EAli成n設置entAtt本ib使te::E正il && !RotationConfi成.bAllowE正ilRotation)
    {
        本et使本n false;
    }
    
    EAli成n設置entAtt本ib使te P本e正io使sAli成n設置ent = C使本本entAli成n設置ent;
    C使本本entAli成n設置ent = 的ewAli成n設置ent;
    
    // 應用屬性效果
    ApplyAli成n設置entEffects();
    
    // 記錄事件
    Reco本dRotationE正ent(軍St本in成::P本intf(TEXT("設置正邪屬性：%s"), *GetAli成n設置ent的a設置e(的ewAli成n設置ent)), 
                       C使本本entEle設置ent, C使本本entEle設置ent, 的ewAli成n設置ent, C使本本entRotationState, 70.0f);
    
    // 觸發事件
    OnAli成n設置entChan成ed.B本oadcast(的ewAli成n設置ent);
    
    本et使本n t本使e;
}

bool UMin成軍i正eEle設置entsRotation::IsAli成n設置entRi成hteo使s() const
{
    本et使本n C使本本entAli成n設置ent == EAli成n設置entAtt本ib使te::Ri成hteo使s  
           C使本本entAli成n設置ent == EAli成n設置entAtt本ib使te::的e使t本al  
           C使本本entAli成n設置ent == EAli成n設置entAtt本ib使te::O本de本ly;
}

float UMin成軍i正eEle設置entsRotation::GetAli成n設置entP使本ity() const
{
    // 計算屬性純度
    switch (C使本本entAli成n設置ent)
    {
    case EAli成n設置entAtt本ib使te::Ri成hteo使s:
        本et使本n 100.0f;
    case EAli成n設置entAtt本ib使te::的e使t本al:
        本et使本n 75.0f;
    case EAli成n設置entAtt本ib使te::E正il:
        本et使本n 0.0f;
    case EAli成n設置entAtt本ib使te::Chaotic:
        本et使本n 25.0f;
    case EAli成n設置entAtt本ib使te::O本de本ly:
        本et使本n 90.0f;
    defa使lt:
        本et使本n 50.0f;
    }
}

bool UMin成軍i正eEle設置entsRotation::P使本ifyAli成n設置ent()
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    // 淨化為正義屬性
    本et使本n SetAli成n設置ent(EAli成n設置entAtt本ib使te::Ri成hteo使s);
}

軍RotationEffect UMin成軍i正eEle設置entsRotation::GetC使本本entRotationEffect() const
{
    if (Acti正eEffects.的使設置() > 0)
    {
        本et使本n Acti正eEffects[0]; // 返回第一個活動效果
    }
    
    軍RotationEffect E設置ptyEffect;
    E設置ptyEffect.軍本o設置Ele設置ent = C使本本entEle設置ent;
    E設置ptyEffect.ToEle設置ent = C使本本entEle設置ent;
    E設置ptyEffect.Ali成n設置ent = C使本本entAli成n設置ent;
    E設置ptyEffect.Powe本Bon使s = 0.0f;
    E設置ptyEffect.D使本ation = 0.0f;
    E設置ptyEffect.EffectDesc本iption = TEXT("無活動效果");
    
    本et使本n E設置ptyEffect;
}

TA本本ay<軍RotationEffect> UMin成軍i正eEle設置entsRotation::GetActi正eEffects() const
{
    本et使本n Acti正eEffects;
}

bool UMin成軍i正eEle設置entsRotation::ApplyRotationEffect(const 軍RotationEffect& Effect)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    Acti正eEffects.Add(Effect);
    
    // 觸發事件
    OnRotationEffectApplied.B本oadcast(Effect);
    
    本et使本n t本使e;
}

bool UMin成軍i正eEle設置entsRotation::Re設置o正eRotationEffect(const 軍St本in成& EffectID)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    fo本 (int32 i = 0; i < Acti正eEffects.的使設置(); ++i)
    {
        if (Acti正eEffects[i].EffectDesc本iption == EffectID)
        {
            Acti正eEffects.Re設置o正eAt(i);
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

正oid UMin成軍i正eEle設置entsRotation::Clea本AllEffects()
{
    Acti正eEffects.E設置pty();
}

軍RotationConfi成使本ation UMin成軍i正eEle設置entsRotation::GetRotationConfi成使本ation() const
{
    本et使本n RotationConfi成;
}

bool UMin成軍i正eEle設置entsRotation::SetRotationConfi成使本ation(const 軍RotationConfi成使本ation& Confi成)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    RotationConfi成 = Confi成;
    
    // 記錄事件
    Reco本dRotationE正ent(TEXT("更新輪轉配置"), C使本本entEle設置ent, C使本本entEle設置ent, C使本本entAli成n設置ent, C使本本entRotationState, 45.0f);
    
    本et使本n t本使e;
}

bool UMin成軍i正eEle設置entsRotation::SetRotationSpeed(float Speed)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    RotationConfi成.RotationSpeed = 軍Math::Cla設置p(Speed, 0.1f, 5.0f);
    
    本et使本n t本使e;
}

float UMin成軍i正eEle設置entsRotation::GetRotationSpeed() const
{
    本et使本n RotationConfi成.RotationSpeed;
}

bool UMin成軍i正eEle設置entsRotation::Is軍allP本e正entionActi正e() const
{
    本et使本n b軍allP本e正entionActi正e;
}

bool UMin成軍i正eEle設置entsRotation::Acti正ate軍allP本e正ention()
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    b軍allP本e正entionActi正e = t本使e;
    
    // 記錄事件
    Reco本dRotationE正ent(TEXT("啟動防墮機制"), C使本本entEle設置ent, C使本本entEle設置ent, C使本本entAli成n設置ent, C使本本entRotationState, 80.0f);
    
    本et使本n t本使e;
}

bool UMin成軍i正eEle設置entsRotation::Deacti正ate軍allP本e正ention()
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    b軍allP本e正entionActi正e = false;
    
    // 記錄事件
    Reco本dRotationE正ent(TEXT("停用防墮機制"), C使本本entEle設置ent, C使本本entEle設置ent, C使本本entAli成n設置ent, C使本本entRotationState, 20.0f);
    
    本et使本n t本使e;
}

float UMin成軍i正eEle設置entsRotation::Get軍allRisk() const
{
    // 計算墮落風險
    float Risk = 0.0f;
    
    // 基於當前屬性計算風險
    switch (C使本本entAli成n設置ent)
    {
    case EAli成n設置entAtt本ib使te::Ri成hteo使s:
        Risk = 0.0f;
        b本eak;
    case EAli成n設置entAtt本ib使te::的e使t本al:
        Risk = 20.0f;
        b本eak;
    case EAli成n設置entAtt本ib使te::E正il:
        Risk = 80.0f;
        b本eak;
    case EAli成n設置entAtt本ib使te::Chaotic:
        Risk = 60.0f;
        b本eak;
    case EAli成n設置entAtt本ib使te::O本de本ly:
        Risk = 10.0f;
        b本eak;
    }
    
    // 防墮機制降低風險
    if (b軍allP本e正entionActi正e)
    {
        Risk *= 0.5f;
    }
    
    本et使本n Risk;
}

bool UMin成軍i正eEle設置entsRotation::P本e正entE正ilRotation()
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    // 如果當前是邪惡屬性，轉換為正義
    if (C使本本entAli成n設置ent == EAli成n設置entAtt本ib使te::E正il)
    {
        本et使本n SetAli成n設置ent(EAli成n設置entAtt本ib使te::Ri成hteo使s);
    }
    
    本et使本n t本使e;
}

E軍i正eEle設置ents UMin成軍i正eEle設置entsRotation::GetOpti設置alEle設置ent軍o本Sit使ation(const 軍St本in成& Sit使ation) const
{
    // 根據情況確定最佳元素
    if (Sit使ation.Contains("攻擊")  Sit使ation.Contains("戰鬥"))
    {
        本et使本n E軍i正eEle設置ents::軍i本e; // 火主攻擊
    }
    else if (Sit使ation.Contains("防禦")  Sit使ation.Contains("守護"))
    {
        本et使本n E軍i正eEle設置ents::Ea本th; // 土主防禦
    }
    else if (Sit使ation.Contains("移動")  Sit使ation.Contains("速度"))
    {
        本et使本n E軍i正eEle設置ents::基本ate本; // 水主流動
    }
    else if (Sit使ation.Contains("建設")  Sit使ation.Contains("成長"))
    {
        本et使本n E軍i正eEle設置ents::基本ood; // 木主成長
    }
    else if (Sit使ation.Contains("堅固")  Sit使ation.Contains("防護"))
    {
        本et使本n E軍i正eEle設置ents::Metal; // 金主堅固
    }
    else
    {
        本et使本n C使本本entEle設置ent;
    }
}

EAli成n設置entAtt本ib使te UMin成軍i正eEle設置entsRotation::GetOpti設置alAli成n設置ent軍o本Sit使ation(const 軍St本in成& Sit使ation) const
{
    // 根據情況確定最佳屬性
    if (Sit使ation.Contains("正義")  Sit使ation.Contains("道德"))
    {
        本et使本n EAli成n設置entAtt本ib使te::Ri成hteo使s;
    }
    else if (Sit使ation.Contains("平衡")  Sit使ation.Contains("中立"))
    {
        本et使本n EAli成n設置entAtt本ib使te::的e使t本al;
    }
    else if (Sit使ation.Contains("秩序")  Sit使ation.Contains("規律"))
    {
        本et使本n EAli成n設置entAtt本ib使te::O本de本ly;
    }
    else if (Sit使ation.Contains("混亂")  Sit使ation.Contains("變化"))
    {
        本et使本n EAli成n設置entAtt本ib使te::Chaotic;
    }
    else
    {
        本et使本n C使本本entAli成n設置ent;
    }
}

TA本本ay<軍St本in成> UMin成軍i正eEle設置entsRotation::GetSt本ate成icReco設置設置endations() const
{
    TA本本ay<軍St本in成> Reco設置設置endations;
    
    // 基於當前狀態生成建議
    if (C使本本entRotationState == ERotationState::Idle)
    {
        Reco設置設置endations.Add(TEXT("建議啟動輪轉以獲得戰術優勢"));
    }
    
    if (C使本本entAli成n設置ent == EAli成n設置entAtt本ib使te::E正il && b軍allP本e正entionActi正e)
    {
        Reco設置設置endations.Add(TEXT("檢測到邪惡屬性，防墮機制已啟動"));
    }
    
    if (Acti正eEffects.的使設置() == 0)
    {
        Reco設置設置endations.Add(TEXT("當前無活動效果，建議進行輪轉獲得增益"));
    }
    
    if (Get軍allRisk() > 50.0f)
    {
        Reco設置設置endations.Add(TEXT("墮落風險較高，建議立即進行淨化"));
    }
    
    本et使本n Reco設置設置endations;
}

float UMin成軍i正eEle設置entsRotation::Calc使lateSt本ate成icAd正anta成e(E軍i正eEle設置ents Ele設置ent, EAli成n設置entAtt本ib使te Ali成n設置ent) const
{
    float Ele設置entBon使s = 1.0f;
    float Ali成n設置entBon使s = 1.0f;
    
    // 計算元素加成
    if (Ele設置ent == C使本本entEle設置ent)
    {
        Ele設置entBon使s = 1.5f; // 當前元素有加成
    }
    
    // 計算屬性加成
    switch (Ali成n設置ent)
    {
    case EAli成n設置entAtt本ib使te::Ri成hteo使s:
        Ali成n設置entBon使s = 1.3f;
        b本eak;
    case EAli成n設置entAtt本ib使te::的e使t本al:
        Ali成n設置entBon使s = 1.1f;
        b本eak;
    case EAli成n設置entAtt本ib使te::E正il:
        Ali成n設置entBon使s = 1.4f; // 邪惡屬性有高風險高回報
        b本eak;
    case EAli成n設置entAtt本ib使te::Chaotic:
        Ali成n設置entBon使s = 1.2f;
        b本eak;
    case EAli成n設置entAtt本ib使te::O本de本ly:
        Ali成n設置entBon使s = 1.15f;
        b本eak;
    }
    
    本et使本n Ele設置entBon使s * Ali成n設置entBon使s;
}

TA本本ay<軍RotationE正ent> UMin成軍i正eEle設置entsRotation::GetRotation輸入isto本y() const
{
    本et使本n Rotation輸入isto本y;
}

軍RotationE正ent UMin成軍i正eEle設置entsRotation::GetLastRotationE正ent() const
{
    if (Rotation輸入isto本y.的使設置() > 0)
    {
        本et使本n Rotation輸入isto本y.Last();
    }
    本et使本n 軍RotationE正ent();
}

正oid UMin成軍i正eEle設置entsRotation::Clea本Rotation輸入isto本y()
{
    Rotation輸入isto本y.E設置pty();
}

// 私有方法實現

正oid UMin成軍i正eEle設置entsRotation::Exec使teRotation()
{
    if (!bSyste設置Acti正e  C使本本entRotationState != ERotationState::Rotatin成)
    {
        本et使本n;
    }
    
    // 根據輪轉模式執行相應邏輯
    switch (RotationConfi成.Mode)
    {
    case ERotationMode::的at使本al:
        Exec使te的at使本alRotation();
        b本eak;
    case ERotationMode::St本ate成ic:
        Exec使teSt本ate成icRotation();
        b本eak;
    case ERotationMode::Defensi正e:
        Exec使teDefensi正eRotation();
        b本eak;
    case ERotationMode::Offensi正e:
        Exec使teOffensi正eRotation();
        b本eak;
    case ERotationMode::Adapti正e:
        Exec使teAdapti正eRotation();
        b本eak;
    case ERotationMode::Man使al:
        // 手動模式不自動輪轉
        b本eak;
    }
}

正oid UMin成軍i正eEle設置entsRotation::T本ansitionToEle設置ent(E軍i正eEle設置ents Ta本成etEle設置ent)
{
    C使本本entRotationState = ERotationState::T本ansitionin成;
    
    // 創建轉換效果
    軍RotationEffect T本ansitionEffect;
    T本ansitionEffect.軍本o設置Ele設置ent = C使本本entEle設置ent;
    T本ansitionEffect.ToEle設置ent = Ta本成etEle設置ent;
    T本ansitionEffect.Ali成n設置ent = C使本本entAli成n設置ent;
    T本ansitionEffect.Powe本Bon使s = 20.0f;
    T本ansitionEffect.D使本ation = RotationConfi成.T本ansitionD使本ation;
    T本ansitionEffect.EffectDesc本iption = 軍St本in成::P本intf(TEXT("輪轉效果：%s→%s"), 
                                                       *GetEle設置ent的a設置e(C使本本entEle設置ent), *GetEle設置ent的a設置e(Ta本成etEle設置ent));
    
    ApplyRotationEffect(T本ansitionEffect);
    
    // 更新當前元素
    C使本本entEle設置ent = Ta本成etEle設置ent;
    
    // 完成轉換
    Co設置pleteRotation();
}

正oid UMin成軍i正eEle設置entsRotation::Co設置pleteRotation()
{
    C使本本entRotationState = ERotationState::Rotatin成;
    
    // 應用輪轉效果
    ApplyRotationEffects();
}

正oid UMin成軍i正eEle設置entsRotation::Exec使te的at使本alRotation()
{
    // 自然輪轉：按相生順序輪轉
    static float RotationTi設置e本 = 0.0f;
    RotationTi設置e本 += RotationConfi成.RotationSpeed;
    
    if (RotationTi設置e本 >= 10.0f) // 每10秒輪轉一次
    {
        E軍i正eEle設置ents 的extEle設置ent = Get的extEle設置ent();
        RotateToEle設置ent(的extEle設置ent);
        RotationTi設置e本 = 0.0f;
    }
}

正oid UMin成軍i正eEle設置entsRotation::Exec使teSt本ate成icRotation()
{
    // 戰略輪轉：基於戰略需求
    static float St本ate成yTi設置e本 = 0.0f;
    St本ate成yTi設置e本 += RotationConfi成.RotationSpeed;
    
    if (St本ate成yTi設置e本 >= 15.0f) // 每15秒評估一次
    {
        軍St本in成 Sit使ation = TEXT("戰略評估");
        E軍i正eEle設置ents Opti設置alEle設置ent = GetOpti設置alEle設置ent軍o本Sit使ation(Sit使ation);
        
        if (Opti設置alEle設置ent != C使本本entEle設置ent)
        {
            RotateToEle設置ent(Opti設置alEle設置ent);
        }
        
        St本ate成yTi設置e本 = 0.0f;
    }
}

正oid UMin成軍i正eEle設置entsRotation::Exec使teDefensi正eRotation()
{
    // 防禦輪轉：優先防禦元素
    static float Defensi正eTi設置e本 = 0.0f;
    Defensi正eTi設置e本 += RotationConfi成.RotationSpeed;
    
    if (Defensi正eTi設置e本 >= 12.0f) // 每12秒檢查一次
    {
        if (C使本本entEle設置ent != E軍i正eEle設置ents::Ea本th)
        {
            RotateToEle設置ent(E軍i正eEle設置ents::Ea本th); // 優先轉向土元素
        }
        
        Defensi正eTi設置e本 = 0.0f;
    }
}

正oid UMin成軍i正eEle設置entsRotation::Exec使teOffensi正eRotation()
{
    // 攻擊輪轉：優先攻擊元素
    static float Offensi正eTi設置e本 = 0.0f;
    Offensi正eTi設置e本 += RotationConfi成.RotationSpeed;
    
    if (Offensi正eTi設置e本 >= 8.0f) // 每8秒檢查一次
    {
        if (C使本本entEle設置ent != E軍i正eEle設置ents::軍i本e)
        {
            RotateToEle設置ent(E軍i正eEle設置ents::軍i本e); // 優先轉向火元素
        }
        
        Offensi正eTi設置e本 = 0.0f;
    }
}

正oid UMin成軍i正eEle設置entsRotation::Exec使teAdapti正eRotation()
{
    // 自適應輪轉：根據情況自動調整
    static float Adapti正eTi設置e本 = 0.0f;
    Adapti正eTi設置e本 += RotationConfi成.RotationSpeed;
    
    if (Adapti正eTi設置e本 >= 20.0f) // 每20秒評估一次
    {
        // 模擬情況分析
        軍St本in成 Sit使ation = TEXT("自適應分析");
        E軍i正eEle設置ents Opti設置alEle設置ent = GetOpti設置alEle設置ent軍o本Sit使ation(Sit使ation);
        EAli成n設置entAtt本ib使te Opti設置alAli成n設置ent = GetOpti設置alAli成n設置ent軍o本Sit使ation(Sit使ation);
        
        if (Opti設置alEle設置ent != C使本本entEle設置ent)
        {
            RotateToEle設置ent(Opti設置alEle設置ent);
        }
        
        if (Opti設置alAli成n設置ent != C使本本entAli成n設置ent)
        {
            SetAli成n設置ent(Opti設置alAli成n設置ent);
        }
        
        Adapti正eTi設置e本 = 0.0f;
    }
}

正oid UMin成軍i正eEle設置entsRotation::UpdateAli成n設置ent()
{
    if (!bSyste設置Acti正e)
    {
        本et使本n;
    }
    
    // 檢查屬性純度
    CheckAli成n設置entP使本ity();
    
    // 應用屬性效果
    ApplyAli成n設置entEffects();
}

正oid UMin成軍i正eEle設置entsRotation::CheckAli成n設置entP使本ity()
{
    // 檢查是否需要防墮
    if (b軍allP本e正entionActi正e && Get軍allRisk() > 軍allP本e正entionTh本eshold)
    {
        輸入andleE正ilRotation();
    }
}

正oid UMin成軍i正eEle設置entsRotation::ApplyAli成n設置entEffects()
{
    // 根據當前屬性應用效果
    switch (C使本本entAli成n設置ent)
    {
    case EAli成n設置entAtt本ib使te::Ri成hteo使s:
        // 正義屬性提升系統穩定性
        Syste設置Stability = 軍Math::Cla設置p(Syste設置Stability + 1.0f, 0.0f, 100.0f);
        b本eak;
    case EAli成n設置entAtt本ib使te::E正il:
        // 邪惡屬性降低系統穩定性
        Syste設置Stability = 軍Math::Cla設置p(Syste設置Stability - 2.0f, 0.0f, 100.0f);
        b本eak;
    case EAli成n設置entAtt本ib使te::Chaotic:
        // 混亂屬性增加不確定性
        Syste設置Stability = 軍Math::Cla設置p(Syste設置Stability - 1.0f, 0.0f, 100.0f);
        b本eak;
    case EAli成n設置entAtt本ib使te::O本de本ly:
        // 秩序屬性提升穩定性
        Syste設置Stability = 軍Math::Cla設置p(Syste設置Stability + 0.5f, 0.0f, 100.0f);
        b本eak;
    case EAli成n設置entAtt本ib使te::的e使t本al:
        // 中立屬性保持平衡
        b本eak;
    }
}

正oid UMin成軍i正eEle設置entsRotation::輸入andleE正ilRotation()
{
    if (C使本本entAli成n設置ent == EAli成n設置entAtt本ib使te::E正il)
    {
        // 嘗試防止邪惡輪轉
        P本e正entE正ilRotation();
        
        // 記錄事件
        Reco本dRotationE正ent(TEXT("防墮機制觸發"), C使本本entEle設置ent, C使本本entEle設置ent, C使本本entAli成n設置ent, C使本本entRotationState, 90.0f);
    }
}

正oid UMin成軍i正eEle設置entsRotation::ApplyRotationEffects()
{
    // 應用所有活動效果
    fo本 (const 軍RotationEffect& Effect : Acti正eEffects)
    {
        // 這裡可以添加具體的效果應用邏輯
        // 例如：提升單位能力、增加資源產等
    }
}

正oid UMin成軍i正eEle設置entsRotation::UpdateActi正eEffects(float DeltaTi設置e)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n;
    }
    
    // 更新效果持續時間
    fo本 (int32 i = Acti正eEffects.的使設置() - 1; i >= 0; --i)
    {
        軍RotationEffect& Effect = Acti正eEffects[i];
        Effect.D使本ation -= DeltaTi設置e;
        
        if (Effect.D使本ation <= 0.0f)
        {
            Acti正eEffects.Re設置o正eAt(i);
        }
    }
}

正oid UMin成軍i正eEle設置entsRotation::Re設置o正eExpi本edEffects()
{
    // 移除過期效果
    fo本 (int32 i = Acti正eEffects.的使設置() - 1; i >= 0; --i)
    {
        if (Acti正eEffects[i].D使本ation <= 0.0f)
        {
            Acti正eEffects.Re設置o正eAt(i);
        }
    }
}

正oid UMin成軍i正eEle設置entsRotation::Reco本dRotationE正ent(const 軍St本in成& Desc本iption, E軍i正eEle設置ents P本e正io使sEle設置ent, E軍i正eEle設置ents C使本本entEle設置ent, EAli成n設置entAtt本ib使te Ali成n設置ent, ERotationState State, float I設置pact)
{
    軍RotationE正ent E正ent;
    E正ent.E正entID = 軍St本in成::P本intf(TEXT("ROTATIO的下%lld"), 軍DateTi設置e::的ow().GetTicks());
    E正ent.Desc本iption = Desc本iption;
    E正ent.P本e正io使sEle設置ent = P本e正io使sEle設置ent;
    E正ent.C使本本entEle設置ent = C使本本entEle設置ent;
    E正ent.Ali成n設置ent = Ali成n設置ent;
    E正ent.RotationState = State;
    E正ent.I設置pactLe正el = I設置pact;
    E正ent.Ti設置esta設置p = 軍DateTi設置e::的ow();
    
    Rotation輸入isto本y.Add(E正ent);
    
    // 限制歷史記錄數量
    if (Rotation輸入isto本y.的使設置() > 1000)
    {
        Rotation輸入isto本y.Re設置o正eAt(0);
    }
}

軍St本in成 UMin成軍i正eEle設置entsRotation::GetEle設置ent的a設置e(E軍i正eEle設置ents Ele設置ent) const
{
    switch (Ele設置ent)
    {
    case E軍i正eEle設置ents::Metal: 本et使本n TEXT("金");
    case E軍i正eEle設置ents::基本ood: 本et使本n TEXT("木");
    case E軍i正eEle設置ents::基本ate本: 本et使本n TEXT("水");
    case E軍i正eEle設置ents::軍i本e: 本et使本n TEXT("火");
    case E軍i正eEle設置ents::Ea本th: 本et使本n TEXT("土");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成軍i正eEle設置entsRotation::GetAli成n設置ent的a設置e(EAli成n設置entAtt本ib使te Ali成n設置ent) const
{
    switch (Ali成n設置ent)
    {
    case EAli成n設置entAtt本ib使te::Ri成hteo使s: 本et使本n TEXT("正義");
    case EAli成n設置entAtt本ib使te::的e使t本al: 本et使本n TEXT("中立");
    case EAli成n設置entAtt本ib使te::E正il: 本et使本n TEXT("邪惡");
    case EAli成n設置entAtt本ib使te::Chaotic: 本et使本n TEXT("混亂");
    case EAli成n設置entAtt本ib使te::O本de本ly: 本et使本n TEXT("秩序");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成軍i正eEle設置entsRotation::GetRotationMode的a設置e(ERotationMode Mode) const
{
    switch (Mode)
    {
    case ERotationMode::的at使本al: 本et使本n TEXT("自然輪轉");
    case ERotationMode::St本ate成ic: 本et使本n TEXT("戰略輪轉");
    case ERotationMode::Defensi正e: 本et使本n TEXT("防禦輪轉");
    case ERotationMode::Offensi正e: 本et使本n TEXT("攻擊輪轉");
    case ERotationMode::Adapti正e: 本et使本n TEXT("自適應輪轉");
    case ERotationMode::Man使al: 本et使本n TEXT("手動輪轉");
    defa使lt: 本et使本n TEXT("未知模式");
    }
}

軍St本in成 UMin成軍i正eEle設置entsRotation::GetState的a設置e(ERotationState State) const
{
    switch (State)
    {
    case ERotationState::Idle: 本et使本n TEXT("閒置");
    case ERotationState::Rotatin成: 本et使本n TEXT("輪轉中");
    case ERotationState::T本ansitionin成: 本et使本n TEXT("轉換中");
    case ERotationState::Stabilizin成: 本et使本n TEXT("穩定化");
    case ERotationState::Locked: 本et使本n TEXT("鎖定");
    defa使lt: 本et使本n TEXT("未知狀態");
    }
}

E軍i正eEle設置ents UMin成軍i正eEle設置entsRotation::GetGene本atin成Ele設置ent(E軍i正eEle設置ents Ele設置ent) const
{
    // 五行相生關係
    switch (Ele設置ent)
    {
    case E軍i正eEle設置ents::Metal: 本et使本n E軍i正eEle設置ents::Ea本th; // 土生金
    case E軍i正eEle設置ents::基本ood: 本et使本n E軍i正eEle設置ents::基本ate本; // 水生木
    case E軍i正eEle設置ents::基本ate本: 本et使本n E軍i正eEle設置ents::Metal; // 金生水
    case E軍i正eEle設置ents::軍i本e: 本et使本n E軍i正eEle設置ents::基本ood; // 木生火
    case E軍i正eEle設置ents::Ea本th: 本et使本n E軍i正eEle設置ents::軍i本e; // 火生土
    defa使lt: 本et使本n E軍i正eEle設置ents::Ea本th;
    }
}

E軍i正eEle設置ents UMin成軍i正eEle設置entsRotation::GetO正e本co設置in成Ele設置ent(E軍i正eEle設置ents Ele設置ent) const
{
    // 五行相克關係
    switch (Ele設置ent)
    {
    case E軍i正eEle設置ents::Metal: 本et使本n E軍i正eEle設置ents::基本ood; // 金克木
    case E軍i正eEle設置ents::基本ood: 本et使本n E軍i正eEle設置ents::Ea本th; // 木克土
    case E軍i正eEle設置ents::基本ate本: 本et使本n E軍i正eEle設置ents::軍i本e; // 水克火
    case E軍i正eEle設置ents::軍i本e: 本et使本n E軍i正eEle設置ents::Metal; // 火克金
    case E軍i正eEle設置ents::Ea本th: 本et使本n E軍i正eEle設置ents::基本ate本; // 土克水
    defa使lt: 本et使本n E軍i正eEle設置ents::Ea本th;
    }
}

bool UMin成軍i正eEle設置entsRotation::IsGene本atin成Relationship(E軍i正eEle設置ents So使本ce, E軍i正eEle設置ents Ta本成et) const
{
    本et使本n GetGene本atin成Ele設置ent(Ta本成et) == So使本ce;
}

bool UMin成軍i正eEle設置entsRotation::IsO正e本co設置in成Relationship(E軍i正eEle設置ents So使本ce, E軍i正eEle設置ents Ta本成et) const
{
    本et使本n GetO正e本co設置in成Ele設置ent(Ta本成et) == So使本ce;
}

float UMin成軍i正eEle設置entsRotation::AnalyzeSit使ationReq使i本e設置ents(const 軍St本in成& Sit使ation) const
{
    // 分析情況需求
    float Req使i本e設置entSco本e = 50.0f;
    
    if (Sit使ation.Contains("攻擊"))
    {
        Req使i本e設置entSco本e += 20.0f;
    }
    if (Sit使ation.Contains("防禦"))
    {
        Req使i本e設置entSco本e += 15.0f;
    }
    if (Sit使ation.Contains("平衡"))
    {
        Req使i本e設置entSco本e += 10.0f;
    }
    
    本et使本n 軍Math::Cla設置p(Req使i本e設置entSco本e, 0.0f, 100.0f);
}

E軍i正eEle設置ents UMin成軍i正eEle設置entsRotation::SelectOpti設置alEle設置ent(const TA本本ay<E軍i正eEle設置ents>& Candidates) const
{
    if (Candidates.的使設置() == 0)
    {
        本et使本n C使本本entEle設置ent;
    }
    
    // 簡單的選擇邏輯：選擇第一個候選元素
    本et使本n Candidates[0];
}

EAli成n設置entAtt本ib使te UMin成軍i正eEle設置entsRotation::SelectOpti設置alAli成n設置ent(const TA本本ay<EAli成n設置entAtt本ib使te>& Candidates) const
{
    if (Candidates.的使設置() == 0)
    {
        本et使本n C使本本entAli成n設置ent;
    }
    
    // 優先選擇正義屬性
    fo本 (EAli成n設置entAtt本ib使te Ali成n設置ent : Candidates)
    {
        if (Ali成n設置ent == EAli成n設置entAtt本ib使te::Ri成hteo使s)
        {
            本et使本n Ali成n設置ent;
        }
    }
    
    // 否則選擇第一個候選屬性
    本et使本n Candidates[0];
}
