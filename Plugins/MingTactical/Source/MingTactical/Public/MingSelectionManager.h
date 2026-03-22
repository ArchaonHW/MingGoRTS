#pragma once

#include "CoreMinimal.h"
#include "MingCore/Source/MingCore/Public/MingCoreEventBus.h"
#include "MingSelectionManager.generated.h"

USTRUCT(BlueprintType)
struct MINGTACTICAL_API FSelectionBox
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FVector2D StartPosition;

    UPROPERTY(BlueprintReadOnly)
    FVector2D CurrentPosition;

    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;

    FSelectionBox() : bIsActive(false) {}

    FRect GetScreenRect() const
    {
        float MinX = FMath::Min(StartPosition.X, CurrentPosition.X);
        float MaxX = FMath::Max(StartPosition.X, CurrentPosition.X);
        float MinY = FMath::Min(StartPosition.Y, CurrentPosition.Y);
        float MaxY = FMath::Max(StartPosition.Y, CurrentPosition.Y);

        return FRect(MinX, MinY, MaxX - MinX, MaxY - MinY);
    }

    bool ContainsPoint(const FVector2D& Point) const
    {
        FRect Rect = GetScreenRect();
        return Rect.Contains(Point);
    }
};

USTRUCT(BlueprintType)
struct MINGTACTICAL_API FUnitSelectionState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 UnitId;

    UPROPERTY(BlueprintReadOnly)
    bool bIsSelected;

    UPROPERTY(BlueprintReadOnly)
    float SelectionTime;

    FUnitSelectionState() : UnitId(-1), bIsSelected(false), SelectionTime(0.0f) {}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGTACTICAL_API UMingSelectionManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingSelectionManager();

    UPROPERTY(BlueprintReadOnly, Category = "Selection")
    TArray<int32> SelectedUnitIds;

    UPROPERTY(BlueprintReadOnly, Category = "Selection")
    FSelectionBox CurrentSelectionBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
    float SelectionBoxThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
    bool bEnableDebugVisualization;

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void StartSelectionBox(const FVector2D& ScreenPosition);

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void UpdateSelectionBox(const FVector2D& ScreenPosition);

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void EndSelectionBox();

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void SelectSingleUnit(int32 UnitId, bool bAddToSelection);

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void DeselectUnit(int32 UnitId);

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void DeselectAllUnits();

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void InvertSelection();

    UFUNCTION(BlueprintCallable, Category = "Selection")
    bool IsUnitSelected(int32 UnitId) const;

    UFUNCTION(BlueprintCallable, Category = "Selection")
    int32 GetSelectedUnitCount() const;

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void SelectAllUnitsOnScreen();

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void SelectUnitsByType(FString UnitType);

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitSelected, int32, UnitId);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitDeselected, int32, UnitId);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectionCleared);

    UPROPERTY(BlueprintAssignable, Category = "Selection|Events")
    FOnUnitSelected OnUnitSelected;

    UPROPERTY(BlueprintAssignable, Category = "Selection|Events")
    FOnUnitDeselected OnUnitDeselected;

    UPROPERTY(BlueprintAssignable, Category = "Selection|Events")
    FOnSelectionCleared OnSelectionCleared;

private:
    void UpdateSelectedUnits();
    void PerformBoxSelection();
    void PerformSingleSelection(const FVector2D& ScreenPosition);
    TArray<class AMingTacticalUnit*> GetUnitsInScreenRect(const FRect& ScreenRect);
    TArray<class AMingTacticalUnit*> GetUnitUnderCursor(const FVector2D& ScreenPosition);
    void PublishSelectionEvents(const TArray<int32>& NewlySelected, const TArray<int32>& NewlyDeselected);
    void VisualizeSelectionBox();
    bool IsSelectionBoxValid() const;

    UPROPERTY()
    TWeakObjectPtr<class APlayerController> OwningController;

    UPROPERTY()
    TWeakObjectPtr<class AMingRTSPlayerController> RTSController;

    bool bWasCtrlPressed;
    bool bWasShiftPressed;
    FVector2D SelectionStartPosition;
    double SelectionStartTime;

    static constexpr float SELECTION_DELAY_THRESHOLD = 0.15f;
    static constexpr float SELECTION_DRAG_THRESHOLD = 5.0f;
};
