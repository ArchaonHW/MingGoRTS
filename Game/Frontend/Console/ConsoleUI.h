#pragma once

#include "../../Interface/Events/EventSystem.h"
#include "../../Interface/API/GameAPI.h"
#include "../../Interface/Protocol/Command.h"
#include <string>
#include <vector>
#include <functional>

namespace MingGoRTS {

// 菜单类型
enum class MenuType {
    MainMenu,
    CampaignMenu,
    BattleMenu,
    DiplomacyMenu,
    SettlementMenu,
    PauseMenu,
    HelpMenu
};

// UI状态
struct UIState {
    bool showMinimap;
    bool showUnitInfo;
    bool showFactionPanel;
    bool showDiplomacyPanel;
    bool showSettlementPanel;
    bool showBattleHud;
    bool isPaused;
    MenuType currentMenu;
};

// 控制台UI - 基于文本的前端实现
class ConsoleUI : public IEventListener {
public:
    ConsoleUI(GameAPI* api, EventSystem* eventSystem);
    ~ConsoleUI();
    
    // 初始化
    bool Initialize();
    void Shutdown();
    
    // 主循环
    void Run();
    void Stop();
    
    // 更新
    void Update(float deltaTime);
    void Render();
    
    // 输入处理
    void ProcessInput();
    void HandleCommand(const Command& command);
    
    // 菜单
    void OpenMenu(MenuType menu);
    void CloseMenu();
    void ShowHelp();
    
    // 显示面板
    void ShowMinimap(bool show);
    void ShowUnitInfo(bool show);
    void ShowFactionPanel(bool show);
    void ShowDiplomacyPanel(bool show);
    void ShowSettlementPanel(bool show);
    
    // 事件监听
    void OnEvent(const Event& event) override;
    bool ShouldReceiveEvent(EventType type) const override;
    
    // 游戏状态切换
    void EnterCampaign();
    void EnterBattle();
    void ExitToMainMenu();
    
    // 获取状态
    bool IsRunning() const { return isRunning_; }
    const UIState& GetState() const { return state_; }

private:
    GameAPI* api_;
    EventSystem* eventSystem_;
    bool isRunning_;
    UIState state_;
    
    // 输入缓冲
    std::string inputBuffer_;
    std::vector<std::string> commandHistory_;
    int historyIndex_;
    
    // 渲染辅助
    void ClearScreen();
    void DrawHeader();
    void DrawCampaignView();
    void DrawBattleView();
    void DrawMainMenu();
    void DrawMinimap();
    void DrawUnitInfo();
    void DrawFactionPanel();
    void DrawDiplomacyPanel();
    void DrawSettlementPanel();
    void DrawHelp();
    void DrawPrompt();
    
    // 命令解析
    Command ParseInput(const std::string& input);
    void ExecuteCommand(const Command& command);
    
    // 显示辅助
    void PrintLine(const std::string& text);
    void PrintSeparator();
    void PrintError(const std::string& message);
    void PrintSuccess(const std::string& message);
    void PrintInfo(const std::string& message);
    
    // 事件处理
    void OnGameStateChanged(const Event& event);
    void OnTurnChanged(const Event& event);
    void OnBattleStarted(const Event& event);
    void OnBattleEnded(const Event& event);
    void OnSettlementCaptured(const Event& event);
    void OnMoneyChanged(const Event& event);
};

} // namespace MingGoRTS
