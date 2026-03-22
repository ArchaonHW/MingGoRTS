// Copyright (c) 2026 MingGoRTS. All rights reserved.
// �t��摧毀����摧毀��?? - GameMode摧毀

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MingRTSIntegrationTestExecutor.h"
#include "MingRTSIntegrationTestGameMode.generated.h"

/**
 * �t��摧毀����GameMode
 * ??��摧毀�t��摧毀����???X?GameMode
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API AMingRTSIntegrationTestGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMingRTSIntegrationTestGameMode();

    /** GameMode???X?��??*/
    virtual void BeginPlay() override;

    /** 摧毀�t��摧毀���� */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartIntegrationTest();

    /** 摧毀����???X*/
    UFUNCTION(BlueprintPure, Category = "Integration Test")
    UMingRTSIntegrationTestExecutor* GetTestExecutor() const { return TestExecutor; }

protected:
    /** ����???X*/
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    TObjectPtr<UMingRTSIntegrationTestExecutor> TestExecutor;

    /** ���հ���}�l�ƥ� */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestExecutionStarted();

    /** ���հ��槹���ƥ� */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestExecutionCompleted(bool bAllTestsPassed);

    /** ����??�q��?X??? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestPhaseCompleted(const FString& PhaseName, bool bPassed);

private:
    /** ???X?���հ��澹 */
    void InitializeTestExecutor();

    /** �]�m�������ҰѼ� */
    void SetupTestEnvironment();

    /** �M�z�������ҰѼ� */
    void CleanupTestEnvironment();
};
