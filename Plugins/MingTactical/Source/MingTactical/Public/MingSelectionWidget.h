#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingSelectionWidget.generated.h"

UCLASS()
class MINGTACTICAL_API UMingSelectionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UPROPERTY(meta = (BindWidget))
    class UCanvasPanel* SelectionCanvas;

    UPROPERTY(meta = (BindWidgetOptional))
    class UBorder* SelectionBoxBorder;

    UPROPERTY(meta = (BindWidgetOptional))
    class UTextBlock* SelectionCountText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
    FLinearColor SelectionBoxColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
    float SelectionBoxBorderThickness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
    float SelectionBoxOpacity;

    UFUNCTION(BlueprintCallable, Category = "Selection Box")
    void ShowSelectionBox(const FVector2D& StartPosition, const FVector2D& CurrentPosition);

    UFUNCTION(BlueprintCallable, Category = "Selection Box")
    void UpdateSelectionBox(const FVector2D& CurrentPosition);

    UFUNCTION(BlueprintCallable, Category = "Selection Box")
    void HideSelectionBox();

    UFUNCTION(BlueprintCallable, Category = "Selection Info")
    void UpdateSelectionCount(int32 SelectedCount, int32 TotalCount);

    UFUNCTION(BlueprintCallable, Category = "Selection Info")
    void ShowSelectionInfo();

    UFUNCTION(BlueprintCallable, Category = "Selection Info")
    void HideSelectionInfo();

    UFUNCTION(BlueprintPure, Category = "Selection Box")
    bool IsSelectionBoxVisible() const;

    UFUNCTION(BlueprintPure, Category = "Selection Box")
    FRect GetSelectionRect() const;

private:
    FVector2D SelectionStartPosition;
    FVector2D SelectionEndPosition;
    bool bIsSelectionBoxVisible;

    void UpdateSelectionBoxVisuals();
    FVector2D GetViewportPosition(FVector2D ScreenPosition) const;
};
