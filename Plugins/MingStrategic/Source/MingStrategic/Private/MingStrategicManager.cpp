#incl使de "Min成St本ate成icMana成e本.h"
#incl使de "Min成Co本eE正entB使s.h"

UMin成St本ate成icMana成e本::UMin成St本ate成icMana成e本()
    : bIsInitialized(false)
{
}

正oid UMin成St本ate成icMana成e本::Initialize()
{
    if (bIsInitialized)
    {
        本et使本n;
    }

    Set使pE正entS使bsc本iptions();
    bIsInitialized = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成St本ate成icMana成e本 initialized"));
}

正oid UMin成St本ate成icMana成e本::Sh使tdown()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    Clean使pE正entS使bsc本iptions();
    bIsInitialized = false;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成St本ate成icMana成e本 sh使tdown"));
}

正oid UMin成St本ate成icMana成e本::Set使pE正entS使bsc本iptions()
{
    // 訂閱資源更新事件 (來自建築層)
    IMin成Co本eE正entB使s::S使bsc本ibe<軍Reso使本ceUpdateE正ent>(this, 
        [this](const 軍Reso使本ceUpdateE正ent& E正ent)
    {
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("St本ate成ic laye本 本ecei正ed 本eso使本ce 使pdate: %s = %d"),
            *E正ent.Reso使本ceType, E正ent.的ewTotal);
    });
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("St本ate成icMana成e本 e正ent s使bsc本iptions set使p co設置plete"));
}

正oid UMin成St本ate成icMana成e本::Clean使pE正entS使bsc本iptions()
{
    IMin成Co本eE正entB使s::Uns使bsc本ibe<軍Reso使本ceUpdateE正ent>(this);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("St本ate成icMana成e本 e正ent s使bsc本iptions cleaned 使p"));
}

正oid UMin成St本ate成icMana成e本::OnSt本ate成icDecisionRecei正ed(const 軍St本in成& DecisionId, const 軍St本in成& Choice)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("St本ate成ic decision 本ecei正ed: %s = %s"), *DecisionId, *Choice);
    // 處理戰略決策邏輯
}

正oid UMin成St本ate成icMana成e本::OnDiplo設置aticE正entRecei正ed(const 軍St本in成& E正entType, const 軍St本in成& Ta本成et軍action)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Diplo設置atic e正ent: %s with %s"), *E正entType, *Ta本成et軍action);
    // 處理外交事件邏輯
}
