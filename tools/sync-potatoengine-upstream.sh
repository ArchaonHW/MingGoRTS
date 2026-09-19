#!/usr/bin/env bash
# sync-potatoengine-upstream.sh — 把 MingGoRTS 內的 PotatoEngine 引擎模組
# 同步到上游參考副本（預設 C:/HWC/PotatoEngine）。
#
# 上游是唯讀鏡像：include/<Module>/ 放標頭、src/<Module>/ 放實作；
# 上游獨有而 repo 已刪的檔案會被移到 upstream 根的 legacy/（不清除）。
# 用法：
#   tools/sync-potatoengine-upstream.sh [target_dir]
# 在 repo 根目錄以 Git Bash 執行。

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
TARGET="${1:-C:/HWC/PotatoEngine}"

MODULES="Core Security Events FileSystem Logging MathUtils Memory Platform \
Scene Serialization Time Rendering Physics Audio Input Resources ECS \
GameObject GUI AI Quantum"

echo "Source : $REPO_ROOT"
echo "Target : $TARGET"
mkdir -p "$TARGET/include" "$TARGET/src"

# 1) 複製引擎模組（.h/.hpp → include/，.cpp/.c → src/），保留子路徑
total_h=0; total_c=0
for m in $MODULES; do
    [ -d "$REPO_ROOT/$m" ] || { echo "  [skip] $m 不存在"; continue; }
    while IFS= read -r f; do
        rel="${f#$REPO_ROOT/$m/}"
        mkdir -p "$TARGET/include/$m/$(dirname "$rel")"
        cp "$f" "$TARGET/include/$m/$rel"
        total_h=$((total_h+1))
    done < <(find "$REPO_ROOT/$m" -name '*.h' -o -name '*.hpp')
    while IFS= read -r f; do
        rel="${f#$REPO_ROOT/$m/}"
        mkdir -p "$TARGET/src/$m/$(dirname "$rel")"
        cp "$f" "$TARGET/src/$m/$rel"
        total_c=$((total_c+1))
    done < <(find "$REPO_ROOT/$m" -name '*.cpp' -o -name '*.c')
done

# 2) vendored 依賴：引擎建置只需要 glad_gen + tinygltf
#（imgui 為遊戲層工具用，不進上游鏡像，上游 .gitignore 亦排除）
for dep in glad_gen tinygltf; do
    if [ -d "$REPO_ROOT/external/$dep" ]; then
        mkdir -p "$TARGET/external"
        rm -rf "$TARGET/external/$dep"
        cp -r "$REPO_ROOT/external/$dep" "$TARGET/external/$dep"
    fi
done

# 3) 殘留檔：上游有、repo 無對應 → 移入 legacy/（保留但不污染鏡像）
# 注意:比對需用模組內相對路徑（如 Core/Interfaces/IAudio.h）,不能只用 basename,
# 否則子目錄檔案會被誤判 stale;legacy 目錄要建 dirname,直接建 $rel 會產生
# 「同名目錄包同名檔案」的嵌套。
moved=0
while IFS= read -r f; do
    rel="${f#$TARGET/}"          # include/<Mod>/<sub>/x.h 或 src/<Mod>/<sub>/x.cpp
    rest="${rel#*/}"             # <Mod>/<sub>/x.h
    mod="${rest%%/*}"            # <Mod>
    sub="${rest#*/}"             # <sub>/x.h
    [ -f "$REPO_ROOT/$mod/$sub" ] && continue
    mkdir -p "$TARGET/legacy/$(dirname "$rel")"
    mv "$f" "$TARGET/legacy/$rel"
    moved=$((moved+1))
    echo "  [legacy] $rel"
done < <(find "$TARGET/include" "$TARGET/src" -type f 2>/dev/null)

echo "Done: $total_h headers + $total_c sources synced, $moved stale file(s) -> legacy/"
