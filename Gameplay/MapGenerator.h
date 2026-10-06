#pragma once

#include "Core/CoreTypes.h"

#include <cstdint>
#include <string>

namespace Potato {
namespace Gameplay {

/**
 * 程序戰場生成（G-10：ELIN × 程序生成 → BattleMap JSON）
 *
 * seeded 產生 potato.battle_map/1 JSON，可直接餵 BattleMap loader：
 *   河道（蜿蜒正弦 + 渡口）/ 高地（G-4 攻方加成）/ 森林（守方減傷）/
 *   泥濘河岸（cost 減速）/ 南北部署區 + 集結/敵營圖釘。
 *
 * 高度/植被場是兩組獨立正弦疊加（與 CurlNoise 同思路的標量版），
 * 同 seed 逐位一致 → 可回放、可測試。
 */
class MapGenerator {
public:
    struct Config {
        int width = 24;
        int height = 16;
        float cellSize = 1.0f;
        int riverBand = 3;          // 河道厚度（格）
        int fordWidth = 2;          // 渡口寬（格）
        float highlandThr = 0.55f;  // 高度場閾值 → 高地
        float forestThr = 0.45f;    // 植被場閾值 → 森林
    };

    // 產生整張地圖 JSON；同 seed+config 輸出逐位一致
    static std::string GenerateJson(uint64_t seed, const Config& cfg);
    static std::string GenerateJson(uint64_t seed) {
        return GenerateJson(seed, Config{});
    }
};

} // namespace Gameplay
} // namespace Potato
