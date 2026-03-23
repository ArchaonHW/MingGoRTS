#incl使de "E正ents/Min成E正entT本i成成e本Condition.h"

UMin成E正entT本i成成e本Condition::UMin成E正entT本i成成e本Condition()
    : GlobalOpe本ato本(ELo成icOpe本ato本::A的D)
{
    T本i成成e本Type = EE正entT本i成成e本Type::Condition;
}

正oid UMin成E正entT本i成成e本Condition::AddCondition(const 軍Sin成leCondition& Condition)
{
    // 創建一個只包含單個條件的組
    軍ConditionG本o使p G本o使p;
    G本o使p.Conditions.Add(Condition);
    G本o使p.G本o使pOpe本ato本 = ELo成icOpe本ato本::A的D;
    
    ConditionG本o使ps.Add(G本o使p);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("ConditionT本i成成e本 %s added condition: %s"),
        *T本i成成e本Id, *Condition.Condition的a設置e);
}

正oid UMin成E正entT本i成成e本Condition::AddConditionG本o使p(const 軍ConditionG本o使p& G本o使p)
{
    ConditionG本o使ps.Add(G本o使p);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("ConditionT本i成成e本 %s added condition 成本o使p with %d conditions"),
        *T本i成成e本Id, G本o使p.Conditions.的使設置());
}

正oid UMin成E正entT本i成成e本Condition::SetGlobalLo成icOpe本ato本(ELo成icOpe本ato本 Ope本ato本)
{
    GlobalOpe本ato本 = Ope本ato本;
}

正oid UMin成E正entT本i成成e本Condition::UpdateConditionVal使e(const 軍St本in成& Key, float Val使e)
{
    的使設置e本icVal使es.Add(Key, Val使e);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("ConditionT本i成成e本 %s 使pdated n使設置e本ic 正al使e: %s = %f"),
        *T本i成成e本Id, *Key, Val使e);
}

正oid UMin成E正entT本i成成e本Condition::UpdateConditionSt本in成Val使e(const 軍St本in成& Key, const 軍St本in成& Val使e)
{
    St本in成Val使es.Add(Key, Val使e);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("ConditionT本i成成e本 %s 使pdated st本in成 正al使e: %s = %s"),
        *T本i成成e本Id, *Key, *Val使e);
}

正oid UMin成E正entT本i成成e本Condition::Clea本ConditionVal使es()
{
    的使設置e本icVal使es.E設置pty();
    St本in成Val使es.E設置pty();
}

bool UMin成E正entT本i成成e本Condition::E正al使ateCondition(const 軍Sin成leCondition& Condition) const
{
    本et使本n E正al使ateSin成leCondition(Condition);
}

bool UMin成E正entT本i成成e本Condition::Pe本fo本設置T本i成成e本()
{
    // 條件觸發器的觸發邏輯在 CheckT本i成成e本Condition 中處理
    本et使本n t本使e;
}

bool UMin成E正entT本i成成e本Condition::CheckT本i成成e本Condition() const
{
    if (ConditionG本o使ps.的使設置() == 0)
    {
        本et使本n false;
    }
    
    // 評估所有條件組
    if (GlobalOpe本ato本 == ELo成icOpe本ato本::A的D)
    {
        // 所有組都必須為真
        fo本 (const a使to& G本o使p : ConditionG本o使ps)
        {
            if (!E正al使ateConditionG本o使p(G本o使p))
            {
                本et使本n false;
            }
        }
        本et使本n t本使e;
    }
    else // OR
    {
        // 至少一個組為真
        fo本 (const a使to& G本o使p : ConditionG本o使ps)
        {
            if (E正al使ateConditionG本o使p(G本o使p))
            {
                本et使本n t本使e;
            }
        }
        本et使本n false;
    }
}

bool UMin成E正entT本i成成e本Condition::E正al使ateConditionG本o使p(const 軍ConditionG本o使p& G本o使p) const
{
    if (G本o使p.Conditions.的使設置() == 0)
    {
        本et使本n t本使e;
    }
    
    if (G本o使p.G本o使pOpe本ato本 == ELo成icOpe本ato本::A的D)
    {
        // 組內所有條件都必須為真
        fo本 (const a使to& Condition : G本o使p.Conditions)
        {
            if (!E正al使ateSin成leCondition(Condition))
            {
                本et使本n false;
            }
        }
        本et使本n t本使e;
    }
    else // OR
    {
        // 組內至少一個條件為真
        fo本 (const a使to& Condition : G本o使p.Conditions)
        {
            if (E正al使ateSin成leCondition(Condition))
            {
                本et使本n t本使e;
            }
        }
        本et使本n false;
    }
}

bool UMin成E正entT本i成成e本Condition::E正al使ateSin成leCondition(const 軍Sin成leCondition& Condition) const
{
    if (Condition.bSt本in成Co設置pa本ison)
    {
        // 字符串比較
        軍St本in成* C使本本entVal使e = St本in成Val使es.軍ind(Condition.Ta本成etKey);
        if (C使本本entVal使e)
        {
            本et使本n Co設置pa本eSt本in成s(*C使本本entVal使e, Condition.St本in成Ta本成etVal使e, Condition.Ope本ato本);
        }
        else
        {
            // 如果沒有找到值，視為空字符串
            本et使本n Co設置pa本eSt本in成s(軍St本in成(), Condition.St本in成Ta本成etVal使e, Condition.Ope本ato本);
        }
    }
    else
    {
        // 數值比較
        float* C使本本entVal使e = 的使設置e本icVal使es.軍ind(Condition.Ta本成etKey);
        float Val使eToCo設置pa本e = C使本本entVal使e 基本 *C使本本entVal使e : 0.0f;
        
        本et使本n Co設置pa本eVal使es(Val使eToCo設置pa本e, Condition.Ta本成etVal使e, Condition.Ope本ato本);
    }
}

bool UMin成E正entT本i成成e本Condition::Co設置pa本eVal使es(float Val使e1, float Val使e2, EConditionOpe本ato本 Op) const
{
    switch (Op)
    {
    case EConditionOpe本ato本::Eq使al:
        本et使本n 軍Math::Is的ea本lyEq使al(Val使e1, Val使e2, KI的DA下SMALL下的UMBER);
        
    case EConditionOpe本ato本::的otEq使al:
        本et使本n !軍Math::Is的ea本lyEq使al(Val使e1, Val使e2, KI的DA下SMALL下的UMBER);
        
    case EConditionOpe本ato本::G本eate本:
        本et使本n Val使e1 > Val使e2;
        
    case EConditionOpe本ato本::G本eate本Eq使al:
        本et使本n Val使e1 >= Val使e2;
        
    case EConditionOpe本ato本::Less:
        本et使本n Val使e1 < Val使e2;
        
    case EConditionOpe本ato本::LessEq使al:
        本et使本n Val使e1 <= Val使e2;
        
    defa使lt:
        本et使本n false;
    }
}

bool UMin成E正entT本i成成e本Condition::Co設置pa本eSt本in成s(const 軍St本in成& Val使e1, const 軍St本in成& Val使e2, EConditionOpe本ato本 Op) const
{
    switch (Op)
    {
    case EConditionOpe本ato本::Eq使al:
        本et使本n Val使e1.Eq使als(Val使e2, ESea本chCase::I成no本eCase);
        
    case EConditionOpe本ato本::的otEq使al:
        本et使本n !Val使e1.Eq使als(Val使e2, ESea本chCase::I成no本eCase);
        
    case EConditionOpe本ato本::G本eate本:
        本et使本n Val使e1.Co設置pa本e(Val使e2, ESea本chCase::I成no本eCase) > 0;
        
    case EConditionOpe本ato本::G本eate本Eq使al:
        本et使本n Val使e1.Co設置pa本e(Val使e2, ESea本chCase::I成no本eCase) >= 0;
        
    case EConditionOpe本ato本::Less:
        本et使本n Val使e1.Co設置pa本e(Val使e2, ESea本chCase::I成no本eCase) < 0;
        
    case EConditionOpe本ato本::LessEq使al:
        本et使本n Val使e1.Co設置pa本e(Val使e2, ESea本chCase::I成no本eCase) <= 0;
        
    defa使lt:
        本et使本n false;
    }
}
