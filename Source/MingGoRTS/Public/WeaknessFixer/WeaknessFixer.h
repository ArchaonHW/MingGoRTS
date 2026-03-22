// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 寮遍?淇京宸ュ叿 - ??淇京甯歌??忛?

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaknessFixer.generated.h"

/**
 * 淇京椤炲??氳?
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
 * 淇京??绱氭??? */
UENUM(BlueprintType)
enum class EFixPriority : uint8
{
    Critical	UMETA(DisplayName = "Critical"),
    High		UMETA(DisplayName = "High"),
    Medium		UMETA(DisplayName = "Medium"),
    Low		UMETA(DisplayName = "Low")
};

/**
 * 淇京绲愭?
 */
USTRUCT(BlueprintType)
struct FFixResult
{
    GENERATED_BODY()

    /** 淇京?惁?愬? */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    bool bSuccess;

    /** 淇京椤炲? */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    EFixType FixType;

    /** 淇京??绱?*/
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    EFixPriority Priority;

    /** 淇京?忚堪 */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    FString Description;

    /** 淇京瑭虫? */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    FString Details;

    /** 淇京?勬?浠?*/
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    FString FixedFile;

    /** 淇京?傞? */
    UPROPERTY(BlueprintReadOnly, Category = "Fix Result")
    FDateTime FixTime;

    /** ?淇℃伅 */
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
 * 寮遍?淇京?? */
UCLASS(BlueprintType, Blueprintable)
class WEAKNESSFIXER_API UWeaknessFixer : public UObject
{
    GENERATED_BODY()

public:
    UWeaknessFixer(};

    /** ?濆??栦慨寰╁櫒 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    void InitializeFixer(};

    /** ?嬭?瀹屾暣淇京 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    TArray<FFixResult> RunFullFix(};

    /** 淇京?у?娲╂? */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixMemoryLeaks(};

    /** 淇京绶氱?瀹夊叏?忛? */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixThreadSafety(};

    /** 淇京?ц兘?忛? */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixPerformanceIssues(};

    /** 淇京瀹夊叏?忛? */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixSecurityIssues(};

    /** 淇京浠?⒓璩??忛? */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixCodeQuality(};

    /** 淇京?瑰??囦欢 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FFixResult FixSpecificFile(const FString& FilePath};

    /** ?插?淇京姝峰彶 */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    TArray<FFixResult> GetFixHistory(};

    /** ?熸?淇京?卞? */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    FString GenerateFixReport(};

    /** 椹楄?淇京?堟? */
    UFUNCTION(BlueprintCallable, Category = "Weakness Fixer")
    bool VerifyFixes(};

public:
    /** 淇京?嬪?浜嬩欢 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFixStarted, EFixType, FixType};

    /** 淇京瀹屾?浜嬩欢 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFixCompleted, const FFixResult&, FixResult};

    /** 淇京澶辨?浜嬩欢 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFixFailed, const FFixResult&, FixResult};

    UPROPERTY(BlueprintAssignable, Category = "Weakness Fixer|Events")
    FOnFixStarted OnFixStarted;

    UPROPERTY(BlueprintAssignable, Category = "Weakness Fixer|Events")
    FOnFixCompleted OnFixCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Weakness Fixer|Events")
    FOnFixFailed OnFixFailed;

protected:
    /** ?嗘?浠?⒓寮遍? */
    TArray<FString> AnalyzeWeaknesses(const FString& FilePath};

    /** ?夌敤淇京 */
    bool ApplyFix(const FString& FilePath, const FString& Weakness, const FString& Fix};

    /** ?欎唤?熸?浠?*/
    bool BackupFile(const FString& FilePath};

    /** 椹楄?淇京 */
    bool ValidateFix(const FString& FilePath, const FString& OriginalContent, const FString& FixedContent};

    /** 瑷橀?淇京 */
    void RecordFix(const FFixResult& FixResult};

private:
    /** 淇京姝峰彶 */
    UPROPERTY(BlueprintReadOnly, Category = "Weakness Fixer", meta = (AllowPrivateAccess = "true"))
    TArray<FFixResult> FixHistory;

    /** 绯荤当?惁宸插?濮嬪? */
    UPROPERTY(BlueprintReadOnly, Category = "Weakness Fixer", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;

    /** ?欎唤?? */
    UPROPERTY(BlueprintReadOnly, Category = "Weakness Fixer", meta = (AllowPrivateAccess = "true"))
    FString BackupDirectory;

    /** 淇京瑕忓??犲? */
    UPROPERTY(BlueprintReadOnly, Category = "Weakness Fixer", meta = (AllowPrivateAccess = "true"))
    TMap<FString, FString> FixRules;
};

