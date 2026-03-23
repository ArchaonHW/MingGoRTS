#include "MingSageBrainConsoleLauncher.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "TimerManager.h"
#include "Async/Async.h"

UMingSageBrainConsoleLauncher::UMingSageBrainConsoleLauncher()
    : LaunchState(EConsoleLaunchState::NotStarted)
    , bIsInitialized(false)
    , bAutoExecuteStartupCommands(true)
    , LaunchStartTime(0.0f)
{
}

bool UMingSageBrainConsoleLauncher::LaunchConsole(const FConsoleLaunchConfig& LaunchConfig)
{
    if (LaunchState != EConsoleLaunchState::NotStarted)
    {
        LogLaunchEvent(TEXT("启动失败"), TEXT("控制台已经启动"));
        return false;
    }

    if (!ValidateConfig(LaunchConfig))
    {
        HandleLaunchError(TEXT("启动配置无效"));
        return false;
    }

    CurrentConfig = LaunchConfig;
    LaunchStartTime = FPlatformTime::Seconds();

    SetLaunchState(EConsoleLaunchState::Initializing);

    UE_LOG(LogTemp, Log, TEXT("开始启动圣者大脑控制台..."));

    // 初始化组件
    if (!InitializeComponents())
    {
        HandleLaunchError(TEXT("组件初始化失败"));
        return false;
    }

    // 连接组件
    if (!ConnectComponents())
    {
        HandleLaunchError(TEXT("组件连接失败"));
        return false;
    }

    // 应用配置
    ApplyLaunchConfig();

    // 显示终端
    if (CurrentConfig.bShowTerminalOnStart)
    {
        ConsoleTerminal->ShowTerminal();
    }

    // 启动交互式控制台
    if (CurrentConfig.bAutoStartSageBrain)
    {
        InteractiveConsole->RunMainLoop();
    }

    // 执行启动命令
    ExecuteDefaultStartupCommands();

    CompleteLaunch(true);
    return true;
}

void UMingSageBrainConsoleLauncher::ShutdownConsole()
{
    if (LaunchState != EConsoleLaunchState::Running)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("关闭圣者大脑控制台..."));

    // 关闭交互式控制台
    if (InteractiveConsole)
    {
        InteractiveConsole->StopMainLoop();
        InteractiveConsole->ShutdownInteractiveConsole();
    }

    // 关闭终端
    if (ConsoleTerminal)
    {
        ConsoleTerminal->HideTerminal();
        ConsoleTerminal->ShutdownTerminal();
    }

    // 清理资源
    CleanupResources();

    SetLaunchState(EConsoleLaunchState::Stopped);
}

void UMingSageBrainConsoleLauncher::RestartConsole()
{
    UE_LOG(LogTemp, Log, TEXT("重启圣者大脑控制台..."));

    // 保存当前配置
    FConsoleLaunchConfig SavedConfig = CurrentConfig;

    // 关闭
    ShutdownConsole();

    // 等待关闭完成
    FPlatformProcess::Sleep(0.5f);

    // 重新启动
    LaunchConsole(SavedConfig);
}

bool UMingSageBrainConsoleLauncher::QuickLaunch()
{
    FConsoleLaunchConfig QuickConfig;
    QuickConfig.bAutoStartSageBrain = true;
    QuickConfig.bShowTerminalOnStart = true;
    QuickConfig.bEnableAutoComplete = true;
    QuickConfig.bEnableSyntaxHighlighting = true;
    QuickConfig.DefaultTheme = TEXT("Default");
    QuickConfig.DefaultDisplayMode = EConsoleDisplayMode::Enhanced;
    QuickConfig.DefaultInteractionMode = EUserInteractionMode::CommandLine;

    return LaunchConsole(QuickConfig);
}

bool UMingSageBrainConsoleLauncher::LaunchWithDefaults()
{
    return QuickLaunch();
}

bool UMingSageBrainConsoleLauncher::LaunchInMode(EUserInteractionMode Mode)
{
    FConsoleLaunchConfig Config;
    Config.bAutoStartSageBrain = true;
    Config.bShowTerminalOnStart = true;
    Config.bEnableAutoComplete = true;
    Config.bEnableSyntaxHighlighting = true;
    Config.DefaultTheme = TEXT("Default");
    Config.DefaultDisplayMode = EConsoleDisplayMode::Enhanced;
    Config.DefaultInteractionMode = Mode;

    return LaunchConsole(Config);
}

bool UMingSageBrainConsoleLauncher::AreComponentsReady() const
{
    return ConsoleTerminal && InteractiveConsole &&
           ConsoleTerminal->IsReady() &&
           InteractiveConsole->IsInitialized();
}

void UMingSageBrainConsoleLauncher::SetLaunchConfig(const FConsoleLaunchConfig& NewConfig)
{
    CurrentConfig = NewConfig;
    
    // 如果控制台已运行，应用新配置
    if (LaunchState == EConsoleLaunchState::Running)
    {
        ApplyLaunchConfig();
    }
}

void UMingSageBrainConsoleLauncher::SaveConfigToFile(const FString& FilePath)
{
    FString JsonContent = TEXT("{\n");
    JsonContent += FString::Printf(TEXT("  \"bAutoStartSageBrain\": %s,\n"), 
        CurrentConfig.bAutoStartSageBrain ? TEXT("true") : TEXT("false"));
    JsonContent += FString::Printf(TEXT("  \"bShowTerminalOnStart\": %s,\n"), 
        CurrentConfig.bShowTerminalOnStart ? TEXT("true") : TEXT("false"));
    JsonContent += FString::Printf(TEXT("  \"bEnableAutoComplete\": %s,\n"), 
        CurrentConfig.bEnableAutoComplete ? TEXT("true") : TEXT("false"));
    JsonContent += FString::Printf(TEXT("  \"bEnableSyntaxHighlighting\": %s,\n"), 
        CurrentConfig.bEnableSyntaxHighlighting ? TEXT("true") : TEXT("false"));
    JsonContent += FString::Printf(TEXT("  \"DefaultTheme\": \"%s\",\n"), *CurrentConfig.DefaultTheme);
    JsonContent += FString::Printf(TEXT("  \"DefaultDisplayMode\": %d,\n"), static_cast<int32>(CurrentConfig.DefaultDisplayMode));
    JsonContent += FString::Printf(TEXT("  \"DefaultInteractionMode\": %d\n"), static_cast<int32>(CurrentConfig.DefaultInteractionMode));
    JsonContent += TEXT("}");

    if (FFileHelper::SaveStringToFile(JsonContent, *FilePath))
    {
        LogLaunchEvent(TEXT("配置保存"), FString::Printf(TEXT("配置已保存到: %s"), *FilePath));
    }
    else
    {
        LogLaunchEvent(TEXT("配置保存失败"), FString::Printf(TEXT("无法保存到: %s"), *FilePath));
    }
}

void UMingSageBrainConsoleLauncher::LoadConfigFromFile(const FString& FilePath)
{
    FString JsonContent;
    if (!FFileHelper::LoadFileToString(JsonContent, *FilePath))
    {
        LogLaunchEvent(TEXT("配置加载失败"), FString::Printf(TEXT("无法读取: %s"), *FilePath));
        return;
    }

    // 简化的JSON解析
    FConsoleLaunchConfig LoadedConfig;
    
    if (JsonContent.Contains(TEXT("\"bAutoStartSageBrain\": true")))
    {
        LoadedConfig.bAutoStartSageBrain = true;
    }
    
    if (JsonContent.Contains(TEXT("\"bShowTerminalOnStart\": true")))
    {
        LoadedConfig.bShowTerminalOnStart = true;
    }
    
    if (JsonContent.Contains(TEXT("\"bEnableAutoComplete\": true")))
    {
        LoadedConfig.bEnableAutoComplete = true;
    }
    
    if (JsonContent.Contains(TEXT("\"bEnableSyntaxHighlighting\": true")))
    {
        LoadedConfig.bEnableSyntaxHighlighting = true;
    }

    SetLaunchConfig(LoadedConfig);
    LogLaunchEvent(TEXT("配置加载"), FString::Printf(TEXT("配置已从 %s 加载"), *FilePath));
}

FString UMingSageBrainConsoleLauncher::GetStatusString() const
{
    FString Status = TEXT("=== 圣者大脑控制台启动器状态 ===\n");
    
    switch (LaunchState)
    {
    case EConsoleLaunchState::NotStarted:
        Status += TEXT("状态: 未启动\n");
        break;
    case EConsoleLaunchState::Initializing:
        Status += TEXT("状态: 初始化中\n");
        break;
    case EConsoleLaunchState::Running:
        Status += TEXT("状态: 运行中\n");
        break;
    case EConsoleLaunchState::Error:
        Status += TEXT("状态: 错误\n");
        break;
    case EConsoleLaunchState::Stopped:
        Status += TEXT("状态: 已停止\n");
        break;
    }

    Status += FString::Printf(TEXT("圣者大脑连接: %s\n"), 
        IsSageBrainConnected() ? TEXT("是") : TEXT("否"));
    Status += FString::Printf(TEXT("组件就绪: %s\n"), 
        AreComponentsReady() ? TEXT("是") : TEXT("否"));
    
    if (LaunchState == EConsoleLaunchState::Running)
    {
        Status += FString::Printf(TEXT("运行时间: %.1f 秒\n"), GetLaunchDuration());
    }
    
    if (!LastErrorMessage.IsEmpty())
    {
        Status += FString::Printf(TEXT("最后错误: %s\n"), *LastErrorMessage);
    }

    return Status;
}

bool UMingSageBrainConsoleLauncher::IsSageBrainConnected() const
{
    return InteractiveConsole && InteractiveConsole->IsInitialized();
}

void UMingSageBrainConsoleLauncher::ExecuteStartupCommands(const TArray<FString>& Commands)
{
    if (!AreComponentsReady())
    {
        return;
    }

    LogLaunchEvent(TEXT("执行启动命令"), FString::Printf(TEXT("执行 %d 个启动命令"), Commands.Num()));

    for (const FString& Command : Commands)
    {
        ConsoleTerminal->ProcessInput(Command);
        
        // 短暂延迟以避免命令冲突
        FPlatformProcess::Sleep(0.1f);
    }
}

void UMingSageBrainConsoleLauncher::RunStartupScript(const FString& ScriptPath)
{
    if (!FPaths::FileExists(ScriptPath))
    {
        LogLaunchEvent(TEXT("脚本执行失败"), FString::Printf(TEXT("脚本文件不存在: %s"), *ScriptPath));
        return;
    }

    FString ScriptContent;
    if (!FFileHelper::LoadFileToString(ScriptContent, *ScriptPath))
    {
        LogLaunchEvent(TEXT("脚本执行失败"), FString::Printf(TEXT("无法读取脚本: %s"), *ScriptPath));
        return;
    }

    TArray<FString> Commands;
    ScriptContent.ParseIntoArrayLines(Commands, true);

    ExecuteStartupCommands(Commands);
    LogLaunchEvent(TEXT("脚本执行完成"), FString::Printf(TEXT("已执行脚本: %s"), *ScriptPath));
}

void UMingSageBrainConsoleLauncher::SetAutoExecuteCommands(bool bAutoExecute)
{
    bAutoExecuteStartupCommands = bAutoExecute;
    LogLaunchEvent(TEXT("自动执行设置"), bAutoExecute ? TEXT("已启用") : TEXT("已禁用"));
}

void UMingSageBrainConsoleLauncher::BeginDestroy()
{
    ShutdownConsole();
    Super::BeginDestroy();
}

// 私有函数实现
bool UMingSageBrainConsoleLauncher::InitializeComponents()
{
    // 初始化终端
    if (!InitializeTerminal())
    {
        return false;
    }

    // 初始化交互式控制台
    if (!InitializeInteractiveConsole())
    {
        return false;
    }

    bIsInitialized = true;
    return true;
}

bool UMingSageBrainConsoleLauncher::InitializeTerminal()
{
    ConsoleTerminal = NewObject<UMingSageBrainConsoleTerminal>(this);
    if (!ConsoleTerminal)
    {
        LogLaunchEvent(TEXT("终端初始化失败"), TEXT("无法创建终端对象"));
        return false;
    }

    FConsoleTerminalConfig TerminalConfig;
    TerminalConfig.DisplayMode = CurrentConfig.DefaultDisplayMode;
    TerminalConfig.bShowTimestamp = true;
    TerminalConfig.bShowPrompt = true;
    TerminalConfig.bAutoScroll = true;
    TerminalConfig.MaxHistoryLines = 1000;
    TerminalConfig.UpdateInterval = 0.1f;

    if (!ConsoleTerminal->InitializeTerminal(TerminalConfig))
    {
        LogLaunchEvent(TEXT("终端初始化失败"), TEXT("终端初始化方法返回失败"));
        return false;
    }

    LogLaunchEvent(TEXT("终端初始化成功"), TEXT("终端组件已初始化"));
    return true;
}

bool UMingSageBrainConsoleLauncher::InitializeInteractiveConsole()
{
    InteractiveConsole = NewObject<UMingSageBrainInteractiveConsole>(this);
    if (!InteractiveConsole)
    {
        LogLaunchEvent(TEXT("控制台初始化失败"), TEXT("无法创建交互式控制台对象"));
        return false;
    }

    if (!InteractiveConsole->InitializeInteractiveConsole())
    {
        LogLaunchEvent(TEXT("控制台初始化失败"), TEXT("交互式控制台初始化方法返回失败"));
        return false;
    }

    LogLaunchEvent(TEXT("控制台初始化成功"), TEXT("交互式控制台组件已初始化"));
    return true;
}

bool UMingSageBrainConsoleLauncher::ConnectComponents()
{
    if (!ConsoleTerminal || !InteractiveConsole)
    {
        return false;
    }

    // 连接终端到交互式控制台
    ConsoleTerminal->ConnectToSageBrain(InteractiveConsole);

    // 绑定事件
    BindTerminalEvents();
    BindConsoleEvents();

    // 设置组件通信
    SetupComponentCommunication();

    LogLaunchEvent(TEXT("组件连接成功"), TEXT("终端与交互式控制台已连接"));
    return true;
}

void UMingSageBrainConsoleLauncher::SetLaunchState(EConsoleLaunchState NewState)
{
    if (LaunchState != NewState)
    {
        LaunchState = NewState;
        OnConsoleLaunchStateChanged.Broadcast(NewState);
        
        UE_LOG(LogTemp, Log, TEXT("启动器状态变更为: %d"), static_cast<int32>(NewState));
    }
}

void UMingSageBrainConsoleLauncher::CompleteLaunch(bool bSuccess)
{
    if (bSuccess)
    {
        SetLaunchState(EConsoleLaunchState::Running);
        OnConsoleLaunched.Broadcast(true);
        OnSageBrainConnected.Broadcast(true);
        
        float Duration = GetLaunchDuration();
        LogLaunchEvent(TEXT("启动完成"), FString::Printf(TEXT("控制台启动成功，耗时 %.2f 秒"), Duration));
        
        // 显示欢迎信息
        if (ConsoleTerminal)
        {
            ConsoleTerminal->AddOutput(TEXT("🧠 圣者大脑控制台已成功启动！"), 
                EConsoleOutputLevel::Success, TEXT("Launcher"));
            ConsoleTerminal->AddOutput(TEXT("输入 'help' 查看可用命令"), 
                EConsoleOutputLevel::Info, TEXT("Launcher"));
        }
    }
    else
    {
        SetLaunchState(EConsoleLaunchState::Error);
        OnConsoleLaunched.Broadcast(false);
        OnSageBrainConnected.Broadcast(false);
    }
}

void UMingSageBrainConsoleLauncher::HandleLaunchError(const FString& Error)
{
    LastErrorMessage = Error;
    SetLaunchState(EConsoleLaunchState::Error);
    
    UE_LOG(LogTemp, Error, TEXT("控制台启动失败: %s"), *Error);
    LogLaunchEvent(TEXT("启动错误"), Error);
    
    // 清理部分初始化的组件
    CleanupResources();
}

void UMingSageBrainConsoleLauncher::BindTerminalEvents()
{
    if (!ConsoleTerminal)
    {
        return;
    }

    // 绑定终端输入事件，转发到交互式控制台
    ConsoleTerminal->OnTerminalInput.AddDynamic(
        this, &UMingSageBrainConsoleLauncher::OnTerminalInput);
}

void UMingSageBrainConsoleLauncher::BindConsoleEvents()
{
    if (!InteractiveConsole)
    {
        return;
    }

    // 绑定控制台输出事件，显示在终端中
    InteractiveConsole->OnSystemResponseReady.AddDynamic(
        this, &UMingSageBrainConsoleLauncher::OnSageBrainResponse);
}

void UMingSageBrainConsoleLauncher::SetupComponentCommunication()
{
    // 设置自动执行
    if (ConsoleTerminal)
    {
        ConsoleTerminal->SetAutoExecuteCommands(bAutoExecuteStartupCommands);
    }
}

void UMingSageBrainConsoleLauncher::ApplyLaunchConfig()
{
    if (!AreComponentsReady())
    {
        return;
    }

    SetupTerminalTheme();
    SetupInteractionMode();
}

void UMingSageBrainConsoleLauncher::SetupTerminalTheme()
{
    if (ConsoleTerminal && !CurrentConfig.DefaultTheme.IsEmpty())
    {
        ConsoleTerminal->LoadTheme(CurrentConfig.DefaultTheme);
    }
}

void UMingSageBrainConsoleLauncher::SetupInteractionMode()
{
    if (InteractiveConsole)
    {
        InteractiveConsole->SetInteractionMode(CurrentConfig.DefaultInteractionMode);
    }
}

void UMingSageBrainConsoleLauncher::ExecuteDefaultStartupCommands()
{
    if (!bAutoExecuteStartupCommands)
    {
        return;
    }

    TArray<FString> DefaultCommands = GetDefaultStartupCommands();
    ExecuteStartupCommands(DefaultCommands);
}

TArray<FString> UMingSageBrainConsoleLauncher::GetDefaultStartupCommands()
{
    TArray<FString> Commands;
    
    Commands.Add(TEXT("help"));
    Commands.Add(TEXT("status"));
    
    if (CurrentConfig.bAutoStartSageBrain)
    {
        Commands.Add(TEXT("mode command"));
    }
    
    return Commands;
}

void UMingSageBrainConsoleLauncher::ValidateComponentStates()
{
    bool bTerminalReady = ConsoleTerminal && ConsoleTerminal->IsReady();
    bool bConsoleReady = InteractiveConsole && InteractiveConsole->IsInitialized();
    bool bConnected = IsSageBrainConnected();

    if (!bTerminalReady || !bConsoleReady || !bConnected)
    {
        LogLaunchEvent(TEXT("组件状态异常"), TEXT("部分组件未就绪"));
    }
}

void UMingSageBrainConsoleLauncher::UpdateLaunchStatus()
{
    ValidateComponentStates();
}

void UMingSageBrainConsoleLauncher::LogLaunchEvent(const FString& Event, const FString& Details)
{
    UE_LOG(LogTemp, Log, TEXT("[Launcher] %s: %s"), *Event, *Details);
}

void UMingSageBrainConsoleLauncher::CleanupResources()
{
    ConsoleTerminal = nullptr;
    InteractiveConsole = nullptr;
    bIsInitialized = false;
    LastErrorMessage.Empty();
}

float UMingSageBrainConsoleLauncher::GetLaunchDuration() const
{
    if (LaunchStartTime > 0.0f)
    {
        return FPlatformTime::Seconds() - LaunchStartTime;
    }
    return 0.0f;
}

bool UMingSageBrainConsoleLauncher::ValidateConfig(const FConsoleLaunchConfig& Config)
{
    // 基本验证
    if (Config.DefaultTheme.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("启动配置: 主题名为空"));
    }

    return true; // 允许所有配置，只记录警告
}

// 事件处理函数
UFUNCTION()
void UMingSageBrainConsoleLauncher::OnTerminalInput(const FString& Input)
{
    if (InteractiveConsole && InteractiveConsole->IsInitialized())
    {
        // 终端输入直接转发到交互式控制台
        InteractiveConsole->ProcessUserInput(Input);
    }
}

UFUNCTION()
void UMingSageBrainConsoleLauncher::OnSageBrainResponse(const FSystemResponse& Response)
{
    if (ConsoleTerminal)
    {
        // 圣者大脑响应显示在终端中
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

        ConsoleTerminal->AddOutput(Response.ResponseText, Level, TEXT("SageBrain"));
    }
}
