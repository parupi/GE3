#pragma once
#include <function.h>
#include <WindowManager.h>
class Camera
{
public: // メンバ関数
	Camera();

	// 更新
	void Update();

	// フォローカメラ
	void FollowCamera(const Vector3& target);

private:
	Transform transform_;
	Matrix4x4 worldMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 worldViewProjectionMatrix_;
	// 水平方向視野角
	float horizontalFOV_;
	// アスペクト比
	float aspectRatio_;
	// ニアクリップ距離
	float nearClip_;
	// ファークリップ距離
	float farClip_;

	Vector3 followCameraOffsetPosition_ = Vector3(0.0f, 30.0f, -50.0f);

	Vector3 followCameraOffsetRotare_ = Vector3(0.51f, 0.0f, 0.0f);

	// 行列の更新
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

public: // ゲッター // セッター //
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
	const Vector3& GetRotate() const { return transform_.rotate; }

	void SetTranslate(const Vector3& translate) { transform_.translate = translate; }
	const Vector3& GetTranslate() const { return transform_.translate; }

	void SetFovY(const float& horizontalFOV) { horizontalFOV_ = horizontalFOV; UpdateProjectionMatrix(); }
	void SetAspectRate(const float& aspectRatio) { aspectRatio_ = aspectRatio; UpdateProjectionMatrix(); }
	void SetNearClip(const float& nearClip) { nearClip_ = nearClip; UpdateProjectionMatrix(); }
	void SetFarClip(const float& farClip) { farClip_ = farClip; UpdateProjectionMatrix(); }

	// 行列の取得関数
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }
	const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }
	const Matrix4x4& GetViewProjectionMatrix() const { return worldViewProjectionMatrix_; }
};

