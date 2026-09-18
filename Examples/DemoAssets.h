#pragma once

// U-1 垂直切片 demo：資產路徑解析。
// 包裝後 assets/ 放 exe 旁即自動命中；開發期從 build/bin 跑會向上探到
// repo 根。找不到時回退 cwd（維持舊行為，錯誤訊息由呼叫方給）。

#include <filesystem>
#include <string>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX // 不讓 min/max 巨集污染 std::min/std::max
#endif
#include <windows.h>
#endif

namespace DemoAssets {

// exe 所在目錄；取不到時回退 cwd
inline std::filesystem::path ExeDir() {
#ifdef _WIN32
    char buf[MAX_PATH]{};
    const DWORD n = GetModuleFileNameA(nullptr, buf, MAX_PATH);
    if (n > 0 && n < MAX_PATH) {
        return std::filesystem::path(buf).parent_path();
    }
#endif
    return std::filesystem::current_path();
}

// 回傳 assets/ 根目錄：exe 目錄起向上探 ≤6 層，找不到回退 cwd/assets
inline std::filesystem::path Root() {
    namespace fs = std::filesystem;
    fs::path dir = ExeDir();
    for (int i = 0; i < 6; ++i) {
        std::error_code ec;
        if (fs::is_directory(dir / "assets", ec)) {
            return dir / "assets";
        }
        const fs::path parent = dir.parent_path();
        if (parent == dir || parent.empty()) break;
        dir = parent;
    }
    return fs::current_path() / "assets";
}

// rel 是 assets/ 內的相對路徑（如 "maps/duanqiao.json"、"fonts/x.otf"）
inline std::string Resolve(const std::string& rel) {
    return (Root() / rel).generic_string();
}

} // namespace DemoAssets
