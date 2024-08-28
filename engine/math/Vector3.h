#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

    // ベクトルの加算
    Vector3 operator+(const Vector3& other) const {
        return { x + other.x, y + other.y, z + other.z };
    }

    // ベクトルの減算
    Vector3 operator-(const Vector3& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }

    // ベクトルのスカラー倍
    Vector3 operator*(float scalar) const {
        return { x * scalar, y * scalar, z * scalar };
    }

    // ベクトルのスカラー除算
    Vector3 operator/(float scalar) const {
        return { x / scalar, y / scalar, z / scalar };
    }

    // ベクトルの加算代入
    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    // ベクトルの減算代入
    Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    // ベクトルのスカラー倍代入
    Vector3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    // ベクトルのスカラー除算代入
    Vector3& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    // ベクトルの等価比較
    bool operator==(const Vector3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    // ベクトルの非等価比較
    bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }
};