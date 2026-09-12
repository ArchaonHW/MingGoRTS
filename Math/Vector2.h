#pragma once

#include "Core/CoreTypes.h"
#include <cmath>
#include <algorithm>

namespace Potato {

/**
 * 2D 向量類
 * 提供基本的2D向量運算
 */
class Vector2 {
public:
    float x, y;
    
    // 構造函數
    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x, float y) : x(x), y(y) {}
    explicit Vector2(float scalar) : x(scalar), y(scalar) {}
    
    // 拷貝構造
    Vector2(const Vector2& other) : x(other.x), y(other.y) {}
    
    // 靜態工廠方法
    static Vector2 Zero() { return Vector2(0.0f, 0.0f); }
    static Vector2 One() { return Vector2(1.0f, 1.0f); }
    static Vector2 UnitX() { return Vector2(1.0f, 0.0f); }
    static Vector2 UnitY() { return Vector2(0.0f, 1.0f); }
    
    // 基本運算符
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }
    
    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }
    
    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }
    
    Vector2 operator/(float scalar) const {
        return Vector2(x / scalar, y / scalar);
    }
    
    Vector2 operator-() const {
        return Vector2(-x, -y);
    }
    
    // 複合賦值運算符
    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    Vector2& operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    
    Vector2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    
    Vector2& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }
    
    // 比較運算符
    bool operator==(const Vector2& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Vector2& other) const {
        return !(*this == other);
    }
    
    // 向量運算
    float Length() const {
        return std::sqrt(x * x + y * y);
    }
    
    float LengthSquared() const {
        return x * x + y * y;
    }
    
    Vector2 Normalized() const {
        float len = Length();
        if (len > 0.0001f) {
            return Vector2(x / len, y / len);
        }
        return Vector2::Zero();
    }
    
    void Normalize() {
        float len = Length();
        if (len > 0.0001f) {
            x /= len;
            y /= len;
        }
    }
    
    float Dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }
    
    // 2D 叉積 (返回標量)
    float Cross(const Vector2& other) const {
        return x * other.y - y * other.x;
    }
    
    // 距離計算
    static float Distance(const Vector2& a, const Vector2& b) {
        return (a - b).Length();
    }
    
    static float DistanceSquared(const Vector2& a, const Vector2& b) {
        return (a - b).LengthSquared();
    }
    
    // 插值
    static Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
        return a + (b - a) * t;
    }
    
    // 最小/最大
    static Vector2 Min(const Vector2& a, const Vector2& b) {
        return Vector2(std::min(a.x, b.x), std::min(a.y, b.y));
    }
    
    static Vector2 Max(const Vector2& a, const Vector2& b) {
        return Vector2(std::max(a.x, b.x), std::max(a.y, b.y));
    }
    
    // 反射
    Vector2 Reflect(const Vector2& normal) const {
        return *this - normal * (2.0f * Dot(normal));
    }
    
    // 投影
    Vector2 Project(const Vector2& onto) const {
        return onto * (Dot(onto) / onto.LengthSquared());
    }
    
    // 旋轉
    Vector2 Rotated(float angle) const {
        float cos = std::cos(angle);
        float sin = std::sin(angle);
        return Vector2(x * cos - y * sin, x * sin + y * cos);
    }
    
    // 角度計算
    float Angle() const {
        return std::atan2(y, x);
    }
    
    static float AngleBetween(const Vector2& a, const Vector2& b) {
        return std::acos(std::clamp(a.Dot(b) / (a.Length() * b.Length()), -1.0f, 1.0f));
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
};

// 標量 * 向量
inline Vector2 operator*(float scalar, const Vector2& vec) {
    return vec * scalar;
}

} // namespace Potato