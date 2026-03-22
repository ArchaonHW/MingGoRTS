// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Á≥ªÁµ±?ÜÊ?Ê∏¨Ë©¶?∑Ë?Á§∫‰? - GameMode?ÜÊ?

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MingRTSIntegrationTestExecutor.h"
#include "MingRTSIntegrationTestGameMode.generated.h"

/**
 * Á≥ªÁµ±?ÜÊ?Ê∏¨Ë©¶GameMode
 * ?®Êñº?∑Ë?Á≥ªÁµ±?ÜÊ?Ê∏¨Ë©¶?ÑÂ??®GameMode
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API AMingRTSIntegrationTestGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMingRTSIntegrationTestGameMode();

    /** GameMode?ãÂ??ÇÂü∑Ë°?*/
    virtual void BeginPlay() override;

    /** ?üÂ?Á≥ªÁµ±?ÜÊ?Ê∏¨Ë©¶ */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void StartIntegrationTest();

    /** ?≤Â?Ê∏¨Ë©¶?∑Ë???*/
    UFUNCTION(BlueprintPure, Category = "Integration Test")
    UMingRTSIntegrationTestExecutor* GetTestExecutor() const { return TestExecutor; }

protected:
    /** Ê∏¨Ë©¶?∑Ë???*/
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    TObjectPtr<UMingRTSIntegrationTestExecutor> TestExecutor;

    /** Ê∏¨Ë©¶?∑Ë??ãÂ??ïÁ? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestExecutionStarted();

    /** Ê∏¨Ë©¶?∑Ë?ÂÆåÊ??ïÁ? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestExecutionCompleted(bool bAllTestsPassed);

    /** Ê∏¨Ë©¶?éÊÆµÂÆåÊ??ïÁ? */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void OnTestPhaseCompleted(const FString& PhaseName, bool bPassed);

private:
    /** ?ùÂ??ñÊ∏¨Ë©¶Âü∑Ë°åÂô® */
    void InitializeTestExecutor();

    /** Ë®≠ÁΩÆÊ∏¨Ë©¶?∞Â? */
    void SetupTestEnvironment();

    /** Ê∏ÖÁ?Ê∏¨Ë©¶?∞Â? */
    void CleanupTestEnvironment();
};
