#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MingGoRTSHUD.generated.h"

UCLASS()
class MINGGORTS_API AMingGoRTSHUD : public AHUD
{
    GENERATED_BODY()

public:
    AMingGoRTSHUD();

protected:
    virtual void BeginPlay() override;
    virtual void DrawHUD() override;

public:
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void DrawSelectionBox(FVector2D Start, FVector2D End);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ClearSelectionBox();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    bool IsSelecting() const { return bIsSelecting; }

private:
    UPROPERTY()
    bool bIsSelecting;

    UPROPERTY()
    FVector2D SelectionStart;

    UPROPERTY()
    FVector2D SelectionEnd;

    UPROPERTY()
    TArray<class AMingGoRTSUnit*> SelectedUnits;
};

