#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Widget.h"
#include "MingGoRTSTechTree.h"
#include "MingGoRTSTechTreeUI.generated.h"

/**
 * 科技樹技節點UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSTechNodeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSTechNodeWidget();

    // 設置科技節點數據
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void SetTechNode(const FTechNode& TechNode);

    // 更新節點狀態
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void UpdateNodeStatus(ETechStatus NewStatus);

    // 更新研發進度
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void UpdateResearchProgress(float Progress);

    // 设置是否被选中
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void SetIsSelected(bool bSelected);

    // 设置节点位置
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void SetNodePosition(const FVector2D& Position);

    // 获取科技节点ID
    UFUNCTION(BlueprintPure, Category = "Tech Node UI")
    FString GetTechID() const;

    // 是否被选中
    UFUNCTION(BlueprintPure, Category = "Tech Node UI")
    // 拆除中X?否被選進
    UFUNCTION(BlueprintPure, Category = "Tech Node UI")
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

    // 拆除中?科技節點數X
    UPROPERTY(BlueprintReadOnly, Category = "Tech Node UI")
    FTechNode CurrentTechNode;

    // 拆除中X數據
    UPROPERTY(BlueprintReadOnly, Category = "Tech Node UI")
    ETechStatus CurrentStatus;

    // 是否被選中
    UPROPERTY(BlueprintReadOnly, Category = "Tech Node UI")
    bool bIsSelected;

    // 節點位置
    UPROPERTY(BlueprintReadOnly, Category = "Tech Node UI")
    FVector2D NodePosition;

    // 拆除中X?UI
    virtual void NativeConstruct() override;

    // 更新UI顯示
    UFUNCTION(BlueprintImplementableEvent, Category = "Tech Node UI")
    void OnUpdateUI();

    // 拆除中?點進事件
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void OnNodeClicked();

    // 拆除中X進?事件
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void OnNodeHovered();

    // 拆除中X?數據進?事件
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void OnNodeUnhovered();

    // 拆除中X進?數據
    FLinearColor GetStatusColor(ETechStatus Status) const;

    // 拆除中?類別顏色
    FLinearColor GetCategoryColor(ETechCategory Category) const;

private:
    // 綁定事件
    void BindEvents();

    // 解綁事件
    void UnbindEvents();
};

/**
 * 科技樹進技線UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSTechConnectionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSTechConnectionWidget();

    // 設置連接線兩端科技
    UFUNCTION(BlueprintCallable, Category = "Tech Connection UI")
    void SetConnection(const FString& FromTechID, const FString& ToTechID);

    // 設置連接線狀態
    UFUNCTION(BlueprintCallable, Category = "Tech Connection UI")
    void SetConnectionStatus(bool bIsActive);

    // 設置連接線顏色
    UFUNCTION(BlueprintCallable, Category = "Tech Connection UI")
    void SetConnectionColor(const FLinearColor& Color);

    // 更新連接線位置
    UFUNCTION(BlueprintCallable, Category = "Tech Connection UI")
    void UpdateConnectionPosition(const FVector2D& FromPosition, const FVector2D& ToPosition);

protected:
    // UI組件引用
    UPROPERTY(meta = (BindWidget))
    class UImage* ConnectionLine;

    // 源科技ID
    UPROPERTY(BlueprintReadOnly, Category = "Tech Connection UI")
    FString FromTechID;

    // 拆除中?科進ID
    UPROPERTY(BlueprintReadOnly, Category = "Tech Connection UI")
    FString ToTechID;

    // 政府進進否激進
    UPROPERTY(BlueprintReadOnly, Category = "Tech Connection UI")
    bool bIsActive;

    // 拆除中X?UI
    virtual void NativeConstruct() override;

private:
    // 計數據技線路進
    void CalculateConnectionPath();
};

/**
 * 科技樹主UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSTechTreeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSTechTreeWidget();

    // 設置科技樹系統數據
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void SetTechTreeSystem(UMingGoRTSTechTree* TechTreeSystem);

    // 顯示特定類別科技項目
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ShowTechCategory(ETechCategory Category);

    // 顯示所有科技項目
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ShowAllCategories();

    // 選中科技節點
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void SelectTechNode(const FString& TechID);

    // 清除選中狀態
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ClearSelection();

    // 拆除中X?發進中進數據?
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    bool StartResearchSelectedTech();

    // 更新科技樹顯示
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void UpdateTechTreeDisplay();

    // 設置過濾器
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void SetFilter(ETechCategory CategoryFilter, ETechTier TierFilter);

    // 清除過濾器
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ClearFilter();

    // 拆除中X?數據?中進數據?
    UFUNCTION(BlueprintPure, Category = "Tech Tree UI")
    FString GetSelectedTechID() const;

    // 拆除中X進?顯示進數據
    UFUNCTION(BlueprintPure, Category = "Tech Tree UI")
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

    // 科技樹系統數據
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    UMingGoRTSTechTree* TechTreeSystem;

    // 拆除中X?中進數據?ID
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    FString SelectedTechID;

    // 拆除中?顯示進數據
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    ETechCategory CurrentCategory;

    // 拆除中X?濾X
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    ETechCategory CategoryFilter;

    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    ETechTier TierFilter;

    // 科技節點Widget緩存
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    TMap<FString, UMingGoRTSTechNodeWidget*> TechNodeWidgets;

    // 科技連接Widget緩存
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    TArray<UMingGoRTSTechConnectionWidget*> ConnectionWidgets;

    // 拆除中X?UI
    virtual void NativeConstruct() override;

    // 創建科技節點Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Tech Tree UI")
    UMingGoRTSTechNodeWidget* CreateTechNodeWidget(const FTechNode& TechNode);

    // 創建連接Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Tech Tree UI")
    UMingGoRTSTechConnectionWidget* CreateConnectionWidget(const FString& FromTechID, const FString& ToTechID);

    // 拆除中?科技節點數據
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void OnTechNodeClicked(const FString& TechID);

    // 拆除中?科數據?發進進
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void OnTechResearchStarted(const FString& TechID, const FString& BuildingID);

    // 拆除中?科數據?發完進
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void OnTechResearchCompleted(const FString& TechID, const TArray<FTechEffect>& Effects);

    // 拆除中?科進數據
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void OnTechUnlocked(const FString& TechID, ETechCategory Category);

    // 更新選中科技信息
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void UpdateSelectedTechInfo();

    // 佈局科技節點
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void LayoutTechNodes();

    // 創建科技連接
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void CreateTechConnections();

    // 應用過濾器
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ApplyFilters();

    // 拆除中?節點進局位置
    FVector2D GetNodeLayoutPosition(const FString& TechID, int32 Row, int32 Column) const;

private:
    // 綁定科技樹事件
    void BindTechTreeEvents();

    // 解綁科技樹事件
    void UnbindTechTreeEvents();

    // 清除現有Widget
    void ClearExistingWidgets();

    // 創建類別標籤
    void CreateCategoryTabs();

    // 創建等級過濾器
    void CreateTierFilters();
};

/**
 * 科數據?發進板UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSResearchPanelWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSResearchPanelWidget();

    // 設置科技樹系統數據
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void SetTechTreeSystem(UMingGoRTSTechTree* TechTreeSystem);

    // 顯示研發面板
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void ShowResearchPanel();

    // 拆除中X?發進板
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void HideResearchPanel();

    // 更新活躍研發列表
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void UpdateActiveResearchList();

    // 拆除中X?發
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void PauseResearch(const FString& TechID);

    // 拆除中X?發
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void CancelResearch(const FString& TechID);

    // 設置研發建築
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void SetResearchBuilding(const FString& BuildingID);

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

    // 科技樹系統數據
    UPROPERTY(BlueprintReadOnly, Category = "Research Panel UI")
    UMingGoRTSTechTree* TechTreeSystem;

    // 拆除中X?中進數據?建進
    UPROPERTY(BlueprintReadOnly, Category = "Research Panel UI")
    FString SelectedBuildingID;

    // 拆除中X?UI
    virtual void NativeConstruct() override;

    // 創建研發項目Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Research Panel UI")
    UUserWidget* CreateResearchItemWidget(const FTechResearchProgress& ResearchProgress);

    // 拆除中X?發進度進新
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void OnResearchProgressUpdated(const FString& TechID, float Progress, float TimeRemaining);

    // 更新統計信息
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void UpdateStatistics();

private:
    // 綁定事件
    void BindEvents();

    // 解綁事件
    void UnbindEvents();

    // 更新研發建築列表
    void UpdateResearchBuildingList();
};

/**
 * 科技詳情數據?板UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSTechDetailsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSTechDetailsWidget();

    // 顯示科技詳情
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void ShowTechDetails(const FTechNode& TechNode);

    // 拆除中?科技詳情進
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void HideTechDetails();

    // 設置科技樹系統數據
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void SetTechTreeSystem(UMingGoRTSTechTree* TechTreeSystem);

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

    // 拆除中?顯示進數據?
    UPROPERTY(BlueprintReadOnly, Category = "Tech Details UI")
    FTechNode CurrentTechNode;

    // 科技樹系統數據
    UPROPERTY(BlueprintReadOnly, Category = "Tech Details UI")
    UMingGoRTSTechTree* TechTreeSystem;

    // 拆除中X?UI
    virtual void NativeConstruct() override;

    // 更新UI顯示
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void UpdateDisplay();

    // 拆除中X?數據?發
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void OnStartResearchClicked();

    // 拆除中X進?
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void OnCloseClicked();

private:
    // 創建效果列表項
    UUserWidget* CreateEffectItem(const FTechEffect& Effect);

    // 創建前置條件項
    UUserWidget* CreatePrerequisiteItem(const FString& TechID);

    // 創建成本項
    UUserWidget* CreateCostItem(EResourceType ResourceType, float Cost);
};

