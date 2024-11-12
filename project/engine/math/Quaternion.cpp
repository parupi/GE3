#include "Quaternion.h"
#include <cmath>    // sqrtf
#include <stdexcept>
#include <imgui.h>
#include <Matrix4x4.h>
#include <Vector3.h>
#include <function.h>
#include <numbers>
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

Quaternion CreateCameraRotationQuaternion(const Vector3& eulerAngles) {
    // 各軸に対する回転クォータニオンを作成
    Quaternion qPitch = MakeRotateAxisAngleQuaternion(Vector3(1.0f, 0.0f, 0.0f), eulerAngles.x); // X軸 (ピッチ)
    Quaternion qYaw = MakeRotateAxisAngleQuaternion(Vector3(0.0f, 1.0f, 0.0f), eulerAngles.y); // Y軸 (ヨー)
    Quaternion qRoll = MakeRotateAxisAngleQuaternion(Vector3(0.0f, 0.0f, 1.0f), eulerAngles.z); // Z軸 (ロール)

    // 合成して最終的な回転クォータニオンを取得
    // 順序は Yaw -> Pitch -> Roll とする（カメラの回転に一般的な順序）
    return qYaw * qPitch * qRoll;
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
    return { q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w };
}

Quaternion QuaternionFromAxes(const Vector3& x, const Vector3& y, const Vector3& z)
{
    Quaternion q;
    float trace = x.x + y.y + z.z;
    if (trace > 0.0f) {
        float s = sqrtf(trace + 1.0f) * 2.0f;
        q.w = 0.25f * s;
        q.x = (y.z - z.y) / s;
        q.y = (z.x - x.z) / s;
        q.z = (x.y - y.x) / s;
    }
    else if ((x.x > y.y) && (x.x > z.z)) {
        float s = sqrtf(1.0f + x.x - y.y - z.z) * 2.0f;
        q.w = (y.z - z.y) / s;
        q.x = 0.25f * s;
        q.y = (y.x + x.y) / s;
        q.z = (z.x + x.z) / s;
    }
    else if (y.y > z.z) {
        float s = sqrtf(1.0f + y.y - x.x - z.z) * 2.0f;
        q.w = (z.x - x.z) / s;
        q.x = (y.x + x.y) / s;
        q.y = 0.25f * s;
        q.z = (z.y + y.z) / s;
    }
    else {
        float s = sqrtf(1.0f + z.z - x.x - y.y) * 2.0f;
        q.w = (x.y - y.x) / s;
        q.x = (z.x + x.z) / s;
        q.y = (z.y + y.z) / s;
        q.z = 0.25f * s;
    }
    return q;
}

Quaternion SetFromTo(const Vector3& from, const Vector3& to)
{
    Vector3 f = Normalize(from); // 正規化したfromベクトル
    Vector3 t = Normalize(to);   // 正規化したtoベクトル

    Vector3 cross = Cross(f, t);    // fromとtoのクロス積
    float dot = Dot(f, t);          // fromとtoの内積

    // 回転角をクォータニオンに変換
    float w = sqrt((1.0f + dot) * 0.5f); // 回転のスカラー成分
    float s = 0.5f / w;

    float x = cross.x * s;
    float y = cross.y * s;
    float z = cross.z * s;

    return { x, y, z, w };
}

Vector3 ToEulerAngles(const Quaternion& quat)
{
    Vector3 angles;

    // ピッチ（X軸）
    float sinPitch = 2.0f * (quat.w * quat.x + quat.y * quat.z);
    float cosPitch = 1.0f - 2.0f * (quat.x * quat.x + quat.y * quat.y);
    angles.x = atan2(sinPitch, cosPitch);

    // ヨー（Y軸）
    float sinYaw = 2.0f * (quat.w * quat.y - quat.z * quat.x);
    angles.y = fabs(sinYaw) >= 1.0f ? copysign(std::numbers::pi_v<float> / 2, sinYaw) : asin(sinYaw); // 特別なケース

    // ロール（Z軸）
    float sinRoll = 2.0f * (quat.w * quat.z + quat.x * quat.y);
    float cosRoll = 1.0f - 2.0f * (quat.y * quat.y + quat.z * quat.z);
    angles.z = atan2(sinRoll, cosRoll);

    return angles;
}


// ImGuiを使ったクォータニオンの描画
void PrintOnImGui(const Quaternion& q, const char* label) {
    ImGui::Begin("Quaternion");
    ImGui::Text("%s: (x: %.2f, y: %.2f, z: %.2f, w: %.2f)", label, q.x, q.y, q.z, q.w);
    ImGui::End();
}
