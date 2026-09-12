#pragma once

#include "Core/CoreTypes.h"
#include "Vector3.h"
#include "Vector4.h"
#include <cmath>
#include <cstring>

namespace Potato {

/**
 * 4x4 矩陣類
 * 主要用於3D變換和投影
 * 使用列主序存儲 (與OpenGL/Vulkan兼容)
 */
class Matrix4 {
public:
    float m[16]; // 列主序存儲
    
    // 構造函數
    Matrix4() {
        SetIdentity();
    }
    
    explicit Matrix4(float diagonal) {
        SetIdentity();
        m[0] = m[5] = m[10] = m[15] = diagonal;
    }
    
    Matrix4(const float* data) {
        std::memcpy(m, data, sizeof(m));
    }
    
    // 靜態工廠方法
    static Matrix4 Identity() {
        Matrix4 mat;
        mat.SetIdentity();
        return mat;
    }
    
    static Matrix4 Zero() {
        Matrix4 mat;
        mat.SetZero();
        return mat;
    }
    
    // 設置為單位矩陣
    void SetIdentity() {
        std::memset(m, 0, sizeof(m));
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }
    
    // 設置為零矩陣
    void SetZero() {
        std::memset(m, 0, sizeof(m));
    }
    
    // 基本運算符
    Matrix4 operator+(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 16; i++) {
            result.m[i] = m[i] + other.m[i];
        }
        return result;
    }
    
    Matrix4 operator-(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 16; i++) {
            result.m[i] = m[i] - other.m[i];
        }
        return result;
    }
    
    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;
        for (int col = 0; col < 4; col++) {
            for (int row = 0; row < 4; row++) {
                float sum = 0.0f;
                for (int k = 0; k < 4; k++) {
                    sum += m[k * 4 + row] * other.m[col * 4 + k];
                }
                result.m[col * 4 + row] = sum;
            }
        }
        return result;
    }
    
    Matrix4 operator*(float scalar) const {
        Matrix4 result;
        for (int i = 0; i < 16; i++) {
            result.m[i] = m[i] * scalar;
        }
        return result;
    }
    
    Vector4 operator*(const Vector4& vec) const {
        return Vector4(
            m[0] * vec.x + m[4] * vec.y + m[8] * vec.z + m[12] * vec.w,
            m[1] * vec.x + m[5] * vec.y + m[9] * vec.z + m[13] * vec.w,
            m[2] * vec.x + m[6] * vec.y + m[10] * vec.z + m[14] * vec.w,
            m[3] * vec.x + m[7] * vec.y + m[11] * vec.z + m[15] * vec.w
        );
    }
    
    Vector3 TransformPoint(const Vector3& point) const {
        Vector4 result = *this * Vector4(point, 1.0f);
        return result.ToVector3();
    }
    
    Vector3 TransformVector(const Vector3& vec) const {
        Vector4 result = *this * Vector4(vec, 0.0f);
        return result.ToVector3();
    }
    
    // 複合賦值運算符
    Matrix4& operator+=(const Matrix4& other) {
        for (int i = 0; i < 16; i++) {
            m[i] += other.m[i];
        }
        return *this;
    }
    
    Matrix4& operator-=(const Matrix4& other) {
        for (int i = 0; i < 16; i++) {
            m[i] -= other.m[i];
        }
        return *this;
    }
    
    Matrix4& operator*=(const Matrix4& other) {
        *this = *this * other;
        return *this;
    }
    
    Matrix4& operator*=(float scalar) {
        for (int i = 0; i < 16; i++) {
            m[i] *= scalar;
        }
        return *this;
    }
    
    // 比較運算符
    bool operator==(const Matrix4& other) const {
        for (int i = 0; i < 16; i++) {
            if (m[i] != other.m[i]) return false;
        }
        return true;
    }
    
    bool operator!=(const Matrix4& other) const {
        return !(*this == other);
    }
    
    // 轉置
    Matrix4 Transposed() const {
        Matrix4 result;
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                result.m[col * 4 + row] = m[row * 4 + col];
            }
        }
        return result;
    }
    
    void Transpose() {
        *this = Transposed();
    }
    
    // 逆矩陣 (簡化版本，僅支持特定類型)
    Matrix4 Inverted() const {
        Matrix4 inv;
        float det;
        
        // 使用標準4x4矩陣求逆算法
        inv.m[0] = m[5]  * m[10] * m[15] - m[5]  * m[11] * m[14] - m[9]  * m[6]  * m[15] + m[9]  * m[7]  * m[14] + m[13] * m[6]  * m[11] - m[13] * m[7]  * m[10];
        inv.m[4] = -m[4]  * m[10] * m[15] + m[4]  * m[11] * m[14] + m[8]  * m[6]  * m[15] - m[8]  * m[7]  * m[14] - m[12] * m[6]  * m[11] + m[12] * m[7]  * m[10];
        inv.m[8] = m[4]  * m[9] * m[15] - m[4]  * m[11] * m[13] - m[8]  * m[5] * m[15] + m[8]  * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
        inv.m[12] = -m[4]  * m[9] * m[14] + m[4]  * m[10] * m[13] + m[8]  * m[5] * m[14] - m[8]  * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
        inv.m[1] = -m[1]  * m[10] * m[15] + m[1]  * m[11] * m[14] + m[9]  * m[2] * m[15] - m[9]  * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
        inv.m[5] = m[0]  * m[10] * m[15] - m[0]  * m[11] * m[14] - m[8]  * m[2] * m[15] + m[8]  * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
        inv.m[9] = -m[0]  * m[9] * m[15] + m[0]  * m[11] * m[13] + m[8]  * m[1] * m[15] - m[8]  * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
        inv.m[13] = m[0]  * m[9] * m[14] - m[0]  * m[10] * m[13] - m[8]  * m[1] * m[14] + m[8]  * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
        inv.m[2] = m[1]  * m[6] * m[15] - m[1]  * m[7] * m[14] - m[5]  * m[2] * m[15] + m[5]  * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
        inv.m[6] = -m[0]  * m[6] * m[15] + m[0]  * m[7] * m[14] + m[4]  * m[2] * m[15] - m[4]  * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
        inv.m[10] = m[0]  * m[5] * m[15] - m[0]  * m[7] * m[13] - m[4]  * m[1] * m[15] + m[4]  * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
        inv.m[14] = -m[0]  * m[5] * m[14] + m[0]  * m[6] * m[13] + m[4]  * m[1] * m[14] - m[4]  * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
        inv.m[3] = -m[1]  * m[6] * m[11] + m[1]  * m[7] * m[10] + m[5]  * m[2] * m[11] - m[5]  * m[3] * m[10] - m[9]  * m[2] * m[7] + m[9]  * m[3] * m[6];
        inv.m[7] = m[0]  * m[6] * m[11] - m[0]  * m[7] * m[10] - m[4]  * m[2] * m[11] + m[4]  * m[3] * m[10] + m[8]  * m[2] * m[7] - m[8]  * m[3] * m[6];
        inv.m[11] = -m[0]  * m[5] * m[11] + m[0]  * m[7] * m[9] + m[4]  * m[1] * m[11] - m[4]  * m[3] * m[9] - m[8]  * m[1] * m[7] + m[8]  * m[3] * m[5];
        inv.m[15] = m[0]  * m[5] * m[10] - m[0]  * m[6] * m[9] - m[4]  * m[1] * m[10] + m[4]  * m[2] * m[9] + m[8]  * m[1] * m[6] - m[8]  * m[2] * m[5];
        
        det = m[0] * inv.m[0] + m[1] * inv.m[4] + m[2] * inv.m[8] + m[3] * inv.m[12];
        
        if (det == 0.0f) {
            return Matrix4::Identity(); // 無法求逆，返回單位矩陣
        }
        
        det = 1.0f / det;
        
        for (int i = 0; i < 16; i++) {
            inv.m[i] *= det;
        }
        
        return inv;
    }
    
    void Invert() {
        *this = Inverted();
    }
    
    // 變換矩陣創建
    static Matrix4 Translation(const Vector3& translation) {
        Matrix4 mat = Matrix4::Identity();
        mat.m[12] = translation.x;
        mat.m[13] = translation.y;
        mat.m[14] = translation.z;
        return mat;
    }
    
    static Matrix4 RotationX(float angle) {
        Matrix4 mat = Matrix4::Identity();
        float c = std::cos(angle);
        float s = std::sin(angle);
        mat.m[5] = c;
        mat.m[6] = s;
        mat.m[9] = -s;
        mat.m[10] = c;
        return mat;
    }
    
    static Matrix4 RotationY(float angle) {
        Matrix4 mat = Matrix4::Identity();
        float c = std::cos(angle);
        float s = std::sin(angle);
        mat.m[0] = c;
        mat.m[2] = -s;
        mat.m[8] = s;
        mat.m[10] = c;
        return mat;
    }
    
    static Matrix4 RotationZ(float angle) {
        Matrix4 mat = Matrix4::Identity();
        float c = std::cos(angle);
        float s = std::sin(angle);
        mat.m[0] = c;
        mat.m[1] = s;
        mat.m[4] = -s;
        mat.m[5] = c;
        return mat;
    }
    
    static Matrix4 Rotation(const Vector3& axis, float angle) {
        Matrix4 mat = Matrix4::Identity();
        Vector3 a = axis.Normalized();
        float c = std::cos(angle);
        float s = std::sin(angle);
        float t = 1.0f - c;
        
        mat.m[0] = t * a.x * a.x + c;
        mat.m[1] = t * a.x * a.y + s * a.z;
        mat.m[2] = t * a.x * a.z - s * a.y;
        mat.m[4] = t * a.x * a.y - s * a.z;
        mat.m[5] = t * a.y * a.y + c;
        mat.m[6] = t * a.y * a.z + s * a.x;
        mat.m[8] = t * a.x * a.z + s * a.y;
        mat.m[9] = t * a.y * a.z - s * a.x;
        mat.m[10] = t * a.z * a.z + c;
        
        return mat;
    }
    
    static Matrix4 Scale(const Vector3& scale) {
        Matrix4 mat = Matrix4::Identity();
        mat.m[0] = scale.x;
        mat.m[5] = scale.y;
        mat.m[10] = scale.z;
        return mat;
    }
    
    static Matrix4 Scale(float uniformScale) {
        return Scale(Vector3(uniformScale));
    }
    
    // 投影矩陣
    static Matrix4 Perspective(float fov, float aspect, float nearPlane, float farPlane) {
        Matrix4 mat = Matrix4::Zero();
        float tanHalfFov = std::tan(fov / 2.0f);
        
        mat.m[0] = 1.0f / (aspect * tanHalfFov);
        mat.m[5] = 1.0f / tanHalfFov;
        mat.m[10] = -(farPlane + nearPlane) / (farPlane - nearPlane);
        mat.m[11] = -1.0f;
        mat.m[14] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
        
        return mat;
    }
    
    static Matrix4 Orthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
        Matrix4 mat = Matrix4::Identity();
        
        mat.m[0] = 2.0f / (right - left);
        mat.m[5] = 2.0f / (top - bottom);
        mat.m[10] = -2.0f / (farPlane - nearPlane);
        mat.m[12] = -(right + left) / (right - left);
        mat.m[13] = -(top + bottom) / (top - bottom);
        mat.m[14] = -(farPlane + nearPlane) / (farPlane - nearPlane);
        
        return mat;
    }
    
    // 視圖矩陣
    static Matrix4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up) {
        Vector3 f = (center - eye).Normalized();
        Vector3 s = f.Cross(up).Normalized();
        Vector3 u = s.Cross(f);
        
        Matrix4 mat = Matrix4::Identity();
        
        mat.m[0] = s.x;
        mat.m[4] = s.y;
        mat.m[8] = s.z;
        mat.m[1] = u.x;
        mat.m[5] = u.y;
        mat.m[9] = u.z;
        mat.m[2] = -f.x;
        mat.m[6] = -f.y;
        mat.m[10] = -f.z;
        mat.m[12] = -s.Dot(eye);
        mat.m[13] = -u.Dot(eye);
        mat.m[14] = f.Dot(eye);
        
        return mat;
    }
    
    // 數組訪問
    float& operator[](int index) {
        return m[index];
    }
    
    const float& operator[](int index) const {
        return m[index];
    }
    
    // 指針轉換
    float* Data() { return m; }
    const float* Data() const { return m; }
};

// 標量 * 矩陣
inline Matrix4 operator*(float scalar, const Matrix4& mat) {
    return mat * scalar;
}

} // namespace Potato