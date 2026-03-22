// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 系統集成測試執行示例 - GameMode集成

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MingRTSIntegrationTestExecutor.h"
#include "MingRTSIntegrationTestGameMode.generated.h"

/**
 * 系統集成測試GameMode
 * 用於執行系統集成測試的專用GameMode
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API AMingRTSIntegrationTestGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMingRTSIntegrationTestGameMode();

    /** GameMode開始時執行 */
    virtual void BeginPlay() override;

    /** 啟動系統集成測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartIntegrationTest();

    /** 獲取測試執行器 */
    UFUNCTION(BlueprintPure, Category = "Integration Test")
    UMingRTSIntegrationTestExecutor* GetTestExecutor() const { return TestExecutor; }

protected:
    /** 測試執行器 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    TObjectPtr<UMingRTSIntegrationTestExecutor> TestExecutor;

    /** 測試執行開始處理 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestExecutionStarted();

    /** 測試執行完成處理 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestExecutionCompleted(bool bAllTestsPassed);

    /** 測試階段完成處理 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestPhaseCompleted(const FString& PhaseName, bool bPassed);

private:
    /** 初始化測試執行器 */
    void InitializeTestExecutor();

    /** 設置測試環境 */
    void SetupTestEnvironment();

    /** 清理測試環境 */
    void CleanupTestEnvironment();
};
