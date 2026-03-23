#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingMultiplayerWidget.generated.h"

// Forward declarations
class UMingNetworkManager;
class UMingLobbySystem;
class UButton;
class UTextBlock;
class UEditableTextBox;
class UListView;
class UScrollBox;

/**
 * Multiplayer UI Widget
 * Handles multiplayer menu, lobby, and connection interfaces
 */
UCLASS(ClassGroup = (UI), Blueprintable, BlueprintType)
class MINGUI_API UMingMultiplayerWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingMultiplayerWidget(const FObjectInitializer& ObjectInitializer};

    // Initialize
    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget")
    void InitializeMultiplayerWidget(UMingNetworkManager* InNetworkManager, UMingLobbySystem* InLobbySystem};

    // View/Interface
    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget")
    void ShowMainMenu(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget")
    void ShowLobbyInterface(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget")
    void ShowConnectionDialog(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget")
    void HideWidget(};

    // === Main Menu ===

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetMain Menu")
    void OnCreateServerClicked(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetMain Menu")
    void OnJoinServerClicked(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetMain Menu")
    void OnQuickMatchClicked(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetMain Menu")
    void OnSettingsClicked(};

    // === Connection ===

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetConnection")
    void OnConnectClicked(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetConnection")
    void OnDisconnectClicked(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetConnection")
    void OnCancelConnectionClicked(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetConnection")
    void UpdateConnectionStatus(const FString& Status};

    // === Lobby ===

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetLobby")
    void OnLeaveLobbyClicked(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetLobby")
    void OnReadyClicked(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetLobby")
    void OnStartGameClicked(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetLobby")
    void OnCancelStartClicked(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetLobby")
    void OnChangeTeamClicked(int32 TeamID};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetLobby")
    void OnKickPlayerClicked(int32 PlayerID};

    // === Chat ===

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetChat")
    void OnSendMessageClicked(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetChat")
    void AddChatMessage(const FString& PlayerName, const FString& Message, bool bIsSystemMessage = false};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetChat")
    void ClearChat(};

    // === Player List ===

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetPlayer List")
    void UpdatePlayerList(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetPlayer List")
    void AddPlayerToList(int32 PlayerID, const FString& PlayerName, bool bIsReady, bool bIsHost, int32 TeamID};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetPlayer List")
    void RemovePlayerFromList(int32 PlayerID};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetPlayer List")
    void UpdatePlayerStatus(int32 PlayerID, bool bIsReady};

    // === Updates ===

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetUpdates")
    void UpdateNetworkStatusDisplay(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetUpdates")
    void UpdateLobbyInfoDisplay(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetUpdates")
    void ShowCountdown(int32 Seconds};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetUpdates")
    void HideCountdown(};

    // === Events ===

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetEvents")
    void OnNetworkConnected(};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetEvents")
    void OnNetworkDisconnected(const FString& Reason};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetEvents")
    void OnPlayerJoined(int32 PlayerID, const FString& PlayerName};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetEvents")
    void OnPlayerLeft(int32 PlayerID, const FString& Reason};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetEvents")
    void OnPlayerReadyChanged(int32 PlayerID, bool bReady};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetEvents")
    void OnGameStarting(int32 CountdownSeconds};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer WidgetEvents")
    void OnChatMessageReceived(const FString& PlayerName, const FString& Message, bool bIsSystemMessage};

protected:
    // Bind UI elements
    UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer WidgetBinding")
    void BindUIElements(};

    UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer WidgetBinding")
    void SetupEventBindings(};

    // Native construct
    virtual void NativeConstruct() override;

    // Native destruct
    virtual void NativeDestruct() override;

    // Refresh display
    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget")
    void RefreshDisplay(};

protected:
    // Network manager reference
    UPROPERTY(BlueprintReadOnly, Category = "Multiplayer Widget")
    TObjectPtr<UMingNetworkManager> NetworkManager;

    // Lobby system reference
    UPROPERTY(BlueprintReadOnly, Category = "Multiplayer Widget")
    TObjectPtr<UMingLobbySystem> LobbySystem;

    // Current view
    UPROPERTY(BlueprintReadOnly, Category = "Multiplayer Widget")
    FString CurrentView;

    // UI Elements (BindWidget)
    UPROPERTY(meta = (BindWidget), Category = "Multiplayer WidgetUI Elements")
    TObjectPtr<UEditableTextBox> ServerAddressInput;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer WidgetUI Elements")
    TObjectPtr<UEditableTextBox> PortInput;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer WidgetUI Elements")
    TObjectPtr<UEditableTextBox> ChatInput;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer WidgetUI Elements")
    TObjectPtr<UTextBlock> ConnectionStatusText;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer WidgetUI Elements")
    TObjectPtr<UTextBlock> LobbyNameText;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer WidgetUI Elements")
    TObjectPtr<UTextBlock> PlayerCountText;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer WidgetUI Elements")
    TObjectPtr<UTextBlock> CountdownText;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer WidgetUI Elements")
    TObjectPtr<UListView> PlayerListView;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer WidgetUI Elements")
    TObjectPtr<UScrollBox> ChatScrollBox;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer WidgetUI Elements")
    TObjectPtr<UButton> CreateServerButton;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer WidgetUI Elements")
    TObjectPtr<UButton> JoinServerButton;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer WidgetUI Elements")
    TObjectPtr<UButton> ConnectButton;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer WidgetUI Elements")
    TObjectPtr<UButton> DisconnectButton;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer WidgetUI Elements")
    TObjectPtr<UButton> ReadyButton;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer WidgetUI Elements")
    TObjectPtr<UButton> StartGameButton;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer WidgetUI Elements")
    TObjectPtr<UButton> LeaveLobbyButton;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer WidgetUI Elements")
    TObjectPtr<UButton> SendMessageButton;
};
