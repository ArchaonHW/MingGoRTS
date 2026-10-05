#pragma once

#include "Core/CoreTypes.h"

#include <functional>
#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

class BattleController;

/**
 * 戰鬥錄製器（BattleRecorder）——T-7 事件錄製 → JSON 回放
 *
 * 不重跑模擬：只錄 BattleController 發出的事件流（含時間戳），
 * 回放時依時間軸逐條播出。浮點非決定性問題不存在——
 * 回放的是「發生過什麼」，不是「重算一次」。
 *
 * 用法：
 *   recorder.Attach(battle);          // 包進既有事件回調
 *   ... battle.Update(dt) ...
 *   recorder.SaveToFile("replay.json");
 *   recorder.Replay(cb);              // 依錄製時間播出事件
 */
class BattleRecorder {
public:
    struct Record {
        float t;            // 遊戲時間（秒）
        std::string event;  // 事件文字
    };

    // 包裝 battle 的 SetEventCallback：事件進 recorder 後繼續往下游傳
    void Attach(BattleController& battle);

    const std::vector<Record>& GetRecords() const { return records; }
    size_t Count() const { return records.size(); }

    // 存/讀 JSON（potato.battle_replay/1）
    bool SaveToFile(const std::string& path) const;
    bool LoadFromFile(const std::string& path);

    // 回放：cb(t, event) 依序播出；timeScale 可加速（0 = 瞬間播完）
    void Replay(const std::function<void(float, const std::string&)>& cb,
                float timeScale = 0.0f) const;

private:
    std::vector<Record> records;
};

} // namespace Gameplay
} // namespace Potato
