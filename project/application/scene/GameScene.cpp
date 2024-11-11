#include <GameScene.h>
#include <TextureManager.h>
#include <ModelManager.h>
#include <ParticleManager.h>
#include <imgui.h>

void GameScene::Initialize()
{
	// ライトマネージャの生成
	lightManager_ = std::make_unique<LightManager>();
	lightManager_->Initialize();

	lightManager_->SetDirLightActive(0, true);
	lightManager_->SetDirLightIntensity(0, 1.0f);


	// .objファイルからモデルを読み込む
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("ICO.obj");
	railManager_ = std::make_unique<RailManager>();
	railManager_->Initialize();
}

void GameScene::Finalize()
{
	railManager_->Finalize();

}

void GameScene::Update()
{


	railManager_->Update();
	worldTransform_.TransferMatrix();
}

void GameScene::Draw()
{
	// 3Dオブジェクト描画前処理
	Object3dManager::GetInstance()->DrawSet();
	lightManager_->BindLightsToShader();

	railManager_->Draw();
}