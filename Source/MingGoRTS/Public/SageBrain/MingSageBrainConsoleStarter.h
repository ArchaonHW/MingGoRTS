#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSageBrainConsoleLauncher.h"
#include "MingSageBrainConsoleTerminal.h"
#include "MingSageBrainInteractiveConsole.h"
#include "MingSageBrainIntegratedAPI.h"
#include "MingSageBrainConsoleStarter.generated.h"

/**
 * 圣者大脑控制台启动器
 * 提供一键启动控制台与圣者大脑互动的功能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingSageBrainConsoleStarter : public UObject
{
    GENERATED_BODY()

public:
    UMingSageBrainConsoleStarter();

    // 一键启动功能
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Starter", meta = (DisplayName = "启动圣者大脑控制台"))
    static bool StartSageBrainConsole();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Starter", meta = (DisplayName = "快速启动"))
    static bool QuickStart();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Starter", meta = (DisplayName = "启动并显示"))
    static bool StartAndShow();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Starter", meta = (DisplayName = "启动聊天模式"))
    static bool StartChatMode();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Starter", meta = (DisplayName = "启动对话模式"))
    static bool StartDialogueMode();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Starter", meta = (DisplayName = "启动向导模式"))
    static bool StartWizardMode();

    // 高级启动选项
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Advanced", meta = (DisplayName = "自定义启动"))
    static bool CustomStart(EUserInteractionMode Mode, const FString& Theme = TEXT("Default"), bool bShowTerminal = true);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Advanced", meta = (DisplayName = "启动并执行命令"))
    static bool StartWithCommands(const TArray<FString>& Commands);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Advanced", meta = (DisplayName = "启动并加载脚本"))
    static bool StartWithScript(const FString& ScriptPath);

    // 状态检查
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Status", meta = (DisplayName = "是否已启动"))
    static bool IsConsoleRunning();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Status", meta = (DisplayName = "获取启动器实例"))
    static UMingSageBrainConsoleLauncher* GetConsoleLauncher();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Status", meta = (DisplayName = "获取终端"))
    static UMingSageBrainConsoleTerminal* GetTerminal();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Status", meta = (DisplayName = "获取交互式控制台"))
    static UMingSageBrainInteractiveConsole* GetInteractiveConsole();

    // 快捷命令
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Commands", meta = (DisplayName = "发送命令"))
    static bool SendCommand(const FString& Command);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Commands", meta = (DisplayName = "发送聊天消息"))
    static bool SendChatMessage(const FString& Message);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Commands", meta = (DisplayName = "执行帮助命令"))
    static bool ShowHelp();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Commands", meta = (DisplayName = "显示状态"))
    static bool ShowStatus();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Commands", meta = (DisplayName = "生成音乐"))
    static bool GenerateMusic(const FString& Description);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Commands", meta = (DisplayName = "生成场景"))
    static bool GenerateScene(const FString& Description);

    // 便捷功能
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Utilities", meta = (DisplayName = "显示终端"))
    static bool ShowTerminal();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Utilities", meta = (DisplayName = "隐藏终端"))
    static bool HideTerminal();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Utilities", meta = (DisplayName = "切换终端显示"))
    static bool ToggleTerminal();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Utilities", meta = (DisplayName = "切换主题"))
    static bool SwitchTheme(const FString& ThemeName);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Utilities", meta = (DisplayName = "清除输出"))
    static bool ClearOutput();

    // 关闭功能
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Shutdown", meta = (DisplayName = "关闭控制台"))
    static bool ShutdownConsole();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Shutdown", meta = (DisplayName = "重启控制台"))
    static bool RestartConsole();

    // 预设配置
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Presets", meta = (DisplayName = "开发者模式"))
    static bool StartDeveloperMode();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Presets", meta = (DisplayName = "用户模式"))
    static bool StartUserMode();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Presets", meta = (DisplayName = "演示模式"))
    static bool StartDemoMode();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Presets", meta = (DisplayName = "测试模式"))
    static bool StartTestMode();

private:
    // 静态实例管理
    static TObjectPtr<UMingSageBrainConsoleLauncher> ConsoleLauncher;
    static bool bIsInitialized;

    // 初始化函数
    static bool InitializeLauncher();
    static bool EnsureLauncherExists();
    static void CleanupLauncher();

    // 配置创建
    static FConsoleLaunchConfig CreateDefaultConfig();
    static FConsoleLaunchConfig CreateDeveloperConfig();
    static FConsoleLaunchConfig CreateUserConfig();
    static FConsoleLaunchConfig CreateDemoConfig();
    static FConsoleLaunchConfig CreateTestConfig();

    // 命令执行
    static bool ExecuteCommands(const TArray<FString>& Commands);
    static void LogStarterEvent(const FString& Event, const FString& Details);
};
