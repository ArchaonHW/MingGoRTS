#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Containers/Queue.h"
#include "MingSageBrainIntegratedAPI.h"
#include "MingSageBrainAPIConsole.generated.h"

// 控制台命令類型
UENUM(BlueprintType)
enum class ESageBrainConsoleCommandType : uint8
{
    Help           UMETA(DisplayName = "幫助"),
    Status         UMETA(DisplayName = "狀態"),
    Generate       UMETA(DisplayName = "生成"),
    Query          UMETA(DisplayName = "查詢"),
    Config         UMETA(DisplayName = "配置"),
    Test           UMETA(DisplayName = "測試"),
    Export         UMETA(DisplayName = "導出"),
    Import         UMETA(DisplayName = "導入"),
    Clear          UMETA(DisplayName = "清除"),
    Exit           UMETA(DisplayName = "退出"),
    Custom         UMETA(DisplayName = "自定義")
};

// 控制台輸出級別
UENUM(BlueprintType)
enum class EConsoleOutputLevel : uint8
{
    Debug     UMETA(DisplayName = "調試"),
    Info      UMETA(DisplayName = "信息"),
    Warning   UMETA(DisplayName = "警告"),
    Error     UMETA(DisplayName = "錯誤"),
    Success   UMETA(DisplayName = "成功")
};

// 命令執行結果
USTRUCT(BlueprintType)
struct FConsoleCommandResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Console")
    bool bSuccess = false;

    UPROPERTY(BlueprintReadOnly, Category = "Console")
    FString Message;

    UPROPERTY(BlueprintReadOnly, Category = "Console")
    TMap<FString, FString> Data;

    UPROPERTY(BlueprintReadOnly, Category = "Console")
    float ExecutionTime = 0.0f;
};

// 控制台命令結構
USTRUCT(BlueprintType)
struct FConsoleCommand
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    ESageBrainConsoleCommandType Type = ESageBrainConsoleCommandType::Help;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    FString CommandString;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    TArray<FString> Arguments;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    TMap<FString, FString> Parameters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    FString RawInput;
};

// 控制台輸出項目
USTRUCT(BlueprintType)
struct FConsoleOutputItem
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Output")
    EConsoleOutputLevel Level = EConsoleOutputLevel::Info;

    UPROPERTY(BlueprintReadOnly, Category = "Output")
    FString Message;

    UPROPERTY(BlueprintReadOnly, Category = "Output")
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly, Category = "Output")
    FString Category;
};

// 控制台歷史記錄
USTRUCT(BlueprintType)
struct FConsoleHistoryEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "History")
    FConsoleCommand Command;

    UPROPERTY(BlueprintReadOnly, Category = "History")
    FConsoleCommandResult Result;

    UPROPERTY(BlueprintReadOnly, Category = "History")
    FDateTime ExecutionTime;
};

// 輸出日誌委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConsoleOutput, const FConsoleOutputItem&, OutputItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCommandExecuted, const FConsoleCommand&, Command, const FConsoleCommandResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConsoleStatusChanged, const FString&, NewStatus);

/**
 * 聖者大腦 API 控制台系統
 * 提供專業的命令行界面來操作所有 Sage Brain API 功能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingSageBrainAPIConsole : public UObject
{
    GENERATED_BODY()

public:
    UMingSageBrainAPIConsole();

    // 初始化控制台
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console")
    bool InitializeConsole();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console")
    void ShutdownConsole();

    // 命令執行
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Commands")
    FConsoleCommandResult ExecuteCommand(const FString& CommandLine);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Commands")
    FConsoleCommandResult ExecuteCommandStruct(const FConsoleCommand& Command);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Commands")
    void ExecuteCommandAsync(const FString& CommandLine);

    // 命令解析
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Parsing")
    FConsoleCommand ParseCommand(const FString& CommandLine) const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Parsing")
    TArray<FString> GetCommandSuggestions(const FString& PartialInput) const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Parsing")
    bool ValidateCommand(const FString& CommandLine) const;

    // 輸出管理
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Output")
    void LogMessage(const FString& Message, EConsoleOutputLevel Level = EConsoleOutputLevel::Info, const FString& Category = TEXT("General"));

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Output")
    void ClearOutput();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Output")
    TArray<FConsoleOutputItem> GetOutputHistory(int32 MaxItems = 100) const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Output")
    FString GetFormattedOutput() const;

    // 歷史管理
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|History")
    void AddToHistory(const FConsoleCommand& Command, const FConsoleCommandResult& Result);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|History")
    TArray<FConsoleHistoryEntry> GetCommandHistory(int32 MaxEntries = 50) const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|History")
    void ClearHistory();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|History")
    FString GetPreviousCommand();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|History")
    FString GetNextCommand();

    // 快捷命令
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Shortcuts")
    void RegisterShortcut(const FString& ShortcutName, const FString& CommandTemplate);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Shortcuts")
    void RemoveShortcut(const FString& ShortcutName);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Shortcuts")
    TMap<FString, FString> GetAllShortcuts() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Shortcuts")
    FString ExecuteShortcut(const FString& ShortcutName, const TMap<FString, FString>& Parameters);

    // 批處理
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Batch")
    void ExecuteBatchCommands(const TArray<FString>& Commands);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Batch")
    void LoadAndExecuteScript(const FString& ScriptPath);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Batch")
    void SaveCommandHistoryToScript(const FString& ScriptPath);

    // 系統狀態
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Status")
    FString GetConsoleStatus() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Status")
    bool IsInitialized() const { return bIsInitialized; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Status")
    FString GetSystemInfo() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Status")
    TMap<FString, FString> GetAvailableCommands() const;

    // 配置管理
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Config")
    void SetConfigValue(const FString& Key, const FString& Value);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Config")
    FString GetConfigValue(const FString& Key) const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Config")
    void LoadConfig(const FString& ConfigPath);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain Console|Config")
    void SaveConfig(const FString& ConfigPath);

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Console|Events")
    FOnConsoleOutput OnConsoleOutput;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Console|Events")
    FOnCommandExecuted OnCommandExecuted;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain Console|Events")
    FOnConsoleStatusChanged OnConsoleStatusChanged;

private:
    // 核心系統引用
    UPROPERTY()
    TObjectPtr<UMingSageBrainIntegratedAPI> IntegratedAPI;

    // 狀態
    bool bIsInitialized;
    int32 CurrentHistoryIndex;

    // 數據存儲
    TArray<FConsoleOutputItem> OutputHistory;
    TArray<FConsoleHistoryEntry> CommandHistory;
    TQueue<FString> CommandQueue;
    TMap<FString, FString> Shortcuts;
    TMap<FString, FString> ConfigValues;

    // 命令處理器
    FConsoleCommandResult ProcessHelpCommand(const FConsoleCommand& Command);
    FConsoleCommandResult ProcessStatusCommand(const FConsoleCommand& Command);
    FConsoleCommandResult ProcessGenerateCommand(const FConsoleCommand& Command);
    FConsoleCommandResult ProcessQueryCommand(const FConsoleCommand& Command);
    FConsoleCommandResult ProcessConfigCommand(const FConsoleCommand& Command);
    FConsoleCommandResult ProcessTestCommand(const FConsoleCommand& Command);
    FConsoleCommandResult ProcessExportCommand(const FConsoleCommand& Command);
    FConsoleCommandResult ProcessImportCommand(const FConsoleCommand& Command);
    FConsoleCommandResult ProcessClearCommand(const FConsoleCommand& Command);

    // 輔助函數
    void InitializeDefaultShortcuts();
    void InitializeDefaultConfig();
    FString GetHelpText() const;
    FString GetCommandHelp(const FString& CommandName) const;
    void BroadcastOutput(const FString& Message, EConsoleOutputLevel Level, const FString& Category = TEXT("General"));
    FString ReplaceParameters(const FString& Template, const TMap<FString, FString>& Parameters);
    FString FormatExecutionTime(float Seconds) const;
};
