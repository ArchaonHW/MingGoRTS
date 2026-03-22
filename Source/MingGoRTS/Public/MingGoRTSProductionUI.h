#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Widget.h"
#include "MingGoRTSProductionSystem.h"
#include "MingGoRTSProductionUI.generated.h"

/**
 * 進科技�uUI�ե�
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSProductionLineWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionLineWidget();

    // �]�m進科技�u科技X
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void SetProductionLine(const FProductionLine& ProductionLine);

    // 進�s進科技?數據
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void UpdateProductionStatus(EProductionStatus NewStatus);

    // 進�s進科技進科技
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void UpdateProductionProgress(float Progress, float TimeRemaining);

    // �]�m進科技進科技
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void SetCurrentRecipe(const FProductionRecipe& Recipe);

    // �]�m進�_�Q科技進
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void SetIsSelected(bool bSelected);

    // 拆除中X?科技�uID
    UFUNCTION(BlueprintPure, Category = "Production Line UI")
    FString GetProductionLineID() const;

    // 拆除中X?�_�Q科技進
    UFUNCTION(BlueprintPure, Category = "Production Line UI")
    bool IsSelected() const;

protected:
    // UI�ե�ޥ�
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

    // 拆除中X?科技�u科技X
    UPROPERTY(BlueprintReadOnly, Category = "Production Line UI")
    FProductionLine CurrentProductionLine;

    // 拆除中X?科技
    UPROPERTY(BlueprintReadOnly, Category = "Production Line UI")
    FProductionRecipe CurrentRecipe;

    // 進�_�Q科技進
    UPROPERTY(BlueprintReadOnly, Category = "Production Line UI")
    bool bIsSelected;

    // 拆除中X?UI
    virtual void NativeConstruct() override;

    // 進�sUI科技�
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Line UI")
    void OnUpdateUI();

    // 拆除中X?數據進?
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnStartClicked();

    // 拆除中X?數據進?
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnPauseClicked();

    // 拆除中X?數據進?
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnCancelClicked();

    // 拆除中X?數據進?
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnUpgradeClicked();

    // 拆除中X?數據?數據
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnAutomateClicked();

    // 拆除中X進?數據
    FLinearColor GetStatusColor(EProductionStatus Status) const;

    // 拆除中X進?科技數據
    FLinearColor GetPriorityColor(EProductionPriority Priority) const;

    // 拆除中X?數據?科技進
    FString FormatTime(float TimeSeconds) const;

private:
    // �j進�ƥ�
    void BindEvents();

    // 數據�ƥ�
    void UnbindEvents();
};

/**
 * 進科技進科技UI�ե�
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSRecipeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSRecipeWidget();

    // �]�m進科技進進
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void SetRecipe(const FProductionRecipe& Recipe);

    // �]�m進�_進數據
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void SetCanProduce(bool bCanProduce);

    // �]�m進�_�Q科技進
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void SetIsSelected(bool bSelected);

    // 拆除中X?科技ID
    UFUNCTION(BlueprintPure, Category = "Recipe UI")
    FString GetRecipeID() const;

protected:
    // UI�ե�ޥ�
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

    // 拆除中X?科技
    UPROPERTY(BlueprintReadOnly, Category = "Recipe UI")
    FProductionRecipe CurrentRecipe;

    // 進�_進數據
    UPROPERTY(BlueprintReadOnly, Category = "Recipe UI")
    bool bCanProduce;

    // 進�_�Q科技進
    UPROPERTY(BlueprintReadOnly, Category = "Recipe UI")
    bool bIsSelected;

    // 拆除中X?UI
    virtual void NativeConstruct() override;

    // 進�sUI科技�
    UFUNCTION(BlueprintImplementableEvent, Category = "Recipe UI")
    void OnUpdateUI();

    // 拆除中X?科技進進
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void OnProduceClicked();

    // 拆除中X?�J進數據進?
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void OnQueueClicked();

    // 進�ظ�數據?科技X
    UUserWidget* CreateResourceItem(EResourceType ResourceType, float Amount, bool bIsInput);

    // 拆除中X進?科技科技進
    FString FormatResourceDisplay(EResourceType ResourceType, float Amount) const;

private:
    // �j進�ƥ�
    void BindEvents();

    // 數據�ƥ�
    void UnbindEvents();
};

/**
 * 進科技進進UI�ե�
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSProductionQueueWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionQueueWidget();

    // �]�m進科技進數據進?
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void SetProductionQueue(const FProductionQueue& Queue);

    // 進�s進數據?科技
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void UpdateQueueItem(int32 Index, const FString& RecipeID);

    // 科技科技進數據?科技
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void RemoveQueueItem(int32 Index);

    // �M科技進進
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void ClearQueue();

    // �]�m�`科技�進
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void SetLooping(bool bIsLooping);

    // �]�m進數據進?
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void SetAutoStart(bool bAutoStart);

protected:
    // UI�ե�ޥ�
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

    // 拆除中X進?
    UPROPERTY(BlueprintReadOnly, Category = "Production Queue UI")
    FProductionQueue CurrentQueue;

    // 拆除中X?科技Widget進進
    UPROPERTY(BlueprintReadOnly, Category = "Production Queue UI")
    TArray<class UMingGoRTSRecipeWidget*> QueueItemWidgets;

    // 拆除中X?UI
    virtual void NativeConstruct() override;

    // 進科技進數據?科技Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Queue UI")
    UMingGoRTSRecipeWidget* CreateQueueItemWidget(const FString& RecipeID);

    // 拆除中?�M科技進進
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void OnClearQueueClicked();

    // 拆除中?�`科技�進�ܧ�
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void OnLoopingChanged(bool bIsLooping);

    // 拆除中X?數據進?�ܧ�
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void OnAutoStartChanged(bool bAutoStart);

    // 進�s進進�p科技
    void UpdateQueueCount();

private:
    // �j進�ƥ�
    void BindEvents();

    // 數據�ƥ�
    void UnbindEvents();
};

/**
 * 進科技科技進�DUI�ե�
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSProductionManagerWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionManagerWidget();

    // �]�m進科技�t�Τޥ�
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void SetProductionSystem(UMingGoRTSProductionSystem* ProductionSystem);

    // 科技�進進科技�O進數據
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ShowRecipeCategory(EProductionCategory Category);

    // 科技�進進數據
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ShowAllCategories();

    // 進科技進科技進
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void SelectProductionLine(const FString& ProductionLineID);

    // �M科技進科技?數據
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ClearSelection();

    // 進科技進數據進?
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void CreateNewProductionLine();

    // 科技�進科技科技進
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ShowProductionStatistics();

    // �]�m進�oX
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void SetFilter(EProductionCategory CategoryFilter, EProductionPriority PriorityFilter);

    // �M科技進�oX
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ClearFilter();

    // 拆除中X?數據?科技進數據進?
    UFUNCTION(BlueprintPure, Category = "Production Manager UI")
    FString GetSelectedProductionLineID() const;

    // 拆除中X進?科技�進數據
    UFUNCTION(BlueprintPure, Category = "Production Manager UI")
    EProductionCategory GetCurrentCategory() const;

protected:
    // UI�ե�ޥ�
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

    // 進科技�t�Τޥ�
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    UMingGoRTSProductionSystem* ProductionSystem;

    // 拆除中X?科技進數據進?ID
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    FString SelectedProductionLineID;

    // 拆除中?科技�進數據
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    EProductionCategory CurrentCategory;

    // 拆除中X?�oX
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    EProductionCategory CategoryFilter;

    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    EProductionPriority PriorityFilter;

    // 進科技�uWidget進進
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    TMap<FString, UMingGoRTSProductionLineWidget*> ProductionLineWidgets;

    // 進科技Widget進進
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    TArray<UMingGoRTSRecipeWidget*> RecipeWidgets;

    // 拆除中X?UI
    virtual void NativeConstruct() override;

    // 進科技進科技�uWidget
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Manager UI")
    UMingGoRTSProductionLineWidget* CreateProductionLineWidget(const FProductionLine& ProductionLine);

    // 進科技進科技Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Manager UI")
    UMingGoRTSRecipeWidget* CreateRecipeWidget(const FProductionRecipe& Recipe);

    // 拆除中X?科技�u數據
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionLineClicked(const FString& ProductionLineID);

    // 拆除中X?科技�I進
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnRecipeClicked(const FString& RecipeID);

    // 拆除中X?科技進進
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionStarted(const FString& ProductionLineID, const FString& RecipeID);

    // 拆除中X?科技進科技
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionProgress(const FString& ProductionLineID, float Progress, float TimeRemaining);

    // 拆除中X?科技科技進
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionCompleted(const FString& ProductionLineID, const TMap<EResourceType, float>& OutputResources);

    // 拆除中X?科技科技進
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionFailed(const FString& ProductionLineID, const FString& ErrorMessage);

    // 進�s進科技�u進進
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void UpdateProductionLines();

    // 進�s進科技進科技
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void UpdateRecipeList();

    // 進科技科技�O科技科技
    void CreateCategoryTabs();

    // 進科技進進科技進�o科技
    void CreatePriorityFilters();

    // 進科技進�oX
    void ApplyFilters();

    // 進�s科技進�H科技
    void UpdateStatistics();

private:
    // �j數據?科技�t�Ψƥ�
    void BindProductionSystemEvents();

    // 數據?科技�t�Ψƥ�
    void UnbindProductionSystemEvents();

    // �M數據進規idget
    void ClearExistingWidgets();

    // 拆除中?科技�O�C科技
    FLinearColor GetCategoryColor(EProductionCategory Category) const;
};

/**
 * 進科技科技進UI�ե�
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSProductionStatsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionStatsWidget();

    // �]�m進科技科技數據進?
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void SetProductionStatistics(const FProductionStatistics& Stats);

    // 科技ܸԲӲ�進
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void ShowDetailedStats();

    // �ɥX科技數據進?
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void ExportStatsReport();

protected:
    // UI�ե�ޥ�
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

    // 拆除中?科技進
    UPROPERTY(BlueprintReadOnly, Category = "Production Stats UI")
    FProductionStatistics CurrentStats;

    // 拆除中X?UI
    virtual void NativeConstruct() override;

    // 進�ز�數據?科技
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Stats UI")
    UUserWidget* CreateStatItem(const FString& Label, const FString& Value);

    // 進�ظ�數據?科技進科技
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Stats UI")
    UUserWidget* CreateResourceStatItem(EResourceType ResourceType, float Amount);

    // 進科技科技�O進科技進科技
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Stats UI")
    UUserWidget* CreateCategoryStatItem(EProductionCategory Category, float Amount);

    // 拆除中X?�έp科技X
    FString FormatStatValue(float Value) const;

    // �p數據?數據
    float CalculateSuccessRate() const;

    // 拆除中?�ɥX進進
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void OnExportClicked();

    // 拆除中X?數據進?
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void OnCloseClicked();

private:
    // �j進�ƥ�
    void BindEvents();

    // 數據�ƥ�
    void UnbindEvents();
};