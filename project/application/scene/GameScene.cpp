#include <GameScene.h>
#include <TextureManager.h>
#include <ModelManager.h>
#include <ParticleManager.h>
#include <imgui.h>

void GameScene::Initialize()
{
	// カメラの生成
	normalCamera_ = std::make_shared<Camera>();
	railCamera_ = std::make_shared<Camera>();
	cameraManager_.AddCamera(normalCamera_);
	cameraManager_.AddCamera(railCamera_);
	cameraManager_.SetActiveCamera(1);
	normalCamera_->SetTranslate(Vector3{ 0.0f, 10.0f, -30.0f });
	railCamera_->SetTranslate(Vector3{ 0.0f, 0.0f, -100.0f });

	// .objファイルからモデルを読み込む
	ModelManager::GetInstance()->LoadModel("resource", "plane.obj");

	lightManager_ = new LightManager();
	lightManager_->Initialize();

	lightManager_->SetDirLightActive(0, true);
	lightManager_->SetDirLightIntensity(0, 1.0f);

	railManager_ = new RailManager();
	railManager_->Initialize();
	railCameraTransform_.Initialize();

	player_ = new Player();
	player_->Initialize();
	player_->SetParent(&railCameraTransform_);
}

void GameScene::Finalize()
{
	railManager_->Finalize();
	delete railManager_;
	player_->Finalize();
	delete player_;
	delete lightManager_;
}

void GameScene::Update()
{
	cameraManager_.Update();

	railManager_->Update();

	railCamera_->SetTranslate(railManager_->GetCameraTranslate());
	railCamera_->SetRotate(railManager_->GetCameraRotate());
	railCameraTransform_.translation_ = railManager_->GetCameraTranslate();
	railCameraTransform_.rotation_ = railManager_->GetCameraRotate();
	railCameraTransform_.TransferMatrix();

	player_->SetOffset(railManager_->GetOffset());
	player_->SetCameraRotate(railManager_->GetCameraRotate());
	player_->Update();

	Vector3 normalCameraPos = normalCamera_->GetTranslate();
	Vector3 normalCameraRotate = normalCamera_->GetRotate();
	ImGui::Begin("Camera Manager");
	ImGui::DragFloat3("normalPos", &normalCameraPos.x, 0.01f);
	ImGui::DragFloat3("normalRotate", &normalCameraRotate.x, 0.01f);
	if (ImGui::Button("Set Camera 1"))
	{
		cameraManager_.SetActiveCamera(0);
	}
	if (ImGui::Button("Set Camera 2"))
	{
		cameraManager_.SetActiveCamera(1);
	}
	ImGui::Text("%f %f %f", railManager_->GetCameraTranslate().x, railManager_->GetCameraTranslate().y, railManager_->GetCameraTranslate().z);
	ImGui::Text("%f %f %f", railManager_->GetCameraRotate().x, railManager_->GetCameraRotate().y, railManager_->GetCameraRotate().z);
	ImGui::End();
	normalCamera_->SetTranslate(normalCameraPos);
	normalCamera_->SetRotate(normalCameraRotate);
}

void GameScene::Draw()
{
	// 3Dオブジェクト描画前処理
	Object3dManager::GetInstance()->DrawSet();
	lightManager_->BindLightsToShader();
	
	railManager_->Draw();
	player_->Draw();

	SpriteManager::GetInstance()->DrawSet();
	player_->DrawUI();
}