#include "function.h"

Vector3 Add(const Vector3& v1, const Vector3& v2) { return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; }

Vector3 Subtract(const Vector3& v1, const Vector3& v2) { return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}; }

Vector3 Multiply(float scalar, const Vector3& v) { return {scalar * v.x, scalar * v.y, scalar * v.z}; }

float Dot(const Vector3& v1, const Vector3& v2) { return {v1.x * v2.x + v1.y * v2.y + v1.z * v2.z}; }

float Length(const Vector3& v) {
	return {
	    sqrtf(v.x * v.x + v.y * v.y + v.z * v.z),
	};
}

Vector3 Normalize(const Vector3& v) { return {v.x / Length(v), v.y / Length(v), v.z / Length(v)}; }
float Normalize(float value) {
	// 値が0でない場合、絶対値が1になるように正規化
	if (value != 0.0f) {
		return value / fabs(value); // 符号を保ちながら1または-1に正規化
	}
	return 0.0f; // 0の時はそのまま0を返す
}

float Lerp(float a, float b, float t) { return (1.0f - t) * a + t * b; }

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) { return {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, translate.x, translate.y, translate.z, 1}; }

Matrix4x4 MakeScaleMatrix(const Vector3& scale) { return {scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0, 0, 1}; }

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

Vector3 Transformm_(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	return {m1.m[0][0] + m2.m[0][0], m1.m[0][1] + m2.m[0][1], m1.m[0][2] + m2.m[0][2], m1.m[0][3] + m2.m[0][3], m1.m[1][0] + m2.m[1][0], m1.m[1][1] + m2.m[1][1],
	        m1.m[1][2] + m2.m[1][2], m1.m[1][3] + m2.m[1][3], m1.m[2][0] + m2.m[2][0], m1.m[2][1] + m2.m[2][1], m1.m[2][2] + m2.m[2][2], m1.m[2][3] + m2.m[2][3],
	        m1.m[3][0] + m2.m[3][0], m1.m[3][1] + m2.m[3][1], m1.m[3][2] + m2.m[3][2], m1.m[3][3] + m2.m[3][3]};
}

Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	return {m1.m[0][0] - m2.m[0][0], m1.m[0][1] - m2.m[0][1], m1.m[0][2] - m2.m[0][2], m1.m[0][3] - m2.m[0][3], m1.m[1][0] - m2.m[1][0], m1.m[1][1] - m2.m[1][1],
	        m1.m[1][2] - m2.m[1][2], m1.m[1][3] - m2.m[1][3], m1.m[2][0] - m2.m[2][0], m1.m[2][1] - m2.m[2][1], m1.m[2][2] - m2.m[2][2], m1.m[2][3] - m2.m[2][3],
	        m1.m[3][0] - m2.m[3][0], m1.m[3][1] - m2.m[3][1], m1.m[3][2] - m2.m[3][2], m1.m[3][3] - m2.m[3][3]};
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			result.m[row][col] = m1.m[row][0] * m2.m[0][col] + m1.m[row][1] * m2.m[1][col] + m1.m[row][2] * m2.m[2][col] + m1.m[row][3] * m2.m[3][col];
		}
	}
	return result;
}

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

Matrix4x4 Transpose(const Matrix4x4& m) {
	return {m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0], m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1], m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2], m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]};
}

Matrix4x4 MakeIdentity4x4() { return {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}; }

Matrix4x4 MakeRotateXMatrix(float radian) { return {1, 0, 0, 0, 0, std::cosf(radian), std::sinf(radian), 0, 0, std::sinf(-radian), std::cosf(radian), 0, 0, 0, 0, 1}; };

Matrix4x4 MakeRotateYMatrix(float radian) { return {std::cosf(radian), 0, std::sinf(-radian), 0, 0, 1, 0, 0, std::sinf(radian), 0, std::cosf(radian), 0, 0, 0, 0, 1}; };

Matrix4x4 MakeRotateZMatrix(float radian) { return {std::cosf(radian), std::sinf(radian), 0, 0, std::sinf(-radian), std::cosf(radian), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}; };

Matrix4x4 MakeRotateXYZMatrix(Vector3 rotate) {
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateMatrix = Multiply(Multiply(rotateYMatrix, rotateXMatrix), rotateZMatrix);
	return rotateMatrix;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	Matrix4x4 rotateMatrix = Multiply(Multiply(rotateXMatrix, rotateYMatrix), rotateZMatrix);

	return {Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix)};
}

float cotf(float theta) { return 1.0f / std::tanf(theta); }

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	return {1.0f / aspectRatio * cotf(fovY / 2.0f), 0, 0, 0, 0, cotf(fovY / 2.0f), 0, 0, 0, 0, farClip / (farClip - nearClip), 1.0f, 0, 0, -nearClip * farClip / (farClip - nearClip), 0};
};

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	return {2.0f / (right - left),           0,   0, 0, 0, 2.0f / (top - bottom), 0, 0, 0, 0, 1.0f / (farClip - nearClip), 0, (left + right) / (left - right), (top + bottom) / (bottom - top),
	        nearClip / (nearClip - farClip), 1.0f};
};

Matrix4x4 MakeViewPortMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	return {width / 2.0f, 0, 0, 0, 0, -height / 2.0f, 0, 0, 0, 0, maxDepth - minDepth, 0, left + width / 2.0f, top + height / 2.0f, minDepth, 1.0f};
}
Vector3 Cross(const Vector3& v1, const Vector3& v2) { return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x}; };

Matrix4x4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
	Vector3 zaxis = Normalize(target - eye);     // forward
	Vector3 xaxis = Normalize(Cross(up, zaxis)); // right
	Vector3 yaxis = Cross(zaxis, xaxis);         // up

	Matrix4x4 result = {
	    xaxis.x,
	    yaxis.x,
	    zaxis.x,
	    0,
	    xaxis.y,
	    yaxis.y,
	    zaxis.y,
	    0,
	    xaxis.z,
	    yaxis.z,
	    zaxis.z,
	    0,
	    -(xaxis.x * eye.x + xaxis.y * eye.y + xaxis.z * eye.z),
	    -(yaxis.x * eye.x + yaxis.y * eye.y + yaxis.z * eye.z),
	    -(zaxis.x * eye.x + zaxis.y * eye.y + zaxis.z * eye.z),
	    1};

	return result;
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};
	return result;
}

Vector3 ExtractTranslation(const Matrix4x4& matrix) {
	Vector3 translation;
	translation.x = matrix.m[3][0];
	translation.y = matrix.m[3][1];
	translation.z = matrix.m[3][2];
	return translation;
}

Matrix4x4 ScaleMatrixFromVector3(const Vector3& scale)
{
	Matrix4x4 scaleMatrix = {};
	scaleMatrix.m[0][0] = scale.x;  // X方向のスケール
	scaleMatrix.m[1][1] = scale.y;  // Y方向のスケール
	scaleMatrix.m[2][2] = scale.z;  // Z方向のスケール
	scaleMatrix.m[3][3] = 1.0f;     // W成分は1.0

	return scaleMatrix;
}

Matrix4x4 TranslationMatrixFromVector3(const Vector3& translate)
{
	Matrix4x4 translationMatrix = {};
	translationMatrix.m[0][0] = 1.0f;  // 単位行列の成分
	translationMatrix.m[1][1] = 1.0f;  // 単位行列の成分
	translationMatrix.m[2][2] = 1.0f;  // 単位行列の成分
	translationMatrix.m[3][3] = 1.0f;  // 単位行列の成分

	translationMatrix.m[3][0] = translate.x;  // X方向の移動
	translationMatrix.m[3][1] = translate.y;  // Y方向の移動
	translationMatrix.m[3][2] = translate.z;  // Z方向の移動

	return translationMatrix;
}

Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t) {
	assert(points.size() >= 4 && "制御点は4点以上必要です");

	// 区間数は制御点の数-3
	size_t division = points.size() - 3;
	// 1区間の長さ
	float areaWidth = 1.0f / division;

	// 区間内の始点を0.0f, 終点を1.0fとした時の現在位置
	float t_2 = std::fmod(t, areaWidth) * division;
	// 加減(0.0f)と上限(1.0f)の範囲に収める
	t_2 = std::clamp(t_2, 0.0f, 1.0f);

	// 区間番号を計算し、範囲内に収める
	size_t index = std::min(static_cast<size_t>(t / areaWidth), division);

	// 4点分のインデックス
	size_t index0 = index;
	size_t index1 = index + 1;
	size_t index2 = index + 2;
	size_t index3 = index + 3;

	// 最初の区間のp0はp1を重複使用する
	if (index == 0) {
		index0 = 0;
	}
	// 最後の区間のp3はp2を重複使用する
	if (index3 >= points.size()) {
		index3 = points.size() - 1;
	}

	// 4点の座標
	const Vector3& p0 = points[index0];
	const Vector3& p1 = points[index1];
	const Vector3& p2 = points[index2];
	const Vector3& p3 = points[index3];

	return CatmullRomInterpolation(p0, p1, p2, p3, t_2);
}

Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
	const float s = 0.5f;

	float t2 = t * t;
	float t3 = t2 * t;

	Vector3 e3 = p0 * -s + p1 * 3.0f * s - p2 * 3.0f * s + p3 * s;
	Vector3 e2 = p0 * 2.0f * s - p1 * 5.0f * s + p2 * 4.0f * s - p3 * s;
	Vector3 e1 = p0 * -s + p2 * s;
	Vector3 e0 = p1 * 2.0f * s;


	return e3 * t3 + e2 * t2 + e1 * t + e0;
}
Matrix4x4 MakeRotationAxisAngle(const Vector3& axis, float angle)
{
	// 回転軸ベクトルを正規化
	Vector3 normalizedAxis = Normalize(axis);
	float x = normalizedAxis.x;
	float y = normalizedAxis.y;
	float z = normalizedAxis.z;

	// 三角関数を事前に計算
	float cosTheta = std::cos(angle);
	float sinTheta = std::sin(angle);
	float oneMinusCosTheta = 1.0f - cosTheta;

	// 回転行列を生成
	Matrix4x4 rotationMatrix;
	rotationMatrix.m[0][0] = cosTheta + x * x * oneMinusCosTheta;
	rotationMatrix.m[0][1] = x * y * oneMinusCosTheta + z * sinTheta;
	rotationMatrix.m[0][2] = x * z * oneMinusCosTheta - y * sinTheta;
	rotationMatrix.m[0][3] = 0.0f;

	rotationMatrix.m[1][0] = x * y * oneMinusCosTheta - z * sinTheta;
	rotationMatrix.m[1][1] = cosTheta + y * y * oneMinusCosTheta;
	rotationMatrix.m[1][2] = y * z * oneMinusCosTheta + x * sinTheta;
	rotationMatrix.m[1][3] = 0.0f;

	rotationMatrix.m[2][0] = z * x * oneMinusCosTheta + y * sinTheta;
	rotationMatrix.m[2][1] = z * y * oneMinusCosTheta - x * sinTheta;
	rotationMatrix.m[2][2] = cosTheta + z * z * oneMinusCosTheta;
	rotationMatrix.m[2][3] = 0.0f;

	rotationMatrix.m[3][0] = 0.0f;
	rotationMatrix.m[3][1] = 0.0f;
	rotationMatrix.m[3][2] = 0.0f;
	rotationMatrix.m[3][3] = 1.0f;

	return rotationMatrix;
}

Matrix4x4 MakeLookAtMatrix(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	// カメラの「前方向」ベクトル (Z軸)
	Vector3 zAxis = Normalize(target - eye);

	// カメラの「右方向」ベクトル (X軸)
	Vector3 xAxis = Normalize(Cross(up, zAxis));

	// カメラの「上方向」ベクトル (Y軸)
	Vector3 yAxis = Cross(zAxis, xAxis);

	// ビュー行列を生成
	Matrix4x4 viewMatrix;

	// 各軸ベクトルを行列の回転成分に設定
	viewMatrix.m[0][0] = xAxis.x; viewMatrix.m[0][1] = yAxis.x; viewMatrix.m[0][2] = zAxis.x; viewMatrix.m[0][3] = 0.0f;
	viewMatrix.m[1][0] = xAxis.y; viewMatrix.m[1][1] = yAxis.y; viewMatrix.m[1][2] = zAxis.y; viewMatrix.m[1][3] = 0.0f;
	viewMatrix.m[2][0] = xAxis.z; viewMatrix.m[2][1] = yAxis.z; viewMatrix.m[2][2] = zAxis.z; viewMatrix.m[2][3] = 0.0f;

	// カメラの位置を使って平行移動成分を設定
	viewMatrix.m[3][0] = -Dot(xAxis, eye);
	viewMatrix.m[3][1] = -Dot(yAxis, eye);
	viewMatrix.m[3][2] = -Dot(zAxis, eye);
	viewMatrix.m[3][3] = 1.0f;

	return viewMatrix;
}

Vector3 CalculateCameraRotationFromDirection(const Vector3& direction, float fixedRoll)
{
	Vector3 rotation; // 結果として得るピッチ、ヨー、ロール

	// ピッチ（X軸回転）を計算: YZ平面の方向から求める
	rotation.x = std::atan2(direction.y, std::sqrt(direction.x * direction.x + direction.z * direction.z));

	// ヨー（Y軸回転）を計算: XZ平面の方向から求める
	rotation.y = std::atan2(direction.x, direction.z);

	// ロール（Z軸回転）は固定または任意に設定
	rotation.z = fixedRoll;

	return rotation;
}

void MatrixPrintImGui(const Matrix4x4& matrix, const char* label) {
	ImGui::Begin("Matrix");
	ImGui::Text("%s:", label);
	for (int row = 0; row < 4; ++row) {
		ImGui::Text("%.3f  %.3f  %.3f  %.3f",
			matrix.m[row][0], matrix.m[row][1], matrix.m[row][2], matrix.m[row][3]);
	}
	ImGui::End();
}