// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 弱�?修復工具 - ?��?修復常�X��?

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaknessFixer.generated.h"

/**
 * 修復類�X��?
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
 * 修復?��?級�X */
UENUM(BlueprintType)
enum class EFixPriority : uint8
{
    Critical	UMETA(DisplayName = "Critical"),
    High		UMETA(DisplayName = "High"),
    Medium		UMETA(DisplayName = "Medium"),
    Low		UMETA(DisplayName = "Low")
};

/**
 * 修復結�?
 */
USTRUCT(BlueprintType)
struct FFixResult
{
    GENERATED_BODY()

    /** 修復?�否?��? */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    bool bSuccess;

    /** 修復類�? */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    EFixType FixType;

    /** 修復?��?�?*/
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    EFixPriority Priority;

    /** 修復?�述 */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    FString Description;

    /** 修復詳�? */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    FString Details;

    /** 修復?��?�?*/
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    FString FixedFile;

    /** 修復?��? */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    FDateTime FixTime;

    /** ?�誤信息 */
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
 * 弱�?修復X */
UCLASS(BlueprintType, Blueprintable)
class WEAKNESSFIXER_API UWeaknessFixer : public UObject
{
    GENERATED_BODY()

public:
    UWeaknessFixer();

    /** ?��X�修復器 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    void InitializeFixer();

    /** ?��?完整修復 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    TArray<FFixResult> RunFullFix();

    /** 修復?��?洩�? */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixMemoryLeaks();

    /** 修復線�?安全?��? */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixThreadSafety();

    /** 修復?�能?��? */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixPerformanceIssues();

    /** 修復安全?��? */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixSecurityIssues();

    /** 修復�?��質�X��? */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixCodeQuality();

    /** 修復?��X�件 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixSpecificFile(const FString& FilePath};

    /** ?��?修復歷史 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    TArray<FFixResult> GetFixHistory();

    /** ?��?修復?��? */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FString GenerateFixReport();

    /** 驗�?修復?��? */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    bool VerifyFixes();

public:
    /** 修復?��?事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFixStarted, EFixType, FixType};

    /** 修復完�?事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFixCompleted, const FFixResult&, FixResult};

    /** 修復失�?事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFixFailed, const FFixResult&, FixResult};

    UPROPERTY(BlueprintAssignable, Category = "Weakness Fixer|Events")
    FOnFixStarted OnFixStarted;

    UPROPERTY(BlueprintAssignable, Category = "Weakness Fixer|Events")
    FOnFixCompleted OnFixCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Weakness Fixer|Events")
    FOnFixFailed OnFixFailed;

protected:
    /** ?��?�?��弱�? */
    TArray<FString> AnalyzeWeaknesses(const FString& FilePath};

    /** ?�用修復 */
    bool ApplyFix(const FString& FilePath, const FString& Weakness, const FString& Fix};

    /** ?�份?��?�?*/
    bool BackupFile(const FString& FilePath};

    /** 驗�?修復 */
    bool ValidateFix(const FString& FilePath, const FString& OriginalContent, const FString& FixedContent};

    /** 記�?修復 */
    void RecordFix(const FFixResult& FixResult};

private:
    /** 修復歷史 */
    UPROPERTY(BlueprintReadOnly, Category = "Weakness Fixer", meta = (AllowPrivateAccess = "true"))
    TArray<FFixResult> FixHistory;

    /** 系統?�否已�?始�? */
    UPROPERTY(BlueprintReadOnly, Category = "Weakness Fixer", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;

    /** ?�份?��? */
    UPROPERTY(BlueprintReadOnly, Category = "Weakness Fixer", meta = (AllowPrivateAccess = "true"))
    FString BackupDirectory;

    /** 修復規�X��? */
    UPROPERTY(BlueprintReadOnly, Category = "Weakness Fixer", meta = (AllowPrivateAccess = "true"))
    TMap<FString, FString> FixRules;
};

