#pragma once

// Potato Engine 數學庫
// 包含所有數學相關的類型和函數

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Quaternion.h"

namespace Potato {

// 數學常量
namespace MathConstants {
    constexpr float PI = 3.14159265358979323846f;
    constexpr float TWO_PI = 2.0f * PI;
    constexpr float HALF_PI = PI / 2.0f;
    constexpr float QUARTER_PI = PI / 4.0f;
    constexpr float DEG_TO_RAD = PI / 180.0f;
    constexpr float RAD_TO_DEG = 180.0f / PI;
    constexpr float EPSILON = 1e-6f;
}

// 數學工具函數
namespace MathUtils {
    
    // 角度轉換
    inline float DegToRad(float degrees) {
        return degrees * MathConstants::DEG_TO_RAD;
    }
    
    inline float RadToDeg(float radians) {
        return radians * MathConstants::RAD_TO_DEG;
    }
    
    // 夾在範圍內
    template<typename T>
    inline T Clamp(T value, T min, T max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
    
    // 線性插值
    template<typename T>
    inline T Lerp(T a, T b, float t) {
        return a + (b - a) * t;
    }
    
    // 平滑插值
    inline float SmoothStep(float edge0, float edge1, float x) {
        float t = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
        return t * t * (3.0f - 2.0f * t);
    }
    
    // 平滑插值更平滑版本
    inline float SmootherStep(float edge0, float edge1, float x) {
        float t = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
        return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }
    
    // 絕對值
    template<typename T>
    inline T Abs(T value) {
        return value < 0 ? -value : value;
    }
    
    // 最小值
    template<typename T>
    inline T Min(T a, T b) {
        return a < b ? a : b;
    }
    
    // 最大值
    template<typename T>
    inline T Max(T a, T b) {
        return a > b ? a : b;
    }
    
    // 符號函數
    template<typename T>
    inline T Sign(T value) {
        if (value > 0) return 1;
        if (value < 0) return -1;
        return 0;
    }
    
    // 步驟函數
    inline float Step(float edge, float x) {
        return x < edge ? 0.0f : 1.0f;
    }
    
    // 模運算 (處理負數)
    inline float Mod(float x, float y) {
        return x - y * std::floor(x / y);
    }
    
    // 分數部分
    inline float Fract(float x) {
        return x - std::floor(x);
    }
    
    // 最接近的整數
    inline float Round(float x) {
        return std::round(x);
    }
    
    // 向上取整
    inline float Ceil(float x) {
        return std::ceil(x);
    }
    
    // 向下取整
    inline float Floor(float x) {
        return std::floor(x);
    }
    
    // 幂函數
    inline float Pow(float base, float exponent) {
        return std::pow(base, exponent);
    }
    
    // 平方根
    inline float Sqrt(float x) {
        return std::sqrt(x);
    }
    
    // 倒數平方根 (快速版本)
    inline float InvSqrt(float x) {
        return 1.0f / std::sqrt(x);
    }
    
    // 指數函數
    inline float Exp(float x) {
        return std::exp(x);
    }
    
    // 對數函數
    inline float Log(float x) {
        return std::log(x);
    }
    
    inline float Log2(float x) {
        return std::log2(x);
    }
    
    inline float Log10(float x) {
        return std::log10(x);
    }
    
    // 三角函數
    inline float Sin(float x) {
        return std::sin(x);
    }
    
    inline float Cos(float x) {
        return std::cos(x);
    }
    
    inline float Tan(float x) {
        return std::tan(x);
    }
    
    inline float Asin(float x) {
        return std::asin(x);
    }
    
    inline float Acos(float x) {
        return std::acos(x);
    }
    
    inline float Atan(float x) {
        return std::atan(x);
    }
    
    inline float Atan2(float y, float x) {
        return std::atan2(y, x);
    }
    
    // 雙曲函數
    inline float Sinh(float x) {
        return std::sinh(x);
    }
    
    inline float Cosh(float x) {
        return std::cosh(x);
    }
    
    inline float Tanh(float x) {
        return std::tanh(x);
    }
    
    // 隨機數生成
    inline float Random() {
        return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }
    
    inline float RandomRange(float min, float max) {
        return min + Random() * (max - min);
    }
    
    inline int RandomInt(int min, int max) {
        return min + rand() % (max - min + 1);
    }
    
    // 顏色相關函數
    inline Vector3 RGBToHSV(const Vector3& rgb) {
        Vector3 hsv;
        float max = Max(Max(rgb.x, rgb.y), rgb.z);
        float min = Min(Min(rgb.x, rgb.y), rgb.z);
        float delta = max - min;
        
        hsv.z = max; // Value
        
        if (delta < 0.0001f) {
            hsv.x = 0.0f; // Hue
            hsv.y = 0.0f; // Saturation
        } else {
            hsv.y = delta / max; // Saturation
            
            if (max == rgb.x) {
                hsv.x = (rgb.y - rgb.z) / delta + (rgb.y < rgb.z ? 6.0f : 0.0f);
            } else if (max == rgb.y) {
                hsv.x = (rgb.z - rgb.x) / delta + 2.0f;
            } else {
                hsv.x = (rgb.x - rgb.y) / delta + 4.0f;
            }
            
            hsv.x /= 6.0f; // Hue
        }
        
        return hsv;
    }
    
    inline Vector3 HSVToRGB(const Vector3& hsv) {
        Vector3 rgb;
        float h = hsv.x * 6.0f;
        float s = hsv.y;
        float v = hsv.z;
        
        float i = std::floor(h);
        float f = h - i;
        float p = v * (1.0f - s);
        float q = v * (1.0f - s * f);
        float t = v * (1.0f - s * (1.0f - f));
        
        switch (static_cast<int>(i) % 6) {
            case 0: rgb = Vector3(v, t, p); break;
            case 1: rgb = Vector3(q, v, p); break;
            case 2: rgb = Vector3(p, v, t); break;
            case 3: rgb = Vector3(p, q, v); break;
            case 4: rgb = Vector3(t, p, v); break;
            case 5: rgb = Vector3(v, p, q); break;
        }
        
        return rgb;
    }
    
    // 浮點數比較
    inline bool Approximately(float a, float b, float epsilon = MathConstants::EPSILON) {
        return Abs(a - b) < epsilon;
    }
    
    // 顏色亮度計算
    inline float Luminance(const Vector3& color) {
        return 0.2126f * color.x + 0.7152f * color.y + 0.0722f * color.z;
    }
    
    // 顏色混合
    inline Vector3 MixColors(const Vector3& color1, const Vector3& color2, float t) {
        return Lerp(color1, color2, t);
    }
    
    // 伽馬校正
    inline Vector3 GammaCorrect(const Vector3& color, float gamma) {
        return Vector3(
            std::pow(color.x, 1.0f / gamma),
            std::pow(color.y, 1.0f / gamma),
            std::pow(color.z, 1.0f / gamma)
        );
    }
    
} // namespace MathUtils

} // namespace Potato