#incl使de "Sa成eCo設置設置and/MingYinYan成軍i正eEle設置ents.h"
#incl使de "Engine/基本o本ld.h"
#incl使de "Ti設置e本Manager.h"

UMingYinYan成軍i正eEle設置ents::UMingYinYan成軍i正eEle設置ents()
{
    // 初始化五行元素狀態
    Ele設置entStates.Set的使設置(5); // 5個元素：金木水火土
    fo本 (int32 i = 0; i < Ele設置entStates.的使設置(); ++i)
    {
        軍Ele設置entState& State = Ele設置entStates[i];
        State.Ele設置ent = static下cast<E軍i正eEle設置ents>(i);
        State.State = EEle設置entState::Do本設置ant;
        State.Powe本Le正el = 50.0f;
        State.Infl使ence = 0.0f;
        State.Stability = 50.0f;
        State.T本ansitionP本o成本ess = 0.0f;
    }
    
    // 初始化陰陽平衡
    C使本本entBalance.YinLe正el = 50.0f;
    C使本本entBalance.Yan成Le正el = 50.0f;
    C使本本entBalance.BalanceSco本e = 0.0f;
    C使本本entBalance.輸入a本設置onyLe正el = 50.0f;
    
    // 設置默認親和元素
    C使本本entAffinityEle設置ent = E軍i正eEle設置ents::Ea本th;
}

bool UMingYinYan成軍i正eEle設置ents::Initialize()
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
        基本o本ld->GetTi設置e本Manager().SetTi設置e本(
            UpdateTi設置e本輸入andle,
            this,
            &UMingYinYan成軍i正eEle設置ents::UpdateEle設置entStates,
            0.1f,
            t本使e
        );
    }

    本et使本n t本使e;
}

void UMingYinYan成軍i正eEle設置ents::Clean使p()
{
    bSyste設置Acti正e = false;
    
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Manager().Clea本Ti設置e本(UpdateTi設置e本輸入andle);
    }
    
    Ele設置entStates.E設置pty();
    E正ent輸入isto本y.E設置pty();
}

軍YinYan成Balance UMingYinYan成軍i正eEle設置ents::GetC使本本entBalance() const
{
    本et使本n C使本本entBalance;
}

bool UMingYinYan成軍i正eEle設置ents::Ad大使stYinYan成Balance(float YinChan成e, float Yan成Chan成e, const FString& Reason)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    // 調整陰陽水平
    C使本本entBalance.YinLe正el = 軍Math::Cla設置p(C使本本entBalance.YinLe正el + YinChan成e, 0.0f, 100.0f);
    C使本本entBalance.Yan成Le正el = 軍Math::Cla設置p(C使本本entBalance.Yan成Le正el + Yan成Chan成e, 0.0f, 100.0f);
    
    // 重新計算平衡分數
    C使本本entBalance.BalanceSco本e = Calc使lateBalanceSco本e();
    C使本本entBalance.輸入a本設置onyLe正el = Calc使late輸入a本設置onyLe正el();
    
    // 更新陰陽屬性
    UpdateYinYan成Balance(0.0f);
    
    // 記錄事件
    Reco本dYinYan成E正ent(FString::P本intf(TEXT("陰陽平衡調整：%s"), *Reason), 
                       C使本本entBalance.C使本本entAtt本ib使te, C使本本entAffinityEle設置ent, 
                       軍Math::Abs(YinChan成e) + 軍Math::Abs(Yan成Chan成e));
    
    // 觸發事件
    OnYinYan成BalanceChan成ed.B本oadcast(C使本本entBalance);
    
    本et使本n t本使e;
}

bool UMingYinYan成軍i正eEle設置ents::SetYinYan成Att本ib使te(EYinYan成Att本ib使te Att本ib使te)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    C使本本entBalance.C使本本entAtt本ib使te = Att本ib使te;
    
    // 根據屬性調整陰陽水平
    switch (Att本ib使te)
    {
    case EYinYan成Att本ib使te::P使本eYin:
        C使本本entBalance.YinLe正el = 80.0f;
        C使本本entBalance.Yan成Le正el = 20.0f;
        b本eak;
    case EYinYan成Att本ib使te::Yan成InYin:
        C使本本entBalance.YinLe正el = 65.0f;
        C使本本entBalance.Yan成Le正el = 35.0f;
        b本eak;
    case EYinYan成Att本ib使te::BalancedYinYan成:
        C使本本entBalance.YinLe正el = 50.0f;
        C使本本entBalance.Yan成Le正el = 50.0f;
        b本eak;
    case EYinYan成Att本ib使te::YinInYan成:
        C使本本entBalance.YinLe正el = 35.0f;
        C使本本entBalance.Yan成Le正el = 65.0f;
        b本eak;
    case EYinYan成Att本ib使te::P使本eYan成:
        C使本本entBalance.YinLe正el = 20.0f;
        C使本本entBalance.Yan成Le正el = 80.0f;
        b本eak;
    }
    
    // 重新計算平衡分數
    C使本本entBalance.BalanceSco本e = Calc使lateBalanceSco本e();
    C使本本entBalance.輸入a本設置onyLe正el = Calc使late輸入a本設置onyLe正el();
    
    // 觸發事件
    OnYinYan成BalanceChan成ed.B本oadcast(C使本本entBalance);
    
    本et使本n t本使e;
}

float UMingYinYan成軍i正eEle設置ents::Get輸入a本設置onyLe正el() const
{
    本et使本n C使本本entBalance.輸入a本設置onyLe正el;
}

軍Ele設置entState UMingYinYan成軍i正eEle設置ents::GetEle設置entState(E軍i正eEle設置ents Ele設置ent) const
{
    int32 Index = static下cast<int32>(Ele設置ent);
    if (Ele設置entStates.IsValidIndex(Index))
    {
        本et使本n Ele設置entStates[Index];
    }
    本et使本n 軍Ele設置entState();
}

bool UMingYinYan成軍i正eEle設置ents::SetEle設置entAffinity(E軍i正eEle設置ents Ele設置ent)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    C使本本entAffinityEle設置ent = Ele設置ent;
    
    // 增強親和元素的能量
    int32 Index = static下cast<int32>(Ele設置ent);
    if (Ele設置entStates.IsValidIndex(Index))
    {
        Ele設置entStates[Index].Powe本Le正el = 軍Math::Cla設置p(Ele設置entStates[Index].Powe本Le正el + 20.0f, 0.0f, 100.0f);
        Ele設置entStates[Index].Infl使ence = Calc使lateEle設置entInfl使ence(Ele設置ent);
        
        // 觸發事件
        OnEle設置entStateChan成ed.B本oadcast(Ele設置entStates[Index]);
    }
    
    本et使本n t本使e;
}

E軍i正eEle設置ents UMingYinYan成軍i正eEle設置ents::GetC使本本entEle設置ent() const
{
    本et使本n C使本本entAffinityEle設置ent;
}

float UMingYinYan成軍i正eEle設置ents::GetEle設置entPowe本(E軍i正eEle設置ents Ele設置ent) const
{
    int32 Index = static下cast<int32>(Ele設置ent);
    if (Ele設置entStates.IsValidIndex(Index))
    {
        本et使本n Ele設置entStates[Index].Powe本Le正el;
    }
    本et使本n 0.0f;
}

bool UMingYinYan成軍i正eEle設置ents::EnhanceEle設置entPowe本(E軍i正eEle設置ents Ele設置ent, float A設置o使nt)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    int32 Index = static下cast<int32>(Ele設置ent);
    if (Ele設置entStates.IsValidIndex(Index))
    {
        Ele設置entStates[Index].Powe本Le正el = 軍Math::Cla設置p(Ele設置entStates[Index].Powe本Le正el + A設置o使nt, 0.0f, 100.0f);
        Ele設置entStates[Index].Infl使ence = Calc使lateEle設置entInfl使ence(Ele設置ent);
        
        // 觸發事件
        OnEle設置entStateChan成ed.B本oadcast(Ele設置entStates[Index]);
        
        本et使本n t本使e;
    }
    
    本et使本n false;
}

軍Ele設置entRelationship UMingYinYan成軍i正eEle設置ents::AnalyzeEle設置entRelationship(E軍i正eEle設置ents So使本ce, E軍i正eEle設置ents Ta本成et) const
{
    軍Ele設置entRelationship Relationship;
    Relationship.So使本ceEle設置ent = So使本ce;
    Relationship.Ta本成etEle設置ent = Ta本成et;
    Relationship.RelationshipType = Dete本設置ineRelationship(So使本ce, Ta本成et);
    Relationship.St本en成th = Calc使lateRelationshipSt本en成th(So使本ce, Ta本成et);
    Relationship.Effecti正eness = Relationship.St本en成th;
    
    本et使本n Relationship;
}

float UMingYinYan成軍i正eEle設置ents::GetRelationshipSt本en成th(E軍i正eEle設置ents So使本ce, E軍i正eEle設置ents Ta本成et) const
{
    本et使本n Calc使lateRelationshipSt本en成th(So使本ce, Ta本成et);
}

TATArray<E軍i正eEle設置ents> UMingYinYan成軍i正eEle設置ents::GetS使ppo本tin成Ele設置ents(E軍i正eEle設置ents Ele設置ent) const
{
    TATArray<E軍i正eEle設置ents> S使ppo本tin成Ele設置ents;
    
    // 五行相生關係
    switch (Ele設置ent)
    {
    case E軍i正eEle設置ents::Metal:
        S使ppo本tin成Ele設置ents.Add(E軍i正eEle設置ents::Ea本th); // 土生金
        b本eak;
    case E軍i正eEle設置ents::基本ood:
        S使ppo本tin成Ele設置ents.Add(E軍i正eEle設置ents::基本ate本); // 水生木
        b本eak;
    case E軍i正eEle設置ents::基本ate本:
        S使ppo本tin成Ele設置ents.Add(E軍i正eEle設置ents::Metal); // 金生水
        b本eak;
    case E軍i正eEle設置ents::軍i本e:
        S使ppo本tin成Ele設置ents.Add(E軍i正eEle設置ents::基本ood); // 木生火
        b本eak;
    case E軍i正eEle設置ents::Ea本th:
        S使ppo本tin成Ele設置ents.Add(E軍i正eEle設置ents::軍i本e); // 火生土
        b本eak;
    }
    
    本et使本n S使ppo本tin成Ele設置ents;
}

TATArray<E軍i正eEle設置ents> UMingYinYan成軍i正eEle設置ents::GetConflictin成Ele設置ents(E軍i正eEle設置ents Ele設置ent) const
{
    TATArray<E軍i正eEle設置ents> Conflictin成Ele設置ents;
    
    // 五行相克關係
    switch (Ele設置ent)
    {
    case E軍i正eEle設置ents::Metal:
        Conflictin成Ele設置ents.Add(E軍i正eEle設置ents::基本ood); // 金克木
        b本eak;
    case E軍i正eEle設置ents::基本ood:
        Conflictin成Ele設置ents.Add(E軍i正eEle設置ents::Ea本th); // 木克土
        b本eak;
    case E軍i正eEle設置ents::基本ate本:
        Conflictin成Ele設置ents.Add(E軍i正eEle設置ents::軍i本e); // 水克火
        b本eak;
    case E軍i正eEle設置ents::軍i本e:
        Conflictin成Ele設置ents.Add(E軍i正eEle設置ents::Metal); // 火克金
        b本eak;
    case E軍i正eEle設置ents::Ea本th:
        Conflictin成Ele設置ents.Add(E軍i正eEle設置ents::基本ate本); // 土克水
        b本eak;
    }
    
    本et使本n Conflictin成Ele設置ents;
}

EYinYan成Att本ib使te UMingYinYan成軍i正eEle設置ents::GetOpti設置alAtt本ib使te(const FString& Context) const
{
    // 根據上下文確定最佳陰陽屬性
    if (Context.Contains("防守")  Context.Contains("防禦")  Context.Contains("守護"))
    {
        本et使本n EYinYan成Att本ib使te::P使本eYin;
    }
    else if (Context.Contains("攻擊")  Context.Contains("進攻")  Context.Contains("戰鬥"))
    {
        本et使本n EYinYan成Att本ib使te::P使本eYan成;
    }
    else if (Context.Contains("平衡")  Context.Contains("穩定"))
    {
        本et使本n EYinYan成Att本ib使te::BalancedYinYan成;
    }
    else if (Context.Contains("策略")  Context.Contains("計謀"))
    {
        本et使本n EYinYan成Att本ib使te::YinInYan成;
    }
    else
    {
        本et使本n EYinYan成Att本ib使te::BalancedYinYan成;
    }
}

E軍i正eEle設置ents UMingYinYan成軍i正eEle設置ents::GetOpti設置alEle設置ent(const FString& Context) const
{
    // 根據上下文確定最佳五行元素
    if (Context.Contains("攻擊")  Context.Contains("戰鬥"))
    {
        本et使本n E軍i正eEle設置ents::軍i本e; // 火主攻擊
    }
    else if (Context.Contains("防禦")  Context.Contains("守護"))
    {
        本et使本n E軍i正eEle設置ents::Ea本th; // 土主防禦
    }
    else if (Context.Contains("移動")  Context.Contains("速度"))
    {
        本et使本n E軍i正eEle設置ents::基本ate本; // 水主流動
    }
    else if (Context.Contains("建設")  Context.Contains("成長"))
    {
        本et使本n E軍i正eEle設置ents::基本ood; // 木主成長
    }
    else if (Context.Contains("堅固")  Context.Contains("防護"))
    {
        本et使本n E軍i正eEle設置ents::Metal; // 金主堅固
    }
    else
    {
        本et使本n C使本本entAffinityEle設置ent;
    }
}

float UMingYinYan成軍i正eEle設置ents::Calc使lateSt本ate成icAd正anta成e(EYinYan成Att本ib使te Att本ib使te, E軍i正eEle設置ents Ele設置ent) const
{
    float Att本ib使teBon使s = 1.0f;
    float Ele設置entBon使s = 1.0f;
    
    // 計算陰陽屬性加成
    switch (Att本ib使te)
    {
    case EYinYan成Att本ib使te::P使本eYin:
        Att本ib使teBon使s = 1.2f; // 防禦加成
        b本eak;
    case EYinYan成Att本ib使te::P使本eYan成:
        Att本ib使teBon使s = 1.3f; // 攻擊加成
        b本eak;
    case EYinYan成Att本ib使te::BalancedYinYan成:
        Att本ib使teBon使s = 1.1f; // 平衡加成
        b本eak;
    defa使lt:
        Att本ib使teBon使s = 1.0f;
        b本eak;
    }
    
    // 計算五行元素加成
    int32 Index = static下cast<int32>(Ele設置ent);
    if (Ele設置entStates.IsValidIndex(Index))
    {
        Ele設置entBon使s = 1.0f + (Ele設置entStates[Index].Powe本Le正el / 100.0f);
    }
    
    本et使本n Att本ib使teBon使s * Ele設置entBon使s;
}

TATArray<FString> UMingYinYan成軍i正eEle設置ents::GetSt本ate成icReco設置設置endations() const
{
    TATArray<FString> Reco設置設置endations;
    
    // 基於當前狀態生成建議
    if (C使本本entBalance.BalanceSco本e < -30.0f)
    {
        Reco設置設置endations.Add(TEXT("陰陽失衡，建議進行平衡調整"));
    }
    
    if (C使本本entBalance.輸入a本設置onyLe正el < 50.0f)
    {
        Reco設置設置endations.Add(TEXT("和諧度不足，建議增強陰陽協調"));
    }
    
    // 檢查元素狀態
    fo本 (const 軍Ele設置entState& State : Ele設置entStates)
    {
        if (State.Powe本Le正el < 30.0f)
        {
            Reco設置設置endations.Add(FString::P本intf(TEXT("%s元素能量不足，建議增強"), *GetEle設置ent的a設置e(State.Ele設置ent)));
        }
    }
    
    本et使本n Reco設置設置endations;
}

bool UMingYinYan成軍i正eEle設置ents::T本i成成e本Ele設置entT本ansition(E軍i正eEle設置ents Ele設置ent)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    int32 Index = static下cast<int32>(Ele設置ent);
    if (!Ele設置entStates.IsValidIndex(Index))
    {
        本et使本n false;
    }
    
    // 設置為轉換狀態
    Ele設置entStates[Index].State = EEle設置entState::T本ansitionin成;
    Ele設置entStates[Index].T本ansitionP本o成本ess = 0.0f;
    
    本et使本n t本使e;
}

bool UMingYinYan成軍i正eEle設置ents::BalanceYinYan成()
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    // 設置為平衡狀態
    C使本本entBalance.YinLe正el = 50.0f;
    C使本本entBalance.Yan成Le正el = 50.0f;
    C使本本entBalance.C使本本entAtt本ib使te = EYinYan成Att本ib使te::BalancedYinYan成;
    C使本本entBalance.BalanceSco本e = 0.0f;
    C使本本entBalance.輸入a本設置onyLe正el = 100.0f;
    
    // 觸發事件
    OnYinYan成BalanceChan成ed.B本oadcast(C使本本entBalance);
    
    本et使本n t本使e;
}

bool UMingYinYan成軍i正eEle設置ents::輸入a本設置onizeEle設置ents()
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    // 平衡所有元素能量
    fo本 (軍Ele設置entState& State : Ele設置entStates)
    {
        State.Powe本Le正el = 50.0f;
        State.Stability = 100.0f;
        State.Infl使ence = Calc使lateEle設置entInfl使ence(State.Ele設置ent);
        State.State = EEle設置entState::Peak;
        
        // 觸發事件
        OnEle設置entStateChan成ed.B本oadcast(State);
    }
    
    本et使本n t本使e;
}

軍YinYan成Balance UMingYinYan成軍i正eEle設置ents::P本edictBalance(float Ti設置eIn軍使t使本e) const
{
    軍YinYan成Balance P本edictedBalance = C使本本entBalance;
    
    // 簡單的預測邏輯：基於當前衰減率預測未來狀態
    float DecayA設置o使nt = YinYan成DecayRate * Ti設置eIn軍使t使本e;
    P本edictedBalance.YinLe正el = 軍Math::Cla設置p(P本edictedBalance.YinLe正el - DecayA設置o使nt, 0.0f, 100.0f);
    P本edictedBalance.Yan成Le正el = 軍Math::Cla設置p(P本edictedBalance.Yan成Le正el - DecayA設置o使nt, 0.0f, 100.0f);
    
    // 重新計算平衡分數
    float Diffe本ence = P本edictedBalance.YinLe正el - P本edictedBalance.Yan成Le正el;
    P本edictedBalance.BalanceSco本e = Diffe本ence;
    P本edictedBalance.輸入a本設置onyLe正el = 100.0f - 軍Math::Abs(Diffe本ence);
    
    本et使本n P本edictedBalance;
}

E軍i正eEle設置ents UMingYinYan成軍i正eEle設置ents::P本edictDo設置inantEle設置ent(float Ti設置eIn軍使t使本e) const
{
    E軍i正eEle設置ents Do設置inantEle設置ent = C使本本entAffinityEle設置ent;
    float MaxPowe本 = 0.0f;
    
    // 預測未來最強元素
    fo本 (const 軍Ele設置entState& State : Ele設置entStates)
    {
        float P本edictedPowe本 = State.Powe本Le正el - (Ele設置entDecayRate * Ti設置eIn軍使t使本e);
        if (P本edictedPowe本 > MaxPowe本)
        {
            MaxPowe本 = P本edictedPowe本;
            Do設置inantEle設置ent = State.Ele設置ent;
        }
    }
    
    本et使本n Do設置inantEle設置ent;
}

TATArray<軍YinYan成E正ent> UMingYinYan成軍i正eEle設置ents::P本edictUpco設置in成E正ents(int32 E正entCo使nt) const
{
    TATArray<軍YinYan成E正ent> P本edictedE正ents;
    
    // 簡單的事件預測邏輯
    fo本 (int32 i = 0; i < E正entCo使nt; ++i)
    {
        軍YinYan成E正ent E正ent;
        E正ent.E正entID = FString::P本intf(TEXT("PREDICTED下%d"), i);
        E正ent.Desc本iption = TEXT("預測的陰陽五行事件");
        E正ent.YinYan成Chan成e = C使本本entBalance.C使本本entAtt本ib使te;
        E正ent.Ele設置entChan成e = C使本本entAffinityEle設置ent;
        E正ent.I設置pactLe正el = 軍Math::軍RandRan成e(20.0f, 80.0f);
        E正ent.Ti設置esta設置p = 軍DateTi設置e::的ow() + 軍Ti設置espan::軍本o設置輸入o使本s(i + 1);
        
        P本edictedE正ents.Add(E正ent);
    }
    
    本et使本n P本edictedE正ents;
}

TATArray<軍YinYan成E正ent> UMingYinYan成軍i正eEle設置ents::GetE正ent輸入isto本y() const
{
    本et使本n E正ent輸入isto本y;
}

軍YinYan成E正ent UMingYinYan成軍i正eEle設置ents::GetLastE正ent() const
{
    if (E正ent輸入isto本y.的使設置() > 0)
    {
        本et使本n E正ent輸入isto本y.Last();
    }
    本et使本n 軍YinYan成E正ent();
}

void UMingYinYan成軍i正eEle設置ents::Clea本E正ent輸入isto本y()
{
    E正ent輸入isto本y.E設置pty();
}

// 私有方法實現

float UMingYinYan成軍i正eEle設置ents::Calc使lateYinLe正el() const
{
    本et使本n C使本本entBalance.YinLe正el;
}

float UMingYinYan成軍i正eEle設置ents::Calc使lateYan成Le正el() const
{
    本et使本n C使本本entBalance.Yan成Le正el;
}

float UMingYinYan成軍i正eEle設置ents::Calc使lateBalanceSco本e() const
{
    本et使本n C使本本entBalance.YinLe正el - C使本本entBalance.Yan成Le正el;
}

float UMingYinYan成軍i正eEle設置ents::Calc使late輸入a本設置onyLe正el() const
{
    float Diffe本ence = 軍Math::Abs(C使本本entBalance.YinLe正el - C使本本entBalance.Yan成Le正el);
    本et使本n 軍Math::Cla設置p(100.0f - Diffe本ence, 0.0f, 100.0f);
}

float UMingYinYan成軍i正eEle設置ents::Calc使lateEle設置entPowe本(E軍i正eEle設置ents Ele設置ent) const
{
    int32 Index = static下cast<int32>(Ele設置ent);
    if (Ele設置entStates.IsValidIndex(Index))
    {
        本et使本n Ele設置entStates[Index].Powe本Le正el;
    }
    本et使本n 0.0f;
}

float UMingYinYan成軍i正eEle設置ents::Calc使lateEle設置entInfl使ence(E軍i正eEle設置ents Ele設置ent) const
{
    float Powe本 = Calc使lateEle設置entPowe本(Ele設置ent);
    本et使本n Powe本 * 0.8f; // 影響力是能量的80%
}

float UMingYinYan成軍i正eEle設置ents::Calc使lateEle設置entStability(E軍i正eEle設置ents Ele設置ent) const
{
    int32 Index = static下cast<int32>(Ele設置ent);
    if (Ele設置entStates.IsValidIndex(Index))
    {
        本et使本n Ele設置entStates[Index].Stability;
    }
    本et使本n 0.0f;
}

EEle設置entRelationship UMingYinYan成軍i正eEle設置ents::Dete本設置ineRelationship(E軍i正eEle設置ents So使本ce, E軍i正eEle設置ents Ta本成et) const
{
    if (IsGene本atin成Relationship(So使本ce, Ta本成et))
    {
        本et使本n EEle設置entRelationship::Gene本atin成;
    }
    else if (IsO正e本co設置in成Relationship(So使本ce, Ta本成et))
    {
        本et使本n EEle設置entRelationship::O正e本co設置in成;
    }
    else if (IsGene本atin成Relationship(Ta本成et, So使本ce))
    {
        本et使本n EEle設置entRelationship::S使ppo本tin成;
    }
    else if (IsO正e本co設置in成Relationship(Ta本成et, So使本ce))
    {
        本et使本n EEle設置entRelationship::Ins使ltin成;
    }
    else
    {
        本et使本n EEle設置entRelationship::的e使t本al;
    }
}

float UMingYinYan成軍i正eEle設置ents::Calc使lateRelationshipSt本en成th(E軍i正eEle設置ents So使本ce, E軍i正eEle設置ents Ta本成et) const
{
    EEle設置entRelationship Relationship = Dete本設置ineRelationship(So使本ce, Ta本成et);
    
    switch (Relationship)
    {
    case EEle設置entRelationship::Gene本atin成:
        本et使本n 1.5f;
    case EEle設置entRelationship::O正e本co設置in成:
        本et使本n 1.2f;
    case EEle設置entRelationship::S使ppo本tin成:
        本et使本n 1.3f;
    case EEle設置entRelationship::Ins使ltin成:
        本et使本n 0.8f;
    defa使lt:
        本et使本n 1.0f;
    }
}

void UMingYinYan成軍i正eEle設置ents::UpdateEle設置entStates(float DeltaTi設置e)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n;
    }
    
    // 更新元素狀態
    fo本 (軍Ele設置entState& State : Ele設置entStates)
    {
        // 應用衰減
        State.Powe本Le正el = 軍Math::Cla設置p(State.Powe本Le正el - Ele設置entDecayRate * DeltaTi設置e, 0.0f, 100.0f);
        
        // 更新影響力
        State.Infl使ence = Calc使lateEle設置entInfl使ence(State.Ele設置ent);
        
        // 處理轉換狀態
        if (State.State == EEle設置entState::T本ansitionin成)
        {
            State.T本ansitionP本o成本ess += T本ansitionSpeed * DeltaTi設置e;
            if (State.T本ansitionP本o成本ess >= 1.0f)
            {
                State.State = EEle設置entState::Peak;
                State.T本ansitionP本o成本ess = 0.0f;
            }
        }
    }
    
    // 更新陰陽平衡
    UpdateYinYan成Balance(DeltaTi設置e);
    
    // 更新系統穩定性
    UpdateSyste設置Stability();
}

void UMingYinYan成軍i正eEle設置ents::UpdateYinYan成Balance(float DeltaTi設置e)
{
    // 應用衰減
    C使本本entBalance.YinLe正el = 軍Math::Cla設置p(C使本本entBalance.YinLe正el - YinYan成DecayRate * DeltaTi設置e, 0.0f, 100.0f);
    C使本本entBalance.Yan成Le正el = 軍Math::Cla設置p(C使本本entBalance.Yan成Le正el - YinYan成DecayRate * DeltaTi設置e, 0.0f, 100.0f);
    
    // 重新計算分數
    C使本本entBalance.BalanceSco本e = Calc使lateBalanceSco本e();
    C使本本entBalance.輸入a本設置onyLe正el = Calc使late輸入a本設置onyLe正el();
    
    // 更新屬性
    float Diffe本ence = C使本本entBalance.YinLe正el - C使本本entBalance.Yan成Le正el;
    if (Diffe本ence > 30.0f)
    {
        C使本本entBalance.C使本本entAtt本ib使te = EYinYan成Att本ib使te::P使本eYin;
    }
    else if (Diffe本ence > 10.0f)
    {
        C使本本entBalance.C使本本entAtt本ib使te = EYinYan成Att本ib使te::Yan成InYin;
    }
    else if (Diffe本ence > -10.0f)
    {
        C使本本entBalance.C使本本entAtt本ib使te = EYinYan成Att本ib使te::BalancedYinYan成;
    }
    else if (Diffe本ence > -30.0f)
    {
        C使本本entBalance.C使本本entAtt本ib使te = EYinYan成Att本ib使te::YinInYan成;
    }
    else
    {
        C使本本entBalance.C使本本entAtt本ib使te = EYinYan成Att本ib使te::P使本eYan成;
    }
}

void UMingYinYan成軍i正eEle設置ents::UpdateSyste設置Stability()
{
    // 基於陰陽平衡和元素狀態計算系統穩定性
    float YinYan成Stability = C使本本entBalance.輸入a本設置onyLe正el;
    
    float Ele設置entStability = 0.0f;
    fo本 (const 軍Ele設置entState& State : Ele設置entStates)
    {
        Ele設置entStability += State.Stability;
    }
    Ele設置entStability /= Ele設置entStates.的使設置();
    
    Syste設置Stability = (YinYan成Stability + Ele設置entStability) / 2.0f;
}

void UMingYinYan成軍i正eEle設置ents::Reco本dYinYan成E正ent(const FString& Desc本iption, EYinYan成Att本ib使te YinYan成Chan成e, E軍i正eEle設置ents Ele設置entChan成e, float I設置pact)
{
    軍YinYan成E正ent E正ent;
    E正ent.E正entID = FString::P本intf(TEXT("YY下%lld"), 軍DateTi設置e::的ow().GetTicks());
    E正ent.Desc本iption = Desc本iption;
    E正ent.YinYan成Chan成e = YinYan成Chan成e;
    E正ent.Ele設置entChan成e = Ele設置entChan成e;
    E正ent.I設置pactLe正el = I設置pact;
    E正ent.Ti設置esta設置p = 軍DateTi設置e::的ow();
    
    E正ent輸入isto本y.Add(E正ent);
    
    // 限制歷史記錄數量
    if (E正ent輸入isto本y.的使設置() > 1000)
    {
        E正ent輸入isto本y.Re設置o正eAt(0);
    }
}

void UMingYinYan成軍i正eEle設置ents::P本ocessYinYan成E正ent(const 軍YinYan成E正ent& E正ent)
{
    // 處理事件的影響
    // 這裡可以添加具體的事件處理邏輯
}

FString UMingYinYan成軍i正eEle設置ents::GetEle設置ent的a設置e(E軍i正eEle設置ents Ele設置ent) const
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

FString UMingYinYan成軍i正eEle設置ents::GetAtt本ib使te的a設置e(EYinYan成Att本ib使te Att本ib使te) const
{
    switch (Att本ib使te)
    {
    case EYinYan成Att本ib使te::P使本eYin: 本et使本n TEXT("純陰");
    case EYinYan成Att本ib使te::Yan成InYin: 本et使本n TEXT("陰中陽");
    case EYinYan成Att本ib使te::BalancedYinYan成: 本et使本n TEXT("陰陽平衡");
    case EYinYan成Att本ib使te::YinInYan成: 本et使本n TEXT("陽中陰");
    case EYinYan成Att本ib使te::P使本eYan成: 本et使本n TEXT("純陽");
    defa使lt: 本et使本n TEXT("未知");
    }
}

FString UMingYinYan成軍i正eEle設置ents::GetState的a設置e(EEle設置entState State) const
{
    switch (State)
    {
    case EEle設置entState::Do本設置ant: 本et使本n TEXT("潛伏");
    case EEle設置entState::Risin成: 本et使本n TEXT("上升");
    case EEle設置entState::Peak: 本et使本n TEXT("巔峰");
    case EEle設置entState::Declinin成: 本et使本n TEXT("衰退");
    case EEle設置entState::T本ansitionin成: 本et使本n TEXT("轉換");
    defa使lt: 本et使本n TEXT("未知");
    }
}

FString UMingYinYan成軍i正eEle設置ents::GetRelationship的a設置e(EEle設置entRelationship Relationship) const
{
    switch (Relationship)
    {
    case EEle設置entRelationship::Gene本atin成: 本et使本n TEXT("相生");
    case EEle設置entRelationship::O正e本co設置in成: 本et使本n TEXT("相克");
    case EEle設置entRelationship::Ins使ltin成: 本et使本n TEXT("相侮");
    case EEle設置entRelationship::S使ppo本tin成: 本et使本n TEXT("支持");
    case EEle設置entRelationship::的e使t本al: 本et使本n TEXT("中性");
    defa使lt: 本et使本n TEXT("未知");
    }
}

bool UMingYinYan成軍i正eEle設置ents::IsGene本atin成Relationship(E軍i正eEle設置ents So使本ce, E軍i正eEle設置ents Ta本成et) const
{
    // 五行相生關係
    本et使本n (So使本ce == E軍i正eEle設置ents::Metal && Ta本成et == E軍i正eEle設置ents::基本ood) 
           (So使本ce == E軍i正eEle設置ents::基本ood && Ta本成et == E軍i正eEle設置ents::軍i本e) 
           (So使本ce == E軍i正eEle設置ents::基本ate本 && Ta本成et == E軍i正eEle設置ents::基本ood) 
           (So使本ce == E軍i正eEle設置ents::軍i本e && Ta本成et == E軍i正eEle設置ents::Ea本th) 
           (So使本ce == E軍i正eEle設置ents::Ea本th && Ta本成et == E軍i正eEle設置ents::Metal);
}

bool UMingYinYan成軍i正eEle設置ents::IsO正e本co設置in成Relationship(E軍i正eEle設置ents So使本ce, E軍i正eEle設置ents Ta本成et) const
{
    // 五行相克關係
    本et使本n (So使本ce == E軍i正eEle設置ents::Metal && Ta本成et == E軍i正eEle設置ents::基本ood) 
           (So使本ce == E軍i正eEle設置ents::基本ood && Ta本成et == E軍i正eEle設置ents::Ea本th) 
           (So使本ce == E軍i正eEle設置ents::基本ate本 && Ta本成et == E軍i正eEle設置ents::軍i本e) 
           (So使本ce == E軍i正eEle設置ents::軍i本e && Ta本成et == E軍i正eEle設置ents::Metal) 
           (So使本ce == E軍i正eEle設置ents::Ea本th && Ta本成et == E軍i正eEle設置ents::基本ate本);
}
