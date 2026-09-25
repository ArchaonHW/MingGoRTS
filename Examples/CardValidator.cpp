// CardValidator CLI — G-3 敵將卡目錄掃描器（CI 可用）。
//
// 用法：
//   CardValidator [--fix] [cardsDir] [assetsRoot] [doctrineDir]
//   預設掃 assets/cards（assetsRoot=assets，查立繪存在性；
//   doctrineDir=assets/doctrine 交叉驗 signatureDoctrineId 引用）。
//   --fix：warn 級問題自動修正並寫回（tmp+rename 原子替換），
//         error 級拒存。每卡印 OK/FIX/SKIP + 問題清單；
//         任一 error → exit 1。
#include "Gameplay/CardValidator.h"

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using Potato::Gameplay::CardReport;
using Potato::Gameplay::CardValidator;
using Potato::Gameplay::FixResult;

// 修正後內容原子寫回：tmp+rename，中途失敗不留半檔
static bool WriteBack(const std::string& path,
                      const std::string& content) {
    const std::string tmp = path + ".fixtmp";
    {
        std::ofstream f(tmp, std::ios::trunc);
        if (!f) return false;
        f << content;
        if (!f.good()) return false;
    }
    std::error_code ec;
    fs::rename(tmp, path, ec);
    if (ec) {
        std::error_code ec2;
        fs::remove(path, ec2);
        ec.clear();
        fs::rename(tmp, path, ec);
        if (ec) {
            fs::remove(tmp, ec2);
            return false;
        }
    }
    return true;
}

int main(int argc, char** argv) {
    bool fix = false;
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--fix") == 0) {
            fix = true;
        } else {
            args.push_back(argv[i]);
        }
    }
    const std::string dir =
        args.size() > 0 ? args[0] : "assets/cards";
    const std::string root = args.size() > 1 ? args[1] : "assets";
    const std::string doctrine =
        args.size() > 2 ? args[2] : "assets/doctrine";

    auto reports = CardValidator::ValidateDir(dir, root, doctrine);
    int errors = 0, warns = 0, fixed = 0, skipped = 0;
    for (const auto& r : reports) {
        std::printf("%s [%s]\n", r.Ok() ? "OK  " : "FAIL",
                    r.path.c_str());
        for (const auto& i : r.issues) {
            std::printf("  %s %s: %s\n", i.error ? "ERR " : "warn",
                        i.field.c_str(), i.message.c_str());
            i.error ? ++errors : ++warns;
        }
        if (!fix) continue;

        // --fix：先讀原檔修正再驗證；可寫才落盤
        std::ifstream in(r.path);
        if (!in) continue;
        std::ostringstream ss;
        ss << in.rdbuf();
        FixResult fr = CardValidator::FixCard(ss.str());
        if (!fr.writable) {
            ++skipped;
            std::printf("  SKIP: error 級不修（%zu 項）\n",
                        fr.report.issues.size());
            continue;
        }
        if (!fr.changed) continue;
        ++fixed;
        for (const auto& msg : fr.fixes)
            std::printf("  fix : %s\n", msg.c_str());
        if (!WriteBack(r.path, fr.json)) {
            std::printf("  ERR : 寫回失敗\n");
            ++errors;
        }
    }
    std::printf("---- %zu cards, %d errors, %d warnings",
                reports.size(), errors, warns);
    if (fix) std::printf(", %d fixed, %d skipped", fixed, skipped);
    std::printf("\n");
    return errors > 0 ? 1 : 0;
}
