cmake_minimum_required(VERSION 3.10)
project(RequirementSystemLauncher)

# C++ 標準
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源代碼文件
set(SOURCES
    RequirementSystemLauncher.cpp
)

# 創建可執行文件
add_executable(RequirementSystem ${SOURCES})

# Windows 特定設置
if(WIN32)
    target_link_libraries(RequirementSystem shell32)
    set_target_properties(RequirementSystem PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()

# 安裝目標
install(TARGETS RequirementSystem
    RUNTIME DESTINATION bin
)

# 打印配置信息
message(STATUS "Requirement System Launcher Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Output: RequirementSystem.exe")
