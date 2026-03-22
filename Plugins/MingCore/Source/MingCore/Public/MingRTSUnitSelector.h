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
    Single,         // ?ÆÈÅ∏
    Multiple,       // Â§öÈÅ∏
    Box,            // Ê°ÜÈÅ∏
    Group           // ?ÜÁ??∏Ê?
};

UENUM(BlueprintType)
enum class ERTSSelectionFilter : uint8
{
    All,            // ?Ä?âÂñÆ‰Ω?    SameType,       // ?∏Â?È°ûÂ?
    SameState,      // ?∏Â??Ä??    Damaged,        // ?óÊ??Æ‰?
    Idle            // Á©∫È??Æ‰?
};

USTRUCT(BlueprintType)
struct FRTSSelectionData
{
    GENERATED_BODY()

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??©Â? UPROPERTY
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitsSelected, const TArray<AMingGoRTSUnit*>&, Units, ERTSSelectionType, SelectionType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionCleared, ERTSSelectionType, PreviousSelectionType);

/**
 * RTS?Æ‰??∏Ê??? * ?ïÁ??Æ‰??∏Ê??ÅÊ??∏„ÄÅÈ?ÊøæÂ??ÜÁ??üËÉΩ
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSUnitSelector : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSUnitSelector();

    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void InitializeSelector(AMingGoRTSPlayerController* InPlayerController);

    // ?∫Á??∏Ê??üËÉΩ
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void SelectUnit(AMingGoRTSUnit* Unit, bool bAddToSelection = false);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void SelectUnits(const TArray<AMingGoRTSUnit*>& Units, bool bAddToSelection = false);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void SelectUnitsInBox(FVector2D ScreenStart, FVector2D ScreenEnd, bool bAddToSelection = false);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void DeselectAllUnits();

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void DeselectUnit(AMingGoRTSUnit* Unit);

    // ?∏Ê??•Ë©¢
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<AMingGoRTSUnit*> GetSelectedUnits() const { return CurrentSelection.SelectedUnits; }

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    int32 GetSelectedUnitCount() const { return CurrentSelection.SelectedUnits.Num(); }

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    bool IsUnitSelected(AMingGoRTSUnit* Unit) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    AMingGoRTSUnit* GetPrimarySelection() const;

    // ?∏Ê??éÊøæ
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<AMingGoRTSUnit*> FilterSelection(ERTSSelectionFilter FilterType) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void ApplySelectionFilter(ERTSSelectionFilter FilterType);

    // Ê°ÜÈÅ∏?üËÉΩ
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    bool IsUnitInSelectionBox(AMingGoRTSUnit* Unit, FVector2D BoxStart, FVector2D BoxEnd) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<AMingGoRTSUnit*> GetUnitsInBox(FVector2D BoxStart, FVector2D BoxEnd) const;

    // ?ÜÁ??üËÉΩ
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void CreateUnitGroup(const FString& GroupName, const TArray<AMingGoRTSUnit*>& Units);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void SelectUnitGroup(const FString& GroupName, bool bAddToSelection = false);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<AMingGoRTSUnit*> GetUnitGroup(const FString& GroupName) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<FString> GetAllUnitGroups() const;

    // ?∏Ê?Ê≠∑Âè≤
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void SaveSelectionToHistory();

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void RestoreSelectionFromHistory(int32 HistoryIndex = -1);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<FRTSSelectionData> GetSelectionHistory() const { return SelectionHistory; }

    // ?∏Ê?Áµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    FVector GetSelectionCenter() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    float GetSelectionRadius() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    FDateTime GetSelectionTime() const { return CurrentSelection.SelectionTime; }

    // Ë¶ñË¶∫?àÊ?
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void ShowSelectionEffects();

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void HideSelectionEffects();

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void UpdateSelectionVisuals();

    // ‰∫ã‰ª∂ÂßîË?
    UPROPERTY(BlueprintAssignable, Category = "RTS Unit Selector")
    FOnUnitsSelected OnUnitsSelected;

    UPROPERTY(BlueprintAssignable, Category = "RTS Unit Selector")
    FOnSelectionCleared OnSelectionCleared;

protected:
    // ?ßÈÉ®?πÊ?
    void UpdateCurrentSelection(const TArray<AMingGoRTSUnit*>& Units, ERTSSelectionType SelectionType);
    void ClearSelectionInternal();
    bool ValidateSelection(const TArray<AMingGoRTSUnit*>& Units) const;
    void NotifySelectionChanged();

    // ËºîÂä©?πÊ?
    FVector2D WorldToScreen(const FVector& WorldLocation) const;
    FVector ScreenToWorld(const FVector2D& ScreenLocation, float Z = 0.0f) const;
    bool IsPointInBox(FVector2D Point, FVector2D BoxStart, FVector2D BoxEnd) const;

protected:
    // ?∏Â??∏Ê?
    UPROPERTY()
    TObjectPtr<AMingGoRTSPlayerController> PlayerController;

    UPROPERTY()
    TObjectPtr<UMingRTSUnitManager> UnitManager;

    // ?∂Â??∏Ê?
    UPROPERTY()
    FRTSSelectionData CurrentSelection;

    // ?∏Ê?Ê≠∑Âè≤
    UPROPERTY()
    TArray<FRTSSelectionData> SelectionHistory;

    // ?Æ‰??ÜÁ?
    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??©Â? UPROPERTY
    TMap<FString, TArray<AMingGoRTSUnit*>> UnitGroups;

    // ?çÁΩÆ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Config")
    float MaxSelectionRadius = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Config")
    int32 MaxSelectionHistory = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Config")
    bool bShowSelectionEffects = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Config")
    bool bAutoSaveSelectionHistory = true;

    // ?Ä??    UPROPERTY(BlueprintReadOnly, Category = "Selection State")
    bool bIsSelecting = false;

    UPROPERTY(BlueprintReadOnly, Category = "Selection State")
    FVector2D SelectionStartPoint;

    UPROPERTY(BlueprintReadOnly, Category = "Selection State")
    FVector2D SelectionEndPoint;
};
