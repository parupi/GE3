#include "Object3d.h"
#include "Object3dManager.h"
#include "TextureManager.h"
//#include "imgui.h"
#include <WorldTransform.h>
#include <numbers>
//#include <Skeleton.h>
//#include <Animator.h>
#include "ModelManager.h"
Object3d::~Object3d()
{
	//delete animator_;
}

void Object3d::Initialize(const std::string& fileName)
{
	// モデルを検索してセットする
	model_ = ModelManager::GetInstance()->FindModel(fileName);

	objectManager_ = Object3dManager::GetInstance();

	//if (model_->GetModelData().isAnimation) {
	//	animator_ = new Animator();
	//	animator_->Initialize(model_, fileName);
	//	//model_->SetSkeleton(animator_->GetSkeleton());
	//}

	CreateMaterialResource();
	CreateCameraResource();
}

void Object3d::AnimationUpdate()
{
	model_->Update();
	//if (model_->GetModelData().isAnimation) {
	//	animator_->Update();
	//}
}

void Object3d::Draw(WorldTransform& worldTransform)
{
	camera_ = objectManager_->GetDefaultCamera();
	cameraData_->worldPosition = camera_->GetTranslate();

	Matrix4x4 worldViewProjectionMatrix;
	if (camera_) {
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		worldViewProjectionMatrix = worldTransform.GetMatWorld() * viewProjectionMatrix;
	}
	else {
		worldViewProjectionMatrix = worldTransform.GetMatWorld();
	}

	if (model_->GetModelData().isAnimation) {
		if (model_->GetModelData().isHasBones) {
			worldTransform.SetMapWVP(/*model_->GetModelData().rootNode.localMatrix * */worldViewProjectionMatrix);
			worldTransform.SetMapWorld(/*model_->GetModelData().rootNode.localMatrix * */worldTransform.GetMatWorld());
		}
		else {
			worldTransform.SetMapWVP(model_->GetModelData().rootNode.localMatrix * worldViewProjectionMatrix);
			worldTransform.SetMapWorld(model_->GetModelData().rootNode.localMatrix * worldTransform.GetMatWorld());
		}
	}
	else {
		worldTransform.SetMapWVP(/*model_->GetModelData().rootNode.localMatrix * */worldViewProjectionMatrix);
		worldTransform.SetMapWorld(/*model_->GetModelData().rootNode.localMatrix * */worldTransform.GetMatWorld());
	}

	// cameraの場所を指定
	objectManager_->GetDxManager()->GetCommandList()->SetGraphicsRootConstantBufferView(3, cameraResource_->GetGPUVirtualAddress());
	// マテリアルCBufferの場所を指定
	objectManager_->GetDxManager()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	// 3Dモデルが割り当てられていれば描画する
	if (model_) {
		model_->Draw(worldTransform);
	}
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
	materialData_->shininess = 20.0f;
}

void Object3d::CreateCameraResource()
{
	// カメラ用のリソースを作る
	cameraResource_ = objectManager_->GetDxManager()->CreateBufferResource(sizeof(CameraForGPU));
	// 書き込むためのアドレスを取得
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
	// 初期値を入れる
	cameraData_->worldPosition = { 1.0f, 1.0f, 1.0f };
}

void Object3d::SetModel(const std::string& filePath)
{
	// モデルを検索してセットする
	model_ = ModelManager::GetInstance()->FindModel(filePath);
}
