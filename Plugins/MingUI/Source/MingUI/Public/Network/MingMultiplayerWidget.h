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
 * 多人??戲UI組件
 * ????多人??戲X????大廳、??天?X?能??面
 */
UCLASS(ClassGroup = (UI), Blueprintable, BlueprintType)
class MINGUI_API UMingMultiplayerWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingMultiplayerWidget(const FObjectInitializer& ObjectInitializer};

    // ???X    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget")
    void InitializeMultiplayerWidget(UMingNetworkManager* InNetworkManager, UMingLobbySystem* InLobbySystem};

    // 顯示/???X?面
    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget")
    void ShowMainMenu();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget")
    void ShowLobbyInterface();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget")
    void ShowConnectionDialog();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget")
    void HideWidget();

    // === 主?X??X===

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Main Menu")
    void OnCreateServerClicked();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Main Menu")
    void OnJoinServerClicked();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Main Menu")
    void OnQuickMatchClicked();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Main Menu")
    void OnSettingsClicked();

    // === X????能 ===

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Connection")
    void OnConnectClicked();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Connection")
    void OnDisconnectClicked();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Connection")
    void OnCancelConnectionClicked();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Connection")
    void UpdateConnectionStatus(const FString& Status};

    // === 大廳??能 ===

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Lobby")
    void OnLeaveLobbyClicked();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Lobby")
    void OnReadyClicked();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Lobby")
    void OnStartGameClicked();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Lobby")
    void OnCancelStartClicked();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Lobby")
    void OnChangeTeamClicked(int32 TeamID};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Lobby")
    void OnKickPlayerClicked(int32 PlayerID};

    // === ??天??能 ===

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Chat")
    void OnSendMessageClicked();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Chat")
    void AddChatMessage(const FString& PlayerName, const FString& Message, bool bIsSystemMessage = false};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Chat")
    void ClearChat();

    // === ??家??表 ===

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Player List")
    void UpdatePlayerList();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Player List")
    void AddPlayerToList(int32 PlayerID, const FString& PlayerName, bool bIsReady, bool bIsHost, int32 TeamID};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Player List")
    void RemovePlayerFromList(int32 PlayerID};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Player List")
    void UpdatePlayerStatus(int32 PlayerID, bool bIsReady};

    // === ??新??面 ===

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Updates")
    void UpdateNetworkStatusDisplay();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Updates")
    void UpdateLobbyInfoDisplay();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Updates")
    void ShowCountdown(int32 Seconds};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Updates")
    void HideCountdown();

    // === 事件???? ===

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Events")
    void OnNetworkConnected();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Events")
    void OnNetworkDisconnected(const FString& Reason};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Events")
    void OnPlayerJoined(int32 PlayerID, const FString& PlayerName};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Events")
    void OnPlayerLeft(int32 PlayerID, const FString& Reason};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Events")
    void OnPlayerReadyChanged(int32 PlayerID, bool bReady};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Events")
    void OnGameStarting(int32 CountdownSeconds};

    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget|Events")
    void OnChatMessageReceived(const FString& PlayerName, const FString& Message, bool bIsSystemMessage};

protected:
    // 綁??UI????（在????中實????
    UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer Widget|Binding")
    void BindUIElements();

    UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer Widget|Binding")
    void SetupEventBindings();

    // Native construct
    virtual void NativeConstruct() override;

    // Native destruct
    virtual void NativeDestruct() override;

    // ??新顯示
    UFUNCTION(BlueprintCallable, Category = "Multiplayer Widget")
    void RefreshDisplay();

protected:
    // 網絡管?X??X    UPROPERTY(BlueprintReadOnly, Category = "Multiplayer Widget")
    TObjectPtr<UMingNetworkManager> NetworkManager;

    // 大廳系統引用
    UPROPERTY(BlueprintReadOnly, Category = "Multiplayer Widget")
    TObjectPtr<UMingLobbySystem> LobbySystem;

    // ????顯示??X    UPROPERTY(BlueprintReadOnly, Category = "Multiplayer Widget")
    FString CurrentView;

    // UI????引用（在????中設置??
    UPROPERTY(meta = (BindWidget), Category = "Multiplayer Widget|UI Elements")
    TObjectPtr<UEditableTextBox> ServerAddressInput;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer Widget|UI Elements")
    TObjectPtr<UEditableTextBox> PortInput;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer Widget|UI Elements")
    TObjectPtr<UEditableTextBox> ChatInput;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer Widget|UI Elements")
    TObjectPtr<UTextBlock> ConnectionStatusText;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer Widget|UI Elements")
    TObjectPtr<UTextBlock> LobbyNameText;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer Widget|UI Elements")
    TObjectPtr<UTextBlock> PlayerCountText;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer Widget|UI Elements")
    TObjectPtr<UTextBlock> CountdownText;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer Widget|UI Elements")
    TObjectPtr<UListView> PlayerListView;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer Widget|UI Elements")
    TObjectPtr<UScrollBox> ChatScrollBox;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer Widget|UI Elements")
    TObjectPtr<UButton> CreateServerButton;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer Widget|UI Elements")
    TObjectPtr<UButton> JoinServerButton;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer Widget|UI Elements")
    TObjectPtr<UButton> ConnectButton;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer Widget|UI Elements")
    TObjectPtr<UButton> DisconnectButton;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer Widget|UI Elements")
    TObjectPtr<UButton> ReadyButton;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer Widget|UI Elements")
    TObjectPtr<UButton> StartGameButton;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer Widget|UI Elements")
    TObjectPtr<UButton> LeaveLobbyButton;

    UPROPERTY(meta = (BindWidget), Category = "Multiplayer Widget|UI Elements")
    TObjectPtr<UButton> SendMessageButton;
};

