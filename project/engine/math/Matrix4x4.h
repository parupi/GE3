#pragma once
#include <stdexcept>
#include <initializer_list>
#include <cmath>

struct Vector3;
struct Quaternion;

/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
    float m[4][4];

    // デフォルトコンストラクタ
    Matrix4x4();

    // イニシャライザリストを受け取るコンストラクタ
    Matrix4x4(std::initializer_list<float> list);

    // 行列の加算
    Matrix4x4 operator+(const Matrix4x4& other) const;
    // 行列の減算
    Matrix4x4 operator-(const Matrix4x4& other) const;
    // 行列の乗算
    Matrix4x4 operator*(const Matrix4x4& other) const;
    // スカラー倍
    Matrix4x4 operator*(float scalar) const;

    // 加算代入
    Matrix4x4& operator+=(const Matrix4x4& other);
    // 減算代入
    Matrix4x4& operator-=(const Matrix4x4& other);
    // 乗算代入
    Matrix4x4& operator*=(const Matrix4x4& other);
    // スカラー倍代入
    Matrix4x4& operator*=(float scalar);
};

// 行列の逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

// 行列の転置
Matrix4x4 Transpose(const Matrix4x4& m);

// 単位行列を作成
Matrix4x4 MakeIdentity4x4();

// X軸回転行列を作成
Matrix4x4 MakeRotateXMatrix(float radian);

// Y軸回転行列を作成
Matrix4x4 MakeRotateYMatrix(float radian);

// Z軸回転行列を作成
Matrix4x4 MakeRotateZMatrix(float radian);

// 3x3行列式を計算するヘルパー関数
float Determinant3x3(float a, float b, float c, float d, float e, float f, float g, float h, float i);

// 4x4行列の行列式を計算する関数
float Determinant(const Matrix4x4& m);

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

void PrintOnImGui(const Matrix4x4& matrix, const char* label = "Matrix");