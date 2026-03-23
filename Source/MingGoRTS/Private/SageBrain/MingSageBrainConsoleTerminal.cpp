#include "MingSageBrainConsoleTerminal.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "TimerManager.h"
#include "Async/Async.h"

UMingSageBrainConsoleTerminal::UMingSageBrainConsoleTerminal()
    : bIsInitialized(false)
    , bConnectedToSageBrain(false)
    , bAutoExecuteCommands(true)
    , bNeedsRedraw(true)
    , VisibleLines(0)
    , ScrollPosition(0)
    , LastUpdateTime(0.0f)
{
}

bool UMingSageBrainConsoleTerminal::InitializeTerminal(const FConsoleTerminalConfig& Config)
{
    if (bIsInitialized)
    {
        return true;
    }

    TerminalConfig = Config;
    VisibleLines = GetMaxVisibleLines();
    ScrollPosition = 0;
    
    // 初始化终端状态
    TerminalState.bIsVisible = false;
    TerminalState.bIsFocused = false;
    TerminalState.bIsInputEnabled = true;
    TerminalState.CurrentInput.Empty();
    TerminalState.CursorPosition = 0;
    TerminalState.HistoryIndex = -1;

    // 初始化默认主题
    InitializeDefaultThemes();

    // 清空输出历史
    OutputHistory.Empty();
    TerminalState.InputHistory.Empty();

    bIsInitialized = true;
    bNeedsRedraw = true;

    UE_LOG(LogTemp, Log, TEXT("圣者大脑控制台终端已初始化"));
    return true;
}

void UMingSageBrainConsoleTerminal::ShutdownTerminal()
{
    if (!bIsInitialized)
    {
        return;
    }

    DisconnectFromSageBrain();
    
    CleanupResources();
    
    bIsInitialized = false;
    UE_LOG(LogTemp, Log, TEXT("控制台终端已关闭"));
}

void UMingSageBrainConsoleTerminal::SetConfig(const FConsoleTerminalConfig& NewConfig)
{
    TerminalConfig = NewConfig;
    bNeedsRedraw = true;
    
    // 重新计算可见行数
    VisibleLines = GetMaxVisibleLines();
    
    LogTerminalEvent(TEXT("配置更新"), TEXT("终端配置已更新"));
}

void UMingSageBrainConsoleTerminal::ShowTerminal()
{
    if (!bIsInitialized)
    {
        return;
    }

    TerminalState.bIsVisible = true;
    TerminalState.bIsFocused = true;
    bNeedsRedraw = true;
    
    UpdateTerminalState();
    OnTerminalStateChanged.Broadcast(TerminalState);
    
    UE_LOG(LogTemp, Log, TEXT("控制台终端已显示"));
}

void UMingSageBrainConsoleTerminal::HideTerminal()
{
    TerminalState.bIsVisible = false;
    TerminalState.bIsFocused = false;
    
    UpdateTerminalState();
    OnTerminalStateChanged.Broadcast(TerminalState);
    
    UE_LOG(LogTemp, Log, TEXT("控制台终端已隐藏"));
}

void UMingSageBrainConsoleTerminal::ToggleTerminal()
{
    if (TerminalState.bIsVisible)
    {
        HideTerminal();
    }
    else
    {
        ShowTerminal();
    }
}

void UMingSageBrainConsoleTerminal::SetDisplayMode(EConsoleDisplayMode Mode)
{
    if (TerminalConfig.DisplayMode == Mode)
    {
        return;
    }

    TerminalConfig.DisplayMode = Mode;
    bNeedsRedraw = true;
    
    // 根据显示模式调整配置
    switch (Mode)
    {
    case EConsoleDisplayMode::Minimal:
        TerminalConfig.bShowTimestamp = false;
        TerminalConfig.bShowLineNumbers = false;
        break;
    case EConsoleDisplayMode::Full:
        TerminalConfig.bShowTimestamp = true;
        TerminalConfig.bShowLineNumbers = true;
        break;
    default:
        break;
    }
    
    LogTerminalEvent(TEXT("显示模式切换"), FString::Printf(TEXT("切换到模式: %d"), static_cast<int32>(Mode)));
}

void UMingSageBrainConsoleTerminal::AddOutput(const FString& Text, EConsoleOutputLevel Level, const FString& Source)
{
    FConsoleOutputLine OutputLine;
    OutputLine.Text = Text;
    OutputLine.Level = Level;
    OutputLine.Timestamp = FDateTime::Now();
    OutputLine.Source = Source;
    OutputLine.LineNumber = OutputHistory.Num() + 1;
    OutputLine.Color = GetColorForLevel(Level);

    AddOutputLine(OutputLine);
}

void UMingSageBrainConsoleTerminal::AddOutputLine(const FConsoleOutputLine& OutputLine)
{
    OutputHistory.Add(OutputLine);
    
    // 限制历史记录大小
    if (OutputHistory.Num() > TerminalConfig.MaxHistoryLines)
    {
        OutputHistory.RemoveAt(0, OutputHistory.Num() - TerminalConfig.MaxHistoryLines);
    }

    // 自动滚动到底部
    if (ShouldAutoScroll())
    {
        ScrollToBottom();
    }

    bNeedsRedraw = true;
    OnTerminalOutput.Broadcast(OutputLine);
}

void UMingSageBrainConsoleTerminal::ClearOutput()
{
    OutputHistory.Empty();
    ScrollPosition = 0;
    bNeedsRedraw = true;
    
    LogTerminalEvent(TEXT("清除输出"), TEXT("终端输出已清除"));
}

TArray<FConsoleOutputLine> UMingSageBrainConsoleTerminal::GetOutputHistory(int32 MaxLines) const
{
    int32 StartIndex = FMath::Max(0, OutputHistory.Num() - MaxLines);
    int32 Count = FMath::Min(MaxLines, OutputHistory.Num() - StartIndex);
    
    TArray<FConsoleOutputLine> Result;
    for (int32 i = StartIndex; i < StartIndex + Count; i++)
    {
        Result.Add(OutputHistory[i]);
    }
    return Result;
}

void UMingSageBrainConsoleTerminal::ScrollToBottom()
{
    if (OutputHistory.Num() > VisibleLines)
    {
        ScrollPosition = OutputHistory.Num() - VisibleLines;
    }
    else
    {
        ScrollPosition = 0;
    }
    bNeedsRedraw = true;
}

void UMingSageBrainConsoleTerminal::ScrollToLine(int32 LineNumber)
{
    ScrollPosition = FMath::Clamp(LineNumber, 0, FMath::Max(0, OutputHistory.Num() - VisibleLines));
    bNeedsRedraw = true;
}

void UMingSageBrainConsoleTerminal::ProcessInput(const FString& Input)
{
    if (!TerminalState.bIsInputEnabled)
    {
        return;
    }

    // 添加到输入历史
    AddToInputHistory(Input);
    
    // 更新当前输入
    TerminalState.CurrentInput = Input;
    TerminalState.CursorPosition = Input.Len();
    TerminalState.HistoryIndex = -1;

    // 广播输入事件
    OnTerminalInput.Broadcast(Input);

    // 处理命令
    if (bAutoExecuteCommands && !Input.IsEmpty())
    {
        ProcessCommand(Input);
    }

    bNeedsRedraw = true;
    UpdateTerminalState();
}

void UMingSageBrainConsoleTerminal::SetCurrentInput(const FString& Input)
{
    TerminalState.CurrentInput = Input;
    TerminalState.CursorPosition = FMath::Clamp(TerminalState.CursorPosition, 0, Input.Len());
    bNeedsRedraw = true;
}

void UMingSageBrainConsoleTerminal::InsertTextAtCursor(const FString& Text)
{
    FString Left = TerminalState.CurrentInput.Left(TerminalState.CursorPosition);
    FString Right = TerminalState.CurrentInput.RightChop(TerminalState.CursorPosition);
    TerminalState.CurrentInput = Left + Text + Right;
    TerminalState.CursorPosition += Text.Len();
    bNeedsRedraw = true;
}

void UMingSageBrainConsoleTerminal::DeleteTextAtCursor(int32 Count)
{
    if (Count <= 0)
    {
        return;
    }

    int32 StartPos = TerminalState.CursorPosition;
    int32 EndPos = FMath::Min(TerminalState.CursorPosition + Count, TerminalState.CurrentInput.Len());
    
    TerminalState.CurrentInput.RemoveAt(StartPos, EndPos - StartPos);
    bNeedsRedraw = true;
}

void UMingSageBrainConsoleTerminal::MoveCursor(int32 Direction)
{
    TerminalState.CursorPosition = FMath::Clamp(
        TerminalState.CursorPosition + Direction,
        0,
        TerminalState.CurrentInput.Len()
    );
    bNeedsRedraw = true;
}

void UMingSageBrainConsoleTerminal::MoveCursorToPosition(int32 Position)
{
    TerminalState.CursorPosition = FMath::Clamp(Position, 0, TerminalState.CurrentInput.Len());
    bNeedsRedraw = true;
}

void UMingSageBrainConsoleTerminal::AddToInputHistory(const FString& Input)
{
    if (Input.IsEmpty())
    {
        return;
    }

    // 避免重复
    if (TerminalState.InputHistory.Num() > 0 && 
        TerminalState.InputHistory.Last() == Input)
    {
        return;
    }

    TerminalState.InputHistory.Add(Input);
    
    // 限制历史记录大小
    if (TerminalState.InputHistory.Num() > 100)
    {
        TerminalState.InputHistory.RemoveAt(0);
    }
}

FString UMingSageBrainConsoleTerminal::GetPreviousInput()
{
    if (TerminalState.HistoryIndex > 0)
    {
        TerminalState.HistoryIndex--;
    }
    else if (TerminalState.HistoryIndex == -1 && TerminalState.InputHistory.Num() > 0)
    {
        TerminalState.HistoryIndex = TerminalState.InputHistory.Num() - 1;
    }

    if (TerminalState.HistoryIndex >= 0 && TerminalState.HistoryIndex < TerminalState.InputHistory.Num())
    {
        return TerminalState.InputHistory[TerminalState.HistoryIndex];
    }

    return TerminalState.CurrentInput;
}

FString UMingSageBrainConsoleTerminal::GetNextInput()
{
    if (TerminalState.HistoryIndex >= 0 && TerminalState.HistoryIndex < TerminalState.InputHistory.Num() - 1)
    {
        TerminalState.HistoryIndex++;
        return TerminalState.InputHistory[TerminalState.HistoryIndex];
    }
    else if (TerminalState.HistoryIndex == TerminalState.InputHistory.Num() - 1)
    {
        TerminalState.HistoryIndex = -1;
        return FString(); // 返回空字符串表示恢复到当前输入
    }

    return TerminalState.CurrentInput;
}

void UMingSageBrainConsoleTerminal::ClearInputHistory()
{
    TerminalState.InputHistory.Empty();
    TerminalState.HistoryIndex = -1;
    LogTerminalEvent(TEXT("清除历史"), TEXT("输入历史已清除"));
}

void UMingSageBrainConsoleTerminal::ConnectToSageBrain(UMingSageBrainInteractiveConsole* Console)
{
    if (!Console)
    {
        UE_LOG(LogTemp, Warning, TEXT("无法连接到空的圣者大脑控制台"));
        return;
    }

    SageBrainConsole = Console;
    bConnectedToSageBrain = true;

    // 绑定事件
    SageBrainConsole->OnSystemResponseReady.AddDynamic(
        this, &UMingSageBrainConsoleTerminal::OnSageBrainResponse);
    SageBrainConsole->OnUserInputReceived.AddDynamic(
        this, &UMingSageBrainConsoleTerminal::OnSageBrainInput);

    // 发送欢迎消息
    AddOutput(TEXT("已连接到圣者大脑交互式控制台"), EConsoleOutputLevel::Success, TEXT("System"));
    
    LogTerminalEvent(TEXT("连接成功"), TEXT("已连接到圣者大脑控制台"));
}

void UMingSageBrainConsoleTerminal::DisconnectFromSageBrain()
{
    if (!bConnectedToSageBrain || !SageBrainConsole)
    {
        return;
    }

    // 解绑事件
    SageBrainConsole->OnSystemResponseReady.RemoveDynamic(
        this, &UMingSageBrainConsoleTerminal::OnSageBrainResponse);
    SageBrainConsole->OnUserInputReceived.RemoveDynamic(
        this, &UMingSageBrainConsoleTerminal::OnSageBrainInput);

    SageBrainConsole = nullptr;
    bConnectedToSageBrain = false;

    AddOutput(TEXT("已断开与圣者大脑控制台的连接"), EConsoleOutputLevel::Warning, TEXT("System"));
    
    LogTerminalEvent(TEXT("断开连接"), TEXT("已断开与圣者大脑控制台的连接"));
}

void UMingSageBrainConsoleTerminal::SendCommandToSageBrain(const FString& Command)
{
    if (!bConnectedToSageBrain || !SageBrainConsole)
    {
        AddOutput(TEXT("未连接到圣者大脑控制台"), EConsoleOutputLevel::Error, TEXT("System"));
        return;
    }

    // 在终端中显示发送的命令
    AddOutput(RenderPrompt() + Command, EConsoleOutputLevel::Info, TEXT("User"));

    // 发送到圣者大脑
    SageBrainConsole->ProcessUserInput(Command);
}

void UMingSageBrainConsoleTerminal::SetAutoExecuteCommands(bool bAutoExecute)
{
    bAutoExecuteCommands = bAutoExecute;
    LogTerminalEvent(TEXT("自动执行设置"), bAutoExecute ? TEXT("已启用") : TEXT("已禁用"));
}

void UMingSageBrainConsoleTerminal::UpdateTerminal()
{
    if (!bIsInitialized || !TerminalState.bIsVisible)
    {
        return;
    }

    float CurrentTime = FPlatformTime::Seconds();
    if (CurrentTime - LastUpdateTime < TerminalConfig.UpdateInterval)
    {
        return;
    }

    LastUpdateTime = CurrentTime;

    if (bNeedsRedraw)
    {
        ForceRedraw();
    }
}

void UMingSageBrainConsoleTerminal::ForceRedraw()
{
    if (!bIsInitialized)
    {
        return;
    }

    bNeedsRedraw = false;
    
    // 这里可以触发UI更新
    // 实际的渲染逻辑取决于具体的UI实现
    OnTerminalStateChanged.Broadcast(TerminalState);
}

FString UMingSageBrainConsoleTerminal::RenderTerminal() const
{
    FString RenderedContent;
    
    // 渲染输出区域
    RenderedContent += RenderOutput();
    
    // 渲染输入提示符和当前输入
    RenderedContent += RenderPrompt();
    RenderedContent += TerminalState.CurrentInput;
    
    return RenderedContent;
}

FString UMingSageBrainConsoleTerminal::RenderPrompt() const
{
    if (bConnectedToSageBrain && SageBrainConsole)
    {
        return SageBrainConsole->GetInteractivePrompt();
    }
    else
    {
        return TEXT("🧠 > ");
    }
}

FString UMingSageBrainConsoleTerminal::RenderOutput() const
{
    FString Output;
    
    int32 StartIndex = ScrollPosition;
    int32 EndIndex = FMath::Min(StartIndex + VisibleLines, OutputHistory.Num());
    
    for (int32 i = StartIndex; i < EndIndex; i++)
    {
        const FConsoleOutputLine& Line = OutputHistory[i];
        Output += FormatOutputLine(Line);
        Output += TEXT("\n");
    }
    
    return Output;
}

void UMingSageBrainConsoleTerminal::SetTheme(const FConsoleTheme& NewTheme)
{
    TerminalConfig.Theme = NewTheme;
    ApplyThemeToOutput();
    bNeedsRedraw = true;
    
    LogTerminalEvent(TEXT("主题更改"), TEXT("终端主题已更新"));
}

void UMingSageBrainConsoleTerminal::LoadTheme(const FString& ThemeName)
{
    FConsoleTheme Theme = GetThemeByName(ThemeName);
    SetTheme(Theme);
}

void UMingSageBrainConsoleTerminal::SaveTheme(const FString& ThemeName, const FConsoleTheme& Theme)
{
    FString ThemePath = FString::Printf(TEXT("Themes/%s.json"), *ThemeName);
    
    // 这里应该实现主题的序列化和保存
    // 简化实现，只记录日志
    LogTerminalEvent(TEXT("保存主题"), FString::Printf(TEXT("主题已保存到: %s"), *ThemePath));
}

TArray<FString> UMingSageBrainConsoleTerminal::GetAvailableThemes() const
{
    return { TEXT("Default"), TEXT("Dark"), TEXT("Light"), TEXT("Blue"), TEXT("Green") };
}

void UMingSageBrainConsoleTerminal::RegisterShortcut(const FString& Key, const FString& Command)
{
    Shortcuts.Add(Key, Command);
    LogTerminalEvent(TEXT("注册快捷键"), FString::Printf(TEXT("%s -> %s"), *Key, *Command));
}

void UMingSageBrainConsoleTerminal::RemoveShortcut(const FString& Key)
{
    Shortcuts.Remove(Key);
    LogTerminalEvent(TEXT("移除快捷键"), FString::Printf(TEXT("已移除: %s"), *Key));
}

TMap<FString, FString> UMingSageBrainConsoleTerminal::GetAllShortcuts() const
{
    return Shortcuts;
}

bool UMingSageBrainConsoleTerminal::ExecuteShortcut(const FString& Key)
{
    const FString* Command = Shortcuts.Find(Key);
    if (Command)
    {
        ProcessCommand(*Command);
        return true;
    }
    return false;
}

FString UMingSageBrainConsoleTerminal::GetStatusString() const
{
    FString Status = TEXT("=== 控制台终端状态 ===\n");
    Status += FString::Printf(TEXT("初始化: %s\n"), bIsInitialized ? TEXT("是") : TEXT("否"));
    Status += FString::Printf(TEXT("显示: %s\n"), TerminalState.bIsVisible ? TEXT("是") : TEXT("否"));
    Status += FString::Printf(TEXT("聚焦: %s\n"), TerminalState.bIsFocused ? TEXT("是") : TEXT("否"));
    Status += FString::Printf(TEXT("连接圣者大脑: %s\n"), bConnectedToSageBrain ? TEXT("是") : TEXT("否"));
    Status += FString::Printf(TEXT("输出行数: %d\n"), OutputHistory.Num());
    Status += FString::Printf(TEXT("输入历史: %d\n"), TerminalState.InputHistory.Num());
    Status += FString::Printf(TEXT("显示模式: %d\n"), static_cast<int32>(TerminalConfig.DisplayMode));
    return Status;
}

void UMingSageBrainConsoleTerminal::BeginDestroy()
{
    ShutdownTerminal();
    Super::BeginDestroy();
}

// 私有函数实现
void UMingSageBrainConsoleTerminal::UpdateTerminalState()
{
    OnTerminalStateChanged.Broadcast(TerminalState);
}

void UMingSageBrainConsoleTerminal::ProcessCommand(const FString& Command)
{
    FString ParsedCommand;
    TArray<FString> Arguments;
    ParseCommand(Command, ParsedCommand, Arguments);

    // 检查快捷键
    if (ExecuteShortcut(Command))
    {
        return;
    }

    // 发送到圣者大脑
    if (bConnectedToSageBrain)
    {
        ForwardCommandToSageBrain(ParsedCommand, Arguments);
    }
    else
    {
        // 本地命令处理
        HandleLocalCommand(ParsedCommand, Arguments);
    }

    OnTerminalCommand.Broadcast(ParsedCommand, Arguments);
}

void UMingSageBrainConsoleTerminal::ParseCommand(const FString& Command, FString& OutCommand, TArray<FString>& OutArguments)
{
    TArray<FString> Tokens;
    Command.ParseIntoArray(Tokens, TEXT(" "), true);
    
    if (Tokens.Num() > 0)
    {
        OutCommand = Tokens[0];
        for (int32 i = 1; i < Tokens.Num(); i++)
        {
            OutArguments.Add(Tokens[i]);
        }
    }
    else
    {
        OutCommand.Empty();
        OutArguments.Empty();
    }
}

void UMingSageBrainConsoleTerminal::HandleSageBrainResponse(const FSystemResponse& Response)
{
    // 根据响应级别选择颜色
    EConsoleOutputLevel Level = EConsoleOutputLevel::Info;
    switch (Response.Level)
    {
    case EConsoleOutputLevel::Success:
        Level = EConsoleOutputLevel::Success;
        break;
    case EConsoleOutputLevel::Warning:
        Level = EConsoleOutputLevel::Warning;
        break;
    case EConsoleOutputLevel::Error:
        Level = EConsoleOutputLevel::Error;
        break;
    default:
        break;
    }

    AddOutput(Response.ResponseText, Level, TEXT("SageBrain"));

    // 如果有建议操作，也显示出来
    if (Response.SuggestedActions.Num() > 0)
    {
        FString Suggestions = TEXT("建议操作: ");
        for (const FString& Action : Response.SuggestedActions)
        {
            Suggestions += Action + TEXT(" ");
        }
        AddOutput(Suggestions, EConsoleOutputLevel::Info, TEXT("SageBrain"));
    }
}

void UMingSageBrainConsoleTerminal::UpdateCursorPosition(int32 NewPosition)
{
    TerminalState.CursorPosition = FMath::Clamp(NewPosition, 0, TerminalState.CurrentInput.Len());
    bNeedsRedraw = true;
}

void UMingSageBrainConsoleTerminal::ValidateCursorPosition()
{
    TerminalState.CursorPosition = FMath::Clamp(
        TerminalState.CursorPosition,
        0,
        TerminalState.CurrentInput.Len()
    );
}

FString UMingSageBrainConsoleTerminal::FormatOutputLine(const FConsoleOutputLine& Line) const
{
    FString FormattedLine;

    // 添加行号
    if (TerminalConfig.bShowLineNumbers)
    {
        FormattedLine += FString::Printf(TEXT("%4d: "), Line.LineNumber);
    }

    // 添加时间戳
    if (TerminalConfig.bShowTimestamp)
    {
        FormattedLine += FString::Printf(TEXT("[%s] "), *Line.Timestamp.ToString(TEXT("%H:%M:%S")));
    }

    // 添加级别前缀
    FormattedLine += GetLevelPrefix(Line.Level);
    
    // 添加源
    if (!Line.Source.IsEmpty())
    {
        FormattedLine += FString::Printf(TEXT("[%s] "), *Line.Source);
    }

    // 添加文本内容
    FormattedLine += Line.Text;

    return FormattedLine;
}

FString UMingSageBrainConsoleTerminal::ApplyColorToText(const FString& Text, const FLinearColor& Color) const
{
    // 这里应该实现ANSI颜色代码或UE的富文本格式
    // 简化实现，只返回原文本
    return Text;
}

FString UMingSageBrainConsoleTerminal::GetTimestampString(const FDateTime& Timestamp) const
{
    return Timestamp.ToString(TEXT("%H:%M:%S"));
}

FString UMingSageBrainConsoleTerminal::GetLevelPrefix(EConsoleOutputLevel Level) const
{
    switch (Level)
    {
    case EConsoleOutputLevel::Debug:
        return TEXT("[DEBUG] ");
    case EConsoleOutputLevel::Info:
        return TEXT("[INFO] ");
    case EConsoleOutputLevel::Warning:
        return TEXT("[WARN] ");
    case EConsoleOutputLevel::Error:
        return TEXT("[ERROR] ");
    case EConsoleOutputLevel::Success:
        return TEXT("[SUCCESS] ");
    default:
        return TEXT("[UNKNOWN] ");
    }
}

FLinearColor UMingSageBrainConsoleTerminal::GetColorForLevel(EConsoleOutputLevel Level) const
{
    switch (Level)
    {
    case EConsoleOutputLevel::Debug:
        return FLinearColor(0.7f, 0.7f, 0.7f, 1.0f);
    case EConsoleOutputLevel::Info:
        return TerminalConfig.Theme.TextColor;
    case EConsoleOutputLevel::Warning:
        return TerminalConfig.Theme.WarningColor;
    case EConsoleOutputLevel::Error:
        return TerminalConfig.Theme.ErrorColor;
    case EConsoleOutputLevel::Success:
        return TerminalConfig.Theme.SuccessColor;
    default:
        return FLinearColor::White;
    }
}

void UMingSageBrainConsoleTerminal::InitializeDefaultThemes()
{
    // 确保有默认主题
    if (!GetAvailableThemes().Contains(TEXT("Default")))
    {
        // 创建默认主题
        FConsoleTheme DefaultTheme;
        DefaultTheme.BackgroundColor = FLinearColor(0.05f, 0.05f, 0.1f, 0.9f);
        DefaultTheme.TextColor = FLinearColor(0.9f, 0.9f, 0.9f, 1.0f);
        DefaultTheme.AccentColor = FLinearColor(0.2f, 0.6f, 1.0f, 1.0f);
        DefaultTheme.FontName = TEXT("Consolas");
        DefaultTheme.FontSize = 14;
        
        TerminalConfig.Theme = DefaultTheme;
    }
}

void UMingSageBrainConsoleTerminal::ApplyThemeToOutput()
{
    // 应用主题到现有输出
    for (FConsoleOutputLine& Line : OutputHistory)
    {
        Line.Color = GetColorForLevel(Line.Level);
    }
    bNeedsRedraw = true;
}

FConsoleTheme UMingSageBrainConsoleTerminal::GetThemeByName(const FString& ThemeName) const
{
    FConsoleTheme Theme = TerminalConfig.Theme; // 默认返回当前主题
    
    if (ThemeName == TEXT("Dark"))
    {
        Theme.BackgroundColor = FLinearColor(0.1f, 0.1f, 0.15f, 0.95f);
        Theme.TextColor = FLinearColor(0.8f, 0.8f, 0.8f, 1.0f);
    }
    else if (ThemeName == TEXT("Light"))
    {
        Theme.BackgroundColor = FLinearColor(0.95f, 0.95f, 0.9f, 0.95f);
        Theme.TextColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.0f);
    }
    else if (ThemeName == TEXT("Blue"))
    {
        Theme.BackgroundColor = FLinearColor(0.05f, 0.05f, 0.15f, 0.9f);
        Theme.AccentColor = FLinearColor(0.3f, 0.7f, 1.0f, 1.0f);
    }
    else if (ThemeName == TEXT("Green"))
    {
        Theme.BackgroundColor = FLinearColor(0.05f, 0.1f, 0.05f, 0.9f);
        Theme.AccentColor = FLinearColor(0.2f, 0.8f, 0.3f, 1.0f);
    }
    
    return Theme;
}

bool UMingSageBrainConsoleTerminal::HandleSpecialKey(const FString& Key)
{
    if (Key == TEXT("Enter"))
    {
        ProcessInput(TerminalState.CurrentInput);
        TerminalState.CurrentInput.Empty();
        TerminalState.CursorPosition = 0;
        TerminalState.HistoryIndex = -1;
        return true;
    }
    else if (Key == TEXT("Escape"))
    {
        TerminalState.CurrentInput.Empty();
        TerminalState.CursorPosition = 0;
        TerminalState.HistoryIndex = -1;
        bNeedsRedraw = true;
        return true;
    }
    else if (Key == TEXT("Up"))
    {
        FString PreviousInput = GetPreviousInput();
        if (!PreviousInput.IsEmpty())
        {
            TerminalState.CurrentInput = PreviousInput;
            TerminalState.CursorPosition = PreviousInput.Len();
            bNeedsRedraw = true;
        }
        return true;
    }
    else if (Key == TEXT("Down"))
    {
        FString NextInput = GetNextInput();
        if (NextInput.IsEmpty())
        {
            TerminalState.CurrentInput.Empty();
            TerminalState.CursorPosition = 0;
        }
        else
        {
            TerminalState.CurrentInput = NextInput;
            TerminalState.CursorPosition = NextInput.Len();
        }
        bNeedsRedraw = true;
        return true;
    }
    
    return false;
}

void UMingSageBrainConsoleTerminal::ExecuteCommandWithHistory(const FString& Command)
{
    AddToInputHistory(Command);
    ProcessCommand(Command);
    TerminalState.CurrentInput.Empty();
    TerminalState.CursorPosition = 0;
    TerminalState.HistoryIndex = -1;
}

void UMingSageBrainConsoleTerminal::UpdateHistoryNavigation(const FString& CurrentInput)
{
    // 当用户开始输入时，重置历史索引
    if (TerminalState.HistoryIndex >= 0)
    {
        TerminalState.HistoryIndex = -1;
    }
}

void UMingSageBrainConsoleTerminal::OnSageBrainOutput(const FConsoleOutputItem& OutputItem)
{
    EConsoleOutputLevel Level = EConsoleOutputLevel::Info;
    switch (OutputItem.Level)
    {
    case EConsoleOutputLevel::Debug:
        Level = EConsoleOutputLevel::Debug;
        break;
    case EConsoleOutputLevel::Info:
        Level = EConsoleOutputLevel::Info;
        break;
    case EConsoleOutputLevel::Warning:
        Level = EConsoleOutputLevel::Warning;
        break;
    case EConsoleOutputLevel::Error:
        Level = EConsoleOutputLevel::Error;
        break;
    case EConsoleOutputLevel::Success:
        Level = EConsoleOutputLevel::Success;
        break;
    }

    AddOutput(OutputItem.Message, Level, OutputItem.Category);
}

void UMingSageBrainConsoleTerminal::OnSageBrainResponse(const FSystemResponse& Response)
{
    HandleSageBrainResponse(Response);
}

void UMingSageBrainConsoleTerminal::OnSageBrainInput(const FUserInputContext& InputContext)
{
    // 显示圣者大脑接收到的输入
    AddOutput(FString::Printf(TEXT("输入: %s"), *InputContext.InputText), 
              EConsoleOutputLevel::Debug, TEXT("SageBrain"));
}

void UMingSageBrainConsoleTerminal::ForwardCommandToSageBrain(const FString& Command, const TArray<FString>& Arguments)
{
    if (!bConnectedToSageBrain || !SageBrainConsole)
    {
        return;
    }

    FString FullCommand = Command;
    for (const FString& Arg : Arguments)
    {
        FullCommand += TEXT(" ") + Arg;
    }

    SendCommandToSageBrain(FullCommand);
}

void UMingSageBrainConsoleTerminal::HandleLocalCommand(const FString& Command, const TArray<FString>& Arguments)
{
    if (Command == TEXT("clear") || Command == TEXT("cls"))
    {
        ClearOutput();
    }
    else if (Command == TEXT("help") || Command == TEXT("?"))
    {
        AddOutput(TEXT("本地命令:\n")
                  TEXT("  clear/cls - 清除输出\n")
                  TEXT("  help/?    - 显示帮助\n")
                  TEXT("  status    - 显示状态\n")
                  TEXT("  theme     - 切换主题"),
                  EConsoleOutputLevel::Info, TEXT("Terminal"));
    }
    else if (Command == TEXT("status"))
    {
        AddOutput(GetStatusString(), EConsoleOutputLevel::Info, TEXT("Terminal"));
    }
    else if (Command == TEXT("theme"))
    {
        if (Arguments.Num() > 0)
        {
            LoadTheme(Arguments[0]);
        }
        else
        {
            FString AvailableThemes = TEXT("可用主题: ");
            for (const FString& ThemeName : GetAvailableThemes())
            {
                AvailableThemes += ThemeName + TEXT(" ");
            }
            AddOutput(AvailableThemes, EConsoleOutputLevel::Info, TEXT("Terminal"));
        }
    }
    else
    {
        AddOutput(FString::Printf(TEXT("未知命令: %s"), *Command), 
                  EConsoleOutputLevel::Error, TEXT("Terminal"));
    }
}

void UMingSageBrainConsoleTerminal::LogTerminalEvent(const FString& Event, const FString& Details)
{
    UE_LOG(LogTemp, Log, TEXT("[Terminal] %s: %s"), *Event, *Details);
}

void UMingSageBrainConsoleTerminal::CleanupResources()
{
    OutputHistory.Empty();
    TerminalState.InputHistory.Empty();
    Shortcuts.Empty();
    bNeedsRedraw = false;
}

int32 UMingSageBrainConsoleTerminal::GetMaxVisibleLines() const
{
    // 这里应该根据实际的UI大小计算
    // 简化实现，返回固定值
    return 25;
}

bool UMingSageBrainConsoleTerminal::ShouldAutoScroll() const
{
    return TerminalConfig.bAutoScroll && 
           (ScrollPosition >= OutputHistory.Num() - VisibleLines - 1);
}
