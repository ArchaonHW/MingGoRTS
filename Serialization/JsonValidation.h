#pragma once
#include "Serialization/JsonParser.h"
#include <cmath>
namespace Potato::JsonValidation {
inline bool Integer(const JsonValue &v, int lo = 0, int hi = 1000000) {
    return v.IsNumber() && std::isfinite(v.numberValue) && v.numberValue >= lo &&
           v.numberValue <= hi && std::floor(v.numberValue) == v.numberValue;
}
inline bool Number(const JsonValue &v, double lo, double hi) {
    return v.IsNumber() && std::isfinite(v.numberValue) && v.numberValue >= lo &&
           v.numberValue <= hi;
}
inline bool Strings(const JsonValue &v) {
    if (!v.IsArray())
        return false;
    for (const auto &x : v.arrayValue)
        if (!x.IsString())
            return false;
    return true;
}
} // namespace Potato::JsonValidation
