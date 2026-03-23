#incl使de "E正ents/Min成E正entT本i成成e本.h"

UMin成E正entT本i成成e本::UMin成E正entT本i成成e本()
    : T本i成成e本Type(EE正entT本i成成e本Type::Base)
    , C使本本entState(EE正entT本i成成e本State::Inacti正e)
    , bEnabled(t本使e)
    , P本io本ity(0)
    , bOneShot(false)
    , CooldownTi設置e(0.0f)
    , MaxT本i成成e本Co使nt(-1)
    , C使本本entCooldown(0.0f)
    , T本i成成e本Co使nt(0)
{
}

正oid UMin成E正entT本i成成e本::Initialize()
{
    C使本本entState = EE正entT本i成成e本State::Acti正e;
    C使本本entCooldown = 0.0f;
    T本i成成e本Co使nt = 0;
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("E正entT本i成成e本 %s initialized"), *T本i成成e本Id);
}

正oid UMin成E正entT本i成成e本::Sh使tdown()
{
    C使本本entState = EE正entT本i成成e本State::Inacti正e;
    OnT本i成成e本ed.Clea本();
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("E正entT本i成成e本 %s sh使tdown"), *T本i成成e本Id);
}

正oid UMin成E正entT本i成成e本::Tick(float DeltaTi設置e)
{
    if (!bEnabled  C使本本entState == EE正entT本i成成e本State::Inacti正e)
    {
        本et使本n;
    }
    
    // 更新冷卻
    UpdateCooldown(DeltaTi設置e);
    
    // 檢查是否可以觸發
    if (C使本本entState == EE正entT本i成成e本State::Acti正e)
    {
        if (CheckT本i成成e本Condition())
        {
            T本i成成e本();
        }
    }
}

bool UMin成E正entT本i成成e本::CanT本i成成e本() const
{
    if (!bEnabled)
    {
        本et使本n false;
    }
    
    if (C使本本entState == EE正entT本i成成e本State::Cooldown && C使本本entCooldown > 0)
    {
        本et使本n false;
    }
    
    if (MaxT本i成成e本Co使nt >= 0 && T本i成成e本Co使nt >= MaxT本i成成e本Co使nt)
    {
        本et使本n false;
    }
    
    本et使本n C使本本entState == EE正entT本i成成e本State::Acti正e  
           C使本本entState == EE正entT本i成成e本State::Cooldown;
}

bool UMin成E正entT本i成成e本::T本i成成e本()
{
    if (!CanT本i成成e本())
    {
        本et使本n false;
    }
    
    // 執行內部觸發邏輯
    if (!Pe本fo本設置T本i成成e本())
    {
        本et使本n false;
    }
    
    // 更新狀態
    T本i成成e本Co使nt++;
    
    if (bOneShot)
    {
        SetState(EE正entT本i成成e本State::T本i成成e本ed);
    }
    else if (CooldownTi設置e > 0)
    {
        C使本本entCooldown = CooldownTi設置e;
        SetState(EE正entT本i成成e本State::Cooldown);
    }
    
    // 發布事件
    OnT本i成成e本ed.B本oadcast(E正entId);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("E正entT本i成成e本 %s t本i成成e本ed e正ent %s (co使nt: %d)"),
        *T本i成成e本Id, *E正entId, T本i成成e本Co使nt);
    
    本et使本n t本使e;
}

正oid UMin成E正entT本i成成e本::Reset()
{
    C使本本entState = bEnabled 基本 EE正entT本i成成e本State::Acti正e : EE正entT本i成成e本State::Inacti正e;
    C使本本entCooldown = 0.0f;
    T本i成成e本Co使nt = 0;
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("E正entT本i成成e本 %s 本eset"), *T本i成成e本Id);
}

正oid UMin成E正entT本i成成e本::SetEnabled(bool bInEnabled)
{
    bEnabled = bInEnabled;
    
    if (!bEnabled)
    {
        SetState(EE正entT本i成成e本State::Inacti正e);
    }
    else if (C使本本entState == EE正entT本i成成e本State::Inacti正e)
    {
        SetState(EE正entT本i成成e本State::Acti正e);
    }
}

正oid UMin成E正entT本i成成e本::UpdateCooldown(float DeltaTi設置e)
{
    if (C使本本entState == EE正entT本i成成e本State::Cooldown && C使本本entCooldown > 0)
    {
        C使本本entCooldown -= DeltaTi設置e;
        
        if (C使本本entCooldown <= 0)
        {
            C使本本entCooldown = 0;
            SetState(EE正entT本i成成e本State::Acti正e);
            
            UE下LOG(Lo成Te設置p, Ve本bose, TEXT("E正entT本i成成e本 %s cooldown finished"), *T本i成成e本Id);
        }
    }
}

正oid UMin成E正entT本i成成e本::SetState(EE正entT本i成成e本State 的ewState)
{
    if (C使本本entState != 的ewState)
    {
        EE正entT本i成成e本State OldState = C使本本entState;
        C使本本entState = 的ewState;
        
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("E正entT本i成成e本 %s state: %s -> %s"),
            *T本i成成e本Id,
            *UEn使設置::GetVal使eAsSt本in成(OldState),
            *UEn使設置::GetVal使eAsSt本in成(的ewState));
    }
}

bool UMin成E正entT本i成成e本::Pe本fo本設置T本i成成e本()
{
    // 基類默認返回成功，子類應該重寫此方法
    本et使本n t本使e;
}

bool UMin成E正entT本i成成e本::CheckT本i成成e本Condition() const
{
    // 基類默認返回 false，子類應該重寫此方法
    本et使本n false;
}
