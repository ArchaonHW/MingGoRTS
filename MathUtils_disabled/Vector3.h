#pragma once

#ifndef POTATO_VECTOR3_H
#define POTATO_VECTOR3_H

#include "Core/CoreTypes.h"
#include "MathUtils/Vector2.h"
#include <cmath>
#include <algorithm>

namespace Potato {

/**
 * 3D 向量類
 * 提供基本的3D向量運算
 */
class Vector3 {
public:
    float x, y, z;
    
    // 構造函數
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    explicit Vector3(float scalar) : x(scalar), y(scalar), z(scalar) {}
    Vector3(const Vector2& vec, float z) : x(vec.x), y(vec.y), z(z) {}
    
    // 拷貝構造
    Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}
    
    // 靜態工廠方法
    static Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
    static Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }
    static Vector3 UnitX() { return Vector3(1.0f, 0.0f, 0.0f); }
    static Vector3 UnitY() { return Vector3(0.0f, 1.0f, 0.0f); }
    static Vector3 UnitZ() { return Vector3(0.0f, 0.0f, 1.0f); }
    static Vector3 Up() { return Vector3(0.0f, 1.0f, 0.0f); }
    static Vector3 Down() { return Vector3(0.0f, -1.0f, 0.0f); }
    static Vector3 Left() { return Vector3(-1.0f, 0.0f, 0.0f); }
    static Vector3 Right() { return Vector3(1.0f, 0.0f, 0.0f); }
    static Vector3 Forward() { return Vector3(0.0f, 0.0f, 1.0f); }
    static Vector3 Backward() { return Vector3(0.0f, 0.0f, -1.0f); }
    
    // 基本運算符
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    
    Vector3 operator/(float scalar) const {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }
    
    Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }
    
    // 複合賦值運算符
    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    
    Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    
    Vector3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    
    Vector3& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
    
    // 比較運算符
    bool operator==(const Vector3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    
    bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }
    
    // 向量運算
    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    
    float LengthSquared() const {
        return x * x + y * y + z * z;
    }
    
    Vector3 Normalized() const {
        float len = Length();
        if (len > 0.0001f) {
            return Vector3(x / len, y / len, z / len);
        }
        return Vector3::Zero();
    }
    
    void Normalize() {
        float len = Length();
        if (len > 0.0001f) {
            x /= len;
            y /= len;
            z /= len;
        }
    }
    
    float Dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    // 3D 叉積
    Vector3 Cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    
    // 距離計算
    static float Distance(const Vector3& a, const Vector3& b) {
        return (a - b).Length();
    }
    
    static float DistanceSquared(const Vector3& a, const Vector3& b) {
        return (a - b).LengthSquared();
    }
    
    // 插值
    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
        return a + (b - a) * t;
    }
    
    // 最小/最大
    static Vector3 Min(const Vector3& a, const Vector3& b) {
        return Vector3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
    }
    
    static Vector3 Max(const Vector3& a, const Vector3& b) {
        return Vector3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
    }
    
    // 反射
    Vector3 Reflect(const Vector3& normal) const {
        return *this - normal * (2.0f * Dot(normal));
    }
    
    // 投影
    Vector3 Project(const Vector3& onto) const {
        return onto * (Dot(onto) / onto.LengthSquared());
    }
    
    // 角度計算
    static float AngleBetween(const Vector3& a, const Vector3& b) {
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
    
    // 轉換為 Vector2
    Vector2 ToVector2() const {
        return Vector2(x, y);
    }
};

// 標量 * 向量
inline Vector3 operator*(float scalar, const Vector3& vec) {
    return vec * scalar;
}

// 逐元素乘法
inline Vector3 operator*(const Vector3& a, const Vector3& b) {
    return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}

// 逐元素除法
inline Vector3 operator/(const Vector3& a, const Vector3& b) {
    return Vector3(a.x / b.x, a.y / b.y, a.z / b.z);
}

} // namespace Potato

#endif // POTATO_VECTOR3_H