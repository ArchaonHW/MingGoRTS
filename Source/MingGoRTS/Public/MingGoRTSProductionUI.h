#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Widget.h"
#include "MingGoRTSProductionSystem.h"
#include "MingGoRTSProductionUI.generated.h"

/**
 * 生產線UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionLineWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionLineWidget();

    // 設置生產線數據
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void SetProductionLine(const FProductionLine& ProductionLine);

    // 更新生產狀態
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void UpdateProductionStatus(EProductionStatus NewStatus);

    // 更新生產進度
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void UpdateProductionProgress(float Progress, float TimeRemaining);

    // 設置生產配方
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void SetCurrentRecipe(const FProductionRecipe& Recipe);

    // 設置是否被選中
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void SetIsSelected(bool bSelected);

    // 獲取生產線ID
    UFUNCTION(BlueprintPure, Category = "Production Line UI")
    FString GetProductionLineID() const;

    // 獲取是否被選中
    UFUNCTION(BlueprintPure, Category = "Production Line UI")
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

    // 當前生產線數據
    UPROPERTY(BlueprintReadOnly, Category = "Production Line UI")
    FProductionLine CurrentProductionLine;

    // 當前配方
    UPROPERTY(BlueprintReadOnly, Category = "Production Line UI")
    FProductionRecipe CurrentRecipe;

    // 是否被選中
    UPROPERTY(BlueprintReadOnly, Category = "Production Line UI")
    bool bIsSelected;

    // 初始化UI
    virtual void NativeConstruct() override;

    // 更新UI顯示
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Line UI")
    void OnUpdateUI();

    // 處理開始按鈕
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnStartClicked();

    // 處理暫停按鈕
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnPauseClicked();

    // 處理取消按鈕
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnCancelClicked();

    // 處理升級按鈕
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnUpgradeClicked();

    // 處理自動化按鈕
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnAutomateClicked();

    // 獲取狀態顏色
    FLinearColor GetStatusColor(EProductionStatus Status) const;

    // 獲取優先級顏色
    FLinearColor GetPriorityColor(EProductionPriority Priority) const;

    // 格式化時間顯示
    FString FormatTime(float TimeSeconds) const;

private:
    // 綁定事件
    void BindEvents();

    // 解綁事件
    void UnbindEvents();
};

/**
 * 生產配方UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSRecipeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSRecipeWidget();

    // 設置配方數據
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void SetRecipe(const FProductionRecipe& Recipe);

    // 設置是否可生產
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void SetCanProduce(bool bCanProduce);

    // 設置是否被選中
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void SetIsSelected(bool bSelected);

    // 獲取配方ID
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

    // 當前配方
    UPROPERTY(BlueprintReadOnly, Category = "Recipe UI")
    FProductionRecipe CurrentRecipe;

    // 是否可生產
    UPROPERTY(BlueprintReadOnly, Category = "Recipe UI")
    bool bCanProduce;

    // 是否被選中
    UPROPERTY(BlueprintReadOnly, Category = "Recipe UI")
    bool bIsSelected;

    // 初始化UI
    virtual void NativeConstruct() override;

    // 更新UI顯示
    UFUNCTION(BlueprintImplementableEvent, Category = "Recipe UI")
    void OnUpdateUI();

    // 處理生產按鈕
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void OnProduceClicked();

    // 處理加入隊列按鈕
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void OnQueueClicked();

    // 創建資源列表項
    UUserWidget* CreateResourceItem(EResourceType ResourceType, float Amount, bool bIsInput);

    // 格式化資源顯示
    FString FormatResourceDisplay(EResourceType ResourceType, float Amount) const;

private:
    // 綁定事件
    void BindEvents();

    // 解綁事件
    void UnbindEvents();
};

/**
 * 生產隊列UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionQueueWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionQueueWidget();

    // 設置生產隊列數據
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void SetProductionQueue(const FProductionQueue& Queue);

    // 更新隊列項目
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void UpdateQueueItem(int32 Index, const FString& RecipeID);

    // 移除隊列項目
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void RemoveQueueItem(int32 Index);

    // 清空隊列
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void ClearQueue();

    // 設置循環模式
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void SetLooping(bool bIsLooping);

    // 設置自動開始
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void SetAutoStart(bool bAutoStart);

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

    // 當前隊列
    UPROPERTY(BlueprintReadOnly, Category = "Production Queue UI")
    FProductionQueue CurrentQueue;

    // 隊列項目Widget數組
    UPROPERTY(BlueprintReadOnly, Category = "Production Queue UI")
    TArray<class UMingGoRTSRecipeWidget*> QueueItemWidgets;

    // 初始化UI
    virtual void NativeConstruct() override;

    // 創建隊列項目Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Queue UI")
    UMingGoRTSRecipeWidget* CreateQueueItemWidget(const FString& RecipeID);

    // 處理清空隊列
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void OnClearQueueClicked();

    // 處理循環模式變更
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void OnLoopingChanged(bool bIsLooping);

    // 處理自動開始變更
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void OnAutoStartChanged(bool bAutoStart);

    // 更新隊列計數
    void UpdateQueueCount();

private:
    // 綁定事件
    void BindEvents();

    // 解綁事件
    void UnbindEvents();
};

/**
 * 生產管理主UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionManagerWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionManagerWidget();

    // 設置生產系統引用
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void SetProductionSystem(UMingGoRTSProductionSystem* ProductionSystem);

    // 顯示指定類別的配方
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ShowRecipeCategory(EProductionCategory Category);

    // 顯示所有配方
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ShowAllCategories();

    // 選中生產線
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void SelectProductionLine(const FString& ProductionLineID);

    // 清除選中狀態
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ClearSelection();

    // 創建新生產線
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void CreateNewProductionLine();

    // 顯示生產統計
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ShowProductionStatistics();

    // 設置過濾器
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void SetFilter(EProductionCategory CategoryFilter, EProductionPriority PriorityFilter);

    // 清除過濾器
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ClearFilter();

    // 獲取當前選中的生產線
    UFUNCTION(BlueprintPure, Category = "Production Manager UI")
    FString GetSelectedProductionLineID() const;

    // 獲取當前顯示的類別
    UFUNCTION(BlueprintPure, Category = "Production Manager UI")
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

    // 生產系統引用
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    UMingGoRTSProductionSystem* ProductionSystem;

    // 當前選中的生產線ID
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    FString SelectedProductionLineID;

    // 當前顯示的類別
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    EProductionCategory CurrentCategory;

    // 當前過濾器
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    EProductionCategory CategoryFilter;

    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    EProductionPriority PriorityFilter;

    // 生產線Widget映射
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    TMap<FString, UMingGoRTSProductionLineWidget*> ProductionLineWidgets;

    // 配方Widget數組
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    TArray<UMingGoRTSRecipeWidget*> RecipeWidgets;

    // 初始化UI
    virtual void NativeConstruct() override;

    // 創建生產線Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Manager UI")
    UMingGoRTSProductionLineWidget* CreateProductionLineWidget(const FProductionLine& ProductionLine);

    // 創建配方Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Manager UI")
    UMingGoRTSRecipeWidget* CreateRecipeWidget(const FProductionRecipe& Recipe);

    // 處理生產線點擊
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionLineClicked(const FString& ProductionLineID);

    // 處理配方點擊
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnRecipeClicked(const FString& RecipeID);

    // 處理生產開始
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionStarted(const FString& ProductionLineID, const FString& RecipeID);

    // 處理生產進度
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionProgress(const FString& ProductionLineID, float Progress, float TimeRemaining);

    // 處理生產完成
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionCompleted(const FString& ProductionLineID, const TMap<EResourceType, float>& OutputResources);

    // 處理生產失敗
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionFailed(const FString& ProductionLineID, const FString& ErrorMessage);

    // 更新生產線列表
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void UpdateProductionLines();

    // 更新配方列表
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void UpdateRecipeList();

    // 創建類別標籤
    void CreateCategoryTabs();

    // 創建優先級過濾器
    void CreatePriorityFilters();

    // 應用過濾器
    void ApplyFilters();

    // 更新統計信息
    void UpdateStatistics();

private:
    // 綁定生產系統事件
    void BindProductionSystemEvents();

    // 解綁生產系統事件
    void UnbindProductionSystemEvents();

    // 清理現有Widget
    void ClearExistingWidgets();

    // 獲取類別顏色
    FLinearColor GetCategoryColor(EProductionCategory Category) const;
};

/**
 * 生產統計UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionStatsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionStatsWidget();

    // 設置生產統計數據
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void SetProductionStatistics(const FProductionStatistics& Stats);

    // 顯示詳細統計
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void ShowDetailedStats();

    // 導出統計報告
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void ExportStatsReport();

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

    // 當前統計
    UPROPERTY(BlueprintReadOnly, Category = "Production Stats UI")
    FProductionStatistics CurrentStats;

    // 初始化UI
    virtual void NativeConstruct() override;

    // 創建統計項目
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Stats UI")
    UUserWidget* CreateStatItem(const FString& Label, const FString& Value);

    // 創建資源生產項目
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Stats UI")
    UUserWidget* CreateResourceStatItem(EResourceType ResourceType, float Amount);

    // 創建類別生產項目
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Stats UI")
    UUserWidget* CreateCategoryStatItem(EProductionCategory Category, float Amount);

    // 格式化統計數據
    FString FormatStatValue(float Value) const;

    // 計算成功率
    float CalculateSuccessRate() const;

    // 處理導出按鈕
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void OnExportClicked();

    // 處理關閉按鈕
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void OnCloseClicked();

private:
    // 綁定事件
    void BindEvents();

    // 解綁事件
    void UnbindEvents();
};
