// CardValidator CLI — G-3 敵將卡目錄掃描器（CI 可用）。
//
// 用法：CardValidator [cardsDir] [assetsRoot]
//   預設掃 assets/cards（assetsRoot=assets，查立繪存在性）。
// 每卡印 OK/問題清單；任一 error → exit 1。
#include "Gameplay/CardValidator.h"

#include <cstdio>

using Potato::Gameplay::CardValidator;

int main(int argc, char** argv) {
    const std::string dir = argc > 1 ? argv[1] : "assets/cards";
    const std::string root = argc > 2 ? argv[2] : "assets";

    auto reports = CardValidator::ValidateDir(dir, root);
    int errors = 0, warns = 0;
    for (const auto& r : reports) {
        std::printf("%s [%s]\n", r.Ok() ? "OK  " : "FAIL",
                    r.path.c_str());
        for (const auto& i : r.issues) {
            std::printf("  %s %s: %s\n", i.error ? "ERR " : "warn",
                        i.field.c_str(), i.message.c_str());
            i.error ? ++errors : ++warns;
        }
    }
    std::printf("---- %zu cards, %d errors, %d warnings\n",
                reports.size(), errors, warns);
    return errors > 0 ? 1 : 0;
}
