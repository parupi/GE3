#pragma once
#include <Vector3.h>
#include "assert.h"
#include "cmath"
#include <Matrix4x4.h>

static const int kColumnWidth = 60;
static const int kRowHeight = 20;

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);

// 内積
float Dot(const Vector3& v1, const Vector3& v2);

// 長さ（ノルム）
float Length(const Vector3& v);

// 正規化
Vector3 Normalize(const Vector3& v);

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// 座標変換
//Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

// 行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

// 行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m);

// 単位行列の作成
Matrix4x4 MakeIdentity4x4();

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸回転行列							
Matrix4x4 MakeRotateYMatrix(float radian);
// Z軸回転行列							
Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 MakeRotateXYZMatrix(Vector3 rotate);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// tanθの逆数
float cotf(float theta);

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

// ビューポート変換行列
Matrix4x4 MakeViewPortMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

//// デバッグ用
//void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);
//void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

Matrix4x4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

Vector3 ExtractTranslation(const Matrix4x4& matrix);
