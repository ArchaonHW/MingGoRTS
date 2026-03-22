#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSelectionManager.generated.h"

class AMingTacticalUnit;

/**
 * 選擇框結構
 */
USTRUCT(BlueprintType)
struct FSelectionBox
{
    GENERATED_BODY()
    
    // 起始位置
    UPROPERTY(BlueprintReadOnly)
    FVector2D StartScreenPosition;
    
    // 當前位置
    UPROPERTY(BlueprintReadOnly)
    FVector2D CurrentScreenPosition;
    
    // 是否在繪製中
    UPROPERTY(BlueprintReadOnly)
    bool bIsDrawing;
    
    FSelectionBox()
        : StartScreenPosition(FVector2D::ZeroVector)
        , CurrentScreenPosition(FVector2D::ZeroVector)
        , bIsDrawing(false)
    {}
};

/**
 * 選擇管理器
 * 管理所有單位的選擇狀態
 */
UCLASS()
class MINGTACTICAL_API UMingSelectionManager : public UObject
{
    GENERATED_BODY()

public:
    UMingSelectionManager();

    // 初始化
    UFUNCTION(BlueprintCallable, Category = "Selection")
    void Initialize();

    // 關閉
    UFUNCTION(BlueprintCallable, Category = "Selection")
    void Shutdown();

    // 單擊選擇單位
    UFUNCTION(BlueprintCallable, Category = "Selection")
    void SelectUnit(AMingTacticalUnit* Unit, bool bAdditive = false);

    // 選擇多個單位
    UFUNCTION(BlueprintCallable, Category = "Selection")
    void SelectUnits(const TArray<AMingTacticalUnit*>& Units, bool bAdditive = false);

    // 取消選擇單位
    UFUNCTION(BlueprintCallable, Category = "Selection")
    void DeselectUnit(AMingTacticalUnit* Unit);

    // 取消所有選擇
    UFUNCTION(BlueprintCallable, Category = "Selection")
    void DeselectAll();

    // 開始框選
    UFUNCTION(BlueprintCallable, Category = "Selection")
    void StartBoxSelection(const FVector2D& StartPosition);

    // 更新框選
    UFUNCTION(BlueprintCallable, Category = "Selection")
    void UpdateBoxSelection(const FVector2D& CurrentPosition);

    // 結束框選
    UFUNCTION(BlueprintCallable, Category = "Selection")
    void EndBoxSelection();

    // 獲取當前選中的單位
    UFUNCTION(BlueprintPure, Category = "Selection")
    const TArray<AMingTacticalUnit*>& GetSelectedUnits() const { return SelectedUnits; }

    // 獲取選中的單位數量
    UFUNCTION(BlueprintPure, Category = "Selection")
    int32 GetSelectedCount() const { return SelectedUnits.Num(); }

    // 是否有選中的單位
    UFUNCTION(BlueprintPure, Category = "Selection")
    bool HasSelection() const { return SelectedUnits.Num() > 0; }

    // 獲取選擇框
    UFUNCTION(BlueprintPure, Category = "Selection")
    FSelectionBox GetSelectionBox() const { return SelectionBox; }

    // 是否在進行框選
    UFUNCTION(BlueprintPure, Category = "Selection")
    bool IsBoxSelecting() const { return SelectionBox.bIsDrawing; }

    // 靜態獲取實例
    static UMingSelectionManager* Get();

private:
    // 當前選中的單位
    UPROPERTY()
    TArray<TObjectPtr<AMingTacticalUnit>> SelectedUnits;

    // 選擇框
    FSelectionBox SelectionBox;

    // 是否已初始化
    bool bInitialized;

    // 單例實例
    static UMingSelectionManager* Instance;

    // 更新選擇框選中的單位
    void UpdateBoxSelectedUnits();

    // 檢查單位是否在選擇框內
    bool IsUnitInSelectionBox(AMingTacticalUnit* Unit) const;

    // 發布選擇變更事件
    void PublishSelectionChangedEvent();

    // 獲取單位在屏幕上的位置
    FVector2D GetUnitScreenPosition(AMingTacticalUnit* Unit) const;
};
