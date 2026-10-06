/**
 * CognitiveDemo - 認知增強演示
 *
 * 展示 KnowledgeGraph（知識圖譜/長期記憶）與 SelfReflection（自我反思/元認知）
 * 如何協同為 AI Agent 提供智能增強。
 */

#include "AI/KnowledgeGraph.h"
#include "AI/SelfReflection.h"
#include <iostream>

using namespace Potato::AI;

int main() {
    std::cout << "=== Potato Engine Cognitive Enhancement Demo ===\n\n";

    // =========================================================================
    // 1. 知識圖譜：構建代碼庫知識
    // =========================================================================
    std::cout << "[1] Building knowledge graph of codebase concepts...\n";

    KnowledgeGraph kg;

    // 實體：引擎模組
    uint64_t engine   = kg.AddNode("PotatoEngine", "module");
    uint64_t aiSys    = kg.AddNode("AIAgentSystem", "module");
    uint64_t llm      = kg.AddNode("LLMIntegration", "module");
    uint64_t rag      = kg.AddNode("RAGSystem", "module");
    uint64_t kg_mod   = kg.AddNode("KnowledgeGraph", "module");
    uint64_t sr_mod   = kg.AddNode("SelfReflection", "module");
    uint64_t agent    = kg.AddNode("AIAgent", "class");
    uint64_t devAgent = kg.AddNode("DeveloperAgent", "class");

    // 關係
    kg.AddRelation(devAgent, agent, "is-a");
    kg.AddRelation(aiSys, agent, "contains");
    kg.AddRelation(devAgent, llm, "depends-on", 0.9f);
    kg.AddRelation(rag, llm, "depends-on", 0.8f);
    kg.AddRelation(agent, kg_mod, "uses", 0.7f);
    kg.AddRelation(agent, sr_mod, "uses", 0.7f);
    kg.AddRelation(aiSys, engine, "part-of");

    std::cout << "   Nodes: " << kg.NodeCount() << ", Edges: " << kg.EdgeCount() << "\n";

    // 推理：is-a 繼承 + 傳遞閉包
    size_t inherited = kg.InheritThroughIsA();
    std::cout << "   Inferred via is-a inheritance: " << inherited << " edges\n";
    size_t transitive = kg.InferTransitive("part-of");
    std::cout << "   Inferred via part-of transitivity: " << transitive << " edges\n";

    // 查詢：DeveloperAgent 依賴什麼？
    auto deps = kg.Query("DeveloperAgent", "depends-on", "");
    std::cout << "   DeveloperAgent depends-on: ";
    for (const auto& t : deps) std::cout << t.object << "(" << t.confidence << ") ";
    std::cout << "\n";

    // 路徑查詢：DeveloperAgent -> PotatoEngine
    auto path = kg.FindPath(devAgent, engine, 5);
    if (path) {
        std::cout << "   Path DeveloperAgent -> PotatoEngine (score "
                  << path->score << "): ";
        for (size_t i = 0; i < path->nodeIds.size(); i++) {
            // 注意：此處需 map id->name，簡化直接用 id
            std::cout << path->nodeIds[i];
            if (i < path->relations.size()) std::cout << " -[" << path->relations[i] << "]-> ";
        }
        std::cout << "\n";
    }

    // =========================================================================
    // 2. 自我反思：追蹤一次代碼生成任務的推理過程
    // =========================================================================
    std::cout << "\n[2] Self-reflection on a code generation task...\n";

    SelfReflection sr;

    uint64_t chain = sr.BeginChain("Generate a sorting function");
    sr.AddThought(chain, ThoughtType::Observation,
                  "User requested a sort function for integers", 1.0f);
    sr.AddThought(chain, ThoughtType::Reasoning,
                  "QuickSort is efficient; but stability may matter", 0.8f);
    sr.AddThought(chain, ThoughtType::Hypothesis,
                  "MergeSort chosen for stable O(n log n)", 0.75f);
    sr.AddThought(chain, ThoughtType::Action,
                  "Generated merge_sort implementation", 0.9f);
    sr.AddThought(chain, ThoughtType::Evaluation,
                  "Code compiles; edge case: empty array handled", 0.95f);
    sr.EndChain(chain, true);

    // 決策追蹤 + 校準
    uint64_t d1 = sr.RecordDecision("Choose sort algorithm",
                                    {"quick", "merge", "heap"},
                                    "merge", 0.75f, "complexity-analysis");
    sr.RecordOutcome(d1, true, "User accepted merge sort");

    uint64_t d2 = sr.RecordDecision("Name the function",
                                    {"sort", "mergeSort", "stable_sort"},
                                    "sort", 0.9f, "naming-convention");
    sr.RecordOutcome(d2, false, "Too generic; renamed to merge_sort");

    uint64_t d3 = sr.RecordDecision("Error handling style",
                                    {"exceptions", "error-codes"},
                                    "exceptions", 0.6f, "style-guide");
    sr.RecordOutcome(d3, true);

    // 記錄一個錯誤模式
    sr.RecordError("generic-naming", "function 'sort' too ambiguous");
    sr.RecordError("generic-naming", "variable 'data' too ambiguous");

    // =========================================================================
    // 3. 性能評估與改進建議
    // =========================================================================
    std::cout << "\n[3] Performance evaluation...\n";

    PerformanceReport report = sr.Evaluate();
    std::cout << "   Decisions tracked: " << report.totalDecisions << "\n";
    std::cout << "   With outcome: " << report.decisionsWithOutcome << "\n";
    std::cout << "   Accuracy: " << (report.accuracy * 100.0f) << "%\n";
    std::cout << "   Avg confidence: " << report.avgConfidence << "\n";
    std::cout << "   Calibration error: " << report.calibrationError << "\n";
    if (!report.strongestStrategy.empty()) {
        std::cout << "   Strongest strategy: " << report.strongestStrategy << "\n";
    }

    std::cout << "\n[4] Self-improvement suggestions:\n";
    for (const auto& s : sr.SuggestImprovements()) {
        std::cout << "   - " << s << "\n";
    }

    // =========================================================================
    // 5. 持久化演示
    // =========================================================================
    std::cout << "\n[5] Serialization round-trip...\n";
    std::string json = kg.Serialize();
    std::cout << "   Serialized " << json.size() << " bytes\n";

    KnowledgeGraph kg2;
    if (kg2.Deserialize(json)) {
        std::cout << "   Restored: " << kg2.NodeCount() << " nodes, "
                  << kg2.EdgeCount() << " edges\n";
    }

    std::cout << "\n=== Demo complete ===\n";
    return 0;
}
