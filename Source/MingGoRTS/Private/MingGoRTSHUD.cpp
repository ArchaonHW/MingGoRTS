#include "MingGoRTSHUD.h"
#include "MingGoRTSUnit.h"
#include "Kismet/GameplayStatics.h"

AMingGoRTSHUD::AMingGoRTSHUD()
{
    bIsSelecting = false;
    SelectionStart = FVector2D::ZeroVector;
    SelectionEnd = FVector2D::ZeroVector;
}

void AMingGoRTSHUD::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("MingGoRTS HUD Started"));
}

void AMingGoRTSHUD::DrawHUD()
{
    Super::DrawHUD();

    // 繪製選擇框
    if (bIsSelecting)
    {
        DrawSelectionBox(SelectionStart, SelectionEnd);
    }
}

void AMingGoRTSHUD::DrawSelectionBox(FVector2D Start, FVector2D End)
{
    // 確保座標在正確的順序
    FVector2D Min = FVector2D(FMath::Min(Start.X, End.X), FMath::Min(Start.Y, End.Y));
    FVector2D Max = FVector2D(FMath::Max(Start.X, End.X), FMath::Max(Start.Y, End.Y));

    float Width = Max.X - Min.X;
    float Height = Max.Y - Min.Y;
    
    // 定義顏色
    FLinearColor FillColor(0.0f, 0.7f, 1.0f, 0.15f);      // 半透明藍色填充
    FLinearColor BorderColor(0.0f, 0.9f, 1.0f, 1.0f);      // 亮藍色邊框
    FLinearColor CornerColor(1.0f, 1.0f, 0.0f, 1.0f);     // 黃色角落標記
    
    // 繪製填充區域
    DrawRect(FillColor, Min.X, Min.Y, Width, Height);
    
    // 繪製邊框（2像素粗細）
    DrawRect(BorderColor, Min.X, Min.Y, Width, 2.0f);           // 上邊
    DrawRect(BorderColor, Min.X, Min.Y, 2.0f, Height);          // 左邊
    DrawRect(BorderColor, Max.X - 2.0f, Min.Y, 2.0f, Height);   // 右邊
    DrawRect(BorderColor, Min.X, Max.Y - 2.0f, Width, 2.0f);    // 下邊
    
    // 繪製角落標記（8x8 像素的方塊）
    const float CornerSize = 8.0f;
    DrawRect(CornerColor, Min.X - CornerSize/2, Min.Y - CornerSize/2, CornerSize, CornerSize); // 左上
    DrawRect(CornerColor, Max.X - CornerSize/2, Min.Y - CornerSize/2, CornerSize, CornerSize); // 右上
    DrawRect(CornerColor, Min.X - CornerSize/2, Max.Y - CornerSize/2, CornerSize, CornerSize); // 左下
    DrawRect(CornerColor, Max.X - CornerSize/2, Max.Y - CornerSize/2, CornerSize, CornerSize); // 右下
}

void AMingGoRTSHUD::ClearSelectionBox()
{
    bIsSelecting = false;
    SelectionStart = FVector2D::ZeroVector;
    SelectionEnd = FVector2D::ZeroVector;
}
