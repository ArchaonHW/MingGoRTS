#pragma once

#ifndef POTATO_VECTOR4_H
#define POTATO_VECTOR4_H

#include "Core/CoreTypes.h"
#include "MathUtils/Vector3.h"
#include <cmath>
#include <algorithm>

namespace Potato {

/**
 * 4D 向量類
 * 主要用於齊次坐標和顏色表示
 */
class Vector4 {
public:
    float x, y, z, w;
    
    // 構造函數
    Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    explicit Vector4(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
    Vector4(const Vector3& vec, float w) : x(vec.x), y(vec.y), z(vec.z), w(w) {}
    Vector4(const Vector2& vec, float z, float w) : x(vec.x), y(vec.y), z(z), w(w) {}
    
    // 拷貝構造
    Vector4(const Vector4& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}
    
    // 靜態工廠方法
    static Vector4 Zero() { return Vector4(0.0f, 0.0f, 0.0f, 0.0f); }
    static Vector4 One() { return Vector4(1.0f, 1.0f, 1.0f, 1.0f); }
    
    // 顏色工廠方法
    static Vector4 Black() { return Vector4(0.0f, 0.0f, 0.0f, 1.0f); }
    static Vector4 White() { return Vector4(1.0f, 1.0f, 1.0f, 1.0f); }
    static Vector4 Red() { return Vector4(1.0f, 0.0f, 0.0f, 1.0f); }
    static Vector4 Green() { return Vector4(0.0f, 1.0f, 0.0f, 1.0f); }
    static Vector4 Blue() { return Vector4(0.0f, 0.0f, 1.0f, 1.0f); }
    static Vector4 Yellow() { return Vector4(1.0f, 1.0f, 0.0f, 1.0f); }
    static Vector4 Cyan() { return Vector4(0.0f, 1.0f, 1.0f, 1.0f); }
    static Vector4 Magenta() { return Vector4(1.0f, 0.0f, 1.0f, 1.0f); }
    
    // 基本運算符
    Vector4 operator+(const Vector4& other) const {
        return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
    }
    
    Vector4 operator-(const Vector4& other) const {
        return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
    }
    
    Vector4 operator*(float scalar) const {
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
    }
    
    Vector4 operator/(float scalar) const {
        return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
    }
    
    Vector4 operator-() const {
        return Vector4(-x, -y, -z, -w);
    }
    
    // 複合賦值運算符
    Vector4& operator+=(const Vector4& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }
    
    Vector4& operator-=(const Vector4& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }
    
    Vector4& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }
    
    Vector4& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }
    
    // 比較運算符
    bool operator==(const Vector4& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }
    
    bool operator!=(const Vector4& other) const {
        return !(*this == other);
    }
    
    // 向量運算
    float Length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }
    
    float LengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }
    
    Vector4 Normalized() const {
        float len = Length();
        if (len > 0.0001f) {
            return Vector4(x / len, y / len, z / len, w / len);
        }
        return Vector4::Zero();
    }
    
    void Normalize() {
        float len = Length();
        if (len > 0.0001f) {
            x /= len;
            y /= len;
            z /= len;
            w /= len;
        }
    }
    
    float Dot(const Vector4& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }
    
    // 插值
    static Vector4 Lerp(const Vector4& a, const Vector4& b, float t) {
        return a + (b - a) * t;
    }
    
    // 最小/最大
    static Vector4 Min(const Vector4& a, const Vector4& b) {
        return Vector4(std::min(a.x, b.x), std::min(a.y, b.y), 
                      std::min(a.z, b.z), std::min(a.w, b.w));
    }
    
    static Vector4 Max(const Vector4& a, const Vector4& b) {
        return Vector4(std::max(a.x, b.x), std::max(a.y, b.y), 
                      std::max(a.z, b.z), std::max(a.w, b.w));
    }
    
    // 數組訪問
    float& operator[](int index) {
        return (&x)[index];
    }
    
    const float& operator[](int index) const {
        return (&x)[index];
    }
    
    // 指針轉換
    float* Data() { return &x; }
    const float* Data() const { return &x; }
    
    // 轉換為 Vector3 (丟棄 w 分量)
    Vector3 ToVector3() const {
        return Vector3(x, y, z);
    }
    
    // 轉換為 Vector2 (丟棄 z, w 分量)
    Vector2 ToVector2() const {
        return Vector2(x, y);
    }
    
    // RGBA 顏色操作
    static Vector4 FromRGBA(uint8 r, uint8 g, uint8 b, uint8 a = 255) {
        return Vector4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }
    
    void ToRGBA(uint8& r, uint8& g, uint8& b, uint8& a) const {
        r = static_cast<uint8>(std::clamp(x * 255.0f, 0.0f, 255.0f));
        g = static_cast<uint8>(std::clamp(y * 255.0f, 0.0f, 255.0f));
        b = static_cast<uint8>(std::clamp(z * 255.0f, 0.0f, 255.0f));
        a = static_cast<uint8>(std::clamp(w * 255.0f, 0.0f, 255.0f));
    }
};

// 標量 * 向量
inline Vector4 operator*(float scalar, const Vector4& vec) {
    return vec * scalar;
}

// 逐元素乘法
inline Vector4 operator*(const Vector4& a, const Vector4& b) {
    return Vector4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

// 逐元素除法
inline Vector4 operator/(const Vector4& a, const Vector4& b) {
    return Vector4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

} // namespace Potato

#endif // POTATO_VECTOR4_H