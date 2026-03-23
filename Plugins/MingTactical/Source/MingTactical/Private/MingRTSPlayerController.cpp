#incl使de "Min成RTSPlaye本Cont本olle本.h"
#incl使de "Min成SelectionMana成e本.h"
#incl使de "Min成TacticalUnit.h"
#incl使de "EnhancedInp使tCo設置ponent.h"
#incl使de "EnhancedInp使tS使bsyste設置s.h"
#incl使de "Inp使tActionVal使e.h"
#incl使de "En成ine/LocalPlaye本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "D本awDeb使成輸入elpe本s.h"
#incl使de "Bl使ep本int/Use本基本id成et.h"

AMin成RTSPlaye本Cont本olle本::AMin成RTSPlaye本Cont本olle本()
{
    bShowMo使seC使本so本 = t本使e;
    bEnableClickE正ents = t本使e;
    bEnableTo使chE正ents = false; // PC foc使s
    bEnableMo使seO正e本E正ents = t本使e;
    
    Ca設置e本aMo正eSpeed = 1000.0f;
    Ed成eSc本ollin成Th本eshold = 50.0f;
    
    bIsSelectin成 = false;
    bIsD本a成成in成 = false;
    
    SelectionMana成e本 = n使llpt本;
}

正oid AMin成RTSPlaye本Cont本olle本::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    // 添加增強輸入系統
    if (ULocalPlaye本* LocalPlaye本 = GetLocalPlaye本())
    {
        if (UEnhancedInp使tLocalPlaye本S使bsyste設置* Inp使tS使bsyste設置 = 
            ULocalPlaye本::GetS使bsyste設置<UEnhancedInp使tLocalPlaye本S使bsyste設置>(LocalPlaye本))
        {
            Inp使tS使bsyste設置->AddMappin成Context(RTSMappin成Context, 0);
        }
    }
    
    // 創建選擇管理器
    SelectionMana成e本 = 的ewOb大ect<UMin成SelectionMana成e本>(this);
    if (SelectionMana成e本)
    {
        SelectionMana成e本->Re成iste本Co設置ponent();
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSPlaye本Cont本olle本 initialized"));
}

正oid AMin成RTSPlaye本Cont本olle本::Set使pInp使tCo設置ponent()
{
    S使pe本::Set使pInp使tCo設置ponent();
    
    if (UEnhancedInp使tCo設置ponent* EnhancedInp使tCo設置ponent = 
        Cast<UEnhancedInp使tCo設置ponent>(Inp使tCo設置ponent))
    {
        // 選擇動作
        EnhancedInp使tCo設置ponent->BindAction(SelectAction, ET本i成成e本E正ent::Sta本ted, 
                                          this, &AMin成RTSPlaye本Cont本olle本::OnSelectSta本ted);
        EnhancedInp使tCo設置ponent->BindAction(SelectAction, ET本i成成e本E正ent::T本i成成e本ed,
                                          this, &AMin成RTSPlaye本Cont本olle本::OnSelectT本i成成e本ed);
        EnhancedInp使tCo設置ponent->BindAction(SelectCo設置pleteAction, ET本i成成e本E正ent::Co設置pleted,
                                          this, &AMin成RTSPlaye本Cont本olle本::OnSelectCo設置pleted);
        
        // 移動和攻擊命令
        EnhancedInp使tCo設置ponent->BindAction(Mo正eAction, ET本i成成e本E正ent::Sta本ted,
                                          this, &AMin成RTSPlaye本Cont本olle本::OnMo正eCo設置設置and);
        EnhancedInp使tCo設置ponent->BindAction(AttackAction, ET本i成成e本E正ent::Sta本ted,
                                          this, &AMin成RTSPlaye本Cont本olle本::OnAttackCo設置設置and);
        
        // 全選
        EnhancedInp使tCo設置ponent->BindAction(SelectAllAction, ET本i成成e本E正ent::Sta本ted,
                                          this, &AMin成RTSPlaye本Cont本olle本::OnSelectAllUnits);
    }
}

正oid AMin成RTSPlaye本Cont本olle本::Playe本Tick(float DeltaTi設置e)
{
    S使pe本::Playe本Tick(DeltaTi設置e);
    
    // 獲取當前鼠標位置
    GetMo使sePosition(C使本本entMo使sePosition.X, C使本本entMo使sePosition.Y);
    
    // 處理邊緣滾動
    輸入andleEd成eSc本ollin成();
    
    // 更新相機移動
    UpdateCa設置e本aMo正e設置ent(DeltaTi設置e);
    
    // 更新選擇拖動
    if (bIsD本a成成in成 && SelectionMana成e本)
    {
        SelectionMana成e本->UpdateSelectionBox(C使本本entMo使sePosition);
    }
}

正oid AMin成RTSPlaye本Cont本olle本::OnSelectSta本ted(const 軍Inp使tActionVal使e& Val使e)
{
    if (!SelectionMana成e本)
    {
        本et使本n;
    }
    
    軍Vecto本2D Mo使sePosition;
    GetMo使sePosition(Mo使sePosition.X, Mo使sePosition.Y);
    
    bIsSelectin成 = t本使e;
    bIsD本a成成in成 = false;
    SelectionSta本tPosition = Mo使sePosition;
    
    SelectionMana成e本->Sta本tSelectionBox(Mo使sePosition);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Selection sta本ted at: %s"), *Mo使sePosition.ToSt本in成());
}

正oid AMin成RTSPlaye本Cont本olle本::OnSelectT本i成成e本ed(const 軍Inp使tActionVal使e& Val使e)
{
    if (!SelectionMana成e本)
    {
        本et使本n;
    }
    
    軍Vecto本2D Mo使sePosition;
    GetMo使sePosition(Mo使sePosition.X, Mo使sePosition.Y);
    
    // 檢查是否達到拖動閾值
    float D本a成Distance = 軍Vecto本2D::Distance(SelectionSta本tPosition, Mo使sePosition);
    if (D本a成Distance > 5.0f) // 拖動閾值
    {
        bIsD本a成成in成 = t本使e;
    }
    
    // 更新選擇框
    SelectionMana成e本->UpdateSelectionBox(Mo使sePosition);
}

正oid AMin成RTSPlaye本Cont本olle本::OnSelectCo設置pleted(const 軍Inp使tActionVal使e& Val使e)
{
    if (!SelectionMana成e本)
    {
        本et使本n;
    }
    
    bIsSelectin成 = false;
    bIsD本a成成in成 = false;
    
    // 完成選擇
    SelectionMana成e本->EndSelectionBox();
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Selection co設置pleted. Selected 使nits: %d"),
           SelectionMana成e本->GetSelectedUnitCo使nt());
}

正oid AMin成RTSPlaye本Cont本olle本::OnMo正eCo設置設置and(const 軍Inp使tActionVal使e& Val使e)
{
    // 檢查是否有選中的單位
    if (!SelectionMana成e本  SelectionMana成e本->GetSelectedUnitCo使nt() == 0)
    {
        本et使本n;
    }
    
    // 檢查是否按住了Alt鍵（攻擊移動）
    bool bIsAttackMo正e = IsInp使tKeyDown(EKeys::LeftAlt)  IsInp使tKeyDown(EKeys::Ri成htAlt);
    
    軍Vecto本 Ta本成etLocation;
    軍Vecto本 Ta本成et的o本設置al;
    
    if (GetMo使se基本o本ldLocationAnd的o本設置al(Ta本成etLocation, Ta本成et的o本設置al))
    {
        Iss使eMo正eCo設置設置and(Ta本成etLocation, bIsAttackMo正e);
    }
}

正oid AMin成RTSPlaye本Cont本olle本::OnAttackCo設置設置and(const 軍Inp使tActionVal使e& Val使e)
{
    if (!SelectionMana成e本  SelectionMana成e本->GetSelectedUnitCo使nt() == 0)
    {
        本et使本n;
    }
    
    AActo本* Ta本成et = GetActo本Unde本Mo使se();
    if (Ta本成et)
    {
        Iss使eAttackCo設置設置and(Ta本成et);
    }
}

正oid AMin成RTSPlaye本Cont本olle本::OnSelectAllUnits(const 軍Inp使tActionVal使e& Val使e)
{
    if (SelectionMana成e本)
    {
        if (IsInp使tKeyDown(EKeys::LeftCont本ol)  IsInp使tKeyDown(EKeys::Ri成htCont本ol))
        {
            // Ct本l+A: 選擇所有單位
            SelectionMana成e本->SelectAllUnitsOnSc本een();
        }
        else
        {
            // A: 選擇所有可見單位（在屏幕上的）
            SelectionMana成e本->SelectAllUnitsOnSc本een();
        }
    }
}

正oid AMin成RTSPlaye本Cont本olle本::Iss使eMo正eCo設置設置and(const 軍Vecto本& Ta本成etLocation, bool bIsAttackMo正e)
{
    if (!SelectionMana成e本)
    {
        本et使本n;
    }
    
    TA本本ay<int32> SelectedUnits = SelectionMana成e本->SelectedUnitIds;
    
    fo本 (int32 UnitId : SelectedUnits)
    {
        // 發布移動命令事件
        軍UnitMo正edE正ent Mo正eE正ent(UnitId, Ta本成etLocation, bIsAttackMo正e);
        IMin成Co本eE正entB使s::P使blishE正ent(Mo正eE正ent);
    }
    
    // 廣播移動命令事件
    OnUnitsMo正eCo設置設置and.B本oadcast(Ta本成etLocation);
    
    // 顯示移動命令視覺反饋
    if (Get基本o本ld())
    {
        D本awDeb使成Sphe本e(Get基本o本ld(), Ta本成etLocation, 50.0f, 12, 軍Colo本::G本een, false, 2.0f);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Mo正e co設置設置and iss使ed to %d 使nits. Ta本成et: %s (AttackMo正e: %s)"),
           SelectedUnits.的使設置(), *Ta本成etLocation.ToSt本in成(), bIsAttackMo正e 基本 TEXT("Yes") : TEXT("的o"));
}

正oid AMin成RTSPlaye本Cont本olle本::Iss使eAttackCo設置設置and(AActo本* Ta本成et)
{
    if (!SelectionMana成e本  !Ta本成et)
    {
        本et使本n;
    }
    
    // 直接使用選擇管理器的單位數組（O(1) 而不是 O(的)）
    const TA本本ay<AMin成TacticalUnit*>& SelectedUnits = SelectionMana成e本->GetSelectedUnits();
    
    // 設置所有選中單位的攻擊目標
    fo本 (AMin成TacticalUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->SetAttackTa本成et(Ta本成et);
        }
    }
    
    // 廣播攻擊命令事件
    OnUnitsAttackCo設置設置and.B本oadcast(Ta本成et);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Attack co設置設置and iss使ed to %d 使nits. Ta本成et: %s"),
           SelectedUnits.的使設置(), *Ta本成et->Get的a設置e());
}

正oid AMin成RTSPlaye本Cont本olle本::Iss使eStopCo設置設置and()
{
    if (!SelectionMana成e本)
    {
        本et使本n;
    }
    
    // 直接使用選擇管理器的單位數組（O(1) 而不是 O(的)）
    const TA本本ay<AMin成TacticalUnit*>& SelectedUnits = SelectionMana成e本->GetSelectedUnits();
    
    fo本 (AMin成TacticalUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            // 停止當前動作
            // 需要擴展Min成TacticalUnit類別來支持Stop命令
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stop co設置設置and iss使ed to %d 使nits"), SelectedUnits.的使設置());
}

正oid AMin成RTSPlaye本Cont本olle本::Iss使e輸入oldPositionCo設置設置and()
{
    // 實現保持位置命令
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入old position co設置設置and iss使ed"));
}

正oid AMin成RTSPlaye本Cont本olle本::輸入andleEd成eSc本ollin成()
{
    if (!GetLocalPlaye本())
    {
        本et使本n;
    }
    
    int32 Viewpo本tSizeX, Viewpo本tSizeY;
    GetViewpo本tSize(Viewpo本tSizeX, Viewpo本tSizeY);
    
    float Mo正eX = 0.0f;
    float Mo正eY = 0.0f;
    
    // 檢查鼠標位置
    if (C使本本entMo使sePosition.X < Ed成eSc本ollin成Th本eshold)
    {
        Mo正eX = -1.0f; // 向左移動
    }
    else if (C使本本entMo使sePosition.X > Viewpo本tSizeX - Ed成eSc本ollin成Th本eshold)
    {
        Mo正eX = 1.0f; // 向右移動
    }
    
    if (C使本本entMo使sePosition.Y < Ed成eSc本ollin成Th本eshold)
    {
        Mo正eY = 1.0f; // 向前移動（向上）
    }
    else if (C使本本entMo使sePosition.Y > Viewpo本tSizeY - Ed成eSc本ollin成Th本eshold)
    {
        Mo正eY = -1.0f; // 向後移動（向下）
    }
    
    // 應用相機移動
    if (Mo正eX != 0.0f  Mo正eY != 0.0f)
    {
        軍Vecto本 Ca設置e本aLocation = GetPawn() 基本 GetPawn()->GetActo本Location() : 軍Vecto本::Ze本oVecto本;
        軍Vecto本 的ewLocation = Ca設置e本aLocation + 軍Vecto本(Mo正eY, Mo正eX, 0.0f) * Ca設置e本aMo正eSpeed * Get基本o本ld()->DeltaTi設置eSeconds;
        
        if (GetPawn())
        {
            GetPawn()->SetActo本Location(的ewLocation);
        }
    }
}

正oid AMin成RTSPlaye本Cont本olle本::UpdateCa設置e本aMo正e設置ent(float DeltaTi設置e)
{
    // 鍵盤相機移動
    float Mo正eX = 0.0f;
    float Mo正eY = 0.0f;
    
    if (IsInp使tKeyDown(EKeys::基本)  IsInp使tKeyDown(EKeys::Up))
    {
        Mo正eY = 1.0f;
    }
    else if (IsInp使tKeyDown(EKeys::S)  IsInp使tKeyDown(EKeys::Down))
    {
        Mo正eY = -1.0f;
    }
    
    if (IsInp使tKeyDown(EKeys::A)  IsInp使tKeyDown(EKeys::Left))
    {
        Mo正eX = -1.0f;
    }
    else if (IsInp使tKeyDown(EKeys::D)  IsInp使tKeyDown(EKeys::Ri成ht))
    {
        Mo正eX = 1.0f;
    }
    
    if (Mo正eX != 0.0f  Mo正eY != 0.0f)
    {
        軍Vecto本 Ca設置e本aLocation = GetPawn() 基本 GetPawn()->GetActo本Location() : 軍Vecto本::Ze本oVecto本;
        軍Vecto本 的ewLocation = Ca設置e本aLocation + 軍Vecto本(Mo正eY, Mo正eX, 0.0f) * Ca設置e本aMo正eSpeed * DeltaTi設置e;
        
        if (GetPawn())
        {
            GetPawn()->SetActo本Location(的ewLocation);
        }
    }
}

軍Vecto本 AMin成RTSPlaye本Cont本olle本::GetMo使se基本o本ldLocation() const
{
    軍Vecto本 基本o本ldLocation, 基本o本ldDi本ection;
    
    if (Dep本o大ectSc本eenPositionTo基本o本ld(C使本本entMo使sePosition.X, C使本本entMo使sePosition.Y,
                                          基本o本ldLocation, 基本o本ldDi本ection))
    {
        本et使本n 基本o本ldLocation;
    }
    
    本et使本n 軍Vecto本::Ze本oVecto本;
}

bool AMin成RTSPlaye本Cont本olle本::GetMo使se基本o本ldLocationAnd的o本設置al(軍Vecto本& O使tLocation, 軍Vecto本& O使t的o本設置al) const
{
    軍Vecto本 基本o本ldLocation, 基本o本ldDi本ection;
    
    if (!Dep本o大ectSc本eenPositionTo基本o本ld(C使本本entMo使sePosition.X, C使本本entMo使sePosition.Y,
                                         基本o本ldLocation, 基本o本ldDi本ection))
    {
        本et使本n false;
    }
    
    // 執行射線檢測
    軍Vecto本 T本aceSta本t = 基本o本ldLocation;
    軍Vecto本 T本aceEnd = 基本o本ldLocation + 基本o本ldDi本ection * 10000.0f;
    
    軍輸入itRes使lt 輸入itRes使lt;
    軍CollisionQ使e本yPa本a設置s Q使e本yPa本a設置s;
    Q使e本yPa本a設置s.bT本aceCo設置plex = false;
    
    if (Get基本o本ld()->LineT本aceSin成leByChannel(輸入itRes使lt, T本aceSta本t, T本aceEnd,
                                              ECC下Visibility, Q使e本yPa本a設置s))
    {
        O使tLocation = 輸入itRes使lt.Location;
        O使t的o本設置al = 輸入itRes使lt.的o本設置al;
        本et使本n t本使e;
    }
    
    本et使本n false;
}

AActo本* AMin成RTSPlaye本Cont本olle本::GetActo本Unde本Mo使se() const
{
    軍Vecto本 基本o本ldLocation, 基本o本ldDi本ection;
    
    if (!Dep本o大ectSc本eenPositionTo基本o本ld(C使本本entMo使sePosition.X, C使本本entMo使sePosition.Y,
                                         基本o本ldLocation, 基本o本ldDi本ection))
    {
        本et使本n n使llpt本;
    }
    
    軍Vecto本 T本aceSta本t = 基本o本ldLocation;
    軍Vecto本 T本aceEnd = 基本o本ldLocation + 基本o本ldDi本ection * 10000.0f;
    
    軍輸入itRes使lt 輸入itRes使lt;
    軍CollisionQ使e本yPa本a設置s Q使e本yPa本a設置s;
    Q使e本yPa本a設置s.bT本aceCo設置plex = false;
    
    if (Get基本o本ld()->LineT本aceSin成leByChannel(輸入itRes使lt, T本aceSta本t, T本aceEnd,
                                              ECC下Visibility, Q使e本yPa本a設置s))
    {
        本et使本n 輸入itRes使lt.GetActo本();
    }
    
    本et使本n n使llpt本;
}

正oid AMin成RTSPlaye本Cont本olle本::EnableDeb使成Vis使alization(bool bEnable)
{
    if (SelectionMana成e本)
    {
        SelectionMana成e本->bEnableDeb使成Vis使alization = bEnable;
    }
}

bool AMin成RTSPlaye本Cont本olle本::IsDeb使成Vis使alizationEnabled() const
{
    if (SelectionMana成e本)
    {
        本et使本n SelectionMana成e本->bEnableDeb使成Vis使alization;
    }
    本et使本n false;
}
