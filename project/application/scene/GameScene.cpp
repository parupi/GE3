#include <GameScene.h>
#include <TextureManager.h>
#include <ModelManager.h>
#include <ParticleManager.h>
#include <imgui.h>

void GameScene::Initialize()
{
	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->Initialize();

	player_ = std::make_unique<Player>();
	player_->Initialize();

	gameCamera_->SetPlayer(player_.get());
	player_->SetCamera(gameCamera_->GetGameCamera());

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();


}

void GameScene::Finalize()
{

}

void GameScene::Update()
{
	gameCamera_->Update();

	player_->Update();

	enemy_->Update();
}

void GameScene::Draw()
{
	// 3Dオブジェクト描画前処理
	Object3dManager::GetInstance()->DrawSet();

	player_->Draw();
	enemy_->Draw();

}