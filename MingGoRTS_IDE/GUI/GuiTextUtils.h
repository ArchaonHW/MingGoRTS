#pragma once

// Shared text-buffer helpers for IDE GUI code and headless smoke tests.
// Kept header-only so tests can exercise them without linking the GUI target.

#include <cstring>
#include <string>
#include <cstdlib>
#include <vector>

#ifdef _WIN32
// windows.h 的 min/max 巨集會污染包含本標頭的 TU——先關掉
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
// Win32 相容巨集：GetCurrentTime 被定義成 GetTickCount 別名，
// 會把 IDEGUI::GetCurrentTime 成員函式改名——這裡解除
#ifdef GetCurrentTime
#undef GetCurrentTime
#endif
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

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
    if (!dst || dstSize == 0) return;
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

// DevAssistant 生成結果 → 固定 buffer 的寫回 seam（UI-free,可無頭測試）：
// 成功寫入 generatedCode,失敗寫入 "Error: <error>"。
// PollDevelopmentResult 與 headless 測試共用此路徑。
inline void WriteGenerationResult(bool success, const std::string& generatedCode,
                                  const std::string& error, char* dst, size_t dstSize) {
    CopyToBuffer(dst, dstSize, success ? generatedCode : "Error: " + error);
}

// 無 shell 的行程啟動：參數為獨立字串，不經 cmd.exe/sh——
// 消掉 std::system 的命令注入面與 quoting 脆弱性。
// 回傳子行程 exit code；啟動失敗回 -1。
inline int RunProcess(const std::string& exe,
                      const std::vector<std::string>& args) {
#ifdef _WIN32
    // CreateProcess 吃單一 command line——參數按 MSVCRT 規則 quoting
    // （反斜線只在 " 前與結尾翻倍；內嵌 " 轉 \"）
    auto quoteArg = [](const std::string& a) {
        std::string out = "\"";
        size_t backslashes = 0;
        for (char ch : a) {
            if (ch == '\\') { ++backslashes; continue; }
            if (ch == '"') { out.append(backslashes * 2 + 1, '\\'); out += '"'; backslashes = 0; continue; }
            out.append(backslashes, '\\'); out += ch; backslashes = 0;
        }
        out.append(backslashes * 2, '\\');
        out += '"';
        return out;
    };
    std::string cmd = quoteArg(exe);
    for (const auto& a : args) { cmd += ' '; cmd += quoteArg(a); }

    STARTUPINFOA si{};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi{};
    if (!CreateProcessA(nullptr, cmd.data(), nullptr, nullptr, FALSE,
                        0, nullptr, nullptr, &si, &pi)) {
        return -1;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code = 1;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return static_cast<int>(code);
#else
    std::vector<std::string> argvStore;
    argvStore.push_back(exe);
    for (const auto& a : args) argvStore.push_back(a);
    std::vector<char*> argv;
    for (auto& s : argvStore) argv.push_back(s.data());
    argv.push_back(nullptr);

    pid_t pid = fork();
    if (pid < 0) return -1;
    if (pid == 0) {
        execvp(argv[0], argv.data());
        _exit(127);
    }
    int status = 0;
    while (waitpid(pid, &status, 0) < 0) {}
    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
#endif
}

} // namespace MingGoRTSIDE
