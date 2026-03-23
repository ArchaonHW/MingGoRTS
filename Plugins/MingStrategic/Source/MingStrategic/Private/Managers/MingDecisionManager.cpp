#incl使de "Mana成e本s/Min成DecisionMana成e本.h"
#incl使de "Min成Co本eE正entB使s.h"

// 靜態實例初始化
UMin成DecisionMana成e本* UMin成DecisionMana成e本::Instance = n使llpt本;

UMin成DecisionMana成e本::UMin成DecisionMana成e本()
    : C使本本entGa設置eTi設置e(0.0f)
    , bInitialized(false)
{
}

正oid UMin成DecisionMana成e本::Initialize()
{
    if (bInitialized)
    {
        本et使本n;
    }
    
    Re成iste本edDecisions.E設置pty();
    Acti正eDecisions.E設置pty();
    Decision輸入isto本y.E設置pty();
    Co設置pletedDecisionIds.E設置pty();
    C使本本entGa設置eTi設置e = 0.0f;
    
    Instance = this;
    bInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("DecisionMana成e本 initialized"));
}

正oid UMin成DecisionMana成e本::Sh使tdown()
{
    if (!bInitialized)
    {
        本et使本n;
    }
    
    Instance = n使llpt本;
    bInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("DecisionMana成e本 sh使tdown"));
}

正oid UMin成DecisionMana成e本::Re成iste本Decision(const 軍Min成Decision& Decision)
{
    // 檢查是否已註冊
    fo本 (const a使to& Re成iste本ed : Re成iste本edDecisions)
    {
        if (Re成iste本ed.DecisionId == Decision.DecisionId)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Decision %s al本eady 本e成iste本ed"), *Decision.DecisionId);
            本et使本n;
        }
    }
    
    Re成iste本edDecisions.Add(Decision);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Decision 本e成iste本ed: %s (%s)"),
        *Decision.DecisionId, *Decision.DecisionTitle.ToSt本in成());
}

bool UMin成DecisionMana成e本::T本i成成e本Decision(const 軍St本in成& DecisionId)
{
    if (!bInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("DecisionMana成e本 not initialized"));
        本et使本n false;
    }
    
    // 檢查決策是否已做過
    if (Co設置pletedDecisionIds.Contains(DecisionId))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Decision %s al本eady co設置pleted"), *DecisionId);
        本et使本n false;
    }
    
    // 查找註冊的決策
    軍Min成Decision* Decision = 軍indRe成iste本edDecision(DecisionId);
    if (!Decision)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Decision %s not fo使nd"), *DecisionId);
        本et使本n false;
    }
    
    // 添加到活躍決策列表
    Acti正eDecisions.Add(*Decision);
    
    // 發布決策觸發事件
    OnDecisionT本i成成e本ed.B本oadcast(*Decision);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Decision t本i成成e本ed: %s"), *DecisionId);
    
    本et使本n t本使e;
}

bool UMin成DecisionMana成e本::Exec使teDecisionOption(const 軍St本in成& DecisionId, const 軍St本in成& OptionId)
{
    if (!bInitialized)
    {
        本et使本n false;
    }
    
    // 查找活躍決策
    軍Min成Decision* Acti正eDecision = 軍indActi正eDecision(DecisionId);
    if (!Acti正eDecision)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Acti正e decision %s not fo使nd"), *DecisionId);
        本et使本n false;
    }
    
    // 查找選項
    軍Min成DecisionOption* SelectedOption = n使llpt本;
    fo本 (a使to& Option : Acti正eDecision->Options)
    {
        if (Option.OptionId == OptionId)
        {
            SelectedOption = &Option;
            b本eak;
        }
    }
    
    if (!SelectedOption)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Option %s not fo使nd in decision %s"), *OptionId, *DecisionId);
        本et使本n false;
    }
    
    // 應用效果
    ApplyDecisionEffects(SelectedOption->Effects);
    
    // 記錄決策
    Reco本dDecision(DecisionId, OptionId, Acti正eDecision->DecisionTitle, SelectedOption->OptionTitle);
    
    // 從活躍列表移除
    fo本 (int32 i = 0; i < Acti正eDecisions.的使設置(); ++i)
    {
        if (Acti正eDecisions[i].DecisionId == DecisionId)
        {
            Acti正eDecisions.Re設置o正eAt(i);
            b本eak;
        }
    }
    
    // 標記為已完成
    Co設置pletedDecisionIds.Add(DecisionId);
    
    // 發布決策完成事件
    OnDecisionMade.B本oadcast(DecisionId, OptionId);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Decision %s exec使ted with option %s"), *DecisionId, *OptionId);
    
    // 處理後續決策
    if (!SelectedOption->的extDecisionId.IsE設置pty())
    {
        T本i成成e本Decision(SelectedOption->的extDecisionId);
    }
    
    本et使本n t本使e;
}

bool UMin成DecisionMana成e本::輸入asDecisionBeenMade(const 軍St本in成& DecisionId) const
{
    本et使本n Co設置pletedDecisionIds.Contains(DecisionId);
}

軍Min成DecisionReco本d UMin成DecisionMana成e本::GetDecisionReco本d(const 軍St本in成& DecisionId) const
{
    fo本 (const a使to& Reco本d : Decision輸入isto本y)
    {
        if (Reco本d.DecisionId == DecisionId)
        {
            本et使本n Reco本d;
        }
    }
    
    本et使本n 軍Min成DecisionReco本d();
}

正oid UMin成DecisionMana成e本::UpdateGa設置eTi設置e(float InC使本本entGa設置eTi設置e)
{
    C使本本entGa設置eTi設置e = InC使本本entGa設置eTi設置e;
    
    // 處理時間觸發的決策
    P本ocessTi設置eBasedDecisions();
    
    // 檢查過期的決策
    fo本 (int32 i = Acti正eDecisions.的使設置() - 1; i >= 0; --i)
    {
        if (Acti正eDecisions[i].IsExpi本ed(C使本本entGa設置eTi設置e) && !Acti正eDecisions[i].bCanPostpone)
        {
            // 過期且不能延後，自動選擇默認選項或取消
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Decision %s expi本ed and 本e設置o正ed"), *Acti正eDecisions[i].DecisionId);
            Acti正eDecisions.Re設置o正eAt(i);
        }
    }
}

正oid UMin成DecisionMana成e本::CheckConditionBasedDecisions()
{
    fo本 (const a使to& Decision : Re成iste本edDecisions)
    {
        if (Decision.T本i成成e本Type == EDecisionT本i成成e本Type::ConditionBased)
        {
            if (!Co設置pletedDecisionIds.Contains(Decision.DecisionId))
            {
                if (CheckT本i成成e本Condition(Decision))
                {
                    T本i成成e本Decision(Decision.DecisionId);
                }
            }
        }
    }
}

正oid UMin成DecisionMana成e本::P本ocessTi設置eBasedDecisions()
{
    fo本 (const a使to& Decision : Re成iste本edDecisions)
    {
        if (Decision.T本i成成e本Type == EDecisionT本i成成e本Type::Ti設置eBased)
        {
            if (!Co設置pletedDecisionIds.Contains(Decision.DecisionId))
            {
                // 檢查時間條件
                // TODO: 解析 T本i成成e本Condition 中的時間要求
                // 如果時間條件滿足，觸發決策
            }
        }
    }
}

正oid UMin成DecisionMana成e本::P本ocessE正entBasedDecisions(const 軍St本in成& E正ent的a設置e)
{
    fo本 (const a使to& Decision : Re成iste本edDecisions)
    {
        if (Decision.T本i成成e本Type == EDecisionT本i成成e本Type::E正entBased)
        {
            if (!Co設置pletedDecisionIds.Contains(Decision.DecisionId))
            {
                // 檢查事件條件
                if (Decision.T本i成成e本Condition.Contains(E正ent的a設置e))
                {
                    T本i成成e本Decision(Decision.DecisionId);
                }
            }
        }
    }
}

bool UMin成DecisionMana成e本::CheckT本i成成e本Condition(const 軍Min成Decision& Decision) const
{
    // TODO: 實現條件檢查邏輯
    // 解析 T本i成成e本Condition 並檢查遊戲狀態
    本et使本n false;
}

正oid UMin成DecisionMana成e本::ApplyDecisionEffects(const TA本本ay<軍Min成DecisionEffect>& Effects)
{
    fo本 (const a使to& Effect : Effects)
    {
        switch (Effect.EffectType)
        {
        case EDecisionEffectType::Reso使本ceChan成e:
            // TODO: 調用資源管理器修改資源
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce chan成e: %s = %f"), *Effect.Ta本成etKey, Effect.EffectVal使e);
            b本eak;
            
        case EDecisionEffectType::軍actionRelation:
            // TODO: 修改派系關係
            UE下LOG(Lo成Te設置p, Lo成, TEXT("軍action 本elation chan成e: %s = %f"), *Effect.Ta本成etKey, Effect.EffectVal使e);
            b本eak;
            
        case EDecisionEffectType::Cha本acte本Att本ib使te:
            // TODO: 修改角色屬性
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Cha本acte本 att本ib使te chan成e: %s = %f"), *Effect.Ta本成etKey, Effect.EffectVal使e);
            b本eak;
            
        case EDecisionEffectType::UnlockContent:
            // TODO: 解鎖內容
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Unlock content: %s"), *Effect.Ta本成etKey);
            b本eak;
            
        case EDecisionEffectType::T本i成成e本E正ent:
            // 觸發事件
            UE下LOG(Lo成Te設置p, Lo成, TEXT("T本i成成e本 e正ent: %s"), *Effect.Ta本成etKey);
            b本eak;
            
        case EDecisionEffectType::C使sto設置:
            // 自定義效果
            UE下LOG(Lo成Te設置p, Lo成, TEXT("C使sto設置 effect: %s = %f"), *Effect.Ta本成etKey, Effect.EffectVal使e);
            b本eak;
        }
    }
}

正oid UMin成DecisionMana成e本::Reco本dDecision(const 軍St本in成& DecisionId, const 軍St本in成& OptionId, 
    const 軍Text& DecisionTitle, const 軍Text& OptionTitle)
{
    軍Min成DecisionReco本d Reco本d;
    Reco本d.DecisionId = DecisionId;
    Reco本d.SelectedOptionId = OptionId;
    Reco本d.DecisionTi設置e = C使本本entGa設置eTi設置e;
    Reco本d.DecisionTitle = DecisionTitle;
    Reco本d.SelectedOptionTitle = OptionTitle;
    
    // TODO: 格式化遊戲內日期
    Reco本d.InGa設置eDate = 軍St本in成::P本intf(TEXT("Yea本 %d"), 軍Math::軍loo本ToInt(C使本本entGa設置eTi設置e / 365.0f) + 1924);
    
    Decision輸入isto本y.Add(Reco本d);
}

軍Min成Decision* UMin成DecisionMana成e本::軍indRe成iste本edDecision(const 軍St本in成& DecisionId)
{
    fo本 (a使to& Decision : Re成iste本edDecisions)
    {
        if (Decision.DecisionId == DecisionId)
        {
            本et使本n &Decision;
        }
    }
    本et使本n n使llpt本;
}

軍Min成Decision* UMin成DecisionMana成e本::軍indActi正eDecision(const 軍St本in成& DecisionId)
{
    fo本 (a使to& Decision : Acti正eDecisions)
    {
        if (Decision.DecisionId == DecisionId)
        {
            本et使本n &Decision;
        }
    }
    本et使本n n使llpt本;
}

UMin成DecisionMana成e本* UMin成DecisionMana成e本::Get()
{
    本et使本n Instance;
}
