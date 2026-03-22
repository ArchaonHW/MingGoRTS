#include "UI/MingMainGameUI.h"
#include "MingGoRTSGameMode.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"

UMingMainGameUI::UMingMainGameUI(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    StatusUpdateInterval = 1.0f;
    PerformanceUpdateInterval = 0.5f;
}

void UMingMainGameUI::NativeConstruct()
{
    Super::NativeConstruct();

    // 綁定按鈕事件
    if (FactionButton)
    {
        FactionButton->OnClicked.AddDynamic(this, &UMingMainGameUI::OnFactionButtonClicked);
    }
    
    if (HistoryButton)
    {
        HistoryButton->OnClicked.AddDynamic(this, &UMingMainGameUI::OnHistoryButtonClicked);
    }
    
    if (LearningButton)
    {
        LearningButton->OnClicked.AddDynamic(this, &UMingMainGameUI::OnLearningButtonClicked);
    }
    
    if (AssetGeneratorButton)
    {
        AssetGeneratorButton->OnClicked.AddDynamic(this, &UMingMainGameUI::OnAssetGeneratorButtonClicked);
    }
    
    if (SettingsButton)
    {
        SettingsButton->OnClicked.AddDynamic(this, &UMingMainGameUI::OnSettingsButtonClicked);
    }

    // 設置監控系統
    SetupSystemStatusMonitoring();
    SetupPerformanceMonitoring();

    // 顯示主面板
    ShowMainPanel();

    // 更新本地化文本
    UpdateLocalizedText();
}

void UMingMainGameUI::NativeDestruct()
{
    // 清理計時器
    if (GetWorld() && GetWorld()->GetTimerManager().TimerExists(StatusUpdateTimer))
    {
        GetWorld()->GetTimerManager().ClearTimer(StatusUpdateTimer);
    }
    
    if (GetWorld() && GetWorld()->GetTimerManager().TimerExists(PerformanceUpdateTimer))
    {
        GetWorld()->GetTimerManager().ClearTimer(PerformanceUpdateTimer);
    }

    Super::NativeDestruct();
}

void UMingMainGameUI::SetGameMode(AMingGoRTSGameMode* InGameMode)
{
    GameMode = InGameMode;
    
    // 更新系統狀態顯示
    UpdateSystemStatus();
    UpdatePerformanceDisplay();
}

void UMingMainGameUI::ShowMainPanel()
{
    HideAllPanels();
    if (MainPanel)
    {
        MainPanel->SetVisibility(ESlateVisibility::Visible);
    }
}

void UMingMainGameUI::ShowFactionPanel()
{
    HideAllPanels();
    if (FactionPanel)
    {
        FactionPanel->SetVisibility(ESlateVisibility::Visible);
    }
}

void UMingMainGameUI::ShowHistoryPanel()
{
    HideAllPanels();
    if (HistoryPanel)
    {
        HistoryPanel->SetVisibility(ESlateVisibility::Visible);
    }
}

void UMingMainGameUI::ShowLearningPanel()
{
    HideAllPanels();
    if (LearningPanel)
    {
        LearningPanel->SetVisibility(ESlateVisibility::Visible);
    }
}

void UMingMainGameUI::ShowAssetGeneratorPanel()
{
    HideAllPanels();
    if (AssetGeneratorPanel)
    {
        AssetGeneratorPanel->SetVisibility(ESlateVisibility::Visible);
    }
}

void UMingMainGameUI::ShowSettingsPanel()
{
    HideAllPanels();
    if (SettingsPanel)
    {
        SettingsPanel->SetVisibility(ESlateVisibility::Visible);
    }
}

void UMingMainGameUI::UpdateSystemStatus()
{
    UpdateSystemStatusText();
}

void UMingMainGameUI::UpdatePerformanceDisplay()
{
    UpdatePerformanceText();
}

void UMingMainGameUI::HideAllPanels()
{
    if (MainPanel) MainPanel->SetVisibility(ESlateVisibility::Hidden);
    if (FactionPanel) FactionPanel->SetVisibility(ESlateVisibility::Hidden);
    if (HistoryPanel) HistoryPanel->SetVisibility(ESlateVisibility::Hidden);
    if (LearningPanel) LearningPanel->SetVisibility(ESlateVisibility::Hidden);
    if (AssetGeneratorPanel) AssetGeneratorPanel->SetVisibility(ESlateVisibility::Hidden);
    if (SettingsPanel) SettingsPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UMingMainGameUI::ShowPanel(UPanelWidget* PanelToShow)
{
    HideAllPanels();
    if (PanelToShow)
    {
        PanelToShow->SetVisibility(ESlateVisibility::Visible);
    }
}

void UMingMainGameUI::OnFactionButtonClicked()
{
    ShowFactionPanel();
    UE_LOG(LogTemp, Log, TEXT("Faction panel opened"));
}

void UMingMainGameUI::OnHistoryButtonClicked()
{
    ShowHistoryPanel();
    UE_LOG(LogTemp, Log, TEXT("History panel opened"));
}

void UMingMainGameUI::OnLearningButtonClicked()
{
    ShowLearningPanel();
    UE_LOG(LogTemp, Log, TEXT("Learning panel opened"));
}

void UMingMainGameUI::OnAssetGeneratorButtonClicked()
{
    ShowAssetGeneratorPanel();
    UE_LOG(LogTemp, Log, TEXT("Asset generator panel opened"));
}

void UMingMainGameUI::OnSettingsButtonClicked()
{
    ShowSettingsPanel();
    UE_LOG(LogTemp, Log, TEXT("Settings panel opened"));
}

void UMingMainGameUI::SetupSystemStatusMonitoring()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            StatusUpdateTimer,
            this,
            &UMingMainGameUI::UpdateSystemStatusText,
            StatusUpdateInterval,
            true
        );
    }
}

void UMingMainGameUI::UpdateSystemStatusText()
{
    if (!SystemStatusText) return;

    FString StatusText = TEXT("系統狀態:\n");
    
    if (GameMode)
    {
        StatusText += TEXT("• 遊戲模式: 已初始化\n");
        
        // 檢查各個系統狀態
        if (GameMode->GetFactionManager())
        {
            StatusText += TEXT("• 派系管理: 運行中\n");
        }
        else
        {
            StatusText += TEXT("• 派系管理: 未初始化\n");
        }
        
        if (GameMode->GetDynamicHistorySystem())
        {
            StatusText += TEXT("• 動態歷史: 運行中\n");
        }
        else
        {
            StatusText += TEXT("• 動態歷史: 未初始化\n");
        }
        
        if (GameMode->GetSelfLearningSystem())
        {
            StatusText += TEXT("• 自學習系統: 運行中\n");
        }
        else
        {
            StatusText += TEXT("• 自學習系統: 未初始化\n");
        }
        
        if (GameMode->GetAutoSceneGenerator())
        {
            StatusText += TEXT("• 場景生成器: 運行中\n");
        }
        else
        {
            StatusText += TEXT("• 場景生成器: 未初始化\n");
        }
        
        if (GameMode->GetGameAssetGenerator())
        {
            StatusText += TEXT("• 資產生成器: 運行中\n");
        }
        else
        {
            StatusText += TEXT("• 資產生成器: 未初始化\n");
        }
    }
    else
    {
        StatusText += TEXT("• 遊戲模式: 未設置\n");
    }
    
    SystemStatusText->SetText(FText::FromString(StatusText));
}

void UMingMainGameUI::SetupPerformanceMonitoring()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            PerformanceUpdateTimer,
            this,
            &UMingMainGameUI::UpdatePerformanceText,
            PerformanceUpdateInterval,
            true
        );
    }
}

void UMingMainGameUI::UpdatePerformanceText()
{
    if (!PerformanceText) return;

    FString PerfText = TEXT("性能監控:\n");
    
    if (GEngine)
    {
        // 獲取幀率信息
        PerfText += FString::Printf(TEXT("• FPS: %.1f\n"), GEngine->GetFrameRate());
        
        // 獲取內存使用情況
        FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
        float UsedMemoryMB = MemoryStats.UsedPhysical / (1024.0f * 1024.0f);
        PerfText += FString::Printf(TEXT("• 內存使用: %.1f MB\n"), UsedMemoryMB);
        
        // 獲取渲染信息
        if (GEngine->Renderer)
        {
            PerfText += TEXT("• 渲染器: 運行中\n");
        }
        else
        {
            PerfText += TEXT("• 渲染器: 未初始化\n");
        }
    }
    
    if (GameMode)
    {
        PerfText += TEXT("• 遊戲邏輯: 運行中\n");
    }
    else
    {
        PerfText += TEXT("• 遊戲邏輯: 未初始化\n");
    }
    
    PerformanceText->SetText(FText::FromString(PerfText));
}

void UMingMainGameUI::UpdateLocalizedText()
{
    // 更新當前派系顯示
    if (CurrentFactionText && GameMode && GameMode->GetFactionManager())
    {
        // 這裡可以從派系管理器獲取當前派系信息
        CurrentFactionText->SetText(FText::FromString(TEXT("當前派系: 待設置")));
    }
    
    // 更新當前日期顯示
    if (CurrentDateText && GameMode && GameMode->GetDynamicHistorySystem())
    {
        // 這裡可以從歷史系統獲取當前遊戲日期
        CurrentDateText->SetText(FText::FromString(TEXT("遊戲日期: 第1天")));
    }
}
