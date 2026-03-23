#include "MingSageBrainConsoleStarter.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

// 静态成员定义
TObjectPtr<UMingSageBrainConsoleLauncher> UMingSageBrainConsoleStarter::ConsoleLauncher = nullptr;
bool UMingSageBrainConsoleStarter::bIsInitialized = false;

UMingSageBrainConsoleStarter::UMingSageBrainConsoleStarter()
{
}

bool UMingSageBrainConsoleStarter::StartSageBrainConsole()
{
    LogStarterEvent(TEXT("启动圣者大脑控制台"), TEXT("开始启动流程"));

    if (!EnsureLauncherExists())
    {
        LogStarterEvent(TEXT("启动失败"), TEXT("无法创建启动器实例"));
        return false;
    }

    FConsoleLaunchConfig Config = CreateDefaultConfig();
    bool bSuccess = ConsoleLauncher->LaunchConsole(Config);

    if (bSuccess)
    {
        LogStarterEvent(TEXT("启动成功"), TEXT("圣者大脑控制台已成功启动"));
        
        // 显示欢迎信息
        if (ConsoleLauncher->GetTerminal())
        {
            ConsoleLauncher->GetTerminal()->AddOutput(
                TEXT("🧠 圣者大脑控制台已启动！"),
                EConsoleOutputLevel::Success,
                TEXT("Starter")
            );
            ConsoleLauncher->GetTerminal()->AddOutput(
                TEXT("💡 输入 'help' 查看可用命令"),
                EConsoleOutputLevel::Info,
                TEXT("Starter")
            );
        }
    }
    else
    {
        LogStarterEvent(TEXT("启动失败"), TEXT("控制台启动失败"));
    }

    return bSuccess;
}

bool UMingSageBrainConsoleStarter::QuickStart()
{
    LogStarterEvent(TEXT("快速启动"), TEXT("使用默认配置快速启动"));

    if (!EnsureLauncherExists())
    {
        return false;
    }

    return ConsoleLauncher->QuickLaunch();
}

bool UMingSageBrainConsoleStarter::StartAndShow()
{
    LogStarterEvent(TEXT("启动并显示"), TEXT("启动控制台并显示终端"));

    if (!EnsureLauncherExists())
    {
        return false;
    }

    FConsoleLaunchConfig Config = CreateDefaultConfig();
    Config.bShowTerminalOnStart = true;
    
    bool bSuccess = ConsoleLauncher->LaunchConsole(Config);
    
    if (bSuccess && ConsoleLauncher->GetTerminal())
    {
        ConsoleLauncher->GetTerminal()->ShowTerminal();
    }

    return bSuccess;
}

bool UMingSageBrainConsoleStarter::StartChatMode()
{
    LogStarterEvent(TEXT("启动聊天模式"), TEXT("启动聊天交互模式"));

    if (!EnsureLauncherExists())
    {
        return false;
    }

    FConsoleLaunchConfig Config = CreateDefaultConfig();
    Config.DefaultInteractionMode = EUserInteractionMode::Chat;
    Config.bShowTerminalOnStart = true;
    
    bool bSuccess = ConsoleLauncher->LaunchConsole(Config);
    
    if (bSuccess && ConsoleLauncher->GetTerminal())
    {
        ConsoleLauncher->GetTerminal()->AddOutput(
            TEXT("💬 已切换到聊天模式"),
            EConsoleOutputLevel::Success,
            TEXT("Starter")
        );
    }

    return bSuccess;
}

bool UMingSageBrainConsoleStarter::StartDialogueMode()
{
    LogStarterEvent(TEXT("启动对话模式"), TEXT("启动对话交互模式"));

    if (!EnsureLauncherExists())
    {
        return false;
    }

    FConsoleLaunchConfig Config = CreateDefaultConfig();
    Config.DefaultInteractionMode = EUserInteractionMode::Dialogue;
    Config.bShowTerminalOnStart = true;
    
    bool bSuccess = ConsoleLauncher->LaunchConsole(Config);
    
    if (bSuccess && ConsoleLauncher->GetTerminal())
    {
        ConsoleLauncher->GetTerminal()->AddOutput(
            TEXT("🎭 已切换到对话模式"),
            EConsoleOutputLevel::Success,
            TEXT("Starter")
        );
    }

    return bSuccess;
}

bool UMingSageBrainConsoleStarter::StartWizardMode()
{
    LogStarterEvent(TEXT("启动向导模式"), TEXT("启动向导交互模式"));

    if (!EnsureLauncherExists())
    {
        return false;
    }

    FConsoleLaunchConfig Config = CreateDefaultConfig();
    Config.DefaultInteractionMode = EUserInteractionMode::Wizard;
    Config.bShowTerminalOnStart = true;
    
    bool bSuccess = ConsoleLauncher->LaunchConsole(Config);
    
    if (bSuccess && ConsoleLauncher->GetTerminal())
    {
        ConsoleLauncher->GetTerminal()->AddOutput(
            TEXT("🧙 已切换到向导模式"),
            EConsoleOutputLevel::Success,
            TEXT("Starter")
        );
    }

    return bSuccess;
}

bool UMingSageBrainConsoleStarter::CustomStart(EUserInteractionMode Mode, const FString& Theme, bool bShowTerminal)
{
    LogStarterEvent(TEXT("自定义启动"), FString::Printf(TEXT("模式: %d, 主题: %s, 显示终端: %s"), 
        static_cast<int32>(Mode), *Theme, bShowTerminal ? TEXT("是") : TEXT("否")));

    if (!EnsureLauncherExists())
    {
        return false;
    }

    FConsoleLaunchConfig Config = CreateDefaultConfig();
    Config.DefaultInteractionMode = Mode;
    Config.DefaultTheme = Theme;
    Config.bShowTerminalOnStart = bShowTerminal;
    
    return ConsoleLauncher->LaunchConsole(Config);
}

bool UMingSageBrainConsoleStarter::StartWithCommands(const TArray<FString>& Commands)
{
    LogStarterEvent(TEXT("启动并执行命令"), FString::Printf(TEXT("命令数量: %d"), Commands.Num()));

    if (!StartSageBrainConsole())
    {
        return false;
    }

    // 等待启动完成
    FPlatformProcess::Sleep(1.0f);

    return ExecuteCommands(Commands);
}

bool UMingSageBrainConsoleStarter::StartWithScript(const FString& ScriptPath)
{
    LogStarterEvent(TEXT("启动并加载脚本"), FString::Printf(TEXT("脚本路径: %s"), *ScriptPath));

    if (!StartSageBrainConsole())
    {
        return false;
    }

    // 等待启动完成
    FPlatformProcess::Sleep(1.0f);

    if (ConsoleLauncher && ConsoleLauncher->GetTerminal())
    {
        ConsoleLauncher->RunStartupScript(ScriptPath);
        return true;
    }

    return false;
}

bool UMingSageBrainConsoleStarter::IsConsoleRunning()
{
    return ConsoleLauncher && ConsoleLauncher->IsRunning();
}

UMingSageBrainConsoleLauncher* UMingSageBrainConsoleStarter::GetConsoleLauncher()
{
    return ConsoleLauncher;
}

UMingSageBrainConsoleTerminal* UMingSageBrainConsoleStarter::GetTerminal()
{
    return ConsoleLauncher ? ConsoleLauncher->GetTerminal() : nullptr;
}

UMingSageBrainInteractiveConsole* UMingSageBrainConsoleStarter::GetInteractiveConsole()
{
    return ConsoleLauncher ? ConsoleLauncher->GetInteractiveConsole() : nullptr;
}

bool UMingSageBrainConsoleStarter::SendCommand(const FString& Command)
{
    if (!IsConsoleRunning())
    {
        LogStarterEvent(TEXT("命令发送失败"), TEXT("控制台未运行"));
        return false;
    }

    UMingSageBrainConsoleTerminal* Terminal = GetTerminal();
    if (Terminal)
    {
        Terminal->ProcessInput(Command);
        return true;
    }

    return false;
}

bool UMingSageBrainConsoleStarter::SendChatMessage(const FString& Message)
{
    if (!IsConsoleRunning())
    {
        return false;
    }

    // 切换到聊天模式并发送消息
    UMingSageBrainInteractiveConsole* Console = GetInteractiveConsole();
    if (Console)
    {
        Console->SetInteractionMode(EUserInteractionMode::Chat);
        return SendCommand(Message);
    }

    return false;
}

bool UMingSageBrainConsoleStarter::ShowHelp()
{
    return SendCommand(TEXT("help"));
}

bool UMingSageBrainConsoleStarter::ShowStatus()
{
    return SendCommand(TEXT("status"));
}

bool UMingSageBrainConsoleStarter::GenerateMusic(const FString& Description)
{
    FString Command = FString::Printf(TEXT("generate music %s"), *Description);
    return SendCommand(Command);
}

bool UMingSageBrainConsoleStarter::GenerateScene(const FString& Description)
{
    FString Command = FString::Printf(TEXT("generate scene %s"), *Description);
    return SendCommand(Command);
}

bool UMingSageBrainConsoleStarter::ShowTerminal()
{
    if (!IsConsoleRunning())
    {
        return false;
    }

    UMingSageBrainConsoleTerminal* Terminal = GetTerminal();
    if (Terminal)
    {
        Terminal->ShowTerminal();
        return true;
    }

    return false;
}

bool UMingSageBrainConsoleStarter::HideTerminal()
{
    if (!IsConsoleRunning())
    {
        return false;
    }

    UMingSageBrainConsoleTerminal* Terminal = GetTerminal();
    if (Terminal)
    {
        Terminal->HideTerminal();
        return true;
    }

    return false;
}

bool UMingSageBrainConsoleStarter::ToggleTerminal()
{
    if (!IsConsoleRunning())
    {
        return StartAndShow();
    }

    UMingSageBrainConsoleTerminal* Terminal = GetTerminal();
    if (Terminal)
    {
        Terminal->ToggleTerminal();
        return true;
    }

    return false;
}

bool UMingSageBrainConsoleStarter::SwitchTheme(const FString& ThemeName)
{
    if (!IsConsoleRunning())
    {
        return false;
    }

    UMingSageBrainConsoleTerminal* Terminal = GetTerminal();
    if (Terminal)
    {
        Terminal->LoadTheme(ThemeName);
        return true;
    }

    return false;
}

bool UMingSageBrainConsoleStarter::ClearOutput()
{
    if (!IsConsoleRunning())
    {
        return false;
    }

    UMingSageBrainConsoleTerminal* Terminal = GetTerminal();
    if (Terminal)
    {
        Terminal->ClearOutput();
        return true;
    }

    return false;
}

bool UMingSageBrainConsoleStarter::ShutdownConsole()
{
    LogStarterEvent(TEXT("关闭控制台"), TEXT("开始关闭流程"));

    if (ConsoleLauncher)
    {
        ConsoleLauncher->ShutdownConsole();
        CleanupLauncher();
        LogStarterEvent(TEXT("关闭完成"), TEXT("控制台已关闭"));
        return true;
    }

    LogStarterEvent(TEXT("关闭失败"), TEXT("控制台实例不存在"));
    return false;
}

bool UMingSageBrainConsoleStarter::RestartConsole()
{
    LogStarterEvent(TEXT("重启控制台"), TEXT("开始重启流程"));

    if (ConsoleLauncher)
    {
        ConsoleLauncher->RestartConsole();
        return true;
    }

    return StartSageBrainConsole();
}

bool UMingSageBrainConsoleStarter::StartDeveloperMode()
{
    LogStarterEvent(TEXT("开发者模式"), TEXT("启动开发者模式"));

    if (!EnsureLauncherExists())
    {
        return false;
    }

    FConsoleLaunchConfig Config = CreateDeveloperConfig();
    return ConsoleLauncher->LaunchConsole(Config);
}

bool UMingSageBrainConsoleStarter::StartUserMode()
{
    LogStarterEvent(TEXT("用户模式"), TEXT("启动用户模式"));

    if (!EnsureLauncherExists())
    {
        return false;
    }

    FConsoleLaunchConfig Config = CreateUserConfig();
    return ConsoleLauncher->LaunchConsole(Config);
}

bool UMingSageBrainConsoleStarter::StartDemoMode()
{
    LogStarterEvent(TEXT("演示模式"), TEXT("启动演示模式"));

    if (!EnsureLauncherExists())
    {
        return false;
    }

    FConsoleLaunchConfig Config = CreateDemoConfig();
    return ConsoleLauncher->LaunchConsole(Config);
}

bool UMingSageBrainConsoleStarter::StartTestMode()
{
    LogStarterEvent(TEXT("测试模式"), TEXT("启动测试模式"));

    if (!EnsureLauncherExists())
    {
        return false;
    }

    FConsoleLaunchConfig Config = CreateTestConfig();
    return ConsoleLauncher->LaunchConsole(Config);
}

// 私有函数实现
bool UMingSageBrainConsoleStarter::InitializeLauncher()
{
    if (bIsInitialized && ConsoleLauncher)
    {
        return true;
    }

    ConsoleLauncher = NewObject<UMingSageBrainConsoleLauncher>();
    if (ConsoleLauncher)
    {
        bIsInitialized = true;
        LogStarterEvent(TEXT("启动器初始化"), TEXT("启动器实例已创建"));
        return true;
    }

    LogStarterEvent(TEXT("启动器初始化失败"), TEXT("无法创建启动器实例"));
    return false;
}

bool UMingSageBrainConsoleStarter::EnsureLauncherExists()
{
    return InitializeLauncher();
}

void UMingSageBrainConsoleStarter::CleanupLauncher()
{
    if (ConsoleLauncher)
    {
        ConsoleLauncher = nullptr;
    }
    bIsInitialized = false;
    LogStarterEvent(TEXT("清理启动器"), TEXT("启动器实例已清理"));
}

FConsoleLaunchConfig UMingSageBrainConsoleStarter::CreateDefaultConfig()
{
    FConsoleLaunchConfig Config;
    Config.bAutoStartSageBrain = true;
    Config.bShowTerminalOnStart = true;
    Config.bEnableAutoComplete = true;
    Config.bEnableSyntaxHighlighting = true;
    Config.DefaultTheme = TEXT("Default");
    Config.DefaultDisplayMode = EConsoleDisplayMode::Enhanced;
    Config.DefaultInteractionMode = EUserInteractionMode::CommandLine;
    return Config;
}

FConsoleLaunchConfig UMingSageBrainConsoleStarter::CreateDeveloperConfig()
{
    FConsoleLaunchConfig Config = CreateDefaultConfig();
    Config.DefaultTheme = TEXT("Dark");
    Config.DefaultDisplayMode = EConsoleDisplayMode::Full;
    Config.bEnableAutoComplete = true;
    Config.bEnableSyntaxHighlighting = true;
    Config.DefaultInteractionMode = EUserInteractionMode::CommandLine;
    return Config;
}

FConsoleLaunchConfig UMingSageBrainConsoleStarter::CreateUserConfig()
{
    FConsoleLaunchConfig Config = CreateDefaultConfig();
    Config.DefaultTheme = TEXT("Light");
    Config.DefaultDisplayMode = EConsoleDisplayMode::Enhanced;
    Config.bEnableAutoComplete = true;
    Config.bEnableSyntaxHighlighting = true;
    Config.DefaultInteractionMode = EUserInteractionMode::Chat;
    return Config;
}

FConsoleLaunchConfig UMingSageBrainConsoleStarter::CreateDemoConfig()
{
    FConsoleLaunchConfig Config = CreateDefaultConfig();
    Config.DefaultTheme = TEXT("Blue");
    Config.DefaultDisplayMode = EConsoleDisplayMode::Enhanced;
    Config.bEnableAutoComplete = true;
    Config.bEnableSyntaxHighlighting = true;
    Config.DefaultInteractionMode = EUserInteractionMode::Wizard;
    return Config;
}

FConsoleLaunchConfig UMingSageBrainConsoleStarter::CreateTestConfig()
{
    FConsoleLaunchConfig Config = CreateDefaultConfig();
    Config.DefaultTheme = TEXT("Green");
    Config.DefaultDisplayMode = EConsoleDisplayMode::Minimal;
    Config.bEnableAutoComplete = false;
    Config.bEnableSyntaxHighlighting = false;
    Config.DefaultInteractionMode = EUserInteractionMode::CommandLine;
    return Config;
}

bool UMingSageBrainConsoleStarter::ExecuteCommands(const TArray<FString>& Commands)
{
    if (!IsConsoleRunning())
    {
        return false;
    }

    for (const FString& Command : Commands)
    {
        if (!SendCommand(Command))
        {
            return false;
        }
        
        // 短暂延迟避免命令冲突
        FPlatformProcess::Sleep(0.1f);
    }

    return true;
}

void UMingSageBrainConsoleStarter::LogStarterEvent(const FString& Event, const FString& Details)
{
    UE_LOG(LogTemp, Log, TEXT("[ConsoleStarter] %s: %s"), *Event, *Details);
}
