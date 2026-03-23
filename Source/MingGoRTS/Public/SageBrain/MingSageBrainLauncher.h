// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SageBrain/MingSageBrainCompileOptimizer.h"
#include "SageBrain/MingSageBrainIntegratedAPI.h"
#include "SageBrain/MingSageBrainCommandAPI.h"
#include "TaskAnalysis/MingTaskAnalyzer.h"
#include "TaskSystem/MingSageBrainTaskSystem.h"
#include "MingSageBrainLauncher.generated.h"

/**
 * 聖者大腦系統啟動器
 * 提供C++方式啟用和執行聖者大腦編譯優化工作流程
 */
UCLASS(BlueprintType)
class MINGGORTS_API UMingSageBrainLauncher : public UObject
{
    GENERATED_BODY()

public:
    UMingSageBrainLauncher();

    // ==================== 系統啟用 ====================
    
    /**
     * 啟用聖者大腦系統
     * 初始化所有必要的子系統
     * @return 是否成功啟用
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|System")
    bool EnableSageBrain();

    /**
     * 檢查聖者大腦是否已啟用
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|System")
    bool IsSageBrainEnabled() const { return bSageBrainEnabled; }

    /**
     * 關閉聖者大腦系統
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|System")
    void DisableSageBrain();

    // ==================== 編譯優化工作流程 ====================

    /**
     * 執行完整編譯優化工作流程
     * 包含啟用、編譯、分析、建立任務、排序等所有步驟
     * @return 工作流程是否成功完成
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Workflow")
    bool RunCompleteWorkflow();

    /**
     * 執行編譯優化工作流程（異步版本）
     * 不會阻塞主線程，適合在編輯器中使用
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Workflow")
    void RunWorkflowAsync();

    /**
     * 僅分析現有日誌（不執行編譯）
     * @param LogPath 日誌檔案路徑
     * @return 分析結果
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Workflow")
    bool AnalyzeExistingLog(const FString& LogPath);

    // ==================== 工作流程控制 ====================

    /**
     * 設定編譯模式
     * @param Mode 編譯模式 (Debug, Development, Shipping, Test)
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Config")
    void SetBuildMode(const FString& Mode);

    /**
     * 設定是否跳過清理（增量編譯）
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Config")
    void SetSkipClean(bool bSkip) { bSkipClean = bSkip; }

    /**
     * 設定是否自動修復錯誤
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Config")
    void SetAutoFix(bool bAuto) { bAutoFix = bAuto; }

    /**
     * 設定是否建立任務到需求單系統
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Config")
    void SetCreateTasks(bool bCreate) { bCreateTasks = bCreate; }

    // ==================== 狀態查詢 ====================

    /**
     * 獲取最後一次工作流程報告
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Status")
    FCompileOptimizationReport GetLastReport() const { return LastReport; }

    /**
     * 獲取生成的任務清單
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Status")
    TArray<FMingTaskRequirement> GetGeneratedTasks() const { return GeneratedTasks; }

    /**
     * 獲取排序後的任務清單
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Status")
    TArray<FMingTaskRequirement> GetSortedTasks() const { return SortedTasks; }

    /**
     * 工作流程是否正在執行中
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Status")
    bool IsWorkflowRunning() const { return bWorkflowRunning; }

    // ==================== 事件委派 ====================

    /** 聖者大腦啟用事件 */
    UPROPERTY(BlueprintAssignable, Category = "SageBrain|Events")
    FSageBrainEnabledDelegate OnSageBrainEnabled;

    /** 工作流程開始事件 */
    UPROPERTY(BlueprintAssignable, Category = "SageBrain|Events")
    FWorkflowStartedDelegate OnWorkflowStarted;

    /** 工作流程完成事件 */
    UPROPERTY(BlueprintAssignable, Category = "SageBrain|Events")
    FWorkflowCompletedDelegate OnWorkflowCompleted;

    /** 編譯階段開始事件 */
    UPROPERTY(BlueprintAssignable, Category = "SageBrain|Events")
    FCompilePhaseStartedDelegate OnPhaseStarted;

    /** 編譯階段完成事件 */
    UPROPERTY(BlueprintAssignable, Category = "SageBrain|Events")
    FCompilePhaseCompletedDelegate OnPhaseCompleted;

    /** 任務生成事件 */
    UPROPERTY(BlueprintAssignable, Category = "SageBrain|Events")
    FTasksCreatedDelegate OnTasksCreated;

    /** 進度更新事件 */
    UPROPERTY(BlueprintAssignable, Category = "SageBrain|Events")
    FProgressUpdatedDelegate OnProgressUpdated;

protected:
    // 工作流程步驟
    bool Step1_EnableSageBrain();
    bool Step2_DetermineStrategy();
    bool Step3_ExecuteCompilation();
    bool Step4_CollectLogs();
    bool Step5_AnalyzeLogs();
    bool Step6_CreateTasks();
    bool Step7_SortTasks();
    void Step8_GenerateReport();

    // 輔助函數
    void LogLauncher(const FString& Message);
    void UpdateProgress(int32 CurrentStep, int32 TotalSteps, const FString& Message);
    FString GetBuildScriptPath() const;
    FString GetProjectPath() const;

private:
    // 子系統引用
    UPROPERTY()
    TObjectPtr<UMingSageBrainCompileOptimizer> CompileOptimizer;

    UPROPERTY()
    TObjectPtr<UMingSageBrainIntegratedAPI> IntegratedAPI;

    // 狀態標記
    bool bSageBrainEnabled;
    bool bWorkflowRunning;

    // 配置
    FString BuildMode;
    bool bSkipClean;
    bool bAutoFix;
    bool bCreateTasks;

    // 結果資料
    FCompileOptimizationReport LastReport;
    TArray<FMingTaskRequirement> GeneratedTasks;
    TArray<FMingTaskRequirement> SortedTasks;
    FString LastLogPath;
    FString TaskListPath;
};

// ==================== 事件委派類型定義 ====================

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSageBrainEnabledDelegate, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWorkflowStartedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWorkflowCompletedDelegate, bool, bSuccess, const FCompileOptimizationReport&, Report);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCompilePhaseStartedDelegate, ECompilePhase, Phase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCompilePhaseCompletedDelegate, const FCompilePhaseResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTasksCreatedDelegate, int32, TaskCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProgressUpdatedDelegate, int32, Percentage, const FString&, Message);
