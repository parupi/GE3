#include "Object3d.h"
#include "Object3dManager.h"

#include "TextureManager.h"
#include "imgui.h"

void Object3d::Initialize(Object3dManager* objectManager)
{
	objectManager_ = objectManager;

	CreateWVPResource();
	CreateDirectionalLightResource();

	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	
	camera_ = objectManager_->GetDefaultCamera();
}

void Object3d::Update()
{
	transform_.scale = model_->GetSize();
	transform_.rotate = model_->GetRotation();
	transform_.translate = model_->GetPosition();

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 worldViewProjectionMatrix;
	if (camera_) {
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		worldViewProjectionMatrix = worldMatrix * viewProjectionMatrix;
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}
	wvpData_->WVP = worldViewProjectionMatrix;
	wvpData_->World = worldMatrix;
	
}

void Object3d::Draw()
{
	// wvp用のCBufferの場所を設定
	objectManager_->GetDxManager()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	// directionalLightの場所を指定
	objectManager_->GetDxManager()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());
	// 3Dモデルが割り当てられていれば描画する
	if (model_){
		model_->Draw();
	}
}

void Object3d::CreateWVPResource()
{
	// MVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	wvpResource_ = objectManager_->GetDxManager()->CreateBufferResource(sizeof(TransformationMatrix));
	// 書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	// 単位行列を書き込んでおく
	wvpData_->World = MakeIdentity4x4();
	wvpData_->WVP = MakeIdentity4x4();
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

void Object3d::SetModel(const std::string& filePath)
{
	// モデルを検索してセットする
	model_ = ModelManager::GetInstance()->FindModel(filePath);
}
