// BattleSandbox — G-4 沙盤模擬器（FR14 平衡工具）。
//
// 用法：
//   BattleSandbox --deck-a <doctrine_set.json> --deck-b <file>
//                 [--matches N] [--seed S] [--max-seconds T]
//                 [--out report.json]
//
// 輸出 stdout 摘要 + 可選 potato.sandbox_report/1 JSON。
// 同 seed 完全可重現（戰鬥內部 RNG 未啟用、佈局抖動純整數運算）。

#include "Examples/BattleSandboxCore.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>

using namespace Potato::Gameplay;

int main(int argc, char** argv) {
    SandboxConfig cfg;
    std::string pathA, pathB, outPath;
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        auto need = [&]() -> const char* {
            if (i + 1 >= argc) {
                std::fprintf(stderr, "missing value for %s\n",
                             a.c_str());
                std::exit(2);
            }
            return argv[++i];
        };
        if (a == "--deck-a") pathA = need();
        else if (a == "--deck-b") pathB = need();
        else if (a == "--out") outPath = need();
        else if (a == "--matches") cfg.matches = std::atoi(need());
        else if (a == "--seed") cfg.seed =
            static_cast<unsigned>(std::strtoul(need(), nullptr, 10));
        else if (a == "--max-seconds")
            cfg.maxSeconds = static_cast<float>(std::atof(need()));
        else {
            std::fprintf(stderr, "unknown arg: %s\n", a.c_str());
            std::fprintf(stderr,
                "usage: BattleSandbox --deck-a F --deck-b F "
                "[--matches N] [--seed S] [--max-seconds T] "
                "[--out report.json]\n");
            return 2;
        }
    }
    if (pathA.empty() || pathB.empty() || cfg.matches <= 0) {
        std::fprintf(stderr,
            "usage: BattleSandbox --deck-a F --deck-b F "
            "[--matches N] [--seed S] [--max-seconds T] "
            "[--out report.json]\n");
        return 2;
    }
    if (!SandboxLoadDeck(pathA, cfg.deckA)) {
        std::fprintf(stderr, "cannot load deck A: %s\n", pathA.c_str());
        return 1;
    }
    if (!SandboxLoadDeck(pathB, cfg.deckB)) {
        std::fprintf(stderr, "cannot load deck B: %s\n", pathB.c_str());
        return 1;
    }

    SandboxReport rep = RunSandbox(cfg);

    std::printf("=== BattleSandbox (G-4) ===\n");
    std::printf("deck A \"%s\" vs deck B \"%s\"  matches=%d seed=%u\n",
                cfg.deckA.name.c_str(), cfg.deckB.name.c_str(),
                rep.matches, rep.seed);
    std::printf("wins: A=%d B=%d draws=%d  avg duration=%.1fs\n",
                rep.winsA, rep.winsB, rep.draws,
                rep.matches > 0 ? rep.totalDuration / rep.matches : 0.0f);
    auto dump = [](const char* tag,
                   const std::map<std::string, int>& t) {
        std::printf("%s trigger hits:", tag);
        if (t.empty()) std::printf(" (none)");
        for (const auto& [k, n] : t) std::printf(" %s=%d", k.c_str(), n);
        std::printf("\n");
    };
    dump("  A", rep.trigA);
    dump("  B", rep.trigB);

    if (!outPath.empty()) {
        std::ofstream o(outPath, std::ios::binary | std::ios::trunc);
        if (!o) {
            std::fprintf(stderr, "cannot write %s\n", outPath.c_str());
            return 1;
        }
        o << SandboxReportToJson(rep, cfg.deckA.name, cfg.deckB.name)
          << "\n";
    }
    return 0;
}
