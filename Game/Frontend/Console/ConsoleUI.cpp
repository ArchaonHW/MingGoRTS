#include "ConsoleUI.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cstdlib>

namespace MingGoRTS {

ConsoleUI::ConsoleUI(GameAPI* api, EventSystem* eventSystem)
    : api_(api)
    , eventSystem_(eventSystem)
    , isRunning_(false)
    , historyIndex_(-1) {
    
    state_.showMinimap = true;
    state_.showUnitInfo = true;
    state_.showFactionPanel = true;
    state_.showDiplomacyPanel = false;
    state_.showSettlementPanel = false;
    state_.showBattleHud = false;
    state_.isPaused = false;
    state_.currentMenu = MenuType::MainMenu;
}

ConsoleUI::~ConsoleUI() {
    Shutdown();
}

bool ConsoleUI::Initialize() {
    // 订阅事件
    if (eventSystem_) {
        eventSystem_->Subscribe(this);
    }
    
    isRunning_ = true;
    
    ClearScreen();
    PrintLine("==================================");
    PrintLine("  MingGoRTS - Total War Clone");
    PrintLine("==================================");
    PrintLine("");
    PrintLine("Type 'help' for available commands");
    PrintLine("");
    
    return true;
}

void ConsoleUI::Shutdown() {
    if (eventSystem_) {
        eventSystem_->Unsubscribe(this);
    }
    isRunning_ = false;
}

void ConsoleUI::Run() {
    while (isRunning_) {
        Render();
        ProcessInput();
    }
}

void ConsoleUI::Stop() {
    isRunning_ = false;
}

void ConsoleUI::Update(float deltaTime) {
    // 更新逻辑
}

void ConsoleUI::Render() {
    switch (state_.currentMenu) {
        case MenuType::MainMenu:
            DrawMainMenu();
            break;
        case MenuType::CampaignMenu:
            DrawCampaignView();
            break;
        case MenuType::BattleMenu:
            DrawBattleView();
            break;
        case MenuType::HelpMenu:
            DrawHelp();
            break;
        default:
            break;
    }
    
    DrawPrompt();
}

void ConsoleUI::ProcessInput() {
    std::cout << "> ";
    std::getline(std::cin, inputBuffer_);
    
    if (!inputBuffer_.empty()) {
        commandHistory_.push_back(inputBuffer_);
        historyIndex_ = commandHistory_.size();
        
        Command cmd = ParseInput(inputBuffer_);
        ExecuteCommand(cmd);
    }
}

Command ConsoleUI::ParseInput(const std::string& input) {
    Command cmd;
    
    std::istringstream iss(input);
    std::string commandStr;
    iss >> commandStr;
    
    // 转换为大写进行比较
    std::string upperCmd = commandStr;
    std::transform(upperCmd.begin(), upperCmd.end(), upperCmd.begin(), ::toupper);
    
    // 命令映射
    if (upperCmd == "START" || upperCmd == "NEWGAME") {
        cmd.SetType(CommandType::StartGame);
    } else if (upperCmd == "LOAD") {
        cmd.SetType(CommandType::LoadGame);
    } else if (upperCmd == "SAVE") {
        cmd.SetType(CommandType::SaveGame);
    } else if (upperCmd == "EXIT" || upperCmd == "QUIT") {
        cmd.SetType(CommandType::ExitGame);
    } else if (upperCmd == "MOVE" || upperCmd == "M") {
        cmd.SetType(CommandType::MoveArmy);
        int x, y;
        if (iss >> x >> y) {
            cmd.AddParameter("x", x);
            cmd.AddParameter("y", y);
        }
    } else if (upperCmd == "ATTACK" || upperCmd == "A") {
        cmd.SetType(CommandType::AttackTarget);
        int target;
        if (iss >> target) {
            cmd.AddParameter("target", target);
        }
    } else if (upperCmd == "ENDTURN" || upperCmd == "END") {
        cmd.SetType(CommandType::EndTurn);
    } else if (upperCmd == "NEXT" || upperCmd == "N") {
        cmd.SetType(CommandType::NextPhase);
    } else if (upperCmd == "WAR" || upperCmd == "DECLAREWAR") {
        cmd.SetType(CommandType::DeclareWar);
        int faction;
        if (iss >> faction) {
            cmd.AddParameter("faction", faction);
        }
    } else if (upperCmd == "PEACE") {
        cmd.SetType(CommandType::ProposePeace);
        int faction;
        if (iss >> faction) {
            cmd.AddParameter("faction", faction);
        }
    } else if (upperCmd == "HELP" || upperCmd == "H" || upperCmd == "?") {
        cmd.SetType(CommandType::ShowHelp);
    } else if (upperCmd == "MENU") {
        cmd.SetType(CommandType::OpenMenu);
    } else if (upperCmd == "PAUSE") {
        cmd.SetType(CommandType::Pause);
    } else if (upperCmd == "RESUME") {
        cmd.SetType(CommandType::Resume);
    } else {
        cmd.SetType(CommandType::Unknown);
    }
    
    return cmd;
}

void ConsoleUI::ExecuteCommand(const Command& command) {
    switch (command.GetType()) {
        case CommandType::StartGame:
            PrintSuccess("Starting new game...");
            EnterCampaign();
            break;
            
        case CommandType::LoadGame:
            PrintInfo("Loading game...");
            break;
            
        case CommandType::SaveGame:
            PrintInfo("Saving game...");
            break;
            
        case CommandType::ExitGame:
            PrintInfo("Exiting game...");
            Stop();
            break;
            
        case CommandType::MoveArmy:
            if (api_) {
                int x = command.GetIntParameter("x");
                int y = command.GetIntParameter("y");
                PrintInfo("Moving to (" + std::to_string(x) + ", " + std::to_string(y) + ")");
            }
            break;
            
        case CommandType::AttackTarget:
            if (api_) {
                int target = command.GetIntParameter("target");
                PrintInfo("Attacking target " + std::to_string(target));
            }
            break;
            
        case CommandType::EndTurn:
            if (api_) {
                auto response = api_->EndTurn();
                if (response.success) {
                    PrintSuccess("Turn ended");
                } else {
                    PrintError(response.errorMessage);
                }
            }
            break;
            
        case CommandType::NextPhase:
            if (api_) {
                auto response = api_->NextPhase();
                if (response.success) {
                    PrintSuccess("Phase advanced");
                }
            }
            break;
            
        case CommandType::DeclareWar:
            if (api_) {
                int faction = command.GetIntParameter("faction");
                auto response = api_->DeclareWar(faction);
                if (response.success) {
                    PrintSuccess("War declared on faction " + std::to_string(faction));
                } else {
                    PrintError(response.errorMessage);
                }
            }
            break;
            
        case CommandType::ProposePeace:
            if (api_) {
                int faction = command.GetIntParameter("faction");
                PrintInfo("Proposing peace to faction " + std::to_string(faction));
            }
            break;
            
        case CommandType::Pause:
            state_.isPaused = true;
            PrintInfo("Game paused");
            break;
            
        case CommandType::Resume:
            state_.isPaused = false;
            PrintInfo("Game resumed");
            break;
            
        case CommandType::OpenMenu:
            OpenMenu(MenuType::MainMenu);
            break;
            
        case CommandType::ShowHelp:
            OpenMenu(MenuType::HelpMenu);
            break;
            
        case CommandType::Unknown:
        default:
            PrintError("Unknown command. Type 'help' for available commands.");
            break;
    }
}

void ConsoleUI::OpenMenu(MenuType menu) {
    state_.currentMenu = menu;
}

void ConsoleUI::CloseMenu() {
    state_.currentMenu = MenuType::MainMenu;
}

void ConsoleUI::ShowHelp() {
    OpenMenu(MenuType::HelpMenu);
}

void ConsoleUI::EnterCampaign() {
    state_.currentMenu = MenuType::CampaignMenu;
    PrintSuccess("Entered Campaign Mode");
}

void ConsoleUI::EnterBattle() {
    state_.currentMenu = MenuType::BattleMenu;
    state_.showBattleHud = true;
    PrintSuccess("Entered Battle Mode");
}

void ConsoleUI::ExitToMainMenu() {
    state_.currentMenu = MenuType::MainMenu;
    state_.showBattleHud = false;
}

void ConsoleUI::ClearScreen() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void ConsoleUI::DrawHeader() {
    PrintLine("==================================");
    PrintLine("  MingGoRTS - Total War Clone");
    PrintLine("==================================");
}

void ConsoleUI::DrawMainMenu() {
    ClearScreen();
    DrawHeader();
    PrintLine("");
    PrintLine("Main Menu");
    PrintLine("---------");
    PrintLine("1. New Game (start)");
    PrintLine("2. Load Game (load)");
    PrintLine("3. Save Game (save)");
    PrintLine("4. Help (help)");
    PrintLine("5. Exit (exit)");
    PrintLine("");
}

void ConsoleUI::DrawCampaignView() {
    if (!api_) return;
    
    ClearScreen();
    DrawHeader();
    
    // 获取游戏状态
    auto stateResponse = api_->GetGameState();
    auto turnResponse = api_->GetCurrentTurn();
    auto moneyResponse = api_->GetMoney(1);  // 假设玩家派系ID为1
    
    PrintLine("");
    PrintLine("Campaign Mode - Turn " + turnResponse.data);
    PrintLine("Money: " + moneyResponse.data);
    PrintSeparator();
    
    // 获取地图数据（简化显示）
    auto mapResponse = api_->GetCampaignMap(0, 0, 20, 10);
    
    PrintLine("Map View (20x10):");
    PrintLine("");
    
    // 简化的地图显示
    for (int y = 0; y < 10; ++y) {
        std::string line;
        for (int x = 0; x < 20; ++x) {
            // 简化：随机显示地形
            int rand_val = (x + y * 7) % 5;
            switch (rand_val) {
                case 0: line += "."; break;  // 平原
                case 1: line += "^"; break;  // 山地
                case 2: line += "@"; break;  // 森林
                case 3: line += "~"; break;  // 水域
                case 4: line += "S"; break;  // 定居点
            }
        }
        PrintLine(line);
    }
    
    PrintLine("");
    PrintLine("Legend: . = Plains, ^ = Mountains, @ = Forest, ~ = Water, S = Settlement");
    PrintSeparator();
    
    if (state_.showMinimap) {
        DrawMinimap();
    }
    
    if (state_.showFactionPanel) {
        DrawFactionPanel();
    }
}

void ConsoleUI::DrawBattleView() {
    if (!api_) return;
    
    ClearScreen();
    DrawHeader();
    
    auto battleState = api_->GetBattleState();
    
    PrintLine("");
    PrintLine("BATTLE MODE");
    PrintSeparator();
    PrintLine("Battle State: " + battleState.data);
    PrintLine("");
    
    // 简化的战场显示
    PrintLine("Battlefield (simplified):");
    PrintLine("");
    
    for (int y = 0; y < 15; ++y) {
        std::string line;
        for (int x = 0; x < 30; ++x) {
            // 简化显示
            if (y == 7 && x == 5) {
                line += "A";  // 攻击方单位
            } else if (y == 7 && x == 25) {
                line += "D";  // 防守方单位
            } else if (y == 0 || y == 14 || x == 0 || x == 29) {
                line += "#";  // 边界
            } else {
                line += ".";  // 空地
            }
        }
        PrintLine(line);
    }
    
    PrintLine("");
    PrintLine("Legend: A = Attacker, D = Defender, # = Boundary, . = Clear");
    PrintSeparator();
    
    if (state_.showBattleHud) {
        PrintLine("Commands: move <x> <y> | attack <target> | retreat | pause");
    }
}

void ConsoleUI::DrawMinimap() {
    PrintLine("Minimap:");
    PrintLine("[....................]");
    PrintLine("[....A.......D.......]");
    PrintLine("[....................]");
    PrintLine("");
}

void ConsoleUI::DrawFactionPanel() {
    if (!api_) return;
    
    auto factions = api_->GetAllFactions();
    
    PrintLine("Factions:");
    PrintLine(factions.data);
    PrintLine("");
}

void ConsoleUI::DrawDiplomacyPanel() {
    PrintLine("Diplomacy Panel:");
    PrintLine("1. Rome - Allied");
    PrintLine("2. Barbarians - At War");
    PrintLine("3. Eastern Empire - Neutral");
    PrintLine("");
}

void ConsoleUI::DrawSettlementPanel() {
    PrintLine("Settlement Panel:");
    PrintLine("- Population: 5000");
    PrintLine("- Income: 500");
    PrintLine("- Public Order: 75%");
    PrintLine("");
}

void ConsoleUI::DrawHelp() {
    ClearScreen();
    DrawHeader();
    
    PrintLine("");
    PrintLine("Available Commands:");
    PrintLine("-------------------");
    PrintLine("");
    PrintLine("Game Control:");
    PrintLine("  start, newgame  - Start a new game");
    PrintLine("  load            - Load saved game");
    PrintLine("  save            - Save current game");
    PrintLine("  exit, quit      - Exit the game");
    PrintLine("  pause           - Pause game");
    PrintLine("  resume          - Resume game");
    PrintLine("");
    PrintLine("Campaign Commands:");
    PrintLine("  move <x> <y>    - Move selected army");
    PrintLine("  attack <id>     - Attack target army");
    PrintLine("  end, endturn    - End current turn");
    PrintLine("  next            - Advance to next phase");
    PrintLine("");
    PrintLine("Diplomacy Commands:");
    PrintLine("  war <faction>   - Declare war");
    PrintLine("  peace <faction> - Propose peace");
    PrintLine("");
    PrintLine("UI Commands:");
    PrintLine("  menu            - Open main menu");
    PrintLine("  help, ?         - Show this help");
    PrintLine("");
    PrintLine("Press Enter to return...");
}

void ConsoleUI::DrawPrompt() {
    // 在Render中由ProcessInput处理
}

void ConsoleUI::PrintLine(const std::string& text) {
    std::cout << text << std::endl;
}

void ConsoleUI::PrintSeparator() {
    PrintLine("----------------------------------");
}

void ConsoleUI::PrintError(const std::string& message) {
    std::cout << "[ERROR] " << message << std::endl;
}

void ConsoleUI::PrintSuccess(const std::string& message) {
    std::cout << "[OK] " << message << std::endl;
}

void ConsoleUI::PrintInfo(const std::string& message) {
    std::cout << "[INFO] " << message << std::endl;
}

void ConsoleUI::OnEvent(const Event& event) {
    switch (event.type) {
        case EventType::StateChanged:
            OnGameStateChanged(event);
            break;
        case EventType::TurnEnded:
            OnTurnChanged(event);
            break;
        case EventType::BattleStarted:
            OnBattleStarted(event);
            break;
        case EventType::BattleEnded:
            OnBattleEnded(event);
            break;
        case EventType::SettlementCaptured:
            OnSettlementCaptured(event);
            break;
        case EventType::MoneyChanged:
            OnMoneyChanged(event);
            break;
        default:
            break;
    }
}

bool ConsoleUI::ShouldReceiveEvent(EventType type) const {
    // 接收所有事件
    return true;
}

void ConsoleUI::OnGameStateChanged(const Event& event) {
    PrintInfo("Game state changed: " + event.data);
}

void ConsoleUI::OnTurnChanged(const Event& event) {
    PrintInfo("Turn " + event.data + " started");
}

void ConsoleUI::OnBattleStarted(const Event& event) {
    EnterBattle();
}

void ConsoleUI::OnBattleEnded(const Event& event) {
    PrintInfo("Battle ended: " + event.data);
    state_.showBattleHud = false;
    state_.currentMenu = MenuType::CampaignMenu;
}

void ConsoleUI::OnSettlementCaptured(const Event& event) {
    PrintSuccess("Settlement captured!");
}

void ConsoleUI::OnMoneyChanged(const Event& event) {
    // 不每次都打印，避免刷屏
}

void ConsoleUI::ShowMinimap(bool show) {
    state_.showMinimap = show;
}

void ConsoleUI::ShowUnitInfo(bool show) {
    state_.showUnitInfo = show;
}

void ConsoleUI::ShowFactionPanel(bool show) {
    state_.showFactionPanel = show;
}

void ConsoleUI::ShowDiplomacyPanel(bool show) {
    state_.showDiplomacyPanel = show;
}

void ConsoleUI::ShowSettlementPanel(bool show) {
    state_.showSettlementPanel = show;
}

} // namespace MingGoRTS
