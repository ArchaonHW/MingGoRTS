#include "MingPersonalUIManager.h"
#include "MingRelationshipManager.h"
#include "MingAudioRelationshipManager.h"
#include "MingRepublicEraAudioThemes.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"

UMingPersonalUIManager::UMingPersonalUIManager()
    : ActivePanelType(EPersonalUIType::MainDashboard)
    , bIsMainDashboardVisible(false)
    , AnimationSpeed(1.0f)
{
}

void UMingPersonalUIManager::InitializeUIManager(UMingRelationshipManager* InRelationshipManager, 
                                                UMingAudioRelationshipManager* InAudioManager,
                                                UMingRepublicEraAudioThemes* InThemeManager)
{
    RelationshipManager = InRelationshipManager;
    AudioManager = InAudioManager;
    ThemeManager = InThemeManager;

    if (!RelationshipManager || !AudioManager || !ThemeManager)
    {
        UE_LOG(LogTemp, Error, TEXT("UIManager initialization failed - missing system references"));
        return;
    }

    InitializeDefaultConfigs();
    SetupEventBindings();

    UE_LOG(LogTemp, Log, TEXT("Personal UI Manager initialized successfully"));
}

void UMingPersonalUIManager::OpenUIPanel(EPersonalUIType UIType)
{
    if (UIConfigMap.Contains(UIType))
    {
        const FPersonalUIConfig& Config = UIConfigMap[UIType];
        
        // 創建或獲取Widget
        UUserWidget* Widget = GetWidget(UIType);
        if (!Widget)
        {
            Widget = CreateWidget(UIType);
            if (Widget)
            {
                WidgetMap.Add(UIType, Widget);
            }
        }

        if (Widget)
        {
            // 設置可見性
            Widget->SetVisibility(ESlateVisibility::Visible);
            
            // 播放開啟動畫
            PlayOpenAnimation(UIType);
            
            // 更新配置
            FPersonalUIConfig UpdatedConfig = Config;
            UpdatedConfig.bIsVisible = true;
            UIConfigMap.Add(UIType, UpdatedConfig);
            
            ActivePanelType = UIType;
            
            // 觸發事件
            OnUIPanelOpened.Broadcast(UIType, Widget);
            OnUIStateChanged.Broadcast(true);
            
            UE_LOG(LogTemp, Log, TEXT("Opened UI panel: %d"), (int32)UIType);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UI config not found for type: %d"), (int32)UIType);
    }
}

void UMingPersonalUIManager::CloseUIPanel(EPersonalUIType UIType)
{
    if (WidgetMap.Contains(UIType))
    {
        UUserWidget* Widget = WidgetMap[UIType];
        if (Widget)
        {
            // 播放關閉動畫
            PlayCloseAnimation(UIType);
            
            // 設置為不可見
            Widget->SetVisibility(ESlateVisibility::Hidden);
            
            // 更新配置
            if (UIConfigMap.Contains(UIType))
            {
                FPersonalUIConfig Config = UIConfigMap[UIType];
                Config.bIsVisible = false;
                UIConfigMap.Add(UIType, Config);
            }
            
            // 觸發事件
            OnUIPanelClosed.Broadcast(UIType, Widget);
            
            // 檢查是否還有其他面板打開
            if (!IsAnyPanelOpen())
            {
                OnUIStateChanged.Broadcast(false);
                ActivePanelType = EPersonalUIType::MainDashboard;
            }
            
            UE_LOG(LogTemp, Log, TEXT("Closed UI panel: %d"), (int32)UIType);
        }
    }
}

void UMingPersonalUIManager::ToggleUIPanel(EPersonalUIType UIType)
{
    if (IsWidgetVisible(UIType))
    {
        CloseUIPanel(UIType);
    }
    else
    {
        OpenUIPanel(UIType);
    }
}

void UMingPersonalUIManager::CloseAllPanels()
{
    TArray<EPersonalUIType> VisiblePanels = GetVisiblePanels();
    for (EPersonalUIType PanelType : VisiblePanels)
    {
        CloseUIPanel(PanelType);
    }
}

void UMingPersonalUIManager::ShowMainDashboard()
{
    if (!bIsMainDashboardVisible)
    {
        OpenUIPanel(EPersonalUIType::MainDashboard);
        bIsMainDashboardVisible = true;
    }
}

void UMingPersonalUIManager::HideMainDashboard()
{
    if (bIsMainDashboardVisible)
    {
        CloseUIPanel(EPersonalUIType::MainDashboard);
        bIsMainDashboardVisible = false;
    }
}

UUserWidget* UMingPersonalUIManager::GetWidget(EPersonalUIType UIType)
{
    if (WidgetMap.Contains(UIType))
    {
        return WidgetMap[UIType];
    }
    return nullptr;
}

bool UMingPersonalUIManager::IsWidgetVisible(EPersonalUIType UIType)
{
    if (UIConfigMap.Contains(UIType))
    {
        return UIConfigMap[UIType].bIsVisible;
    }
    return false;
}

void UMingPersonalUIManager::SetWidgetVisibility(EPersonalUIType UIType, bool bVisible)
{
    if (bVisible)
    {
        OpenUIPanel(UIType);
    }
    else
    {
        CloseUIPanel(UIType);
    }
}

void UMingPersonalUIManager::AddUIConfig(const FPersonalUIConfig& Config)
{
    UIConfigMap.Add(Config.UIType, Config);
    UE_LOG(LogTemp, Log, TEXT("Added UI config for: %d"), (int32)Config.UIType);
}

void UMingPersonalUIManager::RemoveUIConfig(EPersonalUIType UIType)
{
    UIConfigMap.Remove(UIType);
    WidgetMap.Remove(UIType);
    UE_LOG(LogTemp, Log, TEXT("Removed UI config for: %d"), (int32)UIType);
}

FPersonalUIConfig UMingPersonalUIManager::GetUIConfig(EPersonalUIType UIType)
{
    if (UIConfigMap.Contains(UIType))
    {
        return UIConfigMap[UIType];
    }
    return FPersonalUIConfig();
}

void UMingPersonalUIManager::PlayOpenAnimation(EPersonalUIType UIType)
{
    if (WidgetMap.Contains(UIType))
    {
        UUserWidget* Widget = WidgetMap[UIType];
        if (Widget && UIConfigMap.Contains(UIType))
        {
            const FPersonalUIConfig& Config = UIConfigMap[UIType];
            PlayAnimation(Widget, Config.OpenAnimation, Config.AnimationDuration / AnimationSpeed);
        }
    }
}

void UMingPersonalUIManager::PlayCloseAnimation(EPersonalUIType UIType)
{
    if (WidgetMap.Contains(UIType))
    {
        UUserWidget* Widget = WidgetMap[UIType];
        if (Widget && UIConfigMap.Contains(UIType))
        {
            const FPersonalUIConfig& Config = UIConfigMap[UIType];
            PlayAnimation(Widget, Config.CloseAnimation, Config.AnimationDuration / AnimationSpeed);
        }
    }
}

void UMingPersonalUIManager::SetAnimationSpeed(float Speed)
{
    AnimationSpeed = FMath::Clamp(Speed, 0.1f, 3.0f);
}

void UMingPersonalUIManager::UpdateRelationshipUI()
{
    if (WidgetMap.Contains(EPersonalUIType::RelationshipPanel))
    {
        UUserWidget* RelationshipWidget = WidgetMap[EPersonalUIType::RelationshipPanel];
        if (RelationshipWidget)
        {
            // 這裡應該調用RelationshipWidget的更新函數
            // 例如：Cast<URelationshipPanel>(RelationshipWidget)->UpdateRelationshipData();
            UE_LOG(LogTemp, Log, TEXT("Updated relationship UI"));
        }
    }
}

void UMingPersonalUIManager::UpdateReputationUI()
{
    if (WidgetMap.Contains(EPersonalUIType::ReputationPanel))
    {
        UUserWidget* ReputationWidget = WidgetMap[EPersonalUIType::ReputationPanel];
        if (ReputationWidget)
        {
            // 這裡應該調用ReputationWidget的更新函數
            UE_LOG(LogTemp, Log, TEXT("Updated reputation UI"));
        }
    }
}

void UMingPersonalUIManager::UpdateDialogueUI(const FString& CharacterID)
{
    if (WidgetMap.Contains(EPersonalUIType::DialoguePanel))
    {
        UUserWidget* DialogueWidget = WidgetMap[EPersonalUIType::DialoguePanel);
        if (DialogueWidget)
        {
            // 這裡應該調用DialogueWidget的更新函數
            UE_LOG(LogTemp, Log, TEXT("Updated dialogue UI for character: %s"), *CharacterID);
        }
    }
}

void UMingPersonalUIManager::UpdateQuestUI()
{
    if (WidgetMap.Contains(EPersonalUIType::QuestPanel))
    {
        UUserWidget* QuestWidget = WidgetMap[EPersonalUIType::QuestPanel];
        if (QuestWidget)
        {
            // 這裡應該調用QuestWidget的更新函數
            UE_LOG(LogTemp, Log, TEXT("Updated quest UI"));
        }
    }
}

void UMingPersonalUIManager::UpdateAudioUI()
{
    if (WidgetMap.Contains(EPersonalUIType::AudioPanel))
    {
        UUserWidget* AudioWidget = WidgetMap[EPersonalUIType::AudioPanel);
        if (AudioWidget)
        {
            // 這裡應該調用AudioWidget的更新函數
            UE_LOG(LogTemp, Log, TEXT("Updated audio UI"));
        }
    }
}

void UMingPersonalUIManager::RefreshDashboard()
{
    UpdateRelationshipUI();
    UpdateReputationUI();
    UpdateQuestUI();
    UpdateAudioUI();
    
    UE_LOG(LogTemp, Log, TEXT("Refreshed main dashboard"));
}

void UMingPersonalUIManager::ShowNotification(const FString& Message, float Duration)
{
    // 這裡應該創建並顯示通知Widget
    UE_LOG(LogTemp, Log, TEXT("Notification: %s (Duration: %.1f)"), *Message, Duration);
}

void UMingPersonalUIManager::ShowConfirmationDialog(const FString& Title, const FString& Message, const FString& OnConfirm)
{
    // 這裡應該創建並顯示確認對話框
    UE_LOG(LogTemp, Log, TEXT("Confirmation dialog: %s - %s"), *Title, *Message);
}

bool UMingPersonalUIManager::IsAnyPanelOpen() const
{
    for (const auto& ConfigPair : UIConfigMap)
    {
        if (ConfigPair.Value.bIsVisible)
        {
            return true;
        }
    }
    return false;
}

TArray<EPersonalUIType> UMingPersonalUIManager::GetVisiblePanels() const
{
    TArray<EPersonalUIType> VisiblePanels;
    
    for (const auto& ConfigPair : UIConfigMap)
    {
        if (ConfigPair.Value.bIsVisible)
        {
            VisiblePanels.Add(ConfigPair.Key);
        }
    }
    
    return VisiblePanels;
}

UUserWidget* UMingPersonalUIManager::CreateWidget(EPersonalUIType UIType)
{
    switch (UIType)
    {
        case EPersonalUIType::RelationshipPanel:
            return CreateRelationshipPanel();
        case EPersonalUIType::ReputationPanel:
            return CreateReputationPanel();
        case EPersonalUIType::DialoguePanel:
            return CreateDialoguePanel();
        case EPersonalUIType::QuestPanel:
            return CreateQuestPanel();
        case EPersonalUIType::AudioPanel:
            return CreateAudioPanel();
        case EPersonalUIType::MainDashboard:
            return CreateMainDashboard();
        case EPersonalUIType::CharacterDetails:
            return CreateCharacterDetails();
        case EPersonalUIType::RegionDetails:
            return CreateRegionDetails();
        case EPersonalUIType::SystemSettings:
            return CreateSystemSettings();
        default:
            return nullptr;
    }
}

void UMingPersonalUIManager::InitializeDefaultConfigs()
{
    // 關係面板配置
    FPersonalUIConfig RelationshipConfig;
    RelationshipConfig.UIType = EPersonalUIType::RelationshipPanel;
    RelationshipConfig.WidgetName = TEXT("RelationshipPanel");
    RelationshipConfig.WidgetPath = TEXT("/Game/UI/Personal/RelationshipPanel.RelationshipPanel");
    RelationshipConfig.OpenAnimation = EUIAnimationType::Slide;
    RelationshipConfig.CloseAnimation = EUIAnimationType::Fade;
    RelationshipConfig.AnimationDuration = 0.4f;
    AddUIConfig(RelationshipConfig);

    // 聲望面板配置
    FPersonalUIConfig ReputationConfig;
    ReputationConfig.UIType = EPersonalUIType::ReputationPanel;
    ReputationConfig.WidgetName = TEXT("ReputationPanel");
    ReputationConfig.WidgetPath = TEXT("/Game/UI/Personal/ReputationPanel.ReputationPanel");
    ReputationConfig.OpenAnimation = EUIAnimationType::Scale;
    ReputationConfig.CloseAnimation = EUIAnimationType::Scale;
    ReputationConfig.AnimationDuration = 0.3f;
    AddUIConfig(ReputationConfig);

    // 對話面板配置
    FPersonalUIConfig DialogueConfig;
    DialogueConfig.UIType = EPersonalUIType::DialoguePanel;
    DialogueConfig.WidgetName = TEXT("DialoguePanel");
    DialogueConfig.WidgetPath = TEXT("/Game/UI/Personal/DialoguePanel.DialoguePanel");
    DialogueConfig.OpenAnimation = EUIAnimationType::Fade;
    DialogueConfig.CloseAnimation = EUIAnimationType::Fade;
    DialogueConfig.AnimationDuration = 0.2f;
    AddUIConfig(DialogueConfig);

    // 任務面板配置
    FPersonalUIConfig QuestConfig;
    QuestConfig.UIType = EPersonalUIType::QuestPanel;
    QuestConfig.WidgetName = TEXT("QuestPanel");
    QuestConfig.WidgetPath = TEXT("/Game/UI/Personal/QuestPanel.QuestPanel");
    QuestConfig.OpenAnimation = EUIAnimationType::Slide;
    QuestConfig.CloseAnimation = EUIAnimationType::Slide;
    QuestConfig.AnimationDuration = 0.5f;
    AddUIConfig(QuestConfig);

    // 音頻面板配置
    FPersonalUIConfig AudioConfig;
    AudioConfig.UIType = EPersonalUIType::AudioPanel;
    AudioConfig.WidgetName = TEXT("AudioPanel");
    AudioConfig.WidgetPath = TEXT("/Game/UI/Personal/AudioPanel.AudioPanel");
    AudioConfig.OpenAnimation = EUIAnimationType::Bounce;
    AudioConfig.CloseAnimation = EUIAnimationType::Fade;
    AudioConfig.AnimationDuration = 0.3f;
    AddUIConfig(AudioConfig);

    // 主儀表板配置
    FPersonalUIConfig DashboardConfig;
    DashboardConfig.UIType = EPersonalUIType::MainDashboard;
    DashboardConfig.WidgetName = TEXT("MainDashboard");
    DashboardConfig.WidgetPath = TEXT("/Game/UI/Personal/MainDashboard.MainDashboard");
    DashboardConfig.OpenAnimation = EUIAnimationType::None;
    DashboardConfig.CloseAnimation = EUIAnimationType::None;
    DashboardConfig.AnimationDuration = 0.0f;
    AddUIConfig(DashboardConfig);
}

void UMingPersonalUIManager::SetupEventBindings()
{
    if (RelationshipManager)
    {
        // 綁定關係變化事件
        // RelationshipManager->OnRelationshipChanged.AddDynamic(this, &UMingPersonalUIManager::OnRelationshipChanged);
    }
    
    if (AudioManager)
    {
        // 綁定音頻事件
        // AudioManager->OnDialogueAudioPlayed.AddDynamic(this, &UMingPersonalUIManager::OnDialogueStarted);
    }
}

void UMingPersonalUIManager::OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason)
{
    UpdateRelationshipUI();
    
    // 顯示通知
    if (FMath::Abs(NewValue - OldValue) >= 10.0f)
    {
        FString Notification = FString::Printf(TEXT("與 %s 的關係發生了變化: %s"), *CharacterID, *Reason);
        ShowNotification(Notification);
    }
}

void UMingPersonalUIManager::OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason)
{
    UpdateReputationUI();
    
    // 顯示通知
    if (FMath::Abs(NewValue - OldValue) >= 50.0f)
    {
        FString Notification = FString::Printf(TEXT("在 %s 的聲望發生了變化: %s"), *RegionID, *Reason);
        ShowNotification(Notification);
    }
}

void UMingPersonalUIManager::OnDialogueStarted(const FString& CharacterID, EDialogueAudioType DialogueType)
{
    UpdateDialogueUI(CharacterID);
}

// Widget創建函數的實現
UUserWidget* UMingPersonalUIManager::CreateRelationshipPanel()
{
    // 這裡應該創建實際的Widget
    UE_LOG(LogTemp, Log, TEXT("Creating Relationship Panel"));
    return nullptr; // 暫時返回nullptr，實際實現時會創建真實的Widget
}

UUserWidget* UMingPersonalUIManager::CreateReputationPanel()
{
    UE_LOG(LogTemp, Log, TEXT("Creating Reputation Panel"));
    return nullptr;
}

UUserWidget* UMingPersonalUIManager::CreateDialoguePanel()
{
    UE_LOG(LogTemp, Log, TEXT("Creating Dialogue Panel"));
    return nullptr;
}

UUserWidget* UMingPersonalUIManager::CreateQuestPanel()
{
    UE_LOG(LogTemp, Log, TEXT("Creating Quest Panel"));
    return nullptr;
}

UUserWidget* UMingPersonalUIManager::CreateAudioPanel()
{
    UE_LOG(LogTemp, Log, TEXT("Creating Audio Panel"));
    return nullptr;
}

UUserWidget* UMingPersonalUIManager::CreateMainDashboard()
{
    UE_LOG(LogTemp, Log, TEXT("Creating Main Dashboard"));
    return nullptr;
}

UUserWidget* UMingPersonalUIManager::CreateCharacterDetails()
{
    UE_LOG(LogTemp, Log, TEXT("Creating Character Details"));
    return nullptr;
}

UUserWidget* UMingPersonalUIManager::CreateRegionDetails()
{
    UE_LOG(LogTemp, Log, TEXT("Creating Region Details"));
    return nullptr;
}

UUserWidget* UMingPersonalUIManager::CreateSystemSettings()
{
    UE_LOG(LogTemp, Log, TEXT("Creating System Settings"));
    return nullptr;
}

void UMingPersonalUIManager::PlayAnimation(UUserWidget* Widget, EUIAnimationType AnimationType, float Duration)
{
    if (!Widget)
    {
        return;
    }

    // 這裡應該實現實際的動畫播放邏輯
    switch (AnimationType)
    {
        case EUIAnimationType::Fade:
            UE_LOG(LogTemp, Log, TEXT("Playing fade animation for duration: %.2f"), Duration);
            break;
        case EUIAnimationType::Slide:
            UE_LOG(LogTemp, Log, TEXT("Playing slide animation for duration: %.2f"), Duration);
            break;
        case EUIAnimationType::Scale:
            UE_LOG(LogTemp, Log, TEXT("Playing scale animation for duration: %.2f"), Duration);
            break;
        case EUIAnimationType::Rotate:
            UE_LOG(LogTemp, Log, TEXT("Playing rotate animation for duration: %.2f"), Duration);
            break;
        case EUIAnimationType::Bounce:
            UE_LOG(LogTemp, Log, TEXT("Playing bounce animation for duration: %.2f"), Duration);
            break;
        case EUIAnimationType::None:
        default:
            break;
    }
}

void UMingPersonalUIManager::StopAnimation(UUserWidget* Widget)
{
    if (Widget)
    {
        UE_LOG(LogTemp, Log, TEXT("Stopping animation"));
    }
}
