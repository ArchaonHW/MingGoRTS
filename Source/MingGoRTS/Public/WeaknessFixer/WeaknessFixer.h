// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 弱點修復工具 - 自動修復常見問題

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaknessFixer.generated.h"

/**
 * 修復類型枚舉
 */
UENUM(BlueprintType)
enum class EFixType : uint8
{
    MemoryLeak		UMETA(DisplayName = "Memory Leak Fix"),
    ThreadSafety	UMETA(DisplayName = "Thread Safety Fix"),
    Performance		UMETA(DisplayName = "Performance Fix"),
    Security		UMETA(DisplayName = "Security Fix"),
    CodeQuality		UMETA(DisplayName = "Code Quality Fix")
};

/**
 * 修復優先級枚舉
 */
UENUM(BlueprintType)
enum class EFixPriority : uint8
{
    Critical	UMETA(DisplayName = "Critical"),
    High		UMETA(DisplayName = "High"),
    Medium		UMETA(DisplayName = "Medium"),
    Low		UMETA(DisplayName = "Low")
};

/**
 * 修復結果
 */
USTRUCT(BlueprintType)
struct FFixResult
{
    GENERATED_BODY()

    /** 修復是否成功 */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    bool bSuccess;

    /** 修復類型 */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    EFixType FixType;

    /** 修復優先級 */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    EFixPriority Priority;

    /** 修復描述 */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    FString Description;

    /** 修復詳情 */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    FString Details;

    /** 修復的文件 */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    FString FixedFile;

    /** 修復時間 */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    FDateTime FixTime;

    /** 錯誤信息 */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    FString ErrorMessage;

    FFixResult()
        : bSuccess(false)
        , FixType(EFixType::CodeQuality)
        , Priority(EFixPriority::Low)
        , Description(TEXT(""))
        , Details(TEXT(""))
        , FixedFile(TEXT(""))
        , FixTime(FDateTime::Now())
        , ErrorMessage(TEXT(""))
    {
    }
};

/**
 * 弱點修復器
 */
UCLASS(BlueprintType, Blueprintable)
class WEAKNESSFIXER_API UWeaknessFixer : public UObject
{
    GENERATED_BODY()

public:
    UWeaknessFixer();

    /** 初始化修復器 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    void InitializeFixer();

    /** 運行完整修復 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    TArray<FFixResult> RunFullFix();

    /** 修復內存洩漏 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixMemoryLeaks();

    /** 修復線程安全問題 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixThreadSafety();

    /** 修復性能問題 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixPerformanceIssues();

    /** 修復安全問題 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixSecurityIssues();

    /** 修復代碼質量問題 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixCodeQuality();

    /** 修復特定文件 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixSpecificFile(const FString& FilePath);

    /** 獲取修復歷史 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    TArray<FFixResult> GetFixHistory();

    /** 生成修復報告 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FString GenerateFixReport();

    /** 驗證修復效果 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    bool VerifyFixes();

public:
    /** 修復開始事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFixStarted, EFixType, FixType);

    /** 修復完成事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFixCompleted, const FFixResult&, FixResult);

    /** 修復失敗事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFixFailed, const FFixResult&, FixResult);

    UPROPERTY(BlueprintAssignable, Category = "Weakness Fixer|Events")
    FOnFixStarted OnFixStarted;

    UPROPERTY(BlueprintAssignable, Category = "Weakness Fixer|Events")
    FOnFixCompleted OnFixCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Weakness Fixer|Events")
    FOnFixFailed OnFixFailed;

protected:
    /** 分析代碼弱點 */
    TArray<FString> AnalyzeWeaknesses(const FString& FilePath);

    /** 應用修復 */
    bool ApplyFix(const FString& FilePath, const FString& Weakness, const FString& Fix);

    /** 備份原文件 */
    bool BackupFile(const FString& FilePath);

    /** 驗證修復 */
    bool ValidateFix(const FString& FilePath, const FString& OriginalContent, const FString& FixedContent);

    /** 記錄修復 */
    void RecordFix(const FFixResult& FixResult);

private:
    /** 修復歷史 */
    UPROPERTY(BlueprintReadOnly, Category = "Weakness Fixer", meta = (AllowPrivateAccess = "true"))
    TArray<FFixResult> FixHistory;

    /** 系統是否已初始化 */
    UPROPERTY(BlueprintReadOnly, Category = "Weakness Fixer", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;

    /** 備份目錄 */
    UPROPERTY(BlueprintReadOnly, Category = "Weakness Fixer", meta = (AllowPrivateAccess = "true"))
    FString BackupDirectory;

    /** 修復規則映射 */
    UPROPERTY(BlueprintReadOnly, Category = "Weakness Fixer", meta = (AllowPrivateAccess = "true"))
    TMap<FString, FString> FixRules;
};
