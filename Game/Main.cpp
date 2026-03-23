#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#include "Core/Engine/GameEngine.h"
#include "Interface/API/GameAPI.h"
#include "Interface/Events/EventSystem.h"
#include "Frontend/Console/ConsoleUI.h"

using namespace MingGoRTS;

int main(int argc, char* argv[]) {
    std::cout << "==================================" << std::endl;
    std::cout << "  MingGoRTS - Total War Clone" << std::endl;
    std::cout << "  Backend-Frontend Separation Demo" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << std::endl;
    
    // 初始化游戏配置
    GameConfig config;
    config.maxTurns = 500;
    config.campaignMapWidth = 100;
    config.campaignMapHeight = 100;
    config.battleMapWidth = 100;
    config.battleMapHeight = 100;
    config.battleSpeed = 1.0f;
    config.autoSave = true;
    config.savePath = "Saves/";
    
    // 创建游戏引擎（Backend）
    std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
    
    std::cout << "[Main] Initializing Game Engine..." << std::endl;
    if (!engine->Initialize(config)) {
        std::cerr << "[Main] Failed to initialize game engine!" << std::endl;
        return 1;
    }
    
    // 创建API层（Interface）
    std::unique_ptr<GameAPI> api = std::make_unique<GameAPI>(engine.get());
    api->Initialize();
    
    // 获取事件系统
    EventSystem* eventSystem = engine->GetEventSystem();
    
    // 设置玩家派系
    engine->GetFactionManager()->SetPlayerFaction(1);  // Rome
    
    // 初始化战役层数据
    CampaignLayer* campaign = engine->GetCampaignLayer();
    if (campaign) {
        // 创建定居点
        campaign->CreateSettlement(Vector2D(50, 50), 1, "Rome");
        campaign->CreateSettlement(Vector2D(20, 30), 2, "Barbarian Camp");
        campaign->CreateSettlement(Vector2D(80, 70), 3, "Eastern City");
        
        // 创建军队
        campaign->CreateArmy(1, Vector2D(52, 50));  // 罗马军队
        campaign->CreateArmy(2, Vector2D(22, 32));  // 蛮族军队
        campaign->CreateArmy(3, Vector2D(82, 72));  // 东方军队
        
        // 创建行省
        int province1 = campaign->CreateProvince("Italy");
        int province2 = campaign->CreateProvince("Gaul");
        int province3 = campaign->CreateProvince("Persia");
        
        // 分配定居点到行省
        campaign->AssignSettlementToProvince(1, province1);
        campaign->AssignSettlementToProvince(2, province2);
        campaign->AssignSettlementToProvince(3, province3);
    }
    
    std::cout << "[Main] Game initialized successfully!" << std::endl;
    std::cout << std::endl;
    
    // 创建控制台UI（Frontend）
    std::unique_ptr<ConsoleUI> ui = std::make_unique<ConsoleUI>(api.get(), eventSystem);
    
    std::cout << "[Main] Initializing Console UI..." << std::endl;
    if (!ui->Initialize()) {
        std::cerr << "[Main] Failed to initialize console UI!" << std::endl;
        return 1;
    }
    
    // 进入战役模式
    engine->EnterCampaign();
    ui->EnterCampaign();
    
    std::cout << "[Main] Starting game loop..." << std::endl;
    std::cout << std::endl;
    
    // 游戏主循环
    bool running = true;
    auto lastTime = std::chrono::high_resolution_clock::now();
    
    const float targetFPS = 30.0f;
    const float frameTime = 1.0f / targetFPS;
    
    // 在单独的线程中运行UI（用于非阻塞输入）
    // 这里简化处理，直接在主线程运行
    
    std::cout << "Press Enter to start the game..." << std::endl;
    std::cin.get();
    
    // 运行控制台UI主循环
    ui->Run();
    
    /*
    // 如果需要实时更新的游戏循环，可以使用以下代码：
    while (running && ui->IsRunning()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        // 更新后端
        engine->Update(deltaTime);
        
        // 如果是战斗模式，使用固定时间步更新
        if (engine->GetState() == GameState::Battle) {
            engine->FixedUpdate(1.0f / 60.0f);  // 60 FPS固定步长
        }
        
        // 更新事件系统
        if (eventSystem) {
            eventSystem->Update(deltaTime);
        }
        
        // 更新前端
        ui->Update(deltaTime);
        ui->Render();
        
        // 控制帧率
        float sleepTime = frameTime - deltaTime;
        if (sleepTime > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTime * 1000)));
        }
    }
    */
    
    // 清理
    std::cout << std::endl;
    std::cout << "[Main] Shutting down..." << std::endl;
    
    ui->Shutdown();
    api->Shutdown();
    engine->Shutdown();
    
    std::cout << "[Main] Game ended. Goodbye!" << std::endl;
    
    return 0;
}
