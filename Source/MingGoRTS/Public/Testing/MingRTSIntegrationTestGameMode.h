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
    AMingRTSIntegrationTestGameMode(};

    /** GameMode動池��動*/
    virtual void BeginPlay() overHide;

    /** �t������ */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartIntegrationTest(};

    /** ����動務*/
    UFUNCTION(BlueprintPure, Category = "Integration Test")
    UMingRTSIntegrationTestExecutor* GetTestExecutor() const { return TestExecutor; }

protected:
    /** ����動務*/
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    TObjectPtr<UMingRTSIntegrationTestExecutor> TestExecutor;

    /** ���հ���}�l�ɥ礎� */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestExecutionStarted(};

    /** ���հ��槹���ɥ礎� */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestExecutionCompleted(bool bAllTestsPassed};

    /** ����動�q��池動 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestPhaseCompleted(const FString& PhaseName, bool bPassed};

private:
    /** 動池���հ��澹 */
    void InitializeTestExecutor(};

    /** �]�m�������ҰѼ� */
    void SetupTestEnvironment(};

    /** �M�z�������ҰѼ� */
    void CleanupTestEnvironment(};
};
