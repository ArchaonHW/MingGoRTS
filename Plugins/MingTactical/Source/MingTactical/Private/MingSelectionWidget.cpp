#include "MingSelectionWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UMingSelectionWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    SelectionBoxColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); // 綠色
    SelectionBoxBorderThickness = 2.0f;
    SelectionBoxOpacity = 0.3f;
    
    bIsSelectionBoxVisible = false;
    
    // 隱藏選擇框
    HideSelectionBox();
    HideSelectionInfo();
    
    UE_LOG(LogTemp, Log, TEXT("MingSelectionWidget constructed"));
}

void UMingSelectionWidget::NativeDestruct()
{
    Super::NativeDestruct();
    
    UE_LOG(LogTemp, Log, TEXT("MingSelectionWidget destructed"));
}

void UMingSelectionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    // 更新選擇框視覺
    if (bIsSelectionBoxVisible)
    {
        UpdateSelectionBoxVisuals();
    }
}

void UMingSelectionWidget::ShowSelectionBox(const FVector2D& StartPosition, const FVector2D& CurrentPosition)
{
    SelectionStartPosition = StartPosition;
    SelectionEndPosition = CurrentPosition;
    bIsSelectionBoxVisible = true;
    
    UpdateSelectionBoxVisuals();
    
    UE_LOG(LogTemp, Verbose, TEXT("Selection box shown from (%f, %f) to (%f, %f)"),
           StartPosition.X, StartPosition.Y, CurrentPosition.X, CurrentPosition.Y);
}

void UMingSelectionWidget::UpdateSelectionBox(const FVector2D& CurrentPosition)
{
    SelectionEndPosition = CurrentPosition;
    
    if (bIsSelectionBoxVisible)
    {
        UpdateSelectionBoxVisuals();
    }
}

void UMingSelectionWidget::HideSelectionBox()
{
    bIsSelectionBoxVisible = false;
    
    if (SelectionBoxBorder)
    {
        SelectionBoxBorder->SetVisibility(ESlateVisibility::Hidden);
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Selection box hidden"));
}

void UMingSelectionWidget::UpdateSelectionCount(int32 SelectedCount, int32 TotalCount)
{
    if (SelectionCountText)
    {
        FString CountText = FString::Printf(TEXT("%d / %d"), SelectedCount, TotalCount);
        SelectionCountText->SetText(FText::FromString(CountText));
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Selection count updated: %d / %d"), SelectedCount, TotalCount);
}

void UMingSelectionWidget::ShowSelectionInfo()
{
    if (SelectionCountText)
    {
        SelectionCountText->SetVisibility(ESlateVisibility::Visible);
    }
}

void UMingSelectionWidget::HideSelectionInfo()
{
    if (SelectionCountText)
    {
        SelectionCountText->SetVisibility(ESlateVisibility::Hidden);
    }
}

bool UMingSelectionWidget::IsSelectionBoxVisible() const
{
    return bIsSelectionBoxVisible;
}

FRect UMingSelectionWidget::GetSelectionRect() const
{
    float MinX = FMath::Min(SelectionStartPosition.X, SelectionEndPosition.X);
    float MaxX = FMath::Max(SelectionStartPosition.X, SelectionEndPosition.X);
    float MinY = FMath::Min(SelectionStartPosition.Y, SelectionEndPosition.Y);
    float MaxY = FMath::Max(SelectionStartPosition.Y, SelectionEndPosition.Y);
    
    return FRect(MinX, MinY, MaxX - MinX, MaxY - MinY);
}

void UMingSelectionWidget::UpdateSelectionBoxVisuals()
{
    if (!SelectionBoxBorder)
    {
        return;
    }
    
    // 計算選擇框的位置和大小
    FVector2D ViewportStart = GetViewportPosition(SelectionStartPosition);
    FVector2D ViewportEnd = GetViewportPosition(SelectionEndPosition);
    
    float MinX = FMath::Min(ViewportStart.X, ViewportEnd.X);
    float MaxX = FMath::Max(ViewportStart.X, ViewportEnd.X);
    float MinY = FMath::Min(ViewportStart.Y, ViewportEnd.Y);
    float MaxY = FMath::Max(ViewportStart.Y, ViewportEnd.Y);
    
    float Width = MaxX - MinX;
    float Height = MaxY - MinY;
    
    // 設置選擇框的位置和大小
    if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(SelectionBoxBorder->Slot))
    {
        CanvasSlot->SetPosition(FVector2D(MinX, MinY));
        CanvasSlot->SetSize(FVector2D(Width, Height));
    }
    
    // 設置邊框樣式
    FLinearColor BorderColor = SelectionBoxColor;
    BorderColor.A = SelectionBoxOpacity;
    
    FSlateBrush BorderBrush;
    BorderBrush.TintColor = BorderColor;
    SelectionBoxBorder->SetBrush(BorderBrush);
    
    // 設置邊框顏色
    SelectionBoxBorder->SetBrushColor(BorderColor);
    
    // 顯示選擇框
    SelectionBoxBorder->SetVisibility(ESlateVisibility::Visible);
}

FVector2D UMingSelectionWidget::GetViewportPosition(FVector2D ScreenPosition) const
{
    // 將屏幕坐標轉換為視口坐標
    FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);
    
    // 簡單的1:1映射（如果需要縮放可以在這裡處理）
    return ScreenPosition;
}
