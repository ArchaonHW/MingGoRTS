#include "AIAgentSystem.h"
#include "ECS/Entity.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

namespace Potato {
namespace AI {

// ============================================================================
// AIAgent 實現
// ============================================================================

AIAgent::AIAgent(const AgentDesc& desc)
    : name(desc.name)
    , type(desc.type)
    , autonomous(desc.autonomous)
    , performanceRating(desc.performanceRating)
    , learningRate(desc.learningRate)
    , capabilities(desc.capabilities)
    , currentTaskIndex(-1)
    , active(true)
    , completedTaskCount(0)
    , failedTaskCount(0) {
    
    std::cout << "創建 AI 代理: " << name << " (類型: " << static_cast<int>(type) << ")" << std::endl;
}

AIAgent::~AIAgent() {
    std::cout << "銷毀 AI 代理: " << name << std::endl;
}

bool AIAgent::Initialize() {
    std::cout << "初始化 AI 代理: " << name << std::endl;
    return true;
}

void AIAgent::Shutdown() {
    std::cout << "關閉 AI 代理: " << name << std::endl;
}

void AIAgent::AssignTask(const AgentTask& task) {
    std::lock_guard<std::mutex> lock(taskMutex);
    
    tasks.push_back(task);
    
    if (currentTaskIndex < 0) {
        currentTaskIndex = 0;
    }
    
    std::cout << "分配任務給代理 " << name << ": " << task.description << std::endl;
}

void AIAgent::CompleteTask(const std::string& result) {
    std::lock_guard<std::mutex> lock(taskMutex);
    
    if (currentTaskIndex >= 0 && currentTaskIndex < static_cast<int>(tasks.size())) {
        tasks[currentTaskIndex].status = TaskStatus::Completed;
        tasks[currentTaskIndex].result = result;
        tasks[currentTaskIndex].progress = 1.0f;
        tasks[currentTaskIndex].completedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        
        completedTaskCount++;
        
        std::cout << "代理 " << name << " 完成任務: " << result << std::endl;
        
        // 移動到下一個任務
        currentTaskIndex++;
        if (currentTaskIndex >= static_cast<int>(tasks.size())) {
            currentTaskIndex = -1;
        }
    }
}

void AIAgent::FailTask(const std::string& error) {
    std::lock_guard<std::mutex> lock(taskMutex);
    
    if (currentTaskIndex >= 0 && currentTaskIndex < static_cast<int>(tasks.size())) {
        tasks[currentTaskIndex].status = TaskStatus::Failed;
        tasks[currentTaskIndex].error = error;
        
        failedTaskCount++;
        
        std::cout << "代理 " << name << " 任務失敗: " << error << std::endl;
        
        // 移動到下一個任務
        currentTaskIndex++;
        if (currentTaskIndex >= static_cast<int>(tasks.size())) {
            currentTaskIndex = -1;
        }
    }
}

void AIAgent::CancelTask(const std::string& taskId) {
    std::lock_guard<std::mutex> lock(taskMutex);
    
    for (auto& task : tasks) {
        if (task.id == taskId) {
            task.status = TaskStatus::Cancelled;
            break;
        }
    }
}

void AIAgent::Update(float deltaTime) {
    if (!active) return;
    
    ProcessCurrentTask();
}

void AIAgent::ProcessCurrentTask() {
    std::lock_guard<std::mutex> lock(taskMutex);
    
    if (currentTaskIndex < 0 || currentTaskIndex >= static_cast<int>(tasks.size())) {
        return;
    }
    
    AgentTask& task = tasks[currentTaskIndex];
    
    if (task.status == TaskStatus::Pending) {
        task.status = TaskStatus::InProgress;
        std::cout << "代理 " << name << " 開始處理任務: " << task.description << std::endl;
    }
    
    if (task.status == TaskStatus::InProgress) {
        // 模擬任務進度
        task.progress += 0.01f * performanceRating;
        
        if (task.progress >= 1.0f) {
            task.progress = 1.0f;
            CompleteTask("任務完成");
        }
    }
}

Decision AIAgent::MakeDecision(const std::string& context, const std::vector<std::string>& options) {
    Decision decision;
    
    if (options.empty()) {
        decision.confidence = 0.0f;
        return decision;
    }
    
    // 簡單的決策邏輯
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, options.size() - 1);
    
    decision.action = options[dis(gen)];
    decision.confidence = 0.5f + (static_cast<float>(dis(gen)) / 100.0f);
    decision.reasoning.push_back("基於當前上下文的決策");
    decision.timestamp = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count());
    
    std::cout << "代理 " << name << " 決策: " << decision.action << " (信心: " << decision.confidence << ")" << std::endl;
    
    return decision;
}

void AIAgent::RecordLearning(const std::string& context, const std::string& action, bool success) {
    std::lock_guard<std::mutex> lock(learningMutex);
    
    LearningData data;
    data.context = context;
    data.action = action;
    data.success = success;
    data.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    
    learningHistory.push_back(data);
    
    // 限制學習歷史大小
    if (learningHistory.size() > 1000) {
        learningHistory.erase(learningHistory.begin());
    }
    
    std::cout << "代理 " << name << " 記錄學習: " << action << " = " << (success ? "成功" : "失敗") << std::endl;
}

float AIAgent::GetSuccessRate() const {
    size_t total = completedTaskCount + failedTaskCount;
    if (total == 0) return 1.0f;
    return static_cast<float>(completedTaskCount) / static_cast<float>(total);
}

// ============================================================================
// DeveloperAgent 實現
// ============================================================================

DeveloperAgent::DeveloperAgent(const AgentDesc& desc)
    : AIAgent(desc) {
}

Decision DeveloperAgent::MakeDecision(const std::string& context, const std::vector<std::string>& options) {
    Decision decision = AIAgent::MakeDecision(context, options);
    
    // 開發代理的決策可能考慮代碼效率和最佳實踐
    decision.reasoning.push_back("考慮代碼效率和最佳實踐");
    
    return decision;
}

void DeveloperAgent::ProcessCurrentTask() {
    AIAgent::ProcessCurrentTask();
    
    std::lock_guard<std::mutex> lock(taskMutex);
    
    if (currentTaskIndex >= 0 && currentTaskIndex < static_cast<int>(tasks.size())) {
        AgentTask& task = tasks[currentTaskIndex];
        
        if (task.status == TaskStatus::InProgress && task.category == "Code Generation") {
            std::string code = GenerateCode(task.description);
            if (!code.empty()) {
                CompleteTask("生成代碼: " + code);
            }
        }
    }
}

std::string DeveloperAgent::GenerateCode(const std::string& description) {
    std::cout << "開發代理生成代碼: " << description << std::endl;
    
    // 簡化的代碼生成邏輯
    std::string code = "void " + description + "() {\n    // Implementation\n}";
    
    return code;
}

bool DeveloperAgent::OptimizeCode(const std::string& code) {
    std::cout << "開發代理優化代碼" << std::endl;
    return true;
}

// ============================================================================
// DesignerAgent 實現
// ============================================================================

DesignerAgent::DesignerAgent(const AgentDesc& desc)
    : AIAgent(desc) {
}

Decision DesignerAgent::MakeDecision(const std::string& context, const std::vector<std::string>& options) {
    Decision decision = AIAgent::MakeDecision(context, options);
    
    // 設計代理的決策可能考慮用戶體驗和創意性
    decision.reasoning.push_back("考慮用戶體驗和創意性");
    
    return decision;
}

void DesignerAgent::ProcessCurrentTask() {
    AIAgent::ProcessCurrentTask();
    
    std::lock_guard<std::mutex> lock(taskMutex);
    
    if (currentTaskIndex >= 0 && currentTaskIndex < static_cast<int>(tasks.size())) {
        AgentTask& task = tasks[currentTaskIndex];
        
        if (task.status == TaskStatus::InProgress && task.category == "Design") {
            std::string design = GenerateDesign(task.description);
            if (!design.empty()) {
                CompleteTask("生成設計: " + design);
            }
        }
    }
}

std::string DesignerAgent::GenerateDesign(const std::string& description) {
    std::cout << "設計代理生成設計: " << description << std::endl;
    
    // 簡化的設計生成邏輯
    std::string design = "Design concept for: " + description;
    
    return design;
}

std::vector<std::string> DesignerAgent::BrainstormIdeas(const std::string& topic) {
    std::cout << "設計代理頭腦風暴: " << topic << std::endl;
    
    std::vector<std::string> ideas;
    ideas.push_back("Idea 1 for " + topic);
    ideas.push_back("Idea 2 for " + topic);
    ideas.push_back("Idea 3 for " + topic);
    
    return ideas;
}

// ============================================================================
// AnalystAgent 實現
// ============================================================================

AnalystAgent::AnalystAgent(const AgentDesc& desc)
    : AIAgent(desc) {
}

Decision AnalystAgent::MakeDecision(const std::string& context, const std::vector<std::string>& options) {
    Decision decision = AIAgent::MakeDecision(context, options);
    
    // 分析代理的決策可能考慮數據驅證和統計
    decision.reasoning.push_back("考慮數據驗證和統計");
    
    return decision;
}

void AnalystAgent::ProcessCurrentTask() {
    AIAgent::ProcessCurrentTask();
    
    std::lock_guard<std::mutex> lock(taskMutex);
    
    if (currentTaskIndex >= 0 && currentTaskIndex < static_cast<int>(tasks.size())) {
        AgentTask& task = tasks[currentTaskIndex];
        
        if (task.status == TaskStatus::InProgress && task.category == "Analysis") {
            std::string analysis = AnalyzeData(task.description);
            if (!analysis.empty()) {
                CompleteTask("分析結果: " + analysis);
            }
        }
    }
}

std::string AnalystAgent::AnalyzeData(const std::string& data) {
    std::cout << "分析代理分析數據: " << data << std::endl;
    
    // 簡化的數據分析邏輯
    std::string analysis = "Analysis of: " + data;
    
    return analysis;
}

std::vector<std::string> AnalystAgent::GenerateInsights(const std::string& analysis) {
    std::cout << "分析代理生成洞察: " << analysis << std::endl;
    
    std::vector<std::string> insights;
    insights.push_back("Insight 1 from " + analysis);
    insights.push_back("Insight 2 from " + analysis);
    
    return insights;
}

// ============================================================================
// AIAgentManager 實現
// ============================================================================

AIAgentManager::AIAgentManager()
    : learningEnabled(false)
    , updateInterval(0.1f)
    , updateTimer(0.0f)
    , maxAgents(100) {
}

AIAgentManager::~AIAgentManager() {
    Shutdown();
}

bool AIAgentManager::Initialize() {
    std::cout << "初始化 AI 代理管理器..." << std::endl;
    std::cout << "最大代理數量: " << maxAgents << std::endl;
    std::cout << "更新間隔: " << updateInterval << " 秒" << std::endl;
    std::cout << "學習系統: " << (learningEnabled ? "啟用" : "禁用") << std::endl;
    return true;
}

void AIAgentManager::Shutdown() {
    std::cout << "關閉 AI 代理管理器..." << std::endl;
    
    std::lock_guard<std::mutex> lock(agentsMutex);
    
    for (auto agent : agents) {
        agent->Shutdown();
        delete agent;
    }
    agents.clear();
    agentMap.clear();
    
    std::cout << "✓ AI 代理管理器關閉完成" << std::endl;
}

AIAgent* AIAgentManager::CreateAgent(const AgentDesc& desc) {
    std::lock_guard<std::mutex> lock(agentsMutex);
    
    if (agents.size() >= maxAgents) {
        std::cerr << "錯誤: 已達到最大代理數量 " << maxAgents << std::endl;
        return nullptr;
    }
    
    AIAgent* agent = nullptr;
    
    // 根�類型創建特定代理
    switch (desc.type) {
        case AgentType::Developer:
            agent = new DeveloperAgent(desc);
            break;
        case AgentType::Designer:
            agent = new DesignerAgent(desc);
            break;
        case AgentType::Analyst:
            agent = new AnalystAgent(desc);
            break;
        default:
            agent = new AIAgent(desc);
            break;
    }
    
    if (agent && agent->Initialize()) {
        agents.push_back(agent);
        agentMap[agent->GetName()] = agent;
        
        std::cout << "✓ 創建代理: " << agent->GetName() << std::endl;
        return agent;
    }
    
    if (agent) {
        delete agent;
    }
    
    return nullptr;
}

void AIAgentManager::DestroyAgent(const std::string& agentId) {
    std::lock_guard<std::mutex> lock(agentsMutex);
    
    auto it = agentMap.find(agentId);
    if (it != agentMap.end()) {
        AIAgent* agent = it->second;
        
        agent->Shutdown();
        
        agents.erase(std::remove(agents.begin(), agents.end(), agent), agents.end());
        agentMap.erase(it);
        
        delete agent;
        
        std::cout << "✓ 銷毀代理: " << agentId << std::endl;
    }
}

AIAgent* AIAgentManager::GetAgent(const std::string& agentId) {
    std::lock_guard<std::mutex> lock(agentsMutex);
    
    auto it = agentMap.find(agentId);
    if (it != agentMap.end()) {
        return it->second;
    }
    return nullptr;
}

void AIAgentManager::AssignTaskToAgent(const std::string& agentId, const AgentTask& task) {
    std::lock_guard<std::mutex> lock(agentsMutex);
    
    AIAgent* agent = GetAgent(agentId);
    if (agent) {
        agent->AssignTask(task);
    } else {
        std::cerr << "錯誤: 找不到代理 " << agentId << std::endl;
    }
}

void AIAgentManager::AssignTaskToBestAgent(const AgentTask& task) {
    AIAgent* bestAgent = FindBestAgentForTask(task);
    if (bestAgent) {
        bestAgent->AssignTask(task);
        std::cout << "分配任務給最佳代理: " << bestAgent->GetName() << std::endl;
    } else {
        std::cerr << "錯誤: 無可用代理" << std::endl;
    }
}

void AIAgentManager::AssignTaskToAllAgents(const AgentTask& task) {
    std::lock_guard<std::mutex> lock(agentsMutex);
    
    for (auto agent : agents) {
        AgentTask taskCopy = task;
        taskCopy.id = task.id + "_" + agent->GetName();
        agent->AssignTask(taskCopy);
    }
    
    std::cout << "分配任務給所有代理: " << task.description << std::endl;
}

Decision AIAgentManager::MakeGroupDecision(const std::string& context, const std::vector<std::string>& options) {
    Decision decision;
    
    std::lock_guard<std::mutex> lock(agentsMutex);
    
    if (agents.empty()) {
        return decision;
    }
    
    // 簡化的群體決策邏輯
    std::vector<Decision> decisions;
    for (auto agent : agents) {
        decisions.push_back(agent->MakeDecision(context, options));
    }
    
    // 選擇信心最高的決策
    auto bestDecision = std::max_element(decisions.begin(), decisions.end(),
        [](const Decision& a, const Decision& b) {
            return a.confidence < b.confidence;
        });
    
    if (bestDecision != decisions.end()) {
        decision = *bestDecision;
        decision.reasoning.push_back("群體決策，基於 " + std::to_string(decisions.size()) + " 個代理的投票");
    }
    
    std::cout << "群體決策: " << decision.action << " (信心: " << decision.confidence << ")" << std::endl;
    
    return decision;
}

void AIAgentManager::Update(float deltaTime) {
    updateTimer += deltaTime;
    
    if (updateTimer >= updateInterval) {
        updateTimer = 0.0f;
        
        std::lock_guard<std::mutex> lock(agentsMutex);
        
        for (auto agent : agents) {
            agent->Update(updateInterval);
        }
        
        UpdateAgentPerformance();
    }
}

void AIAgentManager::EnableLearning(bool enable) {
    learningEnabled = enable;
    std::cout << "學習系統: " << (enable ? "啟用" : "禁用") << std::endl;
}

bool AIAgentManager::IsLearningEnabled() const {
    return learningEnabled;
}

void AIAgentManager::RecordGroupLearning(const std::string& context, const std::string& action, bool success) {
    if (learningEnabled) {
        std::cout << "記錄群體學習: " << action << " = " << (success ? "成功" : "失敗") << std::endl;
    }
}

size_t AIAgentManager::GetActiveAgentCount() const {
    std::lock_guard<std::mutex> lock(agentsMutex);
    
    size_t count = 0;
    for (const auto& agent : agents) {
        if (agent->IsActive()) {
            count++;
        }
    }
    return count;
}

size_t AIAgentManager::GetPendingTaskCount() const {
    std::lock_guard<std::mutex> lock(agentsMutex);
    
    size_t count = 0;
    for (const auto& agent : agents) {
        count += agent->GetTasks().size();
    }
    return count;
}

size_t AIAgentManager::GetCompletedTaskCount() const {
    std::lock_guard<std::mutex> lock(agentsMutex);
    
    size_t count = 0;
    for (const auto& agent : agents) {
        count += agent->GetCompletedTaskCount();
    }
    return count;
}

void AIAgentManager::SetMaxAgents(size_t max) {
    maxAgents = max;
    std::cout << "設置最大代理數量: " << max << std::endl;
}

void AIAgentManager::SetUpdateInterval(float interval) {
    updateInterval = interval;
    std::cout << "設置更新間隔: " << interval << " 秒" << std::endl;
}

AIAgent* AIAgentManager::FindBestAgentForTask(const AgentTask& task) {
    std::lock_guard<std::mutex> lock(agentsMutex);
    
    AIAgent* bestAgent = nullptr;
    float bestScore = 0.0f;
    
    for (auto agent : agents) {
        if (!agent->IsActive()) continue;
        if (agent->GetCurrentTask()) continue;
        
        // 簡化的評分邏輯
        float score = agent->GetPerformanceRating();
        
        if (score > bestScore) {
            bestScore = score;
            bestAgent = agent;
        }
    }
    
    return bestAgent;
}

void AIAgentManager::UpdateAgentPerformance() {
    // 根據成功率和學習更新代理性能評分
    std::lock_guard<std::mutex> lock(agentsMutex);
    
    for (auto agent : agents) {
        float successRate = agent->GetSuccessRate();
        float newRating = 0.5f + (successRate * 0.5f);
        
        // 更新性能評分（需要訪問成員變量，這裡簡化處理）
        std::cout << "代理 " << agent->GetName() << " 成功率: " << successRate << ", 新評分: " << newRating << std::endl;
    }
}

// ============================================================================
// AIAgentSystem 實現
// ============================================================================

AIAgentSystem::AIAgentSystem()
    : agentManager(new AIAgentManager())
    , eventBus(nullptr)
    , initialized(false) {
}

AIAgentSystem::~AIAgentSystem() {
    Shutdown();
}

bool AIAgentSystem::Initialize() {
    std::cout << "初始化 AI 代理系統..." << std::endl;
    
    if (!agentManager->Initialize()) {
        std::cerr << "錯誤: 代理管理器初始化失敗" << std::endl;
        return false;
    }
    
    initialized = true;
    std::cout << "✓ AI 代理系統初始化成功" << std::endl;
    
    return true;
}

void AIAgentSystem::Shutdown() {
    if (!initialized) {
        return;
    }
    
    std::cout << "關閉 AI 代理系統..." << std::endl;
    
    if (agentManager) {
        agentManager->Shutdown();
    }
    
    initialized = false;
    std::cout << "✓ AI 代理系統關閉完成" << std::endl;
}

void AIAgentSystem::Update(float deltaTime) {
    if (!initialized) {
        return;
    }
    
    if (agentManager) {
        agentManager->Update(deltaTime);
    }
}

void AIAgentSystem::SetEventBus(EventBus::EventBus* eventBus) {
    this->eventBus = eventBus;
}

void AIAgentSystem::SetMaxAgents(size_t maxAgents) {
    if (agentManager) {
        agentManager->SetMaxAgents(maxAgents);
    }
}

void AIAgentSystem::SetLearningEnabled(bool enabled) {
    if (agentManager) {
        agentManager->EnableLearning(enabled);
    }
}

void AIAgentSystem::PrintStatistics() {
    if (!agentManager) {
        return;
    }
    
    std::cout << "\n📊 AI 代理系統統計:" << std::endl;
    std::cout << "  代理總數: " << agentManager->GetAgentCount() << std::endl;
    std::cout << "  活躍代理: " << agentManager->GetActiveAgentCount() << std::endl;
    std::cout << "  待處理任務: " << agentManager->GetPendingTaskCount() << std::endl;
    std::cout << "  已完成任務: " << agentManager->GetCompletedTaskCount() << std::endl;
    std::cout << "  學習系統: " << (agentManager->IsLearningEnabled() ? "啟用" : "禁用") << std::endl;
}

} // namespace AI
} // namespace Potato