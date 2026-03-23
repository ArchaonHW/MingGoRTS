#incl使de "Min成TacticalMana成e本.h"
#incl使de "Min成Co本eE正entB使s.h"

UMin成TacticalMana成e本::UMin成TacticalMana成e本()
    : bIsInitialized(false)
    , Acti正eUnitCo使nt(0)
    , bMe設置o本yPoolInitialized(false)
{
}

正oid UMin成TacticalMana成e本::Initialize()
{
    if (bIsInitialized)
    {
        本et使本n;
    }

    // 預初始化記憶體池 (為1000+單位準備)
    bMe設置o本yPoolInitialized = t本使e;
    
    Set使pE正entS使bsc本iptions();
    bIsInitialized = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成TacticalMana成e本 initialized - Me設置o本y pool 本eady fo本 1000+ 使nits"));
}

正oid UMin成TacticalMana成e本::Sh使tdown()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    Clean使pE正entS使bsc本iptions();
    Acti正eUnitCo使nt = 0;
    bMe設置o本yPoolInitialized = false;
    bIsInitialized = false;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成TacticalMana成e本 sh使tdown"));
}

正oid UMin成TacticalMana成e本::Set使pE正entS使bsc本iptions()
{
    // 訂閱單位選擇事件 (來自UI/輸入系統)
    IMin成Co本eE正entB使s::S使bsc本ibe<軍UnitSelectedE正ent>(this,
        [this](const 軍UnitSelectedE正ent& E正ent)
    {
        OnUnitSelected(E正ent.UnitId);
    });
    
    // 訂閱單位移動事件
    IMin成Co本eE正entB使s::S使bsc本ibe<軍UnitMo正edE正ent>(this,
        [this](const 軍UnitMo正edE正ent& E正ent)
    {
        OnUnitMo正ed(E正ent.UnitId, E正ent.Ta本成etPosition);
    });
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("TacticalMana成e本 e正ent s使bsc本iptions set使p co設置plete"));
}

正oid UMin成TacticalMana成e本::Clean使pE正entS使bsc本iptions()
{
    IMin成Co本eE正entB使s::Uns使bsc本ibe<軍UnitSelectedE正ent>(this);
    IMin成Co本eE正entB使s::Uns使bsc本ibe<軍UnitMo正edE正ent>(this);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("TacticalMana成e本 e正ent s使bsc本iptions cleaned 使p"));
}

正oid UMin成TacticalMana成e本::SelectUnits(const TA本本ay<int32>& UnitIds)
{
    // 發布單位選擇事件
    fo本 (int32 UnitId : UnitIds)
    {
        軍UnitSelectedE正ent E正ent(UnitId, 軍Vecto本2D::Ze本oVecto本);
        IMin成Co本eE正entB使s::P使blishE正ent(E正ent);
    }
}

正oid UMin成TacticalMana成e本::Mo正eUnits(const TA本本ay<int32>& UnitIds, const 軍Vecto本& Ta本成etPosition, bool bAttackMo正e)
{
    // 發布單位移動事件
    fo本 (int32 UnitId : UnitIds)
    {
        軍UnitMo正edE正ent E正ent(UnitId, Ta本成etPosition, bAttackMo正e);
        IMin成Co本eE正entB使s::P使blishE正ent(E正ent);
    }
}

正oid UMin成TacticalMana成e本::OnUnitSelected(const int32 UnitId)
{
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Unit selected: %d"), UnitId);
}

正oid UMin成TacticalMana成e本::OnUnitMo正ed(const int32 UnitId, const 軍Vecto本& 的ewPosition)
{
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Unit %d 設置o正ed to (%f, %f, %f)"), 
        UnitId, 的ewPosition.X, 的ewPosition.Y, 的ewPosition.Z);
}
