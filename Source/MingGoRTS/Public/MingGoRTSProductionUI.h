#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Widget.h"
#include "MingGoRTSProductionSystem.h"
#include "MingGoRTSProductionUI.generated.h"

/**
 * ?üÁî¢Á∑öUIÁµÑ‰ª∂
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionLineWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionLineWidget(};

    // Ë®≠ÁΩÆ?üÁî¢Á∑öÊï∏??    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void SetProductionLine(const FProductionLine& ProductionLine};

    // ?¥Êñ∞?üÁî¢?Ä??    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void UpdateProductionStatus(EProductionStatus NewStatus};

    // ?¥Êñ∞?üÁî¢?≤Â∫¶
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void UpdateProductionProgress(float Progress, float TimeRemaining};

    // Ë®≠ÁΩÆ?üÁî¢?çÊñπ
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void SetCurrentRecipe(const FProductionRecipe& Recipe};

    // Ë®≠ÁΩÆ?ØÂê¶Ë¢´ÈÅ∏‰∏?    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void SetIsSelected(bool bSelected};

    // ?≤Â??üÁî¢Á∑öID
    UFUNCTION(BlueprintPure, Category = "Production Line UI")
    FString GetProductionLineID() const;

    // ?≤Â??ØÂê¶Ë¢´ÈÅ∏‰∏?    UFUNCTION(BlueprintPure, Category = "Production Line UI")
    bool IsSelected() const;

protected:
    // UIÁµÑ‰ª∂ÂºïÁî®
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

    // ?∂Â??üÁî¢Á∑öÊï∏??    UPROPERTY(BlueprintReadOnly, Category = "Production Line UI")
    FProductionLine CurrentProductionLine;

    // ?∂Â??çÊñπ
    UPROPERTY(BlueprintReadOnly, Category = "Production Line UI")
    FProductionRecipe CurrentRecipe;

    // ?ØÂê¶Ë¢´ÈÅ∏‰∏?    UPROPERTY(BlueprintReadOnly, Category = "Production Line UI")
    bool bIsSelected;

    // ?ùÂ??ñUI
    virtual void NativeConstruct() override;

    // ?¥Êñ∞UIÈ°ØÁ§∫
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Line UI")
    void OnUpdateUI(};

    // ?ïÁ??ãÂ??âÈ?
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnStartClicked(};

    // ?ïÁ??´Â??âÈ?
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnPauseClicked(};

    // ?ïÁ??ñÊ??âÈ?
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnCancelClicked(};

    // ?ïÁ??áÁ??âÈ?
    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnUpgradeClicked(};

    // ?ïÁ??™Â??ñÊ???    UFUNCTION(BlueprintCallable, Category = "Production Line UI")
    void OnAutomateClicked(};

    // ?≤Â??Ä?ãÈ???    FLinearColor GetStatusColor(EProductionStatus Status) const;

    // ?≤Â??™Â?Á¥öÈ???    FLinearColor GetPriorityColor(EProductionPriority Priority) const;

    // ?ºÂ??ñÊ??ìÈ°ØÁ§?    FString FormatTime(float TimeSeconds) const;

private:
    // Á∂ÅÂ?‰∫ã‰ª∂
    void BindEvents(};

    // Ëß??‰∫ã‰ª∂
    void UnbindEvents(};
};

/**
 * ?üÁî¢?çÊñπUIÁµÑ‰ª∂
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSRecipeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSRecipeWidget(};

    // Ë®≠ÁΩÆ?çÊñπ?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void SetRecipe(const FProductionRecipe& Recipe};

    // Ë®≠ÁΩÆ?ØÂê¶?ØÁ???    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void SetCanProduce(bool bCanProduce};

    // Ë®≠ÁΩÆ?ØÂê¶Ë¢´ÈÅ∏‰∏?    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void SetIsSelected(bool bSelected};

    // ?≤Â??çÊñπID
    UFUNCTION(BlueprintPure, Category = "Recipe UI")
    FString GetRecipeID() const;

protected:
    // UIÁµÑ‰ª∂ÂºïÁî®
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

    // ?∂Â??çÊñπ
    UPROPERTY(BlueprintReadOnly, Category = "Recipe UI")
    FProductionRecipe CurrentRecipe;

    // ?ØÂê¶?ØÁ???    UPROPERTY(BlueprintReadOnly, Category = "Recipe UI")
    bool bCanProduce;

    // ?ØÂê¶Ë¢´ÈÅ∏‰∏?    UPROPERTY(BlueprintReadOnly, Category = "Recipe UI")
    bool bIsSelected;

    // ?ùÂ??ñUI
    virtual void NativeConstruct() override;

    // ?¥Êñ∞UIÈ°ØÁ§∫
    UFUNCTION(BlueprintImplementableEvent, Category = "Recipe UI")
    void OnUpdateUI(};

    // ?ïÁ??üÁî¢?âÈ?
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void OnProduceClicked(};

    // ?ïÁ??†ÂÖ•?äÂ??âÈ?
    UFUNCTION(BlueprintCallable, Category = "Recipe UI")
    void OnQueueClicked(};

    // ?µÂª∫Ë≥áÊ??óË°®??    UUserWidget* CreateResourceItem(EResourceType ResourceType, float Amount, bool bIsInput};

    // ?ºÂ??ñË?Ê∫êÈ°ØÁ§?    FString FormatResourceDisplay(EResourceType ResourceType, float Amount) const;

private:
    // Á∂ÅÂ?‰∫ã‰ª∂
    void BindEvents(};

    // Ëß??‰∫ã‰ª∂
    void UnbindEvents(};
};

/**
 * ?üÁî¢?äÂ?UIÁµÑ‰ª∂
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionQueueWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionQueueWidget(};

    // Ë®≠ÁΩÆ?üÁî¢?äÂ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void SetProductionQueue(const FProductionQueue& Queue};

    // ?¥Êñ∞?äÂ??ÖÁõÆ
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void UpdateQueueItem(int32 Index, const FString& RecipeID};

    // ÁßªÈô§?äÂ??ÖÁõÆ
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void RemoveQueueItem(int32 Index};

    // Ê∏ÖÁ©∫?äÂ?
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void ClearQueue(};

    // Ë®≠ÁΩÆÂæ™Áí∞Ê®°Â?
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void SetLooping(bool bIsLooping};

    // Ë®≠ÁΩÆ?™Â??ãÂ?
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void SetAutoStart(bool bAutoStart};

protected:
    // UIÁµÑ‰ª∂ÂºïÁî®
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

    // ?∂Â??äÂ?
    UPROPERTY(BlueprintReadOnly, Category = "Production Queue UI")
    FProductionQueue CurrentQueue;

    // ?äÂ??ÖÁõÆWidget?∏Á?
    UPROPERTY(BlueprintReadOnly, Category = "Production Queue UI")
    TArray<class UMingGoRTSRecipeWidget*> QueueItemWidgets;

    // ?ùÂ??ñUI
    virtual void NativeConstruct() override;

    // ?µÂª∫?äÂ??ÖÁõÆWidget
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Queue UI")
    UMingGoRTSRecipeWidget* CreateQueueItemWidget(const FString& RecipeID};

    // ?ïÁ?Ê∏ÖÁ©∫?äÂ?
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void OnClearQueueClicked(};

    // ?ïÁ?Âæ™Áí∞Ê®°Â?ËÆäÊõ¥
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void OnLoopingChanged(bool bIsLooping};

    // ?ïÁ??™Â??ãÂ?ËÆäÊõ¥
    UFUNCTION(BlueprintCallable, Category = "Production Queue UI")
    void OnAutoStartChanged(bool bAutoStart};

    // ?¥Êñ∞?äÂ?Ë®àÊï∏
    void UpdateQueueCount(};

private:
    // Á∂ÅÂ?‰∫ã‰ª∂
    void BindEvents(};

    // Ëß??‰∫ã‰ª∂
    void UnbindEvents(};
};

/**
 * ?üÁî¢ÁÆ°Á?‰∏ªUIÁµÑ‰ª∂
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionManagerWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionManagerWidget(};

    // Ë®≠ÁΩÆ?üÁî¢Á≥ªÁµ±ÂºïÁî®
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void SetProductionSystem(UMingGoRTSProductionSystem* ProductionSystem};

    // È°ØÁ§∫?áÂ?È°ûÂà•?ÑÈ???    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ShowRecipeCategory(EProductionCategory Category};

    // È°ØÁ§∫?Ä?âÈ???    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ShowAllCategories(};

    // ?∏‰∏≠?üÁî¢Á∑?    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void SelectProductionLine(const FString& ProductionLineID};

    // Ê∏ÖÈô§?∏‰∏≠?Ä??    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ClearSelection(};

    // ?µÂª∫?∞Á??¢Á?
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void CreateNewProductionLine(};

    // È°ØÁ§∫?üÁî¢Áµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ShowProductionStatistics(};

    // Ë®≠ÁΩÆ?éÊøæ??    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void SetFilter(EProductionCategory CategoryFilter, EProductionPriority PriorityFilter};

    // Ê∏ÖÈô§?éÊøæ??    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void ClearFilter(};

    // ?≤Â??∂Â??∏‰∏≠?ÑÁ??¢Á?
    UFUNCTION(BlueprintPure, Category = "Production Manager UI")
    FString GetSelectedProductionLineID() const;

    // ?≤Â??∂Â?È°ØÁ§∫?ÑÈ???    UFUNCTION(BlueprintPure, Category = "Production Manager UI")
    EProductionCategory GetCurrentCategory() const;

protected:
    // UIÁµÑ‰ª∂ÂºïÁî®
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

    // ?üÁî¢Á≥ªÁµ±ÂºïÁî®
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    UMingGoRTSProductionSystem* ProductionSystem;

    // ?∂Â??∏‰∏≠?ÑÁ??¢Á?ID
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    FString SelectedProductionLineID;

    // ?∂Â?È°ØÁ§∫?ÑÈ???    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    EProductionCategory CurrentCategory;

    // ?∂Â??éÊøæ??    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    EProductionCategory CategoryFilter;

    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    EProductionPriority PriorityFilter;

    // ?üÁî¢Á∑öWidget?†Â?
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    TMap<FString, UMingGoRTSProductionLineWidget*> ProductionLineWidgets;

    // ?çÊñπWidget?∏Á?
    UPROPERTY(BlueprintReadOnly, Category = "Production Manager UI")
    TArray<UMingGoRTSRecipeWidget*> RecipeWidgets;

    // ?ùÂ??ñUI
    virtual void NativeConstruct() override;

    // ?µÂª∫?üÁî¢Á∑öWidget
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Manager UI")
    UMingGoRTSProductionLineWidget* CreateProductionLineWidget(const FProductionLine& ProductionLine};

    // ?µÂª∫?çÊñπWidget
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Manager UI")
    UMingGoRTSRecipeWidget* CreateRecipeWidget(const FProductionRecipe& Recipe};

    // ?ïÁ??üÁî¢Á∑öÈ???    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionLineClicked(const FString& ProductionLineID};

    // ?ïÁ??çÊñπÈªûÊ?
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnRecipeClicked(const FString& RecipeID};

    // ?ïÁ??üÁî¢?ãÂ?
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionStarted(const FString& ProductionLineID, const FString& RecipeID};

    // ?ïÁ??üÁî¢?≤Â∫¶
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionProgress(const FString& ProductionLineID, float Progress, float TimeRemaining};

    // ?ïÁ??üÁî¢ÂÆåÊ?
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionCompleted(const FString& ProductionLineID, const TMap<EResourceType, float>& OutputResources};

    // ?ïÁ??üÁî¢Â§±Ê?
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void OnProductionFailed(const FString& ProductionLineID, const FString& ErrorMessage};

    // ?¥Êñ∞?üÁî¢Á∑öÂ?Ë°?    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void UpdateProductionLines(};

    // ?¥Êñ∞?çÊñπ?óË°®
    UFUNCTION(BlueprintCallable, Category = "Production Manager UI")
    void UpdateRecipeList(};

    // ?µÂª∫È°ûÂà•Ê®ôÁ±§
    void CreateCategoryTabs(};

    // ?µÂª∫?™Â?Á¥öÈ?ÊøæÂô®
    void CreatePriorityFilters(};

    // ?âÁî®?éÊøæ??    void ApplyFilters(};

    // ?¥Êñ∞Áµ±Ë?‰ø°ÊÅØ
    void UpdateStatistics(};

private:
    // Á∂ÅÂ??üÁî¢Á≥ªÁµ±‰∫ã‰ª∂
    void BindProductionSystemEvents(};

    // Ëß???üÁî¢Á≥ªÁµ±‰∫ã‰ª∂
    void UnbindProductionSystemEvents(};

    // Ê∏ÖÁ??æÊ?Widget
    void ClearExistingWidgets(};

    // ?≤Â?È°ûÂà•È°èËâ≤
    FLinearColor GetCategoryColor(EProductionCategory Category) const;
};

/**
 * ?üÁî¢Áµ±Ë?UIÁµÑ‰ª∂
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionStatsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionStatsWidget(};

    // Ë®≠ÁΩÆ?üÁî¢Áµ±Ë??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void SetProductionStatistics(const FProductionStatistics& Stats};

    // È°ØÁ§∫Ë©≥Á¥∞Áµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void ShowDetailedStats(};

    // Â∞éÂá∫Áµ±Ë??±Â?
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void ExportStatsReport(};

protected:
    // UIÁµÑ‰ª∂ÂºïÁî®
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

    // ?∂Â?Áµ±Ë?
    UPROPERTY(BlueprintReadOnly, Category = "Production Stats UI")
    FProductionStatistics CurrentStats;

    // ?ùÂ??ñUI
    virtual void NativeConstruct() override;

    // ?µÂª∫Áµ±Ë??ÖÁõÆ
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Stats UI")
    UUserWidget* CreateStatItem(const FString& Label, const FString& Value};

    // ?µÂª∫Ë≥áÊ??üÁî¢?ÖÁõÆ
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Stats UI")
    UUserWidget* CreateResourceStatItem(EResourceType ResourceType, float Amount};

    // ?µÂª∫È°ûÂà•?üÁî¢?ÖÁõÆ
    UFUNCTION(BlueprintImplementableEvent, Category = "Production Stats UI")
    UUserWidget* CreateCategoryStatItem(EProductionCategory Category, float Amount};

    // ?ºÂ??ñÁµ±Ë®àÊï∏??    FString FormatStatValue(float Value) const;

    // Ë®àÁ??êÂ???    float CalculateSuccessRate() const;

    // ?ïÁ?Â∞éÂá∫?âÈ?
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void OnExportClicked(};

    // ?ïÁ??úÈ??âÈ?
    UFUNCTION(BlueprintCallable, Category = "Production Stats UI")
    void OnCloseClicked(};

private:
    // Á∂ÅÂ?‰∫ã‰ª∂
    void BindEvents(};

    // Ëß??‰∫ã‰ª∂
    void UnbindEvents(};
};

