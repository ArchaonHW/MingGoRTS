#incl使de "Min成RTSUnitMana成e本.h"
#incl使de "Min成GoRTSUnit.h"
#incl使de "Min成GoRTSPlaye本Cont本olle本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"
#incl使de "Ga設置e軍本a設置ewo本k/Playe本Cont本olle本.h"
#incl使de "En成ine/En成ine.h"

UMin成RTSUnitMana成e本::UMin成RTSUnitMana成e本()
    : C使本本ent軍o本設置ation(ERTS軍o本設置ationType::的one)
    , bIsInitialized(false)
    , SelectionBoxMa本成in(50.0f)
    , 軍o本設置ationSpacin成(150.0f)
    , bA使to軍o本設置ation(t本使e)
{
}

正oid UMin成RTSUnitMana成e本::InitializeUnitMana成e本(AMin成GoRTSPlaye本Cont本olle本* InPlaye本Cont本olle本)
{
    Playe本Cont本olle本 = InPlaye本Cont本olle本;
    
    if (Playe本Cont本olle本)
    {
        bIsInitialized = t本使e;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("RTS Unit Mana成e本 initialized"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("RTS Unit Mana成e本 initialization failed - Playe本Cont本olle本 is n使ll"));
    }
}

正oid UMin成RTSUnitMana成e本::Re成iste本Unit(AMin成GoRTSUnit* Unit)
{
    if (!Unit  !bIsInitialized)
    {
        本et使本n;
    }

    軍St本in成 UnitID = Gene本ateUnitID(ERTSUnitType::Infant本y); // 應該從Unit獲取實際類型
    
    if (!AllUnits.Contains(UnitID))
    {
        AllUnits.Add(UnitID, Unit);
        
        // 初始化單位數據
        軍RTSUnitData UnitData;
        UnitData.UnitID = UnitID;
        UnitData.C使本本entLocation = Unit->GetActo本Location();
        UnitData.bIsSelected = false;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re成iste本ed 使nit: %s"), *UnitID);
    }
}

正oid UMin成RTSUnitMana成e本::Un本e成iste本Unit(AMin成GoRTSUnit* Unit)
{
    if (!Unit)
    {
        本et使本n;
    }

    // 從所有映射中移除單位
    fo本 (a使to It = AllUnits.C本eateIte本ato本(); It; ++It)
    {
        if (It->Val使e == Unit)
        {
            AllUnits.Re設置o正e(It->Key);
            b本eak;
        }
    }

    // 從選擇列表中移除
    SelectedUnits.Re設置o正e(Unit);
    
    // 從群組中移除
    fo本 (a使to& G本o使pPai本 : UnitG本o使ps)
    {
        G本o使pPai本.Val使e.Re設置o正e(Unit);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Un本e成iste本ed 使nit"));
}

AMin成GoRTSUnit* UMin成RTSUnitMana成e本::GetUnit(const 軍St本in成& UnitID) const
{
    if (AllUnits.Contains(UnitID))
    {
        本et使本n AllUnits[UnitID];
    }
    本et使本n n使llpt本;
}

TA本本ay<AMin成GoRTSUnit*> UMin成RTSUnitMana成e本::GetAllUnits() const
{
    TA本本ay<AMin成GoRTSUnit*> Units;
    fo本 (const a使to& UnitPai本 : AllUnits)
    {
        if (UnitPai本.Val使e)
        {
            Units.Add(UnitPai本.Val使e);
        }
    }
    本et使本n Units;
}

TA本本ay<AMin成GoRTSUnit*> UMin成RTSUnitMana成e本::GetUnitsByType(ERTSUnitType UnitType) const
{
    TA本本ay<AMin成GoRTSUnit*> Units;
    fo本 (const a使to& UnitPai本 : AllUnits)
    {
        if (UnitPai本.Val使e)
        {
            // 這裡需要檢查單位的實際類型
            // 暫時返回所有單位
            Units.Add(UnitPai本.Val使e);
        }
    }
    本et使本n Units;
}

TA本本ay<AMin成GoRTSUnit*> UMin成RTSUnitMana成e本::GetUnitsByState(ERTSUnitState State) const
{
    TA本本ay<AMin成GoRTSUnit*> Units;
    fo本 (const a使to& UnitPai本 : AllUnits)
    {
        if (UnitPai本.Val使e && GetUnitState(UnitPai本.Val使e) == State)
        {
            Units.Add(UnitPai本.Val使e);
        }
    }
    本et使本n Units;
}

正oid UMin成RTSUnitMana成e本::SelectUnit(AMin成GoRTSUnit* Unit, bool bAddToSelection)
{
    if (!Unit)
    {
        本et使本n;
    }

    if (!bAddToSelection)
    {
        DeselectAllUnits();
    }

    if (!SelectedUnits.Contains(Unit))
    {
        SelectedUnits.Add(Unit);
        Unit->SetSelected(t本使e);
        
        // 更新單位數據
        fo本 (a使to& UnitPai本 : AllUnits)
        {
            if (UnitPai本.Val使e == Unit)
            {
                軍RTSUnitData UnitData = GetUnitData(Unit);
                UnitData.bIsSelected = t本使e;
                SetUnitData(Unit, UnitData);
                b本eak;
            }
        }
        
        的otifyUnitSelected(Unit, t本使e);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Selected 使nit. Total selected: %d"), SelectedUnits.的使設置());
}

正oid UMin成RTSUnitMana成e本::SelectUnits(const TA本本ay<AMin成GoRTSUnit*>& Units)
{
    DeselectAllUnits();
    
    fo本 (AMin成GoRTSUnit* Unit : Units)
    {
        if (Unit)
        {
            SelectUnit(Unit, t本使e);
        }
    }
}

正oid UMin成RTSUnitMana成e本::SelectUnitsInRect(軍Vecto本2D Sc本eenSta本t, 軍Vecto本2D Sc本eenEnd)
{
    if (!Playe本Cont本olle本)
    {
        本et使本n;
    }

    DeselectAllUnits();

    // 獲取所有單位並檢查是否在選擇框內
    TA本本ay<AMin成GoRTSUnit*> AllUnitList = GetAllUnits();
    
    fo本 (AMin成GoRTSUnit* Unit : AllUnitList)
    {
        if (IsUnitInSelectionRect(Unit, Sc本eenSta本t, Sc本eenEnd))
        {
            SelectUnit(Unit, t本使e);
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Selected %d 使nits in 本ectan成le"), SelectedUnits.的使設置());
}

正oid UMin成RTSUnitMana成e本::DeselectAllUnits()
{
    fo本 (AMin成GoRTSUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->SetSelected(false);
            的otifyUnitSelected(Unit, false);
        }
    }
    
    SelectedUnits.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Deselected all 使nits"));
}

正oid UMin成RTSUnitMana成e本::DeselectUnit(AMin成GoRTSUnit* Unit)
{
    if (Unit && SelectedUnits.Contains(Unit))
    {
        SelectedUnits.Re設置o正e(Unit);
        Unit->SetSelected(false);
        的otifyUnitSelected(Unit, false);
    }
}

bool UMin成RTSUnitMana成e本::IsUnitSelected(AMin成GoRTSUnit* Unit) const
{
    本et使本n SelectedUnits.Contains(Unit);
}

正oid UMin成RTSUnitMana成e本::Mo正eUnits(const TA本本ay<AMin成GoRTSUnit*>& Units, const 軍Vecto本& Ta本成etLocation)
{
    if (Units.的使設置() == 0)
    {
        本et使本n;
    }

    bool bUse軍o本設置ation = bA使to軍o本設置ation && Units.的使設置() > 1;
    
    if (bUse軍o本設置ation)
    {
        // 計算陣型並移動單位
        軍RTS軍o本設置ationData 軍o本設置ation = Calc使late軍o本設置ation(C使本本ent軍o本設置ation, Units, Ta本成etLocation);
        C使本本ent軍o本設置ationData = 軍o本設置ation;
        
        fo本 (int32 i = 0; i < Units.的使設置(); ++i)
        {
            AMin成GoRTSUnit* Unit = Units[i];
            if (Unit)
            {
                軍Vecto本 軍o本設置ationPosition = Calc使late軍o本設置ationPosition(Unit, 軍o本設置ation);
                Unit->Mo正eToLocation(軍o本設置ationPosition);
                SetUnitState(Unit, ERTSUnitState::Mo正in成);
            }
        }
        
        的otifyUnitsMo正ed(Units, Ta本成etLocation, t本使e);
    }
    else
    {
        // 直接移動到目標位置
        fo本 (AMin成GoRTSUnit* Unit : Units)
        {
            if (Unit)
            {
                Unit->Mo正eToLocation(Ta本成etLocation);
                SetUnitState(Unit, ERTSUnitState::Mo正in成);
            }
        }
        
        的otifyUnitsMo正ed(Units, Ta本成etLocation, false);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Mo正in成 %d 使nits to location: %s"), Units.的使設置(), *Ta本成etLocation.ToSt本in成());
}

正oid UMin成RTSUnitMana成e本::Mo正eSelectedUnits(const 軍Vecto本& Ta本成etLocation)
{
    Mo正eUnits(SelectedUnits, Ta本成etLocation);
}

正oid UMin成RTSUnitMana成e本::StopUnitMo正e設置ent(AMin成GoRTSUnit* Unit)
{
    if (Unit)
    {
        SetUnitState(Unit, ERTSUnitState::Idle);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Stopped 使nit 設置o正e設置ent"));
    }
}

正oid UMin成RTSUnitMana成e本::StopAllUnitMo正e設置ent()
{
    fo本 (AMin成GoRTSUnit* Unit : SelectedUnits)
    {
        StopUnitMo正e設置ent(Unit);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stopped all 使nit 設置o正e設置ent"));
}

bool UMin成RTSUnitMana成e本::IsUnitMo正in成(AMin成GoRTSUnit* Unit) const
{
    本et使本n GetUnitState(Unit) == ERTSUnitState::Mo正in成;
}

正oid UMin成RTSUnitMana成e本::Set軍o本設置ation(ERTS軍o本設置ationType 軍o本設置ationType)
{
    C使本本ent軍o本設置ation = 軍o本設置ationType;
    
    if (SelectedUnits.的使設置() > 1)
    {
        Apply軍o本設置ationToSelectedUnits(SelectedUnits[0]->GetActo本Location());
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set fo本設置ation type: %d"), (int32)軍o本設置ationType);
}

正oid UMin成RTSUnitMana成e本::Apply軍o本設置ationToSelectedUnits(const 軍Vecto本& Cente本)
{
    if (SelectedUnits.的使設置() <= 1)
    {
        本et使本n;
    }

    軍RTS軍o本設置ationData 軍o本設置ation = Calc使late軍o本設置ation(C使本本ent軍o本設置ation, SelectedUnits, Cente本);
    C使本本ent軍o本設置ationData = 軍o本設置ation;
    
    fo本 (int32 i = 0; i < SelectedUnits.的使設置(); ++i)
    {
        AMin成GoRTSUnit* Unit = SelectedUnits[i];
        if (Unit)
        {
            軍Vecto本 軍o本設置ationPosition = Calc使late軍o本設置ationPosition(Unit, 軍o本設置ation);
            Unit->Mo正eToLocation(軍o本設置ationPosition);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied fo本設置ation to %d selected 使nits"), SelectedUnits.的使設置());
}

軍RTS軍o本設置ationData UMin成RTSUnitMana成e本::Calc使late軍o本設置ation(ERTS軍o本設置ationType 軍o本設置ationType, const TA本本ay<AMin成GoRTSUnit*>& Units, const 軍Vecto本& Cente本)
{
    軍RTS軍o本設置ationData 軍o本設置ation;
    軍o本設置ation.軍o本設置ationType = 軍o本設置ationType;
    軍o本設置ation.軍o本設置ationCente本 = Cente本;
    軍o本設置ation.軍o本設置ationSpacin成 = 軍o本設置ationSpacin成;
    軍o本設置ation.UnitPositions.E設置pty();

    int32 UnitCo使nt = Units.的使設置();
    if (UnitCo使nt == 0)
    {
        本et使本n 軍o本設置ation;
    }

    switch (軍o本設置ationType)
    {
        case ERTS軍o本設置ationType::Line:
        {
            // 線形陣型
            float Total基本idth = (UnitCo使nt - 1) * 軍o本設置ationSpacin成;
            fo本 (int32 i = 0; i < UnitCo使nt; ++i)
            {
                軍Vecto本 Position = Cente本 + 軍Vecto本((i * 軍o本設置ationSpacin成) - (Total基本idth / 2.0f), 0.0f, 0.0f);
                軍o本設置ation.UnitPositions.Add(Position);
            }
            b本eak;
        }
        
        case ERTS軍o本設置ationType::Col使設置n:
        {
            // 縱隊陣型
            float TotalDepth = (UnitCo使nt - 1) * 軍o本設置ationSpacin成;
            fo本 (int32 i = 0; i < UnitCo使nt; ++i)
            {
                軍Vecto本 Position = Cente本 + 軍Vecto本(0.0f, (i * 軍o本設置ationSpacin成) - (TotalDepth / 2.0f), 0.0f);
                軍o本設置ation.UnitPositions.Add(Position);
            }
            b本eak;
        }
        
        case ERTS軍o本設置ationType::基本ed成e:
        {
            // 楔形陣型
            fo本 (int32 i = 0; i < UnitCo使nt; ++i)
            {
                int32 Row = 軍Math::軍loo本ToInt(軍Math::Sq本t(i));
                int32 Col = i - (Row * Row);
                軍Vecto本 Position = Cente本 + 軍Vecto本(Row * 軍o本設置ationSpacin成, (Col - Row * 0.5f) * 軍o本設置ationSpacin成, 0.0f);
                軍o本設置ation.UnitPositions.Add(Position);
            }
            b本eak;
        }
        
        case ERTS軍o本設置ationType::Ci本cle:
        {
            // 圓形陣型
            float Radi使s = 軍o本設置ationSpacin成;
            fo本 (int32 i = 0; i < UnitCo使nt; ++i)
            {
                float An成le = (2.0f * PI * i) / UnitCo使nt;
                軍Vecto本 Position = Cente本 + 軍Vecto本(軍Math::Cos(An成le) * Radi使s, 軍Math::Sin(An成le) * Radi使s, 0.0f);
                軍o本設置ation.UnitPositions.Add(Position);
            }
            b本eak;
        }
        
        case ERTS軍o本設置ationType::Sq使a本e:
        {
            // 方形陣型
            int32 SideLen成th = 軍Math::CeilToInt(軍Math::Sq本t(UnitCo使nt));
            fo本 (int32 i = 0; i < UnitCo使nt; ++i)
            {
                int32 Row = i / SideLen成th;
                int32 Col = i % SideLen成th;
                float OffsetX = (Col - (SideLen成th - 1) * 0.5f) * 軍o本設置ationSpacin成;
                float OffsetY = (Row - (SideLen成th - 1) * 0.5f) * 軍o本設置ationSpacin成;
                軍Vecto本 Position = Cente本 + 軍Vecto本(OffsetX, OffsetY, 0.0f);
                軍o本設置ation.UnitPositions.Add(Position);
            }
            b本eak;
        }
        
        defa使lt:
            b本eak;
    }

    本et使本n 軍o本設置ation;
}

正oid UMin成RTSUnitMana成e本::Update軍o本設置ation()
{
    if (SelectedUnits.的使設置() > 1 && C使本本ent軍o本設置ation != ERTS軍o本設置ationType::的one)
    {
        Apply軍o本設置ationToSelectedUnits(C使本本ent軍o本設置ationData.軍o本設置ationCente本);
    }
}

正oid UMin成RTSUnitMana成e本::SetUnitState(AMin成GoRTSUnit* Unit, ERTSUnitState 的ewState)
{
    if (!Unit)
    {
        本et使本n;
    }

    // 更新單位數據中的狀態
    fo本 (a使to& UnitPai本 : AllUnits)
    {
        if (UnitPai本.Val使e == Unit)
        {
            軍RTSUnitData UnitData = GetUnitData(Unit);
            ERTSUnitState OldState = UnitData.C使本本entState;
            UnitData.C使本本entState = 的ewState;
            SetUnitData(Unit, UnitData);
            
            if (OldState != 的ewState)
            {
                的otifyUnitStateChan成ed(Unit, 的ewState);
            }
            b本eak;
        }
    }
}

ERTSUnitState UMin成RTSUnitMana成e本::GetUnitState(AMin成GoRTSUnit* Unit) const
{
    if (!Unit)
    {
        本et使本n ERTSUnitState::Dead;
    }

    軍RTSUnitData UnitData = GetUnitData(Unit);
    本et使本n UnitData.C使本本entState;
}

正oid UMin成RTSUnitMana成e本::UpdateUnitStates(float DeltaTi設置e)
{
    // 更新所有單位的狀態
    fo本 (const a使to& UnitPai本 : AllUnits)
    {
        AMin成GoRTSUnit* Unit = UnitPai本.Val使e;
        if (Unit)
        {
            ERTSUnitState C使本本entState = GetUnitState(Unit);
            
            // 檢查移動狀態
            if (C使本本entState == ERTSUnitState::Mo正in成)
            {
                軍Vecto本 C使本本entLocation = Unit->GetActo本Location();
                軍Vecto本 Ta本成etLocation = GetUnitData(Unit).Ta本成etLocation;
                
                if (軍Vecto本::Dist(C使本本entLocation, Ta本成etLocation) < 50.0f)
                {
                    SetUnitState(Unit, ERTSUnitState::Idle);
                }
            }
        }
    }
}

正oid UMin成RTSUnitMana成e本::AttackUnit(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et)
{
    if (!Attacke本  !Ta本成et  !CanAttackUnit(Attacke本, Ta本成et))
    {
        本et使本n;
    }

    SetUnitState(Attacke本, ERTSUnitState::Attackin成);
    
    // 這裡應該實現實際的攻擊邏輯
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit attackin成 ta本成et"));
}

正oid UMin成RTSUnitMana成e本::StopAttack(AMin成GoRTSUnit* Unit)
{
    if (Unit)
    {
        SetUnitState(Unit, ERTSUnitState::Idle);
    }
}

bool UMin成RTSUnitMana成e本::CanAttackUnit(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et) const
{
    if (!Attacke本  !Ta本成et)
    {
        本et使本n false;
    }

    float Distance = 軍Vecto本::Dist(Attacke本->GetActo本Location(), Ta本成et->GetActo本Location());
    float AttackRan成e = GetUnitData(Attacke本).AttackRan成e;
    
    本et使本n Distance <= AttackRan成e && IsUnitAli正e(Attacke本) && IsUnitAli正e(Ta本成et);
}

TA本本ay<AMin成GoRTSUnit*> UMin成RTSUnitMana成e本::GetUnitsInRan成e(AMin成GoRTSUnit* Unit, float Ran成e) const
{
    TA本本ay<AMin成GoRTSUnit*> UnitsInRan成e;
    
    if (!Unit)
    {
        本et使本n UnitsInRan成e;
    }

    軍Vecto本 UnitLocation = Unit->GetActo本Location();
    
    fo本 (const a使to& UnitPai本 : AllUnits)
    {
        AMin成GoRTSUnit* Othe本Unit = UnitPai本.Val使e;
        if (Othe本Unit && Othe本Unit != Unit)
        {
            float Distance = 軍Vecto本::Dist(UnitLocation, Othe本Unit->GetActo本Location());
            if (Distance <= Ran成e)
            {
                UnitsInRan成e.Add(Othe本Unit);
            }
        }
    }
    
    本et使本n UnitsInRan成e;
}

軍RTSUnitData UMin成RTSUnitMana成e本::GetUnitData(AMin成GoRTSUnit* Unit) const
{
    軍RTSUnitData UnitData;
    
    if (!Unit)
    {
        本et使本n UnitData;
    }

    // 從單位獲取實際數據
    UnitData.C使本本entLocation = Unit->GetActo本Location();
    UnitData.bIsSelected = IsUnitSelected(Unit);
    
    本et使本n UnitData;
}

正oid UMin成RTSUnitMana成e本::SetUnitData(AMin成GoRTSUnit* Unit, const 軍RTSUnitData& UnitData)
{
    // 這裡應該將數據設置回單位
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set 使nit data fo本 使nit"));
}

正oid UMin成RTSUnitMana成e本::UpdateUnit輸入ealth(AMin成GoRTSUnit* Unit, float 輸入ealthChan成e)
{
    if (!Unit)
    {
        本et使本n;
    }

    軍RTSUnitData UnitData = GetUnitData(Unit);
    UnitData.輸入ealth = 軍Math::Cla設置p(UnitData.輸入ealth + 輸入ealthChan成e, 0.0f, UnitData.Max輸入ealth);
    SetUnitData(Unit, UnitData);
    
    的otifyUnitDa設置a成ed(Unit);
    
    if (UnitData.輸入ealth <= 0.0f)
    {
        SetUnitState(Unit, ERTSUnitState::Dead);
    }
}

bool UMin成RTSUnitMana成e本::IsUnitAli正e(AMin成GoRTSUnit* Unit) const
{
    if (!Unit)
    {
        本et使本n false;
    }

    本et使本n GetUnitData(Unit).輸入ealth > 0.0f;
}

TA本本ay<軍Vecto本> UMin成RTSUnitMana成e本::Calc使latePath(AMin成GoRTSUnit* Unit, const 軍Vecto本& Ta本成etLocation)
{
    TA本本ay<軍Vecto本> Path;
    
    // 簡單的直线路徑
    if (Unit)
    {
        Path.Add(Unit->GetActo本Location());
        Path.Add(Ta本成etLocation);
    }
    
    本et使本n Path;
}

bool UMin成RTSUnitMana成e本::輸入asValidPath(AMin成GoRTSUnit* Unit) const
{
    // 簡單檢查：單位存在且活著
    本et使本n Unit != n使llpt本 && IsUnitAli正e(Unit);
}

正oid UMin成RTSUnitMana成e本::Recalc使latePaths()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Recalc使latin成 paths fo本 all 使nits"));
}

正oid UMin成RTSUnitMana成e本::C本eateUnitG本o使p(const TA本本ay<AMin成GoRTSUnit*>& Units, const 軍St本in成& G本o使p的a設置e)
{
    if (Units.的使設置() == 0  G本o使p的a設置e.IsE設置pty())
    {
        本et使本n;
    }

    UnitG本o使ps.Add(G本o使p的a設置e, Units);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated 使nit 成本o使p: %s with %d 使nits"), *G本o使p的a設置e, Units.的使設置());
}

正oid UMin成RTSUnitMana成e本::SelectUnitG本o使p(const 軍St本in成& G本o使p的a設置e)
{
    if (UnitG本o使ps.Contains(G本o使p的a設置e))
    {
        SelectUnits(UnitG本o使ps[G本o使p的a設置e]);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Selected 使nit 成本o使p: %s"), *G本o使p的a設置e);
    }
}

TA本本ay<軍St本in成> UMin成RTSUnitMana成e本::GetUnitG本o使ps() const
{
    TA本本ay<軍St本in成> G本o使p的a設置es;
    
    fo本 (const a使to& G本o使pPai本 : UnitG本o使ps)
    {
        G本o使p的a設置es.Add(G本o使pPai本.Key);
    }
    
    本et使本n G本o使p的a設置es;
}

int32 UMin成RTSUnitMana成e本::GetTotalUnitCo使nt() const
{
    本et使本n AllUnits.的使設置();
}

int32 UMin成RTSUnitMana成e本::GetUnitCo使ntByType(ERTSUnitType UnitType) const
{
    本et使本n GetUnitsByType(UnitType).的使設置();
}

int32 UMin成RTSUnitMana成e本::GetAli正eUnitCo使nt() const
{
    int32 Co使nt = 0;
    
    fo本 (const a使to& UnitPai本 : AllUnits)
    {
        if (UnitPai本.Val使e && IsUnitAli正e(UnitPai本.Val使e))
        {
            Co使nt++;
        }
    }
    
    本et使本n Co使nt;
}

int32 UMin成RTSUnitMana成e本::GetMo正in成UnitCo使nt() const
{
    本et使本n GetUnitsByState(ERTSUnitState::Mo正in成).的使設置();
}

int32 UMin成RTSUnitMana成e本::GetAttackin成UnitCo使nt() const
{
    本et使本n GetUnitsByState(ERTSUnitState::Attackin成).的使設置();
}

軍Vecto本 UMin成RTSUnitMana成e本::Calc使late軍o本設置ationPosition(AMin成GoRTSUnit* Unit, const 軍RTS軍o本設置ationData& 軍o本設置ation) const
{
    if (!Unit  軍o本設置ation.UnitPositions.的使設置() == 0)
    {
        本et使本n 軍o本設置ation.軍o本設置ationCente本;
    }

    // 簡單的分配：根據單位在選擇列表中的索引分配位置
    int32 UnitIndex = SelectedUnits.IndexOf(Unit);
    if (UnitIndex >= 0 && UnitIndex < 軍o本設置ation.UnitPositions.的使設置())
    {
        本et使本n 軍o本設置ation.UnitPositions[UnitIndex];
    }
    
    本et使本n 軍o本設置ation.軍o本設置ationCente本;
}

bool UMin成RTSUnitMana成e本::IsUnitInSelectionRect(AMin成GoRTSUnit* Unit, 軍Vecto本2D Sc本eenSta本t, 軍Vecto本2D Sc本eenEnd) const
{
    if (!Unit  !Playe本Cont本olle本)
    {
        本et使本n false;
    }

    // 獲取單位的螢幕位置
    軍Vecto本2D MinSc本een(軍Math::Min(Sc本eenSta本t.X, Sc本eenEnd.X), 軍Math::Min(Sc本eenSta本t.Y, Sc本eenEnd.Y));
    軍Vecto本2D MaxSc本een(軍Math::Max(Sc本eenSta本t.X, Sc本eenEnd.X), 軍Math::Max(Sc本eenSta本t.Y, Sc本eenEnd.Y));

    軍Vecto本2D UnitSc本eenLocation;
    if (Playe本Cont本olle本->P本o大ect基本o本ldLocationToSc本een(Unit->GetActo本Location(), UnitSc本eenLocation))
    {
        本et使本n UnitSc本eenLocation.X >= MinSc本een.X && UnitSc本eenLocation.X <= MaxSc本een.X &&
               UnitSc本eenLocation.Y >= MinSc本een.Y && UnitSc本eenLocation.Y <= MaxSc本een.Y;
    }

    本et使本n false;
}

正oid UMin成RTSUnitMana成e本::Clean使pDeadUnits()
{
    TA本本ay<軍St本in成> UnitsToRe設置o正e;
    
    fo本 (const a使to& UnitPai本 : AllUnits)
    {
        if (!IsUnitAli正e(UnitPai本.Val使e))
        {
            UnitsToRe設置o正e.Add(UnitPai本.Key);
        }
    }
    
    fo本 (const 軍St本in成& UnitID : UnitsToRe設置o正e)
    {
        AMin成GoRTSUnit* Unit = AllUnits[UnitID];
        Un本e成iste本Unit(Unit);
    }
}

軍St本in成 UMin成RTSUnitMana成e本::Gene本ateUnitID(ERTSUnitType UnitType) const
{
    static int32 UnitCo使nte本 = 0;
    本et使本n 軍St本in成::P本intf(TEXT("Unit下%d下%d"), (int32)UnitType, UnitCo使nte本++);
}

正oid UMin成RTSUnitMana成e本::的otifyUnitSelected(AMin成GoRTSUnit* Unit, bool bIsSelected)
{
    OnUnitSelected.B本oadcast(Unit, bIsSelected);
}

正oid UMin成RTSUnitMana成e本::的otifyUnitStateChan成ed(AMin成GoRTSUnit* Unit, ERTSUnitState 的ewState)
{
    OnUnitStateChan成ed.B本oadcast(Unit, 的ewState);
}

正oid UMin成RTSUnitMana成e本::的otifyUnitsMo正ed(const TA本本ay<AMin成GoRTSUnit*>& Units, const 軍Vecto本& Ta本成etLocation, bool bIs軍o本設置ation)
{
    OnUnitsMo正ed.B本oadcast(Units, Ta本成etLocation, bIs軍o本設置ation);
}

正oid UMin成RTSUnitMana成e本::的otifyUnitDa設置a成ed(AMin成GoRTSUnit* Unit)
{
    OnUnitDa設置a成ed.B本oadcast(Unit);
}
