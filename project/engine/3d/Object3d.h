#pragma once
#include "Object3dManager.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "function.h"
#include <fstream>
#include "Model.h"
#include "ModelManager.h"
#include <Camera.h>
class Object3dManager;

class Object3d
{
public: // メンバ関数
	Object3d() = default;
	~Object3d() = default;
	void Initialize();
	void Update();
	void Draw();
private:
	void CreateWVPResource();
	void CreateDirectionalLightResource();
	void CreateMaterialResource();
private: // 構造体
	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	struct DirectionalLight {
		Vector4 color;		//!< ライトの色
		Vector3 direction;	//!< ライトの向き
		float intensity;	//!< 輝度
	};

	struct Material {
		Vector4 color;
		bool enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};
private: // メンバ変数
	Object3dManager* objectManager_ = nullptr;
	Model* model_ = nullptr;
	Camera* camera_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	TransformationMatrix* wvpData_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;
	Material* materialData_ = nullptr;

	// コピー禁止
	Object3d(const Object3d&) = delete;
	Object3d& operator=(const Object3d&) = delete;
public: // 汎用化変数
	// ローカルスケール
	Vector3 scale_ = { 1, 1, 1 };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0, 0, 0 };
	// ローカル座標
	Vector3 translation_ = { 0, 0, 0 };
	// ローカル → ワールド変換行列
	Matrix4x4 matWorld_;
	// 親となるワールド変換へのポインタ
	const Object3d* parent_ = nullptr;
public: // ゲッター // セッター // 
	// モデル
	void SetModel(Model* model) { model_ = model; }
	void SetModel(const std::string& filePath);
	// カメラ
	void SetCamera(Camera* camera) { camera_ = camera; }

	// 平行移動
	const Vector3& GetTranslate() const { return translation_; }
	void SetTranslate(const Vector3& translation) { translation_ = translation; }
	// 回転
	const Vector3 GetRotation() const { return rotation_; }
	void SetRotation(Vector3 rotation) { rotation_ = rotation; }
	// 拡縮
	const Vector3& GetScale() const { return scale_; }
	void SetScale(const Vector3& size) { scale_ = size; }
	// 色
	const Vector4& GetColor() const { return materialData_->color; }
	void SetColor(const Vector4& color) { materialData_->color = color; }
	// Lighting
	const bool& GetIsLighting() const { return materialData_->enableLighting; }
	void SetIsLighting(const bool isLighting) { materialData_->enableLighting = isLighting; }
};

static_assert(!std::is_copy_assignable_v<Object3d>);