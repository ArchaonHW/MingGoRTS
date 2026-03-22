// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 系統?��?測試?��?示�? - GameMode?��?

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MingRTSIntegrationTestExecutor.h"
#include "MingRTSIntegrationTestGameMode.generated.h"

/**
 * 系統?��?測試GameMode
 * ?�於?��?系統?��?測試?��X�GameMode
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API AMingRTSIntegrationTestGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMingRTSIntegrationTestGameMode(};

    /** GameMode?��X�執�?*/
    virtual void BeginPlay() override;

    /** ?��?系統?��?測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartIntegrationTest(};

    /** ?��?測試?��X*/
    UFUNCTION(BlueprintPure, Category = "Integration Test")
    UMingRTSIntegrationTestExecutor* GetTestExecutor() const { return TestExecutor; }

protected:
    /** 測試?��X*/
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    TObjectPtr<UMingRTSIntegrationTestExecutor> TestExecutor;

    /** 測試?��X��X��? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestExecutionStarted(};

    /** 測試?��?完�X��? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestExecutionCompleted(bool bAllTestsPassed};

    /** 測試?�段完�X��? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestPhaseCompleted(const FString& PhaseName, bool bPassed};

private:
    /** ?��X�測試執行器 */
    void InitializeTestExecutor(};

    /** 設置測試?��? */
    void SetupTestEnvironment(};

    /** 清�?測試?��? */
    void CleanupTestEnvironment(};
};

