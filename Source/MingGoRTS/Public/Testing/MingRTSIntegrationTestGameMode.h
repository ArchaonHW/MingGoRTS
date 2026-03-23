#pragma once

// Copyright (c) 2026 MingGoRTS. All rights reserved.
// �t��������動 - GameMode


#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MingRTSIntegrationTestExecutor.h"
#include "MingRTSIntegrationTestGameMode.generated.h"

/**
 * �t������GameMode
 * 動���t������動池GameMode
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API AMingRTSIntegrationTestGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMingRTSIntegrationTestGameMode();

    /** GameMode初始化動作 */
    virtual void BeginPlay() override;

    /** 開始集成測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartIntegrationTest();

    /** 獲取測試執行器 */
    UFUNCTION(BlueprintPure, Category = "Integration Test")
    UMingRTSIntegrationTestExecutor* GetTestExecutor() const { return TestExecutor; }

    /** 測試執行器 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    TObjectPtr<UMingRTSIntegrationTestExecutor> TestExecutor;

protected:
    /** 測試執行開始回調 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestExecutionStarted();

    /** 測試執行結束回調 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    /** 測試執行完成 */
    void OnTestExecutionCompleted(bool bAllTestsPassed);

    /** 測試階段完成回調 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestPhaseCompleted(const FString& PhaseName, bool bPassed);

private:
    /** 動態創建測試執行器 */
    void InitializeTestExecutor();

    /** 設置測試環境 */
    void SetupTestEnvironment();

    /** 清理測試環境 */
    void CleanupTestEnvironment();
};
