#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSelectionManager.generated.h"

class AMingTacticalUnit;

USTRUCT(BlueprintType)
struct FSelectionBox
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FVector2D StartScreenPosition;

    UPROPERTY(BlueprintReadWrite)
    FVector2D CurrentScreenPosition;

    UPROPERTY(BlueprintReadWrite)
    bool bIsActive;

    FSelectionBox()
        : StartScreenPosition(FVector2D::ZeroVector)
        , CurrentScreenPosition(FVector2D::ZeroVector)
        , bIsActive(false)
    {}
};

UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGTACTICAL_API UMingSelectionManager : public UObject
{
    GENERATED_BODY()

public:
    UMingSelectionManager(};

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void Initialize(};

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void Shutdown(};

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void SelectUnit(AMingTacticalUnit* Unit, bool bAdditive};

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void SelectUnits(const TArray<AMingTacticalUnit*>& Units, bool bAdditive};

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void DeselectUnit(AMingTacticalUnit* Unit};

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void DeselectAll(};

    UFUNCTION(BlueprintPure, Category = "Selection")
    bool IsUnitSelected(AMingTacticalUnit* Unit) const;

    UFUNCTION(BlueprintPure, Category = "Selection")
    const TArray<AMingTacticalUnit*>& GetSelectedUnits() const { return SelectedUnits; }

    UFUNCTION(BlueprintPure, Category = "Selection")
    int32 GetSelectedUnitCount() const { return SelectedUnits.Num(}; }

    UFUNCTION(BlueprintCallable, Category = "Selection Box")
    void StartSelectionBox(const FVector2D& ScreenPosition};

    UFUNCTION(BlueprintCallable, Category = "Selection Box")
    void UpdateSelectionBox(const FVector2D& ScreenPosition};

    UFUNCTION(BlueprintCallable, Category = "Selection Box")
    void EndSelectionBox(};

    UFUNCTION(BlueprintPure, Category = "Selection Box")
    bool IsSelectionBoxActive() const { return SelectionBox.bIsActive; }

    UFUNCTION(BlueprintPure, Category = "Selection Box")
    FSelectionBox GetSelectionBox() const { return SelectionBox; }

    UFUNCTION(BlueprintPure, Category = "Selection")
    static UMingSelectionManager* Get(};

private:
    static UMingSelectionManager* Instance;

    UPROPERTY()
    TArray<AMingTacticalUnit*> SelectedUnits;

    UPROPERTY()
    FSelectionBox SelectionBox;

    UPROPERTY()
    bool bInitialized;

    void PublishSelectionChangedEvent(};
    bool IsUnitInSelectionBox(AMingTacticalUnit* Unit) const;
    FVector2D GetUnitScreenPosition(AMingTacticalUnit* Unit) const;
};

