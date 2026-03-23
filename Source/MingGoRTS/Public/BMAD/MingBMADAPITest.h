#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSBMADSystem.h"
#include "MingBMADAPITest.generated.h"

/**
 * MingGoRTS BMAD API 測試套件
 * 
 * 全面測試 BMAD (Brain Multiple Agent Discussion) 系統的功能
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|BMAD API Test")
class MINGGORTS_API UMingBMADAPITest : public UObject
{
    GENERATED_BODY()

public:
    UMingBMADAPITest();

    // 運行所有測試
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool RunAllTests();

    // 測試 BMAD 系統初始化
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestBMADSystemInitialization();

    // 測試 AI 分身創建
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestAgentCreation();

    // 測試 AI 分身管理
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestAgentManagement();

    // 測試討論會話創建
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestDiscussionSessionCreation();

    // 測試消息發送和接收
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestMessageExchange();

    // 測試情緒狀態管理
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestEmotionStateManagement();

    // 測試洞見提取
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestInsightExtraction();

    // 測試決策生成
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestDecisionGeneration();

    // 測試行動項目創建
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestActionItemCreation();

    // 測試討論質量分析
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestDiscussionQualityAnalysis();

    // 測試參與者貢獻評估
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestParticipantContributionEvaluation();

    // 測試專家分身查找
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestExpertAgentFinding();

    // 測試專業討論組創建
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestExpertDiscussionGroupCreation();

    // 測試分身互動模擬
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestAgentInteractionSimulation();

    // 測試學習和適應
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestLearningAndAdaptation();

    // 測試討論記錄導入導出
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestDiscussionHistoryImportExport();

    // 測試事件系統
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestEventSystem();

    // 測試並發會話處理
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestConcurrentSessionHandling();

    // 測試系統性能
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestSystemPerformance();

    // 測試錯誤處理
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestErrorHandling();

    // 測試數據持久化
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool TestDataPersistence();

    // 生成測試報告
    UFUNCTION(BlueprintCallable, Category = "BMAD API Test")
    bool GenerateTestReport(const FString& ReportPath);

protected:
    // 記錄測試結果
    void LogTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT(""));

    // 創建測試分身配置
    FBMADAgentProfile CreateTestAgentProfile(EBMADAgentRole Role);

    // 創建測試討論會話
    FBMADDiscussionSession CreateTestDiscussionSession();

    // 驗證分身配置
    bool ValidateAgentProfile(const FBMADAgentProfile& Profile);

    // 驗證討論會話
    bool ValidateDiscussionSession(const FBMADDiscussionSession& Session);

    // 等待異步操作
    void WaitForAsyncOperation(float WaitTime = 1.0f);

private:
    // 測試結果
    UPROPERTY()
    TMap<FString, bool> TestResults;

    // 總測試數
    int32 TotalTests;

    // 通過測試數
    int32 PassedTests;

    // 失敗測試數
    int32 FailedTests;

    // BMAD 系統實例
    UPROPERTY()
    UMingRTSBMADSystem* BMADSystem;

    // 測試會話 ID 列表
    UPROPERTY()
    TArray<FString> TestSessionIDs;

    // 測試分身 ID 列表
    UPROPERTY()
    TArray<FString> TestAgentIDs;
};
