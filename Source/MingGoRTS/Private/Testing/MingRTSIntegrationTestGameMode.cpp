// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 系統集成測試執行示例 - GameMode實現

#include "Testing/MingRTSIntegrationTestGameMode.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

AMingRTSIntegrationTestGameMode::AMingRTSIntegrationTestGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    
    // 創建測試執行器
    TestExecutor = CreateDefaultSubobject<UMingRTSIntegrationTestExecutor>(TEXT("TestExecutor"));
}

void AMingRTSIntegrationTestGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Log, TEXT("MingRTS Integration Test GameMode - BeginPlay"));
    
    // 初始化測試執行器
    InitializeTestExecutor();
    
    // 設置測試環境
    SetupTestEnvironment();
    
    // 延遲2秒後自動開始測試
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AMingRTSIntegrationTestGameMode::StartIntegrationTest, 2.0f, false);
}

void AMingRTSIntegrationTestGameMode::InitializeTestExecutor()
{
    if (TestExecutor)
    {
        // 綁定事件
        TestExecutor->OnTestExecutionStarted.AddDynamic(this, &AMingRTSIntegrationTestGameMode::OnTestExecutionStarted);
        TestExecutor->OnTestExecutionCompleted.AddDynamic(this, &AMingRTSIntegrationTestGameMode::OnTestExecutionCompleted);
        TestExecutor->OnTestPhaseExecutionCompleted.AddDynamic(this, &AMingRTSIntegrationTestGameMode::OnTestPhaseCompleted);
        
        UE_LOG(LogTemp, Log, TEXT("Integration Test Executor initialized successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create Integration Test Executor"));
    }
}

void AMingRTSIntegrationTestGameMode::SetupTestEnvironment()
{
    UE_LOG(LogTemp, Log, TEXT("Setting up integration test environment..."));
    
    // 設置測試相關的遊戲參數
    if (GEngine)
    {
        GEngine->bEnableOnScreenDebugMessages = true;
    }
    
    // 設置測試相關的世界設置
    if (GetWorld())
    {
        // 確保世界設置適合測試
        GetWorld()->bWorldWasLoadedThisTick = false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Integration test environment setup completed"));
}

void AMingRTSIntegrationTestGameMode::CleanupTestEnvironment()
{
    UE_LOG(LogTemp, Log, TEXT("Cleaning up integration test environment..."));
    
    // 清理測試相關的資源
    // 這裡可以添加清理邏輯
    
    UE_LOG(LogTemp, Log, TEXT("Integration test environment cleanup completed"));
}

void AMingRTSIntegrationTestGameMode::StartIntegrationTest()
{
    UE_LOG(LogTemp, Log, TEXT("Starting MingGoRTS Integration Test..."));
    
    if (TestExecutor)
    {
        TestExecutor->ExecuteFullIntegrationTest();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot start integration test: TestExecutor is null"));
    }
}

void AMingRTSIntegrationTestGameMode::OnTestExecutionStarted()
{
    UE_LOG(LogTemp, Log, TEXT("=== Integration Test Execution Started ==="));
    
    // 在屏幕上顯示測試開始信息
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Integration Test Started"));
    }
}

void AMingRTSIntegrationTestGameMode::OnTestExecutionCompleted(bool bAllTestsPassed)
{
    UE_LOG(LogTemp, Log, TEXT("=== Integration Test Execution Completed ==="));
    UE_LOG(LogTemp, Log, TEXT("Overall Result: %s"), bAllTestsPassed ? TEXT("PASSED") : TEXT("FAILED"));
    
    // 在屏幕上顯示最終結果
    if (GEngine)
    {
        FColor ResultColor = bAllTestsPassed ? FColor::Green : FColor::Red;
        FString ResultText = bAllTestsPassed ? TEXT("Integration Test PASSED") : TEXT("Integration Test FAILED");
        GEngine->AddOnScreenDebugMessage(-1, 30.0f, ResultColor, ResultText);
    }
    
    // 獲取詳細測試結果
    if (TestExecutor)
    {
        TArray<FString> Results = TestExecutor->GetTestExecutionResults();
        UE_LOG(LogTemp, Log, TEXT("=== Detailed Test Results ==="));
        for (const FString& Result : Results)
        {
            UE_LOG(LogTemp, Log, TEXT("%s"), *Result);
        }
    }
    
    // 清理測試環境
    CleanupTestEnvironment();
    
    // 如果測試通過，可以選擇退出或繼續其他操作
    if (bAllTestsPassed)
    {
        UE_LOG(LogTemp, Log, TEXT("All integration tests passed! System is ready for Beta release."));
        
        // 可以在這裡添加通過後的處理邏輯
        // 例如：自動退出、載入下一個場景等
        
        // 5秒後退出遊戲（可選）
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(TimerHandle, [this]() {
            if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
            {
                UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit);
            }
        }, 5.0f, false);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Some integration tests failed! Please check the logs and fix the issues."));
        
        // 可以在這裡添加失敗後的處理邏輯
        // 例如：保持遊戲運行以便調試
    }
}

void AMingRTSIntegrationTestGameMode::OnTestPhaseCompleted(const FString& PhaseName, bool bPassed)
{
    UE_LOG(LogTemp, Log, TEXT("Test Phase '%s' %s"), *PhaseName, bPassed ? TEXT("PASSED") : TEXT("FAILED"));
    
    // 在屏幕上顯示階段結果
    if (GEngine)
    {
        FColor PhaseColor = bPassed ? FColor::Green : FColor::Red;
        FString PhaseText = FString::Printf(TEXT("Phase %s: %s"), *PhaseName, bPassed ? TEXT("PASSED") : TEXT("FAILED"));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, PhaseColor, PhaseText);
    }
}
