# 圣者大脑控制台自动化系统使用指南

## 概述

`MingSageBrainConsoleAutomation` 提供了强大的自动化操作功能，支持批处理命令、定时任务、智能响应、工作流等高级功能，让您能够自动化与圣者大脑的交互。

## 快速开始

### 1. 启动自动化系统
```cpp
// 基础启动
bool bSuccess = UMingSageBrainConsoleAutomation::StartAutomationSystem();

// 启动并初始化（推荐）
bool bSuccess = UMingSageBrainConsoleAutomation::StartAndInitialize();
```

### 2. 执行批处理命令
```cpp
// 执行多个命令
TArray<FString> Commands = {
    TEXT("help"),
    TEXT("status"),
    TEXT("generate music 史诗战斗音乐"),
    TEXT("generate scene 明朝宫殿")
};
bool bSuccess = UMingSageBrainConsoleAutomation::ExecuteBatchCommands(Commands);
```

## 核心功能

### 🔄 批处理操作

#### 基本批处理
```cpp
// 执行命令数组
TArray<FString> Commands = {
    TEXT("mode chat"),
    TEXT("你好，圣者大脑"),
    TEXT("生成一些明朝音乐"),
    TEXT("展示成就")
};
UMingSageBrainConsoleAutomation::ExecuteBatchCommands(Commands, true); // 显示输出

// 执行脚本文件
UMingSageBrainConsoleAutomation::ExecuteScriptFile(TEXT("MyScript.txt"));
```

#### 预设序列
```cpp
// 音乐生成序列
UMingSageBrainConsoleAutomation::RunMusicGenerationSequence(TEXT("史诗明朝音乐"));

// 场景生成序列
UMingSageBrainConsoleAutomation::RunSceneGenerationSequence(TEXT("故宫"));

// 测试序列
UMingSageBrainConsoleAutomation::RunTestSequence();

// 演示序列
UMingSageBrainConsoleAutomation::RunDemoSequence();

// 开发者序列
UMingSageBrainConsoleAutomation::RunDeveloperSequence();
```

### ⏰ 定时任务

#### 单次定时任务
```cpp
// 5秒后执行命令
UMingSageBrainConsoleAutomation::AddScheduledTask(
    TEXT("DelayedHello"), 
    TEXT("echo 你好，圣者大脑"), 
    5.0f
);

// 10秒后生成音乐
UMingSageBrainConsoleAutomation::AddScheduledTask(
    TEXT("DelayedMusic"), 
    TEXT("generate music 延迟生成的音乐"), 
    10.0f
);
```

#### 重复任务
```cpp
// 每30秒检查一次状态，重复5次
UMingSageBrainConsoleAutomation::AddRepeatingTask(
    TEXT("StatusCheck"), 
    TEXT("status"), 
    30.0f, 
    5
);

// 每分钟生成一次音乐，无限重复
UMingSageBrainConsoleAutomation::AddRepeatingTask(
    TEXT("PeriodicMusic"), 
    TEXT("generate music 定时音乐"), 
    60.0f, 
    -1  // -1 表示无限重复
);
```

#### 任务管理
```cpp
// 获取所有活动任务
TArray<FString> ActiveTasks = UMingSageBrainConsoleAutomation::GetScheduledTasks();

// 移除特定任务
UMingSageBrainConsoleAutomation::RemoveScheduledTask(TEXT("StatusCheck"));
```

### 🧠 智能响应

#### 启用智能响应
```cpp
// 启用智能响应系统
UMingSageBrainConsoleAutomation::EnableIntelligentResponse();

// 添加响应规则
UMingSageBrainConsoleAutomation::AddResponseRule(TEXT("帮助"), TEXT("help"));
UMingSageBrainConsoleAutomation::AddResponseRule(TEXT("状态"), TEXT("status"));
UMingSageBrainConsoleAutomation::AddResponseRule(TEXT("音乐"), TEXT("generate music 默认音乐"));
UMingSageBrainConsoleAutomation::AddResponseRule(TEXT("清除"), TEXT("clear"));

// 移除响应规则
UMingSageBrainConsoleAutomation::RemoveResponseRule(TEXT("清除"));
```

#### 智能响应工作原理
当用户输入包含触发词时，系统会自动执行对应的响应命令：
- 用户输入："请帮助我" → 自动执行 "help"
- 用户输入："查看状态" → 自动执行 "status"
- 用户输入："播放音乐" → 自动执行 "generate music 默认音乐"

### 🔄 工作流

#### 创建和执行工作流
```cpp
// 创建音乐生成工作流
TArray<FString> MusicWorkflowSteps = {
    TEXT("mode chat"),
    TEXT("准备生成音乐"),
    TEXT("generate music 工作流测试音乐"),
    TEXT("play last"),
    TEXT("mode command")
};
UMingSageBrainConsoleAutomation::CreateWorkflow(TEXT("MusicGeneration"), MusicWorkflowSteps);

// 执行工作流
UMingSageBrainConsoleAutomation::ExecuteWorkflow(TEXT("MusicGeneration"));
```

#### 复杂工作流示例
```cpp
// 创建完整的演示工作流
TArray<FString> DemoWorkflowSteps = {
    TEXT("clear"),
    TEXT("help"),
    TEXT("status"),
    TEXT("start dialogue music_creation"),
    TEXT("generate music 史诗明朝音乐"),
    TEXT("generate scene 故宫"),
    TEXT("show achievements"),
    TEXT("end dialogue"),
    TEXT("clear")
};
UMingSageBrainConsoleAutomation::CreateWorkflow(TEXT("FullDemo"), DemoWorkflowSteps);
UMingSageBrainConsoleAutomation::ExecuteWorkflow(TEXT("FullDemo"));
```

### 📊 监控和状态

#### 系统状态
```cpp
// 获取自动化系统状态
FString Status = UMingSageBrainConsoleAutomation::GetAutomationStatus();

// 获取执行历史
TArray<FString> History = UMingSageBrainConsoleAutomation::GetExecutionHistory();

// 清除执行历史
UMingSageBrainConsoleAutomation::ClearExecutionHistory();

// 导出执行报告
UMingSageBrainConsoleAutomation::ExportExecutionReport(TEXT("AutomationReport.txt"));
```

### ⚙️ 高级功能

#### 条件触发
```cpp
// 设置条件触发器
UMingSageBrainConsoleAutomation::SetConditionalTrigger(
    TEXT("错误"), 
    TEXT("show error log")
);
```

#### 错误处理和恢复
```cpp
// 设置错误重试参数
UMingSageBrainConsoleAutomation::SetErrorRetry(5, 2.0f); // 最多重试5次，延迟2秒

// 启用自动恢复
UMingSageBrainConsoleAutomation::EnableAutoRecovery();

// 手动恢复
UMingSageBrainConsoleAutomation::ManualRecovery();
```

#### 配置管理
```cpp
// 保存配置
UMingSageBrainConsoleAutomation::SaveConfiguration(TEXT("AutomationConfig.json"));

// 加载配置
UMingSageBrainConsoleAutomation::LoadConfiguration(TEXT("AutomationConfig.json"));

// 重置配置
UMingSageBrainConsoleAutomation::ResetConfiguration();
```

## 实用示例

### 🎵 音乐生成自动化
```cpp
// 创建音乐生成自动化序列
void AutoGenerateMusic(const TArray<FString>& MusicDescriptions)
{
    // 启动自动化系统
    UMingSageBrainConsoleAutomation::StartAndInitialize();
    
    // 为每个音乐描述创建定时任务
    for (int32 i = 0; i < MusicDescriptions.Num(); i++)
    {
        FString TaskName = FString::Printf(TEXT("MusicTask_%d"), i);
        FString Command = FString::Printf(TEXT("generate music %s"), *MusicDescriptions[i]);
        float Delay = i * 30.0f; // 每30秒生成一首
        
        UMingSageBrainConsoleAutomation::AddScheduledTask(TaskName, Command, Delay);
    }
}

// 使用示例
TArray<FString> MusicList = {
    TEXT("史诗战斗音乐"),
    TEXT("和平的宫廷音乐"),
    TEXT("神秘的探索音乐"),
    TEXT("胜利的凯旋音乐")
};
AutoGenerateMusic(MusicList);
```

### 🎬 场景生成自动化
```cpp
// 场景生成自动化
void AutoGenerateScenes()
{
    TArray<FString> SceneCommands = {
        TEXT("clear"),
        TEXT("status"),
        TEXT("generate scene 故宫太和殿"),
        TEXT("generate scene 长城"),
        TEXT("generate scene 苏州园林"),
        TEXT("generate scene 黄山"),
        TEXT("status")
    };
    
    UMingSageBrainConsoleAutomation::ExecuteBatchCommands(SceneCommands);
}
```

### 🤖 智能助手自动化
```cpp
// 创建智能助手
void CreateIntelligentAssistant()
{
    // 启动自动化系统
    UMingSageBrainConsoleAutomation::StartAndInitialize();
    
    // 启用智能响应
    UMingSageBrainConsoleAutomation::EnableIntelligentResponse();
    
    // 添加智能响应规则
    UMingSageBrainConsoleAutomation::AddResponseRule(TEXT("你好"), TEXT("echo 你好！我是圣者大脑助手"));
    UMingSageBrainConsoleAutomation::AddResponseRule(TEXT("音乐"), TEXT("generate music 为您生成的音乐"));
    UMingSageBrainConsoleAutomation::AddResponseRule(TEXT("场景"), TEXT("generate scene 为您生成的场景"));
    UMingSageBrainConsoleAutomation::AddResponseRule(TEXT("帮助"), TEXT("help"));
    UMingSageBrainConsoleAutomation::AddResponseRule(TEXT("状态"), TEXT("status"));
    
    // 添加欢迎消息
    UMingSageBrainConsoleStarter::SendCommand(TEXT("echo 智能助手已启动，您可以："));
    UMingSageBrainConsoleStarter::SendCommand(TEXT("echo - 说'你好'与我打招呼"));
    UMingSageBrainConsoleStarter::SendCommand(TEXT("echo - 说'音乐'生成音乐"));
    UMingSageBrainConsoleStarter::SendCommand(TEXT("echo - 说'场景'生成场景"));
    UMingSageBrainConsoleStarter::SendCommand(TEXT("echo - 说'帮助'查看更多功能"));
}
```

### 📈 性能监控自动化
```cpp
// 性能监控自动化
void SetupPerformanceMonitoring()
{
    // 每5分钟检查一次性能
    UMingSageBrainConsoleAutomation::AddRepeatingTask(
        TEXT("PerfCheck"), 
        TEXT("performance stats"), 
        300.0f, 
        -1  // 无限重复
    );
    
    // 每小时导出一次日志
    UMingSageBrainConsoleAutomation::AddRepeatingTask(
        TEXT("LogExport"), 
        TEXT("export logs"), 
        3600.0f, 
        -1
    );
    
    // 每天生成一次状态报告
    UMingSageBrainConsoleAutomation::AddRepeatingTask(
        TEXT("DailyReport"), 
        TEXT("generate daily report"), 
        86400.0f, 
        -1
    );
}
```

## Blueprint 使用

所有自动化功能都支持 Blueprint：

1. **右键点击** → 搜索 "Sage Brain Automation"
2. **选择功能** → 连接到事件图表
3. **配置参数** → 运行游戏

### Blueprint 示例流程
```
BeginPlay → StartAndInitialize → 
    ↳ EnableIntelligentResponse → 
    ↳ AddResponseRule("音乐", "generate music") → 
    ↳ AddRepeatingTask("MusicGen", "generate music 定时音乐", 60, -1)
```

## 最佳实践

### 1. 错误处理
```cpp
// 始终检查启动结果
if (UMingSageBrainConsoleAutomation::StartAndInitialize())
{
    // 设置错误处理
    UMingSageBrainConsoleAutomation::SetErrorRetry(3, 1.0f);
    UMingSageBrainConsoleAutomation::EnableAutoRecovery();
    
    // 执行自动化任务
    // ...
}
else
{
    UE_LOG(LogTemp, Error, TEXT("自动化系统启动失败"));
}
```

### 2. 资源管理
```cpp
// 游戏结束时清理资源
void AMyGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    
    // 关闭自动化系统
    UMingSageBrainConsoleAutomation::ShutdownAutomationSystem();
}
```

### 3. 配置管理
```cpp
// 游戏启动时加载配置
void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // 启动自动化系统
    if (UMingSageBrainConsoleAutomation::StartAndInitialize())
    {
        // 加载自定义配置
        FString ConfigPath = FPaths::ProjectConfigDir() + TEXT("AutomationConfig.json");
        if (FPaths::FileExists(ConfigPath))
        {
            UMingSageBrainConsoleAutomation::LoadConfiguration(ConfigPath);
        }
    }
}
```

### 4. 性能优化
```cpp
// 合理设置延迟和重试
UMingSageBrainConsoleAutomation::SetErrorRetry(2, 0.5f); // 减少重试次数和延迟

// 避免过于频繁的任务
UMingSageBrainConsoleAutomation::AddRepeatingTask(
    TEXT("ModerateFreq"), 
    TEXT("status"), 
    30.0f,  // 至少30秒间隔
    -1
);
```

## 故障排除

### 常见问题

1. **自动化系统启动失败**
   - 检查控制台是否已启动
   - 确认没有重复实例
   - 查看引擎日志

2. **定时任务不执行**
   - 检查任务是否正确添加
   - 确认时间设置合理
   - 验证命令语法正确

3. **智能响应不工作**
   - 确认智能响应已启用
   - 检查响应规则设置
   - 验证触发词匹配

4. **工作流执行失败**
   - 检查工作流步骤
   - 确认每个命令有效
   - 查看执行历史

### 调试技巧

1. **查看状态**
   ```cpp
   FString Status = UMingSageBrainConsoleAutomation::GetAutomationStatus();
   UE_LOG(LogTemp, Log, TEXT("自动化状态: %s"), *Status);
   ```

2. **检查执行历史**
   ```cpp
   TArray<FString> History = UMingSageBrainConsoleAutomation::GetExecutionHistory();
   for (const FString& Entry : History)
   {
       UE_LOG(LogTemp, Log, TEXT("历史: %s"), *Entry);
   }
   ```

3. **导出调试报告**
   ```cpp
   UMingSageBrainConsoleAutomation::ExportExecutionReport(TEXT("DebugReport.txt"));
   ```

通过这个强大的自动化系统，您可以大幅提升与圣者大脑交互的效率，实现复杂的自动化工作流程！
