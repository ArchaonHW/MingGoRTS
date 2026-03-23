#pragma once

#include "CoreMinimal.h"
#include "SageBrain/MingSageBrainBridge.h"
#include "SageBrain/MingSageBrainSelfLearningSystem.h"

/**
 * 聖者大腦自我思考與學習功能使用範例
 * 展示如何啟用和使用自我思考、自我學習、自我改進功能
 */
class FMingSageBrainSelfLearningExamples
{
public:
    // ============ 範例 1: 基礎自我思考啟用 ============
    static void Example1_EnableBasicSelfThinking(UMingSageBrainBridge* Bridge)
    {
        if (!Bridge) return;

        UE_LOG(LogTemp, Log, TEXT("=== 範例 1: 基礎自我思考啟用 ==="));

        // 啟用被動模式的自我思考（每60秒思考一次）
        bool bSuccess = Bridge->EnableSelfThinking(ESelfThinkingMode::Passive, 60.0f);

        if (bSuccess)
        {
            UE_LOG(LogTemp, Log, TEXT("✓ 自我思考已啟用"));
        }
    }

    // ============ 範例 2: 主動模式自我思考 ============
    static void Example2_ActiveSelfThinking(UMingSageBrainBridge* Bridge)
    {
        if (!Bridge) return;

        UE_LOG(LogTemp, Log, TEXT("=== 範例 2: 主動模式自我思考 ==="));

        // 啟用主動模式的自我思考（每30秒主動思考）
        Bridge->EnableSelfThinking(ESelfThinkingMode::Active, 30.0f);

        // 觸發一次特定主題的思考
        FSelfThinkingRecord Record = Bridge->TriggerSelfThinking(
            TEXT("分析系統性能優化機會"),
            ESelfThinkingMode::Active
        );

        UE_LOG(LogTemp, Log, TEXT("思考內容: %s"), *Record.ThoughtContent);
        UE_LOG(LogTemp, Log, TEXT("思考深度: %.2f"), Record.DepthScore);
    }

    // ============ 範例 3: 深度反思模式 ============
    static void Example3_DeepReflection(UMingSageBrainBridge* Bridge)
    {
        if (!Bridge) return;

        UE_LOG(LogTemp, Log, TEXT("=== 範例 3: 深度反思模式 ==="));

        // 啟用反思模式
        Bridge->EnableSelfThinking(ESelfThinkingMode::Reflective, 120.0f);

        // 觸發深度反思
        FSelfThinkingRecord Record = Bridge->TriggerSelfThinking(
            TEXT("回顧今日所有決策，分析改進空間"),
            ESelfThinkingMode::Reflective
        );

        // 輸出反思中的關鍵見解
        for (const auto& Insight : Record.Insights)
        {
            UE_LOG(LogTemp, Log, TEXT("見解 [%s]: %.2f"), *Insight.Key, Insight.Value);
        }
    }

    // ============ 範例 4: 創造性思考 ============
    static void Example4_CreativeThinking(UMingSageBrainBridge* Bridge)
    {
        if (!Bridge) return;

        UE_LOG(LogTemp, Log, TEXT("=== 範例 4: 創造性思考 ==="));

        // 啟用創造模式
        Bridge->EnableSelfThinking(ESelfThinkingMode::Creative, 180.0f);

        // 觸發創造性思考
        FSelfThinkingRecord Record = Bridge->TriggerSelfThinking(
            TEXT("設計新的遊戲機制，融合中國傳統文化"),
            ESelfThinkingMode::Creative
        );

        UE_LOG(LogTemp, Log, TEXT("創意內容: %s"), *Record.ThoughtContent);
    }

    // ============ 範例 5: 持續學習啟用 ============
    static void Example5_EnableContinuousLearning(UMingSageBrainBridge* Bridge)
    {
        if (!Bridge) return;

        UE_LOG(LogTemp, Log, TEXT("=== 範例 5: 持續學習啟用 ==="));

        // 啟用持續學習（每30秒檢查並學習）
        bool bSuccess = Bridge->EnableContinuousLearning(30.0f);

        if (bSuccess)
        {
            UE_LOG(LogTemp, Log, TEXT("✓ 持續學習已啟用"));

            // 獲取學習狀態
            FString Status = Bridge->GetSelfLearningStatus();
            UE_LOG(LogTemp, Log, TEXT("學習狀態: %s"), *Status);
        }
    }

    // ============ 範例 6: 從經驗學習 ============
    static void Example6_LearnFromExperience(UMingSageBrainBridge* Bridge)
    {
        if (!Bridge) return;

        UE_LOG(LogTemp, Log, TEXT("=== 範例 6: 從經驗學習 ==="));

        // 需要直接訪問自我學習系統
        UMingSageBrainSelfLearningSystem* SelfLearning = 
            NewObject<UMingSageBrainSelfLearningSystem>();

        // 模擬從一次經驗學習
        FLearningExperience Experience = SelfLearning->LearnFromExperience(
            TEXT("戰鬥AI決策"),
            TEXT("敵軍從東側進攻，我方兵力不足"),
            TEXT("採取防守策略，呼叫援軍"),
            TEXT("成功守住陣地，援軍及時到達"),
            0.85f  // 85% 成功率
        );

        UE_LOG(LogTemp, Log, TEXT("學習經驗已記錄: %s"), *Experience.ExperienceID);
        UE_LOG(LogTemp, Log, TEXT("成功率: %.2f%%"), Experience.SuccessRate * 100);

        // 推進學習階段
        SelfLearning->AdvanceLearningStage(Experience);
        UE_LOG(LogTemp, Log, TEXT("當前學習階段: %s"),
            *UEnum::GetDisplayValueAsText(Experience.Stage).ToString());
    }

    // ============ 範例 7: 知識管理 ============
    static void Example7_KnowledgeManagement(UMingSageBrainBridge* Bridge)
    {
        if (!Bridge) return;

        UE_LOG(LogTemp, Log, TEXT("=== 範例 7: 知識管理 ==="));

        UMingSageBrainSelfLearningSystem* SelfLearning = 
            NewObject<UMingSageBrainSelfLearningSystem>();

        // 添加新知識
        FSelfKnowledgeBase Knowledge = SelfLearning->AddKnowledge(
            TEXT("戰術策略"),
            TEXT("當兵力處於劣勢時，應利用地形優勢進行防守"),
            0.9f  // 高信心度
        );

        UE_LOG(LogTemp, Log, TEXT("新增知識: %s"), *Knowledge.KnowledgeID);

        // 查詢知識
        TArray<FSelfKnowledgeBase> Results = SelfLearning->QueryKnowledge(
            TEXT("戰術策略"),
            TEXT("地形")
        );

        UE_LOG(LogTemp, Log, TEXT("查詢到 %d 條相關知識"), Results.Num());

        // 使用知識
        SelfLearning->UseKnowledge(Knowledge.KnowledgeID);

        // 獲取熱門知識
        TArray<FSelfKnowledgeBase> TopKnowledge = SelfLearning->GetTopKnowledge(5);
        UE_LOG(LogTemp, Log, TEXT("熱門知識數量: %d"), TopKnowledge.Num());
    }

    // ============ 範例 8: 自我改進建議 ============
    static void Example8_SelfImprovement(UMingSageBrainBridge* Bridge)
    {
        if (!Bridge) return;

        UE_LOG(LogTemp, Log, TEXT("=== 範例 8: 自我改進建議 ==="));

        // 獲取自我改進建議
        TArray<FSelfImprovementSuggestion> Suggestions = 
            Bridge->GetSelfImprovementSuggestions();

        UE_LOG(LogTemp, Log, TEXT("獲得 %d 個改進建議"), Suggestions.Num());

        // 顯示建議詳情
        for (const auto& Suggestion : Suggestions)
        {
            UE_LOG(LogTemp, Log, TEXT("建議 [%s]: %s"),
                *Suggestion.TargetArea,
                *Suggestion.SuggestedChange);

            UE_LOG(LogTemp, Log, TEXT("  優先級: %.2f, 預期收益: %s"),
                Suggestion.Priority,
                *Suggestion.ExpectedBenefit);
        }

        // 實施最高優先級的建議
        if (Suggestions.Num() > 0)
        {
            FString TopSuggestionID = Suggestions[0].SuggestionID;
            bool bImplemented = Bridge->ImplementSelfImprovement(TopSuggestionID);

            if (bImplemented)
            {
                UE_LOG(LogTemp, Log, TEXT("✓ 已實施改進建議: %s"), *TopSuggestionID);
            }
        }
    }

    // ============ 範例 9: 自我評估 ============
    static void Example9_SelfAssessment(UMingSageBrainBridge* Bridge)
    {
        if (!Bridge) return;

        UE_LOG(LogTemp, Log, TEXT("=== 範例 9: 自我評估 ==="));

        UMingSageBrainSelfLearningSystem* SelfLearning = 
            NewObject<UMingSageBrainSelfLearningSystem>();

        // 執行全面自我評估
        TMap<FString, float> Assessment = SelfLearning->PerformSelfAssessment();

        // 顯示評估結果
        for (const auto& Metric : Assessment)
        {
            UE_LOG(LogTemp, Log, TEXT("%s: %.2f"), *Metric.Key, Metric.Value);
        }

        // 特別關注總體評分
        if (Assessment.Contains(TEXT("總體評分")))
        {
            float OverallScore = Assessment[TEXT("總體評分")];
            UE_LOG(LogTemp, Log, TEXT("★ 總體評分: %.2f / 1.0"), OverallScore);
        }
    }

    // ============ 範例 10: 完整整合工作流程 ============
    static void Example10_CompleteSelfLearningWorkflow(UMingSageBrainBridge* Bridge)
    {
        if (!Bridge) return;

        UE_LOG(LogTemp, Log, TEXT("=== 範例 10: 完整自我學習工作流程 ==="));

        // 步驟 1: 啟用自我思考
        Bridge->EnableSelfThinking(ESelfThinkingMode::Active, 60.0f);
        UE_LOG(LogTemp, Log, TEXT("[1/5] 自我思考已啟用"));

        // 步驟 2: 啟用持續學習
        Bridge->EnableContinuousLearning(30.0f);
        UE_LOG(LogTemp, Log, TEXT("[2/5] 持續學習已啟用"));

        // 步驟 3: 觸發初始思考
        FSelfThinkingRecord InitialThought = Bridge->TriggerSelfThinking(
            TEXT("系統初始狀態評估"),
            ESelfThinkingMode::Reflective
        );
        UE_LOG(LogTemp, Log, TEXT("[3/5] 初始思考完成，深度: %.2f"), InitialThought.DepthScore);

        // 步驟 4: 獲取改進建議
        TArray<FSelfImprovementSuggestion> Suggestions = 
            Bridge->GetSelfImprovementSuggestions();
        UE_LOG(LogTemp, Log, TEXT("[4/5] 獲得 %d 個改進建議"), Suggestions.Num());

        // 步驟 5: 實施前3個建議
        int32 ImplementedCount = 0;
        for (int32 i = 0; i < FMath::Min(3, Suggestions.Num()); i++)
        {
            if (Bridge->ImplementSelfImprovement(Suggestions[i].SuggestionID))
            {
                ImplementedCount++;
            }
        }
        UE_LOG(LogTemp, Log, TEXT("[5/5] 已實施 %d/%d 個改進建議"), 
            ImplementedCount, FMath::Min(3, Suggestions.Num()));

        // 顯示最終狀態
        FString FinalStatus = Bridge->GetSelfLearningStatus();
        UE_LOG(LogTemp, Log, TEXT("最終狀態: %s"), *FinalStatus);

        UE_LOG(LogTemp, Log, TEXT("✓ 完整自我學習工作流程已建立"));
    }

    // ============ 範例 11: 智能決策支援 ============
    static void Example11_SmartDecisionSupport(UMingSageBrainBridge* Bridge)
    {
        if (!Bridge) return;

        UE_LOG(LogTemp, Log, TEXT("=== 範例 11: 智能決策支援 ==="));

        UMingSageBrainSelfLearningSystem* SelfLearning = 
            NewObject<UMingSageBrainSelfLearningSystem>();

        // 基於當前情境獲取智能建議
        FString Context = TEXT("玩家面對強大敵軍，資源有限");
        TArray<FString> Suggestions = SelfLearning->GenerateSmartSuggestions(Context);

        UE_LOG(LogTemp, Log, TEXT("情境: %s"), *Context);
        UE_LOG(LogTemp, Log, TEXT("獲得 %d 個智能建議:"), Suggestions.Num());

        for (int32 i = 0; i < Suggestions.Num(); i++)
        {
            UE_LOG(LogTemp, Log, TEXT("  %d. %s"), i + 1, *Suggestions[i]);
        }

        // 預測最佳行動
        TArray<FString> PossibleActions;
        PossibleActions.Add(TEXT("正面進攻"));
        PossibleActions.Add(TEXT("防守反擊"));
        PossibleActions.Add(TEXT("撤退重整"));
        PossibleActions.Add(TEXT("呼叫援軍"));

        FString BestAction = SelfLearning->PredictBestAction(Context, PossibleActions);
        UE_LOG(LogTemp, Log, TEXT("★ 預測最佳行動: %s"), *BestAction);
    }

    // ============ 範例 12: 冥想模式深度學習 ============
    static void Example12_MeditativeDeepLearning(UMingSageBrainBridge* Bridge)
    {
        if (!Bridge) return;

        UE_LOG(LogTemp, Log, TEXT("=== 範例 12: 冥想模式深度學習 ==="));

        // 啟用冥想模式（深度思考）
        Bridge->EnableSelfThinking(ESelfThinkingMode::Meditative, 300.0f);

        // 觸發深度冥想思考
        FSelfThinkingRecord Meditation = Bridge->TriggerSelfThinking(
            TEXT("深度探索遊戲AI的本質，尋找創新方向"),
            ESelfThinkingMode::Meditative
        );

        UE_LOG(LogTemp, Log, TEXT("冥想思考完成"));
        UE_LOG(LogTemp, Log, TEXT("內容長度: %d 字符"), Meditation.ThoughtContent.Len());
        UE_LOG(LogTemp, Log, TEXT("思考深度: %.2f/1.0"), Meditation.DepthScore);

        // 分析思考的關聯性
        UE_LOG(LogTemp, Log, TEXT("關聯思考數量: %d"), Meditation.RelatedThoughts.Num());

        // 在冥想後進行知識合併
        UMingSageBrainSelfLearningSystem* SelfLearning = 
            NewObject<UMingSageBrainSelfLearningSystem>();
        SelfLearning->ConsolidateKnowledge();

        UE_LOG(LogTemp, Log, TEXT("✓ 知識合併完成"));
    }
};
