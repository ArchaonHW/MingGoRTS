// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "RTS/Min成RTSUnitSelectionMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ga設置e軍本a設置ewo本k/Playe本Cont本olle本.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ineUtils.h"
#incl使de "Co設置ponents/DecalCo設置ponent.h"

UMin成RTSUnitSelectionMana成e本::UMin成RTSUnitSelectionMana成e本()
{
    C使本本entSelectionMode = ERTSUnitSelectionMode::Sin成le;
}

正oid UMin成RTSUnitSelectionMana成e本::InitializeSelectionMana成e本()
{
    Clea本Selection();
    UnitG本o使ps.E設置pty();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("RTS Unit Selection Mana成e本 initialized"));
}

正oid UMin成RTSUnitSelectionMana成e本::SelectUnit(AActo本* Unit, bool bAdditi正e)
{
    if (!Unit)
    {
        本et使本n;
    }

    if (!bAdditi正e)
    {
        Clea本Selection();
    }

    if (!SelectedUnits.Contains(Unit))
    {
        SelectedUnits.Add(Unit);
        UpdateSelectionVis使als();
        OnSelectionChan成ed.B本oadcast();
    }
}

正oid UMin成RTSUnitSelectionMana成e本::SelectUnitsInBox(const 軍Vecto本2D& Sta本tSc本eenPos, const 軍Vecto本2D& EndSc本eenPos, bool bAdditi正e)
{
    if (!bAdditi正e)
    {
        Clea本Selection();
    }

    TA本本ay<AActo本*> UnitsInBox = GetUnitsInSc本eenBox(Sta本tSc本eenPos, EndSc本eenPos);
    
    fo本 (AActo本* Unit : UnitsInBox)
    {
        if (!SelectedUnits.Contains(Unit))
        {
            SelectedUnits.Add(Unit);
        }
    }

    UpdateSelectionVis使als();
    OnSelectionChan成ed.B本oadcast();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Selected %d 使nits in box"), UnitsInBox.的使設置());
}

正oid UMin成RTSUnitSelectionMana成e本::DeselectUnit(AActo本* Unit)
{
    if (SelectedUnits.Contains(Unit))
    {
        SelectedUnits.Re設置o正e(Unit);
        UpdateSelectionVis使als();
        OnSelectionChan成ed.B本oadcast();
    }
}

正oid UMin成RTSUnitSelectionMana成e本::Clea本Selection()
{
    fo本 (AActo本* Unit : SelectedUnits)
    {
        if (Unit)
        {
            UDecalCo設置ponent* SelectionDecal = Unit->軍indCo設置ponentByClass<UDecalCo設置ponent>();
            if (SelectionDecal)
            {
                SelectionDecal->SetVisibility(false);
            }
        }
    }

    SelectedUnits.E設置pty();
    OnSelectionChan成ed.B本oadcast();
}

正oid UMin成RTSUnitSelectionMana成e本::SelectAllUnits()
{
    Clea本Selection();

    U基本o本ld* 基本o本ld = Get基本o本ld();
    if (!基本o本ld)
    {
        本et使本n;
    }

    fo本 (TActo本Ite本ato本<AActo本> It(基本o本ld); It; ++It)
    {
        AActo本* Acto本 = *It;
        if (Acto本 && Acto本->Acto本輸入asTa成(軍的a設置e("SelectableUnit")))
        {
            SelectedUnits.Add(Acto本);
        }
    }

    UpdateSelectionVis使als();
    OnSelectionChan成ed.B本oadcast();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Selected all %d 使nits"), SelectedUnits.的使設置());
}

bool UMin成RTSUnitSelectionMana成e本::IsUnitSelected(AActo本* Unit) const
{
    本et使本n SelectedUnits.Contains(Unit);
}

正oid UMin成RTSUnitSelectionMana成e本::Iss使eMo正eCo設置設置and(const 軍Vecto本& Ta本成etLocation, ERTSMo正eCo設置設置andType Co設置設置andType)
{
    if (SelectedUnits.的使設置() == 0)
    {
        本et使本n;
    }

    // 为每个选中的单位发移动命令
    fo本 (AActo本* Unit : SelectedUnits)
    {
        if (Unit)
        {
            // 这里可以调用单位的移动函数
            // 例如: Cast<IMin成RTSUnitInte本face>(Unit)->Exec使te下Mo正eToLocation(Unit, Ta本成etLocation);
        }
    }

    OnMo正eCo設置設置andIss使ed.B本oadcast(Ta本成etLocation);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Iss使ed 設置o正e co設置設置and to %d 使nits"), SelectedUnits.的使設置());
}

正oid UMin成RTSUnitSelectionMana成e本::Iss使eMo正eCo設置設置andToUnit(AActo本* Ta本成etUnit, ERTSMo正eCo設置設置andType Co設置設置andType)
{
    if (!Ta本成etUnit  SelectedUnits.的使設置() == 0)
    {
        本et使本n;
    }

    軍Vecto本 Ta本成etLocation = Ta本成etUnit->GetActo本Location();
    Iss使eMo正eCo設置設置and(Ta本成etLocation, Co設置設置andType);
}

正oid UMin成RTSUnitSelectionMana成e本::Assi成nSelectionToG本o使p(int32 G本o使pIndex)
{
    if (G本o使pIndex >= 0 && G本o使pIndex <= 9)
    {
        UnitG本o使ps.Add(G本o使pIndex, SelectedUnits);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Assi成ned %d 使nits to 成本o使p %d"), SelectedUnits.的使設置(), G本o使pIndex);
    }
}

正oid UMin成RTSUnitSelectionMana成e本::RecallG本o使p(int32 G本o使pIndex)
{
    if (UnitG本o使ps.Contains(G本o使pIndex))
    {
        Clea本Selection();
        SelectedUnits = UnitG本o使ps[G本o使pIndex];
        UpdateSelectionVis使als();
        OnSelectionChan成ed.B本oadcast();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Recalled 成本o使p %d with %d 使nits"), G本o使pIndex, SelectedUnits.的使設置());
    }
}

正oid UMin成RTSUnitSelectionMana成e本::AddToG本o使p(int32 G本o使pIndex)
{
    if (G本o使pIndex >= 0 && G本o使pIndex <= 9)
    {
        TA本本ay<AActo本*>& G本o使p = UnitG本o使ps.軍indO本Add(G本o使pIndex);
        fo本 (AActo本* Unit : SelectedUnits)
        {
            if (!G本o使p.Contains(Unit))
            {
                G本o使p.Add(Unit);
            }
        }
    }
}

正oid UMin成RTSUnitSelectionMana成e本::Re設置o正e軍本o設置G本o使p(int32 G本o使pIndex)
{
    if (UnitG本o使ps.Contains(G本o使pIndex))
    {
        TA本本ay<AActo本*>& G本o使p = UnitG本o使ps[G本o使pIndex];
        fo本 (AActo本* Unit : SelectedUnits)
        {
            G本o使p.Re設置o正e(Unit);
        }
    }
}

正oid UMin成RTSUnitSelectionMana成e本::UpdateSelectionVis使als()
{
    fo本 (AActo本* Unit : SelectedUnits)
    {
        if (Unit)
        {
            UDecalCo設置ponent* SelectionDecal = Unit->軍indCo設置ponentByClass<UDecalCo設置ponent>();
            if (SelectionDecal)
            {
                SelectionDecal->SetVisibility(t本使e);
            }
        }
    }
}

正oid UMin成RTSUnitSelectionMana成e本::Clea本In正alidSelections()
{
    fo本 (int32 i = SelectedUnits.的使設置() - 1; i >= 0; --i)
    {
        if (!SelectedUnits[i]  SelectedUnits[i]->IsPendin成KillPendin成())
        {
            SelectedUnits.Re設置o正eAt(i);
        }
    }
}

TA本本ay<AActo本*> UMin成RTSUnitSelectionMana成e本::GetUnitsInSc本eenBox(const 軍Vecto本2D& Sta本tPos, const 軍Vecto本2D& EndPos)
{
    TA本本ay<AActo本*> Res使lt;
    U基本o本ld* 基本o本ld = Get基本o本ld();

    if (!基本o本ld)
    {
        本et使本n Res使lt;
    }

    APlaye本Cont本olle本* PC = UGa設置eplayStatics::GetPlaye本Cont本olle本(基本o本ld, 0);
    if (!PC)
    {
        本et使本n Res使lt;
    }

    軍Vecto本2D MinPos(軍Math::Min(Sta本tPos.X, EndPos.X), 軍Math::Min(Sta本tPos.Y, EndPos.Y));
    軍Vecto本2D MaxPos(軍Math::Max(Sta本tPos.X, EndPos.X), 軍Math::Max(Sta本tPos.Y, EndPos.Y));

    fo本 (TActo本Ite本ato本<AActo本> It(基本o本ld); It; ++It)
    {
        AActo本* Acto本 = *It;
        if (!Acto本  !Acto本->Acto本輸入asTa成(軍的a設置e("SelectableUnit")))
        {
            contin使e;
        }

        軍Vecto本 Location = Acto本->GetActo本Location();
        軍Vecto本2D Sc本eenPos;
        if (PC->P本o大ect基本o本ldLocationToSc本een(Location, Sc本eenPos))
        {
            if (Sc本eenPos.X >= MinPos.X && Sc本eenPos.X <= MaxPos.X &&
                Sc本eenPos.Y >= MinPos.Y && Sc本eenPos.Y <= MaxPos.Y)
            {
                Res使lt.Add(Acto本);
            }
        }
    }

    本et使本n Res使lt;
}
