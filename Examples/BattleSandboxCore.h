#pragma once

// BattleSandboxCore — G-4 沙盤模擬器共用核心（header-only）。
//
// deck-vs-deck 批次模擬：兩副 potato.doctrine_set/1 牌組在
// 鏡像斷橋場景對局 N 場，輸出勝率/均時長/各 trigger 命中數。
// 對稱場景（同兵數鏡像站位）隔離牌組品質；moraleExecThreshold
// 保持 0 → 戰鬥內部無 RNG，同 seed 跨場完全可重現。

#include "Gameplay/BattleController.h"
#include "Gameplay/Doctrine.h"
#include "Serialization/JsonParser.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

struct SandboxDeck {
    std::string name;      // 顯示用（預設檔名去副檔名）
    DoctrineSet doctrine;  // 該隊全部小隊共用同一副
};

struct SandboxConfig {
    SandboxDeck deckA, deckB;
    int matches = 50;
    unsigned seed = 1;
    float maxSeconds = 60.0f;
};

struct SandboxMatch {
    int winnerTeam = -1;   // 0/1；-1 = 和局或超時
    float duration = 0.0f;
    std::map<std::string, int> trigA, trigB; // 本場 trigger 命中數
};

struct SandboxReport {
    std::string schema = "potato.sandbox_report/1";
    unsigned seed = 0;
    int matches = 0;
    int winsA = 0, winsB = 0, draws = 0;
    float totalDuration = 0.0f;
    std::map<std::string, int> trigA, trigB; // 累計
    std::vector<SandboxMatch> results;
};

// 單場對局：鏡像斷橋（20x15，中央牆缺口 y6-8），雙方四隊同兵數
// 鏡像站位——站位抖動對兩邊一致套用，seed 只改佈局不改公平性。
inline SandboxMatch SandboxRunMatch(const SandboxConfig& cfg,
                                    unsigned matchSeed) {
    std::mt19937 rng(matchSeed);
    auto jit = [&](float base, float range) {
        // 整數抖動：uniform_real_distribution 跨標準庫實作異果，
        // CI 可重現需要純整數運算（L-8 同款守衛）
        const int m = static_cast<int>(range * 100.0f);
        return base + static_cast<float>(static_cast<int>(rng() %
                       static_cast<unsigned>(2 * m + 1)) - m) / 100.0f;
    };

    BattleController battle(20, 15, 1.0f);
    for (int y = 0; y < 15; ++y) {
        if (y < 6 || y > 8) battle.GetField().SetObstacle(10, y, true);
    }
    battle.SetObjective(0, Vector2(18, 7));
    battle.SetObjective(1, Vector2(2, 7));
    battle.SetRallyPoint(0, Vector2(1, 7));
    battle.SetRallyPoint(1, Vector2(19, 7));
    battle.SetCommandPoints(0, 0); // 沙盤只測 doctrine——無 CP 介入
    battle.SetCommandPoints(1, 0);

    const char* namesA[] = {"甲前鋒", "甲左翼", "甲右翼", "甲預備"};
    const char* namesB[] = {"乙前鋒", "乙左翼", "乙右翼", "乙預備"};
    const Vector2 posA[] = {{3, 4}, {2, 7}, {3, 10}, {5, 7}};
    for (int i = 0; i < 4; ++i) {
        const float jx = jit(0.0f, 0.8f), jy = jit(0.0f, 0.8f);
        const int members = 10 + static_cast<int>(rng() % 4); // 10-13 兩邊同額
        Vector2 a(std::clamp(posA[i].x + jx, 0.5f, 9.0f),
                  std::clamp(posA[i].y + jy, 0.5f, 14.5f));
        // 鏡像：x 對折（20 格寬 → 19-x），y 不動
        Vector2 b(std::clamp(19.0f - a.x, 10.5f, 19.5f), a.y);
        Squad* sa = battle.CreateSquad(namesA[i], 0, a, members);
        Squad* sb = battle.CreateSquad(namesB[i], 1, b, members);
        battle.AssignDoctrine(sa, cfg.deckA.doctrine);
        battle.AssignDoctrine(sb, cfg.deckB.doctrine);
    }

    battle.BeginExecution();

    SandboxMatch m;
    const float dt = 0.05f; // 固定步長 20tick/s——勿用真實幀時
    float t = 0.0f;
    while (battle.GetOutcome() == BattleOutcome::Ongoing &&
           t < cfg.maxSeconds) {
        battle.Update(dt);
        t += dt;
    }
    m.duration = t;
    if (battle.GetOutcome() == BattleOutcome::Victory) m.winnerTeam = 0;
    else if (battle.GetOutcome() == BattleOutcome::Defeat) m.winnerTeam = 1;

    for (const auto& [trg, n] : battle.TriggerUsage(0)) {
        m.trigA[TriggerName(trg)] = n;
    }
    for (const auto& [trg, n] : battle.TriggerUsage(1)) {
        m.trigB[TriggerName(trg)] = n;
    }
    return m;
}

inline SandboxReport RunSandbox(const SandboxConfig& cfg) {
    SandboxReport rep;
    rep.seed = cfg.seed;
    rep.matches = cfg.matches;
    for (int i = 0; i < cfg.matches; ++i) {
        SandboxMatch m = SandboxRunMatch(cfg, cfg.seed +
                                         static_cast<unsigned>(i));
        if (m.winnerTeam == 0) ++rep.winsA;
        else if (m.winnerTeam == 1) ++rep.winsB;
        else ++rep.draws;
        rep.totalDuration += m.duration;
        for (const auto& [k, n] : m.trigA) rep.trigA[k] += n;
        for (const auto& [k, n] : m.trigB) rep.trigB[k] += n;
        rep.results.push_back(std::move(m));
    }
    return rep;
}

inline std::string SandboxReportToJson(const SandboxReport& rep,
                                       const std::string& nameA,
                                       const std::string& nameB) {
    auto trigJson = [](const std::map<std::string, int>& t) {
        std::string s = "{";
        bool first = true;
        for (const auto& [k, n] : t) {
            if (!first) s += ",";
            first = false;
            s += "\"" + k + "\":" + std::to_string(n);
        }
        return s + "}";
    };
    std::ostringstream o;
    o << "{\"schema\":\"" << rep.schema
      << "\",\"seed\":" << rep.seed
      << ",\"matches\":" << rep.matches
      << ",\"avgDuration\":"
      << (rep.matches > 0 ? rep.totalDuration / rep.matches : 0.0f)
      << ",\"deckA\":{\"name\":\"" << nameA
      << "\",\"wins\":" << rep.winsA
      << ",\"triggerHits\":" << trigJson(rep.trigA) << "}"
      << ",\"deckB\":{\"name\":\"" << nameB
      << "\",\"wins\":" << rep.winsB
      << ",\"triggerHits\":" << trigJson(rep.trigB) << "}"
      << ",\"draws\":" << rep.draws << "}";
    return o.str();
}

// 讀整個檔案並解析 doctrine_set；失敗回 false
inline bool SandboxLoadDeck(const std::string& path, SandboxDeck& out) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return false;
    std::stringstream ss;
    ss << f.rdbuf();
    if (!out.doctrine.FromJson(ss.str())) return false;
    const size_t slash = path.find_last_of("/\\");
    const std::string base =
        slash == std::string::npos ? path : path.substr(slash + 1);
    const size_t dot = base.find_last_of('.');
    out.name = dot == std::string::npos ? base : base.substr(0, dot);
    return true;
}

} // namespace Gameplay
} // namespace Potato
