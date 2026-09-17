/**
 * MingGoRTS IDE Core Framework Implementation
 */

#include "IDECore.h"
#include "AIIntegration.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <thread>

namespace MingGoRTSIDE {

// ============================================================================
// IDECore Implementation
// ============================================================================

IDECore::IDECore()
    : running(false) {
}

IDECore::~IDECore() {
    Shutdown();
}

bool IDECore::Initialize(const IDEConfig& newConfig) {
    std::cout << "Initializing MingGoRTS IDE..." << std::endl;
    
    this->config = newConfig;
    
    // Initialize components
    project = std::make_unique<IDEProject>();
    editor = std::make_unique<IDEEditor>();
    navigator = std::make_unique<IDENavigator>();
    builder = std::make_unique<IDEBuilder>();
    aiInterface = std::make_unique<AIAgentInterface>();
    
    // Set up workspace
    if (!config.workspacePath.empty()) {
        navigator->SetRootPath(config.workspacePath);
    }
    
    // Initialize AI Interface
    if (config.enableAIAssistance) {
        aiInterface->Initialize();
        std::cout << "AI Assistant enabled" << std::endl;
    }
    
    std::cout << "MingGoRTS IDE initialized successfully" << std::endl;
    return true;
}

void IDECore::Shutdown() {
    std::cout << "Shutting down MingGoRTS IDE..." << std::endl;
    
    if (project) {
        project->CloseProject();
    }
    
    if (aiInterface) {
        aiInterface->Shutdown();
    }
    
    running = false;
}

void IDECore::Run() {
    running = true;
    std::cout << "MingGoRTS IDE running..." << std::endl;
    
    // Main IDE loop
    while (running) {
        // Process events
        // Update UI
        // Handle AI requests
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void IDECore::Stop() {
    running = false;
    std::cout << "MingGoRTS IDE stopped" << std::endl;
}

void IDECore::RegisterEventHandler(IDEEventType type, IDEEventHandler handler) {
    eventHandlers[type].push_back(handler);
}

void IDECore::EmitEvent(const IDEEvent& event) {
    auto it = eventHandlers.find(event.type);
    if (it != eventHandlers.end()) {
        for (const auto& handler : it->second) {
            handler(event);
        }
    }
}

// ============================================================================
// IDEProject Implementation
// ============================================================================

IDEProject::IDEProject()
    : isOpen(false) {
}

IDEProject::~IDEProject() {
    CloseProject();
}

bool IDEProject::CreateProject(const std::string& path, const std::string& name) {
    std::cout << "Creating project: " << name << " at " << path << std::endl;
    
    projectPath = path;
    projectName = name;
    isOpen = true;
    
    return true;
}

bool IDEProject::OpenProject(const std::string& path) {
    std::cout << "Opening project: " << path << std::endl;
    
    projectPath = path;
    // Extract project name from path
    size_t lastSlash = path.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        projectName = path.substr(lastSlash + 1);
    } else {
        projectName = path;
    }
    
    isOpen = true;
    return true;
}

bool IDEProject::CloseProject() {
    if (isOpen) {
        std::cout << "Closing project: " << projectName << std::endl;
        isOpen = false;
        projectFiles.clear();
    }
    return true;
}

std::vector<std::string> IDEProject::GetProjectFiles() const {
    return projectFiles;
}

bool IDEProject::AddFile(const std::string& filePath) {
    if (std::find(projectFiles.begin(), projectFiles.end(), filePath) == projectFiles.end()) {
        projectFiles.push_back(filePath);
        std::cout << "Added file to project: " << filePath << std::endl;
        return true;
    }
    return false;
}

bool IDEProject::RemoveFile(const std::string& filePath) {
    auto it = std::find(projectFiles.begin(), projectFiles.end(), filePath);
    if (it != projectFiles.end()) {
        projectFiles.erase(it);
        std::cout << "Removed file from project: " << filePath << std::endl;
        return true;
    }
    return false;
}

// ============================================================================
// IDEEditor Implementation
// ============================================================================

IDEEditor::IDEEditor() {
}

IDEEditor::~IDEEditor() {
    CloseFile(currentFile);
}

bool IDEEditor::OpenFile(const std::string& filePath) {
    std::cout << "Opening file: " << filePath << std::endl;
    
    currentFile = filePath;
    
    if (std::find(openFiles.begin(), openFiles.end(), filePath) == openFiles.end()) {
        openFiles.push_back(filePath);
    }
    
    // In a real implementation, this would load the file content
    fileContents[filePath] = "Sample file content for " + filePath;
    
    return true;
}

bool IDEEditor::SaveFile(const std::string& filePath) {
    std::cout << "Saving file: " << filePath << std::endl;
    
    // In a real implementation, this would save the file content
    return true;
}

bool IDEEditor::CloseFile(const std::string& filePath) {
    std::cout << "Closing file: " << filePath << std::endl;
    
    if (currentFile == filePath) {
        currentFile.clear();
    }
    
    auto it = std::find(openFiles.begin(), openFiles.end(), filePath);
    if (it != openFiles.end()) {
        openFiles.erase(it);
    }
    
    return true;
}

std::string IDEEditor::GetFileContent(const std::string& filePath) const {
    auto it = fileContents.find(filePath);
    if (it != fileContents.end()) {
        return it->second;
    }
    return "";
}

bool IDEEditor::SetFileContent(const std::string& filePath, const std::string& content) {
    fileContents[filePath] = content;
    return true;
}

std::vector<std::string> IDEEditor::GetOpenFiles() const {
    return openFiles;
}

// ============================================================================
// IDENavigator Implementation
// ============================================================================

IDENavigator::IDENavigator() {
}

IDENavigator::~IDENavigator() {
}

bool IDENavigator::SetRootPath(const std::string& path) {
    std::cout << "Setting root path: " << path << std::endl;
    rootPath = path;
    return true;
}

std::vector<std::string> IDENavigator::GetDirectoryContents(const std::string& path) const {
    std::cout << "Getting directory contents: " << path << std::endl;
    
    // In a real implementation, this would use filesystem API
    return {"file1.cpp", "file2.h", "directory1"};
}

bool IDENavigator::CreateDirectory(const std::string& path) {
    std::cout << "Creating directory: " << path << std::endl;
    return true;
}

bool IDENavigator::DeleteDirectory(const std::string& path) {
    std::cout << "Deleting directory: " << path << std::endl;
    return true;
}

std::vector<std::string> IDENavigator::SearchFiles(const std::string& pattern) const {
    std::cout << "Searching files with pattern: " << pattern << std::endl;
    return {"found_file1.cpp", "found_file2.h"};
}

// ============================================================================
// IDEBuilder Implementation
// ============================================================================

IDEBuilder::IDEBuilder()
    : buildStatus("Not configured") {
}

IDEBuilder::~IDEBuilder() {
}

bool IDEBuilder::ConfigureBuildSystem(const std::string& newBuildType) {
    std::cout << "Configuring build system: " << newBuildType << std::endl;
    
    this->buildType = newBuildType;
    buildStatus = "Configured";
    buildErrors.clear();
    buildWarnings.clear();
    
    return true;
}

bool IDEBuilder::BuildProject(const std::string& configuration) {
    std::cout << "Building project with configuration: " << configuration << std::endl;
    
    buildStatus = "Building";
    
    // Simulate build process
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    buildStatus = "Build completed";
    buildWarnings.push_back("Minor warning during build");
    
    return true;
}

bool IDEBuilder::CleanProject() {
    std::cout << "Cleaning project" << std::endl;
    
    buildStatus = "Cleaned";
    buildErrors.clear();
    buildWarnings.clear();
    
    return true;
}

// ============================================================================
// AIAgentInterface Implementation
// ============================================================================

AIAgentInterface::AIAgentInterface()
    : available(false) {
}

AIAgentInterface::~AIAgentInterface() {
    Shutdown();
}

bool AIAgentInterface::Initialize() {
    std::cout << "Initializing AI Agent Interface..." << std::endl;
    
    // Initialize IDE Agent Manager
    agentManager = std::make_unique<IDEAgentManager>();
    if (!agentManager->Initialize()) {
        std::cerr << "Failed to initialize IDE Agent Manager" << std::endl;
        return false;
    }
    
    // Initialize IDE Context Manager
    contextManager = std::make_unique<IDEContextManager>();

    // 啟用預設 agent 編制——面板與 ProcessRequest 依賴非空 agent 集
    agentManager->CreateAgent(GameDevAgentType::EngineCode, "CodeAssistant");
    agentManager->CreateAgent(GameDevAgentType::GameDesigner, "Designer");
    agentManager->CreateAgent(GameDevAgentType::Performance, "PerfAnalyst");
    agentManager->CreateAgent(GameDevAgentType::BuildAgent, "BuildKeeper");

    // This will integrate with Potato Engine AI Agent system
    available = true;
    
    std::cout << "AI Agent Interface initialized successfully" << std::endl;
    return true;
}

void AIAgentInterface::Shutdown() {
    std::cout << "Shutting down AI Agent Interface..." << std::endl;
    
    if (agentManager) {
        agentManager->Shutdown();
        agentManager.reset();
    }
    
    if (contextManager) {
        contextManager.reset();
    }
    
    available = false;
}

namespace {

// 關鍵字意圖分派（中英雙語）——回傳命中的意圖種類
bool ContainsAny(const std::string& s, std::initializer_list<const char*> kws) {
    for (const char* kw : kws) {
        if (s.find(kw) != std::string::npos) return true;
    }
    return false;
}

std::string ToLowerCopy(std::string s) {
    for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

} // namespace

std::string AIAgentInterface::ProcessRequest(const std::string& request) {
    std::cout << "Processing AI request: " << request << std::endl;
    if (!available || !agentManager) {
        return "AI Agent system not available";
    }

    const std::string lower = ToLowerCopy(request);
    SimplifiedAI::Context ctx = contextManager
        ? contextManager->GetCurrentContext() : SimplifiedAI::Context{};

    // 意圖 → 專責 agent。各 manager 方法內部已先嘗試智能後端
    // （SetBackend 由 SetAssistantHook 轉發），回空才走 agent 模板。
    if (ContainsAny(lower, {"generate", "create", "write", "implement",
                            "生成", "產生", "新增", "寫一", "寫個", "建立"})) {
        return "[CodeAssistant] " +
               agentManager->GenerateCode(request, GameDevAgentType::EngineCode, ctx);
    }
    if (ContainsAny(lower, {"analyze", "review", "check", "fix", "bug",
                            "分析", "檢查", "審查", "修"})) {
        return "[PerfAnalyst] " +
               agentManager->AnalyzePerformance(request, ctx);
    }
    if (ContainsAny(lower, {"level", "map", "stage", "design",
                            "關卡", "地圖", "設計"})) {
        return "[Designer] " + agentManager->DesignLevel(request, ctx);
    }
    if (ContainsAny(lower, {"build", "compile", "編譯", "建置", "連結"})) {
        return "[BuildKeeper] " + agentManager->OptimizeBuild(request, ctx);
    }
    if (ContainsAny(lower, {"performance", "optimize", "slow", "效能", "優化", "卡"})) {
        return "[PerfAnalyst] " + agentManager->AnalyzePerformance(request, ctx);
    }

    // 一般問答：智能後端優先，其次第一個 agent 的 IDE 任務處理
    if (assistantHook) {
        std::string r = assistantHook("chat: " + request);
        if (!r.empty()) return "[AI] " + r;
    }
    auto agents = agentManager->GetAllAgents();
    if (!agents.empty()) {
        return "[AI] " + agents[0]->ProcessIDETask(request, ctx);
    }
    return "No agents available";
}

std::string AIAgentInterface::ProcessCodeRequest(const std::string& code, const std::string& context) {
    std::cout << "Processing code request with context" << std::endl;
    if (!available || !agentManager) {
        return "AI Agent system not available";
    }

    SimplifiedAI::Context ctx = contextManager
        ? contextManager->GetCurrentContext() : SimplifiedAI::Context{};
    ctx.currentTask = context;

    if (assistantHook) {
        std::string r = assistantHook(code);
        if (!r.empty()) return r;
    }
    return agentManager->GenerateCode(code, GameDevAgentType::EngineCode, ctx);
}

std::string AIAgentInterface::ProcessBuildRequest(const std::string& buildConfig) {
    std::cout << "Processing build request: " << buildConfig << std::endl;
    if (!available || !agentManager) {
        return "AI Agent system not available";
    }

    SimplifiedAI::Context ctx = contextManager
        ? contextManager->GetCurrentContext() : SimplifiedAI::Context{};
    return agentManager->OptimizeBuild(buildConfig, ctx);
}

void AIAgentInterface::SetAssistantHook(
    std::function<std::string(const std::string&)> hook) {
    assistantHook = hook;
    if (agentManager) {
        agentManager->SetBackend(std::move(hook));
    }
}

size_t AIAgentInterface::GetActiveAgentCount() const {
    if (agentManager) {
        return agentManager->GetAllAgents().size();
    }
    return 0;
}

} // namespace MingGoRTSIDE