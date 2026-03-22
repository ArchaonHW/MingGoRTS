#include "MingMainDashboard.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/ScrollBox.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"

UMingMainDashboard::UMingMainDashboard(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , CurrentTab(EDashboardTab::Overview)
    , bAutoRefresh(true)
    , RefreshInterval(5.0f)
    , MaxNotifications(10)
    , TabContainer(nullptr)
    , TabButtonContainer(nullptr)
    , QuickActionContainer(nullptr)
    , NotificationContainer(nullptr)
    , SummaryText(nullptr)
    , RelationshipCountText(nullptr)
    , QuestCountText(nullptr)
    , ReputationText(nullptr)
    , OverallProgressBar(nullptr)
    , RefreshButton(nullptr)
    , SettingsButton(nullptr)
    , OverviewWidget(nullptr)
    , RelationshipWidget(nullptr)
    , ReputationWidget(nullptr)
    , QuestWidget(nullptr)
    , AudioWidget(nullptr)
    , SettingsWidget(nullptr)
{
}

void UMingMainDashboard::NativeConstruct()
{
    Super::NativeConstruct();

    InitializeTabs();
    InitializeQuickActions();
    SetupEventBindings();
    
    // 顯示默認標籤頁
    ShowOverview();
    
    // 啟動自動刷新
    if (bAutoRefresh && GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(RefreshTimerHandle, this, &UMingMainDashboard::RefreshTimer, RefreshInterval, true);
    }
}

void UMingMainDashboard::NativeDestruct()
{
    // 清理定時器
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(RefreshTimerHandle);
    }

    Super::NativeDestruct();
}

void UMingMainDashboard::InitializeDashboard(UMingPersonalManager* InPersonalManager)
{
    PersonalManager = InPersonalManager;
    
    if (PersonalManager)
    {
        RefreshDashboard();
        UE_LOG(LogTemp, Log, TEXT("Main dashboard initialized successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Main dashboard initialization failed - Personal manager is null"));
    }
}

void UMingMainDashboard::SwitchToTab(EDashboardTab Tab)
{
    CurrentTab = Tab;
    ShowTabContent(Tab);
    UpdateTabUI();
    
    OnDashboardTabChanged.Broadcast(Tab);
    
    UE_LOG(LogTemp, Log, TEXT("Switched to dashboard tab: %d"), (int32)Tab);
}

void UMingMainDashboard::ShowOverview()
{
    SwitchToTab(EDashboardTab::Overview);
}

void UMingMainDashboard::ShowRelationships()
{
    SwitchToTab(EDashboardTab::Relationships);
}

void UMingMainDashboard::ShowReputation()
{
    SwitchToTab(EDashboardTab::Reputation);
}

void UMingMainDashboard::ShowQuests()
{
    SwitchToTab(EDashboardTab::Quests);
}

void UMingMainDashboard::ShowAudio()
{
    SwitchToTab(EDashboardTab::Audio);
}

void UMingMainDashboard::ShowSettings()
{
    SwitchToTab(EDashboardTab::Settings);
}

void UMingMainDashboard::RefreshDashboard()
{
    UpdateSummary();
    UpdateQuickActions();
    UpdateProgressBar();
    UpdateNotificationUI();
    
    OnDashboardRefreshed.Broadcast(DashboardSummary);
    
    UE_LOG(LogTemp, Log, TEXT("Dashboard refreshed"));
}

void UMingMainDashboard::UpdateSummary()
{
    if (!PersonalManager)
    {
        return;
    }
    
    DashboardSummary = CalculateSummary();
    
    // 更新UI文本
    if (SummaryText)
    {
        FString SummaryString = FString::Printf(TEXT("關係: %d | 聲望: %.1f | 任務: %d/%d"), 
                                              DashboardSummary.TotalRelationships,
                                              DashboardSummary.AverageReputation,
                                              DashboardSummary.ActiveQuests,
                                              DashboardSummary.CompletedQuests);
        SummaryText->SetText(FText::FromString(SummaryString));
    }
    
    if (RelationshipCountText)
    {
        RelationshipCountText->SetText(FText::AsNumber(DashboardSummary.TotalRelationships));
    }
    
    if (QuestCountText)
    {
        FString QuestString = FString::Printf(TEXT("%d/%d"), DashboardSummary.ActiveQuests, DashboardSummary.CompletedQuests);
        QuestCountText->SetText(FText::FromString(QuestString));
    }
    
    if (ReputationText)
    {
        FString RepString = FString::Printf(TEXT("%.1f"), DashboardSummary.AverageReputation);
        ReputationText->SetText(FText::FromString(RepString));
    }
}

void UMingMainDashboard::UpdateQuickActions()
{
    QuickActions = GenerateQuickActions();
    
    // 這裡應該更新快速操作按鈕的UI
    UE_LOG(LogTemp, Log, TEXT("Updated quick actions: %d available"), QuickActions.Num());
}

void UMingMainDashboard::ExecuteQuickAction(const FString& ActionName)
{
    for (const FDashboardQuickAction& Action : QuickActions)
    {
        if (Action.ActionName == ActionName && Action.bIsAvailable)
        {
            // 執行對應的快速操作
            if (ActionName == TEXT("AddRelationship"))
            {
                QuickAddRelationship(TEXT("NEW_CHARACTER"));
            }
            else if (ActionName == TEXT("StartQuest"))
            {
                QuickStartQuest(TEXT("QUICK_QUEST"));
            }
            else if (ActionName == TEXT("PlayAudio"))
            {
                QuickPlayAudio(TEXT("Test"));
            }
            else if (ActionName == TEXT("OpenDialogue"))
            {
                QuickOpenDialogue(TEXT("NPC_001"));
            }
            
            OnQuickActionExecuted.Broadcast(ActionName);
            
            UE_LOG(LogTemp, Log, TEXT("Executed quick action: %s"), *ActionName);
            break;
        }
    }
}

void UMingMainDashboard::QuickAddRelationship(const FString& CharacterID)
{
    if (PersonalManager)
    {
        PersonalManager->UpdateCharacterRelationship(CharacterID, 10.0f, TEXT("快速添加"));
        ShowNotification(FString::Printf(TEXT("已添加關係: %s"), *CharacterID), TEXT("Success"));
    }
}

void UMingMainDashboard::QuickStartQuest(const FString& QuestID)
{
    ShowNotification(FString::Printf(TEXT("開始任務: %s"), *QuestID), TEXT("Info"));
}

void UMingMainDashboard::QuickPlayAudio(const FString& AudioType)
{
    if (PersonalManager)
    {
        PersonalManager->PlayRelationshipAudio(TEXT("TEST_NPC"), TEXT("Increase"));
        PersonalManager->PlayRepublicEraTheme(TEXT("Battle"), 1.0f);
        ShowNotification(TEXT("播放測試音頻"), TEXT("Info"));
    }
}

void UMingMainDashboard::QuickOpenDialogue(const FString& CharacterID)
{
    ShowNotification(FString::Printf(TEXT("打開對話: %s"), *CharacterID), TEXT("Info"));
}

void UMingMainDashboard::ShowNotification(const FString& Message, const FString& Type)
{
    AddNotification(Message, Type);
    UpdateNotificationUI();
}

void UMingMainDashboard::ShowWarning(const FString& Message)
{
    ShowNotification(Message, TEXT("Warning"));
}

void UMingMainDashboard::ShowError(const FString& Message)
{
    ShowNotification(Message, TEXT("Error"));
}

void UMingMainDashboard::ClearNotifications()
{
    Notifications.Empty();
    UpdateNotificationUI();
    
    UE_LOG(LogTemp, Log, TEXT("Cleared all notifications"));
}

void UMingMainDashboard::ShowAchievementUnlocked(const FString& AchievementName, const FString& Description)
{
    FString Message = FString::Printf(TEXT("🏆 成就解鎖: %s - %s"), *AchievementName, *Description);
    ShowNotification(Message, TEXT("Achievement"));
}

bool UMingMainDashboard::IsTabVisible(EDashboardTab Tab) const
{
    return CurrentTab == Tab;
}

int32 UMingMainDashboard::GetNotificationCount() const
{
    return Notifications.Num();
}

float UMingMainDashboard::GetRelationshipProgress() const
{
    if (DashboardSummary.TotalRelationships == 0)
    {
        return 0.0f;
    }
    
    return (float)DashboardSummary.FriendCount / DashboardSummary.TotalRelationships;
}

float UMingMainDashboard::GetReputationProgress() const
{
    // 假設最高聲望為1000
    return FMath::Clamp(DashboardSummary.AverageReputation / 1000.0f, 0.0f, 1.0f);
}

float UMingMainDashboard::GetQuestProgress() const
{
    int32 TotalQuests = DashboardSummary.ActiveQuests + DashboardSummary.CompletedQuests;
    if (TotalQuests == 0)
    {
        return 0.0f;
    }
    
    return (float)DashboardSummary.CompletedQuests / TotalQuests;
}

float UMingMainDashboard::GetOverallProgress() const
{
    return (GetRelationshipProgress() + GetReputationProgress() + GetQuestProgress()) / 3.0f;
}

void UMingMainDashboard::OnTabButtonClicked(EDashboardTab Tab)
{
    SwitchToTab(Tab);
}

void UMingMainDashboard::OnQuickActionButtonClicked(const FString& ActionName)
{
    ExecuteQuickAction(ActionName);
}

void UMingMainDashboard::OnNotificationClicked(int32 NotificationIndex)
{
    if (Notifications.IsValidIndex(NotificationIndex))
    {
        FString Notification = Notifications[NotificationIndex];
        UE_LOG(LogTemp, Log, TEXT("Notification clicked: %s"), *Notification);
        
        // 可以在這裡添加點擊通知的處理邏輯
    }
}

void UMingMainDashboard::OnRefreshButtonClicked()
{
    RefreshDashboard();
}

void UMingMainDashboard::InitializeTabs()
{
    // 這裡應該初始化標籤頁Widget
    UE_LOG(LogTemp, Log, TEXT("Initializing dashboard tabs"));
}

void UMingMainDashboard::InitializeQuickActions()
{
    QuickActions.Empty();
    
    // 添加默認快速操作
    FDashboardQuickAction AddRelationshipAction;
    AddRelationshipAction.ActionName = TEXT("AddRelationship");
    AddRelationshipAction.ActionDescription = TEXT("快速添加關係");
    AddRelationshipAction.ActionIcon = TEXT("Add");
    AddRelationshipAction.bIsAvailable = true;
    AddRelationshipAction.ActionColor = FLinearColor(0.0f, 0.8f, 0.0f, 1.0f);
    QuickActions.Add(AddRelationshipAction);
    
    FDashboardQuickAction StartQuestAction;
    StartQuestAction.ActionName = TEXT("StartQuest");
    StartQuestAction.ActionDescription = TEXT("快速開始任務");
    StartQuestAction.ActionIcon = TEXT("Quest");
    StartQuestAction.bIsAvailable = true;
    StartQuestAction.ActionColor = FLinearColor(0.0f, 0.4f, 0.8f, 1.0f);
    QuickActions.Add(StartQuestAction);
    
    FDashboardQuickAction PlayAudioAction;
    PlayAudioAction.ActionName = TEXT("PlayAudio");
    PlayAudioAction.ActionDescription = TEXT("播放測試音頻");
    PlayAudioAction.ActionIcon = TEXT("Audio");
    PlayAudioAction.bIsAvailable = true;
    PlayAudioAction.ActionColor = FLinearColor(0.8f, 0.0f, 0.8f, 1.0f);
    QuickActions.Add(PlayAudioAction);
    
    FDashboardQuickAction OpenDialogueAction;
    OpenDialogueAction.ActionName = TEXT("OpenDialogue");
    OpenDialogueAction.ActionDescription = TEXT("打開對話");
    OpenDialogueAction.ActionIcon = TEXT("Dialogue");
    OpenDialogueAction.bIsAvailable = true;
    OpenDialogueAction.ActionColor = FLinearColor(0.8f, 0.8f, 0.0f, 1.0f);
    QuickActions.Add(OpenDialogueAction);
}

void UMingMainDashboard::UpdateTabUI()
{
    // 更新標籤頁按鈕的視覺狀態
    UE_LOG(LogTemp, Log, TEXT("Updated tab UI for tab: %d"), (int32)CurrentTab);
}

void UMingMainDashboard::UpdateNotificationUI()
{
    // 這裡應該更新通知容器的UI
    UE_LOG(LogTemp, Log, TEXT("Updated notification UI: %d notifications"), Notifications.Num());
}

void UMingMainDashboard::UpdateProgressBar()
{
    if (OverallProgressBar)
    {
        OverallProgressBar->SetPercent(GetOverallProgress());
    }
}

FDashboardSummary UMingMainDashboard::CalculateSummary() const
{
    FDashboardSummary Summary;
    
    if (!PersonalManager)
    {
        return Summary;
    }
    
    // 計算關係統計
    // 這裡需要從RelationshipManager獲取實際數據
    Summary.TotalRelationships = 5; // 模擬數據
    Summary.FriendCount = 3;
    Summary.EnemyCount = 1;
    
    // 計算聲望統計
    Summary.KnownRegions = 4;
    Summary.AverageReputation = 450.0f;
    Summary.HighestReputation = EReputationLevel::Respected;
    Summary.CurrentRegion = TEXT("北京");
    
    // 計算任務統計
    Summary.ActiveQuests = 2;
    Summary.CompletedQuests = 8;
    
    // 當前主題
    Summary.CurrentTheme = TEXT("和平主題");
    
    return Summary;
}

TArray<FDashboardQuickAction> UMingMainDashboard::GenerateQuickActions() const
{
    TArray<FDashboardQuickAction> Actions;
    
    // 根據當前狀態生成可用的快速操作
    if (PersonalManager)
    {
        // 可以根據玩家等級、地區聲望等條件來決定哪些操作可用
        FDashboardQuickAction TestAction;
        TestAction.ActionName = TEXT("TestAction");
        TestAction.ActionDescription = TEXT("測試操作");
        TestAction.ActionIcon = TEXT("Test");
        TestAction.bIsAvailable = true;
        TestAction.ActionColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
        Actions.Add(TestAction);
    }
    
    return Actions;
}

FString UMingMainDashboard::GetTabDisplayName(EDashboardTab Tab) const
{
    switch (Tab)
    {
        case EDashboardTab::Overview: return TEXT("總覽");
        case EDashboardTab::Relationships: return TEXT("關係");
        case EDashboardTab::Reputation: return TEXT("聲望");
        case EDashboardTab::Quests: return TEXT("任務");
        case EDashboardTab::Audio: return TEXT("音頻");
        case EDashboardTab::Settings: return TEXT("設置");
        default: return TEXT("未知");
    }
}

FLinearColor UMingMainDashboard::GetTabColor(EDashboardTab Tab) const
{
    switch (Tab)
    {
        case EDashboardTab::Overview: return FLinearColor(0.8f, 0.8f, 0.8f, 1.0f);
        case EDashboardTab::Relationships: return FLinearColor(0.0f, 0.8f, 0.0f, 1.0f);
        case EDashboardTab::Reputation: return FLinearColor(0.8f, 0.4f, 0.0f, 1.0f);
        case EDashboardTab::Quests: return FLinearColor(0.0f, 0.4f, 0.8f, 1.0f);
        case EDashboardTab::Audio: return FLinearColor(0.8f, 0.0f, 0.8f, 1.0f);
        case EDashboardTab::Settings: return FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
        default: return FLinearColor(0.7f, 0.7f, 0.7f, 1.0f);
    }
}

void UMingMainDashboard::CreateTabButtons()
{
    // 這裡應該創建標籤頁按鈕
    UE_LOG(LogTemp, Log, TEXT("Creating tab buttons"));
}

void UMingMainDashboard::CreateQuickActionButtons()
{
    // 這裡應該創建快速操作按鈕
    UE_LOG(LogTemp, Log, TEXT("Creating quick action buttons"));
}

void UMingMainDashboard::SetupEventBindings()
{
    // 綁定按鈕事件
    if (RefreshButton)
    {
        RefreshButton->OnClicked.AddDynamic(this, &UMingMainDashboard::OnRefreshButtonClicked);
    }
    
    if (SettingsButton)
    {
        SettingsButton->OnClicked.AddDynamic(this, &UMingMainDashboard::ShowSettings);
    }
    
    // 綁定PersonalManager事件
    if (PersonalManager)
    {
        // 這裡需要綁定PersonalManager的各種事件
        // PersonalManager->OnRelationshipChanged.AddDynamic(this, &UMingMainDashboard::OnRelationshipChanged);
        // PersonalManager->OnReputationChanged.AddDynamic(this, &UMingMainDashboard::OnReputationChanged);
        // PersonalManager->OnQuestCompleted.AddDynamic(this, &UMingMainDashboard::OnQuestCompleted);
    }
}

void UMingMainDashboard::OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason)
{
    RefreshDashboard();
    ShowNotification(FString::Printf(TEXT("關係變化: %s - %s"), *CharacterID, *Reason), TEXT("Relationship"));
}

void UMingMainDashboard::OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason)
{
    RefreshDashboard();
    ShowNotification(FString::Printf(TEXT("聲望變化: %s - %s"), *RegionID, *Reason), TEXT("Reputation"));
}

void UMingMainDashboard::OnQuestCompleted(const FString& QuestID, const FString& RegionID, const FString& QuestGiverID)
{
    RefreshDashboard();
    ShowNotification(FString::Printf(TEXT("任務完成: %s"), *QuestID), TEXT("Quest"));
}

void UMingMainDashboard::ShowTabContent(EDashboardTab Tab)
{
    HideAllTabContent();
    
    switch (Tab)
    {
        case EDashboardTab::Overview:
            if (OverviewWidget) OverviewWidget->SetVisibility(ESlateVisibility::Visible);
            break;
        case EDashboardTab::Relationships:
            if (RelationshipWidget) RelationshipWidget->SetVisibility(ESlateVisibility::Visible);
            break;
        case EDashboardTab::Reputation:
            if (ReputationWidget) ReputationWidget->SetVisibility(ESlateVisibility::Visible);
            break;
        case EDashboardTab::Quests:
            if (QuestWidget) QuestWidget->SetVisibility(ESlateVisibility::Visible);
            break;
        case EDashboardTab::Audio:
            if (AudioWidget) AudioWidget->SetVisibility(ESlateVisibility::Visible);
            break;
        case EDashboardTab::Settings:
            if (SettingsWidget) SettingsWidget->SetVisibility(ESlateVisibility::Visible);
            break;
    }
}

void UMingMainDashboard::HideAllTabContent()
{
    if (OverviewWidget) OverviewWidget->SetVisibility(ESlateVisibility::Hidden);
    if (RelationshipWidget) RelationshipWidget->SetVisibility(ESlateVisibility::Hidden);
    if (ReputationWidget) ReputationWidget->SetVisibility(ESlateVisibility::Hidden);
    if (QuestWidget) QuestWidget->SetVisibility(ESlateVisibility::Hidden);
    if (AudioWidget) AudioWidget->SetVisibility(ESlateVisibility::Hidden);
    if (SettingsWidget) SettingsWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UMingMainDashboard::AddNotification(const FString& Message, const FString& Type)
{
    FString Notification = FString::Printf(TEXT("[%s] %s"), *Type, *Message);
    
    Notifications.Insert(Notification, 0); // 添加到開頭
    
    // 限制通知數量
    while (Notifications.Num() > MaxNotifications)
    {
        Notifications.RemoveAt(Notifications.Num() - 1);
    }
}

void UMingMainDashboard::RemoveNotification(int32 Index)
{
    if (Notifications.IsValidIndex(Index))
    {
        Notifications.RemoveAt(Index);
    }
}

void UMingMainDashboard::RefreshTimer()
{
    if (bAutoRefresh)
    {
        RefreshDashboard();
    }
}

void UMingMainDashboard::UpdateProgressBars()
{
    UpdateProgressBar();
}
