#pragma once

#include "Core/CoreTypes.h"

#include <functional>
#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

/**
 * 神話事件具名記錄（N-5，narrative-design + UX myth seepage）
 *
 * 具名神話事件日誌：哪座祠、哪尊靈、何時發生——
 * 結局時神的證詞引用這些記錄；同時作為 UX「神話滲透」
 * 主題切換的事件來源（OnEvent 回呼即滲透觸發介面，
 * E-6 未做前 UX 端可先 stub 消費）。
 */
struct MythEvent {
    std::string shrine;   // 祠（地名/廟名）
    std::string spirit;   // 靈（神祇/精怪名）
    std::string when;     // 時（章節或戰役時間標記）
    std::string detail;   // 記事（「神像流淚」「狐仙示警」…）
};

class MythLog {
public:
    // 記一筆具名神話事件；滲透介面即時觸發
    void Record(const std::string& shrine, const std::string& spirit,
                const std::string& when, const std::string& detail);

    const std::vector<MythEvent>& Events() const { return events; }
    size_t Count() const { return events.size(); }

    // 滲透觸發介面：UX 註冊回呼（主題切換/提示），每次 Record 呼叫
    void SetSeepageCallback(std::function<void(const MythEvent&)> cb) {
        seepage = std::move(cb);
    }

    // 結局神的證詞：每筆事件產一行引用
    std::vector<std::string> TestimonyLines() const;

    // potato.myth_log/1 序列化
    std::string ToJson() const;
    bool FromJson(const std::string& json);

private:
    std::vector<MythEvent> events;
    std::function<void(const MythEvent&)> seepage;
};

} // namespace Gameplay
} // namespace Potato
