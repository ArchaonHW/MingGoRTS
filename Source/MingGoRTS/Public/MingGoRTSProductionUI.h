#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Widget.h"
#include "MingGoRTSProductionSystem.h"
#include "MingGoRTSProductionUI.generated.h"

/**
 * ?�產線UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionLineWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionLineWidget(};

    // 設置?�產線數X    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void SetProductionLine(const FProductionLine& ProductionLine};

    // ?�新?�產?�X    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void UpdateProductionStatus(EProductionStatus NewStatus};

    // ?�新?�產?�度
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void UpdateProductionProgress(float Progress, float TimeRemaining};

    // 設置?�產?�方
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void SetCurrentRecipe(const FProductionRecipe& Recipe};

    // 設置?�否被選�?    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void SetIsSelected(bool bSelected};

    // ?��X�產線ID
    UFUNCTION(BlueprintPure, Category = "Production Line UI")
    FString GetProductionLineID() const;

    // ?��X�否被選�?    UFUNCTION(BlueprintPure, Category = "Production Line UI")
    bool IsSelected() const;

protected:
    // UI組件引用
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ProductionLineName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CurrentRecipeName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ProductionStatus;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* ProductionProgressBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ProgressText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TimeRemainingText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* EfficiencyText;

    UPROPERTY(meta = (BindWidget))
    class UButton* StartButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* PauseButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* CancelButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* UpgradeButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* AutomateButton;

    UPROPERTY(meta = (BindWidget))
    class UBorder* LineBorder;

    // ?��X�產線數X    UPROPERTY(BlueprintReadOnly, Category = "Production Line UI")
    FProductionLine CurrentProductionLine;

    // ?��X�方
    UPROPERTY(BlueprintReadOnly, Category = "Production Line UI")
    FProductionRecipe CurrentRecipe;

    // ?�否被選�?    UPROPERTY(BlueprintReadOnly, Category = "Production Line UI")
    bool bIsSelected;

    // ?��X�UI
    virtual void NativeConstruct() override;

    // ?�新UI顯示
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Line UI")
    void OnUpdateUI(};

    // ?��X��X��?
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnStartClicked(};

    // ?��X��X��?
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnPauseClicked(};

    // ?��X��X��?
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnCancelClicked(};

    // ?��X��X��?
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnUpgradeClicked(};

    // ?��X��X��X    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnAutomateClicked(};

    // ?��X�?��X    FLinearColor GetStatusColor(EProductionStatus Status) const;

    // ?��X��?級�X    FLinearColor GetPriorityColor(EProductionPriority Priority) const;

    // ?��X��X�顯�?    FString FormatTime(float TimeSeconds) const;

private:
    // 綁�?事件
    void BindEvents(};

    // �X事件
    void UnbindEvents(};
};

/**
 * ?�產?�方UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSRecipeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSRecipeWidget(};

    // 設置?�方?��?
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void SetRecipe(const FProductionRecipe& Recipe};

    // 設置?�否?��X    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void SetCanProduce(bool bCanProduce};

    // 設置?�否被選�?    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void SetIsSelected(bool bSelected};

    // ?��X�方ID
    UFUNCTION(BlueprintPure, Category = "Recipe UI")
    FString GetRecipeID() const;

protected:
    // UI組件引用
    UPROPERTY(meta = (BindWidget))
    class UImage* RecipeIcon;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* RecipeName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* RecipeDescription;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ProductionTime;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Efficiency;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* InputResourcesList;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* OutputResourcesList;

    UPROPERTY(meta = (BindWidget))
    class UButton* ProduceButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QueueButton;

    UPROPERTY(meta = (BindWidget))
    class UBorder* RecipeBorder;

    // ?��X�方
    UPROPERTY(BlueprintReadOnly, Category = "Recipe UI")
    FProductionRecipe CurrentRecipe;

    // ?�否?��X    UPROPERTY(BlueprintReadOnly, Category = "Recipe UI")
    bool bCanProduce;

    // ?�否被選�?    UPROPERTY(BlueprintReadOnly, Category = "Recipe UI")
    bool bIsSelected;

    // ?��X�UI
    virtual void NativeConstruct() override;

    // ?�新UI顯示
    UFUNCTION(BlueprintImplementableEvent, Category = "Recipe UI")
    void OnUpdateUI(};

    // ?��X�產?��?
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void OnProduceClicked(};

    // ?��X�入?��X��?
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void OnQueueClicked(};

    // ?�建資�X�表X    UUserWidget* CreateResourceItem(EResourceType ResourceType, float Amount, bool bIsInput};

    // ?��X��?源顯�?    FString FormatResourceDisplay(EResourceType ResourceType, float Amount) const;

private:
    // 綁�?事件
    void BindEvents(};

    // �X事件
    void UnbindEvents(};
};

/**
 * ?�產?��?UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionQueueWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionQueueWidget(};

    // 設置?�產?��X��?
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void SetProductionQueue(const FProductionQueue& Queue};

    // ?�新?��X�目
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void UpdateQueueItem(int32 Index, const FString& RecipeID};

    // 移除?��X�目
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void RemoveQueueItem(int32 Index};

    // 清空?��?
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void ClearQueue(};

    // 設置循環模�?
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void SetLooping(bool bIsLooping};

    // 設置?��X��?
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void SetAutoStart(bool bAutoStart};

protected:
    // UI組件引用
    UPROPERTY(meta = (BindWidget))
    class UScrollBox* QueueScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* QueueContainer;

    UPROPERTY(meta = (BindWidget))
    class UButton* ClearQueueButton;

    UPROPERTY(meta = (BindWidget))
    class UCheckBox* LoopingCheckBox;

    UPROPERTY(meta = (BindWidget))
    class UCheckBox* AutoStartCheckBox;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* QueueCount;

    // ?��X��?
    UPROPERTY(BlueprintReadOnly, Category = "Production Queue UI")
    FProductionQueue CurrentQueue;

    // ?��X�目Widget?��?
    UPROPERTY(BlueprintReadOnly, Category = "Production Queue UI")
    TArray<class UMingGoRTSRecipeWidget*> QueueItemWidgets;

    // ?��X�UI
    virtual void NativeConstruct() override;

    // ?�建?��X�目Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Queue UI")
    UMingGoRTSRecipeWidget* CreateQueueItemWidget(const FString& RecipeID};

    // ?��?清空?��?
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void OnClearQueueClicked(};

    // ?��?循環模�?變更
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void OnLoopingChanged(bool bIsLooping};

    // ?��X��X��?變更
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void OnAutoStartChanged(bool bAutoStart};

    // ?�新?��?計數
    void UpdateQueueCount(};

private:
    // 綁�?事件
    void BindEvents(};

    // �X事件
    void UnbindEvents(};
};

/**
 * ?�產管�?主UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionManagerWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionManagerWidget(};

    // 設置?�產系統引用
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void SetProductionSystem(UMingGoRTSProductionSystem* ProductionSystem};

    // 顯示?��?類別?��X    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ShowRecipeCategory(EProductionCategory Category};

    // 顯示?�?��X    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ShowAllCategories(};

    // ?�中?�產�?    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void SelectProductionLine(const FString& ProductionLineID};

    // 清除?�中?�X    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ClearSelection(};

    // ?�建?��X��?
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void CreateNewProductionLine(};

    // 顯示?�產統�?
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ShowProductionStatistics(};

    // 設置?�濾X    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void SetFilter(EProductionCategory CategoryFilter, EProductionPriority PriorityFilter};

    // 清除?�濾X    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ClearFilter(};

    // ?��X��X�中?��X��?
    UFUNCTION(BlueprintPure, Category = "Production Manager UI")
    FString GetSelectedProductionLineID() const;

    // ?��X��?顯示?��X    UFUNCTION(BlueprintPure, Category = "Production Manager UI")
    EProductionCategory GetCurrentCategory() const;

protected:
    // UI組件引用
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* ProductionLinesContainer;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* RecipesScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* CategoryTabs;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* PriorityFilters;

    UPROPERTY(meta = (BindWidget))
    class UButton* CreateLineButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* StatisticsButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SelectedLineName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TotalEfficiency;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ActiveProductionCount;

    // ?�產系統引用
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    UMingGoRTSProductionSystem* ProductionSystem;

    // ?��X�中?��X��?ID
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    FString SelectedProductionLineID;

    // ?��?顯示?��X    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    EProductionCategory CurrentCategory;

    // ?��X�濾X    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    EProductionCategory CategoryFilter;

    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    EProductionPriority PriorityFilter;

    // ?�產線Widget?��?
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    TMap<FString, UMingGoRTSProductionLineWidget*> ProductionLineWidgets;

    // ?�方Widget?��?
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    TArray<UMingGoRTSRecipeWidget*> RecipeWidgets;

    // ?��X�UI
    virtual void NativeConstruct() override;

    // ?�建?�產線Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Manager UI")
    UMingGoRTSProductionLineWidget* CreateProductionLineWidget(const FProductionLine& ProductionLine};

    // ?�建?�方Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Manager UI")
    UMingGoRTSRecipeWidget* CreateRecipeWidget(const FProductionRecipe& Recipe};

    // ?��X�產線�X    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionLineClicked(const FString& ProductionLineID};

    // ?��X�方點�?
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnRecipeClicked(const FString& RecipeID};

    // ?��X�產?��?
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionStarted(const FString& ProductionLineID, const FString& RecipeID};

    // ?��X�產?�度
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionProgress(const FString& ProductionLineID, float Progress, float TimeRemaining};

    // ?��X�產完�?
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionCompleted(const FString& ProductionLineID, const TMap<EResourceType, float>& OutputResources};

    // ?��X�產失�?
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionFailed(const FString& ProductionLineID, const FString& ErrorMessage};

    // ?�新?�產線�?�?    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void UpdateProductionLines(};

    // ?�新?�方?�表
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void UpdateRecipeList(};

    // ?�建類別標籤
    void CreateCategoryTabs(};

    // ?�建?��?級�?濾器
    void CreatePriorityFilters(};

    // ?�用?�濾X    void ApplyFilters(};

    // ?�新統�?信息
    void UpdateStatistics(};

private:
    // 綁�X�產系統事件
    void BindProductionSystemEvents(};

    // �X�產系統事件
    void UnbindProductionSystemEvents(};

    // 清�X��?Widget
    void ClearExistingWidgets(};

    // ?��?類別顏色
    FLinearColor GetCategoryColor(EProductionCategory Category) const;
};

/**
 * ?�產統�?UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionStatsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionStatsWidget(};

    // 設置?�產統�X��?
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void SetProductionStatistics(const FProductionStatistics& Stats};

    // 顯示詳細統�?
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void ShowDetailedStats(};

    // 導出統�X��?
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void ExportStatsReport(};

protected:
    // UI組件引用
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* StatsContainer;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TotalProductionTime;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* AverageEfficiency;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TotalRuns;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SuccessRate;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* LastProductionTime;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* ResourceProductionList;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* CategoryProductionList;

    UPROPERTY(meta = (BindWidget))
    class UButton* ExportButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* CloseButton;

    // ?��?統�?
    UPROPERTY(BlueprintReadOnly, Category = "Production Stats UI")
    FProductionStatistics CurrentStats;

    // ?��X�UI
    virtual void NativeConstruct() override;

    // ?�建統�X�目
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Stats UI")
    UUserWidget* CreateStatItem(const FString& Label, const FString& Value};

    // ?�建資�X�產?�目
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Stats UI")
    UUserWidget* CreateResourceStatItem(EResourceType ResourceType, float Amount};

    // ?�建類別?�產?�目
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Stats UI")
    UUserWidget* CreateCategoryStatItem(EProductionCategory Category, float Amount};

    // ?��X�統計數X    FString FormatStatValue(float Value) const;

    // 計�X��X    float CalculateSuccessRate() const;

    // ?��?導出?��?
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void OnExportClicked(};

    // ?��X��X��?
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void OnCloseClicked(};

private:
    // 綁�?事件
    void BindEvents(};

    // �X事件
    void UnbindEvents(};
};

