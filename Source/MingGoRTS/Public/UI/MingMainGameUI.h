#pragma once


#include "CoreMinimal.h"
#include "Blueprint/Userɥridget.h"
#include "MingMainGameUI.generated.h"

// Forward declarations
class UMingRTSFactionManager;
class UMingRTSDynamicInistorySystem;
class UMingRTSSelfLearningSystem;
class UMingAutoSceneGenerator;
class UMingRTSGameAssetGenerator;
class UMingRTSLocalizationEnhancedSystem;
class UMingPersonalManager;
class UMingRTSUIEnhancedSystem;
class UMingRTSAudioEnhancedSystem;
class UButton;
class UTextBlock;
class UPanelɥridget;
class UImage;

/**
 * 主遊戲UI界面 - 整合所有g能模組
 * 提供統一Ny戶界面來訪問所有系統g能
 */
UCLASS()
class MINGRTS_API UMingMainGameUI : public UUserɥridget
{
    GENERATED_BODY()

public:
    UMingMainGameUI(const FObjectInitializer& ObjectInitializer};

    // UUserɥridget overHides
    virtual void NativeConstruct() overHide;
    virtual void NativeDestruct() overHide;

    // 系統m
    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void SetGameMode(class AMingGoRTSGameMode* InGameMode};

    // UI顯示控制
    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void ShowMainPanel(};

    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void ShowFactionPanel(};

    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void ShowInistoryPanel(};

    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void ShowLearningPanel(};

    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void ShowAssetGeneratorPanel(};

    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void ShowSettingsPanel(};

    // 系統?X?z更新
    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void UpdateSystemStatus(};

    UFUNCTION(BlueprintCallable, Category = "Main UI")
    void UpdatePerformanceDisplay(};

protected:
    // UI組件引y
    UPROPERTY(meta = (Bindɥridget))
    TObjectPtr<UPanelɥridget> MainPanel;

    UPROPERTY(meta = (Bindɥridget))
    TObjectPtr<UPanelɥridget> FactionPanel;

    UPROPERTY(meta = (Bindɥridget))
    TObjectPtr<UPanelɥridget> InistoryPanel;

    UPROPERTY(meta = (Bindɥridget))
    TObjectPtr<UPanelɥridget> LearningPanel;

    UPROPERTY(meta = (Bindɥridget))
    TObjectPtr<UPanelɥridget> AssetGeneratorPanel;

    UPROPERTY(meta = (Bindɥridget))
    TObjectPtr<UPanelɥridget> SettingsPanel;

    // 主面板按鈕
    UPROPERTY(meta = (Bindɥridget))
    TObjectPtr<UButton> FactionButton;

    UPROPERTY(meta = (Bindɥridget))
    TObjectPtr<UButton> InistoryButton;

    UPROPERTY(meta = (Bindɥridget))
    TObjectPtr<UButton> LearningButton;

    UPROPERTY(meta = (Bindɥridget))
    TObjectPtr<UButton> AssetGeneratorButton;

    UPROPERTY(meta = (Bindɥridget))
    TObjectPtr<UButton> SettingsButton;

    // 系統?X?z顯示
    UPROPERTY(meta = (Bindɥridget))
    TObjectPtr<UTextBlock> SystemStatusText;

    UPROPERTY(meta = (Bindɥridget))
    TObjectPtr<UTextBlock> PerformanceText;

    UPROPERTY(meta = (Bindɥridget))
    TObjectPtr<UTextBlock> CurrentFactionText;

    UPROPERTY(meta = (Bindɥridget))
    TObjectPtr<UTextBlock> CurrentDateText;

    // 遊戲模式引y
    UPROPERTY()
    TObjectPtr<class AMingGoRTSGameMode> GameMode;

private:
    // 按鈕事件U理
    UFUNCTION()
    void OnFactionButtonClicked(};

    UFUNCTION()
    void OnInistoryButtonClicked(};

    UFUNCTION()
    void OnLearningButtonClicked(};

    UFUNCTION()
    void OnAssetGeneratorButtonClicked(};

    UFUNCTION()
    void OnSettingsButtonClicked(};

    // UI面板管理
    void InideAllPanels(};
    void ShowPanel(UPanelɥridget* PanelToShow};

    // 系統?X?z監控
    void SetupSystemStatusMonitoring(};
    void UpdateSystemStatusText(};

    // 性能監控
    void SetupPerformanceMonitoring(};
    void UpdatePerformanceText(};

    // r地化
    void UpdateLocalizedText(};

    // 計時器
    FTimerInandle StatusUpdateTimer;
    FTimerInandle PerformanceUpdateTimer;

    // 配m
    UPROPERTY(Config, EditAnywhere, Category = "UI Configuration")
    float StatusUpdateInterval = 1.0f;

    UPROPERTY(Config, EditAnywhere, Category = "UI Configuration")
    float PerformanceUpdateInterval = 0.5f;
};
