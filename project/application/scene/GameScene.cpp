#include <GameScene.h>
#include <TextureManager.h>
#include <ModelManager.h>
#include <ParticleManager.h>
#include <imgui.h>
#include <SceneManager.h>

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
	ModelManager::GetInstance()->LoadModel("resource", "planeReticle.obj");
	ModelManager::GetInstance()->LoadModel("resource", "cube.obj");
	ModelManager::GetInstance()->LoadModel("resource", "bullet.obj");
	ModelManager::GetInstance()->LoadModel("resource", "sphere.obj");

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

	enemyManager_ = new EnemyManager();
	enemyManager_->Initialize();

	sphere_ = new Sphere();
	sphere_->Initialize();
}

void GameScene::Finalize()
{
	railManager_->Finalize();
	delete railManager_;
	player_->Finalize();
	delete player_;
	enemyManager_->Finalize();
	delete enemyManager_;
	delete lightManager_;
	delete sphere_;
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

	enemyManager_->Update();

	sphere_->Update();

	Vector3 normalCameraPos = normalCamera_->GetTranslate();
	Vector3 normalCameraRotate = normalCamera_->GetRotate();

	normalCamera_->SetTranslate(normalCameraPos);
	normalCamera_->SetRotate(normalCameraRotate);
	CheckAllCollision();

	if (railManager_->GetTimer() >= 1.0f) {
		// シーンの切り替え依頼
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameScene::Draw()
{
	// 3Dオブジェクト描画前処理
	Object3dManager::GetInstance()->DrawSet();
	lightManager_->BindLightsToShader();
	
	sphere_->Draw();
	railManager_->Draw();
	player_->Draw();
	enemyManager_->Draw();

	SpriteManager::GetInstance()->DrawSet();

}

void GameScene::CheckAllCollision()
{
	// 判定対象AとBの座標
	Vector3 posA, posB;

	for (Enemy* enemy : enemyManager_->GetEnemy()) {
		// 敵キャラの座標
		posA = enemy->GetWorldPosition();

		// 自弾と敵キャラのすべての当たり判定
		for (PlayerBullet* bullet : player_->GetBullet()) {
			// 自弾の座標
			posB = bullet->GetWorldPosition();
			// 座標Aと座標Bの距離
			float distance = ((posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y) + (posA.z - posB.z) * (posA.z - posB.z));
			if (distance <= (0.5f + 0.5f) * (0.5f + 0.5f)) {
				// 敵キャラの衝突時にコールバックを呼び出す
				enemy->OoCollision();
				// 自弾の衝突時コールバックを行う
				bullet->OoCollision();
				score += 100;
			}
		}
	}
}
