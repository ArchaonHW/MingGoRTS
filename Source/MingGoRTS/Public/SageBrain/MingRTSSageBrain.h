#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Containers/Queue.h"
#include "Containers/Map.h"
#include "Containers/Set.h"
#include "Async/Async.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "MingRTSSageCommandAPI.h"
#include "MingRTSSageIntegrationAPI.h"
#include "MingBMADProjectPlanner.h"
#include "MingScriptExecutionEngine.h"
#include "MingRTSAPISystem.h"
#include "MingTaskAnalyzer.h"
#include "MingSaveGameManager.h"
#include "MingAutoSceneGenerator.h"
#include "MingRTSSageBrain.generated.h"

/**
 * 安全防護體系狀態枚舉
 */
UENUM(BlueprintType)
enum class ESageSecurityStatus : uint8
{
    Inactive,           // 未啟動
    Active,            // 運行中
    Suspended,         // 暫停
    Emergency,         // 緊急狀態
    Maintenance        // 維護模式
};

/**
 * 聖者大腦核心系統
 * 統一管理所有AI決策、學習和自我進化功能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSSageBrain : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSSageBrain();

    // 初始化聖者大腦
    UFUNCTION(BlueprintCallable, Category = "Sage Brain")
    void InitializeSageBrain();

    // 啟動安全防護
    UFUNCTION(BlueprintCallable, Category = "Sage Brain")
    void ActivateSecurityProtection();

    // 執行AI決策
    UFUNCTION(BlueprintCallable, Category = "Sage Brain")
    void ExecuteAIDecision(const FString& Context);

    // 學習新知識
    UFUNCTION(BlueprintCallable, Category = "Sage Brain")
    void LearnNewKnowledge(const FString& Knowledge);

    // 自我進化
    UFUNCTION(BlueprintCallable, Category = "Sage Brain")
    void EvolveSelf();

    // 獲取大腦狀態
    UFUNCTION(BlueprintPure, Category = "Sage Brain")
    ESageSecurityStatus GetBrainStatus();

    // 獲取決策結果
    UFUNCTION(BlueprintPure, Category = "Sage Brain")
    FString GetLastDecisionResult();

protected:
    // 安全防護狀態
    UPROPERTY(BlueprintReadOnly, Category = "Sage Brain")
    ESageSecurityStatus SecurityStatus;

    // 知識庫
    UPROPERTY(BlueprintReadOnly, Category = "Sage Brain")
    TMap<FString, FString> KnowledgeBase;

    // 決策歷史
    UPROPERTY(BlueprintReadOnly, Category = "Sage Brain")
    TArray<FString> DecisionHistory;

    // 當前決策上下文
    UPROPERTY(BlueprintReadOnly, Category = "Sage Brain")
    FString CurrentContext;

private:
    // 內部方法
    void ProcessDecision(const FString& Context);
    void UpdateKnowledgeBase();
    void AnalyzeDecisionResults();
    void IntegrateWisdomAcrossSystems();
    void ApplyWisdomToDecisionMaking();
    
    // 輔助方法
    void LogBrainActivity(const FString& Activity);
    void SaveBrainState();
    void LoadBrainState();
    void PerformBrainMaintenance();
    void OptimizeBrainPerformance();
};
