# 圣者大脑控制台启动器使用指南

## 概述

`MingSageBrainConsoleStarter` 提供了一键启动圣者大脑控制台的便捷功能，支持多种启动模式和配置选项。

## 基本使用

### 1. 一键启动
```cpp
// 最简单的启动方式
bool bSuccess = UMingSageBrainConsoleStarter::StartSageBrainConsole();

// 快速启动
bool bSuccess = UMingSageBrainConsoleStarter::QuickStart();

// 启动并显示终端
bool bSuccess = UMingSageBrainConsoleStarter::StartAndShow();
```

### 2. 模式启动
```cpp
// 聊天模式
bool bSuccess = UMingSageBrainConsoleStarter::StartChatMode();

// 对话模式
bool bSuccess = UMingSageBrainConsoleStarter::StartDialogueMode();

// 向导模式
bool bSuccess = UMingSageBrainConsoleStarter::StartWizardMode();
```

### 3. 自定义启动
```cpp
// 自定义配置启动
bool bSuccess = UMingSageBrainConsoleStarter::CustomStart(
    EUserInteractionMode::Chat,
    TEXT("Dark"),
    true
);

// 启动并执行命令
TArray<FString> Commands = {
    TEXT("help"),
    TEXT("status"),
    TEXT("generate music 史诗战斗音乐")
};
bool bSuccess = UMingSageBrainConsoleStarter::StartWithCommands(Commands);

// 启动并加载脚本
bool bSuccess = UMingSageBrainConsoleStarter::StartWithScript(TEXT("StartupScript.txt"));
```

## 预设模式

### 开发者模式
```cpp
// 启动开发者模式 - 深色主题，完整显示，命令行模式
bool bSuccess = UMingSageBrainConsoleStarter::StartDeveloperMode();
```

### 用户模式
```cpp
// 启动用户模式 - 浅色主题，增强显示，聊天模式
bool bSuccess = UMingSageBrainConsoleStarter::StartUserMode();
```

### 演示模式
```cpp
// 启动演示模式 - 蓝色主题，增强显示，向导模式
bool bSuccess = UMingSageBrainConsoleStarter::StartDemoMode();
```

### 测试模式
```cpp
// 启动测试模式 - 绿色主题，最小显示，命令行模式
bool bSuccess = UMingSageBrainConsoleStarter::StartTestMode();
```

## 状态管理

### 检查状态
```cpp
// 检查控制台是否运行
bool bIsRunning = UMingSageBrainConsoleStarter::IsConsoleRunning();

// 获取启动器实例
UMingSageBrainConsoleLauncher* Launcher = UMingSageBrainConsoleStarter::GetConsoleLauncher();

// 获取终端
UMingSageBrainConsoleTerminal* Terminal = UMingSageBrainConsoleStarter::GetTerminal();

// 获取交互式控制台
UMingSageBrainInteractiveConsole* Console = UMingSageBrainConsoleStarter::GetInteractiveConsole();
```

## 命令发送

### 基本命令
```cpp
// 发送任意命令
bool bSuccess = UMingSageBrainConsoleStarter::SendCommand(TEXT("help"));

// 发送聊天消息
bool bSuccess = UMingSageBrainConsoleStarter::SendChatMessage(TEXT("你好，圣者大脑"));

// 显示帮助
bool bSuccess = UMingSageBrainConsoleStarter::ShowHelp();

// 显示状态
bool bSuccess = UMingSageBrainConsoleStarter::ShowStatus();
```

### 生成命令
```cpp
// 生成音乐
bool bSuccess = UMingSageBrainConsoleStarter::GenerateMusic(TEXT("史诗战斗音乐"));

// 生成场景
bool bSuccess = UMingSageBrainConsoleStarter::GenerateScene(TEXT("明朝宫殿"));
```

## 终端控制

### 显示控制
```cpp
// 显示终端
bool bSuccess = UMingSageBrainConsoleStarter::ShowTerminal();

// 隐藏终端
bool bSuccess = UMingSageBrainConsoleStarter::HideTerminal();

// 切换终端显示
bool bSuccess = UMingSageBrainConsoleStarter::ToggleTerminal();
```

### 主题和样式
```cpp
// 切换主题
bool bSuccess = UMingSageBrainConsoleStarter::SwitchTheme(TEXT("Dark"));

// 清除输出
bool bSuccess = UMingSageBrainConsoleStarter::ClearOutput();
```

## 关闭和重启

### 关闭控制台
```cpp
// 关闭控制台
bool bSuccess = UMingSageBrainConsoleStarter::ShutdownConsole();

// 重启控制台
bool bSuccess = UMingSageBrainConsoleStarter::RestartConsole();
```

## Blueprint 使用

在 Blueprint 中，所有这些函数都可以直接调用：

1. **右键点击** -> 搜索 "Sage Brain Console"
2. **选择相应功能** -> 连接到事件图表
3. **配置参数** -> 运行游戏

## 常用启动序列

### 开发者启动序列
```cpp
// 1. 启动开发者模式
UMingSageBrainConsoleStarter::StartDeveloperMode();

// 2. 等待启动完成
FPlatformProcess::Sleep(1.0f);

// 3. 执行开发者命令
TArray<FString> DevCommands = {
    TEXT("status"),
    TEXT("mode command"),
    TEXT("test api"),
    TEXT("generate music 测试音乐")
};
UMingSageBrainConsoleStarter::ExecuteCommands(DevCommands);
```

### 用户演示序列
```cpp
// 1. 启动演示模式
UMingSageBrainConsoleStarter::StartDemoMode();

// 2. 等待启动完成
FPlatformProcess::Sleep(1.0f);

// 3. 执行演示命令
TArray<FString> DemoCommands = {
    TEXT("help"),
    TEXT("start dialogue music_creation"),
    TEXT("generate music 史诗明朝音乐"),
    TEXT("generate scene 故宫")
};
UMingSageBrainConsoleStarter::ExecuteCommands(DemoCommands);
```

## 错误处理

### 检查启动结果
```cpp
bool bSuccess = UMingSageBrainConsoleStarter::StartSageBrainConsole();

if (bSuccess)
{
    // 启动成功
    UE_LOG(LogTemp, Log, TEXT("圣者大脑控制台启动成功"));
    
    // 获取组件进行进一步操作
    UMingSageBrainConsoleTerminal* Terminal = UMingSageBrainConsoleStarter::GetTerminal();
    if (Terminal)
    {
        Terminal->AddOutput(TEXT("自定义欢迎消息"), EConsoleOutputLevel::Success, TEXT("Game"));
    }
}
else
{
    // 启动失败
    UE_LOG(LogTemp, Error, TEXT("圣者大脑控制台启动失败"));
}
```

### 安全关闭
```cpp
// 检查是否运行
if (UMingSageBrainConsoleStarter::IsConsoleRunning())
{
    // 安全关闭
    UMingSageBrainConsoleStarter::ShutdownConsole();
}
```

## 性能建议

1. **单例模式**: 启动器使用单例模式，避免重复创建实例
2. **延迟加载**: 只在需要时才创建和初始化组件
3. **资源清理**: 关闭时自动清理所有相关资源
4. **状态检查**: 操作前检查组件状态，避免无效操作

## 最佳实践

1. **启动检查**: 始终检查启动结果
2. **资源管理**: 游戏结束时调用 ShutdownConsole
3. **错误处理**: 为所有操作添加错误处理
4. **日志记录**: 使用日志记录重要操作
5. **用户反馈**: 为用户提供清晰的状态反馈

## 故障排除

### 常见问题

1. **启动失败**: 检查是否已存在运行实例
2. **命令无响应**: 确认控制台已完全启动
3. **主题切换失败**: 确认主题名称正确
4. **脚本加载失败**: 检查脚本文件路径和权限

### 调试技巧

1. **查看日志**: 检查控制台和引擎日志
2. **状态检查**: 使用 IsConsoleRunning() 检查状态
3. **组件验证**: 检查获取的组件是否为空
4. **逐步测试**: 从简单功能开始测试

通过这个启动器，您可以轻松地启动和管理圣者大脑控制台，享受与AI的智能交互体验！
