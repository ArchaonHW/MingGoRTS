#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSageBrainBMADIntegration.generated.h"

// Forward declarations
class UMingSageBrainIntegratedAPI;
class UMingRTSBMADSystem;
class UMingSageBrainCoreSystem;

UENUM(BlueprintType)
enum class ESageBrainBMADRole : uint8
{
    SageMentor,         // 聖者導師 - 提供哲學指導
    StrategicAdvisor,   // 戰略顧問 - 提供戰略建議
    CulturalExpert,     // 文化專家 - 提供文化見解
    InnovationLeader,   // 創新領袖 - 引導創新思考
    DecisionMaker,    // 決策者 - 協助做出關鍵決策
    LearningFacilitator // 學習促進者 - 促進知識傳遞
};

UENUM(BlueprintType)
enum class EIntegrationMode : uint8
{
    Passive,            // 被動模式 - BMAD請求時聖者才回應
    Active,             // 主動模式 - 聖者主動參與討論
    Collaborative,      // 協作模式 - 雙向深度協作
    Directive,          // 指導模式 - 聖者指導BMAD討論
    Autonomous          // 自主模式 - 完全自主規劃
};

UENUM(BlueprintType)
enum class EDevelopmentPhase : uint8
{
    Planning,           // 規劃階段
    Design,             // 設計階段
    Implementation,     // 實現階段
    Testing,            // 測試階段
    Review,             // 審查階段
    Deployment,         // 部署階段
    Optimization        // 優化階段
};

USTRUCT(BlueprintType)
struct FSageBrainBMADMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString MessageID;

    UPROPERTY(BlueprintReadWrite)
    FString SourceSystem;  // "SageBrain" 或 "BMAD"

    UPROPERTY(BlueprintReadWrite)
    FString SourceAgentID;

    UPROPERTY(BlueprintReadWrite)
    FString TargetAgentID;

    UPROPERTY(BlueprintReadWrite)
    FString Content;

    UPROPERTY(BlueprintReadWrite)
    ESageBrainBMADRole Role;

    UPROPERTY(BlueprintReadWrite)
    EDevelopmentPhase ContextPhase;

    UPROPERTY(BlueprintReadWrite)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadWrite)
    float Priority;

    UPROPERTY(BlueprintReadWrite)
    TArray<FString> Tags;

    FSageBrainBMADMessage()
    {
        MessageID = TEXT("");
        SourceSystem = TEXT("");
        SourceAgentID = TEXT("");
        TargetAgentID = TEXT("");
        Content = TEXT("");
        Role = ESageBrainBMADRole::SageMentor;
        ContextPhase = EDevelopmentPhase::Planning;
        Timestamp = FDateTime::Now();
        Priority = 0.5f;
        Tags.Empty();
    }
};

USTRUCT(BlueprintType)
struct FDevelopmentWorkflow
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString WorkflowID;

    UPROPERTY(BlueprintReadWrite)
    FString WorkflowName;

    UPROPERTY(BlueprintReadWrite)
    EDevelopmentPhase CurrentPhase;

    UPROPERTY(BlueprintReadWrite)
    TArray<EDevelopmentPhase> PlannedPhases;

    UPROPERTY(BlueprintReadWrite)
    FString ProjectContext;

    UPROPERTY(BlueprintReadWrite)
    TArray<FString> RequiredExpertise;

    UPROPERTY(BlueprintReadWrite)
    TArray<FSageBrainBMADMessage> WorkflowMessages;

    UPROPERTY(BlueprintReadWrite)
    bool bIsActive;

    UPROPERTY(BlueprintReadWrite)
    FDateTime StartTime;

    UPROPERTY(BlueprintReadWrite)
    FDateTime EstimatedEndTime;

    FDevelopmentWorkflow()
    {
        WorkflowID = TEXT("");
        WorkflowName = TEXT("");
        CurrentPhase = EDevelopmentPhase::Planning;
        ProjectContext = TEXT("");
        RequiredExpertise.Empty();
        WorkflowMessages.Empty();
        bIsActive = false;
        StartTime = FDateTime::Now();
        EstimatedEndTime = FDateTime::Now();
    }
};

USTRUCT(BlueprintType)
struct FDevelopmentDecision
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString DecisionID;

    UPROPERTY(BlueprintReadWrite)
    FString DecisionContent;

    UPROPERTY(BlueprintReadWrite)
    FString Rationale;

    UPROPERTY(BlueprintReadWrite)
    TArray<FString> SupportingAgents;

    UPROPERTY(BlueprintReadWrite)
    float Confidence;

    UPROPERTY(BlueprintReadWrite)
    FDateTime DecisionTime;

    UPROPERTY(BlueprintReadWrite)
    EDevelopmentPhase Phase;

    FDevelopmentDecision()
    {
        DecisionID = TEXT("");
        DecisionContent = TEXT("");
        Rationale = TEXT("");
        SupportingAgents.Empty();
        Confidence = 0.0f;
        DecisionTime = FDateTime::Now();
        Phase = EDevelopmentPhase::Planning;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSageBrainMessageToBMAD, const FSageBrainBMADMessage&, Message, const FString&, TargetSessionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBMADMessageToSageBrain, const FSageBrainBMADMessage&, Message, const FString&, SourceAgentID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWorkflowPhaseChanged, const FString&, WorkflowID, EDevelopmentPhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDevelopmentDecisionMade, const FDevelopmentDecision&, Decision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIntegrationModeChanged, EIntegrationMode, NewMode, const FString&, Reason);

/**
 * 聖者大腦與BMAD集成系統
 * 實現兩個AI系統的雙向對話和協作規劃
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingSageBrainBMADIntegration : public UObject
{
    GENERATED_BODY()

public:
    UMingSageBrainBMADIntegration();

    // ========================================
    // 初始化和配置
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration")
    bool InitializeIntegration();

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration")
    void ShutdownIntegration();

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration")
    void SetIntegrationMode(EIntegrationMode Mode);

    UFUNCTION(BlueprintPure, Category = "SageBrain-BMAD Integration")
    EIntegrationMode GetCurrentIntegrationMode() const { return CurrentMode; }

    UFUNCTION(BlueprintPure, Category = "SageBrain-BMAD Integration")
    bool IsIntegrationActive() const { return bIsIntegrationActive; }

    // ========================================
    // 系統註冊和連接
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration")
    bool RegisterSageBrain(UMingSageBrainIntegratedAPI* SageBrainAPI);

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration")
    bool RegisterBMADSystem(UMingRTSBMADSystem* BMADSystem);

    UFUNCTION(BlueprintPure, Category = "SageBrain-BMAD Integration")
    UMingSageBrainIntegratedAPI* GetRegisteredSageBrain() const { return SageBrainAPI; }

    UFUNCTION(BlueprintPure, Category = "SageBrain-BMAD Integration")
    UMingRTSBMADSystem* GetRegisteredBMADSystem() const { return BMADSystem; }

    // ========================================
    // 雙向消息傳遞
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Communication")
    bool SendMessageFromSageBrainToBMAD(const FSageBrainBMADMessage& Message, const FString& BMADSessionID);

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Communication")
    bool SendMessageFromBMADToSageBrain(const FSageBrainBMADMessage& Message);

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Communication")
    bool BroadcastSageBrainInsightToBMAD(const FString& Insight, ESageBrainBMADRole Role, const FString& BMADSessionID);

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Communication")
    bool RequestSageBrainGuidance(const FString& BMADAgentID, const FString& Question, EDevelopmentPhase Phase);

    // ========================================
    // 開發流程規劃
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Workflow")
    FString CreateDevelopmentWorkflow(const FString& WorkflowName, const FString& ProjectContext, EDevelopmentPhase StartPhase);

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Workflow")
    bool StartWorkflowPlanning(const FString& WorkflowID);

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Workflow")
    bool AdvanceToNextPhase(const FString& WorkflowID);

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Workflow")
    bool SetWorkflowPhase(const FString& WorkflowID, EDevelopmentPhase NewPhase);

    UFUNCTION(BlueprintPure, Category = "SageBrain-BMAD Integration|Workflow")
    FDevelopmentWorkflow GetWorkflow(const FString& WorkflowID) const;

    UFUNCTION(BlueprintPure, Category = "SageBrain-BMAD Integration|Workflow")
    TArray<FDevelopmentWorkflow> GetAllActiveWorkflows() const;

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Workflow")
    bool EndWorkflow(const FString& WorkflowID);

    // ========================================
    // 智能決策協作
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Decision")
    FString ProposeDecision(const FString& ProposerID, const FString& DecisionContent, const FString& Rationale);

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Decision")
    bool EvaluateDecisionWithSageBrain(const FString& DecisionID);

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Decision")
    bool FinalizeDecision(const FString& DecisionID, bool bApproved);

    UFUNCTION(BlueprintPure, Category = "SageBrain-BMAD Integration|Decision")
    TArray<FDevelopmentDecision> GetPendingDecisions() const;

    UFUNCTION(BlueprintPure, Category = "SageBrain-BMAD Integration|Decision")
    TArray<FDevelopmentDecision> GetApprovedDecisions() const;

    // ========================================
    // 專業角色協作
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Roles")
    bool AssignSageBrainRole(ESageBrainBMADRole Role);

    UFUNCTION(BlueprintPure, Category = "SageBrain-BMAD Integration|Roles")
    ESageBrainBMADRole GetCurrentSageBrainRole() const { return CurrentRole; }

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Roles")
    bool CreateSpecializedDiscussion(const FString& Topic, EDevelopmentPhase Phase, ESageBrainBMADRole SageRole);

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Roles")
    bool InviteSageBrainToSession(const FString& BMADSessionID, ESageBrainBMADRole Role);

    // ========================================
    // 哲學和文化整合
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Philosophy")
    bool ApplyPhilosophicalPrinciplesToDiscussion(const FString& BMADSessionID, ESageBrainPhilosophy Philosophy);

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Philosophy")
    bool GenerateCulturalContextForWorkflow(const FString& WorkflowID, const FString& CulturalTheme);

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Philosophy")
    TArray<FString> GetPhilosophicalRecommendations(const FString& Context);

    // ========================================
    // 學習和適應
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Learning")
    bool LearnFromWorkflow(const FString& WorkflowID);

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Learning")
    bool OptimizeIntegrationBasedOnHistory();

    UFUNCTION(BlueprintPure, Category = "SageBrain-BMAD Integration|Learning")
    float GetIntegrationEfficiency() const;

    // ========================================
    // 狀態和報告
    // ========================================

    UFUNCTION(BlueprintPure, Category = "SageBrain-BMAD Integration|Status")
    FString GetIntegrationStatus() const;

    UFUNCTION(BlueprintPure, Category = "SageBrain-BMAD Integration|Status")
    TMap<FString, int32> GetCommunicationStatistics() const;

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Status")
    FString GenerateIntegrationReport();

    UFUNCTION(BlueprintCallable, Category = "SageBrain-BMAD Integration|Status")
    void ResetIntegrationStatistics();

    // ========================================
    // 事件委託
    // ========================================

    UPROPERTY(BlueprintAssignable, Category = "SageBrain-BMAD Integration|Events")
    FOnSageBrainMessageToBMAD OnSageBrainMessageToBMAD;

    UPROPERTY(BlueprintAssignable, Category = "SageBrain-BMAD Integration|Events")
    FOnBMADMessageToSageBrain OnBMADMessageToSageBrain;

    UPROPERTY(BlueprintAssignable, Category = "SageBrain-BMAD Integration|Events")
    FOnWorkflowPhaseChanged OnWorkflowPhaseChanged;

    UPROPERTY(BlueprintAssignable, Category = "SageBrain-BMAD Integration|Events")
    FOnDevelopmentDecisionMade OnDevelopmentDecisionMade;

    UPROPERTY(BlueprintAssignable, Category = "SageBrain-BMAD Integration|Events")
    FOnIntegrationModeChanged OnIntegrationModeChanged;

private:
    // 系統引用
    UPROPERTY()
    TObjectPtr<UMingSageBrainIntegratedAPI> SageBrainAPI;

    UPROPERTY()
    TObjectPtr<UMingRTSBMADSystem> BMADSystem;

    // 狀態
    UPROPERTY()
    bool bIsIntegrationActive;

    UPROPERTY()
    EIntegrationMode CurrentMode;

    UPROPERTY()
    ESageBrainBMADRole CurrentRole;

    // 數據存儲
    UPROPERTY()
    TMap<FString, FDevelopmentWorkflow> ActiveWorkflows;

    UPROPERTY()
    TMap<FString, FDevelopmentDecision> PendingDecisions;

    UPROPERTY()
    TMap<FString, FDevelopmentDecision> ApprovedDecisions;

    UPROPERTY()
    TArray<FSageBrainBMADMessage> MessageHistory;

    // 統計
    UPROPERTY()
    int32 TotalMessagesExchanged;

    UPROPERTY()
    int32 SuccessfulWorkflows;

    UPROPERTY()
    int32 DecisionsMade;

    // 內部方法
    void OnBMADMessageReceived(const FString& SessionID, const FBMADMessage& Message);
    void OnSageBrainDecisionMade(const FString& Context, const FString& Decision);
    void ProcessBidirectionalCommunication();
    void CoordinateDevelopmentPlanning();
    void FacilitateDecisionMaking();
    void SynchronizeLearningData();
    void HandleWorkflowPhaseTransition(const FString& WorkflowID, EDevelopmentPhase NewPhase);
    void GenerateSageBrainResponse(const FString& TriggerMessage, const FString& BMADSessionID);
    void UpdateIntegrationMetrics();
    void LogIntegrationActivity(const FString& Activity);
};
