#pragma once

// 章節狀態（E-7）：戰役進度的最小容器。
// 弧序固定（軍閥/北伐/抗戰/內戰），弧內章節序自由；
// frontsTaken 記已收服戰線的內容 id（對照地圖/章節資產）。

#include "Serialization/JsonParser.h"

#include <string>
#include <vector>

namespace Potato {
namespace Campaign {

struct ChapterState {
    int arc = 0;                // 0=軍閥 1=北伐 2=抗戰 3=內戰
    int chapter = 0;            // 弧內章節序
    std::string chapterId;      // 內容 id（"duanqiao" 等）
    std::vector<std::string> frontsTaken; // 已收服戰線 id

    JsonValue ToJson() const {
        JsonValue o;
        o.type = JsonValue::Type::Object;
        o.objectValue["arc"] = JsonValue::Number(arc);
        o.objectValue["chapter"] = JsonValue::Number(chapter);
        o.objectValue["chapter_id"] = JsonValue::String(chapterId);
        JsonValue f;
        f.type = JsonValue::Type::Array;
        for (const auto& s : frontsTaken) {
            f.arrayValue.push_back(JsonValue::String(s));
        }
        o.objectValue["fronts_taken"] = f;
        return o;
    }

    void FromJson(const JsonValue& j) {
        arc = j["arc"].AsInt(0);
        chapter = j["chapter"].AsInt(0);
        chapterId = j["chapter_id"].AsString();
        frontsTaken.clear();
        for (const auto& s : j["fronts_taken"].AsArray()) {
            frontsTaken.push_back(s.AsString());
        }
    }
};

} // namespace Campaign
} // namespace Potato
