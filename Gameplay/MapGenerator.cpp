#include "MapGenerator.h"

#include <algorithm>
#include <cmath>
#include <random>
#include <sstream>
#include <vector>

namespace Potato {
namespace Gameplay {
namespace {

// seeded 正弦疊加標量場（CurlNoise 思路的標量版）：
// n(x,y) = Σᵢ Aᵢ·sin(kᵢ·p + φᵢ)，A 遞減 → 大尺度主導、無格線週期
struct ScalarField {
    struct Wave { float kx, ky, amp, phase; };
    std::vector<Wave> waves;

    void Build(int octaves, uint64_t seed) {
        std::mt19937_64 rng(seed);
        std::uniform_real_distribution<float> u01(0.0f, 1.0f);
        float freq = 0.10f, amp = 1.0f;
        for (int i = 0; i < octaves; ++i) {
            const float az = 6.2831853f * u01(rng);
            waves.push_back({std::cos(az) * freq, std::sin(az) * freq,
                             amp * (0.5f + u01(rng)),
                             6.2831853f * u01(rng)});
            freq *= 2.0f;
            amp *= 0.5f;
        }
    }

    float Sample(float x, float y) const {
        float n = 0.0f;
        for (const Wave& w : waves)
            n += w.amp * std::sin(w.kx * x + w.ky * y + w.phase);
        return n; // 約 [-1.5, 1.5]
    }
};

// 格子類別（合併 rect 的 key）
enum class Cell : uint8 { Plain, Water, Ford, Mud, Highland, Forest };

const char* CellNote(Cell c) {
    switch (c) {
    case Cell::Water:    return "河道（不可通行）";
    case Cell::Ford:     return "渡口（泥濘涉水）";
    case Cell::Mud:      return "河岸泥濘（減速）";
    case Cell::Highland: return "高地（攻方加成）";
    case Cell::Forest:   return "森林（守方減傷）";
    default:             return "";
    }
}

struct Patch {
    int x, y, w, h;
    Cell cell;
};

} // namespace

std::string MapGenerator::GenerateJson(uint64_t seed, const Config& cfg) {
    const int W = cfg.width, H = cfg.height;
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<float> u01(0.0f, 1.0f);

    // ---- 河道：蜿蜒正弦中心線 ----
    const float midY = H * 0.5f;
    const float meanderAmp = (H * 0.15f) * (0.5f + u01(rng));
    const float meanderFreq = 1.0f + u01(rng) * 1.5f;
    const float meanderPhase = u01(rng) * 6.2831853f;
    auto riverCenter = [&](int x) {
        return midY + meanderAmp *
               std::sin(6.2831853f * meanderFreq * x / W + meanderPhase);
    };
    // 渡口位置（避開邊緣）
    const int fordX = 2 + static_cast<int>(u01(rng) * (W - cfg.fordWidth - 4));

    // ---- 地形分類 ----
    ScalarField heightF, vegF;
    heightF.Build(5, seed ^ 0x9e3779b9u);
    vegF.Build(5, seed ^ 0x243f6a88u);

    std::vector<Cell> cells(static_cast<size_t>(W) * H, Cell::Plain);
    int riverTop = H, riverBot = 0;
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            Cell& c = cells[static_cast<size_t>(y) * W + x];
            const float cy = riverCenter(x);
            const float dy = y - cy;
            if (std::fabs(dy) < cfg.riverBand * 0.5f + 0.5f) {
                riverTop = std::min(riverTop, y);
                riverBot = std::max(riverBot, y + 1);
                c = (x >= fordX && x < fordX + cfg.fordWidth) ? Cell::Ford
                                                            : Cell::Water;
                continue;
            }
            // 河岸泥濘帶（水邊一格）
            if (std::fabs(dy) < cfg.riverBand * 0.5f + 1.5f) {
                c = Cell::Mud;
                continue;
            }
            const float h = heightF.Sample(static_cast<float>(x),
                                           static_cast<float>(y));
            const float v = vegF.Sample(static_cast<float>(x),
                                        static_cast<float>(y));
            if (h > cfg.highlandThr) c = Cell::Highland;
            else if (v > cfg.forestThr) c = Cell::Forest;
        }
    }
    riverTop = std::max(0, riverTop);
    riverBot = std::min(H, riverBot);

    // ---- 行內連段 → 逐列合併成 rect ----
    std::vector<Patch> patches;
    for (int y = 0; y < H; ++y) {
        int x = 0;
        while (x < W) {
            const Cell c = cells[static_cast<size_t>(y) * W + x];
            if (c == Cell::Plain) { ++x; continue; }
            int x1 = x + 1;
            while (x1 < W && cells[static_cast<size_t>(y) * W + x1] == c)
                ++x1;
            patches.push_back({x, y, x1 - x, 1, c});
            x = x1;
        }
    }
    // 垂直合併（同 x/w/cell 的相鄰列）
    std::vector<Patch> merged;
    for (const Patch& p : patches) {
        if (!merged.empty() && merged.back().x == p.x &&
            merged.back().w == p.w && merged.back().cell == p.cell &&
            merged.back().y + merged.back().h == p.y) {
            merged.back().h += 1;
        } else {
            merged.push_back(p);
        }
    }

    // ---- JSON 輸出（potato.battle_map/1）----
    std::ostringstream js;
    js << "{\n  \"schema\": \"potato.battle_map/1\",\n"
       << "  \"name\": \"程序戰場-" << seed << "\",\n"
       << "  \"grid\": {\"width\": " << W << ", \"height\": " << H
       << ", \"cellSize\": " << cfg.cellSize << "},\n"
       << "  \"terrain\": [\n";
    bool first = true;
    for (const Patch& p : merged) {
        if (!first) js << ",\n";
        first = false;
        const bool blocked = (p.cell == Cell::Water);
        const float cost = (p.cell == Cell::Mud || p.cell == Cell::Ford)
                               ? 2.0f
                               : (p.cell == Cell::Forest ? 1.5f : 1.0f);
        js << "    {\"rect\": [" << p.x << ", " << p.y << ", " << p.w
           << ", " << p.h << "]"
           << ", \"blocked\": " << (blocked ? "true" : "false")
           << ", \"cost\": " << cost;
        // Water/Ford 不標 type（保持 Plain）；其餘給 G-4 地形類型
        if (p.cell == Cell::Highland) js << ", \"type\": \"highland\"";
        else if (p.cell == Cell::Forest) js << ", \"type\": \"forest\"";
        else if (p.cell == Cell::Mud || p.cell == Cell::Ford)
            js << ", \"type\": \"mud\"";
        js << ", \"note\": \"" << CellNote(p.cell) << "\"}";
    }
    js << "\n  ],\n";

    js << "  \"zones\": [\n"
       << "    {\"name\": \"北岸\", \"rect\": [0, 0, " << W << ", "
       << riverTop << "]},\n"
       << "    {\"name\": \"河面\", \"rect\": [0, " << riverTop << ", "
       << W << ", " << (riverBot - riverTop) << "]},\n"
       << "    {\"name\": \"南岸\", \"rect\": [0, " << riverBot << ", "
       << W << ", " << (H - riverBot) << "]}\n"
       << "  ],\n";

    js << "  \"fords\": [\n"
       << "    {\"name\": \"渡口\", \"rect\": [" << fordX << ", "
       << riverTop << ", " << cfg.fordWidth << ", "
       << (riverBot - riverTop) << "]}\n"
       << "  ],\n";

    js << "  \"deployZones\": {\n"
       << "    \"0\": {\"rect\": [0, 0, " << W << ", "
       << std::max(1, riverTop - 1) << "]},\n"
       << "    \"1\": {\"rect\": [0, " << std::min(H - 1, riverBot + 1)
       << ", " << W << ", "
       << std::max(1, H - riverBot - 1) << "]}\n"
       << "  },\n";

    const float pinX = W * 0.5f;
    js << "  \"pins\": [\n"
       << "    {\"name\": \"渡口\", \"pos\": ["
       << (fordX + cfg.fordWidth * 0.5f) << ", "
       << (riverTop + riverBot) * 0.5f << "], \"note\": \"必爭之地\"},\n"
       << "    {\"name\": \"北岸集結點\", \"pos\": [" << pinX << ", "
       << std::max(1.0f, riverTop * 0.5f)
       << "], \"note\": \"我方集結/撤退點\"},\n"
       << "    {\"name\": \"南岸敵營\", \"pos\": [" << pinX << ", "
       << (riverBot + H) * 0.5f - 0.5f
       << "], \"note\": \"敵方營地，玩家進攻目標\"}\n"
       << "  ]\n}\n";
    return js.str();
}

} // namespace Gameplay
} // namespace Potato
