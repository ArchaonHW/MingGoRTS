#include "MingMainDashboard.h"
#include "MingRelationshipManager.h"
#include "MingAudioRelationshipManager.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"

UMingMainDashboard::UMingMainDashboard()
{
    bIsInitialized = false;
    RelationshipManager = nullptr;
    AudioRelationshipManager = nullptr;
    bIsVisible = false;
    CurrentActivePanel = EDashboardPanel::None;
    bAutoRefresh = true;
    RefreshInterval = 5.0f;
}

void UMingMainDashboard::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 綁定按鈕事件
    if (ShowDashboardButton)
    {
        ShowDashboardButton->OnClicked.AddDynamic(this, &UMingMainDashboard::OnShowDashboardClicked);
    }
    
    if (HideDashboardButton)
    {
        HideDashboardButton->OnClicked.AddDynamic(this, &UMingMainDashboard::OnHideDashboardClicked);
    }
    
    if (RelationshipPanelButton)
    {
        RelationshipPanelButton->OnClicked.AddDynamic(this, &UMingMainDashboard::OnRelationshipPanelClicked);
    }
    
    if (ReputationPanelButton)
    {
        ReputationPanelButton->OnClicked.AddDynamic(this, &UMingMainDashboard::OnReputationPanelClicked);
    }
    
    if (DialoguePanelButton)
    {
        DialoguePanelButton->OnClicked.AddDynamic(this, &UMingMainDashboard::OnDialoguePanelClicked);
    }
    
    if (QuestPanelButton)
    {
        QuestPanelButton->OnClicked.AddDynamic(this, &UMingMainDashboard::OnQuestPanelClicked);
    }
    
    if (AudioPanelButton)
    {
        AudioPanelButton->OnClicked.AddDynamic(this, &UMingMainDashboard::OnAudioPanelClicked);
    }
    
    if (SettingsButton)
    {
        SettingsButton->OnClicked.AddDynamic(this, &UMingMainDashboard::OnSettingsClicked);
    }
    
    if (RefreshButton)
    {
        RefreshButton->OnClicked.AddDynamic(this, &UMingMainDashboard::OnRefreshClicked);
    }
    
    // 初始化面板
    InitializePanels();
    
    // 隱藏初始狀態
    HideDashboard();
}

void UMingMainDashboard::InitializeDashboard(UMingRelationshipManager* InRelationshipManager, UMingAudioRelationshipManager* InAudioManager)
{
    RelationshipManager = InRelationshipManager;
    AudioRelationshipManager = InAudioManager;
    
    if (RelationshipManager && AudioRelationshipManager)
    {
        bIsInitialized = true;
        
        // 初始化各個面板
        InitializeRelationshipPanel();
        InitializeReputationPanel();
        InitializeDialoguePanel();
        InitializeQuestPanel();
        InitializeAudioPanel();
        
        // 更新儀表板數據
        UpdateDashboardData();
        
        // 開始自動刷新
        if (bAutoRefresh)
        {
            StartAutoRefresh();
        }
        
        UE_LOG(LogTemp, Log, TEXT("個人系統主儀表板初始化完成"));
    }
}

void UMingMainDashboard::ShowDashboard()
{
    if (bIsVisible)
    {
        return;
    }
    
    bIsVisible = true;
    
    // 顯示主面板
    if (MainDashboardPanel)
    {
        MainDashboardPanel->SetVisibility(ESlateVisibility::Visible);
        PlayShowAnimation();
    }
    
    // 顯示默認面板
    ShowPanel(EDashboardPanel::Relationship);
    
    // 觸發顯示事件
    OnDashboardShown.Broadcast();
    
    UE_LOG(LogTemp, Log, TEXT("顯示個人系統主儀表板"));
}

void UMingMainDashboard::HideDashboard()
{
    if (!bIsVisible)
    {
        return;
    }
    
    bIsVisible = false;
    
    // 停止自動刷新
    StopAutoRefresh();
    
    // 隱藏所有面板
    HideAllPanels();
    
    // 隱藏主面板
    if (MainDashboardPanel)
    {
        PlayHideAnimation();
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
        {
            if (MainDashboardPanel)
            {
                MainDashboardPanel->SetVisibility(ESlateVisibility::Hidden);
            }
        }, 0.3f, false);
    }
    
    // 觸發隱藏事件
    OnDashboardHidden.Broadcast();
    
    UE_LOG(LogTemp, Log, TEXT("隱藏個人系統主儀表板"));
}

void UMingMainDashboard::ToggleDashboard()
{
    if (bIsVisible)
    {
        HideDashboard();
    }
    else
    {
        ShowDashboard();
    }
}

void UMingMainDashboard::ShowPanel(EDashboardPanel PanelType)
{
    if (!bIsVisible)
    {
        ShowDashboard();
    }
    
    // 隱藏當前活動面板
    HideCurrentPanel();
    
    // 顯示新面板
    CurrentActivePanel = PanelType;
    
    switch (PanelType)
    {
        case EDashboardPanel::Relationship:
            ShowRelationshipPanel();
            break;
            
        case EDashboardPanel::Reputation:
            ShowReputationPanel();
            break;
            
        case EDashboardPanel::Dialogue:
            ShowDialoguePanel();
            break;
            
        case EDashboardPanel::Quest:
            ShowQuestPanel();
            break;
            
        case EDashboardPanel::Audio:
            ShowAudioPanel();
            break;
            
        case EDashboardPanel::Settings:
            ShowSettingsPanel();
            break;
            
        default:
            break;
    }
    
    // 更新導航按鈕狀態
    UpdateNavigationButtons();
    
    // 觸發面板切換事件
    OnPanelSwitched.Broadcast(PanelType);
}

void UMingMainDashboard::HideCurrentPanel()
{
    switch (CurrentActivePanel)
    {
        case EDashboardPanel::Relationship:
            HideRelationshipPanel();
            break;
            
        case EDashboardPanel::Reputation:
            HideReputationPanel();
            break;
            
        case EDashboardPanel::Dialogue:
            HideDialoguePanel();
            break;
            
        case EDashboardPanel::Quest:
            HideQuestPanel();
            break;
            
        case EDashboardPanel::Audio:
            HideAudioPanel();
            break;
            
        case EDashboardPanel::Settings:
            HideSettingsPanel();
            break;
            
        default:
            break;
    }
}

void UMingMainDashboard::RefreshDashboard()
{
    if (!bIsInitialized)
    {
        return;
    }
    
    // 更新儀表板數據
    UpdateDashboardData();
    
    // 刷新當前活動面板
    RefreshCurrentPanel();
    
    // 播放刷新動畫
    PlayRefreshAnimation();
    
    // 觸發刷新事件
    OnDashboardRefreshed.Broadcast();
    
    UE_LOG(LogTemp, Log, TEXT("刷新個人系統主儀表板"));
}

void UMingMainDashboard::SetAutoRefresh(bool bEnabled, float Interval)
{
    bAutoRefresh = bEnabled;
    RefreshInterval = FMath::Clamp(Interval, 1.0f, 60.0f);
    
    if (bIsVisible)
    {
        if (bAutoRefresh)
        {
            StartAutoRefresh();
        }
        else
        {
            StopAutoRefresh();
        }
    }
}

void UMingMainDashboard::UpdateDashboardData()
{
    if (!bIsInitialized)
    {
        return;
    }
    
    // 更新統計信息
    UpdateStatistics();
    
    // 更新快速狀態
    UpdateQuickStatus();
    
    // 更新通知
    UpdateNotifications();
}

// 按鈕事件處理
void UMingMainDashboard::OnShowDashboardClicked()
{
    ShowDashboard();
}

void UMingMainDashboard::OnHideDashboardClicked()
{
    HideDashboard();
}

void UMingMainDashboard::OnRelationshipPanelClicked()
{
    ShowPanel(EDashboardPanel::Relationship);
}

void UMingMainDashboard::OnReputationPanelClicked()
{
    ShowPanel(EDashboardPanel::Reputation);
}

void UMingMainDashboard::OnDialoguePanelClicked()
{
    ShowPanel(EDashboardPanel::Dialogue);
}

void UMingMainDashboard::OnQuestPanelClicked()
{
    ShowPanel(EDashboardPanel::Quest);
}

void UMingMainDashboard::OnAudioPanelClicked()
{
    ShowPanel(EDashboardPanel::Audio);
}

void UMingMainDashboard::OnSettingsClicked()
{
    ShowPanel(EDashboardPanel::Settings);
}

void UMingMainDashboard::OnRefreshClicked()
{
    RefreshDashboard();
}

// 私有輔助函數
void UMingMainDashboard::InitializePanels()
{
    // 初始化各個面板的引用
    // 這些通常在藍圖中設置
}

void UMingMainDashboard::InitializeRelationshipPanel()
{
    if (RelationshipPanel && RelationshipManager)
    {
        RelationshipPanel->UpdateRelationshipData(RelationshipManager);
    }
}

void UMingMainDashboard::InitializeReputationPanel()
{
    if (ReputationPanel && RelationshipManager)
    {
        ReputationPanel->UpdateReputationData(RelationshipManager);
    }
}

void UMingMainDashboard::InitializeDialoguePanel()
{
    // 對話面板初始化
    if (DialoguePanel)
    {
        // 初始化對話面板
    }
}

void UMingMainDashboard::InitializeQuestPanel()
{
    // 任務面板初始化
    if (QuestPanel && RelationshipManager)
    {
        QuestPanel->UpdateQuestData(RelationshipManager);
    }
}

void UMingMainDashboard::InitializeAudioPanel()
{
    // 音頻面板初始化
    if (AudioPanel && AudioRelationshipManager)
    {
        AudioPanel->InitializeAudioPanel(AudioRelationshipManager, RelationshipManager);
    }
}

void UMingMainDashboard::ShowRelationshipPanel()
{
    if (RelationshipPanel)
    {
        RelationshipPanel->SetVisibility(ESlateVisibility::Visible);
        if (RelationshipManager)
        {
            RelationshipPanel->UpdateRelationshipData(RelationshipManager);
        }
    }
}

void UMingMainDashboard::ShowReputationPanel()
{
    if (ReputationPanel)
    {
        ReputationPanel->SetVisibility(ESlateVisibility::Visible);
        if (RelationshipManager)
        {
            ReputationPanel->UpdateReputationData(RelationshipManager);
        }
    }
}

void UMingMainDashboard::ShowDialoguePanel()
{
    if (DialoguePanel)
    {
        DialoguePanel->SetVisibility(ESlateVisibility::Visible);
    }
}

void UMingMainDashboard::ShowQuestPanel()
{
    if (QuestPanel)
    {
        QuestPanel->SetVisibility(ESlateVisibility::Visible);
        if (RelationshipManager)
        {
            QuestPanel->UpdateQuestData(RelationshipManager);
        }
    }
}

void UMingMainDashboard::ShowAudioPanel()
{
    if (AudioPanel)
    {
        AudioPanel->SetVisibility(ESlateVisibility::Visible);
        if (AudioRelationshipManager)
        {
            AudioPanel->InitializeAudioPanel(AudioRelationshipManager, RelationshipManager);
        }
    }
}

void UMingMainDashboard::ShowSettingsPanel()
{
    if (SettingsPanel)
    {
        SettingsPanel->SetVisibility(ESlateVisibility::Visible);
        UpdateSettingsPanel();
    }
}

void UMingMainDashboard::HideAllPanels()
{
    HideRelationshipPanel();
    HideReputationPanel();
    HideDialoguePanel();
    HideQuestPanel();
    HideAudioPanel();
    HideSettingsPanel();
}

void UMingMainDashboard::HideRelationshipPanel()
{
    if (RelationshipPanel)
    {
        RelationshipPanel->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UMingMainDashboard::HideReputationPanel()
{
    if (ReputationPanel)
    {
        ReputationPanel->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UMingMainDashboard::HideDialoguePanel()
{
    if (DialoguePanel)
    {
        DialoguePanel->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UMingMainDashboard::HideQuestPanel()
{
    if (QuestPanel)
    {
        QuestPanel->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UMingMainDashboard::HideAudioPanel()
{
    if (AudioPanel)
    {
        AudioPanel->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UMingMainDashboard::HideSettingsPanel()
{
    if (SettingsPanel)
    {
        SettingsPanel->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UMingMainDashboard::RefreshCurrentPanel()
{
    switch (CurrentActivePanel)
    {
        case EDashboardPanel::Relationship:
            if (RelationshipPanel && RelationshipManager)
            {
                RelationshipPanel->UpdateRelationshipData(RelationshipManager);
            }
            break;
            
        case EDashboardPanel::Reputation:
            if (ReputationPanel && RelationshipManager)
            {
                ReputationPanel->UpdateReputationData(RelationshipManager);
            }
            break;
            
        case EDashboardPanel::Quest:
            if (QuestPanel && RelationshipManager)
            {
                QuestPanel->UpdateQuestData(RelationshipManager);
            }
            break;
            
        case EDashboardPanel::Audio:
            if (AudioPanel && AudioRelationshipManager)
            {
                AudioPanel->UpdateAudioSettings();
            }
            break;
            
        default:
            break;
    }
}

void UMingMainDashboard::UpdateNavigationButtons()
{
    // 更新導航按鈕的視覺狀態
    if (RelationshipPanelButton)
    {
        RelationshipPanelButton->SetIsPressed(CurrentActivePanel == EDashboardPanel::Relationship);
    }
    
    if (ReputationPanelButton)
    {
        ReputationPanelButton->SetIsPressed(CurrentActivePanel == EDashboardPanel::Reputation);
    }
    
    if (DialoguePanelButton)
    {
        DialoguePanelButton->SetIsPressed(CurrentActivePanel == EDashboardPanel::Dialogue);
    }
    
    if (QuestPanelButton)
    {
        QuestPanelButton->SetIsPressed(CurrentActivePanel == EDashboardPanel::Quest);
    }
    
    if (AudioPanelButton)
    {
        AudioPanelButton->SetIsPressed(CurrentActivePanel == EDashboardPanel::Audio);
    }
}

void UMingMainDashboard::UpdateStatistics()
{
    if (!RelationshipManager)
    {
        return;
    }
    
    // 獲取統計數據
    TArray<FRelationshipData> AllRelationships = RelationshipManager->GetAllRelationships();
    TArray<FReputationData> AllReputations = RelationshipManager->GetAllReputations();
    TArray<FQuestData> AllQuests = RelationshipManager->GetAllQuests();
    
    int32 TotalRelationships = AllRelationships.Num();
    int32 PositiveRelationships = 0;
    int32 NegativeRelationships = 0;
    
    int32 TotalReputations = AllReputations.Num();
    int32 HighReputations = 0;
    int32 LowReputations = 0;
    
    int32 ActiveQuests = 0;
    int32 CompletedQuests = 0;
    
    // 計算統計數據
    for (const FRelationshipData& Relationship : AllRelationships)
    {
        if (Relationship.RelationshipValue > 0.0f)
        {
            PositiveRelationships++;
        }
        else if (Relationship.RelationshipValue < 0.0f)
        {
            NegativeRelationships++;
        }
    }
    
    for (const FReputationData& Reputation : AllReputations)
    {
        if (Reputation.ReputationValue > 75.0f)
        {
            HighReputations++;
        }
        else if (Reputation.ReputationValue < 25.0f)
        {
            LowReputations++;
        }
    }
    
    for (const FQuestData& Quest : AllQuests)
    {
        if (Quest.bIsActive && !Quest.bIsCompleted)
        {
            ActiveQuests++;
        }
        else if (Quest.bIsCompleted)
        {
            CompletedQuests++;
        }
    }
    
    // 更新UI顯示
    if (TotalRelationshipsText)
    {
        TotalRelationshipsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TotalRelationships)));
    }
    
    if (PositiveRelationshipsText)
    {
        PositiveRelationshipsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PositiveRelationships)));
    }
    
    if (NegativeRelationshipsText)
    {
        NegativeRelationshipsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), NegativeRelationships)));
    }
    
    if (TotalReputationsText)
    {
        TotalReputationsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TotalReputations)));
    }
    
    if (HighReputationsText)
    {
        HighReputationsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), HighReputations)));
    }
    
    if (LowReputationsText)
    {
        LowReputationsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), LowReputations)));
    }
    
    if (ActiveQuestsText)
    {
        ActiveQuestsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), ActiveQuests)));
    }
    
    if (CompletedQuestsText)
    {
        CompletedQuestsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), CompletedQuests)));
    }
}

void UMingMainDashboard::UpdateQuickStatus()
{
    if (!RelationshipManager)
    {
        return;
    }
    
    // 獲取快速狀態信息
    float AverageRelationship = RelationshipManager->GetAverageRelationshipValue();
    float AverageReputation = RelationshipManager->GetAverageReputationValue();
    int32 RecentNotifications = RelationshipManager->GetRecentNotificationCount();
    
    // 更新UI顯示
    if (AverageRelationshipText)
    {
        AverageRelationshipText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), AverageRelationship)));
    }
    
    if (AverageReputationText)
    {
        AverageReputationText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), AverageReputation)));
    }
    
    if (RecentNotificationsText)
    {
        RecentNotificationsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), RecentNotifications)));
    }
    
    // 更新進度條
    if (RelationshipProgressBar)
    {
        float NormalizedValue = (AverageRelationship + 100.0f) / 200.0f; // -100 到 100 映射到 0 到 1
        RelationshipProgressBar->SetPercent(FMath::Clamp(NormalizedValue, 0.0f, 1.0f));
    }
    
    if (ReputationProgressBar)
    {
        float NormalizedValue = (AverageReputation + 100.0f) / 200.0f; // -100 到 100 映射到 0 到 1
        ReputationProgressBar->SetPercent(FMath::Clamp(NormalizedValue, 0.0f, 1.0f));
    }
}

void UMingMainDashboard::UpdateNotifications()
{
    if (!RelationshipManager)
    {
        return;
    }
    
    // 獲取通知列表
    TArray<FNotificationData> Notifications = RelationshipManager->GetRecentNotifications();
    
    // 更新通知顯示
    if (NotificationList)
    {
        NotificationList->ClearChildren();
        
        for (const FNotificationData& Notification : Notifications)
        {
            UTextBlock* NotificationText = NewObject<UTextBlock>(this);
            if (NotificationText)
            {
                NotificationText->SetText(FText::FromString(Notification.Message));
                FSlateFontInfo FontInfo = NotificationText->GetFont();
                FontInfo.Size = 12;
                NotificationText->SetFont(FontInfo);
                NotificationText->SetMargin(FMargin(5.0f, 2.0f));
                
                // 根據通知類型設置顏色
                FSlateColor Color = GetNotificationColor(Notification.Type);
                NotificationText->SetColorAndOpacity(Color);
                
                NotificationList->AddChild(NotificationText);
            }
        }
    }
    
    // 更新通知計數
    if (NotificationCountText)
    {
        NotificationCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Notifications.Num())));
    }
}

void UMingMainDashboard::UpdateSettingsPanel()
{
    // 更新設置面板的UI
    if (AutoRefreshCheckBox)
    {
        AutoRefreshCheckBox->SetIsChecked(bAutoRefresh);
    }
    
    if (RefreshIntervalSlider)
    {
        RefreshIntervalSlider->SetValue(RefreshInterval);
    }
    
    if (RefreshIntervalText)
    {
        RefreshIntervalText->SetText(FText::FromString(FString::Printf(TEXT("%.0f 秒"), RefreshInterval)));
    }
}

void UMingMainDashboard::StartAutoRefresh()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(AutoRefreshTimerHandle, this, &UMingMainDashboard::RefreshDashboard, RefreshInterval, true);
    }
}

void UMingMainDashboard::StopAutoRefresh()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoRefreshTimerHandle);
    }
}

FSlateColor UMingMainDashboard::GetNotificationColor(ENotificationType NotificationType)
{
    switch (NotificationType)
    {
        case ENotificationType::Info:
            return FLinearColor::Blue;
        case ENotificationType::Warning:
            return FLinearColor::Yellow;
        case ENotificationType::Error:
            return FLinearColor::Red;
        case ENotificationType::Success:
            return FLinearColor::Green;
        default:
            return FLinearColor::White;
    }
}

void UMingMainDashboard::PlayShowAnimation()
{
    // 播放顯示動畫
    UE_LOG(LogTemp, Log, TEXT("播放主儀表板顯示動畫"));
}

void UMingMainDashboard::PlayHideAnimation()
{
    // 播放隱藏動畫
    UE_LOG(LogTemp, Log, TEXT("播放主儀表板隱藏動畫"));
}

void UMingMainDashboard::PlayRefreshAnimation()
{
    // 播放刷新動畫
    if (RefreshButton)
    {
        UE_LOG(LogTemp, Log, TEXT("播放刷新動畫"));
    }
}
