#pragma once
#include <stdexcept>

/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];

	// デフォルトコンストラクタ
	Matrix4x4() : m{ {0.0f} } {}

	// イニシャライザリストを受け取るコンストラクタ
	Matrix4x4(std::initializer_list<float> list) {
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
	Matrix4x4 operator+(const Matrix4x4& other) const {
		Matrix4x4 result;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				result.m[i][j] = m[i][j] + other.m[i][j];
		return result;
	}

	// 行列の減算
	Matrix4x4 operator-(const Matrix4x4& other) const {
		Matrix4x4 result;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				result.m[i][j] = m[i][j] - other.m[i][j];
		return result;
	}

	// 行列の乗算
	Matrix4x4 operator*(const Matrix4x4& other) const {
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
	Matrix4x4 operator*(float scalar) const {
		Matrix4x4 result;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				result.m[i][j] = m[i][j] * scalar;
		return result;
	}

	// 加算代入
	Matrix4x4& operator+=(const Matrix4x4& other) {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				m[i][j] += other.m[i][j];
		return *this;
	}

	// 減算代入
	Matrix4x4& operator-=(const Matrix4x4& other) {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				m[i][j] -= other.m[i][j];
		return *this;
	}

	// 乗算代入
	Matrix4x4& operator*=(const Matrix4x4& other) {
		*this = *this * other;
		return *this;
	}

	// スカラー倍代入
	Matrix4x4& operator*=(float scalar) {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				m[i][j] *= scalar;
		return *this;
	}
};
