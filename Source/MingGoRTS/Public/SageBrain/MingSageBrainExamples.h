// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 聖者大腦系統 C++ 使用範例
 * 
 * 展示如何以純 C++ 方式啟用聖者大腦系統並執行編譯優化工作流程
 */

class FEnableSageBrainExample
{
public:
    /**
     * 範例 1: 最簡單的啟用方式
     * 一行代碼啟用聖者大腦並執行完整工作流程
     */
    static void Example1_SimpleEnable()
    {
        #include "SageBrain/MingSageBrainLauncher.h"
        
        // 創建啟動器
        UMingSageBrainLauncher* Launcher = NewObject<UMingSageBrainLauncher>();
        
        // 啟用聖者大腦
        if (Launcher->EnableSageBrain())
        {
            UE_LOG(LogTemp, Log, TEXT("聖者大腦系統已成功啟用！"));
        }
        
        // 執行完整工作流程
        bool bSuccess = Launcher->RunCompleteWorkflow();
        
        if (bSuccess)
        {
            UE_LOG(LogTemp, Log, TEXT("工作流程執行成功！"));
        }
    }

    /**
     * 範例 2: 帶事件監聽的啟用方式
     * 監聽所有工作流程事件
     */
    static void Example2_WithEventListening(UObject* Outer)
    {
        #include "SageBrain/MingSageBrainLauncher.h"
        
        UMingSageBrainLauncher* Launcher = NewObject<UMingSageBrainLauncher>(Outer);
        
        // 綁定事件
        Launcher->OnSageBrainEnabled.AddLambda([](bool bSuccess)
        {
            UE_LOG(LogTemp, Log, TEXT("聖者大腦啟用狀態: %s"), 
                bSuccess ? TEXT("成功") : TEXT("失敗"));
        });
        
        Launcher->OnWorkflowStarted.AddLambda([]()
        {
            UE_LOG(LogTemp, Log, TEXT("工作流程開始..."));
        });
        
        Launcher->OnPhaseStarted.AddLambda([](ECompilePhase Phase)
        {
            UE_LOG(LogTemp, Log, TEXT("開始階段: %d"), static_cast<int32>(Phase));
        });
        
        Launcher->OnPhaseCompleted.AddLambda([](const FCompilePhaseResult& Result)
        {
            UE_LOG(LogTemp, Log, TEXT("階段完成: %s - %s"),
                *UEnum::GetValueAsString(Result.Phase),
                Result.bSuccess ? TEXT("成功") : TEXT("失敗"));
        });
        
        Launcher->OnTasksCreated.AddLambda([](int32 TaskCount)
        {
            UE_LOG(LogTemp, Log, TEXT("已建立 %d 個任務"), TaskCount);
        });
        
        Launcher->OnWorkflowCompleted.AddLambda([](bool bSuccess, const FCompileOptimizationReport& Report)
        {
            UE_LOG(LogTemp, Log, TEXT("工作流程完成 - 成功: %s, 錯誤: %d, 警告: %d"),
                bSuccess ? TEXT("是") : TEXT("否"),
                Report.TotalErrors,
                Report.TotalWarnings);
        });
        
        Launcher->OnProgressUpdated.AddLambda([](int32 Percentage, const FString& Message)
        {
            UE_LOG(LogTemp, Log, TEXT("[%d%%] %s"), Percentage, *Message);
        });
        
        // 執行
        Launcher->RunCompleteWorkflow();
    }

    /**
     * 範例 3: 異步執行（不阻塞主線程）
     * 適合在編輯器 UI 中使用
     */
    static void Example3_AsyncExecution()
    {
        #include "SageBrain/MingSageBrainLauncher.h"
        
        UMingSageBrainLauncher* Launcher = NewObject<UMingSageBrainLauncher>();
        
        // 綁定進度更新事件用於 UI 顯示
        Launcher->OnProgressUpdated.AddLambda([](int32 Percentage, const FString& Message)
        {
            // 更新 UI 進度條
            UE_LOG(LogTemp, Log, TEXT("UI 進度: [%d%%] %s"), Percentage, *Message);
        });
        
        // 異步執行
        Launcher->RunWorkflowAsync();
        
        // 主線程立即返回，工作流程在背景執行
        UE_LOG(LogTemp, Log, TEXT("工作流程已在背景啟動"));
    }

    /**
     * 範例 4: 僅分析現有日誌
     * 不重新編譯，只分析已存在的日誌
     */
    static void Example4_AnalyzeOnly()
    {
        #include "SageBrain/MingSageBrainLauncher.h"
        
        UMingSageBrainLauncher* Launcher = NewObject<UMingSageBrainLauncher>();
        
        // 設定只分析，不建立任務
        Launcher->SetCreateTasks(false);
        
        // 分析現有日誌
        FString LogPath = FPaths::ProjectSavedDir() / TEXT("Logs\Compile_20260323_120000.log");
        bool bSuccess = Launcher->AnalyzeExistingLog(LogPath);
        
        if (bSuccess)
        {
            // 獲取生成的任務
            TArray<FMingTaskRequirement> Tasks = Launcher->GetGeneratedTasks();
            
            for (const auto& Task : Tasks)
            {
                UE_LOG(LogTemp, Log, TEXT("任務: %s [優先級: %d]"), 
                    *Task.Title, 
                    static_cast<int32>(Task.Priority));
            }
        }
    }

    /**
     * 範例 5: 完整配置的工作流程
     * 設定所有配置選項後執行
     */
    static void Example5_FullConfiguration()
    {
        #include "SageBrain/MingSageBrainLauncher.h"
        
        UMingSageBrainLauncher* Launcher = NewObject<UMingSageBrainLauncher>();
        
        // 配置工作流程
        Launcher->SetBuildMode(TEXT("Development"));  // 編譯模式
        Launcher->SetSkipClean(false);                // 不跳過清理
        Launcher->SetAutoFix(true);                   // 自動修復錯誤
        Launcher->SetCreateTasks(true);              // 建立任務到需求單系統
        
        // 綁定完成事件以獲取結果
        Launcher->OnWorkflowCompleted.AddLambda([Launcher](bool bSuccess, const FCompileOptimizationReport& Report)
        {
            UE_LOG(LogTemp, Log, TEXT("編譯結果: %s"), 
                bSuccess ? TEXT("成功") : TEXT("失敗"));
            UE_LOG(LogTemp, Log, TEXT("總耗時: %.2f 秒"), Report.TotalDuration);
            UE_LOG(LogTemp, Log, TEXT("錯誤數: %d"), Report.TotalErrors);
            UE_LOG(LogTemp, Log, TEXT("警告數: %d"), Report.TotalWarnings);
            
            // 獲取排序後的任務清單
            TArray<FMingTaskRequirement> SortedTasks = Launcher->GetSortedTasks();
            
            for (int32 i = 0; i < FMath::Min(10, SortedTasks.Num()); ++i)
            {
                const auto& Task = SortedTasks[i];
                UE_LOG(LogTemp, Log, TEXT("[%d] %s (優先級: %d, 預估: %.1fh)"),
                    i + 1,
                    *Task.Title,
                    static_cast<int32>(Task.Priority),
                    Task.EstimatedHours);
            }
        });
        
        // 執行工作流程
        Launcher->RunCompleteWorkflow();
    }

    /**
     * 範例 6: 在 GameMode 中整合
     * 在遊戲開始時自動啟用聖者大腦
     */
    static void Example6_InGameMode()
    {
        // 在 GameMode 的 BeginPlay 中調用
        /*
        void AMyGameMode::BeginPlay()
        {
            Super::BeginPlay();
            
            // 檢查是否為開發模式
            #if WITH_EDITOR
            // 啟用聖者大腦系統
            SageBrainLauncher = NewObject<UMingSageBrainLauncher>(this);
            if (SageBrainLauncher->EnableSageBrain())
            {
                UE_LOG(LogTemp, Log, TEXT("聖者大腦系統已啟用"));
            }
            #endif
        }
        */
    }

    /**
     * 範例 7: 在 Editor Module 中整合
     * 創建自定義編輯器命令
     */
    static void Example7_EditorCommand()
    {
        // 在編輯器模組中註冊命令
        /*
        void FMyEditorModule::StartupModule()
        {
            // 註冊命令
            FUICommandInfo::RegisterCommandContext(
                TEXT("SageBrainCommands"),
                NSLOCTEXT("SageBrain", "SageBrainCommands", "聖者大腦命令")
            );
            
            // 添加菜單項
            FToolMenuOwnerScoped OwnerScoped(this);
            UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Build");
            FToolMenuSection& Section = Menu->FindOrAddSection("SageBrain");
            
            Section.AddMenuEntry(
                TEXT("EnableSageBrain"),
                NSLOCTEXT("SageBrain", "EnableSageBrain", "啟用聖者大腦系統"),
                NSLOCTEXT("SageBrain", "EnableSageBrainTooltip", "啟用聖者大腦並執行編譯優化"),
                FSlateIcon(),
                FUIAction(FExecuteAction::CreateLambda([]()
                {
                    UMingSageBrainLauncher* Launcher = NewObject<UMingSageBrainLauncher>();
                    Launcher->RunCompleteWorkflow();
                }))
            );
        }
        */
    }

    /**
     * 範例 8: 檢查系統狀態
     * 查詢聖者大腦各子系統的狀態
     */
    static void Example8_CheckSystemStatus()
    {
        #include "SageBrain/MingSageBrainLauncher.h"
        #include "SageBrain/MingSageBrainIntegratedAPI.h"
        
        UMingSageBrainLauncher* Launcher = NewObject<UMingSageBrainLauncher>();
        
        // 檢查是否已啟用
        if (Launcher->IsSageBrainEnabled())
        {
            UE_LOG(LogTemp, Log, TEXT("聖者大腦系統狀態: 已啟用"));
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("聖者大腦系統狀態: 未啟用"));
        }
        
        // 檢查工作流程狀態
        if (Launcher->IsWorkflowRunning())
        {
            UE_LOG(LogTemp, Log, TEXT("工作流程狀態: 執行中"));
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("工作流程狀態: 閒置"));
        }
        
        // 啟用系統
        if (Launcher->EnableSageBrain())
        {
            UE_LOG(LogTemp, Log, TEXT("聖者大腦系統已啟用！"));
        }
    }
};

/**
 * 快捷啟用宏
 * 提供最簡潔的啟用方式
 */
#define ENABLE_SAGE_BRAIN() \
    [&]() -> bool { \
        UMingSageBrainLauncher* __Launcher = NewObject<UMingSageBrainLauncher>(); \
        return __Launcher->RunCompleteWorkflow(); \
    }()

/**
 * 快捷啟用宏（帶結果獲取）
 */
#define ENABLE_SAGE_BRAIN_WITH_REPORT(Report) \
    [&]() -> bool { \
        UMingSageBrainLauncher* __Launcher = NewObject<UMingSageBrainLauncher>(); \
        bool __Result = __Launcher->RunCompleteWorkflow(); \
        Report = __Launcher->GetLastReport(); \
        return __Result; \
    }()

/**
 * 實際使用範例：
 * 
 * // 在任何 C++ 代碼中：
 * 
 * void SomeFunction()
 * {
 *     // 最簡單的方式
 *     bool bSuccess = ENABLE_SAGE_BRAIN();
 *     
 *     // 或者使用啟動器物件
 *     UMingSageBrainLauncher* Launcher = NewObject<UMingSageBrainLauncher>();
 *     Launcher->RunCompleteWorkflow();
 * }
 */
