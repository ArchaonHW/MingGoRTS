#pragma once

/**
 * Potato Engine Self Reflection - 自我反思與元認知引擎
 *
 * 為 AI Agent 提供元認知能力：
 * - 思維鏈記錄（觀察→推理→行動→評估）
 * - 決策追蹤與結果驗證
 * - 置信度校準（confidence vs 實際成功率）
 * - 錯誤模式偵測
 * - 策略效果統計與改進建議
 *
 * 設計原則：
 * - 自包含：僅依賴標準庫
 * - 線程安全：所有公開方法內部加鎖
 * - 可掛接到 AIAgentSystem::AIAgent 的決策流程
 */

#include <string>
#include <vector>
#include <unordered_map>
#include <deque>
#include <mutex>
#include <cstdint>
#include <optional>

namespace Potato {
namespace AI {

/**
 * 思維鏈步驟類型
 */
enum class ThoughtType {
    Observation,    // 觀察到的資訊
    Reasoning,      // 推理過程
    Hypothesis,     // 假設
    Action,         // 採取的行動
    Evaluation,     // 自我評估
    Correction      // 自我修正
};

/**
 * 思維鏈中的一個步驟
 */
struct ThoughtStep {
    ThoughtType type = ThoughtType::Reasoning;
    std::string content;
    float confidence = 1.0f;
    uint64_t timestamp = 0;
};

/**
 * 思維鏈（一次任務的完整推理過程）
 */
struct ThoughtChain {
    uint64_t id = 0;
    std::string taskDescription;
    std::vector<ThoughtStep> steps;
    bool completed = false;
    bool successful = false;
    uint64_t startTime = 0;
    uint64_t endTime = 0;
};

/**
 * 決策記錄（用於事後校準）
 */
struct DecisionRecord {
    uint64_t id = 0;
    std::string context;
    std::vector<std::string> options;
    std::string chosen;
    float confidence = 0.5f;       // 決策時的置信度
    std::string strategy;          // 使用的策略名稱
    bool outcomeKnown = false;
    bool success = false;
    std::string feedback;
    uint64_t timestamp = 0;
};

/**
 * 錯誤模式
 */
struct ErrorPattern {
    std::string signature;         // 錯誤特徵
    uint32_t count = 0;
    std::vector<std::string> contexts;
    uint64_t lastSeen = 0;
};

/**
 * 性能評估結果
 */
struct PerformanceReport {
    uint32_t totalDecisions = 0;
    uint32_t decisionsWithOutcome = 0;
    float accuracy = 0.0f;              // 成功率
    float calibrationError = 0.0f;      // 平均 |置信度 - 結果|
    float avgConfidence = 0.0f;
    std::string strongestStrategy;
    std::string weakestStrategy;
    std::vector<std::string> topErrorPatterns;
};

/**
 * 自我反思引擎
 */
class SelfReflection {
public:
    SelfReflection();
    ~SelfReflection();

    // ---- 思維鏈 ----
    uint64_t BeginChain(const std::string& taskDescription);
    void AddThought(uint64_t chainId, ThoughtType type, const std::string& content,
                    float confidence = 1.0f);
    void EndChain(uint64_t chainId, bool successful);
    const ThoughtChain* GetChain(uint64_t chainId);
    std::vector<const ThoughtChain*> GetRecentChains(size_t count) const;

    // ---- 決策追蹤 ----
    uint64_t RecordDecision(const std::string& context,
                            const std::vector<std::string>& options,
                            const std::string& chosen,
                            float confidence,
                            const std::string& strategy = "");
    void RecordOutcome(uint64_t decisionId, bool success,
                       const std::string& feedback = "");

    // ---- 錯誤分析 ----
    void RecordError(const std::string& signature, const std::string& context);
    std::vector<ErrorPattern> GetTopErrorPatterns(size_t n) const;

    // ---- 評估 ----
    PerformanceReport Evaluate() const;
    // 策略維度統計：策略名 -> {成功率, 樣本數}
    std::unordered_map<std::string, std::pair<float, uint32_t>> GetStrategyStats() const;

    // ---- 改進建議 ----
    std::vector<std::string> SuggestImprovements() const;

    // ---- 容量管理 ----
    void SetMaxChains(size_t max) { maxChains = max; }
    void SetMaxDecisions(size_t max) { maxDecisions = max; }

    // ---- 序列化 ----
    std::string SerializeReport() const;

private:
    uint64_t nextChainId = 1;
    uint64_t nextDecisionId = 1;

    std::deque<ThoughtChain> chains;
    std::deque<DecisionRecord> decisions;
    std::unordered_map<std::string, ErrorPattern> errorPatterns;

    size_t maxChains = 256;
    size_t maxDecisions = 1024;

    mutable std::mutex mutex;

    uint64_t Now() const;
};

} // namespace AI
} // namespace Potato
