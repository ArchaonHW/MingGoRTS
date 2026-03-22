#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingMainGameUI.generated.h"

// Forward declarations
class UMingRTSFactionManager;
class UMingRTSDynamicHistorySystem;
class UMingRTSSelfLearningSystem;
class UMingAutoSceneGenerator;
class UMingRTSGameAssetGenerator;
class UMingRTSLocalizationEnhancedSystem;
class UMingPersonalManager;
class UMingRTSUIEnhancedSystem;
class UMingRTSAudioEnhancedSystem;
class UButton;
class UTextBlock;
class UPanelWidget;
class UImage;

/**
 * 主遊戲UI界面 - 整合所有功能模組
 * 提供統一的用戶界面來訪問所有系統功能
 */
UCLASS()
class MINGGORTS_API UMingMainGameUI : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingMainGameUI(const FObjectInitializer& ObjectInitializer);

    // UUserWidget overrides
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // 系統設置
    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void SetGameMode(class AMingGoRTSGameMode* InGameMode);

    // UI顯示控制
    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void ShowMainPanel();

    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void ShowFactionPanel();

    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void ShowHistoryPanel();

    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void ShowLearningPanel();

    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void ShowAssetGeneratorPanel();

    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void ShowSettingsPanel();

    // 系統狀態更新
    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void UpdateSystemStatus();

    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void UpdatePerformanceDisplay();

protected:
    // UI組件引用
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPanelWidget> MainPanel;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPanelWidget> FactionPanel;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPanelWidget> HistoryPanel;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPanelWidget> LearningPanel;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPanelWidget> AssetGeneratorPanel;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPanelWidget> SettingsPanel;

    // 主面板按鈕
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> FactionButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> HistoryButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> LearningButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> AssetGeneratorButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> SettingsButton;

    // 系統狀態顯示
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> SystemStatusText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> PerformanceText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> CurrentFactionText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> CurrentDateText;

    // 遊戲模式引用
    UPROPERTY()
    TObjectPtr<class AMingGoRTSGameMode> GameMode;

private:
    // 按鈕事件處理
    UFUNCTION()
    void OnFactionButtonClicked();

    UFUNCTION()
    void OnHistoryButtonClicked();

    UFUNCTION()
    void OnLearningButtonClicked();

    UFUNCTION()
    void OnAssetGeneratorButtonClicked();

    UFUNCTION()
    void OnSettingsButtonClicked();

    // UI面板管理
    void HideAllPanels();
    void ShowPanel(UPanelWidget* PanelToShow);

    // 系統狀態監控
    void SetupSystemStatusMonitoring();
    void UpdateSystemStatusText();

    // 性能監控
    void SetupPerformanceMonitoring();
    void UpdatePerformanceText();

    // 本地化
    void UpdateLocalizedText();

    // 計時器
    FTimerHandle StatusUpdateTimer;
    FTimerHandle PerformanceUpdateTimer;

    // 配置
    UPROPERTY(Config, EditAnywhere, Category = "UI Configuration")
    float StatusUpdateInterval = 1.0f;

    UPROPERTY(Config, EditAnywhere, Category = "UI Configuration")
    float PerformanceUpdateInterval = 0.5f;
};
