/**
 * MingGoRTS IDE GUI Implementation
 * ImGui-based graphical user interface for AI-powered game development
 */

#pragma once

#include "../IDECore.h"
#include "../AIIntegration.h"
#include "../IntelligentSuggestion.h"
#include "../../AI/IntelligentDevelopmentSystem.h"
#include "I18N.h"
#include "imgui.h"
#include <string>
#include <vector>
#include <memory>
#include <future>
#include <unordered_map>

namespace MingGoRTSIDE {

/**
 * IDE GUI Window Panel Types
 */
enum class PanelType {
    FileExplorer,
    CodeEditor,
    AIAgentPanel,
    Terminal,
    Output,
    Properties,
    Settings,
    Help,
    Debugger,
    GitPanel,
    CodeAnalysis
};

// Debugger breakpoint structure
struct Breakpoint {
    std::string filePath;
    int lineNumber;
    bool enabled;
};

// Code analysis issue structure
struct CodeIssue {
    std::string filePath;
    int lineNumber;
    std::string severity; // error, warning, info
    std::string message;
};

/**
 * IDE GUI State
 */
struct IDEGUIState {
    bool running = true;
    bool showFileExplorer = true;
    bool showCodeEditor = true;
    bool showAIAgentPanel = true;
    bool showTerminal = true;
    bool showOutput = true;
    bool showProperties = false;
    bool showToolbar = true;
    bool showStatusBar = true;
    bool showSettings = false;
    bool showHelp = false;
    bool showDebugger = false;
    bool showGitPanel = false;
    bool showCodeAnalysis = false;
    
    // File explorer state
    std::string currentPath;
    std::vector<std::string> selectedFiles;
    bool showSearch = false;
    
    // Code editor state
    std::string currentFile;
    std::string editorContent;
    char editorBuffer[8192]; // Increased buffer size
    int currentLine = 1;
    int currentColumn = 1;
    int scrollY = 0;
    
    // Tab management
    struct EditorTab {
        std::string filePath;
        std::string fileName;
        std::string content;
        char buffer[8192];
        int currentLine = 1;
        int currentColumn = 1;
        bool modified = false;
    };
    std::vector<EditorTab> openTabs;
    int activeTab = -1;
    
    // Search functionality
    char searchBuffer[256];
    char replaceBuffer[256];
    bool searchCaseSensitive = false;
    bool searchWholeWord = false;
    int currentSearchResult = 0;
    int totalSearchResults = 0;
    
    // AI Agent panel state
    char aiInputBuffer[1024];
    std::vector<std::string> aiConversation;
    // chat 背景處理：submit 投到 worker，PollAIChatResult 每幀輪詢寫回——
    // 外部 LLM 連線（OpenAI/Anthropic/Ollama）時不阻塞 UI thread。
    // pendingIndex 指 conversation 裡 "thinking" placeholder 的位置，
    // 完成時原地替換以保訊息順序。
    std::future<std::string> aiChatFuture;
    bool aiChatPending = false;
    int aiChatPendingIndex = -1;
    bool aiScrollToBottom = false;
    
    // Terminal state
    char terminalBuffer[1024];
    std::vector<std::string> terminalHistory;
    int terminalHistoryIndex = -1;
    
    // Output state
    std::vector<std::string> outputLog;
    
    // Build system state
    std::string buildConfiguration = "Release";
    bool building = false;
    float buildProgress = 0.0f;
    
    // Settings state
    enum class Theme { Dark, Light, HighContrast };
    Theme currentTheme = Theme::Dark;
    int fontSize = 14;
    bool showLineNumbers = true;
    bool autoSave = true;
    int autoSaveInterval = 300; // seconds
    bool wordWrap = false;
    bool showMinimap = false;
    
    // Panel layout state
    ImVec2 fileExplorerPos = ImVec2(0, 60);
    ImVec2 fileExplorerSize = ImVec2(250, 400);
    ImVec2 codeEditorPos = ImVec2(250, 60);
    ImVec2 codeEditorSize = ImVec2(600, 400);
    ImVec2 aiAgentPos = ImVec2(850, 60);
    ImVec2 aiAgentSize = ImVec2(250, 400);
    ImVec2 terminalPos = ImVec2(0, 460);
    ImVec2 terminalSize = ImVec2(400, 200);
    ImVec2 outputPos = ImVec2(400, 460);
    ImVec2 outputSize = ImVec2(400, 200);
    ImVec2 propertiesPos = ImVec2(800, 460);
    ImVec2 propertiesSize = ImVec2(300, 200);
    
    // Template system
    std::vector<std::string> fileTemplates;
    std::string selectedTemplate;
    
    // Debugger state
    std::vector<Breakpoint> breakpoints;
    bool debugging = false;
    bool debugPaused = false;
    int currentDebugLine = -1;
    std::string debugExpression;
    
    // Git state
    std::string gitBranch;
    std::string gitStatus;
    std::vector<std::string> gitStagedFiles;
    std::vector<std::string> gitModifiedFiles;
    std::vector<std::string> gitCommitHistory;
    
    // Code analysis state
    std::vector<CodeIssue> codeIssues;
    bool analyzingCode = false;
    
    // Autocomplete state
    std::vector<std::string> autocompleteSuggestions;
    bool showAutocomplete = false;
    int autocompleteIndex = 0;
    
    // Intelligent suggestion system
    bool intelligentSuggestionsEnabled = true;
    std::vector<Suggestion> currentSuggestions;
    bool showSuggestions = false;
    int suggestionIndex = 0;
    
    // Intelligent Development System state
    bool showDevelopmentAssistant = false;
    char developmentPrompt[4096];
    char developmentResponse[8192];
    bool developmentProcessing = false;
    // 背景生成任務（非阻塞 UI）；worker 完成後在 render 迴圈輪詢寫回
    std::future<Potato::AI::CodeGenerationResult> devGenFuture;
    
    // Card Gallery（武將名冊）state
    bool showCardGallery = false;
    int cardGallerySelected = -1;

    // AI / LLM 設定（Settings 面板可覆寫；預設讀自 POTATO_LLM_* 環境變數）
    char llmProvider[64];
    char llmModel[128];
    char llmBaseUrl[256];
    char llmApiKey[256];
    char llmAgent[128];
};

/**
 * IDE GUI Main Class
 */
class IDEGUI {
public:
    IDEGUI();
    ~IDEGUI();
    
    bool Initialize(IDECore* core);
    void Shutdown();
    
    void Run();
    void Stop();
    
    // UI Rendering
    void RenderMainMenu();
    void RenderToolbar();
    void RenderStatusBar();
    void RenderFileExplorer();
    void RenderCodeEditor();
    void RenderAIAgentPanel();
    void RenderTerminal();
    void RenderOutput();
    void RenderProperties();
    void RenderSettings();
    void RenderHelp();
    void RenderDebugger();
    void RenderGitPanel();
    void RenderCodeAnalysis();
    
    // Enhanced Features
    void RenderEditorTabs();
    void RenderSearchPanel();
    void RenderBuildPanel();
    void RenderSyntaxHighlighting();
    void RenderTemplatesPanel();
    void RenderAutocomplete();
    
    // Tab Management
    void OpenTab(const std::string& filePath);
    void CloseTab(int index);
    void SwitchTab(int index);
    void SaveTab(int index);
    
    // Search Functionality
    void SearchInEditor(const std::string& searchText);
    void ReplaceInEditor(const std::string& searchText, const std::string& replaceText);
    void FindNext();
    void FindPrevious();
    
    // Debugger Functionality
    void ToggleBreakpoint(const std::string& filePath, int lineNumber);
    void StartDebugging();
    void StopDebugging();
    void StepOver();
    void StepInto();
    void StepOut();
    void ContinueDebug();
    void EvaluateExpression(const std::string& expression);
    
    // Git Functionality
    void InitializeGit();
    void GitStatus();
    void GitCommit(const std::string& message);
    void GitPush();
    void GitPull();
    void GitCreateBranch(const std::string& branchName);
    void GitSwitchBranch(const std::string& branchName);
    void GitDiff(const std::string& filePath);
    
    // Code Analysis Functionality
    void AnalyzeCurrentFile();
    void AnalyzeProject();
    void FixCodeIssue(int issueIndex);
    void RunCodeAnalysis();
    
    // Autocomplete Functionality
    void UpdateAutocomplete();
    void SelectAutocompleteSuggestion(int index);
    void InsertAutocomplete();
    std::vector<std::string> GetSuggestionsForContext(const std::string& context);
    bool ShouldShowAutocomplete();
    std::vector<std::string> GetCppKeywords();
    std::vector<std::string> GetStandardLibraryFunctions();
    
    // Intelligent Suggestions
    void UpdateIntelligentSuggestions();
    void RenderIntelligentSuggestions();
    void ApplySuggestion(const Suggestion& suggestion);
    void LearnFromSuggestion(const std::string& suggestionId, bool accepted);
    
    // Code Analysis Helpers
    int CalculateCyclomaticComplexity(const std::string& code);
    int CalculateNestingDepth(const std::string& code);
    std::vector<std::string> FindLongFunctions(const std::string& code, int maxLines = 50);
    
    // Intelligent Development System Integration
    void GenerateCodeFromPrompt();
    void PollDevelopmentResult();
    void ConfigureDevSystemLLM();
    void AnalyzeCodeWithAI();
    void GenerateTestsWithAI();
    void GenerateDocumentationWithAI();
    void FixBugWithAI();
    void OptimizeCodeWithAI();
    void ReviewCodeWithAI();
    void ShowDevelopmentAssistant();
    void RenderDevelopmentAssistant();

    // Card Gallery（武將名冊）：瀏覽 assets/cards 角色卡 + 立繪上屏
    void RenderCardGallery();
    
    // File System
    std::vector<std::string> ScanDirectory(const std::string& path);
    bool IsDirectory(const std::string& path);
    std::string GetFileName(const std::string& path);
    std::string GetFileExtension(const std::string& path);
    
    // Build System
    void StartBuild();
    void StopBuild();
    void UpdateBuildProgress();
    
    // Event Handling
    void HandleFileOpen(const std::string& filePath);
    void HandleFileSave(const std::string& filePath);
    void HandleAISubmit();
    void PollAIChatResult();
    void HandleTerminalCommand();
    void HandleKeyboardShortcuts();
    
    // State Management
    IDEGUIState& GetState() { return state; }
    
    // I18N
    void UpdateLanguage(Language lang);
    
private:
    IDECore* core;
    IDEGUIState state;
    bool running;
    
    // ImGui style
    void SetupImGuiStyle();
    void SetDarkTheme();
    
    // Layout management
    void SetupDefaultLayout();
    void SaveLayout();
    void LoadLayout();
    void ApplyLayout();
    
    // Helper functions
    void AddOutputLog(const std::string& message);
    void AddTerminalHistory(const std::string& command);
    void UpdateCursorPosition();
    std::string GetCurrentTime();
    
    // Template System
    void InitializeTemplates();
    void ApplyTemplate(const std::string& templateName);
    std::string GetTemplateContent(const std::string& templateName);
    
    // Syntax Highlighting
    bool IsKeyword(const std::string& word);
    bool IsComment(const std::string& line, size_t pos);
    bool IsString(const std::string& line, size_t pos);
    ImVec4 GetSyntaxColor(const std::string& token);
    
    // Debugger helpers
    void AddBreakpoint(const std::string& filePath, int lineNumber);
    void RemoveBreakpoint(const std::string& filePath, int lineNumber);
    bool HasBreakpoint(const std::string& filePath, int lineNumber);
    
    // Git helpers
    std::string ExecuteGitCommand(const std::string& command);
    void UpdateGitStatus();
    
    // Code analysis helpers
    void DetectSyntaxErrors();
    void DetectStyleIssues();
    void DetectPotentialBugs();

    // Card Gallery internals
    struct CardEntry {
        std::string jsonPath;
        std::string id, name, epithet, rarity, faction, artRel;
    };
    struct CardTexture {
        unsigned int id = 0;   // GLuint；不透過 GL 標頭保持此檔純 C++
        int w = 0, h = 0;
        bool failed = false;   // 解碼/上傳失敗——避免每帧重試
    };
    std::vector<CardEntry> cardEntries;
    bool cardListScanned = false;
    std::unordered_map<std::string, CardTexture> cardTextures;
    std::future<int> cardBakeFuture;   // 背景「重新產生立繪」任務
    int cardBakeTarget = -1;
    void ScanCardGallery();
    std::string ResolveCardArtPath(const CardEntry& card) const;
    const CardTexture* EnsureCardTexture(const std::string& path);
    void InvalidateCardTexture(const std::string& path);
    void ReleaseCardTextures();
};

} // namespace MingGoRTSIDE