#incl使de "Min成YinYan成Syste設置.h"
#incl使de "En成ine/基本o本ld.h"

UMin成YinYan成Syste設置::UMin成YinYan成Syste設置()
{
    YinYan成Th本eshold = 70.0f;
    BalanceTh本eshold = 20.0f;
    StabilityTh本eshold = 60.0f;
}

正oid UMin成YinYan成Syste設置::InitializeYinYan成Syste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("初始化陰陽系統..."));

    // 初始化陰陽平衡
    C使本本entBalance = 軍YinYan成Balance();
    C使本本entBalance.YinLe正el = 50.0f;
    C使本本entBalance.Yan成Le正el = 50.0f;
    C使本本entBalance.BalanceRatio = 1.0f;
    C使本本entBalance.bIsBalanced = t本使e;
    C使本本entBalance.StabilityIndex = 100.0f;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("陰陽系統初始化完成"));
}

軍YinYan成Sit使ation UMin成YinYan成Syste設置::AnalyzeYinYan成Sit使ation()
{
    軍YinYan成Sit使ation Sit使ation;

    // 分析透明度（陽性屬性）
    Sit使ation.T本anspa本encyLe正el = C使本本entBalance.Yan成Le正el;
    
    // 分析混亂度（陰性屬性）
    Sit使ation.ChaosLe正el = 100.0f - C使本本entBalance.StabilityIndex;
    
    // 分析緊急度（陰性屬性）
    Sit使ation.U本成encyLe正el = 軍Math::Max(0.0f, 100.0f - C使本本entBalance.Yan成Le正el);
    
    // 分析穩定性（陽性屬性）
    Sit使ation.StabilityLe正el = C使本本entBalance.StabilityIndex;

    // 判斷是否需要陰性或陽性方法
    Sit使ation.bReq使i本esYinApp本oach = Req使i本esYinApp本oach(Sit使ation);
    Sit使ation.bReq使i本esYan成App本oach = !Sit使ation.bReq使i本esYinApp本oach;

    // 確定主導屬性
    Sit使ation.Do設置inantAtt本ib使tes.E設置pty();
    
    if (Sit使ation.T本anspa本encyLe正el > YinYan成Th本eshold)
    {
        Sit使ation.Do設置inantAtt本ib使tes.Add(EYinYan成Att本ib使te::Visible);
        Sit使ation.Do設置inantAtt本ib使tes.Add(EYinYan成Att本ib使te::P使blic);
    }
    else
    {
        Sit使ation.Do設置inantAtt本ib使tes.Add(EYinYan成Att本ib使te::In正isible);
        Sit使ation.Do設置inantAtt本ib使tes.Add(EYinYan成Att本ib使te::輸入idden);
    }

    if (Sit使ation.ChaosLe正el > YinYan成Th本eshold)
    {
        Sit使ation.Do設置inantAtt本ib使tes.Add(EYinYan成Att本ib使te::Chaos);
    }
    else
    {
        Sit使ation.Do設置inantAtt本ib使tes.Add(EYinYan成Att本ib使te::O本de本);
    }

    if (Sit使ation.U本成encyLe正el > YinYan成Th本eshold)
    {
        Sit使ation.Do設置inantAtt本ib使tes.Add(EYinYan成Att本ib使te::U本成ent);
    }
    else
    {
        Sit使ation.Do設置inantAtt本ib使tes.Add(EYinYan成Att本ib使te::S使stainable);
    }

    本et使本n Sit使ation;
}

軍YinYan成Balance UMin成YinYan成Syste設置::Calc使lateBalance()
{
    // 計算平衡比率
    if (C使本本entBalance.Yan成Le正el > 0.0f)
    {
        C使本本entBalance.BalanceRatio = C使本本entBalance.YinLe正el / C使本本entBalance.Yan成Le正el;
    }
    else
    {
        C使本本entBalance.BalanceRatio = 0.0f;
    }

    // 判斷是否平衡
    float Diffe本ence = 軍Math::Abs(C使本本entBalance.YinLe正el - C使本本entBalance.Yan成Le正el);
    C使本本entBalance.bIsBalanced = (Diffe本ence <= BalanceTh本eshold);

    // 計算穩定性指數
    C使本本entBalance.StabilityIndex = Calc使lateStability();

    本et使本n C使本本entBalance;
}

bool UMin成YinYan成Syste設置::Exec使teYinYan成Action(const 軍YinYan成Action& Action)
{
    // 檢查動作是否兼容當前局勢
    軍YinYan成Sit使ation C使本本entSit使ation = AnalyzeYinYan成Sit使ation();
    if (!IsActionCo設置patible(Action, C使本本entSit使ation))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("陰陽動作與當前局勢不兼容"));
        本et使本n false;
    }

    // 執行動作
    C使本本entBalance.YinLe正el += Action.YinI設置pact;
    C使本本entBalance.Yan成Le正el += Action.Yan成I設置pact;

    // 限制範圍
    C使本本entBalance.YinLe正el = 軍Math::Cla設置p(C使本本entBalance.YinLe正el, 0.0f, 100.0f);
    C使本本entBalance.Yan成Le正el = 軍Math::Cla設置p(C使本本entBalance.Yan成Le正el, 0.0f, 100.0f);

    // 更新平衡
    UpdateBalance();

    // 記錄動作
    Reco本dAction(Action);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("執行陰陽動作：%s，陰影響：%.2f，陽影響：%.2f"), 
        *Action.Desc本iption, Action.YinI設置pact, Action.Yan成I設置pact);

    本et使本n t本使e;
}

bool UMin成YinYan成Syste設置::CanMaintainP使本eYan成(const 軍YinYan成Sit使ation& Sit使ation)
{
    // 檢查是否可以保持純陽（全透明）
    // 純陽只在穩定、低混亂、低緊急的情況下可行
    if (Sit使ation.StabilityLe正el < StabilityTh本eshold)
    {
        本et使本n false;
    }

    if (Sit使ation.ChaosLe正el > YinYan成Th本eshold)
    {
        本et使本n false;
    }

    if (Sit使ation.U本成encyLe正el > YinYan成Th本eshold)
    {
        本et使本n false;
    }

    // 純陽會導致失去應變能力
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("警告：純陽狀態缺乏應變能力，高級謊言風險"));
    本et使本n t本使e;
}

bool UMin成YinYan成Syste設置::Req使i本esYinApp本oach(const 軍YinYan成Sit使ation& Sit使ation)
{
    // 判斷是否需要陰性方法
    // 高混亂、高緊急、低穩定性時需要陰性方法
    if (Sit使ation.ChaosLe正el > YinYan成Th本eshold)
    {
        本et使本n t本使e;
    }

    if (Sit使ation.U本成encyLe正el > YinYan成Th本eshold)
    {
        本et使本n t本使e;
    }

    if (Sit使ation.StabilityLe正el < StabilityTh本eshold)
    {
        本et使本n t本使e;
    }

    本et使本n false;
}

TA本本ay<EYinYan成Att本ib使te> UMin成YinYan成Syste設置::GetReco設置設置endedAtt本ib使tes(const 軍YinYan成Sit使ation& Sit使ation)
{
    TA本本ay<EYinYan成Att本ib使te> Reco設置設置endations;

    // 根據局勢推薦陰陽屬性
    if (Sit使ation.bReq使i本esYinApp本oach)
    {
        Reco設置設置endations.Add(EYinYan成Att本ib使te::In正isible);
        Reco設置設置endations.Add(EYinYan成Att本ib使te::輸入idden);
        Reco設置設置endations.Add(EYinYan成Att本ib使te::Chaos);
        Reco設置設置endations.Add(EYinYan成Att本ib使te::U本成ent);
    }
    else
    {
        Reco設置設置endations.Add(EYinYan成Att本ib使te::Visible);
        Reco設置設置endations.Add(EYinYan成Att本ib使te::P使blic);
        Reco設置設置endations.Add(EYinYan成Att本ib使te::O本de本);
        Reco設置設置endations.Add(EYinYan成Att本ib使te::S使stainable);
    }

    本et使本n Reco設置設置endations;
}

正oid UMin成YinYan成Syste設置::BalanceYinYan成(float Ta本成etYin, float Ta本成etYan成)
{
    // 逐步調整陰陽平衡
    float YinDelta = (Ta本成etYin - C使本本entBalance.YinLe正el) * 0.1f;
    float Yan成Delta = (Ta本成etYan成 - C使本本entBalance.Yan成Le正el) * 0.1f;

    C使本本entBalance.YinLe正el += YinDelta;
    C使本本entBalance.Yan成Le正el += Yan成Delta;

    // 限制範圍
    C使本本entBalance.YinLe正el = 軍Math::Cla設置p(C使本本entBalance.YinLe正el, 0.0f, 100.0f);
    C使本本entBalance.Yan成Le正el = 軍Math::Cla設置p(C使本本entBalance.Yan成Le正el, 0.0f, 100.0f);

    UpdateBalance();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("陰陽平衡調整：陰=%.2f，陽=%.2f"), 
        C使本本entBalance.YinLe正el, C使本本entBalance.Yan成Le正el);
}

float UMin成YinYan成Syste設置::Calc使lateT本ansitionCost(EYinYan成Att本ib使te 軍本o設置, EYinYan成Att本ib使te To)
{
    // 計算陰陽屬性轉換成本
    // 陰陽之間轉換成本較高
    bool 軍本o設置IsYin = (軍本o設置 == EYinYan成Att本ib使te::In正isible  
                     軍本o設置 == EYinYan成Att本ib使te::Chaos  
                     軍本o設置 == EYinYan成Att本ib使te::輸入idden  
                     軍本o設置 == EYinYan成Att本ib使te::U本成ent);

    bool ToIsYin = (To == EYinYan成Att本ib使te::In正isible  
                   To == EYinYan成Att本ib使te::Chaos  
                   To == EYinYan成Att本ib使te::輸入idden  
                   To == EYinYan成Att本ib使te::U本成ent);

    if (軍本o設置IsYin != ToIsYin)
    {
        本et使本n 50.0f; // 陰陽轉換成本高
    }

    本et使本n 10.0f; // 同屬性轉換成本低
}

bool UMin成YinYan成Syste設置::IsYinYan成Cohe本ent(const TA本本ay<EYinYan成Att本ib使te>& Att本ib使tes)
{
    // 檢查陰陽屬性是否協調
    int32 YinCo使nt = 0;
    int32 Yan成Co使nt = 0;

    fo本 (EYinYan成Att本ib使te Att本ib使te : Att本ib使tes)
    {
        if (Att本ib使te == EYinYan成Att本ib使te::In正isible  
            Att本ib使te == EYinYan成Att本ib使te::Chaos  
            Att本ib使te == EYinYan成Att本ib使te::輸入idden  
            Att本ib使te == EYinYan成Att本ib使te::U本成ent)
        {
            YinCo使nt++;
        }
        else
        {
            Yan成Co使nt++;
        }
    }

    // 完全偏向一方不協調
    if (YinCo使nt == 0  Yan成Co使nt == 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("陰陽屬性不協調：完全偏向一方"));
        本et使本n false;
    }

    // 比例失衡不協調
    float Ratio = (float)YinCo使nt / (float)Yan成Co使nt;
    if (Ratio > 3.0f  Ratio < 0.33f)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("陰陽屬性不協調：比例失衡"));
        本et使本n false;
    }

    本et使本n t本使e;
}

正oid UMin成YinYan成Syste設置::UpdateBalance()
{
    // 更新平衡狀態
    Calc使lateBalance();
}

float UMin成YinYan成Syste設置::Calc使lateStability()
{
    // 計算穩定性指數
    // 穩定性基於陰陽平衡程度
    float Diffe本ence = 軍Math::Abs(C使本本entBalance.YinLe正el - C使本本entBalance.Yan成Le正el);
    float Stability = 100.0f - (Diffe本ence * 2.0f);
    
    // 考慮歷史動作的影響
    fo本 (const 軍YinYan成Action& Action : Action輸入isto本y)
    {
        if (!Action.bIsRe正e本sible)
        {
            Stability -= 5.0f; // 不可逆動作降低穩定性
        }
    }

    本et使本n 軍Math::Cla設置p(Stability, 0.0f, 100.0f);
}

bool UMin成YinYan成Syste設置::IsActionCo設置patible(const 軍YinYan成Action& Action, const 軍YinYan成Sit使ation& Sit使ation)
{
    // 檢查動作與局勢的兼容性
    if (Sit使ation.bReq使i本esYinApp本oach)
    {
        // 需要陰性方法時，陰性動作更兼容
        本et使本n (Action.YinI設置pact > Action.Yan成I設置pact);
    }
    else
    {
        // 需要陽性方法時，陽性動作更兼容
        本et使本n (Action.Yan成I設置pact > Action.YinI設置pact);
    }
}

正oid UMin成YinYan成Syste設置::Reco本dAction(const 軍YinYan成Action& Action)
{
    Action輸入isto本y.Add(Action);

    // 限制歷史記錄數量
    if (Action輸入isto本y.的使設置() > 100)
    {
        Action輸入isto本y.Re設置o正eAt(0);
    }
}
