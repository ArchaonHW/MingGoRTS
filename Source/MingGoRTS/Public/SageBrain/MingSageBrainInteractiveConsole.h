#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSageBrainAPIConsole.h"
#include "MingSageBrainInteractiveConsole.generated.h"

// 用户交互模式
UENUM(BlueprintType)
enum class EUserInteractionMode : uint8
{
    CommandLine    UMETA(DisplayName = "命令行模式"),
    Dialogue       UMETA(DisplayName = "对话模式"),
    Wizard         UMETA(DisplayName = "向导模式"),
    Chat           UMETA(DisplayName = "聊天模式"),
    Interactive    UMETA(DisplayName = "交互模式")
};

// 会话状态
UENUM(BlueprintType)
enum class EConsoleSessionState : uint8
{
    Idle           UMETA(DisplayName = "空闲"),
    Listening      UMETA(DisplayName = "监听中"),
    Processing     UMETA(DisplayName = "处理中"),
    Responding     UMETA(DisplayName = "响应中"),
    Waiting        UMETA(DisplayName = "等待用户"),
    Closed         UMETA(DisplayName = "已关闭")
};

// 用户输入上下文
USTRUCT(BlueprintType)
struct FUserInputContext
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Context")
    FString InputText;

    UPROPERTY(BlueprintReadOnly, Category = "Context")
    EUserInteractionMode Mode;

    UPROPERTY(BlueprintReadOnly, Category = "Context")
    FDateTime InputTime;

    UPROPERTY(BlueprintReadOnly, Category = "Context")
    int32 SessionID = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Context")
    TMap<FString, FString> ContextData;
};

// 系统响应
USTRUCT(BlueprintType)
struct FSystemResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Response")
    FString ResponseText;

    UPROPERTY(BlueprintReadOnly, Category = "Response")
    EConsoleOutputLevel Level;

    UPROPERTY(BlueprintReadOnly, Category = "Response")
    TArray<FString> SuggestedActions;

    UPROPERTY(BlueprintReadOnly, Category = "Response")
    TMap<FString, FString> ResponseData;

    UPROPERTY(BlueprintReadOnly, Category = "Response")
    bool bRequiresUserInput = false;

    UPROPERTY(BlueprintReadOnly, Category = "Response")
    FString PromptForInput;
};

// 会话历史
USTRUCT(BlueprintType)
struct FConsoleSession
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    int32 SessionID = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    FDateTime LastActivity;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    int32 MessageCount = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    EUserInteractionMode CurrentMode;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    TMap<FString, FString> SessionVariables;
};

// 交互式对话回调
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnUserInputReceived, const FUserInputContext&, InputContext);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSystemResponseReady, const FSystemResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSessionStateChanged, EConsoleSessionState, NewState);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnInteractiveModeChanged, EUserInteractionMode, NewMode);

/**
 * 圣者大脑交互式控制台
 * 提供与终端用户直接互动的功能，支持多种交互模式
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingSageBrainInteractiveConsole : public UObject
{
    GENERATED_BODY()

public:
    UMingSageBrainInteractiveConsole();

    // 初始化
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive")
    bool InitializeInteractiveConsole();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive")
    void ShutdownInteractiveConsole();

    // 交互模式管理
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Mode")
    void SetInteractionMode(EUserInteractionMode Mode);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Mode")
    EUserInteractionMode GetCurrentInteractionMode() const { return CurrentMode; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Mode")
    TArray<FString> GetAvailableModes() const;

    // 用户输入处理
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Input")
    void ProcessUserInput(const FString& Input);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Input")
    void StartListening();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Input")
    void StopListening();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Input")
    bool IsListening() const { return SessionState == EConsoleSessionState::Listening; }

    // 系统响应
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Response")
    FSystemResponse GenerateResponse(const FUserInputContext& Input);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Response")
    void SendResponse(const FSystemResponse& Response);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Response")
    void SendWelcomeMessage();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Response")
    void SendHelpMessage();

    // 交互式向导
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Wizard")
    void StartWizard(const FString& WizardName);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Wizard")
    void ProcessWizardStep(const FString& Input);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Wizard")
    void CancelWizard();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Wizard")
    bool IsInWizardMode() const { return bInWizardMode; }

    // 对话模式
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Dialogue")
    void StartDialogue(const FString& Topic);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Dialogue")
    void ProcessDialogueInput(const FString& Input);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Dialogue")
    void EndDialogue();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Dialogue")
    bool IsInDialogueMode() const { return bInDialogueMode; }

    // 聊天模式
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Chat")
    void StartChatSession();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Chat")
    void ProcessChatMessage(const FString& Message);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Chat")
    void EndChatSession();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Chat")
    FConsoleSession GetCurrentSession() const { return CurrentSession; }

    // 会话管理
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Session")
    void StartNewSession();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Session")
    void EndCurrentSession();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Session")
    void SetSessionVariable(const FString& Key, const FString& Value);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Session")
    FString GetSessionVariable(const FString& Key) const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Session")
    void ClearSessionVariables();

    // 智能提示和建议
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Smart")
    TArray<FString> GetSmartSuggestions(const FString& PartialInput);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Smart")
    FString GetNextRecommendedAction();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Smart")
    bool IsValidCommand(const FString& Input);

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Interactive|Events")
    FOnUserInputReceived OnUserInputReceived;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Interactive|Events")
    FOnSystemResponseReady OnSystemResponseReady;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Interactive|Events")
    FOnSessionStateChanged OnSessionStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Interactive|Events")
    FOnInteractiveModeChanged OnInteractiveModeChanged;

    // 主循环
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|MainLoop")
    void RunMainLoop();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|MainLoop")
    void StopMainLoop();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|MainLoop")
    bool IsRunning() const { return bIsRunning; }

    // 状态查询
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Status")
    EConsoleSessionState GetSessionState() const { return SessionState; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Status")
    FString GetStatusString() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Interactive|Status")
    FString GetInteractivePrompt() const;

private:
    // 核心系统
    UPROPERTY()
    TObjectPtr<UMingSageBrainAPIConsole> APIConsole;

    // 状态
    bool bIsInitialized;
    bool bIsRunning;
    bool bInWizardMode;
    bool bInDialogueMode;
    EUserInteractionMode CurrentMode;
    EConsoleSessionState SessionState;
    FConsoleSession CurrentSession;
    int32 SessionCounter;

    // 当前向导状态
    FString CurrentWizardName;
    int32 CurrentWizardStep;
    TMap<FString, FString> WizardData;

    // 历史记录
    TArray<FUserInputContext> InputHistory;
    TArray<FSystemResponse> ResponseHistory;

    // 内部处理函数
    FSystemResponse ProcessCommandLineInput(const FUserInputContext& Input);
    FSystemResponse ProcessDialogueInput(const FUserInputContext& Input);
    FSystemResponse ProcessWizardInput(const FUserInputContext& Input);
    FSystemResponse ProcessChatInput(const FUserInputContext& Input);
    FSystemResponse ProcessInteractiveInput(const FUserInputContext& Input);

    // 向导处理
    FSystemResponse ExecuteWizardStep();
    TArray<FString> GetWizardSteps(const FString& WizardName);
    void CompleteWizard();

    // 智能处理
    FString AnalyzeUserIntent(const FString& Input);
    TArray<FString> ExtractEntities(const FString& Input);
    FString GenerateSmartResponse(const FString& Intent, const TArray<FString>& Entities);

    // 辅助函数
    void UpdateSessionState(EConsoleSessionState NewState);
    void UpdateInteractionMode(EUserInteractionMode NewMode);
    void LogInteraction(const FString& Direction, const FString& Content);
    FString GetTimestamp() const;
    int32 GenerateSessionID();
};
