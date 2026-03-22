#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRTSErrorHandlingSystem.generated.h"

UENUM(BlueprintType)
enum class EErrorSeverity: uint8 {
    Info,               // 信息
    Warning,            // 警告
    Error,              // 錯誤
    Critical,           // 嚴重錯誤
    Fatal               // 致命錯誤
};

UENUM(BlueprintType)
enum class EErrorCategory: uint8 {
    System,             // 系統錯誤
    Network,            // 網絡錯誤
    Database,           // 數據庫錯誤
    FileIO,             // 文件I/O錯誤
    Memory,             // 內存錯誤
    Performance,        // 性能錯誤
    Security,           // 安全錯誤
    UserInterface,      // 用戶界面錯誤
    GameLogic,          // 遊戲邏輯錯誤
    Audio,              // 音頻錯誤
    Rendering,          // 渲染錯誤
    AI,                 // AI錯誤
    Input,              // 輸入錯誤
    SaveLoad,           // 存檔載入錯誤
    Custom              // 自定義錯誤
};

USTRUCT(BlueprintType)
struct FErrorInfo
{
    GENERATED_BODY()

    UPROPERTY()
    FString ErrorID;

    UPROPERTY()
    FString ErrorMessage;

    UPROPERTY()
    EErrorSeverity Severity;

    UPROPERTY()
    EErrorCategory Category;

    UPROPERTY()
    FString SystemName;

    UPROPERTY()
    FString FunctionName;

    UPROPERTY()
    int32 LineNumber;

    UPROPERTY()
    FString StackTrace;

    UPROPERTY()
    FDateTime Timestamp;

    UPROPERTY()
    TMap<FString, FString> Context;

    UPROPERTY()
    bool bIsResolved;

    UPROPERTY()
    FDateTime ResolvedTimestamp;

    UPROPERTY()
    FString ResolutionNote;

    FErrorInfo()
        : Severity(EErrorSeverity::Info)
        , Category(EErrorCategory::Custom)
        , LineNumber(0)
        , Timestamp(FDateTime::Now())
        , bIsResolved(false)
    {}
};

USTRUCT(BlueprintType)
struct FLogEntry
{
    GENERATED_BODY()

    UPROPERTY()
    FString Message;

    UPROPERTY()
    EErrorSeverity Severity;

    UPROPERTY()
    FString Category;

    UPROPERTY()
    FString SystemName;

    UPROPERTY()
    FDateTime Timestamp;

    UPROPERTY()
    TMap<FString, FString> Metadata;

    FLogEntry()
        : Severity(EErrorSeverity::Info)
        , Timestamp(FDateTime::Now())
    {}
};

/**
 * 統一的錯誤處理和日誌系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSErrorHandlingSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSErrorHandlingSystem();

    // 錯誤處理
    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    FString ReportError(const FString& ErrorMessage, EErrorSeverity Severity, EErrorCategory Category, const FString& SystemName = TEXT(""));

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    FString ReportErrorWithContext(const FString& ErrorMessage, EErrorSeverity Severity, EErrorCategory Category, const FString& SystemName, const TMap<FString, FString>& Context);

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    bool ResolveError(const FString& ErrorID, const FString& ResolutionNote);

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    void MarkErrorAsUnresolved(const FString& ErrorID);

    // 日誌記錄
    UFUNCTION(BlueprintCallable, Category = "Logging")
    void LogInfo(const FString& Message, const FString& Category = TEXT("General"), const FString& SystemName = TEXT(""));

    UFUNCTION(BlueprintCallable, Category = "Logging")
    void LogWarning(const FString& Message, const FString& Category = TEXT("General"), const FString& SystemName = TEXT(""));

    UFUNCTION(BlueprintCallable, Category = "Logging")
    void LogError(const FString& Message, const FString& Category = TEXT("General"), const FString& SystemName = TEXT(""));

    UFUNCTION(BlueprintCallable, Category = "Logging")
    void LogCritical(const FString& Message, const FString& Category = TEXT("General"), const FString& SystemName = TEXT(""));

    UFUNCTION(BlueprintCallable, Category = "Logging")
    void LogCustom(const FString& Message, EErrorSeverity Severity, const FString& Category, const FString& SystemName, const TMap<FString, FString>& Metadata);

    // 查詢功能
    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    TArray<FErrorInfo> GetErrors(EErrorSeverity MinSeverity = EErrorSeverity::Warning, int32 MaxCount = 100) const;

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    TArray<FErrorInfo> GetSystemErrors(const FString& SystemName, EErrorSeverity MinSeverity = EErrorSeverity::Warning) const;

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    TArray<FErrorInfo> GetUnresolvedErrors() const;

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    FErrorInfo GetError(const FString& ErrorID) const;

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    int32 GetErrorCount(EErrorSeverity MinSeverity = EErrorSeverity::Warning) const;

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    int32 GetSystemErrorCount(const FString& SystemName, EErrorSeverity MinSeverity = EErrorSeverity::Warning) const;

    // 日誌查詢
    UFUNCTION(BlueprintCallable, Category = "Logging")
    TArray<FLogEntry> GetLogs(const FDateTime& StartTime, const FDateTime& EndTime, EErrorSeverity MinSeverity = EErrorSeverity::Info) const;

    UFUNCTION(BlueprintCallable, Category = "Logging")
    TArray<FLogEntry> GetSystemLogs(const FString& SystemName, const FDateTime& StartTime, const FDateTime& EndTime) const;

    UFUNCTION(BlueprintCallable, Category = "Logging")
    TArray<FLogEntry> GetCategoryLogs(const FString& Category, const FDateTime& StartTime, const FDateTime& EndTime) const;

    // 報告生成
    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    FString GenerateErrorReport(const FDateTime& StartTime, const FDateTime& EndTime) const;

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    FString GenerateSystemErrorReport(const FString& SystemName) const;

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    bool SaveErrorReport(const FString& FilePath, const FDateTime& StartTime, const FDateTime& EndTime) const;

    UFUNCTION(BlueprintCallable, Category = "Logging")
    bool SaveLogReport(const FString& FilePath, const FDateTime& StartTime, const FDateTime& EndTime) const;

    // 配置管理
    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    void SetMaxLogEntries(int32 MaxEntries);

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    void SetMaxErrorHistory(int32 MaxErrors);

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    void EnableAutoSave(bool bEnabled, const FString& FilePath = TEXT(""));

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    void SetLogLevel(EErrorSeverity MinLogLevel);

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    void EnableSystemLogging(const FString& SystemName, bool bEnabled);

    // 清理功能
    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    void ClearOldLogs(const FDateTime& BeforeTime);

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    void ClearResolvedErrors();

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    void ClearAllErrors();

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    void ClearAllLogs();

    // 統計功能
    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    TMap<EErrorCategory, int32> GetErrorStatistics() const;

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    TMap<FString, int32> GetSystemErrorStatistics() const;

    UFUNCTION(BlueprintCallable, Category = "Error Handling")
    TArray<FString> GetTopErrorMessages(int32 TopCount = 10) const;

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnErrorReported, const FErrorInfo&, ErrorInfo);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnErrorResolved, const FString&, ErrorID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLogEntry, const FString&, Message, EErrorSeverity, Severity);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCriticalError, const FErrorInfo&, ErrorInfo);

    UPROPERTY(BlueprintAssignable, Category = "Error Handling")
    FOnErrorReported OnErrorReported;

    UPROPERTY(BlueprintAssignable, Category = "Error Handling")
    FOnErrorResolved OnErrorResolved;

    UPROPERTY(BlueprintAssignable, Category = "Error Handling")
    FOnLogEntry OnLogEntry;

    UPROPERTY(BlueprintAssignable, Category = "Error Handling")
    FOnCriticalError OnCriticalError;

protected:
    virtual void BeginDestroy() override;

    // 內部方法
    FString GenerateErrorID() const;
    void AddLogEntry(const FLogEntry& LogEntry);
    void AddErrorInfo(const FErrorInfo& ErrorInfo);
    void AutoSaveIfNeeded();
    void ProcessCriticalError(const FErrorInfo& ErrorInfo);

private:
    // 錯誤存儲
    UPROPERTY()
    TArray<FErrorInfo> ErrorHistory;

    UPROPERTY()
    TArray<FLogEntry> LogHistory;

    // 配置
    UPROPERTY()
    int32 MaxLogEntries;

    UPROPERTY()
    int32 MaxErrorHistorySize;

    UPROPERTY()
    EErrorSeverity MinLogLevel;

    UPROPERTY()
    bool bAutoSaveEnabled;

    UPROPERTY()
    FString AutoSaveFilePath;

    UPROPERTY()
    TMap<FString, bool> SystemLoggingEnabled;

    // 統計
    UPROPERTY()
    TMap<EErrorCategory, int32> ErrorStatistics;

    UPROPERTY()
    TMap<FString, int32> SystemErrorStatistics;

    // 內部計數器
    UPROPERTY()
    int32 ErrorCounter;

    UPROPERTY()
    FDateTime LastAutoSave;
};
