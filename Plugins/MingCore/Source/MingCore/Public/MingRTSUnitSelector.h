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
    Single,         // 單選
    Multiple,       // 多選
    Box,            // 框選
    Group           // 分組選擇
};

UENUM(BlueprintType)
enum class ERTSSelectionFilter : uint8
{
    All,            // 所有單位
    SameType,       // 相同類型
    SameState,      // 相同狀態
    Damaged,        // 受損單位
    Idle            // 空閒單位
};

USTRUCT(BlueprintType)
struct FRTSSelectionData
{
    GENERATED_BODY()

    // 注意：AMingGoRTSUnit 是主專案類型，不適合 UPROPERTY
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
 * RTS單位選擇器
 * 處理單位選擇、框選、過濾和分組功能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSUnitSelector : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSUnitSelector();

    // 初始化
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void InitializeSelector(AMingGoRTSPlayerController* InPlayerController);

    // 基礎選擇功能
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

    // 選擇查詢
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<AMingGoRTSUnit*> GetSelectedUnits() const { return CurrentSelection.SelectedUnits; }

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    int32 GetSelectedUnitCount() const { return CurrentSelection.SelectedUnits.Num(); }

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    bool IsUnitSelected(AMingGoRTSUnit* Unit) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    AMingGoRTSUnit* GetPrimarySelection() const;

    // 選擇過濾
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<AMingGoRTSUnit*> FilterSelection(ERTSSelectionFilter FilterType) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void ApplySelectionFilter(ERTSSelectionFilter FilterType);

    // 框選功能
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    bool IsUnitInSelectionBox(AMingGoRTSUnit* Unit, FVector2D BoxStart, FVector2D BoxEnd) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<AMingGoRTSUnit*> GetUnitsInBox(FVector2D BoxStart, FVector2D BoxEnd) const;

    // 分組功能
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void CreateUnitGroup(const FString& GroupName, const TArray<AMingGoRTSUnit*>& Units);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void SelectUnitGroup(const FString& GroupName, bool bAddToSelection = false);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<AMingGoRTSUnit*> GetUnitGroup(const FString& GroupName) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<FString> GetAllUnitGroups() const;

    // 選擇歷史
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void SaveSelectionToHistory();

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void RestoreSelectionFromHistory(int32 HistoryIndex = -1);

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    TArray<FRTSSelectionData> GetSelectionHistory() const { return SelectionHistory; }

    // 選擇統計
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    FVector GetSelectionCenter() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    float GetSelectionRadius() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    FDateTime GetSelectionTime() const { return CurrentSelection.SelectionTime; }

    // 視覺效果
    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void ShowSelectionEffects();

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void HideSelectionEffects();

    UFUNCTION(BlueprintCallable, Category = "RTS Unit Selector")
    void UpdateSelectionVisuals();

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "RTS Unit Selector")
    FOnUnitsSelected OnUnitsSelected;

    UPROPERTY(BlueprintAssignable, Category = "RTS Unit Selector")
    FOnSelectionCleared OnSelectionCleared;

protected:
    // 內部方法
    void UpdateCurrentSelection(const TArray<AMingGoRTSUnit*>& Units, ERTSSelectionType SelectionType);
    void ClearSelectionInternal();
    bool ValidateSelection(const TArray<AMingGoRTSUnit*>& Units) const;
    void NotifySelectionChanged();

    // 輔助方法
    FVector2D WorldToScreen(const FVector& WorldLocation) const;
    FVector ScreenToWorld(const FVector2D& ScreenLocation, float Z = 0.0f) const;
    bool IsPointInBox(FVector2D Point, FVector2D BoxStart, FVector2D BoxEnd) const;

protected:
    // 核心數據
    UPROPERTY()
    TObjectPtr<AMingGoRTSPlayerController> PlayerController;

    UPROPERTY()
    TObjectPtr<UMingRTSUnitManager> UnitManager;

    // 當前選擇
    UPROPERTY()
    FRTSSelectionData CurrentSelection;

    // 選擇歷史
    UPROPERTY()
    TArray<FRTSSelectionData> SelectionHistory;

    // 單位分組
    // 注意：AMingGoRTSUnit 是主專案類型，不適合 UPROPERTY
    TMap<FString, TArray<AMingGoRTSUnit*>> UnitGroups;

    // 配置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Config")
    float MaxSelectionRadius = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Config")
    int32 MaxSelectionHistory = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Config")
    bool bShowSelectionEffects = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Config")
    bool bAutoSaveSelectionHistory = true;

    // 狀態
    UPROPERTY(BlueprintReadOnly, Category = "Selection State")
    bool bIsSelecting = false;

    UPROPERTY(BlueprintReadOnly, Category = "Selection State")
    FVector2D SelectionStartPoint;

    UPROPERTY(BlueprintReadOnly, Category = "Selection State")
    FVector2D SelectionEndPoint;
};
