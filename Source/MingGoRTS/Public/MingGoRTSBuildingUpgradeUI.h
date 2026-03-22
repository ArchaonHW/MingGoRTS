#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Widget.h"
#include "MingGoRTSBuildingUpgradeSystem.h"
#include "MingGoRTSBuildingUpgradeUI.generated.h"

/**
 * 建築升級路徑UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSUpgradePathWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSUpgradePathWidget();

    // 設置升級路徑數據
    UFUNCTION(BlueprintCallable, Category = "Upgrade Path UI")
    void SetUpgradePath(const FBuildingUpgradePath& UpgradePath);

    // 設置當前等級
    UFUNCTION(BlueprintCallable, Category = "Upgrade Path UI")
    void SetCurrentLevel(int32 CurrentLevel);

    // 設置目標等級
    UFUNCTION(BlueprintCallable, Category = "Upgrade Path UI")
    void SetTargetLevel(int32 TargetLevel);

    // 設置是否可升級
    UFUNCTION(BlueprintCallable, Category = "Upgrade Path UI")
    void SetCanUpgrade(bool bCanUpgrade);

    // 設置是否被選中
    UFUNCTION(BlueprintCallable, Category = "Upgrade Path UI")
    void SetIsSelected(bool bSelected);

    // 更新升級進度
    UFUNCTION(BlueprintCallable, Category = "Upgrade Path UI")
    void UpdateUpgradeProgress(float Progress, float TimeRemaining);

    // 獲取升級路徑ID
    UFUNCTION(BlueprintPure, Category = "Upgrade Path UI")
    FString GetUpgradePathID() const;

    // 獲取是否被選中
    UFUNCTION(BlueprintPure, Category = "Upgrade Path UI")
    bool IsSelected() const;

protected:
    // UI組件引用
    UPROPERTY(meta = (BindWidget))
    class UImage* PathIcon;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PathName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PathDescription;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CurrentLevelText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MaxLevelText;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* LevelProgressBar;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* EffectsList;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* RequirementsList;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* CostList;

    UPROPERTY(meta = (BindWidget))
    class UButton* UpgradeButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* InstantUpgradeButton;

    UPROPERTY(meta = (BindWidget))
    class UBorder* PathBorder;

    // 當前升級路徑
    UPROPERTY(BlueprintReadOnly, Category = "Upgrade Path UI")
    FBuildingUpgradePath CurrentUpgradePath;

    // 當前等級
    UPROPERTY(BlueprintReadOnly, Category = "Upgrade Path UI")
    int32 CurrentLevel;

    // 目標等級
    UPROPERTY(BlueprintReadOnly, Category = "Upgrade Path UI")
    int32 TargetLevel;

    // 是否可升級
    UPROPERTY(BlueprintReadOnly, Category = "Upgrade Path UI")
    bool bCanUpgrade;

    // 是否被選中
    UPROPERTY(BlueprintReadOnly, Category = "Upgrade Path UI")
    bool bIsSelected;

    // 初始化UI
    virtual void NativeConstruct() override;

    // 更新UI顯示
    UFUNCTION(BlueprintImplementableEvent, Category = "Upgrade Path UI")
    void OnUpdateUI();

    // 處理升級按鈕
    UFUNCTION(BlueprintCallable, Category = "Upgrade Path UI")
    void OnUpgradeClicked();

    // 處理立即升級按鈕
    UFUNCTION(BlueprintCallable, Category = "Upgrade Path UI")
    void OnInstantUpgradeClicked();

    // 創建效果列表項
    UUserWidget* CreateEffectItem(const FBuildingUpgradeEffect& Effect);

    // 創建需求列表項
    UUserWidget* CreateRequirementItem(const FBuildingUpgradeRequirement& Requirement);

    // 創建成本列表項
    UUserWidget* CreateCostItem(EResourceType ResourceType, float Cost);

    // 格式化效果顯示
    FString FormatEffectDisplay(const FBuildingUpgradeEffect& Effect) const;

    // 格式化需求顯示
    FString FormatRequirementDisplay(const FBuildingUpgradeRequirement& Requirement) const;

    // 格式化成本顯示
    FString FormatCostDisplay(EResourceType ResourceType, float Cost) const;

private:
    // 綁定事件
    void BindEvents();

    // 解綁事件
    void UnbindEvents();
};

/**
 * 建築升級進度UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSUpgradeProgressWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSUpgradeProgressWidget();

    // 設置升級進度數據
    UFUNCTION(BlueprintCallable, Category = "Upgrade Progress UI")
    void SetUpgradeProgress(const FBuildingUpgradeProgress& Progress);

    // 更新進度
    UFUNCTION(BlueprintCallable, Category = "Upgrade Progress UI")
    void UpdateProgress(float Progress, float TimeRemaining);

    // 設置建築名稱
    UFUNCTION(BlueprintCallable, Category = "Upgrade Progress UI")
    void SetBuildingName(const FString& BuildingName);

    // 設置升級路徑名稱
    UFUNCTION(BlueprintCallable, Category = "Upgrade Progress UI")
    void SetUpgradePathName(const FString& PathName);

    // 獲取建築ID
    UFUNCTION(BlueprintPure, Category = "Upgrade Progress UI")
    FString GetBuildingID() const;

protected:
    // UI組件引用
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BuildingNameText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* UpgradePathNameText;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* UpgradeProgressBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ProgressText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TimeRemainingText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* StatusText;

    UPROPERTY(meta = (BindWidget))
    class UButton* PauseButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* CancelButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* InstantUpgradeButton;

    UPROPERTY(meta = (BindWidget))
    class UBorder* ProgressBorder;

    // 當前升級進度
    UPROPERTY(BlueprintReadOnly, Category = "Upgrade Progress UI")
    FBuildingUpgradeProgress CurrentProgress;

    // 建築名稱
    UPROPERTY(BlueprintReadOnly, Category = "Upgrade Progress UI")
    FString BuildingName;

    // 升級路徑名稱
    UPROPERTY(BlueprintReadOnly, Category = "Upgrade Progress UI")
    FString UpgradePathName;

    // 初始化UI
    virtual void NativeConstruct() override;

    // 更新UI顯示
    UFUNCTION(BlueprintImplementableEvent, Category = "Upgrade Progress UI")
    void OnUpdateUI();

    // 處理暫停按鈕
    UFUNCTION(BlueprintCallable, Category = "Upgrade Progress UI")
    void OnPauseClicked();

    // 處理取消按鈕
    UFUNCTION(BlueprintCallable, Category = "Upgrade Progress UI")
    void OnCancelClicked();

    // 處理立即升級按鈕
    UFUNCTION(BlueprintCallable, Category = "Upgrade Progress UI")
    void OnInstantUpgradeClicked();

    // 獲取狀態顏色
    FLinearColor GetStatusColor(EBuildingUpgradeStatus Status) const;

    // 格式化時間顯示
    FString FormatTime(float TimeSeconds) const;

private:
    // 綁定事件
    void BindEvents();

    // 解綁事件
    void UnbindEvents();
};

/**
 * 建築升級管理主UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSBuildingUpgradeManagerWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSBuildingUpgradeManagerWidget();

    // 設置建築升級系統引用
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void SetBuildingUpgradeSystem(UMingGoRTSBuildingUpgradeSystem* UpgradeSystem);

    // 設置當前建築
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void SetCurrentBuilding(const FString& BuildingID);

    // 顯示指定類別的升級路徑
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void ShowUpgradePathsByType(EBuildingType BuildingType);

    // 顯示所有升級路徑
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void ShowAllUpgradePaths();

    // 選中升級路徑
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void SelectUpgradePath(const FString& UpgradePathID);

    // 清除選中狀態
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void ClearSelection();

    // 開始升級
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    bool StartSelectedUpgrade(int32 TargetLevel = -1);

    // 立即完成升級
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    bool InstantCompleteUpgrade();

    // 顯示升級統計
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void ShowUpgradeStatistics();

    // 設置過濾器
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void SetFilter(EBuildingType BuildingTypeFilter, EBuildingUpgradeStatus StatusFilter);

    // 清除過濾器
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void ClearFilter();

    // 獲取當前選中的建築
    UFUNCTION(BlueprintPure, Category = "Building Upgrade Manager UI")
    FString GetCurrentBuildingID() const;

    // 獲取當前選中的升級路徑
    UFUNCTION(BlueprintPure, Category = "Building Upgrade Manager UI")
    FString GetSelectedUpgradePathID() const;

protected:
    // UI組件引用
    UPROPERTY(meta = (BindWidget))
    class UScrollBox* UpgradePathsScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* UpgradePathsContainer;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* ActiveUpgradesScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* ActiveUpgradesContainer;

    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* BuildingTypeTabs;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* StatusFilters;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CurrentBuildingName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SelectedPathName;

    UPROPERTY(meta = (BindWidget))
    class UButton* StartUpgradeButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* InstantUpgradeButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* StatisticsButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TotalActiveUpgrades;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* UpgradeSpeedBonus;

    // 建築升級系統引用
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade Manager UI")
    UMingGoRTSBuildingUpgradeSystem* UpgradeSystem;

    // 當前建築ID
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade Manager UI")
    FString CurrentBuildingID;

    // 當前選中的升級路徑ID
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade Manager UI")
    FString SelectedUpgradePathID;

    // 當前過濾器
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade Manager UI")
    EBuildingType BuildingTypeFilter;

    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade Manager UI")
    EBuildingUpgradeStatus StatusFilter;

    // 升級路徑Widget映射
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade Manager UI")
    TMap<FString, UMingGoRTSUpgradePathWidget*> UpgradePathWidgets;

    // 活動升級Widget數組
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade Manager UI")
    TArray<UMingGoRTSUpgradeProgressWidget*> ActiveUpgradeWidgets;

    // 初始化UI
    virtual void NativeConstruct() override;

    // 創建升級路徑Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Building Upgrade Manager UI")
    UMingGoRTSUpgradePathWidget* CreateUpgradePathWidget(const FBuildingUpgradePath& UpgradePath);

    // 創建升級進度Widget
    UFUNCTION(BlueprintImplementableEvent, Category = "Building Upgrade Manager UI")
    UMingGoRTSUpgradeProgressWidget* CreateUpgradeProgressWidget(const FBuildingUpgradeProgress& Progress);

    // 處理升級路徑點擊
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void OnUpgradePathClicked(const FString& UpgradePathID);

    // 處理升級開始
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void OnUpgradeStarted(const FString& BuildingID, const FString& UpgradePathID);

    // 處理升級進度
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void OnUpgradeProgress(const FString& BuildingID, float Progress, float TimeRemaining);

    // 處理升級完成
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void OnUpgradeCompleted(const FString& BuildingID, const TArray<FBuildingUpgradeEffect>& Effects);

    // 處理升級失敗
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void OnUpgradeFailed(const FString& BuildingID, const FString& ErrorMessage);

    // 處理升級暫停
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void OnUpgradePaused(const FString& BuildingID, const FString& Reason);

    // 處理升級恢復
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void OnUpgradeResumed(const FString& BuildingID, const FString& Reason);

    // 處理升級路徑解鎖
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void OnUpgradePathUnlocked(const FString& UpgradePathID, EBuildingType BuildingType);

    // 更新升級路徑列表
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void UpdateUpgradePaths();

    // 更新活動升級列表
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade Manager UI")
    void UpdateActiveUpgrades();

    // 創建建築類別標籤
    void CreateBuildingTypeTabs();

    // 創建狀態過濾器
    void CreateStatusFilters();

    // 應用過濾器
    void ApplyFilters();

    // 更新統計信息
    void UpdateStatistics();

private:
    // 綁定升級系統事件
    void BindUpgradeSystemEvents();

    // 解綁升級系統事件
    void UnbindUpgradeSystemEvents();

    // 清理現有Widget
    void ClearExistingWidgets();

    // 獲取建築類別顏色
    FLinearColor GetBuildingTypeColor(EBuildingType BuildingType) const;
};

/**
 * 升級效果預覽UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSUpgradePreviewWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSUpgradePreviewWidget();

    // 設置預覽數據
    UFUNCTION(BlueprintCallable, Category = "Upgrade Preview UI")
    void SetPreviewData(const FString& BuildingID, const FString& UpgradePathID, int32 TargetLevel);

    // 顯示預覽
    UFUNCTION(BlueprintCallable, Category = "Upgrade Preview UI")
    void ShowPreview();

    // 隱藏預覽
    UFUNCTION(BlueprintCallable, Category = "Upgrade Preview UI")
    void HidePreview();

    // 設置比較模式
    UFUNCTION(BlueprintCallable, Category = "Upgrade Preview UI")
    void SetComparisonMode(bool bEnableComparison);

protected:
    // UI組件引用
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PreviewTitle;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* CurrentEffectsScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* PreviewEffectsScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* ComparisonScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* UpgradeCostText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* UpgradeTimeText;

    UPROPERTY(meta = (BindWidget))
    class UButton* ConfirmButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* CancelButton;

    UPROPERTY(meta = (BindWidget))
    class UCheckBox* ComparisonCheckBox;

    // 預覽數據
    UPROPERTY(BlueprintReadOnly, Category = "Upgrade Preview UI")
    FString BuildingID;

    UPROPERTY(BlueprintReadOnly, Category = "Upgrade Preview UI")
    FString UpgradePathID;

    UPROPERTY(BlueprintReadOnly, Category = "Upgrade Preview UI")
    int32 TargetLevel;

    // 是否啟用比較模式
    UPROPERTY(BlueprintReadOnly, Category = "Upgrade Preview UI")
    bool bComparisonMode;

    // 初始化UI
    virtual void NativeConstruct() override;

    // 創建效果項目
    UFUNCTION(BlueprintImplementableEvent, Category = "Upgrade Preview UI")
    UUserWidget* CreateEffectItem(const FBuildingUpgradeEffect& Effect);

    // 創建比較項目
    UFUNCTION(BlueprintImplementableEvent, Category = "Upgrade Preview UI")
    UUserWidget* CreateComparisonItem(const FString& EffectName, float CurrentValue, float PreviewValue);

    // 處理確認按鈕
    UFUNCTION(BlueprintCallable, Category = "Upgrade Preview UI")
    void OnConfirmClicked();

    // 處理取消按鈕
    UFUNCTION(BlueprintCallable, Category = "Upgrade Preview UI")
    void OnCancelClicked();

    // 處理比較模式變更
    UFUNCTION(BlueprintCallable, Category = "Upgrade Preview UI")
    void OnComparisonModeChanged(bool bEnableComparison);

    // 更新預覽顯示
    void UpdatePreviewDisplay();

    // 計算效果差異
    TArray<FBuildingUpgradeEffect> CalculateEffectDifferences() const;

private:
    // 綁定事件
    void BindEvents();

    // 解綁事件
    void UnbindEvents();
};

/**
 * 升級統計UI組件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSUpgradeStatsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSUpgradeStatsWidget();

    // 設置升級統計數據
    UFUNCTION(BlueprintCallable, Category = "Upgrade Stats UI")
    void SetUpgradeStatistics(const FBuildingUpgradeStatistics& Stats);

    // 顯示詳細統計
    UFUNCTION(BlueprintCallable, Category = "Upgrade Stats UI")
    void ShowDetailedStats();

    // 導出統計報告
    UFUNCTION(BlueprintCallable, Category = "Upgrade Stats UI")
    void ExportStatsReport();

protected:
    // UI組件引用
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* StatsContainer;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TotalUpgrades;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SuccessRate;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* AverageUpgradeTime;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* LastUpgradeTime;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* BuildingTypeStatsList;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* EffectTypeStatsList;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* ResourceCostStatsList;

    UPROPERTY(meta = (BindWidget))
    class UButton* ExportButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* CloseButton;

    // 當前統計
    UPROPERTY(BlueprintReadOnly, Category = "Upgrade Stats UI")
    FBuildingUpgradeStatistics CurrentStats;

    // 初始化UI
    virtual void NativeConstruct() override;

    // 創建統計項目
    UFUNCTION(BlueprintImplementableEvent, Category = "Upgrade Stats UI")
    UUserWidget* CreateStatItem(const FString& Label, const FString& Value);

    // 創建建築類型統計項目
    UFUNCTION(BlueprintImplementableEvent, Category = "Upgrade Stats UI")
    UUserWidget* CreateBuildingTypeStatItem(EBuildingType BuildingType, int32 Count);

    // 創建效果類型統計項目
    UFUNCTION(BlueprintImplementableEvent, Category = "Upgrade Stats UI")
    UUserWidget* CreateEffectTypeStatItem(EBuildingUpgradeEffectType EffectType, int32 Count);

    // 創建資源成本統計項目
    UFUNCTION(BlueprintImplementableEvent, Category = "Upgrade Stats UI")
    UUserWidget* CreateResourceCostStatItem(EResourceType ResourceType, float Cost);

    // 格式化統計數據
    FString FormatStatValue(float Value) const;

    // 計算成功率
    float CalculateSuccessRate() const;

    // 計算平均升級時間
    float CalculateAverageUpgradeTime() const;

    // 處理導出按鈕
    UFUNCTION(BlueprintCallable, Category = "Upgrade Stats UI")
    void OnExportClicked();

    // 處理關閉按鈕
    UFUNCTION(BlueprintCallable, Category = "Upgrade Stats UI")
    void OnCloseClicked();

private:
    // 綁定事件
    void BindEvents();

    // 解綁事件
    void UnbindEvents();
};
