#include "Object3d.h"
#include "Object3dManager.h"

#include "TextureManager.h"
#include "imgui.h"
#include <WorldTransform.h>

void Object3d::Initialize()
{
	objectManager_ = Object3dManager::GetInstance();

	CreateWVPResource();
	CreateDirectionalLightResource();
	CreateMaterialResource();	
}

void Object3d::Draw(WorldTransform& worldTransform)
{
	camera_ = objectManager_->GetDefaultCamera();
	Matrix4x4 worldViewProjectionMatrix;
	if (camera_) {
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		worldViewProjectionMatrix = worldTransform.GetMatWorld() * viewProjectionMatrix;
	}
	else {
		worldViewProjectionMatrix = worldTransform.GetMatWorld();
	}
	worldTransform.SetMapWVP(worldViewProjectionMatrix);

	// wvp用のCBufferの場所を設定
	objectManager_->GetDxManager()->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.GetConstBuffer()->GetGPUVirtualAddress());
	// directionalLightの場所を指定
	objectManager_->GetDxManager()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());
	// マテリアルCBufferの場所を指定
	objectManager_->GetDxManager()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	// 3Dモデルが割り当てられていれば描画する
	if (model_){
		model_->Draw();
	}
}

void Object3d::CreateDirectionalLightResource()
{
	// 平行光源用のリソースを作る
	directionalLightResource_ = objectManager_->GetDxManager()->CreateBufferResource(sizeof(DirectionalLight));
	// 書き込むためのアドレスを取得
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	// 今回は白を書き込んで置く
	directionalLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLightData_->direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData_->intensity = 1.0f;
}

void Object3d::CreateMaterialResource()
{
	// マテリアル用のリソースを作る。今回はFcolor1つ分のサイズを用意する
	materialResource_ = objectManager_->GetDxManager()->CreateBufferResource(sizeof(Material));
	// 書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// 白を入れる
	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();
}

void Object3d::SetModel(const std::string& filePath)
{
	// モデルを検索してセットする
	model_ = ModelManager::GetInstance()->FindModel(filePath);
}
