// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSageCommandTest.generated.h"

class UMingSageCharacterSystem;
class UMingThreePowerSystem;
class UMingMoralAuthority;
class UMingStrategyAuthority;
class UMingMilitaryAuthority;
class UMingWuXingRhythmSystem;
class UMingAntiFallSystem;

/**
 * 測試結果類型枚舉
 */
UENUM(BlueprintType)
enum class ETestResultType: uint8 {
    NotRun      UMETA(DisplayName = "未運行"),
    Passed      UMETA(DisplayName = "通過"),
    Failed      UMETA(DisplayName = "失敗"),
    Skipped     UMETA(DisplayName = "跳過"),
    Error       UMETA(DisplayName = "錯誤"),
    Count
};

/**
 * 測試類別枚舉
 */
UENUM(BlueprintType)
enum class ESageCommandTestCategory: uint8 {
    CharacterSystem     UMETA(DisplayName = "角色類型系統"),
    ThreePowerSystem    UMETA(DisplayName = "三權指揮系統"),
    MoralAuthority      UMETA(DisplayName = "道權系統"),
    StrategyAuthority   UMETA(DisplayName = "策權系統"),
    MilitaryAuthority   UMETA(DisplayName = "兵權系統"),
    WuXingRhythm        UMETA(DisplayName = "五行節奏系統"),
    AntiFallSystem      UMETA(DisplayName = "防墮機制系統"),
    Integration         UMETA(DisplayName = "集成測試"),
    Count
};

/**
 * 單個測試結果結構
 */
USTRUCT(BlueprintType)
struct MINGGORTS_API FSageCommandTestResult
{
    GENERATED_BODY()

    // 測試名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    FString TestName;

    // 測試類別
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    ESageCommandTestCategory Category = ESageCommandTestCategory::CharacterSystem;

    // 測試結果
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    ETestResultType Result = ETestResultType::NotRun;

    // 測試描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    FString Description;

    // 錯誤信息 (如果失敗)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    FString ErrorMessage;

    // 執行時間 (秒)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    float ExecutionTime = 0.0f;

    FSageCommandTestResult()
        : Category(ESageCommandTestCategory::CharacterSystem)
        , Result(ETestResultType::NotRun)
        , ExecutionTime(0.0f)
    {}
};

/**
 * 測試套件摘要結構
 */
USTRUCT(BlueprintType)
struct MINGGORTS_API FTestSuiteSummary
{
    GENERATED_BODY()

    // 總測試數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summary")
    int32 TotalTests = 0;

    // 通過數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summary")
    int32 PassedTests = 0;

    // 失敗數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summary")
    int32 FailedTests = 0;

    // 跳過數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summary")
    int32 SkippedTests = 0;

    // 錯誤數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summary")
    int32 ErrorTests = 0;

    // 總執行時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summary")
    float TotalExecutionTime = 0.0f;

    // 通過率
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summary")
    float PassRate = 0.0f;

    // 是否全部通過
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summary")
    bool bAllPassed = false;

    FTestSuiteSummary()
        : TotalTests(0)
        , PassedTests(0)
        , FailedTests(0)
        , SkippedTests(0)
        , ErrorTests(0)
        , TotalExecutionTime(0.0f)
        , PassRate(0.0f)
        , bAllPassed(false)
    {}
};

/**
 * 至聖者指揮學 - 綜合測試套件
 * 測試所有子系統的功能和集成
 */
UCLASS(ClassGroup = (SageCommand), meta = (BlueprintSpawnableComponent))
class MINGGORTS_API UMingSageCommandTest : public UObject
{
    GENERATED_BODY()

public:
    UMingSageCommandTest();

    // 初始化測試套件
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Test")
    void InitializeTestSuite();

    // 設置測試目標系統
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Test")
    void SetTargetSystems(UMingSageCharacterSystem* CharacterSys,
                          UMingThreePowerSystem* PowerSys,
                          UMingMoralAuthority* MoralAuth,
                          UMingStrategyAuthority* StrategyAuth,
                          UMingMilitaryAuthority* MilitaryAuth,
                          UMingWuXingRhythmSystem* WuXingSys,
                          UMingAntiFallSystem* AntiFallSys);

    // 運行所有測試
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Test")
    FTestSuiteSummary RunAllTests();

    // 運行特定類別測試
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Test")
    TArray<FSageCommandTestResult> RunTestCategory(ESageCommandTestCategory Category);

    // 運行單個測試
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Test")
    FSageCommandTestResult RunSingleTest(const FString& TestName);

    // 獲取所有測試結果
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Test")
    TArray<FSageCommandTestResult> GetAllTestResults() const { return TestResults; }

    // 獲取測試摘要
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Test")
    FTestSuiteSummary GetTestSummary() const;

    // 生成測試報告
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Test")
    FString GenerateTestReport() const;

    // 保存測試報告到文件
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Test")
    bool SaveTestReportToFile(const FString& FilePath) const;

    // 事件：測試完成
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestCompleted, const FTestSuiteSummary&, Summary);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|Test")
    FOnTestCompleted OnTestCompleted;

    // 事件：單個測試完成
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSingleTestCompleted, const FSageCommandTestResult&, Result);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|Test")
    FOnSingleTestCompleted OnSingleTestCompleted;

protected:
    // 各類別測試方法
    TArray<FSageCommandTestResult> RunCharacterSystemTests();
    TArray<FSageCommandTestResult> RunThreePowerSystemTests();
    TArray<FSageCommandTestResult> RunMoralAuthorityTests();
    TArray<FSageCommandTestResult> RunStrategyAuthorityTests();
    TArray<FSageCommandTestResult> RunMilitaryAuthorityTests();
    TArray<FSageCommandTestResult> RunWuXingRhythmTests();
    TArray<FSageCommandTestResult> RunAntiFallSystemTests();
    TArray<FSageCommandTestResult> RunIntegrationTests();

    // 輔助測試方法
    void RecordTestResult(const FString& TestName, ESageCommandTestCategory Category, 
                          ETestResultType Result, const FString& Description, 
                          const FString& ErrorMessage = TEXT(""), float ExecutionTime = 0.0f);

    bool AssertTrue(bool Condition, const FString& ErrorMessage);
    bool AssertEquals(int32 Expected, int32 Actual, const FString& ErrorMessage);
    bool AssertNotNull(UObject* Object, const FString& ErrorMessage);

protected:
    // 測試結果列表
    UPROPERTY()
    TArray<FSageCommandTestResult> TestResults;

    // 目標系統引用
    UPROPERTY()
    TObjectPtr<UMingSageCharacterSystem> CharacterSystem;

    UPROPERTY()
    TObjectPtr<UMingThreePowerSystem> ThreePowerSystem;

    UPROPERTY()
    TObjectPtr<UMingMoralAuthority> MoralAuthority;

    UPROPERTY()
    TObjectPtr<UMingStrategyAuthority> StrategyAuthority;

    UPROPERTY()
    TObjectPtr<UMingMilitaryAuthority> MilitaryAuthority;

    UPROPERTY()
    TObjectPtr<UMingWuXingRhythmSystem> WuXingRhythmSystem;

    UPROPERTY()
    TObjectPtr<UMingAntiFallSystem> AntiFallSystem;

    // 是否已初始化
    UPROPERTY()
    bool bIsInitialized = false;

    // 測試開始時間
    UPROPERTY()
    float TestStartTime = 0.0f;
};
