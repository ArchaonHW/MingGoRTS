cmake_minimum_required(VERSION 3.16)
project(MingGoRTSGame VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源文件分组
set(CORE_SOURCES
    Core/Engine/GameEngine.cpp
    Core/Engine/GameState.cpp
    Core/Campaign/CampaignLayer.cpp
    Core/Campaign/FactionManager.cpp
    Core/Campaign/Province.cpp
    Core/Campaign/Settlement.cpp
    Core/Battle/BattleLayer.cpp
    Core/Battle/BattleField.cpp
    Core/Battle/Formation.cpp
    Core/Faction/Faction.cpp
    Core/Faction/Culture.cpp
    Core/Unit/Unit.cpp
    Core/Unit/UnitType.cpp
    Core/Unit/Army.cpp
    Core/Politics/PoliticsSystem.cpp
    Core/Politics/Character.cpp
    Core/Politics/Party.cpp
    Core/Economy/EconomySystem.cpp
    Core/Economy/TradeRoute.cpp
    Core/Tech/TechTree.cpp
    Core/Tech/Technology.cpp
)

set(INTERFACE_SOURCES
    Interface/API/GameAPI.cpp
    Interface/Events/EventSystem.cpp
    Interface/Events/Event.cpp
    Interface/Protocol/Command.cpp
    Interface/Protocol/Response.cpp
)

set(FRONTEND_SOURCES
    Frontend/Console/ConsoleUI.cpp
    Frontend/Console/ConsoleRenderer.cpp
)

set(MAIN_SOURCE
    Main.cpp
)

# 创建库
add_library(GameCore STATIC ${CORE_SOURCES})
add_library(GameInterface STATIC ${INTERFACE_SOURCES})
add_library(GameFrontend STATIC ${FRONTEND_SOURCES})

# 主可执行文件
add_executable(MingGoRTSGame ${MAIN_SOURCE})

# 链接依赖
target_link_libraries(GameInterface GameCore)
target_link_libraries(GameFrontend GameInterface)
target_link_libraries(MingGoRTSGame GameCore GameInterface GameFrontend)

# 包含目录
target_include_directories(GameCore PUBLIC Core)
target_include_directories(GameInterface PUBLIC Interface)
target_include_directories(GameFrontend PUBLIC Frontend)

# 平台检测
if(WIN32)
    target_compile_definitions(MingGoRTSGame PRIVATE PLATFORM_WINDOWS)
elseif(UNIX)
    target_compile_definitions(MingGoRTSGame PRIVATE PLATFORM_LINUX)
endif()

# 编译选项
if(MSVC)
    target_compile_options(MingGoRTSGame PRIVATE /W4)
else()
    target_compile_options(MingGoRTSGame PRIVATE -Wall -Wextra -Wpedantic)
endif()
