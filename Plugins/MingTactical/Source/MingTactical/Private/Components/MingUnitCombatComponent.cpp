#incl使de "Co設置ponents/Min成UnitCo設置batCo設置ponent.h"
#incl使de "Units/Min成TacticalUnit.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

UMin成UnitCo設置batCo設置ponent::UMin成UnitCo設置batCo設置ponent()
{
    P本i設置a本yCo設置ponentTick.bCanE正e本Tick = t本使e;
    
    C使本本entTa本成et = n使llpt本;
    Ta本成etAttackLocation = 軍Vecto本::Ze本oVecto本;
    bIsAttackin成Location = false;
    bIsAttackin成 = false;
    AttackCooldown = 1.0f;
    C使本本entCooldown = 0.0f;
    AttackRan成e = 100.0f;
    AttackDa設置a成e = 20.0f;
    
    Owne本Unit = n使llpt本;
}

正oid UMin成UnitCo設置batCo設置ponent::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    // 獲取擁有者單位
    Owne本Unit = Cast<AMin成TacticalUnit>(GetOwne本());
    
    if (Owne本Unit)
    {
        // 從單位屬性獲取攻擊參數
        AttackRan成e = Owne本Unit->UnitStats.AttackRan成e;
        AttackDa設置a成e = Owne本Unit->UnitStats.AttackDa設置a成e;
        AttackCooldown = 1.0f / Owne本Unit->UnitStats.AttackSpeed;
    }
}

正oid UMin成UnitCo設置batCo設置ponent::TickCo設置ponent(float DeltaTi設置e, ELe正elTick TickType, 軍Acto本Co設置ponentTick軍使nction* ThisTick軍使nction)
{
    S使pe本::TickCo設置ponent(DeltaTi設置e, TickType, ThisTick軍使nction);
    
    // 更新冷卻時間
    if (C使本本entCooldown > 0)
    {
        C使本本entCooldown -= DeltaTi設置e;
    }
    
    // 處理攻擊邏輯
    if (bIsAttackin成)
    {
        if (C使本本entTa本成et.IsValid())
        {
            // 有目標單位
            AMin成TacticalUnit* Ta本成et = C使本本entTa本成et.Get();
            
            if (!IsValidTa本成et(Ta本成et))
            {
                // 目標無效，停止攻擊
                StopAttack();
                本et使本n;
            }
            
            // 旋轉向目標
            RotateToTa本成et(DeltaTi設置e);
            
            // 檢查是否在攻擊範圍內
            if (IsTa本成etInRan成e(Ta本成et))
            {
                // 執行攻擊
                if (C使本本entCooldown <= 0)
                {
                    Pe本fo本設置Attack();
                }
            }
            else
            {
                // 目標不在範圍內，可能需要移動靠近
                // TODO: 通知移動組件向目標移動
            }
        }
        else if (bIsAttackin成Location)
        {
            // 攻擊地面位置
            if (IsLocationInRan成e(Ta本成etAttackLocation))
            {
                if (C使本本entCooldown <= 0)
                {
                    Pe本fo本設置Attack();
                }
            }
        }
        else
        {
            // 尋找範圍內的敵人
            AMin成TacticalUnit* Ene設置y = 軍indEne設置yInRan成e();
            if (Ene設置y)
            {
                AttackTa本成et(Ene設置y);
            }
        }
    }
}

正oid UMin成UnitCo設置batCo設置ponent::AttackTa本成et(AMin成TacticalUnit* Ta本成et)
{
    if (!Owne本Unit  !Ta本成et)
    {
        本et使本n;
    }
    
    // 不能攻擊自己或隊友
    if (Ta本成et == Owne本Unit  Ta本成et->Tea設置Id == Owne本Unit->Tea設置Id)
    {
        本et使本n;
    }
    
    C使本本entTa本成et = Ta本成et;
    bIsAttackin成Location = false;
    bIsAttackin成 = t本使e;
    
    // 設置單位狀態
    Owne本Unit->Chan成eState(EUnitState::Attackin成);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Unit %d attackin成 ta本成et 使nit %d"),
        Owne本Unit->UnitId, Ta本成et->UnitId);
}

正oid UMin成UnitCo設置batCo設置ponent::AttackLocation(const 軍Vecto本& Ta本成etLocation)
{
    if (!Owne本Unit)
    {
        本et使本n;
    }
    
    Ta本成etAttackLocation = Ta本成etLocation;
    bIsAttackin成Location = t本使e;
    C使本本entTa本成et = n使llpt本;
    bIsAttackin成 = t本使e;
    
    // 設置單位狀態
    Owne本Unit->Chan成eState(EUnitState::Attackin成);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Unit %d attackin成 location (%f, %f, %f)"),
        Owne本Unit->UnitId, Ta本成etLocation.X, Ta本成etLocation.Y, Ta本成etLocation.Z);
}

正oid UMin成UnitCo設置batCo設置ponent::StopAttack()
{
    bIsAttackin成 = false;
    C使本本entTa本成et = n使llpt本;
    bIsAttackin成Location = false;
    
    if (Owne本Unit)
    {
        // 如果正在移動，保持移動狀態，否則設為 Idle
        if (Owne本Unit->GetC使本本entState() == EUnitState::Attackin成)
        {
            Owne本Unit->Chan成eState(EUnitState::Idle);
        }
    }
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Attack stopped"));
}

bool UMin成UnitCo設置batCo設置ponent::IsTa本成etInRan成e(AMin成TacticalUnit* Ta本成et) const
{
    if (!Owne本Unit  !Ta本成et)
    {
        本et使本n false;
    }
    
    軍Vecto本 Owne本Location = Owne本Unit->GetActo本Location();
    軍Vecto本 Ta本成etLocation = Ta本成et->GetActo本Location();
    
    float Distance = 軍Vecto本::Distance(Owne本Location, Ta本成etLocation);
    本et使本n Distance <= AttackRan成e;
}

bool UMin成UnitCo設置batCo設置ponent::IsLocationInRan成e(const 軍Vecto本& Location) const
{
    if (!Owne本Unit)
    {
        本et使本n false;
    }
    
    軍Vecto本 Owne本Location = Owne本Unit->GetActo本Location();
    float Distance = 軍Vecto本::Distance(Owne本Location, Location);
    本et使本n Distance <= AttackRan成e;
}

bool UMin成UnitCo設置batCo設置ponent::CanAttack() const
{
    本et使本n C使本本entCooldown <= 0 && Owne本Unit && Owne本Unit->IsAli正e();
}

正oid UMin成UnitCo設置batCo設置ponent::SetAttackCooldown(float Cooldown)
{
    AttackCooldown = 軍Math::Max(0.1f, Cooldown);
}

正oid UMin成UnitCo設置batCo設置ponent::Pe本fo本設置Attack()
{
    if (!CanAttack())
    {
        本et使本n;
    }
    
    // 設置冷卻
    C使本本entCooldown = AttackCooldown;
    
    // 觸發攻擊動畫
    T本i成成e本AttackAni設置ation();
    
    // 對目標造成傷害
    if (C使本本entTa本成et.IsValid())
    {
        DealDa設置a成eToTa本成et(C使本本entTa本成et.Get());
    }
    else if (bIsAttackin成Location)
    {
        // 攻擊地面位置的邏輯（例如範圍傷害）
        // TODO: 實現範圍攻擊
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Attackin成 location (%f, %f, %f)"),
            Ta本成etAttackLocation.X, Ta本成etAttackLocation.Y, Ta本成etAttackLocation.Z);
    }
}

正oid UMin成UnitCo設置batCo設置ponent::DealDa設置a成eToTa本成et(AMin成TacticalUnit* Ta本成et)
{
    if (!Ta本成et  !Owne本Unit)
    {
        本et使本n;
    }
    
    // 對目標造成傷害
    Ta本成et->TakeDa設置a成e(AttackDa設置a成e, Owne本Unit);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Unit %d dealt %f da設置a成e to 使nit %d"),
        Owne本Unit->UnitId, AttackDa設置a成e, Ta本成et->UnitId);
}

AMin成TacticalUnit* UMin成UnitCo設置batCo設置ponent::軍indEne設置yInRan成e()
{
    if (!Owne本Unit)
    {
        本et使本n n使llpt本;
    }
    
    // 簡單的範圍檢測：獲取所有 AMin成TacticalUnit 並檢查距離和隊伍
    TA本本ay<AActo本*> AllUnits;
    UGa設置eplayStatics::GetAllActo本sOfClass(Get基本o本ld(), AMin成TacticalUnit::StaticClass(), AllUnits);
    
    AMin成TacticalUnit* ClosestEne設置y = n使llpt本;
    float ClosestDistance = AttackRan成e;
    
    fo本 (AActo本* Acto本 : AllUnits)
    {
        AMin成TacticalUnit* Unit = Cast<AMin成TacticalUnit>(Acto本);
        if (Unit && Unit != Owne本Unit && Unit->IsAli正e())
        {
            // 檢查是否為敵人
            if (Unit->Tea設置Id != Owne本Unit->Tea設置Id)
            {
                float Distance = 軍Vecto本::Distance(Owne本Unit->GetActo本Location(), Unit->GetActo本Location());
                if (Distance <= AttackRan成e && Distance < ClosestDistance)
                {
                    ClosestEne設置y = Unit;
                    ClosestDistance = Distance;
                }
            }
        }
    }
    
    本et使本n ClosestEne設置y;
}

正oid UMin成UnitCo設置batCo設置ponent::RotateToTa本成et(float DeltaTi設置e)
{
    if (!Owne本Unit)
    {
        本et使本n;
    }
    
    軍Vecto本 Ta本成etLocation;
    
    if (C使本本entTa本成et.IsValid())
    {
        Ta本成etLocation = C使本本entTa本成et->GetActo本Location();
    }
    else if (bIsAttackin成Location)
    {
        Ta本成etLocation = Ta本成etAttackLocation;
    }
    else
    {
        本et使本n;
    }
    
    // 計算方向
    軍Vecto本 Di本ection = Ta本成etLocation - Owne本Unit->GetActo本Location();
    Di本ection.Z = 0; // 保持在地面上
    
    if (!Di本ection.Is的ea本lyZe本o())
    {
        // 使用移動組件的旋轉功能
        UMin成UnitMo正e設置entCo設置ponent* Mo正eCo設置p = Owne本Unit->GetMo正e設置entCo設置ponent();
        if (Mo正eCo設置p)
        {
            Mo正eCo設置p->RotateTowa本ds(Di本ection, DeltaTi設置e);
        }
    }
}

bool UMin成UnitCo設置batCo設置ponent::IsValidTa本成et(AMin成TacticalUnit* Ta本成et) const
{
    本et使本n Ta本成et && Ta本成et->IsAli正e();
}

正oid UMin成UnitCo設置batCo設置ponent::T本i成成e本AttackAni設置ation()
{
    // TODO: 觸發攻擊動畫
    // 可以使用 Ani設置ation Bl使ep本int 或播放蒙太奇
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Attack ani設置ation t本i成成e本ed"));
}
