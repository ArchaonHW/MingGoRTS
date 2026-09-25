/**
 * DownloadGuard 實作——落盤即掃目錄守衛
 */

#include "Security/DownloadGuard.h"
#include "Security/AuditLedger.h"

namespace Potato {
namespace Security {

namespace {

// findings 的 ruleId 串成摘要（入帳 payload 只留其 SHA-256）
std::string FindingIds(const FileScanResult& r) {
    std::string out;
    for (const auto& f : r.findings) {
        if (!out.empty()) out += ',';
        out += f.ruleId;
    }
    return out;
}

} // anonymous namespace

bool DownloadGuard::Watch(const std::string& dirPath) {
    if (!watcher.WatchDirectory(dirPath)) return false;
    dir = dirPath;
    return true;
}

void DownloadGuard::Unwatch() {
    watcher.UnwatchAll();
    dir.clear();
}

bool DownloadGuard::IsWatching() const {
    return !dir.empty() && watcher.IsWatching(dir);
}

void DownloadGuard::SetAutoQuarantine(bool on,
                                      const std::string& quarantineDir_) {
    autoQuarantine = on;
    quarantineDir = quarantineDir_;
}

std::vector<FileScanResult> DownloadGuard::Poll() {
    std::vector<FileScanResult> out;
    for (const auto& change : watcher.Poll()) {
        if (change.type == FileChangeType::Deleted) continue;
        if (change.type == FileChangeType::Modified && !scanModified) continue;

        FileScanResult r = scanner.ScanFile(change.path);
        if (callback) callback(r);

        // 非 Clean 入帳：payload 為可讀摘要,帳本只留其 SHA-256
        if (auditLedger && r.verdict != ScanVerdict::Clean) {
            auditLedger->Append("scan", ScanVerdictToString(r.verdict),
                                r.path + "|" + r.sha256 + "|" + FindingIds(r),
                                "download_guard:" + dir);
        }

        if (autoQuarantine && r.verdict == ScanVerdict::Malicious &&
            !quarantineDir.empty()) {
            // 隔離失敗不回頭改判定——檔案仍在原處,由回呼端決定後續
            const bool ok = scanner.QuarantineFile(change.path, quarantineDir,
                                                   &r, nullptr);
            if (auditLedger) {
                auditLedger->Append("quarantine", ok ? "ok" : "failed",
                                    r.path + "|" + r.sha256,
                                    "download_guard:" + dir);
            }
        }
        out.push_back(std::move(r));
    }
    return out;
}

} // namespace Security
} // namespace Potato
