#pragma once

// JsonValue → 字串的最小遞迴寫出器。
// JsonParser.h 只讀不寫；戰役存檔需要把聚合的 JsonValue 文件
// 落成單一檔案，故補一個對稱的寫出器（含字串跳脫與數字格式）。
// 數字用「最短 roundtrip」寫出：由 6 位有效數字起逐步升精度，
// 直到 strtod 讀回原值——0.6 寫 0.6 而非 0.59999999999999998，
// 同時保證任意 double roundtrip 無損（舊 %g 六位在 >~1e6 時截斷）。

#include "Serialization/JsonParser.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace Potato {
namespace Campaign {

inline void WriteJsonString(std::string& out, const std::string& s) {
    out += '"';
    for (char c : s) {
        switch (c) {
        case '"':  out += "\\\""; break;
        case '\\': out += "\\\\"; break;
        case '\n': out += "\\n"; break;
        case '\r': out += "\\r"; break;
        case '\t': out += "\\t"; break;
        default:
            // 控制字元一律 \uXXXX，UTF-8 高位元組原樣透傳
            if (static_cast<unsigned char>(c) < 0x20) {
                char buf[8];
                std::snprintf(buf, sizeof(buf), "\\u%04x", c & 0xFF);
                out += buf;
            } else {
                out += c;
            }
        }
    }
    out += '"';
}

inline void WriteJsonValue(std::string& out, const JsonValue& v) {
    switch (v.type) {
    case JsonValue::Type::Null:
        out += "null";
        break;
    case JsonValue::Type::Bool:
        out += v.boolValue ? "true" : "false";
        break;
    case JsonValue::Type::Number: {
        char buf[32];
        // 最短 roundtrip：由低到高試精度，第一個能讀回原值的就用
        for (int prec = 6; prec <= 17; ++prec) {
            std::snprintf(buf, sizeof(buf), "%.*g", prec,
                          v.numberValue);
            if (std::strtod(buf, nullptr) == v.numberValue) break;
        }
        out += buf;
        break;
    }
    case JsonValue::Type::String:
        WriteJsonString(out, v.stringValue);
        break;
    case JsonValue::Type::Array:
        out += '[';
        for (size_t i = 0; i < v.arrayValue.size(); ++i) {
            if (i) out += ',';
            WriteJsonValue(out, v.arrayValue[i]);
        }
        out += ']';
        break;
    case JsonValue::Type::Object: {
        out += '{';
        // objectValue 是 unordered_map——迭代序不確定，
        // 排序鍵寫出讓同文件同位元組（replay/golden diff 可比對）
        std::vector<std::string> keys;
        keys.reserve(v.objectValue.size());
        for (const auto& kv : v.objectValue) keys.push_back(kv.first);
        std::sort(keys.begin(), keys.end());
        bool first = true;
        for (const std::string& k : keys) {
            if (!first) out += ',';
            first = false;
            WriteJsonString(out, k);
            out += ':';
            WriteJsonValue(out, v.objectValue.at(k));
        }
        out += '}';
        break;
    }
    }
}

inline std::string WriteJson(const JsonValue& v) {
    std::string out;
    WriteJsonValue(out, v);
    return out;
}

} // namespace Campaign
} // namespace Potato
