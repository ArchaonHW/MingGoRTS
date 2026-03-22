#include "MingPersonalUIManager.h"
#include "MingRelationshipManager.h"
#include "MingAudioRelationshipManager.h"
#include "MingRepublicEraAudioThemes.h"
#include "MingRelationshipPanel.h"
#include "MingReputationPanel.h"
#include "MingDialoguePanel.h"
#include "MingQuestPanel.h"
#include "MingAudioPanel.h"
#include "MingMainDashboard.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

UMingPersonalUIManager::UMingPersonalUIManager()
{
    AnimationSpeed = 1.0f;
    ActivePanelType = EPersonalUIType::MainDashboard;
    bIsMainDashboardVisible = false;
}

void UMingPersonalUIManager::InitializeUIManager(UMingRelationshipManager* InRelationshipManager, 
                                               UMingAudioRelationshipManager* InAudioManager,
                                               UMingRepublicEraAudioThemes* InThemeManager)
{
    RelationshipManager = InRelationshipManager;
    AudioManager = InAudioManager;
    ThemeManager = InThemeManager;

    // 初始化默認配置
    InitializeDefaultConfigs();

    // 設置事件綁定
    SetupEventBindings();

    UE_LOG(LogTemp, Log, TEXT("個人系統UI管理器已初始化"));
}

void UMingPersonalUIManager::OpenUIPanel(EPersonalUIType UIType)
{
    if (IsWidgetVisible(UIType))
    {
        UE_LOG(LogTemp, Warning, TEXT("UI面板已經打開：%d"), (int32)UIType);
        return;
    }

    // 獲取或創建Widget
    UUserWidget* Widget = GetWidget(UIType);
    if (!Widget)
    {
        Widget = CreateWidget(UIType);
        if (!Widget)
        {
            UE_LOG(LogTemp, Error, TEXT("無法創建UI面板：%d"), (int32)UIType);
            return;
        }
        WidgetMap.Add(UIType, Widget);
    }

    // 設置為可見
    SetWidgetVisibility(UIType, true);

    // 播放打開動畫
    PlayOpenAnimation(UIType);

    // 更新當前活動面板
    ActivePanelType = UIType;

    // 觸發事件
    OnUIPanelOpened.Broadcast(UIType, Widget);
    OnUIStateChanged.Broadcast(true);

    // 根據面板類型更新數據
    switch (UIType)
    {
        case EPersonalUIType::RelationshipPanel:
            UpdateRelationshipUI();
            break;
        case EPersonalUIType::ReputationPanel:
            UpdateReputationUI();
            break;
        case EPersonalUIType::DialoguePanel:
            // 對話面板需要傳入角色ID
            break;
        case EPersonalUIType::QuestPanel:
            UpdateQuestUI();
            break;
        case EPersonalUIType::AudioPanel:
            UpdateAudioUI();
            break;
        case EPersonalUIType::MainDashboard:
            RefreshDashboard();
            break;
        default:
            break;
    }

    UE_LOG(LogTemp, Log, TEXT("已打開UI面板：%d"), (int32)UIType);
}

void UMingPersonalUIManager::CloseUIPanel(EPersonalUIType UIType)
{
    if (!IsWidgetVisible(UIType))
    {
        UE_LOG(LogTemp, Warning, TEXT("UI面板未打開：%d"), (int32)UIType);
        return;
    }

    UUserWidget* Widget = GetWidget(UIType);
    if (!Widget)
    {
        return;
    }

    // 播放關閉動畫
    PlayCloseAnimation(UIType);

    // 設置為不可見
    SetWidgetVisibility(UIType, false);

    // 觸發事件
    OnUIPanelClosed.Broadcast(UIType, Widget);
    OnUIStateChanged.Broadcast(IsAnyPanelOpen());

    UE_LOG(LogTemp, Log, TEXT("已關閉UI面板：%d"), (int32)UIType);
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
    for (EPersonalUIType UIType : VisiblePanels)
    {
        CloseUIPanel(UIType);
    }

    // 隱藏主儀表板
    if (bIsMainDashboardVisible)
    {
        HideMainDashboard();
    }

    UE_LOG(LogTemp, Log, TEXT("已關閉所有UI面板"));
}

void UMingPersonalUIManager::ShowMainDashboard()
{
    if (bIsMainDashboardVisible)
    {
        return;
    }

    UUserWidget* Dashboard = GetWidget(EPersonalUIType::MainDashboard);
    if (!Dashboard)
    {
        Dashboard = CreateWidget(EPersonalUIType::MainDashboard);
        if (Dashboard)
        {
            WidgetMap.Add(EPersonalUIType::MainDashboard, Dashboard);
        }
    }

    if (Dashboard)
    {
        SetWidgetVisibility(EPersonalUIType::MainDashboard, true);
        PlayOpenAnimation(EPersonalUIType::MainDashboard);
        bIsMainDashboardVisible = true;
        RefreshDashboard();
    }
}

void UMingPersonalUIManager::HideMainDashboard()
{
    if (!bIsMainDashboardVisible)
    {
        return;
    }

    UUserWidget* Dashboard = GetWidget(EPersonalUIType::MainDashboard);
    if (Dashboard)
    {
        PlayCloseAnimation(EPersonalUIType::MainDashboard);
        SetWidgetVisibility(EPersonalUIType::MainDashboard, false);
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
    UUserWidget* Widget = GetWidget(UIType);
    if (Widget)
    {
        FPersonalUIConfig Config = GetUIConfig(UIType);
        return Widget->IsVisible() && Config.bIsVisible;
    }
    return false;
}

void UMingPersonalUIManager::SetWidgetVisibility(EPersonalUIType UIType, bool bVisible)
{
    UUserWidget* Widget = GetWidget(UIType);
    if (Widget)
    {
        Widget->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        
        // 更新配置
        if (UIConfigMap.Contains(UIType))
        {
            UIConfigMap[UIType].bIsVisible = bVisible;
        }
    }
}

void UMingPersonalUIManager::AddUIConfig(const FPersonalUIConfig& Config)
{
    UIConfigMap.Add(Config.UIType, Config);
    UE_LOG(LogTemp, Log, TEXT("已添加UI配置：%s"), *Config.WidgetName);
}

void UMingPersonalUIManager::RemoveUIConfig(EPersonalUIType UIType)
{
    if (UIConfigMap.Contains(UIType))
    {
        UIConfigMap.Remove(UIType);
        UE_LOG(LogTemp, Log, TEXT("已移除UI配置：%d"), (int32)UIType);
    }
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
    UUserWidget* Widget = GetWidget(UIType);
    if (!Widget)
    {
        return;
    }

    FPersonalUIConfig Config = GetUIConfig(UIType);
    PlayAnimation(Widget, Config.OpenAnimation, Config.AnimationDuration);
}

void UMingPersonalUIManager::PlayCloseAnimation(EPersonalUIType UIType)
{
    UUserWidget* Widget = GetWidget(UIType);
    if (!Widget)
    {
        return;
    }

    FPersonalUIConfig Config = GetUIConfig(UIType);
    PlayAnimation(Widget, Config.CloseAnimation, Config.AnimationDuration);
}

void UMingPersonalUIManager::SetAnimationSpeed(float Speed)
{
    AnimationSpeed = FMath::Clamp(Speed, 0.1f, 3.0f);
    UE_LOG(LogTemp, Log, TEXT("動畫速度已設置為：%.2f"), AnimationSpeed);
}

void UMingPersonalUIManager::UpdateRelationshipUI()
{
    UUserWidget* Widget = GetWidget(EPersonalUIType::RelationshipPanel);
    if (!Widget || !RelationshipManager)
    {
        return;
    }

    // 獲取關係面板並更新數據
    UMingRelationshipPanel* RelationshipPanel = Cast<UMingRelationshipPanel>(Widget);
    if (RelationshipPanel)
    {
        RelationshipPanel->UpdateRelationshipData(RelationshipManager);
        UE_LOG(LogTemp, Log, TEXT("關係UI已更新"));
    }
}

void UMingPersonalUIManager::UpdateReputationUI()
{
    UUserWidget* Widget = GetWidget(EPersonalUIType::ReputationPanel);
    if (!Widget || !RelationshipManager)
    {
        return;
    }

    // 獲取聲望面板並更新數據
    UMingReputationPanel* ReputationPanel = Cast<UMingReputationPanel>(Widget);
    if (ReputationPanel)
    {
        ReputationPanel->UpdateReputationData(RelationshipManager);
        UE_LOG(LogTemp, Log, TEXT("聲望UI已更新"));
    }
}

void UMingPersonalUIManager::UpdateDialogueUI(const FString& CharacterID)
{
    UUserWidget* Widget = GetWidget(EPersonalUIType::DialoguePanel);
    if (!Widget)
    {
        return;
    }

    // 獲取對話面板並更新數據
    UMingDialoguePanel* DialoguePanel = Cast<UMingDialoguePanel>(Widget);
    if (DialoguePanel)
    {
        DialoguePanel->UpdateDialogueData(CharacterID);
        UE_LOG(LogTemp, Log, TEXT("對話UI已更新：%s"), *CharacterID);
    }
}

void UMingPersonalUIManager::UpdateQuestUI()
{
    UUserWidget* Widget = GetWidget(EPersonalUIType::QuestPanel);
    if (!Widget)
    {
        return;
    }

    // 獲取任務面板並更新數據
    UMingQuestPanel* QuestPanel = Cast<UMingQuestPanel>(Widget);
    if (QuestPanel)
    {
        QuestPanel->UpdateQuestData();
        UE_LOG(LogTemp, Log, TEXT("任務UI已更新"));
    }
}

void UMingPersonalUIManager::UpdateAudioUI()
{
    UUserWidget* Widget = GetWidget(EPersonalUIType::AudioPanel);
    if (!Widget || !AudioManager || !ThemeManager)
    {
        return;
    }

    // 獲取音頻面板並更新數據
    UMingAudioPanel* AudioPanel = Cast<UMingAudioPanel>(Widget);
    if (AudioPanel)
    {
        AudioPanel->UpdateAudioData(AudioManager, ThemeManager);
        UE_LOG(LogTemp, Log, TEXT("音頻UI已更新"));
    }
}

void UMingPersonalUIManager::RefreshDashboard()
{
    UUserWidget* Widget = GetWidget(EPersonalUIType::MainDashboard);
    if (!Widget)
    {
        return;
    }

    // 獲取主儀表板並刷新數據
    UMingMainDashboard* Dashboard = Cast<UMingMainDashboard>(Widget);
    if (Dashboard)
    {
        Dashboard->RefreshDashboard(RelationshipManager, AudioManager, ThemeManager);
        UE_LOG(LogTemp, Log, TEXT("主儀表板已刷新"));
    }
}

void UMingPersonalUIManager::ShowNotification(const FString& Message, float Duration)
{
    // 這裡應該顯示通知UI
    // 暫時使用引擎日誌
    UE_LOG(LogTemp, Log, TEXT("通知：%s"), *Message);
    
    // TODO: 實現通知UI顯示邏輯
}

void UMingPersonalUIManager::ShowConfirmationDialog(const FString& Title, const FString& Message, const FString& OnConfirm)
{
    // 這裡應該顯示確認對話框
    UE_LOG(LogTemp, Log, TEXT("確認對話框 - 標題：%s，消息：%s"), *Title, *Message);
    
    // TODO: 實現確認對話框顯示邏輯
}

bool UMingPersonalUIManager::IsAnyPanelOpen() const
{
    for (const auto& WidgetPair : WidgetMap)
    {
        if (WidgetPair.Value && WidgetPair.Value->IsVisible())
        {
            return true;
        }
    }
    return bIsMainDashboardVisible;
}

TArray<EPersonalUIType> UMingPersonalUIManager::GetVisiblePanels() const
{
    TArray<EPersonalUIType> VisiblePanels;
    
    for (const auto& WidgetPair : WidgetMap)
    {
        if (WidgetPair.Value && WidgetPair.Value->IsVisible())
        {
            VisiblePanels.Add(WidgetPair.Key);
        }
    }
    
    return VisiblePanels;
}

// 私有輔助函數
UUserWidget* UMingPersonalUIManager::CreateWidget(EPersonalUIType UIType)
{
    UUserWidget* NewWidget = nullptr;
    
    switch (UIType)
    {
        case EPersonalUIType::RelationshipPanel:
            NewWidget = CreateRelationshipPanel();
            break;
        case EPersonalUIType::ReputationPanel:
            NewWidget = CreateReputationPanel();
            break;
        case EPersonalUIType::DialoguePanel:
            NewWidget = CreateDialoguePanel();
            break;
        case EPersonalUIType::QuestPanel:
            NewWidget = CreateQuestPanel();
            break;
        case EPersonalUIType::AudioPanel:
            NewWidget = CreateAudioPanel();
            break;
        case EPersonalUIType::MainDashboard:
            NewWidget = CreateMainDashboard();
            break;
        case EPersonalUIType::CharacterDetails:
            NewWidget = CreateCharacterDetails();
            break;
        case EPersonalUIType::RegionDetails:
            NewWidget = CreateRegionDetails();
            break;
        case EPersonalUIType::SystemSettings:
            NewWidget = CreateSystemSettings();
            break;
        default:
            UE_LOG(LogTemp, Warning, TEXT("未知的UI類型：%d"), (int32)UIType);
            break;
    }
    
    return NewWidget;
}

void UMingPersonalUIManager::InitializeDefaultConfigs()
{
    // 為每種UI類型創建默認配置
    TArray<EPersonalUIType> UITypes = {
        EPersonalUIType::RelationshipPanel,
        EPersonalUIType::ReputationPanel,
        EPersonalUIType::DialoguePanel,
        EPersonalUIType::QuestPanel,
        EPersonalUIType::AudioPanel,
        EPersonalUIType::MainDashboard,
        EPersonalUIType::CharacterDetails,
        EPersonalUIType::RegionDetails,
        EPersonalUIType::SystemSettings
    };
    
    for (EPersonalUIType UIType : UITypes)
    {
        if (!UIConfigMap.Contains(UIType))
        {
            FPersonalUIConfig Config;
            Config.UIType = UIType;
            Config.WidgetName = GetWidgetName(UIType);
            Config.WidgetPath = GetWidgetPath(UIType);
            Config.bIsVisible = false;
            Config.bIsInteractive = true;
            Config.OpenAnimation = EUIAnimationType::Fade;
            Config.CloseAnimation = EUIAnimationType::Fade;
            Config.AnimationDuration = 0.3f;
            
            UIConfigMap.Add(UIType, Config);
        }
    }
}

void UMingPersonalUIManager::SetupEventBindings()
{
    if (RelationshipManager)
    {
        // 綁定關係變化事件
        RelationshipManager->OnRelationshipChanged.AddDynamic(this, &UMingPersonalUIManager::OnRelationshipChanged);
        RelationshipManager->OnReputationChanged.AddDynamic(this, &UMingPersonalUIManager::OnReputationChanged);
    }
    
    if (AudioManager)
    {
        // 綁定音頻事件
        AudioManager->OnDialogueStarted.AddDynamic(this, &UMingPersonalUIManager::OnDialogueStarted);
    }
}

void UMingPersonalUIManager::OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason)
{
    // 當關係發生變化時更新UI
    if (IsWidgetVisible(EPersonalUIType::RelationshipPanel))
    {
        UpdateRelationshipUI();
    }
    
    if (IsWidgetVisible(EPersonalUIType::MainDashboard))
    {
        RefreshDashboard();
    }
}

void UMingPersonalUIManager::OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason)
{
    // 當聲望發生變化時更新UI
    if (IsWidgetVisible(EPersonalUIType::ReputationPanel))
    {
        UpdateReputationUI();
    }
    
    if (IsWidgetVisible(EPersonalUIType::MainDashboard))
    {
        RefreshDashboard();
    }
}

void UMingPersonalUIManager::OnDialogueStarted(const FString& CharacterID, EDialogueAudioType DialogueType)
{
    // 當對話開始時打開對話面板
    UpdateDialogueUI(CharacterID);
    OpenUIPanel(EPersonalUIType::DialoguePanel);
}

// Widget創建函數
UUserWidget* UMingPersonalUIManager::CreateRelationshipPanel()
{
    // 這裡應該從藍圖或C++類創建關係面板
    // 暫時返回nullptr，需要在實際實現中創建
    UE_LOG(LogTemp, Log, TEXT("創建關係面板"));
    return nullptr;
}

UUserWidget* UMingPersonalUIManager::CreateReputationPanel()
{
    UE_LOG(LogTemp, Log, TEXT("創建聲望面板"));
    return nullptr;
}

UUserWidget* UMingPersonalUIManager::CreateDialoguePanel()
{
    UE_LOG(LogTemp, Log, TEXT("創建對話面板"));
    return nullptr;
}

UUserWidget* UMingPersonalUIManager::CreateQuestPanel()
{
    UE_LOG(LogTemp, Log, TEXT("創建任務面板"));
    return nullptr;
}

UUserWidget* UMingPersonalUIManager::CreateAudioPanel()
{
    UE_LOG(LogTemp, Log, TEXT("創建音頻面板"));
    return nullptr;
}

UUserWidget* UMingPersonalUIManager::CreateMainDashboard()
{
    UE_LOG(LogTemp, Log, TEXT("創建主儀表板"));
    return nullptr;
}

UUserWidget* UMingPersonalUIManager::CreateCharacterDetails()
{
    UE_LOG(LogTemp, Log, TEXT("創建角色詳情"));
    return nullptr;
}

UUserWidget* UMingPersonalUIManager::CreateRegionDetails()
{
    UE_LOG(LogTemp, Log, TEXT("創建地區詳情"));
    return nullptr;
}

UUserWidget* UMingPersonalUIManager::CreateSystemSettings()
{
    UE_LOG(LogTemp, Log, TEXT("創建系統設置"));
    return nullptr;
}

// 動畫輔助函數
void UMingPersonalUIManager::PlayAnimation(UUserWidget* Widget, EUIAnimationType AnimationType, float Duration)
{
    if (!Widget)
    {
        return;
    }

    // 這裡應該實現實際的動畫播放邏輯
    // 暫時使用日誌記錄
    FString AnimationName;
    switch (AnimationType)
    {
        case EUIAnimationType::Fade:
            AnimationName = TEXT("淡入淡出");
            break;
        case EUIAnimationType::Slide:
            AnimationName = TEXT("滑動");
            break;
        case EUIAnimationType::Scale:
            AnimationName = TEXT("縮放");
            break;
        case EUIAnimationType::Rotate:
            AnimationName = TEXT("旋轉");
            break;
        case EUIAnimationType::Bounce:
            AnimationName = TEXT("彈跳");
            break;
        default:
            AnimationName = TEXT("無動畫");
            break;
    }
    
    UE_LOG(LogTemp, Log, TEXT("播放動畫：%s，持續時間：%.2f秒"), *AnimationName, Duration * AnimationSpeed);
}

void UMingPersonalUIManager::StopAnimation(UUserWidget* Widget)
{
    if (!Widget)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("停止動畫"));
}

// 輔助函數
FString UMingPersonalUIManager::GetWidgetName(EPersonalUIType UIType)
{
    switch (UIType)
    {
        case EPersonalUIType::RelationshipPanel:
            return TEXT("RelationshipPanel");
        case EPersonalUIType::ReputationPanel:
            return TEXT("ReputationPanel");
        case EPersonalUIType::DialoguePanel:
            return TEXT("DialoguePanel");
        case EPersonalUIType::QuestPanel:
            return TEXT("QuestPanel");
        case EPersonalUIType::AudioPanel:
            return TEXT("AudioPanel");
        case EPersonalUIType::MainDashboard:
            return TEXT("MainDashboard");
        case EPersonalUIType::CharacterDetails:
            return TEXT("CharacterDetails");
        case EPersonalUIType::RegionDetails:
            return TEXT("RegionDetails");
        case EPersonalUIType::SystemSettings:
            return TEXT("SystemSettings");
        default:
            return TEXT("UnknownWidget");
    }
}

FString UMingPersonalUIManager::GetWidgetPath(EPersonalUIType UIType)
{
    // 這裡應該返回Widget的藍圖路徑
    switch (UIType)
    {
        case EPersonalUIType::RelationshipPanel:
            return TEXT("/Game/UI/Relationship/RelationshipUI.RelationshipUI_C");
        case EPersonalUIType::ReputationPanel:
            return TEXT("/Game/UI/Reputation/ReputationUI.ReputationUI_C");
        case EPersonalUIType::DialoguePanel:
            return TEXT("/Game/UI/Dialogue/DialogueUI.DialogueUI_C");
        case EPersonalUIType::QuestPanel:
            return TEXT("/Game/UI/Quest/QuestUI.QuestUI_C");
        case EPersonalUIType::AudioPanel:
            return TEXT("/Game/UI/Audio/AudioUI.AudioUI_C");
        case EPersonalUIType::MainDashboard:
            return TEXT("/Game/UI/Dashboard/DashboardUI.DashboardUI_C");
        case EPersonalUIType::CharacterDetails:
            return TEXT("/Game/UI/Character/CharacterDetailsUI.CharacterDetailsUI_C");
        case EPersonalUIType::RegionDetails:
            return TEXT("/Game/UI/Region/RegionDetailsUI.RegionDetailsUI_C");
        case EPersonalUIType::SystemSettings:
            return TEXT("/Game/UI/Settings/SettingsUI.SettingsUI_C");
        default:
            return TEXT("");
    }
}
