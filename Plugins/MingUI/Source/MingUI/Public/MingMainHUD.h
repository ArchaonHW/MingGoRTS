#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MingMainHUD.generated.h"

UENUM(BlueprintType)
enum class EMingHUDMode: uint8 {
    Gameplay UMETA(DisplayName = "Gameplay"),
    Dialogue UMETA(DisplayName = "Dialogue"),
    Menu UMETA(DisplayName = "Menu"),
    Inventory UMETA(DisplayName = "Inventory"),
    Map UMETA(DisplayName = "Map"),
    Pause UMETA(DisplayName = "Pause"),
    Cinematic UMETA(DisplayName = "Cinematic")
};

UENUM(BlueprintType)
enum class EMingHUDLayer: uint8 {
    Background UMETA(DisplayName = "Background"),
    MainContent UMETA(DisplayName = "Main Content"),
    Overlay UMETA(DisplayName = "Overlay"),
    Modal UMETA(DisplayName = "Modal"),
    Notification UMETA(DisplayName = "Notification"),
    Debug UMETA(DisplayName = "Debug")
};

/**
 * Main HUD for MingGoRTS Personal System
 * Integrates Relationship, Reputation, Dialogue, and Quest UI
 */
UCLASS(ClassGroup = (UI), Blueprintable)
class MINGUI_API AMingMainHUD : public AHUD
{
    GENERATED_BODY()

public:
    AMingMainHUD(const FObjectInitializer& ObjectInitializer);

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Ming HUD")
    void InitializeHUD(class APlayerController* PC);

    UFUNCTION(BlueprintPure, Category = "Ming HUD")
    bool IsInitialized() const { return bIsInitialized; }

    // HUD Mode Management
    UFUNCTION(BlueprintCallable, Category = "Ming HUD")
    void SetHUDMode(EMingHUDMode NewMode);

    UFUNCTION(BlueprintPure, Category = "Ming HUD")
    EMingHUDMode GetCurrentMode() const { return CurrentMode; }

    UFUNCTION(BlueprintCallable, Category = "Ming HUD")
    void SetPreviousMode();

    // Widget Visibility
    UFUNCTION(BlueprintCallable, Category = "Ming HUD")
    void ShowWidget(TSubclassOf<UUserWidget> WidgetClass, EMingHUDLayer Layer);

    UFUNCTION(BlueprintCallable, Category = "Ming HUD")
    void HideWidget(TSubclassOf<UUserWidget> WidgetClass);

    UFUNCTION(BlueprintCallable, Category = "Ming HUD")
    void HideAllWidgets();

    UFUNCTION(BlueprintPure, Category = "Ming HUD")
    bool IsWidgetVisible(TSubclassOf<UUserWidget> WidgetClass) const;

    // Relationship Panel
    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void ShowRelationshipPanel();

    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void HideRelationshipPanel();

    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void ToggleRelationshipPanel();

    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void OpenCharacterRelationship(FName CharacterID);

    UFUNCTION(BlueprintPure, Category = "Relationship UI")
    bool IsRelationshipPanelVisible() const;

    // Reputation Panel
    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void ShowReputationPanel();

    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void HideReputationPanel();

    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void ToggleReputationPanel();

    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void OpenRegionReputation(FName RegionID);

    UFUNCTION(BlueprintPure, Category = "Reputation UI")
    bool IsReputationPanelVisible() const;

    // Dialogue System
    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void StartDialogue(FName CharacterID);

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void EndDialogue();

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void ShowDialogueOptions(const TArray<struct FMingDialogueOption>& Options);

    UFUNCTION(BlueprintPure, Category = "Dialogue UI")
    bool IsDialogueActive() const;

    // Quest Panel
    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void ShowQuestPanel();

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void HideQuestPanel();

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void ToggleQuestPanel();

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void ShowQuestDetails(FName QuestID);

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void ShowQuestNotification(FName QuestID, const FString& Message);

    UFUNCTION(BlueprintPure, Category = "Quest UI")
    bool IsQuestPanelVisible() const;

    // Audio Panel
    UFUNCTION(BlueprintCallable, Category = "Audio UI")
    void ShowAudioPanel();

    UFUNCTION(BlueprintCallable, Category = "Audio UI")
    void HideAudioPanel();

    UFUNCTION(BlueprintCallable, Category = "Audio UI")
    void ToggleAudioPanel();

    // Quick Access
    UFUNCTION(BlueprintCallable, Category = "Quick Access")
    void ShowQuickRelationshipBar();

    UFUNCTION(BlueprintCallable, Category = "Quick Access")
    void HideQuickRelationshipBar();

    UFUNCTION(BlueprintCallable, Category = "Quick Access")
    void ShowQuickReputationBar();

    UFUNCTION(BlueprintCallable, Category = "Quick Access")
    void HideQuickReputationBar();

    UFUNCTION(BlueprintCallable, Category = "Quick Access")
    void UpdateQuickAccessDisplays();

    // Notifications
    UFUNCTION(BlueprintCallable, Category = "Notifications")
    void ShowNotification(const FString& Title, const FString& Message, float Duration = 5.0f);

    UFUNCTION(BlueprintCallable, Category = "Notifications")
    void ShowRelationshipNotification(FName CharacterID, float OldValue, float NewValue);

    UFUNCTION(BlueprintCallable, Category = "Notifications")
    void ShowReputationNotification(FName RegionID, float OldValue, float NewValue);

    UFUNCTION(BlueprintCallable, Category = "Notifications")
    void ShowQuestUpdateNotification(FName QuestID, const FString& UpdateMessage);

    // Input Handling
    UFUNCTION(BlueprintCallable, Category = "Input")
    void HandleToggleRelationshipInput();

    UFUNCTION(BlueprintCallable, Category = "Input")
    void HandleToggleReputationInput();

    UFUNCTION(BlueprintCallable, Category = "Input")
    void HandleToggleQuestInput();

    UFUNCTION(BlueprintCallable, Category = "Input")
    void HandleToggleAudioInput();

    UFUNCTION(BlueprintCallable, Category = "Input")
    void HandlePauseInput();

    UFUNCTION(BlueprintCallable, Category = "Input")
    void HandleBackInput();

    // Event Response
    UFUNCTION()
    void OnRelationshipChanged(FName CharacterID, float NewValue);

    UFUNCTION()
    void OnReputationChanged(FName RegionID, float NewValue);

    UFUNCTION()
    void OnQuestStatusChanged(FName QuestID, int32 NewStatus);

    UFUNCTION()
    void OnDialogueOptionSelected(int32 OptionID);

    // Audio Integration
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayUISound(int32 SoundType);

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayPanelOpenSound();

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayPanelCloseSound();

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayNotificationSound(int32 NotificationType);

    // Debug
    UFUNCTION(BlueprintCallable, Category = "Debug")
    void ShowDebugInfo(bool bShow);

    UFUNCTION(BlueprintCallable, Category = "Debug")
    void ToggleDebugInfo();

    UFUNCTION(BlueprintPure, Category = "Debug")
    FString GetDebugInfo() const;

    // Event Delegates
    
    
    
    

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnHUDModeChanged OnHUDModeChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPanelOpened OnPanelOpened;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPanelClosed OnPanelClosed;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnNotificationShown OnNotificationShown;

protected:
    // Widget References
    UPROPERTY(EditDefaultsOnly, Category = "Widgets")
    TSubclassOf<class UMingRelationshipWidget> RelationshipWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Widgets")
    TSubclassOf<class UMingReputationWidget> ReputationWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Widgets")
    TSubclassOf<class UMingDialogueWidget> DialogueWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Widgets")
    TSubclassOf<class UMingQuestWidget> QuestWidgetClass;

    UPROPERTY()
    UMingRelationshipWidget* RelationshipWidget;

    UPROPERTY()
    UMingReputationWidget* ReputationWidget;

    UPROPERTY()
    UMingDialogueWidget* DialogueWidget;

    UPROPERTY()
    UMingQuestWidget* QuestWidget;

    // Quick Access Widgets
    UPROPERTY(EditDefaultsOnly, Category = "Widgets")
    TSubclassOf<UUserWidget> QuickRelationshipWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Widgets")
    TSubclassOf<UUserWidget> QuickReputationWidgetClass;

    UPROPERTY()
    UUserWidget* QuickRelationshipWidget;

    UPROPERTY()
    UUserWidget* QuickReputationWidget;

    // State
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    EMingHUDMode CurrentMode;

    UPROPERTY()
    EMingHUDMode PreviousMode;

    UPROPERTY()
    TArray<TPair<TSubclassOf<UUserWidget>, UUserWidget*>> ActiveWidgets;

    UPROPERTY()
    TWeakObjectPtr<class APlayerController> PlayerController;

    UPROPERTY()
    TWeakObjectPtr<class UMingPersonalManager> PersonalManager;

    UPROPERTY()
    float NotificationTimer;

    // Layer Management
    UPROPERTY()
    TMap<EMingHUDLayer, TArray<UUserWidget*>> LayerWidgets;

    // Internal Functions
    void InitializeWidgets();
    void SetupInputBindings();
    void BindToPersonalManager();
    void UnbindFromPersonalManager();
    
    UUserWidget* CreateWidgetOnLayer(TSubclassOf<UUserWidget> WidgetClass, EMingHUDLayer Layer);
    void AddWidgetToLayer(UUserWidget* Widget, EMingHUDLayer Layer);
    void RemoveWidgetFromLayer(UUserWidget* Widget);
    void ClearLayer(EMingHUDLayer Layer);
    
    void CloseAllPanels();
    void UpdateHUDForMode();
    void ShowLayerWidgets(EMingHUDLayer Layer);
    void HideLayerWidgets(EMingHUDLayer Layer);
    
    bool CanOpenPanel() const;
    void ReturnToGameplayMode();
    
    // Sound Functions
    void PlaySound(int32 SoundType);
    FString GetSoundCuePath(int32 SoundType) const;

public:
    // Blueprint getters for widget access
    UFUNCTION(BlueprintPure, Category = "Widgets")
    UMingRelationshipWidget* GetRelationshipWidget() const { return RelationshipWidget; }

    UFUNCTION(BlueprintPure, Category = "Widgets")
    UMingReputationWidget* GetReputationWidget() const { return ReputationWidget; }

    UFUNCTION(BlueprintPure, Category = "Widgets")
    UMingDialogueWidget* GetDialogueWidget() const { return DialogueWidget; }

    UFUNCTION(BlueprintPure, Category = "Widgets")
    UMingQuestWidget* GetQuestWidget() const { return QuestWidget; }

    UFUNCTION(BlueprintPure, Category = "Widgets")
    UUserWidget* GetQuickRelationshipWidget() const { return QuickRelationshipWidget; }

    UFUNCTION(BlueprintPure, Category = "Widgets")
    UUserWidget* GetQuickReputationWidget() const { return QuickReputationWidget; }
};

