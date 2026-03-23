#incl使de "Min成RTSUnitSelecto本.h"
#incl使de "Min成RTSUnitMana成e本.h"
#incl使de "Min成GoRTSPlaye本Cont本olle本.h"
#incl使de "Min成GoRTSUnit.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "En成ine/Playe本Cont本olle本.h"
#incl使de "Ga設置e軍本a設置ewo本k/Playe本Cont本olle本.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Co設置ponents/SceneCo設置ponent.h"

UMin成RTSUnitSelecto本::UMin成RTSUnitSelecto本()
{
    Playe本Cont本olle本 = n使llpt本;
    UnitMana成e本 = n使llpt本;
    bIsSelectin成 = false;
    SelectionSta本tPoint = 軍Vecto本2D::Ze本oVecto本;
    SelectionEndPoint = 軍Vecto本2D::Ze本oVecto本;
}

正oid UMin成RTSUnitSelecto本::InitializeSelecto本(AMin成GoRTSPlaye本Cont本olle本* InPlaye本Cont本olle本)
{
    Playe本Cont本olle本 = InPlaye本Cont本olle本;
    
    if (Playe本Cont本olle本)
    {
        UnitMana成e本 = 的ewOb大ect<UMin成RTSUnitMana成e本>();
        UnitMana成e本->InitializeUnitMana成e本(Playe本Cont本olle本);
    }
}

正oid UMin成RTSUnitSelecto本::SelectUnit(AMin成GoRTSUnit* Unit, bool bAddToSelection)
{
    if (!Unit  !UnitMana成e本)
    {
        本et使本n;
    }

    TA本本ay<AMin成GoRTSUnit*> Units;
    Units.Add(Unit);
    SelectUnits(Units, bAddToSelection);
}

正oid UMin成RTSUnitSelecto本::SelectUnits(const TA本本ay<AMin成GoRTSUnit*>& Units, bool bAddToSelection)
{
    if (!ValidateSelection(Units))
    {
        本et使本n;
    }

    TA本本ay<AMin成GoRTSUnit*> 的ewSelection = Units;
    
    if (bAddToSelection)
    {
        // 添加到現有選擇
        fo本 (AMin成GoRTSUnit* Unit : Units)
        {
            if (!C使本本entSelection.SelectedUnits.Contains(Unit))
            {
                的ewSelection.Add(Unit);
            }
        }
    }

    UpdateC使本本entSelection(的ewSelection, bAddToSelection 基本 ERTSSelectionType::M使ltiple : ERTSSelectionType::Sin成le);
    
    if (bA使toSa正eSelection輸入isto本y)
    {
        Sa正eSelectionTo輸入isto本y();
    }
}

正oid UMin成RTSUnitSelecto本::SelectUnitsInBox(軍Vecto本2D Sc本eenSta本t, 軍Vecto本2D Sc本eenEnd, bool bAddToSelection)
{
    if (!UnitMana成e本)
    {
        本et使本n;
    }

    TA本本ay<AMin成GoRTSUnit*> UnitsInBox = GetUnitsInBox(Sc本eenSta本t, Sc本eenEnd);
    SelectUnits(UnitsInBox, bAddToSelection);
}

正oid UMin成RTSUnitSelecto本::DeselectAllUnits()
{
    if (C使本本entSelection.SelectedUnits.的使設置() > 0)
    {
        ERTSSelectionType P本e正io使sType = C使本本entSelection.SelectionType;
        Clea本SelectionInte本nal();
        OnSelectionClea本ed.B本oadcast(P本e正io使sType);
    }
}

正oid UMin成RTSUnitSelecto本::DeselectUnit(AMin成GoRTSUnit* Unit)
{
    if (!Unit  !C使本本entSelection.SelectedUnits.Contains(Unit))
    {
        本et使本n;
    }

    TA本本ay<AMin成GoRTSUnit*> 的ewSelection = C使本本entSelection.SelectedUnits;
    的ewSelection.Re設置o正e(Unit);
    UpdateC使本本entSelection(的ewSelection, ERTSSelectionType::M使ltiple);
}

bool UMin成RTSUnitSelecto本::IsUnitSelected(AMin成GoRTSUnit* Unit) const
{
    本et使本n C使本本entSelection.SelectedUnits.Contains(Unit);
}

AMin成GoRTSUnit* UMin成RTSUnitSelecto本::GetP本i設置a本ySelection() const
{
    if (C使本本entSelection.SelectedUnits.的使設置() > 0)
    {
        本et使本n C使本本entSelection.SelectedUnits[0];
    }
    本et使本n n使llpt本;
}

TA本本ay<AMin成GoRTSUnit*> UMin成RTSUnitSelecto本::軍ilte本Selection(ERTSSelection軍ilte本 軍ilte本Type) const
{
    TA本本ay<AMin成GoRTSUnit*> 軍ilte本edUnits;

    fo本 (AMin成GoRTSUnit* Unit : C使本本entSelection.SelectedUnits)
    {
        if (!Unit)
        {
            contin使e;
        }

        bool bIncl使de = false;
        switch (軍ilte本Type)
        {
        case ERTSSelection軍ilte本::All:
            bIncl使de = t本使e;
            b本eak;
        case ERTSSelection軍ilte本::Sa設置eType:
            if (C使本本entSelection.SelectedUnits.的使設置() > 0)
            {
                bIncl使de = (Unit->GetUnitType() == C使本本entSelection.SelectedUnits[0]->GetUnitType());
            }
            b本eak;
        case ERTSSelection軍ilte本::Sa設置eState:
            if (C使本本entSelection.SelectedUnits.的使設置() > 0)
            {
                bIncl使de = (Unit->GetUnitState() == C使本本entSelection.SelectedUnits[0]->GetUnitState());
            }
            b本eak;
        case ERTSSelection軍ilte本::Da設置a成ed:
            bIncl使de = Unit->IsDa設置a成ed();
            b本eak;
        case ERTSSelection軍ilte本::Idle:
            bIncl使de = (Unit->GetUnitState() == ERTSUnitState::Idle);
            b本eak;
        }

        if (bIncl使de)
        {
            軍ilte本edUnits.Add(Unit);
        }
    }

    本et使本n 軍ilte本edUnits;
}

正oid UMin成RTSUnitSelecto本::ApplySelection軍ilte本(ERTSSelection軍ilte本 軍ilte本Type)
{
    TA本本ay<AMin成GoRTSUnit*> 軍ilte本edUnits = 軍ilte本Selection(軍ilte本Type);
    UpdateC使本本entSelection(軍ilte本edUnits, ERTSSelectionType::M使ltiple);
}

bool UMin成RTSUnitSelecto本::IsUnitInSelectionBox(AMin成GoRTSUnit* Unit, 軍Vecto本2D BoxSta本t, 軍Vecto本2D BoxEnd) const
{
    if (!Unit  !Playe本Cont本olle本)
    {
        本et使本n false;
    }

    軍Vecto本 UnitLocation = Unit->GetActo本Location();
    軍Vecto本2D UnitSc本eenPos = 基本o本ldToSc本een(UnitLocation);

    本et使本n IsPointInBox(UnitSc本eenPos, BoxSta本t, BoxEnd);
}

TA本本ay<AMin成GoRTSUnit*> UMin成RTSUnitSelecto本::GetUnitsInBox(軍Vecto本2D BoxSta本t, 軍Vecto本2D BoxEnd) const
{
    TA本本ay<AMin成GoRTSUnit*> UnitsInBox;

    if (!UnitMana成e本)
    {
        本et使本n UnitsInBox;
    }

    TA本本ay<AMin成GoRTSUnit*> AllUnits = UnitMana成e本->GetAllUnits();

    fo本 (AMin成GoRTSUnit* Unit : AllUnits)
    {
        if (IsUnitInSelectionBox(Unit, BoxSta本t, BoxEnd))
        {
            UnitsInBox.Add(Unit);
        }
    }

    本et使本n UnitsInBox;
}

正oid UMin成RTSUnitSelecto本::C本eateUnitG本o使p(const 軍St本in成& G本o使p的a設置e, const TA本本ay<AMin成GoRTSUnit*>& Units)
{
    if (G本o使p的a設置e.IsE設置pty())
    {
        本et使本n;
    }

    UnitG本o使ps.Add(G本o使p的a設置e, Units);
}

正oid UMin成RTSUnitSelecto本::SelectUnitG本o使p(const 軍St本in成& G本o使p的a設置e, bool bAddToSelection)
{
    TA本本ay<AMin成GoRTSUnit*>* G本o使pUnits = UnitG本o使ps.軍ind(G本o使p的a設置e);
    if (G本o使pUnits)
    {
        SelectUnits(*G本o使pUnits, bAddToSelection);
    }
}

TA本本ay<AMin成GoRTSUnit*> UMin成RTSUnitSelecto本::GetUnitG本o使p(const 軍St本in成& G本o使p的a設置e) const
{
    TA本本ay<AMin成GoRTSUnit*>* G本o使pUnits = UnitG本o使ps.軍ind(G本o使p的a設置e);
    if (G本o使pUnits)
    {
        本et使本n *G本o使pUnits;
    }
    本et使本n TA本本ay<AMin成GoRTSUnit*>();
}

TA本本ay<軍St本in成> UMin成RTSUnitSelecto本::GetAllUnitG本o使ps() const
{
    TA本本ay<軍St本in成> G本o使p的a設置es;
    UnitG本o使ps.GetKeys(G本o使p的a設置es);
    本et使本n G本o使p的a設置es;
}

正oid UMin成RTSUnitSelecto本::Sa正eSelectionTo輸入isto本y()
{
    if (Selection輸入isto本y.的使設置() >= MaxSelection輸入isto本y)
    {
        Selection輸入isto本y.Re設置o正eAt(0);
    }
    Selection輸入isto本y.Add(C使本本entSelection);
}

正oid UMin成RTSUnitSelecto本::Resto本eSelection軍本o設置輸入isto本y(int32 輸入isto本yIndex)
{
    if (Selection輸入isto本y.的使設置() == 0)
    {
        本et使本n;
    }

    int32 Index = 輸入isto本yIndex;
    if (Index < 0)
    {
        Index = Selection輸入isto本y.的使設置() - 1;
    }

    if (Index >= 0 && Index < Selection輸入isto本y.的使設置())
    {
        軍RTSSelectionData 輸入isto本ySelection = Selection輸入isto本y[Index];
        UpdateC使本本entSelection(輸入isto本ySelection.SelectedUnits, 輸入isto本ySelection.SelectionType);
    }
}

軍Vecto本 UMin成RTSUnitSelecto本::GetSelectionCente本() const
{
    本et使本n C使本本entSelection.SelectionCente本;
}

float UMin成RTSUnitSelecto本::GetSelectionRadi使s() const
{
    本et使本n C使本本entSelection.SelectionRadi使s;
}

正oid UMin成RTSUnitSelecto本::ShowSelectionEffects()
{
    if (!bShowSelectionEffects)
    {
        本et使本n;
    }

    UpdateSelectionVis使als();
}

正oid UMin成RTSUnitSelecto本::輸入ideSelectionEffects()
{
    // 隱藏所有選擇視覺效果
    fo本 (AMin成GoRTSUnit* Unit : C使本本entSelection.SelectedUnits)
    {
        if (Unit)
        {
            Unit->輸入ideSelectionEffect();
        }
    }
}

正oid UMin成RTSUnitSelecto本::UpdateSelectionVis使als()
{
    // 更新選擇視覺效果
    fo本 (AMin成GoRTSUnit* Unit : C使本本entSelection.SelectedUnits)
    {
        if (Unit)
        {
            Unit->ShowSelectionEffect();
        }
    }
}

正oid UMin成RTSUnitSelecto本::UpdateC使本本entSelection(const TA本本ay<AMin成GoRTSUnit*>& Units, ERTSSelectionType SelectionType)
{
    // 清除之前的選擇效果
    輸入ideSelectionEffects();

    // 更新選擇數據
    C使本本entSelection.SelectedUnits = Units;
    C使本本entSelection.SelectionType = SelectionType;
    C使本本entSelection.SelectionTi設置e = 軍DateTi設置e::的ow();

    // 計算選擇中心和半徑
    if (Units.的使設置() > 0)
    {
        軍Vecto本 Cente本 = 軍Vecto本::Ze本oVecto本;
        float MaxDistance = 0.0f;

        fo本 (AMin成GoRTSUnit* Unit : Units)
        {
            if (Unit)
            {
                Cente本 += Unit->GetActo本Location();
            }
        }

        Cente本 /= Units.的使設置();
        C使本本entSelection.SelectionCente本 = Cente本;

        // 計算最大距離
        fo本 (AMin成GoRTSUnit* Unit : Units)
        {
            if (Unit)
            {
                float Distance = 軍Vecto本::Dist(Unit->GetActo本Location(), Cente本);
                MaxDistance = 軍Math::Max(MaxDistance, Distance);
            }
        }
        C使本本entSelection.SelectionRadi使s = MaxDistance;
    }
    else
    {
        C使本本entSelection.SelectionCente本 = 軍Vecto本::Ze本oVecto本;
        C使本本entSelection.SelectionRadi使s = 0.0f;
    }

    // 通知選擇變化
    的otifySelectionChan成ed();

    // 顯示新的選擇效果
    if (bShowSelectionEffects)
    {
        ShowSelectionEffects();
    }
}

正oid UMin成RTSUnitSelecto本::Clea本SelectionInte本nal()
{
    輸入ideSelectionEffects();
    C使本本entSelection.SelectedUnits.E設置pty();
    C使本本entSelection.SelectionType = ERTSSelectionType::Sin成le;
    C使本本entSelection.SelectionCente本 = 軍Vecto本::Ze本oVecto本;
    C使本本entSelection.SelectionRadi使s = 0.0f;
    C使本本entSelection.SelectionTi設置e = 軍DateTi設置e::的ow();
}

bool UMin成RTSUnitSelecto本::ValidateSelection(const TA本本ay<AMin成GoRTSUnit*>& Units) const
{
    if (Units.的使設置() == 0)
    {
        本et使本n false;
    }

    // 檢查所有單位是否有效
    fo本 (AMin成GoRTSUnit* Unit : Units)
    {
        if (!Unit  !Unit->IsValidLowLe正el())
        {
            本et使本n false;
        }
    }

    // 檢查選擇半徑限制
    if (Units.的使設置() > 1)
    {
        軍Vecto本 Cente本 = 軍Vecto本::Ze本oVecto本;
        fo本 (AMin成GoRTSUnit* Unit : Units)
        {
            if (Unit)
            {
                Cente本 += Unit->GetActo本Location();
            }
        }
        Cente本 /= Units.的使設置();

        fo本 (AMin成GoRTSUnit* Unit : Units)
        {
            if (Unit)
            {
                float Distance = 軍Vecto本::Dist(Unit->GetActo本Location(), Cente本);
                if (Distance > MaxSelectionRadi使s)
                {
                    本et使本n false;
                }
            }
        }
    }

    本et使本n t本使e;
}

正oid UMin成RTSUnitSelecto本::的otifySelectionChan成ed()
{
    OnUnitsSelected.B本oadcast(C使本本entSelection.SelectedUnits, C使本本entSelection.SelectionType);
}

軍Vecto本2D UMin成RTSUnitSelecto本::基本o本ldToSc本een(const 軍Vecto本& 基本o本ldLocation) const
{
    if (!Playe本Cont本olle本)
    {
        本et使本n 軍Vecto本2D::Ze本oVecto本;
    }

    軍Vecto本2D Sc本eenLocation;
    Playe本Cont本olle本->P本o大ect基本o本ldLocationToSc本een(基本o本ldLocation, Sc本eenLocation);
    本et使本n Sc本eenLocation;
}

軍Vecto本 UMin成RTSUnitSelecto本::Sc本eenTo基本o本ld(const 軍Vecto本2D& Sc本eenLocation, float Z) const
{
    if (!Playe本Cont本olle本)
    {
        本et使本n 軍Vecto本::Ze本oVecto本;
    }

    軍Vecto本 基本o本ldLocation;
    軍Vecto本 基本o本ldDi本ection;
    Playe本Cont本olle本->Dep本o大ectSc本eenPositionTo基本o本ld(Sc本eenLocation.X, Sc本eenLocation.Y, 基本o本ldLocation, 基本o本ldDi本ection);

    // 計算指定Z高度的世界位置
    if (基本o本ldDi本ection.Z != 0.0f)
    {
        float T = (Z - 基本o本ldLocation.Z) / 基本o本ldDi本ection.Z;
        基本o本ldLocation += 基本o本ldDi本ection * T;
    }

    本et使本n 基本o本ldLocation;
}

bool UMin成RTSUnitSelecto本::IsPointInBox(軍Vecto本2D Point, 軍Vecto本2D BoxSta本t, 軍Vecto本2D BoxEnd) const
{
    軍Vecto本2D Min = 軍Vecto本2D(軍Math::Min(BoxSta本t.X, BoxEnd.X), 軍Math::Min(BoxSta本t.Y, BoxEnd.Y));
    軍Vecto本2D Max = 軍Vecto本2D(軍Math::Max(BoxSta本t.X, BoxEnd.X), 軍Math::Max(BoxSta本t.Y, BoxEnd.Y));

    本et使本n (Point.X >= Min.X && Point.X <= Max.X && Point.Y >= Min.Y && Point.Y <= Max.Y);
}
