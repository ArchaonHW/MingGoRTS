#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Widget.h"
#include "MingGoRTSTechTree.h"
#include "MingGoRTSTechTreeUI.generated.h"

/**
 * ÁßëÊ?Ê®πÁ?ÈªûUIÁµÑ‰ª∂
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSTechNodeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSTechNodeWidget();

    // Ë®≠ÁΩÆÁßëÊ?ÁØÄÈªûÊï∏??    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void SetTechNode(const FTechNode& TechNode);

    // ?¥Êñ∞ÁØÄÈªûÁ???    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void UpdateNodeStatus(ETechStatus NewStatus);

    // ?¥Êñ∞?îÁôº?≤Â∫¶
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void UpdateResearchProgress(float Progress);

    // Ë®≠ÁΩÆ?ØÂê¶Ë¢´ÈÅ∏‰∏?    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void SetIsSelected(bool bSelected);

    // Ë®≠ÁΩÆÁØÄÈªû‰?ÁΩ?    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void SetNodePosition(const FVector2D& Position);

    // ?≤Â?ÁßëÊ?ÁØÄÈªûID
    UFUNCTION(BlueprintPure, Category = "Tech Node UI")
    FString GetTechID() const;

    // ?≤Â??ØÂê¶Ë¢´ÈÅ∏‰∏?    UFUNCTION(BlueprintPure, Category = "Tech Node UI")
    bool IsSelected() const;

protected:
    // UIÁµÑ‰ª∂ÂºïÁî®
    UPROPERTY(meta = (BindWidget))
    class UImage* TechIcon;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TechNameText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TechDescriptionText;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* ResearchProgressBar;

    UPROPERTY(meta = (BindWidget))
    class UButton* ResearchButton;

    UPROPERTY(meta = (BindWidget))
    class UBorder* NodeBorder;

    UPROPERTY(meta = (BindWidget))
    class UOverlay* StatusOverlay;

    // ?∂Â?ÁßëÊ?ÁØÄÈªûÊï∏??    UPROPERTY(BlueprintReadOnly, Category = "Tech Node UI")
    FTechNode CurrentTechNode;

    // ?∂Â??Ä??    UPROPERTY(BlueprintReadOnly, Category = "Tech Node UI")
    ETechStatus CurrentStatus;

    // ?ØÂê¶Ë¢´ÈÅ∏‰∏?    UPROPERTY(BlueprintReadOnly, Category = "Tech Node UI")
    bool bIsSelected;

    // ÁØÄÈªû‰?ÁΩ?    UPROPERTY(BlueprintReadOnly, Category = "Tech Node UI")
    FVector2D NodePosition;

    // ?ùÂ??ñUI
    virtual void NativeConstruct() override;

    // ?¥Êñ∞UIÈ°ØÁ§∫
    UFUNCTION(BlueprintImplementableEvent, Category = "Tech Node UI")
    void OnUpdateUI();

    // ?ïÁ?ÈªûÊ?‰∫ã‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void OnNodeClicked();

    // ?ïÁ??∏Â?‰∫ã‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void OnNodeHovered();

    // ?ïÁ??ñÊ??∏Â?‰∫ã‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void OnNodeUnhovered();

    // ?≤Â??Ä?ãÈ???    FLinearColor GetStatusColor(ETechStatus Status) const;

    // ?≤Â?È°ûÂà•È°èËâ≤
    FLinearColor GetCategoryColor(ETechCategory Category) const;

private:
    // Á∂ÅÂ?‰∫ã‰ª∂
    void BindEvents();

    // Ëß??‰∫ã‰ª∂
    void UnbindEvents();
};

/**
 * ÁßëÊ?Ê®πÈÄ?é•Á∑öUIÁµÑ‰ª∂
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSTechConnectionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSTechConnectionWidget();

    // Ë®≠ÁΩÆ??é•?ÑÂÖ©?ãÁ?Èª?    UFUNCTION(BlueprintCallable, Category = "Tech Connection UI")
    void SetConnection(const FString& FromTechID, const FString& ToTechID);

    // Ë®≠ÁΩÆ??é•?Ä??    UFUNCTION(BlueprintCallable, Category = "Tech Connection UI")
    void SetConnectionStatus(bool bIsActive);

    // Ë®≠ÁΩÆ??é•Á∑öÈ???    UFUNCTION(BlueprintCallable, Category = "Tech Connection UI")
    void SetConnectionColor(const FLinearColor& Color);

    // ?¥Êñ∞??é•Á∑ö‰?ÁΩ?    UFUNCTION(BlueprintCallable, Category = "Tech Connection UI")
    void UpdateConnectionPosition(const FVector2D& FromPosition, const FVector2D& ToPosition);

protected:
    // UIÁµÑ‰ª∂ÂºïÁî®
    UPROPERTY(meta = (BindWidget))
    class UImage* ConnectionLine;

    // Ê∫êÁ??ÄID
    UPROPERTY(BlueprintReadOnly, Category = "Tech Connection UI")
    FString FromTechID;

    // ?ÆÊ?ÁßëÊ?ID
    UPROPERTY(BlueprintReadOnly, Category = "Tech Connection UI")
    FString ToTechID;

    // ??é•?ØÂê¶ÊøÄÊ¥?    UPROPERTY(BlueprintReadOnly, Category = "Tech Connection UI")
    bool bIsActive;

    // ?ùÂ??ñUI
    virtual void NativeConstruct() override;

private:
    // Ë®àÁ???é•Á∑öË∑ØÂæ?    void CalculateConnectionPath();
};

/**
 * ÁßëÊ?Ê®π‰∏ªUIÁµÑ‰ª∂
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSTechTreeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSTechTreeWidget();

    // Ë®≠ÁΩÆÁßëÊ?Ê®πÁ≥ªÁµ±Â???    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void SetTechTreeSystem(UMingGoRTSTechTree* TechTreeSystem);

    // È°ØÁ§∫?áÂ?È°ûÂà•?ÑÁ??ÄÊ®?    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ShowTechCategory(ETechCategory Category);

    // È°ØÁ§∫?Ä?âÁ??ÄÊ®?    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ShowAllCategories();

    // ?∏‰∏≠ÁßëÊ?ÁØÄÈª?    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void SelectTechNode(const FString& TechID);

    // Ê∏ÖÈô§?∏‰∏≠?Ä??    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ClearSelection();

    // ?ãÂ??îÁôº?∏‰∏≠?ÑÁ??Ä
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    bool StartResearchSelectedTech();

    // ?¥Êñ∞ÁßëÊ?Ê®πÈ°ØÁ§?    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void UpdateTechTreeDisplay();

    // Ë®≠ÁΩÆ?éÊøæ??    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void SetFilter(ETechCategory CategoryFilter, ETechTier TierFilter);

    // Ê∏ÖÈô§?éÊøæ??    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ClearFilter();

    // ?≤Â??∂Â??∏‰∏≠?ÑÁ??Ä
    UFUNCTION(BlueprintPure, Category = "Tech Tree UI")
    FString GetSelectedTechID() const;

    // ?≤Â??∂Â?È°ØÁ§∫?ÑÈ???    UFUNCTION(BlueprintPure, Category = "Tech Tree UI")
    ETechCategory GetCurrentCategory() const;

protected:
    // UIÁµÑ‰ª∂ÂºïÁî®
    UPROPERTY(meta = (BindWidget))
    class UCanvasPanel* TechTreeCanvas;

    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* CategoryTabs;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* TierFilters;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* TechTreeScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UButton* StartResearchButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SelectedTechName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SelectedTechDescription;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ResearchCostText;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* ResearchProgress;

    // ÁßëÊ?Ê®πÁ≥ªÁµ±Â???    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    UMingGoRTSTechTree* TechTreeSystem;

    // ?∂Â??∏‰∏≠?ÑÁ??ÄID
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    FString SelectedTechID;

    // ?∂Â?È°ØÁ§∫?ÑÈ???    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    ETechCategory CurrentCategory;

    // ?∂Â??éÊøæ??    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    ETechCategory CategoryFilter;

    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    ETechTier TierFilter;

    // ÁßëÊ?ÁØÄÈªûWidget?†Â?
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    TMap<FString, UMingGoRTSTechNodeWidget*> TechNodeWidgets;

    // ÁßëÊ???é•Widget?∏Á?
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    TArray<UMingGoRTSTechConnectionWidget*> ConnectionWidgets;

    // ?ùÂ??ñUI
    virtual void NativeConstruct() override;

    // ?µÂª∫ÁßëÊ?ÁØÄÈªûWidget
    UFUNCTION(BlueprintImplementableEvent, Category = "Tech Tree UI")
    UMingGoRTSTechNodeWidget* CreateTechNodeWidget(const FTechNode& TechNode);

    // ?µÂª∫??é•Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Tech Tree UI")
    UMingGoRTSTechConnectionWidget* CreateConnectionWidget(const FString& FromTechID, const FString& ToTechID);

    // ?ïÁ?ÁßëÊ?ÁØÄÈªûÈ???    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void OnTechNodeClicked(const FString& TechID);

    // ?ïÁ?ÁßëÊ??îÁôº?ãÂ?
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void OnTechResearchStarted(const FString& TechID, const FString& BuildingID);

    // ?ïÁ?ÁßëÊ??îÁôºÂÆåÊ?
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void OnTechResearchCompleted(const FString& TechID, const TArray<FTechEffect>& Effects);

    // ?ïÁ?ÁßëÊ?Ëß??
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void OnTechUnlocked(const FString& TechID, ETechCategory Category);

    // ?¥Êñ∞?∏‰∏≠ÁßëÊ?‰ø°ÊÅØ
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void UpdateSelectedTechInfo();

    // ‰ΩàÂ?ÁßëÊ?ÁØÄÈª?    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void LayoutTechNodes();

    // ?µÂª∫ÁßëÊ???é•Á∑?    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void CreateTechConnections();

    // ?âÁî®?éÊøæ??    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ApplyFilters();

    // ?≤Â?ÁØÄÈªû‰?Â±Ä‰ΩçÁΩÆ
    FVector2D GetNodeLayoutPosition(const FString& TechID, int32 Row, int32 Column) const;

private:
    // Á∂ÅÂ?ÁßëÊ?Ê®π‰?‰ª?    void BindTechTreeEvents();

    // Ëß??ÁßëÊ?Ê®π‰?‰ª?    void UnbindTechTreeEvents();

    // Ê∏ÖÁ??æÊ?Widget
    void ClearExistingWidgets();

    // ?µÂª∫È°ûÂà•Ê®ôÁ±§
    void CreateCategoryTabs();

    // ?µÂª∫Á≠âÁ??éÊøæ??    void CreateTierFilters();
};

/**
 * ÁßëÊ??îÁôº?¢ÊùøUIÁµÑ‰ª∂
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSResearchPanelWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSResearchPanelWidget();

    // Ë®≠ÁΩÆÁßëÊ?Ê®πÁ≥ªÁµ±Â???    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void SetTechTreeSystem(UMingGoRTSTechTree* TechTreeSystem);

    // È°ØÁ§∫?îÁôº?¢Êùø
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void ShowResearchPanel();

    // ?±Ë??îÁôº?¢Êùø
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void HideResearchPanel();

    // ?¥Êñ∞Ê¥ªÂ??îÁôº?óË°®
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void UpdateActiveResearchList();

    // ?´Â??îÁôº
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void PauseResearch(const FString& TechID);

    // ?ñÊ??îÁôº
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void CancelResearch(const FString& TechID);

    // Ë®≠ÁΩÆ?îÁôºÂª∫Á?
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void SetResearchBuilding(const FString& BuildingID);

protected:
    // UIÁµÑ‰ª∂ÂºïÁî®
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* ActiveResearchList;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TotalResearchSpeed;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* AvailableScientists;

    UPROPERTY(meta = (BindWidget))
    class UComboBoxString* ResearchBuildingSelector;

    // ÁßëÊ?Ê®πÁ≥ªÁµ±Â???    UPROPERTY(BlueprintReadOnly, Category = "Research Panel UI")
    UMingGoRTSTechTree* TechTreeSystem;

    // ?∂Â??∏‰∏≠?ÑÁ??ºÂª∫ÁØ?    UPROPERTY(BlueprintReadOnly, Category = "Research Panel UI")
    FString SelectedBuildingID;

    // ?ùÂ??ñUI
    virtual void NativeConstruct() override;

    // ?µÂª∫?îÁôº?ÖÁõÆWidget
    UFUNCTION(BlueprintImplementableEvent, Category = "Research Panel UI")
    UUserWidget* CreateResearchItemWidget(const FTechResearchProgress& ResearchProgress);

    // ?ïÁ??îÁôº?≤Â∫¶?¥Êñ∞
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void OnResearchProgressUpdated(const FString& TechID, float Progress, float TimeRemaining);

    // ?¥Êñ∞Áµ±Ë?‰ø°ÊÅØ
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void UpdateStatistics();

private:
    // Á∂ÅÂ?‰∫ã‰ª∂
    void BindEvents();

    // Ëß??‰∫ã‰ª∂
    void UnbindEvents();

    // ?¥Êñ∞?îÁôºÂª∫Á??óË°®
    void UpdateResearchBuildingList();
};

/**
 * ÁßëÊ?Ë©≥Ê??¢ÊùøUIÁµÑ‰ª∂
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSTechDetailsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSTechDetailsWidget();

    // È°ØÁ§∫ÁßëÊ?Ë©≥Ê?
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void ShowTechDetails(const FTechNode& TechNode);

    // ?±Ë?ÁßëÊ?Ë©≥Ê?
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void HideTechDetails();

    // Ë®≠ÁΩÆÁßëÊ?Ê®πÁ≥ªÁµ±Â???    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void SetTechTreeSystem(UMingGoRTSTechTree* TechTreeSystem);

protected:
    // UIÁµÑ‰ª∂ÂºïÁî®
    UPROPERTY(meta = (BindWidget))
    class UImage* TechIcon;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TechName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TechDescription;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* HistoricalContext;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* EffectsList;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* PrerequisitesList;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* CostList;

    UPROPERTY(meta = (BindWidget))
    class UButton* StartResearchButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* CloseButton;

    // ?∂Â?È°ØÁ§∫?ÑÁ??Ä
    UPROPERTY(BlueprintReadOnly, Category = "Tech Details UI")
    FTechNode CurrentTechNode;

    // ÁßëÊ?Ê®πÁ≥ªÁµ±Â???    UPROPERTY(BlueprintReadOnly, Category = "Tech Details UI")
    UMingGoRTSTechTree* TechTreeSystem;

    // ?ùÂ??ñUI
    virtual void NativeConstruct() override;

    // ?¥Êñ∞UIÈ°ØÁ§∫
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void UpdateDisplay();

    // ?ïÁ??ãÂ??îÁôº
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void OnStartResearchClicked();

    // ?ïÁ??úÈ?
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void OnCloseClicked();

private:
    // ?µÂª∫?àÊ??óË°®??    UUserWidget* CreateEffectItem(const FTechEffect& Effect);

    // ?µÂª∫?çÁΩÆÊ¢ù‰ª∂?óË°®??    UUserWidget* CreatePrerequisiteItem(const FString& TechID);

    // ?µÂª∫?êÊú¨?óË°®??    UUserWidget* CreateCostItem(EResourceType ResourceType, float Cost);
};
