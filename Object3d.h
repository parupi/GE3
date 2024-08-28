#pragma once
#include "Object3dManager.h"
#include "math/Vector3.h"
#include "math/Vector2.h"
#include "math/Vector4.h"
#include "math/Matrix4x4.h"
#include "math/function.h"
#include <fstream>
#include "Model.h"
class Object3dManager;

class Object3d
{
public: // メンバ関数
	void Initialize(Object3dManager* objectManager);
	void Update();
	void Draw();
private:
	void CreateWVPResource();
	void CreateDirectionalLightResource();
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
private: // メンバ変数
	Object3dManager* objectManager_ = nullptr;
	Model* model_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;

	TransformationMatrix* wvpData_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;

	Transform transform_;
	Transform cameraTransform_;
public: // ゲッター // セッター // 
	void SetModel(Model* model) { model_ = model; }

};

