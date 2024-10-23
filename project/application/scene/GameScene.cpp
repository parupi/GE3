#include <GameScene.h>
#include <TextureManager.h>
#include <ModelManager.h>
#include <ParticleManager.h>
#include <imgui.h>

void GameScene::Initialize()
{
	// カメラの生成
	normalCamera_ = std::make_shared<Camera>();
	bossCamera_ = std::make_shared<Camera>();
	cameraManager_.AddCamera(normalCamera_);
	cameraManager_.AddCamera(bossCamera_);
	cameraManager_.SetActiveCamera(0);
	normalCamera_->SetTranslate(Vector3{ 0.0f, 10.0f, -30.0f });
	bossCamera_->SetTranslate(Vector3{ 0.0f, 0.0f, -100.0f });

	// .objファイルからモデルを読み込む
	ModelManager::GetInstance()->LoadModel("plane.obj");

	object_ = new Object3d();
	object_->Initialize();
	object_->SetModel("plane.obj");


}

void GameScene::Finalize()
{

}

void GameScene::Update()
{
	cameraManager_.Update();

	Vector3 normalCameraPos = normalCamera_->GetTranslate();
	Vector3 bossCameraPos = bossCamera_->GetTranslate();

	ImGui::Begin("Camera Manager");
	ImGui::DragFloat3("normalPos", &normalCameraPos.x, 0.01f);
	ImGui::DragFloat3("bossPos", &bossCameraPos.x, 0.01f);

	if (ImGui::Button("Set Camera 1"))
	{
		cameraManager_.SetActiveCamera(0);
	}
	if (ImGui::Button("Set Camera 2"))
	{
		cameraManager_.SetActiveCamera(1);
	}
	ImGui::End();

	normalCameraPos += {0.0f, 0.0f, 0.01f};

	normalCamera_->SetTranslate(normalCameraPos);
	bossCamera_->SetTranslate(bossCameraPos);

	//object_->Update();
}

void GameScene::Draw()
{
	// 3Dオブジェクト描画前処理
	Object3dManager::GetInstance()->DrawSet();

	//object_->Draw();
}