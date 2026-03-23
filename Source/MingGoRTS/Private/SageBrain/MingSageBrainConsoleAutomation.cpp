#include "MingSageBrainConsoleAutomation.h"
#include "MingSageBrainBridge.h"
#include "MingSupremeSageCommandSystem.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "TimerManager.h"

// 静态成员定义
TObjectPtr<UMingSageBrainConsoleAutomation> UMingSageBrainConsoleAutomation::AutomationInstance = nullptr;
bool UMingSageBrainConsoleAutomation::bIsInitialized = false;

UMingSageBrainConsoleAutomation::UMingSageBrainConsoleAutomation()
{
}

bool UMingSageBrainConsoleAutomation::StartAutomationSystem()
{
    LogAutomationEvent(TEXT("启动自动化系统"), TEXT("开始初始化自动化系统"));

    if (!EnsureAutomationExists())
    {
        LogAutomationEvent(TEXT("启动失败"), TEXT("无法创建自动化实例"));
        return false;
    }

    // 确保控制台已启动
    if (!UMingSageBrainConsoleStarter::IsConsoleRunning())
    {
        if (!UMingSageBrainConsoleStarter::StartSageBrainConsole())
        {
            LogAutomationEvent(TEXT("启动失败"), TEXT("控制台启动失败"));
            return false;
        }
    }

    // 启动定时器
    if (GEngine && GEngine->GetWorld())
    {
        GEngine->GetWorld()->GetTimerManager().SetTimer(
            AutomationInstance->SchedulerTimer,
            AutomationInstance, 
            &UMingSageBrainConsoleAutomation::OnSchedulerTick,
            1.0f, // 每秒检查一次
            true
        );

        GEngine->GetWorld()->GetTimerManager().SetTimer(
            AutomationInstance->MonitorTimer,
            AutomationInstance,
            &UMingSageBrainConsoleAutomation::OnMonitorTick,
            5.0f, // 每5秒监控一次
            true
        );
    }

    LogAutomationEvent(TEXT("启动成功"), TEXT("自动化系统已启动"));
    UpdateExecutionHistory(TEXT("自动化系统启动"));
    return true;
}

bool UMingSageBrainConsoleAutomation::StartAndInitialize()
{
    if (!StartAutomationSystem())
    {
        return false;
    }

    // 初始化默认配置
    AutomationInstance->Config.bIntelligentResponseEnabled = false;
    AutomationInstance->Config.bEventListeningEnabled = false;
    AutomationInstance->Config.bAutoRecoveryEnabled = true;
    AutomationInstance->Config.MaxRetries = 3;
    AutomationInstance->Config.RetryDelay = 1.0f;
    AutomationInstance->Config.CommandDelay = 0.5f;

    // 添加默认响应规则
    AddResponseRule(TEXT("帮助"), TEXT("help"));
    AddResponseRule(TEXT("状态"), TEXT("status"));
    AddResponseRule(TEXT("清除"), TEXT("clear"));

    LogAutomationEvent(TEXT("初始化完成"), TEXT("自动化系统已初始化"));
    return true;
}

bool UMingSageBrainConsoleAutomation::ExecuteBatchCommands(const TArray<FString>& Commands, bool bShowOutput)
{
    LogAutomationEvent(TEXT("执行批处理"), FString::Printf(TEXT("命令数量: %d"), Commands.Num()));

    if (!UMingSageBrainConsoleStarter::IsConsoleRunning())
    {
        LogAutomationEvent(TEXT("执行失败"), TEXT("控制台未运行"));
        return false;
    }

    int32 SuccessCount = 0;
    for (const FString& Command : Commands)
    {
        if (ExecuteCommandWithRetry(Command))
        {
            SuccessCount++;
        }
        
        // 命令间延迟
        FPlatformProcess::Sleep(AutomationInstance->Config.CommandDelay);
    }

    FString Result = FString::Printf(TEXT("批处理完成: %d/%d 成功"), SuccessCount, Commands.Num());
    UpdateExecutionHistory(Result);
    
    if (bShowOutput && AutomationInstance)
    {
        UMingSageBrainConsoleTerminal* Terminal = UMingSageBrainConsoleStarter::GetTerminal();
        if (Terminal)
        {
            Terminal->AddOutput(Result, EConsoleOutputLevel::Info, TEXT("Automation"));
        }
    }

    return SuccessCount == Commands.Num();
}

bool UMingSageBrainConsoleAutomation::ExecuteScriptFile(const FString& ScriptPath)
{
    LogAutomationEvent(TEXT("执行脚本文件"), FString::Printf(TEXT("文件路径: %s"), *ScriptPath));

    if (!FPaths::FileExists(ScriptPath))
    {
        LogAutomationEvent(TEXT("脚本执行失败"), TEXT("脚本文件不存在"));
        return false;
    }

    FString ScriptContent;
    if (!FFileHelper::LoadFileToString(ScriptContent, *ScriptPath))
    {
        LogAutomationEvent(TEXT("脚本执行失败"), TEXT("无法读取脚本文件"));
        return false;
    }

    TArray<FString> Commands;
    ScriptContent.ParseIntoArrayLines(Commands, true);

    return ExecuteBatchCommands(Commands);
}

bool UMingSageBrainConsoleAutomation::ExecuteIntelligentSequence(const FString& SequenceName)
{
    LogAutomationEvent(TEXT("执行智能序列"), FString::Printf(TEXT("序列名称: %s"), *SequenceName));

    TArray<FString> Sequence;

    if (SequenceName == TEXT("music_generation"))
    {
        Sequence = GetMusicGenerationSequence(TEXT("default"));
    }
    else if (SequenceName == TEXT("scene_generation"))
    {
        Sequence = GetSceneGenerationSequence(TEXT("default"));
    }
    else if (SequenceName == TEXT("test"))
    {
        Sequence = GetTestSequence();
    }
    else if (SequenceName == TEXT("demo"))
    {
        Sequence = GetDemoSequence();
    }
    else if (SequenceName == TEXT("developer"))
    {
        Sequence = GetDeveloperSequence();
    }
    else
    {
        LogAutomationEvent(TEXT("序列执行失败"), TEXT("未知序列名称"));
        return false;
    }

    return ExecuteBatchCommands(Sequence);
}

bool UMingSageBrainConsoleAutomation::AddScheduledTask(const FString& TaskName, const FString& Command, float DelaySeconds)
{
    if (!AutomationInstance)
    {
        return false;
    }

    FScheduledTask NewTask;
    NewTask.TaskName = TaskName;
    NewTask.Command = Command;
    NewTask.ExecuteTime = FPlatformTime::Seconds() + DelaySeconds;
    NewTask.Interval = 0.0f;
    NewTask.RepeatCount = 1;
    NewTask.CurrentRepeat = 0;
    NewTask.bActive = true;

    AutomationInstance->ScheduledTasks.Add(NewTask);

    LogAutomationEvent(TEXT("添加定时任务"), FString::Printf(TEXT("任务: %s, 延迟: %.1f秒"), *TaskName, DelaySeconds));
    return true;
}

bool UMingSageBrainConsoleAutomation::AddRepeatingTask(const FString& TaskName, const FString& Command, float IntervalSeconds, int32 RepeatCount)
{
    if (!AutomationInstance)
    {
        return false;
    }

    FScheduledTask NewTask;
    NewTask.TaskName = TaskName;
    NewTask.Command = Command;
    NewTask.ExecuteTime = FPlatformTime::Seconds() + IntervalSeconds;
    NewTask.Interval = IntervalSeconds;
    NewTask.RepeatCount = RepeatCount;
    NewTask.CurrentRepeat = 0;
    NewTask.bActive = true;

    AutomationInstance->ScheduledTasks.Add(NewTask);

    LogAutomationEvent(TEXT("添加重复任务"), FString::Printf(TEXT("任务: %s, 间隔: %.1f秒, 重复: %d"), *TaskName, IntervalSeconds, RepeatCount));
    return true;
}

bool UMingSageBrainConsoleAutomation::RemoveScheduledTask(const FString& TaskName)
{
    if (!AutomationInstance)
    {
        return false;
    }

    for (int32 i = 0; i < AutomationInstance->ScheduledTasks.Num(); i++)
    {
        if (AutomationInstance->ScheduledTasks[i].TaskName == TaskName)
        {
            AutomationInstance->ScheduledTasks.RemoveAt(i);
            LogAutomationEvent(TEXT("移除定时任务"), FString::Printf(TEXT("任务: %s"), *TaskName));
            return true;
        }
    }

    return false;
}

TArray<FString> UMingSageBrainConsoleAutomation::GetScheduledTasks()
{
    TArray<FString> TaskList;
    
    if (AutomationInstance)
    {
        for (const FScheduledTask& Task : AutomationInstance->ScheduledTasks)
        {
            if (Task.bActive)
            {
                TaskList.Add(Task.TaskName);
            }
        }
    }

    return TaskList;
}

bool UMingSageBrainConsoleAutomation::EnableIntelligentResponse()
{
    if (!AutomationInstance)
    {
        return false;
    }

    AutomationInstance->Config.bIntelligentResponseEnabled = true;
    LogAutomationEvent(TEXT("启用智能响应"), TEXT("智能响应系统已启用"));
    return true;
}

bool UMingSageBrainConsoleAutomation::DisableIntelligentResponse()
{
    if (!AutomationInstance)
    {
        return false;
    }

    AutomationInstance->Config.bIntelligentResponseEnabled = false;
    LogAutomationEvent(TEXT("禁用智能响应"), TEXT("智能响应系统已禁用"));
    return true;
}

bool UMingSageBrainConsoleAutomation::AddResponseRule(const FString& Trigger, const FString& Response)
{
    if (!AutomationInstance)
    {
        return false;
    }

    FResponseRule NewRule;
    NewRule.Trigger = Trigger;
    NewRule.Response = Response;
    NewRule.bEnabled = true;

    AutomationInstance->ResponseRules.Add(NewRule);
    LogAutomationEvent(TEXT("添加响应规则"), FString::Printf(TEXT("触发: %s -> 响应: %s"), *Trigger, *Response));
    return true;
}

bool UMingSageBrainConsoleAutomation::RemoveResponseRule(const FString& Trigger)
{
    if (!AutomationInstance)
    {
        return false;
    }

    for (int32 i = 0; i < AutomationInstance->ResponseRules.Num(); i++)
    {
        if (AutomationInstance->ResponseRules[i].Trigger == Trigger)
        {
            AutomationInstance->ResponseRules.RemoveAt(i);
            LogAutomationEvent(TEXT("移除响应规则"), FString::Printf(TEXT("触发: %s"), *Trigger));
            return true;
        }
    }

    return false;
}

bool UMingSageBrainConsoleAutomation::RunMusicGenerationSequence(const FString& MusicDescription)
{
    TArray<FString> Sequence = GetMusicGenerationSequence(MusicDescription);
    return ExecuteBatchCommands(Sequence);
}

bool UMingSageBrainConsoleAutomation::RunSceneGenerationSequence(const FString& SceneDescription)
{
    TArray<FString> Sequence = GetSceneGenerationSequence(SceneDescription);
    return ExecuteBatchCommands(Sequence);
}

bool UMingSageBrainConsoleAutomation::RunTestSequence()
{
    TArray<FString> Sequence = GetTestSequence();
    return ExecuteBatchCommands(Sequence);
}

bool UMingSageBrainConsoleAutomation::RunDemoSequence()
{
    TArray<FString> Sequence = GetDemoSequence();
    return ExecuteBatchCommands(Sequence);
}

bool UMingSageBrainConsoleAutomation::RunDeveloperSequence()
{
    TArray<FString> Sequence = GetDeveloperSequence();
    return ExecuteBatchCommands(Sequence);
}

FString UMingSageBrainConsoleAutomation::GetAutomationStatus()
{
    if (!AutomationInstance)
    {
        return TEXT("自动化系统未初始化");
    }

    FString Status = TEXT("=== 自动化系统状态 ===\n");
    Status += FString::Printf(TEXT("初始化状态: %s\n"), bIsInitialized ? TEXT("已初始化") : TEXT("未初始化"));
    Status += FString::Printf(TEXT("智能响应: %s\n"), AutomationInstance->Config.bIntelligentResponseEnabled ? TEXT("启用") : TEXT("禁用"));
    Status += FString::Printf(TEXT("事件监听: %s\n"), AutomationInstance->Config.bEventListeningEnabled ? TEXT("启用") : TEXT("禁用"));
    Status += FString::Printf(TEXT("自动恢复: %s\n"), AutomationInstance->Config.bAutoRecoveryEnabled ? TEXT("启用") : TEXT("禁用"));
    Status += FString::Printf(TEXT("最大重试: %d\n"), AutomationInstance->Config.MaxRetries);
    Status += FString::Printf(TEXT("重试延迟: %.1f秒\n"), AutomationInstance->Config.RetryDelay);
    Status += FString::Printf(TEXT("命令延迟: %.1f秒\n"), AutomationInstance->Config.CommandDelay);
    Status += FString::Printf(TEXT("定时任务: %d\n"), AutomationInstance->ScheduledTasks.Num());
    Status += FString::Printf(TEXT("响应规则: %d\n"), AutomationInstance->ResponseRules.Num());
    Status += FString::Printf(TEXT("工作流: %d\n"), AutomationInstance->Workflows.Num());
    Status += FString::Printf(TEXT("执行历史: %d\n"), AutomationInstance->ExecutionHistory.Num());

    return Status;
}

TArray<FString> UMingSageBrainConsoleAutomation::GetExecutionHistory()
{
    if (AutomationInstance)
    {
        return AutomationInstance->ExecutionHistory;
    }
    return TArray<FString>();
}

bool UMingSageBrainConsoleAutomation::ClearExecutionHistory()
{
    if (!AutomationInstance)
    {
        return false;
    }

    AutomationInstance->ExecutionHistory.Empty();
    LogAutomationEvent(TEXT("清除执行历史"), TEXT("执行历史已清除"));
    return true;
}

bool UMingSageBrainConsoleAutomation::ExportExecutionReport(const FString& FilePath)
{
    if (!AutomationInstance)
    {
        return false;
    }

    FString Report = GetAutomationStatus();
    Report += TEXT("\n=== 执行历史 ===\n");
    
    for (const FString& Entry : AutomationInstance->ExecutionHistory)
    {
        Report += Entry + TEXT("\n");
    }

    return FFileHelper::SaveStringToFile(Report, *FilePath);
}

bool UMingSageBrainConsoleAutomation::CreateWorkflow(const FString& WorkflowName, const TArray<FString>& Steps)
{
    if (!AutomationInstance)
    {
        return false;
    }

    FAutomationWorkflow NewWorkflow;
    NewWorkflow.WorkflowName = WorkflowName;
    NewWorkflow.Steps = Steps;
    NewWorkflow.CurrentStep = 0;
    NewWorkflow.bRunning = false;

    AutomationInstance->Workflows.Add(NewWorkflow);
    LogAutomationEvent(TEXT("创建工作流"), FString::Printf(TEXT("工作流: %s, 步骤数: %d"), *WorkflowName, Steps.Num()));
    return true;
}

bool UMingSageBrainConsoleAutomation::ExecuteWorkflow(const FString& WorkflowName)
{
    if (!AutomationInstance)
    {
        return false;
    }

    for (FAutomationWorkflow& Workflow : AutomationInstance->Workflows)
    {
        if (Workflow.WorkflowName == WorkflowName)
        {
            Workflow.bRunning = true;
            Workflow.CurrentStep = 0;

            // 执行工作流步骤
            for (const FString& Step : Workflow.Steps)
            {
                if (!ExecuteCommandWithRetry(Step))
                {
                    Workflow.bRunning = false;
                    return false;
                }
                FPlatformProcess::Sleep(AutomationInstance->Config.CommandDelay);
            }

            Workflow.bRunning = false;
            LogAutomationEvent(TEXT("工作流执行完成"), FString::Printf(TEXT("工作流: %s"), *WorkflowName));
            return true;
        }
    }

    return false;
}

bool UMingSageBrainConsoleAutomation::SetConditionalTrigger(const FString& Condition, const FString& Command)
{
    // 简化实现，将条件作为响应规则
    return AddResponseRule(Condition, Command);
}

bool UMingSageBrainConsoleAutomation::EnableEventListening()
{
    if (!AutomationInstance)
    {
        return false;
    }

    AutomationInstance->Config.bEventListeningEnabled = true;
    LogAutomationEvent(TEXT("启用事件监听"), TEXT("事件监听系统已启用"));
    return true;
}

bool UMingSageBrainConsoleAutomation::SetErrorRetry(int32 MaxRetries, float RetryDelay)
{
    if (!AutomationInstance)
    {
        return false;
    }

    AutomationInstance->Config.MaxRetries = MaxRetries;
    AutomationInstance->Config.RetryDelay = RetryDelay;
    
    LogAutomationEvent(TEXT("设置错误重试"), FString::Printf(TEXT("最大重试: %d, 延迟: %.1f秒"), MaxRetries, RetryDelay));
    return true;
}

bool UMingSageBrainConsoleAutomation::EnableAutoRecovery()
{
    if (!AutomationInstance)
    {
        return false;
    }

    AutomationInstance->Config.bAutoRecoveryEnabled = true;
    LogAutomationEvent(TEXT("启用自动恢复"), TEXT("自动恢复系统已启用"));
    return true;
}

bool UMingSageBrainConsoleAutomation::ManualRecovery()
{
    LogAutomationEvent(TEXT("手动恢复"), TEXT("开始手动恢复流程"));

    // 检查控制台状态
    if (!UMingSageBrainConsoleStarter::IsConsoleRunning())
    {
        if (!UMingSageBrainConsoleStarter::StartSageBrainConsole())
        {
            LogAutomationEvent(TEXT("恢复失败"), TEXT("控制台重启失败"));
            return false;
        }
    }

    // 清理无效任务
    if (AutomationInstance)
    {
        for (int32 i = AutomationInstance->ScheduledTasks.Num() - 1; i >= 0; i--)
        {
            if (!AutomationInstance->ScheduledTasks[i].bActive)
            {
                AutomationInstance->ScheduledTasks.RemoveAt(i);
            }
        }
    }

    LogAutomationEvent(TEXT("恢复完成"), TEXT("手动恢复流程完成"));
    return true;
}

bool UMingSageBrainConsoleAutomation::SaveConfiguration(const FString& ConfigPath)
{
    if (!AutomationInstance)
    {
        return false;
    }

    FString JsonContent = TEXT("{\n");
    JsonContent += FString::Printf(TEXT("  \"bIntelligentResponseEnabled\": %s,\n"), 
        AutomationInstance->Config.bIntelligentResponseEnabled ? TEXT("true") : TEXT("false"));
    JsonContent += FString::Printf(TEXT("  \"bEventListeningEnabled\": %s,\n"), 
        AutomationInstance->Config.bEventListeningEnabled ? TEXT("true") : TEXT("false"));
    JsonContent += FString::Printf(TEXT("  \"bAutoRecoveryEnabled\": %s,\n"), 
        AutomationInstance->Config.bAutoRecoveryEnabled ? TEXT("true") : TEXT("false"));
    JsonContent += FString::Printf(TEXT("  \"MaxRetries\": %d,\n"), AutomationInstance->Config.MaxRetries);
    JsonContent += FString::Printf(TEXT("  \"RetryDelay\": %.1f,\n"), AutomationInstance->Config.RetryDelay);
    JsonContent += FString::Printf(TEXT("  \"CommandDelay\": %.1f\n"), AutomationInstance->Config.CommandDelay);
    JsonContent += TEXT("}");

    return FFileHelper::SaveStringToFile(JsonContent, *ConfigPath);
}

bool UMingSageBrainConsoleAutomation::LoadConfiguration(const FString& ConfigPath)
{
    FString JsonContent;
    if (!FFileHelper::LoadFileToString(JsonContent, *ConfigPath))
    {
        return false;
    }

    if (!AutomationInstance)
    {
        return false;
    }

    // 简化的JSON解析
    if (JsonContent.Contains(TEXT("\"bIntelligentResponseEnabled\": true")))
    {
        AutomationInstance->Config.bIntelligentResponseEnabled = true;
    }
    
    if (JsonContent.Contains(TEXT("\"bEventListeningEnabled\": true")))
    {
        AutomationInstance->Config.bEventListeningEnabled = true;
    }
    
    if (JsonContent.Contains(TEXT("\"bAutoRecoveryEnabled\": true")))
    {
        AutomationInstance->Config.bAutoRecoveryEnabled = true;
    }

    LogAutomationEvent(TEXT("配置加载"), FString::Printf(TEXT("从 %s 加载配置"), *ConfigPath));
    return true;
}

bool UMingSageBrainConsoleAutomation::ResetConfiguration()
{
    if (!AutomationInstance)
    {
        return false;
    }

    AutomationInstance->Config = FAutomationConfig();
    LogAutomationEvent(TEXT("配置重置"), TEXT("配置已重置为默认值"));
    return true;
}

bool UMingSageBrainConsoleAutomation::ShutdownAutomationSystem()
{
    LogAutomationEvent(TEXT("关闭自动化系统"), TEXT("开始关闭流程"));

    if (AutomationInstance && GEngine && GEngine->GetWorld())
    {
        // 清理定时器
        GEngine->GetWorld()->GetTimerManager().ClearTimer(AutomationInstance->SchedulerTimer);
        GEngine->GetWorld()->GetTimerManager().ClearTimer(AutomationInstance->MonitorTimer);
    }

    CleanupAutomation();
    LogAutomationEvent(TEXT("关闭完成"), TEXT("自动化系统已关闭"));
    return true;
}

bool UMingSageBrainConsoleAutomation::CleanupResources()
{
    if (AutomationInstance)
    {
        AutomationInstance->ScheduledTasks.Empty();
        AutomationInstance->ResponseRules.Empty();
        AutomationInstance->Workflows.Empty();
        AutomationInstance->ExecutionHistory.Empty();
    }

    return true;
}

// 私有函数实现
bool UMingSageBrainConsoleAutomation::InitializeAutomation()
{
    if (bIsInitialized && AutomationInstance)
    {
        return true;
    }

    AutomationInstance = NewObject<UMingSageBrainConsoleAutomation>();
    if (AutomationInstance)
    {
        bIsInitialized = true;
        LogAutomationEvent(TEXT("自动化初始化"), TEXT("自动化实例已创建"));
        return true;
    }

    LogAutomationEvent(TEXT("自动化初始化失败"), TEXT("无法创建自动化实例"));
    return false;
}

bool UMingSageBrainConsoleAutomation::EnsureAutomationExists()
{
    return InitializeAutomation();
}

void UMingSageBrainConsoleAutomation::CleanupAutomation()
{
    if (AutomationInstance)
    {
        AutomationInstance = nullptr;
    }
    bIsInitialized = false;
    LogAutomationEvent(TEXT("清理自动化"), TEXT("自动化实例已清理"));
}

bool UMingSageBrainConsoleAutomation::ExecuteCommandWithRetry(const FString& Command)
{
    if (!ValidateCommand(Command))
    {
        return false;
    }

    int32 RetryCount = 0;
    int32 MaxRetries = AutomationInstance ? AutomationInstance->Config.MaxRetries : 3;
    float RetryDelay = AutomationInstance ? AutomationInstance->Config.RetryDelay : 1.0f;

    while (RetryCount <= MaxRetries)
    {
        if (UMingSageBrainConsoleStarter::SendCommand(Command))
        {
            UpdateExecutionHistory(FString::Printf(TEXT("执行成功: %s"), *Command));
            return true;
        }

        RetryCount++;
        if (RetryCount <= MaxRetries)
        {
            FPlatformProcess::Sleep(RetryDelay);
        }
    }

    UpdateExecutionHistory(FString::Printf(TEXT("执行失败: %s (重试 %d 次)"), *Command, MaxRetries));
    return false;
}

void UMingSageBrainConsoleAutomation::ProcessScheduledTasks()
{
    if (!AutomationInstance)
    {
        return;
    }

    float CurrentTime = FPlatformTime::Seconds();

    for (int32 i = AutomationInstance->ScheduledTasks.Num() - 1; i >= 0; i--)
    {
        FScheduledTask& Task = AutomationInstance->ScheduledTasks[i];
        
        if (!Task.bActive)
        {
            continue;
        }

        if (CurrentTime >= Task.ExecuteTime)
        {
            ExecuteCommandWithRetry(Task.Command);
            
            Task.CurrentRepeat++;
            
            if (Task.Interval > 0.0f && (Task.RepeatCount == -1 || Task.CurrentRepeat < Task.RepeatCount))
            {
                Task.ExecuteTime = CurrentTime + Task.Interval;
            }
            else
            {
                AutomationInstance->ScheduledTasks.RemoveAt(i);
            }
        }
    }
}

void UMingSageBrainConsoleAutomation::ProcessIntelligentResponse(const FString& Input)
{
    if (!AutomationInstance || !AutomationInstance->Config.bIntelligentResponseEnabled)
    {
        return;
    }

    for (const FResponseRule& Rule : AutomationInstance->ResponseRules)
    {
        if (Rule.bEnabled && Input.Contains(Rule.Trigger))
        {
            ExecuteCommandWithRetry(Rule.Response);
            break;
        }
    }
}

void UMingSageBrainConsoleAutomation::UpdateExecutionHistory(const FString& Entry)
{
    if (!AutomationInstance)
    {
        return;
    }

    FString TimestampedEntry = FString::Printf(TEXT("[%s] %s"), *GetCurrentTimestamp(), *Entry);
    AutomationInstance->ExecutionHistory.Add(TimestampedEntry);

    // 限制历史记录大小
    if (AutomationInstance->ExecutionHistory.Num() > 1000)
    {
        AutomationInstance->ExecutionHistory.RemoveAt(0, AutomationInstance->ExecutionHistory.Num() - 1000);
    }
}

TArray<FString> UMingSageBrainConsoleAutomation::GetMusicGenerationSequence(const FString& Description)
{
    TArray<FString> Sequence;
    Sequence.Add(TEXT("status"));
    Sequence.Add(FString::Printf(TEXT("generate music %s"), *Description));
    Sequence.Add(TEXT("status"));
    Sequence.Add(TEXT("play last"));
    return Sequence;
}

TArray<FString> UMingSageBrainConsoleAutomation::GetSceneGenerationSequence(const FString& Description)
{
    TArray<FString> Sequence;
    Sequence.Add(TEXT("status"));
    Sequence.Add(FString::Printf(TEXT("generate scene %s"), *Description));
    Sequence.Add(TEXT("status"));
    Sequence.Add(TEXT("preview last"));
    return Sequence;
}

TArray<FString> UMingSageBrainConsoleAutomation::GetTestSequence()
{
    TArray<FString> Sequence;
    Sequence.Add(TEXT("help"));
    Sequence.Add(TEXT("status"));
    Sequence.Add(TEXT("test api"));
    Sequence.Add(TEXT("test connection"));
    Sequence.Add(TEXT("clear"));
    return Sequence;
}

TArray<FString> UMingSageBrainConsoleAutomation::GetDemoSequence()
{
    TArray<FString> Sequence;
    Sequence.Add(TEXT("start dialogue music_creation"));
    Sequence.Add(TEXT("generate music 史诗明朝音乐"));
    Sequence.Add(TEXT("generate scene 故宫"));
    Sequence.Add(TEXT("show achievements"));
    Sequence.Add(TEXT("end dialogue"));
    return Sequence;
}

TArray<FString> UMingSageBrainConsoleAutomation::GetDeveloperSequence()
{
    TArray<FString> Sequence;
    Sequence.Add(TEXT("mode command"));
    Sequence.Add(TEXT("status"));
    Sequence.Add(TEXT("test all"));
    Sequence.Add(TEXT("debug info"));
    Sequence.Add(TEXT("performance stats"));
    Sequence.Add(TEXT("export logs"));
    return Sequence;
}

FString UMingSageBrainConsoleAutomation::GetCurrentTimestamp()
{
    return FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S"));
}

bool UMingSageBrainConsoleAutomation::ValidateCommand(const FString& Command)
{
    return !Command.IsEmpty() && Command.Len() > 0;
}

void UMingSageBrainConsoleAutomation::LogAutomationEvent(const FString& Event, const FString& Details)
{
    UE_LOG(LogTemp, Log, TEXT("[Automation] %s: %s"), *Event, *Details);
}

bool UMingSageBrainConsoleAutomation::SaveHistoryToFile(const FString& FilePath)
{
    if (!AutomationInstance)
    {
        return false;
    }

    FString Content;
    for (const FString& Entry : AutomationInstance->ExecutionHistory)
    {
        Content += Entry + TEXT("\n");
    }

    return FFileHelper::SaveStringToFile(Content, *FilePath);
}

// 定时器回调
void UMingSageBrainConsoleAutomation::OnSchedulerTick()
{
    ProcessScheduledTasks();
}

void UMingSageBrainConsoleAutomation::OnMonitorTick()
{
    // 监控系统状态
    if (!UMingSageBrainConsoleStarter::IsConsoleRunning() && Config.bAutoRecoveryEnabled)
    {
        LogAutomationEvent(TEXT("检测到控制台异常"), TEXT("开始自动恢复"));
        ManualRecovery();
    }
}

// ==================== 至聖者指揮系統預設序列實現 ====================

bool UMingSageBrainConsoleAutomation::RunSupremeSageInitSequence()
{
    UE_LOG(LogTemp, Log, TEXT("=== 運行至聖者初始化序列 ==="));
    
    TArray<FString> Sequence = GetSupremeSageInitSequence();
    return ExecuteBatchCommands(Sequence, true);
}

bool UMingSageBrainConsoleAutomation::RunFiveElementsSequence()
{
    UE_LOG(LogTemp, Log, TEXT("=== 運行五行輪轉序列 ==="));
    
    TArray<FString> Sequence = GetFiveElementsSequence();
    return ExecuteBatchCommands(Sequence, true);
}

bool UMingSageBrainConsoleAutomation::RunSixConquestSequence()
{
    UE_LOG(LogTemp, Log, TEXT("=== 運行六伐策略序列 ==="));
    
    TArray<FString> Sequence = GetSixConquestSequence();
    return ExecuteBatchCommands(Sequence, true);
}

bool UMingSageBrainConsoleAutomation::RunTwelveStrategiesSequence()
{
    UE_LOG(LogTemp, Log, TEXT("=== 運行十二策序列 ==="));
    
    TArray<FString> Sequence = GetTwelveStrategiesSequence();
    return ExecuteBatchCommands(Sequence, true);
}

bool UMingSageBrainConsoleAutomation::RunCorruptionPreventionSequence()
{
    UE_LOG(LogTemp, Log, TEXT("=== 運行防墮檢查序列 ==="));
    
    TArray<FString> Sequence = GetCorruptionPreventionSequence();
    return ExecuteBatchCommands(Sequence, true);
}

bool UMingSageBrainConsoleAutomation::RunSupremeSageFullWorkflow()
{
    UE_LOG(LogTemp, Log, TEXT("=== 運行至聖者完整工作流 ==="));
    
    TArray<FString> Sequence = GetSupremeSageFullWorkflow();
    return ExecuteBatchCommands(Sequence, true);
}

bool UMingSageBrainConsoleAutomation::IntelligentSupremeSageDecision(const FString& Situation)
{
    UE_LOG(LogTemp, Log, TEXT("=== 智能至聖者決策 ==="));
    UE_LOG(LogTemp, Log, TEXT("情境: %s"), *Situation);
    
    TArray<FString> Sequence = GetIntelligentSupremeSageDecision(Situation);
    return ExecuteBatchCommands(Sequence, true);
}

TArray<FString> UMingSageBrainConsoleAutomation::GetSupremeSageInitSequence()
{
    TArray<FString> Sequence;
    
    // 初始化至聖者系統
    Sequence.Add(TEXT("bridge initialize_supreme_sage_system"));
    Sequence.Add(TEXT("bridge get_sage_command_recommendation"));
    Sequence.Add(TEXT("bridge perform_corruption_check"));
    Sequence.Add(TEXT("log_automation_event 至聖者系統初始化完成"));
    
    return Sequence;
}

TArray<FString> UMingSageBrainConsoleAutomation::GetFiveElementsSequence()
{
    TArray<FString> Sequence;
    
    // 五行輪轉演示
    Sequence.Add(TEXT("bridge enter_five_element_phase Wood \"立名階段\""));
    Sequence.Add(TEXT("wait 2"));
    Sequence.Add(TEXT("bridge enter_five_element_phase Fire \"造勢階段\""));
    Sequence.Add(TEXT("wait 2"));
    Sequence.Add(TEXT("bridge enter_five_element_phase Earth \"收權階段\""));
    Sequence.Add(TEXT("wait 2"));
    Sequence.Add(TEXT("bridge enter_five_element_phase Metal \"裁斷階段\""));
    Sequence.Add(TEXT("wait 2"));
    Sequence.Add(TEXT("bridge enter_five_element_phase Water \"留白階段\""));
    
    return Sequence;
}

TArray<FString> UMingSageBrainConsoleAutomation::GetSixConquestSequence()
{
    TArray<FString> Sequence;
    
    // 六伐策略演示
    Sequence.Add(TEXT("bridge execute_six_conquest_strategy ConquerMind Righteous \"目標A\""));
    Sequence.Add(TEXT("wait 1"));
    Sequence.Add(TEXT("bridge execute_six_conquest_strategy ConquerSpirit Righteous \"目標B\""));
    Sequence.Add(TEXT("wait 1"));
    Sequence.Add(TEXT("bridge execute_six_conquest_strategy ConquerMomentum Reverse \"目標C\""));
    Sequence.Add(TEXT("wait 1"));
    Sequence.Add(TEXT("bridge execute_six_conquest_strategy ConquerLife Righteous \"目標D\""));
    Sequence.Add(TEXT("wait 1"));
    Sequence.Add(TEXT("bridge execute_six_conquest_strategy ConquerShadow Reverse \"目標E\""));
    Sequence.Add(TEXT("wait 1"));
    Sequence.Add(TEXT("bridge execute_six_conquest_strategy ConquerNothing Righteous \"目標F\""));
    
    return Sequence;
}

TArray<FString> UMingSageBrainConsoleAutomation::GetTwelveStrategiesSequence()
{
    TArray<FString> Sequence;
    
    // 十二策演示
    Sequence.Add(TEXT("bridge execute_twelve_strategy EstablishNation \"立國策略\""));
    Sequence.Add(TEXT("wait 1"));
    Sequence.Add(TEXT("bridge execute_twelve_strategy EstablishSystem \"立制策略\""));
    Sequence.Add(TEXT("wait 1"));
    Sequence.Add(TEXT("bridge execute_twelve_strategy EstablishPeople \"立人策略\""));
    Sequence.Add(TEXT("wait 1"));
    Sequence.Add(TEXT("bridge execute_twelve_strategy BreakSituation \"破局策略\""));
    Sequence.Add(TEXT("wait 1"));
    Sequence.Add(TEXT("bridge execute_twelve_strategy BreakStructure \"破結構策略\""));
    Sequence.Add(TEXT("wait 1"));
    Sequence.Add(TEXT("bridge execute_twelve_strategy BreakNotPerson \"不破人策略\""));
    
    return Sequence;
}

TArray<FString> UMingSageBrainConsoleAutomation::GetCorruptionPreventionSequence()
{
    TArray<FString> Sequence;
    
    // 防墮檢查序列
    Sequence.Add(TEXT("bridge perform_corruption_check"));
    Sequence.Add(TEXT("bridge generate_sage_self_audit_report"));
    Sequence.Add(TEXT("wait 1"));
    Sequence.Add(TEXT("bridge perform_corruption_check"));
    Sequence.Add(TEXT("log_automation_event 防墮檢查序列完成"));
    
    return Sequence;
}

TArray<FString> UMingSageBrainConsoleAutomation::GetSupremeSageFullWorkflow()
{
    TArray<FString> Sequence;
    
    // 完整工作流
    Sequence.Add(TEXT("=== 至聖者指揮系統完整工作流 ==="));
    Sequence.Add(TEXT("bridge initialize_supreme_sage_system"));
    Sequence.Add(TEXT("wait 1"));
    
    // 五行輪轉
    Sequence.Add(TEXT("=== 開始五行輪轉 ==="));
    Sequence.Add(TEXT("bridge enter_five_element_phase Wood \"立名階段\""));
    Sequence.Add(TEXT("wait 2"));
    Sequence.Add(TEXT("bridge enter_five_element_phase Fire \"造勢階段\""));
    Sequence.Add(TEXT("wait 2"));
    
    // 策略執行
    Sequence.Add(TEXT("=== 執行策略 ==="));
    Sequence.Add(TEXT("bridge execute_twelve_strategy EstablishNation \"建立正當性\""));
    Sequence.Add(TEXT("wait 1"));
    Sequence.Add(TEXT("bridge execute_six_conquest_strategy ConquerMind Righteous \"奪取意志\""));
    Sequence.Add(TEXT("wait 1"));
    
    // 防墮檢查
    Sequence.Add(TEXT("=== 防墮檢查 ==="));
    Sequence.Add(TEXT("bridge perform_corruption_check"));
    Sequence.Add(TEXT("wait 1"));
    Sequence.Add(TEXT("bridge generate_sage_self_audit_report"));
    
    return Sequence;
}

TArray<FString> UMingSageBrainConsoleAutomation::GetIntelligentSupremeSageDecision(const FString& Situation)
{
    TArray<FString> Sequence;
    
    // 智能決策序列
    Sequence.Add(FString::Printf(TEXT("=== 智能至聖者決策: %s ==="), *Situation));
    Sequence.Add(TEXT("bridge get_sage_command_recommendation"));
    Sequence.Add(TEXT("wait 1"));
    
    // 根據情境選擇策略
    if (Situation.Contains(TEXT("混亂")) || Situation.Contains(TEXT("緊急")))
    {
        Sequence.Add(TEXT("=== 緊急情況：使用逆策 ==="));
        Sequence.Add(TEXT("bridge execute_twelve_strategy BreakSituation \"破局應對\""));
        Sequence.Add(TEXT("bridge execute_six_conquest_strategy ConquerMomentum Reverse \"逆轉時勢\""));
    }
    else if (Situation.Contains(TEXT("穩定")) || Situation.Contains(TEXT("正常")))
    {
        Sequence.Add(TEXT("=== 穩定情況：使用正策 ==="));
        Sequence.Add(TEXT("bridge execute_twelve_strategy EstablishNation \"鞏固正當性\""));
        Sequence.Add(TEXT("bridge execute_six_conquest_strategy ConquerMind Righteous "奪取意志\""));
    }
    else
    {
        Sequence.Add(TEXT("=== 一般情況：平衡策略 ==="));
        Sequence.Add(TEXT("bridge get_sage_command_recommendation"));
        Sequence.Add(TEXT("bridge deploy_force Righteous \"部署正兵\""));
    }
    
    // 執行防墮檢查
    Sequence.Add(TEXT("bridge perform_corruption_check"));
    
    return Sequence;
}
