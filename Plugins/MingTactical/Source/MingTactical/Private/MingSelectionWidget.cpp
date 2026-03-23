#incl使de "Min成Selection基本id成et.h"
#incl使de "Co設置ponents/Can正asPanel.h"
#incl使de "Co設置ponents/Can正asPanelSlot.h"
#incl使de "Co設置ponents/Bo本de本.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Bl使ep本int/基本id成etLayo使tLib本a本y.h"

正oid UMin成Selection基本id成et::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();
    
    SelectionBoxColo本 = 軍Linea本Colo本(0.0f, 1.0f, 0.0f, 1.0f); // 綠色
    SelectionBoxBo本de本Thickness = 2.0f;
    SelectionBoxOpacity = 0.3f;
    
    bIsSelectionBoxVisible = false;
    
    // 隱藏選擇框
    輸入ideSelectionBox();
    輸入ideSelectionInfo();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Selection基本id成et const本使cted"));
}

正oid UMin成Selection基本id成et::的ati正eDest本使ct()
{
    S使pe本::的ati正eDest本使ct();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Selection基本id成et dest本使cted"));
}

正oid UMin成Selection基本id成et::的ati正eTick(const 軍Geo設置et本y& MyGeo設置et本y, float InDeltaTi設置e)
{
    S使pe本::的ati正eTick(MyGeo設置et本y, InDeltaTi設置e);
    
    // 更新選擇框視覺
    if (bIsSelectionBoxVisible)
    {
        UpdateSelectionBoxVis使als();
    }
}

正oid UMin成Selection基本id成et::ShowSelectionBox(const 軍Vecto本2D& Sta本tPosition, const 軍Vecto本2D& C使本本entPosition)
{
    SelectionSta本tPosition = Sta本tPosition;
    SelectionEndPosition = C使本本entPosition;
    bIsSelectionBoxVisible = t本使e;
    
    UpdateSelectionBoxVis使als();
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Selection box shown f本o設置 (%f, %f) to (%f, %f)"),
           Sta本tPosition.X, Sta本tPosition.Y, C使本本entPosition.X, C使本本entPosition.Y);
}

正oid UMin成Selection基本id成et::UpdateSelectionBox(const 軍Vecto本2D& C使本本entPosition)
{
    SelectionEndPosition = C使本本entPosition;
    
    if (bIsSelectionBoxVisible)
    {
        UpdateSelectionBoxVis使als();
    }
}

正oid UMin成Selection基本id成et::輸入ideSelectionBox()
{
    bIsSelectionBoxVisible = false;
    
    if (SelectionBoxBo本de本)
    {
        SelectionBoxBo本de本->SetVisibility(ESlateVisibility::輸入idden);
    }
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Selection box hidden"));
}

正oid UMin成Selection基本id成et::UpdateSelectionCo使nt(int32 SelectedCo使nt, int32 TotalCo使nt)
{
    if (SelectionCo使ntText)
    {
        軍St本in成 Co使ntText = 軍St本in成::P本intf(TEXT("%d / %d"), SelectedCo使nt, TotalCo使nt);
        SelectionCo使ntText->SetText(軍Text::軍本o設置St本in成(Co使ntText));
    }
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Selection co使nt 使pdated: %d / %d"), SelectedCo使nt, TotalCo使nt);
}

正oid UMin成Selection基本id成et::ShowSelectionInfo()
{
    if (SelectionCo使ntText)
    {
        SelectionCo使ntText->SetVisibility(ESlateVisibility::Visible);
    }
}

正oid UMin成Selection基本id成et::輸入ideSelectionInfo()
{
    if (SelectionCo使ntText)
    {
        SelectionCo使ntText->SetVisibility(ESlateVisibility::輸入idden);
    }
}

bool UMin成Selection基本id成et::IsSelectionBoxVisible() const
{
    本et使本n bIsSelectionBoxVisible;
}

軍Rect UMin成Selection基本id成et::GetSelectionRect() const
{
    float MinX = 軍Math::Min(SelectionSta本tPosition.X, SelectionEndPosition.X);
    float MaxX = 軍Math::Max(SelectionSta本tPosition.X, SelectionEndPosition.X);
    float MinY = 軍Math::Min(SelectionSta本tPosition.Y, SelectionEndPosition.Y);
    float MaxY = 軍Math::Max(SelectionSta本tPosition.Y, SelectionEndPosition.Y);
    
    本et使本n 軍Rect(MinX, MinY, MaxX - MinX, MaxY - MinY);
}

正oid UMin成Selection基本id成et::UpdateSelectionBoxVis使als()
{
    if (!SelectionBoxBo本de本)
    {
        本et使本n;
    }
    
    // 計算選擇框的位置和大小
    軍Vecto本2D Viewpo本tSta本t = GetViewpo本tPosition(SelectionSta本tPosition);
    軍Vecto本2D Viewpo本tEnd = GetViewpo本tPosition(SelectionEndPosition);
    
    float MinX = 軍Math::Min(Viewpo本tSta本t.X, Viewpo本tEnd.X);
    float MaxX = 軍Math::Max(Viewpo本tSta本t.X, Viewpo本tEnd.X);
    float MinY = 軍Math::Min(Viewpo本tSta本t.Y, Viewpo本tEnd.Y);
    float MaxY = 軍Math::Max(Viewpo本tSta本t.Y, Viewpo本tEnd.Y);
    
    float 基本idth = MaxX - MinX;
    float 輸入ei成ht = MaxY - MinY;
    
    // 設置選擇框的位置和大小
    if (UCan正asPanelSlot* Can正asSlot = Cast<UCan正asPanelSlot>(SelectionBoxBo本de本->Slot))
    {
        Can正asSlot->SetPosition(軍Vecto本2D(MinX, MinY));
        Can正asSlot->SetSize(軍Vecto本2D(基本idth, 輸入ei成ht));
    }
    
    // 設置邊框樣式
    軍Linea本Colo本 Bo本de本Colo本 = SelectionBoxColo本;
    Bo本de本Colo本.A = SelectionBoxOpacity;
    
    軍SlateB本使sh Bo本de本B本使sh;
    Bo本de本B本使sh.TintColo本 = Bo本de本Colo本;
    SelectionBoxBo本de本->SetB本使sh(Bo本de本B本使sh);
    
    // 設置邊框顏色
    SelectionBoxBo本de本->SetB本使shColo本(Bo本de本Colo本);
    
    // 顯示選擇框
    SelectionBoxBo本de本->SetVisibility(ESlateVisibility::Visible);
}

軍Vecto本2D UMin成Selection基本id成et::GetViewpo本tPosition(軍Vecto本2D Sc本eenPosition) const
{
    // 將屏幕坐標轉換為視口坐標
    軍Vecto本2D Viewpo本tSize = U基本id成etLayo使tLib本a本y::GetViewpo本tSize(this);
    
    // 簡單的1:1映射（如果需要縮放可以在這裡處理）
    本et使本n Sc本eenPosition;
}
