#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSUnitSelector.generated.h"

class AMingGoRTSUnit;
class UMingRTSUnitManager;
class AMingGoRTSPlayerController;

UENUM(BlueprintType)
enum class ERTSSelectionType : uint8
{
    Single,         // ?�選
    Multiple,       // 多選
    Box,            // 框選
    Group           // ?��X��?
};

UENUM(BlueprintType)
enum class ERTSSelectionFilter : uint8
{
    All,            // ?�?�單�?    SameType,       // ?��?類�?
    SameState,      // ?��X�X    Damaged,        // ?��X��?
    Idle            // 空�X��?
};

USTRUCT(BlueprintType)
struct FRTSSelectionData
{
    GENERATED_BODY()

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X��? UPROPERTY
    TArray<AMingGoRTSUnit*> SelectedUnits;

    UPROPERTY(BlueprintReadOnly, Category = "Selection Data")
    ERTSSelectionType SelectionType;

    UPROPERTY(BlueprintReadOnly, Category = "Selection Data")
    FVector SelectionCenter;

    UPROPERTY(BlueprintReadOnly, Category = "Selection Data")
    float SelectionRadius;

    UPROPERTY(BlueprintReadOnly, Category = "Selection Data")
    FDateTime SelectionTime;

    FRTSSelectionData()
    {
        SelectionType = ERTSSelectionType::Single;
        SelectionCenter = FVector::ZeroVector;
        SelectionRadius = 0.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitsSelected, const TArray<AMingGoRTSUnit*>&, Units, ERTSSelectionType, SelectionType};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionCleared, ERTSSelectionType, PreviousSelectionType};

/**
 * RTS?��X��X * ?��X��X��X��X�、�?濾�X��X�能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSUnitSelector : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSUnitSelector(); // ?��X    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void InitializeSelector(AMingGoRTSPlayerController* InPlayerController};

    // ?��X��X�能
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void SelectUnit(AMingGoRTSUnit* Unit, bool bAddToSelection = false};

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void SelectUnits(const TArray<AMingGoRTSUnit*>& Units, bool bAddToSelection = false};

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void SelectUnitsInBox(FVector2D ScreenStart, FVector2D ScreenEnd, bool bAddToSelection = false};

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void DeselectAllUnits(};

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void DeselectUnit(AMingGoRTSUnit* Unit};

    // ?��X�詢
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<AMingGoRTSUnit*> GetSelectedUnits() const { return CurrentSelection.SelectedUnits; }

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    int32 GetSelectedUnitCount() const { return CurrentSelection.SelectedUnits.Num(); }

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    bool IsUnitSelected(AMingGoRTSUnit* Unit) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    AMingGoRTSUnit* GetPrimarySelection() const;

    // ?��X�濾
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<AMingGoRTSUnit*> FilterSelection(ERTSSelectionFilter FilterType) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void ApplySelectionFilter(ERTSSelectionFilter FilterType};

    // 框選?�能
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    bool IsUnitInSelectionBox(AMingGoRTSUnit* Unit, FVector2D BoxStart, FVector2D BoxEnd) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<AMingGoRTSUnit*> GetUnitsInBox(FVector2D BoxStart, FVector2D BoxEnd) const;

    // ?��X�能
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void CreateUnitGroup(const FString& GroupName, const TArray<AMingGoRTSUnit*>& Units};

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void SelectUnitGroup(const FString& GroupName, bool bAddToSelection = false};

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<AMingGoRTSUnit*> GetUnitGroup(const FString& GroupName) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<FString> GetAllUnitGroups() const;

    // ?��?歷史
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void SaveSelectionToHistory(};

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void RestoreSelectionFromHistory(int32 HistoryIndex = -1};

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<FRTSSelectionData> GetSelectionHistory() const { return SelectionHistory; }

    // ?��?統�?
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    FVector GetSelectionCenter() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    float GetSelectionRadius() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    FDateTime GetSelectionTime() const { return CurrentSelection.SelectionTime; }

    // 視覺?��?
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void ShowSelectionEffects(};

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void HideSelectionEffects(};

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void UpdateSelectionVisuals(); // 事件委�?
    UPROPERTY(BlueprintAssignable, Category = "RTS Unit Selector")
    FOnUnitsSelected OnUnitsSelected;

    UPROPERTY(BlueprintAssignable, Category = "RTS Unit Selector")
    FOnSelectionCleared OnSelectionCleared;

protected:
    // ?�部?��?
    void UpdateCurrentSelection(const TArray<AMingGoRTSUnit*>& Units, ERTSSelectionType SelectionType};
    void ClearSelectionInternal(};
    bool ValidateSelection(const TArray<AMingGoRTSUnit*>& Units) const;
    void NotifySelectionChanged(); // 輔助?��?
    FVector2D WorldToScreen(const FVector& WorldLocation) const;
    FVector ScreenToWorld(const FVector2D& ScreenLocation, float Z = 0.0f) const;
    bool IsPointInBox(FVector2D Point, FVector2D BoxStart, FVector2D BoxEnd) const;

protected:
    // ?��X��?
    UPROPERTY()
    TObjectPtr<AMingGoRTSPlayerController> PlayerController;

    UPROPERTY()
    TObjectPtr<UMingRTSUnitManager> UnitManager;

    // ?��X��?
    UPROPERTY()
    FRTSSelectionData CurrentSelection;

    // ?��?歷史
    UPROPERTY()
    TArray<FRTSSelectionData> SelectionHistory;

    // ?��X��?
    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X��? UPROPERTY
    TMap<FString, TArray<AMingGoRTSUnit*>> UnitGroups;

    // ?�置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Config")
    float MaxSelectionRadius = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Config")
    int32 MaxSelectionHistory = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Config")
    bool bShowSelectionEffects = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Config")
    bool bAutoSaveSelectionHistory = true;

    // ?�X    UPROPERTY(BlueprintReadOnly, Category = "Selection State")
    bool bIsSelecting = false;

    UPROPERTY(BlueprintReadOnly, Category = "Selection State")
    FVector2D SelectionStartPoint;

    UPROPERTY(BlueprintReadOnly, Category = "Selection State")
    FVector2D SelectionEndPoint;
};

