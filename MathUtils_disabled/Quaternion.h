#pragma once

#ifndef POTATO_QUATERNION_H
#define POTATO_QUATERNION_H

#include "Core/CoreTypes.h"
#include "MathUtils/Vector3.h"
#include "MathUtils/Matrix4.h"
#include <cmath>

namespace Potato {

/**
 * 四元數類
 * 主要用於3D旋轉表示和插值
 * 形式: w + xi + yj + zk
 */
class Quaternion {
public:
    float x, y, z, w;
    
    // 構造函數
    Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    explicit Quaternion(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
    
    // 拷貝構造
    Quaternion(const Quaternion& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}
    
    // 靜態工廠方法
    static Quaternion Identity() {
        return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    static Quaternion Zero() {
        return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
    }
    
    // 從旋轉軸和角度創建
    static Quaternion FromAxisAngle(const Vector3& axis, float angle) {
        Quaternion result;
        float halfAngle = angle * 0.5f;
        float sinHalf = std::sin(halfAngle);
        
        Vector3 normalizedAxis = axis.Normalized();
        
        result.x = normalizedAxis.x * sinHalf;
        result.y = normalizedAxis.y * sinHalf;
        result.z = normalizedAxis.z * sinHalf;
        result.w = std::cos(halfAngle);
        
        return result;
    }
    
    // 從歐拉角創建 (YXZ順序)
    static Quaternion FromEuler(const Vector3& euler) {
        return FromEuler(euler.x, euler.y, euler.z);
    }
    
    static Quaternion FromEuler(float pitch, float yaw, float roll) {
        Quaternion qX = Quaternion::FromAxisAngle(Vector3::UnitX(), pitch);
        Quaternion qY = Quaternion::FromAxisAngle(Vector3::UnitY(), yaw);
        Quaternion qZ = Quaternion::FromAxisAngle(Vector3::UnitZ(), roll);
        
        return qY * qX * qZ; // YXZ順序
    }
    
    // 從旋轉矩陣創建
    static Quaternion FromMatrix(const Matrix4& matrix) {
        Quaternion result;
        
        float trace = matrix.m[0] + matrix.m[5] + matrix.m[10];
        
        if (trace > 0.0f) {
            float s = std::sqrt(trace + 1.0f) * 2.0f;
            result.w = 0.25f * s;
            result.x = (matrix.m[9] - matrix.m[6]) / s;
            result.y = (matrix.m[2] - matrix.m[8]) / s;
            result.z = (matrix.m[4] - matrix.m[1]) / s;
        } else if ((matrix.m[0] > matrix.m[5]) && (matrix.m[0] > matrix.m[10])) {
            float s = std::sqrt(1.0f + matrix.m[0] - matrix.m[5] - matrix.m[10]) * 2.0f;
            result.w = (matrix.m[9] - matrix.m[6]) / s;
            result.x = 0.25f * s;
            result.y = (matrix.m[1] + matrix.m[4]) / s;
            result.z = (matrix.m[2] + matrix.m[8]) / s;
        } else if (matrix.m[5] > matrix.m[10]) {
            float s = std::sqrt(1.0f + matrix.m[5] - matrix.m[0] - matrix.m[10]) * 2.0f;
            result.w = (matrix.m[2] - matrix.m[8]) / s;
            result.x = (matrix.m[1] + matrix.m[4]) / s;
            result.y = 0.25f * s;
            result.z = (matrix.m[6] + matrix.m[9]) / s;
        } else {
            float s = std::sqrt(1.0f + matrix.m[10] - matrix.m[0] - matrix.m[5]) * 2.0f;
            result.w = (matrix.m[4] - matrix.m[1]) / s;
            result.x = (matrix.m[2] + matrix.m[8]) / s;
            result.y = (matrix.m[6] + matrix.m[9]) / s;
            result.z = 0.25f * s;
        }
        
        return result.Normalized();
    }
    
    // 基本運算符
    Quaternion operator+(const Quaternion& other) const {
        return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
    }
    
    Quaternion operator-(const Quaternion& other) const {
        return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
    }
    
    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }
    
    Quaternion operator*(float scalar) const {
        return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
    }
    
    Quaternion operator-() const {
        return Quaternion(-x, -y, -z, -w);
    }
    
    // 複合賦值運算符
    Quaternion& operator+=(const Quaternion& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }
    
    Quaternion& operator-=(const Quaternion& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }
    
    Quaternion& operator*=(const Quaternion& other) {
        *this = *this * other;
        return *this;
    }
    
    Quaternion& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }
    
    // 比較運算符
    bool operator==(const Quaternion& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }
    
    bool operator!=(const Quaternion& other) const {
        return !(*this == other);
    }
    
    // 四元數運算
    float Length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }
    
    float LengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }
    
    Quaternion Normalized() const {
        float len = Length();
        if (len > 0.0001f) {
            return Quaternion(x / len, y / len, z / len, w / len);
        }
        return Quaternion::Identity();
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
    
    // 共軛四元數
    Quaternion Conjugate() const {
        return Quaternion(-x, -y, -z, w);
    }
    
    // 逆四元數
    Quaternion Inverse() const {
        float lenSq = LengthSquared();
        if (lenSq > 0.0001f) {
            return Conjugate() * (1.0f / lenSq);
        }
        return Quaternion::Identity();
    }
    
    // 點積
    float Dot(const Quaternion& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }
    
    // 球面線性插值
    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t) {
        Quaternion result;
        
        float dot = a.Dot(b);
        
        // 如果點積為負，取反b以確保最短路徑
        Quaternion tempB = b;
        if (dot < 0.0f) {
            tempB = -b;
            dot = -dot;
        }
        
        // 如果四元數非常接近，使用線性插值
        if (dot > 0.9995f) {
            result = a + (tempB - a) * t;
            result.Normalize();
            return result;
        }
        
        float angle = std::acos(dot);
        float sinAngle = std::sin(angle);
        float invSinAngle = 1.0f / sinAngle;
        
        float scaleA = std::sin((1.0f - t) * angle) * invSinAngle;
        float scaleB = std::sin(t * angle) * invSinAngle;
        
        result = a * scaleA + tempB * scaleB;
        return result.Normalized();
    }
    
    // 線性插值 (然後歸一化)
    static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t) {
        Quaternion result = a + (b - a) * t;
        result.Normalize();
        return result;
    }
    
    // 旋轉向量
    Vector3 RotateVector(const Vector3& vec) const {
        // 使用四元數乘法旋轉向量: q * v * q^-1
        Quaternion vecQuat(vec.x, vec.y, vec.z, 0.0f);
        Quaternion result = *this * vecQuat * Conjugate();
        return Vector3(result.x, result.y, result.z);
    }
    
    // 轉換為旋轉矩陣
    Matrix4 ToMatrix() const {
        Matrix4 mat = Matrix4::Identity();
        
        float xx = x * x;
        float yy = y * y;
        float zz = z * z;
        float xy = x * y;
        float xz = x * z;
        float yz = y * z;
        float wx = w * x;
        float wy = w * y;
        float wz = w * z;
        
        mat.m[0] = 1.0f - 2.0f * (yy + zz);
        mat.m[1] = 2.0f * (xy + wz);
        mat.m[2] = 2.0f * (xz - wy);
        
        mat.m[4] = 2.0f * (xy - wz);
        mat.m[5] = 1.0f - 2.0f * (xx + zz);
        mat.m[6] = 2.0f * (yz + wx);
        
        mat.m[8] = 2.0f * (xz + wy);
        mat.m[9] = 2.0f * (yz - wx);
        mat.m[10] = 1.0f - 2.0f * (xx + yy);
        
        return mat;
    }
    
    // 轉換為軸角表示
    void ToAxisAngle(Vector3& axis, float& angle) const {
        float len = std::sqrt(x * x + y * y + z * z);
        
        if (len > 0.0001f) {
            axis = Vector3(x / len, y / len, z / len);
            angle = 2.0f * std::acos(std::clamp(w, -1.0f, 1.0f));
        } else {
            axis = Vector3::UnitX();
            angle = 0.0f;
        }
    }
    
    // 轉換為歐拉角
    Vector3 ToEuler() const {
        Vector3 euler;
        
        // 計算pitch (X軸旋轉)
        float sinPitch = -2.0f * (y * z + w * x);
        float cosPitch = w * w - x * x - y * y + z * z;
        
        if (std::abs(sinPitch) >= 1.0f) {
            euler.x = std::copysign(3.14159f / 2.0f, sinPitch); // 90度
        } else {
            euler.x = std::atan2(sinPitch, cosPitch);
        }
        
        // 計算yaw (Y軸旋轉)
        float sinYaw = 2.0f * (x * z - w * y);
        if (std::abs(sinYaw) >= 1.0f) {
            euler.y = std::copysign(3.14159f / 2.0f, sinYaw); // 90度
        } else {
            euler.y = std::asin(sinYaw);
        }
        
        // 計算roll (Z軸旋轉)
        float sinRoll = 2.0f * (x * y + w * z);
        float cosRoll = w * w + x * x - y * y - z * z;
        euler.z = std::atan2(sinRoll, cosRoll);
        
        return euler;
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

// 標量 * 四元數
inline Quaternion operator*(float scalar, const Quaternion& quat) {
    return quat * scalar;
}

} // namespace Potato

#endif // POTATO_QUATERNION_H