#!/usr/bin/env bash
# sync-potatoengine-upstream.sh — 把 MingGoRTS 內的引擎模組同步到上游
# PotatoEngine repo(預設 C:/HWC/PotatoEngine)。
#
# 拆倉後上游是獨立 git repo(filter-repo 產物),扁平模組目錄佈局——
# 舊的 include/<Mod>/ + src/<Mod>/ 鏡像佈局已廢除,本腳本不再產生它。
# 遊戲側正式切出(add_subdirectory 消費)前,repo 內引擎仍是真相來源,
# 本腳本負責單向傳播 MingGoRTS → PotatoEngine。
#
# 引擎 CMake 用 file(GLOB_RECURSE) 收源檔——新檔案複製過去即自動入建置,
# 不用改上游 CMakeLists;刪除靠目錄級鏡像(rm -rf + cp)反映,上游 repo
# 的 git history 即稽核軌跡,不再維護 legacy/。
#
# 用法:
#   tools/sync-potatoengine-upstream.sh [target_dir]
# 在 repo 根目錄以 Git Bash 執行。

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
TARGET="${1:-C:/HWC/PotatoEngine}"

# AI/ 是混合目錄:只有 NeuralNetwork 歸引擎,其餘 AI*(Agent/RAG/RL 等)
# 全歸遊戲層——AI 不在 MODULES 內,改用檔案級白名單同步。
MODULES="Audio Core ECS Events FileSystem GameObject Input Logging \
MathUtils Media Memory Physics Platform Quantum Rendering Resources Scene \
Security Serialization Time"

# 引擎側 AI 檔(檔案級;MingGoRTS/AI/ 內其餘檔案皆遊戲側,不複製)
AI_FILES="NeuralNetwork.h NeuralNetwork.cpp"

# 引擎側 Examples 白名單(不含副檔名;遊戲側範例留在 MingGoRTS)
EXAMPLES="AIDemo AITestSuite AngelLegionVideo AudioTest CoreSystemsTest \
EngineExample FakeCheatModule FileSystemTest GLSmokeTest ImageCodecTest \
InputTest LogTest LoggerTest MathTest MediaPipeTest MemoryTest MinimalTest \
MiniaudioBackendTest ModelLoaderTest NeuralArtTool NeuralGraphicsTest \
OptimizedAIDemo \
PhysicsMathTest PhysicsTest PlatformTest PortraitBaker PortraitRenderer \
QuasiModelsTest RenderPipelineTest RenderTargetTest SceneTest \
SecurityRedTeamTest SerializationTest SimpleExample SpriteAtlasTest \
SynthDataDemo TimeTest UnsolvedBatch3Test VrmLoadTest compat_log"

echo "Source : $REPO_ROOT"
echo "Target : $TARGET"

# 安全閘:目標若仍是舊鏡像佈局(include/+src/),拒絕執行——
# 繼續跑會把扁平 repo 與鏡像檔混在一起
if [ -d "$TARGET/include" ] || [ -d "$TARGET/src" ]; then
    echo "ERROR: $TARGET 仍是舊 include/+src/ 鏡像佈局,拒絕同步。" >&2
    echo "       拆倉後上游應為扁平模組 repo;請確認 target 指向正確。" >&2
    exit 1
fi
[ -f "$TARGET/CMakeLists.txt" ] || { echo "ERROR: $TARGET 不像 PotatoEngine repo(缺 CMakeLists.txt)"; exit 1; }

# 1) 引擎模組:目錄級鏡像(source 無此模組 → target 整目錄移除)
total=0
for m in $MODULES; do
    if [ -d "$REPO_ROOT/$m" ]; then
        rm -rf "$TARGET/$m"
        cp -r "$REPO_ROOT/$m" "$TARGET/$m"
        n=$(find "$TARGET/$m" -type f | wc -l)
        total=$((total+n))
    else
        rm -rf "$TARGET/$m"
        echo "  [removed] $m(上游已刪模組)"
    fi
done

# 1b) AI/ 檔案級同步(混合目錄特例)
mkdir -p "$TARGET/AI"
for f in $AI_FILES; do
    if [ -f "$REPO_ROOT/AI/$f" ]; then
        cp "$REPO_ROOT/AI/$f" "$TARGET/AI/$f"
        total=$((total+1))
    elif [ -f "$TARGET/AI/$f" ]; then
        rm "$TARGET/AI/$f"
        echo "  [removed] AI/$f"
    fi
done

# 2) vendored 依賴:同步 glad_gen + tinygltf + miniaudio(F-4);
#    external/imgui 是上游 submodule,絕不觸碰
for dep in glad_gen tinygltf miniaudio; do
    if [ -d "$REPO_ROOT/external/$dep" ]; then
        rm -rf "$TARGET/external/$dep"
        mkdir -p "$TARGET/external"
        cp -r "$REPO_ROOT/external/$dep" "$TARGET/external/$dep"
    fi
done

# 3) 引擎側範例/測試:白名單 .cpp/.h 複製;白名單內但 source 已刪 → target 移除
copied=0; removed=0
for e in $EXAMPLES; do
    for ext in cpp h; do
        src="$REPO_ROOT/Examples/$e.$ext"
        dst="$TARGET/Examples/$e.$ext"
        if [ -f "$src" ]; then
            cp "$src" "$dst"
            copied=$((copied+1))
        elif [ -f "$dst" ]; then
            rm "$dst"
            removed=$((removed+1))
            echo "  [removed] Examples/$e.$ext"
        fi
    done
done

echo "Done: $total module files mirrored, $copied example files synced, $removed stale removed"
echo "Note: 上游 git status 若顯示漂移屬預期——請在上游 repo 提交同步結果。"
