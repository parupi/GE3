#include "Vector3.h"
#include <imgui.h>
#include <Quaternion.h>

// ベクトルの加算
Vector3 Vector3::operator+(const Vector3& other) const {
	return { x + other.x, y + other.y, z + other.z };
}

// ベクトルの減算
Vector3 Vector3::operator-(const Vector3& other) const {
	return { x - other.x, y - other.y, z - other.z };
}

// ベクトルのスカラー倍
Vector3 Vector3::operator*(float scalar) const {
	return { x * scalar, y * scalar, z * scalar };
}

// ベクトルのスカラー除算
Vector3 Vector3::operator/(float scalar) const {
	return { x / scalar, y / scalar, z / scalar };
}

// ベクトルの加算代入
Vector3& Vector3::operator+=(const Vector3& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

// ベクトルの減算代入
Vector3& Vector3::operator-=(const Vector3& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

// ベクトルのスカラー倍代入
Vector3& Vector3::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

// ベクトルのスカラー除算代入
Vector3& Vector3::operator/=(float scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

// ベクトルの等価比較
bool Vector3::operator==(const Vector3& other) const {
	return x == other.x && y == other.y && z == other.z;
}

// ベクトルの非等価比較
bool Vector3::operator!=(const Vector3& other) const {
	return !(*this == other);
}

// ヘルパー関数
float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Length(const Vector3& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Normalize(const Vector3& v) {
	float len = Length(v);
	return { v.x / len, v.y / len, v.z / len };
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion) {
	Quaternion vecQuat(vector.x, vector.y, vector.z, 0.0f); // ベクトルをクォータニオンに変換
	Quaternion conjugateQuat = Conjugate(quaternion);

	// クォータニオンの積で回転を適用
	Quaternion rotatedQuat = quaternion * vecQuat * conjugateQuat;

	// 回転後のベクトルを返す
	return Vector3(rotatedQuat.x, rotatedQuat.y, rotatedQuat.z);
}

// クォータニオンからオイラー角に変換する関数
Vector3 QuaternionToEuler(const Quaternion& q) {
	Vector3 euler;

	// Roll (x-axis rotation)
	float sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
	float cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
	euler.x = std::atan2(sinr_cosp, cosr_cosp);

	// Pitch (y-axis rotation)
	float sinp = 2.0f * (q.w * q.y - q.z * q.x);
	if (std::abs(sinp) >= 1)
		euler.y = std::copysign(3.14 / 2, sinp); // Use 90 degrees if out of range
	else
		euler.y = std::asin(sinp);

	// Yaw (z-axis rotation)
	float siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
	float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
	euler.z = std::atan2(siny_cosp, cosy_cosp);

	return euler;
}

Vector3 Lerp(const Vector3& start, const Vector3& end, float t) {
	return start * (1.0f - t) + end * t;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	return { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
}

void PrintOnImGui(const Vector3& v, const char* label)
{
	ImGui::Begin(label);
	ImGui::Text("%s: (x: %.2f, y: %.2f, z: %.2f)", "Vector3", v.x, v.y, v.z);
	ImGui::End();
}
