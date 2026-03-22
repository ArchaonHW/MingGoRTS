#include "MingUISystem.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

UMingUISystem::UMingUISystem()
{
    WorldContext = GetWorld();
    HistoryIndex = -1;
    
    // Initialize default style
    CurrentStyle.PrimaryColor = FLinearColor(0.2f, 0.4f, 0.8f, 1.0f);
    CurrentStyle.SecondaryColor = FLinearColor(0.1f, 0.2f, 0.4f, 1.0f);
    CurrentStyle.AccentColor = FLinearColor(0.8f, 0.6f, 0.2f, 1.0f);
    CurrentStyle.TextColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
    CurrentStyle.BackgroundColor = FLinearColor(0.05f, 0.05f, 0.1f, 0.9f);
    CurrentStyle.BorderColor = FLinearColor(0.3f, 0.3f, 0.3f, 1.0f);
    CurrentStyle.BorderRadius = 8.0f;
    CurrentStyle.BorderThickness = 2.0f;
}

void UMingUISystem::InitializeUI()
{
    InitializeDefaultScreens();
    InitializeDefaultThemes();
    SetupScreenHistory();
    
    UE_LOG(LogTemp, Log, TEXT("UI System initialized with %d screens"), ScreenDataMap.Num());
}

void UMingUISystem::InitializeDefaultScreens()
{
    // Main Menu Screen
    FMingUIScreenData MainMenuData;
    MainMenuData.ScreenType = EMingUIScreen::MainMenu;
    MainMenuData.ScreenName = TEXT("主選單");
    MainMenuData.Description = TEXT("遊戲主選單界面");
    MainMenuData.bIsModal = true;
    MainMenuData.bDestroyOnClose = false;
    MainMenuData.OpenAnimation = EMingUIAnimationType::Fade;
    MainMenuData.CloseAnimation = EMingUIAnimationType::Fade;
    MainMenuData.AnimationDuration = 0.8f;
    ScreenDataMap.Add(MainMenuData.ScreenType, MainMenuData);
    
    // Campaign Map Screen
    FMingUIScreenData CampaignMapData;
    CampaignMapData.ScreenType = EMingUIScreen::CampaignMap;
    CampaignMapData.ScreenName = TEXT("戰役地圖");
    CampaignMapData.Description = TEXT("戰役選擇地圖界面");
    CampaignMapData.bIsModal = false;
    CampaignMapData.bDestroyOnClose = true;
    CampaignMapData.OpenAnimation = EMingUIAnimationType::Slide;
    CampaignMapData.CloseAnimation = EMingUIAnimationType::Slide;
    CampaignMapData.AnimationDuration = 0.6f;
    ScreenDataMap.Add(CampaignMapData.ScreenType, CampaignMapData);
    
    // Battle HUD Screen
    FMingUIScreenData BattleHUDData;
    BattleHUDData.ScreenType = EMingUIScreen::BattleHUD;
    BattleHUDData.ScreenName = TEXT("戰鬥界面");
    BattleHUDData.Description = TEXT("戰鬥時的HUD界面");
    BattleHUDData.bIsModal = false;
    BattleHUDData.bDestroyOnClose = true;
    BattleHUDData.OpenAnimation = EMingUIAnimationType::None;
    BattleHUDData.CloseAnimation = EMingUIAnimationType::None;
    BattleHUDData.AnimationDuration = 0.0f;
    ScreenDataMap.Add(BattleHUDData.ScreenType, BattleHUDData);
    
    // Character Panel Screen
    FMingUIScreenData CharacterPanelData;
    CharacterPanelData.ScreenType = EMingUIScreen::CharacterPanel;
    CharacterPanelData.ScreenName = TEXT("角色面板");
    CharacterPanelData.Description = TEXT("角色信息和管理界面");
    CharacterPanelData.bIsModal = false;
    CharacterPanelData.bDestroyOnClose = true;
    CharacterPanelData.OpenAnimation = EMingUIAnimationType::Scale;
    CharacterPanelData.CloseAnimation = EMingUIAnimationType::Scale;
    CharacterPanelData.AnimationDuration = 0.4f;
    ScreenDataMap.Add(CharacterPanelData.ScreenType, CharacterPanelData);
    
    // Research Tree Screen
    FMingUIScreenData ResearchTreeData;
    ResearchTreeData.ScreenType = EMingUIScreen::ResearchTree;
    ResearchTreeData.ScreenName = TEXT("科技樹");
    ResearchTreeData.Description = TEXT("科技研發樹界面");
    ResearchTreeData.bIsModal = false;
    ResearchTreeData.bDestroyOnClose = true;
    ResearchTreeData.OpenAnimation = EMingUIAnimationType::Fade;
    ResearchTreeData.CloseAnimation = EMingUIAnimationType::Fade;
    ResearchTreeData.AnimationDuration = 0.5f;
    ScreenDataMap.Add(ResearchTreeData.ScreenType, ResearchTreeData);
    
    // Settings Screen
    FMingUIScreenData SettingsData;
    SettingsData.ScreenType = EMingUIScreen::Settings;
    SettingsData.ScreenName = TEXT("設定");
    SettingsData.Description = TEXT("遊戲設定界面");
    SettingsData.bIsModal = true;
    SettingsData.bDestroyOnClose = true;
    SettingsData.OpenAnimation = EMingUIAnimationType::Slide;
    SettingsData.CloseAnimation = EMingUIAnimationType::Slide;
    SettingsData.AnimationDuration = 0.3f;
    ScreenDataMap.Add(SettingsData.ScreenType, SettingsData);
    
    // Loading Screen
    FMingUIScreenData LoadingData;
    LoadingData.ScreenType = EMingUIScreen::Loading;
    LoadingData.ScreenName = TEXT("載入中");
    LoadingData.Description = TEXT("載入畫面");
    LoadingData.bIsModal = true;
    LoadingData.bDestroyOnClose = true;
    LoadingData.OpenAnimation = EMingUIAnimationType::Fade;
    LoadingData.CloseAnimation = EMingUIAnimationType::Fade;
    LoadingData.AnimationDuration = 0.3f;
    ScreenDataMap.Add(LoadingData.ScreenType, LoadingData);
    
    // Victory Screen
    FMingUIScreenData VictoryData;
    VictoryData.ScreenType = EMingUIScreen::Victory;
    VictoryData.ScreenName = TEXT("勝利");
    VictoryData.Description = TEXT("勝利結局界面");
    VictoryData.bIsModal = true;
    VictoryData.bDestroyOnClose = true;
    VictoryData.OpenAnimation = EMingUIAnimationType::Bounce;
    VictoryData.CloseAnimation = EMingUIAnimationType::Fade;
    VictoryData.AnimationDuration = 1.0f;
    ScreenDataMap.Add(VictoryData.ScreenType, VictoryData);
    
    // Defeat Screen
    FMingUIScreenData DefeatData;
    DefeatData.ScreenType = EMingUIScreen::Defeat;
    DefeatData.ScreenName = TEXT("失敗");
    DefeatData.Description = TEXT("失敗結局界面");
    DefeatData.bIsModal = true;
    DefeatData.bDestroyOnClose = true;
    DefeatData.OpenAnimation = EMingUIAnimationType::Fade;
    DefeatData.CloseAnimation = EMingUIAnimationType::Fade;
    DefeatData.AnimationDuration = 0.8f;
    ScreenDataMap.Add(DefeatData.ScreenType, DefeatData);
    
    // Pause Screen
    FMingUIScreenData PauseData;
    PauseData.ScreenType = EMingUIScreen::Pause;
    PauseData.ScreenName = TEXT("暫停");
    PauseData.Description = TEXT("遊戲暫停界面");
    PauseData.bIsModal = true;
    PauseData.bDestroyOnClose = true;
    PauseData.OpenAnimation = EMingUIAnimationType::Scale;
    PauseData.CloseAnimation = EMingUIAnimationType::Scale;
    PauseData.AnimationDuration = 0.2f;
    ScreenDataMap.Add(PauseData.ScreenType, PauseData);
}

void UMingUISystem::InitializeDefaultThemes()
{
    // Republic Era Theme (Default)
    FMingUIStyle RepublicTheme;
    RepublicTheme.PrimaryColor = FLinearColor(0.2f, 0.4f, 0.8f, 1.0f); // Blue
    RepublicTheme.SecondaryColor = FLinearColor(0.1f, 0.2f, 0.4f, 1.0f);
    RepublicTheme.AccentColor = FLinearColor(0.8f, 0.6f, 0.2f, 1.0f); // Gold
    RepublicTheme.TextColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
    RepublicTheme.BackgroundColor = FLinearColor(0.05f, 0.05f, 0.1f, 0.9f);
    RepublicTheme.BorderColor = FLinearColor(0.3f, 0.3f, 0.3f, 1.0f);
    ThemeStyles.Add(TEXT("Republic"), RepublicTheme);
    
    // Imperial Theme
    FMingUIStyle ImperialTheme;
    ImperialTheme.PrimaryColor = FLinearColor(0.8f, 0.2f, 0.2f, 1.0f); // Red
    ImperialTheme.SecondaryColor = FLinearColor(0.4f, 0.1f, 0.1f, 1.0f);
    ImperialTheme.AccentColor = FLinearColor(0.9f, 0.7f, 0.3f, 1.0f); // Yellow
    ImperialTheme.TextColor = FLinearColor(1.0f, 0.95f, 0.8f, 1.0f);
    ImperialTheme.BackgroundColor = FLinearColor(0.1f, 0.05f, 0.05f, 0.9f);
    ImperialTheme.BorderColor = FLinearColor(0.6f, 0.3f, 0.3f, 1.0f);
    ThemeStyles.Add(TEXT("Imperial"), ImperialTheme);
    
    // Modern Theme
    FMingUIStyle ModernTheme;
    ModernTheme.PrimaryColor = FLinearColor(0.1f, 0.8f, 0.4f, 1.0f); // Green
    ModernTheme.SecondaryColor = FLinearColor(0.05f, 0.4f, 0.2f, 1.0f);
    ModernTheme.AccentColor = FLinearColor(0.2f, 0.6f, 0.9f, 1.0f); // Cyan
    ModernTheme.TextColor = FLinearColor(0.9f, 0.9f, 0.9f, 1.0f);
    ModernTheme.BackgroundColor = FLinearColor(0.02f, 0.02f, 0.02f, 0.95f);
    ModernTheme.BorderColor = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f);
    ThemeStyles.Add(TEXT("Modern"), ModernTheme);
}

void UMingUISystem::SetupScreenHistory()
{
    ScreenHistory.Empty();
    HistoryIndex = -1;
}

void UMingUISystem::ShowScreen(EMingUIScreen ScreenType)
{
    FMingUIScreenData* ScreenData = ScreenDataMap.Find(ScreenType);
    if (!ScreenData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Screen data not found for type: %d"), (int32)ScreenType);
        return;
    }
    
    // Check if screen is already visible
    if (IsScreenVisible(ScreenType))
    {
        UE_LOG(LogTemp, Warning, TEXT("Screen already visible: %s"), *ScreenData->ScreenName);
        return;
    }
    
    // Handle modal screens
    if (ScreenData->bIsModal)
    {
        // Hide all non-modal screens
        for (auto& Pair : ActiveWidgets)
        {
            const FMingUIScreenData* ExistingData = ScreenDataMap.Find(Pair.Key);
            if (ExistingData && !ExistingData->bIsModal)
            {
                HideScreen(Pair.Key);
            }
        }
    }
    
    // Create and show the widget
    UUserWidget* Widget = CreateWidget(ScreenType);
    if (Widget)
    {
        Widget->AddToViewport();
        
        // Play opening animation
        if (ScreenData->OpenAnimation != EMingUIAnimationType::None)
        {
            PlayAnimationInternal(Widget, ScreenData->OpenAnimation, ScreenData->AnimationDuration);
        }
        
        // Update screen stack
        UpdateScreenStack(ScreenType);
        
        // Trigger event
        OnScreenShown.Broadcast(ScreenType, Widget);
        OnUIScreenChanged.Broadcast(ScreenType);
        
        UE_LOG(LogTemp, Log, TEXT("Screen shown: %s"), *ScreenData->ScreenName);
    }
}

void UMingUISystem::HideScreen(EMingUIScreen ScreenType)
{
    UUserWidget* Widget = GetWidget(ScreenType);
    if (!Widget)
    {
        return;
    }
    
    FMingUIScreenData* ScreenData = ScreenDataMap.Find(ScreenType);
    if (ScreenData && ScreenData->CloseAnimation != EMingUIAnimationType::None)
    {
        // Play closing animation
        PlayAnimationInternal(Widget, ScreenData->CloseAnimation, ScreenData->AnimationDuration);
        
        // Schedule widget removal after animation
        FTimerHandle TimerHandle;
        if (WorldContext.IsValid())
        {
            WorldContext->GetTimerManager().SetTimer(
                TimerHandle,
                [this, ScreenType]()
                {
                    CloseScreen(ScreenType);
                },
                ScreenData->AnimationDuration,
                false
            );
        }
    }
    else
    {
        CloseScreen(ScreenType);
    }
}

void UMingUISystem::CloseScreen(EMingUIScreen ScreenType)
{
    UUserWidget* Widget = GetWidget(ScreenType);
    if (!Widget)
    {
        return;
    }
    
    // Trigger event before removal
    OnScreenHidden.Broadcast(ScreenType, Widget);
    
    // Remove from viewport
    Widget->RemoveFromParent();
    
    // Clean up
    CleanupAnimation(Widget);
    
    // Destroy if configured
    FMingUIScreenData* ScreenData = ScreenDataMap.Find(ScreenType);
    if (ScreenData && ScreenData->bDestroyOnClose)
    {
        DestroyWidgetInternal(ScreenType);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Screen closed: %s"), ScreenData ? *ScreenData->ScreenName : TEXT("Unknown"));
}

void UMingUISystem::CloseAllScreens()
{
    TArray<EMingUIScreen> ScreensToClose;
    for (auto& Pair : ActiveWidgets)
    {
        ScreensToClose.Add(Pair.Key);
    }
    
    for (EMingUIScreen ScreenType : ScreensToClose)
    {
        CloseScreen(ScreenType);
    }
}

void UMingUISystem::ShowModalScreen(EMingUIScreen ScreenType)
{
    ShowScreen(ScreenType);
}

bool UMingUISystem::IsScreenVisible(EMingUIScreen ScreenType) const
{
    return ActiveWidgets.Contains(ScreenType);
}

UUserWidget* UMingUISystem::GetCurrentScreen() const
{
    if (ScreenHistory.Num() > 0 && HistoryIndex >= 0 && HistoryIndex < ScreenHistory.Num())
    {
        return GetWidget(ScreenHistory[HistoryIndex]);
    }
    return nullptr;
}

TArray<UUserWidget*> UMingUISystem::GetVisibleScreens() const
{
    TArray<UUserWidget*> Result;
    for (auto& Pair : ActiveWidgets)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

void UMingUISystem::NavigateToScreen(EMingUIScreen ScreenType)
{
    AddToHistory(ScreenType);
    ShowScreen(ScreenType);
}

void UMingUISystem::GoBack()
{
    if (!CanGoBack())
    {
        return;
    }
    
    EMingUIScreen CurrentScreen = GetPreviousScreen();
    if (CurrentScreen != EMingUIScreen::MainMenu) // Don't go back from main menu
    {
        HideScreen(ScreenHistory[HistoryIndex]);
        HistoryIndex--;
        ShowScreen(ScreenHistory[HistoryIndex]);
    }
}

void UMingUISystem::GoForward()
{
    if (!CanGoForward())
    {
        return;
    }
    
    HideScreen(ScreenHistory[HistoryIndex]);
    HistoryIndex++;
    ShowScreen(ScreenHistory[HistoryIndex]);
}

bool UMingUISystem::CanGoBack() const
{
    return HistoryIndex > 0;
}

bool UMingUISystem::CanGoForward() const
{
    return HistoryIndex < ScreenHistory.Num() - 1;
}

void UMingUISystem::PlayScreenAnimation(EMingUIScreen ScreenType, EMingUIAnimationType AnimationType, float Duration)
{
    UUserWidget* Widget = GetWidget(ScreenType);
    if (Widget)
    {
        PlayAnimationInternal(Widget, AnimationType, Duration);
    }
}

void UMingUISystem::FadeScreen(EMingUIScreen ScreenType, float TargetAlpha, float Duration)
{
    UUserWidget* Widget = GetWidget(ScreenType);
    if (Widget)
    {
        // This would implement fade animation
        UE_LOG(LogTemp, Log, TEXT("Fading screen to alpha %.2f over %.1f seconds"), TargetAlpha, Duration);
    }
}

void UMingUISystem::SlideScreen(EMingUIScreen ScreenType, FVector2D Direction, float Duration)
{
    UUserWidget* Widget = GetWidget(ScreenType);
    if (Widget)
    {
        // This would implement slide animation
        UE_LOG(LogTemp, Log, TEXT("Sliding screen in direction %s over %.1f seconds"), *Direction.ToString(), Duration);
    }
}

void UMingUISystem::ScaleScreen(EMingUIScreen ScreenType, float Scale, float Duration)
{
    UUserWidget* Widget = GetWidget(ScreenType);
    if (Widget)
    {
        // This would implement scale animation
        UE_LOG(LogTemp, Log, TEXT("Scaling screen to %.2f over %.1f seconds"), Scale, Duration);
    }
}

void UMingUISystem::SetUIStyle(const FMingUIStyle& NewStyle)
{
    CurrentStyle = NewStyle;
    
    // Apply style to all visible widgets
    for (auto& Pair : ActiveWidgets)
    {
        // This would apply style to widget
        UE_LOG(LogTemp, Log, TEXT("Applied style to screen: %d"), (int32)Pair.Key);
    }
}

FMingUIStyle UMingUISystem::GetCurrentStyle() const
{
    return CurrentStyle;
}

void UMingUISystem::ApplyTheme(const FString& ThemeName)
{
    FMingUIStyle* Theme = ThemeStyles.Find(ThemeName);
    if (Theme)
    {
        SetUIStyle(*Theme);
        UE_LOG(LogTemp, Log, TEXT("Applied theme: %s"), *ThemeName);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Theme not found: %s"), *ThemeName);
    }
}

TArray<FString> UMingUISystem::GetAvailableThemes() const
{
    TArray<FString> Result;
    for (const auto& Pair : ThemeStyles)
    {
        Result.Add(Pair.Key);
    }
    return Result;
}

void UMingUISystem::RegisterScreen(const FMingUIScreenData& ScreenData)
{
    ScreenDataMap.Add(ScreenData.ScreenType, ScreenData);
    UE_LOG(LogTemp, Log, TEXT("Registered screen: %s"), *ScreenData.ScreenName);
}

FMingUIScreenData UMingUISystem::GetScreenData(EMingUIScreen ScreenType) const
{
    const FMingUIScreenData* Data = ScreenDataMap.Find(ScreenType);
    return Data ? *Data : FMingUIScreenData();
}

TArray<FMingUIScreenData> UMingUISystem::GetAllScreenData() const
{
    TArray<FMingUIScreenData> Result;
    for (const auto& Pair : ScreenDataMap)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

UUserWidget* UMingUISystem::CreateWidget(EMingUIScreen ScreenType)
{
    FMingUIScreenData* ScreenData = ScreenDataMap.Find(ScreenType);
    if (!ScreenData || !ScreenData->WidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot create widget - invalid screen data"));
        return nullptr;
    }
    
    return CreateWidgetInternal(*ScreenData);
}

void UMingUISystem::DestroyWidget(EMingUIScreen ScreenType)
{
    DestroyWidgetInternal(ScreenType);
}

UUserWidget* UMingUISystem::GetWidget(EMingUIScreen ScreenType) const
{
    const UUserWidget* const* Widget = ActiveWidgets.Find(ScreenType);
    return Widget ? *Widget : nullptr;
}

FString UMingUISystem::GetScreenName(EMingUIScreen ScreenType)
{
    switch (ScreenType)
    {
    case EMingUIScreen::MainMenu: return TEXT("主選單");
    case EMingUIScreen::CampaignMap: return TEXT("戰役地圖");
    case EMingUIScreen::BattleHUD: return TEXT("戰鬥界面");
    case EMingUIScreen::CharacterPanel: return TEXT("角色面板");
    case EMingUIScreen::ResearchTree: return TEXT("科技樹");
    case EMingUIScreen::Settings: return TEXT("設定");
    case EMingUIScreen::Loading: return TEXT("載入中");
    case EMingUIScreen::Victory: return TEXT("勝利");
    case EMingUIScreen::Defeat: return TEXT("失敗");
    case EMingUIScreen::Pause: return TEXT("暫停");
    default: return TEXT("未知");
    }
}

FString UMingUISystem::GetAnimationTypeName(EMingUIAnimationType AnimationType)
{
    switch (AnimationType)
    {
    case EMingUIAnimationType::Fade: return TEXT("淡入淡出");
    case EMingUIAnimationType::Slide: return TEXT("滑動");
    case EMingUIAnimationType::Scale: return TEXT("縮放");
    case EMingUIAnimationType::Rotate: return TEXT("旋轉");
    case EMingUIAnimationType::Bounce: return TEXT("彈跳");
    case EMingUIAnimationType::Elastic: return TEXT("彈性");
    default: return TEXT("無");
    }
}

FString UMingUISystem::SaveUISettings() const
{
    FString Result = TEXT("{\n");
    Result += TEXT("  \"current_theme\": \"Republic\",\n");
    Result += FString::Printf(TEXT("  \"screen_history_count\": %d,\n"), ScreenHistory.Num());
    Result += FString::Printf(TEXT("  \"history_index\": %d,\n"), HistoryIndex);
    Result += FString::Printf(TEXT("  \"active_screens\": %d\n"), ActiveWidgets.Num());
    Result += TEXT("}\n");
    
    return Result;
}

void UMingUISystem::LoadUISettings(const FString& JsonString)
{
    // Parse JSON and restore UI settings
    UE_LOG(LogTemp, Log, TEXT("Loading UI settings"));
}

void UMingUISystem::UpdateScreenStack(EMingUIScreen ScreenType)
{
    // Remove any future history if we're not at the end
    if (HistoryIndex < ScreenHistory.Num() - 1)
    {
        ScreenHistory.RemoveAt(HistoryIndex + 1, ScreenHistory.Num() - HistoryIndex - 1);
    }
    
    // Add current screen to history
    AddToHistory(ScreenType);
}

void UMingUISystem::PlayAnimationInternal(UUserWidget* Widget, EMingUIAnimationType AnimationType, float Duration)
{
    if (!Widget)
    {
        return;
    }
    
    // This would implement actual animation using UMG animation system
    UE_LOG(LogTemp, Log, TEXT("Playing animation: %s for %.1f seconds"), 
           *GetAnimationTypeName(AnimationType), Duration);
}

void UMingUISystem::CleanupAnimation(UUserWidget* Widget)
{
    // Clean up animation resources
    if (Widget)
    {
        UE_LOG(LogTemp, Verbose, TEXT("Cleaning up animation for widget"));
    }
}

UUserWidget* UMingUISystem::CreateWidgetInternal(const FMingUIScreenData& ScreenData)
{
    if (!WorldContext.IsValid())
    {
        return nullptr;
    }
    
    UUserWidget* Widget = CreateWidget<UUserWidget>(WorldContext.Get(), ScreenData.WidgetClass);
    if (Widget)
    {
        ActiveWidgets.Add(ScreenData.ScreenType, Widget);
        PositionWidget(Widget, ScreenData.ScreenType);
    }
    
    return Widget;
}

void UMingUISystem::DestroyWidgetInternal(EMingUIScreen ScreenType)
{
    UUserWidget* Widget = GetWidget(ScreenType);
    if (Widget)
    {
        Widget->RemoveFromParent();
        Widget->MarkPendingKill();
        ActiveWidgets.Remove(ScreenType);
    }
}

void UMingUISystem::PositionWidget(UUserWidget* Widget, EMingUIScreen ScreenType)
{
    if (!Widget)
    {
        return;
    }
    
    // Set widget position and size based on screen type
    switch (ScreenType)
    {
    case EMingUIScreen::MainMenu:
    case EMingUIScreen::CampaignMap:
    case EMingUIScreen::Victory:
    case EMingUIScreen::Defeat:
        // Full screen
        Widget->SetDesiredSizeInViewport(FVector2D(1920, 1080));
        break;
        
    case EMingUIScreen::Settings:
    case EMingUIScreen::Pause:
        // Centered modal
        Widget->SetDesiredSizeInViewport(FVector2D(800, 600));
        break;
        
    case EMingUIScreen::CharacterPanel:
    case EMingUIScreen::ResearchTree:
        // Side panel
        Widget->SetDesiredSizeInViewport(FVector2D(400, 800));
        break;
        
    case EMingUIScreen::BattleHUD:
        // HUD overlay
        Widget->SetDesiredSizeInViewport(FVector2D(1920, 1080));
        break;
        
    default:
        // Default size
        Widget->SetDesiredSizeInViewport(FVector2D(800, 600));
        break;
    }
}

void UMingUISystem::AddToHistory(EMingUIScreen ScreenType)
{
    // Remove duplicates
    ScreenHistory.Remove(ScreenType);
    
    // Add to end
    ScreenHistory.Add(ScreenType);
    HistoryIndex = ScreenHistory.Num() - 1;
    
    // Limit history size
    if (ScreenHistory.Num() > 20)
    {
        ScreenHistory.RemoveAt(0);
        HistoryIndex--;
    }
}

EMingUIScreen UMingUISystem::GetPreviousScreen() const
{
    if (HistoryIndex > 0)
    {
        return ScreenHistory[HistoryIndex - 1];
    }
    return EMingUIScreen::MainMenu; // Default to main menu
}

EMingUIScreen UMingUISystem::GetNextScreen() const
{
    if (HistoryIndex < ScreenHistory.Num() - 1)
    {
        return ScreenHistory[HistoryIndex + 1];
    }
    return EMingUIScreen::MainMenu; // Default to main menu
}
