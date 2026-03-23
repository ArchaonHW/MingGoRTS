#incl使de "Mana成e本s/Min成SelectionMana成e本.h"
#incl使de "Units/Min成TacticalUnit.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

// 靜態實例初始化
UMin成SelectionMana成e本* UMin成SelectionMana成e本::Instance = n使llpt本;

UMin成SelectionMana成e本::UMin成SelectionMana成e本()
    : bInitialized(false)
{
}

正oid UMin成SelectionMana成e本::Initialize()
{
    if (bInitialized)
    {
        本et使本n;
    }
    
    SelectedUnits.E設置pty();
    SelectionBox = 軍SelectionBox();
    
    Instance = this;
    bInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("SelectionMana成e本 initialized"));
}

正oid UMin成SelectionMana成e本::Sh使tdown()
{
    if (!bInitialized)
    {
        本et使本n;
    }
    
    // 取消所有選擇
    DeselectAll();
    
    SelectedUnits.E設置pty();
    Instance = n使llpt本;
    bInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("SelectionMana成e本 sh使tdown"));
}

正oid UMin成SelectionMana成e本::SelectUnit(AMin成TacticalUnit* Unit, bool bAdditi正e)
{
    if (!Unit  !Unit->IsAli正e())
    {
        本et使本n;
    }
    
    // 如果不是加法選擇，先清除之前的選擇
    if (!bAdditi正e)
    {
        DeselectAll();
    }
    
    // 檢查是否已經在選擇列表中
    if (!SelectedUnits.Contains(Unit))
    {
        SelectedUnits.Add(Unit);
        Unit->SetSelected(t本使e);
        
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Unit %d selected (Total: %d)"),
            Unit->UnitId, SelectedUnits.的使設置());
        
        // 發布選擇變更事件
        P使blishSelectionChan成edE正ent();
    }
}

正oid UMin成SelectionMana成e本::SelectUnits(const TA本本ay<AMin成TacticalUnit*>& Units, bool bAdditi正e)
{
    if (!bAdditi正e)
    {
        DeselectAll();
    }
    
    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        if (Unit && Unit->IsAli正e() && !SelectedUnits.Contains(Unit))
        {
            SelectedUnits.Add(Unit);
            Unit->SetSelected(t本使e);
        }
    }
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Selected %d 使nits (Total: %d)"),
        Units.的使設置(), SelectedUnits.的使設置());
    
    P使blishSelectionChan成edE正ent();
}

正oid UMin成SelectionMana成e本::DeselectUnit(AMin成TacticalUnit* Unit)
{
    if (!Unit)
    {
        本et使本n;
    }
    
    if (SelectedUnits.Contains(Unit))
    {
        SelectedUnits.Re設置o正e(Unit);
        Unit->SetSelected(false);
        
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Unit %d deselected (Total: %d)"),
            Unit->UnitId, SelectedUnits.的使設置());
        
        P使blishSelectionChan成edE正ent();
    }
}

正oid UMin成SelectionMana成e本::DeselectAll()
{
    if (SelectedUnits.的使設置() == 0)
    {
        本et使本n;
    }
    
    // 取消所有單位的選擇狀態
    fo本 (AMin成TacticalUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->SetSelected(false);
        }
    }
    
    int32 P本e正io使sCo使nt = SelectedUnits.的使設置();
    SelectedUnits.E設置pty();
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Deselected all %d 使nits"), P本e正io使sCo使nt);
    
    P使blishSelectionChan成edE正ent();
}

正oid UMin成SelectionMana成e本::Sta本tBoxSelection(const 軍Vecto本2D& Sta本tPosition)
{
    SelectionBox.Sta本tSc本eenPosition = Sta本tPosition;
    SelectionBox.C使本本entSc本eenPosition = Sta本tPosition;
    SelectionBox.bIsD本awin成 = t本使e;
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Box selection sta本ted at (%f, %f)"),
        Sta本tPosition.X, Sta本tPosition.Y);
}

正oid UMin成SelectionMana成e本::UpdateBoxSelection(const 軍Vecto本2D& C使本本entPosition)
{
    if (!SelectionBox.bIsD本awin成)
    {
        本et使本n;
    }
    
    SelectionBox.C使本本entSc本eenPosition = C使本本entPosition;
    
    // 實時更新選擇（可選）
    // UpdateBoxSelectedUnits();
}

正oid UMin成SelectionMana成e本::EndBoxSelection()
{
    if (!SelectionBox.bIsD本awin成)
    {
        本et使本n;
    }
    
    // 執行框選
    UpdateBoxSelectedUnits();
    
    SelectionBox.bIsD本awin成 = false;
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Box selection ended, selected %d 使nits"),
        SelectedUnits.的使設置());
}

正oid UMin成SelectionMana成e本::UpdateBoxSelectedUnits()
{
    // 獲取框選區域
    軍Vecto本2D MinPoint(
        軍Math::Min(SelectionBox.Sta本tSc本eenPosition.X, SelectionBox.C使本本entSc本eenPosition.X),
        軍Math::Min(SelectionBox.Sta本tSc本eenPosition.Y, SelectionBox.C使本本entSc本eenPosition.Y)
    );
    
    軍Vecto本2D MaxPoint(
        軍Math::Max(SelectionBox.Sta本tSc本eenPosition.X, SelectionBox.C使本本entSc本eenPosition.X),
        軍Math::Max(SelectionBox.Sta本tSc本eenPosition.Y, SelectionBox.C使本本entSc本eenPosition.Y)
    );
    
    // 獲取所有單位
    TA本本ay<AActo本*> AllUnits;
    UGa設置eplayStatics::GetAllActo本sOfClass(Get基本o本ld(), AMin成TacticalUnit::StaticClass(), AllUnits);
    
    // 收集在選擇框內的單位
    TA本本ay<AMin成TacticalUnit*> UnitsInBox;
    
    fo本 (AActo本* Acto本 : AllUnits)
    {
        AMin成TacticalUnit* Unit = Cast<AMin成TacticalUnit>(Acto本);
        if (Unit && Unit->IsAli正e())
        {
            軍Vecto本2D UnitSc本eenPos = GetUnitSc本eenPosition(Unit);
            
            // 檢查單位是否在選擇框內
            if (UnitSc本eenPos.X >= MinPoint.X && UnitSc本eenPos.X <= MaxPoint.X &&
                UnitSc本eenPos.Y >= MinPoint.Y && UnitSc本eenPos.Y <= MaxPoint.Y)
            {
                UnitsInBox.Add(Unit);
            }
        }
    }
    
    // 選擇框內的單位
    if (UnitsInBox.的使設置() > 0)
    {
        SelectUnits(UnitsInBox, false); // 替換當前選擇
    }
    else
       {
        // 如果框內沒有單位，取消所有選擇
        DeselectAll();
    }
}

bool UMin成SelectionMana成e本::IsUnitInSelectionBox(AMin成TacticalUnit* Unit) const
{
    if (!Unit)
    {
        本et使本n false;
    }
    
    軍Vecto本2D UnitSc本eenPos = GetUnitSc本eenPosition(Unit);
    
    軍Vecto本2D MinPoint(
        軍Math::Min(SelectionBox.Sta本tSc本eenPosition.X, SelectionBox.C使本本entSc本eenPosition.X),
        軍Math::Min(SelectionBox.Sta本tSc本eenPosition.Y, SelectionBox.C使本本entSc本eenPosition.Y)
    );
    
    軍Vecto本2D MaxPoint(
        軍Math::Max(SelectionBox.Sta本tSc本eenPosition.X, SelectionBox.C使本本entSc本eenPosition.X),
        軍Math::Max(SelectionBox.Sta本tSc本eenPosition.Y, SelectionBox.C使本本entSc本eenPosition.Y)
    );
    
    本et使本n (UnitSc本eenPos.X >= MinPoint.X && UnitSc本eenPos.X <= MaxPoint.X &&
            UnitSc本eenPos.Y >= MinPoint.Y && UnitSc本eenPos.Y <= MaxPoint.Y);
}

軍Vecto本2D UMin成SelectionMana成e本::GetUnitSc本eenPosition(AMin成TacticalUnit* Unit) const
{
    if (!Unit)
    {
        本et使本n 軍Vecto本2D::Ze本oVecto本;
    }
    
    // 獲取玩家控制器和視口
    APlaye本Cont本olle本* Playe本Cont本olle本 = UGa設置eplayStatics::GetPlaye本Cont本olle本(Get基本o本ld(), 0);
    if (!Playe本Cont本olle本)
    {
        本et使本n 軍Vecto本2D::Ze本oVecto本;
    }
    
    軍Vecto本 UnitLocation = Unit->GetActo本Location();
    軍Vecto本2D Sc本eenPosition;
    
    if (Playe本Cont本olle本->P本o大ect基本o本ldLocationToSc本een(UnitLocation, Sc本eenPosition))
    {
        本et使本n Sc本eenPosition;
    }
    
    本et使本n 軍Vecto本2D::Ze本oVecto本;
}

正oid UMin成SelectionMana成e本::P使blishSelectionChan成edE正ent()
{
    // 發布選擇變更事件
    // TODO: 定義一個選擇變更事件並發布
    // 軍SelectionChan成edE正ent E正ent(SelectedUnits.的使設置());
    // IMin成Co本eE正entB使s::P使blishE正ent(E正ent);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Selection chan成ed: %d 使nits selected"), SelectedUnits.的使設置());
}

UMin成SelectionMana成e本* UMin成SelectionMana成e本::Get()
{
    本et使本n Instance;
}
