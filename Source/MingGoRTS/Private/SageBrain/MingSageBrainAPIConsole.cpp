#include "MingSageBrainAPIConsole.h"
#include "Engine/Engine.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "TimerManager.h"

UMingSageBrainAPIConsole::UMingSageBrainAPIConsole()
    : bIsInitialized(false)
    , CurrentHistoryIndex(-1)
{
}

bool UMingSageBrainAPIConsole::InitializeConsole()
{
    if (bIsInitialized)
    {
        LogMessage(TEXT("控制台已經初始化"), EConsoleOutputLevel::Warning);
        return true;
    }

    // 初始化集成API
    IntegratedAPI = NewObject<UMingSageBrainIntegratedAPI>(this);
    if (!IntegratedAPI)
    {
        LogMessage(TEXT("無法創建集成API實例"), EConsoleOutputLevel::Error);
        return false;
    }

    if (!IntegratedAPI->InitializeIntegratedAPI())
    {
        LogMessage(TEXT("集成API初始化失敗"), EConsoleOutputLevel::Error);
        return false;
    }

    // 初始化默認配置和快捷命令
    InitializeDefaultConfig();
    InitializeDefaultShortcuts();

    bIsInitialized = true;
    LogMessage(TEXT("聖者大腦API控制台初始化完成"), EConsoleOutputLevel::Success);
    LogMessage(TEXT("輸入 'help' 查看可用命令"), EConsoleOutputLevel::Info);

    OnConsoleStatusChanged.Broadcast(TEXT("Ready"));
    return true;
}

void UMingSageBrainAPIConsole::ShutdownConsole()
{
    if (!bIsInitialized)
    {
        return;
    }

    if (IntegratedAPI)
    {
        IntegratedAPI->ShutdownIntegratedAPI();
        IntegratedAPI = nullptr;
    }

    bIsInitialized = false;
    LogMessage(TEXT("控制台已關閉"), EConsoleOutputLevel::Info);
    OnConsoleStatusChanged.Broadcast(TEXT("Shutdown"));
}

FConsoleCommandResult UMingSageBrainAPIConsole::ExecuteCommand(const FString& CommandLine)
{
    FConsoleCommand Command = ParseCommand(CommandLine);
    return ExecuteCommandStruct(Command);
}

FConsoleCommandResult UMingSageBrainAPIConsole::ExecuteCommandStruct(const FConsoleCommand& Command)
{
    FConsoleCommandResult Result;
    double StartTime = FPlatformTime::Seconds();

    if (!bIsInitialized)
    {
        Result.bSuccess = false;
        Result.Message = TEXT("控制台未初始化");
        LogMessage(Result.Message, EConsoleOutputLevel::Error);
        return Result;
    }

    LogMessage(FString::Printf(TEXT("> %s"), *Command.RawInput), EConsoleOutputLevel::Debug);

    // 根據命令類型處理
    switch (Command.Type)
    {
    case ESageBrainConsoleCommandType::Help:
        Result = ProcessHelpCommand(Command);
        break;
    case ESageBrainConsoleCommandType::Status:
        Result = ProcessStatusCommand(Command);
        break;
    case ESageBrainConsoleCommandType::Generate:
        Result = ProcessGenerateCommand(Command);
        break;
    case ESageBrainConsoleCommandType::Query:
        Result = ProcessQueryCommand(Command);
        break;
    case ESageBrainConsoleCommandType::Config:
        Result = ProcessConfigCommand(Command);
        break;
    case ESageBrainConsoleCommandType::Test:
        Result = ProcessTestCommand(Command);
        break;
    case ESageBrainConsoleCommandType::Export:
        Result = ProcessExportCommand(Command);
        break;
    case ESageBrainConsoleCommandType::Import:
        Result = ProcessImportCommand(Command);
        break;
    case ESageBrainConsoleCommandType::Clear:
        Result = ProcessClearCommand(Command);
        break;
    default:
        Result.bSuccess = false;
        Result.Message = FString::Printf(TEXT("未知命令類型: %s"), *Command.CommandString);
        LogMessage(Result.Message, EConsoleOutputLevel::Error);
        break;
    }

    Result.ExecutionTime = FPlatformTime::Seconds() - StartTime;

    // 添加到歷史
    AddToHistory(Command, Result);

    // 廣播事件
    OnCommandExecuted.Broadcast(Command, Result);

    return Result;
}

void UMingSageBrainAPIConsole::ExecuteCommandAsync(const FString& CommandLine)
{
    AsyncTask(ENamedThreads::GameThread, [this, CommandLine]()
    {
        ExecuteCommand(CommandLine);
    });
}

FConsoleCommand UMingSageBrainAPIConsole::ParseCommand(const FString& CommandLine) const
{
    FConsoleCommand Command;
    Command.RawInput = CommandLine;

    // 解析命令行
    TArray<FString> Tokens;
    CommandLine.ParseIntoArray(Tokens, TEXT(" "), true);

    if (Tokens.Num() == 0)
    {
        Command.Type = ESageBrainConsoleCommandType::Help;
        return Command;
    }

    // 解析主命令
    FString MainCommand = Tokens[0].ToLower();
    Command.CommandString = MainCommand;

    // 映射到命令類型
    if (MainCommand == TEXT("help") || MainCommand == TEXT("h") || MainCommand == TEXT("?"))
    {
        Command.Type = ESageBrainConsoleCommandType::Help;
    }
    else if (MainCommand == TEXT("status") || MainCommand == TEXT("stat"))
    {
        Command.Type = ESageBrainConsoleCommandType::Status;
    }
    else if (MainCommand == TEXT("generate") || MainCommand == TEXT("gen") || MainCommand == TEXT("g"))
    {
        Command.Type = ESageBrainConsoleCommandType::Generate;
    }
    else if (MainCommand == TEXT("query") || MainCommand == TEXT("q") || MainCommand == TEXT("search"))
    {
        Command.Type = ESageBrainConsoleCommandType::Query;
    }
    else if (MainCommand == TEXT("config") || MainCommand == TEXT("cfg") || MainCommand == TEXT("set"))
    {
        Command.Type = ESageBrainConsoleCommandType::Config;
    }
    else if (MainCommand == TEXT("test") || MainCommand == TEXT("t"))
    {
        Command.Type = ESageBrainConsoleCommandType::Test;
    }
    else if (MainCommand == TEXT("export") || MainCommand == TEXT("ex"))
    {
        Command.Type = ESageBrainConsoleCommandType::Export;
    }
    else if (MainCommand == TEXT("import") || MainCommand == TEXT("im"))
    {
        Command.Type = ESageBrainConsoleCommandType::Import;
    }
    else if (MainCommand == TEXT("clear") || MainCommand == TEXT("cls"))
    {
        Command.Type = ESageBrainConsoleCommandType::Clear;
    }
    else
    {
        Command.Type = ESageBrainConsoleCommandType::Custom;
    }

    // 解析參數
    for (int32 i = 1; i < Tokens.Num(); i++)
    {
        FString Token = Tokens[i];

        // 檢查是否為命名參數 (--key=value 或 -k value)
        if (Token.StartsWith(TEXT("--")))
        {
            FString Param = Token.RightChop(2);
            FString Key, Value;
            if (Param.Split(TEXT("="), &Key, &Value))
            {
                Command.Parameters.Add(Key, Value);
            }
            else if (i + 1 < Tokens.Num())
            {
                Command.Parameters.Add(Param, Tokens[++i]);
            }
        }
        else if (Token.StartsWith(TEXT("-")))
        {
            FString Flag = Token.RightChop(1);
            Command.Arguments.Add(Flag);
        }
        else
        {
            Command.Arguments.Add(Token);
        }
    }

    return Command;
}

TArray<FString> UMingSageBrainAPIConsole::GetCommandSuggestions(const FString& PartialInput) const
{
    TArray<FString> Suggestions;

    TArray<FString> AllCommands = {
        TEXT("help"), TEXT("status"), TEXT("generate"), TEXT("query"),
        TEXT("config"), TEXT("test"), TEXT("export"), TEXT("import"),
        TEXT("clear"), TEXT("exit")
    };

    FString LowerInput = PartialInput.ToLower();
    for (const FString& Cmd : AllCommands)
    {
        if (Cmd.StartsWith(LowerInput))
        {
            Suggestions.Add(Cmd);
        }
    }

    // 添加快捷命令建議
    for (const auto& Pair : Shortcuts)
    {
        if (Pair.Key.StartsWith(LowerInput))
        {
            Suggestions.Add(Pair.Key);
        }
    }

    return Suggestions;
}

bool UMingSageBrainAPIConsole::ValidateCommand(const FString& CommandLine) const
{
    FConsoleCommand Command = ParseCommand(CommandLine);
    return Command.Type != ESageBrainConsoleCommandType::Custom || Shortcuts.Contains(Command.CommandString);
}

void UMingSageBrainAPIConsole::LogMessage(const FString& Message, EConsoleOutputLevel Level, const FString& Category)
{
    FConsoleOutputItem Item;
    Item.Message = Message;
    Item.Level = Level;
    Item.Category = Category;
    Item.Timestamp = FDateTime::Now();

    OutputHistory.Add(Item);

    // 限制歷史記錄大小
    if (OutputHistory.Num() > 1000)
    {
        OutputHistory.RemoveAt(0, 100);
    }

    OnConsoleOutput.Broadcast(Item);
}

void UMingSageBrainAPIConsole::ClearOutput()
{
    OutputHistory.Empty();
    LogMessage(TEXT("輸出歷史已清除"), EConsoleOutputLevel::Info);
}

TArray<FConsoleOutputItem> UMingSageBrainAPIConsole::GetOutputHistory(int32 MaxItems) const
{
    int32 StartIndex = FMath::Max(0, OutputHistory.Num() - MaxItems);
    int32 Count = FMath::Min(MaxItems, OutputHistory.Num() - StartIndex);
    
    TArray<FConsoleOutputItem> Result;
    for (int32 i = StartIndex; i < StartIndex + Count; i++)
    {
        Result.Add(OutputHistory[i]);
    }
    return Result;
}

FString UMingSageBrainAPIConsole::GetFormattedOutput() const
{
    FString Output;
    for (const FConsoleOutputItem& Item : OutputHistory)
    {
        FString LevelStr;
        switch (Item.Level)
        {
        case EConsoleOutputLevel::Debug: LevelStr = TEXT("[DEBUG]"); break;
        case EConsoleOutputLevel::Info: LevelStr = TEXT("[INFO]"); break;
        case EConsoleOutputLevel::Warning: LevelStr = TEXT("[WARN]"); break;
        case EConsoleOutputLevel::Error: LevelStr = TEXT("[ERROR]"); break;
        case EConsoleOutputLevel::Success: LevelStr = TEXT("[SUCCESS]"); break;
        }

        Output += FString::Printf(TEXT("%s %s %s: %s\n"),
            *Item.Timestamp.ToString(TEXT("%H:%M:%S")),
            *LevelStr,
            *Item.Category,
            *Item.Message);
    }
    return Output;
}

void UMingSageBrainAPIConsole::AddToHistory(const FConsoleCommand& Command, const FConsoleCommandResult& Result)
{
    FConsoleHistoryEntry Entry;
    Entry.Command = Command;
    Entry.Result = Result;
    Entry.ExecutionTime = FDateTime::Now();

    CommandHistory.Add(Entry);
    CurrentHistoryIndex = CommandHistory.Num();

    // 限制歷史大小
    if (CommandHistory.Num() > 100)
    {
        CommandHistory.RemoveAt(0, 10);
    }
}

TArray<FConsoleHistoryEntry> UMingSageBrainAPIConsole::GetCommandHistory(int32 MaxEntries) const
{
    int32 StartIndex = FMath::Max(0, CommandHistory.Num() - MaxEntries);
    int32 Count = FMath::Min(MaxEntries, CommandHistory.Num() - StartIndex);
    
    TArray<FConsoleHistoryEntry> Result;
    for (int32 i = StartIndex; i < StartIndex + Count; i++)
    {
        Result.Add(CommandHistory[i]);
    }
    return Result;
}

void UMingSageBrainAPIConsole::ClearHistory()
{
    CommandHistory.Empty();
    CurrentHistoryIndex = -1;
    LogMessage(TEXT("命令歷史已清除"), EConsoleOutputLevel::Info);
}

FString UMingSageBrainAPIConsole::GetPreviousCommand()
{
    if (CommandHistory.Num() == 0 || CurrentHistoryIndex <= 0)
    {
        return FString();
    }

    CurrentHistoryIndex--;
    return CommandHistory[CurrentHistoryIndex].Command.RawInput;
}

FString UMingSageBrainAPIConsole::GetNextCommand()
{
    if (CommandHistory.Num() == 0 || CurrentHistoryIndex >= CommandHistory.Num() - 1)
    {
        CurrentHistoryIndex = CommandHistory.Num();
        return FString();
    }

    CurrentHistoryIndex++;
    return CommandHistory[CurrentHistoryIndex].Command.RawInput;
}

void UMingSageBrainAPIConsole::RegisterShortcut(const FString& ShortcutName, const FString& CommandTemplate)
{
    Shortcuts.Add(ShortcutName, CommandTemplate);
    LogMessage(FString::Printf(TEXT("已註冊快捷命令: %s"), *ShortcutName), EConsoleOutputLevel::Info);
}

void UMingSageBrainAPIConsole::RemoveShortcut(const FString& ShortcutName)
{
    Shortcuts.Remove(ShortcutName);
    LogMessage(FString::Printf(TEXT("已移除快捷命令: %s"), *ShortcutName), EConsoleOutputLevel::Info);
}

TMap<FString, FString> UMingSageBrainAPIConsole::GetAllShortcuts() const
{
    return Shortcuts;
}

FString UMingSageBrainAPIConsole::ExecuteShortcut(const FString& ShortcutName, const TMap<FString, FString>& Parameters)
{
    FString* Template = Shortcuts.Find(ShortcutName);
    if (!Template)
    {
        LogMessage(FString::Printf(TEXT("快捷命令不存在: %s"), *ShortcutName), EConsoleOutputLevel::Error);
        return FString();
    }

    FString CommandLine = ReplaceParameters(*Template, Parameters);
    ExecuteCommand(CommandLine);
    return CommandLine;
}

void UMingSageBrainAPIConsole::ExecuteBatchCommands(const TArray<FString>& Commands)
{
    LogMessage(FString::Printf(TEXT("開始執行批處理命令 (%d 條)"), Commands.Num()), EConsoleOutputLevel::Info);

    for (int32 i = 0; i < Commands.Num(); i++)
    {
        LogMessage(FString::Printf(TEXT("[%d/%d] 執行: %s"), i + 1, Commands.Num(), *Commands[i]), EConsoleOutputLevel::Info);
        ExecuteCommand(Commands[i]);
    }

    LogMessage(TEXT("批處理命令執行完成"), EConsoleOutputLevel::Success);
}

void UMingSageBrainAPIConsole::LoadAndExecuteScript(const FString& ScriptPath)
{
    if (!FPaths::FileExists(ScriptPath))
    {
        LogMessage(FString::Printf(TEXT("腳本文件不存在: %s"), *ScriptPath), EConsoleOutputLevel::Error);
        return;
    }

    FString ScriptContent;
    if (!FFileHelper::LoadFileToString(ScriptContent, *ScriptPath))
    {
        LogMessage(FString::Printf(TEXT("無法讀取腳本文件: %s"), *ScriptPath), EConsoleOutputLevel::Error);
        return;
    }

    TArray<FString> Lines;
    ScriptContent.ParseIntoArrayLines(Lines, true);

    TArray<FString> Commands;
    for (FString& Line : Lines)
    {
        Line = Line.TrimStartAndEnd();
        if (!Line.IsEmpty() && !Line.StartsWith(TEXT("#")) && !Line.StartsWith(TEXT("//")))
        {
            Commands.Add(Line);
        }
    }

    ExecuteBatchCommands(Commands);
}

void UMingSageBrainAPIConsole::SaveCommandHistoryToScript(const FString& ScriptPath)
{
    FString ScriptContent = TEXT("# Sage Brain API Console Script\n");
    ScriptContent += FString::Printf(TEXT("# Generated: %s\n\n"), *FDateTime::Now().ToString());

    for (const FConsoleHistoryEntry& Entry : CommandHistory)
    {
        if (Entry.Result.bSuccess)
        {
            ScriptContent += Entry.Command.RawInput + TEXT("\n");
        }
    }

    if (FFileHelper::SaveStringToFile(ScriptContent, *ScriptPath))
    {
        LogMessage(FString::Printf(TEXT("命令歷史已保存到: %s"), *ScriptPath), EConsoleOutputLevel::Success);
    }
    else
    {
        LogMessage(FString::Printf(TEXT("無法保存腳本: %s"), *ScriptPath), EConsoleOutputLevel::Error);
    }
}

FString UMingSageBrainAPIConsole::GetConsoleStatus() const
{
    if (!bIsInitialized)
    {
        return TEXT("Not Initialized");
    }

    return FString::Printf(TEXT("Ready | History: %d commands | Output: %d items"),
        CommandHistory.Num(), OutputHistory.Num());
}

FString UMingSageBrainAPIConsole::GetSystemInfo() const
{
    FString Info = TEXT("=== Sage Brain API Console ===\n");
    Info += FString::Printf(TEXT("Status: %s\n"), bIsInitialized ? TEXT("Initialized") : TEXT("Not Initialized"));
    Info += FString::Printf(TEXT("IntegratedAPI: %s\n"), IntegratedAPI ? TEXT("Connected") : TEXT("Disconnected"));
    Info += FString::Printf(TEXT("Command History: %d entries\n"), CommandHistory.Num());
    Info += FString::Printf(TEXT("Output Items: %d entries\n"), OutputHistory.Num());
    Info += FString::Printf(TEXT("Shortcuts: %d registered\n"), Shortcuts.Num());
    Info += TEXT("===========================");
    return Info;
}

TMap<FString, FString> UMingSageBrainAPIConsole::GetAvailableCommands() const
{
    TMap<FString, FString> Commands;
    Commands.Add(TEXT("help"), TEXT("顯示幫助信息"));
    Commands.Add(TEXT("status"), TEXT("顯示系統狀態"));
    Commands.Add(TEXT("generate"), TEXT("生成內容 (音樂/資產/場景)"));
    Commands.Add(TEXT("query"), TEXT("查詢信息"));
    Commands.Add(TEXT("config"), TEXT("配置設置"));
    Commands.Add(TEXT("test"), TEXT("運行測試"));
    Commands.Add(TEXT("export"), TEXT("導出數據"));
    Commands.Add(TEXT("import"), TEXT("導入數據"));
    Commands.Add(TEXT("clear"), TEXT("清除輸出"));
    return Commands;
}

void UMingSageBrainAPIConsole::SetConfigValue(const FString& Key, const FString& Value)
{
    ConfigValues.Add(Key, Value);
    LogMessage(FString::Printf(TEXT("配置已設置: %s = %s"), *Key, *Value), EConsoleOutputLevel::Info);
}

FString UMingSageBrainAPIConsole::GetConfigValue(const FString& Key) const
{
    const FString* Value = ConfigValues.Find(Key);
    return Value ? *Value : FString();
}

void UMingSageBrainAPIConsole::LoadConfig(const FString& ConfigPath)
{
    if (!FPaths::FileExists(ConfigPath))
    {
        LogMessage(FString::Printf(TEXT("配置文件不存在: %s"), *ConfigPath), EConsoleOutputLevel::Warning);
        return;
    }

    FString JsonContent;
    if (!FFileHelper::LoadFileToString(JsonContent, *ConfigPath))
    {
        LogMessage(FString::Printf(TEXT("無法讀取配置文件: %s"), *ConfigPath), EConsoleOutputLevel::Error);
        return;
    }

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonContent);

    if (FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        for (const auto& Pair : JsonObject->Values)
        {
            FString Value;
            if (Pair.Value->TryGetString(Value))
            {
                SetConfigValue(Pair.Key, Value);
            }
        }
        LogMessage(FString::Printf(TEXT("配置已從 %s 加載"), *ConfigPath), EConsoleOutputLevel::Success);
    }
    else
    {
        LogMessage(TEXT("配置文件格式錯誤"), EConsoleOutputLevel::Error);
    }
}

void UMingSageBrainAPIConsole::SaveConfig(const FString& ConfigPath)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

    for (const auto& Pair : ConfigValues)
    {
        JsonObject->SetStringField(Pair.Key, Pair.Value);
    }

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    if (FFileHelper::SaveStringToFile(OutputString, *ConfigPath))
    {
        LogMessage(FString::Printf(TEXT("配置已保存到 %s"), *ConfigPath), EConsoleOutputLevel::Success);
    }
    else
    {
        LogMessage(FString::Printf(TEXT("無法保存配置: %s"), *ConfigPath), EConsoleOutputLevel::Error);
    }
}

// 命令處理器實現
FConsoleCommandResult UMingSageBrainAPIConsole::ProcessHelpCommand(const FConsoleCommand& Command)
{
    FConsoleCommandResult Result;
    Result.bSuccess = true;

    if (Command.Arguments.Num() > 0)
    {
        Result.Message = GetCommandHelp(Command.Arguments[0]);
    }
    else
    {
        Result.Message = GetHelpText();
    }

    LogMessage(Result.Message, EConsoleOutputLevel::Info, TEXT("Help"));
    return Result;
}

FConsoleCommandResult UMingSageBrainAPIConsole::ProcessStatusCommand(const FConsoleCommand& Command)
{
    FConsoleCommandResult Result;
    Result.bSuccess = true;
    Result.Message = GetSystemInfo();

    if (IntegratedAPI)
    {
        Result.Data.Add(TEXT("SystemStatus"), IntegratedAPI->GetIntegratedSystemStatus());
    }

    LogMessage(Result.Message, EConsoleOutputLevel::Info, TEXT("Status"));
    return Result;
}

FConsoleCommandResult UMingSageBrainAPIConsole::ProcessGenerateCommand(const FConsoleCommand& Command)
{
    FConsoleCommandResult Result;

    if (!IntegratedAPI)
    {
        Result.bSuccess = false;
        Result.Message = TEXT("集成API未初始化");
        return Result;
    }

    if (Command.Arguments.Num() < 2)
    {
        Result.bSuccess = false;
        Result.Message = TEXT("用法: generate <type> <prompt> [--duration=30] [--style=orchestral]");
        LogMessage(Result.Message, EConsoleOutputLevel::Warning);
        return Result;
    }

    FString Type = Command.Arguments[0].ToLower();
    FString Prompt = Command.Arguments[1];

    FString GeneratedPath;
    bool bSuccess = false;

    if (Type == TEXT("music"))
    {
        int32 Duration = 30;
        if (const FString* DurValue = Command.Parameters.Find(TEXT("duration")))
        {
            Duration = FCString::Atoi(**DurValue);
        }

        GeneratedPath = IntegratedAPI->GenerateSageBrainMusic(Prompt, Duration);
        bSuccess = !GeneratedPath.IsEmpty();
    }
    else if (Type == TEXT("asset"))
    {
        GeneratedPath = IntegratedAPI->GenerateSageBrainScene(Prompt);
        bSuccess = !GeneratedPath.IsEmpty();
    }
    else if (Type == TEXT("character"))
    {
        GeneratedPath = IntegratedAPI->GenerateSageBrainCharacter(Prompt);
        bSuccess = !GeneratedPath.IsEmpty();
    }
    else
    {
        Result.bSuccess = false;
        Result.Message = FString::Printf(TEXT("未知生成類型: %s (可用: music, asset, character)"), *Type);
        LogMessage(Result.Message, EConsoleOutputLevel::Error);
        return Result;
    }

    Result.bSuccess = bSuccess;
    Result.Message = bSuccess ? 
        FString::Printf(TEXT("生成成功: %s"), *GeneratedPath) : 
        TEXT("生成失敗");
    Result.Data.Add(TEXT("Path"), GeneratedPath);

    LogMessage(Result.Message, bSuccess ? EConsoleOutputLevel::Success : EConsoleOutputLevel::Error, TEXT("Generate"));
    return Result;
}

FConsoleCommandResult UMingSageBrainAPIConsole::ProcessQueryCommand(const FConsoleCommand& Command)
{
    FConsoleCommandResult Result;
    Result.bSuccess = true;

    FString QueryType = Command.Arguments.Num() > 0 ? Command.Arguments[0].ToLower() : TEXT("status");

    if (QueryType == TEXT("commands"))
    {
        TMap<FString, FString> Commands = GetAvailableCommands();
        for (const auto& Pair : Commands)
        {
            Result.Message += FString::Printf(TEXT("  %s - %s\n"), *Pair.Key, *Pair.Value);
        }
    }
    else if (QueryType == TEXT("shortcuts"))
    {
        for (const auto& Pair : Shortcuts)
        {
            Result.Message += FString::Printf(TEXT("  %s -> %s\n"), *Pair.Key, *Pair.Value);
        }
    }
    else if (QueryType == TEXT("config"))
    {
        for (const auto& Pair : ConfigValues)
        {
            Result.Message += FString::Printf(TEXT("  %s = %s\n"), *Pair.Key, *Pair.Value);
        }
    }
    else
    {
        Result.Message = GetConsoleStatus();
    }

    LogMessage(Result.Message, EConsoleOutputLevel::Info, TEXT("Query"));
    return Result;
}

FConsoleCommandResult UMingSageBrainAPIConsole::ProcessConfigCommand(const FConsoleCommand& Command)
{
    FConsoleCommandResult Result;

    if (Command.Arguments.Num() < 1)
    {
        Result.bSuccess = false;
        Result.Message = TEXT("用法: config <action> [key] [value]");
        LogMessage(Result.Message, EConsoleOutputLevel::Warning);
        return Result;
    }

    FString Action = Command.Arguments[0].ToLower();

    if (Action == TEXT("set") && Command.Arguments.Num() >= 3)
    {
        SetConfigValue(Command.Arguments[1], Command.Arguments[2]);
        Result.bSuccess = true;
        Result.Message = TEXT("配置已更新");
    }
    else if (Action == TEXT("get") && Command.Arguments.Num() >= 2)
    {
        FString Value = GetConfigValue(Command.Arguments[1]);
        Result.bSuccess = true;
        Result.Message = FString::Printf(TEXT("%s = %s"), *Command.Arguments[1], *Value);
    }
    else if (Action == TEXT("load") && Command.Arguments.Num() >= 2)
    {
        LoadConfig(Command.Arguments[1]);
        Result.bSuccess = true;
        Result.Message = TEXT("配置已加載");
    }
    else if (Action == TEXT("save") && Command.Arguments.Num() >= 2)
    {
        SaveConfig(Command.Arguments[1]);
        Result.bSuccess = true;
        Result.Message = TEXT("配置已保存");
    }
    else
    {
        Result.bSuccess = false;
        Result.Message = TEXT("無效的配置操作");
    }

    LogMessage(Result.Message, Result.bSuccess ? EConsoleOutputLevel::Success : EConsoleOutputLevel::Error, TEXT("Config"));
    return Result;
}

FConsoleCommandResult UMingSageBrainAPIConsole::ProcessTestCommand(const FConsoleCommand& Command)
{
    FConsoleCommandResult Result;

    if (!IntegratedAPI)
    {
        Result.bSuccess = false;
        Result.Message = TEXT("集成API未初始化");
        return Result;
    }

    FString TestType = Command.Arguments.Num() > 0 ? Command.Arguments[0].ToLower() : TEXT("all");
    bool bSuccess = false;

    if (TestType == TEXT("system"))
    {
        bSuccess = IntegratedAPI->RunSageBrainSystemTests();
    }
    else if (TestType == TEXT("skills"))
    {
        bSuccess = IntegratedAPI->RunSageBrainSkillTests();
    }
    else if (TestType == TEXT("all"))
    {
        bSuccess = IntegratedAPI->RunCompleteSageBrainTestSuite();
    }
    else
    {
        Result.bSuccess = false;
        Result.Message = FString::Printf(TEXT("未知測試類型: %s"), *TestType);
        LogMessage(Result.Message, EConsoleOutputLevel::Warning);
        return Result;
    }

    Result.bSuccess = bSuccess;
    Result.Message = bSuccess ? TEXT("測試通過") : TEXT("測試失敗");

    LogMessage(Result.Message, bSuccess ? EConsoleOutputLevel::Success : EConsoleOutputLevel::Error, TEXT("Test"));
    return Result;
}

FConsoleCommandResult UMingSageBrainAPIConsole::ProcessExportCommand(const FConsoleCommand& Command)
{
    FConsoleCommandResult Result;

    if (Command.Arguments.Num() < 2)
    {
        Result.bSuccess = false;
        Result.Message = TEXT("用法: export <type> <path>");
        LogMessage(Result.Message, EConsoleOutputLevel::Warning);
        return Result;
    }

    FString Type = Command.Arguments[0].ToLower();
    FString Path = Command.Arguments[1];

    bool bSuccess = false;

    if (Type == TEXT("history"))
    {
        SaveCommandHistoryToScript(Path);
        bSuccess = true;
    }
    else if (Type == TEXT("config"))
    {
        SaveConfig(Path);
        bSuccess = true;
    }
    else
    {
        Result.bSuccess = false;
        Result.Message = FString::Printf(TEXT("未知導出類型: %s"), *Type);
        LogMessage(Result.Message, EConsoleOutputLevel::Error);
        return Result;
    }

    Result.bSuccess = bSuccess;
    Result.Message = FString::Printf(TEXT("已導出到: %s"), *Path);

    LogMessage(Result.Message, EConsoleOutputLevel::Success, TEXT("Export"));
    return Result;
}

FConsoleCommandResult UMingSageBrainAPIConsole::ProcessImportCommand(const FConsoleCommand& Command)
{
    FConsoleCommandResult Result;

    if (Command.Arguments.Num() < 2)
    {
        Result.bSuccess = false;
        Result.Message = TEXT("用法: import <type> <path>");
        LogMessage(Result.Message, EConsoleOutputLevel::Warning);
        return Result;
    }

    FString Type = Command.Arguments[0].ToLower();
    FString Path = Command.Arguments[1];

    bool bSuccess = false;

    if (Type == TEXT("script"))
    {
        LoadAndExecuteScript(Path);
        bSuccess = true;
    }
    else if (Type == TEXT("config"))
    {
        LoadConfig(Path);
        bSuccess = true;
    }
    else
    {
        Result.bSuccess = false;
        Result.Message = FString::Printf(TEXT("未知導入類型: %s"), *Type);
        LogMessage(Result.Message, EConsoleOutputLevel::Error);
        return Result;
    }

    Result.bSuccess = bSuccess;
    Result.Message = FString::Printf(TEXT("已從 %s 導入"), *Path);

    LogMessage(Result.Message, EConsoleOutputLevel::Success, TEXT("Import"));
    return Result;
}

FConsoleCommandResult UMingSageBrainAPIConsole::ProcessClearCommand(const FConsoleCommand& Command)
{
    FConsoleCommandResult Result;
    Result.bSuccess = true;

    FString Type = Command.Arguments.Num() > 0 ? Command.Arguments[0].ToLower() : TEXT("output");

    if (Type == TEXT("output"))
    {
        ClearOutput();
        Result.Message = TEXT("輸出已清除");
    }
    else if (Type == TEXT("history"))
    {
        ClearHistory();
        Result.Message = TEXT("歷史已清除");
    }
    else if (Type == TEXT("all"))
    {
        ClearOutput();
        ClearHistory();
        Result.Message = TEXT("輸出和歷史已清除");
    }
    else
    {
        Result.bSuccess = false;
        Result.Message = TEXT("用法: clear [output|history|all]");
    }

    LogMessage(Result.Message, EConsoleOutputLevel::Info, TEXT("Clear"));
    return Result;
}

// 輔助函數
void UMingSageBrainAPIConsole::InitializeDefaultShortcuts()
{
    RegisterShortcut(TEXT("gen-music"), TEXT("generate music \"$1\" --duration=$2 --style=$3"));
    RegisterShortcut(TEXT("gen-asset"), TEXT("generate asset \"$1\""));
    RegisterShortcut(TEXT("test-all"), TEXT("test all"));
    RegisterShortcut(TEXT("status-quick"), TEXT("status"));
    RegisterShortcut(TEXT("help-gen"), TEXT("help generate"));
}

void UMingSageBrainAPIConsole::InitializeDefaultConfig()
{
    ConfigValues.Add(TEXT("output.format"), TEXT("wav"));
    ConfigValues.Add(TEXT("output.quality"), TEXT("high"));
    ConfigValues.Add(TEXT("ai.provider"), TEXT("aiva"));
    ConfigValues.Add(TEXT("ai.timeout"), TEXT("30"));
    ConfigValues.Add(TEXT("console.max_history"), TEXT("100"));
    ConfigValues.Add(TEXT("console.auto_save"), TEXT("true"));
}

FString UMingSageBrainAPIConsole::GetHelpText() const
{
    FString Help = TEXT("=== Sage Brain API Console 幫助 ===\n\n");
    Help += TEXT("基本命令:\n");
    Help += TEXT("  help [command]       - 顯示幫助信息\n");
    Help += TEXT("  status               - 顯示系統狀態\n");
    Help += TEXT("  generate <type> <prompt> [--param=value]\n");
    Help += TEXT("                       - 生成內容 (music, asset, character)\n");
    Help += TEXT("  query <type>         - 查詢信息 (commands, shortcuts, config)\n");
    Help += TEXT("  config <action>      - 配置管理 (set, get, load, save)\n");
    Help += TEXT("  test [type]          - 運行測試 (system, skills, all)\n");
    Help += TEXT("  export <type> <path> - 導出數據\n");
    Help += TEXT("  import <type> <path> - 導入數據\n");
    Help += TEXT("  clear [type]         - 清除輸出/歷史\n\n");
    Help += TEXT("快捷命令:\n");
    for (const auto& Pair : Shortcuts)
    {
        Help += FString::Printf(TEXT("  %s -> %s\n"), *Pair.Key, *Pair.Value);
    }
    Help += TEXT("\n提示: 使用 Tab 鍵自動補全命令");
    return Help;
}

FString UMingSageBrainAPIConsole::GetCommandHelp(const FString& CommandName) const
{
    if (CommandName == TEXT("generate"))
    {
        return TEXT("generate <type> <prompt> [options]\n\n")
               TEXT("類型:\n")
               TEXT("  music    - 生成音樂\n")
               TEXT("  asset    - 生成場景資產\n")
               TEXT("  character- 生成角色\n\n")
               TEXT("選項:\n")
               TEXT("  --duration=<秒>  - 音樂時長 (默認30)\n")
               TEXT("  --style=<風格>   - 音樂風格\n")
               TEXT("  --key=<調>       - 音樂調性\n\n")
               TEXT("示例:\n")
               TEXT("  generate music \"epic battle theme\" --duration=60 --style=orchestral");
    }
    else if (CommandName == TEXT("config"))
    {
        return TEXT("config <action> [args]\n\n")
               TEXT("操作:\n")
               TEXT("  set <key> <value>  - 設置配置值\n")
               TEXT("  get <key>          - 獲取配置值\n")
               TEXT("  load <path>        - 從文件加載配置\n")
               TEXT("  save <path>        - 保存配置到文件\n\n")
               TEXT("示例:\n")
               TEXT("  config set ai.provider soundraw\n")
               TEXT("  config get output.quality\n")
               TEXT("  config save MyConfig.json");
    }
    return FString::Printf(TEXT("未知命令: %s"), *CommandName);
}

void UMingSageBrainAPIConsole::BroadcastOutput(const FString& Message, EConsoleOutputLevel Level, const FString& Category)
{
    FConsoleOutputItem Item;
    Item.Message = Message;
    Item.Level = Level;
    Item.Category = Category;
    Item.Timestamp = FDateTime::Now();
    OnConsoleOutput.Broadcast(Item);
}

FString UMingSageBrainAPIConsole::ReplaceParameters(const FString& Template, const TMap<FString, FString>& Parameters)
{
    FString Result = Template;
    for (const auto& Pair : Parameters)
    {
        FString Placeholder = FString::Printf(TEXT("$%s"), *Pair.Key);
        Result = Result.Replace(*Placeholder, *Pair.Value);
    }
    return Result;
}

FString UMingSageBrainAPIConsole::FormatExecutionTime(float Seconds) const
{
    if (Seconds < 1.0f)
    {
        return FString::Printf(TEXT("%.0fms"), Seconds * 1000.0f);
    }
    return FString::Printf(TEXT("%.2fs"), Seconds);
}
