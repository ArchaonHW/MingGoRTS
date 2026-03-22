#include "MingUISystemTest.h"
#include "MingRelationshipManager.h"
#include "MingAudioRelationshipManager.h"
#include "MingPersonalUIManager.h"
#include "MingRelationshipPanel.h"
#include "MingReputationPanel.h"
#include "MingDialoguePanel.h"
#include "MingQuestPanel.h"
#include "MingAudioPanel.h"
#include "MingMainDashboard.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"

UMingUISystemTest::UMingUISystemTest()
{
    TestResults = MakeShared<FTestSuiteResults>();
    CurrentTestIndex = 0;
    bIsRunning = false;
    TestTimeout = 30.0f;
}

void UMingUISystemTest::InitializeTestSuite(UMingRelationshipManager* InRelationshipManager, UMingAudioRelationshipManager* InAudioManager, UMingPersonalUIManager* InUIManager)
{
    RelationshipManager = InRelationshipManager;
    AudioRelationshipManager = InAudioManager;
    UIManager = InUIManager;
    
    // 初始化測試結果
    TestResults->TotalTests = 0;
    TestResults->PassedTests = 0;
    TestResults->FailedTests = 0;
    TestResults->SkippedTests = 0;
    TestResults->TestResults.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("UI系統測試套件初始化完成"));
}

FTestSuiteResults UMingUISystemTest::RunAllTests()
{
    if (bIsRunning)
    {
        UE_LOG(LogTemp, Warning, TEXT("測試套件已在運行中"));
        return *TestResults;
    }
    
    bIsRunning = true;
    CurrentTestIndex = 0;
    
    // 重置測試結果
    TestResults->TotalTests = 0;
    TestResults->PassedTests = 0;
    TestResults->FailedTests = 0;
    TestResults->SkippedTests = 0;
    TestResults->TestResults.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("開始運行UI系統測試套件"));
    
    // 運行所有測試
    RunTestCategory(ETestCategory::UIManager);
    RunTestCategory(ETestCategory::RelationshipPanel);
    RunTestCategory(ETestCategory::ReputationPanel);
    RunTestCategory(ETestCategory::DialoguePanel);
    RunTestCategory(ETestCategory::QuestPanel);
    RunTestCategory(ETestCategory::AudioPanel);
    RunTestCategory(ETestCategory::MainDashboard);
    RunTestCategory(ETestCategory::Integration);
    RunTestCategory(ETestCategory::Performance);
    RunTestCategory(ETestCategory::Accessibility);
    RunTestCategory(ETestCategory::ErrorHandling);
    
    bIsRunning = false;
    
    // 生成測試報告
    GenerateTestReport();
    
    UE_LOG(LogTemp, Log, TEXT("UI系統測試套件運行完成 - 通過: %d, 失敗: %d, 跳過: %d"), 
           TestResults->PassedTests, TestResults->FailedTests, TestResults->SkippedTests);
    
    return *TestResults;
}

FTestSuiteResults UMingUISystemTest::RunTestCategory(ETestCategory Category)
{
    TArray<TFunction<void()>> TestFunctions;
    
    switch (Category)
    {
        case ETestCategory::UIManager:
            TestFunctions = {
                [this]() { TestUIManagerInitialization(); },
                [this]() { TestUIManagerPanelManagement(); },
                [this]() { TestUIManagerEventHandling(); },
                [this]() { TestUIManagerThemeSystem(); }
            };
            break;
            
        case ETestCategory::RelationshipPanel:
            TestFunctions = {
                [this]() { TestRelationshipPanelInitialization(); },
                [this]() { TestRelationshipPanelDataUpdate(); },
                [this]() { TestRelationshipPanelFiltering(); },
                [this]() { TestRelationshipPanelSorting(); }
            };
            break;
            
        case ETestCategory::ReputationPanel:
            TestFunctions = {
                [this]() { TestReputationPanelInitialization(); },
                [this]() { TestReputationPanelDataUpdate(); },
                [this]() { TestReputationPanelMapDisplay(); },
                [this]() { TestReputationPanelEffects(); }
            };
            break;
            
        case ETestCategory::DialoguePanel:
            TestFunctions = {
                [this]() { TestDialoguePanelInitialization(); },
                [this]() { TestDialoguePanelTypewriterEffect(); },
                [this]() { TestDialoguePanelOptions(); },
                [this]() { TestDialoguePanelActions(); }
            };
            break;
            
        case ETestCategory::QuestPanel:
            TestFunctions = {
                [this]() { TestQuestPanelInitialization(); },
                [this]() { TestQuestPanelDataUpdate(); },
                [this]() { TestQuestPanelFiltering(); },
                [this]() { TestQuestPanelActions(); }
            };
            break;
            
        case ETestCategory::AudioPanel:
            TestFunctions = {
                [this]() { TestAudioPanelInitialization(); },
                [this]() { TestAudioPanelVolumeControl(); },
                [this]() { TestAudioPanelThemeSelection(); },
                [this]() { TestAudioPanelTestFeatures(); }
            };
            break;
            
        case ETestCategory::MainDashboard:
            TestFunctions = {
                [this]() { TestMainDashboardInitialization(); },
                [this]() { TestMainDashboardPanelSwitching(); },
                [this]() { TestMainDashboardStatistics(); },
                [this]() { TestMainDashboardAutoRefresh(); }
            };
            break;
            
        case ETestCategory::Integration:
            TestFunctions = {
                [this]() { TestPanelDataIntegration(); },
                [this]() { TestPanelEventIntegration(); },
                [this]() { TestCrossPanelCommunication(); },
                [this]() { TestSystemWideUpdates(); }
            };
            break;
            
        case ETestCategory::Performance:
            TestFunctions = {
                [this]() { TestUIPerformance(); },
                [this]() { TestMemoryUsage(); },
                [this]() { TestRenderingPerformance(); },
                [this]() { TestLargeDatasetHandling(); }
            };
            break;
            
        case ETestCategory::Accessibility:
            TestFunctions = {
                [this]() { TestKeyboardNavigation(); },
                [this]() { TestScreenReaderSupport(); },
                [this]() { TestColorBlindnessSupport(); },
                [this]() { TestTextScaling(); }
            };
            break;
            
        case ETestCategory::ErrorHandling:
            TestFunctions = {
                [this]() { TestInvalidDataHandling(); },
                [this]() { TestNullPointerHandling(); },
                [this]() { TestNetworkErrorHandling(); },
                [this]() { TestCorruptedDataRecovery(); }
            };
            break;
    }
    
    // 運行類別中的所有測試
    for (const auto& TestFunction : TestFunctions)
    {
        if (CurrentTestIndex < TestFunctions.Num())
        {
            TestFunction();
            CurrentTestIndex++;
        }
    }
    
    return *TestResults;
}

FTestResult UMingUISystemTest::RunSingleTest(const FString& TestName)
{
    FTestResult Result;
    Result.TestName = TestName;
    Result.TestCategory = ETestCategory::Custom;
    Result.bPassed = false;
    Result.ExecutionTime = 0.0f;
    Result.ErrorMessage = TEXT("");
    
    double StartTime = FPlatformTime::Seconds();
    
    try
    {
        // 根據測試名稱運行對應的測試
        if (TestName == TEXT("UIManagerInitialization"))
        {
            TestUIManagerInitialization();
            Result.bPassed = true;
        }
        else if (TestName == TEXT("RelationshipPanelInitialization"))
        {
            TestRelationshipPanelInitialization();
            Result.bPassed = true;
        }
        // 可以添加更多測試...
        else
        {
            Result.ErrorMessage = TEXT("未知的測試名稱");
        }
    }
    catch (const std::exception& e)
    {
        Result.ErrorMessage = FString(e.what());
        Result.bPassed = false;
    }
    
    Result.ExecutionTime = FPlatformTime::Seconds() - StartTime;
    
    return Result;
}

void UMingUISystemTest::StopTestSuite()
{
    bIsRunning = false;
    
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(TestTimeoutTimerHandle);
    }
    
    UE_LOG(LogTemp, Log, TEXT("UI系統測試套件已停止"));
}

// UI管理器測試
void UMingUISystemTest::TestUIManagerInitialization()
{
    RunTest(TEXT("UIManagerInitialization"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        // 檢查UI管理器是否正確初始化
        bool bInitialized = UIManager->IsInitialized();
        if (!bInitialized)
        {
            return TestResult(false, TEXT("UI管理器初始化失敗"));
        }
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestUIManagerPanelManagement()
{
    RunTest(TEXT("UIManagerPanelManagement"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        // 測試面板創建和管理
        bool bPanelCreated = UIManager->CreatePanel(ETestPanel::Relationship);
        if (!bPanelCreated)
        {
            return TestResult(false, TEXT("面板創建失敗"));
        }
        
        bool bPanelDestroyed = UIManager->DestroyPanel(ETestPanel::Relationship);
        if (!bPanelDestroyed)
        {
            return TestResult(false, TEXT("面板銷毀失敗"));
        }
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestUIManagerEventHandling()
{
    RunTest(TEXT("UIManagerEventHandling"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        // 測試事件處理
        bool bEventBound = UIManager->BindEvent(ETestEvent::PanelShown, this, TEXT("OnPanelShown"));
        if (!bEventBound)
        {
            return TestResult(false, TEXT("事件綁定失敗"));
        }
        
        bool bEventTriggered = UIManager->TriggerEvent(ETestEvent::PanelShown);
        if (!bEventTriggered)
        {
            return TestResult(false, TEXT("事件觸發失敗"));
        }
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestUIManagerThemeSystem()
{
    RunTest(TEXT("UIManagerThemeSystem"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        // 測試主題系統
        bool bThemeLoaded = UIManager->LoadTheme(TEXT("DefaultTheme"));
        if (!bThemeLoaded)
        {
            return TestResult(false, TEXT("主題加載失敗"));
        }
        
        bool bThemeApplied = UIManager->ApplyTheme(TEXT("DefaultTheme"));
        if (!bThemeApplied)
        {
            return TestResult(false, TEXT("主題應用失敗"));
        }
        
        return TestResult(true);
    });
}

// 關係面板測試
void UMingUISystemTest::TestRelationshipPanelInitialization()
{
    RunTest(TEXT("RelationshipPanelInitialization"), [this]()
    {
        if (!UIManager || !UIManager->GetRelationshipPanel())
        {
            return TestResult(false, TEXT("關係面板未創建"));
        }
        
        UMingRelationshipPanel* Panel = UIManager->GetRelationshipPanel();
        if (!Panel->IsValidLowLevel())
        {
            return TestResult(false, TEXT("關係面板無效"));
        }
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestRelationshipPanelDataUpdate()
{
    RunTest(TEXT("RelationshipPanelDataUpdate"), [this]()
    {
        if (!RelationshipManager || !UIManager)
        {
            return TestResult(false, TEXT("依賴組件未初始化"));
        }
        
        UMingRelationshipPanel* Panel = UIManager->GetRelationshipPanel();
        if (!Panel)
        {
            return TestResult(false, TEXT("關係面板未創建"));
        }
        
        // 測試數據更新
        Panel->UpdateRelationshipData(RelationshipManager);
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestRelationshipPanelFiltering()
{
    RunTest(TEXT("RelationshipPanelFiltering"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        UMingRelationshipPanel* Panel = UIManager->GetRelationshipPanel();
        if (!Panel)
        {
            return TestResult(false, TEXT("關係面板未創建"));
        }
        
        // 測試過濾功能
        Panel->SetRelationshipFilter(ERelationshipFilter::Friends);
        Panel->SetRelationshipFilter(ERelationshipFilter::Enemies);
        Panel->SetRelationshipFilter(ERelationshipFilter::All);
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestRelationshipPanelSorting()
{
    RunTest(TEXT("RelationshipPanelSorting"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        UMingRelationshipPanel* Panel = UIManager->GetRelationshipPanel();
        if (!Panel)
        {
            return TestResult(false, TEXT("關係面板未創建"));
        }
        
        // 測試排序功能
        Panel->SetRelationshipSort(ERelationshipSortType::ByName);
        Panel->SetRelationshipSort(ERelationshipSortType::ByValue);
        Panel->SetRelationshipSort(ERelationshipSortType::ByChange);
        
        return TestResult(true);
    });
}

// 聲望面板測試
void UMingUISystemTest::TestReputationPanelInitialization()
{
    RunTest(TEXT("ReputationPanelInitialization"), [this]()
    {
        if (!UIManager || !UIManager->GetReputationPanel())
        {
            return TestResult(false, TEXT("聲望面板未創建"));
        }
        
        UMingReputationPanel* Panel = UIManager->GetReputationPanel();
        if (!Panel->IsValidLowLevel())
        {
            return TestResult(false, TEXT("聲望面板無效"));
        }
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestReputationPanelDataUpdate()
{
    RunTest(TEXT("ReputationPanelDataUpdate"), [this]()
    {
        if (!RelationshipManager || !UIManager)
        {
            return TestResult(false, TEXT("依賴組件未初始化"));
        }
        
        UMingReputationPanel* Panel = UIManager->GetReputationPanel();
        if (!Panel)
        {
            return TestResult(false, TEXT("聲望面板未創建"));
        }
        
        // 測試數據更新
        Panel->UpdateReputationData(RelationshipManager);
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestReputationPanelMapDisplay()
{
    RunTest(TEXT("ReputationPanelMapDisplay"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        UMingReputationPanel* Panel = UIManager->GetReputationPanel();
        if (!Panel)
        {
            return TestResult(false, TEXT("聲望面板未創建"));
        }
        
        // 測試地圖顯示功能
        Panel->SelectRegion(TEXT("TestRegion"));
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestReputationPanelEffects()
{
    RunTest(TEXT("ReputationPanelEffects"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        UMingReputationPanel* Panel = UIManager->GetReputationPanel();
        if (!Panel)
        {
            return TestResult(false, TEXT("聲望面板未創建"));
        }
        
        // 測試效果顯示
        Panel->ShowReputationEffects(TEXT("TestRegion"));
        
        return TestResult(true);
    });
}

// 對話面板測試
void UMingUISystemTest::TestDialoguePanelInitialization()
{
    RunTest(TEXT("DialoguePanelInitialization"), [this]()
    {
        if (!UIManager || !UIManager->GetDialoguePanel())
        {
            return TestResult(false, TEXT("對話面板未創建"));
        }
        
        UMingDialoguePanel* Panel = UIManager->GetDialoguePanel();
        if (!Panel->IsValidLowLevel())
        {
            return TestResult(false, TEXT("對話面板無效"));
        }
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestDialoguePanelTypewriterEffect()
{
    RunTest(TEXT("DialoguePanelTypewriterEffect"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        UMingDialoguePanel* Panel = UIManager->GetDialoguePanel();
        if (!Panel)
        {
            return TestResult(false, TEXT("對話面板未創建"));
        }
        
        // 測試打字機效果
        Panel->SetTypingSpeed(0.05f);
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestDialoguePanelOptions()
{
    RunTest(TEXT("DialoguePanelOptions"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        UMingDialoguePanel* Panel = UIManager->GetDialoguePanel();
        if (!Panel)
        {
            return TestResult(false, TEXT("對話面板未創建"));
        }
        
        // 測試選項功能
        Panel->SelectDialogueOption(0);
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestDialoguePanelActions()
{
    RunTest(TEXT("DialoguePanelActions"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        UMingDialoguePanel* Panel = UIManager->GetDialoguePanel();
        if (!Panel)
        {
            return TestResult(false, TEXT("對話面板未創建"));
        }
        
        // 測試自動前進功能
        Panel->SetAutoAdvance(true, 3.0f);
        
        return TestResult(true);
    });
}

// 任務面板測試
void UMingUISystemTest::TestQuestPanelInitialization()
{
    RunTest(TEXT("QuestPanelInitialization"), [this]()
    {
        if (!UIManager || !UIManager->GetQuestPanel())
        {
            return TestResult(false, TEXT("任務面板未創建"));
        }
        
        UMingQuestPanel* Panel = UIManager->GetQuestPanel();
        if (!Panel->IsValidLowLevel())
        {
            return TestResult(false, TEXT("任務面板無效"));
        }
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestQuestPanelDataUpdate()
{
    RunTest(TEXT("QuestPanelDataUpdate"), [this]()
    {
        if (!RelationshipManager || !UIManager)
        {
            return TestResult(false, TEXT("依賴組件未初始化"));
        }
        
        UMingQuestPanel* Panel = UIManager->GetQuestPanel();
        if (!Panel)
        {
            return TestResult(false, TEXT("任務面板未創建"));
        }
        
        // 測試數據更新
        Panel->UpdateQuestData(RelationshipManager);
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestQuestPanelFiltering()
{
    RunTest(TEXT("QuestPanelFiltering"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        UMingQuestPanel* Panel = UIManager->GetQuestPanel();
        if (!Panel)
        {
            return TestResult(false, TEXT("任務面板未創建"));
        }
        
        // 測試過濾功能
        Panel->SetQuestFilter(EQuestFilter::Active);
        Panel->SetQuestFilter(EQuestFilter::Completed);
        Panel->SetQuestFilter(EQuestFilter::All);
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestQuestPanelActions()
{
    RunTest(TEXT("QuestPanelActions"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        UMingQuestPanel* Panel = UIManager->GetQuestPanel();
        if (!Panel)
        {
            return TestResult(false, TEXT("任務面板未創建"));
        }
        
        // 測試任務操作
        Panel->AcceptQuest(TEXT("TestQuest"));
        Panel->TrackQuest(TEXT("TestQuest"));
        
        return TestResult(true);
    });
}

// 音頻面板測試
void UMingUISystemTest::TestAudioPanelInitialization()
{
    RunTest(TEXT("AudioPanelInitialization"), [this]()
    {
        if (!UIManager || !UIManager->GetAudioPanel())
        {
            return TestResult(false, TEXT("音頻面板未創建"));
        }
        
        UMingAudioPanel* Panel = UIManager->GetAudioPanel();
        if (!Panel->IsValidLowLevel())
        {
            return TestResult(false, TEXT("音頻面板無效"));
        }
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestAudioPanelVolumeControl()
{
    RunTest(TEXT("AudioPanelVolumeControl"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        UMingAudioPanel* Panel = UIManager->GetAudioPanel();
        if (!Panel)
        {
            return TestResult(false, TEXT("音頻面板未創建"));
        }
        
        // 測試音量控制
        Panel->SetMasterVolume(0.8f);
        Panel->SetMusicVolume(0.7f);
        Panel->SetSFXVolume(0.9f);
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestAudioPanelThemeSelection()
{
    RunTest(TEXT("AudioPanelThemeSelection"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        UMingAudioPanel* Panel = UIManager->GetAudioPanel();
        if (!Panel)
        {
            return TestResult(false, TEXT("音頻面板未創建"));
        }
        
        // 測試主題選擇
        Panel->SetTheme(TEXT("DefaultTheme"));
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestAudioPanelTestFeatures()
{
    RunTest(TEXT("AudioPanelTestFeatures"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        UMingAudioPanel* Panel = UIManager->GetAudioPanel();
        if (!Panel)
        {
            return TestResult(false, TEXT("音頻面板未創建"));
        }
        
        // 測試音頻測試功能
        Panel->TestRelationshipAudio(TEXT("TestCharacter"), 50.0f);
        Panel->TestReputationAudio(TEXT("TestRegion"), 60.0f);
        
        return TestResult(true);
    });
}

// 主儀表板測試
void UMingUISystemTest::TestMainDashboardInitialization()
{
    RunTest(TEXT("MainDashboardInitialization"), [this]()
    {
        if (!UIManager || !UIManager->GetMainDashboard())
        {
            return TestResult(false, TEXT("主儀表板未創建"));
        }
        
        UMingMainDashboard* Dashboard = UIManager->GetMainDashboard();
        if (!Dashboard->IsValidLowLevel())
        {
            return TestResult(false, TEXT("主儀表板無效"));
        }
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestMainDashboardPanelSwitching()
{
    RunTest(TEXT("MainDashboardPanelSwitching"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        UMingMainDashboard* Dashboard = UIManager->GetMainDashboard();
        if (!Dashboard)
        {
            return TestResult(false, TEXT("主儀表板未創建"));
        }
        
        // 測試面板切換
        Dashboard->ShowPanel(EDashboardPanel::Relationship);
        Dashboard->ShowPanel(EDashboardPanel::Reputation);
        Dashboard->ShowPanel(EDashboardPanel::Quest);
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestMainDashboardStatistics()
{
    RunTest(TEXT("MainDashboardStatistics"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        UMingMainDashboard* Dashboard = UIManager->GetMainDashboard();
        if (!Dashboard)
        {
            return TestResult(false, TEXT("主儀表板未創建"));
        }
        
        // 測試統計更新
        Dashboard->RefreshDashboard();
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestMainDashboardAutoRefresh()
{
    RunTest(TEXT("MainDashboardAutoRefresh"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        UMingMainDashboard* Dashboard = UIManager->GetMainDashboard();
        if (!Dashboard)
        {
            return TestResult(false, TEXT("主儀表板未創建"));
        }
        
        // 測試自動刷新
        Dashboard->SetAutoRefresh(true, 5.0f);
        
        return TestResult(true);
    });
}

// 集成測試
void UMingUISystemTest::TestPanelDataIntegration()
{
    RunTest(TEXT("PanelDataIntegration"), [this]()
    {
        if (!RelationshipManager || !UIManager)
        {
            return TestResult(false, TEXT("依賴組件未初始化"));
        }
        
        // 測試面板數據集成
        UIManager->UpdateAllPanels();
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestPanelEventIntegration()
{
    RunTest(TEXT("PanelEventIntegration"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        // 測試面板事件集成
        UIManager->BroadcastEvent(ETestEvent::DataUpdated);
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestCrossPanelCommunication()
{
    RunTest(TEXT("CrossPanelCommunication"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        // 測試跨面板通信
        UIManager->SendPanelMessage(ETestPanel::Relationship, TEXT("TestMessage"));
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestSystemWideUpdates()
{
    RunTest(TEXT("SystemWideUpdates"), [this]()
    {
        if (!UIManager)
        {
            return TestResult(false, TEXT("UI管理器未初始化"));
        }
        
        // 測試系統範圍更新
        UIManager->PerformSystemUpdate();
        
        return TestResult(true);
    });
}

// 性能測試
void UMingUISystemTest::TestUIPerformance()
{
    RunTest(TEXT("UIPerformance"), [this]()
    {
        double StartTime = FPlatformTime::Seconds();
        
        // 執行性能測試操作
        for (int32 i = 0; i < 1000; ++i)
        {
            if (UIManager)
            {
                UIManager->UpdateAllPanels();
            }
        }
        
        double ExecutionTime = FPlatformTime::Seconds() - StartTime;
        
        // 檢查性能要求（1000次更新應該在1秒內完成）
        if (ExecutionTime > 1.0)
        {
            return TestResult(false, FString::Printf(TEXT("UI性能測試失敗，執行時間: %.3f秒"), ExecutionTime));
        }
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestMemoryUsage()
{
    RunTest(TEXT("MemoryUsage"), [this]()
    {
        // 記憶體使用測試
        SIZE_T InitialMemory = FPlatformMemory::GetPhysicalUsedRAM();
        
        // 創建大量UI對象
        if (UIManager)
        {
            for (int32 i = 0; i < 100; ++i)
            {
                UIManager->CreatePanel(ETestPanel::Relationship);
                UIManager->DestroyPanel(ETestPanel::Relationship);
            }
        }
        
        SIZE_T FinalMemory = FPlatformMemory::GetPhysicalUsedRAM();
        SIZE_T MemoryIncrease = FinalMemory - InitialMemory;
        
        // 檢查記憶體增長是否合理（小於10MB）
        if (MemoryIncrease > 10 * 1024 * 1024)
        {
            return TestResult(false, FString::Printf(TEXT("記憶體使用測試失敗，增長: %d bytes"), MemoryIncrease));
        }
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestRenderingPerformance()
{
    RunTest(TEXT("RenderingPerformance"), [this]()
    {
        // 渲染性能測試
        double StartTime = FPlatformTime::Seconds();
        
        // 執行渲染密集操作
        if (UIManager)
        {
            UIManager->ForceRenderAllPanels();
        }
        
        double ExecutionTime = FPlatformTime::Seconds() - StartTime;
        
        // 檢查渲染性能（應該在100ms內完成）
        if (ExecutionTime > 0.1)
        {
            return TestResult(false, FString::Printf(TEXT("渲染性能測試失敗，執行時間: %.3f秒"), ExecutionTime));
        }
        
        return TestResult(true);
    });
}

void UMingUISystemTest::TestLargeDatasetHandling()
{
    RunTest(TEXT("LargeDatasetHandling"), [this]()
    {
        if (!RelationshipManager || !UIManager)
        {
            return TestResult(false, TEXT("依賴組件未初始化"));
        }
        
        double StartTime = FPlatformTime::Seconds();
        
        // 模拟大量數據處理
        UMingRelationshipPanel* Panel = UIManager->GetRelationshipPanel();
        if (Panel)
        {
            Panel->UpdateRelationshipData(RelationshipManager);
        }
        
        double ExecutionTime = FPlatformTime::Seconds() - StartTime;
        
        // 檢查大數據集處理性能（應該在500ms內完成）
        if (ExecutionTime > 0.5)
        {
            return TestResult(false, FString::Printf(TEXT("大數據集處理測試失敗，執行時間: %.3f秒"), ExecutionTime));
        }
        
        return TestResult(true);
    });
}

// 輔助函數
void UMingUISystemTest::RunTest(const FString& TestName, TFunction<FTestResult()> TestFunction)
{
    FTestResult Result = TestFunction();
    Result.TestName = TestName;
    
    TestResults->TotalTests++;
    
    if (Result.bPassed)
    {
        TestResults->PassedTests++;
        UE_LOG(LogTemp, Log, TEXT("✓ %s - 通過 (%.3f秒)"), *TestName, Result.ExecutionTime);
    }
    else
    {
        TestResults->FailedTests++;
        UE_LOG(LogTemp, Error, TEXT("✗ %s - 失敗: %s (%.3f秒)"), *TestName, *Result.ErrorMessage, Result.ExecutionTime);
    }
    
    TestResults->TestResults.Add(Result);
}

FTestResult UMingUISystemTest::TestResult(bool bPassed, const FString& ErrorMessage)
{
    FTestResult Result;
    Result.bPassed = bPassed;
    Result.ErrorMessage = ErrorMessage;
    Result.ExecutionTime = 0.0f;
    return Result;
}

void UMingUISystemTest::GenerateTestReport()
{
    FString ReportContent = TEXT("UI系統測試報告\n");
    ReportContent += TEXT("================\n\n");
    
    ReportContent += FString::Printf(TEXT("總測試數: %d\n"), TestResults->TotalTests);
    ReportContent += FString::Printf(TEXT("通過: %d\n"), TestResults->PassedTests);
    ReportContent += FString::Printf(TEXT("失敗: %d\n"), TestResults->FailedTests);
    ReportContent += FString::Printf(TEXT("跳過: %d\n"), TestResults->SkippedTests);
    ReportContent += FString::Printf(TEXT("成功率: %.1f%%\n\n"), 
        TestResults->TotalTests > 0 ? (float)TestResults->PassedTests / TestResults->TotalTests * 100.0f : 0.0f);
    
    ReportContent += TEXT("詳細結果:\n");
    ReportContent += TEXT("----------\n");
    
    for (const FTestResult& Result : TestResults->TestResults)
    {
        FString Status = Result.bPassed ? TEXT("通過") : TEXT("失敗");
        ReportContent += FString::Printf(TEXT("%s: %s"), *Result.TestName, *Status);
        
        if (!Result.bPassed)
        {
            ReportContent += FString::Printf(TEXT(" - %s"), *Result.ErrorMessage);
        }
        
        ReportContent += FString::Printf(TEXT(" (%.3f秒)\n"), Result.ExecutionTime);
    }
    
    // 保存報告到文件
    FString FileName = FString::Printf(TEXT("UI_Test_Report_%s.txt"), 
        *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
    
    if (FFileHelper::SaveStringToFile(ReportContent, *FileName))
    {
        UE_LOG(LogTemp, Log, TEXT("測試報告已保存到: %s"), *FileName);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("無法保存測試報告"));
    }
}
