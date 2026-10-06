#pragma once

// JsonValue → 字串的最小遞迴寫出器。
// JsonParser.h 只讀不寫；戰役存檔需要把聚合的 JsonValue 文件
// 落成單一檔案，故補一個對稱的寫出器（含字串跳脫與數字格式）。
// 數字用 %g 寫出：整數不帶小數點、浮點保精度，roundtrip 安全。

#include "Serialization/JsonParser.h"

#include <cstdio>
#include <string>

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
        std::snprintf(buf, sizeof(buf), "%g", v.numberValue);
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
        bool first = true;
        for (const auto& kv : v.objectValue) {
            if (!first) out += ',';
            first = false;
            WriteJsonString(out, kv.first);
            out += ':';
            WriteJsonValue(out, kv.second);
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
