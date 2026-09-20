#!/usr/bin/env python3
"""把 monorepo CMakeLists.txt 切成 engine/game 兩份。"""
import re, sys

SRC = r"C:\HWC\MingGoRTS\CMakeLists.txt"
ENGINE_OUT = r"C:\HWC\PotatoEngine-repo\CMakeLists.txt"
GAME_OUT = r"C:\HWC\MingGoRTS-game\CMakeLists.txt"

ENGINE_TARGETS = {
    "NeuralNetwork", "Quantum", "Media",
    "MinimalTest", "EngineExample", "SimpleExample",
    "AIDemo", "OptimizedAIDemo", "SimpleAIDemo", "AITestSuite",
    "MathTest", "PhysicsTest", "PhysicsMathTest",
    "SecurityRedTeamTest", "FakeCheatModule",
    "FileSystemTest", "LoggerTest", "MemoryTest", "PlatformTest",
    "SceneTest", "SerializationTest", "RenderPipelineTest", "MediaPipeTest",
    "RenderTargetTest", "AngelLegionVideo",
    "ModelLoaderTest", "VrmLoadTest", "InputTest", "AudioTest", "TimeTest",
    "ImageCodecTest", "CoreSystemsTest",
    "NeuralGraphicsTest", "NeuralArtTool", "GLSmokeTest",
    "PortraitRenderer", "SynthDataDemo", "PortraitBaker",
    "LogTest", "SpriteAtlasTest",
}
ENGINE_LIBS = {"NeuralNetwork", "Quantum", "Media"}  # 守衛外的 lib
ENGINE_EXES = ENGINE_TARGETS - ENGINE_LIBS - {"PotatoEngine"}

GAME_EXES = {
    "DoctrineBattleDemo", "AutoPlannerDemo", "DuanqiaoDemo", "SageCommandTest",
    "SageCommandDemo", "QuantumTest", "QuasiRandomTest", "BattlePickerTest",
    "PlanningDeckTest", "DuanqiaoPlayable", "QuantumFogBattleTest",
    "QuantumBench", "DuanqiaoTutorial", "TutorialScriptTest", "QuantumFogDemo",
    "AIAgentSmoke", "MingGoRTS_IDE_CLI", "MingGoRTS_IDE_GUI",
    "BattleSceneTest", "SquadFatigueTest", "TerrainBonusTest", "PostBattleTest",
    "MapGenTest", "SquadTemplateTest", "GeneralGuardTest", "BattlePlanTest",
    "RefitCampTest", "UIThemeTest", "HistorianReportTest", "RivalDeckTest",
    "CampaignLedgerTest", "GeneralDossierTest", "MythLogTest",
    "CampaignStateTest", "BattleRenderDemo", "BattleTrainerDemo",
    "VideoDataDemo", "AIEnhancedDemo", "UltimateAIAgentDemo",
    "IntelligentDevelopmentDemo", "CognitiveDemo", "DevAssistantSmoke",
    "SuggestionRulesTest", "AIAgentGUIExample",
    "ChapterLibraryTest", "LogTest_dup_no",
    "QuantumPlanTest", "LedgerChainTest", "LedgerAuditTest",
    "ChapterConventionsTest", "ReplayIntegrityTest", "GovernanceTest",
    "SpriteAtlasTest_dup_no",
}
GAME_LIBS = {
    "ReinforcementLearning", "NaturalLanguageProcessing", "AIAgentSystem",
    "LLMIntegration", "RAGSystem", "AgentChain", "ToolFramework",
    "KnowledgeGraph", "SelfReflection", "IntelligentDevelopmentSystem",
    "IntelligentSuggestion", "Gameplay", "Campaign",
}

ENGINE_TESTS = [
    "MinimalTest", "MathTest", "PhysicsTest", "PhysicsMathTest",
    "FileSystemTest", "LoggerTest", "MemoryTest", "PlatformTest",
    "SceneTest", "SerializationTest", "RenderPipelineTest",
    "ModelLoaderTest", "VrmLoadTest", "InputTest", "AudioTest", "TimeTest",
    "ImageCodecTest", "CoreSystemsTest", "NeuralGraphicsTest", "GLSmokeTest",
    "SecurityRedTeamTest", "AITestSuite", "MediaPipeTest", "RenderTargetTest",
    "LogTest", "SpriteAtlasTest", "SynthDataDemo",
]
GAME_TESTS = [
    "BattleSceneTest", "SquadFatigueTest", "DevAssistantSmoke",
    "SuggestionRulesTest", "AIAgentSmoke", "DoctrineBattleDemo",
    "AutoPlannerDemo", "BattleTrainerDemo", "DuanqiaoDemo", "SageCommandTest",
    "QuantumTest", "QuasiRandomTest", "QuantumFogBattleTest", "QuantumBench",
    "TutorialScriptTest", "BattlePickerTest", "PlanningDeckTest",
    "TerrainBonusTest", "PostBattleTest", "MapGenTest", "SquadTemplateTest",
    "GeneralGuardTest", "RefitCampTest", "BattlePlanTest", "QuantumPlanTest",
    "UIThemeTest", "HistorianReportTest", "RivalDeckTest", "CampaignLedgerTest",
    "GeneralDossierTest", "CampaignStateTest", "ChapterLibraryTest",
    "MythLogTest", "ChapterConventionsTest", "LedgerChainTest",
    "LedgerAuditTest", "ReplayIntegrityTest", "GovernanceTest",
]

lines = open(SRC, encoding="utf-8").read().splitlines()

# --- 找邊界 ---
add_re = re.compile(r'^\s*#?\s*add_(executable|library)\s*\(\s*([A-Za-z0-9_]+)')
first_add = next(i for i, l in enumerate(lines) if add_re.match(l))
install_marker = next(i for i, l in enumerate(lines) if "安裝 (暫時禁用)" in l)
et = next(i for i, l in enumerate(lines) if "enable_testing()" in l)
test_marker = et - 3  # "# ====..." + "# CTest - ..." + "# ====..."
cpack_marker = next(i for i, l in enumerate(lines) if "CPack 打包配置" in l)

header = lines[:first_add]
mid = lines[first_add:install_marker]
install_block = lines[install_marker:test_marker]
tail = lines[cpack_marker:]

# --- mid 切 chunk ---
chunks = []  # (target_name, [lines])
pending = []
for l in mid:
    m = add_re.match(l)
    if m:
        if pending:
            chunks.append(pending)
        pending = [l]
    else:
        if pending:
            pending.append(l)
        else:
            # 前置註解（第一個 add_* 前的註解行）歸下一 chunk——先存著
            chunks.append(["__preamble__", l])
if pending:
    chunks.append(pending)

def chunk_name(c):
    for l in c:
        m = add_re.match(l)
        if m:
            return m.group(2)
    return "__preamble__"

named = [(chunk_name(c), c) for c in chunks]

def emit(name):
    w = open(name, "w", encoding="utf-8", newline="\n")
    return w

def fix_source_dir(txt):
    return txt.replace("CMAKE_SOURCE_DIR", "CMAKE_CURRENT_SOURCE_DIR")

# ================= 引擎檔 =================
w = emit(ENGINE_OUT)
for l in header:
    w.write(fix_source_dir(l) + "\n")
w.write("\n# 引擎附屬函式庫（消費端可連結）\n")

# 引擎 lib chunk（NeuralNetwork/Quantum/Media）放守衛外
engine_lib_chunks = [c for n, c in named if n in ENGINE_LIBS]
for c in engine_lib_chunks:
    for l in c:
        w.write(fix_source_dir(l) + "\n")
    w.write("\n")

w.write("""
# ============================================================================
# Standalone 守衛：examples/tests/install 只在引擎 repo 獨立建置時註冊；
# 被遊戲 repo 以 add_subdirectory 消費時只產出函式庫 target。
# ============================================================================
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
""")
for n, c in named:
    if n in ENGINE_EXES or n == "__preamble__":
        if n == "__preamble__":
            continue  # 前置註解含 SimpleExample 註解,跳過避免重複
        for l in c:
            w.write(fix_source_dir(l) + "\n")
        w.write("\n")

for l in install_block:
    w.write(fix_source_dir(l) + "\n")

w.write("""
# ============================================================================
# CTest - 非互動測試可用 ctest 一鍵執行
# ============================================================================
enable_testing()

set(POTATO_TESTS
""")
for t in ENGINE_TESTS:
    w.write(f"    {t}\n")
w.write(""")
foreach(test_name IN LISTS POTATO_TESTS)
    if(TARGET ${test_name})
        add_test(NAME ${test_name} COMMAND ${test_name})
        set_tests_properties(${test_name} PROPERTIES
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endif()
endforeach()

""")
for l in tail:
    w.write(fix_source_dir(l) + "\n")
w.write("endif() # STANDALONE\n")
w.close()

# ================= 遊戲檔 =================
w = emit(GAME_OUT)
w.write("""# MingGoRTS - CMake 建置配置
# 遊戲層：以 add_subdirectory 消費 PotatoEngine 上游引擎 repo
#
# 用法：
#   git clone <PotatoEngine> ../PotatoEngine   # 或任意路徑
#   cmake -S . -B build -DPOTATO_ENGINE_ROOT=<engine path>
#   cmake --build build && cd build && ctest -C Release

cmake_minimum_required(VERSION 3.15)
project(MingGoRTS VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ============================================================================
# 引擎消費點：POTATO_ENGINE_ROOT 指向上游 PotatoEngine checkout
# ============================================================================
set(POTATO_ENGINE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../PotatoEngine"
    CACHE PATH "Path to the upstream PotatoEngine repository checkout")
if(NOT EXISTS "${POTATO_ENGINE_ROOT}/CMakeLists.txt")
    message(FATAL_ERROR
        "POTATO_ENGINE_ROOT 無效: ${POTATO_ENGINE_ROOT}\\n"
        "請 clone PotatoEngine 至該路徑,或以 "
        "-DPOTATO_ENGINE_ROOT=<path> 指定。")
endif()
add_subdirectory(${POTATO_ENGINE_ROOT} ${CMAKE_BINARY_DIR}/PotatoEngine)

# 編譯選項（與引擎一致）
if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

option(POTATO_SECURITY_HARDENING "Enable security hardening flags" ON)
if(POTATO_SECURITY_HARDENING)
    if(MSVC)
        add_compile_options(/sdl /GS /guard:cf)
        add_link_options(/DYNAMICBASE /NXCOMPAT /HIGHENTROPYVA /GUARD:CF /CETCOMPAT)
    else()
        add_compile_options(-fstack-protector-strong -fstack-clash-protection -fPIE)
        if(WIN32)
            add_link_options(
                $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:-pie>
                -Wl,--dynamicbase -Wl,--nxcompat -Wl,--high-entropy-va)
        else()
            add_compile_definitions(_FORTIFY_SOURCE=2)
            add_link_options(
                $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:-pie>
                -Wl,-z,relro,-z,now -Wl,-z,noexecstack)
        endif()
    endif()
endif()

if(MINGW)
    add_link_options(-static-libgcc -static-libstdc++ -static)
endif()

find_package(OpenGL REQUIRED)

# 遊戲側 include：根目錄讓 "Gameplay/..." "Campaign/..." "AI/..." 可用；
# 引擎標頭經 PotatoEngine PUBLIC include dirs 傳遞
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Gameplay
    ${CMAKE_CURRENT_SOURCE_DIR}/Campaign
    ${CMAKE_CURRENT_SOURCE_DIR}/AI
    ${CMAKE_CURRENT_SOURCE_DIR}/MingGoRTS_IDE
    ${OPENGL_INCLUDE_DIRS}
)

""")

def fix_game(l):
    l = fix_source_dir(l)
    # external/* 引用改指引擎 checkout
    l = l.replace("${CMAKE_CURRENT_SOURCE_DIR}/external",
                  "${POTATO_ENGINE_ROOT}/external")
    l = l.replace("external/imgui/", "${POTATO_ENGINE_ROOT}/external/imgui/")
    l = l.replace("external/glad_gen/", "${POTATO_ENGINE_ROOT}/external/glad_gen/")
    # IDE_GUI 直編引擎源檔 → 引擎路徑
    l = l.replace("    Rendering/ImageCodec.cpp",
                  "    ${POTATO_ENGINE_ROOT}/Rendering/ImageCodec.cpp")
    l = l.replace("    Logging/Logger.cpp",
                  "    ${POTATO_ENGINE_ROOT}/Logging/Logger.cpp")
    return l

for n, c in named:
    if n in GAME_LIBS or n in GAME_EXES:
        for l in c:
            w.write(fix_game(l) + "\n")
        w.write("\n")

w.write("""
# ============================================================================
# CTest - 非互動測試可用 ctest 一鍵執行
# ============================================================================
enable_testing()

set(POTATO_TESTS
""")
for t in GAME_TESTS:
    w.write(f"    {t}\n")
w.write(""" )
foreach(test_name IN LISTS POTATO_TESTS)
    if(TARGET ${test_name})
        add_test(NAME ${test_name} COMMAND ${test_name})
        set_tests_properties(${test_name} PROPERTIES
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endif()
endforeach()

# CPack 打包配置
set(CPACK_PACKAGE_NAME "MingGoRTS")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MingGoRTS - Potato Engine 策略遊戲")
set(CPACK_PACKAGE_VENDOR "Potato Engine Team")
set(CPACK_GENERATOR "ZIP;TGZ")

include(CPack)
""")
w.close()
print("done")
