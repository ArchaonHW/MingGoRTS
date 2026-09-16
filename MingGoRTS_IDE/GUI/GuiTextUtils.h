#pragma once

// Shared text-buffer helpers for IDE GUI code and headless smoke tests.
// Kept header-only so tests can exercise them without linking the GUI target.

#include <cstring>
#include <string>
#include <cstdlib>

namespace MingGoRTSIDE {

// 可移植環境變數讀取（/sdl 下 MSVC 的 getenv 被禁用；Windows 用 _dupenv_s）
inline std::string GetEnvVar(const char* name) {
#ifdef _WIN32
    char* value = nullptr;
    size_t len = 0;
    if (_dupenv_s(&value, &len, name) != 0 || value == nullptr) {
        return "";
    }
    std::string result(value, len > 0 ? len - 1 : 0); // len 含 null 結尾
    std::free(value);
    return result;
#else
    const char* value = std::getenv(name);
    return value ? std::string(value) : "";
#endif
}

// 寫入 char 緩衝區並保證 NUL 結尾；超出時附加 "...[truncated]" 標記。
// /sdl 禁用 strncpy/strncat —— 用 memcpy 配精確長度。
inline void CopyToBuffer(char* dst, size_t dstSize, const std::string& src) {
    if (dstSize == 0) return;
    const char* truncMarker = "...[truncated]";
    const size_t markerLen = strlen(truncMarker);
    const bool trunc = src.size() >= dstSize && dstSize > markerLen + 1;
    const size_t keep = src.size() < dstSize ? src.size()
        : (dstSize > markerLen + 1 ? dstSize - markerLen - 1 : dstSize - 1);
    memcpy(dst, src.data(), keep);
    size_t pos = keep;
    if (trunc) {
        memcpy(dst + pos, truncMarker, markerLen);
        pos += markerLen;
    }
    dst[pos] = '\0';
}

} // namespace MingGoRTSIDE
