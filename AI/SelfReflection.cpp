/**
 * Potato Engine Self Reflection Implementation
 */

#include "SelfReflection.h"
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cmath>

namespace Potato {
namespace AI {

SelfReflection::SelfReflection() = default;
SelfReflection::~SelfReflection() = default;

uint64_t SelfReflection::Now() const {
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count());
}

// ============================================================================
// Thought chains
// ============================================================================

uint64_t SelfReflection::BeginChain(const std::string& taskDescription) {
    std::lock_guard<std::mutex> lock(mutex);
    ThoughtChain chain;
    chain.id = nextChainId++;
    chain.taskDescription = taskDescription;
    chain.startTime = Now();
    chains.push_back(chain);
    if (chains.size() > maxChains) chains.pop_front();
    return chain.id;
}

void SelfReflection::AddThought(uint64_t chainId, ThoughtType type,
                                const std::string& content, float confidence) {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& chain : chains) {
        if (chain.id == chainId && !chain.completed) {
            ThoughtStep step;
            step.type = type;
            step.content = content;
            step.confidence = confidence;
            step.timestamp = Now();
            chain.steps.push_back(step);
            return;
        }
    }
}

void SelfReflection::EndChain(uint64_t chainId, bool successful) {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& chain : chains) {
        if (chain.id == chainId) {
            chain.completed = true;
            chain.successful = successful;
            chain.endTime = Now();
            return;
        }
    }
}

const ThoughtChain* SelfReflection::GetChain(uint64_t chainId) {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& chain : chains) {
        if (chain.id == chainId) return &chain;
    }
    return nullptr;
}

std::vector<const ThoughtChain*> SelfReflection::GetRecentChains(size_t count) const {
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<const ThoughtChain*> result;
    size_t n = std::min(count, chains.size());
    for (size_t i = chains.size() - n; i < chains.size(); i++) {
        result.push_back(&chains[i]);
    }
    return result;
}

// ============================================================================
// Decision tracking
// ============================================================================

uint64_t SelfReflection::RecordDecision(const std::string& context,
                                        const std::vector<std::string>& options,
                                        const std::string& chosen,
                                        float confidence,
                                        const std::string& strategy) {
    std::lock_guard<std::mutex> lock(mutex);
    DecisionRecord rec;
    rec.id = nextDecisionId++;
    rec.context = context;
    rec.options = options;
    rec.chosen = chosen;
    rec.confidence = confidence;
    rec.strategy = strategy;
    rec.timestamp = Now();
    decisions.push_back(rec);
    if (decisions.size() > maxDecisions) decisions.pop_front();
    return rec.id;
}

void SelfReflection::RecordOutcome(uint64_t decisionId, bool success,
                                   const std::string& feedback) {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& d : decisions) {
        if (d.id == decisionId) {
            d.outcomeKnown = true;
            d.success = success;
            d.feedback = feedback;
            return;
        }
    }
}

// ============================================================================
// Error analysis
// ============================================================================

void SelfReflection::RecordError(const std::string& signature,
                                 const std::string& context) {
    std::lock_guard<std::mutex> lock(mutex);
    auto& p = errorPatterns[signature];
    p.signature = signature;
    p.count++;
    p.lastSeen = Now();
    if (p.contexts.size() < 16) {
        p.contexts.push_back(context);
    }
}

std::vector<ErrorPattern> SelfReflection::GetTopErrorPatterns(size_t n) const {
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<ErrorPattern> all;
    for (const auto& [sig, p] : errorPatterns) all.push_back(p);
    std::sort(all.begin(), all.end(),
              [](const ErrorPattern& a, const ErrorPattern& b) { return a.count > b.count; });
    if (all.size() > n) all.resize(n);
    return all;
}

// ============================================================================
// Evaluation
// ============================================================================

PerformanceReport SelfReflection::Evaluate() const {
    std::lock_guard<std::mutex> lock(mutex);
    PerformanceReport report;
    report.totalDecisions = static_cast<uint32_t>(decisions.size());

    float confSum = 0.0f, calSum = 0.0f;
    std::unordered_map<std::string, std::pair<uint32_t, uint32_t>> strat; // name -> {success, total}

    for (const auto& d : decisions) {
        confSum += d.confidence;
        if (d.outcomeKnown) {
            report.decisionsWithOutcome++;
            if (d.success) report.accuracy += 1.0f;
            calSum += std::fabs(d.confidence - (d.success ? 1.0f : 0.0f));
            if (!d.strategy.empty()) {
                strat[d.strategy].second++;
                if (d.success) strat[d.strategy].first++;
            }
        }
    }

    if (report.totalDecisions > 0) {
        report.avgConfidence = confSum / report.totalDecisions;
    }
    if (report.decisionsWithOutcome > 0) {
        report.accuracy /= report.decisionsWithOutcome;
        report.calibrationError = calSum / report.decisionsWithOutcome;
    }

    // 最強/最弱策略
    float bestRate = -1.0f, worstRate = 2.0f;
    for (const auto& [name, st] : strat) {
        if (st.second < 3) continue;  // 至少 3 個樣本
        float rate = static_cast<float>(st.first) / st.second;
        if (rate > bestRate) { bestRate = rate; report.strongestStrategy = name; }
        if (rate < worstRate) { worstRate = rate; report.weakestStrategy = name; }
    }

    // 常見錯誤
    std::vector<ErrorPattern> errs;
    for (const auto& [sig, p] : errorPatterns) errs.push_back(p);
    std::sort(errs.begin(), errs.end(),
              [](const ErrorPattern& a, const ErrorPattern& b) { return a.count > b.count; });
    for (size_t i = 0; i < std::min(errs.size(), size_t(5)); i++) {
        report.topErrorPatterns.push_back(errs[i].signature);
    }

    return report;
}

std::unordered_map<std::string, std::pair<float, uint32_t>>
SelfReflection::GetStrategyStats() const {
    std::lock_guard<std::mutex> lock(mutex);
    std::unordered_map<std::string, std::pair<uint32_t, uint32_t>> strat;
    for (const auto& d : decisions) {
        if (!d.outcomeKnown || d.strategy.empty()) continue;
        strat[d.strategy].second++;
        if (d.success) strat[d.strategy].first++;
    }
    std::unordered_map<std::string, std::pair<float, uint32_t>> result;
    for (const auto& [name, st] : strat) {
        result[name] = {static_cast<float>(st.first) / st.second, st.second};
    }
    return result;
}

// ============================================================================
// Improvement suggestions
// ============================================================================

std::vector<std::string> SelfReflection::SuggestImprovements() const {
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<std::string> suggestions;

    // 計算整體指標
    uint32_t withOutcome = 0, successCount = 0;
    float confSum = 0.0f, calSum = 0.0f;
    for (const auto& d : decisions) {
        confSum += d.confidence;
        if (d.outcomeKnown) {
            withOutcome++;
            if (d.success) successCount++;
            calSum += std::fabs(d.confidence - (d.success ? 1.0f : 0.0f));
        }
    }

    if (decisions.empty()) {
        suggestions.push_back("No decisions recorded yet. Start tracking decisions to enable self-improvement.");
        return suggestions;
    }

    float accuracy = withOutcome ? static_cast<float>(successCount) / withOutcome : 0.0f;
    float avgConf = confSum / decisions.size();
    float calErr = withOutcome ? calSum / withOutcome : 0.0f;

    // 校準問題
    if (withOutcome >= 5 && calErr > 0.4f) {
        if (avgConf > accuracy + 0.15f) {
            suggestions.push_back("Overconfidence detected: lower stated confidence or gather more evidence before deciding.");
        } else if (avgConf < accuracy - 0.15f) {
            suggestions.push_back("Underconfidence detected: the agent succeeds more often than it expects; raise confidence or attempt harder tasks.");
        }
    }

    // 低準確率
    if (withOutcome >= 5 && accuracy < 0.5f) {
        suggestions.push_back("Low decision accuracy (<50%): review recent failures and consider alternative strategies.");
    }

    // 錯誤模式
    std::vector<ErrorPattern> errs;
    for (const auto& [sig, p] : errorPatterns) errs.push_back(p);
    std::sort(errs.begin(), errs.end(),
              [](const ErrorPattern& a, const ErrorPattern& b) { return a.count > b.count; });
    if (!errs.empty() && errs[0].count >= 3) {
        suggestions.push_back("Recurring error pattern '" + errs[0].signature +
                              "' seen " + std::to_string(errs[0].count) +
                              " times: investigate root cause.");
    }

    // 思維鏈質量
    uint32_t failedChains = 0;
    for (const auto& c : chains) {
        if (c.completed && !c.successful) failedChains++;
    }
    if (failedChains > chains.size() / 3 && chains.size() >= 6) {
        suggestions.push_back("High chain failure rate: add explicit verification steps (Evaluation) before actions.");
    }

    if (suggestions.empty()) {
        suggestions.push_back("Performance is healthy. Continue current strategies.");
    }
    return suggestions;
}

// ============================================================================
// Serialization
// ============================================================================

std::string SelfReflection::SerializeReport() const {
    std::lock_guard<std::mutex> lock(mutex);
    std::stringstream ss;
    ss << "=== Self-Reflection Report ===\n";
    ss << "Decisions: " << decisions.size() << "\n";
    ss << "Chains: " << chains.size() << "\n";
    ss << "Error patterns: " << errorPatterns.size() << "\n\n";

    ss << "Recent chains:\n";
    size_t shown = 0;
    for (auto it = chains.rbegin(); it != chains.rend() && shown < 5; ++it, ++shown) {
        ss << "  #" << it->id << " \"" << it->taskDescription << "\" "
           << (it->completed ? (it->successful ? "[OK]" : "[FAIL]") : "[...]")
           << " steps=" << it->steps.size() << "\n";
    }
    return ss.str();
}

} // namespace AI
} // namespace Potato
