#include "MingMainHUD.h"
#include "Relationship/MingRelationshipWidget.h"
#include "Reputation/MingReputationWidget.h"
#include "Dialogue/MingDialogueWidget.h"
#include "Quest/MingQuestWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "MingPersonal/Public/MingPersonalManager.h"
#include "MingPersonal/Public/Relationship/MingRelationshipManager.h"
#include "MingPersonal/Public/Reputation/MingReputationManager.h"
#include "MingAudio/Public/MingMetaSoundsSystem.h"

AMingMainHUD::AMingMainHUD(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , bIsInitialized(false)
    , CurrentMode(EMingHUDMode::Gameplay)
    , PreviousMode(EMingHUDMode::Gameplay)
    , NotificationTimer(0.0f)
    , RelationshipWidget(nullptr)
    , ReputationWidget(nullptr)
    , DialogueWidget(nullptr)
    , QuestWidget(nullptr)
    , QuickRelationshipWidget(nullptr)
    , QuickReputationWidget(nullptr)
{
}

void AMingMainHUD::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize HUD
    if (APlayerController* PC = GetOwningPlayerController())
    {
        InitializeHUD(PC);
    }
}

void AMingMainHUD::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Update notification timer
    if (NotificationTimer > 0.0f)
    {
        NotificationTimer -= DeltaTime;
        if (NotificationTimer <= 0.0f)
        {
            // Hide notification
        }
    }
    
    // Update quick access displays
    UpdateQuickAccessDisplays();
}

void AMingMainHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UnbindFromPersonalManager();
    Super::EndPlay(EndPlayReason);
}

void AMingMainHUD::InitializeHUD(APlayerController* PC)
{
    if (!PC || bIsInitialized)
    {
        return;
    }
    
    PlayerController = PC;
    
    // Find Personal Manager
    if (UWorld* World = GetWorld())
    {
        // This would find the personal manager from the game mode or game state
        // PersonalManager = ...;
    }
    
    // Initialize widgets
    InitializeWidgets();
    
    // Setup input bindings
    SetupInputBindings();
    
    // Bind to personal manager events
    BindToPersonalManager();
    
    bIsInitialized = true;
}

void AMingMainHUD::InitializeWidgets()
{
    // Create main widgets (but keep them hidden initially)
    if (RelationshipWidgetClass)
    {
        RelationshipWidget = CreateWidget<UMingRelationshipWidget>(GetOwningPlayerController(), RelationshipWidgetClass);
        if (RelationshipWidget)
        {
            RelationshipWidget->AddToViewport(1);
            RelationshipWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    
    if (ReputationWidgetClass)
    {
        ReputationWidget = CreateWidget<UMingReputationWidget>(GetOwningPlayerController(), ReputationWidgetClass);
        if (ReputationWidget)
        {
            ReputationWidget->AddToViewport(1);
            ReputationWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    
    if (DialogueWidgetClass)
    {
        DialogueWidget = CreateWidget<UMingDialogueWidget>(GetOwningPlayerController(), DialogueWidgetClass);
        if (DialogueWidget)
        {
            DialogueWidget->AddToViewport(2);
            DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    
    if (QuestWidgetClass)
    {
        QuestWidget = CreateWidget<UMingQuestWidget>(GetOwningPlayerController(), QuestWidgetClass);
        if (QuestWidget)
        {
            QuestWidget->AddToViewport(1);
            QuestWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    
    // Create quick access widgets
    if (QuickRelationshipWidgetClass)
    {
        QuickRelationshipWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), QuickRelationshipWidgetClass);
        if (QuickRelationshipWidget)
        {
            QuickRelationshipWidget->AddToViewport(0);
            QuickRelationshipWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    
    if (QuickReputationWidgetClass)
    {
        QuickReputationWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), QuickReputationWidgetClass);
        if (QuickReputationWidget)
        {
            QuickReputationWidget->AddToViewport(0);
            QuickReputationWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void AMingMainHUD::SetupInputBindings()
{
    // Input bindings would be set up here
    // This would bind keys to the Handle*Input functions
}

void AMingMainHUD::BindToPersonalManager()
{
    if (!PersonalManager.IsValid())
    {
        return;
    }
    
    // Subscribe to events from personal manager
    // These would be actual event subscriptions in the real implementation
}

void AMingMainHUD::UnbindFromPersonalManager()
{
    if (!PersonalManager.IsValid())
    {
        return;
    }
    
    // Unsubscribe from events
}

void AMingMainHUD::SetHUDMode(EMingHUDMode NewMode)
{
    if (CurrentMode != NewMode)
    {
        PreviousMode = CurrentMode;
        CurrentMode = NewMode;
        
        UpdateHUDForMode();
        OnHUDModeChanged.Broadcast(NewMode);
    }
}

void AMingMainHUD::SetPreviousMode()
{
    SetHUDMode(PreviousMode);
}

void AMingMainHUD::UpdateHUDForMode()
{
    switch (CurrentMode)
    {
    case EMingHUDMode::Gameplay:
        HideAllPanels();
        ShowQuickRelationshipBar();
        ShowQuickReputationBar();
        break;
        
    case EMingHUDMode::Dialogue:
        HideAllPanels();
        if (DialogueWidget)
        {
            DialogueWidget->SetVisibility(ESlateVisibility::Visible);
        }
        break;
        
    case EMingHUDMode::Menu:
    case EMingHUDMode::Pause:
        // Show menu/pause UI
        break;
        
    case EMingHUDMode::Cinematic:
        HideAllPanels();
        HideQuickRelationshipBar();
        HideQuickReputationBar();
        break;
        
    default:
        break;
    }
}

void AMingMainHUD::ShowWidget(TSubclassOf<UUserWidget> WidgetClass, EMingHUDLayer Layer)
{
    UUserWidget* Widget = CreateWidgetOnLayer(WidgetClass, Layer);
    if (Widget)
    {
        Widget->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMingMainHUD::HideWidget(TSubclassOf<UUserWidget> WidgetClass)
{
    for (auto& Pair : ActiveWidgets)
    {
        if (Pair.Key == WidgetClass && Pair.Value)
        {
            Pair.Value->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void AMingMainHUD::HideAllWidgets()
{
    for (auto& Pair : ActiveWidgets)
    {
        if (Pair.Value)
        {
            Pair.Value->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    
    CloseAllPanels();
}

bool AMingMainHUD::IsWidgetVisible(TSubclassOf<UUserWidget> WidgetClass) const
{
    for (const auto& Pair : ActiveWidgets)
    {
        if (Pair.Key == WidgetClass && Pair.Value)
        {
            return Pair.Value->GetVisibility() == ESlateVisibility::Visible;
        }
    }
    return false;
}

// Relationship Panel

void AMingMainHUD::ShowRelationshipPanel()
{
    if (RelationshipWidget && CanOpenPanel())
    {
        RelationshipWidget->SetVisibility(ESlateVisibility::Visible);
        RelationshipWidget->InitializeWidget(PersonalManager.IsValid() ? PersonalManager->GetRelationshipManager() : nullptr);
        PlayPanelOpenSound();
        OnPanelOpened.Broadcast();
    }
}

void AMingMainHUD::HideRelationshipPanel()
{
    if (RelationshipWidget)
    {
        RelationshipWidget->SetVisibility(ESlateVisibility::Collapsed);
        PlayPanelCloseSound();
        OnPanelClosed.Broadcast();
    }
}

void AMingMainHUD::ToggleRelationshipPanel()
{
    if (IsRelationshipPanelVisible())
    {
        HideRelationshipPanel();
    }
    else
    {
        ShowRelationshipPanel();
    }
}

void AMingMainHUD::OpenCharacterRelationship(FName CharacterID)
{
    ShowRelationshipPanel();
    if (RelationshipWidget)
    {
        RelationshipWidget->SelectCharacter(CharacterID);
    }
}

bool AMingMainHUD::IsRelationshipPanelVisible() const
{
    return RelationshipWidget && RelationshipWidget->GetVisibility() == ESlateVisibility::Visible;
}

// Reputation Panel

void AMingMainHUD::ShowReputationPanel()
{
    if (ReputationWidget && CanOpenPanel())
    {
        ReputationWidget->SetVisibility(ESlateVisibility::Visible);
        ReputationWidget->InitializeWidget(PersonalManager.IsValid() ? PersonalManager : nullptr);
        PlayPanelOpenSound();
        OnPanelOpened.Broadcast();
    }
}

void AMingMainHUD::HideReputationPanel()
{
    if (ReputationWidget)
    {
        ReputationWidget->SetVisibility(ESlateVisibility::Collapsed);
        PlayPanelCloseSound();
        OnPanelClosed.Broadcast();
    }
}

void AMingMainHUD::ToggleReputationPanel()
{
    if (IsReputationPanelVisible())
    {
        HideReputationPanel();
    }
    else
    {
        ShowReputationPanel();
    }
}

void AMingMainHUD::OpenRegionReputation(FName RegionID)
{
    ShowReputationPanel();
    if (ReputationWidget)
    {
        ReputationWidget->SelectRegion(RegionID);
    }
}

bool AMingMainHUD::IsReputationPanelVisible() const
{
    return ReputationWidget && ReputationWidget->GetVisibility() == ESlateVisibility::Visible;
}

// Dialogue System

void AMingMainHUD::StartDialogue(FName CharacterID)
{
    SetHUDMode(EMingHUDMode::Dialogue);
    
    if (DialogueWidget)
    {
        DialogueWidget->InitializeDialogue(CharacterID, PersonalManager.IsValid() ? PersonalManager->GetRelationshipManager() : nullptr);
        DialogueWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMingMainHUD::EndDialogue()
{
    if (DialogueWidget)
    {
        DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
    ReturnToGameplayMode();
}

void AMingMainHUD::ShowDialogueOptions(const TArray<FMingDialogueOption>& Options)
{
    if (DialogueWidget)
    {
        DialogueWidget->ShowOptions(Options);
    }
}

bool AMingMainHUD::IsDialogueActive() const
{
    return CurrentMode == EMingHUDMode::Dialogue && DialogueWidget && DialogueWidget->GetVisibility() == ESlateVisibility::Visible;
}

// Quest Panel

void AMingMainHUD::ShowQuestPanel()
{
    if (QuestWidget && CanOpenPanel())
    {
        QuestWidget->SetVisibility(ESlateVisibility::Visible);
        QuestWidget->InitializeWidget(PersonalManager.IsValid() ? PersonalManager : nullptr);
        QuestWidget->RefreshQuestList();
        PlayPanelOpenSound();
        OnPanelOpened.Broadcast();
    }
}

void AMingMainHUD::HideQuestPanel()
{
    if (QuestWidget)
    {
        QuestWidget->SetVisibility(ESlateVisibility::Collapsed);
        PlayPanelCloseSound();
        OnPanelClosed.Broadcast();
    }
}

void AMingMainHUD::ToggleQuestPanel()
{
    if (IsQuestPanelVisible())
    {
        HideQuestPanel();
    }
    else
    {
        ShowQuestPanel();
    }
}

void AMingMainHUD::ShowQuestDetails(FName QuestID)
{
    ShowQuestPanel();
    if (QuestWidget)
    {
        QuestWidget->SelectQuest(QuestID);
    }
}

void AMingMainHUD::ShowQuestNotification(FName QuestID, const FString& Message)
{
    ShowNotification(TEXT("Quest Update"), Message);
    PlayNotificationSound(2); // Quest notification sound
}

bool AMingMainHUD::IsQuestPanelVisible() const
{
    return QuestWidget && QuestWidget->GetVisibility() == ESlateVisibility::Visible;
}

// Audio Panel

void AMingMainHUD::ShowAudioPanel()
{
    PlayPanelOpenSound();
    OnPanelOpened.Broadcast();
}

void AMingMainHUD::HideAudioPanel()
{
    PlayPanelCloseSound();
    OnPanelClosed.Broadcast();
}

void AMingMainHUD::ToggleAudioPanel()
{
    // Toggle audio panel visibility
}

// Quick Access

void AMingMainHUD::ShowQuickRelationshipBar()
{
    if (QuickRelationshipWidget)
    {
        QuickRelationshipWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMingMainHUD::HideQuickRelationshipBar()
{
    if (QuickRelationshipWidget)
    {
        QuickRelationshipWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void AMingMainHUD::ShowQuickReputationBar()
{
    if (QuickReputationWidget)
    {
        QuickReputationWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMingMainHUD::HideQuickReputationBar()
{
    if (QuickReputationWidget)
    {
        QuickReputationWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void AMingMainHUD::UpdateQuickAccessDisplays()
{
    // Update the quick access widgets with current relationship/reputation data
}

// Notifications

void AMingMainHUD::ShowNotification(const FString& Title, const FString& Message, float Duration)
{
    // Create and show notification widget
    OnNotificationShown.Broadcast(Title, Message);
    NotificationTimer = Duration;
}

void AMingMainHUD::ShowRelationshipNotification(FName CharacterID, float OldValue, float NewValue)
{
    float Delta = NewValue - OldValue;
    FString Direction = Delta > 0 ? TEXT("increased") : TEXT("decreased");
    FString Message = FString::Printf(TEXT("Relationship with %s %s by %.0f"), 
        *CharacterID.ToString(), *Direction, FMath::Abs(Delta));
    
    ShowNotification(TEXT("Relationship Change"), Message);
    PlayNotificationSound(0); // Relationship sound
}

void AMingMainHUD::ShowReputationNotification(FName RegionID, float OldValue, float NewValue)
{
    float Delta = NewValue - OldValue;
    FString Direction = Delta > 0 ? TEXT("increased") : TEXT("decreased");
    FString Message = FString::Printf(TEXT("Reputation in %s %s by %.0f"),
        *RegionID.ToString(), *Direction, FMath::Abs(Delta));
    
    ShowNotification(TEXT("Reputation Change"), Message);
    PlayNotificationSound(1); // Reputation sound
}

void AMingMainHUD::ShowQuestUpdateNotification(FName QuestID, const FString& UpdateMessage)
{
    ShowNotification(TEXT("Quest Update"), UpdateMessage);
    PlayNotificationSound(2); // Quest sound
}

// Input Handling

void AMingMainHUD::HandleToggleRelationshipInput()
{
    ToggleRelationshipPanel();
}

void AMingMainHUD::HandleToggleReputationInput()
{
    ToggleReputationPanel();
}

void AMingMainHUD::HandleToggleQuestInput()
{
    ToggleQuestPanel();
}

void AMingMainHUD::HandleToggleAudioInput()
{
    ToggleAudioPanel();
}

void AMingMainHUD::HandlePauseInput()
{
    if (CurrentMode == EMingHUDMode::Pause)
    {
        SetPreviousMode();
    }
    else
    {
        SetHUDMode(EMingHUDMode::Pause);
    }
}

void AMingMainHUD::HandleBackInput()
{
    // Handle back button/input
    CloseAllPanels();
}

// Event Response

void AMingMainHUD::OnRelationshipChanged(FName CharacterID, float NewValue)
{
    // Show notification if significant change
    // Update relationship widget if visible
    if (RelationshipWidget && RelationshipWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        RelationshipWidget->HighlightRelationshipChange(CharacterID, NewValue);
    }
    
    // Update quick access
    UpdateQuickAccessDisplays();
}

void AMingMainHUD::OnReputationChanged(FName RegionID, float NewValue)
{
    // Show notification if significant change
    // Update reputation widget if visible
    if (ReputationWidget && ReputationWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        ReputationWidget->HighlightReputationChange(RegionID, NewValue);
    }
    
    // Update quick access
    UpdateQuickAccessDisplays();
}

void AMingMainHUD::OnQuestStatusChanged(FName QuestID, int32 NewStatus)
{
    // Update quest widget if visible
    if (QuestWidget && QuestWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        QuestWidget->RefreshQuestList();
    }
}

void AMingMainHUD::OnDialogueOptionSelected(int32 OptionID)
{
    // Handle dialogue option selection
}

// Audio Integration

void AMingMainHUD::PlayUISound(int32 SoundType)
{
    PlaySound(SoundType);
}

void AMingMainHUD::PlayPanelOpenSound()
{
    PlaySound(100); // Panel open sound ID
}

void AMingMainHUD::PlayPanelCloseSound()
{
    PlaySound(101); // Panel close sound ID
}

void AMingMainHUD::PlayNotificationSound(int32 NotificationType)
{
    int32 SoundID = 200 + NotificationType; // Base notification sound ID
    PlaySound(SoundID);
}

// Internal Functions

UUserWidget* AMingMainHUD::CreateWidgetOnLayer(TSubclassOf<UUserWidget> WidgetClass, EMingHUDLayer Layer)
{
    if (!WidgetClass || !GetOwningPlayerController())
    {
        return nullptr;
    }
    
    UUserWidget* Widget = CreateWidget<UUserWidget>(GetOwningPlayerController(), WidgetClass);
    if (Widget)
    {
        AddWidgetToLayer(Widget, Layer);
        ActiveWidgets.Add(TPair<TSubclassOf<UUserWidget>, UUserWidget*>(WidgetClass, Widget));
    }
    
    return Widget;
}

void AMingMainHUD::AddWidgetToLayer(UUserWidget* Widget, EMingHUDLayer Layer)
{
    if (!Widget)
    {
        return;
    }
    
    int32 ZOrder = static_cast<int32>(Layer) * 10;
    Widget->AddToViewport(ZOrder);
    
    if (!LayerWidgets.Contains(Layer))
    {
        LayerWidgets.Add(Layer, TArray<UUserWidget*>());
    }
    LayerWidgets[Layer].Add(Widget);
}

void AMingMainHUD::RemoveWidgetFromLayer(UUserWidget* Widget)
{
    if (!Widget)
    {
        return;
    }
    
    // Remove from layer tracking
    for (auto& Pair : LayerWidgets)
    {
        Pair.Value.Remove(Widget);
    }
}

void AMingMainHUD::ClearLayer(EMingHUDLayer Layer)
{
    if (LayerWidgets.Contains(Layer))
    {
        for (UUserWidget* Widget : LayerWidgets[Layer])
        {
            if (Widget)
            {
                Widget->RemoveFromParent();
            }
        }
        LayerWidgets[Layer].Empty();
    }
}

void AMingMainHUD::CloseAllPanels()
{
    HideRelationshipPanel();
    HideReputationPanel();
    HideQuestPanel();
    HideAudioPanel();
}

bool AMingMainHUD::CanOpenPanel() const
{
    // Check if we can open a panel based on current state
    // Don't allow opening panels during dialogue or cinematic
    return CurrentMode != EMingHUDMode::Dialogue && CurrentMode != EMingHUDMode::Cinematic;
}

void AMingMainHUD::ReturnToGameplayMode()
{
    SetHUDMode(EMingHUDMode::Gameplay);
}

void AMingMainHUD::PlaySound(int32 SoundType)
{
    // Play sound using audio system
    // This would use UMingMetaSoundsSystem to play the appropriate sound
}

// Debug

void AMingMainHUD::ShowDebugInfo(bool bShow)
{
    // Show or hide debug information
}

void AMingMainHUD::ToggleDebugInfo()
{
    // Toggle debug information visibility
}

FString AMingMainHUD::GetDebugInfo() const
{
    FString DebugInfo;
    DebugInfo += FString::Printf(TEXT("HUD Mode: %d\n"), static_cast<int32>(CurrentMode));
    DebugInfo += FString::Printf(TEXT("Relationship Panel: %s\n"), IsRelationshipPanelVisible() ? TEXT("Visible") : TEXT("Hidden"));
    DebugInfo += FString::Printf(TEXT("Reputation Panel: %s\n"), IsReputationPanelVisible() ? TEXT("Visible") : TEXT("Hidden"));
    DebugInfo += FString::Printf(TEXT("Quest Panel: %s\n"), IsQuestPanelVisible() ? TEXT("Visible") : TEXT("Hidden"));
    DebugInfo += FString::Printf(TEXT("Dialogue Active: %s\n"), IsDialogueActive() ? TEXT("Yes") : TEXT("No"));
    DebugInfo += FString::Printf(TEXT("Active Widgets: %d\n"), ActiveWidgets.Num());
    
    return DebugInfo;
}
