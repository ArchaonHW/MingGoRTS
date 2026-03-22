#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Widget.h"
#include "MingGoRTSTechTree.h"
#include "MingGoRTSTechTreeUI.generated.h"

/**
 * 科??樹??點UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSTechNodeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSTechNodeWidget();

    // 設置科??節點數X    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void SetTechNode(const FTechNode& TechNode};

    // ??新節點?X    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void UpdateNodeStatus(ETechStatus NewStatus};

    // ??新??發??度
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void UpdateResearchProgress(float Progress);

    // 设置是否被选中    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void SetIsSelected(bool bSelected);

    // 设置节点位置    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void SetNodePosition(const FVector2D& Position);

    // 获取科技节点ID
    UFUNCTION(BlueprintPure, Category = "Tech Node UI")
    FString GetTechID() const;

    // 是否被选中    UFUNCTION(BlueprintPure, Category = "Tech Node UI")
    // ???X?否被選??    UFUNCTION(BlueprintPure, Category = "Tech Node UI")
    bool IsSelected() const;

protected:
    // UI組件引用
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

    // ????科??節點數X    UPROPERTY(BlueprintReadOnly, Category = "Tech Node UI")
    FTechNode CurrentTechNode;

    // ???X?X    UPROPERTY(BlueprintReadOnly, Category = "Tech Node UI")
    ETechStatus CurrentStatus;

    // ??否被選??    UPROPERTY(BlueprintReadOnly, Category = "Tech Node UI")
    bool bIsSelected;

    // 節點????    UPROPERTY(BlueprintReadOnly, Category = "Tech Node UI")
    FVector2D NodePosition;

    // ???X?UI
    virtual void NativeConstruct() override;

    // ??新UI顯示
    UFUNCTION(BlueprintImplementableEvent, Category = "Tech Node UI")
    void OnUpdateUI();

    // ????點??事件
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void OnNodeClicked();

    // ???X???事件
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void OnNodeHovered();

    // ???X??X???事件
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void OnNodeUnhovered();

    // ???X????X    FLinearColor GetStatusColor(ETechStatus Status) const;

    // ????類別顏色
    FLinearColor GetCategoryColor(ETechCategory Category) const;

private:
    // 綁??事件
    void BindEvents();

    // ?X事件
    void UnbindEvents();
};

/**
 * 科??樹????線UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSTechConnectionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSTechConnectionWidget();

    // 設置X????兩??????    UFUNCTION(BlueprintCallable, Category = "Tech Connection UI")
    void SetConnection(const FString& FromTechID, const FString& ToTechID};

    // 設置X????X    UFUNCTION(BlueprintCallable, Category = "Tech Connection UI")
    void SetConnectionStatus(bool bIsActive};

    // 設置X??線?X    UFUNCTION(BlueprintCallable, Category = "Tech Connection UI")
    void SetConnectionColor(const FLinearColor& Color};

    // ??新X??線????    UFUNCTION(BlueprintCallable, Category = "Tech Connection UI")
    void UpdateConnectionPosition(const FVector2D& FromPosition, const FVector2D& ToPosition};

protected:
    // UI組件引用
    UPROPERTY(meta = (BindWidget))
    class UImage* ConnectionLine;

    // 源?X?ID
    UPROPERTY(BlueprintReadOnly, Category = "Tech Connection UI")
    FString FromTechID;

    // ????科??ID
    UPROPERTY(BlueprintReadOnly, Category = "Tech Connection UI")
    FString ToTechID;

    // X????否激??    UPROPERTY(BlueprintReadOnly, Category = "Tech Connection UI")
    bool bIsActive;

    // ???X?UI
    virtual void NativeConstruct() override;

private:
    // 計?X??線路??    void CalculateConnectionPath();
};

/**
 * 科??樹主UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSTechTreeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSTechTreeWidget();

    // 設置科??樹系統?X    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void SetTechTreeSystem(UMingGoRTSTechTree* TechTreeSystem};

    // 顯示????類別???X???    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ShowTechCategory(ETechCategory Category};

    // 顯示?????X???    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ShowAllCategories();

    // ??中科??節??    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void SelectTechNode(const FString& TechID};

    // 清除??中??X    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ClearSelection();

    // ???X?發??中???X?
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    bool StartResearchSelectedTech();

    // ??新科??樹顯??    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void UpdateTechTreeDisplay();

    // 設置??濾X    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void SetFilter(ETechCategory CategoryFilter, ETechTier TierFilter};

    // 清除??濾X    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ClearFilter();

    // ???X??X?中???X?
    UFUNCTION(BlueprintPure, Category = "Tech Tree UI")
    FString GetSelectedTechID() const;

    // ???X???顯示???X    UFUNCTION(BlueprintPure, Category = "Tech Tree UI")
    ETechCategory GetCurrentCategory() const;

protected:
    // UI組件引用
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

    // 科??樹系統?X    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    UMingGoRTSTechTree* TechTreeSystem;

    // ???X?中???X?ID
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    FString SelectedTechID;

    // ????顯示???X    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    ETechCategory CurrentCategory;

    // ???X?濾X    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    ETechCategory CategoryFilter;

    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    ETechTier TierFilter;

    // 科??節點Widget????
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    TMap<FString, UMingGoRTSTechNodeWidget*> TechNodeWidgets;

    // 科?X??Widget????
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    TArray<UMingGoRTSTechConnectionWidget*> ConnectionWidgets;

    // ???X?UI
    virtual void NativeConstruct() override;

    // ??建科??節點Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Tech Tree UI")
    UMingGoRTSTechNodeWidget* CreateTechNodeWidget(const FTechNode& TechNode};

    // ??建X??Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Tech Tree UI")
    UMingGoRTSTechConnectionWidget* CreateConnectionWidget(const FString& FromTechID, const FString& ToTechID};

    // ????科??節點?X    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void OnTechNodeClicked(const FString& TechID};

    // ????科?X?發????
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void OnTechResearchStarted(const FString& TechID, const FString& BuildingID};

    // ????科?X?發完??
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void OnTechResearchCompleted(const FString& TechID, const TArray<FTechEffect>& Effects};

    // ????科???X
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void OnTechUnlocked(const FString& TechID, ETechCategory Category};

    // ??新??中科??信息
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void UpdateSelectedTechInfo();

    // 佈??科??節??    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void LayoutTechNodes();

    // ??建科?X????    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void CreateTechConnections();

    // ??用??濾X    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ApplyFilters();

    // ????節點??局位置
    FVector2D GetNodeLayoutPosition(const FString& TechID, int32 Row, int32 Column) const;

private:
    // 綁??科??樹????    void BindTechTreeEvents();

    // ?X科??樹????    void UnbindTechTreeEvents();

    // 清?X???Widget
    void ClearExistingWidgets();

    // ??建類別標籤
    void CreateCategoryTabs();

    // ??建等?X?濾X    void CreateTierFilters();
};

/**
 * 科?X?發??板UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSResearchPanelWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSResearchPanelWidget();

    // 設置科??樹系統?X    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void SetTechTreeSystem(UMingGoRTSTechTree* TechTreeSystem};

    // 顯示??發??板
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void ShowResearchPanel();

    // ???X?發??板
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void HideResearchPanel();

    // ??新活?X?發??表
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void UpdateActiveResearchList();

    // ???X?發
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void PauseResearch(const FString& TechID};

    // ???X?發
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void CancelResearch(const FString& TechID};

    // 設置??發建??
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void SetResearchBuilding(const FString& BuildingID};

protected:
    // UI組件引用
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* ActiveResearchList;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TotalResearchSpeed;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* AvailableScientists;

    UPROPERTY(meta = (BindWidget))
    class UComboBoxString* ResearchBuildingSelector;

    // 科??樹系統?X    UPROPERTY(BlueprintReadOnly, Category = "Research Panel UI")
    UMingGoRTSTechTree* TechTreeSystem;

    // ???X?中???X?建??    UPROPERTY(BlueprintReadOnly, Category = "Research Panel UI")
    FString SelectedBuildingID;

    // ???X?UI
    virtual void NativeConstruct() override;

    // ??建??發??目Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Research Panel UI")
    UUserWidget* CreateResearchItemWidget(const FTechResearchProgress& ResearchProgress};

    // ???X?發??度??新
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void OnResearchProgressUpdated(const FString& TechID, float Progress, float TimeRemaining};

    // ??新統??信息
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void UpdateStatistics();

private:
    // 綁??事件
    void BindEvents();

    // ?X事件
    void UnbindEvents();

    // ??新??發建?X?表
    void UpdateResearchBuildingList();
};

/**
 * 科??詳?X?板UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSTechDetailsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSTechDetailsWidget();

    // 顯示科??詳??
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void ShowTechDetails(const FTechNode& TechNode};

    // ????科??詳??
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void HideTechDetails();

    // 設置科??樹系統?X    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void SetTechTreeSystem(UMingGoRTSTechTree* TechTreeSystem};

protected:
    // UI組件引用
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

    // ????顯示???X?
    UPROPERTY(BlueprintReadOnly, Category = "Tech Details UI")
    FTechNode CurrentTechNode;

    // 科??樹系統?X    UPROPERTY(BlueprintReadOnly, Category = "Tech Details UI")
    UMingGoRTSTechTree* TechTreeSystem;

    // ???X?UI
    virtual void NativeConstruct() override;

    // ??新UI顯示
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void UpdateDisplay();

    // ???X??X?發
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void OnStartResearchClicked();

    // ???X???
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void OnCloseClicked();

private:
    // ??建???X?表X    UUserWidget* CreateEffectItem(const FTechEffect& Effect);

    // CreatePrerequisiteItem
    UUserWidget* CreatePrerequisiteItem(const FString& TechID);

    // CreateCostItem
    UUserWidget* CreateCostItem(EResourceType ResourceType, float Cost);
};

