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

    // 繪製選擇框
    DrawRect(FLinearColor(0.0f, 0.5f, 1.0f, 0.2f), Min.X, Min.Y, Max.X - Min.X, Max.Y - Min.Y);
    DrawRect(FLinearColor(0.0f, 0.5f, 1.0f, 1.0f), Min.X, Min.Y, Max.X - Min.X, 2.0f);
    DrawRect(FLinearColor(0.0f, 0.5f, 1.0f, 1.0f), Min.X, Min.Y, 2.0f, Max.Y - Min.Y);
    DrawRect(FLinearColor(0.0f, 0.5f, 1.0f, 1.0f), Max.X - 2.0f, Min.Y, 2.0f, Max.Y - Min.Y);
    DrawRect(FLinearColor(0.0f, 0.5f, 1.0f, 1.0f), Min.X, Max.Y - 2.0f, Max.X - Min.X, 2.0f);
}

void AMingGoRTSHUD::ClearSelectionBox()
{
    bIsSelecting = false;
    SelectionStart = FVector2D::ZeroVector;
    SelectionEnd = FVector2D::ZeroVector;
}
