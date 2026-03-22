#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Widget.h"
#include "MingGoRTSProductionSystem.h"
#include "MingGoRTSProductionUI.generated.h"

/**
 * ??���uUI�ե�
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionLineWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionLineWidget();

    // �]�m??���u��X
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void SetProductionLine(const FProductionLine& ProductionLine);

    // ??�s??��??X
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void UpdateProductionStatus(EProductionStatus NewStatus);

    // ??�s??��??��
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void UpdateProductionProgress(float Progress, float TimeRemaining);

    // �]�m??��??��
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void SetCurrentRecipe(const FProductionRecipe& Recipe);

    // �]�m??�_�Q��??
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void SetIsSelected(bool bSelected);

    // ???X?���uID
    UFUNCTION(BlueprintPure, Category = "Production Line UI")
    FString GetProductionLineID() const;

    // ???X?�_�Q��??
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

    // ???X?���u��X
    UPROPERTY(BlueprintReadOnly, Category = "Production Line UI")
    FProductionLine CurrentProductionLine;

    // ???X?��
    UPROPERTY(BlueprintReadOnly, Category = "Production Line UI")
    FProductionRecipe CurrentRecipe;

    // ??�_�Q��??
    UPROPERTY(BlueprintReadOnly, Category = "Production Line UI")
    bool bIsSelected;

    // ???X?UI
    virtual void NativeConstruct() override;

    // ??�sUI���
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Line UI")
    void OnUpdateUI();

    // ???X??X???
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnStartClicked();

    // ???X??X???
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnPauseClicked();

    // ???X??X???
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnCancelClicked();

    // ???X??X???
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnUpgradeClicked();

    // ???X??X??X
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnAutomateClicked();

    // ???X????X
    FLinearColor GetStatusColor(EProductionStatus Status) const;

    // ???X???��?X
    FLinearColor GetPriorityColor(EProductionPriority Priority) const;

    // ???X??X?��??
    FString FormatTime(float TimeSeconds) const;

private:
    // �j??�ƥ�
    void BindEvents();

    // ?X�ƥ�
    void UnbindEvents();
};

/**
 * ??��??��UI�ե�
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSRecipeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSRecipeWidget();

    // �]�m??��????
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void SetRecipe(const FProductionRecipe& Recipe);

    // �]�m??�_???X
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void SetCanProduce(bool bCanProduce);

    // �]�m??�_�Q��??
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void SetIsSelected(bool bSelected);

    // ???X?��ID
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

    // ???X?��
    UPROPERTY(BlueprintReadOnly, Category = "Recipe UI")
    FProductionRecipe CurrentRecipe;

    // ??�_???X
    UPROPERTY(BlueprintReadOnly, Category = "Recipe UI")
    bool bCanProduce;

    // ??�_�Q��??
    UPROPERTY(BlueprintReadOnly, Category = "Recipe UI")
    bool bIsSelected;

    // ???X?UI
    virtual void NativeConstruct() override;

    // ??�sUI���
    UFUNCTION(BlueprintImplementableEvent, Category = "Recipe UI")
    void OnUpdateUI();

    // ???X?��????
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void OnProduceClicked();

    // ???X?�J???X???
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void OnQueueClicked();

    // ??�ظ�?X?��X
    UUserWidget* CreateResourceItem(EResourceType ResourceType, float Amount, bool bIsInput);

    // ???X???����??
    FString FormatResourceDisplay(EResourceType ResourceType, float Amount) const;

private:
    // �j??�ƥ�
    void BindEvents();

    // ?X�ƥ�
    void UnbindEvents();
};

/**
 * ??��????UI�ե�
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionQueueWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionQueueWidget();

    // �]�m??��???X???
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void SetProductionQueue(const FProductionQueue& Queue);

    // ??�s???X?��
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void UpdateQueueItem(int32 Index, const FString& RecipeID);

    // ����???X?��
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void RemoveQueueItem(int32 Index);

    // �M��????
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void ClearQueue();

    // �]�m�`���??
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void SetLooping(bool bIsLooping);

    // �]�m???X???
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

    // ???X???
    UPROPERTY(BlueprintReadOnly, Category = "Production Queue UI")
    FProductionQueue CurrentQueue;

    // ???X?��Widget????
    UPROPERTY(BlueprintReadOnly, Category = "Production Queue UI")
    TArray<class UMingGoRTSRecipeWidget*> QueueItemWidgets;

    // ???X?UI
    virtual void NativeConstruct() override;

    // ??��???X?��Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Queue UI")
    UMingGoRTSRecipeWidget* CreateQueueItemWidget(const FString& RecipeID);

    // ????�M��????
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void OnClearQueueClicked();

    // ????�`���??�ܧ�
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void OnLoopingChanged(bool bIsLooping);

    // ???X??X???�ܧ�
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void OnAutoStartChanged(bool bAutoStart);

    // ??�s????�p��
    void UpdateQueueCount();

private:
    // �j??�ƥ�
    void BindEvents();

    // ?X�ƥ�
    void UnbindEvents();
};

/**
 * ??����??�DUI�ե�
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionManagerWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionManagerWidget();

    // �]�m??���t�Τޥ�
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void SetProductionSystem(UMingGoRTSProductionSystem* ProductionSystem);

    // ���????���O???X
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ShowRecipeCategory(EProductionCategory Category);

    // ���?????X
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ShowAllCategories();

    // ??��??��??
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void SelectProductionLine(const FString& ProductionLineID);

    // �M��??��??X
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ClearSelection();

    // ??��???X???
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void CreateNewProductionLine();

    // ���??����??
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ShowProductionStatistics();

    // �]�m??�oX
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void SetFilter(EProductionCategory CategoryFilter, EProductionPriority PriorityFilter);

    // �M��??�oX
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ClearFilter();

    // ???X??X?��???X???
    UFUNCTION(BlueprintPure, Category = "Production Manager UI")
    FString GetSelectedProductionLineID() const;

    // ???X???���???X
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

    // ??���t�Τޥ�
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    UMingGoRTSProductionSystem* ProductionSystem;

    // ???X?��???X???ID
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    FString SelectedProductionLineID;

    // ????���???X
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    EProductionCategory CurrentCategory;

    // ???X?�oX
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    EProductionCategory CategoryFilter;

    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    EProductionPriority PriorityFilter;

    // ??���uWidget????
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    TMap<FString, UMingGoRTSProductionLineWidget*> ProductionLineWidgets;

    // ??��Widget????
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    TArray<UMingGoRTSRecipeWidget*> RecipeWidgets;

    // ???X?UI
    virtual void NativeConstruct() override;

    // ??��??���uWidget
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Manager UI")
    UMingGoRTSProductionLineWidget* CreateProductionLineWidget(const FProductionLine& ProductionLine);

    // ??��??��Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Manager UI")
    UMingGoRTSRecipeWidget* CreateRecipeWidget(const FProductionRecipe& Recipe);

    // ???X?���u?X
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionLineClicked(const FString& ProductionLineID);

    // ???X?���I??
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnRecipeClicked(const FString& RecipeID);

    // ???X?��????
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionStarted(const FString& ProductionLineID, const FString& RecipeID);

    // ???X?��??��
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionProgress(const FString& ProductionLineID, float Progress, float TimeRemaining);

    // ???X?����??
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionCompleted(const FString& ProductionLineID, const TMap<EResourceType, float>& OutputResources);

    // ???X?����??
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionFailed(const FString& ProductionLineID, const FString& ErrorMessage);

    // ??�s??���u????
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void UpdateProductionLines();

    // ??�s??��??��
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void UpdateRecipeList();

    // ??�����O����
    void CreateCategoryTabs();

    // ??��????��??�o��
    void CreatePriorityFilters();

    // ??��??�oX
    void ApplyFilters();

    // ??�s��??�H��
    void UpdateStatistics();

private:
    // �j?X?���t�Ψƥ�
    void BindProductionSystemEvents();

    // ?X?���t�Ψƥ�
    void UnbindProductionSystemEvents();

    // �M?X???Widget
    void ClearExistingWidgets();

    // ????���O�C��
    FLinearColor GetCategoryColor(EProductionCategory Category) const;
};

/**
 * ??����??UI�ե�
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionStatsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionStatsWidget();

    // �]�m??����?X???
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void SetProductionStatistics(const FProductionStatistics& Stats);

    // ��ܸԲӲ�??
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void ShowDetailedStats();

    // �ɥX��?X???
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

    // ????��??
    UPROPERTY(BlueprintReadOnly, Category = "Production Stats UI")
    FProductionStatistics CurrentStats;

    // ???X?UI
    virtual void NativeConstruct() override;

    // ??�ز�?X?��
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Stats UI")
    UUserWidget* CreateStatItem(const FString& Label, const FString& Value);

    // ??�ظ�?X?��??��
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Stats UI")
    UUserWidget* CreateResourceStatItem(EResourceType ResourceType, float Amount);

    // ??�����O??��??��
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Stats UI")
    UUserWidget* CreateCategoryStatItem(EProductionCategory Category, float Amount);

    // ???X?�έp��X
    FString FormatStatValue(float Value) const;

    // �p?X??X
    float CalculateSuccessRate() const;

    // ????�ɥX????
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void OnExportClicked();

    // ???X??X???
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void OnCloseClicked();

private:
    // �j??�ƥ�
    void BindEvents();

    // ?X�ƥ�
    void UnbindEvents();
};