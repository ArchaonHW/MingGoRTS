#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSUnitSelector.generated.h"

class AMingGoRTSUnit;
class UMingRTSUnitManager;
class AMingGoRTSPlayerController;

UENUM(BlueprintType)
enum class ERTSSelectionType: uint8 {
    Single,         // ??��
    Multiple,       // �h��
    Box,            // �ؿ�
    Group           // 目標數量
};

UENUM(BlueprintType)
enum class ERTSSelectionFilter: uint8 {
    All,            // 摧毀��??
    SameType,       // 摧毀��??
    SameState,      // ???X?X
    Damaged,        // 目標數量
    Idle            // ��?X???
};

USTRUCT(BlueprintType)
struct FRTSSelectionData
{
    GENERATED_BODY()

    
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




/**
 * RTS故事重要性 * 故事選項X??X?�B??�o?X??X?��
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSUnitSelector : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSUnitSelector(); 
    void InitializeSelector(AMingGoRTSPlayerController* InPlayerController);

    // 故事重要性?��
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

    // ???X?��
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<AMingGoRTSUnit*> GetSelectedUnits() const { return CurrentSelection.SelectedUnits; }

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    int32 GetSelectedUnitCount() const { return CurrentSelection.SelectedUnits.Num(); }

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    bool IsUnitSelected(AMingGoRTSUnit* Unit) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    AMingGoRTSUnit* GetPrimarySelection() const;

    // ???X?�o
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<AMingGoRTSUnit*> FilterSelection(ERTSSelectionFilter FilterType) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void ApplySelectionFilter(ERTSSelectionFilter FilterType);

    // �ؿ�??��
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    bool IsUnitInSelectionBox(AMingGoRTSUnit* Unit, FVector2D BoxStart, FVector2D BoxEnd) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<AMingGoRTSUnit*> GetUnitsInBox(FVector2D BoxStart, FVector2D BoxEnd) const;

    // ???X?��
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void CreateUnitGroup(const FString& GroupName, const TArray<AMingGoRTSUnit*>& Units);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void SelectUnitGroup(const FString& GroupName, bool bAddToSelection = false);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<AMingGoRTSUnit*> GetUnitGroup(const FString& GroupName) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<FString> GetAllUnitGroups() const;

    // 摧毀���v
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void SaveSelectionToHistory();

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void RestoreSelectionFromHistory(int32 HistoryIndex = -1);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<FRTSSelectionData> GetSelectionHistory() const { return SelectionHistory; }

    // 摧毀��??
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    FVector GetSelectionCenter() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    float GetSelectionRadius() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    FDateTime GetSelectionTime() const { return CurrentSelection.SelectionTime; }

    // ��ı摧毀
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void ShowSelectionEffects();

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void HideSelectionEffects();

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void UpdateSelectionVisuals(); // �ƥ�e??
    UPROPERTY(BlueprintAssignable, Category = "RTS Unit Selector")
    FOnUnitsSelected OnUnitsSelected;

    UPROPERTY(BlueprintAssignable, Category = "RTS Unit Selector")
    FOnSelectionCleared OnSelectionCleared;

protected:
    // ??��摧毀
    void UpdateCurrentSelection(const TArray<AMingGoRTSUnit*>& Units, ERTSSelectionType SelectionType);
    void ClearSelectionInternal();
    bool ValidateSelection(const TArray<AMingGoRTSUnit*>& Units) const;
    void NotifySelectionChanged(); // ���U摧毀
    FVector2D WorldToScreen(const FVector& WorldLocation) const;
    FVector ScreenToWorld(const FVector2D& ScreenLocation, float Z = 0.0f) const;
    bool IsPointInBox(FVector2D Point, FVector2D BoxStart, FVector2D BoxEnd) const;

protected:
    // 目標數量
    UPROPERTY()
    TObjectPtr<AMingGoRTSPlayerController> PlayerController;

    UPROPERTY()
    TObjectPtr<UMingRTSUnitManager> UnitManager;

    // 目標數量
    UPROPERTY()
    FRTSSelectionData CurrentSelection;

    // 摧毀���v
    UPROPERTY()
    TArray<FRTSSelectionData> SelectionHistory;

    // 目標數量
    
    TMap<FString, TArray<AMingGoRTSUnit*>> UnitGroups;

    // ??�m
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Config")
    float MaxSelectionRadius = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Config")
    int32 MaxSelectionHistory = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Config")
    bool bShowSelectionEffects = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Config")
    bool bAutoSaveSelectionHistory = true;

    
    bool bIsSelecting = false;

    UPROPERTY(BlueprintReadOnly, Category = "Selection State")
    FVector2D SelectionStartPoint;

    UPROPERTY(BlueprintReadOnly, Category = "Selection State")
    FVector2D SelectionEndPoint;
};

