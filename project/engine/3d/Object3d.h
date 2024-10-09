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

	Transform transform_;
public: // ゲッター // セッター //
	// 平行移動
	const Vector3& GetPosition() const { return transform_.translate; }
	void SetPosition(const Vector3& position) { transform_.translate = position; }
	// 回転
	const Vector3 GetRotation() const { return transform_.rotate; }
	void SetRotation(Vector3 rotation) { transform_.rotate = rotation; }
	// 拡縮
	const Vector3& GetSize() const { return transform_.scale; }
	void SetSize(const Vector3& size) { transform_.scale = size; }
	// モデル
	void SetModel(Model* model) { model_ = model; }
	void SetModel(const std::string& filePath);
	// カメラ
	void SetCamera(Camera* camera) { camera_ = camera; }
	// 色
	const Vector4& GetColor() const { return materialData_->color; }
	void SetColor(const Vector4& color) { materialData_->color = color; }
	
};

