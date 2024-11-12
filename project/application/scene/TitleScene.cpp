#include "TitleScene.h"
#include <Object3dManager.h>
#include <Input.h>
#include <GameScene.h>

void TitleScene::Initialize()
{
	// カメラの生成
	camera_ = std::make_unique<Camera>();
	Object3dManager::GetInstance()->SetDefaultCamera(camera_.get());
	TextureManager::GetInstance()->LoadTexture("resource/title.png");
	titleBGSprite_ = new Sprite();
	titleBGSprite_->Initialize("resource/title.png");
	titleBGSprite_->SetSize(Vector2{ 1280.0f,720.0f });
	titleBGSprite_->SetTextureSize(Vector2{ 1280,720 });
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// シーンの切り替え依頼
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
	titleBGSprite_->Update();
}

void TitleScene::Draw()
{
	SpriteManager::GetInstance()->DrawSet();
	titleBGSprite_->Draw();
}
