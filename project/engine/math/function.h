#pragma once
#include <Vector3.h>
#include "assert.h"
#include "cmath"
#include <Matrix4x4.h>
#include <imgui.h>
#include <vector>
#include <algorithm>

static const int kColumnWidth = 60;
static const int kRowHeight = 20;

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

//// 内積
//float Dot(const Vector3& v1, const Vector3& v2);
//
//// 長さ（ノルム）
//float Length(const Vector3& v);
//Vector3 Normalize(const Vector3& v);

// 正規化
float Normalize(float value);

float Lerp(float a, float b, float t);

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// 座標変換
//Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
Vector3 TransformFunc(const Vector3& vector, const Matrix4x4& matrix);

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

//Matrix4x4 ScaleMatrixFromVector3(const Vector3& scale);
//
//Matrix4x4 TranslationMatrixFromVector3(const Vector3& translate);

//Matrix4x4 MakeRotaeAxisAngle(const Vector3& axis, float angle);

// ある方向からある方向へ向けるための回転行列を作成する関数
Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);

Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t);
Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);