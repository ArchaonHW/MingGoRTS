#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Widget.h"
#include "MingGoRTSTechTree.h"
#include "MingGoRTSTechTreeUI.generated.h"

/**
 * 科技樹節點UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSTechNodeWidget : public UUserWidget
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

    // 設置是否被選中
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void SetIsSelected(bool bSelected);

    // 設置節點位置
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void SetNodePosition(const FVector2D& Position);

    // 獲取科技節點ID
    UFUNCTION(BlueprintPure, Category = "Tech Node UI")
    FString GetTechID() const;

    // 獲取是否被選中
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

    // 當前科技節點數據
    UPROPERTY(BlueprintReadOnly, Category = "Tech Node UI")
    FTechNode CurrentTechNode;

    // 當前狀態
    UPROPERTY(BlueprintReadOnly, Category = "Tech Node UI")
    ETechStatus CurrentStatus;

    // 是否被選中
    UPROPERTY(BlueprintReadOnly, Category = "Tech Node UI")
    bool bIsSelected;

    // 節點位置
    UPROPERTY(BlueprintReadOnly, Category = "Tech Node UI")
    FVector2D NodePosition;

    // 初始化UI
    virtual void NativeConstruct() override;

    // 更新UI顯示
    UFUNCTION(BlueprintImplementableEvent, Category = "Tech Node UI")
    void OnUpdateUI();

    // 處理點擊事件
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void OnNodeClicked();

    // 處理懸停事件
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void OnNodeHovered();

    // 處理取消懸停事件
    UFUNCTION(BlueprintCallable, Category = "Tech Node UI")
    void OnNodeUnhovered();

    // 獲取狀態顏色
    FLinearColor GetStatusColor(ETechStatus Status) const;

    // 獲取類別顏色
    FLinearColor GetCategoryColor(ETechCategory Category) const;

private:
    // 綁定事件
    void BindEvents();

    // 解綁事件
    void UnbindEvents();
};

/**
 * 科技樹連接線UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSTechConnectionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSTechConnectionWidget();

    // 設置連接的兩個節點
    UFUNCTION(BlueprintCallable, Category = "Tech Connection UI")
    void SetConnection(const FString& FromTechID, const FString& ToTechID);

    // 設置連接狀態
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

    // 目標科技ID
    UPROPERTY(BlueprintReadOnly, Category = "Tech Connection UI")
    FString ToTechID;

    // 連接是否激活
    UPROPERTY(BlueprintReadOnly, Category = "Tech Connection UI")
    bool bIsActive;

    // 初始化UI
    virtual void NativeConstruct() override;

private:
    // 計算連接線路徑
    void CalculateConnectionPath();
};

/**
 * 科技樹主UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSTechTreeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSTechTreeWidget();

    // 設置科技樹系統引用
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void SetTechTreeSystem(UMingGoRTSTechTree* TechTreeSystem);

    // 顯示指定類別的科技樹
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ShowTechCategory(ETechCategory Category);

    // 顯示所有科技樹
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ShowAllCategories();

    // 選中科技節點
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void SelectTechNode(const FString& TechID);

    // 清除選中狀態
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ClearSelection();

    // 開始研發選中的科技
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

    // 獲取當前選中的科技
    UFUNCTION(BlueprintPure, Category = "Tech Tree UI")
    FString GetSelectedTechID() const;

    // 獲取當前顯示的類別
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

    // 科技樹系統引用
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    UMingGoRTSTechTree* TechTreeSystem;

    // 當前選中的科技ID
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    FString SelectedTechID;

    // 當前顯示的類別
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    ETechCategory CurrentCategory;

    // 當前過濾器
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    ETechCategory CategoryFilter;

    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    ETechTier TierFilter;

    // 科技節點Widget映射
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    TMap<FString, UMingGoRTSTechNodeWidget*> TechNodeWidgets;

    // 科技連接Widget數組
    UPROPERTY(BlueprintReadOnly, Category = "Tech Tree UI")
    TArray<UMingGoRTSTechConnectionWidget*> ConnectionWidgets;

    // 初始化UI
    virtual void NativeConstruct() override;

    // 創建科技節點Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Tech Tree UI")
    UMingGoRTSTechNodeWidget* CreateTechNodeWidget(const FTechNode& TechNode);

    // 創建連接Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Tech Tree UI")
    UMingGoRTSTechConnectionWidget* CreateConnectionWidget(const FString& FromTechID, const FString& ToTechID);

    // 處理科技節點點擊
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void OnTechNodeClicked(const FString& TechID);

    // 處理科技研發開始
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void OnTechResearchStarted(const FString& TechID, const FString& BuildingID);

    // 處理科技研發完成
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void OnTechResearchCompleted(const FString& TechID, const TArray<FTechEffect>& Effects);

    // 處理科技解鎖
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void OnTechUnlocked(const FString& TechID, ETechCategory Category);

    // 更新選中科技信息
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void UpdateSelectedTechInfo();

    // 佈局科技節點
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void LayoutTechNodes();

    // 創建科技連接線
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void CreateTechConnections();

    // 應用過濾器
    UFUNCTION(BlueprintCallable, Category = "Tech Tree UI")
    void ApplyFilters();

    // 獲取節點佈局位置
    FVector2D GetNodeLayoutPosition(const FString& TechID, int32 Row, int32 Column) const;

private:
    // 綁定科技樹事件
    void BindTechTreeEvents();

    // 解綁科技樹事件
    void UnbindTechTreeEvents();

    // 清理現有Widget
    void ClearExistingWidgets();

    // 創建類別標籤
    void CreateCategoryTabs();

    // 創建等級過濾器
    void CreateTierFilters();
};

/**
 * 科技研發面板UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSResearchPanelWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSResearchPanelWidget();

    // 設置科技樹系統引用
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void SetTechTreeSystem(UMingGoRTSTechTree* TechTreeSystem);

    // 顯示研發面板
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void ShowResearchPanel();

    // 隱藏研發面板
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void HideResearchPanel();

    // 更新活動研發列表
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void UpdateActiveResearchList();

    // 暫停研發
    UFUNCTION(BlueprintCallable, Category = "Research Panel UI")
    void PauseResearch(const FString& TechID);

    // 取消研發
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

    // 科技樹系統引用
    UPROPERTY(BlueprintReadOnly, Category = "Research Panel UI")
    UMingGoRTSTechTree* TechTreeSystem;

    // 當前選中的研發建築
    UPROPERTY(BlueprintReadOnly, Category = "Research Panel UI")
    FString SelectedBuildingID;

    // 初始化UI
    virtual void NativeConstruct() override;

    // 創建研發項目Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Research Panel UI")
    UUserWidget* CreateResearchItemWidget(const FTechResearchProgress& ResearchProgress);

    // 處理研發進度更新
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
 * 科技詳情面板UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSTechDetailsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSTechDetailsWidget();

    // 顯示科技詳情
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void ShowTechDetails(const FTechNode& TechNode);

    // 隱藏科技詳情
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void HideTechDetails();

    // 設置科技樹系統引用
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

    // 當前顯示的科技
    UPROPERTY(BlueprintReadOnly, Category = "Tech Details UI")
    FTechNode CurrentTechNode;

    // 科技樹系統引用
    UPROPERTY(BlueprintReadOnly, Category = "Tech Details UI")
    UMingGoRTSTechTree* TechTreeSystem;

    // 初始化UI
    virtual void NativeConstruct() override;

    // 更新UI顯示
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void UpdateDisplay();

    // 處理開始研發
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void OnStartResearchClicked();

    // 處理關閉
    UFUNCTION(BlueprintCallable, Category = "Tech Details UI")
    void OnCloseClicked();

private:
    // 創建效果列表項
    UUserWidget* CreateEffectItem(const FTechEffect& Effect);

    // 創建前置條件列表項
    UUserWidget* CreatePrerequisiteItem(const FString& TechID);

    // 創建成本列表項
    UUserWidget* CreateCostItem(EResourceType ResourceType, float Cost);
};
