#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSageBrainInteractiveConsole.h"
#include "MingSageBrainBridge.h"
#include "MingSageBrainConsoleTerminal.generated.h"

// 终端显示模式
UENUM(BlueprintType)
enum class EConsoleDisplayMode : uint8
{
    Text           UMETA(DisplayName = "文本模式"),
    Enhanced       UMETA(DisplayName = "增强模式"),
    Graphical      UMETA(DisplayName = "图形模式"),
    Minimal        UMETA(DisplayName = "极简模式"),
    Full           UMETA(DisplayName = "完整模式")
};

// 终端主题
USTRUCT(BlueprintType)
struct FConsoleTheme
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Theme")
    FLinearColor BackgroundColor = FLinearColor(0.05f, 0.05f, 0.1f, 0.9f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Theme")
    FLinearColor TextColor = FLinearColor(0.9f, 0.9f, 0.9f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Theme")
    FLinearColor AccentColor = FLinearColor(0.2f, 0.6f, 1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Theme")
    FLinearColor SuccessColor = FLinearColor(0.2f, 0.8f, 0.2f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Theme")
    FLinearColor WarningColor = FLinearColor(1.0f, 0.6f, 0.2f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Theme")
    FLinearColor ErrorColor = FLinearColor(1.0f, 0.2f, 0.2f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Theme")
    FString FontName = TEXT("Consolas");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Theme")
    int32 FontSize = 14;
};

// 终端配置
USTRUCT(BlueprintType)
struct FConsoleTerminalConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    EConsoleDisplayMode DisplayMode = EConsoleDisplayMode::Enhanced;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    FConsoleTheme Theme;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    int32 MaxHistoryLines = 1000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    bool bShowTimestamp = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    bool bShowPrompt = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    bool bAutoScroll = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    bool bShowLineNumbers = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    float UpdateInterval = 0.1f;
};

// 终端输出行
USTRUCT(BlueprintType)
struct FConsoleOutputLine
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Output")
    FString Text;

    UPROPERTY(BlueprintReadOnly, Category = "Output")
    EConsoleOutputLevel Level;

    UPROPERTY(BlueprintReadOnly, Category = "Output")
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly, Category = "Output")
    FString Source;

    UPROPERTY(BlueprintReadOnly, Category = "Output")
    int32 LineNumber;

    UPROPERTY(BlueprintReadOnly, Category = "Output")
    FLinearColor Color;
};

// 终端状态
USTRUCT(BlueprintType)
struct FConsoleTerminalState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bIsVisible = false;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bIsFocused = false;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bIsInputEnabled = true;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    FString CurrentInput;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    int32 CursorPosition = 0;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    TArray<FString> InputHistory;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    int32 HistoryIndex = -1;
};

// 终端事件
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnTerminalOutput, const FConsoleOutputLine&, OutputLine);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnTerminalInput, const FString&, Input);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnTerminalStateChanged, const FConsoleTerminalState&, State);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnTerminalCommand, const FString&, Command, const TArray<FString>&, Arguments);

/**
 * 圣者大脑控制台终端
 * 提供完整的终端界面和圣者大脑系统的对接
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingSageBrainConsoleTerminal : public UObject
{
    GENERATED_BODY()

public:
    UMingSageBrainConsoleTerminal();

    // 初始化和配置
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal")
    bool InitializeTerminal(const FConsoleTerminalConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal")
    void ShutdownTerminal();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal")
    void SetConfig(const FConsoleTerminalConfig& NewConfig);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal")
    FConsoleTerminalConfig GetConfig() const { return TerminalConfig; }

    // 显示控制
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Display")
    void ShowTerminal();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Display")
    void HideTerminal();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Display")
    void ToggleTerminal();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Display")
    bool IsVisible() const { return TerminalState.bIsVisible; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Display")
    void SetDisplayMode(EConsoleDisplayMode Mode);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Display")
    EConsoleDisplayMode GetDisplayMode() const { return TerminalConfig.DisplayMode; }

    // 输出管理
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Output")
    void AddOutput(const FString& Text, EConsoleOutputLevel Level = EConsoleOutputLevel::Info, const FString& Source = TEXT("System"));

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Output")
    void AddOutputLine(const FConsoleOutputLine& OutputLine);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Output")
    void ClearOutput();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Output")
    TArray<FConsoleOutputLine> GetOutputHistory(int32 MaxLines = 100) const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Output")
    void ScrollToBottom();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Output")
    void ScrollToLine(int32 LineNumber);

    // 输入处理
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Input")
    void ProcessInput(const FString& Input);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Input")
    void SetCurrentInput(const FString& Input);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Input")
    FString GetCurrentInput() const { return TerminalState.CurrentInput; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Input")
    void InsertTextAtCursor(const FString& Text);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Input")
    void DeleteTextAtCursor(int32 Count);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Input")
    void MoveCursor(int32 Direction);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Input")
    void MoveCursorToPosition(int32 Position);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Input")
    int32 GetCursorPosition() const { return TerminalState.CursorPosition; }

    // 历史管理
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|History")
    void AddToInputHistory(const FString& Input);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|History")
    FString GetPreviousInput();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|History")
    FString GetNextInput();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|History")
    void ClearInputHistory();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|History")
    TArray<FString> GetInputHistory() const { return TerminalState.InputHistory; }

    // 圣者大脑集成
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Integration")
    void ConnectToSageBrain(UMingSageBrainInteractiveConsole* Console);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Integration")
    void DisconnectFromSageBrain();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Integration")
    bool IsConnectedToSageBrain() const { return bConnectedToSageBrain; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Integration")
    void SendCommandToSageBrain(const FString& Command);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Integration")
    void SetAutoExecuteCommands(bool bAutoExecute);

    // 渲染和更新
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Rendering")
    void UpdateTerminal();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Rendering")
    void ForceRedraw();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Rendering")
    FString RenderTerminal() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Rendering")
    FString RenderPrompt() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Rendering")
    FString RenderOutput() const;

    // 主题和样式
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Theme")
    void SetTheme(const FConsoleTheme& NewTheme);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Theme")
    FConsoleTheme GetTheme() const { return TerminalConfig.Theme; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Theme")
    void LoadTheme(const FString& ThemeName);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Theme")
    void SaveTheme(const FString& ThemeName, const FConsoleTheme& Theme);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Theme")
    TArray<FString> GetAvailableThemes() const;

    // 快捷键和命令
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Shortcuts")
    void RegisterShortcut(const FString& Key, const FString& Command);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Shortcuts")
    void RemoveShortcut(const FString& Key);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Shortcuts")
    TMap<FString, FString> GetAllShortcuts() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Shortcuts")
    bool ExecuteShortcut(const FString& Key);

    // 状态查询
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Status")
    FConsoleTerminalState GetState() const { return TerminalState; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Status")
    FString GetStatusString() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Terminal|Status")
    bool IsReady() const { return bIsInitialized; }

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Terminal|Events")
    FOnTerminalOutput OnTerminalOutput;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Terminal|Events")
    FOnTerminalInput OnTerminalInput;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Terminal|Events")
    FOnTerminalStateChanged OnTerminalStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Terminal|Events")
    FOnTerminalCommand OnTerminalCommand;

protected:
    // 虚函数重载
    virtual void BeginDestroy() override;

private:
    // 核心组件
    UPROPERTY()
    TObjectPtr<UMingSageBrainInteractiveConsole> SageBrainConsole;

    // 配置和状态
    FConsoleTerminalConfig TerminalConfig;
    FConsoleTerminalState TerminalState;
    TArray<FConsoleOutputLine> OutputHistory;
    TMap<FString, FString> Shortcuts;

    // 状态标志
    bool bIsInitialized;
    bool bConnectedToSageBrain;
    bool bAutoExecuteCommands;
    bool bNeedsRedraw;

    // 渲染相关
    int32 VisibleLines;
    int32 ScrollPosition;
    float LastUpdateTime;

    // 内部处理函数
    void UpdateTerminalState();
    void ProcessCommand(const FString& Command);
    void ParseCommand(const FString& Command, FString& OutCommand, TArray<FString>& OutArguments);
    void HandleSageBrainResponse(const FSystemResponse& Response);
    void UpdateCursorPosition(int32 NewPosition);
    void ValidateCursorPosition();

    // 渲染辅助函数
    FString FormatOutputLine(const FConsoleOutputLine& Line) const;
    FString ApplyColorToText(const FString& Text, const FLinearColor& Color) const;
    FString GetTimestampString(const FDateTime& Timestamp) const;
    FString GetLevelPrefix(EConsoleOutputLevel Level) const;

    // 主题管理
    void InitializeDefaultThemes();
    void ApplyThemeToOutput();
    FConsoleTheme GetThemeByName(const FString& ThemeName) const;

    // 快捷键处理
    bool HandleSpecialKey(const FString& Key);
    void ExecuteCommandWithHistory(const FString& Command);
    void UpdateHistoryNavigation(const FString& CurrentInput);

    // 圣者大脑集成
    void OnSageBrainOutput(const FConsoleOutputItem& OutputItem);
    void OnSageBrainResponse(const FSystemResponse& Response);
    void OnSageBrainInput(const FUserInputContext& InputContext);
    void ForwardCommandToSageBrain(const FString& Command, const TArray<FString>& Arguments);

    // 辅助函数
    void LogTerminalEvent(const FString& Event, const FString& Details);
    void CleanupResources();
    int32 GetMaxVisibleLines() const;
    bool ShouldAutoScroll() const;
};
