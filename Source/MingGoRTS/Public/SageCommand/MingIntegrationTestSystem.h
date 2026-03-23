#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingIntegrationTestSystem.generated.h"

// 测试状态
UENUM(BlueprintType)
enum class EIntegrationTestStatus : uint8
{
    NotStarted,    // 未开始
    Running,       // 运行中
    Passed,        // 通过
    Failed,        // 失败
    Error          // 错误
};

// 测试结果
USTRUCT(BlueprintType)
struct MINGGORTS_API FIntegrationTestResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TestName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EIntegrationTestStatus Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Message;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;
};

// 系统集成测试主类
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingIntegrationTestSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingIntegrationTestSystem();

    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void InitializeSystem();

    // 运行所有测试
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void RunAllTests();

    // 测试角色系统
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    bool TestCharacterSystem();

    // 测试三权系统
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    bool TestThreePowerSystem();

    // 测试策略系统
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    bool TestStrategySystem();

    // 测试防堕系统
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    bool TestAntiFallSystem();

    // 测试 AI 适配系统
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    bool TestAIAdapterSystem();

    // 获取测试结果
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    TArray<FIntegrationTestResult> GetTestResults() const { return TestResults; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Integration Test")
    TArray<FIntegrationTestResult> TestResults;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Integration Test")
    int32 TotalTests;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Integration Test")
    int32 PassedTests;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Integration Test")
    int32 FailedTests;

private:
    void AddTestResult(const FString& TestName, EIntegrationTestStatus Status, const FString& Message);
    void ResetTestResults();
};
