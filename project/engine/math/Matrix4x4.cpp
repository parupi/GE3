#include "Matrix4x4.h"
#include <imgui.h>
#include <Quaternion.h>
#include <Vector3.h>
#include <function.h>

// デフォルトコンストラクタ
Matrix4x4::Matrix4x4() : m{ {0.0f} } {}

// イニシャライザリストを受け取るコンストラクタ
Matrix4x4::Matrix4x4(std::initializer_list<float> list) {
    if (list.size() != 16) {
        throw std::invalid_argument("Matrix4x4 initializer list must contain exactly 16 elements.");
    }
    auto it = list.begin();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m[i][j] = *it++;
        }
    }
}

// 行列の加算
Matrix4x4 Matrix4x4::operator+(const Matrix4x4& other) const {
    Matrix4x4 result;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            result.m[i][j] = m[i][j] + other.m[i][j];
    return result;
}

// 行列の減算
Matrix4x4 Matrix4x4::operator-(const Matrix4x4& other) const {
    Matrix4x4 result;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            result.m[i][j] = m[i][j] - other.m[i][j];
    return result;
}

// 行列の乗算
Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const {
    Matrix4x4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                result.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
    }
    return result;
}

// スカラー倍
Matrix4x4 Matrix4x4::operator*(float scalar) const {
    Matrix4x4 result;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            result.m[i][j] = m[i][j] * scalar;
    return result;
}

// 加算代入
Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& other) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] += other.m[i][j];
    return *this;
}

// 減算代入
Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& other) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] -= other.m[i][j];
    return *this;
}

// 乗算代入
Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& other) {
    *this = *this * other;
    return *this;
}

// スカラー倍代入
Matrix4x4& Matrix4x4::operator*=(float scalar) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] *= scalar;
    return *this;
}

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion) {
    Matrix4x4 rotationMatrix;

    float x = quaternion.x;
    float y = quaternion.y;
    float z = quaternion.z;
    float w = quaternion.w;

    rotationMatrix.m[0][0] = w * w + x * x - y * y -z * z;
    rotationMatrix.m[0][1] = 2.0f * (x * y + w * z);
    rotationMatrix.m[0][2] = 2.0f * (x * z - w * y);
    rotationMatrix.m[0][3] = 0.0f;

    rotationMatrix.m[1][0] = 2.0f * (x * y - w * z);
    rotationMatrix.m[1][1] = w * w - x * x + y * y - z * z;
    rotationMatrix.m[1][2] = 2.0f * (y * z + w * x);
    rotationMatrix.m[1][3] = 0.0f;

    rotationMatrix.m[2][0] = 2.0f * (x * z + w * y);
    rotationMatrix.m[2][1] = 2.0f * (y * z - w * x);
    rotationMatrix.m[2][2] = w * w - x * x - y * y + z * z;
    rotationMatrix.m[2][3] = 0.0f;

    rotationMatrix.m[3][0] = 0.0f;
    rotationMatrix.m[3][1] = 0.0f;
    rotationMatrix.m[3][2] = 0.0f;
    rotationMatrix.m[3][3] = 1.0f;

    return rotationMatrix;
}

// 逆行列を計算する関数
Matrix4x4 Inverse(const Matrix4x4& m) {
    float A =
        m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
        m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] - m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
        m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +
        m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
        m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
        m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] + m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

    return {
        (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] -
         m.m[1][1] * m.m[2][3] * m.m[3][2]) /
            A,

        (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] +
         m.m[0][1] * m.m[2][3] * m.m[3][2]) /
            A,

        (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] -
         m.m[0][1] * m.m[1][3] * m.m[3][2]) /
            A,

        (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] +
         m.m[0][1] * m.m[1][3] * m.m[2][2]) /
            A,

        (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] +
         m.m[1][0] * m.m[2][3] * m.m[3][2]) /
            A,

        (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] -
         m.m[0][0] * m.m[2][3] * m.m[3][2]) /
            A,

        (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] +
         m.m[0][0] * m.m[1][3] * m.m[3][2]) /
            A,

        (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][3] -
         m.m[0][0] * m.m[1][3] * m.m[2][2]) /
            A,

        (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] -
         m.m[1][0] * m.m[2][3] * m.m[3][1]) /
            A,

        (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] +
         m.m[0][0] * m.m[2][3] * m.m[3][1]) /
            A,

        (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] -
         m.m[0][0] * m.m[1][3] * m.m[3][1]) /
            A,

        (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] +
         m.m[0][0] * m.m[1][3] * m.m[2][1]) /
            A,

        (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] +
         m.m[1][0] * m.m[2][2] * m.m[3][1]) /
            A,

        (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] -
         m.m[0][0] * m.m[2][2] * m.m[3][1]) /
            A,

        (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] +
         m.m[0][0] * m.m[1][2] * m.m[3][1]) /
            A,

        (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] -
         m.m[0][0] * m.m[1][2] * m.m[2][1]) /
            A

    };
}

// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m) {
    return {
        m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
        m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
        m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
        m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]
    };
}

// 単位行列
Matrix4x4 MakeIdentity4x4() {
    return { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
}

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {
    return { 1, 0, 0, 0, 0, cosf(radian), sinf(radian), 0, 0, -sinf(radian), cosf(radian), 0, 0, 0, 0, 1 };
}

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
    return { cosf(radian), 0, -sinf(radian), 0, 0, 1, 0, 0, sinf(radian), 0, cosf(radian), 0, 0, 0, 0, 1 };
}

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
    return { cosf(radian), sinf(radian), 0, 0, -sinf(radian), cosf(radian), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
}

Matrix4x4 CreateViewMatrix(const Vector3& cameraPosition, const Vector3& cameraRotation)
{
    // カメラのオイラー角（ピッチ、ヨー、ロール）をクォータニオンに変換
    Quaternion rotationQuat = CreateCameraRotationQuaternion(cameraRotation);

    // クォータニオンを使って、カメラの前方向、上方向、右方向を計算
    Vector3 forward = RotateVector(Vector3(0.0f, 0.0f, -1.0f), rotationQuat); // -Z方向が前方向
    Vector3 up = RotateVector(Vector3(0.0f, 1.0f, 0.0f), rotationQuat);       // Y方向が上方向
    Vector3 right = Cross(up, forward);                                       // 右方向は上と前の外積

    // ビュー行列の計算
    Matrix4x4 matView;
    matView.m[0][0] = right.x;
    matView.m[1][0] = right.y;
    matView.m[2][0] = right.z;
    matView.m[0][1] = up.x;
    matView.m[1][1] = up.y;
    matView.m[2][1] = up.z;
    matView.m[0][2] = -forward.x;
    matView.m[1][2] = -forward.y;
    matView.m[2][2] = -forward.z;
    matView.m[3][0] = -Dot(right, cameraPosition);
    matView.m[3][1] = -Dot(up, cameraPosition);
    matView.m[3][2] = Dot(forward, cameraPosition);
    matView.m[3][3] = 1.0f;
    return matView;
}

Matrix4x4 CreateProjectionMatrix(float fov, float aspectRatio, float nearClip, float farClip) {
    float f = 1.0f / tanf(fov * 0.5f);

    Matrix4x4 matProjection = {};
    matProjection.m[0][0] = f / aspectRatio;
    matProjection.m[1][1] = f;
    matProjection.m[2][2] = (farClip + nearClip) / (nearClip - farClip);
    matProjection.m[2][3] = -1.0f;
    matProjection.m[3][2] = (2.0f * farClip * nearClip) / (nearClip - farClip);
    return matProjection;
}

void PrintOnImGui(const Matrix4x4& matrix, const char* label) {
    ImGui::Begin("Matrix4x4");
    ImGui::Text("%s:", label);
    for (int row = 0; row < 4; ++row) {
        ImGui::Text("%.3f  %.3f  %.3f  %.3f",
            matrix.m[row][0], matrix.m[row][1], matrix.m[row][2], matrix.m[row][3]);
    }
    ImGui::End();
}