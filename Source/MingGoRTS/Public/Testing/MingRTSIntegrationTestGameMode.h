// Copyright (c) 2026 MingGoRTS. All rights reserved.
// ╰参????代刚????ボ?? - GameMode????

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MingRTSIntegrationTestExecutor.h"
#include "MingRTSIntegrationTestGameMode.generated.h"

/**
 * ╰参????代刚GameMode
 * ??????╰参????代刚???X?GameMode
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API AMingRTSIntegrationTestGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMingRTSIntegrationTestGameMode();

    /** GameMode???X?磅??*/
    virtual void BeginPlay() override;

    /** ????╰参????代刚 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartIntegrationTest();

    /** ????代刚???X*/
    UFUNCTION(BlueprintPure, Category = "Integration Test")
    UMingRTSIntegrationTestExecutor* GetTestExecutor() const { return TestExecutor; }

protected:
    /** 代刚???X*/
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    TObjectPtr<UMingRTSIntegrationTestExecutor> TestExecutor;

    /** 代刚磅︽秨﹍ㄆン */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestExecutionStarted();

    /** 代刚磅︽ЧΘㄆン */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestExecutionCompleted(bool bAllTestsPassed);

    /** 代刚??琿Ч?X??? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestPhaseCompleted(const FString& PhaseName, bool bPassed);

private:
    /** ???X?代刚磅︽竟 */
    void InitializeTestExecutor();

    /** 砞竚代刚吏挂把计 */
    void SetupTestEnvironment();

    /** 睲瞶代刚吏挂把计 */
    void CleanupTestEnvironment();
};
