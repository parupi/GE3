#include "Quaternion.h"
#include <cmath>    // sqrtf
#include <stdexcept>
#include <imgui.h>
#include <Matrix4x4.h>
#include <Vector3.h>
// コンストラクタ
Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

// クォータニオンの乗算
Quaternion Quaternion::operator*(const Quaternion& q) const {
    return Quaternion(
        w * q.x + x * q.w + y * q.z - z * q.y,
        w * q.y - x * q.z + y * q.w + z * q.x,
        w * q.z + x * q.y - y * q.x + z * q.w,
        w * q.w - x * q.x - y * q.y - z * q.z
    );
}

// 等価・非等価演算子
bool Quaternion::operator==(const Quaternion& q) const {
    return x == q.x && y == q.y && z == q.z && w == q.w;
}

bool Quaternion::operator!=(const Quaternion& q) const {
    return !(*this == q);
}

// 加算・減算演算子
Quaternion Quaternion::operator+(const Quaternion& q) const {
    return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
}

Quaternion Quaternion::operator-(const Quaternion& q) const {
    return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
}

Quaternion& Quaternion::operator+=(const Quaternion& q) {
    x += q.x;
    y += q.y;
    z += q.z;
    w += q.w;
    return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& q) {
    x -= q.x;
    y -= q.y;
    z -= q.z;
    w -= q.w;
    return *this;
}

// スカラー倍演算子
Quaternion Quaternion::operator*(float scalar) const {
    return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
}

Quaternion& Quaternion::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

// 単位クォータニオン（乗法の単位元）
Quaternion Identity() {
    return Quaternion(0, 0, 0, 1);
}

// クォータニオンの共役
Quaternion Conjugate(const Quaternion& q) {
    return Quaternion(-q.x, -q.y, -q.z, q.w);
}

// クォータニオンのノルム
float Norm(const Quaternion& q) {
    return sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

// クォータニオンの正規化
Quaternion Normalize(const Quaternion& q) {
    float norm = Norm(q);
    if (norm == 0.0f) {
        throw std::runtime_error("Cannot normalize a zero-norm quaternion.");
    }
    return Quaternion(q.x / norm, q.y / norm, q.z / norm, q.w / norm);
}

// クォータニオンの逆
Quaternion Inverse(const Quaternion& q) {
    float norm = Norm(q);
    if (norm == 0.0f) {
        throw std::runtime_error("Cannot invert a zero-norm quaternion.");
    }
    Quaternion conjugate = Conjugate(q);
    return Quaternion(conjugate.x / (norm * norm), conjugate.y / (norm * norm), conjugate.z / (norm * norm), conjugate.w / (norm * norm));
}

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
    Vector3 normalAxis = Normalize(axis);
    float halfAngle = angle * 0.5f;
    float sinHalfAngle = sinf(halfAngle);
    return Quaternion(
        normalAxis.x * sinHalfAngle,
        normalAxis.y * sinHalfAngle,
        normalAxis.z * sinHalfAngle,
        cosf(halfAngle)
    );
}

Quaternion Slerp(Quaternion q0, Quaternion q1, float t)
{
    float dot = Dot(q0, q1);
    if (dot < 0.0f) {
        q0 = { -q0.x, -q0.y, -q0.z, -q0.w }; // 反対方向に補間
        dot = -dot;
    }

    // なす角を求める
    float theta = std::acos(dot);
    float sinTheta = std::sin(theta);

    // 補間係数を求める
    if (sinTheta > 0.001f) { // 数値安定性のための閾値
        float scale0 = std::sin((1 - t) * theta) / sinTheta;
        float scale1 = std::sin(t * theta) / sinTheta;

        // 補間後のQuaternionを計算
        return q0 * scale0 + q1 * scale1;
    }
    else {
        // ほぼ同じ方向の場合、線形補間
        return q0 * (1 - t) + q1 * t;
    }
}

float Dot(const Quaternion& q0, const Quaternion& q1)
{
    return q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
}

// ImGuiを使ったクォータニオンの描画
void PrintOnImGui(const Quaternion& q, const char* label) {
    ImGui::Begin(label);
    ImGui::Text("%s: (x: %.2f, y: %.2f, z: %.2f, w: %.2f)", "Quaternion", q.x, q.y, q.z, q.w);
    ImGui::End();
}
