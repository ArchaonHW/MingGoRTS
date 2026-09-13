#pragma once

#include "Core/CoreTypes.h"
#include "ECS/Entity.h"
#include "Events/EventBus.h"
#include "Time/TimeManager.h"
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace Potato {
namespace AI {

/**
 * AI 代理類型
 */
enum class AgentType {
    Developer,      // 開發代理 - 代碼生成和優化
    Designer,       // 設計代理 - 創意和設計任務
    Analyst,        // 分析代理 - 數據分析和優化
    Tester,         // 測試代理 - 自動化測試
    Debugger,       // 調試代理 - 問題診斷和修復
    Researcher,     // 研究代理 - 技術研究和創新
    Custom         // 自定義代理
};

/**
 * 任務優先級
 */
enum class TaskPriority {
    Low = 0,
    Medium = 1,
    High = 2,
    Critical = 3
};

/**
 * 任務狀態
 */
enum class TaskStatus {
    Pending,
    InProgress,
    Completed,
    Failed,
    Cancelled
};

/**
 * 任務描述
 */
struct AgentTask {
    std::string id;
    std::string description;
    std::string category;
    TaskPriority priority;
    TaskStatus status;
    float progress;
    std::string result;
    std::string error;
    uint64 createdTime;
    uint64 completedTime;
    uint64 estimatedTime;
    
    AgentTask()
        : priority(TaskPriority::Medium)
        , status(TaskStatus::Pending)
        , progress(0.0f)
        , createdTime(0)
        , completedTime(0)
        , estimatedTime(0) {
    }
};

/**
 * 代理描述
 */
struct AgentDesc {
    std::string name;
    AgentType type;
    bool autonomous;
    float performanceRating;
    float learningRate;
    std::vector<std::string> capabilities;
    
    AgentDesc()
        : type(AgentType::Custom)
        , autonomous(true)
        , performanceRating(0.8f)
        , learningRate(0.1f) {
    }
};

/**
 * 決策結果
 */
struct Decision {
    std::string action;
    float confidence;
    std::vector<std::string> reasoning;
    std::string timestamp;
    
    Decision()
        : confidence(0.0f) {
    }
};

/**
 * AI 代理基類
 */
class AIAgent {
public:
    AIAgent(const AgentDesc& desc);
    virtual ~AIAgent();
    
    // 生命周期
    virtual bool Initialize();
    virtual void Shutdown();
    
    // 任務管理
    void AssignTask(const AgentTask& task);
    void CompleteTask(const std::string& result);
    void FailTask(const std::string& error);
    void CancelTask(const std::string& taskId);
    
    // 執行
    virtual void Update(float deltaTime);
    virtual void ProcessCurrentTask();
    
    // 決策
    virtual Decision MakeDecision(const std::string& context, const std::vector<std::string>& options);
    
    // 狀態查詢
    bool IsActive() const { return active; }
    void SetActive(bool isActive) { active = isActive; }
    
    bool IsAutonomous() const { return autonomous; }
    
    // 任務查詢
    const std::vector<AgentTask>& GetTasks() const { return tasks; }
    const AgentTask* GetCurrentTask() const;
    
    // 信息查詢
    const std::string& GetName() const { return name; }
    AgentType GetType() const { return type; }
    float GetPerformanceRating() const { return performanceRating; }
    
    // 學習
    void RecordLearning(const std::string& context, const std::string& action, bool success);
    float GetLearningRate() const { return learningRate; }
    
    // 統計
    size_t GetCompletedTaskCount() const { return completedTaskCount; }
    size_t GetFailedTaskCount() const { return failedTaskCount; }
    float GetSuccessRate() const;
    
protected:
    std::string name;
    AgentType type;
    bool autonomous;
    float performanceRating;
    float learningRate;
    std::vector<std::string> capabilities;
    
    std::vector<AgentTask> tasks;
    int currentTaskIndex;
    
    bool active;
    size_t completedTaskCount;
    size_t failedTaskCount;
    
    // 學習數據
    struct LearningData {
        std::string context;
        std::string action;
        bool success;
        uint64 timestamp;
    };
    std::vector<LearningData> learningHistory;
    
    std::mutex taskMutex;
    std::mutex learningMutex;
};

/**
 * 開發代理 - 專注於代碼生成和優化
 */
class DeveloperAgent : public AIAgent {
public:
    DeveloperAgent(const AgentDesc& desc);
    
    Decision MakeDecision(const std::string& context, const std::vector<std::string>& options) override;
    void ProcessCurrentTask() override;
    
private:
    std::string GenerateCode(const std::string& description);
    bool OptimizeCode(const std::string& code);
};

/**
 * 設計代理 - 專注於創意和設計任務
 */
class DesignerAgent : public AIAgent {
public:
    DesignerAgent(const AgentDesc& desc);
    
    Decision MakeDecision(const std::string& context, const std::vector<std::string>& options) override;
    void ProcessCurrentTask() override;
    
private:
    std::string GenerateDesign(const std::string& description);
    std::vector<std::string> BrainstormIdeas(const std::string& topic);
};

/**
 * 分析代理 - 專注於數據分析和優化
 */
class AnalystAgent : public AIAgent {
public:
    AnalystAgent(const AgentDesc& desc);
    
    Decision MakeDecision(const std::string& context, const std::vector<std::string>& options) override;
    void ProcessCurrentTask() override;
    
private:
    std::string AnalyzeData(const std::string& data);
    std::vector<std::string> GenerateInsights(const std::string& analysis);
};

/**
 * AI 代理管理器
 */
class AIAgentManager {
public:
    AIAgentManager();
    ~AIAgentManager();
    
    // 生命周期
    bool Initialize();
    void Shutdown();
    
    // 代理管理
    AIAgent* CreateAgent(const AgentDesc& desc);
    void DestroyAgent(const std::string& agentId);
    AIAgent* GetAgent(const std::string& agentId);
    const std::vector<AIAgent*>& GetAgents() const { return agents; }
    
    // 任務分配
    void AssignTaskToAgent(const std::string& agentId, const AgentTask& task);
    void AssignTaskToBestAgent(const AgentTask& task);
    void AssignTaskToAllAgents(const AgentTask& task);
    
    // 集體決策
    Decision MakeGroupDecision(const std::string& context, const std::vector<std::string>& options);
    
    // 更新
    void Update(float deltaTime);
    
    // 學習系統
    void EnableLearning(bool enable);
    bool IsLearningEnabled() const;
    void RecordGroupLearning(const std::string& context, const std::string& action, bool success);
    
    // 統計
    size_t GetAgentCount() const { return agents.size(); }
    size_t GetActiveAgentCount() const;
    size_t GetPendingTaskCount() const;
    size_t GetCompletedTaskCount() const;
    
    // 配置
    void SetMaxAgents(size_t maxAgents);
    void SetUpdateInterval(float interval);
    
private:
    std::vector<AIAgent*> agents;
    std::unordered_map<std::string, AIAgent*> agentMap;
    
    bool learningEnabled;
    float updateInterval;
    float updateTimer;
    
    size_t maxAgents;
    
    std::mutex agentsMutex;
    std::mutex tasksMutex;
    
    AIAgent* FindBestAgentForTask(const AgentTask& task);
    void UpdateAgentPerformance();
};

/**
 * AI 代理事件
 */
struct AgentEvent {
    enum class Type {
        Created,
        TaskAssigned,
        TaskCompleted,
        TaskFailed,
        DecisionMade,
        LearningOccurred
    };
    
    Type type;
    std::string agentId;
    std::string data;
    uint64 timestamp;
};

/**
 * AI 代理系統
 * 整合所有 AI 代理功能的主系統
 */
class AIAgentSystem {
public:
    AIAgentSystem();
    ~AIAgentSystem();
    
    // 初始化
    bool Initialize();
    void Shutdown();
    
    // 主系統
    void Update(float deltaTime);
    
    // 代理管理
    AIAgentManager* GetAgentManager() { return agentManager.get(); }
    
    // 事件系統
    void SetEventBus(EventBus::EventBus* eventBus);
    
    // 配置
    void SetMaxAgents(size_t maxAgents);
    void SetLearningEnabled(bool enabled);
    
    // 統計
    void PrintStatistics();
    
private:
    std::unique_ptr<AIAgentManager> agentManager;
    EventBus::EventBus* eventBus;
    
    bool initialized;
};

} // namespace AI
} // namespace Potato