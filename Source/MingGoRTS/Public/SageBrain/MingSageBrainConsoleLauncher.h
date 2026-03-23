#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSageBrainConsoleTerminal.h"
#include "MingSageBrainInteractiveConsole.h"
#include "MingSageBrainConsoleLauncher.generated.h"

// 启动配置
USTRUCT(BlueprintType)
struct FConsoleLaunchConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch Config")
    bool bAutoStartSageBrain = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch Config")
    bool bShowTerminalOnStart = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch Config")
    bool bEnableAutoComplete = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch Config")
    bool bEnableSyntaxHighlighting = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch Config")
    FString DefaultTheme = TEXT("Default");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch Config")
    EConsoleDisplayMode DefaultDisplayMode = EConsoleDisplayMode::Enhanced;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch Config")
    EUserInteractionMode DefaultInteractionMode = EUserInteractionMode::CommandLine;
};

// 启动状态
UENUM(BlueprintType)
enum class EConsoleLaunchState : uint8
{
    NotStarted      UMETA(DisplayName = "未启动"),
    Initializing    UMETA(DisplayName = "初始化中"),
    Running         UMETA(DisplayName = "运行中"),
    Error           UMETA(DisplayName = "错误"),
    Stopped         UMETA(DisplayName = "已停止")
};

// 启动事件
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnConsoleLaunched, bool, bSuccess);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnConsoleLaunchStateChanged, EConsoleLaunchState, NewState);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSageBrainConnected, bool, bConnected);

/**
 * 圣者大脑控制台启动器
 * 负责启动和管理控制台与圣者大脑的交互
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingSageBrainConsoleLauncher : public UObject
{
    GENERATED_BODY()

public:
    UMingSageBrainConsoleLauncher();

    // 启动和关闭
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher")
    bool LaunchConsole(const FConsoleLaunchConfig& LaunchConfig);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher")
    void ShutdownConsole();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher")
    void RestartConsole();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher")
    EConsoleLaunchState GetLaunchState() const { return LaunchState; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher")
    bool IsRunning() const { return LaunchState == EConsoleLaunchState::Running; }

    // 快速启动
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher|Quick")
    bool QuickLaunch();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher|Quick")
    bool LaunchWithDefaults();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher|Quick")
    bool LaunchInMode(EUserInteractionMode Mode);

    // 组件访问
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher|Components")
    UMingSageBrainConsoleTerminal* GetTerminal() const { return ConsoleTerminal; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher|Components")
    UMingSageBrainInteractiveConsole* GetInteractiveConsole() const { return InteractiveConsole; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher|Components")
    bool AreComponentsReady() const;

    // 配置管理
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher|Config")
    void SetLaunchConfig(const FConsoleLaunchConfig& NewConfig);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher|Config")
    FConsoleLaunchConfig GetLaunchConfig() const { return CurrentConfig; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher|Config")
    void SaveConfigToFile(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher|Config")
    void LoadConfigFromFile(const FString& FilePath);

    // 状态监控
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher|Status")
    FString GetStatusString() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher|Status")
    bool IsSageBrainConnected() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher|Status")
    FString GetLastErrorMessage() const { return LastErrorMessage; }

    // 自动化功能
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher|Automation")
    void ExecuteStartupCommands(const TArray<FString>& Commands);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher|Automation")
    void RunStartupScript(const FString& ScriptPath);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Launcher|Automation")
    void SetAutoExecuteCommands(bool bAutoExecute);

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Launcher|Events")
    FOnConsoleLaunched OnConsoleLaunched;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Launcher|Events")
    FOnConsoleLaunchStateChanged OnConsoleLaunchStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Launcher|Events")
    FOnSageBrainConnected OnSageBrainConnected;

protected:
    // 虚函数重载
    virtual void BeginDestroy() override;

private:
    // 核心组件
    UPROPERTY()
    TObjectPtr<UMingSageBrainConsoleTerminal> ConsoleTerminal;

    UPROPERTY()
    TObjectPtr<UMingSageBrainInteractiveConsole> InteractiveConsole;

    // 配置和状态
    FConsoleLaunchConfig CurrentConfig;
    EConsoleLaunchState LaunchState;
    FString LastErrorMessage;

    // 启动标志
    bool bIsInitialized;
    bool bAutoExecuteStartupCommands;
    float LaunchStartTime;

    // 初始化函数
    bool InitializeComponents();
    bool InitializeTerminal();
    bool InitializeInteractiveConsole();
    bool ConnectComponents();

    // 启动流程
    void SetLaunchState(EConsoleLaunchState NewState);
    void CompleteLaunch(bool bSuccess);
    void HandleLaunchError(const FString& Error);

    // 组件连接
    void BindTerminalEvents();
    void BindConsoleEvents();
    void SetupComponentCommunication();

    // 配置应用
    void ApplyLaunchConfig();
    void SetupTerminalTheme();
    void SetupInteractionMode();

    // 自动化
    void ExecuteDefaultStartupCommands();
    TArray<FString> GetDefaultStartupCommands();

    // 状态检查
    void ValidateComponentStates();
    void UpdateLaunchStatus();
    void LogLaunchEvent(const FString& Event, const FString& Details);

    // 辅助函数
    void CleanupResources();
    float GetLaunchDuration() const;
    bool ValidateConfig(const FConsoleLaunchConfig& Config);
};
