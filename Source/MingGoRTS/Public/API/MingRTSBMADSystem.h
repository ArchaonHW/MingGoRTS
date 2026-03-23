// Copyright (c) 2026 MingGoRTS. All rights reserved.
// BMAD System - Brain Multiple Agent Discussion System

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRTSBMADSystem.generated.h"

UENUM(BlueprintType)
enum class EBMADAgentRole : uint8
{
    Architect,          // 架構師 - 負責系統架構設計
    Developer,          // 開發者 - 負責代碼實現
    Tester,             // 測試者 - 負責質量保證
    Analyst,            // 分析師 - 負責需求分析
    Designer,            // 設計師 - 負責用戶體驗
    Security,            // 安全專家 - 負責安全審查
    Performance,         // 性能專家 - 負責性能優化
    Documentation,      // 文檔專家 - 負責文檔編寫
    Coordinator,         // 協調者 - 負責團隊協調
    Innovator           // 創新者 - 負責創新思考
};

UENUM(BlueprintType)
enum class EBMADDiscussionType : uint8
{
    Technical,           // 技術討論
    Architecture,        // 架構討論
    Design,             // 設計討論
    Security,           // 安全討論
    Performance,         // 性能討論
    Testing,            // 測試討論
    Documentation,       // 文檔討論
    Planning,           // 規劃討論
    Review,             // 審查討論
    Brainstorm          // 腦力激盪
};

UENUM(BlueprintType)
enum class EBMADEmotionState : uint8
{
    Neutral,             // 中性
    Confident,           // 自信
    Curious,             // 好奇
    Concerned,           // 擔憂
    Excited,             // 興奮
    Skeptical,           // 懷疑
    Supportive,           // 支持
    Critical,            // 批評
    Collaborative,        // 協作
    Innovative           // 創新
};

USTRUCT(BlueprintType)
struct FBMADAgentProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Agent")
    FString AgentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Agent")
    FString AgentName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Agent")
    EBMADAgentRole Role;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Agent")
    FString Personality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Agent")
    FString Expertise;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Agent")
    FString CommunicationStyle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Agent")
    float Confidence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Agent")
    float Creativity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Agent")
    float AnalyticalThinking;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMAD Agent")
    float CollaborationSkill;

    FBMADAgentProfile()
    {
        AgentID = TEXT("");
        AgentName = TEXT("");
        Role = EBMADAgentRole::Developer;
        Personality = TEXT("");
        Expertise = TEXT("");
        CommunicationStyle = TEXT("");
        Confidence = 0.8f;
        Creativity = 0.7f;
        AnalyticalThinking = 0.8f;
        CollaborationSkill = 0.9f;
    }
};

USTRUCT(BlueprintType)
struct FBMADMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString MessageID;

    UPROPERTY(BlueprintReadOnly)
    FString SenderID;

    UPROPERTY(BlueprintReadOnly)
    FString Content;

    UPROPERTY(BlueprintReadOnly)
    EBMADEmotionState EmotionState;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Tags;

    UPROPERTY(BlueprintReadOnly)
    float Confidence;

    UPROPERTY(BlueprintReadOnly)
    FString Context;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> References;

    FBMADMessage()
    {
        MessageID = TEXT("");
        SenderID = TEXT("");
        Content = TEXT("");
        EmotionState = EBMADEmotionState::Neutral;
        Timestamp = FDateTime::Now();
        Tags.Empty();
        Confidence = 0.8f;
        Context = TEXT("");
        References.Empty();
    }
};

USTRUCT(BlueprintType)
struct FBMADDiscussionSession
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString SessionID;

    UPROPERTY(BlueprintReadOnly)
    FString Topic;

    UPROPERTY(BlueprintReadOnly)
    EBMADDiscussionType DiscussionType;

    UPROPERTY(BlueprintReadOnly)
    FString ProjectContext;

    UPROPERTY(BlueprintReadOnly)
    TArray<FBMADAgentProfile> Participants;

    UPROPERTY(BlueprintReadOnly)
    TArray<FBMADMessage> Messages;

    UPROPERTY(BlueprintReadOnly)
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EndTime;

    UPROPERTY(BlueprintReadOnly)
    bool bActive;

    UPROPERTY(BlueprintReadOnly)
    FString Summary;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Decisions;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ActionItems;

    FBMADDiscussionSession()
    {
        SessionID = TEXT("");
        Topic = TEXT("");
        DiscussionType = EBMADDiscussionType::Technical;
        ProjectContext = TEXT("");
        Participants.Empty();
        Messages.Empty();
        StartTime = FDateTime::Now();
        EndTime = FDateTime::Now();
        bActive = false;
        Summary = TEXT("");
        Decisions.Empty();
        ActionItems.Empty();
    }
};

USTRUCT(BlueprintType)
struct FBMADInsight
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString InsightID;

    UPROPERTY(BlueprintReadOnly)
    FString Content;

    UPROPERTY(BlueprintReadOnly)
    FString SourceAgentID;

    UPROPERTY(BlueprintReadOnly)
    EBMADEmotionState EmotionState;

    UPROPERTY(BlueprintReadOnly)
    float Importance;

    UPROPERTY(BlueprintReadOnly)
    float Novelty;

    UPROPERTY(BlueprintReadOnly)
    FString Category;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SupportingEvidence;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    FBMADInsight()
    {
        InsightID = TEXT("");
        Content = TEXT("");
        SourceAgentID = TEXT("");
        EmotionState = EBMADEmotionState::Neutral;
        Importance = 0.5f;
        Novelty = 0.5f;
        Category = TEXT("");
        SupportingEvidence.Empty();
        Timestamp = FDateTime::Now();
    }
};

/**
 * BMAD系統 - 多AI分身討論系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSBMADSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSBMADSystem();

    /// 初始化BMAD系統
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    void InitializeBMADSystem();

    /// 創建AI分身
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    FString CreateAgent(const FBMADAgentProfile& Profile);

    /// 刪除AI分身
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    bool RemoveAgent(const FString& AgentID);

    /// 獲取分身配置
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    FBMADAgentProfile GetAgentProfile(const FString& AgentID);

    /// 更新分身配置
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    bool UpdateAgentProfile(const FString& AgentID, const FBMADAgentProfile& Profile);

    /// 啟動討論會話
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    FString StartDiscussionSession(const FString& Topic, EBMADDiscussionType DiscussionType, const FString& ProjectContext);

    /// 添加參與者到討論
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    bool AddParticipantToSession(const FString& SessionID, const FString& AgentID);

    /// 發送消息到討論
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    bool SendMessageToSession(const FString& SessionID, const FString& AgentID, const FString& Content, EBMADEmotionState EmotionState);

    /// 獲取討論會話
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    FBMADDiscussionSession GetDiscussionSession(const FString& SessionID);

    /// 獲取所有活躍會話
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    TArray<FBMADDiscussionSession> GetActiveSessions();

    /// 結束討論會話
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    bool EndDiscussionSession(const FString& SessionID);

    /// 生成討論總結
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    FString GenerateDiscussionSummary(const FString& SessionID);

    /// 提取洞見
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    TArray<FBMADInsight> ExtractInsights(const FString& SessionID);

    /// 自動生成決策
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    TArray<FString> GenerateDecisions(const FString& SessionID);

    /// 創建行動項目
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    TArray<FString> CreateActionItems(const FString& SessionID);

    /// 分析討論質量
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    float AnalyzeDiscussionQuality(const FString& SessionID);

    /// 評估參與者貢獻
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    TMap<FString, float> EvaluateParticipantContributions(const FString& SessionID);

    /// 智能建議下一步
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    FString SuggestNextStep(const FString& SessionID);

    /// 尋找專家分身
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    TArray<FString> FindExpertAgents(const FString& ExpertiseArea);

    /// 創建專業討論組
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    FString CreateExpertDiscussionGroup(const TArray<FString>& AgentIDs, const FString& Topic);

    /// 模擬分身互動
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    void SimulateAgentInteraction(const FString& AgentID1, const FString& AgentID2, const FString& Scenario);

    /// 學習和適應
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    void LearnFromDiscussion(const FString& SessionID);

    /// 導出討論記錄
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    FString ExportDiscussionHistory(const FString& SessionID);

    /// 導入討論記錄
    UFUNCTION(BlueprintCallable, Category = "BMAD System")
    bool ImportDiscussionHistory(const FString& SessionID, const FString& HistoryData);

    /// 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDiscussionStarted, const FString&, const FString&, const FString&);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMessageReceived, const FString&, const FBMADMessage&);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInsightGenerated, const FString&, const FBMADInsight&);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDiscussionEnded, const FString&, const FString&);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAgentInteraction, const FString&, const FString&);

    UPROPERTY(BlueprintAssignable)
    FOnDiscussionStarted OnDiscussionStarted;

    UPROPERTY(BlueprintAssignable)
    FOnMessageReceived OnMessageReceived;

    UPROPERTY(BlueprintAssignable)
    FOnInsightGenerated OnInsightGenerated;

    UPROPERTY(BlueprintAssignable)
    FOnDiscussionEnded OnDiscussionEnded;

    UPROPERTY(BlueprintAssignable)
    FOnAgentInteraction OnAgentInteraction;

private:
    /// AI分身管理
    UPROPERTY()
    TMap<FString, FBMADAgentProfile> AgentProfiles;

    /// 討論會話管理
    UPROPERTY()
    TMap<FString, FBMADDiscussionSession> DiscussionSessions;

    /// 洞見庫
    UPROPERTY()
    TArray<FBMADInsight> InsightLibrary;

    /// 學習數據
    UPROPERTY()
    TMap<FString, TArray<FString>> LearningData;

    /// 系統配置
    UPROPERTY()
    bool bBMADSystemEnabled;

    UPROPERTY()
    int32 MaxAgentsPerSession;

    UPROPERTY()
    float DiscussionTimeoutMinutes;

    /// 內部方法
    FString GenerateUniqueSessionID();
    FString GenerateUniqueMessageID();
    FString GenerateUniqueInsightID();
    FBMADMessage ProcessAgentResponse(const FString& AgentID, const FString& Stimulus, const FString& Context);
    EBMADEmotionState DetermineEmotionState(const FString& AgentID, const FString& Content);
    float CalculateMessageRelevance(const FBMADMessage& Message, const FString& Topic);
    void UpdateAgentLearning(const FString& AgentID, const FString& Interaction);
    FString GenerateContextualResponse(const FString& AgentID, const FString& Topic, const TArray<FBMADMessage>& History);
    void SaveSessionData(const FString& SessionID);
    void LoadSessionData(const FString& SessionID);
    void CleanupInactiveSessions();
    void OptimizeAgentInteractions();
};
