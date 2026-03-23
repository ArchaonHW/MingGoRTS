#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingSageBrainConsoleStarter.h"
#include "MingSageBrainConsoleTerminal.h"
#include "MingSageBrainInteractiveConsole.h"
#include "MingSageBrainIntegratedAPI.h"
#include "MingSupremeSageCommandSystem.h"
#include "MingSageBrainConsoleAutomation.generated.h"

/**
 * 圣者大脑控制台自动化系统
 * 提供自动化操作功能，支持批处理、定时任务、智能响应等
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingSageBrainConsoleAutomation : public UObject
{
    GENERATED_BODY()

public:
    UMingSageBrainConsoleAutomation();

    // 自动化启动
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Startup", meta = (DisplayName = "启动自动化系统"))
    static bool StartAutomationSystem();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Startup", meta = (DisplayName = "启动并初始化"))
    static bool StartAndInitialize();

    // 批处理命令
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Batch", meta = (DisplayName = "执行批处理命令"))
    static bool ExecuteBatchCommands(const TArray<FString>& Commands, bool bShowOutput = true);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Batch", meta = (DisplayName = "执行脚本文件"))
    static bool ExecuteScriptFile(const FString& ScriptPath);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Batch", meta = (DisplayName = "执行智能序列"))
    static bool ExecuteIntelligentSequence(const FString& SequenceName);

    // 定时任务
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Scheduler", meta = (DisplayName = "添加定时任务"))
    static bool AddScheduledTask(const FString& TaskName, const FString& Command, float DelaySeconds);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Scheduler", meta = (DisplayName = "添加重复任务"))
    static bool AddRepeatingTask(const FString& TaskName, const FString& Command, float IntervalSeconds, int32 RepeatCount = -1);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Scheduler", meta = (DisplayName = "移除定时任务"))
    static bool RemoveScheduledTask(const FString& TaskName);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Scheduler", meta = (DisplayName = "获取任务列表"))
    static TArray<FString> GetScheduledTasks();

    // 智能响应
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Intelligence", meta = (DisplayName = "启用智能响应"))
    static bool EnableIntelligentResponse();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Intelligence", meta = (DisplayName = "禁用智能响应"))
    static bool DisableIntelligentResponse();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Intelligence", meta = (DisplayName = "添加响应规则"))
    static bool AddResponseRule(const FString& Trigger, const FString& Response);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Intelligence", meta = (DisplayName = "移除响应规则"))
    static bool RemoveResponseRule(const FString& Trigger);

    // 预设自动化序列
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Presets", meta = (DisplayName = "运行音乐生成序列"))
    static bool RunMusicGenerationSequence(const FString& MusicDescription);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Presets", meta = (DisplayName = "运行场景生成序列"))
    static bool RunSceneGenerationSequence(const FString& SceneDescription);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Presets", meta = (DisplayName = "运行测试序列"))
    static bool RunTestSequence();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Presets", meta = (DisplayName = "运行演示序列"))
    static bool RunDemoSequence();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Presets", meta = (DisplayName = "运行开发者序列"))
    static bool RunDeveloperSequence();

    // 至聖者指揮系統預設序列
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Supreme Sage", meta = (DisplayName = "运行至聖者初始化序列"))
    static bool RunSupremeSageInitSequence();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Supreme Sage", meta = (DisplayName = "运行五行輪轉序列"))
    static bool RunFiveElementsSequence();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Supreme Sage", meta = (DisplayName = "运行六伐策略序列"))
    static bool RunSixConquestSequence();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Supreme Sage", meta = (DisplayName = "运行十二策序列"))
    static bool RunTwelveStrategiesSequence();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Supreme Sage", meta = (DisplayName = "运行防墮檢查序列"))
    static bool RunCorruptionPreventionSequence();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Supreme Sage", meta = (DisplayName = "运行至聖者完整工作流"))
    static bool RunSupremeSageFullWorkflow();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Supreme Sage", meta = (DisplayName = "智能至聖者決策"))
    static bool IntelligentSupremeSageDecision(const FString& Situation);

    // 监控和状态
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Monitoring", meta = (DisplayName = "获取自动化状态"))
    static FString GetAutomationStatus();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Monitoring", meta = (DisplayName = "获取执行历史"))
    static TArray<FString> GetExecutionHistory();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Monitoring", meta = (DisplayName = "清除执行历史"))
    static bool ClearExecutionHistory();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Monitoring", meta = (DisplayName = "导出执行报告"))
    static bool ExportExecutionReport(const FString& FilePath);

    // 高级功能
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Advanced", meta = (DisplayName = "创建自动化工作流"))
    static bool CreateWorkflow(const FString& WorkflowName, const TArray<FString>& Steps);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Advanced", meta = (DisplayName = "执行工作流"))
    static bool ExecuteWorkflow(const FString& WorkflowName);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Advanced", meta = (DisplayName = "设置条件触发"))
    static bool SetConditionalTrigger(const FString& Condition, const FString& Command);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Advanced", meta = (DisplayName = "启用事件监听"))
    static bool EnableEventListening();

    // 错误处理和恢复
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Recovery", meta = (DisplayName = "设置错误重试"))
    static bool SetErrorRetry(int32 MaxRetries, float RetryDelay);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Recovery", meta = (DisplayName = "启用自动恢复"))
    static bool EnableAutoRecovery();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Recovery", meta = (DisplayName = "手动恢复"))
    static bool ManualRecovery();

    // 配置管理
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Config", meta = (DisplayName = "保存配置"))
    static bool SaveConfiguration(const FString& ConfigPath);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Config", meta = (DisplayName = "加载配置"))
    static bool LoadConfiguration(const FString& ConfigPath);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Config", meta = (DisplayName = "重置配置"))
    static bool ResetConfiguration();

    // 关闭和清理
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Shutdown", meta = (DisplayName = "关闭自动化系统"))
    static bool ShutdownAutomationSystem();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Automation|Shutdown", meta = (DisplayName = "清理资源"))
    static bool CleanupResources();

private:
    // 静态实例管理
    static TObjectPtr<UMingSageBrainConsoleAutomation> AutomationInstance;
    static bool bIsInitialized;

    // 配置和数据
    FAutomationConfig Config;
    TArray<FScheduledTask> ScheduledTasks;
    TArray<FResponseRule> ResponseRules;
    TArray<FAutomationWorkflow> Workflows;
    TArray<FString> ExecutionHistory;

    // 定时器句柄
    FTimerHandle SchedulerTimer;
    FTimerHandle MonitorTimer;

    // 初始化函数
    static bool InitializeAutomation();
    static bool EnsureAutomationExists();
    static void CleanupAutomation();

    // 核心功能
    static bool ExecuteCommandWithRetry(const FString& Command);
    static void ProcessScheduledTasks();
    static void ProcessIntelligentResponse(const FString& Input);
    static void UpdateExecutionHistory(const FString& Entry);

    // 预设序列
    static TArray<FString> GetMusicGenerationSequence(const FString& Description);
    static TArray<FString> GetSceneGenerationSequence(const FString& Description);
    static TArray<FString> GetTestSequence();
    static TArray<FString> GetDemoSequence();
    static TArray<FString> GetDeveloperSequence();

    // 至聖者指揮系統預設序列
    static TArray<FString> GetSupremeSageInitSequence();
    static TArray<FString> GetFiveElementsSequence();
    static TArray<FString> GetSixConquestSequence();
    static TArray<FString> GetTwelveStrategiesSequence();
    static TArray<FString> GetCorruptionPreventionSequence();
    static TArray<FString> GetSupremeSageFullWorkflow();
    static TArray<FString> GetIntelligentSupremeSageDecision(const FString& Situation);

    // 工具函数
    static FString GetCurrentTimestamp();
    static bool ValidateCommand(const FString& Command);
    static void LogAutomationEvent(const FString& Event, const FString& Details);
    static bool SaveHistoryToFile(const FString& FilePath);

    // 定时器回调
    UFUNCTION()
    void OnSchedulerTick();

    UFUNCTION()
    void OnMonitorTick();
};
